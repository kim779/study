// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300200.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CString file, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_file = file;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, PERCENT_USER, m_user);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(CONFIRM_CHK, OnChk)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_user.SetNumeric();

	((CComboBox *)GetDlgItem(PERCENT_COMBO))->AddString("기준가");
	((CComboBox *)GetDlgItem(PERCENT_COMBO))->AddString("전일 종가");
	((CComboBox *)GetDlgItem(PERCENT_COMBO))->AddString("시가");
	((CComboBox *)GetDlgItem(PERCENT_COMBO))->AddString("고가");
	((CComboBox *)GetDlgItem(PERCENT_COMBO))->AddString("저가");

	InitData();

	if (m_config.percent == 0)
		((CButton *)GetDlgItem(PERCENT_NO))->SetCheck(1);
	else if (m_config.percent == 1)
		((CButton *)GetDlgItem(PERCENT_DRYL))->SetCheck(1);
	else if (m_config.percent == 2)
		((CButton *)GetDlgItem(PERCENT_DIFF))->SetCheck(1);
//	((CButton *)GetDlgItem(PERCENT_CHK))->SetCheck(m_config.percent);
	if (m_config.percentRadio == 0)
		((CButton *)GetDlgItem(PERCENT_RADIO1))->SetCheck(1);
	else if (m_config.percentRadio == 1)
		((CButton *)GetDlgItem(PERCENT_RADIO2))->SetCheck(1);
	else if (m_config.percentRadio == 2)
		((CButton *)GetDlgItem(PERCENT_RADIO3))->SetCheck(1);
	((CComboBox *)GetDlgItem(PERCENT_COMBO))->SetCurSel(m_config.percentKind);
	((CEdit *)GetDlgItem(PERCENT_USER))->SetWindowText(m_config.user);

	if (m_config.mod == 0)
		((CButton *)GetDlgItem(MOD_ALL))->SetCheck(1);
	else
		((CButton *)GetDlgItem(MOD_PART))->SetCheck(1);

	if (m_config.jmcn == 0)
		((CButton *)GetDlgItem(JMCN_ALL))->SetCheck(1);
	else
		((CButton *)GetDlgItem(JMCN_PART))->SetCheck(1);

	((CButton *)GetDlgItem(CONFIRM_CHK))->SetCheck(m_config.confirm);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigDlg::InitData()
{
	m_config.percent = GetDataInt("HOGAJUMUN", "percent", 0, m_file);
	m_config.percentRadio = GetDataInt("HOGAJUMUN", "percentRadio", 0, m_file);
	m_config.percentKind = GetDataInt("HOGAJUMUN", "percentKind", 1, m_file);
	m_config.user = GetDataString("HOGAJUMUN", "user", "0", m_file);
	m_config.mod = GetDataInt("HOGAJUMUN", "mod", 1, m_file);
	m_config.jmcn = GetDataInt("HOGAJUMUN", "jmcn", 0, m_file);
	m_config.confirm = GetDataInt("HOGAJUMUN", "confirm", 1, m_file);
}

void CConfigDlg::OnOK()
{
	SetConfigData();

	CString tmpS;

	tmpS.Format("%d", m_config.percent);
	SetData("HOGAJUMUN", "percent", tmpS, m_file);
	tmpS.Format("%d", m_config.percentRadio);
	SetData("HOGAJUMUN", "percentRadio", tmpS, m_file);
	tmpS.Format("%d", m_config.percentKind);
	SetData("HOGAJUMUN", "percentKind", tmpS, m_file);
	SetData("HOGAJUMUN", "user", m_config.user, m_file);
	
	tmpS.Format("%d", m_config.mod);
	SetData("HOGAJUMUN", "mod", tmpS, m_file);
	
	tmpS.Format("%d", m_config.jmcn);
	SetData("HOGAJUMUN", "jmcn", tmpS, m_file);

	tmpS.Format("%d", m_config.confirm);
	SetData("HOGAJUMUN", "confirm", tmpS, m_file);

	CDialog::OnOK();
}

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CConfigDlg::SetConfigData()
{
	if (((CButton *)GetDlgItem(PERCENT_NO))->GetCheck())
		m_config.percent = 0;
	else if (((CButton *)GetDlgItem(PERCENT_DRYL))->GetCheck())
		m_config.percent = 1;
	else if (((CButton *)GetDlgItem(PERCENT_DIFF))->GetCheck())
		m_config.percent = 2;
//	m_config.percent = ((CButton *)GetDlgItem(PERCENT_CHK))->GetCheck();

	if (((CButton *)GetDlgItem(PERCENT_RADIO1))->GetCheck())
		m_config.percentRadio = 0;
//	else if (((CButton *)GetDlgItem(PERCENT_RADIO2))->GetCheck())
//		m_config.percentRadio = 1;
	else if (((CButton *)GetDlgItem(PERCENT_RADIO3))->GetCheck())
		m_config.percentRadio = 2;
	m_config.percentKind = ((CComboBox *)GetDlgItem(PERCENT_COMBO))->GetCurSel();
	((CEdit *)GetDlgItem(PERCENT_USER))->GetWindowText(m_config.user);
	m_config.user.Remove(',');

	if (((CButton *)GetDlgItem(MOD_ALL))->GetCheck())
		m_config.mod = 0;
	else if (((CButton *)GetDlgItem(MOD_PART))->GetCheck())
		m_config.mod = 1;

	if (((CButton *)GetDlgItem(JMCN_ALL))->GetCheck())
		m_config.jmcn = 0;
	else if (((CButton *)GetDlgItem(JMCN_PART))->GetCheck())
		m_config.jmcn = 1;

	m_config.confirm = ((CButton *)GetDlgItem(CONFIRM_CHK))->GetCheck();
}

void CConfigDlg::SetData(CString major, CString minor, CString data, CString file)
{
	WritePrivateProfileString(major, minor, data, file);
}

CString CConfigDlg::GetDataString(CString major, CString minor, CString init, CString file)
{
	char	wb[32];

	GetPrivateProfileString(major, minor, init, wb, sizeof(wb), file);
	return wb;
}

int CConfigDlg::GetDataInt(CString major, CString minor, int init, CString file)
{
	return GetPrivateProfileInt(major, minor, init, file);
}

void CConfigDlg::OnChk() 
{
	::MessageBox(m_hWnd, "호가주문의 특성상 마우스의 더블클릭만으로 주문이 제출되고 체결될 수 있으니 주문입력 확인창의 확인절차없이 주문이 제출되어 발생하는 문제는 본 서비스를 제공하는 회사가 책임을 지지않음을 알려드립니다. ",
		"주문확인 경고", MB_ICONWARNING);
}

HBRUSH CConfigDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	const UINT nID = pWnd->GetDlgCtrlID();
    if (nID == IDC_STATIC_RED)	pDC->SetTextColor(RGB(255,0,0));	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
