#if !defined(AFX_EXTABCTRL_H__D5931E2F_30B5_11D5_941A_00001CD7EE63__INCLUDED_)
#define AFX_EXTABCTRL_H__D5931E2F_30B5_11D5_941A_00001CD7EE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

//#include "resource.h"

#define CLR_UNSEL	RGB(200, 200, 200)
#define	CLR_SEL		RGB(170, 170, 170)
#define	CLR_FONTUNSEL	RGB(0, 0, 0)
#define CLR_FONTSEL	RGB(0, 0, 0)


class AFX_EXT_CLASS CExTabCtrl : public CTabCtrl
{
public:
	CExTabCtrl();
	virtual ~CExTabCtrl();

	//{{AFX_VIRTUAL(CExTabCtrl)
	protected:
	virtual void PreSubclassWindow();
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CExTabCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	InitTab(CString* tName, CDialog** pDlg, UINT* idcX, int cnt);
	void	InitTabImage(CBitmap& bm);
	void	ChangeTab(int nIndex);
	void	resizeTab();
	void	SetOwnerDraw();
	void	SetColours(COLORREF clrUnSel, COLORREF clrSel, COLORREF clrBack = GetSysColor(COLOR_3DFACE));
private:
	CRect	newRect(int left, int right, int top, int bottom);

private:
	CDialog* m_pPage[5]{};
	int		m_nIndex{};

	COLORREF	m_clrUnSel;
	COLORREF	m_clrSel;
	COLORREF	m_clrFontUnSel;
	COLORREF	m_clrFontSel;
	COLORREF	m_clrBack;
	CImageList*	m_pImageList;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EXTABCTRL_H__D5931E2F_30B5_11D5_941A_00001CD7EE63__INCLUDED_)
