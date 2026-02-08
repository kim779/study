// Data_ConditionTitle.h: interface for the CData_ConditionTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_CONDITIONTITLE_H__30D0ED4D_1496_49DA_BA4F_D8C177975F2A__INCLUDED_)
#define AFX_DATA_CONDITIONTITLE_H__30D0ED4D_1496_49DA_BA4F_D8C177975F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 27
// Comments		: 
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
class CData_ConditionTitle  
{
public:
	CString GetConditionTitle( const CString &strIndicatorName );


private:
	CString MakeString( const CString &title, const CString &compart );
};

#endif // !defined(AFX_DATA_CONDITIONTITLE_H__30D0ED4D_1496_49DA_BA4F_D8C177975F2A__INCLUDED_)
