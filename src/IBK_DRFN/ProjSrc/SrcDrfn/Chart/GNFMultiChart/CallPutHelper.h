#ifndef __DFKJDKJFKDJFKDJKFJDK__FKDKFJKDJ_
#define __DFKJDKJFKDJFKDJKFJDK__FKDKFJKDJ_

#include "../../inc/ITreeListMng.h"

class CCallPutItemData
{
public:
	//CString m_szKey;
	CString m_szPrice;
	BOOL	m_isCall;	// call있으면 True
	BOOL	m_isPut;	// put있으면 True
	CString m_strCallCode;
	CString m_strPutCode;

public:
	CCallPutItemData()
	{
		m_isCall = FALSE;
		m_isPut = FALSE;
	}
};

typedef struct
{
	char szA[1+1];	// Call/Put여부
	char szB[6+1];	// 월물
	char szC[10+1];	// 행사가
	char szD[8+1];	// 종목코드
	//8+12+30+1
	//201C6227KR4201C62279코스피200 C 200806 227.5      I 
	//201C6227
	//KR4201C62279
	//코스피200 C 200806 227.5      
	//I 
} ST_OPTION_ITEM_DATA;

#define INIT_KOSPI200OPTION_ITEMDATA(pstVal, xxStr) \
	memset(pstVal, 0x00, sizeof(ST_OPTION_ITEM_DATA));	\
	memcpy(pstVal->szA, &xxStr[30], 1);					\
	memcpy(pstVal->szB, &xxStr[32], 6);					\
	memcpy(pstVal->szC, &xxStr[39], 5);					

#define INIT_JOPTION_ITEMDATA(pstVal, xxStr) \
	memset(pstVal, 0x00, sizeof(ST_OPTION_ITEM_DATA));	\
	memcpy(pstVal->szA, &xxStr[1], 1);					\
	memcpy(pstVal->szB, &xxStr[3], 6);					\
	memcpy(pstVal->szC, &xxStr[9], 10);					

typedef CList<CCallPutItemData*, CCallPutItemData*> List_CCallPutItemData;

class CCallPutIMonthData
{
public:
	CString m_szKey;
	List_CCallPutItemData m_ListData;

public:
	~CCallPutIMonthData()
	{
		ClearList();
	}

	BOOL fnDataToList(ST_OPTION_ITEM_DATA* pstItemData);
	void ClearList();
};

inline void CCallPutIMonthData::ClearList()
{
	CCallPutItemData* pItemData = NULL;
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
		delete m_ListData.GetNext(pos);
	
	m_ListData.RemoveAll();
}

// 월물을 관리하는 리스트에 추가한다.
inline BOOL CCallPutIMonthData::fnDataToList(ST_OPTION_ITEM_DATA* pstItemData)
{
	// 리스트에 있으면 업데이트
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
	{
		CCallPutItemData* pItem = m_ListData.GetNext(pos);
		if(pItem->m_szPrice.Compare(pstItemData->szC)==0)
		{
			if(pstItemData->szA[0]=='C')
			{
				pItem->m_isCall = TRUE;
				pItem->m_strCallCode = pstItemData->szD;
			}
			else if(pstItemData->szA[0]=='P')
			{
				pItem->m_isPut = TRUE;
				pItem->m_strPutCode = pstItemData->szD;
			}
			return TRUE;
		}
	}

	// 새로운 행사가이면 추가한다.
	CCallPutItemData* pItem = new CCallPutItemData;
	pItem->m_szPrice = pstItemData->szC;

	if(pstItemData->szA[0]=='C')
	{
		pItem->m_isCall = TRUE;
		pItem->m_strCallCode = pstItemData->szD;
	}
	else if(pstItemData->szA[0]=='P')
	{
		pItem->m_isPut = TRUE;
		pItem->m_strPutCode = pstItemData->szD;
	}

	m_ListData.AddTail(pItem);

	return TRUE;
}

