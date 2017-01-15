// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef CHANNEL_H
#define	CHANNEL_H

#include "../common_old/cbasetypes.h"

//namespace rA {

struct s_map_session_data;
struct s_guild;
struct s_DBMap;

#define CHAN_NAME_LENGTH 20
#define CHAN_MSG_LENGTH 150

enum e_Channel_Opt {
	CHAN_OPT_BASE		= 0,
	CHAN_OPT_ANNOUNCE_JOIN	= 1,	//display message when join or leave
	CHAN_OPT_MSG_DELAY	= 2,
	CHAN_OPT_COLOR_OVERRIDE = 3,
};

enum e_Channel_Type {
	CHAN_TYPE_PUBLIC	= 0,	//config file made
	CHAN_TYPE_PRIVATE	= 1,	//user made
	CHAN_TYPE_MAP		= 2,	//made by map
	CHAN_TYPE_ALLY		= 3,	//guild
};

struct s_Channel_Config {
	unsigned long *colors;		//color avail int list
	char **colors_name;		//colors avail name list
	unsigned char colors_count;	//color avail count
	unsigned char map_chcolor, ally_chcolor; //msg color for map, ally
	bool map_enable, ally_enable, user_chenable; //map, ally, users channels enable ?
	bool map_autojoin, ally_autojoin;	//do user auto join in mapchange, guildjoin ?
	char map_chname[CHAN_NAME_LENGTH], ally_chname[CHAN_NAME_LENGTH]; //channel name for map and ally
	bool closing;			//server is closing
};
extern struct s_Channel_Config channel_config;

struct s_Channel {
	char name[CHAN_NAME_LENGTH];	//channel name
	char pass[CHAN_NAME_LENGTH];	//channel password
	unsigned char color;		//msg color
	s_DBMap *users;			//users in channel charid list
	s_DBMap *banned;			//users banned from channel charid list
	enum e_Channel_Opt opt;		//flag for some treatement
	enum e_Channel_Type type;		//type of channel
	unsigned int owner;		//if chan_type private charid of creator
	uint16 m;			//if chan_type map guild_id
	int gid;			//if chan_type guild type guild_id
	unsigned char msg_delay;	//delay in second if opt_msg_delay
};

s_DBMap* channel_get_db(void);

struct s_Channel* channel_create(char *name, char *pass, unsigned char color, enum e_Channel_Type chantype, int val);
int channel_delete(struct s_Channel *channel);

int channel_join(struct s_Channel *channel, struct s_map_session_data *sd);
int channel_mjoin(struct s_map_session_data *sd);
int channel_gjoin(struct s_map_session_data *sd, int flag);
int channel_ajoin(struct s_guild *g);
int channel_clean(struct s_Channel *channel, struct s_map_session_data *sd, int flag);
int channel_pcquit(struct s_map_session_data *sd, int type);

int channel_send(struct s_Channel *channel, struct s_map_session_data *sd, const char *msg);
void channel_read_config(void);

int channel_chk(char *name, char *pass, int type);
struct s_Channel* channel_name2channel(char *chname, struct s_map_session_data *sd, int flag);
int channel_haspc(struct s_Channel *channel,struct s_map_session_data *sd);
int channel_haspcbanned(struct s_Channel *channel,struct s_map_session_data *sd);
int channel_pc_haschan(struct s_map_session_data *sd, struct s_Channel *channel);
int channel_display_list(struct s_map_session_data *sd, char *option);

int channel_pccreate(struct s_map_session_data *sd, char *chname, char *pass);
int channel_pcdelete(struct s_map_session_data *sd, char *chname);
int channel_pcjoin(struct s_map_session_data *sd, char *chname, char *pass);
int channel_pcleave(struct s_map_session_data *sd, char *chname);
int channel_pccolor(struct s_map_session_data *sd, char *chname, char *color);
int channel_pcbind(struct s_map_session_data *sd, char *chname);
int channel_pcunbind(struct s_map_session_data *sd);
int channel_pcban(struct s_map_session_data *sd, char *chname, char *pname, int flag);
int channel_pcsetopt(struct s_map_session_data *sd, char *chname, const char *option, const char *val);

void do_init_channel(void);
void do_final_channel(void);

//}

#endif	/* CHANNEL_H */
