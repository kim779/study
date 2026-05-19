#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlText : public CTlRect
{
public:
	CTlText(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlText();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ txNONE=0, txLABEL }	m_type;
	void	drawEnd();
};


