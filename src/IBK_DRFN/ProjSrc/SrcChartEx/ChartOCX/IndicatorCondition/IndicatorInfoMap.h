// IndicatorInfoMap.h: interface for the CIndicatorInfoMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORINFOMAP_H__4BBBF49F_43CB_4AC2_9C1B_A5FB6007A72E__INCLUDED_)
#define AFX_INDICATORINFOMAP_H__4BBBF49F_43CB_4AC2_9C1B_A5FB6007A72E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../Include_Chart/BlockBaseEnum.h"							// for CChartInfo
#include "../Include_Chart/DLL_Load/IndicatorList.h"				// for EIndicatorInfoType
#include "../Include_Chart/DLL_Load/IndicatorConditionDefineData.h"	// for CStringDataList

class CIndicatorInfo; 
typedef CTypedPtrMap < CMapStringToPtr, CString, CIndicatorInfo *> CIndicatorInfoMapBase;

// (2006/12/8 - Seung-Won, Bae) Define Indicattor Info Map.
class CIndicatorInfoMap : public CIndicatorInfoMapBase
{
public:
	CIndicatorInfoMap( EIndicatorInfoType p_eIndicatorInfoType);
	virtual ~CIndicatorInfoMap();

protected:
	EIndicatorInfoType	m_eIndicatorInfoType;
public:
	EIndicatorInfoType	GetIndicatorInfoType( void) const	{	return m_eIndicatorInfoType;	}

public:
	BOOL			Delete( CIndicatorInfo *p_pIndicatorInfo);
	BOOL			DeleteAll( void);
	BOOL			GetIndicatorNameList( CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode) const;

public:
	virtual BOOL				AddIndicatorInfo( CIndicatorInfo *p_pIndicatorInfo)																				= 0;
	virtual BOOL				RemoveAllIndicator( const char *p_szIndicatorName)																				{	return FALSE;	}
	virtual CIndicatorInfo *	GetFirstIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator, CChartInfo::CHART_MODE p_eChartMode)	const	{	return NULL;	}
	virtual CIndicatorInfo *	GetNextIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator)											const	{	return NULL;	}
	virtual BOOL				GetGraphNameList( CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode)										const	{	return FALSE;	}
	virtual BOOL				GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList& indiGroupDataList, CChartInfo::CHART_MODE p_eChartMode)		const	{	return FALSE;	}

public:
	CIndicatorInfo *	GetIndicatorInfo( const char *p_szIndicatorKeyName, CChartInfo::CHART_MODE p_eChartMode);
};

// (2006/12/8 - Seung-Won, Bae) Define Default Indicattor Info Map.
class CDefaultIndicatorInfoMap : public CIndicatorInfoMap
{
public:
	CDefaultIndicatorInfoMap( EIndicatorInfoType p_eIndicatorInfoType);
	virtual ~CDefaultIndicatorInfoMap();

protected:
	CStringList		m_slIndicatorNameList;

public:
	virtual BOOL				AddIndicatorInfo( CIndicatorInfo *p_pIndicatorInfo);
	virtual BOOL				GetAllIndicatorNameWithGroupName( CIndicatorGroupDataList& indiGroupDataList, CChartInfo::CHART_MODE p_eChartMode)		const;
};

// (2006/12/8 - Seung-Won, Bae) Define Graph Indicattor Info Map.
class CGraphIndicatorInfoMap : public CIndicatorInfoMap
{
public:
	CGraphIndicatorInfoMap( EIndicatorInfoType p_eIndicatorInfoType);
	virtual ~CGraphIndicatorInfoMap();

public:
	virtual BOOL				AddIndicatorInfo( CIndicatorInfo *p_pIndicatorInfo);
	virtual BOOL				RemoveAllIndicator( const char *p_szIndicatorName);
	virtual CIndicatorInfo *	GetFirstIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator)	const;
	virtual CIndicatorInfo *	GetNextIndicatorInfo( const char *p_szIndicatorName, POSITION *p_ppsnIndicator)		const;
	virtual BOOL				GetGraphNameList( CStringDataList& dataList, CChartInfo::CHART_MODE p_eChartMode)	const;
};

#endif // !defined(AFX_INDICATORINFOMAP_H__4BBBF49F_43CB_4AC2_9C1B_A5FB6007A72E__INCLUDED_)
