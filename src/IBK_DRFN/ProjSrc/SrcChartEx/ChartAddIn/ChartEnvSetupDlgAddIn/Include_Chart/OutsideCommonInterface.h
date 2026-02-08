#ifndef OUTSIDECOMMONINTERFACE_H_
#define OUTSIDECOMMONINTERFACE_H_
//////////////////////////////////////////////////////////////////////////
// OutsideCommonInterface.h
// Chart OCX과 MultiChart DLL이 이것을 상속받아 필요한 함수를 구현한다.
// OCX와 DLL은 이 인터페이를 통해 서로 의사소통을 한다.
//////////////////////////////////////////////////////////////////////////
#include <AFXTEMPL.H>

#include "BlockBaseEnum.h"			// for CToolOptionInfo

// (2004.10.11, 배승원) DLL Container와 Communication을 위한 Message를 정의한다.
// (2004.05.06, 배승원) Chart OCX에서 보내는 Event Message를 정의한다.
//		WPARAM	: ID
//		LPARAM	: MSG Structure Address
//			MSG msg;
//			msg.hwnd	= GetSafeHwnd();
//			msg.message	= message;
//			msg.wParam	= wParam;
//			msg.lParam	= lParam;
#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"
extern const UINT RMSG_CHARTMSG_EVENTMESSAGE;

// (2004.05.18, 배승원) RMSG_CHARTMSG_EVENTMESSAGE로 전달되는 Event Message Structure에 다음의 Message로 전달된다.
// (2004.05.18, 배승원) DLL Container에게 한 화면의 Data 수 (ZoomValue)의 변화를 전달한다.
//		이때, LParam의 값이 ZoomValue이다.
#define UWM_ZOOMVALUE				( WM_USER + 0x2D36)
// (2004.06.04, 배승원) Scroll이 변경될 때마다 DLL Container에게 끝일시를 전달한다.
//		이때, LParam의 값이 현 화면의 마지막 Data의 일시를 Long Value로 전달하게 된다.
#define UWM_VIEWENDTIME				( WM_USER + 0x2D37)
// (2004.10.08, 배승원) 십자선 이용시의 수치조회창 이용여부 변경시 Multi-Chart에게 알려,
//		다른 분할차트에도 이 설정이 적용되도록 한다.
//		이때, WParam의 값이 새로이 설정된 수치조회창 이용여부 설정값이다.
#define UWM_USEINQUIRY_CROSSLINE	( WM_USER + 0x2D39)
// (2004.11.04, 배승원) 차트의 Global 공유 Data 발생을 DLL Container에게 알리는 Message이다.
//		이때 LParam으로 "공유명;공유Data"의 형태로 Data가 전달된다.
//		기본적으로 Chart는 시간공유, 일반호가공유, 특수호가공유(화면별제한불가)를 발생시킨다.
#define UWM_FIREGSHAREDATA			( WM_USER + 0x2D3B)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify I (Drag Start Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGSTART				( WM_USER + 0x2D3C)
// (2005/2/17 - Seung-Won, Bae) Term Analysis Infomation Notify II (Drag End Position in Time Serial)
//		LParam : Time Value (long) of Mouse Position
#define UWM_DRAGEND					( WM_USER + 0x2D3D)
// (2005/10/25 - Sun-Young, Kim) 차트 전체초기화
#define UWM_ORGRETURN				( WM_USER + 0x2D40)
// 선택된 차트 알림 : 복수종목 - ojtaso (20070122)
#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
// 복수종목의 Total Count를 멀티차트에 알려줌 : 복수종목 - ojtaso (20070704)
#define UWM_MULTITOTALCOUNT			( WM_USER + 0x2D47)

class COutsideCommonInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;
protected:
	// OutsideCommonInterface 의 type
	enum TYPE_OCIF
	{
		BASE_TYPE = 0,
		MC_TYPE_ORG,
		MC_TYPE_061115,		// (2006/11/15 - Seung-Won, Bae) Add New Interface Type.
		// (2006/11/18 - Seung-Won, Bae) Caution!
		//		The Next Version Interface must support the Previous Version Interface Correctly.
		//		Because, Old Version Supported Interface Checking is processed with Version Nember by '<=' (Greater than or Equal to)
	};

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return BASE_TYPE; };

