// PreviewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "PreviewWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd

IMPLEMENT_DYNCREATE(CPreviewWnd, CView)

CPreviewWnd::CPreviewWnd()
{
}

CPreviewWnd::~CPreviewWnd()
{
}


BEGIN_MESSAGE_MAP(CPreviewWnd, CView)
	//{{AFX_MSG_MAP(CPreviewWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd drawing

void CPreviewWnd::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd diagnostics

#ifdef _DEBUG
void CPreviewWnd::AssertValid() const
{
	//CView::AssertValid();
}

void CPreviewWnd::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPreviewWnd message handlers

void CPreviewWnd::OnDestroy() 
{
	m_pDropTarget = NULL;
	CView::OnDestroy();	
}
