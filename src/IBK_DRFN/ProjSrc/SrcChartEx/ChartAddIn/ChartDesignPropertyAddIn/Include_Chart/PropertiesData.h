// PropertiesData.h: interface for the CPropertiesData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIESDATA_H__1690F66F_1721_4210_A550_EC605AD3D1CD__INCLUDED_)
#define AFX_PROPERTIESDATA_H__1690F66F_1721_4210_A550_EC605AD3D1CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum CHART_PROPERTY_TYPE
{
	CLRBACKGROUND,						// 0
	CLRBACKBORDER,						// 1
	CLRBLOCK,							// 2
	CLRBLOCKBORDER,						// 3
	CLRSCALELINE,						// 4
	CLRSCALELINE2,						// 5
	CLRSCALETEXT,						// 6
	CLRGRAPHBASE1,						// 7
	CLRGRAPHBASE2,						// 8
	CLRGRAPHBASE3,						// 9
	CLRGRAPHBASE4,						// 10
	CLRGRAPHBASE5,						// 11
	CLRGRAPHBASE6,						// 12
	CLRANALYTICSTOOL,					// 13
	CLRINQUIRYWINDOW,					// 14
	TYPEBLOCKTYPE,						// 15
	TYPEBLOCKCOMPART,					// 16
	NBLOCKROWCOUNT,						// 17
	NBLOCKCOLUMNCOUNT,					// 18
	NBLOCKGAP,							// 19
	BLOCKBORDERTHICKNESS,				// 20
	NROWDOUBLESIZEBLOCK,				// 21
	NCOLUMNDOUBLESIZEBLOCK,				// 22
	CLRRATTICE,							// 23
	STRCHARTLIST,						// 24
	TYPEBORDER,							// 25
	TYPESCALELINE_NOT_USED,				// 26
	BWHOLEVIEW,							// 27
	NONEPAGEDATACOUNT,					// 28
	NUSERBLOCKADDCOUNT,					// 29
	BGRAPHDRAGDROP,						// 30
	NLEFTMARGIN,						// 31
	NRIGHTMARGIN,						// 32
	NTOPMARGIN,							// 33
	NBOTTOMMARGIN,						// 34
	BADDGRAPHMENUSHOW,					// 35
	STRENVIRONMENTFILENAME,				// 36
	STRUSERINDICATORLISTFILENAME,		// 37
	BHIDE,								// 38
	BISINPUT,							// 39
	STRTOOLBAROPTION,					// 40
	STRPACKETOPTION,					// 41
	STRLOCALSHAREDLIST,					// 42
	BUSEREALDATA,						// 43
	STRREALLIST,						// 44
	NMAGINBORDERTYPE,					// 45
	BUSEINQUIRYDLGWHENUSECROSSLINE,		// 46
	BUSESHORTKEY,						// 47
	BUSECONDITIONTOTITLE,				// 48
	BUSEDATATOTITLE,					// 49
	nHorzGap,							// 50
	nVertGap,							// 51
	nRButtonMenuType,					// 52
	bShowWarningMessageBox,				// 53
	bControlBlockSize,					// 54
	bBlockMoveDelete,					// 55
	bSelectTitle,						// 56
	bSelectGraph,						// 57
	bUseScaleRButtonMenu,				// 58
	nGraphRButtonType,					// 59
	nHoldingPacketSize,					// 60
	nMinDataCountOnOnePage,				// 61
	bYesOrNoRedrawbyReal,				// 62
	STRBASEPACKETNAME,					// 63
	bFIREREALEVENT,						// 64
	NUMERICALINQUIRYOPTION,				// 65
	STRGLOBALSHAREDLIST,				// 66
	USESKIN,							// 67
	MAPNAME,							// 68
	BONGCOUNTMARGIN,					// 69
	DISPLAYTRDATA,						// 70
	CUSTOMFONT,							// 71
	BLOCKREGION,						// 72
	ADDINLIST,							// 73
	MAPINDICATORINFO,					// 74
	LOCALLANGUAGETABLE,					// 75
//==============================================
	CHART_PROPERTY_COUNT				// 76
};


// (2006/8/25 - Seung-Won, Bae) Default Values for each site.
// Koscom
// 해당 site관련 Addin Define 삭제 - ojtaso (20090907)
//#if _ADDIN_KC

