// LForeignRate.h: interface for the CLForeignRate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LFOREIGNRATE_H__16012102_F0AB_4C62_8361_A8B7F378CE78__INCLUDED_)
#define AFX_LFOREIGNRATE_H__16012102_F0AB_4C62_8361_A8B7F378CE78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLForeignRate : public CLineBase  
{
public:
	CLForeignRate(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLForeignRate();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
};

#endif // !defined(AFX_LFOREIGNRATE_H__16012102_F0AB_4C62_8361_A8B7F378CE78__INCLUDED_)
