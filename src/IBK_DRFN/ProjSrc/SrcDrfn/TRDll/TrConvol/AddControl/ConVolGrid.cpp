// ConVolGrid.cpp: implementation of the CConVolGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConVolGrid.h"

#include "../resource.h"
#include "../DefineFiles/client_typedef.h"
#include "../DefineFiles/message_typedef.h"

#include "../MainFrmWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern HINSTANCE g_hInstance;

const		LPCTSTR		Head_Code[] = { _T("종목코드"), _T("종목명") };
const		LPCTSTR		Head_Vol[] = { _T("시작"), _T("거래량") };
const		LPCTSTR		Head_Price[] = { _T("시작"), _T("거래대금") };

#define ROW_DEFHEIGHT							18
#define ROW_HEAD_DEFHEIGHT						20

const   COLORREF  clrHeader	   = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine  = COLOR_GRID_LINE;
const	COLORREF  clrCurSelRow = RGB(255, 255, 155);

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
#define BLACK	RGB(0,0,0)

/*const UINT RMSG_DBCLICK = ::RegisterWindowMessage(_T("RMSG_DBCLICK"));*/

BEGIN_MESSAGE_MAP(CConVolGrid,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConVolGrid::CConVolGrid()
{
	m_nSortMethode		= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;

	m_bCallAdjustOneTime = FALSE;
  
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	m_bTypeChange = FALSE;
	m_nRow = -1;

	m_nFixCol = 0;

	m_nLastRow = -1;
}

CConVolGrid::~CConVolGrid()
{
  m_fontDefault.DeleteObject();
  m_penGridLine.DeleteObject();
  m_mapCount.RemoveAll();
}

void CConVolGrid::ClearAll()
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

	SetNumberRows(0);
	RedrawAll();
}

BOOL CConVolGrid::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}

void CConVolGrid::OnSetup()
{
	SetSH_Width(0);
	SetSH_NumberCols(0);
	SetHighlightRow(FALSE);
  
	SetDefRowHeight(ROW_DEFHEIGHT);
	SetTH_Height(ROW_HEAD_DEFHEIGHT);

	SetCancelMode(FALSE);
	EnableMenu(TRUE);

	EnableUpdate(TRUE);
	UseHints(TRUE);

	EnableExcelBorders(FALSE);
 	SetVScrollMode(UG_SCROLLTRACKING);
	SetHScrollMode(UG_SCROLLNORMAL);

	SetUserSizingMode(FALSE);
	AdjustComponentSizes();

	m_bCandleLoadFalg = LoadCandleIndexToXml();
	CSize szFont = GetFontSize();
	m_nFontHeight = szFont.cy;
	m_nFontWidth = szFont.cx;
}

void CConVolGrid::HeaderSetup()
{
	ClearAll();

	int nColCnt = _countof(DGN_HEADER_SIZE);
	SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
	    QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		QuickSetText(nCol, -1, DGN_HEADER_NAME[nCol]);
		SetColWidth(nCol, (int)(DGN_HEADER_SIZE[nCol]));
		QuickSetBackColor(nCol, -1, clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}

	m_bCallAdjustOneTime = FALSE;

	AdjustComponentSizes();
	RedrawAll();
}

