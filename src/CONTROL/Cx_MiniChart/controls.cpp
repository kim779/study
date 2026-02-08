#include "stdafx.h"
#include "Controls.h"

/********************************************************************/
// CBaseCombo class
/********************************************************************/

CBaseCombo::CBaseCombo()
{	
	m_hListWnd = nullptr;
	m_pList = nullptr;
	m_pShowWnd = nullptr;
}

CBaseCombo::~CBaseCombo()
{	
}


BEGIN_MESSAGE_MAP(CBaseCombo, CComboBox)
	//{{AFX_MSG_MAP(CBaseCombo)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelChange)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CBaseCombo::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_pShowWnd)
		m_pShowWnd->SendMessage(WM_CTRLMSG, (WPARAM)CTRL_LBUTTONUP);
	CComboBox::OnLButtonUp(nFlags, point);
}

BOOL CBaseCombo::EnableWindow(BOOL bEnable)
{
	const BOOL bRet = CComboBox::EnableWindow(bEnable);
	CWnd*	pFWnd = GetWindow(GW_CHILD);
	CWnd*	pWnd = nullptr;

	if (pFWnd == nullptr)
		return bRet;

	pFWnd->EnableWindow(bEnable);
	pWnd = pFWnd;

	for ( int ii = 0 ; ; ii++ )
	{
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);

		if (pWnd == nullptr)
			break;

		if (pWnd->m_hWnd == pFWnd->m_hWnd)
			break;

		pWnd->EnableWindow(bEnable);		
	}
	
	this->Invalidate(FALSE);
	return bRet;
}

void CBaseCombo::OnDestroy() 
{
	CComboBox::OnDestroy();
	
	if (m_hListWnd)
		m_pList->UnsubclassWindow();
	m_pShowWnd = nullptr;	
}

HBRUSH CBaseCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH	hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		{
			if (m_hListWnd == nullptr)
			{
				m_hListWnd = pWnd->m_hWnd;
				m_pList->SubclassWindow(m_hListWnd);
			}
		}
		break;
	}

	return hbr;
}


void CBaseCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct)
	{
		DrawContent(lpDrawItemStruct->hDC, lpDrawItemStruct->itemID, 
			&lpDrawItemStruct->rcItem, lpDrawItemStruct->itemAction, 
			lpDrawItemStruct->itemState);
	}
}

void CBaseCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

}

int CBaseCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	LONG	style = 0;

	style |= WS_VISIBLE;
	style |= WS_VSCROLL;
	style |= WS_CHILD;
	style |= WS_TABSTOP;
	style |= CBS_DROPDOWNLIST;
	style |= CBS_OWNERDRAWFIXED;
	style |= CBS_HASSTRINGS;
	style &= ~CBS_SORT;
	style &= ~CBS_SIMPLE;
	style &= ~CBS_DROPDOWN;		

	::SetWindowLong(this->m_hWnd, GWL_STYLE, style);

	for ( int ii = 0 ; ii < m_nItemSize ; ii++ )
		AddString(_T(""));

	SetCurSel(0);
	init();

	const BOOL	bEnable = IsWindowEnabled();
	EnableWindow(bEnable);

	if (m_pList == nullptr)
	{
		m_pList = std::make_unique<CListWnd>();
		m_pList->SetWindow(this);
	}
	return 0;
}

void CBaseCombo::OnCloseup() 
{
	GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CBN_CLOSEUP));
}

void CBaseCombo::OnSelChange()
{
	selectOper();
}

/********************************************************************/
// CLineKindCombo class
/********************************************************************/
CLineKindCombo::CLineKindCombo()
{
	m_nLines.Add(PS_SOLID);
	m_nLines.Add(PS_DOT);
	m_nLines.Add(PS_DASH);
	m_nLines.Add(PS_DASHDOT);
	m_nLines.Add(PS_DASHDOTDOT);	
	m_nLine = (int)m_nLines.GetAt(0);
	m_nItemSize = m_nLines.GetSize();
}

CLineKindCombo::~CLineKindCombo()
{
	m_nLines.RemoveAll();
}

BEGIN_MESSAGE_MAP(CLineKindCombo, CBaseCombo)
	//{{AFX_MSG_MAP(CLineKindCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLineKindCombo::init()
{
}

