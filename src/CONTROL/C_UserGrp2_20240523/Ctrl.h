#pragma once
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
