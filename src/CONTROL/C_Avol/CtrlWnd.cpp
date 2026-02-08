// CtrlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_Avol.h"
#include "CtrlWnd.h"
#include "ctrl.h"
#include "../../h/axiscgrp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_SLIDER	100
#define	maxBTN	6
const int gapHEIGHT = 0;
const int gapWIDTH = 1;

#define	disp_GAP	5
#define	timer_ID	1004
#define	timer_GAP	200


/////////////////////////////////////////////////////////////////////////////
// CCtrlWnd

CCtrlWnd::CCtrlWnd(CWnd *pView, CWnd *pParent)
{
	m_pView = pView;
	m_pParent = pParent;

	m_ctrlSAVE = ctl_NO;

	m_pScroll = nullptr;
	m_pSlider = nullptr;

	m_totalDay = 0;
	m_dispPos = 0;
	m_dispEnd = 0;
	m_dispDay = 0;

	m_clrBack = RGB(238,242,223);
	m_clrText = RGB(0,0,0);
	m_clrForm = RGB(238,238,255);

	m_bCtrlEnable = false;
	m_bLDown = false;
	m_bCapture = false;
	m_nSlider = 0;

	m_pApp = (CC_AVolApp *)AfxGetApp();
	if (m_pApp->CreateImage())
		m_pCtrlImage = m_pApp->GetCtrlImage();
	else
		m_pCtrlImage = nullptr;

	m_TimerID = 0;
	m_AutoScroll = autoStop;
}

CCtrlWnd::~CCtrlWnd()
{
}


BEGIN_MESSAGE_MAP(CCtrlWnd, CWnd)
	//{{AFX_MSG_MAP(CCtrlWnd)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CM_CTRL, OnCtrl)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCtrlWnd message handlers

LRESULT CCtrlWnd::OnCtrl(WPARAM wParam, LPARAM lParam)
{
	int	pos{};

	switch (LOWORD(wParam))
	{
	case CTRL_Enable:
		m_bCtrlEnable = (HIWORD(wParam) == TRUE)? true:false;
		m_pScroll->EnableWindow(m_bCtrlEnable);
		m_pSlider->EnableWindow(m_bCtrlEnable);
		break;
	case CTRL_UpdateScr:
		UpdateScroll();
		break;
	case CTRL_ChgScrPos:
		if (m_AutoScroll)
		{
			setAutoScroll(autoStop);
			break;
		}

		switch ((int)HIWORD(wParam))
		{
		case SB_LINEUP:
			GetScrInfo();
			if (m_dispPos-1 <= 0)
				break;

			m_dispPos--;
			m_dispEnd--;

			m_pParent->SendMessage(CM_GRP,
				MAKEWPARAM(GRP_Control, ChgDispPos), MAKELPARAM(m_dispPos, m_dispEnd));
			InitScroll(m_dispPos, m_dispEnd, m_dispDay, m_totalDay);

			break;
		case SB_LINEDOWN:
			GetScrInfo();
			if (m_dispEnd >= m_totalDay)	break;

			m_dispPos++;
			m_dispEnd++;

			m_pParent->SendMessage(CM_GRP, 
				MAKEWPARAM(GRP_Control, ChgDispPos), MAKELPARAM(m_dispPos, m_dispEnd));
			InitScroll(m_dispPos, m_dispEnd, m_dispDay, m_totalDay);

			break;
		case SB_PAGEDOWN:
		case SB_PAGEUP:
		case SB_THUMBTRACK:
			pos = (int)lParam;
			if (pos >= 0)
			{
				int dispPos = (int)((pos) * (m_totalDay) / 100); 
				int dispEnd = m_dispDay + dispPos;
				if (dispEnd > m_totalDay)
				{
					const int gap = dispEnd - m_totalDay;
					dispPos -= gap;
					dispEnd -= gap;
				}
				m_pParent->SendMessage(CM_GRP, 
					MAKEWPARAM(GRP_Control, ChgDispPos), MAKELPARAM(dispPos, dispEnd));
			}
			break;
		}
		break;
	case CTRL_Clear:
		if (m_AutoScroll)
			setAutoScroll(autoStop);

	case CTRL_Focus:
		if (m_pSlider->IsWindowEnabled())
			m_pSlider->SetFocus();
		break;
	}
	return TRUE;
}

void CCtrlWnd::OnDestroy() 
{
	if (m_AutoScroll)	KillTimer(m_TimerID);

	DeleteAllBtn();

	CWnd::OnDestroy();
}

int CCtrlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CCtrlWnd::OnPaint() 
{
	CPaintDC dc(this);
	if (m_totalDay == 0)
		m_bCtrlEnable = FALSE;
	else
		m_bCtrlEnable = TRUE;

	CDC* pDC = &dc;

	CRect	rc;
	GetClientRect(&rc);
	const COLORREF bgColor = m_clrBack = GetSysColor(COLOR_BTNFACE);
	CBrush	*cBrush = m_pApp->GetBrush(m_pView, bgColor);
	pDC->FillRect(&rc, cBrush);

	for (int ii = ctl_ZOOMOUT, jj = 0; ii <= ctl_EXCEL; ii++, jj += 3)
	{
//		if (!m_MapCont.Lookup(ii, pInfoCTL))
//			continue;
		
		auto ft = _mapCont.find(ii);
		if (ft == _mapCont.end())
			continue;
		DrawBtn(pDC, jj, ft->second.get());
	}
}

void CCtrlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)
		return;

	RedrawCtrl(true);
	Invalidate();
}

void CCtrlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_bCtrlEnable)
		return;

	const int selectID = SearchPointInControl(point);
	if (selectID == ctl_NO)
		return;

	m_ctrlSAVE = selectID;
	auto ft = _mapCont.find(int(selectID));
	if (ft != _mapCont.end())
	{
		ft->second->pushed = true;
		InvalidateRect(ft->second->rect);
	}
	
	if (!m_bCapture)
	{
		::SetCapture(this->m_hWnd);
		m_bCapture = true;
	}
	m_bLDown = true;
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CCtrlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!m_bCtrlEnable)
		return;
	if (m_bCapture)
	{
		::ReleaseCapture();
		m_bCapture = false;
	}
	m_bLDown = false;

	const int selectID = SearchPointInControl(point);

	if (selectID == ctl_NO || m_ctrlSAVE != selectID)
	{		
		auto ft = _mapCont.find(int(m_ctrlSAVE));
		if (ft == _mapCont.end())
			return;
		
		ft->second->pushed = false;
		InvalidateRect(ft->second->rect);
		m_ctrlSAVE = ctl_NO;
		CWnd::OnLButtonUp(nFlags, point);
		return;	
	}
	
	switch (selectID)
	{
	case ctl_ZOOMIN:
		if (m_AutoScroll)	break;

		m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ChgDispScale), -1);
		UpdateScroll(true);
		break;
	case ctl_ZOOMOUT:
		if (m_AutoScroll)	break;

		m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ChgDispScale), 1);
		UpdateScroll(true);
		break;
	case ctl_PRE:
		if (m_AutoScroll)	break;

		setAutoScroll(autoPre);
		break;
	case ctl_STOP:
		if (m_AutoScroll)	setAutoScroll(autoStop);

		break;
	case ctl_NEXT:
		if (m_AutoScroll)	break;

		setAutoScroll(autoNext);
		break;
	case ctl_EXCEL:
		m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ExcelData), 0);
		break;
	}


	auto ft = _mapCont.find(int(selectID));
	if (ft != _mapCont.end())
	{
		ft->second->pushed = false;
		InvalidateRect(ft->second->rect);
	}
	m_ctrlSAVE = ctl_NO;
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CCtrlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);
}

void CCtrlWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (m_AutoScroll)
		setAutoScroll(autoStop);

	if (m_pSlider && m_pSlider->IsWindowEnabled() && pScrollBar->GetDlgCtrlID() == IDC_SLIDER
		&& m_pSlider->GetRangeMax() >= 20)
	{
		
		const int pos = m_pSlider->GetPos();
		if (m_nSlider != pos)
		{
			m_nSlider = pos;
			if (m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ChgDisplay), pos))
				UpdateScroll();
		}
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCtrlWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == timer_ID)
		AutoShiftAction();
	
	CWnd::OnTimer(nIDEvent);
}

void CCtrlWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

void CCtrlWnd::InitCtrl(CString sDay)
{
	DeleteAllBtn();
	m_ctrlSAVE = ctl_NO;

	if (sDay.IsEmpty())
	{
		Invalidate();
		return;
	}

	ParsingScrInfo(sDay);
	UpdateScroll(true);
	RedrawCtrl(false);
}

void CCtrlWnd::AddControlClass(int key, CRect rc, CString text)
{
	auto mt = _mapCont.emplace( std::make_pair(key, std::make_unique<_ctlINFO>()) );

	mt.first->second->pushed = false;
	mt.first->second->rect = rc;
	mt.first->second->text = text;

	switch (key)
	{
	case ctl_ZOOMOUT:
		mt.first->second->imgup = IMG_ZOOMOUT;
		mt.first->second->imgdown = IMG_ZOOMOUT + 2;
		break;
	case ctl_ZOOMIN:
		mt.first->second->imgup = IMG_ZOOMIN;
		mt.first->second->imgdown = IMG_ZOOMIN + 2;
		break;
	case ctl_PRE:
		mt.first->second->imgup = IMG_PRE;
		mt.first->second->imgdown = IMG_PRE + 2;
		break;
	case ctl_STOP:
		mt.first->second->imgup = IMG_STOP;
		mt.first->second->imgdown = IMG_STOP + 2;
		break;
	case ctl_NEXT:
		mt.first->second->imgup = IMG_NEXT;
		mt.first->second->imgdown = IMG_NEXT + 2;
		break;
	case ctl_EXCEL:
		mt.first->second->imgup = IMG_EXCEL;
		mt.first->second->imgdown = IMG_EXCEL + 2;
		break;
	}

	//if (bNewCreate)
	//	m_MapCont.SetAt(key, pInfoCTL);
}

