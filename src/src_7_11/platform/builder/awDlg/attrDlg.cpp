// attrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "attrDlg.h"

#include "../form_w.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FA_MASK	(FA_COMMA|FA_RIGHT|FA_PASSWD|FA_ZEROSUP|FA_SEND|FA_PROTECT|FA_WRAP|FA_MAND|FA_HAN|FA_SKIP|FA_CORGB|FA_DOMINO|FA_REF|FA_FLASH|FA_UPPER|FA_DRAG|FA_NOR|FA_CONTRAST|FA_VALIDX)

/////////////////////////////////////////////////////////////////////////////
// CAttrDlg dialog

CAttrDlg::CAttrDlg(CWnd* pParent, BYTE kind, BYTE type, BYTE iok, DWORD attr, DWORD attr2)
	: CDialog(CAttrDlg::IDD, pParent)
{
	m_kind  = kind;
	m_type  = type;
	m_iok   = iok;
	m_attr  = attr;
	m_attr2 = attr2;

	//{{AFX_DATA_INIT(CAttrDlg)
	m_at01 = FALSE;
	m_at02 = FALSE;
	m_at03 = FALSE;
	m_at04 = FALSE;
	m_at05 = FALSE;
	m_at06 = FALSE;
	m_at07 = FALSE;
	m_at08 = FALSE;
	m_at09 = FALSE;
	m_at10 = FALSE;
	m_at11 = FALSE;
	m_at12 = FALSE;
	m_at13 = FALSE;
	//m_at14 = FALSE;
	m_at15 = FALSE;
	m_at16 = FALSE;
	m_at17 = FALSE;
	m_at18 = FALSE;
	m_at19 = FALSE;
	m_at20 = FALSE;
	//}}AFX_DATA_INIT
}


void CAttrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAttrDlg)
	DDX_Check(pDX, IDC_OPT10, m_at01);
	DDX_Check(pDX, IDC_OPT11, m_at02);
	DDX_Check(pDX, IDC_OPT12, m_at03);
	DDX_Check(pDX, IDC_OPT13, m_at04);
	DDX_Check(pDX, IDC_OPT14, m_at05);
	DDX_Check(pDX, IDC_OPT15, m_at06);
	DDX_Check(pDX, IDC_OPT16, m_at07);
	DDX_Check(pDX, IDC_OPT17, m_at08);
	DDX_Check(pDX, IDC_OPT18, m_at09);
	DDX_Check(pDX, IDC_OPT19, m_at10);
	DDX_Check(pDX, IDC_OPT20, m_at11);
	DDX_Check(pDX, IDC_OPT21, m_at12);
	DDX_Check(pDX, IDC_OPT22, m_at13);
	//DDX_Check(pDX, IDC_OPT23, m_at14);
	DDX_Check(pDX, IDC_OPT24, m_at15);
	DDX_Check(pDX, IDC_OPT25, m_at16);
	DDX_Check(pDX, IDC_OPT26, m_at17);
	DDX_Check(pDX, IDC_OPT27, m_at18);
	DDX_Check(pDX, IDC_OPT28, m_at19);
	DDX_Check(pDX, IDC_OPT29, m_at20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAttrDlg, CDialog)
	//{{AFX_MSG_MAP(CAttrDlg)
	ON_BN_CLICKED(IDC_OPT13, OnCheckZerosup)
	ON_BN_CLICKED(IDC_OPT20, OnCheckCoRgb)
	ON_BN_CLICKED(IDC_OPT14, OnCheckSend)
	ON_BN_CLICKED(IDC_OPT10, OnCheckComma)
	ON_BN_CLICKED(IDC_OPT11, OnCheckRight)
	ON_BN_CLICKED(IDC_OPT28, OnCheckContrast)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_OPT01, IDC_OPT03, OnZeroSup)
	ON_COMMAND_RANGE(IDC_OPT04, IDC_OPT06, OnCoRgb)
	ON_COMMAND_RANGE(IDC_OPT07, IDC_OPT08, OnSend)
	ON_COMMAND_RANGE(IDC_OPT31, IDC_OPT32, OnContrast)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAttrDlg message handlers

