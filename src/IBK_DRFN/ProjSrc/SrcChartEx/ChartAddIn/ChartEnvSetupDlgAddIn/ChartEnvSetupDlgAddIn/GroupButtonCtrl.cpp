// GroupButtonCtrl.cpp: implementation of the CGroupButtonCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "GroupButtonCtrl.h"

#include "DlgMainTabIndicatorSetUp.h"
#include "IndicatorNameListCtrl.h"	// ListCtrl
#include "IndicatorGroupButton.h"	// Button
// 2008.01.25 by LYH >>
//#include "DefineData.h"
// 2008.01.25 by LYH <<
#include "IndiConfigurationDefineData.h"
#include <assert.h>

// 2008.01.25 by LYH >>
#include "../Include_Chart/Dll_Load/IMetaTable.h"	// for _MTOL0()
// 2008.01.25 by LYH <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CGroupButtonCtrl::CGroupButtonCtrl()
{
	m_pDlgMainTabIndicatorSetUp = NULL;
	m_nSelectListCtrlIndex = 0;
	m_nSelectButtonIndex = 0;	// zero base

	m_pIPacketManager = NULL;

	// ( 2007 / 01 / 09 - Sang-Woo, Cho ) 
	m_nBtnType = 0;

// 2008.01.25 by LYH >>
	m_hOcxWnd = NULL;
// 2008.01.25 by LYH <<
}

