#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

#define MAX_PIPE_BUFF_SIZE 8192
// CADBDlg 대화 상자입니다.

class AFX_EXT_CLASS CADBDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CADBDlg)

public:
	CADBDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CADBDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ADB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	char m_current_path[MAX_PATH];
	SECURITY_ATTRIBUTES m_security_attributes;
	PROCESS_INFORMATION m_process_info;
	HANDLE mh_pipe_read, mh_pipe_write;
	BOOL m_run_flag;
	BOOL m_bHold;
	BOOL m_bPipeDataShow;
	CString m_sRoot;
	int m_iLineNum;
	CString m_strSearchText;
	char m_cLogLev;
	CArray<FINDTEXTEX, FINDTEXTEX> m_arryReverseFind;
	bool m_bInit;

	int m_nVisibleLine;
	int m_nTextLength;
	bool m_bSearchMode;
	FINDTEXTEX m_CaretFt;

public:
	void RunPipe(CString strData, BOOL bShow);
	void Search(int iLine, bool bCaretFt = true);
	void ResetRichEditFormat();
	void PipeDataParsor(CString strPipe);
	bool ChkCurLogLev(char cSeq);
	int GetLogLev(char cSeq);
	HANDLE GetProcessList(LPCTSTR szFilename, bool bMultiAdb);
	int GetProcessCnt(LPCTSTR szFilename);
	BOOL SafeTerminateProcess(HANDLE hProcess, UINT uExitCode);
	int GetCountPerPage();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CRichEditCtrl m_richEdlt;
	afx_msg void OnBnClickedButtonClear();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonLogsave();
	CComboBox m_cbDevice;
	afx_msg void OnBnClickedButtonDevice();
	CComboBox m_cbTag;
	CComboBox m_cbLogLev;
	afx_msg void OnBnClickedButtonEnter();
	afx_msg void OnBnClickedCheckHold();
	afx_msg void OnEnKillfocusEditInput();
	afx_msg void OnEnSetfocusEditInput();
	afx_msg void OnEnChangeEditInput();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
