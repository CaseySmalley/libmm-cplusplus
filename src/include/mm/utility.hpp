#ifndef MM_UTIL_HPP
#define MM_UTIL_HPP
#include "mm/type_traits.hpp"

namespace mm {
	template <class T> constexpr remove_reference_t<T>&& move(T&& t) { return static_cast<remove_reference_t<T>&&>(t); }

	template <class T> constexpr T&& forward(remove_reference_t<T>& t) { return static_cast<T&&>(t); }
	template <class T> constexpr T&& forward(remove_reference_t<T>&& t) { return static_cast<T&&>(t); }

	template <class T> add_rvalue_reference_t<T> declval();

	template <class T>
	void swap(T& lhs,T& rhs) {
		T tmp(move(lhs));
		lhs = move(rhs);
		rhs = move(tmp);
	}

	template <class T>
	T* address_of(T& t) {
		return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(t)));
	}

	template <class T,class... Args>
	T* construct_at(T* p,Args&&... args) {
		return static_cast<T*>(new(static_cast<void*>(p)) T(forward<Args>(args)...));
	}

	template <class T>
	void destroy_at(T* p) {
		p->~T();
	}
}

#endif
