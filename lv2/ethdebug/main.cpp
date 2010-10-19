#include "lv2.h"

#include "kammy_lv2.h"

#include "debug.h"
#include "lv1call.h"

int ttyWriteHook(int channel, const char* message, int length, int* written)
{
	debug_printf("%s", message);
	if (written)
		*written = length;
	return 0;
}

extern "C" u64 start()
{
	u64 ret = debug_init();
	if (ret)
		return ret;
	ret = debug_printf("ethdebug.bin: Initialized\n");
	if (ret)
		return ret;
	Kammy_HookSyscall(403, (void*)ttyWriteHook); // tty_write
	Kammy_HookBranch((void*)0x800000000028A654, (void*)debug_printf, false); // lv2_printf
	Kammy_HookBranch((void*)0x800000000028F050, (void*)debug_printf, false); // lv2_printf_null
	return 0;
}
