// CodeGrid.cpp: implementation of the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid_Comm.h"

#include  <math.h>

#include "../MainFrmWnd.h"
#include "print4Grid.h"

#include "../../../../SrcSite/include/axis_msg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		HEADNAME_RESULT[] = { _T("종목명"), _T("현재가"), _T("등락폭"), _T("등락율"), _T("거래량"), _T("전일비") };//, _T("유사도")
const		int			HEADWIDTH_RESULT[] = { 100, 65, 39, 40, 80, 38,38 };


//const		LPCTSTR		HEADNAME_BASE[] = { _T("종목코드"), _T("종목명") };
const		LPCTSTR		HEADNAME_BASE[] = {_T("분"), _T("신"), _T("∨종목명") };
const		int			HEADWIDTH_BASE[] = { 0, 0, 100 };


#define DEFAULT_LOCKCOL		3                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
#define ROW_DEFHEIGHT		18
#define ROW_HEAD_DEFHEIGHT	20

const   COLORREF clrHeader	 = COLOR_GRID_HEADER_FACE;
const   COLORREF clrSelRow   = COLOR_GRID_SEL_ROW;
const   COLORREF clrEvenRow  = COLOR_GRID_EVEN_ROW;
const   COLORREF clrOddRow	 = COLOR_GRID_ODD_ROW;
const   COLORREF clrGridLine = COLOR_GRID_LINE;

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
#define BLACK	RGB(0,0,0)

//# 삼성증권
#define	UM_CHANGE_PARENT			(WM_USER + 450)		// 독립실행시 프레임 윈도우 변경을 통지하기위해...
//#define COL_CODE      0
#define COL_BUN      0
#define COL_SIN      1
#define COL_NAME     2

#define	UM_GET_SCREENLINKMENU		(WM_USER + 235)		// 화면의 그리드에서 연결화면 리스트 목록 취득용
#define	UM_CHILDSCREEN				(WM_USER + 150)		// 일반 화면 스크립트롤 호출용

BEGIN_MESSAGE_MAP(CGrid_Comm,CUGCtrl)
//{{AFX_MSG_MAP(ShowGrid)
ON_MESSAGE(UM_CHANGE_PARENT, OnChangeChildFrameWindowHandle)
//}}AFX_MSG_MAP
ON_WM_DESTROY()
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
	m_nPrevRow			= -1;
	m_nLastCol			= -1;
	
	m_bCallAdjustOneTime = FALSE;
	
	m_bShowCol = TRUE;
	m_nHideCol = 1;
	
	m_nFixCol = 0;
	
	m_bShow_Order = FALSE;
	
	//--> Drag&Drop
	m_pDragBitmap           = NULL;
	m_pDragImage			= NULL;
	m_bDragAndDrop			= FALSE;
	m_MouseMode				= MOUSE_NOTHING;
	m_bFocus				= FALSE;
	m_lDragRow				= -1;
	m_nDragCol				= 0;	// 종목명위치
	m_uGridNotifyControlMsg = ::RegisterWindowMessage("FN_GRID_NOTIFY_CONTROL_MSG");
	//<-- Drag&Drop
	
	m_hChildFrame = NULL;

	m_pPrint			= new CPrint4Grid;
	m_pPrint->m_ctrl	= this;
	m_pPrint->m_GI		= m_GI;
	m_pBorderPen4Print  = NULL;
	m_nStyle4Print		= 0;
	m_pPrintTailFont	= NULL;

	SetPrintTailFont("굴림체", 600);

	m_nPatternType = 0; // 0:추세, 1:캔들

	m_bSelectCheckFlag = TRUE;
}

CGrid_Comm::~CGrid_Comm()
{
	m_fontDefault.DeleteObject();
	m_penGridLine.DeleteObject();

	if(m_pPrint != NULL)
		delete m_pPrint;

	if(m_pBorderPen4Print != NULL)
		delete m_pBorderPen4Print;

	if(m_pPrintTailFont != NULL)
		delete m_pPrintTailFont;
}

void CGrid_Comm::OnDestroy()
{
	
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
	m_bSelectCheckFlag = TRUE;

	RedrawAll();
}

BOOL CGrid_Comm::PreTranslateMessage(MSG* pMsg) 
{	
	//--> 08.07.01 Drag&Drop 이준옥
	if( pMsg->message == WM_LBUTTONDOWN )
	{
		CPoint point = pMsg->pt;
		//ScreenToClient(&point);
		FnLButtonDown(point);
	}
	else if( pMsg->message == WM_LBUTTONUP)
	{
		CPoint point = pMsg->pt;
		//ScreenToClient(&point);
		FnLButtonUp(point);
	}
	else if( pMsg->message == WM_MOUSEMOVE)
	{
		CPoint point = pMsg->pt;
		//ScreenToClient(&point);
		FnMouseMove(point);
		
		//D&D할때 스크롤 현상이 있어서 방지하기 위해서 return 1L함.
		if(m_MouseMode == MOUSE_CODE_DRAG)	return 1L;
	}
	//<-- 08.07.01 이준옥
	else if( pMsg->message == WM_RBUTTONUP)
	{
		CPoint point = pMsg->pt;
		FnRButtonUp(point);
	}
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
		
	EnableExcelBorders(FALSE);
	
	SetVScrollMode(UG_SCROLLNORMAL);
	SetHScrollMode(UG_SCROLLNORMAL);
	
	HeaderSetup(strType);
    
	SetHighlightRow(TRUE);
	SetUserSizingMode(TRUE);
	AdjustComponentSizes();
	
	CSize	szFont = GetFontSize();
	m_nFontHeight = szFont.cy;
	m_nFontWidth = szFont.cx;
}

CSize CGrid_Comm::GetFontSize()
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

void CGrid_Comm::HeaderSetup(CString strType)
{
	ClearAll();
	
	int nColCnt;
	if (strType == "RESULT")
		nColCnt = _countof(HEADNAME_BASE);
	
	SetNumberCols(nColCnt);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		
		if (strType == "RESULT")
		{
			QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_BASE[nCol]));
		}
		
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);		
	}
	
	m_strType = strType;
	
	RedrawAll();
}


void CGrid_Comm::SetHideCol(int nCol, BOOL bShow)
{
	m_bShowCol = bShow;
	m_nHideCol = nCol;
	
	AdjustComponentSizes();
}

void CGrid_Comm::SetHide_Order(BOOL bShow)
{
	m_bShow_Order = bShow;
	
	AdjustComponentSizes();
}

void CGrid_Comm::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1))
	{
		SetSelectRow(m_nLastSelRow, row);
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
		//SetCommMenu(point);
		if(m_strType != "RESULT")
			return;
		
		CString strCode = "";
		CUGCell cell;
		GetCell((int)COL_NAME, m_nLastSelRow, &cell);
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");
		
		CWnd*		pModuleMainWnd;
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
		pModuleMainWnd = pwndMainFrm->GetModuleMainWnd();
		GetCursorPos(point);
		ScreenToClient(point);
		ClientToScreen(point);
		
		CString strLPARM;
		strLPARM.Format("%s\n%d\t%d\n1301\t%s\n", "IB713700", point->x, point->y, strCode);
		// mapN \n pt.x \t pt.y \n SYM \t DATA \n
		pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<
	}
}

void CGrid_Comm::SetCommMenu_Add(CMenu* pMenu)
{
	int bIsHTS = 1;
	
	int nMenuCnt;
	UINT nMenuType;
	
	pMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)"");
	
	nMenuCnt = _countof(RBTNMENU_NAME);
	CString strScreenInfo;
	for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
	{
		if (RBTNMENU_NAME[nIndex] == "")
			nMenuType = MF_SEPARATOR;
		else 
			nMenuType = MF_STRING;
		
		if(bIsHTS)
		{
		strScreenInfo = (CString)RBTNMENU_SCREEN_NO[nIndex];
		strScreenInfo = strScreenInfo.Left(4);
		}
		else
		{
			strScreenInfo = (CString)RBTNMENU_SCREEN_NOEMP[nIndex];
			strScreenInfo = strScreenInfo.Left(5);
		}

		strScreenInfo = "[" + strScreenInfo + "] " + (CString)RBTNMENU_NAME[nIndex];
		pMenu->AppendMenu(nMenuType, RBTNMENU_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
	}
}

CString CGrid_Comm::GetCommMenu_IndexToScreenNo(int nIndex_SelMenu)
{
	CString strScreenNo = "";
	int nIndex;
	
	for (nIndex = 0; nIndex < _countof(RBTNMENU_INDEX); nIndex++)
	{
		if ((int)RBTNMENU_INDEX[nIndex] == nIndex_SelMenu)
			strScreenNo = RBTNMENU_SCREEN_NO[nIndex];
	}
	
	return strScreenNo;
}

