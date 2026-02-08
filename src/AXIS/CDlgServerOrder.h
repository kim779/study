#pragma once


// CDlgServerOrder 대화 상자

class CDlgServerOrder : public CDialog
{
	//DECLARE_DYNAMIC(CDlgServerOrder)

public:
	CDlgServerOrder(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgServerOrder();

	BOOL	Init();
	void ChangePalette();
	void LoadCondition();

	CWnd* m_parent{};
	
	CFont m_gridfont;
	std::unique_ptr<CFont> m_font{};
	CStringArray m_arr;
	CRect   m_recOri, m_recDef;
	std::unique_ptr<class CfxGrid> m_pGrid;
	int m_iSumGridCol{};
	void InitGrid();
	void ResizeGrid();
	void	AddServerOrd(CString dat, int max);
public:
	CString parseX(CString& srcstr, CString substr);
	int _httoi(const TCHAR* value);
	DWORD GetAttrByStr(CString sAttr);
	COLORREF GetColor(int index);
	CString Parser(CString& srcstr, CString substr);

public:
	double m_dBtn1RatioL{}, m_dBtn1RatioR{}, m_dBtn2RatioL{}, m_dBtn2RatioR{}, m_dBtn3RatioL{}, m_dBtn3RatioR{}, m_dBtn4RatioL{}, m_dBtn4RatioR{};
	double m_dBtn1RatioT{}, m_dBtn1RatioB{}, m_dBtn2RatioT{}, m_dBtn2RatioB{}, m_dBtn3RatioT{}, m_dBtn3RatioB{}, m_dBtn4RatioT{}, m_dBtn4RatioB{};
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SERVERORDER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedSsetup();
	afx_msg void OnBnClickedOrirect();
	afx_msg void OnBnClickedSclear();
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnPaint();
};
