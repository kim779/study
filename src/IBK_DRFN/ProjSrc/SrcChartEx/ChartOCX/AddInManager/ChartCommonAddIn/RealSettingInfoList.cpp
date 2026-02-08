// RealSettingInfoList.cpp: implementation of the CRealSettingInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealSettingInfoList.h"

#include "RsiPacketGroup.h"			// for CRsiPacketGroup

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.25, 배승원) Direct Advise/Unadvise를 위한 내부 공유병을 정의한다.
#define _CHART_INNER_LSHARE_NAME	"$$$ChartInnerLShareName$$$ - "

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRealSettingInfoList::CRealSettingInfoList()
{

}

CRealSettingInfoList::~CRealSettingInfoList()
{
	// (2004.11.29, 배승원) 등록된 모든 Real Setting Info를 제거한다.
	RemoveAllRealSettingInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) Real Setting Info를 구성하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. XFM과 Script에서 지정되는 Real 설정 속성 String Data로 구성하는 Interface를 제공한다.
BOOL CRealSettingInfoList::ResetRealSettingInfo( const char *p_szRealSettingData)
{
	// 0. 기존 구성을 초기화한다.
	RemoveAllRealSettingInfo();

	// 1. Real Setting String이 없는 경우를 처리한다. 
	if( !p_szRealSettingData || !*p_szRealSettingData) return TRUE;

	// 2. 최종 Return 값을 초기화한다.
	BOOL bReturn = TRUE;

	// 3. Real Setting Item을 하나씩 분리하여 처리한다.
	CString strRealSetting;
	char *szRealSetting = strRealSetting.GetBufferSetLength( strlen( p_szRealSettingData));
	strcpy( szRealSetting, p_szRealSettingData);
	while( szRealSetting && *szRealSetting)
	{
		// 3.1 다음 Real Setting Item을 확인하여 분리한다.
		char *szNextRealSetting = strchr( szRealSetting, ';');
		if( szNextRealSetting) *szNextRealSetting++ = '\0';

		// 3.2 1개의 Real Setting Item을 등록한다.
		bReturn = bReturn && AddRealSettingInfo( szRealSetting);

		// 3.3 다음 Real Setting Item을 진행하도록 설정한다.
		szRealSetting = szNextRealSetting;
	}

	// 4. 최종 Return 값을 반환한다.
	return bReturn;
}

// 2. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
//		특히, p_pstrShareName과 p_pstrSharedData가 주어지는 경우에는 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
//			p_szRealSettingItem에는 Local Share Name이 없이 바로 Real Key Code가 주어진다. 
//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
//		(ResetRealSettingInfo()와 AddRealItem()가 공유할 수 있도록 하여 CString의 Pointer로 처리한다.)
BOOL CRealSettingInfoList::AddRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator)
{
	// 1. Real Setting Item Data를 확인한다.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 2. Real Setting Item을 복사한다.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 3.5 Item간 구분자가 있을 수 있으니, 이를 제거한다.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';

	// 3. Chart Packet Name을 구한다.
	char *szChartPacketName = szRealSettingItem;

	// 4. Real Packet Name을 구하여 분리한다.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';

	// 5. Real Field Name을 구하여 분리한다.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';

	// 6. Local Share Name을 구하여 분리한다.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';

	// 7. 반환할 Local Shared Data를 미리 설정한다.
	if( p_pstrSharedData) *p_pstrSharedData = szShareName;

	// 8. 해당 Real Setting Info를 등록한다.
	return AddRealSettingInfo( szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName);
}

