// HogaFilterDLG.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "HogaFilterDLG.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define CharCombo 52 
#define CharCombo 48
static CString check[8] = {"ALLCANCEL", "CODECANCEL", "BECANCEL", "BEEDIT", "SELORDER", "BUYORDER", "ALLORDER", "AUTOSORT"};

/////////////////////////////////////////////////////////////////////////////
// CHogaFilterDLG dialog


CHogaFilterDLG::CHogaFilterDLG(CWnd* pParent /*=NULL*/, CString root, CString user)
	: CDialog(CHogaFilterDLG::IDD, pParent)
{
	m_path.Format("%s\\%s\\%s\\HogaKey.ini", root, USRDIR, user);
	m_HotPath.Format("%s\\%s\\%s\\HotKey.ini", root, USRDIR, user);
}


void CHogaFilterDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHogaFilterDLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHogaFilterDLG, CDialog)
	//{{AFX_MSG_MAP(CHogaFilterDLG)
	ON_BN_CLICKED(IDC_FutureTrade, OnFutureTrade)
	ON_BN_CLICKED(IDC_FutureDepth, OnFutureDepth)
	ON_BN_CLICKED(IDC_OptionTrade, OnOptionTrade)
	ON_BN_CLICKED(IDC_OptionDepth, OnOptionDepth)
	ON_BN_CLICKED(IDC_HOGAFILTER, OnHogafilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHogaFilterDLG message handlers

void CHogaFilterDLG::OnFutureTrade() 
{
	if (((CButton *)GetDlgItem(IDC_FutureTrade))->GetCheck())
		m_bFutureTrade = TRUE;
	else
		m_bFutureTrade = FALSE;
}

void CHogaFilterDLG::OnFutureDepth() 
{
	if (((CButton *)GetDlgItem(IDC_FutureDepth))->GetCheck())
		m_bFutureDepth = TRUE;
	else
		m_bFutureDepth = FALSE;	
}

void CHogaFilterDLG::OnOptionTrade() 
{
	if (((CButton *)GetDlgItem(IDC_OptionTrade))->GetCheck())
		m_bOptionTrade = TRUE;
	else
		m_bOptionTrade = FALSE;	
}

void CHogaFilterDLG::OnOptionDepth() 
{
	if (((CButton *)GetDlgItem(IDC_OptionDepth))->GetCheck())
		m_bOptionDepth = TRUE;
	else
		m_bOptionDepth = FALSE;
}

BOOL CHogaFilterDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD	dwRc;
	char	wb[64];
	int	value;
	CString volume;

	dwRc = GetPrivateProfileString("HOGA", "COMBOIDX", "0", wb, sizeof(wb), m_path);
	if (dwRc > 0) value = atoi(wb);
	else value = 0;

	((CComboBox *)GetDlgItem(IDC_HOGAKEY))->SetCurSel(value);

	dwRc = GetPrivateProfileString("HOGA", "CHECK", "0", wb, sizeof(wb), m_path);

	if (dwRc > 0 && wb[0] == '1')
	{
		m_bHOGAFilter = TRUE;
		((CButton *)GetDlgItem(IDC_HOGAFILTER))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_HOGAKEY))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_FutureTrade))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_FutureDepth))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_OptionTrade))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_OptionDepth))->EnableWindow(TRUE);
	}
	else
	{
		m_bHOGAFilter = FALSE;
		((CButton *)GetDlgItem(IDC_HOGAFILTER))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_HOGAKEY))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_FutureTrade))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_FutureDepth))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_OptionTrade))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_OptionDepth))->EnableWindow(FALSE);
	}
	
	dwRc = GetPrivateProfileString("HOGA_FILTER", "FutureTrade", "0", wb, sizeof(wb), m_path);

	if (dwRc > 0 && wb[0] == '1')
	{
		((CButton *)GetDlgItem(IDC_FutureTrade))->SetCheck(1);
		m_bFutureTrade = TRUE;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_FutureTrade))->SetCheck(0);
		m_bFutureTrade = FALSE;
	}

	
	dwRc = GetPrivateProfileString("HOGA_FILTER", "FutureDepth", "0", wb, sizeof(wb), m_path);

	if (dwRc > 0 && wb[0] == '1')
	{
		((CButton *)GetDlgItem(IDC_FutureDepth))->SetCheck(1);
		m_bFutureDepth = TRUE;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_FutureDepth))->SetCheck(0);
		m_bFutureDepth = FALSE;
	}

	dwRc = GetPrivateProfileString("HOGA_FILTER", "FutureVolume", "0", wb, sizeof(wb), m_path);
	volume = CString(wb, sizeof(wb));
	if (atoi(volume) <= 0)
		volume = "10";
	((CEdit *)GetDlgItem(IDC_FutureVolume))->SetWindowText(volume);


	dwRc = GetPrivateProfileString("HOGA_FILTER", "OptionTrade", "0", wb, sizeof(wb), m_path);

	if (dwRc > 0 && wb[0] == '1')
	{
		((CButton *)GetDlgItem(IDC_OptionTrade))->SetCheck(1);
		m_bOptionTrade = TRUE;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_OptionTrade))->SetCheck(0);
		m_bOptionTrade = FALSE;
	}


	dwRc = GetPrivateProfileString("HOGA_FILTER", "OptionDepth", "0", wb, sizeof(wb), m_path);

	if (dwRc > 0 && wb[0] == '1')
	{
		((CButton *)GetDlgItem(IDC_OptionDepth))->SetCheck(1);
		m_bOptionDepth = TRUE;
	}
	else
	{
		((CButton *)GetDlgItem(IDC_OptionDepth))->SetCheck(0);
		m_bOptionDepth = FALSE;
	}
	
	dwRc = GetPrivateProfileString("HOGA_FILTER", "OptionVolume", "0", wb, sizeof(wb), m_path);
	volume = CString(wb, sizeof(wb));
	if (atoi(volume) <= 0)
		volume = "10";
	((CEdit *)GetDlgItem(IDC_OptionVolume))->SetWindowText(volume);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHogaFilterDLG::OnHogafilter() 
{
	if (((CButton *)GetDlgItem(IDC_HOGAFILTER))->GetCheck())
	{
		m_bHOGAFilter = TRUE;
		((CButton *)GetDlgItem(IDC_HOGAKEY))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_FutureTrade))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_FutureDepth))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_OptionTrade))->EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_OptionDepth))->EnableWindow(TRUE);
	}
	else
	{
		m_bHOGAFilter = FALSE;
		((CButton *)GetDlgItem(IDC_HOGAKEY))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_FutureTrade))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_FutureDepth))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_OptionTrade))->EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_OptionDepth))->EnableWindow(FALSE);
	}
}

