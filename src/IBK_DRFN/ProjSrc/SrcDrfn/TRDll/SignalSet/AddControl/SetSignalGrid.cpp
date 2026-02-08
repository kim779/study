// SetSignalGrid.cpp: implementation of the CSetSignalGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetSignalGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		Head_Name[] = { _T("적용항목"), _T("강도"), 
_T("신호등"), _T("티커"), _T("신호주문"), _T("작동상태") };
//const		int				Head_Width[] = { 75, 50, 45, 45, 60, 50 };
const		int				Head_Width[] = { 75, 55, 0, 0, 0, 55 };

const		LPCTSTR		COMBO_POINTDATA = "+3\n+2\n+1\n0\n-1\n-2\n-3\n";
const		LPCTSTR		COMBO_POINTLIST[] = { "+3", "+2", "+1", "0", "-1", "-2", "-3" };

#define ROW_DEFHEIGHT									18
#define ROW_HEAD_DEFHEIGHT						20

const   COLORREF	clrHeader		 = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine	 = COLOR_GRID_LINE;
const   COLORREF  clrBtn			 = COLOR_GRID_BTN;


#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
#define BLACK		RGB(0,0,0)


#define COL_TITEL			0
#define COL_POINT			1
#define COL_SIGNAL		2
#define COL_TICKER		3
#define COL_ORDER			4
#define COL_STATE			5


BEGIN_MESSAGE_MAP(CSetSignalGrid,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSetSignalGrid::CSetSignalGrid()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);

	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);

	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;

	m_bCallAdjustOneTime = TRUE;
	m_nLastSelRow = -1;

	m_nIndex_SetSignal = 0;

	m_nTickerVal = 0;
	m_nOrderVal = 0;
	m_nStateVal = 1;
}

CSetSignalGrid::~CSetSignalGrid()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();

	m_penGridLine.DeleteObject();
}

void CSetSignalGrid::ClearAll()
{
	m_nLastSelRow = -1;
	m_nIndex_SetSignal = 0;

	m_nTickerVal = 0;
	m_nOrderVal = 0;
	m_nStateVal = 1;

	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
		RemoveDataSource(nRow);

	while(GetNumberRows()) 
	{
		m_GI->m_defDataSource->DeleteRow(0);
		m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}

	SetNumberRows(0);
	RedrawAll();
}

BOOL CSetSignalGrid::PreTranslateMessage(MSG* pMsg) 
{	
	return CWnd::PreTranslateMessage(pMsg);
}

void CSetSignalGrid::OnSetup()
{

}

void CSetSignalGrid::SetInit()
{
	SetSH_Width(0);
	SetSH_NumberCols(0);

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
	SetHighlightRow(TRUE);

	HeaderSetup();

	m_bCallAdjustOneTime = FALSE;
	m_nLastSelRow = -1;

	AdjustComponentSizes();
	RedrawAll();
}

void CSetSignalGrid::HeaderSetup()
{
	ClearAll();

	int nSet = 1; 
	int nColCnt = _countof(Head_Name);
	SetNumberCols(nColCnt);

	if (nSet == 0)
	{
		SetTH_NumberRows(3);
		SetTH_Height(ROW_HEAD_DEFHEIGHT * 2);

		int nRow, nCol;
		for (nRow = -1; nRow > -3; nRow--)
		{
			SetTH_RowHeight(nRow, ROW_HEAD_DEFHEIGHT);

			for (nCol = 0; nCol < nColCnt; nCol++)
			{
				QuickSetAlignment(nCol, nRow, UG_ALIGNVCENTER | UG_ALIGNCENTER);

				if ((nRow == -2) && (nCol == (int)COL_SIGNAL))
					QuickSetText(nCol, nRow, "신호통보처");
				else
					QuickSetText(nCol, nRow, Head_Name[nCol]);

				SetColWidth(nCol, (int)(Head_Width[nCol]));
				QuickSetBackColor(nCol, nRow, m_clrHeader);
				QuickSetTextColor(nCol, nRow, COLOR_GRID_HEADER_TEXT);
				QuickSetFont(nCol, nRow, &m_fontDefault);
			}
		}

		JoinCells(COL_TITEL, -2, COL_TITEL, -1);
		JoinCells(COL_POINT, -2, COL_POINT, -1);
		JoinCells(COL_STATE, -2, COL_STATE, -1);

		JoinCells(COL_SIGNAL, -2, COL_ORDER, -2);
	}
	else if (nSet == 1)
	{
		SetTH_NumberRows(1);
		SetTH_Height(ROW_HEAD_DEFHEIGHT);

		int nRow, nCol;
		nRow = -1;
		SetTH_RowHeight(nRow, ROW_HEAD_DEFHEIGHT);

		for (nCol = 0; nCol < nColCnt; nCol++)
		{
			QuickSetAlignment(nCol, nRow, UG_ALIGNVCENTER | UG_ALIGNCENTER);

			if ((nRow == -2) && (nCol == (int)COL_SIGNAL))
				QuickSetText(nCol, nRow, "신호통보처");
			else
				QuickSetText(nCol, nRow, Head_Name[nCol]);

			SetColWidth(nCol, (int)(Head_Width[nCol]));
			QuickSetBackColor(nCol, nRow, m_clrHeader);
			QuickSetTextColor(nCol, nRow, COLOR_GRID_HEADER_TEXT);
			QuickSetFont(nCol, nRow, &m_fontDefault);
		}
	}

	RedrawAll();
}

