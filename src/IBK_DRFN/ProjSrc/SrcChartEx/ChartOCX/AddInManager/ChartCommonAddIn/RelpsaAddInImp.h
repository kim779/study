// RelpsaAddInImp.h: interface for the CRelpsaAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RELPSA_H__65FB3CF0_12B9_4FA9_8856_8816E58FE251__INCLUDED_)
#define AFX_RELPSA_H__65FB3CF0_12B9_4FA9_8856_8816E58FE251__INCLUDED_

// (2004.10.14, 배승원) RelpsaAddInImp (Reset Last PriceS After), 이 보조메시지로 지정된 시각이후의 Real이 수신시에는
//			전 가격(시/고/저/종)을 모두 종가로 통일시킨다. 임시 일봉 Data를 Clear하는 기능이다.
//		특히 일봉으로 제함함으로써 Real 수신이후에 종가로 시/고/저가를 통일시키는 방법으로 구현한다.
//		따라서, AP에서는 일봉이면서 Reset 시켜야만 하는 종목에서만 이 보조 메시지(HHMMSS)를 내려줘야 한다.
// (2004.10.22, 배승원) 이 기능들은 동양 지존차트의 종목/업종의 가격/비율형 봉차트를 지원하면서,
//			전일 종가 표시를 Reset 시키는 기능을 처리하는 것이다.
//			(특히 종목 비율형과 업종의 가격/비율형은 Real Packet에 알맞는 시고저 값이 내려오지 않아,
//				전일 종가의 시가 수용 문제를 해결하기 위한 기능이다.)
//		특히 이 기능들은 차트내 1개의 Graph에만 적용할 수 있는 기능으로
//			(기능의 Data 구조가 AddIn 당 1개로 국한된 기술적인 문제이다.)
//			'시가/고가/저가/종가'라는 이름의 Packet을 이용하는 Graph에만 효과를 발휘한다.
// (2004.11.30, 배승원) 이 기능이 수신하려고 하는 Real은 보조메시지로 지정된 종목코드로 요청한 "자료일자" Real이
//		수신되는 경우에서만 동작하도록 수정한다.
//		*. 주의! 실체결과 무관한 Real이 수신될 경우 오동작하게 된다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

class CRelpsaAddInImp : public CChartAddInBase
{
public:
	CRelpsaAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CRelpsaAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.10.14, 배승원) 초기화 기준 시각(장시작 시각)과 종목을 관리한다.
protected:
	double		m_dResetTime;
	CString		m_strResetItemCode;
public:
	void		ResetRELPSA( const char *p_szResetMsg);

// (2004.10.14, 배승원) Real을 수신하여 Reset할 시간인지를 확인/처리한다.
protected:
	// (2004.10.15, 배승원) OnDrdsPacketEnd()에서는 원하던 Real이었는지 식별할 수 없어 OnDrdsData에서 모든 상황 판단을 끝내 값을 관리한다.
	BOOL		m_bOnResetReal;

// (2004.10.15, 배승원) 가격 Data 초기화를 위한 Real 수신 환경을 Clear하는 Routine을 제공한다.
protected:
	void		ClearRELPSA( void);

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2007/2/8 - Seung-Won, Bae) On Real Data
	virtual void	OnRealData( const char *p_szPacketName, double p_dData, const char *p_szRQ);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
};

#endif // !defined(AFX_RELPSA_H__65FB3CF0_12B9_4FA9_8856_8816E58FE251__INCLUDED_)
