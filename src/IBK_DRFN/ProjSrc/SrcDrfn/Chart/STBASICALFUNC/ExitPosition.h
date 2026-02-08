// ExitPosition.h: interface for the CExitPosition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXITPOSITION_H__8748FCC1_5DAC_41B3_82F5_D24BCDA013A4__INCLUDED_)
#define AFX_EXITPOSITION_H__8748FCC1_5DAC_41B3_82F5_D24BCDA013A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CExitPosition   : public CInnerFunction
{
public:
	CExitPosition();
	virtual ~CExitPosition();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
protected:
	CString m_strName;
	CString m_strInputInfo;
	double	m_dPosValue;

};

#endif // !defined(AFX_EXITPOSITION_H__8748FCC1_5DAC_41B3_82F5_D24BCDA013A4__INCLUDED_)
