
/* *********************************** */
/* 제작자: 이일렬(yiilyoul@magicn.com) */
/* 제작일: 2007.03.29                  */
/* *********************************** */

#pragma once
#pragma warning(disable: 4786)

#include <map>
#include <string>
#include <vector>
#include <set>

#include "../Include_Chart/Dll_Load/XScaleManager.h"
#include "../Include_Chart/DataMath.h"

#include "XScaleTimeDiffManager.h"						// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
#include "XScaleGridTime.h"								// for CXScaleGridTime

// DRI : Draw Index	- 그리기 위치
// DTI : Data Index	- 데이터 위치
// RQ  : Request	- 복수종목(다른 주기)을 처리하기 위해서는 이런 (종목+주기)를 부르는 용어가 필요하고 이 용어를 RQ라고 합니다.
// NS  : Next Step	- 다음 위치
// ITEM : 그려질때 하나의 항목을 의미합니다.
// OEBT : one eye balance table(일목 균형표...) ^^
// BC  : BlockColumn

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
#define _MAX_COL	10

class CPacket;
class CPacketRQ;
class CPacketList;
class CPacketListManager;
class CIndicatorList;
interface IChartCtrl;

class CXScaleManagerImp : public CXScaleManager
{
public:
	CXScaleManagerImp(void);

public:
	virtual ~CXScaleManagerImp(void);

public:
	// RQ의 정보를 가지는 구조체
	typedef struct tagRQ_INFO
	{
		CPacketRQ *							pPacketRQ;				// ?
		CPacketList *						pPacketList;			// ?
		CPacket *							pTimePacket;			// ?
		int									nDTI_Start_Data;		// the prev DTI on view (if none, it is the first DTI on view).
		int									nDTI_End_Data;			// the last DTI on view.

		CScaleBaseData::HORZSCALEDRAWERTYPE	eScaleUnit;				// the time type of RQ.
		int									nScaleInterval_Unit;	// the time unit of the time line of RQ
		
		// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		//int									nScaleInterval_Sec;		// Temporary Variable to calculate the Min/Max Scale Info.
		__int64									nScaleInterval_Sec;		// Temporary Variable to calculate the Min/Max Scale Info.
		// 2011.01.24 by SYS <<

		int									nLastRealDTI;			// the DTI that will be receive Real Data.
		BOOL								bRealReceived;			// the Flag if Real Data is received.
		int									nUpdatedRealDRI;		// the DRI that was changed by Real Data.
		BOOL								m_bStringDataType;		// 문자형 데이타인지 : xScaleManager - ojtaso (20070703)
		long								m_lEndSpecialTimeCode;	// (2009/5/28 - Seung-Won, Bae) the future can be steady special time.
	} RQ_INFO, *PRQ_INFO;

	// 일목균형표의 정보를 가지는 구조체
	typedef struct
	{
		CString strGraphName;
		int nFutureCount;
	} OEBT_INFO, *POEBT_INFO;

	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	typedef struct
	{
		double dHorzValue;
		COLORREF clrColor;
		BOOL bFill;
	} VERTBOX_INFO, *PVERTBOX_INFO;

	typedef map<string, CDataBlockList<AREA>*>		MAP_RQ_AREAfDTI;
	typedef pair<string, CDataBlockList<AREA>*>		PAIR_RQ_AREAfDTI;

	typedef map<string, int>						MAP_RQ_INDEX;
	typedef pair<string, int>						PAIR_RQ_INDEX;

	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	typedef map<string, VERTBOX_INFO*>				MAP_RQ_VERTBOX;
	typedef pair<string, VERTBOX_INFO*>				PAIR_RQ_VERTBOX;

	typedef struct tagBC_INFO
	{
		tagBC_INFO()
		{
			nScreenLeft = 0;
			nScreenWidth = 0;
		}

		int nScreenLeft;
		int nScreenWidth;

		int dItemWidth;
		int nItemWidth;

		MAP_RQ_AREAfDTI			mblAreafDTI;
		CDataBlockList<AREA>	blAreafDRI;
	} BC_INFO, *PBC_INFO;
	
	typedef vector<PBC_INFO>					VECTOR_BC_INFO;

	// 현재 그리기 위치 정보를 가지는 구조체
	typedef struct
	{
		CString strRQ;
		CDataBlockList<AREA>* pAreafDTI;
		int nDTI_Start_Data;
	} DRAW_INFO;

