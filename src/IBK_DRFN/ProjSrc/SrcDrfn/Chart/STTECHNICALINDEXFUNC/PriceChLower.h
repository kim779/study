// PriceChLower.h: interface for the CPriceChLower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRICECHLOWER_H__57A3DC6C_D864_4712_89A4_94D371940017__INCLUDED_)
#define AFX_PRICECHLOWER_H__57A3DC6C_D864_4712_89A4_94D371940017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CPriceChLower  : public CInnerFunction
{
public:
	CPriceChLower();
	virtual ~CPriceChLower();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_PRICECHLOWER_H__57A3DC6C_D864_4712_89A4_94D371940017__INCLUDED_)
