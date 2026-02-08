// ImageComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ImageComboBox.h"
#include "iclncom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageComboBox
#define LINE_ITEM_HEIGHT   13

CImageComboBox::CImageComboBox()
{
	m_ToolTip.m_hWnd = NULL;
}

CImageComboBox::~CImageComboBox()
{
}


BEGIN_MESSAGE_MAP(CImageComboBox, CComboBox)
	//{{AFX_MSG_MAP(CImageComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageComboBox message handlers

void CImageComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	

	if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
	{
		if ((long int)lpDrawItemStruct->itemData == -1) return;
		CString str((LPCTSTR)lpDrawItemStruct->itemData); 
		int Index = (int)atof(str);

		if (Index == 0 && str[0] != '0') return;

		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap* pOld = memDC.SelectObject(&m_bitmap);

		CRect rect;
		rect.CopyRect(&lpDrawItemStruct->rcItem);
//		int top = LINE_ITEM_HEIGHT;
		pDC->BitBlt(rect.left, rect.top, rect.Width(), LINE_ITEM_HEIGHT,
			&memDC, 0, LINE_ITEM_HEIGHT*Index, SRCCOPY);
		memDC.SelectObject(pOld);
	}

	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		CBrush br(RGB(192,0,192));
		pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
	}

	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		CBrush br(RGB(255,255,255));
		pDC->FrameRect(&lpDrawItemStruct->rcItem, &br);
	}
}


void CImageComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = LINE_ITEM_HEIGHT;
}

void CImageComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_strToolTipText.IsEmpty())
		return;

	if(m_ToolTip.m_hWnd == NULL)
		SetToolTipText(m_strToolTipText, true);

	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = WM_MOUSEMOVE;
	msg.wParam = 0;
	msg.lParam = 0;
	m_ToolTip.RelayEvent(&msg);

	CComboBox::OnMouseMove(nFlags, point);
}

void CImageComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Invalidate();	
	CComboBox::OnLButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// public =====================================================================
void CImageComboBox::SetToolTipText(const CString& strText)
{
	m_strToolTipText = strText;
}

// CString 에서 tooltip
void CImageComboBox::SetToolTipText(const CString& strText, const bool bActivate)
{
	if(strText.IsEmpty())
		return;

	InitToolTip();
	AddToolInTooltip(strText, bActivate);
}

// ----------------------------------------------------------------------------
CBitmap& CImageComboBox::GetBitmap()
{
	return m_bitmap;
}

// private ====================================================================
void CImageComboBox::InitToolTip()
{
	if(m_ToolTip.m_hWnd != NULL)
		return;

	m_ToolTip.Create(this);
	m_ToolTip.Activate(FALSE);
}

void CImageComboBox::AddToolInTooltip(const CString& strText, const bool bActivate)
{
	// tooltip의 text가 없을 경우에 추가 한다.
	if(m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)strText, rectBtn, 1);
	}

	// ooltip text 를 set
	m_ToolTip.UpdateTipText((LPCTSTR)strText, this, 1);
	m_ToolTip.Activate(bActivate);
}


void CImageComboBox::OnSetFocus(CWnd* pOldWnd) 
{
//	CComboBox::OnSetFocus(pOldWnd);
	
	CWnd *pParent = GetParent();			// Getting ToolBar DLG
	if( pParent)
	{
		pParent = pParent->GetParent();
		if( pParent) pParent->SetFocus();	// Getting Chart OCX
	}
	
}
