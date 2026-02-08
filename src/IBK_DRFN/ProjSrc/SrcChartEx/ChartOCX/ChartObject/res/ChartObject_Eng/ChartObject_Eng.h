
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTOBJECT_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTOBJECT_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTOBJECT_ENG_EXPORTS
#define CHARTOBJECT_ENG_API __declspec(dllexport)
#else
#define CHARTOBJECT_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartObject_Eng.dll
class CHARTOBJECT_ENG_API CChartObject_Eng {
public:
	CChartObject_Eng(void);
	// TODO: add your methods here.
};

extern CHARTOBJECT_ENG_API int nChartObject_Eng;

CHARTOBJECT_ENG_API int fnChartObject_Eng(void);

