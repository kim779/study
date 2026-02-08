// DropButton.cpp : implementation file
//

#include "stdafx.h"
#include "DropBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDropList::CDropList()
{
	m_nPrevSelection	= 0;
	m_nGroupCount		= 0;

	m_pHeaderFont		= NULL;
	m_pBkBrush		= NULL;
	m_pBkHighlightBrush	= NULL;
	m_pBkPen		= NULL;

	m_pTextFont		= NULL;
	m_pCurListItem = NULL;
}

CDropList::~CDropList()
{
	if (m_pHeaderFont)
	{
		if (m_pHeaderFont->GetSafeHandle())
			m_pHeaderFont->DeleteObject();
		//delete m_pHeaderFont;	m_pHeaderFont = NULL;m_pTriPen
		m_pHeaderFont = nullptr;
	}
	if (m_pBkBrush)
	{
		//delete m_pBkBrush;	m_pBkBrush = NULL;
	}
	if (m_pBkHighlightBrush) 
	{
		//delete m_pBkHighlightBrush;	m_pBkHighlightBrush = NULL;
	}
	if (m_pBkPen)
	{
		//delete m_pBkPen;	m_pBkPen = NULL;
	}
	ClearItems();
}

void CDropList::ClearItems()
{
	POSITION Pos = m_ItemList.GetHeadPosition();
	while (Pos)
	{
		m_pCurListItem.reset((LISTITEM*)m_ItemList.GetNext(Pos));
		if (m_pCurListItem)
		{
			///delete m_pCurListItem;	m_pCurListItem = NULL;
			m_pCurListItem = nullptr;
		}
	}
	m_ItemList.RemoveAll();
	m_nGroupCount = 0;
	m_nPrevSelection = 0;
}

BEGIN_MESSAGE_MAP(CDropList, CListBox)
	//{{AFX_MSG_MAP(CDropList)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CDropList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pBkBrush = std::make_unique<CBrush>(RGB(255,255,255));
	m_pBkPen = std::make_unique<CPen>(PS_SOLID, 1, RGB(255, 255, 255));
	m_pBkHighlightBrush = std::make_unique<CBrush>(RGB(247,247,239));

	SetItemHeight(-1,16);
	return 0;
}
void CDropList::OnSize(UINT nType, int cx, int cy) 
{
	CListBox::OnSize(nType, cx, cy);
	GetClientRect(m_WindowRect);		
}

void CDropList::OnKillFocus(CWnd* pNewWnd) 
{
	CListBox::OnKillFocus(pNewWnd);
	GetParent()->ShowWindow(SW_HIDE);
	if (pNewWnd == ((CListWnd*)GetParent())->m_pParent)
	{
	} 
	else
	{
		((CDropButton*)((CListWnd*)GetParent())->m_pParent)->m_bClked = false;
	}
}

void CDropList::OnSetFocus(CWnd* pOldWnd) 
{
	CListBox::OnSetFocus(pOldWnd);

		
}
void CDropList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if(lpDrawItemStruct->itemID == LB_ERR )
		return;
     
  	GetText(lpDrawItemStruct->itemID, m_csText);
	m_pCurListItem.reset( (LISTITEM*)m_ItemList.GetAt(m_ItemList.FindIndex(lpDrawItemStruct->itemID)) );
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect ItemRect(lpDrawItemStruct->rcItem);
	ItemRect.InflateRect(1,1);
	DrawItem(pDC, ItemRect, lpDrawItemStruct->itemState & ODS_SELECTED);
}

