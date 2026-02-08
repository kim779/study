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

// [04/10/14] Graph Info를 조회하는 Interface를 제공한다.
public:
	// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]) = 0;
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo *		GetGraphInfo( const char *p_szGraphName) = 0;
};

#endif // !defined(AFX_IGRAPHINFOMANAGER_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
