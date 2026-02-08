// Grid_Field.cpp: implementation of the CGrid_Field class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid_Field.h"


#include "../DefineFiles/client_typedef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "../resource.h"
/*
"MatchValSource"
"MatchValDest"
"QuickMakeSource"
"QuickMakeDest"
"Folder"
"File"
"FieldEditSource"
"FieldEditDest"
*/


const		LPCTSTR		Head_Name_MatchValSource[] = { _T(""), _T("필드명"), _T("strField"), 
_T("Element"), _T("Field Index") };
const		int				Head_Width_MatchValSource[] = { 20, 120, 0, 0, 0 };

const		LPCTSTR		Head_Name_MatchValDest[] = { _T("필드명"), _T("Opt"), _T("strField"), 
_T("Element"), _T("nField Index"), _T("nGridSourceRow") };
const		int				Head_Width_MatchValDest[] = { 160, 40, 0, 0, 0, 0 };


const		LPCTSTR		Head_Name_QuickMakeSource[] = { _T(""), _T("사용자조건명"), _T("strFilePath"), 
_T("strFolderPath"), _T("strType"), _T("bAlertReg") };
const		int				Head_Width_QuickMakeSource[] = { 20, 200, 0, 0, 0, 0 };

const		LPCTSTR		Head_Name_QuickMakeDest[] = { _T("보기"), _T("퀵버튼명"), _T("strFileName"), 
_T("strFilePath"), _T("strFolderPath"), _T("strType"), 
_T("bAlertReg") };
const		int				Head_Width_QuickMakeDest[] = { 30, 130, 0, 0, 0, 0, 0 };

const		LPCTSTR		Head_Name_Folder[] = { _T("그룹명"), _T("Path"), _T("개수"), 
_T("Lock"), _T("선택") };
const		int				Head_Width_Folder[] = { 110, 0, 30, 0, 30 };

const		LPCTSTR		Head_Name_File[] = { _T("조건명"), _T("Path"), _T("FolderPath"), _T("Type") };
const		int				Head_Width_File[] = { 180, 0, 0, 0 };

const		LPCTSTR		Head_Name_FieldEditSource[] = { _T("선택 리스트"), _T("선택 리스트"), _T("ID"), _T("ID") };
const		int				Head_Width_FieldEditSource[] = { 129, 129, 0, 0 };

const		LPCTSTR		Head_Name_FieldEditDest[] = { _T("선택 항목"), _T("ID"), _T("Index") };
const		int				Head_Width_FieldEditDest[] = { 150, 0, 0 };

#define ROW_DEFHEIGHT									18
#define ROW_HEAD_DEFHEIGHT						20

#define DEFVAL_SELECTCHK "0"

const   COLORREF  clrBtn_type1			= RGB(192, 192, 192);
const   COLORREF  clrBtn_type2			= RGB(192, 192, 180);
const   COLORREF  clrBtnText				= RGB(0, 0, 0);

const   COLORREF	clrHeader		 = COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow    = COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow   = COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow	   = COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine	 = COLOR_GRID_LINE;

BEGIN_MESSAGE_MAP(CGrid_Field,CUGCtrl)
//{{AFX_MSG_MAP(ShowGrid)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code !
//}}AFX_MSG_MAP
//	ON_COMMAND_RANGE(ID_LINKMENU, ID_LINKMENU + 40, OnLinkSelect)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid_Field::CGrid_Field(CWnd* pwndParent, CString strType, COLORREF clrHeader)
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	
	m_bCallAdjustOneTime = FALSE;
	
	m_pwndParent = pwndParent;
	m_strType = strType;
	
	m_nSelectRow = -1;
	m_nSelectCol = -1;

	m_clrHeader = clrHeader;
}

CGrid_Field::~CGrid_Field()
{
	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();
	
	m_penGridLine.DeleteObject();
}

void CGrid_Field::ClearAll()
{
	for (int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		RemoveDataSource(nRow);
	}
		
	while (GetNumberRows()) 
	{
		m_GI->m_defDataSource->DeleteRow(0);
		m_GI->m_numberRows = m_GI->m_numberRows - 1;
	}
	
	SetNumberRows(0);
	RedrawAll();
	
	m_nSelectRow = -1;
	m_nSelectCol = -1;
}

BOOL CGrid_Field::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}

// ###############################################################################
// Event를 처리하는 함수들입니다.
void CGrid_Field::OnSetup()
{
	SetSH_Width(0);
	SetSH_NumberCols(0);
	SetHighlightRow(FALSE);
	
	SetDefRowHeight(ROW_DEFHEIGHT);
	SetTH_Height(ROW_HEAD_DEFHEIGHT);
	
	SetCancelMode(FALSE);
	EnableMenu(TRUE);
	
	//EnableUpdate(TRUE);
	EnableUpdate(FALSE);
	UseHints(FALSE);
	
	EnableExcelBorders(FALSE);
	
	SetVScrollMode(UG_SCROLLTRACKING);
	SetHScrollMode(UG_SCROLLNORMAL);
	
	SetUserSizingMode(FALSE);
	//  LockColumns(1);
	
	HeaderSetup(m_nColCnt);
	AdjustComponentSizes();
}

