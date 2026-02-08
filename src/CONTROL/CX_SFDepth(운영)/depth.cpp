// depth.cpp : implementation file
//

#include "stdafx.h"
#include "CX_SFDepth.h"
#include "depth.h"

#include <math.h>
#include "keys.hxx"
#include "tooltip.h"
#include "ConfigDlg.h"
#include "ConfigDlgL.h"
#include "ConfigDlgR.h"
#include "ContractWnd.h"
#include "MBongWnd.h"
#include "../../h/form.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	HALFLINE	50
#define RATERECTSEL(a)	a.left + (a.Width() * HALFLINE) / 100
#define RATERECTBUY(a)	a.left + (a.Width() * (100-HALFLINE)) / 100
#define P_RATIO		0.60
#define	idDEPTH		10235
#define	symCODE		"ed_focod"
#define	symECN		"81301"

#define menuBase	10000
#define bongWIDTH	10

#define FILENAME	"sfhoga.env"

const int	imgCX1 = 15, imgCY1 = 15;
/////////////////////////////////////////////////////////////////////////////
// Cdepth

Cdepth::Cdepth()
{
	EnableAutomation();

	m_class  = AfxRegisterWndClass(0);
	m_parent = NULL;

	m_rect.SetRectEmpty();
	m_options = _T("");
	m_refsym  = _T("");

	m_font  = NULL;
	m_fonts = _T("굴림");
	m_point = 9;
	m_style = 0;

	m_code  = _T("");
	m_items.RemoveAll();
	m_indices.RemoveAll();

	for (int ii = 0; ii < 3; ii++)
		m_columns[ii] = 0;
	m_rowH = 18;

	m_tracking = FALSE;
	m_focus    = -1;
	m_maxsize  = 0;
	m_prect.SetRectEmpty();
	m_rcConfig.SetRectEmpty();
	m_rcConfigL.SetRectEmpty();
	m_rcConfigR.SetRectEmpty();

	m_curr = -1;
	m_clos = -1;

	m_depth          = 10;
	m_type           = ctNo;
	m_showBeforeDiff = false;
	m_bar            = grNo;
	m_showPreValue   = false;
	m_showTop        = false;
	m_bottom         = btNo;
	m_showLine       = false;
	m_action         = acNo;
	m_showOHLC	 = infoNO;
	m_bGradient	 = false;
	m_percent	 = pcNo;
	m_nUDPercent	 = 0;
	m_bLButtonDown	 = false;
	m_bSiga          = false;
	m_bBong		 = false;
	m_bong		 = bongBong;
	m_bCurr		 = true;
	m_bBoldPrice	 = false;
	m_bBoldRemain	 = false;
	m_bBoldCnt		 = false;
	m_bConfig	 = true;

	m_pBitmapC1 = NULL;
	m_pBitmapC2 = NULL;
	m_pBitmapP1 = NULL;
	m_pBitmapP2 = NULL;

	EW_Init();

	ZeroMemory(&m_config, sz_CONFIG);
}

Cdepth::~Cdepth()
{
	m_indices.RemoveAll(); freeItems();

	if (IsWindow(m_pToolTip->GetSafeHwnd()))
		m_pToolTip->SendMessage(WM_CLOSE);
	m_pToolTip.reset();
}

void Cdepth::OnDestroy() 
{
	EW_Destroy();

	CWnd::OnDestroy();
}

void Cdepth::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(Cdepth, CWnd)
	//{{AFX_MSG_MAP(Cdepth)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_MBONG, OnMBong)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(Cdepth, CWnd)
	//{{AFX_DISPATCH_MAP(Cdepth)
	DISP_PROPERTY_EX(Cdepth, "Type", _getType, _setType, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "HogaType", _getHogaType, _setHogaType, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "BeforeCompare", _getBeforeCompare, _setBeforeCompare, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "Percent", _getPercent, _setPercent, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "Graph", _getGraph, _setGraph, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "TopDisplay", _getTopDisplay, _setTopDisplay, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "BottomDisplay", _getBottomDisplay, _setBottomDisplay, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "Line", _getLine, _setLine, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "LButtonDown", _getLButtonDown, _setLButtonDown, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "OHLC", _getOHLC, _setOHLC, VT_I4)
	DISP_PROPERTY_EX(Cdepth, "visible", _GetVisible, _SetVisible, VT_BOOL)
	DISP_PROPERTY_EX(Cdepth, "BongStatus", _GetBongStatus, _SetBongStatus, VT_I2)
	DISP_PROPERTY_EX(Cdepth, "CurrStatus", _GetCurrStatus, _SetCurrStatus, VT_I2)
	DISP_PROPERTY_EX(Cdepth, "BoldPrice", _getBoldPrice, _setBoldPrice, VT_BOOL)
	DISP_PROPERTY_EX(Cdepth, "BoldRemain", _getBoldRemain, _setBoldRemain, VT_BOOL)
	DISP_PROPERTY_EX(Cdepth, "BoldRemainCount", GetBoldRemainCount, SetBoldRemainCount, VT_BOOL)
	DISP_FUNCTION(Cdepth, "SetProperties", _SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(Cdepth, "GetProperties", _GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(Cdepth, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(Cdepth, "ChangeInfo", _ChangeInfo, VT_I2, VTS_NONE)
	DISP_FUNCTION(Cdepth, "ChangeBong", ChangeBong, VT_I2, VTS_NONE)
	DISP_FUNCTION(Cdepth, "Clear", _Clear, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Idepth to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D31D7272-4BBD-4056-B48E-28104E17B458}
static const IID IID_Idepth =
{ 0xd31d7272, 0x4bbd, 0x4056, { 0xb4, 0x8e, 0x28, 0x10, 0x4e, 0x17, 0xb4, 0x58 } };

BEGIN_INTERFACE_MAP(Cdepth, CWnd)
	INTERFACE_PART(Cdepth, IID_Idepth, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cdepth message handlers

long Cdepth::_getType() 
{
	return (long)m_type;
}

void Cdepth::_setType(long nNewValue) 
{
	if (nNewValue >= 1 && nNewValue <= 7)
		m_type = _ctype(nNewValue);
}

long Cdepth::_getHogaType() 
{
	return m_depth;
}

void Cdepth::_setHogaType(long nNewValue) 
{
	const	int	depth = m_depth;

	if (nNewValue == 5 || nNewValue == 10)
		m_depth = nNewValue;

	if (depth != m_depth)
	{
		if (m_pContract)
			m_pContract->ChangeDisplay(m_depth);
	}
}

long Cdepth::_getBeforeCompare() 
{
	return m_showBeforeDiff ? 1 : 0;
}

void Cdepth::_setBeforeCompare(long nNewValue) 
{
	m_showBeforeDiff = nNewValue ? true : false;
}

long Cdepth::_getPercent() 
{
	return m_percent;
}

void Cdepth::_setPercent(long nNewValue) 
{
	m_percent = (enum _percent)nNewValue;
}

long Cdepth::_getGraph() 
{
	return (long)m_bar;
}

void Cdepth::_setGraph(long nNewValue) 
{
	if (nNewValue >= 0 && nNewValue <= 2)
		m_bar = (_graph)nNewValue;
}

long Cdepth::_getTopDisplay() 
{
	return m_showTop ? 1 : 0;
}

void Cdepth::_setTopDisplay(long nNewValue) 
{
	m_showTop = nNewValue ? true : false;
}

long Cdepth::_getBottomDisplay() 
{
	return (long)m_bottom;
}

void Cdepth::_setBottomDisplay(long nNewValue) 
{
	if (nNewValue >= 0 && nNewValue <= 3)
		m_bottom = (_bottom)nNewValue;
}

long Cdepth::_getLine() 
{
	return m_showLine ? 1 : 0;
}

void Cdepth::_setLine(long nNewValue) 
{
	m_showLine = nNewValue ? true : false;
}

long Cdepth::_getLButtonDown() 
{
	return (long)m_action;
}

void Cdepth::_setLButtonDown(long nNewValue) 
{
	if (nNewValue >= 0 && nNewValue <= 2)
		m_action = (_act)nNewValue;
}

long Cdepth::_getOHLC() 
{
	return m_showOHLC;
}

void Cdepth::_setOHLC(long nNewValue) 
{
	m_showOHLC = (enum _showInfo)nNewValue;
}

short Cdepth::_ChangeInfo() 
{
	if (m_showOHLC == infoNO)
		m_showOHLC = infoMGVL;
	else if (m_showOHLC == infoMGVL)
		m_showOHLC = infoSHGA;
	else if (m_showOHLC == infoSHGA)
		m_showOHLC = infoNO;

	m_config.info = m_showOHLC;
	
	SaveCondition(m_path, &m_config);
	
	Invalidate();

	return m_showOHLC;
}

short Cdepth::ChangeBong() 
{
	if (m_bong == bongNo)
		m_bong = bongBong;
	else if (m_bong == bongBong)
		m_bong = bongChgl;
	else
		m_bong = bongNo;

	m_config.mbong = m_bong;

	SaveCondition(m_path, &m_config);
	
	return m_bong;
}

short Cdepth::_GetBongStatus() 
{
	return m_bong;
}

void Cdepth::_SetBongStatus(short nNewValue) 
{
	m_bong = (enum _bong)nNewValue;

	m_config.mbong = m_bong;

	Invalidate();

	SaveCondition(m_path, &m_config);
}

short Cdepth::_GetCurrStatus()
{
	return m_config.currset;
	SaveCondition(m_path, &m_config);
}

void Cdepth::_SetCurrStatus(short nNewValue) 
{
	m_config.currset = (char)nNewValue;
	SaveCondition(m_path, &m_config);
}

BOOL Cdepth::_getBoldPrice() 
{
	return m_bBoldPrice ? TRUE : FALSE;
}

void Cdepth::_setBoldPrice(BOOL bNewValue) 
{
	m_bBoldPrice = bNewValue ? true : false;
	Invalidate();
}

void Cdepth::_SetProperties(LPCTSTR options) 
{
	m_depth          = 10;
	m_type           = ctNo;
	m_showBeforeDiff = false;
	m_percent	 = pcNo;
	m_bar            = grNo;
	m_showPreValue   = false;
	m_showTop        = false;
	m_bottom         = btNo;
	m_showLine       = false;
	m_action         = acNo;
	m_showOHLC       = infoNO;
	m_bGradient	 = false;

	parseOptions();
	setItemRect();
	Invalidate();
}

BSTR Cdepth::_GetProperties() 
{
	CString strResult, text;

	strResult.Format("/u%d/d%d/s%d/h%d", (int)m_type, m_depth, (int)m_bar, (int)m_bottom);

	if (m_showBeforeDiff)
		strResult += _T("/b");
	
	if (m_percent)
		strResult.Format("%s/p%d", strResult, m_percent);

	if (m_showTop)
		strResult += _T("/c");

	if (m_showLine)
		strResult += _T("/l");

	return strResult.AllocSysString();
}

void Cdepth::_Refresh() 
{
	setItemRect();
	EW_Move();
	Invalidate();
}

BOOL Cdepth::Create(CWnd* parent, void* ptr)
{
	char	wb[128]{};
	struct	_param*	param = (struct _param*)ptr;

	m_parent  = parent;
	strcpy(wb, param->fonts);
	m_fonts   = wb;
	m_szName  = CString(param->name, param->name.GetLength());
	m_key	  = param->key;
	m_point   = param->point;
	m_style   = param->style;
	m_rect    = param->rect;
	strcpy(wb, param->options);
	m_options = wb;
	
	CString root, user;
	root = Variant(homeCC, "");
	user = Variant(nameCC, "");
	m_file.Format("%s\\user\\%s\\curamtset.ini", root, user);
	m_configFile = _T("");

	initialColor();		// updateX_20060102

	if (CWnd::CreateEx(NULL, m_class, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_rect, m_parent, idDEPTH, NULL))
	{
		createFont();
		parseOptions();
		initialize();
		if (!EW_Create())
			return FALSE;

		EW_Font();

		m_pToolTip = std::make_unique<CToolTip>(m_parent, this);
		m_pToolTip->Create();
		m_pToolTip->ShowWindow(SW_HIDE);

		EW_Move();
		EW_Show();

		Invalidate(FALSE);
		return TRUE;
	}

	return FALSE;
}

BOOL Cdepth::OnEraseBkgnd(CDC* dc) 
{
//	return CWnd::OnEraseBkgnd(dc);
	return TRUE;
}

void Cdepth::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	m_rect.right  = m_rect.left + cx;
	m_rect.bottom = m_rect.top + cy;

	calculateRowHeight();
	calculateColumnWidth();
	EW_Move();
	if (m_items.GetSize() > 0)
		setItemRect();
}

void Cdepth::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	m_sync.Lock();
	draw(&dc);
	m_sync.Unlock();
}

void Cdepth::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_tracking)
	{
		TRACKMOUSEEVENT tme;

		tme.cbSize    = sizeof(tme);
		tme.hwndTrack = GetSafeHwnd();
		tme.dwFlags   = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_tracking = _TrackMouseEvent(&tme);
	}
	setFocus(point);
	CWnd::OnMouseMove(nFlags, point);
}

void Cdepth::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLButtonDown = true;

	CRect	rect = m_rcInfoX, itemRC;
	rect.left = RATERECTSEL(rect)+1;

	CString	entry = _T("");
	if (rect.PtInRect(point))
	{
		int line = 5 + (m_showTop ? 1 : 0);
		for (int ii = 0; ii < 5; ii++, line++)
		{
			itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
			if (!itemRC.PtInRect(point))
				continue;

			switch (ii)
			{
			case 0: entry = m_items.GetAt(shPrice)->m_data;		break;
			case 1: entry = m_items.GetAt(openPrice)->m_data;	break;
			case 2: entry = m_items.GetAt(highPrice)->m_data;	break;
			case 3: entry = m_items.GetAt(lowPrice)->m_data;	break;
			case 4: entry = m_items.GetAt(hhPrice)->m_data;		break;
			}
			if (!entry.IsEmpty())
			{
				CString string; string.Format("%s\t%s\n", "clickItemX", entry); trigger(string);
			}
			break;
		}

	}
	//체결데이터 확인위해 넣었던 테스트 코드
	/*  
	else
	{
		//asdfasdf
		AfxMessageBox("EW_Alert");
		EW_Alert("201	60887	025	 150.05	202	165.64	026	 150.00	203	+1.30	027	16739	204	-14.29	028	11853	205	-9.41	029	+162.30	019	+1	206	+198	207	0	030	+162.30	020	-0	031	-135.00	032	+1	033	+0.03	023	-150.00	034	160035	002	+164.34	024	+0.05	");
	}
	*/

	CWnd::OnLButtonDown(nFlags, point);
}

void Cdepth::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_action == acNo)
		return;

	if (m_bLButtonDown == false)
		return;

	if (m_prect.PtInRect(point))
	{
		if (m_focus > -1)
		{
			if (m_focus >= askPrice1 && m_focus <= askPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
				m_focus = -1;
				InvalidateRect(m_prect);
			}
			else if (m_focus >= bidPrice1 && m_focus <= bidPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
				m_focus = -1;
				InvalidateRect(m_prect);
			}
		}

		const	int	select = getIndexByPoint(point);
		if (select > 0)
		{
			CString Price = m_items.GetAt(select)->m_data;
			Price.Remove(',');
			switch (m_type)
			{
			case ctFuture:
			case ctOption:
			case ctKOFEX:
				if (m_items.GetAt(select)->m_data.GetLength() > 0)
				{
					switch (m_action)
					{
					case acPopup:
						popupMenu(point, Price);
						break;
					case acMsg:
						sendMessage(point, Price);
					case acNo:
						break;
					}
				}
				break;
			}
		}
	}
	else if (m_rcInfo.PtInRect(point) && m_action != acNo)
	{
		CRect	rect; rect = m_rcInfo;
		rect.left = RATERECTBUY(rect) +1;

		if (rect.PtInRect(point))
			clickAction(rect, point);
	}
	else if (m_bConfig)
	{
		if (m_rcConfig.PtInRect(point))
			ConfigDlg();	
		else if (m_rcConfigL.PtInRect(point))
			ConfigDlgL();	
		else if (m_rcConfigR.PtInRect(point))
			ConfigDlgR();	
	}

	if (!m_refsym.IsEmpty())
	{
		m_bLButtonDown = false;
		return;
	}

	m_bLButtonDown = false;

	CWnd::OnLButtonUp(nFlags, point);
}

void Cdepth::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonUp(nFlags, point);
}

long Cdepth::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		m_sync.Lock();
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		dispatch(exth->data, exth->size);
		m_sync.Unlock();
		break;
	case DLL_OUB:
		m_sync.Lock();
		dispatch((char *)lParam, HIWORD(wParam));
		m_sync.Unlock();
		break;

// 	case DLL_ALERT:
// 		if (HIWORD(wParam) == alert_DEIN)
// 			break;
// 		m_sync.Lock();
// 		alert((char *)lParam);
// 		EW_Alert((char *)lParam);
// 		m_sync.Unlock();
// 		break;
	case DLL_ALERTx:
		{
			if (HIWORD(wParam) == alert_DEIN)
				break;
			struct _alertR* alertR;
			alertR = (struct _alertR*)lParam;
			
			m_sync.Lock();
			//AfxMessageBox((char *)lParam);
			alert(alertR);
			//TRACE((char *)lParam);
			
			EW_Alert(alertR);
			m_sync.Unlock();
		}
		break;
	case DLL_SETPAL:
		initialColor(); Invalidate();
		break;
		if (m_showTop)
		{
			CRect	rect;
			rect.SetRect(0, 0, m_columns[2], m_topH);
			InvalidateRect(rect);
		}
		break;

	case DLL_SETFONT:
	case DLL_SETFONTx:
		m_point = HIWORD(wParam);
		if (lParam)
		{
			CString	fonts = CString((char *)lParam);
			if (!fonts.IsEmpty())
				m_fonts = fonts;
		}
		createFont();
		Invalidate(FALSE);
		break;

	case DLL_TRIGGER:
	case DLL_DOMINO:
	case DLL_INB:
	case DLL_NOTICE:
	case DLL_DROP:
	case DLL_AXIS:
	case DLL_EXCEL:
	case DLL_INVOKE:
	case DLL_SETFCB:
	case DLL_SELECT:
		break;
	}

	return 0;
}

