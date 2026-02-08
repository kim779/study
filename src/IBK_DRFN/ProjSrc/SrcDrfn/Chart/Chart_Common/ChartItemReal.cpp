// ChartItemReal.cpp: implementation of the CChartItem's Real Interface.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MultiChart.h"

//======================================================================================
// 2005. 08. 17 added by sy,nam
// 소스 세이프 상에서 하나의 파일로 관리하기 위해
// 아래 내용을 추가한다.
// 이 파일의 실제 위치는 Multichart 폴더에 있다.
//======================================================================================
#ifdef _MULTICHART
	#include "MultichartDlg.h"
#endif

#ifdef _COMPARECHART
	#include "../CompareChart/ChartItem.h"
	#include "../CompareChart/CompareChartDlg.h"
#endif

#ifdef _WEEKCHART
	#include "../WeekChart/ChartItem.h"
	#include "../WeekChart/WeekChartDlg.h"
#endif

#ifdef _FSPEEDJUMUN_EAGLE
	#include "ChartItem.h"
	#include "../HSJumunDlg_Eagle.h"
#endif

#ifdef  _FSPEEDJUMUN
	#include "ChartItem.h"
	#include "../HSJumunDlg.h"
#endif
//======================================================================================


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2005/8/16 - Seung-Won, Bae) Updated for Hanwha Platform
//	Updated Interface List
//		*. m_plRealSettingInfoList(CRealSettingInfoList *) does not have "S31.open". it has "50.3"
//			in Hanwha Platform, The Packet and Field are not used with name. They are used with ID Number.
//			So Next Interfaces are updated for Packet's and Field's ID Number.
//		Set&GetAllProperties Key
//			RL
//		Property
//			strRealList 
//		Method
//			AddRealItem
//			RemoveRealItem
//			GetRealSettingInfo
//	Not used Interface List
//		Method
//			SetMultiChartDlg
//		Message
//			WM_DRDS_DATA
//			WM_DRDS_DATA_PACKET_END
//		Class & Routine
//			CParentChartDlg* m_pMultiChartDlg;
//			COutsideCommonInterfaceFunction::Outside_SetRealInfo
//			COutsideCommonInterfaceFunction::Outside_UnSetRealInfo 



//////////////////////////////////////////////////////////////////////
// Support Base Desk Interface
//////////////////////////////////////////////////////////////////////

#define DR_MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;


LPCSTR CChartItem::CXIBaseDesk::GetBasePath()
{
	DR_MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)

#ifdef	_MULTICHART	
		return ((CMultiChartDlg*	)(pThis->GetParent()->GetParent()))->GetMainPath();
#endif

#ifdef _COMPARECHART
		return ((CCompareChartDlg*	)(pThis->GetParent()->GetParent()))->GetMainPath();
#endif

#ifdef _WEEKCHART
		return ((CWeekChartDlg*		)(pThis->GetParent()->GetParent()))->GetMainPath();
#endif

#ifdef _FSPEEDJUMUN_EAGLE
		return ((CHSJumunDlg_Eagle*	)(pThis->GetParent()->GetParent()))->GetMainPath();
#endif

#ifdef _FSPEEDJUMUN
		return ((CHSJumunDlg*		)(pThis->GetParent()->GetParent()))->GetMainPath();
#endif

	
	//return pThis->m_strMainDir;
}

HWND  CChartItem::CXIBaseDesk::GetDeskHwnd()
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	
	return NULL;
}

LPDISPATCH CChartItem::CXIBaseDesk::GetUnknown()
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return NULL;
}

LPUNKNOWN CChartItem::CXIBaseDesk::GetCtrlUnknown(LPCTSTR szName)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return NULL;
}

HWND CChartItem::CXIBaseDesk::GetCtrlHwnd(LPCTSTR szName)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return NULL;
}

LPVOID CChartItem::CXIBaseDesk::GetSharedInterface()
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return NULL;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/7/31
// Modifier		: 
// Comments		: Process Event of Chart OCX
//-----------------------------------------------------------------------------
BOOL CChartItem::CXIBaseDesk::InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam)
{
	DR_MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	return false;
}

