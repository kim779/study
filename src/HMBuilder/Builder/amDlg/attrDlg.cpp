// attrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "attrDlg.h"
#include "afxdialogex.h"
#include "colorDlg.h"

#include "../h/axisvar.h"
#include "../amLib/amlib.h"
#include "../h/mainvar.h"
#include "../h/form_w.h"

#define FA_MASK	(FA_COMMA|FA_PASSWD|FA_ZEROSUP|FA_PROTECT|FA_WRAP|FA_SKIP|FA_CORGB|FA_DOMINO|FA_UPPER|FA_NOR|FA_FLOAT|FA_FLOATx|FA_PASS)

// CAttrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAttrDlg, CDialogEx)

CAttrDlg::CAttrDlg(CWnd* pParent, BYTE kind, BYTE type, BYTE iok, DWORD attr, DWORD attr2, int upRGB, int dnRGB)
	: CDialogEx(CAttrDlg::IDD, pParent)
{
	m_kind  = kind;
	m_type  = type;
	m_iok   = iok;
	m_attr  = attr;
	m_attr2 = attr2;

	m_upRGB  = upRGB;
	m_dnRGB  = dnRGB;

	m_at01 = FALSE;		
	m_at02 = FALSE;
	m_at03 = FALSE;
	m_at04 = FALSE;
	m_at05 = FALSE;
	m_at06 = FALSE;
	m_at08 = FALSE;
	m_at09 = FALSE;
	m_at10 = FALSE;
	m_at12 = FALSE;
	m_at13 = FALSE;
	m_at14 = FALSE;
}

CAttrDlg::~CAttrDlg()
{
}

void CAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_OPT10, m_at01);			
	DDX_Check(pDX, IDC_OPT11, m_at02);
	DDX_Check(pDX, IDC_OPT12, m_at03);
	DDX_Check(pDX, IDC_OPT13, m_at04);
	DDX_Check(pDX, IDC_OPT14, m_at05);
	DDX_Check(pDX, IDC_OPT15, m_at06);
	DDX_Check(pDX, IDC_OPT17, m_at08);
	DDX_Check(pDX, IDC_OPT18, m_at09);
	DDX_Check(pDX, IDC_OPT19, m_at10);
	DDX_Check(pDX, IDC_OPT21, m_at12);
	DDX_Check(pDX, IDC_OPT22, m_at13);
	DDX_Check(pDX, IDC_OPT23, m_at14);
	DDX_Control(pDX, IDC_CORGB, m_corgb);
	DDX_Control(pDX, IDC_CORGB2, m_corgb2);

	DDX_Text(pDX, IDC_MINUSE, m_downE);
	DDV_MaxChars(pDX, m_downE, 3);
	DDX_Text(pDX, IDC_PLUSE, m_upE);
	DDV_MaxChars(pDX, m_upE, 3);
}


BEGIN_MESSAGE_MAP(CAttrDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OPT12, &CAttrDlg::OnCheckZerosup)
	ON_COMMAND_RANGE(IDC_OPT01, IDC_OPT03, &CAttrDlg::OnZeroSup)
	ON_BN_CLICKED(IDC_PLUSB, &CAttrDlg::OnUp)
	ON_BN_CLICKED(IDC_MINUSB, &CAttrDlg::OnDown)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAttrDlg 메시지 처리기입니다.


