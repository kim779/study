// fontSizeCombo.cpp : implementation file
//

#include "stdafx.h"
#include "fontSizeCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfontSizeCombo

CfontSizeCombo::CfontSizeCombo()
{
}

CfontSizeCombo::~CfontSizeCombo()
{
}


BEGIN_MESSAGE_MAP(CfontSizeCombo, CComboBox)
	//{{AFX_MSG_MAP(CfontSizeCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfontSizeCombo message handlers

int CfontSizeCombo::AddSize(int point, LONG height)
{
	CString	str;
	int	value, entry, count;

	if (height == 0)
	{
		CDC*	dc = GetDC();
		height = -MulDiv(point, dc->GetDeviceCaps(LOGPIXELSY), 72);
		ReleaseDC(dc);
	}

	str.Format("%d", point);
	count = GetCount();

	// we use positive height values for non-truetype fonts, negitive for true type
	if (height > 0)
	{
		for (entry = 0; entry < count; entry++)
		{
			value = (int)(height - GetHeight(entry));
			if (!value)
				return CB_ERR;
			if (value < 0)
				break;
		}
	}
	else
	{
		for (entry = 0; entry < count; entry++)
		{
			value = (int)(height - GetHeight(entry));
			if (!value)
				return CB_ERR;
			if (value > 0)
				break;
		}
	}

	if (entry == count)
		entry = -1;
	entry = InsertString(entry, str);
	if (entry != CB_ERR)
		SetItemData(entry, (DWORD)height);

	return entry;
}

LONG CfontSizeCombo::GetHeight(int sel)
{
	if (sel == -1)
		sel = GetCurSel();

	if (sel == -1)
	{
		CString	text;

		GetWindowText(text);
		sel = FindString(-1, text);
		if (sel == CB_ERR)
		{
			int	point = atoi(text);

			if (point)
			{
				CDC*	dc = GetDC();
				point = -MulDiv(point, dc->GetDeviceCaps(LOGPIXELSY), 72);
				ReleaseDC(dc);
				return point;
			}
			else
				sel = 0;
		}
	}

	return (LONG) GetItemData(sel);
}