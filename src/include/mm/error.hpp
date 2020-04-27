#ifndef MM_ERROR_HPP
#define MM_ERROR_HPP
#include "mm/utility.hpp"

namespace mm {
	enum class error {
		ERROR_GENERAL = 1,
		ERROR_FAILED_ALLOC,
		ERROR_UNITIALIZED_OPTIONAL
	};

	const char *error_msg[] = {
		[ERROR_GENERAL] = "",
		[ERROR_FAILED_ALLOC] = "failed to allocate memory",
		[ERROR_UNITIALIZED_OPTIONAL] = "tried to accessed unitialized optional"
	};
}

#endif
