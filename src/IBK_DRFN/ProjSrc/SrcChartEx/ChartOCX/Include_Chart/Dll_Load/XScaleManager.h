// XScaleManager.h : interface for the CXScaleManager class.
//

#if !defined(AFX_XSCALEMANAGER_H__9BC10770_EEE0_4222_9E09_D247BF6D09EE__INCLUDED_)
#define AFX_XSCALEMANAGER_H__9BC10770_EEE0_4222_9E09_D247BF6D09EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* *********************************** */
/* 제작자: 이일렬(yiilyoul@magicn.com) */
/* 제작일: 2007.03.29                  */
/* *********************************** */

#include <afxtempl.h>								// for CArray

#include "../DataMath.h"
#include "../BlockBaseEnum.h"						// for CScaleBaseData
#include "../DataBlockList.h"						// for CDataBlockList

#define VALUE_L_EMPTY_DTI	-1
#define VALUE_R_EMPTY_DTI	-100000000

#define THREAD_CALCULATION 1
#define THREAD_DRAWING	   2

class CMapTimeToLong : public CMapPtrToPtr
{
public:
	void	SetAt( time_t key, long value)
			{	CMapPtrToPtr::SetAt( ( void *)key, ( void *)value);	}
	BOOL	RemoveKey( time_t key)
			{	return CMapPtrToPtr::RemoveKey( ( void *)key);	}
	BOOL	Lookup( time_t key, long &rValue) const
			{
				void *pValue;
				BOOL bResult = CMapPtrToPtr::Lookup( ( void *)key, pValue);
				rValue = ( long)pValue;
				return bResult;
			}
};

// DRI : Draw Index	- 그리기 위치
// DTI : Data Index	- 데이터 위치
// RQ  : Request	- 복수종목(다른 주기)을 처리하기 위해서는 이런 (종목+주기)를 부르는 용어가 필요하고 이 용어를 RQ라고 합니다.
// NS  : Next Step	- 다음 위치
// ITEM : 그려질때 하나의 항목을 의미합니다.

class CPacket;
class CPacketList;
class CPacketListManager;
class CIndicatorList;
interface IChartCtrl;

class CXScaleManager
{
public:
	typedef struct tagAREA
	{
		short left;
		short center;
		short right;

		tagAREA()
		{
			left = center = right = 0;
		}

		tagAREA(int left_p, int center_p, int right_p)
		{
			left = left_p;
			center = center_p;
			right = right_p;
		}

		void Offset(short nX)
		{
			if( nX == SHRT_MIN || nX == SHRT_MAX)
				left = center = right = nX;
			else
			{
				left += nX;
				center += nX;
				right += nX;
			}
		}

	} AREA, *PAREA;

	// (2008/12/30 - Seung-Won, Bae) Do not support CT_AUTO.
	typedef enum { CT_NONE, CT_DRI, CT_AREA, CT_ALL} CALC_TYPE;				// 계산 종류
	typedef enum { RSAT_ITEM_COUNT_FIX, RSAT_ITEM_WIDTH_FIX}	RSA_TYPE;		// 리사이즈 봉(ITEM) 처리 방법	- [봉(ITEM)의 폭 고정 기능, 봉(ITEM)의 개수 고정 기능]
	
// 제공변수
public:
	int		m_nThread_Flag; //차트 성능 개선 관련 1:계산, 2:그리기

public:
	CXScaleManager(void)
	{		
	};
	
	virtual ~CXScaleManager(void) {};

	// Retrieve the RQ Index. (RQ is indexed in m_midxRQ. but why?)
	virtual int			GetRQIndexFromString( CString strRQ) = 0;

	virtual void					SetPacketListManager(CPacketListManager* ppacketListManager) = 0;
	virtual void					SetIndicatorList(CIndicatorList* pIndicatorList) = 0;
	virtual CPacketListManager *	GetPacketListManager( void) = 0;

	virtual void SetApplyToolBar(BOOL bApplyToolBar) = 0;								// 툴바에 반영할지의 여부를 임시 기록하기 위해서 사용
	virtual BOOL GetApplyToolBar() = 0;													// CMainBlockImplementation::CalculateDataForDrawing, CKoscomChartCtrl::SetStartEndIndex 참고

	virtual void SetReverse( BOOL bReverse) = 0;										// 뒤집기(REVERSE) 기능
	virtual BOOL GetReverse() = 0;

	virtual int	GetMinScaleInterval_nUnit( void) = 0;

	virtual void SetWeightedVolume(BOOL bWeightedVolume, BOOL bCalc) = 0;				// 거래량 가중치 기능
	virtual int	 GetWeightedVolume() = 0;

