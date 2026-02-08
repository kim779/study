#include "stdafx.h"
#include "Controls.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/********************************************************************/
// CGroupCtrl class
/********************************************************************/

CGroupCtrl::CGroupCtrl()
{
	m_bInit = false;
	m_arMembers.RemoveAll();
}

CGroupCtrl::~CGroupCtrl()
{
	m_bInit = false;
	m_arMembers.RemoveAll();
}


BEGIN_MESSAGE_MAP(CGroupCtrl, CButton)
	//{{AFX_MSG_MAP(CGroupCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGroupCtrl::ShowWindow(int nCmdShow)
{
	if (!m_bInit)
		regMember();

	CWnd*	pWnd = NULL;
	int	nCount = m_arMembers.GetSize();

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		pWnd = CWnd::FromHandle(m_arMembers.GetAt(ii));
		pWnd->ShowWindow(nCmdShow);
	}
	
	return CButton::ShowWindow(nCmdShow);
}

BOOL CGroupCtrl::EnableWindow(BOOL bEnable)
{
	if (!m_bInit)
		regMember();

	CWnd*	pWnd = NULL;
	int	nCount = m_arMembers.GetSize();

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		pWnd = CWnd::FromHandle(m_arMembers.GetAt(ii));
		pWnd->EnableWindow(bEnable);
	}
	
	return CButton::EnableWindow(bEnable);
}

void CGroupCtrl::regMember()
{
	CWnd*	pWnd = NULL;
	CWnd*	pParent = GetParent();

	m_arMembers.RemoveAll();
	pWnd = pParent->GetNextDlgGroupItem(this);

	if (pWnd && pParent)
	{
		while (pWnd->m_hWnd != this->m_hWnd)
		{
			m_arMembers.Add(pWnd->m_hWnd);
			pWnd = pParent->GetNextDlgGroupItem(pWnd);
		}
	}

	m_bInit = true;
}

/********************************************************************/
// CAxNumEdit class
/********************************************************************/

CAxNumEdit::CAxNumEdit()
{
}

CAxNumEdit::~CAxNumEdit()
{
}

BEGIN_MESSAGE_MAP(CAxNumEdit, CEdit)
	//{{AFX_MSG_MAP(CAxNumEdit)
	ON_WM_CHAR()	
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAxNumEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	this->SetSel(0, -1, TRUE);	
}

void CAxNumEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == 8)
		CEdit::OnChar(nChar, nRepCnt, nFlags);

	POINT caret; ::GetCaretPos (&caret);
	caret.x = LOWORD(CharFromPos(caret));

	CString text; GetWindowText(text);

	if (isdigit(nChar)) 
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	else if (nChar == '-')
	{
		if (!caret.x)
		{
			if (((text.GetLength() > 0) && (text[0]!='-')) || (text.GetLength()==0))
				CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			if (text.GetLength())
			{
				int nStart = 0, nEnd = 0;
				this->GetSel(nStart, nEnd);
				
				if (nStart == 0 && nEnd == text.GetLength())
					CEdit::OnChar(nChar, nRepCnt, nFlags);
			}			
		}
	}

	else if (nChar == '.')
	{
		for ( int ii = 0 ; ii < text.GetLength() ; ii++ )
		{
			if (text[ii] == '.')
				return ;
		}

		for ( int ii = 0 ; ii < text.GetLength() ; ii++ )
		{
			if (((text[ii] == 'e') ||(text[ii]=='E')) && (caret.x > ii))
				return ;
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CAxNumEdit::SetValue(double val)
{
	CString tmp;
	tmp.Format ("%G",val);
	SetWindowText (tmp);
}

double CAxNumEdit::GetValue()
{
	CString tmp;
	GetWindowText (tmp);
	return strtod (tmp,NULL);
}

void CAxNumEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	SetSel(0, -1);
}

/********************************************************************/
// CaxdoubleEdit class
/********************************************************************/

CaxdoubleEdit::CaxdoubleEdit()
{
}

CaxdoubleEdit::~CaxdoubleEdit()
{
}


BEGIN_MESSAGE_MAP(CaxdoubleEdit, CAxNumEdit)
	//{{AFX_MSG_MAP(CaxdoubleEdit)
	ON_WM_CHAR()
	ON_WM_SETFOCUS()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CaxdoubleEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_CHAR)
	{
		if (pMsg->wParam < 0 || pMsg->wParam > 128)
			return TRUE;
	}

	return CAxNumEdit::PreTranslateMessage(pMsg);
}

void CaxdoubleEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CAxNumEdit::OnSetFocus(pOldWnd);
	this->SetSel(0, -1, TRUE);	
}

void CaxdoubleEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == 8)
		CEdit::OnChar(nChar, nRepCnt, nFlags);

	POINT caret;
	::GetCaretPos (&caret);
	caret.x = LOWORD (CharFromPos (caret));

	CString text;
	GetWindowText (text);

	if (isdigit(nChar)) 
	{	
		CString	tmpstr;
		int	nPos = text.Find(".");

		if (nPos >= 0)
		{
			tmpstr = text.Mid(nPos);

			int	nStart = 0, nEnd = 0;
			this->GetSel(nStart, nEnd);
			BOOL	bSel = FALSE;

			if (nEnd != nStart) 
				bSel = TRUE;
			
			if (tmpstr.GetLength() >= 3 && !bSel)
				return;
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else if (nChar == '-')
	{
		if (LOWORD(GetSel()) == 0 && HIWORD(GetSel()) == text.GetLength())	// added by schbnag 2005.03.28, 전체선택시 -눌러도 작동가능하게
			CEdit::OnChar(nChar, nRepCnt, nFlags);

		else if (!caret.x)
		{
			if (((text.GetLength() > 0) && (text[0]!='-')) || (text.GetLength()==0))
				CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if (nChar == '.')
	{	
		int	nStart = 0, nEnd = 0;
		this->GetSel(nStart, nEnd);

		if (nStart == 0)
			return;

		CString	tmpstr;
		tmpstr = text.Mid(nEnd);
		if (tmpstr.GetLength() >= 3)
			return;

		for (int i=0; i<text.GetLength(); i++)
		{
			if (text[i] == '.')
				return ;
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

/********************************************************************/
// CBaseCombo class
/********************************************************************/

CBaseCombo::CBaseCombo()
{	
	m_hListWnd = NULL;
	m_pList = NULL;
	m_pShowWnd = NULL;
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
	BOOL bRet = CComboBox::EnableWindow(bEnable);
	CWnd*	pFWnd = GetWindow(GW_CHILD);
	CWnd*	pWnd = NULL;

	if (pFWnd == NULL)
		return bRet;

	pFWnd->EnableWindow(bEnable);
	pWnd = pFWnd;

	for ( int ii = 0 ; ; ii++ )
	{
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);

		if (pWnd == NULL)
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
	
	if (m_pList != NULL)
	{
		delete m_pList;
		m_pList = NULL;
	}

	m_pShowWnd = NULL;	
}

HBRUSH CBaseCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH	hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		{
			if (m_hListWnd == NULL)
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

	m_CurSel = 0;
	SetCurSel(m_CurSel);
	init();

	BOOL	bEnable = IsWindowEnabled();
	EnableWindow(bEnable);

	if (m_pList == NULL)
	{
		m_pList = new CListWnd();
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
	m_CurSel = GetCurSel();
}


/********************************************************************/
// CColorCombo class
/********************************************************************/

CColorCombo::CColorCombo()
{
	m_colors.Add(LINCR);
	m_colors.Add(VOLCR);
	m_colors.Add(BUPCR);
	m_colors.Add(BDNCR);	//#define	MALINE3	BDNCR
	m_colors.Add(AVOLCR);
	m_colors.Add(DEFCR1);
	m_colors.Add(DEFCR2);
	m_colors.Add(DEFCR3);
	m_colors.Add(DEFCR4);	//#define	MALINE4	DEFCR4
	m_colors.Add(DEFCR5);
	m_colors.Add(DEFCR6);	// not used
	m_colors.Add(DEFCR7);
	m_colors.Add(DEFCR8);	//#define	MALINE1	DEFCR8
	m_colors.Add(DEFCR9);
	m_colors.Add(DEFCR10);
	m_colors.Add(DEFCR11);	// not used
	m_colors.Add(DEFCR12);	//#define	MALINE2	DEFCR12
	m_colors.Add(DEFCR13);
	m_colors.Add(DEFCR14);
	m_colors.Add(DEFCR15);


	m_colors.Add(::GetSysColor(COLOR_WINDOWTEXT));	// Others...

	m_CurSel = 0;
	m_color = (COLORREF)m_colors.GetAt(0);
	m_nItemSize = m_colors.GetSize();	
}

CColorCombo::~CColorCombo()
{	
	m_colors.RemoveAll();
}


BEGIN_MESSAGE_MAP(CColorCombo, CBaseCombo)
	//{{AFX_MSG_MAP(CColorCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CColorCombo::init()
{
	
}

void CColorCombo::selectOper()
{
	int	sel = GetCurSel();
	int	nOther = 0;
	if (sel == CB_ERR || sel >= m_colors.GetUpperBound())
	{
		CColorDialog clrDlg(m_color);
		if (clrDlg.DoModal() == IDOK)
		{
			COLORREF*	color = clrDlg.GetSavedCustomColors();
			
			color[0] = m_color;
			m_color = clrDlg.GetColor();
			m_colors.SetAt(m_colors.GetSize() - 1, m_color);
			nOther = 1;
		}
		else
		{
			SetCurSel(m_CurSel);
			m_colors.SetAt(m_CurSel, m_color);
		}		
	}
	else
		m_color = (COLORREF)m_colors.GetAt(sel);

	CWnd*	pWnd = GetParent();
	pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)nOther);	
}

void CColorCombo::DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem)
{
	CDC		dc;
	CRect		rect;	
	int		nCount = 1;

	dc.Attach(hDC);
	rect.CopyRect(rcItem);
	dc.FillSolidRect(rect, RGB(255, 255, 255));
	rect.DeflateRect(1, 1);

	CPen	pen, *oldpen = NULL;
	CBrush	brush, *oldbrush;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&pen);

	brush.CreateSolidBrush(m_colors.GetAt(itemID));
	oldbrush = dc.SelectObject(&brush);
	dc.Rectangle(rect);
	
	COLORREF	txcolor = LIB_GetReverseColor(m_colors.GetAt(itemID));

	if (itemID == (UINT)(this->GetCount() - 1))
	{
		int	bkmode = dc.SetBkMode(TRANSPARENT);
		int	textcolor = dc.SetTextColor(txcolor);	
		
		dc.DrawText(_T("Others.."), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		dc.SetBkMode(bkmode);
		dc.SetTextColor(textcolor);
	}
	
	if (itemState & ODS_SELECTED)
		dc.DrawFocusRect(rect);

	dc.SelectObject(oldbrush);
	dc.SelectObject(oldpen);
	pen.DeleteObject();
	brush.DeleteObject();
	dc.Detach();
}

void CColorCombo::SetData(DWORD input)
{
	m_color = (COLORREF)input;
	for (int ii = 0; ii < m_colors.GetUpperBound(); ii++)
	{
		if (m_color == (COLORREF)m_colors.GetAt(ii))
		{
			SetCurSel(ii);
			return;
		}
	}

	m_colors.SetAt(m_colors.GetSize() - 1, m_color);
	GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)CTRL_OTHER);
	SetCurSel(m_colors.GetUpperBound());
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
	int	sel = GetCurSel();
	CWnd*	pWnd = NULL;
	
	if (sel == CB_ERR)
		return;			
	else
	{
		m_nLine = (int)m_nLines.GetAt(sel);
		pWnd = GetParent();

		if (pWnd != NULL)
			pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CBN_SELCHANGE), (LPARAM)this->m_hWnd);
	}
}

