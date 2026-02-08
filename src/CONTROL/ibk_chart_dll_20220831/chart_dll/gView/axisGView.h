#if !defined(AFX_AXISGVIEW_H__45166C9A_6406_4046_9F4C_ED6426599CBB__INCLUDED_)
#define AFX_AXISGVIEW_H__45166C9A_6406_4046_9F4C_ED6426599CBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "../../h/axisgenv.h"
#include "../../h/axisgwin.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#include <afxtempl.h>

enum { fileNot = 0, fileOrg, fileUser, fileDelete };

#define WM_AXUSER		WM_USER+10000
#define	WM_AXDROPITEM	WM_AXUSER+1
#define	WM_AXSETCTRL	WM_AXUSER+2
#define	WM_AXGETCTRL	WM_AXUSER+3
#define	WM_AXDELKIND	WM_AXUSER+4
#define	WM_AXRELOAD		WM_AXUSER+5

#define	WM_AXRMOUSE		WM_AXUSER+6
#define	WM_AXADDUSER	WM_AXUSER+7
#define	WM_AXDELUSER	WM_AXUSER+8
#define	WM_AXGETUGRP	WM_AXUSER+9
#define	WM_AXADDGRP		WM_AXUSER+10

#define WM_AXITEMCLICK  WM_AXUSER+11


class CAxisGViewApp : public CWinApp
{
public:
	CAxisGViewApp();
	virtual ~CAxisGViewApp();

	//{{AFX_VIRTUAL(CAxisGViewApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisGViewApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CWnd*	Create(CWnd* pParent, char* pcDllName);
	CString GetRoot()		const { return m_root; }
	CString	GetUID()		const { return m_uid; }
	CString	GetUName()		const { return m_uname; }
	CString	GetTabPath()	const { return m_tabDir; }
	CString	GetUserPath()	const { return m_userDir; }
	CString	GetGexPath()	const { return m_gexDir; }
	CString GetImgPath()	const { return m_imgDir; }
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd *pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd *pView, COLORREF rColor);

private:
	CString		m_root;
	CString		m_uid;
	CString		m_uname;
	CString		m_tabDir;
	CString		m_userDir;
	CString		m_gexDir;
	CString		m_imgDir;
public:
	UINT	MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = COMPANYNAME, UINT button = MB_OK | MB_ICONINFORMATION);
	UINT    MessageBoxF(LPCTSTR lpszFormat, ...);
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_AXISGVIEW_H__45166C9A_6406_4046_9F4C_ED6426599CBB__INCLUDED_)
