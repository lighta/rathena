/**
 * @file account.c
 * Module purpose is to save, load, and update changes into the account table or file.
 * Licensed under GNU GPL.
 *  For more information, see LICENCE in the main folder.
 * @author Athena Dev Teams < r15k
 * @author rAthena Dev Team
 */
#include "account.h"
#include <stdlib.h>
#include <string.h>
#include <algorithm> //min / max


#include "../common_old/malloc.h"
#include "../common_old/mmo.h"
#include "../common_old/showmsg.h"
#include "../common_old/socket.h"
#include "../common_old/sql.h"
#include "../common_old/strlib.h"

//using namespace ra::common_new;
namespace ra {
namespace login {
/// global defines
static const int ACCOUNT_SQL_DB_VERSION = 20170115;

c_ModuleAccount::c_ModuleAccount()
{
}

c_ModuleAccount& c_ModuleAccount::smGetIntance()
{
	static c_ModuleAccount lInstance; //meyer singleton for now

	return lInstance;
}

/// internal structure

struct AccountDB_SQL {
	s_AccountDB vtable;   // public interface
	Sql*        accounts; // SQL handle accounts storage
	std::string db_hostname;
	uint16      db_port;
	std::string db_username;
	std::string db_password;
	std::string db_database;
	std::string codepage;
	// other settings
	bool        case_sensitive;
	//table name
	std::string account_db;
	std::string global_acc_reg_num_table;
	std::string global_acc_reg_str_table;

	AccountDB_SQL()
		: accounts(nullptr)
		, db_hostname()
		, db_port(0)
		, case_sensitive(false)
	{
	}
};

/// internal structure

struct AccountDBIterator_SQL {
	s_AccountDBIterator vtable; // public interface
	AccountDB_SQL*      db;
	int                 last_account_id;

