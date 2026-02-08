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
		Not_Element		= -1,
		Select_Element	= 0,
		Cross_Line		= 1,		// 십자선
		Vert_Line		= 3,		// 수직선
		Horz_Line		= 4,		// 수평선
		Line			= 5,		// 추세선
		Trisection		= 6,		// 삼등분선
		Quadrisect		= 7,		// 사등분선
		Fibo_Semicircle = 8,		// 피보 반원
		Fibo_Circle		= 9,		// 피보 원
		Fibo_Fan_Line	= 10,		// 피보 펜라인
		Fibo_Retrace	= 11,		// 피보 되돌림(조정대)
		Fibo_Time		= 12,		// 피보 시간대
		Gann_Line1		= 13,		// 갠라인(45)
		Gann_Line2		= 14,		// 갠라인(315)
		Gann_Fan1		= 15,		// 갠펜(45)
		Gann_Fan2		= 16,		// 갠펜(135)
		Gann_Fan3		= 17,		// 갠펜(225)
		Gann_Fan4		= 18,		// 갠펜(315)
		Gann_Grid		= 19,		// 갠그리드
		AndrewsPitchfork= 20,		// 엔드류 피치포크
		ElliotWave_Line	= 21,		// 엘리오트 파동선
		Cycle_Line		= 22,		// 사이클 구간
		Cut_Line		= 23,		// 꺽은선 분석라인
		Speed_Line		= 24,		// 스피드 라인
		Speed_Fan		= 25,		// 스피드 Fan
		Speed_Arc		= 26,		// 스피드 호
		Linear_Regression=27,		// 선형 회귀선(회귀선)
		Raff_Regression	= 28,		// Raff 채널선(회귀채널)
		Angle_Line		= 29,		// 각도 추세선
		Candle_Line		= 30,		// Candle중심선
		Candle_Bong		= 31,		// Candle가상봉
		Adjust_Ratio	= 32,		// 조정백분율
		Period_Summary	= 33,		// 기간요약
		Tracing_ToolTip	= 34,		// TracingToolTip
		Pencil_Line		= 35,		// 연필 추세선		
		Balance_Basic   = 36,		// 일목수치				// 2014.09.24 박성경 : 0010862: [7111] 삼성증권 차트 툴바의 [일목수치] 기능 IBK차트에도 동일하게 추가 요청
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
