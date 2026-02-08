
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTDESIGNPROPERTYADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTDESIGNPROPERTYADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTDESIGNPROPERTYADDIN_ENG_EXPORTS
#define CHARTDESIGNPROPERTYADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTDESIGNPROPERTYADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartDesignPropertyAddIn_Eng.dll
class CHARTDESIGNPROPERTYADDIN_ENG_API CChartDesignPropertyAddIn_Eng {
public:
	CChartDesignPropertyAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTDESIGNPROPERTYADDIN_ENG_API int nChartDesignPropertyAddIn_Eng;

CHARTDESIGNPROPERTYADDIN_ENG_API int fnChartDesignPropertyAddIn_Eng(void);

