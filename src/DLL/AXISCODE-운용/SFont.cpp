// SFont.cpp : implementation file
//

#include "stdafx.h"
#include "SFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSFont

CSFont::CSFont()
{
}

CSFont::~CSFont()
{
	DeleteObject();
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSFont, CFont)
	//{{AFX_MSG_MAP(CSFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSFont member functions

BOOL CSFont::CreateFont(int nHeight, LPCTSTR lpszFacename, int nWeight)
{
	const BOOL bflag = CFont::CreateFont( nHeight, 0, 0, 0, nWeight,
							FALSE, FALSE, FALSE, DEFAULT_CHARSET, 
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
							DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
							lpszFacename );
	return bflag;
}
