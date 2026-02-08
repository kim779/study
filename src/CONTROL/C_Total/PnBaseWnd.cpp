// PnBaseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_Total.h"
#include "PnBaseWnd.h"
#include "../../chart_dll/gCom/defInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnBaseWnd

CPnBaseWnd::CPnBaseWnd(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info)
{
	m_pApp = (CC_TotalApp *)AfxGetApp();
	m_pView = pView;
	m_pParent = pParent;
	m_pFont = pFont;
	m_ctrlID = ctrlID;
	m_arComs.RemoveAll();

	m_bLButton = false;
	m_bCapture = false;
	m_moveID = 0;
	m_selID = 0;

	m_clrBack = m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 7);
	m_clrFrame = PALETTERGB(0, 0, 0);

	m_wdLine = 1;
	m_moveID = 0;
}

CPnBaseWnd::~CPnBaseWnd()
{
	DeleteComs();
}


BEGIN_MESSAGE_MAP(CPnBaseWnd, CWnd)
	//{{AFX_MSG_MAP(CPnBaseWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnBaseWnd message handlers

struct _comp* CPnBaseWnd::SearchCom(CPoint pt)
{
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		if (comp->cRC.PtInRect(pt) && comp->bshow)
			return comp;
	}
	return NULL;
}

struct _comp* CPnBaseWnd::SearchCom(int ID)
{
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		if (comp->cID == ID)
			return comp;
	}
	
	return NULL;
}

CString CPnBaseWnd::GetPosComment(CPoint pt)
{
	ScreenToClient(&pt);
	CString tmpS;
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		if (comp->cRC.PtInRect(pt) && comp->bshow)
		{
			tmpS = CString(comp->comment);
			tmpS.TrimRight();
			return tmpS;
		}
	}	

	return _T("");
}

void CPnBaseWnd::DeleteComs()
{
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		LIB_DELETE(comp);
	}
	m_arComs.RemoveAll();	
}

void CPnBaseWnd::InvalidateCom(int idc, int status)
{
	struct _comp *comp = SearchCom(idc);
	InvalidateCom(comp, status);
}

void CPnBaseWnd::InvalidateCom(struct _comp *comp, int status)
{
	if (!comp)
		return;

	comp->status = status;
	InvalidateRect(comp->cRC);
}

void CPnBaseWnd::DrawBackground(CDC* pDC)
{
	CRect wRC;
	GetClientRect(&wRC);
	CBrush *brush = m_pApp->GetBrush(m_pView, m_clrBack);
	pDC->FillRect(&wRC, brush);
}

void CPnBaseWnd::DrawFrame(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(1, 1, 1, 1);
	CPen	*fPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, m_clrFrame);
	CPen	*sPen = pDC->SelectObject(fPen);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.left, rect.top);
	pDC->SelectObject(sPen);
}

