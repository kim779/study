// CodeGrid.cpp: implementation of the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid_Comm.h"

#include "../resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern HINSTANCE g_hInstance;

#define _countof(array) (sizeof(array)/sizeof(array[0]))

const		LPCTSTR		HEADNAME_KEYWORD[] = { _T("상황문"), _T("상황문"), _T("상황문"), _T("강도") };
const		int				HEADWIDTH_KEYWORD[] = { 75, 75, 75, 45 };

const		LPCTSTR		HEADNAME_CODE[] = { _T("종목코드"), _T("종목명") };
const		LPCTSTR		HEADNAME_VOL[] = { _T("시작"), _T("거래량") };
const		LPCTSTR		HEADNAME_PRICE[] = { _T("시작"), _T("거래대금") };
const		int				HEADWIDTH_SUBSET[] = { 60, 80 };


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

BEGIN_MESSAGE_MAP(CGrid_Comm,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid_Comm::CGrid_Comm()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	
	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;
	
	m_bCallAdjustOneTime = FALSE;
	
	m_bShowCol = TRUE;
	m_nHideCol = 1;
}

CGrid_Comm::~CGrid_Comm()
{
	m_fontDefault.DeleteObject();
	m_penGridLine.DeleteObject();
	
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

void CGrid_Comm::SetInit(CString strType, ST_GRIDCOLOR stGridColor)
{
	m_clrHeader = stGridColor.clrGridHeader;
	m_clrSelRow = stGridColor.clrGridHilight;
	
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
	
	SetVScrollMode(UG_SCROLLTRACKING);
	SetHScrollMode(UG_SCROLLNORMAL);
	
	HeaderSetup(strType);
    
	SetHighlightRow(TRUE);
	SetUserSizingMode(FALSE);
	
	m_bCallAdjustOneTime = FALSE;
	AdjustComponentSizes();
}

void CGrid_Comm::HeaderSetup(CString strType)
{
	ClearAll();
	
	int nColCnt;
	if (strType == "KEYWORD")
		nColCnt = _countof(HEADNAME_KEYWORD);
	else if ((strType == "CODE") || (strType == "VOL") ||(strType == "PRICE"))
		nColCnt = _countof(HEADWIDTH_SUBSET);
	
	SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		
		if (strType == "KEYWORD")
		{
			QuickSetText(nCol, -1, HEADNAME_KEYWORD[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_KEYWORD[nCol]));
		}
		else if (strType == "CODE")
		{
			QuickSetText(nCol, -1, HEADNAME_CODE[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_SUBSET[nCol]));
		}
		else if (strType == "VOL")
		{
			QuickSetText(nCol, -1, HEADNAME_VOL[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_SUBSET[nCol]));
		}
		else if (strType == "PRICE")
		{
			QuickSetText(nCol, -1, HEADNAME_PRICE[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_SUBSET[nCol]));
		}
		
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}
	
	m_strType = strType;
	
	RedrawAll();
}

int CGrid_Comm::SetAdd_Row(KeywordData stKeywordData, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	CString strTemp;
	
	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = 0;
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
	
	nCol = 1;
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
	
	
	nCol = 2;
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
	
	
	nCol = 3;
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

void CGrid_Comm::SetDelete_Row(int nRow)
{
	DeleteRow(nRow);
	RedrawAll();
	
	if (GetNumberRows() > 0)
	{
		if (m_nLastSelRow > GetNumberRows())
			OnRowChange(m_nLastSelRow, GetNumberRows() - 1);
	}
	else
		m_nLastSelRow = -1;
}

void CGrid_Comm::GetFind_Keyword(CString strWord, CUIntArray &iaRow)
{
	iaRow.RemoveAll();

	CUGCell cell;
	int nCol = 0;
	
	CString strTitle;
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetCell(nCol, nRow, &cell);
		strTitle = cell.GetText();

		if (strTitle.Find(strWord, 0) > -1)
			iaRow.Add(nRow);
	}
}

void CGrid_Comm::SetGoto(int nRow, BOOL bSelete /*= FALSE*/)
{
	GotoRow(nRow);
	OnRowChange(m_nLastSelRow, nRow);
}

int CGrid_Comm::GetSelectRow()
{
	return m_nLastSelRow;
}

void CGrid_Comm::GetData_Row(int nRow, KeywordData	&stKeywordData)
{
	CUGCell cell;

	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strTitle = cell.GetText();

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strName_S = cell.GetText();
	stKeywordData.strCode_S = cell.GetOtherJongCode();
	stKeywordData.nPoint_S = cell.GetOtherDataInt();

	nCol = 2;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strName_V = cell.GetText();
	stKeywordData.strCode_V = cell.GetOtherJongCode();
	stKeywordData.nPoint_V = cell.GetOtherDataInt();

	CString strTitle;
	strTitle = stKeywordData.strName_S;
	if ((stKeywordData.strName_V).IsEmpty() == FALSE)
		strTitle += "+" + stKeywordData.strName_V;
	stKeywordData.strTitle = strTitle;

	nCol = 3;
	GetCell(nCol, nRow, &cell);
	stKeywordData.strCode = cell.GetOtherJongCode();
	stKeywordData.nBasePoint = cell.GetOtherDataInt();

	stKeywordData.nType = (int)cell.GetOtherDataLong();
}

