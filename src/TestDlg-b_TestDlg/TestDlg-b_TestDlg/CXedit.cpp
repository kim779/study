// CXedit.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CXedit.h"
#include "NoteDefine.h"

// CXedit

IMPLEMENT_DYNAMIC(CXedit, CEdit)

CXedit::CXedit()
{
	m_textColor = COLOR_BARTEXT;
	m_bkColor = COLOR_FUNC_BACK_COL;
	m_brush.CreateSolidBrush(m_bkColor);
}

CXedit::~CXedit()
{
}

BEGIN_MESSAGE_MAP(CXedit, CEdit)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, &CXedit::OnEnChange)
END_MESSAGE_MAP()

// CXedit 메시지 처리기
void CXedit::setaxiscall(bool (*callback)(int, WPARAM, LPARAM))
{
	m_sticker_call = callback;
}

void CXedit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CEdit::OnPaint()을(를) 호출하지 마십시오.

	//CRect rc;
	//GetClientRect(rc);
	//dc.FillSolidRect(rc, RGB(255, 0, 0));
}


HBRUSH CXedit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

HBRUSH CXedit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(COLOR_COMLINE);
	pDC->SetBkColor(COLOR_FUNC_BACK_COL);
	return (HBRUSH)m_brush;
}

void CXedit::OnEnChange()
{
	CString text;
	GetWindowText(text);
	if(text != "검색..")
		(*m_sticker_call)(MSG_FUNCWND_LOG, 0, (LPARAM)text.GetBuffer(0));
	Invalidate();
//	m_isEmpty = text.IsEmpty();
	if(text.IsEmpty())
		SetWindowText(_T("검색.."));
//	Invalidate(); // 컨트롤을 다시 그리도록 요청
}