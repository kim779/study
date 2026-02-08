// IWGraphInfoManager.h: interface for the CIWGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
#define AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/IndicatorList.h"			// for CIndicatorList
#include "../_IWUnknown.h"
#include "_IGraphInfoManager.h"
#include "_IChartOCX.h"												// for IChartOCX

// (2004.10.13, 배승원) IndicatorList의 Interface Wrapper Class를 정의한다.
class CIWGraphInfoManager : public CIWUnknown, public IGraphInfoManager
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IGraphInfoManager, CIWGraphInfoManager, CIndicatorList, m_pIndicatorList)
	
// [05/07/15] Getting Graph Info Interface
protected:
	virtual IGraphInfo *	GetGraphInfo( const IEGraphInfoListType p_eGraphInfoType, const char *p_szGraphName);
	virtual IGraphInfo *	GetIndicatorInfo( const IEGraphInfoListType p_eGraphInfoType, const char *p_szIndicatorName, const IEGraphInfoGettingType p_eGettingType);

// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Type and Style Infos.
protected:
	virtual IStringList *		GetAllGraphTypeNames( void);
	virtual BOOL				GetGraphTypeAndStyle( int nType, int nStyle, ILPCSTR &p_pszType, ILPCSTR &p_pszStyle);
	virtual BOOL				GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, int &p_nTypeIndex, int &p_nStyleIndex);
	virtual BOOL				GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, const char *p_szDrawStyleName, int &p_nTypeIndex, int &p_nStyleIndex, int &p_nDrawStyleIndex);
	virtual const IStringList *	GetAllGraphTypeStyleNameList( const char *p_szGraphTypeName);
	virtual const IStringList *	GetAllGraphDrawStyleNameList( const char *p_szGraphTypeName);

// (2006/7/6 - Seung-Won, Bae) Retrieve the Indicator Names
protected:
	virtual IStringList *	GetIndicatorGroupNameStringList( void);
	virtual IStringList *	GetIndicatorNameStringList( const char *p_szGroupName);
	virtual	IStringList *	GetIndicatorNameStringList( IEGraphInfoListType p_eGraphInfoListType, IEGraphInfoType p_eIndicatorInfoType);

// (2006/8/15 - Seung-Won, Bae) Control GraphInfo
protected:
	virtual BOOL	RemoveAllIndicatorInfo( IEGraphInfoListType p_eGraphInfoListType);

};

#endif // !defined(AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
