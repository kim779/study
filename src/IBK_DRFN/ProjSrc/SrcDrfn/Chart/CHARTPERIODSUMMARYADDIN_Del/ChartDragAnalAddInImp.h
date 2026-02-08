// ChartDragAnalAddInImp.h: interface for the CChartDragAnalAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTDRAGANALADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
#define AFX_CHARTDRAGANALADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn/Include_AddIn.h"							// for Ixxxxxx.h

class CChartDragAnalAddInImp : public CChartAddInBase
{
public:
	CChartDragAnalAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CChartDragAnalAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;
	virtual BOOL InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	
// save point for inflate when mouse drag & drop
protected:
	CPoint	m_ptStartDragDrop;
	CPoint	m_ptCurrDragDrop;
	CPoint	m_ptStart, m_ptEnd;
	long	m_lStartDate, m_lEndDate;
	int		m_nStartIndex, m_nEndIndex;

// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
protected:
	DWORD	m_dwMouseActionID;

// (2006/2/6 - Seung-Won, Bae) Draw Trace Rectangle
protected:
	CRect DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptCurr, int nWeight);

// ( 2006 / 11 / 27 - Sang-Woo, Cho ) 
	BOOL	m_bRun;

protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

protected:
	virtual void	OnDrawAfterGraph( HDC p_hDC);		// 차트보다 앞 쪽에 그릴 때
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual void OnPacketData( const char *p_szPacketNames, BOOL p_bWithReset);	// 데이터가 들어 올 때 호출됨
	virtual void OnResetChart( void);	// 차트가 리셋될 때 호출됨
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	virtual void	OnUpdatePacketDataMultiItem( LPCTSTR lpszRQ, BOOL p_bTrUpdate);

protected:
	CRect	DrawFill(CDC *pDC, const CPoint &Startpt, const CPoint &Endpt, const CRect& drawingRegion);
	void	SelectClipRegion(CDC* pDC, const CRect& rect);
	void	SelectClipRegion(CDC* pDC);
	BOOL	GetChartBasicData(int &p_nStartIndex, int &p_nEndIndex, int &p_nR, int &p_nC, CRect &p_rctBlockRegion, CRect &p_rctGraphRegion);
	int		GetIndexFromPoint(int x, int nStartIndex, int nEndIndex, CRect rctBlockRegion);
	int		GetIndexFromValue(CString strTime, int nStartIndex, int nEndIndex);
	int		GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion);
	BOOL	m_bLog;
	BOOL	m_bReverse;
	double	m_dViewMin, m_dViewMax;
	CList<CString, CString&>		m_listDataWindow;
	BOOL AddData(int nStart, int nEnd);
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);	// 스트링의 추출
};

#endif // !defined(AFX_CHARTDRAGANALADDINIMP_H__DA9542DB_C2AE_4A00_8CA5_CE7334AB74E4__INCLUDED_)
