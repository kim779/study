#if !defined(AFX_LPSY_H__0E06C0E8_7A59_48CD_A6FE_0C70BA09972E__INCLUDED_)
#define AFX_LPSY_H__0E06C0E8_7A59_48CD_A6FE_0C70BA09972E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLPsy : public CLineBase  
{
public:
	CLPsy(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLPsy();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LPSY_H__0E06C0E8_7A59_48CD_A6FE_0C70BA09972E__INCLUDED_)
