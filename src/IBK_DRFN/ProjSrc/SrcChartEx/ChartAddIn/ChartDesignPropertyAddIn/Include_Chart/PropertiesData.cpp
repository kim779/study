// PropertiesData.cpp: implementation of the CPropertiesData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PropertiesData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.30
//
//	수정	:	(2003.12.30) Index값을 확인할 수 있도록 Comment를 추가한다.
///////////////////////////////////////////////////////////////////////////////

// Property Names
char *g_aszPropertyName[ CHART_PROPERTY_COUNT] =
{
	"clrBackground",					// 0
	"clrBackBorder",					// 1
	"clrBlock",							// 2
	"clrBlockBorder",					// 3
	"clrScaleLine",						// 4
	"clrScaleLine2",					// 5
	"clrScaleText",						// 6
	"clrGraphBase1",					// 7
	"clrGraphBase2",					// 8
	"clrGraphBase3",					// 9
	"clrGraphBase4",					// 10
	"clrGraphBase5",					// 11
	"clrGraphBase6",					// 12
	"clrAnalysisTool",					// 13
	"clrInquiryWindow",					// 14
	"nBlockType",						// 15
	"nBlockCompart",					// 16
	"nBlockRowCount",					// 17
	"nBlockColumnCount",				// 18
	"nBlockGap",						// 19
	"nBlockBorderThickness",			// 20
	"nRowDoubleSizeBlock",				// 21
	"nColumnDoubleSizeBlock",			// 22
	"clrLattice",						// 23
	"strChartList",						// 24
	"nBorderType",						// 25
	"nScaleLineType",					// 26	// Not Used
	"bWholeView",						// 27
	"nOnePageDataCount",				// 28
	"nUserBlockAddCount",				// 29
	"bGraphDragDrop",					// 30
	"nLeftMargin",						// 31
	"nRightMargin",						// 32
	"nTopMargin",						// 33
	"nBottomMargin",					// 34
	"bAddGraphMenuShow",				// 35
	"strUserEnvironmentFileName",		// 36
	"strUserIndicatorListFileName",		// 37
	"bHide",							// 38
	"bIsInput",							// 39
	"strToolbarOption",					// 40
	"strPacketOption",					// 41
	"strLocalSharedList",				// 42
	"bUseRealData",						// 43
	"strRealList",						// 44
	"nMarginBorderType",				// 45
	"bUseInquiryDlgWhenUseCrossLine",	// 46
	"bUseShortKey",						// 47
	"bUseConditiontoTitle",				// 48
	"bUseDatatoTitle",					// 49
	"nHorzGap",							// 50
	"nVertGap",							// 51
	"nRButtonMenuType",					// 52
	"bShowWarningMessageBox",			// 53
	"bControlBlockSize",				// 54
	"bBlockMoveDelete",					// 55
	"bSelectTitle",						// 56
	"bSelectGraph",						// 57
	"bUseScaleRButtonMenu",				// 58
	"nGraphRButtonType",				// 59
	"nHoldingPacketSize",				// 60
	"nMinDataCountOnOnePage",			// 61
	"bRedrawAfterUpdatedAllPackets",	// 62
	"szBasePacketName",					// 63
	"bFireRealEvent",					// 64
	"nNumericalInquiryOption",			// 65
	"strGlobalSharedList",				// 66
	"bDoesUseSkinColor",				// 67
	"strMapName",						// 68
	"nBongCountMargin",					// 69
	"bOnePageDataCountInTRCount",		// 70
	"CustomFont",						// 71
	"strBlockRegion",					// 72
	"strAddInLoadList",					// 73
	"strMapIndicatorInfo",				// 74
	"strLanguageTableFN",				// 75
};

// Property Short Key
char *g_anPropertyShortKey[ CHART_PROPERTY_COUNT] = 
{
	"CBKG",		// 0
	"CBKB",		// 1
	"CB",		// 2
	"CBB",		// 3
	"CSL",		// 4
	"CSL2",		// 5
	"CST",		// 6
	"CGB1",		// 7
	"CGB2",		// 8
	"CGB3",		// 9
	"CGB4",		// 10
	"CGB5",		// 11
	"CGB6",		// 12
	"CAT",		// 13
	"CIW",		// 14
	"BT",		// 15
	"BC",		// 16
	"ROWC",		// 17
	"CC",		// 18
	"BG",		// 19
	"BBT",		// 20
	"RDB",		// 21
	"CDB",		// 22
	"CLTI",		// 23
	"CL",		// 24
	"BDT",		// 25
	"SLT",		// 26		// Not Used
	"WV",		// 27
	"PDC",		// 28
	"BAC",		// 29
	"GDD",		// 30
	"LM",		// 31
	"RM",		// 32
	"TM",		// 33
	"BM",		// 34
	"AGM",		// 35
	"EFN",		// 36
	"IFN",		// 37
	"HI",		// 38
	"IR",		// 39
	"TO",		// 40
	"PO",		// 41
	"LSL",		// 42
	"URD",		// 43
	"RL",		// 44
	"MBT",		// 45
	"UIDWCL",	// 46
	"USK",		// 47
	"UCTT",		// 48
	"UDTT",		// 49
	"NHG",		// 50
	"NVG",		// 51
	"NRBMT",	// 52
	"BSWMB",	// 53
	"BCBS",		// 54
	"BBMD",		// 55
	"BST",		// 56
	"BSG",		// 57
	"BUSRBM",	// 58
	"NGRBT",	// 59
	"NHPS",		// 60
	"NMDCOOP",	// 61
	"BRAUAP",	// 62
	"SBPN",		// 63
	"BFRE",		// 64
	"NIO",		// 65
	"GSL",		// 66
	"USESKIN",	// 67
	"MAPNAME",	// 68
	"BONGMG",	// 69
	"PGCNTTR2",	// 70
	"CFT",		// 71
	"BR",		// 72
	"ADINL",	// 73
	"MINDI",	// 74
	"LLT",		// 75
};

