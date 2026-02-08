// GraphDraw_Eng.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "GraphDraw_Eng.h"

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
GRAPHDRAW_ENG_API int nGraphDraw_Eng=0;

// This is an example of an exported function.
GRAPHDRAW_ENG_API int fnGraphDraw_Eng(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see GraphDraw_Eng.h for the class definition
CGraphDraw_Eng::CGraphDraw_Eng()
{ 
	return; 
}

