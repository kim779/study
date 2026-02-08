// ChartItemFXJan.h: interface for the CChartItemFXJan class.
//
// 차트 주식, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ChartItemFXJan_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_ChartItemFXJan_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/SM_CheJanStruct.h"

class CChartItemFXJan  
{
public:
	CChartItemFXJan();
	virtual ~CChartItemFXJan();
	void SetInData(FX_CHE_RQ_INPUT* pData);
	void SetTR(CString strTR);

	int		m_nFldCnt;//m_nTickCnt, m_nSecCnt, m_nMinCnt, m_nDayCnt, m_nWeekCnt, m_nGridCnt;
	int		m_nReqFldCnt;
	FX_CHE_RQ_INPUT m_InData;
	CString m_strTR;

	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	
private:
	bool m_bSet;
};

#endif // !defined(AFX_ChartItemFXJan_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
