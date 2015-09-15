#ifndef __STRUCTSEND0_AUTO_H
#define __STRUCTSEND0_AUTO_H
#include "server_struct_auto.h"

struct _slk_NetS_Login {
	int m_result;	//����������-����ֵ
	short m_username_length;	//����������-�û�������
	char m_username[51];	//����������-�û���
	short m_token_length;	//����������-token����
	char m_token[256];	//����������-token
};
typedef struct _slk_NetS_Login SLK_NetS_Login;	//�������˷��͵�¼������ͻ���

struct _slk_ListInfo {
	int m_actorid;	//��ɫID
	char m_name[22];	//��ɫ����
	char m_aclass;	//��ɫ����
	short m_level;	//��ɫ�ȼ�
	char m_offset;	//��ɫλ��
	char m_lockstat;	//����״̬
	int m_delete_stoptime;	//ɾ��ʱ��
};
typedef struct _slk_ListInfo SLK_ListInfo;	//������ɫ���б���Ϣ

struct _slk_NetS_List {
	char m_actor_num;	//����˷���-��ɫ����
	SLK_ListInfo m_listinfo[8];	//����˷���-��ɫ����
};
typedef struct _slk_NetS_List SLK_NetS_List;	//�������˷��ͽ�ɫ�б�

struct _slk_NetS_Create {
	char m_result;	//����˷���-������ɫ�Ľ��
	int m_actorid;	//����˷���-������ɫ�ı��
};
typedef struct _slk_NetS_Create SLK_NetS_Create;	//���������ش�����ɫ���

struct _slk_NetS_EnterInfo {
	int m_actorid;	//����˷���-������Ϸ��ɫID
	int m_data_int;	//����˷���-������Ϸ��������[����]
	short m_data_short;	//����˷���-������Ϸ��������[����]
	char m_data_char;	//����˷���-������Ϸ��������[����]
};
typedef struct _slk_NetS_EnterInfo SLK_NetS_EnterInfo;	//���������ؽ�����Ϸ�����Ϣ

struct _slk_NetS_Delete {
	int m_result;	//����������-ɾ������ֵ
	int m_actorid;	//����������-ɾ���Ľ�ɫ���
};
typedef struct _slk_NetS_Delete SLK_NetS_Delete;	//����������ɾ����ɫ

struct _slk_NetS_Heart {
	int m_value[2];	//����-��ֵ
};
typedef struct _slk_NetS_Heart SLK_NetS_Heart;	//����

struct _slk_NetS_Notify {
	short m_msgid;	//����������-����ϢID
	short m_valuenum;	//����������-����Ϣ��������
	int m_value[10];	//����������-����Ϣ��ֵ����
	short m_msg_length;	//����������-����Ϣ�ַ�����Ϣ����
	char m_msg[256];	//����������-����Ϣ�ַ�������
};
typedef struct _slk_NetS_Notify SLK_NetS_Notify;	//ͨ�ö���Ϣ

struct _slk_NetS_Talk {
	char m_type;	//����������-��������
	int m_actorid;	//����������-�����ɫID
	char m_name[22];	//����������-�����ɫ��
	short m_msg_length;	//����������-�������ݳ���
	char m_msg[256];	//����������-��������
	char m_shape;	//����������-��������
	int m_sendtime;	//����������-����ʱ��
};
typedef struct _slk_NetS_Talk SLK_NetS_Talk;	//��������������

struct _slk_NetS_FightIn {
	char m_srcpos;	//ս��λ��
	short m_shape;	//Ӣ��ͷ��
	char m_namelen;	//���ֳ���
	char m_name[22];	//Ӣ������
	char m_isenemy;	//�Ƿ������
	char m_posion;	//ս��λ��
	short m_corps;	//���ֱ��
	int m_curnumber;	//ʿ������
	int m_maxnumber;	//���ʿ��
	char m_ratenumber;	//ʿ������(0-100)
};
typedef struct _slk_NetS_FightIn SLK_NetS_FightIn;	//ս����ʼ

struct _slk_NetS_FightSet {
	short m_turn;	//��ǰ�غ�
	char m_fightflag;	//������־
};
typedef struct _slk_NetS_FightSet SLK_NetS_FightSet;	//�غϿ�ʼ/����/ս������

