#if !defined(AFX_LMOMENTOM_H__B07F900C_D99F_4CDE_BEDE_8365148E3297__INCLUDED_)
#define AFX_LMOMENTOM_H__B07F900C_D99F_4CDE_BEDE_8365148E3297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLMomentom : public CLineBase  
{
public:
	CLMomentom(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLMomentom();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LMOMENTOM_H__B07F900C_D99F_4CDE_BEDE_8365148E3297__INCLUDED_)
