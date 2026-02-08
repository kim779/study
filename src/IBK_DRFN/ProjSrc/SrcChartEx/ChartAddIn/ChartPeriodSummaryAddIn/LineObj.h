#pragma once

// CLineObj 명령 대상입니다.

// 클릭된 위치가 어느 영역에 포함되는 지
#define N_PT		0	// No point
#define S_PT		1	// Start point
#define E_PT		2	// End point
#define M_PT		4	// Mid point
#define ON_PT		31	// 선 위

#define OFFSET_RC_IN		4			// 전체 RC와 그리기 RC와의 offset
#define OFFSET_LT			20			// 라인과 테이블 사이의 기본(최소) offset
#define OFFSET_TT_IN		5			// 테이블과 텍스트 사이의 기본(최소) offset
#define RADIUS_OF_END_RC	2			// 시작점 및 끝점의 원 영역 반지름
#define DEFAULT_HEIGHT		20			// pixels, 선의 기본 높이
#define TEXT_HEIGHT			15			// 단위 문자의 높이
#define TEXT_WIDTH			8			// 단위 문자의 폭 
#define TEXT_RC_OFFSET	CPoint(30,30)	// 텍스트의 기본 위치
#define WH_OF_MID_RC		5			// 중간점애 표시할 정사각형의 폭/높이
#define UP					-1			// 위방향
#define DOWN				1			// 아래방향
#define MAX_NUM_OF_ROW_OF_TABLE	10		// 테이블의 최대 로 수
#define LINE_ON_SENSITIVITY	8			// 선위에 마우스 on했을 때의 민감도
enum{STAT_NONE, STAT_UP, STAT_DOWN};

class CLineObj : public CObject
{
public:
	CLineObj( HWND p_hOcxWnd);
	virtual ~CLineObj();
public:
	void GenerateSerial();
	double m_dSClicked;		// 시작점의 실질적인 Y 값
	double m_dEClicked;		// 종료점의 실질적인 Y 값
	int m_nDateType;		// 일, 주 윌 분 틱
	BOOL m_bSelected;		// 선택 여부
	int m_Num;				// 일련 번호

	CRect m_rc;				// 전체 영역
	CRect m_rcLine;			// 라인 영역
	CRect m_rcTable;		// 테이블 영역
	CRect m_rcText;			// 텍스트 영역
	CRect m_rcCut;			// 라인 영역의 내부 제거용

	CPoint m_ptAOffset;		// 전체 영역의 절대 offset
	CPoint m_ptRLineOffset;	// 라인 영역의 전체 영역 대비 offset
	CPoint m_ptALineOffset;	// 라인 영역의 절대 offset = 라인 영역의 전체 영역 대비 offset + 전체 영역의 절대 offset
	CPoint m_ptRTableOffset;// 테이블 영역의 전체 영역 대비 offset
	CPoint m_ptATableOffset;// 테이블 영역의 절대 offset = 텍스트 영역의 전체 영역 대비 offset + 전체 영역의 절대 offset
	CPoint m_ptRTextOffset;	// 텍스트 영역의 전체 영역 대비 offset
	CPoint m_ptATextOffset;	// 텍스트 영역의 절대 offset = 텍스트 영역의 전체 영역 대비 offset + 전체 영역의 절대 offset
	CPoint m_ptLTOffset;	// 테이블 영역의 라인 영역대비 offset

	CRect m_oldrc;			// 영역 백업
	CRect m_rcDraw;			// 그리기 영역 (text 영역 ∪ Line 영역)
	CPoint m_ptAStart;		// 시적점 절대 위치
	CPoint m_ptAEnd;		// 끝점 절대 위치
	CPoint m_ptRStart;		// 시적점 상대 위치
	CPoint m_ptREnd;		// 끝점 상대 위치
	CPoint m_ptAt;
	CPoint m_ptAExpand;		// 확장하여 바운드에 닿는 위치
	CPoint m_ptAIndexedStart;	// 인덱스에 해당하는 위치
	CPoint m_ptAIndexedEnd;		// 인덱스에 해당하는 위치