struct _slk_NetS_FightMove {
	char m_srcpos;	//ս��λ��
	char m_movstep;	//�ƶ�����
};
typedef struct _slk_NetS_FightMove SLK_NetS_FightMove;	//��λ�ƶ�ָ��

struct _slk_NetS_FightAction {
	char m_srcpos;	//����λ��
	char m_tgtpos;	//Ŀ��λ��
	char m_action;	//����
	int m_damage;	//�˺�
	int m_srcstat;	//����״̬
	int m_tgtstat;	//Ŀ��״̬
	char m_nexttgt;	//�Ƿ����¸�
};
typedef struct _slk_NetS_FightAction SLK_NetS_FightAction;	//��λ����ָ��

struct _slk_ArmyCorrdinate {
	int m_unit_index;	//��ͼ��Ԫ-���
	short m_posx;	//��ͼ��Ԫ-��ǰ����
	short m_posy;	//��ͼ��Ԫ-��ǰ����
};
typedef struct _slk_ArmyCorrdinate SLK_ArmyCorrdinate;	//��ͼ��ʾ��Ԫ-����λ��

struct _slk_NetS_CityResInfo {
	int m_wood;	//�ǳػ�����Դ��Ϣ
	int m_food;	//�ǳػ�����Դ��Ϣ
	int m_iron;	//�ǳػ�����Դ��Ϣ
	int m_mithril;	//�ǳػ�����Դ��Ϣ
	int m_gold;	//�ǳػ�����Դ��Ϣ
	char m_towncenter_level;	//�ǳػ�����Դ��Ϣ
	char m_enter_worldmap;	//�ǳػ�����Դ��Ϣ
};
typedef struct _slk_NetS_CityResInfo SLK_NetS_CityResInfo;	//�ǳ���Դ��Ϣ

struct _slk_NetS_BuildingBaseInfo {
	unsigned char m_kind;	//���н���������Ϣ-����
	char m_level;	//���н���������Ϣ-�ȼ�
	unsigned char m_offset;	//���н���������Ϣ-��������
};
typedef struct _slk_NetS_BuildingBaseInfo SLK_NetS_BuildingBaseInfo;	//һ������������Ϣ

struct _slk_NetS_CityBuildingList {
	short m_count;	//���н���������Ϣ-��������
	SLK_NetS_BuildingBaseInfo m_list[40];	//���н���������Ϣ
};
typedef struct _slk_NetS_CityBuildingList SLK_NetS_CityBuildingList;	//�ǳؽ���������Ϣ�б�

struct _slk_NetS_ActorInfo {
	int m_actorid;	//������������һ�����Ϣ-��ɫ���
	char m_name[22];	//������������һ�����Ϣ-��ɫ����
	char m_aclass;	//������������һ�����Ϣ-��ɫ����
	short m_shape;	//������������һ�����Ϣ-����
	short m_level;	//������������һ�����Ϣ-����
	i64 m_experience;	//������������һ�����Ϣ-����
	i64 m_experience_max;	//������������һ�����Ϣ-��������
	short m_leadership;	//������������һ�����Ϣ-�쵼��
	int m_token;	//������������һ�����Ϣ-��ʯ
	char m_viplevel;	//������������һ�����Ϣ-VIP�ȼ�
};
typedef struct _slk_NetS_ActorInfo SLK_NetS_ActorInfo;	//��ɫ������Ϣ

struct _slk_NetS_ItemUse {
	short m_itemoffset;	//��Ʒλ��
	short m_usenum;	//ʹ�ø�����װ��λ��
	int m_effres;	//Ӱ�죬һ��������Ч��ʾ
};
typedef struct _slk_NetS_ItemUse SLK_NetS_ItemUse;	//����ʹ��|װ��

struct _slk_NetS_ItemPut {
	short m_res_offset;	//��ƷԴ
	short m_res_num;	//��ƷԴ����
	short m_target_offset;	//��ƷĿ��
	short m_target_num;	//Ŀ�����
};
typedef struct _slk_NetS_ItemPut SLK_NetS_ItemPut;	//���߷���|ж��

