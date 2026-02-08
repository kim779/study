// ChartEnvSetupDlgAddIn_Eng.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn_Eng.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
CHARTENVSETUPDLGADDIN_ENG_API int nChartEnvSetupDlgAddIn_Eng=0;

// This is an example of an exported function.
CHARTENVSETUPDLGADDIN_ENG_API int fnChartEnvSetupDlgAddIn_Eng(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ChartEnvSetupDlgAddIn_Eng.h for the class definition
CChartEnvSetupDlgAddIn_Eng::CChartEnvSetupDlgAddIn_Eng()
{ 
	return; 
}

