// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "../common_old/cbasetypes.h"

//#include "../common_old/mmo.h"
struct s_item;
//#include "map.h"
struct s_map_session_data; //pc.h
struct s_storage; //mmo.h

extern struct s_storage_table *storage_db;
extern int storage_count;

enum e_storage_add {
	STORAGE_ADD_OK,
	STORAGE_ADD_NOROOM,
	STORAGE_ADD_NOACCESS,
	STORAGE_ADD_INVALID,
};

const char *storage_getName(uint8 id);
bool storage_exists(uint8 id);

int storage_delitem(s_map_session_data* sd, s_storage *stor, int index, int amount);
int storage_storageopen(s_map_session_data *sd);
void storage_storageadd(s_map_session_data *sd, s_storage *stor, int index, int amount);
void storage_storageget(s_map_session_data *sd, s_storage *stor, int index, int amount);
void storage_storageaddfromcart(s_map_session_data *sd, s_storage *stor, int index, int amount);
void storage_storagegettocart(s_map_session_data *sd, s_storage *stor, int index, int amount);
void storage_storagesave(s_map_session_data *sd);
void storage_storageclose(s_map_session_data *sd);
void storage_sortitem(struct s_item* items, unsigned int size);
void do_init_storage(void);
void do_final_storage(void);
void do_reconnect_storage(void);
void storage_storage_quit(s_map_session_data *sd, int flag);

s_storage* guild2storage(int guild_id);
s_storage* guild2storage2(int guild_id);
void storage_guild_delete(int guild_id);
char storage_guild_storageopen(s_map_session_data *sd);
bool storage_guild_additem(s_map_session_data *sd,s_storage *stor,s_item *item_data,int amount);
bool storage_guild_additem2(s_storage* stor, s_item* item, int amount);
bool storage_guild_delitem(s_map_session_data *sd,s_storage *stor,int n,int amount);
void storage_guild_storageadd(s_map_session_data *sd,int index,int amount);
void storage_guild_storageget(s_map_session_data *sd,int index,int amount);
void storage_guild_storageaddfromcart(s_map_session_data *sd,int index,int amount);
void storage_guild_storagegettocart(s_map_session_data *sd,int index,int amount);
void storage_guild_storageclose(s_map_session_data *sd);
void storage_guild_storage_quit(s_map_session_data *sd,int flag);
bool storage_guild_storagesave(uint32 account_id, int guild_id, int flag);
void storage_guild_storagesaved(int guild_id); //Ack from char server that guild store was saved.

// Premium Storage [Cydh]
void storage_premiumStorage_open(s_map_session_data *sd);
bool storage_premiumStorage_load(s_map_session_data *sd, uint8 num, uint8 mode);
void storage_premiumStorage_save(s_map_session_data *sd);
void storage_premiumStorage_saved(s_map_session_data *sd);
void storage_premiumStorage_close(s_map_session_data *sd);
void storage_premiumStorage_quit(s_map_session_data *sd);

int compare_item(struct s_item *a, struct s_item *b);

#endif /* _STORAGE_H_ */
