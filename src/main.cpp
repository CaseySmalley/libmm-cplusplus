#include "mm/memory.hpp"

int main(int argc,const char *argv[]) {
	mm::unique_ptr<int,mm::default_delete<int>> ptr;

	return 0;
}
