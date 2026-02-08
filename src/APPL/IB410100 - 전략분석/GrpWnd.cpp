// GrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB410100.h"
#include "GrpWnd.h"
#include "CrossLine.h"
#include "PopupWnd.h"
#include "math.h"
#include "memdc.h"
#include "../../h/axisfire.h"
#include "../../h/symbol.h"

#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define YEARDATE_CAL	365.00
#define	YEARDATE_WORK	250.00
#define SONIK_UNIT	100.0
#define FUTURES_UNIT	5.0
#define JISU_UNIT	0.1

#define PAYOFF_JISU1	0
#define PAYOFF_JISU2	1
#define PAYOFF_DAY	2
#define PAYOFF_CHANGE	3

#define MINMAX_TYPE1	0
#define MINMAX_TYPE2	1

#define FEE_FUTURE	0
#define FEE_OPTION	1

#define ARROW		0
#define UPARROW		1
#define SIZEWE		2

#define POPWIDTH	80
#define POPHEIGHT	17
#define sPer		'|'

#define MINGAM_SONIK	0

#define	X_GAMMA		0.2316419
#define CNORMAL_1	0.31938153
#define CNORMAL_2	-0.356563782
#define CNORMAL_3	1.781477937
#define CNORMAL_4	-1.821255978
#define CNORMAL_5	1.330274429

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd

CGrpWnd::CGrpWnd(CWnd* pView, CWnd* parent)
{
	m_pApp = (CIB410100App *)AfxGetApp();
	m_pParent = parent;
	m_pView   = pView;

	m_GraphData.RemoveAll();

	m_cdkr  = 0;
	m_k200  = 0;
	m_k200Jisu = 0;		// cej
	m_Total = 0;

	m_PayOffStatus = PAYOFF_JISU1;

	for (int ii = DELTA ; ii <= IRON ; ii++)
		m_MinGamStatus[ii] = FALSE;

	m_ctlFontSize = 9;

	m_hCursor.cursor = NULL;
	m_hCursor.kind = -1;
	m_hCursor.point = -1;
	m_hCursor.btnFlag = false;

	m_moveLine.RemoveAll();

	m_bTracking = FALSE;
	m_chkyetak  = false;
	m_MinJjis   = 9999.;
	m_Maxjjis   = 0.;
	m_ironFlag  = false;
	m_pPopup    = NULL;
	m_pCrossLine = NULL;
	m_chknum    = 0;
	m_names     = _T("");
	m_delta = m_gamma = m_seta = m_vega = _T("");
	m_chkdelta = m_chkgamma = m_chkseta = m_chkvega = false;
	m_MouseValCnt = 11;

	m_yearDate = YEARDATE_CAL;
	m_jisuStart = -1;
	m_jisuEnd = -1;
	m_jjilMove = 0;
	m_bMoveStg = true;
	m_hTopHwnd = NULL;
}

CGrpWnd::~CGrpWnd()
{
	for (int ii = 0; ii < m_GraphData.GetSize(); ii++)	 {
		m_GraphData[ii] = nullptr;
		//delete data;
	}

	m_GraphData.RemoveAll();

	if (m_hCursor.cursor) 
		::DestroyCursor(m_hCursor.cursor);

	if (m_pPopup)
	{
		m_pPopup->DestroyWindow();
		m_pPopup = nullptr;
	}

	if (m_pCrossLine)
	{
		m_pCrossLine = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CGrpWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_GRPRefresh, OnMsgRefresh)
	ON_MESSAGE(WM_GRPAlert, OnMsgAlert)
	ON_MESSAGE(WM_GRPAddData, OnMsgAddData)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_GRPSetParam, OnMsgSetParam)
	ON_MESSAGE(WM_GRPBasicPrc, OnBasicPrc)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers
long CGrpWnd::OnMsgRefresh(WPARAM wParam, LPARAM lParam)
{
	switch ((int) wParam)
	{
	case DeleteGrpData: // Delete
		DeleteGraphData(0, TRUE);
		break;
	case RedrawGraph: // ReDraw
		if (((CMapWnd*)m_pParent)->m_jjIdx == 1)
			m_yearDate = YEARDATE_CAL;
		else
			m_yearDate = YEARDATE_WORK;

		RemoveGLineArray();
		break;
	case SetPayOff: // PayOff Set
		m_PayOffStatus = lParam;
		RemoveGLineArray();
		break;
	case ResetSensitive: // MinGam Set	(FALSE)
		m_MinGamStatus[lParam] = FALSE;
		RemoveGLineArray();
		break;
	case SetSensitive: // MinGam Set	(TRUE)
		m_MinGamStatus[lParam] = TRUE;
		RemoveGLineArray();
		break;
	case SetStgMouseMove:
		m_bMoveStg = lParam;
		break;
	}
	
	return 0;
}

long CGrpWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_SETPAL:
		initProperties();
		Invalidate();
		break;
	}

	return 0;
}

long CGrpWnd::OnMsgAlert(WPARAM wParam, LPARAM lParam)
{
	return 0;
	/*
	CString szData = CString ((char*) lParam);
	CString Symbol, Data;

	CString strCODX = m_pApp->Parser(szData, cTAB);

	if (strCODX.Compare(_T("K2001")) == 0) // KOSPI200
	{
		do
		{
			Symbol = m_pApp->Parser(szData, cTAB);
			Data = m_pApp->Parser(szData, cTAB);

			if (atoi(Symbol) == CURR)
			{
K200은 설정에 따른 기초자산값이 들어가야 한다.
				m_k200 = m_pApp->BuhoModify(Data);
				if (!m_hCursor.btnFlag)		
					RemoveGLineArray();

				break;
			}
		} while (szData.Compare(_T("")) != 0);
	}
	else
	{
		int	nSize = m_GraphData.GetSize();
		for (int ii = 0; ii < nSize; ii++)
		{
			if (m_GraphData.GetAt(ii)->code == strCODX)
			{
				do
				{
					Symbol = m_pApp->Parser(szData, cTAB);
					Data = m_pApp->Parser(szData, cTAB);
					
					if (atoi(Symbol) == CURR)
					{
						m_GraphData.GetAt(ii)->curr = m_pApp->BuhoModify(Data);
						if (((CMapWnd*)m_pParent)->m_idxVar == 1)
							RemoveGLineArray();

						break;
					}
				} while (szData.Compare(_T("")) != 0);
				
				break;
			}
		}

	}
	*/
	return 0;
}

long CGrpWnd::OnMsgAddData(WPARAM wParam, LPARAM lParam)
{
// 	OutputDebugString("[KSJ] OnMsgAddData");
	_GraphData *gData = (_GraphData*)lParam;
	AddGraphData(*gData);
	return 0;
}

long CGrpWnd::OnBasicPrc(WPARAM wParam, LPARAM lParam)
{
	const double prc = (double)lParam;
//TRACE("prc[%.2f]\n", prc);
	m_k200Jisu = prc / 100.0;
	Invalidate();
	return 0;
}

void CGrpWnd::RMouseUpMsg(int xpt, int ypt)
{
	_mouseLink	mLink;
	mLink.xpt = xpt;
	mLink.ypt = ypt;

	m_pParent->SendMessage(WM_MAPMouseUp, (WPARAM) FALSE, (LPARAM) (char*) &mLink);
}

int CGrpWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_ctlFont = m_pApp->GetFont(m_pView, m_ctlFontSize);
	initProperties();

	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, RGB(255, 255, 255), RGB(180,77,77));
	m_pPopup = std::make_unique<CPopupWnd>();
	m_pPopup->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(m_rcGraph.left, m_rcGraph.top, POPWIDTH, POPHEIGHT*4), this, NULL);
	m_pPopup->ShowWindow(SW_HIDE);
	
	if(GetActiveWindow() == NULL)		m_hTopHwnd = NULL;
	else								m_hTopHwnd = GetActiveWindow()->m_hWnd;
	return 0;
}

void CGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	memDC(&dc);

	DrawGraph(&memDC);
}

void CGrpWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString	Value;

	if (!m_pCrossLine)
		return;

	if (m_GraphData.GetSize() == 0)
	{
		CWnd::OnMouseMove(nFlags, point);
		return;
	}

	if (!m_rcGraph.PtInRect(point))
	{
		m_pCrossLine->HideCrossLine(point);
		m_pPopup->ShowWindow(SW_HIDE);
		return;
	}

	m_pCrossLine->ShowCrossLine(point);

	if (m_rcGraph.PtInRect(point))
	{
		for (int ii = DELTA ; ii <= VEGA; ii++)
		{
			GetSonik2(m_PayOffStatus, ii, point.x);
		}

		if (m_chkdelta)
			Value += m_delta + "|";
		if (m_chkgamma)
			Value += m_gamma + "|";
		if (m_chkseta)
			Value += m_seta + "|";
		if(m_chkvega)
			Value += m_vega + "|";
		
		m_pPopup->DrawPopup(m_chknum, m_names, Value);

		if (m_rcGraph.right / 2 < point.x && m_chknum > 0)
			m_pPopup->MoveWindow(CRect(m_rcGraph.left, m_rcGraph.top, POPWIDTH + m_rcGraph.left, POPHEIGHT * (m_chknum + 1) - (m_chknum + 1)));
		else
			m_pPopup->MoveWindow(CRect(m_rcGraph.right-POPWIDTH, m_rcGraph.top, m_rcGraph.right, POPHEIGHT * (m_chknum + 1) - (m_chknum + 1)));

		if (m_rcGraph.PtInRect(point) && m_chknum > 0)
			m_pPopup->ShowWindow(SW_SHOW);
	}

	if (m_rcGraph.PtInRect(point))
	{
		for (int ii = SONIK ; ii <= VEGA; ii++)
		{
			if (m_MinGamStatus[ii])
			{
				GetSonik(m_PayOffStatus, ii, point.x);
				break;
			}
		}
	}

	if (m_PayOffStatus == PAYOFF_JISU1 && m_MinGamStatus[SONIK])
	{
		if (m_hCursor.btnFlag)
		{
			switch (m_hCursor.kind)
			{
			case UPARROW:
				if (m_hCursor.point != -1)
				{
					DrawMoveLine(point.x, m_hCursor.kind);
					SetTrackMouse();
				}
				break;
			case SIZEWE:
				DrawMoveLine(point.x, m_hCursor.kind);
				SetTrackMouse();
				break;
			}
		}
		else
			LineCheck(point.x, point.y);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CGrpWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;

	SetMouseCursor(ARROW);
	Invalidate();

	return 0;
}

void CGrpWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_moveLine.RemoveAll();
	m_hCursor.btnFlag = true;
	m_hCursor.startXpt = point.x;

	switch (m_hCursor.kind)
	{
	case UPARROW:
		{
			m_hCursor.point = GetLinePoint(point.x);// xval 의 X idx
			if (m_hCursor.point != -1)
				BoldDrawLine(point.x, m_hCursor.kind);
		}
		break;
	case SIZEWE:
		{
			BoldDrawLine(point.x, m_hCursor.kind);
		}
		break;
	}

	if (m_rcGraph.right/2 < point.x && m_chknum > 0)
		m_pPopup->SetWindowPos(this, m_rcGraph.left, m_rcGraph.top, POPWIDTH, POPHEIGHT * m_chknum - (m_chknum - 1), SWP_FRAMECHANGED|SWP_SHOWWINDOW);
	else
		m_pPopup->SetWindowPos(this, m_rcGraph.right-POPWIDTH, m_rcGraph.top, m_rcGraph.right, POPHEIGHT * m_chknum - (m_chknum - 1), SWP_FRAMECHANGED|SWP_SHOWWINDOW);

	if (m_chknum > 0)
		m_pPopup->ShowWindow(SW_SHOW);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CGrpWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_hCursor.btnFlag = false;

	switch (m_hCursor.kind)
	{
	case UPARROW:
		{
			if (m_hCursor.point != -1)
			{
				_userMsg	uMsg;
				uMsg.kind = LineMovePoint;

				CString code = GetMoveCode(m_hCursor.cidx);

				if (m_hCursor.moveFlag)
				{
					uMsg.gab = GetMoveGab(m_hCursor.startXpt, point.x);

					m_pParent->SendMessage(WM_MAPLineMove, (WPARAM) &uMsg, (LPARAM) (char*) code.operator LPCSTR());
					SortHsga();
				}
				else // 이동가능한 최대위치일경우
				{
					uMsg.gab = GetMoveGab(m_hCursor.startXpt, m_moveLine[1].xpt);

					m_pParent->SendMessage(WM_MAPLineMove, (WPARAM) &uMsg, (LPARAM) (char*) code.operator LPCSTR());				
				}
			}
			else
				Invalidate();
		}
		break;
	case SIZEWE:
		{
			_userMsg	uMsg;
			uMsg.gab = GetMoveGab(m_hCursor.startXpt, point.x);
			uMsg.kind = LineMoveAll;

			m_pParent->SendMessage(WM_MAPLineMove, (WPARAM) &uMsg, (LPARAM) 0);
		}
		break;
	}
	
	SetMouseCursor(ARROW);
	m_pPopup->ShowWindow(SW_HIDE);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CGrpWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	RMouseUpMsg(point.x, point.y);
	
	CWnd::OnRButtonUp(nFlags, point);
}

