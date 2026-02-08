// DlgSaveCandle.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSaveCandle.h"
#include "MainFrmWnd.h"

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
	m_pWndMainFrm = pParent;
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
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
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
	else if(m_strCandleName.GetLength() > 8)
	{
		::MessageBox(m_hWnd, "패턴명이 긴 경우 지정한 이름대로 표시되지 않을 수 있습니다.", PROGRAM_TITLE, MB_OK);
	}
	
	CDialog::OnOK();
}

int CDlgSaveCandle::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
	
	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDOK);
	pMainFrmWnd->SetBtnImg(&m_btnOk, 4, FALSE);
	
	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDCANCEL);
	pMainFrmWnd->SetBtnImg(&m_btnCancel, 4, FALSE);
	
	return 0;
}

void CDlgSaveCandle::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, pMainFrmWnd->GetAxColor(66));
}

void CDlgSaveCandle::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect reClient, reTemp;
	GetClientRect(reClient);
	
	reTemp = reClient;
	
	reTemp.top = reClient.bottom - 25;
	reTemp.bottom = reTemp.top + 20;
	
	reTemp.right = reClient.right - 5;
	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnOk.MoveWindow(&reTemp);
	
	Invalidate();
}

HBRUSH CDlgSaveCandle::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pWndMainFrm;
		
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(pMainFrmWnd->m_clrMainSkin);
		hbr = pMainFrmWnd->GetAxBrush(pMainFrmWnd->GetAxColor(66));
	}
	
	return hbr;
}
