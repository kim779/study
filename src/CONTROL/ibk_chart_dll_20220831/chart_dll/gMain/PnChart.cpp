// PnChart.cpp : implementation file
//

#include "stdafx.h"
#include "axisgmain.h"
#include "PnChart.h"
#include "ObjMgr.h"
#include "ObjEvent.h"
#include "GrpGrid.h"
#include "ToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "excel.h"
#include "../../h/axisvar.h"
/////////////////////////////////////////////////////////////////////////////
// CPnChart

CPnChart::CPnChart()
{
	m_pObjMgr = NULL;
	m_strRtnData = _T(""); // 2006.11.20 lsh
	//	m_bProc = false;	// test

	InitStanLine();
}

CPnChart::~CPnChart()
{
}
/*
void CPnChart::StartSise()	// test
{
	SetTimer(TIMER_AGAP+1, 200, NULL);
}

#include <stdlib.h>
#include <time.h>
void CPnChart::MakeSise()	// test
{
	if (m_bProc)
		return;

	char	*code = (char *)SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseCode), 0);
	char	temp[64];
	double	dCurr;
	SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseCurr), (long)temp);
	if (strlen(temp))
	{
		CString	str = CString(temp, strlen(temp));
		int	index = str.Find("\t");
		if (index < 0)
			return;

		CString	curr = str.Left(index);
		dCurr = atof(curr);
	}

	CTime	time = CTime::GetCurrentTime();
	CString	sTime;
	sTime.Format("%02d%02d%02d", time.GetHour(), time.GetMinute(), time.GetSecond());

	static int xx = 0;
//	srand( (unsigned)500 );
	int x = rand();
	int	inc = x%10;
	switch (inc%4)
	{
	case 0:	dCurr += inc;	break;
	case 1:	dCurr -= inc;	break;
	case 2:	dCurr += x%5;	break;
	case 3:	dCurr -= x%5;	break;
	}

	CString	siseStr;
	siseStr.Format("%s\t%s\t%f\t%s\t%s\t%s\t%f\t%s\t%d\t", code, R_CURR, dCurr, R_CTIM, sTime, R_KOGA, dCurr+1, R_CVOL, x%500);

	if (m_pObjMgr)	m_pObjMgr->RealtimeGraphData((char*)siseStr.operator LPCTSTR());
}
*/
BEGIN_MESSAGE_MAP(CPnChart, CWnd)
	//{{AFX_MSG_MAP(CPnChart)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_CHART, OnChartEvent)
	ON_MESSAGE(GEV_VIEW, OnViewEvent)
	ON_MESSAGE(GEV_INPUT, OnInputEvent)
	ON_MESSAGE(GEV_SISE, OnSiseEvent)
	ON_MESSAGE(GEV_CTRL, OnCtrlEvent)
	ON_MESSAGE(GEV_TOOL, OnToolEvent)
	ON_MESSAGE(GEV_TABLE, OnTableEvent)
	ON_MESSAGE(WM_USER + 999, OnExtMessage)
	//** macho add
	ON_MESSAGE(GEV_AID, OnAidEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnChart message handlers

CWnd* CPnChart::CreateCtrl(CWnd* pwndView, CWnd* pwndGView, char *info, CFont* pFont)
{
	Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, 1, 1), pwndGView, 0);
	m_pwndView = pwndView;
	m_pwndGView = pwndGView;
	m_pFont = pFont;

	m_crBG = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 4);

	m_pObjMgr = new CObjMgr(m_pwndView, this, info, m_crBG, m_pFont);

	return this;
}

