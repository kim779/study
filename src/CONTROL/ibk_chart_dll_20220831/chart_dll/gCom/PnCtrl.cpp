// PnCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnCtrl.h"
#include "PnWndCtrl.h"
#include "PnToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnCtrl
#define	sliderWIDTH	90
#define	disp_GAP	5
#define	timer_ID	1004
#define	timer_GAP	200

struct _info	g_infoCtrl[] = {
	{ IC_SCROLL,	 0,  0,  0, 0,	"",	"" },
	{ IC_SLIDER,	 0,  0,  0, 0,	"",	"" },
	{ IC_REDUCE,	 0,  1,  2, 0,	"축소",	"축소" },
	{ IC_EXPAND,	 3,  4,  5, 0,	"확대",	"확대" },
	{ IC_BEFORE,	 9, 10, 11, 0,	"이전",	"자동이전조회" },
	{ IC_STOP,	12, 13, 14, 0,	"멈춤",	"자동조회중지" },
	{ IC_NEXT,	15, 16, 17, 0,	"다음",	"자동다음조회" },
	{ IC_EXCEL,	18, 19, 20, 0,	"엑셀",	"엑셀로 저장" },
	{ IC_IMAGE,	21, 22, 23, 0,	"저장","이미지로 저장" },
	{ IC_GRID,	24, 25, 26, 0,	"수치",	"수치조회" },
	{ IC_TABLE,	27, 28, 29, 0,	"자료","자료조회" },
};

CPnCtrl::CPnCtrl(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info)
: CPnBaseWnd(pView, pParent, pFont, ctrlID, info)
{
	_envInfo* envinfo = (_envInfo*)info;
	m_iMainKind = envinfo->datainfo.wMainGrpKind;

	m_pScroll = NULL;
	m_pSlider = NULL;
	m_pToolTip = NULL;
	m_pimglstCtrl = NULL;

	m_iSlider = 0;
	m_crColourBk = GetSysColor(COLOR_BTNFACE);

	m_iTimerID = 0;
	m_iAutoScroll = autoStop;
	m_bLButton = false;
	m_iMoveID = 0;

	m_bTable = false;
	m_bGrid = (envinfo->display.dwDspOption & DO_VIEWGRID)? true:false;
}

CPnCtrl::~CPnCtrl()
{
}


BEGIN_MESSAGE_MAP(CPnCtrl, CPnBaseWnd)
	//{{AFX_MSG_MAP(CPnCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_CTRL, OnCtrlEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnCtrl message handlers

LRESULT CPnCtrl::OnCtrlEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_SCROLL_POS:
		if (m_iAutoScroll)
		{
			setAutoScroll(autoStop);
			break;
		}
		switch ((int)HIWORD(wParam))
		{
		case SB_LINEUP:
			{
				getScrInfo();
				if (m_iDispPos <= 0)
					break;
				m_iDispPos--;
				m_iDispEnd--;
				if (m_pwndGView)
					m_pwndGView->SendMessage(GEV_CHART, 
						MAKEWPARAM(chartCtrl, changePosition), MAKELPARAM(m_iDispPos, m_iDispEnd));
				initScroll(m_iDispPos, m_iDispEnd, m_iDispDay, m_iTotalDay);
			}
			break;
		case SB_LINEDOWN:
			{
				getScrInfo();
				if (m_iDispEnd >= m_iTotalDay)
					break;
				m_iDispPos++;
				m_iDispEnd++;
				if (m_pwndGView)
					m_pwndGView->SendMessage(GEV_CHART, 
						MAKEWPARAM(chartCtrl, changePosition), MAKELPARAM(m_iDispPos, m_iDispEnd));
			
				initScroll(m_iDispPos, m_iDispEnd, m_iDispDay, m_iTotalDay);
			}
			break;
		case ctrlWheelLeft:
			if (m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispScaleCtrl), disp_GAP*(-3)))
				UpdateScroll();
