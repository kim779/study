// FavoriteThingsMng.h: interface for the CFavoriteThingsMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAVORITETHINGSMNG_H__6E2FD617_9F17_48E6_ADAE_8617DC4B2AC3__INCLUDED_)
#define AFX_FAVORITETHINGSMNG_H__6E2FD617_9F17_48E6_ADAE_8617DC4B2AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./define/ChartMsgDef.h"
#include <afxtempl.h>
class CFavoriteThingsMng  
{
public:
	CFavoriteThingsMng();
	virtual ~CFavoriteThingsMng();
	void SetPath(CString strPath);
	BOOL LoadGrpInfo();
	BOOL SaveItemInfo(stFvrData  *pFvrData, LPCTSTR lpGrpName = NULL);
	BOOL DeleteItemInfo(LPCTSTR lpItemName, LPCTSTR lpGrpName = NULL);
	BOOL LoadItemInfo(LPCTSTR lpGrpName);
	BOOL NewGrp(LPCTSTR lpGrpName);
	BOOL DeleteGrp(LPCTSTR lpGrpName);
	CString GetCurrentGrp() { return m_strCurrentGrp;} 
	
	CStringArray m_strArrayGrp;
	CArray<stFvrData,stFvrData> m_ArrayFvrItem;

private:
	int FindGrp(LPCTSTR lpGrpName);
	int FindItem(stFvrData  *pFvrData);

	CString m_strPathFavorite;
	CString m_strCurrentGrp;
};

#endif // !defined(AFX_FAVORITETHINGSMNG_H__6E2FD617_9F17_48E6_ADAE_8617DC4B2AC3__INCLUDED_)
