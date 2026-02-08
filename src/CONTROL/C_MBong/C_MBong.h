#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

class CC_MBongApp : public CWinApp
{
public:
	CC_MBongApp();
	CFont*		GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*		GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush*		GetBrush(CWnd* pView, COLORREF rColor);
	COLORREF	GetColor(CWnd *pView, int color);
	CImageList*	GetImage(CWnd* pView, CString path, int Width, int Height, UINT flag, COLORREF crMask);
	void		SetView(CWnd *pView) { m_pView = pView; }
	//{{AFX_VIRTUAL(CC_MBongApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_MBongApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWnd* m_pView {};
};