// 3. 1개의 Real 설정 Data를 추가하는 Interface를 제공한다.
//		역시, p_strShareName가 주어지는 경우로, 별도의 Method(AddRealItem)를 통하여 지정되는 방식이며,
//			p_szRealSettingItem에는 Local Share Name이 없이 바로 Real Key Code가 주어진다. 
//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
//		(ResetRealSettingInfo()와 AddRealItem()가 공유할 수 있도록 하여 CString의 Pointer로 처리한다.)
BOOL CRealSettingInfoList::AddRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName)
{
	// 7. p_pstrShareName가 지정되는 경우,
	//			이 Real Setting Item은 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
	//			Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		이때에는 적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	if( p_pstrShareName)
	{
		*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szShareName;
		p_szShareName = *p_pstrShareName;
	}

	// 8. 해당 Real Setting Info Packet Group을 검색한다. (없는 경우 생성하여 등록한다.)
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup)
	{
		AddTail( new CRsiPacketGroup( p_szRealPacketName));
		posRsiPacketGroup = GetTailPosition();
	}

	// 9. 검색된 Real Setting Info Packet Group에 Real Setting Info를 등록시킨다.
	// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
	CRsiPacketGroup *&pRsiPacketGroup = GetAt( posRsiPacketGroup);
	return pRsiPacketGroup->AddRealSettingInfo( p_szChartPacketName, p_szRealFieldName, p_szShareName);
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
BOOL CRealSettingInfoList::AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, CString *p_pstrSharedData, char p_cSeparator)
{
	// 1. Real Setting Item Data를 확인한다.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 2. Real Setting Item을 복사한다.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 3.5 Item간 구분자가 있을 수 있으니, 이를 제거한다.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';

	// 3. Chart Packet Name을 구한다.
	char *szChartPacketName = szRealSettingItem;

	// 4. Real Packet Name을 구하여 분리한다.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';

	// 5. Real Field Name을 구하여 분리한다.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';

	// 6. Local Share Name을 구하여 분리한다.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';

	// 7. 반환할 Local Shared Data를 미리 설정한다.
	if( p_pstrSharedData) *p_pstrSharedData = szShareName;

	// 8. 해당 Real Setting Info를 등록한다.
	return AddRealSettingInfo(lpszRQ, szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName);
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
BOOL CRealSettingInfoList::AddRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szShareName, CString *p_pstrShareName)
{
	// 7. p_pstrShareName가 지정되는 경우,
	//			이 Real Setting Item은 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
	//			Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		이때에는 적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	if( p_pstrShareName)
	{
		*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szShareName;
		p_szShareName = *p_pstrShareName;
	}

	// 8. 해당 Real Setting Info Packet Group을 검색한다. (없는 경우 생성하여 등록한다.)
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup)
	{
		AddTail( new CRsiPacketGroup( p_szRealPacketName));
		posRsiPacketGroup = GetTailPosition();
	}

	// 9. 검색된 Real Setting Info Packet Group에 Real Setting Info를 등록시킨다.
	// (2006/4/3 - Seung-Won, Bae) Use Reference for Numega Memory Leak Checking.
	CRsiPacketGroup *&pRsiPacketGroup = GetAt( posRsiPacketGroup);
	return pRsiPacketGroup->AddRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, p_szShareName);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 Real Setting Info를 제거하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 일괄 제거하는 Interface를 제공한다.
void CRealSettingInfoList::RemoveAllRealSettingInfo( void)
{
	while( !IsEmpty())
	{
		CRsiPacketGroup *pRsiPacketGroup = RemoveHead();
		pRsiPacketGroup->RemoveAllRealSettingInfo();
		delete pRsiPacketGroup;
	}
}

// 2. 별도의 Method(RemoveRealItem)를 통하여 지정되는 1개의 Real Setting Info를 삭제하는 Interface를 제공한다.
//		특히, Local Share Name이 없이 바로 Real Key Code가 주어져,
//			적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환해야 한다.
//		참고로 더이상 해당 공유 Key로 설정된 Real이 없는 경우에만 공유명이 반환된다.
//		(AddRealSettingInfo()와 Interface를 통일하도록 하여 CString의 Pointer로 처리한다.)
BOOL CRealSettingInfoList::RemoveRealSettingInfo( const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator)
{
	// 0. 먼저 오류의 경우를 대비하여 공유명을 Clear한다.
	if( !p_pstrShareName) return FALSE;
	p_pstrShareName->Empty();

	// 0. Real Setting Item Data를 확인한다.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 1. 먼저 해당 Real Setting Item을 제거한다.
	// 1.1 Real Setting Item을 복사한다.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 1.2 Item간 구분자가 있을 수 있으니, 이를 제거한다.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';
	// 1.3 Chart Packet Name을 구한다.
	char *szChartPacketName = szRealSettingItem;
	// 1.4 Real Packet Name을 구하여 분리한다.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';
	// 1.5 Real Field Name을 구하여 분리한다.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';
	// 1.6 Local Share Name을 구하여 분리한다.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';
	// 1.7 이 Real Setting Item은 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
	//			Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		이때에는 적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	// 1.8 Real Setting Info를 제거한다.
	if( !RemoveRealSettingInfo( szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName)) return FALSE;

	// 2. 같은 공유 Key로 남은 Real Setting Info가 있는지 확인한다.
	//		Local Share Info에서도 공유 명을 제거할지 결정하기 위함이다.
	//		제거하지 말아야 하는 경우 p_strShareName를 Empty() 처리한다.
	if( FindShareName( *p_pstrShareName)) p_pstrShareName->Empty();

	// 3. 처리 완료된 경우 TRUE를 Return한다.
	return TRUE;
}

