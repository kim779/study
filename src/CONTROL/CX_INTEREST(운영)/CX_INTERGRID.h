// CX_INTERGRID.h : main header file for the CX_INTERGRID DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"

/////////////////////////////////////////////////////////////////////////////
// CCX_INTERGRIDApp
// See CX_INTERGRID.cpp for the implementation of this class
//

class CCX_INTERGRIDApp : public CWinApp
{
public:
	CCX_INTERGRIDApp();
	int _httoi(const TCHAR *value);
	
	auto&	getMemoMap(CWnd* pwnd, CString sCode) 
	{ 
		if (pwnd && !sCode.IsEmpty())
		{
			if (_memoMap.find(sCode.GetString()) != _memoMap.end())
				_memoWnd.clear();
			_memoWnd[pwnd] = true;
		}
		return _memoMap; 
	}
	
	void    setMainWnd(CWnd* pWnd) { _pMwnd = (class CMainWnd*)pWnd; }

	const CString& getMemo(CString sCode);

	void	setBlinkColor(COLORREF color) { _blinkColor = color == 0 ? RGB(230, 255, 230) : color; }
	void    setBlinkType(const int type) { _blinkType = type;  }
	void    setMemoType(const int type)  { _memoType = type;   }
	const int      getBlinkType() const  { return _blinkType;  }
	const int      getMemoType() const   { return _memoType;   }
	const COLORREF getBlinkColor() const { return _blinkColor; }

private:
	std::unordered_map<std::string, CString> _memoMap;
	class CMainWnd* _pMwnd{};
	std::map<CWnd*, bool> _memoWnd;

	int _blinkType = 1; // 0: off, 1: cell, 2: row
	COLORREF _blinkColor = RGB(230, 255, 230);; // 기본 블링크 색상
	int _memoType = 1;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_INTERGRIDApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_INTERGRIDApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};