void CLineKindCombo::DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem)
{
	CDC		dc;
	CRect		rect;
	CPoint		pt[2];
	COLORREF	clrTemp;
	COLORREF	selColor = GetSysColor(COLOR_HIGHLIGHT);
	CPen		pen, *oldpen = NULL;
	CBrush		brush, *oldbrush = NULL;
	
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
	int	nMode = dc.SetBkMode(TRANSPARENT);
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
	int	sel = GetCurSel();
	CWnd*	pWnd = NULL;
	
	if (sel == CB_ERR)
		return;

	m_nWidth = sel;
	pWnd = GetParent();

	if (pWnd != NULL)
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
	COLORREF	clrTemp;
	COLORREF	selColor = GetSysColor(COLOR_HIGHLIGHT);
	
	dc.Attach(hDC);
	CFont	font;
	font.CreatePointFont(9*10, _T("굴림체"));
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

	int	nMode = dc.SetBkMode(TRANSPARENT);
	COLORREF	clrText = dc.SetTextColor(clrTemp);
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

/********************************************************************/
// CCheckCombo class
/********************************************************************/

CCheckCombo::CCheckCombo()
{
	m_arData.RemoveAll();
	m_nWidth = 0;
}

CCheckCombo::~CCheckCombo()
{	
}


BEGIN_MESSAGE_MAP(CCheckCombo, CBaseCombo)
	//{{AFX_MSG_MAP(CCheckCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCheckCombo::AddItem(CString szContent, int nValue, bool nCheck)
{
	DWORD	dword = (DWORD)((nCheck << 24) | nValue);
	m_arData.Add(szContent);
	AddString("");	
	int	nSize = GetCount() - 1;
	SetItemData(nSize, dword);
}

int CCheckCombo::drawCheck(CDC* pDC, CRect rect, int Check)
{
	CRect	rcCheck;
	int	nHeight = 0;
	
	rect.DeflateRect(1, 1);	
	rcCheck = rect;
	nHeight = rect.Height();
	rcCheck.right = rcCheck.left + nHeight;
	pDC->DrawFrameControl(rcCheck, DFC_BUTTON, (Check) ? DFCS_CHECKED : DFCS_BUTTONCHECK);
	return nHeight;
	
}

void CCheckCombo::init()
{

}

void CCheckCombo::DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem)
{
	int	nSize = GetCount();
	if (nSize <= 0)
		return;

	CDC	dc;
	CRect	rect, rectORG;	
	dc.Attach(hDC);
	rect.CopyRect(rcItem);
	DWORD	dword = GetItemData(itemID);
	int	nCheck = getCheck(dword);
	int	nData = getData(dword);
	COLORREF	selColor = GetSysColor(COLOR_HIGHLIGHT);
	COLORREF	clrText;
	
	CFont	font;
	font.CreatePointFont(9*10, _T("굴림체"));
	CFont*	oldfont = dc.SelectObject(&font);

	dc.FillSolidRect(rect, RGB(255, 255, 255));

	if (bDrawItem)
	{
		m_nWidth = drawCheck(&dc, rect, nCheck);
		rect.left += m_nWidth + 2;	
	}

	int	nMode = dc.SetBkMode(TRANSPARENT);
	
	if (itemState & ODS_SELECTED)	
	{		
		CPen	pen, *oldpen = NULL;
		CBrush	brush, *oldbrush = NULL;
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldpen = dc.SelectObject(&pen);
		brush.CreateSolidBrush(selColor);
		oldbrush = dc.SelectObject(&brush);		
		dc.Rectangle(rect);
		dc.SelectObject(oldbrush);
		dc.SelectObject(oldpen);
		pen.DeleteObject();
		brush.DeleteObject();		
		clrText = RGB(255, 255, 255);
	}
	else
		clrText = RGB(0, 0, 0);

	COLORREF	color = dc.SetTextColor(clrText);
	CString		tmpstr;

	tmpstr = m_arData.GetAt(itemID);
	dc.DrawText(tmpstr, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);	
	dc.SelectObject(oldfont);	
	font.DeleteObject();
	dc.SetTextColor(color);

	if (itemState & ODS_SELECTED)
		dc.DrawFocusRect(rect);

	dc.SetBkMode(nMode);
	dc.Detach();
}