	virtual void SetFutureTimeCount( int p_nFutureTimeCount) = 0;									// 미래영역 확보 기능
	virtual int	 GetFutureTimeCount() = 0;

	virtual void SetLeftDataHideCount(int nLeftDataHideCount, BOOL bCalc) = 0;			// 좌측 데이터 숨기기 기능
	virtual int	 GetLeftDataHideCount() = 0;

	virtual BOOL GetUseOEBTMargin() = 0;												// 일목균형표 전용여백(사용여부)
	virtual void SetUseOEBTMargin(BOOL bUseOEBTMargin) = 0;
	
	virtual void SetRightMarginWidth( int nRightMarginWidth) = 0;						// 우측 여백 기능
	virtual void SetRightMarginAndFutureTime( int p_nRightMarginWidth, int p_nFutureTimeCount) = 0;		// (2009/5/17 - Seung-Won, Bae) for Right Margin and Future Time

	virtual void SetCandleTypeOnResize( RSA_TYPE rmtype) = 0;					// 우측 여백 동작 방식 설정
	virtual RSA_TYPE GetCandleTypeOnResize() = 0;
	
	// 봉간격 일정 : xScaleManager - ojtaso (20070528)
	virtual void SetFixedInterval(BOOL bFixedInterval, BOOL bCalc) = 0;
	virtual BOOL IsFixedInterval() = 0;

//	virtual void SetResizeType(RSA_TYPE rstype) = 0;									// 리사이즈 동작 방식 설정
//	virtual RSA_TYPE GetResizeType() = 0;

	virtual void AddOEBTRQ(CString strRQ, CString strGraphName, CString strIndicatorName) = 0;
																						// 일목 균형표가 소스로 사용중인 RQ 추가
	virtual void DelOEBTRQ( CString strRQ) = 0;
																						// 일목 균형표가 소스로 사용중인 RQ 제거
	virtual void Clear_OEBTRQ() = 0;													// 일목 균형표가 소스로 사용중인 RQ들의 목록을 모두 지워줍니다.

	virtual void Calc_DRI( BOOL p_bWithMerge = TRUE) = 0;													// 오리지날 데이터 (DRIfDTI, DTIfDRI...)를 다시 계산합니다.
	virtual void Calc_DRI_Merge( void) = 0;	// Calculate DRI and Time Line from DTI.
	virtual void RealDataReceived( CString strRQ, BOOL p_bIndexedReal = FALSE) = 0;		// REAL DATA 수신시에 호출

	virtual void SetBlockColumnCount(int nBlockColumn_Count) = 0;						// 블록컬럼의 갯수를 설정합니다.
	virtual void SetBlockColumn_Cur(int nBlockColumn) = 0;								// 다른 함수(스케일, 그래프 관련 함수)들이 기본으로 참고할 블록컬럼을 설정합니다.

	// OCX 세팅 : xScaleManager - ojtaso (20070504)
	virtual void SetIChartCtrl(IChartCtrl* p_pICharetCtrl) = 0;

	/**************************************************************************/
	/* 스크롤, 슬라이더 에서 사용하는 함수들								  */
	/**************************************************************************/
	virtual int GetCount() = 0;															// 전체 갯수를 얻어옵니다.
	virtual int GetDrawCount_Cur() = 0;													// 현재 화면에 보여지는 갯수를 얻어옵니다.
	virtual int GetStartDrawIdx_Cur() = 0;												// 현재 화면에 보여지는 영역의 시작 위치를 얻어옵니다.
	virtual int GetEndDrawIdx_Cur() = 0;												// 현재 화면에 보여지는 영역의 끝 위치를 얻어옵니다.
	virtual void SetStartEndDrawIdx_Cur(int nStartDrawIdx, int nEndDrawIdx, BOOL p_bMoreZoomIn = FALSE) = 0;		// 현재 화면에 보여지는 영역의 시작,끝 위치를 설정합니다.
	virtual void GetStartEndDrawIdx_Cur(int& nStartDrawIdx, int& nEndDrawIdx) = 0;		// 현재 화면에 보여지는 영역의 시작,끝 위치를 얻어옵니다.
	virtual int GetPageCountNextStep(const int nSliderPos_Prev,	const int nSliderPos_New, const int nSBCode, int nMin = -1, int nMax = -1) = 0;
																					// 다음 페이지로 이동할수 있는 봉(아이템)개수를 넘겨줍니다.
																						// (봉폭 고정시 봉(아이템)개수를 사용자가 마음대로 설정할수 없습니다.	
	/**************************************************************************/
	/* 그래프를 그릴때 사용하는 함수들										  */
	/**************************************************************************/
	virtual void GetDrawIndex(CString strRQ, BOOL bIncludePrev, BOOL bOEBT, 
							int& nDTI_Start, int& nDTI_End) = 0;						// 그래프가 현재 보여지는 영역에서 그려야할 영역 정보를 넘겨줍니다.
	// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
	virtual void GetDrawIndex(BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End) = 0;
	virtual int GetDTIFromDRI_AbleRange(CString strRQ, int nDRI, int* pnDTI_Org = NULL) = 0;
	