/*			{

				getScrInfo();
				if (m_iDispPos <= 0)
					break;
				int	iLeft = (m_iDispPos >= 5)? 5:m_iDispPos;
				m_iDispPos -= iLeft;
				m_iDispEnd -= iLeft;
				if (m_pwndGView)
					m_pwndGView->SendMessage(GEV_CHART, 
						MAKEWPARAM(chartCtrl, changePosition), MAKELPARAM(m_iDispPos, m_iDispEnd));
				initScroll(m_iDispPos, m_iDispEnd, m_iDispDay, m_iTotalDay);

			}
*/			break;
		case ctrlWheelRight:
			if (m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispScaleCtrl), disp_GAP*3))
				UpdateScroll();
/*			{
				getScrInfo();
				if (m_iDispEnd >= m_iTotalDay)
					break;
				int	iRight = (m_iTotalDay - m_iDispEnd >= 5)? 5:m_iTotalDay - m_iDispEnd;
				m_iDispPos += iRight;
				m_iDispEnd += iRight;
				if (m_pwndGView)
					m_pwndGView->SendMessage(GEV_CHART, 
						MAKEWPARAM(chartCtrl, changePosition), MAKELPARAM(m_iDispPos, m_iDispEnd));
			
				initScroll(m_iDispPos, m_iDispEnd, m_iDispDay, m_iTotalDay);
			}
*/			break;
		case SB_PAGEDOWN:
		case SB_PAGEUP:
		case SB_THUMBTRACK:
			{
				int pos = (int)lParam;
				if (pos >= 0)
				{
					int dispPos = (int)((pos) * (m_iTotalDay) / 100); 
					int dispEnd = m_iDispDay + dispPos;
					if (dispEnd > m_iTotalDay)
					{
						int gap = dispEnd - m_iTotalDay;
						dispPos -= gap;
						dispEnd -= gap;
					}

					if (m_pwndGView)
						m_pwndGView->SendMessage(GEV_CHART, 
							MAKEWPARAM(chartCtrl, changePosition), MAKELPARAM(dispPos, dispEnd));
				}
			}
			break;
		}
		break;
	case CTRL_GET_POS_COMMENT:
		m_comment = GetPosComment(CPoint(LOWORD(lParam), HIWORD(lParam)));

		return (LRESULT)(LPCTSTR)m_comment;
	case CTRL_ENV:
		switch (HIWORD(wParam))
		{
		case ctrlMain:
			m_iMainKind = lParam;
			m_pScroll->EnableWindow(FALSE);
			m_pSlider->EnableWindow(FALSE);
			break;
		case ctrlDisplay:
			if (m_iAutoScroll)
				setAutoScroll(autoStop);

			UpdateScroll();
			break;
		case ctrlEnable:
			if (m_iAutoScroll)
				setAutoScroll(autoStop);
			{
				BOOL bEnable = (BOOL)lParam;
				m_pScroll->EnableWindow(bEnable);
				m_pSlider->EnableWindow(bEnable);
			}
			break;
		case ctrlAutotran:
			break;
		case ctrlGrid:
			SetGrid((lParam)? true:false);
			break;
		case ctrlTable:
			SetTable((lParam)? true:false);
			break;
		case ctrlReload:
			{
				_envInfo *envinfo = (_envInfo *)lParam;
				m_bGrid = (envinfo->display.dwDspOption & DO_VIEWGRID)? true:false;
				SetGrid(m_bGrid);
			}
			SetTable(false);
			break;
		case ctrlClear:
			m_pScroll->EnableWindow(FALSE);
			m_pSlider->EnableWindow(FALSE);
			break;
		case CTRL_UPDATE_SLIDE:
			UpdateScroll();
			break;
		}
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

int CPnCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_clrBack = PALETTERGB(233, 233, 231);

	setInfo();

	if (!m_pScroll)
	{
		m_pScroll = new CPnScrollBar();
		m_pScroll->Create(WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_BOTTOMALIGN, 
			CRect(0, 0, 0, 0), this, IC_SCROLL);
		
		m_pScroll->SetOwner(this);
		m_pScroll->EnableWindow(FALSE);
	}
	if (!m_pSlider)
	{
		m_pSlider = new CPnSlider();
		m_pSlider->Create(WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_BOTTOM|TBS_AUTOTICKS|TBS_TOOLTIPS,
			CRect(0, 0, 0, 0), this, IC_SLIDER);
		m_pSlider->SetOwner(this);
		m_pSlider->EnableWindow(FALSE);
	}
	if (!m_pToolTip)
	{
		m_pToolTip = new CPnToolTip(m_pwndView, this, GEV_CTRL);
		m_pToolTip->Create(m_pFont);
	}
	
	CString path = m_pApp->GetImagePath(m_pwndView, CTRLIMAGE);
	m_pimglstCtrl = m_pApp->GetImage(imgCtrl, path);

	m_bMouseIN = false;

	return 0;
}

