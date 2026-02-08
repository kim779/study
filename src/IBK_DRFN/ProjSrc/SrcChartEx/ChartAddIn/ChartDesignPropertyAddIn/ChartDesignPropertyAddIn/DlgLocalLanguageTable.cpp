// DlgLocalLanguageTable.cpp : implementation file
//

#include "stdafx.h"
#include "chartdesignpropertyaddin.h"
#include "DlgLocalLanguageTable.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "../Include_Chart/xUtil_Chart.h"				// for CHART_CONVERT_BEGIN
#include "../Include_Chart/PropertyMap.h"				// for CPropertyMap
#include "../Include_Chart/PropertiesData.h"			// for g_anPropertyShortKey[]

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLocalLanguageTable dialog


CDlgLocalLanguageTable::CDlgLocalLanguageTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLocalLanguageTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLocalLanguageTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_ptMinSize.x = 500;
	m_ptMinSize.y = 350;
	
	m_eLanguageID = IMetaTable::ENGLISH;
}


void CDlgLocalLanguageTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLocalLanguageTable)
	DDX_Control(pDX, IDC_LST_LANGUAGE_TABLE, m_lstLanguageTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLocalLanguageTable, CDialog)
	//{{AFX_MSG_MAP(CDlgLocalLanguageTable)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BTN_REDEFINE_GLOBAL, OnBtnRedefineGlobal)
	ON_CBN_SELENDOK(IDC_CMB_GLOBAL, OnSelendokCmbGlobal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLocalLanguageTable message handlers

// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
void CDlgLocalLanguageTable::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(		p_pIPropertyMap, g_anPropertyShortKey[	LOCALLANGUAGETABLE],	m_strLocalLanguageTable,	_DPV_LOCALLANGUAGETABLE);	
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}