BOOL CChartItem::CXIBaseDesk::InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return TRUE;
}
	
BOOL CChartItem::CXIBaseDesk::SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) 
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return FALSE; 
}

LPVOID CChartItem::CXIBaseDesk::GetLSharedData()
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return NULL;
}

void CChartItem::CXIBaseDesk::CallCommand(LPCSTR _szCmd)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return;
}

// * 사용자 컬러 정보를 리턴한다.
OLE_COLOR CChartItem::CXIBaseDesk::GetUserColor(short nIndex, LPCTSTR szName, short nFlag)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)

	return RGB(255,255,255);
}

// * 현재 아무것도 키값에 대한 정의가 되어있지 않으므로 널리턴.
// * 요구 또는 필요가 생길경우 여기에서 구현한다.
long CChartItem::CXIBaseDesk::GetDeskInfo(LPCSTR szKey)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return NULL;
}

long CChartItem::CXIBaseDesk::GetUserInfo(LPCSTR szKey)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return 0;
}

long CChartItem::CXIBaseDesk::GetFrameInfo(LPCSTR szKey)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return 0;
}

long CChartItem::CXIBaseDesk::GetMainInfo(LPCSTR szKey)
{
	DR_MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	if(strcmp(szKey, "USERCOLORTABLE")==0) 
	{
		return (long)0;
	}
	else if (strcmp(szKey, "FRAMEHANDLE")==0)
	{
		return (long)0;
	}
	else if(strcmp(szKey, "MAINHANDLE") == 0) 
	{
		CWnd* pMainFrm = (CWnd*)AfxGetMainWnd();
		if(pMainFrm)
		{
			return (long)pMainFrm;
		}
	}
	else if(strcmp(szKey, "FONTNAME")==0)
	{
		return (long)0;
	}		
	return 0;
}

//* 수정 시작 (2002.8.22 001) 모니터링 툴 추가
BOOL CChartItem::CXIBaseDesk::FP_vAdviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return FALSE;
}


void CChartItem::CXIBaseDesk::FP_vUnadviseDebug(LPCSTR sName, UINT pMonitoringCommand)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
}

LPCSTR CChartItem::CXIBaseDesk::FP_sGetMyVBName(HWND hWnd)
{
	return NULL;
}

void CChartItem::CXIBaseDesk::FP_vSendDebug(LPCSTR csSection, LPCSTR csKey, LPCSTR csValue)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
}

long CChartItem::CXIBaseDesk::FP_nGetCtrlPos()
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return 0;
}

// 컴포넌트를 리사이즈 시킨다.
void CChartItem::CXIBaseDesk::FP_vResizeCtrl()
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
}

void CChartItem::CXIBaseDesk::SetMapMessage(long lType, LPCSTR lpData)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
}

int CChartItem::CXIBaseDesk::FP_nGetDataInt(int nCommand)
{
	// METHOD_PROLOGUE(CChartItem, IBaseDesk)
	return 0;
}

// (2005/8/10 - Seung-Won, Bae) Shared Data Manager Interface
ISharedDataMgr *CChartItem::CXIBaseDesk::GetSharedDataManager( void)
{
	DR_MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	return pThis;
}

// (2005/8/12 - Seung-Won, Bae) Real Table Manager Interface
IRealTableMgr *CChartItem::CXIBaseDesk::GetRealTableManager( void)
{
	DR_MYMETHOD_PROLOGUE(CChartItem, IBaseDesk)
	return pThis;
}

