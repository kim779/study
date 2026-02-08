// RealSettingInfoList.h: interface for the CRealSettingInfoList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALSETTINGINFOLIST_H__3B37BD54_A9BA_4125_95E6_1C46C6103BB2__INCLUDED_)
#define AFX_REALSETTINGINFOLIST_H__3B37BD54_A9BA_4125_95E6_1C46C6103BB2__INCLUDED_

// (2004.11.29, 배승원) Real Setting Info를 관리하는 Data Object List Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>		// for CTypedPtrList

// (2004.11.29, 배승원) Real Setting Info List Base Class를 정의한다.
class CRsiPacketGroup;
typedef CTypedPtrList< CObList, CRsiPacketGroup *> CRealSettingInfoListBase;

// (2004.11.29, 배승원) Real Setting Info를 관리하는 Data Object List Class를 정의한다.
class CRealSettingInfoList : public CRealSettingInfoListBase
{
public:
	CRealSettingInfoList();
	virtual ~CRealSettingInfoList();

// (2004.11.29, 배승원) Real Setting Info를 구성하는 Interface를 제공한다.
public:
	// 1. XFM과 Script에서 지정되는 Real 설정 속성 String Data로 구성하는 Interface를 제공한다.
	BOOL	ResetRealSettingInfo( const char *p_szRealSettingData);
	// 2. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
	//		특히, p_pstrShareName과 p_pstrSharedData가 주어지는 경우에는 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
	//			p_szRealSettingItem에는 Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	//		(ResetRealSettingInfo()와 AddRealItem()가 공유할 수 있도록 하여 CString의 Pointer로 처리한다.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	BOOL	AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, CString *p_pstrSharedData = NULL, char p_cSeparator = '.');
	// 3. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
	//		역시, p_strShareName가 주어지는 경우로, 별도의 Method(AddRealItem)를 통하여 지정되는 방식이며,
	//			p_szRealSettingItem에는 Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	//		(ResetRealSettingInfo()와 AddRealItem()가 공유할 수 있도록 하여 CString의 Pointer로 처리한다.)
	BOOL	AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	BOOL	AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator);
	BOOL	AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

// (2004.11.29, 배승원) 구성된 Real Setting Info를 제거하는 Interface를 제공한다.
public:
	// 1. 일괄 제거하는 Interface를 제공한다.
	void	RemoveAllRealSettingInfo( void);
	// 2. 별도의 Method(RemoveRealItem)를 통하여 지정되는 1개의 Real Setting Info를 삭제하는 Interface를 제공한다.
	//		특히, Local Share Name이 없이 바로 Real Key Code가 주어져,
	//			적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환해야 한다.
	//		참고로 더이상 해당 공유 Key로 설정된 Real이 없는 경우에만 공유명이 반환된다.
	//		(AddRealSettingInfo()와 Interface를 통일하도록 하여 CString의 Pointer로 처리한다.)
	// (2006/4/28 - Seung-Won, Bae) with Separator
	BOOL	RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.');
	// 3. Real Setting Info 값을 지정받아 Real Setting Info를 제거하는 Interface를 제공한다.
	//		역시, p_strShareName가 주어지는 경우로, 별도의 Method(AddRealItem)를 통하여 지정되는 방식이며,
	//			Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	//		(AddRealSettingInfo()와 Interface를 통일하도록 하여 CString의 Pointer로 처리한다.)
	BOOL	RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

	// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
	BOOL	RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName = NULL, char p_cSeparator = '.');
	BOOL	RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName);

// (2004.11.29, 배승원) 구성된 Real Setting Info를 검색하는 Interface를 제공한다.
public:
	// 1. Real Setting Info 값을 지정받아 검색하는 Inteface를 제공한다.
	POSITION	FindRsiPacketGroup( const char *p_szRealPacketName);
	// 2. Local Share Name만을 지정받아 해당 공유명으로 등록된 Real Setting Info가 있는지 검색한다.
	POSITION	FindShareName( const char *p_szShareName);
};

#endif // !defined(AFX_REALSETTINGINFOLIST_H__3B37BD54_A9BA_4125_95E6_1C46C6103BB2__INCLUDED_)
