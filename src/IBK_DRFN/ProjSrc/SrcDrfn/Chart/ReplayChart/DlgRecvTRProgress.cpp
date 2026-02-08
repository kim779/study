// DlgRecvTRProgress.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "DlgRecvTRProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRecvTRProgress dialog


CDlgRecvTRProgress::CDlgRecvTRProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRecvTRProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRecvTRProgress)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nTRTotal = 0;
//	m_progRecvTR.SetRange( 0, 100 );

}


void CDlgRecvTRProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRecvTRProgress)
	DDX_Control(pDX, IDC_PROGRESS_RECV_TR, m_progRecvTR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRecvTRProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgRecvTRProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRecvTRProgress message handlers

//BOOL CDlgRecvTRProgress::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
BOOL CDlgRecvTRProgress::Create( CWnd* pParentWnd ) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CDlgRecvTRProgress::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;

	CDialog::PostNcDestroy();
}

void CDlgRecvTRProgress::AddCount(int nGap)
{
	m_nTRCur += nGap;
	int nPer = ( m_nTRCur * 100 ) / m_nTRTotal;
	m_progRecvTR.SetPos( nPer );

}

void CDlgRecvTRProgress::SetTRTotalCount( int nTRTotal )
{
	m_nTRTotal = nTRTotal;
	m_nTRCur = 0;
}