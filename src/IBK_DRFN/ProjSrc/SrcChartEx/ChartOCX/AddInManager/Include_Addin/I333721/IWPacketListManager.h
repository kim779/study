// IWPacketListManager.h: interface for the CIWPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
#define AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/DLL_Load/PacketListManager.h"		// for CPacketListManager
#include "../_IWUnknown.h"
#include "_IPacketListManager.h"
#include "_IChartOCX.h"												// for IChartOCX

class CIWPacketListManager : public CIWUnknown, public IPacketListManager
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IPacketListManager, CIWPacketListManager, CPacketListManager, m_pPacketListManager)

// (2008/10/14 - Seung-Won, Bae) Retrieve PacketManager
protected:
	virtual	IPacketManager *	GetPacketManager( const char *p_szRQ);

// (2007/2/13 - Seung-Won, Bae) Define Interfaces.
protected:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual IPacket *	GetPacket( LPCTSTR lpszRQ, const char *p_szPacketName);
	// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount);
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount);
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode);

// 2008.08.07 김진순 종목간 Ratio chart		: 모든 RQ의 이름을 구해온다		<<
protected:
	virtual void		GetAllRQ(CStringArray& arRQ);
	virtual void		GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName);

	// 20081024 JS.Kim	허수봉을 ocx 및 AddIn 으로 주기위해 허수봉의 시간을 실봉시간으로 환산한다.
	virtual bool		CalcNMTimeToRealTime( LPCTSTR lpszRQ, IPacket *p_pIPacket, double p_dData, double& dRealTime, double& dCloseData);
	// 20081027 JS.Kim	RealData 받고 안받고의 처리를 종목별로 한다
	virtual bool		IsInUseRealData( LPCTSTR lpszRQ);
};

#endif // !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
