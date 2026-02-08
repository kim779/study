// Properties_text.cpp : implementation file
//

#include "stdafx.h"
#include "symbol.h"
#include "Properties_text.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PropertiesData_text.h"
/////////////////////////////////////////////////////////////////////////////
// CProperties_text dialog


CProperties_text::CProperties_text(CPropertiesData_text* pProperties, CWnd* pParent /*=NULL*/)
	: CDialog(CProperties_text::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProperties_text)
	//}}AFX_DATA_INIT
	m_pProperties = pProperties;
}


void CProperties_text::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperties_text)
	DDX_Control(pDX, IDC_TEXT, m_Text_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperties_text, CDialog)
	//{{AFX_MSG_MAP(CProperties_text)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_EN_CHANGE(IDC_TEXT, OnChangeText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperties_text message handlers

void CProperties_text::OnFont() 
{
	//sy 2005.03.30.
	if(m_pProperties == NULL)
		return;

	CFontDialog fontDlg(&m_pProperties->GetLogFont());
	fontDlg.m_cf.rgbColors = m_pProperties->GetTextColor();
	if(fontDlg.DoModal() == IDOK)
	{
		fontDlg.GetCurrentFont(&m_pProperties->GetLogFont());
		m_pProperties->SetTextColor(fontDlg.GetColor());
	}	
}

BOOL CProperties_text::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//미리보기
	m_Text_Edit.SetWindowText(m_pProperties->GetText());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString CProperties_text::GetModifyText()
{
	CString text;
	m_Text_Edit.GetWindowText(text);
	return text;
}

void CProperties_text::OnChangeText() 
{
	CString text;
	m_Text_Edit.GetWindowText(text);
	m_pProperties->SetText(text);
}
