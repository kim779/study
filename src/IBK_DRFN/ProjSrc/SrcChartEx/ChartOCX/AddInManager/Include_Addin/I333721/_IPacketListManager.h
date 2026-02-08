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
	virtual IPacket *	GetPacket(LPCTSTR lpszRQ, const char *p_szPacketName) = 0;
	// RQ별로 RealData 처리 : 복수종목 - ojtaso (20070110)
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual BOOL		SetRealData(LPCTSTR lpszRQ, BOOL& bUpdate, IPacket *p_pIPacket, const char *p_szField, const char *p_szKey, double p_dData, int &p_nRemovePacketDataCount) = 0;
	virtual IStringList *	GetRQByItemCode( LPCTSTR lpszItemCode) = 0;

public:
	// 2008.08.07 김진순 종목간 Ratio chart		: 모든 RQ의 이름을 구해온다		<<
	virtual void		GetAllRQ(CStringArray& arRQ) = 0;
	virtual void		GetItemCode(LPCTSTR lpszRQ, LPCTSTR& lpszItemCode, LPCTSTR& lpszItemName) = 0;

	// 20081024 JS.Kim	허수봉을 ocx 및 AddIn 으로 주기위해 허수봉의 시간을 실봉시간으로 환산한다.
	virtual bool		CalcNMTimeToRealTime( LPCTSTR lpszRQ, IPacket *p_pIPacket, double p_dData, double& dRealTime, double& dCloseData) = 0;
	// 20081027 JS.Kim	RealData 받고 안받고의 처리를 종목별로 한다
	virtual bool		IsInUseRealData( LPCTSTR lpszRQ) = 0;
};

#endif // !defined(AFX_IPACKETLISTMANAGER_H__7F7BE182_1F90_44BA_AAAC_5CE7FAD1D48A__INCLUDED_)