CGroupButtonCtrl::~CGroupButtonCtrl()
{
	// (2006/4/3 - Seung-Won, Bae) Release Memory of CIndicatorGroupButton 
	while( !m_lstGroupButton.IsEmpty()) delete m_lstGroupButton.RemoveHead();

	// (2006/4/6 - Seung-Won, Bae) 
	while( !m_lstListCtrl.IsEmpty()) delete m_lstListCtrl.RemoveHead();

	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void : 
// Param  CWnd *pWnd : 
// Comments		: 부모윈도우, IndicatorList 포인터를 세팅.
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::SetData( CWnd *pWnd)
{
	m_pDlgMainTabIndicatorSetUp = (CDlgMainTabIndicatorSetUp*)pWnd;

	CFont *pFont = NULL;
	pFont = m_pDlgMainTabIndicatorSetUp->GetFont();
	if( pFont )
	{
	   LOGFONT lfGroupName;
	   pFont->GetLogFont( &lfGroupName );
	   m_font.CreateFontIndirect( &lfGroupName );
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 4
// Return void : 
// Param  const CRect& region : 
// Comments		: list ctrl 의 높이를 고정값으로 하지 않는다.
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::SetIndicatorSize(const CRect& region)
{
	m_rcTotalRegion = region;
}

// (2009/6/22 - Seung-Won, Bae) Check Market Index.
const UINT RMSG_CAN_ADD_MARKET_INDEX = ::RegisterWindowMessage( _T( "RMSG_CAN_ADD_MARKET_INDEX"));
// wParam - Index Name String
// lParam - the Pointer to receive the result Msg String address (Out) - Can be NULL.
// return value : 0 - can not add, 1 - can add.

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 08
// Return void	: 
// Param  IStringList * p_pIGraphNameList : 
// Param  IChartOCX *p_pIChartOCX : 
// Comments		: 기존의 GroupButtonCtrl을 생성할때 사용한다.
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::CreateGeneralCtrl( IStringList * p_pIIndicatorNameList, IChartOCX *p_pIChartOCX )
{
	if( !p_pIChartOCX) return;
	m_pIPacketManager = p_pIChartOCX->GetIDefaultPacketManager();
	IGraphInfoManager *pIGraphInfoManager = p_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return;

	// 2. 지표 그룹 데이터의 List를 얻어온다.
	IStringList *pIGroupNameList = pIGraphInfoManager->GetIndicatorGroupNameStringList();
	if( pIGroupNameList)
	{
		// 3. 지정된 영역에 각 Ctrl을 생성한다.
		UINT nID = 0x50000;					// 버튼과 ListCtrl의 시작 ID.
		CRect listCtrlRegion(0, 0, 0, 0), buttonRegion(0, 0, 0, 0);
		
		// 4. Indicator Button과 지표 List가 만들어질 영역을 계산한다.
		SetEachCtrlSize( pIGroupNameList->GetCount(), nID, listCtrlRegion, buttonRegion );

		// (2009/6/22 - Seung-Won, Bae) Check Market Index.
		HWND hParentDLL = ::GetParent( p_pIChartOCX->GetOcxHwnd());

		int i = 0;
		IStringList *pITotalIndicatorNameList = p_pIChartOCX->CreateObjectIStringList();
		ILPCSTR szGroupName = NULL;
		POSITION pos = pIGroupNameList->GetHeadPosition();
		while( pos )
		{
			pIGroupNameList->GetNext( pos, szGroupName);
			if( szGroupName.IsEmpty() || !strcmp( szGroupName, _MTEXT( C7_SPECIAL))) continue;

			// listCtrl 과 button 생성
			IStringList *pIIndicatorNameList = pIGraphInfoManager->GetIndicatorNameStringList( szGroupName);
			if( !pIIndicatorNameList) continue;

			// (2009/6/22 - Seung-Won, Bae) Check Market Index.
			ILPCSTR szGraphName = NULL;
			POSITION psnGraphName = pIIndicatorNameList->GetHeadPosition();
			IStringList *pIIndicatorNameList2 = p_pIChartOCX->CreateObjectIStringList();
			while( psnGraphName)
			{
				pIIndicatorNameList->GetNext( psnGraphName, szGraphName);
				// 20130509 이상신 : 시장지표도 보여주기로 함.(IBK)
//				if( szGroupName == CString( _MTEXT( C7_MARKET)) || szGraphName == CString( _MTEXT( C2_VOLUME_AMOUNT)))
				if( szGraphName == CString( _MTEXT( C2_VOLUME_AMOUNT)))
				{
					if( !::SendMessage( hParentDLL, RMSG_CAN_ADD_MARKET_INDEX, ( WPARAM)( const char *)szGraphName, NULL))
						continue;
				}
				pIIndicatorNameList2->AddTail( szGraphName);
			}

			// 20130509 이상신 : 가격지표에 특수지표 중 (그물차트, 대기매물, 일목균형표)를 포함하여 보여주기로 함.(IBK) --->>
			// (그물차트, 대기매물, 일목균형표)를 가격지표로 옮김.
/*			if (szGroupName == CString( _MTEXT( C7_PRICE)))
			{
				IStringList *pSpecialIndList = pIGraphInfoManager->GetIndicatorNameStringList( CString(_MTEXT( C7_SPECIAL)));
				psnGraphName = pSpecialIndList->GetHeadPosition();
				while( psnGraphName)
				{
					pSpecialIndList->GetNext( psnGraphName, szGraphName);
					if (szGraphName == CString(_MTEXT(C2_RAINBOW_CHART)) ||
						szGraphName == CString(_MTEXT(C2_OVERHANGING_SUPPLY)) ||
						szGraphName == CString(_MTEXT(C2_OBVIOUSLY_BALANCE_CHART)))
					{
						pIIndicatorNameList2->AddTail( szGraphName);
					}
				}
				pSpecialIndList->Release();
			}
*/			// -------------<<

			pIIndicatorNameList->Release();
			if( pIIndicatorNameList2->GetCount() <= 0) continue;

// 2008.02.12 by LYH >>		
			//if( CreateListCtrlAndButton( nID, i, szGroupName, listCtrlRegion, buttonRegion,  NULL, pIIndicatorNameList, false))
			CString strUIText = _MTOL7(szGroupName);
			if( CreateListCtrlAndButton( nID, i, strUIText, listCtrlRegion, buttonRegion,  NULL, pIIndicatorNameList2, false))
// 2008.02.12 by LYH >>
			{
				// "전체지표" group에 추가. (특수지표는 추가안한다.)
				if( !strstr( szGroupName, CIndiConfigurationDefineData::GROUPNAME_SPECIAL))
				{
					ILPCSTR szGraphName = NULL;
					POSITION psnGraphName = pIIndicatorNameList2->GetHeadPosition();
					while( psnGraphName)
					{
						pIIndicatorNameList2->GetNext( psnGraphName, szGraphName);
						pITotalIndicatorNameList->AddTail( szGraphName);
					}
				}

				i++;
			}
			pIIndicatorNameList2->Release();
		}

		// 전체 버튼
		CreateListCtrlAndButton(nID, i, CIndiConfigurationDefineData::GROUPNAME_TOTAL,  listCtrlRegion, buttonRegion, NULL, pITotalIndicatorNameList, true);
		pITotalIndicatorNameList->Release();

		// 해당 버튼&list 활성화.
		SelectGroupButton( IsCheckSpecialIndicator( p_pIIndicatorNameList) );

		//sy 2005.11.02.
		//-> group갯수가 틀려질경우 listCtrl의 위치를 조정.
		MoveWindowInAllListCtrl( pIGroupNameList->GetCount(), listCtrlRegion);
		pIGroupNameList->Release();
	}

	pIGraphInfoManager->Release();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 08
// Return void	: 
// Param  IStringList * p_pIGraphNameList : 
// Param  IChartOCX *p_pIChartOCX : 
// Comments		: 사용지표용 GroupButtonCtrl을 생성할때 사용한다.
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::CreateUseCtrl( IStringList * p_pIGraphNameList, IChartOCX *p_pIChartOCX )
{
	if( !p_pIChartOCX) return;
	m_pIPacketManager = p_pIChartOCX->GetIDefaultPacketManager();
	IGraphInfoManager *pIGraphInfoManager = p_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return;

	if( p_pIGraphNameList )
	{
		// 3. 지정된 영역에 각 Ctrl을 생성한다.
		UINT nID = 0x60000;					// 버튼과 ListCtrl의 시작 ID.
		CRect listCtrlRegion(0, 0, 0, 0), buttonRegion(0, 0, 0, 0);
		
		// 4. Indicator Button과 지표 List가 만들어질 영역을 계산한다.
		SetEachCtrlSize( 1, nID, listCtrlRegion, buttonRegion );

		IStringList *pIUseGraphNameList		= p_pIChartOCX->CreateObjectIStringList();
		IStringList *pIUseIndicatorNameList	= p_pIChartOCX->CreateObjectIStringList();

		ILPCSTR szGraphName = NULL, szIndicatorName = NULL;
		POSITION psnGraphName = p_pIGraphNameList->GetHeadPosition();
		while( psnGraphName)
		{
			p_pIGraphNameList->GetNext( psnGraphName, szGraphName);
			IGraphInfo *pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
			if( pIGraphInfo)
			{
				ILPCSTR szIndicatorName = pIGraphInfo->GetIndicatorNameString();
				pIUseGraphNameList->AddTail(		( const char *)szGraphName);
				pIUseIndicatorNameList->AddTail(	( const char *)szIndicatorName);
				pIGraphInfo->Release();
			}
		}


		// 전체 버튼
		CString strUsedIndicator;
		strUsedIndicator.LoadString( IDS_USED_INDICATOR);
 		CreateListCtrlAndButton(nID, 0, strUsedIndicator,  listCtrlRegion, buttonRegion, pIUseGraphNameList, pIUseIndicatorNameList, false);
		pIUseGraphNameList->Release();
		pIUseIndicatorNameList->Release();

		// 해당 버튼&list 활성화.
		SelectGroupButton( IsCheckSpecialIndicator(p_pIGraphNameList) );

		//sy 2005.11.02.
		//-> group갯수가 틀려질경우 listCtrl의 위치를 조정.
		MoveWindowInAllListCtrl( 1, listCtrlRegion);
	}

	pIGraphInfoManager->Release();
}



//sy 2005.11.02.
//-> group갯수가 틀려질경우 listCtrl의 위치를 조정.
void CGroupButtonCtrl::MoveWindowInAllListCtrl(const int nGroupCount, const CRect& curListCtrlRegion)
{
	if(m_lstListCtrl.GetCount() == nGroupCount)
		return;

	if(m_lstListCtrl.GetCount() <= 0 || m_lstGroupButton.GetCount() <= 0)
		return;

	CRect bottonRegion(0, 0, 0, 0);
	CIndicatorGroupButton* pButton = m_lstGroupButton.GetTail();
	pButton->GetWindowRect(bottonRegion);
	m_pDlgMainTabIndicatorSetUp->ScreenToClient(bottonRegion);

	CRect moveListCtrlRegion = curListCtrlRegion;
	moveListCtrlRegion.top = bottonRegion.bottom + 1;

	POSITION listPos = m_lstListCtrl.GetHeadPosition();
	while(listPos != NULL)
	{
		CIndicatorNameListCtrl* pListCtrl = m_lstListCtrl.GetNext(listPos);
		if(pListCtrl == NULL)
			continue;
		pListCtrl->MoveWindow(moveListCtrlRegion.left, moveListCtrlRegion.top, moveListCtrlRegion.Width(), moveListCtrlRegion.Height());
	}
}
//sy end

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void : 
// Param  const int nGroupCount : 
// Param  UINT &nID : 
// Param  CRect &rcListCtrlRegion : 
// Param  CRect &rcButtonRegion : 
// Comments		: 
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::SetEachCtrlSize( const int nGroupCount, UINT &nID, CRect &rcListCtrlRegion, CRect &rcButtonRegion )
{
	// 2. ButtonCtrl region
	CButton *pBtn = (CButton *)m_pDlgMainTabIndicatorSetUp->GetDlgItem(IDC_BTN_TOP_INDICATOR);
	if( pBtn == NULL )
		return;

	// 3. IDC_BTN_TOP_INDICATOR Button의 위치 정보를 얻어온다.
	CRect rcBtnRect;
	pBtn->GetWindowRect( rcBtnRect );
	m_pDlgMainTabIndicatorSetUp->ScreenToClient( rcBtnRect );

	m_rcFirstIndicatorBtn = rcBtnRect;

	rcListCtrlRegion = m_rcTotalRegion;
	rcButtonRegion = m_rcTotalRegion;

	rcButtonRegion.bottom = m_rcTotalRegion.top + ( rcBtnRect.Height() * ( nGroupCount - 1 ) );
	rcListCtrlRegion.top = rcButtonRegion.bottom + 1;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 6
// Return bool : 
// Param  const UINT& nID : 
// Param  const int nIndex : 
// Param  const CString& strGroupName : 
// Param  const CRect& listCtrlRegion : 
// Param  const CRect& buttonRegion : 
// Param  CList<CString : 
// Param  CString>* pIndicatorNameList : 
// Param  const bool bIsSorting : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CGroupButtonCtrl::CreateListCtrlAndButton(const UINT& nID, const int nIndex, const char *p_szGroupName, const CRect& listCtrlRegion,
											   const CRect& buttonRegion, IStringList * pGraphNameList, IStringList * pIndicatorNameList, const bool bIsSorting)
{
	if( !CreateListCtrl(nID, nIndex, listCtrlRegion, p_szGroupName, pGraphNameList, pIndicatorNameList, bIsSorting))
		return false;

	// Button 생성
	CreateButton(nID, nIndex, p_szGroupName, GetButtonRegion( buttonRegion, nIndex ) );
	return true;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 6
// Return bool : 
// Param  const UINT& nID : 
// Param  const int nIndex : 
// Param  const CRect& region : 
// Param  const CString& strGroupName : 
// Param  CList<CString : 
// Param  CString>* pIndicatorNameList : 
// Param  const bool bIsSorting : 
// Comments		: 
//-----------------------------------------------------------------------------
bool CGroupButtonCtrl::CreateListCtrl(const UINT& nID, const int nIndex, const CRect& region, const CString& strGroupName,
									  IStringList * pGraphNameList, IStringList * pIndicatorNameList, const bool bIsSorting)
{
	if( !m_pIPacketManager || pIndicatorNameList == NULL || pIndicatorNameList->GetCount() <= 0 ) return false;

	CIndicatorNameListCtrl *pListCtrl = new CIndicatorNameListCtrl( this, strGroupName, nIndex, m_pIPacketManager);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE| WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER;
	if(bIsSorting)
		dwStyle = WS_CHILD | WS_VISIBLE| WS_BORDER | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_SORTASCENDING;

	pListCtrl->Create( dwStyle, region, m_pDlgMainTabIndicatorSetUp, nID + ( nIndex * 2 + 1 ) );
	
	// 지표명이 없을 경우엔 button 생성 안함.
	if(  !pListCtrl->Initialize( strGroupName, pGraphNameList, pIndicatorNameList) )
	{
		delete pListCtrl;
		return false;
	}

	pListCtrl->ShowWindow(SW_HIDE);

	m_lstListCtrl.AddTail(pListCtrl);
	return true;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 6
// Return void : 
// Param  const UINT& nID : 
// Param  const int nIndex : 
// Param  const CString& strWndText : 
// Param  const CRect& region : 
// Comments		: 
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::CreateButton(const UINT& nID, const int nIndex, const CString& strWndText, const CRect& region)
{
	CIndicatorGroupButton *pButton = new CIndicatorGroupButton( this, nIndex );
	pButton->Create( strWndText, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, region, m_pDlgMainTabIndicatorSetUp, nID+(nIndex*2) );
	pButton->SetFont( &m_font, TRUE );
	
	m_lstGroupButton.AddTail(pButton);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 6
// Return CRect : 
// Param  const CRect& baseRegion : 
// Param  const int nIndex  const : 
// Comments		: 
//-----------------------------------------------------------------------------
CRect CGroupButtonCtrl::GetButtonRegion(const CRect& baseRegion, const int nIndex)
{
	CRect rect = baseRegion;

	rect.bottom = rect.top + ( nIndex * m_rcFirstIndicatorBtn.Height() ); 
	rect.top = rect.bottom - m_rcFirstIndicatorBtn.Height();

	return rect;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 11
// Return void : 
// Param  const bool bIsCurSpecialIndicator : 
// Comments		: 
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::SelectGroupButton(const bool bIsCurSpecialIndicator)
{
	// 현재 "특수지표" 아닌 경우 -> 일반지표 활성화.
	if(!bIsCurSpecialIndicator)
	{
		CIndicatorGroupButton* pSelectButton = GetSelectGroupButton();
		if(pSelectButton == NULL)
			return;
		pSelectButton->SetState(TRUE);

		CIndicatorNameListCtrl* pSelectListCtrl = GetSelectListCtrl();
		if(pSelectListCtrl == NULL)
			return;
		pSelectListCtrl->ShowWindow(SW_SHOW);

		return;
	}

	// 현재 "특수지표" 인 경우.
	int nIndex = 0;
	POSITION pos = m_lstGroupButton.GetHeadPosition();
	while( pos )
	{
		CButton* pButton = m_lstGroupButton.GetNext(pos);
		assert(pButton != NULL);
		CString strText;
		pButton->GetWindowText(strText);
		if(strText.Find(CIndiConfigurationDefineData::GROUPNAME_SPECIAL) != -1)
		{
			OnGroupButtonDown(nIndex, false);
			break;
		}
		nIndex++;
	}
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 23
// Return CIndicatorGroupButton* : 
// Param  int nIndex/* = -1*/ : 
// Comments		: 현재 선택한 IndicatorGroupButton을 가져온다.
//-----------------------------------------------------------------------------
CIndicatorGroupButton* CGroupButtonCtrl::GetSelectGroupButton(int nIndex/* = -1*/)
{
	if(m_lstGroupButton.IsEmpty())
		return NULL;
	
	POSITION pos;
	if(nIndex == -1)
		pos = m_lstGroupButton.FindIndex(m_nSelectButtonIndex);
	else
		pos = m_lstGroupButton.FindIndex(nIndex);

	return m_lstGroupButton.GetAt(pos);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 23
// Return CIndicatorNameListCtrl* : 
// Param  int nIndex /* = -1*/ : 
// Comments		: 해당 인덱스의 listCtrl을 가져온다. (디폴트는 현재선택한 그룹의 listCtrl이다.)
//-----------------------------------------------------------------------------
CIndicatorNameListCtrl *CGroupButtonCtrl::GetSelectListCtrl( int nIndex /* = -1*/ )
{
	if(m_lstListCtrl.IsEmpty())
		return NULL;

	POSITION pos;
	if(nIndex == -1)
		pos = m_lstListCtrl.FindIndex(m_nSelectButtonIndex);
	else
		pos = m_lstListCtrl.FindIndex(nIndex);
	
	return m_lstListCtrl.GetAt(pos);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 23
// Return bool : 
// Param  CList< CString : 
// Param  CString > &p_pIGraphNameList : 
// Comments		: 현재 특수지표를 이용중인지 확인한다.
//-----------------------------------------------------------------------------
bool CGroupButtonCtrl::IsCheckSpecialIndicator( IStringList *p_pIGraphNameList)
{
	ILPCSTR szGraphName = NULL;
	POSITION pos = p_pIGraphNameList->GetHeadPosition();
	while(pos)
	{
		p_pIGraphNameList->GetNext( pos, szGraphName);
		if( g_iMetaTable.IsSpecialGroupGraph( szGraphName)) return true;
	}
	return false;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 23
// Return void : 
// Param  int nIndex : 
// Param  const bool bIsDlgCreat : 
// Comments		: IndicatorGroup Button을 눌렀을때.
//-----------------------------------------------------------------------------
void CGroupButtonCtrl::OnGroupButtonDown(int nIndex, const bool bIsDlgCreat)
{
	ReleaseFocusItem();
	ReleaseAllFocusSpecialTab();

	// 이전에 눌려져 있던 버튼은 release시킨다. 그리고 해당 ListCtrl도 Hide시킨다.
	CIndicatorGroupButton* pButton = NULL;
	CIndicatorNameListCtrl* pListCtrl = NULL;

	if(m_nSelectButtonIndex != nIndex)
	{
		// 버튼 Release
		pButton = GetSelectGroupButton();
		if(pButton == NULL)
			return;
		pButton->SetState(FALSE);

		// ListCtrl Hide
		pListCtrl = GetSelectListCtrl();
		if(pListCtrl == NULL)
			return;
		pListCtrl->ShowWindow(SW_HIDE);
	}

	// 현재 선택되어진 Button값을 세팅한다.
	m_nSelectButtonIndex = nIndex;

	// 버튼 Press
	pButton = GetSelectGroupButton();
	if(pButton == NULL)
		return;
	pButton->SetState(TRUE);

	// ListCtrl Show
	pListCtrl = GetSelectListCtrl();
	if(pListCtrl == NULL)
		return;
	pListCtrl->ShowWindow(SW_SHOW);

	// 지표 이름을 표시한다.
	CString szGroupName;
	pButton->GetWindowText( szGroupName );	
	
	if( szGroupName.Find( '&' ) )
	{
		szGroupName.Replace("&", "&&");
	}

// (2007/1/24 - Seung-Won, Bae) Do not select first Indicator.
//	m_pDlgMainTabIndicatorSetUp->SetDlgItemText( IDC_STATIC_INDICATOR_NAME, szGroupName );
//	m_pDlgMainTabIndicatorSetUp->GetDlgItem(IDC_STATIC_INDICATOR_NAME)->UpdateWindow();
//	CString strIndicatorName, strGraphName;
//	GetFirstCheckItem( strIndicatorName, strGraphName);
//	m_pDlgMainTabIndicatorSetUp->SelChangeGroupButton( strIndicatorName, strGraphName);
	//sy 2004.05.21. "특수차트"일 경우 CSetting_All_Indicator::OnInitDialog() 부분에서
	// 처리를 하기 때문에 true/false인지를 둔것임
	// 처리 안한 경우 : list에 선택표시가 2개가 된다.(무조건 첫번째 지표, 실제 체크된 지표)
	if(bIsDlgCreat)
		SetFocusItem();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseFocusItem
 - Created at  :  2004-03-31   11:39
 - Author      :  최종찬
 - Description :  포커스를 푼다.
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::ReleaseFocusItem()
{
	CIndicatorNameListCtrl* pListCtrl = GetSelectListCtrl();
	if(pListCtrl == NULL)
		return;
	pListCtrl->ReleaseFocusItem();
	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ReleaseAllFocusSpecialTab
 - Created at  :  2004-03-31   15:13
 - Author      :  최종찬
 - Description :  특수지표의 모든 아이템을 uncheck한다.
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::ReleaseAllFocusSpecialTab()
{
	CIndicatorNameListCtrl* pListCtrl = GetSelectListCtrl();
	if(pListCtrl == NULL)
		return;

	//특수지표의 경우..
	if(pListCtrl->GetGroupName().Find(CIndiConfigurationDefineData::GROUPNAME_SPECIAL) >= 0 )
		ModifyCheckWhenSpecialIndicator(_T(""));
	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetFocusItem
 - Created at  :  2004-03-31   10:01
 - Author      :  최종찬
 - Description :  해당 Item에 포커스를 준다.
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::SetFocusItem()
{
	CIndicatorNameListCtrl* pListCtrl = GetSelectListCtrl();
	if(pListCtrl == NULL)
		return;

	pListCtrl->SetFocusItem(m_nSelectListCtrlIndex);
	return;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  ModifyCheckWhenSpecialIndicator
 - Created at  :  2004-03-31   16:09
 - Author      :  최종찬
 - Description :  (특수지표의 경우에서만) 현재 선택된 지표명과 ListCtrl의 Item Text를 비교하여
				  체크상태를 설정한다.
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::ModifyCheckWhenSpecialIndicator(const CString &strIndicatorName)
{
	CIndicatorNameListCtrl* pListCtrl = GetSelectListCtrl();
	if(pListCtrl == NULL)
		return;

	for(int i = 0 ; i < pListCtrl->GetItemCount() ; i++) {
		if(strIndicatorName.CompareNoCase(pListCtrl->GetItemText(i, 0)) == 0)
			pListCtrl->SetCheck(i);
		else
			pListCtrl->SetCheck(i, false);
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetCheck
 - Created at  :  2004-03-31   09:37
 - Author      :  최종찬
 - Description :  현재차트에 표시되어 있는 지표를 조사하여  ListCtrl 해당지표명Item에 
				  체크표시를 하도록 ListCtrl의 멤버변수를 호출.
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::SetCheck(IStringList * p_pIGraphNameList)
{
	POSITION pos = m_lstListCtrl.GetHeadPosition();
	
	while(pos)
	{
		CIndicatorNameListCtrl* pListCtrl = m_lstListCtrl.GetNext(pos);
		pListCtrl->SetCheckSeletecItem(p_pIGraphNameList);
	}
	
	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetFirstCheckItem
 - Created at  :  2004-03-31   10:01
 - Author      :  최종찬
 - Description :  ListCtrl에서 첫번째 체크된 Item의 문자열을 가져온다. 없으면 맨윗에 놈으로..
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::GetFirstCheckItem( CString &p_strIndicatorName, CString &p_strGraphName)
{
	p_strIndicatorName.Empty();
	p_strGraphName.Empty();
	CIndicatorNameListCtrl* pListCtrl = GetSelectListCtrl();
	
	if(pListCtrl == NULL) return;

	CString szIndicatorName;

	for(int i = 0 ; i < pListCtrl->GetItemCount() ; i++) 
	{
		if(pListCtrl->GetCheck(i) == TRUE)
		{
			m_nSelectListCtrlIndex = i;

			szIndicatorName = pListCtrl->GetItemText(i, 0);

			if( szIndicatorName.Find( '&' ) )
			{
				szIndicatorName.Replace("&", "&&");
			}

			m_pDlgMainTabIndicatorSetUp->SetDlgItemText( IDC_STATIC_INDICATOR_NAME, szIndicatorName );
// 2008.01.25 by LYH >>
			//p_strIndicatorName	= pListCtrl->GetItemText( i, 0);
			p_strIndicatorName	= _LTOM2(pListCtrl->GetItemText( i, 0));
// 2008.01.25 by LYH <<
			p_strGraphName		= pListCtrl->GetItemText( i, 1);
			return;
		}
	}

	m_nSelectListCtrlIndex = 0;

	szIndicatorName = pListCtrl->GetItemText(0, 0);
	if( szIndicatorName.Find( '&' ) )
	{
		szIndicatorName.Replace("&", "&&");
	}

	m_pDlgMainTabIndicatorSetUp->SetDlgItemText( IDC_STATIC_INDICATOR_NAME, szIndicatorName );

// 2008.01.25 by LYH >>
	//p_strIndicatorName	= pListCtrl->GetItemText( 0, 0);
	p_strIndicatorName	= _LTOM2(pListCtrl->GetItemText( 0, 0));
// 2008.01.25 by LYH <<
	p_strGraphName		= pListCtrl->GetItemText( 0, 1);
	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetAllUnCheck
 - Created at  :  2005-01-24   10:57
 - Author      :  최종찬
 - Description :  지표명ListCtrl의 모든 아이템을 UnCheck한다.
 -----------------------------------------------------------------------------------------*/
void CGroupButtonCtrl::SetAllUnCheck()
{
	POSITION pos = m_lstListCtrl.GetHeadPosition();
	while(pos)
	{
		CIndicatorNameListCtrl* pListCtrl = m_lstListCtrl.GetNext(pos);
		pListCtrl->SetAllUnCheck();
	}
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 10
// Return BOOL	: 
// Param  CString strIndicatorName : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CGroupButtonCtrl::AddGraph( CString strIndicatorName )
{


	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2007 / 01 / 10
// Return BOOL	: 
// Param  CString strIndicatorName : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CGroupButtonCtrl::RemoveGraph( CString strIndicatorName )
{
	return TRUE;
}