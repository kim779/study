#include "stdafx.h"
#include "Cx_NotifyEx.h"
#include "info.h"
#include "OpSensCal.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CInfo::CInfo(CWnd *pControl)
{
	m_pControl = pControl;
	sCode = sJmm = sGubn = sRemainAmt = sPossibleAmt = sRPrice = sCurr = sRDiff = sPPrice = "";
	sCurr = sRDiff = sPPrice = sPgSonik = sSuik = sMaip = sDiff = sMchg = "";
	sDelta = sGamma = "";
	sJjTrade = sJjCalendar = sBdjs = "";
	m_noCont[0] = m_noCont[1] = 0;
	m_dBase = 0;
}

CInfo::~CInfo()
{

}

void CInfo::RemainParsing(struct f_rec *frec)
{
	sCode	= _GetString(frec->cod2, sizeof(frec->cod2));
	sJmm	= _GetString(frec->hnam, sizeof(frec->hnam));
	sGubn	= _GetString(frec->dsgb, sizeof(frec->dsgb));
	sRemainAmt = _GetString(frec->bqty, sizeof(frec->bqty));
	sPossibleAmt = _GetString(frec->cqty, sizeof(frec->cqty));
	sRPrice = _GetString(frec->pprc, sizeof(frec->pprc));
	sCurr	= _GetString(frec->curr, sizeof(frec->curr));
	sRDiff	= _GetString(frec->diff, sizeof(frec->diff));
	sPPrice	= _GetString(frec->pamt, sizeof(frec->pamt));
	sPgSonik = _GetString(frec->psam, sizeof(frec->psam));
	sSuik	= _GetString(frec->prat, sizeof(frec->prat));
	if (sSuik.GetLength() > 0 && atof(sSuik) > 0.0 && sSuik.GetAt(0) != '+')
		sSuik = "+" + sSuik;
	sMaip	= _GetString(frec->tamt, sizeof(frec->tamt));
}


void CInfo::SetNoContNew(int md, int ms)
{
	m_noCont[0] = md;
	m_noCont[1] = ms;
}

void CInfo::SetNoCont(CString gubnS, CString amtS)
{
	int idx = 0;
	int amt = atoi(amtS);
	if (gubnS.Find("매도") != -1)
		idx = 0;
	else if (gubnS.Find("매수") != -1)
		idx = 1;
	m_noCont[idx] += amt;
}

int CInfo::getNoCont(CString gubnS)
{
	int idx = 0;
	if (gubnS.Find("매도") != -1)
		idx = 0;
	else if (gubnS.Find("매수") != -1)
		idx = 1;
	return m_noCont[idx];
}

void CInfo::setNoCont(CString gubnS, CString stateS, int amt)
{
	int idx = 0;
	if (gubnS.Find("매도") != -1)
		idx = 0;
	else if (gubnS.Find("매수") != -1)
		idx = 1;
	
	int saveAmt = m_noCont[idx];
	
	if (!stateS.Compare("체결"))
		saveAmt -= amt;
	else if (!stateS.Compare("취소확인"))
		saveAmt -= amt;
	else if (!stateS.Compare("거부"))
		saveAmt -= amt;
	if (stateS.Find("접수", 0) != -1 && stateS.Find("정정", 0) == -1 && stateS.Find("취소", 0) == -1)
		saveAmt += amt;

	m_noCont[idx] = saveAmt;
}

