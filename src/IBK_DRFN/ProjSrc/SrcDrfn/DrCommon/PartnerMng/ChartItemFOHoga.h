// ChartItemFOHoga.h: interface for the CChartItemFOHoga class.
//
// 차트 주식, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMFOHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
#define AFX_CHARTITEMFOHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_f0201.h"

class CChartItemFOHoga  
{
public:
	CChartItemFOHoga();
	virtual ~CChartItemFOHoga();
	void SetInData(KB_f0201_InRec1* pData);

	int		m_nCnt;
	KB_f0201_InRec1 m_InData;
	CString Convert();
	char* Convert2Struct(LPBYTE pData, int nLen, CString szTRCode, int &nDataLen);
	//KHD : IBK는 선물하고 옵션하고 따로 관리하기에 분리함.
	CString ConvertFuture();
	CString ConvertOption();
	CString ConvertCommodity();//상품선물
	int m_nCodeType;
//END
private:
	bool m_bSet;
};

#endif // !defined(AFX_CHARTITEMFOHOGA_H__981C9079_A480_41FB_AB52_E106169CA5BE__INCLUDED_)
