// ContGrid.h: interface for the CContGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTGRID_H__99A5EBA3_73DA_4906_BB7B_8F3ACDCB7580__INCLUDED_)
#define AFX_CONTGRID_H__99A5EBA3_73DA_4906_BB7B_8F3ACDCB7580__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../dll/sm/fxIGrid.h"

#define COL_COLOR	0
#define COL_ORDNO	2

class CContGrid : public CfxIGrid  
{
public:
	COLORREF GetCellFgColor(int nRow, int nCol);
	CContGrid();
	virtual ~CContGrid();
// Generated message map functions
protected:
	//{{AFX_MSG(CContGrid)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CONTGRID_H__99A5EBA3_73DA_4906_BB7B_8F3ACDCB7580__INCLUDED_)
