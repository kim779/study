// Grid_Comm.cpp: implementation of the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Grid_Comm.h"

#include "../../DrCommon/Dr_Control/DrXLSFile.h"
#include "../MainFrmWnd.h"
#include "../../Dr_Include/CompanySS.h"

#include "../FieldEditDlg.h"

#include "../../DrCommon/UGridCtrl/FindDataGridUtil.h"	

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const		LPCTSTR		HEADNAME_RESULT[] = { _T("종목명"), _T("현재가"), _T("대비"), 
_T("등락률"), _T("연속등락"), _T("거래량"), _T("전일거래대비") };
const		int				HEADWIDTH_RESULT[] = { 20, 65, 50, 50, 50, 70, 60 };


const		LPCTSTR		HEADNAME_REALRESULT[] = { _T("구분"), _T("시간"), _T("종목명"), _T("현재가"), _T("대비"),
_T("등락률"), _T("진입"), _T("거래량"), _T("전일거래대비") };
const		int				HEADWIDTH_REALRESULT[] = { 40, 40, 100, 50, 50, 30, 30, 70, 60 };

const		LPCTSTR		HEADNAME_INDUSTRY[] = { _T(""), _T("업종명"), _T("지수"), _T("대비"), 
_T("등락률"), _T("연속등락"), _T("거래량(천주)"), _T("전일거래대비") };
const		int				HEADWIDTH_INDUSTRY[] = { 5, 110, 50, 45, 40, 50, 70, 60 };

const		LPCTSTR		HEADNAME_LIST[] = { _T("S"), _T("조건명") };
const		int				HEADWIDTH_LIST[] = { 20, 80 };

const		LPCTSTR		HEADNAME_BASE[] = {_T("분"), _T("신"), _T("∨종목코드"), _T("종목명"), __T("예상체결가") };
const		int			HEADWIDTH_BASE[] = { 0, 0, 60, 105, 80};

const		LPCTSTR		HEADNAME_BASE_REAL[] = {_T("분"), _T("신"), _T("∨종목코드"), _T("종목명")};
const		int			HEADWIDTH_BASE_REAL[] = { 0, 0, 60, 105};

const		LPCTSTR		REAL_HEADNAME_BASE[] = {_T("분"), _T("신"), _T("구분"), _T("시간"), _T("종목명") };
const		int			REAL_HEADWIDTH_BASE[] = { 0, 0, 40, 60, 105};

const LPCSTR TYPE_MARKETCODE[] = {_T("0001"), _T("1001"), _T("2001"), _T("3001"), _T("4001")};


CString DEFVAL_SELECTCHK = "1";

#define ROW_DEFHEIGHT									18
#define ROW_HEAD_DEFHEIGHT						20

const   COLORREF  clrHeader		= COLOR_GRID_HEADER_FACE;
const   COLORREF  clrSelRow		= COLOR_GRID_SEL_ROW;
const   COLORREF  clrEvenRow	= COLOR_GRID_EVEN_ROW;
const   COLORREF  clrOddRow		= COLOR_GRID_ODD_ROW;
const   COLORREF  clrGridLine	= COLOR_GRID_LINE;

const   COLORREF  clrBtn		= RGB(255, 255, 255);
const   COLORREF  clrBtnText	= RGB(0, 0, 0);

#define RED     RGB(255,0,0)
#define YELLOW  RGB(255,255,0)
#define MAGENTA RGB(255,0,255)
#define WHITE   RGB(255,255,255)
#define BLUE    RGB(0,0,255)
//#define BLACK		RGB(0,0,0)
#define BLACK		RGB(57,57,57)


//# 삼성증권
#define	UM_CHANGE_PARENT			(WM_USER + 450)		// 독립실행시 프레임 윈도우 변경을 통지하기위해...

BEGIN_MESSAGE_MAP(CGrid_Comm,CUGCtrl)
	//{{AFX_MSG_MAP(ShowGrid)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_COMMAND_RANGE(ID_MENU_START, ID_MENU_END, OnSelectMenu)
	ON_WM_TIMER()
	//# 삼성증권
	ON_MESSAGE(UM_CHANGE_PARENT,	OnChangeChildFrameWindowHandle)
END_MESSAGE_MAP()


void CGrid_Comm::OnTimer( UINT nIDEvent ) 
{
	if(nIDEvent == 100)
	{
		KillTimer(nIDEvent);
		m_bSendTR = FALSE;
	}

	CUGCtrl::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGrid_Comm::CGrid_Comm()
{
	m_nSortMethode	= UG_SORT_DESCENDING;
	m_nSortArrow		= AddCellType(&m_sortArrow);
	m_nSortCol			= -1;
	
	m_bCallAdjustOneTime = FALSE;
	m_nLastSelRow = -1;
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	m_clrHeader  = COLOR_GRID_LINE;
	m_clrSelRow  = COLOR_GRID_SEL_ROW;
	
	m_penGridLine.CreatePen(PS_SOLID, 1, clrGridLine);
	
	m_hChildFrame = NULL;
	
	m_nIndustry = 0;

	//<< 07.07.10 이준옥
	m_pDragBitmap           = NULL;
	m_pDragImage			= NULL;
	m_bDragAndDrop			= FALSE;
	m_MouseMode				= MOUSE_NOTHING;
	m_bFocus				= FALSE;
	m_lDragRow				= -1;
	m_nDragCol				= 0;	// 종목명위치
	m_uGridNotifyControlMsg = ::RegisterWindowMessage("FN_GRID_NOTIFY_CONTROL_MSG");
	//>> 07.07.10 이준옥
	m_bChkDelOut = FALSE;

	m_pFieldEditDlg = NULL;
	m_nFixCol = 0;

	m_nCol_Hide = -1;
	m_bCol_HideState = FALSE;
	m_bCol_ForecastPrice_HideState = FALSE;

	m_nPrevRow			= -1;
	m_nLastCol			= -1;

	m_bSendTR = FALSE;

	m_bSelectCheckFlag = TRUE;
}

CGrid_Comm::~CGrid_Comm()
{
	m_fontDefault.DeleteObject();
	m_penGridLine.DeleteObject();
}

void CGrid_Comm::SetInit(CString strType, int nImgID)
{
	CWnd *pwndChildFrm = GetFrameWnd(this);
	m_hChildFrame = pwndChildFrm->GetSafeHwnd();


	SetSH_Width(0);
	SetSH_NumberCols(0);
	SetHighlightRow(FALSE);

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

	SetUserSizingMode(TRUE);
	

	HeaderSetup(strType, nImgID);
			
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

void CGrid_Comm::OnDestroy()
{
	m_tipInfo.RemoveTool(this);
}

void CGrid_Comm::ClearAll()
{
	m_nOffset_CodeToRow = 0;
	m_mapCodeToRow.RemoveAll();
////2008.07.31 유진 - 탈퇴종목 삭제 관련
	m_saCode.RemoveAll();
	m_saCode.FreeExtra();
//	
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
	
	m_nLastSelRow = -1;
	/*SetNumberRows(0, FALSE);*/
	/*RedrawAll();*/

	m_bSelectCheckFlag = TRUE;
}

BOOL CGrid_Comm::PreTranslateMessage(MSG* pMsg) 
{
	//<< 07.07.10 이준옥
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
//		FnMouseMove(point);

		//D&D할때 스크롤 현상이 있어서 방지하기 위해서 return 1L함.
		if(m_MouseMode == MOUSE_CODE_DRAG)	return 1L;
	}
	//>> 07.07.10 이준옥
//  	else if( pMsg->message == WM_RBUTTONUP)
//  	{
//  		CPoint point = pMsg->pt;
//  		FnRButtonUp(point);
//  	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CGrid_Comm::OnSetup(CString strType, UINT nImgID)
{
	m_bCandleLoadFalg = LoadCandleIndexToXml();
}

void CGrid_Comm::HeaderSetup(CString strType, UINT nImgID)
{
	m_imgList.DeleteImageList();
	m_strType = strType;
	
	m_bCallAdjustOneTime = FALSE;
	ClearAll();
	
	CMainFrmWnd* pWnd = (CMainFrmWnd*)GetParent();

	int nColCnt;
	if (strType == "RESULT" && pWnd->m_szScreenKey == "IB712800")
		nColCnt = _countof(HEADNAME_BASE_REAL);
	else if (strType == "RESULT")
		nColCnt = _countof(HEADNAME_BASE);
	else if (strType == "LIST")
	{
		nColCnt = _countof(HEADNAME_LIST);
		
		if ((int)nImgID > -1)
		{
			m_imgList.Create(nImgID, 0, 3, RGB(255, 0, 255));
			m_imgList.Add(AfxGetApp()->LoadIcon(nImgID));
		}
	}
	else if (strType == "GROUP")
		nColCnt = _countof(HEADNAME_LIST);
	else if (strType == "REAL")
	{
		nColCnt = _countof(REAL_HEADNAME_BASE);
		
		if ((int)nImgID > -1)
		{
			m_imgList.Create(nImgID, 36, 3, RGB(255, 0, 255));
			m_imgList.Add(AfxGetApp()->LoadIcon(nImgID));
		}
	}
	else if (strType == "INDUSTRY")
	{
		nColCnt = _countof(HEADNAME_INDUSTRY);
		if ( !m_imgList.m_hImageList)
		{
			if ( !m_imgList.Create( 15, 16, ILC_MASK | ILC_COLORDDB, 1, 1 ) )
				OutputDebugString("Can not Find file");
			
			CString strImgPath, strRootPath;
			strRootPath = pWnd->Variant(homeCC);
			strImgPath.Format("%s\\image\\거래소.bmp", strRootPath);
			CBitmap bitmap;
			bitmap.Attach ((HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
			m_imgList.Add(&bitmap, RGB(255, 0, 255));
			
			strImgPath.Format("%s\\image\\코스닥.bmp", strRootPath);
			bitmap.Attach ((HBITMAP)::LoadImage(NULL, strImgPath, IMAGE_BITMAP, 0, 0, 
				LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
			m_imgList.Add(&bitmap, RGB(255, 0, 255));
			bitmap.DeleteObject();
		}
	}
	
	SetNumberCols(nColCnt, FALSE);
	
	if ((strType == "GROUP") || (strType == "LIST")) 
	{
		SetTH_NumberRows(0);
		SetTH_Height(0);
		
		SetDefRowHeight(ROW_DEFHEIGHT + 5);
		return;
	}
	
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		QuickSetAlignment(nCol, -1, UG_ALIGNVCENTER | UG_ALIGNCENTER);
		
		if (strType == "RESULT" && pWnd->m_szScreenKey != "IB712800")
		{
			QuickSetText(nCol, -1, HEADNAME_BASE[nCol]);

			if(m_bCol_ForecastPrice_HideState == FALSE && HEADNAME_BASE[nCol] == "예상체결가")
				SetColWidth(nCol, 0);
			else
				SetColWidth(nCol, (int)(HEADWIDTH_BASE[nCol]));

		}
		else if (strType == "RESULT" && pWnd->m_szScreenKey == "IB712800")
		{
			QuickSetText(nCol, -1, HEADNAME_BASE_REAL[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_BASE_REAL[nCol]));
		}
		else if (strType == "LIST")
		{
			QuickSetText(nCol, -1, HEADNAME_LIST[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_LIST[nCol]));
		}
		else if (strType == "GROUP")
		{
			
		}
		else if (strType == "REAL")
		{
			QuickSetText(nCol, -1, REAL_HEADNAME_BASE[nCol]);
			SetColWidth(nCol, (int)(REAL_HEADWIDTH_BASE[nCol]));
		}
		else if (strType == "INDUSTRY")
		{
			QuickSetText(nCol, -1, HEADNAME_INDUSTRY[nCol]);
			SetColWidth(nCol, (int)(HEADWIDTH_INDUSTRY[nCol]));
		}
		
		QuickSetBackColor(nCol, -1, m_clrHeader);
		QuickSetTextColor(nCol, -1, COLOR_GRID_HEADER_TEXT);
		QuickSetFont(nCol, -1, &m_fontDefault);
	}
	
	SetHighlightRow(TRUE);
	AdjustComponentSizes();
	
//	SetInit();
	
	/*RedrawAll();*/
}

void CGrid_Comm::SetColResize(BOOL bState)
{
	m_bCallAdjustOneTime = !bState;
	
	if (bState == TRUE)
	{
		/*AdjustComponentSizes();*/
		/*RedrawAll();*/
	}
}

void CGrid_Comm::SetAddRow_Group(CString strName, int nID, BOOL bRedraw)
{
	CUGCell cell;
	//COLORREF clrBk;
	
	AppendRow();
	int nRow = GetNumberRows() - 1;
	int nCol = 0;
	
	int buttonIndex = AddCellType(&m_button);
	QuickSetAlignment(nCol, nRow, UG_ALIGNLEFT | UG_ALIGNVCENTER);
	QuickSetCellType(nCol, nRow, buttonIndex);
	QuickSetCellTypeEx(nCol, nRow, UGCT_BUTTONNOFOCUS);
	QuickSetText(nCol, nRow, strName);
	
	QuickSetBackColor(nCol, nRow, clrBtn);
	QuickSetTextColor(nCol, nRow, clrBtnText);
	QuickSetAlignment(nCol, nRow, UG_ALIGNCENTER | UG_ALIGNVCENTER); 
	QuickSetFont(nCol, nRow, &m_fontDefault);

	
	GetCell(nCol, nRow, &cell);
	cell.SetOtherDataInt(nID);
	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
	
		/*
		GetEvenOddBkColor(nRow, clrBk);
		
		  nCol = 0;
		  GetCell(nCol, nRow, &cell);
		  cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
		  cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
		  cell.SetFont(&m_fontDefault);
		  
			cell.SetBorderColor(&m_penGridLine);
			cell.SetBorder(UG_BDR_RTHIN);
			cell.SetBackColor(clrBk);
			
			  cell.SetText(strName);
			  cell.SetOtherDataInt(nID);
			  
				SetCell(nCol, nRow, &cell);
	*/
	
// 	if (bRedraw == TRUE) 
// 		RedrawAll();
}

void CGrid_Comm::SetAddRow_List(ConditionInfo	*pstConditionInfo, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	
	AppendRow(bRedraw);
	int nRow = GetNumberRows() - 1;
	int nCol;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	CString strTitle = "";
	if (pstConditionInfo->strAlertKey == "TRUE")
		strTitle = "S";
	else if (pstConditionInfo->strAlertKey == "FALSE")
		strTitle = "";
	else  if (pstConditionInfo->strAlertKey.IsEmpty() == FALSE)
		strTitle = "S";
	
	if (m_imgList.GetImageCount() > 0)
	{
		if (strTitle == "S")
			cell.SetImageList(0, &m_imgList);
	}
	else
		cell.SetText(strTitle);
	cell.SetOtherDataCellText(pstConditionInfo->strGroupName);
	cell.SetOtherDataInt(pstConditionInfo->nGroupID);
	cell.SetOtherJongCode(pstConditionInfo->strExplain);
	cell.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cell);
	
	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	cell.SetText(pstConditionInfo->strName);
	cell.SetOtherJongCode(pstConditionInfo->strPath);
	cell.SetOtherDataCellText(pstConditionInfo->strAlertKey);
	cell.SetOtherDataLong(pstConditionInfo->nOrderCnt);
	cell.SetReadOnly(TRUE);

	SetCell(nCol, nRow, &cell);
	
	if (bRedraw == TRUE) 
		RedrawAll();
}

void CGrid_Comm::SetAddRow_Result(int nRow, TR_RESULT_LIST stTR_RESULT_LIST, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	int nCol;
	
	char *pchData;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	/*
	nCol = (int)COL_CHECK;
	QuickSetCellType(nCol, nRow, UGCT_CHECKBOX);
	QuickSetCellTypeEx(nCol, nRow, UGCT_CHECKBOXCHECKMARK);
	QuickSetText(nCol, nRow, DEFVAL_SELECTCHK);
	QuickSetBackColor(nCol, nRow, clrBk);
	*/
	
	nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	pchData = new char[sizeof(stTR_RESULT_LIST.sMarket) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sMarket) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sMarket, sizeof(stTR_RESULT_LIST.sMarket));
	strTemp = (CString)pchData;
	delete [] pchData;

	if (m_imgList.GetImageCount() > 0)
	{
		if(strTemp == "1")
			cell.SetImageList(0, &m_imgList);
		else
			cell.SetImageList(1, &m_imgList);
	}

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);

	nCol = 1;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sName) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sName) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sName, sizeof(stTR_RESULT_LIST.sName));
	strTemp = (CString)pchData;
	strTemp.TrimLeft(" ");
	strTemp.TrimRight(" ");
	cell.SetText(strTemp);
	delete [] pchData;
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sCode) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sCode) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sCode, sizeof(stTR_RESULT_LIST.sCode));
	cell.SetOtherJongCode((CString)pchData);
	delete [] pchData;
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sMarket) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sMarket) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sMarket, sizeof(stTR_RESULT_LIST.sMarket));
	cell.SetOtherDataInt(atoi(pchData));
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	
	int nSign;
	COLORREF clrSignText;
	pchData = new char[sizeof(stTR_RESULT_LIST.sSign) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sSign) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sSign, sizeof(stTR_RESULT_LIST.sSign));
	nSign = atoi(pchData);
	delete [] pchData;
	
	GetSignColor(nSign, clrSignText);
	
	nCol = 2;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sPrice) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sPrice) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sPrice, sizeof(stTR_RESULT_LIST.sPrice));
	cell.SetOtherDataCellText((CString)pchData);
	
	int nUnitNum = 0;
	if (m_strType == "INDUSTRY")
		nUnitNum = 100;
	
	strTemp = GetFormatStr(atof(pchData), nUnitNum, FALSE, "");
	cell.SetText(strTemp);
	cell.SetOtherDataLong(atol(pchData) * 100);
	cell.SetTextColor(clrSignText);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	nCol = 3;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sChange) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sChange) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sChange, sizeof(stTR_RESULT_LIST.sChange));
	
	strTemp = GetFormatStr(atof(pchData), nUnitNum, FALSE, "");
	if(atof(strTemp) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);

	if ((nSign == 4) || (nSign == 5))
		cell.SetOtherDataLong((long)(atof(pchData) * 100.0) * -1);
	else 
		cell.SetOtherDataLong((long)(atof(pchData) * 100.0));

	cell.SetTextColor(clrSignText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetSign(nSign);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	nCol = 4;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sChgRate) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sChgRate) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sChgRate, sizeof(stTR_RESULT_LIST.sChgRate));
	
	
	CString strSign = " ";
	if ((nSign == 4) || (nSign == 5))
	{
		strSign = "-";
		cell.SetOtherDataLong((long)(atof(pchData) * 100.0) * -1);
	}
	else 
		cell.SetOtherDataLong((long)(atof(pchData) * 100.0));

	CString strData = pchData;
	strData.TrimLeft();
	strData.TrimRight();
	strTemp.Format("%s%s%s", strSign, strData, "%");
	if(atof(strData) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);
	
	cell.SetTextColor(clrSignText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	
	nCol = 5;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sSignCnt) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sSignCnt) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sSignCnt, sizeof(stTR_RESULT_LIST.sSignCnt));
	
	strTemp.Format("%ld", atol(pchData));
	
	cell.SetOtherDataCellText(strTemp);
	cell.SetText(strTemp);
	cell.SetOtherDataLong(atol(pchData));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	nCol = 6;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sVolume) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sVolume) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sVolume, sizeof(stTR_RESULT_LIST.sVolume));
	
	cell.SetOtherDataCellText((CString)pchData);
	strTemp = GetFormatStr(atol(pchData), 0, FALSE, "");

	if(atof(strTemp) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp);

	cell.SetOtherDataInt(nSign);
	cell.SetOtherDataLong(atol(pchData));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	
	nCol = 7;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sVolumeRate) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sVolumeRate) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sVolumeRate, sizeof(stTR_RESULT_LIST.sVolumeRate));
	strTemp = GetFormatStr(atof(pchData), 100, FALSE, "");
	if(atof(strTemp) == 0)
		cell.SetText("");
	else
		cell.SetText(strTemp + "%");
	
