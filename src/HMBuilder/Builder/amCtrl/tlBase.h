#pragma once

#include "../h/mapform.h"
#include "../ChildView.h"

const	COLORREF clBLACK   = PALETTERGB(  0,   0,   0);
const	COLORREF clBASIC   = PALETTERGB(192, 192, 192);
const	COLORREF clGREEN   = PALETTERGB(  0, 102,   0);
const	COLORREF clDGRAY   = PALETTERGB( 34,  34,  34);
const	COLORREF clBOTTOM  = PALETTERGB( 64,  64,  64);
const	COLORREF clEDGE    = PALETTERGB(128, 128, 128);
const	COLORREF clLGRAY   = PALETTERGB(234, 234, 234);
const	COLORREF clYWHITE  = PALETTERGB(241, 241, 229);
const	COLORREF clWHITE   = PALETTERGB(255, 255, 255);
const	COLORREF clSORTD   = PALETTERGB(160, 160, 160);
const	COLORREF clSORTL   = PALETTERGB(238, 238, 238);

#define	EDITFACE	_T("Edit")
#define	COMBOFACE	_T("ComboBox")
#define	MEMOFACE	_T("Memo")
#define	GROUPFACE	_T("Group")
#define	RADIOFACE	_T("Radio")
#define	CHECKFACE	_T("Check")
#define	LABELFACE	_T("Label")
#define	OUTPUTFACE	_T("Output")
#define	COLUMNFACE	_T("column")

#define	szSCROLL	16

#define	dmDATA		0
#define	dmSYMBOL	1
#define	dmNONE		2

class AFX_EXT_CLASS CTlBase : public CObject
{
public:
	CTlBase()	{}
	CTlBase(CChildView* view, class CAmDraw* draw, class CFormItem* formItem);
	virtual ~CTlBase();

public:
	CString	m_name;
	int	m_fN;	// form index, change always
	CDC*	m_dc;
	CRect	m_rect;
protected:
	int	m_delta;
	int	m_display;
	CString	m_root;

	CChildView* m_view;
	CPoint	m_lastpt, m_toPt;
	CPoint	m_endpt;
	CRect	m_bound;


	int	m_saveDC;
	class	CAmDraw*	m_draw;
	CFormItem*	m_formItem;

	int	m_fheight;
	int	m_fwidth;

public:
	void	SetIndex(int fN, CDC* dc)			{ m_fN = fN; m_dc = dc; }
	void	MouseDown(CPoint point);
	void	MouseMove(CPoint point);
	void	MouseUp();
	void	ReleaseBlock();
	void	DefaultLayout();

	virtual	void	Draw(CDC* dc, int fN)			{ m_fN = fN; }
	virtual	bool	OptimumSize(CPoint point)		{ return false; }
	virtual	void	MouseDrag(){}

protected:
	virtual void 	drawBegin(CPoint point)	{}
	virtual void 	drawTo(CPoint point)	{}
	virtual void 	drawEnd()		{}
	CRect		getRect(int fN = -1);

	COLORREF getRGB(int color);
	void	setPen(CDC* dc, COLORREF rgb, int width = 1);
	void	setPen(CDC* dc, int width=1);
	CBrush*	setBrush(CDC* dc, COLORREF rgb);
	CBrush*	setBrush(CDC* dc);
	void	setFont(CDC* dc);
	void	setCellFont(CDC* dc, struct _cellR* cellR);
	void	setTextColor(CDC* dc, COLORREF rgb);
	void	setTextColor(CDC* dc);
	void	setBkColor(CDC* dc, COLORREF rgb);
	void	setBkColor(CDC* dc);

	void	getPath(CString &path);
	CString	getRoot();
	void	drawBMP(CDC* dc, CRect rc, CString txt = _T("IMAGE"));
	void	drawPNG(CDC* dc, CRect rc, CString txt);
	void	editAmpersand(CString& str);
	void	editComma(CString& str, int len);
	void	setInOutFace(CString& str, int len, int type);
	void	drawEdge(CDC* dc, CRect rect, UINT edge, UINT flags = BF_RECT);
	CRect	GroupRect(int nGroup);
	CRect	getFormRect(int idx, int flag = -1);

	COLORREF getDefaultBRGB(CString sKind, int nDefault);
	COLORREF getDefaultTRGB(CString sKind, int nDefault);
	COLORREF getDefaultPRGB(CString sKind, int nDefault);
	int	getDefaultHeight(CString sKind, int nDefault);
	int	getDefaultWidth(CString sKind, int nDefault);

	void	setBoldFont(CDC* dc, bool isBold);
	CString parse(CString &srcstr, CString substr);
	CString convertVString(CString str);
};