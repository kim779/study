#pragma once

class CSmartGLine;

class AFX_EXT_CLASS CSmartGLineMngr
{
public:
	CSmartGLineMngr();
	CSmartGLineMngr(CChildView* view);
	virtual ~CSmartGLineMngr();
public:
	class CChildView* m_view;
public:
	void CheckSmartGLine(int Sel, CRect rc, CRect ScreenRc);
	void Init();
	void DelSGLineWnd(int left, int right, int top, int bottom);
private:
	//CArray <CSmartGLine*, CSmartGLine*> m_SGLineArry;
	CSmartGLine* m_pSGLineLeft;
	CSmartGLine* m_pSGLineTop;
	CSmartGLine* m_pSGLineRight;
	CSmartGLine* m_pSGLineBottom;

	int m_iLeft;
	int m_iTop;
	int m_iRight;
	int m_iBottom;
private:
	void drawLineWnd(CSmartGLine** pSGLine, int lineXY, int DrawPos, int ObjPos, int WndPos);
	void DelAllGLine();
	void DelGLine(CSmartGLine** pSGLine);
};