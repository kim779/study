// option.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "option.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../h/mapform.h"

// COption 대화 상자입니다.

IMPLEMENT_DYNAMIC(COption, CDialogEx)

COption::COption(class mapForm* mapForm, CWnd* pParent)
	: CDialogEx(COption::IDD, pParent)
{
	m_mapH = mapForm;

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
}

COption::~COption()
{
}

void COption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
}


BEGIN_MESSAGE_MAP(COption, CDialogEx)
END_MESSAGE_MAP()


// COption 메시지 처리기입니다.


BOOL COption::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COption::OnOK()
{
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	UpdateData(TRUE);

	m_mapH->modified = true;

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

	CDialogEx::OnOK();
}

void COption::OnClick(UINT cmdID)
{
	UpdateData(TRUE);
	//SetModified(TRUE);
}