// AbsValue.h: interface for the CAbsValue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABSVALUE_H__B21FE15C_5011_4B80_BD6B_70F683C978EB__INCLUDED_)
#define AFX_ABSVALUE_H__B21FE15C_5011_4B80_BD6B_70F683C978EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CAbsValue  : public CInnerFunction
{
public:
	CAbsValue();
	virtual ~CAbsValue();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController, long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES


};

#endif // !defined(AFX_ABSVALUE_H__B21FE15C_5011_4B80_BD6B_70F683C978EB__INCLUDED_)
