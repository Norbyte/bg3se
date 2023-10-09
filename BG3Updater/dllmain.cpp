#include "stdafx.h"

#include "DWriteWrapper.h"
#include "Updater.h"

extern "C" {

using namespace bg3se;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		gCoreLibPlatformInterface.ThisModule = hModule;
		DisableThreadLibraryCalls(hModule);
		gDWriteWrapper = std::make_unique<DWriteWrapper>();

		if (ShouldLoad()) {
			StartUpdaterThread();
		}
		break;

	case DLL_PROCESS_DETACH:
		if (gDWriteWrapper) {
			gDWriteWrapper.reset();
		}
		break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

}