BOOL CGrpWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor.cursor != NULL)
	{
		::SetCursor(m_hCursor.cursor);
		return TRUE;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGrpWnd::initProperties()
{
	m_gbColor = m_pApp->GetIndexColor(m_pView, 68);

	m_gColor = RGB(255, 255, 255);
	m_gslColor = RGB(230, 230, 230);
	m_gL1Color = RGB(0, 102, 255);
	m_gfColor = RGB(0, 0, 0);
	//m_gk2Color = RGB(0, 136, 0);
	m_gk2Color = RGB(128, 128, 0);		// 20071214
	m_gLzColor = RGB(98, 98, 98);

	m_gMlColor[0] = RGB(240, 0, 0);
	m_gMlColor[1] = RGB(255, 0, 255);
	m_gMlColor[2] = RGB(128, 128, 255);
	m_gMlColor[3] = RGB(0, 192, 0);
	m_gMlColor[4] = RGB(255, 128, 0);

	m_bunkiCount[PAYOFF_JISU1] = 0;
	m_bunkiCount[PAYOFF_JISU2] = 0;
}

void CGrpWnd::DrawGraph(CDC* pDC)
{
	const int	lspace = 40;
	const int	rspace = 50;
	const int	bspace = 20;
	const int	tspace = 15;

	CRect	rc;
	GetClientRect(rc);

	CBrush	gBback(m_gbColor);
	pDC->FillRect(rc, &gBback);
	DrawBox(pDC, RGB(141, 141, 141), NULL, rc, BOX_TOP, FALSE);

	m_rcGraph = CRect(rc.left + lspace, rc.top + tspace, 
				rc.right - rspace, rc.bottom - bspace);

	m_pCrossLine->Resize(m_rcGraph);

	CBrush	Gback(m_gColor);
	pDC->FillRect(m_rcGraph, &Gback);

	if (m_GraphData.GetSize() == 0)
		return;
	else
		SortHsga();

	double minY=0, maxY=0;

	SetxValue();

	bool	stepLine = TRUE;
	int	flag=0, mingam=0;

	CString strTemp;

	if(GetActiveWindow() == NULL) return;

	HWND hWnd = GetActiveWindow()->m_hWnd;

// 	strTemp.Format("[KSJ] [%x][%x][%x][%x][%x][%d]", m_pView->m_hWnd, GetTopWindow()->m_hWnd, hWnd, m_hTopHwnd, m_hWnd, ((CMapWnd*)m_pParent)->m_bOnDialog);
// 	OutputDebugString(strTemp);

	switch (m_PayOffStatus)
	{
	case PAYOFF_JISU1:
	case PAYOFF_JISU2:
		{
// 			strTemp.Format("[KSJ]m_MinGamStatus[%s]", m_MinGamStatus[SONIK]? "TRUE":"FALSE");
// 			OutputDebugString(strTemp);

			if (m_MinGamStatus[SONIK])
			{
				flag = PAYOFF_JISU1;
				mingam = SONIK;
				CalMangi(flag, mingam);
				CalIron(flag, mingam);
				GetMinMaxY(flag, mingam, minY, maxY, MINMAX_TYPE1);

				//2012.07.23 KSJ 종목다이얼로그가 떠있으면 그리지 않는다.
				//팝업창이 떠 있으면 그리지 않는다.
				if(!((CMapWnd*)m_pParent)->m_bOnDialog && (hWnd == m_hTopHwnd))
				{
					DrawXLine(pDC, mingam, minY, maxY);				
				}

				DrawYLine(pDC, flag);

// 				DrawXLine(pDC, mingam, minY, maxY);
// 				
// // 				OutputDebugString("[KSJ] DrawXLine123 OUT");
// 
// 				DrawYLine(pDC, flag);

// 				OutputDebugString("[KSJ] DrawYLine");

				// 이론손익
				if (m_MinGamStatus[IRON])
				{
					flag = PAYOFF_JISU2;
					mingam = SONIK;
					CalIron(flag, mingam);
					BungiGap(flag, mingam);
					DrawgLine(pDC, m_gL1Color, flag, mingam, minY, maxY);
				}

				// 만기손익
				flag = PAYOFF_JISU1;
				mingam = SONIK;
//				CalIron(flag, mingam);
				DrawgLine(pDC, m_gMlColor[0], flag, mingam, minY, maxY);
				stepLine = FALSE;

				if (flag == PAYOFF_JISU1)
				{
					BungiGap(flag, mingam);
					//GetParent()->SendMessage(WM_USER + 500, 0, 1);
				}
			}
			flag = PAYOFF_JISU2;
			for (int ii = DELTA ; ii <= VEGA ; ii++)
			{
				if (m_MinGamStatus[ii])
				{
					mingam = ii;
					CalMinGam(flag, mingam);
					GetMinMaxY(flag, mingam, minY, maxY, MINMAX_TYPE1);

					if (stepLine)
					{
						//2012.07.23 KSJ 종목다이얼로그가 떠있으면 그리지 않는다.
						//팝업창이 떠 있으면 그리지 않는다.
						if(!((CMapWnd*)m_pParent)->m_bOnDialog && (hWnd == m_hTopHwnd))
						{
							DrawXLine(pDC, mingam, minY, maxY);				
						}

						DrawYLine(pDC, flag);
						stepLine = FALSE;
					}

					DrawgLine(pDC, m_gMlColor[ii], flag, mingam, minY, maxY);
				}
			}
		}
		break;

	case PAYOFF_DAY:
	case PAYOFF_CHANGE:
		{
			flag = m_PayOffStatus;

			if (m_MinGamStatus[SONIK])
			{
				mingam = SONIK;
				CalIron(flag, mingam);
				GetMinMaxY(flag, mingam, minY, maxY, MINMAX_TYPE1);

				//2012.07.23 KSJ 종목다이얼로그가 떠있으면 그리지 않는다.
				//팝업창이 떠 있으면 그리지 않는다.
				if(!((CMapWnd*)m_pParent)->m_bOnDialog && (hWnd == m_hTopHwnd))
				{
					DrawXLine(pDC, mingam, minY, maxY);				
				}

				DrawYLine(pDC, flag);

				DrawgLine(pDC, m_gMlColor[0], flag, mingam, minY, maxY);
				stepLine = FALSE;
			}

			for (int ii = DELTA ; ii <= VEGA ; ii++)
			{
				if (m_MinGamStatus[ii])
				{
					mingam = ii;
					CalMinGam(flag, mingam);
					GetMinMaxY(flag, mingam, minY, maxY, MINMAX_TYPE1);

					if (stepLine)
					{
						//2012.07.23 KSJ 종목다이얼로그가 떠있으면 그리지 않는다.
						//팝업창이 떠 있으면 그리지 않는다.
						if(!((CMapWnd*)m_pParent)->m_bOnDialog && (hWnd == m_hTopHwnd))
						{
							DrawXLine(pDC, mingam, minY, maxY);				
						}

						DrawYLine(pDC, flag);
						stepLine = FALSE;
					}

					DrawgLine(pDC, m_gMlColor[ii], flag, mingam, minY, maxY);
				}
			}
		}
		break;
	}

	DrawLineInfo(pDC, flag);
	m_pCrossLine->DrawCrossLine(pDC);
	GetParent()->PostMessage(WM_POSInitParam, 0, 0);
}

void CGrpWnd::DrawLineInfo(CDC* pDC, int flag)
{
	CRect rc, lRc, tRc;
	GetClientRect(&rc);

	const int left = rc.left + 2;
	const int right = m_rcGraph.left - 2;

	CFont	*pOldFont = (CFont*)pDC->SelectObject(m_ctlFont);
	CPen	*oldpen   = pDC->SelectObject(m_pApp->GetPen(m_pView, m_gslColor, PS_SOLID, 1));

	tRc = CRect(left, rc.top + 10, right, rc.top + 10);

	bool	k200 = FALSE;
	CString text[] = {"이론", "델타", "감마", "세타", "베가"};
	switch (flag)
	{
	case PAYOFF_JISU1:
	case PAYOFF_JISU2:
		{
			if (m_MinGamStatus[SONIK])
			{
				lRc = CRect(left + 8, tRc.bottom + 3, right - 8, tRc.bottom + 3  + 1);
				tRc = CRect(left, lRc.bottom + 3, right, lRc.bottom + 3 + 15);
				DrawBox(pDC, m_gMlColor[0], NULL, lRc, BOX_ALL, FALSE);
				SetText(pDC, tRc, "만기", m_gfColor);

				lRc = CRect(left + 8, tRc.bottom + 3, right - 8, tRc.bottom + 3  + 1);
				tRc = CRect(left, lRc.bottom + 3, right, lRc.bottom + 3 + 15);
				DrawBox(pDC, m_gL1Color, NULL, lRc, BOX_ALL, FALSE);
				SetText(pDC, tRc, "이론", m_gfColor);

				lRc = CRect(left + 8, tRc.bottom + 3, right - 8, tRc.bottom + 3  + 1);
				tRc = CRect(left, lRc.bottom + 3, right, lRc.bottom + 3 + 15);
				DrawBox(pDC, m_gk2Color, NULL, lRc, BOX_ALL, FALSE);
				//SetText(pDC, tRc, "K200", m_gfColor);
				SetText(pDC, tRc, "기초", m_gfColor);
				k200 = TRUE;
			}

			for (int ii = DELTA ; ii <= VEGA; ii++)
			{
				if (m_MinGamStatus[ii])
				{
					if (!k200)
					{
						lRc = CRect(left + 8, tRc.bottom + 3, right - 8, tRc.bottom + 3  + 1);
						tRc = CRect(left, lRc.bottom + 3, right, lRc.bottom + 3 + 15);
						DrawBox(pDC, m_gk2Color, NULL, lRc, BOX_ALL, FALSE);
						SetText(pDC, tRc, "K200", m_gfColor);
						k200 = TRUE;
					}
					lRc = CRect(left + 8, tRc.bottom + 3, right - 8, tRc.bottom + 3  + 1);
					tRc = CRect(left, lRc.bottom + 3, right, lRc.bottom + 3 + 15);
					DrawBox(pDC, m_gMlColor[ii], NULL, lRc, BOX_ALL, FALSE);
					SetText(pDC, tRc, text[ii], m_gfColor);
				}
			}
		}
		break;
	case PAYOFF_DAY:
	case PAYOFF_CHANGE:
		{
			for (int ii = SONIK ; ii <= VEGA; ii++)
			{
				if (m_MinGamStatus[ii])
				{
					lRc = CRect(left + 8, tRc.bottom + 3, right - 8, tRc.bottom + 3  + 1);
					tRc = CRect(left, lRc.bottom + 3, right, lRc.bottom + 3 + 15);
					DrawBox(pDC, m_gMlColor[ii], NULL, lRc, BOX_ALL, FALSE);
					SetText(pDC, tRc, text[ii], m_gfColor);
				}
			}
		}
		break;
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(oldpen);
}

double CGrpWnd::MingamBuho(CString jcode, int mmgb, int mingam)
{
	int reValue = 0;

	if (!jcode.GetLength())
		return reValue;

	switch (jcode[0])
	{
	case '1':
	case 'A':  //파생상품 코드개편
		break;
	case '2':
	case 'B':  //파생상품 코드개편
		if (mmgb == -1)
		{
			if (mingam == CETA)
				reValue =  1;
			else
				reValue = -1;
		}
		else if (mmgb == 1)
		{
			if (mingam == CETA)
				reValue = -1;
			else
				reValue = 1;
		}
		break;

	case '3':
	case 'C':  //파생상품 코드개편
		if (mmgb == -1)
		{
			if (mingam == GAMMA || mingam == VEGA)
				reValue = -1;
			else
				reValue = 1;
		}                
		else if (mmgb == 1)
		{
			if (mingam == DELTA || mingam == CETA)
				reValue = -1;
			else
				reValue = 1;
		}
		break;

	}

	return reValue;
}

double CGrpWnd::GetMinGamCal(int flag, int mingam, double ii)
{
	double dbjisu{}, dbjjmg{}, dbhvbd{}, d1{}, d2{}, buho{};
	double yVal = 0;

	for (int jj = 0 ; jj < m_GraphData.GetSize() ; jj++) // 데이타 수 만큼
	{
		switch (flag)
		{
		case PAYOFF_JISU1:
		case PAYOFF_JISU2:
			dbjisu = ii/* - ((CMapWnd*)m_pParent)->m_bData->bdjs*/;				// 지수
			dbjjmg = GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]) / m_yearDate;
			dbhvbd = GetEditHV(m_GraphData[jj]);	// 역사적 변동성
			if (dbhvbd <= 0) dbhvbd = 0.001;
			break;
		case PAYOFF_DAY:
			dbjisu = m_k200 - ((CMapWnd*)m_pParent)->m_bData->bdjs;
			dbjjmg = ii / m_yearDate;
			dbhvbd = GetEditHV(m_GraphData[jj]);
			if (dbhvbd <= 0) dbhvbd = 0.001;
			break;
		case PAYOFF_CHANGE:
			dbjisu = m_k200;
			dbjjmg = GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]) / m_yearDate;
			//dbhvbd = ii * 0.01;
			dbhvbd = GetEditHV(m_GraphData[jj]) + ii * 0.01;
			if (dbhvbd <= 0) dbhvbd = 0.001;
			break;
		}

		if (m_GraphData[jj]->gubn > 2 && m_GraphData[jj]->gubn < 7)
		{
			// 콜, 풋옵션 일때만 계산
			// d1 = GetD1(기초자산, 행사가격, r, HV, T)
			// d2 = GetD2(d1, HV, T)
			d1 = GetD1(dbjisu, m_GraphData[jj]->hsga, m_cdkr, dbhvbd, dbjjmg);
			d2 = GetD2(d1, dbhvbd, dbjjmg);
		}

		buho = MingamBuho(m_GraphData[jj]->code, m_GraphData[jj]->mmgb, mingam);

// 		CString strTemp;
// 		strTemp.Format("[KSJ] GetMinGamCal code[%s] dan[%d]\n", m_GraphData[jj]->code, m_GraphData[jj]->gdan);
// 		OutputDebugString(strTemp);

		switch (mingam)
		{
		case DELTA:
			switch (m_GraphData[jj]->gubn)
			{
			case 1: // 선물매도
				yVal = yVal - m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2016.04.14 KSJ 미니는 /5 
				break;
			case 2: // 선물매수
				yVal = yVal + m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2016.04.14 KSJ 미니는 /5 
				break;
			case 3: // 콜옵션
			case 4: 
				yVal = yVal + buho * fabs(GetNx(d1)) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				break;

			case 5: // 풋옵션
			case 6: 
				yVal = yVal + buho * fabs(-GetNx(-d1)) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				break;
			}
			break;

		case GAMMA:
			switch (m_GraphData[jj]->gubn)
			{
			case 3:
			case 4:
			case 5:
			case 6:
				yVal = yVal + buho * fabs(GetNu(d1) / (dbjisu * dbhvbd * sqrt(dbjjmg))) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				break;
			}
			break;

		case VEGA:
			switch (m_GraphData[jj]->gubn)
			{
			case 3:
			case 4:
			case 5:
			case 6:
				yVal = yVal + buho * fabs(dbjisu * sqrt(dbjjmg) * GetNu(d1) / 100) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				break;
			}
			break;
		case CETA:
			{
				double ceta_cdkr{};
				double ceta_d2{};
				double ceta{};

				switch(m_GraphData[jj]->gubn)
				{
				case 3: // 콜옵션
				case 4:
					ceta_cdkr = m_cdkr;
					ceta_d2 = d2;

					ceta = (-dbjisu * dbhvbd * GetNu(d1) / (2 * sqrt(dbjjmg)) - m_cdkr
						* m_GraphData[jj]->hsga * exp(ceta_cdkr * dbjjmg) * GetNx(ceta_d2));

					yVal = yVal + buho * fabs(ceta / m_yearDate) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
					break;
				case 5: // 풋옵션
				case 6:
					ceta_cdkr = -1 * m_cdkr;
					ceta_d2 = -1 * d2;

					ceta = (-dbjisu * dbhvbd * GetNu(d1) / (2 * sqrt(dbjjmg)) - m_cdkr
						* m_GraphData[jj]->hsga * exp(ceta_cdkr * dbjjmg) * GetNx(ceta_d2));

					yVal = yVal + buho * fabs(ceta / m_yearDate) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
					break;
				}
			}
			break;
		}
	}

	return yVal;
}

