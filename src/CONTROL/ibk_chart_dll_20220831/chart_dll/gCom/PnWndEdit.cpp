// PnWndEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnWndEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnWndEdit

CPnWndEdit::CPnWndEdit(CWnd* pParent, CWnd* pView, int type)
{
	m_pParent = pParent;
	m_pView = pView;
	m_Type = type;
}

CPnWndEdit::~CPnWndEdit()
{
}


BEGIN_MESSAGE_MAP(CPnWndEdit, CEdit)
	//{{AFX_MSG_MAP(CPnWndEdit)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPnWndEdit message handlers

void CPnWndEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString tmpS = _T("");
	GetWindowText(tmpS);

	switch (nChar)
	{
	case VK_RETURN:
		switch (m_Type)
		{
			/**
		case EDT_RTIME:
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipRotate), atoi(tmpS));
			break;
			**/
		case EDT_GIGAN:
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_GIGAN), atoi(tmpS));
			break;
		case EDT_DISP:
			m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipDisplay), atoi(tmpS));
			break;
		case IC_EDIT:
			m_pParent->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlChgDisp), atoi(tmpS));
			break;
		}
		break;
	case VK_TAB:
		switch (m_Type)
		{
			//**case EDT_RTIME:
			case EDT_GIGAN:
			case EDT_DISP:
				m_pParent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipTab), m_Type);
				break;
			case IC_EDIT:
				m_pParent->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlITab), m_Type);
				break;
		}
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CPnWndEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	SetSel(0, -1);
}

void CPnWndEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);

	SetSel(0, -1);
}

