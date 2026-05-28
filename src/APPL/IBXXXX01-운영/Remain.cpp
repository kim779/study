#include "stdafx.h"
#include "IBXXXX01.h"
#include "Remain.h"
#include "DefineAll.h"
#include <math.h>
#include "MapWnd.h"
#include "../../H/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TESTSAVEFILE	//2014.10.10 KSJ 로그를 남기고 싶으면 주석해제하면 된다.


CRemain::CRemain()
{
	m_pParent = NULL;

	m_code = m_name = m_remain = m_amt = m_pmaip = m_possible = m_pbep = "";
	m_curr = m_rprice = m_pgsonik = m_psuik = m_maip = "";
	m_date = m_creditS = m_symt = "";
	m_type = "";
	m_biyo = "";

	m_dFee =0;
	m_dMass = 0;//buffet
	m_dSave = 0;//buffet
	m_dCalcType = 0;
	m_iMcgb = 0;
	m_b3RDCode = false;
	m_bKONEXCode = false;
	m_bSJSuikCode = false;
	m_dTax = TAX_RATE;

	m_betf = false;
	m_bchg = false;		// 체결내역이 들어왔는지 체크.
	
	FillString(m_date, LEN_DATE);
	FillString(m_creditS, LEN_SYGB);
	FillString(m_symt, LEN_SYMT);

	m_mapCredit.RemoveAll();
	m_mapCreditX.RemoveAll();

	// 신용구분
	struct	_creditGubn
	{
		int	gubn;
		char	text[10];
		char	simple[4];
	};

	const _creditGubn creditGubn[] = 
	{
		{ 1, "유통융자 ", "유 " },
		{ 2, "단기융자 ", "단 " },
		{ 3, "자기융자 ", "융 " },
		{ 5, "유통대주 ", "유 " },
		{ 6, "매입자금 ", "매 " },
		{ 7, "자기대주 ", "자 " },
		{ 80, "예탁담보 ", "담 " },
		{ 10, "KOSPI    ", "   " },
		{ 20, "KOSDAQ   ", "   " },
		{ 30, "OTC      ", "   " },
		{ 40, "채권     ", "   " },
		{ 50, "대용     ", "대 " },
	};
	CString tmpS;
	for (int ii = 0; ii < sizeof(creditGubn)/sizeof(_creditGubn); ii++)
	{
		tmpS = CString(creditGubn[ii].text, sizeof(creditGubn[ii].text));
		tmpS.TrimRight();
		m_mapCredit.SetAt(creditGubn[ii].gubn, tmpS);

		tmpS = CString(creditGubn[ii].simple, sizeof(creditGubn[ii].simple));
		tmpS.TrimRight();
		m_mapCreditX.SetAt(creditGubn[ii].gubn, tmpS);
	}
}

CRemain::CRemain(CWnd* pMainWnd)
{
	CRemain();

	m_pParent = (CMapWnd*)pMainWnd;
}

CRemain::~CRemain()
{
	m_mapCredit.RemoveAll();
	m_mapCreditX.RemoveAll();
}

BEGIN_MESSAGE_MAP(CRemain, CWnd)
	//{{AFX_MSG_MAP(CRemain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CString CRemain::TotalData()
{
	//             0:code        1:name         2:결제잔고          3:주문잔량
	//             4:매입평균가       5:매도가능잔고      6:현재가       7:평가금액
	//             8:평가손익         9:평가수익률     10:매입금액     
	//  신용추가   11:대출일	12:신용구분	  13:신용구분내용     14:신용구분약자	15:신용대출금액
	//				16 : 잔고구분 17.BEP단가 18.수수료
	//m_biyo = "500";

	CString pmaip, data;
	pmaip.Format("%.f", atof(m_pmaip));

	/*double dval = trunc(atof(m_pmaip));
	pmaip.Format("%.f", dval);*/
	
	data = m_code + "\t" + m_name + "\t" + m_remain + "\t" + m_amt + "\t" +
		pmaip + "\t" + m_possible + "\t" + m_curr + "\t" + m_rprice + "\t" + 
		m_pgsonik + "\t" + m_psuik + "\t" + m_maip + "\t" + 
		m_date + "\t" + m_creditS + "\t" + getCredit(m_creditS) + "\t" + getCreditX(m_creditS) + "\t" + m_symt + "\t" +
		m_type + "\t" + m_pbep + "\t" + m_biyo + "\t" + m_srate + "\t";

	m_slog.Format("[jango][%s]<%d> [%s][%s]  평가손익(m_pgsonik)=[%s]  평가수익률(m_psuik)=[%s] 한주평균매입금액[pmaip]=[%s]  전체매입금액[m_maip]=[%s] 평가금액[m_rprice 계산값]=[%s]", 
		__FUNCTION__, __LINE__, m_code, m_name, m_pgsonik, m_psuik, pmaip, m_maip, m_rprice);
	Output_DebugString(m_slog);

	return data;
}