void CGrpWnd::CalMinGam(int flag, int mingam)
{
	if (m_gLine[flag][mingam].GetSize() > 0)
		return;

	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	_Line	line;
	double yVal=0;

	switch (flag)
	{
	case PAYOFF_JISU2:
	case PAYOFF_CHANGE:
		{
			for (double ii = leftX ; ii <= rightX ; ii = ii + 0.5)
			{
				yVal = GetMinGamCal(flag, mingam, ii);

				line.xval = ii;
				line.yval = yVal;

				m_gLine[flag][mingam].Add(line);
			}
		}
		break;
	case PAYOFF_DAY:
		{
			for (double ii = leftX ; ii >= rightX ; ii = ii - 0.5)
			{
				yVal = GetMinGamCal(flag, mingam, ii);

				line.xval = ii;
				line.yval = yVal;

				m_gLine[flag][mingam].Add(line);
			}
		}
		break;
	}
}

void CGrpWnd::DrawK200(CDC* pDC)
{
	if (m_PayOffStatus > PAYOFF_JISU2)
		return;

	const double leftX = m_Standard[PAYOFF_JISU2].leftX;
	const double rightX = m_Standard[PAYOFF_JISU2].rightX;

	const double perY = (double)m_rcGraph.Width() / (rightX - leftX);

	const int y1 = m_rcGraph.top;
	const int y2 = m_rcGraph.bottom;

	const double x = (double)m_rcGraph.left + perY * (m_k200Jisu - leftX);	// cej

	if (x > m_rcGraph.left && x < m_rcGraph.right)
	{
		CPen *oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, m_gk2Color, PS_SOLID, 1));

		pDC->MoveTo((int)x, y1);
		pDC->LineTo((int)x, y2);

		pDC->SelectObject(oldpen);
	}
}

void CGrpWnd::DrawYLine(CDC* pDC, int flag)
{
	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	CFont	*pOldFont = (CFont*)pDC->SelectObject(m_ctlFont);
	CPen	*oldpen   = pDC->SelectObject(m_pApp->GetPen(m_pView, m_gslColor, PS_SOLID, 1));

	double x=0;
	double valueX = 0;
	CString strX;
	CRect rc;
	rc.top = m_rcGraph.bottom + 5;
	rc.bottom = m_rcGraph.bottom + 20;

	switch (flag)
	{
	case PAYOFF_JISU1:
	case PAYOFF_JISU2:
		{
		const int yCnt = 8;
			const double perY = (double)m_rcGraph.Width() / (rightX - leftX);
			const double JisuStep = (double)((rightX - leftX) / yCnt);

			for (double ii = 0 ; ii <= yCnt ; ii++)
			{
				valueX = leftX + (JisuStep * ii);
				strX.Format("%.0lf",valueX);
				valueX = atof(strX);
				x = m_rcGraph.left + (valueX - leftX) * perY;

				rc.left = (int)(x - 17);
				rc.right = (int)(x + 17);
				pDC->MoveTo((int)x, m_rcGraph.top);
				pDC->LineTo((int)x, m_rcGraph.bottom);
				SetText(pDC, rc, strX, m_gfColor);
			}
		}
		break;
	case PAYOFF_DAY:
		{
			const double perY = (double)m_rcGraph.Width() / (leftX - rightX);
			int unit = 0;
			int nLine = 0;

			if ((leftX - rightX) < 12) unit = 1;
			else if ((leftX - rightX) < 20) unit = 2;
			else if ((leftX - rightX) < 30) unit = 3;
			else unit = 10;

			do
			{
				valueX = leftX - (unit * nLine);
				strX.Format("%.0f일",valueX);
				
				x = m_rcGraph.left + perY * (unit * nLine);
				rc.left = (int)(x - 17);
				rc.right = (int)(x + 17);

				if (x > m_rcGraph.right)
					break;

				pDC->MoveTo(RoundDouble(x), m_rcGraph.top);
				pDC->LineTo(RoundDouble(x), m_rcGraph.bottom);
				SetText(pDC, rc, strX, m_gfColor);

				nLine++;
			} while (valueX > rightX);
		}
		break;
	case PAYOFF_CHANGE:
		{
			const double perY = (double)m_rcGraph.Width() / (rightX - leftX);
			//int unit = 10;
			const int unit = 5;
			const int gap = 0;
			int nLine = 0;

			while (valueX < rightX)
			{
				//if (nLine == 0) gap = 0;
				//else gap = -1;

				valueX = leftX + ((unit * nLine) + gap);
				strX.Format("%.0f%%",valueX);

				x = m_rcGraph.left + perY * ((unit * nLine) + gap);
				rc.left = (int)(x - 17);
				rc.right = (int)(x + 17);

				pDC->MoveTo(RoundDouble(x), m_rcGraph.top);
				pDC->LineTo(RoundDouble(x), m_rcGraph.bottom);
				SetText(pDC, rc, strX, m_gfColor);

				nLine++;
			}
		}
		break;
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(oldpen);

	DrawK200(pDC);
}

int CGrpWnd::SetgRSpace(CDC* pDC, int mingam, double minY, double maxY)
{
	CString min, max;

	min = StringModify(minY, mingam);
	max = StringModify(maxY, mingam);

	const CSize minSize = pDC->GetTextExtent(min);
	const CSize maxSize = pDC->GetTextExtent(max);

	int	rspace = 0;
	if (minSize.cx > maxSize.cx)
	{
		rspace = minSize.cx + 5;
	}
	else
	{
		rspace = maxSize.cx + 5;
	}

	if (rspace < 20)
		rspace = 20;

	CRect rc;
	GetClientRect(&rc);

	CBrush	gBback(m_gbColor);
	pDC->FillRect(rc, &gBback);
	DrawBox(pDC, RGB(141, 141, 141), NULL, rc, BOX_TOP, FALSE);

	m_rcGraph.right = rc.right - rspace;

	m_pCrossLine->Resize(m_rcGraph);

	CBrush Gback(m_gColor);
	pDC->FillRect(m_rcGraph, &Gback);

	return rspace;
}

void CGrpWnd::DrawXLine(CDC* pDC, int mingam, double minY, double maxY)
{
// 	OutputDebugString("[KSJ] DrawXLine123 IN");

	CFont	*pOldFont = (CFont*)pDC->SelectObject(m_ctlFont);
	CPen	*oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, m_gslColor, PS_SOLID, 1));

	const int rspace = SetgRSpace(pDC, mingam, minY, maxY);

	CRect rc, bRc;
	GetClientRect(bRc);
	rc = CRect(bRc.right - 200, bRc.top + 1, bRc.right, bRc.top + 15);
	CString str[] = {"손익(천)", "델타", "감마", "세타", "베가"};
	CString str1[] = {"만기예상자산현황(천)", "델타", "감마", "세타", "베가"};

	if (!m_chkyetak)
		SetText(pDC, rc, str[mingam], m_gfColor, DT_RIGHT);
	else
		SetText(pDC, rc, str1[mingam], m_gfColor, DT_RIGHT);

	rc.left = bRc.right - rspace;
	rc.right = bRc.right - 2;

	double	ValueStep = 1;
	if ((maxY - minY) < 100)
		ValueStep = 100;

	const double	perY = (double)m_rcGraph.Height() / (maxY - minY);
//	double	pValue[100000];
	std::unique_ptr<double[]> pValue = std::make_unique<double[]>(10000);

	const int	cnt = GetDrawTickValue(pDC, pValue.get(), maxY * ValueStep, minY * ValueStep);
	double	y=0, valueY=0;

	for (int ii = 0 ; ii <= cnt ; ii++)
	{
		valueY = pValue[ii] / ValueStep;
		y = (double)m_rcGraph.bottom - (perY * (valueY - minY));

		if (y > m_rcGraph.top)
		{
			rc.top = RoundDouble(y - 5.0);
			rc.bottom = RoundDouble(y + 10.0);

			pDC->MoveTo(m_rcGraph.left, RoundDouble(y));		// back xline
			pDC->LineTo(m_rcGraph.right, RoundDouble(y));
			SetText(pDC, rc, StringModify(valueY, mingam), m_gfColor, DT_RIGHT);
		}
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(oldpen);

	DrawZero(pDC, minY, maxY);

	pValue = nullptr;
}

void CGrpWnd::DrawZero(CDC* pDC, double minY, double maxY)
{
	double stdY=0;
	const double perY = (double)m_rcGraph.Height() / (maxY - minY);
	
	stdY = perY * maxY;
	const double y = (double)m_rcGraph.top + stdY;

	if (y < m_rcGraph.bottom && y > m_rcGraph.top )
	{
		CPen *oldpen;
		oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, m_gLzColor, PS_DOT, 1));

		const int x1 = m_rcGraph.left;
		const int x2 = m_rcGraph.right;

		pDC->MoveTo(x1, RoundDouble(y));
		pDC->LineTo(x2, RoundDouble(y));

		pDC->SelectObject(oldpen);
	}
}

void CGrpWnd::RemoveGLineArray(int flag /* = -1 */)
{
	if (flag < 0)
	{
		for (int ii = 0 ; ii < 4 ; ii++)
		{
			for (int jj = 0 ; jj < 5 ; jj++)
			{
				m_gLine[ii][jj].RemoveAll();
			}
		}
	}
	else
	{
		for (int jj = 0 ; jj < 5 ; jj++)
		{
			m_gLine[flag][jj].RemoveAll();
		}
		
	}

	Invalidate();
}

void CGrpWnd::AddGraphData(_GraphData Data)
{

	gsl::owner<_GraphData*> GData = new _GraphData;

	sprintf(GData->code, "%s", Data.code);			// 종목코드
	GData->curr = Data.curr;				// 현재가
	GData->prc =  Data.prc;					// 주문가
	if (GData->prc == 0) GData->prc = GData->curr;
	GData->hvbd = Data.hvbd * 0.01;				// 역사적 변동성
	GData->numb = Data.numb ;				// 수량
	GData->mmgb = Data.mmgb;				// 매매구분(-1. 매도, 1. 매수)
	GData->jjis[0] = Data.jjis[0];				// 잔존만기일
	GData->jjis[1] = Data.jjis[1];				// 잔존만기일
	GData->gdan = Data.gdan;	//2012.05.17 KSJ	거래승수

	GData->gubn = GetCodeType(GData->code, GData->mmgb);	// Cal 코드
	GData->hsga = GetHsGa(GData->code);			// 행사가

	m_GraphData.Add(GData);

	m_CalData.RemoveAll();
	for (int ii = 0 ; ii <= m_GraphData.GetSize() ; ii++)
	{
		_CalData CData;
		m_CalData.Add(CData);
	}

// 	OutputDebugString("[KSJ] AddGraphData out");
}

void CGrpWnd::SetxValue()
{
	double min = 0;
	double max = 0;

	int sCount = 0;
	const int nSize = m_GraphData.GetSize();
	for (int ii = 0 ; ii < nSize ; ii++)
	{
		if (ii == 0)
		{
			min = m_GraphData[ii]->hsga;
			max = m_GraphData[ii]->hsga;
		}

		if (m_GraphData[ii]->gubn != 1 && m_GraphData[ii]->gubn != 2)
		{
			if (m_GraphData[ii]->hsga < min)
				min = m_GraphData[ii]->hsga;
			if (m_GraphData[ii]->hsga > max)
				max = m_GraphData[ii]->hsga;

			sCount++;
		}
	}

	if (m_jisuStart != -1 && m_jisuEnd != -1)
	{
		m_Standard[PAYOFF_JISU1].leftX = m_jisuStart; 
		m_Standard[PAYOFF_JISU1].rightX = m_jisuEnd;
		m_Standard[PAYOFF_JISU2].leftX = m_jisuStart; 
		m_Standard[PAYOFF_JISU2].rightX = m_jisuEnd;
	}
	else
	{
		double midVal=0;

		if (sCount == 0) // 옵션이 없을때
		{
			midVal = GetFiveTen(m_GraphData[0]->hsga);
			m_Standard[PAYOFF_JISU1].leftX = midVal - (5 * 3); m_Standard[PAYOFF_JISU1].rightX = midVal + (5 * 3);
		}
		else if (max == min)
		{
			midVal = GetFiveTen(max);
			m_Standard[PAYOFF_JISU1].leftX = midVal - (5 * 3); m_Standard[PAYOFF_JISU1].rightX = midVal + (5 * 3);
		}
		else
		{
			if ((max - min) > 30)
			{
				midVal = GetFiveTen((max + min) / 2.0);
				m_Standard[PAYOFF_JISU1].leftX = midVal - (5 * 3); m_Standard[PAYOFF_JISU1].rightX = midVal + (5 * 3);
			}
			else
			{
				midVal = GetFiveTen((max + min) / 2.0);
				m_Standard[PAYOFF_JISU1].leftX = midVal - (5 * 3); m_Standard[PAYOFF_JISU1].rightX = midVal + (5 * 3);
			}
		}
		
		// 이론손익
		m_Standard[PAYOFF_JISU2].leftX = m_Standard[PAYOFF_JISU1].leftX;
		m_Standard[PAYOFF_JISU2].rightX = m_Standard[PAYOFF_JISU1].rightX;
		m_jisuStart = m_Standard[PAYOFF_JISU2].leftX;
		m_jisuEnd = m_Standard[PAYOFF_JISU2].rightX;
	}

	// 잔존일수
	min = 0; max = 0;
	for (int ii = 0 ; ii < nSize ; ii++)
	{
		if (m_PayOffStatus == PAYOFF_DAY)
		{
			if (ii == 0)
				min = m_GraphData[ii]->jjis[((CMapWnd*)m_pParent)->m_jjIdx];

			if (min > m_GraphData[ii]->jjis[((CMapWnd*)m_pParent)->m_jjIdx])
				min = m_GraphData[ii]->jjis[((CMapWnd*)m_pParent)->m_jjIdx];
		}
		else
		{
			if (ii == 0)
				min = GetJJis(m_GraphData[ii]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]);

			if (min > GetJJis(m_GraphData[ii]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]))
				min = GetJJis(m_GraphData[ii]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]);
		}
	}
	m_Standard[PAYOFF_DAY].leftX = min; m_Standard[PAYOFF_DAY].rightX = 1;

	// 변동성
	//m_Standard[PAYOFF_CHANGE].leftX = 1; m_Standard[PAYOFF_CHANGE].rightX = 100;
	m_Standard[PAYOFF_CHANGE].leftX = -15; m_Standard[PAYOFF_CHANGE].rightX = 15;
}

