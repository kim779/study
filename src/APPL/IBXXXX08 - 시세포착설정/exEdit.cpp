#include "stdafx.h"
#include "IBXXXX08.h"
#include "exEdit.h"
#include "libDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExEdit::CExEdit()
{
	m_pParent = nullptr;
	m_str = "";
}

CExEdit::~CExEdit()
{
}


BEGIN_MESSAGE_MAP(CExEdit, CEdit)
	//{{AFX_MSG_MAP(CExEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// 한글입력인 경우 VK_RETURN들어오면 마지막 글자가 다시 나오는 오류 수정
	switch (nChar)
	{
	case VK_RETURN:
		{
			/*
			TRACE("OnChar return\n");
			CString tmpS;
			GetWindowText(tmpS);
			SetWindowText("");
			SetSel(0, -1);
			*/
		}
		break;
	default:
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CExEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_pParent && (nChar == VK_UP || nChar == VK_DOWN || nChar == VK_RETURN))
		m_pParent->SendMessage(WM_LOCAL, EVT_CHAR, nChar);
	
	//if (nChar == VK_RETURN)
	//	TRACE("OnKeyDown return\n");

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExEdit::OnChange() 
{
	GetWindowText(m_str);
	const int len = m_str.GetLength();
	if (len <= 0)	return;
	m_str.MakeUpper();

	if (!m_pParent)
		return;
	m_pParent->SendMessage(WM_LOCAL, EVT_TEXT, (LPARAM)(LPCTSTR)m_str);
}
