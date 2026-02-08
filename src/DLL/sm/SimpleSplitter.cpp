#include "stdafx.h"
#include "SimpleSplitter.h"
#include "smdefine.h"

#define FULL_SIZE	32768
//const COLORREF clrLine   = RGB(40, 117, 197);
const COLORREF clrLine   = RGB(40, 117, 197);
//const COLORREF clrBack   = RGB( 1,  78, 134);
const COLORREF clrBack	= RGB(180,201,209);


inline int MulDivRound(int x, int mul, int div)
{
	return (x * mul + div / 2) / div;
}


BEGIN_MESSAGE_MAP(CSimpleSplitter, CWnd)
	//{{AFX_MSG_MAP(CSimpleSplitter)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_NCCREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSimpleSplitter::CSimpleSplitter(int nPanes, UINT nOrientation, int nMinSize, int nBarThickness):
	m_nPanes(nPanes),
	m_nOrientation(nOrientation),
	m_nMinSize(nMinSize), 
	m_nBarThickness(nBarThickness)
{
	m_down		= false;
	m_bkIMG		= NULL;
	m_barColor	= ::GetSysColor(COLOR_BTNHILIGHT);

	ASSERT(nPanes > 0);
	ASSERT(nMinSize >= 0);
	ASSERT(nBarThickness >= 0);
	ASSERT(nOrientation == SSP_HORZ || nOrientation == SSP_VERT);

#ifdef DF_USESTL_SMSPLITTER
	for (int ii = 0; ii < m_nPanes; ii++)
	{
		m_pane.emplace_back(nullptr);
		m_size.emplace_back(0);
		m_orig.emplace_back(0);
	}
	m_orig.emplace_back(0);
#else
	m_pane = new CWnd * [m_nPanes];
	m_size = new int[m_nPanes];
	m_orig = new int[m_nPanes + 1];
	::ZeroMemory(m_pane, m_nPanes * sizeof(CWnd*));
#endif
	
	
	int total = 0;
	for (int i = 0, total = 0; i < m_nPanes - 1; i++)			// default, set equal size to all panes
	{
		m_size[i] = (FULL_SIZE + m_nPanes / 2) / m_nPanes;
		total += m_size[i];
	}
	m_size[m_nPanes - 1] = FULL_SIZE - total;

	
}

CSimpleSplitter::~CSimpleSplitter()
{
#ifndef DF_USESTL_SMSPLITTER
	delete[] m_pane;
	delete[] m_size;
	delete[] m_orig;
#endif
}


BOOL CSimpleSplitter::Create(CWnd* pParent, COLORREF barColor, UINT nID)
{
	HCURSOR crsResize{};
	CRect rcOuter;

	ASSERT(pParent);
	pParent->GetClientRect(rcOuter);
	crsResize = ::LoadCursor(0, MAKEINTRESOURCE(m_nOrientation == SSP_HORZ ? AFX_IDC_HSPLITBAR : AFX_IDC_VSPLITBAR));
	if (!crsResize)	crsResize = ::LoadCursor(0, m_nOrientation == SSP_HORZ ? IDC_SIZEWE : IDC_SIZENS);
	
	m_barColor = barColor;
	const DWORD	dwStyle = WS_CHILD | WS_VISIBLE;//| WS_CLIPCHILDREN| WS_CLIPSIBLINGS;
	return CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, crsResize, NULL, NULL), NULL, dwStyle, rcOuter, pParent, nID, NULL);
}