void CGrid_Comm::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1 && col != -1) && (updn == 1))
	{
		//이전 Focus가 메모Grid일 경우 저장
		CUGCell cell;
		GetCell(m_nLastCol, m_nPrevRow, &cell);

		if(!cell.GetReadOnly() && col != 2)
		{
			CString strCode = cell.GetOtherJongCode();
			CString strName = cell.GetOtherDataCellText();
			CString strNote = cell.GetText();
			strName.TrimRight();

 			if(strNote != "")
				SaveMemo(strCode.Mid(0, 6), strNote);

			StartEdit();
		}
		else
		{
			if (updn == 1)
				SetSelectRow(row);
		}

		OnRowChange(m_nLastSelRow, row);
		m_nLastCol = col;
		m_nPrevRow = m_nLastSelRow;

		
	}
}

void CGrid_Comm::OnKeyDown(UINT *vcKey,BOOL processed)
{
	m_CUGGrid->m_keyRepeatCount = 1;

	UINT nKey = *vcKey;

	if(nKey == VK_UP)
	{
		if(GotoRow(m_GI->m_dragRow-1) == UG_SUCCESS)
			SetSelectRow(m_GI->m_currentRow);
	}
	else if(nKey == VK_DOWN)
	{
		if(GotoRow(m_GI->m_dragRow+1) == UG_SUCCESS)
			SetSelectRow(m_GI->m_currentRow);
	}
	*vcKey = 0;
}

void CGrid_Comm::SetSelectRow(int nRow)
{
	CUGCell	cell;
	
	//CWnd *pwndParent = GetParent();
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)GetParent();
	
	SetSelectRow(m_nLastSelRow, nRow);
	if (m_strType == "RESULT")
	{
		GetCell(COL_NAME, nRow, &cell);
		
		CString strData;
		strData.Format("%s/%s", cell.GetOtherJongCode(), cell.GetOtherDataCellText());
		pwndMainFrm->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_CLK, (LPARAM)&strData);
	}
}

void CGrid_Comm::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
/*
if (m_strType == "RESULT")
return;
	*/
	
	if (updn && !processed)
	{
//		{
//			CString szDebug;
//			szDebug.Format("[SMalzio]OnTH_LClicked. col(%d).", col);
//			OutputDebugString(szDebug);
//		}

		int nCodeCol = GetCodeColNumber();

		if ((row == -1) && (col == nCodeCol) && point->x<=10)
		{
			CUGCell cell;
			m_bSelectCheckFlag = !m_bSelectCheckFlag;

			for (int nRow = 0; nRow < GetNumberRows(); nRow++)
			{
				GetCell(nCodeCol, nRow, &cell);
				if (cell.GetCellType() == UGCT_CHECKBOX)
				{
					if (m_bSelectCheckFlag == TRUE)
						QuickSetText(nCodeCol, nRow, "1");
					else
						QuickSetText(nCodeCol, nRow, "0");
				}
			}
			RedrawAll();
			return;

			//CWnd* pwndParent = GetParent();       //# CMainWnd
			//pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_HEADERCLK, (LPARAM)NULL);
		}

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
	
	if (m_nSortType[m_nSortCol] == 1)
	{
		CString text1;
		cell1->GetLabelText(&text1);
		if(text1.GetLength())	//체크속성일때의소트
		{
			CString text2;
			cell2->GetLabelText(&text2);
			nResult = text1.Compare(text2);
		}

		else
		{
			nResult = cell1->m_string.Compare(cell2->m_string);
		}
	}
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
	
}


void CGrid_Comm::SetSelectRow(long oldrow, long newrow)
{
	if (GetNumberRows() - 1 < newrow)
		return;
	
	COLORREF clrBk;
	CUGCell	cell;
	
	int nColCnt = GetNumberCols();
	GetEvenOddBkColor(oldrow, clrBk);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		if(nCol == COL_BUN || nCol == COL_SIN)
			continue;
		
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

void CGrid_Comm::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	if (m_strType == "RESULT")
	{
		CString strData;
		CUGCell cell;
//		GetCell(COL_NAME, row, &cell);
//		strData = cell.GetOtherJongCode();
		strData = GetRowToCode(row);
		GetParent()->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_DBCLICK_L, (LPARAM)&strData);
	}
	
}

int CGrid_Comm::GetCodeColNumber(int nType)
{
	//if(nType==1) return ;
	return COL_NAME;
}

CString CGrid_Comm::GetRowToCode(int nRow)
{
	CString strResult = "";
	if (GetNumberRows() - 1 < nRow)
		return strResult;
	
	if (m_strType == "RESULT")
	{
		CUGCell cell;
		GetCell(COL_NAME, nRow, &cell);
		strResult = cell.GetOtherJongCode();
	}
	
	return strResult;
}

BOOL CGrid_Comm::IsRowCodeChecked(int nRow)
{
	CString strResult = "";
	if (GetNumberRows() - 1 < nRow)
		return FALSE;

	if (m_strType == "RESULT")
	{
		LPCSTR strCheck = QuickGetText(COL_NAME, nRow);
		return (BOOL)atoi(strCheck);
	}

	return FALSE;
}

//extern CString dll_GetStockDetailInfo(CString strCode, int nType);
void CGrid_Comm::SetHint_Code(int nRow)
{
// 	CString strTip;
// 	CUGCell cell;
// 	GetCell((int)COL_NAME, nRow, &cell);
// 	
// 	if (m_tipInfo.GetShowState() == FALSE)
// 	{
// 		CString strCode;
// 		strCode = cell.GetOtherJongCode();
// 		
// 		strCode.TrimRight(" ");
// 		if (strCode.IsEmpty() == TRUE)
// 			return;
// 		
// 		int nMarket;
// 		nMarket = cell.GetOtherDataInt();
// 
// 		int nID = 5;
// 		IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
// 		IMasterDataManager* pDataManager = NULL;
// 		if(pGateMng)
// 		{
// 			pDataManager = (IMasterDataManager*)pGateMng->GetInterface(nID);
// 			if(pDataManager)
// 			{
// 				strTip = pDataManager->GetStockDetailInfo(strCode, nMarket);
// 				m_tipInfo.UpdateToolTip(this, strTip, 1);
// 				m_tipInfo.Show(strTip);
// 			}
// 		}
// 	}
}

int CGrid_Comm::OnHint(int col, long row, int section, CString *string)
{
	if ((int)COL_NAME != col)
		return FALSE;
	
	if (m_strType == "RESULT")
		SetHint_Code(row);
	
	return FALSE;
//	if (m_strType == "RESULT")
//	{
//		CUGCell cell;
//		CString strData, strData1, strData2;
//		GetCell(1, row, &cell);
//		strData1 = cell.GetText();
//		strData1.TrimRight();
//		
//		GetCell(0, row, &cell);
//		strData2 = cell.GetText();
//		strData2.TrimRight();
//		
//		strData.Format("%s(%s)", strData1 , strData2);
//		
//		if (strData.GetLength() > 86)
//			strData = strData.Left(86) + "...";
//		
//		*string = strData;
//		return TRUE;
//	}
//	
//	return FALSE;
}

void CGrid_Comm::OnAdjustComponentSizes(RECT *grid, 
										RECT *topHdg,RECT *sideHdg,
										RECT *cnrBtn,RECT *vScroll,
										RECT *hScroll,RECT *tabs)
{
	if (m_bCallAdjustOneTime == TRUE)
		return;
	
		/*
		CRect  rtGrid;
		double dWidthGrid = 0.0;
		double dWidthCell = 0.0;
		
		  int ntotWidth = 0;
		  int nCol = 0, nColCnt = 0;
		  if (m_strType == "RESULT")
		  {
		  nColCnt = _countof(HEADWIDTH_RESULT);
		  
			for (nCol = 0; nCol < _countof(HEADWIDTH_RESULT); nCol++)
			{
			if ((m_nHideCol == nCol) && (m_bShowCol == FALSE))
			continue;
			
			  if ((nCol == 5) && (m_bShow_Order == FALSE))
			  continue;
			  
				if (nCol == 3)
				continue;
				
				  ntotWidth += HEADWIDTH_RESULT[nCol];
				  }
				  }
				  
					if (!m_bCallAdjustOneTime)
					{
					rtGrid = CRect(grid);
					
					  dWidthGrid = rtGrid.Width() - 1 - ntotWidth;
					  if (m_strType == "RESULT")
					  {
					  for (nCol = 0; nCol < _countof(HEADWIDTH_RESULT); nCol++)
					  {
					  if (nCol == 3)
					  SetColWidth(nCol, (int)dWidthGrid);
					  else if ((m_nHideCol == nCol) && (m_bShowCol == FALSE))
					  SetColWidth(nCol, 0);
					  else if ((nCol == 5) && (m_bShow_Order == FALSE))
					  SetColWidth(nCol, 0);
					  else
					  SetColWidth(nCol, (int)HEADWIDTH_RESULT[nCol]);
					  }
					  }
					  
						m_bCallAdjustOneTime = TRUE;
						AdjustComponentSizes();
						m_bCallAdjustOneTime = FALSE;
						}
	*/
	
	return;
}

