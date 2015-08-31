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
/* ��չ����Դ·�� */
char g_ExpandResPath[MAX_PATH]={0};
/* ��ǰ�޸Ĺ��İ汾��Դ·�� */
char g_NowVersionResPath[MAX_PATH]={0};
/* Ӧ�ó�������Ŀ¼ */
char g_WorkPath[MAX_PATH]={0};
/* �ļ�������������ʾ */
int g_FileCount = 0;
/* �ļ���� */
FILE *g_Expandfp = NULL;
FILE *g_Nowfp = NULL;
/* �ж�·���ĺϷ���:ȥ��"*",��ΪDispSubFiles()��·���Ѱ���"*" */
char *ResetFilePath(char *path)
{
	int len = 0;
	int tmpi = 0;
	char *temp = NULL; //���غϷ�·��
	len = strlen(path);//�����·������
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

/* �ݹ����Ŀ¼��������Ŀ¼ */
void IteratesFiles( FILE* fp)
{
	long handle;
	char path[MAX_PATH] = {0};
	struct _finddata_t t;
	handle=_findfirst("*",&t);
	if(handle==-1)
	{// ���handle=-1,��ʾĿ¼������
		return ;
	}
	if(t.attrib & _A_SUBDIR)
	{// 1. �жϵ�һ���ļ�:���ΪĿ¼
		if(t.name[0]!='.')
		{// ���Ŀ¼�ǿ�
			_chdir(t.name);
			_getcwd(path,MAX_PATH);
			//puts(path);
			IteratesFiles(fp);// �ݹ����
			_chdir("..");  // �������ļ��������,�����ϼ�Ŀ¼
		}
		//���Ϊ��Ŀ¼,��t.nameΪ"."��"..",�����
	}
	else
	{// �����һ�����ļ�����ΪĿ¼��Ŀǰ��̫������ô�趨
		_getcwd(path,MAX_PATH);
		strcat(path,"\\");
		strcat(path,t.name);
		//puts(path);
	}
	while( !( _findnext(handle,&t) ) )//2. �ж���һ���ļ�,ֱ�����������ļ�
	{ 
		if(t.attrib & _A_SUBDIR)//�����һ���ļ�ΪĿ¼
		{
			if(t.name[0]!='.')//���Ŀ¼�ǿ�
			{
				_chdir(t.name);
				_getcwd(path,MAX_PATH);
				//puts(path);
				IteratesFiles(fp);//�ݹ����
				_chdir("..");//�������ļ��������,�����ϼ�Ŀ¼
			}
			//���Ϊ��Ŀ¼,��t.nameΪ"."��"..",�����
		}
		else
		{//������ļ�����ΪĿ¼,��ʾ�ļ�����·��
			if ( (t.attrib & _A_HIDDEN) )
			{ // �������ص��ļ�
			}
			else
			{
				_getcwd(path,MAX_PATH);// ��ȡ�ļ�·��
				strcat(path,"\\");
				strcat(path,t.name);// �ļ�����·��

				//puts(path);// ���·��
				fprintf( fp, "%s\n", path );
				g_FileCount+=1;
			}
		}
	}
	_findclose(handle);
}

/* ��һ���汾��������Դ·��������ļ� */
int ExpandResPathOutFile( char *pathname )
{
	char szMsg[64]={0};
	char dirc[MAX_PATH]={0};

	sprintf( dirc, "%s\\", pathname );
	_chdir(ResetFilePath(dirc));// ��ָ��Ŀ¼
	IteratesFiles(g_Expandfp);			// ��������Ŀ¼���ļ�

	sprintf( szMsg, "Expand����Դ������%d\n", g_FileCount );
	puts(szMsg);
	g_FileCount = 0;
	return 0;
}

/* ��ǰ�汾��������Դ·��������ļ� */
int NowVersionResPathOutFile( char *pathname )
{
	char szMsg[64]={0};
	char dirc[MAX_PATH]={0};

	sprintf( dirc, "%s\\", pathname );
	_chdir(ResetFilePath(dirc));// ��ָ��Ŀ¼
	IteratesFiles(g_Nowfp);			// ��������Ŀ¼���ļ�

	sprintf( szMsg, "��ǰ�޸Ĺ��İ汾��Դ������%d\n", g_FileCount );
	puts(szMsg);
	g_FileCount = 0;
	return 0;
}

/* �������� */
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
		printf( "%s��ʧ�ܣ������룺%d\n",NowOutFile, GetLastError());
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

	// ����������Դ�ļ���
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
				// ��·������һ���汾ȫ�ļ���
				sprintf( fileName, "%s\\%s", g_ExpandResPath, p );
				// ���֮ǰ�İ汾���Ƿ�������ļ�
				if ( !file_is_exist(fileName) )
				{
					// û������ļ�����������ļ�
					sprintf( fromName, "%s\\%s", g_NowVersionResPath, p );
					sprintf( toName, "%s\\Resources%s", g_WorkPath, p );
					// ����Ҫ�������ļ����ڵ�Ŀ¼�㼶��Ҫһ�𴴽�����
					my_mkpath( toName );
					my_copy_file( fromName, toName, 1 );
					printf( "û���ҵ��ļ�[%s] ==�������ļ�[%s]\n",fromName, toName );
				}
				else
				{ 
					// ������ļ�,��ô�ͱȽ�����ļ�
					sprintf( fromName, "%s\\%s", g_NowVersionResPath, p );
					sprintf( toName, "%s\\%s", g_ExpandResPath, p );
					CompRet = CompFile( fromName, toName );
					if ( CompRet == 1 )
					{ // ����ͬ
						sprintf( toName, "%s\\Resources%s", g_WorkPath, p );
						// ����Ҫ�������ļ����ڵ�Ŀ¼�㼶��Ҫһ�𴴽�����
						my_mkpath( toName );
						my_copy_file( fromName, toName, 1 );
						printf( "�ļ����ݲ�ͬ[%s] ==�����´���[%s]\n", fromName, toName );
					}
					else if ( CompRet == -1 )
					{ // �Ƚ�ʧ��
						printf( "�Ƚ�ʧ��[%s]==[%s]\n", fromName, toName );
					}
					else
					{ // 0 ��ȫһ��
						//printf( "��ȫһ��\n" );
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
		printf( "��������ȷ��\n" );
		return;
	}

	sprintf( g_WorkPath, "%s", argv[1] );
	sprintf( g_ExpandResPath, "%s", argv[2] );
	sprintf( g_NowVersionResPath, "%s", argv[3] );

	//sprintf( g_WorkPath, "%s", "C:\\Project\\NxProject\\NxAutoTools\\debug" );
	//sprintf( g_ExpandResPath, "%s", "C:\\Project\\NxProject\\NxAutoTools\\debug\\expand" );
	//sprintf( g_NowVersionResPath, "%s", "C:\\Project\\NxProject\\NxAutoTools\\debug\\1.0.22" );

	printf( "������·��[%s]\n", g_WorkPath );
	printf( "��չ��Expand��Դ·��[%s]\n", g_ExpandResPath );
	printf( "�����޸Ĺ�����Դ·��[%s]\n", g_NowVersionResPath );

	/* �ȴ��ļ�����ֹһ�������Ŀ¼�����仯��ʱ��д��ȥ�˾Ͳ�֪���� */
	sprintf( filename, "%s\\%s", g_WorkPath, ExpandOutFile );
	g_Expandfp = fopen( filename, "w" );
	if( g_Expandfp == NULL )
	{
		printf( "%s��ʧ�ܣ������룺%d\n",ExpandOutFile, GetLastError());
		return -1;
	}

	/* �ȴ��ļ�����ֹһ�������Ŀ¼�����仯��ʱ��д��ȥ�˾Ͳ�֪���� */
	sprintf( filename, "%s\\%s", g_WorkPath, NowOutFile );
	g_Nowfp = fopen( filename, "w" );
	if( g_Nowfp == NULL )
	{
		printf( "%s��ʧ�ܣ������룺%d\n",NowOutFile, GetLastError());
		return -1;
	}

	/* �����ļ� */
	ExpandResPathOutFile( g_ExpandResPath );
	NowVersionResPathOutFile( g_NowVersionResPath );

	/* �رհ� */
	fclose(g_Expandfp);
	fclose(g_Nowfp);

	// ��ʼ��������
	_chdir(g_WorkPath);
	CreatePatch();

	system("PAUSE");
}
