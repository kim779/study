#pragma once
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"
#include <float.h>
#define UNUSED_VALUE	-9999999.999
// 데이터 헤더 
// struct _cdMultiline

// 데이터 구조
// struct _multi_lineD 이고 size는 라인의 개수에 의해 10씩 증가한다.

struct _multi_lineD
{
	char	index[14];
	char	data[1][10];
};

struct _multi_line
{
	struct _cgIndex	index;
	double	data[1];
};

/////////////////////////////////////////////////////////////////////////////
// CC_MultiLineApp
// See C_MultiLine.cpp for the implementation of this class
//

class CC_MultiLineApp : public CWinApp
{
public:
	CC_MultiLineApp();
	virtual ~CC_MultiLineApp();
	void SetView(CWnd *pView) { m_pView = pView; }
	bool CreateImage();
	CImageList* GetCtrlImage() { return m_pImgCtrl; }
	CImageList* getImage(CString path, int Width, int Height, UINT flag, COLORREF crMask);
	CFont*	GetFont(CWnd* pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd* pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd* pView, COLORREF rColor);
	CImageList*	GetImage(CWnd* pView, CString path, int Width, int Height, UINT flag, COLORREF crMask);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CC_MultiLineApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CC_MultiLineApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWnd		*m_pView;
	CImageList	*m_pImgCtrl;
};
