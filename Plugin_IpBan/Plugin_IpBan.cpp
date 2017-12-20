// Plugin_IpBan.cpp : Defines the exported functions for the DLL application.
//

#include "Plugin_IpBan.h"
#include <iostream>
#include <fstream>

// Create Object
 Plugin_IpBan_API rA::login::IpBanIF* _cdecl CreateIpBanObj() {
    return new rAIpBan();
}

void rAIpBan::ipban_init() 
{
	std::cout << "Entering " << __PRETTY_FUNCTION__ << "\n";
	std::ofstream lOfs;
	lOfs.open( "test.txt" );
}

void rAIpBan::ipban_final() 
{
	std::cout << "Entering " << __PRETTY_FUNCTION__ << "\n";
}

bool rAIpBan::ipban_config_read(const char* key, const char* value) 
{
	std::cout << "Entering " << __PRETTY_FUNCTION__ << "\n";
	return true;
}

void rAIpBan::ipban_log(uint32 ip) 
{
	std::cout << "Entering " << __PRETTY_FUNCTION__ << "\n";
}

bool rAIpBan::ipban_check(uint32 ip) 
{
	std::cout << "Entering " << __PRETTY_FUNCTION__ << "\n";
	return false;
}

int rAIpBan::ipban_cleanup(int tid, unsigned int tick, int id, intptr_t data) 
{
	std::cout << "Entering " << __PRETTY_FUNCTION__ << "\n";
	return 0;
}

