#ifndef MM_COMMON_HPP
#define MM_COMMON_HPP
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#ifdef DEBUG
#define SHOW_PRINTF
#define SHOW_INFO
#define SHOW_WARNINGS
#define SHOW_ERRORS
#endif

#ifdef SHOW_PRINTF
#define PRINTF(...)\
	printf(__VA_ARGS__);\
	printf("\n")
#else
#define PRINTF(...)
#endif

#ifdef SHOW_INFO
#define INFO(...)\
	printf("\033[36mINFO\033[0m: ");\
	printf(__VA_ARGS__);\
	printf("\n")
#else
#define INFO(...)
#endif

#ifdef SHOW_WARNINGS
#define WARNING(...)\
	fprintf(stderr,"\033[33mWARNING\033[0m: ");\
	fprintf(stderr,__VA_ARGS__);\
	fprintf(stderr,"\n")
#else
#define WARNING(...)
#endif

#ifdef SHOW_ERRORS
#define ERROR(...)\
	fprintf(stderr,"\033[31mERROR\033[0m: ");\
	fprintf(stderr,__VA_ARGS__);\
	fprintf(stderr,"\n");\
	exit(EXIT_FAILURE)
#else
#define ERROR(...)\
	exit(EXIT_FAILURE)
#endif

#define ASSUME(cond,...)\
	if (!(cond)) {\
		WARNING(__VA_ARGS__);\
	}

#define ASSERT(cond,...)\
	if (!(cond)) {\
		ERROR(__VA_ARGS__);\
	}

#define STATIC_ASSERT(cond,msg)

namespace mm {
	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using u8 = uint8_t;
	using u16 = uint32_t;
	using u32 = uint32_t;
	using f32 = float;
	using f64 = double;
	using size_t = size_t;
	using ptrdiff_t = ptrdiff_t;
	using nullptr_t = decltype(nullptr);
}

inline void* operator new(mm::size_t length) {
	return calloc(1,length);
}

inline void* operator new[](mm::size_t length) {
	return calloc(1,length);
}

inline void operator delete(void *ptr) {
	free(ptr);
}

inline void operator delete[](void *ptr) {
	free(ptr);
}

#endif
