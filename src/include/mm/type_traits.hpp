#ifndef MM_TYPE_TRAITS_HPP
#define MM_TYPE_TRAITS_HPP
#include "mm/common.hpp"

namespace mm {
	template <class T,T v>
	struct integral_constant {
		using value_type = T;
		using type = integral_constant;
		
		static constexpr value_type value = v;
		constexpr operator value_type() { return value; }
	};

	template <class T>
	struct type_identity {
		using type = T;
	};

	template <class...> using void_t = void;

	#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
	template <class T> struct is_union : mm::integral_constant<bool,__is_union(T)> {};

	// is_aggregate

	template <class T> struct is_literal_type : mm::integral_constant<bool,__is_literal_type(T)> {};
	template <class T> struct is_standard_layout : mm::integral_constant<bool,__is_standard_layout(T)> {};
	template <class T> struct has_virtual_destructor : mm::integral_constant<bool,__has_virtual_destructor(T)> {};
	template <class T> struct is_abstract : mm::integral_constant<bool,__is_abstract(T)> {};
	
	// is_final

	template <class T> struct is_empty : mm::integral_constant<bool,__is_empty(T)> {};
	template <class T> struct is_enum : mm::integral_constant<bool,__is_enum(T)> {};
	
	template <class T,class... Args> struct is_trivially_constructible : mm::integral_constant<bool,__is_trivially_constructible(T,Args...)> {};
	template <class T,class U> struct is_trivially_assignable : mm::integral_constant<bool,__is_trivially_assignable(T,U)> {};
	template <class T> struct is_trivially_destructible : mm::integral_constant<bool,__has_trivial_destructor(T)> {};
	template <class T> struct is_trivially_copyable : mm::integral_constant<bool,__has_trivial_copy(T)> {};
	template <class T> using underlying_type_t = __underlying_type(T);
	#elif defined(__clang__)
	#elif defined(_MSC_VER)
	#endif

	template <class T> constexpr T min(const T lhs,const T rhs) { return lhs < rhs ? lhs : rhs; }
	template <class T> constexpr T max(const T lhs,const T rhs) { return lhs > rhs ? lhs : rhs; }

	using true_t = integral_constant<bool,true>;
	using false_t = integral_constant<bool,false>;
	
	template <class T> using deduced_true_t = true_t;
	template <class T> using deduced_false_t = false_t;

	template <bool B,class T,class F> struct condition : type_identity<T> {};
	template <class T,class F> struct condition<false,T,F> : type_identity<F> {};
	template <bool B,class T = mm::true_t,class F = mm::false_t> using condition_t = typename condition<B,T,F>::type;

	template <bool B,class T = void> struct enable_if {};
	template <class T> struct enable_if<true,T> : type_identity<T> {};

	template <class T> struct remove_const : type_identity<T> {};
	template <class T> struct remove_const<const T> : type_identity<T> {};
	template <class T> using remove_const_t = typename remove_const<T>::type;
	
	template <class T> struct remove_volatile : type_identity<T> {};
	template <class T> struct remove_volatile<volatile T> : type_identity<T> {};
	template <class T> using remove_volatile_t = typename remove_volatile<T>::type;	

	template <class T> struct remove_cv : type_identity<T> {};
	template <class T> struct remove_cv<const T> : type_identity<T> {};
	template <class T> struct remove_cv<volatile T> : type_identity<T> {};
	template <class T> struct remove_cv<const volatile T> : type_identity<T> {};
	template <class T> using remove_cv_t = typename remove_cv<T>::type;

	template <class T> struct add_const : type_identity<const T> {};
	template <class T> struct add_volatile : type_identity<volatile T> {};
	template <class T> struct add_cv : type_identity<const volatile T> {};
	template <class T> using add_cv_t = typename remove_cv<T>::type;

	template <class T> struct remove_reference : type_identity<T> {};
	template <class T> struct remove_reference<T&> : type_identity<T> {};
	template <class T> struct remove_reference<T&&> : type_identity<T> {};
	template <class T> using remove_reference_t = typename remove_reference<T>::type;

