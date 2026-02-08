// ChartGuideLineAddIn_Eng.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ChartGuideLineAddIn_Eng.h"

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
CHARTGUIDELINEADDIN_ENG_API int nChartGuideLineAddIn_Eng=0;

// This is an example of an exported function.
CHARTGUIDELINEADDIN_ENG_API int fnChartGuideLineAddIn_Eng(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see ChartGuideLineAddIn_Eng.h for the class definition
CChartGuideLineAddIn_Eng::CChartGuideLineAddIn_Eng()
{ 
	return; 
}

