// SystemTradingWorkBar.h : main header file for the SYSTEMTRADINGWORKBAR DLL
//

#if !defined(AFX_SYSTEMTRADINGWORKBAR_H__6259DE49_5368_41F6_9D7E_97120DD000D9__INCLUDED_)
#define AFX_SYSTEMTRADINGWORKBAR_H__6259DE49_5368_41F6_9D7E_97120DD000D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../Include_ST/StructDefine.h"

#include "ListCtrlForFlatTab.h"
#include "ListBoxForFlatTab.h"

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingWorkBarApp
// See SystemTradingWorkBar.cpp for the implementation of this class
//

class CTreeCtrlSysTrading;
class CWndTreeCtrlOwner;
class CSystemTradingWorkBarApp : public CWinApp
{
public:
	CSystemTradingWorkBarApp();

	BOOL	CreateWorkspace(CFrameWnd* p_pMainWnd, LPSTR p_lpszRootPath, VT_SCRIPTINFO* p_pVtStartegyInfo, VT_SCRIPTINFO* p_pVtUserFuncInfo);
	void	DeleteWorkspace();

	VT_SCRIPTINFO* GetStrategyInfo() {return m_pVtStrategyInfo;}
	VT_SCRIPTINFO* GetUserFuncInfo() {return m_pVtUserFuncInfo;}

	CFrameWnd*	GetFrameMainWnd() {return m_pMainWnd;}

	CWnd*	GetStategyWnd();
	CWnd*	GetFunctionWnd();
	CWnd*	GetFunctionInStrategyWnd();
	CWnd*	GetOutputWnd();

	void	AddListFunction(CScriptInfomation* pFunction);
	void	AddListStrategy(CScriptInfomation* pStrategy);
	void	AddListFunctionInStrategy(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction);
	void	AddListFunctionInFunction(LPCSTR lpszName, VT_DECLDATA* pVtUsedUserFunction);
	void	InsertFunctionItem(HTREEITEM hItem, CDeclareData* pFunctionItem, CTreeCtrlSysTrading* pTreeFunctionInStrategy);
	void	ChangeCompiledOption(UINT nFileType, LPCSTR szName, UINT nCompiled);
	void	DeleteListFunctionInStrategy(LPCSTR lpszName);
	void	EnsureVisibleFunctionInStrategy(LPCSTR lpszName);
	void	ClearOutputText();
	void	ClearFindText();
	void	ClearMessageText();

	void	CreateTabs();
	BOOL	CreateViews();

	void	SetOutputText(int nMessage, LPCSTR lpszText, CErrorInfomation* pErrorInfo);
	int		GetCompileResultImageIndex(int nMessage);
	void	SetFindText(LPCSTR lpszFindScriptName, UINT nFileType);
	void	SetServerMessage(LPCSTR lpszMessage, UINT nLevel);

	LPSTR	GetRootPath() {return m_lpszRootPath;}
	LPSTR	GetUserPath() {return (LPSTR)(LPCSTR)m_szUserPath;}


private:
	CFrameWnd*					m_pMainWnd;
	LPSTR						m_lpszRootPath;
	CString						m_szUserPath;


	VT_SCRIPTINFO*				m_pVtStrategyInfo;
	VT_SCRIPTINFO*				m_pVtUserFuncInfo;

	CWndTreeCtrlOwner*			m_pWndFunction;
	CWndTreeCtrlOwner*			m_pWndFunctionInStrategy;
	CWndTreeCtrlOwner*			m_pWndStrategy;

//	CTreeCtrlSysTrading*		m_pTreeStrategy;

	CXTExcelTabCtrl*				m_pFlatTabCtrl;

    CXTNoFlickerWnd <CListCtrlForFlatTab> m_sheet1;
    CXTNoFlickerWnd <CListCtrlForFlatTab> m_sheet2;
    CXTNoFlickerWnd <CListBoxForFlatTab> m_sheet3;

	CImageList	m_ImageList;
	CImageList	m_ImageListOutput;

	enum {IMG_TREE = 0, IMG_STS, IMG_STS_LOCK, IMG_STS_CERT, IMG_STS_LOCK_CERT, IMG_STS_DLL, IMG_STS_LOCK_DLL,
						IMG_FUNC, IMG_FUNC_LOCK, IMG_FUNC_CERT, IMG_FUNC_LOCK_CERT, IMG_FUNC_DLL, IMG_FUNC_LOCK_DLL};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingWorkBarApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSystemTradingWorkBarApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CSystemTradingWorkBarApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGWORKBAR_H__6259DE49_5368_41F6_9D7E_97120DD000D9__INCLUDED_)
