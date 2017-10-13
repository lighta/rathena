// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _CHAT_H_
#define _CHAT_H_

#include "map.h" // struct s_block_list, CHATROOM_TITLE_SIZE

struct s_map_session_data;
struct s_chat_data;

#define MAX_CHAT_USERS 20

struct s_chat_data {
	struct s_block_list bl;            // data for this map object
	char title[CHATROOM_TITLE_SIZE]; // room title 
	char pass[CHATROOM_PASS_SIZE];   // password
	bool pub;                        // private/public flag
	uint8 users;                     // current user count
	uint8 limit;                     // join limit
	uint8 trigger;                   // number of users needed to trigger event
	uint32 zeny;						 // required zeny to join
	uint32 minLvl;					 // minimum base level to join
	uint32 maxLvl;					 // maximum base level allowed to join
	s_map_session_data* usersd[MAX_CHAT_USERS];
	struct s_block_list* owner;
	char npc_event[EVENT_NAME_LENGTH];
	s_DBMap* kick_list;				//DBMap of users who were kicked from this chat
};

int chat_createpcchat(s_map_session_data* sd, const char* title, const char* pass, int limit, bool pub);
int chat_joinchat(s_map_session_data* sd, int chatid, const char* pass);
int chat_leavechat(s_map_session_data* sd, bool kicked);
int chat_changechatowner(s_map_session_data* sd, const char* nextownername);
int chat_changechatstatus(s_map_session_data* sd, const char* title, const char* pass, int limit, bool pub);
int chat_kickchat(s_map_session_data* sd, const char* kickusername);

int chat_createnpcchat(struct s_npc_data* nd, const char* title, int limit, bool pub, int trigger, const char* ev, int zeny, int minLvl, int maxLvl);
int chat_deletenpcchat(struct s_npc_data* nd);
int chat_enableevent(struct s_chat_data* cd);
int chat_disableevent(struct s_chat_data* cd);
int chat_npckickall(struct s_chat_data* cd);

int chat_npckickchat(struct s_chat_data* cd, const char* kickusername);

#endif /* _CHAT_H_ */
