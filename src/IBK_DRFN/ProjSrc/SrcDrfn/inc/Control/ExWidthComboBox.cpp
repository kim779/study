// MyComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ExWidthComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExWidthComboBox

CExWidthComboBox::CExWidthComboBox()
{
}

CExWidthComboBox::~CExWidthComboBox()
{
}

/////////////////////////////////////////////////////////////////////////////
// Implementation

void CExWidthComboBox::RecalcDropWidth()
{
    // Reset the dropped width
    int nNumEntries = GetCount();
    int nWidth = 0;
    CString str;

    CClientDC dc(this);
    int nSave = dc.SaveDC();
    dc.SelectObject(GetFont());

    int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
    for (int i = 0; i < nNumEntries; i++)
    {
        GetLBText(i, str);
        int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
        nWidth = max(nWidth, nLength);
    }
    
    // Add margin space to the calculations
    nWidth += dc.GetTextExtent("0").cx;

    dc.RestoreDC(nSave);
    SetDroppedWidth(nWidth);
}

BEGIN_MESSAGE_MAP(CExWidthComboBox, CComboBox)
	//{{AFX_MSG_MAP(CExWidthComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExWidthComboBox message handlers

void CExWidthComboBox::OnDropdown() 
{
    RecalcDropWidth();
}
