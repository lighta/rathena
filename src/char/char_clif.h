/*
 * File:   char_clif.h
 * Author: lighta
 *
 * Created on June 15, 2013, 12:06 PM
 */

#ifndef CHAR_CLIF_H
#define CHAR_CLIF_H

#include "char.h"

void chclif_moveCharSlotReply(int fd, s_char_session_data* sd, unsigned short index, short reason);
int chclif_parse_moveCharSlot(int fd, s_char_session_data* sd);

#if PACKETVER_SUPPORTS_PINCODE
void chclif_pincode_sendstate(int fd, s_char_session_data* sd, e_pincode_state state);
int chclif_parse_reqpincode_window(int fd, s_char_session_data* sd);
int chclif_parse_pincode_check(int fd, s_char_session_data* sd);
int chclif_parse_pincode_change(int fd, s_char_session_data* sd);
int chclif_parse_pincode_setnew(int fd, s_char_session_data* sd);

#endif

void chclif_reject(int fd, uint8 errCode);
void chclif_refuse_delchar(int fd, uint8 errCode);
void chclif_charlist_notify(int fd, s_char_session_data* sd);
void chclif_block_character(int fd, s_char_session_data* sd);
int chclif_mmo_send006b(int fd, s_char_session_data* sd);
void chclif_mmo_send082d(int fd, s_char_session_data* sd);
void chclif_mmo_send099d(int fd, s_char_session_data* sd);
void chclif_mmo_char_send(int fd, s_char_session_data* sd);
void chclif_send_auth_result(int fd, char result);
void chclif_char_delete2_ack(int fd, uint32 char_id, uint32 result, time_t delete_date);
void chclif_char_delete2_accept_ack(int fd, uint32 char_id, uint32 result);
void chclif_char_delete2_cancel_ack(int fd, uint32 char_id, uint32 result);

int chclif_parse_char_delete2_req(int fd, s_char_session_data* sd);
int chclif_parse_char_delete2_accept(int fd, s_char_session_data* sd);
int chclif_parse_char_delete2_cancel(int fd, s_char_session_data* sd);

int chclif_parse_maplogin(int fd);
int chclif_parse_reqtoconnect(int fd, s_char_session_data* sd, uint32 ipl);
int chclif_parse_req_charlist(int fd, s_char_session_data* sd);
int chclif_parse_charselect(int fd, s_char_session_data* sd, uint32 ipl);
int chclif_parse_createnewchar(int fd, s_char_session_data* sd, int cmd);
int chclif_parse_delchar(int fd, s_char_session_data* sd, int cmd);
int chclif_parse_keepalive(int fd);
int chclif_parse_reqrename(int fd, s_char_session_data* sd);
int chclif_parse_ackrename(int fd, s_char_session_data* sd);
int chclif_ack_captcha(int fd);
int chclif_parse_reqcaptcha(int fd);
int chclif_parse_chkcaptcha(int fd);
void chclif_block_character(int fd, s_char_session_data* sd);

int chclif_parse(int fd);

#endif  /* CHAR_CLIF_H */

