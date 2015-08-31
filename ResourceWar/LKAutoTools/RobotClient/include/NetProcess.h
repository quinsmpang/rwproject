#ifndef NETPROCESS_H_
#define NETPROCESS_H_
#include "Game.h"

struct _act {
	char m_nPos;
	char m_nType;
	short m_nValue;
};
typedef struct _act SLK_Act;

struct _sactorlistinfo
{
	int actorid;
	char lockstat;
	int freepoint;
	int shape;
	char name[NAME_SIZE];
	char profession;
	char gender;
	short level;
};
typedef struct _sactorlistinfo SActorListInfo;

struct _giveitem
{
	short m_index;	// ��Ʒ���е����
	short m_num;	// ��Ʒ����
};
typedef struct _giveitem SLK_GiveItem;


#define ACTOR_STAT_STAND	0
#define ACTOR_STAT_WALK		1
#define ACTOR_STAT_FIGHT	2

#define ACTOR_STAT_UPGRADE	10
#define ACTOR_STAT_USEITEM	11


// �ӷ��������յ�����Ϣ
#define CMDS_BASE			0
#define CMDS_LOGIN			(CMDS_BASE+1)	// ֪ͨ��ɫ�Ѿ�����
#define CMDS_ENTERINFO		(CMDS_BASE+2)	
#define CMDS_LIST			(CMDS_BASE+3)	// �н�ɫ
#define CMDS_CREATE			(CMDS_BASE+4)	// ������ɫ
#define CMDS_DELETE			(CMDS_BASE+5)	// ɾ����ɫ
#define CMDS_ENTERGAME		(CMDS_BASE+6)	// ֪ͨ��ɫ������Ϸ
#define CMDS_ADDACTOR		(CMDS_BASE+7)	// ֪ͨһ���½�ɫ����
#define CMDS_DELACTOR		(CMDS_BASE+8)	// ֪ͨһ����ɫ�Ѿ��뿪
#define CMDS_MOVE			(CMDS_BASE+9)	// ֪ͨһ����ɫ����·��
#define CMDS_TALK			(CMDS_BASE+10)	// ֪ͨһ����Ϣ
#define CMDS_CHANGEMAP		(CMDS_BASE+11)	// �л�����
#define CMDS_FIGHT			(CMDS_BASE+12)	// ս��
#define CMDS_FIGHT_PROCESS	(CMDS_BASE+13)	// ս�����
#define CMDS_ADDTEAM		(CMDS_BASE+14)	// �����³�Ա
#define CMDS_ITEMLIST		(CMDS_BASE+15)	// ��Ʒ�б�
#define CMDS_ITEMUSE		(CMDS_BASE+16)	// ʹ����Ʒ
#define CMDS_ITEMPUT		(CMDS_BASE+17)	// ʹ����Ʒ
#define CMDS_EXPERIENCE		(CMDS_BASE+18)	// ��ɫ��þ���(����֪ͨ)
#define CMDS_ACTORINFO		(CMDS_BASE+19)	// ��ɫ��Ϣ
#define CMDS_SMPINFO		(CMDS_BASE+20)	// ��ɫ����Ϣ
#define CMDS_NPCTALK		(CMDS_BASE+21)	// ��NPC�Ի���Ϣ
#define CMDS_LOSTITEM		(CMDS_BASE+22)	// ʧȥ��Ʒ
#define CMDS_GETITEM		(CMDS_BASE+23)	// �õ���Ʒ
#define CMDS_MONEY			(CMDS_BASE+24)	// ��ɫ��ý�Ǯ(����֪ͨ)
#define CMDS_ASKMAKETEAM	(CMDS_BASE+25)	// ���������ӵ���Ϣ
#define CMDS_SYSNOTIFY		(CMDS_BASE+26)	// ϵͳ֪ͨ
#define CMDS_REPLYMAKETEAM	(CMDS_BASE+27)	// �����
#define CMDS_SETTEAMINFO	(CMDS_BASE+28)	// �����Ϣ
#define CMDS_LEAVETEAM		(CMDS_BASE+29)	// �뿪����
#define CMDS_GETQUEST		(CMDS_BASE+30)	// �����б�
#define CMDS_COORDINATE		(CMDS_BASE+31)	// ��ɫ������Ϣ
#define CMDS_ASKMAKEFRIEND	(CMDS_BASE+32)	// ���Ӻ��ѵĴ�
#define CMDS_FRIENDMSG		(CMDS_BASE+33)	// ���ѵ���Ϣ
#define CMDS_OPENSHOP		(CMDS_BASE+34)	// �򿪹�����Ʒ�ĶԻ���
#define CMDS_BUYITEM		(CMDS_BASE+35)	// ��������Ʒ
#define CMDS_SKILLLIST		(CMDS_BASE+36)	// �����б�
#define CMDS_ITEMINFO		(CMDS_BASE+37)	// ��Ʒ��Ϣ
#define CMDS_ACTORMODIFYINFO	(CMDS_BASE+38)	// ��ɫװ����Ϣ
#define CMDS_GETPETLIST		(CMDS_BASE+39)	// �����б�
#define CMDS_GETPETINFO		(CMDS_BASE+40)	// ������Ϣ
#define CMDS_SETPETNAME		(CMDS_BASE+41)	// �������ֱ��
#define CMDS_LOSTPET		(CMDS_BASE+42)	// ���ﶪʧ
#define CMDS_SETFIGHTPET	(CMDS_BASE+43)	// ���ó����ս
#define CMDS_SHOWPET		(CMDS_BASE+44)	// ���ó���ۿ�
#define CMDS_STSTCHANGE		(CMDS_BASE+45)	// ��ɫ״̬�ı�
#define CMDS_NEWPET			(CMDS_BASE+46)	// ץ�����³���
#define CMDS_ITEMTALK		(CMDS_BASE+47)	// ���ͳ�����
#define CMDS_SELLITEM		(CMDS_BASE+48)	// ��������Ʒ
#define CMDS_OPENSELL		(CMDS_BASE+49)	// �򿪳��۵ĶԻ���
#define CMDS_OPENTRADE		(CMDS_BASE+50)	// �򿪽��׵ĶԻ���
#define CMDS_TRADESET		(CMDS_BASE+51)	// ���Է����ý��׵���Ʒ��Ǯ����ȡ������
#define CMDS_TRADEOVER		(CMDS_BASE+52)	// ��˫�������˽��׻��ߵ�����ȡ�����ף����յ�����
#define CMDS_OPENTAKE		(CMDS_BASE+53)	// �򿪼Ĵ洦�ĶԻ���
#define CMDS_TAKEITEM		(CMDS_BASE+54)	// �Ĵ����ȡ������Ʒ
#define CMDS_OPENBANK		(CMDS_BASE+55)	// �����еĶԻ���
#define CMDS_TAKEMONEY		(CMDS_BASE+56)	// �����ȡ��Ǯ
#define CMDS_FRIENDINFO		(CMDS_BASE+57)	// ��ѯ���ĺ�����Ϣ
#define CMDS_FRIENDONLINE	(CMDS_BASE+58)	// ������������Ϣ
#define CMDS_GETPREFIXLIST	(CMDS_BASE+59)	// ���ͳ�ν�б�
#define CMDS_SETPREFIX		(CMDS_BASE+60)	// �����µĳ�ν
#define CMDS_SETBOOTHSIGN	(CMDS_BASE+61)	// ���õ�������
#define CMDS_BOOTHREPLY		(CMDS_BASE+62)	// �𸴵���
#define CMDS_BOOTHPROCESS	(CMDS_BASE+63)	// ֪ͨ����(���¼ܳɹ�ʧ��)
#define CMDS_BOOTHLIST		(CMDS_BASE+64)	// ̯λ����Ʒ�б�
#define CMDS_BOOTHBUY		(CMDS_BASE+65)	// ̯λ����
#define CMDS_TREASURETALK	(CMDS_BASE+66)	// ������Ϣ
#define CMDS_OPENSTONE		(CMDS_BASE+67)	// ����Ƕ��ʯ�Ի���
#define CMDS_STONECOMPOSE	(CMDS_BASE+68)	// ��ʯ�ϳ����
#define CMDS_ACTORSMALLINFO	(CMDS_BASE+69)	// һЩ�����µĽ�ɫ��Ϣ
#define CMDS_SKILLINFO		(CMDS_BASE+70)	// ����������Ϣ
#define CMDS_CARDSTAT		(CMDS_BASE+71)	// ��ɫ��״̬�ı�
#define CMDS_OPENSELLCARD	(CMDS_BASE+72)	// �򿪼��ۿ��ĶԻ���
#define CMDS_OPENBUYCARD	(CMDS_BASE+73)	// �򿪹��򿨵ĶԻ���
#define CMDS_REPAIRITEM		(CMDS_BASE+74)	// �������
#define CMDS_SURPRISEGET	(CMDS_BASE+75)	// �õ���ϲ
#define CMDS_TEAMLIST		(CMDS_BASE+76)	// �����Ա�򵥵��б�


