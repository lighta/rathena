#pragma once
#include "../src/login/ipban.hpp"
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the Plugin_IpBan_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// Plugin_IpBan_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef Plugin_IpBan_EXPORTS
#define Plugin_IpBan_API __declspec(dllexport)
#else
#define Plugin_IpBan_API __declspec(dllimport)
#endif

// This is an example of a class exported from the Plugin_IpBan.dll

class rAIpBan 
	: public rA::login::IpBanIF
{
public:
	void ipban_init(void);
	void ipban_final(void);
	bool ipban_config_read(const char* key, const char* value);
	void ipban_log(uint32 ip);
	bool ipban_check(uint32 ip);

	static int ipban_cleanup(int tid, unsigned int tick, int id, intptr_t data);
};

//  Factory function that will return the new object instance. (Only function
//  should be declared with DLLCALL)
extern "C" /*Important for avoiding Name decoration*/
{
    Plugin_IpBan_API rA::login::IpBanIF* _cdecl CreateIpBanObj();
};

