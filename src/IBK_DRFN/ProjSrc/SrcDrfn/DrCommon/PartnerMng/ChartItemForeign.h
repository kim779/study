// ChartItemForeign.h: interface for the CChartItemForeign class.
//
// 차트 해외, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMFOREIGN_H__C91BEE50_8E43_45E7_B192_364E095C05E3__INCLUDED_)
#define AFX_CHARTITEMFOREIGN_H__C91BEE50_8E43_45E7_B192_364E095C05E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_p0606.h"
//@Solomon:091123SM068
#include "../../inc/IChartBozoMsgMng.h"

class CChartItemForeign  
{
public:
	CChartItemForeign();
	virtual ~CChartItemForeign();
	void SetInData(KB_p0606_InRec1* pData);

	KB_p0606_InRec1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
	//@Solomon:091123SM068	<--

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMFOREIGN_H__C91BEE50_8E43_45E7_B192_364E095C05E3__INCLUDED_)