// �ӿͻ���Ҫ��������Ϣ
#define CMDC_BASE			0
//#define CMDC_TIMER			(CMDC_BASE+1)	// ������ר����Ϣ,�ͻ��˲�ʹ��
#define CMDC_LOGIN			(CMDC_BASE+2)	// ���������
#define CMDC_ENTERINFO		(CMDC_BASE+3)
#define CMDC_LIST			(CMDC_BASE+4)	// �н�ɫ
#define CMDC_CREATE			(CMDC_BASE+5)	// ������ɫ
#define CMDC_DELETE			(CMDC_BASE+6)	// ɾ����ɫ
#define CMDC_ENTERGAME		(CMDC_BASE+7)	// ������Ϸ
#define CMDC_LEAVEGAME		(CMDC_BASE+8)	// �뿪��Ϸ
#define CMDC_MOVE			(CMDC_BASE+9)	// �ƶ�����
#define CMDC_TALK			(CMDC_BASE+10)	// ����˵��
#define CMDC_FIGHT			(CMDC_BASE+11)	// ��������
#define CMDC_MAKETEAM		(CMDC_BASE+12)	// �������(��ָ���Ѿ�����)
#define CMDC_ITEMLIST		(CMDC_BASE+13)	// ��ȡ��Ʒ�б�
#define CMDC_ITEMUSE		(CMDC_BASE+14)	// ʹ����Ʒ
#define CMDC_ITEMPUT		(CMDC_BASE+15)	// ��Ʒ���õ�����
#define CMDC_ACTORINFO		(CMDC_BASE+16)	// ��ɫ��Ϣ
#define CMDC_TOUCHNPC		(CMDC_BASE+17)	// �Ӵ�NPC
#define CMDC_GIVEITEM		(CMDC_BASE+18)	// ������Ʒ
#define CMDC_ASKMAKETEAM	(CMDC_BASE+19)	// �������
#define CMDC_REPLYMAKETEAM	(CMDC_BASE+20)	// �����
#define CMDC_PROCESSTEAM	(CMDC_BASE+21)	// ��Ӳ���
#define CMDC_GETQUEST		(CMDC_BASE+22)	// ��ȡ�����б�
#define CMDC_ATTACK			(CMDC_BASE+23)	// ��������
#define CMDC_ASKMAKEFRIEND	(CMDC_BASE+24)	// ��������
#define CMDC_FRIENDMSG		(CMDC_BASE+25)	// ������Ϣ
#define CMDC_APPLYATTR		(CMDC_BASE+26)	// �������Ե�
#define CMDC_SHOPBUY		(CMDC_BASE+27)	// �̵깺��
#define CMDC_ITEMDROP		(CMDC_BASE+28)	// ������Ʒ
#define CMDC_GMCOMMAND		(CMDC_BASE+29)	// GMָ��
#define CMDC_SKILLLIST		(CMDC_BASE+30)	// �����б�
#define CMDC_ITEMINFO		(CMDC_BASE+31)	// ��Ʒ��Ϣ
#define CMDC_GETPETLIST		(CMDC_BASE+32)	// �����б�
#define CMDC_PETPROCESS		(CMDC_BASE+33)	// ������� 0ȡ���� 1�ͷ� 2����ս�� 3���ùۿ�
#define CMDC_SETPETNAME		(CMDC_BASE+34)	// �������ֱ��
#define CMDC_APPLYPETATTR	(CMDC_BASE+35)	// �������Ա��
#define CMDC_STATCHANGE		(CMDC_BASE+36)	// ��ɫ״̬�ı�Ĺ㲥
#define CMDC_SHOPSELL		(CMDC_BASE+37)	// ��Ʒ����
#define CMDC_REQUESTTRADE	(CMDC_BASE+38)	// ������������
#define CMDC_TRADESET		(CMDC_BASE+39)	// ���ý��׵���Ʒ��Ǯ - ��ȡ������
#define CMDC_TRADEOVER		(CMDC_BASE+40)	// ���������Ѿ�ȷ������ȡ��
#define CMDC_SHOPTAKE		(CMDC_BASE+41)	// �Ĵ��ȡ��
#define CMDC_SHOPBANK		(CMDC_BASE+42)	// ���ȡǮ
#define CMDC_QUERYFRIEND	(CMDC_BASE+43)	// ��ѯ����
#define CMDC_FRIENDINFO		(CMDC_BASE+44)	// ��������
#define CMDC_DELETEFRIEND	(CMDC_BASE+45)	// ɾ������
#define CMDC_PROCESSPREFIX	(CMDC_BASE+46)	// �����ν,0���� 1ɾ��
#define CMDC_ASKBOOTH		(CMDC_BASE+47)	// �����̯
#define CMDC_CHANGEBOOTHNAME (CMDC_BASE+48)	// ��̯��
#define CMDC_BOOTHPROCESS	(CMDC_BASE+49)	// �����̯,�ϼܺ��¼�
#define CMDC_OPENBOOTH		(CMDC_BASE+50)	// ����ҵ�̯λ
#define CMDC_BOOTHBUY		(CMDC_BASE+51)	// ����̯λ�ϵ���Ʒ
#define CMDC_SETCURSKILL	(CMDC_BASE+52)	// ���ü���
#define CMDC_FRIENDGROUPMOVE (CMDC_BASE+53)	// �������ƶ�����
#define CMDC_FRIENDGROUPCHG	(CMDC_BASE+54)	// ���������
#define CMDC_STONECOMPOSE	(CMDC_BASE+55)	// ��ʯ�ϳ�
#define CMDC_SETCONFIG		(CMDC_BASE+56)	// ������ҵ�����
#define CMDC_HEARTBEAT		(CMDC_BASE+57)	// ����
#define CMDC_CARDSELL		(CMDC_BASE+58)	// ����
#define CMDC_CARDBUY		(CMDC_BASE+59)	// ��
#define CMDC_REPAIRITEM		(CMDC_BASE+60)	// ����װ��
#define CMDC_SURPRISESET	(CMDC_BASE+61)	// ���þ�ϲ

void LK_NetInit();

int Net_HeartBeat( int sock_index );

int Net_Login( char *szUserName, char *szPassWord, int sock_index );
int Net_Logout( int sock_index );
int Net_List( int sock_index );
int Net_Create( char nPosition, char *name, int sock_index );
int Net_Delete( char nOffset, int sock_index );
int Net_EnterGame( int actorid, int sock_index );
int Net_LeaveGame( int sock_index );
int Net_Move( int posx, int posy, int sock_index );
int Net_Talk( const char *pText, char nType, int sock_index );
int Net_Fight( SLK_Act *pAct, int sock_index );
int Net_GMCommand( short nCmd, int value1, int value2, int value3, int value4, char *pMsg, int sock_index );
int Net_TouchNpc( char nType, char nSelected, int nActorID, int sock_index );
int Net_ItemUse( short nItemIndex, short nItemNum, char nPetIndex, int sock_index );

#endif
