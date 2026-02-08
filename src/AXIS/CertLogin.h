#if !defined(AFX_CERTLOGIN_H__22C3127B_5981_41B3_891F_3AD98CEF5EBA__INCLUDED_)
#define AFX_CERTLOGIN_H__22C3127B_5981_41B3_891F_3AD98CEF5EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CertLogin.h : header file
//
#include "button.h"
#include "BtnST.h"
#include "BmpButton.h"
#include "ShapeButton.h"
#include "ASTx/StSdkExDef.h"

#define	SM_NO		0
#define	SM_EXIT		1
#define	SM_RSM		2
#define	SM_WSH		3

#define SVS_STOCK	0
#define SVS_STANDARD	1
#define SVS_FUTURE	2

#define MAX_SHAPE	7

#define WM_SECURETOOLS  WM_USER + 1212

/////////////////////////////////////////////////////////////////////////////
// CCertLogin dialog
#include "useMaindefine.h"
class CCertLogin : public CDialog
{
// Construction
public:
	CCertLogin(CWnd* pParent = NULL, CString userID = ""/*=NULL*/);   // standard constructor
	virtual	~CCertLogin();
// Dialog Data
private:
	//{{AFX_DATA(CCertLogin)
	enum { IDD = IDD_DT_NEW_CONNECT};
	CButtonST	m_ckSISEONLY;
	CButtonST	m_ckSAVEPASS;
	CButtonST	m_ckSAVEID;
	CButtonST	m_ckCSTART;
	CButtonST	m_ckCERTLOGIN;
	CStatic		m_static;
	CString		m_user;
	CString		m_pass;
	CString		m_cPass;
	//}}AFX_DATA

public:
protected:
	BOOL	m_bNew;
	int	m_mode;
	CWnd*	m_frame;

	class	CProgressDlg* m_progress;
	CString	m_backUser;
	CStatic m_stGuest;

	BOOL	m_bOnlyGuest;
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
	bool	m_bReboot;
	BOOL	m_bCertStart;

	BOOL	m_cust;
	BOOL	m_saveID;
	BOOL	m_onlysise;
	BOOL	m_savePass;
	BOOL	m_staff;

	CFont	m_font;
	CFont	m_bFont;
	HBITMAP	m_bitmap;
	CBitmap*	m_status;
	CBitmap*	m_status_err;
	
	HBITMAP m_bitmapInput;
	HBITMAP m_bitmapEvent;
	//CEnBitmap m_enBitmap;
	CString m_strDN;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCertLogin)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

public:
	void OnChangeDuser();
	BOOL IsNewProgressImage();
	CString GetUserID();
	CString GetPassword();
	CString	GetCPass();
	void	SetUserID(CString user);
	void	SetPassword(CString pass);
	void	SetCPass(CString pass);
	void	Save();
	CString	GetSignInfo();

	void	SetStaff(BOOL bStaff = FALSE) { m_staff = bStaff; }
	void	SetStatus(int mode);
	void	SetChoice(BOOL choice);
	void	SetGuide(CString msg);
	void	SetGuide(CString msg, CString detail);
	void	ClearPassword();

	void	SetTotal(CString total);
	void	SetProgress(bool onoff);
	void	SetProgress(CString detail, int rate, BOOL error = FALSE);

	void	SetProgK(char progK) { m_progK = progK; }
	void	SetFrameRun() { m_framerun = true; }
	void	initLogin();
	void	MakeShape();
	void	ShowToolTip();
	void	HideToolTip();
	int		ShowMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);
	void	SetEditFocus();
	HWND	GetPassHwnd();
	HWND	GetCPassHwnd();
	void	SetAK(IAstxMkd8* AkSdk);
protected:
	void	initStaff();
	void	invisibleButton();
	void	InfoRSC();
	CWnd*	GetWnd(UINT nRes);
	void	RunHelpCom();
#ifdef DF_USE_CPLUS17
	std::unique_ptr<class	CMyToolTip>m_pToolTip;
#else
	class	CMyToolTip*	m_pToolTip;
