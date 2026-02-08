
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHARTDATALISTVIEWERADDIN_ENG_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHARTDATALISTVIEWERADDIN_ENG_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHARTDATALISTVIEWERADDIN_ENG_EXPORTS
#define CHARTDATALISTVIEWERADDIN_ENG_API __declspec(dllexport)
#else
#define CHARTDATALISTVIEWERADDIN_ENG_API __declspec(dllimport)
#endif

// This class is exported from the ChartDataListViewerAddIn_Eng.dll
class CHARTDATALISTVIEWERADDIN_ENG_API CChartDataListViewerAddIn_Eng {
public:
	CChartDataListViewerAddIn_Eng(void);
	// TODO: add your methods here.
};

extern CHARTDATALISTVIEWERADDIN_ENG_API int nChartDataListViewerAddIn_Eng;

CHARTDATALISTVIEWERADDIN_ENG_API int fnChartDataListViewerAddIn_Eng(void);

