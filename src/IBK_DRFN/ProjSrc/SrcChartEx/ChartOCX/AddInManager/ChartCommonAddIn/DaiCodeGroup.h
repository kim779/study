// DaiCodeGroup.h: interface for the CDaiCodeGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAICODEGROUP_H__A3CAF812_F128_41AA_AD68_328137619069__INCLUDED_)
#define AFX_DAICODEGROUP_H__A3CAF812_F128_41AA_AD68_328137619069__INCLUDED_

// (2004.11.29, 배승원) Packet별 DRDS Advised Info를 Code별 관리하는 Data Object List Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>								// for CTypedPtrList
#include "../Include_AddIn/I000000/_IPacket.h"		// for IPacket

// (2004.11.29, 배승원) DRDS Advised Info Object Class를 정의한다.
class CDrdsAdvisedInfo : public CObject
{
public:
	CDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName)
	{
		// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
		m_lpszRQ				= NULL;
		m_pIPacket				= p_pIPacket;
		m_strRealFieldName		= p_szRealFieldName;
		if( m_pIPacket) m_pIPacket->AddRef();
	}
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	CDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName)
	{
		m_lpszRQ				= ::strdup(lpszRQ);
		m_pIPacket				= p_pIPacket;
		m_strRealFieldName		= p_szRealFieldName;
		if( m_pIPacket) m_pIPacket->AddRef();
	}
	~CDrdsAdvisedInfo()
	{
		if( m_pIPacket) m_pIPacket->Release();

		// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
		if(m_lpszRQ)
		{
			::free(m_lpszRQ);
			m_lpszRQ = NULL;
		}
	}
public:
	IPacket *	m_pIPacket;
	CString		m_strRealFieldName;
	LPSTR		m_lpszRQ;		// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
};

// (2004.11.29, 배승원) 같은 Real Packet Name 별로 관리하는 DRDS Advised Info List Packet Group중
//		같은 Real Key Code별로 DRDS Advised Info를 관리하는 Code Group Base Class를 정의한다.
typedef CTypedPtrList< CObList, CDrdsAdvisedInfo *> CDaiCodeGroupBase;

// (2004.11.29, 배승원) Packet별 DRDS Advised Info를 Real Key Code별로 관리하는 Data Object Class를 정의한다.
class CDaiCodeGroup : public CObject
{
public:
	CDaiCodeGroup( const char *p_szRealKeyCode);
	virtual ~CDaiCodeGroup();

// (2004.11.29, 배승원) 관리중인 DRDS Advised Info의 Key Code 값을 관리한다.
public:
	CString		m_strRealKeyCode;

// (2004.11.30, 배승원) CDaiPacketGroup에 CDaiCodeGroup Adding시 CDaiCodeGroup을 CDaiPacketGroup로 
//		오해하지 않도록 CDaiCodeGroup는 CDaiCodeGroupBase에서 계승받지 않고 Member로 관리한다.
protected:
	CDaiCodeGroupBase	m_lDaiCodeGroup;
public:
	void				RemoveAt( POSITION position)			{	m_lDaiCodeGroup.RemoveAt( position);			}
	BOOL				IsEmpty( void) const					{	return m_lDaiCodeGroup.IsEmpty();				}
	POSITION			AddTail( CDrdsAdvisedInfo *newElement)	{	return m_lDaiCodeGroup.AddTail( newElement);	}
	POSITION			GetHeadPosition( void) const			{	return m_lDaiCodeGroup.GetHeadPosition();		}
	CDrdsAdvisedInfo *	GetAt( POSITION position) const			{	return m_lDaiCodeGroup.GetAt( position);		}
	CDrdsAdvisedInfo *&	GetNext( POSITION& rPosition)			{	return m_lDaiCodeGroup.GetNext( rPosition);		}
	CDrdsAdvisedInfo *	RemoveHead( void)						{	return m_lDaiCodeGroup.RemoveHead();			}

// (2004.11.29, 배승원) DRDS Advised Info를 구성하는 Interface를 제공한다.
public:
	// 1. 1개의 DRDS Advised Info Data를 추가하는 Interface를 제공한다.
	BOOL	AddDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName);
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	BOOL	AddDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName);

// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 제거하는 Interface를 제공한다.
public:
	// 1. 일괄 제거하는 Interface를 제공한다.
	void	RemoveAllDrdsAdvisedInfo( void);

// (2004.11.29, 배승원) 구성된 DRDS Advised Info를 검색하는 Interface를 제공한다.
public:
	// 1. Drds Advised Info 값을 지정받아 검색하는 Inteface를 제공한다.
	POSITION	FindDrdsAdvisedInfo( IPacket *p_pIPacket, const char *p_szRealFieldName);
	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	POSITION	FindDrdsAdvisedInfo(LPCTSTR lpszRQ, IPacket *p_pIPacket, const char *p_szRealFieldName);

// (2004.12.01, 배승원) Real Packet의 수신여부를 관리하는 Interface를 제공한다.
//		(전 Packet의 수신시를 확인하여, Drawing의 별도 처리를 하기 위함이이다.)
public:
	BOOL		m_bPacketReceived;
};

#endif // !defined(AFX_DAICODEGROUP_H__A3CAF812_F128_41AA_AD68_328137619069__INCLUDED_)
