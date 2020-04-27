#ifndef MM_LIMITS_HPP
#define MM_LIMITS_HPP
#include <limits.h>
#include <float.h>
#include "mm/common.hpp"
#define DECLARE_INT_LIMITS(type,_min,_max)\
	template <>\
	struct numeric_limits<type> {\
		static constexpr type lowest = _min;\
		static constexpr type min = _min;\
		static constexpr type max = _max;\
	}

#define DECLARE_FLOAT_LIMITS(type,_min,_max,_epsilon)\
	template<>\
	struct numeric_limits<type> {\
		static constexpr type lowest = _min;\
		static constexpr type min = -_max;\
		static constexpr type max = _max;\
		static constexpr type epsilon = _epsilon;\
	}

namespace mm {
	template <class T> struct numeric_limits { TRAIT_ERROR_MSG(T,"tried to use mm::numeric_limits with non specialised type"); };

	DECLARE_INT_LIMITS(bool,false,true);
	DECLARE_INT_LIMITS(char,CHAR_MIN,CHAR_MAX);
	DECLARE_INT_LIMITS(wchar_t,WCHAR_MIN,WCHAR_MAX);
	DECLARE_INT_LIMITS(char16_t,0,UINT_LEAST16_MAX);
	DECLARE_INT_LIMITS(char32_t,0,UINT_LEAST32_MAX);
	DECLARE_INT_LIMITS(signed char,SCHAR_MIN,SCHAR_MAX);
	DECLARE_INT_LIMITS(unsigned char,0,UCHAR_MAX);
	DECLARE_INT_LIMITS(short,SHRT_MIN,SHRT_MAX);
	DECLARE_INT_LIMITS(unsigned short,0,USHRT_MAX);
	DECLARE_INT_LIMITS(int,INT_MIN,INT_MAX);
	DECLARE_INT_LIMITS(unsigned int,0,UINT_MAX);
	DECLARE_INT_LIMITS(long,LONG_MIN,LONG_MAX);
	DECLARE_INT_LIMITS(unsigned long,0,ULONG_MAX);
	DECLARE_INT_LIMITS(long long,LLONG_MIN,LLONG_MAX);
	DECLARE_INT_LIMITS(unsigned long long,0,ULLONG_MAX);
	DECLARE_FLOAT_LIMITS(float,FLT_MIN,FLT_MAX,FLT_EPSILON);
	DECLARE_FLOAT_LIMITS(double,DBL_MIN,DBL_MAX,DBL_EPSILON);
}

#undef DECLARE_INT_LIMITS
#undef DECLARE_FLOAT_LIMITS
#endif
