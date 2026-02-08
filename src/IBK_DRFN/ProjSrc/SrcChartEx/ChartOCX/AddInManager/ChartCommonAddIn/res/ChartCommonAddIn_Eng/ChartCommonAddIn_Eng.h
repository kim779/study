
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTCOMMONADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTCOMMONADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTCOMMONADDIN_ENG_EXPORTS
#define CHARTCOMMONADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTCOMMONADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartCommonAddIn_Eng.dll
class CHARTCOMMONADDIN_ENG_API CChartCommonAddIn_Eng {
public:
	CChartCommonAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTCOMMONADDIN_ENG_API int nChartCommonAddIn_Eng;

CHARTCOMMONADDIN_ENG_API int fnChartCommonAddIn_Eng(void);

