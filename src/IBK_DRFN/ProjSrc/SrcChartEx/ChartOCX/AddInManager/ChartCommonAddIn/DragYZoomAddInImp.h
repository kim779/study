// DragYZoomAddInImp.h: interface for the CDragYZoomAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAGYZOOMADDINIMP_H__D0430341_82F9_4DC9_A154_9409F3148624__INCLUDED_)
#define AFX_DRAGYZOOMADDINIMP_H__D0430341_82F9_4DC9_A154_9409F3148624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../Include_Addin/I000000/_ChartAddInBase.h"				// for CChartAddInBase


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 21
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CDragYZoomAddInImp : public CChartAddInBase  
{
public:
	CDragYZoomAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CDragYZoomAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// save point for inflate when mouse drag & drop
protected:
	CPoint	m_ptStartDragDrop;
	CPoint	m_ptCurrDragDrop;
	CRect	m_rcPrevDrag;

	// (2006/2/6 - Seung-Won, Bae) Manage Mouse Action ID for Serial Action.
protected:
	DWORD	m_dwMouseActionID;

	BOOL	m_bRun;

	int		m_nVertScaleGroupIndex;
	CRect	m_rcGraphRegion;

	// (2006/2/6 - Seung-Won, Bae) Draw Trace Rectangle
protected:
	BOOL DrawDragRect(CDC * pdc, CPoint ptFirst, CPoint ptPrev, CPoint ptCurr, int nWeight, BOOL bMouseMove = TRUE);

	// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawAfterGraph( HDC p_hDC);

	virtual void OnToolCommand( const int p_nToolType, const int p_nCmdOption );
		
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_DRAGYZOOMADDINIMP_H__D0430341_82F9_4DC9_A154_9409F3148624__INCLUDED_)
