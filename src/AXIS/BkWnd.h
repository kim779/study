#if !defined(AFX_BKWND_H__3D3F28A5_9710_439D_81F9_B511A140432A__INCLUDED_)
#define AFX_BKWND_H__3D3F28A5_9710_439D_81F9_B511A140432A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BkWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBkWnd window
#include "useMaindefine.h"
class CBkWnd : public CWnd
{
// Construction
public:
	CBkWnd();

// Attributes
protected:
	int		m_width{};
	int		m_height{};
public:
	void	SetSize(CSize sz);
	void	SetSize(int width, int height);
	CSize   GetSize();
public:
	CString		m_mapN{};
	int		m_key{};
	int		m_vwTYPE{};

	HCURSOR		m_hCursor{};
	HCURSOR		m_hCursorHand{};

#ifdef DF_USE_CPLUS17
	std::map<CString, std::shared_ptr<CRect>> m_mapCursorMap;
#else
	CMapStringToPtr	m_mapCursorMap;
#endif
	BOOL		m_bTracking{};
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBkWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMapName(bool (*axiscall)(int, WPARAM, LPARAM), CString sMapName);
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	void RemoveAllMouseMap();
	afx_msg LRESULT OnSetCursorMap(WPARAM wParam, LPARAM lParam);
	virtual ~CBkWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBkWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BKWND_H__3D3F28A5_9710_439D_81F9_B511A140432A__INCLUDED_)
