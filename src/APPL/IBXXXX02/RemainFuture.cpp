#include "stdafx.h"
#include "IBXXXX01.h"
#include "RemainFuture.h"
#include "DefineAll.h"
#include <math.h>
#include "MapWnd.h"
#include "../../h/jmcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRemainFuture::CRemainFuture()
{
	m_pParent = NULL;
	m_dBase = 0;
	m_code = m_name = m_gubn = m_possible = m_diff = m_rdiff = "";
	m_rprice = m_curr = m_pprice = m_pgsonik = m_suik = m_remian = m_maip = "";
	m_NoContMap.RemoveAll();

}

CRemainFuture::CRemainFuture(CWnd* pMainWnd)
{
	m_pParent = (CMapWnd*)pMainWnd;
	m_dBase = 0;
	m_code = m_name = m_gubn = m_possible = m_diff = m_rdiff = "";
	m_rprice = m_curr = m_pprice = m_pgsonik = m_suik = m_remian = m_maip = "";
	m_NoContMap.RemoveAll();


}


CRemainFuture::~CRemainFuture()
{
}

BEGIN_MESSAGE_MAP(CRemainFuture, CWnd)
	//{{AFX_MSG_MAP(CRemainFuture)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRemainFuture::OnDestroy() 
{
	CWnd::OnDestroy();
}

CString CRemainFuture::TotalData()
{
	//           00(종목코드), 01(종목명), 02(구분),        03(잔고수량),      
	//		04(매도가능),      05(평균가),  	06(현재가),   07(평균가대비),
	//		08(평가금액),     09(평가손익),   10(수익률),     11(매입금액)	12(전일대비)
	CString t_rprice = m_rprice;
	CString t_curr = m_curr;
	if (m_code.GetLength()==8)
	{
		int mkgb = atoi(m_code.Mid(1,2));
		if (mkgb>=85 && mkgb<99)	// 상품선물
		{
			t_rprice.Format("%.f", atof(t_rprice));
			t_curr.Format("%.f", atof(t_curr));
		}
		else if (mkgb>=10 && mkgb<=59)	// 주식선물
		{
			t_rprice.Format("%+.f", atof(t_rprice));
			t_curr.Format("%+.f", atof(t_curr));
		}
		
	}

	CString data = m_code + "\t" + m_name + "\t" + m_gubn + "\t" + m_remian + "\t" + 
			m_possible + "\t" + t_rprice + "\t" + t_curr + "\t" + m_rdiff + "\t" + 
			m_pprice + "\t" + m_pgsonik + "\t" + m_suik + "\t" + m_maip + "\t" + m_diff + "\t" + m_srate + "\t";
	return data;
}

void CRemainFuture::ParsingRemainData(char *pData)
{
	struct f_rec frec;
	CopyMemory(&frec, pData, L_frec);
		
	m_code	= getString(frec.cod2, sizeof(frec.cod2));
	m_name	= getString(frec.hnam, sizeof(frec.hnam));
	m_gubn	= getString(frec.dsgb, sizeof(frec.dsgb));
	m_remian = getString(frec.bqty, sizeof(frec.bqty));
	m_possible = getString(frec.cqty, sizeof(frec.cqty));
	m_rprice = getString(frec.pprc, sizeof(frec.pprc));
	m_curr	= getString(frec.curr, sizeof(frec.curr));
	m_rdiff	= getString(frec.diff, sizeof(frec.diff));
	m_pprice = getString(frec.pamt, sizeof(frec.pamt));
	m_pgsonik = getString(frec.psam, sizeof(frec.psam));
	m_suik	= getString(frec.prat, sizeof(frec.prat));
	if (m_suik.GetLength() > 0 && atof(m_suik) > 0.0 && m_suik.GetAt(0) != '+')
		m_suik = "+" + m_suik;
	m_maip	= getString(frec.tamt, sizeof(frec.tamt));

	CString sKey = "2";
	if (m_gubn.Find("매도") == -1)
		sKey = "1";
	
	int iRemain, iPossible;
	iRemain = atoi(m_remian);
	iPossible = atoi(m_possible);

	m_NoContMap.SetAt(sKey, (void*)(iRemain-iPossible));


}

void CRemainFuture::ParsingAllaccnData(CString sData)
{
	m_code	= parser(sData, "\t");
	m_name	= parser(sData, "\t");
	m_gubn	= parser(sData, "\t");
	m_remian = parser(sData, "\t");
	m_possible = parser(sData, "\t");
	m_rprice = parser(sData, "\t");	
	m_curr	= parser(sData, "\t");
	m_rdiff	= parser(sData, "\t");
	m_pprice	= parser(sData, "\t");
	m_pgsonik = parser(sData, "\t");
	m_suik	= parser(sData, "\t");
	m_maip	= parser(sData, "\t");	
}

