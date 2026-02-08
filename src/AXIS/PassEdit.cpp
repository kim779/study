// PassEdit.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "PassEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define _SIMPLE_OPER_
////////////////////////////////////////////////////////////////////////////
// CPassEdit

CPassEdit::CPassEdit()
{
	m_szOrg = _T("");
}

CPassEdit::~CPassEdit()
{
}


BEGIN_MESSAGE_MAP(CPassEdit, CEdit)
	//{{AFX_MSG_MAP(CPassEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(EN_MAXTEXT, OnMaxtext)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPassEdit message handlers

void CPassEdit::OnChange() 
{	
}

void CPassEdit::AllAsta()
{
	CString	str = _T("");
	int	nLen = 0;

	GetWindowText(str);
	nLen = str.GetLength();
	str = _T("");
	for ( int ii = 0 ; ii < nLen ; ii++ )
	{
		str += "*";
	}
	SetWindowText(str);
}

void CPassEdit::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
	AllAsta();
}

void CPassEdit::OnMaxtext() 
{
	// TODO: Add your control notification handler code here
	AllAsta();
}

void CPassEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#ifdef _SIMPLE_OPER_
	if (nChar != VK_BACK)
	{
		CString	szAst = _T(""), szTemp = _T("");
		int	nLen = 0;
		szAst.Format("%s%c", m_szOrg, nChar);
		m_szOrg.Format("%s", szAst);		
		GetWindowText(szAst);
		nLen = szAst.GetLength();	
		szAst = _T("");
		if (nLen)
		{
			for ( int ii = 0 ; ii < nLen ; ii++ )
			{
				szAst += "*";
			}
			szTemp.Format("%s%c", szAst, nChar);
			SetWindowText(szTemp);
			SetSel(szTemp.GetLength(), szTemp.GetLength());
			return;
		}
	}
	else
	{
		
		int	nStart = 0, nEnd = 0;
		CString	str = _T("");
		GetWindowText(str);
		const int	nLen = str.GetLength();

		if (nLen)
		{

			GetSel(nStart, nEnd);

			if (nStart != nLen || nEnd != nLen)
			{
				m_szOrg = _T("");	
				SetSel(0, nLen);
			}
			else
			{
				SetSel(nLen, nLen);
				m_szOrg = m_szOrg.Left(nLen - 1);
				
			}

		}
	}
#else
	if (nChar != VK_BACK)
	{
		
		CString	szAst = _T(""), szTemp = _T("");
		int	nLen = 0;
		GetWindowText(szAst);
		nLen = szAst.GetLength();	
		

		if (nLen)
		{
			int	nStart = 0, nEnd = 0;
			GetSel(nStart, nEnd);
			if (nStart == nLen && nEnd == nLen)
			{
				szAst.Format("%s%c", m_szOrg, nChar);
				m_szOrg.Format("%s", szAst);		
		
				szAst = _T("");
				for ( int ii = 0 ; ii < nLen ; ii++ )
				{
					szAst += "*";
				}
				szTemp.Format("%s%c", szAst, nChar);
				SetWindowText(szTemp);
				SetSel(szTemp.GetLength(), szTemp.GetLength());
				return;
			}
			else
			{
				CString	szHead = _T(""), szTail = _T("");
				szAst = m_szOrg;
				int	nHead = 0, nTail = 0;
				
				if (nStart > 0)
					szHead = szAst.Left(nStart);
				if (nEnd < nLen)
					szTail = szAst.Mid(nEnd);
				nHead = szHead.GetLength();
				nTail = szTail.GetLength();
				m_szOrg.Format("%s%c%s", szHead, nChar, szTail);

				szHead = _T(""), szTail = _T("");
				for ( int ii = 0 ; ii < nHead ; ii++ )
				{
					szHead += "*";
				}

				for ( ii = 0 ; ii < nTail ; ii++ )
				{
					szTail += "*";
				}

				szTemp.Format("%s%c%s", szHead, nChar, szTail);
				SetWindowText(szTemp);
				SetSel(szTemp.GetLength(), szTemp.GetLength());
				return;
			}
		}
		else
		{
			m_szOrg.Format("%c", nChar);
		}

	}
	else
	{
		
		int	nStart = 0, nEnd = 0;
		CString	str = _T("");
		GetWindowText(str);
		int	nLen = str.GetLength();

		if (nLen)
		{

			GetSel(nStart, nEnd);
			if (nStart == nLen || nEnd == nLen)
				m_szOrg = m_szOrg.Left(nLen - 1);
			else
			{
				if (nStart != 0 || nEnd != 0)
				{
					CString	str = _T(""), szHead = _T(""), szTail = _T("");
					str = m_szOrg;
					if (nStart > 0)
					{
						if (nStart == nEnd)
							szHead = str.Left(nStart - 1);
						else
							szHead = str.Left(nStart);
					}
					if (nEnd < nLen)
						szTail = str.Mid(nEnd);
					m_szOrg.Format("%s%s", szHead, szTail);
				}
			}
		}
	}
#endif
	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CPassEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if (nChar == VK_DELETE)
	{
		CString	str = _T("");
		GetWindowText(str);

		const int	nLen = str.GetLength();

		if (nLen)
		{
			int	nStart = 0, nEnd = 0;

			GetSel(nStart, nEnd);
			if (nStart != nLen || nEnd != nLen)
			{
#ifdef _SIMPLE_OPER_
				m_szOrg = _T("");
				SetSel(0, nLen);
#else
				CString	szHead = _T(""), szTail = _T("");
				str = m_szOrg;
				if (nStart > 0)
					szHead = str.Left(nStart);
				if (nEnd < nLen)
				{
					if (nStart == nEnd)
						szTail = str.Mid(nEnd + 1);
					else
						szTail = str.Mid(nEnd);
				}
				m_szOrg.Format("%s%s", szHead, szTail);
#endif
			}				
		}
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPassEdit::SetPass(CString pswd)
{
	m_szOrg.Format("%s", pswd);
	const int	nLen = pswd.GetLength();

	pswd = _T("");
	for ( int ii = 0 ; ii < nLen ; ii++ )
	{
		pswd += "*";
	}

	SetWindowText(pswd);
}