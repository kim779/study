
#pragma once
// Connect.h : header file
//
#include "resource.h"
#include "button.h"
#include "BtnST.h"

#include "HanEdit.h"

#define	SM_NO		0
#define	SM_EXIT		1
#define	SM_RSM		2
#define	SM_WSH		3

#define SVS_STOCK	0
#define SVS_STANDARD	1
#define SVS_FUTURE	2

#define WM_KEYLOCK	WM_USER+7777

/////////////////////////////////////////////////////////////////////////////
// CConnect dialog

class CConnect : public CDialog
{
// Construction
public:
	CConnect(CWnd* pParent = NULL);   // standard constructor
	CConnect(CWnd* pParent, CString home, CString userID);   // standard constructor
	virtual	~CConnect();
// Dialog Data
	//{{AFX_DATA(CConnect)
	enum { IDD = IDD_DT_CONNECT };
	CButtonST	m_ckSISEONLY;
	
	CButtonST	m_ckSAVEID;
	CStatic		m_static;
	CString		m_user;
	CString		m_pass;
	CString		m_cPass;
	//}}AFX_DATA

public:
	CBrush m_brBkgnd, m_brBkgnd1;

protected:
	int	m_mode;
	CString	m_home;
	class	CMainFrame*	m_frame;
	class	CProgressDlg*	m_progress;
	
	bool	m_auto;
	bool	m_setup;
	bool	m_skinShow;
	bool	m_framerun;
	bool	m_bWeb1;
	bool	m_bWeb2;
	int	m_by;
	int	m_serviceK;
	char	m_progK;

	int	m_rate;
	int	m_totalCNT;
	int	m_curCNT;
	double	m_totalSIZE;
	double	m_curSIZE;
	CString	m_currentFILE;

	BOOL	m_cust;
	BOOL	m_saveID;
	BOOL	m_onlysise;
	BOOL	m_savePass;
	BOOL	m_staff;
	BOOL        m_bAutologin;

	CFont	m_bFont;
	HBITMAP	m_bitmap;

	Cbutton	m_bnRUN;
	Cbutton	m_bnEXIT;
	Cbutton	m_bnSETUP;
	Cbutton	m_bnGUIDE;

	Cbutton m_bnMINI;
	Cbutton	m_bnCA;
	Cbutton	m_bnBREAK;
	Cbutton m_bnCENTER;
	Cbutton	m_bnHELP;
	Cbutton m_bnIDLOST;
	Cbutton m_btnclose;

	CButtonST m_ckSavePw;
	CButtonST m_ckFileDown;
	CButtonST m_ckAutoConnect;
	CButtonST m_ckSAVEPASS;
	
	DWORD		m_Conversion, m_Sentence;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnect)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int		urlCheck() ;
	CString GetUserID();
	CString GetPassword();
	CString	GetCPass();
	void	SetUserID(CString user);
	void	SetPassword(CString pass);
	void	SetCPass(CString pass);
	void	Save();

	void	SetStaff(BOOL bStaff = FALSE) { m_staff = bStaff; }
	void	SetStatus(int mode);
	void	SetChoice(BOOL choice);
	void	SetGuide(CString msg, bool bMsgbox = false);
	void	SetGuide(CString msg, CString detail);
	void	ClearPassword();

	void	SetTotal(CString total);
	void	SetProgress(bool onoff);
	void	SetProgress(CString detail, int rate, BOOL error = FALSE);

	void	SetProgK(char progK) { m_progK = progK; }
	void	SetFrameRun() { m_framerun = true; }
	void	initLogin();
	void	MakeShape();

	void	useKeyProtect(bool bSet);
	CString GetdecPassword();

protected:
	void	initStaff();
	void	invisibleButton();
	void	initButtons();
	void	InfoRSC();
	CWnd*	GetWnd(UINT nRes);
	BOOL	IsexistHangul(CString id);
	int	valid_usid(CString usid);
	HRGN DIBToRgn(HBITMAP hBmp,COLORREF BkColor,BOOL Direct);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnect)
	virtual BOOL OnInitDialog();
	afx_msg void OnRun();
	afx_msg void OnMember();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnMemberModify();
	afx_msg void OnSaveID();
	afx_msg void OnExit();
	afx_msg void OnOnlysise();
	afx_msg void OnWorkCall();
	afx_msg void OnRemote();
	afx_msg void OnMsgLogin();
	afx_msg void OnUniKey();
	afx_msg void OnIDPassFind();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnStop(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
public:
	
	afx_msg void OnBnClickedDautoconnect();
	afx_msg void OnSetup();
	afx_msg void OnBnClickedBnHelp2();
	

	afx_msg void OnBnClickedDmsg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedDsavepw();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetfocusDuser();
	afx_msg void OnKillfocusDuser();
	CEdit m_edituser;
	CHanEdit m_edithan;
};

