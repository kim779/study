// BlockSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "BlockSetDlg.h"

#include "../Include_Chart/xUtil_Chart.h"		// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"		// for IPropertyMap
#include "InitializeControlDlg.h"
#include "PropertiesData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockSetDlg dialog

CBlockSetDlg::CBlockSetDlg( HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CBlockSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockSetDlg)
	m_nBlockRowCount = 0;
	m_nBlockColumnCount = 0;
	m_nBlockBorderWidth = 0;
	m_nBlockRow2Count = 0;
	m_nBlockColumn2Count = 0;
	//}}AFX_DATA_INIT
	m_nBlockGap = -1;
	m_nBlockRowCount = 1;
	m_nBlockRow2Count = -1;
	m_nBlockColumnCount = 1;
	m_nBlockColumn2Count = 0;
	m_nBlockBorderWidth = 1;
	m_nStyleBlockSection = 0;
	m_nStyleBlock = 0;

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( p_hOcxWnd);
}


void CBlockSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockSetDlg)
	DDX_Control(pDX, IDC_BLOCKGAP, m_GapEdit);
	DDX_Control(pDX, IDC_BLOCKWIDTH, m_ThicknessEdit);
	DDX_Control(pDX, IDC_BLOCKSHAPE2, m_BlockShape2);
	DDX_Control(pDX, IDC_BLOCKSHAPE, m_BlockShape);
	DDX_Text(pDX, IDC_ROW, m_nBlockRowCount);
	DDX_Text(pDX, IDC_COLUMN, m_nBlockColumnCount);
	DDX_Text(pDX, IDC_BLOCKWIDTH, m_nBlockBorderWidth);
	DDX_Text(pDX, IDC_BLOCKGAP, m_nBlockGap);
	DDX_Text(pDX, IDC_ROW2, m_nBlockRow2Count);
	DDX_Text(pDX, IDC_COLUMN2, m_nBlockColumn2Count);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockSetDlg, CDialog)
	//{{AFX_MSG_MAP(CBlockSetDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_BLOCKSHAPE, OnDblclkBlockshape)
	ON_NOTIFY(NM_CLICK, IDC_BLOCKSHAPE2, OnClickBlockshape2)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_BLOCKGAP, OnChangeBlockgap)
	ON_NOTIFY(NM_CLICK, IDC_BLOCKSHAPE, OnClickBlockshape)
	ON_EN_CHANGE(IDC_ROW, OnChangeRow)
	ON_EN_CHANGE(IDC_COLUMN, OnChangeColumn)
	ON_EN_CHANGE(IDC_COLUMN2, OnChangeColumn2)
	ON_EN_CHANGE(IDC_ROW2, OnChangeRow2)
	ON_NOTIFY(NM_KILLFOCUS, IDC_BLOCKSHAPE, OnKillfocusBlockshape)
	ON_NOTIFY(NM_KILLFOCUS, IDC_BLOCKSHAPE2, OnKillfocusBlockshape2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockSetDlg message handlers

BOOL CBlockSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_ImageList.DeleteImageList();
	m_ImageList1.DeleteImageList();
	m_ImageList2.DeleteImageList();
	m_ImageList3.DeleteImageList();

	ML_SET_DEFAULT_RES();
	m_ImageList.Create( IDB_BLOCK, 32, 1, RGB(255, 255, 255));
	m_ImageList1.Create( IDB_ROWBLOCKEDGESPACE, 32, 1, RGB(255, 255, 255));
	m_ImageList2.Create( IDB_COLUMNBLOCKSHAPE, 32, 1, RGB(255, 255, 255));
	m_ImageList3.Create( IDB_MULTIBLOCKSHAPE, 32, 1, RGB(255, 255, 255));

	m_BlockShape.SetImageList(&m_ImageList, LVSIL_NORMAL);
	if(m_nStyleBlock == 0)
		m_BlockShape2.SetImageList(&m_ImageList1, LVSIL_NORMAL);
	else if(m_nStyleBlock == 1)
		m_BlockShape2.SetImageList(&m_ImageList2, LVSIL_NORMAL);
	else if(m_nStyleBlock == 2)
		m_BlockShape2.SetImageList(&m_ImageList3, LVSIL_NORMAL);

	UINT nIDs[] = { IDS_HORIZONTAL, IDS_VERTICAL, IDS_CHECKERS, 0};
	UINT nIDs2[] = { IDS_SEPARATOR, IDS_GAP, IDS_BORDER, 0};

	InitListCtrl(&m_BlockShape, nIDs, 3);
	InitListCtrl(&m_BlockShape2, nIDs2, 3);

	m_BlockShape.SetIconSpacing(85, 50);
	m_BlockShape2.SetIconSpacing(85, 50); 

	m_nPreIndex = m_nStyleBlock;
	m_nPreStyleBlockSection = m_nStyleBlockSection;
	m_BlockShape.SetItemState(m_nPreIndex, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 
	m_BlockShape2.SetItemState(m_nPreStyleBlockSection, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 

	SetEnableWindow();	
	//툴팁
	CreateToolTip();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//init listCtrl
void CBlockSetDlg::InitListCtrl(CListCtrl* pList, UINT nIDs[], int num)
{
	ML_SET_LANGUAGE_RES();
	CString strText;
	for(int i = 0; i < num ; i++)
	{
		LV_ITEM listItem;
		listItem.mask = LVIF_TEXT | LVIF_IMAGE ;
		listItem.iSubItem = 0 ;
		listItem.iImage = i ;       // index of the list view item’s icon 
		listItem.iItem = i ;
		strText.LoadString( nIDs[ i]);
		listItem.pszText = ( char *)( const char *)strText;    
		pList->InsertItem( &listItem) ;
	}	
}

void CBlockSetDlg::OnDblclkBlockshape(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	UINT nIDs2[] = { IDS_SEPARATOR, IDS_GAP, IDS_BORDER, 0};
    int   iItem = pNMListView->iItem; 
	m_nStyleBlock = iItem;

	if(m_nStyleBlock == -1)
		return;

	m_BlockShape2.DeleteAllItems();
	switch(iItem)
	{
		case 0:
			m_BlockShape2.SetImageList(&m_ImageList1, LVSIL_NORMAL);
			InitListCtrl(&m_BlockShape2,nIDs2, 3);			
			break;
		case 1:
			m_BlockShape2.SetImageList(&m_ImageList2, LVSIL_NORMAL);
			InitListCtrl(&m_BlockShape2,nIDs2, 3);			
			break;
		case 2:
			m_BlockShape2.SetImageList(&m_ImageList3, LVSIL_NORMAL);
			InitListCtrl(&m_BlockShape2,nIDs2, 3);			
			break;
	}

	m_nPreStyleBlockSection = m_nStyleBlockSection;
	m_BlockShape2.SetItemState(m_nPreStyleBlockSection, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE);	
	*pResult = 0;
}

//Enable Row, Column Edit by Block Type
void CBlockSetDlg::SetEnableWindow()
{
	CEdit* pRow = (CEdit* )GetDlgItem(IDC_ROW);
	CEdit* pRow2 = (CEdit* )GetDlgItem(IDC_ROW2);
	CEdit* pColumn = (CEdit* )GetDlgItem(IDC_COLUMN);
	CEdit* pColumn2 = (CEdit* )GetDlgItem(IDC_COLUMN2);

	if(m_nStyleBlock == 0) {
		pColumn->EnableWindow(FALSE);
		pColumn2->EnableWindow(FALSE);
		pRow->EnableWindow(TRUE);
		pRow2->EnableWindow(TRUE);
	}
	else if(m_nStyleBlock == 1) {
		pRow->EnableWindow(FALSE);
		pRow2->EnableWindow(FALSE);
		pColumn->EnableWindow(TRUE);
		pColumn2->EnableWindow(TRUE);
	}
	else if(m_nStyleBlock == 2) {
		pRow->EnableWindow(TRUE);
		pRow2->EnableWindow(TRUE);
		pColumn->EnableWindow(TRUE);
		pColumn2->EnableWindow(TRUE);
	}
}

void CBlockSetDlg::OnClickBlockshape2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nStyleBlockSection = pNMListView->iItem;

	if(m_nPreStyleBlockSection != -1)	{
		m_BlockShape2.SetItemState(m_nPreStyleBlockSection, LVIS_SELECTED , LVIF_STATE);	
		m_nPreStyleBlockSection = -1;
	}

	m_nPreIndex = m_nStyleBlock;
	m_BlockShape.SetItemState(m_nPreIndex, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 

	m_nBlockGap = 4;
	UpdateData(FALSE);

	*pResult = 0;
}

void CBlockSetDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	m_ImageList.Detach();
	m_ImageList1.Detach();
	m_ImageList2.Detach();
	m_ImageList3.Detach();
}

void CBlockSetDlg::OnChangeBlockgap() 
{
	CString str;
	CEdit* pGap = (CEdit* )GetDlgItem(IDC_BLOCKGAP);
	pGap->GetWindowText(str);
	m_nBlockGap = atoi(str);

	ML_SET_LANGUAGE_RES();
	if(m_nStyleBlockSection == 0)
	{
		if(m_nBlockGap < -20 || m_nBlockGap > 20)
		{
			CString strMsg;
			strMsg.LoadString( IDS_ENTER_20TO20);
			AfxMessageBox( strMsg);
		}
	}
	else
	{
		if(m_nBlockGap < 0 || m_nBlockGap > 20)
		{
			CString strMsg;
			strMsg.LoadString( IDS_ENTER_0TO20);
			AfxMessageBox( strMsg);
		}
	}
}


void CBlockSetDlg::OnClickBlockshape(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nStyleBlock = pNMListView->iItem;

	if(m_nPreIndex != -1)	{
		m_BlockShape.SetItemState(m_nPreIndex, LVIS_SELECTED , LVIF_STATE);	
		m_nPreIndex = -1;
	}

	m_nPreStyleBlockSection = m_nStyleBlockSection;
	m_BlockShape2.SetItemState(m_nPreStyleBlockSection, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 

	UpdateColumnandRowbyDefaultValue();
    SetEnableWindow();
	*pResult = 0;
}

void CBlockSetDlg::OnChangeRow() 
{
	UpdateData(TRUE);
	int num = (m_nBlockRowCount * m_nBlockColumnCount);
	if(m_nBlockRowCount == 0) {
		m_nBlockRowCount = 1;
		UpdateData(FALSE);
	}

	if(num > 10) 
		TreateWrongInputData( IDS_BLOCK_COUNT_OVER, (10/m_nBlockColumnCount), &m_nBlockRowCount);
}

void CBlockSetDlg::OnChangeColumn() 
{
	UpdateData(TRUE);
	int num = (m_nBlockRowCount * m_nBlockColumnCount);
	if(m_nBlockColumnCount == 0) {
		m_nBlockColumnCount = 1;
		UpdateData(FALSE);
	}

	if(num > 10) 
		TreateWrongInputData( IDS_BLOCK_COUNT_OVER, (10/m_nBlockRowCount), &m_nBlockColumnCount);
}

//툴팁에 적용
BOOL CBlockSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	bool bRButton;
	bRButton = false;

	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN ) {
		m_toolTip.ShowToolTip( (CPoint)pt );
		bRButton = true;
	}
	if(pMsg->message == WM_MOUSEMOVE) {
		m_toolTip.HideToolTip( (CPoint)pt );
	}

	if(bRButton)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

//Create tooltip object and Add text to StringArray 
void CBlockSetDlg::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_BLOCKSHAPE_TOOLTIP_1);
	dwaInfo.Add( IDS_BLOCKSHAPE_TOOLTIP_2);
	m_toolTip.AddControlInfo( IDC_BLOCKSHAPE, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_BLOCKSHAPE2_TOOLTIP_1);
	dwaInfo.Add( IDS_BLOCKSHAPE2_TOOLTIP_2);
	m_toolTip.AddControlInfo( IDC_BLOCKSHAPE2, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_1);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_2);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_3);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_4);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_5);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_6);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_7);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_8);
	dwaInfo.Add( IDS_BLOCKSET_STATIC3_TOOLTIP_9);
	m_toolTip.AddControlInfo( IDC_STATIC3, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_BLOCKWIDTH_TOOLTIP_1);
	m_toolTip.AddControlInfo( IDC_STATIC2, dwaInfo);
	m_toolTip.AddControlInfo( IDC_BLOCKWIDTH, dwaInfo);

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_1);
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_2);
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_3);
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_4);
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_5);
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_6);
	dwaInfo.Add( IDS_BLOCKGAP_TOOLTIP_7);
	m_toolTip.AddControlInfo( IDC_STATIC7, dwaInfo);
	m_toolTip.AddControlInfo( IDC_BLOCKGAP, dwaInfo);
}

