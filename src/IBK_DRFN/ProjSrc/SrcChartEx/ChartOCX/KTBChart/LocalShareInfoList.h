// LocalShareInfoList.h: interface for the CLocalShareInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOCALSHAREINFOLIST_H__4ADF05F0_B0F7_4610_B8DB_63CB55B9A91C__INCLUDED_)
#define AFX_LOCALSHAREINFOLIST_H__4ADF05F0_B0F7_4610_B8DB_63CB55B9A91C__INCLUDED_

// (2004.11.29, 배승원) Local Share Info를 관리하는 Data Object List Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CTypedPtrList

// (2004.11.29, 배승원) Local Share Info Object Class를 정의한다.
class CLocalShareInfo : public CObject
{
public:
	CLocalShareInfo( const char *p_szLocalShareName, int p_nLocalShareSettingCode, const char *p_szLocalSharedData = NULL)
	{
		m_strLocalShareName		= p_szLocalShareName;
		m_bStart				= BOOL( p_nLocalShareSettingCode & 1);
		m_bSet					= BOOL( p_nLocalShareSettingCode & 2);
		m_bGet					= BOOL( p_nLocalShareSettingCode & 4);
		m_strLocalSharedData	= p_szLocalSharedData;
	}
public:
	CString		m_strLocalShareName;
	BOOL		m_bStart;
	BOOL		m_bSet;
	BOOL		m_bGet;
	CString		m_strLocalSharedData;
};

// (2004.11.29, 배승원) Local Share Info List Base Class를 정의한다.
typedef CTypedPtrList< CObList, CLocalShareInfo *> CLocalShareInfoListBase;

// (2004.11.29, 배승원) Local Share Info를 관리하는 Data Object List Class를 정의한다.
class CLocalShareInfoList : public CLocalShareInfoListBase
{
public:
	CLocalShareInfoList();
	virtual ~CLocalShareInfoList();

// (2004.11.29, 배승원) Local Share Info를 구성하는 Interface를 제공한다.
public:
	// 1. Local Share Info을 추가하는 Interface를 제공한다.
	//		특히, 등록 직후 Local Share Setting Flag을 확인할 수 있도록 Object Pointer로 Return한다.
	// (2004.12.01, 배승원) 초기 등록시 공유값도 지정할 수 있도록 한다.
	CLocalShareInfo *	AddLocalShareInfo( const char *p_szLocalShareName, int p_nLocalShareSettingCode, const char *p_szLocalSharedData = NULL);

// (2004.11.29, 배승원) 구성된 Local Share Info를 제거하는 Interface를 제공한다.
public:
	// 1. 일괄 제거하는 Interface를 제공한다.
	void	RemoveAllLocalShareInfo( void);
	// 2. 지정되는 Local Share Info를 제거하는 Interface를 제공한다.
	BOOL	RemoveLocalShareInfo( const char *p_szLocalShareName);

// (2004.11.29, 배승원) 구성된 Local Share Info를 검색하는 Interface를 제공한다.
public:
	// 1. Local Share Info를 검색하는 Inteface를 제공한다.
	POSITION	FindLocalShareInfo( const char *p_szLocalShareName);

// (2004.11.29, 배승원) Local Shared Data를 설정 및 조회하는 Interface를 제공한다.
public:
	// 1. Local Shared Data를 설정하는 Interface를 제공한다.
	BOOL			SetLocalSharedData( const char *p_szLocalShareName, const char *p_szLocalSharedData);
	// 2. Local Shared Data를 조회하는 Interface를 제공한다.
	const char *	GetLocalSharedData( const char *p_szLocalShareName);
};

#endif // !defined(AFX_LOCALSHAREINFOLIST_H__4ADF05F0_B0F7_4610_B8DB_63CB55B9A91C__INCLUDED_)
