// OutsideCommonInterfaceFunction.h: interface for the COutsideCommonInterfaceFunction class.
// 종찬(05/07/28) 멀티차트등(외부DLL)에게 함수를 호출하는 함수들의 집합체.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTSIDECOMMONINTERFACEFUNCTION_H__6E946239_8F15_49F3_89C0_6D6A117578DA__INCLUDED_)
#define AFX_OUTSIDECOMMONINTERFACEFUNCTION_H__6E946239_8F15_49F3_89C0_6D6A117578DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

#include "BlockBaseEnum.h"				// for CToolOptionInfo

class CMChartInterface;
class CMChartInterface061115;
class COldMChartInterface;			// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
class COldMChartInterface061115;
class COutsideCommonInterface;
class COutsideCommonInterfaceFunction  
{
//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for Interface in COutsideCommonInterface
//////////////////////////////////////////////////////////////////////
private:
	COutsideCommonInterface* m_pOutsideCommonInterface;

public:
	COutsideCommonInterfaceFunction();
	virtual ~COutsideCommonInterfaceFunction();

	void						SetOutsideCommonInterface(COutsideCommonInterface* pOutsideCommonInterface, CString p_strDataPath = "");
	COutsideCommonInterface *	GetOutsideCommonInterface( void);

public:
	// 리얼처리.
	void Outside_SetRealInfo (void* pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);
	void Outside_UnSetRealInfo (void* pDrds, HWND hWnd,	UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName);

	// Hts Path와 User ID 구하기
	LPCTSTR Outside_GetMainPath();
	LPCTSTR Outside_GetUserID();
	
	// TR 요청
	void Outside_RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew);

	// 시장지표 Load 할지에 대한 유무
	bool Outside_IsLoad_MarketChart() const;

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for Interface in CMChartInterface
//////////////////////////////////////////////////////////////////////
public:
	BOOL				InitMChartInterface();
private:
	BOOL						m_bMChartInterfaceInited;
	CMChartInterface *			m_pMChartInterface;
	COldMChartInterface *		m_pOldMChartInterface;	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface

public:
	// 데이타창.
	bool MChart_IsActiveDataWindow();
	void MChart_SetDataWindowData(CList<CString, CString&>* pListData);

	// 환경설정.
	bool MChart_SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)	;	// 허수봉설정 및 갭보정
	bool MChart_SetYScalePositionToOutside(const int nPosition)				;	// Y축 눈금표시 (0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음)
	bool MChart_SetRightMarginToOutside(const int nRightMargin)				;	// 오른쪽 여백
	bool MChart_SetShowIndicatorVariableToOutside(const bool bShow)			;	// 지표정보 - 지표변수 표시
	bool MChart_SetShowIndicatorValueToOutside(const bool bShow)			;	// 지표정보 - 지표값 표시
	bool MChart_SetShowHighLowBoundToOutside(const bool bShow)				;	// 상/하한가 표시
	bool MChart_SetShowExRightDividendToOutside(const bool bShow)			;	// 권리락/배당락 표시
	bool MChart_SetShowCurrentPriceToOutside(const int nShowType)			;	// 가격눈금위에 현재가표시
	bool MChart_SetTrendLineColorToOutside(const COLORREF clrLineColor)		;	// 추세선 색상
	bool MChart_SetShowTrendLineValueToOutside(const int nPosition)			;	// 추세선값 표시 (0: , 1: , 2: , 3: )
	bool MChart_SetTimeLinkageToOutside(const bool bTimeLinkage)			;	// 시간연동 수신
	bool MChart_SetAutoSaveToOutside(const bool bAutoSave)					;	// 차트종료시 자동 저장 유무
	bool MChart_SetShowTrendLineTextToOutside(const bool bShow)				;	// 추세선Text 보이기
	bool MChart_SetShowTimeLineTextToOutside(const bool bShow)				;	// 시간선Text 보이기
	bool MChart_SetShowPriceLineLeftTextToOutside(const bool bShow)			;	// 가격선 왼쪽 Text 보이기
	bool MChart_SetShowPriceLineRightTextToOutside(const bool bShow)		;	// 가격선 오른쪽 Text 보이기
	bool MChart_SetShowTrisectionLineTextToOutside(const bool bShow)		;	// 삼등분선 Text 보이기
	bool MChart_SetShowQuadrisectionLineTextToOutside(const bool bShow)		;	// 사등분선 Text 보이기
	bool MChart_SetShowControlLineTextToOutside(const bool bShow)			;	// 조정선 Text 보이기
	bool MChart_GetImaginaryCandleToOutside(bool& bImaginaryCandle)			;	// 허수봉설정.
	bool MChart_GetUseGapRevisionToOutside(bool& bUseGapRevision)			;	// 갭보정
	bool MChart_GetYScalePositionToOutside(int& nPosition)					;	// Y축 눈금 표시
	bool MChart_GetRightMarginToOutside(int& nRightMargin)					;	// 오른쪽 여백
	bool MChart_GetShowIndicatorVariableToOutside(bool& bShow)				;	// 지표정보 - 지표변수 표시
	bool MChart_GetShowIndicatorValueToOutside(bool& bShow)					;	// 지표정보 - 지표값
	bool MChart_GetShowHighLowBoundToOutside(bool& bShow)					;	// 상/하한가 표시
	bool MChart_GetShowExRightDividendToOutside(bool& bShow)				;	// 권리락/배당락 표시
	bool MChart_GetShowCurrentPriceToOutside(int& nShowType)				;	// 가격 눈금위에 현재값 표시
	bool MChart_GetTrendLineColorToOutside(COLORREF& clrLineColor)			;	// 추세선 색상
	bool MChart_GetShowTrendLineValueToOutside(int& nPosition)				;	// 추세선값 표시
	bool MChart_GetTimeLinkageToOutside(bool& bTimeLinkage)					;	// 시간연동 수신
	bool MChart_GetAutoSaveToOutside(bool& bAutoSave)						;	// 차트종료시 자동저장
	bool MChart_GetShowTrendLineTextToOutside(bool& bShow)					;	// 추세선Text 보이기
	bool MChart_GetShowTimeLineTextToOutside(bool& bShow)					;	// 시간선Text 보이기
	bool MChart_GetShowPriceLineLeftTextToOutside(bool& bShow)				;	// 가격선 왼쪽 Text 보이기
	bool MChart_GetShowPriceLineRightTextToOutside(bool& bShow)				;	// 가격선 오른쪽 Text 보이기
	bool MChart_GetShowTrisectionLineTextToOutside(bool& bShow)				;	// 삼등분선 Text 보이기
	bool MChart_GetShowQuadrisectionLineTextToOutside(bool& bShow)			;	// 사등분선 Text 보이기
	bool MChart_GetShowControlLineTextToOutside(bool& bShow)				;	// 조정선 Text 보이기
	bool MChart_ChartEnvSetupOk();						// Ok버튼을 눌렀을때 멀티차트에 알려준다.
	bool MChart_SetPriceLog(const bool bLog);	// 가격차트 log 셋팅

	bool MChart_OnChartEnvSetupOk( void);

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for New Interface in 06/11/15
//////////////////////////////////////////////////////////////////////
public:
	BOOL						InitMChartInterface061115();
private:
	BOOL						m_bMChartInterfaceInited061115;
	CMChartInterface061115 *	m_pMChartInterface061115;
	COldMChartInterface061115 *	m_pOldMChartInterface061115;	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface

// (2006/11/15 - Seung-Won, Bae) Revised Price
public:
	BOOL	GetRevisedPriceFlag( void);
	BOOL	SetRevisedPriceFlag( BOOL p_bRevisedFlag);

// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
public:
	BOOL	OnIndicatorAddOrRemoved( const char *p_szGraphName, const char *p_szIndicatorName, const BOOL p_bAdded);

// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
public:
	BOOL	OnUserZoomChanged( const int p_nDataCountInView);

// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
public:
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

	// FX차트 주문 연동 - onlyojt (20091204)
	BOOL	OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3);

// (2009/9/3 - Seung-Won, Bae) for Old MChartInterface
protected:
	BOOL	m_bIsOldInterface;
};

#endif // !defined(AFX_OUTSIDECOMMONINTERFACEFUNCTION_H__6E946239_8F15_49F3_89C0_6D6A117578DA__INCLUDED_)
