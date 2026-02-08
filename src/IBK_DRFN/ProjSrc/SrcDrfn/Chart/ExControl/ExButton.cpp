// ExButton.cpp: implementation of the CExButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExButton.h"
#include "../../Inc/ExDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExButton
CExButton::CExButton()
{
	m_bDrawBackground = FALSE;
}

CExButton::~CExButton()
{
}

BEGIN_MESSAGE_MAP(CExButton, CButton)
	//{{AFX_MSG_MAP(CExButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExButton message handlers
void CExButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	
	ModifyStyle(0, BS_OWNERDRAW);

	DWORD dwStyle = GetStyle();
	if(dwStyle & BS_CHECKBOX || dwStyle & BS_RADIOBUTTON)
		m_bDrawBackground = TRUE;
}

void CExButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
/*
	if(m_bDrawBackground)
	{
		CExDialog* pDialog = (CExDialog*)GetParent();
		if(!pDialog->IsKindOf(RUNTIME_CLASS(CExDialog)))
			return;

		if(pDialog->GetSkinColorManager())
		{
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect rcBtn(lpDrawItemStruct->rcItem);

			pDC->FillSolidRect(rcBtn, pDialog->GetSkinColorManager()->GetColor(1));
		}
	}
*/
}
