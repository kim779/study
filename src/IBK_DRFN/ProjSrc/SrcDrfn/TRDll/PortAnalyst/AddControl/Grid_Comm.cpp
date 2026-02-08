// CodeGrid.cpp: implementation of the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid_Comm.h"

#include "../resource.h"
#include "../DefineFiles/client_typedef.h"
#include "../../Dr_Include/CompanySS.h"
#include "../../../DrCommon/Dr_Control/DrXLSFile.h"
#include "../AnalystResultWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		HEADNAME_CODE[] = { _T("∨"), _T("종목명"), _T("현재가"), _T("대비"), _T("등락율") };
const		int			HEADWIDTH_CODE[] = { 19, 80, 65, 70, 50 };

const		LPCTSTR		HEADNAME_RESULT[] = { _T("기준가"), _T("상승"), _T("보합"), _T("하락"), _T("전체"),
										_T("상승종목비율"), _T("기준대비율") };
const		int			HEADWIDTH_RESULT[] = { 70, 30, 30, 30, 30, 80, 60 };

const		LPCTSTR		HEADNAME_PROFIT[] = { _T("일자"), _T("시간"), _T("평균 등락율"), _T("종목 등락율"), _T("누적"),
										_T("상승수"), _T("하락수"), _T("상승종목비율"),  };
const		int			HEADWIDTH_PROFIT[] = { 65, 36, 60, 60, 40, 30, 30, 70 };


CString DEFVAL_SELECTCHK = "1";

#define ROW_DEFHEIGHT									18
#define ROW_HEAD_DEFHEIGHT						20

const   COLORREF	clrHeader		 = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine	 = COLOR_GRID_LINE;

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
#define BLACK		RGB(0,0,0)

#define COL_CHECK					0
#define COL_NAME					1
#define COL_PRICE					2
#define COL_RATE					3
#define COL_CHANGERATE		4

#define VIEW_COL_CNT	3

//# 삼성증권
#define	UM_CHANGE_PARENT			(WM_USER + 450)		// 독립실행시 프레임 윈도우 변경을 통지하기위해...

BEGIN_MESSAGE_MAP(CGrid_Comm,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
	//# 삼성증권
	ON_MESSAGE(UM_CHANGE_PARENT,	OnChangeChildFrameWindowHandle)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid_Comm::CGrid_Comm()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	m_clrHeader  = COLOR_GRID_LINE;
	m_clrSelRow  = COLOR_GRID_SEL_ROW;

	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;
	
	m_nViewCol = 2;
	
	//# One Col Hide
	m_nCol_Hide = -1;
	m_bCol_HideState = FALSE;
	
	m_bCallAdjustOneTime = FALSE;
	m_bNoUserSorting = FALSE;
}

CGrid_Comm::~CGrid_Comm()
{
	m_fontDefault.DeleteObject();
	m_penGridLine.DeleteObject();
}

void CGrid_Comm::OnDestroy()
{
	m_tipInfo.RemoveTool(this);
}

void CGrid_Comm::ClearAll()
{
	m_nLastSelRow = -1;
	
	for (int nCount = 0; nCount < GetNumberRows(); nCount++)
		RemoveDataSource(nCount);
	
	while(GetNumberRows()) 
	{
		m_GI->m_defDataSource->DeleteRow(0);
		m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}
	
	QuickSetCellType(m_nSortCol, -1, 0);
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortCol = -1;
	
	m_bSelectCheckFlag = (DEFVAL_SELECTCHK == "1") ? TRUE : FALSE;
	
	SetNumberRows(0);
	RedrawAll();
}

BOOL CGrid_Comm::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}

void CGrid_Comm::OnSetup()
{
	
}

void CGrid_Comm::SetInit(CString strType)
{
	SetSH_Width(0);
	SetSH_NumberCols(0);
	
	SetDefRowHeight(ROW_DEFHEIGHT);
	SetTH_Height(ROW_HEAD_DEFHEIGHT);
	
	SetCancelMode(FALSE);
	EnableMenu(TRUE);
	
	EnableUpdate(TRUE);
	UseHints(TRUE);
	
	m_tipInfo.Create(this, "BASE", 1);
	m_tipInfo.SetFont(&m_fontDefault);
	m_tipInfo.AddTool(this, (LPCTSTR)" ");
	
	EnableExcelBorders(FALSE);
	
	if(strType != "RESULT")
		SetVScrollMode(UG_SCROLLTRACKING);
	SetHScrollMode(UG_SCROLLNORMAL);
	
	HeaderSetup(strType);
	
	SetHighlightRow(TRUE);
	SetUserSizingMode(FALSE);
	AdjustComponentSizes();
	
	CWnd *pwndChildFrm = GetFrameWnd(this);
	m_hChildFrame = pwndChildFrm->GetSafeHwnd();
}

long CGrid_Comm::OnChangeChildFrameWindowHandle(WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;
	
	return 0L;
}

CWnd* CGrid_Comm::GetFrameWnd(CWnd* pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void CGrid_Comm::HeaderSetup(CString strType)
{
	ClearAll();
	
	int nColCnt;
	if (strType == "CODE")
		nColCnt = _countof(HEADNAME_CODE);
	else if (strType == "RESULT")
		nColCnt = _countof(HEADNAME_RESULT);
	else if (strType == "PROFIT")
		nColCnt = _countof(HEADNAME_PROFIT);
	
	SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		
		if (strType == "CODE")
		{
			QuickSetText(nCol, -1, HEADNAME_CODE[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_CODE[nCol]));
		}
		else if (strType == "RESULT")
		{
			QuickSetText(nCol, -1, HEADNAME_RESULT[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_RESULT[nCol]));
		}
		else if (strType == "PROFIT")
		{
			QuickSetText(nCol, -1, HEADNAME_PROFIT[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_PROFIT[nCol]));
		}
		
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}
	
	m_strType = strType;
	
	RedrawAll();
}


