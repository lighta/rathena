/*
 * File:   c_Module.h
 * Author: lighta
 *
 * Created on January 15, 2017, 1:25 PM
 */

#ifndef C_MODULE_H
#define C_MODULE_H
#include <memory>

namespace ra {
namespace common_new {
class c_Module {
public:
	c_Module();
	c_Module(const c_Module &orig);
	virtual ~c_Module();

	size_t mAttach(c_Module *);

	void mDettach(size_t);
	void virtual do_init()  = 0;
	void virtual do_final() = 0;


	void virtual update(size_t pKeyEvent) {}
	void notifyAll(size_t pKeyEvent);

private:
	struct pImpl;
	std::unique_ptr<pImpl> aPimpl;
};
}
}

#endif  /* C_MODULE_H */