int CGrid_Comm::OnCellTypeNotify(long ID, int col, long row, long msg, long param)
{
	RedrawAll();
	Invalidate();
	
	if (row == -1 && col == -1)
		return -1;
	
	CUGCell cell;
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd *)GetParent();
	
	CString strData;
	if (m_strType == "RESULT")
	{
		if (col == 5)
		{
			if (msg == UGCT_BUTTONCLICK)
			{
				
			}
		}
	}

	if(ID == m_nButtonIndex)	
	{
		if(msg == UGCT_BUTTONUP)
		{
			GetCell(col, row, &cell);

// 			CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
// 			pReWnd->m_saDelItemCode.Add(cell.GetOtherJongCode());
			DeleteRow(row);
		}
	}
	
	return 0;
}

const LPCSTR TYPE_MARKETCODE[] = {_T("0001"), _T("1001"), _T("2001"), _T("3001"), _T("4001")};
void CGrid_Comm::SetAddRow(int nRow, int lOutFieldCnt, DRAW_OUT_LIST stDRAW_OUT_LIST, int arCandleData[5])
{
	char *pBuff;
	
	int nSign, nMarkType;
	int ntotCol = lOutFieldCnt;
	CString strItemCode, strItemName;
	CString strTemp;
	
	double* dcolData = stDRAW_OUT_LIST.dFieldData;
	
	INT_CONVERT(pBuff, stDRAW_OUT_LIST.sSign, sizeof(stDRAW_OUT_LIST.sSign), nSign);
	INT_CONVERT(pBuff, stDRAW_OUT_LIST.sMarket, sizeof(stDRAW_OUT_LIST.sMarket), nMarkType);
	
	STR_CONVERT(pBuff, stDRAW_OUT_LIST.sHname, sizeof(stDRAW_OUT_LIST.sHname), strItemName);
	STR_CONVERT(pBuff, stDRAW_OUT_LIST.sJmcode, sizeof(stDRAW_OUT_LIST.sJmcode), strItemCode);

	CString strStatus;
	CStringArray arrData;
	COLORREF clrBunText;
	COLORREF clrBunBack; 
	
	strStatus = GetStockStatus(strItemCode, strItemName, clrBunText, clrBunBack);
	
	int nCol;
	CString strData[RESULT_FIELD_MAX];
	CUGCell cellItemName, cellItemCode, cellData[RESULT_FIELD_MAX];
	
	COLORREF clrBk;
	GetEvenOddBkColor(nRow, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;
	
	CUGCell cellItemBun, cellItemSin;
	
	int nRealDataIndex = 0;
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		TempHeadType = m_plistField->GetAt(pos);
		
		int nDataLength = TempHeadType.nDataLength;
		int nUnitNum = TempHeadType.nUnitNum;
		
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
			CString strSign = (dcolData[nRealDataIndex] > 0) ? "" : "-";
			strSign = (dcolData[nRealDataIndex] == 0) ? " " : strSign;
			strData[nRealDataIndex] = /*strSign +*/ GetFormatStr(fabs(dcolData[nRealDataIndex]), nUnitNum, FALSE, "");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "debiuel")  //## 대비율 ##
		{
			CString strSign = "";
			if(nSign == 4 || nSign == 5)
				strSign = "-";
		
			strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dcolData[nRealDataIndex]), nUnitNum, FALSE, "");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "date")  //## 날짜 ##
		{
			strData[nRealDataIndex] = GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, TRUE, "XXXX/XX/XX");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "time")  //## 시간 ##
		{
			strData[nRealDataIndex] = GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, TRUE, "XX:XX:XX");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "period")  //## 기간 ##
		{
			strData[nRealDataIndex] = GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, FALSE, "");
			nRealDataIndex++;
		}
		else		//# int, double
		{
			CString strSign = (dcolData[nRealDataIndex] > 0)?' ':'-';
			strSign = (dcolData[nRealDataIndex] == 0) ? " " : strSign;
			
			if (TempHeadType.strDataType == "int")
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, FALSE, "");
			if (TempHeadType.strDataType == "double")
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dcolData[nRealDataIndex]), nUnitNum, FALSE, "");
			
			nRealDataIndex++;
		}
	}
	

	GetCell((int)COL_BUN, nRow, &cellItemBun);
	GetCell((int)COL_SIN, nRow, &cellItemSin);	
//	GetCell(COL_CODE, nRow, &cellItemCode);
	GetCell(COL_NAME, nRow, &cellItemName);
	
	int nCol_Offset;
	nCol_Offset = DEFAULT_LOCKCOL;
	
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);

	cellItemBun.SetFont(&m_fontDefault);
	cellItemBun.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemBun.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	if(strStatus != "")
	{	
		cellItemBun.SetText(strStatus);
		cellItemBun.SetBackColor(clrBunBack);
		cellItemBun.SetTextColor(clrBunText);
	}
	cellItemBun.SetReadOnly(TRUE);

	cellItemSin.SetFont(&m_fontDefault);
	cellItemSin.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemSin.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellItemSin.SetText(strStatus);
	cellItemSin.SetBackColor(clrBunBack);	
	cellItemSin.SetTextColor(clrBunText);	
	cellItemSin.SetReadOnly(TRUE);
	
// 	cellItemCode.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cellItemCode.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
// 	cellItemCode.SetFont(&m_fontDefault);
// 	cellItemCode.SetText(strItemCode);
// 	cellItemCode.SetBackColor(clrBk);
// 	cellItemCode.SetOtherJongCode(strItemCode);
// 	cellItemCode.SetOtherDataLong(atol(strItemCode));
// 	cellItemCode.m_string = strItemCode;
	//cellItemCode.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherDataInt(nMarkType);
	
	cellItemName.SetFont(&m_fontDefault);
	//>>[20110826alzioyes]0000135: 조건검색 결과 리스트에 체크박스 및 기능 추가
	//구간검색은 종목코드를 보여주지 않기때문에 종목명 부분에 체크추가.
	cellItemName.SetCellType(UGCT_CHECKBOX);
	cellItemName.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
	//cellItemName.SetText(strItemName);
	cellItemName.SetText("1");
	cellItemName.SetLabelText(strItemName);
	cellItemName.SetBackColor(clrBk);
	//cellItemName.m_string = strItemName;
	cellItemName.SetReadOnly(FALSE);		//ReadOnly속성을 빼줘야 체크를 사용할 수 있다.
	
	cellItemName.SetOtherJongCode(strItemCode);
	
	CString strDateTime, strDate;
	STR_CONVERT(pBuff, stDRAW_OUT_LIST.sDateTime_Start, sizeof(stDRAW_OUT_LIST.sDateTime_Start), strTemp);
	strDateTime = strTemp;
	STR_CONVERT(pBuff, stDRAW_OUT_LIST.sDateTime_End, sizeof(stDRAW_OUT_LIST.sDateTime_End), strDate);
	strDateTime = strDateTime + strDate;
	cellItemName.SetOtherDataCellText(strDateTime);
	
	COLORREF clrContrastText;
	//COLORREF clrContrastBk;
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

		cellData[nCol + nCol_Offset].SetReadOnly(TRUE);
		
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
			//strData[nRealDataIndex] = FindCandleIdFromName((long)dcolData[nRealDataIndex]);
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong((long)dcolData[nRealDataIndex]);
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "pattern")  //# 패턴
		{
			//      strData[nRealDataIndex] = FindPatenIdFromName((long)dcolData[nRealDataIndex]);
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong((long)dcolData[nRealDataIndex]);
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "period")  //## CODE ##
		{
			if(strTemp.Right(6) == "000000")
			{
				strTemp.Format("%s/%s/%s", strDate.Left(4), strDate.Mid(4,2), strDate.Mid(6,2));
			}
			else
			{
				strTemp.Format("%s/%s %s:%s", strDate.Mid(4,2), strDate.Mid(6,2), strDate.Mid(8,2), strDate.Mid(10,2));
			}

			cellData[nCol + nCol_Offset].SetText(strTemp);
			cellData[nCol + nCol_Offset].m_string = strTemp;

			cellData[nCol + nCol_Offset].SetOtherDataCellText(strDate);
			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 9072 || TempHeadType.lHeadID == 9073) // 일봉, L일봉H
		{
			CString strCandleData;
			int nTemp, arTempCandleData[5];
			nTemp = arCandleData[0];
			arTempCandleData[0] = arCandleData[1];
			arTempCandleData[1] = arCandleData[2];
			arTempCandleData[2] = arCandleData[3];
			arTempCandleData[3] = arCandleData[4];
			arTempCandleData[4] = nTemp;

			for(int i = 0 ; i < 5; i++)
			{
				strTemp.Format("%d|", arTempCandleData[i]);
				strCandleData += strTemp;
			}

			if(TempHeadType.lHeadID == 9072)
			{
				cellData[nCol + nCol_Offset].SetCellTypeEx(UGCELL_CANDLE_SET);
				cellData[nCol + nCol_Offset].SetCandleData(strCandleData);
			}
			else
			{
				cellData[nCol + nCol_Offset].SetCellTypeEx(UGCELL_LHCANDLE_SET);
				cellData[nCol + nCol_Offset].SetLHCandleData(strCandleData);
			}			

			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 8003) // 삭제
		{
			m_nButtonIndex = AddCellType(&m_ugButtonType);
			cellData[nCol + nCol_Offset].SetText(_T("X"));
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
			cellData[nCol + nCol_Offset].SetCellType(m_nButtonIndex);
			cellData[nCol + nCol_Offset].SetCellTypeEx(UGCT_BUTTONNOFOCUS);
			cellData[nCol + nCol_Offset].SetBackColor(GetSysColor(COLOR_BTNFACE));
			cellData[nCol + nCol_Offset].SetOtherJongCode(strItemCode);

			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 9033) // 업종구분
		{
			CString strUpjongCode;
			strUpjongCode.Format("%3d", _ttoi(strData[nRealDataIndex]));
			strUpjongCode.Replace(' ', '0');
			
			CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();
			CString strMarketName = pMainFrmWnd->GetMarketNameToCode(strUpjongCode);
			if(strMarketName != "")
			{
				cellData[nCol + nCol_Offset].SetText((LPCTSTR)strMarketName);
			}
			else
				cellData[nCol + nCol_Offset].SetText((LPCTSTR)strUpjongCode);

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
			
			CString strNote;
			if (TempHeadType.lHeadID == 8002) // 메모
			{	
				strNote = LoadMemo(strItemCode.Mid(0, 6));

				cellData[nCol + nCol_Offset].SetText(strNote);
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
				cellData[nCol + nCol_Offset].SetReadOnly(FALSE);
				cellData[nCol + nCol_Offset].SetOtherJongCode(strItemCode);
				cellData[nCol + nCol_Offset].SetOtherDataCellText(strItemName);
			}
			else 
			{
				if(atof(strData[nRealDataIndex]) == 0)
					cellData[nCol + nCol_Offset].SetText("");
				else
					cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);

				cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			}

		//>> jeoyoho
			if (TempHeadType.strDataType == "int")
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex]));
			else
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * 100));
			
			//cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * 100));
		//<< jeoyoho
			
			if (TempHeadType.strDataType == "debi")    //## 대비 ##
			{
				GetSignInfo(nRow, nSign, clrContrastText);
				
				if(nSign > 3)
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * -100));

				cellData[nCol + nCol_Offset].SetTextColor(clrContrastText);
				cellData[nCol + nCol_Offset].SetSign(nSign);
			}
			else if (TempHeadType.strDataType == "debiuel")   //## 대비율 ##
			{
				GetSignInfo(nRow, nSign, clrContrastText);

				if(nSign > 3)
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * -100));

				cellData[nCol + nCol_Offset].SetTextColor(clrContrastText);
			}
			nRealDataIndex++;
		}
		
		cellData[nCol + nCol_Offset].SetBackColor(clrBk);
		cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
  }
  