void CGrid_Comm::SetAddRow(BOOL bChk, int nRow, BaseData_Code stBaseData, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	
	int nCol;
	char *pchData;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = (int)COL_CHECK;
	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
	//QuickSetText(nCol, nRow, DEFVAL_SELECTCHK);
	QuickSetText(nCol, nRow, (bChk == TRUE) ? "1" : "0");
	QuickSetBackColor(nCol, nRow, clrBk);
	
	nCol = (int)COL_NAME;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sName, sizeof(stBaseData.sName), strTemp);
	strTemp.TrimLeft(" ");
	strTemp.TrimRight(" ");
	cell.SetText(strTemp);
	//  cell.SetOtherDataCellText(strTemp);
	
	STR_CONVERT(pchData, stBaseData.sCode, sizeof(stBaseData.sCode), strTemp);
	cell.SetOtherJongCode(strTemp);
	
	STR_CONVERT(pchData, stBaseData.sMarket, sizeof(stBaseData.sMarket), strTemp);
	cell.SetOtherDataInt(atoi(strTemp));
	
	//# Data Index : 2006.10
	if (bChk == TRUE)
		cell.SetOtherDataLong(nRow);
	else
		cell.SetOtherDataLong(-1);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	COLORREF clrContrastText;
	char chAttribute;
	memcpy(&chAttribute, &stBaseData.sSign, sizeof(stBaseData.sSign));
	GetAttributeInfo(nRow, chAttribute, clrContrastText);
	
	nCol = (int)COL_PRICE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sPrice, sizeof(stBaseData.sPrice), strTemp);
	cell.SetOtherDataCellText(strTemp);
	strTemp = GetFormatStr(atol(strTemp), 0, FALSE, "");
	cell.SetText(strTemp);
	cell.SetOtherDataInt(atoi(&chAttribute));
	if ((chAttribute == '4') || (chAttribute == '5'))
		cell.SetOtherDataLong(atol(strTemp) * -1);
	else
		cell.SetOtherDataLong(atol(strTemp));
	
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	
	nCol = (int)COL_RATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sChange, sizeof(stBaseData.sChange), strTemp);
	strTemp = GetFormatStr(abs(atol(strTemp)), 0, FALSE, "");
	if (atol(strTemp) < 0)
		strTemp = "-" + strTemp;
	
	cell.SetSign(atoi(&chAttribute));
	cell.SetText(strTemp);
	cell.SetOtherDataLong((long)(atof(strTemp) * 100.0));
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	
	nCol = (int)COL_CHANGERATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sChgRate, sizeof(stBaseData.sChgRate), strTemp);
	strTemp.TrimLeft(" ");
	if (atof(strTemp) == 0)
	{
		
	}
	else if ((long)(atof(strTemp) * 100.0) > 0)
		strTemp = "+" + strTemp;
	cell.SetText(strTemp);
	cell.SetOtherDataLong((long)(atof(strTemp) * 100.0));
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
}


void CGrid_Comm::SetAddRow(BOOL bChk, BaseData_Code stBaseData, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	
	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol;
	
	char *pchData;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = (int)COL_CHECK;
	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
	//QuickSetText(nCol, nRow, DEFVAL_SELECTCHK);
	QuickSetText(nCol, nRow, (bChk == TRUE) ? "1" : "0");
	QuickSetBackColor(nCol, nRow, clrBk);
	
	nCol = (int)COL_NAME;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sName, sizeof(stBaseData.sName), strTemp);
	strTemp.TrimLeft(" ");
	strTemp.TrimRight(" ");
	cell.SetText(strTemp);
	//  cell.SetOtherDataCellText(strTemp);
	
	STR_CONVERT(pchData, stBaseData.sCode, sizeof(stBaseData.sCode), strTemp);
	cell.SetOtherJongCode(strTemp);
	
	STR_CONVERT(pchData, stBaseData.sMarket, sizeof(stBaseData.sMarket), strTemp);
	cell.SetOtherDataInt(atoi(strTemp));
	
	//# Data Index : 2006.10
	if (bChk == TRUE)
		cell.SetOtherDataLong(nRow);
	else
		cell.SetOtherDataLong(-1);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	COLORREF clrContrastText;
	char chAttribute;
	memcpy(&chAttribute, &stBaseData.sSign, sizeof(stBaseData.sSign));
	GetAttributeInfo(nRow, chAttribute, clrContrastText);
	
	nCol = (int)COL_PRICE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sPrice, sizeof(stBaseData.sPrice), strTemp);
	cell.SetOtherDataCellText(strTemp);
	strTemp = GetFormatStr(atol(strTemp), 0, FALSE, "");
	cell.SetText(strTemp);
	cell.SetOtherDataInt(atoi(&chAttribute));
	if ((chAttribute == '4') || (chAttribute == '5'))
		cell.SetOtherDataLong(atol(strTemp) * -1);
	else
		cell.SetOtherDataLong(atol(strTemp));
	
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	
	nCol = (int)COL_RATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sChange, sizeof(stBaseData.sChange), strTemp);
	strTemp = GetFormatStr(abs(atol(strTemp)), 0, FALSE, "");
	if (atol(strTemp) < 0)
		strTemp = "-" + strTemp;
	
	cell.SetText(strTemp);
	cell.SetOtherDataLong((long)(atof(strTemp) * 100.0));
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	
	nCol = (int)COL_CHANGERATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sChgRate, sizeof(stBaseData.sChgRate), strTemp);
	strTemp.TrimLeft(" ");
	if (atof(strTemp) == 0)
	{
		
	}
	else if ((long)(atof(strTemp) * 100.0) > 0)
		strTemp = "+" + strTemp;
	cell.SetText(strTemp);
	cell.SetOtherDataLong((long)(atof(strTemp) * 100.0));
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
}

