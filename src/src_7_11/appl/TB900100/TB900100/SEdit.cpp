// SEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SEdit.h"


// SEdit

IMPLEMENT_DYNAMIC(SEdit, CEdit)

SEdit::SEdit()
{

}

SEdit::~SEdit()
{
}


BEGIN_MESSAGE_MAP(SEdit, CEdit)
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// SEdit 메시지 처리기입니다.




void SEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nChar == VK_RETURN)
		GetParent()->SendMessage(WM_USER+ 888, 10, 0);
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}
