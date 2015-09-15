CC=g++
SRC = \
../AutoS/src/auto_actor.cpp \
../AutoS/src/auto_actor_hero.cpp \
../AutoS/src/auto_actor_item.cpp \
../AutoS/src/server_netrecv_auto.cpp \
../AutoS/src/server_netsend_auto.cpp \
../AutoS/src/server_process_auto.cpp \
../AutoS/src/server_structrecv_auto.cpp \
../AutoS/src/server_structsend_auto.cpp \
account.cpp \
actor.cpp \
actor_func.cpp \
actor_gmcmd.cpp \
actor_notify.cpp \
actor_send.cpp \
area.cpp \
army.cpp \
building.cpp \
city.cpp \
city_send.cpp \
client.cpp \
club.cpp \
db.cpp \
fight.cpp \
fight_calc.cpp \
fight_msg.cpp \
fight_recode.cpp \
fight_send.cpp \
gamelog.cpp \
gameproc.cpp \
global.cpp \
gmcmd.cpp \
hero.cpp \
item.cpp \
item_equip.cpp \
item_proc.cpp \
item_send.cpp \
login_net.cpp \
login_user.cpp \
mail_msg.cpp \
main.cpp \
mapunit.cpp \
plat.cpp \
robot.cpp \
script.cpp \
script_auto.cpp \
script_reg_auto.cpp \
shm.cpp \
shm_ao.cpp \
system.cpp \
system_askinfo.cpp \
timecalc.cpp \
timegmcmd.cpp \
troop.cpp \
wildsite.cpp \
worldmap.cpp \
wqueue.cpp \


INCLUDE = -I ../include  -I ../AutoS/include -I  ../../LKUtils/include  -I ../../LKNetServer/include  -I ../../Lua/src  -I /usr/local/mysql/include/mysql
LIBRARY = -L ../../../public/lib -L /usr/local/mysql/lib/mysql
OBJ = $(SRC:.cpp=.o)
LIB = -llua -llknet -llkutils -lmysqlclient -lpthread -ldl -lm
EXE = ../../../exec/undeadServer
CFLAGS = -g
$(EXE) : $(OBJ)
	$(CC)  $(CFLAGS) $(INCLUDE) $(LIBRARY) -o $(EXE) $(CF) $(OBJ) $(LIB)


../AutoS/src/auto_actor.o: ../AutoS/src/auto_actor.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_actor_hero.o: ../AutoS/src/auto_actor_hero.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/auto_actor_item.o: ../AutoS/src/auto_actor_item.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_netrecv_auto.o: ../AutoS/src/server_netrecv_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_netsend_auto.o: ../AutoS/src/server_netsend_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_process_auto.o: ../AutoS/src/server_process_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_structrecv_auto.o: ../AutoS/src/server_structrecv_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
../AutoS/src/server_structsend_auto.o: ../AutoS/src/server_structsend_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@
account.o: account.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor.o: actor.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_func.o: actor_func.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_gmcmd.o: actor_gmcmd.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_notify.o: actor_notify.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
actor_send.o: actor_send.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
area.o: area.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
army.o: army.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
building.o: building.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
city.o: city.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
city_send.o: city_send.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
client.o: client.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
club.o: club.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
db.o: db.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
fight.o: fight.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
fight_calc.o: fight_calc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
fight_msg.o: fight_msg.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
fight_recode.o: fight_recode.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
fight_send.o: fight_send.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gamelog.o: gamelog.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gameproc.o: gameproc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
global.o: global.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
gmcmd.o: gmcmd.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
hero.o: hero.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item.o: item.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item_equip.o: item_equip.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item_proc.o: item_proc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
item_send.o: item_send.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
login_net.o: login_net.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
login_user.o: login_user.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
mail_msg.o: mail_msg.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
main.o: main.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
mapunit.o: mapunit.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
plat.o: plat.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
robot.o: robot.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
script.o: script.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
script_auto.o: script_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
script_reg_auto.o: script_reg_auto.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
shm.o: shm.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
shm_ao.o: shm_ao.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
system.o: system.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
system_askinfo.o: system_askinfo.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
timecalc.o: timecalc.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
timegmcmd.o: timegmcmd.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
troop.o: troop.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
wildsite.o: wildsite.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
worldmap.o: worldmap.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<
wqueue.o: wqueue.cpp 
	$(CC) $(CFLAGS) -c $(INCLUDE) $<


all: $(EXE)
clean :
	rm -f *.o
	rm -f ../AutoS/src/*.o
	rm -f $(EXE)
