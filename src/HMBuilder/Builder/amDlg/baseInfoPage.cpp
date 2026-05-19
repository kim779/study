// baseInfoPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "baseInfoPage.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../h/axisvar.h"

// CBaseInfoPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBaseInfoPage, CPropertyPage)

CBaseInfoPage::CBaseInfoPage()
	: CPropertyPage(CBaseInfoPage::IDD)
	, m_merge(_T(""))
{
	char	wb[512];
	CString	path;

	m_displayMode = 0;
	m_gap = 0;
	m_maps = _T("");
	m_path = _T("");

	m_psp.dwFlags &= ~PSP_HASHELP;

	CString currPath = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	path = currPath + _T("\\workshop.ini");

	GetPrivateProfileString(ENVIRONMENT, _T("Emul_Path"), _T(""), wb, sizeof(wb), path);
	m_path = wb;
	
	m_gap		= GetPrivateProfileInt(ENVIRONMENT, _T("gap"), 5, path);	
	GetPrivateProfileString(SERVER, _T("IP"), _T(""), wb, sizeof(wb), path);
	m_ips = wb;
	GetPrivateProfileString(ENVIRONMENT, _T("INITMAP"), _T(""), wb, sizeof(wb), path);
	m_maps = wb;
	m_displayMode = GetPrivateProfileInt(ENVIRONMENT, _T("DISPLAYMODE"), 0, path);

	m_EmulSize = GetPrivateProfileInt(ENVIRONMENT, _T("Emul_Resolution"), 100, path);
	
	GetPrivateProfileString(ENVIRONMENT, _T("MERGE"), _T(""), wb, sizeof(wb), path);
	m_merge = wb;	

}

CBaseInfoPage::~CBaseInfoPage()
{
}

void CBaseInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR, m_addr);
	DDX_Radio(pDX, IDC_DISPLAYDATA_RDO, m_displayMode);
	DDV_MinMaxInt(pDX, m_displayMode, 0, 3);
	DDX_Text(pDX, IDC_EDITGAP, m_gap);
	DDV_MinMaxInt(pDX, m_gap, 0, 100);
	DDX_Text(pDX, IDC_MAPPATH, m_maps);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Text(pDX, IDC_EMULSIZE, m_EmulSize);
	DDV_MinMaxInt(pDX, m_EmulSize, 1, 100);
	DDX_Text(pDX, IDC_MERGEPATH, m_merge);
}


BEGIN_MESSAGE_MAP(CBaseInfoPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSER, &CBaseInfoPage::OnBrowser)
	ON_BN_CLICKED(IDC_BROWSER2, &CBaseInfoPage::OnBrowser2)
	ON_BN_CLICKED(IDC_DISPLAYDATA_RDO, &CBaseInfoPage::OnDisplaydataMode)
	ON_BN_CLICKED(IDC_DISPLAYSYMBOL_RDO, &CBaseInfoPage::OnDisplaydataMode)
	ON_BN_CLICKED(IDC_DSPLAYNONE_RDO, &CBaseInfoPage::OnDisplaydataMode)
	ON_BN_CLICKED(IDC_MERGE, &CBaseInfoPage::OnBnClickedMerge)
END_MESSAGE_MAP()


// CBaseInfoPage 메시지 처리기입니다.


BOOL CBaseInfoPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	for (int ii = 0; ii < 4; ii++)
		m_addr.SetFieldRange(ii, 0, 255);

	if (m_ips.IsEmpty())
	{
		m_addr.SetFieldFocus(0);
		return TRUE;
	}

	BYTE	field0, field1, field2, field3;
	getAddress(field0, field1, field2, field3);
	m_addr.SetAddress(field0, field1, field2, field3);
	m_addr.EnableWindow(FALSE);

	if (strcmp(m_path.Right(1), "\\") == 0)
	{
		if (strcmp(m_path.Right(2), ":\\") != 0)
			m_path = m_path.Left(m_path.GetLength()-1);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBaseInfoPage::OnBrowser()
{
	UpdateData(TRUE);

	CFolderPickerDialog dlg(m_path, OFN_PATHMUSTEXIST);
	if (dlg.DoModal() == IDOK)
	{
		m_path = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CBaseInfoPage::OnBrowser2()
{
	/*
	UpdateData(TRUE);

	CFileDialog	dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_PATHMUSTEXIST, "All Files (*.*)|*.*||");
	if (dlg.DoModal() == IDOK)
	{
		m_maps = dlg.GetPathName();
		UpdateData(FALSE);
	}
	*/
	UpdateData(TRUE);

	CFolderPickerDialog dlg(m_path, OFN_PATHMUSTEXIST);
	if (dlg.DoModal() == IDOK)
	{
		m_maps = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CBaseInfoPage::OnDisplaydataMode()
{
	UpdateData(TRUE);
}

void CBaseInfoPage::getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3)
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

void CBaseInfoPage::OnBnClickedMerge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog	dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, "All Files (*.*)|*.*||");
	if (dlg.DoModal() == IDOK)
	{
		m_merge = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
