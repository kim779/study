// AskingPriceIndicatorAddInImp.h: interface for the CAskingPriceIndicatorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASKINGPRICEINDICATORADDINIMP_H__D927AE83_DA1F_4030_B127_B0F228D6D864__INCLUDED_)
#define AFX_ASKINGPRICEINDICATORADDINIMP_H__D927AE83_DA1F_4030_B127_B0F228D6D864__INCLUDED_

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/10
// Comments		: 
// Using way	: 
// See			: 
//
// Help Message Reference : BOUNDMARK
//	{{{
//		BOUNDMARK : 가격차트의 영역의 우측 Scale에 해당 종목의 상하한선을 표시하도록 알려주는 보조 메시지
//			[Type I] CBoundMarker Class Header 참고.
//			[Type II] 호가 연동 기능
//				BOUNDMARK=GN:UB,LB:IC:MT:YC:RT
//					IC = 종목코드
//					MT = 장구분 (선물 옵션 기능 없음)
//						KOSPI		1
//						KOSDAQ		2
//						ETF_KOSPI	7	
//						ETF_KOSDAQ	9
//					YC = 전일종가
//					RT = 등락률 Chart 구분 Code (1:가격, 2:대비율)
//	}}}
//-----------------------------------------------------------------------------

// (2004.10.22, 배승원) 상/하한가 표시기능에 호가연동 기능을 보강한 Code Class이다.
/*
[Asking Price Indicator]

1. Action List
	On Mouse Over
		- Draw Line & Text (Value)
	On Mouse Move
		- Clear Previous Line & Text (Value)
		- Draw Next Line & Text (Value)
	On Mouse Out
		- Clear Previous Line & Text (Value)
	On Draw
		if On Mouse Over
			- Draw Line & Text (Value)

2. Drawing Method for Clear
	Line -> XOR Line
	Text -> Original Image Backup & Drawing

3. Action Flow
			On Draw
	Start	->	On Mouse Over
	Action	->	On Mouse Move
			On Draw (with New Graph)
	End	->	On Mouse Out
			On Draw
4. Event Routine
x	On Draw
x		- Draw BoundMark (Check Reverse)
x		- Set IndicatorRegion
x		- Check, SetCaptureFlag TRUE ~ Process as New Mouse Move
x			CALL ReleaseCaptureWithFlag
x			Get Mouse Position & State (GetCursorPos, NULL Flag)
x			CALL Event On Mouse Move
x	On Mouse Move
x		- Check, AskingPriceGShareFireFlag TRUE
x				~ It must be Pre-Process before Out-Indicator-Region-Process for Jumping to Out after Fire
x			CALL ClearAskingPriceGShareFireFlag
x		- Check, In Indicator Region (CRect::PtInRect)
x			- Check, SetCaptureFlag FALSE & No Mouse Flag & No Special Cursor (OCX's Get Cursor Handle)
x				CALL SetCaptureWithFlag for Mouse Out
x				CALL ShowNewIndicator
x			- Check, SetCaptureFlag
x				CALL ClearPreviousIndicatorDraw
x				CALL ShowNewIndicator
x		- Check, Out Indicator Region & SetCaptureFlag TRUE
x			CALL ClearPreviousIndicatorDraw
x			CALL ReleaseCaptureWithFlag
x	On LButtonDown
x		- Check, In Indicator Region & SetCaptureFlag TRUE ~ for Click Cursor
x			Set Yellow Diamond Cursor (LoadCursor, OCX's Set Cursor Handle)
x			Set AskingPriceGShareFireFlag TRUE
x			Fire AskingPriceGShare with IndicatedPrice
x	On LButtonUp
x		- Check, AskingPriceGShareFireFlag TRUE
x			CALL ClearAskingPriceGShareFireFlag

5. Library Routine
x	SetCpatureWithFlag
x		Set Capture
x		Set SetCaptureFlag TRUE
x		Set White Diamond Cursor (LoadCursor, OCX's Set Cursor Handle)
x	ReleaseCaptureWithFlag
x		Set SetCaptureFlag FALSE
x		Release Capture
x		Clear White Diamond Cursor (OCX's Set Cursor Handle NULL)
x		Release Backup Buffer (Check NOT NULL, DeleteObject, delete, NULL)
x		Set AskingPriceGShareFireFlag FALSE
x	ClearPreviousIndicatorDraw
x		Draw NOT Line on IndicatorPosition
x		Restore Backup Image to IndicatorTextArea [See 'Capturing an Image' of MSDN]
x	ShowNewIndicator
x		Set IndicatorPosition
x		Calculate New Indicator Value
x		Set IndicatorTextArea (GetTextExtent)
x		Backup New Indicator Drawing Area (Check Backup Buffer, Create on None)
x			[See 'Capturing an Image' of MSDN]
x		Draw New Indicator (Draw NOT Line, Draw Text with Rectangle)
x	ClearAskingPriceGShareFireFlag
x		Set AskingPriceGShareFireFlag FALSE
x		Clear Yellow Diamond Cursor (Load White Diamond Cursor, OCX's Set Cursor Handle)

6. Variable & Initializing
	BOOL		m_bOnSetCaprture		= FALSE;	~ Indicator Drawing Situation Flag	(SetCaptureFlag)
	int		m_nPreIndicatorPosY		= 0;		~ Last Drawing Position			(IndicatorPosition)
	CRect		m_rctPreIndicatorTextArea			~ Last Text Drawing Area		(IndicatorTextArea)
	CRect		m_rctIndicatorRegion				~ Indicator Region of BoundMark		(IndicatorRegion)
	CBitmap *	m_pTextAreaBackup		= NULL;		~ Backup of Indicator Text Area		(TextAreaBackupBitmap)
	BOOL		m_bOnFireAskingPriceGShare	= FALSE;	~ Asking Price GShare Firing Flag	(AskingPriceGShareFireFlag)
	double		m_dIndicatedPrice		= 0.;		~ Indicated Price			(IndicatedPrice)
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoundMarkerAddInImp.h"

interface IChartManager;
class CAskingPriceIndicatorAddInImp : public CBoundMarkerAddInImp
{
public:
	CAskingPriceIndicatorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase);
	virtual ~CAskingPriceIndicatorAddInImp();

	// Declare AddIn Event Map
	DECLARE_ADDIN_EVENT_MAP()

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
public:
	static CString		m_strAddInItemName;

// (2004.10.25, 배승원) 호가연동을 위한 Member Variable을 정의한다.
protected:
	int			m_nIndicatorPosY;				// Current Indicator Drawing Position			(IndicatorPosition)
	BOOL		m_bOnSetCaprture;				// Indicator Drawing Situation Flag			(SetCaptureFlag)
	BOOL		m_bOnFireAskingPriceGShare;		// Asking Price GShare Firing Flag			(AskingPriceGShareFireFlag)
	CRect		m_rctAskingPriceMark;			// Indicator Region of BoundMark			(IndicatorRegion)
	double		m_dIndicatedPrice;				// Indicated Price							(IndicatedPrice)
	// (2006/11/5 - Seung-Won, Bae) Use MainOSB
	//	CBitmap *	m_pTextAreaBackup;			// Backup of Indicator Text Area			(TextAreaBackupBitmap)

// (2004.10.25, 배승원) 호가연동을 위한 Library Routine을 제공한다.
protected:
	// (2004.10.25, 배승원) 호가연동 처리 Mark를 Clear시키는 Routine을 제공한다.
	void	ClearAskingPriceGShareFireFlag( void);

// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
protected:
	HCURSOR		m_hAPCursor;

// (2004.10.25, 배승원) Bound값을 설정하는 Interface를 제공한다.
//		확장된 종목코드 정보를 받아 관리하여 종목공유시에 이용한다.
protected:
//	CString			m_strItemCode;
protected:
	virtual void	ResetBound( const char *p_szBound);

// (2004.10.26, 배승원) 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
protected:
	double		m_dPacketUnitValue;

// (2004.10.26, 배승원) 조회중인 호가 String을 관리하여, 선택시 바로 공유가 발생하도록 한다.
protected:
	CString		m_strAskingPrice;

// (2004.11.01, 배승원) 장구분과 전일종가, 호가산출 DLL을 관리하여, 호가 연동에 이용한다.
//				FnGetPriceUnit( 장구분,  가격,  전일종가,  가격/대비율구분[nFlag 1:가격, 2:대비율] )
//					Error Value -99999999
#define _FnGetPriceUnit_ERROR		-99999999
typedef double ( *FnGetPriceUnit)( int nJongMokGubun, double fValue, double fPreClosePrice, int nFlag);
protected:
	int				m_nMarketType;
	int				m_nRatioByYCP;			// (2004.11.02, 배승원) 대비율(등락률)인지 가격인지 Flag을 받아 관리한다.
	double			m_dYClosePrice;
	HMODULE			m_hAskingPriceFixDLL;
	FnGetPriceUnit	m_fnFnGetPriceUnit;
// (2004.11.22, 배승원) 전일종가를 m_apiAskingPriceIndicatorAddInImp로부터 추출하여 CYChangeMark에 설정해 동작토록 한다.
protected:
	double			GetYClosePrice( void) const		{	return m_dYClosePrice;	}

// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
protected:
	// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
	virtual void	OnResetChart( void);
	// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
	virtual void	OnDrawBeforeGraph( HDC p_hDC);
	// RQ별 Packet : 복수종목 - ojtaso (20070111)
	virtual void	OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset);
	// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
	virtual void	OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap);
	virtual void	OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap);

// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
protected:
	// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
	virtual RECT	DrawCursor( HDC p_hDC);

// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
protected:
	virtual BOOL	InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData);
	virtual BOOL	OnAddInToolCommand(	const EAICommandType p_eCommandID, const long p_lParam);
	virtual BOOL	OnAddInToolCommand(	long &p_lResult, const EAICommandType p_eCommandID);
protected:
	virtual BOOL	OnMouseMove(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB);	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
	virtual BOOL	OnLButtonDown(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
	virtual BOOL	OnLButtonUp(		HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID);
};

#endif // !defined(AFX_ASKINGPRICEINDICATORADDINIMP_H__D927AE83_DA1F_4030_B127_B0F228D6D864__INCLUDED_)
