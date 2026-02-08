// StoFastD.h: interface for the CStoFastD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOFASTD_H__91197FD7_6EE9_494E_BB6A_608F4C1C7377__INCLUDED_)
#define AFX_STOFASTD_H__91197FD7_6EE9_494E_BB6A_608F4C1C7377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CStoFastD  : public CInnerFunction
{
public:
	CStoFastD();
	virtual ~CStoFastD();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STOFASTD_H__91197FD7_6EE9_494E_BB6A_608F4C1C7377__INCLUDED_)
