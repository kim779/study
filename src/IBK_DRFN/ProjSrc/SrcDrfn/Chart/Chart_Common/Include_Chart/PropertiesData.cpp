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
//	Initialize Properties Name
char *pInitPropertiesName[] = \
{
	"clrBackground",			// 0
	"clrBackBorder",			// 1
	"clrBlock",					// 2
	"clrBlockBorder",			// 3
	"clrScaleLine",				// 4
	"clrScaleLine2",			// 5
	"clrScaleText",				// 6
	"clrGraphBase1",			// 7
	"clrGraphBase2",			// 8
	"clrGraphBase3",			// 9
	"clrGraphBase4",			// 10
	"clrGraphBase5",			// 11
	"clrGraphBase6",			// 12
	"clrAnalysisTool",			// 13
	"clrInquiryWindow",			// 14
	"nBlockType",				// 15
	"nBlockCompart",			// 16
	"nBlockRowCount",			// 17
	"nBlockColumnCount",		// 18
	"nBlockGap",				// 19
	"nBlockBorderThickness",	// 20
	"nRowDoubleSizeBlock",		// 21
	"nColumnDoubleSizeBlock",	// 22
	"clrLattice"				// 23
};

// (2003.11.12, 배승원) Index값을 확인할 수 있도록 Comment를 추가한다.
//	Property Page Properties Name
char *pPropertiesListName[] =
{
	"strChartList",						// 0
	"nBorderType",						// 1
	"nScaleLineType",					// 2
	"bWholeView",						// 3
	"nOnePageDataCount",				// 4
	"nUserBlockAddCount",				// 5
	"bGraphDragDrop",					// 6
	"nLeftMargin",						// 7
	"nRightMargin",						// 8
	"nTopMargin",						// 9
	"nBottomMargin",					// 10
	"bAddGraphMenuShow",				// 11
	"strUserEnvironmentFileName",		// 12
	"strUserIndicatorListFileName",		// 13
	"strUserSeriesListFileName",		// 14
	"bHide",							// 15
	"bIsInput",							// 16
	"strToolbarOption",					// 17
	"strPacketOption",					// 18
	"strLocalSharedList",				// 19 (Not used in Hanwha)
	"bUseRealData",						// 20
	"strRealList",						// 21
	"nMarginBorderType",				// 22
	"bUseInquiryDlgWhenUseCrossLine",	// 23
	"bUseShortKey",						// 24
	"bUseConditiontoTitle",				// 25
	"bUseDatatoTitle",					// 26
	"nHorzGap",							// 27
	"nVertGap",							// 28
	"nRButtonMenuType",					// 29
	"bShowWarningMessageBox",			// 30
	"bControlBlockSize",				// 31
	"bBlockMoveDelete",					// 32
	"bSelectTitle",						// 33
	"bSelectGraph",						// 34
	"bUseScaleRButtonMenu",				// 35
	"nGraphRButtonType",				// 36
	"nHoldingPacketSize",				// 37
	"nMinDataCountOnOnePage",			// 38
	"bRedrawAfterUpdatedAllPackets",	// 39
	"szBasePacketName",					// 40
	"bFireRealEvent",					// 41
	"nNumericalInquiryOption",			// 42
	"strGlobalSharedList",				// 43 (Not used in Hanwha)
	"bDoesUseSkinColor",				// 44 //sy 2005.04.26.
	"strShareInfoListHW",				// 45 (2005/8/9 - Seung-Won, Bae) Used in only Hanwha for Data Share
	"strMapName",						// 46 //sy 2005.09.06.
	"nBongCountMargin",					// 47 //sy 2005.09.06.
	"bOnePageDataCountInTRCount",		// 48 //sy 2005.12.08.
	"bIsMapDefault"						// 49 //sy 2006.02.20.
};

//	Initialize Properties Default Value
char *pInitPropertiesValue[] = \
{	"255,255,255","0,0,0","255,255,255","0,0,0","128,128,128",			\
	"128,128,128","0,0,0","0,0,0","0,0,0","0,0,0",						\
	"0,0,0","0,0,0","0,0,0","0,0,0","203,228,203",						\
	"0","2","1","1","0",												\
	"1","1","1", "255,255,255"											\
};

//	Property Page Properties Default Value
// -> 갯수 증가 시 <PPG_PROPERTIES_NUM> 수 증가
char *pPropertiesListValue[] = \
{
	"",			// 0	
	"0",		// 1		
	"0",		// 2		
	"FALSE",	// 3			
	"120",		// 4		
	"4",		// 5		
	"FALSE",	// 6			
	"0",		// 7		
	"0",		// 8		
	"0",		// 9		
	"0",		// 10		
	"FALSE",	// 11			
	"",			// 12	
	"",			// 13	
	"",			// 14	
	"FALSE",	// 15			
	"FALSE",	// 16			
	"",			// 17	
	"",			// 18	
	"",			// 19 (Not Used in Hanwha)
	"FALSE",	// 20			
	"",			// 21	
	"3",		// 22		
	"FALSE",	// 23			
	"FALSE",	// 24			
	"FALSE",	// 25			
	"FALSE",	// 26			
	"0",		// 27		
	"0",		// 28		
	"0",		// 29		
	"FALSE",	// 30			
	"FALSE",	// 31			
	"FALSE",	// 32			
	"FALSE",	// 33			
	"FALSE",	// 34			
	"FALSE",	// 35			
	"1",		// 36		
	"0",		// 37		
	"5",		// 38		
	"TRUE",		// 39		
	"자료일자",	// 40			
	"FALSE",	// 41			
	"0",		// 42		
	"",			// 43 (Not Used in Hanwha)
	"FALSE",	// 44 //sy 2005.04.26.
	"",			// 45 (2005/8/9 - Seung-Won, Bae) Used in only Hanwha for Data Share
	"",			// 46 //sy 2005.09.06.
	"0",		// 47 //sy 2005.09.06.
	"FALSE",	// 48 //sy 2005.12.08.
	"FALSE"		// 49 //sy 2006.02.20.
};

