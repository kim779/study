// StochasticsD_Raw.h: interface for the CStochasticsD_Raw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCHASTICSD_RAW_H__3397CFCE_3858_44EC_9122_D396F28F2506__INCLUDED_)
#define AFX_STOCHASTICSD_RAW_H__3397CFCE_3858_44EC_9122_D396F28F2506__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CStochasticsD_Raw  : public CInnerFunction
{
public:
	CStochasticsD_Raw();
	virtual ~CStochasticsD_Raw();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_STOCHASTICSD_RAW_H__3397CFCE_3858_44EC_9122_D396F28F2506__INCLUDED_)
