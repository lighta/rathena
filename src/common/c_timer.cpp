/*
 * File:   timer.cpp
 * Author: lighta
 *
 * Created on January 15, 2017, 11:50 AM
 */

#include "c_timer.h"
#include <ctime> //strftime

namespace ra {
namespace common_new {
c_timer::c_timer()
{
}

c_timer::c_timer(const c_timer& orig)
{
}

c_timer::~c_timer()
{
}

std::string c_timer::timestamp2string(time_t timestamp, const char* format)
{
	char str[24];

	strftime(str, 24, format, localtime(&timestamp));
	return str;
}
}
}