void CPnCtrl::OnPaint() 
{
	CPaintDC dc(this);

	DrawBackground(&dc);
	CFont *sFont = dc.SelectObject(m_pFont);
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
		drawButton(&dc, m_arComs.GetAt(ii));

	dc.SelectObject(sFont);
}

void CPnCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)
		return;

	ArrangeCtrl(CRect(0, 0, cx, cy));
}

void CPnCtrl::OnDestroy() 
{
	if (m_iAutoScroll)	KillTimer(m_iTimerID);
	if (m_pScroll)		WND_DELETE(m_pScroll);
	if (m_pSlider)		WND_DELETE(m_pSlider);
	if (m_pToolTip)		WND_DELETE(m_pToolTip);

	CPnBaseWnd::OnDestroy();
}

void CPnCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnLButtonDown(nFlags, point);

	struct _comp* comp = SearchCom(point);
	if (!comp)
	{
		return;
	}
	m_bLButton = true;
	m_iSelID = comp->cID;

	if (m_iSelID == IC_GRID)
	{
		if (m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartIsValid), 0))
			SetGrid(!m_bGrid);
	}
	else if (m_iSelID == IC_TABLE)
	{
		if (m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartIsValid), 0))
			SetTable(!m_bTable);
	}
	else
		InvalidateCom(comp, btnDown);
}

void CPnCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnLButtonUp(nFlags, point);
	lbuttonup(point);
}

void CPnCtrl::lbuttonup(CPoint point)
{
	if (!m_bLButton)
	{
		return;
	}

	m_iMoveID = 0;
	m_bLButton = false;

	struct _comp* pOldCom = SearchCom(m_iSelID);
	struct _comp* comp = SearchCom(point);
	if (pOldCom)
	{
		if (m_iSelID == IC_GRID)
		{
			if (comp &&comp->cID == IC_GRID)
			{
				if (m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartIsValid), 0))
					m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridCtrl), m_bGrid);
			}
			else
			{
				SetGrid(!m_bGrid);
			}

			return;
		}
		else if (m_iSelID == IC_TABLE)
		{
			if (comp &&comp->cID == IC_TABLE)
			{
				if (m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartIsValid), 0))
					m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, tableCtrl), m_bTable);
			}
			else
			{
				SetTable(!m_bTable);
			}
			return;
		}
		else
		{
			InvalidateCom(pOldCom, btnUp);
		}
	}

	if (!comp || m_iSelID != comp->cID || !comp->bshow)
		return;

	switch (comp->cID)
	{
	case IC_SLIDER:
	case IC_SCROLL:
		return;
	}

	if (comp->cID != IC_IMAGE)
	{
		switch (m_iMainKind)
		{
		case GK_THREE:	case GK_PNF:	case GK_CLOCK:
		case GK_PV:	case GK_KAGI:	case GK_RENKO:
			return;
		}
	}

	switch (comp->cID)
	{// process etc button
	case IC_BEFORE:
		if (m_iAutoScroll == autoPre)	break;

		setAutoScroll(autoStop);
		setAutoScroll(autoPre);
		break;
	case IC_NEXT:
		if (m_iAutoScroll == autoNext)	break;

		setAutoScroll(autoStop);
		setAutoScroll(autoNext);
		break;
	case IC_STOP:	
		if (m_iAutoScroll)	setAutoScroll(autoStop);

		break;
	case IC_EXPAND:
		if (m_iAutoScroll)
			break;

		if (m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispScaleCtrl), disp_GAP*(-1)))
			UpdateScroll();
		break;
	case IC_REDUCE:
		if (m_iAutoScroll)	break;

		if (m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispScaleCtrl), disp_GAP))
			UpdateScroll();
		break;
	case IC_EXCEL:
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewUtility, viewExcel), 0);
		break;
	case IC_IMAGE:
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewUtility, viewImage), 0);
		break;
	default:
		break;
	}
}

void CPnCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnMouseMove(nFlags, point);

	if (!m_bMouseIN)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		m_bMouseIN = _TrackMouseEvent(&tme);
	}

	if (m_bLButton)
		return;

	CPoint	Orgpoint = point;
	struct _comp* mcomp = SearchCom(Orgpoint);
	if (!mcomp)
	{
		mcomp = SearchCom(m_iMoveID);
		if (mcomp)
		{
			if (!(m_iMoveID == IC_GRID && m_bGrid) && !(m_iMoveID == IC_TABLE && m_bTable))
				InvalidateCom(mcomp, btnUp);
		}
		m_iMoveID = 0;
		return;
	}

	if (m_iMoveID == mcomp->cID)
	{
		return;
	}

	struct _comp* ocomp = SearchCom(m_iMoveID);
	if (ocomp)
	{
		if (!(m_iMoveID == IC_GRID && m_bGrid) && !(m_iMoveID == IC_TABLE && m_bTable))
			InvalidateCom(ocomp, btnUp);
	}
	m_iMoveID = mcomp->cID;
	if (!(m_iMoveID == IC_GRID && m_bGrid) && !(m_iMoveID == IC_TABLE && m_bTable))
		InvalidateCom(mcomp, btnMove);
}

void CPnCtrl::mouseLeaveAction(UINT nFlags, CPoint point)
{
	if (m_bLButton)
		lbuttonup(point);

	else if (m_iMoveID)
	{
		struct _comp* ocomp = SearchCom(m_iMoveID);
		if (ocomp && !(m_iMoveID == IC_GRID && m_bGrid) && !(m_iMoveID == IC_TABLE && m_bTable))
			InvalidateCom(ocomp, btnUp);
	}
	m_iMoveID = 0;
	m_bMouseIN = FALSE;
		
	if (m_pToolTip)
	{
		m_pToolTip->m_dispStr = _T("");
		m_pToolTip->hideToolTip();
	}
}

void CPnCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CPnBaseWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

void CPnCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == timer_ID)
		AutoShiftAction();
	
	CPnBaseWnd::OnTimer(nIDEvent);
}

void CPnCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (m_iAutoScroll)
		setAutoScroll(autoStop);

	// 슬리이더바
	if (m_pSlider && m_pSlider->IsWindowEnabled() 
		&& pScrollBar->GetDlgCtrlID() == IC_SLIDER)
	{
		int pos = m_pSlider->GetPos();
		if (m_iSlider != pos)
		{
			m_iSlider = pos;
			m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispDayCtrl), pos);
			UpdateScroll();
		}
	}	
	
	CPnBaseWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CPnCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_MOUSELEAVE)
	{
		mouseLeaveAction(0, CPoint(0,0));
		return TRUE;
	}

	if (m_pToolTip)
		m_pToolTip->RelayEvent(message);	
	
	return CPnBaseWnd::WindowProc(message, wParam, lParam);
}