void CBlockSetDlg::OnChangeColumn2() 
{
	ChangeDataOftwofoldBlockEdit(&m_nBlockColumnCount, &m_nBlockColumn2Count, IDC_COLUMN2);
}

void CBlockSetDlg::OnChangeRow2() 
{
	ChangeDataOftwofoldBlockEdit(&m_nBlockRowCount, &m_nBlockRow2Count, IDC_ROW2);
}


void CBlockSetDlg::UpdateColumnandRowbyDefaultValue()
{
	if(m_nStyleBlock == 0) { //가로형
		m_nBlockRow2Count = -1;
		m_nBlockColumn2Count = 0;
	}
	else if(m_nStyleBlock == 1) {// 세로형
		m_nBlockRow2Count = 0;
		m_nBlockColumn2Count = -1;
	}
	else { //바둑판형
		m_nBlockRow2Count = -1;
		m_nBlockColumn2Count = -1;
	}
	m_nBlockColumnCount = 1;
	m_nBlockRowCount = 1;
	UpdateData(FALSE);
}


void CBlockSetDlg::ChangeDataOftwofoldBlockEdit(int* nBlockCount, int* ntwofoldBlockCount, int nID)
{
	CEdit* pEdit = (CEdit* )GetDlgItem(nID);

	CString strData;
	pEdit->GetWindowText(strData);
	int ntwofoldCount = atoi(strData);

	if(ntwofoldCount < -1) 	
		TreateWrongInputData( IDS_CANNOT_BE_NEGATIVE, -1, ntwofoldBlockCount);
	if((*nBlockCount) <= ntwofoldCount)
		TreateWrongInputData( IDS_BLOCKNUMBER_OVER, (*nBlockCount - 1), ntwofoldBlockCount);
}

