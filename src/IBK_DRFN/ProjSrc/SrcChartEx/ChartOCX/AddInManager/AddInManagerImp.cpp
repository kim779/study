// AddInManagerImp.cpp: implementation of the CAddInManagerImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddInManagerImp.h"

#include "../Include_Chart/PropertyMap.h"		// for CPropertyMap
#include "./Include_AddIn/IAddInDll.h"			// for IAddInDll

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddInManagerImp::CAddInManagerImp( IChartCtrl *p_pIChartCtrl, const char *p_szOcxPath)	// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path
{
#ifdef _DEBUG
// (2006/8/7 - Seung-Won, Bae) AddIn Version Check Counter
	m_bVCheck000000 = FALSE;	// Chart Common AddIn
	m_bVCheck003531 = FALSE;	// CO - COCA		// Addin분리 - onlyojt (20100305)
	m_bVCheck003721 = FALSE;	// CO - COCA		// Addin분리 - onlyojt (20100305)
	m_bVCheck133721 = FALSE;	// KC - CGLA
	m_bVCheck423611 = FALSE;	// DB - CPAA

	m_bVCheck133101 = FALSE;	// KC - CATA
	m_bVCheck133691 = FALSE;	// KC - CPSA
	m_bVCheck134221 = FALSE;	// KC - DLVA
	m_bVCheck134661 = FALSE;	// KC - DPPA
	m_bVCheck135914 = FALSE;	// KC - ESUD
	m_bVCheck139092 = FALSE;	// KC - STIA2
	m_bVCheck339092 = FALSE;	// WW - STIA2
	m_bVCheck509092 = FALSE;	// EG - STIA2
#endif

	// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
	//		AddInManager는 Chart OCX에 의해서 관리된다.
	//		따라서, Release에 의해 자동 삭제되지 않도록 미리 AddRef()처리한다.
	AddRef();

	// (2004.10.08, 배승원) Chart OCX Interface를 제공하기 위하여, CEzpChartCtrl Pointer를 관리한다.
	m_pIChartCtrl = p_pIChartCtrl;

	// (2006/1/18 - Seung-Won, Bae) for Load AddIn DLL in OCX Path
	m_strOcxPath = p_szOcxPath;

	// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
	m_pIAddInDll_KeyboardExclusive		= NULL;
	m_pIAddInDll_LButtonExclusive		= NULL;
	m_pIAddInDll_RButtonExclusive		= NULL;
	m_pIAddInDll_MouseMoveExclusive		= NULL;
	m_pIAddInDll_MouseWheelExclusive	= NULL;

	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	m_rctCursorArea.SetRectEmpty();
}

CAddInManagerImp::~CAddInManagerImp()
{
	// (2004.10.11, 배승원) Addin DLL을 일괄 Unloading하는 Interface를 제공한다.
	//		AddIn Unload시에서 정상적인 OCX를 Access할 수 있도록 Window Destroy Time에서 일괄 Unload하도록 한다.
	UnloadAllAddin();	// 혹시 OnDestroy에서 Unload되지 못한 AddIn을 Unload시킨다.
}

//////////////////////////////////////////////////////////////////////
// (2006/2/12 - Seung-Won, Bae) Register/Unregister AddIn Event Map
//////////////////////////////////////////////////////////////////////
#define CASE_ADDINEVENT( OnAddInEvent)								\
	case IAddInDll::EAddInEventType::ADDINEVENT( OnAddInEvent):		\
		plIAddInDll = &LIST_ADDINEVENT( OnAddInEvent);				\
		break;
#define CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnAddInEvent)								\
	case IAddInDll::EAddInEventType::ADDINEVENT( OnAddInEvent):							\
		if( p_bOverExclusive)	plIAddInDll = &LIST_ADDINEVENT_OVER( OnAddInEvent);		\
		else					plIAddInDll = &LIST_ADDINEVENT_UNDER( OnAddInEvent);	\
		break;
