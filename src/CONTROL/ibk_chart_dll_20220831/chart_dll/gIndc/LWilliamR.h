#if !defined(AFX_LWILLIAMR_H__3776B9D4_C035_464A_AC38_1174C0C9430F__INCLUDED_)
#define AFX_LWILLIAMR_H__3776B9D4_C035_464A_AC38_1174C0C9430F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLWilliamR : public CLineBase  
{
public:
	CLWilliamR(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLWilliamR();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LWILLIAMR_H__3776B9D4_C035_464A_AC38_1174C0C9430F__INCLUDED_)