BOOL CDlgLocalLanguageTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CRect rctWindow;
	GetWindowRect( &rctWindow);
	rctWindow.right = rctWindow.left + m_ptMinSize.x + 1;
	rctWindow.bottom = rctWindow.top + m_ptMinSize.y + 1;
	MoveWindow( rctWindow);

	m_lstLanguageTable.SetExtendedStyle( m_lstLanguageTable.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_eLanguageID = g_iMetaTable.GetCurrentLanguageID( m_pParentWnd->GetSafeHwnd());
	m_lstLanguageTable.InsertColumn( 0, "Meta Text", LVCFMT_LEFT, 80);
	m_lstLanguageTable.InsertColumn( 1, "English", LVCFMT_LEFT, 130);
	CString strHeader;
	switch( m_eLanguageID)
	{
		case IMetaTable::KOREAN:	strHeader = "Korean";
									break;
	}
	if( !strHeader.IsEmpty()) m_lstLanguageTable.InsertColumn( 2, strHeader, LVCFMT_LEFT, 130);
	m_lstLanguageTable.m_bEditableColumn[ 0] = 1;		// Set meta column to be read only.

	CString strLanguageTable;
	char *szLanguageTable = strLanguageTable.GetBufferSetLength( m_strLocalLanguageTable.GetLength());
	strcpy( szLanguageTable, m_strLocalLanguageTable);

	char *szNext = szLanguageTable;
	char *szNextLine = NULL;
	char *szMetaText = NULL;
	char *szNextText = NULL;
	char *szEnglishText = NULL;
	char *szLangaugeText = NULL;
	int nLanguageID = 0;
	CStringList *pLanguageList = NULL;
	int nInsertIndex = 0;
	while( szNext)
	{
		szNextLine = strstr( szNext, "\r\n");
		if( szNextLine)
		{
			*szNextLine++ = '\0';
			szNextLine++;
			if( !*szNextLine) szNextLine = NULL;
		}

		// Meta Code
		szMetaText = strchr( szNext, ';');
		if( szMetaText)
		{
			*szMetaText++ = '\0';

			if( '$' == *szMetaText)
			{
				// (2008/1/17 - Seung-Won, Bae) for Static Local Meta Data
				m_lstStaticMetaData.AddTail( CString( ';') + szMetaText + "\r\n");
			}
			else
			{
				// Language Text
				szNextText = strchr( szMetaText, ';');
				if( szNextText)
				{
					*szNextText++ = '\0';

					// 6. Get Language Text
					pLanguageList = new CStringList;
					nLanguageID = 0;
					szEnglishText = NULL;
					szLangaugeText = NULL;
					while( szNextText && nLanguageID < IMetaTable::LANGUAGE_ID_END + 1)		// +1 for Other MetaText
					{
						szNext = strchr( szNextText, ';');
						if( !szNext) break;

						*szNext++ = '\0';
						pLanguageList->AddTail( szNextText);
						if( IMetaTable::ENGLISH == nLanguageID - 1)	szEnglishText = szNextText;		// -1 for Other MetaText
						if( m_eLanguageID == nLanguageID - 1)		szLangaugeText = szNextText;	// -1 for Other MetaText

						szNextText = szNext;
						nLanguageID++;
					}

					nInsertIndex = m_lstLanguageTable.InsertItem( 0, szMetaText);
					m_lstLanguageTable.SetItemText( nInsertIndex, 1, szEnglishText);
					if( m_eLanguageID != IMetaTable::ENGLISH)
						m_lstLanguageTable.SetItemText( nInsertIndex, 2, szLangaugeText);
					m_lstLanguageTable.SetItemData( nInsertIndex, ( DWORD)pLanguageList);
				}
			}
		}

		szNext = szNextLine;
	}

	// (2008/3/31 - Seung-Won, Bae) Support Global Redefine.
	CComboBox *pcmbGlobalMetas = ( CComboBox *)GetDlgItem( IDC_CMB_GLOBAL);
	int j = 0;
	int nMetaTextCount = g_iMetaTable.GetMetaTextCount( IMetaTable::PACKET_NAME);
	char **szMetaTexts = new char *[ nMetaTextCount];
	g_iMetaTable.GetAllMetaTexts( IMetaTable::PACKET_NAME, szMetaTexts);
	for(  j = 0; j < nMetaTextCount; j++) pcmbGlobalMetas->AddString( szMetaTexts[ j]);
	delete [] szMetaTexts;
	nMetaTextCount = g_iMetaTable.GetMetaTextCount( IMetaTable::GRAPH_TITLE);
	szMetaTexts = new char *[ nMetaTextCount];
	g_iMetaTable.GetAllMetaTexts( IMetaTable::GRAPH_TITLE, szMetaTexts);
	for( j = 0; j < nMetaTextCount; j++)
		if( pcmbGlobalMetas->FindString( 0, szMetaTexts[ j]) == CB_ERR)
			pcmbGlobalMetas->AddString( szMetaTexts[ j]);
	delete [] szMetaTexts;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgLocalLanguageTable::OnOK() 
{
	m_strLocalLanguageTable.Empty();
	
	long lTableTextLen = 0;
	POSITION psnStaticMeta = m_lstStaticMetaData.GetHeadPosition();
	while( psnStaticMeta) lTableTextLen += m_lstStaticMetaData.GetNext( psnStaticMeta).GetLength();	// ;MetaText;LanguageText;\r\n
	int nItemCount = m_lstLanguageTable.GetItemCount();
	int j = 0;
	CStringList *pLanguageTexts = NULL;
	POSITION psnLanguageText = NULL;
	CString strLanguageText;
	int i = 0;
	for(  i = 0; i < nItemCount; i++)
	{
		lTableTextLen++;														// ';'
		strLanguageText = m_lstLanguageTable.GetItemText( i, 0);
		lTableTextLen += strLanguageText.GetLength();							// MetaText
		lTableTextLen++;														// ';'

		pLanguageTexts = ( CStringList *)m_lstLanguageTable.GetItemData( i);
		psnLanguageText = pLanguageTexts->GetHeadPosition();
		for( j = IMetaTable::ENGLISH; j < IMetaTable::LANGUAGE_ID_END + 1; j++)		// +1 for Other MetaText
		{
			strLanguageText.Empty();
			if( psnLanguageText) strLanguageText = pLanguageTexts->GetNext( psnLanguageText);
			if( IMetaTable::ENGLISH == j - 1)
				strLanguageText = m_lstLanguageTable.GetItemText( i, 1);		// +1 for Other MetaText
			else if( m_eLanguageID != IMetaTable::ENGLISH)
				strLanguageText = m_lstLanguageTable.GetItemText( i, 2);		// +1 for Other MetaText
			
			lTableTextLen += strLanguageText.GetLength();						// LanguageText
			lTableTextLen++;													// ';'
		}
		lTableTextLen += 2;														// '\r\n'
	}

	char *szLanguageTableText = m_strLocalLanguageTable.GetBufferSetLength( lTableTextLen);
	psnStaticMeta = m_lstStaticMetaData.GetHeadPosition();
	while( psnStaticMeta)
	{
		strLanguageText = m_lstStaticMetaData.GetNext( psnStaticMeta);
		strcpy( szLanguageTableText, strLanguageText);							// MetaText;LanguageText;
		szLanguageTableText += strLanguageText.GetLength();
	}
	for( i = 0; i < nItemCount; i++)
	{
		*szLanguageTableText++ = ';';											// ';'
		strLanguageText = m_lstLanguageTable.GetItemText( i, 0);
		strcpy( szLanguageTableText, strLanguageText);							// MetaText
		szLanguageTableText += strLanguageText.GetLength();
		*szLanguageTableText++ = ';';											// ';'

		pLanguageTexts = ( CStringList *)m_lstLanguageTable.GetItemData( i);
		psnLanguageText = pLanguageTexts->GetHeadPosition();
		for( j = IMetaTable::ENGLISH; j < IMetaTable::LANGUAGE_ID_END + 1; j++)		// +1 for Other MetaText
		{
			strLanguageText.Empty();
			if( psnLanguageText) strLanguageText = pLanguageTexts->GetNext( psnLanguageText);
			if( IMetaTable::ENGLISH == j - 1)
				strLanguageText = m_lstLanguageTable.GetItemText( i, 1);		// +1 for Other MetaText
			else if( m_eLanguageID != IMetaTable::ENGLISH)
				strLanguageText = m_lstLanguageTable.GetItemText( i, 2);		// +1 for Other MetaText
			
			strcpy( szLanguageTableText, strLanguageText);						// LanguageText
			szLanguageTableText += strLanguageText.GetLength();
			*szLanguageTableText++ = ';';										// ';'
		}
		*szLanguageTableText++ = '\r';											// '\r'
		*szLanguageTableText++ = '\n';											// '\n'
	}
	*szLanguageTableText = '\0';

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	LOCALLANGUAGETABLE],	m_strLocalLanguageTable,	_DPV_LOCALLANGUAGETABLE);
	CHART_ADDTOSTRLIST_END

	CDialog::OnOK();
}

