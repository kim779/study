// PnWndCtrl.h: interface for the CPnWndCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PNWNDCTRL_H__6EC2998D_E906_4025_8FF3_38B1826E62E2__INCLUDED_)
#define AFX_PNWNDCTRL_H__6EC2998D_E906_4025_8FF3_38B1826E62E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////
class CPnSlider : public CSliderCtrl
{
public:
	CPnSlider();
	virtual ~CPnSlider();

	//{{AFX_VIRTUAL(CPnSlider)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CPnSlider)
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////
class CPnScrollBar : public CScrollBar
{
public:
	CPnScrollBar();
	virtual ~CPnScrollBar();
	//{{AFX_VIRTUAL(CPnScrollBar)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CPnScrollBar)
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	Initialize(SCROLLINFO sInfo);
private:
	int	getPos(int type, int gap = -1);
};


#endif // !defined(AFX_PNWNDCTRL_H__6EC2998D_E906_4025_8FF3_38B1826E62E2__INCLUDED_)