	namespace detail {
		template <class T> type_identity<T&> add_lvalue_reference_impl(int);
		template <class T> type_identity<T> add_lvalue_reference_impl(...);
	}

	template <class T> struct add_lvalue_reference : decltype(detail::add_lvalue_reference_impl<T>(0)) {};
	template <class T> using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

	namespace detail {
		template <class T> type_identity<T&&> add_rvalue_reference_impl(int);
		template <class T> type_identity<T> add_rvalue_reference_impl(...);
	}

	template <class T> struct add_rvalue_reference : decltype(detail::add_rvalue_reference_impl<T>(0)) {};
	template <class T> using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

	template <class T> struct remove_pointer : type_identity<T> {};
	template <class T> struct remove_pointer<T*> : type_identity<T> {};
	template <class T> struct remove_pointer<T* const> : type_identity<T> {};
	template <class T> struct remove_pointer<T* volatile> : type_identity<T> {};
	template <class T> struct remove_pointer<T* const volatile> : type_identity<T> {};
	template <class T> using remove_pointer_t = typename remove_pointer<T>::type;

	namespace detail {
		template <class T> type_identity< typename mm::remove_reference<T>::type* > add_pointer_impl(int);
		template <class T> type_identity<T> add_pointer_impl(...);
	}

	template <class T> struct add_pointer : decltype(detail::add_pointer_impl<T>(0)) {};
	template <class T> using add_pointer_t = typename add_pointer<T>::type;
	
	namespace detail {
		template <class T> struct is_floating_point_impl : mm::false_t {};
		template <> struct is_floating_point_impl<float> : mm::true_t {};
		template <> struct is_floating_point_impl<double> : mm::true_t {};
		template <> struct is_floating_point_impl<long double> : mm::true_t {};
	}

	template <class T> struct is_floating_point : detail::is_floating_point_impl< mm::remove_cv_t<T> > {};

	namespace detail {
		template <class T> struct is_integral_impl : mm::false_t {};
		template <> struct is_integral_impl<bool> : mm::true_t {};
		template <> struct is_integral_impl<char> : mm::true_t {};
		template <> struct is_integral_impl<signed char> : mm::true_t {};
		template <> struct is_integral_impl<unsigned char> : mm::true_t {};
		template <> struct is_integral_impl<char16_t> : mm::true_t {};
		template <> struct is_integral_impl<char32_t> : mm::true_t {};
		template <> struct is_integral_impl<signed short> : mm::true_t {};
		template <> struct is_integral_impl<unsigned short> : mm::true_t {};
		template <> struct is_integral_impl<signed int> : mm::true_t {};
		template <> struct is_integral_impl<unsigned int> : mm::true_t {};
		template <> struct is_integral_impl<signed long> : mm::true_t {};
		template <> struct is_integral_impl<unsigned long> : mm::true_t {};
		template <> struct is_integral_impl<signed long long> : mm::true_t {};
		template <> struct is_integral_impl<unsigned long long> : mm::true_t {};
	}

	template <class T> struct is_integral : detail::is_integral_impl< mm::remove_cv_t<T> > {};
	template <class T> struct is_arithmetic : mm::integral_constant<bool, mm::is_integral<T>::value || mm::is_floating_point<T>::value > {};
	
	namespace detail {
		template <class T,bool = mm::is_arithmetic<T>::value> struct is_signed_impl : mm::integral_constant<bool,(T(-1) < T(0))> {};
		template <class T> struct is_signed_impl<T,false> : mm::false_t {};
	}

	template <class T> struct is_signed : detail::is_signed_impl<T>::type {};
	template <class T> struct is_unsigned : mm::integral_constant< bool, !mm::is_signed<T>::value > {};

	namespace detail {
		template <class T,class Type,class... Types> struct try_match_size : mm::condition<
			sizeof(T) == sizeof(Type),
			Type,
			typename try_match_size<T,Types...>::type
		> {};

