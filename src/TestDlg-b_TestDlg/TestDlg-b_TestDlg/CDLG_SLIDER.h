#pragma once


// CDLG_SLIDER 대화 상자

class CDLG_SLIDER : public CDialog
{
	DECLARE_DYNAMIC(CDLG_SLIDER)

public:
	CDLG_SLIDER(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDLG_SLIDER();
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SLIDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdrawSliderRate(NMHDR* pNMHDR, LRESULT* pResult);
	CSliderCtrl m_sliders;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	CComboBox m_cb;
	afx_msg void OnCbnSelchangeCombo1();
};
