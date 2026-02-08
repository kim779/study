// SPrice.h: interface for the CSPrice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRICE_H__ED729725_DE44_42FC_9456_A562F9EC8A16__INCLUDED_)
#define AFX_SPRICE_H__ED729725_DE44_42FC_9456_A562F9EC8A16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSPrice   : public CInnerFunction
{
public:
	CSPrice();
	virtual ~CSPrice();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SPRICE_H__ED729725_DE44_42FC_9456_A562F9EC8A16__INCLUDED_)
