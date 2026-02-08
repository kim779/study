
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTPERIODSUMMARYADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTPERIODSUMMARYADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTPERIODSUMMARYADDIN_ENG_EXPORTS
#define CHARTPERIODSUMMARYADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTPERIODSUMMARYADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartPeriodSummaryAddIn_Eng.dll
class CHARTPERIODSUMMARYADDIN_ENG_API CChartPeriodSummaryAddIn_Eng {
public:
	CChartPeriodSummaryAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTPERIODSUMMARYADDIN_ENG_API int nChartPeriodSummaryAddIn_Eng;

CHARTPERIODSUMMARYADDIN_ENG_API int fnChartPeriodSummaryAddIn_Eng(void);