void CGrid_Comm::SetModifyRow(int nRow, BaseData_Code stBaseData, BOOL bRedraw)
{
	BOOL bChk = TRUE;
	
	CUGCell cell;
	COLORREF clrBk;
	
	int nCol;
	char *pchData;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = (int)COL_CHECK;
	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
	//QuickSetText(nCol, nRow, DEFVAL_SELECTCHK);
	QuickSetText(nCol, nRow, (bChk == TRUE) ? "1" : "0");
	QuickSetBackColor(nCol, nRow, clrBk);
	
	nCol = (int)COL_NAME;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sName, sizeof(stBaseData.sName), strTemp);
	strTemp.TrimLeft(" ");
	strTemp.TrimRight(" ");
	cell.SetText(strTemp);
	
	STR_CONVERT(pchData, stBaseData.sCode, sizeof(stBaseData.sCode), strTemp);
	cell.SetOtherJongCode(strTemp);
	
	STR_CONVERT(pchData, stBaseData.sMarket, sizeof(stBaseData.sMarket), strTemp);
	cell.SetOtherDataInt(atoi(strTemp));
	
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	COLORREF clrContrastText;
	char chAttribute;
	memcpy(&chAttribute, &stBaseData.sSign, sizeof(stBaseData.sSign));
	GetAttributeInfo(nRow, chAttribute, clrContrastText);
	
	nCol = (int)COL_PRICE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sPrice, sizeof(stBaseData.sPrice), strTemp);
	cell.SetOtherDataLong(atol(strTemp));
	strTemp = GetFormatStr(atol(strTemp), 0, FALSE, "");
	cell.SetText(strTemp);
	cell.SetOtherDataInt(atoi(&chAttribute));
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	
	nCol = (int)COL_RATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sChange, sizeof(stBaseData.sChange), strTemp);
	cell.SetOtherDataLong(atol(strTemp));

	strTemp = GetFormatStr(abs(atol(strTemp)), 0, FALSE, "");
	if (atol(strTemp) < 0)
		strTemp = "-" + strTemp;
	
	cell.SetSign(atoi(&chAttribute));
	
	if(atof(strTemp) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);

	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	
	nCol = (int)COL_CHANGERATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	STR_CONVERT(pchData, stBaseData.sChgRate, sizeof(stBaseData.sChgRate), strTemp);
	strTemp.TrimLeft(" ");
	if (atof(strTemp) == 0)
	{
		
	}
	else if ((long)(atof(strTemp) * 100.0) > 0)
		strTemp = "" + strTemp;

	if(atof(strTemp) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);

	cell.SetOtherDataLong((long)(atof(strTemp) * 100.0));
	cell.SetTextColor(clrContrastText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
}

void CGrid_Comm::SetModifyRow_CodeData(int nRow, double dData, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	CString strData;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	int nCol = 3;   //# 종목 등락율(%)
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetFont(&m_fontDefault);
	
	COLORREF clrContrastText;
	if (dData == 100)
	{
		cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		clrContrastText = BLACK;
		
		cell.SetText("-");
	}
	else
	{
		cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
		strData.Format("%.2f", dData);
		cell.SetText(strData);
		
		if (dData == 0)
			clrContrastText = BLACK;
		else if (dData > 0)
			clrContrastText = RED;
		else if (dData < 0)
			clrContrastText = BLUE;
	}
	cell.SetTextColor(clrContrastText);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	if (m_nLastSelRow == nRow)
		cell.SetBackColor(m_clrSelRow);
	else
		cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
}

void CGrid_Comm::SetAddRow(AnalystInfo stAnalystInfo, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	
	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol;
	
	CString strData;
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = 0;   //# 일자
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData = "";
	if (stAnalystInfo.strDate.IsEmpty() == FALSE)
		strData = GetFormatStr(atol(stAnalystInfo.strDate), 1, TRUE, "XXXX/XX/XX");
	cell.SetText(strData);
	
	strData.Remove('/');
	cell.SetOtherDataLong(atol(strData));
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 1;   //# 시간
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData = "";
	if (stAnalystInfo.strTime.IsEmpty() == FALSE)
		strData = stAnalystInfo.strTime.Left(2) + ":" + stAnalystInfo.strTime.Right(2);
	cell.SetText(strData);
	
	strData.Remove(':');
	cell.SetOtherDataLong(atol(strData));
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 2;   //# 대비(%)
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	COLORREF clrContrastText;
	
	strData.Format("%.2f", stAnalystInfo.dParallel);
	cell.SetText(strData);
	cell.SetOtherDataLong(long(atof(strData) ));
	cell.SetOtherDataCellText(strData);

	if (stAnalystInfo.dParallel == 0)
		clrContrastText = BLACK;
	else if (stAnalystInfo.dParallel > 0)
		clrContrastText = RED;
	else if (stAnalystInfo.dParallel < 0)
		clrContrastText = BLUE;
	cell.SetTextColor(clrContrastText);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	
	nCol = 3;   //# 종목 등락율(%)
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetText("-");
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetOtherDataCellText(strData);
	SetCell(nCol, nRow, &cell);
	
	nCol = 4;   //# 누적(%)
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%.2f", stAnalystInfo.dAccumulate);
	cell.SetText(strData);
	cell.SetOtherDataLong(long(atof(strData) * 100.0));

	if (stAnalystInfo.dAccumulate == 0)
		clrContrastText = BLACK;
	else if (stAnalystInfo.dAccumulate > 0)
		clrContrastText = RED;
	else if (stAnalystInfo.dAccumulate < 0)
		clrContrastText = BLUE;
	cell.SetTextColor(clrContrastText);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetOtherDataCellText(strData);
	SetCell(nCol, nRow, &cell);
	
	nCol = 5;   //# 상승수
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", stAnalystInfo.nUpCnt);
	cell.SetText(strData);
	cell.SetOtherDataLong(atol(strData));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 6;   //# 하락수
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", stAnalystInfo.nDownCnt);
	cell.SetText(strData);
	cell.SetOtherDataLong(atol(strData));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 7;   //# 상승비중
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	int nCodeCnt;
	nCodeCnt = stAnalystInfo.nDownCnt + stAnalystInfo.nSteadyCnt + stAnalystInfo.nUpCnt;
	
	if (stAnalystInfo.nUpCnt == 0)
		strData = "0.00";
	else
		strData.Format("%.2f", stAnalystInfo.nUpCnt / (float)nCodeCnt * 100);
	cell.SetText(strData);
	
	cell.SetOtherDataLong(long(atof(strData) * 100.0));
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
}