void CLineKindCombo::selectOper() 
{
	const int	sel = GetCurSel();
	CWnd*	pWnd = nullptr;
	
	if (sel == CB_ERR)
		return;			
	else
	{
		m_nLine = (int)m_nLines.GetAt(sel);
		pWnd = GetParent();

		if (pWnd != nullptr)
			pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)this->m_hWnd);
	}
}

void CLineKindCombo::DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem)
{
	CDC		dc;
	CRect		rect;
	CPoint		pt[2];
	COLORREF	clrTemp{};
	const COLORREF	selColor = GetSysColor(COLOR_HIGHLIGHT);
	CPen		pen, *oldpen = nullptr;
	CBrush		brush, *oldbrush = nullptr;
	
	dc.Attach(hDC);
	rect.CopyRect(rcItem);
	dc.FillSolidRect(rect, RGB(255, 255, 255));

	pt[1] = pt[0] = rect.CenterPoint();
	pt[0].x = rect.left + 4;
	pt[1].x = rect.right - 4;

	if (itemState & ODS_SELECTED)	
	{		
		
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldpen = dc.SelectObject(&pen);
		brush.CreateSolidBrush(selColor);
		oldbrush = dc.SelectObject(&brush);		
		dc.Rectangle(rect);
		dc.SelectObject(oldbrush);
		dc.SelectObject(oldpen);
		pen.DeleteObject();
		brush.DeleteObject();
		clrTemp = RGB(255, 255, 255);
	}
	else
		clrTemp = RGB(0, 0, 0);		

	pen.CreatePen(m_nLines.GetAt(itemID), 1, clrTemp);	

	oldpen	= dc.SelectObject(&pen);
	const int	nMode = dc.SetBkMode(TRANSPARENT);
	dc.MoveTo(pt[0]);
	dc.LineTo(pt[1]);
	dc.SelectObject(oldpen);
	pen.DeleteObject();	
	dc.SetBkMode(nMode);

	if (itemState & ODS_SELECTED)	
	{	
		dc.DrawFocusRect(rect);
	}

	dc.Detach();	
}

DWORD CLineKindCombo::GetData()
{
	return (DWORD)m_nLine;
}

void CLineKindCombo::SetData(DWORD input)
{
	m_nLine = (BYTE)input;
	
	for (int ii = 0; ii < m_nLines.GetSize(); ii++)
	{
		if (m_nLine == (int)m_nLines.GetAt(ii))
		{
			SetCurSel(ii);
			return;
		}
	}

	SetCurSel(0);
	m_nLine = m_nLines.GetAt(0);
}

/********************************************************************/
// CLineWidthCombo class
/********************************************************************/

CLineWidthCombo::CLineWidthCombo()
{
	m_nWidth = 0;
	m_nItemSize = 5;
}

CLineWidthCombo::~CLineWidthCombo()
{	
}


BEGIN_MESSAGE_MAP(CLineWidthCombo, CBaseCombo)
	//{{AFX_MSG_MAP(CLineWidthCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLineWidthCombo::init()
{

}

void CLineWidthCombo::selectOper() 
{
	const int	sel = GetCurSel();
	CWnd*	pWnd = nullptr;
	
	if (sel == CB_ERR)
		return;

	m_nWidth = sel;
	pWnd = GetParent();

	if (pWnd != nullptr)
		pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)this->m_hWnd);
}