void CSetSignalGrid::SetDelete_Row(int nRow)
{
	CString strTitle = GetRowToTitle(nRow);

	for (nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		if (strTitle == GetRowToTitle(nRow))
		{
			m_GI->m_defDataSource->DeleteRow(nRow);
			m_GI->m_numberRows = m_GI->m_numberRows - 1;
			if (m_GI->m_numberRows > 0)
			{
				if (nRow == 0)
					OnRowChange(m_nLastSelRow, 0);
				else if (nRow == m_GI->m_numberRows - 1)
					OnRowChange(nRow, nRow - 1);
				else
					OnRowChange(nRow, nRow);
			}
			break;
		}
	}

	if (m_GI->m_numberRows == 0)
		ClearAll();
}

/*
void CSetSignalGrid::SetAdd_Row(SetSignalData	stSetSignalData, BOOL bRedraw)
{
CUGCell cell;
COLORREF clrBk;

AppendRow();
int nRow = GetNumberRows() - 1;
int nCol;

CString strTemp;
GetEvenOddBkColor(nRow, clrBk);

nCol = (int)COL_TITEL;
GetCell(nCol, nRow, &cell);
cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
cell.SetFont(&m_fontDefault);

cell.SetText(stSetSignalData.strTitle);
cell.SetOtherDataInt(m_nIndex_SetSignal);
cell.SetOtherDataCellText(stSetSignalData.strKey);

cell.SetBorderColor(&m_penGridLine);
cell.SetBorder(UG_BDR_RTHIN);
cell.SetBackColor(clrBk);

SetCell(nCol, nRow, &cell);


nCol = (int)COL_POINT;
GetCell(nCol, nRow, &cell);
cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
cell.SetFont(&m_fontDefault);

strTemp.Format("%d", stSetSignalData.nPoint);
cell.SetText(strTemp);
cell.SetOtherDataInt(stSetSignalData.nPoint);

cell.SetBorderColor(&m_penGridLine);
cell.SetBorder(UG_BDR_RTHIN);

COLORREF clrPointBk, clrPointText;
GetPointToColor(clrBk, stSetSignalData.nPoint, clrPointBk, clrPointText);
cell.SetBackColor(clrPointBk);
cell.SetTextColor(clrPointText);

SetCell(nCol, nRow, &cell);

if (stSetSignalData.nType != 1)		//# Keyword, User
{
QuickSetCellType(nCol, nRow, UGCT_DROPLIST);
QuickSetCellTypeEx(nCol, nRow, UGCT_NORMALELLIPSIS);
QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);
QuickSetText(nCol, nRow, COMBO_POINTLIST[stSetSignalData.nPoint + 3]);
QuickSetLabelText(nCol, nRow, COMBO_POINTDATA);
}


nCol = (int)COL_SIGNAL;
GetCell(nCol, nRow, &cell);
cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
cell.SetFont(&m_fontDefault);

strTemp = (stSetSignalData.nSend_Signal == 1) ? "O":"X";
cell.SetText(strTemp);
cell.SetOtherDataInt(stSetSignalData.nSend_Signal);

cell.SetBorderColor(&m_penGridLine);
cell.SetBorder(UG_BDR_RTHIN);
cell.SetBackColor(clrBk);

SetCell(nCol, nRow, &cell);


nCol = (int)COL_TICKER;
GetCell(nCol, nRow, &cell);
cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
cell.SetFont(&m_fontDefault);

strTemp.Format("%d", stSetSignalData.nSend_Ticker);
cell.SetText(strTemp);
cell.SetOtherDataInt(stSetSignalData.nSend_Ticker);

cell.SetBorderColor(&m_penGridLine);
cell.SetBorder(UG_BDR_RTHIN);
cell.SetBackColor(clrBk);

SetCell(nCol, nRow, &cell);


int buttonIndex = AddCellType(&m_button);
QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
QuickSetCellType(nCol, nRow, buttonIndex);
QuickSetCellTypeEx(nCol, nRow, UGCT_BUTTONNOFOCUS);
strTemp = (stSetSignalData.nSend_Ticker == 1) ? "O":"X";
QuickSetText(nCol, nRow, strTemp);
QuickSetBackColor(nCol, nRow, clrBtn);
QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

nCol = (int)COL_ORDER;
GetCell(nCol, nRow, &cell);
cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
cell.SetFont(&m_fontDefault);

cell.SetOtherDataInt(stSetSignalData.nSend_Order);
cell.SetBorderColor(&m_penGridLine);
cell.SetBorder(UG_BDR_RTHIN);
cell.SetBackColor(clrBk);

SetCell(nCol, nRow, &cell);

QuickSetCellType(nCol, nRow, UGCT_DROPLIST);
QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);
QuickSetLabelText(nCol, nRow, COMBO_ORDERDATA);
QuickSetText(nCol, nRow, COMBO_ORDERLIST[stSetSignalData.nSend_Order]);


nCol = (int)COL_STATE;
GetCell(nCol, nRow, &cell);
cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
cell.SetFont(&m_fontDefault);

strTemp.Format("%d", stSetSignalData.nState);
cell.SetText(strTemp);
cell.SetOtherDataInt(stSetSignalData.nState);

cell.SetBorderColor(&m_penGridLine);
cell.SetBorder(UG_BDR_RTHIN);
cell.SetBackColor(clrBk);

SetCell(nCol, nRow, &cell);


buttonIndex = AddCellType(&m_button);
QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
QuickSetCellType(nCol, nRow, buttonIndex);
QuickSetCellTypeEx(nCol, nRow, UGCT_BUTTONNOFOCUS);
strTemp = (stSetSignalData.nState == 1) ? "On":"Off";
QuickSetText(nCol, nRow, strTemp);
QuickSetBackColor(nCol, nRow, clrBtn);
QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

m_nIndex_SetSignal++;

if (bRedraw == TRUE) 
RedrawAll();
}
*/