void CDlgLocalLanguageTable::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	int nItemCount = m_lstLanguageTable.GetItemCount();
	for( int i = 0; i < nItemCount; i++) delete ( CStringList *)m_lstLanguageTable.GetItemData( i);
}

void CDlgLocalLanguageTable::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here

	GetDlgItem( IDC_BTN_ADD)->ShowWindow( SW_HIDE);
	GetDlgItem( IDC_EDT_LANGUAGE_TEXT)->ShowWindow( SW_SHOW);
	GetDlgItem( IDC_EDT_LANGUAGE_TEXT)->SetWindowText( "");
	GetDlgItem( IDC_EDT_LANGUAGE_TEXT)->SetFocus();
}

void CDlgLocalLanguageTable::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here

	POSITION posSelection = m_lstLanguageTable.GetFirstSelectedItemPosition();
	if( !posSelection) return;

	int nSelectedItem = m_lstLanguageTable.GetNextSelectedItem( posSelection);

	delete ( CStringList *)m_lstLanguageTable.GetItemData( nSelectedItem);
	m_lstLanguageTable.DeleteItem( nSelectedItem);

	if( m_lstLanguageTable.GetItemCount() <= nSelectedItem) nSelectedItem--;
	if( 0 <= nSelectedItem) m_lstLanguageTable.SetSelectionMark( nSelectedItem);
}

