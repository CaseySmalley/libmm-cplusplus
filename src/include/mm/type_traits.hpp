#ifndef MM_TYPE_TRAITS
#define MM_TYPE_TRAITS
#include "common.hpp"
#define TRAIT_NAME(name) __TRAIT_##name##__

namespace mm {
	template <class T,T v>
	struct TRAIT_NAME(constant) {
		using type = constant<T,v>;
		using value_type = T;
		static constexpr T value {v};
		operator bool() { return value; }
	};

	using true_t = TRAIT_NAME(constant)<bool,true>;
	using false_t = TRAIT_NAME(constant)<bool,false>;

	template <class...> using void_t = void;

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

	template <bool B,class T,class F> struct TRAIT_NAME(condition) { using type = T; };
	template <class T,class F> struct TRAIT_NAME(condition)<false,T,F> { using type = F; };
	template <bool B,class T = true_t,class F = false_t> using condition_t = typename TRAIT_NAME(condition)<B,T,F>::type;

	template <class...> struct TRAIT_NAME(and) : true_t {};
	template <class T> struct TRAIT_NAME(and)<T> : T {};
	template <class T,class... Ts> struct TRAIT_NAME(and)<T,Ts...> : condition_t<T::value,TRAIT_NAME(and)<Ts...>,T> {};
	template <class... Ts> using and_t = using TRAIT_NAME(and)<Ts...>::type;

	template <class...> struct TRAIT_NAME(or) : false_t {};
	template <class T> struct TRAIT_NAME(or)<T> : T {};
	template <class T,class... Ts> struct TRAIT_NAME(or)<T,Ts...> : condition_t<T::value,T,TRAIT_NAME(or)<Ts...>> {};
	template <class... Ts> using or_t = using TRAIT_NAME(or)<Ts...>::type;

	template <class T> class TRAIT_NAME(not) : condition_t<!T::value> {};
	template <class T> using not_t = typename TRAIT_NAME(not)::type;

	template <class T> struct TRAIT_NAME(is_pointer) : false_t {};
	template <class T> struct TRAIT_NAME(is_pointer)<T*> : true_t {};
	template <class T> using is_pointer_v = TRAIT_NAME(is_pointer)<T>::value;

	template <class T> struct TRAIT_NAME(is_lvalue_reference) : false_t;
	template <class T> struct TRAIT_NAME(is_lvalue_reference)<T&> : true_t;
	template <class T> using is_lvalue_reference_v = TRAIT_NAME(is_lvalue_reference)<T>::value;

	template <class T> struct TRAIT_NAME(is_rvalue_reference) : false_t;
	template <class T> struct TRAIT_NAME(is_rvalue_reference)<T&&> : true_t;
	template <class T> using is_rvalue_reference_v = TRAIT_NAME(is_rvalue_reference)<T>::value;
};

#endif
