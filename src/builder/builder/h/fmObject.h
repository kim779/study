#pragma once
//#include "../iform/iForm.h"
//#include "../../h/axisevent.h"
//#include "../../h/fmProperties.h"
//#include "../../h/axisvar.h"
//#include "../../h/axisColor.h"

// values
#define	userCOLOR	0x02000000
#define	szSCROLL	16
#define	szSPIN		16
#define	szCALENDAR	19
#define	szYYYYMMDD	8
#define	szYYMMDD	6

// color
const	int	cBLACK	= 255;		// RGB(  0,   0,   0)
const	int	cDBLUE	= 230;		// RGB(  0,   0, 128)
const	int	cBASIC	= 208;		// RGB(192, 192, 192)
const	int	cGRAY	= 248;		// RGB(119, 119, 119)
const	int	cBOTTOM	= 251;		// RGB( 64,  64,  64)
const	int	cLGRAY	= 241;		// RGB(234, 234, 234)
const	int	cWHITE	= 240;		// RGB(255, 255, 255)
const	int	cEDGE	= 216;		// RGB(127, 127, 127)
const	int	cYWHITE	= 240;		// RGB(241, 241, 229)

const	int	cRGB_UP = 209;		// CORGB : +
const	int	cRGB_DN = 214;		// CORGB : -

// 
enum	wch_t	{ WC_AN, WC_H1, WC_H2, WC_SO, WC_SI, WC_NC };

#define CLR_DATABACK		1		// 배경
#define CLR_DATATEXT            2		// 표준 텍스트
#define CLR_DATACONUP           17		// 상승(▲)텍스트
#define CLR_DATACONDOWN         18		// 하락(▲)텍스트
#define CLR_DATACONSTAY         19		// 보합텍스트
#define CLR_DATACONMAX          17		// 상한(▲)텍스트
#define CLR_DATACONMIN          18		// 하한(▲)텍스트

// to parent message
// event ID
#define	OE_CLICK		1
#define	OE_DOUBLECLICK		2
#define	OE_CHANGE		3
#define	OE_CHARCHANGE		4
#define	OE_KEY			5
#define	OE_FOCUS		6
#define	OE_MOUSEENTER		7
#define	OE_MOUSELEAVE		8

#define AXMESSAGE(t)	m_view->SendMessage(WM_USER, (LONG)this, (LONG)t)

// CfmObject 명령 대상입니다.
class AFX_EXT_CLASS CfmObject : public CObject
{
public:
	CfmObject(){};
	CfmObject(class CfmProperties* prop, class CiForm* ifm, CWnd* view);
	virtual ~CfmObject();

// Attributes
public:
	class CfmProperties*	m_prop;
	class CiForm*		m_ifm;
	CWnd*			m_view;

protected:
	CRect		m_rect;		// orginal rect
	CRect		m_pRc;		// drawing rect

	COLORREF	m_tRGB;		// text color
	COLORREF	m_pRGB;		// paint color
	COLORREF	m_bRGB;		// border color

	int		m_itRGB;	// tRGB index of palette
	
	WORD		m_cfPoint;	// font point (drawing)
	WORD		m_fPoint;	// font point (backup)
	CString		m_fonts;	// font name

	float		m_fHeight;
	float		m_fWidth;

	bool		m_focus;

	CSize		m_size;		// map scroll size
	float		m_hR;
	float		m_vR;

	int		m_ndc;
	int		m_rts;
	bool		m_bReload;

// Operations
public:
	virtual	CString	GetName(int col = -1);		// 객체명
	virtual	int	GetRtsName(int col = -1);	// rts 심볼명
	virtual	int	GetIndexByName(CString name) { return 0; }	// for grid

	virtual	CRect	GetRect(bool original = false);
	virtual	void	SetRect(CRect rect, CRect pRc);
	virtual	void	Adjust(CSize size);
	virtual	void	Adjust(CPoint pt, float hR, float vR);
	virtual	void	Offset(CSize offs);
	virtual	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	virtual	void	Reload();

	virtual	void	SetFont(int point, int style, CString fonts, bool resize);

	virtual	void	SetFocus(bool focus)		{ m_focus = focus; }
	virtual	void	SetVisible(bool visible, int col = -1);
	virtual	bool	IsVisible(int col = -1);
	virtual	void	SetEnable(bool enable);
	virtual	bool	IsEnable();
	
	virtual	void	SetFgColor(int rgb, int col = -1, int row = -1);
	virtual void	SetBkColor(int rgb, int col = -1, int row = -1);
	virtual	int	GetFgColor(int col = -1, int row = -1);
	virtual int	GetBkColor(int col = -1, int row = -1);
	virtual	void	SetDomino(bool redraw = true)			{}

