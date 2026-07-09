#if !defined(AFX_SHOW_H__424346AA_887C_4EC8_9155_0A9BECCAD380__INCLUDED_)
#define AFX_SHOW_H__424346AA_887C_4EC8_9155_0A9BECCAD380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// show.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CShow object

class AFX_EXT_CLASS CShow : public CObject
{
	DECLARE_DYNCREATE(CShow)

	CShow()	{}		// protected constructor used by dynamic creation
 	CShow(CWnd* pWnd, class CAxisPalette* palette, class CAxisDraw* draw,
				CRect rect, CString root = _T(""), CString fileN = _T(""));
	virtual ~CShow();

// Attributes
public:
	bool	m_load;
	CRect	m_rect;

protected:
	CString	m_root;
	CString	m_fileN;
	CWnd*	m_view;
	CAxisPalette*	m_palette;
	CAxisDraw*	m_draw;

	class	CAxisForm*	m_form;
	CMap	<int, int, CShow*, CShow*>	m_forms;

// Operations
public:
	bool	Load(CString fileN, int flag = -1);
	void	Draw(CDC* dc);
	void	Resize(CRect rect);
	void	Resize(CRect rect, float hRatio, float vRatio);
	CSize	GetSize();
	CString	GetTitle();

protected:
	void	removeForms();
	CString	getPath(CString mapN);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOW_H__424346AA_887C_4EC8_9155_0A9BECCAD380__INCLUDED_)