LRESULT CPnChart::OnChartEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case 999: // get chart index
		{
			CString str((LPCTSTR)lParam);
			CString time, price;
			int nPos = str.Find('\t');
			if (nPos <= 0) 
				return 0;
			time = str.Left(nPos);
			price = str.Right(str.GetLength() - (nPos + 1));
			int x, y;
			if (m_pObjMgr->GetXY(time, price, x, y)) 
			{
				return MAKELONG(x, y);
			}
			else 
			{
				return 0;
			}
		}
		break;
	case chartChgPalette:
		m_crBG = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 4);
		if (m_pObjMgr)
			//m_pObjMgr->m_clrBack = m_clrBack;
			m_pObjMgr->SetCrBG(m_crBG);
		if (m_pObjMgr)
			m_pObjMgr->ReDrawGraphObject(true);
		break;
	case CHART_SET_GRP:
		switch (HIWORD(wParam))
		{
		case CHANGE_ENV_INFO:
			if (m_pObjMgr)
				m_pObjMgr->ChangeEnvInfo();
			m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);
			break;
		case CHANGE_MAIN_GRP:
			if (m_pObjMgr)
				m_pObjMgr->ChangeMainGraph(lParam);
			m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);
			break;
		case changeMapInfo:
			if (m_pObjMgr)
				m_pObjMgr->ChangeMap((char *)lParam);