void CSetSignalGrid::SetAdd_Row(SetSignalData	*pstSetSignalData, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;

	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol;

	CString strTemp;
	GetEvenOddBkColor(nRow, clrBk);

	nCol = (int)COL_TITEL;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	cell.SetText(pstSetSignalData->strTitle);
	cell.SetOtherDataInt(m_nIndex_SetSignal);
	cell.SetOtherDataCellText(pstSetSignalData->strKey);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	SetCell(nCol, nRow, &cell);


	nCol = (int)COL_POINT;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	cell.SetText(COMBO_POINTLIST[pstSetSignalData->nPoint * -1 + 3]);
	cell.SetOtherDataInt(pstSetSignalData->nPoint);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);

	COLORREF clrPointBk, clrPointText;
	GetPointToColor(clrBk, pstSetSignalData->nPoint, clrPointBk, clrPointText);
	cell.SetBackColor(clrPointBk);
	cell.SetTextColor(clrPointText);
	SetCell(nCol, nRow, &cell);


	/*
	if (pstSetSignalData->nType != 1)		//# Keyword, User
	{
	QuickSetCellType(nCol, nRow, UGCT_DROPLIST);
	QuickSetCellTypeEx(nCol, nRow, UGCT_NORMALELLIPSIS);
	QuickSetAlignment(nCol, nRow, UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	QuickSetText(nCol, nRow, COMBO_POINTLIST[pstSetSignalData->nPoint * -1 + 3]);
	QuickSetLabelText(nCol, nRow, COMBO_POINTDATA);
	}
	*/

	QuickSetCellType(nCol, nRow, UGCT_DROPLIST);
	QuickSetCellTypeEx(nCol, nRow, UGCT_NORMALELLIPSIS);
	QuickSetAlignment(nCol, nRow, UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	QuickSetText(nCol, nRow, COMBO_POINTLIST[pstSetSignalData->nPoint * -1 + 3]);
	QuickSetLabelText(nCol, nRow, COMBO_POINTDATA);

	nCol = (int)COL_SIGNAL;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	strTemp = (pstSetSignalData->nSend_Signal == 1) ? "O":"X";
	cell.SetText(strTemp);
	cell.SetOtherDataInt(pstSetSignalData->nSend_Signal);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	SetCell(nCol, nRow, &cell);


	nCol = (int)COL_TICKER;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	strTemp.Format("%d", pstSetSignalData->nSend_Ticker);
	cell.SetText(strTemp);
	cell.SetOtherDataInt(pstSetSignalData->nSend_Ticker);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	SetCell(nCol, nRow, &cell);


	int buttonIndex = AddCellType(&m_button);
	QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(nCol, nRow, buttonIndex);
	QuickSetCellTypeEx(nCol, nRow, UGCT_BUTTONNOFOCUS);
	strTemp = (pstSetSignalData->nSend_Ticker == 1) ? "O":"X";
	QuickSetText(nCol, nRow, strTemp);
	QuickSetBackColor(nCol, nRow, clrBtn);
	QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 


	nCol = (int)COL_ORDER;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	cell.SetOtherDataInt(pstSetSignalData->nSend_Order);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	SetCell(nCol, nRow, &cell);

	buttonIndex = AddCellType(&m_button);
	QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(nCol, nRow, buttonIndex);
	QuickSetCellTypeEx(nCol, nRow, UGCT_BUTTONNOFOCUS);
	strTemp = (pstSetSignalData->nSend_Order == 1) ? "O":"X";
	QuickSetText(nCol, nRow, strTemp);
	QuickSetBackColor(nCol, nRow, clrBtn);
	QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);


	nCol = (int)COL_STATE;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	strTemp.Format("%d", pstSetSignalData->nState);
	cell.SetText(strTemp);
	cell.SetOtherDataInt(pstSetSignalData->nState);

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);

	SetCell(nCol, nRow, &cell);


	buttonIndex = AddCellType(&m_button);
	QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(nCol, nRow, buttonIndex);
	QuickSetCellTypeEx(nCol, nRow, UGCT_BUTTONNOFOCUS);
	strTemp = (pstSetSignalData->nState == 1) ? "On":"Off";
	QuickSetText(nCol, nRow, strTemp);
	QuickSetBackColor(nCol, nRow, clrBtn);
	QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 

	m_nIndex_SetSignal++;

	if (bRedraw == TRUE) 
		RedrawAll();
}