double CGrpWnd::GetFiveTen(double data)
{
	CString string;
	string.Format("%.0f", data);

	CString strVal;
	double	rValue = 0;

	CString prev = string.Left(string.GetLength() - 1);
	CString itmp = string.Right(1);

	switch (atoi(itmp))
	{
	case 0:
	case 1:
	case 2:
		strVal.Format("%s%d", prev, 0);
		rValue = atof(strVal);
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		strVal.Format("%s%d", prev, 5);
		rValue = atof(strVal);
		break;
	case 7:
	case 8:
	case 9:
		strVal.Format("%s%d", prev, 0);
		rValue = atof(strVal) + 10;
		break;
	}

	return rValue;
}

int CGrpWnd::GetCodeType(CString Code, int mmgb)
{
	int rValue = 0;

	switch (Code[0])
	{
	case '1':	// 선물
	case 'A':	//파생상품 코드개편
		if (mmgb == -1)
			rValue = 1;
		else if (mmgb == 1)
			rValue = 2;
		break;

	case '2':	// 콜
	case 'B':  //파생상품 코드개편
		if (mmgb == -1)
			rValue = 3;
		else if (mmgb == 1)
			rValue = 4;
		break;

	case '3':	// 풋
	case 'C':  //파생상품 코드개편
		if (mmgb == -1)
			rValue = 5;
		else if (mmgb == 1)
			rValue = 6;
		break;
	}

	return rValue;
}

double CGrpWnd::GetHsGa(CString Code)
{
	if (Code.GetLength() == 0)
		return 0;

	double rValue=0;

	if (Code[0] == '1' || Code[0] == 'A')	// 선물  //파생상품 코드개편
	{
		rValue = m_k200;
	}
	else			// 옵션
	{
		CString tmpVal = Code.Right(3);
		rValue = atof(tmpVal);

		if (tmpVal[2] == '2' || tmpVal[2] == '7')
			rValue += 0.5;		
	}

	return	rValue;
}

void CGrpWnd::DeleteGraphData(int nItem, BOOL allDelete /* = FALSE */)
{
	gsl::owner<_GraphData*> GData = nullptr;

	if (allDelete)
	{
		const int nSize = m_GraphData.GetSize();
		for (int ii = 0 ; ii < nSize ; ii++)
		{
			m_GraphData.GetAt(ii) = nullptr;
			//GData = m_GraphData.GetAt(ii);
			//			delete GData;
		}
		m_GraphData.RemoveAll();
	}
	else
	{
		m_GraphData.GetAt(nItem) = nullptr;
		//GData = m_GraphData.GetAt(nItem);
		//delete GData;
		m_GraphData.RemoveAt(nItem);
	}

	RemoveGLineArray();
}

void CGrpWnd::DrawgLine(CDC* pDC, COLORREF gLColor, int flag, int mingam, double minY, double maxY)
{
	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	CPen *oldpen=nullptr;

	if (gLColor == m_gMlColor[0])
	{
		oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, gLColor, PS_SOLID, 2));

		int x=0, y=0;
		int nCnt = 0;
		for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
		{
			x = GetXpt(m_gLine[flag][mingam][ii].xval, leftX, rightX);
			y = GetYpt(m_gLine[flag][mingam][ii].yval + m_Total, minY, maxY);
			
			if (y > m_rcGraph.top && y < m_rcGraph.bottom)
			{
				if (nCnt == 0)
					pDC->MoveTo(x, y);
				else
					pDC->LineTo(x, y);
				
				nCnt++;
			}
		}
	}
	else
	{
		oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, gLColor, PS_SOLID, 1));

		int x=0, y=0;
		int nCnt = 0;
		for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
		{
			x = GetXpt(m_gLine[flag][mingam][ii].xval, leftX, rightX);
			y = GetYpt(m_gLine[flag][mingam][ii].yval + m_Total, minY, maxY);
			
			if (y > m_rcGraph.top && y < m_rcGraph.bottom)
			{
				if (nCnt == 0)
					pDC->MoveTo(x, y);
				else
					pDC->LineTo(x, y);
				
				nCnt++;
			}
		}
	}

	pDC->SelectObject(oldpen);
}

int CGrpWnd::GetYpt(double ypt, double minY, double maxY)
{
	double perY=0;

	if (maxY - minY == 0)
		perY = 0;
	else
		perY = (double)m_rcGraph.Height() / (maxY - minY);

	if (ypt < 0)
		return RoundDouble(maxY * perY + (-ypt * perY) + (double)m_rcGraph.top);
	else
		return RoundDouble(maxY * perY - (ypt * perY) + (double)m_rcGraph.top);
}

int CGrpWnd::GetXpt(double xpt, double leftX, double rightX)
{
	double perX=0;

	if ( rightX - leftX == 0 )
		perX = 0;
	else
		perX = (double)m_rcGraph.Width() / (rightX - leftX);

	return RoundDouble((xpt - leftX) * perX + (double)m_rcGraph.left);
}

void CGrpWnd::SortHsga()
{
	_GraphData* temp = nullptr;

	for (int ii = 0 ; ii < m_GraphData.GetSize() ; ii++) // 데이타 수
	{
		for (int jj = ii ; jj < m_GraphData.GetSize() ; jj++) // 데이타 수
		{
			if (m_GraphData[ii]->hsga > m_GraphData[jj]->hsga)
			{
				//m_GraphData[ii].swap(m_GraphData[jj]);
				temp = m_GraphData[ii];
				m_GraphData[ii] = m_GraphData[jj];
				m_GraphData[jj] = temp;
			}
		}
	}
}

void CGrpWnd::CalIron(int flag, int mingam)
{
	if (m_gLine[flag][mingam].GetSize() > 0)
		return;

	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	_Line	line;
	double	yVal=0;
//	int	jjis = 0;

	m_MinJjis   = 9999.;
	m_Maxjjis   = 0.;
	for (int jj = 0 ; jj < m_GraphData.GetSize() ; jj++) // 데이타 수 만큼
	{
		m_MinJjis = _KMIN(m_MinJjis, GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]) / m_yearDate);
		m_Maxjjis = _KMAX(m_Maxjjis, GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]) / m_yearDate);
	}

	/*
	for (jj = 0 ; jj < m_GraphData.GetSize() ; jj++) // 데이타 수 만큼
	{
		jjis = (int)GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]);

		if (jj < m_GraphData.GetSize() - 1)
		{
			if (jjis != (int)GetJJis(m_GraphData[jj + 1]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]))
			{
CString sTest;
sTest.Format("IC ??????? ((CMapWnd*)m_pParent)->m_jjIdx:%d jjis:%d  (idx:%d / %f): %d", ((CMapWnd*)m_pParent)->m_jjIdx, jjis, jj+1, m_GraphData[jj + 1]->jjis[((CMapWnd*)m_pParent)->m_jjIdx], (int)GetJJis(m_GraphData[jj + 1]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]));
OutputDebugString(sTest);

				m_ironFlag = true;
				break;
			}
			else
				m_ironFlag = false;
		}
	}
	*/
	m_ironFlag = true;

	switch (flag) // X축
	{
	case PAYOFF_JISU1:	//만기
	case PAYOFF_JISU2:	//이론
		{
//CString sTest;
//sTest.Format("IC >>>>>>>> CalIron flag:%d mingam:%d  m_ironFlag:%d jjis:%d ((CMapWnd*)m_pParent)->m_jjIdx:%d", flag, mingam, m_ironFlag, jjis, ((CMapWnd*)m_pParent)->m_jjIdx);
//OutputDebugString(sTest);
			for (double ii = leftX ; ii <= rightX ; ii = ii + JISU_UNIT)
			{
				yVal = GetIronCal(flag, ii);
//CString sTest;
//sTest.Format("IC CalIron ii:%f yVal:%f", ii, yVal);
//OutputDebugString(sTest);
				line.xval = ii;
				line.yval = yVal;

				m_gLine[flag][mingam].Add(line);
			}
		}
		break;
	case PAYOFF_CHANGE:	// 변동성
		{
			for (double ii = leftX ; ii <= rightX ; ii = ii + 1.0)
			{
				yVal = GetIronCal(flag, ii);

				line.xval = ii;
				line.yval = yVal;

				m_gLine[flag][mingam].Add(line);
			}
		}
		break;
	case PAYOFF_DAY:	// 잔존일수
		{
			for (double ii = leftX ; ii >= rightX ; ii = ii - 0.5)
			{
				yVal = GetIronCal(flag, ii);
//CString sTest;
//sTest.Format("IC X:%f iRon:%f\n", ii, yVal);
//OutputDebugString(sTest);
				line.xval = ii;
				line.yval = yVal;

				m_gLine[flag][mingam].Add(line);
			}
		}
		break;
	}
}

double CGrpWnd::GetIronCal(int flag, double ii)
{
	double dbjisu=0, dbjjmg=0, dbhvbd=0, d1=0, d2=0;
	double yVal = 0;
	double iPrice=0;

	CString strTemp;

	for (int jj = 0 ; jj < m_GraphData.GetSize() ; jj++) // 데이타 수 만큼
	{
		switch (flag)
		{
		case PAYOFF_JISU1:
		case PAYOFF_JISU2:

			dbjisu = ii/* - ((CMapWnd*)m_pParent)->m_bData->bdjs*/;				// 지수
//if (flag == PAYOFF_JISU2 && ii >= 172.30 && ii <= 172.50)
//{
//	Sleep(0);
//}
			dbjjmg = GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]) / m_yearDate;	// 종목별 잔존일수
			dbhvbd = GetEditHV(m_GraphData[jj]);	// 변동성
			if (dbhvbd <= 0) dbhvbd = 0.001;

			break;
		case PAYOFF_DAY:
			dbjisu = m_k200 - ((CMapWnd*)m_pParent)->m_bData->bdjs;
			dbjjmg = ii / m_yearDate;
			dbhvbd = GetEditHV(m_GraphData[jj]);
			if (dbhvbd <= 0) dbhvbd = 0.001;
			break;
		case PAYOFF_CHANGE:
			dbjisu = m_k200 - ((CMapWnd*)m_pParent)->m_bData->bdjs;
			dbjjmg = GetJJis(m_GraphData[jj]->jjis[((CMapWnd*)m_pParent)->m_jjIdx]) / m_yearDate;
			//dbhvbd = ii * 0.01;
			dbhvbd = GetEditHV(m_GraphData[jj]) + ii * 0.01;
			if (dbhvbd <= 0) dbhvbd = 0.001;
			break;
		}

		if (m_GraphData[jj]->gubn > 2 && m_GraphData[jj]->gubn < 7)
		{
			// 콜, 풋옵션 일때만 계산
			// d1 = GetD1(기초자산, 행사가격, r, HV, T)
			// d2 = GetD2(d1, HV, T)
			d1 = GetD1(dbjisu, m_GraphData[jj]->hsga, m_cdkr, dbhvbd, dbjjmg);
			d2 = GetD2(d1, dbhvbd, dbjjmg);
		}
// CString sTest;
// sTest.Format("[KSJ]IC GetIronCal jj:%d m_GraphData[jj]->gubn:%d dbjjmg:%f dbhvbd:%f d1:%f d2:%f", jj, m_GraphData[jj]->gubn, dbjjmg, dbhvbd, d1, d2);
//OutputDebugString(sTest);

// sTest.Format("[KSJ]m_ironFlag[%s]" ,m_ironFlag? "TRUE":"FALSE");
// OutputDebugString(sTest);

		if (m_ironFlag)
		{
			switch (m_GraphData[jj]->gubn)
			{
			case 1: // 선물매도
				// 기울기 : y = (-지수 + 선물매입가)*5 * 수량
				{
					if(m_MinJjis != dbjjmg || flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
						iPrice = F_IRGA(dbjisu, m_cdkr, m_GraphData[jj]->hvbd, dbjjmg);
					else
						iPrice = dbjisu;
					
					//double bPrice = (-1.0 * iPrice + m_GraphData[jj]->curr) * 5.0 * m_GraphData[jj]->numb;
					const double bPrice = (-1.0 * iPrice + m_GraphData[jj]->prc) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2016.04.14 KSJ 미니는 /5  // 20071213
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, bPrice);
				}
				break;
			case 2: // 선물매수
				// 기울기 : y = (지수 - 선물매입가)*5 * 수량
				{
					if(m_MinJjis != dbjjmg || flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
						iPrice = F_IRGA(dbjisu, m_cdkr, m_GraphData[jj]->hvbd, dbjjmg);
					else
						iPrice = dbjisu;
					//double bPrice = (iPrice - m_GraphData[jj]->curr) * 5.0 * m_GraphData[jj]->numb;
					const double bPrice = (iPrice - m_GraphData[jj]->prc) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2016.04.14 KSJ 미니는 /5 		// 20071213
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, bPrice);
				}
				break;
			case 3:
			case 4: // 콜옵션 (m_GraphData(jj).mmgb == -1 이면 매도)
				{
					// 이론가 - 현재가
					if(m_MinJjis != dbjjmg || flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
					{
						iPrice = m_GraphData[jj]->mmgb * (dbjisu * GetNx(d1) - m_GraphData[jj]->hsga * exp(-m_cdkr * dbjjmg) * GetNx(d2));
						iPrice = floor(iPrice * 1000) / 1000;	
					}
					else
					{
						iPrice = m_GraphData[jj]->mmgb * ( dbjisu - m_GraphData[jj]->hsga);
						if(dbjisu < m_GraphData[jj]->hsga)
							iPrice = 0;
					}
					const double hPrice = m_GraphData[jj]->mmgb * m_GraphData[jj]->prc;
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, (iPrice - hPrice) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan);	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				}
				break;
			case 5:
			case 6: // 풋옵션 (m_GraphData(jj).mmgb == -1 이면 매도)
				{
					// 이론가 - 현재가
					if(m_MinJjis != dbjjmg || flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
					{
						iPrice = m_GraphData[jj]->mmgb * (-dbjisu * GetNx(-d1) + m_GraphData[jj]->hsga * exp(-m_cdkr * dbjjmg) * GetNx(-d2));
						iPrice = floor(iPrice * 1000) / 1000;
					}
					else
					{
						iPrice = m_GraphData[jj]->mmgb * (- dbjisu + m_GraphData[jj]->hsga);
						if(dbjisu > m_GraphData[jj]->hsga)
							iPrice = 0;
						
					}
					const double hPrice = m_GraphData[jj]->mmgb * m_GraphData[jj]->prc;
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, (iPrice - hPrice) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan);	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				}
				break;
			}
		}
		else
		{
			switch (m_GraphData[jj]->gubn)
			{
			case 1: // 선물매도
				// 기울기 : y = (-지수 + 선물매입가)*5 * 수량
				{
					if(flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
						iPrice = F_IRGA(dbjisu, m_cdkr, m_GraphData[jj]->hvbd, dbjjmg);
					else
						iPrice = dbjisu;
					
					const double bPrice = (-1.0 * iPrice + m_GraphData[jj]->curr) * 5.0 * m_GraphData[jj]->numb;
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, bPrice);
				}
				break;
			case 2: // 선물매수
				// 기울기 : y = (지수 - 선물매입가)*5 * 수량
				{
					if(flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
						iPrice = F_IRGA(dbjisu, m_cdkr, m_GraphData[jj]->hvbd, dbjjmg);
					else
						iPrice = dbjisu;
					const double bPrice = (iPrice - m_GraphData[jj]->curr) * 5.0 * m_GraphData[jj]->numb;
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, bPrice);
				}
				break;
			case 3:
			case 4: // 콜옵션 (m_GraphData(jj).mmgb == -1 이면 매도)
				{
					// 이론가 - 현재가
					if(flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
					{
						iPrice = m_GraphData[jj]->mmgb * (dbjisu * GetNx(d1) - m_GraphData[jj]->hsga * exp(-m_cdkr * dbjjmg) * GetNx(d2));
						//iPrice = floor(iPrice * 1000) / 1000;
					}
					else
					{
						iPrice = m_GraphData[jj]->mmgb * ( dbjisu - m_GraphData[jj]->hsga);
						if(dbjisu < m_GraphData[jj]->hsga)
							iPrice = 0;
					}
//if (flag == PAYOFF_JISU2 && ii >= 172.30 && ii <= 172.50)
//{
//	Sleep(0);
//}
					const double hPrice = m_GraphData[jj]->mmgb * m_GraphData[jj]->prc;
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, (iPrice - hPrice) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan);	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
//if (flag == PAYOFF_JISU2 && ii >= 172.30 && ii <= 172.50)
//{
//	CString sTest;
//	sTest.Format("IC dbjisu:%f <<iron:%f>> hPrice:%f d1:%f d2:%f dbhvbd:%f \n", dbjisu, iPrice, hPrice, d1, d2, dbhvbd);
//	OutputDebugString(sTest);
//}
				}
				break;
			case 5:
			case 6: // 풋옵션 (m_GraphData(jj).mmgb == -1 이면 매도)
				{
					// 이론가 - 현재가
					if(flag == PAYOFF_JISU2 || flag == PAYOFF_DAY || flag == PAYOFF_CHANGE)
					{
						iPrice = m_GraphData[jj]->mmgb * (-dbjisu * GetNx(-d1) + m_GraphData[jj]->hsga * exp(-m_cdkr * dbjjmg) * GetNx(-d2));
						iPrice = floor(iPrice * 1000) / 1000;
					}
					else
					{
						iPrice = m_GraphData[jj]->mmgb * (- dbjisu + m_GraphData[jj]->hsga);
						if(dbjisu > m_GraphData[jj]->hsga)
							iPrice = 0;
						
					}
					const double hPrice = m_GraphData[jj]->mmgb * m_GraphData[jj]->prc;
					yVal = yVal + CalFee(true, m_GraphData[jj]->gubn, (iPrice - hPrice) * m_GraphData[jj]->numb * m_GraphData[jj]->gdan);	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				}
				break;
		}
		}
	}

	return yVal;
}


