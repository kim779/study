// AccumN.h: interface for the CAccumN class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCUMN_H__10FE712B_3C4D_4316_B6AA_DF7C90715684__INCLUDED_)
#define AFX_ACCUMN_H__10FE712B_3C4D_4316_B6AA_DF7C90715684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CAccumN  : public CInnerFunction
{
public:
	CAccumN();
	virtual ~CAccumN();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_ACCUMN_H__10FE712B_3C4D_4316_B6AA_DF7C90715684__INCLUDED_)