void CPnBaseWnd::DrawButton(CDC* pDC, int dtype, struct _comp *comp, CImageList* m_pImage)
{
	CRect cRC = comp->cRC;
	CPoint imgPt = CPoint(cRC.left, cRC.top);

	switch (dtype)
	{
	case dtButton:
		if (!m_pImage)
		{
			switch (comp->status)
			{
			case btnDown:
				cRC.DeflateRect(1, 1, 1, 1);
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				cRC.DeflateRect(1, 1, 1, 1);
				pDC->DrawEdge(&cRC, BDR_RAISEDINNER, BF_RECT);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(1, 1, 1, 1);
			CString tmpS = CString(comp->str);
			tmpS.TrimRight();
			int mode = pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(tmpS, cRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			pDC->SetBkMode(mode);
			break;
		}
		switch (comp->status)
		{
		case btnDown:
			m_pImage->Draw(pDC, comp->imgsel, imgPt, ILD_TRANSPARENT);
			break;
		case btnUp:
			m_pImage->Draw(pDC, comp->imggen, imgPt, ILD_TRANSPARENT);
			break;
		case btnMove:
			m_pImage->Draw(pDC, comp->imgmov, imgPt, ILD_TRANSPARENT);
			break;
		case btnDis:
			m_pImage->Draw(pDC, comp->imgdis, imgPt, ILD_TRANSPARENT);
		}
		break;
	case dtColor:
		{
			switch (comp->status)
			{
			case btnDown:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(2, 2, 2, 2);
			CBrush	*cBrush = m_pApp->GetBrush(m_pView, m_crColourBk);
			pDC->FillRect(cRC, cBrush);
		}
		break;
	case dtWidth:
		{
			switch (comp->status)
			{
			case btnDown:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(2, 2, 2, 2);

			CBrush	*cBrush = m_pApp->GetBrush(m_pView, RGB(255, 255, 255));
			pDC->FillRect(cRC, cBrush);

			CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, PALETTERGB(0, 0, 0));
			CPen	*sPen = pDC->SelectObject(cPen);
			int hori = cRC.top + (cRC.Height())/2 - m_wdLine/2;
			for (int ii = 0; ii < m_wdLine; ii++)
			{
				pDC->MoveTo(cRC.left+1, hori+ii+1);
				pDC->LineTo(cRC.right-2, hori+ii+1);
			}
			pDC->SelectObject(sPen);
		}
		break;
	case dtEtc:
		{
			switch (comp->status)
			{
			case btnDown:
			case btnMove:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(2, 2, 2, 2);
			CBrush	*cBrush = m_pApp->GetBrush(m_pView, ::GetSysColor(COLOR_BTNFACE));
			pDC->FillRect(&cRC, cBrush);
			if (m_pImage)
			{
				if (cRC.Width() < 10)
					imgPt.x = cRC.left;
				else
					imgPt.x = cRC.right-szCtrlImgX/2-1;
				imgPt.y = cRC.top;
				m_pImage->Draw(pDC, comp->imgsel, imgPt, ILD_TRANSPARENT);
			}
		}
		break;
	case dtSeparator:
		{
			cRC.DeflateRect(2, 0, 2, 0);

			int height = cRC.top + cRC.Height()/2;

			CPen	*grayPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, PALETTERGB(128,128, 128));
			CPen	*whitePen = m_pApp->GetPen(m_pView, PS_SOLID, 1, PALETTERGB(255, 255, 255));
			CPen	*sPen = pDC->SelectObject(grayPen);

			pDC->MoveTo(cRC.left, height);
			pDC->LineTo(cRC.right, height);

			pDC->SelectObject(whitePen);
			pDC->MoveTo(cRC.left, height+1);
			pDC->LineTo(cRC.right, height+1);
			
			pDC->SelectObject(sPen);
		}
		break;
	}
}


////////////////////////////////////////////////////////////
// SliderCtrl
////////////////////////////////////////////////////////////

CPnSlider::CPnSlider()
{
}

CPnSlider::~CPnSlider()
{
}

BEGIN_MESSAGE_MAP(CPnSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(CPnSlider)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPnSlider::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (zDelta > 0)
	{
		GetParent()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, ctrlWheelLeft), NULL);
	}
	else
	{
		GetParent()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, ctrlWheelRight), NULL);
	}
	return TRUE;
	
	return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CPnSlider::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CSliderCtrl::OnLButtonDown(nFlags, point);
	GetParent()->GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
}

void CPnSlider::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CSliderCtrl::OnLButtonDown(nFlags, point);
	GetParent()->GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
}


/////////////////////////////////////////////////////////////
// ScrollBar
/////////////////////////////////////////////////////////////

CPnScrollBar::CPnScrollBar()
{
}

CPnScrollBar::~CPnScrollBar()
{
}


BEGIN_MESSAGE_MAP(CPnScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CPnScrollBar)
	ON_WM_HSCROLL_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPnScrollBar::Initialize(SCROLLINFO sInfo)
{
	SetScrollInfo(&sInfo);
	if ((int)sInfo.nPage >= sInfo.nMax)
		EnableScrollBar(ESB_DISABLE_BOTH);
	else
		EnableScrollBar(ESB_ENABLE_BOTH);
}

void CPnScrollBar::HScroll(UINT nSBCode, UINT nPos) 
{
	SCROLLINFO sInfo;
	GetScrollInfo(&sInfo, SIF_ALL);  
	int pos = GetScrollPos();

	switch (nSBCode)
	{
	case SB_LINEDOWN:
	case SB_LINEUP:
		GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), 0);
		break;
	case SB_PAGEDOWN:
		if ((nPos = getPos(SB_PAGEDOWN, sInfo.nPage)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), GetScrollPos());
		break;
	case SB_PAGEUP:
		if ((nPos = getPos(SB_PAGEUP)) >= 0)
			SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), GetScrollPos());
		break;
	case SB_THUMBTRACK:
		SetScrollPos(nPos);
		if (pos != GetScrollPos())
			GetOwner()->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_SCROLL_POS, nSBCode), GetScrollPos());
		break;
	default:
		break;
	}
}

void CPnScrollBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CScrollBar::OnLButtonDown(nFlags, point);
	GetParent()->GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
}

void CPnScrollBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CScrollBar::OnLButtonDown(nFlags, point);
	GetParent()->GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
}

