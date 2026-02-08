#define WM_CTRLMSG		WM_USER + 2002
#define CTRL_COLORCHANGE	0x00000001
#define CTRL_VALUECHANGE	0x00000002
#define CTRL_OTHER		0x00000004
#define CTRL_TREECLICK		0x00000008
#define CTRL_TREEDELETE		0x00000010
#define CTRL_TREECHECK		0x00000020
#define CTRL_LISTSELECT		0x00000040
#define CTRL_LISTDBCLICK	0x00000080
#define CTRL_COMBOSELECT	0x00000100
#define CTRL_COMBOCLOSE		0x00000200
#define CTRL_COMBOCHECK		0x00000400
#define CTRL_EDITCHANGE		0x00000800
#define CTRL_HDRCLICK		0x00001000
#define CTRL_LBUTTONUP		0x00002000
#include <afxtempl.h>
/********************************************************************/
// CBaseCombo class
/********************************************************************/
class CBaseCombo : public CComboBox
{
public:
	CBaseCombo();
	virtual ~CBaseCombo();
	virtual void DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem = true) {}
	HWND GetListBox() { return m_hListWnd; }
	BOOL EnableWindow(BOOL bEnable = TRUE);
	void SetShow(CWnd* pWnd) { m_pShowWnd = pWnd; }

	virtual DWORD GetData() { return 0; }
	virtual void SetData(DWORD nData) {}
	
public:
	//{{AFX_VIRTUAL(CBaseCombo)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBaseCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelChange();
	afx_msg void OnCloseup();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	virtual void init() {}
	virtual void selectOper() {}	
	
	int		m_nItemSize{};
	HWND		m_hListWnd{};
	
	// listbox subclassing class in combobox
	class CListWnd  : public CWnd
	{
	public:
		CListWnd() { m_pWnd = nullptr; }
		virtual ~CListWnd() { m_pWnd = nullptr; }
		void SetWindow(CWnd* pWnd) { m_pWnd = pWnd; }

	protected:
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
		{
			if (message == WM_LBUTTONUP)
			{
				if (m_pWnd)
					m_pWnd->SendMessage(message, wParam, lParam);				
			}
			return CWnd::WindowProc(message, wParam, lParam);
		}

		CWnd* m_pWnd{};
	};

	std::unique_ptr<CListWnd> m_pList;
	CWnd* m_pShowWnd{};
};


/********************************************************************/
// CLineKindCombo class
/********************************************************************/

class CLineKindCombo : public CBaseCombo
{
public:
	CLineKindCombo();
	virtual ~CLineKindCombo();
	void DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem = true);

public:
	DWORD	GetData();
	void	SetData(DWORD input);
	
	//{{AFX_VIRTUAL(CLineKindCombo)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLineKindCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()	

private:
	void	init();
	void	selectOper();

	int			m_nLine{};
	CArray<int, int>	m_nLines;
};

/********************************************************************/
// CLineWidthCombo class
/********************************************************************/

class CLineWidthCombo : public CBaseCombo
{
public:
	CLineWidthCombo();
	virtual ~CLineWidthCombo();
	void DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem = true);

public:
	DWORD	GetData();
	void	SetData(DWORD input);
	void	SetMax(int nMax = 5) { m_nItemSize = nMax; }
	//{{AFX_VIRTUAL(CLineWidthCombo)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CLineWidthCombo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	init();
	void	selectOper();
	
	int	m_nWidth{};
};

/*************************************************/
// CShowBox Class
/*************************************************/
class CShowBox : public CWnd
{
public:
	CShowBox(CWnd* pMain, CBaseCombo* pCombo);
	virtual ~CShowBox();

	void SetButton(bool bPush = false) { m_bLbutton = false; }
	void DrawBitmap(bool bPush);
	
public:
	//{{AFX_VIRTUAL(CShowBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CShowBox)
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
	void LoadBitmap(CDC *pDC);
	void DrawButton(CDC* pDC);
	void DrawFill(CDC* pDC);

	CBaseCombo* m_pCombo{};
	CRect		m_rcShow;
	CRect		m_rcButton;
	std::unique_ptr<CBitmap>	m_pBitmap;
	CWnd*		m_pMain{};
	bool		m_bLbutton{};
};


/********************************************************************/
// CBaseStatic class
/********************************************************************/
class CBaseStatic : public CStatic
{
public:
	CBaseStatic();
	virtual ~CBaseStatic();

	BOOL	EnableWindow(BOOL bEnable = TRUE);

public:
	//{{AFX_VIRTUAL(CBaseStatic)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);	
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBaseStatic)
	afx_msg void OnDestroy();	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	bool		m_bPreCreate{};

	virtual void init() {}
	virtual void destroyOper() {}
	virtual void setFocusOper(CWnd* pOldWnd) {}
};

/********************************************************************/
// CDrawStatic class
/********************************************************************/

class CDrawStatic : public CBaseStatic
{
public:
	CDrawStatic();
	virtual ~CDrawStatic();

public:
	//{{AFX_VIRTUAL(CDrawStatic)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDrawStatic)
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	virtual void init();
	virtual void destroyOper();
	virtual void setFocusOper(CWnd* pOldWnd);

	std::unique_ptr<CBaseCombo>	m_pCombo{};
	std::unique_ptr<CShowBox>	m_pShow{};
	UINT		m_ctrlID[2]{};
	
};

/********************************************************************/
// CWidthCtrl class
/********************************************************************/

class CWidthCtrl : public CDrawStatic
{
public:
	CWidthCtrl();
	virtual ~CWidthCtrl();	

public:
	int GetLineWidth();
	void SetLineWidth(int nWidth);

public:
	//{{AFX_VIRTUAL(CWidthCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CWidthCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()	
};

/********************************************************************/
// CKindCtrl class
/********************************************************************/

class CKindCtrl : public CDrawStatic
{
public:
	CKindCtrl();
	virtual ~CKindCtrl();	

public:
	int GetLine();
	void SetLine(int nKind);

public:
	//{{AFX_VIRTUAL(CKindCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CKindCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()	
};

