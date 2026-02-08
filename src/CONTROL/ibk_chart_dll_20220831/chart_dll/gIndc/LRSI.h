#if !defined(AFX_LRSI_H__65D522C7_EFCA_4557_A9F0_562E965C7520__INCLUDED_)
#define AFX_LRSI_H__65D522C7_EFCA_4557_A9F0_562E965C7520__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLRSI : public CLineBase  
{
public:
	CLRSI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLRSI();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LRSI_H__65D522C7_EFCA_4557_A9F0_562E965C7520__INCLUDED_)