void CConVolGrid::SetAddRow(StConVolClient& stConVolClient, BOOL bInsert, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;

	int nCol, nRow;	
	if(bInsert)
	{
		InsertRow(0);
		nRow = 0;
		GetEvenOddBkColor(GetNumberRows() - 1, clrBk);
	}
	else
	{
		if(m_nMaxRow > GetNumberRows())
			AppendRow();
	
		m_nRow++;
		nRow = m_nRow;
		GetEvenOddBkColor(nRow, clrBk);

		if(m_nMaxRow == m_nRow)
		{	
			nRow = m_nRow = 0;			
 		}
//화면 클리어후 갱신방식
// 		AppendRow();
// 		nRow = GetNumberRows() - 1;
// 		GetEvenOddBkColor(nRow, clrBk);
// 
// 		if(m_nMaxRow == nRow)
// 		{
// 			ClearAll();
// 			InsertRow(0);
// 			nRow = 0;			
// 		}
	}

	for(nCol = 0; nCol < GetNumberCols(); ++nCol)
	{
		GetCell(nCol, nRow, &cell);

		if(nCol <= 1)
			cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
		else if( nCol == 2)
			cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		else
			cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);

		cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
		cell.SetFont(&m_fontDefault);
		cell.SetBorderColor(&m_penGridLine);
		cell.SetBorder(UG_BDR_RTHIN);
		cell.SetBackColor(clrBk);
		cell.SetTextColor(BLACK);
		SetCell(nCol, nRow, &cell);
	}

	char* pBuff;
	CString strTemp, strInserComma;
	CString strCode, strEntryCount;

	GetCell(0, nRow, &cell);			// 시간
	STR_CONVERT(pBuff, stConVolClient.sTime, sizeof(stConVolClient.sTime), strTemp);
	strTemp = strTemp.Left(2) + ":" + strTemp.Mid(2, 2) + ":" + strTemp.Mid(4, 2);
	cell.SetText(strTemp);
	SetCell(0, nRow, &cell);

	GetCell(1, nRow, &cell);			// 종목코드
	STR_CONVERT(pBuff, stConVolClient.sCode, sizeof(stConVolClient.sCode), strTemp);
	cell.SetText(strTemp);
	cell.SetOtherJongCode(strTemp);
	SetCell(1, nRow, &cell);
	strCode = strTemp;

	GetCell(2, nRow, &cell);			// 종목명
	STR_CONVERT(pBuff, stConVolClient.sItemName, sizeof(stConVolClient.sItemName), strTemp);
	cell.SetText(strTemp);
	CString szTest = strTemp;
	SetCell(2, nRow, &cell);

	COLORREF clrSignText;			//  등락부호
	STR_CONVERT(pBuff, stConVolClient.cSign, sizeof(stConVolClient.cSign), strTemp);				
	int nSign = atoi(strTemp);
	GetSignColor(nSign, clrSignText);

	GetCell(3, nRow, &cell);			// 현재가
	STR_CONVERT(pBuff, stConVolClient.sPriceNow, sizeof(stConVolClient.sPriceNow), strTemp);
	cell.SetTextColor(clrSignText);
	strTemp.TrimLeft();
	InsetCommar(strTemp);
	cell.SetText(strTemp);
	SetCell(3, nRow, &cell);

	GetCell(4, nRow, &cell);			// 전일대비
	STR_CONVERT(pBuff, stConVolClient.sComparison, sizeof(stConVolClient.sComparison), strTemp);
	cell.SetSign(nSign);
	cell.SetTextColor(clrSignText);
	strTemp.TrimLeft();
	InsetCommar(strTemp);

	if(atoi(strTemp) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);

	SetCell(4, nRow, &cell);

	GetCell(5, nRow, &cell);			// 등락률
	CString strData;
	STR_CONVERT(pBuff, stConVolClient.sChgRate, sizeof(stConVolClient.sChgRate), strData);
	cell.SetTextColor(clrSignText);
	strData.TrimLeft();

	CString strSign = "";
	if(nSign == 4 || nSign == 5)
		strSign = "-";

	strTemp = strSign + strData;
	if(atof(strData) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);

	SetCell(5, nRow, &cell);

	GetCell(6, nRow, &cell);			// 체결량
	STR_CONVERT(pBuff, stConVolClient.sConVol, sizeof(stConVolClient.sConVol), strTemp);
	strTemp.TrimLeft();
	InsetCommar(strTemp);
	cell.SetText(strTemp);

	if(stConVolClient.chGubun[0] == '+')
		cell.SetTextColor(RGB(255, 0, 0));
	else
		cell.SetTextColor(RGB(0, 0, 255));

	SetCell(6, nRow, &cell);

	GetCell(7, nRow, &cell);			// 누적거래량
	STR_CONVERT(pBuff, stConVolClient.sAccumDealVol, sizeof(stConVolClient.sAccumDealVol), strTemp);
	strTemp.TrimLeft();
	InsetCommar(strTemp);
	cell.SetText(strTemp);
	SetCell(7, nRow, &cell);

	GetCell(8, nRow, &cell);			// 전일거래량비
	STR_CONVERT(pBuff, stConVolClient.sDealVolRate, sizeof(stConVolClient.sDealVolRate), strTemp);
	strTemp.TrimLeft();
	InsetCommar(strTemp);
	cell.SetText(strTemp);
	SetCell(8, nRow, &cell);

	GetCell(9, nRow, &cell);			// 체결량비
	STR_CONVERT(pBuff, stConVolClient.sConVolRate, sizeof(stConVolClient.sConVolRate), strTemp);
	strTemp.TrimLeft();
	InsetCommar(strTemp);
	cell.SetText(strTemp);
	SetCell(9, nRow, &cell);

	GetCell(10, nRow, &cell);			// 체결강도
	STR_CONVERT(pBuff, stConVolClient.sVolumePower, sizeof(stConVolClient.sVolumePower), strTemp);
	strTemp.TrimLeft();
	InsetCommar(strTemp);
	cell.SetText(strTemp);

	if(atof(strTemp) >= 100.0)
		cell.SetTextColor(RGB(255, 0, 0));
	else
		cell.SetTextColor(RGB(0, 0, 255));

	SetCell(10, nRow, &cell);

	// 카운트
	STR_CONVERT(pBuff, stConVolClient.sEntryCnt, sizeof(stConVolClient.sEntryCnt), strTemp);
	strTemp.TrimLeft();
	int nEntryCount = 0;
	int nEntryCountFirst = 0;
	if(m_mapCount.Lookup(strCode, strEntryCount))
	{
		nEntryCountFirst = atoi(strEntryCount);
		nEntryCount = atoi(strTemp);
		nEntryCount -= (nEntryCountFirst - 1);
	}
	else
	{
		m_mapCount.SetAt(strCode, strTemp);
		nEntryCount = 1;
	}
	strTemp.Format("%d", nEntryCount);
	cell.SetText(strTemp);
	cell.SetTextColor(RGB(0, 0, 0));
	SetCell(11, nRow, &cell);

	if (bRedraw == FALSE)
		RedrawRow(nRow);
	//SetSelectRow(m_nLastSelRow, m_nLastSelRow + 1, TRUE);
	CurSelectRow(nRow, bInsert);

// 	if (bRedraw == TRUE) 
// 		RedrawAll();
}


void CConVolGrid::SetAddRow(int nData1, int nData2, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;

	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol;

	GetEvenOddBkColor(nRow, clrBk);

	CString strData;

	nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	strData.Format("%d 시", nData1);
	cell.SetText(strData);
	cell.SetOtherDataInt(nData1);


	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	if (m_strType == "VOL")
		strData.Format("%d 천주", nData2);
	else if (m_strType == "PRICE")
		strData.Format("%d 백만원", nData2);

	cell.SetText(strData);
	cell.SetOtherDataInt(nData2);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);

	if (bRedraw == TRUE) 
		RedrawAll();
}

void CConVolGrid::SetModifyRow(int nRow, CString strData1, CString strData2)
{
	CUGCell cell;
	COLORREF clrBk;
 
	int nCol;
	GetEvenOddBkColor(nRow, clrBk);

	nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(strData1);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(strData2);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);

	RedrawAll();
}

void CConVolGrid::SetModifyRow(int nRow, int nData1, int nData2)
{
	CUGCell cell;
	COLORREF clrBk;

	GetEvenOddBkColor(nRow, clrBk);

	CString strData;

	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	strData.Format("%d 시", nData1);
	cell.SetText(strData);
	cell.SetOtherDataInt(nData1);


	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	if (m_strType == "VOL")
		strData.Format("%d 천주", nData2);
	else if (m_strType == "PRICE")
		strData.Format("%d 백만원", nData2);
	cell.SetText(strData);
	cell.SetOtherDataInt(nData2);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
  
	RedrawAll();
}

void CConVolGrid::SetModifyRow(int nData1, int nData2)
{
	CUGCell cell;
	COLORREF clrBk;

	int nRow;
	BOOL bModify = FALSE;
	int nData_1, nData_2;
	for (nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetRowData(nRow, nData_1, nData_2);
		if (nData1 == nData_1)
		{
			bModify = TRUE;
			break;
		}
	}

	if (bModify == FALSE)
	{
		AppendRow();
		nRow = GetNumberRows() - 1;
	}

	GetEvenOddBkColor(nRow, clrBk);

	CString strData;

	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	strData.Format("%d 시", nData1);
	cell.SetText(strData);
	cell.SetOtherDataInt(nData1);
  

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	if (m_strType == "VOL")
		strData.Format("%d 천주", nData2);
	else if (m_strType == "PRICE")
		strData.Format("%d 백만원", nData2);
	cell.SetText(strData);
	cell.SetOtherDataInt(nData2);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
  
	RedrawAll();
}

