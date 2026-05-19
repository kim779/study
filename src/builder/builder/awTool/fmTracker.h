#pragma once

class CObjectLoad;
class CChildView;
class CTrackerWnd;

class AFX_EXT_CLASS CfmTracker : public CObject
{
public:
	CfmTracker(CChildView* view, CObjectLoad* ObjLoad);
	virtual ~CfmTracker();
public:
	CChildView* m_view;
	int m_x, m_y, m_w, m_h;
	bool m_bFst;
	int m_nHit;
	CRect m_rc[8];
	CTrackerWnd* m_TrkWnd[8];
	CObjectLoad* m_ObjLoad;
	CArray <CTrackerWnd*, CTrackerWnd*> m_TkWnd;
	void SetPostion(int x, int y, int w, int h);
	void SetPostion();
	void MovePostion(CPoint point);
	void ResizePostion(int left, int top, int right, int bottom);
	void Init();
	void MoveTracker();
	void Hide();
	void Redraw();
	void SetHit(int nHit);
	void SetFst(bool bFst);
};