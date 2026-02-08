// DrdsRealReceiverAddInImp.h: interface for the CDrdsRealReceiverAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRDSREALRECEIVERADDINIMP_H__7BB74194_3BD9_473E_8610_381F7D82E35F__INCLUDED_)
#define AFX_DRDSREALRECEIVERADDINIMP_H__7BB74194_3BD9_473E_8610_381F7D82E35F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include/LoadDrds.h"						// for CDrdsLoadLib
#include "RealReceiverAddInImp.h"						// for CRealReceiverAddInImp

class CRealSettingInfoList;
class CDrdsAdvisedInfoList;
class CXScaleManager;				// XScaleManager 추가 : 복수종목 - ojtaso (20070118)
class CDrdsRealReceiverAddInImp : public CRealReceiverAddInImp
{
public:
	CDrdsRealReceiverAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CDrdsRealReceiverAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;


// (2006/7/25 - Seung-Won, Bae) Support Real Requesting
protected:
	// 1. XFM과 Script에서 지정되는 Real 설정 속성 String Data로 구성하는 Interface를 제공한다.
	virtual BOOL	ResetRealSettingInfo( const char *p_szRealSettingInfo);
	// 2. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
	//		특히, p_pstrShareName과 p_pstrSharedData가 주어지는 경우에는 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
	//			p_szRealSettingItem에는 Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	//		(ResetRealSettingInfo()와 AddRealItem()가 공유할 수 있도록 하여 CString의 Pointer로 처리한다.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	virtual BOOL	AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, CString *p_pstrSharedData = NULL, char p_cSeparator = '.');
	// 2. 별도의 Method(RemoveRealItem)를 통하여 지정되는 1개의 Real Setting Info를 삭제하는 Interface를 제공한다.
	//		특히, Local Share Name이 없이 바로 Real Key Code가 주어져,
	//			적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환해야 한다.
	//		참고로 더이상 해당 공유 Key로 설정된 Real이 없는 경우에만 공유명이 반환된다.
	//		(AddRealSettingInfo()와 Interface를 통일하도록 하여 CString의 Pointer로 처리한다.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	virtual BOOL	RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.');
protected:
	virtual	BOOL	SetRealAdvise( BOOL p_bAdvise);
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	virtual	BOOL	SetRealAdviseRQ(LPCTSTR lpszRQ, BOOL p_bAdvise);
	virtual BOOL	GetRealAdvisedInfo( CString &p_strAdvisedInfo);

// (2006/7/25 - Seung-Won, Bae) Support DRDS Interface
protected:
	virtual BOOL	SetDrdsData( UINT wParam, LPARAM lParam);
	virtual BOOL	SetDrdsPacketEnd( UINT wParam, LPARAM lParam);

protected:
	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	virtual BOOL	AddRealSettingInfoRQ(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator);
	virtual BOOL	RemoveRealSettingInfoRQ(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator);
	// RQ별 Packet 삭제 : 복수종목 - ojtaso (20070122)
	virtual void	RemoveRQPacket(LPCTSTR lpszRQ);

//////////////////////////////////////////////////////////////////////
// (2004.11.17, 배승원, 분석) Real 관련 Interface이다.
//		Real의 처리를 위해서는 기본적으로 3가지 자료구조가 이용된다.
//		이것은 3가지 개념의 상호 연동에 의한 것이기 때문이며,
//			경우에 따라 이 3가지 자료구조가 불일치하고, 
//			이를 적절하게 운용해 나가는 절차가 필요하기 때문이다.
//	1. 자료구조
//		1.1 DRDS Advised Info
//			- 현재 DRDS에 Advise된 설정이 무엇인지 관리하는 자료구조이다.
//			- On/OffDrdsAdvise()에서 Real Setting Info와 Local Share Info를 참조하여
//				실질적인 Advise가 처리되면서 구성된다.
//			- Data :
//					ID (CPacket Object Pointer)
//					Real Packet Name
//					Real Field Name
//					Real Key Code
//			- 위 4가지 Data가 모두 Key가 되며, 모두 같은 경우에만 중복 Advise를 하지 않는다.
//				(모든 Key가 다 같아도 Advise에서 FALSE를 Return하지 않는다.)
//		1.2 Real Setting Info
//			- Real을 요청하기 위한 자료구조이다.
//			- XFM의 설정에서와 같이 Real Key Code 대신 Local Share Name을 관리한다.
//			- Method를 통하여 직접 Real Setting Info를 수정하여도 바로 Real 요청에 반영하지 않고,
//				OnDrdsAdvise() 처리시에만 Real에 반영한다. (즉시 처리시 머리 아파진다.)
//			- Data :
//					TR Packet Name (CPacket Object Pointer)
//					Real Packet Name
//					Real Field Name
//					Local Share Name
//			- 위 4가지 Data가 모두 Key가 되며, 모두 같은 경우에만 중복 관리하지 않는다.
//		1.3 Local Share Info
//			- Real을 요청하기 위한, Key Code를 관리하는 자료구조이다.
//			- XFM의 설정에서와 같이 Local Share Name과 그 속성을 관리한다.
//			- Local 공유값이 변경되어도 Real을 직접 조정하지 않고,
//				OnDrdsAdvise() 처리시에만 Real에 반영한다. (즉시 처리시 머리 아파진다.)
//			-  Data :
//					Local Share Name
//					Local Shared Data
//					Start	Share Flag
//					Set		Share Flag
//					Get		Share Flag
//			- Local Share Name이 Key이며, 이를 기준으로 Real 요청시 참조된다.
//	2. 운용법
//		2.1 SetAllProperties2()에서 XFM의 Real 설정인 m_strRealList를 분석하여 Real Setting Info를 구성한다.
//			(이렇게 구성된 Real Setting Info는 Destructor에서나 제거된다.)
//		2.2 이어, InitLSharedData()를 통해 Real을 요청하기위해 필요한 Real Key Code의 Local Share Info를 구성한다.
//		2.3 Map Loading이 되고, 초기 Local 공유가 전달되어 설정된다. (Start Flag이 아니어도 Start 효과가 날듯)
//		2.4 TR이 수신되어 Real이 Off/On되면서 구성된 Real Setting Info와 Local Share Info를 바탕으로 Real을 요청하게 된다.
//		2.5 OnDrdsData()와 OnDrdsPacketEnd()를 통해서 Real Data를 처리하게 된다.
//				특히 OnDrdsPacketEnd()의 처리에서는 m_bRedrawAfterUpdatedAllPackets의 값에 따라 요청한 Real Packet이
//					모두 1회 이상 수신된 경우에만 Drawing과 기타 Real 처리 마무리 작업을 진행하게 된다.
//		2.6 이렇게 Real이 수신되는 와중에 공유가 변경되거나, Real 설정이 변경되면,
//				Local Share Info나 Real Setting Info만 변경된다. (즉시적으로 Real 요청에 반영되지는 않는다.)
//		2.7 Off/OnDrdsAdvise()를 통하여 조정된 공유나 Real 설정에 의한 Real을 다시 요청한다.
//				(이때 Real은 일괄 재요청되며, DRDS Advised Info도 함께 갱신된다.)
//		2.8 종료시 Window가 제거되면서(OnDestroyWindow) OffDrdsAdvise()가 호출되 Real 수신을 끝낸다.
//		2.* 주의! Real Setting Info와 Local Shared Data가 변경되어도 즉시 Real Advise에는 적용하지 않는다.
//				항상 OnDrdsAdvise()를 통하여만 적용되도록 한다.
//				특히 XFM에 설정되지 않고, 즉시적으로 Real의 설정을 지정하는 경우에도
//				OnDrdsAdvise()를 통해서만 적용되도록 한다. 안그럼 너무 머리 아프다.
//		2.* 주의! Method에 의한 Real Setting Info와 Local Share Info의 설정 및 제거 String은
//				Map의 설정 String과 Parsing Routine을 공유하도록 Format을 통일한다. (구분자를 ','에서 '.'으로 변경한다.)
//	3. 허수봉
//		3.1 허수봉을 위하여는 m_bNormalizedMinuteTimeRule Flag으로 허수봉을 지원하는 차트인지를 관리한다.
//		3.2 m_bNormalizedMinuteTimeRule의 값은 Data Folder의 Config.cfg에 [CHART]의 NMTR 값이 0 or 1로 전 OCX의 기본값이 되고,
//				또한 별도의 Property 설정으로 따로 지정이 가능하다.
//		3.3 현재 허수봉의 Real을 처리중인지는 m_bOnNMTR로 별도 관리한다. (일/주/월/틱은 제외된다.)
//		3.4 OnDrdsPacketEnd()에서 m_bRedrawAfterUpdatedAllPackets의 값에 따라 요청한 Real Packet이
//				모두 1회 이상 수신된 경우에만 Drawing하게 되는데, 이때 허수봉을 위한 시간 지수 Packet은
//				검토 대상에서 제외된다. (항상 수신되어 있는 형태로 관리하는 개념)
//		3.5 또한 m_nTimeDataCount라는 시간 Data 수를 별도로 관리하여,
//				OnDrdsPacketEnd()에서는 요청한 Real Packet이 모두 1회 이상 수신된 경우외에,
//				허수봉을 위한 시간 지수 Packet에 의해 시간 봉수가 증가하는 경우에도 바로 반영되도록 한다.
//				이 경우, 수신되지 않은 Packet도 반영되므로 모든 Packet의 Data수가 맞지 않는 경우가 발생한다.
//////////////////////////////////////////////////////////////////////
// DRDS DLL Data
protected:
	void *				m_DrdsVoid;					// DRDS Client ID
	CDrdsLoadLib		m_DrdsDLL;					// DRDS DLL Helper(Interface) Class Object이다.
// Real On/Off Routine
protected:
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	BOOL	OffDrdsAdvise(LPCTSTR lpszRQ = NULL);	
	BOOL	OnDrdsAdvise(LPCTSTR lpszRQ = NULL);

