// MultiChartDataViewAddInImp.h: interface for the CMultiChartDataViewAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTICHARTDATAVIEWADDINIMP_H__6CA19E9D_E870_483A_8353_C1C020E6E164__INCLUDED_)
#define AFX_MULTICHARTDATAVIEWADDINIMP_H__6CA19E9D_E870_483A_8353_C1C020E6E164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../Include_Chart/OutsideCommonInterfaceFunction.h"		// for COutsideCommonInterfaceFunction
#include "../Include_Addin/I000000/_ChartAddInBase.h"				// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/7/27
// Comments		: MultiChart Data View Data Maker
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CMultiChartDataViewAddInImp : public CChartAddInBase
{
public:
	CMultiChartDataViewAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CMultiChartDataViewAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;


// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/9/25 - Seung-Won, Bae) On User IndicatorInfo Loaded
	virtual void	OnUserIndiatorConditionLoaded( void);
	// (2006/9/25 - Seung-Won, Bae) On Graph Added.
	virtual void	OnManualRealData( const char *p_szPacketName, const double p_dData, const int p_nOption);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB


// 종찬(05/07/26) DataWindow
private:
	COutsideCommonInterfaceFunction m_OutsideCommonInterfaceFunction;
	CList<CString, CString&>		m_listDataWindow;
public:
	int		GetViewDataIndex(CPoint ptMouse);
	int		GetViewEndDataIndex(const bool bData);
	int		GetDataWindowEndIndex(const int nDataIndex, int& nHighlightIndex);
	void	SetDataWindowData(const char *szRQ, CPoint point);	// 데이타창의 데이타를 구성한다.
	void	SetDataWindowStart( CPoint point);		// 데이타창의 조회데이타 구성의 시작점.
	void	SetDataWindowHeader(const char *szRQ, CPoint point);	// 데이타창의 ListCtrl Header Title을 구성한다.
	void	SetDataWindowViewData(const char *szRQ, const char *szTitle, const char *szPacketNames, const int nDataIndex, CString& strViewData);
	void	SetDataWindowScaleData(const char *szRQ, const char *szHorzScaleName, const int nDataIndex, CString& strViewData);
	CString	GetDataWindowRealPacketNames( const char *szTitle, const char *szPacketNames) const;

// (2008/5/13 - Seung-Won, Bae) Refresh with Mouse Move Message
protected:
	void	Refresh( void);
};

#endif // !defined(AFX_MULTICHARTDATAVIEWADDINIMP_H__6CA19E9D_E870_483A_8353_C1C020E6E164__INCLUDED_)
