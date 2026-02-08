#if !defined(AFX_LNEWPSY_H__34FC2C61_9D56_431A_8213_4911953609A8__INCLUDED_)
#define AFX_LNEWPSY_H__34FC2C61_9D56_431A_8213_4911953609A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLNewPsy : public CLineBase  
{
public:
	CLNewPsy(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLNewPsy();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LNEWPSY_H__34FC2C61_9D56_431A_8213_4911953609A8__INCLUDED_)