void CConVolGrid::SetDeleteRow(int nRow)
{
	DeleteRow(m_nLastSelRow);
	RedrawAll();

	if (GetNumberRows() > 0)
	{
		if (m_nLastSelRow > GetNumberRows())
//			OnRowChange(m_nLastSelRow, GetNumberRows() - 1);
			SetSelectRow(m_nLastSelRow, GetNumberRows() - 1, TRUE);
	}
	else
		m_nLastSelRow = -1;
}

void CConVolGrid::GetRowData(int nRow, CString &strData1, CString &strData2)
{
	CUGCell cell;
	
	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	strData1 = cell.GetText();

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	strData2 = cell.GetText();
}

void CConVolGrid::GetRowData(int nRow, int &nData1, int &nData2)
{
	CUGCell cell;
	
	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	nData1 = cell.GetOtherDataInt();

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	nData2 = cell.GetOtherDataInt();
}

int CConVolGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

void CConVolGrid::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if (row != -1)
	{
		//OnRowChange(row, row);
		SetSelectRow(m_nLastSelRow, row);

		this->SetFocus();
	}
}
// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
void CConVolGrid::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1))
	{
		//SetCommMenu(point);
		
		SetSelectRow(m_nLastSelRow, row);
		CString strCode = "";
		CUGCell cell;
		GetCell(1, m_nLastSelRow, &cell);
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");
		
		RButtonInfo stRInfo;
		stRInfo.xPoint = point->x;
		stRInfo.yPoint = point->y;
		stRInfo.strCode = strCode;
		CWnd* pwndParent = GetParent();       //# CMainWnd
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RCLK, (LPARAM)&stRInfo);
	}
}
// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<
void CConVolGrid::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	if (updn && !processed)
	{		
		if ((row == -1) && (col == 0))
		{
				
		}
		else
		{
/*
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
*/
		}
	}
}

int CConVolGrid::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
/*	CString strCell1, strCell2;

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
*/
	CString strCell1, strCell2;

	if (m_nSortType[m_nSortCol]/*m_nSortCol == 0*/)
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

void CConVolGrid::EvenOddColorSetting()
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
			cell.SetBackColor(clrBk);

			//## Line Color ##
			cell.SetBorderColor(&m_penGridLine);
			cell.SetBorder(UG_BDR_RTHIN);

			SetCell(nCol, nRow, &cell);
		}
	}
}


void CConVolGrid::OnRowChange(long oldrow, long newrow)
{
/*
	int nCount, nLength;
	COLORREF clrBk;
	CUGCell	cell;

	nLength = GetNumberCols();
	GetEvenOddBkColor(oldrow, clrBk);
	for (nCount = 0; nCount < nLength; nCount++)
	{
		if (oldrow != -1)
		{
			GetCell(nCount, oldrow, &cell);
			cell.SetBackColor(clrBk);
			SetCell(nCount, oldrow, &cell);
		}
		
		if (newrow != -1)
		{
			GetCell(nCount, newrow, &cell);
			cell.SetBackColor(clrSelRow);
			SetCell(nCount, newrow, &cell);
		}
	}

	m_nLastSelRow = newrow;
  
  CWnd *pwndParent = GetParent();
//  pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_ITEM_CLK, (LPARAM)m_nLastSelRow);

	RedrawAll();
*/
}

inline void CConVolGrid::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CConVolGrid::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(clrHeader);
}

void CConVolGrid::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	LONG dwData = MAKELONG(row, col);
	GetParent()->SendMessage(RMSG_DBCLICK, 0, dwData);
}

int CConVolGrid::OnHint(int col, long row, int section, CString *string)
{
	CUGCell cell;

	/*
	GetCell((int)COL_NAME, row, &cell);
	*string = cell.GetOtherJongCode();
	*/

	return TRUE;
}

void CConVolGrid::OnAdjustComponentSizes(RECT *grid, RECT *topHdg,RECT *sideHdg, 
													 RECT *cnrBtn,RECT *vScroll,
													 RECT *hScroll,RECT *tabs)
{
/*
	CRect  rtGrid;
	double dWidthCell = 0.0;
		  
	if (!m_bCallAdjustOneTime)
	{
		rtGrid = CRect(grid);
		dWidthCell = rtGrid.Width() - 1 - DGN_HEADER_SIZE[0];;

		SetColWidth(0, (int)DGN_HEADER_SIZE[0]);
		SetColWidth(1, (int)dWidthCell);

		m_bCallAdjustOneTime = TRUE;
		AdjustComponentSizes();
		m_bCallAdjustOneTime = FALSE;
	}
*/
}

inline void CConVolGrid::GetSignColor(int nSign, COLORREF &clrSignText)
{
	switch (nSign)
	{
	case 1:		// 상한(화살표)
		clrSignText = RED;
		break;
	case 2:		// 상승(삼각형)
		clrSignText = RED;
		break;
	case 4:		// 하한(화살표)
		clrSignText = BLUE;
		break;
	case 5:		// 하락(삼각형)
		clrSignText = BLUE;
		break;
	case 3:		// 보합
	default:
		clrSignText = BLACK;
	}
}

void CConVolGrid::InsetCommar(CString& str1)
{
	CString str,strTemp1,strTemp2,strTemp3;
	str = str1;
	//부호 처리.
	if(str.GetLength() > 0)
	{
		if(str.GetAt(0) == '-' || str.GetAt(0) == '+')
		{
			strTemp1 = str.Left(1);
			str = str.Mid(1);//부호제거한 문자열.
		}
	}
	
	int pos ;
	int x,y,i;
	//소수점 부분을 제거한다.
	if((pos = str.Find(".")) != -1)
	{
		strTemp3 = str.Mid(pos);//소수점부터 이하값을 가지고 있는 변수.
		str = str.Left(pos);//소수점 위의 내용을 가지고 있다.
	}
	//문자열의 길이를 구한다.
	int nLength = str.GetLength();
	int max;//콤마의 최대갯수
	
	//정수 3자리 이상의 값일 경우.
	if((x = nLength / 3) > 0)//몫->x
	{
		if((y = nLength % 3) > 0) //정확히 나누어 떨어지지 않을 때.
			max = x;
		else if(y == 0)
			max = x-1;
		
		for(i = 0 ; i < max ; i++)//실제로 콤마를 삽입하는 부분.
		{
			if((pos = str.Find(','))== -1)//콤마가 문자열에 없을 경우.
			{
				str = str.Left(nLength-3)+","+str.Mid(nLength-3);					
			}
			else
			{
				str = str.Left(pos-3)+","+str.Mid(pos-3);					
			}
		}
		str1 = strTemp1 + str + strTemp3;		
	}
}

