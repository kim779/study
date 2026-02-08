main// DropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "intgrid.h"
#include "DropTarget.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDropTarget

CDropTarget::CDropTarget()
{
	m_intGrid = nullptr;
	m_registered = FALSE;
}

CDropTarget::~CDropTarget()
{
}

BOOL CDropTarget::Register(CintGrid *pintGrid)
{
	if (m_registered)
		return FALSE;

	if (IS_NULL(pintGrid))
		return FALSE;


	ASSERT(pintGrid->IsKindOf(RUNTIME_CLASS(CintGrid)));
	ASSERT(pintGrid);

	if (!pintGrid || !pintGrid->IsKindOf(RUNTIME_CLASS(CintGrid)))
		return FALSE;

	m_intGrid = pintGrid;

	m_registered = COleDropTarget::Register(pintGrid);
	return m_registered;
}

void CDropTarget::Revoke()
{
	m_registered = FALSE;
	COleDropTarget::Revoke();
}

BEGIN_MESSAGE_MAP(CDropTarget, COleDropTarget)
	//{{AFX_MSG_MAP(CDropTarget)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDropTarget message handlers
DROPEFFECT CDropTarget::OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint /*point*/)
{
	if (pWnd->GetSafeHwnd() == m_intGrid->GetSafeHwnd())
	{
		if (dwKeyState & MK_CONTROL)
			return DROPEFFECT_COPY;
		else
			return DROPEFFECT_MOVE;
	}

	return DROPEFFECT_NONE;
}

DROPEFFECT CDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT(m_intGrid);
	if (IS_NULL(m_intGrid))
		return  DROPEFFECT_NONE;
	if (pWnd->GetSafeHwnd() == m_intGrid->GetSafeHwnd())
		return m_intGrid->OnDragEnter(pDataObject, dwKeyState, point);
	else
		return DROPEFFECT_NONE;
}

void CDropTarget::OnDragLeave(CWnd* pWnd)
{
	ASSERT(m_intGrid);
	if (IS_NULL(m_intGrid))
		return;
	if (pWnd->GetSafeHwnd() == m_intGrid->GetSafeHwnd())
		m_intGrid->OnDragLeave();
}

DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	ASSERT(m_intGrid);

	if (IS_NULL(m_intGrid))
		return  DROPEFFECT_NONE;
	if (pWnd->GetSafeHwnd() == m_intGrid->GetSafeHwnd())
		return m_intGrid->OnDragOver(pDataObject, dwKeyState, point);
	else
		return DROPEFFECT_NONE;
}

BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	ASSERT(m_intGrid);
	if (IS_NULL(m_intGrid))
		return  FALSE;
	if (pWnd->GetSafeHwnd() == m_intGrid->GetSafeHwnd())
		return m_intGrid->OnDrop(pDataObject, dropEffect, point);
	else
		return FALSE;
}

BOOL CDropTarget::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleDropTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}
