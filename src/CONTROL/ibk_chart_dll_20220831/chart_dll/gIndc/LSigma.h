#if !defined(AFX_LSIGMA_H__E624235F_07F7_4E9C_A408_C1C47D0D85E2__INCLUDED_)
#define AFX_LSIGMA_H__E624235F_07F7_4E9C_A408_C1C47D0D85E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLSigma : public CLineBase  
{
public:
	CLSigma(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLSigma();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LSIGMA_H__E624235F_07F7_4E9C_A408_C1C47D0D85E2__INCLUDED_)
