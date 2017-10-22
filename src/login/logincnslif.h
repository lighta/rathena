/**
 * @file logincnslif.h
 * Module purpose is to handle incoming and outgoing requests with console.
 * Licensed under GNU GPL.
 *  For more information, see LICENCE in the main folder.
 * @author Athena Dev Teams originally in login.c
 * @author rAthena Dev Team
 */

#pragma once
#ifndef CONSOLEIF_H
#define CONSOLEIF_H

#include <memory>
/**
 * Login-server console help: starting option info.
 *  Do not rename function used as extern.
 * @param do_exit: terminate program execution ?
 * Remark ,to integrate to module
 */
void display_helpscreen(bool do_exit);

namespace ra {
namespace login {
class c_ModuleCnslif {
private:
	struct pImpl;
	std::unique_ptr<pImpl> aPimpl;
	c_ModuleCnslif();
public:
	static c_ModuleCnslif& smGetInstance();
};

/**
 * Console Command Parser
 * Transmited from command cli.c
 * note common name for all serv do not rename (extern in cli)
 * @author [Wizputer]
 * @param buf: buffer to parse, (from console)
 * @return 1=success
 */
int cnslif_parse(const char* buf);

/**
 * Read the option specified in command line
 *  and assign the confs used by the different server.
 * @param argc:
 * @param argv:
 * @return true or Exit on failure.
 */
int logcnslif_get_options(int argc, char** argv);

/**
 * Initialize the module.
 * Launched at login-serv start, create db or other long scope variable here.
 */
void do_init_logincnslif(void);

/**
 * Handler to cleanup module, called when login-server stops.
 */
void do_final_logincnslif(void);
}
}

#endif  /* CONSOLEIF_H */