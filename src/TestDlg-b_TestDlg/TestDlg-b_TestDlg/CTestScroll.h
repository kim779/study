#pragma once


// CTestScroll
#include "CScrollWindow.h"
class CTestScroll : public CScrollWindow
{
//	DECLARE_DYNAMIC(CTestScroll)

public:
	CTestScroll();
	virtual ~CTestScroll();
	CFont m_Font;
	virtual void OnDraw(CDC* pDC);
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


