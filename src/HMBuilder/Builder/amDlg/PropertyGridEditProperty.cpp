// PropertyGridEditProperty.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PropertyGridEditProperty.h"


// CPropertyGridEditProperty

IMPLEMENT_DYNAMIC(CPropertyGridEditProperty, CMFCPropertyGridProperty)

CPropertyGridEditProperty::CPropertyGridEditProperty(int nLimit, DWORD dwStyle, const CString& strName, const _variant_t& varValue, LPCTSTR lpszDescr, DWORD dwData)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData)
{
	m_limit = nLimit;
	m_style = dwStyle;
	m_bColor = false;
}

CPropertyGridEditProperty::~CPropertyGridEditProperty()
{
}

CWnd* CPropertyGridEditProperty::CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat)
{
	CWnd* pWnd = CMFCPropertyGridProperty::CreateInPlaceEdit(rectEdit, bDefaultFormat);
	if (pWnd != NULL)
	{
		if (m_limit > 0)
			((CEdit *)pWnd)->LimitText(m_limit);
		if (m_style > 0)
			((CEdit *)pWnd)->ModifyStyle(0, m_style);
	}
	return pWnd;
}

void CPropertyGridEditProperty::OnDrawValue(CDC* pDC, CRect rect)
{
	if (m_bColor)
		pDC->FillSolidRect(&rect, RGB(250, 230, 230));
	CMFCPropertyGridProperty::OnDrawValue(pDC, rect);
}