	AccountDBIterator_SQL()
		: db(nullptr)
		, last_account_id(0)
	{
	}
};

/// internal functions
static bool account_db_sql_init(s_AccountDB* self);
static void account_db_sql_destroy(s_AccountDB* self);
static bool account_db_sql_get_property(s_AccountDB* self, const char* key, char* buf, size_t buflen);
static bool account_db_sql_set_property(s_AccountDB* self, const char* option, const char* value);
static bool account_db_sql_create(s_AccountDB* self, struct s_mmo_account* acc);
static bool account_db_sql_remove(s_AccountDB* self, const uint32 account_id);
static bool account_db_sql_save(s_AccountDB* self, const struct s_mmo_account* acc);
static bool account_db_sql_load_num(s_AccountDB* self, struct s_mmo_account* acc, const uint32 account_id);
static bool account_db_sql_load_str(s_AccountDB* self, struct s_mmo_account* acc, const char* userid);
static s_AccountDBIterator* account_db_sql_iterator(s_AccountDB* self);
static void account_db_sql_iter_destroy(s_AccountDBIterator* self);
static bool account_db_sql_iter_next(s_AccountDBIterator* self, struct s_mmo_account* acc);

static bool mmo_auth_fromsql(AccountDB_SQL* db, struct s_mmo_account* acc, uint32 account_id);
static bool mmo_auth_tosql(AccountDB_SQL* db, const struct s_mmo_account* acc, bool is_new);

/// public constructor

s_AccountDB* c_ModuleAccount::account_db_sql(void)
{
	AccountDB_SQL* db = new AccountDB_SQL;

	// set up the vtable
	db->vtable.init         = &account_db_sql_init;
	db->vtable.destroy      = &account_db_sql_destroy;
	db->vtable.get_property = &account_db_sql_get_property;
	db->vtable.set_property = &account_db_sql_set_property;
	db->vtable.save         = &account_db_sql_save;
	db->vtable.create       = &account_db_sql_create;
	db->vtable.remove       = &account_db_sql_remove;
	db->vtable.load_num     = &account_db_sql_load_num;
	db->vtable.load_str     = &account_db_sql_load_str;
	db->vtable.iterator     = &account_db_sql_iterator;

	// initialize to default values
	db->accounts = nullptr;
	// local sql settings
	db->db_hostname = "127.0.0.1";
	db->db_port     = 3306;
	db->db_username = "ragnarok";
	db->db_password = "ragnarok";
	db->db_database = "ragnarok";
	// other settings
	db->case_sensitive           = false;
	db->account_db               = "login";
	db->global_acc_reg_num_table = "global_acc_reg_num";
	db->global_acc_reg_str_table = "global_acc_reg_str";

	return &db->vtable;
}


/* ------------------------------------------------------------------------- */

/**
 * Establish the database connection.
 * @param self: pointer to db
 */
static bool account_db_sql_init(s_AccountDB* self)
{
	AccountDB_SQL* db = (AccountDB_SQL*)self;
	Sql*           sql_handle;
	std::string    username = "ragnarok";
	std::string    password = "";
	std::string    hostname = "127.0.0.1";
	uint16         port     = 3306;
	std::string    database = "ragnarok";
	std::string    codepage;

	db->accounts = Sql_Malloc();
	sql_handle   = db->accounts;

	if (db->db_hostname[0] != '\0') { // local settings
		username = db->db_username;
		password = db->db_password;
		hostname = db->db_hostname;
		port     = db->db_port;
		database = db->db_database;
		codepage = db->codepage;
	}

	if (SQL_ERROR == Sql_Connect(sql_handle, username.c_str(), password.c_str(), hostname.c_str(), port, database.c_str())) {
		ShowError("Couldn't connect with uname='%s',passwd='%s',host='%s',port='%d',database='%s'\n",
		          username, password, hostname, port, database);
		Sql_ShowDebug(sql_handle);
		Sql_Free(db->accounts);
		db->accounts = nullptr;
		return false;
	}

	if (!codepage.empty() && SQL_ERROR == Sql_SetEncoding(sql_handle, codepage.c_str()))
		Sql_ShowDebug(sql_handle);

	return true;
}

/**
 * Destroy the database and close the connection to it.
 * @param self: pointer to db
 */
static void account_db_sql_destroy(s_AccountDB* self)
{
	AccountDB_SQL* db = (AccountDB_SQL*)self;

	Sql_Free(db->accounts);
	db->accounts = nullptr;
	//aFree(db);
	delete db;
}

/**
 * Get configuration information into buf.
 *  If the option is supported, adjust the internal state.
 * @param self: pointer to db
 * @param key: config keyword
 * @param buf: value set of the keyword
 * @param buflen: size of buffer to avoid out of bound
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_get_property(s_AccountDB* self, const char* key, char* buf, size_t buflen)
{
	AccountDB_SQL* db = (AccountDB_SQL*)self;
	const char*    signature;

	signature = "login_server_";
	if (strncmpi(key, signature, strlen(signature)) == 0) {
		key += strlen(signature);
		if (strcmpi(key, "ip") == 0)
			safesnprintf(buf, buflen, "%s", db->db_hostname.c_str());
		else
		if (strcmpi(key, "port") == 0)
			safesnprintf(buf, buflen, "%d", db->db_port);
		else
		if (strcmpi(key, "id") == 0)
			safesnprintf(buf, buflen, "%s", db->db_username.c_str());
		else
		if (strcmpi(key, "pw") == 0)
			safesnprintf(buf, buflen, "%s", db->db_password.c_str());
		else
		if (strcmpi(key, "db") == 0)
			safesnprintf(buf, buflen, "%s", db->db_database.c_str());
		else
		if (strcmpi(key, "account_db") == 0)
			safesnprintf(buf, buflen, "%s", db->account_db.c_str());
		else
		if (strcmpi(key, "global_acc_reg_str_table") == 0)
			safesnprintf(buf, buflen, "%s", db->global_acc_reg_str_table.c_str());
		else
		if (strcmpi(key, "global_acc_reg_num_table") == 0)
			safesnprintf(buf, buflen, "%s", db->global_acc_reg_num_table.c_str());
		else
			return false;  // not found

		return true;
	}

	signature = "login_";
	if (strncmpi(key, signature, strlen(signature)) == 0) {
		key += strlen(signature);
		if (strcmpi(key, "codepage") == 0)
			safesnprintf(buf, buflen, "%s", db->codepage);
		else
		if (strcmpi(key, "case_sensitive") == 0)
			safesnprintf(buf, buflen, "%d", (db->case_sensitive ? 1 : 0));
		else
			return false;  // not found

		return true;
	}

	return false;   // not found
} // account_db_sql_get_property

/**
 * Read and set configuration.
 *  If the option is supported, adjust the internal state.
 * @param self: pointer to db
 * @param key: config keyword
 * @param value: config value for keyword
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_set_property(s_AccountDB* self, const char* key, const char* value)
{
	AccountDB_SQL* db = (AccountDB_SQL*)self;
	const char*    signature;

	signature = "login_server_";
	if (strncmp(key, signature, strlen(signature)) == 0) {
		key += strlen(signature);
		if (strcmpi(key, "ip") == 0)
			db->db_hostname = value;
		else
		if (strcmpi(key, "port") == 0)
			db->db_port = (uint16)strtoul(value, NULL, 10);
		else
		if (strcmpi(key, "id") == 0)
			db->db_username = value;
		else
		if (strcmpi(key, "pw") == 0)
			db->db_password = value;
		else
		if (strcmpi(key, "db") == 0)
			db->db_database = value;
		else
		if (strcmpi(key, "account_db") == 0)
			db->account_db = value;
		//safestrncpy(db->account_db, value, sizeof(db->account_db));
		else
		if (strcmpi(key, "global_acc_reg_str_table") == 0) {
			db->global_acc_reg_str_table = value;
			//safestrncpy(db->global_acc_reg_str_table, value, sizeof(db->global_acc_reg_str_table));
		} else
		if (strcmpi(key, "global_acc_reg_num_table") == 0) {
			db->global_acc_reg_num_table = value;
			//safestrncpy(db->global_acc_reg_num_table, value, sizeof(db->global_acc_reg_num_table));
		} else
			return false;  // not found

		return true;
	}

	signature = "login_";
	if (strncmpi(key, signature, strlen(signature)) == 0) {
		key += strlen(signature);
		if (strcmpi(key, "codepage") == 0)
			db->codepage = value;
		else
		if (strcmpi(key, "case_sensitive") == 0)
			db->case_sensitive = (config_switch(value) == 1);
		else
			return false;  // not found

		return true;
	}

	return false;   // not found
} // account_db_sql_set_property

/**
 * Create a new account entry.
 *  If acc->account_id is -1, the account id will be auto-generated,
 *  and its value will be written to acc->account_id if everything succeeds.
 * @param self: pointer to db
 * @param acc: pointer of mmo_account to save
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_create(s_AccountDB* self, struct s_mmo_account* acc)
{
	AccountDB_SQL* db         = (AccountDB_SQL*)self;
	Sql*           sql_handle = db->accounts;

	// decide on the account id to assign
	uint32 account_id;

	if (acc->account_id > 0) { // caller specifies it manually
		account_id = acc->account_id;
	} else {                   // ask the database
		char*  data;
		size_t len;

		if (SQL_SUCCESS != Sql_Query(sql_handle, "SELECT MAX(`account_id`)+1 FROM `%s`", db->account_db.c_str())) {
			Sql_ShowDebug(sql_handle);
			return false;
		}
		if (SQL_SUCCESS != Sql_NextRow(sql_handle)) {
			Sql_ShowDebug(sql_handle);
			Sql_FreeResult(sql_handle);
			return false;
		}

		Sql_GetData(sql_handle, 0, &data, &len);
		account_id = (data != NULL) ? atoi(data) : 0;
		Sql_FreeResult(sql_handle);
		account_id = max((uint32_t)START_ACCOUNT_NUM, account_id);
	}

	// reached max account. (if 0 is historique, should neverbe true)
	if (account_id == 0 || account_id > END_ACCOUNT_NUM)
		return false;

	// insert the data into the database
	acc->account_id = account_id;
	return mmo_auth_tosql(db, acc, true);
}

/**
 * Delete an existing account entry and its regs.
 * @param self: pointer to db
 * @param account_id: id of user account
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_remove(s_AccountDB* self, const uint32 account_id)
{
	AccountDB_SQL* db         = (AccountDB_SQL*)self;
	Sql*           sql_handle = db->accounts;
	bool           result     = false;

	if (SQL_SUCCESS != Sql_QueryStr(sql_handle, "START TRANSACTION")
	    || SQL_SUCCESS != Sql_Query(sql_handle, "DELETE FROM `%s` WHERE `account_id` = %d", db->account_db.c_str(), account_id)
	    || SQL_SUCCESS != Sql_Query(sql_handle, "DELETE FROM `%s` WHERE `account_id` = %d", db->global_acc_reg_num_table.c_str(), account_id)
	    || SQL_SUCCESS != Sql_Query(sql_handle, "DELETE FROM `%s` WHERE `account_id` = %d", db->global_acc_reg_str_table.c_str(), account_id))
		Sql_ShowDebug(sql_handle);
	else
		result = true;

	result &= (SQL_SUCCESS == Sql_QueryStr(sql_handle, (result == true) ? "COMMIT" : "ROLLBACK"));

	return result;
}

/**
 * Update an existing account with the new data provided (both account and regs).
 * @param self: pointer to db
 * @param acc: pointer of mmo_account to save
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_save(s_AccountDB* self, const struct s_mmo_account* acc)
{
	AccountDB_SQL* db = (AccountDB_SQL*)self;

	return mmo_auth_tosql(db, acc, false);
}

/**
 * Retrieve data from db and store it in the provided data structure.
 *  Filled data structure is done by delegation to mmo_auth_fromsql.
 * @param self: pointer to db
 * @param acc: pointer of mmo_account to fill
 * @param account_id: id of user account
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_load_num(s_AccountDB* self, struct s_mmo_account* acc, const uint32 account_id)
{
	AccountDB_SQL* db = (AccountDB_SQL*)self;

	return mmo_auth_fromsql(db, acc, account_id);
}

/**
 * Retrieve data from db and store it in the provided data structure.
 *  Doesn't actually retrieve data yet: escapes and checks userid, then transforms it to accid for fetching.
 *  Filled data structure is done by delegation to account_db_sql_load_num.
 * @param self: pointer to db
 * @param acc: pointer of mmo_account to fill
 * @param userid: name of user account
 * @return true if successful, false if something has failed
 */
static bool account_db_sql_load_str(s_AccountDB* self, struct s_mmo_account* acc, const char* userid)
{
	AccountDB_SQL* db         = (AccountDB_SQL*)self;
	Sql*           sql_handle = db->accounts;
	char           esc_userid[2 * NAME_LENGTH + 1];
	uint32         account_id;
	char*          data;

	Sql_EscapeString(sql_handle, esc_userid, userid);

	// get the list of account IDs for this user ID
	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `account_id` FROM `%s` WHERE `userid`= %s '%s'",
	                           db->account_db.c_str(), (db->case_sensitive ? "BINARY" : ""), esc_userid)) {
		Sql_ShowDebug(sql_handle);
		return false;
	}