//	if (nSign >= 4)
//		cell.SetOtherDataLong((long)(atof(pchData) * -100.0));
//	else 
		cell.SetOtherDataLong((long)(atof(pchData) * 100.0));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	if (bRedraw == TRUE) 
		RedrawAll();
}

void CGrid_Comm::SetAddRow_Alert(int nRow, TR_RESULT_LIST stTR_RESULT_LIST, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	
	int nCol;
	char *pchData;
	CString strTemp;
	
	GetEvenOddBkColor(nRow, clrBk);
	
	nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetText(" ");
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	/*
	CDateTimeCtrl ctrlDateTime;
	CTime timeDest;
	ctrlDateTime.GetTime(timeDest);
	strTemp.Format("%h:%m", timeDest);
	*/
	strTemp = "";
	cell.SetText(strTemp);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sName) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sName) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sName, sizeof(stTR_RESULT_LIST.sName));
	strTemp = (CString)pchData;
	strTemp.TrimLeft(" ");
	strTemp.TrimRight(" ");
	cell.SetText(strTemp);
	delete [] pchData;
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sCode) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sCode) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sCode, sizeof(stTR_RESULT_LIST.sCode));
	cell.SetOtherJongCode((CString)pchData);
	
	CString strCode;
	strCode = (CString)pchData;
	strCode.TrimRight(" ");
	strCode.TrimLeft(" ");
	
	strTemp.Format("%d", nRow);
	m_mapCodeToRow.SetAt(strCode, strTemp);
////2008.07.31 유진 - 탈퇴종목 삭제 관련
	m_saCode.Add(strCode);
//	
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	int nSign;
	COLORREF clrSignText;
	pchData = new char[sizeof(stTR_RESULT_LIST.sSign) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sSign) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sSign, sizeof(stTR_RESULT_LIST.sSign));
	nSign = atoi(pchData);
	delete [] pchData;
	
	GetSignColor(nSign, clrSignText);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sPrice) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sPrice) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sPrice, sizeof(stTR_RESULT_LIST.sPrice));
	cell.SetOtherDataCellText((CString)pchData);
	
	int nUnitNum = 0;
	if (m_strType == "INDUSTRY")
		nUnitNum = 100;
	
	strTemp = GetFormatStr(atof(pchData), nUnitNum, FALSE, "");
	cell.SetText(strTemp);
	cell.SetOtherDataLong(atol(pchData));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sChange) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sChange) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sChange, sizeof(stTR_RESULT_LIST.sChange));
	strTemp = GetFormatStr(atof(pchData), nUnitNum, FALSE, "");
	cell.SetText(strTemp);
	if ((nSign == 4) || (nSign == 5))
		cell.SetOtherDataLong(atol(pchData) * -1);
	else 
		cell.SetOtherDataLong(atol(pchData));

	cell.SetTextColor(clrSignText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetSign(nSign);
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sChgRate) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sChgRate) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sChgRate, sizeof(stTR_RESULT_LIST.sChgRate));
	
	cell.SetText((CString)pchData + "%");
	cell.SetOtherDataLong((long)(atof(pchData) * 100.0));
		
	cell.SetTextColor(clrSignText);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	
	/*
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	  pchData = new char[sizeof(stTR_RESULT_LIST.sSignCnt) + 1];
	  memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sSignCnt) + 1);
	  memcpy(pchData, stTR_RESULT_LIST.sSignCnt, sizeof(stTR_RESULT_LIST.sSignCnt));
	  
		cell.SetOtherDataCellText((CString)pchData);
		cell.SetText((CString)pchData);
		cell.SetOtherDataLong(atol(pchData));
		
		  cell.SetBorderColor(&m_penGridLine);
		  cell.SetBorder(UG_BDR_RTHIN);
		  cell.SetBackColor(clrBk);
		  
			SetCell(nCol, nRow, &cell);
			delete [] pchData;
	*/
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	cell.SetText("0");
	cell.SetOtherDataInt(0);
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sVolume) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sVolume) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sVolume, sizeof(stTR_RESULT_LIST.sVolume));
	
	cell.SetOtherDataCellText((CString)pchData);
	strTemp = GetFormatStr(atol(pchData), 0, FALSE, "");
	cell.SetText(strTemp);
	cell.SetOtherDataInt(nSign);
	cell.SetOtherDataLong(atol(pchData));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stTR_RESULT_LIST.sVolumeRate) + 1];
	memset(pchData, 0x00, sizeof(stTR_RESULT_LIST.sVolumeRate) + 1);
	memcpy(pchData, stTR_RESULT_LIST.sVolumeRate, sizeof(stTR_RESULT_LIST.sVolumeRate));
	strTemp = GetFormatStr(atof(pchData), 2, FALSE, "");
	cell.SetText(strTemp + "%");
	
	if (nSign >= 4)
		cell.SetOtherDataLong((long)(atof(pchData) * -100.0));
	else 
		cell.SetOtherDataLong((long)(atof(pchData) * 100.0));
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	SetCell(nCol, nRow, &cell);
	delete [] pchData;
	
	if (bRedraw == TRUE) 
		RedrawAll();

	EnableWindow(TRUE);

}

void CGrid_Comm::SetAddRow_Real(SIGNAL_CONDITION stSIGNAL_CONDITION, BOOL bJobChk, BOOL bRedraw)
{
	CUGCell cell;
	COLORREF clrBk;
	
	char *pchData;
	CString strData, strCode;
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.sCode) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sCode) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sCode, sizeof(stSIGNAL_CONDITION.sCode));
	strCode = (CString)pchData;
	delete [] pchData;
	
	
	int nRow, nCol;
	CString strRow;
//2008.07.31 유진 - 탈퇴종목 삭제 관련 - 추가
//-=>
	pchData = new char[sizeof(stSIGNAL_CONDITION.sJobFlag) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sJobFlag) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag));

	BOOL	bSaved = FALSE;
	CString strSavedCode;
	int nIndex = 0;
	for(nIndex=0; nIndex < m_saCode.GetSize(); nIndex++)
	{
		strSavedCode = m_saCode.GetAt(nIndex);
		if(strSavedCode.CompareNoCase(strCode) == 0)
		{
			bSaved = TRUE;
			break;
		}
	}
