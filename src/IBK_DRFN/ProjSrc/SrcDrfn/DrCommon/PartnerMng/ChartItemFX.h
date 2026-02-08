// ChartItemFX.h: interface for the CChartItemFX class.
//
// 차트 주식, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ChartItemFX_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_ChartItemFX_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/SM_fx112.h"
//@Solomon:091123SM068
#include "../../inc/IChartBozoMsgMng.h"

class CChartItemFX  
{
public:
	CChartItemFX();
	virtual ~CChartItemFX();
	void SetInData(SM_fx112_InRec1* pData);

	int		m_nFldCnt;//m_nTickCnt, m_nSecCnt, m_nMinCnt, m_nDayCnt, m_nWeekCnt, m_nGridCnt;
	int		m_nReqFldCnt;
	SM_fx112_InRec1 m_InData;
//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
//@Solomon:091123SM068	<--

	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	
private:
	bool m_bSet;
};

#endif // !defined(AFX_ChartItemFX_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
