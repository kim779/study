#if !defined(AFX_FMBASE_H__DD37D7B2_967F_4986_A7C4_FD3BBCDBE68F__INCLUDED_)
#define AFX_FMBASE_H__DD37D7B2_967F_4986_A7C4_FD3BBCDBE68F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ambase.h : header file
//

#include "StdAfx.h"
#include "../../h/form_w.h"
//#include "../../h/form.h"


// normal state
#define	RC_NOP		-1
#define	RC_OK		-2

// edit state
#define	RC_NODELETE	-3	// There is no data to delete
#define	RC_FULL		-4	// text full
#define	RC_OVERFLOW	-5	// text overflow
#define	RC_DENY		-6

// mouse state
#define	RC_ITEM		-7	// item click
#define	RC_HEAD		-8	// scroll head click
#define	RC_THUMB	-9	// vertical scroll thumb click
#define	RC_SCRUP	-10	// exUP
#define	RC_SCRDN	-11	// exDOWN
#define	RC_UPM		-12	// scroll up click
#define	RC_DN		-13	// scroll down click
#define	RC_PAGEUP	-14	// page exUP click
#define	RC_PAGEDN	-15	// page exDOWN click
#define	RC_LEFT		-16	// scroll left click
#define	RC_RIGHT	-17	// scroll right	click
#define	RC_RGNUP	-18	// scroll up region click
#define	RC_RGNDN	-19	// scroll down region click
#define	RC_RGNLEFT	-20	// scroll left region click
#define	RC_RGNRIGHT	-21	// scroll right region click
#define	RC_OUT		-22	// outside click
#define	RC_FIRST	-23	// page first click

#define	RC_CBCLOSE	-24	// combo folded
#define	RC_CBSPREAD	-25	// combo spreaded

#define	RC_SHIFT	-26	// SHIFT key State
#define	RC_CONTROL	-27	// CONTROL key state
#define	RC_CTRLSHIFT	-28	// SHIFT + CONTROL key state
#define	RC_SEND		-29	// send data
#define	RC_FOLDER	-30	// folder

#define	RC_HOVER	-31	// hover image
#define	RC_CHANGED	-32	// data changed
#define	RC_CODE		-33	// edit code click			// 삭제
#define	RC_SEARCH	-34	// edit search mode			// 삭제
#define	RC_UPDATE	-35	// grid cell update

#define	RC_SPIN		-36	// edit spin click
#define	RC_IN		-37	// puse in (on MouseOver)

// point in rect
#define	P_IN		-1
#define	P_OUT		0

// values
#define	WHEEL_AMOUNT	3
#define	MAX_COMBO_LINE	8

#define	userCOLOR	0x02000000
#define	offsRTS		3
#define	szSCROLL	16
#define	szYYYYMMDD	8
#define	szYYMMDD	6

enum	wch_t	{ WC_AN, WC_H1, WC_H2, WC_SO, WC_SI, WC_NC };

#define	ct_uLimit	4
#define	ct_Rise		3
#define	ct_Fall		2
#define	ct_lLimit	1
#define	ct_Flat		0

#define	USR_ROOT	"ROOT"
#define	LAST_ITEM	"LAST_ITEM"

#define	ET_ITEM		0
#define	ET_TAB		1
#define	ET_USER		2

// color
const	int	cBLACK	= 255;		// RGB(  0,   0,   0)
const	int	cDBLUE	= 230;		// RGB(  0,   0, 128)
const	int	cBASIC	= 208;		// RGB(192, 192, 192)
const	int	cSPLIT	= 243;		// RGB(204, 204, 204)
const	int	cGRAY	= 248;		// RGB(119, 119, 119)
const	int	cBOTTOM	= 251;		// RGB( 64,  64,  64)
const	int	cXFG	= 248;		// RGB(119, 119, 119)
const	int	cXBG	= 243;		// RGB(203, 203, 203)
const	int	cLGRAY	= 241;		// RGB(234, 234, 234)
const	int	cWHITE	= 240;		// RGB(255, 255, 255)
const	int	cSORTD	= 245;		// RGB(160, 160, 160)
const	int	cSORTL	= 241;		// RGB(238, 238, 238)
const	int	cEDGE	= 216;		// RGB(127, 127, 127)
const	int	cYWHITE	= 240;		// RGB(241, 241, 229)

const	int	cRGB_UP = 209;		// CORGB : +
const	int	cRGB_DN = 214;		// CORGB : -

const	int	cCB_LINE = 100;		// edit/combo rectagnle color