//<=-	
	if( bSaved == TRUE)
	{
		nRow = nIndex;
		GetEvenOddBkColor(nRow, clrBk);

		if(m_bChkDelOut == TRUE && strcmp(pchData, "O") == 0)
		{
			m_saCode.RemoveAt(nIndex);
			DeleteRow(nIndex);
			m_nLastSelRow--;
			return;
		}
	}
	else
	{
		if(bJobChk == TRUE)
		{
//2008.07.31 유진 - 탈퇴종목 삭제 관련 - 추가
//-=>
	//		if(m_bChkDelOut == FALSE)
	//		{			
	//			if ((CString)pchData != "N")
	//				return;	
	//		}
			if(strcmp(pchData, "O") == 0)
				return;			
			
		}

		if(InsertRow(0) == UG_SUCCESS)
		{
			m_saCode.InsertAt(0, strCode);
			nRow = 0;
			GetEvenOddBkColor(GetNumberRows() - 1, clrBk);
			m_nLastSelRow++;
		}
	}




/*
	
//
	if (m_mapCodeToRow.Lookup(strCode, strRow) == TRUE)
	{
		nRow = atoi(strRow) + m_nOffset_CodeToRow;
		GetEvenOddBkColor(nRow, clrBk);
	}
	else
	{
		if (bJobChk == TRUE)
		{
//2008.07.31 유진 - 탈퇴종목 삭제 관련 - 수정
//-=>
		//	pchData = new char[sizeof(stSIGNAL_CONDITION.sJobFlag) + 1];
		//	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sJobFlag) + 1);
		//	memcpy(pchData, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag));
			
			//# 신규:'N', 재진입:'R', 탈퇴:'O'
//			if ((CString)pchData != "N")
//				return;
			if(strcmp(pchData, "O") == 0)
				return;
//<=-			
		}
		
		if (InsertRow(0) == UG_SUCCESS)
		{
			m_nOffset_CodeToRow++;
			
			nRow = 0;
			GetEvenOddBkColor(GetNumberRows() - 1, clrBk);
			m_nLastSelRow++;
		}	
	}
//2008.07.31 유진 - 탈퇴종목 삭제 관련 - 추가
//-=>
		
//			
//<=-
*/	
	nCol = 0;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.sJobFlag) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sJobFlag) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag));
	strData = (CString)pchData;
	delete [] pchData;

	
	//# 신규:'N', 재진입:'R', 탈퇴:'O'
	if (m_imgList.GetImageCount() > 0)
	{
		if (strData == "N")
			cell.SetImageList(0, &m_imgList);
		else if (strData == "R")
			cell.SetImageList(1, &m_imgList);
		else if (strData == "O")
			cell.SetImageList(2, &m_imgList);
	}
	else
		cell.SetText(strData);

//2008.07.31 유진 - 탈퇴종목 삭제 관련 - 추가
//-=>
	cell.SetText(strData);
//<=-

	
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.sTime) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sTime) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sTime, sizeof(stSIGNAL_CONDITION.sTime));
	strData = GetFormatStr(atof(pchData), 0, TRUE, "XX:XX:XX");
	cell.SetText(strData);
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.sName) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sName) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sName, sizeof(stSIGNAL_CONDITION.sName));
	strData = (CString)pchData;
	strData.TrimLeft(" ");
	strData.TrimRight(" ");
	cell.SetText(strData);
	delete [] pchData;
	
	//# Code 
	cell.SetOtherJongCode(strCode);
	
	//# Market
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sMarket) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sMarket) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sMarket, sizeof(stSIGNAL_CONDITION.stCommon.sMarket));
	cell.SetOtherDataInt(atoi(pchData));
	delete [] pchData;
	
	cell.SetOtherDataCellText(strCode);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sPrice) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sPrice) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sPrice, sizeof(stSIGNAL_CONDITION.stCommon.sPrice));
	strData = GetFormatStr(atof(pchData), 0, FALSE, "");
	cell.SetText(strData);
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	int nSign;
	COLORREF clrSignText;
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sSign) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sSign) + 1);
	memcpy(pchData, &stSIGNAL_CONDITION.stCommon.sSign, sizeof(stSIGNAL_CONDITION.stCommon.sSign));
	nSign = atoi(pchData);
	delete [] pchData;
	
	GetSignColor(nSign, clrSignText);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sChage) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sChage) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sChage, sizeof(stSIGNAL_CONDITION.stCommon.sChage));
	strData = GetFormatStr(atof(pchData), 0, FALSE, "");
	cell.SetText(strData);
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	
	cell.SetTextColor(clrSignText);
	cell.SetSign(nSign);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sChgRate) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sChgRate) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sChgRate, sizeof(stSIGNAL_CONDITION.stCommon.sChgRate));
	//strData = GetFormatStr(atof(pchData), 2, FALSE, "");
	cell.SetText((CString)pchData + "%");
	cell.SetOtherDataLong((long)(atof(pchData) * 100.0));
	cell.SetTextColor(clrSignText);
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.sEntryCnt) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sEntryCnt) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sEntryCnt, sizeof(stSIGNAL_CONDITION.sEntryCnt));
	strData = GetFormatStr(atol(pchData), 0, FALSE, "");
	delete [] pchData;
	
	cell.SetText(strData);
	cell.SetOtherDataInt(atol(strData));
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sVolume) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sVolume) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sVolume, sizeof(stSIGNAL_CONDITION.stCommon.sVolume));
	strData = GetFormatStr(atof(pchData), 0, FALSE, "");
	cell.SetText(strData);
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sVolumeRect) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sVolumeRect) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sVolumeRect, sizeof(stSIGNAL_CONDITION.stCommon.sVolumeRect));
	strData = GetFormatStr(atof(pchData), 0, FALSE, "");
	cell.SetText(strData + "%");
	delete [] pchData;
	
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	SetCell(nCol, nRow, &cell);
	
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

inline void CGrid_Comm::GetSignColor(int nSign, COLORREF &clrSignText)
{
	switch (nSign)
	{
	case 1:		// 상한(화살표)
		clrSignText = RED;
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

int CGrid_Comm::OnCanSizeTopHdg()
{
	return FALSE;
}

void CGrid_Comm::OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((m_strType != "LIST" && m_strType != "INDUSTRY" )&& (row != -1 && col != -1) && (updn == 1))
	{
		//이전 Focus가 메모Grid일 경우 저장
		CUGCell cell;
		GetCell(m_nLastCol, m_nPrevRow, &cell);

		if(!cell.GetReadOnly())
		{
			CString strCode = cell.GetOtherJongCode();
			CString strName = cell.GetOtherDataCellText();
			CString strNote = cell.GetText();
			strName.TrimRight();

			
// 			if(strNote != "")
// 			{
// 				IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
// 				if(pMainInfo)
// 					pMainInfo->SetNoteText((LPCSTR)strCode, (LPCSTR)strName, (LPCSTR)strNote);
// 			}
			

			SaveMemo(strCode.Mid(0, 6), strNote);

			StartEdit();
		}

		OnRowChange(m_nLastSelRow, row);
		m_nLastCol = col;
		m_nPrevRow = m_nLastSelRow;
	}
	else if(m_strType == "LIST" && (row != -1 && col != -1) && (updn == 1))
	{
		if (SetDrew_SelectRow(row) == TRUE)
		{
			if (m_nLastSelRow > -1)
				SetSendData(row);
		}
	}
	else if(m_strType == "INDUSTRY" && (row != -1 && col != -1) && (updn == 1))
	{
		SetDrew_SelectRow(row);

		CString strCode = "";
		CUGCell cell;
		GetCell(1, m_nLastSelRow, &cell);
		strCode = cell.GetOtherJongCode();
		strCode.TrimRight(" ");
		
		CWnd*		pModuleMainWnd;
		CMainFrmWnd* pWnd = (CMainFrmWnd*)GetParent();
		pModuleMainWnd = pWnd->m_pModuleMainWnd;
		
		CString strSendData;
		strSendData.Format("%s /t/s/p5/d20301\t%s", "IB203200", strCode);
		pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strSendData);

		this->SetFocus();
	}
}

//#include "..\..\..\Common\KHResource.h" -hidden-
//extern void KSLink_SetFormLinkTagData( CString sTag, CString sValue ); -hidden-
//extern void KSLink_SetLinkTagData( LPCTSTR sTag, LPCTSTR sValue);		-hidden-
void CGrid_Comm::SetCommMenu(POINT *point)
{
	if (m_hChildFrame == NULL)
		return;
	
	long lMenu = 0;	//::SendMessage(m_hChildFrame, UM_GET_SCREENLINKMENU, 0, 0 ); -hidden-
	if ( lMenu > 0 )
	{
		CMenu* pMenu = (CMenu*)lMenu;
		ASSERT ( pMenu );
		
		SetCommMenu_Add(m_nIndustry, pMenu);
		
		GetCursorPos(point);
		int nIndex_SelMenu = pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL | TPM_RETURNCMD,
			point->x, point->y, this);
		pMenu->DestroyMenu();
		delete pMenu;
		
		if (nIndex_SelMenu > -1)
		{
			HWND hWnd = m_hChildFrame;
			
			CString strScreenNo = "";
			strScreenNo = GetCommMenu_IndexToScreenNo(nIndex_SelMenu);
			
			int nCol = -1;
			if (m_strType == "RESULT")
				nCol = 0;
			else if (m_strType == "LIST")
				nCol = -1;
			else if (m_strType == "GROUP")
				nCol = -1;
			else if (m_strType == "REAL")
				nCol = 2;
			else if (m_strType == "INDUSTRY")
				nCol = 0;
			
			CString strCode = "";
			if (nCol > -1)
			{
				CUGCell cell;
				GetCell(nCol, m_nLastSelRow, &cell);
				strCode = cell.GetOtherJongCode();
				strCode.TrimRight(" ");
			}
			
			//KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
			// 종목코드가 여러개일 경우에는 !를 구분으로 해서 종목리스트를 만든다.
			// 마지막 종목다음에는 !를 넣지않아야 한다.
			// 관심설정창 MenuID : UM_PORTFOLIO_REG = 4316
			if (strScreenNo.IsEmpty() == TRUE)
			{
				if(nIndex_SelMenu==4316)
				{
					int nItemCount = GetNumberRows();
					int nMarketType = 0;
					CString strData = "";

					for(int nRow = 0; nRow < nItemCount; nRow++)
					{
						GetRowToCode(nRow, strCode, nMarketType);
						strData += strCode;

						if(nRow != nItemCount - 1)
							strData += "!";
					}
//					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strData); -hidden-
//					for(int i=0; i<nCnt; i++)
//					{
//						CUGCell cell;
//						GetCell(nCol, i, &cell);
//						strCode = cell.GetOtherJongCode();
//						if(i != nCnt-1)
//							strCode += "!";
//					}
//					KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
				}
				else
				{
//	hidden
//					if (strCode.IsEmpty() == FALSE)
//						KSLink_SetFormLinkTagData(_T("#GRID_CODE_DATA#"), strCode);
				}
				//::SendMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
				::PostMessage(hWnd, WM_COMMAND, nIndex_SelMenu, 0);
			}
			else
			{
				if (strCode.IsEmpty() == FALSE)
				{
				/* hidden-
					if (m_bIndustry == TRUE)
						KSLink_SetLinkTagData("UPCODE", strCode);		// 외부화면용
					else
						KSLink_SetLinkTagData("JCODE", strCode);		// 외부화면용
				*/
				}
				
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)4, (LPARAM)(LPCTSTR)strScreenNo);
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)strlen(strScreenNo), (LPARAM)(LPCTSTR)strScreenNo);
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo);
				//::SendMessage(hWnd, UM_CHILDSCREEN, (WPARAM)5, (LPARAM)(LPCTSTR)strScreenNo);		-hidden-
			}
		}
	}
}

void CGrid_Comm::SetCommMenu_Add(BOOL bIndustry, CMenu* pMenu)
{
//유진삭제
/*	USE_SS_HTS();

	int nMenuCnt;
	UINT nMenuType;
	
	pMenu->AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)"");
	
	if (bIndustry == TRUE)
	{
		nMenuCnt = _countof(MENU_INDUSTRY_NAME);
		CString strScreenInfo;
		for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
		{
			if (DRMENU_NAME[nIndex] == "")
				nMenuType = MF_SEPARATOR;
			else 
				nMenuType = MF_STRING;

			if(bIsHTS)
			{
				strScreenInfo = (CString)MENU_INDUSTRY_SCREEN_NOMAX[nIndex];
				strScreenInfo = strScreenInfo.Left(4);
			}
			else
			{
				strScreenInfo = (CString)MENU_INDUSTRY_SCREEN_NOEMP[nIndex];
				strScreenInfo = strScreenInfo.Left(5);
			}

			strScreenInfo = "[" + strScreenInfo + "] " + (CString)MENU_INDUSTRY_NAME[nIndex];
			pMenu->AppendMenu(nMenuType, MENU_INDUSTRY_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
		}
	}
	else
	{
		nMenuCnt = _countof(DRMENU_NAME);
		CString strScreenInfo;
		for (int nIndex = 0; nIndex < nMenuCnt; nIndex++)
		{
			if (DRMENU_NAME[nIndex] == "")
				nMenuType = MF_SEPARATOR;
			else 
				nMenuType = MF_STRING;

			if(bIsHTS)
			{
				strScreenInfo = (CString)MENU_SCREEN_NOMAX[nIndex];
				strScreenInfo = strScreenInfo.Left(4);
			}
//			else
//			{
//				strScreenInfo = (CString)MENU_SCREEN_NOEMP[nIndex];
//				strScreenInfo = strScreenInfo.Left(5);
//			}

			strScreenInfo = "[" + strScreenInfo + "] " + (CString)DRMENU_NAME[nIndex];
			pMenu->AppendMenu(nMenuType, MENU_INDEX[nIndex], (LPCTSTR)strScreenInfo); 
		}
	}
*/
}

