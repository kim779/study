// ChartReplay.h: interface for the CChartReplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTREPLAY_H__0F2F97B9_637D_40C2_ACAA_D9AE86083016__INCLUDED_)
#define AFX_CHARTREPLAY_H__0F2F97B9_637D_40C2_ACAA_D9AE86083016__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_d1016.h"

#define STOCK 0
#define FUTURE 1
#define OPTION 2

class CChartReplay  
{
public:
	CChartReplay();
	virtual ~CChartReplay();
	void SetInData(KB_d1016_InRec1* pData);

	KB_d1016_InRec1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);

private:
	bool	m_bSet;
	int		m_nFutOpt; //주식 :0 , 선물 : 1 , 옵션 : 2 
	int		m_nPage;
	int		m_nRepColCnt;
};

#endif // !defined(AFX_CHARTREPLAY_H__0F2F97B9_637D_40C2_ACAA_D9AE86083016__INCLUDED_)
