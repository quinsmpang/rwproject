#ifndef __PLAT_H_
#define __PLAT_H_

// ---------------------------------- ƽ̨ ---------------------------------------
struct _platinfo {
	int m_platid;				// ƽ̨��ţ�Ψһ��
	char m_userhost[64];		// �û���������ַ
	unsigned short m_userport;	// �û��������˿�
	char m_allow;				// �Ƿ��������
	char m_allowpay;			// �Ƿ�����֧��
};
typedef struct _platinfo PlatInfo;

// ƽ̨��Ϣ��ʼ��
int plat_init();

// ƽ̨��Ϣ�ض�
void plat_reload();


// ---------------------------------- ���� ---------------------------------------
#define LOCALIZE_TEXTMAXLEN	128

typedef enum _language
{
	CHINESE	= 0,	// ���庺��
	ENGLISH,		// Ӣ��
	LANGUAGE_MAX
}Language;

typedef struct _localize
{
	char text[LANGUAGE_MAX][LOCALIZE_TEXTMAXLEN];
}Localize;

int localize_init();
char *localize_gettext( short language, int textid );
#endif