void CGrid_Comm::SetHideCol(int nCol, BOOL bShow)
{
	m_bShowCol = bShow;
	m_nHideCol = nCol;
	
	AdjustComponentSizes();
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

void CGrid_Comm::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	CUGCell	cell;

	if (row != -1)
	{
		if (updn == 1)
			OnRowChange(m_nLastSelRow, row);
	}
}

void CGrid_Comm::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
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

int CGrid_Comm::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
	CString strCell1, strCell2;
	
	if ((m_strType == "VOL") || (m_strType == "PRICE"))
	{
		if (cell1->m_nOtherData > cell2->m_nOtherData)
			nResult = 1;
		else if (cell1->m_nOtherData < cell2->m_nOtherData)
			nResult = -1;
	}
	else
		nResult = cell1->m_string.Compare(cell2->m_string);
	
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
      cell.SetBackColor(clrBk);
			
      //## Line Color ##
      cell.SetBorderColor(&m_penGridLine);
		  cell.SetBorder(UG_BDR_RTHIN);

			SetCell(nCol, nRow, &cell);
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
	
	CWnd *pwndParent = GetParent();
	if (m_strType == "KEYWORD")
	{
		if (newrow > -1)
		{
			KeywordData	stKeywordData;
			GetData_Row(newrow, stKeywordData);
			pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_ITEM_CLK, (LPARAM)&stKeywordData);
		}
	}
	else if ((m_strType == "CODE") || (m_strType == "VOL") ||(m_strType == "PRICE"))
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_ITEM_CLK, (LPARAM)m_nLastSelRow);
	
	RedrawAll();
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

void	CGrid_Comm::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	if (m_strType == "KEYWORD")
	{
		KeywordData	stKeywordData;
		GetData_Row(row, stKeywordData);
		GetParent()->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_ITEM_DBCLK, (LPARAM)&stKeywordData);
	}
}

//extern CString dll_GetStockDetailInfo(CString strCode, int nType);
void CGrid_Comm::SetHint_Code(int nRow)
{
	CString strTip;
	CUGCell cell;
	GetCell(1, nRow, &cell);
	
	if (m_tipInfo.GetShowState() == FALSE)
	{
    /*
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
	*/
	}
}

int CGrid_Comm::OnHint(int col, long row, int section, CString *string)
{
	if (m_strType == "RESULT")
	{
		if (col != 1)
			return FALSE;
		
		SetHint_Code(row);
		return FALSE;
	}
	else if (m_strType == "KEYWORD")
	{
		CUGCell cell;
		GetCell(0, row, &cell);
		
		*string = cell.GetText();
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
	if (m_strType == "KEYWORD")
	{
		ntotWidth += HEADWIDTH_KEYWORD[3];
		
		/*
		nColCnt = _countof(HEADWIDTH_KEYWORD);
		for (nCol = 0; nCol < _countof(HEADWIDTH_KEYWORD); nCol++)
		{
		if (nCol == 0)
        continue;
		
		  ntotWidth += HEADWIDTH_RESULT[nCol];
		  }
		*/
	}
	else if ((m_strType == "CODE") || (m_strType == "VOL") ||(m_strType == "PRICE"))
		ntotWidth += HEADWIDTH_SUBSET[0];
    
	rtGrid = CRect(grid);
	dWidthGrid = rtGrid.Width() - 1 - ntotWidth;
	if (m_strType == "KEYWORD")
	{
		for (nCol = 0; nCol < _countof(HEADWIDTH_KEYWORD) - 1; nCol++)
		{
			if (nCol == 0)
				SetColWidth(nCol, (int)dWidthGrid);
			else if (nCol == 3)
				SetColWidth(nCol, (int)HEADWIDTH_KEYWORD[3]);
			else
				SetColWidth(nCol, 0);
		}
	}
	else if ((m_strType == "CODE") || (m_strType == "VOL") ||(m_strType == "PRICE"))
	{
		SetColWidth(0, (int)HEADWIDTH_SUBSET[0]);
		SetColWidth(1, (int)dWidthGrid);
	}
	
	m_bCallAdjustOneTime = TRUE;
	AdjustComponentSizes();
	m_bCallAdjustOneTime = FALSE;
}


void CGrid_Comm::GetData_Row(int nRow, CString &strData1, CString &strData2)
{
	CUGCell cell;
	
	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	strData1 = cell.GetText();
	
	nCol = 1;
	GetCell(nCol, nRow, &cell);
	strData2 = cell.GetText();
}

void CGrid_Comm::GetData_Row(int nRow, int &nData1, int &nData2)
{
	CUGCell cell;
	
	int nCol = 0;
	GetCell(nCol, nRow, &cell);
	nData1 = cell.GetOtherDataInt();
	
	nCol = 1;
	GetCell(nCol, nRow, &cell);
	nData2 = cell.GetOtherDataInt();
}

void CGrid_Comm::SetAdd_Row(CString strData1, CString strData2, BOOL bRedraw)
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

void CGrid_Comm::SetAdd_Row(int nData1, int nData2, BOOL bRedraw)
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

void CGrid_Comm::SetModify_Row(int nRow, CString strData1, CString strData2)
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

void CGrid_Comm::SetModify_Row(int nRow, int nData1, int nData2)
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

void CGrid_Comm::SetModify_Row(int nData1, int nData2)
{
	CUGCell cell;
	COLORREF clrBk;
	
	int nRow;
	BOOL bModify = FALSE;
	int nData_1, nData_2;
	for (nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetData_Row(nRow, nData_1, nData_2);
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
