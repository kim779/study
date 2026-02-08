// LSonaM.h: interface for the CLSonaM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSONAM_H__1164D35A_41D0_4A4C_943C_B5B8DA660837__INCLUDED_)
#define AFX_LSONAM_H__1164D35A_41D0_4A4C_943C_B5B8DA660837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLSonaM : public CLineBase  
{
public:
	CLSonaM(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLSonaM();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LSONAM_H__1164D35A_41D0_4A4C_943C_B5B8DA660837__INCLUDED_)