	// 현재 스케일 위치 정보를 가지는 구조체
	typedef struct
	{
		int nIndex;
	} SCALE_INFO;

// 내부변수
protected:

//	VECTOR_BC_INFO			m_vBCInfo;			// Block Column 별로 정보를 가지고 있습니다.
//	PBC_INFO				m_pbcinfo_cur;		// 현재 선택된 Block Column 정보를 가지고 있습니다.

	BOOL m_bApplyToolBar;				// 툴바에 반영할지의 여부를 기억하기 위한 내부 변수

	// 외부 객체들
	BOOL m_bReverse;					// 뒤집기
	BOOL m_bWeightedVolume;				// 거래량 가중치
	BOOL m_bUseOEBTMargin;				// 일목균형표 전용여백(사용여부)
	int	 m_nLeftDataHideCount;			// 좌측 데이터 숨기기
	int	 m_nRightMarginWidth;			// 우측 여백

	RSA_TYPE m_rstype;					// 리사이즈 동작 방식 ( RSAT_ITEM_WIDTH_FIX, RSAT_ITEM_COUNT_FIX )
	BOOL m_bFixedInterval;				// 봉간격 일정 : xScaleManager - ojtaso (20070528)

	int	 m_nDRI_Start_Cur;				// 현재 - 그리기 시작 DRI
	int	 m_nDRI_End_Cur;				// 현재 - 그리기 끝 DRI
	int	 m_nDrawCount_Cur;				// 현재 - 그리기 갯수
	
	int	m_nMinDataCountOnOnePage;		// 최소 Count
	
	SCALE_INFO	m_scaleinfo;			// 스케일을 그릴때 사용하는 현재 상태 정보

	AREA	m_areaNONE;
	AREA	m_areaTEMP;

	CPacketListManager* m_ppacketListManager;
	CIndicatorList* m_pIndicatorList;

	IChartCtrl* m_pIChartCtrl;	// ocx interface
	
// 내부함수
protected:
	inline BOOL EndOfRQs( int* naDTI_Cur);
	time_t GetLeastTime( int* naDTI_Cur, int* naIRQ, int* pnIRQ_Count);
	
	void Calc_RQ_Scale_MinMax();

	void ClearData();

	int GetNextDRI_ByScaleInterval(CScaleBaseData::HORZSCALEDRAWERTYPE ScaleUnit, int nScaleInterval_Unit, int nDRI_Cur);
	
	//time_t GetTimeFromTimeValue(unsigned __int64 lTime, CString strType, RQ_INFO* pifRQ, tm &tmResult, const char *p_szRQ, CMapTimeToLong *p_pamapSpecialTime = NULL);

	// 2011.01.27 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	time_t GetTimeFromTimeValue(unsigned __int64 lTime, CString strType, RQ_INFO* pifRQ, tm &tmResult, const char *p_szRQ, CMapTimeToLong *p_pamapSpecialTime = NULL);
	// 2011.01.27 by SYS <<

	// time_t를 double로 변환 : xScaleManager - ojtaso (20070405)
	double GetTimeValueFromTime(time_t tmValue, CString strType);

	int GetDTIFromDRI(CString strRQ, int nDRI);	

// 제공함수
public:
	virtual void					SetPacketListManager( CPacketListManager* ppacketListManager);
	virtual void					SetIndicatorList( CIndicatorList* pIndicatorList);
	virtual CPacketListManager *	GetPacketListManager( void)		{	return m_ppacketListManager;	}

	virtual void SetApplyToolBar(BOOL bApplyToolBar);								// 툴바에 반영할지의 여부를 임시 기록하기 위해서 사용
	virtual BOOL GetApplyToolBar();
	
	virtual void SetReverse( BOOL bReverse);										// 뒤집기(REVERSE) 기능
	virtual BOOL GetReverse();

	virtual void SetWeightedVolume(BOOL bWeightedVolume, BOOL bCalc);				// 거래량 가중치 기능
	virtual BOOL GetWeightedVolume();

	virtual void SetLeftDataHideCount(int nLeftDataHideCount, BOOL bCalc);			// 좌측 데이터 숨기기 기능
	virtual int	 GetLeftDataHideCount();
	
	virtual BOOL GetUseOEBTMargin();												// 일목균형표 전용여백(사용여부)
	virtual void SetUseOEBTMargin(BOOL bUseOEBTMargin);

	virtual void SetRightMarginWidth(int nRightMarginWidth);						// 우측 여백 기능

