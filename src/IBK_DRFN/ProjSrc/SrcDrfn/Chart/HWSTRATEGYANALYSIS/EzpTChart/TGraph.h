// TGraph.h: interface for the CTGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGRAPH_H__905739A0_6027_49DC_976C_74B661E7FB33__INCLUDED_)
#define AFX_TGRAPH_H__905739A0_6027_49DC_976C_74B661E7FB33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/22
// Comments		: TChart Graph Interface Class
// Using way	: It's Self Contructing and Detructing Object by CTGraphImp.
// See			: 
//-----------------------------------------------------------------------------

class CTAxis;
class CTGraphImp;
class AFX_EXT_CLASS CTGraph : public CObject  
{
friend class CTGraphImp;
protected:
	CTGraph( CTGraphImp *p_pTGraphImp);
	virtual ~CTGraph();

// (2005/9/23 - Seung-Won, Bae) Defines
public:
	typedef enum { m_eGTypeLine, }	EGraphType;

// (2005/9/23 - Seung-Won, Bae) Manage TGraph Implementation.
protected:
	CTGraphImp *	m_pTGraphImp;
public:
	CTGraphImp *	GetTGraphImp( void)		{	return m_pTGraphImp;	}

// (2005/9/23 - Seung-Won, Bae) Operations
public:
	long		GetXValueWithY( double &p_dX, double p_dY);		// (2005/9/29 - Seung-Won, Bae) return Data Index
	long		GetYValueWithX( double &p_dY, double p_dX);
public:
	void		AddData( double p_dX, double p_dY, COLORREF p_clr = RGB( 0, 0, 0));
	BOOL		SetData( long p_lDataIndex, double p_dX, double p_dY);
public:
	void		SetAxis( CTAxis *p_pTAxis);
	void		SetBarWidthPercent( long p_lPercent);
	void		SetDataSort( BOOL p_bAscending = TRUE, BOOL p_bByX = TRUE);
};

#endif // !defined(AFX_TGRAPH_H__905739A0_6027_49DC_976C_74B661E7FB33__INCLUDED_)
