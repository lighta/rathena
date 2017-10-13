// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef	_VENDING_H_
#define	_VENDING_H_

#include "../common_old/cbasetypes.h"
#include "../common_old/db.h"
//#include "map.h"

struct s_map_session_data;
struct s_search_store_search;
struct s_autotrader;

struct s_vending {
	short index; /// cart index (return item data)
	short amount; ///amout of the item for vending
	unsigned int value; ///at wich price
};

s_DBMap* vending_getdb();
void do_final_vending(void);
void do_init_vending(void);
void do_init_vending_autotrade( void );
 
void vending_reopen( s_map_session_data* sd );
void vending_closevending(s_map_session_data* sd);
int8 vending_openvending(s_map_session_data* sd, const char* message, const uint8* data, int count, struct s_autotrader *at);
void vending_vendinglistreq(s_map_session_data* sd, int id);
void vending_purchasereq(s_map_session_data* sd, int aid, int uid, const uint8* data, int count);
bool vending_search(s_map_session_data* sd, unsigned short nameid);
bool vending_searchall(s_map_session_data* sd, const struct s_search_store_search* s);

#endif /* _VENDING_H_ */
