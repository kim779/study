#pragma once
// ScreenSetDlg.h : header file
//
#include "../../control/fx_misc/fxStatic.h"
/////////////////////////////////////////////////////////////////////////////
// CScreenSetDlg dialog

class CScreenSetDlg : public CDialog
{
// Construction
public:
	COLORREF m_clrBack;
	COLORREF	m_clrInBack;
	HBITMAP getBitmap2(CString bmps);
	COLORREF GetIndexColor(int index);
	CScreenSetDlg(CWnd* pParent = NULL, CString root = "", CString user = "", CWnd* m_Wizard = NULL);   // standard constructor
protected:
	CWnd		*m_pParent;
	CWnd		*m_pWizard;
	
	CString		m_sRoot;
	CString		m_sUser;

	CString		m_sScreenNo;		// 옵션 연결화면 번호
	CString		m_sScreenName;		// 옵션 연결화면 이름
	CString		m_sFScreenNo;		// 선물 연결화면 번호
	CString		m_sFScreenName;		// 선물 연결화면 이름
	BOOL		m_bDbClick;		// Double Click 유무
	BOOL		m_bCurScreen;		// 옵션 연결화면 현재가 유무
	BOOL		m_bFCurScreen;		// 선물 연결화면 현재가 유무
	
	std::unique_ptr<CfxStatic>	m_panel2{};
	
	CBrush	m_backBrush;
	CBrush	m_dataBrush;

protected:	
	void		SetParent(CWnd *pParent)	{ m_pParent = pParent; }
	
	CString		GetScreenNo()			{ return m_sScreenNo;	}
	CString		GetFScreenNo()			{ return m_sFScreenNo;	}
	BOOL		GetDbClick()			{ return m_bDbClick;	}
	BOOL		GetCurScreen()			{ return m_bCurScreen;	}
	BOOL		GetFCurScreen()			{ return m_bFCurScreen;	}

	bool		loadConfig();
	void		saveConfig();

	CString		parseX(CString &srcstr, CString substr);
// Dialog Data
	//{{AFX_DATA(CScreenSetDlg)
	enum { IDD = IDD_DLG_SET };
	CEdit		m_UserDefine;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenSetDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreenSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonInit();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