//		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);
			break;
		case ADD_AID_GRP:
			if (m_pObjMgr)
				m_pObjMgr->ChangeAidGraph(true, (_graph *)lParam);
			break;
		case delAidGrp:
			if (m_pObjMgr)
				m_pObjMgr->ChangeAidGraph(false, (_graph *)lParam);
			break;
		case chartReload:
			if (m_pObjMgr)
				m_pObjMgr->ReloadGrp((char *)lParam);
			break;
		case chartIReload:
			if (m_pObjMgr)	m_pObjMgr->ReloadGrp((char *)lParam, false);
			break;
		case chartBookMark:
			if (m_pObjMgr)
				m_pObjMgr->m_pObjEvent->SetBookMark((char*)lParam);
			break;
		}
		break;
	case CHART_TR_DATA:
		{
			// debug : 20090915
			//CString strDebug;
			//strDebug.Format("CPnChart::OnChartEvent() start");
			//AfxMessageBox(strDebug);

			struct _trData* pTD = (struct _trData*)lParam;
			if (!pTD || !pTD->pcData[0] || pTD->iLen[0] - pTD->iSiseLen[0] == 0 || pTD->iSiseLen[0] == 0)
			{
				if (m_pObjMgr)
					m_pObjMgr->ObjectClear();

				m_pwndGView->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlEnable), FALSE);

				return FALSE;
			}
		
			if (m_pObjMgr)
			{
				int iDispDay = m_pObjMgr->DispatchGraphData(pTD);
				if (iDispDay > 9999 || iDispDay < 1) return FALSE;

				if (iDispDay)
				{
					m_pwndGView->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlDisplay), 0);
					m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInput, setDisplayEdit), iDispDay);
				}
				else
				{
					if (m_pObjMgr)
						m_pObjMgr->ObjectClear();
					m_pwndGView->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlEnable), FALSE);
					m_pwndGView->SendMessage(GEV_TABLE, tbReset, 0);

					return FALSE;
				}				
			}
		}
		break;

	case CHART_TR_DATAS:
		{
			struct _trData* pTD = (_trData*)lParam;
			if (!pTD || !pTD->pcData || pTD->iLen - pTD->iSiseLen == 0)
				return FALSE;

			if (m_pObjMgr)
				m_pObjMgr->DispatchGraphDataS(pTD);
		}
		break;

	case CHART_DATA:
		switch (HIWORD(wParam))
		{
		case REQUEST_HEADER:
			if (m_pObjMgr)
				return (long)m_pObjMgr->GetGrpHeader((int*)lParam);
			return NULL;
		case REQUEST_HEADER2:
			if  (m_pObjMgr)
				return (long)m_pObjMgr->GetGrpHeader2((int*)lParam);
			return NULL;
		case requestHeader3:
			return NULL;
		case requestCurInter:
			if (m_pObjMgr)
				return (long)m_pObjMgr->GetMapInter();
			else	
				return 0;
			break;
		case REALTIME_DATA:
			if (m_pObjMgr)
				m_pObjMgr->RealtimeGraphData((char*)lParam);
			break;
		case REQUEST_CUR_ENV:
			if (m_pObjMgr)
				return long(m_pObjMgr->GetEnvInfo());
			return NULL;
		case REQUEST_CUR_MAP:
			if (m_pObjMgr)
				return long(m_pObjMgr->GetMapInfo());
			return NULL;
		case excelData:
			ExcelProc();			
			break;
		case getTableData:
			if (m_pObjMgr)
				return long(m_pObjMgr->GetTableData());
			return NULL;
		case getPrintTitle:
			if (m_pObjMgr)
				return long(m_pObjMgr->GetPrintString());
			return NULL;
		case getSaveName:
			if (m_pObjMgr)
				return long(m_pObjMgr->GetSaveString());
			return NULL;
		case chartClear:
			if (m_pObjMgr)
				m_pObjMgr->ObjectClear();
			break;
		case requestHeaderS:
			if  (m_pObjMgr)
				return (long)m_pObjMgr->GetGrpHeaderS((int*)lParam);
			return NULL;
		}
		break;
	case CHART_ENV:
		switch (HIWORD(wParam))
		{
		case chartIsValid:
			if (m_pObjMgr)
			{
				if (m_pObjMgr->m_arGraphQue.GetSize() > 0)
					return TRUE;
			}

			return FALSE;
		case chartIndex:
			if (m_pObjMgr)
				m_pObjMgr->ChangeIndex(LOWORD(lParam), HIWORD(lParam));
			break;
		case chartUnit:
			if (m_pObjMgr)
				m_pObjMgr->ChangeUnit(lParam);
			break;
		case chartTotal:
			if (m_pObjMgr)
				m_pObjMgr->ChangeTotalDay(lParam);
			break;
		case CHART_DISPLAY:
			if (m_pObjMgr)
				return m_pObjMgr->ChangeDisplay(lParam);
			break;
		case chartScale:
			if (m_pObjMgr)
				return m_pObjMgr->ChangeDisplayScale(lParam);
			break;
		case chartTable:
			if (m_pObjMgr)
			{
				m_pObjMgr->SetTableMode((lParam)? true:false, false);
				return TRUE;
			}
			break;
		case chartGrid:
			if (m_pObjMgr)
			{
				m_pObjMgr->SetGridMode(false);
				return TRUE;
			}
		case chartTip:
			if (m_pObjMgr)
			{
				m_pObjMgr->SetTipMode(false);
				return TRUE;
			}
			break;
		}
		break;
	case chartCtrl:
		switch (HIWORD(wParam))
		{
		case getScrollData:
			if (m_pObjMgr)
				return m_pObjMgr->GetScrollInfo((char*)lParam);
			break;
		case changePosition:
			{
				int dispPos = LOWORD(lParam);
				int dispEnd = HIWORD(lParam);
				if (m_pObjMgr)
					m_pObjMgr->ChangeDisplayPos(dispPos, dispEnd);
			}
			break;
		case changeShift:
			if (m_pObjMgr)
				return m_pObjMgr->ChangeDisplayShift(lParam);
			break;
		case chartAutoScroll:
			if (m_pObjMgr)
			{
				bool bStatus = (lParam == 0)? false:true;
				m_pObjMgr->SetAutoScroll(bStatus);
			}
			break;
		case isAutoScroll:
			if (m_pObjMgr)
				return m_pObjMgr->IsAutoScr();
			return FALSE;
		case chartToolSelect:		// tool select
			if (m_pObjMgr)
				return m_pObjMgr->ChangeTool(lParam);
			return false;
		case chartToolColor:
			if (m_pObjMgr)
				m_pObjMgr->ChangeToolColor(lParam);
			break;
		case chartToolWidth:
			if (m_pObjMgr)
				m_pObjMgr->ChangeToolWidth(lParam);
			break;
		case chartExcel:	// excel data
			break;
		case chartAutotran:
			break;
		case chartUpdate:
			if (m_pObjMgr)
				m_pObjMgr->ReDrawGraphObject(lParam? true:false);
			break;
		case patternChanged:
			if (m_pObjMgr)
				m_pObjMgr->SetPattern(LOWORD(lParam), HIWORD(lParam)? true:false);
			break;
		}
		break;
	case chartViewFocus:
		if (!lParam)
		{
			if (m_pObjMgr)
				m_pObjMgr->CreateGrid(false);
		}