CString CSetSignalGrid::GetRowToTitle(int nRow)
{
	CUGCell cell;

	int nCol = (int)COL_TITEL;
	GetCell(nCol, nRow, &cell);
	return cell.GetText();
}

int CSetSignalGrid::GetTitleToRow(CString strTitle)
{
	CUGCell cell;

	CString strData;
	int nCol = (int)COL_TITEL;
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetCell(nCol, nRow, &cell);
		strData = cell.GetText();
		if (strTitle == strData)
			return nRow;
	}

	return -1;
}

BOOL CSetSignalGrid::GetHaveState(CString strTitle)
{
	BOOL bResult = FALSE;

	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		if (strTitle == GetRowToTitle(nRow))
		{
			bResult = TRUE;
			break;
		}
	}

	return bResult;
}

CString CSetSignalGrid::GetRowToKey(int nRow)
{
	CString strResult = "";

	CUGCell cell;

	int nCol = (int)COL_TITEL;
	GetCell(nCol, nRow, &cell);
	strResult = cell.GetOtherDataCellText();

	return strResult;
}

void CSetSignalGrid::GetData_Row(int nRow, SetSignalData	&stSetSignalData)
{
	CUGCell cell;

	int nCol = (int)COL_TITEL;
	GetCell(nCol, nRow, &cell);
	stSetSignalData.strTitle = cell.GetText();
	stSetSignalData.strKey = cell.GetOtherDataCellText();

	int nIndex_SetSignal;
	nIndex_SetSignal = cell.GetOtherDataInt();

	nCol = (int)COL_POINT;
	GetCell(nCol, nRow, &cell);
	stSetSignalData.nPoint = cell.GetOtherDataInt();

	nCol = (int)COL_SIGNAL;
	GetCell(nCol, nRow, &cell);
	stSetSignalData.nSend_Signal = cell.GetOtherDataInt();

	nCol = (int)COL_TICKER;
	GetCell(nCol, nRow, &cell);
	stSetSignalData.nSend_Ticker = cell.GetOtherDataInt();

	nCol = (int)COL_ORDER;
	GetCell(nCol, nRow, &cell);
	stSetSignalData.nSend_Order = cell.GetOtherDataInt();

	nCol = (int)COL_STATE;
	GetCell(nCol, nRow, &cell);
	stSetSignalData.nState = cell.GetOtherDataInt();
}

