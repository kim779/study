// ILButton.cpp : implementation file
//

#include "stdafx.h"
#include "ILButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CILButton

#define UMSG_SIGNAL_CLICK		WM_USER+182 //05.09.09  신호등 Click

CILButton::CILButton()
{
	m_pImageList = NULL;
	m_bPushLike  = FALSE;
	m_nNormal    = -1;
	m_nDown		 = -1;
	m_nDisable	 = -1;
	m_nOver		 = -1;
	m_bButtonDown= FALSE;
	m_bCheck	 = FALSE;
	m_bLbuttonDown= FALSE;
	m_bUseToolTip = TRUE;
	m_bUseDrawTitle = FALSE;
	
}

CILButton::~CILButton()
{
	if (m_bUseToolTip)
	{
		if (m_Tooltip.GetSafeHwnd())
		{
			//m_Tooltip.DestroyToolTipCtrl();	
			m_Tooltip.DestroyWindow();
		}
	}
}


BEGIN_MESSAGE_MAP(CILButton, CButton)
	//{{AFX_MSG_MAP(CILButton)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CILButton message handlers

void CILButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

/*==========================================================================================
	Author		 : 도 성 진   Date : 2002-09-24 Time : 오전 11:16:00
 ==========================================================================================
	Function	 : CILButton::FP_vSetImage
	Return		 : void - 
	Parameters	 : 
			CImageList* pImageList - 이미지리스트의 포인터
			int nNormal - Normal 상태의 이미지 인덱스
			int nDown - Down 상태의 이미지 인덱스
			int nDisable - Disable 상태의 이미지 인덱스
			int nOver - Over 상태의 이미지 인덱스
			BOOL bPushLike - PushLike 로버튼을 사용할지 여부
	Note		 : 이미지리스트와 이미지 리스트에서 버튼의 이미지로 사용할 이미지의 인덱스를
					받아서 멤버변수에 저장해 놓는다.
 ==========================================================================================*/
void CILButton::FP_vSetImage(CImageList* pImageList	,							 
							 int nNormal			,
							 int nDown				,
							 BOOL bDrawTitle		,							 
							 CString strTitle		,
							 BOOL bPushLike			,
							 int nDisable			,
							 int nOver											 
							 )
{
	m_pImageList = pImageList;
	m_bPushLike  = bPushLike;
	m_bUseDrawTitle = bDrawTitle;

	if(m_bUseDrawTitle)
		m_szDrawTitle = strTitle;

	m_nNormal = nNormal;					// Normal 상태의 이미지 인덱스
	if(nDown == -1)							// Down 상태의 이미지 인덱스
	{m_nDown = nNormal;}
	else
	{m_nDown = nDown;}

	if(nDisable == -1)						// Disable 상태의 이미지 인덱스
	{m_nDisable = nNormal;}
	else
	{m_nDisable = nDisable;}

	if(nOver == -1)							// Over 상태의 이미지 인덱스
	{m_nOver = nNormal;}
	else
	{m_nOver = nOver;}
}

void CILButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   POINT mouse_position;
    if ((m_bButtonDown) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position)))
	{
		if (::WindowFromPoint(mouse_position) == m_hWnd)
		{
			if ((GetState() & BST_PUSHED) != BST_PUSHED) 
			{
				SetState(TRUE);
				return;
			}
		} else {
			if ((GetState() & BST_PUSHED) == BST_PUSHED) {
				SetState(FALSE);
				return;
			}
		}
	}	
	
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// DC 를 얻어온다.


	RECT r=lpDrawItemStruct->rcItem;					// Rect 를 얻어온다.
	CRect rc = r;

	pDC->SetBkMode(TRANSPARENT);						// 텍스트의 바탕을 투명하게

	if(m_pImageList == NULL)
		return;
	
	if (lpDrawItemStruct->itemState & ODS_DISABLED)		// 비활성 상태
	{
		m_pImageList->Draw(pDC,m_nDisable, CPoint(0,0), ILD_TRANSPARENT);
	}
	else
	{
		if ((lpDrawItemStruct->itemState & ODS_SELECTED) || (m_bPushLike && m_bCheck))// 버튼다운상태
		{
			m_pImageList->Draw(pDC,m_nDown, CPoint(0,0), ILD_TRANSPARENT);
		}
		else																// 보통상태
		{
			m_pImageList->Draw(pDC,m_nNormal, CPoint(0,0), ILD_TRANSPARENT);
		}
	}

	if (m_bUseDrawTitle)
	{
		CFont *pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
		CFont* pOldFont=pDC->SelectObject(pFont);			// get text font

		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		CRect TextRect = lpDrawItemStruct->rcItem;
		if (lpDrawItemStruct->itemState & ODS_DISABLED)		// 비활성 상태
			pDC->SetTextColor(RGB(172,168,153));
		else
			//<<20100422_JS.Kim 솔로몬 버튼 글자색상 변경
			//pDC->SetTextColor(RGB(0,0,0));
			pDC->SetTextColor(RGB(255,255,255));
			//>>
		pDC->DrawText(m_szDrawTitle,TextRect,DT_CENTER|DT_SINGLELINE|DT_VCENTER);

		pDC->SelectObject(pOldFont);
	}
}