BOOL CAttrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_type != IO_NUM)
	{
		GetDlgItem(IDC_OPT10)->EnableWindow(FALSE);	// COMMA
		GetDlgItem(IDC_OPT11)->EnableWindow(FALSE);	// RIGHT
	}

	GetDlgItem(IDC_OPT27)->EnableWindow(FALSE);		// NOR
	GetDlgItem(IDC_OPT28)->EnableWindow(FALSE);		// CONTRAST
	GetDlgItem(IDC_OPT29)->EnableWindow(FALSE);		// isValid onTR
	switch (m_kind)
	{
	case FM_EDIT:
		if (m_iok == EIO_INPUT)
		{
			GetDlgItem(IDC_OPT20)->EnableWindow(FALSE);	// CORGB
			GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);	// DOMINO
			GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);	// REFERENCE
		}
		GetDlgItem(IDC_OPT27)->EnableWindow(TRUE);		// NOR
		GetDlgItem(IDC_OPT29)->EnableWindow(TRUE);		// isValid onTR
		break;

	case FM_COMBO:
		GetDlgItem(IDC_OPT10)->EnableWindow(FALSE);	// COMMA
		//GetDlgItem(IDC_OPT11)->EnableWindow(FALSE);	// RIGHT
		GetDlgItem(IDC_OPT12)->EnableWindow(FALSE);	// PASSWD
		GetDlgItem(IDC_OPT13)->EnableWindow(FALSE);	// ZEROSUP
		GetDlgItem(IDC_OPT15)->EnableWindow(FALSE);	// PROTECT
		GetDlgItem(IDC_OPT18)->EnableWindow(FALSE);	// HAN
		GetDlgItem(IDC_OPT20)->EnableWindow(FALSE);	// CORGB
		GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);	// DOMINO
		GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);	// REFERENCE
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);	// DATE
		GetDlgItem(IDC_OPT25)->EnableWindow(FALSE);	// UPPER
		GetDlgItem(IDC_OPT27)->EnableWindow(TRUE);	// NOR
		break;

	case FM_TABLE:
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);	// DATE
	case FM_OUT:
		GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);	// SEND
		GetDlgItem(IDC_OPT15)->EnableWindow(FALSE);	// PROTECT
		GetDlgItem(IDC_OPT16)->EnableWindow(FALSE);	// WRAP
		GetDlgItem(IDC_OPT17)->EnableWindow(FALSE);	// MUST
		GetDlgItem(IDC_OPT18)->EnableWindow(FALSE);	// HAN
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);	// DATE
		GetDlgItem(IDC_OPT25)->EnableWindow(FALSE);	// UPPER
		GetDlgItem(IDC_OPT28)->EnableWindow(TRUE);	// CONTRAST
		break;

	case FM_GRID:
		switch (m_iok)
		{
		case EIO_INPUT:
			GetDlgItem(IDC_OPT20)->EnableWindow(FALSE);	// CORGB
			GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);	// DOMINO
			GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);	// REFERENCE
		case EIO_INOUT:
			GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);	// SEND
			GetDlgItem(IDC_OPT23)->EnableWindow(TRUE);	// DATE
			GetDlgItem(IDC_OPT27)->EnableWindow(TRUE);	// NOR
			break;
		default:
			GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);	// SEND
			GetDlgItem(IDC_OPT28)->EnableWindow(TRUE);	// CONTRAST
			break;
		}
		//GetDlgItem(IDC_OPT19)->EnableWindow(FALSE);	// SKIP
		break;
	}

	if (GetDlgItem(IDC_OPT10)->IsWindowEnabled())
	{
		if (m_attr & FA_COMMA)
			m_at01 = TRUE;
	}

	if (GetDlgItem(IDC_OPT11)->IsWindowEnabled())
	{
		if (m_attr & FA_RIGHT)
			m_at02 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT12)->IsWindowEnabled())
	{
		if (m_attr & FA_PASSWD)
			m_at03 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT13)->IsWindowEnabled())
	{
		if (m_attr & FA_ZEROSUP)
		{
			m_at04 = TRUE;

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
	
	if (GetDlgItem(IDC_OPT14)->IsWindowEnabled())
	{
		if (m_attr & FA_SEND)
		{
			m_at05 = TRUE;

			CButton* send;
			switch (m_attr2 & SA_SEND)
			{
			case SA_ALL:
				send = (CButton *) GetDlgItem(IDC_OPT07);
				send->SetCheck(1);	break;
			case SA_THIS:
				send = (CButton *) GetDlgItem(IDC_OPT08);
				send->SetCheck(1);	break;
			default:
				break;
			}
		}
		else
		{
			GetDlgItem(IDC_OPT07)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPT08)->EnableWindow(FALSE);
		}

	}
	else
	{
		GetDlgItem(IDC_OPT07)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPT08)->EnableWindow(FALSE);
	}
	
	if (GetDlgItem(IDC_OPT15)->IsWindowEnabled())
	{
		if (m_attr & FA_PROTECT)
			m_at06 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT16)->IsWindowEnabled())
	{
		if (m_attr & FA_WRAP)
			m_at07 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT17)->IsWindowEnabled())
	{
		if (m_attr & FA_MAND)
			m_at08 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT18)->IsWindowEnabled())
	{
		if (m_attr & FA_HAN)
			m_at09 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT19)->IsWindowEnabled())
	{
		if (m_attr & FA_SKIP)
			m_at10 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT20)->IsWindowEnabled())
	{
		if (m_attr & FA_CORGB)
		{
			m_at11 = TRUE;

			CButton* corgb;
			switch (m_attr2 & SA_CORGB)
			{
			case SA_CORAW:
				corgb = (CButton *) GetDlgItem(IDC_OPT04);
				corgb->SetCheck(1);	break;
			case SA_COSUP:
				corgb = (CButton *) GetDlgItem(IDC_OPT05);
				corgb->SetCheck(1);	break;
			case SA_COSIGN:
				corgb = (CButton *) GetDlgItem(IDC_OPT06);
				corgb->SetCheck(1);	break;
			default:
				break;
			}
		}
		else
		{
			GetDlgItem(IDC_OPT04)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPT05)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPT06)->EnableWindow(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_OPT04)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPT05)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPT06)->EnableWindow(FALSE);
	}
	
	if (GetDlgItem(IDC_OPT21)->IsWindowEnabled())
	{
		if (m_attr & FA_DOMINO)
			m_at12 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT22)->IsWindowEnabled())
	{
		if (m_attr & FA_REF)
			m_at13 = TRUE;
	}
	
	/*if (GetDlgItem(IDC_OPT23)->IsWindowEnabled())
	{
		if (m_attr & FA_DATE)
			m_at14 = TRUE;
	}*/
	
	if (GetDlgItem(IDC_OPT24)->IsWindowEnabled())
	{
		if (m_attr & FA_FLASH)
			m_at15 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT25)->IsWindowEnabled())
	{
		if (m_attr & FA_UPPER)
			m_at16 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT26)->IsWindowEnabled())
	{
		if (m_attr & FA_DRAG)
			m_at17 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT27)->IsWindowEnabled())
	{
		if (m_attr & FA_NOR)
			m_at18 = TRUE;
	}
	
	if (GetDlgItem(IDC_OPT28)->IsWindowEnabled())
	{
		if (m_attr & FA_CONTRAST)
		{
			m_at19 = TRUE;

			CButton* send;
			switch (m_attr2 & SA_CONTRAST)
			{
			case SA_TEXT:
				send = (CButton *) GetDlgItem(IDC_OPT31);
				send->SetCheck(1);	break;
			case SA_PAINT:
				send = (CButton *) GetDlgItem(IDC_OPT32);
				send->SetCheck(1);	break;
			default:
				break;
			}
		}
		else
		{
			GetDlgItem(IDC_OPT31)->EnableWindow(FALSE);
			GetDlgItem(IDC_OPT32)->EnableWindow(FALSE);
		}

	}
	else
	{
		GetDlgItem(IDC_OPT31)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPT32)->EnableWindow(FALSE);
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAttrDlg::OnCheckZerosup() 
{
	UpdateData(TRUE);

	if (m_at04)	// ZEROSUP
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

void CAttrDlg::OnCheckCoRgb() 
{
	UpdateData(TRUE);

	if (m_at11)	// CORGB
	{
		GetDlgItem(IDC_OPT04)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT05)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT06)->EnableWindow(TRUE);

		CButton* corgb;
		switch (m_attr2 & SA_CORGB)
		{
		case SA_CORAW:
			corgb = (CButton *) GetDlgItem(IDC_OPT04);
			corgb->SetCheck(1);	break;
		case SA_COSUP:
			corgb = (CButton *) GetDlgItem(IDC_OPT05);
			corgb->SetCheck(1);	break;
		case SA_COSIGN:
			corgb = (CButton *) GetDlgItem(IDC_OPT06);
			corgb->SetCheck(1);	break;
		default:
			break;
		}
	}
	else
	{
		CButton* corgb;
		corgb = (CButton *) GetDlgItem(IDC_OPT04);
		corgb->SetCheck(0);
		corgb->EnableWindow(FALSE);

		corgb = (CButton *) GetDlgItem(IDC_OPT05);
		corgb->SetCheck(0);
		corgb->EnableWindow(FALSE);

		corgb = (CButton *) GetDlgItem(IDC_OPT06);
		corgb->SetCheck(0);
		corgb->EnableWindow(FALSE);
	}
}

