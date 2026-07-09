// BaseInfoPage.cpp : implementation file
//

#include "stdafx.h"
#include "BaseInfoPage.h"
#include "folderBrowser.h"

#include "../mainvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseInfoPage property page

IMPLEMENT_DYNCREATE(CBaseInfoPage, CPropertyPage)

CBaseInfoPage::CBaseInfoPage() : CPropertyPage(CBaseInfoPage::IDD)
{
	//{{AFX_DATA_INIT(CBaseInfoPage)
	m_nGap = 0;
	m_mapPath = _T("");
	m_path = _T("");
	m_nDisplayMode = 0;
	//}}AFX_DATA_INIT
	m_psp.dwFlags &= ~PSP_HASHELP;

	m_path = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	m_currentPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH);
	m_nGap = GetPrivateProfileInt(ENVIRONMENT, "gap", 5, m_currentPath + "\\workshop.ini");
	
	char twb[512];
	GetPrivateProfileString(ENVIRONMENT, SHOPIPS, "", twb, 512, m_currentPath + "\\workshop.ini");
	m_ips = twb;

	GetPrivateProfileString(ENVIRONMENT, "INITMAP", "", twb, sizeof(twb), m_currentPath + "\\workshop.ini");
	m_mapPath = twb;

	m_nDisplayMode = GetPrivateProfileInt(ENVIRONMENT, "DISPLAYMODE", 0, m_currentPath + "\\workshop.ini");
}

CBaseInfoPage::~CBaseInfoPage()
{
}

void CBaseInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBaseInfoPage)
	DDX_Control(pDX, IDC_ADDR, m_addr);
	DDX_Text(pDX, IDC_EDITGAP, m_nGap);
	DDV_MinMaxInt(pDX, m_nGap, 0, 1000);
	DDX_Text(pDX, IDC_MAPPATH, m_mapPath);
	DDX_Text(pDX, IDC_PATH, m_path);
	DDX_Radio(pDX, IDC_DISPLAYDATA_RDO, m_nDisplayMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBaseInfoPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBaseInfoPage)
	ON_BN_CLICKED(IDC_DISPLAYDATA_RDO, OnDisplayMode)
	ON_BN_CLICKED(IDC_DISPLAYSYMBOL_RDO, OnDisplayMode)
	ON_BN_CLICKED(IDC_DSPLAYNONE_RDO, OnDisplayMode)
	ON_BN_CLICKED(IDC_BROWSER, OnBrowser)
	ON_BN_CLICKED(IDC_BROWSER2, OnBrowser2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseInfoPage message handlers

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
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBaseInfoPage::OnOK() 
{
	
	CPropertyPage::OnOK();
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

void CBaseInfoPage::OnDisplayMode() 
{
	UpdateData(TRUE);	
}

void CBaseInfoPage::OnBrowser() 
{
	UpdateData(TRUE);
	CFolderBrowser folder(m_path);
	if (folder.DoModal() == IDOK)
	{
		m_path = folder.GetPathName();
		UpdateData(FALSE);
	}	
}

void CBaseInfoPage::OnBrowser2() 
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
