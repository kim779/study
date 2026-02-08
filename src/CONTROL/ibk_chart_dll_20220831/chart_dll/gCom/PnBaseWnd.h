#if !defined(AFX_PNBASEWND_H__1C498742_8816_4EFD_BC44_14DE111029D6__INCLUDED_)
#define AFX_PNBASEWND_H__1C498742_8816_4EFD_BC44_14DE111029D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnBaseWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnBaseWnd window
#include "afxtempl.h"

class CPnBaseWnd : public CWnd
{
// Construction
public:
	CPnBaseWnd(CWnd *pwndView, CWnd* pwndGView, CFont* pFont, int ctrlID, char* info);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnBaseWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnBaseWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnBaseWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetCtrlID(int ctrlID) { m_ctrlID = ctrlID; }
	int	GetCtlrID() { return m_ctrlID; }

	void		SetBackColor(COLORREF clrBack)	{ m_clrBack = clrBack; }
	COLORREF	GetBackColor() { return m_clrBack; }
	void		SetFrameColor(COLORREF clrFrame){ m_clrFrame = clrFrame; }
	COLORREF	GetFrameColor() { return m_clrFrame; }

protected:
	class CAxisGComApp	*m_pApp;
	CWnd*		m_pwndView;
	CWnd*		m_pwndGView;
	CFont		*m_pFont;
	
	int		m_ctrlID;	
	COLORREF	m_clrBack;
	COLORREF	m_clrFrame;
	COLORREF	m_clrInBack;

	// mouse move
	bool		m_bLButton;
	int		m_iMoveID;
	int		m_iSelID;
	bool		m_bCapture;

	CArray <_comp *, _comp *>	m_arComs;
	CString		m_comment;

	COLORREF	m_crColourBk;
	int		m_wdLine;
	enum	{ dtButton = 1, dtColor, dtWidth, dtEtc, dtSeparator };

protected:
	struct _comp*	SearchCom(CPoint pt);
	struct _comp*	SearchCom(int ID);
	CString	GetPosComment(CPoint pt);
	void	DeleteComs();

	void	InvalidateCom(int idc, int status);
	void	InvalidateCom(struct _comp *comp, int status);

	void	DrawBackground(CDC* pDC);
	void	DrawFrame(CDC* pDC);
	void	DrawButton(CDC* pDC, int dtype, struct _comp *comp, CImageList* m_pImage = NULL);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNBASEWND_H__1C498742_8816_4EFD_BC44_14DE111029D6__INCLUDED_)
