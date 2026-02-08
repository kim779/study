// NewListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NewListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewListCtrl

CNewListCtrl::CNewListCtrl()
{
}

CNewListCtrl::~CNewListCtrl()
{
}


BEGIN_MESSAGE_MAP(CNewListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CNewListCtrl)
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomDrawList )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewListCtrl message handlers

void CNewListCtrl::PreSubclassWindow() 
{
	//use our custom CHeaderCtrl as long as there
	//is a headerctrl object to subclass
	CListCtrl::PreSubclassWindow();
}


void CNewListCtrl::OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	static bool bHighlighted = false;
	
    *pResult = CDRF_DODEFAULT;

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		const  int iRow = (int)pLVCD->nmcd.dwItemSpec;
		
		bHighlighted = IsRowHighlighted(m_hWnd, iRow);
		if (bHighlighted)
		{
			pLVCD->clrText   = Axis::GetColor(66);
			pLVCD->clrTextBk = Axis::GetColor(69);
			
			EnableHighlighting(m_hWnd, iRow, false);
		}
		
		*pResult = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		
	}
	else if(CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage)
	{
	if (bHighlighted)
      {
		const  int  iRow = (int)pLVCD->nmcd.dwItemSpec;

        EnableHighlighting(m_hWnd, iRow, true);
      }

      *pResult = CDRF_DODEFAULT;

	}
}

void CNewListCtrl::EnableHighlighting(HWND hWnd, int row, bool bHighlight)
{
  ListView_SetItemState(hWnd, row, bHighlight? 0xff: 0, LVIS_SELECTED);
}

bool CNewListCtrl::IsRowSelected(HWND hWnd, int row)
{
  return ListView_GetItemState(hWnd, row, LVIS_SELECTED) != 0;
}

bool CNewListCtrl::IsRowHighlighted(HWND hWnd, int row)
{
  return IsRowSelected(hWnd, row) /*&& (::GetFocus() == hWnd)*/;
}

void CNewListCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	UpdateWindow();
	CListCtrl::OnNcCalcSize(bCalcValidRects, lpncsp);
}

BOOL CNewListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CNewListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNewListCtrl::Init()
{
	//another way to hide scrollbars
	InitializeFlatSB(m_hWnd);
	FlatSB_EnableScrollBar(m_hWnd, SB_BOTH, ESB_DISABLE_BOTH);

	const CWnd* pParent = GetParent();

	//Create scrollbars at runtime
	//call this to position the scrollbars properly
	PositionScrollBars();
}

void CNewListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CNewListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
	//return CListCtrl::OnEraseBkgnd(pDC);
}


class CMMDC : public CDC 
{
private:
#ifdef DF_USESTL_NLISTCTRL
	std::unique_ptr<CBitmap> m_bitmap;
#else
	CBitmap* m_bitmap;
#endif

	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
public:
	CMMDC(CDC* pDC, const CRect& rcBounds) : CDC()
	{
		CreateCompatibleDC(pDC);
#ifdef DF_USESTL_NLISTCTRL
		m_bitmap = std::make_unique<CBitmap>();
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap.get());
#else
		m_bitmap = new CBitmap;
		m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
		m_oldBitmap = SelectObject(m_bitmap);
#endif
		
		m_pDC = pDC;
		m_rcBounds = rcBounds;
	}
	~CMMDC() 
	{
		m_pDC->BitBlt(m_rcBounds.left, m_rcBounds.top, m_rcBounds.Width(), m_rcBounds.Height(), 
					this, m_rcBounds.left, m_rcBounds.top, SRCCOPY);
		SelectObject(m_oldBitmap);

#ifndef DF_USESTL_NLISTCTRL
		if (m_bitmap != NULL)
			delete m_bitmap;
#endif
	}

	CMMDC* operator->() 
	{
		return this;
	}
};

void CNewListCtrl::OnPaint() 
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CMMDC memDC(&dc, rect);
	
	//funky code to allow use to double buffer
	//the onpaint calls for flicker free drawing
	//of the list items

	CRect headerRect;
	GetDlgItem(0)->GetWindowRect(&headerRect);
	ScreenToClient(&headerRect);
	dc.ExcludeClipRect(&headerRect);
	   
	   
	CRect clip;
	memDC.GetClipBox(&clip);
	memDC.FillSolidRect(clip, Axis::GetColor(66));
	   
	SetTextBkColor(Axis::GetColor(66));
	   
	DefWindowProc(WM_PAINT, (WPARAM)memDC->m_hDC, (LPARAM)0);
}

void CNewListCtrl::PositionScrollBars()
{
	//Thanks goes to mindows for this function
	//he posted on the message forums. He modified
	//it a bit based on the original init function,
	//and now I have modified his version a tiny bit ;)
	//The pParent->ScreenToClient that you did made it
	//possible for me to make the scrollbars position
	//properly based on any dialog size/borders/titlebar etc... :D

	const CWnd* pParent = GetParent();
	
	CRect windowRect;
	GetWindowRect(&windowRect);

	
	int nTitleBarHeight = 0;
	
	if(pParent->GetStyle() & WS_CAPTION)
		nTitleBarHeight = GetSystemMetrics(SM_CYSIZE);
	
	
	int nDialogFrameHeight = 0;
	int nDialogFrameWidth = 0;
	if((pParent->GetStyle() & WS_BORDER))
	{
		nDialogFrameHeight = GetSystemMetrics(SM_CYDLGFRAME);
		nDialogFrameWidth = GetSystemMetrics(SM_CYDLGFRAME);
	}
	
	if(pParent->GetStyle() & WS_THICKFRAME)
	{
		nDialogFrameHeight+=1;
		nDialogFrameWidth+=1;
	}
	
	pParent->ScreenToClient(&windowRect);

	windowRect.top+=nTitleBarHeight+nDialogFrameHeight;
	windowRect.bottom+=nTitleBarHeight+nDialogFrameHeight;
	windowRect.left +=nDialogFrameWidth;
	windowRect.right+=nDialogFrameWidth;
}