CListIAddInDll *CAddInManagerImp::GetEventMapAddInList( const IAddInDll::EAddInEventType p_eAddInEventType, const BOOL p_bOverExclusive)
{
	CListIAddInDll *plIAddInDll = NULL;
	switch( p_eAddInEventType)
	{
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long)
		CASE_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long)
		CASE_ADDINEVENT( OnCaptureChanged)
		CASE_ADDINEVENT( OnCmdMsg)
		CASE_ADDINEVENT( OnDrawAfterGraph)
		CASE_ADDINEVENT( OnDrawBeforeGraph)
		CASE_ADDINEVENT( OnGSharedDataChange)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnKeyDown)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnKeyUp)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnLButtonDblClk)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnLButtonDown)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnLButtonUp)
		CASE_ADDINEVENT( OnLSharedDataChange)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnMouseMove)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnMouseWheel)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnRButtonDblClk)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnRButtonDown)
		CASE_ADDINEVENT_WITH_OVER_EXCLUSIVE( OnRButtonUp)
		CASE_ADDINEVENT( OnResetChart)
		CASE_ADDINEVENT( OnTimer)
		CASE_ADDINEVENT( OnResetMainBlock)
		CASE_ADDINEVENT( OnUserIndiatorConditionLoaded)
		CASE_ADDINEVENT( OnChartItemChanged)
		CASE_ADDINEVENT( OnGraphDataChanged)
		CASE_ADDINEVENT( OnBlockRemoved)
		CASE_ADDINEVENT( OnGraphAdded)
		CASE_ADDINEVENT( OnManualRealData)
		CASE_ADDINEVENT( OnContextMenu)
		CASE_ADDINEVENT( OnGraphDeleted)
		CASE_ADDINEVENT( OnPacketTruncated)
		CASE_ADDINEVENT( OnBlockChanged)
		CASE_ADDINEVENT( OnToolCommand)
		CASE_ADDINEVENT( OnRealData)
		CASE_ADDINEVENT( OnIndexedRealData)
		CASE_ADDINEVENT( OnRealPacketEnd)
		CASE_ADDINEVENT( OnAddInMsg)
		CASE_ADDINEVENT( OnLoadEnvironment)
		CASE_ADDINEVENT( OnSaveEnvironment)
		CASE_ADDINEVENT( DrawCursor)
		CASE_ADDINEVENT( OnRecalculationComplete)
		// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
		CASE_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject)
		// RQ별 Packet : 복수종목 - ojtaso (20070111)
		CASE_ADDINEVENT( OnPacketDataMultiItem)
		// (2008/10/22 - Seung-Won, Bae) for Real Filtering
		//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
		CASE_ADDINEVENT( OnPreRealData)
		CASE_ADDINEVENT( OnPreIndexedRealData)
		CASE_ADDINEVENT( OnPreRealPacketEnd)
		CASE_ADDINEVENT( OnPreNMTRClock)
	}
	return plIAddInDll;
}
BOOL CAddInManagerImp::RegisterEventMap( const long p_eAddInEventType, IAddInDll *p_pIAddInDll, const BOOL p_bOverExclusive, const BOOL p_bAddtional)
{
	if( !p_pIAddInDll) return FALSE;

	CListIAddInDll *plIAddInDll = GetEventMapAddInList( p_bAddtional ? ( IAddInDllBase::EAddInEventType)p_eAddInEventType : p_pIAddInDll->GetGlobalAddInEventType( p_eAddInEventType), p_bOverExclusive);
	if( !plIAddInDll) return FALSE;

	if( p_bAddtional) if( plIAddInDll->Find( p_pIAddInDll)) return FALSE;

	plIAddInDll->AddHead( p_pIAddInDll);

	return TRUE;
}
BOOL CAddInManagerImp::UnregisterEventMap( const long p_eAddInEventType, IAddInDll *p_pIAddInDll, const BOOL p_bOverExclusive, const BOOL p_bAddtional)
{
	if( !p_pIAddInDll) return FALSE;

	CListIAddInDll *plIAddInDll = GetEventMapAddInList( p_bAddtional ? ( IAddInDllBase::EAddInEventType)p_eAddInEventType : p_pIAddInDll->GetGlobalAddInEventType( p_eAddInEventType), p_bOverExclusive);
	if( !plIAddInDll) return FALSE;

	POSITION posAddInPrev = NULL;
	POSITION posAddIn = plIAddInDll->GetHeadPosition();
	while( posAddIn)
	{
		posAddInPrev = posAddIn;
		if( plIAddInDll->GetNext( posAddIn) == p_pIAddInDll)
		{
			plIAddInDll->RemoveAt( posAddInPrev);
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Keyboard & Mouse Exclusive Setting
//////////////////////////////////////////////////////////////////////
BOOL CAddInManagerImp::GetKeyboardExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_KeyboardExclusive && m_pIAddInDll_KeyboardExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_KeyboardExclusive = p_pIAddInDLL;
	return TRUE;
}
BOOL CAddInManagerImp::IsKeyboardExclusive( IAddInDll *p_pIAddInDLL)
{
	return m_pIAddInDll_KeyboardExclusive == p_pIAddInDLL;
}
BOOL CAddInManagerImp::ReleaseKeyboardExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_KeyboardExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_KeyboardExclusive = NULL;
	return TRUE;
}
BOOL CAddInManagerImp::GetAllMouseExclusive( IAddInDll *p_pIAddInDLL)
{
	BOOL bResultLButton	= GetLButtonExclusive( p_pIAddInDLL);
	BOOL bResultRButton	= GetRButtonExclusive( p_pIAddInDLL);
	BOOL bResultMove	= GetMouseMoveExclusive( p_pIAddInDLL);
	BOOL bResultWheel	= GetMouseWheelExclusive( p_pIAddInDLL);
	return bResultLButton || bResultRButton || bResultMove || bResultWheel;
}
BOOL CAddInManagerImp::IsAllMouseExclusive( IAddInDll *p_pIAddInDLL)
{
	BOOL bResultLButton	= IsLButtonExclusive( p_pIAddInDLL);
	BOOL bResultRButton	= IsRButtonExclusive( p_pIAddInDLL);
	BOOL bResultMove	= IsMouseMoveExclusive( p_pIAddInDLL);
	BOOL bResultWheel	= IsMouseWheelExclusive( p_pIAddInDLL);
	return bResultLButton || bResultRButton || bResultMove || bResultWheel;
}
BOOL CAddInManagerImp::ReleaseAllMouseExclusive( IAddInDll *p_pIAddInDLL)
{
	BOOL bResultLButton	= ReleaseLButtonExclusive( p_pIAddInDLL);
	BOOL bResultRButton	= ReleaseRButtonExclusive( p_pIAddInDLL);
	BOOL bResultMove	= ReleaseMouseMoveExclusive( p_pIAddInDLL);
	BOOL bResultWheel	= ReleaseMouseWheelExclusive( p_pIAddInDLL);
	return bResultLButton || bResultRButton || bResultMove || bResultWheel;
}
BOOL CAddInManagerImp::GetLButtonExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_LButtonExclusive && m_pIAddInDll_LButtonExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_LButtonExclusive = p_pIAddInDLL;
	return TRUE;
}
BOOL CAddInManagerImp::IsLButtonExclusive( IAddInDll *p_pIAddInDLL)
{
	return m_pIAddInDll_LButtonExclusive == p_pIAddInDLL;
}
BOOL CAddInManagerImp::ReleaseLButtonExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_LButtonExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_LButtonExclusive = NULL;
	return TRUE;
}
BOOL CAddInManagerImp::GetRButtonExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_RButtonExclusive && m_pIAddInDll_RButtonExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_RButtonExclusive = p_pIAddInDLL;
	return TRUE;
}
BOOL CAddInManagerImp::IsRButtonExclusive( IAddInDll *p_pIAddInDLL)
{
	return m_pIAddInDll_RButtonExclusive == p_pIAddInDLL;
}
BOOL CAddInManagerImp::ReleaseRButtonExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_RButtonExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_RButtonExclusive = NULL;
	return TRUE;
}
BOOL CAddInManagerImp::GetMouseMoveExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_MouseMoveExclusive && m_pIAddInDll_MouseMoveExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_MouseMoveExclusive = p_pIAddInDLL;
	return TRUE;
}
BOOL CAddInManagerImp::IsMouseMoveExclusive( IAddInDll *p_pIAddInDLL)
{
	return m_pIAddInDll_MouseMoveExclusive == p_pIAddInDLL;
}
BOOL CAddInManagerImp::ReleaseMouseMoveExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_MouseMoveExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_MouseMoveExclusive = NULL;
	return TRUE;
}
BOOL CAddInManagerImp::GetMouseWheelExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_MouseWheelExclusive && m_pIAddInDll_MouseWheelExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_MouseWheelExclusive = p_pIAddInDLL;
	return TRUE;
}
BOOL CAddInManagerImp::IsMouseWheelExclusive( IAddInDll *p_pIAddInDLL)
{
	return m_pIAddInDll_MouseWheelExclusive == p_pIAddInDLL;
}
BOOL CAddInManagerImp::ReleaseMouseWheelExclusive( IAddInDll *p_pIAddInDLL)
{
	if( m_pIAddInDll_MouseWheelExclusive != p_pIAddInDLL) return FALSE;
	m_pIAddInDll_MouseWheelExclusive = NULL;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL을 관리한다.
//////////////////////////////////////////////////////////////////////

IAddInDll *CAddInManagerImp::GetIAddInDllFromList( CListIAddInDll &p_lIAddInDll, const char *p_szAddInModule, const char *p_szAddInItem, BOOL p_bWithPop)
{
	if( !p_szAddInModule) return NULL;

	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	//		4.	Rule IV		: LoadAddIn of loaded AddIn make it to be Top. 
	const char *szAddInItemName = NULL;
	IAddInDll *pIAddInDll = NULL;
	POSITION posAddInPrev = NULL;
	POSITION posAddIn = p_lIAddInDll.GetHeadPosition();
	while( posAddIn)
	{
		posAddInPrev = posAddIn;
		pIAddInDll = p_lIAddInDll.GetNext( posAddIn);
		if( !stricmp( pIAddInDll->GetAddInDllName(), p_szAddInModule))
		{
			szAddInItemName = pIAddInDll->GetAddInItemName();

			// (2006/2/15 - Seung-Won, Bae) Check None
			if( !p_szAddInItem)
			{
				if( szAddInItemName) if( *szAddInItemName)			posAddInPrev = NULL;
			}
			else if( !*p_szAddInItem)
			{
				if( szAddInItemName) if( *szAddInItemName)			posAddInPrev = NULL;
			}
			else 
			{
				if( !szAddInItemName)								posAddInPrev = NULL;
				else if( !*szAddInItemName)							posAddInPrev = NULL;
				else if( stricmp( p_szAddInItem, szAddInItemName))	posAddInPrev = NULL;
			}

			if( posAddInPrev)
			{
				if( p_bWithPop) p_lIAddInDll.RemoveAt( posAddInPrev);
				else pIAddInDll->AddRef();
				return pIAddInDll;
			}
		}
	}
	return NULL;
}

// (2004.10.07, 배승원) Addin DLL을 Loading하는 Interface를 제공한다.
// (2004.10.13, 배승원) AddIn DLL Loading시 바로 AddIn Interface를 제공한다.
// (2005.01.18, 배승원) AddIn Object의 Interface를 제공하지 않고, DLL Wrapper를 전달하여,
//		동적인 연결이 가능하도록 처리한다.
typedef int	( *FN_GetAddInIVersion)( void);
BOOL CAddInManagerImp::LoadAddInItem( const char *p_szAddInModule, const char *p_szAddInItem, const BOOL p_bOverExclusive)
{
	// (2006/3/5 - Seung-Won, Bae) Seperate Module Name from Full Path.
	CString strAddInName( p_szAddInModule);
	int nIndex = strAddInName.ReverseFind( '\\');
	if( 0 <= nIndex) strAddInName = strAddInName.Mid( nIndex + 1);

	// (2006/2/8 - Seung-Won, Bae) Check Already Loaded.
	IAddInDll *pIAddInDll = GetIAddInDllFromList( m_lIAddInDll, strAddInName, p_szAddInItem, TRUE);
	if( pIAddInDll)
	{
		// (2006/4/10 - Seung-Won, Bae) Caution! Do not call AddRef() or Release(). It will be added in List.
		pIAddInDll->UnregisterEventMap( this);
		m_lIAddInDll.AddHead( pIAddInDll);
		pIAddInDll->RegisterEventMap( this, p_bOverExclusive);
		return TRUE;
	}

	// 1. Addin DLL이 있는지 확인한다.
	// (2006/3/5 - Seung-Won, Bae) Check OCX Path first.
	HMODULE hAddIn = LoadLibraryEx( m_strOcxPath + strAddInName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if( !hAddIn)
	{
		hAddIn = LoadLibraryEx( p_szAddInModule, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		if( !hAddIn)
		{
			TRACE( "Load AddIn Failed : %lu Error in '%s'.\r\n", GetLastError(),p_szAddInModule);
			return FALSE;
		}
	}

	// 2. Addin DLL의 Interface Version을 확인한다. (년월일)
	FN_GetAddInIVersion fnGetAddInIVersion = ( FN_GetAddInIVersion)GetProcAddress( hAddIn, "GetAddInIVersion");
	if( !fnGetAddInIVersion)
	{
		FreeLibrary( hAddIn);
		return FALSE;
	}
	int nAddInIVersion = ( *fnGetAddInIVersion)();

	// 3. Addin DLL의 Interface를 구한다.
	// 0. 먼저 Addin DLL의 Handle을 확인한다.
	pIAddInDll = NULL;
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll000000( strAddInName, nAddInIVersion, hAddIn);
	// Addin분리 - onlyojt (20100305)
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll003531( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll003721( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll423611( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll133721( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll333721( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll363721( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll503721( strAddInName, nAddInIVersion, hAddIn);
#ifdef _AIM_KOSCOM 
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll133101( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll134221( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll134661( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll135914( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll139092( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll133691( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll381931( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll339092( strAddInName, nAddInIVersion, hAddIn);
	if( !pIAddInDll) pIAddInDll = GetCIWAddInDll509092( strAddInName, nAddInIVersion, hAddIn);
#endif
	if( !pIAddInDll)
	{
		FreeLibrary( hAddIn);
		TRACE( CString( "Loading Failure in ") + strAddInName + " with Interface I%06d\r\n", nAddInIVersion);
		return FALSE;
	}

	// 4. Addin DLL Interface를 Map으로 관리한다.
	//	(주의, DLL 초기화에 앞서 임시라도 등록하여야 한다. 그래야 초기화 작업중에도 연동을 할 수 있다.)
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	//		4.	Rule IV		: LoadAddIn of loaded AddIn make it to be Top. 
	m_lIAddInDll.AddHead( pIAddInDll);

	// 5. 등록이 완료 되었으므로, Addin DLL을 초기화시킨다.
	// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOcx Interface Gateway)를 전달하여
	//		DLL의 AddIn Interface를 확보한다.
	if( !pIAddInDll->InitAddInDll( p_szAddInItem, this, pIAddInDll))
	{
		// 6. 초기화에 실패시에는 다시 DLL을 Unload시킨다.
		// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
		//		4.	Rule IV		: LoadAddIn of loaded AddIn make it to be Top. 
		m_lIAddInDll.RemoveHead();
		pIAddInDll->Release();
		TRACE( CString( "Loading Failure [") + p_szAddInItem + "] in " + strAddInName + "\r\n");
		return FALSE;
	}

	TRACE( strAddInName + ":" + p_szAddInItem + " is loaded.\r\n");

	// (2006/2/12 - Seung-Won, Bae) Register AddIn Event Map
	pIAddInDll->RegisterEventMap( this, p_bOverExclusive);

	// (2004.10.13, 배승원) AddIn DLL Loading시 바로 AddIn Interface를 제공한다.
	return TRUE;
}

// (2005/10/25 - Seung-Won, Bae) Unload AddIn DLL
BOOL CAddInManagerImp::UnloadAddInItem( const char *p_szAddInModule, const char *p_szAddInItem)
{
	IAddInDll *pIAddInDll = GetIAddInDllFromList( m_lIAddInDll, p_szAddInModule, p_szAddInItem, TRUE);
	if( !pIAddInDll) return FALSE;

	// (2006/2/12 - Seung-Won, Bae) Unregister AddIn Event Map
	pIAddInDll->UnregisterEventMap( this);

	// 2. Release DLL
	pIAddInDll->Release();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/2/14 - Seung-Won, Bae) AddIn Loading with Multiple.
//////////////////////////////////////////////////////////////////////

BOOL CAddInManagerImp::LoadMultipleAddInItem( const char *p_szAddInModule, const BOOL p_bOverExclusive)
{
	if( !p_szAddInModule) return FALSE;

	// 1. Make Buffer
	CString strAddInItems;
	char *szAddInItems = strAddInItems.GetBufferSetLength( strlen( p_szAddInModule));
	strcpy( szAddInItems, p_szAddInModule);

	// 2. Get the Head of Name
	char *szAddInDllName = szAddInItems;
	char *szAddInItemName = strchr( szAddInItems, ':');
	if( szAddInItemName)
	{
		*szAddInItemName = '\0';
		szAddInItemName++;
	}
	// 3. Process One AddIn.
	else return LoadAddInItem( szAddInDllName, NULL, p_bOverExclusive);

	BOOL bResultOr = FALSE;
	char *szNextAddIn = NULL;
	do
	{
		// 4. Find next Item Name and Seperate it.
		szNextAddIn = strchr( szAddInItemName, ',');
		if( szNextAddIn)
		{
			*szNextAddIn = '\0';
			szNextAddIn++;
		}

		BOOL bResult = LoadAddInItem( szAddInDllName, szAddInItemName, p_bOverExclusive);
		bResultOr |= bResult;
		
		szAddInItemName = szNextAddIn;
	}
	while( szAddInItemName);

	return bResultOr;
}

BOOL CAddInManagerImp::UnloadMultipleAddInItem( const char *p_szAddInModule)
{
	if( !p_szAddInModule) return FALSE;

	// 1. Make Buffer
	CString strAddInItems;
	char *szAddInItems = strAddInItems.GetBufferSetLength( strlen( p_szAddInModule));
	strcpy( szAddInItems, p_szAddInModule);

	// 2. Get the Head of Name
	char *szAddInDllName = szAddInItems;
	char *szAddInItemName = strchr( szAddInItems, ':');
	if( szAddInItemName)
	{
		*szAddInItemName = '\0';
		szAddInItemName++;
	}
	// 3. Process One AddIn.
	else return UnloadAddInItem( szAddInDllName, NULL);

	BOOL bResultOr = FALSE;
	char *szNextAddIn = NULL;
	do
	{
		// 4. Find next Item Name and Seperate it.
		szNextAddIn = strchr( szAddInItemName, ',');
		if( szNextAddIn)
		{
			*szNextAddIn = '\0';
			szNextAddIn++;
		}

		BOOL bResult = UnloadAddInItem( szAddInDllName, szAddInItemName);
		bResultOr |= bResult;
		
		szAddInItemName = szNextAddIn;
	}
	while( szAddInItemName);

	return bResultOr;
}

BOOL CAddInManagerImp::LoadMultipleAddInDll( const char *p_szAddInModule, const BOOL p_bOverExclusive)
{
	if( !p_szAddInModule) return FALSE;

	CString strAddInNames;
	char *szAddInNames = strAddInNames.GetBufferSetLength( strlen( p_szAddInModule));
	strcpy( szAddInNames, p_szAddInModule);

	BOOL bResultOr = FALSE;
	char *szNextAddIn = NULL;
	do
	{
		szNextAddIn = strchr( szAddInNames, ';');
		if( szNextAddIn)
		{
			*szNextAddIn = '\0';
			szNextAddIn++;
		}

		BOOL bResult = LoadMultipleAddInItem( szAddInNames, p_bOverExclusive);
		bResultOr |= bResult;
		
		szAddInNames = szNextAddIn;
	}
	while( szAddInNames);

	return bResultOr;
}

BOOL CAddInManagerImp::UnloadMultipleAddInDll( const char *p_szAddInModule)
{
	if( !p_szAddInModule) return FALSE;

	CString strAddInNames;
	char *szAddInNames = strAddInNames.GetBufferSetLength( strlen( p_szAddInModule));
	strcpy( szAddInNames, p_szAddInModule);

	BOOL bResultOr = FALSE;
	char *szNextAddIn = NULL;
	do
	{
		szNextAddIn = strchr( szAddInNames, ';');
		if( szNextAddIn)
		{
			*szNextAddIn = '\0';
			szNextAddIn++;
		}

		BOOL bResult = UnloadMultipleAddInItem( szAddInNames);
		bResultOr |= bResult;
		
		szAddInNames = szNextAddIn;
	}
	while( szAddInNames);

	return bResultOr;
}

// (2004.10.11, 배승원) Addin DLL을 일괄 Unloading하는 Interface를 제공한다.
//		AddIn Unload시에서 정상적인 OCX를 Access할 수 있도록 Window Destroy Time에서 일괄 Unload하도록 한다.
void CAddInManagerImp::UnloadAllAddin( void)
{
	// (2004.10.07, 배승원) Addin DLL Interface를 List로 관리한다.
	//		등록순에 따라 Event 전달 순이 되도록 List로 관리한다.
	IAddInDll *pIAddInDll = NULL;
	while( !m_lIAddInDll.IsEmpty())
	{
		pIAddInDll = m_lIAddInDll.RemoveHead();
		pIAddInDll->UnregisterEventMap( this);
		pIAddInDll->Release();
	}
}

// (2004.10.11, 배승원) AddIn Object의 Interface를 조회하여 OCX에서 직접 Access할 수 있도록 제공한다.
//		(Version 별로 제공하지 않고, OCX내에서 Casting하여 이용한다.)
// (2005.01.18, 배승원) AddIn Object의 Interface를 제공하지 않고, DLL Wrapper를 전달하여,
//		동적인 연결이 가능하도록 처리한다.
IAddInDll *CAddInManagerImp::GetIAddInDll( const char *p_szAddInModule, const char *p_szAddInItem)
{
	return GetIAddInDllFromList( m_lIAddInDll, p_szAddInModule, p_szAddInItem);
}

// (2006/1/7 - Seung-Won, Bae) Interface for Spacial Interface of AddIn DLL
IUnknown *CAddInManagerImp::GetAddInCustomInterface( const char *p_szAddInModule)
{
	if( !p_szAddInModule) return NULL;

	// 1. Make Buffer
	CString strAddInItems;
	char *szAddInItems = strAddInItems.GetBufferSetLength( strlen( p_szAddInModule));
	strcpy( szAddInItems, p_szAddInModule);

	// 2. Get the Head of Name
	char *szAddInDllName = szAddInItems;
	char *szAddInItemName = strchr( szAddInItems, ':');
	if( szAddInItemName)
	{
		*szAddInItemName = '\0';
		szAddInItemName++;
	}

	IAddInDll *pIAddInDll = GetIAddInDll( szAddInDllName, szAddInItemName);
	// (2005.07.08, 배승원) 특정 DLL 전용 Interface를 범용 Interface와 분리하여 처리.
	if( !pIAddInDll) return NULL;

	IUnknown *pIUnknown = pIAddInDll->GetAddInCustomInterface();
	pIAddInDll->Release();
	return pIUnknown;
}

// (2006/8/25 - Seung-Won, Bae) Check AddIn Loaded (DLL:IDS)
BOOL CAddInManagerImp::IsAddInLoaded( const char *p_szAddInItem)
{
	CString strAddInItem( p_szAddInItem);
	IAddInDll *pIAddInDll = NULL;
	int nIndex = strAddInItem.Find( ':');
	if( nIndex < 0) pIAddInDll = GetIAddInDll( strAddInItem, NULL);
	else pIAddInDll = GetIAddInDll( strAddInItem.Left( nIndex), strAddInItem.Mid( nIndex + 1));
	if( !pIAddInDll) return FALSE;
	pIAddInDll->Release();
	return TRUE;
}

// (2006/1/7 - Seung-Won, Bae) Set Command to the AddIn DLL
BOOL CAddInManagerImp::InvokeAddInItem( const char *p_szAddInDllName, const char *p_szAddInItem, short p_nCommandType, long p_lData)
{
	BOOL bResult = FALSE;
	IAddInDll *pIAddInDll = GetIAddInDll( p_szAddInDllName, p_szAddInItem);
	if( pIAddInDll)
	{
		bResult = pIAddInDll->InvokeAddIn( p_nCommandType, p_lData);
		pIAddInDll->Release();
	}
	return bResult;
}
BOOL CAddInManagerImp::InvokeAddInItemStr( const char *p_szAddInDllName, const char *p_szAddInItem, const char *p_szCommandName, LPCTSTR p_szData)
{
	BOOL bResult = FALSE;
	IAddInDll *pIAddInDll = GetIAddInDll( p_szAddInDllName, p_szAddInItem);
	if( pIAddInDll)
	{
		bResult = pIAddInDll->InvokeAddInStr( p_szCommandName, p_szData);
		pIAddInDll->Release();
	}
	return bResult;
}
BOOL CAddInManagerImp::InvokeAddIn( const char *p_szAddInDllName, short p_nCommandType, long p_lData)
{
	if( !p_szAddInDllName) return FALSE;

	const char *szAddInItem = strchr( p_szAddInDllName, ':');
	if( !szAddInItem) return InvokeAddInItem( p_szAddInDllName, NULL, p_nCommandType, p_lData);

	// 1. Make Buffer
	int nNameLen = szAddInItem - p_szAddInDllName;
	CString strAddInName;
	char *szAddInName = strAddInName.GetBufferSetLength( nNameLen);
	strncpy( szAddInName, p_szAddInDllName, nNameLen);
	szAddInName[ nNameLen] = '\0';

	// 2.Skip Colon
	szAddInItem++;

	return InvokeAddInItem( szAddInName, szAddInItem, p_nCommandType, p_lData);
}
BOOL CAddInManagerImp::InvokeAddInStr( const char *p_szAddInDllName, const char *p_szCommandName, LPCTSTR p_szData)
{
	if( !p_szAddInDllName) return FALSE;

	const char *szAddInItem = strchr( p_szAddInDllName, ':');
	if( !szAddInItem) return InvokeAddInItemStr( p_szAddInDllName, NULL, p_szCommandName, p_szData);

	// 1. Make Buffer
	int nNameLen = szAddInItem - p_szAddInDllName;
	CString strAddInName;
	char *szAddInName = strAddInName.GetBufferSetLength( nNameLen);
	strncpy( szAddInName, p_szAddInDllName, nNameLen);
	szAddInName[ nNameLen] = '\0';

	// 2.Skip Colon
	szAddInItem++;

	return InvokeAddInItemStr( szAddInName, szAddInItem, p_szCommandName, p_szData);
}

// (2007/3/4 - Seung-Won, Bae) Notify OCX Destroying to Addin
void CAddInManagerImp::OnOcxDestroyWindow( void)
{
	POSITION posAddIn = m_lIAddInDll.GetHeadPosition();
	while( posAddIn) m_lIAddInDll.GetNext( posAddIn)->OnOcxDestroyWindow();
}

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
void CAddInManagerImp::ChangeChartMode( CChartInfo::CHART_MODE p_eChartMode)
{
	POSITION posAddIn = m_lIAddInDll.GetHeadPosition();
	while( posAddIn) m_lIAddInDll.GetNext( posAddIn)->ChangeChartMode( p_eChartMode);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) Global 공유를 전달하는 Interface를 제공한다.
// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
void CAddInManagerImp::OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	POSITION posAddIn = LIST_ADDINEVENT( OnGSharedDataChange).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnGSharedDataChange).GetNext( posAddIn)->OnGSharedDataChange( p_szGShareKey, p_szGShareValue, p_bStartShare);
}
// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
void CAddInManagerImp::OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	POSITION posAddIn = LIST_ADDINEVENT( OnLSharedDataChange).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnLSharedDataChange).GetNext( posAddIn)->OnLSharedDataChange( p_szLShareKey, p_szLShareValue);
}

// (2004.10.11, 배승원) AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
void CAddInManagerImp::OnResetChart( void)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	POSITION posAddIn = LIST_ADDINEVENT( OnResetChart).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnResetChart).GetNext( posAddIn)->OnResetChart();
}

// (2004.10.11, 배승원) Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CAddInManagerImp::OnDrawBeforeGraph( HDC p_hDC)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	//		3.	Rule III	: and Drawing Order is reverse of AddIn Order (Tail First)
	POSITION posAddIn = LIST_ADDINEVENT( OnDrawBeforeGraph).GetTailPosition();
	while( posAddIn) LIST_ADDINEVENT( OnDrawBeforeGraph).GetPrev( posAddIn)->OnDrawBeforeGraph( p_hDC);
}

// (2004.12.25, 배승원) Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CAddInManagerImp::OnDrawAfterGraph( HDC p_hDC)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	//		3.	Rule III	: and Drawing Order is reverse of AddIn Order (Tail First)
	POSITION posAddIn = LIST_ADDINEVENT( OnDrawAfterGraph).GetTailPosition();
	while( posAddIn) LIST_ADDINEVENT( OnDrawAfterGraph).GetPrev( posAddIn)->OnDrawAfterGraph( p_hDC);
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CAddInManagerImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	POSITION posAddIn = LIST_ADDINEVENT( OnPacketDataMultiItem).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnPacketDataMultiItem).GetNext( posAddIn)->OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);
}


// (2004.10.29, 배승원) Timer Event를 처리하는 Interface를 제공한다.
void CAddInManagerImp::OnTimer( UINT p_uTimerID)
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	POSITION posAddIn = LIST_ADDINEVENT( OnTimer).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnTimer).GetNext( posAddIn)->OnTimer( p_uTimerID);
}

// (2004.10.29, 배승원) Mouse Capture Release Event를 AddIn DLL에게 전달하도록 한다.
//		주의! OnCaptureChanged()는 SetCapture()/RelaseCapture()에 의한 경우 외에도,
//			Focus 나 Activate, ToolTip, TrackMenu등에 의해서도 수시로 (SetCapture없이)
//			또한 자기 자신을 다시 SetCapture()하는 경우에도 호출됨을 유의한다.
void CAddInManagerImp::OnCaptureChanged( CWnd *pWnd, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// (2005/12/6 - Seung-Won, Bae) AddIn Tool's Z-Order
	POSITION posAddIn = LIST_ADDINEVENT( OnCaptureChanged).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnCaptureChanged).GetNext( posAddIn)->OnCaptureChanged( pWnd, p_hbmpMainOSB);
}

// (2006/9/25 - Seung-Won, Bae) On Reset MainBlock
void CAddInManagerImp::OnResetMainBlock( void)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnResetMainBlock).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnResetMainBlock).GetNext( posAddIn)->OnResetMainBlock();
}

// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
void CAddInManagerImp::OnUserIndiatorConditionLoaded( void)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnUserIndiatorConditionLoaded).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnUserIndiatorConditionLoaded).GetNext( posAddIn)->OnUserIndiatorConditionLoaded();
}

// (2006/9/25 - Seung-Won, Bae) On ChartItem Changed.
void CAddInManagerImp::OnChartItemChanged( void)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnChartItemChanged).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnChartItemChanged).GetNext( posAddIn)->OnChartItemChanged();
}

// (2006/9/25 - Seung-Won, Bae) On GraphData Changed.
void CAddInManagerImp::OnGraphDataChanged( void)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnGraphDataChanged).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnGraphDataChanged).GetNext( posAddIn)->OnGraphDataChanged();
}

// (2006/9/25 - Seung-Won, Bae) On Block Removed.
void CAddInManagerImp::OnBlockRemoved( const int p_nRow, const int p_nColumn)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnBlockRemoved).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnBlockRemoved).GetNext( posAddIn)->OnBlockRemoved( p_nRow, p_nColumn);
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CAddInManagerImp::OnGraphAdded( const char *p_szGraphName, const char *p_szIndicatorName)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnGraphAdded).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnGraphAdded).GetNext( posAddIn)->OnGraphAdded( p_szGraphName, p_szIndicatorName);
}

