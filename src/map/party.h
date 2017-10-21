// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _PARTY_H_
#define _PARTY_H_

#include <cstdarg>

#include "../common_old/mmo.h" // struct party

struct s_block_list;
struct s_map_session_data;
struct s_party;
struct s_item;


#define MAX_PARTY_BOOKING_JOBS       6
#define MAX_PARTY_BOOKING_RESULTS    10

struct s_party_member_data {
	s_map_session_data* sd;
	unsigned int        hp; //For HP,x,y refreshing.
	unsigned short      x, y;
};

struct s_party_data {
	struct s_party             party;
	struct s_party_member_data data[MAX_PARTY];
	uint8                      itemc; //For item distribution, position of last picker in party
	unsigned short             instance_id;
	struct {
		unsigned monk    : 1; //There's at least one monk in party?
		unsigned sg      : 1; //There's at least one Star Gladiator in party?
		unsigned snovice : 1; //There's a Super Novice
		unsigned tk      : 1; //There's a taekwon
	}
	state;
};

struct s_party_booking_detail {
	short level;
	short mapid;
	short job[MAX_PARTY_BOOKING_JOBS];
};

struct s_party_booking_ad_info {
	unsigned long                 index;
	char                          charname[NAME_LENGTH];
	long                          starttime;
	struct s_party_booking_detail p_detail;
};

extern int party_create_byscript;

void do_init_party(void);
void do_final_party(void);
struct s_party_data* party_search(int party_id);
struct s_party_data* party_searchname(const char* str);
int party_getmemberid(struct s_party_data* p, s_map_session_data* sd);
s_map_session_data* party_getavailablesd(struct s_party_data* p);

int party_create(s_map_session_data* sd, char* name, int item, int item2);
void party_created(uint32 account_id, uint32 char_id, int fail, int party_id, char* name);
int party_request_info(int party_id, uint32 char_id);
int party_invite(s_map_session_data* sd, s_map_session_data* tsd);
void party_member_joined(s_map_session_data* sd);
int party_member_added(int party_id, uint32 account_id, uint32 char_id, int flag);
int party_leave(s_map_session_data* sd);
int party_removemember(s_map_session_data* sd, uint32 account_id, char* name);
int party_removemember2(s_map_session_data* sd, uint32 char_id, int party_id);
int party_member_withdraw(int party_id, uint32 account_id, uint32 char_id, char* name, enum e_party_member_withdraw type);
int party_reply_invite(s_map_session_data* sd, int party_id, int flag);

#define party_add_member(party_id, sd)    party_reply_invite(sd, party_id, 1)
int party_recv_noinfo(int party_id, uint32 char_id);
int party_recv_info(struct s_party* sp, uint32 char_id);
int party_recv_movemap(int party_id, uint32 account_id, uint32 char_id, unsigned short map, int online, int lv);
int party_broken(int party_id);
int party_optionchanged(int party_id, uint32 account_id, int exp, int item, int flag);
int party_changeoption(s_map_session_data* sd, int exp, int item);
int party_setoption(struct s_party_data* party, int option, int flag);
int party_changeleader(s_map_session_data* sd, s_map_session_data* t_sd, struct s_party_data* p);
void party_send_movemap(s_map_session_data* sd);
void party_send_levelup(s_map_session_data* sd);
int party_send_logout(s_map_session_data* sd);
int party_send_message(s_map_session_data* sd, const char* mes, int len);
int party_recv_message(int party_id, uint32 account_id, const char* mes, int len);
int party_skill_check(s_map_session_data* sd, int party_id, uint16 skill_id, uint16 skill_lv);
int party_send_xy_clear(struct s_party_data* p);
void party_exp_share(struct s_party_data* p, struct s_block_list* src, unsigned int base_exp, unsigned int job_exp, int zeny);
int party_share_loot(struct s_party_data* p, s_map_session_data* sd, struct s_item* item, int first_charid);
int party_send_dot_remove(s_map_session_data* sd);
int party_sub_count(struct s_block_list* bl, va_list ap);
int party_sub_count_class(struct s_block_list* bl, va_list ap);
int party_foreachsamemap(int (* func)(struct s_block_list*, va_list), s_map_session_data* sd, int range, ...);

/*==========================================
 * Party Booking in KRO [Spiria]
 *------------------------------------------*/
void party_booking_register(s_map_session_data* sd, short level, short mapid, short* job);
void party_booking_update(s_map_session_data* sd, short* job);
void party_booking_search(s_map_session_data* sd, short level, short mapid, short job, unsigned long lastindex, short resultcount);
bool party_booking_delete(s_map_session_data* sd);

#endif /* _PARTY_H_ */