void CAttrDlg::OnCheckSend() 
{
	UpdateData(TRUE);

	if (m_at05)	// SEND
	{
		GetDlgItem(IDC_OPT07)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT08)->EnableWindow(TRUE);

		CButton* send;
		switch (m_attr2 & SA_SEND)
		{
		case SA_ALL:
			send = (CButton *) GetDlgItem(IDC_OPT07);
			send->SetCheck(1);	break;
		case SA_THIS:
			send = (CButton *) GetDlgItem(IDC_OPT08);
			send->SetCheck(1);	break;
		default:
			break;
		}
	}
	else
	{
		CButton* send;
		send = (CButton *) GetDlgItem(IDC_OPT07);
		send->SetCheck(0);
		send->EnableWindow(FALSE);

		send = (CButton *) GetDlgItem(IDC_OPT08);
		send->SetCheck(0);
		send->EnableWindow(FALSE);
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

	switch (m_attr2 & SA_CORGB)
	{
	case SA_CORAW:
		check = (CButton *) GetDlgItem(IDC_OPT04);
		check->SetCheck(1);	break;
	case SA_COSUP:
		check = (CButton *) GetDlgItem(IDC_OPT05);
		check->SetCheck(1);	break;
	case SA_COSIGN:
		check = (CButton *) GetDlgItem(IDC_OPT06);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_SEND)
	{
	case SA_ALL:
		check = (CButton *) GetDlgItem(IDC_OPT07);
		check->SetCheck(1);	break;
	case SA_THIS:
		check = (CButton *) GetDlgItem(IDC_OPT08);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_CONTRAST)
	{
	case SA_TEXT:
		check = (CButton *) GetDlgItem(IDC_OPT31);
		check->SetCheck(1);	break;
	case SA_PAINT:
		check = (CButton *) GetDlgItem(IDC_OPT32);
		check->SetCheck(1);	break;
	default:
		break;
	}
}