const	COLORREF clBLACK   = _ARGB(255,  0,   0,   0);
const   COLORREF clDBLUE   = _ARGB(255,   0,   0, 128);
const	COLORREF clBASIC   = _ARGB(255, 192, 192, 192);
const	COLORREF clSPLIT   = _ARGB(255, 204, 204, 204);
const	COLORREF clBOTTOM  = _ARGB(255, 64,  64,  64);
const   COLORREF clGRAY	   = _ARGB(255, 119, 119, 119);
const   COLORREF clXBG	   = _ARGB(255, 203, 203, 203);
const	COLORREF clLGRAY   = _ARGB(255, 234, 234, 234);
const	COLORREF clWHITE   = _ARGB(255,255, 255, 255);
const	COLORREF clSORTD   = _ARGB(255,160, 160, 160);
const	COLORREF clSORTL   = _ARGB(255,238, 238, 238);
const	COLORREF clEDGE    = _ARGB(255,127, 127, 127);
const	COLORREF clYWHITE  = _ARGB(255,241, 241, 229);
const	COLORREF clGREEN   = _ARGB(255,  0, 102,   0);
const	COLORREF clDGRAY   = _ARGB(255, 34,  34,  34);



#define EDGE_LINE 7



/////////////////////////////////////////////////////////////////////////////
// CamBase command target

class AFX_EXT_CLASS CamBase : public CCmdTarget
{
	DECLARE_DYNCREATE(CamBase)

public:
	CamBase();           // protected constructor used by dynamic creation
	CamBase(class CAmForm* axform, struct _formR* formR);
	virtual	~CamBase();

// Attributes
public:
	struct		_formR*	m_form;
	CAmForm*	m_axform;
	int			m_itRGB;	// tRGB index of palette
	string		m_symbol;

protected:
	CRect		m_rect;		// form rect
	CRect		m_pRc;		// rect : drawing


	ARGB	m_pRGB;
	ARGB	m_tRGB;
	ARGB	m_bRGB;
	ARGB	m_sRGB;				// select RGB

	int			m_fPoint;
	int			m_cfPoint;

	float		m_fHeight;
	float		m_fWidth;

	bool		m_focus;

	CSize		m_size;		// map scroll size
	float		m_hR;
	float		m_vR;

	CString		m_fonts;
	CString		m_strR;
	CString		m_strRawData;
	CString		m_strRTmp;	// CString 연산 후 메모리 휘발 방지
	int			m_ndc;

	CString		m_name;
	int			m_rts;

// Operations
public:
	virtual	CString	GetSymbolName(int col = -1)			{ return m_name; }
	virtual	int		GetName(int col = -1)				{ return m_rts; }
	virtual	bool	IsValid(int& guide);
	virtual	int		GetLength()					{ return 0; }

	virtual	CRect	GetRect(bool original = false);
	virtual	void	SetRect(CRect rect, CRect pRc);
	virtual	void	Adjust(CSize size);
	virtual	void	Adjust(CPoint pt, float hR, float vR);
	virtual	void	Offset(CSize offs);
	virtual	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	virtual	void	Reload();
	virtual	int		IsPointInRect(CPoint pt);

	virtual	void	SetFont(int point, int style, CString fonts, bool resize);
	virtual void	Draw(CDC* dc)					{}

	virtual void	OnLButton(bool down, CPoint pt, int& result)	{}
	virtual void	OnMButton(bool down, CPoint pt, int& result)	{}
	virtual void	OnRButton(bool down, CPoint pt, int& result)	{}
	virtual	void	OnDblClick(CPoint pt, int& result)		{}
	virtual	bool	OnRepeat(CPoint pt, int& result)		{ return true; }
	virtual	bool	OnMouseWheel(int delta, int& result)		{ return false; }
	virtual	int		OnDrag(CPoint pt, int& result);
	virtual	bool	OnDrop(CPoint pt, CString data, CString& key)	{ return false; }
	virtual int		OnKey(int key, int& result);

	virtual	void	SetFocus(bool focus)		{ m_focus = focus; }
	virtual	void	SetVisible(bool visible, int col = -1);
	virtual	bool	IsVisible(int col = -1);
	virtual	void	SetEnable(bool enable);
	virtual	bool	IsEnable();
	virtual	void	SetAttributes(DWORD attr, bool set = true, int col = -1, int row = -1);
	
	virtual	void	SetFgColor(int rgb, int col = -1, int row = -1);
	virtual void	SetBkColor(int rgb, int col = -1, int row = -1);
	virtual	int  	GetFgColor(int col = -1, int row = -1);
	virtual int 	GetBkColor(int col = -1, int row = -1);
	virtual	void	SetDomino(bool redraw = true)			{}

	virtual	void	ReadAll(CString& data, bool edit = false, bool head = false);
	virtual void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
						// row == -1 : HEAD

	virtual	void	WriteAll(CString str, bool redraw = true);
	virtual	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	virtual void	ClearData(bool selectOnly = false)		{}

	virtual	void	UpdateData(int key, bool moving, int& result)	{}
	virtual	void	InsertData(int key, bool moving, int& result)	{}

	virtual	int	    GetSize()								{ return 0; }
	virtual bool	IsChanged(bool reset = true)			{ return true; }