BOOL CSimpleSplitter::Create(CWnd* pParent, CBitmap* bkImage, UINT nID)
{
	HCURSOR crsResize{};
	CRect rcOuter;

	ASSERT(pParent);
	pParent->GetClientRect(rcOuter);
	rcOuter.DeflateRect(0, HORZFIT_TOP, 0, HORZFIT_BOTTOM);
	crsResize = ::LoadCursor(0, MAKEINTRESOURCE(m_nOrientation == SSP_HORZ ? AFX_IDC_HSPLITBAR : AFX_IDC_VSPLITBAR));
	if (!crsResize)	crsResize = ::LoadCursor(0, m_nOrientation == SSP_HORZ ? IDC_SIZEWE : IDC_SIZENS);
	
	//m_bkIMG = bkImage;
	m_bkIMG = Axis::GetBitmap("TK_INDEX_BACK2");
	const DWORD	dwStyle = WS_CHILD | WS_VISIBLE| WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	return CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS, crsResize, NULL, NULL), NULL, dwStyle, rcOuter, pParent, nID, NULL);
}

BOOL CSimpleSplitter::CreatePane(int nIndex, CWnd* pPaneWnd, DWORD dwStyle, DWORD dwExStyle, LPCTSTR lpszClassName)
{
	CRect rcPane;

	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	m_pane[nIndex] = pPaneWnd;
	dwStyle |= WS_CHILD | WS_VISIBLE;
	GetPaneRect(nIndex, rcPane);
	return pPaneWnd->CreateEx(dwExStyle, lpszClassName, NULL, dwStyle, rcPane, this, AFX_IDW_PANE_FIRST + nIndex);
}

void CSimpleSplitter::SetPane(int nIndex, CWnd* pPaneWnd)
{
	CRect rcPane;

	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	ASSERT(pPaneWnd);
	ASSERT(pPaneWnd->m_hWnd);

	m_pane[nIndex] = pPaneWnd;
	GetPaneRect(nIndex, rcPane);
	pPaneWnd->MoveWindow(rcPane, false);
}

CWnd* CSimpleSplitter::GetPane(int nIndex) const
{
	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	return m_pane[nIndex];
}

void CSimpleSplitter::SetActivePane(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex < m_nPanes));
	m_pane[nIndex]->SetFocus();
}

CWnd* CSimpleSplitter::GetActivePane(int* pIndex) const
{
	for (int i = 0; i < m_nPanes; i++)
		if (m_pane[i]->GetFocus())
		{
			*pIndex = i;
			return m_pane[i];
		}
	return NULL;
}

void CSimpleSplitter::SetPaneSizes(const int* sizes)
{
	int i{}, total = 0, total_in = 0;
	
	for (i = 0; i < m_nPanes; i++)
	{
		ASSERT(sizes[i] >= 0);
		total += sizes[i];
	}
	for (i = 0; i < m_nPanes - 1; i++)
	{
		m_size[i] = MulDivRound(sizes[i], FULL_SIZE, total);
		total_in += m_size[i];
	}
	m_size[m_nPanes - 1] = FULL_SIZE - total_in;
	RecalcLayout();
	ResizePanes();
}

void CSimpleSplitter::GetPaneRect(int nIndex, CRect& rcPane) const
{
	ASSERT(nIndex >= 0 && nIndex < m_nPanes);
	GetClientRect(&rcPane);
	if (m_nOrientation == SSP_HORZ)
	{
		rcPane.left = m_orig[nIndex];
		rcPane.right = m_orig[nIndex + 1] - m_nBarThickness;
	}
	else
	{
		rcPane.top = m_orig[nIndex];
		rcPane.bottom = m_orig[nIndex + 1] - m_nBarThickness;
	}
}

void CSimpleSplitter::GetBarRect(int nIndex, CRect& rcBar) const
{
	ASSERT(nIndex > 0 && nIndex < m_nPanes);
	GetClientRect(&rcBar);
	if (m_nOrientation == SSP_HORZ)
	{
		rcBar.left = m_orig[nIndex];
		rcBar.right = m_orig[nIndex + 1] - m_nBarThickness;
	}
	else
	{
		rcBar.top = m_orig[nIndex];
		rcBar.bottom = m_orig[nIndex + 1] - m_nBarThickness;
	}
}

// CSimpleSplitter protected