LRESULT CCheckCombo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_COMMAND)
	{
		if (HIWORD(wParam) == CBN_SELCHANGE || HIWORD(wParam) == CBN_DBLCLK)
		{
			CPoint	pt;			
			
			::GetCursorPos(&pt);
			ScreenToClient(&pt);

			if (pt.x <= (m_nWidth + 2) && pt.x >= 0)
			{
				int	nCurSel = GetCurSel();
				SetCheck(!GetCheck(nCurSel), nCurSel);
				
				CWnd*	pWnd = FromHandle(m_hListWnd);
				pWnd->Invalidate(FALSE);

				DWORD	dword = GetItemData(nCurSel);
				dword = getData(dword);
				GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)dword);
				return TRUE;
			}
		}		
	}
	return CComboBox::WindowProc(message, wParam, lParam);
}

void CCheckCombo::selectOper()
{

}

void CCheckCombo::SetCheck(bool bCheck, int nIndex)
{
	if (nIndex < 0)
		nIndex = GetCurSel();

	if (nIndex < 0)
		return;

	DWORD	dword = 0;
	int	nCheck = 0;
	int	nData = 0;

	dword = GetItemData(nIndex);
	nData = getData(dword);
	nCheck = bCheck;
	nCheck = nCheck << 24;
	dword = nCheck | nData;
	SetItemData(nIndex, dword);
	Invalidate(FALSE);
}

bool CCheckCombo::GetCheck(int nIndex)
{
	DWORD	color = 0;
	
	if (nIndex < 0)
	{
		nIndex = GetCurSel();
	}
	
	if (nIndex >= 0)
	{
		color = GetItemData(nIndex);		
		color = getCheck(color);

		if (color)
			return true;
		else 
			return false;		
	}

	return false;
}

int CCheckCombo::GetIndex(int nData)
{
	int	ii = 0;
	DWORD	dword = 0;
	int	nCount = GetCount();

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		dword = GetItemData(ii);
		dword = getData(dword);

		if (nData == (int)dword)
			return ii;
	}

	return -1;
}

int CCheckCombo::GetData(int nIndex)
{	
	int	nValue = 0;

	if (nIndex < 0)
		return -1;

	nValue = GetItemData(nIndex);
	nValue = getData(nValue);
	return nValue;
}


/**********************************************************************/
/** CShowBox							     **/
/**								     **/
/**********************************************************************/

CShowBox::CShowBox(CWnd* pMain, CBaseCombo* pCombo)
{
	m_pBitmap = NULL;	
	m_bLbutton = false;	
	m_pCombo = pCombo;
	m_pMain = pMain;
}

CShowBox::~CShowBox()
{
	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_pCombo = NULL;
	m_pMain = NULL;
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
		DrawBitmap(false);

	return 0;
}