// (2006/9/25 - Seung-Won, Bae) On Graph Added.
void CAddInManagerImp::OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnManualRealData).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnManualRealData).GetNext( posAddIn)->OnManualRealData( p_szPacketName, p_dData, p_nOption);
}

// (2006/9/27 - Seung-Won, Bae) On Context Menu.
void CAddInManagerImp::OnContextMenu( HWND p_hWnd, POINT &p_pt)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnContextMenu).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnContextMenu).GetNext( posAddIn)->OnContextMenu( p_hWnd, p_pt);
}

// (2006/9/27 - Seung-Won, Bae) On Graph Deleted.
void CAddInManagerImp::OnGraphDeleted( const char *p_szGraphName, const char *p_szIndicatorName)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnGraphDeleted).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnGraphDeleted).GetNext( posAddIn)->OnGraphDeleted( p_szGraphName, p_szIndicatorName);
}

// (2006/9/27 - Seung-Won, Bae) On Packet Truncated.
void CAddInManagerImp::OnPacketTruncated( const int p_nDataCount)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnPacketTruncated).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnPacketTruncated).GetNext( posAddIn)->OnPacketTruncated( p_nDataCount);
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CAddInManagerImp::OnBlockChanged( const CBlockBaseData::BLOCKBEHAVIOR p_eChangeType, const int p_nSourceRow, const int p_nSourceColumn, const int p_nTargetRow, const int p_nTargetColumn)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnBlockChanged).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnBlockChanged).GetNext( posAddIn)->OnBlockChanged( p_eChangeType, p_nSourceRow, p_nSourceColumn, p_nTargetRow, p_nTargetColumn);
}

