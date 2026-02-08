// ConfigHelper.h: interface for the CConfigHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONFIGHELPER_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
#define AFX_CONFIGHELPER_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "Control/BitmapItemComboBox.h"
#include "../Common_ST/STConfigDef.h"

// graph region 색
class CConfigHelper
{
public:
	// 신호설정등에서
	// 신호모양표시
	// nType  (0) : 매수타입, (1) : 매도타입
	static int cmSignal(CComboBox* pCb, int nType)
	{
		char *aStr[] = STRLIST_SIGNAL;

		int nSize = sizeof(aStr) / sizeof(char*);
		for(int i=0; i<nSize; i+=2) {
			if(nType==0)	pCb->AddString(aStr[i]);
			else pCb->AddString(aStr[i+1]);
		}

		//pCb->AddString("↑");
		//pCb->AddString("↓");
		//pCb->AddString("▲");
		//pCb->AddString("▼");
		//pCb->AddString("●");
		//pCb->AddString("○");
		//pCb->AddString("■");
		//pCb->AddString("□");

		return pCb->GetCount();
	}

	// 색상표시
	static void cmColor(CComboBox* pCb)
	{
	}

	// % , Point
	static int cmPercentPoint(CComboBox* pCb)
	{
		pCb->AddString("%");
		pCb->AddString("Point");

		return pCb->GetCount();
	}

	// 계좌콤보, 계좌명
	static int cmAcct(CComboBox* pCb, CStatic* pTxt)
	{
	}

	// 진입가격
	// nCodeType  주식(0), 선옵(1)
	static int cmBuy(CComboBox* pCb, int nCodeType)
	{
		pCb->AddString("현재가");
		pCb->AddString("매수1호가");
		pCb->AddString("매수2호가");
		pCb->AddString("매수3호가");
		pCb->AddString("매도1호가");
		pCb->AddString("매도2호가");
		pCb->AddString("매도3호가");

		if(nCodeType==0)
		{
			pCb->AddString("상한가");
			pCb->AddString("하한가");
		}

		return pCb->GetCount();
	}

	// 청산가격
	// nCodeType  주식(0), 선옵(1)
	static int cmSell(CComboBox* pCb, int nCodeType)
	{
		pCb->AddString("현재가");
		pCb->AddString("매수1호가");
		pCb->AddString("매수2호가");
		pCb->AddString("매수3호가");
		pCb->AddString("매도1호가");
		pCb->AddString("매도2호가");
		pCb->AddString("매도3호가");

		if(nCodeType==0)
		{
			pCb->AddString("상한가");
			pCb->AddString("하한가");
		}

		return pCb->GetCount();
	}

	// 주문방식
	static int cmJumunMethod(CComboBox* pCb)
	{
		pCb->AddString("주문없음(가상적용)");
		pCb->AddString("반자동주문(확인 후 주문전송)");
//		pCb->AddString("자동주문(확인없이 주문전송)");

		return pCb->GetCount();
	}
	// 주문시점
	static int cmJumunTime(CComboBox* pCb)
	{
		pCb->AddString("조건 만족시");
		pCb->AddString("봉 완성시");

		return pCb->GetCount();
	}

	// 주문시작신호
	static int cmJumunStartSignal(CComboBox* pCb)
	{
		pCb->AddString("진입신호");
		pCb->AddString("모든신호");

		return pCb->GetCount();
	}

	// 신호음발생 진입시
	static void cmBuySound(CComboBox* pCb)
	{
	}

	// 신호음발생 청산시
	static int cmSellSound(CComboBox* pCb)
	{
	}

	// 시간
	static int cmTimeA(CComboBox* pCb)
	{
		pCb->AddString("오전");
		pCb->AddString("오후");

		return pCb->GetCount();
	}

	// 허용포지션
	static int cmAllow(CComboBox* pCb)
	{
		pCb->AddString("매수/매수청산");
		pCb->AddString("매도/매도청산");
		pCb->AddString("모두 허용");

		return pCb->GetCount();
	}

	// 중복진입
	static int cmRepeatIn(CComboBox* pCb)
	{
		pCb->AddString("허용 안함");
		pCb->AddString("다른 진입신호만 허용");
		pCb->AddString("모두 허용");

		return pCb->GetCount();
	}

	// 운영설정에서 표시형태
	// *
	static int cmStar(CComboBox* pCb)
	{
		char *aStr[] = STRLIST_CLEAR;

		int nSize = sizeof(aStr) / sizeof(char*);
		for(int i=0; i<nSize; i++)
			pCb->AddString(aStr[i]);

		//pCb->AddString("★");
		//pCb->AddString("☆");
		//pCb->AddString("♣");
		//pCb->AddString("♧");
		//pCb->AddString("◈");
		//pCb->AddString("▣");
		//pCb->AddString("■");
		//pCb->AddString("□");

		return pCb->GetCount();
	}

};

#endif // !defined(AFX_CONFIGHELPER_H__C7820E6C_33BB_4535_AA24_5FEBB070AF9F__INCLUDED_)
