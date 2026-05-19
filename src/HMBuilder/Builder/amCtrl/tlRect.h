#pragma once

#include "tlBase.h"

class AFX_EXT_CLASS CTlRect : public CTlBase
{
public:
	CTlRect(CChildView* view, CAmDraw* draw, CFormItem* formItem);
	virtual ~CTlRect();

protected:
	void		drawBegin(CPoint point);
	void		drawTo(CPoint point);
	virtual void 	drawEnd();
};


