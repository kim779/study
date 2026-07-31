#pragma once


// CMDLSbox 대화 상자


class CMDLSbox : public CDialog
{
	DECLARE_DYNAMIC(CMDLSbox)

public:
	CMDLSbox(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMDLSbox();

	CWnd* m_pParent{};
	CString m_text{};
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MDLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
