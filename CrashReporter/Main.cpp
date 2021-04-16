#include "stdafx.h"
#include "CrashReporter.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	CrashReporter reporter(GetCommandLineW());
	reporter.Report();

    return 0;
}
