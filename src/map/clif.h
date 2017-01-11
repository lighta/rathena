// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _CLIF_H_
#define _CLIF_H_

#include "../common/cbasetypes.h"
#include "../common/db.h" //dbmap
//#include "../common/mmo.h"

struct s_Channel;
struct s_item;
struct s_storage_data;
struct s_guild_storage;
//#include "map.h"
struct s_block_list;
struct s_unit_data;
struct s_map_session_data;
struct s_homun_data;
struct s_pet_data;
struct s_mob_data;
struct s_npc_data;
struct s_chat_data;
struct s_flooritem_data;
struct s_skill_unit;
struct s_vending;
struct s_party;
struct s_party_data;
struct s_guild;
struct s_battleground_data;
struct s_quest;
struct s_party_booking_ad_info;
enum e_party_member_withdraw : uint8_t;
#include <cstdarg>

enum e_PacketDBVersion { // packet DB
	MIN_PACKET_DB  = 0x064,
	MAX_PACKET_DB  = 0xAFF,
	MAX_PACKET_VER = 55,
	MAX_PACKET_POS = 20,
};

enum e_packet_ack {
	ZC_ACK_OPEN_BANKING = 0,
	ZC_ACK_BANKING_DEPOSIT,
	ZC_ACK_BANKING_WITHDRAW,
	ZC_BANKING_CHECK,
	ZC_PERSONAL_INFOMATION,
	ZC_PERSONAL_INFOMATION_CHN,
	ZC_CLEAR_DIALOG,
	ZC_C_MARKERINFO,
	ZC_NOTIFY_BIND_ON_EQUIP,
	ZC_WEAR_EQUIP_ACK,
	ZC_MERGE_ITEM_OPEN,
	ZC_ACK_MERGE_ITEM,
	ZC_BROADCASTING_SPECIAL_ITEM_OBTAIN,
	//add other here
	MAX_ACK_FUNC //auto upd len
};

struct s_packet_db {
	short len;
	void (*func)(int, struct s_map_session_data *);
	short pos[MAX_PACKET_POS];
};

//#ifdef PACKET_OBFUSCATION
/// Keys based on packet versions
struct s_packet_keys {
	unsigned int keys[3]; ///< 3-Keys
};
//#endif

enum e_CASHSHOP_ACK {
	ERROR_TYPE_NONE             = 0, ///< The deal has successfully completed.
	ERROR_TYPE_NPC              = 1, ///< The Purchase has failed because the NPC does not exist.
	ERROR_TYPE_SYSTEM           = 2, ///< The Purchase has failed because the Kafra Shop System is not working correctly.
	ERROR_TYPE_INVENTORY_WEIGHT = 3, ///< You are over your Weight Limit.
	ERROR_TYPE_EXCHANGE         = 4, ///< You cannot purchase items while you are in a trade.
	ERROR_TYPE_ITEM_ID          = 5, ///< The Purchase has failed because the Item Information was incorrect.
	ERROR_TYPE_MONEY            = 6, ///< You do not have enough Kafra Credit Points.
	ERROR_TYPE_AMOUNT           = 7, ///< You can purchase up to 10 items.
	ERROR_TYPE_PURCHASE_FAIL    = 8, ///< Some items could not be purchased.
};

enum e_BANKING_DEPOSIT_ACK : uint8 {
	BDA_SUCCESS  = 0x0,
	BDA_ERROR    = 0x1,
	BDA_NO_MONEY = 0x2,
	BDA_OVERFLOW = 0x3,
};

enum e_BANKING_WITHDRAW_ACK : uint8 {
	BWA_SUCCESS       = 0x0,
	BWA_NO_MONEY      = 0x1,
	BWA_UNKNOWN_ERROR = 0x2,
};

enum e_RECV_ROULETTE_ITEM_REQ {
	RECV_ITEM_SUCCESS    = 0x0,
	RECV_ITEM_FAILED     = 0x1,
	RECV_ITEM_OVERCOUNT  = 0x2,
	RECV_ITEM_OVERWEIGHT = 0x3,
};

enum e_RECV_ROULETTE_ITEM_ACK {
	RECV_ITEM_NORMAL =  0x0,
	RECV_ITEM_LOSING =  0x1,
};

enum e_GENERATE_ROULETTE_ACK {
	GENERATE_ROULETTE_SUCCESS         = 0x0,
	GENERATE_ROULETTE_FAILED          = 0x1,
	GENERATE_ROULETTE_NO_ENOUGH_POINT = 0x2,
	GENERATE_ROULETTE_LOSING          = 0x3,
};

enum e_OPEN_ROULETTE_ACK {
	OPEN_ROULETTE_SUCCESS = 0x0,
	OPEN_ROULETTE_FAILED  = 0x1,
};

enum e_CLOSE_ROULETTE_ACK {
	CLOSE_ROULETTE_SUCCESS = 0x0,
	CLOSE_ROULETTE_FAILED  = 0x1,
};

enum e_MERGE_ITEM_ACK {
	MERGE_ITEM_SUCCESS = 0x0,
	MERGE_ITEM_FAILED_NOT_MERGE = 0x1,
	MERGE_ITEM_FAILED_MAX_COUNT = 0x2,
};

enum e_BROADCASTING_SPECIAL_ITEM_OBTAIN {
	ITEMOBTAIN_TYPE_BOXITEM =  0x0,
	ITEMOBTAIN_TYPE_MONSTER_ITEM =  0x1,
	ITEMOBTAIN_TYPE_NPC =  0x2,
};

enum e_adopt_reply {
	ADOPT_REPLY_MORE_CHILDREN = 0,
	ADOPT_REPLY_LEVEL_70,
	ADOPT_REPLY_MARRIED,
};

enum e_wip_block : uint8_t {
	WIP_DISABLE_NONE = 0x0,
	WIP_DISABLE_SKILLITEM = 0x1,
	WIP_DISABLE_NPC = 0x2,
	WIP_DISABLE_ALL = 0x3,
};

enum e_party_invite_reply {
	PARTY_REPLY_JOIN_OTHER_PARTY = 0,   ///< result=0 : "The Character already joined another party." -> MsgStringTable[80]
	PARTY_REPLY_REJECTED,			    ///< result=1 : "Request for party rejected." -> MsgStringTable[81]
	PARTY_REPLY_ACCEPTED,			    ///< result=2 : "Request for party accepted." -> MsgStringTable[82]
	PARTY_REPLY_FULL,				    ///< result=3 : "Party Capacity exceeded." -> MsgStringTable[83]
	PARTY_REPLY_DUAL,				    ///< result=4 : "Character in the same account already joined." -> MsgStringTable[608]
	PARTY_REPLY_JOINMSG_REFUSE,		    ///< result=5 : !TODO "The character blocked the party invitation." -> MsgStringTable[1324] (since 20070904)
	PARTY_REPLY_UNKNOWN_ERROR,		    ///< result=6 : ??
	PARTY_REPLY_OFFLINE,			    ///< result=7 : "The Character is not currently online or does not exist." -> MsgStringTable[71] (since 20070904)
	PARTY_REPLY_INVALID_MAPPROPERTY,    ///< result=8 : !TODO "Unable to organize a party in this map" -> MsgStringTable[1388] (since 20080527)
	PARTY_REPLY_INVALID_MAPPROPERTY_ME, ///< return=9 : !TODO "Cannot join a party in this map" -> MsgStringTable[1871] (since 20110205)
};

