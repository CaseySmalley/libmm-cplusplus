#ifndef MM_ERROR_HPP
#define MM_ERROR_HPP
#include "mm/utility.hpp"

namespace mm {
	namespace error {
		// errors are functors, that can resolve their state & carry a string message
		// make_error<T>(...) <-- use to construct error objects
		// call handle<T>(error) to fix stuff (plan to abuse template deduction here)
		// the caller is assumed to be responsible for cleaning stuff up
	}
}

#endif
