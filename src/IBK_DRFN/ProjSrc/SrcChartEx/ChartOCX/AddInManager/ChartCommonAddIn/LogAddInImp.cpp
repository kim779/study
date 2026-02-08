// LogAddInImp.cpp: implementation of the CLogAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "LogAddInImp.h"

#include "../../Include/LoadDrds.h"							// for DrdsData
#include "../../Include/DumpLogger.h"						// for CDumpLogger
#include "../../Include_Chart/PropertyMap.h"				// for CPropertyMap
#include "../Include_AddIn/AddInCommand.h"					// for EAI_LOG_REAL_SETTING_CHANGED
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IHelpMessageMap.h"		// for IHelpMessageMap
#include "../Include_AddIn/I000000/_IPacketList.h"			// for CIPacketList
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn/I000000/_IStringList.h"			// for IStringList
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogAddInImp::CLogAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2005/9/2 - Seung-Won, Bae) Manage Logger
	m_pDumpLogger = NULL;
}

CLogAddInImp::~CLogAddInImp()
{
	// (2006/1/23 - Seung-Won, Bae) Set Log Mode to OCX
	if( m_pIChartOCX) m_pIChartOCX->SetLoggingFlag( FALSE);

	// (2005/9/2 - Seung-Won, Bae) Manage Logger
	if( m_pDumpLogger)
	{
		delete m_pDumpLogger;
		m_pDumpLogger = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CLogAddInImp::m_strAddInItemName = "LOG";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CLogAddInImp)
	ONADDINEVENT( OnGSharedDataChange)
	ONADDINEVENT( OnLSharedDataChange)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnRealData)
	ONADDINEVENT( OnIndexedRealData)
	ONADDINEVENT( OnRealPacketEnd)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
	ONADDINEVENT( OnKeyDown)
	ONADDINEVENT( OnLButtonDblClk)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
IUnknown *CLogAddInImp::GetAddInCustomInterface( void)
{
// (2006/1/23 - Seung-Won, Bae) Do not use it. Use AddIn Command!
/*
	IUnknown *pIUnknown = ( IUnknown *)( IChartLogAddIn *)this;
	pIUnknown->AddRef();
	return pIUnknown;
*/
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
//////////////////////////////////////////////////////////////////////
// (2005/9/2 - Seung-Won, Bae) for String Data
BOOL CLogAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// (2005/9/2 - Seung-Won, Bae) Process LOGFILE
	if( !stricmp( p_szCommandName, "LOGFILE"))
	{
		if( m_pDumpLogger)
		{
			delete m_pDumpLogger;
			m_pDumpLogger = NULL;
		}
		m_pDumpLogger = new CDumpLogger( p_szData, TRUE);

		// (2006/1/23 - Seung-Won, Bae) Set Log Mode to OCX
		m_pIChartOCX->SetLoggingFlag( TRUE);
		return TRUE;
	}

	// (2006/4/18 - Seung-Won, Bae) Log AllProperty
	if( !stricmp( p_szCommandName, "LOGPROPERTY"))
	{
		LogAllProperties();
		return TRUE;
	}

	if( !m_pDumpLogger) return FALSE;
	CString strLogData;
	strLogData.Format( "InvokeAddInStr\t%s\t%s\r\n", p_szCommandName, p_szData);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////
// [04/10/11] 공유를 전달하는 Interface를 제공한다.
// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
void CLogAddInImp::OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare)
{
	if( !m_pDumpLogger) return;
	CString strLogData;
	strLogData.Format( "OnGSharedDataChange\t%s\t%s\t%d\r\n", p_szGShareKey, p_szGShareValue, ( int)p_bStartShare);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}
// (2005/11/21 - Seung-Won, Bae) Not used in Hanwha
void CLogAddInImp::OnLSharedDataChange( const char *p_szLShareKey, const char *p_szLShareValue)
{
	if( !m_pDumpLogger) return;
	CString strLogData;
	strLogData.Format( "OnLSharedDataChange\t%s\t%s\r\n", p_szLShareKey, p_szLShareValue);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}
// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
void CLogAddInImp::OnResetChart( void)
{
	if( !m_pDumpLogger) return;
	CString strLogData( "OnResetChart\r\n");
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}
// [04/10/13] TR의 수신을 AddIn DLL들에게 알려 대응토록 한다.
void CLogAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( !m_pDumpLogger) return;

	// (2005/9/29 - Seung-Won, Bae) Log Help Message
	IHelpMessageMap *pIHelpMessageMap = m_pIDefaultPacketManager->GetHelpMessageMap();
	if( !pIHelpMessageMap) return;
	// 1. Data Len
	const char *szHelpMsgTitle = "OnPacketDataMultiItem-HelpMessage\r\n";
	int nLogLength = strlen( szHelpMsgTitle);
	ILPCSTR szKey, szValue;
	POSITION posMsg = pIHelpMessageMap->GetStartPosition();
	while( posMsg)
	{
		pIHelpMessageMap->GetNextAssoc( posMsg, szKey, szValue);
		nLogLength += strlen( szKey)	+ 3;	// with '\t\t', '='
		nLogLength += strlen( szValue)	+ 2;	// with '\r\n'
	}
	// 2. Log Data
	CString strLogData;
	char *szLogData = strLogData.GetBufferSetLength( nLogLength);
	char *szLogPtr = szLogData;
	strcpy( szLogPtr, szHelpMsgTitle);
	szLogPtr += strlen( szHelpMsgTitle);
	posMsg = pIHelpMessageMap->GetStartPosition();
	while( posMsg)
	{
		pIHelpMessageMap->GetNextAssoc( posMsg, szKey, szValue);
		*szLogPtr++ = '\t';
		*szLogPtr++ = '\t';
		strcpy( szLogPtr, szKey);
		szLogPtr += strlen( szKey);
		*szLogPtr++ = '=';
		strcpy( szLogPtr, szValue);
		szLogPtr += strlen( szValue);
		*szLogPtr++ = '\r';
		*szLogPtr++ = '\n';
	}
	m_pDumpLogger->WriteToStringLog( NULL, szLogData, nLogLength);
	pIHelpMessageMap->Release();

	// 0. Get Packet Lists
	CIPacketList lIPackets;
	if( !m_pIDefaultPacketManager->GetPacketList( p_szPacketNames, &lIPackets)) return;
	int nPacketCount = lIPackets.GetCount();
	if( nPacketCount < 1) return;
	IDoubleList **plDataList	= new IDoubleList *[ nPacketCount];
	IStringList	**plStrDataList	= new IStringList *[ nPacketCount];
	POSITION *pPosData		= new POSITION[ nPacketCount];
	POSITION *pPosStrData	= new POSITION[ nPacketCount];

	CStringList slPacketLog;
	// 1. Make Head Log
	CString strPacketName( p_szPacketNames);
	strPacketName.Replace( ";", "\t");
	strLogData.Format( "OnPacketDataMultiItem-TrData\t%s%d", strPacketName, ( int)p_bWithReset);
	slPacketLog.AddTail( strLogData);

	// 2. Make Packet for Data
	// 2.1 Get Data List
	int i = 0;
	IPacket *pIPacket = NULL;
	POSITION posIPacket = lIPackets.GetHeadPosition();
	while( posIPacket)
	{
		pIPacket = lIPackets.GetNext( posIPacket);
		// (2005/9/2 - Seung-Won, Bae) Get One List Double or String (Double First)
		plDataList[ i] = pIPacket->GetDataList();
		if( !plDataList[ i]) plStrDataList[ i] = pIPacket->GetStrDataList();
		else plStrDataList[ i] = NULL;
		i++;
	}
	// 2.2 Get Head Position
	for( i = 0; i < nPacketCount; i++)
	{
		if( plDataList[ i])		pPosData[ i] = plDataList[ i]->GetHeadPosition();
		else pPosData[ i]	= NULL;
		if( plStrDataList[ i])	pPosStrData[ i] = plStrDataList[ i]->GetHeadPosition();
		else pPosStrData[ i] = NULL;
	}
	// 2.3 Get All Data String
	CString strData;
	BOOL bData = FALSE;
	double dData = 0.0;
	do
	{
		bData = FALSE;
		strLogData = "\t";
		// 2.3.1 Get One Data String for Same Index
		for( i = 0; i < nPacketCount; i++)
		{
			if( plDataList[ i] && pPosData[ i])
			{
				double dData;
				plDataList[ i]->GetNext( pPosData[ i], dData);
				strData.Format( "\t%f", dData);
				bData = TRUE;
			}
			else if( plStrDataList[ i] && pPosStrData[ i])
			{
				ILPCSTR szData = NULL;
				plStrDataList[ i]->GetNext( pPosStrData[ i], szData);
				strData.Format( "\t%s", ( const char *)szData);
				bData = TRUE;
			}
			else strData = "\t ";
			strLogData += strData;
		}

		// 2.3.2 Add Data String
		if( bData) slPacketLog.AddTail( strLogData);
	} while( bData);
	
	// (2006/7/12 - Seung-Won, Bae) Release IDoubleList
	for( i = 0; i < nPacketCount; i++) if( plDataList[ i]) plDataList[ i]->Release();
	delete [] plDataList;

	// (2006/7/12 - Seung-Won, Bae) 
	for( i = 0; i < nPacketCount; i++) if( plStrDataList[ i]) plStrDataList[ i]->Release();
	delete [] plStrDataList;

	delete [] pPosData;
	delete [] pPosStrData;
	// (2006/6/11 - Seung-Won, Bae) Release IPacket Wrapper.
	while( !lIPackets.IsEmpty()) lIPackets.RemoveHead()->Release();


	// 3. Make One String
	// 3.1 Get Full Buffer Length
	nLogLength = 0;
	POSITION posLog = slPacketLog.GetHeadPosition();
	while( posLog) nLogLength += slPacketLog.GetNext( posLog).GetLength() + 2;
	// 3.2 Get Full Buffer
	szLogData = strLogData.GetBufferSetLength( nLogLength);
	// 3.3 Make One String
	szLogPtr = szLogData;
	posLog = slPacketLog.GetHeadPosition();
	while( posLog)
	{
		CString &strLog = slPacketLog.GetNext( posLog);
		strcpy( szLogPtr, strLog);
		szLogPtr += strLog.GetLength();
		*szLogPtr++ = '\r';
		*szLogPtr++ = '\n';
	}

	m_pDumpLogger->WriteToStringLog( NULL, szLogData, nLogLength);
}