int CRemainFuture::CalRemainData(CMapStringToString &symbolMap, CString keyS, bool bNewCode)
{
	CString stateS, contAmtS, cpriceS, gubnS;
	CString tmpS1, tmpS2, test;	
	
	CString codeS = keyS.Left(LEN_FCODE);

	m_code = codeS;
	int	nKind= getJKind(m_code);

	symbolMap.Lookup(RT_STATE, stateS);
	symbolMap.Lookup(RT_GUBN, tmpS1);
	symbolMap.Lookup(RT_CAMT, tmpS2);
	if (tmpS1.Find("매도") != -1)
		tmpS1 = "-매도";
	else
		tmpS1 = "+매수";
	symbolMap.SetAt(RT_GUBN, tmpS1);

	parsingNcont(tmpS1, stateS, atoi(tmpS2));
	if (!m_dBase)	
		getBase();
	
	if (bNewCode)
	{
		if (stateS.Compare("체결"))
			return CHEG_NOT;

		symbolMap.Lookup(RT_JNAME, m_name);
		symbolMap.Lookup(RT_GUBN, m_gubn);
		// 20071010 신규 체결시 주문가 오류 수정
		// symbolMap.Lookup(RT_JPRICE, tmpS1);
		if (nKind == JK_SFUTURE || nKind == JK_SFSPREAD)		
		{
			//m_rprice.Format("%d", atof(tmpS1));
			symbolMap.Lookup(RT_CAMT, contAmtS);
			symbolMap.Lookup(RT_CPRICE, tmpS1);
			cpriceS.Format("%.f", atof(tmpS1)/100);
			m_rprice.Format("%.f", atof(tmpS1)/100);
		}
		else
		{
			//m_rprice.Format("%.2f", atof(tmpS1) / 100);
			symbolMap.Lookup(RT_CAMT, contAmtS);
			symbolMap.Lookup(RT_CPRICE, tmpS1);
			cpriceS.Format("%.2f", atof(tmpS1) / 100);
			m_rprice.Format("%.2f", atof(tmpS1) / 100);
			
		}

			
		m_maip.Format("%.f", atof(cpriceS) * atof(contAmtS) * m_dBase);
		m_remian = contAmtS;
		m_possible.Format("%d", atoi(contAmtS) - getNoCont(m_gubn=="-매도"?"+매수":"-매도"));

TRACE("1. calRemainData [%s] - [%d] = [%s], [%s]\n", contAmtS, getNoCont(m_gubn=="-매도"?"+매수":"-매도") , m_possible, m_gubn);
		return CHEG_INSERT;
	}

	else if (!stateS.Compare("체결"))
	{
		
		symbolMap.Lookup(RT_CAMT, contAmtS);
		symbolMap.Lookup(RT_GUBN, gubnS);
		symbolMap.Lookup(RT_CPRICE, tmpS1);
		
		if (nKind == JK_SFUTURE || nKind == JK_SFSPREAD)
			cpriceS.Format("%.f", atof(tmpS1) / 100);
		else
			cpriceS.Format("%.2f", atof(tmpS1) / 100);
		
		if (m_gubn == gubnS)	
		{

			m_remian.Format("%d", atoi(contAmtS) + atoi(m_remian));
TRACE("2. calRemainData [%s] - [%d] = [%s], [%s]\n", m_remian, getNoCont(gubnS=="-매도"?"+매수":"-매도") , m_possible, gubnS);

			m_possible.Format("%d", atoi(m_remian) - getNoCont(gubnS=="-매도"?"+매수":"-매도"));
			if (atoi(m_possible)<0) m_possible = "0";
			m_maip.Format("%.f", atof(m_maip) + atof(cpriceS) * atof(contAmtS) * m_dBase);
			
		}
		else
		{
			m_remian.Format("%.f", atof(m_remian) - atof(contAmtS));
			
			if (!atoi(m_remian))
				return CHEG_DELETE;
			else if (atoi(m_remian) < 0)
			{
				m_remian.Replace("-", "");
				symbolMap.Lookup(RT_GUBN, m_gubn);
				
				// 주문가능수량 m_possible
TRACE("3. calRemainData [%s] - [%d] = [%s], [%s]\n", m_remian, getNoCont(gubnS=="-매도"?"+매수":"-매도") , m_possible, gubnS);
				m_possible.Format("%d", atoi(m_remian) - getNoCont(gubnS=="-매도"?"+매수":"-매도"));

				if (atoi(m_possible)<0) m_possible = "0";
				m_maip.Format("%.f", atof(cpriceS) * atof(m_remian) * m_dBase);

			}
			else
			{
				m_possible.Format("%d", atoi(m_remian) - getNoCont(gubnS));
TRACE("4. calRemainData [%s] - [%d] = [%s], [%s]\n", m_remian, getNoCont(gubnS) , m_possible, m_gubn);

				if (atoi(m_possible)<0) m_possible = "0";
				m_maip.Format("%.f", atof(m_maip) * atof(m_remian) / (atof(m_remian) + atof(contAmtS)));
			}		
		}
		if (nKind == JK_SFUTURE || nKind == JK_SFSPREAD)
			m_rprice.Format("%.f", atof(m_maip) / atof(m_remian) / m_dBase);
		else
			m_rprice.Format("%.2f", atof(m_maip) / atof(m_remian) / m_dBase);
		//CString tmp; tmp.Format("%s=%f/%f/%f", m_rprice, atof(m_maip) , atof(m_remian) , m_dBase);
		//AfxMessageBox(tmp);

		CalPgsonik();
	}
	else
	{
		CString sTemp = (m_gubn=="-매도"?"+매수":"-매도");
		m_possible.Format("%d", atoi(m_remian) - getNoCont(m_gubn=="-매도"?"+매수":"-매도"));	
TRACE("4. calRemainData [%s] - [%d] = [%s], [%s]\n", m_remian, getNoCont(m_gubn=="-매도"?"+매수":"-매도") , m_possible, m_gubn);

		if (atoi(m_possible)<0) m_possible = "0";
	}
	return CHEG_UPDATE;
}

