// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _QUEST_H_
#define _QUEST_H_

#include "../common_old/cbasetypes.h"
#include "../common_old/strlib.h"

enum e_quest_state : uint8;
struct s_map_session_data;
//#include "map.h"

struct s_quest_dropitem {
	uint16 nameid;
	uint16 count;
	uint16 rate;
	uint16 mob_id;
	//uint8 bound;
	//bool isAnnounced;
	//bool isGUID;
};

struct s_quest_objective {
	uint16 mob;
	uint16 count;
};

struct s_quest_db {
	// TODO: find out if signed or unsigned in client
	int                       id;
	unsigned int              time;
	bool                      time_type;
	uint8                     objectives_count;
	struct s_quest_objective* objectives;
	uint8                     dropitem_count;
	struct s_quest_dropitem*  dropitem;
	StringBuf                 name;
};

extern struct s_quest_db quest_dummy;   ///< Dummy entry for invalid quest lookups

// Questlog check types
enum e_quest_check_type {
	HAVEQUEST, ///< Query the state of the given quest
	PLAYTIME,  ///< Check if the given quest has been completed or has yet to expire
	HUNTING,   ///< Check if the given hunting quest's requirements have been met
};

int quest_pc_login(s_map_session_data* sd);

int quest_add(s_map_session_data* sd, int quest_id);
int quest_delete(s_map_session_data* sd, int quest_id);
int quest_change(s_map_session_data* sd, int qid1, int qid2);
int quest_update_objective_sub(struct s_block_list* bl, va_list ap);
void quest_update_objective(s_map_session_data* sd, int mob_id);
int quest_update_status(s_map_session_data* sd, int quest_id, enum e_quest_state status);
int quest_check(s_map_session_data* sd, int quest_id, enum e_quest_check_type type);
void quest_clear(void);

struct s_quest_db* quest_search(int quest_id);

void do_init_quest(void);
void do_final_quest(void);
void do_reload_quest(void);

#endif
