#if !defined(AFX_LVR_H__5080FF33_5F42_4901_ADBD_7ED5B45580A5__INCLUDED_)
#define AFX_LVR_H__5080FF33_5F42_4901_ADBD_7ED5B45580A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLVR : public CLineBase  
{
public:
	CLVR(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLVR();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LVR_H__5080FF33_5F42_4901_ADBD_7ED5B45580A5__INCLUDED_)
