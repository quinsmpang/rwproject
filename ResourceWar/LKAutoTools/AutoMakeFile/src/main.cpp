#include <iostream>  
#include <io.h>  
#include <direct.h>  
#include <string>  
#include <vector>  
#include <iomanip>  
#include <ctime>  
#include "utils.h"
using namespace std;

#define SERVER_PATH "../../undeadServer/GameServer"
#define SERVER_AUTOS_PATH	"../../undeadServer/GameServer/AutoS/src"
#define SERVER_SRC_PATH	"../../undeadServer/GameServer/src"

char *remove_suffix( char *file ) 
{
	char *last_dot = strrchr( file, '.' );
	if ( last_dot != NULL && strrchr( file, '\\' ) < last_dot )
		*last_dot = '\0';
	return file;
}

void getFiles( string path, vector<string>& files ) 
{
	//文件句柄    
	long   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;
	string p;
	if ( (hFile = _findfirst( p.assign( path ).append( "/*" ).c_str( ), &fileinfo )) != -1 )  
	{
		do  
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ( (fileinfo.attrib   &   _A_SUBDIR) ) 
			{
				if ( strcmp( fileinfo.name, "." ) != 0 && strcmp( fileinfo.name, ".." ) != 0 )
					getFiles( p.assign( path ).append( "/" ).append( fileinfo.name ), files );
			}
			else  
			{
				char *ext = u_suffix( fileinfo.name );
				if ( ext && (strcmp( ext, "cpp" ) == 0 || strcmp( ext, "c" ) == 0) )
					files.push_back( fileinfo.name );
			}
		} 
		while ( _findnext( hFile, &fileinfo ) == 0 );

		_findclose( hFile );
	}
}

void getFilesChild( string path, vector<string>& files )
{
	//文件句柄    
	long   hFile = 0;
	//文件信息    
	struct _finddata_t fileinfo;
	string p;
	if ( (hFile = _findfirst( p.assign( path ).append( "/*" ).c_str( ), &fileinfo )) != -1 )
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ( (fileinfo.attrib   &   _A_SUBDIR) )
			{
				if ( strcmp( fileinfo.name, "." ) != 0 && strcmp( fileinfo.name, ".." ) != 0 )
					getFiles( p.assign( path ).append( "/" ).append( fileinfo.name ), files );
			}
			else
			{
				char *ext = u_suffix( fileinfo.name );
				if ( ext && (strcmp( ext, "cpp" ) == 0 || strcmp( ext, "c" ) == 0) )
					files.push_back( p.assign( path ).append( "/" ).append( fileinfo.name ) );
			}
		} while ( _findnext( hFile, &fileinfo ) == 0 );

		_findclose( hFile );
	}
}

void main( int argc, char *argv[] )
{
	vector<string>   autos_files;
	vector<string>   src_files;
	getFiles( SERVER_AUTOS_PATH, autos_files );
	getFiles( SERVER_SRC_PATH, src_files );

	char szFile[256] = { 0 };
	sprintf( szFile, "%s/Makefile", SERVER_SRC_PATH );

	FILE *fp;
	fp = fopen( szFile, "wb" );
	if ( fp == NULL )
		return;
	fprintf( fp, "CC=g++\r\n" );
	fprintf( fp, "SRC = \\\r\n" );
	for ( int j = 0; j < autos_files.size( ); ++j )
	{
		fprintf( fp, "../AutoS/src/%s \\\r\n", autos_files[j].c_str( ) );
	}
	for ( int j = 0; j < src_files.size( ); ++j )
	{
		fprintf( fp, "%s \\\r\n", src_files[j].c_str( ) );
	}
	fprintf( fp, "\r\n\r\n");
	fprintf( fp, "INCLUDE = -I ../include  -I ../AutoS/include -I  ../../LKUtils/include  -I ../../LKNetServer/include  -I ../../Lua/src  -I /usr/local/mysql/include/mysql\r\n" );
	fprintf( fp, "LIBRARY = -L ../../../public/lib -L /usr/local/mysql/lib/mysql\r\n" );
	fprintf( fp, "OBJ = $(SRC:.cpp=.o)\r\n" );
	fprintf( fp, "LIB = -llua -llknet -llkutils -lmysqlclient -lpthread -ldl -lm\r\n" );
	fprintf( fp, "EXE = ../../../exec/undeadServer\r\n" );
	fprintf( fp, "CFLAGS = -g\r\n" );
	fprintf( fp, "$(EXE) : $(OBJ)\r\n" );
	fprintf( fp, "\t$(CC)  $(CFLAGS) $(INCLUDE) $(LIBRARY) -o $(EXE) $(CF) $(OBJ) $(LIB)\r\n" );

	fprintf( fp, "\r\n\r\n" );
	for ( int j = 0; j < autos_files.size(); ++j )
	{
		remove_suffix( (char*)autos_files[j].c_str() );
		fprintf( fp, "../AutoS/src/%s.o: ../AutoS/src/%s.cpp \r\n", autos_files[j].c_str(), autos_files[j].c_str() );
		fprintf( fp, "\t$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@\r\n" );
	}
	for ( int j = 0; j < src_files.size( ); ++j )
	{
		remove_suffix( (char*)src_files[j].c_str() );
		fprintf( fp, "%s.o: %s.cpp \r\n", src_files[j].c_str(), src_files[j].c_str() );
		fprintf( fp, "\t$(CC) $(CFLAGS) -c $(INCLUDE) $<\r\n" );
	}

	fprintf( fp, "\r\n\r\n" );
	fprintf( fp, "all: $(EXE)\r\n" );
	fprintf( fp, "clean :\r\n" );
	fprintf( fp, "\trm -f *.o\r\n" );
	fprintf( fp, "\trm -f ../AutoS/src/*.o\r\n" );
	fprintf( fp, "\trm -f $(EXE)\r\n" );
}