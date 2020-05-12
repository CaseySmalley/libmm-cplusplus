#include "mm/memory.hpp"
#include <stdio.h>

struct type {
	type() { printf("constructor\n"); }
	~type() { printf("destructor\n"); }
};

int main(int argc,const char *argv[]) {
	mm::shared_ptr<type> ptr(new type());
	return 0;
}
