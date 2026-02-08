// CommonInterface.h: interface for the CCommonInterface class.
// 종찬(05/01/20) Chart OCX의 부모클래스. 공통DLL에서 필요로하는 OCX함수들을 여기다 담아서
//			요 클래스 포인터로 엑세스하도록 한다. (즉 DLL공통인터페이스란 말이쥥~)
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMONINTERFACE_H__C214783A_7D85_4235_A569_F76E35F3F96C__INCLUDED_)
#define AFX_COMMONINTERFACE_H__C214783A_7D85_4235_A569_F76E35F3F96C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "BlockBaseEnum.h"

class CIndicatorList;
class CIndicatorInfo;
class CPacket;
class CMainBlock;
class CPacketList;
class CAnalysisToolData;
class CStyleData;

class CCommonInterface  
{
public:
	enum ETCMESSAGETYPE
	{
		PRICE_LOG = 0
	};

public:
	//////////////////////////////////////////////////////////////////////////
	// 기본 내용
	virtual CString GetDefaultPath() const = 0;
	virtual CString GetUserPath() const = 0;

	//////////////////////////////////////////////////////////////////////////
	// 환경설정에서 사용할 함수들...
	virtual void SetImaginaryCandle(const bool bImaginaryCandle, const bool bOutside = true) = 0;	// 허수봉설정.
	virtual void SetNumericalInquiryOption(const long nInquiryOption, const bool bOutside = true) = 0;	// 수치조회창 표시여부 0:없음, 1:Window
	virtual void SetYScalePosition(const int nPosition, const bool bOutside = true) = 0;			// Y축 눈금표시 (0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음)
	virtual void SetRightMargin(const int nRightMargin, const bool bOutside = true) = 0;			// 오른쪽 여백
	virtual void SetShowIndicatorVariable(const bool bShow, const bool bOutside = true) = 0;		// 지표정보 - 지표변수 표시
	virtual void SetShowIndicatorValue(const bool bShow, const bool bOutside = true) = 0;			// 지표정보 - 지표값 표시
	virtual void SetShowHighLowBound(const bool bShow, const bool bOutside = true) = 0;				// 상/하한가 표시
	virtual void SetShowExRightDividend(const bool bShow, const bool bOutside = true) = 0;			// 권리락/배당락 표시
	virtual void SetShowCurrentPrice(const int nShowType, const bool bOutside = true) = 0;			// 가격눈금위에 현재가표시
	virtual void SetTrendLineColor(const COLORREF clrLineColor, const bool bOutside = true) = 0;	// 추세선 색상
	virtual void SetShowTrendLineValue(const int nPosition, const bool bOutside = true) = 0;		// 추세선값 표시 (0: , 1: , 2: , 3: )
	virtual void SetTimeLinkage(const bool bTimeLinkage, const bool bOutside = true) = 0;			// 시간연동 수신
	virtual void SetAutoSave(const bool bAutoSave, const bool bOutside = true) = 0;					// 차트종료시 자동 저장 유무
	virtual void SetShowMinMaxPrice(const bool bShow, const bool bOutside = true) = 0;				// 최고가/최저가 표시

	virtual bool GetImaginaryCandle() = 0;						// 허수봉설정.
	virtual long GetNumericalInquiryOption( void) const = 0;	// 수치조회창 표시여부 0:없음, 1:Window
	virtual int GetYScalePosition() = 0;						// Y축 눈금표시 (0:왼쪽, 1:오른쪽, 2:양쪽, 3:없음)
	virtual int GetRightMargin() = 0;							// 오른쪽 여백
	virtual bool GetShowIndicatorVariable() = 0;				// 지표정보 - 지표변수 표시
	virtual bool GetShowIndicatorValue() = 0;					// 지표정보 - 지표값 표시
	virtual bool GetExistenceHighLowBound() const = 0;				// "상/하한가 표시"가 설정되어 있는지의 유무(사용유무).
	virtual bool GetShowHighLowBound() = 0;						// 상/하한가 표시
	virtual bool GetShowExRightDividend() = 0;					// 권리락/배당락 표시
	virtual int GetShowCurrentPrice()  = 0;						// 가격눈금위에 현재가표시
	virtual COLORREF GetTrendLineColor() = 0;					// 추세선 색상
	virtual int	GetShowTrendLineValue() = 0;					// 추세선값 표시 (0: , 1: , 2: , 3: )
	virtual bool GetTimeLinkage() = 0;							// 시간연동 수신
	virtual bool GetAutoSave() = 0;								// 차트종료시 자동 저장 유무
	virtual bool GetShowMinMaxPrice() = 0;						// 최고가/최저가 표시

	virtual void ChartEnvSetupOk() = 0;							// 차트설정창 OK버튼을 눌렀을때.
	
	virtual CIndicatorList* GetIndicatorList() = 0;
	virtual CMainBlock*	GetMainBlock() = 0;
	virtual CPacketList* GetPacketList() = 0;
	virtual void GetAnalysisToolData(CAnalysisToolData& analysisToolData) = 0;
	virtual void GetStyleData(CStyleData& styleData) const = 0;

