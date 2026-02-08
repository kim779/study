// PnToolTip.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnToolTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnToolTip
const int ID_SHOW = 100;

CPnToolTip::CPnToolTip(CWnd* pView, CWnd* pParent, int event)
{
	m_pApp = (CAxisGComApp *)AfxGetApp();
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

LRESULT CPnToolTip::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ACTIVATEAPP)
	{
		if (!(BOOL)wParam)
			hideToolTip();
		return TRUE;
	}

	return CWnd::WindowProc(message, wParam, lParam);
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
				case GEV_AID:
					buf = (char *)m_pParent->SendMessage(m_event, 
						MAKEWPARAM(AID_GET_POS_COMMENT, 0), MAKELPARAM(pt.x, pt.y));
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
				{
					hideToolTip();
				}
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