	if (Sql_NumRows(sql_handle) > 1) { // serious problem - duplicit account
		ShowError("account_db_sql_load_str: multiple accounts found when retrieving data for account '%s'!\n", userid);
		Sql_FreeResult(sql_handle);
		return false;
	}

	if (SQL_SUCCESS != Sql_NextRow(sql_handle)) { // no such entry
		Sql_FreeResult(sql_handle);
		return false;
	}

	Sql_GetData(sql_handle, 0, &data, nullptr);
	account_id = atoi(data);

	return account_db_sql_load_num(self, acc, account_id);
}

/**
 * Create a new forward iterator.
 * @param self: pointer to db iterator
 * @return a new db iterator
 */
static s_AccountDBIterator* account_db_sql_iterator(s_AccountDB* self)
{
	AccountDB_SQL*         db   = (AccountDB_SQL*)self;
	AccountDBIterator_SQL* iter = new AccountDBIterator_SQL;

	// set up the vtable
	iter->vtable.destroy = &account_db_sql_iter_destroy;
	iter->vtable.next    = &account_db_sql_iter_next;

	// fill data
	iter->db              = db;
	iter->last_account_id = -1;

	return &iter->vtable;
}

/**
 * Destroys this iterator, releasing all allocated memory (including itself).
 * @param self: pointer to db iterator
 */
