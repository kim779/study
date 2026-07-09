// uTabPage.cpp : implementation file
//

#include "stdafx.h"
#include "uTabPage.h"

#include "../form_w.h"
#include "../mapvar.h"
#include "../../h/axisvar.h"
#include "../awCommon/FormItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUTabPage dialog

CUTabPage::CUTabPage(CWnd* pParent, struct _mapH* mapH, int index)
	: CDialog(CUTabPage::IDD, pParent)
{
	m_mapH = mapH;
	m_sel = 0;
	m_index = index;
	//{{AFX_DATA_INIT(CUTabPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUTabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUTabPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUTabPage, CDialog)
	//{{AFX_MSG_MAP(CUTabPage)
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
	ON_EN_SETFOCUS(IDC_OPT17, OnSetFocusID1)
	ON_EN_SETFOCUS(IDC_OPT18, OnSetFocusID2)
	ON_EN_SETFOCUS(IDC_OPT19, OnSetFocusID3)
	ON_EN_SETFOCUS(IDC_OPT20, OnSetFocusID4)
	ON_EN_SETFOCUS(IDC_OPT21, OnSetFocusID5)
	ON_EN_SETFOCUS(IDC_OPT22, OnSetFocusID6)
	ON_EN_SETFOCUS(IDC_OPT23, OnSetFocusID7)
	ON_EN_SETFOCUS(IDC_OPT24, OnSetFocusID8)
	ON_EN_SETFOCUS(IDC_OPT25, OnSetFocusID9)
	ON_EN_SETFOCUS(IDC_OPT26, OnSetFocusID10)
	ON_EN_SETFOCUS(IDC_OPT27, OnSetFocusID11)
	ON_EN_SETFOCUS(IDC_OPT28, OnSetFocusID12)
	ON_EN_SETFOCUS(IDC_OPT29, OnSetFocusID13)
	ON_EN_SETFOCUS(IDC_OPT30, OnSetFocusID14)
	ON_EN_SETFOCUS(IDC_OPT31, OnSetFocusID15)
	ON_EN_SETFOCUS(IDC_OPT32, OnSetFocusID16)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_CHECK01, OnCheck01)
	ON_BN_CLICKED(IDC_CHECK02, OnCheck02)
	ON_BN_CLICKED(IDC_CHECK03, OnCheck03)
	ON_BN_CLICKED(IDC_CHECK04, OnCheck04)
	ON_BN_CLICKED(IDC_CHECK05, OnCheck05)
	ON_BN_CLICKED(IDC_CHECK06, OnCheck06)
	ON_BN_CLICKED(IDC_CHECK07, OnCheck07)
	ON_BN_CLICKED(IDC_CHECK08, OnCheck08)
	ON_BN_CLICKED(IDC_CHECK09, OnCheck09)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	ON_BN_CLICKED(IDC_CHECK16, OnCheck16)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUTabPage message handlers

