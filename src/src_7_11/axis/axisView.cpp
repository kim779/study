// axisView.cpp : implementation of the CAxisView class
//

#include "stdafx.h"
#include "axis.h"

#include "axisDoc.h"
#include "axisView.h"

#include "axmsg.hxx"
#include "axMisc.h"

#include "../h/axisvar.h"
#include "../h/axisfire.h"
#include <Winuser.h>
#include <Windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxisView

IMPLEMENT_DYNCREATE(CAxisView, CView)

BEGIN_MESSAGE_MAP(CAxisView, CView)
	//{{AFX_MSG_MAP(CAxisView)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisView construction/destruction

CAxisView::CAxisView()
{
	m_key		= 0;
	m_axisres	= NULL;
}

CAxisView::~CAxisView()
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSEV, m_key), 0);
}

BOOL CAxisView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_CLIPCHILDREN;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CAxisView drawing

void CAxisView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CAxisView printing

BOOL CAxisView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAxisView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAxisView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CAxisView diagnostics

#ifdef _DEBUG
void CAxisView::AssertValid() const
{
	CView::AssertValid();
}

void CAxisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAxisDoc* CAxisView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAxisDoc)));
	return (CAxisDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxisView message handlers

void CAxisView::SetInfo(int key, class _axisRes* axisres)
{
	m_key		= key;
	m_axisres	= axisres;
}

void CAxisView::DrawEdge(CDC* pDC)
{
	CRect		winRc, rc;
	COLORREF	color;

	GetWindowRect(winRc);
	winRc -= winRc.TopLeft();
	rc = winRc;
	if (m_axisres)
	{
//		color = m_axisres->GetColor(ACTFLINE_4);
		color =  FRAMELINE;	
		pDC->Draw3dRect(rc, color, color);
		rc.DeflateRect(1, 1);
	//	color = m_axisres->GetColor(0);		
		pDC->Draw3dRect(rc, color, color);
	}

}

void CAxisView::OnNcPaint() 
{
	CView::OnNcPaint();
	CWindowDC dc(this);
	DrawEdge(&dc);
}