CString CGrid_Comm::GetCommMenu_IndexToScreenNo(int nIndex_SelMenu)
{
//	USE_SS_HTS();	-hidden-

	CString strScreenNo = "";
	int nIndex;
	if (1 == m_nIndustry)
	{
//--> HTS구분없앰. 사용하면 추후 다시
//		if(bIsHTS)
		{
			for (nIndex = 0; nIndex < _countof(MENU_INDUSTRY_INDEX); nIndex++)
			{
				if ((int)MENU_INDUSTRY_INDEX[nIndex] == nIndex_SelMenu)
					strScreenNo = MENU_INDUSTRY_SCREEN_NOMAX[nIndex];
			}
		}
//		else
//		{
//			for (nIndex = 0; nIndex < _countof(MENU_INDUSTRY_INDEX); nIndex++)
//			{
//				if ((int)MENU_INDUSTRY_INDEX[nIndex] == nIndex_SelMenu)
//					strScreenNo = MENU_INDUSTRY_SCREEN_NOEMP[nIndex];
//			}
//		}
	}
	else
	{
//		if(bIsHTS)
		{
			for (nIndex = 0; nIndex < _countof(MENU_INDEX); nIndex++)
			{
				if ((int)MENU_INDEX[nIndex] == nIndex_SelMenu)
					strScreenNo = MENU_SCREEN_NOMAX[nIndex];
			}
		}
//		else
//		{
//			for (nIndex = 0; nIndex < _countof(MENU_INDEX); nIndex++)
//			{
//				if ((int)MENU_INDEX[nIndex] == nIndex_SelMenu)
//					strScreenNo = MENU_SCREEN_NOEMP[nIndex];
//			}
//		}
	}
//<--
	
	return strScreenNo;
}

void CGrid_Comm::OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed)
{
	if ((row != -1) && (updn == 1))
	{
		SetDrew_SelectRow(row);
//		SetCommMenu(point);

		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 >>
		int nCol = -1;
		if (m_strType == "RESULT")
			nCol = 3;
		else if (m_strType == "LIST")
			nCol = -1;
		else if (m_strType == "GROUP")
			nCol = -1;
		else if (m_strType == "REAL")
			nCol = 4;
		else if (m_strType == "INDUSTRY")
			nCol = 0;
		
		CString strCode = "";
		if (nCol > -1)
		{
			CUGCell cell;
			GetCell(nCol, m_nLastSelRow, &cell);
			strCode = cell.GetOtherJongCode();
			strCode.TrimRight(" ");
		}

		CWnd*		pModuleMainWnd;
		CMainFrmWnd* pWnd = (CMainFrmWnd*)GetParent();
		pModuleMainWnd = pWnd->m_pModuleMainWnd;
		GetCursorPos(point);
		ScreenToClient(point);
		ClientToScreen(point);

		CString strLPARM;
		strLPARM.Format("%s\n%d\t%d\n1301\t%s\n", "IB713000", point->x, point->y, strCode);
		// mapN \n pt.x \t pt.y \n SYM \t DATA \n
		pModuleMainWnd->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)strLPARM.operator LPCTSTR());
		// 2012.4.5 박성경: winix플랫폼 R메뉴 호출 <<
	}
}

void CGrid_Comm::OnSelectMenu(UINT nIndex)
{
//	USE_SS_HTS();	-hidden-

	int nItem = 0;
	if ((nIndex >= ID_MENU_START) && (nIndex <= ID_MENU_END))
	{
		for (nItem = 0; nItem < _countof(MENU_INDEX); nItem++)
		{
			if (MENU_INDEX[nItem] == nIndex)
				break;
		}
		
		ShardData	stShardData;
		stShardData.bOpen = TRUE;
//--> HTS구분없앰. 사용하면 추후 다시
//		if(bIsHTS)
			stShardData.strScreenNo = MENU_SCREEN_NOMAX[nItem];
//		else
//			stShardData.strScreenNo = MENU_SCREEN_NOEMP[nItem];
//<--

		stShardData.strMapNo = MENU_MAP_NO[nItem];
		
		CUGCell cell;
		GetCell(0, m_nLastSelRow, &cell);
		CString strData = cell.GetText();
		strData = strData.Mid(0, 2);
		if (strData == "ⓚ")
			stShardData.iaMarkType.Add(1);
		else if (strData == "ⓣ")
			stShardData.iaMarkType.Add(2);
		else
			stShardData.iaMarkType.Add(0);
		stShardData.saCode.Add(cell.GetOtherJongCode());
		
		CWnd* pwndParent = GetParent();
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_SHARED, (LPARAM)&stShardData);
	}
}

void CGrid_Comm::OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed)
{
	CUGCell	cell;
	CWnd *pwndParent = GetParent();
	if ((m_strType == "RESULT") || (m_strType == "INDUSTRY") || m_strType == "REAL")
	{
		// 20110328 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능 >>
		CUGCell cell;
		int nCodeCol = GetCodeColNumber();
		GetCell(nCodeCol, row, &cell);
		
		CString strCode;
		strCode = cell.GetOtherJongCode();
		if(!strCode.IsEmpty())
			pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_DBCLK, (LPARAM)(LPCTSTR)strCode);

		//if (col == 0)
 		//	pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_DBCLK, (LPARAM)row);
		// 20110328 박찬모 검색 결과 Double Click 를 통한 종목 연동 기능 <<
	}
	else if (m_strType == "LIST")
	{
		SetDrew_SelectRow(row);
// 		if (SetDrew_SelectRow(row) == TRUE)
// 			SetSendData(row);
	}
}

void CGrid_Comm::SetSendData(int nRow)
{
	CUGCell	cell;
	CWnd *pwndParent = GetParent();
	if ((m_strType == "RESULT") || (m_strType == "REAL") || (m_strType == "INDUSTRY"))
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_CLK, (LPARAM)nRow);
	else if (m_strType == "LIST")
	{
		if (GetNumberRows() <= 0)
			return;
	
		GetCell(0, nRow, &cell);
		
		ConditionInfo	stConditionInfo;
		stConditionInfo.nGroupID = cell.GetOtherDataInt();
		stConditionInfo.strGroupName = cell.GetOtherDataCellText();
		
		stConditionInfo.strExplain = cell.GetOtherJongCode();
		
		GetCell(1, nRow, &cell);
		stConditionInfo.strName = cell.GetText();
		
		stConditionInfo.strPath = cell.GetOtherJongCode();
		stConditionInfo.strAlertKey = cell.GetOtherDataCellText();
		stConditionInfo.nOrderCnt = cell.GetOtherDataLong();
		
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_LIST_CLK, (LPARAM)&stConditionInfo);

		m_bSendTR = TRUE;
		SetTimer(100, 2000, NULL);
	}
	else if (m_strType == "GROUP")
	{
		GetCell(0, nRow, &cell);
		CString strGroupName = cell.GetText();
		
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_GROUP_CLK, (LPARAM)&strGroupName);
	}
}



void CGrid_Comm::SetSelectRow(int nRow)
{
	CUGCell	cell;
	m_nLastSelRow = -1;
	OnRowChange(m_nLastSelRow, nRow);
	
	CWnd *pwndParent = GetParent();
	if (m_strType == "LIST")
	{
		if (SetDrew_SelectRow(nRow) == TRUE)
			SetSendData(nRow);
	}
	else if (m_strType == "GROUP")
	{
		GetCell(0, nRow, &cell);
		CString strGroupName = cell.GetText();
		
		pwndParent->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_GROUP_CLK, (LPARAM)&strGroupName);
	}
}

void CGrid_Comm::OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed)
{
	if (m_strType == "REAL")
	{
		int nCodeCol = GetCodeColNumber(1);
		
		if ((row == -1) && (col == nCodeCol) && point->x<=10)
		{
		}
		else
		{
			return;
		}
	}
		
	if (updn && !processed)
	{		
		int nCodeCol = GetCodeColNumber(1);
		
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
		}
		
		if ((row == -1) && (updn == 1))
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
		//# Is Just String
		else
			nResult = cell1->m_string.Compare(cell2->m_string);
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
	if (m_strType == "LIST")
		return;
	
	if ((m_strType != "LIST") && SetDrew_SelectRow(newrow) == TRUE)
	{
		if (m_nLastSelRow > -1)
			SetSendData(newrow);
		
		/*RedrawAll();*/
		//Invalidate(FALSE);
	}
	else if (m_nLastSelRow > -1)
	{
		GetParent()->SendMessage(WM_NOTIFY_GRID, (WPARAM)NM_GRID_RESULT_CLK, (LPARAM)newrow);
	}
}

BOOL CGrid_Comm::SetDrew_SelectRow(int nRow)
{
	if (m_nLastSelRow == nRow)
		return FALSE;
	
	int nColCnt;
	COLORREF clrBk;
	CUGCell	cell;
	
	nColCnt = GetNumberCols();
	GetEvenOddBkColor(m_nLastSelRow, clrBk);
	for (int nCol = 0; nCol < nColCnt; nCol++)
	{
		if((m_strType == "RESULT"  || m_strType == "REAL") && (nCol == 0 || nCol == 1))
			continue;
		
		if (m_nLastSelRow != -1)
		{
			GetCell(nCol, m_nLastSelRow, &cell);
			cell.SetBackColor(clrBk);
			SetCell(nCol, m_nLastSelRow, &cell);
		}
		
		if (nRow != -1)
		{
			GetCell(nCol, nRow, &cell);
			cell.SetBackColor(m_clrSelRow);
			SetCell(nCol, nRow, &cell);
		}
	}
	
	m_nLastSelRow = nRow;
	
	return TRUE;
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
	//m_GI->m_hdgDefaults->SetBackColor(clrHeader);
	m_GI->m_hdgDefaults->SetBackColor(m_clrHeader);
}

int CGrid_Comm::OnHint(int col, long row, int section, CString *string)
{
	CUGCell cell;
	GetCell(0, row, &cell);
	
	if (m_strType == "RESULT")
	{
		if (col != 0)
			return FALSE;
		
		SetHint_Code(row);
		return FALSE;
	}
	else if (m_strType == "REAL")
	{
		if (col != 2)
			return FALSE;
		
		SetHint_Code(row);
		return FALSE;
	}
	else if (m_strType == "LIST")
	{
		GetCell(1, row, &cell);
		*string = cell.GetText();
	}
	else if (m_strType == "GROUP")
		*string = cell.GetText();
	
	return TRUE;
}

extern CString dll_GetStockDetailInfo(CString strCode, int nType);
void CGrid_Comm::SetHint_Code(int nRow)
{
	CString strTip;
	CUGCell cell;
	
	int nCol = 0;
	if (m_strType == "RESULT")
		nCol = 0;
	else if (m_strType == "REAL")
		nCol = 2;
	
	GetCell(nCol, nRow, &cell);

	if(!m_tipInfo)
		return;
	
	if (m_tipInfo.GetShowState() == FALSE)
	{
		CString strCode;
		strCode = cell.GetOtherJongCode();
		
		strCode.TrimRight(" ");
		if (strCode.IsEmpty() == TRUE)
			return;
//-->@080616-001 종목명에 갔을 때 종목코드가 툴팁으로 보이도록 수정.
//		int nMarket;
//		nMarket = cell.GetOtherDataInt();		
//		strTip = dll_GetStockDetailInfo(strCode, nMarket);
		strTip = strCode;
//<--
		
		m_tipInfo.UpdateToolTip(this, strTip, 1);
		m_tipInfo.Show(strTip);
	}
}

int CGrid_Comm::OnCanMove(int oldcol,long oldrow,int newcol,long newrow)
{
	if(m_strType == "LIST")
	{
		if(m_bSendTR) 
			return FALSE;
	}

	return TRUE;
}

/***************************************************
OnTopRowChange
	Sent whenever the top visible row in the grid changes
****************************************************/


