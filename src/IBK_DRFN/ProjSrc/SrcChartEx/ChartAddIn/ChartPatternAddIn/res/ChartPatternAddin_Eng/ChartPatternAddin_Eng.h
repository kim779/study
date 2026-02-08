
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTPATTERNADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTPATTERNADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTPATTERNADDIN_ENG_EXPORTS
#define CHARTPATTERNADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTPATTERNADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartPatternAddin_Eng.dll
class CHARTPATTERNADDIN_ENG_API CChartPatternAddin_Eng {
public:
	CChartPatternAddin_Eng(void);
	// TODO: add your methods here.
};

extern CHARTPATTERNADDIN_ENG_API int nChartPatternAddin_Eng;

CHARTPATTERNADDIN_ENG_API int fnChartPatternAddin_Eng(void);

