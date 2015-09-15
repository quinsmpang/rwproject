#include "server_struct_auto.h"
#include "server_netsend_auto.h"
#include "netfunc.h"
#include "actor_send.h"


extern int g_maxactornum;

int netsend_login_S( int actor_index, char send_type, SLK_NetS_Login *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LOGIN;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LOGIN; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Login_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_list_S( int actor_index, char send_type, SLK_NetS_List *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_List_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_create_S( int actor_index, char send_type, SLK_NetS_Create *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CREATE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CREATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Create_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_enterinfo_S( int actor_index, char send_type, SLK_NetS_EnterInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ENTERINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ENTERINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_EnterInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_delete_S( int actor_index, char send_type, SLK_NetS_Delete *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_DELETE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_DELETE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Delete_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heart_S( int actor_index, char send_type, SLK_NetS_Heart *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEART;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEART; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Heart_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_notify_S( int actor_index, char send_type, SLK_NetS_Notify *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_NOTIFY;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_NOTIFY; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Notify_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_talk_S( int actor_index, char send_type, SLK_NetS_Talk *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_TALK;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_TALK; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_Talk_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightin_S( int actor_index, char send_type, SLK_NetS_FightIn *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTIN;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTIN; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightIn_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightset_S( int actor_index, char send_type, SLK_NetS_FightSet *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTSET;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTSET; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightSet_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightmove_S( int actor_index, char send_type, SLK_NetS_FightMove *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTMOVE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTMOVE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightMove_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightaction_S( int actor_index, char send_type, SLK_NetS_FightAction *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTACTION;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTACTION; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightAction_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_actorinfo_S( int actor_index, char send_type, SLK_NetS_ActorInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ACTORINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ACTORINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ActorInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityresinfo_S( int actor_index, char send_type, SLK_NetS_CityResInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYRESINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYRESINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityResInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_citybuildinglist_S( int actor_index, char send_type, SLK_NetS_CityBuildingList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYBUILDINGLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYBUILDINGLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityBuildingList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_addmapunit_S( char *databuf, int size, SLK_NetS_AddMapUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ADDMAPUNIT;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ADDMAPUNIT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_AddMapUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_delmapunit_S( char *databuf, int size, SLK_NetS_DelMapUnit *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_DELMAPUNIT;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_DELMAPUNIT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_DelMapUnit_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_itemuse_S( int actor_index, char send_type, SLK_NetS_ItemUse *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMUSE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMUSE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemUse_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemput_S( int actor_index, char send_type, SLK_NetS_ItemPut *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMPUT;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMPUT; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemPut_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemsettle_S( int actor_index, char send_type, SLK_NetS_ItemSettle *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMSETTLE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMSETTLE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemSettle_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_lostitem_S( int actor_index, char send_type, SLK_NetS_LostItem *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_LOSTITEM;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_LOSTITEM; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_LostItem_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_getitem_S( int actor_index, char send_type, SLK_NetS_GetItem *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_GETITEM;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_GETITEM; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_GetItem_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_itemlist_S( int actor_index, char send_type, SLK_NetS_ItemList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_iteminfo_S( int actor_index, char send_type, SLK_NetS_ItemInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_ITEMINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_ITEMINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ItemInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildtimeinfo_S( int actor_index, char send_type, SLK_NetS_BuildTimeInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDTIMEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDTIMEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildTimeInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildtimeinfolist_S( int actor_index, char send_type, SLK_NetS_BuildTimeInfoList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDTIMEINFOLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDTIMEINFOLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildTimeInfoList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_cityreschangeinfo_S( int actor_index, char send_type, SLK_NetS_CityResChangeInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYRESCHANGEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYRESCHANGEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityResChangeInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingbaseinfo_S( int actor_index, char send_type, SLK_NetS_BuildingBaseInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGBASEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGBASEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingBaseInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingabilityinfo_S( int actor_index, char send_type, SLK_NetS_BuildingAbilityInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGABILITYINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGABILITYINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingAbilityInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_worldmapinfo_S( int actor_index, char send_type, SLK_NetS_WorldMapInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_WORLDMAPINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_WORLDMAPINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_WorldMapInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mapunitcorrdinate_S( char *databuf, int size, SLK_NetS_MapUnitCorrdinate *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAPUNITCORRDINATE;


	ptr = tmpbuf;
	tmpsize = 0;
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAPUNITCORRDINATE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MapUnitCorrdinate_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	if( tmpsize > size )
		return 0;

	memcpy( databuf, tmpbuf, tmpsize );
	return tmpsize;
}

int netsend_marchroute_S( int actor_index, char send_type, SLK_NetS_MarchRoute *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MARCHROUTE;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MARCHROUTE; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MarchRoute_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_outsidemapinfo_S( int actor_index, char send_type, SLK_NetS_OutsideMapInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_OUTSIDEMAPINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_OUTSIDEMAPINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_OutsideMapInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mailsimpleinfo_S( int actor_index, char send_type, SLK_NetS_MailSimpleInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAILSIMPLEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAILSIMPLEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MailSimpleInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_maillist_S( int actor_index, char send_type, SLK_NetS_MailList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAILLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAILLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MailList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_mailfullinfo_S( int actor_index, char send_type, SLK_NetS_MailFullInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_MAILFULLINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_MAILFULLINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_MailFullInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightmsgsimpleinfo_S( int actor_index, char send_type, SLK_NetS_FightMsgSimpleInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTMSGSIMPLEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTMSGSIMPLEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightMsgSimpleInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightmsglist_S( int actor_index, char send_type, SLK_NetS_FightMsgList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTMSGLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTMSGLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightMsgList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_fightmsginfo_S( int actor_index, char send_type, SLK_NetS_FightMsgInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_FIGHTMSGINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_FIGHTMSGINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_FightMsgInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_buildingabilitylist_S( int actor_index, char send_type, SLK_NetS_BuildingAbilityList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_BUILDINGABILITYLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_BUILDINGABILITYLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_BuildingAbilityList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_chatmasklist_S( int actor_index, char send_type, SLK_NetS_ChatMaskList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CHATMASKLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CHATMASKLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_ChatMaskList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_troopinfo_S( int actor_index, char send_type, SLK_NetS_TroopInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_TROOPINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_TROOPINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_TroopInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_troopinfolist_S( int actor_index, char send_type, SLK_NetS_TroopInfoList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_TROOPINFOLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_TROOPINFOLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_TroopInfoList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heroinfolist_S( int actor_index, char send_type, SLK_NetS_HeroList *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROINFOLIST;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROINFOLIST; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroList_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_heroinfo_S( int actor_index, char send_type, SLK_NetS_HeroInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_HEROINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_HEROINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_HeroInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}

int netsend_citybattleinfo_S( int actor_index, char send_type, SLK_NetS_CityBattleInfo *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr, *ptrsize;
	short cmd=CMDS_CITYBATTLEINFO;

	if( actor_index < 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDS_CITYBATTLEINFO; ptr+=sizeof(short); tmpsize+=sizeof(short);
	ptrsize = ptr;	ptr+=sizeof(short);tmpsize+=sizeof(short);

	struct_NetS_CityBattleInfo_send( &ptr, &tmpsize, pValue );

	*(short *)ptrsize = tmpsize - (int)sizeof(short)*2;
	*(unsigned short *)tmpbuf = tmpsize;

	actor_senddata( actor_index, send_type, tmpbuf, tmpsize );
	return 0;
}


int netsend_logined_S( int actor_index, char send_type, SLK_NetU_Logined *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd=CMDC_LOGINED;

	if( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_LOGINED; ptr+=sizeof(short); tmpsize+=sizeof(short);

	struct_NetU_Logined_send( &ptr, &tmpsize, pValue );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );
	return 0;
}

int netsend_timer_S( int actor_index, char send_type, char *pValue )
{
	char tmpbuf[2048];
	int tmpsize;
	char *ptrsubdata;
	char *ptr;
	short cmd=CMDC_TIMER;

	if( actor_index >= 0 )
		return -1;

	ptr = tmpbuf;
	tmpsize = 0;
	ptr+=sizeof(short);
	ptrsubdata = ptr;
	*(short *)ptr = CMDC_TIMER; ptr+=sizeof(short); tmpsize+=sizeof(short);

	LKSET_BYTE_SEND( ptr, pValue, tmpsize );

	*(unsigned short *)tmpbuf = tmpsize;

	readtoqueue( actor_index, tmpbuf, tmpsize+sizeof(short), 0 );
	return 0;
}