long Cdepth::OnMBong(WPARAM wParam, LPARAM lParam)
{
	CString	entry = (char*)lParam;
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if (!entry.IsEmpty())
	{
		entry.Remove(',');

		switch (m_action)
		{
		case acPopup:	popupMenu(point, entry);	break;
		case acMsg:	sendMessage(point, entry);	break;
		case acNo:					break;
		}
	}

	return 0;
}

LRESULT Cdepth::OnMouseLeave(WPARAM wParam, LPARAM lParam) 
{
	m_tracking = FALSE;
	if (m_focus > -1)
	{
		if (m_focus >= askPrice1 && m_focus <= askPrice10)
		{
			m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
			m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
			m_focus = -1;
			InvalidateRect(m_prect);
		}
		else if (m_focus>= bidPrice1 && m_focus <= bidPrice10)
		{
			m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
			m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
			m_focus = -1;
			InvalidateRect(m_prect);
		}
	}

	if (m_pToolTip->IsWindowVisible())
	{
		m_pToolTip->ShowWindow(SW_HIDE);
	}
	return 0;
}

// updateX_20060102
void Cdepth::initialColor()
{
	m_clrWhite	= PALETTERGB(255, 255, 255);

	m_clrHeadFg	= getColor(76);
	m_clrHeadBk	= getColor(74);
	m_clrDataFg	= getColor(69);

	m_clrAskGs	= getColor(110);
	m_clrAskGe	= getColor(119);

	m_clrBidGs	= getColor(120);
	m_clrBidGe	= getColor(110);


	m_clrAskSzFg	= getColor(81);
	m_clrBidSzFg	= getColor(84);

	m_clrLine	= getColor(75);
	m_clrDash	= getColor(75);
	m_clrBox	= getColor(75);
	m_clrCurr	= getColor(86);
	m_clrBottom	= m_clrWhite;

	m_clrUp		= getColor(94);
	m_clrDown	= getColor(95);

	m_clrFocus	= getColor(87);

	m_clrBarAsk	= getColor(82);
	m_clrBarBid	= getColor(85);

	EW_Palette();
}

void Cdepth::initialize()
{
	std::shared_ptr<Citem> item = nullptr;

	for (int ii = 0; ii < N_DATA; ii++)
	{
		item = std::make_shared<Citem>();
		item->m_data   = _T("");
		item->m_fRGB   = m_clrDataFg;
		item->m_center = false;
		item->m_attr   = atNormal;
		item->m_bRc.SetRectEmpty();
		item->m_fRc.SetRectEmpty();

		if (ii >= askSize1 && ii <= askSize10)
			item->m_fRGB = m_clrAskSzFg;
		else if (ii >= bidSize1 && ii <= bidSize10)
			item->m_fRGB = m_clrBidSzFg;

		if (ii >= askPrice1 && ii <= askBefore10)
		{
			item->m_pRGB = SetItemBGColor(ii);
			if (ii >= askPrice1 && ii <= askPrice10)
			{
				switch (m_type)
				{
				case ctFuture:
				case ctOption:
				case ctKOFEX:
					item->m_attr = atCoRgbSup;
					break;
				}
			}
			else if (ii >= askPercent1 && ii <= askPercent10)
				item->m_fRGB = m_clrDown;
			else if (ii >= askSize1 && ii <= askSize10)
				item->m_attr = atSignSup;
			else	item->m_attr = atCoRgbSup;
		}
		else if (ii >= bidPrice1 && ii <= bidBefore10)
		{
			item->m_pRGB = SetItemBGColor(ii);
			if (ii >= bidPrice1 && ii <= bidPrice10)
			{
				switch (m_type)
				{
				case ctFuture:
				case ctOption:
				case ctKOFEX:
					item->m_attr = atCoRgbSup;	break;
				}
			}
			else if (ii >= bidPercent1 && ii <= bidPercent10)
				item->m_fRGB = m_clrUp;
			else if (ii >= bidSize1 && ii <= bidSize10)
				item->m_attr = atSignSup;
			else	item->m_attr = atCoRgbSup;
		}
		else
		{
			item->m_pRGB = m_clrWhite;
			switch (ii)
			{
			case cTime:
				item->m_attr = atTime;
				item->m_center = true;
				item->m_pRGB = m_clrBottom;
				break;
			case sizeDiff:
				item->m_center = true;
				item->m_attr = atCoRgbSup;
				break;
			case askTotalSize:
				item->m_attr = atNoColor;
				break;
			case askTotalBefore:
			case bidTotalBefore:
				item->m_attr = atCoRgbSup;
				break;
			case bidTotalSize:
				item->m_attr = atNoColor;
				break;
			case askSizeTime:
				item->m_attr = atNoColor;
				item->m_fRGB = m_clrDown;
				break;
			case askSizeTimeBefore:
				item->m_attr = atNoColor;
				break;
			case bidSizeTime:
				item->m_attr = atNoColor;
				item->m_fRGB = m_clrUp;
				break;
			case bidSizeTimeBefore:
				item->m_attr = atNoColor;
				break;
			case openPrice:
			case highPrice:
			case lowPrice:
			case cloPrice:
			case gajungPrice:
				item->m_attr = atCoRgbSup;
				break;
			case prePrice:
				item->m_attr = atCoRgbSup;
				break;
			case preBefore:
				item->m_attr = atArrow;
				break;
			case prePercent:
				item->m_attr = atPercent;
				break;
			case infoRSH:
			case shPrice:
				item->m_fRGB = m_clrUp;
			case infoRHH:
			case hhPrice:
				if (ii == hhPrice || ii == infoRHH)
					item->m_fRGB = m_clrDown;
				item->m_attr = atComma;
				break;
			case gAMT:
			case infoMGJV:
				item->m_attr = atComma;
				break;
			case infoMGDF:
			case infoRATE:
			case infoDELT:
			case infoGAMM:
				item->m_attr = atCoRgb;
				break;
			case infoBASE:
			case infoMDSD:
			case infoMSSD:
				item->m_attr = atCoRgbSup;
				break;
// updateX_20060309
			case infoCURR:
				item->m_attr = atCoRgbSup;
				break;
			case infoDIFF:
				item->m_attr = atArrow;
				break;
			case infoUDYL:
				item->m_attr = atCoRgb;
				break;
			case infoGVOL:
				item->m_attr = atComma;
				break;
			case infoGUNSUCHA:
				item->m_attr = atCoRgbSup;
			}
		}
		m_items.Add(item);
	}
	
	setIndices();
	calculateRowHeight();
	calculateColumnWidth();

	getMaxSize(); setItemRect();

	CString root, image;
	root = Variant(homeCC, "");
	image.Format("%s\\%s\\", root, IMAGEDIR);
	if (m_pBitmapC1 == NULL) m_pBitmapC1 = getBitmap(image + _T("호가_설정.bmp"));
	if (m_pBitmapC2 == NULL) m_pBitmapC2 = getBitmap(image + _T("호가_설정_dn.bmp"));
	if (m_pBitmapP1 == NULL) m_pBitmapP1 = getBitmap(image + _T("호가_시간.bmp"));
	if (m_pBitmapP2 == NULL) m_pBitmapP2 = getBitmap(image + _T("호가_시간_dn.bmp"));

	CString name = Variant(nameCC, "");
	m_path.Format("%s\\%s\\%s\\", root, USRDIR, name);

	if (!ReadCondition(m_path, &m_config))
	{
/*		m_config.info = 1;
		m_config.mbong = 2;
		m_config.curr = 1;
		m_config.hbold = 0;
		m_config.sgrp = 1;
		m_config.hdyul = 1;
		m_config.info = 1;
		m_config.currset = 1;
*/
		m_config.info = 1;
		m_config.mbong = 2;
		m_config.curr = 1;
		m_config.hbold = 1;
		m_config.sgrp = 1;
		m_config.hdyul = 1;
		m_config.sbong = 1;
		m_config.currset = 1;
		m_config.pre = 1;
		m_config.hrbold = 0;
		m_config.hcbold = 0;
		
		SaveCondition(m_path, &m_config);
	}
	InitEnv();
}

void Cdepth::setIndices()
{
	int	ii = 0;

	switch (m_type)
	{
	case ctOption: case ctKOption: case ctFuture: case ctKOFEX:
		for (ii = 0; ii < sizeof(keys)/sizeof(struct _keys); ii++)
			m_indices.SetAt(keys[ii].key, keys[ii].index);
		break;
	default:
		break;
	}
}

void Cdepth::dispatch(char* datB, int datL)
{
	int	size = 0;

	if (m_type == ctFuture || m_type == ctOption || m_type == ctKOFEX || m_type == ctKOption)
		size = sizeof(struct _hogax);
	else
		return;

	if (datL == 0)
	{
		clear();
		Invalidate();
		return;
	}

	if (datL == sz_hoga + sz_Extra)
	{
		dispatchTEN(datB, datL);
		EW_Dispatch(datB, true);
	}
	else if (datL == sz_hogax + sz_Extra)
	{
		dispatchFIVE(datB, datL);
		EW_Dispatch(datB, false);
	}
}

void Cdepth::dispatchTEN(char* datB, int datL)
{
	CString	tmps, price;
	struct	_hoga*	hoga = (struct _hoga *) datB;

	m_code = CString(hoga->codx, sizeof(hoga->codx));
	m_code.Trim();

	tmps = CString(hoga->curr, sizeof(hoga->curr));
	tmps.Trim();
	m_curr = fabs(str2double(tmps));

	tmps = CString(hoga->pcls, sizeof(hoga->pcls));
	tmps.Trim();
	m_clos = str2double(tmps);
	m_items.GetAt(cloPrice)->m_data = format(tmps, cloPrice);

	for (int ii = 0; ii < 10; ii++)
	{
		tmps = CString(hoga->price[ii].ask, sizeof(hoga->price[ii].ask));
		//m_items.GetAt(askPrice1+ii)->m_data = format(tmps, askPrice1+ii);

		price = tmps;

		tmps = CString(hoga->price[ii].avol, sizeof(hoga->price[ii].avol)); tmps.Trim();
		m_items.GetAt(askSize1+ii)->m_data  = _T("");
		if (atoi(tmps) > 0)
		{
			//2016.11.10 KSJ 매도 건수가 있을 때만 표시해준다.
			m_items.GetAt(askPrice1+ii)->m_data = format(price, askPrice1+ii);
			m_items.GetAt(askSize1+ii)->m_data = format(tmps, askSize1+ii);
		}
		else
		{
			m_items.GetAt(askPrice1+ii)->m_data = "";
			m_items.GetAt(askSize1+ii)->m_data = "";
		}
		
		tmps = CString(hoga->price[ii].acha, sizeof(hoga->price[ii].acha));
		m_items.GetAt(askBefore1+ii)->m_data = format(tmps, askBefore1+ii);
		
		// 매수
		tmps = CString(hoga->price[ii].bid, sizeof(hoga->price[ii].bid));
		price = tmps;

		//m_items.GetAt(bidPrice1+ii)->m_data = format(tmps, bidPrice1+ii);
		
		tmps = CString(hoga->price[ii].bvol, sizeof(hoga->price[ii].bvol)); tmps.Trim();
		m_items.GetAt(bidSize1+ii)->m_data  = _T("");
		if (atoi(tmps) > 0)
		{
			//2016.11.10 KSJ 매수 건수가 있을 때만 표시해준다.
			m_items.GetAt(bidPrice1+ii)->m_data = format(price, bidPrice1+ii);
			m_items.GetAt(bidSize1+ii)->m_data = format(tmps, bidSize1+ii);
		}
		else
		{
			m_items.GetAt(bidPrice1+ii)->m_data = "";
			m_items.GetAt(bidSize1+ii)->m_data = "";
		}
		
		tmps = CString(hoga->price[ii].bcha, sizeof(hoga->price[ii].bcha));
		m_items.GetAt(bidBefore1+ii)->m_data = format(tmps, bidBefore1+ii);
	}

	m_items.GetAt(infoCURR)->m_data = format(CString(hoga->curr, sizeof(hoga->curr)), infoCURR);
	m_items.GetAt(infoDIFF)->m_data = format(CString(hoga->diff, sizeof(hoga->diff)), infoDIFF);
	m_items.GetAt(infoUDYL)->m_data = format(CString(hoga->udyl, sizeof(hoga->udyl)), infoUDYL);
	m_items.GetAt(infoGVOL)->m_data = format(CString(hoga->gvol, sizeof(hoga->gvol)), infoGVOL);
	
	tmps = CString(hoga->pcls, sizeof(hoga->pcls)); tmps.Trim();
	m_clos = str2double(tmps);
	m_items.GetAt(cloPrice)->m_data = format(tmps, cloPrice);
	
	m_items.GetAt(cTime)->m_data = format(CString(hoga->htim, sizeof(hoga->htim)), cTime);
	m_items.GetAt(askTotalSize)->m_data   = format(CString(hoga->avols, sizeof(hoga->avols)), askTotalSize);
	m_items.GetAt(askTotalBefore)->m_data = format(CString(hoga->acha, sizeof(hoga->acha)), askTotalBefore);
	m_items.GetAt(bidTotalSize)->m_data   = format(CString(hoga->bvols, sizeof(hoga->bvols)), bidTotalSize);
	m_items.GetAt(bidTotalBefore)->m_data = format(CString(hoga->bcha, sizeof(hoga->bcha)), bidTotalBefore);
	
	m_items.GetAt(openPrice)->m_data  = format(CString(hoga->siga, sizeof(hoga->siga)), openPrice);
	m_items.GetAt(openPrice)->m_data.TrimLeft();
	if (!m_items.GetAt(openPrice)->m_data.IsEmpty())
		m_bSiga = true;
	m_items.GetAt(highPrice)->m_data  = format(CString(hoga->koga, sizeof(hoga->koga)), highPrice);
	m_items.GetAt(lowPrice)->m_data   = format(CString(hoga->jega, sizeof(hoga->jega)), lowPrice);
	m_items.GetAt(paperPrice)->m_data = format(CString(hoga->amga, sizeof(hoga->amga)), paperPrice);
	m_items.GetAt(gijunPrice)->m_data = format(CString(hoga->gjga, sizeof(hoga->gjga)), gijunPrice);
	
	m_items.GetAt(prePrice)->m_data  = format(CString(hoga->ysga, sizeof(hoga->ysga)), prePrice);
	m_items.GetAt(preVolume)->m_data = format(CString(hoga->yvol, sizeof(hoga->yvol)), preVolume);
	
	m_items.GetAt(shPrice)->m_data = format(CString(hoga->shga, sizeof(hoga->shga)), shPrice);
	m_items.GetAt(hhPrice)->m_data = format(CString(hoga->hhga, sizeof(hoga->hhga)), hhPrice);
	
	m_items.GetAt(askSizeTime)->m_data       = format(CString(hoga->ahvol, sizeof(hoga->ahvol)), askSizeTime);
	m_items.GetAt(askSizeTimeBefore)->m_data = format(CString(hoga->ahvc, sizeof(hoga->ahvc)), askSizeTimeBefore);
	m_items.GetAt(bidSizeTime)->m_data       = format(CString(hoga->bhvol, sizeof(hoga->bhvol)), bidSizeTime);
	m_items.GetAt(bidSizeTimeBefore)->m_data = format(CString(hoga->bhvc, sizeof(hoga->bhvc)), bidSizeTimeBefore);
	
	m_items.GetAt(sizeDiff)->m_data = format(CString(hoga->dscha, sizeof(hoga->dscha)), sizeDiff);
	
	m_items.GetAt(infoMGJV)->m_data = format(CString(hoga->mgjv, sizeof(hoga->mgjv)), infoMGJV);
	m_items.GetAt(infoMGDF)->m_data = format(CString(hoga->mgdf, sizeof(hoga->mgdf)), infoMGDF);
	m_items.GetAt(infoMGSI)->m_data = format(CString(hoga->mgsi, sizeof(hoga->mgsi)), infoMGSI);
	m_items.GetAt(infoMGKO)->m_data = format(CString(hoga->mgko, sizeof(hoga->mgko)), infoMGKO);
	m_items.GetAt(infoMGJE)->m_data = format(CString(hoga->mgje, sizeof(hoga->mgje)), infoMGJE);
	
	m_items.GetAt(infoBASE)->m_data = format(CString(hoga->base, sizeof(hoga->base)), infoBASE);
	
	m_items.GetAt(infoRATE)->m_data = format(CString(hoga->rate, sizeof(hoga->rate)), infoRATE);
	m_items.GetAt(infoIRGA)->m_data = format(CString(hoga->irga, sizeof(hoga->irga)), infoIRGA);
	m_items.GetAt(infoNJBD)->m_data = format(CString(hoga->njbd, sizeof(hoga->njbd)), infoNJBD);
	m_items.GetAt(infoDELT)->m_data = format(CString(hoga->delt, sizeof(hoga->delt)), infoDELT);
	m_items.GetAt(infoGAMM)->m_data = format(CString(hoga->gamm, sizeof(hoga->gamm)), infoGAMM);

	m_items.GetAt(infoRSH)->m_data = format(CString(hoga->msha, sizeof(hoga->msha)), infoRSH);
	m_items.GetAt(infoRHH)->m_data = format(CString(hoga->mhha, sizeof(hoga->mhha)), infoRHH);

	CString sAskTotalBefore,sBidTotalBefore;
	sAskTotalBefore = m_items.GetAt(askTotalBefore)->m_data;
	sAskTotalBefore.Remove(',');
	
	sBidTotalBefore = m_items.GetAt(bidTotalBefore)->m_data;
	sBidTotalBefore.Remove(',');
	
	const	int	nATB = atoi(sAskTotalBefore);
	const	int	nBTB = atoi(sBidTotalBefore);
	
	CString gunsu;
	if (nBTB > nATB)
		gunsu.Format("+%d",nBTB - nATB);
	else if (nBTB < nATB)
		gunsu.Format("%d",nBTB - nATB);
	else if (nBTB == nATB)
			gunsu = "0";
	
	m_items.GetAt(infoGUNSUCHA)->m_data = format(gunsu, infoGUNSUCHA);
	
	m_sum4 = atof(CString(hoga->sum4, sizeof(hoga->sum4)));
	m_sum9 = atof(CString(hoga->sum9, sizeof(hoga->sum9)));
	
	if (str2double(m_items.GetAt(prePrice)->m_data))
		m_showPreValue = true;
	else
		m_showPreValue = false;
	
	setItemRect(); calculatePercent(); getMaxSize();
	
	Invalidate();
}

