// PnTool.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnTool.h"
//#include "NewMenu.h"
#include "PnToolTip.h"
#include "PnToolInfo.h"
#include "../Common/SavedHead.h"
#include "memDC.h"
#include "../../h/axisgobj.h"
//#include "../Common/MyTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//const int	c_iMaxBtn = 17;
const int	maxPOP = 8;
const int	gapHEIGHT = 3;
const int	gapWIDTH = 1;
const int	btnWIDTH = 22;
const int	eachGAP = 3;

#define	TIME_ID	2
#define TOOLPOP	31	// popLINE - toolHLINE

int g_iToolKind[] = {	toolHIDE, toolMOUSE, toolHISTORY, toolDELETE, toolINPUT, 
			toolWIDTH, toolPALETTE, toolTEND, toolTENDEX, toolHLINE,
			toolARC, toolUARROW, toolGIGAN, toolDAEDEUNG, toolFIBOLINE,
			toolGANNFAN, toolTRILINE, toolAndrewPf, toolPatternUP, toolPatternDN, toolPENCIL};

const int c_iMaxBtn = sizeof(g_iToolKind) / sizeof(int);

CString g_strToolText[] = {	"도구숨기기", "마우스", "하나씩지우기", "전체지우기", "문자입력", 
				"도구굵기설정", "도구색상설정", "추세선", "연장추세선", "가로선",
				"원", "위화살표", "기간 및 등락표시", "대등일표시", "Fibonacci TimeZone",
				"Gann Fan(상승)", "Trizone Line", "Andrews Pichfork",
				"상승패턴분석", "하락패턴분석", "연필"};
				//"Gann Fan(상승)", "Trizone Line"};


#define RGB_BG		RGB(220,226,234)
//#define RGB_BG		RGB(209,209,209)

/////////////////////////////////////////////////////////////////////////////
// CPnTool

CPnTool::CPnTool(CWnd *pwndView, CWnd* pwndGView, CFont* pFont, int ctrlID, char* pcDllName)
{
	m_pApp = (CAxisGComApp *)AfxGetApp();
	m_pwndView = pwndView;
	m_pwndGView = pwndGView;
	m_pfont = pFont;
	m_pToolTip = NULL;

	m_mapPnToolInfo.RemoveAll();
	m_iToolSave = toolNO;
	m_iMoveSave = toolNO;
	m_bCapture = false;
	m_bLDown = false;
	m_bMouseIN = false;
	m_bPattern = false;

	m_iSelLine = toolHLINE;
	m_iSelFigure = toolARC;
	m_iSelArrow = toolUARROW;
	m_iSelGigan = toolGIGAN;
	m_iSelFibo = toolFIBOLINE;
	m_iSelGann = toolGANNFAN;
	m_iSelTri = toolTRILINE;
	m_iSelWidth = 1;
	m_iToolCount = 0;

	m_bPopupFailed = false;

	m_ePosi = eNoPosi;

//	m_pMyTool = m_pApp->GetMyTool(pwndView);

	m_patternUP = FALSE;
	m_patternDN = FALSE;
}

CPnTool::~CPnTool()
{
}


BEGIN_MESSAGE_MAP(CPnTool, CWnd)
	//{{AFX_MSG_MAP(CPnTool)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_MEASUREITEM()
	ON_WM_INITMENUPOPUP()
	ON_WM_MENUCHAR()
	ON_WM_DESTROY()
	ON_COMMAND_RANGE(ID_TOOL_HLINE, ID_TOOL_TRILINELEVEL, &CPnTool::OnMenuCheck)
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_TOOL, OnToolEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnTool message handlers

LRESULT CPnTool::OnToolEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case TOOL_GET_POS_COMMENT:
		m_strComment = GetPosComment(CPoint(LOWORD(lParam), HIWORD(lParam)));
		return (LRESULT)(LPCTSTR)m_strComment;
	case TOOL_RELEASE:
		m_iMoveSave = toolNO;
		SetMouseBtn(toolMOUSE);
		break;
	case TOOL_CHECK_RECT:
		{
			CPoint	*pPoint = (CPoint *)lParam;
			ScreenToClient(pPoint);

			int id = SearchPointInTool(*pPoint);
			return id;
		}
		break;
	case TOOL_GET_SELECT_TOOL:
		SetMouseBtn(lParam);
		
		switch(lParam)
		{
		case toolHLINE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelLine);
			break;
		case toolARC:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelFigure);
			break;
		case toolUARROW:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelArrow);
			break;
		case toolGIGAN:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelGigan);
			break;
		case toolFIBOLINE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelFibo);
			break;
		case toolGANNFAN:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelGann);
			break;
		case toolTRILINE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelTri);
			break;
		default:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART, 
					MAKEWPARAM(chartCtrl, chartToolSelect), lParam);
			break;
		}
		break;
	case TOOL_AUTOTRAN:
		break;
	case TOOL_CONFIG:
		ToolConfig();
		break;

	case TOOL_PATTERN:
		switch (HIWORD(wParam)) 
		{
			case getPatternUPEnabled:
				return m_patternUP;
			case setPatternUPEnabled:
				SetPatternUP(lParam);
				return m_patternUP;
			case getPatternDNEnabled:
				return m_patternUP;
			case setPatternDNEnabled:
				SetPatternDN(lParam);
				return m_patternDN;
		}
		break;

	default:
		break;
	}

	return TRUE;
}

int CPnTool::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return Init();
}

