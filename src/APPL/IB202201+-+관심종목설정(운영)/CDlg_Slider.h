#pragma once


// CDlg_ 대화 상자
class CDlg_Slider : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Slider)

public:
	CDlg_Slider(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Slider();

	CString m_root{};
	BOOL m_bCustomer{};
	void LoadRate();
	void SaveRate();
	bool isIPInRange(CString ip, CString network_s, CString network_e);
	unsigned int IPToUInt(CString ip);
	CString CheckIP();

	int m_iTime{};
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SLIDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_slider;
	afx_msg void OnCustomdrawSliderRate(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
};
