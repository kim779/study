// attrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "attrDlg.h"
#include "afxdialogex.h"

#include "../h/formOb.h"
#include "../h/fmProperties.h"

#define FA_MASK	(faCOMMA|faRIGHT|faPASSWD|faZEROSUP|faSEND|faPROTECT|faWRAP|faMUST|faMULTI|faSKIP|faCORGB|faDOMINO|faREF|faFLASH|faUPPER|faDRAG|faNOR|faCONTRAST|faVALIDX)

// CAttrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAttrDlg, CDialogEx)

CAttrDlg::CAttrDlg(CWnd* pParent, BYTE kind, BYTE type, BYTE iok, DWORD attributes, DWORD auxAttributes)
	: CDialogEx(CAttrDlg::IDD, pParent)
{
	m_kind  = kind;
	m_type  = type;
	m_iok   = iok;
	m_attributes  = attributes;
	m_auxAttributes = auxAttributes;

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
//	m_at14 = FALSE;
	m_at15 = FALSE;
	m_at16 = FALSE;
	m_at17 = FALSE;
	m_at18 = FALSE;
	m_at19 = FALSE;
	m_at20 = FALSE;
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
	DDX_Check(pDX, IDC_OPT16, m_at07);
	DDX_Check(pDX, IDC_OPT17, m_at08);
	DDX_Check(pDX, IDC_OPT18, m_at09);
	DDX_Check(pDX, IDC_OPT19, m_at10);
	DDX_Check(pDX, IDC_OPT20, m_at11);
	DDX_Check(pDX, IDC_OPT21, m_at12);
	DDX_Check(pDX, IDC_OPT22, m_at13);
//	DDX_Check(pDX, IDC_OPT23, m_at14);
	DDX_Check(pDX, IDC_OPT24, m_at15);
	DDX_Check(pDX, IDC_OPT25, m_at16);
	DDX_Check(pDX, IDC_OPT26, m_at17);
	DDX_Check(pDX, IDC_OPT27, m_at18);
	DDX_Check(pDX, IDC_OPT28, m_at19);
	DDX_Check(pDX, IDC_OPT29, m_at20);
}


BEGIN_MESSAGE_MAP(CAttrDlg, CDialogEx)
	ON_BN_CLICKED(IDC_OPT10, &CAttrDlg::OnCheckComma)
	ON_BN_CLICKED(IDC_OPT11, &CAttrDlg::OnCheckRight)
	ON_BN_CLICKED(IDC_OPT13, &CAttrDlg::OnCheckZerosup)
	ON_BN_CLICKED(IDC_OPT14, &CAttrDlg::OnCheckSend)
	ON_BN_CLICKED(IDC_OPT20, &CAttrDlg::OnCheckCoRgb)
	ON_BN_CLICKED(IDC_OPT28, &CAttrDlg::OnCheckContrast)
	ON_COMMAND_RANGE(IDC_OPT01, IDC_OPT03, &CAttrDlg::OnZeroSup)
	ON_COMMAND_RANGE(IDC_OPT04, IDC_OPT06, &CAttrDlg::OnCoRgb)
	ON_COMMAND_RANGE(IDC_OPT07, IDC_OPT08, &CAttrDlg::OnSend)
	ON_COMMAND_RANGE(IDC_OPT31, IDC_OPT32, &CAttrDlg::OnContrast)
END_MESSAGE_MAP()


// CAttrDlg 메시지 처리기입니다.


