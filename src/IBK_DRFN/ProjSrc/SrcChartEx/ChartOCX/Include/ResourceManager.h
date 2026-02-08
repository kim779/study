// ResourceManager.h: interface for the CResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCEMANAGER_H__53250219_D1C7_40D5_A024_4ACB6B207838__INCLUDED_)
#define AFX_RESOURCEMANAGER_H__53250219_D1C7_40D5_A024_4ACB6B207838__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "FontKey.h"

class CResourceManager  
{
public:
	CResourceManager(){};
	virtual ~CResourceManager(){};

//	virtual LONG UnAdviseFont(CFontKey &key){return 0;};
//	virtual LONG AdviseFont(CFontKey &key, CFont *pFont){return 0;};
	virtual LONG UnAdviseFont(CFontKey &key){return 0;};
	virtual LONG AdviseFont(CFontKey &key){return 0;};
	virtual LONG GetBaseFont(LONG lStyle){return NULL;};
	virtual BOOL CreateBaseFont(char* sFontName, int nSize){return 0;};
	virtual BOOL DeleteBaseFont(){return 0;};
};


#endif // !defined(AFX_RESOURCEMANAGER_H__53250219_D1C7_40D5_A024_4ACB6B207838__INCLUDED_)
