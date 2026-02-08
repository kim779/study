// SetGrid.cpp: implementation of the CSetGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetGrid.h"

#include "../resource.h"
#include "../DefineFiles/client_typedef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern HINSTANCE g_hInstance;

const		LPCTSTR		Head_Code[] = { _T("종목코드"), _T("종목명") };
const		LPCTSTR		Head_Vol[] = { _T("시작"), _T("거래량") };
const		LPCTSTR		Head_Price[] = { _T("시작"), _T("거래대금") };
const		int				Head_Width[] = { 60, 80 };


#define ROW_DEFHEIGHT									18
#define ROW_HEAD_DEFHEIGHT						20

const   COLORREF  clrHeader	   = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine  = COLOR_GRID_LINE;

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
#define BLACK		RGB(0,0,0)

BEGIN_MESSAGE_MAP(CSetGrid,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetGrid::CSetGrid()
{
	m_nSortMethode		= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;

	m_bCallAdjustOneTime = FALSE;
  
  LOGFONT	  logFont;
  GETFONT(logFont, 12, 400, m_fontDefault);
  m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
}

CSetGrid::~CSetGrid()
{
  m_fontDefault.DeleteObject();
  m_penGridLine.DeleteObject();
}

void CSetGrid::ClearAll()
{
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

BOOL CSetGrid::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}

void CSetGrid::OnSetup()
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
}

void CSetGrid::HeaderSetup(CString strType)
{
	ClearAll();

	m_strType = strType;

	int nColCnt = _countof(Head_Width);
	SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
	    QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);

		if (strType == "CODE")
			QuickSetText(nCol, -1, Head_Code[nCol]);
		else if (strType == "VOL")
			QuickSetText(nCol, -1, Head_Vol[nCol]);
		else if (strType == "PRICE")
			QuickSetText(nCol, -1, Head_Price[nCol]);
    
		SetColWidth(nCol, (int)(Head_Width[nCol]));
		QuickSetBackColor(nCol, -1, clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}

	m_bCallAdjustOneTime = FALSE;

	AdjustComponentSizes();
	RedrawAll();
}

void CSetGrid::SetAddRow(CString strData1, CString strData2, BOOL bRedraw)
{
  CUGCell cell;
	COLORREF clrBk;

  AppendRow();
  int nRow = GetNumberRows() - 1;
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

  if (bRedraw == TRUE) 
		RedrawAll();
}


void CSetGrid::SetAddRow(int nData1, int nData2, BOOL bRedraw)
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

void CSetGrid::SetModifyRow(int nRow, CString strData1, CString strData2)
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

void CSetGrid::SetModifyRow(int nRow, int nData1, int nData2)
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

void CSetGrid::SetModifyRow(int nData1, int nData2)
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

void CSetGrid::SetDeleteRow(int nRow)
{
	DeleteRow(m_nLastSelRow);
	RedrawAll();

	if (GetNumberRows() > 0)
	{
		if (m_nLastSelRow > GetNumberRows())
			OnRowChange(m_nLastSelRow, GetNumberRows() - 1);
	}
	else
		m_nLastSelRow = -1;
}

void CSetGrid::GetRowData(int nRow, CString &strData1, CString &strData2)
{
	CUGCell cell;
	
	int nCol = 0;
  GetCell(nCol, nRow, &cell);
	strData1 = cell.GetText();

	nCol = 1;
  GetCell(nCol, nRow, &cell);
	strData2 = cell.GetText();
}

void CSetGrid::GetRowData(int nRow, int &nData1, int &nData2)
{
	CUGCell cell;
	
	int nCol = 0;
  GetCell(nCol, nRow, &cell);
	nData1 = cell.GetOtherDataInt();

	nCol = 1;
  GetCell(nCol, nRow, &cell);
	nData2 = cell.GetOtherDataInt();
}

int CSetGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

void CSetGrid::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if (row != -1)
		OnRowChange(row, row);
}

void CSetGrid::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	if (updn && !processed)
  {		
    if ((row == -1) && (col == 0))
    {
			
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

int CSetGrid::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
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

void CSetGrid::EvenOddColorSetting()
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


void CSetGrid::OnRowChange(long oldrow, long newrow)
{
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
}

inline void CSetGrid::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CSetGrid::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(clrHeader);
}

void	CSetGrid::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	LONG dwData = MAKELONG(row, col);
	GetParent()->SendMessage(RMSG_DBCLICK, 0, dwData);
}

int CSetGrid::OnHint(int col, long row, int section, CString *string)
{
	CUGCell cell;

	/*
	GetCell((int)COL_NAME, row, &cell);
	*string = cell.GetOtherJongCode();
	*/

	return TRUE;
}

void CSetGrid::OnAdjustComponentSizes(RECT *grid, 
                                             RECT *topHdg,RECT *sideHdg,
                                             RECT *cnrBtn,RECT *vScroll,
                                             RECT *hScroll,RECT *tabs)
{
  CRect  rtGrid;
  double dWidthCell = 0.0;
		  
  if (!m_bCallAdjustOneTime)
  {
    rtGrid = CRect(grid);
		dWidthCell = rtGrid.Width() - 1 - Head_Width[0];;

		SetColWidth(0, (int)Head_Width[0]);
		SetColWidth(1, (int)dWidthCell);

	  m_bCallAdjustOneTime = TRUE;
    AdjustComponentSizes();
    m_bCallAdjustOneTime = FALSE;
  }
}
