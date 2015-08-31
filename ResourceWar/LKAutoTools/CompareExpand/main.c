//=============================================================================
#include "compare.h"
#include <Windows.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

#define ExpandOutFile "fpExpand.txt"
#define NowOutFile  "fpNow.txt"
/* 扩展包资源路径 */
char g_ExpandResPath[MAX_PATH]={0};
/* 当前修改过的版本资源路径 */
char g_NowVersionResPath[MAX_PATH]={0};
/* 应用程序工作的目录 */
char g_WorkPath[MAX_PATH]={0};
/* 文件个数，用于提示 */
int g_FileCount = 0;
/* 文件句柄 */
FILE *g_Expandfp = NULL;
FILE *g_Nowfp = NULL;
/* 判断路径的合法性:去掉"*",因为DispSubFiles()中路径已包含"*" */
char *ResetFilePath(char *path)
{
	int len = 0;
	int tmpi = 0;
	char *temp = NULL; //返回合法路径
	len = strlen(path);//输入的路径长度
	temp = (char *)malloc(len*sizeof(char));
	for( tmpi = 0; tmpi < len; tmpi++ )
	{
		temp[tmpi]=path[tmpi];
	}
	temp[tmpi] = '\0';
	if( len == 1 )
	{
		strcat( temp, ":\\" );
		return temp;
	}
	else if( ( len==2 ) || ( len==3 ) )
	{
		if( temp[1]==':' )
		{
			strcat( temp, "\\" );
			return temp;
		}
		else
		{
			temp[1] = ':';
			temp[2] = '\0';
			strcat( temp, "\\" );
			return temp;
		}
	}
	else 
	{
		if( temp[len-1] == '.' )
		{
			if( temp[len-2]=='\\' )
			{
				temp[len-1] = '\0';
				return temp;
			}
			else
			{
				temp[len-1] = '\0';
				return temp;
			}
		}
		else if( temp[len-1]=='*' )
		{
			if( temp[len-2]=='\\' )
			{
				temp[len-1] = '\0';
				return temp;
			}
			else if( temp[len-2]=='.' )
			{
				if( temp[len-3]=='*' )
				{
					temp[len-3] = '\0';
					return temp;
				}
				else
				{
					temp[len-2] = '\0';
					return temp;
				}
			}
			else
			{
				temp[len-1] = '\0';
				return temp;
			}
		}
		else
			return temp;
	}
}

/* 递归遍历目录，包括子目录 */
void IteratesFiles( FILE* fp)
{
	long handle;
	char path[MAX_PATH] = {0};
	struct _finddata_t t;
	handle=_findfirst("*",&t);
	if(handle==-1)
	{// 如果handle=-1,表示目录不存在
		return ;
	}
	if(t.attrib & _A_SUBDIR)
	{// 1. 判断第一个文件:如果为目录
		if(t.name[0]!='.')
		{// 如果目录非空
			_chdir(t.name);
			_getcwd(path,MAX_PATH);
			//puts(path);
			IteratesFiles(fp);// 递归调用
			_chdir("..");  // 当所有文件遍历完毕,返回上级目录
		}
		//如果为空目录,即t.name为"."或"..",则不输出
	}
	else
	{// 如果第一个是文件，不为目录，目前不太可能这么设定
		_getcwd(path,MAX_PATH);
		strcat(path,"\\");
		strcat(path,t.name);
		//puts(path);
	}
	while( !( _findnext(handle,&t) ) )//2. 判断下一个文件,直至遍历所有文件
	{ 
		if(t.attrib & _A_SUBDIR)//如果第一个文件为目录
		{
			if(t.name[0]!='.')//如果目录非空
			{
				_chdir(t.name);
				_getcwd(path,MAX_PATH);
				//puts(path);
				IteratesFiles(fp);//递归调用
				_chdir("..");//当所有文件遍历完毕,返回上级目录
			}
			//如果为空目录,即t.name为"."或"..",则不输出
		}
		else
		{//如果是文件，不为目录,显示文件完整路径
			if ( (t.attrib & _A_HIDDEN) )
			{ // 忽略隐藏的文件
			}
			else
			{
				_getcwd(path,MAX_PATH);// 获取文件路径
				strcat(path,"\\");
				strcat(path,t.name);// 文件完整路径

				//puts(path);// 输出路径
				fprintf( fp, "%s\n", path );
				g_FileCount+=1;
			}
		}
	}
	_findclose(handle);
}

/* 上一个版本的完整资源路径输出到文件 */
int ExpandResPathOutFile( char *pathname )
{
	char szMsg[64]={0};
	char dirc[MAX_PATH]={0};

	sprintf( dirc, "%s\\", pathname );
	_chdir(ResetFilePath(dirc));// 打开指定目录
	IteratesFiles(g_Expandfp);			// 遍历所有目录及文件

	sprintf( szMsg, "Expand包资源数量：%d\n", g_FileCount );
	puts(szMsg);
	g_FileCount = 0;
	return 0;
}

/* 当前版本的完整资源路径输出到文件 */
int NowVersionResPathOutFile( char *pathname )
{
	char szMsg[64]={0};
	char dirc[MAX_PATH]={0};

	sprintf( dirc, "%s\\", pathname );
	_chdir(ResetFilePath(dirc));// 打开指定目录
	IteratesFiles(g_Nowfp);			// 遍历所有目录及文件

	sprintf( szMsg, "当前修改过的版本资源数量：%d\n", g_FileCount );
	puts(szMsg);
	g_FileCount = 0;
	return 0;
}

