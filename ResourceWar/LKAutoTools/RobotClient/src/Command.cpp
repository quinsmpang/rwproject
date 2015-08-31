#include "Command.h"
#include "Message.h"

#include "NetProcess.h"
SLK_Player g_Player[1000];

int CALLBACK client_process_pack( char *pack_data, int packlen );

void LK_CommandInit()
{
	memset( g_Player, 0, sizeof(SLK_Player)*1000 );
}

int Recv_Login( char *data, int size, int sock_index )
{
	int userid;
	
	//CLK_NetS_Login Value = {};
	//LKSET_DWORD_RECV( Value.m_result, (data), (*size) );
	//LKSET_BYTE_RECV( Value.m_ekey_type, (data), (*size) );
	//LKSET_MEM_RECV( &Value.m_challenge, (data), 16*sizeof(char), (*size) );
	//LKSET_MEM_RECV( &Value.m_username, (data), 51*sizeof(char), (*size) );
	//LKSET_MEM_RECV( &Value.m_token, (data), 65*sizeof(char), (*size) );
	int result = *((int *)data); data+=sizeof(int);size+=sizeof(int);
	if ( result != 0 )
	{
		return 0;
	}
	g_Player[sock_index].stat = STAT_LOGIN;
	return 0;
}

int Recv_Logout( char *data, int size, int sock_index )
{
	if( *data >= 0 )
	{
		memset( &g_Player[sock_index], 0, sizeof(SLK_Player) );
		g_Player[sock_index].stat = STAT_LOGOUT;
		return 0;
	}
	return -1;
}

int Recv_List( char *data, int size, int sock_index )
{
	int tmpsize;
	char *ptr;
	int nActorNum;
	int tmpi;

	char offset;
	int tmpvalue;

	tmpsize = 0;
	ptr = data;

	nActorNum = *ptr; ptr+=sizeof(char);tmpsize+=sizeof(char);
	g_Player[sock_index].actorid = -1;
	if( nActorNum < 0 )
		return -1;
	else if( nActorNum == 0 )
	{
		g_Player[sock_index].stat = STAT_LIST;
		return 0;
	}
	else if ( nActorNum == 1 )
	{
		tmpvalue = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		offset = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		tmpvalue = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		g_Player[sock_index].actorid = *((int *)ptr); ptr+=sizeof(int);tmpsize+=sizeof(int);

		g_Player[sock_index].stat = STAT_LIST;
		return 0;
	}
	for( tmpi = 0; tmpi < nActorNum; tmpi++ )
	{
		offset = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		tmpvalue = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		tmpvalue = *((int *)ptr); ptr+=sizeof(int);tmpsize+=sizeof(int);
		tmpvalue = *((int *)ptr); ptr+=sizeof(int);tmpsize+=sizeof(int);
		tmpvalue = *((int *)ptr); ptr+=sizeof(int);tmpsize+=sizeof(int);
		//strcpy( ListInfo.name, (char *)ptr );
		ptr+=NAME_SIZE*sizeof(char);tmpsize+=NAME_SIZE*sizeof(char);

		tmpvalue = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		tmpvalue = *((char *)ptr); ptr+=sizeof(char);tmpsize+=sizeof(char);
		tmpvalue = *((short *)ptr); ptr+=sizeof(short);tmpsize+=sizeof(short);

		if( offset == 0 )
			g_Player[sock_index].actorid = 0;

	}
	g_Player[sock_index].stat = STAT_LIST;

	return 0;
}

int Recv_Create( char *data, int size, int sock_index )
{
	int tmpsize;
	char *ptr;
	tmpsize = 0;
	ptr = data;
	char result = *((char *)ptr); ptr += sizeof(char); tmpsize += sizeof(char);
	int actorid = *((int *)ptr); ptr += sizeof(int); tmpsize += sizeof(int);
	if ( result < 0 )
	{
		return 0;
	}
	// 这里直接进入游戏
	Net_EnterGame( actorid, sock_index );
	return 0;
}

int Recv_EnterGame( char *data, int size, int sock_index )
{
	char result;
	int tmpsize;
	char *ptr;

	unsigned short	nMapID;

	tmpsize = 0;
	ptr = data;

	result = *ptr; ptr+=sizeof(char);tmpsize+=sizeof(char);
	if( result < 0 )
		return -1;
	else if( result == 0 )
	{
		return 0;
	}
	else if( result == 1 )
	{
		g_Player[sock_index].stat = STAT_ENTER;
	}
	else if( result == 2 )
	{
		// 即将进入战斗
		//g_Player[sock_index].stat = STAT_FIGHT;
		g_Player[sock_index].stat = STAT_ENTER;
	}
	return 0;
}

