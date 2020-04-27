#ifndef MM_TYPE_TRAITS
#define MM_TYPE_TRAITS
#include "common.hpp"
#define TRAIT_NAME(name) __TRAIT_##name##__
#define TRAIT_HELPER(trait,name) __TRAIT_##trait##_HELPER_##name##__
#define TRAIT_ERROR_NAME(name) __TRAIT_ERROR_##name##__
#define TRAIT_ERROR_MSG(type,msg) static_assert(mm::deduced_false_t<type>::value,msg)

namespace mm {
	
	template <class T,T v>
	struct TRAIT_NAME(constant) {
		using type = TRAIT_NAME(constant)<T,v>;
		using value_type = T;
		static constexpr T value {v};
		operator bool() { return value; }
	};

	template <class T> struct TRAIT_NAME(type_identity) { using type = T; };

	using true_t = TRAIT_NAME(constant)<bool,true>;
	using false_t = TRAIT_NAME(constant)<bool,false>;
	template <class> using deduced_false_t = false_t;

	#define DECLARE_TRAIT_ERROR(name,msg)\
		template <class T>\
		struct TRAIT_ERROR_NAME(name) {\
			TRAIT_ERROR_MSG(T,msg);\
		}

	template <class...> using void_t = void;

	template <bool B,class = void> struct TRAIT_NAME(enable_if) {};
	template <class T> struct TRAIT_NAME(enable_if)<true,T> { using type = T; };
	template <bool B,class T = void> using enable_if_t = typename TRAIT_NAME(enable_if)<B,T>::type;

	template <class T> struct TRAIT_NAME(first_template_arg) {};
	template <template <class,class...> class TT,class T,class... Ts> struct TRAIT_NAME(first_template_arg)<TT<T,Ts...>> { using type = T; };
	template <class T> using first_template_arg_t = typename TRAIT_NAME(first_template_arg)<T>::type;

	template <class T,class U> struct TRAIT_NAME(rebind_first_template) {};
	template <template <class,class...> class TT,class U,class T,class... Ts> struct TRAIT_NAME(rebind_first_template)<TT<T,Ts...>,U> { using type = TT<U,Ts...>; };
	template <class T,class U> using rebind_first_template_t = typename TRAIT_NAME(rebind_first_template)<T,U>::type;
	
	template <class T> struct TRAIT_NAME(remove_const) { using type = T; };
	template <class T> struct TRAIT_NAME(remove_const)<const T> { using type = T; };
	template <class T> using remove_const_t = typename TRAIT_NAME(remove_const)<T>::type;

	template <class T> struct TRAIT_NAME(remove_volatile) { using type = T; };
	template <class T> struct TRAIT_NAME(remove_volatile)<volatile T> { using type = T; };
	template <class T> using remove_volatile_t = typename TRAIT_NAME(remove_volatile)<T>::type;

	template <class T> struct TRAIT_NAME(remove_cv) { using type = T; };
	template <class T> struct TRAIT_NAME(remove_cv)<const T> { using type = T; };
	template <class T> struct TRAIT_NAME(remove_cv)<volatile T> { using type = T; };
	template <class T> struct TRAIT_NAME(remove_cv)<const volatile T> { using type = T; };
	template <class T> using remove_cv_t = typename TRAIT_NAME(remove_cv)<T>::type;

	template <class T> struct TRAIT_NAME(remove_reference) { using type = T; };
	template <class T> struct TRAIT_NAME(remove_reference)<T&> { using type = T;} ;
	template <class T> struct TRAIT_NAME(remove_reference)<T&&> { using type = T;} ;
	template <class T> using remove_reference_t = typename TRAIT_NAME(remove_reference)<T>::type;

	template <class T> struct TRAIT_NAME(remove_cvref) { using type = remove_cv_t<remove_reference_t<T>>; };
	template <class T> using remove_cvref_t = typename TRAIT_NAME(remove_cvref)<T>::type;

	template <class T> struct TRAIT_NAME(remove_pointer) { using type = T; };
	template <class T> struct TRAIT_NAME(remove_pointer)<T*> { using type = T; };
	template <class T> using remove_pointer_t = typename TRAIT_NAME(remove_pointer)<T>::type;

	template <class T> struct TRAIT_NAME(remove_cv_ptr_ref) { using type = remove_cv_t<remove_pointer_t<remove_reference_t<T>>>; };
	template <class T> using remove_cv_ptr_ref_t = typename TRAIT_NAME(remove_cv_ptr_ref)<T>::type;

