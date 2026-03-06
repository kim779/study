#include "stdafx.h"
#include "axMenu.h"
#include "intGrid.h"

const int base = 200;

void CAxMenu::DrawItem(LPDRAWITEMSTRUCT lpdraw)
{
        CDC* pDC = CDC::FromHandle(lpdraw->hDC);

        CRect rc(lpdraw->rcItem);
	rc.DeflateRect(2, 2);
        pDC->FillSolidRect(rc, __menuClr[lpdraw->itemID - base]);

        if (lpdraw->itemState & ODS_SELECTED)
                pDC->DrawFocusRect(rc);

      //rc.DeflateRect(1, 1);
      //pDC->DrawText(CString((char*)lpdraw->itemData), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
	
void CAxMenu::MeasureItem(LPMEASUREITEMSTRUCT lp)
{
       lp->itemHeight = 24;
       lp->itemWidth = 100;
}