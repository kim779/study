#pragma once


// CDlg_wmic 대화 상자

class CDlg_wmic : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_wmic)

public:
	CDlg_wmic(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_wmic();

	//wmic
	CMapStringToString m_mapWmic;
	
	void SearchWmic(CString skey, CString sval);
	void initcombo();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_WMIC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cbKey;
	CComboBox m_cbVal;
	CBitmap* m_pClearBmp{};
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnDlgwmic();
	afx_msg void OnBnClickedBtnDlgwmic2();
	afx_msg void OnPaint();
};
