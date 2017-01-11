// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _ELEMENTAL_H_
#define _ELEMENTAL_H_

#include "../common/mmo.h"

#include "status.h" // struct s_status_data, struct s_status_change
#include "unit.h" // struct s_unit_data


#define MIN_ELETHINKTIME 100
#define MIN_ELEDISTANCE 2
#define MAX_ELEDISTANCE 5

#define EL_MODE_AGGRESSIVE (e_MonsterMode::CANMOVE|e_MonsterMode::AGGRESSIVE|e_MonsterMode::CANATTACK)
#define EL_MODE_ASSIST (e_MonsterMode::CANMOVE|e_MonsterMode::ASSIST)
#define EL_MODE_PASSIVE e_MonsterMode::CANMOVE

///Enum of Elemental Skill Mode
enum e_elemental_skillmode : uint8 {
	EL_SKILLMODE_PASSIVE    = 0x1,
	EL_SKILLMODE_ASSIST     = 0x2,
	EL_SKILLMODE_AGGRESSIVE = 0x4,
};

///Enum of Elemental ID
enum e_elemental_elementalid : uint16 {
	ELEMENTALID_AGNI_S = 2114,
	ELEMENTALID_AGNI_M,
	ELEMENTALID_AGNI_L,
	ELEMENTALID_AQUA_S,
	ELEMENTALID_AQUA_M,
	ELEMENTALID_AQUA_L,
	ELEMENTALID_VENTUS_S,
	ELEMENTALID_VENTUS_M,
	ELEMENTALID_VENTUS_L,
	ELEMENTALID_TERA_S,
	ELEMENTALID_TERA_M,
	ELEMENTALID_TERA_L,
};

struct s_elemental_skill {
	unsigned short id, lv;
	short mode;
};

struct s_elemental_db {
	int class_;
	char sprite[NAME_LENGTH], name[NAME_LENGTH];
	unsigned short lv;
	short range2, range3;
	struct s_status_data status;
	struct s_view_data vd;
	struct s_elemental_skill skill[MAX_ELESKILLTREE];
};

extern struct s_elemental_db elemental_db[MAX_ELEMENTAL_CLASS];

struct s_elemental_data {
	struct s_block_list bl;
	struct s_unit_data ud;
	struct s_view_data *vd;
	struct s_status_data base_status, battle_status;
	struct s_status_change sc;
	struct regen_data regen;

	struct s_elemental_db *db;
	struct s_elemental elemental;

	int masterteleport_timer;
	struct s_map_session_data *master;
	int summon_timer;
	int skill_timer;

	unsigned last_thinktime, last_linktime, last_spdrain_time;
	short min_chase;
	int target_id, attacked_id;
};

bool elemental_class(int class_);
struct s_view_data * elemental_get_viewdata(int class_);

int elemental_create(struct s_map_session_data *sd, int class_, unsigned int lifetime);
int elemental_data_received(struct s_elemental *ele, bool flag);
int elemental_save(struct s_elemental_data *ed);

int elemental_change_mode_ack(struct s_elemental_data *ed, enum e_elemental_skillmode skill_mode);
int elemental_change_mode(struct s_elemental_data *ed, e_MonsterMode mode);

void elemental_heal(struct s_elemental_data *ed, int hp, int sp);
int elemental_dead(struct s_elemental_data *ed);

int elemental_delete(struct s_elemental_data *ed);
void elemental_summon_stop(struct s_elemental_data *ed);

int elemental_get_lifetime(struct s_elemental_data *ed);

int elemental_unlocktarget(struct s_elemental_data *ed);
bool elemental_skillnotok(uint16 skill_id, struct s_elemental_data *ed);
int elemental_set_target( struct s_map_session_data *sd, struct s_block_list *bl );
int elemental_clean_single_effect(struct s_elemental_data *ed, uint16 skill_id);
int elemental_clean_effect(struct s_elemental_data *ed);
int elemental_action(struct s_elemental_data *ed, struct s_block_list *bl, unsigned int tick);
struct s_skill_condition elemental_skill_get_requirements(uint16 skill_id, uint16 skill_lv);

#define elemental_stop_walking(ed, type) unit_stop_walking(&(ed)->bl, type)
#define elemental_stop_attack(ed) unit_stop_attack(&(ed)->bl)

void read_elemental_skilldb(void);
void reload_elementaldb(void);
void reload_elemental_skilldb(void);
void do_init_elemental(void);
void do_final_elemental(void);

#endif /* _ELEMENTAL_H_ */
