#pragma once

#define MAX_R       30
#define TK_PIBO4013		(1)
#define TK_JISU 		(2)
#define TK_OPTION		(4)
#define TK_FUTURE		(100)

typedef struct _OPTION_HEADER {
	char	cjym[11][6];		// CALL 옵션 종목 타이틀
	char	pjym[11][6];		// PUT  옵션 종목 타이틀
} OPTION_HEADER;

typedef struct _FUTURE_HEADER {
    char    codx[8];        /* 단축코드 (8)                 */
    char    hnam[20];       /* 한글명                       */
    char    enam[20];       /* 영문명                       */
    char    mchk;           /* 매매유형허용                 */
							/* 0x01:시장가허용              */
							/* 0x02:조건부지정가허용        */
							/* 0x04:최유리지정가허용        */
} FUTURE_HEADER;

struct pibo4013_mid 
{
	char type[1];
	char prd[1];
    char func[1];    /* start    'F'         */
    char mymd[6];    /* 만기월               */
};

struct pibo4013_grid {
    char    c_rtcd[9];  /* 콜RTS CODE           */
	char	c_jega[8];
	char	c_koga[8];
	char	c_siga[8];
	char	c_raw[8];
	char	c_bega[8];
	char	c_theta[8];
	char	c_gamma[8];
	char	c_delta[8];
    char    c_mdif[8];  /* 콜증감(전일)         */
    char    c_mgjv[8];  /* 콜미결제약정         */
    char    c_gvol[8];  /* 콜거래량             */
    char    c_msga[8];  /* 콜매수               */
    char    c_mdga[8];  /* 콜매도               */
    char    c_rate[8];  /* 콜등락률             */
    char    c_diff[7];  /* 콜대비               */
    char    c_curr[8];  /* 콜현재가             */
	char    c_ycur[8];  /* 콜예상가				*/
	char	   c_ycvl[9];	// 콜예상수량 //0422
    char    hang[8];    /* 행사가               */
    char    jihs[8];    /* 지수환산             */
    char    p_rtcd[9];  /* 풋RTS CODE           */
    char    p_curr[8];  /* 풋현재가             */
	char    p_ycur[8];  /* 풋예상가             */
	char	   p_ycvl[9];	// 풋예상수량 //0422
    char    p_diff[7];  /* 풋대비               */
    char    p_rate[8];  /* 풋등락률             */
    char    p_mdga[8];  /* 풋매도               */
    char    p_msga[8];  /* 풋매수               */
    char    p_gvol[8];  /* 풋거래량             */
    char    p_mgjv[8];  /* 풋미결제약정         */
    char    p_mdif[8];  /* 풋증감(전일)         */
	char	p_delta[8];
	char	p_gamma[8];
	char	p_theta[8];
	char	p_bega[8];
	char	p_raw[8];
	char	p_siga[8];
	char    p_koga[8];
	char	p_jega[8];
};

struct pibo4013_mod {
    char    atmg[8];    /* ATM  */
    char    grec[4];
    struct  pibo4013_grid grid[MAX_R];
};

struct pibo4013_rank {
    int     hang;               /* 행사가       */
    int     jihs;               /* 지수환산산   */
    struct  mjopsi  *c_opsi;    /* call 옵션    */
    struct  mjopsi  *p_opsi;    /* put 옵션     */
};


