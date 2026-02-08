#if !defined(AFX_GRIDCELLPASSWORD_H__F1443470_D611_45E0_8D60_6A3541208532__INCLUDED_)
#define AFX_GRIDCELLPASSWORD_H__F1443470_D611_45E0_8D60_6A3541208532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GridCell.h"

class CGridCellPassword : public CGridCell
{
    DECLARE_DYNCREATE(CGridCellPassword)

public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELLPASSWORD_H__F1443470_D611_45E0_8D60_6A3541208532__INCLUDED_)
