#if !defined(AFX_CONTROLS_H__76AED34D_491A_4A52_A685_9BB0FB5187C4__INCLUDED_)
#define AFX_CONTROLS_H__76AED34D_491A_4A52_A685_9BB0FB5187C4__INCLUDED_

/********************************************************************/
// CGroupCtrl class
/********************************************************************/

#define WM_CTRLMSG	WM_USER + 2002



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

#define WM_FORM		WM_USER + 2003

#include <afxtempl.h>


class CGroupCtrl : public CButton
{
public:
	CGroupCtrl();
	virtual ~CGroupCtrl();

public:	// interface
	BOOL	ShowWindow(int nCmdShow);
	BOOL	EnableWindow(BOOL bEnable = TRUE);

public:
	//{{AFX_VIRTUAL(CGroupCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CGroupCtrl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	void regMember();
	
	bool			m_bInit;
	CArray<HWND, HWND>	m_arMembers;	
	
};

/********************************************************************/
// CAxNumEdit class
/********************************************************************/

class CAxNumEdit : public CEdit
{
public:
	CAxNumEdit();
	virtual ~CAxNumEdit();	
public:
	// public interface
	double GetValue();
	void SetValue (double val);
public:
	//{{AFX_VIRTUAL(CAxNumEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAxNumEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/********************************************************************/
// CaxdoubleEdit class
/********************************************************************/
class CaxdoubleEdit : public CAxNumEdit
{
public:
	CaxdoubleEdit();
	virtual ~CaxdoubleEdit();
public:
	//{{AFX_VIRTUAL(CaxdoubleEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CaxdoubleEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

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
	
	int	m_CurSel;

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
	
	int	m_nItemSize;
	HWND	m_hListWnd;
	
	// listbox subclassing class in combobox
	class CListWnd  : public CWnd
	{
	public:
		CListWnd() { m_pWnd = NULL; }
		virtual ~CListWnd() { m_pWnd = NULL; }
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

		CWnd*	m_pWnd;
	};

	std::unique_ptr <CListWnd>	m_pList;
	CWnd*		m_pShowWnd;	
};

/********************************************************************/
// CColorCombo class
/********************************************************************/

class CColorCombo : public CBaseCombo
{
public:
	CColorCombo();
	virtual ~CColorCombo();
	void DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem = true);

protected:
	COLORREF			m_color;
	CArray<COLORREF, COLORREF>	m_colors;

public:
	DWORD GetData() { return (DWORD)m_color; }
	void SetData(DWORD input);

	//{{AFX_VIRTUAL(CColorCombo)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CColorCombo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void	init();
	void	selectOper();		
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

	int			m_nLine;
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
	
	int	m_nWidth;
};

/********************************************************************/
// CCheckCombo class
/********************************************************************/

class CCheckCombo : public CBaseCombo
{
public:
	CCheckCombo();
	virtual ~CCheckCombo();	
	void DrawContent(HDC hDC, UINT itemID, RECT* rcItem, UINT itemAction, UINT itemState, bool bDrawItem = true);
	int	GetData(int nIndex);
	void	SetCheck(bool bCheck, int nIndex = -1);
	bool	GetCheck(int nIndex = -1);	
	int	GetIndex(int nData);
	void	AddItem(CString szContent, int nValue, bool nCheck);
	void	RemoveAll() { CComboBox::ResetContent(), m_arData.RemoveAll(); }

public:

	//{{AFX_VIRTUAL(CCheckCombo)
	protected:
	virtual LRESULT	WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCheckCombo)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()		

