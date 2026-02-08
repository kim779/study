#pragma once
// DllCtrl.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDllCtrl window

class CDllCtrl : public CWnd
{
// Construction
public:
	CDllCtrl(class CScreenWnd *pParent);

// Attributes
public:

protected:
	class CMapWnd *m_pMapWnd;
	class CScreenWnd *m_pScreenWnd;

	int m_nkey;				//load한 맵의 key
	std::unique_ptr<CWnd> m_pScreen;

	int m_nHeight;
	CFont *m_pFont;

	std::unique_ptr<CfxImgButton> m_pBtnUp;

	HBITMAP		m_hBmpRoundPanel[4]{};
	
	BOOL		m_bBtnState;			//버튼의 상태
	BOOL		m_bCreated;

	CString m_strName;

// Operations
public:
	BOOL GetBtnState();
	void LoadMap(CString strMapN, CString strName);
	void SetScreenParam(CString strParam, CString strData);
	int GetMapHeight();

protected:
	void CreateControl();
	void CloseMap();
	void SetBitmap();
	void Draw(CDC *dc, CRect mapRc);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDllCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDllCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

