#pragma once

// Scrollable child dialog that hosts the test buttons (IDD_BTN_PANEL).
// Clicks handled here first (new handlers); unhandled ones are forwarded to CTestDlgDlg.
class CBtnPanel : public CDialogEx
{
public:
	enum { IDD = IDD_BTN_PANEL };	// lets the IDE wizard bind the resource to this class

	CBtnPanel();

protected:
	int m_nScrollPos{};
	int m_nContentHeight{};

	void UpdateScrollInfo();
	void ScrollTo(int nPos);

	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAccCheck();
	afx_msg void OnBnClickedBtnNewtest();
};
