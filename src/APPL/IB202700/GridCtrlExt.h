
#pragma once

#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

#include "grid/MemDC.h"
#include "grid/GridCtrl.h"

class CArrow
{
public:
	CCellID m_sid, m_eid;
	CPoint m_st, m_ed;
	CPoint m_poly[3];
	BOOL m_bmarket{};
public:
	explicit CArrow(int st_x, int st_y, int ed_x, int ed_y)	: m_st(st_x, st_y), m_ed(ed_x, ed_y) {}
	CArrow() {}
	int operator==(const CArrow& rhs) const { return (m_sid==rhs.m_sid && m_eid==rhs.m_eid && m_bmarket == rhs.m_bmarket); }
	int operator!=(const CArrow& rhs) const { return !(operator==(rhs)); }
};

class CGridCtrlExt : public CGridCtrl
{
public:
	CGridCtrlExt();
	virtual void  OnDraw(CDC* pDC);
private:
	//CArrow m_arrAvr;
protected:
	vector<CArrow> m_Arrows;
	COLORREF m_crArrow;
public:
	CArrow m_arrAvr;

	BOOL AddArrow(CArrow &ar);
	void SetAverage(CArrow ar);
	BOOL DelArrow(const CArrow &ar);
	void DrawArrow(const CArrow &ar, CDC *pDC=nullptr);
	void DrawStar(const CArrow &ar, CDC *pDC=nullptr);
	void DrawAverage(const CArrow &ar, CDC *pDC=nullptr);
	BOOL GetAbsCellRect(const CCellID &id, CRect &rc);
	void ClearAverage();
	virtual BOOL RedrawCell(int nRow, int nCol, CDC* pDC = nullptr);
    virtual BOOL RedrawCell(const CCellID& cell, CDC* pDC = nullptr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridCtrlExt)
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

public:
	virtual ~CGridCtrlExt();

protected:
	//{{AFX_MSG(CGridCtrlExt)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

