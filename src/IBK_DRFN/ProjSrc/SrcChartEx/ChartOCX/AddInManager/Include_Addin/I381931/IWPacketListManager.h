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

// (2007/2/13 - Seung-Won, Bae) Define Interfaces.
protected:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual IPacket *	GetPacket( LPCTSTR lpszRQ, const char *p_szPacketName);
	// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount);
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount);
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode);
};

#endif // !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