double CGrpWnd::CalFee(bool chk, int code, double price)
{
	return price;
/*
	int type;
	double rValue = 0;
	double fee;

	if (code <= 2)
		type = FEE_FUTURE;
	else
		type = FEE_OPTION;

//	fee = 1.0 - GetEditFee(type);
//	rValue = price * fee;

	if (chk)
	{
		fee = GetEditFee(chk, type) * price;

		if (price > 0)
			rValue = price - fee;
		else
			rValue = price + fee;

		return rValue;
	}
	else
	{
		fee = GetEditFee(chk, type) * price;

		if (fee != 0)
		{
			if (price > 0)
				rValue = price - fee;
			else
				rValue = price + fee;

			return rValue;
		}
		else
			return false;
	
	  }
*/
}

double CGrpWnd::GetEditHV(_GraphData *pData)
{
	double	rValue = pData->hvbd;
	CString buf;

	buf = (char*) GetParent()->SendMessage(WM_POSGetHV, 0, long(pData));

	if (buf != _T(""))
		rValue = atof(buf) * 0.01;

	return rValue;
}

/*
double CGrpWnd::GetEditFee(bool chk, int type)
{
	double rValue = 0;
	CString buf;

	buf = (char*) GetParent()->SendMessage(WM_POSGetFee, type, 0);

	if (chk)
	{
		if (buf != _T(""))
			rValue = atof(buf) * 0.01;
		
		return rValue;
	}
	else
	{
		if (buf == _T("") || buf == "0")
			return false;
		else
		{
			rValue = atof(buf) * 0.01;
			return rValue;
		}
	}
}
*/

double CGrpWnd::GetD1(double mValue, double hValue, double r, double HV, double T)
{
	// GetD1 = (Log(기초자산 / 행사가격) / Log(Exp(1)) + (r + 0.5 * HV ^ 2) * T) / (HV * Sqr(T))
	if ((HV * sqrt(T)) == 0 || hValue == 0)
		return 0;
	else
	        return (log(mValue / hValue) + (r + pow(HV, 2) / 2) * T) / (HV * pow(T, 0.5));
}

double CGrpWnd::GetD2(double d1, double HV, double T)
{
	//return d1 - HV * sqrt(T);
	return d1 - HV * pow(T, 0.5);
}

double CGrpWnd::GetNx(double xvalue)
{
	// 누적 정규분포
	/*
	double k;
	double a;
	double b;
	double c;
	double d;
	double e;
    
	k = 1 / (1 + 0.2316419 * xvalue);
	a = 0.31938153;
	b = -0.356563782;
	c = 1.781477937;
	d = -1.821255978;
	e = 1.330274429;
	
	if (xvalue >= 0) 
		return 1 - GetNu(xvalue) * (a * k + b * pow(k, 2) + c * pow(k, 3) + d * pow(k, 4) + e * pow(k, 5));
	else
		return 1 - GetNx(-xvalue);
	*/
	double i = 0, phai = 0, ret = 0;
	
	i = 1 / (1 + X_GAMMA * xvalue);
	phai = 3.1415924814;
	
	if (xvalue >= 0) 
		ret = 1 - (((1 / (pow((2 * phai), 0.5))) * exp(-1 * pow(xvalue, 2) / 2)) * 
		(CNORMAL_1 * i + CNORMAL_2 * pow(i, 2) + CNORMAL_3 * pow(i, 3) + CNORMAL_4 * pow(i, 4) + CNORMAL_5 * pow(i, 5)));
	else
		ret = 1 - GetNx(-1 * xvalue);

	return ret;
}

double CGrpWnd::GetNu(double xvalue)
{
	double PI;
	PI = 3.14159265;

	return 1 / sqrt(2 * PI) * exp(-0.5 * pow(xvalue, 2));
}

void CGrpWnd::GetSonik(int flag, int mingam, int xpt)
{
	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	double	rValue[20]{}, kpt[20]{}, perTwip{};
	CString	arStr[20];

	CRect rc;
	GetClientRect(&rc);

	const int lspace = m_rcGraph.left - rc.left;
	xpt = xpt - lspace;

	perTwip = (rightX - leftX) / (double)m_rcGraph.Width();
	CString strTmp;
	double step = 0;
	if (flag == PAYOFF_JISU1)
	{
		step = 0.1 ;
		strTmp.Format("%.1lf", leftX + (double)xpt * perTwip);
	}
	else if (flag == PAYOFF_DAY)
	{
		step = 1.0;
		strTmp.Format("%.0lf", leftX + (double)xpt * perTwip);
	}
	else if (flag == PAYOFF_CHANGE)
	{
		step = 1.0;
		strTmp.Format("%.0lf", leftX + (double)xpt * perTwip);
	}

	double	mouseVal;
	mouseVal = atof(strTmp) + (m_MouseValCnt - 1) * step / 2.0 ;

	for (int ii = 0 ; ii < m_MouseValCnt ; ii++)
	{
		strTmp.Format("%.1lf", (mouseVal - (double)ii * step) * 100.0);
		arStr[ii] = strTmp;
		kpt[ii] = atof(strTmp) / 100.0;
		rValue[ii] = 0;
	}

	switch (flag)
	{
	case PAYOFF_JISU1:
		{
			for (int jj = 0 ; jj < m_MouseValCnt ; jj++)
			{
				if (kpt[jj] >= leftX && kpt[jj] < rightX)
				{
					switch (mingam)
					{
					case SONIK:
						{
							if (m_gLine[flag][mingam].GetSize() > 0)
							{
								const double kkk = atof(arStr[jj]);
								const int idx = (int)((kkk - leftX * 100.0) / JISU_UNIT / 100.0);
								if (m_MinGamStatus[IRON])
									rValue[jj] = m_gLine[PAYOFF_JISU2][mingam][idx].yval;
								else
									rValue[jj] = m_gLine[flag][mingam][idx].yval;
							}
						}
						break;
					case DELTA:
					case GAMMA:
					case CETA:
					case VEGA:
						rValue[jj] = GetMinGamCal(flag, mingam, kpt[jj]);
						break;
					}
				}
				else
				{
					kpt[jj] = -1;
				}
			}
		}
		break;

	case PAYOFF_DAY:
		{
			for (int jj = 0 ; jj < m_MouseValCnt ; jj++)
			{
				if (kpt[jj] <= leftX && kpt[jj] >= rightX)
				{
					switch (mingam)
					{
					case SONIK:
						rValue[jj] = GetIronCal(flag, kpt[jj]);
						break;
					case DELTA:
					case GAMMA:
					case CETA:
					case VEGA:
						rValue[jj] = GetMinGamCal(flag, mingam, kpt[jj]);
						break;
					}
				}
				else
				{
					kpt[jj] = -1;
				}
			}
		}
		break;

	case PAYOFF_CHANGE:
		{
			for (int jj = 0 ; jj < m_MouseValCnt ; jj++)
			{
				if (kpt[jj] >= leftX && kpt[jj] <= rightX)
				{
					switch (mingam)
					{
					case SONIK:
							rValue[jj] = GetIronCal(flag, kpt[jj]);
						break;
					case DELTA:
					case GAMMA:
					case CETA:
					case VEGA:
							rValue[jj] = GetMinGamCal(flag, mingam, kpt[jj]);
						break;
					}
				}
				else
				{
					kpt[jj] = -1;
				}
			}
		}
		break;
	}

	_MouseValue	MouseValue[20];
	if (flag == PAYOFF_JISU1)
	{
		for (int kk = 0 ; kk < m_MouseValCnt ; kk++)
		{
			if (kpt[kk] < 0)
			{
				MouseValue[kk].kpt = _T("");
				MouseValue[kk].sValue = _T("");
			}
			else
			{
				MouseValue[kk].kpt = StringModify(kpt[kk], 5);

				if(!m_chkyetak)
				{
					if (mingam == SONIK)
						//MouseValue[kk].sValue.Format("%9s/%9s", 
						//	StringModify(rValue[kk] + m_Total, mingam), 
						//	StringModify(rValue2[kk] + m_Total, mingam));
						MouseValue[kk].sValue =	StringModify(rValue[kk] + m_Total, mingam);
					else
						MouseValue[kk].sValue = StringModify(rValue[kk], mingam);
				}
				else
				{
					if (mingam == SONIK)
						//MouseValue[kk].sValue.Format("%9s/%9s", 
						//	StringModify(rValue[kk] + m_Total, mingam), 
						//	StringModify(rValue2[kk] + m_Total, mingam));
						MouseValue[kk].sValue =	StringModify(rValue[kk] + m_Total, mingam);
					else
						MouseValue[kk].sValue = StringModify(rValue[kk], mingam);
				}
			}
		}
	}
	else
	{
		for (int kk = 0 ; kk < m_MouseValCnt ; kk++)
		{
			if ((flag != PAYOFF_CHANGE && kpt[kk] < 0) || (flag == PAYOFF_CHANGE && kpt[kk] == -1.0 && rValue[kk] == 0))
			{
				MouseValue[kk].kpt = _T("");
				MouseValue[kk].sValue = _T("");
			}
			else
			{
				MouseValue[kk].kpt = StringModify(kpt[kk], 5);

				if(!m_chkyetak)
				{
					if (mingam == SONIK)
						MouseValue[kk].sValue = StringModify(rValue[kk] + m_Total, mingam);
					else
						MouseValue[kk].sValue = StringModify(rValue[kk], mingam);
				}
				else
				{
					if (mingam == SONIK)
						MouseValue[kk].sValue = StringModify(rValue[kk] + m_Total, mingam);
					else
						MouseValue[kk].sValue = StringModify(rValue[kk], mingam);
				}
			}
		}
	}
	GetParent()->SendMessage(WM_POSMSVAL, (WPARAM) &MouseValue, 0);
}


void CGrpWnd::GetSonik2(int flag, int mingam, int xpt)
{
	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	double	rValue=0, kpt=0, perTwip=0;

	CRect rc;
	GetClientRect(&rc);

	const int lspace = m_rcGraph.left - rc.left +1;
	xpt = xpt - lspace;

	perTwip = (rightX - leftX) / (double)m_rcGraph.Width();
	CString strTmp;
	strTmp.Format("%.1lf", leftX + (double)xpt * perTwip);

	kpt = atof(strTmp);
	rValue = 0;

	switch (flag)
	{
	case PAYOFF_JISU1:
		{
			if (kpt >= leftX && kpt < rightX)
			{
				switch (mingam)
				{
				case DELTA:
					rValue  = GetMinGamCal(flag, mingam, kpt);
					m_delta = StringModify(rValue, mingam);
					break;
				case GAMMA:
					rValue  = GetMinGamCal(flag, mingam, kpt);
					m_gamma = StringModify(rValue, mingam);
					break;
				case CETA:
					rValue = GetMinGamCal(flag, mingam, kpt);
					m_seta = StringModify(rValue, mingam);
					break;
				case VEGA:
					rValue = GetMinGamCal(flag, mingam, kpt);
					m_vega = StringModify(rValue, mingam);
					break;
				}
			}
			else
			{
				kpt = -1;
			}
		}
		break;
	case PAYOFF_DAY:
		{
			if (kpt <= leftX && kpt >= rightX)
			{
				switch (mingam)
				{
				case DELTA:
					rValue  = GetMinGamCal(flag, mingam, kpt);
					m_delta = StringModify(rValue, mingam);
					break;
				case GAMMA:
					rValue  = GetMinGamCal(flag, mingam, kpt);
					m_gamma = StringModify(rValue, mingam);
					break;
				case CETA:
					rValue = GetMinGamCal(flag, mingam, kpt);
					m_seta = StringModify(rValue, mingam);
					break;
				case VEGA:
					rValue = GetMinGamCal(flag, mingam, kpt);
					m_vega = StringModify(rValue, mingam);
					break;
				}
			}
			else
			{
				kpt = -1;
			}
		}
		break;

	case PAYOFF_CHANGE:
		{
			if (kpt >= leftX && kpt <= rightX)
			{
				switch (mingam)
				{
				case DELTA:
					rValue  = GetMinGamCal(flag, mingam, kpt);
					m_delta = StringModify(rValue, mingam);
					break;
				case GAMMA:
					rValue  = GetMinGamCal(flag, mingam, kpt);
					m_gamma = StringModify(rValue, mingam);
					break;
				case CETA:
					rValue = GetMinGamCal(flag, mingam, kpt);
					m_seta = StringModify(rValue, mingam);
					break;
				case VEGA:
					rValue = GetMinGamCal(flag, mingam, kpt);
					m_vega = StringModify(rValue, mingam);
					break;
				}
			}
			else
			{
				kpt = -1;
			}
		}
		break;
	}
}

