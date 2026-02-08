// ReceivedTRData.h: interface for the CReceivedTRData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECEIVEDTRDATA_H__215A6665_2EE6_4A3D_8889_8273BE018216__INCLUDED_)
#define AFX_RECEIVEDTRDATA_H__215A6665_2EE6_4A3D_8889_8273BE018216__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
//#include "../chart_common/ReplayChartInfoDef.h"
#include "../COMMONTR/KB_d1016.h"



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 26
// Explain		: 
// Using way	: 
//-----------------------------------------------------------------------------
class CReceivedEachTR
{
public:
	CReceivedEachTR();
	virtual ~CReceivedEachTR();
	void ClearTRData();

// Attribute
public:
	CString	m_szStockCode;

	char	m_szKey[28];

	int		m_nNDayBefore;		// 몇일전 Data인지 정보
	CString m_szDateInfo;		// TR의 일자 정보 YYYYMMDDHHMM( 당일일때에는 복기 시작시간 or 장시작시간 )
	CString m_szMarketEndTime;	// 장 종료시간 정보 HHMMSS
	int		m_nTickIdx;			// TR의 Index. 복기할 때 진행된 Index
	int		m_nTotalTickCount;	// 전체 Tick Count 
	CString	m_strBojoMsg;		// 보조메세지.

	CArray<KB_d1016_OutRec2, KB_d1016_OutRec2>	m_arrChart;
//	KB_d1016_CHART *m_pTRData;		// TR Data
};



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 10 / 17
// Explain		: 이전에 받은 TR의 Data를 저장한다.
// Using way	: 
//-----------------------------------------------------------------------------
class CReceivedTRData  
{
public:
	CReceivedTRData();
	virtual ~CReceivedTRData();


// Attribute
public:
	CString m_szStockCode;		// 종목코드	
	BOOL m_bRemainData;			// Data를 유지할 것인지를 판단하는 flag ( TRUE : TR 유지, FALSE : TR 삭제 )
	CList < CReceivedEachTR *, CReceivedEachTR * > m_lstTRData;			// 종목 TR Data
};




#endif // !defined(AFX_RECEIVEDTRDATA_H__215A6665_2EE6_4A3D_8889_8273BE018216__INCLUDED_)
