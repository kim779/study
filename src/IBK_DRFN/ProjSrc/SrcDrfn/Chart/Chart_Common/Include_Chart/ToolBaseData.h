// ToolBaseData.h: interface for the CToolBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBASEDATA_H__AB2749C8_2642_4A1B_9267_ADFEF7D5C985__INCLUDED_)
#define AFX_TOOLBASEDATA_H__AB2749C8_2642_4A1B_9267_ADFEF7D5C985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CToolEnumData
{
public:
	enum ElementType
	{
		Cross_Line = 1,	// 십자선
		Vert_Line,		// 수직선
		Horz_Line,		// 수평선
		Line,			// 추세선
		Trisection,		// 삼등분선
		Quadrisect,		// 사등분선
		Fibo_Semicircle,// 피보 반원
		Fibo_Circle,	// 피보 원
		Fibo_Fan_Line,	// 피보 펜라인
		Fibo_Retrace,	// 피보 되돌림(조정대)
		Fibo_Time,		// 피보 시간대
		Gann_Line,		// 갠라인
		Gann_Fan,		// 갠펜
		Gann_Grid,		// 갠그리드
		AndrewsPitchfork,// 엔드류 피치포크
		ElliotWave_Line,// 엘리오트 파동선
		Cycle_Line,		// 사이클 구간
		Cut_Line,		// 꺽은선 분석라인
		Speed_Line,		// 스피드 라인
		Linear_Regression,// 선형 회귀선(회귀선)
		Raff_Regression	// Raff 채널선(회귀채널)
	};

	enum DrawType
	{
		DRAW_NONE,
		DRAW_PEN,
		DRAW_NODROP
	};

	enum ModifyType
	{
		MODIFY_NONE,
		MODIFY_CENTER,
		MODIFY_START,
		MODIFY_MIDDLE,
		MODIFY_END,
		MODIFY_X,
		MODIFY_Y
	};

	enum PenStyle
	{
		Solid = 0,
		Dot,
		Dash,
		DashDot,
		DashDotDot
	};
};

#endif // !defined(AFX_TOOLBASEDATA_H__AB2749C8_2642_4A1B_9267_ADFEF7D5C985__INCLUDED_)
