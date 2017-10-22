// Copyright (c) Athena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <string>

#ifndef _WIN32
#include <unistd.h>
#else
#include "winapi.h" // Console close event handling
#include <direct.h> // _chdir
#endif

#include "core.h"
#include "mmo.h"
#include "showmsg.h"
#include "malloc.h"
#include "strlib.h"
#ifndef MINICORE
#include "ers.h"
#include "socket.h"
#include "timer.h"
#include "thread.h"
#include "mempool.h"
#include "sql.h"
#endif

/// Called when a terminate signal is received.
void (* shutdown_callback)(void) = NULL;

#if defined (BUILDBOT)
int buildbotflag = 0;
#endif

int   runflag     = CORE_ST_RUN;
char  db_path[12] = "db"; /// relative path for db from server

char* SERVER_NAME = NULL;
char  SERVER_TYPE = ATHENA_SERVER_NONE;

#ifndef MINICORE        // minimalist Core
// Added by Gabuzomeu
//
// This is an implementation of signal() using sigaction() for portability.
// (sigaction() is POSIX; signal() is not.)  Taken from Stevens' _Advanced
// Programming in the UNIX Environment_.
//
#ifdef WIN32    // windows don't have SIGPIPE
#define SIGPIPE    SIGINT
#endif

#ifndef POSIX
#define compat_signal(signo, func)    signal(signo, func)
#else
sigfunc* compat_signal(int signo, sigfunc* func)
{
	struct sigaction sact, oact;

	sact.sa_handler = func;
	sigemptyset(&sact.sa_mask);
	sact.sa_flags = 0;
#ifdef SA_INTERRUPT
	sact.sa_flags |= SA_INTERRUPT;  /* SunOS */
#endif

	if (sigaction(signo, &sact, &oact) < 0)
		return(SIG_ERR);

	return(oact.sa_handler);
}
#endif

/*======================================
 *	CORE : Console events for Windows
 *--------------------------------------*/
