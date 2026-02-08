// IPacketListManager.h: interface for the IPacketListManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_)
#define AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"			// for AddIn Version Define

interface IPacket;
interface IStringList;
interface IPacketManager;

interface IPacketListManager : public IUnknown
{
protected:
	virtual ~IPacketListManager()	{}

// (2008/10/14 - Seung-Won, Bae) Retrieve PacketManager
public:
	virtual	IPacketManager *	GetPacketManager( const char *p_szRQ) = 0;

public:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual IPacket *		GetPacket(LPCTSTR lpszRQ, const char *p_szPacketName) = 0;
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode) = 0;
	virtual IStringList *	GetAllRQ( void) = 0;
	virtual void			GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName) = 0;
	// 현재가 등락률 - ojtaso (20080602)
	virtual void			OffsetPacketData(LPCTSTR lpszRQ, double dData, CString strPacketName = _T("")) = 0;
	// (2009/2/16 - Seung-Won, Bae) Support Non-Time X Scale Manager.
	virtual BOOL			IsBasePacketString( void) = 0;

// 20081007 JS.Kim	사용자시간대 선택
public:
	virtual IPacket*	GetDatePacket(LPCTSTR lpszRQ) = 0;
};

#endif // !defined(AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_)
