#pragma once

// CNewsPackObj 명령 대상입니다.
#define N_POINT		0	// No point
#define S_POINT		1	// Start point
#define E_POINT		2	// End point
#define M_POINT		3	// Mid point

#define RC_HEIGHT	26
#define RC_WIDTH	26

class CNewsPackObj : public CObject
{
public:
	CNewsPackObj();
	virtual ~CNewsPackObj();
public:
	void DrawEndPoints(CDC* pDC);
	BOOL m_bSelected;
	int m_Num;
	CDC m_dcMem;
	CRect m_rc;
	CRect m_oldrc;
	CPoint m_ptOffset;
	CRect m_rcDraw;
	int m_CurClickedPosition;
	CPoint m_ptStart;
	CPoint m_ptEnd;

	BOOL IsInRegion(CPoint pt);
	void Initilize(void);
	void ButtonDownAt(const CPoint& pt);
	void ButtonUpAt(const CPoint& pt);
	CRect GetRect();
	void SetPos(CPoint pt, int type);
	CPoint GetPos(int type);
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);
private:
	CPoint m_ptAt;
	CDC m_dcTransMem;
	CBitmap m_bitmap;
	CBitmap* m_pOldBmp;
	CBitmap m_bitmapTrans;
	CBitmap* m_pOldBitmapTrans;
	CSize m_size;

	void ReCalcRect();
	BOOL IsOnNewsPack(CPoint pt);
	BOOL IsOnEndPoint(CPoint pt);
};

class CNewsPackList : public CObList
{
public:
	int GetNewsPackCount(){return GetCount();}
	void DeleteAll();
	void AddHeadAgent(CNewsPackObj* pNPbject/*IN*/)
				{ AddHead(pNPbject); }
	void SetAtNewsPack(POSITION pos/*IN*/, CNewsPackObj* pNPbject/*IN*/)
				{ SetAt(pos, pNPbject);}
	CNewsPackObj* GetHeadAgent()
				{ return (CNewsPackObj*)GetHead(); }
	CNewsPackObj* GetNextNewsPack(POSITION& pos/*IN*/)
				{ return (CNewsPackObj*)GetNext(pos); }
	CNewsPackObj* GetPrevNewsPack(POSITION& pos/*IN*/)
				{ return (CNewsPackObj*)GetPrev(pos); }

};