struct _slk_NetS_ItemSettle {
	short m_itemnum;	//�������ĸ���
	short m_itemoffset[100];	//���õ���Ʒλ��
};
typedef struct _slk_NetS_ItemSettle SLK_NetS_ItemSettle;	//��������

struct _slk_NetS_LostItem {
	short m_itemoffset;	//��Ʒλ��
	short m_itemnum;	//����
	int m_targetid;	//����Ķ���id
	char m_path;	//;��
};
typedef struct _slk_NetS_LostItem SLK_NetS_LostItem;	//��ʧ����

struct _slk_NetS_GetItem {
	short m_itemoffset;	//��Ʒλ��
	int m_kind;	//��Ʒ����
	short m_type;	//��Ʒ����
	char m_color;	//��ɫ
	short m_num;	//����
	int m_targetid;	//����Ķ���id
	char m_path;	//;��
};
typedef struct _slk_NetS_GetItem SLK_NetS_GetItem;	//��õ���

struct _slk_ItemSmpInfo {
	short m_offset;	//��Ʒλ��
	short m_kind;	//��Ʒ����
	short m_type;	//��Ʒ����
	short m_num;	//����
	char m_colorlevel;	//��Ʒ��ɫ
};
typedef struct _slk_ItemSmpInfo SLK_ItemSmpInfo;	//���߼�����Ϣ

struct _slk_NetS_ItemList {
	char m_itemext;	//��չ�ĸ���
	short m_kindnum;	//��Ʒ����
	SLK_ItemSmpInfo m_item[100];	//��Ʒ
};
typedef struct _slk_NetS_ItemList SLK_NetS_ItemList;	//�����б�

struct _slk_ItemAttr {
	short m_type;	//��Ʒ������������
	short m_ability;	//��Ʒ����
	int m_value;	//��Ʒ����
	int m_addvalue;	//��Ʒ��������
};
typedef struct _slk_ItemAttr SLK_ItemAttr;	//��������

struct _slk_NetS_ItemInfo {
	short m_itemoffset;	//��Ʒλ��
	short m_itemkind;	//װ��kind,У��
	short m_type;	//��Ʒ����
	short m_level;	//һ��ȼ�
	char m_color_level;	//��ɫ�ּ�
	int m_price;	//�ۼ�
	short m_attr_num;	//���Ը���
	SLK_ItemAttr m_attr[10];	//����
};
typedef struct _slk_NetS_ItemInfo SLK_NetS_ItemInfo;	//������Ϣ

struct _slk_NetS_BuildTimeInfo {
	unsigned char m_offset;	//��������
	int m_time;	//�������ʱ��
	char m_state;	//����״̬
};
typedef struct _slk_NetS_BuildTimeInfo SLK_NetS_BuildTimeInfo;	//����ʱ����Ϣ

struct _slk_NetS_BuildTimeInfoList {
	unsigned char m_count;	//��ʱ����Ϣ�Ľ�������
	SLK_NetS_BuildTimeInfo m_timeinfo[16];	//�����ʱ����Ϣ
	int m_servertime;	//������ʱ���
};
typedef struct _slk_NetS_BuildTimeInfoList SLK_NetS_BuildTimeInfoList;	//�����Ľ���ʱ���б�

struct _slk_NetS_CityResChangeInfo {
	char m_restype;	//��Դ����
	int m_resvalue;	//��Դ����
	char m_path;	//�ı�;��
};
typedef struct _slk_NetS_CityResChangeInfo SLK_NetS_CityResChangeInfo;	//������Դ�ı�

struct _slk_NetS_BuildingAbilityInfo {
	unsigned char m_offset;	//��������
	int m_value[4];	//��������
};
typedef struct _slk_NetS_BuildingAbilityInfo SLK_NetS_BuildingAbilityInfo;	//һ������������Ϣ