// packet_db[SERVER] is reserved for server use
#define SERVER 0
#define packet_len(cmd) packet_db[SERVER][cmd].len
extern struct s_packet_db packet_db[MAX_PACKET_VER+1][MAX_PACKET_DB+1];
extern int packet_db_ack[MAX_PACKET_VER + 1][MAX_ACK_FUNC + 1];

// local define
enum e_send_target : uint8 {
	ALL_CLIENT = 0,
	ALL_SAMEMAP,
	AREA,				// area
	AREA_WOS,			// area, without self
	AREA_WOC,			// area, without chatrooms
	AREA_WOSC,			// area, without own chatroom
	AREA_CHAT_WOC,		// hearable area, without chatrooms
	CHAT,				// current chatroom
	CHAT_WOS,			// current chatroom, without self
	PARTY,
	PARTY_WOS,
	PARTY_SAMEMAP,
	PARTY_SAMEMAP_WOS,
	PARTY_AREA,
	PARTY_AREA_WOS,
	GUILD,
	GUILD_WOS,
	GUILD_SAMEMAP,
	GUILD_SAMEMAP_WOS,
	GUILD_AREA,
	GUILD_AREA_WOS,
	GUILD_NOBG,
	DUEL,
	DUEL_WOS,
	SELF,

	BG,					// BattleGround System
	BG_WOS,
	BG_SAMEMAP,
	BG_SAMEMAP_WOS,
	BG_AREA,
	BG_AREA_WOS,
};

enum e_broadcast_flags : uint8{
	BC_ALL			= 0,
	BC_MAP			= 1,
	BC_AREA			= 2,
	BC_SELF			= 3,
	BC_TARGET_MASK	= 0x07,

	BC_PC			= 0x00,
	BC_NPC			= 0x08,
	BC_SOURCE_MASK	= 0x08, // BC_PC|BC_NPC

	BC_YELLOW		= 0x00,
	BC_BLUE			= 0x10,
	BC_WOE			= 0x20,
	BC_COLOR_MASK	= 0x30, // BC_YELLOW|BC_BLUE|BC_WOE

	BC_DEFAULT		= BC_ALL|BC_PC|BC_YELLOW
};

enum e_emotion_type {
	E_GASP = 0,     // /!
	E_WHAT,         // /?
	E_HO,
	E_LV,
	E_SWT,
	E_IC,
	E_AN,
	E_AG,
	E_CASH,         // /$
	E_DOTS,         // /...
	E_SCISSORS,     // /gawi --- 10
	E_ROCK,         // /bawi
	E_PAPER,        // /bo
	E_KOREA,
	E_LV2,
	E_THX,
	E_WAH,
	E_SRY,
	E_HEH,
	E_SWT2,
	E_HMM,          // --- 20
	E_NO1,
	E_NO,           // /??
	E_OMG,
	E_OH,
	E_X,
	E_HLP,
	E_GO,
	E_SOB,
	E_GG,
	E_KIS,          // --- 30
	E_KIS2,
	E_PIF,
	E_OK,
	E_MUTE,         // red /... used for muted characters
	E_INDONESIA,
	E_BZZ,          // /bzz, /stare
	E_RICE,
	E_AWSM,         // /awsm, /cool
	E_MEH,
	E_SHY,          // --- 40
	E_PAT,          // /pat, /goodboy
	E_MP,           // /mp, /sptime
	E_SLUR,
	E_COM,          // /com, /comeon
	E_YAWN,         // /yawn, /sleepy
	E_GRAT,         // /grat, /congrats
	E_HP,           // /hp, /hptime
	E_PHILIPPINES,
	E_MALAYSIA,
	E_SINGAPORE,    // --- 50
	E_BRAZIL,
	E_FLASH,        // /fsh
	E_SPIN,         // /spin
	E_SIGH,
	E_DUM,          // /dum
	E_LOUD,         // /crwd
	E_OTL,          // /otl, /desp
	E_DICE1,
	E_DICE2,
	E_DICE3,        // --- 60
	E_DICE4,
	E_DICE5,
	E_DICE6,
	E_INDIA,
	E_LUV,          // /love
	E_RUSSIA,
	E_VIRGIN,
	E_MOBILE,
	E_MAIL,
	E_CHINESE,      // --- 70
	E_ANTENNA1,
	E_ANTENNA2,
	E_ANTENNA3,
	E_HUM,
	E_ABS,
	E_OOPS,
	E_SPIT,
	E_ENE,
	E_PANIC,
	E_WHISP,        // --- 80
	E_YUT1,
	E_YUT2,
	E_YUT3,
	E_YUT4,
	E_YUT5,
	E_YUT6,
	E_YUT7,
	//
	E_MAX
};

enum e_clr_type : uint8
{
	CLR_OUTSIGHT = 0,
	CLR_DEAD,
	CLR_RESPAWN,
	CLR_TELEPORT,
	CLR_TRICKDEAD,
};

enum e_map_property  : uint8
{// clif_map_property
	MAPPROPERTY_NOTHING       = 0,
	MAPPROPERTY_FREEPVPZONE   = 1,
	MAPPROPERTY_EVENTPVPZONE  = 2,
	MAPPROPERTY_AGITZONE      = 3,
	MAPPROPERTY_PKSERVERZONE  = 4, // message "You are in a PK area. Please beware of sudden attacks." in color 0x9B9BFF (light red)
	MAPPROPERTY_PVPSERVERZONE = 5,
	MAPPROPERTY_DENYSKILLZONE = 6,
};

enum e_map_type  : uint8
{// clif_map_type
	MAPTYPE_VILLAGE              = 0,
	MAPTYPE_VILLAGE_IN           = 1,
	MAPTYPE_FIELD                = 2,
	MAPTYPE_DUNGEON              = 3,
	MAPTYPE_ARENA                = 4,
	MAPTYPE_PENALTY_FREEPKZONE   = 5,
	MAPTYPE_NOPENALTY_FREEPKZONE = 6,
	MAPTYPE_EVENT_GUILDWAR       = 7,
	MAPTYPE_AGIT                 = 8,
	MAPTYPE_DUNGEON2             = 9,
	MAPTYPE_DUNGEON3             = 10,
	MAPTYPE_PKSERVER             = 11,
	MAPTYPE_PVPSERVER            = 12,
	MAPTYPE_DENYSKILL            = 13,
	MAPTYPE_TURBOTRACK           = 14,
	MAPTYPE_JAIL                 = 15,
	MAPTYPE_MONSTERTRACK         = 16,
	MAPTYPE_PORINGBATTLE         = 17,
	MAPTYPE_AGIT_SIEGEV15        = 18,
	MAPTYPE_BATTLEFIELD          = 19,
	MAPTYPE_PVP_TOURNAMENT       = 20,
	//Map types 21 - 24 not used.
	MAPTYPE_SIEGE_LOWLEVEL       = 25,
	//Map types 26 - 28 remains opens for future types.
	MAPTYPE_UNUSED               = 29,
};