#ifdef _WIN32
static BOOL WINAPI console_handler(DWORD c_event)
{
	switch (c_event)
	{
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		if (shutdown_callback != NULL)
			shutdown_callback();
		else
			runflag = CORE_ST_STOP;  // auto-shutdown
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

static void cevents_init()
{
	if (SetConsoleCtrlHandler(console_handler, TRUE) == FALSE)
		ShowWarning("Unable to install the console handler!\n");
}
#endif

/*======================================
 *	CORE : Signal Sub Function
 *--------------------------------------*/
static void sig_proc(int sn)
{
	static int is_called = 0;

	switch (sn)
	{
	case SIGINT:
	case SIGTERM:
		if (++is_called > 3)
			exit(EXIT_SUCCESS);
		if (shutdown_callback != NULL)
			shutdown_callback();
		else
			runflag = CORE_ST_STOP;  // auto-shutdown
		break;

	case SIGSEGV:
	case SIGFPE:
		do_abort();
		// Pass the signal to the system's default handler
		compat_signal(sn, SIG_DFL);
		raise(sn);
		break;

#ifndef _WIN32
	case SIGXFSZ:
		// ignore and allow it to set errno to EFBIG
		ShowWarning("Max file size reached!\n");
		//run_flag = 0;	// should we quit?
		break;

	case SIGPIPE:
		//ShowInfo ("Broken pipe found... closing socket\n");	// set to eof in socket.c
		break;  // does nothing here
#endif
	}
}

void signals_init(void)
{
	compat_signal(SIGTERM, sig_proc);
	compat_signal(SIGINT, sig_proc);
#ifndef _DEBUG // need unhandled exceptions to debug on Windows
	compat_signal(SIGSEGV, sig_proc);
	compat_signal(SIGFPE, sig_proc);
#endif
#ifndef _WIN32
	compat_signal(SIGILL, SIG_DFL);
	compat_signal(SIGXFSZ, sig_proc);
	compat_signal(SIGPIPE, sig_proc);
	compat_signal(SIGBUS, SIG_DFL);
	compat_signal(SIGTRAP, SIG_DFL);
#endif
}
#endif

// GIT path
#define GIT_ORIGIN    "refs/remotes/origin/master"

// Grabs the hash from the last time the user updated their working copy (last pull)
const char* get_git_hash(void)
{
	static std::string GitHash;

	if (!GitHash.empty())
		return GitHash.c_str();

	std::ifstream file(".git/" GIT_ORIGIN);

	if (file.is_open()) {
		std::getline(file, GitHash);
	} else {
		GitHash = UNKNOWN_VERSION;
	}
	file.close();

	return GitHash.c_str();
}

/*======================================
 *	CORE : Display title
 *  ASCII By CalciumKid 1/12/2011
 *--------------------------------------*/
static void display_title(void)
{
	const char* git = get_git_hash();

	ShowMessage("\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "                                                                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "       " CL_BT_WHITE "            rAthena Development Team presents                  " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "                 ___   __  __                                    " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "           _____/   | / /_/ /_  ___  ____  ____ _                " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "          / ___/ /| |/ __/ __ \\/ _ \\/ __ \\/ __ `/                " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "         / /  / ___ / /_/ / / /  __/ / / / /_/ /                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "        /_/  /_/  |_\\__/_/ /_/\\___/_/ /_/\\__,_/                  " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "                                                                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "       " CL_GREEN "              http://rathena.org/board/                        " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "       " CL_GREEN "                  Project rAthena++                           " CL_PASS "" CL_CLL "" CL_NORMAL "\n");
	ShowMessage("" CL_PASS "     " CL_BOLD "                                                                 " CL_PASS "" CL_CLL "" CL_NORMAL "\n");

	if (git[0] != UNKNOWN_VERSION)
		ShowInfo("Git Hash: '" CL_WHITE "%s" CL_RESET "'\n", git);
	ShowInfo("Server compiled for packet version %d.\n", PACKETVER);
}

// Warning if executed as superuser (root)
void usercheck(void)
{
#ifndef _WIN32
	if (geteuid() == 0) {
		ShowWarning("You are running rAthena with root privileges, it is not necessary.\n");
	}
#endif
}

/*======================================
 *	CORE : MAINROUTINE
 *--------------------------------------*/
int main(int argc, char** argv)
{
	{       // initialize program arguments
		char* p1;
		if ((p1 = strrchr(argv[0], '/')) != NULL || (p1 = strrchr(argv[0], '\\')) != NULL) {
			char*  pwd = NULL; //path working directory
			size_t n   = 0;
			SERVER_NAME = ++p1;
			n           = p1 - argv[0]; //calc dir name len
			pwd         = safestrncpy((char*)malloc(n + 1), argv[0], n);
			if (chdir(pwd) != 0)
				ShowError("Couldn't change working directory to %s for %s, runtime will probably fail", pwd, SERVER_NAME);
			free(pwd);
		} else {
			// On Windows the .bat files have the executeable names as parameters without any path seperator [Lemongrass]
			SERVER_NAME = argv[0];
		}
	}

	malloc_init(); // needed for Show* in display_title() [FlavioJS]

#ifdef MINICORE        // minimalist Core
	display_title();
	usercheck();
	do_init(argc, argv);
	do_final();
#else   // not MINICORE
	set_server_type();
	display_title();
	usercheck();

	Sql_Init();
	rathread_init();
	mempool_init();
	db_init();
	signals_init();

#ifdef _WIN32
	cevents_init();
#endif

	timer_init();
	socket_init();

	do_init(argc, argv);

	// Main runtime cycle
	while (runflag != CORE_ST_STOP)
	{
		int next = do_timer(gettick_nocache());
		do_sockets(next);
	}

	do_final();

	timer_final();
	socket_final();
	db_final();
	mempool_final();
	rathread_final();
	ers_final();
#endif

	malloc_final();

#if defined (BUILDBOT)
	if (buildbotflag) {
		exit(EXIT_FAILURE);
	}
#endif

	return 0;
} // main