		template <class T,class Type> struct try_match_size<T,Type> : mm::condition<
			sizeof(T) == sizeof(Type),
			Type,
			T
		> {};

		template <class T,class U> struct rebind_cv : type_identity<U> {};
		template <class T,class U> struct rebind_cv<const T,U> : type_identity<const U> {};
		template <class T,class U> struct rebind_cv<volatile T,U> : type_identity<volatile U> {};
		template <class T,class U> struct rebind_cv<const volatile T,U> : type_identity<const volatile U> {};

		template <class T> struct try_make_signed : try_match_size<T,signed char,signed short,signed int,signed long,signed long long> {};
		template <> struct try_make_signed<bool> : type_identity<bool> {};
		template <> struct try_make_signed<signed char> : type_identity<signed char> {};
		template <> struct try_make_signed<unsigned char> : type_identity<signed char> {};
		template <> struct try_make_signed<signed int> : type_identity<signed int> {};
		template <> struct try_make_signed<unsigned int> : type_identity<signed int> {};
		template <> struct try_make_signed<signed long> : type_identity<signed long> {};
		template <> struct try_make_signed<unsigned long> : type_identity<signed long> {};
		template <> struct try_make_signed<signed long long> : type_identity<signed long long> {};
		template <> struct try_make_signed<unsigned long long> : type_identity<signed long long> {};
	}

	template <class T> struct make_signed : detail::rebind_cv< T, typename detail::try_make_signed<T>::type > {};
	template <class T> using make_signed_t = typename make_signed<T>::type;

	namespace detail {
		template <class T> struct try_make_unsigned : try_match_size<T,unsigned char,unsigned short,unsigned int,unsigned long,unsigned long long> {};
		template <> struct try_make_unsigned<bool> : type_identity<bool> {};
		template <> struct try_make_unsigned<signed char> : type_identity<unsigned char> {};
		template <> struct try_make_unsigned<unsigned char> : type_identity<unsigned char> {};
		template <> struct try_make_unsigned<signed int> : type_identity<unsigned int> {};
		template <> struct try_make_unsigned<unsigned int> : type_identity<unsigned int> {};
		template <> struct try_make_unsigned<signed long> : type_identity<unsigned long> {};
		template <> struct try_make_unsigned<unsigned long> : type_identity<unsigned long> {};
		template <> struct try_make_unsigned<signed long long> : type_identity<unsigned long long> {};
		template <> struct try_make_unsigned<unsigned long long> : type_identity<unsigned long long> {};
	}

	template <class T> struct make_unsigned : detail::rebind_cv< T, typename detail::try_make_unsigned<T>::type > {};
	template <class T> using make_unsigned_t = typename make_unsigned<T>::type;

	template <class T> struct remove_extent : mm::type_identity<T> {};
	template <class T> struct remove_extent<T[]> : mm::type_identity<T> {};
	template <class T,size_t I> struct remove_extent<T[I]> : mm::type_identity<T> {};
	template <class T> using remove_extent_t = typename remove_extent<T>::type;

	template <class T> struct remove_all_extents : mm::type_identity<T> {};
	template <class T> struct remove_all_extents<T[]> : mm::type_identity< typename mm::remove_all_extents<T>::type > {};
	template <class T,size_t I> struct remove_all_extents<T[I]> : mm::type_identity< typename mm::remove_all_extents<T>::type > {};

	template <class T,class U> struct is_same : mm::false_t {};
	template <class T> struct is_same<T,T> : mm::true_t {};

	template <class T> struct is_void : mm::is_same<T,void> {};
	template <class T> struct is_null_pointer : mm::is_same< mm::nullptr_t, mm::remove_cv_t<T> > {};
	

	template <class T> struct is_fundamental : mm::integral_constant<bool,
		mm::is_arithmetic<T>::value
	     || mm::is_void<T>::value
	     || mm::is_same<mm::nullptr_t,mm::remove_cv_t<T>>::value
	> {};

	template <class T> struct is_compound : mm::integral_constant<bool,!mm::is_fundamental<T>::value> {};