/* 创建补丁 */
int CreatePatch()
{
	FILE *fp=NULL;
	int tmpi;
	char *pbuf;
	char *ptr,*p;
	int filesize;
	int bufsize;
	char fileName[MAX_PATH]={0};
	char fromName[MAX_PATH]={0};
	char toName[MAX_PATH]={0};
	int CompRet = 0;

	fp = fopen( NowOutFile, "rb" );
	if( fp == NULL )
	{
		printf( "%s打开失败，错误码：%d\n",NowOutFile, GetLastError());
		return -1;
	}
	fseek( fp, 0, SEEK_END );
	filesize = ftell( fp );
	bufsize = filesize;
	if( bufsize <= 0 )
	{
		fclose( fp );
		return -1;
	}
	pbuf = (char *)malloc( bufsize );
	if( pbuf == NULL )
	{
		fclose( fp );
		return -1;
	}
	fseek( fp, 0, SEEK_SET );
	if( fread( pbuf, 1, bufsize, fp ) != bufsize )
	{
		fclose( fp );
		return -1;
	}
	fclose( fp );

	// 创建补丁资源文件夹
	my_mkdir("Resources");

	ptr = pbuf;
	for( tmpi = 0; tmpi < bufsize; tmpi++ )
	{
		if( (*(pbuf+tmpi) == 0x0d || *(pbuf+tmpi) == 0x0a) )
		{
			*(pbuf+tmpi) = 0;
			if( *ptr != 0 )
			{
				p = my_strstr( ptr, g_NowVersionResPath );
				// 带路径的上一个版本全文件名
				sprintf( fileName, "%s\\%s", g_ExpandResPath, p );
				// 检查之前的版本库是否有这个文件
				if ( !file_is_exist(fileName) )
				{
					// 没有这个文件，复制这个文件
					sprintf( fromName, "%s\\%s", g_NowVersionResPath, p );
					sprintf( toName, "%s\\Resources%s", g_WorkPath, p );
					// 这里要检查这个文件所在的目录层级，要一起创建出来
					my_mkpath( toName );
					my_copy_file( fromName, toName, 1 );
					printf( "没有找到文件[%s] ==》创建文件[%s]\n",fromName, toName );
				}
				else
				{ 
					// 有这个文件,那么就比较这个文件
					sprintf( fromName, "%s\\%s", g_NowVersionResPath, p );
					sprintf( toName, "%s\\%s", g_ExpandResPath, p );
					CompRet = CompFile( fromName, toName );
					if ( CompRet == 1 )
					{ // 不相同
						sprintf( toName, "%s\\Resources%s", g_WorkPath, p );
						// 这里要检查这个文件所在的目录层级，要一起创建出来
						my_mkpath( toName );
						my_copy_file( fromName, toName, 1 );
						printf( "文件内容不同[%s] ==》重新创建[%s]\n", fromName, toName );
					}
					else if ( CompRet == -1 )
					{ // 比较失败
						printf( "比较失败[%s]==[%s]\n", fromName, toName );
					}
					else
					{ // 0 完全一样
						//printf( "完全一样\n" );
					}
				}
			}
			ptr = pbuf+tmpi+1;
		}
	}
	free( pbuf );
	return 0;
}

void main( int argc, char *argv[] )
{
	char filename[MAX_PATH] = {0};
	if ( argc != 4 || strcmp( argv[1], "" ) == 0 || strcmp( argv[2], "" ) == 0 || strcmp( argv[3], "" ) == 0 )
	{
		printf( "参数不正确！\n" );
		return;
	}

	sprintf( g_WorkPath, "%s", argv[1] );
	sprintf( g_ExpandResPath, "%s", argv[2] );
	sprintf( g_NowVersionResPath, "%s", argv[3] );

	//sprintf( g_WorkPath, "%s", "C:\\Project\\NxProject\\NxAutoTools\\debug" );
	//sprintf( g_ExpandResPath, "%s", "C:\\Project\\NxProject\\NxAutoTools\\debug\\expand" );
	//sprintf( g_NowVersionResPath, "%s", "C:\\Project\\NxProject\\NxAutoTools\\debug\\1.0.22" );

	printf( "程序工作路径[%s]\n", g_WorkPath );
	printf( "扩展包Expand资源路径[%s]\n", g_ExpandResPath );
	printf( "当先修改过的资源路径[%s]\n", g_NowVersionResPath );

	/* 先打开文件，防止一会程序工作目录发生变化到时候写哪去了就不知道了 */
	sprintf( filename, "%s\\%s", g_WorkPath, ExpandOutFile );
	g_Expandfp = fopen( filename, "w" );
	if( g_Expandfp == NULL )
	{
		printf( "%s打开失败，错误码：%d\n",ExpandOutFile, GetLastError());
		return -1;
	}

	/* 先打开文件，防止一会程序工作目录发生变化到时候写哪去了就不知道了 */
	sprintf( filename, "%s\\%s", g_WorkPath, NowOutFile );
	g_Nowfp = fopen( filename, "w" );
	if( g_Nowfp == NULL )
	{
		printf( "%s打开失败，错误码：%d\n",NowOutFile, GetLastError());
		return -1;
	}

	/* 遍历文件 */
	ExpandResPathOutFile( g_ExpandResPath );
	NowVersionResPathOutFile( g_NowVersionResPath );

	/* 关闭吧 */
	fclose(g_Expandfp);
	fclose(g_Nowfp);

	// 开始创建补丁
	_chdir(g_WorkPath);
	CreatePatch();

	system("PAUSE");
}
