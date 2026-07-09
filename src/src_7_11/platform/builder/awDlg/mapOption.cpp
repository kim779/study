// mapOption.cpp : implementation file
//

#include "stdafx.h"
#include "mapOption.h"
#include "../mainvar.h"
#include "../mapform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapOption property page

IMPLEMENT_DYNCREATE(CMapOption, CDialog)

CMapOption::CMapOption(struct _mapH* mapH)
	: CDialog(CMapOption::IDD)
{
	m_mapH = mapH;

	//{{AFX_DATA_INIT(CMapOption)
	m_opt01 = FALSE;
	m_opt04 = FALSE;
	m_opt06 = FALSE;
	m_opt11 = FALSE;
	m_opt12 = FALSE;
	m_opt15 = FALSE;
	m_opt16 = FALSE;
	m_opt17 = FALSE;
	m_opt19 = FALSE;
	m_opt22 = FALSE;
	m_opt23 = FALSE;
	m_opt24 = FALSE;
	m_opt25 = FALSE;
	m_opt26 = FALSE;
	m_opt27 = FALSE;
	//}}AFX_DATA_INIT
}

CMapOption::~CMapOption()
{
}

void CMapOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapOption)
	DDX_Check(pDX, IDC_OPT11, m_opt01);
	DDX_Check(pDX, IDC_OPT14, m_opt04);
	DDX_Check(pDX, IDC_OPT16, m_opt06);
	DDX_Check(pDX, IDC_OPT21, m_opt11);
	DDX_Check(pDX, IDC_OPT22, m_opt12);
	DDX_Check(pDX, IDC_OPT25, m_opt15);
	DDX_Check(pDX, IDC_OPT26, m_opt16);
	DDX_Check(pDX, IDC_OPT27, m_opt17);
	DDX_Check(pDX, IDC_OPT29, m_opt19);
	DDX_Check(pDX, IDC_OPT32, m_opt22);
	DDX_Check(pDX, IDC_OPT33, m_opt23);
	DDX_Check(pDX, IDC_OPT34, m_opt24);
	DDX_Check(pDX, IDC_OPT35, m_opt25);
	DDX_Check(pDX, IDC_OPT36, m_opt26);
	DDX_Check(pDX, IDC_OPT37, m_opt27);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapOption, CDialog)
	//{{AFX_MSG_MAP(CMapOption)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_OPT01, IDC_OPT07, OnClick)
	ON_COMMAND_RANGE(IDC_OPT11, IDC_OPT27, OnClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapOption message handlers

BOOL CMapOption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_typeB[0] = (CButton *)GetDlgItem(IDC_OPT01);
	m_typeB[1] = (CButton *)GetDlgItem(IDC_OPT02);
	m_typeB[2] = (CButton *)GetDlgItem(IDC_OPT03);
	m_typeB[3] = (CButton *)GetDlgItem(IDC_OPT04);
	m_typeB[4] = (CButton *)GetDlgItem(IDC_OPT05);
	m_typeB[5] = (CButton *)GetDlgItem(IDC_OPT06);
	m_typeB[6] = (CButton *)GetDlgItem(IDC_OPT07);
	m_typeB[7] = (CButton *)GetDlgItem(IDC_OPT08);

	m_typeB[m_mapH->typeH]->SetCheck(1);
	if (m_mapH->options & OP_OOP)
		m_opt01 = TRUE;
	if (m_mapH->options & OP_CLS)
		m_opt04 = TRUE;
	if (m_mapH->options & OP_CNV)
		m_opt06 = TRUE;
	if (m_mapH->options & OP_FLASH)
		m_opt11 = TRUE;
	if (m_mapH->options & OP_ENC)
		m_opt12 = TRUE;
	if (m_mapH->options & OP_TABS)
		m_opt15 = TRUE;
	if (m_mapH->options & OP_CR2TB)
		m_opt16 = TRUE;
	if (m_mapH->options & OP_DROP)
		m_opt17 = TRUE;
	if (m_mapH->options & OP_INTER)
		m_opt19 = TRUE;
	if (m_mapH->options & OP_USER)
		m_opt22 = TRUE;
	if (m_mapH->options & OP_CERTIFY)
		m_opt23 = TRUE;
	if (m_mapH->options & OP_OOP2)
		m_opt24 = TRUE;
	if (m_mapH->options & OP_DOMINO)
		m_opt25 = TRUE;
	if (m_mapH->options & OP_XFONT)
		m_opt26 = TRUE;
	if (m_mapH->options & OP_SAVE)
		m_opt27 = TRUE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapOption::OnClick(UINT cmdID)
{
	UpdateData(TRUE);
	//SetModified(TRUE);
}

void CMapOption::OnOK() 
{
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	UpdateData(TRUE);

	m_mapH->modified = true;
	for (int ii = 0; ii < 8; ii++)
	{
		if (m_typeB[ii]->GetCheck())
		{
			m_mapH->typeH = (BYTE) ii;
			break;
		}
	}

	m_mapH->options = 0;
	if (m_opt01)
		m_mapH->options |= OP_OOP;
	if (m_opt04)
		m_mapH->options |= OP_CLS;
	if (m_opt06)
		m_mapH->options |= OP_CNV;
	if (m_opt11)
		m_mapH->options |= OP_FLASH;
	if (m_opt12)
		m_mapH->options |= OP_ENC;
	if (m_opt15)
		m_mapH->options |= OP_TABS;
	if (m_opt16)
		m_mapH->options |= OP_CR2TB;
	if (m_opt17)
		m_mapH->options |= OP_DROP;
	if (m_opt19)
		m_mapH->options |= OP_INTER;
	if (m_opt22)
		m_mapH->options |= OP_USER;
	if (m_opt23)
		m_mapH->options |= OP_CERTIFY;
	if (m_opt24)
		m_mapH->options |= OP_OOP2;
	if (m_opt25)
		m_mapH->options |= OP_DOMINO;
	if (m_opt26)
		m_mapH->options |= OP_XFONT;
	if (m_opt27)
		m_mapH->options |= OP_SAVE;

	CDialog::OnOK();
}

