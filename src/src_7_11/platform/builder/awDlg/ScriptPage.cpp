// ScriptPage.cpp : implementation file
//

#include "stdafx.h"
#include "ScriptPage.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CBI_BKGND	0
#define	CBI_NORMALTEXT	1
#define	CBI_SYMBOL	2
#define	CBI_COMMENT	3
#define	CBI_CONSTANT	4
#define	CBI_KEYWORD	5
#define	CBI_STRING	6

/////////////////////////////////////////////////////////////////////////////
// CScriptPage property page

IMPLEMENT_DYNCREATE(CScriptPage, CPropertyPage)

CScriptPage::CScriptPage() : CPropertyPage(CScriptPage::IDD)
{
	//{{AFX_DATA_INIT(CScriptPage)
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;

	CString currentPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	m_clBkGnd    = GetPrivateProfileInt("SCRIPT", "CLRBKGND", ::GetSysColor(COLOR_WINDOW), currentPath + "\\workshop.ini");
	m_clText     = GetPrivateProfileInt("SCRIPT", "CLRNORMAL", ::GetSysColor(COLOR_WINDOWTEXT), currentPath + "\\workshop.ini");
	m_clSymbol   = GetPrivateProfileInt("SCRIPT", "CLRSYMBOL", RGB(0, 150, 180), currentPath + "\\workshop.ini");
	m_clComment  = GetPrivateProfileInt("SCRIPT", "CLRCOMMENT", RGB(0, 128, 0), currentPath + "\\workshop.ini");
	m_clConstant = GetPrivateProfileInt("SCRIPT", "CLRCONSTANT", RGB(0x80, 0x00, 0x60), currentPath + "\\workshop.ini");
	m_clKeyword  = GetPrivateProfileInt("SCRIPT", "CLRKEYWORD", RGB(0, 0, 255), currentPath + "\\workshop.ini");
	m_clString   = GetPrivateProfileInt("SCRIPT", "CLRSTRING", RGB(255, 0, 0), currentPath + "\\workshop.ini");
}

CScriptPage::~CScriptPage()
{
	m_brushCLR.DeleteObject();
}

void CScriptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptPage)
	DDX_Control(pDX, IDC_COLORLIST_CBO, m_colorItemCBO);
	DDX_Control(pDX, IDC_SETCOLOR_CBO, m_colorCBO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptPage, CPropertyPage)
	//{{AFX_MSG_MAP(CScriptPage)
	ON_WM_PAINT()
	ON_CBN_SELENDOK(IDC_COLORLIST_CBO, OnSelendokColorlistCbo)
	ON_CBN_SELENDOK(IDC_SETCOLOR_CBO, OnSelendokSetcolorCbo)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, OnDefaultButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptPage message handlers

BOOL CScriptPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_refColor = &m_clBkGnd;

	m_colorCBO.SetColor(m_clBkGnd);
	m_colorItemCBO.SetCurSel(0);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScriptPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = 150;
	rc.DeflateRect(5, 5, 5, 5);
	dc.Draw3dRect(rc, RGB(0, 0, 0), RGB(0, 0, 0)); 

	rc.DeflateRect(1, 1, 1, 1);
	dc.FillSolidRect(rc, m_clBkGnd);

	rc.DeflateRect(2, 2, 2, 2);
	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(m_clComment);
	dc.DrawText("// Comment Descriptions", rc, DT_LEFT|DT_SINGLELINE);

	rc.DeflateRect(0, 20, 0, 0);

	dc.SetTextColor(m_clSymbol);
	dc.DrawText("SCREEN", rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clText);
	rc.DeflateRect(dc.GetTextExtent("SCREEN").cx, 0, 0, 0);
	dc.DrawText(".", rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clKeyword);
	rc.DeflateRect(dc.GetTextExtent(".").cx, 0, 0, 0);
	dc.DrawText("ChangeMap", rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clString);
	rc.DeflateRect(dc.GetTextExtent("ChangeMap").cx, 0, 0, 0);
	dc.DrawText(" \"MapName\"", rc, DT_LEFT|DT_SINGLELINE);

	rc.DeflateRect(0, 20, 0, 0);
	rc.left = 5;

	dc.SetTextColor(m_clText);
	dc.DrawText("myVariable = ", rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clConstant);
	rc.DeflateRect(dc.GetTextExtent("myVariable = ").cx, 0, 0, 0);
	dc.DrawText("True", rc, DT_LEFT|DT_SINGLELINE);
}