	/**************************************************************************/
	/* 스케일을 그릴때 사용하는 함수들										  */
	/**************************************************************************/
	// RQ별 Unit정보 관리 : 복수종목 - ojtaso (20070601)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetScaleUnit_Max(LPCTSTR lpszRQ = NULL) = 0;	// 스케일을 그릴때 사용할 Type정보입니다.
	virtual int GetScaleInterval_Unit_Max() = 0;
	
	virtual void SetCurScaleInfo(int nDRI = -1) = 0;									// 스케일 정보를 려고 하는 위치를 설정합니다.
	virtual void GetCurScaleInfo(int& nDRI) = 0;										// 스케일 정보를 얻으려고 하는 위치를 얻어옵니다.
	virtual BOOL NextScaleTime(time_t* ptimeScale, PAREA* pparea) = 0;					// 다음 스케일 위치의 시간 정보를 구합니다.
	virtual void PrevScaleTime(time_t* ptimeScale, PAREA* pparea) = 0;					// 이전 스케일 위치의 시간 정보를 구합니다.
	virtual void GetAt_ScaleTime(int nDRI, time_t* ptimeScale, PAREA* pparea) = 0;
	
	// 패킷타입 : xScaleManager - ojtaso (20070703)
	virtual CString GetPacketType(LPCTSTR lpszRQ) = 0;

	/**************************************************************************/
	/* 추세선, 십자선에서 사용할수 있는 함수								  */
	/**************************************************************************/
	
																					// 추세선에서 Pixel값에 해당하는 Data Index와 영역 정보를 구하고자 할때 사용합니다.
	virtual BOOL GetAREAfRQnDTI(CString strRQ, int nDTI, int nBlockColumn, PAREA parea) = 0;										
	// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
	virtual BOOL GetAREAfRQnDTI(int nDTI, int nBlockColumn, PAREA parea) = 0;
																						// 추세선에서 특정 RQ의 DTI에 해당하는 영역 정보를 구하고자 할때 사용합니다.
	virtual BOOL GetRQInfosFromPt(int nCursor_x, int& nBlockColumn, CStringArray& saRQ, CArray<int, int>& naDTI, PAREA parea) = 0;
																						// 십자선과 같은 곳에서 Pixel값에 RQ들, DTI들, DRI_AREA의 정보를 구하고자 할때 사용합니다.
	virtual time_t GetTimeFromX(int nCursor_x, int* pnBlockColumn = NULL, PAREA* pparea = NULL) = 0;
	

	// double값으로 변환 : xScaleManager - ojtaso (20070402)
	virtual double GetDoubleFromX( const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, PAREA* pparea, const char *p_szRQ = NULL) = 0;
	virtual int GetXFromDouble(const int nRow, const int nCol, double dblFind, PAREA* pparea = NULL) = 0;

	/**************************************************************************/
	/* 그밖에 곳에서 사용할수 있는 함수										  */
	/**************************************************************************/	
	virtual int GetScaleUnitAlphaWeight(CString strRQ) = 0;
	virtual BOOL IsUseable() = 0;

	virtual void GetTimeFromDRI(int nDRI, time_t* ptimeScale) = 0;
	// xScale 초기화 : xScaleManager - ojtaso (20070628)
	virtual void Initialize(BOOL bCalc = FALSE) = 0;
	// 문자형 데이타인지 : xScaleManager - ojtaso (20070703)
//	virtual BOOL IsStringDataType() = 0;
	// 컬럼갯수 : xScaleManager - ojtaso (20071128)
//	virtual int GetCountOfBlockColumn() = 0;

