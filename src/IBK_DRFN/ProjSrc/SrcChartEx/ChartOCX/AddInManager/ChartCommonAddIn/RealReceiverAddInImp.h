// RealReceiverAddInImp.h: interface for the CRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALRECEIVERADDINIMP_H__3A920719_067B_4CD3_969C_520CD6E51C53__INCLUDED_)
#define AFX_REALRECEIVERADDINIMP_H__3A920719_067B_4CD3_969C_520CD6E51C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2009/10/31 - Seung-Won, Bae) 허수봉 정리.
// 1. 기준 시간값의 유형	: 지수, Local Time, Main Time
// 2. 허봉 진행 방식
//		i.		허봉 추가시점과 무관하게 실봉 수신시, 추가된 허봉 Rollback 처리후 적용 기능 필요.
//		ii.		허봉 시간에 해당하
//		iii.	


#include <afxmt.h>										// for CCriticalSection

#include "../../Include_Chart/TimeTypeConvert.h"		// for CTimeTypeConvert
#include "../../Include_Chart/IRealReceiver.h"			// for IRealReceiver
#include "../../Include_Chart/IOldHTSRealReceiver.h"	// for IOldHTSRealReceiver
#include "../../Include_Chart/IOldFXRealReceiver.h"		// for IOldFXRealReceiver

#include "../Include_Addin/I000000/_ChartAddInBase.h"	// for CChartAddInBase

class CDumpLogger;
class CRealReceiverAddInImp : public CChartAddInBase, public IRealReceiver, public IOldFXRealReceiver, public IOldHTSRealReceiver
{
public:
	CRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CRealReceiverAddInImp();


// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
private:
	virtual IUnknown *	GetAddInCustomInterface( void);
// (2005.07.08, 배승원) AddIn Special Interface의 제공을 위하여 기본 Interface (IUnknown용)를 제공한다.
//		*. 위의 GetAddInCustomInterface()의 기능 제공을 위한 것으로, 
//			GetAddInCustomInterface() 삭제시 함께 삭제한다.
private:
	_IWRAPPER_IUNKNOWN_INTERFACE()


// (2006/7/26 - Seung-Won, Bae) Manage Removed Packet Data Count for Reset Scroll in OnDrdsPacketEnd().
private:
	int		m_nRemovePacketDataCount;


//////////////////////////////////////////////////////////////////////
// (2007/3/2 - Seung-Won, Bae) Manage Real Receiveing Check to skip the Empty PacketEnd Event.
//////////////////////////////////////////////////////////////////////
private:
	int		m_nRealReceivedType;		// 0:No Real, 1:Time Real, 2:Indexed Real


// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
private:								// Skip same '자료일자'
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_eScaleDrawType;
	double								m_dPrevServerTime;
private:				// Status Data
	long				m_lRealReceiveCount;

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long				m_lServerTime;
	__int64				m_lServerTime;
	// 2011.01.06 by SYS <<

	long				m_lDrawSpeed;
private:				// Analysis Real-Delay
	BOOL				m_bSecondTime;
	long				m_lServerBaseTime;
	long				m_lClientBaseTime1000;
	long				m_lRealLoadTime;
	long				m_lRealLoadTimeMax;
private:				// Real Process Speed
	long				m_lPrevClientTime1000;
	double				m_d5RealReceiveSpeed;
	double				m_dRealReceiveSpeedAverage;
	double				m_dRealReceiveSpeedMax;
	long				m_lLastRealCountOnDraw;
	long				m_l5RealProcessCountPerDraw;
	long				m_l5RealProcessCountPerDrawAverage;
	long				m_l5RealProcessCountPerDrawMax;
private:				// Multi-Thread
	static CWinThread *	m_pWinThread;
	static HANDLE		m_hRealReceiveEvent;
private:
	void				GetRealMonitorDataInPacketEnd( void);
	long				InvalidateControl( void);
	//BOOL				GetLoadTime( long &p_lLoadTime, long &p_lDelayTime);	// (2007/4/6 - Seung-Won, Bae) Use Min Load Time for Other Map Effect.
	static UINT			RealUpdateWithClutch( LPVOID pParam);
// (2007/3/13 - Seung-Won, Bae) Synchronized Draw
private:
	BOOL														m_bDoInvalidate;
	static CCriticalSection										m_csRealReceivers;
	static CTypedPtrArray < CPtrArray, CRealReceiverAddInImp *>	m_aRealReceivers;
private:
	static long			InvalidateControls( void);
	//static long			GetMaxLoadTime( void);

// (2012/04/05) for Clutch Draw Speed.
protected:
	static double		m_dDrawSkipFactor;
	static int			m_nCpuCount;
public:
	static double		GetDrawSkipFactor( void)						{	return m_dDrawSkipFactor * m_nCpuCount;					}
	static void			SetDrawSkipFactor( double p_dDrawSkipFactor)	{	m_dDrawSkipFactor = p_dDrawSkipFactor / m_nCpuCount;	}

// (2007/4/6 - Seung-Won, Bae) Real Time Speed Logging
private:
	CDumpLogger *	m_pDumpLogger;

// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
private:
	BOOL		m_bRealLoadIndicator;
	BOOL		m_bRealSpeedLog;
	BOOL		m_bRealDataLog;
	BOOL		m_bRealNoData;

// (2008/12/26 - Seung-Won, Bae) Make cache.
private:
	BOOL		m_bIsRuntimeMode;
	BOOL		m_bInUseRealData;

// 2008.08.25 김진순 허수봉 처리	<<
private:
	__int64					m_nRealClockSecs;
	CTimeTypeConvert 		m_tcReal;
	static int				m_nNomalizedMinuteTimerID;
private:
	static void	CALLBACK	OnNomalizedMinuteTimerProc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	void					SetUpdateTick( void);
public:
							// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
	virtual _int64			GetNMTRClock( void)		{	return m_nRealClockSecs;	}

protected:
	virtual BOOL	UpdatePacketData( const char *p_szPacketName, const int p_nIndex, const double p_dData);
	// 복수종목에 리얼처리 : 복수종목 - ojtaso (20070208)
	// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	virtual BOOL	SetRealDataRQ(LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData, BOOL p_bRealData);
	virtual BOOL	SetRealPacketEndRQ(LPCTSTR lpszRQ, BOOL p_bRealData);

//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
private:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);

private:
	CString m_szDebugRealData;
};

#endif // !defined(AFX_REALRECEIVERADDINIMP_H__3A920719_067B_4CD3_969C_520CD6E51C53__INCLUDED_)
