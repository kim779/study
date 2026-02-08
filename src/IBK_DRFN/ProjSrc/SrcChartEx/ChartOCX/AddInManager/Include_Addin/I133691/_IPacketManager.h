// IPacketManager.h: interface for the IPacketManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETMANAGER_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
#define AFX_IPACKETMANAGER_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>			// for CTypedPtrList
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

#include "BlockBaseEnum.h"		// for CScaleBaseData::HORZSCALEDRAWERTYPE

interface IPacket;
interface IPacketList;
interface IHelpMessageMap;
interface IPacketManager : public IUnknown  
{
public:
	typedef enum
	{
		INONAME = -1,
		ISTOCK = 0,	// 주식
		IINDUSTRY,	// 업종
		IFUTURES,	// 선물
		IOPTION		// 옵션
	} ICOMMODITY_BOND_NAME;

protected:
	virtual ~IPacketManager()	{}

// [04/10/11] Packet Object의 Interface를 구하는 Interface를 제공한다.
public:
	// [04/10/11] Packet Name으로 Packet Object를 구하는 Interface를 제공한다.
	virtual IPacket *	GetPacket( const char *p_szPacketName) = 0;
	// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
	virtual BOOL		GetPacketList( const char *p_szPacketNames, IPacketList *p_lIPacketList) = 0;

// [04/10/13] 보조 Message를 검색하는 Interface를 제공한다. (최근에 수신된 TR에 제한된다.)
public:
	virtual BOOL				GetHelpMessageItem( const char *p_szMsgKey, ILPCSTR &p_pszMessage) = 0;
	// [05/09/29] Get Help Message List
	virtual IHelpMessageMap *	GetHelpMessageMap( void) = 0;

// (2006/6/22 - Seung-Won, Bae) Retrieve Packet Infos
public:
	// (2006/6/22 - Seung-Won, Bae) Get RealUpdated Data Index
	virtual int			GetLastestRealIndex( void) = 0;
	// (2006/6/22 - Seung-Won, Bae) Get Maximum Data Count
	virtual long		GetMaximumDataCount( void) = 0;
	virtual long		GetMaximumDataCount( const char *p_szPacketNames, const char *p_szCompartString = ",") = 0;
	// (2006/6/26 - Seung-Won, Bae) Get the BaseTime Info
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE	GetBaseTimeDataType( void) = 0;
	virtual int			GetBaseTimeUnit( void) = 0;
	// (2006/7/26 - Seung-Won, Bae) Retrieve the flag of Real Usage
	virtual BOOL		IsInUseRealData( void) = 0;
	// (2006/8/15 - Seung-Won, Bae) Retrieve Main Item Type
	virtual ICOMMODITY_BOND_NAME	GetCommodityBondNameInBase( void) = 0;
	//>> Gap Revision - vntsorl(20090213)
	virtual ILPCSTR		GetTRPacketNames(const char *p_szCompart = ";") = 0;
	// (2008/11/3 - Seung-Won, Bae) Support User TimeScale Setting.
	virtual int			GetCalcDataTimeDifference( void) = 0;
	virtual int			GetBaseTimeDifference( void) = 0;
};

#endif // !defined(AFX_IPACKETMANAGER_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
