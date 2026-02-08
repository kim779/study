// DlgLoadScript.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "DlgLoadScript.h"
#include "../Include_ST/MessageDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadScript dialog


CDlgLoadScript::CDlgLoadScript(LPCSTR lpszName, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLoadScript::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLoadScript)
	m_strName = _T(lpszName);
	//}}AFX_DATA_INIT
}


void CDlgLoadScript::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLoadScript)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_stsResult);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 64);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLoadScript, CDialog)
	//{{AFX_MSG_MAP(CDlgLoadScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLoadScript message handlers

BOOL CDlgLoadScript::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_stsResult.SetBkColor(RGB(255, 255, 255));

	m_btnOK.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);
	m_btnCancel.SetXButtonStyle(BS_XT_XPFLAT|BS_XT_SHOWFOCUS);

	m_editName.SetSel(-1);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLoadScript::OnOK() 
{
	if(!VerifyScriptName())
		return;
	
	CDialog::OnOK();
}

BOOL CDlgLoadScript::VerifyScriptName()
{
	UpdateData();

	LRESULT lResult = theApp.m_pMainWnd->SendMessage(RMSG_VERIFYSCRIPTNAME, 0, (LPARAM)(LPCSTR)m_strName);
	if(lResult == 0L)
	{
		CString strName;
		strName.Format(_T("[%s] 이름은 사용할 수 없습니다."), m_strName);

		m_stsResult.SetText(strName);

		m_editName.SetFocus();
		m_editName.SetSel(0, -1);

		return FALSE;
	}

	return TRUE;
}