CString CInfo::GetTotalData()
{
	// 미체결수량 : 매도+매수
	int nMchg = 0;
	nMchg = getNoCont(sGubn=="-매도"?"+매수":"-매도");
	sMchg.Format("%d", nMchg);

	CString deltaS, gammaS;
	// 델타, 감마 : 매도는 (* -1), * 수량 

	double remainAmt = atof(sRemainAmt);
	double delta = atof(sDelta) * remainAmt;
	double gamma = atof(sGamma) * remainAmt;
	
	if (sGubn.Find("매도", 0) != -1)
	{
		delta *= (-1);
		gamma *= (-1);
	}
	deltaS.Format("%.4f", delta);
	gammaS.Format("%.4f", gamma);
	
	// 주문가능수량
	CString possibleS = sPossibleAmt;
	double posibble = atof(sPossibleAmt);
	if (posibble < 0.0)	possibleS = "0";

	//              00(종목코드), 01(종목명), 02(구분),        03(잔고수량), 0      4(매도가능),      05(평균가),
	//		06(현재가),   07(평균가대비),08(평가금액),     09(평가손익),   10(수익률),     11(매입금액)
	//		12(전일대비)   13(미체결)   14(델타)        15(감마)
	CString data = sCode + "\t" + sJmm + "\t" + sGubn + "\t" + sRemainAmt + "\t" + possibleS + "\t" + sRPrice + "\t" + 
			sCurr + "\t" + sRDiff + "\t" + sPPrice + "\t" + sPgSonik + "\t" + sSuik + "\t" + sMaip + "\t" + 
			sDiff + "\t" + sMchg +"\t" + deltaS + "\t" + gammaS + "\t";
	return data;
}

bool CInfo::CalPgsonik(CString newCurrS, CString newDiffS)
{
	if (newCurrS == sCurr && newDiffS == sDiff)
		return false;
	else if (_GetFormatData(newCurrS) == _GetFormatData(sCurr))
	{
		sDiff = newDiffS;
		return true;
	}
	sCurr = newCurrS;
	sDiff = newDiffS;
	CalPgsonik();
	return true;
}

void CInfo::CalPgsonik()
{
	// sCurr : 현재가, sRemainAmt : 잔고수량, sRPrice : 평균단가
	if (!m_dBase)	getBase();
	double dSonik = 0;			// 손익
	double dGubn = 1;			// *-1이유 : 매도는 손익이 반대
	double dTemp = 0.0;			// 평균가대비
	bool	bMd = false;
	int kind = getJKind(sCode);
	if (sGubn.Find("매도") != -1)
		bMd = true;
	
	if (bMd)	
		dGubn = -1;
		

	// 평가금액 : 잔고수량 * 현재가 * 기준가
	sPPrice.Format("%.f", atof(sRemainAmt) * atof(_GetFormatData(sCurr)) * m_dBase);
	// 평가손익 : 평가금액 - 매입가
	dSonik = atof(_GetFormatData(sPPrice)) - atof(_GetFormatData(sMaip));
	dSonik *= dGubn;
	sPgSonik.Format("%.0f", dSonik);

	// 수익률 : 평가손익 / (평균단가 * 기준가 * 잔고수량) * 100
	dSonik *= 100;
	double div = (atof(sRPrice) * m_dBase * atof(sRemainAmt));
	if (div == 0.0)
		dSonik = 0.0;
	else
		dSonik /= div;

	sSuik.Format("%.2f", dSonik);
	if (dSonik < 0)
		sSuik = "-" + sSuik;		// 수익률 --로 나오는 이유
	else if (dSonik > 0)
		sSuik = "+" + sSuik;

	// 평균가 대비
	dTemp = atof(_GetFormatData(sCurr)) - atof(_GetFormatData(sRPrice));
	dTemp *= dGubn;
	sRDiff.Format("%.2f", fabs(dTemp));
	sRDiff.Insert(0, CString(' ', 10 - sRDiff.GetLength()));
	if (dTemp > 0.0)
		sRDiff.SetAt(0, '+');
	else if (dTemp < 0.0)
		sRDiff.SetAt(0, '-');
}


