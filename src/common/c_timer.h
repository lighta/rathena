/*
 * File:   timer.h
 * Author: lighta
 *
 * Created on January 15, 2017, 11:50 AM
 */

#ifndef TIMER_H
#define TIMER_H

#include <string>

namespace ra {
namespace common_new {
class c_timer {
public:
	c_timer();
	c_timer(const c_timer &orig);
	virtual ~c_timer();
private:
public:
	static std::string timestamp2string(time_t timestamp, const char *format);
};
}
}
#endif  /* TIMER_H */