void CAttrDlg::OnCoRgb(UINT cmdID)
{
	CButton* check;

	m_attr2 &= ~SA_CORGB;
	switch (cmdID)
	{
	case IDC_OPT04:
		m_attr2 |= SA_CORAW;
		check = (CButton *) GetDlgItem(IDC_OPT04);
		check->SetCheck(1);
		check = (CButton *) GetDlgItem(IDC_OPT05);
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT06);
		check->SetCheck(0);
		break;
	case IDC_OPT05:
		m_attr2 |= SA_COSUP;
		check = (CButton *) GetDlgItem(IDC_OPT04); 
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT05);
		check->SetCheck(1);
		check = (CButton *) GetDlgItem(IDC_OPT06);
		check->SetCheck(0);
		break;
	case IDC_OPT06:
		m_attr2 |= SA_COSIGN;
		check = (CButton *) GetDlgItem(IDC_OPT04); 
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT05);
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT06);
		check->SetCheck(1);
		break;

	default:
		break;
	}

	switch (m_attr2 & SA_ZEROSUP)
	{
	case SA_SPACE:
		check = (CButton *) GetDlgItem(IDC_OPT01);
		check->SetCheck(1);	break;
	case SA_ASTER:
		check = (CButton *) GetDlgItem(IDC_OPT02);
		check->SetCheck(1);	break;
	case SA_ZERO:
		check = (CButton *) GetDlgItem(IDC_OPT03);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_SEND)
	{
	case SA_ALL:
		check = (CButton *) GetDlgItem(IDC_OPT07);
		check->SetCheck(1);	break;
	case SA_THIS:
		check = (CButton *) GetDlgItem(IDC_OPT08);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_CONTRAST)
	{
	case SA_TEXT:
		check = (CButton *) GetDlgItem(IDC_OPT31);
		check->SetCheck(1);	break;
	case SA_PAINT:
		check = (CButton *) GetDlgItem(IDC_OPT32);
		check->SetCheck(1);	break;
	default:
		break;
	}
}

