// StopWarn.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "StopWarn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStopWarn dialog


CStopWarn::CStopWarn(CWnd* pParent /*=NULL*/)
	: CDialog(CStopWarn::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStopWarn)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStopWarn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStopWarn)
	DDX_Control(pDX, IDC_WARNMSG, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStopWarn, CDialog)
	//{{AFX_MSG_MAP(CStopWarn)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStopWarn message handlers

BOOL CStopWarn::OnInitDialog() 
{
	CDialog::OnInitDialog();
	loadMsg();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CStopWarn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const int nID = pWnd->GetDlgCtrlID();
	switch (nID)
	{
	case IDC_RED1:	pDC->SetTextColor(RGB(255, 0, 0));	break;
	}
	pDC->SetBkMode(TRANSPARENT);
	return hbr;
}

void CStopWarn::loadMsg()
{
	CString	file;
	CFile	fileH;

	file.Format("%s\\tab\\stoplossmsg.txt", Axis::home);

	if (fileH.Open(file, CFile::modeRead))
	{
		const DWORD	len = (DWORD)fileH.GetLength();
		//char*	wb = new char [len +1];
		std::unique_ptr<char[]>wb = std::make_unique<char[]>(len + 1);
		
		ZeroMemory(wb.get(), len+1);
		fileH.Read(wb.get(), len);
		m_edit.SetWindowText(wb.get());
		//delete [] wb;
		fileH.Close();
	}
}