// (2006/9/27 - Seung-Won, Bae) On Block Changed.
void CAddInManagerImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnToolCommand).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnToolCommand).GetNext( posAddIn)->OnToolCommand( p_nToolType, p_nCmdOption);
}

// (2007/2/8 - Seung-Won, Bae) On Real Data
void CAddInManagerImp::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnRealData).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnRealData).GetNext( posAddIn)->OnRealData( p_szPacketName, p_dData, p_szRQ);
}
void CAddInManagerImp::OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnIndexedRealData).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnIndexedRealData).GetNext( posAddIn)->OnIndexedRealData( p_szPacketName, p_nIndex, p_dData);
}

// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CAddInManagerImp::OnRealPacketEnd( const char *p_szRQ)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnRealPacketEnd).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnRealPacketEnd).GetNext( posAddIn)->OnRealPacketEnd( p_szRQ);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void CAddInManagerImp::OnLoadEnvironment( CPropertyMap *p_pAddInEnvMap)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnLoadEnvironment).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnLoadEnvironment).GetNext( posAddIn)->OnLoadEnvironment( p_pAddInEnvMap);
}
void CAddInManagerImp::OnSaveEnvironment( CPropertyMap *p_pAddInEnvMap)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnSaveEnvironment).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnSaveEnvironment).GetNext( posAddIn)->OnSaveEnvironment( p_pAddInEnvMap);
}

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
void CAddInManagerImp::ClearCursor( CDC *p_pDC, const CBitmap *p_bmpMainOSB, CRect *p_pRect)
{
	if( m_rctCursorArea.IsRectEmpty() && !p_pRect) return;

	if( p_pDC && p_bmpMainOSB)
	{
		CDC dcMemory;
		dcMemory.CreateCompatibleDC( p_pDC); 

		// 2.2 Bitmap을 Memory DC에 등록한다.
		CBitmap *pbmpOld = dcMemory.SelectObject( ( CBitmap *)p_bmpMainOSB);

		// 2.3 Dispaly 영역으로 Memory DC로 Copy한다.
		if( p_pRect)	p_pDC->BitBlt( p_pRect->left, p_pRect->top, p_pRect->Width(), p_pRect->Height(),
							&dcMemory, p_pRect->left, p_pRect->top, SRCCOPY);
		else			p_pDC->BitBlt( m_rctCursorArea.left, m_rctCursorArea.top, m_rctCursorArea.Width(), m_rctCursorArea.Height(),
							&dcMemory, m_rctCursorArea.left, m_rctCursorArea.top, SRCCOPY);

		dcMemory.SelectObject( pbmpOld);

		// 4.6 DC들을 반납한다.
		dcMemory.DeleteDC();
	}

	m_rctCursorArea.SetRectEmpty();
}
void CAddInManagerImp::DrawCursor( CDC *p_pDC)
{
	POSITION posAddIn = LIST_ADDINEVENT( DrawCursor).GetHeadPosition();
	while( posAddIn)
	{
		CRect rctCursorArea = LIST_ADDINEVENT( DrawCursor).GetNext( posAddIn)->DrawCursor( p_pDC ? p_pDC->GetSafeHdc() : NULL);
		m_rctCursorArea.UnionRect( m_rctCursorArea, rctCursorArea);
	}
}

