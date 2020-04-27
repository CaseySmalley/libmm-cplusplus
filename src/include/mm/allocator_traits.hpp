#ifndef MM_ALLOCATOR_TRAITS
#define MM_ALLOCATOR_TRAITS
#include "mm/utility.hpp"
#include "mm/limits.hpp"
#include "mm/pointer_traits.hpp"

namespace mm {
	namespace allocator_traits_detail {
		template <class Alloc> typename Alloc::pointer pointer(int);
		template <class Alloc> typename Alloc::value_type* pointer(long);

		template <class Alloc,class Ptr> typename Alloc::const_pointer const_pointer(int);
		template <class Alloc,class Ptr> typename pointer_traits<Ptr>::template rebind<const typename Alloc::value_type> const_pointer(long);

		template <class Alloc,class Ptr> typename Alloc::void_pointer void_pointer(int);
		template <class Alloc,class Ptr> typename pointer_traits<Ptr>::template rebind<void> void_pointer(long);

		template <class Alloc,class Ptr> typename Alloc::const_void_pointer const_void_pointer(int);
		template <class Alloc,class Ptr> typename pointer_traits<Ptr>::template rebind<const void> const_void_pointer(long);

		template <class Alloc,class Ptr> typename Alloc::difference_type difference_type(int);
		template <class Alloc,class Ptr> typename pointer_traits<Ptr>::difference_type difference_type(long);

		template <class Alloc,class Diff_type> typename Alloc::size_type size_type(int);
		template <class Alloc,class Diff_type> make_unsigned_t<Diff_type> size_type(long);

		template <class Alloc> typename Alloc::propagate_on_container_copy_assignment pocca(int);
		template <class Alloc> false_t pocca(long);

		template <class Alloc> typename Alloc::propagate_on_container_move_assignment pocma(int);
		template <class Alloc> false_t pocma(long);

		template <class Alloc> typename Alloc::is_always_equal is_always_equal(int);
		template <class Alloc> false_t is_always_equal(long);

		template <class Alloc,class U> typename Alloc::template rebind<U> rebind(int);
		template <class Alloc,class U> rebind_first_template_t<Alloc,U> rebind(long);

		template <class Alloc,class Size,class Ptr,class CVptr>
		auto allocate(Alloc& a,Size n,CVptr hint,int) -> decltype(a.allocate(n,hint)) { return a.allocate(n,hint); }
		
		template <class Alloc,class Size,class Ptr,class CVptr>
		auto allocate(Alloc& a,Size n,CVptr hint,long) -> decltype(a.allocate(n)) { return a.allocate(n); }

		template <class Alloc,class T,class... Args>
		auto construct(int,Alloc& a,T* p,Args&&... args) -> decltype(a.construct(p,forward<Args>(args)...)) { a.construct(p,forward<Args...>(args)...); }

		template <class Alloc,class T,class... Args>
		void construct(long,Alloc& a,T* p,Args&&... args) { construct_at(p,forward<Args>(args)...); }

		template <class Alloc,class T>
		auto destroy(Alloc& a,T* p,int) -> decltype(a.destroy(p)) { a.destroy(p); }

		template <class Alloc,class T>
		void destroy(Alloc& a,T* p,long) { destroy_at(p); }

		template <class Alloc,class Value,class Size>
		auto max_size(const Alloc& a) -> decltype(a.max_size()) { return a.max_size(); }

		template <class Alloc,class Value,class Size>
		Size max_size(const Alloc& a) { return numeric_limits<Size>::max / sizeof(Value); }

		template <class Alloc>
		auto soccc(const Alloc& a,int) -> decltype(a.select_on_container_copy_construct()) { return a.select_on_container_copy_construct(); }

		template <class Alloc>
		Alloc soccc(const Alloc& a,long) { return a; }
	}

	template <class Alloc>
	struct allocator_traits {
		using allocator_type = Alloc;
		using value_type = typename Alloc::value_type;
		using pointer = decltype(allocator_traits_detail::pointer<Alloc>(0));
		using const_pointer = decltype(allocator_traits_detail::const_pointer<Alloc,pointer>(0));
		using void_pointer = decltype(allocator_traits_detail::void_pointer<Alloc,pointer>(0));
		using const_void_pointer = decltype(allocator_traits_detail::const_void_pointer<Alloc,pointer>(0));
		using difference_type = decltype(allocator_traits_detail::difference_type<Alloc,pointer>(0));
		using size_type = decltype(allocator_traits_detail::size_type<Alloc,difference_type>(0));

		using propagate_on_container_copy_assignment = decltype(allocator_traits_detail::pocca<Alloc>(0));
		using propagate_on_container_move_assignment = decltype(allocator_traits_detail::pocma<Alloc>(0));
		using is_always_equal = decltype(allocator_traits_detail::is_always_equal<Alloc>(0));

		template <class U> using rebind_alloc = decltype(allocator_traits_detail::rebind<Alloc,U>(0));
		template <class U> using rebind_traits = allocator_traits<rebind_alloc<U>>;

		static pointer allocate(Alloc& a,size_type n) {
			return a.allocate(n);
		}

		static pointer allocate(Alloc& a,size_type n,const_void_pointer hint) {
			return allocator_traits_detail::allocate<Alloc,size_type,pointer,const_void_pointer>(a,n,hint,0);
		}

		static void deallocate(Alloc& a,pointer p,size_type n) {
			return a.deallocate(p,n);
		}

		template <class T,class... Args>
		static void construct(Alloc& a,T* p,Args&&... args) {
			return allocator_traits_detail::construct<Alloc,T,Args...>(0,a,p,forward<Args>(args)...);
		}

		template <class T>
		static void destroy(Alloc& a,T* p) {
			allocator_traits_detail::destroy<Alloc,T>(a,p,0);
		}

		static size_type max_size(const Alloc& a) {
			return allocator_traits_detail::max_size<Alloc,value_type,size_type>(0,a);
		}

		static Alloc select_on_container_copy_construction(const Alloc& a) {
			return allocator_traits_detail::soccc(a,0);
		}
	};
}

#endif
