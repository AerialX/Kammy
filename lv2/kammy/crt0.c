#include <stdint.h>

uint64_t __start();
uint64_t start();

uint64_t _start(uint64_t rtoc)
{
	uint64_t originalrtoc;
	asm("mr %0, %%r2":"=r"(originalrtoc):);
	asm("mr %%r2, %0"::"r"(rtoc));
	uint64_t ret = __start();
	asm("mr %%r2, %0"::"r"(originalrtoc));
	return ret;
}