BOOL CShowBox::PreTranslateMessage(MSG* pMsg) 
{	
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (pMsg->message == WM_KEYDOWN)
	{
		if (LIB_IsWndAlive(m_pCombo))
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
	
	if (LIB_IsWndAlive(m_pCombo))
		m_pCombo->ShowDropDown();

	if (m_rcButton.PtInRect(point))
	{
		DrawBitmap(true);
		SetTimer(1000, 1, NULL);
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

	int	nHeight = rect.Height();
	
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
	CBitmap*	oldBitmap;
	CString		tmpstr;
	int		ii = 0, jj = 0;
	m_bLbutton = bPush;

	GetClientRect(rect);

	if (m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_pBitmap = new CBitmap;
	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	CWnd*	pWnd = GetParent();

	if (pWnd->IsWindowEnabled())
		memDC.FillSolidRect(rect, RGB(255, 255, 255));
	else
		memDC.FillSolidRect(rect, RGB(126, 126, 126));
	
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
	if (LIB_IsWndAlive(m_pCombo))
		itemID = m_pCombo->GetCurSel();
	UINT	itemState = 0;
	CRect	rect = m_rcShow;

	if (GetFocus() == this)
		itemState = ODS_SELECTED;

	if (LIB_IsWndAlive(m_pCombo))
		m_pCombo->DrawContent(hDC, itemID, rect, 0, itemState, false);
}

void CShowBox::LoadBitmap(CDC *pDC)
{
	CRect	rect;	

	if (m_pBitmap)
	{
		CDC	 memDC;
		CBitmap* oldBitmap = NULL;
		
		GetClientRect(rect);
		memDC.CreateCompatibleDC(pDC);	

		if (memDC.m_hDC != NULL) 
			oldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

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
	UINT	nEnable;

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
	BOOL bRet = CStatic::EnableWindow(bEnable);
	CWnd*	pFWnd = GetWindow(GW_CHILD);
	CWnd*	pWnd = NULL;

	if (pFWnd == NULL)
		return bRet;

	pFWnd->EnableWindow(bEnable);
	pWnd = pFWnd;

	for ( int ii = 0 ; ; ii++ )
	{
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);

		if (pWnd == NULL)
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

		BOOL	bEnable = IsWindowEnabled();
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
// CSpinCtrl class
/********************************************************************/

CSpinCtrl::CSpinCtrl()
{
	m_nMin = 1;
	m_nMax = 9999;
	m_pEdit = NULL;
	m_pSpinCtrl = NULL;
	m_editID = 1001;
	m_spinID = 1002;
	m_ftDefault.CreatePointFont(9 * 10, _T("굴림체"));
	m_pOld = NULL;
	m_bSetWindowText = false;
	m_bPreCreate = false;
}

CSpinCtrl::~CSpinCtrl()
{
	m_ftDefault.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSpinCtrl, CBaseStatic)
	//{{AFX_MSG_MAP(CSpinCtrl)
	ON_EN_CHANGE(1001, OnChangeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSpinCtrl::init()
{
	int	nCtrlID = this->GetDlgCtrlID();
	m_editID = nCtrlID + 1;
	m_spinID = nCtrlID + 2;
	CRect	rect;
	CRect	rcEdit, rcSpin;
	GetWindowRect(rect);
	ScreenToClient(rect);
	
	int	nHeight = rect.Height();
	int	nWidth1 = 0, nWidth2 = nHeight;
	DWORD	style = WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT;
	DWORD	exstyle = WS_EX_CLIENTEDGE | WS_EX_WINDOWEDGE;
	rcEdit = rect;
	rcEdit.right -= nWidth2;
	rcSpin = rect;
	rcSpin.left = rcEdit.right + 2;
	
	if (m_pEdit == NULL)
	{
		m_pEdit = new CAxNumEdit();
		m_pEdit->CreateEx(exstyle, "EDIT", NULL, style, rcEdit, this, m_editID, NULL);
		m_pEdit->SetFont(&m_ftDefault);
	}

	if (m_pSpinCtrl == NULL)
	{
		m_pSpinCtrl = new CSpinButtonCtrl();
				
		style = WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_AUTOBUDDY | UDS_NOTHOUSANDS;		
		m_pSpinCtrl->Create(style, rcSpin, this, m_spinID);		
		m_pOld = m_pSpinCtrl->SetBuddy(m_pEdit);
	}	

	SetMinMax(m_nMin, m_nMax);
}

void CSpinCtrl::setFocusOper(CWnd* pOldWnd) 
{	
	if (m_pEdit)
		m_pEdit->SetFocus();
}

void CSpinCtrl::destroyOper()
{
	m_pSpinCtrl->SetBuddy(m_pOld);
	LIB_DeleteWnd(m_pEdit);
	LIB_DeleteWnd(m_pSpinCtrl);	
}

void CSpinCtrl::OnChangeEdit()
{
	CString		tmpstr;
	int		nValue = 0;
	bool		bSel = false;
	
	if (m_pEdit) 
	{
		m_pEdit->GetWindowText(tmpstr);
		nValue = atoi(tmpstr);

		if (nValue < m_nMin)
		{
			nValue = m_nMin;
			bSel = true;
		}
		else if (nValue > m_nMax)
		{
			nValue = m_nMax;
			bSel = true;
		}
		else
			return;

		tmpstr.Format("%d", nValue);
		m_pEdit->SetWindowText(tmpstr);		

		if (bSel)
		{
			m_pEdit->SetSel(0, -1, TRUE);
		}
	}
}

void CSpinCtrl::SetMinMax(int nMin, int nMax) 
{ 
	if (nMin < 0) m_nMin = 0; 
	else m_nMin = nMin;
	if (nMax < 0) m_nMax = 9999;
	else m_nMax = nMax;

	CString	tmpstr;
	tmpstr.Format("%d", m_nMax);

	if (m_pSpinCtrl)
	{
		m_pSpinCtrl->SetRange(m_nMin, m_nMax);
	}
	if (m_pEdit)
	{
		m_pEdit->SetLimitText(tmpstr.GetLength());
	}
}


BOOL CSpinCtrl::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
		if (m_pEdit)
			m_pEdit->SetSel(0, -1, TRUE);
		break;
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)
		{
			CString	string = _T("");
			int	nValue = 0;

			if (m_pEdit)
				m_pEdit->GetWindowText(string);
			
			if (string.IsEmpty())
				return TRUE;

			nValue = atoi(string);
			
			if (pMsg->wParam == VK_UP)
			{
				nValue++;
				if (!CheckValue(nValue))
					return TRUE;
				string.Format("%d", nValue);
				if (m_pEdit)
					m_pEdit->SetWindowText(string);

				m_pEdit->SetSel(0, -1, TRUE);
				return TRUE;
			}

			if (pMsg->wParam == VK_DOWN)
			{
				nValue--;

				if (!CheckValue(nValue))
					return TRUE;

				string.Format("%d", nValue);

				if (m_pEdit)
					m_pEdit->SetWindowText(string);

				m_pEdit->SetSel(0, -1, TRUE);
				return TRUE;
			}
		}
		break;
	}

	return CBaseStatic::PreTranslateMessage(pMsg);
}

BOOL CSpinCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (LOWORD(wParam) == m_editID)
	{
		if (HIWORD(wParam) == EN_CHANGE)
		{				
			//m_pEdit->HideCaret();
			CWnd* pWnd = GetParent();
			int	nLimit = m_pEdit->GetLimitText();
			CString		tmpstr;
			int		nValue = 0;
			m_pEdit->GetWindowText(tmpstr);

			if (tmpstr.GetLength() == nLimit)
			{
				nValue = atoi(tmpstr);
				if (nValue < m_nMin)
				{
					tmpstr.Format("%d", m_nMin);
					m_pEdit->SetWindowText(tmpstr);
					m_pEdit->SetSel(0, -1, TRUE);
				}
				else if (nValue > m_nMax)
				{
					tmpstr.Format("%d", m_nMax);
					m_pEdit->SetWindowText(tmpstr);
					m_pEdit->SetSel(0, -1, TRUE);
				}				
			}			
			
			if (pWnd != NULL)
			{
				if (!m_bSetWindowText)
					pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CTRL_EDITCHANGE), (LPARAM)(m_pEdit->m_hWnd));
				else
					m_bSetWindowText = false;
			}
		}		
	}
	return CBaseStatic::OnCommand(wParam, lParam);
}