	namespace detail {
		template <class B,class D> mm::true_t is_base_of_impl(const volatile B*);
		template <class B,class D> mm::false_t is_base_of_impl(const volatile void*);
	}

	template <class B,class D> struct is_base_of : decltype(detail::is_base_of_impl<B,D>(static_cast<D*>(nullptr))) {};

	template <class T> struct is_array : mm::false_t {};
	template <class T> struct is_array<T[]> : mm::true_t {};
	template <class T,size_t I> struct is_array<T[I]> : mm::true_t {};

	namespace detail {
		template <class T> mm::integral_constant<bool,!mm::is_union<T>::value> is_class_impl(int T::*);
		template <class T> mm::false_t is_class_impl(...);
	}

	template <class T> struct is_class : decltype(detail::is_class_impl<T>(nullptr)) {};
	
	namespace detail {
		template <class T> struct is_pointer_impl : mm::false_t {};
		template <class T> struct is_pointer_impl<T*> : mm::true_t {};
	}

	template <class T> struct is_pointer : detail::is_pointer_impl< mm::remove_cv_t<T> > {};

	template <class T> struct is_lvalue_reference : mm::false_t {};
	template <class T> struct is_lvalue_reference<T&> : mm::true_t {};

	template <class T> struct is_rvalue_reference : mm::false_t {};
	template <class T> struct is_rvalue_reference<T&&> : mm::true_t {};

	template <class T> struct is_reference : mm::integral_constant<bool,
		mm::is_lvalue_reference<T>::value
	     || mm::is_rvalue_reference<T>::value
	> {};

	template <class T> struct is_const : mm::false_t {};
	template <class T> struct is_const<const T> : mm::true_t {};

	template <class T> struct is_volatile : mm::false_t {};
	template <class T> struct is_volatile<volatile T> : mm::true_t {};

	template <class T> struct is_function : mm::integral_constant<bool,!mm::is_const<const T>::value && mm::is_reference<T>::value> {};

	namespace detail {
		template <class T> struct is_member_pointer_impl : mm::false_t {};
		template <class T,class U> struct is_member_pointer_impl<T U::*> : mm::true_t {};
	}

	template <class T> struct is_member_pointer : detail::is_member_pointer_impl< mm::remove_cv_t<T> > {};
	
	namespace detail {
		template <class T> struct is_member_function_pointer_impl : mm::false_t {};
		template <class T,class U> struct is_member_function_pointer_impl<T U::*> : mm::is_function<T> {};
	}

	template <class T> struct is_member_function_pointer : detail::is_member_function_pointer_impl< mm::remove_cv_t<T> > {};
	template <class T> struct is_member_object_pointer : mm::integral_constant<bool, mm::is_member_pointer<T>::value && !mm::is_member_function_pointer<T>::value > {};

	template <class T> struct is_scalar : mm::integral_constant<bool,
		mm::is_arithmetic<T>::value
	     || mm::is_enum<T>::value
	     || mm::is_pointer<T>::value
	     || mm::is_member_pointer<T>::value
	     || mm::is_null_pointer<T>::value
	> {};

	template <class T> struct is_object : mm::integral_constant<bool,
		mm::is_scalar<T>::value
	     || mm::is_array<T>::value
	     || mm::is_union<T>::value
	     || mm::is_class<T>::value
	> {};

	namespace detail {
		template <class T> mm::true_t is_polymorphic_impl(decltype(dynamic_cast<const volatile void*>(static_cast<T*>(nullptr))));
		template <class T> mm::false_t is_polymorphic_impl(...);
	}

	template <class T> struct is_polymorphic : decltype(detail::is_polymorphic_impl<T>(nullptr)) {};
	
	template <class T> struct is_bounded_array : mm::false_t {};
	template <class T,size_t I> struct is_bounded_array<T[I]> : mm::true_t {};

	template <class T> struct is_unbounded_array : mm::false_t {};
	template <class T> struct is_unbounded_array<T[]> : mm::true_t {};

	template <class T> mm::add_rvalue_reference_t<T> declval();
	
