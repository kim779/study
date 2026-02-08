// TAxis.h: interface for the CTAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAXIS_H__878198F6_0A11_46C4_9AED_D3F63A0CEDD3__INCLUDED_)
#define AFX_TAXIS_H__878198F6_0A11_46C4_9AED_D3F63A0CEDD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/9/22
// Comments		: TChart Axis Interface Class
// Using way	: It's Self Contructing and Detructing Object by CTAxisImp.
// See			: 
//-----------------------------------------------------------------------------

class CTAxisImp;
class AFX_EXT_CLASS CTAxis : public CObject  
{
friend class CTAxisImp;
protected:
	CTAxis( CTAxisImp *p_pTAxisImp);
	virtual ~CTAxis();

// (2005/9/23 - Seung-Won, Bae) Defines
public:
	typedef enum	{
						m_eATypeLeft,
						m_eATypeRight,
						m_eATypeTop,
						m_eATypeBottom,
						m_eATypeVertical,
						m_eATypeHorizontal
		}	EAxisType;

// (2005/9/23 - Seung-Won, Bae) Manage TAxis Implementation.
protected:
	CTAxisImp *	m_pTAxisImp;
public:
	CTAxisImp *	GetTAxisImp( void)		{	return m_pTAxisImp;	}

// (2005/9/23 - Seung-Won, Bae) Operation
public:
	void		GetPosition( double &p_dPosition, double &p_dStart, double &p_dEnd, double &p_dZ);
	void		SetPosition( double p_dPosition, double p_dStart, double p_dEnd, double p_dZ);
public:
	void		SetAutomatic( BOOL p_bMinimum, BOOL p_bMaximum);
	void		GetMinMax( double &p_dMin, double &p_dMax);
	void		SetMinMax( double p_dMin, double p_dMax);
public:
	double		GetScaleUnit( void);					// (2005/9/29 - Seung-Won, Bae) 0 is Automatic
	void		SetScaleUnit( double p_dScaleUnit);
public:
	void		SetTicksVisible( BOOL p_bAxisLine, BOOL p_bGridLine, BOOL p_bTickLine, BOOL p_bInnerTickLine, BOOL p_bMinorGridLine, BOOL p_bMinorTickLine);
};

#endif // !defined(AFX_TAXIS_H__878198F6_0A11_46C4_9AED_D3F63A0CEDD3__INCLUDED_)