void CSpinCtrl::SetNumber(int nNum)
{
	CString	tmpstr;
	
	if (nNum < m_nMin)
		tmpstr.Format("%d", m_nMin);
	else if (nNum > m_nMax)
		tmpstr.Format("%d", m_nMax);
	else
		tmpstr.Format("%d", nNum);

	if (m_pEdit)
	{
		m_bSetWindowText = true;
		m_pEdit->SetWindowText(tmpstr);		
	}
}

int CSpinCtrl::GetNumber()
{
	CString tmpstr;

	if (m_pEdit)
		m_pEdit->GetWindowText(tmpstr);

	int	nValue = 0;

	nValue = atoi(tmpstr);

	if (nValue < m_nMin)
	{
		nValue = m_nMin;
		tmpstr.Format("%d", m_nMin);
		m_bSetWindowText = true;
		m_pEdit->SetWindowText(tmpstr);
	}
	else if (nValue > m_nMax)
		nValue = m_nMax;

	return nValue;
}

void CSpinCtrl::ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	if (m_pEdit)
		m_pEdit->ModifyStyle(dwRemove, dwAdd, nFlags);
}

/********************************************************************/
// CAxFontDialog class
/********************************************************************/

IMPLEMENT_DYNAMIC(CAxFontDialog, CFontDialog)

CAxFontDialog::CAxFontDialog(LPLOGFONT lplfInitial, DWORD dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) : 
	CFontDialog(lplfInitial, dwFlags, pdcPrinter, pParentWnd)
{
}


BEGIN_MESSAGE_MAP(CAxFontDialog, CFontDialog)
	//{{AFX_MSG_MAP(CAxFontDialog)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CAxFontDialog::OnInitDialog() 
{
	CFontDialog::OnInitDialog();
	
	CRect	rect;

	GetWindowRect(rect);
	int	nHeight = rect.Height() / (4 * 1);

	rect.bottom -= nHeight;
	this->MoveWindow(rect);
	return TRUE;
}

void CAxFontDialog::OnDestroy() 
{
	CFontDialog::OnDestroy();
}