CString CGrpWnd::StringModify(double data, int mingam)
{
	CString rValue;
	CString	tmpData;
	int	pos = -1;

	switch (mingam)
	{
	case SONIK:
		tmpData.Format("%.0lf", data * SONIK_UNIT);

		if (tmpData == "-0")
			tmpData = "0";

		rValue = m_pApp->CommaModify(tmpData);
		break;
	case DELTA:
	case GAMMA:
	case CETA:
	case VEGA:
		if (data == 0)
			rValue = _T("0");
		else
		{
			rValue.Format("%.4lf", data);

			pos = rValue.Find(".");

			if (pos != -1)
			{
				tmpData = m_pApp->CommaModify(rValue.Left(pos));
				rValue  = tmpData + "." + rValue.Mid(pos +1);
			}
		}
		break;
	case 5:
		rValue.Format("%.2lf", data);
		break;
	case 6:
		rValue.Format("%.0lf", data);
		break;
	}
	return rValue;
}

int CGrpWnd::RoundDouble(double data)
{
	CString round;
	round.Format("%.0lf", data);

	return atoi(round);
}

void CGrpWnd::SetTrackMouse()
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
}

void CGrpWnd::DrawMoveLine(int xPoint, int kind)
{
	switch (kind)
	{
	case UPARROW:	// POINT
		{
			if (!CheckMove(xPoint))
			{
				return;
			}

			BoldDrawLine(xPoint, kind);
		}
		break;
	case SIZEWE:	// ALL
		{
			BoldDrawLine(xPoint, kind);

			CRect	rc, rcLeft, rcRight;
			GetClientRect(rc);

			rcLeft = rcRight = rc;
			rcLeft.right = m_rcGraph.left;
			rcRight.left = m_rcGraph.right;

			InvalidateRect(rcLeft);
			InvalidateRect(rcRight);
		}
		break;
	}
}