void CRemain::ParsingRemainData(char *pData)
{
	CString stmp;
	struct s_rec srec;
	CopyMemory(&srec, pData, L_srec);
	
	m_type = CString(srec.jggb, sizeof(srec.jggb));
	m_code	= CString(srec.cod2, sizeof(srec.cod2));
	m_name	= CString(srec.hnam, sizeof(srec.hnam));
	m_remain = CString(srec.jqty, sizeof(srec.jqty));
	m_amt = CString(srec.jqty, sizeof(srec.jqty));
	m_pmaip	= CString(srec.pamt, sizeof(srec.pamt));
	m_pmaip.Format("%f",atof(m_pmaip));

	m_maip	= CString(srec.mamt, sizeof(srec.mamt));
	m_omaip.Format("%.f", atof(m_maip));
	m_psuik	= CString(srec.srat, sizeof(srec.srat));
	m_possible = CString(srec.xqty, sizeof(srec.xqty));
	m_curr	= CString(srec.curr, sizeof(srec.curr));

#ifdef SISE_EXTEND
	m_srate = CString(srec.rate, sizeof(srec.rate));
#endif
	m_rprice = CString(srec.camt, sizeof(srec.camt));
	m_pgsonik = CString(srec.tamt, sizeof(srec.tamt));
	m_code.TrimRight();
#ifdef	CREDIT
	m_date = CString(srec.sydt, sizeof(srec.sydt));
	m_creditS = CString(srec.sycd, sizeof(srec.sycd));
//	if (atoi(m_creditS) >= 10 || atoi(m_creditS) == 0)	// 신용구분 아니면 0
//		m_creditS = "  ";
	m_symt = CString(srec.samt, sizeof(srec.samt));
stmp.Format("조회값 현재가=[%s] 매입금액=[%s] 평가금액=[%s]", m_curr, m_maip, m_rprice);

// 	CString strTemp;
// 	strTemp.Format("[KSJ][ParsingRemainData111] 융자금포함[%d] 신용[%s] 종목[%s](%s) dFee(%d) 수량(%d) 단가(%f) 매입금액(%.f) 원매입금액(%.f) 평가금액(%f) 체결여부(%d) 수익률(%f) 신용금액(%f) 현재가(%s)",
// 		m_dCalcType, m_creditS, m_name, m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg ? 1:0, atof(m_psuik), atof(m_symt), m_curr);
//putDebugString(strTemp);
// 	testSaveFile(strTemp);

//TRACE("잔고조회 [%s:%s] 수익[%s]\n", m_code, m_name, m_psuik);
m_slog.Format("[jango]---------------------------------------------------------");
Output_DebugString(m_slog);

m_slog.Format("[jango][%s]<%d> m_code[종목코드] =[%s]  m_name[종목명] =[%s]  m_remain[잔고수량] =[%s]  m_amt[잔고수량] =[%s] m_bSjsuik = [%d]",
	__FUNCTION__, __LINE__, m_code.Trim(), m_name.Trim(), m_remain.Trim(), m_amt.Trim() , m_bSJSuikCode);
Output_DebugString(m_slog);

m_slog.Format("[jango][%s]<%d> m_pmaip[평균단가] =[%s]  m_possible[주문가능수량] =[%s]  m_curr[현재가] =[%s] ",
	__FUNCTION__, __LINE__, m_pmaip.Trim(), m_possible.Trim(), m_curr.Trim());
Output_DebugString(m_slog);

m_slog.Format("[jango][%s]<%d>[%s][%s]  m_curr[현재가] =[%s]  m_rprice[평가금액] =[%s]  m_pgsonik[평가손익] =[%s]  m_psuik[평가수익률] =[%s]  m_maip[매입금액] =[%s]",
	__FUNCTION__, __LINE__, m_code.Trim(), m_name.Trim(), m_curr.Trim(),  m_rprice.Trim(), m_pgsonik.Trim(), m_psuik.Trim(), m_maip.Trim());
Output_DebugString(m_slog);

m_slog.Format("[jango][%s]<%d> m_dFee=[%d] m_dMass=[%.5f] m_dTax=[%.4f]",
	__FUNCTION__, __LINE__,
	m_dFee,
	m_dMass,
	m_dTax);
Output_DebugString(m_slog);

#else
	FillString(m_date, LEN_DATE);
	FillString(m_creditS, LEN_SYGB);
	FillString(m_symt, LEN_SYMT);

#endif


/*************NEW CALC******************************/
	double forby = 0;
	forby = atof(m_rprice);
	///////////////////////////////////////////
	double dSonik{};
	if ( m_dFee == 0 )
	{
		double dd{};

		//pyt 11.09
		if (m_dCalcType==1)	
		{
			if(m_creditS == "05")  //유통대주
			{
				m_maip.Format("%.f",atof(m_maip));//매입금액 + 신용금액
				m_rprice.Format("%.f",atof(m_rprice));//평가금액 + 신용금액
				
				dSonik = atof(m_rprice) - atof(m_maip);
				dSonik *= -1;
			}
			else
			{
				m_maip.Format("%.f",atof(m_maip)+atof(m_symt));//매입금액 + 신용금액
				m_rprice.Format("%.f",atof(m_rprice)+atof(m_symt));//평가금액 + 신용금액
				
				dSonik = atof(m_rprice) - atof(m_maip);
			}
		
			m_pgsonik.Format("%.f",dSonik);//평가손익
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			dd = atof(m_pgsonik)*100/(atof(m_maip));
		}
		else
		{
			dSonik = atof(m_rprice) - atof(m_maip);

			if(m_creditS == "05")  //유통대주 
				dSonik *= -1;

			m_pgsonik.Format("%.f",dSonik);//평가손익
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;

			dd = atof(m_pgsonik)*100/atof(m_maip);
		}
	
		m_psuik.Format("%.2f",dd);

		if (dd > 0)	
			m_psuik = "+" + m_psuik;
	}
	/////////////////////////////////////////////
	if ( m_dFee > 0)
	{
	//	double dMaip = atof(m_maip)*(1+m_dMass) + m_dSave;//매입가
		double	symt = atof(m_symt);

		if(m_creditS == "05")
			symt = 0;

		if(m_b3RDCode)// 2015.04.22 KSJ K-OTC일 경우 수수료가 0.15임. 제세금은 0.0005임
		{
			m_dTax = TAX_RATE_K_OTC;

			if(m_iMcgb == 0) //2015.04.22 KSJ 매체구분이 0:HTS/MTS/홈페이지, 1:영업점/콜센터, 2:ARS
			{
				m_dMass = 0.0015;
				m_dSave = 0;
			}
		}
		else if(m_bKONEXCode)
		{
			m_dTax = TAX_RATE_KONEX;
		}
		else if (m_bSJSuikCode)
		{
			m_dTax = TAX_RATE_SJSUIK;
		}
		else if (m_iCodetype == BDCpstType || m_iCodetype == BDCicyType)
		{
			m_dTax = 0;
		}
		else
		{
			m_dTax = TAX_RATE;
		}

		//double dMaip = atof(m_maip) + RoundDown((atof(m_maip)+symt) * m_dMass) + m_dSave;//매입가
		double dMaip{};
		// 매도수수료만 적용하게 수정
		if(m_dFee == 1)
		{
			if(m_creditS == "05")  //유통대주f
				dMaip = atof(m_maip) - RoundDown((atof(m_maip)+symt) * m_dMass) - m_dSave;//매입가
			else
				dMaip = atof(m_maip) + RoundDown((atof(m_maip)+symt) * m_dMass) + m_dSave;//매입가
		}
		else 
			dMaip = atof(m_maip);
		
		CString strTemp;
		strTemp.Format("[KSJ] dMaip[%.f] m_dMass[%.f] m_maip[%s] atof(m_maip)(%.f) RoundDown(%.f)", dMaip, m_dMass, m_maip, atof(m_maip), RoundDown((atof(m_maip)+symt) * m_dMass));
		Output_DebugString(strTemp);

		const double dmass = RoundDown((atof(m_rprice)+symt) * m_dMass);
		const double dtax = Round((atof(m_rprice)+symt) * m_dTax);			

		//수수료및 제세금 적용
		double dprice = 0;
		//if ( m_code.GetAt(0) == 'J' ) dprice = atof(m_rprice)*(1 - m_dMass ) - m_dSave;
		//else dprice = atof(m_rprice)*(1 - m_dMass - TAX_RATE ) - m_dSave;
	
		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 ) || m_betf || m_code.GetAt(0) == 'Q' || m_iCodetype == BDCpstType || m_iCodetype == BDCicyType) //2015.04.22 ETN추가
		{
			if(m_creditS == "05")  //유통대주
				dprice = atof(m_rprice) + ( dmass + m_dSave);
			else
				dprice = atof(m_rprice) - ( dmass + m_dSave);
		}	
		else
		{
			if(m_creditS == "05")  //유통대주
				dprice = atof(m_rprice) + (dmass + dtax + m_dSave);
			else	
				dprice = atof(m_rprice) - (dmass + dtax + m_dSave);
		}	
		
		double dSonik{};

		//pyt 11.09
		if (m_dCalcType==1)
		{	
			dMaip = dMaip + symt;
			m_maip.Format("%.f",dMaip);			//매입금액
			dprice = dprice + symt;				//평가금액	
			m_rprice.Format("%.f",dprice);
			dSonik = (dprice) - dMaip;
		}
		else
		{		
			m_maip.Format("%.f",dMaip);
			m_rprice.Format("%.f",dprice);		//평가금액
			dSonik = dprice - dMaip; 
		}

		if(m_creditS == "05")  //유통대주
			dSonik *= -1;
		
		m_pgsonik.Format("%.f",dSonik);//평가손익
		if (dSonik > 0)	
			m_pgsonik = "+" + m_pgsonik;
		 
		if ( dMaip == 0 )
			m_psuik.Format("%.2f",0.0);
		else
		{
			double dSuik{};
			if (m_dCalcType==1)
				dSuik = dSonik*100/dMaip;//dSuik = dSonik*100/(dMaip+symt);
			else
				dSuik = dSonik*100/dMaip;

			m_psuik.Format("%.2f",dSuik);

			if (dSuik > 0)
				m_psuik = "+" + m_psuik;
		}

