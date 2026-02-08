// Grp_Vol.h: interface for the CGrp_Vol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRP_VOL_H__F4C1336A_742A_42C8_B8C6_6692C8B364B7__INCLUDED_)
#define AFX_GRP_VOL_H__F4C1336A_742A_42C8_B8C6_6692C8B364B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Grp_Base.h"

class CGrp_Vol  : public CGrp_Base
{
public:
	CGrp_Vol(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Vol();

	void	DrawGraph(CDC *pDC);
	void	DrawVolume(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift = false);
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelData(int idx);

public:
	CBrush	*m_brUp, *m_brDown;
	CPen	*m_penUp, *m_penDown;
};

#endif // !defined(AFX_GRP_VOL_H__F4C1336A_742A_42C8_B8C6_6692C8B364B7__INCLUDED_)