void CSimpleSplitter::RecalcLayout()
{
	int i{}, size_sum{}, remain{}, remain_new = 0;
	bool bGrow = true;
	CRect rcOuter;
	
	rcOuter = m_rcClient;
	size_sum = m_nOrientation == SSP_HORZ ? rcOuter.Width() : rcOuter.Height();
	size_sum -= (m_nPanes - 1) * m_nBarThickness;

	while (bGrow)									// adjust sizes on the beginning
	{												// and while we have growed something
		bGrow = false;
		remain = remain_new = FULL_SIZE;
		for (i = 0; i < m_nPanes; i++)				// grow small panes to minimal size
			if (MulDivRound(m_size[i], size_sum, FULL_SIZE) <= m_nMinSize)
			{
				remain -= m_size[i];
				if (MulDivRound(m_size[i], size_sum, FULL_SIZE) < m_nMinSize)
				{
					if (m_nMinSize > size_sum)
						m_size[i] = FULL_SIZE;
					else
						m_size[i] = MulDivRound(m_nMinSize, FULL_SIZE, size_sum);
					bGrow = true;
				}
				remain_new -= m_size[i];
			}
		if (remain_new <= 0)						// if there isn't place to all panes
		{											// set the minimal size to the leftmost/topmost
			remain = FULL_SIZE;						// and set zero size to the remainimg
			for (i = 0; i < m_nPanes; i++)
			{
				if (size_sum == 0)
					m_size[i] = 0;
				else
					m_size[i] = MulDivRound(m_nMinSize, FULL_SIZE, size_sum);
				if (m_size[i] > remain)
					m_size[i] = remain;
				remain -= m_size[i];
			}
			break;
		}
		if (remain_new != FULL_SIZE)				// adjust other pane sizes, if we have growed some
			for (i = 0; i < m_nPanes; i++)
				if (MulDivRound(m_size[i], size_sum, FULL_SIZE) != m_nMinSize)
					m_size[i] = MulDivRound(m_size[i], remain_new, remain);
	}

	m_orig[0] = 0;									// calculate positions (in pixels) from relative sizes
	for (i = 0; i < m_nPanes - 1; i++)
		m_orig[i + 1] = m_orig[i] + MulDivRound(m_size[i], size_sum, FULL_SIZE) + m_nBarThickness;
	m_orig[m_nPanes] = size_sum + m_nBarThickness * m_nPanes;
}

void CSimpleSplitter::ResizePanes()
{
	CRect rcOuter;

	rcOuter = m_rcClient; 
	
	if (m_nOrientation == SSP_HORZ)
	{
		for (int ii = 0; ii < m_nPanes; ii++)
		{
			if (!m_pane[ii])	continue;
			m_pane[ii]->MoveWindow(m_orig[ii], 0,
				m_orig[ii + 1] - m_orig[ii] - m_nBarThickness, rcOuter.Height());
		}
	}
	else
	{
		for (int ii = 0; ii < m_nPanes; ii++)
		{
			if (!m_pane[ii])	continue;
			m_pane[ii]->MoveWindow(0, m_orig[ii],
				rcOuter.Width(), m_orig[ii + 1] - m_orig[ii] - m_nBarThickness);
		}
	}
}

void CSimpleSplitter::InvertTracker()
{
	CDC*	pDC = GetDC();
	CBrush*	pBrush = CDC::GetHalftoneBrush();
	HBRUSH	hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);

	if (m_nOrientation == SSP_HORZ)
	{
		const int	x = m_nTracker - m_nBarThickness + 1;
		const int	y = 1;
		const int	cx = m_nBarThickness-2;
		const int	cy = m_nTrackerLength - 2;
		pDC->PatBlt(x, y, cx, cy, PATINVERT);
		//pDC->PatBlt(m_nTracker - m_nBarThickness, 0, m_nBarThickness, m_nTrackerLength, PATINVERT);
	}
	else	pDC->PatBlt(0, m_nTracker - m_nBarThickness, m_nTrackerLength, m_nBarThickness, PATINVERT);
	if (hOldBrush)	SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);
}

// CSimpleSplitter messages

