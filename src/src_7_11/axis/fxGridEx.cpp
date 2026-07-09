// fxGridEx.cpp : implementation file
//

#include "stdafx.h"
#include "fxGridEx.h"

#include "../control/fx_grid/memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfxGridEx

CfxGridEx::CfxGridEx(LOGFONT* logfont, CWnd* parent) : CfxGrid(logfont)
{
	m_parent = parent;
}

CfxGridEx::~CfxGridEx()
{
}

BEGIN_MESSAGE_MAP(CfxGridEx, CfxGrid)
	//{{AFX_MSG_MAP(CfxGridEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CfxGridEx message handlers

void CfxGridEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
//	CAxMemDC	MemDC(&dc);
//	CfxGrid::OnDraw(&MemDC);
}
