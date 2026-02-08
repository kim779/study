// Terminator.cpp : implementation file
//

#include "stdafx.h"
//#include "axissm.h"
#include "Terminator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTerminator

CTerminator::CTerminator()
{
}

CTerminator::~CTerminator()
{
}


BEGIN_MESSAGE_MAP(CTerminator, CStatic)
	//{{AFX_MSG_MAP(CTerminator)
	ON_WM_SIZE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerminator message handlers
const COLORREF clrSilver = RGB(200, 200, 222);
const COLORREF clrGray   = RGB(170, 160, 192);

void CTerminator::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	SetWindowPos(NULL, 0, 0, default_width, default_height, SWP_NOMOVE | SWP_DRAWFRAME);
}

void CTerminator::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	Draw(&dc);
}

void CTerminator::Draw(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	CDC dc;
	CBitmap bmp, *oldBmp;

	dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, default_width, default_height);

	oldBmp = dc.SelectObject(&bmp);

	CPen silverPen, grayPen, *oldPen;

	silverPen.CreatePen(PS_SOLID, 1, clrSilver);
	grayPen.CreatePen(PS_SOLID, 1, clrGray);

	oldPen = dc.SelectObject(&silverPen);
	dc.MoveTo(0, 0);
	dc.LineTo(0, default_height);

	dc.SelectObject(&grayPen);
	dc.MoveTo(1, 0);
	dc.LineTo(1, default_height);

	dc.SelectObject(oldPen);

	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &dc, 0, 0, SRCCOPY);
	dc.SelectObject(oldBmp);
}

#include "usedefine.h"
void AddNewTerminator(CTerminatorList& list, CWnd* parent)
{
#ifdef DF_USESTL_DBAR2

	std::shared_ptr<CTerminator> t = std::make_shared<CTerminator>();
	t->Create("", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, CRect(0, 0, 0, 0), parent);

	list.AddTail(t.get());
#else
	CTerminator* t = new CTerminator;

	t->Create("", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, CRect(0, 0, 0, 0), parent);

	list.AddTail(t);
#endif
	
}

void ClearTerminator(CTerminatorList& list)
{
	while (list.GetCount())
	{
		CTerminator* t;
		t = list.RemoveHead();
		t->DestroyWindow();
		delete t;
	}
}
