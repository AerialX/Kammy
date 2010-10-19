#include "kammy_lv2.h"

#include "list.h"

typedef u64 (*Subcall)(u64, u64, u64, u64, u64, u64);
typedef Pair<u64, Subcall> SubcallPair;
typedef List<SubcallPair> CustomSubcallList;
CustomSubcallList* CustomSubcalls;

static Subcall GetSubcall(u64 subcall) {
	for (CustomSubcallList::Iterator iter = CustomSubcalls->Begin(); iter != CustomSubcalls->End(); iter++) {
		if (iter->Key == subcall)
			return iter->Value;
	}
	return NULL;
}

void __Init_Module()
{
	CustomSubcalls = new CustomSubcallList();
}

s64 Kammy_HookSubcall(u64 subcall, void* func)
{
	for (CustomSubcallList::Iterator iter = CustomSubcalls->Begin(); iter != CustomSubcalls->End(); iter++) {
		if (iter->Key == subcall) {
			iter->Value = (Subcall)func;
			return KAMMY_ERROR_OK;
		}
	}
	CustomSubcalls->Add(SubcallPair(subcall, (Subcall)func));
	return KAMMY_ERROR_OK;
}

u64 Kammy_CallSubcall(u64 subcall, u64 param1, u64 param2, u64 param3, u64 param4, u64 param5, u64 param6)
{
	Subcall func = GetSubcall(subcall);
	if (func)
		return func(param1, param2, param3, param4, param5, param6);
	return KAMMY_ERROR;
}

s64 Kammy_GetModules(void** buffer, u64* size)
{
	return KAMMY_ERROR_UNIMPLEMENTED;
}

s64 Kammy_UnloadModule(void* base)
{
	return KAMMY_ERROR_UNIMPLEMENTED;
}

s64 Kammy_RegisterModule(void* base)
{
	return KAMMY_ERROR_UNIMPLEMENTED;
}