void CHogaFilterDLG::OnOK() 
{
	CString tmps;
	int	value;
	BOOL	bcheck = TRUE;
	
	value = ((CComboBox *)GetDlgItem(IDC_HOGAKEY))->GetCurSel();

	if (value == 0)
		bcheck = FALSE;
	else
	{
		for (int ii =0; ii < 8; ii++)
		{
			DWORD	dwRc;
			char	wb[64];
			dwRc = GetPrivateProfileString(check[ii], "CHECK", "0", wb, sizeof(wb), m_HotPath);

			if (dwRc > 0 && wb[0] == '1')
			{
				dwRc = GetPrivateProfileString(check[ii], "COMBOIDX", "0", wb, sizeof(wb), m_HotPath);
				if (dwRc > 0 && value == atoi(wb)) bcheck = FALSE; 
			}
			else
				continue;
		}
	}
	
	if (!bcheck)
	{
		AfxMessageBox("키가 다른 단축키와 중복되었거나 키가 입력 되어 있질 않습니다.");
		return;
	}

	value = ((CButton *)GetDlgItem(IDC_HOGAFILTER))->GetCheck();

	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA", "CHECK", tmps, m_path);

	value = ((CComboBox *)GetDlgItem(IDC_HOGAKEY))->GetCurSel();
	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA", "COMBOIDX", tmps, m_path);

	switch (value)
	{
	case  0: value = 0;			break;
	case  1: value = VK_ESCAPE;		break;
	case  2: value = VK_SPACE;		break;
	case  3: value = VK_RETURN;		break;
	case  4: value = VK_SHIFT;		break;
	case  5: value = VK_CONTROL;		break;
	case  6: value = VK_MENU;		break;
	case  7: value = VK_F1;			break;
	case  8: value = VK_F2;			break;
	case  9: value = VK_F3;			break;
	case 10: value = VK_F4;			break;
	case 11: value = VK_F5;			break;	
	case 12: value = VK_F6;			break;
	case 13: value = VK_F7;			break;
	case 14: value = VK_F8;			break;
	case 15: value = VK_F9;			break;
	case 16: value = VK_F10;		break;
// 2009. 8. 19. "'" 추가 
// 주의 : 기존 키값과 호환성을 위해서는 항상 마지막에 추가해야 한다.
	case 43: value = VK_OEM_7;		break;
	case 44: value = VK_OEM_1;		break;
	case 45: value = VK_HANGUL;		break;
	case 46: value = VK_HANJA;		break;
	default: value = value + CharCombo;	break;		
	}
	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA", "KEYVALUE", tmps, m_path);

	value = ((CButton *)GetDlgItem(IDC_FutureTrade))->GetCheck();
	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA_FILTER", "FutureTrade", tmps, m_path);
	
	value = ((CButton *)GetDlgItem(IDC_FutureDepth))->GetCheck();
	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA_FILTER", "FutureDepth", tmps, m_path);

	((CEdit *)GetDlgItem(IDC_FutureVolume))->GetWindowText(tmps);
	WritePrivateProfileString("HOGA_FILTER", "FutureVolume", tmps, m_path);

	value = ((CButton *)GetDlgItem(IDC_OptionTrade))->GetCheck();
	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA_FILTER", "OptionTrade", tmps, m_path);

	value = ((CButton *)GetDlgItem(IDC_OptionDepth))->GetCheck();
	tmps.Format("%d", value);
	WritePrivateProfileString("HOGA_FILTER", "OptionDepth", tmps, m_path);
	
	((CEdit *)GetDlgItem(IDC_OptionVolume))->GetWindowText(tmps);
	WritePrivateProfileString("HOGA_FILTER", "OptionVolume", tmps, m_path);
	CDialog::OnOK();
}