	virtual void SetThreadFlag(int nFlag) = 0;
	virtual int  GetThreadFlag() = 0;

// (2009/1/16 - Seung-Won, Bae) Do not call calc_area by any way except the OnDraw.
protected:
	friend UINT CKoscomChartCtrl_PrepareOnDraw( LPVOID pParam);

// (2009/1/16 - Seung-Won, Bae) Support the Re-Calculation Mode on OCX OnDrawing
public:
	virtual int		GetCalculationMode( void) = 0;	// 0:No-Calc, 1:CalcByReal, 2:CalcAll
	virtual void	SetNeedCalcAreaFlag( int p_nNeedCalcArea) = 0;

// (2009/1/19 - Seung-Won, Bae) for Scale Time List
public:
	virtual BOOL	IsSpecialTime( const char *p_szRQ, time_t p_time, long &p_lSpecialTimeCode) = 0;

// (2009/2/12 - Seung-Won, Bae) Support default data space.
public:
	virtual void	SetDefaultDataSpace( int p_nDefaultDataSpace, int p_nMinDataCountOnOnePage) = 0;

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
public:
	virtual void						Calc_AREA( int p_nColumnCount) = 0;													// 그리기 위해 필요한 정보만 다시 계산합니다.	
	virtual int							GetColumnWidth( int p_nColumn) = 0;
	virtual int							GetDRInFromPt( int nCursor_x, int *pnBlockColumn = NULL, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE, BOOL p_bWithBoundCheck = FALSE) = 0;	// 우측여백도 포함된 시간예상 : xScaleManager - ojtaso (20070608)
	virtual int							GetGraphXArea_End( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetGraphXArea_Left( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetGraphXArea_Right( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetGraphXArea_Start( int p_nColumn, BOOL bIncludeMargin = FALSE) = 0;
	virtual int							GetScaleDRIandTime( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
											const int p_nScaleRegionWidth, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nTimeInterval, 
											const char *p_szRQ, int &p_nSkipFactor, const CString &p_strPacketType) = 0;
	virtual int							GetScreenWidth( int p_nColumn) = 0;
	virtual int							GetXFromTime( int p_nColumn, time_t timeFind, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE) = 0;	// 우측여백도 포함된 X예상 : xScaleManager - ojtaso (20070608)
	virtual BOOL						GetDTInAREAfRQnPt( int nCursor_x, int& nBlockColumn, int* pnDTI, PAREA parea) = 0;	// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
	virtual BOOL						GetDTInAREAfRQnPt( CString strRQ, int nCursor_x, int& nBlockColumn, int* pnDTI = NULL, PAREA parea = NULL, BOOL p_bWithBoundCheck = FALSE) = 0;
	virtual BOOL						IsAllOnePixel( int p_nColumn, const char *p_szRQ) = 0;
	virtual BOOL						IsInGraphXArea( int p_nColumn, int nX, BOOL bIncludeEqual = FALSE) = 0;				// Include equal: 같은것 까지 포함해서 
	virtual void						GetAreaFromDRI( int p_nColumn, int nDRI, PAREA* parea) = 0;
	virtual void						GetGraphXAreaLR( int p_nColumn, int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE) = 0;
	virtual void						GetGraphXAreaSE( int p_nColumn, int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE) = 0;
	virtual void						SetColumnWidth( int p_nColumn, int p_nColumnWidth) = 0;
	virtual void						SetScreenRegion( int p_nColumn, int nLeft, int nWidth, BOOL p_bReCalc = FALSE) = 0;				// 화면 폭을 설정합니다.
	virtual CDataBlockList< AREA> *		GetDTIArea( int p_nColumn, const char *p_szRQ, int &p_nZeroDTI) = 0;	// (2009/1/10 - Seung-Won, Bae) Support Area array of DTI for drawing.

// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
public:
	virtual void	SetFlagForCalcDRIandScrollByReal( BOOL p_bWithTrucated = FALSE) = 0;
	virtual BOOL	IsScrollByReal( void) = 0;

// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
public:
	virtual void	OnUserZoomChanged( void) = 0;

// (2009/5/28 - Seung-Won, Bae) Check! Is Data Empty?
public:
	virtual BOOL	IsEmptyData( void) = 0;
	
	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	virtual void AddVerticalBoxInfo(LPCTSTR lpszRQ, double dHorzValue, COLORREF clrColor, BOOL bFill) = 0;
	virtual void GetVerticalBoxInfo(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, CRect& rcBox, COLORREF& clrColor, BOOL& bFill) = 0;

	virtual int		GetPriceMinMaxShowType( void) = 0;
	virtual void	SetPriceMinMaxShowType( int nMinMaxRatio) = 0;

	virtual int		GetScaleDRIandTimeAllData( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType,	const char *p_szRQ, int &p_nSkipFactor) = 0;

};

typedef CXScaleManager *(*FN_CreateXScaleManager)( void); 

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_XSCALEMANAGER_H__9BC10770_EEE0_4222_9E09_D247BF6D09EE__INCLUDED_)
