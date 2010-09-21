#include <sys.h>
#include <kammy.h>

#include "debug.h"

#include "kammy.bin.h"

extern "C" int main(int argc, char** argv)
{
	const Lv2Module* module = NULL;

	if (!Debug_Connect())
		return 0;

	Debug_Print("Kammy v" KAMMY_VERSION "\n");

	Debug_Print("\tInitializing... ");
	if (Kammy_Init())
		Debug_Print("Done! Syscall injected successfully.\n");
	else {
		Debug_Print("Failed! Syscalls could not be injected. Does this payload have peek/poke?\n");
		goto fail;
	}

	Debug_Print("\tLoading kammy.bin into lv2... ");
	module = Kammy_Load(kammy_bin);
	if (!module->Verify()) {
		Debug_Print("Failed! kammy.bin magic exception; needs moar mana.\n");
		goto fail;
	}
	u64 ret;
	if (!module->Execute(&ret)) {
		Debug_Print("Failed! Kammy was unable to allocate memory for kammy.bin.\n");
		goto fail;
	} else {
		Debug_Print("Done! Result returned from kammy.bin in lv2: 0x%08x%08x\n", (u32)(ret >> 32), (u32)ret);
	}

fail:
	Debug_Disconnect();

	return 0;
}
