#pragma once
// MiddleWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiddleWnd window

class CScreenWnd : public CWnd
{
// Construction
public:
	CScreenWnd(class CMapWnd *pParent);

// Attributes
public:
	
protected:
	class CMapWnd*	m_pMapWnd=nullptr;
	std::unique_ptr <class CDllCtrl>	m_pOrderDll;
	std::unique_ptr <class CDllCtrl2>	m_pSignedDll;
	std::unique_ptr <class CDllCtrl>	m_pBalanceDll;

	std::unique_ptr <class SplitterCtrl> m_pSplitter;
	
	int m_nHeight[3]{};
	int m_nTotalHeight{};			//Map화면의 총 높이

	BOOL m_bBtnState[3]{};		//보이기, 감추기 버튼 flag
	BOOL m_bClick{};				//onsize호출이 버튼클릭인지 사이즈 변경인지 구분

// Operations
public:
	CMapWnd* GetMapWnd();
	void SetScreenAccount(CString strParam, CString strAccount);	//계좌번호를 load한 화면으로 전송
	void GetBtnState(BOOL *btn);			
	void SetBtnState();
	void SetPal();
	int GetScreenHeight();
	void SetClickBtn(BOOL bClick);

protected:
	void CreateControl();
	void BtnClickReSize(int cx, int cy);		//버튼 클릭시 onSize
	void ScreenReSize(int cx, int cy);			//splitter 영역을 정상적으로 그리기 위해 사용
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScreenWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScreenWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

