/* 
 * File:   c_rAString.h
 * Author: lighta
 *
 * Created on January 15, 2017, 12:14 PM
 */

#ifndef C_RASTRING_H
#define	C_RASTRING_H
#include <string>

namespace ra {
    namespace common_new {
        class c_rAString {
        public:
            c_rAString();
            c_rAString(const c_rAString& orig);
            virtual ~c_rAString();
        private:
        public:
            static bool is_valid_email(const std::string& email);
        };
    }
}
#endif	/* C_RASTRING_H */

