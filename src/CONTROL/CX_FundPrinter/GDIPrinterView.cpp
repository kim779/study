// GDIpPrinterView.cpp : implementation of the CGDIpPrinterView class
//

#include "stdafx.h"
#include "GDIpPrinter.h"

#include "GDIpPrinterDoc.h"
#include "GDIpPrinterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDIpPrinterView

IMPLEMENT_DYNCREATE(CGDIpPrinterView, CView)

BEGIN_MESSAGE_MAP(CGDIpPrinterView, CView)
	//{{AFX_MSG_MAP(CGDIpPrinterView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIpPrinterView construction/destruction

CGDIpPrinterView::CGDIpPrinterView()
{
	// TODO: add construction code here

}

CGDIpPrinterView::~CGDIpPrinterView()
{
}

BOOL CGDIpPrinterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGDIpPrinterView drawing

void CGDIpPrinterView::OnDraw(CDC* pDC)
{
	CGDIpPrinterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Graphics graphics(pDC->m_hDC);

	CRect rect;
	GetClientRect(rect);

	DrawStuff(pDC->m_hDC, rect, graphics, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CGDIpPrinterView printing

BOOL CGDIpPrinterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDIpPrinterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDIpPrinterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CGDIpPrinterView diagnostics

#ifdef _DEBUG
void CGDIpPrinterView::AssertValid() const
{
	CView::AssertValid();
}

void CGDIpPrinterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDIpPrinterDoc* CGDIpPrinterView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIpPrinterDoc)));
	return (CGDIpPrinterDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGDIpPrinterView message handlers
float CGDIpPrinterView::CreateFontSize(HDC hDC, int points, double dpiRatio)
{
	// This will calculate the font size for the printer that is specified
	// by a point size.
	//
	// if points is:
	//  (-) negative uses height as value for Net Font Height (ie. point size)
	//	(+) positive height is Total Height plus Leading Height!
	ASSERT(hDC);
	
	POINT size;
	int logPixelsY=::GetDeviceCaps(hDC, LOGPIXELSY);
	size.x = size.y = MulDiv(points, logPixelsY, 72);

	// here we scale the font...
	return (float)floor(size.y*dpiRatio);
}


void CGDIpPrinterView::DrawStuff(HDC hDC, CRect rect, Graphics& graphics, double dpiRatio)
{
	// setting stuff to get nicer results...
	graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	//////////////////////////////////////////////////////////////////////////
	// drawing text
	FontFamily  fontFamily(L"Times New Roman");
	// Here get the right unit
	Unit graphicUnit = graphics.GetPageUnit();
	// replace UnitDisplay by Unit Pixel
	if (graphicUnit == UnitDisplay)
		graphicUnit=UnitPixel;
	// creating font with dpiratio
	Font        font(&fontFamily, CreateFontSize(hDC, 24, dpiRatio), FontStyleRegular, graphicUnit);

	PointF      pointF((REAL)(rect.left+50), (REAL)(rect.top+100));
	SolidBrush  solidBrush(Color(255, 255, 0, 0));


	graphics.DrawString(L"This is a text test", -1, &font, pointF, &solidBrush);


	////////////////////////////////////////////////////////////////////////////////////
	// Draw rectangle...
	// drawing stuff...
	Pen pen(Color(255, 0, 0, 255));
	graphics.DrawRectangle(&pen, 
		(REAL)(rect.left+25), (REAL)(rect.top+50),
		(REAL)(400), (REAL)(200));
}


void CGDIpPrinterView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	REAL dpiRatio;
	Graphics graphics(pDC->m_hDC);

	CPrintDialog* pPrintDialog = pInfo->m_pPD;

	if (!pPrintDialog || pInfo->m_bPreview)
		dpiRatio = 1;
	else
	{
		// the dpi of printer is enclosed in DEVMODE structure,
		DEVMODE* pDev=pPrintDialog->GetDevMode();
		// getting dpi ratio between 300dpi and current printer dpi
		dpiRatio=300.0f/pDev->dmPrintQuality;
		// deallocating memory for pDev
		VERIFY(GlobalUnlock(pDev));
	}

	if (!pInfo->m_bPreview)
	{
		graphics.SetPageUnit(UnitDocument);
	}

	graphics.SetPageScale(dpiRatio);

	DrawStuff(pDC->m_hDC, pInfo->m_rectDraw, graphics, dpiRatio);
}
