#pragma once

#include "../GridCell.h"

class CGridCellPassword : public CGridCell
{
    DECLARE_DYNCREATE(CGridCellPassword)

public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
};