void Cdepth::dispatchFIVE(char* datB, int datL)
{
	CString	tmps;
	struct	_hogax*	hoga;

	hoga = (struct _hogax *) datB;
	m_code = CString(hoga->codx, sizeof(hoga->codx));
	m_code.Trim();

	for (int ii = 0; ii < 5; ii++)
	{
	// 매도
		tmps = CString(hoga->price[ii].ask, sizeof(hoga->price[ii].ask));
		m_items.GetAt(askPrice1+ii)->m_data = format(tmps, askPrice1+ii);

		tmps = CString(hoga->price[ii].avol, sizeof(hoga->price[ii].avol)); tmps.Trim();
		m_items.GetAt(askSize1+ii)->m_data  = _T("");
		if (atoi(tmps) > 0)
			m_items.GetAt(askSize1+ii)->m_data = format(tmps, askSize1+ii);

		tmps = CString(hoga->price[ii].acha, sizeof(hoga->price[ii].acha));
		m_items.GetAt(askBefore1+ii)->m_data = format(tmps, askBefore1+ii);

	// 매수
		tmps = CString(hoga->price[ii].bid, sizeof(hoga->price[ii].bid));
		m_items.GetAt(bidPrice1+ii)->m_data = format(tmps, bidPrice1+ii);

		tmps = CString(hoga->price[ii].bvol, sizeof(hoga->price[ii].bvol)); tmps.Trim();
		m_items.GetAt(bidSize1+ii)->m_data  = _T("");
		if (atoi(tmps) > 0)
			m_items.GetAt(bidSize1+ii)->m_data = format(tmps, bidSize1+ii);

		tmps = CString(hoga->price[ii].bcha, sizeof(hoga->price[ii].bcha));
		m_items.GetAt(bidBefore1+ii)->m_data = format(tmps, bidBefore1+ii);
	}

	tmps = CString(hoga->curr, sizeof(hoga->curr)); tmps.Trim();
	m_curr = str2double(tmps);

// updateX_20060309
	m_items.GetAt(infoCURR)->m_data = format(CString(hoga->curr, sizeof(hoga->curr)), infoCURR);
	m_items.GetAt(infoDIFF)->m_data = format(CString(hoga->diff, sizeof(hoga->diff)), infoDIFF);
	m_items.GetAt(infoUDYL)->m_data = format(CString(hoga->udyl, sizeof(hoga->udyl)), infoUDYL);
	m_items.GetAt(infoGVOL)->m_data = format(CString(hoga->gvol, sizeof(hoga->gvol)), infoGVOL);

	tmps = CString(hoga->pcls, sizeof(hoga->pcls)); tmps.Trim();
	m_clos = str2double(tmps);
	m_items.GetAt(cloPrice)->m_data = format(tmps, cloPrice);

	m_items.GetAt(cTime)->m_data = format(CString(hoga->htim, sizeof(hoga->htim)), cTime);
	m_items.GetAt(askTotalSize)->m_data   = format(CString(hoga->avols, sizeof(hoga->avols)), askTotalSize);
	m_items.GetAt(askTotalBefore)->m_data = format(CString(hoga->acha, sizeof(hoga->acha)), askTotalBefore);
	m_items.GetAt(bidTotalSize)->m_data   = format(CString(hoga->bvols, sizeof(hoga->bvols)), bidTotalSize);
	m_items.GetAt(bidTotalBefore)->m_data = format(CString(hoga->bcha, sizeof(hoga->bcha)), bidTotalBefore);

	m_items.GetAt(openPrice)->m_data  = format(CString(hoga->siga, sizeof(hoga->siga)), openPrice);
	m_items.GetAt(openPrice)->m_data.TrimLeft();
	if (!m_items.GetAt(openPrice)->m_data.IsEmpty())
		m_bSiga = true;
	m_items.GetAt(highPrice)->m_data  = format(CString(hoga->koga, sizeof(hoga->koga)), highPrice);
	m_items.GetAt(lowPrice)->m_data   = format(CString(hoga->jega, sizeof(hoga->jega)), lowPrice);
	m_items.GetAt(paperPrice)->m_data = format(CString(hoga->amga, sizeof(hoga->amga)), paperPrice);
	m_items.GetAt(gijunPrice)->m_data = format(CString(hoga->gjga, sizeof(hoga->gjga)), gijunPrice);

	m_items.GetAt(prePrice)->m_data  = format(CString(hoga->ysga, sizeof(hoga->ysga)), prePrice);
	m_items.GetAt(preVolume)->m_data = format(CString(hoga->yvol, sizeof(hoga->yvol)), preVolume);

	m_items.GetAt(shPrice)->m_data = format(CString(hoga->shga, sizeof(hoga->shga)), shPrice);
	m_items.GetAt(hhPrice)->m_data = format(CString(hoga->hhga, sizeof(hoga->hhga)), hhPrice);

	m_items.GetAt(askSizeTime)->m_data       = format(CString(hoga->ahvol, sizeof(hoga->ahvol)), askSizeTime);
	m_items.GetAt(askSizeTimeBefore)->m_data = format(CString(hoga->ahvc, sizeof(hoga->ahvc)), askSizeTimeBefore);
	m_items.GetAt(bidSizeTime)->m_data       = format(CString(hoga->bhvol, sizeof(hoga->bhvol)), bidSizeTime);
	m_items.GetAt(bidSizeTimeBefore)->m_data = format(CString(hoga->bhvc, sizeof(hoga->bhvc)), bidSizeTimeBefore);

	m_items.GetAt(sizeDiff)->m_data = format(CString(hoga->dscha, sizeof(hoga->dscha)), sizeDiff);

	m_items.GetAt(infoMGJV)->m_data = format(CString(hoga->mgjv, sizeof(hoga->mgjv)), infoMGJV);
	m_items.GetAt(infoMGDF)->m_data = format(CString(hoga->mgdf, sizeof(hoga->mgdf)), infoMGDF);
	m_items.GetAt(infoMGSI)->m_data = format(CString(hoga->mgsi, sizeof(hoga->mgsi)), infoMGSI);
	m_items.GetAt(infoMGKO)->m_data = format(CString(hoga->mgko, sizeof(hoga->mgko)), infoMGKO);
	m_items.GetAt(infoMGJE)->m_data = format(CString(hoga->mgje, sizeof(hoga->mgje)), infoMGJE);

	m_items.GetAt(infoBASE)->m_data = format(CString(hoga->base, sizeof(hoga->base)), infoBASE);

	m_items.GetAt(infoRATE)->m_data = format(CString(hoga->rate, sizeof(hoga->rate)), infoRATE);
	m_items.GetAt(infoIRGA)->m_data = format(CString(hoga->irga, sizeof(hoga->irga)), infoIRGA);
	m_items.GetAt(infoNJBD)->m_data = format(CString(hoga->njbd, sizeof(hoga->njbd)), infoNJBD);
	m_items.GetAt(infoDELT)->m_data = format(CString(hoga->delt, sizeof(hoga->delt)), infoDELT);
	m_items.GetAt(infoGAMM)->m_data = format(CString(hoga->gamm, sizeof(hoga->gamm)), infoGAMM);

	m_items.GetAt(infoRSH)->m_data = format(CString(hoga->msha, sizeof(hoga->msha)), infoRSH);
	m_items.GetAt(infoRHH)->m_data = format(CString(hoga->mhha, sizeof(hoga->mhha)), infoRHH);

	CString sAskTotalBefore,sBidTotalBefore;
	sAskTotalBefore = m_items.GetAt(askTotalBefore)->m_data;
	sAskTotalBefore.Remove(',');
	
	sBidTotalBefore = m_items.GetAt(bidTotalBefore)->m_data;
	sBidTotalBefore.Remove(',');
	
	const	int	nATB = atoi(sAskTotalBefore);
	const	int	nBTB = atoi(sBidTotalBefore);
	CString gunsu;

	if (nBTB > nATB)
		gunsu.Format("+%d",nBTB - nATB);
	else if (nBTB < nATB)
		gunsu.Format("%d",nBTB - nATB);
	else if (nBTB == nATB)
		gunsu = "0";
	
	m_items.GetAt(infoGUNSUCHA)->m_data = format(gunsu, infoGUNSUCHA);

	m_sum4 = atof(CString(hoga->sum4, sizeof(hoga->sum4))) * 0.01;
	m_sum9 = atof(CString(hoga->sum9, sizeof(hoga->sum9))) * 0.01;

	if (str2double(m_items.GetAt(prePrice)->m_data))
		m_showPreValue = true;
	else
		m_showPreValue = false;

	setItemRect(); calculatePercent(); getMaxSize();

	Invalidate();
}

void WriteFile(char* pBytes, int nBytes)
{
	CFile	Dfile;
	CString	filename = "c:\\hoga.log";
	const	BOOL	bExist = Dfile.Open(filename, CFile::modeReadWrite | CFile::shareDenyNone);

	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate|CFile::modeReadWrite | CFile::shareDenyNone);
		Dfile.Write(pBytes, nBytes);
	}
	else
	{
		const	ULONGLONG	pos = Dfile.SeekToEnd();
		Dfile.Seek(pos, CFile::begin);
		Dfile.Write(pBytes, nBytes);
	}
	Dfile.Close();
}

void Cdepth::alert(CString data)
{
	CRect	rect;
	CString	syms, text;
	int	pos = 0, index = 0;
	bool	changePrice = false;
	bool	changeSize  = false;
	bool	changeCurr  = false;
	bool	changePre   = false;
	bool	changeMgvlx = false;
	std::shared_ptr<Citem>	item = nullptr;

	rect.SetRectEmpty();
	for (int ii = 0; !data.IsEmpty(); ii++)
	{
		pos = data.Find('\t');
		if (pos != -1)
		{
			text = data.Left(pos++);
			data = data.Mid(pos);
		}
		else
		{
			text = data;
			data = _T("");
		}

		if (ii % 2)
		{
			if (syms.IsEmpty() || text.IsEmpty())
				continue;

			if (atoi(syms) == SHG9)
				syms = syms;

			if (!m_indices.Lookup(atoi(syms), index))
				continue;

			if (atoi(syms) == CURR)
			{
				m_curr = fabs(str2double(text));
				changeCurr = true;
				rect.UnionRect(rect, CRect(0, 0, 10, 10));
			}

			item = m_items.GetAt(index);
			text = format(text, index);

			if (atoi(syms) == DHCJ)
			{
				CString pre, cur, dat;
				pre = item->m_data;
				pre.Remove(',');
				cur = text;
				cur.Remove(',');
				
				dat.Format("%d", atoi(cur) - atoi(pre));
				if (atoi(dat) > 0)	dat = "+" + dat;
				m_items.GetAt(infoMDSD)->m_data = format(dat, infoMDSD);
			}
			else if (atoi(syms) == SHCJ)
			{
				CString pre, cur, dat;
				pre = item->m_data;
				pre.Remove(',');
				cur = text;
				cur.Remove(',');
				
				dat.Format("%d", atoi(cur) - atoi(pre));
				if (atoi(dat) > 0)	dat = "+" + dat;
				m_items.GetAt(infoMSSD)->m_data = format(dat, infoMSSD);
			}
			else if (atoi(syms) == MGJY)
			{
				std::shared_ptr<Citem>	pItem = nullptr;
				double	koga = 0, jega = 0, jjga = 0;
				CString value;

				jjga = str2double(text);
				
				pItem = m_items.GetAt(infoMGKO);
				koga = str2double(pItem->m_data);
				if (jjga > koga)
				{
					koga = jjga;
					value.Format("%.f", koga);
					pItem->m_data = format(value, infoMGKO);
				}

				pItem = m_items.GetAt(infoMGJE);
				jega = str2double(pItem->m_data);
				if (jjga < jega)
				{
					jega = jjga;
					value.Format("%.f", jega);
					pItem->m_data = format(value, infoMGKO);
				}
			}
			if (item->m_data.CompareNoCase(text))
			{
				item->m_data = text;

				rect.UnionRect(rect, item->m_fRc);
				if ((index >= askPrice1 && index <= askPrice10) || (index >= bidPrice1 && index <= bidPrice10))
					changePrice = true;
				else if ((index >= askSize1 && index <= askSize10) || (index >= bidSize1 && index <= bidSize10))
					changeSize = true;
				else if (index >= prePrice && index <= preVolume)
					changePre = true;
// updateX_20060309
				else if (index >= infoCURR && index <= infoGVOL)
					changeMgvlx = true;
			}
		}
		else
			syms = text;
	}

	if ((changePrice || changeCurr) && m_percent)
		rect.UnionRect(rect, getPercentRect());

	if (changePrice && changeCurr)
		rect.UnionRect(rect, m_prect);

// updateX_20060309
	if (changeMgvlx)
		rect.UnionRect(rect, m_rcInfoX);

	if (changeSize && (m_bar == grBar || m_bar == grBarText))
		getMaxSize();

	if (!rect.IsRectEmpty())
		InvalidateRgn(m_pRgn.get());
}

void Cdepth::alert(struct _alertR* alertR)
{
	CRect	rect;
	CString	syms, text;
	bool	changePrice = false;
	bool	changeSize  = false;
	bool	changeCurr  = false;
	bool	changePre   = false;
	bool	changeMgvlx = false;
	bool	changeTotalSize = false;
	bool	changeRSHH  = false;
	int	index = 0;
	DWORD*	data = nullptr;
	std::shared_ptr<Citem>	item = nullptr;

	rect.SetRectEmpty();
	data = (DWORD*)alertR->ptr[0];
	
	for (int jj = 0; jj < 1000; jj++)
	{
		if (data[jj])
			text = (char*)data[jj];
		else
			text = "";
		
		if (text.IsEmpty() || text == "")
			continue;
		
		if (jj == SHG9)
			syms = syms;
		
		if (!m_indices.Lookup(jj, index))
			continue;
		
		if (jj == CURR)
		{
			m_curr = fabs(str2double(text));
			changeCurr = true;
			rect.UnionRect(rect, CRect(0, 0, 10, 10));
		}
		
		item = m_items.GetAt(index);
		text = format(text, index);
		
		if (jj == DHCJ)
		{
			CString pre, cur, dat;
			pre = item->m_data;
			pre.Remove(',');
			cur = text;
			cur.Remove(',');
			
			dat.Format("%d", atoi(cur) - atoi(pre));
			if (atoi(dat) > 0)	dat = "+" + dat;
			m_items.GetAt(infoMDSD)->m_data = format(dat, infoMDSD);
		}
		else if (jj == SHCJ)
		{
			CString pre, cur, dat;
			pre = item->m_data;
			pre.Remove(',');
			cur = text;
			cur.Remove(',');
			
			dat.Format("%d", atoi(cur) - atoi(pre));
			if (atoi(dat) > 0)	dat = "+" + dat;
			m_items.GetAt(infoMSSD)->m_data = format(dat, infoMSSD);
		}
		else if (jj == MGJY)
		{
			std::shared_ptr<Citem>	pItem = nullptr;
			double	koga = 0, jega = 0, jjga = 0;
			CString value;
			
			jjga = str2double(text);
			pItem = m_items.GetAt(infoMGKO);
			koga = str2double(pItem->m_data);
			if (jjga > koga)
			{
				koga = jjga;
				value.Format("%.f", koga);
				pItem->m_data = format(value, infoMGKO);
			}
			
			pItem = m_items.GetAt(infoMGJE);
			jega = str2double(pItem->m_data);
			if (jjga < jega)
			{
				jega = jjga;
				value.Format("%.f", jega);
				pItem->m_data = format(value, infoMGKO);
			}
		}
		if (item->m_data.CompareNoCase(text))
		{
			item->m_data = text;
			
			rect.UnionRect(rect, item->m_fRc);
			if ((index >= askPrice1 && index <= askPrice10) || (index >= bidPrice1 && index <= bidPrice10))
				changePrice = true;
			else if ((index >= askSize1 && index <= askSize10) || (index >= bidSize1 && index <= bidSize10))
				changeSize = true;
			else if (index >= prePrice && index <= preVolume)
				changePre = true;
			// updateX_20060309
			else if (index >= infoCURR && index <= infoGVOL)
				changeMgvlx = true;
			else if (index == askTotalBefore || index == bidTotalBefore)
				changeTotalSize = true;
			else if (index == infoRSH || index == infoRHH)
				changeRSHH = true;
		}
	}

	if (changeTotalSize)
	{
		CString sAskTotalBefore,sBidTotalBefore;
		
		sAskTotalBefore = m_items.GetAt(askTotalBefore)->m_data;
		sAskTotalBefore.Remove(',');
		
		sBidTotalBefore = m_items.GetAt(bidTotalBefore)->m_data;
		sBidTotalBefore.Remove(',');
		
		const	int	nATB = atoi(sAskTotalBefore);
		const	int	nBTB = atoi(sBidTotalBefore);
		CString gunsu;

		if (nBTB > nATB)
			gunsu.Format("+%d",nBTB - nATB);
		else if (nBTB < nATB)
			gunsu.Format("%d",nBTB - nATB);
		else if (nBTB == nATB)
			gunsu = "0";
		
		m_items.GetAt(infoGUNSUCHA)->m_data = format(gunsu, infoGUNSUCHA);
	}

	if ((changePrice || changeCurr) && m_percent)
		rect.UnionRect(rect, getPercentRect());
	
	if (changePrice && changeCurr)
		rect.UnionRect(rect, m_prect);
	
	// updateX_20060309
	if (changeMgvlx || changeRSHH)
		rect.UnionRect(rect, m_rcInfoX);
	
	if (changeSize && (m_bar == grBar || m_bar == grBarText))
		getMaxSize();
	
	if (!rect.IsRectEmpty())
		InvalidateRgn(m_pRgn.get());
}

