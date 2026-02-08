// axScrollView.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "axisDoc.h"
#include "axMisc.h"

#include "axmsg.hxx"
#include "axScrollView.h"
#include "childfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// CAxScrollView
#pragma warning(disable : 26409)
IMPLEMENT_DYNCREATE(CAxScrollView, CScrollView)
#pragma warning(default : 26409)
CAxScrollView::CAxScrollView()
{
	m_resourceHelper = ResourceHelper();
}

CAxScrollView::~CAxScrollView()
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSEV, m_key), 0);
}


BEGIN_MESSAGE_MAP(CAxScrollView, CScrollView)
	//{{AFX_MSG_MAP(CAxScrollView)
	ON_WM_CREATE()
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxScrollView drawing


void CAxScrollView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize	szTotal;
	szTotal.cx = 800;
	szTotal.cy = 600;
	SetScrollSizes(MM_TEXT, szTotal);
}

void CAxScrollView::OnDraw(CDC* pDC)
{
	const CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAxScrollView diagnostics

#ifdef _DEBUG
void CAxScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CAxScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAxScrollView message handlers

int CAxScrollView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

CAxisDoc* CAxScrollView::GetDocument()
{
#ifdef _DEBUG
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAxisDoc)));
	return (CAxisDoc*)m_pDocument;
#endif
	return (CAxisDoc*)m_pDocument;
}

void CAxScrollView::SetSizeView(int cx, int cy, bool nomove)
{
	if (cx == 0 || cy == 0)
		return;

	CSize	szTotal;

	szTotal.cx = cx;
	szTotal.cy = cy;
	SetScrollSizes(MM_TEXT, szTotal);
	ScrollToPosition(CPoint(0, 0));
}

void CAxScrollView::SetInfo(int key)
{
	m_key		= key;
}

void CAxScrollView::DrawEdge(CDC* pDC)
{
	CRect		winRc, rc;
	COLORREF	color{};

	GetWindowRect(winRc);
	winRc -= winRc.TopLeft();
	rc = winRc;

	color = m_resourceHelper->GetColor(ACTFLINE_4);
	pDC->Draw3dRect(rc, color, color);
	rc.DeflateRect(1, 1);

	color = m_resourceHelper->GetColor(ACTFLINE_5);
	pDC->Draw3dRect(rc, color, color);
}

BOOL CAxScrollView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= WS_CLIPCHILDREN;	
	return CScrollView::PreCreateWindow(cs);
}

void CAxScrollView::OnNcPaint() 
{
	CView::OnNcPaint();
	CWindowDC dc(this);
	DrawEdge(&dc);
}
