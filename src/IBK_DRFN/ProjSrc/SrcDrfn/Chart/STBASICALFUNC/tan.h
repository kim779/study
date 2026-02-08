// tan.h: interface for the Ctan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAN_H__3B9B3348_76DA_4F77_ADA5_37CF82AB3568__INCLUDED_)
#define AFX_TAN_H__3B9B3348_76DA_4F77_ADA5_37CF82AB3568__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CTan   : public CInnerFunction
{
public:
	CTan();
	virtual ~CTan();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController ,  long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES

};

#endif // !defined(AFX_TAN_H__3B9B3348_76DA_4F77_ADA5_37CF82AB3568__INCLUDED_)