BOOL CAttrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CORGB)->EnableWindow(FALSE);			// CORGB
	GetDlgItem(IDC_CORGB2)->EnableWindow(FALSE);			// CORGB2

	GetDlgItem(IDC_PLUSB)->EnableWindow(FALSE);			// PLUSBTN
	GetDlgItem(IDC_PLUSE)->EnableWindow(FALSE);			// PLUSEDIT
	GetDlgItem(IDC_PLUSS)->EnableWindow(FALSE);			// PLUSSPIN

	GetDlgItem(IDC_MINUSB)->EnableWindow(FALSE);			// MINUSBTN
	GetDlgItem(IDC_MINUSE)->EnableWindow(FALSE);			// MINUSEDIT
	GetDlgItem(IDC_MINUSS)->EnableWindow(FALSE);			// MINUSSPIN

	GetDlgItem(IDC_OPT10)->EnableWindow(TRUE);			// COMMA

	GetDlgItem(IDC_OPT19)->EnableWindow(FALSE);			// NOR
	switch (m_kind)
	{
	case FM_EDIT:
		//if (m_iok == EIO_INPUT)
		//	GetDlgItem(IDC_OPT17)->EnableWindow(FALSE);	// DOMINO

		GetDlgItem(IDC_OPT19)->EnableWindow(TRUE);		// NOR
		GetDlgItem(IDC_OPT21)->EnableWindow(TRUE);		// FLOAT
		break;

	case FM_COMBO:
		GetDlgItem(IDC_OPT10)->EnableWindow(FALSE);		// COMMA
		GetDlgItem(IDC_OPT11)->EnableWindow(FALSE);		// PASSWD
		GetDlgItem(IDC_OPT12)->EnableWindow(FALSE);		// ZEROSUP
		GetDlgItem(IDC_OPT13)->EnableWindow(FALSE);		// PROTECT
		GetDlgItem(IDC_OPT17)->EnableWindow(FALSE);		// DOMINO
		GetDlgItem(IDC_OPT18)->EnableWindow(FALSE);		// UPPER
		GetDlgItem(IDC_CORGB)->EnableWindow(FALSE);		// CORGB
		GetDlgItem(IDC_OPT19)->EnableWindow(TRUE);		// NOR
		break;

	case FM_OUT:
		GetDlgItem(IDC_OPT13)->EnableWindow(FALSE);		// PROTECT
		GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);		// WRAP
		GetDlgItem(IDC_OPT18)->EnableWindow(FALSE);		// UPPER
		GetDlgItem(IDC_OPT15)->EnableWindow(FALSE);		// I/O SKIP
		//GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);		// FLOAT
		//GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);		// FLOATx
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);		// TAB PASS
		break;

	case FM_GRID:
	case FM_GRIDEX:
		switch (m_iok)
		{
		case EIO_INPUT:
			//GetDlgItem(IDC_OPT16)->EnableWindow(FALSE);	// CORGB
			GetDlgItem(IDC_OPT17)->EnableWindow(FALSE);	// DOMINO
		case EIO_INOUT:
			GetDlgItem(IDC_OPT19)->EnableWindow(TRUE);	// NOR
			break;
		default:
			break;
		}

		if (m_kind == FM_GRID)
			GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);	// WRAP

		break;

		GetDlgItem(IDC_OPT21)->EnableWindow(TRUE);		// FLOAT

	case FM_CONTROL:
		GetDlgItem(IDC_OPT10)->EnableWindow(FALSE);		// COMMA
		GetDlgItem(IDC_OPT11)->EnableWindow(FALSE);		// PASSWD
		GetDlgItem(IDC_OPT12)->EnableWindow(FALSE);		// ZEROSUP
		GetDlgItem(IDC_OPT13)->EnableWindow(FALSE);		// PROTECT
		GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);		// WRAP
		GetDlgItem(IDC_OPT15)->EnableWindow(FALSE);		// WRAP
		GetDlgItem(IDC_OPT17)->EnableWindow(FALSE);		// DOMINO
		GetDlgItem(IDC_OPT18)->EnableWindow(FALSE);		// UPPER
		GetDlgItem(IDC_OPT19)->EnableWindow(FALSE);		// NOR
		GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);		// FLOAT
		GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);		// FLOATx
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);		// WRAP
		break;
	}

	if (m_kind == FM_EDIT || m_kind == FM_OUT || m_kind == FM_GRID || m_kind == FM_GRIDEX || m_kind == FM_TABLE  || m_kind == FM_CONTROL)
	{
		GetDlgItem(IDC_CORGB)->EnableWindow(TRUE);		// CORGB
		GetDlgItem(IDC_CORGB2)->EnableWindow(TRUE);		// CORGB2

		GetDlgItem(IDC_PLUSB)->EnableWindow(TRUE);		// PLUSBTN
		GetDlgItem(IDC_PLUSE)->EnableWindow(TRUE);		// PLUSEDIT
		GetDlgItem(IDC_PLUSS)->EnableWindow(TRUE);		// PLUSSPIN

		GetDlgItem(IDC_MINUSB)->EnableWindow(TRUE);		// MINUSBTN
		GetDlgItem(IDC_MINUSE)->EnableWindow(TRUE);		// MINUSEDIT
		GetDlgItem(IDC_MINUSS)->EnableWindow(TRUE);		// MINUSSPIN
	}

	if (GetDlgItem(IDC_OPT10)->IsWindowEnabled())
	{
		if (m_attr & FA_COMMA)
			m_at01 = TRUE;
	}

	if (GetDlgItem(IDC_OPT11)->IsWindowEnabled())
	{
		if (m_attr & FA_PASSWD)
			m_at02 = TRUE;
	}

	if (GetDlgItem(IDC_OPT12)->IsWindowEnabled())
	{
		if (m_attr & FA_ZEROSUP)
		{
			m_at03 = TRUE;

			CButton* zero;
			switch (m_attr2 & SA_ZEROSUP)
			{
			case SA_SPACE:
				zero = (CButton *)GetDlgItem(IDC_OPT01);
				zero->SetCheck(1);	break;
			case SA_ASTER:
				zero = (CButton *)GetDlgItem(IDC_OPT02);
				zero->SetCheck(1);	break;
			case SA_ZERO:
				zero = (CButton *)GetDlgItem(IDC_OPT03);
				zero->SetCheck(1);	break;
			default:
				break;
			}
		}
		else
		{
			GetDlgItem(IDC_OPT01)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPT02)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPT03)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_OPT01)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPT02)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPT03)->EnableWindow(FALSE);
	}

	if (GetDlgItem(IDC_OPT13)->IsWindowEnabled())
	{
		if (m_attr & FA_PROTECT)
			m_at04 = TRUE;
	}

	if (GetDlgItem(IDC_OPT14)->IsWindowEnabled())
	{
		if (m_attr & FA_WRAP)
			m_at05 = TRUE;
	}

	if (GetDlgItem(IDC_OPT15)->IsWindowEnabled())
	{
		if (m_attr & FA_SKIP)
			m_at06 = TRUE;
	}

	if (GetDlgItem(IDC_OPT17)->IsWindowEnabled())
	{
		if (m_attr & FA_DOMINO)
			m_at08 = TRUE;
	}

	if (GetDlgItem(IDC_OPT18)->IsWindowEnabled())
	{
		if (m_attr & FA_UPPER)
			m_at09 = TRUE;
	}

	if (GetDlgItem(IDC_OPT19)->IsWindowEnabled())
	{
		if (m_attr & FA_NOR)
			m_at10 = TRUE;
	}

	if (GetDlgItem(IDC_OPT21)->IsWindowEnabled())
	{
		if (m_attr & FA_FLOAT)
			m_at12 = true;
	}

	if (GetDlgItem(IDC_OPT22)->IsWindowEnabled())
	{
		if (m_attr & FA_FLOATx)
			m_at13 = true;
	}

	if (GetDlgItem(IDC_OPT23)->IsWindowEnabled())
	{
		if (m_attr & FA_PASS)
			m_at14 = true;
	}

	//// combo 처리
	int comboindex = 0;
	int comboindex2 = 0;
	if (m_attr & FA_CORGB)
	{
		switch (m_attr2 & SA_CORGB)
		{
		case SA_CORAW:
			comboindex = 1;
			break;
		case SA_COSIGN:
			comboindex = 2;
			break;
		case SA_COSUP:
			comboindex = 3;
			break;
		case SA_TEXT:
			comboindex2 = 1;
			break;
		case SA_PAINT:
			comboindex2 = 2;
			break;
		case SA_TEXTSUP:	// add 2014. 01. 29. + supress
			comboindex2 = 3;
			break;
		case SA_PAINTSUP:	// add 2014. 01. 29. + Supress
			comboindex2 = 4;
			break;
		}
	}	

	m_corgb.SetCurSel(comboindex);
	m_corgb2.SetCurSel(comboindex2);	

	CEdit*	ed;
	CSpinButtonCtrl* spin;

	ed   = (CEdit *)GetDlgItem(IDC_PLUSE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_PLUSS);
	spin->SetBuddy(ed);
	spin->SetRange(0, 255);
	m_upE.Format(_T("%d"), m_upRGB);

	ed   = (CEdit *)GetDlgItem(IDC_MINUSE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_MINUSS);
	spin->SetBuddy(ed);
	spin->SetRange(0, 255);
	m_downE.Format(_T("%d"), m_dnRGB);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAttrDlg::OnCheckZerosup() 
{
	UpdateData(TRUE);

	if (m_at03)	// ZEROSUP
	{
		GetDlgItem(IDC_OPT01)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT02)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT03)->EnableWindow(TRUE);

		CButton* zero;
		switch (m_attr2 & SA_ZEROSUP)
		{
		case SA_SPACE:
			zero = (CButton *) GetDlgItem(IDC_OPT01);
			zero->SetCheck(1);	break;
		case SA_ASTER:
			zero = (CButton *) GetDlgItem(IDC_OPT02);
			zero->SetCheck(1);	break;
		case SA_ZERO:
			zero = (CButton *) GetDlgItem(IDC_OPT03);
			zero->SetCheck(1);	break;
		default:
			break;
		}
	}
	else
	{
		CButton* zero;
		zero = (CButton *) GetDlgItem(IDC_OPT01);
		zero->SetCheck(0);
		zero->EnableWindow(FALSE);

		zero = (CButton *) GetDlgItem(IDC_OPT02);
		zero->SetCheck(0);
		zero->EnableWindow(FALSE);

		zero = (CButton *) GetDlgItem(IDC_OPT03);
		zero->SetCheck(0);
		zero->EnableWindow(FALSE);

		m_attr2 &= ~SA_ZEROSUP;
	}
}

