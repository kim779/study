// configDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "configDlg.h"

#include "../h/axisvar.h"
#include "../h/axisfire.h"

#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#pragma	comment(lib, "Winmm.lib")
/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

CConfigDlg::CConfigDlg(CWnd* pParent, CString root)
	: CDialog(CConfigDlg::IDD, pParent)
{
	m_path.Format("%s\\%s\\config.ini", root, TABDIR);
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_EN_KILLFOCUS(IDC_SCNE, OnScreenKillFocus)
	ON_EN_KILLFOCUS(IDC_MSGE, OnIntervalKillFocus)
	ON_BN_CLICKED(IDC_MDCON_SEARCH, OnMdconSearch)
	ON_BN_CLICKED(IDC_MDCON_PLAY, OnMdconPlay)
	ON_BN_CLICKED(IDC_MDCON, OnMdcon)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_SCN01,  IDC_SCN07,  OnEditScreenCount)
	ON_COMMAND_RANGE(IDC_MSG01,  IDC_MSG02,  OnEditInterval)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int	value;
	CString	tmps;
	CWinApp* app = AfxGetApp();

	// 화면갯수
	value = app->GetProfileInt(ENVIRONMENT, WNDINFO, 32);
	value = LOWORD(value);
	GetDlgItem(IDC_SCNE)->EnableWindow(FALSE);
	switch (value)
	{
	case 16:
		((CButton *)GetDlgItem(IDC_SCN05))->SetCheck(1);	break;
	case 32:
		((CButton *)GetDlgItem(IDC_SCN06))->SetCheck(1);	break;
	default:
		((CButton *)GetDlgItem(IDC_SCN07))->SetCheck(1);
		GetDlgItem(IDC_SCNE)->EnableWindow(TRUE);
		tmps.Format("%d", value);
		GetDlgItem(IDC_SCNE)->SetWindowText(tmps);
		break;
	}

	// 윈도우 생성위치
	value = app->GetProfileInt(ENVIRONMENT, CHILDPOS, 0);
	value = HIWORD(value);
	switch (value)
	{
	case cpTOP:
		((CButton *)GetDlgItem(IDC_POS02))->SetCheck(1);	break;
	case cpCENTER:
		((CButton *)GetDlgItem(IDC_POS03))->SetCheck(1);	break;
	case cpMAX:
		((CButton *)GetDlgItem(IDC_POS04))->SetCheck(1);	break;
	case cpDEFAULT:
	default:
		((CButton *)GetDlgItem(IDC_POS01))->SetCheck(1);	break;
	}
	
	// 메세지
	char	wb[64];
	DWORD dwRc = GetPrivateProfileString("MESSAGE", "IntervalSet", "", wb, sizeof(wb), m_path);
	if (dwRc > 0)
	{
		if (!atoi(wb))
		{
			((CButton *)GetDlgItem(IDC_MSG01))->SetCheck(1);
			GetDlgItem(IDC_MSGE)->EnableWindow(FALSE);
			
			dwRc = GetPrivateProfileString("MESSAGE", "Interval", "", wb, sizeof(wb), m_path);
			if (dwRc > 0)
				m_interval = max(0, atoi(wb)) / 1000;
			else
				m_interval = 0;

			tmps.Format("%d", m_interval);
			GetDlgItem(IDC_MSGE)->SetWindowText(tmps);
		}
		else
		{
			dwRc = GetPrivateProfileString("MESSAGE", "Interval", "", wb, sizeof(wb), m_path);
			if (dwRc > 0)
				m_interval = max(0, atoi(wb)) / 1000;
			else
				m_interval = 0;

			((CButton *)GetDlgItem(IDC_MSG02))->SetCheck(1);
			GetDlgItem(IDC_MSGE)->EnableWindow(TRUE);
			tmps.Format("%d", m_interval);
			GetDlgItem(IDC_MSGE)->SetWindowText(tmps);
		}
	}
	else
	{
		((CButton *)GetDlgItem(IDC_MSG01))->SetCheck(1);
		GetDlgItem(IDC_MSGE)->EnableWindow(FALSE);

		dwRc = GetPrivateProfileString("MESSAGE", "Interval", "", wb, sizeof(wb), m_path);
		if (dwRc > 0)
			m_interval = max(0, atoi(wb)) / 1000;
		else
			m_interval = 0;

		tmps.Format("%d", m_interval);
		GetDlgItem(IDC_MSGE)->SetWindowText(tmps);
	}

	dwRc = GetPrivateProfileString("MESSAGE", "POS", "4", wb, sizeof(wb), m_path);
	if (dwRc > 0)
		value = atoi(wb);
	else
		value = RbottomPOS;
	switch (value)
	{
	case LtopPOS:
		((CButton *)GetDlgItem(IDC_PO7))->SetCheck(1);		break;
	case RtopPOS:
		((CButton *)GetDlgItem(IDC_PO8))->SetCheck(1);		break;
	case LbottomPOS:
		((CButton *)GetDlgItem(IDC_PO9))->SetCheck(1);		break;
	case CenterPOS:
		((CButton *)GetDlgItem(IDC_PO11))->SetCheck(1);		break;
	case RbottomPOS:
	default:
		((CButton *)GetDlgItem(IDC_PO10))->SetCheck(1);		break;
	}

	// 팝업 주문창 위치
	dwRc = GetPrivateProfileString("POPORDER", "POS", "4", wb, sizeof(wb), m_path);
	if (dwRc > 0)
		value = atoi(wb);
	else
		value = RbottomPOS;
	switch (value)
	{
	case LtopPOS:
		((CButton *)GetDlgItem(IDC_PO1))->SetCheck(1);		break;
	case RtopPOS:
		((CButton *)GetDlgItem(IDC_PO2))->SetCheck(1);		break;
	case LbottomPOS:
		((CButton *)GetDlgItem(IDC_PO3))->SetCheck(1);		break;
	case LCenterPOS:
		((CButton *)GetDlgItem(IDC_PO5))->SetCheck(1);		break;
 	case RCenterPOS:
 		((CButton *)GetDlgItem(IDC_PO6))->SetCheck(1);		break;
	case CenterPOS:
		((CButton *)GetDlgItem(IDC_CENTER))->SetCheck(1);	break;
	case RbottomPOS:
	default:
		((CButton *)GetDlgItem(IDC_PO4))->SetCheck(1);		break;
	}

	// Flash 표시
	dwRc = GetPrivateProfileString("DISPLAY", "FLASH", "1", wb, sizeof(wb), m_path);
	if (dwRc > 0)
		m_flash = atoi(wb) ? false : true;
	else
		m_flash = false;
	((CButton *)GetDlgItem(IDC_FLASH))->SetCheck(m_flash ? 1 : 0);

	// 계좌창정렬
	dwRc = GetPrivateProfileString("ACCOUNT", "SORT", "0", wb, sizeof(wb), m_path);
	if (dwRc > 0 && wb[0] == '1')
		((CButton *)GetDlgItem(IDC_ANAM))->SetCheck(1);
	else
		((CButton *)GetDlgItem(IDC_ACCT))->SetCheck(1);

	// 옵션 정렬
	dwRc = GetPrivateProfileString("OPTION", "SORT", "0", wb, sizeof(wb), m_path);
	if (dwRc > 0 && wb[0] == '1')
		((CButton *)GetDlgItem(IDC_OP_UP))->SetCheck(1);
	else
		((CButton *)GetDlgItem(IDC_OP_DOWN))->SetCheck(1);

	if (GetPrivateProfileInt("Common", "SOUNDON_MDCON", 1, m_path))
		((CButton *) GetDlgItem(IDC_MDCON))->SetCheck(1);

	dwRc = GetPrivateProfileString("Common", "SOUNDFILE_MDCON", "", wb, sizeof(wb), m_path);
	if (dwRc > 0)	m_waveF = wb;

	GetDlgItem(IDC_MDCON_FILENAME)->SetWindowText(m_waveF);

	dwRc = GetPrivateProfileString("Common", "Interval", "", wb, sizeof(wb), m_path);
	if (dwRc > 0)
		m_CONinterval = max(0, atoi(wb)) / 1000;
	else
		m_CONinterval = 0;

	tmps.Format("%d", m_CONinterval);
	GetDlgItem(IDC_DelayTimer)->SetWindowText(tmps);

	dwRc = GetPrivateProfileString("JUMUN_CNT", "FACNT", "0", wb, sizeof(wb), m_path);
	tmps = wb;
	GetDlgItem(IDC_FACNT)->SetWindowText(tmps);

	dwRc = GetPrivateProfileString("JUMUN_CNT", "OPCNT", "0", wb, sizeof(wb), m_path);
	tmps = wb;
	GetDlgItem(IDC_OPCNT)->SetWindowText(tmps);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::OnOK() 
{
	int	value;
	CString	tmps, tmpx;
	CWinApp* app = AfxGetApp();

	// 화면갯수
	value = 32;
	if (((CButton *)GetDlgItem(IDC_SCN05))->GetCheck())
		value = 16;
	else if (((CButton *)GetDlgItem(IDC_SCN06))->GetCheck())
		value = 32;
	else if (((CButton *)GetDlgItem(IDC_SCN07))->GetCheck())
	{
		GetDlgItem(IDC_SCNE)->GetWindowText(tmps);
		value = atoi(tmps);
	}
	value = max(16, min(64, value));
	value = MAKELONG(value, 0);
	app->WriteProfileInt(ENVIRONMENT, WNDINFO, value);

	// 윈도우 생성위치
	value = cpDEFAULT;
	if (((CButton *)GetDlgItem(IDC_POS02))->GetCheck())
		value = cpTOP;
	else if (((CButton *)GetDlgItem(IDC_POS03))->GetCheck())
		value = cpCENTER;
	else if (((CButton *)GetDlgItem(IDC_POS04))->GetCheck())
		value = cpMAX;
	else if (((CButton *)GetDlgItem(IDC_POS01))->GetCheck())
		value = cpDEFAULT;

	value = MAKELONG(0, value);
	app->WriteProfileInt(ENVIRONMENT, CHILDPOS, value);

	// 메세지
	if (((CButton *)GetDlgItem(IDC_MSG01))->GetCheck())
	{
		tmps = "0";
		WritePrivateProfileString("MESSAGE", "IntervalSet", tmps, m_path);
		GetDlgItem(IDC_MSGE)->GetWindowText(tmps);
		m_interval = max(0, min(20, atoi(tmps)));
		tmps.Format("%d", m_interval * 1000);
		WritePrivateProfileString("MESSAGE", "Interval", tmps, m_path);
	}
	else
	{
		tmps = "1";
		WritePrivateProfileString("MESSAGE", "IntervalSet", tmps, m_path);

		m_interval = 0;
		if (((CButton *)GetDlgItem(IDC_MSG02))->GetCheck())
		{
			GetDlgItem(IDC_MSGE)->GetWindowText(tmps);
			m_interval = max(0, min(20, atoi(tmps)));
		}
		tmps.Format("%d", m_interval * 1000);
		WritePrivateProfileString("MESSAGE", "Interval", tmps, m_path);
	}

	value = RtopPOS;
	if (((CButton *)GetDlgItem(IDC_PO7))->GetCheck())
		value = LtopPOS;
	else if (((CButton *)GetDlgItem(IDC_PO8))->GetCheck())
		value = RtopPOS;
	else if (((CButton *)GetDlgItem(IDC_PO9))->GetCheck())
		value = LbottomPOS;
	else if (((CButton *)GetDlgItem(IDC_PO10))->GetCheck())
		value = RbottomPOS;
	else if (((CButton *)GetDlgItem(IDC_PO11))->GetCheck())
		value = CenterPOS;

	tmps.Format("%d", value);
	WritePrivateProfileString("MESSAGE", "POS", tmps, m_path);

	// 사용자 화면 Open시 기존화면 유지
	tmps.Format("%d", 1);
	WritePrivateProfileString("SCREEN", "CLOSE", tmps, m_path);

	// 팝업 주문창 위치
	value = RbottomPOS;
	if (((CButton *)GetDlgItem(IDC_PO1))->GetCheck())
		value = LtopPOS;
	else if (((CButton *)GetDlgItem(IDC_PO2))->GetCheck())
		value = RtopPOS;
	else if (((CButton *)GetDlgItem(IDC_PO3))->GetCheck())
		value = LbottomPOS;
	else if (((CButton *)GetDlgItem(IDC_PO4))->GetCheck())
		value = RbottomPOS;
	else if (((CButton *)GetDlgItem(IDC_PO5))->GetCheck())
		value = LCenterPOS;
 	else if (((CButton *)GetDlgItem(IDC_PO6))->GetCheck())
 		value = RCenterPOS;
	else if (((CButton *)GetDlgItem(IDC_CENTER))->GetCheck())
 		value = CenterPOS;

	tmps.Format("%d", value);
	WritePrivateProfileString("POPORDER", "POS", tmps, m_path);

	// Flash 표시
	if (((CButton *)GetDlgItem(IDC_FLASH))->GetCheck())
		m_flash = true;
	else
		m_flash = false;
	tmps.Format("%d", m_flash ? 0 : 1);
	WritePrivateProfileString("DISPLAY", "FLASH", tmps, m_path);

	// 계좌창정렬
	value = 1;
	if (((CButton *)GetDlgItem(IDC_ACCT))->GetCheck())
		value = 0;
	tmps.Format("%d", value);
	WritePrivateProfileString("ACCOUNT", "SORT", tmps, m_path);

	// 옵션 정렬
	value = 1;
	if (((CButton *)GetDlgItem(IDC_OP_UP))->GetCheck())
		value = 1;
	else if (((CButton *)GetDlgItem(IDC_OP_DOWN))->GetCheck())
		value = 0;

	tmps.Format("%d", value);
	WritePrivateProfileString("OPTION", "SORT", tmps, m_path);

	if (((CButton *) GetDlgItem(IDC_MDCON))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_MDCON", "1", m_path);
	else	WritePrivateProfileString("Common", "SOUNDON_MDCON", "0", m_path);

	WritePrivateProfileString("Common", "SOUNDFILE_MDCON", m_waveF, m_path);

	GetDlgItem(IDC_DelayTimer)->GetWindowText(tmps);
	m_CONinterval = max(0, min(60, atoi(tmps)));

	tmps.Format("%d", m_CONinterval * 1000);
	WritePrivateProfileString("Common", "Interval", tmps, m_path);

	GetDlgItem(IDC_FACNT)->GetWindowText(tmps);
	WritePrivateProfileString("JUMUN_CNT", "FACNT", tmps, m_path);

	GetDlgItem(IDC_OPCNT)->GetWindowText(tmps);
	WritePrivateProfileString("JUMUN_CNT", "OPCNT", tmps, m_path);

	CDialog::OnOK();
}

void CConfigDlg::OnEditScreenCount(UINT cmdID) 
{
	if (((CButton *)GetDlgItem(IDC_SCN07))->GetCheck())
		GetDlgItem(IDC_SCNE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_SCNE)->EnableWindow(FALSE);
}

void CConfigDlg::OnScreenKillFocus() 
{
	CString	tmps;

	GetDlgItem(IDC_SCNE)->GetWindowText(tmps);
	tmps.Format("%d", max(1, min(64, atoi(tmps))));
	GetDlgItem(IDC_SCNE)->SetWindowText(tmps);
}

void CConfigDlg::OnEditInterval(UINT cmdID) 
{
	if (((CButton *)GetDlgItem(IDC_MSG02))->GetCheck())
		GetDlgItem(IDC_MSGE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_MSGE)->EnableWindow(FALSE);
}

void CConfigDlg::OnIntervalKillFocus() 
{
	CString	tmps;

	GetDlgItem(IDC_MSGE)->GetWindowText(tmps);
	tmps.Format("%d", max(0, min(20, atoi(tmps))));
	GetDlgItem(IDC_MSGE)->SetWindowText(tmps);
}

void CConfigDlg::SearchFile()
{
	CFileDialog	dlg(TRUE, "*.wav", m_waveF, 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER,
			"Sound file(*.wav) |*.wav||", NULL );
	if (dlg.DoModal() == IDOK)
	{
		m_waveF = dlg.GetPathName();
		GetDlgItem(IDC_MDCON_FILENAME)->SetWindowText(m_waveF);
	}
}

void CConfigDlg::Soundplay()
{
	if (m_waveF.IsEmpty())
		MessageBeep(MB_OK);
	else
	{
		sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
		sndPlaySound(m_waveF, SND_NODEFAULT|SND_ASYNC);
	}
}

void CConfigDlg::OnMdconSearch() 
{
	SearchFile();		
}

void CConfigDlg::OnMdconPlay() 
{
	Soundplay();	
}

void CConfigDlg::OnMdcon() 
{
	if (((CButton *)GetDlgItem(IDC_MDCON))->GetCheck())
		GetDlgItem(IDC_DelayTimer)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DelayTimer)->EnableWindow(FALSE);
}
