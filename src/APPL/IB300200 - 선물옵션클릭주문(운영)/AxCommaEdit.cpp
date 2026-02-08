// CommaEdit.cpp : implementation file
//

#include "stdafx.h"
#include "AxCommaEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxCommaEdit

CAxCommaEdit::CAxCommaEdit()
{
	m_bOnce = true;
}

CAxCommaEdit::~CAxCommaEdit()
{
}


BEGIN_MESSAGE_MAP(CAxCommaEdit, CEdit)
	//{{AFX_MSG_MAP(CAxCommaEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxCommaEdit message handlers

void CAxCommaEdit::OnChange() 
{
	int nStart{}, nEnd{}, nLength{};
	CString str;
	GetSel(nStart, nEnd);
	GetWindowText(str);
	if (m_bOnce)
	{
		m_bOnce = false;
		nLength = str.GetLength();
		if (!m_beforeStr.CompareNoCase(SetComma(str)) && (m_nChar == VK_BACK || m_nChar == VK_DELETE))
		{
			if (m_nChar == VK_BACK)
				str.Delete(nStart - 1);
			else
			{
				str.Delete(nStart);
				nStart=++nEnd;
			}
		}
		str = SetComma(str);
		SetWindowText(str);
		
		SetSel(nStart + str.GetLength() - nLength, nEnd + str.GetLength() - nLength);
		m_beforeStr = str;
	}
	m_bOnce = true;
	GetParent()->SendMessage(WM_USER, MAKEWPARAM(GetDlgCtrlID(), EN_CHANGE));
}

CString CAxCommaEdit::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii{}, jj = -1;

	src.TrimLeft();
	src.Replace(",", "");
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

void CAxCommaEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_nChar = nChar;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAxCommaEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


CString CAxCommaEdit::GetText(bool bTrim)
{
	CString sText;
	GetWindowText(sText);
	if (bTrim)
		sText.Replace(",", "");
	return sText;
}