private:
	DWORD	getData(DWORD dword) { return 0x00FFFFFF & dword; }
	DWORD	getCheck(DWORD dword) { return (HIBYTE(HIWORD(dword))); }
	int	drawCheck(CDC* pDC, CRect rect, int Check);
	void	init();
	void	selectOper();

	CArray<CString, CString>	m_arData;
	int				m_nWidth;	
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
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void LoadBitmap(CDC *pDC);
	void DrawButton(CDC* pDC);
	void DrawFill(CDC* pDC);

	CBaseCombo*	m_pCombo;
	CRect		m_rcShow;
	CRect		m_rcButton;
	std::unique_ptr <CBitmap>	m_pBitmap;
	CWnd*		m_pMain;
	bool		m_bLbutton;
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

	bool		m_bPreCreate;

	virtual void init() {}
	virtual void destroyOper() {}
	virtual void setFocusOper(CWnd* pOldWnd) {}
};

/********************************************************************/
// CSpinCtrl class
/********************************************************************/

class CSpinCtrl : public CBaseStatic
{
public:
	CSpinCtrl();
	virtual ~CSpinCtrl();
public:

	//{{AFX_VIRTUAL(CSpinCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:	
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSpinCtrl)	
	afx_msg void OnChangeEdit();	
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

	virtual void init();
	virtual void destroyOper();
	virtual void setFocusOper(CWnd* pOldWnd);
public:
	void SetMinMax(int nMin = -1, int nMax = -1);	
	
	inline	BOOL CheckValue(int nValue)
	{
		if (nValue < m_nMin || nValue > m_nMax)
			return FALSE;
		return TRUE;
	}

	inline UINT GetEditID() { return m_editID;}
	inline UINT GetSpinID() { return m_spinID;}
	void ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
	void SetNumber(int nNum);
	int GetNumber();	
private:
	int	m_nMin;
	int	m_nMax;
	
	bool			m_bPreCreate;
	std::unique_ptr <CAxNumEdit>	m_pEdit;
	std::unique_ptr <CSpinButtonCtrl> m_pSpinCtrl;
	UINT			m_editID;
	UINT			m_spinID;
	
	CFont			m_ftDefault;
	CWnd*			m_pOld;
	bool			m_bSetWindowText;
	
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
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

	virtual void init();
	virtual void destroyOper();
	virtual void setFocusOper(CWnd* pOldWnd);

	std::unique_ptr <CBaseCombo>	m_pCombo;
	std::unique_ptr <CShowBox>	m_pShow;
	UINT	m_ctrlID[2];
};


/********************************************************************/
// CColorCtrl class
/********************************************************************/

class CColorCtrl : public CDrawStatic
{
public:
	CColorCtrl();
	virtual ~CColorCtrl();	

public:
	COLORREF GetColor();
	void SetColor(COLORREF color);

public:
	//{{AFX_VIRTUAL(CColorCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CColorCtrl)	
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
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


/********************************************************************/
// CCheckCBCtrl class
/********************************************************************/


class CCheckCBCtrl : public CDrawStatic
{
public:
	CCheckCBCtrl();
	virtual ~CCheckCBCtrl();	

public:
	//{{AFX_VIRTUAL(CCheckCBCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCheckCBCtrl)
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	int     GetData(int nIndex) { return ((CCheckCombo*)m_pCombo.get())->GetData(nIndex); }
	void    AddItem(CString szContent, int nValue, bool nCheck) { ((CCheckCombo*)m_pCombo.get())->AddItem(szContent, nValue, nCheck); }
	void	ResetContent() { ((CCheckCombo*)m_pCombo.get())->ResetContent(); m_pShow->DrawBitmap(false); }
	void    SetCheck(bool bCheck, int nIndex = -1);
	bool    GetCheck(int nIndex = -1) { return ((CCheckCombo*)m_pCombo.get())->GetCheck(nIndex); }
	int	GetCount() { return m_pCombo->GetCount(); }
	int	GetIndex(int nData) { return ((CCheckCombo*)m_pCombo.get())->GetIndex(nData); }
	int	GetCurSel() { return m_pCombo->GetCurSel(); }
	void	SetCurSel(int nSelect) { m_pCombo->SetCurSel(nSelect); m_pShow->DrawBitmap(false); }
};



//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_CONTROLS_H__76AED34D_491A_4A52_A685_9BB0FB5187C4__INCLUDED_)