BOOL CAttrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_type != ctNUMERIC)
	{
		GetDlgItem(IDC_OPT10)->EnableWindow(FALSE);	// COMMA
		GetDlgItem(IDC_OPT11)->EnableWindow(FALSE);	// RIGHT
	}

	GetDlgItem(IDC_OPT27)->EnableWindow(FALSE);		// NOR
	GetDlgItem(IDC_OPT28)->EnableWindow(FALSE);		// CONTRAST
	GetDlgItem(IDC_OPT29)->EnableWindow(FALSE);		// isValid onTR

	switch (m_kind)
	{
	case fmEDIT:
		if (m_iok == ioINPUT)
		{
			GetDlgItem(IDC_OPT20)->EnableWindow(FALSE);	// CORGB
			GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);	// DOMINO
			GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);	// REFERENCE
		}
		GetDlgItem(IDC_OPT27)->EnableWindow(TRUE);		// NOR
		GetDlgItem(IDC_OPT29)->EnableWindow(TRUE);		// isValid onTR
		break;

	case fmCOMBO:
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

	case fmTABLE:
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);	// DATE
	case fmOUTPUT:
		GetDlgItem(IDC_OPT14)->EnableWindow(FALSE);	// SEND
		GetDlgItem(IDC_OPT15)->EnableWindow(FALSE);	// PROTECT
		GetDlgItem(IDC_OPT16)->EnableWindow(FALSE);	// WRAP
		GetDlgItem(IDC_OPT17)->EnableWindow(FALSE);	// MUST
		GetDlgItem(IDC_OPT18)->EnableWindow(FALSE);	// HAN
		GetDlgItem(IDC_OPT23)->EnableWindow(FALSE);	// DATE
		GetDlgItem(IDC_OPT25)->EnableWindow(FALSE);	// UPPER
		GetDlgItem(IDC_OPT28)->EnableWindow(TRUE);	// CONTRAST
		break;

	case fmGRID:
	case fmGRIDEX:
		switch (m_iok)
		{
		case ioINPUT:
			GetDlgItem(IDC_OPT20)->EnableWindow(FALSE);	// CORGB
			GetDlgItem(IDC_OPT21)->EnableWindow(FALSE);	// DOMINO
			GetDlgItem(IDC_OPT22)->EnableWindow(FALSE);	// REFERENCE
		case ioBOTH:
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

	if (GetDlgItem(IDC_OPT10)->IsWindowEnabled() && (m_attributes & faCOMMA))
		m_at01 = TRUE;

	if (GetDlgItem(IDC_OPT11)->IsWindowEnabled() && (m_attributes & faRIGHT))
		m_at02 = TRUE;
	
	if (GetDlgItem(IDC_OPT12)->IsWindowEnabled() && (m_attributes & faPASSWD))
		m_at03 = TRUE;
	
	if (GetDlgItem(IDC_OPT13)->IsWindowEnabled())
	{
		if (m_attributes & faZEROSUP)
		{
			m_at04 = TRUE;
			switch (getTypeOfZeroSupress(m_auxAttributes))
			{
			case spSPACE:
				((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(1);
				break;
			case spASTER:
				((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(1);
				break;
			case spZERO:
				((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(1);
				break;
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
		if (m_attributes & faSEND)
		{
			m_at05 = TRUE;
			switch (getTypeOfSendMethod(m_auxAttributes))
			{
			case smALL:
				((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(1);
				break;
			case smTHIS:
				((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(1);
				break;
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
	
	if (GetDlgItem(IDC_OPT15)->IsWindowEnabled() && (m_attributes & faPROTECT))
		m_at06 = TRUE;
	
	if (GetDlgItem(IDC_OPT16)->IsWindowEnabled() && (m_attributes & faWRAP))
		m_at07 = TRUE;
	
	if (GetDlgItem(IDC_OPT17)->IsWindowEnabled() && (m_attributes & faMUST))
		m_at08 = TRUE;
	
	if (GetDlgItem(IDC_OPT18)->IsWindowEnabled() && (m_attributes & faMULTI))
		m_at09 = TRUE;
	
	if (GetDlgItem(IDC_OPT19)->IsWindowEnabled() && (m_attributes & faSKIP))
		m_at10 = TRUE;
	
	if (GetDlgItem(IDC_OPT20)->IsWindowEnabled())
	{
		if (m_attributes & faCORGB)
		{
			m_at11 = TRUE;
			switch (getTypeOfConditionColor(m_auxAttributes))
			{
			case ccRAW:
				((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
				break;
			case ccSUP:
				((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(1);
				break;
			case ccSIGN:
				((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(1);
				break;
			default:
				((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
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
	
	if (GetDlgItem(IDC_OPT21)->IsWindowEnabled() && (m_attributes & faDOMINO))
		m_at12 = TRUE;
	
	if (GetDlgItem(IDC_OPT22)->IsWindowEnabled() && (m_attributes & faREF))
		m_at13 = TRUE;
	
	//if (GetDlgItem(IDC_OPT23)->IsWindowEnabled() && (m_attributes & faDATE))
	//	m_at14 = TRUE;
	
	if (GetDlgItem(IDC_OPT24)->IsWindowEnabled() && (m_attributes & faFLASH))
		m_at15 = TRUE;
	
	if (GetDlgItem(IDC_OPT25)->IsWindowEnabled() && (m_attributes & faUPPER))
		m_at16 = TRUE;
	
	if (GetDlgItem(IDC_OPT26)->IsWindowEnabled() && (m_attributes & faDRAG))
		m_at17 = TRUE;
	
	if (GetDlgItem(IDC_OPT27)->IsWindowEnabled() && (m_attributes & faNOR))
		m_at18 = TRUE;
	
	if (GetDlgItem(IDC_OPT28)->IsWindowEnabled())
	{
		if (m_attributes & faCONTRAST)
		{
			m_at19 = TRUE;
			switch (getTypeOfConditionColor(m_auxAttributes))
			{
			case ccTEXT:
				((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
				break;
			case ccPAINT:
				((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(1);
				break;
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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAttrDlg::OnOK()
{
	UpdateData(TRUE);

	m_attributes = m_attributes & ~FA_MASK;
	if (m_at01)	m_attributes |= faCOMMA;
	if (m_at02)	m_attributes |= faRIGHT;
	if (m_at03)	m_attributes |= faPASSWD;
	if (m_at04)	m_attributes |= faZEROSUP;
	if (m_at05)	m_attributes |= faSEND;
	if (m_at06)	m_attributes |= faPROTECT;
	if (m_at07)	m_attributes |= faWRAP;
	if (m_at08)	m_attributes |= faMUST;
	if (m_at09)	m_attributes |= faMULTI;
	if (m_at10)	m_attributes |= faSKIP;
	if (m_at11)	m_attributes |= faCORGB;
	if (m_at12)	m_attributes |= faDOMINO;
	if (m_at13)	m_attributes |= faREF;
//	if (m_at14)	m_attributes |= faDATE;
	if (m_at15)	m_attributes |= faFLASH;
	if (m_at16)	m_attributes |= faUPPER;
	if (m_at17)	m_attributes |= faDRAG;
	if (m_at18)	m_attributes |= faNOR;
	if (m_at19)	m_attributes |= faCONTRAST;
	if (m_at20)	m_attributes |= faVALIDX;

	if (m_attributes & faCOMMA)
		m_attributes |= faRIGHT;
	//if (m_attributes & faCORGB)
	//{
	//	switch (getTypeOfConditionColor(m_auxAttributes))
	//	{
	//	case ccRAW:
	//		break;
	//	case ccSUP:
	//		break;
	//	case ccSIGN:
	//		break;
	//	default:
	//		m_auxAttributes |= ccRAW;
	//		break;
	//	}
	//}

	CDialogEx::OnOK();
}


void CAttrDlg::OnCheckComma()
{
	UpdateData();
	if (m_at01)
		m_at02 = TRUE;
	UpdateData(FALSE);
}


void CAttrDlg::OnCheckRight()
{
	UpdateData();
	if (m_at01)
		m_at02 = TRUE;
	UpdateData(FALSE);
}


void CAttrDlg::OnCheckZerosup()
{
	UpdateData(TRUE);

	if (m_at04)	// ZEROSUP
	{
		GetDlgItem(IDC_OPT01)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT02)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT03)->EnableWindow(TRUE);

		switch (getTypeOfZeroSupress(m_auxAttributes))
		{
		case spSPACE:
			((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(1);
			break;
		case spASTER:
			((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(1);
			break;
		case spZERO:
			((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(1);
			break;
		default:
			break;
		}
	}
	else
	{
		CButton* btn;

		btn = (CButton *)GetDlgItem(IDC_OPT01);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);

		btn = (CButton *)GetDlgItem(IDC_OPT02);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);

		btn = (CButton *)GetDlgItem(IDC_OPT03);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);

		m_auxAttributes &= ~getTypeOfZeroSupress(m_auxAttributes);
	}
}


void CAttrDlg::OnCheckSend()
{
	UpdateData(TRUE);

	if (m_at05)	// SEND
	{
		GetDlgItem(IDC_OPT07)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT08)->EnableWindow(TRUE);

		switch (getTypeOfSendMethod(m_auxAttributes))
		{
		case smALL:
			((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(1);
			break;
		case smTHIS:
			((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(1);
			break;
		default:
			break;
		}
	}
	else
	{
		CButton* btn;

		btn = (CButton *)GetDlgItem(IDC_OPT07);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);

		btn = (CButton *)GetDlgItem(IDC_OPT08);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);
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

		switch (getTypeOfConditionColor(m_auxAttributes))
		{
		case ccRAW:
			((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
			break;
		case ccSUP:
			((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(1);
			break;
		case ccSIGN:
			((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(1);
			break;
		default:
			((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
			break;
		}
	}
	else
	{
		CButton* btn;

		btn = (CButton *)GetDlgItem(IDC_OPT04);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);

		btn = (CButton *)GetDlgItem(IDC_OPT05);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);

		btn = (CButton *)GetDlgItem(IDC_OPT06);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);
	}
}

void CAttrDlg::OnCheckContrast()
{
	UpdateData();
	
	if (m_at19)	// CONTRAST
	{
		GetDlgItem(IDC_OPT31)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPT32)->EnableWindow(TRUE);

		switch (getTypeOfConditionColor(m_auxAttributes))
		{
		case ccTEXT:
			((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
			break;
		case ccPAINT:
			((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(1);
			break;
		default:
			m_auxAttributes |= ccTEXT;
			((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
			break;
		}
	}
	else
	{
		CButton* btn;

		btn = (CButton *)GetDlgItem(IDC_OPT31);
		btn->SetCheck(1);
		btn->EnableWindow(FALSE);

		btn = (CButton *)GetDlgItem(IDC_OPT32);
		btn->SetCheck(0);
		btn->EnableWindow(FALSE);
	}
}

void CAttrDlg::OnContrast(UINT cmdID)
{
	m_auxAttributes &= ~getTypeOfConditionColor(m_auxAttributes);

	switch (cmdID)
	{
	case IDC_OPT31:
		m_auxAttributes |= ccTEXT;
		((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(0);
		break;
	case IDC_OPT32:
		m_auxAttributes |= ccPAINT;
		((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(1);
		break;

	default:
		break;
	}

	switch (getTypeOfConditionColor(m_auxAttributes))
	{
	case ccRAW:
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		break;
	case ccSUP:
		((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(1);
		break;
	case ccSIGN:
		((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(1);
		break;
	default:
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		break;
	}

	switch (getTypeOfZeroSupress(m_auxAttributes))
	{
	case spSPACE:
		((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(1);
		break;
	case spASTER:
		((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(1);
		break;
	case spZERO:
		((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfSendMethod(m_auxAttributes))
	{
	case smALL:
		((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(1);
		break;
	case smTHIS:
		((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(1);
		break;
	default:
		break;
	}
}

void CAttrDlg::OnZeroSup(UINT cmdID)
{
	m_auxAttributes &= ~getTypeOfZeroSupress(m_auxAttributes);

	switch (cmdID)
	{
	case IDC_OPT01:
		m_auxAttributes |= spSPACE;
		((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(0);
		break;
	case IDC_OPT02:
		m_auxAttributes |= spASTER;
		((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(0);
		break;
	case IDC_OPT03:
		m_auxAttributes |= spZERO;
		((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfConditionColor(m_auxAttributes))
	{
	case ccRAW:
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		break;
	case ccSUP:
		((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(1);
		break;
	case ccSIGN:
		((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(1);
		break;
	default:
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		break;
	}

	switch (getTypeOfSendMethod(m_auxAttributes))
	{
	case smALL:
		((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(1);
		break;
	case smTHIS:
		((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfConditionColor(m_auxAttributes))
	{
	case ccTEXT:
		((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
		break;
	case ccPAINT:
		((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(1);
		break;
	default:
		break;
	}
}

void CAttrDlg::OnCoRgb(UINT cmdID)
{
	m_auxAttributes &= ~getTypeOfConditionColor(m_auxAttributes);

	switch (cmdID)
	{
	case IDC_OPT04:
		m_auxAttributes |= ccRAW;
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(0);
		break;
	case IDC_OPT05:
		m_auxAttributes |= ccSUP;
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(0);
		break;
	case IDC_OPT06:
		m_auxAttributes |= ccSIGN;
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfZeroSupress(m_auxAttributes))
	{
	case spSPACE:
		((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(1);
		break;
	case spASTER:
		((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(1);
		break;
	case spZERO:
		((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfSendMethod(m_auxAttributes))
	{
	case smALL:
		((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(1);
		break;
	case smTHIS:
		((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfConditionColor(m_auxAttributes))
	{
	case ccTEXT:
		((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
		break;
	case ccPAINT:
		((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(1);
		break;
	default:
		break;
	}
}

void CAttrDlg::OnSend(UINT cmdID)
{
	m_auxAttributes &= ~getTypeOfSendMethod(m_auxAttributes);

	switch (cmdID)
	{
	case IDC_OPT07:
		m_auxAttributes |= smALL;
		((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(0);
		break;
	case IDC_OPT08:
		m_auxAttributes |= smTHIS;
		((CButton *)GetDlgItem(IDC_OPT07))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_OPT08))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfZeroSupress(m_auxAttributes))
	{
	case spSPACE:
		((CButton *)GetDlgItem(IDC_OPT01))->SetCheck(1);
		break;
	case spASTER:
		((CButton *)GetDlgItem(IDC_OPT02))->SetCheck(1);
		break;
	case spZERO:
		((CButton *)GetDlgItem(IDC_OPT03))->SetCheck(1);
		break;
	default:
		break;
	}

	switch (getTypeOfConditionColor(m_auxAttributes))
	{
	case ccRAW:
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		break;
	case ccSUP:
		((CButton *)GetDlgItem(IDC_OPT05))->SetCheck(1);
		break;
	case ccSIGN:
		((CButton *)GetDlgItem(IDC_OPT06))->SetCheck(1);
		break;
	default:
		((CButton *)GetDlgItem(IDC_OPT04))->SetCheck(1);
		break;
	}

	switch (getTypeOfConditionColor(m_auxAttributes))
	{
	case ccTEXT:
		((CButton *)GetDlgItem(IDC_OPT31))->SetCheck(1);
		break;
	case ccPAINT:
		((CButton *)GetDlgItem(IDC_OPT32))->SetCheck(1);
		break;
	default:
		break;
	}
}