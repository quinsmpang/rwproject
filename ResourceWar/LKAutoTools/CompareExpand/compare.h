#ifndef _COMPARE_H_
#define _COMPARE_H_

#define MAXGETSIZE	2048
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

/* �Ƚ������ļ��Ƿ���ͬ */
int CompFile( char *pFile1, char *pFile2 );

/* �Ƚ�һ���ļ���һ���ڴ����ݱ� */
int CompFileBuf( char *pFile1, char *pBuf2, int nSize );

/* �Ƚ��������ڴ������Ƿ���ͬ */
int CompBuf( char *pBuf1, char *pBuf2, int nSize );

/* �ж��ļ��Ƿ���� */
int file_is_exist (const char *filename);

/* ��� path ��Ŀ¼����0, ���򷵻�1 */
int file_non_dir (const char *path);

/* �������м���Ŀ¼ */
int my_mkdir (const char *directory);

/* �����ļ������м���Ŀ¼,��󲻰�����ļ���Ŀ¼��ֻ�������ĸ�Ŀ¼���� */
int my_mkpath (const char *directory);

/* ����һ���ַ�����������ָ�룬ע��ʹ������ͷ� */
char *my_strdup (const char *str);

/* �����ļ� */
int my_copy_file(char *from,char *to,int fflag);

/* strstr���ص����׵�ַ������β��ַ */
char *my_strstr(const char *s1, const char *s2);
#endif
