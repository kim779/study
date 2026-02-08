// DropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "fxgrid.h"
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
	m_fxGrid = nullptr;
	m_registered = FALSE;
}

CDropTarget::~CDropTarget()
{
}

BOOL CDropTarget::Register(CfxGrid *pfxGrid)
{
	if (m_registered || pfxGrid == nullptr)
		return FALSE;

	ASSERT(pfxGrid->IsKindOf(RUNTIME_CLASS(CfxGrid)));
	ASSERT(pfxGrid);

	if (!pfxGrid || !pfxGrid->IsKindOf(RUNTIME_CLASS(CfxGrid)))
		return FALSE;

	m_fxGrid = pfxGrid;

	m_registered = COleDropTarget::Register(pfxGrid);
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
	if (pWnd->GetSafeHwnd() == m_fxGrid->GetSafeHwnd())
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
	if (IS_NULL(m_fxGrid))
		return DROPEFFECT_NONE;

	if (pWnd->GetSafeHwnd() == m_fxGrid->GetSafeHwnd())
		return m_fxGrid->OnDragEnter(pDataObject, dwKeyState, point);
	else
		return DROPEFFECT_NONE;
}

void CDropTarget::OnDragLeave(CWnd* pWnd)
{
	if (IS_NULL(m_fxGrid))
		return;

	if (pWnd->GetSafeHwnd() == m_fxGrid->GetSafeHwnd())
		m_fxGrid->OnDragLeave();
}

DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if (IS_NULL(m_fxGrid))
		return DROPEFFECT_NONE;;

	if (pWnd->GetSafeHwnd() == m_fxGrid->GetSafeHwnd())
		return m_fxGrid->OnDragOver(pDataObject, dwKeyState, point);
	else
		return DROPEFFECT_NONE;
}

BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if (IS_NULL(m_fxGrid))
		return FALSE;

	if (pWnd->GetSafeHwnd() == m_fxGrid->GetSafeHwnd())
		return m_fxGrid->OnDrop(pDataObject, dropEffect, point);
	else
		return FALSE;
}
