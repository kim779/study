// tlIO.h: interface for the CtlIO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLIO_H__707B6700_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TLIO_H__707B6700_50C2_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tool.h"
#include "../../h/axisfire.h"

class AFX_EXT_CLASS CtlIO : public CtlRect  
{
public:
	CtlIO(CChildView* view, UINT id);
	virtual ~CtlIO();

	void	Draw(CDC* dc, int fN);

protected:
	struct _param	m_param;
	HINSTANCE	m_hControlDll;
	enum	{ ioNONE=0, ioEDIT, ioOUT, ioCOMBO, ioMEMO, ioCONTROL }	m_type;
	void	drawEnd();
};

#endif // !defined(AFX_TLIO_H__707B6700_50C2_11D4_A024_00001CD7F9BE__INCLUDED_)
