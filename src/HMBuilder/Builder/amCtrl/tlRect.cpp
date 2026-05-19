// tlRect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tlRect.h"
#include "../h/mainvar.h"
// CTlRect

CTlRect::CTlRect(CChildView* view, CAmDraw* draw, CFormItem* formItem)
	: CTlBase(view, draw, formItem)
{
}

CTlRect::~CTlRect()
{
}

void CTlRect::drawBegin(CPoint point)
{
	m_rect.left = m_rect.right = point.x;
	m_rect.top  = m_rect.bottom = point.y;

	m_dc->SelectStockObject(BLACK_PEN);
	m_dc->SelectStockObject(NULL_BRUSH);
	m_dc->SetROP2(R2_NOT);
	m_dc->Rectangle(m_rect);
}

void CTlRect::drawTo(CPoint point)
{
	CPen pen, *open;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(255, 255, 255);
	pen.CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);
	open = m_dc->SelectObject(&pen);


	m_dc->Rectangle(m_rect);
	m_rect.right = point.x;
	m_rect.bottom = point.y;
	m_dc->Rectangle(m_rect);

	m_dc->SelectObject(open);
	pen.DeleteObject();
	// 2015-02-23
	// New Object tooltip kjs 
	CString sTip;
	sTip.Format(_T("(%d, %d, %d, %d)"), m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)sTip.operator LPCTSTR());
}

void CTlRect::drawEnd()
{
	m_rect.NormalizeRect();

	m_dc->Rectangle(m_bound);
	m_dc->SetROP2(R2_COPYPEN);
	setPen(m_dc, clBLACK);
	m_dc->SelectStockObject(NULL_BRUSH);
}