/************************************************************************/
/* FILENAME:    AxMBarCtrl.h                                            */
/* DESCRIPTION: CAxMBarCtrl class definetion file.                      */
/* WRITTEN:     macho@2006.11.06                                        */
/*                                                                      */
/************************************************************************/
#if !defined(AFX_AXMBARCTRL_H__13D536EC_504C_44C9_A5AD_3FD26CDA05C7__INCLUDED_)
#define AFX_AXMBARCTRL_H__13D536EC_504C_44C9_A5AD_3FD26CDA05C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.H>

class CAxMTabCtrl;
class CAxMButtonCtrl;
class CAxMListCtrl;
class CAxMTreeCtrl;
class CAxMSpeedButton;

#define MBAR_NONE		0x00
#define MBAR_BAR		0x01
#define MBAR_DLG		0x02

#define MBAR_TREE		0x01
#define MBAR_LIST		0x02
#define MBAR_BUTTON		0x03
#define MBAR_STATIC		0x04
#define MBAR_SPDBTN		0x05
#define MBAR_RADIO		0x06
#define MBAR_CHECK		0x07
#define MBAR_SPIN		0x08
#define MBAR_EDIT		0x09

#define MBARAL_NONE		0x00
#define MBARAL_TOP		0x01
#define MBARAL_CLIENT	0x02
#define MBARAL_BOTTOM	0x03

// Notify enent constants..
#define BARN_TABCHANGE	0x01
#define BARN_BTNCHANGE	0x02

typedef struct {
	CWnd*			cc;
	UINT			ccType;
	UINT			align;
	UINT			top, height;
	//UINT			heightKind;
	CAxMSpeedButton* btn;
} AXCONTROL, *PAXCONTROL;

typedef CTypedPtrList<CPtrList, PAXCONTROL>CControlList;

typedef struct {
	CControlList list;
	UINT		 conType; // MBAR_BAR or MBAR_DLG
} CONTROLLIST, *PCONTROLLIST;

typedef CTypedPtrList<CPtrList, PCONTROLLIST> CControlListContainer;

/////////////////////////////////////////////////////////////////////////////
// CAxMBarCtrl window

class AFX_EXT_CLASS CAxMBarCtrl : public CWnd
{
// Construction
public:
	CAxMBarCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxMBarCtrl)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxMBarCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxMBarCtrl)
	afx_msg LRESULT OnSetMessageHandler(WPARAM, LPARAM);
	afx_msg LRESULT OnGetMessageHandler(WPARAM, LPARAM);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	CAxMTabCtrl *m_tab;
	CControlListContainer m_cons;
	CFont	m_font;

	int		m_buttonHeight;
	UINT	m_idBase;
	UINT	m_buttonShadeID;
	COLORREF m_buttonColor;
	UINT	m_buttonColorPercent;
	CString m_imageFolder;

	void	InitializeControls();
	void	ReleaseControls();
	void	ResizeControls();
	void	ResizeControls(int cx, int cy);
	void	ResizeControl_BAR(CControlList& list, int cx, int cy);
	void	ResizeControl_DLG(CControlList& list, int cx, int cy);


	PCONTROLLIST GetControlList();
	void	OnCommandControl(UINT id, UINT notify, LPARAM lParam);
	CAxMSpeedButton* GetButton(UINT id);
	CWnd*	CreateContainControl(UINT ccType, LPCTSTR text = NULL);
	void	ResetButtons();
	
	UINT	GetAutoID() {
			return m_idBase++;
	}

	void	SetButtonBitmap(CAxMSpeedButton* btn, LPCTSTR bmpName);
	CAxMSpeedButton* m_currBtn;
public:
	// window functions
	CWnd*	Create(const CRect &rect, UINT id, CWnd* parent, DWORD style = WS_CHILD | WS_VISIBLE);
	void	SetImageFolder(LPCTSTR folder);

	// tab management
	int		AddTab(LPCTSTR tabName, LONG value = 0);
	void	DeleteTab(int index);
	int		GetTabIndex();
	void	SetTabIndex(int index);
	CString	GetTabString(int index);
	void	SetTabString(int index, LPCTSTR str);
	LONG	GetTabValue(int index);
	void	SetTabValue(int index, LONG value, PLONG oldValue = NULL);
	BOOL	IsValidTabIndex(int index);

	// button management
	int		GetButtonHeight();
	void	SetButtonHeight(int height);

	int		AddButton(LPCTSTR btnName, UINT containControlType, LONG value = 0);
	void	DeleteButton(int index);
	void	DeleteAllControls(CControlList* bl);
	int		GetButtonIndex();
	void	SetButtonIndex(int index);
	CString GetButtonString(int index);
	void	SetButtonString(int index, LPCTSTR str);
	LONG	GetButtonValue(int index);
	void	SetButtonValue(int index, LONG value);
	BOOL	IsValidButtonIndex(int index);

	CWnd*   GetButtonControl(int index);

	// top or bottom controls
	CWnd*	AddControl(int ctrlKind, int align, int top, LPCTSTR lpszHeight = "0", LPCTSTR lpszText = NULL);


	//
	void	Refresh();
	
	// Attributes
	void	SetButtonShade(UINT shadeID);
	UINT	GetButtonShade();

	void	SetButtonColor(COLORREF color);
	COLORREF GetButtonColor();

	void	SetButtonColorPercent(UINT percent);
	UINT	GetButtonCOlorPercent();

	int		GetTopPixel();
protected:
	CWnd*   m_messageHandler;
	LRESULT Notify(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
protected:
	CString m_paletteName;
public:
	void	SetMessageHandler(CWnd* wnd, BOOL setChild = FALSE);
	CWnd*   GetMessageHandler(CWnd* child = NULL);

	void	ChangePalette(LPCTSTR paletteName, COLORREF clrHeader);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXMBARCTRL_H__13D536EC_504C_44C9_A5AD_3FD26CDA05C7__INCLUDED_)





