void CDlgLocalLanguageTable::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CRect rctCtrl;
	CWnd *pWnd = NULL;
	
	// 1. Check Gap
	pWnd = GetDlgItem( IDC_LST_LANGUAGE_TABLE);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	int nLeftGap = rctCtrl.left;
	int nTopGap = rctCtrl.top;

	// 2. Set Cancel
	pWnd = GetDlgItem( IDCANCEL);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.OffsetRect( cx - rctCtrl.Width() - nLeftGap - rctCtrl.left, cy - rctCtrl.Height() - nTopGap - rctCtrl.top);
	pWnd->MoveWindow( rctCtrl);

	// 3. Set OK
	CRect rctCtrl2;
	pWnd = GetDlgItem( IDOK);
	pWnd->GetWindowRect( &rctCtrl2);
	ScreenToClient( rctCtrl2);
	rctCtrl2.OffsetRect( cx - rctCtrl2.Width() - rctCtrl.Width() - nLeftGap * 2 - rctCtrl2.left, cy - rctCtrl2.Height() - nTopGap - rctCtrl2.top);
	pWnd->MoveWindow( rctCtrl2);

	// 4. Set Add
	pWnd = GetDlgItem( IDC_BTN_ADD);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.OffsetRect( 0, cy - rctCtrl.Height() - nTopGap - rctCtrl.top);
	pWnd->MoveWindow( rctCtrl);

	// 4.5 Set Edit
	pWnd = GetDlgItem( IDC_EDT_LANGUAGE_TEXT);
	pWnd->MoveWindow( rctCtrl);

	// 5. Set Redefine
	pWnd = GetDlgItem( IDC_BTN_REDEFINE_GLOBAL);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.OffsetRect( 0, cy - rctCtrl.Height() - nTopGap - rctCtrl.top);
	pWnd->MoveWindow( rctCtrl);

	// 5. Set Redefine
	pWnd = GetDlgItem( IDC_CMB_GLOBAL);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.OffsetRect( 0, cy - rctCtrl.Height() - nTopGap - rctCtrl.top);
	rctCtrl.bottom += 100;
	pWnd->MoveWindow( rctCtrl);

	// 5. Set Delete
	pWnd = GetDlgItem( IDC_BTN_DELETE);
	pWnd->GetWindowRect( &rctCtrl);
	ScreenToClient( rctCtrl);
	rctCtrl.OffsetRect( 0, cy - rctCtrl.Height() - nTopGap - rctCtrl.top);
	pWnd->MoveWindow( rctCtrl);

	// 5. Set Grid
	pWnd = GetDlgItem( IDC_LST_LANGUAGE_TABLE);
	pWnd->GetWindowRect( &rctCtrl2);
	ScreenToClient( rctCtrl2);
	rctCtrl2.right = cx - nLeftGap;
	rctCtrl2.bottom = cy - rctCtrl.Height() - nTopGap * 2;
	pWnd->MoveWindow( rctCtrl2);

	// 6. Redraw
	InvalidateRect( NULL);
}

void CDlgLocalLanguageTable::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	
	lpMMI->ptMinTrackSize = m_ptMinSize;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

