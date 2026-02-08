#if !defined(AFX_LMAO_H__D3AEE918_C95A_4214_8A79_9EDFB93AA835__INCLUDED_)
#define AFX_LMAO_H__D3AEE918_C95A_4214_8A79_9EDFB93AA835__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLMAO : public CLineBase  
{
public:
	CLMAO(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLMAO();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);	
};

#endif // !defined(AFX_LMAO_H__D3AEE918_C95A_4214_8A79_9EDFB93AA835__INCLUDED_)