protected:
	// 리얼처리용.
	virtual void    SetRealInfo		(	void* pDrds,		// Drds포인터인데 NULL로 보냄.
										HWND hWnd,			// Chart OCX의 윈도우 핸들
										UINT id, 			// CPacket* 
										LPSTR DataName,		//  ex: "S31"
										LPSTR FieldName, 	//  ex: "time"
										LPSTR KeyName)	{ };//  ex: "000660"

	virtual void    UnSetRealInfo	(	void* pDrds,		// Drds포인터인데 NULL로 보냄.
										HWND hWnd,			// Chart OCX의 윈도우 핸들
										UINT id, 			// CPacket* 
										LPSTR DataName,		//  ex: "S31"
										LPSTR FieldName, 	//  ex: "time"
										LPSTR KeyName) { };	//  ex: "000660"


	// 경로 및 UserID
	virtual LPCTSTR GetMainPath() const { return _T(""); };
	virtual LPCTSTR GetUserID() const { return _T(""); };

	// TR Data요청
	virtual void RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew) { };

	// 시장지표 Load 할지에 대한 유무
	virtual bool IsLoad_MarketChart() const { return false; };

};

// 멀티차트용 인터페이스
class CMChartInterface : public COutsideCommonInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_ORG; };

protected:
	// 데이타창.
	virtual bool	IsActiveDataWindow()										{ return false; };
	virtual void	SetDataWindowData(CList<CString, CString&>* pListData)		{ };

	// 환경설정.
	virtual bool	SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)
																				{ return false; };	// 허수봉설정 및 갭보정
	virtual bool	SetInquiryDataWindowTypeToOutside(const int nWindowType)	{ return false; };	// 수치조회창 표시여부 0:없음, 1:Window
	virtual bool	SetInquiryDataWindowMoveTypeToOutside(const int nMoveType)	{ return false; };	// 수치조회창 이동형태설정 0:이동, 1:고정
	virtual bool	SetYScalePositionToOutside(const int nPosition)				{ return false; };	// Y축 눈금표시 (0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음)
	virtual bool	SetRightMarginToOutside(const int nRightMargin)				{ return false; };	// 오른쪽 여백
	virtual bool	SetShowIndicatorVariableToOutside(const bool bShow)			{ return false; };	// 지표정보 - 지표변수 표시
	virtual bool	SetShowIndicatorValueToOutside(const bool bShow)			{ return false; };	// 지표정보 - 지표값 표시
	virtual bool	SetShowHighLowBoundToOutside(const bool bShow)				{ return false; };	// 상/하한가 표시
	virtual bool	SetShowExRightDividendToOutside(const bool bShow)			{ return false; };	// 권리락/배당락 표시
	virtual bool	SetShowCurrentPriceToOutside(const int nShowType)			{ return false; };	// 가격눈금위에 현재가표시
	virtual bool	SetTrendLineColorToOutside(const COLORREF clrLineColor)		{ return false; };	// 추세선 색상
	virtual bool	SetShowTrendLineValueToOutside(const int nPosition)			{ return false; };	// 추세선값 표시 (0: , 1: , 2: , 3: )
	virtual bool	SetTimeLinkageToOutside(const bool bTimeLinkage)			{ return false; };	// 시간연동 수신
	virtual bool	SetAutoSaveToOutside(const bool bAutoSave)					{ return false; };	// 차트종료시 자동 저장 유무
	virtual bool	SetShowTrendLineTextToOutside(const bool bShow)				{ return false; };	// 추세선Text 보이기
	virtual bool	SetShowTimeLineTextToOutside(const bool bShow)				{ return false; };	// 시간선Text 보이기
	virtual bool	SetShowPriceLineLeftTextToOutside(const bool bShow)			{ return false; };	// 가격선 왼쪽 Text 보이기
	virtual bool	SetShowPriceLineRightTextToOutside(const bool bShow)		{ return false; };	// 가격선 오른쪽 Text 보이기
	virtual bool	SetShowTrisectionLineTextToOutside(const bool bShow)		{ return false; };	// 삼등분선 Text 보이기
	virtual bool	SetShowQuadrisectionLineTextToOutside(const bool bShow)		{ return false; };	// 사등분선 Text 보이기
	virtual bool	SetShowControlLineTextToOutside(const bool bShow)			{ return false; };	// 조정선 Text 보이기
	virtual bool	SetShowCrossLineToOutside(const bool bShow)					{ return false; };	// 수치조회시 십자선표시여부

	virtual bool	GetImaginaryCandleToOutside(bool& bImaginaryCandle)			{ return false; };	// 허수봉설정.
	virtual bool	GetUseGapRevisionToOutside(bool& bUseGapRevision)			{ return false; };	// 갭보정
	virtual bool	GetInquiryDataWindowTypeToOutside(int& nWindowType)			{ return false;	};	// 수치조회창 표시여부
	virtual bool	GetInquiryDataWindowMoveTypeToOutside(int& nMoveType)		{ return false; };	// 수치조회창 이동형태설정
	virtual bool	GetYScalePositionToOutside(int& nPosition)					{ return false; };	// Y축 눈금 표시
	virtual bool	GetRightMarginToOutside(int& nRightMargin)					{ return false; };	// 오른쪽 여백
	virtual bool	GetShowIndicatorVariableToOutside(bool& bShow)				{ return false; };	// 지표정보 - 지표변수 표시
	virtual bool	GetShowIndicatorValueToOutside(bool& bShow)					{ return false; };	// 지표정보 - 지표값
	virtual bool	GetShowHighLowBoundToOutside(bool& bShow)					{ return false; };	// 상/하한가 표시
	virtual bool	GetShowExRightDividendToOutside(bool& bShow)				{ return false; };	// 권리락/배당락 표시
	virtual bool	GetShowCurrentPriceToOutside(int& nShowType)				{ return 0;		};	// 가격 눈금위에 현재값 표시
	virtual bool	GetTrendLineColorToOutside(COLORREF& clrLineColor)			{ return false; };	// 추세선 색상
	virtual bool	GetShowTrendLineValueToOutside(int& nPosition)				{ return false; };	// 추세선값 표시
	virtual bool	GetTimeLinkageToOutside(bool& bTimeLinkage)					{ return false; };	// 시간연동 수신
	virtual bool	GetAutoSaveToOutside(bool& bAutoSave)						{ return false; };	// 차트종료시 자동저장
	virtual bool	GetShowTrendLineTextToOutside(bool& bShow)					{ return false; };	// 추세선Text 보이기
	virtual bool	GetShowTimeLineTextToOutside(bool& bShow)					{ return false; };	// 시간선Text 보이기
	virtual bool	GetShowPriceLineLeftTextToOutside(bool& bShow)				{ return false; };	// 가격선 왼쪽 Text 보이기
	virtual bool	GetShowPriceLineRightTextToOutside(bool& bShow)				{ return false; };	// 가격선 오른쪽 Text 보이기
	virtual bool	GetShowTrisectionLineTextToOutside(bool& bShow)				{ return false; };	// 삼등분선 Text 보이기
	virtual bool	GetShowQuadrisectionLineTextToOutside(bool& bShow)			{ return false; };	// 사등분선 Text 보이기
	virtual bool	GetShowControlLineTextToOutside(bool& bShow)				{ return false; };	// 조정선 Text 보이기
	virtual bool	GetShowCrossLineToOutside(bool& bShow)						{ return false; };	// 수치조회시 십자선 표시여부.

	virtual bool	OnChartEnvSetupOk() { return false; };

	virtual bool	SetPriceLog(const bool bLog) { return false; };	// 가격차트 log 셋팅.
};