void CScriptPage::OnSelendokColorlistCbo() 
{
	int nIdx = m_colorItemCBO.GetCurSel();
	if (nIdx != CB_ERR)
	{
		switch (nIdx)
		{
		case CBI_BKGND:
			m_refColor = &m_clBkGnd;	break;
		case CBI_NORMALTEXT:
			m_refColor = &m_clText;		break;
		case CBI_SYMBOL:
			m_refColor = &m_clSymbol;	break;
		case CBI_COMMENT:
			m_refColor = &m_clComment;	break;
		case CBI_CONSTANT:
			m_refColor = &m_clConstant;	break;
		case CBI_KEYWORD:
			m_refColor = &m_clKeyword;	break;
		case CBI_STRING:
			m_refColor = &m_clString;	break;
		}
		m_colorCBO.SetColor(*m_refColor);
	}
}

void CScriptPage::OnSelendokSetcolorCbo() 
{
	*m_refColor = m_colorCBO.GetColor();
	InvalidateSample();
}

void CScriptPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDC *pDC = GetDC();
	CRect rc, textRc;
	
	GetClientRect(&rc);
	rc.bottom = 150;
	rc.DeflateRect(6, 6, 6, 6);
	
	if (rc.PtInRect(point))
	{	
		CSize szText;
		BOOL bSet = FALSE;

		rc.DeflateRect(2, 2, 2, 2);
		szText = pDC->GetTextExtent("// Comment Descriptions");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_COMMENT);
			bSet = TRUE;
		}
		
		rc.DeflateRect(0, 20, 0, 0);

		szText = pDC->GetTextExtent("SCREEN");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_SYMBOL);
			bSet = TRUE;
		}
				
		rc.DeflateRect(pDC->GetTextExtent("SCREEN").cx, 0, 0, 0);
		szText = pDC->GetTextExtent(".");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_NORMALTEXT);
			bSet = TRUE;
		}
		
		rc.DeflateRect(pDC->GetTextExtent(".").cx, 0, 0, 0);
		szText = pDC->GetTextExtent("ChangeMap");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_KEYWORD);
			bSet = TRUE;
		}
		
		rc.DeflateRect(pDC->GetTextExtent("ChangeMap").cx, 0, 0, 0);
		szText = pDC->GetTextExtent(" \"MapName\"");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_STRING);
			bSet = TRUE;
		}
		
		rc.DeflateRect(0, 20, 0, 0);
		rc.left = 5;
		
		szText = pDC->GetTextExtent("myVariable = ");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_NORMALTEXT);
			bSet = TRUE;
		}
		
		rc.DeflateRect(pDC->GetTextExtent("myVariable = ").cx, 0, 0, 0);
		szText = pDC->GetTextExtent("True ");
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_colorItemCBO.SetCurSel(CBI_CONSTANT);
			bSet = TRUE;
		}

		if (!bSet)
			m_colorItemCBO.SetCurSel(CBI_BKGND);

		OnSelendokColorlistCbo();
	}
	CPropertyPage::OnLButtonDown(nFlags, point);
}

void CScriptPage::OnDefaultButton() 
{
	m_clBkGnd    = ::GetSysColor(COLOR_WINDOW);
	m_clText     = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clSymbol   = RGB(0, 150, 180);
	m_clComment  = RGB(0, 128, 0);
	m_clConstant = RGB(0x80, 0x00, 0x60);
	m_clKeyword  = RGB(0, 0, 255);
	m_clString   = RGB(255, 0, 0);
	InvalidateSample();
}

void CScriptPage::InvalidateSample()
{
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = 150;
	InvalidateRect(rc);
}
