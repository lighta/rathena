// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _UNIT_H_
#define _UNIT_H_

#include "../common_old/cbasetypes.h"

//#include "map.h"
enum e_sc_type : int16;
struct s_block_list;
struct s_unit_data;
struct s_map_session_data;
enum e_clr_type : uint8;

//#include "clif.h"  // clr_type
//#include "map.h" // struct s_block_list
#include "path.h" // struct walkpath_data
#include "skill.h" // struct skill_timerskill, struct s_skill_unit_group, struct s_skill_unit_group_tickset

extern const short dirx[DIR_MAX]; ///lookup to know where will move to x according dir
extern const short diry[DIR_MAX]; ///lookup to know where will move to y according dir

struct s_unit_data {
	struct s_block_list *bl; ///link to owner object BL_PC|BL_MOB|BL_PET|BL_NPC|BL_HOM|BL_MER|BL_ELEM
	struct s_walkpath_data walkpath;
	struct s_skill_timerskill *skilltimerskill[MAX_SKILLTIMERSKILL];
	struct s_skill_unit_group *skillunit[MAX_SKILLUNITGROUP];
	struct s_skill_unit_group_tickset skillunittick[MAX_SKILLUNITGROUPTICKSET];
	short attacktarget_lv;
	short to_x, to_y;
	short skillx, skilly;
	uint16 skill_id, skill_lv;
	int skilltarget;
	int skilltimer;
	int target;
	int target_to;
	int attacktimer;
	int walktimer;
	int chaserange;
	bool stepaction; //Action should be executed on step [Playtester]
	int steptimer; //Timer that triggers the action [Playtester]
	uint16 stepskill_id, stepskill_lv; //Remembers skill that should be casted on step [Playtester]
	unsigned int attackabletime;
	unsigned int canact_tick;
	unsigned int canmove_tick;
	bool immune_attack; ///< Whether the unit is immune to attacks
	uint8 dir;
	unsigned char walk_count;
	unsigned char target_count;
	struct s_udState {
		unsigned change_walk_target : 1 ;
		unsigned skillcastcancel : 1 ;
		unsigned attack_continue : 1 ;
		unsigned step_attack : 1;
		unsigned walk_easy : 1 ;
		unsigned running : 1;
		unsigned speed_changed : 1;
		unsigned walk_script : 1;
		unsigned blockedmove : 1;
		unsigned blockedskill : 1;
	} state;
	char walk_done_event[EVENT_NAME_LENGTH];
};

struct s_view_data {
#ifdef __64BIT__
	unsigned int class_; //why arch dependant ??? make no sense imo [lighta]
#endif
	unsigned short
#ifndef __64BIT__
		class_,
#endif
		weapon,
		shield, //Or left-hand weapon.
		robe,
		head_top,
		head_mid,
		head_bottom,
		hair_style,
		hair_color,
		cloth_color,
		body_style;
	char sex;
	unsigned dead_sit : 2;
};

/// Enum for unit_blown_immune
enum e_unit_blown {
	UB_KNOCKABLE = 0, // Can be knocked back / stopped
	UB_NO_KNOCKBACK_MAP, // On a WoE/BG map
	UB_MD_KNOCKBACK_IMMUNE, // Target is MD_KNOCKBACK_IMMUNE
	UB_TARGET_BASILICA, // Target is in Basilica area
	UB_TARGET_NO_KNOCKBACK, // Target has 'special_state.no_knockback'
	UB_TARGET_TRAP, // Target is a trap that cannot be knocked back
};

/// Enum for unit_stop_walking
enum e_unit_stop_walking {
	USW_NONE = 0x0, /// Unit will keep walking to their original destination
	USW_FIXPOS = 0x1, /// Issue a fixpos packet afterwards
	USW_MOVE_ONCE = 0x2, /// Force the unit to move one cell if it hasn't yet
	USW_MOVE_FULL_CELL = 0x4, /// Enable moving to the next cell when unit was already half-way there (may cause on-touch/place side-effects, such as a scripted map change)
	USW_FORCE_STOP = 0x8, /// Force stop moving, even if walktimer is currently INVALID_TIMER
	USW_ALL = 0xf,
};

