// SetupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SetupDialog.h"
#include "imm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupDialog dialog


CSetupDialog::CSetupDialog(UINT idResource, CWnd* pParent /*=NULL*/)
	: CDialog(idResource, pParent)
{
	//{{AFX_DATA_INIT(CSetupDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_resourceID = idResource;
}


void CSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupDialog, CDialog)
	//{{AFX_MSG_MAP(CSetupDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupDialog message handlers

void CSetupDialog::DefaultSetup()
{
}

void CSetupDialog::ApplySetup()
{
}
void CSetupDialog::SearchGroupList()
{
}

void CSetupDialog::ChangeHangulMode(HWND hWnd)
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
}

void CSetupDialog::ChangeEnglishMode(HWND hWnd)
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
		ImmSetConversionStatus(hImc, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
		ImmReleaseContext(hWnd, hImc);
	}
}

BOOL CSetupDialog::OnEraseBkgnd(CDC* pDC) 
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

HBRUSH CSetupDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

BOOL CSetupDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//m_changer = new CControlChanger(this);
	m_changer = std::make_unique< CControlChanger>(this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupDialog::OnDestroy() 
{
	//delete m_changer;
	CDialog::OnDestroy();
		
}
