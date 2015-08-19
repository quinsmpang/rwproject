#ifndef __ACTORDATA_CONFIG_H
#define __ACTORDATA_CONFIG_H

int actordata_init();
int actordata_create();
char *actordata_getdatatype( short datatype, char *ptr, char *lkset );

int actorsave_create();
int actorsql_create();
#endif
