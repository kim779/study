// XTPShadowsManager.h : interface for the CXTPShadowsManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ?004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTPSHADOWSMANAGER_H__)
#define __XTPSHADOWSMANAGER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

#include "XTPCommandBarsDefines.h"

//:Ignore

typedef BOOL(WINAPI *LayeredProc)  
(
	HWND hwnd,             // Handle to layered window
	HDC hdcDst,            // Handle to screen DC
	POINT *pptDst,         // New screen position
	SIZE *psize,           // New size of the layered window
	HDC hdcSrc,            // Handle to surface DC
	POINT *pptSrc,         // Layer position
	COLORREF crKey,        // Color key
	BLENDFUNCTION *pblend, // Blend function
	DWORD dwFlags          // Options
);

//#define NOALPHASHADOW

class CXTPControlPopup;
class CXTPCommandBar;


// Internally used class.

class _XTP_EXT_CLASS CXTPShadowsManager
{
	class CShadowList;

	class CShadowWnd : public CWnd, public IHookAble
	{
	public:
		CShadowWnd();
		BOOL  Create(BOOL bHoriz, CRect rcWindow);
		void LongShadow(CShadowList* pList);
		BOOL ExcludeRect(CRect rcExclude);
	private:
		virtual int OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);
		inline UINT Factor(int& nRed, int& nGreen, int& nBlue, double dFactor);

	private:
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);	
		afx_msg UINT OnNcHitTest(CPoint point);


		DECLARE_MESSAGE_MAP();

	private:
		static BOOL m_bAlphaShadow;
		
		BOOL m_bHoriz;
		BOOL m_bOfficeLikeShadow;
		CXTPCommandBar* m_pCommandBar;

		friend class CXTPShadowsManager;
	};

	class _XTP_EXT_CLASS CShadowList : public CList<CShadowWnd*, CShadowWnd*>
	{
	public:
		void AddShadow(CShadowWnd* pShadow)
		{
			pShadow->LongShadow(this);
			AddTail(pShadow);
		}
		void RemoveShadow(CShadowWnd* pShadow)
		{
			POSITION pos = Find(pShadow);
			ASSERT(pos);
			RemoveAt(pos);
		}
	};

	CXTPShadowsManager(void);

public:

	// Summary: Destroys a CXTPShadowsManager object, handles cleanup and de-allocation.
	~CXTPShadowsManager(void);

	// Returns: TRUE if alpha shadow available; otherwise returns FALSE
	// Summary: Check the system alpha shadow ability.
	BOOL AlphaShadow()
	{
		#ifdef NOALPHASHADOW
			return FALSE;		
		#endif
		
		return (UpdateLayeredWindow != NULL);
	}
	
	// Input:	pCommandBar - Points to a CXTPCommandBar object
	//			rcExclude - Excluded rectangle.
	// Summary: Sets the command bar  shadow.
	void SetShadow(CXTPCommandBar* pCommandBar, const CRect& rcExclude = CRect(0, 0, 0, 0));
	
	// Input:	pControl - Points to a CXTPControlPopup object
	// Summary: Sets the control shadow.
	void SetShadow(CXTPControlPopup* pControl);
	
	
	// Input:	pCommandBar - Points to a CXTPCommandBar object
	// Summary: Removes shadows for the command bar.
	void RemoveShadow(CXTPCommandBar* pCommandBar);

private:
	void DestroyShadow(CShadowWnd*);
	CShadowWnd* CreateShadow(BOOL bHoriz, CRect rc, CRect rcExclude, CXTPCommandBar* pCommandBar);

public:
	COLORREF m_clrFactor;		// Darkness factor.

private:
	
	LayeredProc UpdateLayeredWindow;
	HMODULE m_hLib;

	CShadowList m_lstShadow;

	static CXTPShadowsManager s_managerInstance;

	
	friend class CShadowWnd;
	friend class CXTPShadowsManager* XTPShadowsManager();
};

//:End Ignore

AFX_INLINE CXTPShadowsManager* XTPShadowsManager() { 
	return &CXTPShadowsManager::s_managerInstance; 
}

#endif //#if !defined(__XTPSHADOWSMANAGER_H__)