struct _slk_NetS_AddMapUnit {
	int m_unit_index;	//��ͼ��Ԫ-���
	char m_type;	//��ͼ��Ԫ-����
	unsigned char m_shape;	//��ͼ��Ԫ-����
	char m_stat;	//��ͼ��Ԫ-״̬
	char m_lastway;	//��ͼ��Ԫ-����
	short m_posx;	//��ͼ��Ԫ-��ǰ����
	short m_posy;	//��ͼ��Ԫ-��ǰ����
	short m_tposx;	//��ͼ��Ԫ-Ŀ������
	short m_tposy;	//��ͼ��Ԫ-Ŀ������
	char m_namelen;	//��ͼ��Ԫ-���Ƴ���
	char m_name[22];	//��ͼ��Ԫ-����
	unsigned char m_level;	//��ͼ��Ԫ-�ȼ�
	unsigned char m_underfire_count;	//��ͼ��Ԫ-�����ٲ��ӹ���
	SLK_ArmyCorrdinate m_underfire_frompos[32];	//��ͼ��Ԫ-�����ٲ��ӹ�������
};
typedef struct _slk_NetS_AddMapUnit SLK_NetS_AddMapUnit;	//��ʾ��Ԫ�����ͼ

struct _slk_NetS_DelMapUnit {
	int m_unit_index;	//��ͼ��Ԫ-���
};
typedef struct _slk_NetS_DelMapUnit SLK_NetS_DelMapUnit;	//��ʾ��Ԫ�뿪��ͼ

struct _slk_NetS_WorldMapInfo {
	short m_map_width;	//�����ͼ��С����������
	short m_map_height;	//�����ͼ��С����������
	char m_area_width;	//�����С��һ�����򼸸���ͼ���ӣ�
	char m_area_height;	//�����С��һ�����򼸸���ͼ���ӣ�
	short m_map_area_xnum;	//һ����ͼ�ж�������
	short m_map_area_ynum;	//һ����ͼ�ж�������
	short m_my_city_posx;	//�ҵĳǳ�λ��
	short m_my_city_posy;	//�ҵĳǳ�λ��
	int m_my_city_unit_index;	//�ҵĳǳص�ͼ��Ԫ-���
};
typedef struct _slk_NetS_WorldMapInfo SLK_NetS_WorldMapInfo;	//�����ͼ��Ϣ

struct _slk_NetS_MapUnitCorrdinate {
	int m_unit_index;	//��ͼ��Ԫ-���
	short m_posx;	//��ͼ��Ԫ-��ǰ����
	short m_posy;	//��ͼ��Ԫ-��ǰ����
};
typedef struct _slk_NetS_MapUnitCorrdinate SLK_NetS_MapUnitCorrdinate;	//��ͼ��ʾ��Ԫλ��

struct _slk_NetS_MarchRoute {
	int m_from_unit_index;	//�о�·��-������Ԫ
	short m_from_posx;	//�о�·��-����λ��
	short m_from_posy;	//�о�·��-����λ��
	int m_to_unit_index;	//�о�·��-���ﵥԪ
	short m_to_posx;	//�о�·��-����λ��
	short m_to_posy;	//�о�·��-����λ��
};
typedef struct _slk_NetS_MarchRoute SLK_NetS_MarchRoute;	//��������ʱ�򴴽����о�·��

struct _slk_NetS_OutsideMapInfo {
	short m_map_width;	//Ұ���ͼ��С����������
	short m_map_height;	//Ұ���ͼ��С����������
	short m_my_city_posx;	//�ҵĳǳ�λ��
	short m_my_city_posy;	//�ҵĳǳ�λ��
	short m_match_city_posx;	//���ֳǳ�λ��
	short m_match_city_posy;	//���ֳǳ�λ��
};
typedef struct _slk_NetS_OutsideMapInfo SLK_NetS_OutsideMapInfo;	//Ұ���ͼ��Ϣ

struct _slk_NetS_MailSimpleInfo {
	i64 m_mailid;	//�ʼ�id
	char m_type;	//�ʼ�����
	short m_from_name_length;	//���������Ƴ���
	char m_from_name[22];	//����������
	short m_title_length;	//���ⳤ��
	char m_title[32];	//����
	short m_simple_content_length;	//�����ݳ���
	char m_simple_content[32];	//������
	char m_has_attach;	//�Ƿ��и���
	int m_recvtime;	//����ʱ��
	int m_deltime;	//ɾ��ʱ��
	char m_read;	//�Ѷ����
	char m_attach_isget;	//�����Ƿ���ȡ
};
typedef struct _slk_NetS_MailSimpleInfo SLK_NetS_MailSimpleInfo;	//�ʼ�����Ϣ

