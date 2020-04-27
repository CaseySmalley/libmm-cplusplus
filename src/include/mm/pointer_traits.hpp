#ifndef POINTER_TRAITS_HPP
#define POINTER_TRAITS_HPP
#include "mm/type_traits.hpp"

namespace mm {
	namespace pointer_traits_detail {
		template <class Ptr> typename Ptr::element_type element_type(int);
		template <class Ptr> first_template_arg_t<Ptr> element_type(long);

		template <class Ptr> typename Ptr::difference_type difference_type(int);
		template <class Ptr> ptrdiff_t difference_type(long);

		template <class Ptr,class U> typename Ptr::template rebind<U> rebind(int);
		template <class Ptr,class U> rebind_first_template_t<Ptr,U> rebind(long);
	}

	template <class Ptr>
	struct pointer_traits {
		using pointer = Ptr;
		using element_type = decltype(pointer_traits_detail::element_type<Ptr>(0));
		using difference_type = decltype(pointer_traits_detail::difference_type<Ptr>(0));
		template <class U> using rebind = decltype(pointer_traits_detail::rebind<Ptr,U>(0));

		static pointer pointer_to(element_type& e) { return Ptr::pointer_to(e); }
	};

	template <class T>
	struct pointer_traits<T*> {
		using pointer = T*;
		using element_type = T;
		using difference_type = ptrdiff_t;
		template <class U> using rebind = U*;

		static pointer pointer_to(element_type& e) { return &e; };
	};
}

#endif
