#ifndef MM_UTIL_HPP
#define MM_UTIL_HPP
#include "mm/type_traits.hpp"

namespace mm {
	template <class T> constexpr typename remove_reference_t<T>&& move(T&& t) { return static_cast<typename remove_reference_t<T>&&>(t); }
	template <class T> constexpr T&& forward(typename remove_reference_t<T>& t) { return static_cast<T&&>(t); }
	template <class T> constexpr T&& forward(typename remove_reference_t<T>&& t) { return static_cast<T&&>(t); }
}

#endif
