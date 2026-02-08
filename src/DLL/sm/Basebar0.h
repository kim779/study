#if !defined(AFX_BASEBAR0_H__046BAACA_9766_4969_836E_1DC60CBF36E4__INCLUDED_)
#define AFX_BASEBAR0_H__046BAACA_9766_4969_836E_1DC60CBF36E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Basebar0.h : header file
//
#include <afxtempl.h>
#include "usedefine.h"
#define	VARWIDTH	200
#define	BANDGAP		4
#define	BANDHEIGHT	24
#define	CONTROLH	110

#define	CTRL_CLOSE	1000
#define	CTRL_GROUP	1001
#define	CTRL_CODE	1002
#define	CTRL_MAPADD	1003
#define	CTRL_MAP	1004
#define CTRL_CATEGORY	1005
#define CTRL_REQUESTGROUP 1006
#define CTRL_REQUESTGROUPCODE 1007

#define	TM_CON		99

/////////////////////////////////////////////////////////////////////////////
// CBasebar0 dialog
#include "resource.h"
class CBasebar0 : public CDialog
{
// Construction
public:
	CBasebar0(CWnd* parent, bool (*callback)(int, WPARAM, LPARAM), CWnd* pParent = NULL);   // standard constructor
	~CBasebar0();

// Dialog Data
	//{{AFX_DATA(CBasebar0)
	enum { IDD = IDD_DBAR0 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasebar0)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
// Attributes
protected:
	CWnd* m_parent{};	// mainframe
	CString		m_code;
	CString		m_mapN;

	CFont		m_font;
	CRect		m_tRc[2];

	COLORREF	m_bk;
	COLORREF	m_color{};
	CListCtrl	m_codeList;
	CListCtrl	m_mapList;

#ifdef DF_USESTL_CBASEBAR
	std::unique_ptr< class CRunX> m_runX;
	std::unique_ptr< class CNButton> m_codeS;
	std::unique_ptr<class CNButton>m_mapA;
	std::unique_ptr <class CNButton> m_mapS;
	std::unique_ptr <class CSEdit> m_screen;
#else
	class CRunX* m_runX;
	class CNButton* m_codeS;
	class CNButton* m_mapA;
	class CNButton* m_mapS;
	class CSEdit* m_screen;
#endif

	class CTMenu* m_menu{};
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
	CStringArray	m_arMap;
// Operations
public:
	void	make_Ctrl();
	void	change_BandInfo();
	void	set_MenuInfo(class CTMenu* menu);
	void	reload_Code(bool first = false);
	void	Append_Map(CString dispN);

#ifdef DF_USESTL_CBASEBAR
	CWnd* GetCategoryWnd() { return (CWnd*)m_codeS.get(); }
#else
	CWnd* GetCategoryWnd() { return (CWnd*)m_codeS; }
#endif
	CString	GetCode();
	CString	GetMapN();

#ifdef DF_USESTL_CBASEBAR
	std::unique_ptr<class CCategoryPopup> m_categoryPopup;
#else
	class CCategoryPopup* m_categoryPopup;
#endif
protected:
	void	draw_Band(CDC* pDC = NULL);
	void	change_Position();
	void	fit(CListCtrl& list);
	BOOL	addColumn(CListCtrl& list, LPCTSTR columns, int col, int width = 0);
	BOOL	addItem(CListCtrl& list, CString items, int item, int subi = 0);

	void	reload_Map();
	void	enable_Button(int kind);

	void	Prev();
	void	Next();
	void	Play();
	void	Pause();
	void	Continue();
	void	Stop();
	void	ChangeMap(bool code = false);
	int	GetOrder();
	int	GetKind();
	int	GetInterval();

	COLORREF GetBkColor();



	void	SelectCode();
	void	OnSelectCode(const char* data);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBasebar0)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEBAR0_H__046BAACA_9766_4969_836E_1DC60CBF36E4__INCLUDED_)
