// coRGB.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "coRGB.h"
#include "afxdialogex.h"
#include "colorDlg.h"

#include "../../h/axisvar.h"
#include "../../dll/lib/axislib.h"
#include "../h/mainvar.h"
//#include "../h/form_w.h"
#include "../h/formOb.h"

// CCoRGB 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCoRGB, CDialogEx)

CCoRGB::CCoRGB(CWnd* pParent, CString name, BYTE whatEdit, int upRGB, int dnRGB)
	: CDialogEx(CCoRGB::IDD, pParent)
{
	m_downE = _T("");
	m_upE   = _T("");
	m_name  = _T("");

	m_whatEdit = whatEdit;
	m_upRGB  = upRGB;
	m_dnRGB  = dnRGB;
	m_name   = name;
}

CCoRGB::~CCoRGB()
{
}

void CCoRGB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_DOWNE, m_downE);
	DDV_MaxChars(pDX, m_downE, 3);
	DDX_Text(pDX, IDC_UPE, m_upE);
	DDV_MaxChars(pDX, m_upE, 3);
	DDX_Text(pDX, IDC_SYMBOL, m_name);
}


BEGIN_MESSAGE_MAP(CCoRGB, CDialogEx)
	ON_BN_CLICKED(IDC_UP, &CCoRGB::OnUp)
	ON_BN_CLICKED(IDC_DOWN, &CCoRGB::OnDown)
	ON_BN_CLICKED(IDC_NOEDIT, &CCoRGB::OnNoEdit)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCoRGB 메시지 처리기입니다.


BOOL CCoRGB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CEdit*	ed;
	CSpinButtonCtrl* spin;

	ed   = (CEdit *)GetDlgItem(IDC_UPE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_UPS);
	spin->SetBuddy(ed);
	spin->SetRange(0, 255);
	m_upE.Format(_T("%d"), m_upRGB);

	ed   = (CEdit *)GetDlgItem(IDC_DOWNE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_DOWNS);
	spin->SetBuddy(ed);
	spin->SetRange(0, 255);
	m_downE.Format(_T("%d"), m_dnRGB);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCoRGB::OnOK()
{
	UpdateData(TRUE);

	m_whatEdit = weCORGB;
	m_upRGB = max(0, min(atoi(m_upE),   255));
	m_dnRGB = max(0, min(atoi(m_downE), 255));

	CDialogEx::OnOK();
}


void CCoRGB::OnUp()
{
	CRect	rc;

	GetDlgItem(IDC_UP)->GetWindowRect(&rc);
	UpdateData(TRUE);

	m_upRGB = atoi(m_upE);
	CColorDlg* clDlg = new CColorDlg(this, m_upRGB, false, rc);
	if (clDlg->DoModal() == IDOK)
	{
		m_upRGB = clDlg->m_color;
		m_upE.Format(_T("%d"), m_upRGB);
		UpdateData(FALSE);
	}
	delete clDlg;
}


void CCoRGB::OnDown()
{
	CRect	rc;

	GetDlgItem(IDC_DOWN)->GetWindowRect(&rc);
	UpdateData(TRUE);

	m_dnRGB = atoi(m_downE);
	CColorDlg* clDlg = new CColorDlg(this, m_dnRGB, false, rc);
	if (clDlg->DoModal() == IDOK)
	{
		m_dnRGB = clDlg->m_color;
		m_downE.Format(_T("%d"), m_dnRGB);
		UpdateData(FALSE);
	}
	delete clDlg;
}


void CCoRGB::OnNoEdit()
{
	m_whatEdit = weNOP;
	m_upRGB = 0;
	m_dnRGB = 0;

	CDialogEx::OnOK();
}


HBRUSH CCoRGB::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

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

	CDialogEx::OnDestroy();
}
