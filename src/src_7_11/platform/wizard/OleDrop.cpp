// OleDrop.cpp: implementation of the COleDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wizard.h"
#include "OleDrop.h"
#include "Works.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleDrop::COleDrop()
{

}

COleDrop::~COleDrop()
{

}

BEGIN_MESSAGE_MAP(COleDrop, COleDropTarget)
	//{{AFX_MSG_MAP(COleDrop)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COleDrop message handlers

DROPEFFECT COleDrop::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	return DROPEFFECT_COPY;	
}

DROPEFFECT COleDrop::OnDragOver(CWnd* pWnd,COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	return DROPEFFECT_COPY;
}

BOOL COleDrop::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	HGLOBAL	hg = pDataObject->GetGlobalData(CF_TEXT);
	if (hg != NULL)
	{
		CString	text;
		
		text = (char *)::GlobalLock(hg);

		::GlobalUnlock(hg);
		::GlobalFree(hg);

		if (m_works->m_status & S_DROP)
			m_works->OnDrop(point, text);
		return TRUE;
	}

	return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point);
}

BOOL COleDrop::Register(CWorks* works)
{
	m_works = works;
	COleDropTarget::Revoke();
	return COleDropTarget::Register(works->m_view);
}
