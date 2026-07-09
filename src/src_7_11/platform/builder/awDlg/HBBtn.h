// HBBtn.h: interface for the CHBBtn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HBBTN_H__5D3381AC_2C80_4224_85DF_13D97492232F__INCLUDED_)
#define AFX_HBBTN_H__5D3381AC_2C80_4224_85DF_13D97492232F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AFX_EXT_CLASS CHBBtn : public CBitmapButton  
{
public:
	CHBBtn();
	virtual ~CHBBtn();

	BOOL	m_bTracking, m_bHover;
	void DrawItem(LPDRAWITEMSTRUCT lpDIS);
protected:
	//{{AFX_MSG(CHBBtn)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_HBBTN_H__5D3381AC_2C80_4224_85DF_13D97492232F__INCLUDED_)