void CGrid_Comm::OnAdjustComponentSizes(RECT *grid, 
										RECT *topHdg,RECT *sideHdg,
										RECT *cnrBtn,RECT *vScroll,
										RECT *hScroll,RECT *tabs)
{
	
	CRect  rtGrid;
	double dWidthGrid = 0.0;
	double dWidthCell = 0.0;
	
	int nCol, ntotWidth = 0;
	if (m_strType == "LIST") 
	{
		for (nCol = 0; nCol < _countof(HEADNAME_LIST); nCol++)
			ntotWidth += HEADWIDTH_LIST[nCol];
	}
	else if (m_strType == "GROUP")
		ntotWidth = 0;
	else if (m_strType == "INDUSTRY")
	{
		for (nCol = 0; nCol < _countof(HEADWIDTH_INDUSTRY); nCol++)
			ntotWidth += HEADWIDTH_INDUSTRY[nCol];
	}
	else if (m_strType == "REAL")
	{
// 		for (nCol = 0; nCol < _countof(HEADWIDTH_REALRESULT); nCol++)
// 			ntotWidth += HEADWIDTH_REALRESULT[nCol];
	}
	else if (m_strType == "RESULT")
	{
// 		int nOffsetIndex = 2;
// 		if(m_bCol_ForecastPrice_HideState)
// 			nOffsetIndex = 3;
// 		
// 		CMainFrmWnd* pWnd = (CMainFrmWnd*)GetParent();
// 		for (nCol = 0; nCol < nOffsetIndex + pWnd->m_listField.GetCount(); nCol++)
// 		{
// 			if((m_nCol_Hide == nCol) && !m_bCol_HideState)
// 				continue;
// 
// 			if(nCol < nOffsetIndex)
// 				ntotWidth += HEADWIDTH_BASE[nCol];
// 			else
// 			{
// 				POSITION pos;
// 				HeadInfoType stHeadInfoType;
// 				pos = pWnd->m_listField.FindIndex(nCol - nOffsetIndex);
// 				stHeadInfoType = pWnd->m_listField.GetAt(pos);
// 				ntotWidth += (int)(stHeadInfoType.nColWidth * (m_nFontWidth + 1));
// 			}
// 		}
	}
	
	if (!m_bCallAdjustOneTime)
	{
		rtGrid = CRect(grid);
		
		dWidthGrid  = rtGrid.Width() - 1 - ntotWidth;
		if (m_strType == "LIST")
		{
			SetColWidth(0, HEADWIDTH_LIST[0]);
			SetColWidth(1, rtGrid.Width() - HEADWIDTH_LIST[0] - 1);
		}
		else if (m_strType == "GROUP")
		{
			dWidthCell = dWidthGrid;
			SetColWidth(0, (int)dWidthCell);
		}
		else if (m_strType == "INDUSTRY")
		{
			dWidthCell = dWidthGrid / _countof(HEADWIDTH_INDUSTRY);
			ntotWidth = 0;
			for (nCol = 0; nCol < _countof(HEADWIDTH_INDUSTRY); nCol++)
			{
				if (nCol > 1)
				{
					SetColWidth(nCol, (int)(HEADWIDTH_INDUSTRY[nCol] + dWidthCell));
					ntotWidth += (int)(HEADWIDTH_INDUSTRY[nCol] + dWidthCell);
				}
			}
			SetColWidth(0, 17);
			SetColWidth(1, rtGrid.Width() - 22 - ntotWidth);
		}
		else if (m_strType == "REAL")
		{
// 			dWidthCell = dWidthGrid / _countof(HEADWIDTH_REALRESULT);
// 			ntotWidth = 0;
// 			for (nCol = 0; nCol < _countof(HEADWIDTH_REALRESULT); nCol++)
// 			{
// 				if (nCol > 0)
// 				{
// 					SetColWidth(nCol, (int)(HEADWIDTH_REALRESULT[nCol] + dWidthCell));
// 					ntotWidth += (int)(HEADWIDTH_REALRESULT[nCol] + dWidthCell);
// 				}
// 			}
// 			SetColWidth(0, rtGrid.Width() - 1 - ntotWidth);
		}
		else if (m_strType == "RESULT")
		{
// 			dWidthCell = dWidthGrid / _countof(HEADWIDTH_BASE);
// 			ntotWidth = 0;
// 			for (nCol = 0; nCol < _countof(HEADWIDTH_BASE); nCol++)
// 			{
// 				if (nCol > 0)
// 				{
// 					SetColWidth(nCol, (int)(HEADWIDTH_BASE[nCol] + dWidthCell));
// 					ntotWidth += (int)(HEADWIDTH_BASE[nCol] + dWidthCell);
// 				}
// 			}
// 			SetColWidth(0, rtGrid.Width() - 1 - ntotWidth);
		}
		
		m_bCallAdjustOneTime = TRUE;
		/*AdjustComponentSizes();*/
		m_bCallAdjustOneTime = FALSE;
	}
}

BOOL CGrid_Comm::GetRowToCode(int nRow, CString &strCode, int &nMarketType)
{
	BOOL bResult = FALSE;
	
	if (nRow < GetNumberRows())
	{
		int nCol = GetCodeColNumber();
		
		CUGCell cell;
		GetCell(nCol, nRow, &cell);

		strCode = cell.GetOtherJongCode();
		nMarketType = cell.GetOtherDataInt();
		
		bResult = TRUE;
	}
	else 
		bResult = FALSE;
	
	return bResult;
}

void CGrid_Comm::SetSelectJong(CStringArray *psaSelectJong)
{
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

void CGrid_Comm::SetSaveGridToExcel(CString strFileName)
{
	int nRow, nCol;
	CDrXLSFile xlsfile;

	CString strColName = "";
	CString strData = "";

	if (!xlsfile.OpenFile(strFileName))
	{
		CString strMsg = "지정한 파일이 다른 프로그램에서 사용중입니다";
		
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();

		if(pwndMainFrm->m_nIndustry == 0)
			SetMessageBox(strMsg, (CString)PROGRAM_TITLE, MB_OK);
		else if(pwndMainFrm->m_nIndustry == 1)
			SetMessageBox(strMsg, (CString)PROGRAM_TITLE_INDUSTRY, MB_OK);
		else
			SetMessageBox(strMsg, (CString)PROGRAM_TITLE_REAL, MB_OK);

		return;
	}
	else
	{
		nRow = -1;
		CUGCell cell;
		
		int nRate = -1;				// 등락률 컬럼 
		int nYesterdayRate = -1;	// 전일대비 컬럼
		for (nCol = 2; nCol < GetNumberCols(); nCol++)
		{
			GetCell(nCol, nRow, &cell);
			strData = cell.GetText();
			xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
			xlsfile.WriteLabel(0, nCol - 2, strData);
			if(strData == _T("전일대비"))
				nYesterdayRate = nCol;
			if(strData == _T("등락률"))
				nRate = nCol;
		}

		int nWriteRow=0;
		int nCodeCol = GetCodeColNumber(1);

		for (nRow = 0; nRow < GetNumberRows(); nRow++)
		{
			if(!IsRowCodeChecked(nRow)) continue;
			for (nCol = 2; nCol < GetNumberCols(); nCol++)
			{
				GetCell(nCol, nRow, &cell);
				if(nCodeCol==nCol) strData = cell.GetLabelText();
				else strData = cell.GetText();

				strData.Replace("&&", "&");

				strData.TrimRight(" ");
				strData.TrimLeft(" ");
				
				if(nYesterdayRate != -1 || nRate != -1)
				{
					if(nCol == nYesterdayRate)
					{
						COLORREF clrText, clrBack;
						GetCellColor(nRow, nCol, clrText, clrBack);

						if (clrText == COLOR_BLUE)
							strData = "-" + strData;
						else if (clrText == COLOR_RED)
							strData = "+" + strData;
					}
					else if(nCol == nRate)
					{
						COLORREF clrText, clrBack;
						GetCellColor(nRow, nCol, clrText, clrBack);

						if (clrText == COLOR_BLUE)
							strData = "-" + strData;
						else if (clrText == COLOR_RED)
							strData = "+" + strData;
					}
				}

				//if (strColName.Compare("종목코드") == 0)
				if (strColName.Find("종목코드") >= 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_CENTER);
				else if (strColName.Compare("종목명") == 0)
					xlsfile.SetTextAlign(EXCEL_ALIGN_LEFT);
				else
					xlsfile.SetTextAlign(EXCEL_ALIGN_RIGHT);

				xlsfile.WriteLabel(nWriteRow + 1, nCol - 2, (LPCSTR)strData);
			}
			nWriteRow++;
		}
	}
	xlsfile.CloseFile();
}

void CGrid_Comm::SetSaveGridToText(CString strFileName)
{
	CFile SaveFile(strFileName,CFile::modeCreate | CFile::modeWrite);
	
	int nRowCnt = GetNumberRows();
	
	CString strLineData, strData;
	int nRow, nCol;
	CUGCell cell;
	for (nRow = 0; nRow < nRowCnt; nRow++)
	{
		// 타이틀 저장용
		if (nRow == 0)
		{
			GetCell(nRow, nCol, &cell);
			strData = cell.GetText();
			strLineData = strData;	// 종목명
			strLineData += " ";
			
			for (nCol = 1; nCol < GetNumberCols(); nCol++)
			{
				GetCell(0, nCol, &cell);
				strData = cell.GetText();
				strLineData += strData;
				strLineData += " ";
			}
			
			strLineData += "\r\n";
			SaveFile.Write(strLineData, strLineData.GetLength());
		}
		
		if(!IsRowCodeChecked(nRow)) continue;
		
		GetCell(0, nCol, &cell);
		strData = cell.GetText();
		strLineData = strData;	// 종목명
		strLineData += " ";
		
		for (nCol = 1; nCol < GetNumberCols();  nCol++)
		{
			GetCell(nRow, nCol, &cell);
			strData = cell.GetText();
			
			strLineData += strData;	// 각 Item 정보
			strLineData += " ";
		}
		
		strLineData += "\r\n";
		SaveFile.Write(strLineData, strLineData.GetLength());
	}
	
	SaveFile.Close();
}

int CGrid_Comm::SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	return pwndMainFrm->SetMessageBox(lpszText, lpszCaption, nType);
}

int CGrid_Comm::GetFluctuating(CUIntArray *piaFluctuating, double &dAvgRate)
{
	CUGCell cell;
	
	int nTotCnt = GetNumberRows();
	int nAddRateVal = 0;
	for (int nIndex = 0; nIndex < 5; nIndex++)
		piaFluctuating->Add(0);
	
	for (int nRow = 0; nRow < nTotCnt; nRow++)
	{
		GetCell(2, nRow, &cell);
		int nSign = cell.GetSign();
		if ((nSign >= 1) && (nSign <= 5))
		{
			int nCnt = piaFluctuating->GetAt(nSign - 1);
			piaFluctuating->SetAt(nSign - 1, nCnt + 1);
		}
		
		GetCell(3, nRow, &cell);
		nAddRateVal = nAddRateVal + cell.GetOtherDataLong();
	}
	
	dAvgRate = (nAddRateVal / 100) / nTotCnt;
	return nTotCnt;
}

//<< 07.07.10 이준옥
void CGrid_Comm::FnLButtonDown(CPoint point)
{
	m_MouseMode = MOUSE_CODE_DRAG;

	if(::GetFocus()!=GetSafeHwnd())	SetFocus();

	ScreenToClient(&point);
	// ScreenToClient(&point);
	int col;

//	CUGCell cell;
//	GetCell(point.x, point.y,&cell);
//	if(cell.IsPropertySet(UGCELL_JOIN_SET))
//	{
//		RECT rect;
//		GetCellRect(&col, &m_lDragRow, &rect);
//	}
//	point.y -= GetTH_Height();		
	GetCellFromPoint(point.x, point.y, &col, &m_lDragRow);
	if(col!=m_nDragCol || m_lDragRow == 0) 
	{
		m_MouseMode = MOUSE_NOTHING;
		m_bDragAndDrop = FALSE;
		m_strDragCode = "";
		return;
	}
	m_lDragRow --;	// 실제보다 하나작게
	//TRACE("m_lDragRow=%ld\n", m_lDragRow);
}

//#include "../Builder/LinkInfoMng/KSLinkInfoMng.h"
extern long KSLink_GetEnvInfo(int nType);
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

/* hidden
			if ( lRet == 0 && !rcFrameRect.PtInRect( point) )
			{
				static DRAG_DATA_INFO DndDataInfo;
				CPoint DropPoint(point);
				ClientToScreen(&DropPoint);
				DndDataInfo.pt	= DropPoint;
				strcpy(DndDataInfo.szCode, (LPCTSTR)m_strDragCode);

				HWND		m_hRunWnd;
				m_hRunWnd = (HWND)KSLink_GetEnvInfo(0);
				::PostMessage( m_hRunWnd, UM_DND_NOTIFY, NULL, (LPARAM)&DndDataInfo);
				m_MouseMode = MOUSE_NOTHING;
				m_bDragAndDrop = FALSE;
				m_strDragCode = "";

				if ( m_bFocus )
				{
					m_bFocus	= FALSE;	
				}
				
				return;
			}
*/

/*
			if( static_cast<CJMComboCtl*>(GetParent()->GetParent())->pFormBase )
			{
				CRect rcFormRect;
				::GetClientRect( static_cast<CJMComboCtl*>(GetParent()->GetParent())->pFormBase->GetSafeHwnd(), &rcFormRect );
				if ( !rcFormRect.PtInRect( point) )
				{
					static DRAG_DATA_INFO DndDataInfo;
					CPoint DropPoint(point);
					ClientToScreen(&DropPoint);
					DndDataInfo.pt	= DropPoint;
					strcpy(DndDataInfo.szCode, (LPCTSTR)m_strDragCode);

					HWND		m_hRunWnd;
					m_hRunWnd = (HWND)KSLink_GetEnvInfo(0);
					::PostMessage( m_hRunWnd, UM_DND_NOTIFY, NULL, (LPARAM)&DndDataInfo);
				}
			}
*/
		}
	}

	m_MouseMode = MOUSE_NOTHING;
	m_bDragAndDrop = FALSE;
	m_strDragCode = "";

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
	strText.TrimRight();
	if(strText.GetLength()==0)	return FALSE;

	//CFont *pFont = (CFont*) GetFont(0);
	CFont *pFont = &m_fontDefault;
	if(!pFont)	return FALSE;
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

//>> 07.07.10 이준옥
void CGrid_Comm::FnRButtonUp(CPoint point)
{
//	SetCommMenu(&point);
}

