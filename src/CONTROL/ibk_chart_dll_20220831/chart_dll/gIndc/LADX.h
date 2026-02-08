// LADX.h: interface for the CLADX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LADX_H__7FA82CDA_136C_4D5A_866C_96E2CA011634__INCLUDED_)
#define AFX_LADX_H__7FA82CDA_136C_4D5A_866C_96E2CA011634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLADX : public CLineBase  
{
public:
	CLADX(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLADX();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LADX_H__7FA82CDA_136C_4D5A_866C_96E2CA011634__INCLUDED_)