BOOL CDlgLocalLanguageTable::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		if( pMsg->hwnd == GetDlgItem( IDC_EDT_LANGUAGE_TEXT)->GetSafeHwnd())
	{
		GetDlgItem( IDC_EDT_LANGUAGE_TEXT)->ShowWindow( SW_HIDE);
		GetDlgItem( IDC_BTN_ADD)->ShowWindow( SW_SHOW);

		CString strLanguageText;
		GetDlgItem( IDC_EDT_LANGUAGE_TEXT)->GetWindowText( strLanguageText);

		CString strMetaText;
		const char *szMetaText = strLanguageText;

		int nLen = strlen( szMetaText), j = 0;
		char *szTempMetaText = strMetaText.GetBuffer( nLen * 2 + 3);
		unsigned char c1 = 0, c2 = 0;
		szTempMetaText[ j++] = '_';
		szTempMetaText[ j++] = 'M';
		for( int i = 0; i < nLen; i++)
		{
			c1 = szMetaText[ i];
			if( c1 < 0x80) szTempMetaText[ j++]	= c1;
			else
			{
				c2						= '0' + ( c1 >> 4);
				szTempMetaText[ j++]	= ( '9' < c2 ? c2 - '9' - 1 + 'A' : c2);
				c2						= '0' + ( c1 & 0x0F);
				szTempMetaText[ j++]	= ( '9' < c2 ? c2 - '9' - 1 + 'A' : c2);
			}
		}
		szTempMetaText[ j++]		= '_';
		szTempMetaText[ j++]		= '\0';
		strMetaText.ReleaseBuffer();

		LVFINDINFO info;
		info.flags = LVFI_STRING;
		info.psz = strMetaText;
		int nIndex = m_lstLanguageTable.FindItem( &info);
		if( 0 <= nIndex)
		{
			AfxMessageBox( "It was added already!");
			return TRUE;
		}

		CStringList *pLanguageList = new CStringList;
		for( int nLanguageID = 0; nLanguageID < IMetaTable::LANGUAGE_ID_END + 1; nLanguageID++)	// +1 for Other MetaText
			pLanguageList->AddTail( strLanguageText);

		nIndex = m_lstLanguageTable.InsertItem( 0, strMetaText);
		m_lstLanguageTable.SetItemText( nIndex, 1, strLanguageText);
		if( m_eLanguageID != IMetaTable::ENGLISH)
			m_lstLanguageTable.SetItemText( nIndex, 2, strLanguageText);
		m_lstLanguageTable.SetItemData( nIndex, ( DWORD)pLanguageList);

		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgLocalLanguageTable::OnBtnRedefineGlobal() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem( IDC_BTN_REDEFINE_GLOBAL)->ShowWindow( SW_HIDE);
	GetDlgItem( IDC_BTN_DELETE)->ShowWindow( SW_HIDE);
	GetDlgItem( IDC_CMB_GLOBAL)->ShowWindow( SW_SHOW);
	GetDlgItem( IDC_CMB_GLOBAL)->SetFocus();
}

void CDlgLocalLanguageTable::OnSelendokCmbGlobal() 
{
	// TODO: Add your control notification handler code here

	CString strMetaText;
	GetDlgItem( IDC_CMB_GLOBAL)->GetWindowText( strMetaText);

	LVFINDINFO info;
	info.flags = LVFI_STRING;
	info.psz = strMetaText;
	int nIndex = m_lstLanguageTable.FindItem( &info);
	if( 0 <= nIndex)
	{
		AfxMessageBox( "It was added already!");

		GetDlgItem( IDC_BTN_REDEFINE_GLOBAL)->ShowWindow( SW_SHOW);
		GetDlgItem( IDC_BTN_DELETE)->ShowWindow( SW_SHOW);
		GetDlgItem( IDC_CMB_GLOBAL)->ShowWindow( SW_HIDE);
		return;
	}

	CString strLanguageText, strEnglishText;
	CStringList *pLanguageList = new CStringList;
	pLanguageList->AddTail( strMetaText);				// for Other MetaText
	for( int nLanguageID = IMetaTable::ENGLISH; nLanguageID < IMetaTable::LANGUAGE_ID_END; nLanguageID++)
	{
		const char *szLanguageText = _MTOLDN( IMetaTable::PACKET_NAME, nLanguageID, strMetaText);
		if( !szLanguageText || strMetaText == szLanguageText)
			szLanguageText = _MTOLDN( IMetaTable::GRAPH_TITLE, nLanguageID, strMetaText);
		pLanguageList->AddTail( szLanguageText);

		if( m_eLanguageID == nLanguageID) strLanguageText = szLanguageText;
		if( IMetaTable::ENGLISH == nLanguageID) strEnglishText = szLanguageText;
	}

	nIndex = m_lstLanguageTable.InsertItem( 0, strMetaText);
	m_lstLanguageTable.SetItemText( nIndex, 1, strEnglishText);
	if( m_eLanguageID != IMetaTable::ENGLISH)
		m_lstLanguageTable.SetItemText( nIndex, 2, strLanguageText);
	m_lstLanguageTable.SetItemData( nIndex, ( DWORD)pLanguageList);

	GetDlgItem( IDC_BTN_REDEFINE_GLOBAL)->ShowWindow( SW_SHOW);
	GetDlgItem( IDC_BTN_DELETE)->ShowWindow( SW_SHOW);
	GetDlgItem( IDC_CMB_GLOBAL)->ShowWindow( SW_HIDE);
}