void CGrid_Comm::DeleteOutItem()
{
	CUGCell cell;
	CString strState;
	for(int nRow = 0; nRow < GetNumberRows(); nRow++)
	{
		GetCell(0, nRow, &cell);
		strState = cell.GetText();
		if(strState.CompareNoCase("O") == 0)
		{
			DeleteRow(nRow);
			m_saCode.RemoveAt(nRow);
		}
		strState = "";
	}
	EvenOddColorSetting();

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

void CGrid_Comm::GetFieldList()
{
	m_saFieldList.RemoveAll();
	int nCount = _countof(HEADNAME_RESULT);

	for(int i = 0; i < nCount; i++)
	{
		m_saFieldList.Add(HEADNAME_RESULT[i]);
	}
}

void CGrid_Comm::OnBtnFieldEdit()
{
	SetFieldEdit();
}

void CGrid_Comm::SetFieldEdit()
{
	GetFieldList();

	if(m_pFieldEditDlg == NULL)
	{
		m_pFieldEditDlg = new CFieldEditDlg(this, 5);
	}

	if(m_pFieldEditDlg->DoModal() == IDOK)
	{
//		AfxMessageBox("1");
	}
}

//20100330 SJ_KIM 항목편집
void CGrid_Comm::SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField, CString szType)
{
	CMapStringToString UserColWidthsMap;
	CFindData_GridUtil::GetGridAllWidth(this, &UserColWidthsMap);
	
	CUGCell	cell;

	ClearAll();
	//ZeroMemory(&m_nSortType, sizeof(int) * RESULT_FIELD_MAX);

	int nColCnt = plistField->GetCount();
	int nCol = 0;
	
	CMainFrmWnd* pWnd = (CMainFrmWnd*)GetParent();
	if(szType == "RESULT" && pWnd->m_szScreenKey == "IB712800")
	{
		SetNumberCols(_countof(HEADNAME_BASE_REAL) + nColCnt, FALSE);
		nCol = _countof(HEADNAME_BASE_REAL);
	}
	else if(m_bCol_ForecastPrice_HideState || !szType.Compare("RESULT"))
	{
		SetNumberCols(_countof(HEADNAME_BASE) + nColCnt, FALSE);
		nCol = _countof(HEADNAME_BASE);
	}
	else
	{
		SetNumberCols(_countof(REAL_HEADNAME_BASE) + nColCnt, FALSE);
		nCol = _countof(REAL_HEADNAME_BASE);
	}

	m_nSortType[2] = 1;
	m_nSortType[3] = 1;

	POSITION		pos;
	HeadInfoType    stHeadType;
	m_nCol_Hide		= 0;
	m_bCol_HideState = FALSE;
	
	BOOL bExistBun = FALSE;
	int nColBun = 0;
	int nColSin = 0;
	BOOL bExistSin = FALSE;
	CString strHeadName;
	
	for (int nIndex = 0; nIndex < nColCnt; nIndex++)
	{
		pos = plistField->FindIndex(nIndex);
		stHeadType = plistField->GetAt(pos);

		// 예상체결탭외에도 예상체결 필드 보여주기
		if(!stHeadType.strHeadName.Compare("예상체결가") && !m_bCol_ForecastPrice_HideState)
		{
			m_bCol_HideState = TRUE;
			m_nCol_Hide = nIndex;
		}

		if(m_bCol_ForecastPrice_HideState && !(stHeadType.strHeadName.Compare("예상체결가")))
			m_nCol_Hide = nIndex; // 사용자가 선택한 예상체결가 필드 저장장소 -> 예상체결 탭에서만 동작

		TRACE("\n # Count      : %d", nCol);
		TRACE("\n   # HeadName : %s", stHeadType.strHeadName);
		TRACE("\n   # HeadID   : %d", stHeadType.lHeadID);

		if(stHeadType.strHeadName == "분")
		{
			bExistBun = TRUE;
			nColBun = nCol;
			stHeadType.strHeadName = "분HIDE";
			stHeadType.nColWidth = 0;
			strHeadName = "분HIDE";
		}
		else if(stHeadType.strHeadName == "신")
		{
			bExistSin = TRUE;
			nColSin = nCol;
			stHeadType.strHeadName = "신HIDE";
			stHeadType.nColWidth = 0;
			strHeadName = "신HIDE";
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
			(stHeadType.strDataType == "pattern") ||	//# 패턴
			(stHeadType.strDataType == "period")	// 기간
			)	
			m_nSortType[nCol] = 1;
		nCol++;
	}

	if(bExistBun)
	{
		strHeadName = "분HIDE";
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, "분", 0, 20);
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, strHeadName, nColBun, 0);
	}
	else
	{
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, "분", 0, 0);
	}

	if(bExistSin)
	{
		strHeadName = "신HIDE";
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, "신", 1, 20);
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, strHeadName, nColSin, 0);
	}
	else
	{
		CFindData_GridUtil::SetBunSinFieldWidth(this, &UserColWidthsMap, "신", 1, 0);
	}

	CFindData_GridUtil::SetGridAllWidth(this, &UserColWidthsMap);

	if(m_bCol_HideState)
		SetColWidth(4, 0);

	RedrawAll();
}

void CGrid_Comm::SetLockColumns(int nCol, BOOL bOffSet)
{
	if (bOffSet == TRUE)
		m_nFixCol = 2 + nCol;
	else
		m_nFixCol = nCol;

	LockColumns(m_nFixCol);
}