//		if (m_pObjMgr)
//			m_pObjMgr->HideGrid();
		break;
	case chartToolTip:	// adCross message
		{
			BOOL bShow = (BOOL)HIWORD(wParam);
			if (!bShow)
			{
				if (m_pObjMgr)
					m_pObjMgr->GetCrossValue(CPoint(0,0), NULL, true);
			}
		}
		break;
	case CHART_MENU:
		switch (HIWORD(wParam))
		{
		case ADD_CHART_MENU:
			if (m_pObjMgr)
			{
				if (m_pObjMgr->m_arGraphQue.GetSize() <= 0)
					return 0;
				m_pObjMgr->m_pObjEvent->AppendChartMenu((CMenu *)lParam, false);
			}
			else
			{
				return 0;
			}
			break;
		case selChartMenu:
			if (m_pObjMgr)
				m_pObjMgr->m_pObjEvent->ProcessChartMenu(LOWORD(lParam), HIWORD(lParam));
			break;
		}
	// 2006.11.20 lsh (start)
	case CHART_USER_SELECT: 
		m_strRtnData = _T("");
		switch(HIWORD(wParam)) 
		{
		
		case getSelectTime:
			Sleep(0);
			if (m_pObjMgr)
			{
				m_strRtnData = m_pObjMgr->m_pObjEvent->m_strClickDateTime;
			}
			break;
		case setSelectTime:
			break;
		case getSelectPrice:
			m_strRtnData = m_pObjMgr->m_pObjEvent->m_strClickPrice;
			break;
		case StartOrderMode:
			m_pObjMgr->SetOrderMode(true);
			break;
		}
		
		return (long)(LPCTSTR)m_strRtnData;
		// 2006.11.20 lsh (end)
	//** add by macho@20061226 <start>
	case CHART_STANLINE:
		return (LRESULT)StanLineProc(HIWORD(wParam), lParam);
	//** add by macho@20061226 <end>

	//** add by macho@20070102 <start>
	case CHART_ANAL:
		if (m_pObjMgr) 
		{
			return m_pObjMgr->AnalMessageProc(HIWORD(wParam), lParam);
		}
		break;
	}
	//** add by macho@20070102 <end>

	return TRUE;
}

LRESULT CPnChart::OnViewEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_VIEW, wParam, lParam);
	return TRUE;
}

LRESULT CPnChart::OnInputEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_INPUT, wParam, lParam);
	return TRUE;
}

LRESULT CPnChart::OnSiseEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_SISE, wParam, lParam);
	return TRUE;
}

LRESULT CPnChart::OnCtrlEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_CTRL, wParam, lParam);
	return TRUE;
}

LRESULT CPnChart::OnToolEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_TOOL, wParam, lParam);
	return TRUE;
}

LRESULT CPnChart::OnAidEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_AID, wParam, lParam);
	return TRUE;
}

LRESULT CPnChart::OnTableEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndGView)
		return m_pwndGView->SendMessage(GEV_TABLE, wParam, lParam);
	return TRUE;
}

int CPnChart::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CPnChart::OnDestroy() 
{
	if (m_pObjMgr)
	{
		delete m_pObjMgr;
		m_pObjMgr = NULL;
	}

	CWnd::OnDestroy();
}

void CPnChart::OnPaint() 
{
	CPaintDC dc(this);

	if (m_pObjMgr)
	{
		m_pObjMgr->DrawGraphObject();
	}
	else
	{
		CAxisGMainApp	*pApp = (CAxisGMainApp *)AfxGetApp();

		CRect rect;
		GetClientRect(rect);
		CBrush *brush = pApp->GetBrush(m_pwndView, m_crBG);
		
		dc.FillRect(&rect, brush);
	}
}

void CPnChart::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (cx == 0 || cy == 0)
		return;

	if (m_pObjMgr)
	{
		m_pObjMgr->ResizeGraphObject();
		m_pObjMgr->ReDrawGraphObject(true);
	}
}

