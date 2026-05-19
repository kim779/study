#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"

// CUpDlg 대화 상자입니다.

class AFX_EXT_CLASS CUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUpDlg)

public:
	CUpDlg(CWnd* pParent = NULL)	{}   // 표준 생성자입니다.
	CUpDlg(CWnd* pParent, class mapForm* mapForm, CString userID, bool isMap = true);
	virtual ~CUpDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UP };
	CString m_source;
	CString	m_object;
	CProgressCtrl m_step;
	CString m_sRoot;
protected:
	class mapForm*	m_mapH;
	class	CwSock*	m_sock;

	enum	{ W_SRC, W_BIN, W_RES} m_what;
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
	CString setMapPath(CString mapN);
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
	afx_msg void OnBnClickedCheckTmp();
	CButton m_CheckTmp;
	CRichEditCtrl m_rEditCmt;
	afx_msg void OnBnClickedButton1Diff();
};
