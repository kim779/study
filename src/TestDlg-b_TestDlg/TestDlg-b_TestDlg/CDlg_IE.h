#pragma once


// CDlg_IE 대화 상자

class CDlg_IE : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_IE)

public:
	CDlg_IE(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_IE();
	class CAxBrowser* m_pHtml;
	bool m_bScroll{};
	CRect m_Htmlrc;
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_IE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
