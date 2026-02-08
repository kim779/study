#pragma once
class	Citem
{
public:
	DWORD	m_flag{};
	COLORREF m_clr{};
	CString	m_maps;

public:
	Citem*	Clone()
	{
		std::unique_ptr<Citem>	item;

		item = std::make_unique<Citem>();
		item->m_flag = m_flag;
		item->m_clr  = m_clr;
		item->m_maps = m_maps;
		return item.release();
	}
};


//
//	종목코드 \t 구분 \t 시간 \t 종목명 \t 특이사항
//	시간	: HHMM
//	구분	: XXYYYYYY
//	특이사항 : 공시/뉴스인 경우에만 전송
//

//
//	대구분
//
#define S_NEWS		0x01000000      // 뉴스/공시
#define S_QUOTE		0x02000000      // 시세분석
#define S_TREND		0x04000000      // 매매동향
#define S_CHART		0x08000000      // 차트신호

#define S_CANL		0x40000000      // 조건취소


//
//	뉴스/공시
//
#define F_GONG		0x00000001      // 공시
#define F_NEWS		0x00000002      // 종목뉴스

//
//	시세분석
//
#define F_JUN_KOGA	0x00000001      // 전일 고가 갱신
#define F_JUN_GVOL	0x00000002      // 전일 거래량 갱신
#define F_DAN_SHGA	0x00000004      // 당일 상한가 진입
#define F_DAN_HHGA	0x00000008      // 당일 하한가 진입
#define F_MAMUL_5D	0x00000010      // 최근 5일 매물대 돌파
#define F_MAMUL_60D	0x00000020      // 최근 60일 매물대 돌파
#define F_SHGA_5D	0x00000040      // 최근 5일 첫 상한가
#define F_GVOL_52M	0x00000080      // 52주 최고거래량 갱신
#define F_GVOL_5D	0x00000100      // 최근 5일 최고거래량 갱신
#define F_3D_UP_15P	0x00000200      // 최근 3일간 주가등락 15%이상
#define F_5D_UP_20P	0x00000400      // 최근 5일간 주가등락 20%이상
#define F_KOGA_5D	0x00000800      // 최근 5일 신고가 갱신
#define F_JEGA_5D	0x00001000      // 최근 5일 신저가 갱신
#define F_UPDN_5P	0x00002000      // 당일 최고/최저 대비 등락폭 5%이상
#define F_SIGA_5P	0x00004000      // 시가대비 5%이상 등락
#define F_UPXX_3D	0x00008000      // 연속 3일 상승
#define F_DOWN_3D	0x00010000      // 연속 3일 하락
#define F_MASU_200P	0x00020000      // 현재 매수비율 200%이상
#define F_RATE_20P	0x00040000      // 당일 거래량 회전율 20%이상
#define	F_RE_SHGA	0x00100000	// 상한가 재진입
#define	F_RE_HHGA	0x00200000	// 하한가 재진입
#define	F_DAN_KOGA	0x00400000	// 당일 신고가 진입
#define	F_DAN_JEGA	0x00800000	// 당일 신저가 진입

//
//	매매동향
//
#define F_WMASU		0x00000001      // 외국인 순매수
#define F_WMADO		0x00000002      // 외국인 순매도
#define F_WMASU_3D	0x00000004      // 외국인 3일 연속 순매수
#define F_WMADO_3D	0x00000008      // 외국인 3일 연속 순매도
#define F_TMASU_1	0x00000010      // 외국계증권사 순매수 만주이상
#define F_TMADO_1	0x00000020      // 외국계증권사 순매도 만주이상
#define F_TMASU_F	0x00000040      // 외국계증권사 첫 매수거래발생
#define F_TMADO_F	0x00000080      // 외국계증권사 첫 매도거래발생

//
//	차트신호
//
#define F_JUGA_GC	0x00000001      // 주가 골든크로스(5MA > 20MA)
#define F_JUGA_DC	0x00000002      // 주가 데드크로스(5MA < 20MA)
#define F_GVOL_GC	0x00000004      // 거래량 골든크로스(5MA > 20MA)
#define F_GVOL_DC	0x00000008      // 거래량 데드크로스(5MA < 20MA)
#define F_3SUN_UP	0x00000010      // 삼선전환도 양전환
#define F_3SUN_DN	0x00000020      // 삼선전환도 음전환
#define F_5MAJ_UP	0x00000040      // 주가 5MA 상향돌파
#define F_5MAJ_DN	0x00000080      // 주가 5MA 하향돌파
#define F_5MAG_UP	0x00000100      // 거래량 5MA 상향돌파