void CPnCtrl::ArrangeCtrl(CRect wRC)
{
	struct _comp	*comp;
	int	scrollWidth;
	int	sliderWidth;
	int	btnCnt = m_arComs.GetSize() - 2;
	wRC.DeflateRect(1, 0);

	if (wRC.Width() < szCtrlImgX + sliderWIDTH*2 + 2)
	{
		if (wRC.Width() < sliderWIDTH*2 + 2)
		{
			scrollWidth = sliderWidth = (wRC.Width() - 1)/2;
		}
		else
		{
			scrollWidth = sliderWIDTH;
			sliderWidth = sliderWIDTH;
		}
	}
	else
	{
		sliderWidth = 90;
		scrollWidth = wRC.Width() - sliderWidth - szCtrlImgX*btnCnt - 2;
	}

	if (scrollWidth < sliderWIDTH)
		scrollWidth = sliderWIDTH;

	int	left = wRC.left;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		switch (comp->cID)
		{
		case IC_SCROLL:
			comp->cRC.left = left;
			comp->cRC.right = left + scrollWidth;
			comp->cRC.top = wRC.top;
			comp->cRC.bottom = wRC.bottom;
			left += scrollWidth + 1;
			break;
		case IC_SLIDER:
			comp->cRC.left = left;
			comp->cRC.right = left + sliderWidth;
			comp->cRC.top = wRC.top;
			comp->cRC.bottom = wRC.bottom;
			left += sliderWidth;
			break;
		case IC_REDUCE:
		case IC_EXPAND:
		case IC_BEFORE:
		case IC_STOP:
		case IC_NEXT:
		case IC_EXCEL:
		case IC_IMAGE:
		case IC_GRID:
		case IC_TABLE:
			if (left + szCtrlImgX > wRC.right)
			{
				comp->bshow = false;
				break;
			}
			comp->bshow = true;
			comp->cRC.left = left;
			comp->cRC.right = left + szCtrlImgX;
			comp->cRC.top = wRC.top;
			comp->cRC.bottom = wRC.bottom;
			left += szCtrlImgX;
			break;
		}
	}

	if (m_pScroll)
	{
		comp = SearchCom(IC_SCROLL);
		m_pScroll->MoveWindow(comp->cRC);
	}
	if (m_pSlider)
	{
		comp = SearchCom(IC_SLIDER);
		m_pSlider->MoveWindow(comp->cRC);
	}
}

void CPnCtrl::drawButton(CDC* pDC, struct _comp* comp)
{
	if (!comp->bshow)
		return;
	CRect rc;
	switch (comp->cID)
	{
	case IC_SCROLL:
/*		
		GetDlgItem(comp->cID)->GetWindowRect(&rc);
		pDC->Rectangle(rc);
		break;
*/
  case IC_SLIDER:
/*
		GetDlgItem(comp->cID)->GetWindowRect(&rc);
		pDC->Rectangle(rc);
*/
		break;
	default:
		DrawButton(pDC, dtButton, comp, m_pimglstCtrl);
		break;
	}
}

void CPnCtrl::setInfo()
{
	int	size = sizeof(g_infoCtrl) / sz_INFO;

	for (int ii = 0; ii < size; ii++)
	{
		_comp *comp = new _comp;
		ZeroMemory(comp, sz_COMP);

		comp->cID = g_infoCtrl[ii].kind;
		if (comp->cID == IC_GRID)
		{
			if (m_bGrid)
				comp->status = btnDown;
			else
				comp->status = btnUp;
		}
		else
		{
			comp->status = btnUp;
		}

		comp->bshow = true;
		comp->imggen = g_infoCtrl[ii].imggen;
		comp->imgmov = g_infoCtrl[ii].imgmov;
		comp->imgsel = g_infoCtrl[ii].imgsel;
		comp->imgdis = g_infoCtrl[ii].imgdis;
		CopyMemory(comp->str, g_infoCtrl[ii].disp, sizeof(g_infoCtrl[ii].disp));
		CopyMemory(comp->comment, g_infoCtrl[ii].com, sizeof(g_infoCtrl[ii].com));

		m_arComs.Add(comp);
	}
}

void CPnCtrl::initScroll(int pos, int end, int day, int total)
{
	SCROLLINFO sInfo;
	sInfo.cbSize = sizeof(SCROLLINFO);
	sInfo.fMask = SIF_ALL|SIF_DISABLENOSCROLL;     
	sInfo.nMin = 0;
	sInfo.nMax = 100;

	if ((total) == 0)
	{
		sInfo.nPos = 100;
		sInfo.nPage = 110;
	}
	else
	{
		sInfo.nPos = (end == total)? 100:(pos*100) / (total);
		sInfo.nPage = (day*100) / (total);
	}

	if ((int)sInfo.nPage == sInfo.nMax)
		sInfo.nPage = sInfo.nMax+1;
	if (m_pScroll)
		m_pScroll->Initialize(sInfo);
}

