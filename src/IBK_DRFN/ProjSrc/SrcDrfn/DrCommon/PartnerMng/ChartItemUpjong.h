// ChartItemUpjong.h: interface for the CChartItemUpjong class.
//
// 차트 업종, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMUPJONG_H__C7FBF33F_4605_4058_852E_328EFB8A3C91__INCLUDED_)
#define AFX_CHARTITEMUPJONG_H__C7FBF33F_4605_4058_852E_328EFB8A3C91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_p0603.h"
//@Solomon:091123SM068
#include "../../inc/IChartBozoMsgMng.h"

class CChartItemUpjong  
{
public:
	CChartItemUpjong();
	virtual ~CChartItemUpjong();
	void SetInData(KB_p0603_InRec1* pData);

	KB_p0603_InRec1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	//@Solomon:091123SM068	-->
	CChartBozoInput m_bojoInput;
	CChartBozoInput	GetBozoInputData()	{ return m_bojoInput;	};
	//@Solomon:091123SM068	<--

private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMUPJONG_H__C7FBF33F_4605_4058_852E_328EFB8A3C91__INCLUDED_)
