// SearchWordSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB771000.h"
#include "SearchWordSetDlg.h"
#include "MapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchWordSetDlg dialog


CSearchWordSetDlg::CSearchWordSetDlg(CMapDlg *mapdlg)
	: CDialog(CSearchWordSetDlg::IDD, NULL)
{
	m_pMapDlg = mapdlg;
	m_pSearchWordArray = NULL;
	//{{AFX_DATA_INIT(CSearchWordSetDlg)
	m_edWord = _T("");
	//}}AFX_DATA_INIT
}


void CSearchWordSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearchWordSetDlg)
	DDX_Control(pDX, IDC_LIST_WORD, m_listWord);
	DDX_Text(pDX, IDC_EDIT_WORD, m_edWord);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_Btn[0]);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_Btn[1]);
	DDX_Control(pDX, IDOK, m_Btn[2]);
	DDX_Control(pDX, IDCANCEL, m_Btn[3]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearchWordSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSearchWordSetDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearchWordSetDlg message handlers

void CSearchWordSetDlg::OnButtonAdd() 
{
	UpdateData();
	
	if( m_edWord == "")
		return;

	m_listWord.AddString(m_edWord);
	m_edWord = "";
	UpdateData(false);
	
}

void CSearchWordSetDlg::OnButtonDel() 
{
	UpdateData();

	int selidx = 0;
	if( m_edWord == "")
	{
		selidx = m_listWord.GetCurSel();
	}else{
		CString szTemp;
		for(int i = 0 ; i < m_listWord.GetCount() ; i ++){
			m_listWord.GetText(i,szTemp);
			if( szTemp == m_edWord)
			{
				selidx = i;
				break;
			}
		}
	}

	m_listWord.DeleteString(selidx);
	m_edWord = "";

	UpdateData(false);
}

void CSearchWordSetDlg::SetWordArray(CStringArray * array){

	m_pSearchWordArray= array ;
	
}

BOOL CSearchWordSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!m_pSearchWordArray)
		return false;
	
	for(int i = 0 ; i < m_pSearchWordArray->GetSize()	; i++){
		m_listWord.AddString(m_pSearchWordArray->GetAt(i));
	}

	for(int i = 0 ; i < 4; i++){
	
//		m_Btn[i].setGradient(true);
		m_Btn[i].SetFont( m_pMapDlg->GetAxFont(9,false,"±¼¸²Ã¼"));
	}

	SetBrush(m_pMapDlg->GetAxColor(64));

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearchWordSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	DrawBkgrd(&dc,rc);
}

HBRUSH CSearchWordSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	
	if( nCtlColor == CTLCOLOR_STATIC  ) 
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor( m_pMapDlg->GetAxColor(69) );
		return (HBRUSH)m_brBk;
	}
	return hbr;
}
void CSearchWordSetDlg::OnOK() 
{
	if( m_pSearchWordArray )
	{
		m_pSearchWordArray->RemoveAll();
		CString szTemp = "";
		for(int i = 0 ; i < m_listWord.GetCount() ; i++){
			m_listWord.GetText( i, szTemp);
			m_pSearchWordArray->Add(szTemp);
		}
	}
	CDialog::OnOK();
}
