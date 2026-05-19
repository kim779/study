
// axisBuilder.h : axisBuilder 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CaxisBuilderApp:
// 이 클래스의 구현에 대해서는 axisBuilder.cpp을 참조하십시오.
//

class CaxisBuilderApp : public CWinAppEx
{
public:
	CaxisBuilderApp();


protected:
	HANDLE	m_hSemaphore;
	class	CMainFrame* m_pMainFrame;

private:
	CString	m_strKeywordsLower;
	CString	m_strConstantsLower;

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	COleTemplateServer m_server;
		// 문서 만들기에 대한 서버 개체입니다.

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void	SetKey(CString sKey = _T("AXIS"));
	int	IsConstant(LPCTSTR lpszSymbol);
	int	IsKeyword(LPCTSTR lpszSymbol);

	afx_msg void OnAppAbout();
	afx_msg void OnHelpContents();
	afx_msg void OnHelpAxhelp();
	afx_msg void OnHelpVbhelp();

	DECLARE_MESSAGE_MAP()
};

extern CaxisBuilderApp theApp;
