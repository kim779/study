// tabPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tabPage.h"
#include "afxdialogex.h"

#include "../h/axisvar.h"
#include "../amCom/formItem.h"

// CTabPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabPage, CDialogEx)

CTabPage::CTabPage(CWnd* pParent, struct _mapH* mapH, int index)
	: CDialogEx(CTabPage::IDD, pParent)
{
	m_mapH = mapH;
	m_sel = 0;
	m_index = index;
}

CTabPage::~CTabPage()
{
}

void CTabPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPage, CDialogEx)
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
	ON_EN_SETFOCUS(IDC_OPT33, OnSetFocusForm1)
	ON_EN_SETFOCUS(IDC_OPT34, OnSetFocusForm2)
	ON_EN_SETFOCUS(IDC_OPT35, OnSetFocusForm3)
	ON_EN_SETFOCUS(IDC_OPT36, OnSetFocusForm4)
	ON_EN_SETFOCUS(IDC_OPT37, OnSetFocusForm5)
	ON_EN_SETFOCUS(IDC_OPT38, OnSetFocusForm6)
	ON_EN_SETFOCUS(IDC_OPT39, OnSetFocusForm7)
	ON_EN_SETFOCUS(IDC_OPT40, OnSetFocusForm8)
	ON_EN_SETFOCUS(IDC_OPT41, OnSetFocusForm9)
	ON_EN_SETFOCUS(IDC_OPT42, OnSetFocusForm10)
	ON_EN_SETFOCUS(IDC_OPT43, OnSetFocusForm11)
	ON_EN_SETFOCUS(IDC_OPT44, OnSetFocusForm12)
	ON_EN_SETFOCUS(IDC_OPT45, OnSetFocusForm13)
	ON_EN_SETFOCUS(IDC_OPT46, OnSetFocusForm14)
	ON_EN_SETFOCUS(IDC_OPT47, OnSetFocusForm15)
	ON_EN_SETFOCUS(IDC_OPT48, OnSetFocusForm16)
END_MESSAGE_MAP()

// CTabPage 메시지 처리기입니다.