// (2007/2/8 - Seung-Won, Bae) On Real Data
void CLogAddInImp::OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ)
{
	if( !m_pDumpLogger) return;

	CString strData;
	strData.Format( "%f", p_dData);
	m_strRealLogData += CString( "\t") + p_szPacketName + "\t" + strData;
}
void CLogAddInImp::OnIndexedRealData( const char *p_szPacketName, const int p_nIndex, double p_dData)
{
	if( !m_pDumpLogger) return;

	CString strData;
	strData.Format( "[%d]\t%f", p_nIndex, p_dData);
	m_strRealLogData += CString( "\t") + p_szPacketName + "\t" + strData;
}
// (2006/10/4 - Seung-Won, Bae) On Real Packet End
void CLogAddInImp::OnRealPacketEnd( const char *p_szRQ)
{
	if( !m_pDumpLogger) return;

	CString strLogData;
	strLogData.Format( "OnRealPacketEnd\t:%s\r\n", m_strRealLogData);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
	m_strRealLogData.Empty();
}

//////////////////////////////////////////////////////////////////////
// (2005/11/23 - Seung-Won, Bae) Log for Real Data
//////////////////////////////////////////////////////////////////////
// (2005/8/16 - Seung-Won, Bae) Not used in Hanwha
// (2005/11/21 - Seung-Won, Bae) Add General Interface
void CLogAddInImp::OnDrdsAdvise( long p_lRequestID, const char *p_szPacket, const char *p_szField, const char *p_szAdviseKey)
{
	if( !m_pDumpLogger) return;
	CString strLogData;
	strLogData.Format( "OnDrdsAdvise\t%u\t%s.%s(%s)\r\n", p_lRequestID, p_szPacket, p_szField, p_szAdviseKey);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}
void CLogAddInImp::OffDrdsAdvise( long p_lRequestID, const char *p_szPacket, const char *p_szField, const char *p_szAdviseKey)
{
	if( !m_pDumpLogger) return;
	CString strLogData;
	strLogData.Format( "OffDrdsAdvise\t%u\t%s.%s(%s)\r\n", p_lRequestID, p_szPacket, p_szField, p_szAdviseKey);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}

//////////////////////////////////////////////////////////////////////
// (2005/9/29 - Seung-Won, Bae) Log the New Real Setting
//////////////////////////////////////////////////////////////////////
void CLogAddInImp::OnRealSettingChanged( const char *p_szRealSetting)
{
	if( !m_pDumpLogger) return;
	CString strLogData;
	strLogData.Format( "OnRealSettingChanged\t%s\r\n", p_szRealSetting);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}


//////////////////////////////////////////////////////////////////////
// (2005/11/7 - Seung-Won, Bae) Normalized Minute Time Rule Reset
//////////////////////////////////////////////////////////////////////
void CLogAddInImp::OnNormalizedMinuteTimeRuleReset( BOOL p_bNormalizedMinuteTimeRule)
{
	if( !m_pDumpLogger) return;
	CString strLogData;
	strLogData.Format( "OnNormalizedMinuteTimeRuleReset\t%s\r\n", ( p_bNormalizedMinuteTimeRule ? "TRUE" : "FALSE"));
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}