enum e_useskill_fail_cause  : uint8
{// clif_skill_fail
	USESKILL_FAIL_LEVEL = 0,
	USESKILL_FAIL_SP_INSUFFICIENT = 1,
	USESKILL_FAIL_HP_INSUFFICIENT = 2,
	USESKILL_FAIL_STUFF_INSUFFICIENT = 3,
	USESKILL_FAIL_SKILLINTERVAL = 4,
	USESKILL_FAIL_MONEY = 5,
	USESKILL_FAIL_THIS_WEAPON = 6,
	USESKILL_FAIL_REDJAMSTONE = 7,
	USESKILL_FAIL_BLUEJAMSTONE = 8,
	USESKILL_FAIL_WEIGHTOVER = 9,
	USESKILL_FAIL = 10,
	USESKILL_FAIL_TOTARGET = 11,
	USESKILL_FAIL_ANCILLA_NUMOVER = 12,
	USESKILL_FAIL_HOLYWATER = 13,
	USESKILL_FAIL_ANCILLA = 14,
	USESKILL_FAIL_DUPLICATE_RANGEIN = 15,
	USESKILL_FAIL_NEED_OTHER_SKILL = 16,
	USESKILL_FAIL_NEED_HELPER = 17,
	USESKILL_FAIL_INVALID_DIR = 18,
	USESKILL_FAIL_SUMMON = 19,
	USESKILL_FAIL_SUMMON_NONE = 20,
	USESKILL_FAIL_IMITATION_SKILL_NONE = 21,
	USESKILL_FAIL_DUPLICATE = 22,
	USESKILL_FAIL_CONDITION = 23,
	USESKILL_FAIL_PAINTBRUSH = 24,
	USESKILL_FAIL_DRAGON = 25,
	USESKILL_FAIL_POS = 26,
	USESKILL_FAIL_HELPER_SP_INSUFFICIENT = 27,
	USESKILL_FAIL_NEER_WALL = 28,
	USESKILL_FAIL_NEED_EXP_1PERCENT = 29,
	USESKILL_FAIL_CHORUS_SP_INSUFFICIENT = 30,
	USESKILL_FAIL_GC_WEAPONBLOCKING = 31,
	USESKILL_FAIL_GC_POISONINGWEAPON = 32,
	USESKILL_FAIL_MADOGEAR = 33,
	USESKILL_FAIL_NEED_EQUIPMENT_KUNAI = 34,
	USESKILL_FAIL_TOTARGET_PLAYER = 35,
	USESKILL_FAIL_SIZE = 36,
	USESKILL_FAIL_CANONBALL = 37,
	//XXX_USESKILL_FAIL_II_MADOGEAR_ACCELERATION = 38,
	//XXX_USESKILL_FAIL_II_MADOGEAR_HOVERING_BOOSTER = 39,
	USESKILL_FAIL_MADOGEAR_HOVERING = 40,
	//XXX_USESKILL_FAIL_II_MADOGEAR_SELFDESTRUCTION_DEVICE = 41,
	//XXX_USESKILL_FAIL_II_MADOGEAR_SHAPESHIFTER = 42,
	USESKILL_FAIL_GUILLONTINE_POISON = 43,
	//XXX_USESKILL_FAIL_II_MADOGEAR_COOLING_DEVICE = 44,
	//XXX_USESKILL_FAIL_II_MADOGEAR_MAGNETICFIELD_GENERATOR = 45,
	//XXX_USESKILL_FAIL_II_MADOGEAR_BARRIER_GENERATOR = 46,
	//XXX_USESKILL_FAIL_II_MADOGEAR_OPTICALCAMOUFLAGE_GENERATOR = 47,
	//XXX_USESKILL_FAIL_II_MADOGEAR_REPAIRKIT = 48,
	//XXX_USESKILL_FAIL_II_MONKEY_SPANNER = 49,
	USESKILL_FAIL_MADOGEAR_RIDE = 50,
	USESKILL_FAIL_SPELLBOOK = 51,
	USESKILL_FAIL_SPELLBOOK_DIFFICULT_SLEEP = 52,
	USESKILL_FAIL_SPELLBOOK_PRESERVATION_POINT = 53,
	USESKILL_FAIL_SPELLBOOK_READING = 54,
	//XXX_USESKILL_FAIL_II_FACE_PAINTS = 55,
	//XXX_USESKILL_FAIL_II_MAKEUP_BRUSH = 56,
	USESKILL_FAIL_CART = 57,
	//XXX_USESKILL_FAIL_II_THORNS_SEED = 58,
	//XXX_USESKILL_FAIL_II_BLOOD_SUCKER_SEED = 59,
	USESKILL_FAIL_NO_MORE_SPELL = 60,
	//XXX_USESKILL_FAIL_II_BOMB_MUSHROOM_SPORE = 61,
	//XXX_USESKILL_FAIL_II_GASOLINE_BOOMB = 62,
	//XXX_USESKILL_FAIL_II_OIL_BOTTLE = 63,
	//XXX_USESKILL_FAIL_II_EXPLOSION_POWDER = 64,
	//XXX_USESKILL_FAIL_II_SMOKE_POWDER = 65,
	//XXX_USESKILL_FAIL_II_TEAR_GAS = 66,
	//XXX_USESKILL_FAIL_II_HYDROCHLORIC_ACID_BOTTLE = 67,
	//XXX_USESKILL_FAIL_II_HELLS_PLANT_BOTTLE = 68,
	//XXX_USESKILL_FAIL_II_MANDRAGORA_FLOWERPOT = 69,
	USESKILL_FAIL_MANUAL_NOTIFY = 70,
	USESKILL_FAIL_NEED_ITEM = 71,
	USESKILL_FAIL_NEED_EQUIPMENT = 72,
	USESKILL_FAIL_COMBOSKILL = 73,
	USESKILL_FAIL_SPIRITS = 74,
	USESKILL_FAIL_EXPLOSIONSPIRITS = 75,
	USESKILL_FAIL_HP_TOOMANY = 76,
	USESKILL_FAIL_NEED_ROYAL_GUARD_BANDING = 77,
	USESKILL_FAIL_NEED_EQUIPPED_WEAPON_CLASS = 78,
	USESKILL_FAIL_EL_SUMMON = 79,
	USESKILL_FAIL_RELATIONGRADE = 80,
	USESKILL_FAIL_STYLE_CHANGE_FIGHTER = 81,
	USESKILL_FAIL_STYLE_CHANGE_GRAPPLER = 82,
	USESKILL_FAIL_THERE_ARE_NPC_AROUND = 83,
	USESKILL_FAIL_NEED_MORE_BULLET = 84,

	USESKILL_FAIL_MAX
};

enum e_clif_messages  : uint16 {
	/* Constant values */
	// clif_cart_additem_ack flags
	ADDITEM_TO_CART_FAIL_WEIGHT = 0x0,
	ADDITEM_TO_CART_FAIL_COUNT = 0x1,

