// AddInLoadInfoPage.cpp : implementation file
//

#include "stdafx.h"
#include "chartdesignpropertyaddin.h"
#include "AddInLoadInfoPage.h"

#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "../Include_Chart/PropertiesData.h"	// for g_anPropertyShortKey

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddInLoadInfoPage property page

IMPLEMENT_DYNCREATE(CAddInLoadInfoPage, CPropertyPage)

CAddInLoadInfoPage::CAddInLoadInfoPage() : CPropertyPage(CAddInLoadInfoPage::IDD)
{
	//{{AFX_DATA_INIT(CAddInLoadInfoPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
	for( int i = 0; i < AID_COUNT; i++) m_mapAddInLoadString.SetAt( g_aszAddInItemName[ i], g_aszAddInLoadString[ i]);
}

CAddInLoadInfoPage::~CAddInLoadInfoPage()
{
	// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
	m_mapAddInLoadString.RemoveAll();
}

void CAddInLoadInfoPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddInLoadInfoPage)
	DDX_Control(pDX, IDC_STC_COMMENT, m_stcComment);
	DDX_Control(pDX, IDC_LST_ADDIN, m_lstAddInItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddInLoadInfoPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAddInLoadInfoPage)
	ON_BN_CLICKED(IDC_BTN_SELECT_ALL, OnBtnSelectAll)
	ON_BN_CLICKED(IDC_BTN_CLEAR_ALL, OnBtnClearAll)
	ON_BN_CLICKED(IDC_BTN_RESET_UNDO, OnBtnResetUndo)
	ON_BN_CLICKED(IDC_BTN_RESET_DEFAULT, OnBtnResetDefault)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_ADDIN, OnItemchangedLstAddin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddInLoadInfoPage message handlers

BOOL CAddInLoadInfoPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	// 1. Init Control
	m_lstAddInItem.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES);
	m_lstAddInItem.InsertColumn( 0, "AddIn IDs",	LVCFMT_CENTER,	120);
	m_lstAddInItem.InsertColumn( 1, "DLL Name",		LVCFMT_LEFT,	100);

	// 2. Set AddIn Data
	CString strAddInNameList, strAddInLoadString;
	char *szAddInNameList = strAddInNameList.GetBufferSetLength( strlen( _ADDIN_LOAD_ORDER));
	strcpy( szAddInNameList, _ADDIN_LOAD_ORDER);
	char *szItem = szAddInNameList + 7;
	int nItemLen = 0;				// Not Used
	int nItemOver = 0;				// Over Code : 0(Under), 1(Last Under), 2(Over)
	char *szItemNext = NULL;
	AddInNameListParse( szItem, szItemNext, &nItemLen, &nItemOver);
	while( *szItem)
	{
		m_lstAddInItem.InsertItem( 0, szItem);
		if( m_mapAddInLoadString.Lookup( szItem, strAddInLoadString))
		{
			int nIndex = strAddInLoadString.Find( ':');
			if( 0 <= nIndex) strAddInLoadString = strAddInLoadString.Left( nIndex);
		}
		else strAddInLoadString.Empty();
		m_lstAddInItem.SetItemText( 0, 1, strAddInLoadString);
		AddInNameListParse( szItem, szItemNext, &nItemLen, &nItemOver);
	}
	
	// 3. Set Map Setting
	SetAddInNameListWith( m_strAddInNameList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// (2006/8/24 - Seung-Won, Bae)  Set Received Value From ocx
void CAddInLoadInfoPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	ADDINLIST	], m_strAddInNameList,	_DPV_ADDINLIST	);
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

BOOL CAddInLoadInfoPage::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class

	CString strAddInNameList;
	char *szAddInNameList = strAddInNameList.GetBufferSetLength( strlen( _ADDIN_LOAD_ORDER));
	// 1. Version Field
	strncpy( szAddInNameList, _ADDIN_LOAD_ORDER, 7);
	szAddInNameList += 7;
	// 2. Get Selection
	BOOL bOverTool = TRUE;
	CString strAddInName;
	int nCount = m_lstAddInItem.GetItemCount();
	for( int i = nCount - 1; 0 <= i; i--) if( m_lstAddInItem.GetCheck( i))
	{
		if( bOverTool && i < _ADDIN_MARK_OVER_COUNT)
		{
			if( *( szAddInNameList - 1) == ',') *( szAddInNameList - 1) = ':';
			else
			{
				*szAddInNameList = ':';
				szAddInNameList++;
			}

			bOverTool = FALSE;
		}

		strAddInName = m_lstAddInItem.GetItemText( i, 0);
		strcpy( szAddInNameList, strAddInName);
		szAddInNameList += strAddInName.GetLength();
		*szAddInNameList = ',';
		szAddInNameList++;
	}
	if( bOverTool)
	{
		if( *( szAddInNameList - 1) == ',') *( szAddInNameList - 1) = ':';
		else
		{
			*szAddInNameList = ':';
			szAddInNameList++;
		}
	}
	if( *( szAddInNameList - 1) == ',') *( szAddInNameList - 1) = '\0';
	else *szAddInNameList = '\0';

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	ADDINLIST	], strAddInNameList,	_DPV_ADDINLIST	);
	CHART_ADDTOSTRLIST_END
	
	return CPropertyPage::OnApply();
}