//		TRACE("매입 %f 신용 %f 평가 %f 손익 %F\n", dMaip, symt, dprice, dSonik);
	}
	
	

	double dbep = atof(m_pmaip);

	if(m_b3RDCode)// 2015.04.22 KSJ K-OTC일 경우 수수료가 0.15임. 제세금은 0.0005임
	{
		m_dTax = TAX_RATE_K_OTC;
		
		if(m_iMcgb == 0) //2015.04.22 KSJ 매체구분이 0:HTS/MTS/홈페이지, 1:영업점/콜센터, 2:ARS
		{
			m_dMass = 0.0015;
			m_dSave = 0;
		}
	}
	else if(m_bKONEXCode)
	{
		m_dTax = TAX_RATE_KONEX;
	}
	else if (m_bSJSuikCode)
	{
		m_dTax = TAX_RATE_SJSUIK;
	}
	else if (m_iCodetype == BDCpstType || m_iCodetype == BDCicyType)
	{
		m_dTax = 0;
	}
	else
	{
		m_dTax = TAX_RATE;
	}

	const double bep_mass = RoundDown2(fabs(atof(m_pmaip)) * m_dMass); 

	const double cur_tax = RoundDown2(fabs(atof(m_creditS == "05" ? m_pmaip:m_curr)) * m_dTax);
	const double cur_mass = RoundDown2(fabs(atof(m_curr)) * m_dMass);//TAX_RATE);
	
	if ( m_dFee > 0 )
	{
	//	if ( m_code.GetAt(0) == 'J' ) dbep = fabs(atof(m_pmaip))*(1+m_dMass) + fabs(atof(m_curr))*m_dMass;
	//	else dbep = fabs(atof(m_pmaip))*(1+m_dMass) + fabs(atof(m_curr))*(m_dMass+TAX_RATE);

		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )|| m_betf || m_code.GetAt(0) == 'Q' || m_iCodetype == BDCpstType || m_iCodetype == BDCicyType) //2015.04.22 ETN추가
		{	
			if(m_creditS == "05")  //유통대주 // (매도금액 - 수수료)/(수량 * (1 + 수수료율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 + m_dMass));
			else					// (매수금액 + 매수수수료)/(수량*(1-수수료율-제세금율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass));
			/*
			if(m_creditS == "05")  //유통대주
				dbep = fabs(atof(m_pmaip)) - bep_mass - cur_mass;
			else
				dbep = fabs(atof(m_pmaip)) + bep_mass + cur_mass;
			*/
			m_biyo.Format("%d",(UINT) (RoundDown2(fabs(forby) * m_dTax)+ RoundDown2(fabs(forby) * m_dMass)));
		}	
		else
		{
			if(m_creditS == "05")  //유통대주  //유통대주 // (매도금액 - 수수료 - 제세금)/(수량 * (1 + 수수료율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass) - Round(fabs(atof(m_curr))*atoi(m_remain) * m_dTax))/(atoi(m_remain) * (1 + m_dMass ));
			else                  // (매수금액 + 매수수수료)/(수량*(1-수수료율-제세금율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass - m_dTax));
			/*
			if(m_creditS == "05")  //유통대주
				dbep = fabs(atof(m_pmaip)) - bep_mass - cur_tax - cur_mass;
			else
				dbep = fabs(atof(m_pmaip)) + bep_mass + cur_tax + cur_mass;
			*/
			m_biyo.Format("%d",(int)(RoundDown2(fabs(forby) * m_dTax)+RoundDown2(fabs(forby) * m_dMass) + m_dSave));
		}	
	}		

	m_pbep.Format("%.f",dbep);

	m_symt.TrimLeft(); m_symt.TrimRight();

	CString strTemp;
	strTemp.Format("[KSJ][ParsingRemainData222] 융자금포함[%d] %s 신용[%s] 종목[%s](%s) dFee(%d) 수량(%d) 단가(%f) 매입금액(%.f) 원매입금액(%.f) 평가금액(%f) 평가손익(%f)  체결여부(%d) 수익률(%f) 신용금액(%f) 현재가(%s) TAX(%f) 수수료(%f)",
		m_dCalcType, stmp, m_creditS, m_name, m_code, m_dFee, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), atof(m_pgsonik), m_bchg ? 1:0, atof(m_psuik), atof(m_symt), m_curr, m_dTax, m_dMass);
	Output_DebugString(strTemp);
