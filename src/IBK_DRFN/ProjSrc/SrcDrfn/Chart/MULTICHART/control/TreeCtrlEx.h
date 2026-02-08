/////////////////////////////////////////////////////////////
//	
//	Author:		Sami (M.ALSAMSAM), ittiger@ittiger.net
//
//	Filename:	TreeCtrlEx.h
//
//	http	 :	www.ittiger.net
//
//////////////////////////////////////////////////////////////
#if !defined(AFX_TREECTRLEX_H__5D969ED4_7DEA_4FB5_8C1D_E12D1CCF0989__INCLUDED_)
#define AFX_TREECTRLEX_H__5D969ED4_7DEA_4FB5_8C1D_E12D1CCF0989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////
class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)

public:
					CTreeCtrlEx();
	virtual			~CTreeCtrlEx();

	void			SetItemFont(HTREEITEM, LOGFONT&);
	void			SetItemBold(HTREEITEM, BOOL);
	void			SetItemColor(HTREEITEM, COLORREF);
	BOOL			GetItemFont(HTREEITEM, LOGFONT *);
	BOOL			GetItemBold(HTREEITEM);
	COLORREF		GetItemColor(HTREEITEM);
	void	SetHilightColor(COLORREF crText,COLORREF crBackground) 
	{ 		
		m_crTextColorSelected = crText;
		m_crBackColorSelected = crBackground;
		m_bUseUsrClr = TRUE;
	}

private:
	BOOL			m_bUseUsrClr;
	COLORREF		m_crBackColorSelected;
	COLORREF		m_crTextColorSelected;
protected:

	struct Color_Font {
		COLORREF color;
		LOGFONT  logfont;
	};

	CMap <void*, void*, Color_Font, Color_Font&> m_mapColorFont;

	protected:
	//{{AFX_MSG(CTreeCtrlEx)
		afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_TREECTRLEX_H__5D969ED4_7DEA_4FB5_8C1D_E12D1CCF0989__INCLUDED_)