void CSimpleSplitter::OnPaint() 
{
	CPaintDC dc(this);

	draw(&dc);
}

void CSimpleSplitter::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (cx > 0 && cy > 0)
		m_rcClient.SetRect(0, 0, cx, cy);
	RecalcLayout();
	ResizePanes();
}

void CSimpleSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcClient;
	const int mouse_pos = m_nOrientation == SSP_HORZ ? point.x : point.y;

	SetCapture();
	for (m_nTrackIndex = 1; (m_nTrackIndex < m_nPanes && m_orig[m_nTrackIndex] < mouse_pos); m_nTrackIndex++);
	m_nTracker = m_orig[m_nTrackIndex];
	m_nTrackerMouseOffset = mouse_pos - m_nTracker;
	GetClientRect(&rcClient);
	m_nTrackerLength = m_nOrientation == SSP_HORZ ? rcClient.Height() : rcClient.Width();
	InvertTracker();

	m_down = true;
	ModifyStyle(WS_CLIPCHILDREN, 0);
}

void CSimpleSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ModifyStyle(0, WS_CLIPCHILDREN);
	if (!m_down)	return;
	m_down = false;
//	if (GetCapture() != this) 
//		return;

	CRect rcOuter, invaldateRc;
	int size_sum{};

	GetClientRect(rcOuter);
	size_sum = m_nOrientation == SSP_HORZ ? rcOuter.Width() : rcOuter.Height();
	size_sum -= (m_nPanes - 1) * m_nBarThickness;

	InvertTracker();
	m_orig[m_nTrackIndex] = m_nTracker;
	m_size[m_nTrackIndex - 1] = MulDivRound(m_orig[m_nTrackIndex] - m_orig[m_nTrackIndex - 1] - m_nBarThickness, FULL_SIZE, size_sum);
	m_size[m_nTrackIndex]     = MulDivRound(m_orig[m_nTrackIndex + 1] - m_orig[m_nTrackIndex] - m_nBarThickness, FULL_SIZE, size_sum);
	ResizePanes();

	if (m_nOrientation == SSP_HORZ)
	{
		invaldateRc.left	= m_nTracker - m_nBarThickness;
		invaldateRc.top		= rcOuter.top;
		invaldateRc.right	= invaldateRc.left + m_nBarThickness;
		invaldateRc.bottom	= rcOuter.bottom;
	}
	else	invaldateRc.CopyRect(&rcOuter);

	InvalidateRect(invaldateRc, FALSE);
}

void CSimpleSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetCapture() != this) 
		return;
	InvertTracker();
	m_nTracker = (m_nOrientation == SSP_HORZ ? point.x : point.y) - m_nTrackerMouseOffset;
	if (m_nTracker > m_orig[m_nTrackIndex + 1] - m_nBarThickness - m_nMinSize)
		m_nTracker = m_orig[m_nTrackIndex + 1] - m_nBarThickness - m_nMinSize;
	else if (m_nTracker < m_orig[m_nTrackIndex - 1] + m_nBarThickness + m_nMinSize)
		m_nTracker = m_orig[m_nTrackIndex - 1] + m_nBarThickness + m_nMinSize;
	InvertTracker();
}

BOOL CSimpleSplitter::OnNcCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (!CWnd::OnNcCreate(lpCreateStruct))
		return FALSE;

	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);
	pParent->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_DRAWFRAME);
	return TRUE;
}

void CSimpleSplitter::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags |= SWP_NOCOPYBITS;
	CWnd::OnWindowPosChanging(lpwndpos);
}

