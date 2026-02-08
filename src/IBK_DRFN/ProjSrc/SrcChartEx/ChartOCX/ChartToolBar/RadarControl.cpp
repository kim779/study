// RadarControl.cpp: implementation of the CRadarControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RadarControl.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "Definition.h"
#include "ToolStatic.h"									// for CToolStatic

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Radar.h"
//#include "ObjTool.h"
#include "ToolBarDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadarControl::CRadarControl()
{
//	m_pMiniToolBarDlg = NULL;
	m_bOnRadar = FALSE;
	m_radar = NULL;
	m_static = NULL;

	// (2003.12.03, 배승원) Radar Chart에게 전달할 Data View 정보를 관리한다.
	//		(처음 생성시와 Zoom Full Mode가 해제되는 시점에서는 SetDataViewInfo가 호출되지 않아 수동 호출한다.)
	m_nDataSize			= 0;
	m_nDataStartIndex	= 0;
	m_nOnePageDataCount	= 1;

	// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
	m_pIChartCtrl = NULL;
}

CRadarControl::~CRadarControl()
{
	// 1. Radar Button Window를 제거한다.
	if( m_radar)
	{
		delete m_radar;
		m_radar = NULL;
	}
	// 2. Radar Button Window를 제거한다.
	if( m_static)
	{
		delete m_static;
		m_static = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.03
//
//	목적	:	Zoom Full Mode 여부와 조정된 Data View 정보를 전달받아 Radar Chart에 반영한다.
//
//	수정	:	(2003.12.03) Zoom Full Mode의 처리도 겸한다.
//					(Radar의 경우 Zoom Full Mode여도 Data를 지속적으로 반영한다.)
///////////////////////////////////////////////////////////////////////////////
void CRadarControl::SetDataViewInfo( int nDataSize, int nDataStartIndex, int nOnePageDataCount, BOOL bWholeView)
{
	// 1. 값의 유효성을 먼저 확인한다.
//	if( !m_pMiniToolBarDlg) return;

	// (2003.12.03, 배승원) Radar Chart에게 전달할 Data View 정보를 관리한다.
	//		(처음 생성시와 Zoom Full Mode가 해제되는 시점에서는 SetDataViewInfo가 호출되지 않아 수동 호출한다.)
	m_nDataSize			= nDataSize;
	m_nDataStartIndex	= nDataStartIndex;
	m_nOnePageDataCount	= nOnePageDataCount;

	// 2. Radar Chart에게 Data View Info를 전달한다. (이때 Data Size는 0일 수 있다.)
	if( m_radar) m_radar->SetDataViewInfo( nDataSize, nDataStartIndex, nOnePageDataCount, bWholeView);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.02
//
//	분석	:	Radar Button을 생성한다.
///////////////////////////////////////////////////////////////////////////////
//void CRadarControl::CreateRadarButton(CFont* pFont, CMiniToolBarDlg* MiniToolBarDlg, CRect controlItemRect, CObjTool* objTool, CString filePath)
//{
//	// 1. 먼저 내부 ToolBar Object Pointer의 유효성을 확인하고 관리한다.
//	if( !MiniToolBarDlg) return;
//	m_pMiniToolBarDlg = MiniToolBarDlg;
//
//	// 2. Data Index를 보여주기 위한 Font를 관리한다.
//	m_pFont = pFont;
//
//	// 3. Radar Chart를 제어하는 Button을 생성한다.
////	MiniToolBarDlg->CreateButtonItem( &m_radarButton, objTool->GetName(), controlItemRect, IDC_RADARBUTTON, objTool->GetType());
//	m_radarButton.Create( objTool->GetName(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, controlItemRect, MiniToolBarDlg, IDC_RADARBUTTON);
//	MiniToolBarDlg->m_allControlIdList.AddTail( IDC_RADARBUTTON);
//	MiniToolBarDlg->m_allControlRectList.AddTail( controlItemRect);
//	m_radarButton.SetButtonStyle( BS_OWNERDRAW);
//	m_radarButton.SizeToContent();
//	m_radarButton.SetToolOption( objTool->GetType());
//	m_radarButton.SetToolTipText( objTool->GetName());
//	m_radarButton.SetMiniToolBarDlg( MiniToolBarDlg);
//
//}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.12.02
//
//	분석	:	Radar Button의 Click에 따라 Radar Chart를 생성하거나 제거한다.
//
//	수정	:	(2003.12.03) Zoom Full Mode에서도 Radar를 생성 삭제할 수 있도록 한다.
///////////////////////////////////////////////////////////////////////////////
//void CRadarControl::RadarButtonDown()
//{
//	// 1. 내부 ToolBar Object Pointer를 확인한다.
//
//	if( !m_pMiniToolBarDlg) return;
//
//	// 2. Button의 Click후 바로 Focus를 없앤다. (왜?)
//	m_radarButton.SendMessage(WM_KILLFOCUS);
//
//	// 3. Radar가 이미 생성된 경우 삭제를 처리한다.
//	if( m_bOnRadar)
//	{
//		// 3.1 먼저 Radar Button을 Pop한다. (왜 두개의 State로 관리하는지..?)
//		m_radarButton.SetState( FALSE);
//		m_radarButton.SetButtonState( FALSE);
//		// 3.2 Radar Button Window를 제거한다.
//		if( m_radar)
//		{
//			m_radar->DestroyWindow();
//			delete m_radar;
//			m_radar = NULL;
//		}
//		// 3.3 Radar Button Window를 제거한다.
//		if( m_static)
//		{
//			m_static->DestroyWindow();
//			delete m_static;
//			m_static = NULL;
//		}
//	}
//	// 4. Radar Chart가 없어 생성하는 경우를 처리한다.
//	else
//	{
//		// 4.1 먼저 Radar Button을 Push한다. (왜 두개의 State로 관리하는지..?)
//		m_radarButton.SetState( TRUE);
//		m_radarButton.SetButtonState( TRUE);
//
//		// 4.2 ToolBar의 Client 영역 크기를 확인한다.
//		//		(Radar Chart와 Radar Status의 위치와 크기 산출에 이용한다.)
//		CRect rctToolClient;
//		m_pMiniToolBarDlg->GetClientRect( &rctToolClient);	
//
//		// 4.3 Radar Flat Button을 생성한다.
//		// 4.3.1 Radar Flat Button의 위치를 산출한다.
//		CRect rctRadarItem( rctToolClient);
//		rctRadarItem.top	= rctToolClient.top	+ 18;
//		rctRadarItem.bottom	= rctRadarItem.top + _TOOLRADARCONTROL_RADAR_HEIGHT;
//		rctRadarItem.right -= 120;
//		// 4.3.2 Radar Flat Button을 생성한다.
//		m_radar = new CRadar();
//		m_radar->Create( NULL, "radar", WS_CHILD | WS_VISIBLE | BS_FLAT | BS_OWNERDRAW, rctRadarItem, m_pMiniToolBarDlg, IDC_RADAR);
//		// 4.3.3 Radar Chart가 Graph를 그릴 수 있도록 Packet 정보를 설정한다.
//		CString data = "종가";
//		// (2003.12.04, 배승원) 구간 선택을 통지할 내부 ToolBar Dialog의 Pointer도 함께 설정한다.
//		m_radar->SetData(m_pMiniToolBarDlg->GetPacketList(), data, m_pMiniToolBarDlg->GetOcxWnd());
//
//		// 4.4 Radar Status Static을 생성한다.
//		// 4.4.1 Radar Status Static의 위치를 산출한다.
//		rctRadarItem.left	= rctRadarItem.right;
//		rctRadarItem.right	= rctToolClient.right;
//		rctRadarItem.DeflateRect( 0, 4);
//		// 4.4.2 Radar Status Static을 생성한다.
//		m_static = new CStatic();
//		m_static->Create( _T( ""), WS_CHILD | WS_VISIBLE | SS_CENTER, rctRadarItem, ( CWnd *)m_pMiniToolBarDlg);
//		// 4.4.3 Radar Status Static을 생성한다.
//		m_static->SetFont( m_pFont);
//
//		// 4.5 Radar Flat Button에게 Status Static을 설정한다.
//		m_radar->SetRadarStatus( m_static);
//
//		// (2003.12.03, 배승원) Radar Chart에게 전달할 Data View 정보를 전달한다.
//		//		(Radar Chart의 경우 Zoom Full Mode를 별도로 설정받지 않는다.)
//		m_radar->SetDataViewInfo( m_nDataSize, m_nDataStartIndex, m_nOnePageDataCount, pMainOcxWnd->IsWholeView());
//	}
//
//	m_bOnRadar = !m_bOnRadar;
//}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2004.03.11
//
//	목적	:	ToolBar의 크기가 변경되는 경우, Radar의 배치도 조정토록한다.
///////////////////////////////////////////////////////////////////////////////
void CRadarControl::OnToolBarPositionChanged( CRect p_rctToolRect)
{
	// 1. 먼저 Radar가 실행중인가 확인한다.
//	if( !m_bOnRadar) return;

	// 종찬(04/11/04) 수정.
	// 2. Radar Input Button의 위치를 설정한다.
	CRect rctRadarItem (0, 0, p_rctToolRect.Width(), p_rctToolRect.Height());

	rctRadarItem.top	= rctRadarItem.top	+ TB_BUTTON_HEIGHT;
	rctRadarItem.bottom	= rctRadarItem.top + TB_RADAR_HEIGHT; 
	rctRadarItem.right -= TB_RADAR_STATIC_WIDTH;
	m_radar->MoveWindow( rctRadarItem);
	// 4.4 Radar Output Static의 위치를 조정한다.
	rctRadarItem.left	= rctRadarItem.right;
	rctRadarItem.right	= p_rctToolRect.Width();
	rctRadarItem.DeflateRect( 0, 4);
	m_static->MoveWindow( rctRadarItem);
}



/*-----------------------------------------------------------------------------------------
 - Function    :  Create
 - Created at  :  2004-11-04   10:58
 - Author      :  최종찬
 - Description :  CCtrlFactory에서 이 함수를 호출하여 RadarChartControl을 생성한다. 
				  (배차장님 소스 90% 카피. ^^; )
 -----------------------------------------------------------------------------------------*/
bool CRadarControl::Create(CWnd* pParent, UINT nID)
{
	CRect rectToolClient;
	pParent->GetClientRect(&rectToolClient);
	
	// 1. Radar Chart 생성.
	CRect rectRadarItem(rectToolClient);
	rectRadarItem.top = rectRadarItem.bottom - TB_RADAR_HEIGHT;
	rectRadarItem.right -= TB_RADAR_STATIC_WIDTH;

	m_radar = new CRadar( m_pIChartCtrl);
	m_radar->Create( NULL, "radar", WS_CHILD|WS_VISIBLE|BS_FLAT|BS_OWNERDRAW, rectRadarItem, pParent, nID);
	CString name = _MTEXT( C0_CLOSE);

	CToolBarDlg* pToolBarDlg = (CToolBarDlg*)pParent;
	m_radar->SetData(pToolBarDlg->GetPacketList(), pToolBarDlg->GetPacketListManager(), pToolBarDlg->GetXScaleManager(), name);

	// 2. Radar Static 생성.
	rectRadarItem.left = rectRadarItem.right;
	rectRadarItem.right = rectToolClient.right;
	rectRadarItem.DeflateRect(0, 4);

	m_static = new CToolStatic();
	m_static->Create(_T(""), WS_CHILD|WS_VISIBLE|SS_CENTER, rectRadarItem, pParent);
	m_static->SetFont(((CToolBarDlg*)pParent)->GetResourceFont());

	// Radar Chart에게 Status Static을 설정한다.
	m_radar->SetRadarStatus(m_static);

	m_radar->SetDataViewInfo(m_nDataSize, m_nDataStartIndex, m_nOnePageDataCount, ((CToolBarDlg*)pParent)->IsWholeView());
	
	return true;
}


