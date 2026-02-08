#include "stdafx.h"
#include "listHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExListHeader::CExListHeader()
{
}

CExListHeader::~CExListHeader()
{
}

BEGIN_MESSAGE_MAP(CExListHeader, CHeaderCtrl)
	//{{AFX_MSG_MAP(CExListHeader)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExListHeader::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rc,rcHeader;
	int	nTotW = 0;
	const	int	nCount = GetItemCount();
	TCHAR	szText[100]{};
	HDITEM	hditem{};
	CFont	Font;

	Font.CreatePointFont(90, "±¼¸²");
	GetClientRect(&rcHeader);
	for (int ii = 0, iItem = 0; ii < nCount; ii++)
	{
		iItem = OrderToIndex(ii);

		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT;//|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rc));
//		rc.bottom=18;
		nTotW += rc.Width();
		dc.FillSolidRect(rc , RGB(182,182,182));
		dc.Draw3dRect(rc , RGB(222,222,222),RGB(106,106,106));
		
		if (rc.Width() > 0 && hditem.mask & HDI_TEXT && hditem.fmt & HDF_STRING)
		{
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(20,20,20));
			// always center column headers
			CFont* pOld= dc.SelectObject(&Font);
			dc.DrawText(hditem.pszText, rc, 
				DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
			dc.SelectObject(pOld);
		}
	}
	Font.DeleteObject();

	if (rcHeader.right > nTotW)
	{
		dc.FillSolidRect(nTotW,rcHeader.top,rcHeader.right, rcHeader.bottom,RGB(200,200,255));
		dc.Draw3dRect(nTotW,rcHeader.top,rcHeader.right, rcHeader.bottom ,  RGB(180,180,244),RGB(180,180,233));
	}
}

LRESULT CExListHeader::OnLayout(WPARAM wp, LPARAM lp)
{
	const	LRESULT	lr = CHeaderCtrl::DefWindowProcA(HDM_LAYOUT, 0, lp);
	HDLAYOUT& hdl = *(HDLAYOUT*)lp;
	hdl.pwpos->cy = 16;
	hdl.prc->top  = 16;

	return lr;
}