void CInfo::CalMingam(_mingamBase *mgBase, bool bAll /*= true*/)
{
	if (!m_pOpSensCal)	return;
	
	int kind = getJKind(sCode);
	// 선물, 스프레드
	if (kind == -1 || kind == JK_FUTURE || kind == JK_SPREAD || kind == JK_STAR)
	{
		if (!bAll)			return;
		sDelta = "5";	sGamma = "0";	return;
	}
	
	if (sJjTrade == "" && sJjCalendar == "")	// 영업일, 달력일이 없을 경우
	{
		if (!bAll)			return;
		sDelta = "0";	sGamma = "0";	return;
	}
	// 옵션
	CString currS, strikeS;
	m_pOpSensCal->SetJCode(sCode);				// 종목코드
	double strike = getOptionStrike(sCode);			// 행사가
	strikeS.Format("%.2f", strike);
	m_pOpSensCal->SetStrike(strikeS);

	m_pOpSensCal->SetBzRemainDays(sJjTrade);		// 영업일
	m_pOpSensCal->SetCalRemainDays(sJjCalendar);		// 달력일
	m_pOpSensCal->SetBedangJisu(sBdjs);			// 배당지수
	currS.Format("%.2f", fabs(atof(sCurr)));		// 현재가
	m_pOpSensCal->CalcEx(currS, mgBase->k200cur, mgBase->kcur, mgBase->time);

	sDelta = m_pOpSensCal->GetDelta();
	sGamma = m_pOpSensCal->GetGamma();

//if (sCode == "201C2237")
//	TRACE("strike[%s] 영업일[%s] 달력일[%s] 배당지수[%s] 현재가[%s] k200[%s] 선물[%s] time[%s] delta[%s]\n", strikeS, sJjTrade, sJjCalendar, sBdjs, sCurr, mgBase->k200cur, mgBase->kcur, mgBase->time, sDelta);
}