struct _slk_NetS_MailList {
	short m_count;	//�ʼ�����
	SLK_NetS_MailSimpleInfo m_list[10];	//�ʼ��б�
};
typedef struct _slk_NetS_MailList SLK_NetS_MailList;	//�ʼ��б�

struct _slk_NetS_MailFullInfo {
	i64 m_mailid;	//�ʼ�id
	short m_content_length;	//�������ݳ���
	char m_content[1024];	//��������
	short m_attach_length;	//�������ݳ���
	char m_attach[128];	//��������
	char m_read;	//�Ѷ����
};
typedef struct _slk_NetS_MailFullInfo SLK_NetS_MailFullInfo;	//�ʼ�����������Ϣ

struct _slk_NetS_FightMsgSimpleInfo {
	i64 m_msgid;	//ս����Ϣid
	char m_read;	//�Ѷ����
	int m_recvtime;	//ʱ��
	char m_status;	//״̬
	char m_target_type;	//ս��Ŀ������
	short m_title_length;	//ս�����ⳤ��
	char m_title[32];	//ս������
};
typedef struct _slk_NetS_FightMsgSimpleInfo SLK_NetS_FightMsgSimpleInfo;	//ս������Ϣ

struct _slk_NetS_FightMsgList {
	short m_count;	//ս���б�����
	SLK_NetS_FightMsgSimpleInfo m_list[10];	//ս���б�
};
typedef struct _slk_NetS_FightMsgList SLK_NetS_FightMsgList;	//ս���б�

struct _slk_NetS_FightMsgInfo {
	i64 m_msgid;	//ս����Ϣid
	char m_read;	//�Ѷ����
	int m_recvtime;	//ʱ��
	short m_posx;	//����������
	short m_posy;	//����������
	char m_status;	//״̬
	char m_target_type;	//�Է�����
	int m_target_id;	//�Է����Ͷ�Ӧid
	short m_target_posx;	//�Է�λ��
	short m_target_posy;	//�Է�λ��
	int m_wood;	//��Դ-ľͷ
	int m_iron;	//��Դ-��
	int m_food;	//��Դ-ʳ��
	i64 m_recordid;	//¼��
	short m_title_length;	//���ⳤ��
	char m_title[32];	//����
};
typedef struct _slk_NetS_FightMsgInfo SLK_NetS_FightMsgInfo;	//ս��������Ϣ

struct _slk_NetS_BuildingAbilityList {
	short m_count;	//���������б����
	SLK_NetS_BuildingAbilityInfo m_list[35];	//���������б�
};
typedef struct _slk_NetS_BuildingAbilityList SLK_NetS_BuildingAbilityList;	//����������Ϣ�б�

struct _slk_NetS_ChatMaskList {
	short m_count;	//������������б�
	int m_chat_mask_list[64];	//������������б�
};
typedef struct _slk_NetS_ChatMaskList SLK_NetS_ChatMaskList;	//���������б�

struct _slk_NetS_TroopInfo {
	short m_corpstype;	//��������
	char m_level;	//�ȼ�
	int m_have;	//ӵ������
	int m_wound;	//��������
};
typedef struct _slk_NetS_TroopInfo SLK_NetS_TroopInfo;	//������Ϣ

struct _slk_NetS_TroopInfoList {
	short m_count;	//�б���
	SLK_NetS_TroopInfo m_troop_list[50];	//������Ϣ�б�
};
typedef struct _slk_NetS_TroopInfoList SLK_NetS_TroopInfoList;	//������Ϣ�б�

struct _slk_NetS_HeroSmpInfo {
	short m_offset;	//Ӣ������
	short m_herokind;	//Ӣ�����
	short m_level;	//Ӣ�۵ȼ�
	char m_official;	//Ӣ�۹�ְ
};
typedef struct _slk_NetS_HeroSmpInfo SLK_NetS_HeroSmpInfo;	//Ӣ�ۼ�����Ϣ

struct _slk_NetS_HeroList {
	short m_count;	//Ӣ���б����
	SLK_NetS_HeroSmpInfo m_list[32];	//Ӣ���б�
};
typedef struct _slk_NetS_HeroList SLK_NetS_HeroList;	//Ӣ���б�

