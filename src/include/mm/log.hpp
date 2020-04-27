#ifndef MM_LOG_HPP
#define MM_LOG_HPP
#include "mm/utility.hpp"

/*
	info
	warning
	error
	debug
*/

namespace mm {
	namespace log_impl {
		template <size_t LEVEL> struct should_do_log : condition_t<LEVEL >= logging_level> {};

		template <class Ts...>
		void do_log(false_t,Ts&&... ts...) {}

		template <class Ts...>
		void do_log(true_t,Ts&&... ts...) {
			
		}
	}

	template <size_t LEVEL,class... Ts>
	void log(Ts&&... ts) {
		log_impl::do_log( typename log_impl::should_do_log<LEVEL>::type {}, foward<Ts...>(ts)... );
	}
}

#endif
