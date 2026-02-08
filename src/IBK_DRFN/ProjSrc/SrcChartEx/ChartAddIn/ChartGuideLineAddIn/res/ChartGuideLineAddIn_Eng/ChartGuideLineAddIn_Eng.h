
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTGUIDELINEADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTGUIDELINEADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTGUIDELINEADDIN_ENG_EXPORTS
#define CHARTGUIDELINEADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTGUIDELINEADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartGuideLineAddIn_Eng.dll
class CHARTGUIDELINEADDIN_ENG_API CChartGuideLineAddIn_Eng {
public:
	CChartGuideLineAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTGUIDELINEADDIN_ENG_API int nChartGuideLineAddIn_Eng;

CHARTGUIDELINEADDIN_ENG_API int fnChartGuideLineAddIn_Eng(void);

