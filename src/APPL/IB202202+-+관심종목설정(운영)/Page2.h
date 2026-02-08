#pragma once
// Page2.h : header file
//
#include <afxtempl.h>
#include "colorBtn.h"

struct	_field {
	CString	text;
	CString	symb;
};

/////////////////////////////////////////////////////////////////////////////
// CPage2 dialog

class CPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2)

// Construction
public:
	CPage2(CWnd* parent = nullptr);
	~CPage2();

// Dialog Data
	//{{AFX_DATA(CPage2)
	enum { IDD = IDD_PAGE02 };
	CComboBox	m_fontname;
	CEdit		m_fieldnm;
	CComboBox	m_cbfield;
	CStatic	m_usermap;
	CSpinButtonCtrl	m_spin;
	CEdit		m_line;
	CColorBtn	m_cbtReal;
	CColorBtn	m_cbtBK103;
	CColorBtn	m_cbtBK102;
	CColorBtn	m_cbtBK101;
	CColorBtn   m_cbtBookmark;
	CColorBtn   m_cbtBookmarkPen;
	CComboBox	m_fontsize;
	CListCtrl	m_list2;
	CListCtrl	m_list1;
	//}}AFX_DATA

public:
	CWnd*		m_parent{};
	CString		m_root;
	CString		m_name;
	bool		m_activePage{};
	BOOL		m_bInterest{};
	int		m_nType{};
	int		m_bOthers{};
	CString		m_mapName;

	BOOL m_bSet{};				//¸Þ´º Å½»ö±â°¡ ²¨Á³´ÂÁö 

	CString		m_userM;

	bool		m_autoScroll;
	struct	_dragdata {		// drag item struct
		int	list{};
		int	item{};
		CString	code;
		CString	name;
	};
	_dragdata	m_dragData{};
	std::shared_ptr<CImageList>	m_dragImage{};
	int		m_activeno{};	// active field no
	CArray < _field, _field > m_fields;

	bool m_bAllBold{};

public:
	BOOL	IsMenuStatusClosed();
	void	initList();
	void	initField();
	void	initCombo();
	void	initSpin();
	bool	loadField(int fno);
	bool	loadConfig();
	void	saveField(int fno);
	void	saveConfig();
	void	setDefaultField(int fno);
	void	setDefaultColor();
	void	OnEndDrag(CPoint point);

	void	AppendItemToList2(CString text, CString symb, int nitem);
	void	DeleteItemToList2(int nitem);
	void	insertList1toList2(int sitem = -1);
	void	deleteList2();
	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);

	std::shared_ptr<CImageList> CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl);
	void	SetAutoScroll(CListCtrl* listctrl, int direct);

	void	OKaction();
	void	CCaction();
	CString GetRoot() { return m_root; }
	CString GetName() { return m_name; }

	void	CheckField(CString text);
protected:
	BOOL	AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	int	AddItem(CListCtrl *ctrl, LPCSTR string, int nitem, int nSubItem = -1);
	int	_httoi(const TCHAR *value);
	CString parseX(CString &srcstr, CString substr);
	CString Variant(int comm, CString data);
	COLORREF getIndexColor(int color);
	bool	GuideMsg(CString msg, bool question = false);
	CString	idTOstring(UINT id);
	COLORREF GetAxColor(int color);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetSize();
	bool IsWin8();
	// Generated message map functions
	//{{AFX_MSG(CPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAdd();
	afx_msg void OnDel();
	afx_msg void OnMoveup();
	afx_msg void OnMovedn();
	afx_msg void OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeCbfield();
	afx_msg void OnChkOverlap();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnSetting();
};

