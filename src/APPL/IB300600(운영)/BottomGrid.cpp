// BottomGrid.cpp: implementation of the CBottomGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB300600.h"
#include "BottomGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// BottomGrid

#define TM_SETMICURSOR 9897

BottomGrid::BottomGrid(CWnd* pParent)
: CGridCtrl(), CAxisExt(pParent)
{

	m_hHandCursor = ::LoadCursor(NULL, IDC_HAND);
	m_hOriCursor = ::LoadCursor(NULL, IDC_ARROW);
	m_mouseDragmode = false;
}

BottomGrid::~BottomGrid()
{
}


BEGIN_MESSAGE_MAP(BottomGrid, CGridCtrl)
	//{{AFX_MSG_MAP(BottomGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// BottomGrid message handlers

void BottomGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CString slog;
	
	CCellID id = GetCellFromPt(point);

slog.Format("[3006]_cursor -->OnLButtonDown<--  point x=[%d] point y=[%d] id.row=[%d] id.col=[%d] m_mouseDragmode=[%d]", point.x, point.y, id.row, id.col, m_mouseDragmode);
OutputDebugString(slog);
	
	if (id.row==0 && ( id.col==1 || id.col==8))
	{	
		CString data = GetItemText(id.row, id.col);
		data.TrimRight();
		if(data.IsEmpty())
		{
		//	CGridCtrl::OnLButtonDown(nFlags, point);
slog.Format("[3006]_cursor  point data=[%s] ", data);
OutputDebugString(slog);
			return;
		}
		
		m_sDragMicheMS.Format("%d", id.col);
		m_mouseDragmode = true;

slog.Format("[3006]_cursor  point data=[%s] m_sDragMicheMS=[%s] m_mouseDragmode=[%d]", data, m_sDragMicheMS, m_mouseDragmode);
OutputDebugString(slog);

		SetCursor(m_hHandCursor);  
		SetCapture();				
		
#ifndef _DEBUG
		SetTimer(TM_SETMICURSOR, 100, NULL);
#endif
		return;
	}
	
	
	CWnd::OnLButtonDown(nFlags, point);
}

void BottomGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString slog;
	CCellID id = GetCellFromPt(point);
	
	KillTimer(TM_SETMICURSOR);
slog.Format("[3006]_cursor -->OnLButtonUp<--  point x=[%d] point y=[%d] id.row=[%d] id.col=[%d] m_mouseDragmode=[%d]", point.x, point.y, id.row, id.col, m_mouseDragmode);
OutputDebugString(slog);

	if(m_mouseDragmode)
	{
		ReleaseCapture();
		m_mouseDragmode = false;  
		ClientToScreen(&point);
		if (WindowFromPoint(point) == this)
		{	
			CGridCtrl::OnLButtonUp(nFlags, point);
			return;
		}
		
		GetParent()->GetParent()->SendMessage(WM_APP_SIG, WP_DRAG_MICHE, (LPARAM)(LPCSTR)m_sDragMicheMS);
	}
	m_mouseDragmode = false;  
	m_sDragMicheMS.Empty();
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL BottomGrid::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	if(m_mouseDragmode == true)
	{
		CString slog;
		slog.Format("[3006] OnSetCursor m_mouseDragmode true", m_mouseDragmode);
		//OutputDebugString(slog);
		SetCursor(m_hHandCursor);
		return TRUE;
	}else
	{
		SetCursor(m_hOriCursor);
		return TRUE;
	}
	*/
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

int BottomGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL BottomGrid::Create( const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle /*= WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE */, DWORD dwExStyle /*= 0*/ )
{
	BOOL bRet = CGridCtrl::Create(rect, parent, nID, dwStyle, dwExStyle);
	AxInit();

	return bRet;
}

void BottomGrid::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString slog;
	switch(nIDEvent)
	{
	case TM_SETMICURSOR:
		{
			if(!m_mouseDragmode)
			{

slog.Format("[3006]_cursor -->OnTimer<-- m_mouseDragmode=[%d]",  m_mouseDragmode);
OutputDebugString(slog);

				SetCursor(m_hOriCursor);
				KillTimer(TM_SETMICURSOR);
			}
			else
			{
//slog.Format("[3006]_cursor -->OnTimer<-- m_mouseDragmode=[%d]",  m_mouseDragmode);
//OutputDebugString(slog);

				SetCursor(m_hHandCursor);
			}
		}
		break;
	}
	CGridCtrl::OnTimer(nIDEvent);
}

void BottomGrid::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_mouseDragmode)
		SetCursor(m_hHandCursor);

	CGridCtrl::OnMouseMove(nFlags, point);
}
