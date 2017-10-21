// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef CHANNEL_H
#define CHANNEL_H

#include "../common_old/cbasetypes.h"
#include "../common_old/mmo.h"

//namespace rA {

struct s_map_session_data;
struct s_guild;
struct s_DBMap;

#define CHAN_NAME_LENGTH    20
#define CHAN_MSG_LENGTH     150

enum e_Channel_Opt {
	CHAN_OPT_NONE           = 0,    ///< None
	CHAN_OPT_ANNOUNCE_SELF  = 0x01, ///< Shows info when player joined/left channel to self
	CHAN_OPT_ANNOUNCE_JOIN  = 0x02, ///< Shows info if player joined the channel
	CHAN_OPT_ANNOUNCE_LEAVE = 0x04, ///< Shows info if player left the channel
	CHAN_OPT_MSG_DELAY      = 0x08, ///< Enables chat delay
	CHAN_OPT_COLOR_OVERRIDE = 0x10, ///< Enables color channel be override by player's font color
	CHAN_OPT_CAN_CHAT       = 0x20, ///< Allows player to chat in the channel
	CHAN_OPT_CAN_LEAVE      = 0x40, ///< Allows player to leave the channel
	CHAN_OPT_AUTOJOIN       = 0x80, ///< Player will be autojoined to the channel

	CHAN_OPT_BASE           = CHAN_OPT_ANNOUNCE_SELF | CHAN_OPT_MSG_DELAY | CHAN_OPT_CAN_CHAT | CHAN_OPT_CAN_LEAVE,
};

enum e_Channel_Type {
	CHAN_TYPE_PUBLIC  = 0, ///< Config file made
	CHAN_TYPE_PRIVATE = 1, ///< User's channel
	CHAN_TYPE_MAP     = 2, ///< Local map
	CHAN_TYPE_ALLY    = 3, ///< Guild + its alliance
};

struct s_Channel {
	//unsigned short id;			  ///< Channel ID (unused yet)
	char            name[CHAN_NAME_LENGTH];  ///< Channel Name
	char            pass[CHAN_NAME_LENGTH];  ///< Channe display name
	char            alias[CHAN_NAME_LENGTH]; ///< Password
	e_Channel_Type  type;                    ///< Channel type @see enum e_Channel_Type
	unsigned long   color;                   ///< Channel color in BGR
	unsigned char   opt;                     ///< Channel options @see enum Channel_Opt
	unsigned short  msg_delay;               ///< Chat delay in miliseconds
	unsigned int    char_id;                 ///< If CHAN_TYPE_PRIVATE, owner is char_id of channel creator
	uint16          m;                       ///< If CHAN_TYPE_MAP, owner is map id
	int             gid;                     ///< If CHAN_TYPE_ALLY, owner is first logged guild_id
	s_DBMap*        users;                   ///< List of users
	s_DBMap*        banned;                  ///< List of banned chars -> char_id
	unsigned short  group_count;             ///< Number of group id
	unsigned short* groups;                  ///< List of group id, only these groups can join the channel
};

struct s_chan_banentry {
	uint32 char_id;
	char   char_name[NAME_LENGTH];
};
extern s_chan_banentry chan_banentry;

struct s_Channel_Config {
	unsigned long* colors;          ///< List of available colors
	char**         colors_name;     ///< Name list of available colors
	unsigned char  colors_count;    ///< Number of available colors

	/// Private channel default configs
	struct {
		unsigned char  opt;                ///< Options @see enum Channel_Opt
		unsigned long  color;              ///< Default color
		unsigned int   delay;              ///< Message delay
		unsigned short max_member;         ///< Max member for each channel
		unsigned       allow          : 1; ///< Allow private channel creation?
		unsigned       ban            : 1; ///< Allow player to ban
		unsigned       kick           : 1; ///< Allow player to kick
		unsigned       color_override : 1; ///< Owner cannot change the color_override
		unsigned       change_delay   : 1; ///< Owner cannot change the delay
	}
	          private_channel;

	s_Channel map_tmpl;  ///< Map channel default config
	s_Channel ally_tmpl; ///< Alliance channel default config

	bool      closing;   ///< Server is closing
};
extern struct s_Channel_Config channel_config;

s_DBMap* channel_get_db(void);

s_Channel* channel_create(struct s_Channel* tmp_chan);
s_Channel* channel_create_simple(char* name, char* pass, e_Channel_Type chantype, unsigned int owner);
int channel_delete(struct s_Channel* channel, bool force);

int channel_join(struct s_Channel* channel, s_map_session_data* sd);
int channel_mjoin(s_map_session_data* sd);
int channel_gjoin(s_map_session_data* sd, int flag);
int channel_ajoin(s_guild* g);
int channel_clean(struct s_Channel* channel, s_map_session_data* sd, int flag);
int channel_pcquit(s_map_session_data* sd, int type);

unsigned long channel_getColor(const char* color_str);

int channel_send(struct s_Channel* channel, s_map_session_data* sd, const char* msg);
void channel_read_config(void);

int channel_chk(char* name, char* pass, int type);
struct s_Channel* channel_name2channel(char* chname, s_map_session_data* sd, int flag);
int channel_haspc(struct s_Channel* channel, s_map_session_data* sd);
int channel_haspcbanned(struct s_Channel* channel, s_map_session_data* sd);
int channel_pc_haschan(s_map_session_data* sd, struct s_Channel* channel);
int channel_display_list(s_map_session_data* sd, char* option);

void channel_autojoin(s_map_session_data* sd);
bool channel_pccheckgroup(struct s_Channel* channel, int group_id);

int channel_pccreate(s_map_session_data* sd, char* chname, char* pass);
int channel_pcdelete(s_map_session_data* sd, char* chname);
int channel_pcjoin(s_map_session_data* sd, char* chname, char* pass);
int channel_pcleave(s_map_session_data* sd, char* chname);
int channel_pccolor(s_map_session_data* sd, char* chname, char* color);
int channel_pcbind(s_map_session_data* sd, char* chname);
int channel_pcunbind(s_map_session_data* sd);
int channel_pcban(s_map_session_data* sd, char* chname, char* pname, int flag);
int channel_pckick(s_map_session_data* sd, char* chname, char* pname);
int channel_pcsetopt(s_map_session_data* sd, char* chname, const char* option, const char* val);

void do_init_channel(void);
void do_final_channel(void);

//}

#endif  /* CHANNEL_H */
