
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTENVSETUPDLGADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTENVSETUPDLGADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTENVSETUPDLGADDIN_ENG_EXPORTS
#define CHARTENVSETUPDLGADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTENVSETUPDLGADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartEnvSetupDlgAddIn_Eng.dll
class CHARTENVSETUPDLGADDIN_ENG_API CChartEnvSetupDlgAddIn_Eng {
public:
	CChartEnvSetupDlgAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTENVSETUPDLGADDIN_ENG_API int nChartEnvSetupDlgAddIn_Eng;

CHARTENVSETUPDLGADDIN_ENG_API int fnChartEnvSetupDlgAddIn_Eng(void);

