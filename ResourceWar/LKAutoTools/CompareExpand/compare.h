#ifndef _COMPARE_H_
#define _COMPARE_H_

#define MAXGETSIZE	2048
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

/* 比较两个文件是否相同 */
int CompFile( char *pFile1, char *pFile2 );

/* 比较一个文件和一块内存数据比 */
int CompFileBuf( char *pFile1, char *pBuf2, int nSize );

/* 比较两个块内存数据是否相同 */
int CompBuf( char *pBuf1, char *pBuf2, int nSize );

/* 判断文件是否存在 */
int file_is_exist (const char *filename);

/* 如果 path 是目录返回0, 否则返回1 */
int file_non_dir (const char *path);

/* 创建所有级别目录 */
int my_mkdir (const char *directory);

/* 创建文件的所有级别目录,最后不把这个文件当目录，只创建它的父目录而已 */
int my_mkpath (const char *directory);

/* 复制一个字符串并返回其指针，注意使用完后释放 */
char *my_strdup (const char *str);

/* 复制文件 */
int my_copy_file(char *from,char *to,int fflag);

/* strstr返回的是首地址，这是尾地址 */
char *my_strstr(const char *s1, const char *s2);
#endif
