
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SYMBOL_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SYMBOL_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SYMBOL_ENG_EXPORTS
#define SYMBOL_ENG_API __declspec(dllexport)
#else
#define SYMBOL_ENG_API __declspec(dllimport)
#endif

// This class is exported from the Symbol_Eng.dll
class SYMBOL_ENG_API CSymbol_Eng {
public:
	CSymbol_Eng(void);
	// TODO: add your methods here.
};

extern SYMBOL_ENG_API int nSymbol_Eng;

SYMBOL_ENG_API int fnSymbol_Eng(void);

