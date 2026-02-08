// PacketPage.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "PacketPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTEXT()
#include "../Include_Chart/xUtil_Chart.h"			// for CHART_CONVERT_BEGIN
#include "../Include_Chart/IPropertyMap.h"			// for IPropertyMap

#include "ChartPropertySheet.h"
#include "PropertiesData.h"
#include "Conversion.h"
#include "DataOfPacketPage.h"
#include "ToolListData.h"						// for pPacketNameItem
#include "PacketSuperGrid.h"					// for CPacketSuperGrid

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPacketPage property page

IMPLEMENT_DYNCREATE(CPacketPage, CPropertyPage)

CPacketPage::CPacketPage() : CPropertyPage(CPacketPage::IDD)
{
	//{{AFX_DATA_INIT(CPacketPage)
	//}}AFX_DATA_INIT

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = NULL;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;
}

CPacketPage::~CPacketPage()
{
}

void CPacketPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPacketPage)
	DDX_Control(pDX, IDC_STATIC_FRAME, m_Frame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPacketPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPacketPage)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPacketPage message handlers

BOOL CPacketPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CRect rc;
	m_Frame.GetWindowRect(rc);
	ScreenToClient(rc);
	m_Frame.ShowWindow(SW_HIDE);
	if(m_PacketList.GetSafeHwnd())
			m_PacketList.MoveWindow(rc);

	CreateToolTip();
	m_PacketPagedata.Init(m_strReceiveDataFromOcx, &m_PacketList);
//	CDataOfPacketPage packetpagedata(m_strReceiveDataFromOcx, &m_PacketList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CPacketPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_PacketList.Create(WS_CHILD | WS_VISIBLE|WS_BORDER,CRect(0,0,0,0),this,0x100);
	DWORD dwNewStyle = LVS_EX_FULLROWSELECT|LVS_SORTASCENDING|LVS_EX_HEADERDRAGDROP;
	m_PacketList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,0,LPARAM(dwNewStyle));
	
	m_PacketList.InitializeGrid();

	return 0;
}


BOOL CPacketPage::OnKillActive() 
{
	(((CChartPropertySheet *)GetParent())->m_RealPacketPage).SetRealPacketInfo(GetUseRealPacketItems());
	(((CChartPropertySheet *)GetParent())->m_ChartListPage).SetInfoOfAllPacketItems(GetAllDataFromListCtrl());

	return CPropertyPage::OnKillActive();
}

BOOL CPacketPage::PreTranslateMessage(MSG* pMsg) 
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

//ÅøÆÁ
void CPacketPage::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );
	CDWordArray dwaInfo;

	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_1);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_2);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_3);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_4);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_5);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_6);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_7);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_8);
	dwaInfo.Add( IDS_PACKETSET_TOOLTIP_9);
	m_toolTip.AddControlInfo( 0x100, dwaInfo);
}


CString CPacketPage::GetReceivedAllDataFromOcx()
{
	return m_strReceiveDataFromOcx;
}

CString CPacketPage::GetAllDataFromListCtrl()
{
	UpdateData(TRUE);

	return m_PacketPagedata.GetAllDataFromListCtrl(&m_PacketList);
}

CString CPacketPage::GetUseRealPacketItems()
{
	CString szAllData = m_PacketPagedata.GetAllDataFromListCtrl(&m_PacketList);
	return m_PacketPagedata.GetUseRealPacketItems(szAllData);
}


void CPacketPage::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_CONVERT_BEGIN()
		CHART_CONVERT_STRING(	p_pIPropertyMap, g_anPropertyShortKey[	STRPACKETOPTION	], m_strReceiveDataFromOcx,	_DPV_STRPACKETOPTION	);
	CHART_CONVERT_END()

	// (2007/3/14 - Seung-Won, Bae) Dummy Packet for Date FID in Koscom
	if( !m_strReceiveDataFromOcx.IsEmpty() && m_strReceiveDataFromOcx.Find( _MTEXTN( ePacketNameItem[ CPacketSuperGrid::SECONDDATE_ITEM])) < 0)
		m_strReceiveDataFromOcx = CString( "0;") + _MTEXT( C0_SECOND_TIME) + ";;;YYYYMMDD;" + _MTEXT( C6_NO) + ";\r\n" + m_strReceiveDataFromOcx;

	// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
	m_pIPropertyMap = p_pIPropertyMap;
}

BOOL CPacketPage::OnApply() 
{
	CString strPacketOption = GetAllDataFromListCtrl();

	// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
	CHART_ADDTOSTRLIST_START( TRUE)
		CHART_ADDTOSTRLIST_STRING(	m_pIPropertyMap, g_anPropertyShortKey[	STRPACKETOPTION	], strPacketOption,	_DPV_STRPACKETOPTION	);
	CHART_ADDTOSTRLIST_END

	return CPropertyPage::OnApply();
}

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CPacketPage::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_PacketList.SetOcxHwnd( p_hOcxWnd);
	m_PacketPagedata.SetOcxHwnd( p_hOcxWnd);
	m_toolTip.SetOcxHwnd( p_hOcxWnd);
}