	// clif_equipitemack flags
	ITEM_EQUIP_ACK_OK = 0,
	ITEM_EQUIP_ACK_FAIL = 1,
	ITEM_EQUIP_ACK_FAILLEVEL = 2,
	/* -end- */

	//! NOTE: These values below need client version validation
	ITEM_CANT_OBTAIN_WEIGHT = 0x34, /* You cannot carry more items because you are overweight. */
	ITEM_NOUSE_SITTING = 0x297,
	MERC_MSG_BASE = 0x4f2,
	SKILL_CANT_USE_AREA = 0x536,
	ITEM_CANT_USE_AREA =  0x537,
	VIEW_EQUIP_FAIL = 0x54d,
	RUNE_CANT_CREATE = 0x61b,
	ITEM_CANT_COMBINE = 0x623,
	INVENTORY_SPACE_FULL = 0x625,
	ITEM_PRODUCE_SUCCESS = 0x627,
	ITEM_PRODUCE_FAIL = 0x628,
	ITEM_UNIDENTIFIED = 0x62d,
	ITEM_REUSE_LIMIT = 0x746,
	WORK_IN_PROGRESS = 0x783,
	NEED_REINS_OF_MOUNT = 0x78c,
	MERGE_ITEM_NOT_AVAILABLE = 0x887,
};

enum e_personalinfo  : uint8 {
	PINFO_BASIC = 0,
	PINFO_PREMIUM,
	PINFO_SERVER,
	PINFO_CAFE,
	PINFO_MAX,
};

enum e_damage_type : uint8 {
	DMG_NORMAL = 0,			/// damage [ damage: total damage, div: amount of hits, damage2: assassin dual-wield damage ]
	DMG_PICKUP_ITEM,		/// pick up item
	DMG_SIT_DOWN,			/// sit down
	DMG_STAND_UP,			/// stand up
	DMG_ENDURE,				/// damage (endure)
	DMG_SPLASH,				/// (splash?)
	DMG_SKILL,				/// (skill?)
	DMG_REPEAT,				/// (repeat damage?)
	DMG_MULTI_HIT,			/// multi-hit damage
	DMG_MULTI_HIT_ENDURE,	/// multi-hit damage (endure)
	DMG_CRITICAL,			/// critical hit
	DMG_LUCY_DODGE,			/// lucky dodge
	DMG_TOUCH,				/// (touch skill?)
};

int clif_setip(const char* ip);
void clif_setbindip(const char* ip);
void clif_setport(uint16 port);

uint32 clif_getip(void);
uint32 clif_refresh_ip(void);
uint16 clif_getport(void);
void packetdb_readdb(bool reload);

void clif_authok(struct s_map_session_data *sd);
void clif_authrefuse(int fd, uint8 error_code);
void clif_authfail_fd(int fd, int type);
void clif_charselectok(int id, uint8 ok);
void clif_dropflooritem(struct s_flooritem_data* fitem);
void clif_clearflooritem(struct s_flooritem_data *fitem, int fd);

void clif_clearunit_single(int id,enum e_clr_type type, int fd);
void clif_clearunit_area(struct s_block_list* bl,enum e_clr_type type);
void clif_clearunit_delayed(struct s_block_list* bl,enum e_clr_type type, unsigned int tick);
int clif_spawn(struct s_block_list *bl);	//area
void clif_walkok(struct s_map_session_data *sd);	// self
void clif_move(struct s_unit_data *ud); //area
void clif_changemap(struct s_map_session_data *sd, short m, int x, int y);	//self
void clif_changemapserver(struct s_map_session_data* sd, unsigned short map_index, int x, int y, uint32 ip, uint16 port);	//self
void clif_blown(struct s_block_list *bl); // area
void clif_slide(struct s_block_list *bl, int x, int y); // area
void clif_fixpos(struct s_block_list *bl);	// area
void clif_npcbuysell(struct s_map_session_data* sd, int id);	//self
void clif_buylist(struct s_map_session_data *sd, struct s_npc_data *nd);	//self
void clif_selllist(struct s_map_session_data *sd);	//self
void clif_npc_market_open(struct s_map_session_data *sd, struct s_npc_data *nd);
void clif_parse_NPCMarketClosed(int fd, struct s_map_session_data *sd);
void clif_parse_NPCMarketPurchase(int fd, struct s_map_session_data *sd);
void clif_scriptmes(struct s_map_session_data *sd, int npcid, const char *mes);	//self
void clif_scriptnext(struct s_map_session_data *sd,int npcid);	//self
void clif_scriptclose(struct s_map_session_data *sd, int npcid);	//self
void clif_scriptclear(struct s_map_session_data *sd, int npcid);	//self
void clif_scriptmenu(struct s_map_session_data* sd, int npcid, const char* mes);	//self
void clif_scriptinput(struct s_map_session_data *sd, int npcid);	//self
void clif_scriptinputstr(struct s_map_session_data *sd, int npcid);	// self
void clif_cutin(struct s_map_session_data* sd, const char* image, int type);	//self
void clif_viewpoint(struct s_map_session_data *sd, int npc_id, int type, int x, int y, int id, int color);	//self
void clif_additem(struct s_map_session_data *sd, int n, int amount, unsigned char fail); // self
void clif_dropitem(struct s_map_session_data *sd,int n,int amount);	//self
void clif_delitem(struct s_map_session_data *sd,int n,int amount, short reason); //self
void clif_updatestatus(struct s_map_session_data *sd,int type);	//self
void clif_changestatus(struct s_map_session_data* sd,int type,int val);	//area
int clif_damage(struct s_block_list* src, struct s_block_list* dst, unsigned int tick, int sdelay, int ddelay, int64 sdamage, int div, enum e_damage_type type, int64 sdamage2, bool spdamage);	// area
void clif_takeitem(struct s_block_list* src, struct s_block_list* dst);
void clif_sitting(struct s_block_list* bl);
void clif_standing(struct s_block_list* bl);
void clif_sprite_change(struct s_block_list *bl, int id, int type, int val, int val2, enum e_send_target target);
void clif_changelook(struct s_block_list *bl,int type,int val);	// area
void clif_changetraplook(struct s_block_list *bl,int val); // area
void clif_refreshlook(struct s_block_list *bl,int id,int type,int val,enum e_send_target target); //area specified in 'target'
void clif_arrowequip(struct s_map_session_data *sd,int val); //self
void clif_arrow_fail(struct s_map_session_data *sd,int type); //self
void clif_arrow_create_list(struct s_map_session_data *sd);	//self
void clif_statusupack(struct s_map_session_data *sd,int type,int ok,int val);	// self
void clif_equipitemack(struct s_map_session_data *sd,int n,int pos,uint8 flag);	// self
void clif_unequipitemack(struct s_map_session_data *sd,int n,int pos,int ok);	// self
void clif_misceffect(struct s_block_list* bl,int type);	// area
void clif_changeoption(struct s_block_list* bl);	// area
void clif_changeoption2(struct s_block_list* bl);	// area
void clif_useitemack(struct s_map_session_data *sd,int index,int amount,bool ok);	// self
void clif_GlobalMessage(struct s_block_list* bl, const char* message,enum e_send_target target);
void clif_createchat(struct s_map_session_data* sd, int flag);	// self
void clif_dispchat(struct s_chat_data* cd, int fd);	// area or fd
void clif_joinchatfail(struct s_map_session_data *sd,int flag);	// self
void clif_joinchatok(struct s_map_session_data *sd,struct s_chat_data* cd);	// self
void clif_addchat(struct s_chat_data* cd,struct s_map_session_data *sd);	// chat
void clif_changechatowner(struct s_chat_data* cd, struct s_map_session_data* sd);	// chat
void clif_clearchat(struct s_chat_data *cd,int fd);	// area or fd
void clif_leavechat(struct s_chat_data* cd, struct s_map_session_data* sd, bool flag);	// chat
void clif_changechatstatus(struct s_chat_data* cd);	// chat
void clif_refresh_storagewindow(struct s_map_session_data *sd);
void clif_refresh(struct s_map_session_data *sd);	// self