int Recv_Move( char *data, int size, int sock_index )
{
	return 0;
}

int Recv_Talk( char *data, int size, int sock_index )
{
	return 0;
}

int Recv_Fight( char *data, int size, int sock_index )
{
	// 还要设置一下坐标啊
	g_Player[sock_index].stat = STAT_FIGHT;
	return 0;
}

int Recv_FightProcess( char *data, int size, int sock_index )
{

	int tmpi,tmpj;
	int tmpsize;
	char *ptr;

	char nGrade[2];
	int fightsize;

	int tmpv;
	int tmpx;

	g_Player[sock_index].stat = STAT_FIGHTWAIT;
	g_Player[sock_index].waitframe = 16;

	tmpsize = 0;
	ptr = data;

	tmpi = 0;

	ptr+=sizeof(char);	tmpsize+=sizeof(char);
	fightsize = size-1;
	while( fightsize > tmpsize )
	{
		tmpv = *((char *)ptr);		ptr+=sizeof(char);	tmpsize+=sizeof(char);
		if( tmpv < 0 )
			break;
		ptr+=sizeof(char);	tmpsize+=sizeof(char);
		ptr+=sizeof(short);	tmpsize+=sizeof(short);
		ptr+=sizeof(int);	tmpsize+=sizeof(int);
		ptr+=sizeof(short);	tmpsize+=sizeof(short);
		ptr+=sizeof(int);	tmpsize+=sizeof(int);

		tmpj = 0;
		tmpv = *((char *)ptr);			ptr+=sizeof(char);	tmpsize+=sizeof(char);
		while( tmpv )
		{
			ptr+=sizeof(char);	tmpsize+=sizeof(char);
			ptr+=sizeof(int);	tmpsize+=sizeof(int);
			ptr+=sizeof(int);	tmpsize+=sizeof(int);

			ptr+=sizeof(char);	tmpsize+=sizeof(char);
			ptr+=sizeof(char);	tmpsize+=sizeof(char);
			ptr+=sizeof(int);	tmpsize+=sizeof(int);

			ptr+=sizeof(short);	tmpsize+=sizeof(short);
			ptr+=sizeof(int);	tmpsize+=sizeof(int);
			tmpx = *((short *)ptr);		ptr+=sizeof(short);	tmpsize+=sizeof(short);
			if( tmpx & 0x08 )
			{
				ptr+=sizeof(int);	tmpsize+=sizeof(int);
				ptr+=NAME_SIZE*sizeof(char);tmpsize+=NAME_SIZE*sizeof(char);
				ptr+=sizeof(int);	tmpsize+=sizeof(int);
				ptr+=sizeof(int);	tmpsize+=sizeof(int);
			}
			ptr+=sizeof(char);tmpsize+=sizeof(char);
			ptr+=sizeof(char);tmpsize+=sizeof(char);

			tmpv = *((char *)ptr);			ptr+=sizeof(char);	tmpsize+=sizeof(char);
			if( tmpv == 2 )
			{
				if( g_Player[sock_index].stat2 == 1 )
				{
					g_Player[sock_index].stat = STAT_STEP18;
					g_Player[sock_index].waitframe = 12;
				}
				else if( g_Player[sock_index].stat2 == 2 )
				{
					g_Player[sock_index].stat = STAT_STEP51;
					g_Player[sock_index].waitframe = 12;
				}
				else if( g_Player[sock_index].stat2 == 3 )
				{
					g_Player[sock_index].stat = STAT_STEP52;
					g_Player[sock_index].waitframe = 12;
				}
				else
					g_Player[sock_index].stat = STAT_NORMAN;
				break;
			}
			tmpj++;
		}
		tmpi++;
	}

	ptr+=sizeof(char);tmpsize+=sizeof(char);
	ptr+=sizeof(char);tmpsize+=sizeof(char);

	tmpj = 0;
	tmpv = *((char *)ptr);			ptr+=sizeof(char);	tmpsize+=sizeof(char);
	while( tmpv )
	{
		if( tmpv == 2 )
		{
			if( g_Player[sock_index].stat2 == 1 )
			{
				g_Player[sock_index].stat = STAT_STEP18;
				g_Player[sock_index].waitframe = 12;
			}
			else if( g_Player[sock_index].stat2 == 2 )
			{
				g_Player[sock_index].stat = STAT_STEP51;
				g_Player[sock_index].waitframe = 12;
			}
			else if( g_Player[sock_index].stat2 == 3 )
			{
				g_Player[sock_index].stat = STAT_STEP52;
				g_Player[sock_index].waitframe = 12;
			}
			else
				g_Player[sock_index].stat = STAT_NORMAN;
			break;
		}
		ptr+=sizeof(char);	tmpsize+=sizeof(char);
		ptr+=sizeof(int);	tmpsize+=sizeof(int);
		ptr+=sizeof(int);	tmpsize+=sizeof(int);
		ptr+=sizeof(int);	tmpsize+=sizeof(int);

		tmpv = *((short *)ptr); ptr+=sizeof(short);	tmpsize+=sizeof(short);
		if ( tmpv != 0 )
		{
			ptr+=sizeof(char)*16;	tmpsize+=sizeof(char)*16;
		}
		ptr+=sizeof(short);	tmpsize+=sizeof(short);

		tmpv = *((char *)ptr);			ptr+=sizeof(char);	tmpsize+=sizeof(char);
		tmpj++;
	}
	nGrade[0] = *((char *)ptr);			ptr+=sizeof(char);	tmpsize+=sizeof(char);
	nGrade[1] = *((char *)ptr);			ptr+=sizeof(char);	tmpsize+=sizeof(char);
	if( nGrade[0] < 0 )
		g_Player[sock_index].stat = STAT_NORMAN;
	return 0;
}

