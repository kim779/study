
#include "stdafx.h"
#include "OrderWnd.h"
#include "../h/formOb.h"

COrderWnd::COrderWnd(CformProp* pProp, DWORD dwOrderKind)
{
	//m_strIdx.Format("%d",idx);
	m_pProp = pProp;
	m_dwOrderKind = dwOrderKind;
}


COrderWnd::~COrderWnd()
{

}

BEGIN_MESSAGE_MAP(COrderWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void COrderWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rc;
	GetClientRect(rc);
	CString str("");

	if(m_dwOrderKind == 1)
	{
		dc.FillSolidRect(rc, RGB(0, 0, 120));
		str.Format("%d",m_pProp->index);
	}
	else
	{
		dc.FillSolidRect(rc, RGB(0, 120, 0));
		str.Format("%d",m_pProp->iorder);
	}
	dc.SetTextColor(RGB(255, 255, 254));
	dc.DrawText(str, rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}
