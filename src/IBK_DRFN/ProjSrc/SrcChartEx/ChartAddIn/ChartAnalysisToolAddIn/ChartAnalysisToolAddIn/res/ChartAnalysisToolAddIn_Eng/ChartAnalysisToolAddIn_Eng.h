
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTANALYSISTOOLADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTANALYSISTOOLADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTANALYSISTOOLADDIN_ENG_EXPORTS
#define CHARTANALYSISTOOLADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTANALYSISTOOLADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartAnalysisToolAddIn_Eng.dll
class CHARTANALYSISTOOLADDIN_ENG_API CChartAnalysisToolAddIn_Eng {
public:
	CChartAnalysisToolAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTANALYSISTOOLADDIN_ENG_API int nChartAnalysisToolAddIn_Eng;

CHARTANALYSISTOOLADDIN_ENG_API int fnChartAnalysisToolAddIn_Eng(void);

