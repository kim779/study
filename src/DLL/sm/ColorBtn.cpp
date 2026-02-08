// ColorBtn.cpp : implementation file
//

#include "stdafx.h"
#include "ColorBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	IDC_BTN_COLOR	13000
/////////////////////////////////////////////////////////////////////////////
// CColorBtn

CColorBtn::CColorBtn()
{
	//m_btn = NULL;
	m_bDef = m_bColor = RGB(255,255,166);
}

CColorBtn::~CColorBtn()
{
	//if (m_btn) delete m_btn;
}


BEGIN_MESSAGE_MAP(CColorBtn, CStatic)
	//{{AFX_MSG_MAP(CColorBtn)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_COLOR, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBtn message handlers

void CColorBtn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	rc, btnRc, colorRc;
	GetClientRect(&rc);
	
	btnRc = colorRc = rc;

	colorRc.right = rc.right - 20;
	btnRc.left = rc.right - 20;

	//if (!m_btn)
	{
		//m_btn = new CButton();
		m_btn = std::make_unique<CButton>();
		m_btn->Create("...", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, btnRc, this, IDC_BTN_COLOR);
	}

	dc.FillSolidRect(colorRc, m_bColor);
}

void CColorBtn::OnClicked() 
{
	CColorDialog clrDlg(m_bColor, CC_FULLOPEN);
	COLORREF* clr = clrDlg.GetSavedCustomColors();
	clr[0] =  m_bDef;
	
	if (clrDlg.DoModal() == IDOK)
	{
		m_bColor = clrDlg.GetColor();
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), 0), 0);
		Invalidate();
	}
}

COLORREF CColorBtn::GetColor()
{
	return	m_bColor;
}

void CColorBtn::SetColor(COLORREF Color)
{
	m_bColor = Color;
	Invalidate();
}