void clif_fame_blacksmith(struct s_map_session_data *sd, int points);
void clif_fame_alchemist(struct s_map_session_data *sd, int points);
void clif_fame_taekwon(struct s_map_session_data *sd, int points);

void clif_emotion(struct s_block_list *bl,int type);
void clif_talkiebox(struct s_block_list* bl, const char* talkie);
void clif_wedding_effect(struct s_block_list *bl);
void clif_divorced(struct s_map_session_data* sd, const char* name);
void clif_callpartner(struct s_map_session_data *sd);
void clif_playBGM(struct s_map_session_data* sd, const char* name);
void clif_soundeffect(struct s_map_session_data* sd, struct s_block_list* bl, const char* name, int type);
void clif_soundeffectall(struct s_block_list* bl, const char* name, int type, enum e_send_target coverage);
void clif_parse_ActionRequest_sub(struct s_map_session_data *sd, int action_type, int target_id, unsigned int tick);
void clif_parse_LoadEndAck(int fd,struct s_map_session_data *sd);
void clif_hotkeys_send(struct s_map_session_data *sd);

// trade
void clif_traderequest(struct s_map_session_data* sd, const char* name);
void clif_tradestart(struct s_map_session_data* sd, uint8 type);
void clif_tradeadditem(struct s_map_session_data* sd, struct s_map_session_data* tsd, int index, int amount);
void clif_tradeitemok(struct s_map_session_data* sd, int index, int fail);
void clif_tradedeal_lock(struct s_map_session_data* sd, int fail);
void clif_tradecancelled(struct s_map_session_data* sd);
void clif_tradecompleted(struct s_map_session_data* sd, int fail);
void clif_tradeundo(struct s_map_session_data* sd);

// storage
void clif_storagelist(struct s_map_session_data* sd, struct s_item* items, int items_length);
void clif_updatestorageamount(struct s_map_session_data* sd, int amount, int max_amount);
void clif_storageitemadded(struct s_map_session_data* sd, struct s_item* i, int index, int amount);
void clif_storageitemremoved(struct s_map_session_data* sd, int index, int amount);
void clif_storageclose(struct s_map_session_data* sd);

int clif_insight(struct s_block_list *bl,va_list ap);	// map_forallinmovearea callback
int clif_outsight(struct s_block_list *bl,va_list ap);	// map_forallinmovearea callback

void clif_class_change(struct s_block_list *bl,int class_,int type);
#define clif_mob_class_change(md, class_) clif_class_change(&md->bl, class_, 1)

void clif_skillinfoblock(struct s_map_session_data *sd);
void clif_skillup(struct s_map_session_data *sd, uint16 skill_id, int lv, int range, int upgradable);
void clif_skillinfo(struct s_map_session_data *sd,int skill_id, int inf);
void clif_addskill(struct s_map_session_data *sd, int skill_id);
void clif_deleteskill(struct s_map_session_data *sd, int skill_id);

void clif_skillcasting(struct s_block_list* bl, int src_id, int dst_id, int dst_x, int dst_y, uint16 skill_id, int property, int casttime);
void clif_skillcastcancel(struct s_block_list* bl);
void clif_skill_fail(struct s_map_session_data *sd,uint16 skill_id,enum e_useskill_fail_cause cause,int btype);
void clif_skill_cooldown(struct s_map_session_data *sd, uint16 skill_id, unsigned int tick);
int clif_skill_damage(struct s_block_list *src,struct s_block_list *dst,unsigned int tick,int sdelay,int ddelay,int64 sdamage,int div,uint16 skill_id,uint16 skill_lv,enum e_damage_type type);
//int clif_skill_damage2(struct s_block_list *src,struct s_block_list *dst,unsigned int tick,int sdelay,int ddelay,int damage,int div,uint16 skill_id,uint16 skill_lv,enum e_damage_type type);
int clif_skill_nodamage(struct s_block_list *src,struct s_block_list *dst,uint16 skill_id,int heal,int fail);
void clif_skill_poseffect(struct s_block_list *src,uint16 skill_id,int val,int x,int y,int tick);
void clif_skill_estimation(struct s_map_session_data *sd,struct s_block_list *dst);
void clif_skill_warppoint(struct s_map_session_data* sd, uint16 skill_id, uint16 skill_lv, unsigned short map1, unsigned short map2, unsigned short map3, unsigned short map4);
void clif_skill_memomessage(struct s_map_session_data* sd, int type);
void clif_skill_teleportmessage(struct s_map_session_data *sd, int type);
void clif_skill_produce_mix_list(struct s_map_session_data *sd, int skill_id, int trigger);
void clif_cooking_list(struct s_map_session_data *sd, int trigger, uint16 skill_id, int qty, int list_type);

void clif_produceeffect(struct s_map_session_data* sd,int flag, unsigned short nameid);

void clif_getareachar_skillunit(struct s_block_list *bl, struct s_skill_unit *unit, enum e_send_target target, bool visible);
void clif_skill_delunit(struct s_skill_unit *unit);
void clif_skillunit_update(struct s_block_list* bl);

void clif_autospell(struct s_map_session_data *sd,uint16 skill_lv);
void clif_devotion(struct s_block_list *src, struct s_map_session_data *tsd);
void clif_spiritball(struct s_block_list *bl);
void clif_combo_delay(struct s_block_list *bl,int wait);
void clif_bladestop(struct s_block_list *src, int dst_id, int active);
void clif_changemapcell(int fd, int16 m, int x, int y, int type, enum e_send_target target);

#define clif_status_load(bl, type, flag) clif_status_change((bl), (type), (flag), 0, 0, 0, 0)
void clif_status_change(struct s_block_list *bl, int type, int flag, int tick, int val1, int val2, int val3);
void clif_efst_status_change(struct s_block_list *bl, int tid, enum e_send_target target, int type, int tick, int val1, int val2, int val3);
void clif_efst_status_change_sub(struct s_map_session_data *sd, struct s_block_list *bl, enum e_send_target target);

void clif_wis_message(int fd, const char* nick, const char* mes, int mes_len);
void clif_wis_end(int fd, int result);