int CInfo::NoticeData(CMapStringToString& mapNotice, CString codeS, bool bNew)
{

	// return 0: cannot find, 1: update, 2: delete, 3: OOP Send
	sCode = codeS;								// 종목코드
	CString stateS, gubnS, amtS, chegS, chgaS, tmpS;
	mapNotice.Lookup(RT_STATE, stateS);					// 주문상태
	
	mapNotice.Lookup(RT_GUBN, gubnS);					// 주문구분
	if (gubnS.Find("매도") != -1)
		gubnS = "-매도";
	else
		gubnS = "+매수";
	mapNotice.SetAt(RT_GUBN, gubnS);

	mapNotice.Lookup(RT_CAMT, amtS);					// 주문수량
	setNoCont(gubnS, stateS, atoi(amtS));					// 미체결수량
	if (!m_dBase)	getBase();
	
	if (bNew)	// 신규종목
	{
		if (stateS.Compare("체결"))
			return SM_NOT;

		mapNotice.Lookup(RT_JNAME, sJmm);				// 종목명
		mapNotice.Lookup(RT_GUBN, sGubn);				// 주문구분
		//mapNotice.Lookup(RT_JPRICE, tmpS);				// 평균가
		sRPrice.Format("%.2f", atof(tmpS) / 100);
		mapNotice.Lookup(RT_CAMT, chegS);				// 체결수량
		mapNotice.Lookup(RT_CPRICE, tmpS);	
		sRPrice.Format("%.2f", atof(tmpS) / 100);			// 평균가
		chgaS.Format("%.2f", atof(tmpS) / 100);				// 체결가
		sMaip.Format("%.f", atof(chgaS) * atof(chegS) * m_dBase);	// 매입금액
		sRemainAmt = chegS;						// 체결수량
		sPossibleAmt.Format("%d", atoi(chegS) - getNoCont(sGubn=="-매도"?"+매수":"-매도"));
										// 주문가능수량
		return SM_INSERT;
	}
	else if (!stateS.Compare("체결"))	// 체결
	{
		CString sNGubn, sCPrice;
		mapNotice.Lookup(RT_CAMT, chegS);				// 체결수량
		mapNotice.Lookup(RT_GUBN, sNGubn);				// 주문구분
		mapNotice.Lookup(RT_CPRICE, tmpS);				// 체결가
		chgaS.Format("%.2f", atof(tmpS) / 100);
		sCPrice = chgaS;
		
		//=================================================================================
		// * 포지션 같은 경우 
		//	- 잔고수량 = 기존잔고수량 + 체결수량
		//	- 주문가능수량 = 새잔고수량 - 미체결수량
		//	- 매입가 = 기존매입가 + 체결가 * 체결량 * 기준가
		//
		// * 포지션 반대 경우
		//	- 잔고수량 = 기존잔고수량 - 체결수량 (잔고 없음 delete)
		//	- 계산잔고수량이 0보다 작은경우(반대 포지션)
		//		- 잔고수량 * -1
		//		- 주문 구분자 새로운 포지션으로
		//		- 주문가능수량 = 잔고수량 - 미체결수량
		//		- 매입가 : 체결가 * 잔고수량 * 기준가
		//	- 계산잔고수량이 0보다 큰경우(현 포지션에 추가)
		//		- 주문가능수량 = 잔고수량 - 미체결수량
		//		- 매입가 : 매입가 * 잔고수량 / (잔고수량 + 체결수량)
		//=================================================================================
		if (sGubn == sNGubn)					
		{
			sRemainAmt.Format("%d", atoi(chegS) + atoi(sRemainAmt));
			sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sNGubn=="-매도"?"+매수":"-매도"));
			sMaip.Format("%.f", atof(sMaip) + atof(sCPrice) * atof(chegS) * m_dBase);
		}
		else								// 포지션 다른 경우
		{
			sRemainAmt.Format("%.f", atof(sRemainAmt) - atof(chegS));
			if (!atoi(sRemainAmt))
				return SM_DELETE;
			else if (atoi(sRemainAmt) < 0)
			{
				sRemainAmt.Replace("-", "");
				mapNotice.Lookup(RT_GUBN, sGubn);
				sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sNGubn== "-매도" ? "+매수" : "-매도"));
				sMaip.Format("%.f", atof(sCPrice) * atof(sRemainAmt) * m_dBase);
			}
			else
			{
				sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sNGubn));
				double div = atof(sRemainAmt) + atof(chegS);
				if (div == 0.0)
					sMaip = "0";
				else
					sMaip.Format("%.f", atof(sMaip) * atof(sRemainAmt) / (atof(sRemainAmt) + atof(chegS)));
			}
		}
		if (atof(sRemainAmt) == 0.0)
			sRPrice = "0.00";
		else
			sRPrice.Format("%.2f", atof(sMaip) / atof(sRemainAmt) / m_dBase);		// 평균가 = 매입가/보유수량/기준가
		CalPgsonik();
	}
	else
	{
		sPossibleAmt.Format("%d", atoi(sRemainAmt) - getNoCont(sGubn=="-매도"?"+매수":"-매도"));
												// 주문가능수량 = 잔고수량 - 미체결수량
	}
	return SM_UPDATE;
}

int CInfo::getJKind(CString sCode)
{
	if (sCode.GetLength() != 8)
		return -1;
	switch (sCode.GetAt(0))
	{
	case '1':	// future
	case 'A':	 //파생상품 코드개편
		{
			if (sCode.Mid(1,2) == "03")
				return JK_STAR;
			return JK_FUTURE;
		}
	case '4':	// future spread
	case 'D':
		return JK_SPREAD;
	case '2':	// call option
	case 'B':
		if (sCode.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
	case 'C':
		if (sCode.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

double CInfo::getOptionStrike(CString opCode)
{
	double strike;
	strike = atof(opCode.Mid(5, 3));
	if (opCode[7] == '2' || opCode[7] == '7')
		strike = strike + 0.5;
	return strike;
} 

void CInfo::getBase()
{
	if (sCode.IsEmpty())	return;
	m_dBase = 100000;
	
	switch (getJKind(sCode))
	{
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	}
}