// 	testSaveFile(strTemp);

/**************************************************/
	TRACE("PR[%x][%x] 종목(%s) 수량(%d) 단가(%f) 매입금액(%.f) 원매입금액(%.f) 평가금액(%f) 수익률(%f)\n",
		this, GetCurrentThreadId(),	m_code, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), atof(m_psuik));

	m_slog.Format("[jango][%s]<%d> m_dFee=[%d] m_dMass=[%.5f] m_dTax=[%.4f] codetype=[%d]",
		__FUNCTION__, __LINE__,
		m_dFee,
		m_dMass,
		m_dTax, m_iCodetype);
	Output_DebugString(m_slog);

	m_slog.Format("[jango][%s]<%d> m_dFee -------------------------------",
		__FUNCTION__, __LINE__);
	Output_DebugString(m_slog);
}

void CRemain::ParsingAllaccnData(CString sData)
{
	m_code	= parser(sData, "\t");
	m_name	= parser(sData, "\t");
	m_remain = parser(sData, "\t");
	m_amt = parser(sData, "\t");
	m_pmaip	= parser(sData, "\t");
	m_maip	= parser(sData, "\t");
	m_omaip = m_maip;
	m_psuik	= parser(sData, "\t");
	m_possible = parser(sData, "\t");
	m_curr	= parser(sData, "\t");
	m_rprice = parser(sData, "\t");
	m_pgsonik = parser(sData, "\t");
	m_code.TrimRight();	

	m_date = parser(sData, "\t");
	if (m_date.IsEmpty())
		FillString(m_date, LEN_DATE);

	m_creditS = parser(sData, "\t");
//	if (atoi(m_creditS) >= 10 || atoi(m_creditS) == 0)	// 신용구분 아니면 0
//		m_creditS = "  ";

	m_symt = parser(sData, "\t");
	if (m_symt.IsEmpty())
		FillString(m_symt, LEN_SYMT);
	

	//FillString(m_date, LEN_DATE);
	//FillString(m_creditS, LEN_SYGB);
	//FillString(m_symt, LEN_SYMT);
	
}

