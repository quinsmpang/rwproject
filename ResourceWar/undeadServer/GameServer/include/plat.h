#ifndef __PLAT_H_
#define __PLAT_H_

// ---------------------------------- 平台 ---------------------------------------
struct _platinfo {
	int m_platid;				// 平台编号（唯一）
	char m_userhost[64];		// 用户服务器地址
	unsigned short m_userport;	// 用户服务器端口
	char m_allow;				// 是否允许登入
	char m_allowpay;			// 是否允许支付
};
typedef struct _platinfo PlatInfo;

// 平台信息初始化
int plat_init();

// 平台信息重读
void plat_reload();


// ---------------------------------- 语言 ---------------------------------------
#define LOCALIZE_TEXTMAXLEN	128

typedef enum _language
{
	CHINESE	= 0,	// 简体汉字
	ENGLISH,		// 英文
	LANGUAGE_MAX
}Language;

typedef struct _localize
{
	char text[LANGUAGE_MAX][LOCALIZE_TEXTMAXLEN];
}Localize;

int localize_init();
char *localize_gettext( short language, int textid );
#endif
