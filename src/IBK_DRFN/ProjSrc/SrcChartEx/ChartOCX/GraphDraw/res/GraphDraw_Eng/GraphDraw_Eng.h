
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GRAPHDRAW_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GRAPHDRAW_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef GRAPHDRAW_ENG_EXPORTS
#define GRAPHDRAW_ENG_API __declspec(dllexport)
#else
#define GRAPHDRAW_ENG_API __declspec(dllimport)
#endif

// This class is exported from the GraphDraw_Eng.dll
class GRAPHDRAW_ENG_API CGraphDraw_Eng {
public:
	CGraphDraw_Eng(void);
	// TODO: add your methods here.
};

extern GRAPHDRAW_ENG_API int nGraphDraw_Eng;

GRAPHDRAW_ENG_API int fnGraphDraw_Eng(void);

