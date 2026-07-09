// Panel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TB205001.h"
#include "Panel.h"


// CPanel

IMPLEMENT_DYNAMIC(CPanel, CWnd)

CPanel::CPanel()
{
	m_bleft = true;
}

CPanel::~CPanel()
{
	delete m_pPenup;
	m_pPenup = NULL;

	delete m_pPen;
	m_pPen = NULL;
}


BEGIN_MESSAGE_MAP(CPanel, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CPanel 메시지 처리기입니다.




void CPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CRect rc, tmprc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, RGB(249,249,249));

	CPen *pOldPen = dc.SelectObject(m_pPenup);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right, rc.top);

	dc.MoveTo(rc.left, rc.bottom - 1);
	dc.LineTo(rc.right, rc.bottom - 1);

	if(!m_bleft)
		return;

	dc.SelectObject(m_pPen);
	tmprc.top = rc.top + 15;
	tmprc.left = rc.left + 10;
	tmprc.right =  tmprc.left + 3;
	tmprc.bottom = tmprc.top + 3;

	dc.MoveTo(tmprc.left, tmprc.top);
	dc.LineTo(tmprc.left, tmprc.bottom);

	dc.MoveTo(tmprc.left, tmprc.bottom);
	dc.LineTo(tmprc.right, tmprc.bottom);

	dc.MoveTo(tmprc.right, tmprc.bottom);
	dc.LineTo(tmprc.right, tmprc.top);

	dc.MoveTo(tmprc.right, tmprc.top);
	dc.LineTo(tmprc.left, tmprc.top);

	dc.SelectObject(pOldPen);

	dc.SetBkMode(TRANSPARENT);

	tmprc.top = rc.top + 8;
	tmprc.left = rc.left + 13;
	tmprc.right =  tmprc.left + 60;
	tmprc.bottom = tmprc.top + 20;

	dc.SelectObject(m_pFont);
	dc.SetTextColor(RGB(75, 96, 139));
	((CDC*)&dc)->DrawText("그룹추가", &tmprc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}


int CPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pPenup = new CPen;
	m_pPenup->CreatePen(PS_SOLID, 1, RGB(183,187,209));

	m_pPen = new CPen;
	m_pPen->CreatePen(PS_SOLID, 1, RGB(0,0,0));
	return 0;
}