void CCtrlWnd::DeleteAllBtn()
{
	_mapCont.clear();
}

int CCtrlWnd::SearchPointInControl(CPoint pt)
{
	for (int ii = ctl_ZOOMOUT; ii <= ctl_EXCEL; ii++)
	{
		auto ft = _mapCont.find(ii);
		if (ft == _mapCont.end())
			continue;
		if (ft->second->rect.PtInRect(pt))
			return ii;
	}

	return ctl_NO;
}

void CCtrlWnd::InitScroll(int pos, int end, int day, int total)
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

void CCtrlWnd::RedrawCtrl(bool bResize)
{
	CRect	btnRC;

	CRect	WindowRC;
	CRect	ClientRC;
	GetWindowRect(WindowRC);
	if (WindowRC.IsRectEmpty())
		return;

	ClientRC.SetRect(0, 0, WindowRC.Width(), WindowRC.Height());
	ClientRC.DeflateRect(1, 0);
	DeleteAllBtn();

	btnRC.top    = ClientRC.top + gapHEIGHT;
	btnRC.bottom = ClientRC.bottom - gapHEIGHT;
	const int	BtnWidthb = btnRC.Height() + 1;
	const int	btnCnt = maxBTN;

	CRect	RC;
	CRect	typeRC;
	CRect	kindRC;
	CRect	sliderRC;
	CRect	scrollRC;
	
	btnRC.right = ClientRC.right - 1;
	btnRC.left = btnRC.right - BtnWidthb * btnCnt;

	typeRC = kindRC = sliderRC = scrollRC = CRect(0,0,0,0);

	if (btnRC.left < ClientRC.left)
	{
		btnRC.left = ClientRC.right;
	}
	else
	{
		const int width = (btnRC.left - ClientRC.left);

		scrollRC.top    = ClientRC.top + gapHEIGHT;
		scrollRC.bottom = ClientRC.bottom - gapHEIGHT;
		
		sliderRC.top    = ClientRC.top + gapHEIGHT;
		sliderRC.bottom = ClientRC.bottom - gapHEIGHT;

		if (width > 100)
		{
			scrollRC.left = ClientRC.left;
			scrollRC.right = btnRC.left - 90;
			sliderRC.left = scrollRC.right + 1;
			sliderRC.right = btnRC.left - 1;
		}
		else if (width > 10)
		{
			scrollRC.left = ClientRC.left;
			scrollRC.right = btnRC.left;
		}
	}

	CString text;
	int idx = 0;

	for (int ii = 0; ii < maxBTN; ii++)
	{
		RC.top    = btnRC.top;
		RC.bottom = btnRC.bottom;
		
		switch (ii+1)
		{
		case ctl_ZOOMOUT:
		case ctl_ZOOMIN:
		case ctl_PRE:
		case ctl_STOP:
		case ctl_NEXT:
		case ctl_EXCEL:
			RC.left   = btnRC.left + (idx * BtnWidthb) + 3;
			break;
		default:
			continue;
		}

		RC.right  = RC.left + BtnWidthb;

		switch (ii+1)
		{
		case ctl_ZOOMOUT:
			text = "+";		break;
		case ctl_ZOOMIN:
			text = "-";		break;
		case ctl_PRE:
			text = "ÀÌÀü";		break;
		case ctl_STOP:
			text = "¸ØÃã";		break;
		case ctl_NEXT:
			text = "´ÙÀ½";		break;
		case ctl_EXCEL:
			text = "¿¢¼¿";		break;
		}
		AddControlClass(ii+ctl_ZOOMOUT, RC, text);
		idx++;
	}

	if (m_pScroll == nullptr)
	{
		m_pScroll = std::make_unique<CExScrollBar>();
		m_pScroll->Create(WS_CHILD|WS_VISIBLE|SBS_HORZ|SBS_BOTTOMALIGN, CRect(0, 0, 0, 0), this, 0);
		m_pScroll->SetOwner(this);
		m_pScroll->EnableWindow(FALSE);
	}
	if (m_pScroll)
		m_pScroll->MoveWindow(scrollRC);


	if (m_pSlider == nullptr)
	{
		m_pSlider = std::make_unique <CExSlider>();
		m_pSlider->Create(WS_CHILD|WS_VISIBLE|TBS_HORZ|TBS_BOTTOM|TBS_AUTOTICKS|TBS_TOOLTIPS,
			CRect(0, 0, 0, 0), this, IDC_SLIDER);
		m_pSlider->SetOwner(this);
		m_pSlider->EnableWindow(FALSE);
	}
	if (m_pSlider)
		m_pSlider->MoveWindow(sliderRC);

	Invalidate();
}

