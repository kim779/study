// DlgSaveCandle.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSaveCandle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveCandle dialog


CDlgSaveCandle::CDlgSaveCandle(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveCandle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSaveCandle)
	m_strCandleName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSaveCandle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSaveCandle)
	DDX_Text(pDX, IDC_ED_CANDLENAME, m_strCandleName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSaveCandle, CDialog)
	//{{AFX_MSG_MAP(CDlgSaveCandle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveCandle message handlers

void CDlgSaveCandle::OnOK() 
{
	UpdateData();

	m_strCandleName.TrimLeft(); m_strCandleName.TrimRight();

	if(m_strCandleName.IsEmpty())
	{
		::MessageBox(m_hWnd, "캔들패턴의 이름을 지정하세요", PROGRAM_TITLE, MB_OK);
		return;
	}
	
	CDialog::OnOK();
}
