#if !defined(AFX_MOVEDLG_H__INCLUDED_)
#define AFX_MOVEDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// transDlg.h : header file
//

// CMoveDlg 대화 상자입니다.

class CMoveDlg : public CDialog
{
	DECLARE_DYNAMIC(CMoveDlg)

public:
	CMoveDlg(CWnd* pParent, int kind);   // 표준 생성자입니다.
	virtual ~CMoveDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MOVE };

public:
	CStringArray	m_files;


protected:
	int	m_kind;			// combol select
	int	m_index;

	char	m_root[256];
	char	m_adbpath[256];		// adb path
	char	m_runpath[256];		// phone path
	char	m_activity[256];	// run activity

	CProgressCtrl m_progress;

	void	guideMsg(CString string);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedRun();
	afx_msg void OnClickedMove();
};

#endif // !defined(AFX_MOVEDLG_H__INCLUDED_)