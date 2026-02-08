// BullBearArea.cpp: implementation of the CBullBearArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BullBearArea.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBullBearArea::CBullBearArea()
{

}

CBullBearArea::~CBullBearArea()
{

}

CString CBullBearArea::GetInputs(long lType)
{
	CString strInputs;
	strInputs.Format("[%s]Numeric,String",m_strInputInfo,m_strName);
	return strInputs;
}


double CBullBearArea::Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10)
{
	double dValue = _variant_t(varArg1);
	CString strTitleName = (LPCTSTR)_bstr_t(_variant_t(varArg2));

	double dRetValue  = 0;
	if(pIController->IsFirstInThisMem())	// init
	{
		CString strTitle;
		CString strComment;
		if(!pIController->IsHoldingBullBearAreaData())
		{
			pIController->SetVariables(0,"Bullbear",NumericSeries,0);
			pIController->SetBullBearDataHolder("Bullbear");
		}
		strTitle.Format("%sNAME",m_strName);
		strComment.Format("%s,",strTitleName);
		pIController->AddCommentAtBullBearData(strTitle,strComment);
	}
	if(dValue)
	{
		pIController->SetBullBearData(0,m_dPosValue);
	}	
	return dRetValue;
}


