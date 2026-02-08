// PlotBaseLine.h: interface for the CPlotBaseLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLOTBASELINE_H__2785CAD2_B64C_4C89_A411_A5C393CF57E3__INCLUDED_)
#define AFX_PLOTBASELINE_H__2785CAD2_B64C_4C89_A411_A5C393CF57E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CPlotBaseLine  : public CInnerFunction
{
public:
	virtual ~CPlotBaseLine();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController, long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
	double GetTemp() { return m_lIndex;}
protected:
	CString m_strName;
	CString m_strInputInfo;
	long	m_lIndex;

};

#endif // !defined(AFX_PLOTBASELINE_H__2785CAD2_B64C_4C89_A411_A5C393CF57E3__INCLUDED_)