void CGrid_Field::OnHeaderSetup()
{
	HeaderSetup(m_nColCnt);
}

void CGrid_Field::HeaderSetup(int &nColCnt)
{
	ClearAll();
	
	if (m_strType == "MatchValSource")
		nColCnt = _countof(Head_Name_MatchValSource);
	else if (m_strType == "MatchValDest")
	{
		nColCnt = _countof(Head_Name_MatchValDest);
		
		for (int nIndex = 0; nIndex < _countof(MATCHVAL_OPT_LIST); nIndex++)
			m_saOpt.Add((CString)MATCHVAL_OPT_LIST[nIndex]);
	}
	else if (m_strType == "QuickMakeSource")
		nColCnt = _countof(Head_Name_QuickMakeSource);
	else if (m_strType == "QuickMakeDest")
		nColCnt = _countof(Head_Name_QuickMakeDest);
	else if (m_strType == "Folder")
		nColCnt = _countof(Head_Name_Folder);
	else if (m_strType == "File")
		nColCnt = _countof(Head_Name_File);
	else if (m_strType == "FieldEditSource")
		nColCnt = _countof(Head_Name_FieldEditSource);
	else if (m_strType == "FieldEditDest")
		nColCnt = _countof(Head_Name_FieldEditDest);
	
	SetNumberCols(nColCnt);
	for(int nCol = 0; nCol < nColCnt; nCol++ )
	{
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		
		if (m_strType == "MatchValSource")
		{
			QuickSetText(nCol, -1, Head_Name_MatchValSource[nCol]);
			SetColWidth(nCol, (int)(Head_Width_MatchValSource[nCol]));
		}
		else if (m_strType == "MatchValDest")
		{
			QuickSetText(nCol, -1, Head_Name_MatchValDest[nCol]);
			SetColWidth(nCol, (int)(Head_Width_MatchValDest[nCol]));
		}
		else if (m_strType == "QuickMakeSource")
		{
			QuickSetText(nCol, -1, Head_Name_QuickMakeSource[nCol]);
			SetColWidth(nCol, (int)(Head_Width_QuickMakeSource[nCol]));
		}
		else if (m_strType == "QuickMakeDest")
		{
			QuickSetText(nCol, -1, Head_Name_QuickMakeDest[nCol]);
			SetColWidth(nCol, (int)(Head_Width_QuickMakeDest[nCol]));
		}
		else if (m_strType == "Folder")
		{
			QuickSetText(nCol, -1, Head_Name_Folder[nCol]);
			SetColWidth(nCol, (int)(Head_Width_Folder[nCol]));
		}
		else if (m_strType == "File")
		{
			QuickSetText(nCol, -1, Head_Name_File[nCol]);
			SetColWidth(nCol, (int)(Head_Width_File[nCol]));
		}
		else if (m_strType == "FieldEditSource")
		{
			QuickSetText(nCol, -1, Head_Name_FieldEditSource[nCol]);
			SetColWidth(nCol, (int)(Head_Width_FieldEditSource[nCol]));
		}
		else if (m_strType == "FieldEditDest")
		{
			QuickSetText(nCol, -1, Head_Name_FieldEditDest[nCol]);
			SetColWidth(nCol, (int)(Head_Width_FieldEditDest[nCol]));
		}
		//SetRowHeight(nCol, m_nFontHeight);
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
		//QuickSetFont(nCol, -1, &m_fontBold);
	}
	
	if (m_strType != "FieldEditDest")
		SetHighlightRow(TRUE);
	
	RedrawAll();
}

void CGrid_Field::SetCellData(int nRow, int nCol, CString strData)
{
	QuickSetText(nCol, nRow, strData);
	RedrawAll();
}


int CGrid_Field::OnCanSizeTopHdg()
{
	return FALSE;
}


void CGrid_Field::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if (( row != -1 && col != -1) && (updn == 1))
	{
		if (m_strType == "FieldEditSource")
			OnCellChange(m_nSelectCol, m_nSelectRow, col, row);
		else
			OnCellChange(0, m_nSelectRow, -1, row);
		
		GridEventInfo	stGridEventInfo;
		
		stGridEventInfo.strGridType = m_strType;
		
		stGridEventInfo.nCol = col;
		stGridEventInfo.nRow = row;
		stGridEventInfo.strType = ""; //#=> BUTTON / CHECK / COMBO
		stGridEventInfo.strData = "";
		
		m_pwndParent->SendMessage(WM_NOTIFY_GRIDEVENT, (WPARAM)NM_CLICK_L, (LPARAM)&stGridEventInfo);
	}
}

