// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _MAIL_H_
#define _MAIL_H_

#include "../common_old/mmo.h"

enum e_mail_attach_result {
	MAIL_ATTACH_SUCCESS = 0,
#if PACKETVER >= 20150513
	MAIL_ATTACH_WEIGHT = 1,
	MAIL_ATTACH_ERROR = 2,
	MAIL_ATTACH_SPACE = 3,
	MAIL_ATTACH_UNTRADEABLE = 4
#else
	MAIL_ATTACH_WEIGHT = 1,
	MAIL_ATTACH_ERROR = 1,
	MAIL_ATTACH_SPACE = 1,
	MAIL_ATTACH_UNTRADEABLE = 1
#endif
};

void mail_clear(s_map_session_data *sd);
int mail_removeitem(s_map_session_data *sd, short flag, int idx, int amount);
bool mail_removezeny(s_map_session_data *sd, bool flag);
enum e_mail_attach_result mail_setitem(s_map_session_data *sd, short idx, uint32 amount);
bool mail_setattachment(s_map_session_data *sd, s_mail_message *msg);
void mail_getattachment(s_map_session_data* sd, s_mail_message* msg, int zeny, s_item* item);
int mail_openmail(s_map_session_data *sd);
void mail_deliveryfail(s_map_session_data *sd, s_mail_message *msg);
bool mail_invalid_operation(s_map_session_data *sd);
void mail_send(s_map_session_data *sd, const char *dest_name, const char *title, const char *body_msg, int body_len);
void mail_refresh_remaining_amount( s_map_session_data* sd );

#endif /* _MAIL_H_ */
