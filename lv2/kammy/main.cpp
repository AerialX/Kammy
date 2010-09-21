#include "lv2.h"

// Random tests for rtoc
static int test = 0x185;
#define KAMMY_RTOC_BASE		0x8000000000332B58ULL

extern "C" u64 start()
{
	return Lv2Syscall(6, KAMMY_RTOC_BASE) + test++;
}

