// AccEdit.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A2.h"
#include "AccEdit.h"
#include "libDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAccEdit

CAccEdit::CAccEdit()
{
	m_bOnce = true;
}

CAccEdit::~CAccEdit()
{
}


BEGIN_MESSAGE_MAP(CAccEdit, CEdit)
	//{{AFX_MSG_MAP(CAccEdit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccEdit message handlers

void CAccEdit::OnChange() 
{
	int nStart{}, nEnd{}, nLength{};
	CString str;

	GetSel(nStart, nEnd);
	GetWindowText(str);

	
	if (m_bOnce)
	{
		m_bOnce = false;
		nLength = str.GetLength();
		str = changeFormat(str);

		if (str.GetLength() >= 11)
		{
			SetWindowText(str.Left(11));
			m_bOnce = true;
			((CDialog*)GetParent())->SendMessage(WM_ACCSEARCH);
			((CDialog*)GetParent())->NextDlgCtrl();
			return;
		}

		SetWindowText(str);
		
		SetSel(nStart + str.GetLength() - nLength, nEnd + str.GetLength() - nLength);
		m_beforeStr = str;

	}
	m_bOnce = true;	
}

CString CAccEdit::changeFormat(CString sAcc)
{
	sAcc.Replace("-", "");
	if (sAcc.GetLength() >= 9)
		sAcc.Insert(8, "-");
		
	return sAcc;
}

CString CAccEdit::GetAccount()
{
	CString sAccount;

	GetWindowText(sAccount);
	sAccount.Replace("-", "");
	return sAccount;
}