void CAttrDlg::OnZeroSup(UINT cmdID)
{
	CButton* check;

	m_attr2 &= ~SA_ZEROSUP;
	switch (cmdID)
	{
	case IDC_OPT01:
		m_attr2 |= SA_SPACE;
		check = (CButton *) GetDlgItem(IDC_OPT01);
		check->SetCheck(1);
		check = (CButton *) GetDlgItem(IDC_OPT02);
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT03);
		check->SetCheck(0);
		break;
	case IDC_OPT02:
		m_attr2 |= SA_ASTER;
		check = (CButton *) GetDlgItem(IDC_OPT01); 
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT02);
		check->SetCheck(1);
		check = (CButton *) GetDlgItem(IDC_OPT03);
		check->SetCheck(0);
		break;
	case IDC_OPT03:
		m_attr2 |= SA_ZERO;
		check = (CButton *) GetDlgItem(IDC_OPT01);
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT02);
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT03); 
		check->SetCheck(1);
		break;
	default:
		break;
	}
}

void CAttrDlg::OnOK() 
{
	UpdateData(TRUE);

	m_attr = (m_attr & ~FA_MASK);
	m_attr2 &= ~SA_CORGB;

	if (m_at01)	m_attr |= FA_COMMA;
	if (m_at02)	m_attr |= FA_PASSWD;
	if (m_at03)	m_attr |= FA_ZEROSUP;
	if (m_at04)	m_attr |= FA_PROTECT;
	if (m_at05)	m_attr |= FA_WRAP;
	if (m_at06)	m_attr |= FA_SKIP;
	if (m_at08)	m_attr |= FA_DOMINO;
	if (m_at09)	m_attr |= FA_UPPER;
	if (m_at10)	m_attr |= FA_NOR;
	if (m_at12)	m_attr |= FA_FLOAT;
	if (m_at13)	m_attr |= FA_FLOATx;
	if (m_at14)	m_attr |= FA_PASS;

	int combosel = m_corgb.GetCurSel();
	int combosel2 = m_corgb2.GetCurSel();

	switch (combosel)
	{
	case 0:
		break;
	case 1:
		m_attr |= FA_CORGB;	m_attr2 |= SA_CORAW;
		break;
	case 2:
		m_attr |= FA_CORGB;	m_attr2 |= SA_COSIGN;
		break;
	case 3:
		m_attr |= FA_CORGB;	m_attr2 |= SA_COSUP;
		break;
	}

	switch (combosel2)
	{
	case 0:
		break;
	case 1:
		m_attr |= FA_CORGB;	m_attr2 |= SA_TEXT;
		break;
	case 2:
		m_attr |= FA_CORGB;	m_attr2 |= SA_PAINT;
		break;
	case 3:		// Supress + Text add. 2014. 01. 29.
		m_attr |= FA_CORGB;	m_attr2 |= SA_TEXTSUP;
		break;
	case 4:		// Supress + Paint add. 2014. 01. 29.
		m_attr |= FA_CORGB;	m_attr2 |= SA_PAINTSUP;
		break;
	}

	m_upRGB = max(0, min(atoi(m_upE),   255));
	m_dnRGB = max(0, min(atoi(m_downE), 255));

	CDialog::OnOK();
}

BOOL CAttrDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == CBN_SELCHANGE)
	{
		int sel;
		switch (LOWORD(wParam))
		{
		case IDC_CORGB:
			sel = m_corgb.GetCurSel();
			if (sel > 0)
				m_corgb2.SetCurSel(0);
			break;
		case IDC_CORGB2:
			sel = m_corgb2.GetCurSel();
			if (sel > 0)
				m_corgb.SetCurSel(0);
			break;
		}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CAttrDlg::OnUp()
{
	CRect	rc;

	GetDlgItem(IDC_PLUSB)->GetWindowRect(&rc);
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


void CAttrDlg::OnDown()
{
	CRect	rc;

	GetDlgItem(IDC_MINUSB)->GetWindowRect(&rc);
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

HBRUSH CAttrDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	UpdateData();
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		if (GetDlgItem(IDC_PLUSE)->GetSafeHwnd() == pWnd->GetSafeHwnd())
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
		if (GetDlgItem(IDC_MINUSE)->GetSafeHwnd() == pWnd->GetSafeHwnd())
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
