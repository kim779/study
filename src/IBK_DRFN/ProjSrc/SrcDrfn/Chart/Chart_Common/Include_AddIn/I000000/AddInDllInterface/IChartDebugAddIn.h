// IChartDebugAddIn.h: interface for the IChartDebugAddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTDEBUGADDIN_H__AC809E8D_5E59_4206_AB03_DBAEB215B8FA__INCLUDED_)
#define AFX_ICHARTDEBUGADDIN_H__AC809E8D_5E59_4206_AB03_DBAEB215B8FA__INCLUDED_

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/8
// Comments		: Debug AddIn Custom Interface
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartDebugAddIn : public IUnknown
{
protected:
	virtual ~IChartDebugAddIn()	{}

// (2005/8/16 - Seung-Won, Bae) Advise & Unadvise Real Update Logging Interface
public:
	virtual void	AddRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
	virtual void	ResetRealUpdate( WORD p_wRealRecordID, const char *p_szRealKeyCode) = 0;
};

#endif // !defined(AFX_ICHARTDEBUGADDIN_H__AC809E8D_5E59_4206_AB03_DBAEB215B8FA__INCLUDED_)
