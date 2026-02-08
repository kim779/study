// IStyleManager.h: interface for the CIStyleManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTYLEMANAGER_H__3827F599_9821_44F5_85DB_7BDAEABCB93D__INCLUDED_)
#define AFX_ISTYLEMANAGER_H__3827F599_9821_44F5_85DB_7BDAEABCB93D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"			// for CChartInfo

//////////////////////////////////////////////////////////////////////
// (2009/6/17 - Seung-Won, Bae) Chart Style Data Manager AddIn, Custom Interface.
//////////////////////////////////////////////////////////////////////
interface IStyleManager : public IUnknown
{
protected:
	virtual ~IStyleManager()	{}

public:
	virtual int				GetStyleCount( void)					= 0;
	virtual const char *	GetStyleName( int p_nStyleIndex)		= 0;
	virtual void			SetStyleToChart( int p_nStyleIndex)		= 0;
	virtual void			LoadCurrentStyle( void)					= 0;
	virtual void			DrawSampleImage( HDC p_hDC, int p_nLeft, int p_nTop, int p_nRight, int p_nBottom, int p_nStyleIndex, HGDIOBJ p_hFont)		= 0;
	virtual void			UpdateIndexStyle( const char *p_szGraphName, int p_nStyleIndex)		= 0;
};

#endif // !defined(AFX_ISTYLEMANAGER_H__3827F599_9821_44F5_85DB_7BDAEABCB93D__INCLUDED_)