const   int       MAX_COL = 30;
#include "math.h"
#define COL_BUN		0
#define COL_SIN		1
#define COL_CODE    2
#define COL_NAME    3
#define COL_FORECAST_PRICE  4
#define REAL_COL_NAME		4
void CGrid_Comm::SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
							 CString strItemName, char dcolData[960], int nPrice, bool bRedraw)
{
	CString strStatus;
	CStringArray arrData;
	COLORREF clrBunText;
	COLORREF clrBunBack;

	strStatus = GetStockStatus(strItemCode, strItemName, clrBunText, clrBunBack);
	
	int nCol;
	CString strData[MAX_COL];
//	CString strTemp;
	char szTemp[17];
	double dTemp;
	CUGCell cellItemDel, cellItemName, cellItemCode, cellForecastPrice, cellData[MAX_COL];
	CUGCell cellItemBun, cellItemSin;
	
	int nRowIndex = nRow;
	COLORREF clrBk;
	GetEvenOddBkColor(nRowIndex, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;

	char szItemCode[7];
	memset(szItemCode, 0x00, sizeof(szItemCode));
	memcpy(szItemCode, strItemCode, 6);	

	int nItemNameIndex;
	for(nItemNameIndex = 0; ; nItemNameIndex++)
	{
		char ch = strItemName.GetAt(nItemNameIndex);
		char chNext = strItemName.GetAt(nItemNameIndex + 1);
		if( ch == _T(' ') && chNext == _T(' '))
			break;
	}

	char szItemName[256];
	memset(szItemName, 0x00, sizeof(szItemName));
	memcpy(szItemName, strItemName, nItemNameIndex);
	
	strItemName.Format("%s", szItemName);
	
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
				if(m_nIndustry == 1)
				{
					TempHeadType.strDataType = "double";
					nUnitNum = 100;
				}
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
				if(chSign == '4' || chSign == '5')
					strSign = "-";
				else
					strSign = " ";		
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
				if(m_nIndustry == 1 && !((TempHeadType.strHeadName).Compare("현재가")))
				{
					TempHeadType.strDataType = "double";
					nUnitNum = 100;
				}
				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 <<
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
	GetCell((int)COL_BUN, nRowIndex, &cellItemBun);
	GetCell((int)COL_SIN, nRowIndex, &cellItemSin);
	GetCell((int)COL_CODE, nRowIndex, &cellItemCode);
	GetCell((int)COL_NAME, nRowIndex, &cellItemName);
	GetCell((int)COL_FORECAST_PRICE, nRowIndex, &cellForecastPrice);

	int nCol_Offset;
	/*nCol_Offset = 5;*/

	CMainFrmWnd* pWnd = (CMainFrmWnd*)GetParent();
	if(pWnd->m_szScreenKey == "IB712800")
		nCol_Offset = 4;
	else
	{
		nCol_Offset = 5;

		if(m_bCol_ForecastPrice_HideState)	// 예상체결가 탭
		{
			SetColWidth(4, HEADWIDTH_BASE[4]);
		}
		else
		{
			SetColWidth(4, 0);
		}
	}
		
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);
	
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
	cellItemSin.SetReadOnly(TRUE);
	
	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
	cellItemCode.SetFont(&m_fontDefault);
	cellItemCode.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemCode.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
	cellItemCode.SetCellType(UGCT_CHECKBOX);
	cellItemCode.SetCellTypeEx(UGCT_CHECKBOXCHECKMARK);
	cellItemCode.SetText("1");
	cellItemCode.SetLabelText(szItemCode);
	cellItemCode.SetBackColor(clrBk);
	
	cellItemName.SetFont(&m_fontDefault);
	cellItemName.SetText(strItemName);
	cellItemName.SetBackColor(clrBk);
	cellItemName.m_string = strItemName;
	
	cellItemName.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherJongCode(szItemCode);
	cellItemName.SetOtherDataInt(atoi(&chMarkType));
	cellItemName.SetReadOnly(TRUE);

	cellForecastPrice.SetFont(&m_fontDefault);
	cellForecastPrice.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellForecastPrice.SetAlignment(UG_ALIGNRIGHT | UG_ALIGNVCENTER);
	cellForecastPrice.SetText(szItemCode);
	cellForecastPrice.SetBackColor(clrBk);
	
	COLORREF clrSignText;
	nRealDataIndex = 0;
	nArrIndex = 0;
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		TempHeadType = m_plistField->GetAt(pos);

		nArrIndex = nCol * 16;
		
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
//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

			memset(szTemp, NULL, sizeof(szTemp));
			memcpy(szTemp, &dcolData[nArrIndex], 16);
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
			memcpy(szTemp, &dcolData[nArrIndex], 16);
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
		else if(TempHeadType.lHeadID == 9072 || TempHeadType.lHeadID == 9073) // 일봉, L일봉H
		{
			CString strCandleData;
			CString strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 전일종가
			memcpy(szTemp, &dcolData[56 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 시
			memcpy(szTemp, &dcolData[57 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 고
			memcpy(szTemp, &dcolData[58 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 저
			memcpy(szTemp, &dcolData[59 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			strTemp.Format("%d|", nPrice);			// 종(현재가)
			strCandleData += strTemp;

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
			
			CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)GetParent();
			CString strMarketName = pwndMainFrm->GetMarketNameToCode(strUpjongCode);
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
				
// 				if(pMainInfo)
// 					strNote = pMainInfo->GetNoteText((LPCSTR)strItemCode);				
				
				
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

//			strTemp.Format("%16.16s", dcolData[nRealDataIndex]);

			memset(szTemp, NULL, sizeof(szTemp));
			memcpy(szTemp, &dcolData[nArrIndex], 16);
			/*memcpy(szTemp, dcolData[nRealDataIndex], 16);*/

			if(TempHeadType.strDataType == "debi" || TempHeadType.strDataType == "debiuel")
			{
				if(chSign == '4' || chSign == '5')
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * -1000.0));
				else
					cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 1000.0));
			}
			else if (TempHeadType.strDataType == "double" && TempHeadType.strUnitText == "%")
			{
				cellData[nCol + nCol_Offset].SetOtherDataLong((long)(atof(szTemp) * 1000.0));
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
	

	cellItemCode.SetBorderColor(&m_penGridLine);
	cellItemCode.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_BUN, nRowIndex, &cellItemBun);

	cellItemCode.SetBorderColor(&m_penGridLine);
	cellItemCode.SetBorder(UG_BDR_RTHIN);
	SetCell((int)COL_SIN, nRowIndex, &cellItemSin);
	
	CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)GetParent();
	if(pwndMainFrm->m_nFixCol == 1)
		cellItemCode.SetBorder(1);
	else
		cellItemCode.SetBorder(UG_BDR_RTHIN);
		
	cellItemCode.SetBackColor(clrBk);
	cellItemCode.SetBorderColor(&m_penGridLine);
	/*cellItemCode.SetBorder(UG_BDR_RTHIN);*/
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

	
	if(m_bCol_ForecastPrice_HideState || m_bCol_HideState)
	{
		if(m_nCol_Hide && m_bCol_ForecastPrice_HideState)
			SetColWidth(m_nCol_Hide + nCol_Offset, 0);
		else if(m_bCol_HideState)
			SetColWidth(m_nCol_Hide + nCol_Offset, HEADWIDTH_BASE[4]);

		memset(szTemp, 0x00, sizeof(szTemp));
		memcpy(szTemp, &dcolData[nArrIndex + 16], 16);
		dTemp = atof(szTemp);
		CString szData = GetFormatStr(fabs(dTemp), 0, FALSE, "");
		cellForecastPrice.SetText(szData);
		
		cellForecastPrice.SetBackColor(clrBk);
		cellForecastPrice.SetBorderColor(&m_penGridLine);
		cellForecastPrice.SetBorder(UG_BDR_RTHIN);	
		cellForecastPrice.SetReadOnly(TRUE);
		if(m_bCol_ForecastPrice_HideState)
			SetCell((int)COL_FORECAST_PRICE, nRowIndex, &cellForecastPrice);
		else if(m_bCol_HideState)
			SetCell(m_nCol_Hide + nCol_Offset, nRowIndex, &cellData[m_nCol_Hide + nCol_Offset]);
	}
	
	EnableWindow(TRUE);
// 	if (bRedraw == true) 
// 		RedrawAll();
}

void CGrid_Comm::SetAddRow_Alert_DynamicField(int nRow, int ntotCol, char dcolData[960], CString strItemName, 
											  char chMarkType, char chSign, CString szItemCode, BOOL bRedraw, int nPrice)
{
	CString strStatus;
	CStringArray arrData;
	COLORREF clrBunText;
	COLORREF clrBunBack;

	/*strItemName.TrimLeft();*/
	strItemName.TrimRight();

	strStatus = GetStockStatus(szItemCode, strItemName, clrBunText, clrBunBack);
	
	CUGCell cell;
	CUGCell cellItemBun, cellItemSin;
	COLORREF clrBk;

	int nCol;
	CString strTemp;

	GetEvenOddBkColor(nRow, clrBk);

	nCol = 0;

	GetCell(nCol, nRow, &cellItemBun);

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
	
	SetCell((int)COL_BUN, nRow, &cellItemBun);
	nCol++;
	
	GetCell(nCol, nRow, &cellItemSin);

	cellItemSin.SetFont(&m_fontDefault);
	cellItemSin.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemSin.SetAlignment(UG_ALIGNCENTER | UG_ALIGNVCENTER);
// 	cellItemSin.SetText(strStatus);
// 	cellItemSin.SetBackColor(clrBunBack);	
// 	cellItemSin.SetTextColor(clrBunText);
	cellItemSin.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cellItemSin);
	nCol++;

	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	cell.SetText(" ");

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);

	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	cell.SetText(" ");

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);
		
	CString strData[MAX_COL];
	char szTemp[17];
	double dTemp;
	CUGCell cellItemDel, cellItemName, cellData[MAX_COL];
	
	int nRowIndex = nRow;
	GetEvenOddBkColor(nRowIndex, clrBk);
	
	POSITION pos;
	HeadInfoType    TempHeadType;	
	
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
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = (dTemp > 0) ? "+" : "-";
				strSign = (dTemp == 0) ? " " : strSign;
				
				//////////////////////////////////////////////////////////////
				// 20100414 박찬모 업종검색에서 전일대비 소수점 이하 2자리 >>
				if(m_nIndustry == 1)
				{
					TempHeadType.strDataType = "double";
					nUnitNum = 100;
				}
				// 20100414 박찬모 업종검색에서 전일대비 소수점 이하 2자리 <<
				//////////////////////////////////////////////////////////////
				
				strData[nRealDataIndex] = /*strSign +*/ GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "debiuel")  //## 대비율 ##
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = _T("");
				if(chSign == '4' || chSign == '5')
					strSign = "-";
				else 
					strSign = " ";				

				strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "date")  //## 날짜 ##
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
				nRealDataIndex++;
			}
			else if (TempHeadType.strDataType == "time")  //## 시간 ##
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				strData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
				nRealDataIndex++;
			}
			else		//# int, double
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &dcolData[nArrIndex], 16);
				dTemp = atof(szTemp);

				CString strSign = (dTemp > 0)?' ':'-';
				strSign = (dTemp == 0) ? " " : strSign;
				
				/////////////////////////////////////////////////////////////////////////////
				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 >> 
				if(m_nIndustry == 1 && !((TempHeadType.strHeadName).Compare("현재가")))
				{
					TempHeadType.strDataType = "double";
					nUnitNum = 100;
				}
				// 20100414 박찬모 업종검색 현재가 소수점 이하 2자리 <<
				/////////////////////////////////////////////////////////////////////////////
				
				if (TempHeadType.strDataType == "int")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");
				if (TempHeadType.strDataType == "double")
					strData[nRealDataIndex] = strSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				
				nRealDataIndex++;
			}
		}
	}
	
	GetCell((int)REAL_COL_NAME, nRowIndex, &cellItemName);
	
	int nCol_Offset;
	nCol_Offset = 5;
	
	for (nCol = 0; nCol < ntotCol; nCol++)
		GetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);

	cellItemName.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cellItemName.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	
	cellItemName.SetFont(&m_fontDefault);
	cellItemName.SetText(strItemName);
	cellItemName.SetBackColor(clrBk);
	cellItemName.m_string = strItemName;
	
	cellItemName.SetOtherDataCellText(strItemName);
	cellItemName.SetOtherDataInt(atoi(&chMarkType));
	
	szItemCode = szItemCode.Left(6);
	cellItemName.SetOtherJongCode(szItemCode);
	m_saCode.Add(szItemCode);
	
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

		cellData[nCol + nCol_Offset].SetReadOnly(TRUE);
		
		if (TempHeadType.strDataType == "code")  //## CODE ##
		{
		}
		else if (TempHeadType.strDataType == "candle")  //# 캔들 
		{
			memset(szTemp, NULL, sizeof(szTemp));

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
			memset(szTemp, NULL, sizeof(szTemp));

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
		else if(TempHeadType.lHeadID == 9072 || TempHeadType.lHeadID == 9073)
		{
			CString strCandleData;
			CString strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 전일종가
			memcpy(szTemp, &dcolData[56 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 시
			memcpy(szTemp, &dcolData[57 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 고
			memcpy(szTemp, &dcolData[58 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 저
			memcpy(szTemp, &dcolData[59 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			strTemp.Format("%d|", nPrice);			// 종(현재가)
			strCandleData += strTemp;

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
			cellData[nCol + nCol_Offset].SetOtherJongCode(szItemCode);

			nRealDataIndex++;
		}
		else if(TempHeadType.lHeadID == 9033) // 업종구분
		{
			CString strUpjongCode;
			strUpjongCode.Format("%3d", _ttoi(strData[nRealDataIndex]));
			strUpjongCode.Replace(' ', '0');
			
			CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)GetParent();
			CString strMarketName = pwndMainFrm->GetMarketNameToCode(strUpjongCode);
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
				
// 				IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
// 				if(pMainInfo)
// 					strNote = pMainInfo->GetNoteText((LPCSTR)szItemCode);				
				
				cellData[nCol + nCol_Offset].SetText(strNote);
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
				cellData[nCol + nCol_Offset].SetReadOnly(FALSE);
				cellData[nCol + nCol_Offset].SetOtherJongCode(szItemCode);
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

			memset(szTemp, NULL, sizeof(szTemp));

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
	
	cellItemName.SetBackColor(clrBk);
	cellItemName.SetBorderColor(&m_penGridLine);
	cellItemName.SetBorder(UG_BDR_RTHIN);
	cellItemName.SetReadOnly(TRUE);

	SetCell((int)REAL_COL_NAME, nRowIndex, &cellItemName);
	
	for (nCol = 0; nCol < ntotCol; nCol++)
	{
		cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
		cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
		SetCell(nCol + nCol_Offset, nRowIndex, &cellData[nCol + nCol_Offset]);
	}
	
	if (bRedraw == TRUE) 
		RedrawAll();
	EnableWindow(TRUE);


}

void CGrid_Comm::SetAddRow_Real_DynamicField(SIGNAL_CONDITION stSIGNAL_CONDITION, BOOL bJobChk, BOOL bRedraw, int ntotCol)
{
	CString strStatus;
	CStringArray arrData;
	COLORREF clrBunText;
	COLORREF clrBunBack;

	char *pchData;
	CString strData, strCode, strName;

	pchData = new char[sizeof(stSIGNAL_CONDITION.sCode) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sCode) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sCode, sizeof(stSIGNAL_CONDITION.sCode));
	strCode = (CString)pchData;
	delete [] pchData;

	pchData = new char[sizeof(stSIGNAL_CONDITION.sName) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sName) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sName, sizeof(stSIGNAL_CONDITION.sName));
	strData = (CString)pchData;
	strData.TrimLeft(" ");
	strData.TrimRight(" ");
	strName = strData;
	delete [] pchData;


	strStatus = GetStockStatus(strCode, strName, clrBunText, clrBunBack);
	
	CUGCell cell;
	CUGCell cellItemBun, cellItemSin;
	COLORREF clrBk;

	int nRow, nCol;
	CString strRow;

	pchData = new char[sizeof(stSIGNAL_CONDITION.sJobFlag) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sJobFlag) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag));

	BOOL	bSaved = FALSE;
	CString strSavedCode;
	int nIndex = 0;
	for(nIndex=0; nIndex < m_saCode.GetSize(); nIndex++)
	{
		strSavedCode = m_saCode.GetAt(nIndex);
		if(strSavedCode.CompareNoCase(strCode) == 0)
		{
			bSaved = TRUE;
			break;
		}
	}

	if( bSaved == TRUE)
	{
		nRow = nIndex;
		GetEvenOddBkColor(nRow, clrBk);

		if(m_bChkDelOut == TRUE && strcmp(pchData, "O") == 0)
		{
			m_saCode.RemoveAt(nIndex);
			DeleteRow(nIndex);
			m_nLastSelRow--;
			return;
		}
	}
	else
	{
		if(bJobChk == TRUE)
		{
			if(strcmp(pchData, "O") == 0)
				return;			
		}

		if(InsertRow(0) == UG_SUCCESS)
		{
			m_saCode.InsertAt(0, strCode);
			nRow = 0;
			GetEvenOddBkColor(GetNumberRows() - 1, clrBk);
			m_nLastSelRow++;
		}
	}

	nCol = 0;
	
	GetCell((int)COL_BUN, nRow, &cellItemBun);
	GetCell((int)COL_SIN, nRow, &cellItemSin);
	
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
// 	cellItemSin.SetText(strStatus);
// 	cellItemSin.SetBackColor(clrBunBack);	
// 	cellItemSin.SetTextColor(clrBunText);
	cellItemSin.SetReadOnly(TRUE);
	
	SetCell(nCol, nRow, &cellItemBun);
	nCol++;
	SetCell(nCol, nRow, &cellItemSin);
	nCol++;
	
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	pchData = new char[sizeof(stSIGNAL_CONDITION.sJobFlag) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sJobFlag) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sJobFlag, sizeof(stSIGNAL_CONDITION.sJobFlag));
	strData = (CString)pchData;
	delete [] pchData;


	//# 신규:'N', 재진입:'R', 탈퇴:'O'
	if (m_imgList.GetImageCount() > 0)
	{
		if (strData == "N")
			cell.SetImageList(0, &m_imgList);
		else if (strData == "R")
			cell.SetImageList(1, &m_imgList);
		else if (strData == "O")
			cell.SetImageList(2, &m_imgList);
	}
	else
		cell.SetText(strData);

	cell.SetText(strData);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);

	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);

	pchData = new char[sizeof(stSIGNAL_CONDITION.sTime) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.sTime) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.sTime, sizeof(stSIGNAL_CONDITION.sTime));
	strData = GetFormatStr(atof(pchData), 0, TRUE, "XX:XX:XX");
	cell.SetText(strData);
	delete [] pchData;

	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);

	nCol++;
	GetCell(nCol, nRow, &cell);
	cell.SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
	cell.SetAlignment(UG_ALIGNLEFT | UG_ALIGNVCENTER);
	cell.SetFont(&m_fontDefault);
	cell.SetText(strName);

	//# Code 
	cell.SetOtherJongCode(strCode);

	//# Market
	pchData = new char[sizeof(stSIGNAL_CONDITION.stCommon.sMarket) + 1];
	memset(pchData, 0x00, sizeof(stSIGNAL_CONDITION.stCommon.sMarket) + 1);
	memcpy(pchData, stSIGNAL_CONDITION.stCommon.sMarket, sizeof(stSIGNAL_CONDITION.stCommon.sMarket));
	cell.SetOtherDataInt(atoi(pchData));
	delete [] pchData;

	cell.SetOtherDataCellText(strCode);
	cell.SetBorderColor(&m_penGridLine);
	cell.SetBorder(UG_BDR_RTHIN);
	cell.SetBackColor(clrBk);
	cell.SetReadOnly(TRUE);
	SetCell(nCol, nRow, &cell);


	CString szData[MAX_COL];
	char szTemp[17] = {0,};
	double dTemp;
	CUGCell cellItemDel, cellItemName, cellData[MAX_COL];

	POSITION pos;
	HeadInfoType stHeadInfoType;

	int nRealDataIndex = 0;
	int nArrIndex = 0;

	CList<HeadInfoType, HeadInfoType&> *m_plistFixHeadSys;
	m_plistFixHeadSys = &((CMainFrmWnd*)GetParent())->m_listFixHeadSys;

	if(m_plistField)
	{
		for(nCol = 0; nCol < ntotCol; nCol++)
		{
			pos = m_plistField->FindIndex(nCol);
			stHeadInfoType = m_plistField->GetAt(pos);

			POSITION _pos;
			HeadInfoType _stHeadInfpType;
			int nSeverDataIndex = 0; // 서버에서 내려온 값을 뽑아내기 위한 인덱스
			for(nSeverDataIndex = 0; nSeverDataIndex < m_plistFixHeadSys->GetCount(); nSeverDataIndex++)
			{
				_pos = m_plistFixHeadSys->FindIndex(nSeverDataIndex);
				_stHeadInfpType = m_plistFixHeadSys->GetAt(_pos);
				if(stHeadInfoType.strHeadName == _stHeadInfpType.strHeadName)
					break;
			}
			nSeverDataIndex *= 16;

			// data setting
			int nDataLength = stHeadInfoType.nDataLength;
			int	nUnitNum = stHeadInfoType.nUnitNum;

			if(stHeadInfoType.strDataType == _T("code"))
				continue;

			else if(stHeadInfoType.strDataType == _T("candle"))
			{
				nRealDataIndex++;
				continue;
			}

			else if(stHeadInfoType.strDataType == _T("pattern"))
			{
				nRealDataIndex++;
				continue;
			}

			else if(stHeadInfoType.strDataType == _T("debi"))
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
				dTemp = atof(szTemp);

				CString szSign = (dTemp > 0) ? "+" : "-";
				szSign = (dTemp == 0) ? " " : szSign;

				if(m_nIndustry == 1)
				{
					stHeadInfoType.strDataType = "double";
					nUnitNum = 100;
				}

				szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				nRealDataIndex++;
			}

			else if(stHeadInfoType.strDataType == _T("debiuel"))
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
				dTemp = atof(szTemp);

				CString szSign = _T("");
				szSign = (dTemp == 0) ? " " : szSign;
				szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");
				nRealDataIndex++;
			}

			else if(stHeadInfoType.strDataType == _T("date"))
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
				dTemp = atof(szTemp);

				szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XXXX/XX/XX");
				nRealDataIndex++;
			}

			else if(stHeadInfoType.strDataType == _T("time"))
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
				dTemp = atof(szTemp);

				szData[nRealDataIndex] = GetFormatStr(fabs(dTemp), 0, TRUE, "XX:XX:XX");
				nRealDataIndex++;
			}

			else
			{
				memset(szTemp, NULL, sizeof(szTemp));
				memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[nSeverDataIndex], 16);
				dTemp = atof(szTemp);

				CString szSign = (dTemp > 0) ? " " : "-";
				szSign = (dTemp == 0) ? " " : szSign;

				if(m_nIndustry == 1 && !((stHeadInfoType.strHeadName).Compare("현재가")))
				{
					stHeadInfoType.strDataType = "double";
					nUnitNum = 100;
				}

				if(stHeadInfoType.strDataType == "int")
					szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), 0, FALSE, "");
				else if(stHeadInfoType.strDataType == "double")
					szData[nRealDataIndex] = szSign + GetFormatStr(fabs(dTemp), nUnitNum, FALSE, "");

				nRealDataIndex++;
			}
		}
	}

	int nCol_Offset = 5;
	COLORREF clrSignText;
	nRealDataIndex = 0;
	for(nCol = 0; nCol < ntotCol; nCol++)
	{
		pos = m_plistField->FindIndex(nCol);
		stHeadInfoType = m_plistField->GetAt(pos);

		if(stHeadInfoType.strHeadName == "char")
			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_STRING | UGCELLDATA_OTHER);
		else if ((stHeadInfoType.strDataType == "double") || 
			(stHeadInfoType.strDataType == "int"))
			cellData[nCol + nCol_Offset].SetDataType(UGCELLDATA_NUMBER | UGCELLDATA_OTHER);

		if (stHeadInfoType.strAlignment == "center")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
		else if (stHeadInfoType.strAlignment == "left")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
		else if (stHeadInfoType.strAlignment == "right")
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNRIGHT);

		cellData[nCol + nCol_Offset].SetReadOnly(TRUE);

		if (stHeadInfoType.strDataType == "code")  //## CODE ##
		{
		}
		else if (stHeadInfoType.strDataType == "candle")  //# 캔들 
		{
			memset(szTemp, NULL, sizeof(szTemp));

			szData[nRealDataIndex] = FindCandleIdFromName((long)atol(szTemp));

			if (!szData[nRealDataIndex].IsEmpty())
			{
				szData[nRealDataIndex].TrimRight();
				szData[nRealDataIndex].TrimLeft();
			}

			cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = szData[nRealDataIndex];

			cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
			nRealDataIndex++;
		}
		else if (stHeadInfoType.strDataType == "pattern")  //# 패턴
		{
			memset(szTemp, NULL, sizeof(szTemp));

			szData[nRealDataIndex] = FindPatenIdFromName(atol(szTemp));

			if (!szData[nRealDataIndex].IsEmpty())
			{
				szData[nRealDataIndex].TrimRight();
				szData[nRealDataIndex].TrimLeft();
			}

			cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].m_string = szData[nRealDataIndex];

			cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
			cellData[nCol + nCol_Offset].SetOtherDataLong(atol(szTemp));
			nRealDataIndex++;
		}
		else if(stHeadInfoType.lHeadID == 9072 || stHeadInfoType.lHeadID == 9073) // 일봉, L일봉H
		{
			CString strCandleData;
			CString strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 전일종가
			memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[8 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 시
			memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[5 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 고
			memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[6 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 저
			memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[7 * 16], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			memset(szTemp, NULL, sizeof(szTemp));	// 종(현재가)
			memcpy(szTemp, &stSIGNAL_CONDITION.stCommon.sResultField[0], 16);
			strTemp.Format("%s|", szTemp);
			strCandleData += strTemp;

			if(stHeadInfoType.lHeadID == 9072)
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
		else if(stHeadInfoType.lHeadID == 8003) // 삭제
		{
			m_nButtonIndex = AddCellType(&m_ugButtonType);
			cellData[nCol + nCol_Offset].SetText(_T("X"));
			cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNCENTER);
			cellData[nCol + nCol_Offset].SetCellType(m_nButtonIndex);
			cellData[nCol + nCol_Offset].SetCellTypeEx(UGCT_BUTTONNOFOCUS);
			cellData[nCol + nCol_Offset].SetBackColor(GetSysColor(COLOR_BTNFACE));
			cellData[nCol + nCol_Offset].SetOtherJongCode(strCode);

			nRealDataIndex++;
		}
		else if(stHeadInfoType.lHeadID == 9033) // 업종구분
		{
			CString strUpjongCode;
			strUpjongCode.Format("%3d", _ttoi(szData[nRealDataIndex]));
			strUpjongCode.Replace(' ', '0');
			
			CMainFrmWnd* pwndMainFrm = (CMainFrmWnd*)GetParent();
			CString strMarketName = pwndMainFrm->GetMarketNameToCode(strUpjongCode);
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
			if (stHeadInfoType.strUnitText.IsEmpty() == FALSE)
				szData[nRealDataIndex] += " " + stHeadInfoType.strUnitText;  //<= 단위
			
			if (!szData[nRealDataIndex].IsEmpty())
			{
				szData[nRealDataIndex].TrimRight();
				szData[nRealDataIndex].TrimLeft();
			}

			CString strNote;
			if (stHeadInfoType.lHeadID == 8002) // 메모
			{
				
// 				IMainInfoManager* pMainInfo = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager, 1);
// 				if(pMainInfo)
// 					strNote = pMainInfo->GetNoteText((LPCSTR)strCode);				
				
				cellData[nCol + nCol_Offset].SetText(strNote);
				cellData[nCol + nCol_Offset].SetAlignment(UG_ALIGNVCENTER | UG_ALIGNLEFT);
				cellData[nCol + nCol_Offset].SetReadOnly(FALSE);
				cellData[nCol + nCol_Offset].SetOtherJongCode(strCode);
				cellData[nCol + nCol_Offset].SetOtherDataCellText(strName);
			}
			else 
			{
				if(atof(szData[nRealDataIndex]) == 0)
					cellData[nCol + nCol_Offset].SetText("");
				else
					cellData[nCol + nCol_Offset].SetText(szData[nRealDataIndex]);

				cellData[nCol + nCol_Offset].SetOtherDataCellText(szData[nRealDataIndex]);
			}

			memset(szTemp, NULL, sizeof(szTemp));

			char chSign = stSIGNAL_CONDITION.stCommon.sSign;
			if(stHeadInfoType.strDataType == "debi" || stHeadInfoType.strDataType == "debiuel")
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
			if (stHeadInfoType.strDataType == "debi")    //## 대비 ##
			{
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
				cellData[nCol + nCol_Offset].SetSign(nSign);
			}
			else if (stHeadInfoType.strDataType == "debiuel")   //## 대비율 ##
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);
			else if(stHeadInfoType.strHeadName == "현재가")
				cellData[nCol + nCol_Offset].SetTextColor(clrSignText);

			nRealDataIndex++;
		}
		cellData[nCol + nCol_Offset].SetFont(&m_fontDefault);
	}

	for(nCol = 0; nCol < ntotCol; nCol++)
	{
		cellData[nCol + nCol_Offset].SetBorderColor(&m_penGridLine);
		cellData[nCol + nCol_Offset].SetBorder(UG_BDR_RTHIN);
		cellData[nCol + nCol_Offset].SetBackColor(clrBk);

		SetCell(nCol + nCol_Offset, nRow, &cellData[nCol + nCol_Offset]);
	}

	RedrawAll();
}

