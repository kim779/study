// ChartItemMarket.h: interface for the CChartItemMarket class.
//
// 차트 업종, 구조체 <-> FID 컨버터 클래스
// 2009.10.06 솔로몬 특화, 위닉스 플랫폼
// by Jeoyoho
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTITEMMARKET_H__C7FBF33F_4605_4058_852E_328EFB8A3C91__INCLUDED_)
#define AFX_CHARTITEMMARKET_H__C7FBF33F_4605_4058_852E_328EFB8A3C91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../chart/commontr/KB_d1015.h"

class CChartItemMarket
{
public:
	CChartItemMarket();
	virtual ~CChartItemMarket();

private:
	bool m_bSet;

	KB_d1015_InRec1		m_InData;

public:
	void		SetInData(KB_d1015_InRec1* pData);

	CString		Convert();
	char*		Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	char*		Convert2Struct1(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	char*		Convert2Struct2(LPBYTE pData, int nLen, int nKey, int &nDataLen);

	int			GetSendDataType();
	CString		GetSendDataType(int nType);

	CString		fnStructType1(CString strGrid, int nType);
	CString		fnStructType2(CString strGrid, int nType);

protected:
	void		MakeDataOfSuspensSeContract(CString &strData, CStringArray* pDataArr);
};

#endif // !defined(AFX_CHARTITEMMARKET_H__C7FBF33F_4605_4058_852E_328EFB8A3C91__INCLUDED_)
