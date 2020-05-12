#ifndef MM_MEMORY_HPP
#define MM_MEMORY_HPP
#include "mm/iterator.hpp"
#include "mm/limits.hpp"

namespace mm {
	template <mm::size_t Length,mm::size_t Alignment> 
	struct aligned_storage {
		struct type {
			alignas(Alignment) mm::u8 data[Length];
		};
	};

	template <mm::size_t Length,mm::size_t Alignment>
	using aligned_storage_t = typename mm::aligned_storage<Length,Alignment>::type;

	template <mm::size_t Length,class... Ts>
	struct aligned_union {
		static constexpr mm::size_t alignment = mm::max(mm::integer_sequence<mm::size_t,alignof(Ts)...> {});

		struct type {
			alignas(alignment) mm::u8 data[mm::max(mm::integer_sequence<mm::size_t,Length,sizeof(Ts)...> {})];
		};
	};

	template <mm::size_t Length,class... Ts>
	using aligned_union_t = typename mm::aligned_union<Length,Ts...>::type;

	template <class T>
	struct alignment_of : mm::integral_constant<mm::size_t,alignof(T)> {};

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
	class default_allocator {
	public:
		using value_type = T;
		using size_type = mm::size_t;
		using difference_type = mm::ptrdiff_t;
		using propagate_on_container_move_assignment = mm::true_t;
		using is_always_equal = mm::true_t;

		default_allocator() = default;
		default_allocator(const default_allocator&) = default;
		default_allocator(default_allocator&&) = default;

		template <class U>
		default_allocator(const default_allocator<U>&) {}

		~default_allocator() = default;

		default_allocator& operator=(const default_allocator&) = default;
		default_allocator& operator=(default_allocator&&) = default;

		template <class U>
		default_allocator& operator=(const default_allocator<U>&) {}

		T* allocate(mm::size_t n) {
			return static_cast<T*>(malloc(sizeof(T) * n));
		}

		void deallocate(T* p,mm::size_t n) { printf("free %p\n",p);
			free(static_cast<void*>(p));
		}
	};
	