//   cellItemCode.SetBackColor(clrBk);
//   cellItemCode.SetBorderColor(&m_penGridLine);
//   cellItemCode.SetBorder(UG_BDR_RTHIN);
//   SetCell(COL_CODE, nRow, &cellItemCode);
  
  SetCell((int)COL_BUN, nRow, &cellItemBun);

  SetCell((int)COL_SIN, nRow, &cellItemSin);
  
  CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();
  if(pMainFrmWnd->m_nFixCol == 1)
	  cellItemName.SetBorder(1);
  else
	  cellItemName.SetBorder(UG_BDR_RTHIN);
  
  cellItemName.SetBackColor(clrBk);
  cellItemName.SetBorderColor(&m_penGridLine);
  /*cellItemName.SetBorder(UG_BDR_RTHIN);*/
  SetCell(COL_NAME, nRow, &cellItemName);
  
  for (nCol = 0; nCol < ntotCol; nCol++)
  {
	  cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
	  cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
	  SetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);
  }
}

void CGrid_Comm::SetAddRow(int nRow, int lOutFieldCnt, CANDLE_OUT_LIST stCANDLE_OUT_LIST, int arCandleData[5])
{
	char *pBuff;
	
	int nSign, nMarkType;
	int ntotCol = lOutFieldCnt;
	CString strItemCode, strItemName;
	CString strTemp;
	
	double* dcolData = stCANDLE_OUT_LIST.dFieldData;
	
	INT_CONVERT(pBuff, stCANDLE_OUT_LIST.sSign, sizeof(stCANDLE_OUT_LIST.sSign), nSign);
	INT_CONVERT(pBuff, stCANDLE_OUT_LIST.sMarket, sizeof(stCANDLE_OUT_LIST.sMarket), nMarkType);
	
	STR_CONVERT(pBuff, stCANDLE_OUT_LIST.sHname, sizeof(stCANDLE_OUT_LIST.sHname), strItemName);
	STR_CONVERT(pBuff, stCANDLE_OUT_LIST.sJmcode, sizeof(stCANDLE_OUT_LIST.sJmcode), strItemCode);
	
	CString strCode = strItemCode.Mid(0, 6);

	CString strStatus;
	CStringArray arrData;
	COLORREF clrBunText;
	COLORREF clrBunBack; 

	strStatus = GetStockStatus(strItemCode, strItemName, clrBunText, clrBunBack);

	int nCol;
	CString strData[RESULT_FIELD_MAX];
	CUGCell cellItemName, cellItemCode, cellData[RESULT_FIELD_MAX];
	CUGCell cellItemBun, cellItemSin;
	
	COLORREF clrBk;
	GetEvenOddBkColor(nRow, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;
	
	int nRealDataIndex = 0;
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		TempHeadType = m_plistField->GetAt(pos);
		
		int nDataLength = TempHeadType.nDataLength;
		int nUnitNum = TempHeadType.nUnitNum;
		
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
			CString strSign = (dcolData[nRealDataIndex] > 0) ? "+" : "-";
			strSign = (dcolData[nRealDataIndex] == 0) ? " " : strSign;
			strData[nRealDataIndex] = /*strSign +*/ GetFormatStr(fabs(dcolData[nRealDataIndex]), nUnitNum, FALSE, "");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "debiuel")  //## 대비율 ##
		{
			CString strSign = "";
			if(nSign == 4 || nSign == 5)
				strSign = "-";
		
			strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dcolData[nRealDataIndex]), nUnitNum, FALSE, "");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "date")  //## 날짜 ##
		{
			strData[nRealDataIndex] = GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, TRUE, "XXXX/XX/XX");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "time")  //## 시간 ##
		{
			strData[nRealDataIndex] = GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, TRUE, "XX:XX:XX");
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "period")  //## 기간 ##
		{
			strData[nRealDataIndex] = GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, FALSE, "");
			nRealDataIndex++;
		}
		else		//# int, double
		{
			CString strSign = (dcolData[nRealDataIndex] > 0)?' ':'-';
			strSign = (dcolData[nRealDataIndex] == 0) ? " " : strSign;
			
			if (TempHeadType.strDataType == "int")
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dcolData[nRealDataIndex]), 0, FALSE, "");
			if (TempHeadType.strDataType == "double")
				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dcolData[nRealDataIndex]), nUnitNum, FALSE, "");
			
			nRealDataIndex++;
		}
		
	}
	
	GetCell((int)COL_BUN, nRow, &cellItemBun);
	GetCell((int)COL_SIN, nRow, &cellItemSin);
//	GetCell(COL_CODE, nRow, &cellItemCode);
	GetCell(COL_NAME, nRow, &cellItemName);
	
	int nCol_Offset;
	nCol_Offset = DEFAULT_LOCKCOL;
	
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);
	
	cellItemBun.SetFont(&m_fontDefault);
	cellItemBun.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemBun.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	if(strStatus != "")
	{	
		cellItemBun.SetText(strStatus);
		cellItemBun.SetBackColor(clrBunBack);
		cellItemBun.SetTextColor(clrBunText);
	}
	cellItemBun.SetReadOnly(TRUE);

	cellItemSin.SetFont(&m_fontDefault);
	cellItemSin.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemSin.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellItemSin.SetText(strStatus);
	cellItemSin.SetBackColor(clrBunBack);	
	cellItemSin.SetTextColor(clrBunText);
	cellItemSin.SetReadOnly(TRUE);