struct _slk_NetS_HeroInfo {
	short m_offset;	//Ӣ������
	char m_name[22];	//Ӣ������
	short m_attack;	//Ӣ�۹�����
	short m_defance;	//Ӣ�۷���
	short m_agility;	//Ӣ������
	short m_hp;	//Ӣ��Ѫ��
	i64 m_experience;	//Ӣ�۾���
	short m_skill[4];	//Ӣ�ۼ���
	short m_equip[6];	//Ӣ��װ��
};
typedef struct _slk_NetS_HeroInfo SLK_NetS_HeroInfo;	//Ӣ����ϸ��Ϣ

struct _slk_NetS_CityBattleTroopList {
	short m_corps;	//����-ʿ������
	char m_level;	//����-ʿ���ȼ�
	int m_count;	//����-ʿ������
};
typedef struct _slk_NetS_CityBattleTroopList SLK_NetS_CityBattleTroopList;	//����-������Ϣ

struct _slk_NetS_CityBattleActorList {
	char m_name[22];	//����-�������
	int m_actor_index;	//����-�������
};
typedef struct _slk_NetS_CityBattleActorList SLK_NetS_CityBattleActorList;	//����-�����Ϣ

struct _slk_NetS_CityBattleInfo {
	char m_actor_count;	//����-��Ҹ���
	SLK_NetS_CityBattleActorList m_actor_list[5];	//����-����б�
	char m_troop_count;	//����-���ָ���
	SLK_NetS_CityBattleTroopList m_troop_list[10];	//����-�����б�
	short m_pos_x;	//����-�����ߵ�����
	short m_pos_y;	//����-�����ߵ�����
	int m_remaining_time;	//����-ʣ��ʱ��
	char m_type;	//����-�ж����(��ͨ����/���ṥ��)
};
typedef struct _slk_NetS_CityBattleInfo SLK_NetS_CityBattleInfo;	//����-ս��������Ϣ

