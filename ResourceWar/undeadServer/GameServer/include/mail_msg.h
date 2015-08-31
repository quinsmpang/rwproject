#ifndef __MAIL_MSG_H
#define __MAIL_MSG_H

#define MAX_MAIL_SAVETIME		86400*7		// 没附件邮件保存时间7天，有附件的*2

#define MAIL_TITLE_MAXSIZE		32			// 邮件标题长度
#define MAIL_CONTENT_MAXSIZE	1024		// 邮件信息长度
#define MAIL_ATTACH_MAXSIZE		128			// 邮件附件长度

#define MAIL_TYPE_SYSTEM	1	// 系统邮件
#define MAIL_TYPE_CHAT		2	// 聊天邮件

/* 发送邮件 参数：邮件类型，发送人id，发送人名称，收件人id，标题，内容，附件 */
int mail_send( char type, int from_actorid, char *from_name, int to_actorid, char *title, char *content, char *attach );

/* 检查是否有未读邮件 */
int mail_check_read( int actor_index );

/* 读取邮件列表 */
int mail_loadlist( int actor_index, i64 begin );

/* 读取详细内容 */
int mail_read_content( int actor_index, i64 mailid );

/* 领取附件 */
int mail_read_attach( int actor_index, i64 mailid );

/* 删除 */
int mail_delete( int actor_index, i64 mailid );

/* 邮件过期检查 */
int mail_overdue();

/* 邮件过期检查每天早上2点检查 */
void mail_overdue_everyday();
#endif
