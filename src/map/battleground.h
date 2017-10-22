// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include "../common_old/cbasetypes.h"
#include "../common_old/mmo.h" // struct party
#include "mapConst.h"

#define MAX_BG_MEMBERS    30

struct s_battleground_member_data {
	unsigned short      x, y;
	s_map_session_data* sd;
	unsigned            afk : 1;
};

struct s_battleground_data {
	unsigned int                      bg_id;
	unsigned char                     count;
	struct s_battleground_member_data members[MAX_BG_MEMBERS];
	// BG Cementery
	unsigned short                    mapindex, x, y;
	// Logout Event
	char                              logout_event[EVENT_NAME_LENGTH];
	char                              die_event[EVENT_NAME_LENGTH];
};

void do_init_battleground(void);
void do_final_battleground(void);

struct s_battleground_data* bg_team_search(int bg_id);
int bg_send_dot_remove(s_map_session_data* sd);
int bg_team_get_id(struct s_block_list* bl);
s_map_session_data* bg_getavailablesd(struct s_battleground_data* bg);

int bg_create(unsigned short mapindex, short rx, short ry, const char* ev, const char* dev);
int bg_team_join(int bg_id, s_map_session_data* sd);
int bg_team_delete(int bg_id);
int bg_team_leave(s_map_session_data* sd, int flag);
int bg_team_warp(int bg_id, unsigned short mapindex, short x, short y);
int bg_member_respawn(s_map_session_data* sd);
int bg_send_message(s_map_session_data* sd, const char* mes, int len);

#endif /* _BATTLEGROUND_H_ */
