// TabAuthority.cpp : implementation file
//

#include "stdafx.h"
#include "TabAuthority.h"
#include "DlgSTOption.h"
#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabAuthority dialog


CTabAuthority::CTabAuthority(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabAuthority::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabAuthority)
	m_bChkPassword = FALSE;
	m_strPassword = _T("");
	m_strPassword2 = _T("");
	m_strUserID = _T("");
	m_timeExpiredDate = CTime::GetCurrentTime();
	m_bChkExpiredDate = FALSE;
	m_bChkUserID = FALSE;
	//}}AFX_DATA_INIT
	m_pDlgSTOption = pParent;
}


void CTabAuthority::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabAuthority)
	DDX_Control(pDX, IDC_CHKPASSWD2, m_stPassword2);
	DDX_Control(pDX, IDC_DATETIMEPICKER_EXPIREDDATE, m_datepickerExpiredDate);
	DDX_Control(pDX, IDC_EDITUSERID, m_editUserID);
	DDX_Control(pDX, IDC_EDITPASSWORD2, m_editPassword2);
	DDX_Control(pDX, IDC_EDITPASSWORD, m_editPassword);
	DDX_Control(pDX, IDC_CHKUSERID, m_chkUserID);
	DDX_Control(pDX, IDC_CHKPASSWD, m_chkPassword);
	DDX_Control(pDX, IDC_CHKEXPIREDDATE, m_chkExpiredDate);
	DDX_Check(pDX, IDC_CHKPASSWD, m_bChkPassword);
	DDX_Text(pDX, IDC_EDITPASSWORD, m_strPassword);
	DDX_Text(pDX, IDC_EDITPASSWORD2, m_strPassword2);
	DDX_Text(pDX, IDC_EDITUSERID, m_strUserID);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_EXPIREDDATE, m_timeExpiredDate);
	DDX_Check(pDX, IDC_CHKEXPIREDDATE, m_bChkExpiredDate);
	DDX_Check(pDX, IDC_CHKUSERID, m_bChkUserID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabAuthority, CRscDlg)
	//{{AFX_MSG_MAP(CTabAuthority)
	ON_BN_CLICKED(IDC_CHKEXPIREDDATE, OnChkexpireddate)
	ON_BN_CLICKED(IDC_CHKPASSWD, OnChkpasswd)
	ON_BN_CLICKED(IDC_CHKUSERID, OnChkuserid)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabAuthority message handlers

void CTabAuthority::OnChkexpireddate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bChkExpiredDate)
	{
		m_datepickerExpiredDate.EnableWindow();
	}	
	else
	{
		m_datepickerExpiredDate.EnableWindow(FALSE);
	}
	
}

void CTabAuthority::OnChkpasswd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bChkPassword)
	{
		m_editPassword.EnableWindow();
		m_editPassword2.EnableWindow();		
	}	
	else
	{
		m_editPassword.EnableWindow(FALSE);
		m_editPassword2.EnableWindow(FALSE);
	}
}

void CTabAuthority::OnChkuserid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if(m_bChkUserID)
	{
		m_editUserID.EnableWindow();
	}	
	else
	{
		m_editUserID.EnableWindow(FALSE);
	}
	
}

BOOL CTabAuthority::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabAuthority::LoadInfo()
{
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();

	CString strPassword = ((CDlgSTOption*)m_pDlgSTOption)->GetPassword();
	BOOL	bUsePassword = ((CDlgSTOption*)m_pDlgSTOption)->GetUsePassword();
	if(strPassword.GetLength())
	{
		m_strPassword = strPassword;
		m_strPassword2 = strPassword;
	}
	if(!bUsePassword)
	{
		m_editPassword.EnableWindow(FALSE);
		m_editPassword2.EnableWindow(FALSE);
		m_bChkPassword = FALSE;
	}
	else
	{
		m_bChkPassword = TRUE;
	}

	CString strUserID = ((CDlgSTOption*)m_pDlgSTOption)->GetUserID();
	BOOL bUseUserID = ((CDlgSTOption*)m_pDlgSTOption)->GetUseUserID();
	if(strUserID.GetLength())
	{
		m_strUserID = strUserID;
	}
	if(!bUseUserID)
	{
		m_editUserID.EnableWindow(FALSE);
		m_bChkUserID = FALSE;
	}
	else
	{
		m_bChkUserID = TRUE;
	}

	CString strExpiredDate = ((CDlgSTOption*)m_pDlgSTOption)->GetExpiredDate();
	BOOL bUseExpiredDate = ((CDlgSTOption*)m_pDlgSTOption)->GetUseExpiredDate();
	if(strExpiredDate.GetLength()==8)
	{
		CString strYear = strExpiredDate.Left(4);
		CString strMonth = strExpiredDate.Mid(4,2);
		CString strDay = strExpiredDate.Right(2);
		CTime timeFromOutside(atol(strYear),atol(strMonth),atol(strDay),0,0,0);
		m_timeExpiredDate = timeFromOutside;
	}
	if(!bUseExpiredDate)
	{
		m_datepickerExpiredDate.EnableWindow(FALSE);
		m_bChkExpiredDate = FALSE;
	}	
	else
	{
		m_bChkExpiredDate = TRUE;
	}

	if(pSTControllerCfg)
	{
		m_chkPassword.EnableWindow(FALSE);
		m_stPassword2.EnableWindow(FALSE);
		m_chkUserID.EnableWindow(FALSE);
		m_chkExpiredDate.EnableWindow(FALSE);

		m_editPassword.EnableWindow(FALSE);
		m_editPassword2.EnableWindow(FALSE);
		m_editUserID.EnableWindow(FALSE);
		m_datepickerExpiredDate.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

LRESULT CTabAuthority::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_ST_SAVE_ENV:
		{
			return SaveInfo();
		}		
		break;
	}	
	return CRscDlg::WindowProc(message, wParam, lParam);
}

BOOL CTabAuthority::SaveInfo()
{
	UpdateData();

	if(m_bChkPassword)
	{
		if(m_strPassword==m_strPassword2)
		{
			((CDlgSTOption*)m_pDlgSTOption)->SetPassword(m_strPassword);
		}
		else
		{
			((CDlgSTOption*)m_pDlgSTOption)->SetErrorMessage("2개의 비밀번호가 일치하지 않습니다. 일치시켜주십시오.");
			return FALSE;
		}		
	}
	else
	{
		((CDlgSTOption*)m_pDlgSTOption)->SetPassword(m_strPassword);
	}
	((CDlgSTOption*)m_pDlgSTOption)->SetUsePassword(m_bChkPassword);	
	((CDlgSTOption*)m_pDlgSTOption)->SetUseUserID(m_bChkUserID);
	((CDlgSTOption*)m_pDlgSTOption)->SetUserID(m_strUserID);
	((CDlgSTOption*)m_pDlgSTOption)->SetUseExpiredDate(m_bChkExpiredDate);
	((CDlgSTOption*)m_pDlgSTOption)->SetExpiredDate(m_timeExpiredDate.Format("%Y%m%d"));
	return TRUE;
}

HBRUSH CTabAuthority::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return *m_pBrBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
