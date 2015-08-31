#ifndef COMMAND_H_
#define COMMAND_H_


// ������Ϣ
#define CMD_LOCALBASE		10
#define CMD_MAKEPATH		(CMD_LOCALBASE+1)

#define CMD_BASE			100
#define CMD_SHOW_MESSAGE	(CMD_BASE+1)
#define CMD_LOGIN			(CMD_BASE+2)	// ֪ͨ��ɫ�Ѿ�����
#define CMD_ENTERINFO		(CMD_BASE+3)
#define CMD_LIST			(CMD_BASE+4)	// �н�ɫ
#define CMD_CREATE			(CMD_BASE+5)	// ������ɫ
#define CMD_DELETE			(CMD_BASE+6)	// ɾ����ɫ
#define CMD_ENTERGAME		(CMD_BASE+7)	// ֪ͨ��ɫ������Ϸ
#define CMD_ADDACTOR		(CMD_BASE+8)	// ֪ͨһ���½�ɫ����
#define CMD_DELACTOR		(CMD_BASE+9)	// ֪ͨһ����ɫ�Ѿ��뿪
#define CMD_MOVE			(CMD_BASE+10)	// ֪ͨһ����ɫ����·��
#define CMD_TALK			(CMD_BASE+11)	// ֪ͨһ����Ϣ
#define CMD_CHANGEMAP		(CMD_BASE+12)	// �л�����
#define CMD_FIGHT			(CMD_BASE+13)	// ս��
#define CMD_FIGHT_PROCESS	(CMD_BASE+14)	// ս������
#define CMD_ADDTEAM			(CMD_BASE+15)	// �����³�Ա
#define CMD_ITEMLIST		(CMD_BASE+16)	// ��Ʒ�б�
#define CMD_ITEMUSE			(CMD_BASE+17)	// ʹ����Ʒ
#define CMD_ITEMPUT			(CMD_BASE+18)	// ʹ����Ʒ
#define CMD_COORDINATE		(CMD_BASE+32)	// �������
#define CMD_STSTCHANGE		(CMD_BASE+46)	// ��ɫ״̬�ı�

#define NOTIFY_NORMAL_MSG				0	// ��ͨ��Ϣ
#define NOTIFY_SYSTEM_MSG				1	// ϵͳ��Ϣ

#define STAT_WAIT	-1	// �ȴ�״̬-�߼���������ʲô������

#define STAT_NONE	0	// �߼�ʲô������
#define STAT_FWAIT	1	// ս���ȴ�
#define STAT_BEGIN	2	// δ��½״̬-�߼�����ִ�е�½
#define STAT_LOGIN	3	// �Ѿ���½״̬-�߼�����ִ���б�
#define STAT_LIST	4	// �Ѿ��б�״̬-�߼�����ִ�н�����Ϸ
#define STAT_NORMAN	5	// �Ѿ�������Ϸ״̬-�߼���ʱִ����ͨ��·����
#define STAT_FIGHT	6	// �Ѿ�����ս��״̬-�߼�����ִ�й�������
#define STAT_FIGHTWAIT	7	// ս���ȴ�״̬-�߼�����ִ�еȴ�һ��ʱ���ת��Ϊfight״̬
#define STAT_ENTER 8
#define STAT_CREATE 9
#define STAT_LOGOUT	10	// �ǳ�

#define STAT_STEP1	11
#define STAT_STEP2	12
#define STAT_STEP3	13
#define STAT_STEP4	14
#define STAT_STEP5	15
#define STAT_STEP6	16
#define STAT_STEP7	17
#define STAT_STEP8	18
#define STAT_STEP9	19
#define STAT_STEP10	20
#define STAT_STEP11	21
#define STAT_STEP12	22
#define STAT_STEP13	23
#define STAT_STEP14	24
#define STAT_STEP15	25
#define STAT_STEP16	26
#define STAT_STEP17	27
#define STAT_STEP18	28
#define STAT_STEP19	29
#define STAT_STEP20	30
#define STAT_STEP21	31
#define STAT_STEP22	32
#define STAT_STEP23	33
#define STAT_STEP24	34
#define STAT_STEP25	35
#define STAT_STEP26	36
#define STAT_STEP27	37
#define STAT_STEP28	38
#define STAT_STEP29	39
#define STAT_STEP30	40
#define STAT_STEP31	41
#define STAT_STEP32	42
#define STAT_STEP33	43
#define STAT_STEP34	44
#define STAT_STEP35	45
#define STAT_STEP36	46
#define STAT_STEP37	47
#define STAT_STEP38	48
#define STAT_STEP39	49
#define STAT_STEP40	50
#define STAT_STEP40x 111
#define STAT_STEP41	51
#define STAT_STEP42	52
#define STAT_STEP43	53
#define STAT_STEP44	54
#define STAT_STEP45	55
#define STAT_STEP46	56
#define STAT_STEP47	57
#define STAT_STEP48	58
#define STAT_STEP49	59
#define STAT_STEP50	60
#define STAT_STEP51	61
#define STAT_STEP52	62
#define STAT_STEP53	63
#define STAT_STEP54	64
#define STAT_STEP55	65
#define STAT_STEP56	66
#define STAT_STEP57	67
#define STAT_STEP58	68
#define STAT_STEP59	69
#define STAT_STEP60	70
#define STAT_STEP61	71
#define STAT_STEP62	72
#define STAT_STEP63	73
#define STAT_STEP64	74
#define STAT_STEP65	75
#define STAT_STEP66	76
#define STAT_STEP67	77
#define STAT_STEP68	78
#define STAT_STEP69	79
#define STAT_STEP70	80
#define STAT_STEP71	81
#define STAT_STEP72	82
#define STAT_STEP73	83
#define STAT_STEP74	84
#define STAT_STEP75	85
#define STAT_STEP76	86
#define STAT_STEP77	87
#define STAT_STEP78	88
#define STAT_STEP79	89
#define STAT_STEP80	90
#define STAT_STEP81	91
#define STAT_STEP82	92
#define STAT_STEP83	93
#define STAT_STEP84	94
#define STAT_STEP85	95
#define STAT_STEP86	96
#define STAT_STEP87	97
#define STAT_STEP88	98
#define STAT_STEP89	99
#define STAT_STEP90	100
#define STAT_STEP91	101
#define STAT_STEP92	102
#define STAT_STEP93	103
#define STAT_STEP94	104
#define STAT_STEP95	105
#define STAT_STEP96	106
#define STAT_STEP97	107
#define STAT_STEP98	108
#define STAT_STEP99	109
#define STAT_STEP100	110

struct _splayer
{
	int actorid;
	int exec_code;
	char stat;
	short posx;
	short posy;
	short waitframe;
	char wayx;
	char wayy;
	int wait_times;
	char stat2;
	char wait_stat;
	char isconnected;
};
typedef struct _splayer SLK_Player;


struct _clk_NetS_Login {
	int m_result;	//����ֵ
	char m_ekey_type;	//�ܱ�����
	char m_challenge[16];	//��ս��
	char m_username[51];	//�û���
	char m_token[65];	//token
};
typedef struct _clk_NetS_Login CLK_NetS_Login;	//�������˷��͵�¼���

int Recv_Login( char *data, int size );
int Recv_Logout( char *data, int size );
int Recv_EnterGame( char *data, int size );
int Recv_Move( char *data, int size );
int Recv_Talk( char *data, int size );

void LK_CommandInit();
int LK_ProcessMsg( int cmd, char *msg, int size, int sock_index );

#endif
