#pragma once

class	CfmTracker;
class	CPointerWnd;

class AFX_EXT_CLASS CFmAlignBar
{
public:
	CFmAlignBar(class CChildView* view, class CFmCtrl* draw);
	virtual ~CFmAlignBar();
public:
	class CFmCtrl* m_FmCtrl;
	class CChildView* m_view;
	CRect		m_stdRc;
	int		m_iSpace;
public:
	void AlignObject(int iPos);
	CPoint GetAlignRect(int iPos, CRect& rc, bool bStd = false);
	void AlignLeft();
	void AlignRight();
	void AlignTop();
	void AlignBottom();
	void AlignHCenter();
	void AlignVCenter();
	void AlignWHCenter();
	void AlignWVCenter();
	void AlignUnionRect();
	void QualHSpace();
	void QualVSpace();
	bool CalcSpace(int iQual);
	void QualHSize();
	void QualVSize();
};