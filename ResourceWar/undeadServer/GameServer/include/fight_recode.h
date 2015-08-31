#ifndef __FIGHT_RECODE_H_
#define __FIGHT_RECODE_H_

int fightrec_init();
int fight_read(int fightid);
int fight_serialize_read(int size);
int fight_serialize_write();
int fight_save(int size);

#endif
