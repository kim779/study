#pragma once
// CodeReg.h : header file
//
#include "MapWnd.h"
#include "ControlWnd.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCodeReg window
struct _code
{
	CString code;
	CString name;
};

class CCodeReg : public CWnd
{
// Construction
public:
	CCodeReg(class CTool* pTool, bool bMenu);

public:

protected:
	bool		m_bLBDown;
	bool		m_bPopMenu;
	int		m_iRow;

	COLORREF	m_clrBG;
	COLORREF	m_clrDT;

	CPen*		m_ppenLine;
	CFont*		m_pfont;

	CPoint		m_ptPopMenu;



	int			m_iCellHeight, m_iCellTop;
	std::unique_ptr<class CfxImgButton>	m_pBtnHead;
	class CTool* m_pTool{};

	CArray <struct _code, struct _code>	m_arCode;

public:
	void		AddJCod(CString strJCode, CString strJName);
	void		Palette();

protected:
	// Init
	void		Init();

	void		DrawSiseText(CDC* pDC, CString strData, CRect rc, UINT nFormat = 0);
	void		LoadCode();
	void		SaveCode();

	void		SetCode(CString strCode);


// Operations
public:
	void ChangeFont(double* lRate, CFont* pfont, CFont* boldFont=NULL);

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeReg)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeReg();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeReg)
//	afx_msg LRESULT OnButtonHead();
	afx_msg void OnButtonHead();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuDelete();
	afx_msg void OnMenuDeleteall();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