	virtual void SetCandleTypeOnResize( RSA_TYPE rmtype);							// 우측 여백 동작 방식 설정
	virtual RSA_TYPE GetCandleTypeOnResize();

	// 봉간격 일정 : xScaleManager - ojtaso (20070528)
	virtual void SetFixedInterval(BOOL bFixedInterval, BOOL bCalc);
	virtual BOOL IsFixedInterval();

//	virtual void SetResizeType(RSA_TYPE rstype);									// 리사이즈 동작 방식 설정
//	virtual RSA_TYPE GetResizeType();

	virtual void AddOEBTRQ( CString strRQ, CString strGraphName, CString strIndicatorName);
																					// 일목균형표가 소스로 사용중인 RQ 추가
	virtual void DelOEBTRQ( CString strRQ);
																					// 일목균형표가 소스로 사용중인 RQ 제거
	virtual void Clear_OEBTRQ();													// 일목균형표가 소스로 사용중인 RQ들의 목록을 모두 지워줍니다.

	virtual void RealDataReceived( CString strRQ, BOOL p_bIndexedReal = FALSE);		// REAL DATA 수신시에 호출

	// OCX 세팅 : xScaleManager - ojtaso (20070504)
	virtual void SetIChartCtrl(IChartCtrl* p_pICharetCtrl);

	/**************************************************************************/
	/* 스크롤, 슬라이더 에서 사용하는 함수들								  */
	/**************************************************************************/
	virtual int GetCount();															// 전체 갯수를 얻어옵니다.
	virtual int GetDrawCount_Cur();													// 현재 화면에 보여지는 갯수를 얻어옵니다.
	virtual int GetStartDrawIdx_Cur();												// 현재 화면에 보여지는 영역의 시작 위치를 얻어옵니다.
	virtual int GetEndDrawIdx_Cur();												// 현재 화면에 보여지는 영역의 끝 위치를 얻어옵니다.
	virtual void SetStartEndDrawIdx_Cur(int nStartDrawIdx, int nEndDrawIdx, BOOL p_bMoreZoomIn = FALSE);		// 현재 화면에 보여지는 영역의 시작,끝 위치를 설정합니다.
	virtual void GetStartEndDrawIdx_Cur(int& nStartDrawIdx, int& nEndDrawIdx);		// 현재 화면에 보여지는 영역의 시작,끝 위치를 얻어옵니다.
	virtual int GetPageCountNextStep(const int nSliderPos_Prev,	const int nSliderPos_New, const int nSBCode, int nMin = -1, int nMax = -1);
																					// 다음 페이지로 이동할수 있는 봉(아이템)개수를 넘겨줍니다.
																					// (봉폭 고정시 봉(아이템)개수를 사용자가 마음대로 설정할수 없습니다.
	
	/**************************************************************************/
	/* 그래프를 그릴때 사용하는 함수들										  */
	/**************************************************************************/
	virtual void GetDrawIndex(CString strRQ, BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End);
	// 현재 선택된 RQ를 자동으로 호출 : xScaleManager - ojtaso (20070409)
	virtual void GetDrawIndex(BOOL bIncludePrev, BOOL bOEBT, int& nDTI_Start, int& nDTI_End);

	virtual int GetDTIFromDRI_AbleRange(CString strRQ, int nDRI, int* pnDTI_Org = NULL);
	
	/**************************************************************************/
	/* 스케일을 그릴때 사용하는 함수들										  */
	/**************************************************************************/
	// RQ별 Unit정보 관리 : 복수종목 - ojtaso (20070601)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetScaleUnit_Max(LPCTSTR lpszRQ = NULL);			// 스케일을 그릴때 사용할 Type정보입니다.
	virtual int GetScaleInterval_Unit_Max();
	virtual void SetCurScaleInfo(int nDRI = -1);									// 스케일 정보를 려고 하는 위치를 설정합니다.
	virtual void GetCurScaleInfo(int& nDRI);										// 스케일 정보를 얻으려고 하는 위치를 얻어옵니다.
	
	// 패킷타입 : xScaleManager - ojtaso (20070703)
	virtual CString GetPacketType(LPCTSTR lpszRQ);