//////////////////////////////////////////////////////////////////////
// (2005/11/18 - Seung-Won, Bae) Log the Current AllProperty Info
//////////////////////////////////////////////////////////////////////
BOOL CLogAddInImp::LogAllProperties()
{
	if( !m_pDumpLogger) return FALSE;

	CPropertyMap mapProperty;
	if( !m_pIChartOCX->GetAllProperties3( &mapProperty)) return FALSE;

	// 0. Set Log Key
	CString strLogKey( "[Current Properties]\r\n");
	// 1. Check Log Length
	const char *szPropertyKey, *szPropertyData;
	int nLogLength = strLogKey.GetLength();
	IPropertyMap *pIPropertyMap = &mapProperty;
	POSITION posProperties = pIPropertyMap->GetStartPosition();
	while( posProperties) 
	{
		pIPropertyMap->GetNextAssoc( posProperties, szPropertyKey, szPropertyData);
		if( !szPropertyKey) continue;
		if( !*szPropertyKey) continue;
		nLogLength += strlen( szPropertyKey);
		if( szPropertyData) if( *szPropertyData) nLogLength += strlen( szPropertyData);
		nLogLength += 1 + 2;	// '=', "\r\n"
	}
	// 2. Get Full Buffer
	CString strLogData;
	char *szLogData = strLogData.GetBufferSetLength( nLogLength);
	// 3.3 Make One String
	char *szLogPtr = szLogData;
	strcpy( szLogPtr, strLogKey);
	szLogPtr += strLogKey.GetLength();
	posProperties = pIPropertyMap->GetStartPosition();
	while( posProperties)
	{
		pIPropertyMap->GetNextAssoc( posProperties, szPropertyKey, szPropertyData);
		if( !szPropertyKey) continue;
		if( !*szPropertyKey) continue;
		strcpy( szLogPtr, szPropertyKey);
		szLogPtr += strlen( szPropertyKey);
		*szLogPtr++ = '=';
		if( szPropertyData) if( *szPropertyData)
		{
			strcpy( szLogPtr, szPropertyData);
			szLogPtr += strlen( szPropertyData);
		}
		*szLogPtr++ = '\r';
		*szLogPtr++ = '\n';
	}

	m_pDumpLogger->WriteToStringLog( NULL, szLogData, nLogLength);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2005/11/7 - Seung-Won, Bae) Write Log Temporary Message.
//////////////////////////////////////////////////////////////////////
void CLogAddInImp::WriteLogMessage( const char *p_szMsg)
{
	if( !m_pDumpLogger || !p_szMsg) return;
	CString strLogData;
	strLogData.Format( "WriteLogMessage\t%s\r\n", p_szMsg);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CLogAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if( EAI_LOG_REAL_SETTING_CHANGED == p_eCommandID)
	{
		OnRealSettingChanged( p_szParam);
		return TRUE;
	}
	return FALSE;
}

BOOL CLogAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	if( EAI_LOG_SET_IMAGENARY_CANDLE == p_eCommandID)
	{
		WriteLogMessage( p_szParam);
		OnNormalizedMinuteTimeRuleReset( p_lParam);
		return TRUE;
	}
	return FALSE;
}
BOOL CLogAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam, const char *p_szParam1, const char *p_szParam2, const char *p_szParam3)
{
	switch( p_eCommandID)
	{
		case EAI_LOG_DRDS_ADVISE:	OnDrdsAdvise( p_lParam, p_szParam1, p_szParam2, p_szParam3);
									break;
		case EAI_LOG_DRDS_UNADVISE:	OffDrdsAdvise( p_lParam, p_szParam1, p_szParam2, p_szParam3);
									break;
		default:	return FALSE;
	}
	return TRUE;
}
// [04/10/14] Real의 수신을 전달한다.
//				속도 관계로 DRDS의 Data를 그대로 Pass한다.
//		Notify Routine of 1 Real Field
//			int			wParam	~ Size of DrdsData, Fixed Value
//			DrdsData *	lParam	~ Data Structure
//						Reial Data is in ( DrdsData->Data + 3)
BOOL CLogAddInImp::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( !m_pDumpLogger) return FALSE;
	CString strLogData;
	strLogData.Format( "OnKeyDown\t%ud\t%ud\t%ud\r\n", nChar, nRepCnt, nFlags);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
	// (2006/1/21 - Seung-Won, Bae) return FALSE to pass the Event to a next AddIn Routine.
	return FALSE;
}
BOOL CLogAddInImp::OnLButtonDblClk( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( !m_pDumpLogger) return FALSE;
	CString strLogData;
	strLogData.Format( "OnLButtonDblClk\t%ud\t%ld\t%ld\r\n", nFlags, point.x, point.y);
	m_pDumpLogger->WriteToStringLog( NULL, strLogData, strLogData.GetLength());
	return FALSE;
}
