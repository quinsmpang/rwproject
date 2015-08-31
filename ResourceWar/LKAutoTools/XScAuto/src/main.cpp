#include <windows.h>
#include "db.h"
#include "struct_config.h"

const char *GetRootPath()
{
	static char szPath[MAX_PATH];
	static char bFirstTime = 1;

	int slen;
	int tmpi;
	if( bFirstTime )
	{
		bFirstTime = 0;
		GetModuleFileName( NULL, szPath, sizeof(szPath) );
		slen = (int)strlen( szPath );
		for( tmpi = slen-1; tmpi > 0; tmpi-- )
		{
			if( szPath[tmpi] == '\\' || szPath[tmpi] == '/' )
			{
				szPath[tmpi] = 0;
				break;
			}
			else if( szPath[tmpi] == ':' )
			{
				szPath[tmpi+1] = 0;
				break;
			}
		}
	}
	return szPath;
}

int main( int argc, char *argv[] )
{
	SetCurrentDirectory( GetRootPath() );

	db_init();
	struct_create();

	db_closedata();
	return 0;
}