void CAttrDlg::OnCheckComma()
{
	UpdateData();
	if (m_at01) m_at02 = TRUE;
	UpdateData(FALSE);
}

void CAttrDlg::OnCheckRight()
{
	UpdateData();
	if (m_at01) m_at02 = TRUE;
	UpdateData(FALSE);
}

void CAttrDlg::OnSend(UINT cmdID)
{
	CButton* check;

	m_attr2 &= ~SA_SEND;
	switch (cmdID)
	{
	case IDC_OPT07:
		m_attr2 |= SA_ALL;
		check = (CButton *) GetDlgItem(IDC_OPT07);
		check->SetCheck(1);
		check = (CButton *) GetDlgItem(IDC_OPT08);
		check->SetCheck(0);
		break;
	case IDC_OPT08:
		m_attr2 |= SA_THIS;
		check = (CButton *) GetDlgItem(IDC_OPT07); 
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT08);
		check->SetCheck(1);
		break;
	default:
		break;
	}

	switch (m_attr2 & SA_ZEROSUP)
	{
	case SA_SPACE:
		check = (CButton *) GetDlgItem(IDC_OPT01);
		check->SetCheck(1);	break;
	case SA_ASTER:
		check = (CButton *) GetDlgItem(IDC_OPT02);
		check->SetCheck(1);	break;
	case SA_ZERO:
		check = (CButton *) GetDlgItem(IDC_OPT03);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_CORGB)
	{
	case SA_CORAW:
		check = (CButton *) GetDlgItem(IDC_OPT04);
		check->SetCheck(1);	break;
	case SA_COSUP:
		check = (CButton *) GetDlgItem(IDC_OPT05);
		check->SetCheck(1);	break;
	case SA_COSIGN:
		check = (CButton *) GetDlgItem(IDC_OPT06);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_CONTRAST)
	{
	case SA_TEXT:
		check = (CButton *) GetDlgItem(IDC_OPT31);
		check->SetCheck(1);	break;
	case SA_PAINT:
		check = (CButton *) GetDlgItem(IDC_OPT32);
		check->SetCheck(1);	break;
	default:
		break;
	}
}

