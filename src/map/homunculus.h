// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _HOMUNCULUS_H_
#define _HOMUNCULUS_H_

#include "../common_old/cbasetypes.h"
#include "status.h" // struct s_status_data, struct s_status_change
#include "unit.h" // struct s_unit_data

#ifdef RENEWAL
	#define	HOMUN_LEVEL_STATWEIGHT_VALUE 0
	#define APPLY_HOMUN_LEVEL_STATWEIGHT()( \
		hom->str_value = hom->agi_value = \
		hom->vit_value = hom->int_value = \
		hom->dex_value = hom->luk_value = hom->level / 10 - HOMUN_LEVEL_STATWEIGHT_VALUE \
		)
#else
	#define APPLY_HOMUN_LEVEL_STATWEIGHT()
#endif

struct s_h_stats {
	unsigned int HP, SP;
	unsigned short str, agi, vit, int_, dex, luk;
};

struct s_homunculus_db {
	int base_class, evo_class;
	char name[NAME_LENGTH];
	struct s_h_stats base, gmin, gmax, emin, emax;
	int foodID;
	int baseASPD;
	long hungryDelay;
	unsigned char element, race, base_size, evo_size;
};
extern struct s_homunculus_db homunculus_db[MAX_HOMUNCULUS_CLASS];

enum e_hom_search_type : uint8  { HOMUNCULUS_CLASS, HOMUNCULUS_FOOD };

enum e_hom_mode : uint8  { MH_MD_FIGHTING = 1, MH_MD_GRAPPLING };

enum e_hom_state : uint8 {
	HOM_ST_ACTIVE	= 0,
	HOM_ST_REST	= 1,
	HOM_ST_MORPH	= 2,
};

enum e_hom_state2 : uint8 {
	SP_ACK      = 0x0,
	SP_INTIMATE = 0x1,
	SP_HUNGRY   = 0x2,
};

struct s_homun_data {
	struct s_block_list bl;
	struct s_unit_data  ud;
	struct s_view_data *vd;
	struct s_status_data base_status, battle_status;
	struct s_status_change sc;
	struct regen_data regen;
	struct s_homunculus_db *homunculusDB;	//[orn]
	struct s_homunculus homunculus;	//[orn]

	int masterteleport_timer;
	s_map_session_data *master; //pointer back to its master
	int hungry_timer;	//[orn]
	unsigned int exp_next;
	char blockskill[MAX_SKILL];	// [orn]
};

#define MAX_HOM_SKILL_REQUIRE 5
#define MAX_HOM_SKILL_TREE 8

/// Homunculus skill entry [Celest]
struct s_homun_skill_tree_entry {
	uint16 id;			///< Skill ID
	uint8 max;			///< Max level for this tree
	uint8 need_level;	///< Homunculus level required
	uint16 intimacy;	///< Intimacy required (n/100)
	struct {
		uint16 id;		///< Skill ID
		uint8 lv;		///< Level of skill
	} need[MAX_HOM_SKILL_REQUIRE]; ///< Skills needed
};

#define HOM_EVO 0x100 //256
#define HOM_S 0x200 //512
#define HOM_REG 0x1000 //4096

/// Houmunculus ID
enum e_homun_mapid {
// Normal Homunculus
	MAPID_LIF = HOM_REG|0x0,
	MAPID_AMISTR,
	MAPID_FILIR,
	MAPID_VANILMIRTH,
// Evolved Homunulus
	MAPID_LIF_E = HOM_REG|HOM_EVO|0x0,
	MAPID_AMISTR_E,
	MAPID_FILIR_E,
	MAPID_VANILMIRTH_E,
// Homunculus S
	MAPID_EIRA = HOM_S|0x0,
	MAPID_BAYERI,
	MAPID_SERA,
	MAPID_DIETER,
	MAPID_ELANOR,
};

/// Homunculus type
enum e_homun_type : int8 {
	HT_REG		= 0x1,
	HT_EVO		= 0x2,
	HT_S		= 0x4,
	HT_INVALID	= -1,
};

