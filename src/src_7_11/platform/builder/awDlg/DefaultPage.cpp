// DefaultPage.cpp : implementation file
//

#include "stdafx.h"
#include "DefaultPage.h"

#include "../mainvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDefaultPage property page

IMPLEMENT_DYNCREATE(CDefaultPage, CPropertyPage)

CDefaultPage::CDefaultPage() : CPropertyPage(CDefaultPage::IDD)
{
	//{{AFX_DATA_INIT(CDefaultPage)
	m_dnRGB = 0;
	m_upRGB = 0;
	m_clrTabPos = -1;
	m_fontname = _T("");
	m_fontpnt = _T("");
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;

	CString currentPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);

	char twb[512];
	GetPrivateProfileString(ENVIRONMENT, "FONTNAME", "±¼¸²", twb, sizeof(twb), currentPath + "\\workshop.ini");
	m_fontname = twb;
	GetPrivateProfileString(ENVIRONMENT, "FONTPNT", "9", twb, sizeof(twb), currentPath + "\\workshop.ini");
	m_fontpnt = twb;
	
	long nCoRGB = GetPrivateProfileInt(ENVIRONMENT, "CORGB", 14024913, currentPath + "\\workshop.ini");
	m_upRGB = LOWORD(nCoRGB);
	m_dnRGB = HIWORD(nCoRGB);

	m_clrTabPos = GetPrivateProfileInt(ENVIRONMENT, "CLRTABPOS", 0, currentPath + "\\workshop.ini");
}

CDefaultPage::~CDefaultPage()
{
	m_ctlBrushDn.DeleteObject();
	m_ctlBrushUp.DeleteObject();
}

void CDefaultPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDefaultPage)
	DDX_Control(pDX, IDC_DNRGBSPIN, m_dnRGBSpin);
	DDX_Control(pDX, IDC_UPRGBSPIN, m_upRGBSpin);
	DDX_Text(pDX, IDC_DNRGB, m_dnRGB);
	DDV_MinMaxInt(pDX, m_dnRGB, 0, 255);
	DDX_Text(pDX, IDC_UPRGB, m_upRGB);
	DDV_MinMaxInt(pDX, m_upRGB, 0, 255);
	DDX_Radio(pDX, IDC_CLRTAB, m_clrTabPos);
	DDX_Text(pDX, IDC_FONTNAME, m_fontname);
	DDX_Text(pDX, IDC_FONTPNT, m_fontpnt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDefaultPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDefaultPage)
	ON_BN_CLICKED(IDC_SETFONTBTN, OnSetfontbtn)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDefaultPage message handlers

BOOL CDefaultPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_upRGBSpin.SetRange(0, 255);
	m_dnRGBSpin.SetRange(0, 255);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDefaultPage::OnSetfontbtn() 
{
	CFontDialog	fontD;

	fontD.m_cf.lStructSize = sizeof(CHOOSEFONT);
	ZeroMemory(fontD.m_cf.lpLogFont, sizeof(LOGFONT));
	strcpy_s(fontD.m_cf.lpLogFont->lfFaceName, m_fontname);
	fontD.m_cf.lpLogFont->lfItalic = FALSE;
	fontD.m_cf.lpLogFont->lfWeight = FW_NORMAL;
	
	CDC *pDC = GetDC();
	fontD.m_cf.lpLogFont->lfHeight = MulDiv(-(int)atoi(m_fontpnt), pDC->GetDeviceCaps(LOGPIXELSY), 72);
	ReleaseDC(pDC);

	fontD.m_cf.lpLogFont->lfCharSet = DEFAULT_CHARSET;
	fontD.m_cf.hDC = 0;
	fontD.m_cf.Flags  = CF_ENABLEHOOK | CF_FORCEFONTEXIST | CF_SCREENFONTS;
	fontD.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT|CF_NOVERTFONTS;
	fontD.m_cf.nFontType  = SCREEN_FONTTYPE;
	if (fontD.DoModal() == IDOK)
	{
		m_fontname = fontD.GetFaceName();
		m_fontpnt.Format("%d", fontD.GetSize()/10);
		UpdateData(FALSE);
	}	
}

HBRUSH CDefaultPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		UpdateData();
		if (GetDlgItem(IDC_UPRGB)->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{		
			m_ctlClrUp = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, max(0, min(m_upRGB,   255)));
			m_ctlBrushUp.DeleteObject();
			m_ctlBrushUp.CreateSolidBrush(m_ctlClrUp);
			pDC->SetBkColor(m_ctlClrUp);
			if ((GetRValue(m_ctlClrUp) + GetGValue(m_ctlClrUp) + GetBValue(m_ctlClrUp)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
			hbr = (HBRUSH) m_ctlBrushUp.operator HBRUSH();
		}
		else if (GetDlgItem(IDC_DNRGB)->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{
			m_ctlClrDn = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, max(0, min(m_dnRGB, 255)));
			m_ctlBrushDn.DeleteObject();
			m_ctlBrushDn.CreateSolidBrush(m_ctlClrDn);
			pDC->SetBkColor(m_ctlClrDn);
			if ((GetRValue(m_ctlClrDn) + GetGValue(m_ctlClrDn) + GetBValue(m_ctlClrDn)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
			hbr = (HBRUSH) m_ctlBrushDn.operator HBRUSH();
		}
		break;
	}
	return hbr;
}
