
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ANALYSISTOOL_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ANALYSISTOOL_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef ANALYSISTOOL_ENG_EXPORTS
#define ANALYSISTOOL_ENG_API __declspec(dllexport)
#else
#define ANALYSISTOOL_ENG_API __declspec(dllimport)
#endif

// This class is exported from the AnalysisTool_Eng.dll
class ANALYSISTOOL_ENG_API CAnalysisTool_Eng {
public:
	CAnalysisTool_Eng(void);
	// TODO: add your methods here.
};

extern ANALYSISTOOL_ENG_API int nAnalysisTool_Eng;

ANALYSISTOOL_ENG_API int fnAnalysisTool_Eng(void);