void Cdepth::freeItems()
{
	std::shared_ptr<Citem>	item = nullptr;

	for (int ii = 0; ii < m_items.GetSize(); ii++)
	{
		item = m_items.GetAt(ii);
		item->m_data.Empty();
		item.reset();
	}
	m_items.RemoveAll();
}

void Cdepth::clear()
{
	for (int ii = 0; ii < N_DATA; ii++)
		m_items.GetAt(ii)->m_data.Empty();

	EW_Clear();
}

void Cdepth::draw(CDC* dc)
{
	CRect	rect;
	CBitmap	bitmap, *pbitmap;
	CDC	memDC;

	GetClientRect(&rect);
	bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());

	memDC.CreateCompatibleDC(dc);
	pbitmap = memDC.SelectObject(&bitmap);
	drawView(&memDC);
	dc->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pbitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void Cdepth::drawItems(CDC* dc)
{
	UINT	style = 0;
	std::shared_ptr<Citem>	item = nullptr;
	const	int	ndc = dc->SaveDC();
	CFont* font = getFont(m_point, m_fonts, FW_BOLD);

	dc->SetBkMode(TRANSPARENT);
	if (m_bGradient)
	{
		CRect	rc, rcx;
		std::shared_ptr<Citem>	sItem = m_items.GetAt(m_idxMDS);
		std::shared_ptr<Citem>	eItem = m_items.GetAt(m_idxMDE);

		rc.SetRect(sItem->m_bRc.left, eItem->m_bRc.top, eItem->m_bRc.right, sItem->m_bRc.bottom);

		int	rs = GetRValue(m_clrAskGs);
		int	gs = GetGValue(m_clrAskGs);
		int	bs = GetBValue(m_clrAskGs);
		int	re = GetRValue(m_clrAskGe);
		int	ge = GetGValue(m_clrAskGe);
		int	be = GetBValue(m_clrAskGe);

		float	rStep = float(rs - re)/float(rc.Height());
		float	gStep = float(gs - ge)/float(rc.Height());
		float	bStep = float(bs - be)/float(rc.Height());

		for (int ii = 0; ii < rc.Height(); ii++)
		{
			rcx.SetRect(rc.left, rc.top + ii, rc.right, rc.top + ii + 1);
			dc->FillSolidRect(&rcx, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
		}

		sItem = m_items.GetAt(m_idxMSS);
		eItem = m_items.GetAt(m_idxMSE);
		rc.SetRect(sItem->m_bRc.left, sItem->m_bRc.top, eItem->m_bRc.right, eItem->m_bRc.bottom);

		rs = GetRValue(m_clrBidGs);
		gs = GetGValue(m_clrBidGs);
		bs = GetBValue(m_clrBidGs);
		re = GetRValue(m_clrBidGe);
		ge = GetGValue(m_clrBidGe);
		be = GetBValue(m_clrBidGe);

		rStep = float(rs - re)/float(rc.Height());
		gStep = float(gs - ge)/float(rc.Height());
		bStep = float(bs - be)/float(rc.Height());

		rc.SetRect(sItem->m_bRc.left, sItem->m_bRc.top, eItem->m_bRc.right, eItem->m_bRc.bottom);

		for (int ii = 0; ii < rc.Height(); ii++)
		{
			rcx.SetRect(rc.left, rc.top + ii, rc.right, rc.top + ii + 1);
			dc->FillSolidRect(&rcx, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
		}
	}

	m_rcCurr.SetRectEmpty();
	for (int ii = askPrice1; ii <= infoMSSD; ii++)
	{
		dc->SelectObject(m_font);
		item = m_items.GetAt(ii);

		if (!m_showBeforeDiff)
		{
			if ((ii >= askBefore1 && ii <= askBefore10) || (ii >= bidBefore1 && ii <= bidBefore10))
				continue;
		}
		
		if (m_focus == ii)
		{
			item->m_pRGB = m_clrFocus;
		}

		if (!m_bGradient) dc->FillSolidRect(item->m_bRc, item->m_pRGB);

		if ((ii >= askPrice1 && ii <= askPrice10) || (ii >= bidPrice1 && ii <= bidPrice10))
		{
			switch (m_type)
			{
			default:
			case ctFuture:
			case ctOption:
			case ctKOFEX:
				if (m_curr != 0 && m_curr == str2double(item->m_data) && m_bCurr)
					m_rcCurr.SetRect(m_columns[0]+1, item->m_bRc.top-1, m_columns[1]+1, item->m_bRc.bottom);
				break;
			}
		}

		if ((ii >= askSize1 && ii <= askSize10) || (ii >= bidSize1 && ii <= bidSize10)
				|| ii == askTotalSize || ii == bidTotalSize)
		{
			if (m_bar == grBarText || m_bar == grBar)
			{
				if (m_maxsize)
					drawGraph(dc, ii);
				if (m_bar == grBar)
					continue;
			}
		}

		dc->SetTextColor(item->m_fRGB);
		style = DT_SINGLELINE|DT_VCENTER;
		if (item->m_center)
			style |= DT_CENTER;
		else
			style |= DT_RIGHT;

		if ((ii >= askPrice1 && ii <= askPrice10) || (ii >= bidPrice1 && ii <= bidPrice10))
		{
			if (m_bBoldPrice)
				dc->SelectObject(font);
			if (true || atof(m_items.GetAt(ii+20)->m_data) > 0)
				dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else if ((ii >= askPercent1 && ii <= askPercent10) || (ii >= bidPercent1 && ii <= bidPercent10))
		{
			if (false && m_bBoldPrice)
				dc->SelectObject(font);
			if (true || atof(m_items.GetAt(ii+10)->m_data) > 0)
				dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else if ((ii>=askSize1 && ii<=askSize10) || (ii>=bidSize1 && ii<=bidSize10))
		{
			if (m_bBoldRemain)
				dc->SelectObject(font);
			dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else if ((ii>=askBefore1 && ii<=askBefore10) || (ii>=bidBefore1 && ii<=bidBefore10))
		{
			if (m_bBoldCnt)
				dc->SelectObject(font);
			dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else
			dc->DrawText(item->m_data, item->m_fRc, style);
	}

	item = m_items.GetAt(infoGUNSUCHA);
	
	dc->SetTextColor(item->m_fRGB);
	
	style = DT_SINGLELINE|DT_VCENTER;
	style |= DT_CENTER;
	
	dc->DrawText(item->m_data, item->m_fRc, style);

	dc->RestoreDC(ndc);
}

void Cdepth::drawBong(CDC* dc)
{
	CString tmpS;
	std::shared_ptr<Citem>	pItem = nullptr;

	double	siga = 0, koga = 0, jega = 0, shga = 0, hhga = 0, pivot = 0;
	pItem = m_items.GetAt(openPrice);
	siga = str2double(pItem->m_data);
	pItem = m_items.GetAt(highPrice);
	koga = str2double(pItem->m_data);
	pItem = m_items.GetAt(lowPrice);
	jega = str2double(pItem->m_data);
	pItem = m_items.GetAt(shPrice);
	shga = str2double(pItem->m_data);	
	pItem = m_items.GetAt(hhPrice);
	hhga = str2double(pItem->m_data);
	pItem = m_items.GetAt(pivotPrice);
	pivot = str2double(pItem->m_data);

	if (siga == 0 || koga == 0 || jega == 0 || shga == 0 || hhga == 0)// || pivot == 0)
		return;	

	const	int	ndc = dc->SaveDC();
	dc->SetBkMode(TRANSPARENT);

	CPen*	pen = nullptr;
	if (siga < m_curr)
		pen = getPen(m_clrUp, 1);
	else
		pen = getPen(m_clrDown, 1);
	CPen*	pPen = dc->SelectObject(pen);

	pItem = m_items.GetAt(askBong);
	CRect rc = pItem->m_bRc;
	rc.bottom = rc.top + rc.Height() / 2;
	rc.DeflateRect(0, 2);

	const	int	sigaY = (int)((rc.Height() * (koga - siga)) / (koga - jega)/*(shga - hhga)*/);
	const	int	kogaY = (int)((rc.Height() * (koga - koga)) / (koga - jega)/*(shga - hhga)*/);
	const	int	jegaY = (int)((rc.Height() * (koga - jega)) / (koga - jega)/*(shga - hhga)*/);
	const	int	jjgaY = (int)((rc.Height() * (koga - m_curr)) / (koga - jega)/*(shga - hhga)*/);
	const	int	pivotY = (int)((rc.Height() * (koga - pivot)) / (koga - jega)/*(shga - hhga)*/);

	dc->Rectangle(CRect(rc.left + 1, rc.top + sigaY, rc.right - 1, rc.top + jjgaY));
	dc->FillSolidRect(CRect(rc.left + 1, rc.top + sigaY, rc.right - 1, rc.top + jjgaY), 
		siga < m_curr ? m_clrUp : m_clrDown);
	dc->MoveTo(rc.CenterPoint().x, rc.top + kogaY);
	dc->LineTo(rc.CenterPoint().x, rc.top + jegaY);
	
	pen = getPen(RGB(100, 100, 100), 1);
	dc->SelectObject(pen);
	dc->MoveTo(rc.left + 1, rc.top + pivotY);
//	dc->LineTo(rc.right - 1, rc.top + pivotY);

	dc->SelectObject(pPen);
	dc->RestoreDC(ndc);
}

void Cdepth::drawBongMGVL(CDC* dc)
{
	CString tmpS;
	std::shared_ptr<Citem>	pItem = nullptr;
	double	siga = 0, koga = 0, jega = 0, jjga = 0;

	pItem = m_items.GetAt(infoMGSI);
	siga = str2double(pItem->m_data);
	pItem = m_items.GetAt(infoMGKO);
	koga = str2double(pItem->m_data);
	pItem = m_items.GetAt(infoMGJE);
	jega = str2double(pItem->m_data);
	pItem = m_items.GetAt(infoMGJV);
	jjga = str2double(pItem->m_data);

	if (siga == 0 || koga == 0 || jega == 0 || jjga == 0)
		return;	

	const	int	ndc = dc->SaveDC();
	dc->SetBkMode(TRANSPARENT);

	CPen*	pen = nullptr;
	if (siga < jjga)
		pen = getPen(m_clrUp, 1);
	else
		pen = getPen(m_clrDown, 1);
	CPen*	pPen = dc->SelectObject(pen);

	pItem = m_items.GetAt(askBong);
	CRect rc = pItem->m_bRc;
	rc.top = rc.top + rc.Height() / 2;

	const	int	sigaY = (int)((rc.Height() * (koga - siga)) / (koga - jega));
	const	int	kogaY = (int)((rc.Height() * (koga - koga)) / (koga - jega));
	const	int	jegaY = (int)((rc.Height() * (koga - jega)) / (koga - jega));
	const	int	jjgaY = (int)((rc.Height() * (koga - jjga)) / (koga - jega));

	dc->Rectangle(CRect(rc.left + 1, rc.top + sigaY, rc.right - 1, rc.top + jjgaY));
	dc->FillSolidRect(CRect(rc.left + 1, rc.top + sigaY, rc.right - 1, rc.top + jjgaY), 
		siga < jjga ? m_clrUp : m_clrDown);
	dc->MoveTo(rc.CenterPoint().x, rc.top + kogaY);
	dc->LineTo(rc.CenterPoint().x, rc.top + jegaY);
	
	dc->SelectObject(pPen);
	dc->RestoreDC(ndc);
}

void Cdepth::drawInfo(CDC* dc)
{
	const	int	ndc = dc->SaveDC();
	CRect	rect;
	CString tmpS;

	if (m_showOHLC == infoMGVL)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		const	int	offset = m_showTop ? m_topH+1 : 1;
		rect = m_rcInfo; rect.DeflateRect(2, 0);
		for (int ii = 0; ii < m_depth; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 0:
				dc->DrawText(_T("미결제"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoMGJV)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoMGJV)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 1:
				dc->DrawText(_T("  증감"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoMGDF)->m_fRGB);
				tmpS = m_items.GetAt(infoMGDF)->m_data;

				if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
					tmpS.Delete(0);
				dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 2:
				dc->DrawText(_T("  시가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(openPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 3:
				dc->DrawText(_T("  고가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(highPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 4:
				dc->DrawText(_T("  저가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(lowPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 5:
				dc->DrawText(_T("실시간상"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoRSH)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoRSH)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 6:
				dc->DrawText(_T("실시간하"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoRHH)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoRHH)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 7:
				dc->DrawText(_T("기준가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(gijunPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(gijunPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 8:
				dc->DrawText(_T(" Basis"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoBASE)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoBASE)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 9:
				dc->DrawText(_T("괴리율"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoRATE)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoRATE)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}
	}
// updateX_20060309, OnStart시 setting
	else if (m_showOHLC == infoMGVLX)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		const	int	offset = m_showTop ? m_topH+1 : 1;
		rect = m_rcInfo; rect.DeflateRect(2, 0);
		
		for (int ii = 0; ii < 5; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 0:
				dc->DrawText(_T("미결제"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoMGJV)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoMGJV)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 1:
				dc->DrawText(_T("  증감"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoMGDF)->m_fRGB);
				tmpS = m_items.GetAt(infoMGDF)->m_data;

				if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
					tmpS.Delete(0);
				dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 2:
				dc->DrawText(_T("  시가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(openPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 3:
				dc->DrawText(_T("  고가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(highPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 4:
				dc->DrawText(_T("  저가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(lowPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}

		rect = m_rcInfoX; rect.DeflateRect(2, 0);
		
		for (int ii = 5; ii < 10; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 5:
				dc->DrawText(_T("현재가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoCURR)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoCURR)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 6:
				dc->DrawText(_T("  대비"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoDIFF)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoDIFF)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 7:
				dc->DrawText(_T("등락률"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoUDYL)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoUDYL)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 8:
				dc->DrawText(_T("거래량"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoGVOL)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoGVOL)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}
	}
	else if (m_showOHLC == infoSHGA && m_type == ctFuture)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		const	int	offset = m_showTop ? m_topH+1 : 1;
		rect = m_rcInfo; rect.DeflateRect(2, 0);

		CString strStr, strSign, strPrice, strText, strGijun = m_items.GetAt(gijunPrice)->m_data;
		strGijun.Replace(",","");

		for (int ii = 0; ii < m_depth; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 0:
				dc->DrawText(_T("미결제"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoMGJV)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoMGJV)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 1:
				dc->DrawText(_T("  증감"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoMGDF)->m_fRGB);
				tmpS = m_items.GetAt(infoMGDF)->m_data;
				
				if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
					tmpS.Delete(0);
				dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 2:
				dc->DrawText(_T("  시가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(openPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 3:	//시가대비
				dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
				strPrice = m_items.GetAt(openPrice)->m_data;
				strPrice.Replace(",", "");
				strStr.Format("%d", atoi(strPrice) - atoi(strGijun));
				strSign = getSign(strStr);
				addComma(strStr);
				strStr.Insert(0, strSign);
				strText.Format("(%7s)", strStr);
				dc->DrawText(strText, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 4:
				dc->DrawText(_T("  고가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(highPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 5:	//고가대비
				dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
				strPrice = m_items.GetAt(highPrice)->m_data;
				strPrice.Replace(",", "");
				strStr.Format("%d", atoi(strPrice) - atoi(strGijun));
				strSign = getSign(strStr);
				addComma(strStr);
				strStr.Insert(0, strSign);
				strText.Format("(%7s)", strStr);
				dc->DrawText(strText, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 6:
				dc->DrawText(_T("  저가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(lowPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 7: //저가대비
				dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
				strPrice = m_items.GetAt(lowPrice)->m_data;
				strPrice.Replace(",", "");
				strStr.Format("%d", atoi(strPrice) - atoi(strGijun));
				strSign = getSign(strStr);
				addComma(strStr);
				strStr.Insert(0, strSign);
				strText.Format("(%7s)", strStr);
				dc->DrawText(strText, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 8:
				dc->DrawText(_T("예상가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(prePrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(prePrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 9:	//예상가대비
				dc->SetTextColor(m_items.GetAt(prePrice)->m_fRGB);
				strPrice = m_items.GetAt(prePrice)->m_data;
				strPrice.Replace(",", "");
				strStr.Format("%d", atoi(strPrice) - atoi(strGijun));
				strSign = getSign(strStr);
				addComma(strStr);
				strStr.Insert(0, strSign);
				strText.Format("(%7s)", strStr);
				dc->DrawText(strText, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}
	}
	else if (m_showOHLC == infoSHGA && m_type == ctOption)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		const	int	offset = m_showTop ? m_topH+1 : 1;
		rect = m_rcInfo; rect.DeflateRect(2, 0);
		
		for (int ii = 0; ii < 5; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 0:
				dc->DrawText(_T("기준가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(gijunPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(gijunPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 1:
				dc->DrawText(_T("이론가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoIRGA)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoIRGA)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 2:
				dc->DrawText(_T("    IV"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoNJBD)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoNJBD)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 3:
				dc->DrawText(_T("  델타"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoDELT)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoDELT)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 4:
				dc->DrawText(_T("  감마"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoGAMM)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoGAMM)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}
	}
	else if (m_showOHLC == infoRSHGA)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		const	int	offset = m_showTop ? m_topH+1 : 1;
		rect = m_rcInfo; rect.DeflateRect(2, 0);
		
		for (int ii = 0; ii < 2; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));
			CString s;
			switch (ii)
			{
			case 0:
				dc->DrawText(_T("실시간상"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoRSH)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoRSH)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 1:
				dc->DrawText(_T("실시간하"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoRHH)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoRHH)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}
	}
// updateX_20060203
	else if (m_showOHLC == infoSHGAX && m_type == ctFuture)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		const	int	offset = m_showTop ? m_topH+1 : 1;

		rect = m_rcInfo; rect.DeflateRect(2, 0);

		for (int ii = 0; ii < 3; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 0:
				continue;
			case 1:
				dc->DrawText(_T(" Basis"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoBASE)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoBASE)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 2:
				dc->DrawText(_T("이론가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(infoIRGA)->m_fRGB);
				dc->DrawText(m_items.GetAt(infoIRGA)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}

		rect = m_rcInfoX; rect.DeflateRect(2, 0);
		
		for (int ii = 5; ii < 10; ii++)
		{
			dc->SetTextColor(m_clrDataFg);
			rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

			switch (ii)
			{
			case 5:
				dc->DrawText(_T("상한가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(shPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(shPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 6:
				dc->DrawText(_T("  시가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(openPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 7:
				dc->DrawText(_T("  고가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(highPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 8:
				dc->DrawText(_T("  저가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(lowPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			case 9:
				dc->DrawText(_T("하한가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
				dc->SetTextColor(m_items.GetAt(hhPrice)->m_fRGB);
				dc->DrawText(m_items.GetAt(hhPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
				break;
			}
		}
	}
	dc->RestoreDC(ndc);
}

void Cdepth::drawGraph(CDC* dc, int index)
{
	CRect	rect;
	std::shared_ptr<Citem>	item = nullptr, itemX = nullptr;
// updateX_20060119
	int	marginT = 0, marginB = 0;
	
	item = m_items.GetAt(index);
	if (index >= askSize1 && index <= askSize10)
	{
		rect = item->m_fRc;
		if (m_showBeforeDiff)
		{
			itemX =  m_items.GetAt(index + 10);
			rect.left = itemX->m_fRc.left;
		}

		marginT = rect.Height() - (m_fontH + 2);
		marginB = 2;
		if (marginT > 0)
		{
			marginB += (marginT % 2);
			marginT /= 2; marginB += marginT;
		}
		rect.DeflateRect(0, marginT, 0, marginB);

		rect.left = rect.right - (int)(((double)rect.Width() * str2double(item->m_data))/m_maxsize);
//		rect.IntersectRect(rect, item->m_bRc);
		dc->FillSolidRect(rect, m_clrBarAsk);
	}
	else if (index >= bidSize1 && index <= bidSize10)
	{
		rect = item->m_fRc;
		if (m_showBeforeDiff)
		{
			itemX =  m_items.GetAt(index + 10);
			rect.right = itemX->m_fRc.right;
		}

		marginT = rect.Height() - (m_fontH + 2);
		marginB = 2;
		if (marginT > 0)
		{
			marginB += (marginT % 2);
			marginT /= 2; marginB += marginT;
		}
		rect.DeflateRect(0, marginT, 0, marginB);

		rect.right = rect.left + (int)(((double)rect.Width() * str2double(item->m_data))/m_maxsize);
//		rect.IntersectRect(rect, item->m_bRc);
		dc->FillSolidRect(rect, m_clrBarBid);
	}
	
	if (m_bottom == btSize || m_bottom == btSizeTime)
	{
		double	ask = 0, bid = 0;
		
		if (index == askTotalSize)
		{
			rect = item->m_fRc;
			if (m_showBeforeDiff)
			{
				itemX =  m_items.GetAt(askTotalBefore);
				rect.left = itemX->m_fRc.left;
			}

			marginT = rect.Height() - (m_fontH + 2);
			marginB = 2;
			if (marginT > 0)
			{
				marginB += (marginT % 2);
				marginT /= 2; marginB += marginT;
			}
			rect.DeflateRect(0, marginT, 0, marginB);
			
			ask = str2double(item->m_data);
			if (ask > 0)
			{
				bid = str2double(m_items.GetAt(bidTotalSize)->m_data);
				if (ask < bid)
				{
					rect.left = rect.right - (int)(((double)rect.Width() * ask)/bid);
//					rect.IntersectRect(rect, item->m_bRc);
				}
				dc->FillSolidRect(rect, m_clrBarAsk);
			}
		}
		else if (index == bidTotalSize)
		{
			rect = item->m_fRc;
			if (m_showBeforeDiff)
			{
				itemX =  m_items.GetAt(bidTotalBefore);
				rect.right = itemX->m_fRc.right;
			}

			marginT = rect.Height() - (m_fontH + 2);
			marginB = 2;
			if (marginT > 0)
			{
				marginB += (marginT % 2);
				marginT /= 2; marginB += marginT;
			}
			rect.DeflateRect(0, marginT, 0, marginB);
			
			bid = str2double(item->m_data);
			if (bid > 0)
			{
				ask = str2double(m_items.GetAt(askTotalSize)->m_data);
				if (bid < ask)
				{
					rect.right = rect.left + (int)(((double)rect.Width() * bid)/ask);
//					rect.IntersectRect(rect, item->m_bRc);
				}
				dc->FillSolidRect(rect, m_clrBarBid);
			}
		}
	}
}

void Cdepth::drawHead(CDC* dc)
{
	CRect	rect;
	const	int	ndc = dc->SaveDC();

	dc->SelectObject(m_font);
	dc->SetTextColor(m_clrHeadFg);
	dc->SetBkMode(TRANSPARENT);

	switch (m_type)
	{
	case ctFuture:
	case ctOption:
	case ctKOption:
		if (m_showBeforeDiff)
		{
			rect.SetRect(0, 1, m_columns[0], (int)(m_topH+1));
			rect.left = RATERECTSEL(rect);
			dc->DrawText(_T("매도"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			
			rect.SetRect(0, 1, m_columns[0], (int)(m_topH + 1));
			rect.right = RATERECTSEL(rect);
			if (m_bConfig)
			{
				const	int	topOff = (rect.Height() - imgCY1) / 2;
				m_rcConfigL.SetRect(rect.right - (imgCX1+2), rect.top + topOff, rect.right - 2, rect.top + topOff + imgCY1);
				rect.DeflateRect(3, 1, 20, 1);
			}
			dc->DrawText(_T("건수"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			
			rect.SetRect(m_columns[0] + 1, 1, m_columns[1], (int)(m_topH + 1));
			
			if (m_bConfig)
			{
				const	int	topOff = (rect.Height() - imgCY1) / 2;
				m_rcConfig.SetRect(rect.right - (imgCX1+2), rect.top + topOff, rect.right - 2, rect.top + topOff + imgCY1);
				rect.DeflateRect(3, 1, 20, 1);
			}

			std::shared_ptr<Citem>	item = m_items.GetAt(cTime);
			dc->DrawText(item->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			rect.right = RATERECTBUY(rect);
			dc->DrawText(_T("매수"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			rect.left = RATERECTBUY(rect);
			if (m_bConfig)
			{
				const	int	topOff = (rect.Height() - imgCY1) / 2;
				m_rcConfigR.SetRect(rect.right - (imgCX1+2), rect.top + topOff, rect.right - 2, rect.top + topOff + imgCY1);
				rect.DeflateRect(3, 1, 20, 1);
			}
			dc->DrawText(_T("건수"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		}
		else
		{
			rect.SetRect(0, 1, m_columns[0], (int)(m_topH + 1));
			if (m_bConfig)
			{
				m_rcConfigL.SetRect(rect.right - 20, rect.top, rect.right - 2, rect.bottom);
				rect.DeflateRect(3, 1, 20, 1);
			}
			dc->DrawText(_T("매도"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			
			rect.SetRect(m_columns[0] + 1, 1, m_columns[1], (int)(m_topH + 1));
			if (m_bConfig)
			{
				const	int	topOff = (rect.Height() - imgCY1) / 2;
				m_rcConfig.SetRect(rect.right - (imgCX1+2), rect.top + topOff, rect.right - 2, rect.top + topOff + imgCY1);
				rect.DeflateRect(3, 1, 20, 1);
			}
			dc->DrawText(_T("호가"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			if (m_bConfig)
			{
				m_rcConfigR.SetRect(rect.right - 20, rect.top, rect.right - 2, rect.bottom);
				rect.DeflateRect(3, 1, 20, 1);
			}
			dc->DrawText(_T("매수"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		}
	}
	dc->RestoreDC(ndc);
}

void Cdepth::drawBottom(CDC* dc)
{
	CRect	bRc, fRc;
	const	int	ndc = dc->SaveDC();
	const	int	top = m_showTop ? m_topH : 0;
	const	int	startL = m_depth*2;
	int	endL   = startL;

	switch (m_bottom)
	{
	case btSize:
	case btTime:
		endL += 2;	break;
	case btSizeTime:
		endL += 2;	break;
	}
	
	dc->SelectObject(m_font);
	dc->SetTextColor(m_clrHeadFg);
	dc->SetBkMode(TRANSPARENT);
	if (m_showTop)
	{
		switch (m_bottom)
		{
		case btNo:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1) + top, m_columns[1], (int)(m_rowH * (startL + 1)) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btSize:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (startL + 1) + 1) + top, m_columns[1], (int)(m_rowH * (startL + 2)) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				//dc->FillSolidRect(bRc, m_clrHeadBk);
				
				//dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				//DKKIM
				bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
				bRc.right = RATERECTSEL(bRc);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (startL + 1) + 1) + top, m_columns[1], (int)(m_rowH * (startL + 2)) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				fRc.DeflateRect(0, 0, 3, 0);
				//dc->FillSolidRect(bRc, m_clrHeadBk);
				
				//dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				//DKKIM
				bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
				bRc.right = RATERECTSEL(bRc);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				// 					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
				// 					bRc.left = RATERECTBUY(bRc) + 1;
				// 					fRc = bRc;
				// 					fRc.DeflateRect(3, 1);
				// 					dc->FillSolidRect(bRc, m_clrHeadBk);
				//dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1) + top, m_columns[1], (int)(m_rowH*endL) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				dc->FillSolidRect(bRc, m_clrBottom);

				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1) + top, m_columns[1], (int)(m_rowH*endL) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				fRc.DeflateRect(0, 0, 3, 0);
				dc->FillSolidRect(bRc, m_clrBottom);

				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btSizeTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1) + top, m_columns[1], (int)(m_rowH * endL) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				dc->FillSolidRect(bRc, m_clrBottom);
				
				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1) + top, m_columns[1], (int)(m_rowH * endL) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);

				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		}
	}
	else
	{
		switch (m_bottom)
		{
		case btNo:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1), m_columns[1], (int)(m_rowH * (startL + 1)));
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btSize:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (startL + 1) + 1), m_columns[1], (int)(m_rowH * (startL + 2)));
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				//dc->FillSolidRect(bRc, m_clrHeadBk);
				
				//dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				//DKKIM
				bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
				bRc.right = RATERECTSEL(bRc);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (startL + 1) + 1), m_columns[1], (int)(m_rowH * (startL + 2)));
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				//dc->FillSolidRect(bRc, m_clrHeadBk);
				
				//dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				//DKKIM
				bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
				bRc.right = RATERECTSEL(bRc);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				// 					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
				// 					bRc.left = RATERECTBUY(bRc) + 1;
				// 					fRc = bRc;
				// 					fRc.DeflateRect(3, 1);
				// 					dc->FillSolidRect(bRc, m_clrHeadBk);
				//dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1), m_columns[1], (int)(m_rowH*endL));
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				
				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1), m_columns[1], (int)(m_rowH * endL));
				fRc = bRc;
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				
				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btSizeTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1), m_columns[1], (int)(m_rowH * endL));
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);
				
				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1), m_columns[1], (int)(m_rowH * endL));
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				fRc.DeflateRect(3, 1, 3, 1);
				dc->FillSolidRect(bRc, m_clrBottom);

				dc->DrawText(_T("직전"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		}
	}
	dc->RestoreDC(ndc);
}

void Cdepth::drawLine(CDC* dc)
{
	int	vertical = 0;
	const	int	top = m_showTop ? m_topH : 0;
	CPen*	pen = getPen(m_clrLine);
	CPen*	ppen = dc->SelectObject(pen);
	
	dc->MoveTo(0, 0);
	dc->LineTo(m_columns[2], 0);
	if (m_showTop)
	{
		dc->MoveTo(0, (int)m_topH);
		dc->LineTo(m_columns[2], (int)m_topH);	
	}
	dc->MoveTo(0, (int)(m_rowH * m_depth + top));
	dc->LineTo(m_columns[2], (int)(m_rowH * m_depth + top));
	
	dc->MoveTo(0, (int)(m_rowH * m_depth*2 + top));
	dc->LineTo(m_columns[2], (int)(m_rowH * m_depth*2 + top));
	
	switch (m_bottom)
	{
	case btNo:
		dc->MoveTo(0, (int)(m_rowH * m_depth*2 + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * m_depth*2 + top));
		vertical = m_depth*2;
		if (!m_showBeforeDiff)
		{
			dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 1) + top));
			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 1) + top));
			vertical++;
		}
		break;
		
	case btSize:
	case btTime:
// 		dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 1) + top));
// 		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 1) + top));
// 		vertical = m_depth*2 + 1;
// 		if (!m_showBeforeDiff)
// 		{
// 			dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 2) + top));
// 			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 2) + top));
// 			vertical++;
// 		}
		dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 1) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 1) + top));
		
		dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 2) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 2) + top));
		vertical = m_depth*2 + 2;
		if (!m_showBeforeDiff)
		{
			dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 3) + top));
			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 3) + top));
			vertical++;
		}
		break;
		
	case btSizeTime:
		dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 1) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 1) + top));
		
		dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 2) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 2) + top));
		vertical = m_depth*2 + 2;
		if (!m_showBeforeDiff)
		{
			dc->MoveTo(0, (int)(m_rowH * (m_depth*2 + 3) + top));
			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth*2 + 3) + top));
			vertical++;
		}
		break;
	}
	
	dc->MoveTo(0, 0);
	dc->LineTo(0, (int)(m_rowH * vertical + top));
	
	dc->MoveTo(m_columns[0], 0);
	dc->LineTo(m_columns[0], (int)(m_rowH * vertical + top));
	
	if (m_bBong)
	{
		dc->MoveTo(m_columns[1] + bongWIDTH, top);
		dc->LineTo(m_columns[1] + bongWIDTH, (int)(m_rowH * (m_depth) + top));
	}
	
	dc->MoveTo(m_columns[1], 0);
	dc->LineTo(m_columns[1], (int)(m_rowH * vertical + top));
	
	dc->MoveTo(m_columns[2], 0);
	dc->LineTo(m_columns[2], (int)(m_rowH * vertical + top + 1));
	
	if (m_showBeforeDiff)
	{
		CRect	rect;
		
		rect.SetRect(1, 1, m_columns[0], 2);
		const	int	left = RATERECTSEL(rect);
		
		rect.SetRect(m_columns[1] + 1, 1, m_columns[2], 2);
		const	int	right = RATERECTBUY(rect);
		
		if (m_showTop)
		{
			dc->MoveTo(left, 0);
			dc->LineTo(left, top);
			
			dc->MoveTo(right, 0);
			dc->LineTo(right, top);
			
			dc->MoveTo(left, top);
		}
		else
			dc->MoveTo(left, 0);
		
		dc->LineTo(left, (int)(m_rowH * m_depth + top));
		dc->MoveTo(right, (int)(m_rowH * m_depth + top));
		dc->LineTo(right, (int)(m_rowH * m_depth*2 + top));
		
		switch (m_bottom)
		{
		case btSize:
		case btTime:
// 			dc->MoveTo(left, (int)(m_rowH * m_depth*2 + top));
// 			dc->LineTo(left, (int)(m_rowH * (m_depth*2 + 1) + top));
// 			
// 			dc->MoveTo(right, (int)(m_rowH * m_depth*2 + top));
// 			dc->LineTo(right, (int)(m_rowH * (m_depth*2 + 1) + top));
			dc->MoveTo(left, (int)(m_rowH * m_depth*2 + top));
			dc->LineTo(left, (int)(m_rowH * (m_depth*2 + 2) + top));
			
			dc->MoveTo(right, (int)(m_rowH * m_depth*2 + top));
			dc->LineTo(right, (int)(m_rowH * (m_depth*2 + 2) + top));
			break;
			
		case btSizeTime:
			dc->MoveTo(left, (int)(m_rowH * m_depth*2 + top));
			dc->LineTo(left, (int)(m_rowH * (m_depth*2 + 2) + top));
			
			dc->MoveTo(right, (int)(m_rowH * m_depth*2 + top));
			dc->LineTo(right, (int)(m_rowH * (m_depth*2 + 2) + top));
			break;
		}
	}

	if (m_bCurr)
	{
		CRect	rect = m_rcCurr; rect.DeflateRect(0, 1, 1, 1);
		CBrush *pOldBrush = (CBrush*)dc->SelectStockObject(NULL_BRUSH);
		CPen *pOldPen = dc->SelectObject(getPen(m_clrCurr, 1));
		dc->Rectangle(rect);
		dc->SelectObject(pOldPen);
		dc->SelectObject(pOldBrush);	
	}	

	dc->SelectObject(ppen);
}

void Cdepth::drawView(CDC* dc)
{
	CFont*	font;
	
	drawBground(dc);
	
	font = dc->SelectObject(m_font);

	drawItems(dc);

	//if (m_showBeforeDiff)	// 20080426 khk
		drawInfo(dc);
	if (m_showLine)
		drawLine(dc);
	if (m_showTop)
		drawHead(dc);
	if (m_bBong)
	{
		drawBong(dc); drawBongMGVL(dc);
	}
	
	drawBottom(dc);
	drawButton(dc);
	drawBox(dc);
	
	dc->SelectObject(font);
}

void Cdepth::drawBground(CDC* dc)
{
	CRect	rect;
	const	int	top = m_showTop ? m_topH : 0;
	
	GetClientRect(rect); dc->FillSolidRect(rect, m_clrWhite);
	
	if (m_showTop)
	{
		rect.SetRect(0, 0, m_columns[2], (int)m_topH);
		dc->FillSolidRect(rect, m_clrHeadBk);
	}
	
	switch (m_bottom)
	{
	case btSize:
	case btTime:
// 		rect.SetRect(0, (int)(m_rowH * (m_depth*2) + top), m_columns[2], (int)(m_rowH * (m_depth*2 + 1) + top));
// 		dc->FillSolidRect(rect, m_clrBottom);
		rect.SetRect(0, (int)(m_rowH * (m_depth*2) + top), m_columns[2], (int)(m_rowH * (m_depth*2 + 2) + top));
		dc->FillSolidRect(rect, m_clrBottom);
		break;
		
	case btSizeTime:
		rect.SetRect(0, (int)(m_rowH * (m_depth*2) + top), m_columns[2], (int)(m_rowH * (m_depth*2 + 2) + top));
		dc->FillSolidRect(rect, m_clrBottom);
		break;
	}
}

void Cdepth::calculateColumnWidth()
{
	int	width;
	
	width = m_rect.Width()-1;
	double	x = 0;
	if (m_showBeforeDiff)
	{
		x = 0.34;
		m_columns[0] = (int)((double)width * x);
		m_columns[1] = (int)((double)width * (1.0 - x));
		m_columns[2] = width;
	}
	else
	{
		x = 0.34;
		m_columns[0] = (int)((double)width * x);
		m_columns[1] = (int)((double)width * (1.0 - x));
		m_columns[2] = width;
	}
}

void Cdepth::calculateRowHeight()
{
	int	divider = m_depth * 2;
	
	if (m_showTop)
		divider += 1;

	switch (m_bottom)
	{
	case btSize:
	case btTime:
		divider += 2;	break;
	case btSizeTime:
		divider += 2;	break;
	}
	if (!m_showBeforeDiff)
		divider += 1;
	
	m_rowH = (double)(m_rect.Height() - 1) / (double)divider;
	m_topH = (int)(m_rowH) + 1;
}

void Cdepth::getMaxSize()
{
	double	size = 0;
	
	m_maxsize = 0;
	for (int ii = 0; ii <= m_depth; ii++)
	{
		size = str2double(m_items.GetAt(askSize1+ii)->m_data);
		m_maxsize = __max(m_maxsize, size);
	}
	
	size = 0;
	for (int ii = 0; ii <= m_depth; ii++)
	{
		size = str2double(m_items.GetAt(bidSize1+ii)->m_data);
		m_maxsize = __max(m_maxsize, size);
	}
}

void Cdepth::setItemRect()
{
	std::shared_ptr<Citem>	item = nullptr;
	
	for (int ii = 0; ii < N_DATA; ii++)
	{
		item = m_items.GetAt(ii);
		m_items.GetAt(ii)->m_fRc.SetRectEmpty();
		m_items.GetAt(ii)->m_bRc.SetRectEmpty();
	}
	
	if (m_depth == 5)
		setItemRect5();
	else
		setItemRect10();
	
	getPriceRect();
}

void Cdepth::setItemRect5()
{
	int	ii = 0, line = 0;
	std::shared_ptr<Citem>	item = nullptr;

	// 매도
	if (m_percent)
	{
		line = 0;
		for (ii = askPrice5; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 0;
		for (ii = askPercent5; ii >= askPercent1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		m_idxMDS = askPrice1; m_idxMDE = askPercent5;
	}
	else
	{
		line = 0;
		for (ii = askPrice5; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}
		m_idxMDS = askPrice1; m_idxMDE = askPrice5;
	}

	if (m_showBeforeDiff)
	{
		line = 0;
		for (ii = askBefore5; ii >= askBefore1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 0;
		for (ii = askSize5; ii >= askSize1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	else
	{
		line = 0;
		for (ii = askBefore5; ii >= askBefore1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bBong)
				item->m_bRc.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			else
				item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 0;
		for (ii = askSize5; ii >= askSize1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}

	// 매수
	if (m_percent)
	{
		line = 5;
		for (ii = bidPrice1; ii <= bidPrice5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 5;
		for (ii = bidPercent1; ii <= bidPercent5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPercent5;
	}
	else
	{
		line = 5;
		for (ii = bidPrice1; ii <= bidPrice5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPrice5;
	}
	
	if (m_showBeforeDiff)
	{
		line = 5;
		for (ii = bidSize1; ii <= bidSize5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 5;
		for (ii = bidBefore1; ii <= bidBefore5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	else
	{
		line = 5;
		for (ii = bidSize1; ii <= bidSize5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 5 + (m_showTop ? 1: 0);
		for (ii = bidBefore1; ii <= bidBefore5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	
	line = 0;
	if (m_bBong)
		m_rcInfo.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+5)));
	else
		m_rcInfo.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+5)));
// updateX_20060203
	line = 5 + (m_showTop ? 1 : 0);
	m_rcInfoX.SetRect(0, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+5)));

	if (m_showTop)	m_rcInfo.OffsetRect(0, m_topH + 1);	

	switch (m_bottom)
	{
	case btNo:
		line = 10;
		
		if (!m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		break;
		
	case btSize:
		line = 10;
		item = m_items.GetAt(sizeDiff);
		item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
		if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
		item->m_fRc = item->m_bRc;
		item->m_fRc.DeflateRect(3, 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		line = 11 + (m_showBeforeDiff ? 0 : 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(prePrice);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			CRect	rect;  //0422
			int	right = 0;
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], 2);
			right = RATERECTBUY(rect);

			item = m_items.GetAt(preVolume);
			item->m_bRc.SetRect(m_columns[1], (int)(m_rowH * line), right, (int)(m_rowH * (line + 1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc) + 1;
			item->m_bRc.left -= 20;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(prePrice);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			CRect	rect;  //0422
			int	right = 0;
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], 2);
			right = RATERECTBUY(rect);

			item = m_items.GetAt(preVolume);
			item->m_bRc.SetRect(m_columns[1], (int)(m_rowH* line), right, (int)(m_rowH* (line + 1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc) + 1;
			item->m_bRc.left -= 20;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		item = m_items.GetAt(infoGUNSUCHA);
		item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
		if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
		item->m_fRc = item->m_bRc;
		item->m_fRc.DeflateRect(3, 1);
		break;
		
	case btTime:
		line = 10;
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(infoMDSD);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			
			item = m_items.GetAt(infoMSSD);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(infoMDSD);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(infoMSSD);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		break;
		
	case btSizeTime:
		line = 10;
		item = m_items.GetAt(sizeDiff);
		item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
		if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
		item->m_fRc = item->m_bRc;
		item->m_fRc.DeflateRect(3, 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 11 + (m_showBeforeDiff ? 0 : 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(infoMDSD);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			
			item = m_items.GetAt(infoMSSD);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(infoMDSD);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(infoMSSD);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}

	if (m_bBong) 
	{
		std::shared_ptr<Citem>	sItem = nullptr;
		std::shared_ptr<Citem>	eItem = nullptr;

		if (m_percent)
		{
			sItem = m_items.GetAt(askPercent5);
			eItem = m_items.GetAt(askPercent1);
			item  = m_items.GetAt(askBong);
			item->m_bRc.SetRect(sItem->m_bRc.right+1, sItem->m_bRc.top, eItem->m_bRc.right+bongWIDTH, eItem->m_bRc.bottom);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			sItem = m_items.GetAt(askPrice5);
			eItem = m_items.GetAt(askPrice1);
			item  = m_items.GetAt(askBong);
			item->m_bRc.SetRect(sItem->m_bRc.right+1, sItem->m_bRc.top, eItem->m_bRc.right+bongWIDTH, eItem->m_bRc.bottom);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
}

void Cdepth::setItemRect10()
{
	int	ii = 0, line = 0;
	std::shared_ptr<Citem>	item = nullptr;
	std::shared_ptr<Citem>	sItem = nullptr;
	std::shared_ptr<Citem>	eItem = nullptr;
	
	// 매도
	if (m_percent)
	{
		line = 0;
		for (ii = askPrice10; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 0;
		for (ii = askPercent10; ii >= askPercent1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		m_idxMDS = askPrice1; m_idxMDE = askPercent10;
	}
	else
	{
		line = 0;
		for (ii = askPrice10; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);

		}  

		m_idxMDS = askPrice1; m_idxMDE = askPrice10;
	}

	if (m_showBeforeDiff)
	{
		line = 0;
		for (ii = askBefore10; ii >= askBefore1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 0;
		for (ii = askSize10; ii >= askSize1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	else
	{
		line = 0;
		for (ii = askBefore10; ii >= askBefore1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bBong)
				item->m_bRc.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			else
				item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 0;
		for (ii = askSize10; ii >= askSize1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	
	// 매수
	if (m_percent)
	{
		line = 10;
		for (ii = bidPrice1; ii <= bidPrice10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 10;
		for (ii = bidPercent1; ii <= bidPercent10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}

		m_idxMSS = bidPrice1; m_idxMSE = bidPercent10;

	}
	else
	{
		line = 10;
		for (ii = bidPrice1; ii <= bidPrice10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}

		m_idxMSS = bidPrice1; m_idxMSE = bidPrice10;
	}

	if (m_showBeforeDiff)
	{
		line = 10;
		for (ii = bidSize1; ii <= bidSize10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		line = 10;
		for (ii = bidBefore1; ii <= bidBefore10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	else
	{
		line = 10;
		for (ii = bidSize1; ii <= bidSize10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 10;
		for (ii = bidBefore1; ii <= bidBefore10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	
	line = 0;
	if (m_bBong)
		m_rcInfo.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+10)));
	else
		m_rcInfo.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+10)));
	if (m_showTop)	m_rcInfo.OffsetRect(0, m_topH + 1);
	
	switch (m_bottom)
	{
	case btNo:
		line = 20;
		if (!m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		break;			
		
	case btSize:
		line = 20;
		item = m_items.GetAt(sizeDiff);
		item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
		if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
		item->m_fRc = item->m_bRc;
		item->m_fRc.DeflateRect(3, 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		line = 21 + (m_showBeforeDiff ? 0 : 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(prePrice);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			CRect	rect;  //0422
			int	right = 0;
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], 2);
			right = RATERECTBUY(rect);

			item = m_items.GetAt(preVolume);
			item->m_bRc.SetRect(m_columns[1], (int)(m_rowH * line), right, (int)(m_rowH * (line + 1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc) + 1;
			item->m_bRc.left -= 20;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(prePrice);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			CRect	rect;  //0422
			int	right = 0;
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], 2);
			right = RATERECTBUY(rect);

			item = m_items.GetAt(preVolume);
			item->m_bRc.SetRect(m_columns[1], (int)(m_rowH* line), right, (int)(m_rowH* (line + 1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc) + 1;
			item->m_bRc.left -= 20;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		item = m_items.GetAt(infoGUNSUCHA);
		item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
		if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
		item->m_fRc = item->m_bRc;
		item->m_fRc.DeflateRect(3, 1);
		break;
		
	case btTime :
		line = 20;
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(askSizeTime);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			sItem = m_items.GetAt(prePrice);
			sItem->m_bRc = item->m_bRc;
			sItem->m_fRc = item->m_fRc;			

			item = m_items.GetAt(askSizeTimeBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidSizeTime);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			sItem = m_items.GetAt(preVolume);
			sItem->m_bRc = item->m_bRc;
			sItem->m_fRc = item->m_fRc;			

			item = m_items.GetAt(bidSizeTimeBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askSizeTime);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidSizeTime);
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		break;
		
	case btSizeTime:
		line = 20;
		item = m_items.GetAt(sizeDiff);
		item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
		if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
		item->m_fRc = item->m_bRc;
		item->m_fRc.DeflateRect(3, 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 21 + (m_showBeforeDiff ? 0 : 1);
		if (m_showBeforeDiff)
		{
			item = m_items.GetAt(askSizeTime);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTSEL(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			sItem = m_items.GetAt(prePrice);
			sItem->m_bRc = item->m_bRc;
			sItem->m_fRc = item->m_fRc;
			
			item = m_items.GetAt(askSizeTimeBefore);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidSizeTime);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			sItem = m_items.GetAt(preVolume);
			sItem->m_bRc = item->m_bRc;
			sItem->m_fRc = item->m_fRc;			

			item = m_items.GetAt(bidSizeTimeBefore);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			item = m_items.GetAt(askSizeTime);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidSizeTime);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		break;
	}

	if (m_bBong) 
	{
		if (m_percent)
		{
			sItem = m_items.GetAt(askPercent10);
			eItem = m_items.GetAt(askPercent1);
			item  = m_items.GetAt(askBong);
			item->m_bRc.SetRect(sItem->m_bRc.right+1, sItem->m_bRc.top, eItem->m_bRc.right+bongWIDTH, eItem->m_bRc.bottom);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			sItem = m_items.GetAt(askPrice10);
			eItem = m_items.GetAt(askPrice1);
			item  = m_items.GetAt(askBong);
			item->m_bRc.SetRect(sItem->m_bRc.right+1, sItem->m_bRc.top, eItem->m_bRc.right+bongWIDTH, eItem->m_bRc.bottom);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
}

void Cdepth::getPriceRect()
{
	m_prect.SetRectEmpty();
	for (int ii = 0; ii < m_depth; ii++)
		m_prect.UnionRect(m_prect, m_items.GetAt(askPrice1+ii)->m_bRc);

	for (int ii = 0; ii < m_depth; ii++)
		m_prect.UnionRect(m_prect, m_items.GetAt(bidPrice1+ii)->m_bRc);
	
	if (m_percent)
	{
		for (int ii = 0; ii < m_depth; ii++)
			m_prect.UnionRect(m_prect, m_items.GetAt(askPercent1+ii)->m_bRc);

		for (int ii = 0; ii < m_depth; ii++)
			m_prect.UnionRect(m_prect, m_items.GetAt(bidPercent1+ii)->m_bRc);
	}
}

CRect Cdepth::getPercentRect()
{
	CRect	rect;

	rect.SetRectEmpty();
	calculatePercent();
	for (int ii = 0; ii < m_depth; ii++)
	{
		rect.UnionRect(rect, m_items.GetAt(askPercent1+ii)->m_fRc);
		rect.UnionRect(rect, m_items.GetAt(bidPercent1+ii)->m_fRc);
	}
	return rect;
}

void Cdepth::setFocus(CPoint point)
{
	CPoint	pt;

	pt = point;
	ClientToScreen(&pt);
	if (!m_prect.PtInRect(point) || this != WindowFromPoint(pt))
	{
		if (m_focus > -1)
		{
			if (m_focus >= askPrice1 && m_focus <= askPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
				m_focus = -1;
				InvalidateRect(m_prect);
			}
			else if (m_focus >= bidPrice1 && m_focus <= bidPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
				m_focus = -1;
				InvalidateRect(m_prect);
			}
		}
	}
	else
	{
		std::shared_ptr<Citem>	item = nullptr, itemx = nullptr;
		bool	found = false;
		int	ii = 0;

		for (ii = askPrice1; ii <= askPrice10; ii++)
		{
			item  = m_items.GetAt(ii);
			itemx = m_items.GetAt(ii+10);
			if (item->m_bRc.PtInRect(point) || itemx->m_bRc.PtInRect(point))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			for (ii = bidPrice1; ii <= bidPrice10; ii++)
			{
				item  = m_items.GetAt(ii);
				itemx = m_items.GetAt(ii+10);
				if (item->m_bRc.PtInRect(point) || itemx->m_bRc.PtInRect(point))
				{
					found = true;
					break;
				}
			}
		}

		if (found && m_focus != ii)
		{
			if (m_focus >= askPrice1 && m_focus <= askPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
			}
			else if (m_focus >= bidPrice1 && m_focus <= bidPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
			}

			m_focus = ii;
			if (!item->m_data.IsEmpty())
			{
				item->m_pRGB  = m_clrFocus;
				itemx->m_pRGB = m_clrFocus;
			}
			InvalidateRect(m_prect);
		}
	}

	 if (m_bBong)
	{
		CString strTooltip;
		bool	bShow = false;

		std::shared_ptr<Citem>	pItem = m_items.GetAt(askBong);
		CRect rc = pItem->m_bRc;
		rc.bottom = rc.top + rc.Height() / 2;
		if (rc.PtInRect(point))
		{
			strTooltip = GetBongStr();
			
			CPoint	pt = point;
			ClientToScreen(&pt);
			pt.x += 10;
			pt.y += 10;
			
			CRect	rect;
			CSize	size;	
			
			rect = m_pToolTip->SetData(strTooltip);
			
			size.cx = rect.Width();
			size.cy = rect.Height();
			
			rect.left = pt.x;
			rect.right = pt.x + size.cx;
			rect.top = pt.y;
			rect.bottom = rect.top + size.cy;
			
			m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
			bShow = true;	
			if (m_pToolTip->IsWindowVisible())
				m_pToolTip->Invalidate(FALSE);
			else
				m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
		}
		rc = pItem->m_bRc;
		rc.top = rc.top + rc.Height() / 2;
	}

	 CString strTooltip;
	 bool	bShow = false;
	 
	 std::shared_ptr<Citem>	pItem = m_items.GetAt(sizeDiff);
	 CRect rc = pItem->m_bRc;
	 rc.bottom = rc.top + rc.Height() / 2;
	 if (rc.PtInRect(point))
	 {
		 strTooltip = "잔량차\t\n";
		 
		 CPoint	pt = point;
		 ClientToScreen(&pt);
		 pt.x += 10;
		 pt.y += 10;
		 
		 CRect	rect;
		 CSize	size;	
		 
		 rect = m_pToolTip->SetData(strTooltip);
		 
		 size.cx = rect.Width();
		 size.cy = rect.Height();
		 
		 rect.left = pt.x;
		 rect.right = pt.x + size.cx;
		 rect.top = pt.y;
		 rect.bottom = rect.top + size.cy;
		 
		 m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
		 bShow = true;	
		 if (m_pToolTip->IsWindowVisible())
			 m_pToolTip->Invalidate(FALSE);
		 else
			 m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
	 }
	 
	 pItem = m_items.GetAt(infoGUNSUCHA);
	 rc = pItem->m_bRc;
	 rc.bottom = rc.top + rc.Height() / 2;
	 
	 if (rc.PtInRect(point))
	 {
		 strTooltip = "건수차\t\n";
		 
		 CPoint	pt = point;
		 ClientToScreen(&pt);
		 pt.x += 10;
		 pt.y += 10;
		 
		 CRect	rect;
		 CSize	size;	
		 
		 rect = m_pToolTip->SetData(strTooltip);
		 
		 size.cx = rect.Width();
		 size.cy = rect.Height();
		 
		 rect.left = pt.x;
		 rect.right = pt.x + size.cx;
		 rect.top = pt.y;
		 rect.bottom = rect.top + size.cy;
		 
		 m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);
		 bShow = true;	
		 if (m_pToolTip->IsWindowVisible())
			 m_pToolTip->Invalidate(FALSE);
		 else
			 m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
	 }
}

int Cdepth::getIndexByPoint(CPoint point)
{
	for (int ii = askPrice1; ii <= askPrice10; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return ii;
	}

	for (int ii = askPercent1; ii <= askPercent10; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return (askPrice1 + ii - askPercent1);
	}

	for (int ii = bidPrice1; ii <= bidPrice10; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return ii;
	}

	for (int ii = bidPercent1; ii <= bidPercent10; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return (bidPrice1 + ii - bidPercent1);
	}

	return -1;
}

CString Cdepth::getOrderPrice(CPoint point)
{
	for (int ii = 0; ii < m_depth; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return m_items.GetAt(ii)->m_data;

		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return m_items.GetAt(ii)->m_data;
	}

	return _T("");
}

void Cdepth::calculatePercent()
{
	for (int ii = 0; ii < m_depth; ii++)
	{
		m_items.GetAt(askPercent1+ii)->m_data = _T("");
		m_items.GetAt(bidPercent1+ii)->m_data = _T("");
	}

	double clos = 0;
	if (m_percent == pcJgga)
		clos = m_clos;
	else if (m_percent == pcSiga)
		clos = str2double(m_items.GetAt(openPrice)->m_data);
	else if (m_percent == pcKoga)
		clos = str2double(m_items.GetAt(highPrice)->m_data);
	else if (m_percent == pcJega)
		clos = str2double(m_items.GetAt(lowPrice)->m_data);
	else if (m_percent == pcUser)
		clos = (double)m_nUDPercent;
	else if (m_percent == pcGijun)
		clos = str2double(m_items.GetAt(gijunPrice)->m_data);

	if (clos == 0)
		return;

	std::shared_ptr<Citem>	item = nullptr;
	double	price = 0, percent = 0;

	for (int ii = 0; ii < m_depth; ii++)
	{
		price = str2double(m_items.GetAt(askPrice1+ii)->m_data);
		if (price > 0)
		{
			item = m_items.GetAt(askPercent1+ii);
			if (price > clos)
			{
				percent = (price - clos)/clos * 100;
				item->m_data.Format("%2.2f", percent);
				item->m_fRGB = m_clrUp;
			}
			else if (price < clos)
			{
				percent = (clos - price)/clos * 100;
				item->m_data.Format("%2.2f", percent);
				item->m_fRGB = m_clrDown;
			}
			else
			{
				item->m_data = _T("0.00");
				item->m_fRGB = m_clrDataFg;
			}
		}

		price = str2double(m_items.GetAt(bidPrice1+ii)->m_data);
		if (price > 0)
		{
			item = m_items.GetAt(bidPercent1+ii);
			if (price > clos)
			{
				percent = (price - clos)/clos * 100;
				item->m_data.Format("%2.2f", percent);
				item->m_fRGB = m_clrUp;
			}
			else if (price < clos)
			{
				percent = (clos - price)/clos * 100;
				item->m_data.Format("%2.2f", percent);
				item->m_fRGB = m_clrDown;
			}
			else
			{
				item->m_data = _T("0.00");
				item->m_fRGB = m_clrDataFg;
			}
		}
	}
}

void Cdepth::calculatePreValue()
{
	double	price;
	CString	string, tmpS;

	price = fabs(str2double(m_items.GetAt(prePrice)->m_data));
	if (price == 0)
		return;

	price -= m_clos;
	string.Format("%.f", price);
	m_items.GetAt(preBefore)->m_data = format(string, preBefore);
	tmpS = format(string, preBefore);

	price = price / m_clos * 100;
	string.Format("%.2f", price);
	m_items.GetAt(prePercent)->m_data = format(string, prePercent);
	tmpS = format(string, prePercent);
}

void Cdepth::popupMenu(CPoint point, CString Price)
{
	switch (m_type)
	{
	case ctFuture:
	case ctOption:
	case ctKOFEX:
		break;
	default:
		return;
	}

	CString	string, maps;
	CPoint	pt = point;

	ClientToScreen(&pt);
	CMenu	popM;
	popM.CreatePopupMenu();
	popM.AppendMenu(MF_STRING|MF_ENABLED, menuBase,   _T("매도주문"));
	popM.AppendMenu(MF_STRING|MF_ENABLED, menuBase+1, _T("매수주문"));
	popM.AppendMenu(MF_STRING|MF_ENABLED, menuBase+2, _T("정정/취소주문"));
	//popM->AppendMenu(MF_STRING|MF_ENABLED, menuBase+3, _T("취소주문"));

	int	index = popM.TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, this);
	index -= menuBase;
	switch (m_type)
	{
	case ctFuture:
	case ctOption:
		switch (index)
		{
		case 0:	maps = _T("IB301100");	break;
		case 1:	maps = _T("IB301200");	break;
		case 2:	maps = _T("IB301300");	break;
		case 3:	maps = _T("IB301400");	break;
		default:return;
		}
		string.Format("%s/S/d%s\t%s\n%s\t%s\n", maps, symCODE, m_code, m_refsym, Price);
		openView(typeVIEW, string);
		break;
	case ctKOFEX:
		break;
	default:
		break;
	}
}

void Cdepth::sendMessage(CPoint point, CString Price)
{
	if (m_refsym.IsEmpty())
		return;

	if (!Price.IsEmpty())
		trigger(m_refsym + "\t" + Price);
}

int Cdepth::openView(int type, CString param)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)param.GetString());
}

void Cdepth::trigger(CString param)
{
	m_parent->SendMessage(WM_USER, MAKEWPARAM(formDLL, -1), (LPARAM)param.GetString());
//	Variant(triggerCC, param);

	m_parent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnClick)), (LPARAM)m_szName.GetString());
}

CString Cdepth::format(CString data, int index)
{
	CString	tmps, sign;
	std::shared_ptr<Citem>	item = m_items.GetAt(index);

	data.Trim();
	switch (item->m_attr)
	{
	case atNormal:		// 일반적인 숫자 (0 제거)
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data))
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}
		addComma(data);
		if (!sign.IsEmpty())
			data.Insert(0, sign.GetAt(0));
		break;

	case atNoColor:		// 일반적인 숫자 (0 제거, 색깔미표시)
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data))
			return _T("");

		addComma(data);
		if (!sign.IsEmpty())
			data.Insert(0, sign.GetAt(0));
		break;

	case atSignSup:		// Normal에서 Sign 기호 제외
		getSign(data);
		eliminateZero(data);
		if (!atof(data))
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}
		addComma(data);
		break;

	case atArrow:		// 계산된 값 (+ 없어도 강세표시)
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data))
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}
		addComma(data);
		if (!sign.IsEmpty() && sign.GetAt(0) == '-')
		{
			data.Insert(0, "▼");
			item->m_fRGB = m_clrDown;
		}
		else
		{
			data.Insert(0, "▲");
			item->m_fRGB = m_clrUp;
		}
		break;

	case atCoRgb:		// +- 에 따라 색깔을 표시
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data))
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}
		addComma(data);
		if (!sign.IsEmpty())
		{
			data.Insert(0, sign.GetAt(0));
			switch (sign.GetAt(0))
			{
			case '+':
				item->m_fRGB = m_clrUp;		break;
			case '-':
				item->m_fRGB = m_clrDown;	break;
			default:
				item->m_fRGB = m_clrDataFg;	break;
			}
		}
		else
		{
			item->m_fRGB = m_clrDataFg;
		}
		break;

	case atCoRgbSup:	// +- 에 따라 색깔을 표시, Sign 기호 제외
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data) && !m_code.IsEmpty() && (m_code.GetAt(0) != '4' && m_code.GetAt(0) != 'D'))	// 20230125 파생상품 코드 개편	'4', 'D' : 스프레드
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}
		addComma(data);
		item->m_fRGB = m_clrDataFg;
		if (!sign.IsEmpty())
		{
			if (sign.GetAt(0) == '+')
			{
				item->m_fRGB = m_clrUp;
			}
			else if (sign.GetAt(0) == '-')
			{
				item->m_fRGB = m_clrDown;
			}
			else if (sign.GetAt(0) == ' ' || sign.GetAt(0) == '0')
			{
				item->m_fRGB = m_clrDataFg;
			}
		}
		break;

	case at2SignZero:	// 2Sign, 0 표시
		sign = getSign(data);
		eliminateZero(data);
		addComma(data);
		if (sign.GetLength() > 1)
			data.Insert(0, sign.GetAt(1));
		break;

	case atPercent:		// % 기호 뒤에 붙이기, 계산된 값 (+ 없어도 강세표시)
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data))
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}

		if (!sign.IsEmpty())
			data.Insert(0, sign.GetAt(0));
		data += '%';

		if (!sign.IsEmpty() && sign.GetAt(0) == '-')
			item->m_fRGB = m_clrDown;
		else
			item->m_fRGB = m_clrUp;
		break;

	case atTime:		// 시간
		if (!atoi(data))
			return _T("");

		tmps.Format("%s:%s:%s", data.Left(2), data.Mid(2, 2), data.Mid(4, 2));
		data = tmps;
		break;
	case atComma:		// 지정된 색으로 수치표시
		if (IsDigit(data))
		{
			eliminateZero(data);
			addComma(data);
		}
		break;
	}

	return data;
}

bool Cdepth::IsDigit(CString str)
{
	int	ii = 0;
	char	tmpc = 0;
	const	int	cnt = str.GetLength();

	for (ii = 0; ii < cnt; ii++)
	{
		tmpc = str.GetAt(ii);
		if (ii == 0 && tmpc == '-')
			continue;
		if (tmpc < '0' || tmpc > '9')
			return false;
	}
	
	return true;
}

void Cdepth::addComma(CString& data)
{
	CString	tmps, compareGiho;
	int	pos = 0, length = 0;

	tmps = data;
	length = tmps.GetLength();
	pos = tmps.Find('.') + 1;
	if (pos > 0)
		return;

	length -= pos;

	if (length < 4)
		return;

	compareGiho.Empty();
	data.Empty();

	for (int ii = 0; ii < length; ii++)
	{
		if ((length-ii) % 3 == 0 && ii != 0)
		{	
			compareGiho = data.Right(1);

			if (compareGiho.Compare("+") == 0 || compareGiho.Compare("-") == 0)
			{
			}
			else
			{
				data += ',';
			}
		}
		
		data += tmps.GetAt(ii);
	}
}

CString Cdepth::getSign(CString& data)
{
	CString	sign;
	int	skip = 0;

	sign.Empty();

	if (data.IsEmpty())
		return "";

	if (data.GetAt(0) == '+' || data.GetAt(0) == '-')
	{
		sign += data.GetAt(0);
		skip++;
	}

/*	if (data.GetLength() > 1 && (data.GetAt(1) == '+' || data.GetAt(1) == '-'))
	{
		sign += data.GetAt(1);
		skip++;
	}
*/
	data = data.Mid(skip);
	return sign;
}

void Cdepth::eliminateZero(CString& data)
{
	CString	tmps;
	int	length;
	
	tmps = data;
	length = tmps.GetLength();
	data.Empty();
	for (int ii = 0; ii < length; ii++)
	{
		switch (tmps.GetAt(ii))
		{
		case '\0':
			data.Empty();
			return;

		case '.':
			if (!ii)
			{
				data.Empty();
				return;
			}
			data = tmps.Mid(ii-1);
			return;

		case '0':
			break;

		default:
			data = tmps.Mid(ii);
			return;
		}
	}
}

double Cdepth::str2double(CString string)
{
	string.Remove(',');
	return atof(string);
}

BOOL Cdepth::createFont()
{
	int	style = FW_NORMAL;
	bool	bItalic = false;

	if (m_style == FS_BOLD || m_style == FS_IBOLD)
		style = FW_BOLD;
	if (m_style == FS_ITALIC || m_style == FS_IBOLD)
		bItalic = true;

	m_font = getFont(m_point, "굴림", style, bItalic);

	// updateX_20060119
	LOGFONT	lf{};
	
	m_font->GetLogFont(&lf);
	m_fontH = abs(lf.lfHeight);

	EW_Font();

	return TRUE;
}

COLORREF Cdepth::getColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CPen* Cdepth::getPen(int color, int width)
{
	struct	_penR	penR{};

	penR.clr   = color;
	penR.width = width;
	penR.style = PS_SOLID;
	return (CPen *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getPEN, 0), (LPARAM)&penR);
}

CFont* Cdepth::getFont(int point, CString name, int bold, bool italic)
{
	struct	_fontR	rFont{};

	rFont.name = (char*)name.GetString();
	rFont.point = point;
	rFont.italic = italic;
	rFont.bold = bold;
	return (CFont*) m_parent->SendMessage(WM_USER, getFONT, (LPARAM)&rFont);
}

CBitmap* Cdepth::getBitmap(CString path)
{
	return (CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

//-----------------------------------------------------------------------------
//	options
//
//	d	5, 10			5호가, 10호가
//	p				호가 전일대비 등락율
//	b				잔량 직전대비
//	s	0 (text)		bar graph
//		1 (bar)
//		2 (text + bar)
//	c				top display (컬럼이름)
//	h	0 (None)		bottom display
//		1 (총잔량)
//		2 (시간외)
//		3 (총잔량 + 시간외)
//	l				내부선 표시
//	u	0 (None)		종목type
//		1 (현물)
//		2 (업종)
//		3 (선물)
//		4 (옵션)
//		5 (ECN)
//		6 (주식옵션)
//		7 (KOFEX)
//	t				항상 10호가로 Data가 옴
//	o				시고저종 표시
//	a	0 (None)		lbutton action
//		1 (주문 팝업창)
//		2 (메시지 전송)
//	r				reference symbol
//	x				미니 당일 일봉 보기
//	w				현재가화면	
//	ex) /d10/p/b/s1/c/h3/f10/l	(10호가 풀옵션)

//	y	미니봉가격숨기기
//	v	미니봉거래량숨기기

void Cdepth::parseOptions()
{
	int	idx = 0, pos = 0;
	CString	keys, text, tmps;

	tmps = _T("/ ");
	keys = _T("dpbschlutoargmxwfzyv");

	for (int ii = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = m_options.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = m_options.Find('/', idx);

		text = (pos < 0) ? m_options.Mid(idx) : m_options.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'd':
			m_depth = atoi(text); 
			break;
		case 'p':
			m_percent = (enum _percent)atoi(text);
			break;
		case 'b':
			m_showBeforeDiff = true;
			break;
		case 's':
			m_bar = (_graph)atoi(text);
			break;
		case 'c':
			m_showTop = true;
			break;
		case 'h':
			m_bottom = (_bottom)atoi(text);
			break;
		case 'l':
			m_showLine = true;
			break;
		case 'u':
			m_type = (_ctype)atoi(text);
			break;
		case 'o':
			m_showOHLC = (enum _showInfo)atoi(text);
			break;
		case 'a':
			m_action = (_act)atoi(text);
			break;
		case 'r':
			m_refsym = text;
			break;
		case 'f':
			m_configFile = text;
			break;
		case 'g':
			m_bGradient = true;
			break;
		case 'x':
			m_bBong	= true;
			break;
		case 'w':
			//**m_bConfig = false;
			m_bConfig = true;
			break;
		case 'z':
			m_bBoldPrice = true;
			break;
		case 'y':
			m_bMBPrice = false;
			break;
		case 'v':
			m_bMBVolume = false;
			break;
		}
	}
}

COLORREF Cdepth::SetItemBGColor(int item)
{
	if (item >= askPrice1 && item <= askPercent10)
		return m_clrAskGe;
	else if (item >= bidPrice1 && item <= bidPercent10)
		return m_clrBidGs;
	
	return m_clrWhite;
}

void Cdepth::SendTR(CString name, CString data, BYTE type, int key)
{
	CString trData = ""; 
	CString tempdata = "";
	CString pass;
	
	struct _userTH udat;
	
	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;

	trData.Empty();
	trData = m_szName;
	trData += '\t';
	trData += CString((char *)&udat, L_userTH);
	trData += data;

	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.GetLength() - L_userTH - m_szName.GetLength() - 1), (LPARAM)trData.GetString());
}

CString Cdepth::GetDataString(CString keys, CString init)
{
	char	wb[32];

	GetPrivateProfileString("CurAmtSet", keys, init, wb, sizeof(wb), m_file);
	return wb;
}

int Cdepth::GetDataInt(CString keys, int init)
{
	return GetPrivateProfileInt("CurAmtSet", keys, init, m_file);
}

CString Cdepth::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void Cdepth::TipMouseMove(CPoint point)
{
	ScreenToClient(&point);

	MouseMove(true, point);
}

void Cdepth::MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly)
{

}

CString Cdepth::GetBongStr()
{
	CString str = _T("");
	std::shared_ptr<Citem>	pItem = nullptr;

	pItem = m_items.GetAt(openPrice);
	CString siga = pItem->m_data;
	pItem = m_items.GetAt(highPrice);
	CString koga = pItem->m_data;
	pItem = m_items.GetAt(lowPrice);
	CString jega = pItem->m_data;
	CString jgga;
	jgga.Format("%d", (int)m_curr);
	jgga = format(jgga,atNormal);
	
	CString ma5;
	ma5.Format("%d", (int)((m_sum4 + m_curr) / 5));
	ma5 = format(ma5,atNormal);
	CString ma10;
	ma10.Format("%d", (int)((m_sum9 + m_curr) / 10));
	ma10 = format(ma10,atNormal);

	str = _T("일봉\t\n시가\t") + siga + _T("\n고가\t") + koga + _T("\n저가\t") + jega + _T("\n종가\t") + jgga
		+ _T("\nMA5\t") + ma5 + _T("\nMA10\t") + ma10 + _T("\n");	

	return str;
}

BOOL Cdepth::_GetVisible() 
{
	return IsWindowVisible();
}

void Cdepth::_SetVisible(BOOL bNewValue) 
{
	ShowWindow(bNewValue? SW_SHOW : SW_HIDE);
}

BOOL Cdepth::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if (m_rcConfig.PtInRect(point) || m_rcConfigL.PtInRect(point) || m_rcConfigR.PtInRect(point))
	//if (m_rcConfig.PtInRect(point) || m_rcConfigL.PtInRect(point))
	{
		HCURSOR	hCursor = (HCURSOR)m_parent->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), 10);
		if (hCursor)
		{
			SetCursor(hCursor);
			return TRUE;
		}
	}
	//::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void Cdepth::drawButton(CDC *pDC)
{
	if (m_bConfig)
	{
		if (!m_rcConfig.IsRectEmpty())
		{
			drawButton(pDC, m_rcConfig, m_pBitmapC1);
		}
		if (!m_rcConfigL.IsRectEmpty())
		{
			drawButton(pDC, m_rcConfigL, m_pBitmapP1);
		}
		if (!m_rcConfigR.IsRectEmpty())
		{
			drawButton(pDC, m_rcConfigR, m_pBitmapP1);
		}
	}
}

void Cdepth::drawButton(CDC *pDC, CRect rc, CBitmap *pBitmap)
{
	if (pBitmap)
	{
		CDC	memDC;
		BITMAP	stBitmapInfo{};
		CBitmap	*pSBitmap = nullptr;

		pBitmap->GetBitmap(&stBitmapInfo);
		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
		if (pSBitmap)
			memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
	else
	{
		pDC->FillSolidRect(rc, RGB(196,196,196));
	}
}

void Cdepth::drawBox(CDC *pDC)
{
	CRect	rect;
	
	GetClientRect(&rect);
	CBrush *pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	CPen *pOldPen = pDC->SelectObject(getPen(m_clrBox, 1));
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldPen); pDC->SelectObject(pOldBrush);	
}

void Cdepth::ConfigDlg()
{
	CConfigDlg	dlg(&m_config);
	if (dlg.DoModal())
	{
		SaveCondition(m_path, &m_config);
		InitEnv();

		Invalidate();
		setItemRect();
		//m_parent->PostMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnChange)), (LPARAM)m_szName.GetString());
	}
}

void Cdepth::ConfigDlgL()
{
	CMenu cMenu;
	cMenu.CreatePopupMenu();
	const	char*	const	pDisplay[3] = { "표시 안함", "그래프 표시", "체결 표시"};
	for (int ii = 0; ii < 3; ii++)
	{
		if (m_config.mbong == ii)
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, ii+1, pDisplay[ii]);
		else
			cMenu.AppendMenu(MFT_STRING, ii+1, pDisplay[ii]);
	}

	CPoint	point;
	GetCursorPos(&point);
	const	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, NULL);
	if (ret > 0)
	{
		m_config.mbong = ret-1;
		SaveCondition(m_path, &m_config);
		EW_Show();
	}
}

void Cdepth::ConfigDlgR()
{
	CMenu cMenu;
	cMenu.CreatePopupMenu();
	const	char*	const	pDisplay[3] = { "표시 안함", "정보1 표시", "정보2 표시"};
	for (int ii = 0; ii < 3; ii++)
	{
		if (m_config.info == ii)
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, ii+1, pDisplay[ii]);
		else
			cMenu.AppendMenu(MFT_STRING, ii+1, pDisplay[ii]);
	}

	CPoint	point;
	GetCursorPos(&point);
	const	int	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, NULL);
	if (ret > 0)
	{
		m_config.info = ret-1;
		SaveCondition(m_path, &m_config);
		InitEnv();
		Invalidate(); setItemRect();
	}
}

bool Cdepth::ReadCondition(CString path, struct _config *pConfig)
{
	CString file;

	if (m_configFile.IsEmpty())
		file.Format("%s\\%s", path, FILENAME);
	else 
	{
		if (m_configFile.Find('.') >= 0)
			file.Format("%s\\%s", path, m_configFile);
		else
			file.Format("%s\\%s.env", path, m_configFile);
	}

	return readFile(file, (char*)pConfig, sz_CONFIG);
}

void Cdepth::SaveCondition(CString path, struct _config *pConfig)
{
	CString file;

	if (m_configFile.IsEmpty())
		file.Format("%s\\%s", path, FILENAME);
	else 
	{
		if (m_configFile.Find('.') >= 0)
			file.Format("%s\\%s", path, m_configFile);
		else
			file.Format("%s\\%s.env", path, m_configFile);
	}

	writeFile(file, (char*)pConfig, sz_CONFIG);
}

bool Cdepth::readFile(CString path, char *pBytes, int nBytes)
{
	CFile f;
	CFileException e;

	if (!f.Open(path, CFile::modeRead | CFile::shareDenyNone, &e))
	{
		TRACE("[READ] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}

	const	UINT	nByteRead = f.Read(pBytes, nBytes);
	if (nByteRead != UINT(nBytes))
	{
		f.Close();
		return false;
	}
	f.Close();

	return true;
}

bool Cdepth::writeFile(CString path, char *pBytes, int nBytes)
{
	CFile f;
	CFileException e;

	if (!f.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		TRACE("[WRITE] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}

	f.Write(pBytes, nBytes);
	f.Close();

	return true;
}

void Cdepth::InitEnv()
{
	m_showOHLC = (enum _showInfo)m_config.info;

	m_bong = (enum _bong)m_config.mbong;

	m_bar = grNo;		if (m_config.sgrp)  m_bar	= grBarText;
	m_percent = pcNo;	if (m_config.hdyul) m_percent	= pcGijun;
	m_bCurr = false;	if (m_config.curr)  m_bCurr	= true;
	m_bBoldPrice = false;	if (m_config.hbold) m_bBoldPrice= true;
	m_bBong = false;	if (m_config.sbong) m_bBong	= true;
	m_bBoldRemain = false; if (m_config.hrbold) m_bBoldRemain = true;
	m_bBoldCnt = false; if (m_config.hcbold) m_bBoldCnt = true;

	setItemRect(); calculatePercent();
}

void Cdepth::_Clear() 
{
	char empty[sz_hogax];
	FillMemory(empty, sz_hogax, ' ');

	dispatch(empty, sz_hogax);
}

void Cdepth::clickAction(CRect rect, CPoint point)
{
	int	line = 0;
	CRect	itemRC;
	CString	entry;

	if (m_showOHLC == infoMGVL)
	{
		entry = _T(""); line = m_showTop ? 1 : 0;
		for (int ii = 0; ii < 5; ii++, line++)
		{
			itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
			if (!itemRC.PtInRect(point))
				continue;

			switch (ii)
			{
			case 2: entry = m_items.GetAt(openPrice)->m_data;	break;
			case 3: entry = m_items.GetAt(highPrice)->m_data;	break;
			case 4: entry = m_items.GetAt(lowPrice)->m_data;	break;
			default:							break;
			}

			if (!entry.IsEmpty())
			{
				entry.Remove(',');
				switch (m_action)
				{
				case acPopup:	popupMenu(point, entry);	break;
				case acMsg:	sendMessage(point, entry);	break;
				case acNo:					break;
				}
			}
			return;
		}
	}
	else if (m_showOHLC == infoSHGA && m_type == ctFuture)
	{
		entry = _T(""); line = m_showTop ? 1 : 0;
		for (int ii = 0; ii < 5; ii++, line++)
		{
			itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
			if (!itemRC.PtInRect(point))
				continue;

			switch (ii)
			{
			case 0: entry = m_items.GetAt(shPrice)->m_data;	break;
			case 1: entry = m_items.GetAt(hhPrice)->m_data;	break;
			case 2: entry = m_items.GetAt(gijunPrice)->m_data;	break;
			default:							break;
			}

			if (!entry.IsEmpty())
			{
				entry.Remove(',');
				switch (m_action)
				{
				case acPopup:	popupMenu(point, entry);	break;
				case acMsg:	sendMessage(point, entry);	break;
				case acNo:					break;
				}
			}
			return;
		}
	}
	else if (m_showOHLC == infoSHGA && m_type == ctOption)
	{
		entry = _T(""); line = m_showTop ? 1 : 0;
		for (int ii = 0; ii < 5; ii++, line++)
		{
			itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
			if (!itemRC.PtInRect(point))
				continue;

			switch (ii)
			{
			case 0: entry = m_items.GetAt(gijunPrice)->m_data;	break;
			default:							break;
			}

			if (!entry.IsEmpty())
			{
				entry.Remove(',');
				switch (m_action)
				{
				case acPopup:	popupMenu(point, entry);	break;
				case acMsg:	sendMessage(point, entry);	break;
				case acNo:					break;
				}
			}
			return;
		}
	}
}

///////////////////////////////////////////////////
// Extra Window
void Cdepth::EW_Init()
{
	m_pContract = nullptr;
	m_pMBong    = nullptr;
	m_pRgn	    = nullptr;

	m_bMBPrice = true;
	m_bMBVolume = true;
}

void Cdepth::EW_Clear()
{
	if (m_pContract)
		m_pContract->Clear();
	if (m_pMBong)
		m_pMBong->Clear();
}

void Cdepth::EW_Destroy()
{
	if (m_pContract)
		m_pContract.reset();

	if (m_pMBong)
		m_pMBong.reset();

	if (m_pRgn)
	{
		m_pRgn->DeleteObject();
		m_pRgn.reset();
	}
}

bool Cdepth::EW_Create()
{
	if (m_bConfig || m_config.mbong == 2)
	{
		m_pContract = std::make_unique<CContractWnd>(m_parent, this);
		if (!m_pContract->Create(NULL, NULL, WS_CHILD, CRect(0,0,10,10), this, 100))
		{
			TRACE("Window Create Failed!!!\n");
			return FALSE;
		}
		m_pContract->Init(m_depth, HALFLINE);
		m_pContract->Palette();
	}
	if (m_bConfig || m_config.mbong == 1)
	{
		m_pMBong = std::make_unique<CMBongWnd>(m_parent, this);
		if (!m_pMBong->Create(NULL, NULL, WS_CHILD, CRect(0,0,10,10), this, 101))
		{
			TRACE("Window Create Failed!!!\n");
			return FALSE;
		}
		m_pMBong->Init(m_bMBPrice, m_bMBVolume, true);
		m_pMBong->Palette();
	}

	m_pRgn = std::make_unique<CRgn>();
	m_pRgn->CreateRectRgn(0, 0, 0, 0);

	return TRUE;
}

void Cdepth::EW_Move()
{
	CRect	crc, rc;
	GetClientRect(&crc);
	rc.SetRect(0, (int)(m_topH + m_rowH * m_depth), m_columns[0], (int)(m_topH + m_rowH * m_depth*2));
	if (m_pMBong)		m_pMBong->MoveWindow(rc);
	if (m_pContract)	m_pContract->MoveWindow(rc);

	if (m_pRgn)
	{
		CRgn rgn1, rgn2;
		rgn1.CreateRectRgn(crc.left, crc.top, crc.right, crc.bottom);
		rgn2.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
		m_pRgn->CombineRgn(&rgn1, &rgn2, RGN_DIFF);
		rgn1.DeleteObject();
		rgn2.DeleteObject();
	}
}

void Cdepth::EW_Show()
{
	switch (m_config.mbong)
	{
	case 1:
		if (m_pMBong)		m_pMBong->ShowWindow(SW_SHOW);
		if (m_pContract)	m_pContract->ShowWindow(SW_HIDE);
		break;
	case 2:
		if (m_pMBong)		m_pMBong->ShowWindow(SW_HIDE);
		if (m_pContract)	m_pContract->ShowWindow(SW_SHOW);
		break;
	case 0:
	default:
		if (m_pMBong)		m_pMBong->ShowWindow(SW_HIDE);
		if (m_pContract)	m_pContract->ShowWindow(SW_HIDE);
		break;
	}
}

void Cdepth::EW_Font()
{
	if (m_pMBong)		m_pMBong->Font(m_font);
	if (m_pContract)	m_pContract->Font(m_font);
}

void Cdepth::EW_Palette()
{
	if (m_pContract)	m_pContract->Palette();
	if (m_pMBong)		m_pMBong->Palette();
}

void Cdepth::EW_Dispatch(char *dataB, bool bTEN)
{
	// 2010.09.02 상품선물(금,미니금,돈육) 의 경우 digit = 0 임.
	int digit = 0;
	if (m_code.GetLength()==8)
	{
		int mkgb = atoi(m_code.Mid(1, 2));
		if (mkgb>=85 && mkgb<=99)
			digit = 0;
	}

	if (bTEN)
	{
		struct	_hoga*	hoga = (_hoga*)dataB;
		if (m_pContract)	m_pContract->Dispatch(&dataB[sz_hoga], digit);
		if (m_pMBong)		m_pMBong->Dispatch(&dataB[sz_hoga], 
			CString(hoga->shga, sizeof(hoga->shga)), CString(hoga->hhga, sizeof(hoga->hhga)), digit);
	}
	else
	{
		struct	_hogax*	hoga = (_hogax*)dataB;
		if (m_pContract)	m_pContract->Dispatch(&dataB[sz_hogax], digit);
		if (m_pMBong)		m_pMBong->Dispatch(&dataB[sz_hogax], 
			CString(hoga->shga, sizeof(hoga->shga)), CString(hoga->hhga, sizeof(hoga->hhga)), digit);
	}
}

void Cdepth::EW_Alert(CString data)
{
	if (m_pContract)	m_pContract->Alert(data);
	if (m_pMBong)		m_pMBong->Alert(data);

}

void Cdepth::EW_Alert(struct _alertR* alertR)
{
	if (m_pContract)	m_pContract->Alert(alertR);
	if (m_pMBong)		m_pMBong->Alert(alertR);
	
}
// Extra Window
///////////////////////////////////////////////////

BOOL Cdepth::_getBoldRemain() 
{
	return m_bBoldRemain ? TRUE : FALSE;
}

void Cdepth::_setBoldRemain(BOOL bNewValue) 
{
	const	BOOL	bOldValue = (m_bBoldRemain) ? TRUE : FALSE;

	if (bOldValue!=bNewValue)
		Invalidate();
	m_bBoldRemain = (bNewValue) ? true : false;
}

BOOL Cdepth::GetBoldRemainCount() 
{
	return m_bBoldCnt ? TRUE : FALSE;
}

void Cdepth::SetBoldRemainCount(BOOL bNewValue) 
{
	const	BOOL	bOldValue = (m_bBoldCnt) ? TRUE : FALSE;
	if (bOldValue!=bNewValue)
		Invalidate();
	m_bBoldCnt = (bNewValue) ? true : false;
}
