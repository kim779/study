#pragma once



class _csDrawData
{
public:
	_csDrawData()
	{
		_sData = "";
		_rect.SetRectEmpty();
		_parentRect.SetRectEmpty();
		frgb = RGB(0, 0, 0);
	}
	~_csDrawData() = default;

	CString _sData{};
	CRect  _rect{};
	CRect _parentRect{};
	COLORREF frgb{};

};


class CDlgSTD : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSTD)

public:
	CDlgSTD(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CString m_slog{};
	int _iCnt{};

	virtual ~CDlgSTD();
	CToolTipCtrl m_toolTip;
	BOOL m_bThreadEnd{};

	std::future<void> future{};
	std::shared_future<int> m_future;

	std::queue<std::shared_ptr< _csDrawData>> g_DrawQueue{};
	std::mutex g_DrawMutex;

	std::future<bool> _fut;
	std::atomic<bool> atmc_StopFlag{ false };

	std::map <CString, int> _map{};

	CComboBox* m_cb{};
	enum TEST_TYPE
	{
		FUTURE = 0,
		REAL,
		REDUCE,
		FOREACH,   //for(auto* it : items}
		ANY_OF,   //any_of  begin 돌면서 하나라도 있으면 true 
		READFILE_THREAD, //비동기 스레드를 통해서 파일 읽기
		MAP_RETURN,    //emplace 리턴에 대한
		LOADLIBFUC   //라이브러리도 로드하고 함수도..
	};
	int m_iTestType{};
// 대화 상자 데이터입니다.
	//void readFile_ayncTd(CString sfile);

public:   //que
	ThreadSafeQueue<CMQue*> m_queRTM;
	std::vector<CWinThread*> m_threads;
	bool m_running = true;

	
public:   //
	void CheckVectorMap();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_STD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMsg();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTest();
	afx_msg void OnBnClickedFuncTemp();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedDrawThread();
	afx_msg void OnCbnSelchangeFuncCb();
	afx_msg void OnBnClickedDrawStop();
	afx_msg void OnBnClickedQueStart();
	afx_msg void OnBnClickedChkVm();
};
