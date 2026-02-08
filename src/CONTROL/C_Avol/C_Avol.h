// C_AVol.h : main header file for the C_AVOL DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"
#include <float.h>
#define UNUSED_VALUE	-9999999.999

/////////////////////////////////////////////////////////////////////////////
// CC_AVolApp
// See C_AVol.cpp for the implementation of this class
//

class CC_AVolApp : public CWinApp
{
public:
	CC_AVolApp();
	virtual ~CC_AVolApp();
	void SetView(CWnd *pView)	{ m_pView = pView; }
	bool CreateImage();
	CImageList* GetCtrlImage() { return m_pImgCtrl.get(); }
	std::shared_ptr<CImageList> getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);
	CFont*	GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd* pView, COLORREF rColor);
	//CImageList*	GetImage(CWnd* pView, CString path, int Width, int Height, UINT flag, COLORREF crMask);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_AVolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_AVolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd		*m_pView;
	std::shared_ptr<CImageList>	m_pImgCtrl;

};