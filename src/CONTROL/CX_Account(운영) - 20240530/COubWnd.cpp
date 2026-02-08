// COubWnd.cpp: 구현 파일
//

#include "stdafx.h"
#include "CX_Account.h"
#include "COubWnd.h"
#include "resource.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/memDC.h"


// COubWnd
const int headerH = 20, dataH = 20;
const int titleH = 18, tabW = 140, gap1 = 1;

IMPLEMENT_DYNAMIC(COubWnd, CWnd)

#define TM_MOVECHECK 9999
#define TM_SLIDESTART 9998
#define TM_SLIDING		 9997

COubWnd::COubWnd()
{

}

COubWnd::~COubWnd()
{
}


BEGIN_MESSAGE_MAP(COubWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// COubWnd 메시지 처리기




void COubWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	//xxx::CMemDC	memdc(&dc);

	CRect rc;
	GetClientRect(&rc);

	CPen* pOldPen;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	drawCaption(&dc);
	drawData(&dc);

	pOldPen = (CPen*)dc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

	// exit button
	GetClientRect(&rc);
	rc.left = rc.right - 14;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;
	dc.Rectangle(rc);
	rc.DeflateRect(1, 1);
	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right - 1, rc.bottom - 1);
	dc.MoveTo(rc.right - 1, rc.top);
	dc.LineTo(rc.left, rc.bottom - 1);

	dc.SelectObject(pOldPen);
}

CPen* COubWnd::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CFont* COubWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

void COubWnd::drawCaption(CDC* pDC)
{
	CFont* pOldFont = nullptr;
	CRect	clientRc, rect;

	GetClientRect(&clientRc); 
	rect = clientRc;
	
	rect.top += gap1; 
	rect.bottom = rect.top + titleH;
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));

	pDC->SetTextColor(RGB(38, 55, 100));
	pOldFont = (CFont*)pDC->SelectObject(getAxFont(_T("굴림체"), 9, 3));


	rect.right = rect.left + tabW;
	//pDC->DrawText("조회결과", -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	rect.left = rect.right; rect.right = clientRc.right; rect.DeflateRect(0, 2, 2, 2);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_HIGHLIGHT));
}

void COubWnd::drawData(CDC* pDC)
{
	CRect		rect, irc, drc;
	CFont		font;
	CString		tmpx, temp, string;
	CStringArray	stringAry;

	GetClientRect(&rect);
	rect.top = rect.top + headerH; rect.DeflateRect(1, 1);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	CFont* pOldFont = (CFont*)pDC->SelectObject(getAxFont(_T("굴림체"), 9, 0));

	drc = rect;
	drc.top += 5;
	//drc.bottom = drc.top + dataH;
	string = " 주문대리인이 약정된 계좌입니다.  \n\n 자세한 사항은 화면번호(8782)에서\n\n 확인하여 주시기 바랍니다.";
	pDC->DrawText(string, drc, DT_LEFT|DT_VCENTER|DT_WORDBREAK);

	pDC->SelectObject(pOldFont);
}

void COubWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void COubWnd::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	//PostMessage(WM_CLOSE, 0, 0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void COubWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	GetClientRect(&rc);

	rc.left = rc.right - 14;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 10;
	rc.right = rc.left + 10;

	if (rc.PtInRect(point))
	{
		//m_firstPnt = point;
		SetCapture();
		//GetClientRect(&m_rc);
		m_bDrag = true;
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void COubWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrag)
	{
		ReleaseCapture();
		m_bDrag = false;
		PostMessage(WM_CLOSE, 0, 0);
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void COubWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_MOVECHECK:
		{
			if(m_bShow)
			{
				CRect rec, tmprec;
				m_pParent->GetWindowRect(&rec);
				if (rec != m_recParent)
				{
					GetWindowRect(tmprec);
					tmprec.OffsetRect(rec.left - m_recParent.left, rec.top - m_recParent.top);
					MoveWindow(tmprec, true);
					m_recParent = rec;
				}

				if (rec.left < 100 && rec.top < 100)
				{
					ShowWindow(SW_HIDE);
					KillTimer(TM_SLIDING);
					m_bShow = false;
					PostMessage(WM_CLOSE, 0, 0);
					return;
				}
			}
			else
			{

			}
		}
		break;
		case TM_SLIDESTART:
		{
			KillTimer(TM_SLIDESTART);
			SetTimer(TM_SLIDING, 50, nullptr);
		}
		break;
		case TM_SLIDING:
		{
			CRect tmprec;
			GetWindowRect(tmprec);
			tmprec.bottom -= 10;
			MoveWindow(tmprec, true);

			if (tmprec.Height() < 50)
			{
				KillTimer(TM_SLIDING);
				m_bShow = false;
				PostMessage(WM_CLOSE, 0, 0);
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


int COubWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	GetClientRect(m_rect);
	m_pParent->GetWindowRect(m_recParent);
	m_bShow = true;
	SetTimer(TM_MOVECHECK, 10, nullptr);
	SetTimer(TM_SLIDESTART, 2000, nullptr);
	return 0;
}


void COubWnd::OnDestroy()
{
	CWnd::OnDestroy();
	m_bShow = false;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
