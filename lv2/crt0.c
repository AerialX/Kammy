#include "lv2.h"

u64 start();

u64 _start(u64 rtoc)
{
	asm("mr %%r2, %0"
			:
			:"r"(rtoc)
			);
	return start();
}
