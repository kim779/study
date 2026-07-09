// tlTree.cpp: implementation of the CtlTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tlTree.h"

#include "../childview.h"
#include "../mapform.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CtlTree::CtlTree(CChildView* view)
	: CtlRect(view)
{
	m_name = _T("TreeView");
}

CtlTree::~CtlTree()
{
}

void CtlTree::drawEnd()
{
	CtlRect::drawEnd();

	if (m_rect.Width() < 10 || m_rect.Height() < 10)
	{
		m_rect.right  = m_rect.left + getDefaultWidth("TREE", 120);
		m_rect.bottom = m_rect.top  + getDefaultHeight("TREE", 50);
	}

	CRect	wRc;
	m_view->GetClientRect(&wRc);
	m_rect.right  = __min(m_rect.right,  wRc.right);
	m_rect.bottom = __min(m_rect.bottom, wRc.bottom);

	m_pFormItem->getFormPoint(m_fN)->left   = m_rect.left;
	m_pFormItem->getFormPoint(m_fN)->top    = m_rect.top;
	m_pFormItem->getFormPoint(m_fN)->right  = m_rect.right;
	m_pFormItem->getFormPoint(m_fN)->bottom = m_rect.bottom;

	m_pFormItem->getFormPoint(m_fN)->kind  = FM_TREE;
	m_pFormItem->getFormPoint(m_fN)->pRGB  = getDefaultPRGB("TREE", basINPUT);
	m_pFormItem->getFormPoint(m_fN)->attr |= FA_SEND;

	m_dc->Rectangle(m_rect);
}

void CtlTree::Draw(CDC* dc, int fN)
{
	CBrush*	brush;
	CRect	fRc;
	int	ndc = dc->SaveDC();

	CTOOL::Draw(dc, fN);
	fRc = getRect();

	setFont(dc);
	setTextColor(dc);
	if (m_pFormItem->getFormPoint(m_fN)->properties & PR_TRANSPARENT)
		dc->SetBkMode(TRANSPARENT);
	else
	{
		brush = setBrush(dc);
		setBkColor(dc);
		dc->FillRect(fRc, brush);
	}
	dc->DrawText("TreeView", fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	dc->RestoreDC(ndc);
}