void CConVolGrid::PaintSelectRow(long oldrow, long newrow, BOOL bAdd)
{
	if (GetNumberRows() - 1 < newrow)
		return;
	
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
			cell.SetBackColor(clrSelRow);
			SetCell(nCol, newrow, &cell);
		}
	}
	RedrawAll();
}

void CConVolGrid::CurSelectRow(int nRow, BOOL bInsert)
{
	int nPrevRow;
	CUGCell	cell;
	COLORREF clrPrevRow;
	COLORREF clrLastRow;
	
	if( bInsert )
	{	// 위로추가
		nPrevRow = nRow + 1;
		GetEvenOddBkColor(GetNumberRows() - 1, clrPrevRow);
		if( m_nLastRow > -1 ) GetEvenOddBkColor(m_nLastRow, clrLastRow);

		// 상한가, 하한가 일때는 색상을 바꾼다.
		GetCell(4, nPrevRow, &cell);
		int nSign = cell.GetSign();
		if( nSign == 1 || nSign == 4 )			// 1:상한가, 4:하한가
			GetEvenOddBkColor(nPrevRow, clrPrevRow);
	}
	else
	{	// 아래로추가
		nPrevRow = nRow - 1;
		GetEvenOddBkColor(nPrevRow, clrPrevRow);
		GetEvenOddBkColor(m_nMaxRow - 1, clrLastRow);
	}

	int nColCnt = GetNumberCols();
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		if (nPrevRow != -1)
		{
			GetCell(nCol, nPrevRow, &cell);
			cell.SetBackColor(clrPrevRow);
			SetCell(nCol, nPrevRow, &cell);
		}

		if (nRow != -1)
		{
			GetCell(nCol, nRow, &cell);
			cell.SetBackColor(clrCurSelRow);
			SetCell(nCol, nRow, &cell);
		}
		
		if( bInsert )
		{	// 위로추가
			if( m_nLastRow > -1 ) 
			{	// 아래로 추가에서 위로추가를 클릭시 셀색상을 변경위해.. 한번만 셀색상을 바꿔준다.
				GetCell(nCol, m_nLastRow + 1, &cell);
				cell.SetBackColor(clrLastRow);
				SetCell(nCol, m_nLastRow + 1, &cell);
			}
		}
		else
		{	// 아래로추가
			if ((m_nMaxRow - 1) != nRow)
			{
				GetCell(nCol, m_nMaxRow - 1, &cell);
				cell.SetBackColor(clrLastRow);
				SetCell(nCol, m_nMaxRow - 1, &cell);
			}
		}
	}

	if( bInsert ) 
	{	// 위로추가
		if( m_nLastRow > -1 ) 
		{
			RedrawRow(m_nLastRow + 1);

			// 아래로 추가에서 위로추가를 클릭시 셀색상을 변경위해.. 한번만 셀색상을 바꿔준다.
			m_nLastRow = -1;
		}
	}
	else
	{	// 아래로추가
		RedrawRow(m_nMaxRow - 1);
		m_nLastRow = nRow;
	}

	RedrawRow(nPrevRow);
	RedrawRow(nRow);
}

void CConVolGrid::SetSelectRow(long oldrow, long newrow, BOOL bAdd)
{
	if (GetNumberRows() - 1 < newrow)
		return;
	
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
			cell.SetBackColor(clrSelRow);
			SetCell(nCol, newrow, &cell);
		}
	}
	
	m_nLastSelRow = newrow;
	RedrawAll();

	if(!bAdd)
	{
		GetCell(1, newrow, &cell);
		CString strCode = cell.GetText();

		CWnd *pParent = GetParent();
		pParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_CLK, (LPARAM)&strCode);
	}
}

void CConVolGrid::HideColumn(int nCol, BOOL bHide)
{
	if(bHide)
	{
		SetColWidth(nCol, 0);
	}
	else
	{
		SetColWidth(nCol, DGN_HEADER_SIZE[nCol]);
	}

	RedrawCol(nCol);
}

void CConVolGrid::ChangeGridColor(ST_GRIDCOLOR stGridColor)
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

const		LPCTSTR		HEADNAME_BASE[] = {_T("시각"), _T("종목코드"), _T("종목명") };
const		int			HEADWIDTH_BASE[] = {90, 50, 90};

const   int       MAX_COL = 21;
#include "math.h"
#define COL_CODE      0
#define COL_NAME      1
void CConVolGrid::SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField)
{
	CUGCell	cell;

	ClearAll();
	//ZeroMemory(&m_nSortType, sizeof(int) * RESULT_FIELD_MAX);

	int nColCnt = plistField->GetCount();
	SetNumberCols(nColCnt + 3);

	int nCol = 0;
	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	GetColDefault(nCol, &cell);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	//SetColDefault(nSetartHead, &cell);
	QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
	SetColWidth(nCol, HEADWIDTH_BASE[nCol]);
	QuickSetBackColor(nCol, -1, clrHeader);
	QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
	QuickSetFont(nCol, -1, &m_fontDefault);
	m_nSortType[nCol] = 1;
	nCol++;

	POSITION   pos;
	HeadInfoType    stHeadType;
	for (int nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistField->FindIndex(nIndex);
		stHeadType = m_plistField->GetAt(pos);

		TRACE("\n # Count      : %d", nCol);
		TRACE("\n   # HeadName : %s", stHeadType.strHeadName);
		TRACE("\n   # HeadID   : %d", stHeadType.lHeadID);

		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		QuickSetText(nCol, -1, stHeadType.strHeadName);
		SetColWidth(nCol, (int)(stHeadType.nColWidth * (m_nFontWidth + 1)));
		SetRowHeight(nCol, m_nFontHeight + 4);
		QuickSetBackColor(nCol, -1, clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);

		if ((stHeadType.strDataType == "code") ||  //## 종목 CODE ##
			(stHeadType.strDataType == "candle")	|| //# 캔들
			(stHeadType.strDataType == "pattern") ||	//# 패턴
			(stHeadType.strDataType == "period")	// 기간
			)	
			m_nSortType[nCol] = 1;
		nCol++;
	}

	RedrawAll();
}

