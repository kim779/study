#include "stdafx.h"
//#include "axissm.h"
#include "RenameDlg.h"
#include "imm.h"

#pragma comment(lib, "imm32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRenameDlg::CRenameDlg(CWnd* pParent /*=NULL*/, CString strName)
	: CDialog(CRenameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRenameDlg)
	m_sName = _T("");
	//}}AFX_DATA_INIT

	m_point.x = -1;
	m_point.y = -1;
}

void CRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenameDlg)
	DDX_Text(pDX, IDC_TEXT, m_sName);
	DDV_MaxChars(pDX, m_sName, 8);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRenameDlg, CDialog)
	//{{AFX_MSG_MAP(CRenameDlg)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void CRenameDlg::OnOK() 
{
	UpdateData();
	CDialog::OnOK();
}

BOOL CRenameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_point.x != -1 && m_point.y != -1)
		SetWindowPos(NULL, m_point.x, m_point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	
	HIMC himc{};
	DWORD dwConversion{}, dwSentence{};
	himc = ImmGetContext(GetSafeHwnd());
	if (himc)
	{
		ImmSetOpenStatus( himc, TRUE );
		ImmGetConversionStatus(himc, &dwConversion, &dwSentence);
		
		dwConversion |= IME_CMODE_NATIVE ;
		
		ImmSetConversionStatus(himc, dwConversion, dwSentence );
		
		ImmReleaseContext(GetSafeHwnd(), himc);
	}
	

	return TRUE;
}

BOOL CRenameDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);
		
		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);
		
		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CRenameDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);
	
	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}
