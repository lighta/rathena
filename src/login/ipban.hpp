/**
 * @file ipban.h
 * Module purpose is to read configuration for login-server and handle accounts,
 *  and also to synchronize all login interfaces: loginchrif, loginclif, logincnslif.
 * Licensed under GNU GPL.
 *  For more information, see LICENCE in the main folder.
 * @author Athena Dev Teams < r15k
 * @author rAthena Dev Team
 */

#pragma once
#ifndef _IPBAN_HPP_
#define _IPBAN_HPP_

#include <memory>
#include <map>
#include <iostream>
#include "../common/cbasetypes.h"
#include "Windows.h"

namespace rA 
{
	namespace login 
	{
		struct IpBanIF 
		{
			//virtual size_t mGetID() = 0;
				/**
			 * Initialize the module.
			 * Launched at login-serv start, create db or other long scope variable here.
			 */
			virtual void ipban_init(void)  = 0;

			/**
			 * Destroy the module.
			 * Launched at login-serv end, cleanup db connection or other thing here.
			 */
			virtual void ipban_final(void)  = 0;

				/**
			 * Read configuration options.
			 * @param key: config keyword
			 * @param value: config value for keyword
			 * @return true if successful, false if config not complete or server already running
			 */
			virtual bool ipban_config_read(const char* key, const char* value)  = 0;

			/**
			 * Log a failed attempt.
			 *  Also bans the user if too many failed attempts are made.
			 * @param ip: ipv4 ip to record the failure
			 */
			virtual void ipban_log(uint32 ip) = 0;

			/**
			 * Check if ip is in the active bans list.
			 * @param ip: ipv4 ip to check if ban
			 * @return true if found or error, false if not in list
			 */
			virtual bool ipban_check(uint32 ip) = 0;
		};

		// Function Pointer Declaration of IpBan concrete implementation() [Entry Point Function]
		typedef IpBanIF* (*CREATE_IpBan) ();

		class IpBanManager {
		private:
			struct sDllEntry { HINSTANCE aDSOHandle; size_t aListenerIndex; };
			std::map<size_t, std::shared_ptr<IpBanIF>> aManagedLister; //those are the one we might need to release
			std::map<size_t, IpBanIF*> aListeners;
			std::map < std::string, sDllEntry > aDlls; 

			IpBanManager()
				: aListeners()
				, aDlls()
			{}
			~IpBanManager()
			{
				for ( const auto& lCur : aDlls )
					mUnLoadDll(lCur.first);
				for ( const auto& lCur : aListeners )
					mDettachListener(lCur.first);
			}
		public:
			static IpBanManager& smGetInstance() 
			{ 
				static IpBanManager lInstance;
				return lInstance;
			}
			size_t mAttachListener( IpBanIF* pListener, size_t pIndex = -1 ) 
			{
				size_t lIndex = pIndex;
				if ( lIndex == -1 )
					lIndex = aListeners.size();
				aListeners[lIndex] = pListener; //index might be taken
				return lIndex;
			}
			size_t mAttachListener( std::shared_ptr<IpBanIF> pListener, size_t pIndex = -1 )
			{
				size_t lIndex = mAttachListener(pListener.get(),pIndex);
				aManagedLister[lIndex] = pListener;
				return lIndex;
			}

			void mDettachListener( size_t pIndex ) 
			{
				aListeners.erase(pIndex);
			}

			bool mLoadDll( const std::string& pDllpath ) {
				if ( aDlls.find( pDllpath ) != aDlls.end() ) //already loaded
					return false;
				 sDllEntry lEntry;
				 lEntry.aDSOHandle = LoadLibrary( pDllpath.c_str() );
				 if (lEntry.aDSOHandle == nullptr) {
					std::cout << "Failed to load library. path="<< pDllpath.c_str() <<"\n";
					return false;
				 }
				 CREATE_IpBan pEntryFunction = (CREATE_IpBan)GetProcAddress(lEntry.aDSOHandle,"CreateIpBanObj");
				 rA::login::IpBanIF* pIpBan = pEntryFunction();
				 lEntry.aListenerIndex = mAttachListener(pIpBan);
				 return true;
			}

			void mUnLoadDll( const std::string& pDllpath ) {
				if ( aDlls.find( pDllpath ) != aDlls.end() ) {
					mDettachListener(aDlls.at( pDllpath ).aListenerIndex);
					FreeLibrary( aDlls.at(pDllpath).aDSOHandle );
					aDlls.erase(pDllpath);
				}
			}

			void mForeachInit(){
				for ( const auto& lCur : aListeners )
				{
					lCur.second->ipban_init();
				}
			}
			void mForeachFinal(){
				for ( const auto& lCur : aListeners )
				{
					lCur.second->ipban_final();
				}
			}
			void mForeachConfigRead(const char* key, const char* value){
				for ( const auto& lCur : aListeners )
				{
					lCur.second->ipban_config_read(key,value);
				}
			}
			void mForeachLog(uint32 ip){
				for ( const auto& lCur : aListeners )
				{
					lCur.second->ipban_log(ip);
				}
			}
			bool mForeachCheck(uint32 ipl){
				bool lActive = false;
				for ( const auto& lCur : aListeners )
				{
					lActive = lCur.second->ipban_check(ipl);
					if ( lActive ) return true;
				}
				return lActive;
			}
		};

		//the default rA impl
		class IpBan 
			: public IpBanIF
		{
		public:
			void ipban_init(void);
			void ipban_final(void);
			bool ipban_config_read(const char* key, const char* value);
			void ipban_log(uint32 ip);
			bool ipban_check(uint32 ip);

			static int ipban_cleanup(int tid, unsigned int tick, int id, intptr_t data);
		};
	}
}

#endif /* _IPBAN_HPP_ */
