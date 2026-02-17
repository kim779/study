#if !defined(AFX_FMPANEL_H__000DCAFE_0063_430C_BEA5_17E482ACC993__INCLUDED_)
#define AFX_FMPANEL_H__000DCAFE_0063_430C_BEA5_17E482ACC993__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmPanel.h : header file
//

#include <afxtempl.h>
#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmPanel command target

class AFX_EXT_CLASS CfmPanel : public CfmBase
{
	DECLARE_DYNCREATE(CfmPanel)

public:
	CfmPanel();	           // protected constructor used by dynamic creation
	CfmPanel(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmPanel();

// Attributes
protected :
	class	Cimage*	m_pic;
	int	m_image;
	CMap	< int, int, CBitmap*, CBitmap* > m_images;

// GO_FIX : Resizable
	int	m_bottom_gap;
	int	m_right_gap;

// Operations
public:
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);

	void	SetEnum(CString text, int value, int func = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmPanel)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	drawRound(CDC* dc);
	void	loadBitmap(CString theme = _T(""));

	// Generated message map functions
	//{{AFX_MSG(CfmPanel)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmPanel)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg void _Refresh();
	afx_msg void _BlinkText(BOOL set, long color);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMPANEL_H__000DCAFE_0063_430C_BEA5_17E482ACC993__INCLUDED_)
