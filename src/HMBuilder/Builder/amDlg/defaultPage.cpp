// defaultPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "defaultPage.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../h/axisvar.h"

// CDefaultPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDefaultPage, CPropertyPage)

CDefaultPage::CDefaultPage()
	: CPropertyPage(CDefaultPage::IDD)
	, m_FormWidth(_T(""))
	, m_FormHeight(_T(""))
{
	char	wb[512];
	CString	path;
	long	value;

	m_psp.dwFlags &= ~PSP_HASHELP;

	path = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	path += _T("\\workshop.ini");

	GetPrivateProfileString(ENVIRONMENT, _T("FONTNAME"), _T("굴림"), wb, sizeof(wb), path);
	m_fontName = wb;
	GetPrivateProfileString(ENVIRONMENT, _T("FONTPNT"), _T("9"), wb, sizeof(wb), path);
	m_fontPoint = wb;

	GetPrivateProfileString(ENVIRONMENT, _T("Form_Width"), _T("380"), wb, sizeof(wb), path);
	m_FormWidth = wb;
	GetPrivateProfileString(ENVIRONMENT, _T("Form_Height"), _T("450"), wb, sizeof(wb), path);
	m_FormHeight = wb;
	
	value = GetPrivateProfileInt(ENVIRONMENT, _T("CORGB"), 14024913, path);
	m_upRGB = LOWORD(value);
	m_dnRGB = HIWORD(value);

	m_clrTabPos = GetPrivateProfileInt(ENVIRONMENT, _T("CLRTABPOS"), 0, path);
}

CDefaultPage::~CDefaultPage()
{
	m_ctlBrushDn.DeleteObject();
	m_ctlBrushUp.DeleteObject();
}

void CDefaultPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_CLRTAB, m_clrTabPos);
	DDX_Text(pDX, IDC_DNRGB, m_dnRGB);
	DDV_MinMaxInt(pDX, m_dnRGB, 0, 255);
	DDX_Control(pDX, IDC_DNRGBSPIN, m_dnSpin);
	DDX_Text(pDX, IDC_UPRGB, m_upRGB);
	DDV_MinMaxInt(pDX, m_upRGB, 0, 255);
	DDX_Control(pDX, IDC_UPRGBSPIN, m_upSpin);
	DDX_Text(pDX, IDC_FONTNAME, m_fontName);
	DDX_Text(pDX, IDC_FONTPNT, m_fontPoint);
	DDX_Text(pDX, IDC_FORMW, m_FormWidth);
	DDX_Text(pDX, IDC_FORMH, m_FormHeight);
}


BEGIN_MESSAGE_MAP(CDefaultPage, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SETFONTBTN, &CDefaultPage::OnFont)
END_MESSAGE_MAP()


// CDefaultPage 메시지 처리기입니다.


BOOL CDefaultPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_upSpin.SetRange(0, 255);
	m_dnSpin.SetRange(0, 255);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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
			m_ctlClrUp = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, max(0, min(m_upRGB,   255)));
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
			m_ctlClrDn = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, max(0, min(m_dnRGB, 255)));
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


void CDefaultPage::OnFont()
{
	CFontDialog	dlg;

	dlg.m_cf.lStructSize = sizeof(CHOOSEFONT);
	ZeroMemory(dlg.m_cf.lpLogFont, sizeof(LOGFONT));
	strcpy_s(dlg.m_cf.lpLogFont->lfFaceName, LF_FACESIZE, m_fontName);
	dlg.m_cf.lpLogFont->lfItalic = FALSE;
	dlg.m_cf.lpLogFont->lfWeight = FW_NORMAL;
	
	CDC *pDC = GetDC();
	dlg.m_cf.lpLogFont->lfHeight = MulDiv(-(int)atoi(m_fontPoint), pDC->GetDeviceCaps(LOGPIXELSY), 72);
	ReleaseDC(pDC);

	dlg.m_cf.lpLogFont->lfCharSet = DEFAULT_CHARSET;
	dlg.m_cf.hDC = 0;
	dlg.m_cf.Flags  = CF_ENABLEHOOK | CF_FORCEFONTEXIST | CF_SCREENFONTS;
	dlg.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT|CF_NOVERTFONTS;
	dlg.m_cf.nFontType  = SCREEN_FONTTYPE;
	if (dlg.DoModal() == IDOK)
	{
		m_fontName = dlg.GetFaceName();
		m_fontPoint.Format(_T("%d"), dlg.GetSize()/10);
		UpdateData(FALSE);
	}
}
