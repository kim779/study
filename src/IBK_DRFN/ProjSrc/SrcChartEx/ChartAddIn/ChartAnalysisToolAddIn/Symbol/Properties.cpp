// Properties.cpp : implementation file
//

#include "stdafx.h"
#include "symbol.h"
#include "Properties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PropertiesData_text.h"
/////////////////////////////////////////////////////////////////////////////
// CProperties dialog


CProperties::CProperties(CPropertiesData_text* pProperties, const CSymbolBaseData::TOOLTYPE SymbolTool, CWnd* pParent /*=NULL*/)
	: CDialog(CProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProperties)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pProperties = pProperties;
	m_eSymbolTool = SymbolTool;
}

void CProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProperties)
	DDX_Control(pDX, IDC_COLOR, m_Color_Bt);
	DDX_Control(pDX, IDC_PREVIEW, m_PreView_Static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProperties, CDialog)
	//{{AFX_MSG_MAP(CProperties)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_CBN_SELCHANGE(IDC_SYMBOL_SIZE, OnSelchangeSymbolSize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProperties message handlers

void CProperties::OnColor() 
{
	CColorDialog color;
	if(color.DoModal() == IDOK) {
		m_pProperties->SetColor(color.GetColor());
		m_Color_Bt.SetColor(m_pProperties->GetColor());
		m_Color_Bt.Invalidate();
		m_PreView_Static.ModifySymbolType(m_eSymbolTool);
	}
}

void CProperties::OnFont() 
{
	//sy 2005.04.06
	if(m_pProperties == NULL)
		return;

	CFontDialog fontDlg(&m_pProperties->GetLogFont());
	fontDlg.m_cf.rgbColors = m_pProperties->GetTextColor();
	if(fontDlg.DoModal() == IDOK)
	{
		fontDlg.GetCurrentFont(&m_pProperties->GetLogFont());
		m_pProperties->SetTextColor(fontDlg.GetColor());
		m_PreView_Static.ModifySymbolType(m_eSymbolTool);
	}	
}

void CProperties::OnSelchangeSymbolSize() 
{
	CComboBox* pcombo = (CComboBox* )GetDlgItem(IDC_SYMBOL_SIZE);
	m_pProperties->SetSizeType(CSymbolBaseData::SIZETYPE(pcombo->GetCurSel()));
	m_PreView_Static.ModifySymbolType(m_eSymbolTool);
}

BOOL CProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//color
	m_Color_Bt.SetColor(m_pProperties->GetColor());
	//size
	CComboBox* pCombo = (CComboBox* )GetDlgItem(IDC_SYMBOL_SIZE);
	pCombo->SetCurSel(m_pProperties->GetSizeType());
	//미리보기
	m_PreView_Static.SetPropertiesData(m_pProperties); 
	m_PreView_Static.ModifySymbolType(m_eSymbolTool);

	//sy 2005.04.06
	EnableWindow_FontBt();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//sy 2005.04.06
void CProperties::SetBackgroundColorInViewer(const COLORREF& color)
{
	m_PreView_Static.SetBackgroundColor(color);
}

//sy 2005.04.06
void CProperties::EnableWindow_FontBt()
{
	if(DoesUseFontSymbol())
		return;

	CWnd* pItem = GetDlgItem(IDC_FONT);
	if(pItem != NULL)
		pItem->EnableWindow(false);
}

//sy 2005.04.06
bool CProperties::DoesUseFontSymbol() const
{
	switch(m_eSymbolTool)
	{
	case CSymbolBaseData::SYMBOL_ARROW_LEFT:
	case CSymbolBaseData::SYMBOL_ARROW_RIGHT:
	case CSymbolBaseData::SYMBOL_ARROW_UP:
	case CSymbolBaseData::SYMBOL_ARROW_DOWN:
	case CSymbolBaseData::SYMBOL_ARROW_LEFTTOP:
	case CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM:
	case CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP:
	case CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM:
		return false;
	}

	return true;
}
