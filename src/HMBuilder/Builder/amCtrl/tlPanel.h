#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlPanel : public CTlRect
{
public:
	CTlPanel(class CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlPanel();

	void	Draw(CDC* dc, int fN);

protected:
	class	CTlImage*	m_image;
	enum	{ pnNONE=0, pnIMAGE, pnGROUP }	m_type;

	void	drawEnd();
};


