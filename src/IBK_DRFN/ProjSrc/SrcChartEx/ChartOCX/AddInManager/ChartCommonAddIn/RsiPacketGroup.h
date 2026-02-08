// RsiPacketGroup.h: interface for the CRsiPacketGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSIPACKETGROUP_H__E74B9B17_E6E8_4D4D_8DE0_DFEBAE9FD8AC__INCLUDED_)
#define AFX_RSIPACKETGROUP_H__E74B9B17_E6E8_4D4D_8DE0_DFEBAE9FD8AC__INCLUDED_

// (2004.11.29, 배승원) Real Setting Info를 관리하는 Data Object List Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CTypedPtrList

// (2004.11.29, 배승원) Real Setting Info Object Class를 정의한다.
class CRealSettingInfo : public CObject
{
public:
	CRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
	{
		m_strChartPacketName	= p_szChartPacketName;
		m_strRealFieldName		= p_szRealFieldName;
		m_strShareName		= p_szShareName;
		m_strRQ.Empty();			// RQ별 Real관리 : 복수종목 - ojtaso (20070111) 
	}
	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	CRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName)
	{
		m_strChartPacketName	= p_szChartPacketName;
		m_strRealFieldName		= p_szRealFieldName;
		m_strShareName		= p_szShareName;
		m_strRQ = lpszRQ;
	}
public:
	CString		m_strChartPacketName;
	CString		m_strRealFieldName;
	CString		m_strShareName;	
	CString		m_strRQ;			// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
};

// (2004.11.29, 배승원) 같은 Real Packet Name 별로 관리하는 Real Info List Packet Group Base Class를 정의한다.
typedef CTypedPtrList< CObList, CRealSettingInfo *> CRsiPacketGroupBase;

// (2004.11.29, 배승원) Real Setting Info를 Real Packet Name별로 관리하는 Data Object Class를 정의한다.
class CRsiPacketGroup : public CObject
{
public:
	CRsiPacketGroup( const char *p_szRealPacketName);
	virtual ~CRsiPacketGroup();

// (2004.11.29, 배승원) 관리중인 Real Setting Info의 Packet 명을 관리한다.
public:
	CString	m_strRealPacketName;

// (2004.11.30, 배승원) CDrdsAdvisedInfoList에 CDaiPacketGroup Adding시 CDaiPacketGroup를 CDrdsAdvisedInfoList로 
//		오해하지 않도록 CDaiPacketGroup는 CDaiPacketGroupBase에서 계승받지 않고 Member로 관리한다.
protected:
	CRsiPacketGroupBase		m_lRsiPacketGroup;
public:
	void				RemoveAt( POSITION position)			{	m_lRsiPacketGroup.RemoveAt( position);			}
	BOOL				IsEmpty( void) const					{	return m_lRsiPacketGroup.IsEmpty();				}
	POSITION			AddTail( CRealSettingInfo *newElement)	{	return m_lRsiPacketGroup.AddTail( newElement);	}
	POSITION			GetHeadPosition( void) const			{	return m_lRsiPacketGroup.GetHeadPosition();		}
	CRealSettingInfo *	GetAt( POSITION position) const			{	return m_lRsiPacketGroup.GetAt( position);		}
	CRealSettingInfo *&	GetNext( POSITION& rPosition)			{	return m_lRsiPacketGroup.GetNext( rPosition);	}
	CRealSettingInfo *	RemoveHead( void)						{	return m_lRsiPacketGroup.RemoveHead();			}

// (2004.11.29, 배승원) Real Setting Info를 구성하는 Interface를 제공한다.
public:
	// 1. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
	BOOL	AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	BOOL	AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);

// (2004.11.29, 배승원) 구성된 Real Setting Info를 제거하는 Interface를 제공한다.
public:
	// 1. 일괄 제거하는 Interface를 제공한다.
	void	RemoveAllRealSettingInfo( void);
	// 2. Real Setting Info 값을 지정받아 Real Setting Info를 제거하는 Interface를 제공한다.
	BOOL	RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	BOOL	RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);

// (2004.11.29, 배승원) 구성된 Real Setting Info를 검색하는 Interface를 제공한다.
public:
	// 1. Real Setting Info 값을 지정받아 검색하는 Inteface를 제공한다.
	POSITION	FindRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	POSITION	FindRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealFieldName, const char *p_szShareName);
	// 2. Local Share Name만을 지정받아 해당 공유명으로 등록된 Real Setting Info가 있는지 검색한다.
	POSITION	FindShareName( const char *p_szShareName);
};

#endif // !defined(AFX_RSIPACKETGROUP_H__E74B9B17_E6E8_4D4D_8DE0_DFEBAE9FD8AC__INCLUDED_)
