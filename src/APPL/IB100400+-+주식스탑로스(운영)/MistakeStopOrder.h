#pragma once
// [strPrice]
// 지정가 일 경우			: 입력받은 가격
// 시장가 / 최유리 일 경우	: 상한가
//
// [strListedQuantity]
// 주식 / ELW 일 경우		: 상장주식수 (1334 Symbol Data)
// 선물옵션일 경우			: Default
#define ALLOW_ORDER		0
#define ORD_OVER_20		1
#define ORD_OVER_50		2
#include "JumunCheck.h"
/*
struct	_jinfo {
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
#define L_jinfo		sizeof(struct _jinfo)
*/
BOOL CheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""))
{
	ASSERT(pWnd != NULL);
	//ASSERT(!strPrice.IsEmpty());
	//ASSERT(!strQuantity.IsEmpty());
	ASSERT(!strCode.IsEmpty());

	CString strMsg(_T(""));
	CString strMsg2(_T(""));
	
	long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	int smode = ALLOW_ORDER;


	

	UINT nType = MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2;
	//AfxMessageBox(strCode);
	switch (strCode.GetLength())
	{
	case 6:		// 주식, ELW
		{
			lAmount = lPrice * lQty;
			/*
			CString tmp;
			tmp.Format("%f = %f * %f",lAmount, lQty, lPrice);
			AfxMessageBox(tmp);	
			*/	
			if (lAmount > 2000000000 && lAmount <= 5000000000)
				strMsg.Format("%s\n", _T("주문금액이 20억을 초과하는 이상주문입니다."));
			else if (lAmount > 5000000000)
			{
				strMsg.Format("%s\n", _T("주문금액이 50억을 초과하는 이상주문입니다."));
				nType = MB_OK | MB_ICONINFORMATION;
			}
			//else 
			{
				if (strCode.Left(1) == _T("5") || strCode.Left(1) == _T("6") || 
					strCode.Left(1) == _T("7") || strCode.Left(1) == _T("8"))
					return TRUE;
				if (lListedQty!=0)
				{
					lQty = lQty * 100;
					
					if (lQty > lListedQty && lQty / 3 <= lListedQty)
						strMsg2.Format("%s\n", _T("주문수량이 상장주식수 대비 1%를 초과하는 이상주문입니다."));
					else if (lQty / 3 > lListedQty)
					{
						strMsg2.Format("%s\n", _T("주문수량이 상장주식수 대비 3%를 초과하는 이상주문입니다."));
						nType = MB_OK | MB_ICONINFORMATION;
					}
				}
			}
		} 
		break;

	case 8:		// 선물옵션
		{
			if (strCode.Left(1) == _T("1") || strCode.Left(1) == _T("4"))
			{
				if (lQty > 300 && lQty <= 500)
					strMsg.Format("%s\n", _T("주문수량이 300계약을 초과한 이상주문입니다."));
				else if (lQty > 500)
				{
					strMsg.Format("%s\n", _T("주문수량이 500계약을 초과한 이상주문입니다."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else if (strCode.Left(1) == _T("2") || strCode.Left(1) == _T("3"))
			{
				lAmount = lQty * lPrice * 100000;


				if (lAmount > 2000000000 && lAmount <= 5000000000)
					strMsg.Format("%s\n", _T("주문금액이 20억원을 초과하는 이상주문입니다."));
				else if (lAmount > 5000000000)
				{
					strMsg.Format("%s\n", _T("주문금액이 50억원을 초과하는 이상주문입니다."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else
				return TRUE;
		}	
		break;

	default:
		break;
	}
	

	if (strMsg.IsEmpty()&&strMsg2.IsEmpty())
		return TRUE;
	else
	{
		CJumunCheck jc = new CJumunCheck();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		
		jc.m_msg = strMsg;
		UINT ret = jc.DoModal();
		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "주문확인", nType);
		if (ret == IDOK)
		{
			return true;
		
		} else if (ret == IDCANCEL)
		{
			return false;
		}	
		//return false;
	}
	return false;
}


BOOL multiCheckupMistakeOrder(CWnd* pWnd, double strPrice, int strQuantity, CString strCode, CString strListedQuantity = _T(""), CString strCodeName = _T(""))
{
	ASSERT(pWnd != NULL);
	//ASSERT(!strPrice.IsEmpty());
	//ASSERT(!strQuantity.IsEmpty());
	ASSERT(!strCode.IsEmpty());

	CString strMsg(_T(""));
	CString strMsg2(_T(""));
	
	long double lPrice = strPrice;//atol((LPCTSTR)strPrice);
	long double lQty = strQuantity;//atol((LPCTSTR)strQuantity);
	ULONG lListedQty = atol((LPCTSTR)strListedQuantity) * 1000;
	long double lAmount = 0;
	int smode = ALLOW_ORDER;


	

	UINT nType = MB_YESNO | MB_ICONINFORMATION | MB_DEFBUTTON2;
	//AfxMessageBox(strCode);
	switch (strCode.GetLength())
	{
	case 6:		// 주식, ELW
		{
			lAmount = lPrice * lQty;
			/*
			CString tmp;
			tmp.Format("%f = %f * %f",lAmount, lQty, lPrice);
			AfxMessageBox(tmp);	
			*/	
			if (lAmount > 2000000000 && lAmount <= 5000000000)
				strMsg.Format("%s[%s]의 %s\n", strCodeName, strCode,  _T("주문금액이 20억원을 초과하는 이상주문입니다."));
			else if (lAmount > 5000000000)
			{
				strMsg.Format("%s[%s]의 %s\n", strCodeName, strCode,  _T("주문금액이 50억원을 초과하는 이상주문입니다."));
				nType = MB_OK | MB_ICONINFORMATION;
			}
			
			//else 
			{
				if (strCode.Left(1) == _T("5") || strCode.Left(1) == _T("6") || 
					strCode.Left(1) == _T("7") || strCode.Left(1) == _T("8"))
					return TRUE;
				
				if (lListedQty!=0)
				{
					lQty = lQty * 100;
					
					if (lQty > lListedQty && lQty / 3 <= lListedQty)
						strMsg2.Format("%s[%s]종목의 %s\n", strCodeName,strCode,  _T("주문수량이 상장주식수 대비 1%를 초과하는 이상주문입니다."));
					else if (lQty / 3 > lListedQty)
					{
						strMsg2.Format("%s[%s]종목의 %s\n", strCodeName, strCode, _T("주문수량이 상장주식수 대비 3%를 초과하는 이상주문입니다."));
						nType = MB_OK | MB_ICONINFORMATION;
					}
				}
			}
		} 
		break;

	case 8:		// 선물옵션
		{
			if (strCode.Left(1) == _T("1") || strCode.Left(1) == _T("4"))
			{
				if (lQty > 300 && lQty <= 500)
					strMsg.Format("%s\n", _T("주문수량이 300계약을 초과한 이상주문입니다."));
				else if (lQty > 500)
				{
					strMsg.Format("%s\n", _T("주문수량이 500계약을 초과한 이상주문입니다."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else if (strCode.Left(1) == _T("2") || strCode.Left(1) == _T("3"))
			{
				lAmount = lQty * lPrice * 100000;


				if (lAmount > 2000000000 && lAmount <= 5000000000)
					strMsg.Format("%s\n", _T("주문금액이 20억원을 초과하는 이상주문입니다."));
				else if (lAmount > 5000000000)
				{
					strMsg.Format("%s\n", _T("주문금액이 50억원을 초과하는 이상주문입니다."));
					nType = MB_OK | MB_ICONINFORMATION;
				}
			}
			else
				return TRUE;
		}	
		break;

	default:
		break;
	}
	
	if (strMsg.IsEmpty() && strMsg2.IsEmpty())
		return TRUE;
	else
	{

		CJumunCheck jc = new CJumunCheck();
		if ((strMsg2 != "") && (strMsg2 != ""))	strMsg = strMsg + strMsg2;
		else if (strMsg2 != "")	strMsg = strMsg2;
		jc.m_msg = strMsg;
		UINT ret = jc.DoModal();
		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "주문확인", nType);
		if (ret == IDOK)
		{
			return true;
		
		} else if (ret == IDCANCEL)
		{
			return false;
		}		
		//return false;
	}
	return false;
}

//2014.05.22 KSJ 주문착오방지
class MistakeOrder
{
public:
	CString strAccn;			//계좌번호	3007 그룹주문에서 사용
	CString strCode;			//종목코드
	int nMmgb;					//매매구분
	CString strHogb;			//호가구분
	long double lPrice;			//주문가
	long double lQuantity;		//주문수량
	CString strMaxPrc;			//상한가
	CString strMinPrc;			//하한가
	CString strListedQuantity;	//상장주식수
	CString strCurPrc;			//현재가
	CString strStdPrc;			//기준가
	int nQtyx;					//거래승수	주식선물에서 사용
};

//0:이상없음, 1: 경고, -1: 보류
BOOL CheckMistakeSOrder(CWnd* pWnd, MistakeOrder misOrd)
{
	ASSERT(pWnd != NULL);
			
	int nResult = 0;
	
	BOOL bReturn = FALSE;
	CString strMsg = "";
	
	long double lTotalAmt = 0;
	long lListedQuantity = 0;
	long double lPrice = misOrd.lPrice;
	long double lQty = misOrd.lQuantity;
	TCHAR cHogb;
		
	cHogb = misOrd.strHogb.GetAt(1);
	
	if(misOrd.nMmgb == 3 && misOrd.nMmgb == 2)	//정정이면서 전부일때는 검사를 못한다. 수량을 모르니깐..
		return TRUE;

	//금액기준 체크strCode.operator LPCTSTR()
	// 가격미지정주문인 경우에는 매수/정정일때는 상한가, 매도일때 하한가 계산
	if((misOrd.nMmgb == 2 || misOrd.nMmgb == 3)&& (cHogb == '3' || cHogb == '5' || cHogb == '6' || cHogb == '7'))	//매수일때 상한가
		lTotalAmt = lQty * atol(misOrd.strMaxPrc);
	else if(misOrd.nMmgb == 1 && (cHogb == '3' || cHogb == '5' || cHogb == '6' || cHogb == '7'))	//매도일때 하한가
		lTotalAmt = lQty * atol(misOrd.strMinPrc);
	else if(cHogb == '1')	//장개시전/시간외 종가
	{
		if(!misOrd.strHogb.Compare("81"))	//시간외 종가
			lTotalAmt = lQty * atol(misOrd.strCurPrc);
		else if(!misOrd.strHogb.Compare("61"))	//장개시전
			lTotalAmt = lQty * atol(misOrd.strStdPrc);
	}
	else
		lTotalAmt = lQty * lPrice;
	
	if(lTotalAmt > 2000000000 && lTotalAmt <= 5000000000)
	{
		strMsg = "[경고]주문금액이 20억을 초과하는 이상주문입니다.";
		nResult = 1;
	}
	else if(lTotalAmt > 5000000000)
	{
		strMsg = "[보류]주문금액이 50억을 초과하는 이상주문입니다.";
		nResult = -1;
	}
	
	int nIndex = 0;
	if(misOrd.strCode.GetLength() == 7)		nIndex = 1;

	//수량기준 체크
	if(misOrd.strCode.GetAt(nIndex) >= '5' && misOrd.strCode.GetAt(nIndex) <= '8')
	{
		//ELW
	}
	else
	{
		//주식
		lListedQuantity = atol(misOrd.strListedQuantity);
		
		if(lQty > (lListedQuantity * 0.01) && lQty <= (lListedQuantity * 0.03))
		{
			strMsg += "\n[경고]주문수량이 상장주식수 대비 1%를 초과하는 이상주문입니다.";
			
			if(nResult != -1)	nResult = 1;	//위에서 보류일때는 그대로 보류로 세팅해둔다.
		}
		else if(lQty > (lListedQuantity * 0.03) && lListedQuantity > 0)
		{
			strMsg += "\n[보류]주문수량이 상장주식수 대비 3%를 초과하는 이상주문입니다.";
			nResult = -1;
		}
	}
	
	if (strMsg.IsEmpty())
		bReturn = TRUE;
	else
	{
		CJumunCheck jc = new CJumunCheck();
		
		jc.m_msg = strMsg;
		UINT ret = jc.DoModal();
		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "주문확인", nType);
		if (ret == IDOK)
		{
			bReturn = TRUE;
			
		} else if (ret == IDCANCEL)
		{
			bReturn = FALSE;
		}	
		//return false;
	}
	
	return bReturn;
}

//0:이상없음, 1: 경고, -1: 보류
BOOL CheckMistakeFOrder(CWnd* pWnd, MistakeOrder misOrd)
{
	ASSERT(pWnd != NULL);
	
	int nResult = 0;
	
	BOOL bReturn = FALSE;
	CString strMsg ="";
	
	bool bSpread = false;
	TCHAR cSigb;	//시장구분	선물, 옵션
	TCHAR cHogb;	//호가구분
	CString sMkty;
	int nTick1 = 0, nTick2 = 0;
	long lTickWarn = 0, lTickLimit = 0;	//경고, 보류
	long double dCurr = 0.0;	//현재가
	long double lTotalAmt = 0;

	long double lQty = misOrd.lQuantity;
	long double dPrc = misOrd.lPrice * 100;
	int nQtyx = (misOrd.nQtyx > 0) ? misOrd.nQtyx:1;	//거래승수

	cSigb = misOrd.strCode.GetAt(0);
	sMkty = misOrd.strCode.Mid(1, 2);
	cHogb = misOrd.strHogb.GetAt(1);

	if(cSigb == '1' || cSigb == '4')
	{
		if(cSigb == '4') bSpread = true;

		if(!sMkty.Compare("01"))	//코스피200선물
		{
			if(!bSpread)
			{
				if(lQty > 300 && lQty <= 500)
				{
					strMsg += "[경고]주문수량이 300계약을 초과한 이상주문입니다.";
					nResult = 1;
				}
				else if(lQty > 500)
				{
					strMsg += "[보류]주문수량이 500계약을 초과한 이상주문입니다.";
					nResult = -1;
				}
			}
			else
			{
				nTick1 = 50;
				nTick2 = 100;
				lTickWarn = (0.05 * nTick1) * 100;
				lTickLimit = (0.05 * nTick2) * 100;

				dCurr = atof(misOrd.strCurPrc) * 100;
			}
		}
		else if(!sMkty.Compare("65") || !sMkty.Compare("66") || !sMkty.Compare("67"))	//금리선물
		{
			nTick1 = 50;
			nTick2 = 100;
			lTickWarn = (0.01 * nTick1) * 100;
			lTickLimit = (0.01 * nTick2) * 100;

			dCurr = atof(misOrd.strCurPrc) * 100;

			if(!bSpread)
			{
				if(cHogb == '5')
				{
					//기준가 세팅
					dCurr = atof(misOrd.strStdPrc) * 100;
				}
			}
		}
		else if(!sMkty.Compare("75") || !sMkty.Compare("76") || !sMkty.Compare("77"))	//통화선물
		{
			long double lTmpCurr =0;

			lTmpCurr = atof(misOrd.strCurPrc);

			if(bSpread)
			{
				nTick1 = 50;
				nTick2 = 100;		
			}
			else
			{
				nTick1 = 100;
				nTick2 = 200;

				if(cHogb == '5')
				{
					//기준가 세팅
					lTmpCurr = atof(misOrd.strStdPrc);
				}
			}
		
			lTickWarn = (0.10 * nTick1) * 100;
			lTickLimit = (0.10 * nTick2) * 100;
			
			dCurr = lTmpCurr * 100;
		}
		else if(10 <= atoi(sMkty) && atoi(sMkty) <= 59)	//주식선물
		{
			if(!bSpread)
			{
				if(cHogb == '3' || cHogb == '5' || cHogb == '6')
				{
					dPrc = atof(misOrd.strStdPrc);
					//기준가 세팅
				}

				lTotalAmt = lQty * dPrc * nQtyx;

				if(lTotalAmt > 500000000 && lTotalAmt <= 1000000000)
				{
					strMsg += "[경고]주문금액이 5억을 초과하는 이상주문입니다.";
					nResult = 1;
				}
				else if(lTotalAmt > 1000000000)
				{
					strMsg += "[보류]주문금액이 10억을 초과하는 이상주문입니다.";
					nResult = -1;
				}
			}
			else
			{
				nTick1 = 50;
				nTick2 = 100;
				
				int nHogaPrc = 0;

				if(dPrc < 10000)					nHogaPrc = 5;
				else if(dPrc < 50000)				nHogaPrc = 25;
				else if(dPrc < 100000)				nHogaPrc = 50;
				else if(dPrc < 500000)				nHogaPrc = 250;
				else								nHogaPrc = 500;

				lTickWarn = 50 * nHogaPrc;
				lTickLimit = 100 * nHogaPrc;

				dCurr = atof(misOrd.strCurPrc);
			}
		}		

		if(cHogb != '3' && cHogb != '6' && lTickWarn > 0 && lTickLimit > 0)
		{
			CString strTmpMsg;
			if((dPrc < (dCurr - lTickLimit - 0.00001)) || (dPrc > (dCurr + lTickLimit + 0.00001)))
			{
				strTmpMsg.Format("\n[보류]주문가격이 %d 틱을 벗어난 이상주문입니다.", nTick2);
				strMsg += strTmpMsg;
				nResult = -1;
			}
			else if((dPrc < (dCurr - lTickWarn - 0.00001)) || (dPrc > (dCurr + lTickWarn + 0.00001)))
			{
				strTmpMsg.Format("\n[경고]주문가격이 %d 틱을 벗어난 이상주문입니다.", nTick1);
				strMsg += strTmpMsg;

				if(nResult != -1)	nResult = 1;	//위에서 보류일때는 그대로 보류로 세팅해둔다.
			}
		}
	}
	else if(cSigb == '2' || cSigb == '3')	// 코스피200 콜/풋 옵션
	{
		if(cHogb == '3' || cHogb == '5' || cHogb == '6')	//시장가, 최유리, 조건부
		{
			dPrc = atof(misOrd.strStdPrc) * 100;
			//기준가 세팅
		}

		lTotalAmt = lQty * dPrc * 5000;

		if(lTotalAmt > 2000000000 && lTotalAmt <= 5000000000)
		{
			strMsg += "[경고]주문금액이 20억을 초과한 이상주문입니다.";
			nResult = 1;
		}
		else if(lTotalAmt > 5000000000)
		{
			strMsg += "[보류]주문금액이 50억을 초과한 이상주문입니다.";
			nResult = -1;
		}
	}
	
	if (strMsg.IsEmpty())
		bReturn = TRUE;
	else
	{
		CJumunCheck jc = new CJumunCheck();
			
		jc.m_msg = !misOrd.strAccn.IsEmpty() ? "계좌번호[" + misOrd.strAccn + "]\n\n" + strMsg:strMsg;
		UINT ret = jc.DoModal();
		if (jc)	delete jc;
		//UINT ret = pWnd->MessageBox(strMsg, "주문확인", nType);
		if (ret == IDOK)
		{
			bReturn = TRUE;
			
		} else if (ret == IDCANCEL)
		{
			bReturn = FALSE;
		}	
		//return false;
	}
	
	return bReturn;
}

BOOL CheckupMistakeOrder(CWnd* pWnd, MistakeOrder misOrd)
{
	if(misOrd.strCode.GetLength() == 8)
	{
		return	CheckMistakeFOrder(pWnd, misOrd);
	}
	else if (misOrd.strCode.GetLength() < 8 && misOrd.strCode.GetLength() >= 6)
	{
		return	CheckMistakeSOrder(pWnd, misOrd);
	}

	return FALSE;
}
//KSJ END