void CDropList::DrawItem(CDC* pDC, CRect ItemRect, BOOL bSelected)
{
	CPen *pOldPen = pDC->SelectObject(m_pBkPen.get());
	CBrush *pOldBrush = pDC->SelectObject(m_pBkBrush.get());
	const COLORREF oldColor = pDC->SetTextColor(m_pCurListItem->crTextColor);

	if (bSelected && !m_pCurListItem->btHeader)
	{
		pDC->SelectObject(m_pBkHighlightBrush.get());
		pDC->SetTextColor(m_pCurListItem->crTextHighlightColor);
	}
	pDC->Rectangle(ItemRect);

	const int oldMode = pDC->SetBkMode(TRANSPARENT);

	ItemRect.left = 4;
	ItemRect.right -= 5;
	if(m_pCurListItem->btHeader)
	{
		pDC->SelectObject(m_pHeaderFont.get());
		pDC->DrawText(m_csText, m_csText.GetLength(), ItemRect, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_CALCRECT);
		pDC->DrawText(m_csText, m_csText.GetLength(), ItemRect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);

		CPen LinePen(PS_SOLID, 1, m_pCurListItem->crTextColor);
		pDC->SelectObject(LinePen);

		pDC->MoveTo(ItemRect.right + 2, ItemRect.top + (ItemRect.Height() / 2));
		pDC->LineTo(m_WindowRect.right - 2, ItemRect.top + (ItemRect.Height() / 2));
	}
	else
	{
		pDC->SelectObject(m_pTextFont);
		pDC->DrawText(m_csText, m_csText.GetLength(), ItemRect, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_END_ELLIPSIS);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldMode);
}

void CDropList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
}

HBRUSH CDropList::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)(m_pBkBrush->GetSafeHandle() );
}

void CDropList::SetPreviousSelection(int nPrevSelection)
{
	m_nPrevSelection = nPrevSelection;
}

int CDropList::AddString(LPCTSTR lpszItem)
{
	m_pCurListItem = std::make_shared<LISTITEM>();
	m_pCurListItem->crTextColor = RGB(0,0,0);
	m_pCurListItem->crTextHighlightColor = RGB(0,0,0);
	m_pCurListItem->btHeader = 0;

	if (!m_ItemList.GetCount())
	{
		m_nGroupCount++;
	}
	else if (m_ItemList.GetCount() == 1) 
		SetCurSel(m_nPrevSelection);

	return AddToList(1, lpszItem);
}

int CDropList::AddString(LPCTSTR lpszItem, int GroupPos, COLORREF crTextColor, COLORREF crTextHighlightColor)
{
	if(GroupPos > m_nGroupCount + 1)
		return -1;
	
	m_pCurListItem = std::make_shared<LISTITEM>();
	m_pCurListItem->crTextColor = crTextColor;
	m_pCurListItem->crTextHighlightColor = crTextHighlightColor;
	m_pCurListItem->btHeader = 0;

	if (GroupPos == m_nGroupCount + 1)
	{
		m_nGroupCount++;
		m_pCurListItem->btHeader = 1;
	}
	
	return AddToList(GroupPos, lpszItem);
}

int CDropList::AddToList(int nGroup, CString csText)
{
	if (m_pCurListItem->btHeader)
	{
		m_ItemList.AddTail(m_pCurListItem.get());
		return CListBox::AddString(csText);
	}
	
	int nListBoxItem = 0, nCurGroup = 1;	
	LISTITEM* pItem{};
	POSITION Pos = m_ItemList.GetHeadPosition();
	POSITION OldPos = Pos;
	while (Pos)
	{
		pItem = (LISTITEM*)m_ItemList.GetNext(Pos);
		if(pItem->btHeader)
			nCurGroup++;

		nListBoxItem++;
		if(nCurGroup == nGroup)
			break;
	}

	if(nGroup == 1)
	{
		Pos = OldPos;
		nListBoxItem = 0;
	}
	else
		OldPos = Pos;

	CString csCurText;
	while (Pos)
	{
		OldPos = Pos;
		pItem = (LISTITEM*)m_ItemList.GetNext(Pos);
		GetText(nListBoxItem, csCurText);
		if(csText.CompareNoCase(csCurText) < 0)
		{
			m_ItemList.InsertBefore(OldPos, m_pCurListItem.get());
			return InsertString(nListBoxItem, csText);
		}
		nListBoxItem++;
	}

	m_ItemList.AddTail(m_pCurListItem.get());
	return CListBox::AddString(csText);
}