typedef CList<CCallPutIMonthData*, CCallPutIMonthData*> List_CCallPutIMonthData;

class CCallPutIHelper
{
public:
	List_CCallPutIMonthData m_ListData;

	CCallPutIMonthData* m_pCurMontList;

	void Parsing(LIST_CCodeTypeA* pCodeType, int nCodeType = 0);	// 1 : kospi200옵션, 2 : 주식옵션, 0 : 그외
	void ClearList();
	CCallPutIMonthData* FindItemData(ST_OPTION_ITEM_DATA* pstItemData);
	CCallPutIMonthData* FindItemData(LPCSTR szMonth);
	BOOL fnDataToList(ST_OPTION_ITEM_DATA* pstItemData);

public:
	CCallPutIHelper()
	{
		m_pCurMontList = NULL;
	}

	~CCallPutIHelper()
	{
		ClearList();
	}
};



inline void CCallPutIHelper::Parsing(LIST_CCodeTypeA* pCodeType, int nCodeType /*= 0*/)
{
	if(nCodeType == 0)
		return;

	CCodeTypeA typeA;
	CString szOpName ;
	ST_OPTION_ITEM_DATA stItemData;
	ST_OPTION_ITEM_DATA* pstItemData = &stItemData;
	char* pszOpName;
	for(POSITION pos = pCodeType->GetHeadPosition(); pos;)
	{
		typeA = pCodeType->GetNext(pos);
		szOpName = typeA.m_szhname;
		pszOpName = (LPSTR)(LPCSTR)szOpName;

		if(nCodeType == 1)			// kospi200옵션
		{
			INIT_KOSPI200OPTION_ITEMDATA(pstItemData, pszOpName);
			memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
		}
		else if(nCodeType == 2)		// 주식옵션
		{
			INIT_JOPTION_ITEMDATA(pstItemData, pszOpName);
			memcpy(pstItemData->szD, (LPSTR)(LPCSTR)typeA.m_szcode, typeA.m_szcode.GetLength());
		}

		fnDataToList(pstItemData);
	}
}


inline BOOL CCallPutIHelper::fnDataToList(ST_OPTION_ITEM_DATA* pstItemData)
{
	if( m_pCurMontList && m_pCurMontList->m_szKey.Compare(pstItemData->szB)==0)
	{
		return m_pCurMontList->fnDataToList(pstItemData);
	}

	CCallPutIMonthData* pItemData = FindItemData(pstItemData);
	if(pItemData)
	{
		m_pCurMontList = pItemData;
		return m_pCurMontList->fnDataToList(pstItemData);
	}
	return FALSE;
}

// 존재하는 리스트에서만 찾는다.
inline CCallPutIMonthData* CCallPutIHelper::FindItemData(LPCSTR szMonth)
{
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
	{
		CCallPutIMonthData* pItem = m_ListData.GetNext(pos);
		if( pItem->m_szKey.Compare(szMonth)==0)
		{
			return pItem;
		}
	}
	return NULL;
}

// 리스트에 없으면 새로운 정보를 생성한다.
inline CCallPutIMonthData* CCallPutIHelper::FindItemData(ST_OPTION_ITEM_DATA* pstItemData)
{
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
	{
		CCallPutIMonthData* pItem = m_ListData.GetNext(pos);
		if( pItem->m_szKey.Compare(pstItemData->szB)==0)
		{
			return pItem;
		}
	}

	CCallPutIMonthData* pItem = new CCallPutIMonthData;
	pItem->m_szKey = pstItemData->szB;

	m_ListData.AddTail(pItem);
	return pItem;
}

inline void CCallPutIHelper::ClearList()
{
	CCallPutIMonthData* pMonthData = NULL;
	for(POSITION pos=m_ListData.GetHeadPosition(); pos;)
		delete m_ListData.GetNext(pos);

	m_ListData.RemoveAll();
	m_pCurMontList = NULL;
}

#endif //__DFKJDKJFKDJFKDJKFJDK__FKDKFJKDJ_