// 3. Real Setting Info 값을 지정받아 Real Setting Info를 제거하는 Interface를 제공한다.
//		역시, p_strShareName가 주어지는 경우로, 별도의 Method(AddRealItem)를 통하여 지정되는 방식이며,
//			Local Share Name이 없이 바로 Real Key Code가 주어진다. 
//		적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
//		(AddRealSettingInfo()와 Interface를 통일하도록 하여 CString의 Pointer로 처리한다.)
BOOL CRealSettingInfoList::RemoveRealSettingInfo( const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode, CString *p_pstrShareName)
{
	// 0. 삭제할 Local Share Name을 구성한뒤 이용한다.
	*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szRealKeyCode;

	// 1. Real Setting Info Packet Group을 검색하여 Real Setting Info를 제거한다.
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup) return FALSE;
	CRsiPacketGroup *pRsiPacketGroup = GetAt( posRsiPacketGroup);
	if( !pRsiPacketGroup->RemoveRealSettingInfo( p_szChartPacketName, p_szRealFieldName, *p_pstrShareName)) return FALSE;

	// 2. 제거가 성공한 경우, 해당 Real Setting Info Packet Group이 비었는지 확인하고 이도 삭제한다.
	if( pRsiPacketGroup->IsEmpty())
	{
		RemoveAt( posRsiPacketGroup);
		delete pRsiPacketGroup;
	}

	return TRUE;
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
BOOL CRealSettingInfoList::RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szRealSettingItem, CString *p_pstrShareName, char p_cSeparator)
{
	// 0. 먼저 오류의 경우를 대비하여 공유명을 Clear한다.
	if( !p_pstrShareName) return FALSE;
	p_pstrShareName->Empty();

	// 0. Real Setting Item Data를 확인한다.
	if( !p_szRealSettingItem || !*p_szRealSettingItem) return FALSE;

	// 1. 먼저 해당 Real Setting Item을 제거한다.
	// 1.1 Real Setting Item을 복사한다.
	CString strRealSettingItem;
	char *szRealSettingItem = strRealSettingItem.GetBufferSetLength( strlen( p_szRealSettingItem));
	strcpy( szRealSettingItem, p_szRealSettingItem);
	// 1.2 Item간 구분자가 있을 수 있으니, 이를 제거한다.
	char *szNextRealSetting = strchr( szRealSettingItem, ';');
	if( szNextRealSetting) *szNextRealSetting++ = '\0';
	// 1.3 Chart Packet Name을 구한다.
	char *szChartPacketName = szRealSettingItem;
	// 1.4 Real Packet Name을 구하여 분리한다.
	char *szRealPacketName = strchr( szChartPacketName, p_cSeparator);
	if( !szRealPacketName) return FALSE;
	*szRealPacketName++ = '\0';
	// 1.5 Real Field Name을 구하여 분리한다.
	char *szRealFieldName = strchr( szRealPacketName, p_cSeparator);
	if( !szRealFieldName) return FALSE;
	*szRealFieldName++ = '\0';
	// 1.6 Local Share Name을 구하여 분리한다.
	const char *szShareName = strchr( szRealFieldName, p_cSeparator);
	if( !szShareName) return FALSE;
	*( char *)szShareName++ = '\0';
	// 1.7 이 Real Setting Item은 별도의 Method(AddRealItem)를 통하여 지정되는 방식으로
	//			Local Share Name이 없이 바로 Real Key Code가 주어진다. 
	//		이때에는 적절한 Local Share Name을 생성하여 적용하고, 참조할 수 있도록 반환한다.
	// 1.8 Real Setting Info를 제거한다.
	if( !RemoveRealSettingInfo(lpszRQ, szChartPacketName, szRealPacketName, szRealFieldName, szShareName, p_pstrShareName)) return FALSE;

	// 2. 같은 공유 Key로 남은 Real Setting Info가 있는지 확인한다.
	//		Local Share Info에서도 공유 명을 제거할지 결정하기 위함이다.
	//		제거하지 말아야 하는 경우 p_strShareName를 Empty() 처리한다.
	if( FindShareName(*p_pstrShareName)) p_pstrShareName->Empty();

	// 3. 처리 완료된 경우 TRUE를 Return한다.
	return TRUE;
}