void CPnChart::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
/*static int xx = 0;	// test
if (xx++%4 == 2)	StartSise();	
else	KillTimer(TIMER_AGAP+1);
*/	

	// activate notify to view
	m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_MSLBDN, point, 0);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CPnChart::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_MSLBUP, point, 0);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CPnChart::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_MSDBLC, point, 0);
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CPnChart::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint	pt;
	GetCursorPos(&pt);
	CWnd *pWnd = WindowFromPoint(pt);

	if (m_pObjMgr)
	{
		if (pWnd != this)
		{
			m_pObjMgr->MouseEvent(GWE_MSMOVE, point, 0);
		}
		else
			m_pObjMgr->MouseEvent(GWE_MSMOVE, point, 1);
	}

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CPnChart::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	bool bLeft;
	switch (zDelta)
	{
	case 120:
		bLeft = true;
		break;
	case -120:
		bLeft = false;
		break;
	default:
		return false;
	}

	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_MSWHEEL, pt, bLeft);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CPnChart::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);
	// activate notify to view
	m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
}

void CPnChart::OnRButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_MSRBUP, point, 0);
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CPnChart::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_FOCUS, CPoint(0,0), 1);

	CWnd::OnSetFocus(pOldWnd);
}

void CPnChart::OnKillFocus(CWnd* pNewWnd) 
{
	if (m_pObjMgr)
		m_pObjMgr->MouseEvent(GWE_FOCUS, CPoint(0,0), 0);

	CWnd::OnKillFocus(pNewWnd);
}

void CPnChart::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_DELETE)
		if (m_pObjMgr)
			m_pObjMgr->MouseEvent(GWE_KEYBOARD, CPoint(0,0), VK_DELETE);
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPnChart::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIMER_PID:
		KillTimer(nIDEvent);
		{
			CPoint	pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			CString str = m_pObjMgr->m_pObjEvent->GetTipStr(pt);

			if (m_pObjMgr->m_pObjEvent->m_strPosData == str)
				m_pObjMgr->m_pObjEvent->MouseMove(true, pt);
		}
		break;
	case TIMER_AID:
		if (!IsPtInGrp())
		{
			KillTimer(nIDEvent);
			m_pObjMgr->m_pObjEvent->m_pToolTip->ShowWindow(SW_HIDE);
			m_pObjMgr->m_pObjEvent->m_nIimerID = 0;
		}
		break;
	case TIMER_GRID:
		KillTimer(nIDEvent);
		if (m_pObjMgr)
		{
			CPoint	pt;
			GetCursorPos(&pt);
			CRect	rc;
			GetWindowRect(&rc);

			CWnd *pWnd = WindowFromPoint(pt);

			if (!rc.PtInRect(pt) || pWnd != this)
				m_pObjMgr->HideGrid();
			else
				SetTimer(TIMER_HIDE, TIMER_HGAP, NULL);
		}
		break;
	case TIMER_HIDE:
		if (m_pObjMgr)
		{
			CPoint	pt;
			GetCursorPos(&pt);
			CRect	rc;
			GetWindowRect(&rc);

			CWnd *pWnd = WindowFromPoint(pt);

			if (pWnd != this)
			{
				m_pObjMgr->HideGrid();
				m_pObjMgr->m_pObjEvent->m_pToolTip->ShowWindow(SW_HIDE);
			}
		}
		break;
	case TIMER_IMG:
		KillTimer(nIDEvent);
		SendMessage(GEV_VIEW, MAKEWPARAM(viewUtility, viewImage), 0);
		break;
/*	case TIMER_AGAP+1:	// test
		MakeSise();
		break;
*/	}
	
	CWnd::OnTimer(nIDEvent);
}

BOOL CPnChart::IsPtInGrp()
{
	POINT	pt;
	CRect	rect;

	::GetCursorPos(&pt);
	GetWindowRect(rect);

	CWnd* pWnd = CWnd::WindowFromPoint(pt);
	if (pWnd != this)
		return FALSE;

	return (rect.PtInRect(pt));
}