//////////////////////////////////////////////////////////////////////
// Support Share Interface
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/10
// Modifier		: 
// Comments		: Advise Shared Data Routine for DtsChart OCX
//-----------------------------------------------------------------------------
BOOL CChartItem::AdviseSharedData( const char *p_szShareName, int p_nShareType, int p_nShareArea)
{
	if( !m_pDBMgr || !p_szShareName || !*p_szShareName) return FALSE;
	return m_pDBMgr->AdviseSharedInfo( p_szShareName, this, p_nShareType, p_nShareArea,0);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/10
// Modifier		: 
// Comments		: Unadvise Shared Data Routine for DtsChart OCX
//-----------------------------------------------------------------------------
BOOL CChartItem::UnadviseSharedData( const char *p_szShareName)
{
	if( !m_pDBMgr || !p_szShareName || !*p_szShareName) return FALSE;
	return m_pDBMgr->UnAdviseSharedInfo( p_szShareName, this);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/10
// Modifier		: 
// Comments		: Get Shared Data Routine for DtsChart OCX
//-----------------------------------------------------------------------------
BOOL CChartItem::GetSharedData( const char *p_szShareName, CString &p_strSharedData)
{
	// 0. Check Base Info

	// m_pParentWnd ==>
	if( !m_pDBMgr /*|| !m_pParentWnd*/ || !p_szShareName || !*p_szShareName) return FALSE;
	if( /*!m_pParentWnd->SendMessage( WMU_GET_SHARE_STATE,0,0) || */m_bSharedOn) return FALSE;

	// 1. Get Shared Data and Check Code Filter
	int nLen, nStock;
	char *pData;
	pData = ( char *)m_pDBMgr->GetSharedData( p_szShareName, nLen, nStock);
	long lCodeFilter = GetCodeFilterOfShareInfoItemHW( p_szShareName);
	if( !pData || ( lCodeFilter && !DK2( lCodeFilter, nStock))) return FALSE;

	// 2. Set Shared Data
	char *szSharedData = p_strSharedData.GetBuffer( nLen);
	strncpy( szSharedData, pData, nLen);
	szSharedData[ nLen] = '\0';
	p_strSharedData.ReleaseBuffer();

	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/10
// Modifier		: 
// Comments		: Notify Shared Data Routine to DtsChart OCX
//-----------------------------------------------------------------------------
long CChartItem::ChangeSharedData( CString sSharedName, BYTE* pData, int nLen, int nStock)
{
	// 0. Check Base Info
	if( !m_pDBMgr /*|| !m_pParentWnd*/) return FALSE;
	if( /*!m_pParentWnd->SendMessage( WMU_GET_SHARE_STATE,0,0) ||*/ m_bSharedOn) return 0;

	// 1. Check Code Filter
	long lCodeFilter = GetCodeFilterOfShareInfoItemHW( sSharedName);
	if( lCodeFilter && !DK2( lCodeFilter, nStock)) return 0;

	// 2. Set Shared Data to OCX
	CString strSharedData;
	char *szSharedData = strSharedData.GetBuffer( nLen);
	strncpy( szSharedData, ( char *)pData, nLen);
	szSharedData[ nLen] = '\0';
	strSharedData.ReleaseBuffer();
	m_bSharedOn = TRUE;
	SetSharedDataHW( sSharedName, strSharedData);
	m_bSharedOn = FALSE;

	return 1;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/10
// Modifier		: 
// Comments		: Fire Shared Data Routine from DtsChart OCX
//-----------------------------------------------------------------------------
BOOL CChartItem::FireSharedData( const char *p_szShareName, const char *p_szSharedData, long p_lFilterCode)
{
	// 0. Check Base Info
	if( !m_pDBMgr /*|| !m_pParentWnd */|| !p_szShareName || !*p_szShareName) return FALSE;
	if( /*!m_pParentWnd->SendMessage( WMU_GET_SHARE_STATE,0,0) ||*/ m_bSharedOn) return FALSE;

	// 1. Fire Shared Data
	return m_pDBMgr->SetSharedData( GetSafeHwnd(), p_szShareName, ( unsigned char *)p_szSharedData, strlen( p_szSharedData), p_lFilterCode);
}

//////////////////////////////////////////////////////////////////////
// Support Real Interface
//////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/12
// Modifier		: 
// Comments		: Init Real Table Tree Control for Real Setting Property Page
//-----------------------------------------------------------------------------
BOOL CChartItem::InitRealTableTreeControl( CTreeCtrl *p_pTreeCtrl)
{
	return FALSE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/13
// Modifier		: 
// Comments		: Retreive the Real Item ID from Real Item Name
//-----------------------------------------------------------------------------
BOOL CChartItem::GetRealIDs( const char *p_szRecordName, const char *p_szItemName, WORD &p_wRecordID, WORD &p_wItemID)
{
	// 0. Check NULL Pointers
	if( !m_pDBMgr || !p_szRecordName || !p_szItemName) return FALSE;

	// 1. Get Real Table Info
	CMapWordToPtr *pRealTable = m_pDBMgr->GetRealTable();
	if( !pRealTable) return FALSE;

	// 2. Search the Real Record by Name
	int					nItemCount;
	WORD				wRecordID;
	void *				pRecordTemp;
	POSITION			posRealRecord = pRealTable->GetStartPosition();
	CRealTblItem *		pItem;
	CRealTblRecord *	pRecord;
	while( posRealRecord)
	{
		pRealTable->GetNextAssoc( posRealRecord, wRecordID, pRecordTemp);
		pRecord = ( CRealTblRecord *)pRecordTemp;
		if( !pRecord) continue;
		if( pRecord->strName != p_szRecordName) continue;
		p_wRecordID = wRecordID;

		// 3. Search the Real Item by Name
		nItemCount = pRecord->Items.GetSize();
		for( int i = 0; i < nItemCount; i++)
		{
			pItem = pRecord->Items.GetAt( i);
			if( !pItem) continue;
			if( pItem->strName != p_szItemName) continue;
			p_wItemID = pItem->wID;

			return TRUE;
		}
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/13
// Modifier		: 
// Comments		: Retreive the Real Record ID from Real Record Name
//-----------------------------------------------------------------------------
BOOL CChartItem::GetRealNames( WORD p_wRecordID, WORD p_wItemID, const char *&p_szRecordName, const char *&p_szItemName)
{
	return FALSE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/16
// Modifier		: 
// Comments		: Advise Real Update
//-----------------------------------------------------------------------------
BOOL CChartItem::AddRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode)
{
	// 0. Check NULL Pointers
	if( !m_pDBMgr || !p_szRealKeyCode || !*p_szRealKeyCode) return FALSE;

	CString strRealKey;
	strRealKey.Format("%d,%s", p_wRealRecordID, p_szRealKeyCode);
	m_pDBMgr->AddRealUpdate( strRealKey, GetSafeHwnd());

	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/16
// Modifier		: 
// Comments		: Unadvise Real Update
//-----------------------------------------------------------------------------
BOOL CChartItem::ResetRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode)
{
	// 0. Check NULL Pointers
	if( !m_pDBMgr || !p_szRealKeyCode || !*p_szRealKeyCode) return FALSE;

	CString strRealKey;
	strRealKey.Format("%d,%s", p_wRealRecordID, p_szRealKeyCode);
	m_pDBMgr->ResetRealUpdate( strRealKey, GetSafeHwnd());

	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/16
// Modifier		: 
// Comments		: Retreive the Real Data Info
//-----------------------------------------------------------------------------
BOOL CChartItem::GetRealRecordInfo( long p_lRealDataAddress, WORD &p_wRealRecordID, CString &p_strRealKeyCode)
{
	if( !m_fnGetRealRecordInfo) return FALSE;
	return ( m_fnGetRealRecordInfo)( p_lRealDataAddress, p_wRealRecordID, p_strRealKeyCode);
}

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/8/16
// Modifier		: 
// Comments		: Retreive the Real Data
//-----------------------------------------------------------------------------
BOOL CChartItem::GetData( long p_lRealDataAddress, WORD p_wRealItemID, double &p_dData)
{
	if( !m_fnGetData) return FALSE;
	return ( m_fnGetData)( p_lRealDataAddress, p_wRealItemID, p_dData);
	
//	if(p_wRealItemID == 2)
//	{
//		CString sTemp;
//		sTemp.Format("%d %d", p_wRealItemID, p_dData);
//		OutputDebugString(sTemp);
//	}
//
//	return TRUE;
}
