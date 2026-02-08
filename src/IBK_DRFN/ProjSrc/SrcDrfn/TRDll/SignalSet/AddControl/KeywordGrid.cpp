// KeywordGrid.cpp: implementation of the CKeywordGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KeywordGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		Head_Name[] = { _T("상황문"), _T("상황문"), _T("상황문"), _T("강도") };
const		int				Head_Width[] = { 75, 75, 75, 30 };

//CString DEFVAL_SELECTCHK = "1";

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

#define COL_TITLE				0
#define COL_TITLE_S			1
#define COL_TITLE_V			2
#define COL_POINT				3


BEGIN_MESSAGE_MAP(CKeywordGrid,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CKeywordGrid::CKeywordGrid()
{
  LOGFONT	  logFont;
  GETFONT(logFont, 12, 400, m_fontDefault);
  GETFONT(logFont, 12, 600, m_fontBold);

	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;

	m_bCallAdjustOneTime = FALSE;
	m_nLastSelRow = -1;
}

CKeywordGrid::~CKeywordGrid()
{
  m_fontDefault.DeleteObject();
  m_fontBold.DeleteObject();

  m_penGridLine.DeleteObject();
}

void CKeywordGrid::ClearAll()
{
	m_nLastSelRow = -1;

	for (int nCount = 0; nCount < GetNumberRows(); nCount++)
		RemoveDataSource(nCount);
	
	while (GetNumberRows()) 
	{
		m_GI->m_defDataSource->DeleteRow(0);
		m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}

	RedrawAll();
}

BOOL CKeywordGrid::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}

void CKeywordGrid::OnSetup()
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
  m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);

  SetVScrollMode(UG_SCROLLTRACKING);
	SetHScrollMode(UG_SCROLLNORMAL);

	SetUserSizingMode(FALSE);
  SetHighlightRow(TRUE);
  
  HeaderSetup();
}

void CKeywordGrid::SetInit()
{
  m_bCallAdjustOneTime = FALSE;
	m_nLastSelRow = -1;

  AdjustComponentSizes();
  RedrawAll();
}

void CKeywordGrid::HeaderSetup()
{
  ClearAll();

	int nColCnt = _countof(Head_Name);
  SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
    QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
    QuickSetText(nCol, -1, Head_Name[nCol]);
		if (((int)COL_TITLE == nCol) || ((int)COL_POINT == nCol))
			SetColWidth(nCol, (int)Head_Width[nCol]);
		else
			SetColWidth(nCol, 0);
    QuickSetBackColor(nCol, -1, clrHeader);
    QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}

  m_bCallAdjustOneTime = FALSE;  
  AdjustComponentSizes();

  RedrawAll();
}

int CKeywordGrid::SetAdd_Row(KeywordData	stKeywordData, BOOL bRedraw)
{
  CUGCell cell;
	COLORREF clrBk;
	CString strTemp;
	
  AppendRow();
  int nRow = GetNumberRows() - 1;
	int nCol;
	
	GetEvenOddBkColor(nRow, clrBk);

	nCol = (int)COL_TITLE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
  cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(stKeywordData.strTitle);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	if (stKeywordData.nType == 0)
		cell.SetTextColor(BLACK);
	else if (stKeywordData.nType == 1)
		cell.SetTextColor(BLUE);

  SetCell(nCol, nRow, &cell);

	nCol = (int)COL_TITLE_S;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
  cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(stKeywordData.strName_S);
	cell.SetOtherJongCode(stKeywordData.strCode_S);
	cell.SetOtherDataInt(stKeywordData.nPoint_S);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
  SetCell(nCol, nRow, &cell);


	nCol = (int)COL_TITLE_V;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
  cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(stKeywordData.strName_V);
	cell.SetOtherJongCode(stKeywordData.strCode_V);
	cell.SetOtherDataInt(stKeywordData.nPoint_V);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	if (stKeywordData.nType == 0)
		cell.SetTextColor(BLACK);
	else if (stKeywordData.nType == 1)
		cell.SetTextColor(BLUE);

  SetCell(nCol, nRow, &cell);

	
	nCol = (int)COL_POINT;
  GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
  cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	strTemp.Format("%d", stKeywordData.nBasePoint);
  if (stKeywordData.nBasePoint > 0)
    strTemp = "+" + strTemp;
	cell.SetText(strTemp);
	cell.SetOtherJongCode(stKeywordData.strCode);
	cell.SetOtherDataInt(stKeywordData.nBasePoint);
	cell.SetOtherDataLong(stKeywordData.nType);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	if (stKeywordData.nType == 0)
		cell.SetTextColor(BLACK);
	else if (stKeywordData.nType == 1)
		cell.SetTextColor(BLUE);

  SetCell(nCol, nRow, &cell);

  if (bRedraw == TRUE) 
		RedrawAll();

	return nRow;
}

void CKeywordGrid::SetDelete_Row(int nRow)
{
	DeleteRow(nRow);

	//# Sel Row;


}

