#include <stdio.h>
#include "db.h"
#include "struct_config.h"

int main( int argc, char *argv[] )
{
	db_init();
	tabcreate();
	db_closedata();
	return 0;
}
