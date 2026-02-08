// GrpBasic.h: interface for the CGrpBasic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPBASIC_H__DB94F0BB_D862_488C_8847_7B4E17506989__INCLUDED_)
#define AFX_GRPBASIC_H__DB94F0BB_D862_488C_8847_7B4E17506989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
struct	_gBasic {
	int		fpr;			// 시가
	int		hpr;			// 고가
	int		lpr;			// 저가
	int		epr;			// 종가
	double		trn;			// 거래량
	double		amt;			// 거래대금
	struct _gIndex	index;

	unsigned char	check;			// information check
	WORD		rate;			// check rate
	int		mgjy;			// 미결제약정
//	WORD		ucnt;			// 상승종목수
//	WORD		dcnt;			// 하락종목수


	//**WORD		fobtain;		// 외구인 지분율
	//**WORD		fexhaust;		// 외국인 소진율
	//**int		fmmvol;			// 외국인 순매수량
	//** 2005.11.08 add by macho
	//int     fmmamt;			// 외국인 순매수금액
	//int     orgvol;			// 기관계 순매수량
	//int		orgamt;			// 기관계 순매수금액
	//int		invdata[6];

	int	orgnvol;
	int orgnamt;
	int frgnvol;
	int frgnamt;
	int fobtain;
	int fexhaust;
};
#define	sz_gBASIC	sizeof(struct _gBasic)
*/

#define	GD_UNUSED		-999999999

class CGrpBasic  
{
public:
	CGrpBasic();
	virtual ~CGrpBasic();

public:
	bool		m_bDummy;
	int		m_iOpen;		// 시가
	int		m_iHigh;		// 고가
	int		m_iLow;			// 저가
	int		m_iClose;		// 종가
	int		m_iMgjy;		// 미결제약정

	/** remarking by macho <begin>
	int		m_iOrgnVol;		// 투자자지표 기관(수량)
	int		m_iOrgnAmt;		// 투자자지표 기관(금액)
	int		m_iFrgnVol;		// 투자자지표 외국인(수량)
	int		m_iFrgnAmt;		// 투자자지표 외국인(금액)
	int		m_iFrgnObRate;		// 외국인 지분율
	int		m_iFrgnExRate;		// 외국인 소지율
	remarking by macho <end> **/

	//** add by macho <begin>
	int		m_frgnHave;		// 외국인보유현항
	int		m_frgnRate;		// 외국인보유비율;
	int		m_psnlPBuy;		// 개인순매수수량
	int		m_frgnPBuy;		// 외국인순매매수량
	int		m_orgnPBuy;		// 기관순매수수량
	int		m_psnlTBuy;		// 개인누적순매수수량
	int		m_frgnTBuy;		// 외국인누적순매수수량
	int		m_orgnTBuy;		// 기관누적순매수수량
	//** add by macho <end>

	// 2007.01.01 schbang : ADR 차트
	int		m_iUpCnt;
	int		m_iDnCnt;


//	__int64		m_iVolume;		// 거래량
//	__int64		m_iTrdMoney;		// 거래대금
	double		m_dVolume;		// 거래량
	double		m_dTrdMoney;		// 거래대금


	//unsigned char	m_ucCheck;		// information check
	DWORD		m_dwCheck;		// information check
	WORD		m_wRate;		// check rate
	


//	struct _gIndex	index;
	struct _gIndex{
		struct _date {
			short		yy;	// year
			unsigned char	mm;	// month
			unsigned char	dd;	// day
		}date;
		struct _time {
			unsigned char	hh;	// hour
			unsigned char	mm;	// minute
			unsigned char	ss;	// second
		}time;
	}		m_index;


private:

public:

private:


};

#endif // !defined(AFX_GRPBASIC_H__DB94F0BB_D862_488C_8847_7B4E17506989__INCLUDED_)