void CLineWidthCombo::DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem)
{
	CDC		dc;
	CRect		rect[3];
	CSize		size;
	CString		tmpstr = "4 pt";
	CPoint		pt[2];
	int		nWidth = 0;
	COLORREF	clrTemp{};
	const COLORREF	selColor = GetSysColor(COLOR_HIGHLIGHT);
	
	dc.Attach(hDC);
	CFont	font;
	font.CreatePointFont(9*10, _T("±¼¸²"));
	CFont*	oldfont = dc.SelectObject(&font);
	rect[0].CopyRect(rcItem);
	dc.FillSolidRect(rect[0], RGB(255, 255, 255));

	if (itemState & ODS_SELECTED)	
	{		
		dc.FillSolidRect(rect[0], selColor);		
		clrTemp = RGB(255, 255, 255);
	}
	else
		clrTemp = RGB(0, 0, 0);

	rect[2] = rect[1] = rect[0];
	size = dc.GetTextExtent(tmpstr);

	rect[1].left += 2;
	rect[1].right = rect[1].left + size.cx;
	rect[2].left = rect[1].right + 4;
	rect[2].right -= 2;
	nWidth = itemID + 1;
	tmpstr.Format("%d pt", nWidth);

	const int	nMode = dc.SetBkMode(TRANSPARENT);
	const COLORREF	clrText = dc.SetTextColor(clrTemp);
	dc.DrawText(tmpstr, rect[1], DT_VCENTER | DT_SINGLELINE | DT_CENTER);
	dc.SetBkMode(nMode);
	dc.SetTextColor(clrText);
	
	pt[1] = pt[0] = rect[2].CenterPoint();
	pt[0].x = rect[2].left;
	pt[1].x = rect[2].right;
	pt[0].y -= (nWidth / 2);
	pt[1].y -= (nWidth / 2);
	pt[1].y += nWidth;
	rect[2].SetRect(pt[0], pt[1]);
	dc.FillSolidRect(rect[2], clrTemp);
	dc.SelectObject(oldfont);
	font.DeleteObject();

	if (itemState & ODS_SELECTED)	
	{		
		dc.DrawFocusRect(rect[0]);
	}
	
	dc.Detach();
}

DWORD CLineWidthCombo::GetData()
{
	return (DWORD)(m_nWidth + 1);
}	

void CLineWidthCombo::SetData(DWORD input)
{
	if ((int)input <= 0 || (int)input > this->GetCount())
		m_nWidth = 0;
	else
		m_nWidth = (int)input - 1;

	SetCurSel(m_nWidth);	
}

/**********************************************************************/
/** CShowBox							     **/
/**								     **/
/**********************************************************************/

CShowBox::CShowBox(CWnd* pMain, CBaseCombo* pCombo)
{
	m_bLbutton = false;	
	m_pCombo = pCombo;
	m_pMain = pMain;
}

CShowBox::~CShowBox()
{
	m_pCombo = nullptr;
	m_pMain = nullptr;
}


BEGIN_MESSAGE_MAP(CShowBox, CWnd)
	//{{AFX_MSG_MAP(CShowBox)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CShowBox::OnDestroy()
{
	CWnd::OnDestroy();
	KillTimer(1000);
}

void CShowBox::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1000)
	{
		CPoint	pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);

		if (!m_rcButton.PtInRect(pt))
		{
			KillTimer(nIDEvent);
			DrawBitmap(false);
		}
	}
	CWnd::OnTimer(nIDEvent);
}

LRESULT CShowBox::OnControls(WPARAM wParam, LPARAM lParam)
{
	if (wParam == CTRL_LBUTTONUP)
	{
		DrawBitmap(false);
	}
	return 0;
}

BOOL CShowBox::PreTranslateMessage(MSG* pMsg) 
{	
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (m_pCombo && m_pCombo->m_hWnd)
			m_pCombo->SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CShowBox::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);	
	DrawBitmap(false);	
}

void CShowBox::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	
	if (m_pCombo && m_pCombo->m_hWnd)
		m_pCombo->ShowDropDown();

	if (m_rcButton.PtInRect(point))
	{
		DrawBitmap(true);
		SetTimer(1000, 1, nullptr);
	}
	else
		DrawBitmap(false);

	CWnd::OnLButtonDown(nFlags, point);
}

int CShowBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	CRect	rect;
	GetClientRect(rect);

	const int	nHeight = rect.Height();
	
	m_rcShow = m_rcButton = rect;
	m_rcShow.right = m_rcShow.right - nHeight;
	m_rcButton.left = m_rcShow.right + 1;
	m_rcShow.DeflateRect(1, 1, 0, 1);	

	


	return 0;
}

void CShowBox::DrawBitmap(bool bPush)
{
	CDC		memDC;
	CDC*		pDC = GetDC();
	CRect		rect;
	CBitmap* oldBitmap{};
	CString		tmpstr;
	//int		ii = 0, jj = 0;
	m_bLbutton = bPush;

	GetClientRect(rect);
	m_pBitmap.reset(nullptr);
	m_pBitmap = std::make_unique<CBitmap>();
	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap.get());
	memDC.FillSolidRect(rect, RGB(255, 255, 255));
	DrawFill(&memDC);
	DrawButton(&memDC);
	memDC.SelectObject(oldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
	Invalidate(FALSE);
}

