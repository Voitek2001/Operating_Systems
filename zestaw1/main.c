#include <stdio.h>
#include "count.h"
#include <stdlib.h>



int main() {
	int x = 10;
	ArrayOfBlocks * st = create_empty_array(x);
	show_at(st ,5);
	return 0;
}
