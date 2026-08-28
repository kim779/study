// HistDetailWnd.h : popup window that shows one full Send/Receive record pair
//   looked up by exact capture timestamp (see CChildView::OnClickedBtnFindTime)
#pragma once

class CHistDetailWnd : public CWnd
{
public:
	CHistDetailWnd();
	virtual ~CHistDetailWnd();

	void ShowText(CWnd* parent, const CString& title, const CString& text);

protected:
	CEdit	m_edit;
	CFont	m_font;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
};
