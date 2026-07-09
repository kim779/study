// GpEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TB205001.h"
#include "GpEdit.h"


// CGpEdit

IMPLEMENT_DYNAMIC(CGpEdit, CEdit)

CGpEdit::CGpEdit()
{

}

CGpEdit::~CGpEdit()
{
}


BEGIN_MESSAGE_MAP(CGpEdit, CEdit)
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CGpEdit 메시지 처리기입니다.




BOOL CGpEdit::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	return CEdit::PreTranslateMessage(pMsg);
}


void CGpEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nChar == VK_RETURN)
		GetParent()->SendMessage(WM_USER + 888, 1, 0);
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
