#pragma once


// CHiddenWnd

class CHiddenWnd : public CWnd
{
	DECLARE_DYNAMIC(CHiddenWnd)

private:
	CWnd* m_pParent{};
	HWND	 m_hWnd{};  //axis mainframe의 핸들
public:
	CHiddenWnd();
	virtual ~CHiddenWnd();

	BOOL	CreateMap(CWnd* pParent);

	void     sendTR(CString trCode, char* datB, int datL, int key);
	void	    Request_GroupList();											//관심그룹 조회
	void     Request_GroupCode(int iseq, int iTRKey);           //관심그룹내부 종목코드 조회
	void     Request_uploadEachGroup(int gno);

protected:
	LRESULT OnMsgFromMain(WPARAM wParam, LPARAM lParam);
	LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


