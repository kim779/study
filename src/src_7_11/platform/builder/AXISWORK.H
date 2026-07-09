// axisWork.h : main header file for the AXISWORK application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include <afxtempl.h>

// CAxBuilderApp:
// 이 클래스의 구현에 대해서는 AxBuilder.cpp을 참조하십시오.
//

class CWorkApp : public CWinApp
{
public:
	CWorkApp();

	// variables
public:
	CWnd		m_wndMDIClient;
	CWnd*		m_pWndCurrentChild;
	int		m_nGap;


	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();


	CMap	<int , int, CWnd*, CWnd*> m_LockMap;
protected:
private:
	CString	m_strKeywordsLower, m_strConstantsLower;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HMENU	m_hMDIMenu;
	HACCEL	m_hMDIAccel;
	HANDLE	m_hSemaphore;

public:
	CMDIChildWnd* GetNextMDIChildWnd();
	void	setRegistryKey(CString sKey = "AXIS");
	int IsConstant(LPCTSTR lpszSymbol);
	int IsKeyword(LPCTSTR lpszSymbol);
	void releaseLock();
	int setLock(CWnd *pWnd);
	bool UnRegisterCtrl(CString name);
	bool RegisterCtrl(CString name);
	//{{AFX_MSG(CWorkApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppHelp();
	afx_msg void OnUpdateAppHelp(CCmdUI* pCmdUI);
	afx_msg void OnAppAxhelp();
	afx_msg void OnAppVbhelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////

