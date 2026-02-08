// GridCellPassword.cpp: implementation of the CGridCellNumeric class.
//
// Written by Andrew Truckle [ajtruckle@wsatkins.co.uk]
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCellPassword.h"
#include "../InPlaceEdit.h"
#include "../GridCtrl.h"

IMPLEMENT_DYNCREATE(CGridCellPassword, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Create a control to do the editing
BOOL CGridCellPassword::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    m_bEditing = TRUE;
    
    // CInPlaceEdit auto-deletes itself
    m_pEditWnd = new CInPlaceEdit(GetGrid(), rect, /*GetStyle() |*/ ES_PASSWORD, nID, nRow, nCol,
		GetText(), nChar);
	
    return TRUE;
}

// Cancel the editing.
void CGridCellPassword::EndEdit()
{
    if (m_pEditWnd)
        ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
}
