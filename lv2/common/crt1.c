#include <stdint.h>

uint64_t start();

typedef void (*ctor)(void);
extern ctor __ctors_start;
extern ctor __ctors_end;

#ifdef KAMMY
uint64_t __start()
#else
uint64_t _start()
#endif
{
	// Static constructors; note that these crash AFAICT.
	for (ctor* c = &__ctors_start; c != &__ctors_end; c++)
		(*c)();

	return start();
}