void CBlockSetDlg::OnKillfocusBlockshape(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nPreIndex = m_nStyleBlock;
	m_BlockShape.SetItemState(m_nPreIndex, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 
	
	*pResult = 0;
}

void CBlockSetDlg::OnKillfocusBlockshape2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nPreStyleBlockSection = m_nStyleBlockSection;
	m_BlockShape2.SetItemState(m_nPreStyleBlockSection, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 

	*pResult = 0;
}

void CBlockSetDlg::TreateWrongInputData( UINT uMessage, const int nReplaceData, int *nTarget)
{
	ML_SET_LANGUAGE_RES();
	CString strMessage;
	strMessage.LoadString( uMessage);
	AfxMessageBox(strMessage);
	*nTarget = nReplaceData;
	UpdateData(FALSE);
}

//Set Received Value From ocx
//nIndex: 속성의 순서
//strItemName : 속성의 이름
//strItemValue : 속성 값.
void CBlockSetDlg::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKTYPE			], m_nStyleBlock,			_DPV_TYPEBLOCKTYPE			);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKCOMPART		], m_nStyleBlockSection,	_DPV_TYPEBLOCKCOMPART		);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	NBLOCKROWCOUNT			], m_nBlockRowCount,		_DPV_NBLOCKROWCOUNT			);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	NBLOCKCOLUMNCOUNT		], m_nBlockColumnCount,		_DPV_NBLOCKCOLUMNCOUNT		);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	NBLOCKGAP				], m_nBlockGap,				_DPV_NBLOCKGAP				);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	BLOCKBORDERTHICKNESS	], m_nBlockBorderWidth,		_DPV_BLOCKBORDERTHICKNESS	);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	NROWDOUBLESIZEBLOCK		], m_nBlockRow2Count,		_DPV_NROWDOUBLESIZEBLOCK	);
		CHART_CONVERT_INT(	p_pIPropertyMap, g_anPropertyShortKey[	NCOLUMNDOUBLESIZEBLOCK	], m_nBlockColumn2Count,	_DPV_NCOLUMNDOUBLESIZEBLOCK	);
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

