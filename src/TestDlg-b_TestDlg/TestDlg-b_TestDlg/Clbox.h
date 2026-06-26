#pragma once


// Clbox

class Clbox : public CListBox
{
	DECLARE_DYNAMIC(Clbox)

public:
	Clbox();
	virtual ~Clbox();

	CFont m_font{};
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