void CConVolGrid::SetLockColumns(int nCol, BOOL bOffSet)
{
	if (bOffSet == TRUE)
		m_nFixCol = 2 + nCol;
	else
		m_nFixCol = nCol;

	LockColumns(m_nFixCol);
}

void CConVolGrid::SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
							 CString strItemName, char dcolData[960], bool bRedraw)
{
	int nCol;
	CString strData[MAX_COL];
//	CString strTemp;
	char szTemp[17];
	double dTemp;
	CUGCell cellItemDel, cellItemName, cellItemCode, cellData[MAX_COL];
	
	int nRowIndex = nRow;
	COLORREF clrBk;
	GetEvenOddBkColor(nRowIndex, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;

	char szItemCode[7];
	memset(szItemCode, 0x00, sizeof(szItemCode));
	memcpy(szItemCode, strItemCode, 6);	
	
	int nRealDataIndex = 0;
	int nArrIndex = 0;
	if(m_plistField)		//070420금 Null체크루틴 추가.
	{
		for (nCol = 0; nCol < ntotCol; nCol++)
		{
			pos = m_plistField->FindIndex(nCol);
			TempHeadType = m_plistField->GetAt(pos);
			
			int nDataLength = TempHeadType.nDataLength;
			int nUnitNum = TempHeadType.nUnitNum;

			
			nArrIndex = nCol * 16;
			
			CString strFormat;
			int nFormatCnt = 0;
			if (TempHeadType.strDataType == "code")  //## CODE ##
				continue;
			else if (TempHeadType.strDataType == "candle")	//# 캔들
			{
				nRealDataIndex++;
				continue;
			}
			else if (TempHeadType.strDataType == "pattern")		//# 패턴
			{
				nRealDataIndex++;
				continue;
			}
			else if (TempHeadType.strDataType == "debi")   //## 대비 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = (dTemp > 0) ? "+" : "-";
				strSign = (dTemp == 0) ? " " : strSign;
				
				//////////////////////////////////////////////////////////////
				// 20100414 박찬모 업종검색에서 전일대비 소수점 이하 2자리 >>
// 				if(m_nIndustry == 1)
// 				{
// 					TempHeadType.strDataType = "double";
// 					nUnitNum = 100;
// 				}
				// 20100414 박찬모 업종검색에서 전일대비 소수점 이하 2자리 <<
				//////////////////////////////////////////////////////////////
				
				strData[nRealDataIndex] = /*strSign +*/ GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "debiuel")  //## 대비율 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = _T("");
				strSign = (dTemp == 0) ? " " : strSign;
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "date")  //## 날짜 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "time")  //## 시간 ##
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
				nRealDataIndex++;
			}
			else		//# int, double
			{
//				strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = (dTemp > 0)?' ':'-';
				strSign = (dTemp == 0) ? " " : strSign;
				
				/////////////////////////////////////////////////////////////////////////////
				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 >> 
// 				if(m_nIndustry == 1 && !((TempHeadType.strHeadName).Compare("현재가")))
// 				{
// 					TempHeadType.strDataType = "double";
// 					nUnitNum = 100;
// 				}
// 				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 <<
				/////////////////////////////////////////////////////////////////////////////
				
				if (TempHeadType.strDataType == "int")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");
				if (TempHeadType.strDataType == "double")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
		}
	}
	
	//GetCell(0, nRowIndex, &cellItemDel);
	GetCell((int)COL_CODE, nRowIndex, &cellItemCode);
	GetCell((int)COL_NAME, nRowIndex, &cellItemName);
	
	int nCol_Offset;
	nCol_Offset = 2;
	
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);
	
	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
	cellItemCode.SetFont(&m_fontDefault);
	cellItemCode.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemCode.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellItemCode.SetText(szItemCode);
	cellItemCode.SetBackColor(clrBk);
	
	cellItemName.SetFont(&m_fontDefault);
	cellItemName.SetText(strItemName);
	cellItemName.SetBackColor(clrBk);
	cellItemName.m_string = strItemName;
	
	cellItemName.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherJongCode(strItemCode);
	cellItemName.SetOtherDataInt(atoi(&chMarkType));
	
	COLORREF clrSignText;
	nRealDataIndex = 0;
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		TempHeadType = m_plistField->GetAt(pos);
		
		if (TempHeadType.strDataType == "char")
			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
		else if ((TempHeadType.strDataType == "double") || 
			(TempHeadType.strDataType == "int"))
			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_NUMBER | UGCELLDATA_OTHER);
		
		if (TempHeadType.strAlignment == "center")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
		else if (TempHeadType.strAlignment == "left")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
		else if (TempHeadType.strAlignment == "right")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNRIGHT);
		
		if (TempHeadType.strDataType == "code")  //## CODE ##
		{
			if (!strItemCode.IsEmpty())
			{
				strItemCode.TrimRight();
				strItemCode.TrimLeft();
			}
			cellData[nCol + nCol_Offset].SetText(strItemCode);
			cellData[nCol + nCol_Offset].m_string = strItemCode;
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strItemCode);
		}
		else if (TempHeadType.strDataType == "candle")  //# 캔들 
		{
//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

			memset(szTemp, NULL, sizeof(szTemp));
			/*memcpy(szTemp, dcolData[nRealDataIndex], 16);*/

			strData[nRealDataIndex] = FindCandleIdFromName((long)atol(szTemp));
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "pattern")  //# 패턴
		{
//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

			memset(szTemp, NULL, sizeof(szTemp));
			/*memcpy(szTemp, dcolData[nRealDataIndex], 16);*/

			strData[nRealDataIndex] = FindPatenIdFromName(atol(szTemp));
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
			nRealDataIndex++;
		}
		else
		{
			if (TempHeadType.strUnitText.IsEmpty() == FALSE)
				strData[nRealDataIndex] += " " + TempHeadType.strUnitText;  //<= 단위
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);

