#ifndef __AXMSHADE_H__
#define __AXMSHADE_H__

#include "AxMDib.h"

class AFX_EXT_CLASS CAxMShade 
{
public:
	CAxMShade();
	virtual ~CAxMShade();

	enum {
		SH_NOISE = 0,
		SH_DIAGSHADE,
		SH_HSHADE,
		SH_VSHADE,
		SH_HBUMP,
		SH_VBUMP,
		SH_SOFTBUMP,
		SH_HARDBUMP,
		SH_METAL
	};
private:
	CAxMDib m_normal, m_down, m_disabled, m_over, m_h, m_v;
public:
	void DrawShade(CDC* dc, CRect rect, UINT shadeID = 0, BYTE granularity = 8, BYTE highlight = 10, BYTE coloring = 0, COLORREF color = 0, BOOL down = FALSE, BOOL over = FALSE);
	void DrawShade(CDC* dc, CRect rect, UINT shadeID, COLORREF color, UINT percent = 50) {
		DrawShade(dc, rect, shadeID, 8, 10, percent, color, FALSE, FALSE);
	}
};


#endif // __AXMSHADE_H__