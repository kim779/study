#pragma once
#include "resource.h"

// CLayoutUpload 대화 상자입니다.

class AFX_EXT_CLASS CLayoutUpload : public CDialogEx
{
	DECLARE_DYNAMIC(CLayoutUpload)

public:
	CLayoutUpload(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CLayoutUpload(CWnd* pParent, CString userID, bool isMap = true);
	virtual ~CLayoutUpload();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LAYOUT };
	CString m_source;
	CString m_path;
	CString	m_object;
	CProgressCtrl m_step;

protected:
	class	CwSock*	m_sock;

	int	m_count;
	int	m_index;
	int	m_size;
	int	m_offset;
	int	m_state;
	bool	m_done;
	CString m_userID;

	bool	sendData(bool next = true);
	int	makeData(char* sndb);
	void	updateProgress(int offset = 0);
	CString setTRPath(CString trName);
	void	goState(int state, CString msg = _T(""));
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	bool	m_isMap;

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnOpenedSock(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReadData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnState(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonUpload();
	afx_msg void OnBnClickedButtonPath();
};
