// ChartListPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ChartListPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTOL5()
#include "../Include_Chart/xUtil_Chart.h"				// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "ChartPropertySheet.h"
#include "PropertiesData.h"
#include "Conversion.h"
#include "DataOfChartListPage.h"

//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô --------->>
#include "ToolListData.h"			// for BLOCK
//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô ---------<<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////
/////////////////////////////////////////////////////////
// CChartListPage property page

IMPLEMENT_DYNCREATE(CChartListPage, CPropertyPage)

CChartListPage::CChartListPage() : CPropertyPage(CChartListPage::IDD)
{
	//{{AFX_DATA_INIT(CChartListPage)
	//}}AFX_DATA_INIT
	m_strChartListData = "";

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	m_pIGraphInfoManager = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;

	//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô --------->>
	m_pSelTreeItem = NULL;
	//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô ---------<<
}

CChartListPage::~CChartListPage()
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
}

void CChartListPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartListPage)
	DDX_Control(pDX, IDC_TREE_CHART, m_TreeChart);
	DDX_Control(pDX, IDC_LIST_CHART, m_listChart);
//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô --------->>
	DDX_Control(pDX, IDC_STATIC_SELGROUP, m_stcSelGroup);
	DDX_Control(pDX, IDC_BTN_SFUNC1, m_btnFunc1);
	DDX_Control(pDX, IDC_BTN_SFUNC2, m_btnFunc2);
//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô ---------<<
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartListPage, CPropertyPage)
	//{{AFX_MSG_MAP(CChartListPage)
	//}}AFX_MSG_MAP
//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô --------->>
	ON_BN_CLICKED(IDC_BTN_SFUNC1, OnBtnFunc1)
	ON_BN_CLICKED(IDC_BTN_SFUNC2, OnBtnFunc2)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_CHART, OnSelchangedTreeChart)
//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô ---------<<
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartListPage message handlers

//¸®½ºÆ®ÄÁÆ®·Ñ ÃÊ±âÈ­
void CChartListPage::InitList(CListCtrl *pList)
{
	m_ImageList.DeleteImageList();

	ML_SET_DEFAULT_RES();
	m_ImageList.Create(IDB_LIST, 16, 1, RGB(172, 172, 172));
	pList->SetImageList(&m_ImageList, LVSIL_SMALL);

	//	List Chart Init
	CRect rect;
	pList->GetWindowRect( &rect );

	// Insert Title of List Chart 
	LV_COLUMN	lvColumn;
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	lvColumn.cx = rect.Width() -5;
	lvColumn.pszText = _T("Chart List");
	pList->InsertColumn(0, &lvColumn);

	SetListItem(pList);
}

//Packet ¼Ó¼ºÆäÀÌÁö¿¡¼­ Á¢±ÙÇØ¾ßÇÏ´Â ÇÔ¼ö. 
//PacketÀÇ ¸ðµç Ç×¸ñ Á¤º¸°¡ ÇÏ³ªÀÇ StringÀ¸·Î µé¾î¿Â´Ù.
void CChartListPage::SetInfoOfAllPacketItems(const CString& strInfoOfAllPacketItems)
{
	m_strAllItemInfoOfPacketPage = strInfoOfAllPacketItems;
}

BOOL CChartListPage::OnSetActive() 
{
	m_TreeChart.SetAllItemInfoOfPacketPage(m_strAllItemInfoOfPacketPage);
	
	//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô --------->>
	HTREEITEM hItem = m_TreeChart.GetRootItem();
	m_TreeChart.SetFocus();
	m_TreeChart.SelectItem(hItem);
	//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô ---------<<

	return CPropertyPage::OnSetActive();
}

//ÅøÆÁ
void CChartListPage::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_1);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_2);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_3);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_4);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_5);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_6);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_7);
	dwaInfo.Add( IDS_LIST_CHART_TOOLTIP_8);
	m_toolTip.AddControlInfo( IDC_LIST_CHART, dwaInfo);
}

