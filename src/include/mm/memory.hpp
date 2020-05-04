#ifndef MM_MEMORY_HPP
#define MM_MEMORY_HPP
#include "mm/utility.hpp"
#include "mm/limits.hpp"

namespace mm {
	namespace detail {
		template <class Ptr> typename Ptr::element_type ptr_element_type(int);
		template <class Ptr> mm::first_template_parameter_t<Ptr> ptr_element_type(...);
		
		template <class Ptr> typename Ptr::difference_type ptr_difference_type(int);
		template <class Ptr> mm::ptrdiff_t ptr_difference_type(...);
		
		template <class Ptr,class U> typename Ptr::template ptr_rebind<U> rebind(int);
		template <class Ptr,class U> mm::rebind_t<Ptr,U> ptr_rebind(...);
	}

	template <class Ptr>
	struct pointer_traits {
		using pointer = Ptr;
		using element_type = decltype(detail::ptr_element_type<Ptr>(0));
		using difference_type = decltype(detail::ptr_difference_type<Ptr>(0));

		template <class U> 
		using rebind = decltype(detail::ptr_rebind<Ptr,U>(0));

		static pointer pointer_to(element_type& e) {
			return Ptr::pointer_to(e);
		}
	};

	template <class T>
	struct pointer_traits<T*> {
		using pointer = T*;
		using element_type = T;
		using difference_type = mm::ptrdiff_t;

		template <class U>
		using rebind = U*;

		static pointer pointer_to(element_type& e) {
			return mm::address_of(e);
		}
	};

	namespace detail {
		template <class Alloc> typename Alloc::pointer alloc_traits_pointer(int);
		template <class Alloc> typename Alloc::value_type* alloc_traits_pointer(...);

		template <class Alloc,class Ptr> typename Alloc::const_pointer alloc_traits_const_pointer(int);
		template <class Alloc,class Ptr> typename mm::pointer_traits<Ptr>::template rebind<const typename Alloc::value_type> alloc_traits_const_pointer(...);

		template <class Alloc,class Ptr> typename Alloc::void_pointer alloc_traits_void_pointer(int);
		template <class Alloc,class Ptr> typename mm::pointer_traits<Ptr>::template rebind<void> alloc_traits_void_pointer(...);

		template <class Alloc,class Ptr> typename Alloc::const_void_pointer alloc_traits_const_void_pointer(int);
		template <class Alloc,class Ptr> typename mm::pointer_traits<Ptr>::template rebind<const void> alloc_traits_const_void_pointer(...);

		template <class Alloc,class Ptr> typename Alloc::difference_type alloc_traits_difference_type(int);
		template <class Alloc,class Ptr> typename mm::pointer_traits<Ptr>::difference_type alloc_traits_difference_type(...);

		template <class Alloc,class Ptr,class Diff> typename Alloc::size_type alloc_traits_size_type(int);
		template <class Alloc,class Ptr,class Diff> mm::make_unsigned_t<Diff> alloc_traits_size_type(...);

		template <class Alloc> typename Alloc::propagate_on_container_copy_assignment alloc_traits_pocca(int);
		template <class Alloc> mm::false_t alloc_traits_pocca(...);

		template <class Alloc> typename Alloc::propagate_on_container_move_assignment alloc_traits_pocma(int);
		template <class Alloc> mm::false_t alloc_traits_pocma(...);

		template <class Alloc> typename Alloc::is_always_equal alloc_traits_is_always_equal(int);
		template <class Alloc> typename mm::is_empty<Alloc>::type alloc_traits_is_always_equal(...);

		template <class Alloc,class T> typename Alloc::template rebind<T>::other alloc_traits_rebind_alloc(int);
		template <class Alloc,class T> mm::rebind_t<Alloc,T> alloc_traits_rebind_alloc(...);

		template <class Alloc,class Ptr,class CVPtr,class Size>
		auto alloc_traits_allocate(Alloc& a,Size n,CVPtr hint) -> decltype(a.allocate(a,hint),Ptr()) {
			return a.allocate(n,hint);
		}

		template <class Alloc,class Ptr,class CVPtr,class Size>
		auto alloc_traits_allocate(Alloc& a,Size n,CVPtr hint) -> Ptr {
			return a.allocate(n);
		}

		template <class Alloc,class T,class... Args>
		auto alloc_traits_construct(Alloc& a,T* p,Args&&... args) -> decltype(a.construct(p,mm::forward<Args>(args)...),void()) {
			a.construct(p,mm::forward<Args>(args)...);
		}

		template <class Alloc,class T,class... Args>
		auto alloc_traits_construct(Alloc& a,T* p,Args&&... args) -> void {
			mm::construct_at(p,mm::forward<Args>(args)...);
		}

