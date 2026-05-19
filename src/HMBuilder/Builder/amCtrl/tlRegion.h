#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlRegion : public CTlRect
{
public:
	CTlRegion(CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlRegion();

	void	Draw(CDC* dc, int fN);

protected:
	enum	{ rnNONE=0, rnOBJECT, rnBROWSER }	m_type;
	void	drawEnd();
};