void CShowBox::DrawFill(CDC* pDC)
{
	HDC	hDC = pDC->m_hDC;
	UINT	itemID = 0;
	if (m_pCombo && m_pCombo->m_hWnd)
		itemID = m_pCombo->GetCurSel();
	UINT	itemState = 0;
	CRect	rect = m_rcShow;

	if (GetFocus() == this)
		itemState = ODS_SELECTED;

	if (m_pCombo && m_pCombo->m_hWnd)
		m_pCombo->DrawContent(hDC, itemID, rect, 0, itemState, false);
}

void CShowBox::LoadBitmap(CDC *pDC)
{
	CRect	rect;	

	if (m_pBitmap)
	{
		CDC	 memDC;
		CBitmap* oldBitmap = nullptr;
		
		GetClientRect(rect);
		memDC.CreateCompatibleDC(pDC);	

		if (memDC.m_hDC != nullptr) 
			oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap.get());

		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBitmap);
		memDC.DeleteDC();
	}
}

void CShowBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting		
	LoadBitmap(&dc);
}

void CShowBox::DrawButton(CDC* pDC)
{
	UINT	nEnable{};

	if (IsWindowEnabled())
		nEnable = 0;
	else
		nEnable = DFCS_INACTIVE;

	pDC->DrawFrameControl(m_rcButton, DFC_SCROLL, m_bLbutton ? DFCS_SCROLLDOWN|DFCS_PUSHED : DFCS_SCROLLDOWN|nEnable);
}

void CShowBox::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

	if (pNewWnd)
	{
		if (GetFocus() != m_pCombo)
		{
			DrawBitmap(false);
		}
	}
}

/********************************************************************/
// CBaseStatic class
/********************************************************************/

CBaseStatic::CBaseStatic()
{
	m_bPreCreate = false;
}

CBaseStatic::~CBaseStatic()
{
}


BEGIN_MESSAGE_MAP(CBaseStatic, CStatic)
	//{{AFX_MSG_MAP(CBaseStatic)
	ON_WM_DESTROY()
	ON_WM_CREATE()	
	ON_WM_SETFOCUS()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBaseStatic::EnableWindow(BOOL bEnable)
{
	const BOOL bRet = CStatic::EnableWindow(bEnable);
	CWnd*	pFWnd = GetWindow(GW_CHILD);
	CWnd*	pWnd = nullptr;

	if (pFWnd == nullptr)
		return bRet;

	pFWnd->EnableWindow(bEnable);
	pWnd = pFWnd;

	for ( int ii = 0 ; ; ii++ )
	{
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);

		if (pWnd == nullptr)
			break;

		if (pWnd->m_hWnd == pFWnd->m_hWnd)
			break;

		pWnd->EnableWindow(bEnable);		
	}
	
	this->Invalidate(FALSE);
	return bRet;
}

BOOL CBaseStatic::PreCreateWindow(CREATESTRUCT& cs)
{
	m_bPreCreate = true;
	return CStatic::PreCreateWindow(cs);
}

void CBaseStatic::PreSubclassWindow() 
{
	
	if (!m_bPreCreate)
	{
		LONG	value = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
		value |= WS_TABSTOP;

		::SetWindowLong(this->m_hWnd, GWL_STYLE, value);
		SetWindowText("");

		init();	

		const BOOL	bEnable = IsWindowEnabled();
		EnableWindow(bEnable);
	}

	CStatic::PreSubclassWindow();
}

int CBaseStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_bPreCreate)
	{
		LONG	value = ::GetWindowLong(this->m_hWnd, GWL_STYLE);
		value |= WS_TABSTOP;

		::SetWindowLong(this->m_hWnd, GWL_STYLE, value);
		SetWindowText("");

		init();	
	}

	return 0;
}

void CBaseStatic::OnSetFocus(CWnd* pOldWnd)
{
	CStatic::OnSetFocus(pOldWnd);
	setFocusOper(pOldWnd);
}

