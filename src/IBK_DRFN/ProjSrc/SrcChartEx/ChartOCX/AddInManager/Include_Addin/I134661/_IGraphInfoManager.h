// IGraphInfoManager.h: interface for the IGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFOMANAGER_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
#define AFX_IGRAPHINFOMANAGER_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>	// for CTypedPtrList
#include "_AddInVer.h"	// for AddIn Version Define

interface IGraphInfo;
interface IGraphInfoManager : public IUnknown  
{
protected:
	virtual ~IGraphInfoManager()	{};

public:
	// (2006/5/13 - Seung-Won, Bae) 4 Target Define for IGraphInfo's Interface
	typedef enum
	{
		IEILT_SITE_DEFAULT,
		IEILT_MAP_DEFAULT,
		IEILT_USER_DEFAULT,
		IEILT_USER
	} IEGraphInfoListType;

	// (2006/5/13 - Seung-Won, Bae) 3 Type Define for IGraphInfo's Interface
	typedef enum
	{
		IEIIT_GENERAL,
		IEIIT_SPECIAL,
		IEIIT_NONE
	} IEGraphInfoType;

	// (2006/5/31 - Seung-Won, Bae) 3 Type Defint for IGraphInfo's Interface
	typedef enum
	{
		IEIIGT_WITHOUT_DEFAULT,
		IEIIGT_WITH_DEFAULT_GET,
		IEIIGT_WITH_DEFAULT_GET_AND_SET
	} IEGraphInfoGettingType;


// [04/10/14] Graph Info를 조회하는 Interface를 제공한다.
public:
	// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]) = 0;
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo *		GetGraphInfo( IEGraphInfoListType p_eGraphInfoType, const char *p_szGraphName, IEGraphInfoGettingType p_nDefaultOption = IEIIGT_WITHOUT_DEFAULT) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Type and Style Infos.
public:
	virtual int							GetAllGraphTypeNames( CList< CString, CString> &p_slGraphTypeNames) = 0;
	virtual BOOL						GetGraphTypeAndStyle( int nType, int nStyle, CString& strType, CString& strStyle) = 0;
	virtual BOOL						GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, int &p_nTypeIndex, int &p_nStyleIndex) = 0;
	virtual BOOL						GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, const char *p_szDrawStyleName, int &p_nTypeIndex, int &p_nStyleIndex, int &p_nDrawStyleIndex) = 0;
	virtual CList< CString, CString> *	GetAllGraphTypeStyleNameList( const char *p_szGraphTypeName) = 0;
	virtual CList< CString, CString> *	GetAllGraphDrawStyleNameList( const char *p_szGraphTypeName) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve the Indicator Names
public:
	virtual BOOL	GetIndicatorNameList( IEGraphInfoListType p_eIndicatorListType, IEGraphInfoType p_eIndicatorInfoType, CList<CString, CString>& GraphNameList) = 0;
};

#endif // !defined(AFX_IGRAPHINFOMANAGER_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