BOOL CAddInLoadInfoPage::AddInNameListParse( char *&szItem, char *&szItemNext, int *pnItemLength, int *pnItemOver)
{
	if( pnItemOver) if( *pnItemOver == 1) *pnItemOver = 2;

	// 1. Check first Item
	if( !szItemNext)
	{
		szItemNext = szItem;
		while( *szItemNext != ',' && *szItemNext != ':' && *szItemNext != '\0') szItemNext++;
		if( pnItemLength) *pnItemLength = szItemNext - szItem;
		if( *szItemNext)
		{
			if( pnItemOver) if( *szItemNext == ':') *pnItemOver = 1;
			*szItemNext = '\0';
			szItemNext++;
		}

		// 2. Check First Over Item
		if( *szItem)		return TRUE;																// found UnderItem
		if( *szItemNext)	return AddInNameListParse( szItem, szItemNext, pnItemLength, pnItemOver);	// find OverItem
		return TRUE;																					// found No Item
	}

	// 3. Get Next Item
	szItem = szItemNext;
	if( !*szItemNext) return TRUE;	// No next Item.
	while( *szItemNext != ',' && *szItemNext != ':' && *szItemNext != '\0') szItemNext++;
	if( pnItemLength) *pnItemLength = szItemNext - szItem;
	if( *szItemNext)
	{
		if( pnItemOver) if( *szItemNext == ':') *pnItemOver = 1;
		*szItemNext = '\0';
		szItemNext++;
	}
	return TRUE;					// found next Item.
}

void CAddInLoadInfoPage::OnBtnSelectAll() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstAddInItem.GetItemCount();
	for( int i = 0; i < nCount; i++) m_lstAddInItem.SetCheck( i);
}

void CAddInLoadInfoPage::OnBtnClearAll() 
{
	// TODO: Add your control notification handler code here
	int nCount = m_lstAddInItem.GetItemCount();
	for( int i = 0; i < nCount; i++) m_lstAddInItem.SetCheck( i, FALSE);
}

void CAddInLoadInfoPage::SetAddInNameListWith( const char *p_szAddInNameList)
{
	if( !p_szAddInNameList) return;
	if( !*p_szAddInNameList) return;

	OnBtnClearAll();

	CString strAddInNameList;
	char *szAddInNameList = strAddInNameList.GetBufferSetLength( strlen( _ADDIN_LOAD_ORDER));
	strcpy( szAddInNameList, p_szAddInNameList);
	char *szItem = szAddInNameList + 7;
	char *szItemNext = NULL;
	AddInNameListParse( szItem, szItemNext);
	int nIndex = -1;
	LVFINDINFO info;
	while( *szItem)
	{
		info.flags = LVFI_STRING;
		info.psz = szItem;
		nIndex = m_lstAddInItem.FindItem( &info);
		if( 0 <= nIndex) m_lstAddInItem.SetCheck( nIndex);
		AddInNameListParse( szItem, szItemNext);
	}
}

void CAddInLoadInfoPage::OnBtnResetUndo() 
{
	// TODO: Add your control notification handler code here
	SetAddInNameListWith( m_strAddInNameList);
}

void CAddInLoadInfoPage::OnBtnResetDefault() 
{
	// TODO: Add your control notification handler code here
	SetAddInNameListWith( _DPV_ADDINLIST);
}

// (2006/8/27 - Seung-Won, Bae) In AddIn Selection Changed, Show AddIn Info.
void CAddInLoadInfoPage::OnItemchangedLstAddin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if( !pNMListView) return;
	if( !( pNMListView->uNewState & LVIS_SELECTED) || ( pNMListView->uOldState & LVIS_SELECTED)) return;

	CString strComment;
	strComment.LoadString( pNMListView->iItem + 1);

	CString strInfo;
	strInfo.Format( "[AddIn ID String]\r\n"
					"    %s\r\n\r\n"
					"[Include in]\r\n"
					"    %s\r\n\r\n"
					"%s",
					m_lstAddInItem.GetItemText( pNMListView->iItem, 0),
					m_lstAddInItem.GetItemText( pNMListView->iItem, 1),
					strComment
					);
	m_stcComment.SetWindowText( strInfo);
}
