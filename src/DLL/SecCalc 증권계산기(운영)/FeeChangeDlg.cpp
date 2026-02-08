// FeeChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecCalc.h"
#include "FeeChangeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFeeChangeDlg dialog
int	g_FeeEdit[] = {IDC_EDIT1, IDC_EDIT2, IDC_EDIT3, IDC_EDIT4 };
int g_ChaEdit[] = {IDC_EDIT5, IDC_EDIT6, IDC_EDIT7, IDC_EDIT8 };

CFeeChangeDlg::CFeeChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeeChangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFeeChangeDlg)
	//}}AFX_DATA_INIT
	m_pApp = (CSecCalcApp *)AfxGetApp();
}


void CFeeChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeeChangeDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeeChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CFeeChangeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFeeChangeDlg message handlers

BOOL CFeeChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (int ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		((CEdit*)GetDlgItem(g_FeeEdit[ii]))->SetWindowText(m_pApp->m_pFeeRate[ii]);
	}

	for (ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		((CEdit*)GetDlgItem(g_ChaEdit[ii]))->SetWindowText(m_pApp->m_pCharFee[ii]);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFeeChangeDlg::OnOK() 
{
	CString	str;
	double	dFee;
	for (int ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		GetDlgItem(g_FeeEdit[ii])->GetWindowText(str);
		dFee = atof(str);
		if (dFee <= 0)
		{
			AfxMessageBox("변경된 수수료에 0이하의 값이 있습니다.");
			return;
		}
	}	
	CString tmp;
	
	for (ii = mk_Stock; ii <= mk_Elw; ii++)
	{
		GetDlgItem(g_FeeEdit[ii])->GetWindowText(str);
		str.TrimLeft();	str.TrimRight();
		tmp.Format("%f",atof(str));
		while(true)
		{
			if(tmp[tmp.GetLength()-1] == '0')
			{
				tmp.Delete(tmp.GetLength()-1,1);
			}else break;
		}
		m_pApp->m_pFeeRate[ii] = tmp;//str;
		GetDlgItem(g_ChaEdit[ii])->GetWindowText(str);
		str.TrimLeft();	str.TrimRight();
		tmp.Format("%d",atoi(str));
		/*
		while(true)
		{
			if(tmp[tmp.GetLength()-1] == '0')
			{
				tmp.Delete(tmp.GetLength()-1,1);
			}else break;
		}
		*/
		m_pApp->m_pCharFee[ii] = tmp;
	}

	m_pApp->SaveInfo();
	
	CDialog::OnOK();
}