// PC, MOB, PET

// Does walk action for unit
int unit_walktoxy(struct s_block_list *bl, short x, short y, unsigned char flag);
int unit_walktobl(struct s_block_list *bl, struct s_block_list *target, int range, unsigned char flag);
void unit_run_hit(struct s_block_list *bl, struct s_status_change *sc, struct s_map_session_data *sd, enum e_sc_type type);
bool unit_run(struct s_block_list *bl, struct s_map_session_data *sd, enum e_sc_type type);
int unit_calc_pos(struct s_block_list *bl, int tx, int ty, uint8 dir);
int unit_delay_walktoxy_timer(int tid, unsigned int tick, int id, intptr_t data);
int unit_delay_walktobl_timer(int tid, unsigned int tick, int id, intptr_t data);

// Causes the target object to stop moving.
int unit_stop_walking(struct s_block_list *bl,int type);
int unit_can_move(struct s_block_list *bl);
int unit_is_walking(struct s_block_list *bl);
int unit_set_walkdelay(struct s_block_list *bl, unsigned int tick, int delay, int type);

int unit_escape(struct s_block_list *bl, struct s_block_list *target, short dist);

// Instant unit changes
bool unit_movepos(struct s_block_list *bl, short dst_x, short dst_y, int easy, bool checkpath);
int unit_warp(struct s_block_list *bl, short map, short x, short y,enum e_clr_type type);
int unit_setdir(struct s_block_list *bl, unsigned char dir);
uint8 unit_getdir(struct s_block_list *bl);
int unit_blown(struct s_block_list* bl, int dx, int dy, int count, e_skill_blown flag);
e_unit_blown unit_blown_immune(struct s_block_list* bl, uint8 flag);

// Can-reach checks
bool unit_can_reach_pos(struct s_block_list *bl,int x,int y,int easy);
bool unit_can_reach_bl(struct s_block_list *bl,struct s_block_list *tbl, int range, int easy, short *x, short *y);

// Unit attack functions
void unit_stop_attack(struct s_block_list *bl);
int unit_attack(struct s_block_list *src,int target_id,int continuous);
int unit_cancel_combo(struct s_block_list *bl);

// Cast on a unit
int unit_skilluse_id(struct s_block_list *src, int target_id, uint16 skill_id, uint16 skill_lv);
int unit_skilluse_pos(struct s_block_list *src, short skill_x, short skill_y, uint16 skill_id, uint16 skill_lv);
int unit_skilluse_id2(struct s_block_list *src, int target_id, uint16 skill_id, uint16 skill_lv, int casttime, int castcancel);
int unit_skilluse_pos2( struct s_block_list *src, short skill_x, short skill_y, uint16 skill_id, uint16 skill_lv, int casttime, int castcancel);

// Step timer used for delayed attack and skill use
int unit_step_timer(int tid, unsigned int tick, int id, intptr_t data);
void unit_stop_stepaction(struct s_block_list *bl);

// Cancel unit cast
int unit_skillcastcancel(struct s_block_list *bl, char type);

int unit_counttargeted(struct s_block_list *bl);
int unit_set_target(struct s_unit_data* ud, int target_id);

// unit_data
void unit_dataset(struct s_block_list *bl);

// Remove unit
struct s_unit_data* unit_bl2ud(struct s_block_list *bl);
void unit_remove_map_pc(struct s_map_session_data *sd,enum e_clr_type clrtype);
void unit_free_pc(struct s_map_session_data *sd);
#define unit_remove_map(bl,clrtype) unit_remove_map_(bl,clrtype,__FILE__,__LINE__,__func__)
int unit_remove_map_(struct s_block_list *bl,enum e_clr_type clrtype, const char* file, int line, const char* func);
int unit_free(struct s_block_list *bl,enum e_clr_type clrtype);
int unit_changeviewsize(struct s_block_list *bl,short size);
int unit_changetarget(struct s_block_list *bl,va_list ap);

void do_init_unit(void);
void do_final_unit(void);


#endif /* _UNIT_H_ */
