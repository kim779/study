// MarketPosition.h: interface for the CMarketPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARKETPOSITION_H__0D3678F2_6719_47AA_A3B8_6AC1DADD7564__INCLUDED_)
#define AFX_MARKETPOSITION_H__0D3678F2_6719_47AA_A3B8_6AC1DADD7564__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CMarketPosition  : public CInnerFunction
{
public:
	CMarketPosition();
	virtual ~CMarketPosition();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_MARKETPOSITION_H__0D3678F2_6719_47AA_A3B8_6AC1DADD7564__INCLUDED_)