// 	cellItemCode.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
// 	cellItemCode.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
// 	cellItemCode.SetFont(&m_fontDefault);
// 	cellItemCode.SetText(strItemCode);
// 	cellItemCode.SetBackColor(clrBk);
// 	cellItemCode.SetOtherJongCode(strItemCode);
// 	cellItemCode.SetOtherDataLong(atol(strItemCode));
// 	cellItemCode.m_string = strItemCode;
	//cellItemCode.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherDataInt(nMarkType);
	
	cellItemName.SetFont(&m_fontDefault);
	//>>[20110826alzioyes]0000135: 조건검색 결과 리스트에 체크박스 및 기능 추가
	//구간검색은 종목코드를 보여주지 않기때문에 종목명 부분에 체크추가.
	cellItemName.SetCellType(UGCT_CHECKBOX);
	cellItemName.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
	//cellItemName.SetText(strItemName);
	cellItemName.SetText("1");
	cellItemName.SetLabelText(strItemName);
	cellItemName.SetBackColor(clrBk);
	//cellItemName.m_string = strItemName;
	cellItemName.SetReadOnly(FALSE);		//ReadOnly속성을 빼줘야 체크를 사용할 수 있다.
	//<<0000135
	
	cellItemName.SetOtherJongCode(strCode);

	CString strDateTime, strDate;
	STR_CONVERT(pBuff, stCANDLE_OUT_LIST.sDateTime_Start, sizeof(stCANDLE_OUT_LIST.sDateTime_Start), strTemp);
	strDateTime = strTemp;
	STR_CONVERT(pBuff, stCANDLE_OUT_LIST.sDateTime_End, sizeof(stCANDLE_OUT_LIST.sDateTime_End), strDate);
	strDateTime = strDateTime + strDate;
	cellItemName.SetOtherDataCellText(strDateTime);
	
	COLORREF clrContrastText;
	//COLORREF clrContrastBk;
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

		cellData[nCol + nCol_Offset].SetReadOnly(TRUE);
		
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
			//strData[nRealDataIndex] = FindCandleIdFromName((long)dcolData[nRealDataIndex]);
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong((long)dcolData[nRealDataIndex]);
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "pattern")  //# 패턴
		{
			//      strData[nRealDataIndex] = FindPatenIdFromName((long)dcolData[nRealDataIndex]);
			
			if (!strData[nRealDataIndex].IsEmpty())
			{
				strData[nRealDataIndex].TrimRight();
				strData[nRealDataIndex].TrimLeft();
			}
			
			cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = strData[nRealDataIndex];
			
			cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong((long)dcolData[nRealDataIndex]);
			nRealDataIndex++;
		}
		else if (TempHeadType.strDataType == "period")  //## CODE ##
		{
			if(strTemp.Right(6) == "000000")
			{
				strTemp.Format("%s/%s/%s", m_strDate.Left(4), m_strDate.Mid(4,2), m_strDate.Mid(6,2));
			}
			else
			{
				strTemp.Format("%s/%s %s:%s", m_strDate.Mid(4,2), m_strDate.Mid(6,2), m_strDate.Mid(8,2), m_strDate.Mid(10,2));
			}

			cellData[nCol + nCol_Offset].SetText(strTemp);
			cellData[nCol + nCol_Offset].m_string = strTemp;

			cellData[nCol + nCol_Offset].SetOtherDataCellText(m_strDate);
			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 9072 || TempHeadType.lHeadID == 9073) // 일봉, L일봉H
		{
			CString strCandleData;
			int nTemp, arTempCandleData[5];
			nTemp = arCandleData[0];
			arTempCandleData[0] = arCandleData[1];
			arTempCandleData[1] = arCandleData[2];
			arTempCandleData[2] = arCandleData[3];
			arTempCandleData[3] = arCandleData[4];
			arTempCandleData[4] = nTemp;

			for(int i = 0 ; i < 5; i++)
			{
				strTemp.Format("%d|", arTempCandleData[i]);
				strCandleData += strTemp;
			}

			if(TempHeadType.lHeadID == 9072)
			{
				cellData[nCol + nCol_Offset].SetCellTypeEx(UGCELL_CANDLE_SET);
				cellData[nCol + nCol_Offset].SetCandleData(strCandleData);
			}
			else
			{
				cellData[nCol + nCol_Offset].SetCellTypeEx(UGCELL_LHCANDLE_SET);
				cellData[nCol + nCol_Offset].SetLHCandleData(strCandleData);
			}			

			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 8003) // 삭제
		{
			m_nButtonIndex = AddCellType(&m_ugButtonType);
			cellData[nCol + nCol_Offset].SetText(_T("X"));
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
			cellData[nCol + nCol_Offset].SetCellType(m_nButtonIndex);
			cellData[nCol + nCol_Offset].SetCellTypeEx(UGCT_BUTTONNOFOCUS);
			cellData[nCol + nCol_Offset].SetBackColor(GetSysColor(COLOR_BTNFACE));
			cellData[nCol + nCol_Offset].SetOtherJongCode(strItemCode);

			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 9033) // 업종구분
		{
			CString strUpjongCode;
			strUpjongCode.Format("%3d", _ttoi(strData[nRealDataIndex]));
			strUpjongCode.Replace(' ', '0');

			CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();
			CString strMarketName = pMainFrmWnd->GetMarketNameToCode(strUpjongCode);
			if(strMarketName != "")
			{
				cellData[nCol + nCol_Offset].SetText((LPCTSTR)strMarketName);
			}
			else
				cellData[nCol + nCol_Offset].SetText((LPCTSTR)strUpjongCode);
			
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
			
			CString strNote;
			if (TempHeadType.lHeadID == 8002) // 메모
			{
				strNote = LoadMemo(strItemCode.Mid(0, 6));
				
				cellData[nCol + nCol_Offset].SetText(strNote);
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
				cellData[nCol + nCol_Offset].SetReadOnly(FALSE);
				cellData[nCol + nCol_Offset].SetOtherJongCode(strItemCode);
				cellData[nCol + nCol_Offset].SetOtherDataCellText(strItemName);
			}
			else 
			{
				if(atof(strData[nRealDataIndex]) == 0)
					cellData[nCol + nCol_Offset].SetText("");
				else
					cellData[nCol + nCol_Offset].SetText(strData[nRealDataIndex]);

				cellData[nCol + nCol_Offset].SetOtherDataCellText(strData[nRealDataIndex]);
			}
		//>> jeoyoho
			if (TempHeadType.strDataType == "int")
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex]));
			if (TempHeadType.strDataType == "double")
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * 100));
			
			//cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * 100));
		//<< jeoyoho
			
			if (TempHeadType.strDataType == "debi")    //## 대비 ##
			{
				GetSignInfo(nRow, nSign, clrContrastText);
				
				cellData[nCol + nCol_Offset].SetTextColor(clrContrastText);
				cellData[nCol + nCol_Offset].SetSign(nSign);
			}
			else if (TempHeadType.strDataType == "debiuel")   //## 대비율 ##
			{
				GetSignInfo(nRow, nSign, clrContrastText);

				if(nSign > 3)
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(dcolData[nRealDataIndex] * -100));

				cellData[nCol + nCol_Offset].SetTextColor(clrContrastText);
			}
			nRealDataIndex++;
		}
		
		cellData[nCol + nCol_Offset].SetBackColor(clrBk);
		cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
  }
  
//   cellItemCode.SetBackColor(clrBk);
//   cellItemCode.SetBorderColor(&m_penGridLine);
//   cellItemCode.SetBorder(UG_BDR_RTHIN);
//   SetCell(COL_CODE, nRow, &cellItemCode);
  SetCell((int)COL_BUN, nRow, &cellItemBun);

  SetCell((int)COL_SIN, nRow, &cellItemSin);
  
  CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)GetParent();
  if(pMainFrmWnd->m_nFixCol == 1)
	  cellItemName.SetBorder(1);
  else
	  cellItemName.SetBorder(UG_BDR_RTHIN);
  
  cellItemName.SetBackColor(clrBk);
  cellItemName.SetBorderColor(&m_penGridLine);
  /*cellItemName.SetBorder(UG_BDR_RTHIN);*/
  SetCell(COL_NAME, nRow, &cellItemName);
  
  for (nCol = 0; nCol < ntotCol; nCol++)
  {
	  cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
	  cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
	  SetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);
  }
}