int CDropList::DeleteString(UINT nIndex)
{
	if(nIndex >= (UINT)GetCount())
		return -1;
	
	POSITION Pos = m_ItemList.FindIndex(nIndex);
	POSITION LastPos = Pos;
	m_pCurListItem.reset((LISTITEM*)m_ItemList.GetNext(Pos));
	if (m_pCurListItem->btHeader)
	{
		m_nGroupCount--;
		while (Pos)
		{
			//delete m_pCurListItem;	m_pCurListItem = NULL;
			m_ItemList.RemoveAt(LastPos);
			CListBox::DeleteString(nIndex);
			LastPos = Pos;
			m_pCurListItem.reset((LISTITEM*)m_ItemList.GetNext(Pos));
			if (m_pCurListItem->btHeader)
				break;
		}
	}

	if (!m_pCurListItem->btHeader || LastPos && !Pos)
	{
		//delete m_pCurListItem;
		//m_pCurListItem = NULL;
		m_ItemList.RemoveAt(LastPos);
		m_pCurListItem = nullptr;
		return CListBox::DeleteString(nIndex);
	}
	return 1;
}

void CDropList::ResetContent()
{
	ClearItems();
	CListBox::ResetContent();
}

void CDropList::SetBkColor(COLORREF crColor)
{
	if (m_pBkBrush)
	{
		//delete m_pBkBrush;	m_pBkBrush = NULL;
		m_pBkBrush = nullptr;
	}
	if (m_pBkPen)
	{
		//delete m_pBkPen;	m_pBkPen = NULL;
		m_pBkPen = nullptr;
	}
	m_pBkPen	= std::make_unique<CPen>(PS_SOLID, 1, crColor);
	m_pBkBrush	= std::make_unique<CBrush>(crColor);	
	RedrawWindow();
}

void CDropList::SetHighlightBkColor(COLORREF crColor)
{
	if (m_pBkHighlightBrush)
	{	
//		delete m_pBkHighlightBrush;
//		m_pBkHighlightBrush = NULL;
		m_pBkHighlightBrush = nullptr;
	}
	m_pBkHighlightBrush = std::make_unique< CBrush >(crColor);
	RedrawWindow();
}

void CDropList::SetFont(CFont* pFont, BOOL bRedraw)
{
	if (m_pTextFont)
		return;
	m_pTextFont = pFont;
	CWnd::SetFont(pFont);

	LOGFONT lpLogFont;
	m_pTextFont->GetLogFont(&lpLogFont);

	lpLogFont.lfWeight = FW_BOLD;
	
	if (m_pHeaderFont)
	{
		if (m_pHeaderFont->GetSafeHandle())
			m_pHeaderFont->DeleteObject();
//		delete m_pHeaderFont;	
		m_pHeaderFont = nullptr;
	}
	m_pHeaderFont = make_unique<CFont>();
	m_pHeaderFont->CreateFontIndirect(&lpLogFont);

	SetItemHeight(-1,lpLogFont.lfHeight);
}
	

/////////////////////////////////////////////////////////////////////////////
// CListWnd
/////////////////////////////////////////////////////////////////////////////
CListWnd::CListWnd()
{
	m_pDropList = NULL;
	m_bShow = false;
	m_pParent = NULL;
}

CListWnd::~CListWnd()
{
	if (m_pDropList)
	{
		//delete m_pDropList;	m_pDropList = NULL;
	}
}

BEGIN_MESSAGE_MAP(CListWnd, CWnd)
	//{{AFX_MSG_MAP(CListWnd)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(ID_CUSTOMLIST, OnSelectionChange)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CListWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

}

