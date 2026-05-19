#pragma once
#include "afxwin.h"
class CFormPreviewStatic :
	public CStatic
{
public:
	CFormPreviewStatic();
	~CFormPreviewStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnObjMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