void CGrid_Comm::SetSelectRow(int nRow)
{
	COLORREF clrBk;
	CUGCell	cell;
	
	int nRow_Old;
	nRow_Old = m_nLastSelRow;
	
	int nColCnt = GetNumberCols();
	GetEvenOddBkColor(nRow_Old, clrBk);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		if (nRow_Old != -1)
		{
			GetCell(nCol, nRow_Old, &cell);
			cell.SetBackColor(clrBk);
			SetCell(nCol, nRow_Old, &cell);
		}
		
		if (nRow != -1)
		{
			GetCell(nCol, nRow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, nRow, &cell);
		}
	}
	
	m_nLastSelRow = nRow;
	RedrawAll();
}

void CGrid_Comm::SetAddRow_Result(AnalystInfo stAnalystInfo)
{
	ClearAll();
	
	CUGCell cell;
	COLORREF clrBk;
	
	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol;
	
	CString strData;
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = 0;   //# 일자
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	//  strData = GetFormatStr(atol(stAnalystInfo.strDate), 1, TRUE, "XXXX/XX/XX");
	cell.SetText("지정일");
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 1;   //# 상승수
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", stAnalystInfo.nUpCnt);
	cell.SetText(strData);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 2;   //# 보합
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", stAnalystInfo.nSteadyCnt);
	cell.SetText(strData);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 3;   //# 하락수
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", stAnalystInfo.nDownCnt);
	cell.SetText(strData);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	int nCodeCnt;
	nCodeCnt = stAnalystInfo.nDownCnt + stAnalystInfo.nSteadyCnt + stAnalystInfo.nUpCnt;
	
	nCol = 4;   //# 전체
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%d", nCodeCnt);
	cell.SetText(strData);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 5;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	if (stAnalystInfo.nUpCnt == 0)
		strData = "0.00";
	else
		strData.Format("%.2f", stAnalystInfo.nUpCnt / (float)nCodeCnt * 100);
	cell.SetText(strData + " %");
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol = 6;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	strData.Format("%.2f", stAnalystInfo.dAccumulate);
	cell.SetText(strData + " %");
	
	COLORREF clrContrastText;
	if (stAnalystInfo.dParallel == 0)
		clrContrastText = BLACK;
	else if (stAnalystInfo.dParallel > 0)
		clrContrastText = RED;
	else if (stAnalystInfo.dParallel < 0)
		clrContrastText = BLUE;
	cell.SetTextColor(clrContrastText);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	RedrawAll();
}

void CGrid_Comm::GetAttributeInfo(int nRow, char chAttribute, COLORREF &clrContrastText)
{
	switch (chAttribute)
	{
	case '1':		// 상한(화살표) : RED 배경색에 WHITE TEXT
		clrContrastText = RED;
		break;
	case '2':		// 상승(삼각형) : WHITE 배경색에 RED TEXT
		clrContrastText = RED;
		break;
	case '4':		// 하한(화살표) : BLUE 배경색에 WHITE TEXT
		clrContrastText = BLUE;
		break;
	case '5':		// 하락(삼각형) : WHITE 배경색에 BLUE TEXT
		clrContrastText = BLUE;
		break;
	case '3':		// 보합 : WHITE 배경색에 BLACK TEXT
	default:
		clrContrastText = BLACK;
	}
}

CString CGrid_Comm::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;
	
	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";
		
		strData.Format(strDis, lData / (float)nUnitNum);
	}
	else
	{
		strDis = "%ld";
		strData.Format(strDis, lData);
	}
	
	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;
		
		if (nLength < nLengthFormat)
		{
			int nCaretIndex = 0;
			for (nIndex = 0; nIndex <= nLengthFormat; nIndex++)
			{
				if ((CString)strFormat[nIndex] == "X")
				{
					strResult += (CString)strData[nCaretIndex];
					nCaretIndex++;
				}
				else
					strResult += (CString)strFormat[nIndex];
			}
		}
	}
	else
	{
		int nCheckIndex = 0;
		BOOL bDotFlag = FALSE;
		nDotPos = strData.Find(".");
		
		int nStartIndex;
		nLength = strData.GetLength();
		if (nDotPos > 0)
		{
			nStartIndex = nDotPos;
			bDotFlag = TRUE;
		}
		else
		{
			nStartIndex = nLength;
			nDotPos = 0;
		}
		
		int nCaretIndex = 0;
		CString strResultInt;
		
		for (nIndex = nStartIndex - 1; nIndex >= 0; nIndex--)
		{
			if (nCheckIndex == 3)
			{
				strResultInt += "," + (CString)strData[nIndex];
				nCheckIndex = 1;
			}
			else
			{
				strResultInt += (CString)strData[nIndex];
				nCheckIndex++;
			}
		}
		
		nLength = strResultInt.GetLength();
		for (nIndex = nLength - 1; nIndex >= 0; nIndex--)
			strResult += (CString)strResultInt[nIndex];
		
		if (nDotPos > 0)
			strResult = strResult + strData.Right(strData.GetLength() - nDotPos);
	}
	
	return strResult;
}

int CGrid_Comm::OnCanSizeTopHdg()
{
	return FALSE;
}

void CGrid_Comm::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1))
	{
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
		//SetCommMenu(point);
		if(m_strType != "CODE")
			return;

		CString strCode = "";
		CUGCell cell;
		GetCell((int)COL_NAME, m_nLastSelRow, &cell);
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");
		
		RButtonInfo stRInfo;
		stRInfo.xPoint = point->x;
		stRInfo.yPoint = point->y;
		stRInfo.strCode = strCode;
		CWnd* pwndParent = GetParent();       //# CMainWnd
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RCLK, (LPARAM)&stRInfo);
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<
	}
}

