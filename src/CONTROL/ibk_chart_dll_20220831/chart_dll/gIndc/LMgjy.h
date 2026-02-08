// LMgjy.h: interface for the CLMgjy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LMGJY_H__946C46EB_43B5_4A33_95BA_8ECE85157C0B__INCLUDED_)
#define AFX_LMGJY_H__946C46EB_43B5_4A33_95BA_8ECE85157C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLMgjy : public CLineBase  
{
public:
	CLMgjy(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLMgjy();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);

};

#endif // !defined(AFX_LMGJY_H__946C46EB_43B5_4A33_95BA_8ECE85157C0B__INCLUDED_)
