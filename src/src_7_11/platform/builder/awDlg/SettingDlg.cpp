// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SettingDlg.h"
#include "../../h/axisvar.h"
#include "folderBrowser.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg dialog


CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/, CString path)
	: CDialog(CSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingDlg)
	m_nGap = 0;
	m_fontname = _T("±¼¸²");
	m_fontpnt = _T("9");
	m_mapPath = _T("");
	m_dnRGB = 0;
	m_upRGB = 0;
	m_clrTabPos = 0;
	//}}AFX_DATA_INIT
	m_path = path;
	m_ips = _T("");
}


void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingDlg)
	DDX_Control(pDX, IDC_UPRGBSPIN, m_upRGBSpin);
	DDX_Control(pDX, IDC_DNRGBSPIN, m_dnRGBSpin);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_EDITGAP, m_nGap);
	DDV_MinMaxInt(pDX, m_nGap, 1, 20);
	DDX_Control(pDX, IDC_ADDR, m_addr);
	DDX_Text(pDX, IDC_FONTNAME, m_fontname);
	DDX_Text(pDX, IDC_FONTPNT, m_fontpnt);
	DDX_Text(pDX, IDC_MAPPATH, m_mapPath);
	DDX_Text(pDX, IDC_DNRGB, m_dnRGB);
	DDV_MinMaxInt(pDX, m_dnRGB, 0, 255);
	DDX_Text(pDX, IDC_UPRGB, m_upRGB);
	DDV_MinMaxInt(pDX, m_upRGB, 0, 255);
	DDX_Radio(pDX, IDC_CLRTAB, m_clrTabPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingDlg)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
	ON_BN_CLICKED(IDC_SETFONTBTN, OnSetfontbtn)
	ON_BN_CLICKED(IDC_BROWSER2, OnBrowser2)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingDlg message handlers

void CSettingDlg::OnOK() 
{
	CString sTmp;
	UpdateData();

	WritePrivateProfileString(ENVIRONMENT, "FONTNAME", m_fontname, m_currentPath + "\\workshop.ini");
	WritePrivateProfileString(ENVIRONMENT, "FONTPNT", m_fontpnt, m_currentPath + "\\workshop.ini");
	WritePrivateProfileString(ENVIRONMENT, "INITMAP", m_mapPath, m_currentPath + "\\workshop.ini");

	sTmp.Format("%ld", MAKEWPARAM(m_upRGB, m_dnRGB));
	WritePrivateProfileString(ENVIRONMENT, "CORGB", sTmp, m_currentPath + "\\workshop.ini");

	sTmp.Format("%d", m_clrTabPos);
	WritePrivateProfileString(ENVIRONMENT, "CLRTABPOS", sTmp, m_currentPath + "\\workshop.ini");

	if (CFolderBrowser::MakeSurePathExists(m_path) == 0)
		CDialog::OnOK();
}

BOOL CSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nGap = GetPrivateProfileInt(ENVIRONMENT, "gap", 5, m_currentPath + "\\workshop.ini");

	for (int ii = 0; ii < 4; ii++)
		m_addr.SetFieldRange(ii, 0, 255);

	char twb[512];
	GetPrivateProfileString(ENVIRONMENT, SHOPIPS, "", twb, 512, m_currentPath + "\\workshop.ini");
	m_ips = twb;
	GetPrivateProfileString(ENVIRONMENT, "FONTNAME", "±¼¸²", twb, sizeof(twb), m_currentPath + "\\workshop.ini");
	m_fontname = twb;
	GetPrivateProfileString(ENVIRONMENT, "FONTPNT", "9", twb, sizeof(twb), m_currentPath + "\\workshop.ini");
	m_fontpnt = twb;

	GetPrivateProfileString(ENVIRONMENT, "INITMAP", "", twb, sizeof(twb), m_currentPath + "\\workshop.ini");
	m_mapPath = twb;

	long nCoRGB = GetPrivateProfileInt(ENVIRONMENT, "CORGB", 14024913, m_currentPath + "\\workshop.ini");
	m_upRGB = LOWORD(nCoRGB);
	m_dnRGB = HIWORD(nCoRGB);
	m_upRGBSpin.SetRange(0, 255);
	m_dnRGBSpin.SetRange(0, 255);

	m_clrTabPos = GetPrivateProfileInt(ENVIRONMENT, "CLRTABPOS", 0, m_currentPath + "\\workshop.ini");

	if (m_ips.IsEmpty())
	{
		m_addr.SetFieldFocus(0);
		return TRUE;
	}

	BYTE	field0, field1, field2, field3;
	getAddress(field0, field1, field2, field3);
	m_addr.SetAddress(field0, field1, field2, field3);
	m_addr.EnableWindow(FALSE);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDlg::OnBrowser()
{
	UpdateData(TRUE);
	CFolderBrowser folder(m_path);
	if (folder.DoModal() == IDOK)
	{
		m_path = folder.GetPathName();
		UpdateData(FALSE);
	}
}

void CSettingDlg::getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3)
{
	int	offs;
	CString	ips = m_ips;

	for (int ii = 0; ii < 4; ii++)
	{
		offs = ips.Find('.');
		if (offs != -1)
		{
			switch (ii)
			{
			case 0:
				field0 = (BYTE)atoi(ips.Left(offs));	break;
			case 1:
				field1 = (BYTE)atoi(ips.Left(offs));	break;
			case 2:
				field2 = (BYTE)atoi(ips.Left(offs));	break;
			case 3:
				field3 = (BYTE)atoi(ips.Left(offs));	break;
			}
			ips = (ips.GetLength() > offs+1) ? ips.Mid(offs+1) : _T("");
		}
		else
		{
			switch (ii)
			{
			case 0:
				field0 = (BYTE)atoi(ips);	break;
			case 1:
				field1 = (BYTE)atoi(ips);	break;
			case 2:
				field2 = (BYTE)atoi(ips);	break;
			case 3:
				field3 = (BYTE)atoi(ips);	break;
			}
			ips = _T("");
		}
	}
}

void CSettingDlg::OnSetfontbtn() 
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

void CSettingDlg::OnBrowser2() 
{
	UpdateData(TRUE);
	CFileDialog	fileD(TRUE, NULL, NULL,
		OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
		"All Files (*.*)|*.*||");
	if (fileD.DoModal() != IDOK)
		return;
	m_mapPath = fileD.GetPathName();
	UpdateData(FALSE);
}

HBRUSH CSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	//UpdateData();
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

void CSettingDlg::OnDestroy() 
{
	m_ctlBrushDn.DeleteObject();
	m_ctlBrushUp.DeleteObject();
	CDialog::OnDestroy();	
}
