// tabPage.cpp : implementation file
//

#include "stdafx.h"
#include "tabPage.h"

#include "../form_w.h"
#include "../mapvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPage dialog

CTabPage::CTabPage(CWnd* pParent, struct _formR* formR)
	: CDialog(CTabPage::IDD, pParent)
{
	m_formR = formR;
	m_sel = 0;
	//{{AFX_DATA_INIT(CTabPage)
	//}}AFX_DATA_INIT
}


void CTabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabPage, CDialog)
	//{{AFX_MSG_MAP(CTabPage)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_EN_SETFOCUS(IDC_OPT01, OnSetFocusCaption1)
	ON_EN_SETFOCUS(IDC_OPT02, OnSetFocusCaption2)
	ON_EN_SETFOCUS(IDC_OPT03, OnSetFocusCaption3)
	ON_EN_SETFOCUS(IDC_OPT04, OnSetFocusCaption4)
	ON_EN_SETFOCUS(IDC_OPT05, OnSetFocusCaption5)
	ON_EN_SETFOCUS(IDC_OPT06, OnSetFocusCaption6)
	ON_EN_SETFOCUS(IDC_OPT07, OnSetFocusCaption7)
	ON_EN_SETFOCUS(IDC_OPT08, OnSetFocusCaption8)
	ON_EN_SETFOCUS(IDC_OPT09, OnSetFocusCaption9)
	ON_EN_SETFOCUS(IDC_OPT10, OnSetFocusCaption10)
	ON_EN_SETFOCUS(IDC_OPT11, OnSetFocusCaption11)
	ON_EN_SETFOCUS(IDC_OPT12, OnSetFocusCaption12)
	ON_EN_SETFOCUS(IDC_OPT13, OnSetFocusCaption13)
	ON_EN_SETFOCUS(IDC_OPT14, OnSetFocusCaption14)
	ON_EN_SETFOCUS(IDC_OPT15, OnSetFocusCaption15)
	ON_EN_SETFOCUS(IDC_OPT16, OnSetFocusCaption16)
	ON_EN_SETFOCUS(IDC_OPT17, OnSetFocusMap1)
	ON_EN_SETFOCUS(IDC_OPT18, OnSetFocusMap2)
	ON_EN_SETFOCUS(IDC_OPT19, OnSetFocusMap3)
	ON_EN_SETFOCUS(IDC_OPT20, OnSetFocusMap4)
	ON_EN_SETFOCUS(IDC_OPT21, OnSetFocusMap5)
	ON_EN_SETFOCUS(IDC_OPT22, OnSetFocusMap6)
	ON_EN_SETFOCUS(IDC_OPT23, OnSetFocusMap7)
	ON_EN_SETFOCUS(IDC_OPT24, OnSetFocusMap8)
	ON_EN_SETFOCUS(IDC_OPT25, OnSetFocusMap9)
	ON_EN_SETFOCUS(IDC_OPT26, OnSetFocusMap10)
	ON_EN_SETFOCUS(IDC_OPT27, OnSetFocusMap11)
	ON_EN_SETFOCUS(IDC_OPT28, OnSetFocusMap12)
	ON_EN_SETFOCUS(IDC_OPT29, OnSetFocusMap13)
	ON_EN_SETFOCUS(IDC_OPT30, OnSetFocusMap14)
	ON_EN_SETFOCUS(IDC_OPT31, OnSetFocusMap15)
	ON_EN_SETFOCUS(IDC_OPT32, OnSetFocusMap16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabPage message handlers

BOOL CTabPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_text[0]  = (CEdit *)GetDlgItem(IDC_OPT01);
	m_text[1]  = (CEdit *)GetDlgItem(IDC_OPT02);
	m_text[2]  = (CEdit *)GetDlgItem(IDC_OPT03);
	m_text[3]  = (CEdit *)GetDlgItem(IDC_OPT04);
	m_text[4]  = (CEdit *)GetDlgItem(IDC_OPT05);
	m_text[5]  = (CEdit *)GetDlgItem(IDC_OPT06);
	m_text[6]  = (CEdit *)GetDlgItem(IDC_OPT07);
	m_text[7]  = (CEdit *)GetDlgItem(IDC_OPT08);
	m_text[8]  = (CEdit *)GetDlgItem(IDC_OPT09);
	m_text[9]  = (CEdit *)GetDlgItem(IDC_OPT10);
	m_text[10] = (CEdit *)GetDlgItem(IDC_OPT11);
	m_text[11] = (CEdit *)GetDlgItem(IDC_OPT12);
	m_text[12] = (CEdit *)GetDlgItem(IDC_OPT13);
	m_text[13] = (CEdit *)GetDlgItem(IDC_OPT14);
	m_text[14] = (CEdit *)GetDlgItem(IDC_OPT15);
	m_text[15] = (CEdit *)GetDlgItem(IDC_OPT16);
	
	m_map[0]  = (CEdit *)GetDlgItem(IDC_OPT17);
	m_map[1]  = (CEdit *)GetDlgItem(IDC_OPT18);
	m_map[2]  = (CEdit *)GetDlgItem(IDC_OPT19);
	m_map[3]  = (CEdit *)GetDlgItem(IDC_OPT20);
	m_map[4]  = (CEdit *)GetDlgItem(IDC_OPT21);
	m_map[5]  = (CEdit *)GetDlgItem(IDC_OPT22);
	m_map[6]  = (CEdit *)GetDlgItem(IDC_OPT23);
	m_map[7]  = (CEdit *)GetDlgItem(IDC_OPT24);
	m_map[8]  = (CEdit *)GetDlgItem(IDC_OPT25);
	m_map[9]  = (CEdit *)GetDlgItem(IDC_OPT26);
	m_map[10] = (CEdit *)GetDlgItem(IDC_OPT27);
	m_map[11] = (CEdit *)GetDlgItem(IDC_OPT28);
	m_map[12] = (CEdit *)GetDlgItem(IDC_OPT29);
	m_map[13] = (CEdit *)GetDlgItem(IDC_OPT30);
	m_map[14] = (CEdit *)GetDlgItem(IDC_OPT31);
	m_map[15] = (CEdit *)GetDlgItem(IDC_OPT32);

	for (int ii = 0; ii < MAX_TAB; ii++)
		m_map[ii]->LimitText(L_MAPN);

	if (m_formR->keys == 0)
		return TRUE;

	CString	tmps = _T("");
	char*	dat = m_formR->dat;
	for (ii = 0 ; ii < (int)m_formR->keys; ii++)
	{
		tmps = dat;
		m_text[ii]->SetWindowText(tmps);
		dat += tmps.GetLength();
		dat++;
	}

	dat = m_formR->str;
	for (ii = 0; ii < (int)m_formR->keys; ii++)
	{
		tmps = CString(dat, L_MAPN);
		m_map[ii]->SetWindowText(tmps);
		dat += L_MAPN;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabPage::OnInsert() 
{
	CString	text;
	CString	map;

	m_text[MAX_TAB-1]->SetWindowText("");
	m_map[MAX_TAB-1]->SetWindowText("");
	for (int ii = MAX_TAB-1; ii > m_sel; ii--)
	{
		text.Empty();
		map.Empty();

		m_text[ii-1]->GetWindowText(text);
		m_map[ii-1]->GetWindowText(map);

		m_text[ii]->SetWindowText(text);
		m_map[ii]->SetWindowText(map);

		m_text[ii-1]->SetWindowText("");
		m_map[ii-1]->SetWindowText("");
	}
	m_text[m_sel]->SetFocus();
}

void CTabPage::OnDelete() 
{
	CString	text;
	CString	map;

	m_text[m_sel]->SetWindowText("");
	m_map[m_sel]->SetWindowText("");

	for (int ii = m_sel; ii < MAX_TAB-1; ii++)
	{
		text.Empty();
		map.Empty();

		m_text[ii+1]->GetWindowText(text);
		m_map[ii+1]->GetWindowText(map);

		m_text[ii]->SetWindowText(text);
		m_map[ii]->SetWindowText(map);

		m_text[ii+1]->SetWindowText("");
		m_map[ii+1]->SetWindowText("");
	}
	m_text[m_sel]->SetFocus();
}

void CTabPage::OnCancel() 
{
	CDialog::OnCancel();
}

void CTabPage::OnOK() 
{
	CString	text;
	CString	map;
	int	disp, data;

	ZeroMemory(m_formR->dat, sizeof(m_formR->dat));
	disp = data = 0;
	for (int ii = 0; ii < MAX_TAB ; ii++)
	{
		text.Empty();
		map.Empty();

		m_text[ii]->GetWindowText(text);
		m_map[ii]->GetWindowText(map);

		if (text.GetLength() == 0 || map.GetLength() == 0)
			break;
		else if (map.GetLength() != L_MAPN)
		{
			text.Format("[%d]번째 MapName 입력 오류", ii+1);
			::MessageBox(AfxGetMainWnd()->m_hWnd, text, "ERROR", MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		CopyMemory(&m_formR->dat[disp], (char *)text.operator LPCTSTR(),
					text.GetLength());
		disp += text.GetLength();
		m_formR->dat[disp] = '\t';	disp++;
		CopyMemory(&m_formR->str[data], (char *)map.operator LPCTSTR(), L_MAPN);
		data += L_MAPN;
	}
	m_formR->keys = (WORD) ii;
	CDialog::OnOK();
}

void CTabPage::OnSetFocusCaption1()	{ setTextIndex(IDC_OPT01); }
void CTabPage::OnSetFocusCaption2()	{ setTextIndex(IDC_OPT02); }
void CTabPage::OnSetFocusCaption3()	{ setTextIndex(IDC_OPT03); }
void CTabPage::OnSetFocusCaption4()	{ setTextIndex(IDC_OPT04); }
void CTabPage::OnSetFocusCaption5()	{ setTextIndex(IDC_OPT05); }
void CTabPage::OnSetFocusCaption6()	{ setTextIndex(IDC_OPT06); }
void CTabPage::OnSetFocusCaption7()	{ setTextIndex(IDC_OPT07); }
void CTabPage::OnSetFocusCaption8()	{ setTextIndex(IDC_OPT08); }
void CTabPage::OnSetFocusCaption9()	{ setTextIndex(IDC_OPT09); }
void CTabPage::OnSetFocusCaption10()	{ setTextIndex(IDC_OPT10); }
void CTabPage::OnSetFocusCaption11()	{ setTextIndex(IDC_OPT11); }
void CTabPage::OnSetFocusCaption12()	{ setTextIndex(IDC_OPT12); }
void CTabPage::OnSetFocusCaption13()	{ setTextIndex(IDC_OPT13); }
void CTabPage::OnSetFocusCaption14()	{ setTextIndex(IDC_OPT14); }
void CTabPage::OnSetFocusCaption15()	{ setTextIndex(IDC_OPT15); }
void CTabPage::OnSetFocusCaption16()	{ setTextIndex(IDC_OPT16); }
void CTabPage::setTextIndex(int id)
{
	m_sel = id - IDC_OPT01;
}

void CTabPage::OnSetFocusMap1()		{ setMapIndex(IDC_OPT17); }
void CTabPage::OnSetFocusMap2()		{ setMapIndex(IDC_OPT18); }
void CTabPage::OnSetFocusMap3()		{ setMapIndex(IDC_OPT19); }
void CTabPage::OnSetFocusMap4()		{ setMapIndex(IDC_OPT20); }
void CTabPage::OnSetFocusMap5()		{ setMapIndex(IDC_OPT21); }
void CTabPage::OnSetFocusMap6()		{ setMapIndex(IDC_OPT22); }
void CTabPage::OnSetFocusMap7()		{ setMapIndex(IDC_OPT23); }
void CTabPage::OnSetFocusMap8()		{ setMapIndex(IDC_OPT24); }
void CTabPage::OnSetFocusMap9()		{ setMapIndex(IDC_OPT25); }
void CTabPage::OnSetFocusMap10()	{ setMapIndex(IDC_OPT26); }
void CTabPage::OnSetFocusMap11()	{ setMapIndex(IDC_OPT27); }
void CTabPage::OnSetFocusMap12()	{ setMapIndex(IDC_OPT28); }
void CTabPage::OnSetFocusMap13()	{ setMapIndex(IDC_OPT29); }
void CTabPage::OnSetFocusMap14()	{ setMapIndex(IDC_OPT30); }
void CTabPage::OnSetFocusMap15()	{ setMapIndex(IDC_OPT31); }
void CTabPage::OnSetFocusMap16()	{ setMapIndex(IDC_OPT32); }

void CTabPage::setMapIndex(int id)
{
	m_sel = id - IDC_OPT17;
}
