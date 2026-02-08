#if !defined(AFX_LSONA_H__B32836CB_D5F7_4AA8_A356_6BF23D104FDE__INCLUDED_)
#define AFX_LSONA_H__B32836CB_D5F7_4AA8_A356_6BF23D104FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLSona : public CLineBase  
{
public:
	CLSona(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLSona();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LSONA_H__B32836CB_D5F7_4AA8_A356_6BF23D104FDE__INCLUDED_)
