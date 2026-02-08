// DrdsAdvisedInfoList.h: interface for the CDrdsAdvisedInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRDSADVISEDINFOLIST_H__CBA26066_3E6F_4590_BE50_3FBBCE8F1BF6__INCLUDED_)
#define AFX_DRDSADVISEDINFOLIST_H__CBA26066_3E6F_4590_BE50_3FBBCE8F1BF6__INCLUDED_

// (2004.11.29, 배승원) DRDS Advised Info를 관리하는 Data Object List Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>								// for CTypedPtrList
#include "../Include_AddIn/I000000/_IPacket.h"		// for IPacket

// (2004.11.29, 배승원) DRDS Advised Info List Base Class를 정의한다.
class CDaiPacketGroup;
typedef CTypedPtrList< CObList, CDaiPacketGroup *> CDrdsAdvisedInfoListBase;

// (2004.11.29, 배승원) DRDS Advised Info를 관리하는 Data Object List Class를 정의한다.
class CDrdsAdvisedInfoList : public CDrdsAdvisedInfoListBase
{
public:
	CDrdsAdvisedInfoList();
	virtual ~CDrdsAdvisedInfoList();

// (2004.11.29, 배승원) DRDS Advised Info를 구성하는 Interface를 제공한다.
public:
	// 1. DRDS Advised Info Packet Group을 추가하는 Interface를 제공한다.
	BOOL	AddDaiPacketGroup( const char *p_szRealPacketName);
	// 2. DRDS Advised Info를 추가하는 Interface를 제공한다.
	//		(주의! 먼저 해당 Real Packet Name의 DRDS Advise Info Packet Group이 등록되어 있어야 한다.)
	BOOL	AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode);
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	BOOL	AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode);

// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 제거하는 Interface를 제공한다.
public:
	// 1. 일괄 제거하는 Interface를 제공한다.
	void	RemoveAllDrdsAdvisedInfo( void);

// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 검색하는 Interface를 제공한다.
public:
	// 1. DRDS Advised Info Packet Group을 검색하는 Inteface를 제공한다.
	POSITION	FindDaiPacketGroup( const char *p_szRealPacketName);

// (2004.12.01, 배승원) Real Packet의 수신여부를 관리하는 Interface를 제공한다.
//		(전 Packet의 수신시를 확인하여, Drawing의 별도 처리를 하기 위함이이다.)
public:
	// 1. Real Packet의 수신여부를 설정하는 Interface를 제공한다.
	//		(이때, 전 Packet의 수신여부를 확인하여 Return한다.)
	BOOL	SetPacketReceivedFlag( const char *p_szRealPacketName, const char *p_szRealKeyCode);
	// 2. 모든 Real Packet이 수신되었는지를 확인하는 Interface를 제공한다.
	BOOL	HasOneMoreReceiveForEachPacket( void);
	// 3. 모든 Real Packet의 수신 여부 Flag을 초기화 처리하는 Interface를 제공한다.
	void	ResetPacketReceiveFlag( void);
};

#endif // !defined(AFX_DRDSADVISEDINFOLIST_H__CBA26066_3E6F_4590_BE50_3FBBCE8F1BF6__INCLUDED_)