//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

			memset(szTemp, NULL, sizeof(szTemp));
			/*memcpy(szTemp, dcolData[nRealDataIndex], 16);*/

			if(TempHeadType.strDataType == "debi" || TempHeadType.strDataType == "debiuel")
			{
				if(chSign == '4' || chSign == '5')
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * -100.0));
				else
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 100.0));
			}
			else
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atol(szTemp)));
			
			char szSign[2] = {0,};
			szSign[0] = chSign;
			int nSign = atoi(szSign);
			GetSignColor(nSign, clrSignText);
			if (TempHeadType.strDataType == "debi")    //## 대비 ##
			{
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
				cellData[nCol + nCol_Offset].SetSign(nSign);
			}
			else if (TempHeadType.strDataType == "debiuel")   //## 대비율 ##
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
			else if(TempHeadType.strHeadName == "현재가")
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);

			nRealDataIndex++;
		}
		
		cellData[nCol + nCol_Offset].SetBackColor(clrBk);
		cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
	}
	
	/*
	cellItemDel.SetBackColor(clrBk);
	cellItemDel.SetBorderColor(&m_penGridLine);
	cellItemDel.SetBorder(UG_BDR_RTHIN);
	SetCell(0, nRowIndex, &cellItemDel);
	*/
	
	cellItemCode.SetBackColor(clrBk);
	cellItemCode.SetBorderColor(&m_penGridLine);
	cellItemCode.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_CODE, nRowIndex, &cellItemCode);
	
	cellItemName.SetBackColor(clrBk);
	cellItemName.SetBorderColor(&m_penGridLine);
	cellItemName.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_NAME, nRowIndex, &cellItemName);
	
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
		cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
		SetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);
	}
	
	if (bRedraw == true) 
		RedrawAll();
}

CString CConVolGrid::FindCandleIdFromName(long lCandleID)
{
	CString strResult = "--";

	int i, nCount;
	POSITION pos;

	CandleInfo      TempCandleInfo;

	if (m_bCandleLoadFalg == FALSE)
		return strResult;

	nCount = m_listCnadle.GetCount() - 1;
	for (i = 0; i < nCount; i++)
	{
		pos = m_listCnadle.FindIndex(i);
		TempCandleInfo = m_listCnadle.GetAt(pos);

		if (TempCandleInfo.lID == lCandleID)
		{
			strResult = TempCandleInfo.strName;
			break;
		}
	}

	return strResult;
}

BOOL CConVolGrid::LoadCandleIndexToXml()
{
	MSXML::IXMLDOMNodePtr		      pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;

	CandleInfo      TempCandleInfo;

	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;
	CString strIndexFilePath;
	//## Candle Index 파일 LOAD ##
	GETPATH_APP(strAppPath);
	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + CANDLE_INDEX_FILE;

	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));

	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}

	m_listCnadle.RemoveAll();
	try {
		//# header의 node를 얻는다.
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		pNS_FItem  = pNIndex->selectNodes("FITEM");

		int nCount, nLength;
		CString strTemp;

		nLength = pNS_FItem->length;
		for(nCount = 0; nCount < nLength; nCount++)
		{
			//# node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	

			//# 속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;

			//##### Each Filed Information START #####
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempCandleInfo.lID = atol(strTemp);

			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			TempCandleInfo.strName = CString((PCHAR)pNAttributeTemp->text);
			//## Candle ID insert ##
			m_listCnadle.AddTail(TempCandleInfo);
		}
	}
	catch(...) {
		bResult = FALSE;
		return bResult;
	}

	return bResult;
}

CString CConVolGrid::FindPatenIdFromName(long lPatenID)
{
	CString strResult = "--";

	int i, nCount;
	POSITION pos;

	PatenInfo      TempPatenInfo;

// 	if (m_bPatenLoadFalg == FALSE)
// 		return strResult;

	nCount = m_listPaten.GetCount() - 1;
	for (i = 0; i < nCount; i++)
	{
		pos = m_listPaten.FindIndex(i);
		TempPatenInfo = m_listPaten.GetAt(pos);

		if (TempPatenInfo.lID == lPatenID)
		{
			strResult = TempPatenInfo.strName;
			break;
		}
	}

	return strResult;
}

BOOL CConVolGrid::LoadPatenIndexToXml()
{
	MSXML::IXMLDOMNodePtr		      pNIndex;    // Header
	MSXML::IXMLDOMNodeListPtr	    pNS_FItem;  // Item List
	MSXML::IXMLDOMNodePtr			    pN_FItem;   // Item
	MSXML::IXMLDOMNamedNodeMapPtr	pNA_FItem;

	PatenInfo      TempPatenInfo;

	BOOL bResult = TRUE;
	VARIANT_BOOL bLoadedXMLFile;
	CString strIndexFilePath;
	//## Candle Index 파일 LOAD ##
	GETPATH_APP(strAppPath);
	strIndexFilePath = strAppPath + FOLDER_DATA + "\\" + PATEN_INDEX_FILE;

	MSXML::IXMLDOMDocumentPtr pDOMConditionIndex ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);
	pDOMConditionIndex->async = false;
	bLoadedXMLFile = pDOMConditionIndex->load(_bstr_t(strIndexFilePath));

	if(bLoadedXMLFile == VARIANT_FALSE) 
	{
		bResult = FALSE;
		return bResult;
	}

	m_listPaten.RemoveAll();
	try {
		//# header의 node를 얻는다.
		pNIndex = pDOMConditionIndex->selectSingleNode("INDEX");
		pNS_FItem  = pNIndex->selectNodes("FITEM");

		int nCount, nLength;
		CString strTemp;

		nLength = pNS_FItem->length;
		for(nCount = 0; nCount < nLength; nCount++)
		{
			//# node를 얻어온다.
			pN_FItem = pNS_FItem->item[nCount];	

			//# 속성을 얻어온다.
			MSXML::IXMLDOMNodePtr	pNAttributeTemp;
			pNA_FItem= pN_FItem->attributes;

			//##### Each Filed Information START #####
			pNAttributeTemp = pNA_FItem->getNamedItem("ID");
			strTemp = CString((PCHAR)pNAttributeTemp->text);
			TempPatenInfo.lID = atol(strTemp);

			pNAttributeTemp = pNA_FItem->getNamedItem("FIELD_NAME");
			TempPatenInfo.strName = CString((PCHAR)pNAttributeTemp->text);
			//## Candle ID insert ##
			m_listPaten.AddTail(TempPatenInfo);
		}
	}
	catch(...) {
		bResult = FALSE;
		return bResult;
	}

	return bResult;
}