bool CRemainFuture::CalPgsonik(CString sNCurr, CString sNDiff, CString sNRate)
{
	if (sNCurr == m_curr && sNDiff == m_diff)
		return false;
	else if (getFormatData(sNCurr) == getFormatData(m_curr))
	{
		m_diff = sNDiff;
		return true;
	}
	m_curr = sNCurr;
	if (!sNDiff.IsEmpty())
		m_diff = sNDiff;

	if (!sNRate.IsEmpty())
		m_srate = sNRate;

	CalPgsonik();
	return true;
}

void CRemainFuture::CalPgsonik()
{
	if (!m_dBase)	getBase();

	double dSonik = 0, dGubn = 1, dTemp = 0.;
	bool	bMd = false;
	int kind = getJKind(m_code);
	if (m_gubn.Find("매도") != -1)
		bMd = true;
	if (bMd)	dGubn = -1;

	m_pprice.Format("%.f", atof(m_remian) * atof(getFormatData(m_curr)) * m_dBase);

	dSonik = atof(getFormatData(m_pprice)) - atof(getFormatData(m_maip));
	dSonik *= dGubn;

	if (dSonik > 0)
		m_pgsonik.Format("+%.0f", dSonik);
	else
		m_pgsonik.Format("%.0f", dSonik);

	// 수익률
	dSonik *= 100;
	dSonik /= atof(getFormatData(m_maip));//(atof(m_rprice) * m_dBase * atof(m_remian));
	m_suik.Format("%.2f", dSonik);
	if (dSonik < 0)
		m_suik = "-" + m_suik;
	else if (dSonik > 0)
		m_suik = "+" + m_suik;

	// 평균가 대비
	dTemp = atof(getFormatData(m_curr)) - atof(getFormatData(m_rprice));
	if (getJKind(m_code) == JK_SFUTURE || getJKind(m_code) == JK_SFSPREAD)
		m_rdiff.Format("%.f", fabs(dTemp));
	else
		m_rdiff.Format("%.2f", fabs(dTemp));
	dTemp *= dGubn;
	m_rdiff.Insert(0, CString(' ', 10 - m_rdiff.GetLength()));
	if (dTemp > 0.)
		m_rdiff.SetAt(0, '+');
	else if (dTemp < 0.)
		m_rdiff.SetAt(0, '-');
}

void CRemainFuture::SetNoCont(CString sMemeGubn, int nAmount)
{
	CString sKey = "2";

	if (!nAmount)
		return;
	if (sMemeGubn.Find("매도") != -1)
		sKey = "1";
	
//	sKey += m_code;	//코드를 더해서 키로 가지고 있는다.

	int nSrcNoCont = 0;
	m_NoContMap.Lookup(sKey, (void*&)nSrcNoCont);
	m_NoContMap.SetAt(sKey, (void*)(nSrcNoCont + nAmount));
}

int CRemainFuture::getNoCont(CString sMemeGubn)
{
	CString sKey = "2";

	if (sMemeGubn.Find("매도") != -1)
		sKey = "1";
	
//	sKey += m_code;	//코드를 더해서 키로 가지고 있는다.

	int nSrcNoCont = 0;
	m_NoContMap.Lookup(sKey, (void*&)nSrcNoCont);

	if (nSrcNoCont<0) nSrcNoCont = 0;

	TRACE(" getNoCont parameter : %s sKey : %s nSrcNoCont : %d\n", sMemeGubn,sKey,nSrcNoCont);

	return nSrcNoCont;
}

