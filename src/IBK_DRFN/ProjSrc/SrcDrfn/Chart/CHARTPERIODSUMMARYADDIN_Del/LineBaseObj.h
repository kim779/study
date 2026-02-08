// BaseObj.h: interface for the CBaseObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEBASEOBJ_H__4058CFD8_D932_4354_9DCF_F85543CA7678__INCLUDED_)
#define AFX_LINEBASEOBJ_H__4058CFD8_D932_4354_9DCF_F85543CA7678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// 정의문

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
#define LINE_ON_SENSITIVITY	4			// 선위에 마우스 on했을 때의 민감도


class CLineBaseObj : public CObject  
{
public:
	virtual CPoint GetPos(int nType);
	virtual void SetPos(CPoint pt, int nType);
	virtual void Draw(CDC* pDC);
	CLineBaseObj();
	virtual ~CLineBaseObj();

};

#endif // !defined(AFX_LINEBASEOBJ_H__4058CFD8_D932_4354_9DCF_F85543CA7678__INCLUDED_)
