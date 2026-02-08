
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the KTBCHART_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// KTBCHART_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef KTBCHART_ENG_EXPORTS
#define KTBCHART_ENG_API __declspec(dllexport)
#else
#define KTBCHART_ENG_API __declspec(dllimport)
#endif

// This class is exported from the KTBChart_Eng.dll
class KTBCHART_ENG_API CKTBChart_Eng {
public:
	CKTBChart_Eng(void);
	// TODO: add your methods here.
};

extern KTBCHART_ENG_API int nKTBChart_Eng;

KTBCHART_ENG_API int fnKTBChart_Eng(void);

