// BkWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BkWnd.h"

#include "axmsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBkWnd

CBkWnd::CBkWnd()
{
	m_key = -1;
	m_bTracking	= FALSE;
	m_hCursor	= AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));
}

CBkWnd::~CBkWnd()
{
	RemoveAllMouseMap();
	if (m_hCursorHand)
		DestroyCursor(m_hCursorHand);
}


BEGIN_MESSAGE_MAP(CBkWnd, CWnd)
	//{{AFX_MSG_MAP(CBkWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CURSORMAP, OnSetCursorMap)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBkWnd message handlers

int CBkWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CBkWnd::OnSize(UINT nType, int cx, int cy) 
{
	
	CWnd::OnSize(nType, cx, cy);	
}

void CBkWnd::OnDestroy() 
{
	if (m_key != -1)
	{
		AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axCLOSEV, m_key), 0);
	}
	CWnd::OnDestroy();
}

void CBkWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags &= ~SWP_NOZORDER;
	lpwndpos->hwndInsertAfter = HWND_BOTTOM;
	CWnd::OnWindowPosChanged(lpwndpos);
}

void CBkWnd::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	lpwndpos->flags &= ~SWP_NOZORDER;
	lpwndpos->hwndInsertAfter = HWND_BOTTOM;
	CWnd::OnWindowPosChanging(lpwndpos);	
}

void CBkWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{	
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}


LRESULT CBkWnd::OnSetCursorMap(WPARAM wParam, LPARAM lParam)
{
	struct _keyRect {
		RECT rect;
		char *pStr;
	};
	switch (LOWORD(wParam))
	{
	case SCM_SETCURSORMAP:
		{
			const _keyRect *pData = (_keyRect *)lParam;
		
#ifdef DF_USE_CPLUS17
			CRect* pRect{};
			const RECT rc = pData->rect;
			const auto ft = m_mapCursorMap.find(pData->pStr);
			if (ft == m_mapCursorMap.end())
			{
				m_mapCursorMap.emplace(std::make_pair<CString, std::shared_ptr<CRect>>(pData->pStr, std::make_shared<CRect>(rc)));
			}
			else
				ft->second->SetRect(rc.left, rc.top, rc.right, rc.bottom);
#else
			CRect* pRect{};
			if (!m_mapCursorMap.Lookup(pData->pStr, (void*&)pRect))
				pRect = new CRect(pData->rect);
			else
				*pRect = CRect(pData->rect);
			m_mapCursorMap.SetAt(pData->pStr, pRect);
#endif
		}
		break;
	case SCM_DELETECURSORMAP:
		{
			const LPSTR pStr = (LPSTR)lParam;

#ifdef DF_USE_CPLUS17
			const auto ft = m_mapCursorMap.find(pStr);
			if (ft != m_mapCursorMap.end())
				m_mapCursorMap.erase(ft);
#else
			CRect *pRect = NULL;
			if (m_mapCursorMap.Lookup(pStr, (void*&)pRect))
			{
				delete pRect;
				m_mapCursorMap.RemoveKey(pStr);
			}
#endif
		}
		break;
	case SCM_DELETEALLCURSORMAP:
		{
			RemoveAllMouseMap();
		}
		break;
	}
	return 0;
}

void CBkWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

#ifdef DF_USE_CPLUS17
	for (const auto& [key, rc] : m_mapCursorMap)
	{
		if (rc)
		{
			if (rc->PtInRect(point))
			{
				m_hCursor = m_hCursorHand;
				break;
			}
		}
	}

#else
	POSITION pos = m_mapCursorMap.GetStartPosition();

	while (pos)
	{
		CString sKey;
		CRect *pRect;
		m_mapCursorMap.GetNextAssoc(pos, sKey, (void*&)pRect);
		if (pRect)
		{
			if (pRect->PtInRect(point))
			{
				m_hCursor = m_hCursorHand;
				break;
			}
		}
	}
#endif

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}	
	CWnd::OnMouseMove(nFlags, point);
}

BOOL CBkWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest)
	{
	case HTCLIENT:
		if (!CWnd::OnSetCursor(pWnd, nHitTest, message))
		{
			SetCursor(m_hCursor);
			return TRUE;
		}
	case HTHSCROLL:
	case HTVSCROLL:
		break;
	}	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CBkWnd::RemoveAllMouseMap()
{
#ifdef DF_USE_CPLUS17
	m_mapCursorMap.clear();
#else
	POSITION pos = m_mapCursorMap.GetStartPosition();

	while (pos)
	{
		CString sKey;
		CRect *pRect;
		m_mapCursorMap.GetNextAssoc(pos, sKey, (void*&)pRect);
		if (pRect)
			delete pRect;
	}
	m_mapCursorMap.RemoveAll();
#endif
}

LRESULT CBkWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_bTracking = FALSE;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	return 0;
}

void CBkWnd::SetMapName(bool (*axiscall)(int, WPARAM, LPARAM), CString sMapName)
{
	m_mapN = sMapName;
}


void CBkWnd::SetSize(CSize sz)
{
	SetSize(sz.cx, sz.cy);
}

void CBkWnd::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

CSize CBkWnd::GetSize()
{
	return CSize(m_width, m_height);
}