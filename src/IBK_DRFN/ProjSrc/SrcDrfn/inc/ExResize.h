// ExResize.h: interface for the CExResize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXRESIZE_H__CDE1CCE2_6CF5_4FD3_97B3_941A69758671__INCLUDED_)
#define AFX_EXRESIZE_H__CDE1CCE2_6CF5_4FD3_97B3_941A69758671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExResizeRect.h"
#include "ExResizePoint.h"

struct EX_SIZING
{
	UINT			id;		// Control identifier of the window sized.
	EX_RESIZERECT	rrc;	// Size of the window sized.
};

class  CExSizeIcon : public CStatic
{
public:
	
	// Summary: Constructs a CExSizeIcon object
	CExSizeIcon();

protected:

	HCURSOR m_hCursor;		// Handle to the cursor displayed for the size icon
	CBitmap	m_bmSizeIcon;	// Size icon bitmap

	//:Ignore
	//{{AFX_MSG(CXTSizeIcon)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	//:End Ignore

	DECLARE_MESSAGE_MAP()
};

class CExItem
{
public:
	
	// Input:	pwnd - Pointer to the window to be sized or moved.
	//			rrcSizing - Reference to a CExResizeRect object.
	//			rcWindow - Reference to a CRect object.
	//			bDeleteWnd - TRUE if the window is to be deleted.
	// Summary:	Constructs a CExItem object
	CExItem(CWnd* pwnd,const CExResizeRect& rrcSizing,CRect& rcWindow,BOOL bDeleteWnd);

	// Summary: Destroys a CExItem object, handles cleanup and de-allocation
	virtual ~CExItem();
	
	BOOL m_bDeleteWnd;				// TRUE if the window is to be deleted.
	BOOL m_bInitialSize;			// Initial size/move has been completed.
	CWnd* m_pwnd;					// A pointer to the window to be sized or moved.
	CExResizeRect m_rrcSizing;		// Sizing option.
	CExResizeRect m_rrcWindow;		// Last control size.
	CExResizeRect m_rrcInitWindow;  // Initial control size.
};

class CExResize  
{
public:
	CExResize(CWnd* pwnd, const UINT nFlags = 0);
	virtual ~CExResize();

	void SetResize(const UINT nID, EX_RESIZE left, EX_RESIZE top, EX_RESIZE right, EX_RESIZE bottom);
	void SetResize(const UINT nID, const EX_RESIZERECT& rrcSizing);
	void SetResize(const UINT nID, const HWND hWnd, const EX_RESIZERECT& rrcSizing);
	void SetResize(const UINT nID, const HWND hWnd, const EX_RESIZEPOINT& rpTopLeft, const EX_RESIZEPOINT& rpBottomRight);
	void SetResize(const UINT nID, const EX_RESIZEPOINT& rpTopLeft, const EX_RESIZEPOINT& rpBottomRight);
	void SetResize(EX_SIZING arr[]);
	void SetMinSize(CSize& sz);
	void SetMaxSize(CSize& sz);
	void SavePlacement(LPCTSTR pszSection);
	void LoadPlacement(LPCTSTR pszSection);
	void AutoLoadPlacement(LPCTSTR pszSection);
	
protected:
	void SetResize(CWnd* pwnd, const CExResizeRect& rrcSizing, CRect rcWindow);
	BOOL RemoveResize(const UINT nID);
	void RemoveAllControls();
	void Init();
	void Size();
	void Reset();
	void GetMinMaxInfo(MINMAXINFO* pMMI);

protected:
	enum EFlags
	{
		SZ_NOSIZEICON			= 0x01, // Do not add size icon.
		SZ_NOHORISONTAL			= 0x02, // No horizontal resizing.
		SZ_NOVERTICAL			= 0x04, // No vertical resizing.
		SZ_NOMINSIZE			= 0x08, // Do not require a minimum size.
		SZ_NOCLIPCHILDREN		= 0x10, // Do not set clip children style.
		SZ_NOTRANSPARENTGROUP	= 0x20, // Do not set transparent style for group boxes.
	};
	
	BOOL HasFlag(EFlags eFlag);
	void SetFlag(EFlags eFlag);
	BOOL Defer(HDWP&, CExItem*, int dx, int dy);

protected: // helper data

	typedef CArray<CExItem*, CExItem*&> CExItemArray;

	UINT			m_nFlagsXX;		// flags passed from constructor
	CWnd*			m_pwnd;			// the associative relation to the window to be resized
	CRect			m_rcWindow;		// last dialog size
	CRect			m_rcInitWindow; // Initial dialog size
	CSize			m_szMin;		// smallest size allowed
	CSize			m_szMax;		// largest size allowed
	CString			m_strSection;	// section in registry where window placement information is saved.
	CExSizeIcon		m_scSizeIcon;	// size icon window
	CExItemArray	m_arrItems;		// array of controls
};

AFX_INLINE void CExResize::SetMinSize(CSize& sz) { 
	m_szMin = sz; 
}
AFX_INLINE void CExResize::SetMaxSize(CSize& sz) { 
	m_szMax = sz; 
}
AFX_INLINE BOOL CExResize::HasFlag(EFlags eFlag) {
	return (m_nFlagsXX & eFlag) != 0;
}
AFX_INLINE void CExResize::SetResize(const UINT nID, const HWND hWnd, const EX_RESIZEPOINT& rpTopLeft, const EX_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, hWnd, CExResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CExResize::SetResize(const UINT nID, const EX_RESIZERECT& rrcSizing) {
	SetResize(nID,NULL,rrcSizing);
}
AFX_INLINE void CExResize::SetResize(const UINT nID, const EX_RESIZEPOINT& rpTopLeft, const EX_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, CExResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CExResize::SetResize(const UINT nID, EX_RESIZE left, EX_RESIZE top, EX_RESIZE right, EX_RESIZE bottom) {
	SetResize(nID, CExResizeRect(left, top, right, bottom));
}

#endif // !defined(AFX_EXRESIZE_H__CDE1CCE2_6CF5_4FD3_97B3_941A69758671__INCLUDED_)
