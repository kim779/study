// SComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "SComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSComboBox

CSComboBox::CSComboBox()
{
}

CSComboBox::~CSComboBox()
{
}


BEGIN_MESSAGE_MAP(CSComboBox, CComboBox)
	//{{AFX_MSG_MAP(CSComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSComboBox message handlers

void CSComboBox::OnDropdown() 
{
    // Reset the dropped width
    const int nNumEntries = GetCount();
    int nWidth = 0;
    CString str;

    CClientDC dc(this);
    const  int nSave = dc.SaveDC();
    dc.SelectObject(GetFont());

    const  int nScrollWidth = 0;//::GetSystemMetrics(SM_CXVSCROLL);
    for (int i = 0; i < nNumEntries; i++)
    {
        GetLBText(i, str);
        const int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
        nWidth = max(nWidth, nLength);
    }
    
    // Add margin space to the calculations
    nWidth += dc.GetTextExtent("0").cx;

    dc.RestoreDC(nSave);
    SetDroppedWidth(nWidth);

}
