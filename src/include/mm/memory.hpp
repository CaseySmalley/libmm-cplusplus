#ifndef MM_MEMORY_HPP
#define MM_MEMORY_HPP
#include "mm/utility.hpp"

namespace mm {
	template <class T>
	struct default_delete {
		default_delete() = default;
		default_delete(const default_delete<T>&) = default;
		default_delete(default_delete<T>&&) = default;
		template <class U> default_delete(const default_delete<U>&) {}

		void operator()(T* p) { delete p; }
	};

	template <class T>
	struct default_delete<T[]> {
		default_delete() = default;
		default_delete(const default_delete<T[]>&) = default;
		default_delete(default_delete<T[]>&&) = default;
		template <class U> default_delete(const default_delete<U[]>&) {}

		template <class U> void operator()(U* p) { delete[] p; }
	};

	namespace unique_ptr_detail {
		template <class T,class Deleter> typename remove_reference_t<Deleter>::type::pointer pointer(int);
		template <class T,class Deleter> T* pointer(long);
	}

	template <class T,class Deleter = default_delete<T>>
	class unique_ptr {
	public:
		using pointer = decltype(unique_ptr_detail::pointer<T,Deleter>(0));
		using element_type = T;
		using deleter_type = Deleter;

	private:
		pointer m_ptr;
		deleter_type m_del;

	public:
		unique_ptr() = default;
		unique_ptr(nullptr_t) : m_ptr{nullptr}, m_del{} {}
		explicit unique_ptr(pointer ptr) : m_ptr{ptr}, m_del{} {}

		template <class U = deleter_type>
		unique_ptr(pointer ptr,enable_if_t<!is_lvalue_reference<U>::value,const U&> del) : m_ptr{ptr}, m_del{forward<decltype(del)>(del)} {}

		template <class U = deleter_type>
		unique_ptr(pointer ptr,enable_if_t<!is_lvalue_reference<U>::value,U&&> del) : m_ptr{ptr}, m_del{forward<decltype(del)>(del)} {}

		template <class U = deleter_type>
		unique_ptr(pointer ptr,enable_if_t<is_mutable_lvalue_reference<U>::value,U&> del) : m_ptr{ptr}, m_del{forward<decltype(del)>(del)} {}

		template <class U = deleter_type>
		unique_ptr(pointer ptr,enable_if_t<is_mutable_lvalue_reference<U>::value,U&&> del) : m_ptr{ptr}, m_del{forward<decltype(del)>(del)} {}

		template <class U = deleter_type>
		unique_ptr(pointer ptr,enable_if_t<is_const_lvalue_reference<U>::value,const U&> del) : m_ptr{ptr}, m_del{forward<decltype(del)>(del)} {}

		template <class U = deleter_type>
		unique_ptr(pointer ptr,enable_if_t<is_const_lvalue_reference<U>::value,const U&&> del) : m_ptr{ptr}, m_del{forward<decltype(del)>(del)} {}

		//~unique_ptr() { reset(nullptr); }

		pointer get() { return m_ptr; }
		deleter_type& get_deleter() { return m_del; }
		const deleter_type& get_deleter() const { return m_del; }
	};

	template <class T,class... Args>
	unique_ptr<T> make_unique(Args&&... args) {
		return unique_ptr<T>(new T(forward<Args>(args)...));
	}
}

#endif