int CPnTool::Init()
{
	m_strMapName = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, getDllName), NULL);
	
	if (!m_pToolTip)
	{
		m_pToolTip = new CPnToolTip(m_pwndView, this, GEV_TOOL);
		m_pToolTip->Create(m_pfont);
	}
	
	CString path = m_pApp->GetImagePath(m_pwndView, TOOLIMAGE);
	m_pImageList = m_pApp->GetImage(imgTool, path);
	path = m_pApp->GetImagePath(m_pwndView, TOOL2IMAGE);
	m_pImageList2 = m_pApp->GetImage(imgTool2, path);

	m_ePosi = eRight;

	if (!Load())
		InitTool();

	return 0;
}

void CPnTool::OnDestroy() 
{
	if (m_pToolTip)
		WND_DELETE(m_pToolTip);

	DeleteAll();

	CWnd::OnDestroy();
}

void CPnTool::OnPaint() 
{
	CPaintDC dc(this); 

	CMemDC	mdc(&dc);
	DrawTool(&mdc);
}

void CPnTool::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)
		return;	
}

void CPnTool::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	if (m_pwndGView)	// if being autoscrolling, return
	{
		BOOL res = m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, isAutoScroll), 0);
		if (res)
		{
			return;
		}
	}

	bool bPopUp = false;
	m_bPattern = false;
	CPnToolInfo* pPnToolInfo;
	int selectID = SearchPointInTool(point);

	if (selectID == toolNO)
	{
		return;
	}

	if (selectID == toolPatternUP) 
	{
		m_bPattern = true;
		if (m_mapPnToolInfo.Lookup(selectID, pPnToolInfo)) 
			SetPatternUP(!m_patternUP);

		return;
	}
	if (selectID == toolPatternDN) 
	{
		m_bPattern = true;
		if (m_mapPnToolInfo.Lookup(selectID, pPnToolInfo)) 
			SetPatternDN(!m_patternDN);

		return;
	}

	if (m_mapPnToolInfo.Lookup(int(m_iToolSave), pPnToolInfo))
	{
		pPnToolInfo->m_iBtnState = btnNOT;
		InvalidateRect(pPnToolInfo->m_rect);
	}

	m_iToolSave = selectID;
	int shiftUpDn = 0;
/*
	if (m_mapPnToolInfo.Lookup(int(selectID), pPnToolInfo))
	{
		pPnToolInfo->m_iBtnState = btnDOWN;
		InvalidateRect(pPnToolInfo->m_rect);

		if ((selectID >= popLINE && selectID <= popETC) || selectID == toolWIDTH)
		{
			CNewMenu menu;
			menu.CreatePopupMenu();

			switch (selectID)
			{
			case popLINE:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_HLINE, _T("가로선"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_VLINE, _T("세로선"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_CROSS, _T("십자선"));
				break;
			case popFIGURE:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_ARC, _T("원"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_TRI, _T("삼각형"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_RECT, _T("사각형"));
				break;
			case popARROW:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_UARROW, _T("위화살표"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_DARROW, _T("아래화살표"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_LARROW, _T("왼화살표"));
				menu.InsertMenu(3, MF_BYPOSITION , ID_TOOL_RARROW, _T("오른화살표"));
				break;

			case popGIGAN:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_GIGAN, _T("단순"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_GIGAN2, _T("복순"));
				switch(m_iSelGigan)
				{
				case toolGIGAN:
					menu.CheckMenuItem(ID_TOOL_GIGAN, MF_BYCOMMAND|MF_CHECKED);
					break;
				case toolGIGAN2:
					menu.CheckMenuItem(ID_TOOL_GIGAN2, MF_BYCOMMAND|MF_CHECKED);
					break;
				}
				break;

			case popDAEDEUNG:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_DAEDEUNGCFG, _T("설정"));
				break;

			case popFIBO:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_FIBOLINE, _T("Fibonacci TimeZone"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_FIBOFAN, _T("Fibonacci Fan"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_FIBOFAN2, _T("Fibonacci Fan2"));
				menu.InsertMenu(3, MF_BYPOSITION , ID_TOOL_FIBOARC, _T("Fibonacci Arc"));
				menu.InsertMenu(4, MF_BYPOSITION , ID_TOOL_FIBORET, __T("Fibonacci Retracement"));
				break;

			case popGANN:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_GANNFAN, _T("Gann Fan(상승)"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_RGANNFAN, _T("Gann Fan(하락)"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_GANNLINE, _T("Gann Line"));
				break;

			case popETC:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_TRILINE, _T("Trizone Line"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_QUARDLINE, _T("Quardrant Line"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_ACCELFAN, _T("Speed Resistence Line"));
				menu.InsertMenu(3, MF_BYPOSITION , ID_TOOL_ACCELARC, _T("Speed Resistence Arc"));				
				menu.InsertMenu(4, MF_BYPOSITION , ID_TOOL_TRILINELEVEL, _T("Trizone Level"));
				break;

			case toolWIDTH:
				menu.InsertMenu(0, MF_BYPOSITION , ID_TOOL_WIDTH1, _T("1 Dot"));
				menu.InsertMenu(1, MF_BYPOSITION , ID_TOOL_WIDTH2, _T("2 Dot"));
				menu.InsertMenu(2, MF_BYPOSITION , ID_TOOL_WIDTH3, _T("3 Dot"));
				menu.InsertMenu(3, MF_BYPOSITION , ID_TOOL_WIDTH4, _T("4 Dot"));
				switch(m_iSelWidth)
				{
				case 1:
					menu.CheckMenuItem(ID_TOOL_WIDTH1, MF_BYCOMMAND|MF_CHECKED);
					break;
				case 2:
					menu.CheckMenuItem(ID_TOOL_WIDTH2, MF_BYCOMMAND|MF_CHECKED);
					break;
				case 3:
					menu.CheckMenuItem(ID_TOOL_WIDTH3, MF_BYCOMMAND|MF_CHECKED);
					break;
				case 4:
					menu.CheckMenuItem(ID_TOOL_WIDTH4, MF_BYCOMMAND|MF_CHECKED);
					break;
				}
				break;
			}

			CPoint pt(point);
			CRect clientRect;
			GetClientRect(&clientRect);
			ClientToScreen(&clientRect);
			ClientToScreen(&pt);
			menu.LoadToolBar(IDR_TOOLBAR1);
			menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, clientRect.left, pt.y, this);

			m_bPopupFailed = true;
			bPopUp = true;
			if (selectID == toolWIDTH)
			{
				pPnToolInfo->m_iBtnState = btnUP;
				InvalidateRect(pPnToolInfo->m_rect);
			}
			SetMouseBtn(toolMOUSE);
		}
	}
*/
	if (!m_bCapture)
	{
		::SetCapture(this->m_hWnd);
		m_bCapture = true;
	}
	
	if (!bPopUp)
		m_bLDown = true;
}

