// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _TRADE_H_
#define _TRADE_H_

//#include "map.h"
struct s_map_session_data;

void trade_traderequest(s_map_session_data* sd, s_map_session_data* target_sd);
void trade_tradeack(s_map_session_data* sd, int type);
void trade_tradeadditem(s_map_session_data* sd, short index, short amount);
void trade_tradeaddzeny(s_map_session_data* sd, int amount);
void trade_tradeok(s_map_session_data* sd);
void trade_tradecancel(s_map_session_data* sd);
void trade_tradecommit(s_map_session_data* sd);

#endif /* _TRADE_H_ */
