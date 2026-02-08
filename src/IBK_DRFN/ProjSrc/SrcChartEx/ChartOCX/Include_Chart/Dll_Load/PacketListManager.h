// PacketListManager.h: interface for the CPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKETLISTMANAGER_H__DF702A19_963D_4547_83BC_0DAA34AE1CD9__INCLUDED_)
#define AFX_PACKETLISTMANAGER_H__DF702A19_963D_4547_83BC_0DAA34AE1CD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>			// for CList

#include "../BlockBaseEnum.h"

class CPacket;
class CPacketRQ;
class CGraphImp;			// 같은 RQ인지 : 복수종목 - ojtaso (20070308)
class CPacketList;
interface IChartCtrl;
interface IRealReceiver;

class CPacketListManager  
{
public:
	virtual ~CPacketListManager()	{}

public:
	virtual LPCTSTR CreateDefaultChartItem( HWND p_hOcxWnd)	= 0;

	// RQ를 만들기 위한 종목코드를 시장/종목/주기/틱수로 변경 : 복수종목 - ojtaso (20070106)
	// 틱 카운트 추가 : 복수종목 - ojtaso (20070329)
	// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	virtual LPCTSTR AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd)	= 0;		// 복수종목 그래프 추가
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	virtual LPCTSTR AddPairItem( LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd)	= 0;
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	virtual BOOL	AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName, HWND p_hOcxWnd)	= 0;
	virtual BOOL	DeleteChartItem(LPCTSTR lpszRQ)	= 0;
	
//	virtual LPCTSTR GetCurrentRQ()	= 0;
//	virtual void	SetCurrentRQ(LPCTSTR lpszRQ)	= 0;
	// RQ를 만들기 위한 종목코드를 시장/종목/주기/틱수로 변경 : 복수종목 - ojtaso (20070106)
	// 틱 카운트 추가 : 복수종목 - ojtaso (20070329)
	// 차트의 원, %, $의 구분관련 : 복수종목 - ojtaso (20070411)
	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	virtual void	SetItemCode(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName)	= 0;

	virtual CPacketList*	GetPacketList(LPCTSTR lpszRQ)	= 0;
//	virtual CPacketList*	GetCurrentPacketList()	= 0;
	virtual CPacketRQ*		GetPacketRQ(LPCTSTR lpszRQ)	= 0;
	virtual CPacket *		GetBasePacket( LPCTSTR lpszRQ)	= 0;

	virtual LPVOID			GetMapPacketList()	= 0;
	virtual void			SetIChartCtrl(IChartCtrl* p_pIChartCtrl)	= 0;
	virtual IChartCtrl *	GetIChartCtrl( void)	= 0;
	
	virtual LPCTSTR GetPacketListRQ(CPacketList* pPacketList)	= 0;

	virtual int		GetPacketListCount()	= 0;

	// 선택된 차트 정보는 외부dll에서 관리토록 수정 : 복수종목 - ojtaso (20070504)
	virtual void	GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName)	= 0;
	virtual void	GetRQByItemCode( LPCTSTR lpszItemCode, CList< CString, CString> &p_slRQ)	= 0;
	// 같은 RQ인지 : 복수종목 - ojtaso (20070308)
	virtual BOOL	IsSameRQ(LPCTSTR lpszRQ, CGraphImp* pGraph)	= 0;
	
	// DEFALUT를 제외한 모든 RQ : 복수종목 - ojtaso (20070427)
	virtual void	GetAllRQ( CList <CString, CString> &p_slRQs) = 0;

	// Graph가 삭제되도 PacketRQ를 삭제하지 않도록 하는 Flag : 복수종목 - ojtaso (20070531)
	virtual void	SetRemovablePacketRQ(BOOL bRemove)	= 0;
	virtual BOOL	GetRemovablePacketRQ()	= 0;

	// 선택된 RQ 변경 - ojtaso (20070620)
	virtual void	ChangePacketRQ(LPCTSTR lpszRQ1, LPCTSTR lpszRQ2)	= 0;

	// OneChart인지 : 복수종목 - ojtaso (20070627)
	virtual void	SetOneChart( BOOL bOneChart, const char *p_szRQ)  = 0;
	virtual BOOL	GetOneChart() = 0;

	// 현재가 등락률 - ojtaso (20080602)
	virtual void	OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName = _T("")) = 0;

// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
public:
	virtual BOOL	IsBasePacketString( void) = 0;

// 2008.10.31 JS.Kim	사용자선택 시간대 초기화
public:
	virtual void	ResetTimeDiff( void) = 0;

// (2009/10/19 - Seung-Won, Bae) Real Data Processing Interface.
public:
					// for CKTBChartCtrl::SetRealDataCode(). ~ Scripted Real Data.
	virtual void	SetRealData( LPCTSTR lpszItemCode, LPCTSTR lpszPacketName, double dData, short nOption)	= 0;
					// for IRealReceiver AddIn Interface.
	virtual BOOL	SetRealData( const char *p_szRQ, const char *p_szPacketName, const double& dData, int &p_nRemovePacketDataCount) = 0;


// (2008/11/4 - Seung-Won, Bae) for Nomalized Minute Time Clock.
public:
	// (2008/11/16 - Seung-Won, Bae) Notify to PacketList.
	virtual void	OnNMTRClock( const __int64 &p_nClock, IRealReceiver *p_pIRealReceiver) = 0;

// (2009/12/2 - Seung-Won, Bae) Notify to other modules.
public:
	virtual void	ChangeChartMode( int p_nChartMode) = 0;

	// (2010/02/25 - alzioyes) 통합버전 이전에 있던 기능임.
	virtual BOOL	FindRQ(LPCTSTR lpszRQ) = 0;

	// (2010/06/01 - alzioyes) 리얼패킷 제한사이즈 세팅.
	virtual void	SetHoldingPacketSize(int nHoldingPacketSize) = 0;
};

typedef CPacketListManager *(*FN_CreatePacketListManager)( CString strPacketOption, CPacketType::REALTYPE eRealType, int nLength, bool bUseRealData, const int nSize, const CString& szPacketName, double dVersion); 

#endif // !defined(AFX_PACKETLISTMANAGER_H__DF702A19_963D_4547_83BC_0DAA34AE1CD9__INCLUDED_)
