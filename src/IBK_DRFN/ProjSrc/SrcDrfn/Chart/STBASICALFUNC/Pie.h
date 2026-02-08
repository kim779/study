// Pie.h: interface for the CPie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIE_H__C430B810_CB4D_4FDB_995F_88D685F04F5A__INCLUDED_)
#define AFX_PIE_H__C430B810_CB4D_4FDB_995F_88D685F04F5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CPie   : public CInnerFunction
{
public:
	CPie();
	virtual ~CPie();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_PIE_H__C430B810_CB4D_4FDB_995F_88D685F04F5A__INCLUDED_)