//#include "..\..\Common\KHResource.h"	-hidden-
//extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue );	-hidden-
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);			-hidden-
void CGrid_Comm::SetCommMenu(POINT *point)
{
/* -hidden-
	if (m_hChildFrame == NULL)
		return;
	
	long lMenu = ::SendMessage(m_hChildFrame, UM_GET_SCREENLINKMENU, 0, 0 );
	if ( lMenu > 0 )
	{
		CMenu* pMenu = (CMenu*)lMenu;
		ASSERT ( pMenu );
		
		SetCommMenu_Add(pMenu);
		
		GetCursorPos(point);
		int nIndex_SelMenu = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
			point->x, point->y, this);
		
		if (nIndex_SelMenu > -1)
		{
			HWND hWnd = m_hChildFrame;
			
			CString strScreenNo = "";
			strScreenNo = GetCommMenu_IndexToScreenNo(nIndex_SelMenu);
			
			int nCol = -1;
			if (m_strType == "CODE")
				nCol = 1;
			else if (m_strType == "RESULT")
				nCol = -1;
			else if (m_strType == "PROFIT")
				nCol = -1;
			
			CString strCode = "";
			if (nCol > -1)
			{
				CUGCell cell;
				GetCell(nCol, m_nLastSelRow, &cell);
				strCode = cell.GetOtherJongCode();
				strCode.TrimRight(" ");
			}
			
			if (strScreenNo.IsEmpty() == TRUE)
			{
				if (strCode.IsEmpty() == FALSE)
					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
				::SendMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
			}
			else
			{
				if (strCode.IsEmpty() == FALSE)
					KSLink_SetLinkTagData("JCODE", strCode);		// 외부화면용
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)4, (LPARAM)(LPCTSTR)strScreenNo);
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)strlen(strScreenNo), (LPARAM)(LPCTSTR)strScreenNo);
				::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo);
			}
		}
		pMenu->DestroyMenu();
		delete pMenu;
	}
*/
}

void CGrid_Comm::SetCommMenu_Add(CMenu* pMenu)
{
/* -hidden-
	USE_SS_HTS(); 
	
	int nMenuCnt;
	UINT nMenuType;
	
	pMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)"");
	
	nMenuCnt = _countof(DRMENU_NAME);
	CString strScreenInfo;
	for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
	{
		if ((CString)DRMENU_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else 
			nMenuType = MF_STRING;
		
		if(bIsHTS)
		{
			strScreenInfo = (CString)MENU_SCREEN_NOMAX[nIndex];
			strScreenInfo = strScreenInfo.Left(4);
		}
		else
		{
			strScreenInfo = (CString)MENU_SCREEN_NOEMP[nIndex];
			strScreenInfo = strScreenInfo.Left(5);
		}
		
		strScreenInfo = "[" + strScreenInfo + "] " + (CString)DRMENU_NAME[nIndex];
		pMenu->AppendMenu(nMenuType, MENU_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
	}
*/
}

CString CGrid_Comm::GetCommMenu_IndexToScreenNo(int nIndex_SelMenu)
{
//	USE_SS_HTS();	-hidden-
	
	CString strScreenNo = "";
/* -hidden-	
	int nIndex;
	if(bIsHTS)
	{
		for (nIndex = 0; nIndex < _countof(MENU_INDEX); nIndex++)
		{
			if ((int)MENU_INDEX[nIndex] == nIndex_SelMenu)
			{
				strScreenNo = MENU_SCREEN_NOMAX[nIndex];
				break;
			}
		}
	}
	else
	{
		for (nIndex = 0; nIndex < _countof(MENU_INDEX); nIndex++)
		{
			if ((int)MENU_INDEX[nIndex] == nIndex_SelMenu)
			{
				strScreenNo = MENU_SCREEN_NOEMP[nIndex];
				break;
			}
		}
	}
*/	
	return strScreenNo;
}

void CGrid_Comm::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	CUGCell	cell;
	
	if (row != -1)
	{
		if ((col == 0) && (updn == 0))
		{
			SetHaveSelectState();
			SetSelectFluctuating();
		}
		else if ((col >= 1) && (updn == 1))
		{
			OnRowChange(m_nLastSelRow, row);
			
			/*
			CodeInfo stCodeInfo;
			if (GetRowToCodeInfo(row, stCodeInfo) == TRUE)
			{
			ShardData	stShardData;
			
			  stShardData.bOpen = FALSE;
			  
				CString strDataIndex;
				strDataIndex.Format("%d", stCodeInfo.nRow);
				stShardData.strMapNo = strDataIndex;  //# Data Index
				
				  stShardData.iaMarkType.Add(stCodeInfo.nMarket);
				  stShardData.saCode.Add(stCodeInfo.strCode);
				  
					CWnd* pwndParent = GetParent();       //# CMainWnd
					pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_CLK, (LPARAM)&stShardData);
					}
			*/
		}
	}
}

void CGrid_Comm::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	if(m_bNoUserSorting)
		return;

	if (updn && !processed)
	{		
		if ((row == -1) && (col == 0))
		{
			CUGCell cell;
			m_bSelectCheckFlag = !m_bSelectCheckFlag;
			
			for (int nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(0, nRow, &cell);
				if (cell.GetCellType() == UGCT_CHECKBOX)
				{
					if (m_bSelectCheckFlag == TRUE)
					{
						if (nRow < (int)CODE_MAX_CNT)
							QuickSetText((int)COL_CHECK, nRow, "1");
						else
							QuickSetText((int)COL_CHECK, nRow, "0");
					}
					else
						QuickSetText((int)COL_CHECK, nRow, "0");
				}
			}
			RedrawAll();
			
			CWnd* pwndParent = GetParent();       //# CMainWnd
			pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_HEADERCLK, (LPARAM)NULL);
		}
		else
		{
			QuickSetCellType(m_nSortCol, -1, 0);
			
			if (m_nSortCol == col && m_nSortMethode == UG_SORT_ASCENDING) 
			{
				m_nSortMethode = UG_SORT_DESCENDING;
				m_nArrowType   = UGCT_SORTARROWUP;
			} 
			else 
			{
				m_nSortMethode = UG_SORT_ASCENDING;
				m_nArrowType   = UGCT_SORTARROWDOWN;
			}
			
			m_nSortCol = col;
			SortBy(m_nSortCol, m_nSortMethode);
			QuickSetCellType(m_nSortCol, -1, m_nSortArrow);
			QuickSetCellTypeEx(m_nSortCol, -1, m_nArrowType);
			
			EvenOddColorSetting();
			RedrawAll();
		}
	}
}

int CGrid_Comm::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
	CString strCell1, strCell2;
	
	if (m_nSortCol == 1)
		nResult = cell1->m_string.Compare(cell2->m_string);
	else
	{
		if (cell1->m_lOtherData > cell2->m_lOtherData)
			nResult = 1;
		else if (cell1->m_lOtherData < cell2->m_lOtherData)
			nResult = -1;
	}
	
	nResult *= (-(flags * 2 - 3));
	return nResult;
}

