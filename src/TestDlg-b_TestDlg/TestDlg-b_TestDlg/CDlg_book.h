#pragma once


// CDlg_book 대화 상자

class cs_92p1
{
public:
	cs_92p1() {};
	~cs_92p1() {};

	__declspec(dllexport) void TestFunc(int a  , int b , int c = 1 );
	 void TestFunc(int a = 1, int b = 3);
};

class CDlg_book : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_book)

public:
	CDlg_book(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_book();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHOLBOOK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();



	afx_msg void OnBnClickedButton2();
};