void CKeywordGrid::GetFind_Keyword(CString strWord, CUIntArray &iaRow)
{
	iaRow.RemoveAll();

	CUGCell cell;
	int nCol = (int)COL_TITLE;
	
	CString strTitle;
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetCell(nCol, nRow, &cell);
		strTitle = cell.GetText();

		if (strTitle.Find(strWord, 0) > -1)
			iaRow.Add(nRow);
	}
}

void CKeywordGrid::SetGoto(int nRow, BOOL bSelete /*= FALSE*/)
{
	GotoRow(nRow);
	OnRowChange(m_nLastSelRow, nRow);
}

int CKeywordGrid::GetSelectRow()
{
	return m_nLastSelRow;
}

void CKeywordGrid::GetData_Row(int nRow, KeywordData	&stKeywordData)
{
	CUGCell cell;

	int nCol = COL_TITLE;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strTitle = cell.GetText();

	nCol = COL_TITLE_S;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strName_S = cell.GetText();
	stKeywordData.strCode_S = cell.GetOtherJongCode();
	stKeywordData.nPoint_S = cell.GetOtherDataInt();

	nCol = COL_TITLE_V;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strName_V = cell.GetText();
	stKeywordData.strCode_V = cell.GetOtherJongCode();
	stKeywordData.nPoint_V = cell.GetOtherDataInt();

	CString strTitle;
	strTitle = stKeywordData.strName_S;
	if ((stKeywordData.strName_V).IsEmpty() == FALSE)
		strTitle += "+" + stKeywordData.strName_V;
	stKeywordData.strTitle = strTitle;

	nCol = COL_POINT;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strCode = cell.GetOtherJongCode();
	stKeywordData.nBasePoint = cell.GetOtherDataInt();

	stKeywordData.nType = (int)cell.GetOtherDataLong();
}

int CKeywordGrid::OnCanSizeTopHdg()
{
	return FALSE;
}

void CKeywordGrid::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1) && (m_nLastSelRow != row))
		OnRowChange(m_nLastSelRow, row);
}

void CKeywordGrid::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	if (updn && !processed)
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

void CKeywordGrid::EvenOddColorSetting()
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

void CKeywordGrid::SetSelectRow(int nRow)
{
  m_nLastSelRow = -1;
	OnRowChange(m_nLastSelRow, nRow);
}

void CKeywordGrid::OnRowChange(long oldrow, long newrow)
{
	COLORREF clrBk;
	CUGCell	cell;

	GetEvenOddBkColor(oldrow, clrBk);
	for (int nCol = 0; nCol < GetNumberCols(); nCol++)
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

	KeywordData	stKeywordData;
	GetData_Row(newrow, stKeywordData);
	GetParent()->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_ITEM_CLK, (LPARAM)&stKeywordData);

	RedrawAll();
}

int CKeywordGrid::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
	CString strCell1, strCell2;

	nResult = cell1->m_string.Compare(cell2->m_string);

	/*
	if (m_nSortCol == 1)
		nResult = cell1->m_string.Compare(cell2->m_string);
	else
	{
		if (cell1->m_lOtherData > cell2->m_lOtherData)
			nResult = 1;
		else if (cell1->m_lOtherData < cell2->m_lOtherData)
			nResult = -1;
  }
	*/

	nResult *= (-(flags * 2 - 3));
	return nResult;
}

inline void CKeywordGrid::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CKeywordGrid::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(clrHeader);
}

void	CKeywordGrid::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	KeywordData	stKeywordData;
	GetData_Row(row, stKeywordData);
	GetParent()->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_ITEM_DBCLK, (LPARAM)&stKeywordData);
}

int CKeywordGrid::OnHint(int col, long row, int section, CString *string)
{
	CUGCell cell;

	/*
	GetCell((int)COL_NAME, row, &cell);
	*string = cell.GetOtherJongCode();
	*/

	return TRUE;
}

void CKeywordGrid::OnAdjustComponentSizes(RECT *grid, 
                                             RECT *topHdg,RECT *sideHdg,
                                             RECT *cnrBtn,RECT *vScroll,
                                             RECT *hScroll,RECT *tabs)
{
  if (m_bCallAdjustOneTime == TRUE)
    return;

  CRect  rtGrid;
  double dWidthCell = 0.0;
  
  rtGrid = CRect(grid);
  dWidthCell = (rtGrid.Width() - 1) - Head_Width[(int)COL_POINT];
	
	SetColWidth(COL_TITLE, (int)dWidthCell);
	SetColWidth(COL_POINT, (int)Head_Width[(int)COL_POINT]);
  
  m_bCallAdjustOneTime = TRUE;
  AdjustComponentSizes();
  m_bCallAdjustOneTime = FALSE;
}

void CKeywordGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	  
}

void CKeywordGrid::SetDrewBox(CPaintDC *pDC, CRect reData)
{
  CPen *pOldPen;
  CPen penLine(PS_SOLID, 0, COLOR_OUTLINE);

  pOldPen = pDC->SelectObject(&penLine);
  pDC->MoveTo(reData.left, reData.top);
  pDC->LineTo(reData.right, reData.top);
  pDC->LineTo(reData.right, reData.bottom);
  pDC->LineTo(reData.left, reData.bottom);
  pDC->LineTo(reData.left, reData.top);

  pDC->SelectObject(pOldPen);
  penLine.DeleteObject();
}