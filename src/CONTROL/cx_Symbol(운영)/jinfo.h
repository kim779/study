#ifndef	_JINFO_H
#define	_JINFO_H

#pragma once


//
//	control symbol name : 17413
//

struct	_jinfo {
	char    codx[12];		// 종목코드
	char    hnam[50];		// 종목명
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '4':ETF, '6':ELW, '7':ELW-조기종료 
	char    size[12];		// 기업규모
	char    k2gb[20];		// 소구분 전기,전자
	char    usun[10];		// 보통주/우선주
	char    ksmm[2];		// 결산월
	char    amga[20];		// 액면가 8->20 단위포함
	char    jbkm[20];		// 자본금(단위:억) 12->20 단위포함
	char    siga[12];		// 전일시가총액(단위:억)
	char    sjjs[12];		// 상장주식수(단위:천주)
	char	jrat[3];		// 증거금율
	char	nrat[3];		// 신용증거금율 20070718 
	char    krgb[12];		// "거래정지", "거래중단", "관리종목", "이상급등", "투자유의", "정상"
	char    jggb[12];		// 장운영 구분
	char    lock[12];		// 락구분
	char    dyga[8];		// 대용가
	char    ujgb[12];		// 업종구분
	char    ujcd[6];		// RTS 업종코드
	char    jisu[8];		// 업종지수
	char    diff[8];		// 업종전일대비
	char    rate[8];		// 업종등략율
	char    gvol[12];		// 업종거래량(단위:천주)
	char    unio[6];		// KRX 100 (0:일반 1:통합지수)
	char	rned[2];		// 임의종료
	char	dist[2];		// 배분정보
	char	frgb[8];		// 외국인 보유비중
	char	rlnk[1];		// 대출등급('A'~'E', 'X', ' ')
	char    vist[1];		// 변동성 완화 장치
	char    vitime[6];		// 변동성 완화 장치 발동 시각
	char	vitype[1];		// 변동성 완화 장치 타입(정적,동적,정/동적)
	char	sijang[40];		// 시장조치
};

struct  _jinfo3 {			// 신 HTS 종목정보 (사이즈 줄인것)
	char    codx[12];		// 종목코드
	char    hnam[50];		// 종목명
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '4':ETF, '6':ELW, '7':ELW-조기종료
	char	jrat[3];		// 증거금율
	char	nrat[3];		// 신용증거금율 20070718 
	char    krgb[12];		// 관리구분
	char	rned[2];		// 임의종료
	char	dist[2];		// 배분정보
	char    vist[1];		// 변동성 완화 장치
	char    vitime[6];		// 변동성 완화 장치 발동 시각
	char	vitype[1];		// 변동성 완화 장치 타입(정적,동적,정/동적)
	char	fill[12];
};

#define L_jinfo		sizeof(struct _jinfo)
#define L_jinfo3	sizeof(struct _jinfo3)

class Cjinfo_small
{
public:
	Cjinfo_small()
	{
	}

	CString		codx;		// 종목코드
	CString		hnam;		// 종목명
	CString		jrat;		// 증거금율
	CString		nrat;		// 신용증거금율 20070718 
	CString		jggb;		// 장운영 구분
};

class Cjinfo
{
public:
	Cjinfo()
	{
		codx = hnam = jgub = size = k2gb = usun = ksmm = amga = _T("");
		jbkm = siga = sjjs = sijang = _T("");

		krgb = jggb = lock = dyga = ujgb = ujcd = _T("");
		jisu = diff = rate = gvol = unio = frgb = _T("");

		bNXT = FALSE;
	}

public:
	CString		codx;		// 종목코드
	CString		hnam;		// 종목명
	CString		jgub;		// 장구분 : '1':KOSPI, 2':KOSPI200, '3':KOSDAQ, '4 :프리보드, '5':ETF
	CString		size;		// 기업규모
	CString		k2gb;		// 소구분 전기,전자
	CString		usun;		// 보통주/우선주
	CString		ksmm;		// 결산월
	CString		amga;		// 액면가 -
	CString		jbkm;		// 자본금(단위:억) -
	CString		siga;		// 전일시가총액(단위:억)
	CString		sjjs;		// 상장주식수(단위:천주)
	CString		jrat;		// 증거금율
	CString		nrat;		// 신용증거금율 20070718 
	CString		krgb;		// 관리구분
	CString		jggb;		// 장운영 구분
	CString		lock;		// 락구분
	CString		dyga;		// 대용가
	CString		ujgb;		// 업종구분
	CString		ujcd;		// RTS 업종코드
	CString		jisu;		// 업종지수
	CString		diff;		// 업종전일대비
	CString		rate;		// 업종등략율
	CString		gvol;		// 업종거래량(단위:천주)
	CString		unio;		// KRX 100 (0:일반 1:통합지수)
	CString		frgb;		// 외국인 보유비중
	CString		rlnk;
//	CString		str2302;	//2012.05.04 KSJ 전일종가
	CString		str1377;	//2013.06.14 KSJ 매매단위
	CString		str1971;	//2013.06.14 KSJ 지정자문인
	CString		rned;		//임의종료
	CString		dist;		//배분정보
	CString		fill;		//필러
	CString		vist;		//변동성
	CString		vitime;		//변동성 발동 시각
	CString		vitype;		// 변동성 완화 장치 타입(정적,동적,정/동적)
	CString		sijang;		//시장조치
	BOOL       bNXT;
};

#endif // _JINFO_H