//#ifdef _SITE_SOL_
	#define _DPV_CLRBACKGROUND					RGB( 238, 238, 238)
	//#define _DPV_CLRBACKBORDER					RGB( 0,   0,   0)
	#define _DPV_CLRBACKBORDER					RGB( 88,   89,   95)	//@Solomon-091001
	#define _DPV_CLRBLOCK						RGB( 255, 255, 255)
	//#define _DPV_CLRBLOCKBORDER					RGB( 0,   0,   0)
	#define _DPV_CLRBLOCKBORDER					RGB( 88,   89,   95)	//@Solomon-091001
	#define _DPV_CLRSCALELINE					RGB( 128, 128, 128)
	#define _DPV_CLRSCALELINE2					RGB( 128, 128, 128)
	#define _DPV_CLRSCALETEXT					RGB( 0,   0,   0)
	#define _DPV_CLRGRAPHBASE1					RGB( 0,   136, 0)
	#define _DPV_CLRGRAPHBASE2					RGB( 255, 55,  255)
	#define _DPV_CLRGRAPHBASE3					RGB( 255, 51,  51)
	#define _DPV_CLRGRAPHBASE4					RGB( 0,   109, 219)
	#define _DPV_CLRGRAPHBASE5					RGB( 255, 133, 11)
	#define _DPV_CLRGRAPHBASE6					RGB( 128, 0,   255)
	#define _DPV_CLRANALYTICSTOOL				RGB( 0,   0,   0)
	#define _DPV_CLRINQUIRYWINDOW				RGB( 203, 228, 203)
	#define _DPV_TYPEBLOCKTYPE					0
	#define _DPV_TYPEBLOCKCOMPART				0
	#define _DPV_NBLOCKROWCOUNT					1
	#define _DPV_NBLOCKCOLUMNCOUNT				1
	#define _DPV_NBLOCKGAP						-1
	#define _DPV_BLOCKBORDERTHICKNESS			1
	#define _DPV_NROWDOUBLESIZEBLOCK			0
	#define _DPV_NCOLUMNDOUBLESIZEBLOCK			0
	#define _DPV_CLRRATTICE						RGB( 255, 255, 255)
	#define _DPV_STRCHARTLIST					""
	#define _DPV_TYPEBORDER						0
	#define _DPV_TYPESCALELINE_NOT_USED			-1								// (2007/7/2 - SKBng-Won, Bae) Do not Support Main Grid Type.			//	for Old Version Map and User File.
	#define _DPV_BWHOLEVIEW						FALSE
	#define _DPV_NONEPAGEDATACOUNT				120
	#define _DPV_NUSERBLOCKADDCOUNT				5
	#define _DPV_BGRAPHDRAGDROP					FALSE
	#define _DPV_NLEFTMARGIN					0
	#define _DPV_NRIGHTMARGIN					0
	#define _DPV_NTOPMARGIN						0
	#define _DPV_NBOTTOMMARGIN					0
	#define _DPV_BADDGRAPHMENUSHOW				FALSE
	#define _DPV_STRENVIRONMENTFILENAME			""
	#define _DPV_STRUSERINDICATORLISTFILENAME	""
	#define _DPV_BHIDE							FALSE
	#define _DPV_BISINPUT						FALSE
	#define _DPV_STRTOOLBAROPTION				""
	#define _DPV_STRPACKETOPTION				""
	#define _DPV_STRLOCALSHAREDLIST				""
	#define _DPV_BUSEREALDATA					FALSE
	#define _DPV_STRREALLIST					""
	#define _DPV_NMAGINBORDERTYPE				3
	#define _DPV_BUSEINQUIRYDLGWHENUSECROSSLINE	FALSE
	#define _DPV_BUSESHORTKEY					FALSE
	#define _DPV_BUSECONDITIONTOTITLE			TRUE
	#define _DPV_BUSEDATATOTITLE				FALSE
	#define _DPV_nHorzGap						0
	#define _DPV_nVertGap						0
	#define _DPV_nRButtonMenuType				1
	#define _DPV_bShowWarningMessageBox			TRUE
	#define _DPV_bControlBlockSize				TRUE
	#define _DPV_bBlockMoveDelete				TRUE
	#define _DPV_bSelectTitle					TRUE
	#define _DPV_bSelectGraph					TRUE
	#define _DPV_bUseScaleRButtonMenu			TRUE
	#define _DPV_nGraphRButtonType				1
	#define _DPV_nHoldingPacketSize				0
	#define _DPV_nMinDataCountOnOnePage			5
	#define _DPV_bYesOrNoRedrawbyReal			TRUE
	#define _DPV_STRBASEPACKETNAME				"#DateTime#"		// _MTEXT( C0_DATE_TIME), for Un-Initialized Time.
	#define _DPV_bFIREREALEVENT					FALSE
	#define _DPV_NUMERICALINQUIRYOPTION			4
	#define _DPV_STRGLOBALSHAREDLIST			""
	#define _DPV_USESKIN						FALSE
	#define _DPV_MAPNAME						""
	#define _DPV_BONGCOUNTMARGIN				0
	#define _DPV_DISPLAYTRDATA					FALSE
	#define _DPV_CUSTOMFONT						"굴림체;12;0;0;0;0;"
	#define _DPV_BLOCKREGION					""
	#define _DPV_ADDINLIST						"070112:DIRECT_REAL,ENV_SETUP,STYLE_DATA,DATA_VIEWER,ADD_INDICATOR_CMD,SAVE_IMAGE,RELPSA,KBD_SCROLL,DRAG_ZOOM,CROSS_DRAG,NIDLG_DRAG,TOOL_TIP,TIME_MARKER,ASKED_PRICE,OBJECT_TOOL,ST_INDICATOR,PERIOD_SUMMARY,SYMBOL_TOOL,ANALYSIS_TOOL,CROSS_TOOL:GLANCEBALANCE_CURSOR"
	#define _DPV_MAPINDICATORINFO				""
	#define _DPV_LOCALLANGUAGETABLE				""

