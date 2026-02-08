// XTCoolMenu.h : interface for the CXTCoolMenu class.
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
#if !defined(__XTCOOLMENU_H__)
#define __XTCOOLMENU_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CXTCoolMenu
{
public:
	void HookWindow(CWnd*) {}
	void HookWindow(HWND) {}
	void LoadToolbar(UINT) {}
};

#define CXTMenu CMenu
//:End Ignore


#endif // #if !defined(__XTCOOLMENU_H__)