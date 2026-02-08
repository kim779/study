#pragma once

#include "afxcoll.h"




// CItemInChart 명령 대상입니다.

class CItemInChart : public CObject
{
public:
	long m_lLengthBackup;
	CString m_strRecord;
	CItemInChart();
	virtual ~CItemInChart();

	CString m_strMarketType;
	CString m_strCode;
	short m_sExponent;
	CString m_strName;
	CString m_strField;
	CString m_strType;
	WORD m_wType;
	bool m_bValid;
	long m_lLength;
	CString m_strFormat;
	BOOL m_bReal;
	int m_nFID;
	int m_nGID;
	int m_nIndex;
};

class CItemInChartEu : public CItemInChart
{
public:
	CItemInChartEu();
	
public:
	int m_nRealID;

	CString m_szRealID;	////@Champ-union090827-alzioyes.
};

//class CItemList :
//	public CObList
//{
//public:
//	CItemList(void);
//	~CItemList(void);
//
//};