int struct_NetS_Login_send( char **pptr, int *psize, SLK_NetS_Login *pValue );
int struct_ListInfo_send( char **pptr, int *psize, SLK_ListInfo *pValue );
int struct_NetS_List_send( char **pptr, int *psize, SLK_NetS_List *pValue );
int struct_NetS_Create_send( char **pptr, int *psize, SLK_NetS_Create *pValue );
int struct_NetS_EnterInfo_send( char **pptr, int *psize, SLK_NetS_EnterInfo *pValue );
int struct_NetS_Delete_send( char **pptr, int *psize, SLK_NetS_Delete *pValue );
int struct_NetS_Heart_send( char **pptr, int *psize, SLK_NetS_Heart *pValue );
int struct_NetS_Notify_send( char **pptr, int *psize, SLK_NetS_Notify *pValue );
int struct_NetS_Talk_send( char **pptr, int *psize, SLK_NetS_Talk *pValue );
int struct_NetS_FightIn_send( char **pptr, int *psize, SLK_NetS_FightIn *pValue );
int struct_NetS_FightSet_send( char **pptr, int *psize, SLK_NetS_FightSet *pValue );
int struct_NetS_FightMove_send( char **pptr, int *psize, SLK_NetS_FightMove *pValue );
int struct_NetS_FightAction_send( char **pptr, int *psize, SLK_NetS_FightAction *pValue );
int struct_ArmyCorrdinate_send( char **pptr, int *psize, SLK_ArmyCorrdinate *pValue );
int struct_NetS_CityResInfo_send( char **pptr, int *psize, SLK_NetS_CityResInfo *pValue );
int struct_NetS_BuildingBaseInfo_send( char **pptr, int *psize, SLK_NetS_BuildingBaseInfo *pValue );
int struct_NetS_CityBuildingList_send( char **pptr, int *psize, SLK_NetS_CityBuildingList *pValue );
int struct_NetS_ActorInfo_send( char **pptr, int *psize, SLK_NetS_ActorInfo *pValue );
int struct_NetS_ItemUse_send( char **pptr, int *psize, SLK_NetS_ItemUse *pValue );
int struct_NetS_ItemPut_send( char **pptr, int *psize, SLK_NetS_ItemPut *pValue );
int struct_NetS_ItemSettle_send( char **pptr, int *psize, SLK_NetS_ItemSettle *pValue );
int struct_NetS_LostItem_send( char **pptr, int *psize, SLK_NetS_LostItem *pValue );
int struct_NetS_GetItem_send( char **pptr, int *psize, SLK_NetS_GetItem *pValue );
int struct_ItemSmpInfo_send( char **pptr, int *psize, SLK_ItemSmpInfo *pValue );
int struct_NetS_ItemList_send( char **pptr, int *psize, SLK_NetS_ItemList *pValue );
int struct_ItemAttr_send( char **pptr, int *psize, SLK_ItemAttr *pValue );
int struct_NetS_ItemInfo_send( char **pptr, int *psize, SLK_NetS_ItemInfo *pValue );
int struct_NetS_BuildTimeInfo_send( char **pptr, int *psize, SLK_NetS_BuildTimeInfo *pValue );
int struct_NetS_BuildTimeInfoList_send( char **pptr, int *psize, SLK_NetS_BuildTimeInfoList *pValue );
int struct_NetS_CityResChangeInfo_send( char **pptr, int *psize, SLK_NetS_CityResChangeInfo *pValue );
int struct_NetS_BuildingAbilityInfo_send( char **pptr, int *psize, SLK_NetS_BuildingAbilityInfo *pValue );
int struct_NetS_AddMapUnit_send( char **pptr, int *psize, SLK_NetS_AddMapUnit *pValue );
int struct_NetS_DelMapUnit_send( char **pptr, int *psize, SLK_NetS_DelMapUnit *pValue );
int struct_NetS_WorldMapInfo_send( char **pptr, int *psize, SLK_NetS_WorldMapInfo *pValue );
int struct_NetS_MapUnitCorrdinate_send( char **pptr, int *psize, SLK_NetS_MapUnitCorrdinate *pValue );
int struct_NetS_MarchRoute_send( char **pptr, int *psize, SLK_NetS_MarchRoute *pValue );
int struct_NetS_OutsideMapInfo_send( char **pptr, int *psize, SLK_NetS_OutsideMapInfo *pValue );
int struct_NetS_MailSimpleInfo_send( char **pptr, int *psize, SLK_NetS_MailSimpleInfo *pValue );
int struct_NetS_MailList_send( char **pptr, int *psize, SLK_NetS_MailList *pValue );
int struct_NetS_MailFullInfo_send( char **pptr, int *psize, SLK_NetS_MailFullInfo *pValue );
int struct_NetS_FightMsgSimpleInfo_send( char **pptr, int *psize, SLK_NetS_FightMsgSimpleInfo *pValue );
int struct_NetS_FightMsgList_send( char **pptr, int *psize, SLK_NetS_FightMsgList *pValue );
int struct_NetS_FightMsgInfo_send( char **pptr, int *psize, SLK_NetS_FightMsgInfo *pValue );
int struct_NetS_BuildingAbilityList_send( char **pptr, int *psize, SLK_NetS_BuildingAbilityList *pValue );
int struct_NetS_ChatMaskList_send( char **pptr, int *psize, SLK_NetS_ChatMaskList *pValue );
int struct_NetS_TroopInfo_send( char **pptr, int *psize, SLK_NetS_TroopInfo *pValue );
int struct_NetS_TroopInfoList_send( char **pptr, int *psize, SLK_NetS_TroopInfoList *pValue );
int struct_NetS_HeroSmpInfo_send( char **pptr, int *psize, SLK_NetS_HeroSmpInfo *pValue );
int struct_NetS_HeroList_send( char **pptr, int *psize, SLK_NetS_HeroList *pValue );
int struct_NetS_HeroInfo_send( char **pptr, int *psize, SLK_NetS_HeroInfo *pValue );
int struct_NetS_CityBattleTroopList_send( char **pptr, int *psize, SLK_NetS_CityBattleTroopList *pValue );
int struct_NetS_CityBattleActorList_send( char **pptr, int *psize, SLK_NetS_CityBattleActorList *pValue );
int struct_NetS_CityBattleInfo_send( char **pptr, int *psize, SLK_NetS_CityBattleInfo *pValue );

#endif