//ocx에 변화된 속성 보냄.
void CBlockSetDlg::OnOK() 
{
////변수들 다 보내기...
	UpdateData(TRUE);

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKTYPE			], m_nStyleBlock,			_DPV_TYPEBLOCKTYPE			);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	TYPEBLOCKCOMPART		], m_nStyleBlockSection,	_DPV_TYPEBLOCKCOMPART		);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	NBLOCKROWCOUNT			], m_nBlockRowCount,		_DPV_NBLOCKROWCOUNT			);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	NBLOCKCOLUMNCOUNT		], m_nBlockColumnCount,		_DPV_NBLOCKCOLUMNCOUNT		);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	NBLOCKGAP				], m_nBlockGap,				_DPV_NBLOCKGAP				);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	BLOCKBORDERTHICKNESS	], m_nBlockBorderWidth,		_DPV_BLOCKBORDERTHICKNESS	);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	NROWDOUBLESIZEBLOCK		], m_nBlockRow2Count,		_DPV_NROWDOUBLESIZEBLOCK	);
		CHART_ADDTOSTRLIST_INT(	m_pIPropertyMap, g_anPropertyShortKey[	NCOLUMNDOUBLESIZEBLOCK	], m_nBlockColumn2Count,	_DPV_NCOLUMNDOUBLESIZEBLOCK	);
	CHART_ADDTOSTRLIST_END

	CDialog::OnOK();
}