void CGrid_Comm::SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField)
{
	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
	CMapStringToString UserColWidthsMap;
	CFindData_GridUtil::GetGridAllWidth(this, &UserColWidthsMap);

	CUGCell	cell;
	
	ClearAll();
	//ZeroMemory(&m_nSortType, sizeof(int) * RESULT_FIELD_MAX);
	
	int nColCnt = plistField->GetCount();
	SetNumberCols(_countof(HEADNAME_BASE) + plistField->GetCount());
	
	int nCol = _countof(HEADNAME_BASE);;

	m_nSortType[COL_NAME] = 1;

	POSITION   pos;
	HeadInfoType    stHeadType;

	BOOL bExistBun = FALSE;
	int nColBun = 0;
	BOOL bExistSin = FALSE;
	CString strHeadName;

	for (int nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistField->FindIndex(nIndex);
		stHeadType = m_plistField->GetAt(pos);
		
		TRACE("\n # Count      : %d", nCol);
		TRACE("\n   # HeadName : %s", stHeadType.strHeadName);
		TRACE("\n   # HeadID   : %d", stHeadType.lHeadID);

		if(stHeadType.strHeadName == "분")
		{
			bExistBun = TRUE;
			nColBun = nCol;
			stHeadType.strHeadName = "분HIDE";
			strHeadName = "분HIDE";
		}
		else if(stHeadType.strHeadName == "신")
		{
			
		}
		
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		QuickSetText(nCol, -1, stHeadType.strHeadName);
		SetColWidth(nCol, (int)(stHeadType.nColWidth * (m_nFontWidth + 1)));
		SetRowHeight(nCol, m_nFontHeight + 4);
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
		
		if ((stHeadType.strDataType == "code") ||  //## 종목 CODE ##
			(stHeadType.strDataType == "candle")	|| //# 캔들
			(stHeadType.strDataType == "pattern")	||	//# 패턴
			(stHeadType.strDataType == "period"))	//# 기간
			m_nSortType[nCol] = 1;
		nCol++;
	}

	if(bExistBun)
	{
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, HEADNAME_BASE[0], COL_BUN, 20);
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, strHeadName, nColBun, 0);
	}
	else
	{
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, HEADNAME_BASE[0], COL_BUN, 0);
	}

	//0000207: 조건검색 실시간, 사용자의 Column 구성과 배치(크기)등의 최종상태 저장 및 복원 기능 추가
	CFindData_GridUtil::SetGridAllWidth(this, &UserColWidthsMap);

	RedrawAll();
}

CString CGrid_Comm::GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat)
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

void CGrid_Comm::GetSignInfo(int nRow, int nSign, COLORREF &clrSignText)
{
	switch (nSign)
	{
	case 1:		// 상한(화살표) : RED 배경색에 WHITE TEXT
		clrSignText = RED;
		break;
	case 2:		// 상승(삼각형) : WHITE 배경색에 RED TEXT
		clrSignText = RED;
		break;
	case 4:		// 하한(화살표) : BLUE 배경색에 WHITE TEXT
		clrSignText = BLUE;
		break;
	case 5:		// 하락(삼각형) : WHITE 배경색에 BLUE TEXT
		clrSignText = BLUE;
		break;
		
		/*
		case 6:		// 기세상한
		clrSignText = RED;
		break;
		case 7:		// 기세상승
		clrSignText = RED;
		break;
		case 8:		// 기세하한
		clrSignText = BLUE;
		break;
		case 9:		// 기세하락
		clrSignText = BLUE;
		break;
		case 0:		// 기세보합
		*/
		
	case 3:		// 보합 : WHITE 배경색에 BLACK TEXT
		
	default:
		clrSignText = BLACK;
	}
}

void CGrid_Comm::SetLockColumns(int nCol, BOOL bOffSet)
{
	if (bOffSet == TRUE)
		m_nFixCol = DEFAULT_LOCKCOL + nCol - 1;
	else
		m_nFixCol = nCol;
	
	LockColumns(m_nFixCol);
}

//--> Drag&Drop : 소리마치용.
BOOL CGrid_Comm::GetRowToCode(int nRow, CString &strCode, int &nMarketType)
{
	BOOL bResult = FALSE;
	
	if (nRow < GetNumberRows())
	{
		CUGCell cell;
		GetCell(COL_NAME, nRow, &cell);
		//strCode = cell.m_string;
		strCode = cell.GetOtherJongCode();

		bResult = TRUE;
	}
	else 
		bResult = FALSE;
	
	return bResult;
}

void CGrid_Comm::FnLButtonDown(CPoint point)
{
	m_MouseMode = MOUSE_CODE_DRAG;

	if(::GetFocus()!=GetSafeHwnd())	SetFocus();

	ScreenToClient(&point);
	int col;
	GetCellFromPoint(point.x, point.y, &col, &m_lDragRow);
	int nRowHeight = GetTHRowHeight(-1);
	if(m_lDragRow == -1 || point.y < nRowHeight || (col!=m_nDragCol && col!=m_nDragCol+1) )
	{
		m_MouseMode = MOUSE_NOTHING;
		m_bDragAndDrop = FALSE;
		m_strDragCode = "";
		return;
	}
	m_lDragRow --;	// 실제보다 하나작게
}

//#include "../Builder/LinkInfoMng/KSLinkInfoMng.h"
//#include "../../../../Common/KHResource.h"
// UM_DND_NOTIFY
// DRAG_DATA_INFO은 KSLinkInfoMng.h에 정의되어 있는 Struct임.
//typedef struct tagDNDData
//{
//	POINT			pt;				// 드랍되는 지점
//	char			szCode[33];		// 드래그 데이타
//	char			szAcctNo[9];	// 계좌번호
//	CStringArray	arDataList;		// 복수데이터 
//} DRAG_DATA_INFO;
//#define	UM_DND_NOTIFY				(WM_USER + 209)		// 드래그 & 드랍 지원숑

//#include "../../../inc/IKSLibMng.h"
void CGrid_Comm::FnLButtonUp(CPoint point)
{
	ScreenToClient(&point);

	if ( m_MouseMode == MOUSE_CODE_DRAG )
	{
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		DragImageInfoClear ( );

		long lRet(0);

		CRect rcFrameRect;
		//::GetClientRect( m_hFrameWnd, &rcFrameRect );
		::GetClientRect( m_hWnd, &rcFrameRect );

		HWND hTragetWnd;
		CWnd* pTragetWnd = NULL;

		// 다른 윈도우로 drop되는 경우에만 처리해준다;
		if ( !m_strDragCode.IsEmpty() )
		{
			CPoint pt;
			GetCursorPos(&pt);

			CPoint ptPoint( pt );
			hTragetWnd = ::WindowFromPoint( pt );

			pTragetWnd = CWnd::FromHandle(hTragetWnd);
			TRACE(" WindowFromPoint...hwnd=[0x%08x]\n", hTragetWnd );
			long lData(0);
			if ( hTragetWnd && GetSafeHwnd()  != hTragetWnd )
			{
				lData = ::GetWindowLong ( hTragetWnd, GWL_USERDATA );
				if ( lData > 0 &&  lData == 1013 )
				{
					lRet = ::SendMessage( hTragetWnd, m_uGridNotifyControlMsg , (WPARAM)MAKELONG( JMCOMBO_TO_GRID_CODE_DROP_NOTIFY, 0 ),  (LPARAM)(LPCTSTR)m_strDragCode );
					if ( lRet > 0 )
					{
						m_MouseMode = MOUSE_NOTHING;
						m_bDragAndDrop = FALSE;
						m_strDragCode = "";

						return;
					}
				}
			}

			if ( lRet == 0 && !rcFrameRect.PtInRect( point) )
			{
				CPoint DropPoint(point);
				ClientToScreen(&DropPoint);
				GetParent()->GetParent()->GetParent()->SendMessage(UMSG_DROP_CODE, (WPARAM)&DropPoint, (LPARAM)(LPCSTR)m_strDragCode);
				m_MouseMode = MOUSE_NOTHING;
				m_bDragAndDrop = FALSE;
				m_strDragCode = "";

				if ( m_bFocus )
				{
					m_bFocus	= FALSE;	
				}
				
				return;
			}
		}
	}

	m_MouseMode = MOUSE_NOTHING;
	m_bDragAndDrop = FALSE;
	m_strDragCode = "";
	m_lDragRow = -1;

	if ( m_bFocus )
	{
		m_bFocus	= FALSE;	
	}
}

void CGrid_Comm::FnMouseMove(CPoint point)
{
	ScreenToClient(&point);
	
	if ( m_MouseMode == MOUSE_CODE_DRAG && m_bDragAndDrop )
	{
		POINT pt = point;
		ClientToScreen( &pt );
		
		CImageList::DragMove(pt);
		CWnd* pDropWnd = WindowFromPoint(pt);
		CImageList::DragShowNolock(TRUE);
		return;
	}
	
	CRect rcRect;
	GetClientRect( rcRect );
	if(m_MouseMode == MOUSE_CODE_DRAG)// && !rcRect.PtInRect( point) )
	{
		// 종목코드 드래그를 시작한다.
		// 종목코드를 취득한다.
		if(m_lDragRow<0) m_lDragRow = GetCurrentRow();
		int nMarketType;
		if(m_strDragCode.IsEmpty())
		{
			if (GetRowToCode(m_lDragRow, m_strDragCode, nMarketType) == FALSE)
				return;
		}
		
		//display code
		if ( MakeDragImage( m_strDragCode) )
		{
			CPoint	pt;	
			IMAGEINFO ii;
			m_pDragImage->GetImageInfo( 0, &ii );
			
			pt.x = (ii.rcImage.right - ii.rcImage.left) / 2;
			pt.y = (ii.rcImage.bottom - ii.rcImage.top) / 2;
			
			SetCursor( LoadCursor(NULL, IDC_ARROW) );
			m_pDragImage->BeginDrag( 0, pt );
			
			CPoint ptTmp = point;
			ClientToScreen( &ptTmp );
			
			m_pDragImage->DragEnter( NULL,ptTmp );
			m_bDragAndDrop = TRUE;
		}
	}
}