int CRemain::CalRemainData(CMapStringToString &symbolMap, CString keyS, bool bNewCode)
{
	CString stateS, gubnS, tmpS;
	CString nContAmt, cpriceS, strMod;
	int nMod = 1;

	CString jggbS = keyS.Left(LEN_JGGB);
	//CString codeS = keyS.Mid(LEN_JGGB, LEN_HCODE);
	CString codeS;
	if(keyS.Find("N.") >= 0 || keyS.Find("M.") >= 0)
		codeS = keyS.Mid(LEN_JGGB, LEN_HCODE + 2);
	else
		codeS = keyS.Mid(LEN_JGGB, LEN_HCODE);

	CString dateS = keyS.Mid(LEN_JGGB + LEN_HCODE, LEN_DATE);

	m_slog.Format("[jango][%s]<%d> 체결 시작  keyS=[%s] , jggbS=[%s] codeS=[%s]  dateS=[%s] ",
		__FUNCTION__, __LINE__, keyS, jggbS,codeS, dateS);
	Output_DebugString(m_slog);


	symbolMap.Lookup(RT_CTDATE, m_date);		// 대출일
	if (m_date.IsEmpty())
	{
		FillString(m_date, LEN_DATE);
	}

	symbolMap.Lookup(RT_STATE, stateS);
	symbolMap.Lookup(RT_GUBN, gubnS);

	symbolMap.Lookup(RT_CREDIT, m_creditS);		// 신용구분
	const int credit = atoi(m_creditS);

	bool bCredit = false;

//	if (credit >= 10 || credit == 0)		// 신용구분 아니면 0
//		m_creditS = "  ";
//	else
//	{
		m_creditS.Format("%02d", credit);
		bCredit = true;
//	}

	const double	symt = atof(m_symt);
	double	tmpSymt = 0.0;
	CString tmpSymtS = "";
	if (symbolMap.Lookup(RT_CTSYMT, tmpSymtS))
	{
		tmpSymt = atof(tmpSymtS);
		if (tmpSymt > 0.0)
		{
			if (gubnS.Find("매도") != -1)
			{
				switch (credit)
				{
				case 1:		// 유통융자
				case 3:		// 자기융자
				case 60:	// 수익증권담보대출
				case 70:	// 매도대금담보대출
				case 80:	// 예탁주식담보신용대출
				case 82:	// 예탁채권담보신용대출
				//case 50:    // 대용
					tmpS.Format("%.f", symt - tmpSymt);
					break;
				case 5:		// 유통대주
				case 7:		// 자기대주
					tmpS.Format("%.f", symt + tmpSymt);
					break;
				}
			}
			else	// 매수
			{
				switch (credit)
				{
				case 1:		// 유통융자
				case 3:		// 자기융자
				case 60:	// 수익증권담보대출
				case 70:	// 매도대금담보대출
				case 80:	// 예탁주식담보신용대출
				case 82:	// 예탁채권담보신용대출
				//case 50:    // 대용	
					tmpS.Format("%.f", symt/* + tmpSymt*/);
					break;
				case 5:	// 유통대주
				case 7:	// 자기대주
					tmpS.Format("%.f", symt - tmpSymt);
					break;
				}
			}
		
			m_symt = tmpS;
		}
	}
	if (m_symt.IsEmpty())	FillString(m_symt, LEN_SYMT);

	//symbolMap.Lookup(RT_STATE, stateS);
	//symbolMap.Lookup(RT_GUBN, gubnS);
	if (gubnS.Find("매도") != -1)
		gubnS = "-" + gubnS;
	else
		gubnS = "+" + gubnS;
	symbolMap.SetAt(RT_GUBN, gubnS);

	if (stateS.Find("취소접수") != -1 || stateS.Find("정정접수") != -1 || stateS.Find("정정확인") != -1)
		return CHEG_NOT;
	
	if (bNewCode)	
	{
		if (stateS.Compare("체결"))
			return CHEG_NOT;

		m_bchg = true;
		//==============================================
		// 신규 체결 :
		// 평균단가 = 체결가 
		// 매입가 = 체결가 * 체결수량(잔고) - 신용대출금
		//==============================================
		m_code = codeS;
		m_type = jggbS;
		symbolMap.Lookup(RT_JNAME, m_name);
		//m_name = " "+m_name;  //test
		symbolMap.Lookup(RT_CAMT, m_remain);
		symbolMap.Lookup(RT_CPRICE, tmpS);
		symbolMap.Lookup(RT_MOD, strMod);
		nMod = atoi((LPCTSTR)strMod);

		if (nMod == 0)
			nMod = 1;

		cpriceS.Format("%.f", atof(tmpS) / nMod);

		m_pmaip = cpriceS;
		//CString tmp; tmp.Format("체결: %f=%f*%f-%f\n",atof(cpriceS) * atof(m_remain)- atof(m_symt), atof(cpriceS) , atof(m_remain), atof(m_symt));
		
		m_maip.Format("%.f", atof(cpriceS) * atof(m_remain));
		
		if (bCredit)	m_maip.Format("%.f", atof(m_maip) - atof(m_symt));
		/*
		if ((atof(m_symt) != 0)&&(m_code == "A000660")&&(m_date == "20091125"))
		{
			CString tmp; tmp.Format("%s | %s\n",m_maip, m_symt);
			TRACE(tmp);
		}
		*/
		m_possible = m_remain;
		return CHEG_INSERT;
	}
	else if (!stateS.Compare("체결"))	// 체결
	{
		m_bchg = true;
		symbolMap.Lookup(RT_CAMT, nContAmt);
		symbolMap.Lookup(RT_CPRICE, tmpS);
		symbolMap.Lookup(RT_MOD, strMod);
		nMod = atoi((LPCTSTR)strMod);

		if (nMod == 0)
			nMod = 1;

		cpriceS.Format("%.f", atof(tmpS) / nMod);

CString slog;

		if (gubnS.Find("매수") != -1)  //체결 매수
		{
			CString maipS;
			if(credit == 5)  //유통대주 매수상환 체결
			{
				m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
				m_remain.Format("%d", atoi(m_remain) - atoi(nContAmt));			// 잔고수량
			}
			else
			{
				maipS.Format("%.f", atoi(m_remain) * atof(m_pmaip) + atof(cpriceS) * atof(nContAmt));	// 매입가
				m_remain.Format("%d", atoi(nContAmt) + atoi(m_remain));			// 잔고수량
				m_pmaip.Format("%f", atof(maipS) / atof(m_remain));			// 평균가
				m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));		// 가능수량
				m_maip = maipS;
			}
			
			if(credit == 5)  //유통대주
			{
				if (!atoi(m_remain))
				{
					return CHEG_DELETE;
				} 
				else
				{
					m_maip.Format("%.f", atof(m_pmaip) * atof(m_remain));
					//if (bCredit)
					//	m_maip.Format("%.f", atof(m_maip) - atof(m_symt));
				}

slog.Format("[baltest]매수 [m_maip]=[%s] 평균단가[m_pmaip]=[%s] 잔고[m_remain]=[%s] 가능수량[m_possible]=[%s] 체결수량[nContAmt]=[%s] cpriceS=[%s]", 
			m_maip, m_pmaip, m_remain, m_possible, nContAmt, cpriceS);
Output_DebugString(slog);
			}
		}
		else  //체결 매도
		{
			CString maipS;
			if(credit == 5)  //유통대주 매도체결 잔고 증가
			{
				maipS.Format("%.f", atoi(m_remain) * atof(m_pmaip) + atof(cpriceS) * atof(nContAmt));	// 매입가
				m_remain.Format("%.d", atoi(m_remain) + atoi(nContAmt));
				m_pmaip.Format("%f", atof(maipS) / atof(m_remain));			// 평균가
				m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));		// 가능수량
				m_maip = maipS;

