/**
 * @file ipban.c
 * Module purpose is to read configuration for login-server and handle accounts,
 *  and also to synchronize all login interfaces: loginchrif, loginclif, logincnslif.
 * Licensed under GNU GPL.
 *  For more information, see LICENCE in the main folder.
 * @author Athena Dev Teams < r15k
 * @author rAthena Dev Team
 */
#include "ipban.h"
#include <stdlib.h>
#include <string.h>

#include "../common_old/cbasetypes.h"
#include "../common_old/showmsg.h"
#include "../common_old/sql.h"
#include "../common_old/strlib.h"
#include "../common_old/timer.h"

#include "login.h"
#include "loginlog.h"

namespace ra {
namespace login {
// globals //tbd move this
struct sRAII_sql_handle {
	Sql* sql_handle;
	sRAII_sql_handle() : sql_handle(nullptr){}
	//auto close connections when no one need modue anymore
	~sRAII_sql_handle() { Sql_Free(sql_handle); }
};
static std::shared_ptr<sRAII_sql_handle> sa_SQL_Handle;

struct                                   c_ModuleIpBan::pImpl {
	int         cleanup_timer_id;
	bool        ipban_inited;
	// login sql settings
	std::string ipban_db_hostname;
	uint16      ipban_db_port = 3306;
	std::string ipban_db_username;
	std::string ipban_db_password;
	std::string ipban_db_database;
	std::string ipban_codepage;
	std::string ipban_table;

