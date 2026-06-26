#pragma once


// CDlg_Thread 대화 상자


#define DF_USETHREAD 1

class CDlg_Thread : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Thread)

public:
	CDlg_Thread(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Thread();

	void initctrl();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_THREAD };
#endif
#ifdef DF_USETHREAD
	class CThreadWnd* m_pWnd[4]{};
	class CThreadWnd* m_pOneWnd{};
#endif



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnTest();

public:  //thread 

	HANDLE m_hThread{};
	HANDLE m_event_TrdDlg;
	int m_threadID;
	BOOL m_bThreadUSE;
	CCriticalSection  m_cs;
	bool SendDataToThWnd(char* pdata, int ilen);
	int m_nGroup = 4;
	UINT m_iThreadID;
	bool m_balive{};
	void SelfSetEvent();
	void SetProcessAffinity(int icore);  //코어에 대한 프로세스 친화도 
	CString GetProcessAffinity();
	void SetThreadAffinity(int icore);  //코어에 대한 스레드 친화도 
	CString GetThreadAffinity();
	CString parser(CString& srcstr, CString substr);


	void TerminateRTM();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnTest2();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	//CComboBox m_cbCnt;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnTest3();
	afx_msg void OnBnClickedBtnTest4();


public:
	int m_tdID = 0;
	class CTD_test* m_hThd1;
	class CTD_test* m_hThd2;

	// race condition demo
	CArray<CString, CString> m_raceArr;
	CCriticalSection m_raceLock;
	void StartRaceDemo(bool bUseLock);
	void StopRaceDemo();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
