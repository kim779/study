// XEdit.cpp : implementation file
//

#include "stdafx.h"
#include "XEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT NEAR WM_XEDIT_VK_RETURN = ::RegisterWindowMessage(_T("WM_XEDIT_VK_RETURN"));
UINT NEAR WM_XEDIT_VK_ESCAPE = ::RegisterWindowMessage(_T("WM_XEDIT_VK_ESCAPE"));

/////////////////////////////////////////////////////////////////////////////
// CXEdit

CXEdit::CXEdit() : m_uEditType(COMMON_TYPE)
{
}

CXEdit::~CXEdit()
{
}


BEGIN_MESSAGE_MAP(CXEdit, CEdit)
	//{{AFX_MSG_MAP(CXEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXEdit message handlers
//KHD : 2007.01.04
void CXEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	if(m_uEditType == COMMON_TYPE)
	{
		TCHAR c = static_cast<TCHAR>(uChar);
		if((c >= '0' && c <= '9') || c=='.')
		{
			CString szVal;
			long lVal;
			GetWindowText(szVal);
			lVal = atol((LPCSTR)szVal);

			if(lVal >= 1000)
			{
				AfxMessageBox("1000 이상의 값은 사용할 수 없습니다. 다시 입력바랍니다.");
				SetWindowText("");
				return;
			}
			CEdit::OnChar(uChar,nRepCnt,nFlags);
		}
	}
	else
	{
		if(!(GetStyle() & ES_READONLY) )
		{
			if(uChar == VK_BACK)//Delete 시 

			{
				CEdit::OnChar(uChar,nRepCnt,nFlags);
				return;
			}

			TCHAR c = static_cast<TCHAR>(uChar);
			if(c >= '0' && c <= '9')
			{
				CString strText;
				int nStart, nEnd,nTimeValue;
				GetSel(nStart, nEnd);
				if(nStart==0)//시간을 구하는 곳이다. 00:00
				{
					strText.Format("%c",c);
					nTimeValue = atoi(strText);
					if(nTimeValue>1 && nTimeValue<10)
					{
						strText = '0' + strText;
						ReplaceSel(strText);
					}
				}

				if(nStart ==1)//시간이 12가 넘으면 안되므로
				{
					strText.Format("%c",c);
					nTimeValue = atoi(strText);
					if(nTimeValue > 2)	return;
					ReplaceSel(CString(c)+":");
					return;
				}

				if(nStart ==3)//분이 6을 넘으면 안되므로
				{
					strText.Format("%c",c);
					nTimeValue = atoi(strText);
					if(nTimeValue > 5)	return;
				}

				GetWindowText(strText);
				int nLen = strText.GetLength();
				if(nLen==2)//2개 입력시 자동으로 : 가 생긴다.
				{
					ReplaceSel(":");
					if(nStart ==0)
						return;
				}
				if(strText.GetLength()>4)	return;

				CEdit::OnChar(uChar,nRepCnt,nFlags);
			}
		}
	}
}

void CXEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ( VK_RETURN == nChar )
	{
		CString clstrText;
		GetWindowText( clstrText );
		GetParent()->SendMessage( WM_XEDIT_VK_RETURN, (WPARAM)(LPCTSTR)clstrText );		
		return;
	}

	if ( VK_ESCAPE == nChar )
	{
		GetParent()->SendMessage( WM_XEDIT_VK_ESCAPE);
		return;
	}		
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
