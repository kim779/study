// IB410100.h : main header file for the IB410100 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <Afxtempl.h>
#include "resource.h"		// main symbols

#define	DELWND(w)	{ if(w) {w->DestroyWindow(); delete w; w=NULL;} }
#define	DELIMG(w)	{ if(w) {w->DeleteImageList(); delete w; w=NULL;} }
#define	DELOBJ(w)	{ if(w) {w->DeleteObject(); delete w; w=NULL;} }
#define	DELMEM(w)	{ if(w) {delete w; w=NULL;} }

/////////////////////////////////////////////////////////////////////////////
// CIB410100App
// See IB410100.cpp for the implementation of this class
//

class CIB410100App : public CWinApp
{
public:
	CIB410100App();

public:
	CFont*		GetFont(CWnd *pView, int point, int bold = 0, CString name = _T("±¼¸²"), bool italic = false);
	CPen*		GetPen(CWnd* pView, COLORREF clr, int style = PS_SOLID, int width = 1);
	CBrush*		GetBrush(CWnd* pView, COLORREF rColor);
	CBitmap*	GetBitmap(CWnd *pView, CString path);
	COLORREF	GetIndexColor(CWnd *pView, int index);
	LOGFONT		GetLogFont(int Size, bool bBold = false, CString fName = _T("±¼¸²"));
	CString		Parser(CString &srcstr, CString substr);
	CString		FillSpace(CString input, int len);
	CString		CommaModify(CString string);
	CString		StringModify(int data, int kind = 0);
	CString		StringModify(double data, int dPoint);
	double		BuhoModify(CString sData);
	double		getOptionStrike(CString opCode);
	CString		JisuToStrike(double ajisu);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB410100App)
	public:
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB410100App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


