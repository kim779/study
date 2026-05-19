// comboItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "comboItem.h"
#include "afxdialogex.h"


// CComboItem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CComboItem, CDialogEx)

CComboItem::CComboItem(CWnd* pParent, int count, int size)
	: CDialogEx(CComboItem::IDD, pParent)
{
	m_count = count;
	m_size  = size;

	m_edata = _T("");
	m_edisplay = _T("");
}

CComboItem::~CComboItem()
{
}

void CComboItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA, m_data);
	DDX_Control(pDX, IDC_DISPLAY, m_display);
	DDX_Text(pDX, IDC_IDATA, m_edata);
	DDX_Text(pDX, IDC_IDISPLAY, m_edisplay);
}


BEGIN_MESSAGE_MAP(CComboItem, CDialogEx)
	ON_BN_CLICKED(IDC_ADD, &CComboItem::OnAdd)
	ON_LBN_DBLCLK(IDC_DATA, &CComboItem::OnDblClkData)
	ON_LBN_SELCHANGE(IDC_DATA, &CComboItem::OnSelChangeData)
	ON_LBN_DBLCLK(IDC_DISPLAY, &CComboItem::OnDblClkDisplay)
	ON_LBN_SELCHANGE(IDC_DISPLAY, &CComboItem::OnSelChangeDisplay)
	ON_BN_CLICKED(IDC_IDELETE, &CComboItem::OnDelete)
	ON_BN_CLICKED(IDC_IDDOWN, &CComboItem::OnDown)
	ON_BN_CLICKED(IDC_IDUP, &CComboItem::OnUp)
	ON_BN_CLICKED(IDC_INSERT, &CComboItem::OnInsert)
	ON_BN_CLICKED(IDC_MODIFY, &CComboItem::OnModify)
END_MESSAGE_MAP()


// CComboItem 메시지 처리기입니다.


BOOL CComboItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString	str = m_disB;
	int	idx = 0;
	for (int ii = 0; ii < m_count; ii++)
		m_display.AddString(parse(str, _T("\t")));

	str = m_datB;
	for (int ii = 0; ii < m_count; ii++)
		m_data.AddString(parse(str, _T("\t")));

	if (m_count != 0)
	{
		m_sel = 0;
		m_display.SetCurSel(m_sel);
		m_data.SetCurSel(m_sel);
	}

	((CEdit *)GetDlgItem(IDC_IDATA))->LimitText(m_size);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CComboItem::OnOK()
{
	CString	tmps;

	m_count = m_display.GetCount();
	m_datB.Empty();
	m_disB.Empty();
	for (int ii = 0; ii < m_count; ii++)
	{
		m_display.GetText(ii, tmps);
		m_disB += tmps;
		m_disB += _T("\t");

		m_data.GetText(ii, tmps);
		m_datB += tmps;
		m_datB += _T("\t");
	}
	
	CDialogEx::OnOK();
}


void CComboItem::OnAdd()
{
	UpdateData(TRUE);
	if (m_edisplay.IsEmpty() || m_edata.IsEmpty())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Fill the required field."), _T("WARNING"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	if (m_size != m_edata.GetLength())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Data length is different."), _T("WARNING"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	m_display.AddString(m_edisplay);
	m_data.AddString(m_edata);
	m_edisplay.Empty();
	m_edata.Empty();
	GetDlgItem(IDC_IDISPLAY)->SetFocus();

	UpdateData(FALSE);
}


void CComboItem::OnDblClkData()
{
	m_sel = m_data.GetCurSel();

	m_display.GetText(m_sel, m_edisplay);
	m_data.GetText(m_sel, m_edata);

	UpdateData(FALSE);
}


void CComboItem::OnSelChangeData()
{
	m_sel = m_data.GetCurSel();
	m_display.SetCurSel(m_sel);
	GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);
}


void CComboItem::OnDblClkDisplay()
{
	m_sel = m_display.GetCurSel();

	m_display.GetText(m_sel, m_edisplay);
	m_data.GetText(m_sel, m_edata);

	UpdateData(FALSE);
}


void CComboItem::OnSelChangeDisplay()
{
	m_sel = m_display.GetCurSel();
	m_data.SetCurSel(m_sel);
	GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);	
}


void CComboItem::OnDelete()
{
	m_display.DeleteString(m_sel);
	m_data.DeleteString(m_sel);

	if (m_display.GetCount() > 0)
	{
		m_sel = 0;
		m_display.SetCurSel(m_sel);
		m_data.SetCurSel(m_sel);
	}
	else
		GetDlgItem(IDC_MODIFY)->EnableWindow(FALSE);
}


void CComboItem::OnDown()
{
	int nIdx = m_display.GetCurSel();
	if (nIdx == LB_ERR || nIdx == m_display.GetCount() - 1)
		return;
	
	CString sDis, sDat;

	m_display.GetText(nIdx, sDis);
	m_data.GetText(nIdx, sDat);

	m_display.DeleteString(nIdx);
	m_data.DeleteString(nIdx);

	m_display.InsertString(++nIdx, sDis);
	m_data.InsertString(nIdx, sDat);
	
	m_display.SetCurSel(nIdx);
	m_data.SetCurSel(nIdx);
}


void CComboItem::OnUp()
{
	int nIdx = m_display.GetCurSel();
	if (nIdx == LB_ERR || nIdx == 0)
		return;
	
	CString sDis, sDat;

	m_display.GetText(nIdx, sDis);
	m_data.GetText(nIdx, sDat);

	m_display.DeleteString(nIdx);
	m_data.DeleteString(nIdx);

	m_display.InsertString(--nIdx, sDis);
	m_data.InsertString(nIdx, sDat);
	
	m_display.SetCurSel(nIdx);
	m_data.SetCurSel(nIdx);
}


void CComboItem::OnInsert()
{
	UpdateData(TRUE);
	if (m_edisplay.IsEmpty() || m_edata.IsEmpty())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Fill the required field."), _T("WARNING"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	if (m_size != m_edata.GetLength())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Data length is different."), _T("WARNING"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	m_display.InsertString(m_sel, m_edisplay);
	m_data.InsertString(m_sel, m_edata);
	m_edisplay.Empty();
	m_edata.Empty();
	GetDlgItem(IDC_IDISPLAY)->SetFocus();

	UpdateData(FALSE);
}


void CComboItem::OnModify()
{
	UpdateData(TRUE);
	if (m_edisplay.IsEmpty() || m_edata.IsEmpty())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Fill the required field."), _T("WARNING"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	if (m_size != m_edata.GetLength())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, _T("Data length is different."), _T("WARNING"), MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	m_display.DeleteString(m_sel);
	m_data.DeleteString(m_sel);
	m_display.InsertString(m_sel, m_edisplay);
	m_data.InsertString(m_sel, m_edata);

	m_edisplay.Empty();
	m_edata.Empty();
	GetDlgItem(IDC_IDISPLAY)->SetFocus();

	UpdateData(FALSE);
}

CString CComboItem::parse(CString &src, CString sub)
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