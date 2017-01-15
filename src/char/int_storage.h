// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _INT_STORAGE_SQL_H_
#define _INT_STORAGE_SQL_H_

#include "../common_old/cbasetypes.h"

  struct s_storage_data;
  struct s_guild_storage;

  int inter_storage_sql_init(void);
  void inter_storage_sql_final(void);
  int inter_storage_delete(uint32 account_id);
  int inter_guild_storage_delete(int guild_id);

  int inter_storage_parse_frommap(int fd);

  int storage_fromsql(uint32 account_id, struct s_storage_data* p);
  int storage_tosql(uint32 account_id, struct s_storage_data *p);
  int guild_storage_tosql(int guild_id, struct s_guild_storage *p);


#endif /* _INT_STORAGE_SQL_H_ */
