// LBandWidth.h: interface for the CLBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LBANDWIDTH_H__84D0383C_B8AC_476B_B042_6A2CCC5906FF__INCLUDED_)
#define AFX_LBANDWIDTH_H__84D0383C_B8AC_476B_B042_6A2CCC5906FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLBandWidth : public CLineBase  
{
public:
	CLBandWidth(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLBandWidth();

protected:
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LBANDWIDTH_H__84D0383C_B8AC_476B_B042_6A2CCC5906FF__INCLUDED_)