// (2006/11/15 - Seung-Won, Bae) Add New Interface Type.
class CMChartInterface061115 : public CMChartInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_061115; };

// (2006/11/15 - Seung-Won, Bae) Revised Price
protected:
	virtual BOOL	GetRevisedPriceFlag( void)									{	return FALSE;	}
	virtual BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag)					{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
protected:
	virtual BOOL	OnIndicatorAddOrRemoved( const char *p_szIndicatorName,	const BOOL p_bAdded)				{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
protected:
	virtual BOOL	OnUserZoomChanged( const int p_nDataCountInView)											{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
protected:
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)		{	return FALSE;	}
};

// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
class COldMChartInterface : public COutsideCommonInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_ORG; };

protected:
	// 데이타창.
	virtual bool	IsActiveDataWindow()										{ return false; };
	virtual void	SetDataWindowData(CList<CString, CString&>* pListData)		{ };

	// 환경설정.
	virtual bool	SetImaginaryCandleToOutside(const bool bImaginaryCandle)	{ return false; };	// 허수봉설정.
	virtual bool	SetInquiryDataWindowTypeToOutside(const int nWindowType)	{ return false; };	// 수치조회창 표시여부 0:없음, 1:Window
	virtual bool	SetInquiryDataWindowMoveTypeToOutside(const int nMoveType)	{ return false; };	// 수치조회창 이동형태설정 0:이동, 1:고정
	virtual bool	SetYScalePositionToOutside(const int nPosition)				{ return false; };	// Y축 눈금표시 (0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음)
	virtual bool	SetRightMarginToOutside(const int nRightMargin)				{ return false; };	// 오른쪽 여백
	virtual bool	SetShowIndicatorVariableToOutside(const bool bShow)			{ return false; };	// 지표정보 - 지표변수 표시
	virtual bool	SetShowIndicatorValueToOutside(const bool bShow)			{ return false; };	// 지표정보 - 지표값 표시
	virtual bool	SetShowHighLowBoundToOutside(const bool bShow)				{ return false; };	// 상/하한가 표시
	virtual bool	SetShowExRightDividendToOutside(const bool bShow)			{ return false; };	// 권리락/배당락 표시
	virtual bool	SetShowCurrentPriceToOutside(const int nShowType)			{ return false; };	// 가격눈금위에 현재가표시
	virtual bool	SetTrendLineColorToOutside(const COLORREF clrLineColor)		{ return false; };	// 추세선 색상
	virtual bool	SetShowTrendLineValueToOutside(const int nPosition)			{ return false; };	// 추세선값 표시 (0: , 1: , 2: , 3: )
	virtual bool	SetTimeLinkageToOutside(const bool bTimeLinkage)			{ return false; };	// 시간연동 수신
	virtual bool	SetAutoSaveToOutside(const bool bAutoSave)					{ return false; };	// 차트종료시 자동 저장 유무
	virtual bool	SetShowTrendLineTextToOutside(const bool bShow)				{ return false; };	// 추세선Text 보이기
	virtual bool	SetShowTimeLineTextToOutside(const bool bShow)				{ return false; };	// 시간선Text 보이기
	virtual bool	SetShowPriceLineLeftTextToOutside(const bool bShow)			{ return false; };	// 가격선 왼쪽 Text 보이기
	virtual bool	SetShowPriceLineRightTextToOutside(const bool bShow)		{ return false; };	// 가격선 오른쪽 Text 보이기
	virtual bool	SetShowTrisectionLineTextToOutside(const bool bShow)		{ return false; };	// 삼등분선 Text 보이기
	virtual bool	SetShowQuadrisectionLineTextToOutside(const bool bShow)		{ return false; };	// 사등분선 Text 보이기
	virtual bool	SetShowControlLineTextToOutside(const bool bShow)			{ return false; };	// 조정선 Text 보이기
	virtual bool	SetShowCrossLineToOutside(const bool bShow)					{ return false; };	// 수치조회시 십자선표시여부

	virtual bool	GetImaginaryCandleToOutside(bool& bImaginaryCandle)			{ return false; };	// 허수봉설정.
	virtual bool	GetInquiryDataWindowTypeToOutside(int& nWindowType)			{ return false;	};	// 수치조회창 표시여부
	virtual bool	GetInquiryDataWindowMoveTypeToOutside(int& nMoveType)		{ return false; };	// 수치조회창 이동형태설정
	virtual bool	GetYScalePositionToOutside(int& nPosition)					{ return false; };	// Y축 눈금 표시
	virtual bool	GetRightMarginToOutside(int& nRightMargin)					{ return false; };	// 오른쪽 여백
	virtual bool	GetShowIndicatorVariableToOutside(bool& bShow)				{ return false; };	// 지표정보 - 지표변수 표시
	virtual bool	GetShowIndicatorValueToOutside(bool& bShow)					{ return false; };	// 지표정보 - 지표값
	virtual bool	GetShowHighLowBoundToOutside(bool& bShow)					{ return false; };	// 상/하한가 표시
	virtual bool	GetShowExRightDividendToOutside(bool& bShow)				{ return false; };	// 권리락/배당락 표시
	virtual bool	GetShowCurrentPriceToOutside(int& nShowType)				{ return 0;		};	// 가격 눈금위에 현재값 표시
	virtual bool	GetTrendLineColorToOutside(COLORREF& clrLineColor)			{ return false; };	// 추세선 색상
	virtual bool	GetShowTrendLineValueToOutside(int& nPosition)				{ return false; };	// 추세선값 표시
	virtual bool	GetTimeLinkageToOutside(bool& bTimeLinkage)					{ return false; };	// 시간연동 수신
	virtual bool	GetAutoSaveToOutside(bool& bAutoSave)						{ return false; };	// 차트종료시 자동저장
	virtual bool	GetShowTrendLineTextToOutside(bool& bShow)					{ return false; };	// 추세선Text 보이기
	virtual bool	GetShowTimeLineTextToOutside(bool& bShow)					{ return false; };	// 시간선Text 보이기
	virtual bool	GetShowPriceLineLeftTextToOutside(bool& bShow)				{ return false; };	// 가격선 왼쪽 Text 보이기
	virtual bool	GetShowPriceLineRightTextToOutside(bool& bShow)				{ return false; };	// 가격선 오른쪽 Text 보이기
	virtual bool	GetShowTrisectionLineTextToOutside(bool& bShow)				{ return false; };	// 삼등분선 Text 보이기
	virtual bool	GetShowQuadrisectionLineTextToOutside(bool& bShow)			{ return false; };	// 사등분선 Text 보이기
	virtual bool	GetShowControlLineTextToOutside(bool& bShow)				{ return false; };	// 조정선 Text 보이기
	virtual bool	GetShowCrossLineToOutside(bool& bShow)						{ return false; };	// 수치조회시 십자선 표시여부.

	virtual bool	OnChartEnvSetupOk() { return false; };

	virtual bool	SetPriceLog(const bool bLog) { return false; };	// 가격차트 log 셋팅.
};
class COldMChartInterface061115 : public COldMChartInterface
{
// (2006/11/18 - Seung-Won, Bae) This Interface used with Only COutsideCommonInterfaceFunction class.
friend class COutsideCommonInterfaceFunction;

protected:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE_061115; };

// (2006/11/15 - Seung-Won, Bae) Revised Price
protected:
	virtual BOOL	GetRevisedPriceFlag( void)									{	return FALSE;	}
	virtual BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag)					{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
protected:
	virtual BOOL	OnIndicatorAddOrRemoved( const char *p_szGraphName,	const BOOL p_bAdded)				{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
protected:
	virtual BOOL	OnUserZoomChanged( const int p_nDataCountInView)											{	return FALSE;	}

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
protected:
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)		{	return FALSE;	}
};


#endif