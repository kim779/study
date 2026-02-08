// DlgGlobalLanguageTable.cpp : implementation file
//

#include "stdafx.h"
#include "chartdesignpropertyaddin.h"
#include "DlgGlobalLanguageTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGlobalLanguageTable dialog


CDlgGlobalLanguageTable::CDlgGlobalLanguageTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGlobalLanguageTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGlobalLanguageTable)
	//}}AFX_DATA_INIT

	// (2008/2/25 - Seung-Won, Bae) for Column Reverse Sort
	m_nSortColumn	= 0;
	m_bSortASC		= TRUE;

	// (2008/2/24 - Seung-Won, Bae) Resize Minimum
	m_ptMinSize.x = 650;
	m_ptMinSize.y = 450;
}

void CDlgGlobalLanguageTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGlobalLanguageTable)
	DDX_Control(pDX, IDC_CMB_TABLE_TYPE, m_cmbTableType);
	DDX_Control(pDX, IDC_LST_META_TABLE, m_lstMetaTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGlobalLanguageTable, CDialog)
	//{{AFX_MSG_MAP(CDlgGlobalLanguageTable)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_EN_UPDATE(IDC_EDT_SEARCH_TEXT, OnUpdateEdtSearchText)
	ON_CBN_SELCHANGE(IDC_CMB_TABLE_TYPE, OnSelchangeCmbTableType)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_META_TABLE, OnItemchangedLstMetaTable)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY(HDN_ITEMCLICK, 0, OnItemclickLstMetaTable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGlobalLanguageTable message handlers

BOOL CDlgGlobalLanguageTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CRect rctWindow;
	GetWindowRect( &rctWindow);
	rctWindow.right = rctWindow.left + m_ptMinSize.x + 1;
	rctWindow.bottom = rctWindow.top + m_ptMinSize.y + 1;
	MoveWindow( rctWindow);

	m_lstMetaTable.SetExtendedStyle( m_lstMetaTable.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CString strHeader;
	m_lstMetaTable.InsertColumn( 0, "Meta Text", LVCFMT_LEFT, 180);
	for( int i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
	{
		switch( i)
		{
			case IMetaTable::ENGLISH:	strHeader = "English";
										break;
			case IMetaTable::KOREAN:	strHeader = "Korean";
										break;
		}
		m_lstMetaTable.InsertColumn( i + 1, strHeader, LVCFMT_LEFT, 200);
	}

	m_cmbTableType.AddString( "Packet Name");
	m_cmbTableType.AddString( "Graph Title");
	m_cmbTableType.AddString( "Indicator Name");
	m_cmbTableType.AddString( "Sub Graph Name");
	m_cmbTableType.AddString( "Tool Name");
	m_cmbTableType.AddString( "Graph Type and Style");
	m_cmbTableType.AddString( "Scale Type and ETC");
	m_cmbTableType.AddString( "Indicator Group Name");
	m_cmbTableType.SetCurSel( 0);
	OnSelchangeCmbTableType();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgGlobalLanguageTable::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	if( !IsWindow( m_lstMetaTable.GetSafeHwnd())) return;
	CRect rctCtrl;
	
	// 1. Check Gap
	CWnd *pWnd = GetDlgItem( IDC_CMB_TABLE_TYPE);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	int nLeftGap = rctCtrl.left;
	int nTopGap = rctCtrl.top;

	// 2. Set Packet Name and Graph Title
	m_lstMetaTable.GetWindowRect( rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.right = cx - nLeftGap;
	rctCtrl.bottom = cy - nTopGap;
	m_lstMetaTable.MoveWindow( rctCtrl);

	// 3. Set Search Text
	pWnd = GetDlgItem( IDC_EDT_SEARCH_TEXT);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.OffsetRect( cx - rctCtrl.Width() - nLeftGap - rctCtrl.left, 0);
	pWnd->MoveWindow( rctCtrl);
}

void CDlgGlobalLanguageTable::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	lpMMI->ptMinTrackSize = m_ptMinSize;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CDlgGlobalLanguageTable::OnUpdateEdtSearchText() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
	CEdit *pSearchText = ( CEdit *)GetDlgItem( IDC_EDT_SEARCH_TEXT);

	CString strSearchText;
	pSearchText->GetWindowText( strSearchText);
	if( strSearchText.Find( "\r\n") < 0) return;

	strSearchText.Replace( "\r\n", "");
	pSearchText->SetWindowText( strSearchText);
	pSearchText->SetSel( 0, -1);
	pSearchText->SetSel( -1, -1);

	strSearchText.MakeLower();
	int nSelectedIndex = -1;
	POSITION psnSelection = m_lstMetaTable.GetFirstSelectedItemPosition();
	if( psnSelection) nSelectedIndex = m_lstMetaTable.GetNextSelectedItem( psnSelection);
	int nMetaCount = m_lstMetaTable.GetItemCount();
	CString strLanguageText;
	int i = 0;
	for(  i = nSelectedIndex + 1; i < nMetaCount; i++)
	{
		strLanguageText = m_lstMetaTable.GetItemText( i, 1);
		strLanguageText.MakeLower();
		if( 0 <= strLanguageText.Find( strSearchText)) break;
		strLanguageText = m_lstMetaTable.GetItemText( i, 2);
		strLanguageText.MakeLower();
		if( 0 <= strLanguageText.Find( strSearchText)) break;
	}
	if( nMetaCount <= i)
	{
		for( i = 0; i < nSelectedIndex + 1; i++)
		{
			strLanguageText = m_lstMetaTable.GetItemText( i, 1);
			strLanguageText.MakeLower();
			if( 0 <= strLanguageText.Find( strSearchText)) break;
			strLanguageText = m_lstMetaTable.GetItemText( i, 2);
			strLanguageText.MakeLower();
			if( 0 <= strLanguageText.Find( strSearchText)) break;
		}

		// Check Not Found
		if( nSelectedIndex <= i) return;
	}

	m_lstMetaTable.SetItemState( i,	LVIS_SELECTED, LVIS_SELECTED);
	m_lstMetaTable.EnsureVisible( i, FALSE);
}

void CDlgGlobalLanguageTable::OnSelchangeCmbTableType() 
{
	// TODO: Add your control notification handler code here

	int nTableType = m_cmbTableType.GetCurSel();
	if( nTableType < 0 || IMetaTable::CODE_PAGE_END <= nTableType) return;

	int nTextCount = m_lstMetaTable.GetItemCount();
	int j = 0;
	for(  j = 0; j < nTextCount; j++)
	{
		CStringArray *psaItems = ( CStringArray *)m_lstMetaTable.GetItemData( j);
		delete psaItems;
	}
	m_lstMetaTable.DeleteAllItems();

	int nIndex = 0;
	nTextCount = g_iMetaTable.GetMetaTextCount( ( IMetaTable::E_CODEPAGE)nTableType);
	char **szMetaTexts = new char *[ nTextCount];
	g_iMetaTable.GetAllMetaTexts( ( IMetaTable::E_CODEPAGE)nTableType, szMetaTexts);
	for( j = 0; j < nTextCount; j++)
	{
		CStringArray *psaItems = new CStringArray;
		psaItems->SetSize( IMetaTable::LANGUAGE_ID_END + 1);		// +1 for Meta Text
		psaItems->SetAt( 0, szMetaTexts[ j]);

		nIndex = m_lstMetaTable.InsertItem( j, szMetaTexts[ j]);
		for( int i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
		{
			const char *szLanguageText = _MTOLDN( nTableType, i, szMetaTexts[ j]);
			m_lstMetaTable.SetItemText( nIndex, i + 1, szLanguageText);

			psaItems->SetAt( i + 1, szLanguageText);
		}

		m_lstMetaTable.SetItemData( nIndex, ( DWORD)psaItems);
	}
	delete [] szMetaTexts;

	// (2008/2/25 - Seung-Won, Bae) for Column Reverse Sort
	m_nSortColumn	= 0;
	m_bSortASC		= TRUE;
}

void CDlgGlobalLanguageTable::OnItemchangedLstMetaTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if( !pNMListView) return;
	if( !( pNMListView->uChanged & LVIF_STATE)) return;
	if( !( pNMListView->uNewState & LVIS_SELECTED)) return;

	CString strMetaText = m_lstMetaTable.GetItemText( pNMListView->iItem, 0);
	CEdit *pCopyBuffer = ( CEdit *)GetDlgItem( IDC_EDT_COPY_BUFFER);
	pCopyBuffer->SetWindowText( strMetaText);
	pCopyBuffer->SetSel( 0, -1);
	pCopyBuffer->Copy();
}

BOOL g_bSortASC = TRUE;
int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CStringArray *psaItems1 = ( CStringArray *)lParam1;
	CStringArray *psaItems2 = ( CStringArray *)lParam2;

	// Include IMetaTable::LANGUAGE_ID_END == lParamSort for Meta Text
	if( lParamSort < IMetaTable::ENGLISH || IMetaTable::LANGUAGE_ID_END < lParamSort)
		return 0;
	int nResult = stricmp( psaItems1->GetAt( lParamSort), psaItems2->GetAt( lParamSort));
	return ( g_bSortASC ? nResult : -nResult);
} 
void CDlgGlobalLanguageTable::OnItemclickLstMetaTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NMLISTVIEW *pLV = ( NMLISTVIEW *)pNMHDR;
	// (2008/2/25 - Seung-Won, Bae) for Column Reverse Sort
	if( pLV->iItem == m_nSortColumn) m_bSortASC = !m_bSortASC;
	else
	{
		m_nSortColumn	= pLV->iItem;
		m_bSortASC		= TRUE;
	}

	g_bSortASC = m_bSortASC;
	m_lstMetaTable.SortItems( SortFunc, pLV->iItem);
}

void CDlgGlobalLanguageTable::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if( IsWindow( m_lstMetaTable.GetSafeHwnd()))
	{
		int nTextCount = m_lstMetaTable.GetItemCount();
		for( int j = 0; j < nTextCount; j++)
		{
			CStringArray *psaItems = ( CStringArray *)m_lstMetaTable.GetItemData( j);
			delete psaItems;
		}
		m_lstMetaTable.DeleteAllItems();
	}
}