void CPnChart::ExcelProc()
{
	CString text = _T("");
	
	if (!m_pObjMgr || !m_pObjMgr->GetExcelData(text))
		return;

	if (text.IsEmpty())
		return;

	CString	fileN, dFile, tFile, tmpS, tmpS_;
	CString	root = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, getRootPath), 0);

	bool	visible = true;
	if (visible)
	{
		CString	sTime = _T("");

		if (m_pObjMgr)
			m_pObjMgr->GetRTSCode(sTime);
		if (sTime.IsEmpty())
		{
			CTime	time = CTime::GetCurrentTime();
			sTime.Format("%04d%02d%02d_%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(),
				time.GetHour(), time.GetMinute(), time.GetSecond());
		}
		tFile.Format("%s\\%s\\%s.txt", root, USRDIR, sTime);
		dFile.Format("%s\\%s\\%s.xls", root, USRDIR, sTime);
	}
	else
	{
		CFileDialog fDlg(FALSE, NULL, NULL,
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
				"WorkSheet Files(*.xls)|*.xls|All Files(*.*)|*.*|");
		if (fDlg.DoModal() != IDOK)
			return;

		dFile = fDlg.GetPathName();
		fileN = fDlg.GetFileName();
		tmpS  = fDlg.GetFileExt();

		int	offs = dFile.ReverseFind('.');
		if (offs == -1)
			tmpS = "xls";
		else
		{
			tmpS  = dFile.Mid(offs+1);
			dFile = dFile.Left(offs);
			if (tmpS.IsEmpty())
				tmpS = "xls";

			offs  = fileN.ReverseFind('.');
			fileN = fileN.Left(offs);
		}

		dFile += ".";
		tmpS_ = dFile;
		dFile += tmpS;

		tFile.Format("%s\\%s\\tmp", root, RUNDIR);
		int	rc = GetFileAttributes(tFile);
		if (rc != FILE_ATTRIBUTE_DIRECTORY)
		{
			if (!CreateDirectory(tFile, NULL))
			{
				AfxGetMainWnd()->MessageBox("Directory를 설정할 수 없습니다.", "확인", MB_OK);
				return;
			}
		}

		tFile += "\\";
		tFile += fileN;
		tFile += ".txt";

		DeleteFile(tFile);
		DeleteFile(dFile);
	}

	CFile	fileH;
	fileH.Open(tFile, CFile::modeCreate|CFile::modeWrite);
	fileH.Write(text.operator LPCTSTR(), text.GetLength());
	fileH.Close();

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		if (!visible)
		{
			tmpS_ += "txt";
			CopyFile(tFile, tmpS_, FALSE);
			DeleteFile(tFile);
		}
		else
			tmpS_ = tFile;

		tmpS_ += _T(" 로 저장되었습니다.");
		AfxGetMainWnd()->MessageBox(tmpS_, "확인", MB_OK);
		return;
	}

	excel.SetVisible(visible);

	COleVariant
		FilenameSave(dFile),
		Fileformat((short)33),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		LPDISPATCH	lpDisp;

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);
		lpDisp = Wbs.Open(tFile,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional,
			covOptional, covOptional, covOptional, covOptional);

		Wb.AttachDispatch(lpDisp);
		Wb.SetSaved(TRUE);
		Wb.SaveAs(FilenameSave, Fileformat, covOptional, covOptional,
				covOptional, covOptional, 7, covOptional,
				covOptional, covOptional, covOptional);

		if (!visible)
			Wbs.Close();
		DeleteFile(tFile);
	}
	CATCH(COleDispatchException, e)
	{
		TRACE(e->m_strDescription);
//		AfxGetMainWnd()->MessageBox("Excel-File 생성 에러:\n"+e->m_strDescription, "확인", MB_OK);
	}
	END_CATCH;

	if (!visible)
		excel.Quit();

	excel.ReleaseDispatch();
}

LRESULT CPnChart::OnExtMessage(WPARAM wParam, LPARAM lParam)
{
	return m_pwndGView->SendMessage(WM_USER + 999, wParam, lParam);
}

void CPnChart::InitStanLine()
{
	m_stanLineColor = RGB(0xFF, 0, 0);
	m_stanLineWidth = 2;
	m_stanLine.Empty();
}

void CPnChart::SetStanLine(int lineWidth, COLORREF lineColor)
{
	m_stanLineWidth = lineWidth;
	m_stanLineColor = lineColor;
	if (m_pObjMgr)
		m_pObjMgr->ReDrawGraphObject(true);
}

void CPnChart::SetStanLine(LPCTSTR datetime)
{
	m_stanLine = datetime;
	if (m_pObjMgr)
		m_pObjMgr->ReDrawGraphObject(true);
}