BOOL CUTabPage::OnInitDialog() 
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
	
	m_ID[0]  = (CEdit *)GetDlgItem(IDC_OPT17);
	m_ID[1]  = (CEdit *)GetDlgItem(IDC_OPT18);
	m_ID[2]  = (CEdit *)GetDlgItem(IDC_OPT19);
	m_ID[3]  = (CEdit *)GetDlgItem(IDC_OPT20);
	m_ID[4]  = (CEdit *)GetDlgItem(IDC_OPT21);
	m_ID[5]  = (CEdit *)GetDlgItem(IDC_OPT22);
	m_ID[6]  = (CEdit *)GetDlgItem(IDC_OPT23);
	m_ID[7]  = (CEdit *)GetDlgItem(IDC_OPT24);
	m_ID[8]  = (CEdit *)GetDlgItem(IDC_OPT25);
	m_ID[9]  = (CEdit *)GetDlgItem(IDC_OPT26);
	m_ID[10] = (CEdit *)GetDlgItem(IDC_OPT27);
	m_ID[11] = (CEdit *)GetDlgItem(IDC_OPT28);
	m_ID[12] = (CEdit *)GetDlgItem(IDC_OPT29);
	m_ID[13] = (CEdit *)GetDlgItem(IDC_OPT30);
	m_ID[14] = (CEdit *)GetDlgItem(IDC_OPT31);
	m_ID[15] = (CEdit *)GetDlgItem(IDC_OPT32);

	m_visible[0]  = (CButton *)GetDlgItem(IDC_CHECK01);
	m_visible[1]  = (CButton *)GetDlgItem(IDC_CHECK02);
	m_visible[2]  = (CButton *)GetDlgItem(IDC_CHECK03);
	m_visible[3]  = (CButton *)GetDlgItem(IDC_CHECK04);
	m_visible[4]  = (CButton *)GetDlgItem(IDC_CHECK05);
	m_visible[5]  = (CButton *)GetDlgItem(IDC_CHECK06);
	m_visible[6]  = (CButton *)GetDlgItem(IDC_CHECK07);
	m_visible[7]  = (CButton *)GetDlgItem(IDC_CHECK08);
	m_visible[8]  = (CButton *)GetDlgItem(IDC_CHECK09);
	m_visible[9]  = (CButton *)GetDlgItem(IDC_CHECK10);
	m_visible[10] = (CButton *)GetDlgItem(IDC_CHECK11);
	m_visible[11] = (CButton *)GetDlgItem(IDC_CHECK12);
	m_visible[12] = (CButton *)GetDlgItem(IDC_CHECK13);
	m_visible[13] = (CButton *)GetDlgItem(IDC_CHECK14);
	m_visible[14] = (CButton *)GetDlgItem(IDC_CHECK15);
	m_visible[15] = (CButton *)GetDlgItem(IDC_CHECK16);

	int ii = 0;
	for (ii = 0; ii < MAX_TAB; ii++)
		m_ID[ii]->LimitText(2);

	CFormItem formItem(m_mapH);
	if (formItem.getFormPoint(m_index)->keys == 0)
		return TRUE;

	CString	tmps = _T("");
	CString	dat = formItem.getFormStr(m_index, "dat");
	for (ii = 0 ; ii < (int)formItem.getFormPoint(m_index)->keys; ii++)
	{
		tmps = Parser(dat, "\t");
		m_text[ii]->SetWindowText(tmps);
	}

	dat = formItem.getFormStr(m_index, "str");
	for (ii = 0; ii < (int)formItem.getFormPoint(m_index)->keys; ii++)
	{
		tmps = Parser(dat, "\t");
		if (tmps[0] == '1')
			m_visible[ii]->SetCheck(1);
		m_ID[ii]->SetWindowText(tmps.Mid(2));
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUTabPage::OnInsert() 
{
	CString	text;
	CString	ID;
	int	visible;

	m_text[MAX_TAB-1]->SetWindowText("");
	m_ID[MAX_TAB-1]->SetWindowText("");
	m_visible[MAX_TAB-1]->SetCheck(0);
	for (int ii = MAX_TAB-1; ii > m_sel; ii--)
	{
		text.Empty();
		ID.Empty();
		visible = 0;

		m_text[ii-1]->GetWindowText(text);
		m_ID[ii-1]->GetWindowText(ID);
		visible = m_visible[ii-1]->GetCheck();

		m_text[ii]->SetWindowText(text);
		m_ID[ii]->SetWindowText(ID);
		m_visible[ii]->SetCheck(visible);

		m_text[ii-1]->SetWindowText("");
		m_ID[ii-1]->SetWindowText("");
		m_visible[ii-1]->SetCheck(0);
	}
	m_text[m_sel]->SetFocus();
}

void CUTabPage::OnDelete() 
{
	CString	text;
	CString	ID;
	int	visible;

	m_text[m_sel]->SetWindowText("");
	m_ID[m_sel]->SetWindowText("");
	m_visible[m_sel]->SetCheck(0);

	for (int ii = m_sel; ii < MAX_TAB-1; ii++)
	{
		text.Empty();
		ID.Empty();
		visible = 0;

		m_text[ii+1]->GetWindowText(text);
		m_ID[ii+1]->GetWindowText(ID);
		visible = m_visible[ii+1]->GetCheck();

		m_text[ii+1]->SetWindowText("");
		m_ID[ii+1]->SetWindowText("");
		m_visible[ii+1]->SetCheck(0);
		
		m_text[ii]->SetWindowText(text);
		m_ID[ii]->SetWindowText(ID);
		m_visible[ii]->SetCheck(visible);
	}
	m_text[m_sel]->SetFocus();
}

void CUTabPage::OnCancel() 
{
	CDialog::OnCancel();
}

void CUTabPage::OnOK() 
{
	CString	text, sDat, sStr;
	CString	id, tmps;
	int ii = 0;
	for (ii = 0; ii < MAX_TAB ; ii++)
	{
		text.Empty();
		tmps.Empty();

		m_text[ii]->GetWindowText(text);
		m_ID[ii]->GetWindowText(tmps);

		if (text.GetLength() == 0 || tmps.GetLength() == 0)
			break;
		else if (tmps.GetLength() == 0)
		{
			text.Format("[%d]번째 ID 입력 오류", ii+1);
			::MessageBox(AfxGetMainWnd()->m_hWnd, text, "ERROR", MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		sDat += text;
		sDat += "\t";

		if (m_visible[ii]->GetCheck())
			id.Format("1:%d", atoi(tmps));
		else
			id.Format("0:%d", atoi(tmps));
		sStr += id;
		sStr += "\t";
	}
	CFormItem formItem(m_mapH);
	formItem.setFormStr(m_index, "dat", sDat);
	formItem.setFormStr(m_index, "str", sStr);
	formItem.getFormPoint(m_index)->keys = (WORD) ii;
	
	CDialog::OnOK();
}

void CUTabPage::OnSetFocusCaption1()	{ setTextIndex(IDC_OPT01); }
void CUTabPage::OnSetFocusCaption2()	{ setTextIndex(IDC_OPT02); }
void CUTabPage::OnSetFocusCaption3()	{ setTextIndex(IDC_OPT03); }
void CUTabPage::OnSetFocusCaption4()	{ setTextIndex(IDC_OPT04); }
void CUTabPage::OnSetFocusCaption5()	{ setTextIndex(IDC_OPT05); }
void CUTabPage::OnSetFocusCaption6()	{ setTextIndex(IDC_OPT06); }
void CUTabPage::OnSetFocusCaption7()	{ setTextIndex(IDC_OPT07); }
void CUTabPage::OnSetFocusCaption8()	{ setTextIndex(IDC_OPT08); }
void CUTabPage::OnSetFocusCaption9()	{ setTextIndex(IDC_OPT09); }
void CUTabPage::OnSetFocusCaption10()	{ setTextIndex(IDC_OPT10); }
void CUTabPage::OnSetFocusCaption11()	{ setTextIndex(IDC_OPT11); }
void CUTabPage::OnSetFocusCaption12()	{ setTextIndex(IDC_OPT12); }
void CUTabPage::OnSetFocusCaption13()	{ setTextIndex(IDC_OPT13); }
void CUTabPage::OnSetFocusCaption14()	{ setTextIndex(IDC_OPT14); }
void CUTabPage::OnSetFocusCaption15()	{ setTextIndex(IDC_OPT15); }
void CUTabPage::OnSetFocusCaption16()	{ setTextIndex(IDC_OPT16); }
void CUTabPage::setTextIndex(int id)
{
	m_sel = id - IDC_OPT01;
}

void CUTabPage::OnSetFocusID1()		{ setIDIndex(IDC_OPT17); } 
void CUTabPage::OnSetFocusID2()		{ setIDIndex(IDC_OPT18); }
void CUTabPage::OnSetFocusID3()		{ setIDIndex(IDC_OPT19); }
void CUTabPage::OnSetFocusID4()		{ setIDIndex(IDC_OPT20); }
void CUTabPage::OnSetFocusID5()		{ setIDIndex(IDC_OPT21); }
void CUTabPage::OnSetFocusID6()		{ setIDIndex(IDC_OPT22); }
void CUTabPage::OnSetFocusID7()		{ setIDIndex(IDC_OPT23); }
void CUTabPage::OnSetFocusID8()		{ setIDIndex(IDC_OPT24); }
void CUTabPage::OnSetFocusID9()		{ setIDIndex(IDC_OPT25); }
void CUTabPage::OnSetFocusID10()	{ setIDIndex(IDC_OPT26); }
void CUTabPage::OnSetFocusID11()	{ setIDIndex(IDC_OPT27); }
void CUTabPage::OnSetFocusID12()	{ setIDIndex(IDC_OPT28); }
void CUTabPage::OnSetFocusID13()	{ setIDIndex(IDC_OPT29); }
void CUTabPage::OnSetFocusID14()	{ setIDIndex(IDC_OPT30); }
void CUTabPage::OnSetFocusID15()	{ setIDIndex(IDC_OPT31); }
void CUTabPage::OnSetFocusID16()	{ setIDIndex(IDC_OPT32); }
void CUTabPage::setIDIndex(int id)
{
	m_sel = id - IDC_OPT17;
}

void CUTabPage::OnCheck01()		{ setVisibleIndex(IDC_CHECK01); }
void CUTabPage::OnCheck02()		{ setVisibleIndex(IDC_CHECK02); }
void CUTabPage::OnCheck03()		{ setVisibleIndex(IDC_CHECK03); }
void CUTabPage::OnCheck04()		{ setVisibleIndex(IDC_CHECK04); }
void CUTabPage::OnCheck05()		{ setVisibleIndex(IDC_CHECK05); }
void CUTabPage::OnCheck06()		{ setVisibleIndex(IDC_CHECK06); }
void CUTabPage::OnCheck07()		{ setVisibleIndex(IDC_CHECK07); }
void CUTabPage::OnCheck08()		{ setVisibleIndex(IDC_CHECK08); }
void CUTabPage::OnCheck09()		{ setVisibleIndex(IDC_CHECK09); }
void CUTabPage::OnCheck10()		{ setVisibleIndex(IDC_CHECK10); }
void CUTabPage::OnCheck11()		{ setVisibleIndex(IDC_CHECK11); }
void CUTabPage::OnCheck12()		{ setVisibleIndex(IDC_CHECK12); }
void CUTabPage::OnCheck13()		{ setVisibleIndex(IDC_CHECK13); }
void CUTabPage::OnCheck14()		{ setVisibleIndex(IDC_CHECK14); }
void CUTabPage::OnCheck15()		{ setVisibleIndex(IDC_CHECK15); }
void CUTabPage::OnCheck16()		{ setVisibleIndex(IDC_CHECK16); }
void CUTabPage::setVisibleIndex(int id)
{
	m_sel = id - IDC_CHECK01;
}

CString CUTabPage::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}