void CCtrlWnd::UpdateScroll(bool bGetData)
{
	if (bGetData)
	{
		GetScrInfo();
		if (m_pSlider)
		{
			int range{};
			m_pSlider->SetRange(20, m_totalDay);
			if (m_totalDay-20 > 0)
			{
				range = (int)((m_totalDay-20) / 10);
				if (range == 1)
					range = 2;
				m_pSlider->SetPageSize(range);
				m_pSlider->SetTicFreq(range);
				m_pSlider->SetPos(m_dispDay);
				m_pSlider->EnableWindow(TRUE);
			}
			else
			{
				range = m_totalDay;
				m_pSlider->SetPos(0);
				m_pSlider->EnableWindow(FALSE);
			}
		}
	}

	if (m_pScroll)
	{
		InitScroll(m_dispPos, m_dispEnd, m_dispDay, m_totalDay);
		m_pScroll->EnableWindow(TRUE);			
	}
}

void CCtrlWnd::GetScrInfo()
{
	char buf[64];
	ZeroMemory(buf, 64);
	const int len = m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, GetScrData), (long)buf);
	CString tmpS = CString(buf, len);

	if (tmpS.IsEmpty())
		return;
	
	ParsingScrInfo(tmpS);
}

void CCtrlWnd::ParsingScrInfo(CString tmpS)
{
	if (tmpS.IsEmpty())
	{
		m_totalDay = 0;
		m_dispPos = 0;
		m_dispEnd = 0;
		m_dispDay = 0;
		return;
	}

	if (!tmpS.IsEmpty() && tmpS.Right(1) != '\t')
		tmpS += "\t";
	
	CString tmpS_;
	int pos= 0;
	for (int ii = 0; !tmpS.IsEmpty(); ii++)
	{
		pos = tmpS.Find('\t');
		if (pos != -1)
		{
			tmpS_ = tmpS.Left(pos);
			switch (ii)
			{
			case 0:
				m_totalDay = atoi(tmpS_);
				break;
			case 1:
				m_dispPos = atoi(tmpS_);
				break;
			case 2:
				m_dispEnd = atoi(tmpS_);
				break;
			case 3:
				m_dispDay = atoi(tmpS_);
				break;
			}
		}
		tmpS = tmpS.Mid(pos+1);
	}
}

void CCtrlWnd::DrawBtn(CDC* pDC, int selectID, _ctlINFO* pInfoCTL)
{
	CPoint	imagePt;
	imagePt.x = pInfoCTL->rect.left + 1;
	imagePt.y = pInfoCTL->rect.top + 1;

	if (m_pCtrlImage)
	{
		if (pInfoCTL->pushed)
			m_pCtrlImage->Draw(pDC, pInfoCTL->imgdown, imagePt, ILD_NORMAL);	
		else
			m_pCtrlImage->Draw(pDC, pInfoCTL->imgup, imagePt, ILD_NORMAL);
	}
	else
	{
		pDC->SetBkMode(TRANSPARENT);
		if (!pInfoCTL->pushed)
		{
			pDC->DrawFrameControl(&pInfoCTL->rect, DFC_BUTTON, DFCS_BUTTONPUSH);
		}
		else
		{
			pDC->DrawFrameControl(&pInfoCTL->rect, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
		}
		
		CRect	rc;
		rc.SetRect(pInfoCTL->rect.left + 1, pInfoCTL->rect.top + 1, 
			pInfoCTL->rect.right - 1, pInfoCTL->rect.bottom - 1);
		pDC->DrawText(pInfoCTL->text, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
}

void CCtrlWnd::setAutoScroll(int autoID)
{
	m_AutoScroll = autoID;
	switch (autoID)
	{
	case autoPre:
	case autoNext:
		m_TimerID = timer_ID;
		SetTimer(m_TimerID, timer_GAP, nullptr);
		m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, autoScroll), TRUE);
		break;
	case autoStop:
		if (m_TimerID)	KillTimer(m_TimerID);
		m_TimerID = 0;

		m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, autoScroll), FALSE);
		break;
	}
}

void CCtrlWnd::AutoShiftAction()
{
	int shiftUpDn = disp_GAP;
	if (m_AutoScroll == autoPre)	shiftUpDn *= -1;

	const BOOL	res = m_pParent->SendMessage(CM_GRP, MAKEWPARAM(GRP_Control, ChgShift), shiftUpDn);

	if (res)
		UpdateScroll();
	else
		setAutoScroll(autoStop);
}
