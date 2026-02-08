// CheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "CheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckBox

CCheckBox::CCheckBox()
{
	m_bCreateBrush	= false;
	m_bCreateFont	= false;
	m_pfont		= NULL;
	m_pbrush	= NULL;
}

CCheckBox::~CCheckBox()
{
	if (m_pbrush && m_bCreateBrush)
	{
		m_pbrush->DeleteObject();
		delete m_pbrush;
		m_pbrush = NULL;
	}

	if (m_pfont && m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
		m_pfont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CCheckBox, CButton)
	//{{AFX_MSG_MAP(CCheckBox)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckBox message handlers
BOOL CCheckBox::Create(LPCTSTR lpszWindowName,
				DWORD dwStyle,
				const RECT& rect,
				CWnd* pParentWnd,
				UINT nID,
				CFont* pfont,
				CBrush* pbrush) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (dwStyle == NULL)
	{
		dwStyle =	WS_CHILDWINDOW
				| WS_CLIPSIBLINGS
				| WS_VISIBLE
				| BS_CHECKBOX
				| BS_AUTOCHECKBOX
				| BS_NOTIFY;
	}

	m_pfont		= pfont;
	m_pbrush	= pbrush;


	return CWnd::Create(_T("BUTTON"), lpszWindowName, dwStyle, rect, pParentWnd, nID);
}

int CCheckBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return Init();
	//return 0;
}

int CCheckBox::Init()
{
	if (m_pfont == NULL)
	{
		m_pfont = (CFont*) new CFont;
		m_pfont->CreatePointFont(90, "±¼¸²");
		m_bCreateFont	= TRUE;
	}

	CButton::SetFont(m_pfont);

	return 0;
}

HBRUSH CCheckBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	if (m_pbrush)
	{
		if (m_pbrush->GetSafeHandle())
		{
			CRect rc;
			GetClientRect(&rc);
			pDC->FillSolidRect(rc,RGB(100,200,225));

			//pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)m_pbrush->GetSafeHandle();
		}
	}
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

void CCheckBox::SetColorBG(COLORREF rgbBG, bool bRefresh)
{
	if (	m_pbrush 
		&& m_bCreateBrush)
	{
		m_pbrush->DeleteObject();
		delete m_pbrush;
	}

	m_pbrush = new CBrush;
	m_pbrush->CreateSolidBrush(rgbBG);
	m_bCreateBrush = true;

	if (bRefresh)
		Invalidate();
}

void CCheckBox::SetFont(int iPoint, CString strFontName, bool bRefresh)
{
	if (	m_pfont 
		&& m_bCreateFont)
	{
		m_pfont->DeleteObject();
		delete m_pfont;
	}

	m_pfont = new CFont;
	m_pfont->CreatePointFont(/*iPoint * */10, strFontName);
	m_bCreateFont = true;

	if (bRefresh)
		Invalidate();

}