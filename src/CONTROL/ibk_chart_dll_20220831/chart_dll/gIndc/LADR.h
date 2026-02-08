#if !defined(AFX_LADR_H__868A5BE1_C7F1_4A98_B2FB_21110C4596A0__INCLUDED_)
#define AFX_LADR_H__868A5BE1_C7F1_4A98_B2FB_21110C4596A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLADR : public CLineBase  
{
public:
	CLADR(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLADR();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LADR_H__868A5BE1_C7F1_4A98_B2FB_21110C4596A0__INCLUDED_)
