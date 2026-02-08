// ToolandRButtonPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ToolandRButtonPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "../Include_Chart/xUtil_Chart.h"				// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "ChartPropertySheet.h"
#include "PropertiesData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonPage property page

IMPLEMENT_DYNCREATE(CToolandRButtonPage, CPropertyPage)

CToolandRButtonPage::CToolandRButtonPage() : CPropertyPage(CToolandRButtonPage::IDD)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;

	//{{AFX_DATA_INIT(CToolandRButtonPage)
	//}}AFX_DATA_INIT

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;
}

CToolandRButtonPage::~CToolandRButtonPage()
{
}

void CToolandRButtonPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolandRButtonPage)
	DDX_Control(pDX, IDC_TAB1, m_ToolRButton_Tab);
	DDX_Control(pDX, IDC_TOOL_LIST, m_ToolRButtonItem_Ctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolandRButtonPage, CPropertyPage)
	//{{AFX_MSG_MAP(CToolandRButtonPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonPage message handlers

void CToolandRButtonPage::InitBaseList(CListCtrl* pListCtrl)
{
	m_ToolRButtonItem_Ctrl.SetExtendedStyle(LVS_EX_GRIDLINES);

	CRect rect;
	m_ToolRButtonItem_Ctrl.GetWindowRect(&rect);
	
	//insert column
	LV_COLUMN	lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = rect.Width() - 20;
	lvColumn.pszText = _T("Chart List");
	pListCtrl->InsertColumn(0, &lvColumn);

	//insert Item.
	for(int i = 0 ; i < IMetaTable::m_nToolOptionCount; i++)
	{
		LV_ITEM listItem;
		listItem.mask = LVIF_TEXT;
		listItem.iSubItem = 0;
		listItem.iItem = i;
		listItem.pszText = ( char *)_LTEXT4( m_eToolOptions[i]);    
		pListCtrl->InsertItem(&listItem);
	}	
}

BOOL CToolandRButtonPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	InitBaseList(&m_ToolRButtonItem_Ctrl);
	m_ToolRButton_Tab.InitTab();
	m_ToolRButton_Tab.SetSavedDataToDlgwithListCtrl();

	m_ToolRButtonItem_Ctrl.InitDropTarget();
	m_ToolRButton_Tab.InitDropTarget();

	m_ToolRButton_Tab.SetTabFocus();

	CreateToolTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolandRButtonPage::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_1);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_2);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_3);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_4);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_5);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_6);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_7);
	dwaInfo.Add( IDS_TOOLLIST_TOOLTIP_8);
	m_toolTip.AddControlInfo( IDC_TOOL_LIST, dwaInfo);
}

BOOL CToolandRButtonPage::PreTranslateMessage(MSG* pMsg) 
{
	bool bRButton;
	bRButton = false;

	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN )	{
		m_toolTip.ShowToolTip( (CPoint)pt );
		bRButton = true;
	}
	if(pMsg->message == WM_MOUSEMOVE){
		m_toolTip.HideToolTip( (CPoint)pt );
	}

	if(bRButton)
		return TRUE;
	else
		return CPropertyPage::PreTranslateMessage(pMsg);
}

//save Received data From ocx
void CToolandRButtonPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	int nNumericalInquiryOption;
	CString strToolBarOption;

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRTOOLBAROPTION		], strToolBarOption,		_DPV_STRTOOLBAROPTION		);
		CHART_CONVERT_INT(		p_pIPropertyMap, g_anPropertyShortKey[	NUMERICALINQUIRYOPTION	], nNumericalInquiryOption,	_DPV_NUMERICALINQUIRYOPTION	);	// ADD: 종찬(04/03/17) 수치조회관련
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;

	// ADD: 종찬(04/03/17) 수치조회관련 옵션 추가
	m_ToolRButton_Tab.SetSavedDataFromParent( strToolBarOption);
	m_ToolRButton_Tab.SetSavedDataFromParent( nNumericalInquiryOption);
}

BOOL CToolandRButtonPage::OnApply() 
{
	int		nNumericalInquiryOption	= m_ToolRButton_Tab.GetNumericalInquiryOption();
	CString	strToolBarOption		= m_ToolRButton_Tab.GetListItemInfo();

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRTOOLBAROPTION		], strToolBarOption,		_DPV_STRTOOLBAROPTION		);
		CHART_ADDTOSTRLIST_INT(		m_pIPropertyMap, g_anPropertyShortKey[	NUMERICALINQUIRYOPTION	], nNumericalInquiryOption,	_DPV_NUMERICALINQUIRYOPTION	);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}

void CToolandRButtonPage::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_ToolRButton_Tab.SetOcxHwnd( p_hOcxWnd);
	m_toolTip.SetOcxHwnd( p_hOcxWnd);
}