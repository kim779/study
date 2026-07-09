#if !defined(AFX_TOOL_H__6FB9C5EB_4E6B_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_TOOL_H__6FB9C5EB_4E6B_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tool.h : header file
//

const	COLORREF clBLACK   = PALETTERGB(  0,   0,   0);
const	COLORREF clDBLUE   = PALETTERGB(  0,   0, 128);
const	COLORREF clBASIC   = PALETTERGB(192, 192, 192);
const	COLORREF clGREEN   = PALETTERGB(  0, 102,   0);
const	COLORREF clDGRAY   = PALETTERGB( 34,  34,  34);
const	COLORREF clBOTTOM  = PALETTERGB( 64,  64,  64);
const	COLORREF clEDGE    = PALETTERGB(128, 128, 128);
const	COLORREF clSPLIT   = PALETTERGB(230, 230, 230);
const	COLORREF clLGRAY   = PALETTERGB(234, 234, 234);
const	COLORREF clYWHITE  = PALETTERGB(241, 241, 229);
const	COLORREF clWHITE   = PALETTERGB(255, 255, 255);
const	COLORREF clSORTD   = PALETTERGB(160, 160, 160);
const	COLORREF clSORTL   = PALETTERGB(238, 238, 238);

#define	EDITFACE	"Edit"
#define	COMBOFACE	"ComboBox"
#define	MEMOFACE	"Memo"
#define	GROUPFACE	"Group"
#define	RADIOFACE	"Radio"
#define	CHECKFACE	"Check"
#define	LABELFACE	"Label"
#define	OUTPUTFACE	"Output"
#define	COLUMNFACE	"Column"

#define	szSCROLL	16

#define	dmDATA		0
#define	dmSYMBOL	1
#define	dmNONE		2

/////////////////////////////////////////////////////////////////////////////
// CTOOL Object

#include "../awCommon/FormItem.h"

class AFX_EXT_CLASS CTOOL : public CObject
{
	DECLARE_DYNCREATE(CTOOL)

	CTOOL()	{}
	CTOOL(class CChildView* view);
	virtual ~CTOOL();

// Attributes
public:
	CString		m_sTabDir;
	CString		m_name;
	int		m_delta;
	int		*m_pDisplayMode;

protected:
	CChildView*	m_view;
	CPoint		m_lastpt, m_toPt;
	CPoint		m_endpt;
	CRect		m_bound;
	CRect		m_rect;
public:
	int			m_fN;	// form index
	int			m_saveDC;
	CDC*			m_dc;
	CBrush*			m_brush;
	class	CAxisDraw*	m_draw;
	CFormItem	*m_pFormItem;

	int		m_fheight;
	int		m_fwidth;

// Operations
public:
	COLORREF getDefaultBRGB(CString sKind, int nDefault);
	COLORREF getDefaultTRGB(CString sKind, int nDefault);
	COLORREF getDefaultPRGB(CString sKind, int nDefault);
	int	getDefaultHeight(CString sKind, int nDefault);
	int	getDefaultWidth(CString sKind, int nDefault);

	void	setBoldFont(CDC* dc, bool isBold);
	CString Parser(CString &srcstr, CString substr);
	CString convertVString(CString str);
	bool	IsInPage(int idx);
	void	MouseDown(CPoint point);
	void	MouseMove(CPoint point);
	void	MouseUp();
	void	ReleaseBlock();

	virtual	void	Draw(CDC* dc, int fN)			{ m_fN = fN; }
	virtual	int	Movable(CPoint point, CRect& rc)	{ return -1; }	// table
	virtual	bool	OptimumSize(CPoint point)		{ return false; }
	virtual	void	MouseDrag(){}

protected:
	virtual void 	drawBegin(CPoint point)	{}
	virtual void 	drawTo(CPoint point)	{}
	virtual void 	drawEnd()		{}
	CRect		getRect(int fN = -1);

	COLORREF	getRGB(int color);
	void		setPen(CDC* dc, COLORREF rgb, int width=1);
	void		setPen(CDC* dc, int width=1);
	CBrush*		setBrush(CDC* dc, COLORREF rgb);
	CBrush*		setBrush(CDC* dc);
	void		setFont(CDC* dc);
	void		setTextColor(CDC* dc, COLORREF rgb);
	void		setTextColor(CDC* dc);
	void		setBkColor(CDC* dc, COLORREF rgb);
	void		setBkColor(CDC* dc);

	void		getPath(CString &path);
	void		drawBMP(CDC* dc, CRect rc, CString txt ="IMAGE");
	void		editAmpersand(CString& str);
	void		editComma(CString& str, int len);
	void		setInOutFace(CString& str, int len, int type);
	void		drawEdge(CDC* dc, CRect rect, UINT edge, UINT flags=BF_RECT);
	CRect		GroupRect(int nGroup);
	CRect		getFormRect(int idx, int flag = -1);
};

class AFX_EXT_CLASS CtlRect : public CTOOL
{
public:
	CtlRect(CChildView* view);
	virtual ~CtlRect();
	virtual	int	Movable(CPoint point, CRect& rc)	{ return -1; }

protected:
	void		drawBegin(CPoint point);
	void		drawTo(CPoint point);
	virtual void 	drawEnd();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOL_H__6FB9C5EB_4E6B_11D4_A024_00001CD7F9BE__INCLUDED_)
