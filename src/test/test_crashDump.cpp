#include "test_crashDump.hpp"
#include <chrono>
#include <thread>

#include "../common/showmsg.h"
#include "../common/core.h"

namespace ra 
{
  namespace unit_tests
  {
	void crashDump_createDump()
    {
		CrashDumper ldump;
		ldump.Backtrace();
	}
	
	void crashDump_doCrash()
    {
		//this should auto generate a dump
		int *foo = (int*)-1; // make a bad pointer
		printf("%d\n", *foo);       // causes segfault
	}
	
	void test_crashDump()
    {
      ShowStatus( "Entering test_crashDump\n" );
      crashDump_createDump(); //checking if the generator work
      crashDump_doCrash(); //checking if the hook is attach correctly
	}
  }
}

/* Expected result
[Status]: Testing test_thread_spinlock
*/
