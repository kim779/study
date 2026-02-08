// Graph.h: interface for the CGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPH_H__907DDA03_3A02_4E1C_97B2_37E4AF0B80CD__INCLUDED_)
#define AFX_GRAPH_H__907DDA03_3A02_4E1C_97B2_37E4AF0B80CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2007/3/9
// Comments		: Interface Class of CCGraphImp
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CGraph : public CObject  
{
public:
	virtual void	SetGraphHide( BOOL p_bHide)		= 0;
	virtual BOOL	IsGraphHide( void)				= 0;
	
	// RQ를 얻는 함수 : 복수종목 - ojtaso (20070412)
	virtual LPCTSTR GetGraphRQ() const = 0;
	virtual void	SetGraphRQ(LPCTSTR lpszRQ) = 0;
};

#endif // !defined(AFX_GRAPH_H__907DDA03_3A02_4E1C_97B2_37E4AF0B80CD__INCLUDED_)
