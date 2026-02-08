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
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode);
	virtual void			GetAllRQ(CStringArray& arRQ);
	virtual void			GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName);
	// 현재가 등락률 - ojtaso (20080602)
	virtual void			OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName = _T(""));
};

#endif // !defined(AFX_IWPACKETLISTMANAGER_H__CD122C5A_B302_4E58_9056_18A7EE51BE4B__INCLUDED_)