void CGrid_Comm::EvenOddColorSetting()
{
	long nRow, nRowCnt;
	int  nCol, nColCnt;
	
	COLORREF clrBk;
	CUGCell cell;
	
	nRowCnt = GetNumberRows();
	nColCnt = GetNumberCols();
	for (nRow = 0; nRow < nRowCnt; nRow++)
	{
		GetEvenOddBkColor(nRow, clrBk);
		for (nCol = 0; nCol < nColCnt; nCol++)
		{
			GetCell(nCol, nRow, &cell);
			SetBackColorIfNotRedBlue(&cell, clrBk);
			
			//## Line Color ##
			cell.SetBorderColor(&m_penGridLine);
			cell.SetBorder(UG_BDR_RTHIN);
			
			SetCell(nCol, nRow, &cell);
		}
	}
}


void CGrid_Comm::SetHaveSelectState()
{
	CUGCell	cell;
	CString strTemp;
	
	BOOL bState = FALSE;
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		QuickGetText(0, nRow, &strTemp);
		if (strTemp[0] == '1')
		{
			bState = TRUE;
			break;
		}
	}
}

void CGrid_Comm::OnRowChange(long oldrow, long newrow)
{
	COLORREF clrBk;
	CUGCell	cell;
	
	int nColCnt = GetNumberCols();
	GetEvenOddBkColor(oldrow, clrBk);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		if (oldrow != -1)
		{
			GetCell(nCol, oldrow, &cell);
			cell.SetBackColor(clrBk);
			SetCell(nCol, oldrow, &cell);
		}
		
		if (newrow != -1)
		{
			GetCell(nCol, newrow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, newrow, &cell);
		}
	}
	
	m_nLastSelRow = newrow;
	
	if (m_strType == "CODE")
	{
		if (newrow > -1)
		{
			CodeInfo stCodeInfo;
			if (GetRowToCodeInfo(newrow, stCodeInfo) == TRUE)
			{
				CWnd* pwndParent = GetParent();       //# CMainWnd
				pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_CLK, (LPARAM)&stCodeInfo);
			}
		}
	}
	
	RedrawAll();
}

inline void CGrid_Comm::SetBackColorIfNotRedBlue(CUGCell *pCell, COLORREF clrSet)
{
	COLORREF clrCurrentBack;
	clrCurrentBack = pCell->GetBackColor();
	if( clrCurrentBack != BLUE && clrCurrentBack != RED )
		pCell->SetBackColor(clrSet);
}

inline void CGrid_Comm::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CGrid_Comm::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(m_clrHeader);
}

void CGrid_Comm::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	if (m_strType == "CODE")
	{
		if ((row > -1) && (col == 1))
		{
			CodeInfo stCodeInfo;
			if (GetRowToCodeInfo(row, stCodeInfo) == TRUE)
			{
				CWnd* pwndParent = GetParent();       //# CMainWnd
				pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_DBCLK, (LPARAM)&stCodeInfo);
			}
		}
	}
}

//extern CString dll_GetStockDetailInfo(CString strCode, int nType);	-hidden-
void CGrid_Comm::SetHint_Code(int nRow)
{
/* -hidden-
	CString strTip;
	CUGCell cell;
	GetCell(1, nRow, &cell);
	
	if (m_tipInfo.GetShowState() == FALSE)
	{
		CString strCode;
		strCode = cell.GetOtherJongCode();
		
		strCode.TrimRight(" ");
		if (strCode.IsEmpty() == TRUE)
			return;
		
		int nMarket;
		nMarket = cell.GetOtherDataInt();
		strTip = dll_GetStockDetailInfo(strCode, nMarket);
		
		m_tipInfo.UpdateToolTip(this, strTip, 1);
		m_tipInfo.Show(strTip);
	}
*/
}

int CGrid_Comm::OnHint(int col, long row, int section, CString *string)
{
	if (m_strType == "CODE")
	{
		if (col != 1)
			return FALSE;
		
		SetHint_Code(row);
		return FALSE;
	}
	else
	{
		CUGCell cell;
		GetCell((int)COL_NAME, row, &cell);
		*string = cell.GetOtherJongCode();
	}
	
	return TRUE;
}

