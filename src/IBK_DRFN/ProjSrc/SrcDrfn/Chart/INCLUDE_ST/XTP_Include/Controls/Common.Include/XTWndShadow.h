// XTWndShadow.h: interface for the CXTWndShadow class.
//
// This file is a part of the XTREME TOOLKIT MFC class library.
// ©2004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTWNDSHADOW_H__)
#define __XTWNDSHADOW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//:Ignore

typedef   BOOL  (WINAPI *LayeredProc)  
(
  HWND hwnd,             // handle to layered window
  HDC hdcDst,            // handle to screen DC
  POINT *pptDst,         // new screen position
  SIZE *psize,           // new size of the layered window
  HDC hdcSrc,            // handle to surface DC
  POINT *pptSrc,         // layer position
  COLORREF crKey,        // color key
  BLENDFUNCTION *pblend, // blend function
  DWORD dwFlags          // options
);

class CXTShadowWnd;

typedef CList<CXTShadowWnd*, CXTShadowWnd*> CXTShadowList;

// Uncomment this line to disable alpha shadow. DEBUG ONLY
// #define NOALPHASHADOW

//////////////////////////////////////////////////////////////////////
// CXTShadowsManager is a stand alone internal Toolkit class. It is used to
// manage shadows for popup menus.  

class CXTShadowsManager
{
	class CShadowList : public CXTShadowList
	{
	public:
		void AddShadow(CXTShadowWnd* pShadow);
		void RemoveShadow(CXTShadowWnd* pShadow);
		friend class CXTShadowWnd;
	};

	CXTShadowsManager(void);

public:
	~CXTShadowsManager(void);

	void SetShadow(HWND hWnd, const CRect& rcExclude = CRect(0, 0, 0, 0));
	void SetShadow(CRect rc, HWND hwndHook);

private:

	BOOL AlphaShadow()
	{
	#ifdef NOALPHASHADOW
		return FALSE;		
	#endif

		return (UpdateLayeredWindow != NULL);
	}
	BOOL PseudoShadow()
	{
		if (AlphaShadow())
			return FALSE;

		return !xtAfxData.bUseSolidShadows;
	}
	void DestroyShadow(CXTShadowWnd*);
	CXTShadowWnd* CreateShadow(BOOL bHoriz, HWND hWnd, CRect rcExclude);
	CXTShadowWnd* CreateShadow(BOOL bHoriz, CRect rc, CRect rcExclude, HWND hwndHook);

private:
	
	HMODULE								m_hLib;
	LayeredProc							UpdateLayeredWindow;
	CShadowList							m_lstShadow;
	static CXTShadowsManager			s_managerInstance;
	CList<CXTShadowWnd*, CXTShadowWnd*> m_lstPool;

	friend class CXTShadowsManager* XTShadowsManager();
	friend class CXTShadowWnd;

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTShadowsManager* XTShadowsManager() {
	return &CXTShadowsManager::s_managerInstance;
}

//////////////////////////////////////////////////////////////////////

class CXTShadowWnd : public CWnd
{
	friend class CXTShadowsManager;
	friend class CXTShadowsManager::CShadowList;
	friend class CXTShadowHook;

private:
	CXTShadowWnd();
	BOOL  Create(BOOL bHoriz, CRect rcWindow);
	void LongShadow(CXTShadowsManager::CShadowList* pList);
	BOOL ExcludeRect(CRect rcExclude);

	int OnHookMessage(UINT nMessage, WPARAM& wParam, LPARAM& lParam);
	
	void DrawPseudoShadow(CDC* pDC, CRect& rcClient);
	void ComputePseudoShadow(CDC* pDC, CRect& rcShadow);
	void CreatePseudoShadow();

private:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnParentPosChanged();
	afx_msg void OnTimer(UINT nID);

	DECLARE_MESSAGE_MAP();

private:

	int			m_nWidth;
	BOOL		m_bAutoPosition;
	BOOL		m_bHoriz;
	BOOL		m_bAlphaShadow;
	BOOL		m_bPseudoShadow;
	HWND		m_hwndHook;
	CRect		m_rcExclude;
	CXTMemDC*	m_pShadowDC;
	CXTWndHook* m_pHook;
};



//////////////////////////////////////////////////////////////////////

#endif // !defined(__XTWNDSHADOW_H__)
//:End Ignore