	// RQ얻기 : 복수종목 - ojtaso (20070111)
	LPCSTR GetDrdsAdvisedRQ(LPVOID p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode);

// Real Setting Info
protected:
	CRealSettingInfoList *	m_plRealSettingInfoList;	// Real Setting Info를 관리하는 Data Object이다.
// DRDS Advised Info
protected:	// DRDS Advised Info
	CDrdsAdvisedInfoList *	m_plDrdsAdvisedInfoList;	// DRDS Advised Info를 관리하는 Data Object이다.
// 허수봉 관련 상태 Data
protected:
//	BOOL	m_bNormalizedMinuteTimeRule;	// (2004.11.26, 배승원) Real의 분차트시 허수봉개념의 전체 시간을 지원할 것인지를 Flag으로 관리 지원한다.
										//		특히, 별도의 Global을 속성을 두어 HTS 기본 설정을 관리하고,
										//		별도의 Property를 제공하여 Chart별 속성을 변경토록 한다.
	BOOL	m_bOnNMTR;					// (2004.12.01, 배승원) OnRealAdvise()로 Real이 시작되면서 허수봉을 처리해하는 중인지를 Flag으로 관리한다.
										//		OnRealAdvse()/OnRealData()/OffRealAdvise()에서만 이용된다.
// 허수봉 관련 상태 Data
protected:
	int		m_nTimeDataCount;			// (2004.12.01, 배승원) 허수봉의 정확한 봉증가 시점을 확인하기 위하여,
										//		최종 자료일자의 Data 개수를 관리한다.

	// XScaleManager 추가 : 복수종목 - ojtaso (20070118)
	CXScaleManager*		m_pXScaleManager;

// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
protected:
	virtual _int64			GetNMTRClock( void)		{	return CRealReceiverAddInImp::GetNMTRClock();	}
};

#endif // !defined(AFX_DRDSREALRECEIVERADDINIMP_H__7BB74194_3BD9_473E_8610_381F7D82E35F__INCLUDED_)
