// cbItem.cpp : implementation file
//

#include "stdafx.h"
#include "cbItem.h"

//#include "../form.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboItem dialog

CComboItem::CComboItem(CWnd* pParent, int count, int size)
	: CDialog(CComboItem::IDD, pParent)
{
	m_count = count;
	m_size  = size;

	//{{AFX_DATA_INIT(CComboItem)
	m_edata = _T("");
	m_edisplay = _T("");
	//}}AFX_DATA_INIT
}


void CComboItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComboItem)
	DDX_Control(pDX, IDC_DATA, m_data);
	DDX_Control(pDX, IDC_DISPLAY, m_display);
	DDX_Text(pDX, IDC_IDATA, m_edata);
	DDX_Text(pDX, IDC_IDISPLAY, m_edisplay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComboItem, CDialog)
	//{{AFX_MSG_MAP(CComboItem)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_IDELETE, OnDelete)
	ON_LBN_SELCHANGE(IDC_DISPLAY, OnSelChangeDisplay)
	ON_LBN_DBLCLK(IDC_DISPLAY, OnDblClkDisplay)
	ON_LBN_SELCHANGE(IDC_DATA, OnSelChangeData)
	ON_LBN_DBLCLK(IDC_DATA, OnDblClkData)
	ON_BN_CLICKED(IDC_IDUP, OnIdup)
	ON_BN_CLICKED(IDC_IDDOWN, OnIddown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboItem message handlers

BOOL CComboItem::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	str = m_disB;
	int	idx = 0;

	for (int ii = 0; ii < m_count; ii++)
		m_display.AddString(Parser(str, "\t"));

	str = m_datB;
	for (int ii = 0; ii < m_count; ii++)
		m_data.AddString(Parser(str, "\t"));

	if (m_count != 0)
	{
		m_sel = 0;
		m_display.SetCurSel(m_sel);
		m_data.SetCurSel(m_sel);
	}

	CEdit*	edit = (CEdit *)GetDlgItem(IDC_IDATA);
	edit->LimitText(m_size);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComboItem::OnInsert() 
{
	UpdateData(TRUE);
	if (m_edisplay.IsEmpty() || m_edata.IsEmpty())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Fill the required field.", "WARNING", MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	if (m_size != m_edata.GetLength())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Data length is difference.", "WARNING", MB_ICONEXCLAMATION|MB_OK);
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
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Fill the required field.", "WARNING", MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	if (m_size != m_edata.GetLength())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Data length is difference.", "WARNING", MB_ICONEXCLAMATION|MB_OK);
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

void CComboItem::OnAdd() 
{
	UpdateData(TRUE);
	if (m_edisplay.IsEmpty() || m_edata.IsEmpty())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Fill the required field.", "WARNING", MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	if (m_size != m_edata.GetLength())
	{
		::MessageBeep(0xFFFFFFFF);
		::MessageBox(AfxGetMainWnd()->m_hWnd, "Data length is difference.", "WARNING", MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	m_display.AddString(m_edisplay);
	m_data.AddString(m_edata);
	m_edisplay.Empty();
	m_edata.Empty();
	GetDlgItem(IDC_IDISPLAY)->SetFocus();

	UpdateData(FALSE);
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

void CComboItem::OnSelChangeDisplay() 
{
	m_sel = m_display.GetCurSel();
	m_data.SetCurSel(m_sel);

	GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);	
}

void CComboItem::OnDblClkDisplay() 
{
	m_sel = m_display.GetCurSel();

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

void CComboItem::OnDblClkData() 
{
	m_sel = m_data.GetCurSel();

	m_display.GetText(m_sel, m_edisplay);
	m_data.GetText(m_sel, m_edata);

	UpdateData(FALSE);
}

void CComboItem::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CComboItem::OnOK() 
{
	CString	disS, datS;

	m_count = (BYTE)m_display.GetCount();

	m_datB.Empty(); m_disB.Empty();
	for (int ii = 0; ii < m_count; ii++)
	{
		m_display.GetText(ii, disS);
		m_data.GetText(ii, datS);

		m_disB += disS;
		m_disB += "\t";
			
		m_datB += datS;
		m_datB += "\t";
	}
	
	CDialog::OnOK();
}

CString CComboItem::Parser(CString &srcstr, CString substr)
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

void CComboItem::OnIdup() 
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

void CComboItem::OnIddown() 
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
