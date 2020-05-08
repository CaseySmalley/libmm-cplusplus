#ifndef MM_FUNCTIONAL_HPP
#define MM_FUNCTIONAL_HPP
#include "mm/utility.hpp"

namespace mm {
	// member func ptr called with obj ref
	template <class B,class T,class D,class... Args,class = mm::enable_if_t<
		mm::is_reference<D>::value
	     && mm::is_base_of< B, mm::decay_t<D> >::value
	>>
	auto invoke(T (B::*f)(Args...),D&& ref,Args&&... args) -> decltype((mm::forward<D>(ref).*f)(mm::forward<Args>(args)...)) {
		return (mm::forward<D>(ref).*f)(mm::forward<Args>(args)...);
	}

	// member func ptr called with obj ptr
	template <class B,class T,class D,class... Args,class = mm::enable_if_t<
		mm::is_pointer<D>::value
	>>
	auto invoke(T (B::*f)(Args...),D&& ptr,Args&&... args) -> decltype(((*mm::forward<D>(ptr)).*f)(mm::forward<Args>(args)...)) {
		return ((*mm::forward<D>(ptr)).*f)(mm::forward<Args>(args)...);
	}

	// member data ptr called with obj ref
	template <class B,class T,class D,class = mm::enable_if_t<
		mm::is_reference<D>::value
	     && mm::is_base_of< B, mm::decay_t<D> >::value
	>>
	auto invoke(T B::*f,D&& ref) -> decltype(mm::forward<D>(ref).*f) {
		return mm::forward<D>(ref).*f;
	}

	// member data ptr called with obj ptr
	template <class B,class T,class D,class = mm::enable_if_t<
		mm::is_pointer<D>::value
	>>
	auto invoke(T B::*f,D&& ptr) -> decltype((*mm::forward<D>(ptr)).*f) {
		return (*mm::forward<D>(ptr)).*f;
	}

	// base case (functor, lambda, function ptr)
	template <class F,class... Args>
	auto invoke(F&& f,Args&&... args) -> decltype(mm::forward<F>(f)(mm::forward<Args>(args)...)) {
		return mm::forward<F>(f)(mm::forward<Args>(args)...);
	}

	template <class>
	struct invoke_result {};

	template <class F,class... Args>
	struct invoke_result<F(Args...)> : mm::type_identity< decltype(mm::invoke(mm::declval<F>(),mm::declval<Args>()...)) > {};

	template <class F,class... Args>
	using invoke_result_t = typename mm::invoke_result<F(Args...)>::type;

	namespace detail {
		template <class T> T& reference_wrapper_not_rvalue(T& t) { return t; }
		template <class T> void reference_wrapper_not_rvalue(T&&) = delete;
	}
	
	template <class T>
	class reference_wrapper {
	public:
		using type = T;

	private:
		type* m_ptr;

	public:
		template <class U,class = decltype(
			detail::reference_wrapper_not_rvalue<T>(mm::declval<U>()),
			mm::enable_if_t< !mm::is_same< reference_wrapper, mm::remove_cvref_t<U> >::value >()
		)>
		reference_wrapper(U&& u) : m_ptr(mm::address_of(detail::reference_wrapper_not_rvalue<T>(mm::forward<U>(u)))) {}
		
		reference_wrapper(const reference_wrapper&) = default;
		reference_wrapper& operator=(const reference_wrapper&) = default;

		type& get() const {
			return *m_ptr;
		}

		operator type&() const {
			return *m_ptr;
		}

		template <class... Args>
		mm::invoke_result_t<type&,Args...> operator()(Args&&... args) const {
			return mm::invoke(get(),mm::forward<Args>(args)...);
		}
	};

	template <class T> constexpr mm::reference_wrapper<T> ref(T& t) {
		return mm::reference_wrapper<T>(t);
	}

	template <class T> constexpr mm::reference_wrapper<T> cref(const T& t) {
		return mm::reference_wrapper<T>(t);
	}
}

#endif
