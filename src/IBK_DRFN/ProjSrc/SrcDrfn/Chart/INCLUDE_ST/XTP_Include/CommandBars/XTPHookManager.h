// XTPHookManager.h : interface for the CXTPHookManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©2004 Codejock Software, All Rights Reserved.
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
#if !defined(__XTPHOOKMANAGER_H__)
#define __XTPHOOKMANAGER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

#include "XTPCommandBarsDefines.h"

//:Ignore

// Internally used class

class _XTP_EXT_CLASS CXTPHookManager
{
	friend class CXTPHookManager* XTPHookManager();
	CXTPHookManager(void);
	
	class _XTP_EXT_CLASS CXTPHookSink : public CArray<IHookAble*, IHookAble*>
	{		
		friend class CXTPHookManager;
		
		CXTPHookSink(HWND hWnd);
		~CXTPHookSink();

		int Find(IHookAble* pItem)
		{
			for (int i = 0; i < GetSize(); i++)
			{
				if (pItem == GetAt(i)) 
					return  i;
			}
			return -1;
		}
		void RemoveHook(IHookAble* pHook)
		{
			int nIndex = Find(pHook);			
			if (nIndex != -1)
			{
				RemoveAt(nIndex);
				if (GetSize() == 0)
				{
					s_managerInstance.m_mapHooks.RemoveKey(m_hWnd);
					delete this;
				}
			}
		}

		BOOL OnHookMessage(HWND hWnd, UINT nMessage, WPARAM& wParam, LPARAM& lParam, LRESULT& lResult);
		WNDPROC m_pOldWndProc;
		HWND m_hWnd;
	};

	friend class CXTPHookSink;
	typedef CMap<HWND, HWND, CXTPHookSink*, CXTPHookSink*> CXTPHookMap;
	
public:
	~CXTPHookManager(void);

	void SetHook(HWND hWnd, IHookAble* pHook );
	void RemoveHook(HWND hWnd, IHookAble* pHook );
	void RemoveAll(IHookAble* pHook);

// Keyboard Hooks

	void HookKeyboard(IHookAble* pHook);
	void UnhookKeyboard(IHookAble* pHook);
	BOOL IsKeyboardHooked() { return m_lstKeyboardHooks.GetCount() > 0; }
	BOOL ProcessKeyboardHooks(UINT, WPARAM wParam, LPARAM lParam = 0);


private:
	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void RemoveAll();
	void RemoveAll(HWND hWnd);
	
	CXTPHookSink* Lookup(HWND hWnd)
	{
		CXTPHookSink* pSink;

		if (m_mapHooks.Lookup(hWnd, pSink))
			return pSink;
        return NULL;
	}

private:
	static CXTPHookManager s_managerInstance;

	CList<IHookAble*, IHookAble*> m_lstKeyboardHooks; 
	CMap<HWND, HWND, CXTPHookSink*, CXTPHookSink*> m_mapHooks;

};

//:End Ignore

AFX_INLINE CXTPHookManager* XTPHookManager() { 
	return &CXTPHookManager::s_managerInstance; 
}

#endif //#if !defined(__XTPHOOKMANAGER_H__)