	pImpl()
		: cleanup_timer_id(INVALID_TIMER)
		, ipban_inited(false)
		, ipban_db_hostname("127.0.0.1")
		, ipban_db_port(3306)
		, ipban_db_username("ragnarok")
		, ipban_db_database("ragnarok")
		, ipban_table("ipbanlist")
	{}
};

c_ModuleIpBan::c_ModuleIpBan()
	: aPimpl(new c_ModuleIpBan::pImpl)
{
}

c_ModuleIpBan& c_ModuleIpBan::smGetInstance()
{
	static c_ModuleIpBan lInstance;

	return lInstance;
}

/**
 * Check if ip is in the active bans list.
 * @param ip: ipv4 ip to check if ban
 * @return true if found or error, false if not in list
 */
bool c_ModuleIpBan::ipban_check(uint32 ip)
{
	uint8* p    = (uint8*)&ip;
	char*  data = NULL;
	int    matches;

	if (!login_config.ipban)
		return false;  // ipban disabled

	//tmp .get issue will be fixed with common_new sql wrapper
	if (SQL_ERROR == Sql_Query(sa_SQL_Handle->sql_handle, "SELECT count(*) FROM `%s` WHERE `rtime` > NOW() AND (`list` = '%u.*.*.*' OR `list` = '%u.%u.*.*' OR `list` = '%u.%u.%u.*' OR `list` = '%u.%u.%u.%u')",
	                           aPimpl->ipban_table.c_str(), p[3], p[3], p[2], p[3], p[2], p[1], p[3], p[2], p[1], p[0])) {
		Sql_ShowDebug(sa_SQL_Handle->sql_handle);
		// close connection because we can't verify their connectivity.
		return true;
	}

	if (SQL_ERROR == Sql_NextRow(sa_SQL_Handle->sql_handle))
		return true;  // Shouldn't happen, but just in case...

	Sql_GetData(sa_SQL_Handle->sql_handle, 0, &data, NULL);
	matches = atoi(data);
	Sql_FreeResult(sa_SQL_Handle->sql_handle);

	return(matches > 0);
}

/**
 * Log a failed attempt.
 *  Also bans the user if too many failed attempts are made.
 * @param ip: ipv4 ip to record the failure
 */
void c_ModuleIpBan::ipban_log(uint32 ip)
{
	unsigned long failures;

	if (!login_config.ipban)
		return;                                                                         // ipban disabled

	failures = loginlog_failedattempts(ip, login_config.dynamic_pass_failure_ban_interval); // how many times failed account? in one ip.

	// if over the limit, add a temporary ban entry
	if (failures >= login_config.dynamic_pass_failure_ban_limit) {
		uint8* p = (uint8*)&ip;
		if (SQL_ERROR == Sql_Query(sa_SQL_Handle->sql_handle, "INSERT INTO `%s`(`list`,`btime`,`rtime`,`reason`) VALUES ('%u.%u.%u.*', NOW() , NOW() +  INTERVAL %d MINUTE ,'Password error ban')",
		                           aPimpl->ipban_table.c_str(), p[3], p[2], p[1], login_config.dynamic_pass_failure_ban_duration))
			Sql_ShowDebug(sa_SQL_Handle->sql_handle);
	}
}

/**
 * Timered function to remove expired bans.
 *  Request all characters to update their registered ip and transmit their new ip.
 *  Performed each ip_sync_interval.
 * @param tid: timer id
 * @param tick: tick of execution
 * @param id: unused
 * @param data: unused
 * @return 0
 */
int c_ModuleIpBan::ipban_cleanup(int tid, unsigned int tick, int id, intptr_t data)
{
	if (!login_config.ipban)
		return 0;  // ipban disabled

	if (SQL_ERROR == Sql_Query(sa_SQL_Handle->sql_handle, "DELETE FROM `ipbanlist` WHERE `rtime` <= NOW()"))
		Sql_ShowDebug(sa_SQL_Handle->sql_handle);

	return 0;
}

/**
 * Read configuration options.
 * @param key: config keyword
 * @param value: config value for keyword
 * @return true if successful, false if config not complete or server already running
 */
bool c_ModuleIpBan::ipban_config_read(const char* key, const char* value)
{
	const char* signature;

	if (aPimpl->ipban_inited)
		return false;  // settings can only be changed before init

	signature = "ipban_db_";
	if (strncmpi(key, signature, strlen(signature)) == 0) {
		key += strlen(signature);
		if (strcmpi(key, "ip") == 0)
			aPimpl->ipban_db_hostname = value;
		else
		if (strcmpi(key, "port") == 0)
			aPimpl->ipban_db_port = (uint16)strtoul(value, nullptr, 10);
		else
		if (strcmpi(key, "id") == 0)
			aPimpl->ipban_db_username = value;
		else
		if (strcmpi(key, "pw") == 0)
			aPimpl->ipban_db_password = value;
		else
		if (strcmpi(key, "db") == 0)
			aPimpl->ipban_db_database = value;
		else
			return false;  // not found

		return true;
	}

	signature = "ipban_";
	if (strncmpi(key, signature, strlen(signature)) == 0) {
		key += strlen(signature);
		if (strcmpi(key, "codepage") == 0)
			aPimpl->ipban_codepage = value;
		else
		if (strcmpi(key, "ipban_table") == 0)
			aPimpl->ipban_table = value;
		else
		if (strcmpi(key, "enable") == 0)
			login_config.ipban = config_switch(value) != 0;
		else
		if (strcmpi(key, "dynamic_pass_failure_ban") == 0)
			login_config.dynamic_pass_failure_ban = config_switch(value) != 0;
		else
		if (strcmpi(key, "dynamic_pass_failure_ban_interval") == 0)
			login_config.dynamic_pass_failure_ban_interval = atoi(value);
		else
		if (strcmpi(key, "dynamic_pass_failure_ban_limit") == 0)
			login_config.dynamic_pass_failure_ban_limit = atoi(value);
		else
		if (strcmpi(key, "dynamic_pass_failure_ban_duration") == 0)
			login_config.dynamic_pass_failure_ban_duration = atoi(value);
		else
			return false;  // not found

		return true;
	}

	return false;   // not found
} // ipban_config_read

/// Constructor destructor

/**
 * Initialize the module.
 * Launched at login-serv start, create db or other long scope variable here.
 */
void c_ModuleIpBan::ipban_init(void)
{
	const char* username = aPimpl->ipban_db_username.c_str();
	const char* password = aPimpl->ipban_db_password.c_str();
	const char* hostname = aPimpl->ipban_db_hostname.c_str();
	uint16      port     = aPimpl->ipban_db_port;
	const char* database = aPimpl->ipban_db_database.c_str();
	const char* codepage = aPimpl->ipban_codepage.c_str();

	aPimpl->ipban_inited = true;

	if (!login_config.ipban)
		return;                           // ipban disabled

	if (!aPimpl->ipban_db_hostname.empty()) { // local settings
		username = aPimpl->ipban_db_username.c_str();
		password = aPimpl->ipban_db_password.c_str();
		hostname = aPimpl->ipban_db_hostname.c_str();
		port     = aPimpl->ipban_db_port;
		database = aPimpl->ipban_db_database.c_str();
		codepage = aPimpl->ipban_codepage.c_str();
	}

	// establish connections
	sa_SQL_Handle.reset(new sRAII_sql_handle());
	sa_SQL_Handle->sql_handle = Sql_Malloc();

	if (SQL_ERROR == Sql_Connect(sa_SQL_Handle->sql_handle, username, password, hostname, port, database)) {
		ShowError("Couldn't connect with uname='%s',passwd='%s',host='%s',port='%d',database='%s'\n",
		          username, password, hostname, port, database);
		Sql_ShowDebug(sa_SQL_Handle->sql_handle);
		Sql_Free(sa_SQL_Handle->sql_handle);
		exit(EXIT_FAILURE);
	}
	ShowInfo("Ipban connection made.\n");

	if (codepage[0] != '\0' && SQL_ERROR == Sql_SetEncoding(sa_SQL_Handle->sql_handle, codepage))
		Sql_ShowDebug(sa_SQL_Handle->sql_handle);

	if (login_config.ipban_cleanup_interval > 0) { // set up periodic cleanup of connection history and active bans
		add_timer_func_list(ipban_cleanup, "ipban_cleanup");
		aPimpl->cleanup_timer_id = add_timer_interval(gettick() + 10, ipban_cleanup, 0, 0, login_config.ipban_cleanup_interval * 1000);
	} else                  // make sure it gets cleaned up on login-server start regardless of interval-based cleanups
		ipban_cleanup(0, 0, 0, 0);
} // ipban_init

/**
 * Destroy the module.
 * Launched at login-serv end, cleanup db connection or other thing here.
 */
void c_ModuleIpBan::ipban_final(void)
{
	if (!login_config.ipban)
		return;  // ipban disabled

	if (login_config.ipban_cleanup_interval > 0)
		// release data
		delete_timer(aPimpl->cleanup_timer_id, ipban_cleanup);

	ipban_cleanup(0, 0, 0, 0); // always clean up on login-server stop
}
}
}