void CGrid_Field::OnCellChange(long oldCol, long oldRow,
							  long newCol, long newRow)
{
	if ((oldCol == newCol) && (oldRow == newRow))
		return;
	
	COLORREF clrBk;
	CUGCell	cell;
	
	int nCol;
	if (oldRow > -1) 
	{
		GetEvenOddBkColor(oldRow, clrBk);
		if (oldCol > -1)
		{
			GetCell(oldCol, oldRow, &cell);
			cell.SetBackColor(clrBk);
			
			SetCell(oldCol, oldRow, &cell);
		}
		else
		{
			for (nCol = 0; nCol < m_nColCnt; nCol++)
			{
				GetCell(nCol, oldRow, &cell);
				cell.SetBackColor(clrBk);
				
				if ((m_strType == "MatchValSource") && (nCol == 1))
				{
					if (cell.GetOtherDataInt() == 1)
						cell.SetFont(&m_fontBold);
					else
						cell.SetFont(&m_fontDefault);
				}
				
				SetCell(nCol, oldRow, &cell);
			}
		}
	}
	
	if (newRow > -1) 
	{
		if (newCol > -1)
		{
			GetCell(newCol, newRow, &cell);
			cell.SetBackColor(clrSelRow);
			
			SetCell(newCol, newRow, &cell);
		}
		else
		{
			for (nCol = 0; nCol < m_nColCnt; nCol++)
			{
				GetCell(nCol, newRow, &cell);
				cell.SetBackColor(clrSelRow);
				
				if ((m_strType == "MatchValSource") && (nCol == 1))
				{
					if (cell.GetOtherDataInt() == 1)
						cell.SetFont(&m_fontBold);
					else
						cell.SetFont(&m_fontDefault);
				}
				
				SetCell(nCol, newRow, &cell);
			}
		}
	}
	
	m_nSelectCol = newCol;
	m_nSelectRow = newRow;
	
	RedrawAll();
}

void CGrid_Field::OnRowChange(long oldRow, long newRow)
{
	if (m_nSelectRow == newRow)
		return;
	
	if (m_strType == "FieldEditSource")
		OnCellChange(m_nSelectCol, oldRow, m_nSelectCol, newRow);
	else
		OnCellChange(-1, m_nSelectRow, -1, newRow);
	
	GridEventInfo	stGridEventInfo;
	
	stGridEventInfo.strGridType = m_strType;
	stGridEventInfo.nCol = m_nSelectCol;
	stGridEventInfo.nRow = newRow;
	stGridEventInfo.strType = ""; //#=> BUTTON / CHECK / COMBO
	stGridEventInfo.strData = "";
	
	m_pwndParent->SendMessage(WM_NOTIFY_GRIDEVENT, (WPARAM)NM_CLICK_L, (LPARAM)&stGridEventInfo);
}

inline void CGrid_Field::GetEvenOddBkColor(int nRow, COLORREF &clrBk)
{
	clrBk = clrEvenRow;
	
	if (nRow % 2) 
		clrBk = clrOddRow;
}

void CGrid_Field::OnSysColorChange() 
{
	CUGCtrl::OnSysColorChange();
	m_GI->m_hdgDefaults->SetBackColor(m_clrHeader);
}

void	CGrid_Field::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	GridEventInfo	stGridEventInfo;
	
	stGridEventInfo.strGridType = m_strType;
	
	stGridEventInfo.nCol = col;
	stGridEventInfo.nRow = row;
	stGridEventInfo.strType = ""; //#=> BUTTON / CHECK / COMBO
	
	if(m_strType == "FieldEditSource")
	{
		CString strCheck = QuickGetText(col, row);
		if(strCheck == "0")
			stGridEventInfo.strData = "1";
		else
		{
			stGridEventInfo.strData = "0";
			QuickSetText(col, row, stGridEventInfo.strData);
			Invalidate();
		}	
	}
	
	m_pwndParent->SendMessage(WM_NOTIFY_GRIDEVENT, (WPARAM)NM_DBCLICK_L, (LPARAM)&stGridEventInfo);
}


