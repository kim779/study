// IWPacketListManager.h: interface for the CIWPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
#define AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/PacketListManager.h"	// for CPacketListManager
#include "../_IWUnknown.h"
#include "_IPacketListManager.h"
#include "_IChartOCX.h"											// for IChartOCX

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
	virtual IPacket *		GetPacket( LPCTSTR lpszRQ, const char *p_szPacketName);
	// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
	virtual BOOL			SetRealData(LPCTSTR lpszRQ, const char *p_szPacketName, double p_dData, int &p_nRemovePacketDataCount);
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode);
	virtual IStringList *	GetAllRQ( void);
	virtual void			GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName);
	// 현재가 등락률 - ojtaso (20080602)
	virtual void			OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName = _T(""));
	// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
	virtual BOOL			IsBasePacketString( void);

// 20081007 JS.Kim	사용자시간대 선택
protected:
	virtual IPacket		*GetDatePacket(LPCTSTR lpszRQ);

// (2008/11/4 - Seung-Won, Bae) for Nomalized Minute Time Clock.
protected:
	virtual void		OnNMTRClock( const __int64 &p_nClock, IRealReceiver *p_pIRealReceiver);

// (2009/11/3 - Seung-Won, Bae) Make Market Date to Real Date of WorldOn Real Data.
protected:
	virtual void		GetRealData_RealDateTime( LPCTSTR lpszRQ, double &p_dData);
};

#endif // !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