#endif

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCertLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnRun();
	afx_msg void OnBanner();
	afx_msg void OnPaint();
	afx_msg void OnExit();
	afx_msg void OnMode();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnSetup();
	afx_msg void OnSaveID();
	afx_msg void OnCertStart();
	afx_msg void OnCert();
	afx_msg void OnGen();
	afx_msg void OnRunX();
	afx_msg void OnOnlysise();
	afx_msg void OnSetfocusDuser();
	afx_msg void OnKillfocusDuser();
	afx_msg void OnSetfocusDpass();
	afx_msg void OnKillfocusDpass();
	afx_msg void OnSetfocusDcpass();
	afx_msg void OnKillfocusDcpass();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnDsavepass();
	afx_msg void OnChangeDpass();
	afx_msg void OnChangeDcpass();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEvent1();
	afx_msg void OnEvent2();
	afx_msg void OnEvent3();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL RunUpdate();
	CStatic* m_bmpInput;
	CStatic* m_bmpEvent;
	CBrush	 m_msgBrush;
	CBrush	 m_msgBrush_Nor;
	CBrush	 m_msgBrush_Err;
	CArray <CBmpButton*, CBmpButton*> m_bmpButtons;
	CBitmap*	m_bmpBg;
	BOOL	m_bCertLogin;

	enum { TYPE_NORMAL_CERT = 0, TYPE_ID_LOGIN, TYPE_CLUDE };  //clude
	int		m_bLoginType = TYPE_ID_LOGIN;
	
	CBmpButton* m_btnRun;
	CBmpButton* m_btnSetup;
	CBmpButton* m_btnExit;
	CBmpButton* m_btnMode;
	CBmpButton* m_btnCert;
	CBmpButton* m_btnGen;
	CBmpButton* m_btnBanner;

	void ShowCloudeBtn(BOOL bShow, BOOL AllShow = TRUE);
	CBmpButton* m_btnClude;  //clude
	CBmpButton* m_btn_pc_Clude;
	CBmpButton* m_btn_PSchange_Clude;
	CBmpButton* m_btn_downcert_Clude;
	CBmpButton* m_btn_upcert_Clude;

	IAstxMkd8* m_AkSdk;

	CBmpButton* CreateBmpButton(UINT buttonID, const char* bmpFileName, BOOL check = FALSE, int statusCount = 2);
	void		ReleaseBmpButton();

	void SetEventBitmap(int index);
	void SetDownloadFile(CString fileName);

	CShapeButton* m_shapeButtons[MAX_SHAPE];
	CShapeButton* CreateShapeButton(UINT buttonID, const char* bmpFileName, CRect rc);
	void		  ReleaseShapeButton();

	void		  OnShapeButton(int index);
	
	int			  m_currEvent;
	void		  RotateBanner();

	BOOL		m_allResourceDownload;
	void		DeleteResourceInfoFile();

	void		ApplySecurity();
public:
	BOOL m_bCloudeUSE = FALSE;
	
	void RunCopyCfg();
	CString DeleteStr(CString p1, CString p2);
	bool IsNumber(CString str);
	bool bLoginVerUp;
	void OnLoginUpdateMode();
	bool GetCertLogin();

	void SomeDelay(HWND hWnd);
	int event_pos_x;
	void change_login(int mode);
	int			m_forceLogin;
	void		DoRun()
	{
//		OutputDebugString("START DORUN.......................................\n");
		GetDlgItem(IDC_RUN)->EnableWindow(TRUE);
		OnRun();
	}

	//로그인 버튼
	void SetLoginBtnEnable(bool bEnable) { m_btnRun->EnableWindow(bEnable); }

	//간편인증
	CString m_slog;
	void OnEasyCert();
	void ShowQRDlg();
	afx_msg void OnBnClickedCloudePcbtn();
	afx_msg void OnBnClickedCloude();
	afx_msg void OnBnClickedCloudeCertup();
	afx_msg void OnBnClickedCloudeDown();
	afx_msg void OnBnClickedCloudePschange();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CERTLOGIN_H__22C3127B_5981_41B3_891F_3AD98CEF5EBA__INCLUDED_)