int CGrid_Field::OnCellTypeNotify(long ID, int col, long row, long msg, long param)
{
	CUGCell cell;
	
	
	RedrawAll();
	Invalidate();
	
	BOOL bSendMsg = FALSE;
	CString strTemp;
	if (row != -1 && col != -1)
	{
		GridEventInfo		stGridEventInfo;
		
		stGridEventInfo.strGridType = m_strType;
		
		stGridEventInfo.nCol = col;
		stGridEventInfo.nRow = row;
		if (msg == UGCT_CHECKBOXSET)
		{
			stGridEventInfo.strType = "CHECK";
			strTemp.Format("%d", (int)param);
			stGridEventInfo.strData = strTemp;
			
			bSendMsg = TRUE;
		}
		else if (msg == UGCT_DROPLISTSTART)
		{
			
		}
		else if (msg == UGCT_DROPLISTSELECT)
		{
			stGridEventInfo.strType = "COMBO";
			stGridEventInfo.strData = "";
			
			if ((m_strType == "MatchValDest") && (col == 1))
			{
				CString strTemp;
				CString strListText = *((CString *)param);
				QuickSetText(col, row, strListText);
				
				for (int nIndex = 0; nIndex < _countof(MATCHVAL_OPT_LIST); nIndex++)
				{
					if (strListText == MATCHVAL_OPT_LIST[nIndex])
					{
						strTemp.Format("%d", nIndex);
						stGridEventInfo.strData = strTemp;
						
						GetCell(1, row, &cell);
						cell.SetOtherDataInt(nIndex);
						SetCell(1, row, &cell);
						break;
					}
				}
			}
			bSendMsg = TRUE;
		}
		else if (msg == UGCT_BUTTONCLICK)
		{
			stGridEventInfo.strType = "BUTTON";
			stGridEventInfo.strData = "";
			
			bSendMsg = TRUE;
		}
		
		if (bSendMsg == TRUE)
			m_pwndParent->SendMessage(WM_NOTIFY_GRIDEVENT, (WPARAM)NM_CLICK_CELLTYPE, (LPARAM)&stGridEventInfo);
	}
	
	return TRUE;
}

void CGrid_Field::OnAdjustComponentSizes(RECT *grid, 
										RECT *topHdg,RECT *sideHdg,
										RECT *cnrBtn,RECT *vScroll,
										RECT *hScroll,RECT *tabs)
{
	CRect  rtGrid;
	if (!m_bCallAdjustOneTime)
	{
		rtGrid = CRect(grid);
		
		if (m_strType == "MatchValSource")
		{
			SetColWidth(0, Head_Width_MatchValSource[0]);
			SetColWidth(1, rtGrid.Width() - Head_Width_MatchValSource[0] - 1);
		}
		else if (m_strType == "MatchValDest")
		{
			SetColWidth(0, rtGrid.Width() - Head_Width_MatchValDest[1] - 1);
			SetColWidth(1, Head_Width_MatchValDest[1]);
		}
		else if (m_strType == "QuickMakeSource")
		{
			SetColWidth(0, Head_Width_QuickMakeSource[0]);
			SetColWidth(1, rtGrid.Width() - Head_Width_QuickMakeSource[0] - 1);
		}
		else if (m_strType == "QuickMakeDest")
		{
			SetColWidth(0, Head_Width_QuickMakeDest[0]);
			SetColWidth(1, rtGrid.Width() - Head_Width_QuickMakeDest[0] - 1);
		}
		else if (m_strType == "Folder")
		{
			SetColWidth(0, rtGrid.Width() - (Head_Width_Folder[2] + Head_Width_Folder[4] + 1));
			SetColWidth(2, Head_Width_Folder[2]);
			SetColWidth(4, Head_Width_Folder[4]);
		}
		else if (m_strType == "File")
			SetColWidth(0, rtGrid.Width() - 1);
		else if (m_strType == "FieldEditSource")
		{
			int nColWidth;
			nColWidth = (rtGrid.Width() - 1) / 2;
			
			SetColWidth(0, nColWidth);
			SetColWidth(1, nColWidth);
		}
		else if (m_strType == "FieldEditDest")
			SetColWidth(0, rtGrid.Width() - 1);
		
		m_bCallAdjustOneTime = TRUE;
		AdjustComponentSizes();
		m_bCallAdjustOneTime = FALSE;
	}
}

void CGrid_Field::SetSelRow(int nRow)
{
	OnRowChange(m_nSelectRow, nRow);
}

void CGrid_Field::SetData_MatchValSource(int nRow, CString strElement, int nFieldIndex, 
										HeadInfoType	stHeadInfoType, BOOL bInvalidate)
{
	CUGCell cell;
	COLORREF clrBk;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	int nCol = 0;
	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
	QuickSetText(nCol, nRow, DEFVAL_SELECTCHK);
	QuickSetBackColor(nCol, nRow, clrBk);
	QuickSetFont(nCol, nRow, &m_fontDefault);
	QuickSetBorderColor(nCol, nRow, &m_penGridLine);
	QuickSetBorder(nCol, nRow, UG_BDR_RTHIN);
	
	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetBackColor(clrBk);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	if (strElement.IsEmpty() == TRUE)
		strTemp = stHeadInfoType.strHeadName;
	else
		strTemp = stHeadInfoType.strHeadName + "(" + strElement + ")";
	cell.SetText(strTemp);
	cell.SetOtherDataInt(0);
	
	SetCell(nCol, nRow, &cell);
	
	nCol = 2;
	QuickSetText(nCol, nRow, stHeadInfoType.strHeadName);
	
	nCol = 3;
	QuickSetText(nCol, nRow, strElement);
	
	nCol = 4;
	strTemp.Format("%d", nFieldIndex);
	QuickSetText(nCol, nRow, strTemp);
	
	if (bInvalidate == TRUE)
	{
		SetSelRow(nRow);
		RedrawAll();
	}
}