void CPnTool::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);	

	if (m_bCapture)
	{
		::ReleaseCapture();
		m_bCapture = false;
	}
	m_bLDown = false;

	if (m_pwndGView)	// if being autoscrolling, return
	{
		BOOL res = m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, isAutoScroll), 0);
		if (res)
		{
			return;
		}
	}

	CPnToolInfo* pPnToolInfo;
	int selectID = SearchPointInTool(point);

	if (!m_bPopupFailed)
	{
		if (m_iToolSave != selectID)
		{
			if (!m_mapPnToolInfo.Lookup(int(m_iToolSave), pPnToolInfo))
			{
				return;
			}

			if (!m_bPattern)	// pattern은 독립적으로 실행됨
			{
				pPnToolInfo->m_iBtnState = btnNOT;
				InvalidateRect(pPnToolInfo->m_rect);

				m_iToolSave = toolNO;
			}
			return;
		}
	}
	else
		m_bPopupFailed = false;

	if (m_mapPnToolInfo.Lookup(int(selectID), pPnToolInfo))
	{
		switch(selectID)
		{
		case toolHLINE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelLine);
			break;
		case toolARC:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelFigure);
			break;
		case toolUARROW:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelArrow);
			break;
		case toolGIGAN:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelGigan);
			break;
		case toolFIBOLINE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelFibo);
			break;
		case toolGANNFAN:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelGann);
			break;
		case toolTRILINE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), m_iSelTri);
			break;
		case toolPALETTE:
			{
				CColorDialog	dlg;
				if (dlg.DoModal() == IDOK)
				{
					COLORREF rgb = dlg.GetColor();
					if (m_pwndGView)
						m_pwndGView->SendMessage(GEV_CHART,
							MAKEWPARAM(chartCtrl, chartToolColor), rgb);
				}
			}
			pPnToolInfo->m_iBtnState = btnUP;
			InvalidateRect(pPnToolInfo->m_rect);
			SetMouseBtn(toolMOUSE);
			break;
		case toolHIDE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_VIEW, 
					MAKEWPARAM(viewShowWindow, showTool), 0);
			SetMouseBtn(toolMOUSE);
			break;
		case toolHISTORY:
		case toolDELETE:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), pPnToolInfo->m_iKind);
			SetMouseBtn(toolMOUSE);
			break;
		default:
			if (m_pwndGView)
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), pPnToolInfo->m_iKind);
			break;
		}
	}
}

void CPnTool::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	if (!m_bMouseIN)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		m_bMouseIN = _TrackMouseEvent(&tme);
	}

	if (m_bLDown)
		return;

	CPnToolInfo* pPnToolInfo = NULL;
	int selectID = SearchPointInTool(point);
	if (m_mapPnToolInfo.Lookup(selectID, pPnToolInfo))
	{
		if (pPnToolInfo->m_iBtnState == btnDOWN)
		{
			if (m_mapPnToolInfo.Lookup(int(m_iMoveSave), pPnToolInfo))
			{
				if (pPnToolInfo->m_iBtnState != btnDOWN)
					pPnToolInfo->m_iBtnState = btnNOT;
				InvalidateRect(pPnToolInfo->m_rect);
			}

			m_iMoveSave = selectID;
			return; 
		}
	}

	CRect rect;
	GetClientRect(rect);
//	CString tipS = _T("");

	if (m_iMoveSave != toolNO && m_iMoveSave != selectID)// && selectID != toolNO)
	{
		if (m_mapPnToolInfo.Lookup(int(m_iMoveSave), pPnToolInfo))
		{
			if (pPnToolInfo->m_iBtnState != btnDOWN)
				pPnToolInfo->m_iBtnState = btnNOT;
			InvalidateRect(pPnToolInfo->m_rect);

			if (m_iMoveSave >= toolHLINE && m_iMoveSave <= toolTRILINE)
			{
				if (m_mapPnToolInfo.Lookup(int (m_iMoveSave + TOOLPOP), pPnToolInfo))
					pPnToolInfo->m_iBtnState = btnNOT;
			}
			else if (m_iMoveSave >= popLINE && m_iMoveSave <= popETC)
			{
				if (m_mapPnToolInfo.Lookup(int(m_iMoveSave - TOOLPOP), pPnToolInfo))
					pPnToolInfo->m_iBtnState = btnNOT;
			}
			InvalidateRect(pPnToolInfo->m_rect);

			m_iMoveSave = selectID;
		}
	}
	else if (m_iMoveSave != toolNO && m_iMoveSave == selectID)
	{
		return; 
	}

	if (m_mapPnToolInfo.Lookup(int(selectID), pPnToolInfo))
	{
		pPnToolInfo->m_iBtnState = btnMOVE;
		InvalidateRect(pPnToolInfo->m_rect);
//		tipS = pPnToolInfo->m_strText;

		if (selectID >= toolHLINE && selectID <= toolTRILINE)
		{
			if (m_mapPnToolInfo.Lookup(int (selectID + TOOLPOP), pPnToolInfo))
				pPnToolInfo->m_iBtnState = btnMOVE;
		}
		else if (selectID >= popLINE && selectID <= popETC)
		{
			if (m_mapPnToolInfo.Lookup(int(selectID - TOOLPOP), pPnToolInfo))
				pPnToolInfo->m_iBtnState = btnMOVE;
		}
		InvalidateRect(pPnToolInfo->m_rect);

		m_iMoveSave = selectID;
		return;
	}
}

