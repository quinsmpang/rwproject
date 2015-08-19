#include <stdio.h>
#include "db.h"
#include "netcmd_config.h"
#include "struct_config.h"
#include "actordata_config.h"

int main( int argc, char *argv[] )
{
	db_init();

	struct_init();
	actordata_init();
	actordata_create();

	struct_create();

	db_closedata();
	return 0;
}