int CPnScrollBar::getPos(int type, int gap)
{
	int nPos = -1;
	
	CPoint	pt;
	GetCursorPos(&pt);
	GetOwner()->ScreenToClient(&pt);
	CRect scrollRC;
	GetClientRect(scrollRC);
	ClientToScreen(scrollRC);

	GetOwner()->ScreenToClient(scrollRC);
	int cx = GetSystemMetrics(SM_CXHSCROLL);
	scrollRC.left += cx;
	scrollRC.right -= cx;
	if (scrollRC.PtInRect(pt))
	{
		int xpos = pt.x - scrollRC.left;
		nPos = int(100 * xpos / (float)scrollRC.Width());
		switch (type)
		{
		case SB_PAGEDOWN:
			nPos = nPos - gap + 2;
			break;
		case SB_PAGEUP:
			break;
		}
	}
	return nPos;
}

/////////////////////////////////////////////////////////////////////////////
// CPnToolTip
const int ID_SHOW = 100;

CPnToolTip::CPnToolTip(CWnd* pView, CWnd* pParent, int event)
{
	m_pApp = (CC_TotalApp *)AfxGetApp();
	m_pView = pView;
	m_pParent = pParent;
	m_event = event;

	m_clrBkColor = PALETTERGB(253, 255, 230);
	m_clrTextColor = PALETTERGB(0, 0, 0);

	m_showDelay = 1000;
	m_dispStr = _T("");
	m_fSize = CSize(14, 9);

	m_cntHeight = 1;
	m_cntWidth = 1;
}

CPnToolTip::~CPnToolTip()
{
}


BEGIN_MESSAGE_MAP(CPnToolTip, CWnd)
	//{{AFX_MSG_MAP(CPnToolTip)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnToolTip message handlers

int CPnToolTip::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CPnToolTip::OnPaint() 
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);
   
	CBrush	*inBrush = m_pApp->GetBrush(m_pView, m_clrBkColor);
	CBrush	*sBrush = dc.SelectObject(inBrush);
	dc.Rectangle(rc);

	dc.SelectObject(sBrush);

	int sMode = dc.SetBkMode(TRANSPARENT);
	COLORREF sColor = dc.SetTextColor(m_clrTextColor);
	CFont	*sFont = dc.SelectObject(m_pFont);

	CRect rect;
	GetClientRect(rect);
	CSize size = dc.GetTextExtent(m_dispStr);
	rect.top += 2;
	dc.DrawText(m_dispStr, rect, DT_CENTER|DT_VCENTER);

	dc.SetBkMode(sMode);
	dc.SetTextColor(sColor);
	dc.SelectObject(sFont);
}

void CPnToolTip::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_SHOW)
	{
		if (!checkViewFocus())
		{
			CWnd::OnTimer(nIDEvent);
			return;
		}
		showToolTip();
	}

	CWnd::OnTimer(nIDEvent);
}

void CPnToolTip::OnDestroy() 
{
	hideToolTip();

	CWnd::OnDestroy();
}

void CPnToolTip::OnActivateApp(BOOL bActive, DWORD lParam) 
{
	CWnd::OnActivateApp(bActive, lParam);
	if (!bActive)
		 hideToolTip();	
}


BOOL CPnToolTip::Create(CFont* pFont) 
{
	BOOL bRet = CWnd::CreateEx(NULL, AfxRegisterWndClass(0), NULL, WS_POPUP, 
		0, 0, 0, 0, 
		m_pParent->GetSafeHwnd(), NULL, NULL);

	
	m_pFont = pFont;
	CDC* pDC = GetDC();
	if (pDC)
	{
		CFont	*sFont = pDC->SelectObject(m_pFont);
		m_fSize = pDC->GetOutputTextExtent("A");
		pDC->SelectObject(sFont);	
		ReleaseDC(pDC);
	}

	return bRet;
}

void CPnToolTip::hideToolTip()
{
	KillTimer(ID_SHOW);
	if (IsWindowVisible())
		ShowWindow(SW_HIDE);
}

void CPnToolTip::showToolTip()
{
	KillTimer(ID_SHOW);
	CPoint pt;
	GetCursorPos(&pt);
	CRect wRC;
	m_pParent->GetWindowRect(wRC);

	if (wRC.PtInRect(pt))
	{
		CPoint posPt = CPoint(pt.x+8, pt.y+19);

		CSize size;
		size.cx = GetSystemMetrics(SM_CXSCREEN);
		size.cy = GetSystemMetrics(SM_CYSCREEN);
		if (posPt.x+m_fSize.cx * m_cntWidth+5 > size.cx)
			posPt.x = posPt.x - (m_fSize.cx * m_cntWidth + 5); 
		if (posPt.y+m_fSize.cy*m_cntHeight+5 > size.cy)
			posPt.y = pt.y - (m_fSize.cy*m_cntHeight+5);

		SetWindowPos(&wndTop, posPt.x, posPt.y, 
					m_fSize.cx * m_cntWidth+5, 
					m_fSize.cy * m_cntHeight + 5, SWP_NOACTIVATE);
		::ShowWindow(GetSafeHwnd(), SW_SHOWNOACTIVATE);
	}
}