void CAttrDlg::OnOK() 
{
	UpdateData(TRUE);

	m_attr = m_attr & ~FA_MASK;
	if (m_at01)	m_attr |= FA_COMMA;
	if (m_at02)	m_attr |= FA_RIGHT;
	if (m_at03)	m_attr |= FA_PASSWD;
	if (m_at04)	m_attr |= FA_ZEROSUP;
	if (m_at05)	m_attr |= FA_SEND;
	if (m_at06)	m_attr |= FA_PROTECT;
	if (m_at07)	m_attr |= FA_WRAP;
	if (m_at08)	m_attr |= FA_MAND;
	if (m_at09)	m_attr |= FA_HAN;
	if (m_at10)	m_attr |= FA_SKIP;
	if (m_at11)	m_attr |= FA_CORGB;
	if (m_at12)	m_attr |= FA_DOMINO;
	if (m_at13)	m_attr |= FA_REF;
	//if (m_at14)	m_attr |= FA_DATE;
	if (m_at15)	m_attr |= FA_FLASH;
	if (m_at16)	m_attr |= FA_UPPER;
	if (m_at17)	m_attr |= FA_DRAG;
	if (m_at18)	m_attr |= FA_NOR;
	if (m_at19)	m_attr |= FA_CONTRAST;
	if (m_at20)	m_attr |= FA_VALIDX;

	if (m_attr & FA_COMMA)
		m_attr |= FA_RIGHT;

	CDialog::OnOK();
}


void CAttrDlg::OnCheckContrast() 
{
	UpdateData();
	
	if (m_at19)	// CONTRAST
	{
		GetDlgItem(IDC_OPT31)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT32)->EnableWindow(TRUE);

		CButton* contrast;
		switch (m_attr2 & SA_CONTRAST)
		{
		case SA_TEXT:
			contrast = (CButton *) GetDlgItem(IDC_OPT31);
			contrast->SetCheck(1);	break;
		case SA_PAINT:
			contrast = (CButton *) GetDlgItem(IDC_OPT32);
			contrast->SetCheck(1);	break;
		default:
			break;
		}
	}
	else
	{
		CButton* contrast;
		contrast = (CButton *) GetDlgItem(IDC_OPT31);
		contrast->SetCheck(1);
		contrast->EnableWindow(FALSE);

		contrast = (CButton *) GetDlgItem(IDC_OPT32);
		contrast->SetCheck(0);
		contrast->EnableWindow(FALSE);
	}
}

void CAttrDlg::OnContrast(UINT cmdID)
{
	CButton* check;

	m_attr2 &= ~SA_CONTRAST;
	switch (cmdID)
	{
	case IDC_OPT31:
		m_attr2 |= SA_TEXT;
		check = (CButton *) GetDlgItem(IDC_OPT31);
		check->SetCheck(1);
		check = (CButton *) GetDlgItem(IDC_OPT32);
		check->SetCheck(0);
		break;
	case IDC_OPT32:
		m_attr2 |= SA_PAINT;
		check = (CButton *) GetDlgItem(IDC_OPT31); 
		check->SetCheck(0);
		check = (CButton *) GetDlgItem(IDC_OPT32);
		check->SetCheck(1);
		break;

	default:
		break;
	}

	switch (m_attr2 & SA_CORGB)
	{
	case SA_CORAW:
		check = (CButton *) GetDlgItem(IDC_OPT04);
		check->SetCheck(1);	break;
	case SA_COSUP:
		check = (CButton *) GetDlgItem(IDC_OPT05);
		check->SetCheck(1);	break;
	case SA_COSIGN:
		check = (CButton *) GetDlgItem(IDC_OPT06);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_ZEROSUP)
	{
	case SA_SPACE:
		check = (CButton *) GetDlgItem(IDC_OPT01);
		check->SetCheck(1);	break;
	case SA_ASTER:
		check = (CButton *) GetDlgItem(IDC_OPT02);
		check->SetCheck(1);	break;
	case SA_ZERO:
		check = (CButton *) GetDlgItem(IDC_OPT03);
		check->SetCheck(1);	break;
	default:
		break;
	}

	switch (m_attr2 & SA_SEND)
	{
	case SA_ALL:
		check = (CButton *) GetDlgItem(IDC_OPT07);
		check->SetCheck(1);	break;
	case SA_THIS:
		check = (CButton *) GetDlgItem(IDC_OPT08);
		check->SetCheck(1);	break;
	default:
		break;
	}
}
