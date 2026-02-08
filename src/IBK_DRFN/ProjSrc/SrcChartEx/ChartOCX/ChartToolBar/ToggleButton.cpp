// ToggleButton.cpp : implementation file
//

#include "stdafx.h"
#include "ToggleButton.h"
#include "ToolBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToggleButton

CToggleButton::CToggleButton()
{
}

CToggleButton::~CToggleButton()
{
}


BEGIN_MESSAGE_MAP(CToggleButton, CButtonEx)
	//{{AFX_MSG_MAP(CToggleButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToggleButton message handlers


/*-----------------------------------------------------------------------------------------
 - Function    :  OnLButtonDown
 - Created at  :  2004-11-09   16:02
 - Author      :  최종찬
 - Description :  버튼이 눌리면 다른 버튼을 릴리즈 시킨다.
 -----------------------------------------------------------------------------------------*/
void CToggleButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// 버튼을 누를때는 다른 눌려진 툴버튼을 푼다.
	if(!m_bState)
		m_pParent->ReleaseOtherButtonUI(m_enumToolOption, m_strParam);

	// 버튼 상태 변경. (이 멤버변수로 인해 버튼상태 이미지가 그려진다. 부모클래스 DrawItem참조)
	m_bState = !m_bState;

	// (2009/10/9 - Seung-Won, Bae) Do not action on ButtonUp. It has some delay.
	if(m_bState)	// 안토글된 상태 -> 토글된 상태
		m_pParent->SetToolFunction(m_enumToolOption, this); // 해당 툴바 기능을 수행한다.
	else	// 토글된 상태 -> 안토글된 상태		
		m_pParent->ReleaseToolFunction(m_enumToolOption, this);// 해당 툴바 기능 수행을 취소시킨다.

	Invalidate();

	CButtonEx::OnLButtonDown(nFlags, point);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2004-11-09   16:25
 - Author      :  최종찬
 - Description :  마우스 움직일때... 툴팁이 있는 CButtonEx클래스의 MOUSEMOVE핸들러 강제호출.
 -----------------------------------------------------------------------------------------*/
void CToggleButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CButtonEx::OnMouseMove(nFlags, point);
}
