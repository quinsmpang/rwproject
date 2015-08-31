#ifndef __NETCMD_CONFIG_H
#define __NETCMD_CONFIG_H

#define _NETPROCESSAUTO

struct _netcmd_config {
	int m_cmdid;
	char m_cmdstr[32];
	short m_structid;
	char m_target_type;
	char m_context[128];
};
typedef struct _netcmd_config SNetcmdConfig;

int netcmd_init();
int netcmd_reset();
int netcmd_create();

#endif
