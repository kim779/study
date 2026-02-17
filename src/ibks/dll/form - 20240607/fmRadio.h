#if !defined(AFX_FMRADIO_H__4876C137_F8B2_4BB8_BBF7_A814C5E1B5BB__INCLUDED_)
#define AFX_FMRADIO_H__4876C137_F8B2_4BB8_BBF7_A814C5E1B5BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmRadio.h : header file
//

#include "fmbase.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CfmRadio command target

class AFX_EXT_CLASS CfmRadio : public CfmBase
{
	DECLARE_DYNCREATE(CfmRadio)

public:
	CfmRadio();	           // protected constructor used by dynamic creation
	CfmRadio(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmRadio();

// Attributes
protected :
	CRect		m_iRc;
	CRect		m_tRc;

	class	Cimage*	m_pic;
	CMap	< int, int, CBitmap*, CBitmap* > m_images;

// Operations
public:
	virtual	void	Draw(CDC* dc);
	virtual	void	OnLButton(bool down, CPoint pt, int& result);
	virtual	void	OnDblClick(CPoint pt, int& result);
	virtual	int	OnDrag(CPoint pt, int& result);
	virtual	void	SetFocus(bool focus);

	void	SetChecked(bool checked);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);

	void	Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmRadio)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual	int	keyArrowLeft(int& result);
	virtual	void	keyArrowRight(int& result);

	void	loadBitmap(bool radio = true);

	// Generated message map functions
	//{{AFX_MSG(CfmRadio)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmRadio)
	afx_msg BSTR _getText();
	afx_msg void _setText(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg BOOL _getChecked();
	afx_msg void _setChecked(BOOL bNewValue);
	afx_msg BOOL _getTabStop();
	afx_msg void _setTabStop(BOOL bNewValue);
	afx_msg BSTR _getCaption();
	afx_msg void _setCaption(LPCTSTR lpszNewValue);
	afx_msg void _SetFocus();
	afx_msg void _Refresh();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMRADIO_H__4876C137_F8B2_4BB8_BBF7_A814C5E1B5BB__INCLUDED_)
