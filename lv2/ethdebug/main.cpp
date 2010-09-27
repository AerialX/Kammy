#include "lv2.h"

#include "kammy_lv2.h"

#include "debug.h"

extern "C" u64 start()
{
	u64 ret = debug_init();
	if (ret)
		return ret;
	return debug_printf("ohai thar");
}