void CPnChart::DeleteStanLine()
{
	m_stanLine.Empty();
	if (m_pObjMgr)
		m_pObjMgr->ReDrawGraphObject(true);
}

BOOL CPnChart::HasStanLine()
{
	return m_stanLine.GetLength() == 15; // seriesGb + yyyymmddhhMMss
										 // seriesGb =  'D': day, 'W': week, 'M': month, 't': tick, 'm': minute
}

void CPnChart::DrawStanLine(CDC* dc, CIndcBase* indc)
{
	if (!HasStanLine()) return;
	if (!indc) return;

	COrgData *pOrgData;
	if (!m_pObjMgr->m_pDataMgr->GetOrgData(indc->GetDataKey(), pOrgData))
		return;
	
	int year, mon, day, hour, minute, second;
	CString stanLine(m_stanLine);

	stanLine.Delete(0, 1);
	year = atoi(stanLine.Left(4));
	mon  = atoi(stanLine.Mid(4, 2));
	day  = atoi(stanLine.Mid(6, 2));
	hour = atoi(stanLine.Mid(8, 2));
	minute = atoi(stanLine.Mid(10, 2));
	second = atoi(stanLine.Mid(12, 2));

	int date, time, compDate, compTime;
	date = year * 10000 + mon * 100 + (m_stanLine[0] != 'M' ? day : 0);

	BOOL isDaily;
	switch (m_stanLine[0]) 
	{
	case 'D': case 'W': case 'M':
		isDaily = TRUE;
		break;
	default:
		isDaily = FALSE;
		break;
	}

	if (isDaily)
		time = 0;
	else 
		time = hour * 3600 + minute * 60 + second;

	int gindex = -1;
	CGrpBasic* gBasic;

	for (int i = pOrgData->GetArraySize() - 1; i >= 0; i--) 
	{
		gBasic = pOrgData->GetGraphData(i);

		compDate = gBasic->m_index.date.yy * 10000 + gBasic->m_index.date.mm * 100 + gBasic->m_index.date.dd;
		if (isDaily) 
		{
			if (date > compDate) 
				gindex = i + 1;
			if (date == compDate)
				gindex = i;
		}
		else 
		{
			compTime = gBasic->m_index.time.hh * 3600 + gBasic->m_index.time.mm * 60 + gBasic->m_index.time.ss;
			if (date > compDate) 
			{
				gindex = i + 1;
			}
			else if (date == compDate) 
			{
				if (time > compTime) 
					gindex = i + 1;
				else if (time == compTime)
					gindex = i;
			}
		}

		if (gindex >= 0) break;
	}

	if (gindex >= 0) 
	{
		if (gindex >= pOrgData->GetArraySize())
			gindex = pOrgData->GetArraySize() - 1;

		CPoint pt;
		int x;

		indc->GetValueToPoint(pt, gindex, 0);
		x = pt.x;
		indc->GetValueToPoint(pt, gindex + 1, 0);
		
		x = (int)((x + pt.x) / 2.0);


		if (indc->GetChartRect().PtInRect(CPoint(x, indc->GetChartRect().top + 1))) 
		{
			CPen pen;

			pen.CreatePen(PS_SOLID, m_stanLineWidth, m_stanLineColor);
			CPen	*sPen = dc->SelectObject(&pen);
			dc->MoveTo(x, indc->GetChartRect().top);
			dc->LineTo(x, indc->GetChartRect().bottom);

			dc->SelectObject(sPen);
		}
	}
}

LRESULT CPnChart::StanLineProc(UINT job, LPARAM lParam)
{
	switch (job) 
	{
		case setStanLineDateTime:
			SetStanLine((LPCTSTR)lParam);
			return 0L;
		case setStanLineWidth:
			SetStanLine((int)lParam, m_stanLineColor);
			return 0L;
		case setStanLineColor:
			SetStanLine(m_stanLineWidth, (COLORREF)lParam);
			return 0L;
		case stanLineDelete:
			DeleteStanLine();
			return 0L;
		case getStanLineDateTime:
			return (LRESULT)(LPCTSTR)m_stanLine;
		case getStanLineWidth:
			return m_stanLineWidth;
		case getStanLineColor:
			return m_stanLineColor;
	}
	return 0L;
}

