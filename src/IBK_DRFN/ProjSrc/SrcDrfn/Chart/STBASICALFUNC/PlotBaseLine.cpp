// PlotBaseLine.cpp: implementation of the CPlotBaseLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STBasicalFunc.h"
#include "PlotBaseLine.h"
#include "../Common_ST/STConfigdef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CString Dtos(double dValue);

CPlotBaseLine::~CPlotBaseLine()
{

}

CString CPlotBaseLine::GetInputs(long lType)
{
	return "Numeric,String,Numeric=-1";
}


// PlotN(Expresion, "<>",ForeColor, BackColor, Width);
double CPlotBaseLine::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	CString strFuncName = (LPCTSTR)_bstr_t(_variant_t(varArg2));
	long lClr = _variant_t(varArg3);
	if(pIController->IsFirstInThisMem())	// init
	{
		CString strTitle;
		CString strComment;
		CString strPlotName;
		CString strVarName;
		double dValue = 0;
		int nPos = strFuncName.Find('%');
		if(nPos>=0)
		{
			strVarName = strFuncName.Right(strFuncName.GetLength()-nPos-1);
			strFuncName.Delete(nPos,strFuncName.GetLength()-nPos);
			nPos = strVarName.Find('%');
			strVarName = strVarName.Left(nPos);
			dValue = pIController->GetInputDataInRoot(strVarName,0);
			strPlotName.Format("BaseLine%d(%s%s)",m_lIndex+1,strFuncName,Dtos(dValue));
		}
		else
		{
			strPlotName.Format("BaseLine%d(%s)",m_lIndex+1,strFuncName);
		}
		pIController->SetVariables(0,strPlotName,NumericSeries,dValue);
		pIController->SetPlotDataHolder(m_lIndex,strPlotName);		
		if(lClr!=-1)
		{
			strTitle.Format("%s",INDEXCOLOR);
			strComment.Format("%ld",lClr);
		}
		pIController->AddCommentAtPlotData(m_lIndex,strTitle,strComment);
	}
	else
	{
		pIController->SetPlotData(m_lIndex,0,dValue);// dValue
	}
	return 0;
}

