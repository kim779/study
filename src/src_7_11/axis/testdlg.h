#pragma once


// testdlg 대화 상자입니다.

class testdlg : public CDialogEx
{
	DECLARE_DYNAMIC(testdlg)

public:
	testdlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~testdlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