static void account_db_sql_iter_destroy(s_AccountDBIterator* self)
{
	AccountDBIterator_SQL* iter = (AccountDBIterator_SQL*)self;

	delete iter;
	//aFree(iter);
}

/**
 * Fetches the next account in the database.
 * @param self: pointer to db iterator
 * @param acc: pointer of mmo_account to fill
 * @return true if next account found and filled, false if something has failed
 */
static bool account_db_sql_iter_next(s_AccountDBIterator* self, struct s_mmo_account* acc)
{
	AccountDBIterator_SQL* iter       = (AccountDBIterator_SQL*)self;
	AccountDB_SQL*         db         = (AccountDB_SQL*)iter->db;
	Sql*                   sql_handle = db->accounts;
	char*                  data;

	// get next account ID
	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `account_id` FROM `%s` WHERE `account_id` > '%d' ORDER BY `account_id` ASC LIMIT 1",
	                           db->account_db.c_str(), iter->last_account_id)) {
		Sql_ShowDebug(sql_handle);
		return false;
	}

	if (SQL_SUCCESS == Sql_NextRow(sql_handle)
	    && SQL_SUCCESS == Sql_GetData(sql_handle, 0, &data, NULL)
	    && data != NULL) {  // get account data
		uint32 account_id;
		account_id = atoi(data);
		if (mmo_auth_fromsql(db, acc, account_id)) {
			iter->last_account_id = account_id;
			Sql_FreeResult(sql_handle);
			return true;
		}
	}
	Sql_FreeResult(sql_handle);
	return false;
}

/**
 * Fetch a struct mmo_account from sql.
 * @param db: pointer to db
 * @param acc: pointer of mmo_account to fill
 * @param account_id: id of user account to take data from
 * @return true if successful, false if something has failed
 */
