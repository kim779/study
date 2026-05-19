// fmGroup.cpp : implementation file
//

#include "stdafx.h"
#include "amGroup.h"
#include "amform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamGroup

IMPLEMENT_DYNCREATE(CamGroup, CamBase)

CamGroup::CamGroup()
{
	EnableAutomation();
}

CamGroup::CamGroup(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
//	m_form->properties |= PR_TRANSPARENT;
}

CamGroup::~CamGroup()
{
}


void CamGroup::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamGroup, CamBase)
	//{{AFX_MSG_MAP(CamGroup)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamGroup, CamBase)
	//{{AFX_DISPATCH_MAP(CamGroup)
	DISP_PROPERTY_EX(CamGroup, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CamGroup, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CamGroup, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmGroup to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0AE7F991-14C7-43C3-99B1-4675DAC7482B}
static const IID IID_IfmGroup =
{ 0xae7f991, 0x14c7, 0x43c3, { 0x99, 0xb1, 0x46, 0x75, 0xda, 0xc7, 0x48, 0x2b } };

BEGIN_INTERFACE_MAP(CamGroup, CamBase)
	INTERFACE_PART(CamGroup, IID_IfmGroup, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamGroup message handlers

void CamGroup::_Refresh() 
{
	Refresh();
}

BSTR CamGroup::_getText() 
{
	CString strResult;

	strResult = m_strR;
	return strResult.AllocSysString();
}

void CamGroup::_setText(LPCTSTR lpszNewValue) 
{
	m_strR = lpszNewValue;
	invalidateRect(&m_pRc, false);
}

BOOL CamGroup::_getVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamGroup::_setVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

void CamGroup::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());
	CRect	cRc;
	CSize	tSz;
	Point	pt[8];

	cRc = m_pRc;
	cRc.top = m_pRc.top + (int)(m_fHeight / 2);
		
	pt[0] = Point(cRc.left+1, cRc.top);
	pt[1] = Point(cRc.left+1, cRc.bottom);
	pt[2] = Point(cRc.left, cRc.bottom);
	pt[3] = Point(cRc.right, cRc.bottom);
	pt[4] = Point(cRc.right-1, cRc.top);
	pt[5] = Point(cRc.right-1, cRc.bottom);
	

	g.DrawLines(setgPen(), pt, 4);
	g.DrawLine(setgPen(), pt[4] ,pt[5]);


	pt[0] = Point(cRc.left, cRc.top);
	pt[1] = Point(cRc.left, cRc.bottom - 1);
	pt[2] = Point(cRc.left, cRc.bottom - 1);
	pt[3] = Point(cRc.right - 2, cRc.bottom - 1) ;
	pt[4] = Point(cRc.right-2, cRc.top);
	pt[5] = Point(cRc.right-2, cRc.bottom - 1);


	g.DrawLines(setgPen(clEDGE), pt, 4);
	g.DrawLine(setgPen(clEDGE), pt[4], pt[5]);

	if (!m_strR.IsEmpty())
	{
		cRc.right = cRc.left + 5;
		// white
		g.DrawLine(setgPen(clYWHITE), Point(cRc.left+1, cRc.top+1), Point(cRc.right, cRc.top+1));
		// gray
		g.DrawLine(setgPen(clEDGE), Point(cRc.left, cRc.top), Point(cRc.right, cRc.top));

		StringFormat sf;
		RectF rt;
		sf.SetAlignment(StringAlignmentNear);
		sf.SetLineAlignment(StringAlignmentCenter);
		g.DrawString(m_strR.AllocSysString(), -1, setgFont(), PointF((REAL)cRc.right + 2, (REAL)m_pRc.top),/* &sf, */setgFBrush());
		g.MeasureString(m_strR.AllocSysString(), -1, setgFont(), PointF((REAL)cRc.right + 2, (REAL)m_pRc.top), /*&sf, */&rt);


		cRc.left = cRc.right + /*tSz.cx*/(LONG)rt.Width;
		cRc.right = m_pRc.right;

		// white
		g.DrawLine(setgPen(clYWHITE), Point(cRc.left, cRc.top+1), Point(cRc.right-3, cRc.top+1));
		// gray
		g.DrawLine(setgPen(clEDGE), Point(cRc.left, cRc.top), Point(cRc.right-2, cRc.top));

	}
	else
	{
		// white
		g.DrawLine(setgPen(clYWHITE), Point(cRc.left+1, cRc.top+1), Point(cRc.right-3, cRc.top+1));
		// gray
		g.DrawLine(setgPen(clEDGE), Point(cRc.left, cRc.top), Point(cRc.right-2, cRc.top));
	}
}

int CamGroup::IsPointInRect(CPoint pt)
{
	return P_OUT;
}