	template <class T,class U> struct TRAIT_NAME(rebind_cv) { using type = remove_cv_t<U>; };
	template <class T,class U> struct TRAIT_NAME(rebind_cv)<const T,U> { using type = const remove_cv_t<U>; };
	template <class T,class U> struct TRAIT_NAME(rebind_cv)<volatile T,U> { using type = const volatile remove_cv_t<U>; };
	template <class T,class U> struct TRAIT_NAME(rebind_cv)<const volatile T,U> { using type = const volatile remove_cv_t<U>; };
	template <class T,class U> using rebind_cv_t = typename TRAIT_NAME(rebind_cv)<T,U>::type;

	template <class T> TRAIT_NAME(type_identity)<T&> TRAIT_HELPER(add_lvalue_reference,impl)(int);
	template <class T> TRAIT_NAME(type_identity)<T> TRAIT_HELPER(add_lvalue_reference,impl)(...);
	template <class T> struct TRAIT_NAME(add_lvalue_reference) : decltype(TRAIT_HELPER(add_lvalue_reference,impl)<T>(0)) {};
	template <class T> using add_lvalue_reference_t = typename TRAIT_NAME(add_lvalue_reference)<T>::type;

	template <class T> TRAIT_NAME(type_identity)<T&&> TRAIT_HELPER(add_rvalue_reference,impl)(int);
	template <class T> TRAIT_NAME(type_identity)<T> TRAIT_HELPER(add_rvalue_reference,impl)(...);
	template <class T> struct TRAIT_NAME(add_rvalue_reference) : decltype(TRAIT_HELPER(add_rvalue_reference,impl)<T>(0)) {};
	template <class T> using add_rvalue_reference_t = typename TRAIT_NAME(add_rvalue_reference)<T>::type;

	template <bool B,class T,class F> struct TRAIT_NAME(condition) { using type = T; };
	template <class T,class F> struct TRAIT_NAME(condition)<false,T,F> { using type = F; };
	template <bool B,class T = true_t,class F = false_t> using condition_t = typename TRAIT_NAME(condition)<B,T,F>::type;

	template <class...> struct TRAIT_NAME(and) : true_t {};
	template <class T> struct TRAIT_NAME(and)<T> : T {};
	template <class T,class... Ts> struct TRAIT_NAME(and)<T,Ts...> : condition_t<T::value,TRAIT_NAME(and)<Ts...>,T> {};
	template <class... Ts> using and_t = typename TRAIT_NAME(and)<Ts...>::type;

	template <class...> struct TRAIT_NAME(or) : false_t {};
	template <class T> struct TRAIT_NAME(or)<T> : T {};
	template <class T,class... Ts> struct TRAIT_NAME(or)<T,Ts...> : condition_t<T::value,T,TRAIT_NAME(or)<Ts...>> {};
	template <class... Ts> using or_t = typename TRAIT_NAME(or)<Ts...>::type;

	template <class T> struct TRAIT_NAME(not) : condition_t<!T::value> {};
	template <class T> using not_t = typename TRAIT_NAME(not)<T>::type;

	template <class T> struct is_floating_point : false_t {};
	template <> struct is_floating_point<float> : true_t {};
	template <> struct is_floating_point<double> : true_t {};

	template <class T> struct is_integer : false_t {};
	template <> struct is_integer<bool> : true_t {};
	template <> struct is_integer<char> : true_t {};
	template <> struct is_integer<char16_t> : true_t {};
	template <> struct is_integer<char32_t> : true_t {};
	template <> struct is_integer<wchar_t> : true_t {};
	template <> struct is_integer<short> : true_t {};
	template <> struct is_integer<int> : true_t {};
	template <> struct is_integer<long> : true_t {};
	template <> struct is_integer<long long> : true_t {};

	template <class T> struct is_arithemetic : TRAIT_NAME(constant)<bool,is_floating_point<T>::value || is_integer<T>::value> {};

	template <class T,bool = is_integer<T>::value> struct TRAIT_HELPER(is_signed,impl) : TRAIT_NAME(constant)<bool,T(-1) < T(0)> {};
	template <class T> struct TRAIT_HELPER(is_signed,impl)<T,false> : false_t {};
	template <class T> struct is_signed : TRAIT_HELPER(is_signed,impl)<T>::type {};
	template <class T> struct is_unsigned : TRAIT_NAME(constant)<bool,!is_signed<T>::value> {};

