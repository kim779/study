// Sin.h: interface for the CSin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIN_H__E100A019_58AD_4E14_B7FC_CB5D66E7C332__INCLUDED_)
#define AFX_SIN_H__E100A019_58AD_4E14_B7FC_CB5D66E7C332__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSin  : public CInnerFunction
{
public:
	CSin();
	virtual ~CSin();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SIN_H__E100A019_58AD_4E14_B7FC_CB5D66E7C332__INCLUDED_)