void CPnTool::mouseLeaveAction(UINT nFlags, CPoint point)
{
	m_bMouseIN = FALSE;
	
	if (m_pToolTip)
	{
		m_pToolTip->m_dispStr = _T("");
		m_pToolTip->hideToolTip();
	}

	CPnToolInfo* pPnToolInfo = NULL;
	if (m_iMoveSave != toolNO)
	{
		if (m_mapPnToolInfo.Lookup(int(m_iMoveSave), pPnToolInfo))
		{
			if (pPnToolInfo->m_iBtnState == btnDOWN)
				return;
			
			pPnToolInfo->m_iBtnState = btnNOT;
			InvalidateRect(pPnToolInfo->m_rect);

			if (m_iMoveSave >= toolHLINE && m_iMoveSave <= toolTRILINE)
			{
				if (m_mapPnToolInfo.Lookup(int (m_iMoveSave + TOOLPOP), pPnToolInfo))
					pPnToolInfo->m_iBtnState = btnNOT;
			}
			else if (m_iMoveSave >= popLINE && m_iMoveSave <= popETC)
			{
				if (m_mapPnToolInfo.Lookup(int(m_iMoveSave - TOOLPOP), pPnToolInfo))
					pPnToolInfo->m_iBtnState = btnNOT;
			}
			InvalidateRect(pPnToolInfo->m_rect);

			m_iMoveSave = toolNO;
		}
	}
}

void CPnTool::OnTimer(UINT nIDEvent) 
{
	CWnd::OnTimer(nIDEvent);
}

LRESULT CPnTool::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_MOUSELEAVE)
	{
		mouseLeaveAction(0, CPoint(0,0));
		return TRUE;
	}

	if (m_pToolTip)
		m_pToolTip->RelayEvent(message);	
	
	return CWnd::WindowProc(message, wParam, lParam);
}

void CPnTool::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
}

void CPnTool::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

LRESULT CPnTool::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult=CWnd::OnMenuChar(nChar, nFlags, pMenu);
	return lresult;
}

