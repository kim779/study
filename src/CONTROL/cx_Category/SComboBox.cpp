// SComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Category.h"
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
	CString	str;
	int	nWidth = 0;
	const	int	nScrollWidth = 0;//::GetSystemMetrics(SM_CXVSCROLL);
	const	int	nNumEntries = GetCount();

	CClientDC dc(this);
	const	int	nSave = dc.SaveDC();

	dc.SelectObject(GetFont());
	for (int ii = 0, nLength = 0; ii < nNumEntries; ii++)
	{
	    GetLBText(ii, str);
	    nLength = dc.GetTextExtent(str).cx + nScrollWidth;
	    nWidth = max(nWidth, nLength);
	}
	
	// Add margin space to the calculations
	nWidth += dc.GetTextExtent("0").cx;

	dc.RestoreDC(nSave);
	SetDroppedWidth(nWidth);
}
