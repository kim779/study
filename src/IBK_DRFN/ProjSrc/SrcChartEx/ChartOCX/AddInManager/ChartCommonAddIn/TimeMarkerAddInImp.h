// TimeMarkerAddInImp.h: interface for the CTimeMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMEMARKERADDINIMP_H__B6813EBB_4701_461A_A07E_03C6F985668B__INCLUDED_)
#define AFX_TIMEMARKERADDINIMP_H__B6813EBB_4701_461A_A07E_03C6F985668B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_AddIn/I000000/_ChartAddInBase.h"		// for CChartAddInBase

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/1/20
// Comments		: 시간공유 표시기 Object Class이다.
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CTimeMarkerAddInImp : public CChartAddInBase
{
// (2004.11.22, 배승원) Chart Manager Interface를 관리하여 처리가 용이토록 한다.
public:
	CTimeMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CTimeMarkerAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.11.22, 배승원, 정리) 시간공유를 전달받아, 시간공유표시를 출력하는 기능을 제공한다.
protected:
	// 1. 현재 Mark 처리중인 일시값을 관리한다.
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long	m_lMarkTime;
	__int64	m_lMarkTime;
	// 2011.01.06 by SYS <<

	// (2004.09.14, 배승원) 녹색 팬만을 관리한다.
	CPen	m_pnPen;
	// (2004.07.05, 배승원) Time Mark의 값이 다른 종목에서도 유효하도록, 
	//		Packet의 Type을 관리하여, 변경된 경우가 아니면 계속 Draw하도록 한다.
	CString	m_strPacketType;
	// (2007/1/17 - Seung-Won, Bae) Manage Double Click Time Interval for LBtnDblClick with force.
	UINT	m_uDoubleClickTime;
	DWORD	m_dwLButtonTime;
	CPoint	m_ptPrevious;

protected:
	// 2. 시간 공유값을 전달받아 TimeMark를 설정한다.
	void	SetTimeMarkString( const char *p_szMarkTime, BOOL p_bWithScroll = TRUE);

// (2004.11.22, 배승원, 정리) 시간공유를 발생시키는 Interface와 Clear 시키는 Interface를 제공한다.
protected:
	// (2004.06.17, 배승원) TimeMark를 Clear시키는 Interface를 제공한다.
	void	ClearTimeMark( void)	{	m_lMarkTime = 0;	}
	// 3. Block의 Double Click시의 시간 공유를 발생시키면서 TimeMarkerAddInImp를 설정하는 Interface를 제공한다.
	void	SetTimeMarkPoint( CPoint p_ptPoint);

// (2004.11.22, 배승원) Mouse Click을 통한 시간공유 출력 해제 기능을 지원한다.
protected:
	// (2004.11.22, 배승원) 시간 공유 값을 현차트의 시간축에 맞춰 Data를 변환하는 Interface를 제공한다.
	//		오류시 0을 Return한다.
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//long	GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex);	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
	__int64	GetTimeValue( const char *p_szMarkTime, int &p_nTimeIndex);	// YYYYMMDD or HHMMSS or YYYYMMDDHHMMSS
	// 2011.01.06 by SYS <<
	//
	
	// (2004.11.22, 배승원) 시간 공유 값을 산출하는 Routine을 제공하여, 사전에 시간 공유값을 확인하는 기능을 제공한다.
	BOOL	GetMarkTimeInString( CPoint p_ptPoint, CString &p_strMarkTime);
protected:
	// (2004.11.22, 배승원) Mouse 위치에 시간공유표시가 설정된 경우, 이를 최소시킨다.
	BOOL	ClearTimeMark( CPoint p_ptPoint);

// (2004.10.11, 배승원) 시간 공유 기능을 Custom AddIn으로 옮긴다.
protected:
	// (2004.09.30, 배승원) 시간공유의 수신을 허용하는 Flag을 두어 제한할 수 있도록 한다.
	//		발신은 제한하지 않는다.
	BOOL		m_bEnableReceivingTimeShare;

// (2004.09.30, 배승원) 시간공유의 수신을 허용하는 Flag을 두어 제한할 수 있도록 한다.
//		발신은 제한하지 않는다.
protected:
	void	OnEnableReceivingTimeShare (BOOL p_bEnable);

// (2004.10.11, 배승원) TimeMark를 설정하는 Interface를 제공한다.
protected:
	void	OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll);

// (2006/11/16 - Seung-Won, Bae) Manage the Disable Flag of TimeMark
protected:
	BOOL			m_bEnableFiringTimeShare;

// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
protected:
	virtual BOOL	InvokeAddIn( int p_nCommandType, long p_lData);

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/10/11] 공유를 전달하는 Interface를 제공한다.
	// (2005/8/10 - Seung-Won, Bae) Not used in Hanwha
	virtual void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare);
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// (2006/11/24 - Seung-Won, Bae) On Tool Command
	virtual void	OnToolCommand( const int p_nToolType, const int p_nCmdOption);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID);
	virtual BOOL	OnAddInToolCommand( const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
protected:
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonDblClk(	HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_TIMEMARKERADDINIMP_H__B6813EBB_4701_461A_A07E_03C6F985668B__INCLUDED_)