void CSetSignalGrid::GetPointToColor(COLORREF clrDefult, int nPoint,
									 COLORREF &clrPointBk, COLORREF &clrPointText)
{
	clrPointBk = clrDefult;

	if (nPoint == -3)
	{
		clrPointBk = RGB(52, 70, 133);
		clrPointText = WHITE;
	}
	else if (nPoint == -2)
	{
		clrPointBk = RGB(0, 114, 188);
		clrPointText = WHITE;
	}
	else if (nPoint == -1)
	{
		clrPointBk = RGB(0, 174, 239);
		clrPointText = WHITE;
	}
	else if (nPoint == 0)
	{
		clrPointBk = RGB(135, 135, 135);
		clrPointText = WHITE;
	}
	else if (nPoint == 1)
	{
		clrPointBk = RGB(250, 166, 26);
		clrPointText = WHITE;
	}
	else if (nPoint == 2)
	{
		clrPointBk = RGB(245, 130, 32);
		clrPointText = WHITE;
	}
	else if (nPoint == 3)
	{
		clrPointBk = RGB(237, 28, 36);
		clrPointText = WHITE;
	}
}

void CSetSignalGrid::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1) && (m_nLastSelRow != row))
		OnRowChange(row, row);
}

void CSetSignalGrid::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	CUGCell	cell;
	CWnd *pwndParent = GetParent();
	if(col == 5) // 작동상태 버튼 더블 클릭시 삭제되는 현상 수정
		return;
	
	pwndParent->SendMessage(WM_NOTIFY_SIGNALGRID, (WPARAM)NM_ITEM_DBCLK, NULL);
}

void CSetSignalGrid::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	CWnd *pwndParent = GetParent();

	CUGCell	cell;
	int nRow;
	CString strData;
	CUIntArray iaData;

	if (updn && !processed)
	{	
		if (col == COL_SIGNAL)
		{


		}
		else if (col == COL_TICKER)
		{
			m_nTickerVal = (m_nTickerVal == 1) ? 0 : 1;

			for (nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(col, nRow, &cell);
				strData = (m_nTickerVal == 1) ? "O":"X";
				cell.SetText(strData);
				cell.SetOtherDataInt(m_nTickerVal);
				SetCell(col, nRow, &cell);
			}

			iaData.RemoveAll();
			iaData.Add(col);
			iaData.Add(m_nTickerVal);
			pwndParent->SendMessage(WM_NOTIFY_SIGNALGRID, (WPARAM)NM_TH_CLK, (LPARAM)&iaData);
		}
		else if (col == COL_ORDER)
		{
			m_nOrderVal = (m_nOrderVal == 1) ? 0 : 1;

			for (nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(col, nRow, &cell);
				strData = (m_nOrderVal == 1) ? "O":"X";
				cell.SetText(strData);
				cell.SetOtherDataInt(m_nOrderVal);
				SetCell(col, nRow, &cell);
			}

			iaData.RemoveAll();
			iaData.Add(col);
			iaData.Add(m_nOrderVal);
			pwndParent->SendMessage(WM_NOTIFY_SIGNALGRID, (WPARAM)NM_TH_CLK, (LPARAM)&iaData);
		}
		else if (col == COL_STATE)
		{
			m_nStateVal = (m_nStateVal == 1) ? 0 : 1;

			for (nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(col, nRow, &cell);
				strData = (m_nStateVal == 1) ? "On":"Off";
				cell.SetText(strData);
				cell.SetOtherDataInt(m_nStateVal);
				SetCell(col, nRow, &cell);
			}

			iaData.RemoveAll();
			iaData.Add(col);
			iaData.Add(m_nStateVal);
			pwndParent->SendMessage(WM_NOTIFY_SIGNALGRID, (WPARAM)NM_TH_CLK, (LPARAM)&iaData);
		}
		else if ((col == COL_TITEL) || (col = COL_POINT))
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

			SetEvenOddColor();
		}

		RedrawAll();
	}
}