void CPnTool::OnMenuCheck(UINT nID)
{
	m_bPopupFailed = false;
	SetMouseBtn(toolMOUSE, true);

	CPnToolInfo* pPnToolInfo = 0;
	switch (nID)
	{
	case ID_TOOL_HLINE:
		m_iSelLine = toolHLINE;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolHLINE);
		if (m_mapPnToolInfo.Lookup(toolHLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("가로선");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_VLINE:
		m_iSelLine = toolVLINE;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolVLINE);
		if (m_mapPnToolInfo.Lookup(toolHLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("세로선");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_CROSS:
		m_iSelLine = toolCROSS;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolCROSS);
		if (m_mapPnToolInfo.Lookup(toolHLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("십자선");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_ARC:
		m_iSelFigure = toolARC;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolARC);
		if (m_mapPnToolInfo.Lookup(toolARC, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("원");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_TRI:
		m_iSelFigure = toolTRI;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolTRI);
		if (m_mapPnToolInfo.Lookup(toolARC, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("삼각형");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_RECT:
		m_iSelFigure = toolRECT;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolRECT);
		if (m_mapPnToolInfo.Lookup(toolARC, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("사각형");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_UARROW:
		m_iSelArrow = toolUARROW;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolUARROW);
		if (m_mapPnToolInfo.Lookup(toolUARROW, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("위화살표");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_DARROW:
		m_iSelArrow = toolDARROW;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolDARROW);
		if (m_mapPnToolInfo.Lookup(toolUARROW, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("아래화살표");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_LARROW:
		m_iSelArrow = toolLARROW;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolLARROW);
		if (m_mapPnToolInfo.Lookup(toolUARROW, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("왼화살표");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_RARROW:
		m_iSelArrow = toolRARROW;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolRARROW);
		if (m_mapPnToolInfo.Lookup(toolUARROW, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("오른화살표");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_GIGAN:
		m_iSelGigan = toolGIGAN;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolGIGAN);
		if (m_mapPnToolInfo.Lookup(toolGIGAN, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_GIGAN2:
		m_iSelGigan = toolGIGAN2;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolGIGAN2);
		if (m_mapPnToolInfo.Lookup(toolGIGAN, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_DAEDEUNGCFG:
		if (m_pwndGView)
		{
			if (m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolDAEDEUNGCFG))
			{
				if (m_mapPnToolInfo.Lookup(toolDAEDEUNG, pPnToolInfo))
				{
					pPnToolInfo->m_iBtnState = btnDOWN;
					InvalidateRect(pPnToolInfo->m_rect);
				}
				m_pwndGView->SendMessage(GEV_CHART,
					MAKEWPARAM(chartCtrl, chartToolSelect), toolDAEDEUNG);
			}
			else
			{
				SetMouseBtn(toolMOUSE);
			}
		}

		break;
	case ID_TOOL_FIBOLINE:
		m_iSelFibo = toolFIBOLINE;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolFIBOLINE);
		if (m_mapPnToolInfo.Lookup(toolFIBOLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Fibonacci TimeZone");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_FIBOFAN:
		m_iSelFibo = toolFIBOFAN;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolFIBOFAN);
		if (m_mapPnToolInfo.Lookup(toolFIBOLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Fibonacci Fan");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_FIBOFAN2:
		m_iSelFibo = toolFIBOFAN2;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolFIBOFAN2);
		if (m_mapPnToolInfo.Lookup(toolFIBOLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Fibonacci Fan2");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;	
	case ID_TOOL_FIBOARC:
		m_iSelFibo = toolFIBOARC;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolFIBOARC);
		if (m_mapPnToolInfo.Lookup(toolFIBOLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Fibonacci Arc");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_FIBORET:
		m_iSelFibo = toolFIBORET;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolFIBORET);
		if (m_mapPnToolInfo.Lookup(toolFIBOLINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Fibonacci Retracement");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_GANNLINE:
		m_iSelGann = toolGANNLINE;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolGANNLINE);
		if (m_mapPnToolInfo.Lookup(toolGANNFAN, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Gann Line");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_GANNFAN:
		m_iSelGann = toolGANNFAN;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolGANNFAN);
		if (m_mapPnToolInfo.Lookup(toolGANNFAN, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Gann Fan(상승)");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_RGANNFAN:
		m_iSelGann = toolRGANNFAN;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolRGANNFAN);
		if (m_mapPnToolInfo.Lookup(toolGANNFAN, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Gann Fan(하락)");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_ACCELFAN:
		m_iSelTri = toolACCELFAN;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolACCELFAN);
		if (m_mapPnToolInfo.Lookup(toolTRILINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Speed Resistence Line");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_ACCELARC:
		m_iSelTri = toolACCELARC;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolACCELARC);
		if (m_mapPnToolInfo.Lookup(toolTRILINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Speed Resistence Arc");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_QUARDLINE:
		m_iSelTri = toolQUARDLINE;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolQUARDLINE);
		if (m_mapPnToolInfo.Lookup(toolTRILINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Quardrant Line");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_TRILINE:
		m_iSelTri = toolTRILINE;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolTRILINE);
		if (m_mapPnToolInfo.Lookup(toolTRILINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Trizone Line");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_TRILINELEVEL:
		m_iSelTri = toolTRILINELEVEL;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolSelect), toolTRILINELEVEL);
		if (m_mapPnToolInfo.Lookup(toolTRILINE, pPnToolInfo))
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			pPnToolInfo->m_strText.Format("Trizone Level");
			InvalidateRect(pPnToolInfo->m_rect);
		}
		break;
	case ID_TOOL_WIDTH1:
		m_iSelWidth = 1;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolWidth), 1);
		SetMouseBtn(toolMOUSE);
		break;
	case ID_TOOL_WIDTH2:
		m_iSelWidth = 2;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolWidth), 2);
		SetMouseBtn(toolMOUSE);
		break;
	case ID_TOOL_WIDTH3:
		m_iSelWidth = 3;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolWidth), 3);
		SetMouseBtn(toolMOUSE);
		break;
	case ID_TOOL_WIDTH4:
		m_iSelWidth = 4;
		if (m_pwndGView)
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, chartToolWidth), 4);
		SetMouseBtn(toolMOUSE);
		break;
	}
	Invalidate();
}

void CPnTool::DrawTool(CDC *pDC)
{
	CFont	*sFont = pDC->SelectObject(m_pfont);
	CBrush	*pBGBbrush = m_pApp->GetBrush(m_pwndView, RGB_BG);

	CRect rectC;
	GetClientRect(rectC);
	CRect rect = rectC;
	//pDC->FillRect(&rect, pbrushBG);
	COLORREF	clrTrans = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 181);
	pDC->FillSolidRect(&rect, clrTrans);
	CPen pn(PS_NULL, 1, clrTrans);
	CPen	*sPen = pDC->SelectObject(&pn);
	CBrush	*sBrush = pDC->SelectObject(pBGBbrush);
	pDC->RoundRect(rect,CPoint(5,5));

	CPnToolInfo* pPnToolInfo;
	CRect	emptyRC = CRect(0, 0, 0, 0);

	for (int ii = 1; ii < c_iMaxBtn; ii++)
	{
		if (!m_mapPnToolInfo.Lookup(g_iToolKind[ii], pPnToolInfo))
			continue;
		
		switch (g_iToolKind[ii])
		{
		case toolHLINE:
			BtnDraw(pDC, g_iToolKind[ii], m_iSelLine * 3, pPnToolInfo, m_pImageList);
			break;
		case toolARC:
			BtnDraw(pDC, g_iToolKind[ii], m_iSelFigure * 3, pPnToolInfo, m_pImageList);
			break;
		case toolUARROW:
			BtnDraw(pDC, g_iToolKind[ii], m_iSelArrow * 3, pPnToolInfo, m_pImageList);
			break;
		case toolFIBOLINE:
			BtnDraw(pDC, g_iToolKind[ii], m_iSelFibo * 3, pPnToolInfo, m_pImageList);
			break;
		case toolGANNFAN:
			BtnDraw(pDC, g_iToolKind[ii], m_iSelGann * 3, pPnToolInfo, m_pImageList);
			break;
		case toolTRILINE:
			BtnDraw(pDC, g_iToolKind[ii], m_iSelTri * 3, pPnToolInfo, m_pImageList);
			break;
		case toolPatternUP:
		case toolPatternDN:
		default:
			BtnDraw(pDC, g_iToolKind[ii], g_iToolKind[ii] * 3, pPnToolInfo, m_pImageList);
			break;
		}

		if (ii == toolDELETE)
		{
			CPen	*ppenGray = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, RGB(128,128, 128));
			CPen	*sPen = pDC->SelectObject(ppenGray);

			pDC->MoveTo(pPnToolInfo->m_rect.left, pPnToolInfo->m_rect.bottom + 2);
			pDC->LineTo(pPnToolInfo->m_rect.right, pPnToolInfo->m_rect.bottom + 2);

			CPen	*ppenWhite = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, RGB(255, 255, 255));
			pDC->SelectObject(ppenWhite);
			pDC->MoveTo(pPnToolInfo->m_rect.left, pPnToolInfo->m_rect.bottom + 3);
			pDC->LineTo(pPnToolInfo->m_rect.right, pPnToolInfo->m_rect.bottom + 3);

			pDC->SelectObject(sPen);
		}
	}

	for (int ii = popLINE; ii <= popLINE + maxPOP; ii++)
	{
		if (!m_mapPnToolInfo.Lookup(ii, pPnToolInfo))
			continue;
		
		BtnDraw(pDC, ii, 0, pPnToolInfo, m_pImageList2);
	}

//	rect = rectC;
//	rect.DeflateRect(1, 1, 1, 0);
//	pDC->Draw3dRect(rect, RGB(255, 255, 255), RGB(0, 0, 0));

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sFont);
}

void CPnTool::SetMouseBtn(int toolID, bool bRelease)
{
	CPnToolInfo* pPnToolInfo;
	if (bRelease)
	{
		if (m_mapPnToolInfo.Lookup(toolID, pPnToolInfo))
			pPnToolInfo->m_iBtnState = btnNOT;

		return;
	}

	for (int ii = 1; ii < c_iMaxBtn; ii++)	// pattern은 독립적으로 실행되므로 제외
	{
		if (c_iMaxBtn - 3 <= ii && ii <= c_iMaxBtn - 2)
			continue;

		if (!m_mapPnToolInfo.Lookup(g_iToolKind[ii], pPnToolInfo))
			continue;
		
		if (g_iToolKind[ii] == toolID)
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			m_iToolSave = toolID;
		}
		else
		{
			pPnToolInfo->m_iBtnState = btnNOT;
		}
	}

	for (int ii = popLINE; ii <= popLINE + maxPOP; ii++)
	{
		if (!m_mapPnToolInfo.Lookup(ii, pPnToolInfo))
			continue;
		
		pPnToolInfo->m_iBtnState = btnNOT;
	}
	Invalidate();
}

void CPnTool::InitTool()
{
	CRect rectC;
	GetClientRect(rectC);
	if (rectC.IsRectEmpty())
		return;

	DeleteAll();

	CRect rectButtonFirst;
	rectButtonFirst.top    = rectC.top + gapHEIGHT;
	rectButtonFirst.left   = rectC.left + 10;
	rectButtonFirst.right   = rectC.left + btnWIDTH + 10;
	int iBtnWidthb = rectButtonFirst.Width() + 1;
	rectButtonFirst.bottom  = rectButtonFirst.top + iBtnWidthb * c_iMaxBtn + eachGAP*2;
	if (rectButtonFirst.bottom > rectC.bottom)
		rectButtonFirst.bottom = rectC.bottom;

	CRect rectButton = rectButtonFirst;
	rectButton.bottom = rectButtonFirst.top;

	for (int ii = 1; ii < c_iMaxBtn; ii++)
	{
//		if (g_iToolKind[ii] == toolAndrewPf)
//			continue;

		rectButton.top   = rectButton.bottom;
		rectButton.bottom  = rectButton.top + iBtnWidthb;
		if (rectButton.right > rectC.right)
			return;

		AddControlClass(g_iToolKind[ii], rectButton, g_iToolKind[ii], g_strToolText[ii], ii);
		switch (ii)
		{
		case toolHLINE:
			AddControlClass(popLINE, rectButton, popLINE, "", ii);
			break;
		case toolARC:
			AddControlClass(popFIGURE, rectButton, popFIGURE, "", ii);
			break;
		case toolUARROW:
			AddControlClass(popARROW, rectButton, popARROW, "", ii);
			break;
		case toolGIGAN:
			AddControlClass(popGIGAN, rectButton, popGIGAN, "", ii);
			break;
		case toolDAEDEUNG:
			AddControlClass(popDAEDEUNG, rectButton, popDAEDEUNG, "", ii);
			break;
		case toolFIBOLINE:
			AddControlClass(popFIBO, rectButton, popFIBO, "", ii);
			break;
		case toolGANNFAN:
			AddControlClass(popGANN, rectButton, popGANN, "", ii);
			break;
		case toolTRILINE:
			AddControlClass(popETC, rectButton, popETC, "", ii);
			break;
		default:
			break;
		}

		rectButton.bottom += eachGAP;
		if (ii == toolDELETE)
			rectButton.bottom += 2;
	}
	m_iToolCount = m_mapPnToolInfo.GetCount();
	Invalidate();
}

void CPnTool::AddControlClass(int iKey, CRect rectTool, int iKind, CString strText, int iIndex)
{
	CPnToolInfo* pPnToolInfo;

	if (iKey == popLINE || iKey == popFIGURE || iKey == popGANN || iKey == popFIBO || 
		iKey == popARROW || iKey == popGIGAN || iKey == popDAEDEUNG || iKey == popETC)
	{
		bool bNewCreate = false;
		if (!m_mapPnToolInfo.Lookup(iKey, pPnToolInfo))
		{
			bNewCreate = true;
			pPnToolInfo = new CPnToolInfo;
		}
		
		pPnToolInfo->m_iBtnState = btnNOT;
		pPnToolInfo->m_rect.SetRect(rectTool.left - 8, rectTool.top, rectTool.right, rectTool.bottom);
		pPnToolInfo->m_iKind = iKind;
		pPnToolInfo->m_strText = strText;
		pPnToolInfo->m_iIndex = iIndex;
		if (bNewCreate)
			m_mapPnToolInfo.SetAt(iKey, pPnToolInfo);
	}
	else
	{
		bool bNewCreate = false;
		if (!m_mapPnToolInfo.Lookup(iKey, pPnToolInfo))
		{
			bNewCreate = true;
			pPnToolInfo = new CPnToolInfo();
		}
		
		if (iKey == toolMOUSE)
		{
			pPnToolInfo->m_iBtnState = btnDOWN;
			m_iToolSave = toolMOUSE;
		}
		else
			pPnToolInfo->m_iBtnState = btnNOT;

		pPnToolInfo->m_rect = rectTool;
		pPnToolInfo->m_iKind = iKind;
		pPnToolInfo->m_strText = strText;
		pPnToolInfo->m_iIndex = iIndex;
		if (bNewCreate)
			m_mapPnToolInfo.SetAt(iKey, pPnToolInfo);
	}
}

int CPnTool::SearchPointInTool(CPoint pt)
{
	CPnToolInfo* pPnToolInfo;
	CRect rect;
	for (int ii = toolHIDE; ii <= toolPENCIL; ii++)
	{
		if (!m_mapPnToolInfo.Lookup(ii, pPnToolInfo))
			continue;
		
		rect = pPnToolInfo->m_rect;
		if (rect.PtInRect(pt))
			return ii;
	}
	for (int ii = popLINE; ii <= popLINE + maxPOP; ii++)
	{
		if (!m_mapPnToolInfo.Lookup(ii, pPnToolInfo))
			continue;

		rect = pPnToolInfo->m_rect;
		if (rect.PtInRect(pt))
			return ii;	
	}

	if (m_mapPnToolInfo.Lookup(36, pPnToolInfo))
	{
		rect = pPnToolInfo->m_rect;
		if (rect.PtInRect(pt))
			return 36;
	}
	return toolNO;
}

void CPnTool::BtnDraw(CDC* pDC, int selectID, int imgID, class CPnToolInfo* pPnToolInfo, CImageList* pImage)
{
	if (pPnToolInfo->m_iIndex < 0)
		return;

	CPoint	imagePt;
	imagePt.x = pPnToolInfo->m_rect.left + 1;
	imagePt.y = pPnToolInfo->m_rect.top + 1;
	
	CRect rect = pPnToolInfo->m_rect;
	if (pImage)
	{
		switch (pPnToolInfo->m_iBtnState)
		{
		case btnMOVE:
			pImage->Draw(pDC, imgID+1, imagePt, ILD_TRANSPARENT);
			break;
		case btnDOWN:
			pImage->Draw(pDC, imgID+2, imagePt, ILD_TRANSPARENT);
			break;
		case btnNOT:
		case btnUP:
			pImage->Draw(pDC, imgID, imagePt, ILD_TRANSPARENT);
			break;
		}
	}
	else
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(pPnToolInfo->m_strText, pPnToolInfo->m_rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	}
}

void CPnTool::DeleteAll()
{
	CPnToolInfo* pPnToolInfo;
	int key;
	for (POSITION pos = m_mapPnToolInfo.GetStartPosition(); pos; )
	{
		m_mapPnToolInfo.GetNextAssoc(pos, key, pPnToolInfo);
		delete pPnToolInfo;
	}
	m_mapPnToolInfo.RemoveAll();
}

CString CPnTool::GetPosComment(CPoint pt)
{
	ScreenToClient(&pt);
	CString tmpS;
	CPnToolInfo* pPnToolInfo;
	int	key;
	for (POSITION pos = m_mapPnToolInfo.GetStartPosition(); pos; )
	{
		m_mapPnToolInfo.GetNextAssoc(pos, key, pPnToolInfo);

		if (pPnToolInfo->m_rect.PtInRect(pt))
		{
			tmpS = CString(pPnToolInfo->m_strText);
			tmpS.TrimRight();
			return tmpS;
		}
	}

	return _T("");
}

bool CPnTool::Load()
{
	CString strFileName;
	strFileName.Format("%s\\GEX\\%s\\%s%s", m_pApp->GetHome(m_pwndView), m_pApp->GetName(m_pwndView), m_strMapName, TOL);

	DWORD dwReadLen;
	CSavedHead sh;
	char* pcData = sh.Read(strFileName, dwReadLen);

	if (!pcData || dwReadLen < 1)
		return false;

	if (sh.m_sh.dwVersion < VERSION_TOL || dwReadLen != sh.m_sh.dwTotalLength)
	{
		return false;
	}
	

	DeleteAll();

	m_iToolCount = sh.m_sh.dwDataCnt;
	for (UINT ii = 0; ii < sh.m_sh.dwDataCnt; ii++)
	{
		CPnToolInfo* ppti = new CPnToolInfo;
		ppti->SetStruck(pcData + ii * SZ_PNTOOLINFO);

		m_mapPnToolInfo.SetAt(ppti->m_iKind, ppti);
	}

	return true;
}

bool CPnTool::Save()
{
	CString strFileName;
	strFileName.Format("%s\\GEX\\%s\\%s%s", m_pApp->GetHome(m_pwndView), m_pApp->GetName(m_pwndView), m_strMapName, TOL);

	CSavedHead sh;
	char* pcData = sh.Make(VERSION_TOL, m_iToolCount, SZ_PNTOOLINFO);

	CPnToolInfo* pPnToolInfo;
	int iKey;
	for (POSITION pos = m_mapPnToolInfo.GetStartPosition(); pos; )
	{
		m_mapPnToolInfo.GetNextAssoc(pos, iKey, pPnToolInfo);
		pPnToolInfo->GetStruck(pcData);
		pcData +=  SZ_PNTOOLINFO;
	}

	if (!sh.Write(strFileName))
		return false;

	return true;
}

void CPnTool::ToolConfig()
{
	int iCount = m_mapPnToolInfo.GetCount();
	char* pcBuffer = new char[SZ_PNTOOLINFO * iCount];

	CPnToolInfo* pPnToolInfo;
	int iKey;
	int ii = 0;
	for (POSITION pos = m_mapPnToolInfo.GetStartPosition(); pos; )
	{
		m_mapPnToolInfo.GetNextAssoc(pos, iKey, pPnToolInfo);
		pPnToolInfo->GetStruck(pcBuffer + SZ_PNTOOLINFO * ii++);
	}

	struct _toolConfig toolConfig;
	toolConfig.pImgLst = m_pImageList;
	toolConfig.pcBuf = pcBuffer;
	toolConfig.iCount = iCount;

	if (m_pwndGView)
	{
		BOOL bResult = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgToolCfg), (LPARAM)&toolConfig);
		if (bResult)
		{
			DeleteAll();
			for (int ii = 0; ii < toolConfig.iCountResult; ii++)
			{
				CPnToolInfo* pPTI = new CPnToolInfo;
				pPTI->SetStruck(toolConfig.pcBufResult + SZ_PNTOOLINFO * ii);
				m_mapPnToolInfo.SetAt(pPTI->m_iKind, pPTI);
			}

			m_iToolCount = toolConfig.iCountResult;
			delete toolConfig.pcBufResult;

			Save();

			for (POSITION pos = m_mapPnToolInfo.GetStartPosition(); pos; )
			{
				CPnToolInfo* pPTI;
				m_mapPnToolInfo.GetNextAssoc(pos, iKey, pPTI);

				switch (pPTI->m_iKind)
				{
				case toolHLINE:
					switch(m_iSelLine)
					{
					case toolVLINE:
						pPTI->m_strText.Format("세로선");
						break;
					case toolCROSS:
						pPTI->m_strText.Format("십자선");
						break;
					}
					break;
				case toolARC:
					switch(m_iSelFigure)
					{
					case toolTRI:
						pPTI->m_strText.Format("삼각형");
						break;
					case toolRECT:
						pPTI->m_strText.Format("사각형");
						break;
					}
					break;
				case toolUARROW:
					switch(m_iSelArrow)
					{
					case toolDARROW:
						pPTI->m_strText.Format("아래화살표");
						break;
					case toolLARROW:
						pPTI->m_strText.Format("왼화살표");
						break;
					case toolRARROW:
						pPTI->m_strText.Format("오른화살표");
						break;
					}
					break;
//				case toolGIGAN:
//					switch(m_iSelGigan)
//					{
//					case :
//						break;
//					}
//					break;
				case toolFIBOLINE:
					switch(m_iSelFibo)
					{
					case toolFIBOFAN:
						pPTI->m_strText.Format("Fibonacci Fan");
						break;
					case toolFIBOFAN2:
						pPTI->m_strText.Format("Fibonacci Fan2");
						break;
					case toolFIBOARC:
						pPTI->m_strText.Format("Fibonacci Arc");
						break;
					case toolFIBORET:
						pPTI->m_strText.Format("Fibonacci Retracement");
						break;
					}
					break;
				case toolGANNFAN:
					switch(m_iSelGann)
					{
					case toolRGANNFAN:
						pPTI->m_strText.Format("Gann Fan(하락)");
						break;
					case toolGANNLINE:
						pPTI->m_strText.Format("Gann Line");
						break;
					}
					break;
				case toolTRILINE:
					switch(m_iSelTri)
					{
					case toolQUARDLINE:
						pPTI->m_strText.Format("Quardrant Line");
						break;
					case toolACCELFAN:
						pPTI->m_strText.Format("Speed Resistence Line");
						break;
					case toolACCELARC:
						pPTI->m_strText.Format("Speed Resistence Arc");
						break;
					case toolTRILINELEVEL:
						pPTI->m_strText.Format("Trizone Level");
						break;
					}

					break;
				}
			}

			Invalidate();
		}
	}

	delete pcBuffer;
}


void CPnTool::SetPatternUP(BOOL pattern)
{
	CPnToolInfo* pPnToolInfo;

	if (m_patternUP != pattern) 
	{
		if (m_mapPnToolInfo.Lookup(toolPatternUP, pPnToolInfo)) 
		{
			m_patternUP = pattern;

			if (m_patternUP) 
				pPnToolInfo->m_iBtnState = btnDOWN;
			else 
				pPnToolInfo->m_iBtnState = btnNOT;

			Invalidate();
	
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, patternChanged), MAKEWPARAM(m_patternUP, true));

		}
	}
}

void CPnTool::SetPatternDN(BOOL pattern)
{
	CPnToolInfo* pPnToolInfo;

	if (m_patternDN != pattern) 
	{
		if (m_mapPnToolInfo.Lookup(toolPatternDN, pPnToolInfo)) 
		{
			m_patternDN = pattern;

			if (m_patternDN) 
				pPnToolInfo->m_iBtnState = btnDOWN;
			else 
				pPnToolInfo->m_iBtnState = btnNOT;

			Invalidate();
	
			m_pwndGView->SendMessage(GEV_CHART,
				MAKEWPARAM(chartCtrl, patternChanged), MAKEWPARAM(m_patternDN, false));

		}
	}
}
