/*
 * File:   c_rAString.cpp
 * Author: lighta
 *
 * Created on January 15, 2017, 12:14 PM
 */

#include "c_rAString.h"
#include <regex>

namespace ra {
namespace common_new {
c_rAString::c_rAString()
{
}


c_rAString::c_rAString(const c_rAString &orig)
{
}


c_rAString::~c_rAString()
{
}


//meant to replace e_mail_check, (I didn't check RFC but did a little regex)
bool c_rAString::is_valid_email(const std::string &email)
{
	static const std::regex pattern("((\\w+)(\\.|_))*(\\w*)@(\\w+)(\\.(\\w+))+");

	return std::regex_match(email, pattern);
}
}
}