///////////////////////////////////////////////////////////////////////////////
// (2006/8/24 - Seung-Won, Bae) AddIn DLL Name Infos for Map
///////////////////////////////////////////////////////////////////////////////
// AddIn ID String
char *g_aszAddInItemName[ AID_COUNT] = 
{
	"ENV_SETUP",			// 0
	"STYLE_DATA",			// 1
	"DIRECT_REAL",			// 2
	"DATA_VIEWER",			// 3
	"ADD_INDICATOR_CMD",	// 4
	"SAVE_IMAGE",			// 5
	"RELPSA",				// 6
	"KBD_SCROLL",			// 7
	"DRAG_ZOOM",			// 8
	"CROSS_DRAG",			// 9
	"NIDLG_DRAG",			// 10
	"TOOL_TIP",				// 11
	"TIME_MARKER",			// 12
	"ASKED_PRICE",			// 13
	"OBJECT_TOOL",			// 14
	"ST_INDICATOR",			// 15
	"SYMBOL_TOOL",			// 16
	"ANALYSIS_TOOL",		// 17
	"NIDLG_TOOL",			// 18
	"CROSS_TOOL",			// 19
	"NIDLG_OVER",			// 20
	"CROSS_OVER",			// 21
	"DRDS_REAL",			// 22
	"LOG",					// 23
	"DESIGN_PROPERTY",		// 24
	"GLANCEBALANCE_CURSOR",	// 25
	"MOUSE_POSITION",		// 26
	"PRICE_YSCALE_SETUP",	// 27
	"AUTO_SCROLL",			// 28
	"TOOL_ORDER_MANAGER",	// 29
	"DRAG_Y_ZOOM",			// 30
	"PAN",					// 31
	"UNDO_REDO_MANAGER",	// 32
	"INDICATORINFO_UNDO_REDO_MANAGER",	// 33
	"PERIOD_SUMMARY",		// 34
	"ORDER_LINE",			// 36
	"EXCEL_IMPORT"			// 37
};

// AddIn Loading String
char *g_aszAddInLoadString[ AID_COUNT] = 
{
	"ChartEnvSetupDlgAddIn.dll:ENV_SETUP",			// 0
	"ChartEnvSetupDlgAddIn.dll:STYLE_DATA",			// 1
	"ChartCommonAddIn.dll:DIRECT_REAL",				// 2
	"ChartDataListViewerAddIn.dll:DATA_VIEWER",		// 3
	"ChartCommonAddIn.dll:ADD_INDICATOR_CMD",		// 4
	"ChartCommonAddIn.dll:SAVE_IMAGE",				// 5
	"ChartCommonAddIn.dll:RELPSA",					// 6
	"ChartCommonAddIn.dll:KBD_SCROLL",				// 7
	"ChartCommonAddIn.dll:DRAG_ZOOM",				// 8
	"ChartAnalysisToolAddIn.dll:CROSS_DRAG",		// 9
	"ChartAnalysisToolAddIn.dll:NIDLG_DRAG",		// 10
	"ChartAnalysisToolAddIn.dll:TOOL_TIP",			// 11
	"ChartCommonAddIn.dll:TIME_MARKER",				// 12
	"ChartCommonAddIn.dll:ASKED_PRICE",				// 13
	"ChartCommonAddIn.dll:OBJECT_TOOL",				// 14
	"ChartSTIndexAddin.dll",						// 15
	"ChartAnalysisToolAddIn.dll:SYMBOL_TOOL",		// 16
	"ChartAnalysisToolAddIn.dll:ANALYSIS_TOOL",		// 17
	"ChartAnalysisToolAddIn.dll:NIDLG_TOOL",		// 18
	"ChartAnalysisToolAddIn.dll:CROSS_TOOL",		// 19
	"ChartAnalysisToolAddIn.dll:NIDLG_OVER",		// 20
	"ChartAnalysisToolAddIn.dll:CROSS_OVER",		// 21
	"ChartCommonAddIn.dll:DRDS_REAL",				// 22
	"ChartCommonAddIn.dll:LOG",						// 23
	"ChartDesignPropertyAddIn.dll:DESIGN_PROPERTY",	// 24
	"ChartCommonAddIn.dll:GLANCEBALANCE_CURSOR",	// 25
	"ChartCommonAddIn.dll:MOUSE_POSITION",			// 26
	"ChartCommonAddIn.dll:PRICE_YSCALE_SETUP",		// 27
	"ChartCommonAddIn.dll:AUTO_SCROLL",				// 28
	"ChartCommonAddIn.dll:TOOL_ORDER_MANAGER",		// 29
	"ChartCommonAddIn.dll:DRAG_Y_ZOOM",				// 30
	"ChartCommonAddIn.dll:PAN",						// 31
	"ChartCommonAddIn.dll:UNDO_REDO_MANAGER",		// 32
	"ChartCommonAddIn.dll:INDICATORINFO_UNDO_REDO_MANAGER",	// 33
	"ChartPeriodSummaryAddIn.dll:PERIOD_SUMMARY",	// 34
	"ChartPeriodSummaryAddIn.dll:ORDER_LINE",		// 35
	"ChartDataListViewerAddIn.dll:EXCEL_IMPORT"		// 36
};
