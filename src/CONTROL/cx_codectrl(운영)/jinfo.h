#ifndef	_JINFO_H
#define	_JINFO_H

#if _MSC_VER > 1000
#pragma once
#endif


//
//	control symbol name : 17413
//
enum CODE_KIND { CK_HCODE = 0, CK_ELW };
struct _jHeader{
	char    codx[12];		// 종목코드
	char    hnam[32];		// 종목명
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '5':ETF
};

#define L_jHeader	sizeof(struct _jHeader)
// 현물종목
struct	_hjinfo {
	char    codx[12];		// 종목코드
	char    hnam[32];		// 종목명
	char    jgub[1];		// 장구분 : '1':거래소 '2':코스닥, '3' :제3시장, '5':ETF
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
					// 투자경고, 투자위험, 투자주의 
	char    jggb[12];		// 장운영 구분
	char    lock[12];		// 락구분
	char    dyga[8];		// 대용가
	char    ujgb[12];		// 업종구분
	char    ujcd[6];		// RTS 업종코드
	char    jisu[8];		// 업종지수
	char    diff[8];		// 업종전일대비
	char    rate[8];		// 업종등략율
	char    gvol[12];		// 업종거래량(단위:천주)
	char    unio[10];		// KRX 100 (0:일반 1:통합지수)

};

#define L_hjinfo		sizeof(struct _hjinfo)

class CjHeader
{
public:
	CjHeader()
	{
		codx = hnam = jgub = _T("");
	}

	virtual ~CjHeader()
	{
		TRACE("Delete CjHeader\n");	
	}

	CString		codx;		// 종목코드
	CString		hnam;		// 종목명
	CString		jgub;		// 장구분 : '1':KOSPI, 2':KOSPI200, '3':KOSDAQ, '4 :프리보드, '5':ETF
};

class Chjinfo : public CjHeader
{
public:
	Chjinfo() : CjHeader()
	{
		codx = hnam = jgub = size = k2gb = usun = ksmm = amga = _T("");
		jbkm = siga = sjjs = _T("");

		krgb = jggb = lock = dyga = ujgb = ujcd = _T("");
		jisu = diff = rate = gvol = unio = _T("");
	}
	
	virtual ~Chjinfo()
	{
		TRACE("Delete Chjinfo\n");
	}

public:
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
};

// ELW
struct  _ejinfo {            /* ELW 종목정보             */
	char    codx[12];       /* 종목코드                 */
	char    hnam[32];       /* 종목명                   */
	char    jgub[1];        /* ELW : 5                  */
	char    gcjs[20];       /* 기초자산                 */
	char    krye[10];       /* 권리유형                 */
	char    khbe[10];       /* 권리행사방법             */
	char    gjbe[10];       /* 결제방법                 */
	char    hbge[20];       /* 한글발행기관명           */
	char    sjjs[9];        /* 발행주식수 (천)          */
	char    sgca[9];        /* 발행가(시가총액:억)      */
	char    hsga[10];       /* 행사가격                 */
	char    jhre[10];       /* 전환비율                 */
	char    sjil[10];       /* 상장일자 (YYYY/MM/DD)    */
	char    edte[10];       /* 최종거래일 (YYYY/MM/DD)  */
	char    mgil[10];       /* 만기일자 (YYYY/MM/DD)    */
	char    jgil[10];       /* 지급일자 (YYYY/MM/DD)    */
	char    jjis[5];        /* 잔존일수                 */
	char    lpsa[20];       /* LP회원사                 */
	char    lpjg[4];        /* LP주문가능여부           */
	char    lpsu[15];       /* LP보유수량               */
	char    lpbr[6];        /* LP보유비율               */
};


#define L_ejinfo		sizeof(struct _ejinfo)

class Cejinfo : public CjHeader
{
public:
	Cejinfo() : CjHeader()
	{
		gcjs = krye = khbe = gjbe = hbge = sjjs = sgca = _T("");
		hsga = jhre = sjil = edte = mgil = _T("");
		jgil = jjis = lpsa = lpjg = lpsu = lpbr = _T("");
	}
	virtual ~Cejinfo()
	{
		TRACE("Delete Cejinfo\n");
	}

public:
	CString    gcjs;       /* 기초자산                 */
	CString    krye;       /* 권리유형                 */
	CString    khbe;       /* 권리행사방법             */
	CString    gjbe;       /* 결제방법                 */
	CString    hbge;       /* 한글발행기관명           */
	CString    sjjs;        /* 발행주식수 (천)          */
	CString    sgca;        /* 발행가(시가총액:억)      */
	CString    hsga;       /* 행사가격                 */
	CString    jhre;       /* 전환비율                 */
	CString    sjil;       /* 상장일자 (YYYY/MM/DD)    */
	CString    edte;       /* 최종거래일 (YYYY/MM/DD)  */
	CString    mgil;       /* 만기일자 (YYYY/MM/DD)    */
	CString    jgil;       /* 지급일자 (YYYY/MM/DD)    */
	CString    jjis;        /* 잔존일수                 */
	CString    lpsa;       /* LP회원사                 */
	CString    lpjg;        /* LP주문가능여부           */
	CString    lpsu;       /* LP보유수량               */
	CString    lpbr;        /* LP보유비율               */
};

#endif // _JINFO_H