CSize CConVolGrid::GetFontSize()
{
	CClientDC	dc(this);
	CFont*	pOldFont = dc.SelectObject(m_GI->m_defFont);

	CSize	sizeFont;
	TEXTMETRIC	tm;

	dc.GetTextMetrics(&tm);
	sizeFont.cx	=	tm.tmAveCharWidth;
	sizeFont.cy	=	tm.tmHeight;

	dc.SelectObject(pOldFont);

	return	sizeFont;
}

CString CConVolGrid::GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
{
	CString strResult = "";
	int nIndex, nLength, nLengthFormat, nDotPos;

	CString strData, strDis, strTemp;
	if (nUnitNum > 1)
	{
		strTemp.Format("%d", nUnitNum);
		strTemp.Format("%d", strTemp.GetLength() - 1);
		strDis = "%." + strTemp + "f";
		strData.Format(strDis, dData);
	}
	else
	{
		strDis = "%.0f";
		strData.Format(strDis, dData);
	}

	if (bFormatFlag == TRUE)
	{
		nLength = strData.GetLength() - 1;
		nLengthFormat = strFormat.GetLength() - 1;

		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
			strData = "0" + strData;

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

CString CConVolGrid::GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
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

		for (nIndex = 1; nIndex < (nLengthFormat - nLength) - 1; nIndex++)
			strData = "0" + strData;

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
				if (strData[nIndex] != '-')
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

void CConVolGrid::SetAddRow(StTrConVol stTrConVol, BOOL bInsert, BOOL bRedraw)
{
// 	CUGCell cell;
// 	COLORREF clrBk;
// 
// 	int nCol, nRow;
// 	if(bInsert)
// 	{
// 		InsertRow(0);
// 		nRow = 0;
// 		GetEvenOddBkColor(GetNumberRows() - 1, clrBk);
// 	}
// 	else
// 	{
// 		if(m_nMaxRow > GetNumberRows())
// 			AppendRow();
// 
// 		m_nRow++;
// 		nRow = m_nRow;
// 		GetEvenOddBkColor(nRow, clrBk);
// 
// 		if(m_nMaxRow == m_nRow)
// 		{	
// 			nRow = m_nRow = 0;			
// 		}
// 	}
// 
// 	char* pchData;
// 	CString szTime, szCode, szItemName;
// 
// 	pchData = new char[sizeof(stTrConVol.sSignaltime) + 1];
// 	memset(pchData, 0x00, sizeof(stTrConVol.sSignaltime) + 1);
// 	memcpy(pchData, stTrConVol.sSignaltime, sizeof(stTrConVol.sSignaltime));
// 	double dTemp = atof(pchData);
// 	szTime = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
// 	delete[] pchData;
// 	
// 	pchData = new char[sizeof(stTrConVol.sItemCode) + 1];
// 	memset(pchData, 0x00, sizeof(stTrConVol.sItemCode) + 1);
// 	memcpy(pchData, stTrConVol.sItemCode, sizeof(stTrConVol.sItemCode));
// 	szCode = (CString)pchData;
// 	delete[] pchData;
// 
// 	pchData = new char[sizeof(stTrConVol.sItemName) + 1];
// 	memset(pchData, 0x00, sizeof(stTrConVol.sItemName) + 1);
// 	memcpy(pchData, stTrConVol.sItemName, sizeof(stTrConVol.sItemName));
// 	szItemName = (CString)pchData;
// 	szItemName.TrimRight(" ");
// 	delete[] pchData;
// 
// 	// 시각
// 	nCol = 0;
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	cell.SetText(szTime);
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	nCol++;
// 
// 	// 코드
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	cell.SetText(szCode);
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	nCol++;
// 
// 	// 종목명
// 	GetCell(nCol, nRow, &cell);
// 	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
// 	cell.SetFont(&m_fontDefault);
// 	cell.SetText(szItemName);
// 	cell.SetBorderColor(&m_penGridLine);
// 	cell.SetBorder(UG_BDR_RTHIN);
// 	cell.SetBackColor(clrBk);
// 	SetCell(nCol, nRow, &cell);
// 	nCol++;
// 	
// 	CString szData[MAX_COL];
// 	char szTemp[17] = {0,};
// 	CUGCell cellItemTime, cellItemCode, cellItemName, cellData[MAX_COL];
// 
// 	POSITION pos;
// 	HeadInfoType stHeadInfoType;
// 
// 	int nRealDataIndex = 0;
// 	int nArrIndex = 0;
// 	
// 	CList<HeadInfoType, HeadInfoType&>* plistFixHeadSys = NULL;
// 	plistFixHeadSys = &((CMainFrmWnd*)GetParent())->m_listFixHeadSys;
// 
// 	int ntotCol = m_plistField->GetSize();
// 
// 	if(plistFixHeadSys)
// 	{
// 		for(nCol = 0; nCol < ntotCol; nCol++)
// 		{
// 			pos = m_plistField->FindIndex(nCol);
// 			stHeadInfoType = m_plistField->GetAt(pos);
// 
// 			POSITION _pos;
// 			HeadInfoType _stHeadInfpType;
// 			int nSeverDataIndex = 0; // 서버에서 내려온 값을 뽑아내기 위한 인덱스
// 			for(nSeverDataIndex = 0; nSeverDataIndex < plistFixHeadSys->GetCount(); nSeverDataIndex++)
// 			{
// 				_pos = plistFixHeadSys->FindIndex(nSeverDataIndex);
// 				_stHeadInfpType = plistFixHeadSys->GetAt(_pos);
// 				if(stHeadInfoType.strHeadName == _stHeadInfpType.strHeadName)
// 					break;
// 			}
// 			nSeverDataIndex *= 16;
// 
// 			// data setting
// 			int nDataLength = stHeadInfoType.nDataLength;
// 			int	nUnitNum = stHeadInfoType.nUnitNum;
// 
// 			if(stHeadInfoType.strDataType == _T("code"))
// 				continue;
// 
// 			else if(stHeadInfoType.strDataType == _T("candle"))
// 			{
// 				nRealDataIndex++;
// 				continue;
// 			}
// 
// 			else if(stHeadInfoType.strDataType == _T("pattern"))
// 			{
// 				nRealDataIndex++;
// 				continue;
// 			}
// 
// 			else if(stHeadInfoType.strDataType == _T("debi"))
// 			{
// 				memset(szTemp, NULL, sizeof(szTemp));
// 				memcpy(szTemp, &stTrConVol.sResultField[nSeverDataIndex], 16);
// 				dTemp = atof(szTemp);
// 
// 				CString szSign = (dTemp > 0) ? "+" : "-";
// 				szSign = (dTemp == 0) ? " " : szSign;
// 
// 				szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
// 				nRealDataIndex++;
// 			}
// 
// 			else if(stHeadInfoType.strDataType == _T("debiuel"))
// 			{
// 				memset(szTemp, NULL, sizeof(szTemp));
// 				memcpy(szTemp, &stTrConVol.sResultField[nSeverDataIndex], 16);
// 				dTemp = atof(szTemp);
// 
// 				CString szSign = _T("");
// 				szSign = (dTemp == 0) ? " " : szSign;
// 				szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
// 				nRealDataIndex++;
// 			}
// 
// 			else if(stHeadInfoType.strDataType == _T("date"))
// 			{
// 				memset(szTemp, NULL, sizeof(szTemp));
// 				memcpy(szTemp, &stTrConVol.sResultField[nSeverDataIndex], 16);
// 				dTemp = atof(szTemp);
// 
// 				szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
// 				nRealDataIndex++;
// 			}
// 
// 			else if(stHeadInfoType.strDataType == _T("time"))
// 			{
// 				memset(szTemp, NULL, sizeof(szTemp));
// 				memcpy(szTemp, &stTrConVol.sResultField[nSeverDataIndex], 16);
// 				dTemp = atof(szTemp);
// 
// 				szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
// 				nRealDataIndex++;
// 			}
// 
// 			else
// 			{
// 				memset(szTemp, NULL, sizeof(szTemp));
// 				memcpy(szTemp, &stTrConVol.sResultField[nSeverDataIndex], 16);
// 				dTemp = atof(szTemp);
// 
// 				CString szSign = (dTemp > 0) ? " " : "-";
// 				szSign = (dTemp == 0) ? " " : szSign;
// 
// 				if(stHeadInfoType.strDataType == "int")
// 					szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");
// 				else if(stHeadInfoType.strDataType == "double")
// 					szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
// 
// 				nRealDataIndex++;
// 			}
// 		}
// 	}
// 
// 	int nCol_Offset = 3;
// 	COLORREF clrSignText;
// 	nRealDataIndex = 0;
// 	for(nCol = 0; nCol < ntotCol; nCol++)
// 	{
// 		pos = m_plistField->FindIndex(nCol);
// 		stHeadInfoType = m_plistField->GetAt(pos);
// 
// 		if(stHeadInfoType.strHeadName == "char")
// 			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 		else if ((stHeadInfoType.strDataType == "double") || 
// 			(stHeadInfoType.strDataType == "int"))
// 			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_NUMBER | UGCELLDATA_OTHER);
// 
// 		if (stHeadInfoType.strAlignment == "center")
// 			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
// 		else if (stHeadInfoType.strAlignment == "left")
// 			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
// 		else if (stHeadInfoType.strAlignment == "right")
// 			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNRIGHT);
// 
// 		if (stHeadInfoType.strDataType == "code")  //## CODE ##
// 		{
// 		}
// 		else if (stHeadInfoType.strDataType == "candle")  //# 캔들 
// 		{
// 			memset(szTemp, NULL, sizeof(szTemp));
// 
// 			szData[nRealDataIndex] = FindCandleIdFromName((long)atol(szTemp));
// 
// 			if (!szData[nRealDataIndex].IsEmpty())
// 			{
// 				szData[nRealDataIndex].TrimRight();
// 				szData[nRealDataIndex].TrimLeft();
// 			}
// 
// 			cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
// 			cellData[nCol + nCol_Offset].m_string = szData[nRealDataIndex];
// 
// 			cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
// 			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
// 			nRealDataIndex++;
// 		}
// 		else if (stHeadInfoType.strDataType == "pattern")  //# 패턴
// 		{
// 			memset(szTemp, NULL, sizeof(szTemp));
// 
// 			szData[nRealDataIndex] = FindPatenIdFromName(atol(szTemp));
// 
// 			if (!szData[nRealDataIndex].IsEmpty())
// 			{
// 				szData[nRealDataIndex].TrimRight();
// 				szData[nRealDataIndex].TrimLeft();
// 			}
// 
// 			cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
// 			cellData[nCol + nCol_Offset].m_string = szData[nRealDataIndex];
// 
// 			cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
// 			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
// 			nRealDataIndex++;
// 		}
// 		else
// 		{
// 			if (stHeadInfoType.strUnitText.IsEmpty() == FALSE)
// 				szData[nRealDataIndex] += " " + stHeadInfoType.strUnitText;  //<= 단위
// 
// 			if (!szData[nRealDataIndex].IsEmpty())
// 			{
// 				szData[nRealDataIndex].TrimRight();
// 				szData[nRealDataIndex].TrimLeft();
// 			}
// 
// 			cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
// 			cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
// 
// 			memset(szTemp, NULL, sizeof(szTemp));
// 
// 			char chSign = stTrConVol.sSign[1];
// 			if(stHeadInfoType.strDataType == "debi" || stHeadInfoType.strDataType == "debiuel")
// 			{
// 				if(chSign == '4' || chSign == '5')
// 					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * -100.0));
// 				else
// 					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 100.0));
// 			}
// 			else
// 				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atol(szTemp)));
// 
// 			char szSign[2] = {0,};
// 			szSign[0] = chSign;
// 			int nSign = atoi(szSign);
// 			GetSignColor(nSign, clrSignText);
// 			if (stHeadInfoType.strDataType == "debi")    //## 대비 ##
// 			{
// 				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
// 				cellData[nCol + nCol_Offset].SetSign(nSign);
// 			}
// 			else if (stHeadInfoType.strDataType == "debiuel")   //## 대비율 ##
// 				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
// 			else if(stHeadInfoType.strHeadName == "현재가")
// 				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
// 
// 			nRealDataIndex++;
// 		}
// 		cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
// 	}
// 
// 	for(nCol = 0; nCol < ntotCol; nCol++)
// 	{
// 		cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
// 		cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
// 		cellData[nCol + nCol_Offset].SetBackColor(clrBk);
// 
// 		SetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);
// 	}
// 
// 	RedrawAll();
}