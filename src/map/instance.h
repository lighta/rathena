// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "../common_old/cbasetypes.h"
#include "../common_old/mmo.h" // struct point
#include "../common_old/strlib.h"

#include "script.h" // struct s_reg_db

enum e_send_target : uint8;
struct s_block_list;

#define MAX_INSTANCE_DATA       300     // Essentially how many instances we can create, but instance creation is primarily decided by MAX_MAP_PER_SERVER
#define MAX_MAP_PER_INSTANCE    255     // Max number of maps per instance (Enter map is counted as one) - Supports up to 255 maps

#define INSTANCE_NAME_LENGTH    (60 + 1)

typedef enum e_instance_state {
	INSTANCE_FREE,
	INSTANCE_IDLE,
	INSTANCE_BUSY
} e_instance_state;

enum e_instance_mode {
	IM_NONE,
	IM_CHAR,
	IM_PARTY,
	IM_GUILD,
	IM_CLAN,
	IM_MAX,
};

enum e_instance_enter {
	IE_OK = 0,
	IE_NOMEMBER,
	IE_NOINSTANCE,
	IE_OTHER
};

struct s_instance_map {
	int16 m, src_m;
};

struct s_instance_data {
	unsigned short          type;       ///< Instance DB ID
	enum e_instance_state   state;      ///< State of instance
	enum e_instance_mode    mode;       ///< Mode of instance
	int                     owner_id;   ///< Owner ID of instance
	unsigned int            keep_limit; ///< Life time of instance
	int                     keep_timer; ///< Remaining life time of instance
	unsigned int            idle_limit; ///< Idle time of instance
	int                     idle_timer; ///< Remaining idle time of instance
	struct s_reg_db         regs;       ///< Instance variables for scripts
	struct s_instance_map** map;        ///< Dynamic array of maps in instance
	uint8                   cnt_map;    ///< Number of maps in an instance
};

/// Instance DB entry struct
struct s_instance_db {
	unsigned short id;          ///< Instance ID
	StringBuf*     name;        ///< Instance name
	unsigned int   limit,       ///< Duration limit
	               timeout;     ///< Timeout limit
	struct s_MapInfo {
		StringBuf* mapname; ///< Mapname, the limit should be MAP_NAME_LENGTH_EXT
		short      x, y;    ///< Map coordinates
	}
	            enter;
	StringBuf** maplist;       ///< Used maps in instance, the limit should be MAP_NAME_LENGTH_EXT
	uint8       maplist_count; ///< Number of used maps
};

extern int                    instance_start;
extern struct s_instance_data instance_data[MAX_INSTANCE_DATA];

struct s_instance_db*         instance_searchtype_db(unsigned short instance_id);
struct s_instance_db*         instance_searchname_db(const char* name);
void instance_getsd(unsigned short instance_id, s_map_session_data** sd, enum e_send_target* target);

int instance_create(int owner_id, const char* name, enum e_instance_mode mode);
int instance_destroy(unsigned short instance_id);
enum e_instance_enter instance_enter(s_map_session_data* sd, unsigned short instance_id, const char* name, short x, short y);
int instance_reqinfo(s_map_session_data* sd, unsigned short instance_id);
int instance_addusers(unsigned short instance_id);
int instance_delusers(unsigned short instance_id);
int16 instance_mapname2mapid(const char* name, unsigned short instance_id);
int instance_addmap(unsigned short instance_id);

void instance_addnpc(struct s_instance_data* im);
void instance_readdb(void);
void instance_reload(void);
void do_reload_instance(void);
void do_init_instance(void);
void do_final_instance(void);

#if MAX_MAP_PER_INSTANCE > 255
	#error Too many maps per instance defined! Please adjust MAX_MAP_PER_INSTANCE to a lower value.
#endif

#endif
