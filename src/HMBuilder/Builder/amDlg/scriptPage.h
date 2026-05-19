#pragma once

#include "resource.h"
#include "colorCombo.h"

// CScriptPage 대화 상자입니다.

class CScriptPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CScriptPage)

public:
	CScriptPage();
	virtual ~CScriptPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_SCRIPT };
	CComboBox m_itemList;
	CColorCombo m_clrList;

	COLORREF	m_clBkGnd;
	COLORREF	m_clText;
	COLORREF	m_clSymbol;
	COLORREF	m_clComment;
	COLORREF	m_clConstant;
	COLORREF	m_clKeyword;
	COLORREF	m_clString;

	COLORREF	*m_refColor;

protected:
	CBrush		m_brushCLR;

	void	invalidateSample();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDefault();
	afx_msg void OnSelendokColorList();
	afx_msg	void OnSelendokSetcolorCbo();
};