// RQ별 Real관리 : 복수종목 - ojtaso (20070111)
BOOL CRealSettingInfoList::RemoveRealSettingInfo(LPCTSTR lpszRQ, const char *p_szChartPacketName, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode, CString *p_pstrShareName)
{
	// 0. 삭제할 Local Share Name을 구성한뒤 이용한다.
	*p_pstrShareName = CString( _CHART_INNER_LSHARE_NAME) + p_szRealKeyCode;

	// 1. Real Setting Info Packet Group을 검색하여 Real Setting Info를 제거한다.
	POSITION posRsiPacketGroup = FindRsiPacketGroup( p_szRealPacketName);
	if( !posRsiPacketGroup) return FALSE;
	CRsiPacketGroup *pRsiPacketGroup = GetAt( posRsiPacketGroup);
	if( !pRsiPacketGroup->RemoveRealSettingInfo(lpszRQ, p_szChartPacketName, p_szRealFieldName, *p_pstrShareName)) return FALSE;

	// 2. 제거가 성공한 경우, 해당 Real Setting Info Packet Group이 비었는지 확인하고 이도 삭제한다.
	if( pRsiPacketGroup->IsEmpty())
	{
		RemoveAt( posRsiPacketGroup);
		delete pRsiPacketGroup;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 Real Setting Info를 검색하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Real Setting Info 값을 지정받아 검색하는 Inteface를 제공한다.
POSITION CRealSettingInfoList::FindRsiPacketGroup( const char *p_szRealPacketName)
{
	POSITION posPrevRsiPacketGroup;
	POSITION posRsiPacketGroup = GetHeadPosition();
	while( posRsiPacketGroup)
	{
		posPrevRsiPacketGroup = posRsiPacketGroup;
		CRsiPacketGroup *pRsiPacketGroup = GetNext( posRsiPacketGroup);
		if( pRsiPacketGroup->m_strRealPacketName	== p_szRealPacketName) return posPrevRsiPacketGroup;
	}
	return NULL;
}

// 2. Local Share Name만을 지정받아 해당 공유명으로 등록된 Real Setting Info가 있는지 검색한다.
POSITION CRealSettingInfoList::FindShareName( const char *p_szShareName)
{
	POSITION posPrevRsiPacketGroup;
	POSITION posRsiPacketGroup = GetHeadPosition();
	while( posRsiPacketGroup)
	{
		posPrevRsiPacketGroup = posRsiPacketGroup;
		CRsiPacketGroup *pRsiPacketGroup = GetNext( posRsiPacketGroup);
		if( pRsiPacketGroup->FindShareName( p_szShareName)) return posPrevRsiPacketGroup;
	}
	return NULL;
}

/*
// (2004.11.25, 배승원) 허수봉을 위한 지수 Real 요청이 등록되어 있는지 확인하는데 이용된다.
//		특히 자료일자가 아직 Real을 수신하는지 확인할 수 있도록 Packet만으로 검색하는 기능도 제공한다.
//		"자료일자" Packet과 지수 Real을 명시하면서 p_bOnlyThisOne를 TRUE로 명시하면 지정된 지수 Real이 반드시 있고,
//		다른 Real요청이 설정되지 않음을 확인하여, 허수봉을 위한 지수 Real의 Unadivse가 가능토록 한다.
BOOL CKoscomChartCtrl::LookupRealInfo( CPacket *p_pRealPacket, const char *p_szRealPacketName, const char *p_szRealFieldName, const char *p_szRealKeyCode, BOOL p_bOnlyThisOne)
{
	if( m_AdviseList != NULL )
	{
		// 1. 특정 Real 설정을 검색하는 기능을 처리한다.
		if( !p_bOnlyThisOne)
		{
			POSITION posAdviseList = m_AdviseList->GetHeadPosition();
			while( posAdviseList)
			{
				CRealInfo *pRealInfo = m_AdviseList->GetNext( posAdviseList);
				if( pRealInfo->LookupInfo( p_pRealPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode, FALSE)) return TRUE;
			}
			return FALSE;
		}
		// 2. 특정 Real 설정만 남았는지 확인하는 기능을 처리한다.
		// 2.1 먼저 그 특정 설정이 있는지를 확인한다.
		if( !LookupRealInfo( p_pRealPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode, FALSE)) return FALSE;
		// 2.2 있는 경우, 그 외의 설정이 있는지 확인한다.
		POSITION posAdviseList = m_AdviseList->GetHeadPosition();
		while( posAdviseList)
		{
			CRealInfo *pRealInfo = m_AdviseList->GetNext( posAdviseList);
			if( pRealInfo->LookupInfo( p_pRealPacket, p_szRealPacketName, p_szRealFieldName, p_szRealKeyCode, TRUE)) return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}


*/