/// Homunculus battle_config setting
enum e_homun_setting : uint8 {
	HOMSET_NO_SUPPORT_SKILL				= 0x01, /// Cannot be targetted by support skills, except for their master
	HOMSET_NO_INSTANT_LAND_SKILL		= 0x02, /// Unit/land skill doesn't applied immediately
	HOMSET_FIRST_TARGET					= 0x04, /// Mobs will always go after them instead of players until attacked
	HOMSET_COPY_SPEED					= 0x08, /// Copy their master's speed on spawn/map-change
	HOMSET_DISPLAY_LUK					= 0x10, /// They display luk/3+1 instead of their actual critical in the stat window, by default they don't crit
	HOMSET_SAME_MATK					= 0x20, /// Their Min-Matk is always the same as their max
	HOMSET_RESET_REUSESKILL_VAPORIZED	= 0x40, /// Skill re-use delay is reset when they are vaporized.
	HOMSET_RESET_REUSESKILL_TELEPORTED	= 0x80, /// Skill re-use delay is reset when they are warped (by skill or item) with player.
};

enum e_homun_grade : uint8 {
	HOMGRADE_HATE_WITH_PASSION = 0,
	HOMGRADE_HATE,
	HOMGRADE_AWKWARD,
	HOMGRADE_SHY,
	HOMGRADE_NEUTRAL,
	HOMGRADE_CORDIAL,
	HOMGRADE_LOYAL,
};

/// Check Homunculus Class ID
#define homdb_checkid(id) ((id) >=  HM_CLASS_BASE && (id) <= HM_CLASS_MAX)

// merc_is_hom_alive(struct s_homun_data *)
#define hom_is_active(x) ((x) && (x)->homunculus.vaporize == HOM_ST_ACTIVE && (x)->battle_status.hp > 0)
int hom_recv_data(uint32 account_id, struct s_homunculus *sh, int flag); //albator
struct s_view_data* hom_get_viewdata(int class_);
int hom_class2mapid(int hom_class);
enum e_homun_type hom_class2type(int class_);
void hom_damage(struct s_homun_data *hd);
int hom_dead(struct s_homun_data *hd);
void hom_skillup(struct s_homun_data *hd,uint16 skill_id);
void hom_calc_skilltree(struct s_homun_data *hd, bool flag_evolve);
short hom_checkskill(struct s_homun_data *hd,uint16 skill_id);
uint8 hom_skill_get_min_level(int class_, uint16 skill_id);
void hom_gainexp(struct s_homun_data *hd,int exp);
int hom_levelup(struct s_homun_data *hd);
int hom_evolution(struct s_homun_data *hd);
int hom_mutate(struct s_homun_data *hd,int homun_id);
void hom_heal(struct s_homun_data *hd);
int hom_vaporize(s_map_session_data *sd, int flag);
int hom_ressurect(s_map_session_data *sd, unsigned char per, short x, short y);
void hom_revive(struct s_homun_data *hd, unsigned int hp, unsigned int sp);
void hom_reset_stats(struct s_homun_data *hd);
int hom_shuffle(struct s_homun_data *hd); // [Zephyrus]
void hom_save(struct s_homun_data *hd);
bool hom_call(s_map_session_data *sd);
bool hom_create_request(s_map_session_data *sd, int class_);
int hom_search(int key,int type);
void hom_menu(s_map_session_data *sd,int type);
int hom_food(s_map_session_data *sd, struct s_homun_data *hd);
int hom_hungry_timer_delete(struct s_homun_data *hd);
int hom_change_name(s_map_session_data *sd,char *name);
void hom_change_name_ack(s_map_session_data *sd, char* name, int flag);
#define hom_stop_walking(hd, type) unit_stop_walking(&(hd)->bl, type)
#define hom_stop_attack(hd) unit_stop_attack(&(hd)->bl)
int hom_increase_intimacy(struct s_homun_data * hd, unsigned int value);
int hom_decrease_intimacy(struct s_homun_data * hd, unsigned int value);
int hom_skill_tree_get_max(int skill_id, int b_class);
void hom_init_timers(struct s_homun_data * hd);
void hom_reload_skill(void);
void hom_reload(void);

void hom_addspiritball(TBL_HOM *hd, int max);
void hom_delspiritball(TBL_HOM *hd, int count, int type);

uint8 hom_get_intimacy_grade(struct s_homun_data *hd);
uint32 hom_intimacy_grade2intimacy(enum e_homun_grade grade);
enum e_homun_grade hom_intimacy_intimacy2grade(uint32 intimacy);

short hom_skill_get_index(uint16 skill_id);

void do_final_homunculus(void);
void do_init_homunculus(void);

#endif /* _HOMUNCULUS_H_ */
