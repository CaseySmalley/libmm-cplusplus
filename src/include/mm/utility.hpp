#ifndef MM_UTIL_HPP
#define MM_UTIL_HPP
#include "mm/type_traits.hpp"

namespace mm {
	template <class T,T... Ts>
	struct integer_sequence {
		using value_type = T;
		
		static constexpr mm::size_t size() {
			return sizeof...(Ts);
		}
	};

	namespace detail {
		template <class T,T Current,T Value,T... Ts> 
		struct int_seq_min_impl : mm::integral_constant<
			T,
			int_seq_min_impl<
				T,
				Current < Value ? Current : Value,
				Ts...
			>::value
		> {};

		template <class T,T Current,T Value>
		struct int_seq_min_impl<T,Current,Value> : mm::integral_constant<
			T,
			Current < Value ? Current : Value
		> {};
	}
	
	template <class T,T... Ts>
	constexpr T min(const mm::integer_sequence<T,Ts...>&) {
		static_assert(mm::is_integral<T>::value,"expected a sequence of integers");
		return detail::int_seq_min_impl<T,T(-1),Ts...>::value;
	}

	namespace detail {
		template <class T,T Current,T Value,T... Ts> 
		struct int_seq_max_impl : mm::integral_constant<
			T,
			int_seq_max_impl<
				T,
				(Current > Value) ? Current : Value,
				Ts...
			>::value
		> {};

		template <class T,T Current,T Value>
		struct int_seq_max_impl<T,Current,Value> : mm::integral_constant<
			T,
			(Current > Value) ? Current : Value
		> {};
	}

	template <class T,T... Ts>
	constexpr T max(const mm::integer_sequence<T,Ts...>&) {
		static_assert(mm::is_integral<T>::value,"expected a sequence of integers");
		return detail::int_seq_max_impl<T,T(0),Ts...>::value;
	}

	template <class T> mm::remove_reference_t<T>&& move(T&& t) { return static_cast<mm::remove_reference_t<T>&&>(t); }
	
	template <class T> T&& forward(mm::remove_reference_t<T>& t) { return static_cast<T&&>(t); }
	template <class T> T&& forward(mm::remove_reference_t<T>&& t) { return static_cast<T&&>(t); }

	template <class T>
	void swap(T& lhs,T& rhs) {
		T tmp(move(lhs));
		lhs = move(rhs);
		rhs = move(tmp);
	}

	template <class T,class... Args> T* construct_at(T* p,Args&&... args) { return static_cast<T*>(new(static_cast<void*>(p)) T(forward<Args>(args)...)); }
	template <class T> void destroy_at(T* p) { p->~T(); }
	template <class T> T* address_of(T& t) { return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(t))); }

	struct in_place_t { explicit in_place_t() = default; };
	constexpr in_place_t in_place {};

	template <class T> struct in_place_type_t { explicit in_place_type_t() = default; };
	template <mm::size_t I> struct in_place_index_t { explicit in_place_index_t() = default; };

}

#endif