void CGrid_Comm::OnAdjustComponentSizes(RECT *grid, 
										RECT *topHdg,RECT *sideHdg,
										RECT *cnrBtn,RECT *vScroll,
										RECT *hScroll,RECT *tabs)
{
	if (m_bCallAdjustOneTime == TRUE)
		return;
	
	CRect  rtGrid;
	double dWidthGrid = 0.0;
	double dWidthCell = 0.0;
	
	int ntotWidth = 0;
	int nCol = 0, nColCnt = 0;
	if (m_strType == "CODE")
	{
		nColCnt = _countof(HEADWIDTH_CODE);
		
		for (nCol = 0; nCol < nColCnt; nCol++)
			ntotWidth += HEADWIDTH_CODE[nCol];
		
		nColCnt = nColCnt - 2;
	}
	else if (m_strType == "RESULT")
	{
		nColCnt = _countof(HEADWIDTH_RESULT);
		
		for (nCol = 0; nCol < _countof(HEADWIDTH_RESULT); nCol++)
			ntotWidth += HEADWIDTH_RESULT[nCol];
	}
	else if (m_strType == "PROFIT")
	{
		nColCnt = _countof(HEADWIDTH_PROFIT);
		
		for (nCol = 0; nCol < nColCnt; nCol++)
		{
			if ((m_bCol_HideState == TRUE) && (m_nCol_Hide == nCol))
			{
				
			}
			else
				ntotWidth += HEADWIDTH_PROFIT[nCol];
		}
		
		if (m_bCol_HideState == TRUE)
			nColCnt--;  
	}
	
	if (!m_bCallAdjustOneTime)
	{
		rtGrid = CRect(grid);
		
		dWidthGrid = rtGrid.Width() - 1 - ntotWidth;
		dWidthCell = dWidthGrid / nColCnt;
		
		ntotWidth = 0;
		if (m_strType == "CODE")
		{
			for (nCol = 0; nCol < _countof(HEADWIDTH_CODE) - 1; nCol++)
			{
				if ((nCol == 0) || (nCol == 1))
				{
					SetColWidth(nCol, (int)HEADWIDTH_CODE[nCol]);
					ntotWidth += (int)HEADWIDTH_CODE[nCol];
				}
				else
				{
					SetColWidth(nCol, (int)HEADWIDTH_CODE[nCol] + (int)dWidthCell);
					ntotWidth += (int)HEADWIDTH_CODE[nCol] + (int)dWidthCell;
				}
			}
			
			nCol = _countof(HEADWIDTH_CODE) - 1;
		}
		else if (m_strType == "RESULT")
		{
			for (nCol = 0; nCol < _countof(HEADWIDTH_RESULT) - 1; nCol++)
			{
				SetColWidth(nCol, (int)HEADWIDTH_RESULT[nCol] + (int)dWidthCell);
				ntotWidth += (int)HEADWIDTH_RESULT[nCol] + (int)dWidthCell;
			}
			
			nCol = _countof(HEADWIDTH_RESULT) - 1;
		}
		else if (m_strType == "PROFIT")
		{
			for (nCol = 0; nCol < _countof(HEADWIDTH_PROFIT) - 1; nCol++)
			{
				if ((m_bCol_HideState == TRUE) && (m_nCol_Hide == nCol))
					SetColWidth(nCol, 0);
				else	
				{
					SetColWidth(nCol, (int)HEADWIDTH_PROFIT[nCol] + (int)dWidthCell);
					ntotWidth += (int)HEADWIDTH_PROFIT[nCol] + (int)dWidthCell;
				}
			}
			nCol = _countof(HEADWIDTH_PROFIT) - 1;
		}
		
		SetColWidth(nCol, rtGrid.Width() - 1 - ntotWidth);
		
		m_bCallAdjustOneTime = TRUE;
		AdjustComponentSizes();
		m_bCallAdjustOneTime = FALSE;
	}
	
	return;
}

void CGrid_Comm::SetViewCol(int nCol)
{
	m_nViewCol = nCol + 2;
	m_bCallAdjustOneTime = FALSE;
	
	AdjustComponentSizes();
	RedrawAll();
}

void CGrid_Comm::SetHideCol(int nCol, BOOL bView)
{
	m_nCol_Hide = nCol;
	m_bCol_HideState = bView;
	
	AdjustComponentSizes();  
}

BOOL CGrid_Comm::GetRowToCodeInfo(int nRow, CodeInfo &stCodeInfo)
{
	BOOL bResult = FALSE;
	
	if (nRow < GetNumberRows())
	{
		CString strTemp;
		QuickGetText(COL_CHECK, nRow, &strTemp);
		
		if (strTemp.IsEmpty() == TRUE)
			return FALSE;
		
		if (strTemp[0] == '1')
			stCodeInfo.bUser = TRUE;
		else
			stCodeInfo.bUser = FALSE;
		
		CUGCell cell;
		GetCell((int)COL_NAME, nRow, &cell);
		stCodeInfo.strName = cell.GetText();
		
		stCodeInfo.strCode = cell.GetOtherJongCode();
		stCodeInfo.nMarket = cell.GetOtherDataInt();
		stCodeInfo.nRow = nRow;
		stCodeInfo.strName = cell.GetText();
		
		bResult = TRUE;
		/*
		if ((stCodeInfo.strName).IsEmpty() == FALSE)
		{
		stCodeInfo.strCode = cell.GetOtherJongCode();
		stCodeInfo.nMarket = cell.GetOtherDataInt();
		stCodeInfo.nRow = nRow;//cell.GetOtherDataLong();
		
		  bResult = TRUE;
		  }
		*/
	}
	else 
		bResult = FALSE;
	
	return bResult;
}

int CGrid_Comm::GetRowToMarketType(int nRow)
{
	int nResult;
	
	CUGCell cell;
	GetCell((int)COL_NAME, nRow, &cell);
	nResult = cell.GetOtherDataInt();
	
	return nResult;
}

void CGrid_Comm::GetMarketTypeCnt(int &nKospiCnt, int &nKosdaqCnt)
{
	nKospiCnt = 0;
	nKosdaqCnt = 0;
	
	CString strTemp;
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		QuickGetText(COL_CHECK, nRow, &strTemp);
		if(strTemp.IsEmpty()) //SJ_KIM
			continue;
		if (strTemp[0] == '1')
		{
			if (GetRowToMarketType(nRow) == 1)
				nKospiCnt++;
			else if (GetRowToMarketType(nRow) == 2)
				nKosdaqCnt++;
		}
		
		if ((nKospiCnt + nKosdaqCnt) >= (int)CODE_MAX_CNT)
			break;
	}
}

void CGrid_Comm::SetSelectJong(CStringArray *psaSelectJong)
{
	int nRow, nIndex;
	CString strTemp1, strTemp2;
	CUGCell cell;
	
	if (GetNumberRows() <= 0)
		return;
	
	for (nRow = 0; nRow < GetNumberRows(); nRow++)
		QuickSetText(COL_CHECK, nRow, "0");
	
	int nCount = psaSelectJong->GetSize();
	if (nCount > 0)
	{
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			for (nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(COL_NAME, nRow, &cell);
				
				strTemp1 = cell.GetText();
				strTemp2 = psaSelectJong->GetAt(nIndex);
				
				strTemp1.TrimRight(" ");
				strTemp2.TrimRight(" ");
				
				if (strTemp1.Find(strTemp2) > -1)
					QuickSetText(COL_CHECK, nRow, "1");
			}
		}
	}
	
	SetHaveSelectState();
	SetSelectFluctuating();
	
	RedrawAll();
}