void CRemainFuture::parsingNcont(CString sMemeGubn, CString sStatus, int nAmount)
{
	CString sKey = "2";
	if (sMemeGubn.Find("매도") != -1)
		sKey = "1";
	
	int prevN;

	int nSrcNoCont = 0;

//	sKey += m_code;	//코드를 더해서 키로 가지고 있는다.
	
	m_NoContMap.Lookup(sKey, (void*&)nSrcNoCont);
	
	prevN = nSrcNoCont;
	if (!sStatus.Compare("체결"))
		nSrcNoCont -= nAmount;
	else if (!sStatus.Compare("정정확인"))
		nSrcNoCont -= nAmount;
	else if (!sStatus.Compare("취소확인"))
		nSrcNoCont -= nAmount * 2;
	else if (!sStatus.Compare("거부"))
		nSrcNoCont -= nAmount;
	if (sStatus.Find("접수", 0) != -1)	// 20071010	 접수시 수량 오류
		nSrcNoCont += nAmount;
	m_NoContMap.SetAt(sKey, (void*)nSrcNoCont);
}


double	CRemainFuture::GetBaseValue(CString code)
{
	double result = 0;
	int readL = 0; 
	CString	filePath, root, user, tempcode, gConv1, gConv2, gdan;
	struct	sfcode sfcode;
	CFile	file;
	
	root = m_pParent->GetRoot();

	filePath.Format("%s/%s/%s", root, "tab", "sfcode.dat");

	if (!file.Open(filePath, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox("선물 파일을 읽을 수 없습니다.");
		return 0;
	}
	
	int countC = (int)file.GetLength() / sizeof(struct sfcode);
	for (int ii = 0; ii < countC; ii++)
	{
		file.Read(&sfcode, sizeof(struct sfcode));
		
		tempcode = CString(sfcode.codx, sizeof(sfcode.codx));
		
		tempcode.TrimRight();
		tempcode.TrimLeft();
		code.TrimRight();
		code.TrimLeft();

		if(code.Compare(tempcode) == 0)
		{
			gdan = CString(sfcode.gdan, sizeof(sfcode.gdan));

			if(strlen(gdan) > 17)
			{
 				gConv1 = gdan.Mid(0,10);
 				gConv2 = gdan.Mid(10,8);
 				
 				result = atof(gConv1) + (atof(gConv2)/100000000);			
			}
			else
			{
				result = 10;
			}
			break;
		}
	}
	
	file.Close();

	return result;
}

void CRemainFuture::getBase()
{
	if (m_code.IsEmpty())
		return;

	CString scode;
	int	nkind = getJKind(m_code);

	m_dBase = 100000;
	
	switch (nkind)
	{
	case JK_CALLOPT:
	case JK_PUTOPT:
		scode = m_code.Mid(1, 4);
		if (scode<="01G6")	m_dBase = 100000;
		else				m_dBase = 500000;
		break;
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_SFUTURE:
	case JK_SFSPREAD:
		m_dBase = GetBaseValue(m_code);
//		m_dBase = 10;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	case JK_KTBFUTURE:
		m_dBase = 1000000;
		break;
	case JK_USDFUTURE:
		m_dBase = 10000;
		break;
	case JK_MINIGOLDFUTURE:
		m_dBase = 100;
		break;
	case JK_GOLDFUTURE:
		m_dBase = 1000;
		break;
	case JK_PIGFUTURE:
		m_dBase = 1000;
		break;
	}
}

CString CRemainFuture::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

int CRemainFuture::getJKind(CString m_code)
{
	if (m_code.GetLength() != 8)
		return -1;
	TCHAR	ch1 = m_code.GetAt(0);
	TCHAR	ch2 = m_code.GetAt(1);
	TCHAR	ch3 = m_code.GetAt(2);

	switch (ch1)
	{
	case '1':	// future
		{
			if (ch2 == '0')
			{
				if (ch3 == '1')
					return JK_FUTURE;
				else
					return JK_STAR;
			}
			else if (ch2 == '6') 
				return JK_KTBFUTURE;
			else if (ch2 == '7')
				return JK_USDFUTURE;
			else if (ch2 == '8' && ch3 == '5')
				return JK_GOLDFUTURE;
			else if (ch2 == '8' && ch3 == '6')
				return JK_PIGFUTURE;
			else if (ch2 == '8' && ch3 == '7')
				return JK_MINIGOLDFUTURE;
			else
				return JK_SFUTURE;			
		}
	case '4':	// future spread
		if (ch2 == '0')
			return JK_SPREAD;
		else
			return JK_SFSPREAD;
	case '2':	// call option
		if (m_code.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
		if (m_code.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

CString CRemainFuture::parser(CString &srcstr, CString substr)
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

CString CRemainFuture::getString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, nMinLen);
	sRtn.TrimRight();	sRtn.TrimLeft();
	return sRtn;
}