	virtual	void	ReadAll(CString& data, bool edit = false, bool head = false) {}
	virtual	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1){}	// row == -1 : HEAD
	virtual	CString	GetHeader() { return _T(""); }
	virtual	int	GetHeaderLength() { return 0; }
	virtual	void	SetHeader(CString data) {}

	virtual	void	WriteAll(CString str, bool redraw = true) {}
	virtual	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1) {}
	virtual void	ClearData(bool selectOnly = false)		{}

	virtual	int	GetRowCount()					{ return 0; }
	virtual	int	GetValidRowCount()				{ return 0; }
	virtual int	GetColumnCount()				{ return 0; }

	virtual	void	InsertRow(int index, CString data) {}
	virtual	void	RemoveRow(int index) {}

	virtual bool	IsChanged(bool reset = true)			{ return true; }

	virtual	void	SetCurSel(int index, bool redraw = true)	{}
	virtual	int	GetCurSel()					{ return 0; }

	virtual	void	GetEnum(CString& text, int& value, int func = -1)	{}	// 추후사용 용도
	virtual	void	SetEnum(CString text, int value, int func = -1)		{}	// 추후사용 용도

	virtual	void	Refresh();
	virtual	void	Blink(bool set, int rgb = 0) {}
	virtual	void	Blink(bool set, int row, int col, int rgb = 0) {}

	virtual	void	SetLayout(CRect rect, bool visible) {}
	virtual LPDISPATCH GetDispatch(){ return nullptr; };

	virtual void	SetChecked(bool checked){}			// RadioButton, CheckBox check/uncheck
	virtual void	InsertRows(int index, CString str){}		// Grid, GridEx InsertRows
	virtual void	SetCellItems(int row, int col, CString items){}	// Grid, GridEx set cell combo-data
	virtual int	GetCellRow(){ return 0; }			// Grid, GridEx get selected row
	virtual int	GetCellColumn(){ return 0; }			// Grid, GridEx get selected column
	virtual CWnd*   GetWnd(){ return nullptr; }
	virtual CString   GetProperties(){ return ""; }
	virtual CString   GetMethod(){ return ""; }
	virtual CString   GetEvent(){ return ""; }
	virtual LONG    receiveBuilder(int kind, WPARAM wParam, LPARAM lParam){ return 0; }

	LONG		Variant(int type, char* fmt, ...);
	LONG		AxInfo(int type, char* fmt, ...);
	void		AxEvent(int type);


private:
	CDC*	getDC();
	void	releaseDC(CDC* dc);

protected:
	virtual void	drawObject(CRect rect, BOOL erase) {};
	virtual long	toViewEvent(long eventID, void* eventVal);

protected:
	void		invalidateRect(LPCRECT lpRect, bool erase, bool force = false);
	COLORREF	getRGB(int index);
	int		getINDEX(COLORREF rgb);
	CRect		calcRect(CRect rect, float hR, float vR);
	void		setFontSize(CDC* dc = NULL, float hR = 1.0, float vR = 1.0, bool original = false);

	CFont*		setFont(CDC* dc, int state = 0);

	void		setTextColor(CDC* dc, COLORREF rgb);
	void		setTextColor(CDC* dc);
	void		setPaintColor(CDC* dc, COLORREF rgb);
	void		setPaintColor(CDC* dc);
	void		setPen(CDC* dc, COLORREF rgb, int width = 1, int style = PS_SOLID);
	void		setPen(CDC* dc, int width = 1, int style = PS_SOLID);
	CBrush*		setBrush(CDC* dc, COLORREF rgb);
	CBrush*		setBrush(CDC* dc);
	void		setBkColor(int rgb);

	int		getCORGB(CString& data, int flen, int supress);
	void		makeData(CString& data, int flen, DWORD attr, DWORD attr2, int type);
	void		makeSignData(CString& data, int col = -1, int row = -1);
	bool		checkKey(int& key, BYTE type, DWORD attr);
	COLORREF	getReverseColor(COLORREF rgb);

	wch_t		whichHangulEdit(int pos, CString str);
	int		getSIGN(CString str, int& offs);
	void		drawBorder(CDC* dc, CRect rect, COLORREF clr, bool stress = false);
	void		drawPolygonText(CDC* dc, CRect rect, CString str, BYTE align, BYTE type, COLORREF tRGB, COLORREF pRGB);
	void		drawEdge(CDC* dc, CRect rect, UINT edge);
	void		drawEdge(CDC* dc, CRect rect, COLORREF clr, UINT flags);
	void		drawGradation(CDC* dc, CRect tRc, COLORREF st_clr, COLORREF end_clr, bool vertical = true);

	CString		convertVString(CString src);
	void		formatData(int attr, CString& data, int nfloat, int size);
	void		transparentBlt(CDC *pDC, CBitmap *pBitmap, int x, int y, int cx, int cy);
	void		changeIME(bool ime);

	void		screenTOclient(CRect& screen, CRect& client);



};

class Cpos
{
public:
	Cpos()	{};
	Cpos(int row, int col)	{ m_row = row; m_col = col; }

public:
	bool	Equal(Cpos pos)
	{
		if (m_row != -1 && m_col != -1 && m_row == pos.m_row && m_col == pos.m_col)
			return true;
		return false;
	}

	bool	IsEmpty()
	{
		if (m_row == -1 && m_col == -1)
			return true;
		return false;
	}

public:
	int	m_row;
	int	m_col;
};
