// SStatic.cpp : implementation file
//

#include "stdafx.h"
#include "IB200300.h"
#include "SStatic.h"
#include "MapWnd.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSStatic

BEGIN_MESSAGE_MAP(CSStatic, CStatic)
	//{{AFX_MSG_MAP(CSStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSStatic message handlers

void CSStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	CString str;
	GetWindowText(str);

	dc.Draw3dRect(rc, RGB(244,244,244) ,m_clrEdge);
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc, m_clrBk);

	CFont* pOld = dc.SelectObject( GetFont());
	dc.SetBkMode( TRANSPARENT);
	dc.SetTextColor( m_clrText);
	
	if( m_bRight )
	{
		rc.DeflateRect( 0,0,5,0);
		dc.DrawText( str,rc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE );
	}else
		dc.DrawText( str,rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	dc.SelectObject( pOld);
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
}
