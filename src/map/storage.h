// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "../common/cbasetypes.h"

//#include "../common/mmo.h"
struct s_storage_data;
struct s_guild_storage;
struct s_item;
//#include "map.h"
struct s_map_session_data;

int storage_delitem(struct s_map_session_data* sd, int n, int amount);
int storage_storageopen(struct s_map_session_data *sd);
void storage_storageadd(struct s_map_session_data *sd,int index,int amount);
void storage_storageget(struct s_map_session_data *sd,int index,int amount);
void storage_storageaddfromcart(struct s_map_session_data *sd,int index,int amount);
void storage_storagegettocart(struct s_map_session_data *sd,int index,int amount);
void storage_storageclose(struct s_map_session_data *sd);
void storage_sortitem(struct s_item* items, unsigned int size);
void do_init_storage(void);
void do_final_storage(void);
void do_reconnect_storage(void);
void storage_storage_quit(struct s_map_session_data *sd, int flag);

struct s_guild_storage* gstorage_guild2storage(int guild_id);
struct s_guild_storage *gstorage_get_storage(int guild_id);
void gstorage_delete(int guild_id);
char gstorage_storageopen(struct s_map_session_data *sd);
bool gstorage_additem(struct s_map_session_data *sd,struct s_guild_storage *stor,struct s_item *item,int amount);
bool gstorage_additem2(struct s_guild_storage *stor, struct s_item *item, int amount);
bool gstorage_delitem(struct s_map_session_data *sd,struct s_guild_storage *stor,int n,int amount);
void gstorage_storageadd(struct s_map_session_data *sd,int index,int amount);
void gstorage_storageget(struct s_map_session_data *sd,int index,int amount);
void gstorage_storageaddfromcart(struct s_map_session_data *sd,int index,int amount);
void gstorage_storagegettocart(struct s_map_session_data *sd,int index,int amount);
void gstorage_storageclose(struct s_map_session_data *sd);
void gstorage_storage_quit(struct s_map_session_data *sd,int flag);
bool gstorage_storagesave(uint32 account_id, int guild_id, int flag);
void gstorage_storagesaved(int guild_id);

int compare_item(struct s_item *a, struct s_item *b);

#endif /* _STORAGE_H_ */