BOOL CTabPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString	tmp, tmps, data;
	CFormItem formItem(m_mapH);

	for (int ii = 0; ii < MAX_TAB; ii++)
	{
		m_text[ii] = (CEdit *)GetDlgItem(IDC_OPT01+ii);
		m_ID[ii] = (CEdit *)GetDlgItem(IDC_OPT17+ii);
		m_ID[ii]->LimitText(2);
		m_visible[ii] = (CButton *)GetDlgItem(IDC_CHECK01+ii);
		m_Form[ii] = (CEdit *)GetDlgItem(IDC_OPT33+ii);
	}

	if (formItem.GetForm(m_index)->keys == 0)
		return TRUE;

	data = formItem.GetFormStr(m_index, _T("dat"));
	for (int ii = 0 ; ii < (int)formItem.GetForm(m_index)->keys; ii++)
	{
		tmps = parse(data, _T("\t"));
		m_text[ii]->SetWindowText(tmps);
	}

	data = formItem.GetFormStr(m_index, _T("str"));
	for (int ii = 0; ii < (int)formItem.GetForm(m_index)->keys; ii++)
	{
		tmps = parse(data, _T("\t"));
		if (tmps[0] == _T('1'))
			m_visible[ii]->SetCheck(1);
		tmps = tmps.Mid(2);

		tmp = parse(tmps, _T(":"));
		m_ID[ii]->SetWindowText(tmp);
		m_Form[ii]->SetWindowText(tmps);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTabPage::OnOK()
{
	int	ii;
	CString	tmps, text, form;
	CString	data, str;
	CFormItem formItem(m_mapH);

	data = str = _T("");
	for (ii = 0; ii < MAX_TAB ; ii++)
	{
		m_text[ii]->GetWindowText(text);
		m_ID[ii]->GetWindowText(tmps);
		m_Form[ii]->GetWindowText(form);

		if (text.IsEmpty() || tmps.IsEmpty())
			break;

		data += text;
		data += _T("\t");

		if (m_visible[ii]->GetCheck())
			text.Format(_T("1:%d:%s"), atoi(tmps), form);
		else
			text.Format(_T("0:%d:%s"), atoi(tmps), form);
		str += text;
		str += _T("\t");
	}
	formItem.SetFormStr(m_index, _T("dat"), data);
	formItem.SetFormStr(m_index, _T("str"), str);
	formItem.GetForm(m_index)->keys = (WORD) ii;

	CDialogEx::OnOK();
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

void CTabPage::OnSetFocusID1()		{ setIDIndex(IDC_OPT17); } 
void CTabPage::OnSetFocusID2()		{ setIDIndex(IDC_OPT18); }
void CTabPage::OnSetFocusID3()		{ setIDIndex(IDC_OPT19); }
void CTabPage::OnSetFocusID4()		{ setIDIndex(IDC_OPT20); }
void CTabPage::OnSetFocusID5()		{ setIDIndex(IDC_OPT21); }
void CTabPage::OnSetFocusID6()		{ setIDIndex(IDC_OPT22); }
void CTabPage::OnSetFocusID7()		{ setIDIndex(IDC_OPT23); }
void CTabPage::OnSetFocusID8()		{ setIDIndex(IDC_OPT24); }
void CTabPage::OnSetFocusID9()		{ setIDIndex(IDC_OPT25); }
void CTabPage::OnSetFocusID10()		{ setIDIndex(IDC_OPT26); }
void CTabPage::OnSetFocusID11()		{ setIDIndex(IDC_OPT27); }
void CTabPage::OnSetFocusID12()		{ setIDIndex(IDC_OPT28); }
void CTabPage::OnSetFocusID13()		{ setIDIndex(IDC_OPT29); }
void CTabPage::OnSetFocusID14()		{ setIDIndex(IDC_OPT30); }
void CTabPage::OnSetFocusID15()		{ setIDIndex(IDC_OPT31); }
void CTabPage::OnSetFocusID16()		{ setIDIndex(IDC_OPT32); }

void CTabPage::setIDIndex(int id)
{
	m_sel = id - IDC_OPT17;
}

void CTabPage::OnCheck01()		{ setVisibleIndex(IDC_CHECK01); }
void CTabPage::OnCheck02()		{ setVisibleIndex(IDC_CHECK02); }
void CTabPage::OnCheck03()		{ setVisibleIndex(IDC_CHECK03); }
void CTabPage::OnCheck04()		{ setVisibleIndex(IDC_CHECK04); }
void CTabPage::OnCheck05()		{ setVisibleIndex(IDC_CHECK05); }
void CTabPage::OnCheck06()		{ setVisibleIndex(IDC_CHECK06); }
void CTabPage::OnCheck07()		{ setVisibleIndex(IDC_CHECK07); }
void CTabPage::OnCheck08()		{ setVisibleIndex(IDC_CHECK08); }
void CTabPage::OnCheck09()		{ setVisibleIndex(IDC_CHECK09); }
void CTabPage::OnCheck10()		{ setVisibleIndex(IDC_CHECK10); }
void CTabPage::OnCheck11()		{ setVisibleIndex(IDC_CHECK11); }
void CTabPage::OnCheck12()		{ setVisibleIndex(IDC_CHECK12); }
void CTabPage::OnCheck13()		{ setVisibleIndex(IDC_CHECK13); }
void CTabPage::OnCheck14()		{ setVisibleIndex(IDC_CHECK14); }
void CTabPage::OnCheck15()		{ setVisibleIndex(IDC_CHECK15); }
void CTabPage::OnCheck16()		{ setVisibleIndex(IDC_CHECK16); }

void CTabPage::OnSetFocusForm1()	{ setFormIndex(IDC_OPT33); }
void CTabPage::OnSetFocusForm2()	{ setFormIndex(IDC_OPT34); }
void CTabPage::OnSetFocusForm3()	{ setFormIndex(IDC_OPT35); }
void CTabPage::OnSetFocusForm4()	{ setFormIndex(IDC_OPT36); }
void CTabPage::OnSetFocusForm5()	{ setFormIndex(IDC_OPT37); }
void CTabPage::OnSetFocusForm6()	{ setFormIndex(IDC_OPT38); }
void CTabPage::OnSetFocusForm7()	{ setFormIndex(IDC_OPT39); }
void CTabPage::OnSetFocusForm8()	{ setFormIndex(IDC_OPT40); }
void CTabPage::OnSetFocusForm9()	{ setFormIndex(IDC_OPT41); }
void CTabPage::OnSetFocusForm10()	{ setFormIndex(IDC_OPT42); }
void CTabPage::OnSetFocusForm11()	{ setFormIndex(IDC_OPT43); }
void CTabPage::OnSetFocusForm12()	{ setFormIndex(IDC_OPT44); }
void CTabPage::OnSetFocusForm13()	{ setFormIndex(IDC_OPT45); }
void CTabPage::OnSetFocusForm14()	{ setFormIndex(IDC_OPT46); }
void CTabPage::OnSetFocusForm15()	{ setFormIndex(IDC_OPT47); }
void CTabPage::OnSetFocusForm16()	{ setFormIndex(IDC_OPT48); }

void CTabPage::setFormIndex(int id)
{
	m_sel = id - IDC_OPT33;
}

void CTabPage::OnInsert() 
{
	CString	text;
	CString	ID;
	CString form;
	int	visible;

	m_text[MAX_TAB-1]->SetWindowText("");
	m_ID[MAX_TAB-1]->SetWindowText("");
	m_visible[MAX_TAB-1]->SetCheck(0);
	m_Form[MAX_TAB-1]->SetWindowText("");
	for (int ii = MAX_TAB-1; ii > m_sel; ii--)
	{
		text.Empty();
		ID.Empty();
		visible = 0;
		form.Empty();

		m_text[ii-1]->GetWindowText(text);
		m_ID[ii-1]->GetWindowText(ID);
		visible = m_visible[ii-1]->GetCheck();
		m_Form[ii-1]->GetWindowText(form);

		m_text[ii]->SetWindowText(text);
		m_ID[ii]->SetWindowText(ID);
		m_visible[ii]->SetCheck(visible);
		m_Form[ii]->SetWindowText(form);

		m_text[ii-1]->SetWindowText("");
		m_ID[ii-1]->SetWindowText("");
		m_visible[ii-1]->SetCheck(0);
		m_Form[ii-1]->SetWindowText("");
	}
	m_text[m_sel]->SetFocus();
}

void CTabPage::OnDelete() 
{
	CString	text;
	CString	ID;
	CString form;
	int	visible;

	m_text[m_sel]->SetWindowText("");
	m_ID[m_sel]->SetWindowText("");
	m_visible[m_sel]->SetCheck(0);
	m_Form[m_sel]->SetWindowText("");

	for (int ii = m_sel; ii < MAX_TAB-1; ii++)
	{
		text.Empty();
		ID.Empty();
		visible = 0;

		m_text[ii+1]->GetWindowText(text);
		m_ID[ii+1]->GetWindowText(ID);
		visible = m_visible[ii+1]->GetCheck();
		m_Form[ii+1]->GetWindowText(form);

		m_text[ii+1]->SetWindowText("");
		m_ID[ii+1]->SetWindowText("");
		m_visible[ii+1]->SetCheck(0);
		m_Form[ii+1]->SetWindowText("");
		
		m_text[ii]->SetWindowText(text);
		m_ID[ii]->SetWindowText(ID);
		m_visible[ii]->SetCheck(visible);
		m_Form[ii]->SetWindowText(form);
	}
	m_text[m_sel]->SetFocus();
}

void CTabPage::setVisibleIndex(int id)
{
	m_sel = id - IDC_CHECK01;
}

CString CTabPage::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src.Empty();
		return tmps;
	}
	else
	{
		tmps = src.Left(pos);
		src = src.Mid(pos + sub.GetLength());
		return tmps;
	}
	return _T("");
}