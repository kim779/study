// MI.h: interface for the CMI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MI_H__AA4F25DC_133C_446C_9848_91DD5F951B2B__INCLUDED_)
#define AFX_MI_H__AA4F25DC_133C_446C_9848_91DD5F951B2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMI  : public CInnerFunction
{
public:
	CMI();
	virtual ~CMI();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_MI_H__AA4F25DC_133C_446C_9848_91DD5F951B2B__INCLUDED_)