void CGrid_Comm::DragImageInfoClear()
{
	if ( m_pDragImage )
		delete m_pDragImage;
	m_pDragImage = NULL;
	
	if ( m_pDragBitmap )
		delete m_pDragBitmap;
	m_pDragBitmap = NULL;
	
}

BOOL CGrid_Comm::MakeDragImage(CString strText)
{
	//CFont *pFont = (CFont*) GetFont(0);
	CFont *pFont = &m_fontDefault;
	if(!pFont) return FALSE;
	ASSERT(pFont);
	
	CRect	rectImage, 
		rect;
	CDC	*pDC = GetDC();
    if (!pDC)
        return FALSE;
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	
	DragImageInfoClear();
	
	CBitmap *pOldBitmap;
	m_pDragBitmap = new CBitmap();
	m_pDragImage  = new CImageList();
	
	//CFont *pOldFont = MemDC.SelectObject(&font);
	CFont *pOldFont = MemDC.SelectObject(pFont);
	
	rectImage.left	= 0;
	rectImage.top	= 0;
	MemDC.DrawText( strText, rectImage, DT_CALCRECT);
	
	// 256색 설정에서도 drag image를 보여주기 위해서.
	// PLANES	: Number of color planes.
	// BITSPIXEL: Number of adjacent color bits for each pixel.
	int nPlanes = pDC->GetDeviceCaps(PLANES);
	int nColors = pDC->GetDeviceCaps(BITSPIXEL);
	m_pDragBitmap->CreateBitmap( rectImage.Width()+4, 
		rectImage.Height()+4, 
		nPlanes, 
		nColors, 
		NULL);
	
	pOldBitmap = MemDC.SelectObject(m_pDragBitmap);	
	
	CBrush	brush;
	brush.CreateSolidBrush( RGB(0,0,255) );
	
	MemDC.SetBkColor( RGB(0,0,255) );
	MemDC.SetTextColor( RGB(255,255,255) );
	
	rect.SetRect(0, 0, rectImage.Width()+4, rectImage.Height()+4);
	
	MemDC.FillRect(&rect, &brush);
	
	rect.SetRect(2,2,rectImage.Width()+2,rectImage.Height()+2);
	
	MemDC.DrawText( strText, rect,  DT_LEFT | DT_SINGLELINE );
	MemDC.SelectObject(pOldBitmap);
	if(pOldFont)
		MemDC.SelectObject(pOldFont);
	
	brush.DeleteObject();	////[EDIT] CBrush 해제 2007-01-19 김종연 
	MemDC.DeleteDC();		////[EDIT] MemDC 해제 2007-01-11 김종연 
	ReleaseDC( pDC );
	
	m_pDragImage->Create(rectImage.Width()+4, rectImage.Height()+4, ILC_COLORDDB | ILC_MASK, 4, 4 );
	m_pDragImage->Add( m_pDragBitmap, RGB(192,192,192) );
	
	return TRUE;
}

//<-- Drag&Drop

void CGrid_Comm::FnRButtonUp(CPoint point)
{
	SetCommMenu(&point);
}

HWND CGrid_Comm::GetFrameWnd(CWnd* pWnd)
{
	if(m_hChildFrame==NULL)
	{
		CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
		while (pParentWnd != NULL)
		{
			DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
			if ( dwExStyle & 0x8000)
			{
				m_hChildFrame = pParentWnd->GetSafeHwnd();
				return m_hChildFrame;
			}
			pParentWnd = pParentWnd->GetParent();
		}
	}
	return m_hChildFrame;
}

long CGrid_Comm::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
{
	m_hChildFrame = (HWND)lParam;
	return 0L;
}

void CGrid_Comm::SetCommMenu(POINT *point)
{
	HWND m_hChildFrame = GetFrameWnd(this);
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
			
			CString strCode = "";
			CUGCell cell;
			GetCell((int)COL_NAME, m_nLastSelRow, &cell);
			strCode = cell.GetOtherJongCode();
			strCode.TrimRight(" ");
			
			if (strScreenNo.IsEmpty() == TRUE)
			{
				if (strCode.IsEmpty() == FALSE)
					GetParent()->GetParent()->GetParent()->SendMessage( WM_USER + 107, (WPARAM)(LPCSTR)strCode, (LPARAM)0);
				::SendMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
			}
			else
			{
				if (strCode.IsEmpty() == FALSE)
				{
					GetParent()->GetParent()->GetParent()->SendMessage( WM_USER + 108, (WPARAM)(LPCSTR)strCode, (LPARAM)0);
//						KSLink_SetLinkTagData("JCODE", strCode);		// 외부화면용
				}
				::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo);
			}
		}
		pMenu->DestroyMenu();
		delete pMenu;
	}
//	HWND hChildFrame = GetFrameWnd(this);
//	if(!hChildFrame) return;
//	
//	long lMenu = ::SendMessage(hChildFrame, UM_GET_SCREENLINKMENU, 0, 0 );
//	if ( lMenu > 0 )
//	{
//		CMenu* pMenu = (CMenu*)lMenu;
//		ASSERT ( pMenu );
//		
//		//		SetCommMenu_Add(m_bIndustry, pMenu);
//		
//		GetCursorPos(point);
//		int nIndex_SelMenu = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
//			point->x, point->y, this);
//		
//		if (nIndex_SelMenu > -1)
//		{
//			HWND hWnd = hChildFrame;
//			
//			CString strScreenNo = "";
//			strScreenNo = GetCommMenu_IndexToScreenNo(nIndex_SelMenu);
//			
//			int nCol = -1;
//			if (m_strType == "RESULT")
//				nCol = 0;
//			
//			
//			CString strCode = "";
//			if (nCol > -1)
//			{
//				CUGCell cell;
//				GetCell(nCol, m_nLastSelRow, &cell);
//				strCode = cell.GetOtherJongCode();
//				strCode.TrimRight(" ");
//			}
//			
//			//KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
//			// 종목코드가 여러개일 경우에는 !를 구분으로 해서 종목리스트를 만든다.
//			// 마지막 종목다음에는 !를 넣지않아야 한다.
//			// 관심설정창 MenuID : UM_PORTFOLIO_REG = 4316
//			if (strScreenNo.IsEmpty() == TRUE)
//			{
//				if(nIndex_SelMenu==4316)
//				{
//					int nItemCount = GetNumberRows();
//					int nMarketType = 0;
//					CString strData = "";
//					
//					for(int nRow = 0; nRow < nItemCount; nRow++)
//					{
//						GetRowToCode(nRow, strCode, nMarketType);
//						strData += strCode;
//						
//						if(nRow != nItemCount - 1)
//							strData += "!";
//					}
//					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strData);
//				}
//				else
//				{
//					if (strCode.IsEmpty() == FALSE)
//						KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
//				}
//				::SendMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
//			}
//			else
//			{
//			/*		if (strCode.IsEmpty() == FALSE)
//			{
//			if (m_bIndustry == TRUE)
//			KSLink_SetLinkTagData("UPCODE", strCode);		// 외부화면용
//			else
//			KSLink_SetLinkTagData("JCODE", strCode);		// 외부화면용
//			}		
//			::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo);
//				*/
//			}
//		}
//		pMenu->DestroyMenu();
//		delete pMenu;
//	}
}
int CGrid_Comm::PrintingInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow, int endCol,long endRow)
{
	return m_pPrint->PrintInit(pDC,pPD,startCol,startRow,endCol,endRow);
}


int CGrid_Comm::PrintingPage(CDC * pDC, int pageNum)
{
	return m_pPrint->PrintPage(pDC,pageNum);
}

