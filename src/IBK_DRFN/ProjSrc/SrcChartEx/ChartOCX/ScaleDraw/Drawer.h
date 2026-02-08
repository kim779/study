// Drawer.h: interface for the Drawer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWER_H__EAA5BD80_7223_4855_8FC5_080B81065942__INCLUDED_)
#define AFX_DRAWER_H__EAA5BD80_7223_4855_8FC5_080B81065942__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxtempl.h"
#include "BlockBaseData.h"

class CPacket;

// 그리기의 기본
class CDrawer
{
// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;

public:
	CDrawer( HWND p_hOcxWnd, CChartInfo::CHART_MODE	p_eChartMode)
		{	m_hOcxWnd = p_hOcxWnd;	m_eChartMode = p_eChartMode;	}

protected:
	int GetPacketDataCount(CPacket* pPacket) const;
	CString GetPacketDataType(CPacket* pPacket) const;
	CString GetTextData(CPacket* pPacket, const int findIndex) const;
	bool GetDateData(CPacket* pPacket, const int findIndex, int& data) const;
	bool GetDateNoneRealData(CPacket* pPacket, const int findIndex, int& data) const;
	// data type를 자릿수 올림에 따라 변경한다.
	CString GetChangeDataType(const CString& orgDataType) const;

	// scale 한구간의 폭(onePtWidth)
	double GetOnePointWidth(const int length, const int dataCount) const;

	void DrawTextOut(CDC* pDC, const CPoint& point, const CString& data, const COLORREF color);
	void DrawTextOut(CDC* pDC, const CRect& rect, const CString& data, const COLORREF bkcolor, const COLORREF textcolor);
	void DrawGrid(CDC* pDC, const CScaleBaseData::SCALEGRIDTYPE scaleGridType,
		const CPoint& moveTo, const CPoint& lineTo, const COLORREF color, const int nWidth);
	void DrawLine(CDC* pDC, const CPoint& movePt, const CPoint& linePt);
	void DrawLine(CDC* pDC, const CPoint& movePt, const CPoint& linePt, 
		const COLORREF color, const int nWidth = 1, const int penStyle = PS_SOLID, const int penMode = R2_COPYPEN);

	// Find Index
	POSITION FindPosition(CPacket* pPacket, const int nIndex) const;

private:
	// text을 그리는데 필요한 Mode 설정.
	void SetDrawTextMode(CDC* pDC, const int bkMode, const COLORREF textColor);
	// line을 그리는데 필요한 Mode 설정.
	void SetDrawLineMode(CDC* pDC, CPen* pPen, const int rop2Mode);

	void DrawLine(CDC* pDC, const CScaleBaseData::SCALEGRIDTYPE scaleGridType, 
		const CPoint& movePt, const CPoint& linePt, const COLORREF color, const int nWidth);
	void DrawDotLine(CDC* pDC, const CPoint& movePt, const CPoint& linePt, const int drawLength, const int gapLength, int nLineWidth);
	void DrawPolyPolyLine(CDC* pDC, POINT *lppt, DWORD *lpdwPolyPoints, DWORD cCount, int nLineWidth);

	bool GetLengths(const CPoint& movePt, const CPoint& linePt, const int drawLength, const int gapLength, 
		int& nDrawLength_x, int& nDrawLength_y, int& nGapLength_x, int& nGapLength_y) const;
};

#endif // !defined(AFX_DRAWER_H__EAA5BD80_7223_4855_8FC5_080B81065942__INCLUDED_)