static bool mmo_auth_fromsql(AccountDB_SQL* db, struct s_mmo_account* acc, uint32 account_id)
{
	Sql*   sql_handle = db->accounts;
	char*  data;
	size_t out_len;

	// retrieve login entry for the specified account
	if (SQL_ERROR == Sql_Query(sql_handle,
#ifdef VIP_ENABLE
	                           "SELECT `account_id`,`userid`,`user_pass`,`sex`,`email`,`group_id`,`state`,`unban_time`,`expiration_time`,`logincount`,`lastlogin`,`last_ip`,`birthdate`,`character_slots`,`pincode`, `pincode_change`, `vip_time`, `old_group` FROM `%s` WHERE `account_id` = %d",
#else
	                           "SELECT `account_id`,`userid`,`user_pass`,`sex`,`email`,`group_id`,`state`,`unban_time`,`expiration_time`,`logincount`,`lastlogin`,`last_ip`,`birthdate`,`character_slots`,`pincode`, `pincode_change` FROM `%s` WHERE `account_id` = %d",
#endif
	                           db->account_db.c_str(), account_id)
	    ) {
		Sql_ShowDebug(sql_handle);
		return false;
	}

	if (SQL_SUCCESS != Sql_NextRow(sql_handle)) { // no such entry
		Sql_FreeResult(sql_handle);
		return false;
	}

	Sql_GetData(sql_handle, 0, &data, &out_len);
	acc->account_id = atoi(data);
	Sql_GetData(sql_handle, 1, &data, &out_len);
	acc->userid = std::string(data, out_len);
	Sql_GetData(sql_handle, 2, &data, &out_len);
	acc->pass = std::string(data, out_len);
	Sql_GetData(sql_handle, 3, &data, &out_len);
	acc->sex = data[0];
	Sql_GetData(sql_handle, 4, &data, &out_len);
	acc->email = std::string(data, out_len);
	Sql_GetData(sql_handle, 5, &data, &out_len);
	acc->group_id = atoi(data);
	Sql_GetData(sql_handle, 6, &data, &out_len);
	acc->state = strtoul(data, NULL, 10);
	Sql_GetData(sql_handle, 7, &data, &out_len);
	acc->unban_time = atol(data);
	Sql_GetData(sql_handle, 8, &data, &out_len);
	acc->expiration_time = atol(data);
	Sql_GetData(sql_handle, 9, &data, &out_len);
	acc->logincount = strtoul(data, nullptr, 10);
	Sql_GetData(sql_handle, 10, &data, &out_len);
	acc->lastlogin = std::string(data, out_len);
	Sql_GetData(sql_handle, 11, &data, &out_len);
	acc->last_ip = std::string(data, out_len);
	Sql_GetData(sql_handle, 12, &data, &out_len);
	acc->birthdate = std::string(data, out_len);
	Sql_GetData(sql_handle, 13, &data, &out_len);
	acc->char_slots = atoi(data);
	Sql_GetData(sql_handle, 14, &data, &out_len);
	acc->pincode = std::string(data, out_len);
	Sql_GetData(sql_handle, 15, &data, &out_len);
	acc->pincode_change = atol(data);
#ifdef VIP_ENABLE
	Sql_GetData(sql_handle, 16, &data, &out_len);
	acc->vip_time = atol(data);
	Sql_GetData(sql_handle, 17, &data, &out_len);
	acc->old_group = atoi(data);
#endif
	Sql_FreeResult(sql_handle);

	return true;
} // mmo_auth_fromsql

/**
 * Save a struct mmo_account in sql.
 * @param db: pointer to db
 * @param acc: pointer of mmo_account to save
 * @param is_new: if it's a new entry or should we update
 * @return true if successful, false if something has failed
 */
