/**
 * @file loginchrif.h
 * Module purpose is to handle incoming and outgoing requests with char-server.
 * Licensed under GNU GPL.
 *  For more information, see LICENCE in the main folder.
 * @author Athena Dev Teams originally in login.c
 * @author rAthena Dev Team
 */

#pragma once
#ifndef LOGINCHRIF_H
#define LOGINCHRIF_H

#include <memory>
#include "../common_old/cbasetypes.h"
#include "../common_old/mmo.h" //PACKETVER_SUPPORTS_PINCODE

namespace ra {
namespace login {
//tmp wrapper
int logchrif_parse(int fd);

class c_ModuleChrif {
private:
	struct pImpl;
	std::unique_ptr<pImpl> aPimpl;
	c_ModuleChrif();

	void logchrif_on_disconnect(int id);
	int logchrif_parse_reqauth(int fd, int id, char* ip);
	int logchrif_parse_ackusercount(int fd, int id);
	int logchrif_send_accdata(int fd, uint32 aid);
	int logchrif_sendvipdata(int fd, struct s_mmo_account acc, unsigned char flag, int mapfd);
	int logchrif_parse_reqaccdata(int fd, int id, char* ip);
	int logchrif_parse_keepalive(int fd);
	int logchrif_parse_reqchangemail(int fd, int id, char* ip);
	int logchrif_parse_requpdaccstate(int fd, int id, char* ip);
	int logchrif_parse_reqbanacc(int fd, int id, char* ip);
	int logchrif_parse_reqchgsex(int fd, int id, char* ip);
	int logchrif_parse_upd_global_accreg(int fd, int id, char* ip);
	int logchrif_parse_requnbanacc(int fd, int id, char* ip);
	int logchrif_parse_setacconline(int fd, int id);
	int logchrif_parse_setaccoffline(int fd);
	int logchrif_parse_updonlinedb(int fd, int id);
	int logchrif_parse_req_global_accreg(int fd);
	int logchrif_parse_updcharip(int fd, int id);
	int logchrif_parse_setalloffline(int fd, int id);

#if PACKETVER_SUPPORTS_PINCODE
	int logchrif_parse_updpincode(int fd);
	int logchrif_parse_pincode_authfail(int fd);

#endif
	int logchrif_parse_reqvipdata(int fd);
	int logchrif_parse_accinfo(int fd);
	void logchrif_server_init(int id);
	void logchrif_server_destroy(int id);
	void logchrif_server_reset(int id);

public:
	static c_ModuleChrif& smGetInstance();

	static int logchrif_sync_ip_addresses(int tid, unsigned int tick, int id, intptr_t data);

	/**
	 * Entry point from char-server to log-server.
	 * Function that checks incoming command, then splits it to the correct handler.
	 * @param fd: file descriptor to parse, (link to char-serv)
	 * @return 0=invalid server,marked for disconnection,unknow packet; 1=success
	 */
	int logchrif_parse(int fd);

	/**
	 * Packet send to all char-servers, except one. (wos: without our self)
	 * @param sfd: fd to discard sending to
	 * @param buf: packet to send in form of an array buffer
	 * @param len: size of packet
	 * @return : the number of char-serv the packet was sent to
	 */
	int logchrif_sendallwos(int sfd, uint8* buf, size_t len);

	/**
	 * loginchrif constructor
	 *  Initialisation, function called at start of the login-serv.
	 */
	void do_init_loginchrif(void);

	/**
	 * Signal handler
	 *  This function attempts to properly close the server when an interrupt signal is received.
	 *  current signal catch : SIGTERM, SIGINT
	 */
	void do_shutdown_loginchrif(void);

	/**
	 * loginchrif destructor
	 *  dealloc..., function called at exit of the login-serv
	 */
	void do_final_loginchrif(void);
};
}       //end namespace
}

#endif  /* LOGINCHRIF_H */