	/**************************************************************************/
	/* 그밖에 곳에서 사용할수 있는 함수										  */
	/**************************************************************************/	
	virtual int GetScaleUnitAlphaWeight(CString strRQ);
	virtual BOOL IsUseable();
	virtual void GetTimeFromDRI(int nDRI, time_t* ptimeScale);
	// xScale 초기화 : xScaleManager - ojtaso (20070628)
	virtual void Initialize(BOOL bCalc = FALSE);
	// 문자형 데이타인지 : xScaleManager - ojtaso (20070703)
//	virtual BOOL IsStringDataType();
	// 컬럼갯수 : xScaleManager - ojtaso (20071128)
//	virtual int GetCountOfBlockColumn();

	virtual void SetThreadFlag(int nFlag);
	virtual int	 GetThreadFlag();

	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	virtual void AddVerticalBoxInfo(LPCTSTR lpszRQ, double dHorzValue, COLORREF clrColor, BOOL bFill);
	virtual void GetVerticalBoxInfo(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, CRect& rcBox, COLORREF& clrColor, BOOL& bFill);

//	Global X scale data.
protected:
	int						m_nCountOfRQ_Area;	// It is previouis number of RQ for ReInitData_Area().
	int						m_nItemCount_Total;	// the count of time item in full time line with default data space.


// X Scale data for each RQ.
//	Manage the RQ with Index. but why?
protected:
	// Retrieve the RQ Index. (RQ is indexed in m_midxRQ. but why?)
	virtual int			GetRQIndexFromString( CString strRQ);

// (2008/12/28 - Seung-Won, Bae) Manage temp pointer variable to check NULL from the CDataBlockList's NULL return.
protected:
	const time_t *		m_pTReturn;
	const int *			m_pIReturn;
	const AREA *		m_pAReturn;
	const tm *			m_pMReturn;

// (2008/12/27 - Seung-Won, Bae) Comment.
//////////////////////////////////////////////////////////////////////
//	RQ Info (all data is array with index of RQ.)
//////////////////////////////////////////////////////////////////////
protected:
	int							m_nCountOfRQ;		// It is number of RQ.
													//	But it initialized with CPacketListManager's PacketList Count in Calc_DRI().
	MAP_RQ_INDEX				m_midxRQ;			// RQ Index.
	CStringArray				m_saRQ;				// RQ Names.
	RQ_INFO**					m_pifaRQ;			// RQ Infos.
	OEBT_INFO**					m_pifaOEBT;			// Obviously Balance Chart Infos.
	// 특정한 위치에 박스형태를 그림 (유진 2512 화면 요청) - ojtaso (20080709)
	MAP_RQ_VERTBOX				m_mifVertBox;

//////////////////////////////////////////////////////////////////////
//	Data indexed DTI for all RQs (all data is array with index of RQ.)
//////////////////////////////////////////////////////////////////////
protected:	// Data, second-indexed by DTI of RQs.
	CDataBlockList<time_t>**	m_pblaRQTime;		// the Time Line on DTI.
													//		It's order is DTI.
													//		Data count is same with Time Packet.
	CDataBlockList<int>**		m_pblaDRIfDTI;		// the DRI on DTI. 
													//		It has end-mark DTI.
													//			It is needed for Cacl_Area().
													//				Cacl_Area() calculate the Area from start DTI to end DTI of view with next DTI.
													//			It can be reset with future DRI.
													//		It's order is DTI.
	CMapTimeToLong **			m_pamapSpecialTime;	// the map of special time list.

//////////////////////////////////////////////////////////////////////
//	Data indexed DRI for all RQ (all data is array with index of RQ.)
//////////////////////////////////////////////////////////////////////
protected:	// Data indexed by DRI of RQs
	CDataBlockList<int>**		m_pblaDTIfDRI;		// the Mapping from DRI to DTI.
													//		It's order is DRI.
													//		It has negative(no matched) DTI for the future DRI.

//////////////////////////////////////////////////////////////////////
//	Main Time Line (full time line indexed DRI)
//////////////////////////////////////////////////////////////////////
protected:
	CDataBlockList<time_t>	m_blTimefDRI;		// XScale의 통일된 시간 정보
												// the full time line of X Scale Manager.
												//	the time item order is DRI.
												//	and It can have the future or hidden past.

// Maximum Scale Info for drawing the x-scale.
protected:
	int									m_maxScaleInterval_nRQ;
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_maxScaleInterval_eType;
	int									m_maxScaleInterval_nUnit;

// Minimum Scale Info to calculate the time of future.
protected:
	// Interval은 5분 차트의 경우에는 5가 됩니다.
	//	ex) 일,주,월 RQ의 경우에는 월 스케일을 그리도록 합니다.
	// 현재 스케일 타입 정보(가장 간격이 큰 정보를 기준으로 그리게 됩니다.)
	int									m_minScaleInterval_nRQ;
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_minScaleInterval_eType;
	int									m_minScaleInterval_nUnit;
protected:
	virtual int		GetMinScaleInterval_nUnit( void)	{	return m_minScaleInterval_nUnit;	}

// Retrieve the DTI from DRI
protected:
			//	Get DTI Value from DRI.
			//		if No DTI for past. -1
			//		if No DTI for future. End Mark DTI.
	int		GetDTIFromDRI( CDataBlockList< int>* pblDTIfDRI, int nDRI);
			//	Get DTI Index from DRI.
			//		if No DTI for past.		0
			//		if No DTI for future.	Last DTI. (can be calculated by EndMark.)
	int		GetDTIFromDRI_AbleRange( CDataBlockList< int>* pblDTIfDRI, int nDRI, int* pnDTI_Org = NULL);

// (2009/1/8 - Seung-Won, Bae) Manage the flags for recalc AREA
protected:
	BOOL	m_bWholeViewPre;

// (2009/1/11 - Seung-Won, Bae) for Comment.
protected:
	virtual void	Calc_DRI( BOOL p_bWithMerge = TRUE);													// 오리지날 데이터 (DRIfDTI, DTIfDRI...)를 다시 계산합니다.
	virtual void	Calc_DRI_Merge( void);	// Calculate DRI and Time Line from DTI.
							// It is called on
							//		Data Processing
							//			receiving new TR(add RQ)
							//				CKoscomChartCtrl::OnPacketTRData()
							//				CKoscomChartCtrl::SetPacketDataRQ()
							//			removing TR(RQ)
							//				CPacketListManagerImp::DeleteChartItem()
							//			receiving Real Data
							//				RealDataReceived() ================> Partial Calc_DRI !!!!!
							//			removeing Real Data
							//				CDirectRealReceiverAddInImp::SetRealPacketEndRQ()
							//		Indicator (Obviously Balance Chart or Special Chart)
							//			adding/deleting Glance Balance Chart
							//				AddOEBTRQ()
							//				DelOEBTRQ()
							//			chaning from/to special Chart.
							//				CKoscomChartCtrl::SetOneChart()
							//			recalcualtion Special Chart and Glance Balance Chart Condition.
							//				CKoscomChartCtrl::SetBEnableIndicatorCalculation()
							//				CKoscomChartCtrl::ReCalculateIndicator()
							//				CGraphImp::ChangeGraphDataFromIndicatorInfo()
							//		ETC
							//			SetLeftDataHideCount()
// (2009/1/13 - Seung-Won, Bae) for ReCalc AREA
protected:
	int				m_nNeedCalcArea;			// the Flag on by DRI() with full(2) or partial(1)
	int				m_nDrawCount_Pre;			// the data count on view
	int				m_nDRI_Start_Pre;			// the first data index on view
	BOOL			m_bWeightedVolume_Pre;		// the Weighted Volume option.
	BOOL			m_bReverse_Pre;				// the Reverse X option.
	int				m_nScreenLeft_Pre;			// the graph area left position on view
	int				m_nScreenWidth_Pre;			// the graph area width on view
	double 			m_daItemWidth_Pre;			// the one data width on view

// (2009/1/16 - Seung-Won, Bae) Support the Re-Calculation Mode on OCX OnDrawing
protected:
	int				m_nCalculationMode;			// 0:No-Calc, 1:CalcByReal, 2:CalcAll
	virtual int		GetCalculationMode( void)	{	return m_nCalculationMode;	}
	virtual void	SetNeedCalcAreaFlag( int p_nNeedCalcArea)
						{	if( m_nNeedCalcArea < p_nNeedCalcArea) m_nNeedCalcArea = p_nNeedCalcArea;	}

protected:
	int				m_nDRI_End_Pre;		// the last DRI Index has calculated AREA.

// (2009/2/12 - Seung-Won, Bae) Support default data space.
protected:
	int				m_nDefaultDataSpace;
protected:
	virtual void	SetDefaultDataSpace( int p_nDefaultDataSpace, int p_nMinDataCountOnOnePage);

// (2009/2/13 - Seung-Won, Bae) for Multi-Column.
protected:
	int						m_nCountOfBlockColumn;
protected:
	int						m_nColumnWidth[			_MAX_COL];		// (2009/3/11 - Seung-Won, Bae) Manage memory for no-reallocation.
	int						m_naScreenRegionLeft[	_MAX_COL];
	int						m_naScreenRegionWidth[	_MAX_COL];
	double					m_daItemWidth[			_MAX_COL];		// 현재 - 봉(ITEM)의 폭
protected:
	int						m_nScreenLeft_Cur[		_MAX_COL];		// 현재 - 화면왼쪽
	int						m_nScreenRight_Cur[		_MAX_COL];		// 현재 - 화면오른쪽
	int						m_nScreenLeftiM_Cur[	_MAX_COL];		// 현재 - 화면왼쪽(여백 포함)
	int						m_nScreenRightiM_Cur[	_MAX_COL];		// 현재 - 화면오른쪽(여백 포함)
	int						m_naScreenStart_Cur[	_MAX_COL];		// 현재 - 화면시작
	int						m_naScreenEnd_Cur[		_MAX_COL];		// 현재 - 화면끝
	int						m_nScreenStartiM_Cur[	_MAX_COL];		// 현재 - 화면시작(여백 포함)
	int						m_nScreenEndiM_Cur[		_MAX_COL];		// 현재 - 화면끝(여백 포함)
	int						m_nScreenWidth_Cur[		_MAX_COL];		// 현재 - 화면폭(여백 제외)
	int						m_naItemWidth[			_MAX_COL];		// 현재 - 봉(ITEM)의 폭
	CDataBlockList<AREA>	m_blAreafDRI[			_MAX_COL];		// Area Info for Full DRI
	CDataBlockList<AREA>**	m_pblaAreafDTI[			_MAX_COL];		// Area Info for All RQs
protected:
	virtual void					Calc_AREA( int p_nColumnCount);													// 그리기 위해 필요한 정보만 다시 계산합니다.	
									// It is called on only Drawing. but it is processed by the environment variables.
									//	the Environment Variables
									//		DRI info changing
									//			m_nNeedCalcArea
									//				2 : Calc_DRI()
									//				1 : RealDataReceived() - m_pifaRQ[ nRQ]->nUpdatedRealDRI ====> partial Calc_AREA !!!!!
									//		View Info
									//			m_nDrawCount_Cur
									//			m_nDRI_Start_Cur
									//			m_nScreenLeft_Cur
									//			m_nScreenWidth_Cur
									//		One Data Width
									//			m_daItemWidth
									//		Weighted Volume Info
									//			m_bWeightedVolume
protected:
	void					Calc_AREAofColumn( int p_nColumn);													// 그리기 위해 필요한 정보만 다시 계산합니다.	
	void					Calc_ItemWidth( int p_nColumn, int nDrawCount, BOOL p_bMoreZoomIn = FALSE);
	void					GetAreaFromDRI_Copy( int p_nColumn, int nDRI, AREA& area);
	void					ItemIntervalApply( int p_nColumn, AREA& area, int nDRI_Count);
	void					ItemIntervalApply_Reverse( int p_nColumn, AREA& area, int nDRI_Count);
protected:
	virtual int						GetColumnWidth( int p_nColumn)							{	return m_nColumnWidth[ p_nColumn];	}
	virtual int						GetDRInFromPt( int nCursor_x, int *pnBlockColumn, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE, BOOL p_bWithBoundCheck = FALSE);
	virtual int						GetGraphXArea_End( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetGraphXArea_Left( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetGraphXArea_Right( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetGraphXArea_Start( int p_nColumn, BOOL bIncludeMargin = FALSE);
	virtual int						GetScaleDRIandTime( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
										const int p_nScaleRegionWidth, const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType, const int p_nTimeInterval,
										const char *p_szRQ, int &p_nSkipFactor, const CString &p_strPacketType);
	virtual int						GetScreenWidth( int p_nColumn);
	virtual int						GetXFromTime( int p_nColumn, time_t timeFind, PAREA* pparea = NULL, BOOL bIncludeRightMargin = FALSE);	// 우측여백도 포함된 X예상 : xScaleManager - ojtaso (20070608)
	virtual BOOL					GetDTInAREAfRQnPt( int nCursor_x, int& nBlockColumn, int* pnDTI, PAREA parea);	
	virtual BOOL					GetDTInAREAfRQnPt( CString strRQ, int nCursor_x, int& nBlockColumn, int* pnDTI = NULL, PAREA parea = NULL, BOOL p_bWithBoundCheck = FALSE);
	virtual BOOL					IsAllOnePixel( int p_nColumn, const char *p_szRQ);
	virtual BOOL					IsInGraphXArea( int p_nColumn, int nX, BOOL bIncludeEqual = FALSE);
	virtual void					GetAreaFromDRI( int p_nColumn, int nDRI, PAREA* parea);
	virtual void					GetGraphXAreaLR( int p_nColumn, int& nLeft, int& nRight, BOOL bIncludeMargin = FALSE);
	virtual void					GetGraphXAreaSE( int p_nColumn, int& nStart, int& nEnd, BOOL bIncludeMargin = FALSE);
	virtual void					SetColumnWidth( int p_nColumn, int p_nColumnWidth)		{	m_nColumnWidth[ p_nColumn] = p_nColumnWidth;	}
	virtual void					SetScreenRegion( int p_nColumn, int nLeft, int nWidth, BOOL p_bReCalc = FALSE);	// 화면 폭을 설정합니다.
	virtual CDataBlockList< AREA> *	GetDTIArea( int p_nColumn, const char *p_szRQ, int &p_nZeroDTI);	// (2009/1/10 - Seung-Won, Bae) Support Area array of DTI for drawing.
protected:
	time_t					GetTimeFromXWithRightMargin(int nCursor_x, int* pnBlockColumn, PAREA* pparea); // 우측여백도 포함된 시간예상 : xScaleManager - ojtaso (20070608)
	virtual void			SetBlockColumnCount(int nBlockColumn_Count);						// 블록컬럼의 갯수를 설정합니다.
	virtual void			SetBlockColumn_Cur(int nBlockColumn);								// 다른 함수(스케일, 그래프 관련 함수)들이 기본으로 참고할 블록컬럼을 설정합니다.
	virtual BOOL			NextScaleTime(time_t* ptimeScale, PAREA* pparea);					// 다음 스케일 위치의 시간 정보를 구합니다.
	virtual void			PrevScaleTime(time_t* ptimeScale, PAREA* pparea);					// 이전 스케일 위치의 시간 정보를 구합니다.
	virtual void			GetAt_ScaleTime(int nDRI, time_t* ptimeScale, PAREA* pparea);
	virtual BOOL			GetAREAfRQnDTI(CString strRQ, int nDTI, int nBlockColumn, PAREA parea);			// 추세선에서 Pixel값에 해당하는 Data Index와 영역 정보를 구하고자 할때 사용합니다.
	virtual BOOL			GetAREAfRQnDTI(int nDTI, int nBlockColumn, PAREA parea);							// 추세선에서 특정 RQ의 DTI에 해당하는 영역 정보를 구하고자 할때 사용합니다.
	virtual BOOL			GetRQInfosFromPt(int nCursor_x, int& nBlockColumn, CStringArray& saRQ, CArray<int, int>& naDTI, PAREA parea);	// 십자선과 같은 곳에서 Pixel값에 RQ들, DTI들, DRI_AREA의 정보를 구하고자 할때 사용합니다.
	virtual time_t			GetTimeFromX(int nCursor_x, int* pnBlockColumn = NULL, PAREA* pparea = NULL);									
	virtual double			GetDoubleFromX(const int nRow, const int nCol, int nCursor_x, int* pnBlockColumn, PAREA* pparea, const char *p_szRQ = NULL);	// double값으로 변환 : xScaleManager - ojtaso (20070402)
	virtual int				GetXFromDouble(const int nRow, const int nCol, double dblFind, PAREA* pparea = NULL);

// (2009/5/12 - Seung-Won, Bae) for Fixed Item Width
protected:
	CRect			m_rctClient;
	BOOL			m_bOnResizing;
	BOOL			m_bOnUserZoomChanged;
	double			m_dLastItemWidth;
protected:
	virtual void	OnUserZoomChanged( void);

// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
protected:
	BOOL			m_bScrollByReal;
	BOOL			m_bTruncatedByReal;
protected:
	virtual void	SetFlagForCalcDRIandScrollByReal( BOOL p_bWithTrucated = FALSE);
	virtual BOOL	IsScrollByReal( void)	{	return m_bScrollByReal;	}

// (2009/5/17 - Seung-Won, Bae) for Future Time Count of Compare Chart with OEBT.
protected:
	int	 m_nFutureTimeCount;				// 미래영역 확보
protected:
	virtual int		GetFutureTimeCount();
	virtual void	SetFutureTimeCount(int nFutureTimeCount);									// 미래영역 확보 기능
	virtual void	SetRightMarginAndFutureTime( int p_nRightMarginWidth, int p_nFutureTimeCount);		// (2009/5/17 - Seung-Won, Bae) for Right Margin and Future Time

// (2009/5/28 - Seung-Won, Bae) Check! Is Data Empty?
protected:
	virtual BOOL	IsEmptyData( void);

// (2009/6/4 - Seung-Won, Bae) for ceiled Minute Scale with second time data.
protected:
	BOOL	m_bCeiling;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;


// 20080925 JS.Kim	서로 다른 시간대의 Scale 처리
protected:
	friend class CXScaleTimeDiffManager;
	CXScaleTimeDiffManager	m_TimeDiffManager;
	bool					m_bTimeScale;
protected:
	int						GetTimeDiffWithCurrentRQ( const CString p_strRQ);

// (2009/1/19 - Seung-Won, Bae) for Scale Time List
protected:
	CTypedPtrMap< CMapWordToPtr, signed short, CXScaleGridTime *>	m_mapXGridTime;

	int									m_naScaleType[ _MAX_COL];
	CDataBlockList< tm>					m_blScaleTime[ _MAX_COL];		// Time value for scale of YYYY/MM/DD/HH/mm/SS
	CDataBlockList< int>				m_blScaleDRI[ _MAX_COL];		// DRI value for scale of YYYY/MM/DD/HH/mm/SS
protected:	// Real X Scale Data.
	int									m_nSkipFactor;
	CScaleBaseData::HORZSCALEDRAWERTYPE	m_eScaleDrawType;
	CString								m_strScaleRQ;
protected:
	inline void				PopLastScaleDataTime( void);
	inline void				PushScaleDataTime( int p_nDRI, time_t p_timeDRI, BOOL p_bEndDRI);
protected:
	virtual BOOL			IsSpecialTime( const char *p_szRQ, time_t p_time, long &p_lSpecialTimeCode);

	int		m_nMinMaxRatio;
	void	SetPriceMinMaxShowType( int nMinMaxRatio) { m_nMinMaxRatio = nMinMaxRatio; }
	int		GetPriceMinMaxShowType( void)		{	return m_nMinMaxRatio;	}

public:
	//KHD : 시간 구분 
	CDataBlockList< tm>					m_blScaleTime2[ _MAX_COL];		// Time value for scale of YYYY/MM/DD/HH/mm/SS
	CDataBlockList< int>				m_blScaleDRI2[ _MAX_COL];		// DRI value for scale of YYYY/MM/DD/HH/mm/SS

	virtual int		GetScaleDRIandTimeAllData( int p_nColumn, CDataBlockList< int> *&p_pblScaleDRI, CDataBlockList< tm> *&p_pblScaleTime,
											const CScaleBaseData::HORZSCALEDRAWERTYPE p_eScaleDrawType,	const char *p_szRQ, int &p_nSkipFactor);

};


/*
	# 가중치 거래량 처리에 대한 정리 #

	1. 현재 소유하고 있는 RQ중 가장 작은 Scale을 기준으로 가중치를 계산한다.(동일할 경우 우선 검색된것)
	2. 현재 보여지는 영역(DRI영역)에서 1번이 소유한 DRI 영역에 대해서만 가중치를 계산하여 영역을 소유하고
	   나머지 DRI 영역은 기본적으로 cx/(DRI 개수)를 width로 소유하도록 한다.
	3. 추세선에서 RQ+DTI구조를 가지도록 해서 보여지지 않은 영역에 대해서는 cx/(DRI 개수) 기반으로 x축 좌표를 구해서 넘겨주도록 합니다.
	4. 삭제되었을때는 추세선에서 가지고 있는 정보도 업데이트 되어야 한다.
	   해서 이것을 업데이트 하도록 하는 작업을 추세선이 하도록 "RQ와 이동해야 하는 DTI의 갯수를" 넘겨서 정보를 업데이트 할수 있도록 한다.
*/

/*
	# 블록 컬럼 처리 #

	1. 블록 컬럼이 2개 이상이면 봉간격 고정 기능은 동작하지 않습니다.
	   이 경우 우측 여백 고정 기능으로 동작하게 됩니다.
*/