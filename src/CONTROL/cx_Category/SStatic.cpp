// SStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Category.h"
#include "SStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSStatic

CSStatic::CSStatic()
{
}

CSStatic::~CSStatic()
{
}


BEGIN_MESSAGE_MAP(CSStatic, CStatic)
	//{{AFX_MSG_MAP(CSStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSStatic message handlers

void CSStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	CString str;
	GetWindowText(str);

//	dc.Draw3dRect(rc, RGB(222,222,222),RGB(211,211,211));
//	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc,RGB(255,255,255));

	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(211,211,211));
	CPen* pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(rc.right-1,rc.top);
	dc.LineTo(rc.right-1,rc.bottom);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();

	CFont *pFont = GetFont(),
		  *pOld = NULL;
	
	if (pFont)
		pOld = dc.SelectObject(pFont);
	

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(255,0,0));
	dc.DrawText(str, rc, DT_VCENTER|DT_CENTER|DT_SINGLELINE| DT_NOPREFIX);

	if (pOld)
		dc.SelectObject(pOld);
}