void clif_solved_charname(int fd, int charid, const char* name);
void clif_name( struct s_block_list* src, struct s_block_list *bl, e_send_target target );
#define clif_name_self(bl) clif_name( (bl), (bl), SELF )
#define clif_name_area(bl) clif_name( (bl), (bl), AREA )

void clif_use_card(struct s_map_session_data *sd,int idx);
void clif_insert_card(struct s_map_session_data *sd,int idx_equip,int idx_card,int flag);

void clif_inventorylist(struct s_map_session_data *sd);
void clif_equiplist(struct s_map_session_data *sd);

void clif_cart_additem(struct s_map_session_data *sd,int n,int amount,int fail);
void clif_cart_additem_ack(struct s_map_session_data *sd, uint8 flag);
void clif_cart_delitem(struct s_map_session_data *sd,int n,int amount);
void clif_cartlist(struct s_map_session_data *sd);
void clif_clearcart(int fd);

void clif_item_identify_list(struct s_map_session_data *sd);
void clif_item_identified(struct s_map_session_data *sd,int idx,int flag);
void clif_item_repair_list(struct s_map_session_data *sd, struct s_map_session_data *dstsd, int lv);
void clif_item_repaireffect(struct s_map_session_data *sd, int idx, int flag);
void clif_item_damaged(struct s_map_session_data* sd, unsigned short position);
void clif_item_refine_list(struct s_map_session_data *sd);
void clif_hat_effects( struct s_map_session_data* sd, struct s_block_list* bl, enum e_send_target target );
void clif_hat_effect_single( struct s_map_session_data* sd, uint16 effectId, bool enable );

void clif_item_skill(struct s_map_session_data *sd,uint16 skill_id,uint16 skill_lv);

void clif_mvp_effect(struct s_map_session_data *sd);
void clif_mvp_item(struct s_map_session_data *sd, unsigned short nameid);
void clif_mvp_exp(struct s_map_session_data *sd, unsigned int exp);
void clif_mvp_noitem(struct s_map_session_data* sd);
void clif_changed_dir(struct s_block_list *bl, enum e_send_target target);

// vending
void clif_openvendingreq(struct s_map_session_data* sd, int num);
void clif_showvendingboard(struct s_block_list* bl, const char* message, int fd);
void clif_closevendingboard(struct s_block_list* bl, int fd);
void clif_vendinglist(struct s_map_session_data* sd, int id, struct s_vending* vending);
void clif_buyvending(struct s_map_session_data* sd, int index, int amount, int fail);
void clif_openvending(struct s_map_session_data* sd, int id, struct s_vending* vending);
void clif_vendingreport(struct s_map_session_data* sd, int index, int amount, uint32 char_id, int zeny);

void clif_movetoattack(struct s_map_session_data *sd,struct s_block_list *bl);

// party
void clif_party_created(struct s_map_session_data *sd,int result);
void clif_party_member_info(struct s_party_data *p, struct s_map_session_data *sd);
void clif_party_info(struct s_party_data* p, struct s_map_session_data *sd);
void clif_party_invite(struct s_map_session_data *sd,struct s_map_session_data *tsd);
void clif_party_invite_reply(struct s_map_session_data* sd, const char* nick, enum e_party_invite_reply reply);
void clif_party_option(struct s_party_data *p,struct s_map_session_data *sd,int flag);
void clif_party_withdraw(struct s_map_session_data *sd, uint32 account_id, const char* name, enum e_party_member_withdraw result, enum e_send_target target);
void clif_party_message(struct s_party_data* p, uint32 account_id, const char* mes, int len);
void clif_party_xy(struct s_map_session_data *sd);
void clif_party_xy_single(int fd, struct s_map_session_data *sd);
void clif_party_hp(struct s_map_session_data *sd);
void clif_hpmeter_single(int fd, int id, unsigned int hp, unsigned int maxhp);

// guild
void clif_guild_created(struct s_map_session_data *sd,int flag);
void clif_guild_belonginfo(struct s_map_session_data *sd);
void clif_guild_masterormember(struct s_map_session_data *sd);
void clif_guild_basicinfo(struct s_map_session_data *sd);
void clif_guild_allianceinfo(struct s_map_session_data *sd);
void clif_guild_memberlist(struct s_map_session_data *sd);
void clif_guild_skillinfo(struct s_map_session_data* sd);
void clif_guild_send_onlineinfo(struct s_map_session_data *sd); //[LuzZza]
void clif_guild_memberlogin_notice(struct s_guild *g,int idx,int flag);
void clif_guild_invite(struct s_map_session_data *sd,struct s_guild *g);
void clif_guild_inviteack(struct s_map_session_data *sd,int flag);
void clif_guild_leave(struct s_map_session_data *sd,const char *name,const char *mes);
void clif_guild_expulsion(struct s_map_session_data* sd, const char* name, const char* mes, uint32 account_id);
void clif_guild_positionchanged(struct s_guild *g,int idx);
void clif_guild_memberpositionchanged(struct s_guild *g,int idx);
void clif_guild_emblem(struct s_map_session_data *sd,struct s_guild *g);
void clif_guild_emblem_area(struct s_block_list* bl);
void clif_guild_notice(struct s_map_session_data* sd);
void clif_guild_message(struct s_guild *g,uint32 account_id,const char *mes,int len);
void clif_guild_reqalliance(struct s_map_session_data *sd,uint32 account_id,const char *name);
void clif_guild_allianceack(struct s_map_session_data *sd,int flag);
void clif_guild_delalliance(struct s_map_session_data *sd,int guild_id,int flag);
void clif_guild_oppositionack(struct s_map_session_data *sd,int flag);
void clif_guild_broken(struct s_map_session_data *sd,int flag);
void clif_guild_xy(struct s_map_session_data *sd);
void clif_guild_xy_single(int fd, struct s_map_session_data *sd);
void clif_guild_xy_remove(struct s_map_session_data *sd);

// Battleground
void clif_bg_hp(struct s_map_session_data *sd);
void clif_bg_xy(struct s_map_session_data *sd);
void clif_bg_xy_remove(struct s_map_session_data *sd);
void clif_bg_message(struct s_battleground_data *bg, int src_id, const char *name, const char *mes, int len);
void clif_bg_updatescore(int16 m);
void clif_bg_updatescore_single(struct s_map_session_data *sd);
void clif_sendbgemblem_area(struct s_map_session_data *sd);
void clif_sendbgemblem_single(int fd, struct s_map_session_data *sd);

// Instancing
void clif_instance_create(unsigned short instance_id, int num);
void clif_instance_changewait(unsigned short instance_id, int num);
void clif_instance_status(unsigned short instance_id, unsigned int limit1, unsigned int limit2);
void clif_instance_changestatus(unsigned int instance_id, int type, unsigned int limit);

// Custom Fonts
void clif_font(struct s_map_session_data *sd);

