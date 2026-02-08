#if !defined(AFX_AXISGMAIN_H__99F50740_C343_4EF2_BE8C_AF980FFEA9A6__INCLUDED_)
#define AFX_AXISGMAIN_H__99F50740_C343_4EF2_BE8C_AF980FFEA9A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "afxtempl.h"
#include "../../h/axisgwin.h"
#include "../../h/axisgenv.h"
#include "../../h/axisgobj.h"

class CAxisGMainApp : public CWinApp
{
public:
	CAxisGMainApp();

	//{{AFX_VIRTUAL(CAxisGMainApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisGMainApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold = 0, bool italic = false);
	CPen*	GetPen(CWnd *pView, int style, int width, COLORREF clr);
	CBrush*	GetBrush(CWnd *pView, COLORREF rColor);
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_AXISGMAIN_H__99F50740_C343_4EF2_BE8C_AF980FFEA9A6__INCLUDED_)