void CGrid_Field::SetData_MatchValSource(int nRow, BOOL bUser)
{
	CUGCell cell;
	
	GetCell(1, nRow, &cell);
	cell.SetOtherDataInt(bUser);
	
	if (bUser == TRUE)
		cell.SetFont(&m_fontBold);
	else
		cell.SetFont(&m_fontDefault);
	
	SetCell(1, nRow, &cell);
	
	GetCell(0, nRow, &cell);
	cell.SetText("0");
	SetCell(0, nRow, &cell);
	
	RedrawAll();
}

BOOL CGrid_Field::GetData_MatchValSource(int nRow, MatchInfo	&stMatchInfo)
{
	BOOL bResult = TRUE;
	if (atoi(QuickGetText(0, nRow)) == 0)
		bResult = FALSE;
	
	stMatchInfo.nOptIndex = 0;
	
	CUGCell cell;
	GetCell(1, nRow, &cell);
	
	stMatchInfo.bUser = cell.GetOtherDataInt();
	
	stMatchInfo.strField = QuickGetText(2, nRow);
	stMatchInfo.strElement = QuickGetText(3, nRow);
	
	stMatchInfo.nFieldIndex = atoi(QuickGetText(4, nRow));
	stMatchInfo.nGridSourceRow = nRow;
	
	return bResult;
}