void CPnCtrl::UpdateScroll(bool bGetData)
{
	if (bGetData)
		getScrInfo();

	switch (m_iMainKind)
	{
	case GK_THREE:	case GK_PNF:	case GK_CLOCK:
	case GK_PV:	case GK_KAGI:	case GK_RENKO:
		if (m_pSlider)
		{
			m_pSlider->SetRange(0, 0);
			m_pSlider->SetPos(0);
			m_pSlider->EnableWindow(FALSE);
		}
		if (m_pScroll)
		{
			// 2006.10.27 schbang 리얼타임시 껌뻑여서 막음
//			initScroll(m_iDispPos, m_iDispEnd, m_iDispDay, m_iTotalDay);
			m_pScroll->EnableWindow(FALSE);
		}
		break;
	default:
		if (m_pSlider)
		{
			int range;
			m_pSlider->SetRange(20, m_iTotalDay);
			if (m_iTotalDay-20 > 0)
			{
				range = (int)((m_iTotalDay-20) / 10);
				if (range == 1)
					range = 2;
				m_pSlider->SetPageSize(range);
				m_pSlider->SetTicFreq(range);
				m_pSlider->SetPos(m_iDispDay);
				m_pSlider->EnableWindow(TRUE);
			}
			else
			{
				range = m_iTotalDay;
				m_pSlider->SetPos(0);
				m_pSlider->EnableWindow(FALSE);
			}
		}
		if (m_pScroll)
		{
			initScroll(m_iDispPos, m_iDispEnd, m_iDispDay, m_iTotalDay);
			m_pScroll->EnableWindow(TRUE);
		}
		break;
	}
}

void CPnCtrl::getScrInfo()
{
	char buf[64];
	memset(&buf, 0x00, 64);
	if (m_pwndGView)
	{
		int len = m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, getScrollData), (long)&buf);

		m_iTotalDay = 0;
		m_iDispPos = 0;
		m_iDispEnd = 0;
		m_iDispDay = 0;

		if (len == 0)
			return;
		
		CString	tmpS;
		tmpS.Format("%.5s", &buf[0]);	m_iTotalDay = atoi(tmpS);
		tmpS.Format("%.5s", &buf[5]);	m_iDispPos = atoi(tmpS);
		tmpS.Format("%.5s", &buf[10]);	m_iDispEnd = atoi(tmpS);
		tmpS.Format("%.5s", &buf[15]);	m_iDispDay = atoi(tmpS);
	}
}

void CPnCtrl::setAutoScroll(int autoID)
{
	switch (m_iMainKind)
	{
	case GK_THREE:	case GK_PNF:	case GK_CLOCK:	
	case GK_PV:	case GK_KAGI:	case GK_RENKO:
		return;
	}

	m_iAutoScroll = autoID;
	switch (autoID)
	{
	case autoPre:
	case autoNext:
		m_iTimerID = timer_ID;
		SetTimer(m_iTimerID, timer_GAP, NULL);
		m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartAutoScroll), TRUE);
		break;
	case autoStop:
		if (m_iTimerID)	KillTimer(m_iTimerID);
		m_iTimerID = 0;

		m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, chartAutoScroll), FALSE);
		break;
	}
}

void CPnCtrl::AutoShiftAction()
{
	int shiftUpDn = disp_GAP;
	if (m_iAutoScroll == autoPre)
		shiftUpDn *= -1;

	BOOL	res = m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(chartCtrl, changeShift), shiftUpDn);

	if (res)
		UpdateScroll();
	else
		setAutoScroll(autoStop);
}

void CPnCtrl::SetGrid(bool bSet)
{
	m_bGrid = bSet;
	struct _comp* comp = SearchCom(IC_GRID);

	if (m_bGrid)
		InvalidateCom(comp, btnDown);
	else
		InvalidateCom(comp, btnUp);

}

void CPnCtrl::SetTable(bool bSet)
{
	m_bTable = bSet;

	struct _comp* comp = SearchCom(IC_TABLE);

	if (m_bTable)
		InvalidateCom(comp, btnDown);
	else
		InvalidateCom(comp, btnUp);
}

BOOL CPnCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0)
	{
		SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, ctrlWheelLeft), NULL);
	}
	else
	{
		SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, ctrlWheelRight), NULL);
	}
	return CPnBaseWnd::OnMouseWheel(nFlags, zDelta, pt);
}
