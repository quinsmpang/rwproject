
#include "compare.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#include <malloc.h>
#include <direct.h>
#include <io.h>
#else
#include <pwd.h>
#include <sys/types.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#endif
#include <sys/stat.h>
#include <fcntl.h>

#ifndef S_ISDIR
#define S_ISDIR(m) (((m) & (_S_IFMT)) == (_S_IFDIR))
#endif

// -1比较失败
// 0 完全一样
// 1 不相同

/* 比较两个文件是否相同 */
int CompFile( char *pFile1, char *pFile2 )
{
	FILE *fp1,*fp2;
	char *buf1;
	char *buf2;
	int filesize1;
	int filesize2;
	int rtn;

	if( (fp1 = fopen( pFile1, "rb" )) == NULL )
		return -1;
	if( (fp2 = fopen( pFile2, "rb" )) == NULL )
	{
		fclose( fp1 );
		return -1;
	}
	fseek( fp1, 0, SEEK_END );
	fseek( fp2, 0, SEEK_END );
	filesize1 = ftell( fp1 );
	filesize2 = ftell( fp2 );
	if( filesize1 != filesize2 )
	{
		fclose( fp1 );
		fclose( fp2 );
		return 1;
	}
	if( filesize1 == 0 )
	{
		fclose( fp1 );
		fclose( fp2 );
		return 0;
	}
	fseek( fp1, 0, SEEK_SET );
	fseek( fp2, 0, SEEK_SET );

	buf1 = (char *)malloc( filesize1 );
	if( buf1 == NULL )
		return -1;
	buf2 = (char *)malloc( filesize2 );
	if( buf2 == NULL )
	{
		free( buf1 );
		return -1;
	}
	if( fread( buf1, sizeof(char), filesize1, fp1 ) != filesize1 )
	{
		free( buf1 );
		free( buf2 );
		fclose( fp1 );
		fclose( fp2 );
		return -1;
	}
	if( fread( buf2, sizeof(char), filesize2, fp2 ) != filesize2 )
	{
		free( buf1 );
		free( buf2 );
		fclose( fp1 );
		fclose( fp2 );
		return -1;
	}

	rtn = CompBuf( buf1, buf2, filesize1 );
	free( buf1 );
	free( buf2 );
	fclose( fp1 );
	fclose( fp2 );
	return rtn;
}
/* 比较一个文件和一块内存数据比 */
int CompFileBuf( char *pFile1, char *pBuf2, int nSize )
{
	FILE *fp1;
	char *buf1;
	int filesize1;
	int rtn;
	if( (fp1 = fopen( pFile1, "rb" )) == NULL )
	{
		if( nSize == 0 )
			return 0;
		return -1;
	}
	fseek( fp1, 0, SEEK_END );
	filesize1 = ftell( fp1 );
	if( filesize1 != nSize )
	{
		fclose( fp1 );
		return 1;
	}
	if( filesize1 == 0 )
	{
		fclose( fp1 );
		return 0;
	}
	fseek( fp1, 0, SEEK_SET );

	buf1 = (char *)malloc( filesize1 );
	if( buf1 == NULL )
		return -1;
	if( fread( buf1, sizeof(char), filesize1, fp1 ) != filesize1 )
	{
		free( buf1 );
		fclose( fp1 );
		return -1;
	}

	rtn = CompBuf( buf1, pBuf2, nSize );
	free( buf1 );
	fclose( fp1 );
	return rtn;
}

/* 比较两个块内存数据是否相同 */
int CompBuf( char *pBuf1, char *pBuf2, int nSize )
{
	int rtn = 0;
	int tmpi;
	for( tmpi = 0; tmpi < nSize; tmpi++ )
	{
		if( pBuf1[tmpi] != pBuf2[tmpi] )
		{
			rtn = 1;
			break;
		}
	}
	return rtn;
}

/* 复制一个字符串并返回其指针，注意使用完后释放 */
char *my_strdup (const char *str)
{
	int len = (int)strlen (str);
	char *s1 = (char *)malloc( len + 1 );
	memcpy (s1, str, len + 1);
	return s1;
}