BOOL CChartListPage::PreTranslateMessage(MSG* pMsg) 
{
	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN )	{
		m_toolTip.ShowToolTip( (CPoint)pt );
	}
	if(pMsg->message == WM_MOUSEMOVE){
		m_toolTip.HideToolTip( (CPoint)pt );
	}

	if(pMsg->message == WM_MOUSEMOVE && pMsg->hwnd == m_TreeChart.m_hWnd) {
		CPoint point(LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
		HTREEITEM hItem = m_TreeChart.HitTest(point);
		if(hItem != NULL) {
			m_tooltip.Activate(TRUE);
			CString text = tooltipMap[hItem];		
			m_tooltip.UpdateTipText(text,&m_TreeChart);
			m_tooltip.RelayEvent(pMsg);
		}
		else
			m_tooltip.Activate(FALSE);
	}

	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CChartListPage::SetRowandColumn(const int &nRow, const int &nColumn)
{
	m_nColumn = nColumn;
	m_nRow = nRow;
}

void CChartListPage::SetToolTipInfo(HTREEITEM &hItem, const CString &strTip)
{
	//ÅøÆÁ.
	tooltipMap[hItem] = strTip;   
}

//sy 2006.02.20.
void CChartListPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	CString strItemValue, strGraphBase1, strGraphBase2, strGraphBase3, strGraphBase4, strGraphBase5, strGraphBase6;

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRCHARTLIST	], strItemValue,	_DPV_STRCHARTLIST	);
		// (2006/7/5 - Seung-Won, Bae) Manage Color with String Type
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE1	], strGraphBase1,		""					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE2	], strGraphBase2,		""					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE3	], strGraphBase3,		""					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE4	], strGraphBase4,		""					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE5	], strGraphBase5,		""					);
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	CLRGRAPHBASE6	], strGraphBase6,		""					);
	CHART_CONVERT_END()

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;

	m_TreeChart.SetColorOfLine( ONELINE_COLOR,		strGraphBase1);
	m_TreeChart.SetColorOfLine( TWOLINE_COLOR,		strGraphBase2);
	m_TreeChart.SetColorOfLine( THREELINE_COLOR,	strGraphBase3);
	m_TreeChart.SetColorOfLine( FOURLINE_COLOR,		strGraphBase4);
	m_TreeChart.SetColorOfLine( FIVELINE_COLOR,		strGraphBase5);
	m_TreeChart.SetColorOfLine( SIXLINE_COLOR,		strGraphBase6);
	SetChartListData( strItemValue);
}
//sy end

BOOL CChartListPage::OnApply() 
{
	m_strChartListData = m_TreeChart.GetBlockandGraphAllData();

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRCHARTLIST	], m_strChartListData,	_DPV_STRCHARTLIST	);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}


// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
BOOL CChartListPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	InitList(&m_listChart);
	CreateToolTip();

	m_TreeChart.InitTreeView();
	m_TreeChart.InitDropTarget();
	m_listChart.InitDropTarget();

	ML_SET_LANGUAGE_RES();
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.SetDelayTime(0);
	m_tooltip.AddTool(GetDlgItem(IDC_TREE_CHART), "Tree Tool Tip"); 

//	CDataOfChartListPage data(m_nRow, m_nColumn, &m_TreeChart);
//	data.SetData(m_strChartListData, m_pIGraphInfoManager);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CChartListPage::SetListItem(CListCtrl *pList)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if( !m_pIGraphInfoManager) return;
	CList< CString, CString> graphTypeList;
	if( !m_pIGraphInfoManager->GetAllGraphTypeNames( graphTypeList)) return;

	int i = 0;
	LV_ITEM listItem;
	listItem.mask = LVIF_TEXT | LVIF_IMAGE;
	listItem.iSubItem = 0;

	POSITION pos = graphTypeList.GetHeadPosition();
	while(pos != NULL)
	{
		listItem.iImage = i + 1;       // index of the list view item¡¯s icon 
		listItem.iItem = i;
		listItem.pszText = ( char *)_MTOL5( graphTypeList.GetNext( pos));
		pList->InsertItem(&listItem);
		i++;
	}
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CChartListPage::SetIGraphInfoManager( IGraphInfoManager *p_pIGraphInfoManager)
{
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->Release();
	m_pIGraphInfoManager = p_pIGraphInfoManager;
	if( m_pIGraphInfoManager) m_pIGraphInfoManager->AddRef();

	m_TreeChart.SetIGraphInfoManager( m_pIGraphInfoManager);
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CChartListPage::SetChartListData(const CString &strChartListData)
{
	m_strChartListData = strChartListData;

	m_TreeChart.InitBlock(strChartListData, m_nRow, m_nColumn);
	CDataOfChartListPage data(m_nRow, m_nColumn, &m_TreeChart, m_hOcxWnd);
	if( m_pIGraphInfoManager) data.SetData(m_strChartListData, m_pIGraphInfoManager);
}

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CChartListPage::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_TreeChart.SetOcxHwnd( m_hOcxWnd);
	m_toolTip.SetOcxHwnd( m_hOcxWnd);
}