BOOL CListWnd::Create(CWnd* pParentWnd, UINT uID)
{
	CString csWndClassName = AfxRegisterWndClass( CS_SAVEBITS , NULL, NULL, NULL );

	if (!CreateEx(WS_EX_TOPMOST, csWndClassName, NULL, WS_BORDER|WS_POPUP, 0, 0, 200, 200, pParentWnd->GetSafeHwnd(), NULL, NULL))
		return FALSE;

	m_pDropList = std::make_unique<CDropList>();
	if (!m_pDropList->Create(WS_CHILD|WS_VSCROLL|WS_VISIBLE|LBS_NOTIFY|LBS_HASSTRINGS|LBS_OWNERDRAWFIXED|LBS_NOINTEGRALHEIGHT, CRect(0,0,0,0), this, ID_CUSTOMLIST))
		return FALSE;
	m_pParent = pParentWnd;
	return TRUE;
}

void CListWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (!m_pDropList)
		return;
	CRect WindowRect;
	GetClientRect(WindowRect);
	m_pDropList->MoveWindow(WindowRect);
}

void CListWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	if (!bShow)
	{	
		GetParent()->RedrawWindow();	
	}
}
void CListWnd::OnSelectionChange()
{
	const LISTITEM* pItem = m_pDropList->GetCurListItem();
	if (pItem->btHeader)
		return;
	
	ShowWindow(SW_HIDE);		
	m_bShow = false;
	((CDropButton*)m_pParent)->m_bClked = false;

	CRect WindowRect;
	GetWindowRect(WindowRect);

	POINT pPoint;
	GetCursorPos(&pPoint);
	if(!WindowRect.PtInRect(pPoint))
	{
		m_pDropList->SetCurSel(m_pDropList->GetPreviousSelection());
		return;
	}
	m_pParent->SendMessage(CM_SELECTION_CHANGED, m_pDropList->GetCurSel(), 0);
}

/////////////////////////////////////////////////////////////////////////////
// CDropButton
/////////////////////////////////////////////////////////////////////////////
CDropButton::CDropButton()
{
	m_nListHeight	= 100;

	m_bClked	= false;
	m_bOver		= false;
	m_bDeleteFont 	= false;
	m_bChangeTitle 	= true;

	m_pDropWnd	= NULL;

	m_pBkBrush 	= NULL;
	m_pBkOverBrush	= NULL;
	m_pBorderPen	= NULL;
	m_pTriBrush 	= NULL;
	m_pTriPen	= NULL;

	m_pTextFont	= NULL;
	m_crTxtColor = NULL;
	m_crTxtOverColor = NULL;
	m_nListWidth = 0;
	m_pParent = NULL;
	m_uID = 0;
}

CDropButton::~CDropButton()
{
	if (m_pDropWnd)
	{
//		delete	m_pDropWnd;	m_pDropWnd = NULL;
	}
	if (m_pBkBrush)
	{
//		delete	m_pBkBrush;	m_pBkBrush = NULL;
	}
	if (m_pBkOverBrush)
	{
//		delete	m_pBkOverBrush;	m_pBkOverBrush = NULL;
	}
	if (m_pBorderPen)
	{
//		delete	m_pBorderPen;	m_pBorderPen = NULL;
	}
	if (m_pTriBrush)
	{
//		delete	m_pTriBrush;	m_pTriBrush = NULL;
	}
	if (m_pTriPen)
	{
//		delete	m_pTriPen;	m_pTriPen = NULL;
	}
	if (m_bDeleteFont)
	{
		if (m_pTextFont->GetSafeHandle())
			m_pTextFont->DeleteObject();
		//delete m_pTextFont;	m_pTextFont = NULL;
		m_pTextFont = nullptr;
	}
}

BEGIN_MESSAGE_MAP(CDropButton, CButton)
	//{{AFX_MSG_MAP(CDropButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CM_SELECTION_CHANGED, OnSelectionChanged)
END_MESSAGE_MAP()