/********************************************************************/
// CFontStatic class
/********************************************************************/
CFontCtrl::CFontCtrl()
{
	m_pStatic = NULL;
	m_pButton = NULL;
	m_staticID = 1001;
	m_buttonID = 1002;
	m_szFontName = _T("굴림체");
	m_nSize = 10;
	m_bBold = FW_NORMAL;
	m_bItalic = 0;
	m_ftDefault.DeleteObject();
}

CFontCtrl::~CFontCtrl()
{
	m_ftDefault.DeleteObject();	
}

BEGIN_MESSAGE_MAP(CFontCtrl, CBaseStatic)
	//{{AFX_MSG_MAP(CFontCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFontCtrl::init() 
{
	int	nCtrlID = this->GetDlgCtrlID();
	m_staticID = nCtrlID + 1;
	m_buttonID = nCtrlID + 2;
	CRect	rect;
	CRect	rcStatic, rcButton;
	GetClientRect(rect);
	int	nHeight = rect.Height();
	int	nWidth1 = 0, nWidth2 = nHeight;
	DWORD	style = WS_BORDER | WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY;
	DWORD	exstyle = 0;
	
	rcStatic = rect;
	rcStatic.right -= nWidth2;	
	rcButton = rect;
	rcButton.left = rcStatic.right + 2;
	
	if (m_pStatic == NULL)
	{
		m_pStatic = new CStatic();
		m_pStatic->CreateEx(exstyle, "STATIC", NULL, style, rcStatic, this, m_staticID);
	}

	if (m_pButton == NULL)
	{
		m_pButton = new CButton();
		style = WS_CHILD | WS_VISIBLE;		
		m_pButton->Create("...", style, rcButton, this, m_buttonID);
	}		
}

void CFontCtrl::destroyOper() 
{
	LIB_DeleteWnd(m_pStatic);
	LIB_DeleteWnd(m_pButton);
}

void CFontCtrl::setFocusOper(CWnd* pOldWnd) 
{
	if (m_pButton != NULL)
		m_pButton->SetFocus();
}

BOOL CFontCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (LOWORD(wParam) == m_buttonID)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			DWORD	dwFlags = CF_ANSIONLY | CF_SCREENFONTS | CF_NOSCRIPTSEL ;
		
			CClientDC dc(this);
			CWnd*	pWnd = GetParent();
	
			LOGFONT	logfont;

			memset(&logfont, 0x00, sizeof(logfont));
			logfont.lfHeight         = -MulDiv(m_nSize, dc.GetDeviceCaps(LOGPIXELSY), 72);
			logfont.lfWidth          = 0;
			logfont.lfEscapement     = 0;
			logfont.lfOrientation    = 0;
			logfont.lfWeight         = m_bBold;
			logfont.lfItalic         = m_bItalic;
			logfont.lfUnderline      = false;
			logfont.lfStrikeOut      = false;
			logfont.lfCharSet        = HANGUL_CHARSET;
			logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
			logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
			logfont.lfQuality        = DEFAULT_QUALITY;
			logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
			strcpy_s(logfont.lfFaceName, LF_FACESIZE, m_szFontName);			

			CAxFontDialog	dlg(&logfont, dwFlags) ;
			
			if (dlg.DoModal() == IDOK)
			{	
				m_bBold = dlg.m_cf.lpLogFont->lfWeight;	
				m_bItalic = dlg.m_cf.lpLogFont->lfItalic;
				m_nSize = dlg.GetSize() / 10;			
				m_szFontName = dlg.GetFaceName();

				m_ftDefault.DeleteObject();

				if (m_szFontName.IsEmpty())
				{
					m_szFontName = _T("굴림체");
				}

				logfont.lfHeight = m_nSize * 10;
				logfont.lfWeight = m_bBold;
				logfont.lfItalic = m_bItalic;
				strcpy_s(logfont.lfFaceName, LF_FACESIZE, m_szFontName);

				m_ftDefault.CreatePointFontIndirect(&logfont);
				m_pStatic->SetFont(&m_ftDefault);				
				CString tmpstr;

				tmpstr.Format("[%d] %s", m_nSize, m_szFontName);				
				
				if (pWnd)
					pWnd->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CTRL_EDITCHANGE), (LPARAM)(this->m_hWnd));

				m_pStatic->SetWindowText(tmpstr);				
				m_pStatic->ModifyStyle(NULL, NULL, SWP_DRAWFRAME);
				m_pStatic->Invalidate(TRUE);			
			}
		}
	}
	else if (LOWORD(wParam) == m_staticID)
	{
		if (HIWORD(wParam) == BN_CLICKED)
		{
			if (m_pButton)
				m_pButton->SendMessage(WM_LBUTTONDOWN);
		}
	}
	
	return CStatic::OnCommand(wParam, lParam);
}

