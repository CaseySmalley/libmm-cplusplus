#include "mm/memory.hpp"
#include <stdio.h>

int main(int argc,const char *argv[]) {
	auto i = mm::make_unique<int>(5);

	return 0;
}
