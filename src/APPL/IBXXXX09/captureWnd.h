#pragma once

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxButton.h"
#include "../../control/fx_misc/memdc.h"
#include "../../control/fx_grid/fxGrid.h"

#define	WM_CAPTURE	(WM_USER + 500)
#define	wpCONFIRM	1

class CCaptureWnd : public CWnd
{
// Construction
public:
	CCaptureWnd(CWnd* parent, CFont* font, CFont* bfont);

// Attributes
public:
	BOOL	CreatePopup(int key, const void* info, void* data, int wndpos = 0);

protected:
	void	initialControl();
	void	displayData(const void* info, void* data);
	void	positionWindow();

	CString idTOstring(UINT id);
	CString chTOstring(char* chB, int chL);
	CString parseX(CString &srcstr, CString substr);

// Operations
public:
	CWnd* m_parent{};
	CFont* m_font{};
	CFont* m_bfont{};
	int		m_key{};
	CString		m_data;
	enum _wndpos { wpCENTER = 0, wpLT = 1, wpRT = 2, wpLB = 3, wpRB = 4 } m_wndpos{};

	CRect		m_winRC, m_titRC, m_boxRC, m_gridRC;

	CfxStatic	m_code;
	CfxStatic	m_name;
	CfxButton	m_ok;

	std::unique_ptr<CfxGrid> m_grid{};

	CBrush		m_boxBrush;

	CWnd* m_pWizard{};
	CString Variant(int comm, CString data);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCaptureWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCaptureWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCaptureWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