static bool mmo_auth_tosql(AccountDB_SQL* db, const struct s_mmo_account* acc, bool is_new)
{
	Sql*     sql_handle = db->accounts;
	SqlStmt* stmt       = SqlStmt_Malloc(sql_handle);
	bool     result     = false;

	// try
	do
	{
		if (SQL_SUCCESS != Sql_QueryStr(sql_handle, "START TRANSACTION")) {
			Sql_ShowDebug(sql_handle);
			break;
		}

		if (is_new) {           // insert into account table
			if (SQL_SUCCESS != SqlStmt_Prepare(stmt,
#ifdef VIP_ENABLE
			                                   "INSERT INTO `%s` (`account_id`, `userid`, `user_pass`, `sex`, `email`, `group_id`, `state`, `unban_time`, `expiration_time`, `logincount`, `lastlogin`, `last_ip`, `birthdate`, `character_slots`, `pincode`, `pincode_change`, `vip_time`, `old_group` ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
#else
			                                   "INSERT INTO `%s` (`account_id`, `userid`, `user_pass`, `sex`, `email`, `group_id`, `state`, `unban_time`, `expiration_time`, `logincount`, `lastlogin`, `last_ip`, `birthdate`, `character_slots`, `pincode`, `pincode_change`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
#endif
			                                   db->account_db.c_str())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 0, SQLDT_INT, (void*)&acc->account_id, sizeof(acc->account_id))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 1, SQLDT_STRING, (void*)acc->userid.c_str(), acc->userid.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 2, SQLDT_STRING, (void*)acc->pass.c_str(), acc->pass.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 3, SQLDT_ENUM, (void*)&acc->sex, sizeof(acc->sex))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 4, SQLDT_STRING, (void*)&acc->email, acc->email.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 5, SQLDT_INT, (void*)&acc->group_id, sizeof(acc->group_id))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 6, SQLDT_UINT, (void*)&acc->state, sizeof(acc->state))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 7, SQLDT_LONG, (void*)&acc->unban_time, sizeof(acc->unban_time))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 8, SQLDT_INT, (void*)&acc->expiration_time, sizeof(acc->expiration_time))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 9, SQLDT_UINT, (void*)&acc->logincount, sizeof(acc->logincount))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 10, SQLDT_STRING, (void*)acc->lastlogin.c_str(), acc->lastlogin.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 11, SQLDT_STRING, (void*)acc->last_ip.c_str(), acc->last_ip.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 12, SQLDT_STRING, (void*)acc->birthdate.c_str(), acc->birthdate.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 13, SQLDT_UCHAR, (void*)&acc->char_slots, sizeof(acc->char_slots))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 14, SQLDT_STRING, (void*)acc->pincode.c_str(), acc->pincode.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 15, SQLDT_LONG, (void*)&acc->pincode_change, sizeof(acc->pincode_change))
#ifdef VIP_ENABLE
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 16, SQLDT_LONG, (void*)&acc->vip_time, sizeof(acc->vip_time))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 17, SQLDT_INT, (void*)&acc->old_group, sizeof(acc->old_group))
#endif
			    || SQL_SUCCESS != SqlStmt_Execute(stmt)
			    ) {
				SqlStmt_ShowDebug(stmt);
				break;
			}
		} else {                // update account table
			if (SQL_SUCCESS != SqlStmt_Prepare(stmt,
#ifdef VIP_ENABLE
			                                   "UPDATE `%s` SET `userid`=?,`user_pass`=?,`sex`=?,`email`=?,`group_id`=?,`state`=?,`unban_time`=?,`expiration_time`=?,`logincount`=?,`lastlogin`=?,`last_ip`=?,`birthdate`=?,`character_slots`=?,`pincode`=?, `pincode_change`=?, `vip_time`=?, `old_group`=? WHERE `account_id` = '%d'",
#else
			                                   "UPDATE `%s` SET `userid`=?,`user_pass`=?,`sex`=?,`email`=?,`group_id`=?,`state`=?,`unban_time`=?,`expiration_time`=?,`logincount`=?,`lastlogin`=?,`last_ip`=?,`birthdate`=?,`character_slots`=?,`pincode`=?, `pincode_change`=? WHERE `account_id` = '%d'",
#endif
			                                   db->account_db.c_str(), acc->account_id)
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 0, SQLDT_STRING, (void*)acc->userid.c_str(), acc->userid.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 1, SQLDT_STRING, (void*)acc->pass.c_str(), acc->pass.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 2, SQLDT_ENUM, (void*)&acc->sex, sizeof(acc->sex))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 3, SQLDT_STRING, (void*)acc->email.c_str(), acc->email.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 4, SQLDT_INT, (void*)&acc->group_id, sizeof(acc->group_id))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 5, SQLDT_UINT, (void*)&acc->state, sizeof(acc->state))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 6, SQLDT_LONG, (void*)&acc->unban_time, sizeof(acc->unban_time))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 7, SQLDT_LONG, (void*)&acc->expiration_time, sizeof(acc->expiration_time))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 8, SQLDT_UINT, (void*)&acc->logincount, sizeof(acc->logincount))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 9, SQLDT_STRING, (void*)acc->lastlogin.c_str(), acc->lastlogin.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 10, SQLDT_STRING, (void*)acc->last_ip.c_str(), acc->last_ip.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 11, SQLDT_STRING, (void*)acc->birthdate.c_str(), acc->birthdate.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 12, SQLDT_UCHAR, (void*)&acc->char_slots, sizeof(acc->char_slots))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 13, SQLDT_STRING, (void*)acc->pincode.c_str(), acc->pincode.size())
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 14, SQLDT_LONG, (void*)&acc->pincode_change, sizeof(acc->pincode_change))
#ifdef VIP_ENABLE
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 15, SQLDT_LONG, (void*)&acc->vip_time, sizeof(acc->vip_time))
			    || SQL_SUCCESS != SqlStmt_BindParam(stmt, 16, SQLDT_INT, (void*)&acc->old_group, sizeof(acc->old_group))
