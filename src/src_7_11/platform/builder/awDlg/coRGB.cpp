// coRGB.cpp : implementation file
//

#include "stdafx.h"
#include "coRGB.h"
#include "colordlg.h"

#include "../../h/axisvar.h"
#include "../../dll/lib/axislib.h"
#include "../mainvar.h"

#include "../form_w.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoRGB dialog

CCoRGB::CCoRGB(CWnd* pParent, CString name, BYTE isEdit, int upRGB, int dnRGB)
	: CDialog(CCoRGB::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCoRGB)
	m_downE = _T("");
	m_upE   = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT

	m_isEdit = isEdit;
	m_upRGB  = upRGB;
	m_dnRGB  = dnRGB;
	m_name   = name;
}


void CCoRGB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCoRGB)
	DDX_Text(pDX, IDC_DOWNE, m_downE);
	DDV_MaxChars(pDX, m_downE, 3);
	DDX_Text(pDX, IDC_UPE, m_upE);
	DDV_MaxChars(pDX, m_upE, 3);
	DDX_Text(pDX, IDC_SYMBOL, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCoRGB, CDialog)
	//{{AFX_MSG_MAP(CCoRGB)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_NOEDIT, OnNoEdit)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoRGB message handlers

BOOL CCoRGB::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit*	ed;
	CSpinButtonCtrl* spin;

	ed   = (CEdit *) GetDlgItem(IDC_UPE);
	spin = (CSpinButtonCtrl *) GetDlgItem(IDC_UPS);
	spin->SetBuddy(ed);
	spin->SetRange(0, 255);
	m_upE.Format("%d", m_upRGB);

	ed   = (CEdit *) GetDlgItem(IDC_DOWNE);
	spin = (CSpinButtonCtrl *) GetDlgItem(IDC_DOWNS);
	spin->SetBuddy(ed);
	spin->SetRange(0, 255);
	m_downE.Format("%d", m_dnRGB);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCoRGB::OnUp() 
{
	CRect voidRc;
	GetDlgItem(IDC_UP)->GetWindowRect(&voidRc);
	UpdateData(TRUE);

	m_upRGB = atoi(m_upE);
	CColorDlg* clDlg = new CColorDlg(this, m_upRGB, false, voidRc);
	if (clDlg->DoModal() == IDOK)
	{
		m_upRGB = clDlg->m_color;
		m_upE.Format("%d", m_upRGB);
		UpdateData(FALSE);
	}
	delete clDlg;
}

void CCoRGB::OnDown() 
{
	CRect voidRc;
	GetDlgItem(IDC_DOWN)->GetWindowRect(&voidRc);
	UpdateData(TRUE);

	m_dnRGB = atoi(m_downE);
	CColorDlg* clDlg = new CColorDlg(this, m_dnRGB, false, voidRc);
	if (clDlg->DoModal() == IDOK)
	{
		m_dnRGB = clDlg->m_color;
		m_downE.Format("%d", m_dnRGB);
		UpdateData(FALSE);
	}
	delete clDlg;
}

void CCoRGB::OnNoEdit() 
{
	m_isEdit = IE_NOP;
	m_upRGB = 0;
	m_dnRGB = 0;

	CDialog::OnOK();
}

void CCoRGB::OnOK() 
{
	UpdateData(TRUE);

	m_isEdit = IE_CORGB;
	m_upRGB = max(0, min(atoi(m_upE),   255));
	m_dnRGB = max(0, min(atoi(m_downE), 255));

	CDialog::OnOK();
}

HBRUSH CCoRGB::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	UpdateData();
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		if (GetDlgItem(IDC_UPE)->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{		
			m_ctlClrUp = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, max(0, min(atoi(m_upE),   255)));
			m_ctlBrushUp.DeleteObject();
			m_ctlBrushUp.CreateSolidBrush(m_ctlClrUp);
			pDC->SetBkColor(m_ctlClrUp);
			if ((GetRValue(m_ctlClrUp) + GetGValue(m_ctlClrUp) + GetBValue(m_ctlClrUp)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
			hbr = (HBRUSH) m_ctlBrushUp.operator HBRUSH();
		}
		else
		{
			m_ctlClrDn = (COLORREF)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCOLOR, max(0, min(atoi(m_downE), 255)));
			m_ctlBrushDn.DeleteObject();
			m_ctlBrushDn.CreateSolidBrush(m_ctlClrDn);
			pDC->SetBkColor(m_ctlClrDn);
			if ((GetRValue(m_ctlClrDn) + GetGValue(m_ctlClrDn) + GetBValue(m_ctlClrDn)) / 3 >= 192)
				pDC->SetTextColor(RGB(0, 0, 0));
			else
				pDC->SetTextColor(RGB(255, 255, 255));
			hbr = (HBRUSH) m_ctlBrushDn.operator HBRUSH();
		}
		break;
	}
	return hbr;
}

void CCoRGB::OnDestroy() 
{
	m_ctlBrushDn.DeleteObject();
	m_ctlBrushUp.DeleteObject();
	CDialog::OnDestroy();	
}
