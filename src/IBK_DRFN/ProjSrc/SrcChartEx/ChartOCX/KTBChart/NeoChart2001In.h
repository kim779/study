// NeoChart2001In.h: interface for the CNeoChart2001In class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEOCHART2001IN_H__06459547_2CF9_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_NEOCHART2001IN_H__06459547_2CF9_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CtrlPackBase.h"
class CBaseCtrlPackIn;
class CBaseCtrlPackOut;

/////////////////////////////////////////////////////////////////////////////
class CNeoChart2001In : public CBaseCtrlPackIn  
{
public:
	friend class CKTBChartCtrl;

	CNeoChart2001In(CKTBChartCtrl* pCtrl);
	CNeoChart2001In();
	
	virtual ~CNeoChart2001In();

	int GetData(char* buf, int bufLen);
	CKTBChartCtrl* m_pCtrl;



};

// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
//		이 값의 Maximum을 정의한다. (고정 Buffer를 선언하기 위함이다.)
#define _KTBCHART_PACKETCOUNTDATASIZE_MAX	255

/////////////////////////////////////////////////////////////////////////////
class CNeoChart2001Out : public CBaseCtrlPackOut    
{
public:
	friend class CKTBChartCtrl;
	CNeoChart2001Out(CKTBChartCtrl* pCtrl);

	CNeoChart2001Out();
	virtual ~CNeoChart2001Out();	

	int SetData(char* buf, int bufLen) ;
	int SetData2(char* buf, int bufLen);
	int	GetPackDataSize(char* pIn);

	CKTBChartCtrl* m_pCtrl;

// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
protected:
	int		m_nPacketCountDataSize;
};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_NEOCHART2001IN_H__06459547_2CF9_11D5_81A5_0050FC28B229__INCLUDED_)
