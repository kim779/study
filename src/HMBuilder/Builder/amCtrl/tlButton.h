#pragma once

#include "tlRect.h"

class AFX_EXT_CLASS CTlButton : public CTlRect
{
public:
	CTlButton(class CChildView* view, CAmDraw* draw, CFormItem* formItem, UINT type);
	virtual ~CTlButton();

	void	Draw(CDC*dc, int fN);

protected:
	class	CTlImage*	m_image;
	enum	{ bnNONE=0, bnRADIO, bnCHECK, bnPUSH }	m_type;

	void	drawEnd();
};


