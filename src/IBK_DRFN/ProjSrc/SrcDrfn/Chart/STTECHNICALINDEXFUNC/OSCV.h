// OSCV.h: interface for the COSCV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OSCV_H__8115C64C_28B3_46FF_BA51_60203D4843C8__INCLUDED_)
#define AFX_OSCV_H__8115C64C_28B3_46FF_BA51_60203D4843C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class COSCV   : public CInnerFunction
{
public:
	COSCV();
	virtual ~COSCV();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_OSCV_H__8115C64C_28B3_46FF_BA51_60203D4843C8__INCLUDED_)
