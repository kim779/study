// mEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "mEdit.h"


// CMEdit

IMPLEMENT_DYNAMIC(CMEdit, CEdit)

CMEdit::CMEdit()
{

}

CMEdit::~CMEdit()
{
}


BEGIN_MESSAGE_MAP(CMEdit, CEdit)
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CMEdit 메시지 처리기입니다.




void CMEdit::OnKillFocus(CWnd* pNewWnd)
{
	ShowWindow(SW_HIDE);
	CEdit::OnKillFocus(pNewWnd);
}
