// C_Total.h : main header file for the C_TOTAL DLL
//

#if !defined(AFX_C_TOTAL_H__4F4E0975_EE2B_431F_A33B_D28192D28A9A__INCLUDED_)
#define AFX_C_TOTAL_H__4F4E0975_EE2B_431F_A33B_D28192D28A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/axisgwin.h"
#include "../../chart_dll/gCom/defInfo.h"

enum { imgDate = 1, imgNTime, imgIOpt, imgCtrl, imgCtrl2};

#define COLOR_PAL_BG	PALETTERGB(27, 79, 129)
#define COLOR_PAL_FG	PALETTERGB(255, 145, 2)
#define	COLOR_WHITE		PALETTERGB(255, 255, 255)	

/////////////////////////////////////////////////////////////////////////////
// CC_TotalApp
// See C_Total.cpp for the implementation of this class
//

class CC_TotalApp : public CWinApp
{
public:
	CC_TotalApp();
	virtual ~CC_TotalApp();

private:
	CString		m_strRoot;
	CString		m_strUserID;
	CString		m_strUserName;
	CString		m_strTabDir;
	CString		m_strUserDir;
	CString		m_strGexDir;

	CImageList*	m_pImgDate;
	CImageList*	m_pImgNTime;
	CImageList*	m_pImgIOpt;
	CImageList*	m_pImgCtrl;
	CImageList*	m_pImgCtrl2;

public:
	HINSTANCE	m_hGMainLib;
	HINSTANCE	m_hGDlgLib;

public:
	CWnd*	Create(CWnd* pParent, struct _param *param);
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd *pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd *pView, COLORREF rColor);

	CImageList*	GetImage(int imgType, CString path);
	CString		GetImagePath(CWnd* pView, CString imgName);
	CImageList*	getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);

	CString GetRoot()	{ return m_strRoot; }
	CString	GetUID()	{ return m_strUserID; }
	CString	GetUName()	{ return m_strUserName; }
	CString	GetTabPath()	{ return m_strTabDir; }
	CString	GetUserPath()	{ return m_strUserDir; }
	CString	GetGexPath()	{ return m_strGexDir; }

	// 2008.09.26
	struct _param	m_param;
	void	CopyParam(_param *pDest, _param *pSource);
	void	ReMakeFile(CWnd *pParent);


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_TotalApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_TotalApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_C_TOTAL_H__4F4E0975_EE2B_431F_A33B_D28192D28A9A__INCLUDED_)
