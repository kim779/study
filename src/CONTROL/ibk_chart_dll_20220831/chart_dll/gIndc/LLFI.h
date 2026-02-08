#if !defined(AFX_LLFI_H__BCC077D1_DC3C_4F9B_B843_A4BC68EE06DA__INCLUDED_)
#define AFX_LLFI_H__BCC077D1_DC3C_4F9B_B843_A4BC68EE06DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLLFI : public CLineBase  
{
public:
	CLLFI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLLFI();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LLFI_H__BCC077D1_DC3C_4F9B_B843_A4BC68EE06DA__INCLUDED_)