#endif
			    || SQL_SUCCESS != SqlStmt_Execute(stmt)
			    ) {
				SqlStmt_ShowDebug(stmt);
				break;
			}
		}

		// if we got this far, everything was successful
		result = true;
	} while (0);
	// finally

	result &= (SQL_SUCCESS == Sql_QueryStr(sql_handle, (result == true) ? "COMMIT" : "ROLLBACK"));
	SqlStmt_Free(stmt);

	return result;
} // mmo_auth_tosql

void c_ModuleAccount::mmo_save_global_accreg(s_AccountDB* self, int fd, int account_id, int char_id)
{
	Sql*           sql_handle = ((AccountDB_SQL*)self)->accounts;
	AccountDB_SQL* db         = (AccountDB_SQL*)self;
	int            count      = RFIFOW(fd, 12);

	if (count) {
		int  cursor = 14, i;
		char key[32], sval[254], esc_key[32 * 2 + 1], esc_sval[254 * 2 + 1];

		for (i = 0; i < count; i++)
		{
			unsigned int index;
			safestrncpy(key, RFIFOCP(fd, cursor + 1), RFIFOB(fd, cursor));
			Sql_EscapeString(sql_handle, esc_key, key);
			cursor += RFIFOB(fd, cursor) + 1;

			index   = RFIFOL(fd, cursor);
			cursor += 4;

			switch (RFIFOB(fd, cursor++))
			{
			// int
			case 0:
				if (SQL_ERROR == Sql_Query(sql_handle, "REPLACE INTO `%s` (`account_id`,`key`,`index`,`value`) VALUES ('%d','%s','%u','%d')", db->global_acc_reg_num_table.c_str(), account_id, esc_key, index, RFIFOL(fd, cursor)))
					Sql_ShowDebug(sql_handle);
				cursor += 4;
				break;

			case 1:
				if (SQL_ERROR == Sql_Query(sql_handle, "DELETE FROM `%s` WHERE `account_id` = '%d' AND `key` = '%s' AND `index` = '%u' LIMIT 1", db->global_acc_reg_num_table.c_str(), account_id, esc_key, index))
					Sql_ShowDebug(sql_handle);
				break;

			// str
			case 2:
				safestrncpy(sval, RFIFOCP(fd, cursor + 1), RFIFOB(fd, cursor));
				cursor += RFIFOB(fd, cursor) + 1;
				Sql_EscapeString(sql_handle, esc_sval, sval);
				if (SQL_ERROR == Sql_Query(sql_handle, "REPLACE INTO `%s` (`account_id`,`key`,`index`,`value`) VALUES ('%d','%s','%u','%s')", db->global_acc_reg_str_table.c_str(), account_id, esc_key, index, esc_sval))
					Sql_ShowDebug(sql_handle);
				break;

			case 3:
				if (SQL_ERROR == Sql_Query(sql_handle, "DELETE FROM `%s` WHERE `account_id` = '%d' AND `key` = '%s' AND `index` = '%u' LIMIT 1", db->global_acc_reg_str_table.c_str(), account_id, esc_key, index))
					Sql_ShowDebug(sql_handle);
				break;

			default:
				ShowError("mmo_save_global_accreg: unknown type %d\n", RFIFOB(fd, cursor - 1));
				return;
			}
		}
	}
} // mmo_save_global_accreg

