// HogaWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ib201600.h"
#include "HogaWnd.h"
#include "SiseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd

CHogaWnd::CHogaWnd(CWnd* parent, CWnd* mainWnd)
{
	m_pParent = parent;
	m_pMainWnd = mainWnd;
	m_mapKey = -1;

	EnableAutomation();
}

CHogaWnd::~CHogaWnd()
{
//	closeMap();
}

void CHogaWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CHogaWnd, CWnd)
	//{{AFX_MSG_MAP(CHogaWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CHogaWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CHogaWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IHogaWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D3314442-9B73-48C6-8B91-7F4FAA7D3CF0}
static const IID IID_IHogaWnd =
{ 0xd3314442, 0x9b73, 0x48c6, { 0x8b, 0x91, 0x7f, 0x4f, 0xaa, 0x7d, 0x3c, 0xf0 } };

BEGIN_INTERFACE_MAP(CHogaWnd, CWnd)
	INTERFACE_PART(CHogaWnd, IID_IHogaWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd message handlers

CString CHogaWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}


void CHogaWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
//	closeMap();
}


void CHogaWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
// 	CMemDC	mdc(&dc);
// 	CRect	rect;
// 	GetClientRect(rect);
// 	mdc.FillSolidRect(rect, GetColor(181));	
	// Do not call CWnd::OnPaint() for painting messages
}


COLORREF CHogaWnd::GetColor(int color)
{
	if (color & 0x02000000)
		return color;
	
	return SendAxisMessage( MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

LRESULT CHogaWnd::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pMainWnd->SendMessage(WM_USER, wParam, lPAram);
}


int CHogaWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	CString mapname = currMAP;

//	loadMap(mapname);

	return 0;
}