//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô --------->>
/*-------------------------------------------------------------------------------
- Function    :  OnBtnFunc1
- Created at  :  2009-09-02
- Author      :  lee sashia(sashia.lee@gmail.com)
- Description :  ±â´É ´«¿¡ ¶ç°Ô ¹öÆ°À¸·Î....
-------------------------------------------------------------------------------*/
void CChartListPage::OnBtnFunc1()
{
	if (m_pSelTreeItem == NULL)
		return;
	
	switch (m_pSelTreeItem->m_iType)
	{
	case ROOT:
	case BLOCK:
		m_TreeChart.SendMessage(WM_COMMAND, IDR_HORIZONSCALE, 0);
		break;
	case VERT_SCALE:
		m_TreeChart.SendMessage(WM_COMMAND, IDR_VERT_SCALE, 0);
		break;
	case GRAPH:
		m_TreeChart.SendMessage(WM_COMMAND, IDR_TITLESCALECOLOR, 0);
		break;
	}
}

/*-------------------------------------------------------------------------------
- Function    :  OnBtnFunc2
- Created at  :  2009-09-02
- Author      :  lee sashia(sashia.lee@gmail.com)
- Description :  ±â´É ´«¿¡ ¶ç°Ô ¹öÆ°À¸·Î....
-------------------------------------------------------------------------------*/
void CChartListPage::OnBtnFunc2()
{
	if (m_pSelTreeItem == NULL)
		return;
	
	switch (m_pSelTreeItem->m_iType)
	{
	case ROOT:
	case BLOCK:
		m_TreeChart.SendMessage(WM_COMMAND, IDR_CHARTINVERT, 0);
		break;
	case VERT_SCALE:
		m_TreeChart.SendMessage(WM_COMMAND, IDR_VERT_DELETE, 0);
		break;
	case GRAPH:
		m_TreeChart.SendMessage(WM_COMMAND, IDR_DELETE, 0);
		break;
	}
}


void CChartListPage::OnSelchangedTreeChart(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	m_pSelTreeItem = NULL;
	
	if (pNMTreeView == NULL)
		return;
	
	m_pSelTreeItem = m_TreeChart.GetSelectItem(pNMTreeView->itemNew.hItem);
	if (m_pSelTreeItem)
	{
		
		switch (m_pSelTreeItem->m_iType)
		{
		case ROOT:
		case BLOCK:
			m_btnFunc1.SetWindowText(_T("Scale ¼³Á¤"));
			m_btnFunc2.SetWindowText(_T("ChartµÚÁý±â ¼³Á¤"));
			break;
		case VERT_SCALE:
			m_btnFunc1.SetWindowText(_T("Virtical Scale ¼³Á¤"));
			m_btnFunc2.SetWindowText(_T("Delete"));
			break;
		case GRAPH:
			m_btnFunc1.SetWindowText(_T("Title, Scale, Color"));
			m_btnFunc2.SetWindowText(_T("Delete"));
			break;
		}
		
		m_stcSelGroup.SetWindowText(m_pSelTreeItem->m_strName);
	}
	
	*pResult = 0;
}
//@solomon[sashia]	20090902 --- ±â´É ´«¿¡ ¶ç°Ô ---------<<

