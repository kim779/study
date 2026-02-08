#if !defined(AFX_LMFI_H__6408F428_2CD5_4E04_91F1_B4C132837254__INCLUDED_)
#define AFX_LMFI_H__6408F428_2CD5_4E04_91F1_B4C132837254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLMFI : public CLineBase  
{
public:
	CLMFI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLMFI();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LMFI_H__6408F428_2CD5_4E04_91F1_B4C132837254__INCLUDED_)
