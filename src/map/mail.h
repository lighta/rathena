// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _MAIL_H_
#define _MAIL_H_

#include "../common/mmo.h"


void mail_clear(struct s_map_session_data *sd);
int mail_removeitem(struct s_map_session_data *sd, short flag);
int mail_removezeny(struct s_map_session_data *sd, short flag);
bool mail_setitem(struct s_map_session_data *sd, short idx, uint32 amount);
bool mail_setattachment(struct s_map_session_data *sd, struct s_mail_message *msg);
void mail_getattachment(struct s_map_session_data* sd, int zeny, struct s_item* item);
int mail_openmail(struct s_map_session_data *sd);
void mail_deliveryfail(struct s_map_session_data *sd, struct s_mail_message *msg);
bool mail_invalid_operation(struct s_map_session_data *sd);
void mail_send(struct s_map_session_data *sd, const char *dest_name, const char *title, const char *body_msg, int body_len);

#endif /* _MAIL_H_ */
