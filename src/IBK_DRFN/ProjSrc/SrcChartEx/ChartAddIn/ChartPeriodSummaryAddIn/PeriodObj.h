#pragma once

// CPeriodObj 명령 대상입니다.

// 클릭된 위치가 어느 영역에 포함되는 지
#define N_PT		0	// No point
#define S_PT		1	// Start point
#define E_PT		2	// End point
#define M_PT		4	// Mid point
#define C1_PT		8	// coner1 point
#define C2_PT		16	// coner2 point
#define ON_PT		31	// 선 위

#define OFFSET_RC_IN		4  // 전체 RC와 그리기 RC와의 offset
#define OFFSET_LT			20 // 라인과 테이블 사이의 기본(최소) offset
#define OFFSET_TT_IN		5	// 테이블과 텍스트 사이의 기본(최소) offset
#define RADIUS_OF_END_RC	2// 시작점 및 끝점의 원 영역 반지름
#define DEFAULT_HEIGHT		20 // pixels, 선의 기본 높이
#define TEXT_HEIGHT			15	// 단위 문자의 높이
#define TEXT_WIDTH			8	// 단위 문자의 폭 
#define TEXT_RC_OFFSET	CPoint(30,30)	// 텍스트의 기본 위치
#define WH_OF_MID_RC		5// 중간점애 표시할 정사각형의 폭/높이
#define UP					-1	// 위방향
#define DOWN				1	// 아래방향
#define MAX_NUM_OF_ROW_OF_TABLE	10	// 테이블의 최대 로 수
#define LINE_ON_SENSITIVITY	8	// 선위에 마우스 on했을 때의 민감도

// 20081009 박동관 >>
class CChartPeriodSummaryAddInImp;
// 20081009 박동관 <<
class CPeriodObj : public CObject
{
public:
	CPeriodObj();
	virtual ~CPeriodObj();