		template <class Alloc,class T>
		auto alloc_traits_destroy(Alloc& a,T* p) -> decltype(a.destroy(p),void()) {
			a.destroy(p);
		}

		template <class Alloc,class T>
		auto alloc_traits_destroy(Alloc& a,T* p) -> void {
			mm::destroy_at(p);
		}

		template <class Alloc,class Value,class Size>
		auto alloc_traits_max_size(const Alloc& a) -> decltype(a.max_size(),Size()) {
			return a.max_size();
		}

		template <class Alloc,class Value,class Size>
		auto alloc_traits_max_size(const Alloc& a) -> Size {
			return mm::numeric_limits<Size>::max / sizeof(Value);
		}

		template <class Alloc>
		auto alloc_traits_soccc(const Alloc& a) -> decltype(a.select_on_container_copy_construction(),Alloc()) {
			return a.select_on_container_copy_construction();
		}

		template <class Alloc>
		auto alloc_traits_soccc(const Alloc& a) -> Alloc {
			return a;
		}
	}

	template <class Alloc>
	struct allocator_traits {
		using allocator_type = Alloc;
		using value_type = typename Alloc::value_type;
		using pointer = decltype(detail::alloc_traits_pointer<Alloc>(0));
		using const_pointer = decltype(detail::alloc_traits_const_pointer<Alloc,pointer>(0));
		using void_pointer = decltype(detail::alloc_traits_void_pointer<Alloc,pointer>(0));
		using const_void_pointer = decltype(detail::alloc_traits_const_void_pointer<Alloc,pointer>(0));
		using difference_type = decltype(detail::alloc_traits_difference_type<Alloc,pointer>(0));
		using size_type = decltype(detail::alloc_traits_size_type<Alloc,pointer,difference_type>(0));
		using propagate_on_container_copy_assignment = decltype(detail::alloc_traits_pocca<Alloc>(0));
		using propagate_on_container_move_assignment = decltype(detail::alloc_traits_pocma<Alloc>(0));
		using is_always_equal = decltype(detail::alloc_traits_is_always_equal<Alloc>(0));
		
		template <class T> using rebind_alloc = decltype(detail::alloc_traits_rebind_alloc<Alloc,T>(0));
		template <class T> using rebind_traits = mm::allocator_traits< rebind_alloc<T> >;

		static pointer allocate(Alloc& a,size_type n) {
			return a.allocate(n);
		}

		static pointer allocate(Alloc& a,size_type n,const_void_pointer hint) {
			return detail::alloc_traits_allocate<Alloc,pointer,const_void_pointer,size_type>(a,n,hint);
		}

		static void deallocate(Alloc& a,pointer p,size_type n) {
			a.deallocate(p,n);
		}

		template <class T,class... Args>
		static void construct(Alloc& a,T* p,Args&&... args) {
			detail::alloc_traits_construct<Alloc,T,Args...>(a,p,mm::forward<Args>(args)...);
		}

		template <class T>
		static void destroy(Alloc& a,T* p) {
			detail::alloc_traits_destroy<Alloc,T>(a,p);
		}

		static size_type max_size(const Alloc& a) {
			return detail::alloc_traits_max_size<Alloc,value_type,size_type>(a);
		}

		static Alloc select_on_container_copy_construction(const Alloc& a) {
			return detail::alloc_traits_soccc(a);
		}
	};

	template <class T>
	class allocator {
	public:
		using value_type = T;
		using size_type = mm::size_t;
		using difference_type = mm::ptrdiff_t;
		using propagate_on_container_move_assignment = mm::true_t;
		using is_always_equal = mm::true_t;

		allocator() = default;
		allocator(const allocator&) = default;
		allocator(allocator&&) = default;

		template <class U>
		allocator(const allocator<U>&) {}

		~allocator() = default;

		allocator& operator=(const allocator&) = default;
		allocator& operator=(allocator&&) = default;

		template <class U>
		allocator& operator=(const allocator<U>&) {}

		T* allocate(mm::size_t n) {
			return ::operator new(n);
		}

		void deallocate(T* p,mm::size_t n) {
			::operator delete(p);
		}
	};
	
	template <class T1,class T2>
	bool operator==(const allocator<T1>&,const allocator<T2>&) {
		return true;
	}

	// tag to mark container constructors that accept allocators
	struct allocator_arg_t {};
	constexpr allocator_arg_t allocator_arg;

	namespace detail {
		template <class T,class Alloc> typename mm::is_convertible< typename T::allocator_type, Alloc >::type uses_allocator_impl(int);
		template <class T,class Alloc> mm::false_t uses_allocator_impl(...);
	}

	template <class T,class Alloc> struct uses_allocator : decltype(detail::uses_allocator_impl<T,Alloc>(0)) {};
}

#endif