typedef struct _OPTION_DATA
{
	char code[12];
	int     mdif;	// 증감(전일)
	int     mgjv;	// 미결제약정
	int     gvol;	// 거래량
	double  gjga;	// 기준가
	double  msga;	// 매수호가
	double  mdga;	// 매도호가
	double  rate;	// 등락률
	double  diff;	// 전일대비
	double  curr;	// 현재가
	double  ycurr;	// 예상가
	int ycuvl; //예상수량 0422
	double  hsga;	// 행사가
	double  jisu;	// 지수환산
	double  grdo;	// 괴리도
	double  irga;	// 이론가
	double  sggc;	// 시간가치
	double  rho;	// 로우
	double  vega;	// 베가
	double  teta;	// 세타
	double  gmma;	// 감마
	double  delt;	// 델타
	double  krxrho;	// 로우
	double  krxvega;	// 베가
	double  krxteta;	// 세타
	double  krxgmma;	// 감마
	double  krxdelt;	// 델타
	double  iv;	// IV
	double  siga; //시가
	double  koga; //고가
	double  jega; //저가
	BOOL b_mdif,b_mgjv,b_gvol,b_gjga,b_msga,b_mdga,b_rate,b_diff,b_curr,b_hsga,b_ycurr, b_ycuvl;
	BOOL b_jisu,b_grdo,b_irga,b_sggc,b_row,b_vega,b_teta,b_gmma,b_delt,b_iv,b_krxrow,b_krxvega,b_krxteta,b_krxgmma,b_krxdelt;
	BOOL b_siga,b_koga,b_jega;
	int  grid_row;

	struct _OPTION_DATA() { ZeroMemory(this, sizeof(struct _OPTION_DATA)); }
	void set_all(BOOL bSet) { b_krxrow = b_krxvega = b_krxteta = b_krxgmma = b_krxdelt = b_mdif = b_mgjv = b_gvol = b_gjga = b_msga = b_mdga = b_rate = b_diff = b_curr = b_ycurr = b_hsga = b_jisu = b_grdo = b_irga = b_sggc = b_row = b_vega = b_teta = b_gmma = b_delt = b_iv = b_jega = b_koga = b_siga = b_ycuvl = bSet; }
	void set_mdif(int     _mdif) { if (mdif != _mdif ) { mdif = _mdif; b_mdif = TRUE; }}
	void set_mgjv(int     _mgjv) { if (mgjv != _mgjv ) { mgjv = _mgjv; b_mgjv = TRUE; }}
	void set_gvol(int     _gvol) { if (gvol != _gvol ) { gvol = _gvol; b_gvol = TRUE; }}
	void set_gjga(double  _gjga) { if (gjga != _gjga ) { gjga = _gjga; b_gjga = TRUE; }}
	void set_msga(double  _msga) { if (msga != _msga ) { msga = _msga; b_msga = TRUE; }}
	void set_mdga(double  _mdga) { if (mdga != _mdga ) { mdga = _mdga; b_mdga = TRUE; }}
	void set_rate(double  _rate) { if (rate != _rate ) { rate = _rate; b_rate = TRUE; }}
	void set_diff(double  _diff) { if (diff != _diff ) { diff = _diff; b_diff = TRUE; }}
	void set_curr(double  _curr) { if (curr != _curr ) { curr = _curr; b_curr = TRUE; }}
	void set_ycurr(double  _ycurr) { if (ycurr != _ycurr ) { ycurr = _ycurr; b_ycurr = TRUE; }}
	void	set_ycuvl(int _ycuvl) { if (ycuvl != _ycuvl) { ycuvl = _ycuvl; b_ycuvl = TRUE; } }//0422
	void set_hsga(double  _hsga) { if (hsga != _hsga ) { hsga = _hsga; b_hsga = TRUE; }}
	void set_jisu(double  _jisu) { if (jisu != _jisu ) { jisu = _jisu; b_jisu = TRUE; }}
	void set_grdo(double  _grdo) { if (grdo != _grdo ) { grdo = _grdo; b_grdo = TRUE; }}
	void set_irga(double  _irga) { if (irga != _irga ) { irga = _irga; b_irga = TRUE; }}
	void set_sggc(double  _sggc) { if (sggc != _sggc ) { sggc = _sggc; b_sggc = TRUE; }}
	void set_rho (double  _row ) { if (rho  != _row  ) { rho  = _row ; b_row  = TRUE; }}
	void set_vega(double  _vega) { if (vega != _vega ) { vega = _vega; b_vega = TRUE; }}
	void set_teta(double  _teta) { if (teta != _teta ) { teta = _teta; b_teta = TRUE; }}
	void set_gmma(double  _gmma) { if (gmma != _gmma ) { gmma = _gmma; b_gmma = TRUE; }}
	void set_delt(double  _delt) { if (delt != _delt ) { delt = _delt; b_delt = TRUE; }}
	void set_krxrho (double  _krxrow ) { if (krxrho  != _krxrow  ) { krxrho  = _krxrow ; b_krxrow  = TRUE; }}
	void set_krxvega(double  _krxvega) { if (krxvega != _krxvega ) { krxvega = _krxvega; b_krxvega = TRUE; }}
	void set_krxteta(double  _krxteta) { if (krxteta != _krxteta ) { krxteta = _krxteta; b_krxteta = TRUE; }}
	void set_krxgmma(double  _krxgmma) { if (krxgmma != _krxgmma ) { krxgmma = _krxgmma; b_krxgmma = TRUE; }}
	void set_krxdelt(double  _krxdelt) { if (krxdelt != _krxdelt ) { krxdelt = _krxdelt; b_krxdelt = TRUE; }}
	void set_iv  (double  _iv  ) { if (iv   != _iv   ) { iv   = _iv  ; b_iv   = TRUE; }}
	void set_siga (double _siga) { if (siga  != _siga  ) { siga  = _siga ; b_siga  = TRUE; }}
	void set_koga (double _koga) { if (koga  != _koga  ) { koga  = _koga ; b_koga  = TRUE; }}
	void set_jega (double _jega) { if (jega  != _jega  ) { jega  = _jega ; b_jega  = TRUE; }}
	BOOL is_mdif() { const BOOL b=b_mdif; b_mdif=FALSE; return b; }
	BOOL is_mgjv() { const BOOL b=b_mgjv; b_mgjv=FALSE; return b; }
	BOOL is_gvol() { const BOOL b=b_gvol; b_gvol=FALSE; return b; }
	BOOL is_gjga() { const BOOL b=b_gjga; b_gjga=FALSE; return b; }
	BOOL is_msga() { const BOOL b=b_msga; b_msga=FALSE; return b; }
	BOOL is_mdga() { const BOOL b=b_mdga; b_mdga=FALSE; return b; }
	BOOL is_rate() { const BOOL b=b_rate; b_rate=FALSE; return b; }
	BOOL is_diff() { const BOOL b=b_diff; b_diff=FALSE; return b; }
	BOOL is_curr() { const BOOL b=b_curr; b_curr=FALSE; return b; }
	BOOL is_ycurr() { const BOOL b=b_ycurr; b_ycurr=FALSE; return b; }
	BOOL	is_ycuvl() { const BOOL b = b_ycuvl; b_ycuvl = FALSE; return b; }  //0422
	BOOL is_hsga() { const BOOL b=b_hsga; b_hsga=FALSE; return b; }
	BOOL is_jisu() { const BOOL b=b_jisu; b_jisu=FALSE; return b; }
	BOOL is_grdo() { const BOOL b=b_grdo; b_grdo=FALSE; return b; }
	BOOL is_irga() { const BOOL b=b_irga; b_irga=FALSE; return b; }
	BOOL is_sggc() { const BOOL b=b_sggc; b_sggc=FALSE; return b; }
	BOOL is_rho () { const BOOL b=b_row ; b_row =FALSE; return b; }
	BOOL is_vega() { const BOOL b=b_vega; b_vega=FALSE; return b; }
	BOOL is_teta() { const BOOL b=b_teta; b_teta=FALSE; return b; }
	BOOL is_gmma() { const BOOL b=b_gmma; b_gmma=FALSE; return b; }
	BOOL is_delt() { const BOOL b=b_delt; b_delt=FALSE; return b; }
	BOOL is_krxrho () { const BOOL b=b_krxrow ; b_krxrow =FALSE; return b; }
	BOOL is_krxvega() { const BOOL b=b_krxvega; b_krxvega=FALSE; return b; }
	BOOL is_krxteta() { const BOOL b=b_krxteta; b_krxteta=FALSE; return b; }
	BOOL is_krxgmma() { const BOOL b=b_krxgmma; b_krxgmma=FALSE; return b; }
	BOOL is_krxdelt() { const BOOL b=b_krxdelt; b_krxdelt=FALSE; return b; }
	BOOL is_iv  () { const BOOL b=b_iv  ; b_iv  =FALSE; return b; }
	BOOL is_siga() { const BOOL b=b_siga; b_siga =FALSE; return b; }
	BOOL is_koga() { const BOOL b=b_koga; b_koga =FALSE; return b; }
	BOOL is_jega() { const BOOL b=b_jega; b_jega =FALSE; return b; }
} OPTION_DATA;