void CSetSignalGrid::SetEvenOddColor()
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
			if ((nCol == COL_TICKER) || (nCol == COL_STATE))
				cell.SetBackColor(clrBtn);
			else if (nCol == COL_POINT)
			{
				int nPoint = cell.GetOtherDataInt();

				COLORREF clrPointBk, clrPointText;
				GetPointToColor(clrBk, nPoint, clrPointBk, clrPointText);
				cell.SetBackColor(clrPointBk);
				cell.SetTextColor(clrPointText);
			}
			else
				cell.SetBackColor(clrBk);

			//## Line Color ##
			cell.SetBorderColor(&m_penGridLine);
			cell.SetBorder(UG_BDR_RTHIN);

			SetCell(nCol, nRow, &cell);
		}
	}
}

int CSetSignalGrid::OnCellTypeNotify(long ID, int col, long row, long msg, long param)
{
	CWnd *pwndParent = GetParent();
	CStringArray	saData;

	RedrawAll();
	Invalidate();

	CString strTemp, strData;
	CUGCell	cell;
	if (row != -1 && col != -1)
	{
		if (col == COL_POINT)
		{
			if (msg == UGCT_DROPLISTSTART)
			{

			}
			else if (msg == UGCT_DROPLISTSELECT)
			{
				strData = *(CString*)param;

				if (col == COL_POINT)
				{
					COLORREF clrBk;
					GetEvenOddBkColor(row, clrBk);

					COLORREF clrPointBk, clrPointText;
					GetPointToColor(clrBk, atoi(strData), clrPointBk, clrPointText);

					GetCell(col, row, &cell);
					cell.SetBackColor(clrPointBk);
					cell.SetTextColor(clrPointText);

					cell.SetText(strData);
					cell.SetOtherDataInt(atoi(strData));
					SetCell(col, row, &cell);

					QuickSetCellType(col, row, UGCT_DROPLIST);
					QuickSetCellTypeEx(col, row, UGCT_NORMALELLIPSIS);
					QuickSetAlignment(col, row, UG_ALIGNRIGHT | UG_ALIGNVCENTER);
					QuickSetText(col, row, COMBO_POINTLIST[atoi(strData) * -1 + 3]);
					QuickSetLabelText(col, row, COMBO_POINTDATA);

					saData.RemoveAll();
					GetCell(COL_TITEL, row, &cell);
					saData.Add(cell.GetText());
					saData.Add(cell.GetOtherDataCellText());
					strTemp.Format("%d", col);
					saData.Add(strTemp);
					strTemp.Format("%d", atoi(strData));
					saData.Add(strTemp);
					pwndParent->SendMessage(WM_NOTIFY_SIGNALGRID, (WPARAM)NM_ITEM_CELLTYPE, (LPARAM)&saData);
				}
			}
		}
		else if ((col == COL_TICKER) || (col == COL_STATE) || (col == COL_ORDER))
		{
			if (msg == UGCT_BUTTONCLICK)
			{
				GetCell(col, row, &cell);
				int nData = cell.GetOtherDataInt();
				int nData_Change = (nData == 1) ? 0 : 1;

				//# Data Modify!
				if ((col == COL_TICKER) || (col == COL_ORDER))
					strTemp = (nData_Change == 1) ? "O":"X";
				else if (col == COL_STATE)
					strTemp = (nData_Change == 1) ? "On":"Off";

				cell.SetText(strTemp);
				cell.SetOtherDataInt(nData_Change);
				SetCell(col, row, &cell);

				saData.RemoveAll();
				GetCell(COL_TITEL, row, &cell);
				saData.Add(cell.GetText());
				saData.Add(cell.GetOtherDataCellText());
				strTemp.Format("%d", col);
				saData.Add(strTemp);
				strTemp.Format("%d", nData_Change);
				saData.Add(strTemp);
				pwndParent->SendMessage(WM_NOTIFY_SIGNALGRID, (WPARAM)NM_ITEM_CELLTYPE, (LPARAM)&saData);
			}
		}
	}

	return TRUE;
}


