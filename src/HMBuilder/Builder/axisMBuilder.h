
// axisMBuilder.h : axisMBuilder 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CaxisMBuilderApp:
// 이 클래스의 구현에 대해서는 axisMBuilder.cpp을 참조하십시오.
//

class CaxisMBuilderApp : public CWinAppEx
{
public:
	CaxisMBuilderApp();

protected:
	HANDLE	m_hSemaphore;
	class	CMainFrame* m_pMainFrame;
	ULONG_PTR m_gdiplusToken;
private:
	CString	m_strKeywordsLower;
	CString	m_strConstantsLower;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	int	IsConstant(LPCTSTR lpszSymbol);
	int	IsKeyword(LPCTSTR lpszSymbol);

	afx_msg void OnAppAbout();
	afx_msg void OnHelpContents();
	afx_msg void OnHelpAxhelp();
	afx_msg void OnHelpVbhelp();

	DECLARE_MESSAGE_MAP()
};

extern CaxisMBuilderApp theApp;
