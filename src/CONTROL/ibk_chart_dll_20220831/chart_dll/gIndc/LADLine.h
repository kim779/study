#if !defined(AFX_LADLINE_H__30D9F205_CAEB_400D_B581_8E81C7E2EA0C__INCLUDED_)
#define AFX_LADLINE_H__30D9F205_CAEB_400D_B581_8E81C7E2EA0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLADLine : public CLineBase  
{
public:
	CLADLine(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLADLine();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);	
};

#endif // !defined(AFX_LADLINE_H__30D9F205_CAEB_400D_B581_8E81C7E2EA0C__INCLUDED_)