	//////////////////////////////////////////////////////////////////////////
	// 가격 관련 정보 클래스
	class CPrice
	{
	public:
		double sPrice;	// 시작일 종가
		double ePrice;	// 종료일 종가
		double max;		// 고
		double min;		// 저
		double avg;		// 평균
		void CalcAvg(int nCnt, double* dData)
		{
			for(int i=0; i<nCnt;i++)
				avg += dData[i];
			if(nCnt)
				avg /= nCnt;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// 거래량 관련 정보 클래스
	class CAmount
	{
	public:
		double max;		// 고
		double min;		// 저
		double avg;		// 평균
		void CalcAvg(int nCnt, double* dData)
		{
			for(int i=0; i<nCnt;i++)
				avg += dData[i];
			if(nCnt)
				avg /= nCnt;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// 본 관련 정보 클래스
	class CCandle
	{
	public:
		int count;		// 개수
		int plus;		// 고
		int minus;		// 저
		double ratio; 	// 비율 %
		double CalcRatio()
		{
			if(!minus) ratio = 100.;
			else ratio = (double)plus/(double)minus*100.;
			return ratio;
		};
		double CalRatio(int nPlus, int nMinus)
		{		
			if(nMinus)
				return nPlus/nMinus*1.;
			return 1.;
		};

	 };

	public:
	BOOL m_bSaved;			// 저장된 것 불러온 것인지 여부
	double m_dEClicked;		// 끝점 값
	double m_dSClicked;		// 시작점 값
	int m_nUpDownSymbol;	// 업/다운 심볼
	int m_nDateType;		// 일자 종류
	BOOL m_bSelected;		// 선택 여부
	int m_Num;				// 오브젝트 순서, 몇 번째....

	CRect m_rc;				// 전체 영역
	CRect m_rcLine;			// 라인 영역
	CRect m_rcTable;		// 테이블 영역
	CRect m_rcText;			// 텍스트 영역
	CRect m_rcCut;			// 라인 영역의 내부 제거용
	CRect m_rcCutS;			// 라인 영역의 내부 제거용
	CRect m_rcCutE;			// 라인 영역의 내부 제거용
	
	CPoint m_ptAOffset;		// 전체 영역의 절대 offset
	CPoint m_ptRLineOffset;	// 라인 영역의 전체 영역 대비 offset
	CPoint m_ptALineOffset;	// 라인 영역의 절대 offset = 라인 영역의 전체 영역 대비 offset + 전체 영역의 절대 offset
	CPoint m_ptRTableOffset;// 테이블 영역의 전체 영역 대비 offset
	CPoint m_ptATableOffset;// 테이블 영역의 절대 offset = 텍스트 영역의 전체 영역 대비 offset + 전체 영역의 절대 offset
	CPoint m_ptRTextOffset;	// 텍스트 영역의 전체 영역 대비 offset
	CPoint m_ptATextOffset;	// 텍스트 영역의 절대 offset = 텍스트 영역의 전체 영역 대비 offset + 전체 영역의 절대 offset
	CPoint m_ptLTOffset;	// 테이블 영역의 라인 영역대비 offset

	CRect m_oldrc;			// 영역 백업
	CPoint m_ptAStart;		// 시적점 절대 위치
	CPoint m_ptAEnd;		// 끝점 절대 위치
	CPoint m_ptRStart;		// 시적점 상대 위치
	CPoint m_ptREnd;		// 끝점 상대 위치
	CPoint m_ptAt;

	int m_CurClickedPosition;	//클릭된 위치

	// 텍스트 화면 표시 여부
	int m_nRowCnt;				// 행 수
	BOOL m_bSEPrice;			// 시작일 종가
	BOOL m_bEEPrice;			// 종료일 종가
	BOOL m_bHPrice;				// 고가
	BOOL m_bLPrice;				// 저가
	BOOL m_bMPrice;				// 평균가
	BOOL m_bHAmount;			//최대 거래량
	BOOL m_bLAmount;			// 최소 거래량
	BOOL m_bMAmount;			// 평균 거래량
	BOOL m_bPCNum;				//양봉 수
	BOOL m_bMCNum;				// 음봉 수
	BOOL m_bPMRatio;			//양음봉 비율

	int m_nTextAlignment;		// 텍스트 정렬
	//////////////////////////////////////////////////////////////////////////
	// 선 속성
	int m_nLineThickness;		// 선 굵기
	int m_nLineStyle;			// 선 스타일
	COLORREF m_clrLineColor;	// 선 색

	//////////////////////////////////////////////////////////////////////////
	// 데이터 관련
	//////////////////////////////////////////////////////////////////////////
	int m_nStartIndex;			// 시작 인덱스
	int m_nEndIndex;			// 끝 인덱스
	CString m_strSDate;			// 시작일자
	CString m_strEDate;			// 종료일자
	
	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//	long m_lSDate;				// 시작일자
	//	long m_lEDate;				// 종료일자
	__int64 m_lSDate;				// 시작일자
	__int64 m_lEDate;				// 종료일자
	// 2011.01.06 by SYS <<
	
	CString m_strPeriod;		// 기간	(2nd raw)
	CString m_strBasicInfo1;	// 기본 정보(1st raw)
	CString m_strBasicInfo2;	// 기본 정보(1st raw)
	CString m_strBasicInfo3;	// 기본 정보(1st raw)
	CPrice m_Price;				// 가격
	CAmount m_Amount;			// 거래량
	CCandle m_Candle;			// 봉
	int m_nTimeDiff;			// 사용자선택 시간대	// 20081007 JS.Kim	사용자시간대 선택

	// 20081009 박동관 부모의 포인터를 저장 >>
	void							SetChartPeriodSummaryAddInImp( CChartPeriodSummaryAddInImp *pChartPeriodSummaryAddInImp);
	CChartPeriodSummaryAddInImp *	GetChartPeriodSummaryAddInImp( void);
	CString							GetPacketType( void);
	// 20081009 박동관 <<
	
	// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CString							GetPacketDateType(void);	// 현재 주기의 날짜시간 Type을 얻는다.
	int								GetPacketDateUnit(void);	// 주기정보를 얻는다.
	// 2011.01.24 by SYS <<

	int IsInRegion(CPoint pt);		// 마우스 포인터가 해당 영역에 속하는지 여부. 선(IsOnLine)/끝점(IsOnEndPoint) 영역으로 구성됨 
	int IsOnLine(CPoint pt);		// 마우스 포인터가 선 위에 있는 지 여부
	BOOL IsOnEndPoint(CPoint pt);	// 마우스 포인터가 끝 점에 있는 지 여부
	void ButtonDownAt(const CPoint& pt);	// 위치 저장
	void ButtonUpAt(const CPoint& pt);		// 위치 저장
	void SetPos(CPoint pt, int type);		// 위치 저장	
	CPoint GetPos(int type);				// 위치 얻기
	void Draw(CDC* pDC, HWND p_hOcxWnd);			// 선그리기
	void DrawEndPoints(CDC* pDC);	// 끝점(시작, 종료 점) 그리기
	void SetUpDown(int nUpDown){m_nUpDown = nUpDown;}
	void DrawTable(CDC* pDC);		// 박스 및 텍스트 정보 그리기
	int m_nUpDown;
private:
	CSize m_size;
	void MakeData( HWND p_hOcxWnd);	// 기본 데이터(캔들 수, 가격, 기간) 계산
	CString CommaStr(CString in);	// 숫자 스트링에 콤마 넣기
	CRect GetRect();						// 화면 표시를 위한 각종 위지 정보, 그리기 영역 계산

	// 20081009 박동관 >>
	CChartPeriodSummaryAddInImp *	m_pChartPeriodSummaryAddInImp; // 부모포인터를 저장
	// 20081009 박동관 <<

	bool GetPacketTypeInfo(int& nDecimalDownCount, CString& strType) const;
	CString GetText_Price( double dPrice, const CString &p_strPacketType, int nDecimalDownCount) const;

// (2008/11/5 - Seung-Won, Bae) for Multiple Item.
public:
	CString		m_strRQ;
	int			m_nRow;
	int			m_nCol;
	CRect		m_rctGraphRegion;
	CRect		m_rctBlockRegion;
	BOOL		m_bLog;
	BOOL		m_bReverse;
	double		m_dViewMin;
	double		m_dViewMax;
	int			m_nStartIndexOfView;
	int			m_nEndIndexOfView;
	CPoint		m_pBaseLine;
};
