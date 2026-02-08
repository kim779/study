#if !defined(AFX_LDMI_H__8E3B195C_B388_4A9A_84F5_A38597E82088__INCLUDED_)
#define AFX_LDMI_H__8E3B195C_B388_4A9A_84F5_A38597E82088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLDMI : public CLineBase  
{
public:
	CLDMI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLDMI();
	
protected:
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LDMI_H__8E3B195C_B388_4A9A_84F5_A38597E82088__INCLUDED_)