void CGrid_Field::SetData_MatchValDest(int nRow, MatchInfo	stMatchInfo, BOOL bInvalidate)
{
	CUGCell cell;
	COLORREF clrBk;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	GetCell(0, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetBackColor(clrBk);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	
	if (stMatchInfo.strElement.IsEmpty() == TRUE)
		strTemp = stMatchInfo.strField;
	else
		strTemp = stMatchInfo.strField + "(" + stMatchInfo.strElement + ")";
	cell.SetText(strTemp);
	SetCell(0, nRow, &cell);
	
	/*
	//# Source Grid Check!
	m_pgridSource->SetCellCheck(stMatchInfo.nGridSourceRow, 0, TRUE);
	m_pgridSource->SetScroll();
	*/
	
	if (nRow == GetNumberRows() - 1)
	{
		QuickSetCellType(1, nRow, UGCT_NORMAL);
		QuickSetText(1, nRow, "");
		if (GetNumberRows() - 1 > 0)
		{
			QuickSetCellType(1, nRow - 1, UGCT_DROPLIST);
			QuickSetAlignment(1, nRow - 1, UG_ALIGNCENTER | UG_ALIGNVCENTER);
			QuickSetFont(1, nRow - 1, &m_fontDefault);
			
			QuickSetLabelText(1, nRow - 1, MATCHVAL_OPT_DATA);
			
			MatchInfo	stMatchInfo_up;
			GetData_MatchValDest(nRow - 1, stMatchInfo_up);
			QuickSetText(1, nRow - 1, MATCHVAL_OPT_LIST[stMatchInfo_up.nOptIndex]);
		}
	}
	else
	{
		QuickSetCellType(1, nRow, UGCT_DROPLIST);
		QuickSetAlignment(1, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);
		QuickSetFont(1, nRow, &m_fontDefault);
		
		QuickSetLabelText(1, nRow, MATCHVAL_OPT_DATA);
		QuickSetText(1, nRow, MATCHVAL_OPT_LIST[stMatchInfo.nOptIndex]);
	}
	QuickSetBackColor(1, nRow, clrBk);
	
	GetCell(1, nRow, &cell);
	cell.SetOtherDataInt(stMatchInfo.nOptIndex);
	SetCell(1, nRow, &cell);
	
	QuickSetText(2, nRow, stMatchInfo.strField);
	QuickSetText(3, nRow, stMatchInfo.strElement);
	
	strTemp.Format("%d", stMatchInfo.nFieldIndex);
	QuickSetText(4, nRow, strTemp);
	
	strTemp.Format("%d", stMatchInfo.nGridSourceRow);
	QuickSetText(5, nRow, strTemp);
	
	if (bInvalidate == TRUE)
	{
		SetSelRow(nRow);
		RedrawAll();
	}
}

void CGrid_Field::GetData_MatchValDest(int nRow, MatchInfo	&stMatchInfo)
{
	stMatchInfo.nRow = nRow;
	stMatchInfo.bUser = TRUE;
	
	stMatchInfo.nOptIndex = 0;
	
	CUGCell cell;
	GetCell(1, nRow, &cell);
	stMatchInfo.nOptIndex = cell.GetOtherDataInt();
	
	stMatchInfo.strField = QuickGetText(2, nRow);
	stMatchInfo.strElement = QuickGetText(3, nRow);
	
	stMatchInfo.nFieldIndex = atoi(QuickGetText(4, nRow));
	stMatchInfo.nGridSourceRow = atoi(QuickGetText(5, nRow));
}

//void CGrid_Field::SetData_QuickMakeDest(int nRow, QuickList stQuickList, BOOL bInvalidate)
//{
//	COLORREF clrBk;
//	GetEvenOddBkColor(nRow, clrBk);
//	
//	QuickSetCellType(0, nRow, UGCT_CHECKBOX);
//	QuickSetCellTypeEx(0, nRow, UGCT_CHECKBOXCHECKMARK);
//	QuickSetText(0, nRow, (stQuickList.bCheck == TRUE)?"1":"0");
//	QuickSetBackColor(0, nRow, clrBk);
//	QuickSetFont(0, nRow, &m_fontDefault);
//	
//	QuickSetBackColor(1, nRow, clrBk);
//	QuickSetFont(1, nRow, &m_fontDefault);
//	QuickSetText(1, nRow, stQuickList.strQuickName);
//	
//	QuickSetText(2, nRow, stQuickList.strFileName);
//	QuickSetText(3, nRow, stQuickList.strFilePath);
//	QuickSetText(4, nRow, stQuickList.strFolderPath);
//	QuickSetText(5, nRow, stQuickList.strType);
//	QuickSetText(6, nRow, (stQuickList.bAlertReg == TRUE) ? "TRUE" : "FALSE");
//	
//	if (bInvalidate == TRUE)
//	{
//		SetSelRow(nRow);
//		RedrawAll();
//	}
//}
//
//void CGrid_Field::GetData_QuickMakeDest(int nRow, QuickList &stQuickList)
//{
//	stQuickList.bCheck = (atoi(QuickGetText(0, nRow)) == 1)?TRUE:FALSE;
//	
//	stQuickList.strQuickName = QuickGetText(1, nRow);
//	stQuickList.strFileName = QuickGetText(2, nRow);
//	stQuickList.strFilePath = QuickGetText(3, nRow);
//	stQuickList.strFolderPath = QuickGetText(4, nRow);
//	stQuickList.strType = QuickGetText(5, nRow);
//	stQuickList.bAlertReg = (QuickGetText(6, nRow) == "TRUE") ? TRUE : FALSE;
//}

//void CGrid_Field::SetData_QuickMakeSource(int nRow, XMLTreeInfoFile stXMLTreeInfoFile, BOOL bInvalidate)
//{
//	CUGCell cell;
//	COLORREF clrBk;
//	GetEvenOddBkColor(nRow, clrBk);
//	
//	QuickSetCellType(0, nRow, UGCT_CHECKBOX);
//	QuickSetCellTypeEx(0, nRow, UGCT_CHECKBOXCHECKMARK);
//	QuickSetText(0, nRow, "0");
//	QuickSetBackColor(0, nRow, clrBk);
//	QuickSetFont(0, nRow, &m_fontDefault);
//	
//	QuickSetBackColor(1, nRow, clrBk);
//	if (stXMLTreeInfoFile.bQuickReg == TRUE)
//		QuickSetFont(1, nRow, &m_fontBold);
//	else
//		QuickSetFont(1, nRow, &m_fontDefault);
//	QuickSetText(1, nRow, stXMLTreeInfoFile.strFileName);
//	
//	QuickSetText(2, nRow, stXMLTreeInfoFile.strFilePath);
//	QuickSetText(3, nRow, stXMLTreeInfoFile.strFolderPath);
//	QuickSetText(4, nRow, stXMLTreeInfoFile.strType);
//	QuickSetText(5, nRow, stXMLTreeInfoFile.strAlertReg);
//	
//	if (bInvalidate == TRUE)
//	{
//		SetSelRow(nRow);
//		RedrawAll();
//	}
//}
//
//void CGrid_Field::GetData_QuickMakeSource(int nRow, XMLTreeInfoFile &stXMLTreeInfoFile)
//{
//	stXMLTreeInfoFile.bQuickReg = (QuickGetText(5, nRow) == "TRUE") ? TRUE : FALSE;
//	stXMLTreeInfoFile.strFileName = QuickGetText(1, nRow);
//	
//	stXMLTreeInfoFile.strFilePath = QuickGetText(2, nRow);
//	stXMLTreeInfoFile.strFolderPath = QuickGetText(3, nRow);
//	stXMLTreeInfoFile.strType = QuickGetText(4, nRow);
//	stXMLTreeInfoFile.strAlertReg = QuickGetText(5, nRow);
//}


//void CGrid_Field::SetData_Folder(int nRow, InfoFolder stInfoFolder, BOOL bInvalidate)
//{
//	CString strTemp;
//	
//	CUGCell cell;
//	COLORREF clrBk;
//	GetEvenOddBkColor(nRow, clrBk);
//	
//	int nCol = 0;
//	QuickSetBackColor(nCol, nRow, clrBk);
//	QuickSetFont(nCol, nRow, &m_fontDefault);
//	QuickSetText(nCol, nRow, stInfoFolder.strName);
//	QuickSetBorderColor(nCol, nRow, &m_penGridLine);
//	QuickSetBorder(nCol, nRow, UG_BDR_RTHIN);
//	
//	nCol = 1;
//	QuickSetText(nCol, nRow, stInfoFolder.strPath);
//	QuickSetBorderColor(nCol, nRow, &m_penGridLine);
//	QuickSetBorder(nCol, nRow, UG_BDR_RTHIN);
//	
//	nCol = 2;
//	QuickSetBackColor(nCol, nRow, clrBk);
//	QuickSetFont(nCol, nRow, &m_fontDefault);
//	QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);
//	strTemp.Format("%d", stInfoFolder.nFileCnt);
//	QuickSetText(nCol, nRow, strTemp);
//	QuickSetBorderColor(nCol, nRow, &m_penGridLine);
//	QuickSetBorder(nCol, nRow, UG_BDR_RTHIN);
//	
//	nCol = 3;
//	strTemp = (stInfoFolder.bLock == TRUE)?"1":"0";
//	QuickSetText(nCol, nRow, strTemp);
//	
//	nCol = 4;
//	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
//	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
//	QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER);
//	
//	QuickSetText(nCol, nRow, (stInfoFolder.bTarget == TRUE) ? "1" : "0");
//	QuickSetBackColor(nCol, nRow, clrBk);
//	QuickSetFont(nCol, nRow, &m_fontDefault);
//	
//	if (bInvalidate == TRUE)
//	{
//		SetSelRow(nRow);
//		RedrawAll();
//	}
//}
//
//BOOL CGrid_Field::GetData_Folder(int nRow, InfoFolder &stInfoFolder)
//{
//	if (((CString)QuickGetText(0, nRow)).IsEmpty() == TRUE)
//		return FALSE;
//	
//	stInfoFolder.strName = QuickGetText(0, nRow);
//	stInfoFolder.strPath = QuickGetText(1, nRow);
//	stInfoFolder.nFileCnt = atoi(QuickGetText(2, nRow));
//	
//	stInfoFolder.bLock = (atoi(QuickGetText(3, nRow)) == 1) ? TRUE : FALSE;
//	stInfoFolder.bTarget = (atoi(QuickGetText(4, nRow)) == 1) ? TRUE : FALSE;
//	
//	return TRUE;
//}


//void CGrid_Field::SetData_File(int nRow, XMLTreeInfoFile	stXMLTreeInfoFile, BOOL bInvalidate)
//{
//	CUGCell cell;
//	COLORREF clrBk;
//	GetEvenOddBkColor(nRow, clrBk);
//	
//	QuickSetCellType(0, nRow, UGCT_CHECKBOX);
//	QuickSetCellTypeEx(0, nRow, UGCT_CHECKBOXCHECKMARK);
//	QuickSetAlignment(0, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
//	
//	//QuickSetText(0, nRow, strCheck);
//	QuickSetText(0, nRow, "0");
//	QuickSetLabelText(0, nRow, stXMLTreeInfoFile.strFileName);
//	QuickSetBackColor(0, nRow, clrBk);
//	QuickSetFont(0, nRow, &m_fontDefault);
//	
//	GetCell(0, nRow, &cell);
//	cell.SetOtherDataCellText(stXMLTreeInfoFile.strFileName);
//	cell.SetBorderColor(&m_penGridLine);
//	cell.SetBorder(UG_BDR_RTHIN);
//	SetCell(0, nRow, &cell);
//	
//	QuickSetText(1, nRow, stXMLTreeInfoFile.strFilePath);
//	QuickSetText(2, nRow, stXMLTreeInfoFile.strFolderPath);
//	QuickSetText(3, nRow, stXMLTreeInfoFile.strType);
//	
//	if (bInvalidate == TRUE)
//	{
//		SetSelRow(nRow);
//		RedrawAll();
//	}
//}
//
//void CGrid_Field::GetData_File(int nRow, XMLTreeInfoFile	&stXMLTreeInfoFile)
//{
//	CUGCell cell;
//	
//	GetCell(0, nRow, &cell);
//	stXMLTreeInfoFile.strFileName = cell.GetOtherDataCellText();
//	SetCell(0, nRow, &cell);
//	
//	stXMLTreeInfoFile.strFilePath = QuickGetText(1, nRow);
//	stXMLTreeInfoFile.strFolderPath = QuickGetText(2, nRow);
//	stXMLTreeInfoFile.strType = QuickGetText(3, nRow);
//}



void CGrid_Field::SetData_FieldEditSource(int nCol, int nRow, CStringArray *psaData, BOOL bInvalidate)
{
	//# Check / Name / Id / Dest Row
	CString strCheck, strName, strId, strDestRow;
	strCheck = psaData->GetAt(0);
	strName = psaData->GetAt(1);
	strId = psaData->GetAt(2);
	strDestRow = psaData->GetAt(3);
	
	CUGCell cell;
	COLORREF clrBk;
	GetEvenOddBkColor(nRow, clrBk);
	
	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
	QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
	QuickSetText(nCol, nRow, strCheck);
	QuickSetLabelText(nCol, nRow, strName);
	QuickSetBackColor(nCol, nRow, clrBk);
	QuickSetFont(nCol, nRow, &m_fontDefault);
	
	GetCell(nCol, nRow, &cell);
	cell.SetOtherDataCellText(strName);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	SetCell(nCol, nRow, &cell);
	
	QuickSetText(nCol + 2, nRow, strId);
	
	GetCell(nCol + 2, nRow, &cell);
	cell.SetOtherDataCellText(strDestRow);
	SetCell(nCol + 2, nRow, &cell);
	
	if (bInvalidate == TRUE)
		RedrawAll();
}

void CGrid_Field::GetData_FieldEditSource(int nCol, int nRow, CStringArray &saData)
{
	saData.RemoveAll();
	
	CUGCell cell;
	
	//# Check / Name / Id / Dest Row
	CString strCheck, strName, strId, strDestRow;
	
	strCheck = QuickGetText(nCol, nRow);
	
	GetCell(nCol, nRow, &cell);
	strName = cell.GetOtherDataCellText();
	
	strId = QuickGetText(nCol + 2, nRow);
	
	GetCell(nCol + 2, nRow, &cell);
	strDestRow = cell.GetOtherDataCellText();
	
	saData.Add(strCheck);
	saData.Add(strName);
	saData.Add(strId);
	saData.Add(strDestRow);
}


void CGrid_Field::SetData_FieldEditDest(int nRow, CStringArray *psaData, BOOL bInvalidate)
{
	//# Name / Id / Index
	CString strName, strId, strIndex;
	strName = psaData->GetAt(0);
	strId = psaData->GetAt(1);
	strIndex = psaData->GetAt(2);
	
	COLORREF clrBk;
	GetEvenOddBkColor(nRow, clrBk);
	
	int nCol = 0;
	QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetBackColor(nCol, nRow, clrBk);
	QuickSetFont(nCol, nRow, &m_fontDefault);
	QuickSetText(nCol, nRow, strName);
	QuickSetBorderColor(nCol, nRow, &m_penGridLine);
	QuickSetBorder(nCol, nRow, UG_BDR_RTHIN);
	
	nCol = 1;
	QuickSetText(nCol, nRow, strId);
	QuickSetBorderColor(nCol, nRow, &m_penGridLine);
	QuickSetBorder(nCol, nRow, UG_BDR_RTHIN);
	
	nCol = 2;
	QuickSetText(nCol, nRow, strIndex);
	
	if (bInvalidate == TRUE)
		RedrawAll();
}

void CGrid_Field::GetData_FieldEditDest(int nRow, CStringArray &saData)
{
	saData.RemoveAll();
	
	//# Name / Id / Index
	CString strName, strId, strIndex;
	
	strName = QuickGetText(0, nRow);
	strId = QuickGetText(1, nRow);
	strIndex = QuickGetText(2, nRow);
	
	saData.Add(strName);
	saData.Add(strId);
	saData.Add(strIndex);
}


CString CGrid_Field::GetCellData(int nRow, int nCol)
{
	CString strReault = "";
	
	CUGCell cell;
	
	if (GetCell(nCol, nRow, &cell) == UG_SUCCESS)
		strReault = cell.GetText();
	
	return strReault;
}

void CGrid_Field::SetColor_EvenOdd()
{
	CUGCell cell;
	COLORREF clrBk;
	
	int nRow, nCol;
	for (nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetEvenOddBkColor(nRow, clrBk);
		for (nCol = 0; nCol < GetNumberCols(); nCol++)
		{
		/*
		GetCell(nCol, nRow, &cell);
		
		  //## Line Color ##
		  cell.SetBorderColor(&m_penGridLine);
		  cell.SetBorder(UG_BDR_RTHIN);
		  
			SetCell(nCol, nRow, &cell);
			*/
			
			QuickSetBackColor(nCol, nRow, clrBk);
		}
	}
}