// atcommand
void clif_displaymessage(const int fd, const char* mes);
void clif_disp_onlyself(struct s_map_session_data *sd, const char *mes, int len);
void clif_disp_message(struct s_block_list* src, const char* mes, int len, enum e_send_target target);
void clif_broadcast(struct s_block_list* bl, const char* mes, int len, int type, enum e_send_target target);
void clif_broadcast2(struct s_block_list* bl, const char* mes, int len, unsigned long fontColor, short fontType, short fontSize, short fontAlign, short fontY, enum e_send_target target);
void clif_heal(int fd,int type,int val);
void clif_resurrection(struct s_block_list *bl,int type);
void clif_map_property(struct s_block_list *bl, enum e_map_property property, enum e_send_target t);
void clif_pvpset(struct s_map_session_data *sd, int pvprank, int pvpnum,int type);
void clif_map_property_mapall(int map, enum e_map_property property);
void clif_refine(int fd, int fail, int index, int val);
void clif_upgrademessage(int fd, int result, unsigned short item_id);

//petsystem
void clif_catch_process(struct s_map_session_data *sd);
void clif_pet_roulette(struct s_map_session_data *sd,int data);
void clif_sendegg(struct s_map_session_data *sd);
void clif_send_petstatus(struct s_map_session_data *sd);
void clif_send_petdata(struct s_map_session_data* sd, struct s_pet_data* pd, int type, int param);
#define clif_pet_equip(sd, pd) clif_send_petdata(sd, pd, 3, (pd)->vd.head_bottom)
#define clif_pet_equip_area(pd) clif_send_petdata(NULL, pd, 3, (pd)->vd.head_bottom)
#define clif_pet_performance(pd, param) clif_send_petdata(NULL, pd, 4, param)
void clif_pet_emotion(struct s_pet_data *pd,int param);
void clif_pet_food(struct s_map_session_data *sd,int foodid,int fail);

//friends list
int clif_friendslist_toggle_sub(struct s_map_session_data *sd,va_list ap);
void clif_friendslist_send(struct s_map_session_data *sd);
void clif_friendslist_reqack(struct s_map_session_data *sd, struct s_map_session_data *f_sd, int type);

void clif_weather(int16 m); // [Valaris]
void clif_specialeffect(struct s_block_list* bl, int type, enum e_send_target target); // special effects [Valaris]
void clif_specialeffect_single(struct s_block_list* bl, int type, int fd);
void clif_messagecolor(struct s_block_list* bl, unsigned long color, const char* msg); // Mob/Npc color talk [SnakeDrak]
void clif_messagecolor2(struct s_map_session_data *sd, unsigned long color, const char* msg);		// Use for dispcolor [Napster]
void clif_specialeffect_value(struct s_block_list* bl, int effect_id, int num, e_send_target target);

void clif_GM_kickack(struct s_map_session_data *sd, int id);
void clif_GM_kick(struct s_map_session_data *sd,struct s_map_session_data *tsd);
void clif_manner_message(struct s_map_session_data* sd, uint32 type);
void clif_GM_silence(struct s_map_session_data* sd, struct s_map_session_data* tsd, uint8 type);

void clif_disp_overhead(struct s_block_list *bl, const char* mes);

void clif_get_weapon_view(struct s_map_session_data* sd, unsigned short *rhand, unsigned short *lhand);

void clif_party_xy_remove(struct s_map_session_data *sd); //Fix for minimap [Kevin]
void clif_gospel_info(struct s_map_session_data *sd, int type);
void clif_feel_req(int fd, struct s_map_session_data *sd, uint16 skill_lv);
void clif_starskill(struct s_map_session_data* sd, const char* mapname, int monster_id, unsigned char star, unsigned char result);
void clif_feel_info(struct s_map_session_data* sd, unsigned char feel_level, unsigned char type);
void clif_hate_info(struct s_map_session_data *sd, unsigned char hate_level,int class_, unsigned char type);
void clif_mission_info(struct s_map_session_data *sd, int mob_id, unsigned char progress);
void clif_feel_hate_reset(struct s_map_session_data *sd);

// [blackhole89]
void clif_hominfo(struct s_map_session_data *sd, struct s_homun_data *hd, int flag);
int clif_homskillinfoblock(struct s_map_session_data *sd);
void clif_homskillup(struct s_map_session_data *sd, uint16 skill_id);	//[orn]
int clif_hom_food(struct s_map_session_data *sd,int foodid,int fail);	//[orn]
void clif_send_homdata(struct s_map_session_data *sd, int state, int param);	//[orn]

void clif_equiptickack(struct s_map_session_data* sd, int flag);
void clif_partytickack(struct s_map_session_data* sd, bool flag);
void clif_viewequip_ack(struct s_map_session_data* sd, struct s_map_session_data* tsd);
void clif_equipcheckbox(struct s_map_session_data* sd);

void clif_msg(struct s_map_session_data* sd, unsigned short id);
void clif_msg_value(struct s_map_session_data* sd, unsigned short id, int value);
void clif_msg_skill(struct s_map_session_data* sd, uint16 skill_id, int msg_id);

//quest system [Kevin] [Inkfish]
void clif_quest_send_list(struct s_map_session_data * sd);
void clif_quest_send_mission(struct s_map_session_data * sd);
void clif_quest_add(struct s_map_session_data * sd, struct s_quest * qd);
void clif_quest_delete(struct s_map_session_data * sd, int quest_id);
void clif_quest_update_status(struct s_map_session_data * sd, int quest_id, bool active);
void clif_quest_update_objective(struct s_map_session_data * sd, struct s_quest * qd, int mobid);
void clif_quest_show_event(struct s_map_session_data *sd, struct s_block_list *bl, short state, short color);
void clif_displayexp(struct s_map_session_data *sd, unsigned int exp, char type, bool quest, bool lost);

int clif_send(const uint8* buf, int len, struct s_block_list* bl, enum e_send_target type);
void do_init_clif(void);
void do_final_clif(void);

// MAIL SYSTEM
void clif_Mail_window(int fd, int flag);
void clif_Mail_read(struct s_map_session_data *sd, int mail_id);
void clif_Mail_delete(int fd, int mail_id, short fail);
void clif_Mail_return(int fd, int mail_id, short fail);
void clif_Mail_send(int fd, bool fail);
void clif_Mail_new(int fd, int mail_id, const char *sender, const char *title);
void clif_Mail_refreshinbox(struct s_map_session_data *sd);
void clif_Mail_getattachment(int fd, uint8 flag);
// AUCTION SYSTEM
void clif_Auction_openwindow(struct s_map_session_data *sd);
void clif_Auction_results(struct s_map_session_data *sd, short count, short pages, uint8 *buf);
void clif_Auction_message(int fd, unsigned char flag);
void clif_Auction_close(int fd, unsigned char flag);
void clif_parse_Auction_cancelreg(int fd, struct s_map_session_data *sd);

void clif_bossmapinfo(int fd, struct s_mob_data *md, short flag);
void clif_cashshop_show(struct s_map_session_data *sd, struct s_npc_data *nd);

// ADOPTION
void clif_Adopt_reply(struct s_map_session_data *sd, int type);
void clif_Adopt_request(struct s_map_session_data *sd, struct s_map_session_data *src, int p_id);

// MERCENARIES
void clif_mercenary_info(struct s_map_session_data *sd);
void clif_mercenary_skillblock(struct s_map_session_data *sd);
void clif_mercenary_message(struct s_map_session_data* sd, int message);
void clif_mercenary_updatestatus(struct s_map_session_data *sd, int type);

