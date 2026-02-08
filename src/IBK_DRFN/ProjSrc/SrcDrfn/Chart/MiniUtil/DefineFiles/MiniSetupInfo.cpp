// MiniSetupInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MiniSetupInfo.h"

void CMiniSetupInfo::SetSetupInfo(CString strTitle, UINT nMarketType, UINT nTimeType, UINT nPeriod, UINT nDataOption)
{
	m_strTitle = strTitle;
	m_nMarketType = nMarketType;
	m_nTimeType = nTimeType;
	m_nPeriod = nPeriod;
	m_nDataOption = nDataOption;
}

void CMiniSetupInfo::ConvertStringToSetupInfo(CString strSetupInfo)
{
	int nStart = 0, nEnd = 0;
	CString strTitle;
	UINT nMarketType;
	UINT nTimeType;
	UINT nPeriodType;
	UINT nDataOption;
	nEnd = strSetupInfo.Find(";", nStart);
	strTitle = strSetupInfo.Left(nEnd);
	strSetupInfo = strSetupInfo.Mid(nEnd + 1);
	nEnd = strSetupInfo.Find(";", nStart);
	nMarketType = atoi(strSetupInfo.Left(nEnd));
	strSetupInfo = strSetupInfo.Mid(nEnd + 1);
	nEnd = strSetupInfo.Find(";", nStart);
	nTimeType = atoi(strSetupInfo.Left(nEnd));
	strSetupInfo = strSetupInfo.Mid(nEnd + 1);
	nEnd = strSetupInfo.Find(";", nStart);
	nPeriodType = atoi(strSetupInfo.Left(nEnd));
	strSetupInfo = strSetupInfo.Mid(nEnd + 1);
	nEnd = strSetupInfo.Find(";", nStart);
	nDataOption = atoi(strSetupInfo.Left(nEnd));

	SetSetupInfo(strTitle, nMarketType, nTimeType, nPeriodType, nDataOption);
}

CString CMiniSetupInfo::ConvertSetupInfoToString()
{
	CString strSetupInfo;
	strSetupInfo.Format("%s;%d;%d;%d;%d;,", m_strTitle, m_nMarketType, m_nTimeType, m_nPeriod, m_nDataOption);
	return strSetupInfo;
}
