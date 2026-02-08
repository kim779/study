// IPacketManager050531.h: interface for the IPacketManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKETMANAGER050531_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
#define AFX_IPACKETMANAGER050531_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 AddIn에서 이용되는 OCX측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) Packet Data List의 Object Interface를 정의한다.
//		AddIn DLL 참조 Interface로 Macro Interface를 정의한다.
#if IPacketManagerVer < 050531
	#define	IPacketManager		IPacketManager050531
	#undef	IPacketManagerVer
	#define	IPacketManagerVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IPacket050531.h"		// for IPacket

interface IPacketManager050531 : public IUnknown  
{
protected:
	virtual ~IPacketManager050531()	{}

// [04/10/11] Packet Object의 Interface를 구하는 Interface를 제공한다.
public:
	// [04/10/11] Packet Name으로 Packet Object를 구하는 Interface를 제공한다.
	virtual IPacket *	GetPacket( const char *p_szPacketName) = 0;
	// (2005/9/2 - Seung-Won, Bae) Get Packet Interface List for Packet Names
	virtual BOOL		GetPacketList( const char *p_szPacketNames, CTypedPtrList< CPtrList, IPacket *> *p_plIPackets) = 0;
	// [04/11/30] Packet Object의 Memory Address으로 Packet Object를 구하는 Interface를 제공한다.
	virtual IPacket *	GetPacketInterface( unsigned long p_ulPacketAddress) = 0;

// [04/10/13] 보조 Message를 검색하는 Interface를 제공한다. (최근에 수신된 TR에 제한된다.)
public:
	virtual BOOL						GetHelpMessageItem( const char *p_szMsgKey, CString &p_szMessage) = 0;
	// [05/09/29] Get Help Message List
	virtual const CMapStringToString *	GetHelpMessageMap( void) = 0;
};

#endif // !defined(AFX_IPACKETMANAGER050531_H__BAD6BB66_01E4_46F1_8938_3A15A0B0048A__INCLUDED_)
