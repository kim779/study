#if !defined(AFX_LABRATIO_H__6F821857_DAB3_4806_BA7A_604F003DEDAC__INCLUDED_)
#define AFX_LABRATIO_H__6F821857_DAB3_4806_BA7A_604F003DEDAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLABRatio : public CLineBase  
{
public:
	CLABRatio(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLABRatio();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LABRATIO_H__6F821857_DAB3_4806_BA7A_604F003DEDAC__INCLUDED_)