BOOL CDropButton::Create(CString csCaption, CRect Rect, CWnd* pParent, UINT uID, COLORREF crBkColor, COLORREF crBkOverColor, COLORREF crTxtColor, COLORREF crTxtOverColor, CFont* pTextFont, COLORREF crBkListColor)
{
	if (crBkColor == -1)
	{
		m_pBkBrush = std::make_unique<CBrush>(RGB(221,217,205));
		m_pBkOverBrush = std::make_unique<CBrush>(RGB(233,229,217));
	}
	else
	{
		m_pBkBrush = std::make_unique<CBrush>(crBkColor);
		m_pBkOverBrush = std::make_unique<CBrush>(crBkOverColor);
	}
	m_pBorderPen = std::make_unique<CPen>(PS_SOLID, 1, RGB(0,0,0));

	// Save the Button Font

	m_pTextFont = std::make_shared<CFont>();
	m_pTextFont->CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_CHARACTER_PRECIS,
		CLIP_LH_ANGLES, DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arial");

	if (pTextFont) {
		m_bDeleteFont = true;
		m_pTextFont.reset(pTextFont);
	}

	// Save the text color
	m_crTxtColor = crTxtColor;
	if(m_crTxtColor == -1)
		m_crTxtColor = RGB(0,0,0);
    
	// Save the text over color
	m_crTxtOverColor = crTxtOverColor;
	if(m_crTxtOverColor == -1)
		m_crTxtOverColor = RGB(0,0,0);

	// Triangle Brush
	m_pTriBrush = std::make_unique<CBrush>(m_crTxtColor); 
	m_pTriPen = std::make_unique<CPen>(PS_SOLID, 1, m_crTxtColor); 

	// Create the CButton
	if(!CButton::Create(csCaption, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW, Rect, pParent, uID))
		return FALSE;
	
	// Get the Client Rect
	GetClientRect(m_WindowRect);
	m_nListWidth = m_WindowRect.Width();

	// Create the List box
	m_pDropWnd = std::make_unique <CListWnd>();
	if (!m_pDropWnd->Create(this, uID + 1))
		return FALSE;

	// Set the Font to the list box
	m_pDropWnd->m_pDropList->SetFont(m_pTextFont.get());
	m_pDropWnd->m_pDropList->SetBkColor(crBkListColor);
	m_pParent = pParent;
	m_uID = uID;
	return TRUE;
}

BOOL CDropButton::Init(CString csCaption, CRect Rect, CWnd* pParent, UINT uID, COLORREF crBkColor, COLORREF crBkOverColor, COLORREF crTxtColor, COLORREF crTxtOverColor, CFont* pTextFont, COLORREF crBkListColor)
{
	if (crBkColor == -1)
	{
		m_pBkBrush		= std::make_unique< CBrush >(RGB(221,217,205));
		m_pBkOverBrush  = std::make_unique< CBrush>(RGB(233,229,217));
	}
	else
	{
		m_pBkBrush = std::make_unique <CBrush>(crBkColor);
		m_pBkOverBrush = std::make_unique <CBrush>(crBkOverColor);
	}

	// Create the Down highlight pen
	m_pBorderPen = std::make_unique <CPen>(PS_SOLID, 1, RGB(0,0,0));

	// Save the Button Font
	m_pTextFont.reset(pTextFont);

	if(!m_pTextFont)
	{
		m_bDeleteFont = true;
		m_pTextFont = std::make_shared<CFont>();
 		m_pTextFont->CreateFont(15, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_CHARACTER_PRECIS, 
								CLIP_LH_ANGLES, DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "Arial");    
	}

	// Save the text color
   	m_crTxtColor = crTxtColor;
	if(m_crTxtColor == -1)
		m_crTxtColor = RGB(0,0,0);
    
	// Save the text over color
	m_crTxtOverColor = crTxtOverColor;
	if(m_crTxtOverColor == -1)
		m_crTxtOverColor = RGB(0,0,0);
	
	// Triangle Brush
	m_pTriBrush = std::make_unique <CBrush>(m_crTxtColor);
	m_pTriPen = std::make_unique <CPen>(PS_SOLID, 1, m_crTxtColor);

    	GetClientRect(m_WindowRect);
	m_nListWidth = m_WindowRect.Width();

	// Create the List box
	m_pDropWnd = std::make_unique < CListWnd >();
	if (!m_pDropWnd->Create(this, uID + 1))
		return FALSE;

	// Set the Font to the list box
	m_pDropWnd->m_pDropList->SetFont(m_pTextFont.get());
	m_pDropWnd->m_pDropList->SetBkColor(crBkListColor);
	m_pParent = pParent;
	m_uID = uID;
	return TRUE;
}

void CDropButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	CDC*	pDC	= CDC::FromHandle(lpDrawItemStruct->hDC);
	const UINT	uState	= lpDrawItemStruct->itemState;
	CRect 	ButtonRect  = lpDrawItemStruct->rcItem;
    
	// Fill the Background
	CBrush* pOldBrush = nullptr;
	CPen*	pOldPen	= (CPen*)pDC->SelectObject(m_pBorderPen.get());
	if(m_bOver)	
		pOldBrush = (CBrush*)pDC->SelectObject(m_pBkOverBrush.get());
	else
		pOldBrush = (CBrush*)pDC->SelectObject(m_pBkBrush.get());
	pDC->Rectangle(ButtonRect);
		
	// Calculate the Total Width of Arrow and Text
	CFont* pOldFont = (CFont*)pDC->SelectObject(m_pTextFont.get());
	const CSize cTri  = pDC->GetTextExtent( "M", 1 );
	CString csText;
	GetWindowText(csText);
	const CSize cText =  pDC->GetTextExtent(csText, csText.GetLength());

	// Calculate the Y Position (Hope they didn't create a font or a button that is too large to fit! - perhaps fix later if they did)
	const int nYStart = (ButtonRect.Height() - cText.cy) / 2;
	
	// Set the Bk Mode and Transparenacy
	const int	nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	const COLORREF crOldColor = pDC->SetTextColor(m_crTxtColor);

	// Draw the Arrow
	const CPoint ptTri(ButtonRect.right - (cTri.cx + 5), nYStart + (cTri.cy / 3));
	DrawTriangle(pDC, ptTri, cTri.cx);

	// If its too long then shorten it and add ellipse's
	pDC->DrawText(csText, csText.GetLength(), CRect(5, 0, m_WindowRect.right - (cTri.cx + 10), m_WindowRect.bottom), DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS|DT_SINGLELINE);

	// Return what we used
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(nOldBkMode);
	pDC->SetTextColor(crOldColor);
	pDC->SelectObject(pOldFont);
}

void CDropButton::DrawTriangle(CDC* pDC, CPoint ptTopLeft, int nWidth)
{
	POINT 	ptArray[3];
	
	ptArray[0] 	 = ptTopLeft;
	ptArray[1].x = ptTopLeft.x + nWidth - 1;
	ptArray[1].y = ptTopLeft.y;
	ptArray[2].x = ptTopLeft.x + nWidth / 2;
	ptArray[2].y = ptTopLeft.y + nWidth / 2;

	// Select the Brush and Draw the triangle
	pDC->SelectObject(m_pTriBrush.get());
	pDC->SelectObject(m_pTriPen.get());
	pDC->Polygon(ptArray, 3);
}

void CDropButton::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{

}

void CDropButton::OnSize(UINT nType, int cx, int cy) 
{
	CButton::OnSize(nType, cx, cy);
	GetClientRect(m_WindowRect);	
}

void CDropButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// If this window already captured
	if(!m_bOver && !m_pDropWnd->IsWindowVisible())
		SetCapture();
	
	// Convert this window to screen coordinates
	CRect NewRect(m_WindowRect);
	ClientToScreen(NewRect);

	// Convert Point to coordinate
	ClientToScreen(&point);

	// Is the mouse in this window
	if(NewRect.PtInRect(point))
	{
		if(!m_bOver && !m_pDropWnd->IsWindowVisible())
		{
			m_bOver = true;	
			RedrawWindow();
		}
	}
	else
	{
		// Is this in the Drop down?
		if(m_pDropWnd->IsWindowVisible())
		{
			m_pDropWnd->GetWindowRect(NewRect);
		}
		else
		{
			m_bOver = false;
			RedrawWindow();
			ReleaseCapture();
		}
	}

	CButton::OnMouseMove(nFlags, point);
}

void CDropButton::DisplayList()
{
	// Convert to this button coordinates to desktop coordinates
	CRect ButtonRect(m_WindowRect);
	MapWindowPoints(GetDesktopWindow(), ButtonRect);
		
	const int nTemp = m_pDropWnd->m_pDropList->GetItemHeight(0);
	const int nCount = m_pDropWnd->m_pDropList->GetCount();
	
	// Should we recalculate the Acutal height
	int nHeight = m_nListHeight;
	if(m_pDropWnd->m_pDropList->GetCount())
		if(m_pDropWnd->m_pDropList->GetCount() * m_pDropWnd->m_pDropList->GetItemHeight(0) < m_nListHeight)
			nHeight = m_pDropWnd->m_pDropList->GetCount() * m_pDropWnd->m_pDropList->GetItemHeight(0);		
	
	// Move the outer List Window
	const CRect NewRect(ButtonRect.left, ButtonRect.bottom - 1, ButtonRect.left + m_nListWidth, ButtonRect.bottom + nHeight );
	m_ListRect = NewRect;
	m_pDropWnd->MoveWindow(m_ListRect, FALSE);

	// What is the size of the list box now? (its integral height - so it may not fill correctly)
	m_pDropWnd->m_pDropList->GetWindowRect(ButtonRect);
	if(ButtonRect.bottom != m_ListRect.bottom)
	{
		ButtonRect.bottom = m_ListRect.bottom; // Two for the border
		ButtonRect.InflateRect(1,1);
		m_pDropWnd->MoveWindow(ButtonRect);
	}

	// Show the Window
	m_pDropWnd->ShowWindow(SW_SHOWNORMAL);
	m_pDropWnd->m_bShow = true;
}

void CDropButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// Show or hide the list
	m_bOver = false;
	
//	TRACE("OnLButtonDown.%d\n", m_pDropWnd->m_bShow);

	if( m_pDropWnd->IsWindowVisible() || m_bClked)
	{
		// Is this outside the Current Rectangle
		MapWindowPoints(GetDesktopWindow(), &point, 2);
		if (!m_ListRect.PtInRect(point))
		{
			m_pDropWnd->ShowWindow(SW_HIDE);
			m_pDropWnd->m_bShow = false;
		}
		m_bClked = false;
	}
	else
	{
		m_bClked = true;
		DisplayList();
	}

	CButton::OnLButtonDown(nFlags, point);
}
void CDropButton::OnSetFocus(CWnd* pOldWnd) 
{	
	CButton::OnSetFocus(pOldWnd);
	if(m_pDropWnd->IsWindowVisible())
		m_pDropWnd->m_pDropList->SetFocus();
}

void CDropButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
}

LONG CDropButton::OnSelectionChanged(UINT wParam, LONG lParam)
{
	if(m_bChangeTitle)
	{
		// Set the New Title
		CString csText;
		m_pDropWnd->m_pDropList->GetText(wParam, csText);
		SetWindowText(csText);

		// repaint
		RedrawWindow();		
	}

	// Send a message to the parent that the selection changed
	GetParent()->PostMessage(CM_SELECTION_CHANGED, m_uID, wParam);
	m_pDropWnd->m_pDropList->SetPreviousSelection(wParam);
	return 1;
}