void CSimpleSplitter::draw(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());
		CBitmap* oldbuffBitmap = memDC.SelectObject(&bitmap);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			drawBackGround(&memDC);
			drawSplitter(&memDC);
			
			pDC->BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CSimpleSplitter::drawBackGround(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	
// 	if (m_bkIMG && m_bkIMG->GetSafeHandle())
// 	{
// 		if (memDC.CreateCompatibleDC(pDC))
// 		{
// 			CBitmap* oldbuffBitmap = memDC.SelectObject(m_bkIMG);
// 			if (oldbuffBitmap != (CBitmap *) 0)
// 			{
// 				BITMAP	 bm;
// 				m_bkIMG->GetBitmap(&bm);
// 				pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
// 						&memDC, 0, HORZFIT_TOP, bm.bmWidth,
// 						bm.bmHeight-(HORZFIT_TOP+HORZFIT_BOTTOM), SRCCOPY);
// 				memDC.SelectObject(oldbuffBitmap);
// 			}
// 			memDC.DeleteDC();
// 		}
// 	}
// 	else
	{
		pDC->FillSolidRect(cRc, clrBack);
	}
	
	//pDC->FillSolidRect(cRc, clrBack);
}

void CSimpleSplitter::drawSplitter(CDC* pDC)
{
	CRect	rc, cRc;
	CPoint	cpt;
	GetClientRect(cRc);
	if (m_nOrientation == SSP_HORZ)
	{
		// draw gripper
		CPen linePen;

		linePen.CreatePen(PS_SOLID, 1, clrLine);
		pDC->SelectObject(&linePen);
		
		
		for (int ii = 1; ii < m_nPanes; ii++)
		{
			rc.left = m_orig[ii] - m_nBarThickness;
			rc.top = cRc.top;
			rc.right = rc.left + m_nBarThickness;
			rc.bottom = cRc.bottom;
//			dc.FillSolidRect(rc, m_barColor);

			cpt = rc.CenterPoint();
			rc.left  = cpt.x ;
			rc.right = cpt.x + 1;
			//rc.DeflateRect(1, 0);
			//pDC->Draw3dRect(rc, GRIP_LT, GRIP_RB);
			pDC->MoveTo(rc.left-2, 0+5);
			pDC->LineTo(rc.left-2, cRc.Height()-6);
			pDC->MoveTo(rc.left+0, 0+2);
			pDC->LineTo(rc.left+0, cRc.Height()-2);
			pDC->MoveTo(rc.left+2, 0+5);
			pDC->LineTo(rc.left+2, cRc.Height()-6);
			/*
			pDC->FillSolidRect(rc.left, rc.top + 1, 1, rc.Height() - 2, GRIP_LT);
			pDC->FillSolidRect(rc.left + 1, rc.top, 1, 1, GRIP_LT);
			pDC->FillSolidRect(rc.left + 2, rc.top + 1, 1, rc.Height() -2, GRIP_RB);
			pDC->FillSolidRect(rc.left + 1, rc.bottom - 1, 1, 1, GRIP_RB);
			*/
		}
		
		/*
		CPen linePen;

		linePen.CreatePen(PS_SOLID, 1, clrLine);

		//oldPen = dc.SelectObject(&linePen);
		pDC->Rectangle(cRc);
		pDC->SelectObject(&linePen);
		pDC->MoveTo(0, 0);
		pDC->LineTo(0, cRc.Height());


		pDC->MoveTo(2, 0);
		pDC->LineTo(2, cRc.Height());

		pDC->MoveTo(4, 0);
		pDC->LineTo(4, cRc.Height());
		*/
	}
	else
	{
		for (int ii = 1; ii < m_nPanes; ii++)
			pDC->FillSolidRect(cRc.left, m_orig[ii] - m_nBarThickness, cRc.Width(), m_nBarThickness, m_barColor);
	}
}

void CSimpleSplitter::DefaultSize()
{
	const int total = 0;
	for (int i = 0, total = 0; i < m_nPanes - 1; i++)			// default, set equal size to all panes
	{
		m_size[i] = (FULL_SIZE + m_nPanes / 2) / m_nPanes;
		total += m_size[i];
	}
	m_size[m_nPanes - 1] = FULL_SIZE - total;
	RecalcLayout();
	ResizePanes();
}

void CSimpleSplitter::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	DefaultSize();
	CWnd::OnLButtonDblClk(nFlags, point);
}