	namespace detail {
		template <class T> mm::deduced_true_t<T()> is_returnable(int);
		template <class T> false_t is_returnable(...);

		template <class T,class U> mm::deduced_true_t< decltype( mm::declval< void(&)(U) >()( mm::declval<T>() ) ) > is_nonvoid_convertible(int);
		template <class T,class U> mm::false_t is_nonvoid_convertible(...);
	}

	template <class T,class U> struct is_convertible : mm::integral_constant<bool,
		(decltype(detail::is_returnable<T>(0))::value && decltype(detail::is_nonvoid_convertible<T,U>(0))::value)
	     || (mm::is_void<T>::value && mm::is_void<U>::value)
	> {};


	namespace detail {
		template <class T,class U = typename mm::remove_reference<T>::type> struct decay : mm::condition_t<
			mm::is_array<U>::value,
			mm::remove_extent_t<U>*,
			mm::condition_t<
				mm::is_function<U>::value,
				mm::add_pointer_t<U>,
				mm::remove_cv_t<U>
			>
		> {};
	}

	template <class T> struct decay : detail::decay<T> {};
	template <class T> using decay_t = typename decay<T>::type;

	template <class T> struct remove_cvref : mm::type_identity< mm::remove_cv_t< mm::remove_reference_t<T> > > {};
	template <class T> using remove_cvref_t = typename remove_cvref<T>::type;

	namespace detail {
		template <class,class T,class... Args> struct is_constructible_impl : mm::false_t {};
		template <class T,class... Args> struct is_constructible_impl< mm::void_t<decltype( T(mm::declval<Args>()...) )>,T,Args...> : mm::true_t {};
	}

	template <class T,class... Args> struct is_constructible : detail::is_constructible_impl<void,T,Args...> {};
	template <class T> struct is_default_constructible : mm::is_constructible<T> {};
	template <class T> struct is_trivially_default_constructible : mm::is_trivially_constructible<T> {};
	template <class T> struct is_trivially_copy_constructible : mm::is_trivially_constructible<T,const T&> {};
	
	template <class T> struct is_move_constructible : mm::is_constructible< T, mm::add_rvalue_reference_t<T> > {};
	template <class T> struct is_trivially_move_constructible : mm::is_constructible<T,T&&> {};

	namespace detail {
		template <class T,class U,class = void> struct is_assignable_impl : mm::false_t {};
		template <class T,class U>  struct is_assignable_impl< T, U, decltype(mm::declval<T>() = mm::declval<U>(),void()) > : mm::true_t {};
	}

	template <class T,class U> struct is_assignable : detail::is_assignable_impl<T,U> {};

	template <class T> struct is_copy_assignable : mm::is_assignable< mm::add_lvalue_reference_t<T>, mm::add_lvalue_reference_t<const T> > {};
	template <class T> struct is_trivially_copy_assignable : mm::is_trivially_assignable<T,const T&> {};

	template <class T> struct is_move_assignable : mm::is_assignable< mm::add_lvalue_reference_t<T>, mm::add_rvalue_reference_t<T> > {};
	template <class T> struct is_trivially_move_assignable : mm::is_trivially_assignable< T, mm::add_rvalue_reference_t<T> > {};

	template <class T> struct is_trivial : mm::integral_constant< bool, mm::is_trivially_copyable<T>::value && mm::is_trivially_default_constructible<T>::value > {};

	template <class T> struct first_template_parameter : mm::type_identity<T> {};
	template <template <class,class...> class TT,class T,class... Ts> struct first_template_parameter<TT<T,Ts...>> : mm::type_identity<T> {};
	template <class T> using first_template_parameter_t = typename first_template_parameter<T>::type;

	template <class T,class U> struct rebind : type_identity<U> {};
	template <template <class,class...> class TT,class T,class... Ts,class U> struct rebind<TT<T,Ts...>,U> : mm::type_identity<TT<U,Ts...>> {};
	template <class T,class U> using rebind_t = typename rebind<T,U>::type;
}

#endif