	virtual bool SetAnalysisToolData(const CAnalysisToolData& analysisToolData) = 0;
	virtual bool SetStyleData(const CStyleData& styleData) = 0;
	virtual bool SaveChartCfgInCurrentOCX() = 0;
	//////////////////////////////////////////////////////////////////////////
	// AddIn
	virtual bool CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList) = 0;
	virtual bool DeleteIndicator(const char* pcszIndicatorName) = 0;
	virtual bool CalculateAll(const CCalculateBaseData::CALCULATETYPE enumCalulateType) = 0;

	//////////////////////////////////////////////////////////////////////////
	// ChartObject.dll
	virtual void BehaviorBlock(const long pBlockBehaviorData) = 0;
	virtual void SetGraphSelectedDataIndex(const int nDataIndex, LPCTSTR pcszIndicatorName) = 0;
	virtual void SetGraphThickness(const int nGraphThickness1) = 0;
	virtual void SetGraphTypeStyle(const int nGraphType, const int nDrawStyle) = 0;
	virtual void SetEtc(const int nOption, const int nUserMargin) = 0;
	virtual void SetGraphMargin(LPCTSTR pcszFunctionName, const bool bIsAddMargin) = 0;
	virtual void SetCursorExchange(const CCursorShape::CHARTOBJECT enumChartObject) = 0;
	virtual int	GetUserMargin(void) const = 0;
	virtual HWND GetWndHandle(void) const = 0;
	virtual void InvalidateRect(const CRect& region, const bool  bErase) = 0;
	

	//////////////////////////////////////////////////////////////////////////
	// IndincatorConfiguration.dll
	virtual void InvalidateOCX(const bool bErase = true)	= 0;	// OCX Invalidate
	virtual int SaveCurrentChartCfg() = 0;						// 현재 차트 화면설정을 저장하고 해당 인덱스를 리턴한다.
	virtual bool RestoreChartCfg(const int nIndex) = 0;			// 인덱스에 해당하는 차트화면으로 복원한다.
	virtual bool AddRemoveGraph(LPCTSTR pcszIndicatorName) = 0;	// 지표그래프를 추가/삭제한다.
	virtual bool IsLoad_MarketChart() const = 0;				// 시장지표 Load 할지에 대한 유무

	//////////////////////////////////////////////////////////////////////////
	// ToolBar.dll
	virtual bool IsWholeView() const = 0;	// 전체보기인지 리턴.
	virtual void InvalidateOleControl() = 0;	// COleControl의 InvalidateControl()을 호출한다.
	virtual void SetStartEndIndex(const int nStartIndex, const int nEndIndex) = 0;	// 스크롤바에 의해 새로 계산한 Start/End Index를 세팅한다.
	virtual void SetScrollInfo(const int nOnePageDataCount) = 0;				// 한페이지의 데이타갯수를 세팅한다.
	virtual bool RunToolFunction(const CToolOptionInfo::TOOLOPTION enumToolOption, const int nParam) = 0; // 툴기능실행/해제
	virtual bool RunToolFuncGraphStyle(const int nParam) = 0;				// 그래프 형태를 설정한다.
	virtual bool RunToolFuncLineThickness(const int nParam) = 0;				// 선 굵기를 설정한다.
	virtual bool RunToolFuncGraphColor(const COLORREF clrGraphColor) = 0;	// 그래프 색을 설정한다.
	virtual void SetStateWholeView(const int nParam = 0) = 0;				// 전체보기를 설정한다.
	virtual CWnd* GetChartWindowPointer() const = 0;	// OCX의 Window Pointer를 넘겨준다.
	virtual int	GetMinCountPerPage() const = 0;		// 한페이지의 최소 데이타갯수를 넘긴다.
	virtual int	GetDataCountForToolBar() const = 0;
	virtual CRect GetToolBarRegion() const = 0;
	virtual CFont* GetRscFont() const = 0;				// 리소스폰트를 가져온다.
	virtual CRect GetOcxRegion() const = 0;			// OCX영역을 가져온다.
	virtual double ConvertYPositionToData(const int nYPosition, const int nYPositionTop, const int nYPositionBottom,
						const double& dDataMax, const double& dDataMin, const bool bLog, const bool bReverse) = 0;	// 수평대를 구해오기위한 함수.
	// TR 요청
	virtual void RequestTRPacket(const CString& strIndicatorName, const CString& strPacketNames) = 0;	// 해당의 패킷의 TR을 요청한다.
	virtual void RequestTRPacket(const CString& strIndicatorName, const CString& strPacketNames, const int nCount, const bool bIsNew) = 0;	// 해당의 패킷의 TR을 요청한다.

	//////////////////////////////////////////////////////////////////////////
	// Etc
	virtual void OnScaleSettingMessage(const ETCMESSAGETYPE eEtcMessageType, const int nData) = 0;

};

#endif // !defined(AFX_COMMONINTERFACE_H__C214783A_7D85_4235_A569_F76E35F3F96C__INCLUDED_)
