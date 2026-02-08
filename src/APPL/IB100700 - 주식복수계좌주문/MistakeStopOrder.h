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
}