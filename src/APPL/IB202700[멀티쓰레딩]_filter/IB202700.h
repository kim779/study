// IB202200.h : main header file for the IB202200 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIB202700App
// See IB202700.cpp for the implementation of this class
//

class CIB202700App : public CWinApp
{
public:
	CIB202700App();

	int _httoi(const TCHAR *value);
	auto&	getMemoMap(CWnd* pwnd, CString sCode) 
	{ 
		if (pwnd && !sCode.IsEmpty())
		{
			for (const auto& wnd : _memoWnd)
			{
				if (wnd && wnd != pwnd && wnd->GetSafeHwnd())
					wnd->ShowWindow(SW_HIDE);
			}
			
			_memoWnd.clear();
			_memoWnd.push_back(pwnd);
		}
		
		return _memoMap; 
	}
	void    setMainWnd(CWnd* pWnd) { _pMwnd = (class CMainWnd*)pWnd; }

	const CString& getMemo(CString sCode);

	void    setDelaytime(const int delay){ _delaytime = delay; }
	void	setBlinkColor(COLORREF color) { _blinkColor = color; }
	void    setBlinkType(const int type) { _blinkType = type;  }
	void    setMemoType(const int type)  { _memoType = type;   }
	const int      getBlinkType() const  { return _blinkType;  }
	const int      getMemoType() const   { return _memoType;   }
	const COLORREF getBlinkColor() const { return _blinkColor; }
	const int      getDelay() const { return _delaytime; }

private:
	std::unordered_map<std::string, CString> _memoMap;
	class CMainWnd* _pMwnd{};
	std::vector<CWnd*> _memoWnd;

	int _blinkType = 0;
	COLORREF _blinkColor = RGB(230, 210, 210); // 기본 블링크 색상
	int _memoType = 0;
	int _delaytime = 10;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB202700App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB202700App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