CString CGrid_Comm::FindCandleIdFromName(long lCandleID)
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

BOOL CGrid_Comm::LoadCandleIndexToXml()
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
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	CString strAppPath;
	strAppPath = pwndMainFrm->Variant(homeCC);
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

CString CGrid_Comm::FindPatenIdFromName(long lPatenID)
{
	CString strResult = "--";

	int i, nCount;
	POSITION pos;

	PatenInfo      TempPatenInfo;

	if (m_bPatenLoadFalg == FALSE)
		return strResult;

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

BOOL CGrid_Comm::LoadPatenIndexToXml()
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
	CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
	CString strAppPath = pwndMainFrm->Variant(homeCC);
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

void CGrid_Comm::SetHideCol(int nCol, BOOL bView)
{
	m_bCol_HideState = bView;
	m_nCol_Hide = nCol;

	AdjustComponentSizes();
}

void CGrid_Comm::SetVisibleForecastPriceCol(BOOL bView)
{
	m_bCol_ForecastPrice_HideState = bView;
}

void CGrid_Comm::GetCellColor(int nRow, int nCol, COLORREF &clrText, COLORREF &clrBack)
{
	CUGCell cell;
	GetCell(nCol, nRow, &cell);

	clrText = cell.GetTextColor();
	clrBack = cell.GetBackColor();
}

int CGrid_Comm::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if(ID == m_nButtonIndex)	
	{
		if(msg == UGCT_BUTTONUP)
		{
			CUGCell cell;
			GetCell(col, row, &cell);

// 			CConditionResultWnd* pReWnd = (CConditionResultWnd*)GetParent();
// 			pReWnd->m_saDelItemCode.Add(cell.GetOtherJongCode());
			DeleteRow(row);
		}
	}

	return TRUE;
}

void CGrid_Comm::SetSignalAtGrid(CString strCode, CString strSignal, COLORREF clrBK, COLORREF clrText)
{
	CUGCell cellSignal;
	CUGCell cellTemp;
	int nRow = -1;
	for(int i = 0; i < GetNumberRows(); i++)
	{
		GetCell(4, i, &cellTemp);
		if(strCode == cellTemp.GetOtherJongCode())
		{
			nRow = i;
			break;
		}
	}

	if(nRow != -1)
	{
		GetCell(COL_SIN, nRow, &cellSignal);
		cellSignal.SetText(strSignal);
		cellSignal.SetBackColor(clrBK);
		cellSignal.SetTextColor(clrText);
		SetCell(1, nRow, &cellSignal);
	}
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

BOOL CGrid_Comm::IsRowCodeChecked(int nRow)
{
	BOOL bResult = FALSE;
	CString strTemp;
	
	if (nRow < GetNumberRows())
	{
		CUGCell cell;
		if ((m_strType == "RESULT") || (m_strType == "INDUSTRY"))
		{
			//GetCell(3, nRow, &cell);
			QuickGetText(2, nRow, &strTemp);
		}
		else if (m_strType == "REAL")
		{
			CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
			if(pwndMainFrm->GetRealFlag())
			{
				//GetCell(4, nRow, &cell);
				QuickGetText(3, nRow, &strTemp);
			}
			else
			{
				//GetCell(3, nRow, &cell);
				QuickGetText(2, nRow, &strTemp);
			}
		}
		bResult = TRUE;
	}
	else 
		bResult = FALSE;
	
	if(!bResult) return bResult;
	
	return (BOOL)atoi(strTemp);
}

int CGrid_Comm::GetCodeColNumber(int nType)
{
	if(nType==1) return 2;

	if ((m_strType == "RESULT") || (m_strType == "INDUSTRY"))
		return 3;
	else if (m_strType == "REAL")
	{
		CMainFrmWnd *pwndMainFrm = (CMainFrmWnd*)GetParent();
		if(pwndMainFrm->GetRealFlag())
			return 4;
		else
			return 3;
	}
	
	return 0;
}

void CGrid_Comm::GetRowCode(int nRow, CString &strCode, int &nMarkType)
{
	CUGCell cell;
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
	strResult = cell.GetText();
	
	return strResult;
}

BOOL CGrid_Comm::GetRowToCodeName(int nRow, CString& strCode, CString& strName, int& nMarketType)
{
	BOOL bResult = FALSE;
	
	if (nRow < GetNumberRows())
	{
		int nCol = GetCodeColNumber();
		
		CUGCell cell;
		GetCell(nCol, nRow, &cell);
		
		strCode = cell.GetOtherJongCode();
		strName = cell.GetText();
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