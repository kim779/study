#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlBox : public CTlRect
{
public:
	CTlBox(class CChildView* view, CAmDraw* draw, CFormItem* formItem);
	virtual ~CTlBox();

	void	Draw(CDC* dc, int fN);

protected:
	void	drawEnd();
	void	draw3DBox(CDC* dc, int fN);
	void	draw2CBox(CDC *dc, int fN);
};