bool CPnToolTip::checkViewFocus()
{
	CWnd *pFocusWnd = AfxGetApp()->m_pMainWnd->GetFocus();
	if (pFocusWnd == NULL)
	{
		hideToolTip();
		return false;
	}
	return true;
}

void CPnToolTip::RelayEvent(UINT message)
{
	switch (message) 
	{
	case WM_KEYDOWN:
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_NCLBUTTONDOWN:
	case WM_NCRBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		hideToolTip();
		break;      
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
		{
			if (!checkViewFocus())
				break;

			CPoint pt;
			GetCursorPos(&pt);
			CRect wRC;
			m_pParent->GetWindowRect(wRC);
		
			if (wRC.PtInRect(pt))
			{
				char* buf = NULL;
				switch (m_event)
				{
				case GEV_TOOL:
					buf = (char *)m_pParent->SendMessage(m_event, 
						MAKEWPARAM(TOOL_GET_POS_COMMENT, 0), MAKELPARAM(pt.x, pt.y));
					break;
				case GEV_CTRL:
					buf = (char *)m_pParent->SendMessage(m_event, 
						MAKEWPARAM(CTRL_GET_POS_COMMENT, 0), MAKELPARAM(pt.x, pt.y));
					break;
				case GEV_INPUT:
					buf = (char *)m_pParent->SendMessage(m_event, 
						MAKEWPARAM(INPUT_GET_POS_COMMENT, 0), MAKELPARAM(pt.x, pt.y));
					break;
				}
				CString tmpS = CString(buf);
				if (tmpS == m_dispStr)
					break;

				m_dispStr = tmpS;
				if (!tmpS.IsEmpty())
				{
					getTextInfo(m_dispStr, m_cntWidth, m_cntHeight);
					hideToolTip();
					SetTimer(ID_SHOW, 1000, NULL);
				}
				else
					hideToolTip();
			}
			else
			{
				m_dispStr = _T("");
				hideToolTip();
			}
		}	
		break;
	}
}


void CPnToolTip::getTextInfo(CString str, int& cntWidth, int& cntHeight)
{
	cntWidth = 1;
	cntHeight = 1;

	CStringArray arStr;
	arStr.RemoveAll();
	for (int pos = 0; !str.IsEmpty(); )
	{
		if ((pos = str.Find('\n')) == -1)
			break;
		cntHeight++;
		arStr.Add(str.Left(pos));
		str = str.Mid(pos+1);
	}
	for (int ii = 0; ii < arStr.GetSize(); ii++)
		cntWidth = max(arStr.GetAt(ii).GetLength(), cntWidth);
	if (arStr.GetSize() == 0)
		cntWidth = str.GetLength();
	arStr.RemoveAll();
}


/////////////////////////////////////////////////////////////////////////////
// CPnWndEdit
CPnWndEdit::CPnWndEdit(CWnd* pParent, CWnd* pView, int type)
{
	m_pParent = pParent;
	m_pView = pView;
	m_Type = type;
}

CPnWndEdit::~CPnWndEdit()
{
}


BEGIN_MESSAGE_MAP(CPnWndEdit, CEdit)
	//{{AFX_MSG_MAP(CPnWndEdit)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPnWndEdit message handlers

void CPnWndEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString tmpS = _T("");
	GetWindowText(tmpS);

	switch (nChar)
	{
	case VK_RETURN:
		switch (m_Type)
		{
			/*
		case EDT_RTIME:
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipRotate), atoi(tmpS));
			break;
			*/
		case EDT_GIGAN:
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_GIGAN), atoi(tmpS));
			break;
		case EDT_DISP:
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipDisplay), atoi(tmpS));
			break;
		case IC_EDIT:
			m_pParent->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlChgDisp), atoi(tmpS));
			break;
		}
		break;
	case VK_TAB:
		switch (m_Type)
		{
			//case EDT_RTIME:
			case EDT_GIGAN:
			case EDT_DISP:
				m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipTab), m_Type);
				break;
			case IC_EDIT:
				m_pParent->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlITab), m_Type);
				break;
		}
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CPnWndEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	GetParent()->GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
	SetSel(0, -1);
}

void CPnWndEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	GetParent()->GetParent()->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);
	SetSel(0, -1);
}

void CPnWndEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);

	SetSel(0, -1);
}