void CGrid_Comm::PrintingTail(CDC * pDC, CPrintInfo * pInfo)
{
	CRect rect = pInfo->m_rectDraw;
	//여백적용
	if(m_pPrint)	
	{
		rect.left  = m_pPrint->m_printLeftMarginPX;	
		rect.right = m_pPrint->m_printRightMarginPX;
	}

	CFont * pOldFont = NULL;
	if(m_pPrintTailFont)
		pOldFont = pDC->SelectObject(m_pPrintTailFont);
	else
		pOldFont = pDC->SelectObject(m_GI->m_defFont);

	CSize sz;
	TEXTMETRIC	tm;	
	pDC->GetTextMetrics(&tm);
	sz.cx =	tm.tmAveCharWidth;
	sz.cy =	tm.tmHeight;

	rect.top = rect.bottom - (sz.cy + 5);

	//시간
	CTime time(NULL);
	time = CTime::GetCurrentTime();
	CString strYear, strTime;
	strYear.Format("%d", time.GetYear());
	strYear = strYear.Right(2);
	strTime.Format("%02d/%02d/%s %02d:%02d:%02d", 
		time.GetMonth(), time.GetDay(), strYear, 
		time.GetHour(),  time.GetMinute(), time.GetSecond());

	//페이지
	CString strPage;
	strPage.Format("%d/%d", pInfo->m_nCurPage, pInfo->GetMaxPage());
	
	int nMode = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(strTime, rect, DT_LEFT | DT_TOP);
	pDC->DrawText(strPage, rect, DT_RIGHT| DT_TOP);

	pDC->SetBkMode(nMode);
	pDC->SelectObject(pOldFont);	
}

int CGrid_Comm::PrintingSetMargin(int whichMargin,int size)
{
	return m_pPrint->PrintSetMargin(whichMargin,size);
}


int CGrid_Comm::PrintingSetScale(double scale)
{	
	return m_pPrint->PrintSetScale(scale);
}


int CGrid_Comm::PrintingSetOption(int option,long param)
{	
	return m_pPrint->PrintSetOption(option,param);
}


int CGrid_Comm::PrintingGetOption(int option,long *param)
{	
	return m_pPrint->PrintGetOption(option,param);
}

int CGrid_Comm::GetTHRowHeight(int row)
{
	row = (row * -1) -1;
	if(row <0 || row > m_GI->m_numberTopHdgRows)
		return 0;

	return m_GI->m_topHdgHeights[row];
}

void CGrid_Comm::SetCellBorderColor4Print(COLORREF clrBorder)
{
	if(m_pBorderPen4Print)
		delete m_pBorderPen4Print;

	m_pBorderPen4Print = new CPen(PS_SOLID, 1, clrBorder);	
}

void CGrid_Comm::SetBorderStyle4Print(short nStyle)
{
	m_nStyle4Print = nStyle;
}

short CGrid_Comm::GetBorderStyle4Print()
{
	return m_nStyle4Print;
}

void CGrid_Comm::SetPrintTailFont(CString strFontName, int nFontSize)
{
	if(m_pPrintTailFont)
		delete m_pPrintTailFont;

	m_pPrintTailFont = new CFont;
	m_pPrintTailFont->CreatePointFont( nFontSize, strFontName );
}

CFont * CGrid_Comm::GetPrintTailFont()
{
	return m_pPrintTailFont;
}
//

void CGrid_Comm::GetRowCode(int nRow, CString &strCode, int &nMarkType)
{
	CUGCell cell;
	//GetCell(1, nRow, &cell);
	GetCell(COL_NAME, nRow, &cell);
	strCode = cell.GetOtherJongCode();
	nMarkType = cell.GetOtherDataInt();
	
	strCode.TrimLeft(" ");
	strCode.TrimRight(" ");
}

CString CGrid_Comm::GetCellData(int nRow, int nCol)
{
	CString strResult = "";
	
	CUGCell cell;
	GetCell(nCol, nRow, &cell);
	strResult = cell.GetLabelText();
	if(strResult.GetLength()>0) return strResult;

	strResult = cell.GetText();
	
	return strResult;
}

void CGrid_Comm::GetCellColor(int nRow, int nCol, COLORREF &clrText, COLORREF &clrBack)
{
	CUGCell cell;
	GetCell(nCol, nRow, &cell);
	
	clrText = cell.GetTextColor();
	clrBack = cell.GetBackColor();
}

void CGrid_Comm::GetCellSignText(int nRow, int nCol, CString &strSignText)
{
	CUGCell cell;
	GetCell(nCol, nRow, &cell);
	
	strSignText = GetSignText(cell.GetSign());
}

CString CGrid_Comm::GetSignText(int nSign)
{
	CString strSignText;
	
	switch (nSign)
	{
	case 1:		// 상한(화살표)
		strSignText = "↑";
	case 2:		// 상승(삼각형)
		strSignText = "▲";
		break;
	case 4:		// 하한(화살표)
		strSignText = "↓";
		break;
	case 5:		// 하락(삼각형)
		strSignText = "▼";
		break;
	case 3:		// 보합
	default:
		strSignText = "";
	}
	
	return strSignText;
}

int CGrid_Comm::GetSelectRow()
{
	return m_nLastSelRow;
}

CString CGrid_Comm::GetStockStatus(CString strItemCode, CString strItemName, COLORREF& clrbunText, COLORREF& clrbunBack)
{
	CString strStockStatus;
	
	CString strData;
	strData = strItemName.GetAt(0);
	
	if(strData != " ")
	{
		if(strData == "-")
		{
			strData = "불";				// 불성실 공시
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "/")
		{
			strData = "액";				// 액면분할, 액면병합
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "#")
		{
			strData = "투";				// 투자유의
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "!")
		{
			strData = "이";				// 이상급등
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "%")
		{
			strData = "관";				// 관리
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "X")
		{
			strData = "거";				// 거래정지
			clrbunBack = RGB(152, 82, 40);
			clrbunText = RGB(255, 255, 255);
		}
		else if(strData == "&")
		{
			strData = "기";				// 기준가 발생
			clrbunBack = RGB(246, 120, 9);
			clrbunText = RGB(255, 255, 255);
			
		}
		
	}
	else
	{
		clrbunBack = RGB(255, 255, 255);
	}
	return strData;
}

BOOL CGrid_Comm::GetRowToCodeName(int nRow, CString& strCode, CString& strName, int& nMarketType)
{
	BOOL bResult = FALSE;
	
	if (nRow < GetNumberRows())
	{
		int nCol = GetCodeColNumber();
		
		CUGCell cell;
		GetCell(COL_NAME, nRow, &cell);
		
		strCode = cell.GetOtherJongCode();
		strName = cell.GetLabelText();
		nMarketType = cell.GetOtherDataInt();
		
		bResult = TRUE;
	}
	else 
		bResult = FALSE;
	
	return bResult;	
}

CString CGrid_Comm::LoadMemo(CString strKey)
{
	if (strKey.IsEmpty())
		return "";
	
	strKey.TrimRight();
	strKey = "A" + strKey;
	
	CString dir, dat;
	char key[12], lBytes[4];
	
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	dir.Format("%s\\%s\\%s\\memo.mmo", pwndMainFrm->Variant(homeCC), "user", pwndMainFrm->Variant(nameCC));
	CFileFind finder;
	
	if (!finder.FindFile(dir))
		return "";
	
	TRY
	{ 
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}
					
					if (!strKey.Compare(key))
					{
						dat.ReleaseBuffer();
						return dat;
						break;
					}
					//160928 강수현: 0014541: [7127] 항목편집의 "메모" 추가후 현재가 화면에서 종목컨트롤 클릭하여 메모 작성시 조건검색에는 마지막에 추가한 종목의 메모만 나오는현상
					//return "";
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);
		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	return "";
}

void CGrid_Comm::SaveMemo(CString strKey, CString strData)
{
	if (strKey.IsEmpty())
		return;

	strKey = "A" + strKey;

	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();	
	CString rDir, wDir;
	rDir.Format("%s\\%s\\%s\\memo.mmo", pwndMainFrm->Variant(homeCC), "user", pwndMainFrm->Variant(nameCC));
	wDir.Format("%s\\%s\\%s\\memo.mm2", pwndMainFrm->Variant(homeCC), "user", pwndMainFrm->Variant(nameCC));
	
	char key[12], lBytes[4];
	CString dat;
	CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate);
	
	if (!strData.IsEmpty())
	{
		FillMemory(key, sizeof(key), ' ');
		FillMemory(lBytes, sizeof(lBytes), ' ');
		
		strcpy(key, strKey.operator LPCTSTR());
		itoa(strData.GetLength(), lBytes, 10);
		wFile.Write(key, 12);
		wFile.Write(lBytes, 4);
		wFile.Write(strData, strData.GetLength());
	}
	
	CFileFind finder;
	if (!finder.FindFile(rDir))
	{
		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);
		return;
	}
	
	TRY
	{
		CFile rFile(rDir, CFile::modeRead);
		
		UINT nBytesRead;
		do
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');
			
			nBytesRead = rFile.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);
					
					if ((int)nBytesRead != lSize)
						break;
					
					if (!strKey.Compare(key))
					{
						dat.ReleaseBuffer();
						continue;
					}
					wFile.Write(key, 12);
					wFile.Write(lBytes, 4);
					wFile.Write(dat, nBytesRead);
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
			
		}while((int)nBytesRead);
		rFile.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
		
		wFile.Close();
	CopyFile(wDir, rDir, false);
	DeleteFile(wDir);
	
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