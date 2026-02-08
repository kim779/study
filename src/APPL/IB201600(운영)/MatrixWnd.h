#pragma once
// MatrixWnd.h : header file
//

#include "sharemsg.h"

/////////////////////////////////////////////////////////////////////////////
// CMatrixWnd window
class CMainWnd;

class CMatrixWnd : public CWnd
{
// Construction
public:
	CMatrixWnd(CMainWnd* parent, CWnd* mainWnd);

// Attributes
	CMainWnd* m_pParent{};
	CWnd* m_pMainWnd{};
	
	int			m_iRows{};
	int			m_iCols{};
	BOOL			m_aryMatrix[MATRIX_MAXCOL][MATRIX_MAXROW]{};
//	CBrush			m_bkBrush;
	CString			m_root;
	CString			m_name;
	CString			m_user;
	CString			m_id;
	CString			m_home;
	CFont*			m_font{};
	CString			m_strPal;

	COLORREF		m_clrPallete{};
	char			m_key{};
	CString			m_strParamName, m_strRealCode;
	CFont* m_fontB{};
	int				m_fontsize{};
	COLORREF		m_clrLineColor{}, m_clrBgColor{}, m_clrDataFgColor{}, m_clrLblFgColor{};
	COLORREF		m_clrColor[10]{};
	int				m_clrColorIdx[10]{};
	
	CEdit* m_editRow{};
	CEdit* m_editCol{};
	CButton*		m_btnPlay{};
	std::unique_ptr<CStatic>	m_labelClose;
public:

// Operations
	void			SendMatrixInfo(bool result);	
	void			initMatrix();
	int			GetRows();
	int			GetCols();
	void			SetRows(int row);
	void			SetCols(int col);
	void			GetRowColFromPoint(CPoint point, int row, int col);
	CString			Variant(int comm, CString data = "");
	CString			GetPalette();
	CBrush*			GetAxBrush(COLORREF clr);
	COLORREF		GetColor(int color);
//	CFont*			getAxFont(struct _fontR* font);
	CFont*			getAxFont(CString fName, int point=9, bool bBold=false, bool bItalic=false);
//	CBitmap*		getAxBitmap(CString path);
	LRESULT			SendAxisMessage(WPARAM wParam, LPARAM lPAram);

	void		    Draw(CDC *pDC, CRect rcClient);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrixWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMatrixWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMatrixWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMatrixWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