// RENTAL SYSTEM
void clif_rental_time(int fd, unsigned short nameid, int seconds);
void clif_rental_expired(int fd, int index, unsigned short nameid);

// BOOK READING
void clif_readbook(int fd, int book_id, int page);

// Show Picker
void clif_party_show_picker(struct s_map_session_data * sd, struct s_item * item_data);

// Progress Bar [Inkfish]
void clif_progressbar(struct s_map_session_data * sd, unsigned long color, unsigned int second);
void clif_progressbar_abort(struct s_map_session_data * sd);

void clif_PartyBookingRegisterAck(struct s_map_session_data *sd, int flag);
void clif_PartyBookingDeleteAck(struct s_map_session_data* sd, int flag);
void clif_PartyBookingSearchAck(int fd, struct s_party_booking_ad_info** results, int count, bool more_result);
void clif_PartyBookingUpdateNotify(struct s_map_session_data* sd, struct s_party_booking_ad_info* pb_ad);
void clif_PartyBookingDeleteNotify(struct s_map_session_data* sd, int index);
void clif_PartyBookingInsertNotify(struct s_map_session_data* sd, struct s_party_booking_ad_info* pb_ad);

/* Bank System [Yommy/Hercules] */
void clif_bank_deposit (struct s_map_session_data *sd, enum e_BANKING_DEPOSIT_ACK reason);
void clif_bank_withdraw (struct s_map_session_data *sd,enum e_BANKING_WITHDRAW_ACK reason);
void clif_parse_BankDeposit (int fd, struct s_map_session_data *sd);
void clif_parse_BankWithdraw (int fd, struct s_map_session_data *sd);
void clif_parse_BankCheck (int fd, struct s_map_session_data *sd);
void clif_parse_BankOpen (int fd, struct s_map_session_data *sd);
void clif_parse_BankClose (int fd, struct s_map_session_data *sd);

void clif_showdigit(struct s_map_session_data* sd, unsigned char type, int value);

/// Buying Store System
void clif_buyingstore_open(struct s_map_session_data* sd);
void clif_buyingstore_open_failed(struct s_map_session_data* sd, unsigned short result, unsigned int weight);
void clif_buyingstore_myitemlist(struct s_map_session_data* sd);
void clif_buyingstore_entry(struct s_map_session_data* sd);
void clif_buyingstore_entry_single(struct s_map_session_data* sd, struct s_map_session_data* pl_sd);
void clif_buyingstore_disappear_entry(struct s_map_session_data* sd);
void clif_buyingstore_disappear_entry_single(struct s_map_session_data* sd, struct s_map_session_data* pl_sd);
void clif_buyingstore_itemlist(struct s_map_session_data* sd, struct s_map_session_data* pl_sd);
void clif_buyingstore_trade_failed_buyer(struct s_map_session_data* sd, short result);
void clif_buyingstore_update_item(struct s_map_session_data* sd, unsigned short nameid, unsigned short amount, uint32 char_id, int zeny);
void clif_buyingstore_delete_item(struct s_map_session_data* sd, short index, unsigned short amount, int price);
void clif_buyingstore_trade_failed_seller(struct s_map_session_data* sd, short result, unsigned short nameid);

/// Search Store System
void clif_search_store_info_ack(struct s_map_session_data* sd);
void clif_search_store_info_failed(struct s_map_session_data* sd, unsigned char reason);
void clif_open_search_store_info(struct s_map_session_data* sd);
void clif_search_store_info_click_ack(struct s_map_session_data* sd, short x, short y);

/// Cash Shop
void clif_cashshop_result( struct s_map_session_data* sd, unsigned short item_id, uint16 result );
void clif_cashshop_open( struct s_map_session_data* sd );

void clif_display_pinfo(struct s_map_session_data *sd, int type);

/// Roulette
void clif_roulette_generate_ack(struct s_map_session_data *sd, unsigned char result, short stage, short prizeIdx, short bonusItemID);
void clif_parse_RouletteOpen(int fd, struct s_map_session_data *sd);
void clif_parse_RouletteInfo(int fd, struct s_map_session_data *sd);
void clif_parse_RouletteClose(int fd, struct s_map_session_data *sd);
void clif_parse_RouletteGenerate(int fd, struct s_map_session_data *sd);
void clif_parse_RouletteRecvItem(int fd, struct s_map_session_data *sd);

int clif_elementalconverter_list(struct s_map_session_data *sd);

void clif_millenniumshield(struct s_block_list *bl, short shields);

int clif_spellbook_list(struct s_map_session_data *sd);

int clif_magicdecoy_list(struct s_map_session_data *sd, uint16 skill_lv, short x, short y);

int clif_poison_list(struct s_map_session_data *sd, uint16 skill_lv);

int clif_autoshadowspell_list(struct s_map_session_data *sd);

int clif_skill_itemlistwindow( struct s_map_session_data *sd, uint16 skill_id, uint16 skill_lv );
void clif_elemental_info(struct s_map_session_data *sd);
void clif_elemental_updatestatus(struct s_map_session_data *sd, int type);

void clif_spiritcharm(struct s_map_session_data *sd);

void clif_snap( struct s_block_list *bl, short x, short y );
void clif_monster_hp_bar( struct s_mob_data* md, int fd );

/**
 * Color Table
 **/
enum clif_colors {
	COLOR_DEFAULT,
	COLOR_RED,
	COLOR_WHITE,
	COLOR_YELLOW,
	COLOR_CYAN,
	COLOR_MAX
};
extern unsigned long color_table[COLOR_MAX];
int clif_colormes(int fd, unsigned long color, const char* msg);

void clif_channel_msg(struct s_Channel *channel, struct s_map_session_data *sd, char *msg, short color);

#define clif_menuskill_clear(sd) (sd)->menuskill_id = (sd)->menuskill_val = (sd)->menuskill_val2 = 0;

void clif_ranklist(struct s_map_session_data *sd, int16 rankingType);
void clif_update_rankingpoint(struct s_map_session_data *sd, int rankingtype, int point);

void clif_crimson_marker(struct s_map_session_data *sd, struct s_block_list *bl, bool remove);

void clif_showscript(struct s_block_list* bl, const char* message);
void clif_party_leaderchanged(struct s_map_session_data *sd, int prev_leader_aid, int new_leader_aid);

void clif_account_name(int fd, uint32 account_id, const char* accname);
void clif_notify_bindOnEquip(struct s_map_session_data *sd, int n);

void clif_merge_item_open(struct s_map_session_data *sd);

void clif_broadcast_obtain_special_item(const char *char_name, unsigned short nameid, unsigned short container, enum e_BROADCASTING_SPECIAL_ITEM_OBTAIN type, const char *srcname);

void clif_dressing_room(struct s_map_session_data *sd, int flag);
void clif_navigateTo(struct s_map_session_data *sd, const char* mapname, uint16 x, uint16 y, uint8 flag, bool hideWindow, uint16 mob_id );
void clif_SelectCart(struct s_map_session_data *sd);

#endif /* _CLIF_H_ */