	virtual	void	SetCurSel(int index, bool redraw = true)	{}
	virtual	int		GetCurSel()					{ return 0; }

	virtual	void	GetEnum(CString& text, int& value, int func = -1)	{}
	virtual	void	SetEnum(CString text, int value, int func = -1)		{}

	virtual	void	Refresh();
	virtual	void	DoListBox(bool apply, bool focus = false)	{}
	virtual bool	IsFocus(HWND hWnd)				{ return false; }
	virtual	void	ShowControl(BOOL visible)			{}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamBase)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDC*		getDC();
	void		releaseDC(CDC* dc);
	COLORREF	getRGB(int index);
	int			getINDEX(COLORREF rgb);
	CRect		calcRect(CRect rect, float hR, float vR);
	void		setFontSize(float hR = 1.0, float vR = 1.0);
	CFont*		setFont(CDC* dc, int state = 0);
	CFont*		setCellFont(CDC* dc, int resolution, struct _cellR* cellR);

	void		setTextColor(CDC* dc, COLORREF rgb);
	void		setTextColor(CDC* dc);
	void		setPaintColor(CDC* dc, COLORREF rgb);
	void		setPaintColor(CDC* dc);
	void		setPen(CDC* dc, COLORREF rgb, int width = 1, int style = PS_SOLID);
	void		setPen(CDC* dc, int width = 1, int style = PS_SOLID);
	CBrush*		setBrush(CDC* dc, COLORREF rgb);
	CBrush*		setBrush(CDC* dc);
	
	virtual	void	keyDelete(int& result)		{ result = RC_NOP; }
	virtual	void	keyBackspace(int& result)	{ result = RC_NOP; }
	virtual	int	keyArrowLeft(int& result)	{ result = RC_NOP; return 0; }
	virtual	void	keyArrowRight(int& result)	{ result = RC_NOP; }
	virtual	void	keyArrowUp(int& result)		{ result = RC_NOP; }
	virtual	void	keyArrowDown(int& result)	{ result = RC_NOP; }
	virtual	void	keyReturn(int& result)		{ result = RC_NOP; }
	virtual	void	keyEscape(int& result)		{ result = RC_NOP; }
	virtual	void	keyPrior(int& result)		{ result = RC_NOP; }
	virtual	void	keyNext(int& result)		{ result = RC_NOP; }
	virtual	void	keyHome(int& result)		{ result = RC_NOP; }
	virtual	void	keyEnd(int& result)		{ result = RC_NOP; }
	virtual	void	keyTab(int& result)		{ result = RC_NOP; }

	void		setBkColor(int rgb);
	void		invalidateRect(LPCRECT lpRect, bool erase, bool force = false);
	void		setCaretPos(CPoint pt);
	void		doEditCORGB(CString& data);
	int		    getCORGB(DWORD coRGB, CString& data, int flen, int supress);
	void		makeData(CString& data, int flen, DWORD attr, DWORD attr2, int type);
	void		makeRawData(CString& data);
	void		makeSignData(CString& data, int col = -1, int row = -1);
	bool		checkKey(int& key, BYTE type, DWORD attr);
	COLORREF	getReverseColor(COLORREF rgb);

	wch_t		whichHangulEdit(int pos, CString str);
	int		    getSIGN(CString str, int& offs);
	void		drawPolygonText(CDC* dc, CRect rect, CString str, BYTE align, BYTE type);
	void		drawEdge(Graphics& g, CRect rect, UINT edge, UINT flags);
	void		drawEdge(CDC *dc, CRect rect, UINT edge, UINT flags);
	CString		convertVString(CString src);
	void		formatData(int attr, CString& data, int nfloat);


	// 2013-05-14 kjs Add Function
	void	drawGRoundRect(Graphics* g, SolidBrush* brush, CRect rc, int radius = 3);
	void	drawG3DRect(Graphics* g, CRect rect, ARGB clUp, ARGB clDn);
	Bitmap*	DrawFrameControl(CDC* dc, CRect rc, UINT nType, UINT nState);
	Pen*	setgPen(ARGB argb, int width = 1);
	Pen*    setgPen(int width = 1);
	Font*	setgFont(int state = 0);
	SolidBrush* setgBrush(ARGB crARGB);
	SolidBrush* setgBrush();
	SolidBrush* setgFBrush();
	ARGB		getgRGB(int index);
	int			getgINDEX(COLORREF rgb);
	ARGB		getgReverseColor(ARGB rgb);
	//bool		ARGBtoRGB(DWORD argb);
	COLORREF	ARGBtoRGB(DWORD argb);	

	// Generated message map functions
	//{{AFX_MSG(CamBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamBase)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

class Cpos
{
public:
	Cpos()	{};
	Cpos(int row, int col)	{ m_row = row; m_col = col; }

public:
	int	m_row;
	int	m_col;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMBASE_H__DD37D7B2_967F_4986_A7C4_FD3BBCDBE68F__INCLUDED_)