void CGrpWnd::BoldDrawLine(int xPoint, int kind)
{
	CDC*	pDC = GetDC();
	CPen*	oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, RGB(0, 255, 0), PS_SOLID, 1));
	pDC->SetROP2(R2_XORPEN);

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&m_rcGraph);
	pDC->SelectClipRgn(&rgn, RGN_COPY);
	rgn.DeleteObject();

	for (int ii = 0 ; ii < m_moveLine.GetSize() ; ii++)
	{
		if (ii != 0)
		{
			pDC->MoveTo(m_moveLine[ii-1].xpt, m_moveLine[ii-1].ypt-1);
			pDC->LineTo(m_moveLine[ii].xpt, m_moveLine[ii].ypt-1);

			pDC->MoveTo(m_moveLine[ii-1].xpt, m_moveLine[ii-1].ypt);
			pDC->LineTo(m_moveLine[ii].xpt, m_moveLine[ii].ypt);

			pDC->MoveTo(m_moveLine[ii-1].xpt, m_moveLine[ii-1].ypt+1);
			pDC->LineTo(m_moveLine[ii].xpt, m_moveLine[ii].ypt+1);
		}
	}

	LineCapture(xPoint, kind);

	for (int ii = 0 ; ii < m_moveLine.GetSize() ; ii++)
	{
		if (ii != 0)
		{
			pDC->MoveTo(m_moveLine[ii-1].xpt, m_moveLine[ii-1].ypt-1);
			pDC->LineTo(m_moveLine[ii].xpt, m_moveLine[ii].ypt-1);

			pDC->MoveTo(m_moveLine[ii-1].xpt, m_moveLine[ii-1].ypt);
			pDC->LineTo(m_moveLine[ii].xpt, m_moveLine[ii].ypt);

			pDC->MoveTo(m_moveLine[ii-1].xpt, m_moveLine[ii-1].ypt+1);
			pDC->LineTo(m_moveLine[ii].xpt, m_moveLine[ii].ypt+1);
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SelectClipRgn(NULL);
	ReleaseDC(pDC);
}

int CGrpWnd::GetLinePoint(int xpt)
{
	const int	flag = PAYOFF_JISU1;
	const int	mingam = SONIK;
	const double	leftX = m_Standard[flag].leftX;
	const double	rightX = m_Standard[flag].rightX;

	int	rValue = -1;
	int	cValue = -1;
	
#if	0	//kangmc	
	int	minGab = 10;
	for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
	{
		int gab = xpt - GetXpt(m_gLine[flag][mingam][ii].xval, leftX, rightX);
		if (minGab > abs(gab))
		{
			minGab = abs(gab);
			rValue = ii;
		}
	}
#else
	double	minGab = JISU_UNIT;
	double	valX = 0.;
	CRect	rc;
	GetClientRect(&rc);
	const int	gxpt = xpt - (m_rcGraph.left - rc.left);
	const double	perX = (rightX - leftX) / (double)m_rcGraph.Width();
	valX = leftX + gxpt * perX;
	if(valX >= leftX && valX <= rightX)
	{
		rValue = (int)(gxpt * perX / JISU_UNIT);
	}

	for (int ii = 0 ; ii < m_GraphData.GetSize() ; ii++) // 데이타 수
	{
		double gab = valX - m_GraphData[ii]->hsga;
		if(gab < 0)
			gab *= -1;
		if (minGab > gab)
		{
			minGab = gab;
			cValue = ii;
			m_hCursor.cidx = ii;
		}
	}

#endif

#if	1 // kangmc
	// 선물 제외
	if (cValue != -1)
	{
		if (GetMoveCode(cValue).GetAt(0) == '1' || GetMoveCode(cValue).GetAt(0) == 'A') //kangmc  rValue -1   //파생상품 코드개편
		{ 
			rValue = -1;
		}
	}
	else
		rValue = -1;
#endif
	return rValue;
}

void CGrpWnd::LineCapture(int xPoint, int kind)
{
	m_moveLine.RemoveAll();

	double	minY{}, maxY{};
	int	flag = PAYOFF_JISU1;
	int	mingam = SONIK;

	const double	leftX = m_Standard[flag].leftX;
	const double	rightX = m_Standard[flag].rightX;
	GetMinMaxY(flag, mingam, minY, maxY, MINMAX_TYPE1);

	if (m_gLine[flag][mingam].GetSize() < 0)
		return;

	switch (kind)
	{
	case UPARROW:	// POINT
		{
			int xpt[3]{}, ypt[3]{};
			const double	prev = MinGrphsga(m_hCursor.cidx);
			const double	next = MaxGrphsga(m_hCursor.cidx);
			int		idx=0;
			idx = (int)((prev - leftX) / JISU_UNIT);
			xpt[0] = GetXpt(m_gLine[flag][mingam][idx].xval, leftX, rightX);
			ypt[0] = GetYpt(m_gLine[flag][mingam][idx].yval + m_Total, minY, maxY);

			xpt[1] = xPoint;
			ypt[1] = GetYpt(m_gLine[flag][mingam][m_hCursor.point].yval + m_Total, minY, maxY);

			idx = (int)((next - leftX) / JISU_UNIT);
			xpt[2] = GetXpt(m_gLine[flag][mingam][idx].xval, leftX, rightX);
			ypt[2] = GetYpt(m_gLine[flag][mingam][idx].yval + m_Total, minY, maxY);

			for (int ii = 0 ; ii < 3 ; ii++)
			{
				_moveLine	moveLine;
				moveLine.xpt = xpt[ii];
				moveLine.ypt = ypt[ii];

				m_moveLine.Add(moveLine);
			}
		}
		break;
	case SIZEWE:	// ALL
		{
		const int	incress = xPoint - m_hCursor.startXpt;

			for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
			{
				const int xpt = GetXpt(m_gLine[flag][mingam][ii].xval, leftX, rightX);
				const int ypt = GetYpt(m_gLine[flag][mingam][ii].yval + m_Total, minY, maxY);

				_moveLine	moveLine;
				moveLine.xpt = xpt + incress;
				moveLine.ypt = ypt;

				m_moveLine.Add(moveLine);
			}
		}
		break;
	}
}

bool CGrpWnd::CheckMove(int xpt)
{
	bool	rValue = false;
	
	const int	flag = PAYOFF_JISU1;
	const int	mingam = SONIK;

	const double	leftX = m_Standard[flag].leftX;
	const double	rightX = m_Standard[flag].rightX;

	CRect	rc;
	GetClientRect(&rc);
	const int	gxpt = xpt - (m_rcGraph.left - rc.left);

	const double	perX = (rightX - leftX) / (double)m_rcGraph.Width();
	const double	valX = leftX + gxpt * perX;

	const int	nSize = m_gLine[flag][mingam].GetSize();

// kangmc 	double	prev = m_gLine[flag][mingam][m_hCursor.point - 1].xval + 2.5;
// kangmc	double	next = m_gLine[flag][mingam][m_hCursor.point + 1].xval - 2.5;

	double	prev = MinGrphsga(m_hCursor.cidx);
	double	next = MaxGrphsga(m_hCursor.cidx);
	if(next != 0)
		next -= 2.5;
	if(prev != 0)
		prev += 2.5;
	if (valX >= prev && valX <= next)
	{
		rValue = true;
	}

	m_hCursor.moveFlag = rValue;

	return rValue;
}

double CGrpWnd::MinGrphsga(int cidx)
{
	double  cck{};
	for(int ii=0; ii <  m_GraphData.GetSize() ; ii++)
		cck = m_GraphData[ii]->hsga;
	
	for(int ii=cidx; ii >= 0; ii--)
	{
		if(m_GraphData[cidx]->hsga > m_GraphData[ii]->hsga)
			return m_GraphData[ii]->hsga;
	}
	return m_Standard[PAYOFF_JISU1].leftX;
}
double CGrpWnd::MaxGrphsga(int cidx)
{
	for(int ii=cidx; ii <  m_GraphData.GetSize() ; ii++)
	{
		if(m_GraphData[cidx]->hsga < m_GraphData[ii]->hsga)
			return m_GraphData[ii]->hsga;
	}
	return m_Standard[PAYOFF_JISU1].rightX;
}

void CGrpWnd::LineCheck(int xpt, int ypt)
{
	if (m_hCursor.btnFlag)
		return;

	if (!m_rcGraph.PtInRect(CPoint(xpt, ypt)))
	{
		SetMouseCursor(ARROW);
		return;
	}

	int	flag = PAYOFF_JISU1;
	int	mingam = SONIK;

	const double	leftX = m_Standard[flag].leftX;
	const double	rightX = m_Standard[flag].rightX;
	double	valX = 0, valY = 0;

	CRect	rc;
	GetClientRect(&rc);
	const int	gxpt = xpt - (m_rcGraph.left - rc.left);

	const double	perX = (rightX - leftX) / (double)m_rcGraph.Width();
	valX = leftX + gxpt * perX;


#if	0	//kangmc
	for (int ii = 0 ; ii < m_CalData.GetSize() ; ii++)
	{
		if (valX >= m_CalData[ii].fstX && valX < m_CalData[ii].secX)
		{
			// ljh 2004.08.20
			valY = m_CalData[ii].xflt * valX + m_CalData[ii].sang + m_Total;
			break;
		}
	}
#else
	if(m_gLine[flag][mingam].GetSize() > 0)
	{
		const int	idx = (int)((valX - leftX) * 10);
		valY = m_gLine[flag][mingam][idx].yval + m_Total;
	}
#endif
//	TRACE("valY : [%lf]\n", valY);
	
	double	minY, maxY;
	GetMinMaxY(flag, mingam, minY, maxY, MINMAX_TYPE1);

	const double	perY = (double)m_rcGraph.Height() / (maxY - minY);
	const int	gypoint = RoundDouble(((double)m_rcGraph.bottom - (perY * (valY - minY))));

	if (ypt >= gypoint - 3 && ypt <= gypoint + 3)
	{
		bool	lFlag = true;

		// 시작점과 끝점 제외
		const int	nSize = m_gLine[flag][mingam].GetSize();
		for (int jj = 1 ; jj < nSize - 1 ; jj++)
		{
			const int	gxpoint = GetXpt(m_gLine[flag][mingam][jj].xval, leftX, rightX);

			if (xpt >= gxpoint - 3 && xpt <= gxpoint + 3)
			{
				// 선물제외
				if (GetLinePoint(xpt) != -1)
				{
					SetMouseCursor(UPARROW);
					lFlag = false;
					break;
				}
			}
		}

		if (lFlag)
		{
			SetMouseCursor(SIZEWE);
		}
	}
	else
	{
		SetMouseCursor(ARROW);
	}
}

CString CGrpWnd::GetMoveCode(int point)
{
	CString rValue, hsga = _T(""), tmp = _T(""), jcod;

	CArray <CString, CString> hsgaArr;
	for (int ii = 0 ; ii < m_GraphData.GetSize() ; ii++)
	{
		jcod = m_GraphData[ii]->code;
		tmp = jcod.Right(3);
		if (ii == 0)
		{
			hsga = tmp;
			hsgaArr.Add(jcod);
		}
		else
		{
			if (hsga.Compare(tmp) != 0)
			{
				hsga = tmp;
				hsgaArr.Add(jcod);
			}
		}
	}

	if (hsgaArr.GetSize() > point)
		return hsgaArr.GetAt(point);
	else
		return "1";
}

CString CGrpWnd::GetMoveGab(int spt, int ept)
{
	const double	leftX = m_Standard[PAYOFF_JISU1].leftX;
	const double	rightX = m_Standard[PAYOFF_JISU1].rightX;
	const double	perX = (rightX - leftX) / (double)m_rcGraph.Width();

	CRect	rc;
	GetClientRect(&rc);

	int	xpt;
	xpt = spt - (m_rcGraph.left - rc.left);
	const double	startVal = leftX + xpt * perX;

	xpt = ept - (m_rcGraph.left - rc.left);
	const double	endVal = leftX + xpt * perX;

	const double	gab = endVal - startVal;

	CString strGab;
	strGab.Format("%.0f", gab);

	CString prev = strGab.Left(strGab.GetLength() - 1);
	CString itmp = strGab.Right(1);
	
	CString	strVal = _T("");
	double	douVal = 0;
	switch (atoi(itmp))
	{
	case 0:
	case 1:
		strVal.Format("%s%s", prev, "0");
		douVal = atof(strVal);
		break;
	case 2:
	case 3:
		strVal.Format("%s%s", prev, "2.5");
		douVal = atof(strVal);
		break;
	case 4:
	case 5:
	case 6:
		strVal.Format("%s%s", prev, "5");
		douVal = atof(strVal);
		break;
	case 7:
	case 8:
		strVal.Format("%s%s", prev, "7.5");
		douVal = atof(strVal);
		break;
	case 9:
		strVal.Format("%s%s", prev, "0");
		if (prev.Left(1) == '-')
			douVal = atof(strVal) - 10;
		else
			douVal = atof(strVal) + 10;
		break;
	}

	CString	rValue;
	rValue.Format("%.1f", douVal);

	return rValue;
}

void CGrpWnd::SetMouseCursor(int kind)
{
	if (!m_bMoveStg) return;

	switch (kind)
	{
	case ARROW:
		m_hCursor.btnFlag = false;
		m_hCursor.cursor = NULL;
		break;
	case UPARROW:
		m_hCursor.cursor = LoadCursor(NULL, IDC_UPARROW);
		break;
	case SIZEWE:
		m_hCursor.cursor = LoadCursor(NULL, IDC_SIZEWE);
		break;
	}
	m_hCursor.kind = kind;

	this->PostMessage(WM_SETCURSOR);
}

double CGrpWnd::SizeCompare(double data, double min, double max)
{
	double rValue = 0;

	if (min <= data && max >= data)
		rValue = data;
	else if (min > data)
		rValue = min;
	else if (max < data)
		rValue = max;
	else
	{
		CString msg;
		msg.Format("SizeCompare >> min(%f), max(%f)", min, max);
		MessageBox(msg);
	}
	
	return rValue;
}

void CGrpWnd::CalMangi(int flag, int mingam)
{
	if (m_gLine[flag][mingam].GetSize() > 0)
		return;

	const double leftX = m_Standard[flag].leftX;
	const double rightX = m_Standard[flag].rightX;

	const int nSize = m_GraphData.GetSize();

	for(int ii = 0 ; ii <= nSize ; ii++) // 기울기 구간
	{		
		m_CalData[ii].xflt = 0;
		m_CalData[ii].sang = 0;

		if (ii == 0)
		{			
			m_CalData[ii].fstX = leftX;
			m_CalData[ii].secX = SizeCompare(m_GraphData[ii]->hsga, leftX, rightX);
		}
		else if (ii == nSize)
		{
			m_CalData[ii].fstX = SizeCompare(m_GraphData[ii-1]->hsga, leftX, rightX);
			m_CalData[ii].secX = rightX;
		}
		else
		{
			m_CalData[ii].fstX = SizeCompare(m_GraphData[ii-1]->hsga, leftX, rightX);
			m_CalData[ii].secX = SizeCompare(m_GraphData[ii]->hsga, leftX, rightX);
		}

		for(int jj = 0 ; jj < nSize ; jj++) // 데이타 수
		{
			GetCalMangi(nSize, ii, jj);
		}
	}
	GuganValue();
}

void CGrpWnd::GetCalMangi(int nSize, int ii, int jj)
{
	double	stmp = 0;
	double	dbjisu = 0;
	//double	bPrice = 0;
	
	dbjisu = m_k200;

	switch (m_GraphData[jj]->gubn)
	{
	case 1: // 선물매도
		// 기울기 : y = (-x + 선물매입가)*5 * 수량
		m_CalData[ii].xflt = m_CalData[ii].xflt - 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2016.04.14 KSJ 미니는 /5 ;
		m_CalData[ii].sang = m_CalData[ii].sang + m_GraphData[jj]->curr * m_GraphData[jj]->numb;
		stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
		m_CalData[ii].sang = m_CalData[ii].sang + stmp;
		//TRACE("[tnp2]%f\t[sang2]%f\n", stmp, m_CalData[ii].sang);
		break;
	case 2: // 선물매수
		// 기울기 : y = (x - 선물매입가)*5 * 수량
		m_CalData[ii].xflt = m_CalData[ii].xflt + 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2016.04.14 KSJ 미니는 /5 ;
		m_CalData[ii].sang = m_CalData[ii].sang - m_GraphData[jj]->curr * m_GraphData[jj]->numb;
		stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
		m_CalData[ii].sang = m_CalData[ii].sang + stmp;
		break;
	case 3: // 콜매도
		// 처음
		if (ii == 0)
		{
			if (m_GraphData[ii]->hsga - 0.01 < m_GraphData[jj]->hsga)
			{
				// 기울기 : y = premium * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
				m_CalData[ii].sang = m_CalData[ii].sang + m_GraphData[jj]->curr * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}
		// 중간구간들
		if (ii > 0 && ii < nSize)
		{
			if (m_GraphData[ii]->hsga - 0.01 > m_GraphData[jj]->hsga)
			{
				// 기울기 : y = (-x + premium + 행사가) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + -1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;	//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang + (m_GraphData[jj]->curr + m_GraphData[jj]->hsga) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
			else
			{
				// 기울기 : y = premium * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
				m_CalData[ii].sang = m_CalData[ii].sang + m_GraphData[jj]->curr * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}
		// 마지막
		if (ii == nSize)
		{
			if (m_GraphData[ii - 1]->hsga + 0.01 > m_GraphData[jj]->hsga)
			{
				// 기울기 : y = (-x + premium + 행사가) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + -1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang + (m_GraphData[jj]->curr + m_GraphData[jj]->hsga) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}
		break;
	case 4: // 콜매수
		// 처음
		if (ii == 0)
		{
			if (m_GraphData[ii]->hsga - 0.01 < m_GraphData[jj]->hsga)
			{
				// 기울기 : y = -premium * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
				m_CalData[ii].sang = m_CalData[ii].sang - m_GraphData[jj]->curr * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}
                    
		// 중간구간들
		if (ii > 0 && ii < nSize)
		{
			if (m_GraphData[ii]->hsga - 0.01 > m_GraphData[jj]->hsga)
			{
				// 기울기 : y = (x - (premium + 행사가)) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang - (m_GraphData[jj]->curr + m_GraphData[jj]->hsga) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
			else
			{
				// 기울기 : y = -premium * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
				m_CalData[ii].sang = m_CalData[ii].sang - m_GraphData[jj]->curr * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}

		// 마지막
		if (ii == nSize)
		{
			// 기울기 : y = (x - (premium + 행사가)) * 수량
			m_CalData[ii].xflt = m_CalData[ii].xflt + 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
			m_CalData[ii].sang = m_CalData[ii].sang - (m_GraphData[jj]->curr + m_GraphData[jj]->hsga) * m_GraphData[jj]->numb;
			stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
			m_CalData[ii].sang = m_CalData[ii].sang + stmp;
		}
		break;

	case 5: // 풋매도
		// 처음
		if (ii == 0)
		{
			if (m_GraphData[ii]->hsga - 0.01 < m_GraphData[jj]->hsga)
			{
				// 기울기 : y = (x - (행사가 - premium)) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang - (m_GraphData[jj]->hsga - m_GraphData[jj]->curr) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}
		// 중간구간들
		if (ii > 0 && ii < nSize)
		{
			if (m_GraphData[ii]->hsga - 0.01 > m_GraphData[jj]->hsga)
			{
				// 기울기 : y = premium * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
				m_CalData[ii].sang = m_CalData[ii].sang + m_GraphData[jj]->curr * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
			else
			{
				// 기울기 : y = (x - (premium - 행사가)) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang - (m_GraphData[jj]->hsga - m_GraphData[jj]->curr) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}
		// 마지막
		if (ii == nSize)
		{
			// 기울기 : y = premium * 수량
			m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
			m_CalData[ii].sang = m_CalData[ii].sang + m_GraphData[jj]->curr * m_GraphData[jj]->numb;
			stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
			m_CalData[ii].sang = m_CalData[ii].sang + stmp;
		}
		break;

	case 6: // 풋매수
		// 처음
		if (ii == 0)
		{
			if (m_GraphData[ii]->hsga - 0.01 < m_GraphData[jj]->hsga)
			{
				// 기울기 : y = (-x + (행사가 - premium)) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt - 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang + (m_GraphData[jj]->hsga - m_GraphData[jj]->curr) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}

		// 중간구간들
		if (ii > 0 && ii < nSize)
		{
			if (m_GraphData[ii]->hsga - 0.01 > m_GraphData[jj]->hsga)
			{
				// 기울기 : y = -premium * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
				m_CalData[ii].sang = m_CalData[ii].sang - m_GraphData[jj]->curr * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
			else
			{
				// 기울기 : y = (-x + (행사가 - premium)) * 수량
				m_CalData[ii].xflt = m_CalData[ii].xflt - 1 * m_GraphData[jj]->numb * m_GraphData[jj]->gdan;//2012.05.16 KSJ 콜옵션풋옵션 5배로 됨
				m_CalData[ii].sang = m_CalData[ii].sang + (m_GraphData[jj]->hsga - m_GraphData[jj]->curr) * m_GraphData[jj]->numb;
				stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
				m_CalData[ii].sang = m_CalData[ii].sang + stmp;
			}
		}

		// 마지막
		if (ii == nSize)
		{
			// 기울기 : y = -premium * 수량
			m_CalData[ii].xflt = m_CalData[ii].xflt + 0;
			m_CalData[ii].sang = m_CalData[ii].sang - m_GraphData[jj]->curr * m_GraphData[jj]->numb;
			stmp = CalFee(false, m_GraphData[jj]->gubn, m_CalData[ii].xflt);
			m_CalData[ii].sang = m_CalData[ii].sang + stmp;
		}
		break;
	}
}

bool CGrpWnd::GetMinMaxY(int flag, int mingam, double &MinY, double &MaxY, int type)
{
	double min=0, max=0, range=0;

	if (flag == 0)
	{
		for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
		{
			if (ii == 0)
			{
				min = m_gLine[flag][mingam][ii].yval + m_Total;
				max = m_gLine[flag][mingam][ii].yval + m_Total;
			}
			else
			{
				if (min > m_gLine[flag][mingam][ii].yval + m_Total)
					min = m_gLine[flag][mingam][ii].yval + m_Total;
				if (max < m_gLine[flag][mingam][ii].yval + m_Total)
					max = m_gLine[flag][mingam][ii].yval + m_Total;
			}
		}
	}
	else
	{
		for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
		{
			if (ii == 0)
			{
				min = m_gLine[flag][mingam][ii].yval + m_Total;
				max = m_gLine[flag][mingam][ii].yval + m_Total;
			}
			else
			{
				if (min > m_gLine[flag][mingam][ii].yval + m_Total)
					min = m_gLine[flag][mingam][ii].yval + m_Total;
				if (max < m_gLine[flag][mingam][ii].yval + m_Total)
					max = m_gLine[flag][mingam][ii].yval + m_Total;
			}
		}
	}

	range = max - min;
	// 간격이 좁으니.. 확대..
	if (range < 0.01)
	{
		if (fabs(max) < 0.1) range = 1;
		else if (fabs(max) < 0.5) range = 5;
		else if (fabs(max) < 1) range = 10;
		else if (fabs(max) < 5) range = 50;
		else if (fabs(max) < 10) range = 100;
		else range = 500;
	}
//	TRACE(" >> %lf\n", range);
	if (range != 0)
	{
		switch (type)
		{
		case MINMAX_TYPE1:
			break;

		case MINMAX_TYPE2:
			{
				if (fabs(max) > fabs(min))
					min = -1.0 * max;
				else if (fabs(max) < fabs(min))
					max = -1.0 * min;
			}
			break;
		}
	}
	else
	{
		if (max < 10)
		{
			max = max + 10;
			min = min - 10;
		}
		else
		{
			max = max + 100;
			min = min - 100;
		}
	}

	double pyValue = range / (double)m_rcGraph.Height();
	MinY = min - (10.0 * pyValue);// + m_Total;	// ljh
	MaxY = max + (10.0 * pyValue);// + m_Total;	// ljh

	return TRUE;
}

void CGrpWnd::SetText(CDC* pDC, CRect rect, CString strInput, COLORREF txColor, UINT fmt)
{
	const COLORREF	color = pDC->SetTextColor(txColor);
	const UINT		deffmt = DT_SINGLELINE | DT_VCENTER | fmt;
	const int		nMode = pDC->SetBkMode(TRANSPARENT);

	pDC->DrawText(strInput, rect, deffmt);
	pDC->SetBkMode(nMode);
	pDC->SetTextColor(color);
}

void CGrpWnd::DrawBox(CDC* pDC, COLORREF nOutLine, COLORREF nBkColor, CRect rect, UINT nLine, BOOL bFill)
{	
	if (rect == CRect(-1, -1, -1, -1))
		GetClientRect(rect);		
	
	if (bFill)
		pDC->FillSolidRect(rect, nBkColor);	

	CPen *oldpen;
	oldpen = pDC->SelectObject(m_pApp->GetPen(m_pView, nOutLine, PS_SOLID, 1));

	if (nLine & BOX_LEFT)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.left, rect.bottom);
	}

	if (nLine & BOX_TOP)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(rect.right, rect.top);
	}

	if (nLine & BOX_BOTTOM)
	{
		pDC->MoveTo(rect.right, rect.bottom);
		pDC->LineTo(rect.left, rect.bottom);
	}

	if (nLine & BOX_RIGHT)
	{
		pDC->MoveTo(rect.right, rect.bottom);
		pDC->LineTo(rect.right, rect.top);
	}

	pDC->SelectObject(oldpen);
}

int CGrpWnd::GetTickCount(CDC* pDC)
{
	int	dispTICK = 5;
	int	tkHeight;
	const CSize size = pDC->GetTextExtent("9");

	tkHeight = m_rcGraph.Height();
	dispTICK = int(tkHeight / (size.cy * 1.2));
	
	if (dispTICK > 15)
		dispTICK = 15;

	return dispTICK;
}

bool CGrpWnd::calculateDisplay(double &div, double Max, double Min, double& drawMinValue, 
		int dispTICK, double* dispValue, int& dispCount)
{
	int	index=0;
	CString	temp;
	double	mok = Min / div;
	temp.Format("%f", mok);
	index = temp.Find('.'); 
	mok = atof(temp.Left(index));
	drawMinValue = mok * div;
	if (drawMinValue < Min)		
		drawMinValue += div;
	const int	drawNum = int ((Max - drawMinValue) / div);
	if (drawNum >= dispTICK)
	{
		if (drawNum <= dispTICK*2)
			div *= 2;
		else if (drawNum <= dispTICK*3)
			div *= 3;
		else if (drawNum <= dispTICK*4)
			div *= 4;
		else
			div *= 5;

		mok = Min / div;
		temp.Format("%f", mok);
		index = temp.Find('.'); 
		mok = atof(temp.Left(index));
		drawMinValue = mok * div;
		if (drawMinValue < Min)		
			drawMinValue += div;
	}
	dispCount = 0;
	dispValue[dispCount] = drawMinValue;
	const double dVal = 0.0;
	while(1)
	{
		if (dispValue[dispCount] > Max + 0.01)
			break;

		dispValue[dispCount+1] = dispValue[dispCount] + div;
		dispCount++;
	}
	if (dispCount <= 0)
	{
		if (Max == Min)
		{
			dispValue[0] = Max;
			dispCount = 1;
		}
		else
			return false;
	}
	return true;
}


int CGrpWnd::GetDrawTickValue(CDC* pDC, double* pValue, double Max, double Min)
{
	int	dispCount=0;
	const int	tickHeight = m_rcGraph.Height();
	double	ValueHeight = Max - Min;

	if (ValueHeight <= 0) 	ValueHeight = 1;
	
	double	div=0;
	if (ValueHeight < 5)			div = 1;
	else if (ValueHeight < 20)		div = 2;
	else if (ValueHeight < 50)		div = 5;
	else if (ValueHeight < 100)		div = 10;
	else if (ValueHeight < 200)		div = 20;
	else if (ValueHeight < 500)		div = 50;
	else if (ValueHeight < 1000)		div = 100;
	else if (ValueHeight < 2000)		div = 200;
	else if (ValueHeight < 5000)		div = 500;
	else if (ValueHeight < 10000)		div = 1000;
	else if (ValueHeight < 20000)		div = 2000;
	else if (ValueHeight < 50000)		div = 5000;
	else if (ValueHeight < 100000)		div = 10000;
	else if (ValueHeight < 200000)		div = 20000;
	else if (ValueHeight < 500000)		div = 50000;
	else if (ValueHeight < 1000000)		div = 100000;
	else if (ValueHeight < 2000000)		div = 200000;
	else if (ValueHeight < 5000000)		div = 500000;
	else if (ValueHeight < 10000000)	div = 1000000;
	else if (ValueHeight < 20000000)	div = 2000000;
	else if (ValueHeight < 50000000)	div = 5000000;
	else if (ValueHeight < 100000000)	div = 10000000;
	else if (ValueHeight < 200000000)	div = 20000000;
	else if (ValueHeight < 500000000)	div = 50000000;
	else if (ValueHeight < 1000000000)	div = 100000000;
	else					div = 500000000;

	const int	dispTICK = GetTickCount(pDC);
	if (dispTICK <= 0)
		return 0;

	double	drawMinValue= 0.0;
	const double	dVal = 0.0;

// 	CString strTemp;
// 	strTemp.Format("[KSJ]div[%f]max[%f]Min[%f]drawMinValue[%f]dispTICK[%d]dispCount[%d]", div, Max, Min, drawMinValue, dispTICK, dispCount);
// 	OutputDebugString(strTemp);

	if (!calculateDisplay(div, Max, Min, drawMinValue, dispTICK, pValue, dispCount))
		return 0;

	return dispCount;	
}

long CGrpWnd::OnMsgSetParam(WPARAM wparam, LPARAM lparam)
{
	CString str, val;
	switch (wparam)
	{
	case GetJisuStart:
		m_rtnStr.Format("%.2f", m_jisuStart);
		return (long)(LPTSTR)(LPCTSTR)m_rtnStr;
	case GetJisuEnd:
		m_rtnStr.Format("%.2f", m_jisuEnd);
		return (long)(LPTSTR)(LPCTSTR)m_rtnStr;
	case SetJisuStart:
		str.Format("%s", (char*)lparam);
		m_jisuStart = atof(str);
		break;
	case SetJisuEnd:
		str.Format("%s", (char*)lparam);
		m_jisuEnd = atof(str);
		break;
	case SetJjilMove:
		str.Format("%s", (char*)lparam);
		m_jjilMove = atof(str);
		break;
	//case SetCallPutVolMove:
	//	str.Format("%s", (char*)lparam);
	//	val.Format("%s", m_pApp->Parser(str, cTAB));	m_callVolMove = atof(val);
	//	val.Format("%s", m_pApp->Parser(str, cTAB));	m_putVolMove = atof(val);
		break;
	}	
	return 0;
}

void CGrpWnd::GuganValue()
{
	if (m_GraphData.GetSize() < 0)
		return;

	const int	size = m_CalData.GetSize();
	int	ii{};

	CString tmp;
	CString allmax;
	CString	allmin;

	double	max{};
	double	min{};
	double	cmp{};

	struct stChkLayer
	{
		double	jisu{};	// 0:Normal, 1:Top, 2:Bottom
		double	iron{};
	};

	stChkLayer chkLayer[100]{};

	CString strTemp;

	chkLayer[0].jisu = m_CalData[0].fstX;
	chkLayer[0].iron = GetIronCal(PAYOFF_JISU1, m_CalData[0].fstX);
	max = chkLayer[0].iron;
	min = chkLayer[0].iron;
	for (ii = 0; ii < size; ii++)
	{
		chkLayer[ii+1].jisu = m_CalData[ii].secX;
		cmp = GetIronCal(PAYOFF_JISU1, m_CalData[ii].secX);
		chkLayer[ii+1].iron = cmp;

		if (max < cmp)
			max = cmp;
		
		if (min > cmp)
			min = cmp;
//		KSJ 처음 조회했을때와 두번째 조회했을때에 결과값이 달라짐..
// 		strTemp.Format("[KSJ]secX[%f]cmp[%f]max[%f]min[%f]", m_CalData[ii].secX, cmp, max, min);
// 		OutputDebugString(strTemp);

	}
	m_Maxiik.Format("%s", StringModify(max, MINGAM_SONIK));
	m_Miniik.Format("%s", StringModify(min, MINGAM_SONIK));

// 	strTemp.Format("[KSJ]size[%d]miniik[%f][%s]maxiik[%f][%s]", size, min, m_Miniik, max, m_Maxiik );
// 	OutputDebugString(strTemp);

	if (StringModify(max) == StringModify(min))
	{
		if (max > 0)
		{
			m_allmax = StringModify(m_CalData[0].fstX) + "~" + StringModify(m_CalData[size - 1].secX);
			m_allmin = _T("0");
		}
		else
		{
			m_allmax = _T("0");
			m_allmin = StringModify(m_CalData[0].fstX) + "~" + StringModify(m_CalData[size - 1].secX);
			
		}
		return;
	}

	m_allmax = "";
	m_allmin = "";
	bool	bContinueMax{}, bContinueMin{};
	bContinueMax = false;
	bContinueMin = false;
	CString strMax(""), strMin("");
	for (ii = 0; ii < size + 1; ii++)
	{
		// max Check
		if (StringModify(chkLayer[ii].iron) == StringModify(max))
		{
			if (strMax.IsEmpty())
			{
				strMax = StringModify(chkLayer[ii].jisu);
			}
			else
			{
				if (strMax != StringModify(chkLayer[ii].jisu))
					bContinueMax = true;	
			}
		}
		else
		{
			if (bContinueMax)
				strMax = strMax + "~" + StringModify(chkLayer[ii-1].jisu);
	
			if (!strMax.IsEmpty())
				m_allmax = m_allmax + " " + strMax;

			strMax.Empty();
			bContinueMax = false;
		}

		// min Check
		if (StringModify(chkLayer[ii].iron) == StringModify(min))
		{
			if (strMin.IsEmpty())
			{
				strMin = StringModify(chkLayer[ii].jisu);
			}
			else
			{
				if (strMin != StringModify(chkLayer[ii].jisu))
					bContinueMin = true;	
			}
		}
		else
		{
			if (bContinueMin)
				strMin = strMin + "~" + StringModify(chkLayer[ii-1].jisu);
	
			if (!strMin.IsEmpty())
				m_allmin = m_allmin + " " + strMin;

			strMin.Empty();
			bContinueMin = false;
		}
	}

	if (!strMax.IsEmpty())
	{
		if (bContinueMax)
		{
			strMax = strMax + "~" + StringModify(chkLayer[size].jisu);
			m_allmax = m_allmax + " " + strMax;
		}
		else
		{
			m_allmax = m_allmax + " " + strMax;
		}
	}

	if (!strMin.IsEmpty())
	{
		if (bContinueMin)
		{
			strMin = strMin + "~" + StringModify(chkLayer[size].jisu);
			m_allmin = m_allmin + " " + strMin;
		}
		else
		{
			m_allmin = m_allmin + " " + strMin;
		}
	}
}
/*
void CGrpWnd::MaxMinValue(int flag, int mingam)
{
	double	curryValue = 0;
	double	currxValue = 0;
	double	Maxiik     = 0;
	double	Miniik	   = 9999999;
	double	MaxX	   = 0;
	
	for (int ii = 0 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
	{
		if (m_gLine[flag][mingam].GetSize() > 0 && ii >= 0)
		{
			curryValue = m_gLine[flag][mingam][ii].yval + m_Total;
			currxValue = m_gLine[flag][mingam][ii].xval;
		}
		Maxiik = _KMAX(Maxiik, curryValue);
		Miniik = _KMIN(Miniik, curryValue);
	}
CString sTest;
sTest.Format("IC Maxiik:%f Miniik:%f\n", Maxiik, Miniik);
OutputDebugString(sTest);

	m_Maxiik.Format("%s", StringModify(Maxiik, MINGAM_SONIK));
	m_Miniik.Format("%s", StringModify(Miniik, MINGAM_SONIK));
}
*/
void CGrpWnd::BungiGap(int flag, int mingam)
{
	CString tmp, stmp;
	CString	perGiho;
	CString	currGiho;
	double	peryValue  = 0;
	double	curryValue = 0;
	double	perxValue  = 0;
	double	currxValue = 0;

	m_bunkiCount[flag] = 0;

	for (int ii = 1 ; ii < m_gLine[flag][mingam].GetSize() ; ii++)
	{
		if (m_gLine[flag][mingam].GetSize() > 0 && ii > 0)
		{
			peryValue  = m_gLine[flag][mingam][ii - 1].yval + m_Total;
			curryValue = m_gLine[flag][mingam][ii].yval + m_Total;
			
			perxValue  = m_gLine[flag][mingam][ii - 1].xval;
			currxValue = m_gLine[flag][mingam][ii].xval;

			if (curryValue == 0)
			{
				BungiCal(flag, m_bunkiCount[flag], perxValue, currxValue);
				m_bunkiCount[flag]++;
//TRACE("[cnt]%d\t[xval1]%f\t[xval2]%f\n", m_bunkiCount, perxValue, currxValue);
			}
			else if (peryValue > 0)
			{
				if (curryValue < 0)
				{
					BungiCal(flag, m_bunkiCount[flag], perxValue, currxValue);
					m_bunkiCount[flag]++;
//TRACE("[cnt]%d\t[xval1]%f\t[xval2]%f\n", m_bunkiCount, perxValue, currxValue);
				}
			}
			else if (peryValue < 0)
			{
				if (curryValue > 0)
				{
					BungiCal(flag, m_bunkiCount[flag], perxValue, currxValue);
					m_bunkiCount[flag]++;
//TRACE("[cnt]%d\t[xval1]%f\t[xval2]%f\n", m_bunkiCount, perxValue, currxValue);
				}
			}
		}
	}
	if (m_bunkiCount[flag] > 10)
		m_bunkiCount[flag] = 0;
	else if (m_bunkiCount[flag] > 4)
		m_bunkiCount[flag] = 4;
}

void CGrpWnd::BungiCal(int flag, int cnt, double leftX, double rightX)
{
	CString	tmp, sValue;
	double	peryVal = 0;
	double	curryVal = 0;
	bool	bunki = false;
	CString	Zero;

	for (double ii = leftX ; ii <= rightX ; ii = ii + 0.01)
	{
		peryVal  = GetIronCal(flag, ii - 0.01);	
		curryVal = GetIronCal(flag, ii);
//TRACE("[%d]\t%f\t%f\t%f\n", cnt, peryVal, curryVal, ii);

		Zero.Format("%.02f", curryVal);

		if (Zero == "0.00" || Zero == "-0.00")
		{
			sValue.Format("%.02f", ii);
			bunki = true;
			break;
		}
		else if (peryVal > 0)
		{
			if (curryVal < 0)
			{
				sValue.Format("%.02f", ii);
				bunki = true;
				break;
			}
		}
		else if (peryVal < 0)
		{
			if (curryVal > 0)
			{
				sValue.Format("%.02f", ii);
				bunki = true;
				break;
			}
		}		
	}

	if (cnt < 4 && bunki)
		m_bunkiValue[flag][cnt] = sValue;
}

double CGrpWnd::GetJJis(double jjis)
{
//CString sTest;
//sTest.Format("IC ((CMapWnd*)m_pParent)->m_timeOpt:%d jjis:%f m_jjilMove:%f", ((CMapWnd*)m_pParent)->m_timeOpt, jjis, m_jjilMove);
//OutputDebugString(sTest);
	if (jjis + m_jjilMove <= 0)
		return 0.00001;
	else
	{
		if (((CMapWnd*)m_pParent)->m_timeOpt ==1) // 장중시간가치 감소적용
		{
			CString stimeStr, etimeStr, ctimeStr;
			stimeStr.Format("%s", ((CMapWnd*)m_pParent)->m_timeStart);
			etimeStr.Format("%s", ((CMapWnd*)m_pParent)->m_timeEnd);
			ctimeStr.Format("%s", ((CMapWnd*)m_pParent)->m_bData->time);

			double stime=0, etime=0, ctime=0, timeloss = 0;
			if (ctimeStr.Find("장마감") >= 0)
				ctimeStr = "1500";
			if (ctimeStr.Find("장종료") >= 0)
				ctimeStr = "1600";

			stime = GetTimeMin(stimeStr);
			etime = GetTimeMin(etimeStr);
			ctime = GetTimeMin(ctimeStr);

			if (ctime > stime)
			{
				if (ctime  > etime)
					ctime = etime;
				if (stime < etime)
					timeloss = (ctime - stime) / (etime - stime);
				jjis = jjis - timeloss;
			}
		}

		return jjis + m_jjilMove;
	}
}

int CGrpWnd::GetTimeMin(CString sTime)
{
	int hh=0, mm=0;
	sTime.TrimRight();
	sTime.Replace(":", "");
	if (sTime.GetLength() < 4)
		return 0;
	hh = atoi(sTime.Left(2));
	mm = atoi(sTime.Mid(2, 2));
	return hh * 60 + mm;
}

