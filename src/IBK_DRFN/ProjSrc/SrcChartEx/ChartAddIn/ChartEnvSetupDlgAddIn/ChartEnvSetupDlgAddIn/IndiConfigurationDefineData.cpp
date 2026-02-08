// IndiConfigurationDefineData.cpp: implementation of the CIndiConfigurationDefineData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndiConfigurationDefineData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const char* CIndiConfigurationDefineData::GROUPNAME_TOTAL = "Total";
const char* CIndiConfigurationDefineData::GROUPNAME_SPECIAL = "특수지표";
const char* CIndiConfigurationDefineData::GROUPNAME_MARKET = "시장지표";
const char* CIndiConfigurationDefineData::GROUPNAME_INVESTOR = "투자자동향";

const char* CIndiConfigurationDefineData::INDINAME_DEPOSIT = "고객예탁금";
const char* CIndiConfigurationDefineData::INDINAME_LOAN = "신용융자잔고";
const char* CIndiConfigurationDefineData::INDINAME_UNCOLLECTED = "위탁자미수금";
const char* CIndiConfigurationDefineData::INDINAME_FOREIGNER_SHARE = "외국인지분율";
const char* CIndiConfigurationDefineData::INDINAME_FOREIGNER_BUY = "외국인순매수";
const char* CIndiConfigurationDefineData::INDINAME_FO_UNAPPROVAL = "선물/옵션 미결제 약정";
const char* CIndiConfigurationDefineData::INDINAME_FO_PRICE = "선물거래예수금";


// "주식, 일"에 해당되는 지표인지 유무
bool CIndiConfigurationDefineData::IsIndicatorNameInStocksDaily(const CString& strIndicatorName)
{
	// [고객예탁금], [신용융자잔고], [위탁자미수금], [외국인지분율], [외국인순매수]
	return (strIndicatorName == INDINAME_DEPOSIT || strIndicatorName == INDINAME_LOAN || 
		strIndicatorName == INDINAME_UNCOLLECTED || strIndicatorName == INDINAME_FOREIGNER_SHARE || 
		strIndicatorName == INDINAME_FOREIGNER_BUY);
}
