#include <psl1ght/lv2.h>
#include <kammy.h>
#include <common/kammy_lv2.h>

#include "ethdebug.bin.h"

#include <stdio.h>

static u64 LoadModule(const u8* bin, const char* name, u64 param1 = 0, u64 param2 = 0, u64 param3 = 0, u64 param4 = 0, u64 param5 = 0, u64 param6 = 0)
{
	u64 ret = KAMMY_ERROR;
	printf("\tLoading %s into lv2... ", name);
	const Lv2Module* module = Kammy_Load(bin);
	if (!module->Verify()) {
		printf("Failed! %s is corrupt.\n", name);
		return ret;
	}
	if (!module->Execute(&ret, param1, param2, param3, param4, param5, param6))
		printf("Failed! Kammy was unable to load %s.\n", name);
	else
		printf("Done! Result returned from %s in lv2: 0x%08x%08x\n", name, (u32)(ret >> 32), (u32)ret);
	return ret;
}

static bool Init()
{
	if (Kammy_IsInitialised())
		printf("\tAlready loaded!\n");
	else {
		printf("\tInitializing... ");
		if (Kammy_Initialise())
			printf("Done! Kammy injected into lv2 successfully.\n");
		else {
			printf("Failed! Syscalls could not be injected. Does this payload have peek/poke?\n");
			return false;
		}
	}

	return true;
}

extern "C" int main(int argc, char** argv)
{
	printf("Kammy v" LIBKAMMY_VERSION "\n");
	
	if (!Init()) {
		printf("Failed!\n");
		return 0;
	}
	
	// Want to see this debug info?
	// 1) The PS3 must be connected to a router/computer via ethernet cable.
	//    If wireless is enabled it won't work.
	// 2) Be on the same network/router/etc as the PS3
	// 3) Install socat on your PC, and run the following command:
	//    socat udp-recv:18194 stdout
	LoadModule(ethdebug_bin, "ethdebug.bin");
	printf("This is a ttyWrite test, it will show up over UDP.\n");

	// After this hook is installed it will stay in place as long as
	// the PS3 stays on. Great for getting printfs from other PSL1GHT
	// apps.

	printf("Aaand we're done for tonight folks.\n");

	return 0;
}
