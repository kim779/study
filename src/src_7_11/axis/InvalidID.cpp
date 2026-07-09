// InvalidID.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "InvalidID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvalidID dialog


CInvalidID::CInvalidID(CWnd* pParent /*=NULL*/)
	: CDialog(CInvalidID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvalidID)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInvalidID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvalidID)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvalidID, CDialog)
	//{{AFX_MSG_MAP(CInvalidID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInvalidID message handlers

void CInvalidID::OnOK() 
{
//	CString url = "http://www.pru.co.kr/hp/hts.jsp?twoDepthNo=1407&threeDepthNo=1482&setNo=1482";
//	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
	
	CDialog::OnOK();
}

void CInvalidID::OnCancel() 
{

	
	CDialog::OnCancel();
}
