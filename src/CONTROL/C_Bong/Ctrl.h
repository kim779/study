// Ctrl.h: interface for the CCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRL_H__51CC0C70_4B0B_43C6_8968_C150BD8D3FD2__INCLUDED_)
#define AFX_CTRL_H__51CC0C70_4B0B_43C6_8968_C150BD8D3FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////////////////////////////////////////////////
class CExSlider : public CSliderCtrl
{
public:
	CExSlider();
	virtual ~CExSlider();

	//{{AFX_VIRTUAL(CExSlider)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExSlider)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////
class CExScrollBar : public CScrollBar
{
public:
	CExScrollBar();
	virtual ~CExScrollBar();
	//{{AFX_VIRTUAL(CExScrollBar)
	//}}AFX_VIRTUAL
protected:
	//{{AFX_MSG(CExScrollBar)
	afx_msg void HScroll(UINT nSBCode, UINT nPos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void	Initialize(SCROLLINFO sInfo);
private:
	int	getPos(int type, int gap = -1);
};



#endif // !defined(AFX_CTRL_H__51CC0C70_4B0B_43C6_8968_C150BD8D3FD2__INCLUDED_)