	template <class T1,class T2>
	bool operator==(const mm::default_allocator<T1>&,const mm::default_allocator<T2>&) {
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

	template <class T>
	struct default_delete {
		default_delete() = default;
		default_delete(const default_delete&) = default;
		default_delete(default_delete&&) = default;

		default_delete& operator=(const default_delete&) = default;
		default_delete& operator=(default_delete&&) = default;

		template <class U>
		default_delete(const mm::default_delete<U>&) {}

		void operator()(T* p) const {
			delete p;
		}
	};

	template <class T>
	struct default_delete<T[]> {
		default_delete() = default;
		default_delete(const default_delete&) = default;
		default_delete(default_delete&&) = default;
		
		template <class U>
		default_delete(const default_delete<T[]>&) {}

		void operator()(T* p) const {
			delete[] p;
		}
	};

	namespace detail {
		template <class T,class Deleter> typename mm::remove_reference_t<Deleter>::type::pointer unique_ptr_pointer(int);
		template <class T,class Deleter> T* unique_ptr_pointer(...);
	}

	template <class T,class Deleter = mm::default_delete<T>>
	class unique_ptr {
	public:
		using pointer = decltype(detail::unique_ptr_pointer<T,Deleter>(0));
		using element_type = T;
		using deleter_type = Deleter;

	private:
		pointer m_ptr;
		deleter_type m_del;

	public:
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

		constexpr unique_ptr() : m_ptr(), m_del() {}
		constexpr unique_ptr(mm::nullptr_t) : m_ptr(), m_del() {}
		explicit unique_ptr(pointer ptr) : m_ptr(ptr), m_del() {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_copy_constructible<D>::value
		    && !mm::is_reference<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,const deleter_type& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_move_constructible<D>::value
		    && !mm::is_reference<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,deleter_type&& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_lvalue_reference<D>::value
		    && !mm::is_const<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,deleter_type& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
		       mm::is_lvalue_reference<D>::value
		    && mm::is_const<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,const deleter_type& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_move_constructible<D>::value
		> = nullptr>
		unique_ptr(unique_ptr&& other) : m_ptr(other.release()), m_del(mm::move(other.get_deleter())) {}

		template <class U,class E,mm::enable_if_t<
			mm::is_convertible<typename unique_ptr<U,E>::pointer,pointer>::value
		    && !mm::is_array<U>::value
		    && ((mm::is_reference<deleter_type>::value && mm::is_same<deleter_type,E>::value)
		    || (!mm::is_reference<deleter_type>::value && mm::is_convertible<E,deleter_type>::value))
		> = nullptr>
		unique_ptr(unique_ptr<U,E>&& other) : m_ptr(other.release()), m_del(mm::forward<E>(other.get_deleter())) {}

		~unique_ptr() {
			if (m_ptr) {
				m_del(m_ptr);
			}
		}

		template <class D = deleter_type,class = mm::enable_if_t<
			mm::is_move_assignable<D>::value
		>>
		unique_ptr& operator=(unique_ptr&& other) {
			unique_ptr(other).swap(*this);
			return *this;
		}

		template <class U,class E,class = mm::enable_if_t<
			!mm::is_array<U>::value
		      && mm::is_convertible< typename unique_ptr<U,E>::pointer, pointer >::value
		      && mm::is_assignable<deleter_type,E&&>::value
		>>
		unique_ptr& operator=(unique_ptr<U,E>&& other) {
			unique_ptr(other).swap(*this);
			return *this;
		}

		unique_ptr& operator=(mm::nullptr_t) {
			reset();
			return *this;
		}

		pointer release() {
			pointer ptr = m_ptr;
			m_ptr = pointer();
			return ptr;
		}

		void reset(pointer ptr = pointer()) {
			pointer old = m_ptr;
			m_ptr = ptr;
			
			if (old) {
				m_del(old);
			}
		}
	
		void swap(unique_ptr& other) {
			mm::swap(this->m_ptr,other.m_ptr);
			mm::swap(this->m_del,other.m_del);
		}

		pointer get() const {
			return m_ptr;
		}

		deleter_type& get_deleter() {
			return m_del;
		}

		const deleter_type& get_deleter() const {
			return m_del;
		}

		explicit operator bool() const {
			return m_ptr != nullptr;
		}

		mm::add_lvalue_reference_t<T> operator*() const {
			return *m_ptr;
		}

		pointer operator->() const {
			return m_ptr;
		}
	};

	template <class T,class Deleter>
	class unique_ptr<T[],Deleter> {
	public:
		using pointer = decltype(detail::unique_ptr_pointer<T,Deleter>(0));
		using element_type = T;
		using deleter_type = Deleter;
	
	private:
		pointer m_ptr;
		deleter_type m_del;

	public:
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

		constexpr unique_ptr() : m_ptr(), m_del() {}
		constexpr unique_ptr(mm::nullptr_t) : m_ptr(), m_del() {}
		explicit unique_ptr(pointer ptr) : m_ptr(ptr), m_del() {}

		template <class U>
		explicit unique_ptr(U ptr) : m_ptr(ptr), m_del() {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_copy_constructible<D>::value
		    && !mm::is_reference<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,const deleter_type& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_move_constructible<D>::value
		    && !mm::is_reference<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,deleter_type&& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_lvalue_reference<D>::value
		    && !mm::is_const<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,deleter_type& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
		       mm::is_lvalue_reference<D>::value
		    && mm::is_const<D>::value
		> = nullptr>
		unique_ptr(pointer ptr,const deleter_type& del) : m_ptr(ptr), m_del(mm::forward<decltype(del)>(del)) {}

		template <class D = deleter_type,mm::enable_if_t<
			mm::is_move_constructible<D>::value
		> = nullptr>
		unique_ptr(unique_ptr&& other) : m_ptr(other.release()), m_del(mm::move(other.get_deleter())) {}

		~unique_ptr() {
			if (m_ptr) {
				m_del(m_ptr);
			}
		}

		template <class D = deleter_type,class = mm::enable_if_t<
			mm::is_move_assignable<D>::value
		>>
		unique_ptr& operator=(unique_ptr&& other) {
			unique_ptr(other).swap(*this);
			return *this;
		}

		template <class U,class E,class = mm::enable_if_t<
			!mm::is_array<U>::value
		      && mm::is_convertible< typename unique_ptr<U,E>::pointer, pointer >::value
		      && mm::is_assignable<deleter_type,E&&>::value
		>>
		unique_ptr& operator=(unique_ptr<U,E>&& other) {
			unique_ptr(other).swap(*this);
			return *this;
		}

		unique_ptr& operator=(mm::nullptr_t) {
			reset();
			return *this;
		}

		pointer release() {
			pointer ptr = m_ptr;
			m_ptr = pointer();
			return ptr;
		}

		void reset(pointer ptr = pointer()) {
			pointer old = m_ptr;
			m_ptr = ptr;
			
			if (old) {
				m_del(old);
			}
		}

		template<class U>
		void reset(U) = delete;

		void reset(mm::nullptr_t) {
			reset(pointer());
		}
	
		void swap(unique_ptr& other) {
			mm::swap(this->m_ptr,other.m_ptr);
			mm::swap(this->m_del,other.m_del);
		}

		pointer get() const {
			return m_ptr;
		}

		deleter_type& get_deleter() {
			return m_del;
		}

		const deleter_type& get_deleter() const {
			return m_del;
		}

		explicit operator bool() const {
			return m_ptr != nullptr;
		}

		element_type& operator[](mm::size_t i) {
			return m_ptr[i];
		}
	};

	template <class T1,class D1,class T2,class D2>
	bool operator==(const mm::unique_ptr<T1,D1>& lhs,const mm::unique_ptr<T2,D2>& rhs) {
		return lhs.get() == rhs.get();
	}

	template <class T1,class D1,class T2,class D2>
	bool operator!=(const mm::unique_ptr<T1,D1>& lhs,const mm::unique_ptr<T2,D2>& rhs) {
		return lhs.get() != rhs.get();
	}

	template <class T1,class D1,class T2,class D2>
	bool operator<(const mm::unique_ptr<T1,D1>& lhs,const mm::unique_ptr<T2,D2>& rhs) {
		return lhs.get() < rhs.get();
	}

	template <class T1,class D1,class T2,class D2>
	bool operator>(const mm::unique_ptr<T1,D1>& lhs,const mm::unique_ptr<T2,D2>& rhs) {
		return lhs.get() > rhs.get();
	}

	template <class T1,class D1,class T2,class D2>
	bool operator<=(const mm::unique_ptr<T1,D1>& lhs,const mm::unique_ptr<T2,D2>& rhs) {
		return lhs.get() <= rhs.get();
	}

	template <class T1,class D1,class T2,class D2>
	bool operator>=(const mm::unique_ptr<T1,D1>& lhs,const mm::unique_ptr<T2,D2>& rhs) {
		return lhs.get() >= rhs.get();
	}

	template <class T,class D>
	bool operator==(const unique_ptr<T,D>& ptr,mm::nullptr_t) {
		return !bool(ptr);
	}

	template <class T,class D>
	bool operator==(mm::nullptr_t,const unique_ptr<T,D>& ptr) {
		return !bool(ptr);
	}

	template <class T,class D>
	bool operator!=(const unique_ptr<T,D>& ptr,mm::nullptr_t) {
		return bool(ptr);
	}

	template <class T,class D>
	bool operator!=(mm::nullptr_t,const unique_ptr<T,D>& ptr) {
		return bool(ptr);
	}

	template <class T,class D>
	bool operator<(const unique_ptr<T,D>& ptr,mm::nullptr_t) {
		return ptr.get() < nullptr;
	}

	template <class T,class D>
	bool operator<(mm::nullptr_t,const unique_ptr<T,D>& ptr) {
		return nullptr < ptr.get();
	}

	template <class T,class D>
	bool operator>(const unique_ptr<T,D>& ptr,mm::nullptr_t) {
		return ptr.get() > nullptr;
	}

	template <class T,class D>
	bool operator>(mm::nullptr_t,const unique_ptr<T,D>& ptr) {
		return nullptr > ptr.get();
	}

	template <class T,class D>
	bool operator<=(const unique_ptr<T,D>& ptr,mm::nullptr_t) {
		return ptr.get() <= nullptr;
	}

	template <class T,class D>
	bool operator<=(mm::nullptr_t,const unique_ptr<T,D>& ptr) {
		return nullptr <= ptr.get();
	}

	template <class T,class D>
	bool operator>=(const unique_ptr<T,D>& ptr,mm::nullptr_t) {
		return ptr.get() >= nullptr;
	}

	template <class T,class D>
	bool operator>=(mm::nullptr_t,const unique_ptr<T,D>& ptr) {
		return nullptr >= ptr.get();
	}

	template <class T,class... Args>
	mm::unique_ptr<T> make_unique(Args&&... args) {
		return mm::unique_ptr<T>(new T(mm::forward<Args>(args)...));
	}

	template <class T,class D>
	void swap(mm::unique_ptr<T,D>& lhs,mm::unique_ptr<T,D>& rhs) {
		lhs.swap(rhs);
	}

	// mm::hash<mm::unique_ptr>

	template <class T>
	class weak_ptr;

	template <class T>
	class shared_ptr {
	public:
		using element_type = T;
		using weak_type = mm::weak_ptr<T>;

	private:
		class control_block {
		protected:
			mm::i32 m_ref_count;
			mm::i32 m_weak_count;
		
		public:
			control_block() : m_ref_count(1), m_weak_count(1) {}

			control_block(const control_block&) = delete;
			control_block(control_block&&) = delete;

			mm::i32 ref_count() const {
				return m_ref_count;
			}

			void inc_reference() {
				++m_ref_count;
				++m_weak_count;
			}

			void inc_weak_reference() {
				++m_weak_count;
			}

			virtual void free_element() = 0;
			virtual void free_control_block() = 0;

			void release_reference() {
				if (--m_ref_count > 0) {
					--m_weak_count;
				} else {
					free_element();

					if (m_weak_count == 0) {
						free_control_block();
					}
				}
			}

			void release_weak_reference() {
				if (--m_weak_count == 0) {
					free_control_block();
				}
			}
		};

		template <class Alloc,class Deleter>
		class control_block_ptr : public control_block {
		private:
			using element_type = T*;
			using allocator_type = Alloc;
			using deleter_type = Deleter;

			element_type m_element;
			allocator_type m_alloc;
			deleter_type m_del;
	
		public:
			control_block_ptr(element_type element,allocator_type alloc,deleter_type del) : 
				control_block(),
				m_element(element),
				m_alloc(mm::move(alloc)),
				m_del(del)
			{}

			virtual void free_element() override {
				m_del(m_element);
			}

			virtual void free_control_block() override {
				allocator_type alloc(m_alloc);
				mm::destroy_at(this);

				mm::allocator_traits<allocator_type>::deallocate(
					alloc,
					reinterpret_cast<mm::u8*>(this),
					sizeof(*this)
				);
			}
		};

		template <class Alloc>
		class control_block_inline : public control_block {
		private:
			using element_type = T;
			using allocator_type = Alloc;
			using storage_type = mm::aligned_storage_t<sizeof(element_type),mm::alignment_of<T>::value>;
			
			storage_type m_memory;
			allocator_type m_alloc;

			element_type* get_ptr() {
				return static_cast<element_type*>(
					static_cast<void*>(
						mm::address_of(m_memory)
					)
				);
			}

		public:
			template <class... Args>
			control_block_inline(allocator_type alloc,Args&&... args) :
				control_block(),
				m_alloc(mm::move(alloc))
			{
				mm::construct_at(get_ptr(),mm::forward<Args>(args)...);
			}

			virtual void free_element() override {
				mm::destroy_at(get_ptr());
			}

			virtual void free_control_block() override {
				allocator_type alloc(m_alloc);
				mm::destroy_at(this);
				mm::allocator_traits<allocator_type>::deallocate(alloc,this,sizeof(*this));
			}
		};
		
		control_block *m_control;
		element_type *m_element;

		template <class U,class Alloc,class Deleter>
		void allocate_control_block_with_ptr(U* ptr,Alloc alloc,Deleter del) {
			using allocator_type = typename mm::allocator_traits<Alloc>::template rebind_alloc<mm::u8>;
			using control_block_type = control_block_ptr<allocator_type,Deleter>;

			allocator_type internal_alloc(alloc);
			
			void* memory = mm::allocator_traits<allocator_type>::allocate(
				internal_alloc,
				sizeof(control_block_type)
			);

			if (memory) {
				m_control = mm::construct_at(
					static_cast<control_block_type*>(memory),
					ptr,
					internal_alloc,
					del
				);

				m_element = ptr;
			}
		}

	public:
		constexpr shared_ptr() = default;
		constexpr shared_ptr(mm::nullptr_t) : m_control(), m_element() {}

		template <class U,mm::enable_if_t<
			mm::is_convertible<U*,element_type*>::value
		> = nullptr>
		explicit shared_ptr(U* ptr) : m_control(), m_element() {
			allocate_control_block_with_ptr(
				ptr,
				mm::default_allocator<mm::u8>(),
				mm::default_delete<T>()
			);
		}

		template <class U,class Deleter,mm::enable_if_t<
			mm::is_convertible<U*,element_type*>::value
		     && mm::is_copy_constructible<Deleter>::value
		> = nullptr>
		shared_ptr(U* ptr,Deleter del) : m_control(), m_element() {
			alloc_control_block_with_ptr(
				ptr,
				mm::default_allocator<mm::u8>(),
				del
			);
		}

		template <class Deleter,mm::enable_if_t<
			mm::is_copy_constructible<Deleter>::value
		> = nullptr>
		shared_ptr(mm::nullptr_t,Deleter del) : m_control(), m_element() {
			alloc_control_block_with_ptr(
				nullptr,
				mm::default_allocator<mm::u8>(),
				del
			);
		}

		template <class U,class Alloc,class Deleter,mm::enable_if_t<
			mm::is_convertible<U*,element_type*>::value
		     && mm::is_copy_constructible<Deleter>::value
		> = nullptr>
		shared_ptr(U* ptr,Alloc alloc,Deleter del) : m_control(), m_element() {
			alloc_control_block_with_ptr(
				ptr,
				alloc,
				del
			);
		}

		~shared_ptr() {
			m_control->release_reference();
		}

		void swap(shared_ptr& other) {
			mm::swap(m_control,other.m_control);
			mm::swap(m_element,other.m_element);
		}
	};

	// mm::hash<mm::shared_ptr>
}

#endif
