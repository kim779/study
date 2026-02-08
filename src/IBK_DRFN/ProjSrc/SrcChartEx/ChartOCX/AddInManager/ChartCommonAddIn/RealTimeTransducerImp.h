// RealTimeTransducerImp.h: interface for the CRealTimeTransducerImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REALTIMETRANSDUCER_H__623AE3D6_825B_43F5_B726_4B2FE89B8C14__INCLUDED_)
#define AFX_REALTIMETRANSDUCER_H__623AE3D6_825B_43F5_B726_4B2FE89B8C14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

#include "RealTimeTransducerDlg.h"							// for CRealTimeTransducerDlg


class CRealTimeTransducerImp : public CChartAddInBase
{
public:
	CRealTimeTransducerImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CRealTimeTransducerImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;


// (2009/10/23 - Seung-Won, Bae) Transducer Dialog
protected:
	CRealTimeTransducerDlg		m_dlgTimeTransducer;


// (2009/10/23 - Seung-Won, Bae) Manage Time Value
protected:
	__int64				m_nRealTime;
	__int64				m_nAddTime;
	__int64				m_nFakeTime;
	CTimeTypeConvert	m_tcTR;
	CTimeTypeConvert	m_tcReal;


//////////////////////////////////////////////////////////////////////
// Event Routines
//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) AddIn DLL에 OCX Event를 전달한다.
protected:
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/10/4 - Seung-Won, Bae) On Real Packet End
	virtual void	OnRealPacketEnd( const char *p_szRQ);
	// (2008/10/22 - Seung-Won, Bae) for Real Filtering
	//	Do not return FALSE. if you do, the real data can not be delivered to other AddIn and OCX.
	//	You can change the Real Data Value. it will be set to chart.
	virtual BOOL	OnPreRealData( const char *p_szPacketName, double &p_dData, const char *p_szRQ = NULL, BOOL p_bRealData = TRUE);
	virtual BOOL	OnPreRealPacketEnd( const char *p_szRQ = NULL, BOOL p_bRealData = TRUE);
	virtual BOOL	OnPreNMTRClock( __int64 &p_nRealClockSecs);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_REALTIMETRANSDUCER_H__623AE3D6_825B_43F5_B726_4B2FE89B8C14__INCLUDED_)
