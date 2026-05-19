#pragma once

class	CfmTracker;
class	CPointerWnd;
class	CChildView;
class	CSmartGLineMngr;

#define	TM_DRAG		100

class AFX_EXT_CLASS CFmCtrl : public CObject
{
public:
	CFmCtrl()	{}
	CFmCtrl(CChildView* view, class CAxisDraw* draw);
	virtual ~CFmCtrl();

	CDC*	m_dc;

	CChildView* m_view;
	CPoint	m_startPt, m_toPt;
	CPoint	Pt;
	CRect	m_FstRc;
	CRect	m_FormRc;
	CString m_selTool;		// 선택된 오브젝트명

	CString m_strTip;		// 좌표값 툴팁
	POINT	m_pt;			// 마우스 좌표
	int		m_nGap;
	int		m_iDownEv;
	int		m_iMoveEv;
	BOOL		m_bShiftKey;

public:
	class	CAxisDraw*	m_draw;
	CArray <CPointerWnd*, CPointerWnd*> m_fmWndArry;		// 선택박스
	CArray <CfmTracker*, CfmTracker*> m_fmTkArry;			// Tracker
	class	CMarkWnd* m_markWnd;
	CSmartGLineMngr*  m_pSmartGLineMngr;

	int	m_bObjSel;
public:
	void	SetName(CString name);
	void	MouseDown(CPoint point);
	void	MouseMove(CPoint point);
	void	MouseUp();
	void	SetTracker(int idx);
	void	SelObj(int idx, bool bClear = true);
	void	SelAddObj(int idx, bool bMulti);
	void	ReDrawTracker();
	void	MoveObj(CRect rc);
	void	LayoutMoveObj();
	void	SetObjRect(int left, int top, int right, int bottom);
	void	InvalidateObject();
	void	SetGap(int nGap) { m_nGap = nGap; }
	void	ShowVScroll(class CformProp* Prop);

public:
	CArray  <CfmTracker*, CfmTracker*> *GetTkArray();
	int	GetSelCount()	{ return m_fmTkArry.GetCount(); };
	void	SetGroup();
	void	ReleaseGroup();
	BOOL 	ObjSelecte(CPoint point);
	void	SetObjRect(class CObjectLoad* ObjLoad, CRect rc);
	void	MouseDrag();
protected:
	void	MoveRect(CRect& rc, CPoint point);
};