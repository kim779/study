#if !defined(AFX_LFASTSTOCH_H__97A1903A_CB83_4EF8_BBFE_2308E9357728__INCLUDED_)
#define AFX_LFASTSTOCH_H__97A1903A_CB83_4EF8_BBFE_2308E9357728__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLFastStoch : public CLineBase  
{
public:
	CLFastStoch(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLFastStoch();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LFASTSTOCH_H__97A1903A_CB83_4EF8_BBFE_2308E9357728__INCLUDED_)