slog.Format("[baltest]매도 [m_maip]=[%s] 평균단가[m_pmaip]=[%s] 잔고[m_remain]=[%s] 가능수량[m_possible]=[%s] 체결수량[nContAmt]=[%s] cpriceS=[%s]", 
			m_maip, m_pmaip, m_remain, m_possible, nContAmt, cpriceS);
Output_DebugString(slog);

			}
			else 	// 매도시 수량 감소
			{
				m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
				m_remain.Format("%.d", atoi(m_remain) - atoi(nContAmt));

				if (!atoi(m_remain))
				{
					return CHEG_DELETE;
				} 
				else
				{
					m_maip.Format("%.f", atof(m_pmaip) * atof(m_remain));
				}
			}	
		}
		CalPgsonik("", "");
	}
	else if (gubnS.Find("매수") != -1 && stateS.Find("접수") != -1 && stateS != "접수오류" && credit == 5)  //유통대주 매수상황 접수
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) - atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) + atoi(nContAmt));
	}
	else if (gubnS.Find("매수") != -1 && stateS.Find("취소확인") != -1 && credit == 5)  //유통대주 매수 취소
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
	}
	else if (gubnS.Find("매도") != -1 && stateS.Find("취소확인") != -1) //매도취소
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) + atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) - atoi(nContAmt));
	}
	else if (gubnS.Find("매도") != -1 && stateS.Find("접수") != -1 && stateS != "접수오류")  //매도접수
	{
		symbolMap.Lookup(RT_CAMT, nContAmt);
		m_possible.Format("%d", atoi(m_possible) - atoi(nContAmt));
		m_amt.Format("%d", atoi(m_amt) + atoi(nContAmt));
	}
//	TRACE("CR[%x][%x] 종목(%s) 수량(%d) 단가(%f) 매입금액(%.f) 원매입금액(%.f) 평가금액(%f) 체결여부(%d) 수익률(%f)\n",
//		this, GetCurrentThreadId(), m_code, atoi(m_remain), atof(m_pmaip), atof(m_maip), atof(m_omaip), atof(m_rprice), m_bchg, atof(m_psuik));
	return CHEG_UPDATE;
}

