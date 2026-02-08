// TabBasic.cpp : implementation file
//

#include "stdafx.h"
#include "TabBasic.h"
#include "DlgSTOption.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTabBasic dialog

CTabBasic::CTabBasic(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabBasic::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabBasic)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlgSTOption = pParent;
}


void CTabBasic::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabBasic)
	DDX_Control(pDX, IDC_EDIT_LANG_EXPLANATION, m_editLangExplanation);
	DDX_Control(pDX, IDC_EDIT_EXPLANATION, m_editExplanation);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_RADIO_LANGTYPE0, m_ctrlLangType0);
	DDX_Control(pDX, IDC_RADIO_LANGTYPE1, m_ctrlLangType1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabBasic, CRscDlg)
	//{{AFX_MSG_MAP(CTabBasic)
	ON_BN_CLICKED(IDC_RADIO_LANGTYPE0, OnRadioLangtype0)
	ON_BN_CLICKED(IDC_RADIO_LANGTYPE1, OnRadioLangtype1)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabBasic message handlers

BOOL CTabBasic::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CTabBasic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

void CTabBasic::LoadInfo()
{
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();
	if(pSTControllerCfg || ((CDlgSTOption*)m_pDlgSTOption)->GetType() == 3 || ((CDlgSTOption*)m_pDlgSTOption)->GetType() == 4)
	{
		m_editName.SetReadOnly();
		m_ctrlLangType0.EnableWindow(FALSE);
		m_ctrlLangType1.EnableWindow(FALSE);
	}

	m_editName.SetWindowText(((CDlgSTOption*)m_pDlgSTOption)->GetSdPath());	
	
	char szBuffer[256];
	DWORD dwSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(BASIC,EXPLANATION,"",szBuffer,sizeof(szBuffer), 1);
	if(dwSizeRead)
	{
		m_editExplanation.SetWindowText(szBuffer);
	}

	int nType = 0;
	if(pSTControllerCfg)
	{
		nType = pSTControllerCfg->GetSourceType();
	}
	else
	{
		dwSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(BASIC,LANGTYPE,"",szBuffer,sizeof(szBuffer),1);
		if(dwSizeRead)
		{
			nType = atol(szBuffer);
		}
	}		
	if(nType==1)
	{
		m_ctrlLangType0.SetCheck(0);
		m_ctrlLangType1.SetCheck(1);
		OnRadioLangtype1();
	}
	else
	{
		m_ctrlLangType0.SetCheck(1);
		m_ctrlLangType1.SetCheck(0);
		OnRadioLangtype0();
	}
}

BOOL CTabBasic::SaveInfo()
{
	if(!((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD())
	{
		CString strName;
		m_editName.GetWindowText(strName);
		((CDlgSTOption*)m_pDlgSTOption)->ChangeSfgPath(strName);	
	}
	
	CString strText;
	m_editExplanation.GetWindowText(strText);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(BASIC,EXPLANATION,strText,1);

	if(m_ctrlLangType0.GetCheck())
	{
		strText.Format("0");
	}
	else if(m_ctrlLangType1.GetCheck())
	{
		strText.Format("1");
	}
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(BASIC,LANGTYPE,strText,1);
	return TRUE;
}

void CTabBasic::OnRadioLangtype0() 
{
	// TODO: Add your control notification handler code here
	m_editLangExplanation.SetWindowText("Trade Station에서 사용되는 문법");
}

void CTabBasic::OnRadioLangtype1() 
{
	// TODO: Add your control notification handler code here
	m_editLangExplanation.SetWindowText("Yes Language에서 사용되는 문법");
	
}


HBRUSH CTabBasic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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