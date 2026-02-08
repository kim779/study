// IMiniJisu.h: interface for the IMiniJisu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMINIJISU_H__005A49E0_D577_49DD_84C9_8227EAC1A366__INCLUDED_)
#define AFX_IMINIJISU_H__005A49E0_D577_49DD_84C9_8227EAC1A366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/3/23
// Comments		: MiniJisu Interface
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

interface IMiniJisu : public IUnknown  
{
protected:
	virtual ~IMiniJisu()	{}

// (2006/3/23 - Seung-Won, Bae) MiniJisu Interface
public:
	virtual void	SendToChartMiniJisu(LPCTSTR szCommand, LPCTSTR szParam1, LPCTSTR szParam2, LPCTSTR szParam3)	= 0;
};

#endif // !defined(AFX_IMINIJISU_H__005A49E0_D577_49DD_84C9_8227EAC1A366__INCLUDED_)