//#else
/*
	#define _DPV_CLRBACKGROUND					RGB( 221, 221, 221)
	#define _DPV_CLRBACKBORDER					RGB(  64, 144, 222)
	#define _DPV_CLRBLOCK						RGB( 255, 255, 255)
	#define _DPV_CLRBLOCKBORDER					RGB( 187, 187, 187)
	#define _DPV_CLRSCALELINE					RGB( 187, 187, 187)
	#define _DPV_CLRSCALELINE2					RGB( 187, 187, 187)
	#define _DPV_CLRSCALETEXT					RGB(  68,  68,  68)
	#define _DPV_CLRGRAPHBASE1					RGB( 0,   136, 0)
	#define _DPV_CLRGRAPHBASE2					RGB( 255, 55,  255)
	#define _DPV_CLRGRAPHBASE3					RGB( 255, 51,  51)
	#define _DPV_CLRGRAPHBASE4					RGB( 0,   109, 219)
	#define _DPV_CLRGRAPHBASE5					RGB( 255, 133, 11)
	#define _DPV_CLRGRAPHBASE6					RGB( 128, 0,   255)
	#define _DPV_CLRANALYTICSTOOL				RGB( 0,   0,   0)
	#define _DPV_CLRINQUIRYWINDOW				RGB( 204, 255, 255)
	#define _DPV_TYPEBLOCKTYPE					0
	#define _DPV_TYPEBLOCKCOMPART				0
	#define _DPV_NBLOCKROWCOUNT					1
	#define _DPV_NBLOCKCOLUMNCOUNT				1
	#define _DPV_NBLOCKGAP						-1
	#define _DPV_BLOCKBORDERTHICKNESS			1
	#define _DPV_NROWDOUBLESIZEBLOCK			0
	#define _DPV_NCOLUMNDOUBLESIZEBLOCK			0
	#define _DPV_CLRRATTICE						RGB( 221, 221, 221)
	#define _DPV_STRCHARTLIST					""
	#define _DPV_TYPEBORDER						0
	#define _DPV_TYPESCALELINE_NOT_USED			-1								// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.			//	for Old Version Map and User File.
	#define _DPV_BWHOLEVIEW						FALSE
	#define _DPV_NONEPAGEDATACOUNT				120
	#define _DPV_NUSERBLOCKADDCOUNT				5
	#define _DPV_BGRAPHDRAGDROP					FALSE
	#define _DPV_NLEFTMARGIN     2
	#define _DPV_NRIGHTMARGIN     2
	#define _DPV_NTOPMARGIN      2
	#define _DPV_NBOTTOMMARGIN     3
	#define _DPV_BADDGRAPHMENUSHOW				FALSE
	#define _DPV_STRENVIRONMENTFILENAME			""
	#define _DPV_STRUSERINDICATORLISTFILENAME	""
	#define _DPV_BHIDE							FALSE
	#define _DPV_BISINPUT						FALSE
	#define _DPV_STRTOOLBAROPTION				""
	#define _DPV_STRPACKETOPTION				""
	#define _DPV_STRLOCALSHAREDLIST				""
	#define _DPV_BUSEREALDATA					FALSE
	#define _DPV_STRREALLIST					""
	#define _DPV_NMAGINBORDERTYPE				3
	#define _DPV_BUSEINQUIRYDLGWHENUSECROSSLINE	FALSE
	#define _DPV_BUSESHORTKEY					FALSE
	#define _DPV_BUSECONDITIONTOTITLE			TRUE
	#define _DPV_BUSEDATATOTITLE				FALSE
	#define _DPV_nHorzGap						0
	#define _DPV_nVertGap						0
	#define _DPV_nRButtonMenuType				1
	#define _DPV_bShowWarningMessageBox			TRUE
	#define _DPV_bControlBlockSize				TRUE
	#define _DPV_bBlockMoveDelete				TRUE
	#define _DPV_bSelectTitle					TRUE
	#define _DPV_bSelectGraph					TRUE
	#define _DPV_bUseScaleRButtonMenu			TRUE
	#define _DPV_nGraphRButtonType				1
	#define _DPV_nHoldingPacketSize				0
	#define _DPV_nMinDataCountOnOnePage			5
	#define _DPV_bYesOrNoRedrawbyReal			TRUE
	#define _DPV_STRBASEPACKETNAME				"#DateTime#"		// _MTEXT( C0_DATE_TIME), for Un-Initialized Time.
	#define _DPV_bFIREREALEVENT					FALSE
	#define _DPV_NUMERICALINQUIRYOPTION			0
	#define _DPV_STRGLOBALSHAREDLIST			""
	#define _DPV_USESKIN						FALSE
	#define _DPV_MAPNAME						""
	#define _DPV_BONGCOUNTMARGIN				0
	#define _DPV_DISPLAYTRDATA					TRUE
	#define _DPV_CUSTOMFONT						"굴림;-12;0;0;0;0;"
	#define _DPV_BLOCKREGION					""
	#define _DPV_ADDINLIST						"070112:DIRECT_REAL,ENV_SETUP,STYLE_DATA,DATA_VIEWER,ADD_INDICATOR_CMD,SAVE_IMAGE,RELPSA,KBD_SCROLL,DRAG_ZOOM,CROSS_DRAG,NIDLG_DRAG,TOOL_TIP,TIME_MARKER,ASKED_PRICE,OBJECT_TOOL,ST_INDICATOR,PERIOD_SUMMARY,SYMBOL_TOOL,ANALYSIS_TOOL,CROSS_TOOL:GLANCEBALANCE_CURSOR"
	#define _DPV_MAPINDICATORINFO				""
	#define _DPV_LOCALLANGUAGETABLE				""
#endif
*/