	template <class T,class Char,class... Chars> struct TRAIT_HELPER(make_signed,char_type) { using type = condition_t<sizeof(T) == sizeof(Char),Char,typename TRAIT_HELPER(make_signed,char_type)<T,Chars...>::type>; };
	template <class T,class Char> struct TRAIT_HELPER(make_signed,char_type)<T,Char> { using type = Char; };
	template <class T> struct TRAIT_HELPER(make_signed,integer_type) : TRAIT_HELPER(make_signed,char_type)<T,char,wchar_t,char16_t,char32_t> {};
	template <> struct TRAIT_HELPER(make_signed,integer_type)<unsigned short> { using type = signed short; };
	template <> struct TRAIT_HELPER(make_signed,integer_type)<unsigned int> { using type = signed int; };
	template <> struct TRAIT_HELPER(make_signed,integer_type)<unsigned long> { using type = signed long; };
	template <> struct TRAIT_HELPER(make_signed,integer_type)<unsigned long long> { using type = signed long long; };
	template <class T> struct TRAIT_NAME(make_signed) { using type = condition_t<is_signed<T>::value,T,rebind_cv_t<T,typename TRAIT_HELPER(make_signed,integer_type)<T>::type>>; };
	template <class T> using make_signed_t = typename TRAIT_NAME(make_signed)<T>::type;

	template <class T> struct TRAIT_HELPER(make_unsigned,integer_type) : TRAIT_HELPER(make_signed,char_type)<T,char,wchar_t,char16_t,char32_t> {};
	template <> struct TRAIT_HELPER(make_unsigned,integer_type)<signed short> { using type = unsigned short; };
	template <> struct TRAIT_HELPER(make_unsigned,integer_type)<signed int> { using type = unsigned int; };
	template <> struct TRAIT_HELPER(make_unsigned,integer_type)<signed long> { using type = unsigned long; };
	template <> struct TRAIT_HELPER(make_unsigned,integer_type)<signed long long> { using type = unsigned long long; };
	template <class T> struct TRAIT_NAME(make_unsigned) { using type = condition_t<is_unsigned<T>::value,T,rebind_cv_t<T,typename TRAIT_HELPER(make_unsigned,integer_type)<T>::type>>; };
	template <class T> using make_unsigned_t = typename TRAIT_NAME(make_unsigned)<T>::type;

	template <class T,class U> struct is_same : false_t {};
	template <class T> struct is_same<T,T> : true_t {};

	template <size_t L,size_t R> struct is_less : condition_t< L < R> {};
	template <size_t L,size_t R> struct is_greater : condition_t<(L > R)> {};

	template <class T> struct is_pointer : false_t {};
	template <class T> struct is_pointer<T*> : true_t {};

	template <class T> struct is_lvalue_reference : false_t {};
	template <class T> struct is_lvalue_reference<T&> : true_t {};

	template <class T> struct is_const_lvalue_reference : false_t {};
	template <class T> struct is_const_lvalue_reference<const T&> : true_t {};

	template <class T> struct is_mutable_lvalue_reference : TRAIT_NAME(constant)<bool,is_lvalue_reference<T>::value && !is_const_lvalue_reference<T>::value> {};

	template <class T> struct is_rvalue_reference : false_t {};
	template <class T> struct is_rvalue_reference<T&&> : true_t {};

	template <class T> false_t TRAIT_HELPER(is_class,test)(...);
	template <class T> true_t TRAIT_HELPER(is_class,test)(int T::*);
	template <class T> struct is_class : decltype(TRAIT_HELPER(is_class,test)<T>(nullptr)) {};

	template <class B,class D> false_t TRAIT_HELPER(is_base_of,test)(...);
	template <class B,class D> true_t TRAIT_HELPER(is_base_of,test)(B*);
	template <class B,class D> struct is_base_of : decltype(TRAIT_HELPER(is_base_of,test)<B,D>(static_cast<D*>(nullptr))) {};

	template <class T> struct TRAIT_HELPER(type_name,impl) { static constexpr const char* value = "?"; };

	#define DECLARE_TYPE_NAME(type,name)\
		template <>\
		struct TRAIT_HELPER(type_name,impl)<type> {\
			static constexpr const char* value = name;\
		}

	DECLARE_TYPE_NAME(bool,"bool");
	DECLARE_TYPE_NAME(int8_t,"i8");
	DECLARE_TYPE_NAME(int16_t,"i16");
	DECLARE_TYPE_NAME(int32_t,"i32");
	DECLARE_TYPE_NAME(int64_t,"i64");
	DECLARE_TYPE_NAME(uint8_t,"u8");
	DECLARE_TYPE_NAME(uint16_t,"u16");
	DECLARE_TYPE_NAME(uint32_t,"u32");
	DECLARE_TYPE_NAME(uint64_t,"u64");
	DECLARE_TYPE_NAME(float,"f32");
	DECLARE_TYPE_NAME(double,"f64");
	DECLARE_TYPE_NAME(nullptr_t,"nullptr_t");

	template <class T> struct type_name {
		static constexpr const char* value = TRAIT_HELPER(type_name,impl)<remove_cv_ptr_ref_t<T>>::value;
	};
};

#endif
