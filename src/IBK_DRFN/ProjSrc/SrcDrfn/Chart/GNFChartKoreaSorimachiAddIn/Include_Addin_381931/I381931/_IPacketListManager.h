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

interface IPacketListManager : public IUnknown
{
protected:
	virtual ~IPacketListManager()	{}

public:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual IPacket *	GetPacket(LPCTSTR lpszRQ, const char *p_szPacketName) = 0;
	// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode) = 0;
};

#endif // !defined(AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_)
