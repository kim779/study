// DaiPacketGroup.h: interface for the CDaiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAIPACKETGROUP_H__4B929BC4_05F2_441D_B492_605AB5FBB18F__INCLUDED_)
#define AFX_DAIPACKETGROUP_H__4B929BC4_05F2_441D_B492_605AB5FBB18F__INCLUDED_

// (2004.11.29, 배승원) DRDS Advised Info를 Packet 별로 관리하는 Data Object List Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>								// for CTypedPtrList
#include "../Include_AddIn/I000000/_IPacket.h"		// for IPacket
#include "DaiCodeGroup.h"							// for CDaiCodeGroup

// (2004.11.29, 배승원) 같은 Real Packet Name 별로 관리하는 DRDS Advised Info List Packet Group Base Class를 정의한다.
typedef CTypedPtrList< CObList, CDaiCodeGroup *> CDaiPacketGroupBase;

// (2004.11.29, 배승원) DRDS Advised Info를 Real Packet Name별로 관리하는 Data Object Class를 정의한다.
class CDaiPacketGroup : public CObject
{
public:
	CDaiPacketGroup( const char *p_szRealPacketName);
	virtual ~CDaiPacketGroup();

// (2004.11.29, 배승원) 관리중인 DRDS Advised Info의 Packet 명을 관리한다.
public:
	CString	m_strRealPacketName;

// (2004.11.30, 배승원) CDrdsAdvisedInfoList에 CDaiPacketGroup Adding시 CDaiPacketGroup를 CDrdsAdvisedInfoList로 
//		오해하지 않도록 CDaiPacketGroup는 CDaiPacketGroupBase에서 계승받지 않고 Member로 관리한다.
protected:
	CDaiPacketGroupBase		m_lDaiPacketGroup;
public:
	void				RemoveAt( POSITION position)			{	m_lDaiPacketGroup.RemoveAt( position);			}
	BOOL				IsEmpty( void) const					{	return m_lDaiPacketGroup.IsEmpty();				}
	POSITION			AddTail( CDaiCodeGroup *newElement)		{	return m_lDaiPacketGroup.AddTail( newElement);	}
	POSITION			GetHeadPosition( void) const			{	return m_lDaiPacketGroup.GetHeadPosition();		}
	POSITION			GetTailPosition( void) const			{	return m_lDaiPacketGroup.GetTailPosition();		}
	CDaiCodeGroup *		GetAt( POSITION position) const			{	return m_lDaiPacketGroup.GetAt( position);		}
	CDaiCodeGroup *&	GetNext( POSITION& rPosition)			{	return m_lDaiPacketGroup.GetNext( rPosition);	}
	CDaiCodeGroup *		RemoveHead( void)						{	return m_lDaiPacketGroup.RemoveHead();			}

// (2004.11.29, 배승원) DRDS Advised Info를 구성하는 Interface를 제공한다.
public:
	// 1. 1개의 DRDS Advised Info Data를 추가하는 Interface를 제공한다.
	BOOL	AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode);
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	BOOL	AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName, const char *p_szRealKeyCode);

// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 제거하는 Interface를 제공한다.
public:
	// 1. 일괄 제거하는 Interface를 제공한다.
	void	RemoveAllDrdsAdvisedInfo( void);

// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 검색하는 Interface를 제공한다.
public:
	// 1. Drds Advised Info 값을 지정받아 검색하는 Inteface를 제공한다.
	POSITION	FindDaiCodeGroup( const char *p_szRealKeyCode);

// (2004.12.01, 배승원) Real Packet의 수신여부를 관리하는 Interface를 제공한다.
//		(전 Packet의 수신시를 확인하여, Drawing의 별도 처리를 하기 위함이이다.)
public:
	// 1. Real Packet의 수신여부를 설정하는 Interface를 제공한다.
	//		(이때, 전 Packet의 수신여부를 확인하여 Return한다.)
	BOOL	SetPacketReceivedFlag( const char *p_szRealKeyCode);
	// 2. 모든 Real Packet이 수신되었는지를 확인하는 Interface를 제공한다.
	BOOL	HasOneMoreReceiveForEachPacket( void);
	// 3. 모든 Real Packet의 수신 여부 Flag을 초기화 처리하는 Interface를 제공한다.
	void	ResetPacketReceiveFlag( void);
};

#endif // !defined(AFX_DAIPACKETGROUP_H__4B929BC4_05F2_441D_B492_605AB5FBB18F__INCLUDED_)