bool CRemain::CalPgsonik(CString currS, CString rateS, int noticeMode)
{
	if (getFormatData(currS) == getFormatData(m_curr) || m_code.IsEmpty())
		return false;

	// 채권, ELS는 계산안함
	switch (m_code.GetAt(0))
	{
	case 'K':	case 'B':	case 'E':
		return false;
		break;
	}

	if (!currS.IsEmpty())
		m_curr = currS;
	
	if (!rateS.IsEmpty())
		m_srate = rateS;

	m_code.TrimRight();
	// 대용잔고는 계산 안함
//	if (m_creditS == "50")
//		return false;

//	Comment : jjhur 2008.08.07  대용잔고여부 확인 이전으로 옮김
//	if (!currS.IsEmpty())
//		m_curr = currS;

	if ( m_dFee > 0 )
	{
		//===================================================================
		// 평가금액 = 현재가 * 잔고, 대투는 신용대출값 빼줌
		// 평가손익 = 평가금액 - 매입가
		// 수익률 = 평가손익 * 100 / 매입가
		//===================================================================
		// 평가금액
		double rprice = 0;
		
		rprice = atof(getFormatData(m_curr)) * atof(m_remain);

		CString strTemp;

		if(m_b3RDCode)// 2015.04.22 KSJ K-OTC일 경우 수수료가 0.15임. 제세금은 0.0005임
		{
			m_dTax = TAX_RATE_K_OTC;
			
			if(m_iMcgb == 0) //2015.04.22 KSJ 매체구분이 0:HTS/MTS/홈페이지, 1:영업점/콜센터, 2:ARS
			{
				m_dMass = 0.0015;
				m_dSave = 0;
			}
		}
		else if(m_bKONEXCode)
		{
			m_dTax = TAX_RATE_KONEX;
		}
		else if (m_bSJSuikCode)
		{
			m_dTax = TAX_RATE_SJSUIK;
		}
		else if (m_iCodetype == BDCpstType || m_iCodetype == BDCicyType)
		{
			m_dTax = 0;
		}
		else
		{
			m_dTax = TAX_RATE;
		}

		const double dmass = RoundDown(rprice * m_dMass);
		const double dtax = Round(rprice * m_dTax);

		//수수료및 제세금 적용
		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )|| m_betf || m_code.GetAt(0) == 'Q' || m_iCodetype == BDCpstType || m_iCodetype == BDCicyType) //2015.04.22 ETN추가
		{
			if(m_creditS == "05")  //유통대주
				rprice = rprice + (dmass + m_dSave);
			else
				rprice = rprice - (dmass + m_dSave);

			m_biyo.Format("%d", (int)(dmass + m_dSave));
		}	
		else
		{
			if(m_creditS == "05")  //유통대주
				rprice = rprice + (dmass + dtax + m_dSave);
			else
				rprice = rprice - (dmass + dtax + m_dSave);

			m_biyo.Format("%d", (int)(dmass + dtax + m_dSave));
		}	
		
		m_rprice.Format("%.f", rprice); //평가금액
	
		double dmaip{};
		if (m_bchg)	
		{
			if(m_creditS == "05")  //유통대주
				dmaip = (atof(m_pmaip)* atof(m_remain));			
			else
				dmaip = (atof(m_pmaip)* atof(m_remain)) - atof(m_symt);				
		}
		else
			dmaip = atof(m_omaip);// + atof(m_symt);

		if ( atof(m_maip) == 0 ) 
			dmaip = 0;//==>대용은 매입가 0
		else
		{
			//dmaip = dmaip + RoundDown((dmaip+atof(m_symt)) * m_dMass) + m_dSave;//매입금에 수수료 적용
			// 매도수수료만 적용하게 수정
			if(m_dFee == 1)
			{
				if(m_creditS == "05")  //유통대주
					dmaip = dmaip - RoundDown(dmaip * m_dMass) - m_dSave;//매입금에 수수료 적용 
				else
					dmaip = dmaip + RoundDown((dmaip+atof(m_symt)) * m_dMass) + m_dSave;//매입금에 수수료 적용  //test
			}
			else
				dmaip = dmaip;
		}
		
		m_maip.Format("%.f",dmaip);
	
		/////////////////////////////////////////////////
		double dbep = 0;

		if(m_b3RDCode)// 2015.04.22 KSJ K-OTC일 경우 수수료가 0.15임. 제세금은 0.0005임
		{
			m_dTax = TAX_RATE_K_OTC;
			
			if(m_iMcgb == 0) //2015.04.22 KSJ 매체구분이 0:HTS/MTS/홈페이지, 1:영업점/콜센터, 2:ARS
			{
				m_dMass = 0.0015;
				m_dSave = 0;
			}
		}
		else if(m_bKONEXCode)
		{
			m_dTax = TAX_RATE_KONEX;
		}
		else if (m_bSJSuikCode)
		{
			m_dTax = TAX_RATE_SJSUIK;
		}
		else if (m_iCodetype == BDCpstType || m_iCodetype == BDCicyType)
		{
			m_dTax = 0;
		}
		else
		{
			m_dTax = TAX_RATE;
		}

		const double bep_mass = RoundDown2(fabs(atof(m_pmaip)) * m_dMass);
		const double cur_mass = RoundDown2(fabs(atof(m_curr)) * m_dMass);  //유통대주

		const double cur_tax = RoundDown2(fabs(atof(m_creditS == "05"?m_pmaip:m_curr)) * m_dTax);

		if (( m_code.GetAt(0) == 'J' && strlen(m_code) == 7 )|| m_betf || m_code.GetAt(0) == 'Q' || m_iCodetype == BDCpstType || m_iCodetype == BDCicyType) //2015.04.22 ETN추가
		{
			if(m_creditS == "05")  //유통대주 // (매도금액 - 수수료)/(수량 * (1 + 수수료율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 + m_dMass ));
			else					// (매수금액 + 매수수수료)/(수량*(1-수수료율-제세금율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass));
		}
		else 
		{
			if(m_creditS == "05")  //유통대주  //유통대주 // (매도금액 - 수수료 - 제세금)/(수량 * (1 + 수수료율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) - RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass) - Round(fabs(atof(m_curr))*atoi(m_remain) * m_dTax))/(atoi(m_remain) * (1 + m_dMass ));
			else                  // (매수금액 + 매수수수료)/(수량*(1-수수료율-제세금율))
				dbep = ((atof(m_pmaip)*atoi(m_remain)) + RoundDown(fabs(atof(m_pmaip))*atoi(m_remain) * m_dMass))/(atoi(m_remain) * (1 - m_dMass - m_dTax));
		}
		
		m_pbep.Format("%.f",dbep);
		///////////////////////////////////////////////////
		double dSonik{};
		
		

		//pyt 11.09
		if (m_dCalcType==1)  //0 융자금차감, 1 융자금포함
		{
			m_rprice.Format("%.f", rprice); //평가금액
			if(m_creditS == "05")  //유통대주
				m_maip.Format("%.f",dmaip);
			else
				m_maip.Format("%.f",dmaip+atof(m_symt));

			dSonik = atof(m_rprice) - atof(m_maip);

			if(m_creditS == "05")  //유통대주
				dSonik *= -1;

			m_pgsonik.Format("%.0f", dSonik);  //소수이하 반올림

			//평가손익
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;

			// 수익률
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}

			dSonik = ((dSonik)*100)/(atof(m_maip));  //수익률 계산 융자포함  평가손익금(m_pgsonik)*100/매입금액(m_maip)
		}
		else
		{
			if(m_creditS == "05")  //유통대주
				m_rprice.Format("%.f", rprice);
			else
				m_rprice.Format("%.f", rprice-atof(m_symt));

			m_maip.Format("%.f",dmaip);
			dSonik = atof(m_rprice) - atof(m_maip) ;

			if(m_creditS == "05")  //유통대주
				dSonik *= -1;

			m_pgsonik.Format("%.0f", dSonik);
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;

			// 수익률
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}

			dSonik = ((dSonik)*100)/(atof(m_maip));  //수익률 계산 융자미포함   평가손익금(m_pgsonik)*100/매입금액(m_maip)
		}
		
		m_psuik.Format("%.2f", dSonik);
		
		if (dSonik < 0)
			m_psuik = "-" + m_psuik;
		else if (dSonik > 0)
			m_psuik = "+" + m_psuik;


		m_slog.Format("[jango][%s]<%d> 수수료적용 dfee=[%d], 코드=[%s][%s]  현재가=[%s]  m_psuik[수익률]=[%s]  m_pgsonik[평가손익]=[%s]",
			__FUNCTION__, __LINE__, m_dFee, m_code, m_name, m_curr, m_psuik, m_pgsonik);
		Output_DebugString(m_slog);
	} 
	else    //dfee 0 일때  수수료 미적용
	{	
		//===================================================================
		// 평가금액 = 현재가 * 잔고, 대투는 신용대출값 빼줌
		// 평가손익 = 평가금액 - 매입가
		// 수익률 = 평가손익 * 100 / 매입가
		//===================================================================
		double rprice = 0;

		rprice = atof(getFormatData(m_curr)) * atof(m_remain);


		m_rprice.Format("%.f", rprice); //평가금액	

		double dmaip{};

		if (m_bchg)	
		{
			if(m_creditS == "05")  //유통대주
				dmaip = (atof(m_pmaip)* atof(m_remain));			
			else
				dmaip = (atof(m_pmaip)* atof(m_remain)) - atof(m_symt);			
		}
		else
			dmaip = atof(m_omaip);// + atof(m_symt);

		if ( atof(m_maip) == 0 ) 
			dmaip = 0;//==>대용은 매입가 0
		

		/////////////////////////////////////////////////
		m_pbep.Format("%.f",atof(m_pmaip));
		/////////////////////////////////////////////////
		
		//pyt 11.09
		double dSonik{};
		if (m_dCalcType==1)
		{
			m_rprice.Format("%.f", rprice); //평가금액	

			if(m_creditS == "05")  //유통대주   
				m_maip.Format("%.f",dmaip);
			else
				m_maip.Format("%.f",dmaip+atof(m_symt));

			dSonik = atof(m_rprice) - atof(m_maip);	
			if(m_creditS == "05") //유통대주   
				dSonik *= -1;
			m_pgsonik.Format("%.0f", dSonik);
			
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			
			// 수익률
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}

			dSonik = (dSonik*100)/(atof(m_maip));
		}
		else
		{
			if(m_creditS == "05")  //유통대주   
				m_rprice.Format("%.f", atof(m_rprice));
			else
				m_rprice.Format("%.f", rprice-atof(m_symt)); //평가금액	

			m_maip.Format("%.f",dmaip);

			dSonik = atof(m_rprice) - atof(m_maip);

			if(m_creditS == "05") //유통대주   
				dSonik *= -1;

			m_pgsonik.Format("%.0f", dSonik);
			
			if (dSonik > 0)	
				m_pgsonik = "+" + m_pgsonik;
			
			// 수익률
			if (atof(m_maip) == 0 )
			{
				m_psuik = "0.00";
				return true;
			}
			dSonik = (dSonik*100)/(atof(m_maip));
		}

		m_psuik.Format("%.2f", dSonik);

		if (dSonik < 0)
			m_psuik = "-" + m_psuik;
		else if (dSonik > 0)
			m_psuik = "+" + m_psuik;

		m_slog.Format("[jango][%s]<%d> 수수료미적용 dfee=[%d], 코드=[%s][%s]  현재가=[%s]  m_psuik[수익률]=[%s]  m_pgsonik[평가손익]=[%s]",
			__FUNCTION__, __LINE__, m_dFee, m_code, m_name, m_curr,m_psuik, m_pgsonik);
		Output_DebugString(m_slog);
	}

	return true;
}

