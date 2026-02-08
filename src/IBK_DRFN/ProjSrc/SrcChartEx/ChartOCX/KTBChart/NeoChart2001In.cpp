// NeoChart2001In.cpp: implementation of the CNeoChart2001In class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeoChart2001In.h"
#include "KTBChartCtl.h"

#include "../Include_Chart/Dll_Load/PacketList.h"		// for CPacketList
#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for ML_LANGUAGE_RES
#include "resource.h"									// for IDS_TOO_GREAT_DATA_COUNT

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeoChart2001In::CNeoChart2001In()
{
	m_pCtrl = NULL;
}

CNeoChart2001In::CNeoChart2001In(CKTBChartCtrl* pCtrl)
{
	m_pCtrl = pCtrl; 
}

CNeoChart2001In::~CNeoChart2001In()
{
}

int CNeoChart2001In::GetData(char* buf, int bufLen)
{
	if(!m_pCtrl)  return BCP_ERROR;

	//Input Data 부분 길이 추가..
	
	return BCP_OK;
}

//////////////////////////////////////
// Out
CNeoChart2001Out::CNeoChart2001Out()
{
	m_pCtrl = NULL;

	// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
	//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
	m_nPacketCountDataSize = 0;
}

CNeoChart2001Out::CNeoChart2001Out(CKTBChartCtrl* pCtrl)
{
	m_pCtrl = pCtrl; 

	// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
	//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
	m_nPacketCountDataSize = 0;
}


CNeoChart2001Out::~CNeoChart2001Out()
{

}

int CNeoChart2001Out::GetPackDataSize(char* pIn)
{
	if(!pIn)		return BCP_INPUT;
	if(!m_pCtrl)	return BCP_ERROR;

	int nRet = 0;
	int lBozo = 0, lData = 0, nCount = 0 ;

	// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
	//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
	if( !m_nPacketCountDataSize)
	{
		m_nPacketCountDataSize = m_pCtrl->GetPacketCountDataSize();
		if( _KTBCHART_PACKETCOUNTDATASIZE_MAX < m_nPacketCountDataSize)
		{
			m_nPacketCountDataSize = _KTBCHART_PACKETCOUNTDATASIZE_MAX;

			_MHWND( m_pCtrl->GetSafeHwnd());
			ML_SET_LANGUAGE_RES();
			CString strMsg;
			strMsg.LoadString( IDS_TOO_GREAT_DATA_COUNT);
			AfxMessageBox( strMsg);
		}
		m_pCtrl->m_pPacketList->SetPacketLengthCount( m_nPacketCountDataSize);
	}

	char szCount[ _KTBCHART_PACKETCOUNTDATASIZE_MAX + 1], szBozo[5] ;
	//	Count
	memcpy(szCount, (void*)pIn, m_nPacketCountDataSize) ;
	szCount[ m_nPacketCountDataSize] = NULL ;	
	nCount = atoi(szCount) ;
	//	보조메세지길이
	memcpy(szBozo, (void*)( pIn +m_nPacketCountDataSize), 4) ;
	szBozo[4] = NULL ;
	lBozo = atoi(szBozo);
	//	Data

	// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
	//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
	nRet = m_nPacketCountDataSize + 4 + lBozo + m_pCtrl->GetOnePacketSize() * nCount ;

	return nRet;
}

int CNeoChart2001Out::SetData(char *buf, int bufLen)
{
	if(!m_pCtrl) return BCP_ERROR;
	
	// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
	//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
	char szCount[ _KTBCHART_PACKETCOUNTDATASIZE_MAX + 1];
	strncpy( szCount, buf, m_nPacketCountDataSize);
	szCount[ m_nPacketCountDataSize] = '\0';	
	int nCount = atoi( szCount);

	m_pCtrl->SetChartData2((long)buf, nCount, 0) ;

	return BCP_OK;	
}

#include "DumpLogger.h"									// for CDumpLogger
int CNeoChart2001Out::SetData2(char *buf, int bufLen)
{
	if(!m_pCtrl) return BCP_ERROR;

	if( m_pCtrl->m_bEnableLogChartLoading) GetLocalTime( &m_pCtrl->m_stLogStart);

	// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
	//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
	char szCount[ _KTBCHART_PACKETCOUNTDATASIZE_MAX + 1];
	strncpy( szCount, buf, m_nPacketCountDataSize);
	szCount[ m_nPacketCountDataSize] = '\0';	
	int nCount = atoi( szCount);

	m_pCtrl->SetChartData2((long)buf, nCount, 0);

	if( m_pCtrl->m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\n",
			m_pCtrl->m_stLogStart.wHour, m_pCtrl->m_stLogStart.wMinute, m_pCtrl->m_stLogStart.wSecond, m_pCtrl->m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds);
		CDumpLogger::LogStringToFile( "CNeoChart2001Out::SetData2()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return BCP_OK;
}