//	Initialize Properties Style
int pInitPropertiesStyle[] =	\
{
	STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,	\
	STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,	\
	STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,STYLE_COLOR,	\
	STYLE_INT,STYLE_INT,STYLE_INT,STYLE_INT,STYLE_INT,				\
	STYLE_INT,STYLE_INT,STYLE_INT,STYLE_COLOR						\
};

//	Property Page Properties Style
int pPropertiesListStyle[] =	\
{
	STYLE_STRING,	// 0				
	STYLE_INT,		// 1			
	STYLE_INT,		// 2			
	STYLE_BOOL,		// 3			
	STYLE_INT,		// 4			
	STYLE_INT,		// 5			
	STYLE_BOOL,		// 6			
	STYLE_INT,		// 7			
	STYLE_INT,		// 8			
	STYLE_INT,		// 9			
	STYLE_INT,		// 10			
	STYLE_BOOL,		// 11			
	STYLE_STRING,	// 12				
	STYLE_STRING,	// 13				
	STYLE_STRING,	// 14				
	STYLE_BOOL,		// 15			
	STYLE_BOOL,		// 16			
	STYLE_STRING,	// 17				
	STYLE_STRING,	// 18				
	STYLE_STRING,	// 19 (Not Used in Hanwha)
	STYLE_BOOL,		// 20			
	STYLE_STRING,	// 21				
	STYLE_INT,		// 22			
	STYLE_BOOL,		// 23			
	STYLE_BOOL,		// 24			
	STYLE_BOOL,		// 25			
	STYLE_BOOL,		// 26			
	STYLE_INT,		// 27			
	STYLE_INT,		// 28			
	STYLE_INT,		// 29			
	STYLE_BOOL,		// 30			
	STYLE_BOOL,		// 31			
	STYLE_BOOL,		// 32			
	STYLE_BOOL,		// 33			
	STYLE_BOOL,		// 34			
	STYLE_BOOL,		// 35			
	STYLE_INT,		// 36			
	STYLE_INT,		// 37			
	STYLE_INT,		// 38			
	STYLE_BOOL,		// 39			
    STYLE_STRING,	// 40				
	STYLE_BOOL,		// 41			
	STYLE_INT,		// 42					
	STYLE_STRING,	// 43 (Not Used in Hanwha)
	STYLE_BOOL,		// 44 //sy 2005.04.26.
	STYLE_STRING,	// 45 (2005/8/9 - Seung-Won, Bae) Used in only Hanwha for Data Share
	STYLE_STRING,	// 46 //sy 2005.09.06.
	STYLE_INT,		// 47 //sy 2005.09.06.
	STYLE_BOOL,		// 48 //sy 2005.12.08.
	STYLE_BOOL		// 49 //sy 2006.02.20.
};

// (2004.01.13, 배승원) Index값을 확인할 수 있도록 Comment를 추가한다.
char *pInitPropertiesShortKey[] =
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
	"ROWC",		// 17 (주의, GetAllProperties2()를 제공하기 위하여 신 Version에서는 ROWC를 이용한다.)
	"CC",		// 18
	"BG",		// 19
	"BBT",		// 20
	"RDB",		// 21
	"CDB",		// 22
	"CLTI"		// 23
};

char *pPropertiesShortKey[] = \
{
	"CL",		// 0			
	"BDT",		// 1			
	"SLT",		// 2			
	"WV",		// 3			
	"PDC",		// 4			
	"BAC",		// 5			
	"GDD",		// 6			
	"LM",		// 7			
	"RM",		// 8			
	"TM",		// 9			
	"BM ",		// 10			
	"AGM",		// 11			
	"EFN",		// 12			
	"IFN",		// 13			
	"SFN",		// 14			
	"HI",		// 15			
	"IR",		// 16			
	"TO",		// 17			
	"PO",		// 18			
	"LSL",		// 19 (Not Used in Hanwha)
	"URD",		// 20			
	"RL",		// 21			
	"MBT",		// 22			
	"UIDWCL",	// 23				
	"USK",		// 24			
	"UCTT",		// 25			
	"UDTT",		// 26			
	"NHG",		// 27			
	"NVG",		// 28			
	"NRBMT",	// 29				
	"BSWMB",	// 30				
	"BCBS",		// 31			
	"BBMD",		// 32			
	"BST",		// 33			
	"BSG",		// 34			
	"BUSRBM",	// 35				
	"NGRBT",	// 36				
	"NHPS",		// 37			
	"NMDCOOP",	// 38				
	"BRAUAP",	// 39				
	"SBPN",		// 40			
	"BFRE",		// 41			
	"NIO",		// 42												
	"GSL",		// 43 (Not Used in Hanwha)
	"USESKIN",	// 44 //sy 2005.04.26.
	"SLHW",		// 45 (2005/8/9 - Seung-Won, Bae) Used in only Hanwha for Data Share
	"MAPNAME",	// 46 //sy 2005.09.06.
	"BONGMG",	// 47 //sy 2005.09.06.
	"PGCNTTR",	// 48 //sy 2005.12.08.
	"MAPDEF"	// 49 //sy 2006.02.20.
};

char *pEtcPropertiesShortKey[] = \
{	"CFT", "BR"	\
};