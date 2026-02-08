
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTFORMULATE_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTFORMULATE_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTFORMULATE_ENG_EXPORTS
#define CHARTFORMULATE_ENG_API __declspec(dllexport)
#else
#define CHARTFORMULATE_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartFormulate_Eng.dll
class CHARTFORMULATE_ENG_API CChartFormulate_Eng {
public:
	CChartFormulate_Eng(void);
	// TODO: add your methods here.
};

extern CHARTFORMULATE_ENG_API int nChartFormulate_Eng;

CHARTFORMULATE_ENG_API int fnChartFormulate_Eng(void);

