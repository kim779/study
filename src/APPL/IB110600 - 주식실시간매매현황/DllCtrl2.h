#pragma once
// DllCtrl2.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDllCtrl2 window

class CDllCtrl2 : public CWnd
{
// Construction
public:
	CDllCtrl2(class CScreenWnd *pParent);

// Attributes
public:

protected:
	class CMapWnd *m_pMapWnd;
	class CScreenWnd *m_pScreenWnd;

	int m_nkey;				//load한 맵의 key
	int m_nkey2;
	std::unique_ptr<CWnd> m_pScreen;
	std::unique_ptr<CWnd> m_pScreen2;

	int m_nHeight;
	CFont *m_pFont;

	std::unique_ptr<CfxImgButton> m_pBtnUp;

	HBITMAP		m_hBmpRoundPanel[4]{};
	
	BOOL		m_bBtnState;			//버튼의 상태
	BOOL		m_bCreated;

	CString m_strName;
	CString m_strName2;

// Operations
public:
	BOOL GetBtnState();
	void LoadMap2(CString strMapN, CString strMapN2, CString strName, CString strName2);
	void SetScreenParam(CString strParam, CString strData);
	int GetMapHeight();

protected:
	void CreateControl();
	void CloseMap();
	void SetBitmap();
	void Draw(CDC *dc, CRect mapRc);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllCtrl2)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDllCtrl2();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDllCtrl2)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