void CBaseStatic::OnDestroy()
{
	CStatic::OnDestroy();
	destroyOper();
}

/********************************************************************/
// CDrawStatic class
/********************************************************************/
CDrawStatic::CDrawStatic()
{
	m_pCombo = nullptr;
	m_pShow = nullptr;
	memset(m_ctrlID, 0x00, sizeof(m_ctrlID));
}

CDrawStatic::~CDrawStatic()
{

}


BEGIN_MESSAGE_MAP(CDrawStatic, CBaseStatic)
	//{{AFX_MSG_MAP(CDrawStatic)
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDrawStatic::init()
{
	const int	nCtrlID = this->GetDlgCtrlID();
	m_ctrlID[0] = nCtrlID + 1;
	m_ctrlID[1] = nCtrlID + 2;
	CRect	rect;
	GetClientRect(rect);	

	if (m_pCombo)
	{
		CRect	rcTemp;
		const UINT	cbStyle = WS_VSCROLL|WS_CHILD|WS_TABSTOP|CBS_DROPDOWNLIST|CBS_OWNERDRAWFIXED;
		const int	nThick = GetSystemMetrics(SM_CXEDGE);
		const UINT	cbExStyle = 0;

		m_pCombo->CreateEx(cbExStyle, "COMBOBOX", "", cbStyle, CRect(rect.left, rect.top, rect.right, 200), this, m_ctrlID[1]);
		m_pCombo->GetWindowRect(rcTemp);
		int	nSHeight = rcTemp.Height();
		nSHeight = nSHeight - rect.Height();
		rcTemp = CRect(rect.left - nThick, rect.top - nSHeight + nThick, rect.right, 200);
		m_pCombo->MoveWindow(rcTemp);
		m_pCombo->ShowWindow(SW_HIDE);
		m_pCombo->SetItemHeight(-1, 0);
		m_pCombo->SetDroppedWidth(rcTemp.Width());
	}	
	
	if (m_pShow == nullptr)
	{
		m_pShow = std::make_unique<CShowBox>(this, m_pCombo.get());
		m_pShow->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE, rect, this, m_ctrlID[0]); 
		m_pShow->DrawBitmap(false);
		m_pCombo->SetShow(m_pShow.get());
	}
}

void CDrawStatic::destroyOper() 
{
	if (m_pCombo)
	{
		if (m_pCombo->m_hWnd)
			m_pCombo->DestroyWindow();
	}

	if (m_pShow)
	{
		if (m_pShow->m_hWnd)
			m_pShow->DestroyWindow();	
	}
}

void CDrawStatic::setFocusOper(CWnd* pOldWnd) 
{
	if (m_pShow != nullptr)
		m_pShow->SetFocus();
}

LRESULT CDrawStatic::OnControls(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == m_ctrlID[1])
	{
		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:			
			GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CTRL_COMBOSELECT), lParam);			
			m_pShow->SetFocus();
			m_pShow->DrawBitmap(false);
			break;
		case CBN_CLOSEUP:			
			m_pShow->SetFocus();
			m_pShow->DrawBitmap(false);
			break;
		}
	}
	return 0;
}

/********************************************************************/
// CWidthCtrl class
/********************************************************************/
CWidthCtrl::CWidthCtrl()
{
	m_pCombo = std::make_unique<CLineWidthCombo>();
}

CWidthCtrl::~CWidthCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CWidthCtrl, CDrawStatic)
	//{{AFX_MSG_MAP(CWidthCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWidthCtrl::SetLineWidth(int nWidth)
{
	m_pCombo->SetData(nWidth);
	m_pShow->DrawBitmap(false);
}

int CWidthCtrl::GetLineWidth()
{
	return m_pCombo->GetData();
}

/********************************************************************/
// CKindCtrl class
/********************************************************************/
CKindCtrl::CKindCtrl()
{
	m_pCombo = std::make_unique<CLineKindCombo>();	
}

CKindCtrl::~CKindCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CKindCtrl, CDrawStatic)
	//{{AFX_MSG_MAP(CKindCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CKindCtrl::SetLine(int nKind)
{
	m_pCombo->SetData(nKind);
	m_pShow->DrawBitmap(false);
}

int CKindCtrl::GetLine()
{
	return m_pCombo->GetData();
}