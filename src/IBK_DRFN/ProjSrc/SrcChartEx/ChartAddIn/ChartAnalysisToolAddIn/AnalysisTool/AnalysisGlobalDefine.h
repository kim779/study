#ifndef __ANALYSISGLOBALDEFINE__
#define __ANALYSISGLOBALDEFINE__

// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
#define _GB_VALUE_COUNT		12

const double G_POSITIVEMAX					= 1E37f;				
const double G_NEGATIVEMAX					= -1E37f;
const double G_NOTAVAILABLE					= DBL_MIN;
const float G_NOTAVAILABLE_FLOAT			= FLT_MIN;		
const int G_NOTAVAILABLE_INT				= INT_MIN;
const TCHAR *const G_ANALYSIS_PROPERTY_INFO	= "AnalysisPropInfo.ans";

//////////////////////////////////////////////////////////////////////////////
//--------------------색상 값 정의-------------------//
/*--------------------------------------------------------------------------*/
#define     G_COLOR_BLACK					RGB(  0,   0,   0)
#define     G_COLOR_LIGHTBLACK				RGB(  45,   45,   45)
#define     G_COLOR_WHITE					RGB(255, 255, 255)
#define     G_COLOR_RED						RGB(255,   0,   0)
#define		G_COLOR_DKRED					RGB(128, 0, 0)
#define		G_COLOR_GREEN					RGB(0, 255, 0)
#define		G_COLOR_GREEN2					RGB(  0, 106,   0)
#define		G_COLOR_GREEN3					RGB(  5, 205,  30)
#define     G_COLOR_LTGREEN					RGB(200,255,200)
#define		G_COLOR_DKGREEN					RGB(0, 128, 0)
#define		G_COLOR_PURPLE					RGB(56,0,56)
#define		G_COLOR_SILVER					RGB(137,137,137)
#define     G_COLOR_YELLOW					RGB(255, 255,   0)
#define		G_COLOR_DKYELLOW				RGB(128, 128, 0)
#define     G_COLOR_BLUE					RGB(  0,   0, 255)
#define		G_COLOR_DKBLUE					RGB(0, 0, 128)
#define		G_COLOR_CLOUDBLUE				RGB(128, 184, 223)
#define		G_COLOR_MAGENTA					RGB(255, 0, 255)
#define		G_COLOR_LTMAGENTA				RGB(255, 196, 255)
#define		G_COLOR_DKMAGENTA				RGB(128, 0, 128)
#define		G_COLOR_CYAN					RGB(0, 255, 255)
//#define	G_COLOR_AQUA					RGB(0,255,255)
#define		G_COLOR_DKCYAN					RGB(0, 128, 128)
#define     G_COLOR_GRAY					RGB(220, 220, 220)//192
#define     G_COLOR_BGRAY					RGB(152, 152, 152)
#define		G_COLOR_DKGRAY					RGB(128, 128, 128)
#define		G_COLOR_LTGRAY					RGB(230, 230, 255)//190
#define     G_COLOR_KBJ						RGB(  0, 192, 192)
#define     G_COLOR_MAROON					RGB(56,0,0)
#define     G_COLOR_FUSCHIA					RGB(255,0,255)
#define     G_COLOR_TEAL					RGB(0,56,56)
#define		G_COLOR_OLIVE					RGB(56,56,0)
#define		G_COLOR_NAVY					RGB(0,0,56)
#define		G_COLOR_JUHANG					RGB(255, 128,   0)
#define		G_COLOR_BORA					RGB(128,   0, 255)
#define		G_COLOR_HWANG					RGB(128, 128,  64)
#define		G_COLOR_PINGCU					RGB(255, 128, 192)
#define		G_COLOR_CHANG					RGB( 64, 128, 128)
//#define	G_COLOR_LIME					RGB(0,255,0)
//--:--------------------색상 값 정의-------------------//

typedef struct tagST_ANALYSIS_TOOL_INFO
{
	double dStartXPos;
	double dEndXPos;
	double dThirdXPos;

	double dStartYValue;
	double dEndYValue;
	double dThirdYValue;

	int nLeftText;
	int nRightText;
	int nRateText;
	int nTopText;
	int nBottomText;
	
	int nLeftExt;
	int nRightExt;
	int nEquiDiv;
	int	nPriceRangeDiv;		// (2008/8/15 - Seung-Won, Bae) Add option for using price range.
	int nHLPercent;

	int nFillRegion;
	
	int nLineStyle;
	int nLineWeight;
	COLORREF crLineColor;

	CString		strFiboCoefficient;		// 2008.08.20 김진순 피보나치 확장

} ST_ANALYSIS_TOOL_INFO,*LPST_ANALYSIS_TOOL_INFO;


#endif