void CGrid_Comm::SetSelectFluctuating()
{
	CUGCell cell;
	CString strTemp;
	
	int nTotCnt = 0;
	double dAddAvgVal = 0;
	CUIntArray iaFluctuating;
	for (int nIndex = 0; nIndex < 5; nIndex++)
		iaFluctuating.Add(0);
	
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		QuickGetText(0, nRow, &strTemp);
		if (strTemp[0] == '1')
		{
			GetCell(COL_PRICE, nRow, &cell);
			int nSign = cell.GetOtherDataInt();
			if ((nSign >= 1) && (nSign <= 5))
			{
				int nCnt = iaFluctuating.GetAt(nSign - 1);
				iaFluctuating.SetAt(nSign - 1, nCnt + 1);
			}
			
			GetCell(COL_CHANGERATE, nRow, &cell);
			dAddAvgVal = dAddAvgVal + cell.GetOtherDataLong();
			
			nTotCnt++;
		}
	}
	
	/*
	CMainInfoWnd	*pwndMainInfo = (CMainInfoWnd*)GetParent();
	pwndMainInfo->SetFluctuatingInfo(nTotCnt, &iaFluctuating, dAddAvgVal / (100.0 * nTotCnt));
	*/
}


void CGrid_Comm::GetSelectJong(CStringArray &saSelectJong)
{
	CUGCell cell;
	CString strTemp, strCode;
	
	saSelectJong.RemoveAll();
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		QuickGetText(0, nRow, &strTemp);
		if (strTemp[0] == '1')
		{
			GetCell(1, nRow, &cell);
			strCode = cell.GetOtherJongCode();
			
			saSelectJong.Add(strCode);
		}
	}
}

int CGrid_Comm::OnCellTypeNotify(long ID, int col, long row, long msg, long param)
{
	CUGCell cell;
	
	RedrawAll();
	Invalidate();
	
	BOOL bSendMsg = FALSE;
	CString strTemp;
	if (row != -1 && col != -1)
	{
		if (msg == UGCT_CHECKBOXSET)
		{
			if (row < GetNumberRows())
			{
				AnalystData stAnalystData;
				
				GetCell((int)COL_NAME, row, &cell);
				
				stAnalystData.bUser = (BOOL)param;
				stAnalystData.nType = cell.GetOtherDataInt();
				stAnalystData.strCode = cell.GetOtherJongCode();
				
				CWnd* pwndParent = GetParent();       //# CMainWnd
				pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_CELLDATA, (LPARAM)&stAnalystData);
			}
		}
	}
	
	return TRUE;
}

void CGrid_Comm::SetColResize(BOOL bState)
{
	m_bCallAdjustOneTime = !bState;
	
	if (bState == TRUE)
	{
		AdjustComponentSizes();
		RedrawAll();
	}
}

void CGrid_Comm::ChangeGridColor(ST_GRIDCOLOR stGridColor)
{
	if(!stGridColor.clrGridHeader)
		return;
	
	m_clrHeader = stGridColor.clrGridHeader;
	m_clrSelRow = stGridColor.clrGridHilight;
	
	int nCurRow = GetCurrentRow();
	CUGCell cell;
	for(int nCol = 0; nCol < GetNumberCols(); nCol++)
	{
		GetCell(nCol, -1, &cell);
		cell.SetBackColor(m_clrHeader);
		SetCell(nCol, -1, &cell);
		
		if(nCurRow > -1)
		{		
			GetCell(nCol, nCurRow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, nCurRow, &cell);	
		}
	}	
}

void CGrid_Comm::SetSaveGridToExcel(CString strFileName)
{
	int nRow, nCol;
	CDrXLSFile xlsfile;
	
	CString strData = "";
	if (!xlsfile.OpenFile(strFileName))
	{
		CString strMsg = "지정한 파일이 다른 프로그램에서 사용중입니다";
		SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
		return;
	}
	else
	{
		CUGCell cell;
		nRow = -1;
		for (nCol = 0; nCol < GetNumberCols(); nCol++)
		{
			if(nCol == 1)
				continue;

			GetCell(nCol, nRow, &cell);
			strData = cell.GetText();
			
			xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);

			if(nCol > 1)
				xlsfile.WriteLabel(0, nCol - 1, strData);
			else
				xlsfile.WriteLabel(0, nCol, strData);
		}
		
		int nWriteRow=0;
		for (nRow = 0; nRow < GetNumberRows(); nRow++)
		{
//			//[20110825alzioyes]0000135: 조건검색결과리스트에체크박스및기능추가, 저장관련.
//			if(!IsRowCodeChecked(nRow)) continue;

			for (nCol = 0; nCol < GetNumberCols(); nCol++)
			{
				if(nCol == 1)
					continue;
				GetCell(nCol, nRow, &cell);

				if(nCol == 2 || nCol == 4)
				{
					strData = cell.GetOtherDataCellText();
				}
				else
				{
					strData = cell.GetText();
				}
				
				strData.Replace("&&", "&");

		//		double data = cell.GetOtherd();
				
				if ((nCol == 3) || (nCol == 2))    //# ChangeRate, Rate
				{
// 					if (cell.GetTextColor() == COLOR_BLUE)
// 						strData = "-" + strData;
// 					else if (cell.GetTextColor() == COLOR_RED)
// 						strData = "+" + strData;
				}
				
				if (nCol == 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_LEFT);
				else
					xlsfile.SetTextAlign(EXCEL_ALIGN_RIGHT);
				
				
				if(nCol > 1)
					xlsfile.Write(nWriteRow + 1, nCol - 1, (LPCSTR)strData);
				else
					xlsfile.Write(nWriteRow + 1, nCol, (LPCSTR)strData);
			}
			nWriteRow++;
		}
	}
	xlsfile.CloseFile();
}

int CGrid_Comm::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CAnalystResultWnd *pwndMainFrm = (CAnalystResultWnd*)GetParent();

	if(!pwndMainFrm) return FALSE;

	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}


void CGrid_Comm::GetRowCode(int nRow, CString &strCode, int &nMarkType)
{
	CUGCell cell;
	GetCell(3, nRow, &cell);
	strCode = cell.GetOtherJongCode();
	nMarkType = cell.GetOtherDataInt();

	strCode.TrimLeft(" ");
	strCode.TrimRight(" ");
}

void CGrid_Comm::ReSelectRow()
{
	CodeInfo stCodeInfo;
	if (GetRowToCodeInfo(m_nLastSelRow, stCodeInfo) == TRUE)
	{
		CWnd* pwndParent = GetParent();       //# CMainWnd
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_CLK, (LPARAM)&stCodeInfo);
	}
}	