	int m_CurClickedPosition;	//클릭된 위치

	CString strType;		//packet type;

	//화면 표시 텍스트
	int m_nRowCnt;			// 행 수
	int m_nTextAlignment;	// 텍스트 정렬
	//////////////////////////////////////////////////////////////////////////
	// 선 속성
	//////////////////////////////////////////////////////////////////////////
	int m_nLineThickness;	// 선 굵기
	int m_nLineStyle;		// 선 스타일
	COLORREF m_clrLineColor;// 선 색

	//////////////////////////////////////////////////////////////////////////
	// 데이터 관련
	//////////////////////////////////////////////////////////////////////////
	CString m_strSerial;	// 종목코드+기준봉+처음생성일짜, 예)05930일20070215165013
	long m_lCode;			// 종목 코드
	CString m_strCode;		// 종목 코드
	CString m_strName;		// 종목명
	CString m_strCandle;		// 기준봉

	int m_nSIndex;		// 시작점 인데스
	int m_nEIndex;		// 종료점 인덱스

	CString m_strSDate;		// 시작일자
	CString m_strEDate;		// 종료일자

	// 2011.01.06 by SYS >> 분차트 YYYY(년) 추가
	//	long m_lSDate;			// 시작일자
	//	long m_lEDate;			// 종료일자
	__int64 m_lSDate;			// 시작일자
	__int64 m_lEDate;			// 종료일자
	// 2011.01.06 by SYS <<


	double m_dSData;		// 시작점 종가
	double m_dEData;		// 종료점 종가

	CString m_strOrderFlag;	// 부문여부 표시 ★,☆,◐,◑

	// 20081007 JS.Kim	사용자시간대 선택
	int		m_nTimeDiff;	// 선택 시간대

	//////////////////////////////////////////////////////////////////////////
	// 화면 표시 항목
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bUpSell;		// 상승 매도
	BOOL m_bUpBuy;		// 상승 매수
	BOOL m_bDownSell;	// 하락 매도
	BOOL m_bDownBuy;	// 하락 매수

	//////////////////////////////////////////////////////////////////////////
	// Alert 관리 항목
	//////////////////////////////////////////////////////////////////////////
	int m_nState;		//Alert 상태
	double m_dAlertPrice;	//Alert 가격

	//////////////////////////////////////////////////////////////////////////
	// 함수
	//////////////////////////////////////////////////////////////////////////
	void SetInfo();
	void DrawExpand(CDC* pDC);							// 사용하지 않음
	CString CommaStr(CString in);						// 사용하지 않음
	int IsInRegion(CPoint pt);							
	int IsOnLine(CPoint pt);
	BOOL IsOnEndPoint(CPoint pt);
	void Initilize(void);								// 초기화 - 사용하지 않음
	void ButtonDownAt(const CPoint& pt);				// 버튼 클릭다운 위치 저장
	void ButtonUpAt(const CPoint& pt);					// 버튼 클릭업 위치 저장
	CRect GetRect();									// 영역 및 offset 계산
	void SetPos(CPoint pt, int type);					// 타입에 따라 코지션 저장
	CPoint GetPos(int type);							// 타입에 따라 코지션 얻기
	CRect	Draw( CDC* pDC);								// 선 그리기
	void DrawEndPoints(CDC* pDC);						// 끝점 그리기
	void DrawTable(CDC* pDC);							// 텍스트 표시
	void Erase(CDC* pDC);								// 사용하지 않음
	void SetUpDown(int nUpDown){m_nUpDown = nUpDown;}	// 사용하지 않음
private:
	CSize m_size;	// 그리기 영역 크기
	int m_nUpDown;	// 사용하지 않음

	void ReCalcRect();	// 영역 재 계산
	void MakeData(void);

// (2008/3/4 - Seung-Won, Bae) for Multi-Language
protected:
	HWND		m_hOcxWnd;
	CString		m_strStar;
};