// (2004.04.13, 배승원) Global Variable을 Static을 이용하지 않고, 별도의 CPP에 정의하여
//		Module의 크기를 줄이도록 한다. (또한 중복 정의되지 않도록 Header Include Condition을 둔다.)
// (2004.06.08, 배승원) Global 공유 속성을 추가한다.
// (2006/7/5 - Seung-Won, Bae) Manage All Property with One Type (Merge Intialize Property and Property Page's)

// Property Names
extern char *g_aszPropertyName[ CHART_PROPERTY_COUNT];
// Property Short Key
extern char *g_anPropertyShortKey[ CHART_PROPERTY_COUNT];

// (2006/8/24 - Seung-Won, Bae) AddIn DLL Name Infos for Map
typedef enum CHART_ADDIN_TYPE
{
	AID_ENV_SETUP,				// 0
	AID_STYLE_DATA,				// 1
	AID_DIRECT_REAL,			// 2
	AID_DATA_VIEWER,			// 3
	AID_ADD_INDICATOR_CMD,		// 4
	AID_SAVE_IMAGE,				// 5
	AID_RELPSA,					// 6
	AID_KBD_SCROLL,				// 7
	AID_DRAG_ZOOM,				// 8
	AID_CROSS_DRAG,				// 9
	AID_NIDLG_DRAG,				// 10
	AID_TOOL_TIP,				// 11
	AID_TIME_MARKER,			// 12
	AID_ASKED_PRICE,			// 13
	AID_OBJECT_TOOL,			// 14
	AID_ST_INDICATOR,			// 15
	AID_SYMBOL_TOOL,			// 16
	AID_ANALYSIS_TOOL,			// 17
	AID_NIDLG_TOOL,				// 18
	AID_CROSS_TOOL,				// 19
	AID_NIDLG_OVER,				// 20	(Over Exclusive AddIn)
	AID_CROSS_OVER,				// 21	(Over Exclusive AddIn)
	AID_DRDS_REAL,				// 22
	AID_LOG,					// 23
	AID_DESIGN_PROPERTY,		// 24
	AID_GLANCEBALANCE_CURSOR,	// 25	(Over Exclusive AddIn)
	AID_MOUSE_POSITION,			// 26	(Over Exclusive AddIn)
	AID_PRICE_YSCALE_SETUP,		// 27
	AID_AUTO_SCROLL,			// 28
	AID_TOOL_ORDER_MANAGER,		// 29
	AID_DRAG_Y_ZOOM,			// 30
	AID_PAN,					// 31
	AID_UNDO_REDO_MANAGER,		// 32
	AID_INDICATORINFO_UNDO_REDO_MANAGER,		// 33
	AID_PERIOD_SUMMARY,			// 34
	AID_ORDER_LINE,				// 35
	AID_EXCEL_IMPORT,			// 36
//==============================================
	AID_COUNT					// 37
};
// AddIn ID String
extern char *g_aszAddInItemName[ AID_COUNT];
// AddIn Loading String
extern char *g_aszAddInLoadString[ AID_COUNT];
// AddIn Loading Order
#define _ADDIN_MARK_OVER_COUNT				4
#define _ADDIN_MARK_NO_ORDER				""
#define _ADDIN_MARK_KEYBOARD_ORDER			""
#define _ADDIN_MARK_MOUSE_AND_DRAW_ORDER	""
#define _ADDIN_MARK_OVER_EXCLUSIVE			""
#define _ADDIN_LOAD_ORDER	"070112:"								\
								_ADDIN_MARK_NO_ORDER				\
									"LOG,"							\
									"DIRECT_REAL,"					\
									"DRDS_REAL,"					\
									"ENV_SETUP,"					\
									"STYLE_DATA,"					\
									"DATA_VIEWER,"					\
									"ADD_INDICATOR_CMD,"			\
									"SAVE_IMAGE,"					\
									"RELPSA,"						\
									"PRICE_YSCALE_SETUP,"			\
									"AUTO_SCROLL,"					\
									"EXCEL_IMPORT,"					\
								_ADDIN_MARK_KEYBOARD_ORDER			\
									"KBD_SCROLL,"					\
								_ADDIN_MARK_MOUSE_AND_DRAW_ORDER	\
									"DRAG_ZOOM,"					\
									"CROSS_DRAG,"					\
									"NIDLG_DRAG,"					\
									"TOOL_TIP,"						\
									"TIME_MARKER,"					\
									"ASKED_PRICE,"					\
									"OBJECT_TOOL,"					\
									"ST_INDICATOR,"					\
									"PERIOD_SUMMARY,"				\
									"SYMBOL_TOOL,"					\
									"ANALYSIS_TOOL,"				\
									"ORDER_LINE,"					\
									"NIDLG_TOOL,"					\
									"CROSS_TOOL:"					\
									"DRAG_Y_ZOOM,"					\
									"PAN,"							\
								_ADDIN_MARK_OVER_EXCLUSIVE			\
									"NIDLG_OVER,"					\
									"CROSS_OVER,"					\
									"GLANCEBALANCE_CURSOR,"			\
									"MOUSE_POSITION"

#endif // !defined(AFX_PROPERTIESDATA_H__1690F66F_1721_4210_A550_EC605AD3D1CD__INCLUDED_)
	