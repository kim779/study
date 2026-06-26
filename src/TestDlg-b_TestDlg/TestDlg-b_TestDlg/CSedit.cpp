// CSedit.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CSedit.h"


// CSedit

IMPLEMENT_DYNAMIC(CSedit, CWnd)

CSedit::CSedit(UINT id)
{

}

CSedit::~CSedit()
{
}


BEGIN_MESSAGE_MAP(CSedit, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CSedit::init(bool(*callback)(int, WPARAM, LPARAM))
{
	m_sticker_call = callback;
	m_pen = std::make_unique<CPen>();
	m_pen.get()->CreatePen(PS_SOLID, 2, COLOR_COMLINE);

	m_edit = std::make_unique<CXedit>();
	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | ES_LEFT |
		ES_NOHIDESEL | ES_UPPERCASE | ES_AUTOHSCROLL;
	if (!m_edit->Create(dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		m_edit = NULL;
		return;
	}
	m_edit->setaxiscall(callback);
}

// CSedit 메시지 처리기


void CSedit::changepos()
{
	CRect	cRc;

	if (m_edit)
	{
		m_edit->MoveWindow(CRect(0, 0, 0, 0));
		GetClientRect(cRc);
		cRc.right = cRc.left + DF_FUNCWND_EDITWIDTH;
		cRc.DeflateRect(3, 3);
		cRc.OffsetRect(0, 2);
		m_edit->MoveWindow(cRc);
	}

	GetClientRect(cRc);
	m_bRc.CopyRect(&cRc);

	m_bRc.left = cRc.right - DF_FUNCWND_EDITWIDTH;
}

void CSedit::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rec;
	GetClientRect(rec);

	dc.FillSolidRect(rec, COLOR_FUNC_BACK_COL);

	dc.SetBkMode(TRANSPARENT);

	CPen* oldpen = dc.SelectObject(m_pen.get());
	COLORREF oldclr = dc.GetTextColor();
	dc.SetTextColor(COLOR_COMLINE);
	CRect tmpRec(rec.left + 3, rec.top + 7, rec.left + 100, rec.bottom);
	dc.DrawText(" 검색...", tmpRec, DT_LEFT);


	tmpRec.right = rec.right;
	tmpRec.left = tmpRec.right - 20;
	tmpRec.OffsetRect(-10, -5);
	m_RcGlass = tmpRec;

	int centerX = m_RcGlass.left + m_RcGlass.Width() / 2;
	int centerY = m_RcGlass.top + m_RcGlass.Height() / 2;
	// 원의 반지름을 설정
	int radius = min(m_RcGlass.Width(), m_RcGlass.Height()) / 4; // 사각형의 크기에 맞게 반지름 조정
	// 원 그리기
	dc.Ellipse(centerX - radius, centerY - radius, centerX + radius, centerY + radius);
	// 직선 그리기 (돋보기 손잡이)
	int lineLength = radius * 1.3; // 직선 길이 설정
	int lineEndX = centerX + radius + (lineLength * cos(45 * 3.14 / 180)); // 45도 각도로 직선 그리기
	int lineEndY = centerY + radius + (lineLength * sin(45 * 3.14 / 180));
	dc.MoveTo(centerX + radius * 0.7, centerY + radius * 0.7); // 원의 가장자리에서 시작
	dc.LineTo(lineEndX, lineEndY); // 직선 끝 점


	dc.SetTextColor(oldclr);
	dc.SelectObject(oldpen);
}


void CSedit::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및CString stmp;
	CString stmp;
	stmp = "tsetsetsets";
	(*m_sticker_call)(MSG_FUNCWND_LOG, 0, (LPARAM)stmp.GetBuffer(0)); 
	CWnd::OnLButtonUp(nFlags, point);
}


void CSedit::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	changepos();
}