void CFontCtrl::SetFontInfo(CString szFaceName, BYTE fstyle, BYTE fweight, BYTE fpoint)
{
	m_bItalic = fstyle;
	m_bBold = fweight;
	m_szFontName = szFaceName;
	m_nSize = fpoint;
	m_ftDefault.DeleteObject();

	LOGFONT	logfont;

	memset(&logfont, 0x00, sizeof(logfont));
	logfont.lfHeight         = m_nSize * 10;
	logfont.lfWidth          = 0;
	logfont.lfEscapement     = 0;
	logfont.lfOrientation    = 0;
	logfont.lfWeight         = m_bBold;
	logfont.lfItalic         = m_bItalic;
	logfont.lfUnderline      = false;
	logfont.lfStrikeOut      = false;
	logfont.lfCharSet        = HANGUL_CHARSET;
	logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality        = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(logfont.lfFaceName, LF_FACESIZE, m_szFontName);

	m_ftDefault.CreatePointFontIndirect(&logfont);
	m_pStatic->SetFont(&m_ftDefault);
	CString tmpstr;

	tmpstr.Format("[%d] %s", m_nSize, m_szFontName);

	if (!m_szFontName.IsEmpty())
	{
		m_pStatic->SetWindowText(tmpstr);
		m_pStatic->UpdateWindow();
	}
}

void CFontCtrl::GetFontInfo(CString& szFaceName, BYTE& fstyle, BYTE& fweight, BYTE& fpoint)
{
	szFaceName = m_szFontName;
	fpoint = m_nSize;
	fstyle = m_bItalic;
	fweight = m_bBold;
}

/********************************************************************/
// CDrawStatic class
/********************************************************************/
CDrawStatic::CDrawStatic()
{
	m_pCombo = NULL;
	m_pShow = NULL;
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
	int	nCtrlID = this->GetDlgCtrlID();
	m_ctrlID[0] = nCtrlID + 1;
	m_ctrlID[1] = nCtrlID + 2;
	CRect	rect;
	GetClientRect(rect);	

	if (m_pCombo)
	{
		CRect	rcTemp;
		UINT	cbStyle = WS_VSCROLL|WS_CHILD|WS_TABSTOP|CBS_DROPDOWNLIST|CBS_OWNERDRAWFIXED;
		UINT	cbExStyle = 0;
		int	nThick = GetSystemMetrics(SM_CXEDGE);

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
	
	if (m_pShow == NULL)
	{
		m_pShow = new CShowBox(this, m_pCombo);
		m_pShow->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, m_ctrlID[0]); 
		m_pShow->DrawBitmap(false);
		m_pCombo->SetShow(m_pShow);
	}
}

void CDrawStatic::destroyOper() 
{
	if (LIB_IsWndAlive(m_pCombo))
		LIB_DeleteWnd(m_pCombo);

	if (LIB_IsWndAlive(m_pShow))
		LIB_DeleteWnd(m_pShow);
}

void CDrawStatic::setFocusOper(CWnd* pOldWnd) 
{
	if (m_pShow != NULL)
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
		default:
			break;
		}
	}
	return 0;
}

/********************************************************************/
// CColorCtrl class
/********************************************************************/
CColorCtrl::CColorCtrl()
{
	m_pCombo = new CColorCombo;	
}

CColorCtrl::~CColorCtrl()
{	
}

BEGIN_MESSAGE_MAP(CColorCtrl, CDrawStatic)
	//{{AFX_MSG_MAP(CColorCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



void CColorCtrl::SetColor(COLORREF color)
{
	m_pCombo->SetData(color);	
	m_pShow->DrawBitmap(false);
}

COLORREF CColorCtrl::GetColor()
{
	return m_pCombo->GetData();
}



/********************************************************************/
// CWidthCtrl class
/********************************************************************/
CWidthCtrl::CWidthCtrl()
{
	m_pCombo = new CLineWidthCombo;
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
	m_pCombo = new CLineKindCombo;	
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

/********************************************************************/
// CCheckCBCtrl class
/********************************************************************/

CCheckCBCtrl::CCheckCBCtrl()
{
	m_pCombo = new CCheckCombo;	
}

CCheckCBCtrl::~CCheckCBCtrl()
{	
}

void CCheckCBCtrl::SetCheck(bool bCheck, int nIndex)
{
	int	nCurSel = m_pCombo->GetCurSel();
	
	if (nIndex == nCurSel)
	{
		if (GetFocus() == m_pShow)
			m_pShow->DrawBitmap(false);
		else
			m_pShow->DrawBitmap(true);
	}

	((CCheckCombo*)m_pCombo)->SetCheck(bCheck, nIndex); 
}

BEGIN_MESSAGE_MAP(CCheckCBCtrl, CDrawStatic)
	//{{AFX_MSG_MAP(CCheckCBCtrl)
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

LRESULT CCheckCBCtrl::OnControls(WPARAM wParam, LPARAM lParam)
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
		case CBN_SELENDOK:
			GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(this->GetDlgCtrlID(), CTRL_COMBOCHECK), lParam);
			break;
		}
	}

	return 0;
}