// (2007/1/18 - Seung-Won, Bae) Notify Recalculation.
void CAddInManagerImp::OnRecalculationComplete( void)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnRecalculationComplete).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT( OnRecalculationComplete).GetNext( posAddIn)->OnRecalculationComplete();
}

// (2008/10/22 - Seung-Won, Bae) for Real Filtering
//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
//	You can change the Real Data Value. it will be set to chart.
BOOL CAddInManagerImp::OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ, BOOL p_bRealData)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnPreRealData).GetHeadPosition();
	while( posAddIn) if( !LIST_ADDINEVENT( OnPreRealData).GetNext( posAddIn)->OnPreRealData( p_szPacketName, p_dData, p_szRQ, p_bRealData)) return FALSE;
	return TRUE;
}
BOOL CAddInManagerImp::OnPreIndexedRealData( const char *p_szPacketName, const int p_nIndex, double &p_dData, BOOL p_bRealData)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnPreIndexedRealData).GetHeadPosition();
	while( posAddIn) if( !LIST_ADDINEVENT( OnPreIndexedRealData).GetNext( posAddIn)->OnPreIndexedRealData( p_szPacketName, p_nIndex, p_dData, p_bRealData)) return FALSE;
	return TRUE;
}
BOOL CAddInManagerImp::OnPreRealPacketEnd( const char *p_szRQ, BOOL p_bRealData)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnPreRealPacketEnd).GetHeadPosition();
	while( posAddIn) if( !LIST_ADDINEVENT( OnPreRealPacketEnd).GetNext( posAddIn)->OnPreRealPacketEnd( p_szRQ, p_bRealData)) return FALSE;
	return TRUE;
}
BOOL CAddInManagerImp::OnPreNMTRClock( __int64 &p_nRealClockSecs)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnPreNMTRClock).GetHeadPosition();
	while( posAddIn) if( !LIST_ADDINEVENT( OnPreNMTRClock).GetNext( posAddIn)->OnPreNMTRClock( p_nRealClockSecs)) return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CAddInManagerImp::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnCmdMsg).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnCmdMsg).GetNext( posAddIn)->OnCmdMsg( nID, nCode, pExtra, pHandlerInfo)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInMsg( const char *p_szAddInItemName, const long p_lParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInMsg).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInMsg).GetNext( posAddIn)->OnAddInMsg( p_szAddInItemName, p_lParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_HDC_UINT_POINT_DWORD).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_szParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_char).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_char).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_szParam1, p_szParam2)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_char_long).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_szParam, p_lParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const IAddInDll *p_pParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_IAddInDllBase).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_pParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_lParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_lParam, p_szParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_lParam, p_szParam1, p_szParam2, p_szParam3)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_lParam1, p_lParam2)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2, const long p_lParam3, const long p_lParam4, const long p_lParam5, const long p_lParam6, const long p_lParam7, const long p_lParam8, const long p_lParam9)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_EAICommandType_long_long_long_long_long_long_long_long_long).GetNext( posAddIn)->OnAddInToolCommand( p_eCommandID, p_lParam1, p_lParam2, p_lParam3, p_lParam4, p_lParam5, p_lParam6, p_lParam7, p_lParam8, p_lParam9)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, CIndicatorInfo *p_pParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_IGraphInfo).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID, p_pParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID, p_szParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, CObject *p_pParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_CObject).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID, p_szParam1, p_szParam2, p_pParam)) return TRUE;
	return FALSE;
}

// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
BOOL CAddInManagerImp::OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3, CObject *p_pParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID, p_szParam1, p_szParam2, p_szParam3, p_pParam)) return TRUE;
	return FALSE;
}

BOOL CAddInManagerImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID, p_lParam)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID, const long p_lParam1, const long p_lParam2)
{
	POSITION posAddIn = LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long).GetHeadPosition();
	while( posAddIn) if( LIST_ADDINEVENT( OnAddInToolCommand_long_EAICommandType_long_long).GetNext( posAddIn)->OnAddInToolCommand( p_lResult, p_eCommandID, p_lParam1, p_lParam2)) return TRUE;
	return FALSE;
}
BOOL CAddInManagerImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnKeyDown).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnKeyDown).GetNext( posAddIn)->OnKeyDown( nChar, nRepCnt, nFlags);

	// (2006/1/18 - Seung-Won, Bae) Support Keyboard Exclusive
	if( m_pIAddInDll_KeyboardExclusive)	return m_pIAddInDll_KeyboardExclusive->OnKeyDown( nChar, nRepCnt, nFlags);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnKeyDown).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnKeyDown).GetNext( posAddIn)->OnKeyDown( nChar, nRepCnt, nFlags)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnKeyUp).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnKeyUp).GetNext( posAddIn)->OnKeyUp( nChar, nRepCnt, nFlags);

	// (2006/1/18 - Seung-Won, Bae) Support Keyboard Exclusive
	if( m_pIAddInDll_KeyboardExclusive)	return m_pIAddInDll_KeyboardExclusive->OnKeyUp( nChar, nRepCnt, nFlags);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnKeyUp).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnKeyUp).GetNext( posAddIn)->OnKeyUp( nChar, nRepCnt, nFlags)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnMouseWheel( UINT nFlags, short zDelta, POINT pt, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnMouseWheel).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnMouseWheel).GetNext( posAddIn)->OnMouseWheel( nFlags, zDelta, pt, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse Wheel Exclusive
	if( m_pIAddInDll_MouseWheelExclusive) return m_pIAddInDll_MouseWheelExclusive->OnMouseWheel( nFlags, zDelta, pt, p_dwMouseActionID);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnMouseWheel).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnMouseWheel).GetNext( posAddIn)->OnMouseWheel( nFlags, zDelta, pt, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnMouseMove).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnMouseMove).GetNext( posAddIn)->OnMouseMove( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB);

	// (2006/1/18 - Seung-Won, Bae) Support MouseMove or LButton Exclusive
	if( m_pIAddInDll_MouseMoveExclusive || ( m_pIAddInDll_LButtonExclusive && (MK_LBUTTON & nFlags)))
	{
		if( m_pIAddInDll_LButtonExclusive && (MK_LBUTTON & nFlags)) return m_pIAddInDll_LButtonExclusive->OnMouseMove( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB);
		else return m_pIAddInDll_MouseMoveExclusive->OnMouseMove( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB);
	}
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnMouseMove).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnMouseMove).GetNext( posAddIn)->OnMouseMove( p_hDC, nFlags, point, p_dwMouseActionID, p_hbmpMainOSB)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnLButtonDown).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnLButtonDown).GetNext( posAddIn)->OnLButtonDown( p_hDC, nFlags, point, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse LButton Exclusive
	if( m_pIAddInDll_LButtonExclusive) return m_pIAddInDll_LButtonExclusive->OnLButtonDown( p_hDC, nFlags, point, p_dwMouseActionID);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnLButtonDown).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnLButtonDown).GetNext( posAddIn)->OnLButtonDown( p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnLButtonUp).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnLButtonUp).GetNext( posAddIn)->OnLButtonUp( p_hDC, nFlags, point, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse LButton Exclusive
	if( m_pIAddInDll_LButtonExclusive) return m_pIAddInDll_LButtonExclusive->OnLButtonUp( p_hDC, nFlags, point, p_dwMouseActionID);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnLButtonUp).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnLButtonUp).GetNext( posAddIn)->OnLButtonUp( p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnLButtonDblClk).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnLButtonDblClk).GetNext( posAddIn)->OnLButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse LButton Exclusive
	if( m_pIAddInDll_LButtonExclusive) return m_pIAddInDll_LButtonExclusive->OnLButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnLButtonDblClk).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnLButtonDblClk).GetNext( posAddIn)->OnLButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnRButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnRButtonDown).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnRButtonDown).GetNext( posAddIn)->OnRButtonDown( p_hDC, nFlags, point, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse RButton Exclusive
	if( m_pIAddInDll_RButtonExclusive) return m_pIAddInDll_RButtonExclusive->OnRButtonDown( p_hDC, nFlags, point, p_dwMouseActionID);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnRButtonDown).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnRButtonDown).GetNext( posAddIn)->OnRButtonDown( p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnRButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnRButtonUp).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnRButtonUp).GetNext( posAddIn)->OnRButtonUp( p_hDC, nFlags, point, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse RButton Exclusive
	if( m_pIAddInDll_RButtonExclusive)
	{
		m_pIAddInDll_RButtonExclusive->OnRButtonUp( p_hDC, nFlags, point, p_dwMouseActionID);
		// (2006/2/13 - Seung-Won, Bae) OnRButtonUp, Do not return FALSE in Exclusive State to restrict Context Menu of OCX
		return TRUE;
	}
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnRButtonUp).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnRButtonUp).GetNext( posAddIn)->OnRButtonUp( p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
BOOL CAddInManagerImp::OnRButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	POSITION posAddIn = LIST_ADDINEVENT_OVER( OnRButtonDblClk).GetHeadPosition();
	while( posAddIn) LIST_ADDINEVENT_OVER( OnRButtonDblClk).GetNext( posAddIn)->OnRButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID);

	// (2006/1/18 - Seung-Won, Bae) Support Mouse RButton Exclusive
	if( m_pIAddInDll_RButtonExclusive) return m_pIAddInDll_RButtonExclusive->OnRButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID);
	else
	{
		posAddIn = LIST_ADDINEVENT_UNDER( OnRButtonDblClk).GetHeadPosition();
		while( posAddIn) if( LIST_ADDINEVENT_UNDER( OnRButtonDblClk).GetNext( posAddIn)->OnRButtonDblClk( p_hDC, nFlags, point, p_dwMouseActionID)) return TRUE;
	}
	return FALSE;
}
