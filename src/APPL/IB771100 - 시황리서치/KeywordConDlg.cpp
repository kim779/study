// KeywordConDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "KeywordConDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeywordConDlg dialog


CKeywordConDlg::CKeywordConDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeywordConDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeywordConDlg)
	m_strKey = _T("");
	//}}AFX_DATA_INIT
}


void CKeywordConDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeywordConDlg)
	DDX_Control(pDX, IDC_ADDCOMBO, m_cboAdd);
	DDX_Control(pDX, IDC_STEPCOMBO, m_cboStp);
	DDX_Control(pDX, IDC_STEPLIST, m_stepList);
	DDX_Control(pDX, IDC_KEYLIST, m_KeyList);
	DDX_Control(pDX, IDC_COLORBUTTON, m_btColor);
	DDX_Text(pDX, IDC_KEYEDIT, m_strKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeywordConDlg, CDialog)
	//{{AFX_MSG_MAP(CKeywordConDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_CBN_SELCHANGE(IDC_STEPCOMBO, OnSelchangeStepcombo)
	ON_BN_CLICKED(IDC_OKBUTTON, OnOkbutton)
	ON_BN_CLICKED(IDC_CANCELBUTTON, OnCancelbutton)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_LBN_SELCHANGE(IDC_KEYLIST, OnSelchangeKeylist)
	ON_LBN_SELCHANGE(IDC_STEPLIST, OnSelchangeSteplist)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnChangeColorPicker)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeywordConDlg message handlers

BOOL CKeywordConDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	for(int i=0; i<m_arKeyList.GetSize();i++) 
	{
		m_stepList.InsertString(i,  CStr( m_arKeyList.GetAt(i).iVal) + "단계");  
		m_KeyList.InsertString(i,  m_arKeyList.GetAt(i).sTxt );
	}
	// TODO: Add extra initialization here
	CString stmp;
	for(int i=0; i<m_arStepColor.GetSize();i++) 
	{
		stmp =  CStr(i+1) +"단계";
		m_cboAdd.InsertString(i, stmp); 
		m_cboStp.InsertString(i, stmp); 
	}

	if(m_arStepColor.GetSize() > 0)
	{
		m_cboStp.SetCurSel(0); 
		m_btColor.SetColor(m_arStepColor[0]); 
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeywordConDlg::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	const int iadd = m_KeyList.GetCount();
	const int istep = m_cboAdd.GetCurSel()+1;
	for(int i=0; i<m_arKeyList.GetSize() ; i++)
	{
		if(m_arKeyList.GetAt(i).iVal ==  istep)
		{
			OnButtonEdit();
			return;
		}
	}

	CString strStep = CStr(istep ) + "단계";
	m_KeyList.InsertString(iadd, m_strKey );
	m_stepList.InsertString(iadd, strStep); 
	m_arKeyList.InsertAt(iadd, AList(m_strKey, istep));
}

void CKeywordConDlg::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	int idel = m_KeyList.GetCurSel() ;
	if(idel < 0) 
	{
		idel = m_stepList.GetCurSel() ;
		if(idel < 0) return;
	}

	m_KeyList.DeleteString(idel); 
	m_stepList.DeleteString(idel);

	m_arKeyList.RemoveAt(idel);
}

void CKeywordConDlg::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iedit = m_KeyList.GetCurSel() ;
	if(iedit < 0) 
	{
		iedit = m_stepList.GetCurSel() ;
		if(iedit < 0) return;
	}
	m_KeyList.DeleteString(iedit); 
	m_stepList.DeleteString(iedit);

	const int istep = m_cboAdd.GetCurSel()+1;
	m_KeyList.InsertString(iedit, m_strKey );
	m_stepList.InsertString(iedit, CStr(istep ) + "단계"); 
	m_arKeyList.SetAt(iedit, AList(m_strKey, istep));

	
}

LONG CKeywordConDlg::OnChangeColorPicker(UINT lParam, LONG wParam)
{
	const int iedit = m_cboStp.GetCurSel();
	if( iedit < 0) return 0;

	const COLORREF SelColor = (DWORD)lParam;
	if(m_arStepColor.GetSize() > iedit) 
		m_arStepColor.SetAt(iedit,  SelColor);
	else 
		m_arStepColor.Add(SelColor);
	

	return 0;
}

void CKeywordConDlg::OnSelchangeStepcombo() 
{
	// TODO: Add your control notification handler code here
	const int idx = m_cboStp.GetCurSel();
	if(idx > -1 && m_arStepColor.GetSize() > idx) 
		m_btColor.SetColor( m_arStepColor.GetAt(idx) ); 
}

void CKeywordConDlg::OnOkbutton() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK(); 
}

void CKeywordConDlg::OnCancelbutton() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel() ;
}


void CKeywordConDlg::OnSelchangeKeylist() 
{
	// TODO: Add your control notification handler code here
	int idx = m_KeyList.GetCurSel(); 
	if(idx < 0) return;

	m_KeyList.GetText(idx,	m_strKey);
	GetDlgItem(IDC_KEYEDIT)->SetWindowText(m_strKey);

	CString stmp;
	m_stepList.GetText(idx,	stmp);
	idx = atoi(stmp.Left(1) )-1;
	m_cboAdd.SetCurSel(idx); 
}

void CKeywordConDlg::OnSelchangeSteplist() 
{
	// TODO: Add your control notification handler code here
	int idx = m_stepList.GetCurSel(); 
	if(idx < 0) return;
	m_KeyList.GetText(idx,	m_strKey);
	GetDlgItem(IDC_KEYEDIT)->SetWindowText(m_strKey);

	CString stmp;
	m_stepList.GetText(idx,	stmp);
	idx = atoi(stmp.Left(1) )-1;
	m_cboAdd.SetCurSel(idx); 
	
}