BOOL CILButton::OnEraseBkgnd(CDC* pDC) 
{
	//return FALSE;	
	//return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}

/*==========================================================================================
	Author		 : 도 성 진   Date : 2002-09-24 Time : 오후 12:40:50
 ==========================================================================================
	Function	 : CILButton::SetCheck
	Return		 : void - 
	Parameters	 : 
			BOOL bCheck - CheckState 인자
	Note		 : CheckState 를 받아서 저장한다.
 ==========================================================================================*/
void CILButton::SetCheck(BOOL bCheck)
{
	m_bCheck = bCheck;
	Invalidate();
}

/*==========================================================================================
	Author		 : 도 성 진   Date : 2002-09-24 Time : 오후 12:45:43
 ==========================================================================================
	Function	 : CILButton::GetCheck
	Return		 : BOOL - 
	Parameters	 : 
	Note		 : CheckState 를 리턴한다.
 ==========================================================================================*/
BOOL CILButton::GetCheck()
{
	return m_bCheck;
}

void CILButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLbuttonDown = TRUE;
//	SetCapture();
	CButton::OnLButtonDown(nFlags, point);
}

void CILButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	if (::GetCapture() == m_hWnd)
//		ReleaseCapture();
	CRect rect;
	GetClientRect(&rect);
	if (rect.PtInRect(point))
	{
		if(m_bLbuttonDown && m_bPushLike)
		{
			m_bCheck = !m_bCheck;
		}
	}
	m_bLbuttonDown = FALSE;
	
	//GetParent()->SendMessage(UMSG_SIGNAL_CLICK);

	CButton::OnLButtonUp(nFlags, point);
}

void CILButton::FP_vSetToolTipText(CString s)
{
	m_bUseToolTip = TRUE;
	if( ! m_Tooltip.GetSafeHwnd() )
	{
		if( m_Tooltip.Create(this))
		{
				
			if(m_Tooltip.AddTool(this, (LPCTSTR)s))
			{
				m_Tooltip.Activate(1);
			}
		}
	}
	else
	{		
		m_Tooltip.UpdateTipText((LPCTSTR)s,this);
	}
	

//	if (m_pTooltip == NULL)
//		m_pTooltip = new CToolTipCtrl;
//	if(m_pTooltip->m_hWnd==NULL)
//	{
//		if(m_pTooltip->Create(this))
//		{
//			if(m_pTooltip->AddTool(this, (LPCTSTR)s))
//			{
//				m_pTooltip->Activate(1);
//			}
//		}
//	}
//	else
//	{
//		m_pTooltip->UpdateTipText((LPCTSTR)s,this);
//	}
}

void CILButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(m_Tooltip.GetSafeHwnd())
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);
		m_Tooltip.RelayEvent(&msg);

		m_Tooltip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOOWNERZORDER);
	}
//	if(m_pTooltip && NULL != m_pTooltip->m_hWnd)
//	{
//		MSG msg;
//		msg.hwnd = m_hWnd;
//		msg.message = message;
//		msg.wParam = wParam;
//		msg.lParam = lParam;
//		msg.time = 0;
//		msg.pt.x = LOWORD(lParam);
//		msg.pt.y = HIWORD(lParam);
//		m_pTooltip->RelayEvent(&msg);
//	}

}

void CILButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bUseToolTip)
		RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));	
	CButton::OnMouseMove(nFlags, point);
}

void CILButton::OnNcLButtonUp(UINT nFlags, CPoint point)
{
//	if (::GetCapture() == m_hWnd)
//		ReleaseCapture();
//	m_bLbuttonDown = FALSE;
//	m_bLbuttonDown = FALSE;

	CButton::OnNcLButtonUp(nFlags, point);
}

int CILButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CILButton::SetBtnTitle(CString szDrawTitle)
{
	m_szDrawTitle = szDrawTitle;
	Invalidate();
}

CString CILButton::GetBtnTitle()
{
	return m_szDrawTitle;
}
//DEL void CILButton::OnDestroy() 
//DEL {
//DEL 	
//DEL 	CButton::OnDestroy();
//DEL }
