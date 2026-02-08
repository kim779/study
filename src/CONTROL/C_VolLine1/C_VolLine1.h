// C_VolLine1.h : main header file for the C_VOLLINE1 DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"

/////////////////////////////////////////////////////////////////////////////
// CC_VolLine1App
// See C_VolLine1.cpp for the implementation of this class
//

class CC_VolLine1App : public CWinApp
{
public:
	CC_VolLine1App();
	virtual ~CC_VolLine1App();
	void	SetView(CWnd *pView)	{ m_pView = pView; }
	bool	CreateImage();
	CImageList* GetCtrlImage() { return m_pImgCtrl; }
	CImageList*	getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);
	CFont*	GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd* pView, COLORREF rColor);
	CImageList*	GetImage(CWnd* pView, CString path, int Width, int Height, UINT flag, COLORREF crMask);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_VolLine1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_VolLine1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd		*m_pView;
	CImageList	*m_pImgCtrl;

};
