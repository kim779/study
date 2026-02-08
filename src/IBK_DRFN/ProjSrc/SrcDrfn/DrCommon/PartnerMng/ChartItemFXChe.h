// ChartItemFXChe.h: interface for the CChartItemFXChe class.
//
// 차트 주식, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ChartItemFXChe_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_ChartItemFXChe_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/SM_CheJanStruct.h"

class CChartItemFXChe  
{
public:
	CChartItemFXChe();
	virtual ~CChartItemFXChe();
	void SetInData(FX_CHE_RQ_INPUT* pData);
	void SetTR(CString strTR);

	int		m_nFldCnt;//m_nTickCnt, m_nSecCnt, m_nMinCnt, m_nDayCnt, m_nWeekCnt, m_nGridCnt;
	int		m_nReqFldCnt;
	CString m_strTR;
	FX_CHE_RQ_INPUT m_InData;

	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	
private:
	bool m_bSet;
};

#endif // !defined(AFX_ChartItemFXChe_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