typedef struct _FUTURE_DATA
{
	char code[12];
	char hnam[40];
	int ycuvl; //예상수량 0422
	double curr, diff, rate, irga, gryl, bsis, isis, gjga,ycurr;
	int gvol, mgjv, mdif;
	BOOL b_curr, b_diff, b_gjga, b_rate, b_irga, b_gryl, b_bsis, b_isis, b_gvol, b_mgjv, b_mdif, b_code, b_name, b_ycurr, b_ycuvl;
	int  grid_row;
	
	struct _FUTURE_DATA() { ZeroMemory(this, sizeof(struct _FUTURE_DATA)); }
	void set_code(LPCSTR _code) { sprintf(code, _code); b_code = TRUE; }
	void set_name(LPCSTR _name) { sprintf(hnam, _name); b_name = TRUE; }
	void set_curr(double _curr) { if (curr != _curr) { curr = _curr; b_curr = TRUE; } }
	void set_ycurr(double _ycurr) { if (ycurr != _ycurr) { ycurr = _ycurr; b_ycurr = TRUE; } }
	void set_ycuvl(int _ycuvl) { if (ycuvl != _ycuvl) { ycuvl = _ycuvl; b_ycuvl = TRUE; } }  //0422
	void set_diff(double _diff) { if (diff != _diff) { diff = _diff; b_diff = TRUE; } }
	void set_gjga(double _gjga) { if (gjga != _gjga) { gjga = _gjga; b_gjga = TRUE; } }
	void set_rate(double _rate) { if (rate != _rate) { rate = _rate; b_rate = TRUE; } }
	void set_irga(double _irga) { if (irga != _irga) { irga = _irga; b_irga = TRUE; } }
	void set_gryl(double _gryl) { if (gryl != _gryl) { gryl = _gryl; b_gryl = TRUE; } }
	void set_bsis(double _bsis) { if (bsis != _bsis) { bsis = _bsis; b_bsis = TRUE; } }
	void set_isis(double _isis) { if (isis != _isis) { isis = _isis; b_isis = TRUE; } }
	void set_gvol(int    _gvol) { if (gvol != _gvol) { gvol = _gvol; b_gvol = TRUE; } }
	void set_mgjv(int    _mgjv) { if (mgjv != _mgjv) { mgjv = _mgjv; b_mgjv = TRUE; } }
	void set_mdif(int    _mdif) { if (mdif != _mdif) { mdif = _mdif; b_mdif = TRUE; } }
	BOOL is_code() { const BOOL b=b_code; b_code=FALSE; return b; }
	BOOL is_name() { const BOOL b=b_name; b_name=FALSE; return b; }
	BOOL is_curr() { const BOOL b=b_curr; b_curr=FALSE; return b; }
	BOOL is_ycurr() { const BOOL b=b_ycurr; b_ycurr=FALSE; return b; }
	BOOL is_ycuvl() { const BOOL b = b_ycuvl; b_ycuvl = FALSE; return b; }
	BOOL is_diff() { const BOOL b=b_diff; b_diff=FALSE; return b; }
	BOOL is_rate() { const BOOL b=b_rate; b_rate=FALSE; return b; }
	BOOL is_irga() { const BOOL b=b_irga; b_irga=FALSE; return b; }
	BOOL is_gryl() { const BOOL b=b_gryl; b_gryl=FALSE; return b; }
	BOOL is_bsis() { const BOOL b=b_bsis; b_bsis=FALSE; return b; }
	BOOL is_isis() { const BOOL b=b_isis; b_isis=FALSE; return b; }
	BOOL is_gvol() { const BOOL b=b_gvol; b_gvol=FALSE; return b; }
	BOOL is_mgjv() { const BOOL b=b_mgjv; b_mgjv=FALSE; return b; }
	BOOL is_mdif() { const BOOL b=b_mdif; b_mdif=FALSE; return b; }
} FUTURE_DATA;

typedef struct _JISU_DATA
{
	char code[12];
	char hnam[40];
	char time[20];
	double curr, pcur;
	BOOL b_curr, b_pcur;
	struct _JISU_DATA() { ZeroMemory(this, sizeof(struct _JISU_DATA)); }
	void set_curr(double _curr) { if (curr != _curr) { curr = _curr; b_curr = TRUE; } }
	void set_pcur(double _pcur) { if (pcur != _pcur) { pcur = _pcur; b_pcur = TRUE; } }
	BOOL is_curr() { const BOOL b=b_curr; b_curr=FALSE; return b; }
	BOOL is_pcur() { const BOOL b=b_pcur; b_pcur=FALSE; return b; }
} JISU_DATA;

