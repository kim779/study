// Sonar_SMA.h: interface for the CSonar_SMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SONAR_SMA_H__3584F7C1_53B0_48F9_A55A_1758FB17883E__INCLUDED_)
#define AFX_SONAR_SMA_H__3584F7C1_53B0_48F9_A55A_1758FB17883E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSonar_SMA : public CInnerFunction  
{
public:
	CSonar_SMA();
	virtual ~CSonar_SMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_SONAR_SMA_H__3584F7C1_53B0_48F9_A55A_1758FB17883E__INCLUDED_)
