#ifndef OUTSIDECOMMONINTERFACE_H_
#define OUTSIDECOMMONINTERFACE_H_
//////////////////////////////////////////////////////////////////////////
// OutsideCommonInterface.h
// Chart OCX과 MultiChart DLL이 이것을 상속받아 필요한 함수를 구현한다.
// OCX와 DLL은 이 인터페이를 통해 서로 의사소통을 한다.
//////////////////////////////////////////////////////////////////////////
#include <AFXTEMPL.H>

class COutsideCommonInterface
{
public:
	// OutsideCommonInterface 의 type
	enum TYPE_OCIF
	{
		BASE_TYPE = 0,
		MC_TYPE
	};

public:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return BASE_TYPE; };

public:
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
public:
	virtual TYPE_OCIF IsOutsideInterfaceType() const { return MC_TYPE; };

public:
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

#endif