/* 判断文件是否存在 */
int file_is_exist (const char *filename)
{
	struct stat buf;
	return stat (filename, &buf) >= 0;
}
/* 如果 path 是目录返回0, 否则返回1 */
int file_non_dir (const char *path)
{
	struct stat buf;
	if (stat (path, &buf) != 0)
		return 0;
	return S_ISDIR (buf.st_mode) ? 0 : 1;
}
/* 创建所有级别目录 */
int my_mkdir (const char *directory)
{
	int quit = 0;
	int tmpi;
	char *dir;

	dir = my_strdup(directory);

	for (tmpi = (*dir == '/' || *dir == '\\' ); 1; tmpi++)
	{
		for (; dir[tmpi] && (dir[tmpi] != '/' && dir[tmpi] != '\\'); tmpi++)
			;
		if (!dir[tmpi])
			quit = 1;
		dir[tmpi] = '\0';
		/* 判断是否有同名的文件或者目录存在 */
		if (!file_is_exist (dir))
		{
#ifndef WIN32
			if (mkdir (dir, 0777) < 0)
#else
			if( tmpi >= 1 && dir[tmpi-1] == ':' )
			{
				if (quit)
					break;
				else
					dir[tmpi] = '/';
				continue;
			}
			if (_mkdir (dir) < 0)
#endif
			{
				free(dir);
				return -1;
			}
		}
		if (quit)
			break;
		else
			dir[tmpi] = '/';
	}
	free(dir);
	return 0;
}

/* 创建文件的所有级别目录,最后不把这个文件当目录，只创建它的父目录而已 */
int my_mkpath(const char *directory)
{
	int tmpi;
	char *dir;

	dir = my_strdup (directory);

	for (tmpi = (*dir == '/' || *dir == '\\' ); 1; tmpi++)
	{
		for (; dir[tmpi] && (dir[tmpi] != '/' && dir[tmpi] != '\\'); tmpi++)
			;
		if (!dir[tmpi])
			break;
		dir[tmpi] = '\0';
		/* 判断是否有同名的文件或者目录存在 */
		if (!file_is_exist (dir))
		{
#ifndef WIN32
			if (mkdir (dir, 0777) < 0)
#else
			if( tmpi >= 1 && dir[tmpi-1] == ':' )
			{
				dir[tmpi] = '/';
				continue;
			}
			if (_mkdir (dir) < 0)
#endif
			{
				free(dir);
				return -1;
			}
		}
		dir[tmpi] = '/';
	}
	free(dir);
	return 0;
}

/* 复制文件 */
int my_copy_file(char *from,char *to,int fflag)
{
	static char buf[MAXGETSIZE];
	int from_fd, rcount, rval, to_fd, wcount, wresid;
	char *bufp;

	if(strcmp(from,to)==0)
		return -1;

	if ((from_fd = _open(from, O_RDONLY | O_BINARY, 0)) == -1)
		return -1;

	if (fflag) //创建方式
	{
		(void)_unlink(to);
		to_fd = _open(to, O_WRONLY | O_TRUNC | O_CREAT | O_BINARY );
	}
	else //覆盖方式
		to_fd = _open(to, O_WRONLY | O_TRUNC, 0);
	if (to_fd == -1)
	{
		(void)_close(from_fd);
		return -1;
	}
	rval = 0;
	while ((rcount = _read(from_fd, buf, MAXGETSIZE)) > 0)
	{
		for (bufp=buf,wresid=rcount;;bufp+=wcount,wresid-=wcount)
		{
			wcount = _write(to_fd, bufp, wresid);
			if (wcount >= wresid || wcount <= 0)
				break;
		}
		if (wcount != wresid)
		{
			rval = -1;
			break;
		}
	}
	if (rcount < 0)
		rval = -1;

	(void)_close(from_fd);
	if (_close(to_fd))
		rval = -1;
	return (rval);
}


char *my_strstr(const char *s1, const char *s2)
{
	const char *p = s1;
	const size_t len = strlen (s2);
	for (; (p = strchr (p, *s2)) != 0; p++)
	{
		if (strncmp (p, s2, len) == 0)
			return (char *)(p+len);
	}
	return (0);
}