void CSetSignalGrid::OnRowChange(long oldrow, long newrow)
{
	COLORREF clrBk;
	CUGCell	cell;

	GetEvenOddBkColor(oldrow, clrBk);
	for (int nCol = 0; nCol < GetNumberCols(); nCol++)
	{
		if (oldrow != -1)
		{
			GetCell(nCol, oldrow, &cell);
			if ((nCol == COL_TICKER) || (nCol == COL_ORDER) || (nCol == COL_STATE))
				cell.SetBackColor(clrBtn);
			else if (nCol == COL_POINT)
			{
				int nPoint = cell.GetOtherDataInt();

				COLORREF clrPointBk, clrPointText;
				GetPointToColor(clrBk, nPoint, clrPointBk, clrPointText);
				cell.SetBackColor(clrPointBk);
				cell.SetTextColor(clrPointText);
			}
			else
				cell.SetBackColor(clrBk);

			SetCell(nCol, oldrow, &cell);
		}

		if (newrow != -1)
		{
			GetCell(nCol, newrow, &cell);
			cell.SetBackColor(m_clrSelRow);
			if (nCol == COL_POINT)
				cell.SetTextColor(BLACK);
			SetCell(nCol, newrow, &cell);
		}
	}

	m_nLastSelRow = newrow;

	RedrawAll();
}

int CSetSignalGrid::OnSortEvaluate(CUGCell *cell1, CUGCell *cell2, int flags)
{
	int  nResult = 0;
	CString strCell1, strCell2;

	if (m_nSortCol == (int)COL_TITEL)
		nResult = cell1->m_string.Compare(cell2->m_string);
	else if (m_nSortCol == (int)COL_POINT)
	{
		if (cell1->m_nOtherData > cell2->m_nOtherData)
			nResult = 1;
		else if (cell1->m_nOtherData < cell2->m_nOtherData)
			nResult = -1;
	}

	nResult *= (-(flags * 2 - 3));
	return nResult;
}

int CSetSignalGrid::GetSelectRow()
{
	return m_nLastSelRow;
}

inline void CSetSignalGrid::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;

	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CSetSignalGrid::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(m_clrHeader);
}

int CSetSignalGrid::OnHint(int col, long row, int section, CString *string)
{
	CUGCell cell;

	GetCell((int)COL_TITEL, row, &cell);
	*string = cell.GetText();

	return TRUE;
}

void CSetSignalGrid::OnAdjustComponentSizes(RECT *grid, 
											RECT *topHdg,RECT *sideHdg,
											RECT *cnrBtn,RECT *vScroll,
											RECT *hScroll,RECT *tabs)
{
	if (m_bCallAdjustOneTime == TRUE)
		return;

	CRect  rtGrid;
	double dWidthCell = 0.0;

	int ntotWidth = 0;
	for (int i = 1; i < _countof(Head_Width); i++)
		ntotWidth += Head_Width[i];

	rtGrid = CRect(grid);
	dWidthCell = rtGrid.Width() - ntotWidth - 1;
	SetColWidth(0, (int)dWidthCell);

	SetColWidth(1, (int)Head_Width[1]);
	SetColWidth(2, (int)Head_Width[2]);
	SetColWidth(3, (int)Head_Width[3]);
	SetColWidth(4, (int)Head_Width[4]);
	SetColWidth(5, (int)Head_Width[5]);

	m_bCallAdjustOneTime = TRUE;
	AdjustComponentSizes();
	m_bCallAdjustOneTime = FALSE;
}

void CSetSignalGrid::ChangeGridColor(ST_GRIDCOLOR stGridColor)
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