void c_ModuleAccount::mmo_send_global_accreg(s_AccountDB* self, int fd, int account_id, int char_id)
{
	Sql*           sql_handle = ((AccountDB_SQL*)self)->accounts;
	AccountDB_SQL* db         = (AccountDB_SQL*)self;
	char*          data;
	size_t         plen = 0;
	size_t         len;

	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `key`, `index`, `value` FROM `%s` WHERE `account_id`='%d'", db->global_acc_reg_str_table.c_str(), account_id))
		Sql_ShowDebug(sql_handle);

	WFIFOHEAD(fd, 60000 + 300);
	WFIFOW(fd, 0) = 0x2726;
	// 0x2 = length, set prior to being sent
	WFIFOL(fd, 4)  = account_id;
	WFIFOL(fd, 8)  = char_id;
	WFIFOB(fd, 12) = 0; // var type (only set when all vars have been sent, regardless of type)
	WFIFOB(fd, 13) = 1; // is string type
	WFIFOW(fd, 14) = 0; // count
	plen           = 16;

	/**
	 * Vessel!
	 *
	 * str type
	 * { keyLength(B), key(<keyLength>), index(L), valLength(B), val(<valLength>) }
	 **/
	while (SQL_SUCCESS == Sql_NextRow(sql_handle))
	{
		Sql_GetData(sql_handle, 0, &data, NULL);
		len = strlen(data) + 1;

		WFIFOB(fd, plen) = (unsigned char)len; // won't be higher; the column size is 32
		plen            += 1;

		safestrncpy(WFIFOCP(fd, plen), data, len);
		plen += len;

		Sql_GetData(sql_handle, 1, &data, NULL);

		WFIFOL(fd, plen) = (unsigned int)atol(data);
		plen            += 4;

		Sql_GetData(sql_handle, 2, &data, NULL);
		len = strlen(data) + 1;

		WFIFOB(fd, plen) = (unsigned char)len; // won't be higher; the column size is 254
		plen            += 1;

		safestrncpy(WFIFOCP(fd, plen), data, len);
		plen += len;

		WFIFOW(fd, 14) += 1;

		if (plen > 60000) {
			WFIFOW(fd, 2) = static_cast<uint16>(plen);
			WFIFOSET(fd, plen);

			// prepare follow up
			WFIFOHEAD(fd, 60000 + 300);
			WFIFOW(fd, 0) = 0x2726;
			// 0x2 = length, set prior to being sent
			WFIFOL(fd, 4)  = account_id;
			WFIFOL(fd, 8)  = char_id;
			WFIFOB(fd, 12) = 0; // var type (only set when all vars have been sent, regardless of type)
			WFIFOB(fd, 13) = 1; // is string type
			WFIFOW(fd, 14) = 0; // count
			plen           = 16;
		}
	}

	WFIFOW(fd, 2) = static_cast<uint16>(plen);
	WFIFOSET(fd, plen);

	Sql_FreeResult(sql_handle);

	if (SQL_ERROR == Sql_Query(sql_handle, "SELECT `key`, `index`, `value` FROM `%s` WHERE `account_id`='%d'", db->global_acc_reg_num_table.c_str(), account_id))
		Sql_ShowDebug(sql_handle);

	WFIFOHEAD(fd, 60000 + 300);
	WFIFOW(fd, 0) = 0x2726;
	// 0x2 = length, set prior to being sent
	WFIFOL(fd, 4)  = account_id;
	WFIFOL(fd, 8)  = char_id;
	WFIFOB(fd, 12) = 0; // var type (only set when all vars have been sent, regardless of type)
	WFIFOB(fd, 13) = 0; // is int type
	WFIFOW(fd, 14) = 0; // count
	plen           = 16;

	/**
	 * Vessel!
	 *
	 * int type
	 * { keyLength(B), key(<keyLength>), index(L), value(L) }
	 **/
	while (SQL_SUCCESS == Sql_NextRow(sql_handle))
	{
		Sql_GetData(sql_handle, 0, &data, NULL);
		len = strlen(data) + 1;

		WFIFOB(fd, plen) = (unsigned char)len; // won't be higher; the column size is 32
		plen            += 1;

		safestrncpy(WFIFOCP(fd, plen), data, len);
		plen += len;

		Sql_GetData(sql_handle, 1, &data, NULL);

		WFIFOL(fd, plen) = (unsigned int)atol(data);
		plen            += 4;

		Sql_GetData(sql_handle, 2, &data, NULL);

		WFIFOL(fd, plen) = atoi(data);
		plen            += 4;

		WFIFOW(fd, 14) += 1;

		if (plen > 60000) {
			WFIFOW(fd, 2) = static_cast<uint16>(plen);
			WFIFOSET(fd, plen);

			// prepare follow up
			WFIFOHEAD(fd, 60000 + 300);
			WFIFOW(fd, 0) = 0x2726;
			// 0x2 = length, set prior to being sent
			WFIFOL(fd, 4)  = account_id;
			WFIFOL(fd, 8)  = char_id;
			WFIFOB(fd, 12) = 0; // var type (only set when all vars have been sent, regardless of type)
			WFIFOB(fd, 13) = 0; // is int type
			WFIFOW(fd, 14) = 0; // count

			plen = 16;
		}
	}

	WFIFOB(fd, 12) = 1;
	WFIFOW(fd, 2)  = static_cast<uint16>(plen);
	WFIFOSET(fd, plen);

	Sql_FreeResult(sql_handle);
} // mmo_send_global_accreg
}
}