inline CString CRemain::parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CRemain::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

CString CRemain::getCredit(CString creditS)
{
	if (creditS.IsEmpty())
		return " ";
	const int credit = atoi(creditS);
	CString tmpS = " ";
	if (!m_mapCredit.Lookup(credit, tmpS))
		tmpS = " ";
	return tmpS;
}

CString CRemain::getCreditX(CString creditS)
{
	if (creditS.IsEmpty())
		return " ";
	const int credit = atoi(creditS);
	CString tmpS = " ";
	if (!m_mapCreditX.Lookup(credit, tmpS))
		tmpS = " ";
	return tmpS;
}


void CRemain::FillString(CString& datS, int len)
{
	CString tmpS;
	tmpS.Format("%*s", len, datS);
	datS = tmpS;
}

double CRemain::RoundDown(double data) //매체수수료(절삭) 1의자리 절삭
{//소숫점 첫째자리까지 절삭
    return ( (int)((data * 0.1)) / 0.1 );
}

double CRemain::Round(double data ) //0.003제세금(반올림)
{
    return (int)(data + 0.5);
}

double CRemain::RoundDown2(double data)  //정수변환
{
	return ( (int) Round(data) );
}


void CRemain::testSaveFile(CString strText)
{	
#ifndef TESTSAVEFILE
	return;
#endif
		
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\%s", m_pParent->GetRoot(), SAVEFILENAME);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s]", strCurTime);
	file.Write(titletime, titletime.GetLength());
	file.SeekToEnd(); 	
	
	file.Write(strText, strText.GetLength());
	file.WriteString("\n");
	
	file.Close();	
}
