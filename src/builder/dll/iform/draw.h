#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

class CaxImage
{
public:
	CaxImage();
	virtual	~CaxImage();

public:
	time_t	m_mtime;
	enum imgType { iNONE = 0, iBMP, iPNG } m_type;
	void*	m_image;

private:
	IPicture*	m_picture;

public:
	bool	Load(CString file);

protected:
	bool	getHandle(HBITMAP& bitmap);
};

class CAxDraw
{
public:
	CAxDraw(void);
	virtual ~CAxDraw(void);

public:
	CFont*	SetFont(CString fontN, int point, bool italic, int bold);
	CPen*	SetPen(COLORREF crRGB, int width = 1, int style = PS_SOLID);
	CBrush*	SetBrush(COLORREF crRGB);
	void*	SetImage(CString fileN);

private:
	void	removeFont();
	void	removePen();
	void	removeBrush();
	void	removeImage();

protected:
	CMapStringToOb	m_penObs;
	CMap	< COLORREF, COLORREF, CBrush *, CBrush * > m_brushObs;
	CMapStringToOb	m_fontObs;
	CMapStringToPtr	m_imgObs;
};