int Recv_Coordinate( char *data, int size, int sock_index )
{
	int tmpsize;
	char *ptr;

	int actorid;
	short posx;
	short posy;

	tmpsize = 0;
	ptr = data;

	actorid = *((int *)ptr);		ptr+=sizeof(int);	tmpsize+=sizeof(int);
	posx = *((short *)ptr);		ptr+=sizeof(short);	tmpsize+=sizeof(short);
	posy = *((short *)ptr);		ptr+=sizeof(short);	tmpsize+=sizeof(short);

	if( actorid <= 0 || actorid == g_Player[sock_index].actorid )
	{
		g_Player[sock_index].posx = posx;
		g_Player[sock_index].posy = posy;
	}
	return 0;
}

int Recv_StatChange( char *data, int size, int sock_index )
{
	int tmpsize;
	char *ptr;

	int nActorID;
	char nStat;
	short nPosX;
	short nPosY;

	tmpsize = 0;
	ptr = data;

	nActorID		= *((int *)ptr)		;ptr+=sizeof(int);	tmpsize+=sizeof(int);
	nStat			= *((char *)ptr)		;ptr+=sizeof(char);	tmpsize+=sizeof(char);
	nPosX			= *((short *)ptr)		;ptr+=sizeof(short);	tmpsize+=sizeof(short);
	nPosY			= *((short *)ptr)		;ptr+=sizeof(short);	tmpsize+=sizeof(short);

	if( nPosX >=0 || nPosY >= 0 )
	{
		g_Player[sock_index].posx = nPosX;
		g_Player[sock_index].posy = nPosY;
	}
	return 0;
}


int LK_ProcessMsg( int cmd, char *msg, int size, int sock_index )
{
	switch( cmd )
	{
	case CMD_LOGIN:
		Recv_Login( msg, size, sock_index );
		break;
	case CMDS_ENTERINFO:
		break;
	case CMD_LIST:
		Recv_List( msg, size, sock_index );
		break;
	case CMD_CREATE:
		Recv_Create( msg, size, sock_index );
		break;
	case CMD_ENTERGAME:
		Recv_EnterGame( msg, size, sock_index );
		break;
	case CMD_MOVE:
		Recv_Move( msg, size, sock_index );
		break;
	case CMD_TALK:
		Recv_Talk( msg, size, sock_index );
		break;
	case CMD_FIGHT:
		Recv_Fight( msg, size, sock_index );
		break;
	case CMD_FIGHT_PROCESS:
		Recv_FightProcess( msg, size, sock_index );
		break;
	case CMD_COORDINATE:
		Recv_Coordinate( msg, size, sock_index );
		break;
	case CMD_STSTCHANGE:
		Recv_StatChange( msg, size, sock_index );
		break;
	default:
		break;
	}
	return 0;
}

