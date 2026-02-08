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
	m_bColor = RGB(192,192,192);
}

CColorBtn::~CColorBtn()
{
}


BEGIN_MESSAGE_MAP(CColorBtn, CStatic)
	//{{AFX_MSG_MAP(CColorBtn)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_COLOR, OnClicked)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBtn message handlers

void CColorBtn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc, colorRc;
	GetClientRect(&rc);
	
	colorRc = rc;

	colorRc.right = rc.right - 20;
	dc.FillSolidRect(colorRc, m_bColor);
}

void CColorBtn::OnClicked() 
{
	CColorDialog clrDlg(m_bColor, CC_FULLOPEN);

	if (clrDlg.DoModal() == IDOK)
	{
		m_bColor = clrDlg.GetColor();
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


int CColorBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CRect btnRc;
	GetClientRect(&btnRc);	
	btnRc.left = btnRc.right - 20;

	m_btn = std::make_unique<CButton>();
	m_btn->Create("...", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, btnRc, this, IDC_BTN_COLOR);
	return 0;
}
