// option.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "option.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../h/mapform.h"

// COption 대화 상자입니다.

IMPLEMENT_DYNAMIC(COption, CDialogEx)

COption::COption(struct _mapH* mapH, CWnd* pParent)
	: CDialogEx(COption::IDD, pParent)
{
	m_mapH = mapH;

	m_opt01 = FALSE;
	m_opt02 = FALSE;
	m_opt03 = FALSE;
	m_opt04 = FALSE;
	m_opt05 = FALSE;
	m_opt06 = FALSE;
	m_opt07 = FALSE;
	m_opt08 = FALSE;
	m_opt09 = FALSE;
}

COption::~COption()
{
}

void COption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_OPT01, m_opt01);
	DDX_Check(pDX, IDC_OPT02, m_opt02);
	DDX_Check(pDX, IDC_OPT03, m_opt03);
	DDX_Check(pDX, IDC_OPT04, m_opt04);
	DDX_Check(pDX, IDC_OPT05, m_opt05);
	DDX_Check(pDX, IDC_OPT06, m_opt06);
	DDX_Check(pDX, IDC_OPT07, m_opt07);
	DDX_Check(pDX, IDC_OPT08, m_opt08);
	DDX_Check(pDX, IDC_OPT09, m_opt09);
}


BEGIN_MESSAGE_MAP(COption, CDialogEx)
END_MESSAGE_MAP()


// COption 메시지 처리기입니다.


BOOL COption::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_mapH->options & OP_LEDGER)
		m_opt01 = TRUE;
	if (m_mapH->options & OP_OOP)
		m_opt02 = TRUE;
	if (m_mapH->options & OP_FLASH)
		m_opt03 = TRUE;
	if (m_mapH->options & OP_DOMINO)
		m_opt04 = TRUE;
	if (m_mapH->options & OP_TABS)
		m_opt05 = TRUE;
	if (m_mapH->options & OP_CR2TB)
		m_opt06 = TRUE;
	if (m_mapH->options & OP_ENC)
		m_opt07 = TRUE;
	if (m_mapH->options & OP_CERTIFY)
		m_opt08 = TRUE;
	if (m_mapH->options & OP_SAVE)
		m_opt09 = TRUE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COption::OnClick(UINT cmdID)
{
	UpdateData(TRUE);
}

void COption::OnOK() 
{
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	UpdateData(TRUE);

	m_mapH->modified = true;
	m_mapH->options = 0;
	if (m_opt01)
		m_mapH->options |= OP_LEDGER;
	if (m_opt02)
		m_mapH->options |= OP_OOP;
	if (m_opt03)
		m_mapH->options |= OP_FLASH;
	if (m_opt04)
		m_mapH->options |= OP_DOMINO;
	if (m_opt05)
		m_mapH->options |= OP_TABS;
	if (m_opt06)
		m_mapH->options |= OP_CR2TB;
	if (m_opt07)
		m_mapH->options |= OP_ENC;
	if (m_opt08)
		m_mapH->options |= OP_CERTIFY;
	if (m_opt09)
		m_mapH->options |= OP_SAVE;

	CDialog::OnOK();
}

