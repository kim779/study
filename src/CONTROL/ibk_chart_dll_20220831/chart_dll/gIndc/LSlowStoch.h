#if !defined(AFX_LSLOWSTOCH_H__80D22656_981F_45FA_86E8_125E6C6A606B__INCLUDED_)
#define AFX_LSLOWSTOCH_H__80D22656_981F_45FA_86E8_125E6C6A606B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLSlowStoch : public CLineBase  
{
public:
	CLSlowStoch(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLSlowStoch();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LSLOWSTOCH_H__80D22656_981F_45FA_86E8_125E6C6A606B__INCLUDED_)
