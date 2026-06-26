#pragma once


// CDlg_Calendar 대화 상자

class CDlg_Calendar : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Calendar)

public:
	CDlg_Calendar(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Calendar();

// 대화 상자 데이터입니다.


	//달력
	void AdjustMonthCalPosition(RECT* pRect);
	CMonthCalCtrl  m_calendarCtrl;  //이건 헤더에서 클래스 맴버롤 해줘야 동작을 하더라...

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CALENDAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCal2();
	afx_msg void OnBnClickedBtnCal1();


	
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedBtnCal3();
};
