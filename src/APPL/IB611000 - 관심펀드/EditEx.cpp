// EditEx.cpp : implementation file
//

#include "stdafx.h"
#include "IB611000.h"
#include "EditEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditEx

CEditEx::CEditEx()
{
	m_nLimit = -1;
}

CEditEx::~CEditEx()
{
}


BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	//{{AFX_MSG_MAP(CEditEx)
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditEx message handlers

void CEditEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_nLimit >= 0)
	{
		CString	str;
		GetWindowText(str);
		const int len = str.GetLength();

		if (nChar < 128)
		{
			if (nChar >= 33 && nChar <= 126)
			{
				if (len >= m_nLimit)
					return;
			}
		}
		else
		{
			if (len >= (m_nLimit - 1))
				return;
		}
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CEditEx::SetLimit(int nLength)
{
	m_nLimit = nLength;
	SetLimitText(nLength);
}

BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CEdit::PreTranslateMessage(pMsg);
}

void CEditEx::OnChange()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here

}
