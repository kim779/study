// Plot1.cpp: implementation of the CPlot1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Plot.h"

#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STConfigdef.h"
#include "../Include_ST/DumpLogger.h"			// for CDumpLogger
#include "../Include_ST/DebugDefine.h"			// for _PLOT_LOG

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CString Dtos(double dValue);

CPlot::~CPlot()
{

}

CString CPlot::GetInputs(long lType)
{
	CString strInputs;
	CString strInputInfo;
	strInputInfo.Format("%s,%s%d,2",INDEX,INDEXNAME,m_lIndex);
	strInputs.Format("[%s]Numeric,String,Numeric=-1",strInputInfo);
	return strInputs;
}


// PlotN(Expresion, "<>",ForeColor, BackColor, Width);
double CPlot::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
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
		CString strPostPart;
		double dValueInput = 0;
		int nPos = strFuncName.Find('%');
		if(nPos>=0)
		{
			strPostPart = strFuncName.Right(strFuncName.GetLength()-nPos-1);
			strFuncName.Delete(nPos,strFuncName.GetLength()-nPos);
			strPostPart.TrimLeft('%');
			nPos = strPostPart.Find('%');
			strVarName = strPostPart.Left(nPos);
			strPostPart = strPostPart.Right(strPostPart.GetLength()-nPos-1);
			strPostPart.TrimLeft('%');
			dValueInput = pIController->GetInputDataInRoot(strVarName,0);
			strPlotName.Format("Plot%02d(%s(%s))%s",m_lIndex+1,strFuncName,Dtos(dValueInput),strPostPart);
		}
		else
		{
			strPlotName.Format("Plot%02d(%s)",m_lIndex+1,strFuncName);
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
	pIController->SetPlotData(m_lIndex,0,dValue);// dValue

#if _PLOT_LOG
	CString strBuffer;
	strBuffer.Format( "%s[%d]:%f\r\n", strFuncName, pIController->BarIndex(), dValue);
	CDumpLogger::LogStringToFile( NULL, strBuffer, strBuffer.GetLength(), "IndiValue");
#endif

	return 0;
}

