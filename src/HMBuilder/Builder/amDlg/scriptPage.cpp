// scriptPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "scriptPage.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"

#define	CBI_BKGND	0
#define	CBI_NORMALTEXT	1
#define	CBI_SYMBOL	2
#define	CBI_COMMENT	3
#define	CBI_CONSTANT	4
#define	CBI_KEYWORD	5
#define	CBI_STRING	6

// CScriptPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CScriptPage, CPropertyPage)

CScriptPage::CScriptPage()
	: CPropertyPage(CScriptPage::IDD)
{
	CString	path;

	m_psp.dwFlags &= ~PSP_HASHELP;

	path = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	path += _T("\\workshop.ini");

	m_clBkGnd    = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRBKGND"), ::GetSysColor(COLOR_WINDOW), path);
	m_clText     = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRNORMAL"), ::GetSysColor(COLOR_WINDOWTEXT), path);
	m_clSymbol   = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRSYMBOL"), RGB(0, 150, 180), path);
	m_clComment  = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRCOMMENT"), RGB(0, 128, 0), path);
	m_clConstant = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRCONSTANT"), RGB(0x80, 0x00, 0x60), path);
	m_clKeyword  = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRKEYWORD"), RGB(0, 0, 255), path);
	m_clString   = GetPrivateProfileInt(_T("SCRIPT"), _T("CLRSTRING"), RGB(255, 0, 0), path);
}

CScriptPage::~CScriptPage()
{
	m_brushCLR.DeleteObject();
}

void CScriptPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLORLIST_CBO, m_itemList);
	DDX_Control(pDX, IDC_SETCOLOR_CBO, m_clrList);
}


BEGIN_MESSAGE_MAP(CScriptPage, CPropertyPage)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DEFAULT_BUTTON, &CScriptPage::OnDefault)
	ON_CBN_SELENDOK(IDC_COLORLIST_CBO, &CScriptPage::OnSelendokColorList)
	ON_CBN_SELENDOK(IDC_SETCOLOR_CBO, &CScriptPage::OnSelendokSetcolorCbo)
END_MESSAGE_MAP()


// CScriptPage 메시지 처리기입니다.


BOOL CScriptPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_refColor = &m_clBkGnd;
	m_itemList.SetCurSel(0);
	m_clrList.SetColor(m_clBkGnd);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CScriptPage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect	rc, textRc;
	CDC*	pDC = GetDC();
	
	GetClientRect(&rc);
	rc.bottom = 150;
	rc.DeflateRect(6, 6, 6, 6);

	if (rc.PtInRect(point))
	{	
		CSize	szText;
		BOOL	bSet = FALSE;

		rc.DeflateRect(2, 2, 2, 2);
		szText = pDC->GetTextExtent(_T("// Comment Descriptions"));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_COMMENT);
			bSet = TRUE;
		}
		
		rc.DeflateRect(0, 20, 0, 0);

		szText = pDC->GetTextExtent(_T("SCREEN"));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_SYMBOL);
			bSet = TRUE;
		}
				
		rc.DeflateRect(pDC->GetTextExtent(_T("SCREEN")).cx, 0, 0, 0);
		szText = pDC->GetTextExtent(_T("."));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_NORMALTEXT);
			bSet = TRUE;
		}
		
		rc.DeflateRect(pDC->GetTextExtent(_T(".")).cx, 0, 0, 0);
		szText = pDC->GetTextExtent(_T("ChangeMap"));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_KEYWORD);
			bSet = TRUE;
		}
		
		rc.DeflateRect(pDC->GetTextExtent(_T("ChangeMap")).cx, 0, 0, 0);
		szText = pDC->GetTextExtent(_T(" \"MapName\""));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_STRING);
			bSet = TRUE;
		}
		
		rc.DeflateRect(0, 20, 0, 0);
		rc.left = 5;
		
		szText = pDC->GetTextExtent(_T("myVariable = "));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_NORMALTEXT);
			bSet = TRUE;
		}
		
		rc.DeflateRect(pDC->GetTextExtent(_T("myVariable = ")).cx, 0, 0, 0);
		szText = pDC->GetTextExtent(_T("True "));
		textRc = CRect(rc.left, rc.top, szText.cx + rc.left, szText.cy + rc.top);
		if (!bSet && textRc.PtInRect(point))
		{
			m_itemList.SetCurSel(CBI_CONSTANT);
			bSet = TRUE;
		}

		if (!bSet)
			m_itemList.SetCurSel(CBI_BKGND);

		OnSelendokColorList();
	}
	CPropertyPage::OnLButtonDown(nFlags, point);
}


void CScriptPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;

	GetClientRect(&rc);
	rc.bottom = 150;
	rc.DeflateRect(5, 5, 5, 5);
	dc.Draw3dRect(rc, RGB(0, 0, 0), RGB(0, 0, 0)); 

	rc.DeflateRect(1, 1, 1, 1);
	dc.FillSolidRect(rc, m_clBkGnd);

	rc.DeflateRect(2, 2, 2, 2);
	dc.SetBkMode(TRANSPARENT);

	dc.SetTextColor(m_clComment);
	dc.DrawText(_T("- - Comment Descriptions"), rc, DT_LEFT|DT_SINGLELINE);

	rc.DeflateRect(0, 20, 0, 0);

	dc.SetTextColor(m_clSymbol);
	dc.DrawText(_T("SCREEN"), rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clText);
	rc.DeflateRect(dc.GetTextExtent(_T("SCREEN")).cx, 0, 0, 0);
	dc.DrawText(_T("."), rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clKeyword);
	rc.DeflateRect(dc.GetTextExtent(_T(".")).cx, 0, 0, 0);
	dc.DrawText(_T("ChangeMap"), rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clString);
	rc.DeflateRect(dc.GetTextExtent(_T("ChangeMap")).cx, 0, 0, 0);
	dc.DrawText(_T(" \"MapName\""), rc, DT_LEFT|DT_SINGLELINE);

	rc.DeflateRect(0, 20, 0, 0);
	rc.left = 5;

	dc.SetTextColor(m_clText);
	dc.DrawText(_T("myVariable = "), rc, DT_LEFT|DT_SINGLELINE);

	dc.SetTextColor(m_clConstant);
	rc.DeflateRect(dc.GetTextExtent(_T("myVariable = ")).cx, 0, 0, 0);
	dc.DrawText(_T("True"), rc, DT_LEFT|DT_SINGLELINE);
}


void CScriptPage::OnDefault()
{
	m_clBkGnd    = ::GetSysColor(COLOR_WINDOW);
	m_clText     = ::GetSysColor(COLOR_WINDOWTEXT);
	m_clSymbol   = RGB(0, 150, 180);
	m_clComment  = RGB(0, 128, 0);
	m_clConstant = RGB(0x80, 0x00, 0x60);
	m_clKeyword  = RGB(0, 0, 255);
	m_clString   = RGB(255, 0, 0);

	invalidateSample();
}


void CScriptPage::OnSelendokColorList()
{
	int	nIdx = m_itemList.GetCurSel();

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
		m_clrList.SetColor(*m_refColor);
	}
}

void CScriptPage::OnSelendokSetcolorCbo()
{
	*m_refColor = m_clrList.GetColor();
	invalidateSample();
}

void CScriptPage::invalidateSample()
{
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = 150;
	InvalidateRect(rc);
}
