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
#include "_IString.h"	// for ILPCSTR

interface IGraphInfo;
interface IStringList;
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
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo *	GetGraphInfo( const IEGraphInfoListType p_eGraphInfoType, const char *p_szGraphName) = 0;
	virtual IGraphInfo *	GetIndicatorInfo( const IEGraphInfoListType p_eGraphInfoType, const char *p_szIndicatorName, const IEGraphInfoGettingType p_eGettingType) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Type and Style Infos.
public:
	virtual IStringList *		GetAllGraphTypeNames( void) = 0;
	virtual BOOL				GetGraphTypeAndStyle( int nType, int nStyle, ILPCSTR &p_pszType, ILPCSTR &p_pszStyle) = 0;
	virtual BOOL				GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, int &p_nTypeIndex, int &p_nStyleIndex) = 0;
	virtual BOOL				GetGraphTypeAndStyle( const char *p_szTypeName, const char *p_szStyleName, const char *p_szDrawStyleName, int &p_nTypeIndex, int &p_nStyleIndex, int &p_nDrawStyleIndex) = 0;
	virtual const IStringList *	GetAllGraphTypeStyleNameList( const char *p_szGraphTypeName) = 0;
	virtual const IStringList *	GetAllGraphDrawStyleNameList( const char *p_szGraphTypeName) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve the Indicator Names
public:
	virtual IStringList *	GetIndicatorGroupNameStringList( void) = 0;
	virtual IStringList *	GetIndicatorNameStringList( const char *p_szGroupName) = 0;
	virtual	IStringList *	GetIndicatorNameStringList( IEGraphInfoListType p_eGraphInfoListType, IEGraphInfoType p_eIndicatorInfoType) = 0;

// (2006/8/15 - Seung-Won, Bae) Control GraphInfo
public:
	virtual BOOL	RemoveAllIndicatorInfo( IEGraphInfoListType p_eGraphInfoListType) = 0;
};

#endif // !defined(AFX_IGRAPHINFOMANAGER_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
