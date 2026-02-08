// ForceIndex.h: interface for the CForceIndex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORCEINDEX_H__0F65D42A_689D_4AB0_905F_E32064336310__INCLUDED_)
#define AFX_FORCEINDEX_H__0F65D42A_689D_4AB0_905F_E32064336310__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CForceIndex : public CInnerFunction 
{
public:
	CForceIndex();
	virtual ~CForceIndex();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_FORCEINDEX_H__0F65D42A_689D_4AB0_905F_E32064336310__INCLUDED_)
