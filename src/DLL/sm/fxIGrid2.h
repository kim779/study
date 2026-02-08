// fxIGrid2.h: interface for the CfxIGrid2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FXIGRID2_H__25A140AE_A220_467A_856C_BB6078EFF1AF__INCLUDED_)
#define AFX_FXIGRID2_H__25A140AE_A220_467A_856C_BB6078EFF1AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fxIGrid.h"

class CfxIGrid2 : public CfxIGrid  
{
public:
	CfxIGrid2();
	virtual ~CfxIGrid2();
protected:
	//{{AFX_MSG(CContGrid)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FXIGRID2_H__25A140AE_A220_467A_856C_BB6078EFF1AF__INCLUDED_)
