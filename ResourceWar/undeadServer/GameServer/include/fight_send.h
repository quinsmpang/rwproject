#ifndef __FIGHT_SEND_H_
#define __FIGHT_SEND_H_

void fight_send_fightin(int actor_index);
void fight_send_fightset(int actor_index, short turn, char flag);
void fight_send_moveact(int actor_index);
int fight_send(int actor_index, int fightid);

#endif
