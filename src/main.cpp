#include "mm/functional.hpp"
#include <stdio.h>

int main(int argc,const char *argv[]) {
	int x = 1;
	int y = 2;
	int z = 3;

	mm::reference_wrapper<int> arr[] {x,y,z};

	y = 500;

	for (int i : arr) {
		printf("%d\n",i);
	}

	return 0;
}
