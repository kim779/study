#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlInOut : public CTlRect
{
public:
	CTlInOut(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlInOut();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ ioNONE=0, ioEDIT, ioOUT, ioCOMBO, ioMEMO, ioCONTROL }	m_type;
	void	drawEnd();
};
