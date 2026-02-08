// depth.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Depth.h"
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

#define LP_AMOUNT		// elw LP 보유물량 공개시 define

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	HALFLINE	46
#define RATERECTSEL(a)	a.left + (a.Width() * HALFLINE) / 100
#define RATERECTBUY(a)	a.left + (a.Width() * (100-HALFLINE)) / 100
#define P_RATIO		5.8 / 10
#define P_BIG_RATIO	6.2 / 10
#define	idDEPTH		10235
#define	symCODE		"1301"
#define	symECN		"81301"

#define menuBase	10000

#define memoWIDTH	6
#define bongWIDTH	10

#define FILENAME	"hoga.env"

#define CLR_OPENPRICE	RGB(200,150,100)	//시가
#define CLR_HIGHPRICE	RGB(255,0,0)//RGB(200,100,50)	//고가
#define CLR_LOWPRICE	RGB(0,0,255)//RGB(50,100,200)	//저가
#define CLR_PIVOTPRICE	RGB(0,0,0)	//기준가
#define CLR_SHPRICE		RGB(255,0,0)		//상한가
#define CLR_HHPRICE		RGB(0,0,255)		//하한가
#define CLR_GAJUNGPRICE	RGB(50,180,240)		//가중평균가
#define CLR_VI			RGB(112,173,71)	//정적vi 상한 하한
#define CLR_KMIDPRICE	RGB(0,255,0)	//KRX 중간가 네모칸
#define CLR_NMIDPRICE	RGB(255,201,14)	//NXT 중간가 네모칸

#define CLR_KMIDPRCLEFT   RGB(59, 138, 105)
#define CLR_NMIDPRCLEFT  RGB(200, 13, 172)

#define DF_TOT 0  //통합
#define DF_SEP  1  //개별

const int	imgCX1 = 15, imgCX2 = 15, imgCY1 = 15, imgCY2 = 15;
/////////////////////////////////////////////////////////////////////////////
// Cdepth

#define DF_MIDPRC
#define CLR_MIDPRC RGB(255, 255, 255)
#define CLR_IVS_MIDPRC RGB(207, 193, 242)

Cdepth::Cdepth()
{
	EnableAutomation();

	m_class  = AfxRegisterWndClass(0);
	m_parent = nullptr;

	m_rect.SetRectEmpty();
	m_options = _T("");
	m_refsym  = _T("");
	m_alwaysTEN = false;

	m_font  = nullptr;
	m_fonts = _T("굴림");
	m_point = 9;
	m_style = 0;

	m_code  = _T("");
	m_mapName  = _T("");
	m_items.RemoveAll();
	m_indices.RemoveAll();
	m_sisememo.RemoveAll();

	for (int ii = 0; ii < 3; ii++)
		m_columns[ii] = 0;
	m_rowH = 18;
	m_topH = 18;

	m_tracking = FALSE;
	m_focus    = -1;
	m_maxsize  = 0;
	m_prect.SetRectEmpty();
	m_rcConfig.SetRectEmpty();
	m_rcConfigL.SetRectEmpty();
	m_rcConfigR.SetRectEmpty();
	m_rcTime.SetRectEmpty();

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
	m_showOHLC	 = infoNo;
	m_bGradient	 = false;
	m_bSiseMemo	 = false;
	m_percent	 = pcNo;
	m_nUDPercent	 = 0;
	m_bLButtonDown	 = false;
	m_bSiga          = false;
	m_bBong		 = false;
	m_bong		 = bongBong;
	m_bCurr		 = true;
	m_bBoldPrice	 = false;
	m_bBoldRemain    = false;
	m_bBoldCnt       = false;
	m_bPredict	 = false;				//하단에 클릭시 토글되는 버튼인데 예상가 여부를 나타낸다
	//m_iBottomType = 0;		 //0:예상체결 ,    1:시간외,    2:현재가(K)   --> m_bPredict 대체
	m_bConfig	 = true;

	m_pBitmapC1 = nullptr;
	m_pBitmapC2 = nullptr;
	m_pBitmapP1 = nullptr;
	m_pBitmapP2 = nullptr;
	m_pBitmapT1 = nullptr;
	m_pBitmapS1 = nullptr;
	
	m_bLPMode = false;

	m_bDragMode =FALSE;
	m_bPercent = FALSE;
	m_nPriceH = 0;
	m_nPriceL = 0;

	m_bBigDigit = false;

	m_bAble = false;

	EW_Init();
	m_mkgubn = mkKRX;
	ZeroMemory(&m_config, sz_CONFIG);
}

Cdepth::~Cdepth()
{
	m_indices.RemoveAll();
	freeItems(); freeSiseMemo();
	
	if (IsWindow(m_pToolTip->GetSafeHwnd()))
		m_pToolTip->SendMessage(WM_CLOSE);
	m_pToolTip.reset();
}

void Cdepth::OnDestroy() 
{
	if (m_configFile.Find("IB2016") < 0)
	{
		m_dropTarget.Revoke();
	}
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
	ON_WM_RBUTTONDOWN()
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
	DISP_PROPERTY_EX(Cdepth, "Info", _GetInfo, _SetInfo, VT_I2)
	DISP_PROPERTY_EX(Cdepth, "BoldRemain", GetBoldRemain, SetBoldRemain, VT_BOOL)
	DISP_PROPERTY_EX(Cdepth, "BoldRemainCount", GetBoldRemainCount, SetBoldRemainCount, VT_BOOL)
	DISP_FUNCTION(Cdepth, "SetProperties", _SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(Cdepth, "GetProperties", _GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(Cdepth, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(Cdepth, "InitAmtSet", InitAmtSet, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(Cdepth, "InitSiseMemoSet", InitSiseMemoSet, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(Cdepth, "ChangeInfo", _ChangeInfo, VT_I2, VTS_NONE)
	DISP_FUNCTION(Cdepth, "ChangeBong", ChangeBong, VT_I2, VTS_NONE)
	DISP_FUNCTION(Cdepth, "SetLPMode", SetLPMode, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(Cdepth, "SetPriceRange", SetPriceRange, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(Cdepth, "SetPercent", SetPercent, VT_EMPTY, VTS_I2 VTS_I2)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(Cdepth, "SetMKtype", dispidSetMKtype, SetMKtype, VT_EMPTY, VTS_I2)
	DISP_FUNCTION_ID(Cdepth, "SetPrcEdit", dispidSetPrcEdit, SetPrcEdit, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(Cdepth, "InsertObjData", dispidInsertObjData, InsertObjData, VT_EMPTY, VTS_BSTR)
	DISP_PROPERTY_EX_ID(Cdepth, "sDepthInfo", dispidsDepthInfo, GetsDepthInfo, SetsDepthInfo, VT_BSTR)
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
	if (m_showOHLC == infoNo)
		m_showOHLC = infoSiga;
	else if (m_showOHLC == infoSiga)
		m_showOHLC = infoPivot;
	else if (m_showOHLC == infoPivot)
		m_showOHLC = infoCurr;
	else if (m_showOHLC == infoCurr)
		m_showOHLC = infoNo;

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
	m_showOHLC       = infoNo;
	m_bGradient	 = false;
	m_bSiseMemo	 = false;

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

	if (m_alwaysTEN)
		strResult += _T("/t");

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
	char	wb[256]{};
	struct	_param*	param = nullptr;
	
	m_parent  = parent;
	param     = (struct _param *)ptr;
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
	m_sUserConfFile.Format("%s\\user\\%s\\userconf.ini", root, user);
	m_configFile = _T("");
	m_mapName = (LPCTSTR)m_parent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0));

	if (m_rect.Width() >= 320)
		m_bAble = true;
	else
		m_bAble = false;

	//m_bAble = false;
	
	m_b1801 = false;
	if (strcmp(m_mapName, "IB180100")==0)
	{
		m_b1801 = true;
	}
	initialColor();
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
		
		if (m_configFile.Find("IB2016") < 0)
		{
			m_dropTarget.Register(this);
			m_dropTarget.GetDepth(this);
		}

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
	if (0)
	{
		CRect clipRect;
		if (dc.GetClipBox(&clipRect) == ERROR)
		{
			// Could not get clip box: fallback to full client
			CRect clientRect;
			GetClientRect(&clientRect);
			//draw(&dc, &clientRect);

			if (GetTickCount() % 200 == 0)
			{
				m_slog.Format("[depth][%s]<%d> GetClipBox error    clientRect [%d][%d][%d][%d] ", clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);
				Output_DebugString(m_slog);
			}

			draw_clip_wrapper(this, &dc, 
				[this](CDC* memDC, const CRect* pClip) {
					this->draw(memDC, pClip);
				}, &clientRect);
		}
		else
		{
			// Draw only the invalidated clip area using clip-aware draw

			if (GetTickCount() % 200 == 0)
			{
				m_slog.Format("[depth][%s]<%d> GetClipBox success clipRect [%d][%d][%d][%d] ", clipRect.left, clipRect.top, clipRect.right, clipRect.bottom);
				Output_DebugString(m_slog);
			}

			draw_clip_wrapper(this, &dc,
				[this](CDC* memDC, const CRect* pClip) {
					this->draw(memDC, pClip);
				}, & clipRect);
		}
	}
	else
	{
		m_sync.Lock();
		CRect rec;
		draw(&dc, &rec);
		m_sync.Unlock();
	}
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

	CWnd::OnLButtonDown(nFlags, point);
}

void Cdepth::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_action == acNo || !m_bLButtonDown)
		return;

	if (m_prect.PtInRect(point))
	{
		if (m_focus > -1)
		{
			if (m_focus >= askPrice1 && m_focus <= askPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
				
				InvalidateRect(m_prect);
				m_focus = -1;
			}
			else if (m_focus >= bidPrice1 && m_focus <= bidPrice10)
			{
				m_items.GetAt(m_focus)->m_pRGB    = SetItemBGColor(m_focus);
				m_items.GetAt(m_focus+10)->m_pRGB = SetItemBGColor(m_focus);
				
				InvalidateRect(m_prect);
				m_focus = -1;
			}
			else if (m_focus == MIDKPRICE && m_iShowMIDPRC)
			{  //좌클릭후 업할때 중간가면 원래색상으로
				m_items.GetAt(m_focus)->m_pRGB = CLR_MIDPRC;
				InvalidateRect(m_prect);
				m_focus = -1;
			}
		}

		const	int	select = getIndexByPoint(point);

CString slog;
slog.Format("[midprc] select = [%d]", select);
Output_DebugString(slog);


		if (select > 0)
		{
			CString Price = m_items.GetAt(select)->m_data;
			Price.Remove(',');
			switch (m_type)
			{
			case ctCode:
			case ctECN:
				if (select == MIDKPRICE)
				{
					sendMessage(point, Price);
				}
				else if(select == MIDNPRICE)
				{
					sendMessage(point, Price);
				}
				else if (select == MIDKPRICE && m_bShowMidtooltip)
				{
					//m_bClickMIDPRC = TRUE;
					//CString strTooltip;
					//if (m_pToolTip->IsWindowVisible())
					//{
					//	m_pToolTip->ShowWindow(SW_HIDE);
					//}
					//CPoint	pt = point;
					//ClientToScreen(&pt);
					//pt.x += 10;
					//pt.y += 10;

					//CRect	rect;
					//CSize	size;

					////strTooltip.Format("중간가로 주문을 원하시면  \t\n구분을 변경해 주세요.  \t\n▶ 중간가(KRX)란?  \t\n    최우선 매도호가와 \t\n    최우선 매수호가의  \t\n    중간가격이에요  \t\n ");
					//strTooltip.Format("▶ K는 KRX중간가,  \t\n   N는 NXT중간가에요. \t\n ");
					//m_pToolTip->SetData(strTooltip);

					//size.cx = rect.Width();
					//size.cy = rect.Height();

					//rect.left = pt.x;
					//rect.right = pt.x + size.cx;
					//rect.top = pt.y;
					//rect.bottom = rect.top + size.cy;

					//m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);  //else if (m_bBong)
					//m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
					return;
				}
				else if (atof(Price) && Price.Find('%') == -1)
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
			case ctFuture:
			case ctOption:
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
	else if (m_rcTime.PtInRect(point))
	{
		m_bPredict = !m_bPredict;	 //버튼클릭시 토글되는곳
		m_config.pre = m_bPredict ? 1 : 0;

		//m_iBottomType = (m_iBottomType + 1) % 3;
		//if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
		//{
		//	if (m_bPredict)
		//		m_iBottomType = 0;
		//	else
		//		m_iBottomType = 1;
		//}

		//m_slog.Format("pcx_depth][%s]<%d>  토글클릭시 설정저장 pre = [%d] m_iBottomType=[%d]", __FUNCTION__, __LINE__, m_config.pre, m_iBottomType);
		//OutputDebugString(m_slog);

		SaveCondition(m_path, &m_config);

		setFocus(point);

		Invalidate();
	}
	else if (m_bConfig)
	{
		if (m_rcConfig.PtInRect(point))
			ConfigDlg();	
	/*	else if (m_rcConfigL.PtInRect(point))
			ConfigDlgL();	
		else if (m_rcConfigR.PtInRect(point))
			ConfigDlgR();	*/
		else if (m_rcConfigL.PtInRect(point) || m_rcConfigR.PtInRect(point))
		{
			if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)
				return;

			if (m_iDataType == 1)
				m_iDataType = 0;
			else if (m_iDataType ==0)
				m_iDataType = 1;
	
			m_parent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnChange)), (LPARAM)m_szName.GetString());
			Invalidate();
		}
	}

	if (!m_refsym.IsEmpty())
	{
		m_bLButtonDown = false;
		return;
	}

	if (m_type == ctCode || m_type == ctECN)
	{
		CString	text;

		if (m_items.GetAt(openPrice)->m_bRc.PtInRect(point))
			text = m_items.GetAt(openPrice)->m_data;
		else if (m_items.GetAt(highPrice)->m_bRc.PtInRect(point))
			text = m_items.GetAt(highPrice)->m_data;
		else if (m_items.GetAt(lowPrice)->m_bRc.PtInRect(point))
			text = m_items.GetAt(lowPrice)->m_data;
		else if (m_items.GetAt(cloPrice)->m_bRc.PtInRect(point))
			text = m_items.GetAt(cloPrice)->m_data;

		text.Remove(',');
		if (!text.IsEmpty())
		{
			CString pdat;
			if (m_sPrcTrigger_File.GetLength() > 0)
			{
				CString stmp;
				char	wb[32];
				GetPrivateProfileString("IB100000", "edit_trigger", "", wb, sizeof(wb), m_sUserConfFile);
				stmp.Format("%s", wb);
				stmp.TrimRight();
				if(stmp.Find("edStopPrc") >= 0)
					pdat.Format("SetStopPrice\t%s,%s", m_code, text);
				else
					pdat.Format("SetPrice\t%s,%s", m_code, text);
			}
			else
			{
				if (m_iPrcType == TYPE_STOPPRC)
					pdat.Format("SetStopPrice\t%s,%s", m_code, text);
				else
					pdat.Format("SetPrice\t%s,%s", m_code, text);
			}
			

			m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)pdat);			
//			trigger(m_refsym + "\t" + text);
		}
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
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		m_sync.Lock();
		m_bBigDigit = FALSE;
		dispatch(exth->data, exth->size);
		m_sync.Unlock();
		break;

	case DLL_OUB:	// NOT USED. User DLL_OUBx Only!!
		m_sync.Lock();
		m_bBigDigit = FALSE;
		dispatch((char *)lParam, HIWORD(wParam));
		m_sync.Unlock();
		break;

// 	case DLL_ALERT:
// 		if (HIWORD(wParam) == alert_DEIN)
// 			break;
// 		m_sync.Lock();
// 		//AfxMessageBox((char *)lParam);
// 		alert((char *)lParam);
// 		//TRACE((char *)lParam);
// 		
// 		EW_Alert((char *)lParam);
// 		m_sync.Unlock();
// 		break;
	case DLL_ALERTx:
		{
			if (HIWORD(wParam) == alert_DEIN)
				break;

			struct _alertR* alertR = (struct _alertR*)lParam;

			m_sync.Lock();
			alert(alertR);
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
		createFont(); Invalidate(FALSE);
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
	CString slog;
	slog.Format("[midprc] OnMouseLeave m_focus= [%d] ",m_focus);
	Output_DebugString(slog);


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
		else if (m_focus == MIDKPRICE && m_iShowMIDPRC)
		{  //중간가에 마우스를 올리면 색이 변하는데 이상태에서 아예 호가윈도우를 벗어나면 색 원복
			m_items.GetAt(m_focus)->m_pRGB = CLR_MIDPRC;
			m_focus = -1;
			InvalidateRect(m_prect);
		}
	}

	if (m_pToolTip->IsWindowVisible())
	{
		if (m_focus == MIDNPRICE)
			m_focus = -1;

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

	m_clrAskGs	= getColor(114);// 110);
	m_clrAskGe	= getColor(114);// 119);

	m_clrBidGs	= getColor(124);// 120);
	m_clrBidGe	= getColor(124);// 110);


	m_clrAskSzFg	= getColor(81);
	m_clrBidSzFg	= getColor(84);

	m_clrLine	= getColor(75);
	m_clrDash	= getColor(75);
	m_clrBox	= getColor(75);
	m_clrCurr	= getColor(86);
	m_clrBottom	= m_clrWhite;

	m_clrUp		= getColor(94);
	m_clrDown	= getColor(95);

	m_clrFocus	= getColor(86);
	
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
		
		if (ii >= askSize1 && ii <= askSize10)   //매수잔량
			item->m_fRGB = m_clrAskSzFg;
		else if (ii >= bidSize1 && ii <= bidSize10)  //매도잔량
			item->m_fRGB = m_clrBidSzFg;

		if (ii >= kmd1size && ii <= kmd10size)   //통합KRX매도잔량
			item->m_fRGB = m_clrAskSzFg;
		if (ii >= kms1size && ii <= kms10size)   //통합KRX매수잔량
			item->m_fRGB = m_clrAskSzFg;
		if (ii >= nmd1size && ii <= nmd10size)   //통합NXT매도잔량
			item->m_fRGB = m_clrAskSzFg;
		if (ii >= nms1size && ii <= nms10size)   //통합NXT매수잔량
			item->m_fRGB = m_clrAskSzFg;

		if (ii >= askPrice1 && ii <= askBefore10)
		{
			item->m_pRGB = SetItemBGColor(ii);
			if (ii >= askPrice1 && ii <= askPrice10)
			{
				switch (m_type)
				{
				case ctCode:
				case ctECN:
					item->m_attr = atCoRgbSup;	break;
				case ctFuture:
				case ctOption:
					item->m_attr = at2SignZero;	break;
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
				case ctCode:
				case ctECN:
					item->m_attr = atCoRgbSup;	break;
				case ctFuture:
				case ctOption:
					item->m_attr = at2SignZero;	break;
				}
			}
			else if (ii >= bidPercent1 && ii <= bidPercent10)
				item->m_fRGB = m_clrUp;
			else if (ii >= bidSize1 && ii <= bidSize10)
				item->m_attr = atSignSup;
			else	item->m_attr = atCoRgbSup;
		}
		else if (ii >= askLp1 && ii <= bidLp10)
		{
			item->m_pRGB = SetItemBGColor(ii);
			item->m_attr = atCoRgbSup;
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
			case askTotalLp:
			case bidTotalLp:
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
			case shPrice:
				item->m_fRGB = m_clrUp;
			case hhPrice:
				if (ii == hhPrice)
					item->m_fRGB = m_clrDown;
				item->m_attr = atComma;
				break;
			case resist2:
			case resist1:
			case pivotV:
			case support1:
			case support2:
			case resistD:
			case supportD:
				item->m_attr = atCoRgbSup;
				break;
			case gAMT:
				item->m_attr = atComma;
				break;
			case max52:
			case min52:
				item->m_attr = atComma;
				break;
			case chegang:
				item->m_attr = atCoRgbSup;
				break;
			case SVIUP:
				item->m_attr = atCoRgbSup;
				break;
			case SVIDN:
				item->m_attr = atCoRgbSup;
				break;
			case MIDKPRCDBMD:     //modi KRX 중간가매도대비
			case MIDNPRCDBMD:     //modi NXT 중간가매도대비
			case tkmiddr:      //통합용  KRX 중간가매도대비
			case tnmiddr:      //통합용  NXT 중간가매도대비
				item->m_attr = atCoRgbSup;
			break;
			case MIDKPRCMD:    //KRX 중간가매도잔량
			case MIDNPRCMD:    //NXT 중간가매도잔량
			case tkmiddv:		//통합용  KRX 중간가매도잔량
			case tnmiddv:		 //통합용  NXT 중간가매도잔량
				item->m_attr = atSignSup;
			break;
			case MIDKPRICE:  //KRX 중간가
			case MIDNPRICE:  //NXT 중간가
			case tkmid:    //통합용  KRX 중간가
			case tnmid:   //통합용  NXT 중간가
				{
					item->m_attr = atCoRgbSup;
					//item->m_center = true;
				}
				break;
			case MIDKPRCMS:   //KRX 중간가매수잔량
			case MIDNPRCMS:   //NXT 중간가매수잔량
			case tkmidsv:   //통합용  KRX 중간가매수잔량
			case tnmidsv:   //통합용  NXT 중간가매수잔량
				item->m_attr = atSignSup;
			break;
			case MIDKPRCDBMS:   //KRX 중간가매수대비
			case MIDNPRCDBMS:   //NXT 중간가매수대비
			case tkmidsr:   //통합용  KRX 중간가매수대비
			case tnmidsr:    //통합용  NXT 중간가매수대비
				item->m_attr = atCoRgbSup;
			break;
			case MIDKPRCRATE:   //KRX 중간가등락율
			case MIDNPRCRATE:   //NXT 중간가등락율
			case tkmidrate:      // 통합용  KRX 중간가등락율
			case tnmidrate:      // 통합용  NXT 중간가등락율
				item->m_attr = atCoRgbSup;
				break;
			}
		}
		m_items.Add(item);
	}
	
	setIndices();
	calculateRowHeight();
	calculateColumnWidth();

	getMaxSize();
	setItemRect();

	CString root, name, image;

	root = Variant(homeCC, "");
	name = Variant(nameCC, "");
	image.Format("%s\\%s\\", root, IMAGEDIR);
	if (m_pBitmapC1 == NULL) m_pBitmapC1 = getBitmap(image + _T("호가_설정.bmp"));
	if (m_pBitmapC2 == NULL) m_pBitmapC2 = getBitmap(image + _T("호가_설정_dn.bmp"));
	if (m_pBitmapP1 == NULL) m_pBitmapP1 = getBitmap(image + _T("호가_시간.bmp"));
	if (m_pBitmapP2 == NULL) m_pBitmapP2 = getBitmap(image + _T("호가_시간_dn.bmp"));
	if (m_pBitmapT1 == NULL) m_pBitmapT1 = getBitmap(image + _T("통합.BMP"));
	if (m_pBitmapS1 == NULL) m_pBitmapS1 = getBitmap(image + _T("개별.BMP"));  
	if (m_pBitmapNoData == NULL) m_pBitmapNoData = getBitmap(image + m_strNoDataBitmap);

	m_path.Format("%s\\%s\\%s\\", root, USRDIR, name);
	if (!ReadCondition(m_path, &m_config))
	{
		m_config.info = 1;
		m_config.mbong = 1;
		m_config.curr = 1;
		m_config.hbold = 1;
		m_config.sgrp = 1;
		m_config.hdyul = 1;
		m_config.sbong = 1;
		m_config.currset = 1;
		m_config.shl = 1;
		m_config.pre = 1;
		m_config.bshowMidPrc = 1;

		SaveCondition(m_path, &m_config);
	}

	InitEnv(); 
}

void Cdepth::setIndices()
{
	int	ii = 0;

	switch (m_type)
	{
	case ctCode:   case ctFuture:
	case ctOption: case ctKOption:
		for (ii = 0; ii < sizeof(keys)/sizeof(struct _keys); ii++)
			m_indices.SetAt(keys[ii].key, keys[ii].index);
		break;
	case ctECN:
		for (ii = 0; ii < sizeof(keyx)/sizeof(struct _keys); ii++)
			m_indices.SetAt(keyx[ii].key, keyx[ii].index);
	default:
		break;
	}
}

int CountOccurrences(const CString& str, TCHAR ch)
{
	int count = 0;
	int pos = 0;

	// CString::Find를 사용하여 ch 문자가 나타나는 위치를 반복적으로 찾습니다.
	while ((pos = str.Find(ch, pos)) != -1)
	{
		count++;
		pos++; // 다음 위치로 이동
	}

	return count;
}

void Cdepth::dispatch(char* datB, int datL)
{
	if (datL == 0)
	{
		clear();
		Invalidate();
		return;
	}
	
	m_slog.Format("[cx_depth][%s]<%d> datL=[%d]  sz_hoga=[%d]  sz_Extra=[%d] ", __FUNCTION__, __LINE__, datL, sz_hoga, sz_Extra);
	Output_DebugString(m_slog);

	//if (datL == sz_hoga + sz_Extra)  //testcode
	clear();  //testcode 우선 클리어
	if(1)
	{
		dispatchTEN(datB, datL);
		EW_Dispatch(datB, true);
		if (m_bSiseMemo)
			SendSiseMemo();

		//SearchChegang();
	}
	else if (datL == sz_hogax + sz_Extra)
	{
		dispatchFIVE(datB, datL);
		EW_Dispatch(datB, false);
		if (m_bSiseMemo)
			SendSiseMemo();

		//SearchChegang();
	}
	else if (datL < 50)
	{

		char* pdata = new char[datL + 1];
		memset(pdata, 0x00, datL + 1);
		memcpy(pdata, datB, datL);

		CString sval, svalue;
		sval.Format("%s", pdata);

		CString slog, stmp;


		if (CountOccurrences(sval, '\t') == 6)
		{
			stmp += "MD비 =";
			svalue = Parser(sval, "\t");
			if (svalue == "0")
				svalue.Empty();
			m_items.GetAt(MIDKPRCDBMD)->m_data = format(svalue, MIDKPRCDBMD);
			//m_items.GetAt(MIDKPRCDBMD)->m_data = format("+555555", MIDKPRCDBMD);    //nxt test
			//m_items.GetAt(MIDNPRCDBMD)->m_data = format("+555555", MIDKPRCDBMD);    //nxt test
			stmp += m_items.GetAt(MIDKPRCDBMD)->m_data;

			stmp += " MD잔량 =";
			svalue = Parser(sval, "\t");
			if (svalue == "0")
				svalue.Empty();
			m_items.GetAt(MIDKPRCMD)->m_data = format(svalue, MIDKPRCMD);;
			//m_items.GetAt(MIDKPRCMD)->m_data = format("+666666", MIDKPRCMD);   //nxt test
			//m_items.GetAt(MIDNPRCMD)->m_data = format("+666666", MIDKPRCMD);   //nxt test
			stmp += m_items.GetAt(MIDKPRCMD)->m_data;

			stmp += "   |중간가 =";
			svalue = Parser(sval, "\t");
			if (svalue == "0")
				svalue.Empty();
			m_strMidPrice = svalue;
			m_strMidPrice.Replace("+", "");
			m_strMidPrice.Replace("-", "");
			addComma(m_strMidPrice);
			m_items.GetAt(MIDKPRICE)->m_data = format(svalue, MIDKPRICE);
			//m_items.GetAt(MIDKPRICE)->m_data = format("+777777", MIDKPRICE);  //nxt test
			//m_items.GetAt(MIDNPRICE)->m_data = format("+787878", MIDKPRICE);  //nxt test
			stmp += m_items.GetAt(MIDKPRICE)->m_data;

			stmp += "   |매수잔량 =";
			svalue = Parser(sval, "\t");
			if (svalue == "0")
				svalue.Empty();
			m_items.GetAt(MIDKPRCMS)->m_data = format(svalue, MIDKPRCMS);
			//m_items.GetAt(MIDKPRCMS)->m_data = format("+888888", MIDKPRCMS);    //nxt test
			//m_items.GetAt(MIDNPRCMS)->m_data = format("+888888", MIDKPRCMS);    //nxt test
			stmp += m_items.GetAt(MIDKPRCMS)->m_data;

			stmp += "   |매수비 =";
			svalue = Parser(sval, "\t");
			if (svalue == "0")
				svalue.Empty();
			m_items.GetAt(MIDKPRCDBMS)->m_data = format(svalue, MIDKPRCDBMS);
			stmp += m_items.GetAt(MIDKPRCDBMS)->m_data;
			//m_items.GetAt(MIDKPRCDBMS)->m_data = format("+999999", MIDKPRCDBMS);   //nxt test
			//m_items.GetAt(MIDNPRCDBMS)->m_data = format("+999999", MIDKPRCDBMS);   //nxt test

			stmp += "   |등락율 =";
			svalue = Parser(sval, "\t");
			if (svalue == "0")
				svalue.Empty();
			if (svalue.Find("0.00") >= 0)
				m_items.GetAt(MIDKPRCRATE)->m_data = "0.00";
			else
				m_items.GetAt(MIDKPRCRATE)->m_data = format(svalue, MIDKPRCRATE);
			stmp += m_items.GetAt(MIDKPRCRATE)->m_data;
			//m_items.GetAt(MIDKPRCRATE)->m_data = format("+0.12", MIDKPRCRATE);   //nxt test
			//m_items.GetAt(MIDNPRCRATE)->m_data = format("+0.12", MIDKPRCRATE);   //nxt test
			Invalidate();
		}
	}

	if (m_bPercent)
	{
		m_nPriceH = (m_clos + (m_clos * abs(m_nHighPercent))/100);
		m_nPriceL = m_clos - (m_clos * abs(m_nLowPercent))/100;
	}
	else if (m_bDragMode)
	{
		m_bDragMode = FALSE;
	}
}

int Cdepth::CheckCodeMkType(CString sCode)
{
	if (sCode.Find("M.") < 0 && sCode.Find("N.") < 0)
		return mkKRX;
	if (sCode.Find("M.") >= 0)
		return mkTOT;
	if (sCode.Find("N.") >= 0)
		return mkNXT;

	return mkKRX;
}

void Cdepth::dispatchTEN(char* datB, int datL)
{
	CString	tmps;
	int	askI = 0, bidI = 0, askA = 0, bidA = 0;
	const	struct	_hoga* hoga = (struct _hoga *) datB;
	m_bIsNXTAble = TRUE;

	//if (hoga->bNXTAble[0] == 'N')
	if (hoga->fill[0] == 'N')
	{
		m_bIsNXTAble = FALSE;
		m_code.Empty();
		Invalidate();
		return;
	}

	m_code = CString(hoga->codx, sizeof(hoga->codx));
	m_code.Trim();

	if (CheckCodeMkType(m_code) != m_mkgubn)
	{
		m_mkgubn = (_mktype)CheckCodeMkType(m_code);
		SetMKtype(m_mkgubn);

		tmps.Format("marketvol  m_code = [%s]  m_mkgubn = [%d]", m_code, m_mkgubn);
		Output_DebugString(tmps);

		PostMessage(WM_PAINT);
		Invalidate();
	}


	bELW = IsELW(m_code);

	if (IsELW(m_code))
	{
		askI = askLp1;
		bidI = bidLp1;
		askA = askTotalLp;
		bidA = bidTotalLp;
	}
	else
	{
		askI = askBefore1;
		bidI = bidBefore1;
		askA = askTotalBefore;
		bidA = bidTotalBefore;
	}

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
		m_items.GetAt(askPrice1+ii)->m_data = format(tmps, askPrice1+ii);

		if (m_items.GetAt(askPrice1+ii)->m_data.GetLength() > 7)
		{
			if (m_bAble || m_config.hdyul == 0)
				m_bBigDigit = TRUE;
		}

		tmps = CString(hoga->price[ii].avol, sizeof(hoga->price[ii].avol));
		tmps.Trim();
		if (atoi(tmps) > 0)
			m_items.GetAt(askSize1+ii)->m_data = format(tmps, askSize1+ii);
		else
		{
			m_items.GetAt(askSize1+ii)->m_data  = _T("");
			m_items.GetAt(askPrice1+ii)->m_data = _T("");
		}

		tmps = CString(hoga->price[ii].acha, sizeof(hoga->price[ii].acha));
		m_items.GetAt(askI+ii)->m_data = format(tmps, askI+ii);

		tmps = CString(hoga->price[ii].bid, sizeof(hoga->price[ii].bid));
		m_items.GetAt(bidPrice1+ii)->m_data = format(tmps, bidPrice1+ii);

		if (m_items.GetAt(bidPrice1+ii)->m_data.GetLength() > 7)
		{
			if (m_bAble || m_config.hdyul == 0)
				m_bBigDigit = TRUE;
		}

		tmps = CString(hoga->price[ii].bvol, sizeof(hoga->price[ii].bvol));
		if (atoi(tmps) > 0)
			m_items.GetAt(bidSize1+ii)->m_data = format(tmps, bidSize1+ii);
		else
		{
			m_items.GetAt(bidSize1+ii)->m_data  = _T("");
			m_items.GetAt(bidPrice1+ii)->m_data = _T("");
		}

		tmps = CString(hoga->price[ii].bcha, sizeof(hoga->price[ii].bcha));
		m_items.GetAt(bidI+ii)->m_data = format(tmps, bidI+ii);
	}
	m_items.GetAt(gVolume)->m_data = format(CString(hoga->gvol, sizeof(hoga->gvol)), gVolume);
	m_items.GetAt(cTime)->m_data = format(CString(hoga->htim, sizeof(hoga->htim)), cTime);
	m_items.GetAt(askTotalSize)->m_data   = format(CString(hoga->avols, sizeof(hoga->avols)), askTotalSize);
	m_items.GetAt(askA)->m_data = format(CString(hoga->acha, sizeof(hoga->acha)), askA);
	m_items.GetAt(bidTotalSize)->m_data   = format(CString(hoga->bvols, sizeof(hoga->bvols)), bidTotalSize);
	m_items.GetAt(bidA)->m_data = format(CString(hoga->bcha, sizeof(hoga->bcha)), bidA);

	m_items.GetAt(openPrice)->m_data  = format(CString(hoga->siga, sizeof(hoga->siga)), openPrice);
	m_items.GetAt(openPrice)->m_data.TrimLeft();
	if (!m_items.GetAt(openPrice)->m_data.IsEmpty())
		m_bSiga = true;
	m_items.GetAt(highPrice)->m_data  = format(CString(hoga->koga, sizeof(hoga->koga)), highPrice);
	m_items.GetAt(lowPrice)->m_data   = format(CString(hoga->jega, sizeof(hoga->jega)), lowPrice);
	m_items.GetAt(paperPrice)->m_data = format(CString(hoga->amga, sizeof(hoga->amga)), paperPrice);

	m_items.GetAt(preBefore)->m_data   = format(CString(hoga->diff, sizeof(hoga->diff)), preBefore);
	m_items.GetAt(prePercent)->m_data = format(CString(hoga->rate, sizeof(hoga->rate)), prePercent);

	m_items.GetAt(prePrice)->m_data  = format(CString(hoga->ysga, sizeof(hoga->ysga)), prePrice);
	m_items.GetAt(preVolume)->m_data = format(CString(hoga->yvol, sizeof(hoga->yvol)), preVolume);

	m_items.GetAt(pivotPrice)->m_data = format(CString(hoga->gjga, sizeof(hoga->gjga)), pivotPrice);
	m_items.GetAt(shPrice)->m_data = format(CString(hoga->shga, sizeof(hoga->shga)), shPrice);
	m_items.GetAt(hhPrice)->m_data = format(CString(hoga->hhga, sizeof(hoga->hhga)), hhPrice);
	m_items.GetAt(gajungPrice)->m_data = format(CString(hoga->pgga, sizeof(hoga->pgga)), gajungPrice);

	m_items.GetAt(sojinRate)->m_data = format(CString(hoga->sjrt, sizeof(hoga->sjrt)), sojinRate);

	m_items.GetAt(askSizeTime)->m_data       = format(CString(hoga->ahvol, sizeof(hoga->ahvol)), askSizeTime);
	m_items.GetAt(askSizeTimeBefore)->m_data = format(CString(hoga->ahvc, sizeof(hoga->ahvc)), askSizeTimeBefore);
	m_items.GetAt(bidSizeTime)->m_data       = format(CString(hoga->bhvol, sizeof(hoga->bhvol)), bidSizeTime);
	m_items.GetAt(bidSizeTimeBefore)->m_data = format(CString(hoga->bhvc, sizeof(hoga->bhvc)), bidSizeTimeBefore);

	if (str2double(m_items.GetAt(prePrice)->m_data))
		m_showPreValue = true;
	else
		m_showPreValue = false;

	m_items.GetAt(sizeDiff)->m_data = format(CString(hoga->dscha, sizeof(hoga->dscha)), sizeDiff);
	m_ma5 = str2double(CString(hoga->gsm5, sizeof(hoga->gsm5)));
	m_ma10 = str2double(CString(hoga->gsm10, sizeof(hoga->gsm10)));
	m_items.GetAt(jiVolume)->m_data = format(CString(hoga->jivl, sizeof(hoga->jivl)), jiVolume);
	m_items.GetAt(evidence)->m_data = format(CString(hoga->jggy, sizeof(hoga->jggy)), evidence);

	m_items.GetAt(resist2)->m_data = format(CString(hoga->res2, sizeof(hoga->res2)), resist2);
	m_items.GetAt(resist1)->m_data = format(CString(hoga->res1, sizeof(hoga->res1)), resist1);
	m_items.GetAt(pivotV)->m_data = format(CString(hoga->pivt, sizeof(hoga->pivt)), pivotV);
	m_items.GetAt(support1)->m_data = format(CString(hoga->spp1, sizeof(hoga->spp1)), support1);
	m_items.GetAt(support2)->m_data = format(CString(hoga->spp2, sizeof(hoga->spp2)), support2);
	m_items.GetAt(resistD)->m_data = format(CString(hoga->resd, sizeof(hoga->resd)), resistD);
	m_items.GetAt(supportD)->m_data = format(CString(hoga->sppd, sizeof(hoga->sppd)), supportD);
	
	m_items.GetAt(gAMT)->m_data = format(CString(hoga->gamt, sizeof(hoga->gamt)), gAMT);
	m_items.GetAt(max52)->m_data = format(CString(hoga->whga, sizeof(hoga->whga)), max52);
	m_items.GetAt(min52)->m_data = format(CString(hoga->wlga, sizeof(hoga->wlga)), min52);
	m_items.GetAt(chegang)->m_data = format(CString(hoga->cpow, sizeof(hoga->cpow)), chegang);

	m_items.GetAt(SVIUP)->m_data = format(CString(hoga->viss, sizeof(hoga->viss)), SVIUP);
	m_items.GetAt(SVIDN)->m_data = format(CString(hoga->vihr, sizeof(hoga->vihr)), SVIDN);

	CString stmp, sresult;
	int icntTick{};
	int icntBong;
	icntTick = atoi(CString(hoga->ntick, sizeof(hoga->ntick)));
	icntBong = atoi(CString(hoga->nbong, sizeof(hoga->nbong)));
	
	//if (m_code.Left(2) == "M." || m_code.Left(2) == "N.")   //통합호가인경우 
	{
		for (int ii = 0; ii < 10; ii++)
		{
			sresult = "[cx_depth]marketvol K매도잔량 ";
			stmp = CString(hoga->hogavol[ii].kdvol, sizeof(hoga->hogavol[ii].kdvol));
			sresult += stmp;
			sresult += "    K매수잔량 ";
			stmp = CString(hoga->hogavol[ii].ksvol, sizeof(hoga->hogavol[ii].ksvol));
			sresult += stmp;
			sresult += "    N매도잔량 ";
			stmp = CString(hoga->hogavol[ii].ndvol, sizeof(hoga->hogavol[ii].ndvol));
			sresult += stmp;
			sresult += "    N매수잔량 ";
			stmp = CString(hoga->hogavol[ii].nsvol, sizeof(hoga->hogavol[ii].nsvol));
			sresult += stmp;
			sresult += "\n";

			//OutputDebugString(sresult);
			sresult.Empty();

			//KRX 매도수량
			stmp = CString(hoga->hogavol[ii].kdvol, sizeof(hoga->hogavol[ii].kdvol));
			stmp.Trim();
			if (atoi(stmp) > 0)
				m_items.GetAt(kmd1size + ii)->m_data = format(stmp, kmd1size + ii);
			else
				m_items.GetAt(kmd1size + ii)->m_data = _T("");

			//KRX 매수수량
			stmp = CString(hoga->hogavol[ii].ksvol, sizeof(hoga->hogavol[ii].ksvol));
			stmp.Trim();
			if (atoi(stmp) > 0)
				m_items.GetAt(kms1size + ii)->m_data = format(stmp, kms1size + ii);
			else
				m_items.GetAt(kms1size + ii)->m_data = _T("");

			//NXT 매도수량
			stmp = CString(hoga->hogavol[ii].ndvol, sizeof(hoga->hogavol[ii].ndvol));
			stmp.Trim();
			if (atoi(stmp) > 0)
				m_items.GetAt(nmd1size + ii)->m_data = format(stmp, nmd1size + ii);
			else
				m_items.GetAt(nmd1size + ii)->m_data = _T("");

			//NXT 매수수량
			stmp = CString(hoga->hogavol[ii].nsvol, sizeof(hoga->hogavol[ii].nsvol));
			stmp.Trim();
			if (atoi(stmp) > 0)
				m_items.GetAt(nms1size + ii)->m_data = format(stmp, nms1size + ii);
			else
				m_items.GetAt(nms1size + ii)->m_data = _T("");
		}

		m_items.GetAt(MIDKPRCMD)->m_data = format(CString(hoga->kmdmq, sizeof(hoga->kmdmq)), MIDKPRCMD);  //KRX중간가매도잔량
		m_items.GetAt(tkmiddv)->m_data = format(CString(hoga->kmdmq, sizeof(hoga->kmdmq)), tkmiddv);

		m_items.GetAt(MIDKPRICE)->m_data = format(CString(hoga->kmiga, sizeof(hoga->kmiga)), MIDKPRICE);  //KRX중간가
		m_items.GetAt(tkmid)->m_data = format(CString(hoga->kmiga, sizeof(hoga->kmiga)), tkmid);

		m_items.GetAt(MIDKPRCMS)->m_data = format(CString(hoga->kmsmq, sizeof(hoga->kmsmq)), MIDKPRCMS);  //KRX중간가매수잔량
		m_items.GetAt(tkmidsv)->m_data = format(CString(hoga->kmsmq, sizeof(hoga->kmsmq)), tkmidsv);

		m_items.GetAt(MIDKPRCDBMD)->m_data = format(CString(hoga->kmdha, sizeof(hoga->kmdha)), MIDKPRCDBMD);  //KRX중간가매도대비
		m_items.GetAt(tkmiddr)->m_data = format(CString(hoga->kmdha, sizeof(hoga->kmdha)), tkmiddr);

		m_items.GetAt(MIDKPRCDBMS)->m_data = format(CString(hoga->kmsha, sizeof(hoga->kmsha)), MIDKPRCDBMS);  //KRX중간가매수대비
		m_items.GetAt(tkmidsr)->m_data = format(CString(hoga->kmsha, sizeof(hoga->kmsha)), tkmidsr);

		CString stmp;
		m_items.GetAt(MIDKPRCRATE)->m_data = format(CString(hoga->krate, sizeof(hoga->krate)), MIDKPRCRATE);  //KRX중간가 등락율
		stmp.Format("%s", CString(hoga->krate, sizeof(hoga->krate)));
		if (stmp.Find("inf") >= 0)
			m_items.GetAt(MIDKPRCRATE)->m_data = "";
	
		m_items.GetAt(tkmidrate)->m_data = format(CString(hoga->krate, sizeof(hoga->krate)), tkmidrate);
		
		sresult.Format("[%s]<%d>[depth] marketvol[K]  매도대비 [%s] 매도잔량 [%s]  중간가[168][%s]  매수잔량 [%s] 매수대비 [%s] 등락율[%s]",
			__FUNCTION__, __LINE__,
			m_items.GetAt(MIDKPRCDBMD)->m_data,
			m_items.GetAt(MIDKPRCMD)->m_data,
			m_items.GetAt(MIDKPRICE)->m_data,
			m_items.GetAt(MIDKPRCMS)->m_data,
			m_items.GetAt(MIDKPRCDBMS)->m_data,
			m_items.GetAt(MIDKPRCRATE)->m_data);
		Output_DebugString(sresult);
		//m_items.GetAt(MIDKPRCDBMD)->m_data = "11111";
		//m_items.GetAt(MIDKPRCMD)->m_data = "22222";
		//m_items.GetAt(MIDKPRICE)->m_data = "33333";
		//m_items.GetAt(MIDKPRCMS)->m_data = "44444";
		//m_items.GetAt(MIDKPRCDBMS)->m_data = "55555";
		//m_items.GetAt(MIDKPRCRATE)->m_data = "66.66";


		m_items.GetAt(MIDNPRCMD)->m_data = format(CString(hoga->nmdmq, sizeof(hoga->nmdmq)), MIDNPRCMD);  //NXT중간가매도잔량
		m_items.GetAt(tnmiddv)->m_data = format(CString(hoga->nmdmq, sizeof(hoga->nmdmq)), tnmiddv);

		m_items.GetAt(MIDNPRICE)->m_data = format(CString(hoga->nmiga, sizeof(hoga->nmiga)), MIDNPRICE);  //NXT중간가
		m_items.GetAt(tnmid)->m_data = format(CString(hoga->nmiga, sizeof(hoga->nmiga)), tnmid);

		m_items.GetAt(MIDNPRCMS)->m_data = format(CString(hoga->nmsmq, sizeof(hoga->nmsmq)), MIDNPRCMS);  //NXT중간가매수잔량
		m_items.GetAt(tnmidsv)->m_data = format(CString(hoga->nmsmq, sizeof(hoga->nmsmq)), tnmidsv);

		m_items.GetAt(MIDNPRCDBMD)->m_data = format(CString(hoga->nmdha, sizeof(hoga->nmdha)), MIDNPRCDBMD);  //NXT중간가매도대비
		m_items.GetAt(tnmiddr)->m_data = format(CString(hoga->nmdha, sizeof(hoga->nmdha)), tnmiddr);

		m_items.GetAt(MIDNPRCDBMS)->m_data = format(CString(hoga->nmsha, sizeof(hoga->nmsha)), MIDNPRCDBMS);  //NXT중간가매수대비
		m_items.GetAt(tnmidsr)->m_data = format(CString(hoga->nmsha, sizeof(hoga->nmsha)), tnmidsr);

		m_items.GetAt(MIDNPRCRATE)->m_data = format(CString(hoga->nrate, sizeof(hoga->nrate)), MIDNPRCRATE);  //NXT중간가등락률
		stmp.Format("%s", CString(hoga->nrate, sizeof(hoga->nrate)));
		if (stmp.Find("inf") >= 0)
			m_items.GetAt(MIDNPRCRATE)->m_data = "";
	
		m_items.GetAt(tnmidrate)->m_data = format(CString(hoga->nrate, sizeof(hoga->nrate)), tnmidrate);

		sresult.Format("[%s]<%d>[depth] marketvol[N]  매도대비 [%s] 매도잔량 [%s]  중간가[172][%s]  매수잔량 [%s] 매수대비 [%s] 등락율[%s]",
			__FUNCTION__,__LINE__,
			m_items.GetAt(MIDNPRCDBMD)->m_data,
			m_items.GetAt(MIDNPRCMD)->m_data,
			m_items.GetAt(MIDNPRICE)->m_data,
			m_items.GetAt(MIDNPRCMS)->m_data,
			m_items.GetAt(MIDNPRCDBMS)->m_data,
			m_items.GetAt(MIDNPRCRATE)->m_data);
		Output_DebugString(sresult);

		m_items.GetAt(tkdtjan)->m_data = format(CString(hoga->kdvol, sizeof(hoga->kdvol)),
			tkdtjan);  //KRX매도호가총수량
		m_items.GetAt(tkstjan)->m_data = format(CString(hoga->ksvol, sizeof(hoga->ksvol)),
			tkstjan);  //KRX매수호가총수량
		m_items.GetAt(tndtjan)->m_data = format(CString(hoga->ndvol, sizeof(hoga->ndvol)),
			tndtjan);  //NXT매도호가총수량
		m_items.GetAt(tnstjan)->m_data = format(CString(hoga->nsvol, sizeof(hoga->nsvol)),
			tnstjan);  //NXT매수호가총수량

		//m_items.GetAt(MIDNPRCDBMD)->m_data = "66666";
		//m_items.GetAt(MIDNPRCMD)->m_data = "77777";
		//m_items.GetAt(MIDNPRICE)->m_data = "88888";
		//m_items.GetAt(MIDNPRCMS)->m_data = "99999";
		//m_items.GetAt(MIDNPRCDBMS)->m_data = "121212";
		//m_items.GetAt(MIDNPRCRATE)->m_data = "77.66";
	}

	calculateRowHeight();
	calculateColumnWidth();
	EW_Move();
	//TRACE("hoga->gamt ] "+m_items.GetAt(gAMT))->m_data);
	setItemRect();
	calculatePercent();
	getMaxSize();
	if (m_type == ctCode || m_type == ctECN)
		calculatePreValue();

	Invalidate();
}

void Cdepth::dispatchFIVE(char* datB, int datL)
{
	CString	tmps;
	int	askI = 0, bidI = 0, askA = 0, bidA = 0;
	const	struct	_hogax* hoga = (struct _hogax *) datB;

	m_code = CString(hoga->codx, sizeof(hoga->codx));
	m_code.Trim();
	//m_code.Replace("N.", "");
	if (IsELW(m_code))
	{
		askI = askLp1;
		bidI = bidLp1;
		askA = askTotalLp;
		bidA = bidTotalLp;
	}
	else
	{
		askI = askBefore1;
		bidI = bidBefore1;
		askA = askTotalBefore;
		bidA = bidTotalBefore;
	}

	for (int ii = 0; ii < 5; ii++)
	{
		tmps = CString(hoga->price[ii].ask, sizeof(hoga->price[ii].ask));
		m_items.GetAt(askPrice1+ii)->m_data = format(tmps, askPrice1+ii);

		if (m_items.GetAt(askPrice1+ii)->m_data.GetLength() > 7)
		{
			if (m_bAble || m_config.hdyul == 0)
				m_bBigDigit = TRUE;
		}

		tmps = CString(hoga->price[ii].avol, sizeof(hoga->price[ii].avol));
		tmps.Trim();
		if (atoi(tmps) > 0)
			m_items.GetAt(askSize1+ii)->m_data = format(tmps, askSize1+ii);
		else
		{
			m_items.GetAt(askSize1+ii)->m_data  = _T("");
			m_items.GetAt(askPrice1+ii)->m_data = _T("");
		}

		tmps = CString(hoga->price[ii].acha, sizeof(hoga->price[ii].acha));
		m_items.GetAt(askI+ii)->m_data = format(tmps, askI+ii);

		tmps = CString(hoga->price[ii].bid, sizeof(hoga->price[ii].bid));
		m_items.GetAt(bidPrice1+ii)->m_data = format(tmps, bidPrice1+ii);

		if (m_items.GetAt(bidPrice1+ii)->m_data.GetLength() > 7)
		{
			if (m_bAble || m_config.hdyul == 0)
				m_bBigDigit = TRUE;
		}

		tmps = CString(hoga->price[ii].bvol, sizeof(hoga->price[ii].bvol));
		if (atoi(tmps) > 0)
			m_items.GetAt(bidSize1+ii)->m_data = format(tmps, bidSize1+ii);
		else
		{
			m_items.GetAt(bidSize1+ii)->m_data  = _T("");
			m_items.GetAt(bidPrice1+ii)->m_data = _T("");
		}

		tmps = CString(hoga->price[ii].bcha, sizeof(hoga->price[ii].bcha));
		m_items.GetAt(bidI+ii)->m_data = format(tmps, bidI+ii);
	}

	tmps = CString(hoga->curr, sizeof(hoga->curr));
	tmps.Trim();
	m_curr = fabs(str2double(tmps));

	tmps = CString(hoga->pcls, sizeof(hoga->pcls));
	tmps.Trim();
	m_clos = str2double(tmps);
	m_items.GetAt(cloPrice)->m_data = format(tmps, cloPrice);

	m_items.GetAt(cTime)->m_data = format(CString(hoga->htim, sizeof(hoga->htim)), cTime);
	m_items.GetAt(askTotalSize)->m_data   = format(CString(hoga->avols, sizeof(hoga->avols)), askTotalSize);
	m_items.GetAt(askA)->m_data = format(CString(hoga->acha, sizeof(hoga->acha)), askA);
	m_items.GetAt(bidTotalSize)->m_data   = format(CString(hoga->bvols, sizeof(hoga->bvols)), bidTotalSize);
	m_items.GetAt(bidA)->m_data = format(CString(hoga->bcha, sizeof(hoga->bcha)), bidA);

	m_items.GetAt(openPrice)->m_data  = format(CString(hoga->siga, sizeof(hoga->siga)), openPrice);
	m_items.GetAt(openPrice)->m_data.TrimLeft();
	if (!m_items.GetAt(openPrice)->m_data.IsEmpty())
		m_bSiga = true;
	m_items.GetAt(highPrice)->m_data  = format(CString(hoga->koga, sizeof(hoga->koga)), highPrice);
	m_items.GetAt(lowPrice)->m_data   = format(CString(hoga->jega, sizeof(hoga->jega)), lowPrice);
	m_items.GetAt(paperPrice)->m_data = format(CString(hoga->amga, sizeof(hoga->amga)), paperPrice);

	m_items.GetAt(prePrice)->m_data  = format(CString(hoga->ysga, sizeof(hoga->ysga)), prePrice);
	m_items.GetAt(preVolume)->m_data = format(CString(hoga->yvol, sizeof(hoga->yvol)), preVolume);

	m_items.GetAt(pivotPrice)->m_data = format(CString(hoga->gjga, sizeof(hoga->gjga)), pivotPrice);
	m_items.GetAt(shPrice)->m_data = format(CString(hoga->shga, sizeof(hoga->shga)), shPrice);
	m_items.GetAt(hhPrice)->m_data = format(CString(hoga->hhga, sizeof(hoga->hhga)), hhPrice);
	m_items.GetAt(gajungPrice)->m_data = format(CString(hoga->pgga, sizeof(hoga->pgga)), gajungPrice);
	m_items.GetAt(sojinRate)->m_data = format(CString(hoga->sjrt, sizeof(hoga->sjrt)), sojinRate);

	m_items.GetAt(askSizeTime)->m_data       = format(CString(hoga->ahvol, sizeof(hoga->ahvol)), askSizeTime);
	m_items.GetAt(askSizeTimeBefore)->m_data = format(CString(hoga->ahvc, sizeof(hoga->ahvc)), askSizeTimeBefore);
	m_items.GetAt(bidSizeTime)->m_data       = format(CString(hoga->bhvol, sizeof(hoga->bhvol)), bidSizeTime);
	m_items.GetAt(bidSizeTimeBefore)->m_data = format(CString(hoga->bhvc, sizeof(hoga->bhvc)), bidSizeTimeBefore);

	if (str2double(m_items.GetAt(prePrice)->m_data))
		m_showPreValue = true;
	else
		m_showPreValue = false;

	setItemRect();
	calculatePercent();
	getMaxSize();
	if (m_type == ctCode || m_type == ctECN)
		calculatePreValue();

	Invalidate();
}

void Cdepth::dispatchMEMO(char *datB, int datL)
{
	CString tmpS;
	const	struct	mod *pMod = (struct mod*)datB;
	int	nrec = atoi(CString(pMod->nrec, sizeof(pMod->nrec)));

	if (datL != (int)(sz_mod + sz_gridO * nrec))
		return;

	for (int ii = 0; ii < nrec; ii++)
	{
		struct grid_o *pGrid = (struct grid_o*)(datB + sz_mod + ii * sz_gridO);
		std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(ii);
		pMemo->val.Format("%.8s", pGrid->vals);
		pMemo->val.Trim();
		addComma(pMemo->val);
	}
	Invalidate();
}

void Cdepth::alert(CString data)
{
	data = data.Mid(0,data.Find('\n'));
	CRect	rect;
	CString	syms, text;
	int	pos = 0, index = 0;
	bool	changePrice = false;
	bool	changeSize = false;
	bool	changeCurr = false;
	bool	changePre = false;
	std::shared_ptr<Citem> item = nullptr;

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
			if (!m_indices.Lookup(atoi(syms), index))
			{
				if (atoi(syms) == CURR)
				{
					if (m_bSiseMemo)
					{
						for (int jj = 0; jj < m_sisememo.GetSize(); jj++)
						{
							std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(jj);
							if (pMemo->gubn == 4 && atoi(pMemo->dat2) == 1)
							{
								pMemo->val.Format("%d", 
									(int)(str2double(pMemo->val) - m_curr + fabs(str2double(text))));
							}
						}
					}
					m_curr = fabs(str2double(text));
					rect.UnionRect(rect, CRect(0,0,10,10));
					changeCurr = true;
				}
				else if (atoi(syms) == 39) //가중평균가
				{
					if (text.Find("+") != -1)
					{
						text.Delete(0,1);
						m_items.GetAt(gajungPrice)->m_fRGB = m_clrUp;
					}
					else if (text.Find("-") != -1)
					{
						text.Delete(0,1);
						m_items.GetAt(gajungPrice)->m_fRGB = m_clrDown;
					}
					else
					{
						m_items.GetAt(gajungPrice)->m_fRGB = m_clrDataFg;
					}

					addComma(text);
					
					m_items.GetAt(gajungPrice)->m_data = text;
				}
				
				continue;
			}
#ifdef LP_AMOUNT
			if (IsELW(m_code))
			{
				if ((index >= askBefore1 && index <= askBefore10) || (index >= bidBefore1 && index <= bidBefore10) ||
					index == askTotalBefore || index == bidTotalBefore)
					continue;
			}
			else
			{
				if ((index >= askLp1 && ii <= askLp10) || (index >= bidLp1 && index <= bidLp10) ||
					index == askTotalLp || index == bidTotalLp)
					continue;
			}
#endif
			item = m_items.GetAt(index);
			text = format(text, index);
			if (item->m_data.CompareNoCase(text))
			{
				item->m_data = text;
				
				if (atoi(syms) == 515)	// 가중평균가
				{
					if (m_bSiseMemo)
					{
						for (int jj = 0; jj < m_sisememo.GetSize(); jj++)
						{
							std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(jj);
							if (pMemo->gubn == 1 && atoi(pMemo->dat1) == 5 && atoi(pMemo->dat2) == 2)
							{
								pMemo->val.Format("%d", (int)fabs(str2double(text)));
								addComma(pMemo->val);
							}
						}
					}
				}
				rect.UnionRect(rect, item->m_fRc);
				if ((index >= askPrice1 && index <= askPrice10) || (index >= bidPrice1 && index <= bidPrice10))
					changePrice = true;
				else if ((index >= askSize1 && index <= askSize10) || (index >= bidSize1 && index <= bidSize10))
					changeSize = true;
				else if (index >= prePrice && index <= preVolume)
					changePre = true;
			}

		}
		else
		{
			syms = text;
		}
	}

	calculatePreValue();
	if (changePrice || changeCurr)
	{
		if (m_percent)
			rect.UnionRect(rect, getPercentRect());
		else
			rect.UnionRect(rect, m_prect);
	}

	if (changeSize && (m_bar == grBar || m_bar == grBarText))
		getMaxSize();

	if ((m_type == ctCode || m_type == ctECN) && changePre)
	{
		item = m_items.GetAt(prePrice);
		if (!m_showPreValue && !item->m_data.IsEmpty())
		{
			m_showPreValue = true;
			setItemRect();
		}
		else
		{
			if (item->m_data.IsEmpty())
			{
				m_showPreValue = false;
				for (int ii = prePrice; ii < preVolume; ii++)
				{
					item = m_items.GetAt(ii);
					rect.UnionRect(rect, item->m_bRc);
					item->m_data = _T("");
				}
				setItemRect();
			}
		}
		calculatePreValue();
		for (int ii = prePrice; ii <= preVolume; ii++)
			rect.UnionRect(rect, m_items.GetAt(ii)->m_bRc);
	}

	if (m_pRgn && !rect.IsRectEmpty())
		InvalidateRgn(m_pRgn.get());
}

void Cdepth::alert(struct _alertR* alertR)  //실제 실시간처리
{
	CRect	rect;
	CString	syms, text, stmp;
	int	index = 0;
	bool	changePrice = false;
	bool	changeSize = false;
	bool	changeCurr = false;
	bool	changePre = false;
	std::shared_ptr<Citem> item = nullptr;
	DWORD* data = (DWORD*)alertR->ptr[0];

	if (m_code != alertR->code.TrimRight())
		return;
	
	rect.SetRectEmpty();
	if (data[23])
	{
		text = (char*)data[23];
		if (m_bSiseMemo)
		{
			for (int jj = 0; jj < m_sisememo.GetSize(); jj++)
			{
				std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(jj);
				if (pMemo->gubn == 4 && atoi(pMemo->dat2) == 1)
				{
					pMemo->val.Format("%d", (int)(str2double(pMemo->val) - m_curr + fabs(str2double(text))));
				}
			}
		}
		m_curr = fabs(str2double(text));
	
		rect.UnionRect(rect, CRect(0,0,10,10));
		changeCurr = true;
	}

	if (m_bDragMode)
	{
		if (m_nPriceH >= m_curr && m_curr >= m_nPriceL)
		{
			m_clrWhite = RGB(255,255,150);
		}
		else
		{
			m_clrWhite = RGB(255,255,255);
		}
	}
	else if (m_bPercent)
	{
		if (m_nPriceH < m_curr || m_nPriceL > m_curr)
		{
			m_clrWhite = RGB(255,255,150);
		}
		else
		{
			m_clrWhite = RGB(255,255,255);
		}
	}

	if (data[39]) //가중평균가
	{
		text = (char*)data[39];
//					TRACE("text [%s]\n",text);
		if (text.Find("+") != -1)
		{
			text.Delete(0,1);
			m_items.GetAt(gajungPrice)->m_fRGB = m_clrUp;
		}
		else if (text.Find("-") != -1)
		{
			text.Delete(0,1);
			m_items.GetAt(gajungPrice)->m_fRGB = m_clrDown;
		}
		else
		{
			m_items.GetAt(gajungPrice)->m_fRGB = m_clrDataFg;
		}

		addComma(text);
			
		m_items.GetAt(gajungPrice)->m_data = text;//atoi(text);
	}

	if (data[36]) //체결강도
	{
		text = (char*)data[36];
		m_items.GetAt(chegang)->m_data = format(text, chegang);
	}

	if (data[241]) //예상VI 상승 
	{
		text = (char*)data[241];
		m_items.GetAt(SVIUP)->m_data = format(text, SVIUP);
	}

	if (data[258]) //예상VI 하락
	{
		text = (char*)data[258];
		m_items.GetAt(SVIDN)->m_data = format(text, SVIDN);
	}

	int ival = sizeof(keys) / sizeof(struct _keys);
	m_slog.Format("[depth] code=[%s] [%d] -------------------------------------]", m_code, ival);
	Output_DebugString(m_slog);

	for (int ii = 0, key = 0; ii < sizeof(keys)/sizeof(struct _keys); ii++)
	{
		key = keys[ii].key;     
		index = keys[ii].index;

		if (key == 555 || key == 561 || key == 146)
		{
			switch (key)
			{
				case 555:    stmp = "통KRX중간가";   break;
				case 561:    stmp = "통KRX중간가";   break;
				case 146:    stmp = "K,N중간가";   break;
				default: stmp = ""; break;
			}

			m_slog.Format("!!!!!! [%s]<%d> [depth] code=[%s]  index=%d key=<%d>  data=[%s] [%s]", __FUNCTION__, __LINE__,    //중간가 실시간
				m_code,  index, key, data[key], stmp);
			Output_DebugString(m_slog);

		}

		if (key >= 51  && key <= 60)
		{
			m_slog.Format(" [%s]<%d> [depth] code=[%s]  index=%d key=%d  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__,   //매도호가실시간
				m_code, index, key, data[key],"매도", key - 50,"호가");
			Output_DebugString(m_slog);
		}
		if (key >= 71 && key <= 80)
		{
			m_slog.Format(" [%s]<%d> [depth] code=[%s]  index=%d key=%d  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__,  //매수호가실시간
				m_code, index, key, data[key], "매수", key - 70, "호가");
			Output_DebugString(m_slog);
		}

		switch (key)
		{
			case 551:
			{
				m_slog.Format("[depth][통합잔량] ");
				Output_DebugString(m_slog);
				m_slog.Format("[depth][통합잔량] code=[%s]  index=[%d] key=[%d] KRX매도=[%s]",
					m_code, index, key, data[key]);
				Output_DebugString(m_slog);
			}
			break;
			case 552:
			{
				m_slog.Format("[depth][통합잔량] code=[%s]  index=[%d] key=[%d] KRX매수=[%s]",
					m_code, index, key, data[key]);
				Output_DebugString(m_slog);
			}
			break;
			case 553:
			{
				m_slog.Format("[depth][통합잔량] code=[%s]  index=[%d] key=[%d] NXT매도=[%s]",
					m_code, index, key, data[key]);
				Output_DebugString(m_slog);
			}
			break;
			case 554:
			{
				m_slog.Format("[depth][통합잔량] code=[%s]  index=[%d] key=[%d] NXT매수=[%s]",
					m_code, index, key, data[key]);
				Output_DebugString(m_slog);
			}
			break;
		}
		

		if (!data[key])
		{
			continue;
		}

		text = (char*)data[key];
			
#ifdef LP_AMOUNT
		if (IsELW(m_code))
		{
			if ((index >= askBefore1 && index <= askBefore10) || (index >= bidBefore1 && index <= bidBefore10) ||
				index == askTotalBefore || index == bidTotalBefore)
				continue;
		}
		else
		{
			if (index >= MIDKPRCMD && index <= MIDKPRCRATE && m_iShowMIDPRC)
			{
				if (index == MIDKPRICE)
				{
					m_strMidPrice = text;
					m_strMidPrice.Replace("+", "");
					m_strMidPrice.Replace("-", "");
					addComma(m_strMidPrice);
				}
			}
			else if ((index >= askLp1 && index <= askLp10) || (index >= bidLp1 && index <= bidLp10) ||
				index == askTotalLp || index == bidTotalLp)
				continue;
		}
#endif
		item = m_items.GetAt(index);
		text = format(text, index);

		if (index == MIDKPRCRATE && key == 181)
			text.TrimRight();

		if (index >= kmd1size && index <= nms10size)   //통합일경우에만 사용됨
		{//511 ~ 550   통합조회후 개별선택할때 사용되는 KRX,NXT 매도매수 잔량
			if (m_mkgubn == mkTOT || m_iDataType == DF_SEP)  //다른경우는 사용안됨
			{
				if ((index >= kmd1size) && (index <= kmd10size))
				{
					m_slog.Format(" [%s]<%d> [depth] code=[%s]  index=%d key=%d  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__,  
						m_code, index, key, data[key], "KRX 통합 매수잔량", index - 185 + 1, "호가");
				}
				else if ((index >= kms1size) && (index <= kms10size))
				{
					m_slog.Format(" [%s]<%d> [depth] code=[%s]  index=%d key=%d  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__,  
						m_code, index, key, data[key], "KRX 통합 매수잔량", index - 195 + 1, "호가");
				}
				else if((index >= nmd1size) && (index <= nmd10size))
				{
					m_slog.Format(" [%s]<%d> [depth] code=[%s]  index=%d key=%d  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__,  
						m_code, index, key, data[key], "NXT 통합 매도잔량", index - 205 + 1, "호가");
				}
				else if ((index >= nms1size) && (index <= nms10size))
				{
					m_slog.Format(" [%s]<%d> [depth] code=[%s]  index=%d key=%d  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__, 
						m_code, index, key, data[key], "NXT 통합 매수잔량", index - 215 + 1, "호가");
				}
			
				Output_DebugString(m_slog);
			}
			//	continue;
		}




		if (item->m_data.CompareNoCase(text))
		{
			item->m_data = text;

			if (key == 515)	// 가중평균가
			{
				if (m_bSiseMemo)
				{
					for (int jj = 0; jj < m_sisememo.GetSize(); jj++)
					{
						std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(jj);
						if (pMemo->gubn == 1 && 
							atoi(pMemo->dat1) == 5 &&
							atoi(pMemo->dat2) == 2)
						{
							pMemo->val.Format("%d", (int)fabs(str2double(text)));
							addComma(pMemo->val);
						}
					}
				}
			}
			rect.UnionRect(rect, item->m_fRc);


			if ((index >= askPrice1 && index <= askPrice10) || (index >= bidPrice1 && index <= bidPrice10))
			{
				changePrice = true;
				if (text.GetLength() > 7) m_bBigDigit = TRUE;
			}
			else if ((index >= askSize1 && index <= askSize10) || (index >= bidSize1 && index <= bidSize10))
				changeSize = true;
			else if (index >= prePrice && index <= preVolume)
				changePre = true;
			else if ((index >= tkmid && index <= tnmidrate))
				changePrice = true;
		}

		//if (m_mkgubn == mkNXT)
		//{
		//	if (index >= 170)
		//	{
		//		switch (index)
		//		{
		//		case MIDNPRCMD:
		//		{
		//			m_slog.Format("%d %d %s", index, key, data);
		//			TRACE(m_slog);
		//		}
		//		break;
		//		case MIDNPRICE:
		//		{
		//			m_slog.Format("%d %d %s", index, key, data);
		//			TRACE(m_slog);
		//		}
		//		break;
		//		case MIDNPRCMS:
		//		{
		//		}
		//		break;
		//		case MIDNPRCDBMD:
		//		{
		//		}
		//		break;
		//		case MIDNPRCDBMS:
		//		{
		//		}
		//		break;
		//		case MIDNPRCRATE:
		//		{
		//		}
		//		break;
		//		}  //switch
		//	}
		//} //if
	}

	calculatePreValue();

	if (changePrice || changeCurr)
	{
		if (m_percent)
			rect.UnionRect(rect, getPercentRect());
		else
			rect.UnionRect(rect, m_prect);
	}

/*
	if ((changePrice || changeCurr) && m_percent)
		rect.UnionRect(rect, getPercentRect());

	if (changePrice && changeCurr)
		rect.UnionRect(rect, m_prect);
*/
	if (changeSize && (m_bar == grBar || m_bar == grBarText))
		getMaxSize();

	if ((m_type == ctCode || m_type == ctECN) && changePre)
	{
		item = m_items.GetAt(prePrice);
		if (!m_showPreValue && !item->m_data.IsEmpty())
		{
			m_showPreValue = true;
			setItemRect();
		}
		else
		{
			if (item->m_data.IsEmpty())
			{
				m_showPreValue = false;
				for (int ii = prePrice; ii < preVolume; ii++)
				{
					item = m_items.GetAt(ii);
					rect.UnionRect(rect, item->m_bRc);
					item->m_data = _T("");
				}
				setItemRect();
			}
		}
		calculatePreValue();
		for (int ii = prePrice; ii <= preVolume; ii++)
			rect.UnionRect(rect, m_items.GetAt(ii)->m_bRc);
	}

	if (m_pRgn && !rect.IsRectEmpty())
		InvalidateRgn(m_pRgn.get());
}

void Cdepth::WriteLog(CString Data)
{
	CString strLogFile;
	CFile pFile;
	
	strLogFile.Format("%s\\dev\\CX_DEPTHLOG_%s.LOG",Variant(homeCC,""),m_code);
	
	if (pFile.Open(strLogFile, CFile::modeReadWrite | CFile::shareDenyNone))
	{
		const	ULONGLONG	pos = pFile.SeekToEnd();
		pFile.Seek(pos, CFile::begin);
	}
	else
		pFile.Open(strLogFile, CFile::modeCreate|CFile::modeReadWrite | CFile::shareDenyNone);
	
	CString strDate, strTime;
	SYSTEMTIME	tm;
	GetLocalTime(&tm);
	strDate.Format("%04d%02d%02d", tm.wYear, tm.wMonth, tm.wDay);    
	strTime.Format("%02d%02d%02d", tm.wHour, tm.wMinute, tm.wSecond);    
	
	CString sWrite;
	sWrite.Format("%s%c%s%c%s", strDate,  '\t', strTime, '\t', Data);
	pFile.Write(sWrite, sWrite.GetLength() );
	pFile.Close();
}

void Cdepth::freeItems()
{
	std::shared_ptr<Citem> item = nullptr;
	for (int ii = 0; ii < m_items.GetSize(); ii++)
	{
		item = m_items.GetAt(ii);
		item->m_data.Empty();
		item.reset();
	}
	m_items.RemoveAll();
}

void Cdepth::freeSiseMemo()
{
	for (int ii = 0; ii < m_sisememo.GetSize(); ii++)
		m_sisememo.GetAt(ii).reset();
	m_sisememo.RemoveAll();
}

void Cdepth::clear()
{
	for ( int ii = 0; ii < N_DATA; ii++)
		m_items.GetAt(ii)->m_data.Empty();

	m_curr = 0;

	EW_Clear();
}

void Cdepth::draw(CDC* dc, const CRect* pClip)
{
	CRect	rect;
	CBitmap	bitmap, *pbitmap;
	CDC	memDC;

	GetClientRect(&rect);
	bitmap.CreateCompatibleBitmap(dc, rect.Width(), rect.Height());

	memDC.CreateCompatibleDC(dc);
	pbitmap = memDC.SelectObject(&bitmap);

	if(m_bIsNXTAble)
		drawView(&memDC);
	else
	{
		CRect	rect;
		const	int	top = m_showTop ? m_topH : 0;
		GetClientRect(rect);
		memDC.FillSolidRect(rect, m_clrWhite);
		if (m_pMBong)		m_pMBong->ShowWindow(SW_HIDE);
		if (m_pContract)	m_pContract->ShowWindow(SW_HIDE);
		CBitmap* pOldBitmap = nullptr, * pBitmap = nullptr;
		//pBitmap = CBitmap::FromHandle(m_pBitmapNoData);

		if (m_pBitmapNoData)
		{
			BITMAP stBitmapInfo;
			m_pBitmapNoData->GetBitmap(&stBitmapInfo);

			memDC.CreateCompatibleDC(dc);
			if (memDC.m_hDC != NULL)
				pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmapNoData);

			dc->StretchBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
			if (pOldBitmap)
				memDC.SelectObject(pOldBitmap);
		}
		else
		{
			UINT  style = 0;
			style = DT_SINGLELINE | DT_VCENTER| DT_CENTER;
			memDC.DrawText("해당 종목은 NXT 불가 종목이에요", rect, style);
		}
	}

	dc->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pbitmap);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void Cdepth::drawItems(CDC* dc)
{
	CString	string, stmp;
	UINT	style = 0;
	const	int	ndc = dc->SaveDC();
	std::shared_ptr<Citem> item = nullptr;
	CFont	*font = getFont(m_point, m_fonts, FW_BOLD);

	dc->SetBkMode(TRANSPARENT);
	//m_bGradient = false;
	if (m_bGradient)
	{
		CRect	rc, rcx;
		float	rStep = 0, gStep = 0, bStep = 0;
		int	rs = GetRValue(m_clrAskGs);
		int	gs = GetGValue(m_clrAskGs);
		int	bs = GetBValue(m_clrAskGs);
		int	re = GetRValue(m_clrAskGe);
		int	ge = GetGValue(m_clrAskGe);
		int	be = GetBValue(m_clrAskGe);

		std::shared_ptr<Citem> sItem = m_items.GetAt(m_idxMDS);
		std::shared_ptr<Citem> eItem = m_items.GetAt(m_idxMDE);
		rc.SetRect(sItem->m_bRc.left, eItem->m_bRc.top, eItem->m_bRc.right, sItem->m_bRc.bottom);

		rStep = float(rs - re)/float(rc.Height());
		gStep = float(gs - ge)/float(rc.Height());
		bStep = float(bs - be)/float(rc.Height());

		for (int ii = 0; ii < rc.Height(); ii++)
		{
			rcx.SetRect(rc.left, rc.top + ii, rc.right, rc.top + ii + 1);
			dc->FillSolidRect(&rcx, RGB(rs - rStep * ii, gs - gStep * ii, bs - bStep * ii));
		}

		sItem = m_items.GetAt(m_idxMSS);
		eItem = m_items.GetAt(m_idxMSE);
		rc.SetRect(sItem->m_bRc.left, sItem->m_bRc.top, eItem->m_bRc.right, eItem->m_bRc.bottom);

		rs = GetRValue(m_clrBidGs); gs = GetGValue(m_clrBidGs); bs = GetBValue(m_clrBidGs);
		re = GetRValue(m_clrBidGe); ge = GetGValue(m_clrBidGe);	be = GetBValue(m_clrBidGe);

		rStep = float(rs - re)/float(rc.Height());
		gStep = float(gs - ge)/float(rc.Height());
		bStep = float(bs - be)/float(rc.Height());

		rc.SetRect(sItem->m_bRc.left, sItem->m_bRc.top, eItem->m_bRc.right, eItem->m_bRc.bottom);

		for (int ii = 0; ii < rc.Height(); ii++)
		{
			rcx.SetRect(rc.left, rc.top + ii, rc.right, rc.top + ii + 1);
			dc->FillSolidRect(&rcx, RGB(rs - rStep * ii, gs - gStep * ii, bs - bStep * ii));
		}
		
	}


	//5단호가 안에 색 넣는 로직 시작
	//매도
	for (int idx = askPrice1; idx < askPrice6; idx++)
	//if ((ii >= askPrice1 && ii <= askPrice10) || (ii >= bidPrice1 && ii <= bidPrice10))
	{
		CRect	drc;
		COLORREF clr = 0;

		drc = m_items.GetAt(idx)->m_fRc;
		drc.left  = m_prect.left;
		drc.right = m_prect.right;				
		drc.top   = drc.top-2;
		if (!m_b1801)
		{
			switch (idx)
			{
			case askPrice1 :  clr = getColor(171);	break;
			case askPrice2 :  clr = getColor(111);	break;
			case askPrice3 :  clr = getColor(112);	break;
			case askPrice4 :  clr = getColor(113);	break;
			case askPrice5 :  clr = getColor(114);	break;
			default:	break;
			}
		}
		else
		{
			switch (idx)
			{
			case askPrice1 :  clr = 0x00FCE5C5;	break;
			case askPrice2 :  clr = 0x00FFEAD4;	break;
			case askPrice3 :  clr = 0x00FDEFDC;	break;
			case askPrice4 :  clr = 0x00FEF3E5;	break;
			case askPrice5 :  clr = 0x00FEF8ED;	break;
			default:	break;
			}
		}
		
		dc->FillSolidRect(drc, clr);
	}

	for (int idx2 = bidPrice1; idx2<bidPrice6; idx2++)
	//if ((ii >= askPrice1 && ii <= askPrice10) || (ii >= bidPrice1 && ii <= bidPrice10))
	{
		CRect	drc;
		COLORREF clr = 0;

		drc = m_items.GetAt(idx2)->m_fRc;
		drc.left  = m_prect.left;
		drc.right = m_prect.right;				
		drc.top   = drc.top-2;
		if (!m_b1801)
		{
			switch (idx2)
			{
			case bidPrice1 :  clr = getColor(170);	break;
			case bidPrice2 :  clr = getColor(121);	break;
			case bidPrice3 :  clr = getColor(122);	break;
			case bidPrice4 :  clr = getColor(123);	break;
			case bidPrice5 :  clr = getColor(124);	break;
			default:	break;
			}
		}
		else
		{
			switch (idx2)
			{
			case bidPrice1 :  clr = 0x00CAC4FC;	break;
			case bidPrice2 :  clr = 0x00D5D0FD;	break;
			case bidPrice3 :  clr = 0x00E0DDFD;	break;
			case bidPrice4 :  clr = 0x00ECE6FF;	break;
			case bidPrice5 :  clr = 0x00F1EDFF;	break;
			default:	break;
			}
		}
		dc->FillSolidRect(drc, clr);
	}
	//5단호가 안에 색 넣는 로직 끝

	if (m_iShowMIDPRC)
	{
		for (int idx3 = MIDKPRCMD; idx3 <= MIDKPRCMS; idx3++)
		{  //modi NXT  ref 중간가 rect 색
			CRect	drc;
			COLORREF clr = 0;

			drc = m_items.GetAt(idx3)->m_fRc;

			switch (idx3)
			{
			case MIDKPRCMD:
			{
				clr = RGB(255, 255, 255);
				drc.top = drc.top - 2;
			}
			break;
			case MIDKPRICE:
			{
				clr = CLR_MIDPRC;
				drc.left = m_prect.left;
				drc.right = m_prect.right;
				drc.top = drc.top - 2;
			}
			break;
			case MIDKPRCMS:
			{
				clr = RGB(255, 255, 255);
				drc.top = drc.top - 2;
			}
			break;
			default:
				break;
			}

			dc->FillSolidRect(drc, clr);
		}
	}

	//특수배경
	if (m_b1801) //매도쪽 배경
	{
		CRect	drc, drc2;

		drc  = m_items.GetAt(askBefore5)->m_fRc;
		drc2 = m_items.GetAt(askSize1)->m_fRc;
		drc.left -= 3;
		drc.bottom = drc2.bottom;
		drc.right = drc2.right;
		drc.right += 3;
		drc.top -= 1;
		dc->FillSolidRect(drc, 0x00FDF6F3);
	}
	if (m_b1801)	//매수쪽 배경
	{
		CRect	drc, drc2;

		drc  = m_items.GetAt(bidSize1)->m_fRc;
		drc2 = m_items.GetAt(bidBefore5)->m_fRc;
		drc.left -= 3;
		drc.bottom = drc2.bottom;
		drc.right = drc2.right;
		drc.right += 3;
		drc.top -= 1;
		dc->FillSolidRect(drc, 0x00F2F3FD);
	}

	m_rcCurr.SetRectEmpty();

#ifdef LP_AMOUNT
	for (int ii = askPrice1; ii <= tnmidrate; ii++)   //실제 드로잉을 위한 루프 1~224
	{
		if (ii >= gAMT && ii < askLp1)
			continue;

		//tsetcode
		if ((ii >= askTotalSize && ii <= bidTotalBefore))
		{
			CString sibal{};
		}
#else
	for (int ii = askPrice1; ii < gAMT; ii++)
	{
#endif
	
		//if (m_mkgubn == mkNXT)   ////0:예상체결 , 1:시간외, 2:현재가(K)
		//{ //NXT에서는 하단토클이 예상->시간외->현재가  3가지라서 다르게 동작하기 때문
		//	if ((m_iBottomType == 0) && (ii >= askSizeTime && ii <= bidSizeTimeBefore))  //예상일때  86~89 시간외는 안받는다
		//		continue;
		//	else if ((m_iBottomType == 1) && (ii >= prePrice && ii <= preVolume)) //시간외일때  98~99 예상은 안받는다
		//		continue;
		//}
		//else
		{
			if (m_bPredict && (ii >= askSizeTime && ii <= bidSizeTimeBefore))  //예상일때  86~89 시간외는 안받는다
				continue;
			else if (!m_bPredict && (ii >= prePrice && ii <= preVolume)) //시간외일때  98~99 예상은 안받는다
				continue;
		}
		if (!m_showBeforeDiff)
		{
			if ((ii >= askBefore1 && ii <= askBefore10) || (ii >= bidBefore1 && ii <= bidBefore10))
				continue;
			if ((ii >= askLp1 && ii <= askLp10) || (ii >= bidLp1 && ii <= bidLp10))
				continue;
		}

		dc->SelectObject(m_font);

		item = m_items.GetAt(ii);

		if (!m_bGradient)
			dc->FillSolidRect(item->m_bRc, item->m_pRGB);
		
		if ((ii >= askPrice1 && ii <= askPrice10) || (ii >= bidPrice1 && ii <= bidPrice10))
		{
			switch (m_type)
			{
			case ctCode:
			case ctECN:
				if (m_curr == fabs(str2double(item->m_data)) && m_curr != 0.0 && m_bCurr)
					m_rcCurr.SetRect(m_columns[0]+1, item->m_bRc.top-1, m_columns[1]+1, item->m_bRc.bottom);
				break;
			case ctFuture:
			case ctOption:
				if (m_curr != 0 && m_curr == fabs(str2double(item->m_data)) && m_bCurr)
					m_rcCurr.SetRect(m_columns[0]+1, item->m_bRc.top-1, m_columns[1]+1, item->m_bRc.bottom);
				break;
			default:break;
			}
		}

		if ((ii >= askSize1 && ii <= askSize10) || (ii >= bidSize1 && ii <= bidSize10)
				|| ii == askTotalSize || ii == bidTotalSize)
		{
			string = item->m_data;
			string.Trim();
			if ( string.GetLength() > 9 ) string.Remove(',');
			item->m_data = string;
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
			if (m_focus == ii)
			{
				CRect	drc;

				drc = m_items.GetAt(ii)->m_fRc;
				drc.right = m_prect.right-1;				
				drc.OffsetRect(0, -1);
				dc->SelectObject(getPen(RGB(210,210,210),1));
				dc->SelectObject(getBrush(RGB(255, 255, 205)));
				
				dc->Rectangle(drc);
			}
			if (m_bBoldPrice)
				dc->SelectObject(font);
			if (atof(m_items.GetAt(ii+20)->m_data) > 0)
			{
				
				string = item->m_data;

				//호가 캡쳐
				if (m_config.shl == 1)
				switch (hogaTypeToInt(string))
				{
				case openPrice:
					dc->FillSolidRect(&CRect(item->m_fRc.right+1,item->m_fRc.top-1,item->m_fRc.right+6,item->m_fRc.bottom-1), CLR_OPENPRICE);
					//dc->RoundRect(item->m_fRc.right+1,item->m_fRc.top-1,item->m_fRc.right+6,item->m_fRc.bottom-1, 2,2); brush.DeleteObject();
					break;
				case highPrice:
					dc->FillSolidRect(&CRect(item->m_fRc.right+1,item->m_fRc.top-1,item->m_fRc.right+6,item->m_fRc.bottom-1), CLR_HIGHPRICE);
					break;
				case lowPrice:
					dc->FillSolidRect(&CRect(item->m_fRc.right+1,item->m_fRc.top-1,item->m_fRc.right+6,item->m_fRc.bottom-1), CLR_LOWPRICE);
					break;
				case pivotPrice:
					dc->FillSolidRect(&CRect(item->m_fRc.right+1,item->m_fRc.top-1,item->m_fRc.right+6,item->m_fRc.bottom-1), CLR_PIVOTPRICE);
					break;
				case SVIUP:
				case SVIDN:
					dc->FillSolidRect(&CRect(item->m_fRc.right+1,item->m_fRc.top-1,item->m_fRc.right+6,item->m_fRc.bottom-1), CLR_VI);
					break;

				default:
					//dc->SetBkMode(TRANSPARENT);
					break;
				}	

				if (!m_bAble && m_config.hdyul == 1)
				{
					if (string.GetLength() > 7)
						string.Remove(',');
				}

				if((ii >= askPrice1 && ii <= askPrice10))
					m_slog.Format("------------------[%s]<%d> [depth] code=[%s] data=[%s] [%s%d%s]", __FUNCTION__, __LINE__, m_code, string, "매도", ii, "호가");   //매도1~10  실제 드로잉
				else
					m_slog.Format("------------------[%s]<%d> [depth] code=[%s]  data=[%s] [%s%d%s]", __FUNCTION__, __LINE__, m_code, string, "매수", ii - 40, "호가"); //매수1~10  실제 드로잉
				Output_DebugString(m_slog);

				dc->DrawText(string, item->m_fRc, style);
			}
		}
		else if ((ii >= askPercent1 && ii <= askPercent10) || (ii >= bidPercent1 && ii <= bidPercent10))
		{
			if (false && m_bBoldPrice)
				dc->SelectObject(font);

			if (atof(m_items.GetAt(ii+10)->m_data) > 0)
				dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else if ((ii >= askSize1 && ii <= askSize10) || (ii >= bidSize1 && ii <= bidSize10) )
		{
			if (m_bBoldRemain)
				dc->SelectObject(font);
			if (m_mkgubn != mkTOT || m_iDataType != DF_SEP)   //통합거래소 아니거나 각거래소 잔량보여주기가 아닌경우
				dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else if (!IsELW(m_code) && ((ii >= askBefore1 && ii <= askBefore10) || (ii >= bidBefore1 && ii <= bidBefore10)) )
		{
			if (m_bBoldCnt)
				dc->SelectObject(font);
			if (m_mkgubn != mkTOT || m_iDataType != DF_SEP)
				dc->DrawText(item->m_data, item->m_fRc, style);
		
		}
		else if (IsELW(m_code) && ((ii >= askLp1 && ii <= askLp10) || (ii >= bidLp1 && ii <= bidLp10)))
		{
			if (m_bBoldCnt)
				dc->SelectObject(font);
			dc->DrawText(item->m_data, item->m_fRc, style);
		}
		else if ((ii >= MIDKPRCMD) && (ii <= MIDKPRCRATE) && m_iShowMIDPRC && (m_mkgubn == mkKRX))
		{
			if (1)
			{
				//drawitem 중간가영역에 실제 값넣어주는 곳
				item = m_items.GetAt(ii);
				string = item->m_data;

				CRect	drc;

				/*if (m_focus == ii)
				{
					drc = m_items.GetAt(MIDKPRICE)->m_fRc;
					drc.right = m_prect.right - 1;
					drc.OffsetRect(0, -1);

					if (m_config.hdyul == 0)
						drc.left = m_columns[0] + 2;

					dc->SelectObject(getBrush(CLR_IVS_MIDPRC));
					dc->Rectangle(drc);
					InvalidateRect(m_items.GetAt(MIDKPRCMD)->m_fRc);
				}
				else*/
				{
					drc = m_items.GetAt(MIDKPRICE)->m_fRc;
					drc.right = m_prect.right - 1;
					drc.OffsetRect(0, -1);
					dc->SelectObject(getBrush(CLR_MIDPRC));
				}

				style = DT_SINGLELINE | DT_VCENTER | DT_RIGHT;

				if (m_bBoldPrice && ii == MIDKPRICE)
					dc->SelectObject(font);

				if (ii == MIDKPRICE)
				{
					//if (!m_bAble && m_config.hdyul == 1)
					{
						if (string.GetLength() > 7)
							string.Remove(',');
					}

					if (string.GetLength() >= 7)
					{
						CFont* poldfont = dc->SelectObject(m_sfont);
						dc->DrawText(string, item->m_fRc, style);
						dc->SelectObject(poldfont);
					}
					else
						dc->DrawText(string, item->m_fRc, style);

					m_slog.Format("------------------[%s]<%d> [depth] KRX중간가 code=[%s]  data=[%s]", __FUNCTION__, __LINE__, m_code, string); //KRX 중간가 드로잉
					Output_DebugString(m_slog);

				}
				else
					dc->DrawText(string, item->m_fRc, style);

				if (ii == MIDKPRCRATE)
				{
					dc->DrawText(string, m_rcMIDPercent, style);
				}
				else if (ii == MIDKPRICE)
				{
					if (string != "0" && string.GetLength() > 0)
						dc->FillSolidRect(&CRect(item->m_fRc.right + 1, item->m_fRc.top, item->m_fRc.right + 6, item->m_fRc.bottom - 1), CLR_KMIDPRICE);
				}
			}
		}
		else if ((ii >= MIDNPRCMD) && (ii <= MIDNPRCRATE) && m_iShowMIDPRC && (m_mkgubn == mkNXT))  //NXT 중간가
		{
			if (1)
			{
				item = m_items.GetAt(ii);
				string = item->m_data;

				CRect	drc;

				/*if (m_focus == ii)
				{
					drc = m_items.GetAt(MIDNPRICE)->m_fRc;
					drc.right = m_prect.right - 1;
					drc.OffsetRect(0, -1);

					if (m_config.hdyul == 0)
						drc.left = m_columns[0] + 2;

					dc->SelectObject(getBrush(CLR_IVS_MIDPRC));
					dc->Rectangle(drc);
					InvalidateRect(m_items.GetAt(MIDNPRCMD)->m_fRc);
				}
				else*/
				{
					drc = m_items.GetAt(MIDNPRICE)->m_fRc;
					drc.right = m_prect.right - 1;
					drc.OffsetRect(0, -1);
					//dc->SelectObject(getPen(RGB(210, 210, 210), 1));
					dc->SelectObject(getBrush(CLR_MIDPRC));
					//dc->Rectangle(drc);
				}

				style = DT_SINGLELINE | DT_VCENTER | DT_RIGHT;

				if (m_bBoldPrice && ii == MIDNPRICE)
					dc->SelectObject(font);

				if (ii == MIDNPRICE)
				{
					//if (!m_bAble && m_config.hdyul == 1)
					{
						if (string.GetLength() > 7)
							string.Remove(',');
					}

					if (string.GetLength() >= 7)
					{
						CFont* poldfont = dc->SelectObject(m_sfont);
						dc->DrawText(string, item->m_fRc, style);
						dc->SelectObject(poldfont);
					}
					else
						dc->DrawText(string, item->m_fRc, style);

					m_slog.Format("------------------[%s]<%d> [depth] NXT중간가 code=[%s]  data=[%s]", __FUNCTION__, __LINE__, m_code, string);  //NXT 중간가 드로잉
					Output_DebugString(m_slog);

				}
				else
					dc->DrawText(string, item->m_fRc, style);

				if (ii == MIDNPRCRATE)
				{
					dc->DrawText(string, m_rcNMIDPercent, style);
				}
				else if (ii == MIDNPRICE)
				{
					if (string != "0" && string.GetLength() > 0)
						dc->FillSolidRect(&CRect(item->m_fRc.right + 1, item->m_fRc.top, item->m_fRc.right + 6, item->m_fRc.bottom - 1), CLR_NMIDPRICE);
				}
			}
		}
		else if (ii >= cticktime && ii <= dbongvol)
		{
		}
		else if (ii >= kmd1size && ii <= nms10size && (m_mkgubn == mkTOT))  //185 ~ 224 통합호가에서 NXT, KRX 매도매수 잔량 그리기
		{
			if (m_bBoldRemain)
				dc->SelectObject(font);
			if (m_mkgubn == mkTOT && m_iDataType == DF_SEP)
			{
				dc->DrawText(item->m_data, item->m_fRc, style);
			}
		}
		else if (ii >= tkdtjan && ii <= tnstjan && (m_mkgubn == mkTOT) && m_iDataType == DF_SEP)  //225 ~ 228 통합호가  개별보기에서 NXT, KRX 매도매수 잔량 그리기
		{
			bool bchange{};
			UINT tstyle = style;
			stmp = item->m_data;
			
			CFont* poldfont{};
			if (stmp.GetLength() >= 9)
			{
				bchange = true;
				stmp.Replace(",", "");
				poldfont = dc->SelectObject(m_sfont);
				style = DT_SINGLELINE | DT_VCENTER| DT_RIGHT;
			}

			switch (ii)
			{
				case tkdtjan:  //통합 KRX 매도총잔량  <--  매도총잔량대비 위치
				{
					//dc->DrawText(item->m_data, m_items.GetAt(askTotalSize)->m_fRc, style);
					dc->DrawText(stmp, m_items.GetAt(askTotalSize)->m_fRc, style);
				}
				break;
				case tkstjan:   //통합 KRX 매수총잔량  <--  매수총잔량대비 위치
				{
					//dc->DrawText(item->m_data, m_items.GetAt(bidTotalSize)->m_fRc, style);
					dc->DrawText(stmp, m_items.GetAt(bidTotalSize)->m_fRc, style);
				}
				break;
				case tndtjan:   //통합 NXT 매도총잔량  <--  매도총잔량 위치
				{
				//	dc->DrawText(item->m_data, m_items.GetAt(askTotalBefore)->m_fRc, style);
					dc->DrawText(stmp, m_items.GetAt(askTotalBefore)->m_fRc, style);
				}
				break; 
				case tnstjan:   //통합 NXT 매수총잔량  <--  매수총잔량 위치
				{
					//dc->DrawText(item->m_data, m_items.GetAt(bidTotalBefore)->m_fRc, style);
					dc->DrawText(stmp, m_items.GetAt(bidTotalBefore)->m_fRc, style);
				}
				break;
			}
			if (bchange)
			{
				dc->SelectObject(poldfont);
				style = tstyle;
			}
		}
		//else if (ii >= tkmid && ii <= tnmidrate && (m_mkgubn == mkTOT))   //229 ~ 240  통합에서 중간가 그리기
		else if (ii >= tkmid && ii <= tnmidrate && (m_mkgubn == mkTOT) && (m_iShowMIDPRC > 0))   //229 ~ 240  통합에서 중간가 그리기
		{
			if (1)
			{
				if (m_mkgubn == mkTOT)
				{
					//dc->DrawText(item->m_data, item->m_fRc, style);
					item = m_items.GetAt(ii);
					string = item->m_data;

					style = DT_SINGLELINE | DT_VCENTER | DT_RIGHT;

					if (m_bBoldPrice && (ii == tkmid || ii == tnmid))
						dc->SelectObject(font);

					if (ii == tkmid || ii == tnmid)
					{
						if (string.GetLength() > 7)
							string.Remove(',');

						if (string.GetLength() >= 7)
						{
							CFont* poldfont = dc->SelectObject(m_sfont);
							dc->DrawText(string, item->m_fRc, style);
							dc->SelectObject(poldfont);
						}
						else
							dc->DrawText(string, item->m_fRc, style);

						m_slog.Format("------------------[%s]<%d> [depth] 통합중간가 code=[%s]  data=[%s]", __FUNCTION__, __LINE__, m_code, string); //통합 중간가 드로잉
						Output_DebugString(m_slog);

					}
					else
						dc->DrawText(string, item->m_fRc, style);

					if (ii == tkmidrate)
					{
						dc->DrawText(string, m_rcMIDPercent, style);   //xhdgk
					}
					else if (ii == tnmidrate)
					{
						dc->DrawText(string, m_rcNMIDPercent, style);
					}
				}
			}  //if(0)

		}
		else
		{
#ifdef LP_AMOUNT
			if (IsELW(m_code))
			{
				if ((ii >= askBefore1 && ii <= askBefore10) || (ii >= bidBefore1 && ii <= bidBefore10) ||
					ii == askTotalBefore || ii == bidTotalBefore)
					continue;
			}
			else
			{
				if ((ii >= askLp1 && ii <= askLp10) || (ii >= bidLp1 && ii <= bidLp10) ||
					ii == askTotalLp || ii == bidTotalLp)
					continue;
			}
#endif
			if (ii == preVolume || ii == bidSizeTime)//예상체결량
			{
				CFont* poldfont{};
				if (item->m_data.GetLength() >= 9)
					poldfont = dc->SelectObject(m_sfont);

				dc->DrawText(item->m_data, item->m_fRc, style);

				if (item->m_data.GetLength() >= 9)
					dc->SelectObject(poldfont);
			}
			else
			{
				if (m_mkgubn == mkTOT && (ii >= MIDKPRCMD && ii <= MIDNPRCRATE))
				{
					CString  ssss;
				}
				else if(m_mkgubn == mkKRX && (ii >= MIDNPRCMD))  //KRX는 NXT의 중간가 처리를 안한다.
				{

				}
				else if (m_mkgubn == mkNXT && (ii >= MIDKPRCMD && ii <= MIDKPRCRATE))  //NXT는 KRX 중간가 처리를 안한다.
				{

				}
				else if (m_mkgubn == mkNXT && (ii >= tnmid && ii <= tnmidrate))  //NXT는
				{

				}
				else if (m_mkgubn == mkTOT && m_iDataType == DF_SEP && (ii >= askTotalSize && ii <=  bidTotalBefore))  
				{//통합에서 개별일때 기존  매도총잔량(askTotalSize(82)) ~ 매수총잔량대비(bidTotalBefore)(85))  영역은 안그린다

				}
				else if (m_mkgubn == mkTOT && m_iDataType == DF_TOT && (ii >= askTotalSize && ii <= bidTotalBefore))
				{//통합에서  통합일때 기존  매도총잔량(askTotalSize(82)) ~ 매수총잔량대비(bidTotalBefore)(85))  영역은 그린다!!!
					dc->DrawText(item->m_data, item->m_fRc, style);
				}
				else
				{
					if (item->m_data == "20300")
						CString str;
					dc->DrawText(item->m_data, item->m_fRc, style);
				}
			}
		}
	}

	// 중간가 K,  N 이니셜 그리는부분 //testcode
	CRect rc;
	rc = m_items.GetAt(MIDKPRICE)->m_fRc;
	rc.left = m_columns[0] ;
	rc.right = m_columns[0] + 24;
	rc.OffsetRect(1, 0);
	style = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
	dc->SelectObject(m_sBfont);
	dc->SetTextColor(RGB(0, 0, 0));

	if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
	{
		dc->SetTextColor(CLR_KMIDPRCLEFT);
		dc->DrawText("K", rc, style);
	}
	else if (m_mkgubn == mkNXT)
	{
		dc->SetTextColor(CLR_NMIDPRCLEFT);
		dc->DrawText("N", rc, style);
	}

	if (m_mkgubn == mkTOT)
	{
		rc = m_items.GetAt(MIDNPRICE)->m_fRc;
		rc.left = m_columns[0] ;
		rc.right = m_columns[0] + 24;
		rc.OffsetRect(1, 0);
		style = DT_SINGLELINE | DT_VCENTER | DT_LEFT;
		//dc->SelectObject(m_sfont);
		dc->SetTextColor(CLR_NMIDPRCLEFT);
		dc->DrawText("N", rc, style);
	}

	dc->RestoreDC(ndc);
}

void Cdepth::drawBong(CDC* dc)
{
	CString tmpS;
	std::shared_ptr<Citem> pItem = nullptr;
	const	double	jjga = m_curr;
	double	siga = 0, koga = 0, jega = 0, shga = 0, hhga = 0, pivot = 0;

	pItem = m_items.GetAt(openPrice);	siga = str2double(pItem->m_data);
	pItem = m_items.GetAt(highPrice);	koga = str2double(pItem->m_data);
	pItem = m_items.GetAt(lowPrice);	jega = str2double(pItem->m_data);
	pItem = m_items.GetAt(shPrice);		shga = str2double(pItem->m_data);	
	pItem = m_items.GetAt(hhPrice);		hhga = str2double(pItem->m_data);
	pItem = m_items.GetAt(pivotPrice);	pivot = str2double(pItem->m_data);

	// elw 
	if (!shga && !hhga)
	{
		drawBongX(dc); return;
	}

	if (siga == 0 || koga == 0 || jega == 0 || jjga == 0 || shga == 0 || hhga == 0 || pivot == 0)
		return;

	const	int	ndc = dc->SaveDC();
	dc->SetBkMode(TRANSPARENT);
	CPen	*pen = nullptr, *pPen = nullptr;

	if (siga <= jjga)
		pen = getPen(m_clrUp, 1);
	else	pen = getPen(m_clrDown, 1);

	pPen = dc->SelectObject(pen);

	pItem = m_items.GetAt(askBong);

	const	int	sigaY = (int)((pItem->m_bRc.Height() * (shga - siga)) / (shga - hhga));
	const	int	kogaY = (int)((pItem->m_bRc.Height() * (shga - koga)) / (shga - hhga));
	const	int	jegaY = (int)((pItem->m_bRc.Height() * (shga - jega)) / (shga - hhga));
	int	jjgaY = (int)((pItem->m_bRc.Height() * (shga - jjga)) / (shga - hhga));
	const	int	pivotY = (int)((pItem->m_bRc.Height()* (shga - pivot))/ (shga - hhga));

	CRect	bongR;
	/*
	if (siga <= jjga)
		bongR.SetRect(pItem->m_bRc.left + 1, pItem->m_bRc.top + sigaY -1, pItem->m_bRc.right - 1, pItem->m_bRc.top + jjgaY);
	else	bongR.SetRect(pItem->m_bRc.left + 1, pItem->m_bRc.top + jjgaY, pItem->m_bRc.right - 1, pItem->m_bRc.top + sigaY+1);
	*/
	if (siga <= jjga)
	{
		if (sigaY == jjgaY) jjgaY++;
		bongR.SetRect(pItem->m_bRc.left + 1, pItem->m_bRc.top + sigaY, pItem->m_bRc.right - 1, pItem->m_bRc.top + jjgaY);
		dc->FillSolidRect(bongR, siga <= jjga ? m_clrUp : m_clrDown);
		//CString tmp; tmp.Format("%d,%d", sigaY, kogaY);
		//dc->TextOut(11,11,tmp);
		dc->MoveTo(pItem->m_bRc.CenterPoint().x, pItem->m_bRc.top + kogaY);
		dc->LineTo(pItem->m_bRc.CenterPoint().x, pItem->m_bRc.top + jegaY);	
		
	}
	else
	{
		if (sigaY == jjgaY) jjgaY++;
		bongR.SetRect(pItem->m_bRc.left + 1, pItem->m_bRc.top + jjgaY, pItem->m_bRc.right - 1, pItem->m_bRc.top + sigaY);
		dc->FillSolidRect(bongR, siga <= jjga ? m_clrUp : m_clrDown);
		dc->MoveTo(pItem->m_bRc.CenterPoint().x, pItem->m_bRc.top + kogaY);
		dc->LineTo(pItem->m_bRc.CenterPoint().x, pItem->m_bRc.top + jegaY);
	}
	

	/*
	// pivot line 표시
	pen = getPen(RGB(100, 100, 100), 1);
	dc->SelectObject(pen);
	dc->MoveTo(pItem->m_bRc.left  + 1, pItem->m_bRc.top + pivotY-1);
	dc->LineTo(pItem->m_bRc.right - 1, pItem->m_bRc.top + pivotY-1);
	*/
	dc->SelectObject(pPen);
	dc->RestoreDC(ndc);
}

void Cdepth::drawBongX(CDC* dc)
{
	double	siga = 0, koga = 0, jega = 0;
	const	double	jjga = m_curr;
	std::shared_ptr<Citem> pItem = nullptr;

	pItem = m_items.GetAt(openPrice);	siga = str2double(pItem->m_data);
	pItem = m_items.GetAt(highPrice);	koga = str2double(pItem->m_data);
	pItem = m_items.GetAt(lowPrice);	jega = str2double(pItem->m_data);

	if (!siga || !koga || !jega || !jjga)
		return;

	pItem = m_items.GetAt(askBong);
	CRect	crc = pItem->m_bRc;
	const	CPoint	cpt = crc.CenterPoint();

	crc.top    = cpt.y - (pItem->m_bRc.Height() / 7);
	crc.bottom = cpt.y + (pItem->m_bRc.Height() / 7);

	const	int	ndc = dc->SaveDC();

	dc->SetBkMode(TRANSPARENT);
	CPen	*pen = getPen(m_clrLine, 1);
	CPen	*pPen = dc->SelectObject(pen);

	dc->MoveTo(CPoint(crc.left,  crc.top));
	dc->LineTo(CPoint(crc.right, crc.top));

	dc->MoveTo(CPoint(crc.left,  crc.bottom));
	dc->LineTo(CPoint(crc.right, crc.bottom));

	dc->SelectObject(pPen);

	if (siga < jjga)
		pen = getPen(m_clrUp, 1);
	else
		pen = getPen(m_clrDown, 1);

	crc.DeflateRect(1, 1, 1, 1);

	const	double	per = (koga - jega) / crc.Height();
	CBrush	brush;
	COLORREF color = 0;
	int	up   = crc.top;
	int	down = crc.bottom;

	if (siga <= jjga)
	{
		color = m_clrUp;

		for (int ii = 0; ; ii++)
		{
			if (koga - per*ii > jjga)
			{
				up++; continue;
			}
			break;
		}
		for (int ii = 0; ; ii++)
		{
			if (jega + per*ii < siga)
			{
				down--; continue;
			}
			break;
		}
	}
	else
	{
		color = m_clrDown;

		for (int ii = 0; ; ii++)
		{
			if (koga - per*ii > siga)
			{
				up++; continue;
			}
			break;
		}
		for (int ii = 0; ; ii++)
		{
			if (jega + per*ii < jjga)
			{
				down--; continue;
			}
			break;
		}
	}

	pen = getPen(color, 1);
	pPen = dc->SelectObject(pen);

	if (koga == jega)
	{
		dc->MoveTo(crc.left,  cpt.y);
		dc->LineTo(crc.right, cpt.y);

		dc->SelectObject(pPen);
		return;
	}

	dc->MoveTo(cpt.x, crc.top);
	dc->LineTo(cpt.x, crc.bottom);

	for (int ii = min(up, down); ii <= max(up, down); ii++)
	{
		dc->MoveTo(crc.left,  ii);
		dc->LineTo(crc.right, ii);
	}
	dc->SelectObject(pPen);
	dc->RestoreDC(ndc);
}

void Cdepth::drawSiseMemo(CDC* dc)
{
	CString tmpS;
	const	int	ndc = dc->SaveDC();
	std::shared_ptr<Citem> pItem = nullptr;

	dc->SetBkMode(TRANSPARENT);
	for (int ii = askMemo1; ii <= bidMemo10; ii++)
	{
		pItem = m_items.GetAt(ii);
		pItem->m_data.Empty();
	}
	
	for (int ii = 0; ii < m_sisememo.GetSize(); ii++)
	{
		std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(ii);
		double	val = 0;

		pMemo->index = -1;
		if (pMemo->gubn == 4)
			val = str2double(pMemo->val) / str2double(pMemo->dat1);
		else
			val = str2double(pMemo->val);
		if (val == 0)
			continue;

		double	hoga = 0, min = 2147483647, gap = 0, minHoga = 2147483647, maxHoga = -2147483646;
		int	pos = -1;

		for (int jj = askPrice10; jj >= askPrice1; jj--)
		{
			pItem = m_items.GetAt(jj);
			hoga = str2double(pItem->m_data);

			if (hoga <= 0)
				continue;
			if (hoga > maxHoga)
				maxHoga = hoga;

			gap = fabs(val - hoga);
			if (gap < min)
			{
				min = gap;
				pos = jj - askPrice1;
			}
		}
		if (val > maxHoga && maxHoga != 0)
			pos = -1;

		for (int jj = bidPrice1; jj <= bidPrice10; jj++)
		{
			pItem = m_items.GetAt(jj);
			hoga = str2double(pItem->m_data);

			if (hoga <= 0)
				continue;
			if (hoga < minHoga)
				minHoga = hoga;
			
			gap = fabs(val - hoga);
			if (gap < min)
			{
				min = gap;
				pos = jj - bidPrice1 + 10;
			}
		}
		if (val < minHoga && minHoga != 0)
			pos = -1;
		
		if (pos != -1)
		{
			pItem = m_items.GetAt(askMemo1 + pos);
			pItem->m_data.Format("%d", atoi(pItem->m_data) + 1);
			pItem->m_pRGB = pMemo->color;
			pMemo->index = askMemo1 + pos;
		}
	}

	for (int ii = askMemo1; ii <= bidMemo10; ii++)
	{
		pItem = m_items.GetAt(ii);
		if (!pItem->m_data.IsEmpty())
		{
			CRect	cRc;
			CPen	*pen = getPen(m_clrWhite);
			CPen	*ppen = dc->SelectObject(pen);
			
			cRc = pItem->m_bRc;
			cRc.DeflateRect(0, 2);
			dc->FillSolidRect(&cRc, pItem->m_pRGB);
			if (atoi(pItem->m_data) > 1)
			{
				dc->MoveTo(pItem->m_bRc.left,  pItem->m_bRc.top + pItem->m_bRc.Height() / 2);
				dc->LineTo(pItem->m_bRc.right, pItem->m_bRc.top + pItem->m_bRc.Height() / 2);
				dc->MoveTo(pItem->m_bRc.left + pItem->m_bRc.Width() / 2, pItem->m_bRc.top);
				dc->LineTo(pItem->m_bRc.left + pItem->m_bRc.Width() / 2, pItem->m_bRc.bottom);
			}
			
			dc->SelectObject(ppen);
		}
	}
	
	dc->RestoreDC(ndc);
}

void Cdepth::drawInfo(CDC* dc)
{
	CRect	rect;
	CString tmpS;
	const	int	ndc = dc->SaveDC();
	const	CFont	*bFont = getFont(m_point, "굴림체", FW_BOLD, false);

	if (m_showOHLC == infoSiga)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		if (m_depth == 10)
		{
			const	int	cnt = 10;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("   시  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
					tmpS = m_items.GetAt(openPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("   고  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
					tmpS = m_items.GetAt(highPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("   저  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
					tmpS = m_items.GetAt(lowPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 3:
					dc->DrawText(_T("   상  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);   //10호가
					dc->SetTextColor(m_items.GetAt(shPrice)->m_fRGB);
					tmpS = m_items.GetAt(shPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T("   하  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(hhPrice)->m_fRGB);
					tmpS = m_items.GetAt(hhPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 5:
					dc->DrawText(_T(" 52고  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					tmpS = m_items.GetAt(max52)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 6:
					dc->DrawText(_T(" 52저  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					tmpS = m_items.GetAt(min52)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					
					break;
				case 7:
					dc->DrawText(_T(" 가중   "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(gajungPrice)->m_fRGB);
					tmpS = m_items.GetAt(gajungPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT); 
					break;
				case 8:
					dc->DrawText(_T(" 기준   "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotPrice)->m_fRGB);
					tmpS = m_items.GetAt(pivotPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 9:
						dc->DrawText(_T(" 체강  "), rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);   //10호가
						dc->SetTextColor(m_items.GetAt(chegang)->m_fRGB);
						tmpS = m_items.GetAt(chegang)->m_data;
						if (!m_bAble && m_config.hdyul == 1)
							if (tmpS.GetLength() > 7) tmpS.Remove(',');
						dc->DrawText(tmpS, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 5)
		{
			const	int	cnt = 6;
			int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);  
			offset += 2;
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_OneMoreRowH * ii); rect.bottom = offset + (int)(m_OneMoreRowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("   시  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
					tmpS = m_items.GetAt(openPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("   고  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
					tmpS = m_items.GetAt(highPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("   저  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
					tmpS = m_items.GetAt(lowPrice)->m_data;
					if (!m_bAble)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 3:
					{
						dc->DrawText(_T("   상  "), rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);   //5호가
						dc->SetTextColor(m_items.GetAt(shPrice)->m_fRGB);
						tmpS = m_items.GetAt(shPrice)->m_data;
						if (!m_bAble && m_config.hdyul == 1)
							if (tmpS.GetLength() > 7) tmpS.Remove(',');
						dc->DrawText(tmpS, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
					}
					break;
				case 4:
					{
						dc->DrawText(_T("   하  "), rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
						dc->SetTextColor(m_items.GetAt(hhPrice)->m_fRGB);
						tmpS = m_items.GetAt(hhPrice)->m_data;
						if (!m_bAble && m_config.hdyul == 1)
							if (tmpS.GetLength() > 7) tmpS.Remove(',');
						dc->DrawText(tmpS, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
						break;
					}
					break;
				case 5:	
						dc->DrawText(_T("  체강  "), rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
						dc->SetTextColor(m_items.GetAt(chegang)->m_fRGB);
						tmpS = m_items.GetAt(chegang)->m_data;
						if (!m_bAble && m_config.hdyul == 1)
							if (tmpS.GetLength() > 7) tmpS.Remove(',');
						dc->DrawText(tmpS, rect, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 3)
		{
			const	int	cnt = 3;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("   시  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
					dc->DrawText(m_items.GetAt(openPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("   고  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
					dc->DrawText(m_items.GetAt(highPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("   저  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
					dc->DrawText(m_items.GetAt(lowPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
	}
	else if (m_showOHLC == infoPivot)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		if (m_depth == 10)
		{
			const	int	cnt = 9;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->FillSolidRect(rect, getColor(3));
					dc->DrawText(_T("  피봇  "), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
					break;
				case 1:
					dc->DrawText(_T(" 2저항  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(resist2)->m_fRGB);
					tmpS = m_items.GetAt(resist2)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T(" 1저항  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(resist1)->m_fRGB);
					tmpS = m_items.GetAt(resist1)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 3:
					dc->DrawText(_T("  피봇  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotV)->m_fRGB);
					tmpS = m_items.GetAt(pivotV)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T(" 1지지  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(support1)->m_fRGB);
					tmpS = m_items.GetAt(support1)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 5:
					dc->DrawText(_T(" 2지지  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(support2)->m_fRGB);
					tmpS = m_items.GetAt(support2)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 6:
					dc->FillSolidRect(rect, getColor(3));
					dc->DrawText(_T("디 마 크"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
					break;
				case 7:
					dc->DrawText(_T(" D저항  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(resistD)->m_fRGB);
					tmpS = m_items.GetAt(resistD)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 8:
					dc->DrawText(_T(" D지지  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(supportD)->m_fRGB);
					tmpS = m_items.GetAt(supportD)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 5)
		{
			const	int	cnt = 5;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T(" 2저항  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
					tmpS = m_items.GetAt(resist2)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T(" 1저항  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(resist1)->m_fRGB);
					tmpS = m_items.GetAt(resist1)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->FillSolidRect(rect, getColor(3));
					dc->DrawText(_T("  피봇  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotPrice)->m_fRGB);
					tmpS = m_items.GetAt(pivotPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 3:
					dc->DrawText(_T(" 1지지  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(support1)->m_fRGB);
					tmpS = m_items.GetAt(support1)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T(" 2지지  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(support2)->m_fRGB);
					tmpS = m_items.GetAt(support2)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 3)
		{
			const	int	cnt = 3;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T(" 1저항  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(resist1)->m_fRGB);
					dc->DrawText(m_items.GetAt(resist1)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->FillSolidRect(rect, getColor(3));
					dc->DrawText(_T("  피봇  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotPrice)->m_fRGB);
					dc->DrawText(m_items.GetAt(pivotPrice)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T(" 1지지  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(support1)->m_fRGB);
					dc->DrawText(m_items.GetAt(support1)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
	}
	else if (m_showOHLC == infoCurr)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		if (m_depth == 10)
		{
			const	int	cnt = 10;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg); dc->SelectObject(m_font);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("   시  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
					tmpS = m_items.GetAt(openPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("   고  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
					tmpS = m_items.GetAt(highPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("   저  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
					tmpS = m_items.GetAt(lowPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 3:
					dc->DrawText(_T("   상  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);   //10호가
					dc->SetTextColor(m_items.GetAt(shPrice)->m_fRGB);
					tmpS = m_items.GetAt(shPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T("   하  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(hhPrice)->m_fRGB);
					tmpS = m_items.GetAt(hhPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 5:
					dc->DrawText(_T("  기준  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotPrice)->m_fRGB);
					tmpS = m_items.GetAt(pivotPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 6:
					dc->DrawText(_T("  현재  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = _T("");
					if (m_curr > 0.)
					{ tmpS.Format("%.f", m_curr); addComma(tmpS); }
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);

					break;
				case 7:
					dc->DrawText(_T("  대비  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(preBefore)->m_fRGB);
					tmpS = m_items.GetAt(preBefore)->m_data;
					dc->DrawText(m_items.GetAt(preBefore)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					tmpS = m_items.GetAt(preBefore)->m_data;
					break;
				case 8:
					dc->DrawText(_T("  등락  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = m_items.GetAt(prePercent)->m_data;

					if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
						tmpS.Delete(0);

					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 9:
					dc->DrawText(_T("  거래  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(gVolume)->m_fRGB);
					tmpS = m_items.GetAt(gVolume)->m_data;
					if (tmpS.GetLength() > 7)
					{
						tmpS = tmpS.Mid(0, tmpS.GetLength() - 4);
						tmpS += _T("천");
					}

					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 5)
		{
			const	int	cnt = 5;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("  기준  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotPrice)->m_fRGB);
					tmpS = m_items.GetAt(pivotPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("  현재  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = _T("");
					if (m_curr > 0.)
					{ tmpS.Format("%.f", m_curr); addComma(tmpS); }
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					dc->SelectObject(m_font);
					break;
				case 2:
					dc->DrawText(_T("  대비  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);    //drawinfo  우측
					dc->SetTextColor(m_items.GetAt(preBefore)->m_fRGB);
					dc->DrawText(m_items.GetAt(preBefore)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					dc->SelectObject(m_font);
					break;
				case 3:
					dc->DrawText(_T("  등락  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = m_items.GetAt(prePercent)->m_data;

					if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
						tmpS.Delete(0);

					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T("  거래  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(gVolume)->m_fRGB);
					tmpS = m_items.GetAt(gVolume)->m_data;
					if (tmpS.GetLength() > 7)
					{
						tmpS = tmpS.Mid(0, tmpS.GetLength() - 4);
						tmpS += _T("천");
					}
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 3)
		{
			const	int	cnt = 3;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("  현재  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = _T("");
					if (m_curr > 0.)
					{ 
						tmpS.Format("%.f", m_curr); addComma(tmpS); 
					}
						dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
						break;
				case 1:
					dc->DrawText(_T("  대비  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(preBefore)->m_fRGB);
					dc->DrawText(m_items.GetAt(preBefore)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("  등락  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = m_items.GetAt(prePercent)->m_data;

					if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
						tmpS.Delete(0);

					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
	}
	else if (m_showOHLC == infoVI)  
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
		
		if (m_depth == 10)
		{
			const	int	cnt = 10;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg); dc->SelectObject(m_font);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("   시  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(openPrice)->m_fRGB);
					tmpS = m_items.GetAt(openPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("   고  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(highPrice)->m_fRGB);
					tmpS = m_items.GetAt(highPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("   저  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(lowPrice)->m_fRGB);
					tmpS = m_items.GetAt(lowPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 3:
					dc->DrawText(_T("   상  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);   //10호가
					dc->SetTextColor(m_items.GetAt(shPrice)->m_fRGB);
					tmpS = m_items.GetAt(shPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T("   하  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(hhPrice)->m_fRGB);
					tmpS = m_items.GetAt(hhPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 5:
					dc->DrawText(_T("정적VI발동예상"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(pivotPrice)->m_fRGB);
					break;
				case 6:
					dc->DrawText(_T("상승가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(SVIUP)->m_fRGB);
					tmpS = m_items.GetAt(SVIUP)->m_data;
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 7:
					dc->DrawText(_T("하락가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(SVIDN)->m_fRGB);
					tmpS = m_items.GetAt(SVIDN)->m_data;
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
		else if (m_depth == 5)
		{
			const	int	cnt = 5;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("정적VI발동예상"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					//dc->SetTextColor(m_items.GetAt(pivotPrice))->m_fRGB);
					/*
					tmpS = m_items.GetAt(pivotPrice)->m_data;
					if (!m_bAble && m_config.hdyul == 1)
						if (tmpS.GetLength() > 7) tmpS.Remove(',');
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					*/
					break;
				case 1:
					dc->DrawText(_T("상승가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					//dc->SetTextColor(m_items.GetAt(cloPrice))->m_fRGB);
					dc->SetTextColor(m_items.GetAt(SVIUP)->m_fRGB);
					dc->DrawText(m_items.GetAt(SVIUP)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
					
				case 2:
					dc->DrawText(_T("하락가"), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(SVIDN)->m_fRGB);
					dc->DrawText(m_items.GetAt(SVIDN)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
					/*
				case 3:
					dc->DrawText(_T("  등락  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = m_items.GetAt(prePercent)->m_data;

					if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
						tmpS.Delete(0);

					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 4:
					dc->DrawText(_T("  거래  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(gVolume)->m_fRGB);
					tmpS = m_items.GetAt(gVolume)->m_data;
					if (tmpS.GetLength() > 7)
					{
						tmpS = tmpS.Mid(0, tmpS.GetLength() - 4);
						tmpS += _T("천");
					}
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
					*/
				}
			}
		}
		else if (m_depth == 3)
		{
			const	int	cnt = 3;
			const	int	offset = m_showTop ? m_topH+1 : 1;

			rect = m_rcInfo; rect.DeflateRect(2, 0);
			for (int ii = 0; ii < cnt; ii++)
			{
				dc->SetTextColor(m_clrDataFg);
				rect.top = offset + (int)(m_rowH * ii); rect.bottom = offset + (int)(m_rowH * (ii+1));

				switch (ii)
				{
				case 0:
					dc->DrawText(_T("  현재  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					//dc->SetTextColor(m_items.GetAt(cloPrice)->m_fRGB);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = _T("");
					if (m_curr > 0.)
					{ tmpS.Format("%.f", m_curr); addComma(tmpS); }
					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 1:
					dc->DrawText(_T("  대비  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(preBefore)->m_fRGB);
					dc->DrawText(m_items.GetAt(preBefore)->m_data, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				case 2:
					dc->DrawText(_T("  등락  "), rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
					dc->SetTextColor(m_items.GetAt(prePercent)->m_fRGB);
					tmpS = m_items.GetAt(prePercent)->m_data;

					if (!tmpS.IsEmpty() && tmpS.GetAt(0) == _T('+'))
						tmpS.Delete(0);

					dc->DrawText(tmpS, rect, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
					break;
				}
			}
		}
	}
	else if (m_showOHLC == infoMID)
	{
		dc->SetTextColor(m_clrDataFg);
		dc->SelectObject(m_font);
		dc->SetBkMode(TRANSPARENT);
	}
	dc->RestoreDC(ndc);
}

void Cdepth::drawGraph(CDC* dc, int index)
{
	CRect	rect;
	int	marginT = 0, marginB = 0;
	std::shared_ptr<Citem> itemX = nullptr;
	std::shared_ptr<Citem> item = m_items.GetAt(index);

	if (m_iDataType != DF_SEP || m_mkgubn != mkTOT)  //통합호가의 KRX, NXT 둘다 보여주기 모드가 아닌경우에 그래프
	{//둘다 보여주기 모드는 통합에서만 제공하는 기능
		if (index >= askSize1 && index <= askSize10)  //매도 잔량 그래프
		{
			rect = item->m_fRc;
			if (m_showBeforeDiff)
			{
				itemX = m_items.GetAt(index + 10);  //매도잔량대비의 왼쪽좌표로 변경
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

			rect.left = rect.right - (int)(((double)rect.Width() * str2double(item->m_data)) / m_maxsize);
			//		rect.IntersectRect(rect, item->m_bRc);
			dc->FillSolidRect(rect, m_clrBarAsk);
		}
		else if (index >= bidSize1 && index <= bidSize10)   //매수 잔량 그래프 
		{
			rect = item->m_fRc;
			if (m_showBeforeDiff)
			{
				itemX = m_items.GetAt(index + 10);  //매수잔량대비의 오른쪽좌표로 변경
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

			rect.right = rect.left + (int)(((double)rect.Width() * str2double(item->m_data)) / m_maxsize);
			//		rect.IntersectRect(rect, item->m_bRc);
			dc->FillSolidRect(rect, m_clrBarBid);
		}
	}
	
	if (m_bottom == btSize || m_bottom == btSizeTime)
	{
		double	ask = 0, bid = 0;
		
		if (index == askTotalSize)
		{
			rect = item->m_fRc;
			if (m_showBeforeDiff)
			{
				itemX = m_items.GetAt(askTotalBefore);
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
				itemX = m_items.GetAt(bidTotalBefore);
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
	CString	str;
	CRect	rect;
	const	int	ndc = dc->SaveDC();

	dc->SelectObject(m_font);
	dc->SetTextColor(m_clrHeadFg);
	dc->SetBkMode(TRANSPARENT);
	
	switch (m_type)
	{
	case ctCode:
	case ctECN:
	case ctFuture:
	case ctOption:
	case ctKOption:
		if (m_showBeforeDiff)
		{
			rect.SetRect(0, 1, m_columns[0], (int)(m_topH+1));
			rect.left = RATERECTSEL(rect);
			if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)  //KRX, NXT 인경우
				dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			else
			{
				if (m_iDataType == DF_TOT) //통합
					dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				else
					dc->DrawText(_T("KRX"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
			
			rect.SetRect(0, 1, m_columns[0], (int)(m_topH + 1));
			rect.right = RATERECTSEL(rect);

			if (m_bConfig)
			{
				CRect rc;
				rc.SetRect(0, 1, m_columns[0], (int)(m_topH + 1));
				m_rcConfigL.SetRect(rc.right - (rc.Height()), rc.top + 1 , rc.right - 2, rc.bottom - 2);
				rect.DeflateRect(3, 1, 20, 1);
			}

#ifdef LP_AMOUNT
			if (IsELW(m_code))
				str = _T("LP");
			else if (m_type == ctCode || m_type == ctECN)
			{
				if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)
					str = _T("대비");
				else
				{
					if (m_iDataType == DF_TOT) //통합
						str = _T("대비");
					else
						str = _T("NXT");
				}
			}
			else	str = _T("건수");
			
			if (str == "대비" || str == "NXT")
			{
				rect.OffsetRect(10, 0);
				dc->DrawText(str, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
			else
				dc->DrawText(str, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
#else
			if (m_type == ctCode || m_type == ctECN)
				dc->DrawText(_T("대비"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			else
				dc->DrawText(_T("건수"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
#endif
			
			rect.SetRect(m_columns[0] + 1, 1, m_columns[1], (int)(m_topH + 1));
			
			if (m_bConfig)
			{
				m_rcConfig.SetRect(rect.right - (rect.Height()), rect.top + 1, rect.right - 2, rect.bottom -2);
				rect.DeflateRect(3, 1, 20, 1);
			}

			int left = rect.left - 2;
			int right = rect.left + m_rcConfig.Width() - 3;
			int top = rect.top;
			int bottom = rect.bottom - 1;
			m_rcMKPOPMenu.SetRect(left, top, right, bottom);
			//m_rcMKPOPMenu.InflateRect(2, 0);
			m_rcMarket = m_rcMKPOPMenu;
			//m_rcMarket.OffsetRect(m_rcMarket.Width() + 2, 0);

			CRect timeRec;
			timeRec = rect;
			timeRec.OffsetRect(5, 0);  //상단 시간영역
			std::shared_ptr<Citem> item = m_items.GetAt(cTime);
			CFont* poldfont = dc->SelectObject(m_sfont);
			dc->DrawText(item->m_data, timeRec, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			dc->SelectObject(poldfont);

			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			rect.right = RATERECTBUY(rect);

			if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)  //KRX, NXT 인경우
				dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			else
			{
				if (m_iDataType == DF_TOT) //통합
					dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				else
					dc->DrawText(_T("KRX"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}

			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			rect.right = RATERECTBUY(rect);
		
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			rect.left = RATERECTBUY(rect);

			if (m_bConfig)
			{
				CRect rc;
				rc = rect;
				rc.right = rect.left;
				rc.left = rc.right - rect.Height();
				m_rcConfigR = rc;
				m_rcConfigR.DeflateRect(1, 1);
				m_rcConfigR.bottom -= 1;
				//m_rcConfigR.SetRect(rect.right - (rect.Height()), rect.top + 1, rect.right - 1, rect.bottom-1);
				rect.DeflateRect(3, 1, 20, 1);
			}

#ifdef LP_AMOUNT
			if (IsELW(m_code))
				str = _T("LP");
			else if (m_type == ctCode || m_type == ctECN)
			{
				if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)
					str = _T("대비");
				else
				{
					if (m_iDataType == DF_TOT) //통합
						str = _T("대비");
					else
						str = _T("NXT");
				}
			}
			else	str = _T("건수");

			if (str == "대비" || str == "NXT")
			{
				rect.OffsetRect(10, 0);
				dc->DrawText(str, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
			else
				dc->DrawText(str, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
#else
			if (m_type == ctCode || m_type == ctECN)
				dc->DrawText(_T("대비"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			else
				dc->DrawText(_T("건수"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
#endif
		}
		else
		{
			rect.SetRect(0, 1, m_columns[0], (int)(m_topH + 1));
			if (m_bConfig)
			{
				CRect rc;
				rc.SetRect(0, 1, m_columns[0], (int)(m_topH + 1));
				m_rcConfigL.SetRect(rc.right - (rc.Height()), rc.top + 1, rc.right - 2, rc.bottom - 2);
				rect.DeflateRect(3, 1, 20, 1);
			}
			if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)  //KRX, NXT 인경우
				dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			else
			{
				if (m_iDataType == DF_TOT) //통합
					dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				else
					dc->DrawText(_T("KRX"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
			
			rect.SetRect(m_columns[0] + 1, 1, m_columns[1], (int)(m_topH + 1));
			if (m_bConfig)
			{
				m_rcConfig.SetRect(rect.right - (rect.Height()), rect.top + +1, rect.right - 2, rect.bottom -2);
				rect.DeflateRect(3, 1, 20, 1);
			}

			int left = rect.left - 2;
			int right = rect.left + m_rcConfig.Width() - 3;
			int top = rect.top;
			int bottom = rect.bottom - 1;
			m_rcMKPOPMenu.SetRect(left, top, right, bottom);
			//m_rcMKPOPMenu.InflateRect(2, 0);
			m_rcMarket = m_rcMKPOPMenu;
			//m_rcMarket.OffsetRect(m_rcMarket.Width() + 2, 0);

			dc->DrawText(_T("호가"), rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			rect.SetRect(m_columns[1] + 1, 1, m_columns[2], (int)(m_topH + 1));
			if (m_bConfig)
			{
				m_rcConfigR.SetRect(rect.right - (rect.Height()), rect.top + 1, rect.right - 1, rect.bottom-1);
				rect.DeflateRect(3, 1, 20, 1);
			}
			if (m_mkgubn == mkKRX || m_mkgubn == mkNXT)  //KRX, NXT 인경우
				dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			else
			{
				if (m_iDataType == DF_TOT) //통합
					dc->DrawText(_T("잔량"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				else
					dc->DrawText(_T("KRX"), rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			}
		}
	}
	dc->RestoreDC(ndc);
}

void Cdepth::drawBottom(CDC* dc)
{
	CRect	bRc, fRc;
	const	int	ndc = dc->SaveDC();
	const	int	top = m_showTop ? m_topH : 0;

     //modi NXT  하단 예상체결관련
	const	int	startL = m_depth * 2 + m_iShowMIDPRC;
	int	endL   = startL;
	
	switch (m_bottom)
	{
	case btSize:
	case btTime:
		endL += 1;	break;
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
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btSize:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (startL + 1) + 1) + top, m_columns[1], (int)(m_rowH * (startL + 2)) + top);
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1) + top, m_columns[1], (int)(m_rowH*endL) + top);
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1, 18, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_iBottomType == 0)  //예상가
				if (m_bPredict)  //test comment  btTime !m_showBeforeDiff
				{
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1) + top, m_columns[1], (int)(m_rowH*endL) + top);
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1, 18, 1);
				fRc.DeflateRect(0, 0, 18, 0);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_iBottomType == 0)  //예상가 
				if (m_bPredict)  //test comment  btTime m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			break;
		case btSizeTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1) + top, m_columns[1], (int)(m_rowH * endL) + top);
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1, 18, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_iBottomType == 0)  //예상가 
				if (m_bPredict)   //test comment btSizeTime !m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1) + top, m_columns[1], (int)(m_rowH * endL) + top);
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1, 18, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_iBottomType == 0)  //예상가
				if (m_bPredict)  //test comment  btSizeTime m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
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
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btSize:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (startL + 1) + 1), m_columns[1], (int)(m_rowH * (startL + 2)));
				fRc = bRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
			break;
		case btTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1), m_columns[1], (int)(m_rowH*endL));
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.DeflateRect(3, 1, 18, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_ bPredict) //test comment  btTime !m_showBeforeDiff
				//if (m_iBottomType == 0)  //예상가
				if (m_bPredict) //test comment  btTime !m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * startL + 1), m_columns[1], (int)(m_rowH * endL));
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1, 18, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_iBottomType == 0)  //예상가
				if (m_bPredict) //test comment  btTime m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			break;
		case btSizeTime:
			if (!m_showBeforeDiff)
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1), m_columns[1], (int)(m_rowH * endL));
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				dc->DrawText(_T("대비"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				
				bRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.OffsetRect(0, (int)(m_rowH + 1));
				fRc.DeflateRect(3, 1, 18, 1);
				dc->FillSolidRect(bRc, m_clrHeadBk);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				//if (m_iBottomType == 0)  //예상가
				if (m_bPredict) //test comment  btSizeTime !m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//{
						dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
						bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
						bRc.right = RATERECTSEL(bRc);
						fRc = bRc;
						fRc.DeflateRect(3, 1);
						dc->FillSolidRect(bRc, m_clrHeadBk);
						dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

						bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
						bRc.left = RATERECTBUY(bRc) + 1;
						fRc = bRc;
						fRc.DeflateRect(3, 1);
						dc->FillSolidRect(bRc, m_clrHeadBk);
						dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			else
			{
				bRc.SetRect(m_columns[0] + 1, (int)(m_rowH * (endL-1) + 1), m_columns[1], (int)(m_rowH * endL));
				fRc = bRc;
				m_rcExpecArea = fRc;
				fRc.DeflateRect(3, 1);
				fRc.DeflateRect(3, 1, 18, 1);
				{
					//int	topOff = (bRc.Height() - imgCY2) / 2;
					//m_rcTime.SetRect(bRc.right - (imgCX1+3), bRc.top + topOff, bRc.right - 3, bRc.top + topOff + imgCY2);
					m_rcTime.SetRect(bRc.right - (bRc.Height()+1), bRc.top +1, bRc.right - 3, bRc.bottom-1);
				}

				dc->FillSolidRect(bRc, m_clrHeadBk);
				//if (m_iBottomType == 0)  //예상가
				if (m_bPredict) //test comment  btSizeTime m_showBeforeDiff
				{
					//dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					dc->DrawText(_T("예상체결"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					
					bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					bRc.right = RATERECTSEL(bRc);
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("예상가"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);

					bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					bRc.left = RATERECTBUY(bRc) + 1;
					fRc = bRc;
					fRc.DeflateRect(3, 1);
					dc->FillSolidRect(bRc, m_clrHeadBk);
					dc->DrawText(_T("체결량"), fRc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
				}
				else
				{
					dc->DrawText(_T("시간외"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//if (m_mkgubn == mkNXT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	if (m_iBottomType == 2)   //현재가(K)를 보여줄때
					//	{
					//		bRc.SetRect(1, bRc.top, m_columns[0], bRc.bottom);
					//		bRc.right = RATERECTSEL(bRc);
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("종가"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

					//		bRc.SetRect(m_columns[1] + 1, bRc.top, m_columns[2], bRc.bottom);
					//		bRc.left = RATERECTBUY(bRc) + 1;
					//		fRc = bRc;
					//		fRc.DeflateRect(3, 1);
					//		dc->FillSolidRect(bRc, m_clrHeadBk);
					//		dc->DrawText(_T("거래량"), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//	}
					//}
					//else if (m_mkgubn == mkKRX || m_mkgubn == mkTOT)
					//{
					//	dc->DrawText(GetBottomDataType(__FUNCTION__, __LINE__), fRc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					//}
				}
			}
			break;
		}
	}
	dc->RestoreDC(ndc);
}

void Cdepth::drawLine(CDC* dc)
{
	const	int	top = m_showTop ? m_topH : 0;
	CPen	*pen = getPen(m_clrLine);
	CPen	*ppen = dc->SelectObject(pen);
	
	dc->MoveTo(0, 0);
	dc->LineTo(m_columns[2], 0);
	if (m_showTop)
	{
		dc->MoveTo(0, (int)m_topH);
		dc->LineTo(m_columns[2], (int)m_topH);	
	}
	dc->MoveTo(0, (int)(m_rowH * m_depth + top));
	dc->LineTo(m_columns[2], (int)(m_rowH * m_depth + top));
	
     //modi NXT  우측하단 호가잔량영역 상단 가로줄
	if (m_iShowMIDPRC)
	{
		dc->MoveTo(0, (int)(m_rowH * m_depth + top + (m_rowH * m_iShowMIDPRC)));
		dc->LineTo(m_columns[2], (int)(m_rowH * m_depth + top + (m_rowH * m_iShowMIDPRC)));
	}
	else
	{
		dc->MoveTo(0, (int)(m_rowH * m_depth * 2 + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * m_depth * 2 + top));
	}

	if (m_depth == 10)
	{
		LOGBRUSH lb{};

		lb.lbStyle = BS_SOLID; 
		lb.lbColor = m_clrDash;
		lb.lbHatch = 0;

		HPEN	hPenDash = (HPEN)ExtCreatePen(PS_GEOMETRIC|PS_DOT, 1, &lb, 0, NULL);
		HPEN	hPenDashOld = (HPEN)dc->SelectObject(hPenDash);

		dc->MoveTo(0, (int)(m_rowH * (m_depth/2) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth/2) + top));

		dc->MoveTo(0, (int)(m_rowH * (m_depth + m_depth/2 + m_iShowMIDPRC) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth+m_depth/2 + m_iShowMIDPRC) + top));

		dc->SelectObject(hPenDashOld);
		DeleteObject(hPenDash);
	}

	int	vertical = 0;
	switch (m_bottom)
	{
	case btNo:
		dc->MoveTo(0, (int)(m_rowH * m_depth*2 + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * m_depth*2 + top));
		vertical = m_depth*2;
		if (!m_showBeforeDiff)
		{
			dc->MoveTo(0, (int)(m_rowH * (m_depth * 2 + 2) + top));
			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth * 2 + 2) + top));
			vertical++;
		}
		break;
		
	case btSize:
	case btTime:
		dc->MoveTo(0, (int)(m_rowH * (m_depth * 2 + 2) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth * 2 + 2) + top));
		vertical = m_depth*2 + 1;
		if (!m_showBeforeDiff)
		{
			dc->MoveTo(0, (int)(m_rowH * (m_depth * 2 + 4) + top));
			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth * 2 + 4) + top));
			vertical++;
		}
		break;
		
	case btSizeTime:
		dc->MoveTo(0, (int)(m_rowH * (m_depth * 2 + m_iShowMIDPRC) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth * 2 + m_iShowMIDPRC) + top));

		dc->MoveTo(0, (int)(m_rowH * (m_depth * 2 + m_iShowMIDPRC + 1) + top));
		dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth * 2 + m_iShowMIDPRC + 1) + top));
		vertical = m_depth * 2 + 2;
		if (!m_showBeforeDiff)
		{
			dc->MoveTo(0, (int)(m_rowH * (m_depth * 2 + 4) + top));
			dc->LineTo(m_columns[2], (int)(m_rowH * (m_depth * 2 + 4) + top));
			vertical++;
		}
		break;
	}

	dc->MoveTo(0, 0);
	dc->LineTo(0, (int)(m_rowH * vertical + top));
	
	dc->MoveTo(m_columns[0], 0);
	dc->LineTo(m_columns[0], (int)(m_rowH * vertical + top));

	if (m_bSiseMemo)
	{
		dc->MoveTo(m_columns[1] - memoWIDTH, top);
		dc->LineTo(m_columns[1] - memoWIDTH, (int)(m_rowH * (m_depth * 2)  + top));
	}
	
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
		int	left = 0, right = 0;
		
		rect.SetRect(1, 1, m_columns[0], 2);
		left = RATERECTSEL(rect);
		
		rect.SetRect(m_columns[1] + 1, 1, m_columns[2], 2);
		right = RATERECTBUY(rect);
		
		//호가와 등락률 사이에 라인 그리는 것 시작 - 정문일
		if (bELW && m_percent)
		{
			rect.SetRect(m_columns[0]+1, (int)(m_rowH*1), m_columns[1], (int)(m_rowH*(2+1)));

			int dleft = 0;
			if (m_bAble && m_bBigDigit)
				dleft = rect.left + (int)(rect.Width() * P_BIG_RATIO);
			else
				dleft = rect.left + (int)(rect.Width() * P_RATIO);
			dc->MoveTo(dleft, (int)m_rowH+1 );
			dc->LineTo(dleft, (int)(m_rowH * (m_depth*2) + top));
		}
		//호가와 등락률 사이에 라인 그리는 것 끝
		
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

		  //modi NXT  우측하단 호가잔량과 그오른쪽 빈영역 세로줄
		dc->MoveTo(right, (int)(m_rowH* (m_depth + m_iShowMIDPRC) + top));
		dc->LineTo(right, (int)(m_rowH* (m_depth * 2 + 2) + top));

		switch (m_bottom)
		{
		case btSize:
		case btTime:
			if (m_mkgubn == mkKRX)
			{
				dc->MoveTo(left, (int)(m_rowH * m_depth * 2 + top));
				dc->LineTo(left, (int)(m_rowH * (m_depth * 2 + 1) + top));

				dc->MoveTo(right, (int)(m_rowH * m_depth * 2 + top));
				dc->LineTo(right, (int)(m_rowH * (m_depth * 2 + 1) + top));
			}
			break;
		case btSizeTime:
			/*if (m_mkgubn == mkKRX)
			{
				dc->MoveTo(left, (int)(m_rowH * m_depth * 2 + top));
				dc->LineTo(left, (int)(m_rowH * (m_depth * 2 + 2) + top));

				dc->MoveTo(right, (int)(m_rowH * m_depth * 2 + top));
				dc->LineTo(right, (int)(m_rowH * (m_depth * 2 + 2) + top));
			}*/
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
	if (m_bSiseMemo)
		drawSiseMemo(dc);
	//if (m_showBeforeDiff)	// 20080426 khk
	drawInfo(dc);
	if (m_showLine)
		drawLine(dc);
	if (m_showTop)
		drawHead(dc);
	if (m_bBong)
		drawBong(dc);

	drawBottom(dc);
	drawButton(dc);
	drawBox(dc);
	

	dc->SelectObject(font);
}

void Cdepth::drawBground(CDC* dc)
{
	CRect	rect;
	const	int	top = m_showTop ? m_topH : 0;
	
	GetClientRect(rect); 
	dc->FillSolidRect(rect, m_clrWhite);

	if (m_showTop)
	{
		rect.SetRect(0, 0, m_columns[2], (int)m_topH);
		dc->FillSolidRect(rect, m_clrHeadBk);
	}
	
	switch (m_bottom)
	{
	case btSize:
	case btTime:
		rect.SetRect(0, (int)(m_rowH * (m_depth*2) + top), m_columns[2], (int)(m_rowH * (m_depth*2 + 1) + top));
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
	if (m_bAble && m_bBigDigit)
	{
		if (m_showBeforeDiff)
		{
			m_columns[0] = (int)(width * 0.33);
			m_columns[1] = (int)(width * 0.67);
			m_columns[2] = width;
		}
		else
		{
			m_columns[0] = (int)(width * 0.33);
			m_columns[1] = (int)(width * 0.67);
			m_columns[2] = width;
		}
	}
	else
	{
		if (m_showBeforeDiff)
		{
			m_columns[0] = (int)(width * 0.35);
			m_columns[1] = (int)(width * 0.65);
			m_columns[2] = width;
		}
		else
		{
			m_columns[0] = (int)(width * 0.35);
			m_columns[1] = (int)(width * 0.65);
			m_columns[2] = width;
		}
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
		divider += 1;	break;
	case btSizeTime:
		divider += 2;	break;
	}
	if (!m_showBeforeDiff)
		divider += 1;

	//modi 중간가
    //modi NXT 최초 호가영역 분할영역 갯수 지정
	divider += m_iShowMIDPRC;

	m_rowH = (double)(m_rect.Height() - 1) / (double)divider;
	m_OneMoreRowH = (double)(m_rect.Height() - 1) / (double)(divider + 3);
	m_topH = (int)m_rowH + 1;
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
	std::shared_ptr<Citem> item = nullptr;
	
	for (int ii = 0; ii < N_DATA; ii++)
	{
		item = m_items.GetAt(ii);
		item->m_fRc.SetRectEmpty();
		item->m_bRc.SetRectEmpty();
	}
	
	if (m_depth == 3)
		setItemRect3();
	else if (m_depth == 5)
		setItemRect5();
	else
		setItemRect10();
	
	getPriceRect();
}

void Cdepth::setItemRect3()
{
	int	ii = 0, line = 0;
	std::shared_ptr<Citem> item = nullptr;
	std::shared_ptr<Citem> sItem = nullptr, eItem = nullptr;

	// 매도
	if (m_percent)
	{
		line = 0;
		for (ii = askPrice3; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 0;
		for (ii = askPercent3; ii >= askPercent1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		line = 0;
		for (ii = askMemo3; ii >= askMemo1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMDS = askPrice1; m_idxMDE = askPercent3;
	}
	else
	{
		line = 0;
		for (ii = askPrice3; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 0;
		for (ii = askMemo3; ii >= askMemo1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMDS = askPrice1; m_idxMDE = askPrice3;
	}
	
	if (m_showBeforeDiff)
	{
		line = 0;
		for (ii = askBefore3; ii >= askBefore1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		line = 0;
		for (ii = askLp3; ii >= askLp1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 0;
		for (ii = askSize3; ii >= askSize1; ii--, line++)
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
		for (ii = askBefore3; ii >= askBefore1; ii--, line++)
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
		for (ii = askLp3; ii >= askLp1; ii--, line++)
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
		for (ii = askSize3; ii >= askSize1; ii--, line++)
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
		line = 3;
		for (ii = bidPrice1; ii <= bidPrice3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}

		line = 3;
		for (ii = bidPercent1; ii <= bidPercent3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		line = 3;
		for (ii = bidMemo1; ii <= bidMemo3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPercent3;
	}
	else
	{
		line = 3;
		for (ii = bidPrice1; ii <= bidPrice3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 3;
		for (ii = bidMemo1; ii <= bidMemo3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPrice3;
	}
	
	if (m_showBeforeDiff)
	{
		line = 3;
		for (ii = bidSize1; ii <= bidSize3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 3;
		for (ii = bidBefore1; ii <= bidBefore3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		line = 3;
		for (ii = bidLp1; ii <= bidLp3; ii++, line++)
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
		line = 3;
		for (ii = bidSize1; ii <= bidSize3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		line = 3;
		for (ii = bidBefore1; ii <= bidBefore3; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		line = 3;
		for (ii = bidLp1; ii <= bidLp3; ii++, line++)
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
		m_rcInfo.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+3)));
	else
		m_rcInfo.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+3)));
	if (m_showTop)	m_rcInfo.OffsetRect(0, m_topH + 1);	

	switch (m_bottom)
	{
	case btNo:
		line = 6;
		
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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btSize:
		line = 6;
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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btTime:
		line = 6;
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
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btSizeTime:
		line = 6;
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
#ifdef LP_AMUONT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		
		line = 7 + (m_showBeforeDiff ? 0 : 1);
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
	}
	if (m_bBong) 
	{
		if (m_percent)
		{
			sItem = m_items.GetAt(askPercent3);
			eItem = m_items.GetAt(askPercent1);
			item  = m_items.GetAt(askBong);
			item->m_bRc.SetRect(sItem->m_bRc.right+1, sItem->m_bRc.top, eItem->m_bRc.right+bongWIDTH, eItem->m_bRc.bottom);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		else
		{
			sItem = m_items.GetAt(askPrice3);
			eItem = m_items.GetAt(askPrice1);
			item  = m_items.GetAt(askBong);
			item->m_bRc.SetRect(sItem->m_bRc.right+1, sItem->m_bRc.top, eItem->m_bRc.right+bongWIDTH, eItem->m_bRc.bottom);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
}

void Cdepth::setItemRect5()
{
	int	ii = 0, line = 0;
	std::shared_ptr<Citem> item = nullptr;
	std::shared_ptr<Citem> sItem = nullptr, eItem = nullptr;

	// 매도
	if (m_percent)
	{
		line = 0;
		for (ii = askPrice5; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 0;
		for (ii = askPercent5; ii >= askPercent1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		line = 0;
		for (ii = askMemo5; ii >= askMemo1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
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
		
		line = 0;
		for (ii = askMemo5; ii >= askMemo1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
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
		for (ii = askLp5; ii >= askLp1; ii--, line++)
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
		for (ii = askLp5; ii >= askLp1; ii--, line++)
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

	for (int ii = 0; ii < 5; ii++)  //통합호가 개별거래소 영역    매도
	{
		item = m_items.GetAt(askSize1 + ii);
		m_items.GetAt(kmd1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(kmd1size + ii)->m_fRc = item->m_fRc;

		item = m_items.GetAt(askBefore1 + ii);
		m_items.GetAt(nmd1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(nmd1size + ii)->m_fRc = item->m_fRc;
	}

	// 매수
	
	if (m_percent)
	{
		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidPrice1; ii <= bidPrice5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidPercent1; ii <= bidPercent5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidMemo1; ii <= bidMemo5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPercent5;
	}
	else
	{
		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidPrice1; ii <= bidPrice5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidMemo1; ii <= bidMemo5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPrice5;
	}
	
	if (m_showBeforeDiff)
	{
		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidSize1; ii <= bidSize5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		//modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidBefore1; ii <= bidBefore5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		  //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidLp1; ii <= bidLp5; ii++, line++)
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
		//modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidSize1; ii <= bidSize5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		//modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidBefore1; ii <= bidBefore5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		 //modi NXT 5호가 하단 라인 시작 5->6
		line = 5 + m_iShowMIDPRC;

		for (ii = bidLp1; ii <= bidLp5; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}

	for (int ii = 0; ii < 5; ii++)  //통합호가 개별거래소 영역    매수
	{
		item = m_items.GetAt(bidSize1 + ii);
		m_items.GetAt(kms1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(kms1size + ii)->m_fRc = item->m_fRc;

		item = m_items.GetAt(bidBefore1 + ii);
		m_items.GetAt(nms1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(nms1size + ii)->m_fRc = item->m_fRc;
	}

	line = 0;
	if (m_bBong)
		m_rcInfo.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+5)));
	else
		m_rcInfo.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+5)));
	if (m_showTop)	m_rcInfo.OffsetRect(0, m_topH + 1);	

	 //modi NXT  ref 5호가의 중간가 1컬 2컬 3컬  중간가영역의 rect를 정해준다 !!!!!   setmidrec 5호가
	if (m_iShowMIDPRC)
	{
		for (int ii = MIDKPRCMD; ii <= MIDKPRCDBMS; ii++)
		{
			line = 5;
			item = m_items.GetAt(ii);

			switch (ii)
			{
			case MIDKPRCMD:   // 중간가매도잔량
				item->m_bRc.SetRect(m_items.GetAt(askSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDKPRICE:  // 중간가 영역 지정 KRX
			{
				item->m_bRc.SetRect(m_items.GetAt(askPrice1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPrice1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcMIDPercent.SetRect(m_items.GetAt(askPercent1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPercent1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcMIDPercent.OffsetRect(-3, m_topH + 1);

				if (m_config.hdyul == 0)  //등락율 안보기
				{
					int igap = (m_columns[1] - m_columns[0])/4.6;
					igap = -1 * igap - (0.2524*(m_columns[1] - m_columns[0])-22.72);
					item->m_bRc.OffsetRect(igap, 0);
				}
				
			}
			break;
			case MIDKPRCMS:  // 중간가매수잔량
				item->m_bRc.SetRect(m_items.GetAt(bidSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDKPRCDBMD:   // 중간가매도대비
				item->m_bRc.SetRect(m_items.GetAt(askBefore1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askBefore1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDKPRCDBMS:    // 중간가매수대비
				item->m_bRc.SetRect(m_items.GetAt(bidBefore5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidBefore5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			}

			m_slog.Format("[%s][%s]  ii=[%d] line=[%d]  m_rowH=[%f] mid top=[%d]  [%d]", DF_LOGKEY, DF_LOGKEY1, ii, line, m_rowH, m_rcMIDPercent.top, m_items.GetAt(MIDKPRICE)->m_bRc.top);
			Output_DebugString(m_slog);

			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}

		//통합호가 KRX 중간가 영역 지정 5호가
		for (int ii = tkmid; ii < tkmidrate; ii++)
		{
			item = m_items.GetAt(ii);
			switch (ii)
			{
				case tkmid:  //중간가
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRICE)->m_bRc.left, m_items.GetAt(MIDKPRICE)->m_bRc.top,
						m_items.GetAt(MIDKPRICE)->m_bRc.right, m_items.GetAt(MIDKPRICE)->m_bRc.bottom);
				}
				break;
				case tkmiddv:  //매도잔량 
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCMD)->m_bRc.left, m_items.GetAt(MIDKPRCMD)->m_bRc.top,
						m_items.GetAt(MIDKPRCMD)->m_bRc.right, m_items.GetAt(MIDKPRCMD)->m_bRc.bottom);
				}
				break;
				case tkmidsv:  //매수잔량
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCMS)->m_bRc.left, m_items.GetAt(MIDKPRCMS)->m_bRc.top,
						m_items.GetAt(MIDKPRCMS)->m_bRc.right, m_items.GetAt(MIDKPRCMS)->m_bRc.bottom);
				}
				break;
				case tkmiddr:  //매도비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCDBMD)->m_bRc.left, m_items.GetAt(MIDKPRCDBMD)->m_bRc.top,
						m_items.GetAt(MIDKPRCDBMD)->m_bRc.right, m_items.GetAt(MIDKPRCDBMD)->m_bRc.bottom);
				}
				break;
				case tkmidsr:  //매수비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCDBMS)->m_bRc.left, m_items.GetAt(MIDKPRCDBMS)->m_bRc.top,
						m_items.GetAt(MIDKPRCDBMS)->m_bRc.right, m_items.GetAt(MIDKPRCDBMS)->m_bRc.bottom);
				}
				break;
			}
			item->m_fRc = item->m_bRc;
		}

		for (int ii = MIDNPRCMD; ii <= MIDNPRCDBMS; ii++)
		{
			if(m_mkgubn == mkNXT || m_mkgubn == mkKRX)
				line = 5; 
			else
				line = 6;

			item = m_items.GetAt(ii);

			switch (ii)
			{
			case MIDNPRCMD:   // 중간가MD잔량
				item->m_bRc.SetRect(m_items.GetAt(askSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDNPRICE:  // 중간가 영역 지정 NXT
			{
				item->m_bRc.SetRect(m_items.GetAt(askPrice1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPrice1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcNMIDPercent.SetRect(m_items.GetAt(askPercent1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPercent1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcNMIDPercent.OffsetRect(-3, m_topH + 1);

				if (m_config.hdyul == 0)  //등락율 안보기
				{
					int igap = (m_columns[1] - m_columns[0]) / 4.6;
					igap = -1 * igap - (0.2524 * (m_columns[1] - m_columns[0]) - 22.72);
					item->m_bRc.OffsetRect(igap, 0);
				}

			}
			break;
			case MIDNPRCMS:  // 중간가매수잔량
				item->m_bRc.SetRect(m_items.GetAt(bidSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDNPRCDBMD:   // 중간가매도대비
				item->m_bRc.SetRect(m_items.GetAt(askBefore1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askBefore1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDNPRCDBMS:    // 중간가매수대비
				item->m_bRc.SetRect(m_items.GetAt(bidBefore5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidBefore5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			}

			m_slog.Format("[%s][%s]  ii=[%d] line=[%d]  m_rowH=[%f] mid top=[%d]  [%d]", DF_LOGKEY, DF_LOGKEY1, ii, line, m_rowH, m_rcMIDPercent.top, m_items.GetAt(MIDKPRICE)->m_bRc.top);

			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}

		//통합호가 NXT 중간가 영역 지정 5호가
		for (int ii = tnmid; ii < tnmidrate; ii++)
		{
			item = m_items.GetAt(ii);
			switch (ii)
			{
				case tnmid:  //중간가
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRICE)->m_bRc.left, m_items.GetAt(MIDNPRICE)->m_bRc.top,
						m_items.GetAt(MIDNPRICE)->m_bRc.right, m_items.GetAt(MIDNPRICE)->m_bRc.bottom);
				}
				break;
				case tnmiddv:  //매도잔량 
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCMD)->m_bRc.left, m_items.GetAt(MIDNPRCMD)->m_bRc.top,
						m_items.GetAt(MIDNPRCMD)->m_bRc.right, m_items.GetAt(MIDNPRCMD)->m_bRc.bottom);
				}
				break;
				case tnmidsv:  //매수잔량
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCMS)->m_bRc.left, m_items.GetAt(MIDNPRCMS)->m_bRc.top,
						m_items.GetAt(MIDNPRCMS)->m_bRc.right, m_items.GetAt(MIDNPRCMS)->m_bRc.bottom);
				}
				break;
				case tnmiddr:  //매도비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCDBMD)->m_bRc.left, m_items.GetAt(MIDNPRCDBMD)->m_bRc.top,
						m_items.GetAt(MIDNPRCDBMD)->m_bRc.right, m_items.GetAt(MIDNPRCDBMD)->m_bRc.bottom);
				}
				break;
				case tnmidsr:  //매수비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCDBMS)->m_bRc.left, m_items.GetAt(MIDNPRCDBMS)->m_bRc.top,
						m_items.GetAt(MIDNPRCDBMS)->m_bRc.right, m_items.GetAt(MIDNPRCDBMS)->m_bRc.bottom);
				}
				break;
			}
			item->m_fRc = item->m_bRc;
		}//for
	}
	else
	{
		for (int ii = MIDKPRCMD; ii <= MIDKPRCDBMS; ii++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRectEmpty();
		}
	}

	switch (m_bottom)
	{
	case btNo:
		 //modi NXT 5호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		if (!m_showBeforeDiff)
		{
			item = m_items.GetAt(askTotalBefore);   //setItemRect5
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btSize:
		 //modi NXT 5호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

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
			
			item = m_items.GetAt(askTotalBefore);  //setItemRect5
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
			
			item = m_items.GetAt(bidTotalBefore);    //setItemRect5
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
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
			
			item = m_items.GetAt(askTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btTime:
		 //modi NXT 5호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

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
			item = m_items.GetAt(askTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);  //setItemRect5
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
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btSizeTime:
		 //modi NXT 5호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

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
			
			item = m_items.GetAt(askTotalBefore);  //setItemRect5
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
			
			item = m_items.GetAt(bidTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		else
		{
			item = m_items.GetAt(askTotalSize);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(askTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalSize);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalBefore);  //setItemRect5
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
	
	   //modi NXT 5호가 하단 라인 마지막 11->12
		line = 11 + m_iShowMIDPRC + (m_showBeforeDiff ? 0 : 1);

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
	}
	if (m_bBong) 
	{
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
	std::shared_ptr<Citem> item = nullptr;
	std::shared_ptr<Citem> sItem = nullptr, eItem = nullptr;
	
	// 매도
	if (m_percent)
	{
		line = 0;
		for (ii = askPrice10; ii >= askPrice1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}
		
		line = 0;
		for (ii = askPercent10; ii >= askPercent1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		line = 0;
		for (ii = askMemo10; ii >= askMemo1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
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
		
		line = 0;
		for (ii = askMemo10; ii >= askMemo1; ii--, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
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
		for (ii = askLp10; ii >= askLp1; ii--, line++)
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
		for (ii = askLp10; ii >= askLp1; ii--, line++)
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
	
	for (int ii = 0; ii < 10; ii++)  //통합호가 개별거래소 영역    매도
	{
		item = m_items.GetAt(askSize1 + ii);
		m_items.GetAt(kmd1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(kmd1size + ii)->m_fRc = item->m_fRc;

		item = m_items.GetAt(askBefore1 + ii);
		m_items.GetAt(nmd1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(nmd1size + ii)->m_fRc = item->m_fRc;
	}


	// 매수
	if (m_percent)
	{
		 //modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidPrice1; ii <= bidPrice10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.right = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc; 
			item->m_fRc.DeflateRect(1, 1);
		}
		
		 //modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidPercent1; ii <= bidPercent10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			if (m_bAble && m_bBigDigit)
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_BIG_RATIO);
			else
				item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);
			//item->m_bRc.left = item->m_bRc.left + (int)(item->m_bRc.Width() * P_RATIO);//item->m_bRc.CenterPoint().x;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1); item->m_fRc.right -= 2;
		}
		
		//modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidMemo1; ii <= bidMemo10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPercent10;

	}
	else
	{
		 //modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidPrice1; ii <= bidPrice10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[0]+1, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.right -= (item->m_fRc.Width()-55)/2;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		 //modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidMemo1; ii <= bidMemo10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			if (m_bSiseMemo)
				item->m_bRc.SetRect(m_columns[1] - memoWIDTH, (int)(m_rowH*line), m_columns[1], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
		}
		m_idxMSS = bidPrice1; m_idxMSE = bidPrice10;
	}

	if (m_showBeforeDiff)
	{
		//modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidSize1; ii <= bidSize10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTBUY(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		//modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidLp1; ii <= bidLp10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		//modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

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
		 //modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidSize1; ii <= bidSize10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
		
		//modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidBefore1; ii <= bidBefore10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}

		 //modi NXT 10호가 하단 라인 마지막 10->11
		line = 10 + m_iShowMIDPRC;

		for (ii = bidLp1; ii <= bidLp10; ii++, line++)
		{
			item = m_items.GetAt(ii);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
		}
	}
	
	for (int ii = 0; ii < 10; ii++)  //통합호가 개별거래소 영역    매수
	{
		item = m_items.GetAt(bidSize1 + ii);
		m_items.GetAt(kms1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(kms1size + ii)->m_fRc = item->m_fRc;

		item = m_items.GetAt(bidBefore1 + ii);
		m_items.GetAt(nms1size + ii)->m_bRc = item->m_bRc;
		m_items.GetAt(nms1size + ii)->m_fRc = item->m_fRc;
	}

	line = 0;
	if (m_bBong)
		m_rcInfo.SetRect(m_columns[1]+bongWIDTH+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+10)));
	else
		m_rcInfo.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+10)));
	if (m_showTop)	m_rcInfo.OffsetRect(0, m_topH + 1);
	
	//modi NXT ref 10호가 중간가 1컬 2컬 3컬  중간가영역의 rect를 정해준다 !!!!!  setmidrec  10호가
	if (m_iShowMIDPRC)
	{
		int iculindex{};
		for (int ii = MIDKPRCMD; ii <= MIDKPRCDBMS; ii++)
		{
			iculindex = ii - MIDKPRCMD;
			line = 10;

			item = m_items.GetAt(ii);

			switch (ii)
			{
			case MIDKPRCMD:   // 중간가매도잔량
				item->m_bRc.SetRect(m_items.GetAt(askSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDKPRICE:  // 중간가 영역 지정  KRX 10호가
			{
				item->m_bRc.SetRect(m_items.GetAt(askPrice1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPrice1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcMIDPercent.SetRect(m_items.GetAt(askPercent1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPercent1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcMIDPercent.OffsetRect(-3, m_topH + 1);

				if (m_config.hdyul == 0)  //등락율 안보기
				{
					int igap = (m_columns[1] - m_columns[0]) / 4.6;
					igap = -1 * igap - (0.2524 * (m_columns[1] - m_columns[0]) - 22.72);
					item->m_bRc.OffsetRect(igap, 0);
				}
			}
			break;
			case MIDKPRCMS:  // 중간가매수잔량
				item->m_bRc.SetRect(m_items.GetAt(bidSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDKPRCDBMD:   // 중간가매도대비
				item->m_bRc.SetRect(m_items.GetAt(askBefore1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askBefore1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDKPRCDBMS:    // 중간가매수대비
				item->m_bRc.SetRect(m_items.GetAt(bidBefore5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidBefore5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;

			}


			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}

		//통합호가 KRX 중간가 영역 지정 10호가
		for (int ii = tkmid; ii < tkmidrate; ii++)
		{
			item = m_items.GetAt(ii);
			switch (ii)
			{
				case tkmid:  //중간가
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRICE)->m_bRc.left, m_items.GetAt(MIDKPRICE)->m_bRc.top,
						m_items.GetAt(MIDKPRICE)->m_bRc.right, m_items.GetAt(MIDKPRICE)->m_bRc.bottom);
				}
				break;
				case tkmiddv:  //매도잔량 
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCMD)->m_bRc.left, m_items.GetAt(MIDKPRCMD)->m_bRc.top,
						m_items.GetAt(MIDKPRCMD)->m_bRc.right, m_items.GetAt(MIDKPRCMD)->m_bRc.bottom);
				}
				break;
				case tkmidsv:  //매수잔량
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCMS)->m_bRc.left, m_items.GetAt(MIDKPRCMS)->m_bRc.top,
						m_items.GetAt(MIDKPRCMS)->m_bRc.right, m_items.GetAt(MIDKPRCMS)->m_bRc.bottom);
				}
				break;
				case tkmiddr:  //매도비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCDBMD)->m_bRc.left, m_items.GetAt(MIDKPRCDBMD)->m_bRc.top,
						m_items.GetAt(MIDKPRCDBMD)->m_bRc.right, m_items.GetAt(MIDKPRCDBMD)->m_bRc.bottom);
				}
				break;
				case tkmidsr:  //매수비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDKPRCDBMS)->m_bRc.left, m_items.GetAt(MIDKPRCDBMS)->m_bRc.top,
						m_items.GetAt(MIDKPRCDBMS)->m_bRc.right, m_items.GetAt(MIDKPRCDBMS)->m_bRc.bottom);
				}
				break;
			}
			item->m_fRc = item->m_bRc;
		}//for

		for (int ii = MIDNPRCMD; ii <= MIDNPRCDBMS; ii++)
		{
			if (m_mkgubn == mkNXT || m_mkgubn == mkKRX)
				line = 10;
			else
				line = 11;
	
			item = m_items.GetAt(ii);

			switch (ii)
			{
			case MIDNPRCMD:   // 중간가MD잔량
				item->m_bRc.SetRect(m_items.GetAt(askSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDNPRICE:  // 중간가
			{
				item->m_bRc.SetRect(m_items.GetAt(askPrice1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPrice1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcNMIDPercent.SetRect(m_items.GetAt(askPercent1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askPercent1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				m_rcNMIDPercent.OffsetRect(-3, m_topH + 1);

				if (m_config.hdyul == 0)  //등락율 안보기
				{
					int igap = (m_columns[1] - m_columns[0]) / 4.6;
					igap = -1 * igap - (0.2524 * (m_columns[1] - m_columns[0]) - 22.72);
					item->m_bRc.OffsetRect(igap, 0);
				}

			}
			break;
			case MIDNPRCMS:  // 중간가매수잔량
				item->m_bRc.SetRect(m_items.GetAt(bidSize5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidSize5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDNPRCDBMD:   // 중간가매도대비
				item->m_bRc.SetRect(m_items.GetAt(askBefore1)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(askBefore1)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			case MIDNPRCDBMS:    // 중간가매수대비
				item->m_bRc.SetRect(m_items.GetAt(bidBefore5)->m_bRc.left, (int)(m_rowH * line), m_items.GetAt(bidBefore5)->m_bRc.right, (int)(m_rowH * (line + 1)));
				break;
			}

			m_slog.Format("[%s][%s]  ii=[%d] line=[%d]  m_rowH=[%f] mid top=[%d]  [%d]", DF_LOGKEY, DF_LOGKEY1, ii, line, m_rowH, m_rcMIDPercent.top, m_items.GetAt(MIDKPRICE)->m_bRc.top);

			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);

			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(1, 1);
		}

		for (int ii = tnmid; ii < tnmidrate; ii++)
		{
			item = m_items.GetAt(ii);
			switch (ii)
			{
				case tnmid:  //중간가
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRICE)->m_bRc.left, m_items.GetAt(MIDNPRICE)->m_bRc.top,
						m_items.GetAt(MIDNPRICE)->m_bRc.right, m_items.GetAt(MIDNPRICE)->m_bRc.bottom);
				}
				break;
				case tnmiddv:  //매도잔량 
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCMD)->m_bRc.left, m_items.GetAt(MIDNPRCMD)->m_bRc.top,
						m_items.GetAt(MIDNPRCMD)->m_bRc.right, m_items.GetAt(MIDNPRCMD)->m_bRc.bottom);
				}
				break;
				case tnmidsv:  //매수잔량
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCMS)->m_bRc.left, m_items.GetAt(MIDNPRCMS)->m_bRc.top,
						m_items.GetAt(MIDNPRCMS)->m_bRc.right, m_items.GetAt(MIDNPRCMS)->m_bRc.bottom);
				}
				break;
				case tnmiddr:  //매도비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCDBMD)->m_bRc.left, m_items.GetAt(MIDNPRCDBMD)->m_bRc.top,
						m_items.GetAt(MIDNPRCDBMD)->m_bRc.right, m_items.GetAt(MIDNPRCDBMD)->m_bRc.bottom);
				}
				break;
				case tnmidsr:  //매수비
				{
					item->m_bRc.SetRect(m_items.GetAt(MIDNPRCDBMS)->m_bRc.left, m_items.GetAt(MIDNPRCDBMS)->m_bRc.top,
						m_items.GetAt(MIDNPRCDBMS)->m_bRc.right, m_items.GetAt(MIDNPRCDBMS)->m_bRc.bottom);
				}
				break;
			}
			item->m_fRc = item->m_bRc;
		}//for
	}

	switch (m_bottom)
	{
	case btNo:
		 //modi NXT 10호가 하단 라인 마지막 20->21
		line = 20 + m_iShowMIDPRC;

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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;			
		
	case btSize:
		 //modi NXT 10호가 하단 라인 마지막 20->21
		line = 20 + m_iShowMIDPRC;

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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btTime :
		 //modi NXT 10호가 하단 라인 마지막 20->21
		line = 20 + m_iShowMIDPRC;

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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		break;
		
	case btSizeTime:
		 //modi NXT 10호가 하단 라인 마지막 20->21
		line = 20 + m_iShowMIDPRC;

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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*line), m_columns[0], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.right = RATERECTSEL(item->m_bRc);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
			
			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*line), m_columns[2], (int)(m_rowH*(line+1)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_bRc.left = RATERECTBUY(item->m_bRc)+1;
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
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
#ifdef LP_AMOUNT
			item = m_items.GetAt(askTotalLp);
			item->m_bRc.SetRect(1, (int)(m_rowH*(line+1)), m_columns[0], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);

			item = m_items.GetAt(bidTotalLp);
			item->m_bRc.SetRect(m_columns[1]+1, (int)(m_rowH*(line+1)), m_columns[2], (int)(m_rowH*(line+2)));
			if (m_showTop)	item->m_bRc.OffsetRect(0, m_topH + 1);
			item->m_fRc = item->m_bRc;
			item->m_fRc.DeflateRect(3, 1);
#endif
		}
		 //modi NXT 10호가 하단 라인 마지막 21->22
		line = 21 + m_iShowMIDPRC + (m_showBeforeDiff ? 0 : 1);

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

	if (m_percent)
	{
		if (m_bAble && m_bBigDigit)
			m_rcPriceTip.left = m_prect.left + (int)(m_prect.Width()*P_BIG_RATIO);
		else
			m_rcPriceTip.left = m_prect.left + (int)(m_prect.Width()*P_RATIO);
	}
	else
		m_rcPriceTip.left = m_prect.left + (int)(m_prect.Width()*7.6 / 10);

	m_rcPriceTip.right = m_rcPriceTip.left+6;
	m_rcPriceTip.top = m_prect.top;
	m_rcPriceTip.bottom = m_prect.bottom;
	//CString tmp;
	//tmp.Format("left : %d,  right : %d,  top : %d, bottom : %d", m_rcPriceTip.left, m_rcPriceTip.right, m_rcPriceTip.top, m_rcPriceTip.bottom);
	//MessageBox(tmp);

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
			 //modi NXT 중간가에 마우스를 올리면 색이 변하는데 이상태에서 아예 호가윈도우를 벗어나면 색 원복
			else if (m_focus == MIDKPRICE && m_iShowMIDPRC)
			{
				m_items.GetAt(m_focus)->m_pRGB = CLR_MIDPRC;
				m_focus = -1;
				InvalidateRect(m_prect);
			}
		}
	}
	else
	{
		int	ii = 0;
		bool	found = false;
		std::shared_ptr<Citem> item = nullptr, itemx = nullptr;

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

		 //modi NXT setfocus 중간가 마우스 가면 색변하기 위한
		if (!found && m_iShowMIDPRC)
		{
			item = m_items.GetAt(MIDKPRICE);
			if (item->m_bRc.PtInRect(point))
			{
				ii = MIDKPRICE;
				found = true;
			}

			item = m_items.GetAt(MIDNPRICE);
			if (item->m_bRc.PtInRect(point))
			{
				ii = MIDNPRICE;
				found = true;
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

CString slog;
slog.Format("[midprc] m_focus = [%d] [%s]", ii, item->m_data);
Output_DebugString(slog);

			if (!item->m_data.IsEmpty())
			{
				item->m_pRGB = RGB(255, 0, 0);
				item->m_pRGB  = m_clrFocus;
				itemx->m_pRGB = m_clrFocus;
			}
			InvalidateRect(m_prect);
		}
		if (found && !m_percent)
		{
			bool	bShow = false;
		
			if (m_prect.PtInRect(point) && (!m_rcPriceTip.PtInRect(point)))
			{
				
				CPoint	pt = point;
				ClientToScreen(&pt);
				pt.x += 10;
				pt.y += 10;
				
				CRect	rect;
				CSize	size;	
				CString string;
				string = calculatePercentByIndex(ii);//m_items.GetAt(ii+10))->m_data;
				
				string.Trim();
				
				if (string=="")
				{
					m_pToolTip->ShowWindow(SW_HIDE);
					
				}
				else
				{	
					rect = m_pToolTip->SetData("  "+string+" %\t\n");
					
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
					return;
					
				}	
			}
			/*
			if (!bShow)
			{
				m_pToolTip->ShowWindow(SW_HIDE);
				return;
			}
			*/
		}
	}

	if (m_focus == MIDKPRICE || m_focus == MIDNPRICE)
	{
		m_slog.Format("[\r\n cx_depth.dll][%s]<%d> m_focus = [%d] \r\n", __FUNCTION__, __LINE__, m_focus);
		Output_DebugString(m_slog);

		CString strTooltip;
		if (m_mkgubn == mkKRX)
		{
			strTooltip.Format("▶ KRX 중간가 : %s  \t\n▶ 중간가란? \t\n최우선 매도호가와 최우선 매수호가  \t\n의 중간가격이에요. \t\nK는 KRX의 중간가, N은 NXT의 중간가를 의미해요. \t\n",
				m_items.GetAt(MIDKPRICE)->m_data.IsEmpty() ? "000,000" : m_items.GetAt(MIDKPRICE)->m_data);
		}
		else if(m_mkgubn == mkNXT)
		{
			strTooltip.Format("▶ NXT중간가 : %s \t\n▶ 중간가란? \t\n최우선 매도호가와 최우선 매수호가  \t\n의 중간가격이에요. \t\nK는 KRX의 중간가, N은 NXT의 중간가를 의미해요. \t\n",
				m_items.GetAt(MIDNPRICE)->m_data.IsEmpty() ? "000,000" : m_items.GetAt(MIDNPRICE)->m_data);
		}
		else if(m_mkgubn == mkTOT)
		{
			strTooltip.Format("▶ KRX 중간가 : %s  \t\n▶ NXT중간가 : %s \t\n▶ 중간가란? \t\n최우선 매도호가와 최우선 매수호가  \t\n의 중간가격이에요. \t\nK는 KRX의 중간가, N은 NXT의 중간가를 의미해요. \t\n",
				m_items.GetAt(tkmid)->m_data.IsEmpty() ? "000,000" : m_items.GetAt(tkmid)->m_data,
				m_items.GetAt(tnmid)->m_data.IsEmpty() ? "000,000" : m_items.GetAt(tnmid)->m_data);
		}
	
	
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

		m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);  //else if (m_bBong)

		if (m_pToolTip->IsWindowVisible())
			m_pToolTip->Invalidate(FALSE);
		else
			m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
	}
	else if (m_bSiseMemo)
	{
		CString strTooltip;
		bool	bShow = false;
		std::shared_ptr<Citem> pItem = nullptr;

		for (int ii = askMemo1; ii <= bidMemo10; ii++)
		{
			pItem = m_items.GetAt(ii);
			if (pItem->m_data.IsEmpty())
				continue;

			if (pItem->m_bRc.PtInRect(point))
			{
				strTooltip = GetTooltipStr(ii);

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
				{
					m_pToolTip->Invalidate(FALSE);
					break;
				}
				else
				{
					m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);
					break;
				}
			}
		} 
		if (!bShow)
			m_pToolTip->ShowWindow(SW_HIDE);
	}
	else if (m_rcPriceTip.PtInRect(point))  //가격툴팁
	{
		if (m_focus == -1) return; 
		if (m_config.shl != 1) return;
		bool	bShow = false;
		
		if (m_rcPriceTip.PtInRect(point))
		{
			
			CPoint	pt = point;
			ClientToScreen(&pt);
			pt.x += 10;
			pt.y += 10;
			
			CRect	rect;
			CSize	size;	
			CString string;
			string = m_items.GetAt(m_focus)->m_data;
			string = GetHogaTip(string);
			
			if (string=="")
			{
				m_pToolTip->ShowWindow(SW_HIDE);
				
			}
			else
			{	
				rect = m_pToolTip->SetData(string);
				
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
		
		if (!bShow)
			m_pToolTip->ShowWindow(SW_HIDE);
	}
	else if (m_rcTime.PtInRect(point))
	{
		bool	bShow = false;

		if (m_rcTime.PtInRect(point))
		{
			CPoint	pt = point;
			ClientToScreen(&pt);
			pt.x += 10;
			pt.y += 10;
			
			CRect	rect;
			CSize	size;	
			
			if (!m_bPredict)  //하단토글버턴 마우스 오버시 툴팁내용
			{
				if (m_mkgubn == mkNXT)
					rect = m_pToolTip->SetData(GetBottomDataType(__FUNCTION__, __LINE__) + "\t\n");
				else
					rect = m_pToolTip->SetData(GetBottomDataType(__FUNCTION__, __LINE__) + "\t\n");
			}
			else
			{
				if (m_mkgubn == mkNXT)
					rect = m_pToolTip->SetData(GetBottomDataType(__FUNCTION__, __LINE__) + "\t\n");
				else
					rect = m_pToolTip->SetData(GetBottomDataType(__FUNCTION__, __LINE__) + "\t\n");
			}
			 
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

		if (!bShow)
			m_pToolTip->ShowWindow(SW_HIDE);
	}
	else if (m_rcInfo.PtInRect(point))  //오른쪽 상단 
	{
		
// 		if (m_depth == 10)
// 		{
// 			if (m_showOHLC != infoSiga && m_showOHLC != infoCurr) return;
// 		}
// 		else if (m_depth == 5)
// 		{
// 			if (m_showOHLC != infoSiga) return;
// 		}
		if (m_showOHLC == infoNo)
			return;
		
		int	infoCount = 0;

		if (m_depth == 10)
		{
			//if (m_showOHLC == infoSiga || m_showOHLC == infoPivot)
			if (m_showOHLC == infoPivot)
				infoCount = 9;
			//else if (m_showOHLC == infoCurr)
			else if (m_showOHLC == infoCurr || m_showOHLC == infoSiga || m_showOHLC == infoVI)
				infoCount = 10;
		}
		else if (m_depth == 5)
		{
			infoCount = 5;
		}

		CRect	rect;
		bool	bShow = false;
		const	int	cnt = m_depth;
		const	int	offset = m_showTop ? m_topH+1 : 1;

		rect = m_rcInfo;
		rect.DeflateRect(2, 0);
		//rect.top = offset + (int)(m_rowH * 0); rect.bottom = offset + (int)(m_rowH * (3));
		if (m_depth == 5 && m_showOHLC == infoSiga)
		{
			infoCount++;
			//	rect.top = offset + (int)(m_rowH * 0); rect.bottom = offset + (int)(m_rowH * (infoCount));
			rect.top = offset + (int)(m_OneMoreRowH * 0); rect.bottom = offset + (int)(m_OneMoreRowH * (infoCount));
		}
		else
		{
			rect.top = offset + (int)(m_rowH * 0); rect.bottom = offset + (int)(m_rowH * (infoCount));
		}
	

		if (rect.PtInRect(point))
		{
			CString	hTip;
			CRect	irect;
			CString	cs_price = m_items.GetAt(cloPrice)->m_data;

			cs_price.Remove(',');
			if (atol(cs_price) == 0) return;
			for (int ii = 0; ii < infoCount; ii++)
			{
				irect = rect;
				if (m_depth == 5 && m_showOHLC == infoSiga)
				{
					irect.top = offset + (int)(m_OneMoreRowH * ii); irect.bottom = offset + (int)(m_OneMoreRowH * (ii+1));
				}
				else
				{
					irect.top = offset + (int)(m_rowH * ii); irect.bottom = offset + (int)(m_rowH * (ii+1));
				}
				
				if (irect.PtInRect(point))
				{
					
					double	c_rate = 0; 
					double	data = 0;
					const	double	c_price = atol(cs_price);
					CString sd_price,hd_price,ld_price,sdv_price,hdv_price,ldv_price;

					switch (ii)
					{
					case 0:
						if ((m_depth == 10 && (m_showOHLC == infoSiga || m_showOHLC == infoCurr || m_showOHLC == infoVI)) || (m_depth == 5 && m_showOHLC == infoSiga))
						{
							sd_price = m_items.GetAt(openPrice)->m_data;
							sdv_price = sd_price;
							sd_price.Remove(',');
							data = atof(sd_price);
							
							if (data > 0)
							{
								c_rate = ((data-c_price)/c_price*100);
								hTip.Format("시가 : %s (%.2f %%)", sdv_price, c_rate);
							}
							else
								hTip.Format("시가 : %s ", sdv_price);
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							return;
						}
						else if (m_depth == 5 && m_showOHLC == infoPivot)
						{
							hTip = _T("2차 저항");
						}
						else if (m_depth == 5 && m_showOHLC == infoCurr)
						{
							hTip = _T("기준가");
						}
						else if (m_depth == 5 && m_showOHLC == infoVI)
						{
							m_pToolTip->ShowWindow(SW_HIDE);
							return;
						}
						break;
					case 1:
						if ((m_depth == 10 && (m_showOHLC == infoSiga || m_showOHLC == infoCurr || m_showOHLC == infoVI)) || (m_depth == 5 && m_showOHLC == infoSiga))
						{
							hd_price = m_items.GetAt(highPrice)->m_data;
							hdv_price = hd_price;
							hd_price.Remove(',');
							data = atof(hd_price);
							
							if (data > 0)
							{
								c_rate = ((data-c_price)/c_price*100);
								hTip.Format("고가 : %s (%.2f %%)", hdv_price, c_rate);
							}
							else
								hTip.Format("고가 : %s ", hdv_price);
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("2차 저항");
						}
						else if (m_depth == 5 && m_showOHLC == infoPivot)
						{
							hTip = _T("1차 저항");
						}
						else if (m_depth == 5 && m_showOHLC == infoCurr)
						{
							hTip = _T("현재가");
						}
						else if (m_depth == 5 && m_showOHLC == infoVI)
						{
						//	m_pToolTip->ShowWindow(SW_HIDE);	 
							hTip = _T("정적VI상승가");
					//		return;
						}
						break;
					case 2:
						if ((m_depth == 10 && (m_showOHLC == infoSiga || m_showOHLC == infoCurr || m_showOHLC == infoVI)) || (m_depth == 5 && m_showOHLC == infoSiga))
						{
							ld_price = m_items.GetAt(lowPrice)->m_data;
							ldv_price = ld_price;
							ld_price.Remove(',');
							data = atof(ld_price);
							
							if (data > 0)
							{
								c_rate = ((data-c_price)/c_price*100);
								hTip.Format("저가 : %s (%.2f %%)", ldv_price, c_rate);
							}
							else
								hTip.Format("저가 : %s ", ldv_price);
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("1차 저항");
						}
						else if (m_depth == 5 && m_showOHLC == infoPivot)
						{
							hTip = _T("피봇");
						}
						else if (m_depth == 5 && m_showOHLC == infoCurr)
						{
							hTip = _T("대비");
						}
						else if (m_depth == 5 && m_showOHLC == infoVI)
						{
						//	m_pToolTip->ShowWindow(SW_HIDE);	
							hTip = _T("정적VI하락가");
						//	return;
						}
						break;
					case 3:
						if ((m_depth == 10 && (m_showOHLC == infoSiga || m_showOHLC == infoCurr || m_showOHLC == infoVI)) || (m_depth == 5 && m_showOHLC == infoSiga))
						{
							//if (m_mkgubn == mkKRX)
								hTip = _T("상한가");
							//hTip.Format("상한가 \t  %s", m_items.GetAt(94)->m_data);
							//else if(m_mkgubn == mkNXT)  //▶ 중간가(KRX) 
							//	hTip = _T("▶ 전일(K)란? KRX전일종가에요.\t\n  ▶ 당일(K)란 KRX장중엔 KRX현\t\n  재가, KRX장 마감후엔 KRX당일종\t\n  가에요  ");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("피봇");
						}
						else if (m_depth == 5 && m_showOHLC == infoPivot)
						{
							hTip = _T("1차 지지");
						}
						else if (m_depth == 5 && m_showOHLC == infoCurr)
						{
							hTip = _T("등락률");
						}
						else if (m_depth == 5 && m_showOHLC == infoVI)
						{
							m_pToolTip->ShowWindow(SW_HIDE);	
							return;
						}
						break;
					case 4:
						if ((m_depth == 10 && (m_showOHLC == infoSiga || m_showOHLC == infoCurr || m_showOHLC == infoVI)) || (m_depth == 5 && m_showOHLC == infoSiga))
						{
							//if (m_mkgubn == mkKRX)
								hTip = _T("하한가");
							//else if (m_mkgubn == mkNXT)  //▶ 중간가(KRX) 
							//	hTip = _T("▶ 전일(K)란? KRX전일종가에요.\t\n  ▶ 당일(K)란 KRX장중엔 KRX현\t\n  재가, KRX장 마감후엔 KRX당일종\t\n  가에요  ");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("1차 지지");
						}
						else if (m_depth == 5 && m_showOHLC == infoPivot)
						{
							hTip = _T("2차 지지");
						}
						else if (m_depth == 5 && m_showOHLC == infoCurr)
						{
							hTip = _T("거래량");
						}
						else if (m_depth == 5 && m_showOHLC == infoVI)
						{
							m_pToolTip->ShowWindow(SW_HIDE);	
							return;
						}
						break;
					case 5:
						if (m_depth == 10 && m_showOHLC == infoSiga)
						{
							hTip = _T("52주 최고가");
						}
						else if (m_depth == 10 && m_showOHLC == infoVI)
						{
							m_pToolTip->ShowWindow(SW_HIDE);	
							return;
						}
						else if (m_depth == 10 && m_showOHLC == infoCurr)
						{
							hTip = _T("기준가");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("2차 지지");
						}
						else if (m_depth == 5)
						{
							ld_price = m_items.GetAt(chegang)->m_data;
							hTip.Format("체결강도 : %s ", ld_price);	
						}
						break;
					case 6:
						if (m_depth == 10 && m_showOHLC == infoSiga)
						{
							hTip = _T("52주 최저가");
						}
						else if (m_depth == 10 && m_showOHLC == infoCurr)
						{
							hTip = _T("현재가");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							return;
						}
						else if (m_depth == 10 && m_showOHLC == infoVI)
						{
				//			m_pToolTip->ShowWindow(SW_HIDE);  
							hTip = _T("정적VI상승가");
						//	return;
						}
						break;
					case 7:
						if (m_depth == 10 && m_showOHLC == infoSiga)
						{
							hTip = _T("가중 평균가");
						}
						else if (m_depth == 10 && m_showOHLC == infoCurr)
						{
							hTip = _T("대비");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("디마크 저항");
						}
						else if (m_depth == 10 && m_showOHLC == infoVI)
						{
							//m_pToolTip->ShowWindow(SW_HIDE);	
							hTip = _T("정적VI하락가");
						//	return;
						}
						break;
					case 8:
						if (m_depth == 10 && m_showOHLC == infoSiga)
						{
							hTip = _T("기준가");
						}
						else if (m_depth == 10 && m_showOHLC == infoCurr)
						{
							hTip = _T("등락률");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							hTip = _T("디마크 지지");
						}
						else if (m_depth == 10 && m_showOHLC == infoVI)
						{
							m_pToolTip->ShowWindow(SW_HIDE);	
							return;
						}
						break;
					case 9:
						if (m_depth == 10 && m_showOHLC == infoSiga)
						{
							ld_price = m_items.GetAt(chegang)->m_data;
							hTip.Format("체결강도 : %s ", ld_price);	
						}
						else if (m_depth == 10 && m_showOHLC == infoCurr)
						{
							hTip = _T("거래량");
						}
						else if (m_depth == 10 && m_showOHLC == infoPivot)
						{
							return;
						}
						else if (m_depth == 10 && m_showOHLC == infoVI)
						{
							m_pToolTip->ShowWindow(SW_HIDE);	
							return;
						}
						break;
					default :
						break;
					}
					break;
				}

			}
							
			CPoint	pt = point;
			ClientToScreen(&pt);
			pt.x += 10;
			pt.y += 10;
			
			CRect	rect;
			CSize	size;	
			
			rect = m_pToolTip->SetData("  "+hTip+"\t\n");
			 
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
			return;
		}
		if (!bShow)
			m_pToolTip->ShowWindow(SW_HIDE);	

		
	}
	else if (m_rcExpecArea.PtInRect(point))
	{
		bool	bShow = false;

		if (m_rcExpecArea.PtInRect(point) && m_mkgubn == mkNXT)
		{
			CPoint	pt = point;
			ClientToScreen(&pt);
			pt.x += 10;
			pt.y += 10;

			CRect	rect;
			CSize	size;


			if (m_bPredict)
				rect = m_pToolTip->SetData("▶ 예상체결(N)란? \t\nNXT 애프터마켓 개시전에 예상되는\t\n시가의 가격과 체결량이에요.\t\n ");
			//else
			//    rect = m_pToolTip->SetData("▶ 현재가(K)란?\t\nKRX 장중엔 현재가. KRX 장 마감\t\n후엔 KRX 당일종가에요.\t\n");

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

		if (!bShow)
			m_pToolTip->ShowWindow(SW_HIDE);
	}
	else if (m_bBong)
	{
		CString strTooltip;
		bool	bShow = false;
		std::shared_ptr<Citem> pItem = m_items.GetAt(askBong);
		
		if (pItem->m_bRc.PtInRect(point))
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

		if (!bShow)
		{
			m_pToolTip->ShowWindow(SW_HIDE);
			if (m_bClickMIDPRC && m_iShowMIDPRC)
				m_bClickMIDPRC = !m_bClickMIDPRC;
		}
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
			return askPrice1 + ii - askPercent1;
	}

	for (int ii = bidPrice1; ii <= bidPrice10; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return ii;
	}

	for (int ii = bidPercent1; ii <= bidPercent10; ii++)
	{
		if (m_items.GetAt(ii)->m_bRc.PtInRect(point))
			return bidPrice1 + ii - bidPercent1;
	}

	//modi NXT getindexbypoint 마우스커서 위치를 통해서 호가영역의 어디인지 판단 <-- 중간가 추가
	if (m_items.GetAt(MIDKPRICE)->m_bRc.PtInRect(point))
		return MIDKPRICE;

	if (m_items.GetAt(MIDNPRICE)->m_bRc.PtInRect(point))
		return MIDNPRICE;

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
	/*
	CString tmp; tmp.Format("m_percent: %d\n", m_percent);
	TRACE(tmp);
	tmp.Format("m_percent: %d\n", pcJgga);
	TRACE(tmp);
	*/
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

	if (clos == 0)
		return;

	std::shared_ptr<Citem> item = nullptr;
	double	price = 0, percent = 0;

	for (int ii = 0; ii < 10/*m_depth*/; ii++)
	{
		price = str2double(m_items.GetAt(askPrice1+ii)->m_data);
		if (price > 0)
		{
			item = m_items.GetAt(askPercent1+ii);
			if (price > clos)
			{
				percent = ((price - clos) * 100)/clos;
				item->m_data.Format("%2.2lf", CeilEX(percent,2));
				item->m_fRGB = m_clrUp;
			}
			else if (price < clos)
			{
				percent = ((price - clos) * 100)/clos;
				item->m_data.Format("%.2lf", CeilEX(percent,2));
				item->m_data.Remove('-');
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
				percent = ((price - clos) * 100)/clos;
				item->m_data.Format("%2.2lf", CeilEX(percent,2));
				item->m_fRGB = m_clrUp;
			}
			else if (price < clos)
			{
				percent = ((price - clos) * 100)/clos;
				item->m_data.Format("%.2lf", CeilEX(percent,2));
				item->m_data.Remove('-');
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
	double	price = 0;
	CString	string, tmpS;

	price = m_curr;//fabs(str2double(m_items.GetAt(prePrice))->m_data));
	if (price == 0)
		return;

	double	mdsize = str2double(m_items.GetAt(askTotalSize)->m_data);
	mdsize = fabs(mdsize);
	double	mssize = str2double(m_items.GetAt(bidTotalSize)->m_data);
	mssize = fabs(mssize);

	price = mssize - mdsize;
	if (price > 0)
		string.Format("+%.f", price);
	else
		string.Format("%.f", price);
	m_items.GetAt(sizeDiff)->m_data = format(string, sizeDiff);
	string = m_items.GetAt(sizeDiff)->m_data;
}

void Cdepth::popupMenu(CPoint point, CString Price)
{
	switch (m_type)
	{
	case ctCode:
	case ctFuture:
	case ctOption:
	case ctECN:
		break;
	default:
		return;
	}

	CString	code, string, maps;
	CPoint	pt = point;
	int pos;
	ClientToScreen(&pt);
	CMenu	popM;
	popM.CreatePopupMenu();
	/*popM.AppendMenu(MF_STRING|MF_ENABLED, menuBase,   _T("매도주문"));
	popM.AppendMenu(MF_STRING|MF_ENABLED, menuBase+1, _T("매수주문"));
	popM.AppendMenu(MF_STRING|MF_ENABLED, menuBase+2, _T("정정/취소주문"));
	int	index = popM.TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, this);
*/
	pos = m_code.Find(_T("."));
	if (pos != -1)
		code = m_code.Mid(pos + 1);
	else 
		code = m_code;
	
	CString sArrMap[4] = {};
	if (code.GetAt(0) == 'J' && code.GetLength() == 9)
	{
		sArrMap[0] = "IB680100";
		sArrMap[1] = "IB680100";
		sArrMap[2] = "IB680100";
		sArrMap[3] = "IB680100";

		popM.AppendMenu(MF_STRING | MF_ENABLED, menuBase, _T("6801 신주인수권주문"));
	}
	else
	{
		sArrMap[0] = "IB101100";
		sArrMap[1] = "IB101200";
		sArrMap[2] = "IB101300";
		sArrMap[3] = "IB101400";

		if (!code.IsEmpty() && (code.GetAt(0) == 'A' || code.GetAt(0) == 'J'))
			code = code.Mid(1);

		popM.AppendMenu(MF_STRING | MF_ENABLED, menuBase, _T("매도주문"));
		popM.AppendMenu(MF_STRING | MF_ENABLED, menuBase + 1, _T("매수주문"));
		popM.AppendMenu(MF_STRING | MF_ENABLED, menuBase + 2, _T("정정/취소주문"));
	}

	int	index = popM.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, this);
	index -= menuBase;
	switch (m_type)
	{
	case ctCode:
		switch (index)
		{
		case 0:	maps = sArrMap[0];	break;
		case 1:	maps = sArrMap[1];	break;
		case 2:	maps = sArrMap[2];	break;
		case 3:	maps = sArrMap[3];	break;
	/*	case 0:	maps = _T("IB101100");	break;
		case 1:	maps = _T("IB101200");	break;
		case 2:	maps = _T("IB101300");	break;
		case 3:	maps = _T("IB101400");	break;*/
		default:return;
		}
		if (code.GetAt(0) == 'J' && code.GetLength() == 9)
			string.Format("%s /S/t0/d%s\t%s", maps, "scoode", code);
		else
			string.Format("%s/S/d%s\t%s\n%s\t%s\n%s\t%d\n", maps, symCODE, code, m_refsym, Price, "order_tab", index+1);
		//AfxMessageBox(string);
		//TRACE("==============================================\n");
		//TRACE(string+"\n");
		//TRACE("==============================================\n");
		openView(typeVIEW, string);
		break;

	case ctECN:
		switch (index)
		{
		case 0:	maps = sArrMap[0];	break;
		case 1:	maps = sArrMap[1];	break;
		case 2:	maps = sArrMap[2];	break;
		case 3:	maps = sArrMap[3];	break;
		//case 0:	maps = _T("IB101100");	break;
		//case 1:	maps = _T("IB101200");	break;
		//case 2:	maps = _T("IB101300");	break;
		//case 3:	maps = _T("IB101400");	break;
		default:return;
		}
		if (code.GetAt(0) == 'J' && code.GetLength() == 9)
			string.Format("%s /S/t0/d%s\t%s", maps, "scoode", code);
		else
			string.Format("%s/S/d%s\t%s\n%s\t%s\n%s\t%d\n", maps, symECN, code, m_refsym, Price, "order_tab", index+1);
		
		openView(typeVIEW, string);
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
	{
		CString pdat, sval;
		sval = m_code;
		sval.Replace("M.", "");
		sval.Replace("N.", "");
		if (m_sPrcTrigger_File.GetLength() > 0)
		{
			CString stmp;
			char	wb[32];
			GetPrivateProfileString("IB100000", "edit_trigger", "", wb, sizeof(wb), m_sUserConfFile);
			stmp.Format("%s", wb);
			stmp.TrimRight();
			if (stmp.Find("edStopPrc") >= 0)
				pdat.Format("SetStopPrice\t%s,%s", sval, Price);
			else
				pdat.Format("SetPrice\t%s,%s", sval, Price);
		}
		else
		{
			if (m_iPrcType == TYPE_STOPPRC)
				pdat.Format("SetStopPrice\t%s,%s", sval, Price);
			else
				pdat.Format("SetPrice\t%s,%s", sval, Price);
		}

		m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)pdat);
			
//		//trigger(m_refsym + "\t" + Price);
	}

}

int Cdepth::openView(int type, CString param)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)param.operator LPCTSTR());
}

void Cdepth::trigger(CString param)
{
	m_parent->SendMessage(WM_USER, MAKEWPARAM(formDLL, -1), (LPARAM)param.GetString());
	m_parent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_key, evOnClick)), (LPARAM)m_szName.GetString());
	Variant(triggerCC, param);
}

CString Cdepth::format(CString data, int index)
{
	CString	tmps, sign;
	std::shared_ptr<Citem> item = m_items.GetAt(index);

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
#if 1
		if (data.IsEmpty())
			break;
		sign = data.GetAt(0);
		data = data.Mid(1);	data.Trim();
		if (sign[0] == '1')
		{
			sign = "↑";
			item->m_fRGB = m_clrUp;
		}
		else if (sign[0] == '2')
		{
			sign = "▲";
			item->m_fRGB = m_clrUp;
		}
		else if (sign[0] == '3')
		{
			sign = "";
			item->m_fRGB = m_clrDataFg;
		}
		else if (sign[0] == '4')
		{
			sign = "↓";
			item->m_fRGB = m_clrDown;
		}
		else if (sign[0] == '5')
		{
			sign = "▼";	
			item->m_fRGB = m_clrDown;
		}
		else
		{
			sign = "";
			item->m_fRGB = m_clrDataFg;
		}
		addComma(data);
		data = sign + data;
		break;
#else 
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
#endif
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
			item->m_fRGB = m_clrDataFg;
		break;

	case atCoRgbSup:	// +- 에 따라 색깔을 표시, Sign 기호 제외
		sign = getSign(data);
		eliminateZero(data);
		if (!atof(data))
		{
			item->m_fRGB = m_clrDataFg;
			return _T("");
		}
		addComma(data);
		item->m_fRGB = m_clrDataFg;
		if (!sign.IsEmpty())
		{
			if (sign.GetAt(0) == '+')
				item->m_fRGB = m_clrUp;
			else if (sign.GetAt(0) == '-')
				item->m_fRGB = m_clrDown;
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
			return _T("0.00%");
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
		eliminateZero(data);
		addComma(data);
		break;
	}

	return data;
}

void Cdepth::addComma(CString& data)
{
	CString	tmps;
	int	pos = 0, length = 0;

	tmps = data;
//	tmps.Trim();
	length = tmps.GetLength();
	pos = tmps.Find('.') + 1;
	length -= pos;

	if (length < 4)
		return;

	data.Empty();
	for (int ii = 0; ii < length; ii++)
	{
		if ((length-ii) % 3 == 0 && ii != 0)
			data += ',';
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

	if (data.GetLength() > 1 && (data.GetAt(1) == '+' || data.GetAt(1) == '-'))
	{
		sign += data.GetAt(1);
		skip++;
	}

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

bool Cdepth::IsELW(CString code)
{
	if (m_bLPMode)
		return true;

// 	if ((code.Find("J") != -1) && (code.GetLength()==9)) //신주인수권이라면
// 		m_percent	 = pcNo;
	if ((code.Find("J") != -1) && (code.GetLength()==7))
		return true;
	return false;
}

BOOL Cdepth::createFont()
{
	int	style = FW_NORMAL;
	bool	bItalic = false;

	if (m_style == FS_BOLD || m_style == FS_IBOLD)
		style = FW_BOLD;
	if (m_style == FS_ITALIC || m_style == FS_IBOLD)
		bItalic = true;

	//m_font = getFont(m_point, "굴림체", style, bItalic);
	m_font = getFont(m_point - 1, "Agency", style, bItalic);
	m_sfont = getFont(m_point - 2, "Agency FB", FW_NORMAL, false);
	m_sBfont = getFont(m_point - 1, "Agency FB", FW_BOLD, false);
	if (m_sfont == nullptr)
		m_sfont = m_font;
	
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
	struct	_penR	penR {};

	penR.clr   = color;
	penR.width = width;
	penR.style = PS_SOLID;
	return (CPen *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getPEN, 0), (LPARAM)&penR);
}

CBrush* Cdepth::getBrush(COLORREF clr)
{
	return (CBrush*)m_parent->SendMessage(WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* Cdepth::getFont(int point, CString name, int bold, bool italic)
{
	struct	_fontR	rFont {};

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
//	p				호가 전일대비 등락률
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
	CString	keys, text, tmps, strtemp;

	tmps = _T("/ ");
	keys = _T("dpbschlutoargmxwfyvz");

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
		case 't':
			//m_alwaysTEN = true;
			m_strOptionFiller = text;
			strtemp = m_strOptionFiller;
			if (Parser(strtemp, "|") == "showMIDtooltip")
				m_bShowMidtooltip = TRUE;

			m_sPrcTrigger_File = Parser(strtemp, "|");
			
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
		case 'm':
			m_bSiseMemo = true;
			break;
		case 'x':
			m_bBong	= true;
			break;
		case 'y':
			m_bMBPrice = false;
			break;
		case 'v':
			m_bMBVolume = false;
			break;
		case 'z':
			m_strNoDataBitmap = text;
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

void Cdepth::SendSiseMemo()
{
	const	int	nrec = m_sisememo.GetSize();

	if (nrec <= 0)
		return;

	if (m_code.IsEmpty())
		return;

	const	int	datl = sz_mid + sz_gridI * nrec;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl+1);

	FillMemory(datb.get(), datl, ' ');

	struct mid* pMid = (struct mid*)datb.get();
	CopyMemory(pMid->code, m_code, sizeof(pMid->code));
	sprintf(pMid->nrec, "%02d", nrec);

	for (int ii = 0; ii < nrec; ii++)
	{
		struct grid_i *pGrid = (struct grid_i*)&datb[sz_mid + sz_gridI * ii];
		std::shared_ptr<_sisememo> pMod = m_sisememo.GetAt(ii);
		
		sprintf(pGrid->indx, "%d", ii);
		sprintf(pGrid->gubn, "%d", pMod->gubn);
		switch (pMod->gubn)
		{
		case 1:
//			sprintf(pGrid->dat1, "%s", pMod->dat1);
//			sprintf(pGrid->dat2, "%08d", atoi(pMod->dat2));
//			break;
		case 2:
//			sprintf(pGrid->dat1, "%s", pMod->dat1);
//			sprintf(pGrid->dat2, "%08d", atoi(pMod->dat2));
//			break;
		case 3:
			sprintf(pGrid->dat1, "%s", (char *)pMod->dat1.GetString());
			sprintf(pGrid->dat2, "%08d", atoi(pMod->dat2));
			break;
		case 4:
			sprintf(pGrid->dat1, "%s", (char *)pMod->dat2.GetString());
			sprintf(pGrid->dat2, "%08d", atoi(pMod->dat1));
			break;
		}
	}

	SendTR("PIBO1011", datb.get(), datl, US_PASS, m_key);
	datb.reset();
}

void Cdepth::SendTR(CString name, CString data, BYTE type, int key)
{
	SendTR(name, (char*)data.GetString(), data.GetLength(), type, key);
}

void Cdepth::SendTR(CString name, char* datb, int datl, BYTE type, int key)
{
	int	idx = 0;
	std::unique_ptr<char[]> sndb = std::make_unique<char[]>(L_userTH + datl + 128);

	CopyMemory(&sndb[idx], (char*)m_szName.GetString(), m_szName.GetLength());
	idx += m_szName.GetLength();
	sndb[idx++] = '\t';

	struct	_userTH* udat = (struct _userTH*)&sndb[idx];
	CopyMemory(udat->trc, (char*)name.GetString(), name.GetLength());
	udat->key  = key;
	udat->stat = type;
	idx += L_userTH;

	CopyMemory(&sndb[idx], datb, datl);

	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)sndb.get());
	sndb.reset();
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

void Cdepth::InitAmtSet() 
{
	return;

	m_percent = (enum _percent)GetDataInt("UDRate", 0);
	if (m_percent == pcUser)
		m_nUDPercent = GetDataInt("UDRateVal", 0);
	setItemRect();
	calculatePercent();
	getMaxSize();

	freeSiseMemo();
	const	int	MCount = GetDataInt("MCount", 0);
	for (int ii = 0; ii < MCount; ii++)
	{
		CString	keys, dat = "", tmpS = "", val = "";
		keys.Format("m%d", ii);
		dat = GetDataString(keys, "");
		if (dat.IsEmpty())
			continue;

		int	pos = 0;
		std::shared_ptr<_sisememo> pMemo = std::make_shared<_sisememo>();

		pos = dat.Find('|');
		val = dat.Left(pos);
		pMemo->gubn = atoi(val) + 1;
		dat = dat.Mid(pos + 1);

		pos = dat.Find('|');
		val = dat.Left(pos);
		pMemo->color = atoi(val);
		dat = dat.Mid(pos + 1);

		pos = dat.Find('|');
		val = dat.Left(pos);
		
		int share = 0, remain = 0;
		if (pMemo->gubn == 1)
		{
			remain = atoi(val) % 5;
			remain++;
			share = atoi(val)  / 5;
			pMemo->dat1.Format("%d", remain);
		}
		else if (pMemo->gubn == 2 || pMemo->gubn == 3)
			pMemo->dat1.Format("%d", atoi(val) + 1);
		else
			pMemo->dat1 = val;
		dat = dat.Mid(pos + 1);

		pos = dat.Find('|');
		val = dat.Left(pos);
		if (pMemo->gubn == 1)// || pMemo->gubn == 4)
		{
			if (share == 1)
				pMemo->dat2 = "2";	
			else
				pMemo->dat2 = "1";
		}
		else if (pMemo->gubn == 4)
			pMemo->dat2.Format("%d", atoi(val) + 1);
		else
			pMemo->dat2 = val;

		pMemo->index = -1;

		m_sisememo.Add(pMemo);
	}
	Invalidate();
//	SendSiseMemo();
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
	std::shared_ptr<Citem> pItem = nullptr;

	pItem = m_items.GetAt(openPrice);
	CString siga = pItem->m_data;
	pItem = m_items.GetAt(highPrice);
	CString koga = pItem->m_data;
	pItem = m_items.GetAt(lowPrice);
	CString jega = pItem->m_data;
	CString jgga;
	jgga.Format("%.f", m_curr);
	addComma(jgga);
	CString ma5;
	ma5.Format("%.f", (m_ma5 + m_curr) / 5);
	addComma(ma5);
	CString ma10;
	ma10.Format("%.f", (m_ma10 + m_curr) / 10);
	addComma(ma10);

	str = _T("시가\t") + siga + _T("\n고가\t") + koga + _T("\n저가\t") + jega + _T("\n종가\t") + jgga
		+ _T("\nMA5\t") + ma5 + _T("\nMA10\t") + ma10 + _T("\n");	

	return str;
}

CString Cdepth::GetTooltipStr(int item)
{
	CString str = _T(""), tmpS = _T("");
	
	for (int ii = 0; ii < m_sisememo.GetSize(); ii++)
	{
		std::shared_ptr<_sisememo> pMemo = m_sisememo.GetAt(ii);
		if (pMemo->index == item)
		{
			switch (pMemo->gubn)
			{
			case 1:
				if (atoi(pMemo->dat2) == 1)
					str += _T("전일 ");
				else if (atoi(pMemo->dat2) == 2)
					str += _T("당일 ");
				
				if (atoi(pMemo->dat1) == 1)
					str += _T("시가\t");
				else if (atoi(pMemo->dat1) == 2)
					str += _T("고가\t");
				else if (atoi(pMemo->dat1) == 3)
					str += _T("저가\t");
				else if (atoi(pMemo->dat1) == 4)
					str += _T("종가\t");
				else if (atoi(pMemo->dat1) == 5)
					str += _T("가중평균가\t");

				str += pMemo->val + _T("\n");
				break;

			case 2:
				str += _T("전일부터 ") + pMemo->dat2 + _T(" 기간중 ");
				if (atoi(pMemo->dat1) == 1)
					str += _T("최고가\t");
				else if (atoi(pMemo->dat1) == 2)
					str += _T("최저가\t");

				str += pMemo->val + _T("\n");
				break;

			case 3:
				if (atoi(pMemo->dat1) == 1)
					str += _T("pivot 2차저항선\t");
				else if (atoi(pMemo->dat1) == 2)
					str += _T("pivot 1차저항선\t");
				else if (atoi(pMemo->dat1) == 3)
					str += _T("pivot 기준선\t");
				else if (atoi(pMemo->dat1) == 4)
					str += _T("pivot 1차지지선\t");
				else if (atoi(pMemo->dat1) == 5)
					str += _T("pivot 2차지지선\t");

				str += pMemo->val + _T("\n");
				break;

			case 4:
				str += pMemo->dat1 + _T("일 이동평균선 (당일");
				if (atoi(pMemo->dat2) == 1)
					str += _T("포함)\t");
				else if (atoi(pMemo->dat2) == 2)
					str += _T("미포함)\t");
				
				tmpS.Format("%.f", str2double(pMemo->val) / str2double(pMemo->dat1));
				addComma(tmpS);
				str += tmpS + _T("\n");
				break;
			}
		}
	}
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

void Cdepth::InitSiseMemoSet() 
{
	freeSiseMemo();

	const	int	MCount = GetDataInt("MCount", 0);
	for (int ii = 0; ii < MCount; ii++)
	{
		CString	keys, dat = "", tmpS = "", val = "";
		keys.Format("m%d", ii);
		dat = GetDataString(keys, "");
		if (dat.IsEmpty())
			continue;

		int	pos = 0;
		std::shared_ptr<_sisememo> pMemo = std::make_shared<_sisememo>();
		pos = dat.Find('|');
		val = dat.Left(pos);
		pMemo->gubn = atoi(val) + 1;
		dat = dat.Mid(pos + 1);

		pos = dat.Find('|');
		val = dat.Left(pos);
		pMemo->color = atoi(val);
		dat = dat.Mid(pos + 1);

		pos = dat.Find('|');
		val = dat.Left(pos);
		
		int share = 0, remain = 0;
		if (pMemo->gubn == 1)
		{
			remain = atoi(val) % 5;
			remain++;
			share = atoi(val)  / 5;
			pMemo->dat1.Format("%d", remain);
		}
		else if (pMemo->gubn == 2 || pMemo->gubn == 3)
			pMemo->dat1.Format("%d", atoi(val) + 1);
		else
			pMemo->dat1 = val;
		dat = dat.Mid(pos + 1);

		pos = dat.Find('|');
		val = dat.Left(pos);
		if (pMemo->gubn == 1)// || pMemo->gubn == 4)
		{
			if (share == 1)
				pMemo->dat2 = "2";	
			else
				pMemo->dat2 = "1";
		}
		else if (pMemo->gubn == 4)
			pMemo->dat2.Format("%d", atoi(val) + 1);
		else
			pMemo->dat2 = val;

		pMemo->index = -1;

		m_sisememo.Add(pMemo);
	}
	Invalidate();
	SendSiseMemo();
}

BOOL Cdepth::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if (m_rcConfig.PtInRect(point) || m_rcTime.PtInRect(point))
	{
		HCURSOR	hCursor = (HCURSOR)m_parent->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), 10);
		if (hCursor)
		{
			SetCursor(hCursor);
			return TRUE;
		}
//		TRACE("CURSOR-1\n");
	}else
	{
		
		HCURSOR	hCursor = (HCURSOR)m_parent->SendMessage(WM_USER, MAKEWPARAM(getCURSOR, 0), 0);
		if (hCursor)
		{
			SetCursor(hCursor);
			return TRUE;
			
		}
		
		//m_tracking = false;
	}
	//::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void Cdepth::drawButton(CDC *pDC)
{
	if (!m_rcTime.IsRectEmpty())
	{
		if (m_bPredict)
			drawButton(pDC, m_rcTime, m_pBitmapP2);
		else
			drawButton(pDC, m_rcTime, m_pBitmapP1);
	}

	if (m_bConfig)
	{
		if (!m_rcConfig.IsRectEmpty())
		{
			drawButton(pDC, m_rcConfig, m_pBitmapC1);
		}

		if (m_mkgubn == mkTOT)
		{
			if (!m_rcConfigL.IsRectEmpty())
			{
				drawButton(pDC, m_rcConfigL, m_pBitmapP1);
			}
			if (!m_rcConfigR.IsRectEmpty())
			{
				drawButton(pDC, m_rcConfigR, m_pBitmapP1);
			}
		}
		/*if (!m_rcMKPOPMenu.IsRectEmpty())
		{
			drawButton(pDC, m_rcMKPOPMenu, m_iDataType == DF_TOT ? m_pBitmapT1 : m_pBitmapS1);
		}*/
	}

	//int fontSize = m_rcMarket.Height() * 10 / 2;  // 높이의 절반 정도를 글자 크기로 사용
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));
	CFont* poldfont{};
	poldfont = pDC->SelectObject(m_font);
	if (m_mkgubn == mkTOT)
	{
		CRect rctmp;
		rctmp = m_rcMarket;
		rctmp.OffsetRect(1, 1);
		pDC->DrawText("통", rctmp, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (m_mkgubn == mkNXT)
		pDC->DrawText("N", m_rcMarket, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	else if (m_mkgubn == mkKRX)
		pDC->DrawText("K", m_rcMarket, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(poldfont);
}

void Cdepth::drawButton(CDC *pDC, CRect rc, CBitmap *pBitmap)
{
	if (pBitmap)
	{
		CDC	memDC;
		CBitmap	*pSBitmap = nullptr;
		BITMAP	stBitmapInfo{};

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
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	CPen*	pOldPen = pDC->SelectObject(getPen(m_clrBox, 1));

	GetClientRect(&rect);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldPen); pDC->SelectObject(pOldBrush);	
}

void Cdepth::ConfigDlg()
{
	CConfigDlg	dlg(&m_config);
	dlg.m_sRoot = Variant(homeCC, "");
	dlg.m_iVersion = m_iVersion;
	if (m_strOptionFiller.Find("DEFAULT_NOMID") >= 0)
		dlg.m_bEnableMid = FALSE;
	else
		dlg.m_bEnableMid = TRUE;
	if (dlg.DoModal())
	{
		SaveCondition(m_path, &m_config);
		EW_Show();
		InitEnv();
// 		if ((m_code.Find("J") != -1) && (m_code.GetLength()==9)) //신주인수권이라면
// 			m_percent	 = pcNo;
		Invalidate(); setItemRect();
	}
}

void Cdepth::ConfigDlgL()
{
	CMenu cMenu;
	CPoint	point;
	const	char* const pDisplay[3] = { "표시 안함", "그래프 표시", "체결 표시" };

	cMenu.CreatePopupMenu();
	for (int ii = 0; ii < 3; ii++)
	{
		if (m_config.mbong == ii)
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, ii+1, pDisplay[ii]);
		else
			cMenu.AppendMenu(MFT_STRING, ii+1, pDisplay[ii]);
	}

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
	//const	char	*pDisplay[4] = { "표시 안함", "시고저종 표시", "PIVOT 표시", "현재가 표시"};
	const	char* const pDisplay[5] = { "표시 안함", "시고저종 표시", "PIVOT 표시", "현재가 표시", "정적VI 표시"};
	for (int ii = 0; ii < 5; ii++)
	//for (int ii = 0; ii < 4; ii++)
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
	CString file; file.Format("%s\\%s", path, FILENAME);

	if (!m_configFile.IsEmpty())
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
	CString file; file.Format("%s\\%s", path, FILENAME);
	CString stemp{};

	if (!m_configFile.IsEmpty())
	{
		if (m_configFile.Find('.') >= 0)
			file.Format("%s\\%s", path, m_configFile);
		else
			file.Format("%s\\%s.env", path, m_configFile);
	}

	writeFile(file, (char*)pConfig, sz_CONFIG);

	CString strpath{};
	const	char* buff = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)0);
	const	char* name = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)0);
	strpath.Format("%s\\user\\%s\\%s.ini", buff, name, name);

	WritePrivateProfileString("VERSION", "cx_depth", (LPCSTR)DF_DEPTHVS_STR, (LPCSTR)strpath);
	m_iVersion = DF_DEPTHVS;

	stemp.Format("%d", m_iBottomType);
	WritePrivateProfileString("BottomType", "cx_depth", stemp, (LPCSTR)strpath);

	if (pConfig->bshowMidPrc == 0)
		m_iShowMIDPRC = 0;
	else
	{
		if (m_mkgubn == mkTOT)
			m_iShowMIDPRC = 2;
		else
			m_iShowMIDPRC = 1;
	}

	//SetMidShowValue(pConfig->bshowMidPrc == 1 ? true : false);
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

	//GetMidShowValue();

	return true;
}

bool Cdepth::writeFile(CString path, char *pBytes, int nBytes)
{
	CFile f; CFileException e;

	if (!f.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		TRACE("[WRITE] %s file open error(%d)\n", path, e.m_cause);
		return false;
	}

	f.Write(pBytes, nBytes); f.Close();
	return true;
}

void Cdepth::InitEnv()
{
	m_showOHLC = (enum _showInfo)m_config.info;

	m_bong = (enum _bong)m_config.mbong;

	m_bar = grNo;		if (m_config.sgrp)  m_bar	= grBarText;
	m_percent = pcNo;	if (m_config.hdyul) m_percent	= pcJgga;
	m_bCurr = false;	if (m_config.curr)  m_bCurr	= true;
	m_bBoldPrice = false;	if (m_config.hbold) m_bBoldPrice= true;
	m_bPredict = false;	if (m_config.pre)   m_bPredict	= true;
	m_bBong = false;	if (m_config.sbong) m_bBong	= true;
	m_bBoldRemain = false; if (m_config.hrbold) m_bBoldRemain = true;
	m_bBoldCnt = false; if (m_config.hcbold) m_bBoldCnt = true;
	//m_iShowMIDPRC = 0; if (m_config.bshowMidPrc) m_iShowMIDPRC = 1;

	CString path{};
	const	char* buff = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)0);
	const	char* name = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)0);
	path.Format("%s\\user\\%s\\%s.ini", buff, name, name);

	m_slog.Format("pcx_depth][%s]<%d>  화면 오픈시 설정로드 m_bPredict = [%d] m_iBottomType=[%d]", __FUNCTION__, __LINE__, m_bPredict, m_iBottomType);
	Output_DebugString(m_slog);

	//GetMidShowValue();

	m_iVersion = GetPrivateProfileInt("VERSION", "cx_depth", 0, path);

	if (m_iVersion < DF_DEPTHVS)
	{
		m_iShowMIDPRC = 1;
		m_config.bshowMidPrc = 1;
		WritePrivateProfileString("VERSION", "cx_depth", (LPCSTR)DF_DEPTHVS_STR, (LPCSTR)path);
	}

	if(m_strOptionFiller.Find("DEFAULT_NOMID") >= 0)
		m_iShowMIDPRC = 0;

	m_slog.Format("[cx_depth]  m_iVersion=[%d] m_strOptionFiller =[%s]", m_iVersion, m_strOptionFiller);
	Output_DebugString(m_slog);

	calculateRowHeight();  //mod nxt
	calculateColumnWidth();
	EW_Move();

	setItemRect(); calculatePercent();
}

short Cdepth::_GetInfo()
{
	return (short)m_config.info;
}

void Cdepth::_SetInfo(short nNewValue) 
{
	switch (nNewValue)
	{
	case 0:
		m_showOHLC = infoNo;	break;
	case 1:
		m_showOHLC = infoSiga;	break;
	case 2:
		m_showOHLC = infoPivot;	break;
	case 3:
		m_showOHLC = infoCurr;	break;
	case 4:
		m_showOHLC = infoVI;	break;
	}

	m_config.info = m_showOHLC;
	
	SaveCondition(m_path, &m_config);
	
	Invalidate();
}

void Cdepth::clickAction(CRect rect, CPoint point)
{
	CRect	itemRC;
	CString	entry;
	int	line = 0;

	if (m_showOHLC == infoSiga)
	{
		if (m_depth == 10)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 10; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				itemRC.OffsetRect(0, 4);
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry = m_items.GetAt(openPrice)->m_data;	break;
				case 1: entry = m_items.GetAt(highPrice)->m_data;	break;
				case 2: entry = m_items.GetAt(lowPrice)->m_data;	break;
				//case 3: entry = m_items.GetAt(pivotPrice)->m_data;	break;
				case 3: entry = m_items.GetAt(shPrice)->m_data;		break;
				case 4: entry = m_items.GetAt(hhPrice)->m_data;		break;

				case 5: entry = m_items.GetAt(max52)->m_data;		break;
				case 6: entry = m_items.GetAt(min52)->m_data;		break;
				case 7: entry = m_items.GetAt(gajungPrice)->m_data;	break;
				case 8: entry = m_items.GetAt(pivotPrice)->m_data;	break;
				default:						break;
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
		else if (m_depth == 5)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 5; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_OneMoreRowH*line), rect.right, (int)(m_OneMoreRowH*(line+1)));
				itemRC.OffsetRect(0, 4);
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry = m_items.GetAt(openPrice)->m_data;	break;
				case 1: entry = m_items.GetAt(highPrice)->m_data;	break;
				case 2: entry = m_items.GetAt(lowPrice)->m_data;	break;
//				case 3: entry = m_items.GetAt(pivotPrice)->m_data;	break;
				case 3: entry = m_items.GetAt(shPrice)->m_data;		break;
				case 4: entry = m_items.GetAt(hhPrice)->m_data;		break;
				default:						break;
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
		else if (m_depth == 3)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 3; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry = m_items.GetAt(openPrice)->m_data;	break;
				case 1: entry = m_items.GetAt(highPrice)->m_data;	break;
				case 2: entry = m_items.GetAt(lowPrice)->m_data;	break;
				default:						break;
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
	else if (m_showOHLC == infoPivot)
	{
		if (m_depth == 10)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 10; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 1: entry = m_items.GetAt(resist2)->m_data;		break;
				case 2: entry = m_items.GetAt(resist1)->m_data;		break;
				case 3: entry = m_items.GetAt(pivotV)->m_data;		break;
				case 4: entry = m_items.GetAt(support1)->m_data;	break;
				case 5: entry = m_items.GetAt(support2)->m_data;	break;
				case 7: entry = m_items.GetAt(resistD)->m_data;		break;
				case 8: entry = m_items.GetAt(supportD)->m_data;	break;
				default:						break;
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
		else if (m_depth == 5)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 5; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry = m_items.GetAt(resist2)->m_data;		break;
				case 1: entry = m_items.GetAt(resist1)->m_data;		break;
				case 2: entry = m_items.GetAt(pivotPrice)->m_data;	break;
				case 3: entry = m_items.GetAt(support1)->m_data;	break;
				case 4: entry = m_items.GetAt(support2)->m_data;	break;
				default:						break;
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
		else if (m_depth == 3)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 3; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry = m_items.GetAt(resist1)->m_data;		break;
				case 1: entry = m_items.GetAt(pivotPrice)->m_data;	break;
				case 2: entry = m_items.GetAt(support1)->m_data;	break;
				default:						break;
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
	else if (m_showOHLC == infoCurr)
	{
		if (m_depth == 10)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 10; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry = m_items.GetAt(openPrice)->m_data;	break;
				case 1: entry = m_items.GetAt(highPrice)->m_data;	break;
				case 2: entry = m_items.GetAt(lowPrice)->m_data;	break;
				case 3: entry = m_items.GetAt(shPrice)->m_data;		break;
				case 4: entry = m_items.GetAt(hhPrice)->m_data;		break;
				case 5: entry = m_items.GetAt(pivotPrice)->m_data;	break;
				case 6: entry.Format("%.f", m_curr);			break;
				default:						break;
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
		else if (m_depth == 5)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 5; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 1: entry = m_items.GetAt(cloPrice)->m_data;	break;
				default:						break;
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
		else if (m_depth == 3)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 3; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 0: entry.Format("%.f", m_curr);	break;
				default:				break;
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
	else if (m_showOHLC == infoVI)
	{
		if (m_depth == 10)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 10; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;
				
				////////////
				switch (ii)
				{
				case 0: entry = m_items.GetAt(openPrice)->m_data;	break;
				case 1: entry = m_items.GetAt(highPrice)->m_data;	break;
				case 2: entry = m_items.GetAt(lowPrice)->m_data;	break;
				case 3: entry = m_items.GetAt(shPrice)->m_data;		break;
				case 4: entry = m_items.GetAt(hhPrice)->m_data;		break;
				case 6: entry = m_items.GetAt(SVIUP)->m_data;		break;
				case 7: entry = m_items.GetAt(SVIDN)->m_data;		break;
				default:						break;
				}
				//////////////

				/*
				switch (ii)
				{
				case 1: entry = m_items.GetAt(SVIUP)->m_data;	break;
				case 2: entry = m_items.GetAt(SVIDN)->m_data;	break;
				default:					break;
				}
				*/

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
		else if (m_depth == 5)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 5; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;
				
				switch (ii)
				{
				case 1: entry = m_items.GetAt(SVIUP)->m_data;	break;
				case 2: entry = m_items.GetAt(SVIDN)->m_data;	break;
				default:					break;
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
		else if (m_depth == 3)
		{
			entry = _T(""); line = m_showTop ? 1 : 0;
			for (int ii = 0; ii < 3; ii++, line++)
			{
				itemRC.SetRect(rect.left, (int)(m_rowH*line), rect.right, (int)(m_rowH*(line+1)));
				if (!itemRC.PtInRect(point))
					continue;

				switch (ii)
				{
				case 1: entry = m_items.GetAt(SVIUP)->m_data;	break;
				case 2: entry = m_items.GetAt(SVIDN)->m_data;	break;
				default:					break;
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
}

///////////////////////////////////////////////////
// Extra Window
void Cdepth::EW_Init()
{
	m_pContract = nullptr;
	m_pMBong    = nullptr;
	m_pRgn      = nullptr;

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
		m_pContract->Font(m_font);
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
		m_pMBong->Font(m_font);
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

	 //modi NXT 좌측하단 봉영역 관련된듯하다
	if (m_iShowMIDPRC)
		rc.SetRect(0, (int)(m_topH + (m_rowH * m_depth) + (m_rowH * m_iShowMIDPRC)), m_columns[0], (int)(m_topH + (m_rowH * m_depth * 2) + m_rowH));
	//rc.SetRect(0, (int)(m_topH + (m_rowH * m_depth) + m_rowH), m_columns[0], (int)(m_topH + (m_rowH * m_depth * 2) + m_rowH));
	else
		rc.SetRect(0, (int)(m_topH + m_rowH * m_depth), m_columns[0], (int)(m_topH + m_rowH * m_depth * 2));

	if (m_pMBong)
		m_pMBong->MoveWindow(rc);
	if (m_pContract)
		m_pContract->MoveWindow(rc);

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

	if (!m_bIsNXTAble)
	{
		if (m_pMBong)		m_pMBong->ShowWindow(SW_HIDE);
		if (m_pContract)	m_pContract->ShowWindow(SW_HIDE);
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

void CopyTickBongData(const struct _hoga* hoga, struct _tickBongData* out)
{
	if (!hoga || !out) return;

	// ntick, nbong 복사
	memcpy(out->ntick, hoga->ntick, sizeof(hoga->ntick));
	memcpy(out->nbong, hoga->nbong, sizeof(hoga->nbong));

	// ctick[15] 복사
	memcpy(out->ctick, hoga->ctick, sizeof(hoga->ctick));

	// dBong[30] 복사
	memcpy(out->dBong, hoga->dBong, sizeof(hoga->dBong));
}

void Cdepth::EW_Dispatch(char *dataB, bool bTEN)
{
	if (bTEN)
	{
		const	struct	_hoga*	hoga = (_hoga*)dataB;
		_tickBongData tickBongData = {};
		CopyTickBongData(hoga, &tickBongData); 

		if (m_pContract)
			m_pContract->Dispatch((char*)&tickBongData, 0);
		if (m_pMBong)
			m_pMBong->Dispatch((char*)&tickBongData,
				CString(hoga->shga, sizeof(hoga->shga)), CString(hoga->hhga, sizeof(hoga->hhga)), 0);
	}
	else
	{
		const	struct	_hogax*	hoga = (_hogax*)dataB;

		if (m_pContract)
			m_pContract->Dispatch(&dataB[sz_hogax], 0);
		if (m_pMBong)
			m_pMBong->Dispatch(&dataB[sz_hogax], 
				CString(hoga->shga, sizeof(hoga->shga)), CString(hoga->hhga, sizeof(hoga->hhga)), 0);
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



int Cdepth::hogaTypeToInt(CString hog)
{
	if (hog.IsEmpty())
		return -1;

	for (int kk = openPrice; kk <= pivotPrice/*gajungPrice*/;kk++)
	{
		if (kk == gajungPrice)
			return -1;

		if (hog == m_items.GetAt(kk)->m_data)
			return kk;
	}

	for (int jj = SVIUP; jj <= SVIDN ;jj++)
		if (hog == m_items.GetAt(jj)->m_data)
			return jj;

	for (int jj = MIDKPRCMD; jj <= MIDKPRCMS; jj++)
		if (hog == m_items.GetAt(jj)->m_data)
			return jj;
	
	return -1;
}

CString Cdepth::GetHogaTip(CString hog)
{
	if (hog.IsEmpty())
		return "";

	int	val = -1;
	CString tip_str;

	for (int kk = openPrice; kk <= pivotPrice/*gajungPrice*/;kk++)
	{
		if (kk == gajungPrice)
		{
			CString str_gj, tmp, simil_price;

			hog.Remove(',');
			str_gj = m_items.GetAt(kk)->m_data;
			if (str_gj.IsEmpty())
				return "";
			str_gj.Trim();
			str_gj.Remove(',');

			const	int	int_gj = atoi(str_gj);
			int	diff = 0;	
			int	pre_diff = 999999999;

			for (int ii = 1; ii <= 50; ii++)
			{
				if (ii==11)
					ii+=30;

				tmp = m_items.GetAt(ii)->m_data;	
				tmp.Remove(',');
				if (tmp.IsEmpty())
					continue;
				diff = abs(atoi(tmp)-int_gj);
				if (pre_diff > diff)
				{
					pre_diff = diff;
					simil_price = tmp;
				}	
			}
			
			if (hog == simil_price)
			{	
				val = gajungPrice;
				tip_str = AddTipStr(val, tip_str, "");
			}
		}
		else
		{
			if (hog == m_items.GetAt(kk)->m_data)
			{	
				val = kk;
				tip_str = AddTipStr(val, tip_str, "");
			}
		}
	}

	for (int jj = SVIUP; jj <= SVIDN; jj++)
	{
		if (hog == m_items.GetAt(jj)->m_data)
		{	
			val = jj;
			tip_str = AddTipStr(val, tip_str, "");
		}
	}

 	return tip_str;
}

CString Cdepth::AddTipStr(int tp, CString str1, CString str2)
{
	CString string;

	switch (tp)
	{
	case openPrice:
		string.Format("시가 \t: %s", m_items.GetAt(tp)->m_data);
		break;
	case highPrice:
		string.Format("고가 \t: %s", m_items.GetAt(tp)->m_data);
		break;
	case lowPrice:
		string.Format("저가 \t: %s", m_items.GetAt(tp)->m_data);
		break;
	case pivotPrice:
		string.Format("기준가 \t: %s", m_items.GetAt(tp)->m_data);
		break;
	case SVIUP:
		string.Format("VI상승가\t: %s", m_items.GetAt(tp)->m_data);
		break;
	case SVIDN:
		string.Format("VI하락가\t: %s", m_items.GetAt(tp)->m_data);
		break;
	//case shPrice:
	//	string.Format("상한가 \t: %s", m_items.GetAt(tp)->m_data);
	//	break;
	//case hhPrice:
	//	string.Format("하한가 \t: %s", m_items.GetAt(tp)->m_data);
	//	break;
	/*	
	case gajungPrice:
		string.Format("가중평균가 \t: %s", m_items.GetAt(tp)->m_data);
		break;
	*/	
	default:
		break;
	}
	if (str1 != "")
		string.Insert(0, str1 + "\n");

	return string;
}

CString Cdepth::calculatePercentByIndex(int idx)
{
	if (m_clos == 0)
		return "";

	if (idx + 10 > m_items.GetCount())
		return "";

	CString ret;
	double	price = 0, percent = 0;
	std::shared_ptr<Citem> item = nullptr;

	price = str2double(m_items.GetAt(idx)->m_data);
	if (price > 0)
	{
		item = m_items.GetAt(idx+10);
		if (price > m_clos)
		{
			percent = (price - m_clos)/ m_clos * 100;
			ret.Format("%2.2f", percent);
		}
		else if (price < m_clos)
		{
			percent = (m_clos - price)/ m_clos * 100;
			ret.Format("-%2.2f", percent);
		}
		else
		{
			ret = _T("0.00");
		}
	}
	return ret;
}

BOOL Cdepth::GetBoldRemain() 
{
	return m_bBoldRemain ? TRUE : FALSE;
}

void Cdepth::SetBoldRemain(BOOL bNewValue) 
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// COleEditDropTarget

CDropTarget::CDropTarget()
{
	m_pDepth = nullptr;
}

CDropTarget::~CDropTarget() {}  

//
// OnDragEnter is called by OLE dll's when drag cursor enters
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{   
	// Check if the control key was pressed          
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;
	
	// if the control key is held down, return a drop effect COPY 
	if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_COPY;
	// Otherwise return a drop effect of MOVE
	return DROPEFFECT_MOVE;   
} 

//
// OnDragLeave is called by OLE dll's when drag cursor leaves
// a window that is REGISTERed with the OLE dll's
//
void CDropTarget::OnDragLeave(CWnd* pWnd)
{
	// Call base class implementation
	COleDropTarget::OnDragLeave(pWnd);
}

// 
// OnDragOver is called by OLE dll's when cursor is dragged over 
// a window that is REGISTERed with the OLE dll's
//
DROPEFFECT CDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if (!pDataObject->IsDataAvailable(CF_TEXT))
		return DROPEFFECT_NONE;

	if ((dwKeyState&MK_CONTROL) == MK_CONTROL)
		return DROPEFFECT_NONE;  

	return DROPEFFECT_MOVE;  // move source
}
	
// 
// OnDrop is called by OLE dll's when item is dropped in a window
// that is REGISTERed with the OLE dll's
//
BOOL CDropTarget::OnDrop(CWnd* pWnd, COleDataObject * pDataObject, DROPEFFECT dropEffect, CPoint point )
{         
	HGLOBAL  hGlobal = NULL;
	LPCSTR   szBuffer = NULL; 
	
	// Get text data from COleDataObject
	if (dropEffect == DROPEFFECT_NONE)
		return FALSE;	
	
	if (pDataObject->GetGlobalData(CF_TEXT))
	{
		// Get pointer to data
		hGlobal = pDataObject->GetGlobalData(CF_TEXT);

		szBuffer = (LPCSTR)GlobalLock(hGlobal);
		ASSERT(szBuffer != NULL);

		GlobalUnlock(hGlobal);

	}

	CString strText = szBuffer;
	CString str1301, strCode;

	strCode.Empty();str1301.Empty();
	str1301 = Parser(strText,"\t");
	if (str1301.Find("1301") != -1)
	{
		strCode = Parser(strText, "\t");
	}
	else
	{
		for (int ii=0 ; ii<10 ; ii++)
		{
			str1301 = Parser(strText, "\t");
			str1301.TrimRight();
				
			if (strText.GetLength() <=0)
				break;

			if (str1301.Find("1301") != -1)
			{
				strCode = Parser(strText, "\t");
				break;
			}
				
		}
	}

//	delete szBuffer;

	strCode.TrimRight();

	CString sendCode;
	if (!strCode.IsEmpty() && str1301.Find("1301") >= 0)
	{
		sendCode.Format("1301\t%s", strCode);
		m_pDepth->trigger(sendCode);
		//맵의 1301에 종목코드 넣고 함수 실행
		return TRUE;
	}

	return FALSE;
}

CString CDropTarget::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CDropTarget::GetDepth(Cdepth* cWnd)
{
	m_pDepth = cWnd;
}
/////////////////////////////////////////////////////////////////////////////
// 

BOOL Cdepth::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	if (pMsg->message == WM_RBUTTONDOWN)
// 	{
// 		TRACE("LEFT MOUSE CLICK");
// 	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

void Cdepth::OnRButtonDown(UINT nFlags, CPoint point) 
{
//	TRACE("LEFT MOUSE CLICK");
	
	CWnd::OnRButtonDown(nFlags, point);
}

void Cdepth::SetLPMode(BOOL flag) 
{
	m_bLPMode = flag;
}

void Cdepth::SetPriceRange(long high, long low) 
{
	// TODO: Add your dispatch handler code here
	m_bDragMode = TRUE;
	m_nPriceH = high;
	m_nPriceL = low;

	if (m_bDragMode)
	{
		if (m_nPriceH >= m_curr && m_curr >= m_nPriceL)
		{
			m_clrWhite = RGB(255,255,150);
		}
		else
		{
			m_clrWhite = RGB(255,255,255);
		}
	}

	Invalidate();
}

void Cdepth::SetPercent(short high, short low) 
{
	// TODO: Add your dispatch handler code here
	CString str;
// 	str.Format("SET [%d][%d][%lf][%lf]",high,low,m_clos,m_curr);
// 	AfxMessageBox(str);
	if (high == 0 && low == 0)
	{
		m_bPercent = FALSE;
		m_bDragMode = FALSE;

		m_clrWhite = RGB(255,255,255);

		Invalidate();
		return;
	}
	else
	{
		m_bDragMode = FALSE;
		m_bPercent = TRUE;
		m_nHighPercent = high;
		m_nLowPercent = low;
	}

	m_nPriceH = m_clos + (m_clos * abs(high)) / 100;
	m_nPriceL = m_clos + (m_clos * abs(low)) / 100;
	if (m_bPercent)
	{
		if (m_nPriceH < m_curr || m_nPriceL > m_curr)
		{
			m_clrWhite = RGB(255,255,150);
		}
		else
		{
			m_clrWhite = RGB(255,255,255);
		}
	}

	Invalidate();
}


void Cdepth::SearchChegang()
{
	CString strText;

	//strText.Format("1301%c%s\t3149\t3147\t3146\t3148\t3150\t3181\t", 0x7f, m_code, );
	strText.Format("1301%c%s\t1777%c%d\t3149\t3147\t3146\t3148\t3150\t3181\t", 0x7f, m_code, 0x7f, m_mkgubn);
	if (m_mkgubn != mkTOT)
		SendTR("POOPPOOP", strText, US_OOP, m_key);
}

CString Cdepth::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}


void Cdepth::SetMKtype(SHORT mkgubn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_iShowMIDPRC = 1;

	if (mkgubn == 1)
	{
		m_mkgubn = mkKRX;
	//	m_iDataType = DF_SEP;  //test 우선주석
	}
	else if (mkgubn == 2)
	{
		m_mkgubn = mkNXT;
		//m_iDataType = DF_SEP;  //test 우선주석
	}
	else if (mkgubn == 3)
	{
		m_mkgubn = mkTOT;
		m_iShowMIDPRC = 2;
		//	m_iDataType = DF_TOT;
	}
	
	if (m_config.bshowMidPrc == 0)
	{
		m_iShowMIDPRC = 0;
	}

	m_slog.Format("[cx_depth][%s]<%d> m_mkgubn=[%d] m_iDataType= [%d]", __FUNCTION__, __LINE__, m_mkgubn, m_iDataType);
	OutputDebugString(m_slog);
	
	//m_slog.Format("%s\t%d", "depth_type", m_iDataType);
	//m_parent->SendMessage(WM_USER, MAKEWPARAM(formDLL, m_key), (LPARAM)m_slog.GetString());

	setIndices();
	getMaxSize();

	InitEnv();
	Invalidate();
}


void Cdepth::SetPrcEdit(BSTR sPrcEdit)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	CString sdata;
	sdata.Format("%s", sPrcEdit);
	sdata.MakeUpper();

	if(sdata.Find("JPRC") >= 0)
		m_iPrcType = TYPE_JPRC;
	else if(sdata.Find("STOP") >= 0)
		m_iPrcType = TYPE_STOPPRC;
}


void Cdepth::InsertObjData(BSTR sdata)
{//안써야 난다
	return;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sVal, stmp, scode, stemp;
	sVal.Format("%s", sdata);

	if (sVal.Find("TYPE_MK_TIME") >= 0)
	{
		stemp = Parser(sVal, "\t");
		m_mkTime = (_mkTimetype)atoi(sVal);
		switch (atoi(sVal))
		{
			case 6: m_bPredict = true; break;
			default:
			{
				m_bPredict = false;
			}
			break;
		}
		Invalidate();
	}
	else if (sVal.Find("TYPE_HOGB") >= 0)
	{
		stemp = Parser(sVal, "\t");
		int igubn = atoi(sVal);
		switch (igubn)
		{
			case 81:
			{
				m_iBottomType = 1;
				Invalidate();
				return;
			}
		}
	}
	else
	{
		stmp = Parser(sVal, "\n");
		stemp = Parser(stmp, "\t");
		scode = stmp;
		m_mapCodeData.SetAt(scode, sVal);  //KRX A**** , NXT N.A***

		m_slog.Format("[TOT][ALERT] scode=[%s] sVal=[%s]", scode, sVal);
		Output_DebugString(m_slog);

		CRect rec, tmprec;
		GetClientRect(rec);
		tmprec.left = rec.right - rec.Width() / 3;
		tmprec.bottom = rec.top + rec.Height() / 2;
		tmprec.right = rec.right;
		tmprec.top = rec.top;

		//통합 > 개별    영역 다시 그리기
		if (m_mkgubn == mkTOT && m_iDataType == DF_SEP)
		{
			CRect rctmp;
			std::shared_ptr<Citem> item = nullptr;
			item = m_items.GetAt(askBefore10);
			rctmp.left = item->m_fRc.left;
			rctmp.top = item->m_fRc.top;
			item = m_items.GetAt(askSize1);
			rctmp.right = item->m_fRc.right;
			rctmp.bottom = item->m_fRc.bottom;
			InvalidateRect(rctmp);

			item = m_items.GetAt(bidSize1);
			rctmp.left = item->m_fRc.left;
			rctmp.top = item->m_fRc.top;
			item = m_items.GetAt(bidBefore10);
			rctmp.right = item->m_fRc.right;
			rctmp.bottom = item->m_fRc.bottom;
			InvalidateRect(rctmp);
		}

		if (m_mkgubn == mkTOT)
		{
			//KRX, NXT 중간가 영역
			CRect rctmp;
			std::shared_ptr<Citem> item = nullptr;
			item = m_items.GetAt(MIDKPRCDBMD);
			rctmp.left = item->m_fRc.left;
			rctmp.top = item->m_fRc.top;
			item = m_items.GetAt(MIDNPRCDBMS);
			rctmp.right = item->m_fRc.right;
			rctmp.bottom = item->m_fRc.bottom;
			InvalidateRect(rctmp);
		}
	}
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

CString  Cdepth::getMKRemainValue(CString scode, CString ssim)
{
	return "";

	CString stmp, sval, stemp;
	if(!m_mapCodeData.Lookup(scode, sval))
		return "";

	while (sval.GetLength() > 0)
	{
		stmp = Parser(sval, "\n");
		if (stmp.Find(ssim) >= 0)
		{
			stemp = Parser(stmp, "\t");
			stmp.TrimLeft();
			stmp.TrimRight();
			//stmp.Replace("+", "");
			//stmp.Replace("-", "");
			//addComma(stmp);
			if (stmp == "0")
				stmp.Empty();
			return stmp;
		}
	}
	return "";
}

void Cdepth::GetMidShowValue()
{
	return;
	CString stmp;
	char	wb[32];
	GetPrivateProfileString("MIDKPRICE", "bSHOW", "", wb, sizeof(wb), m_sUserConfFile);
	stmp.Format("%s", wb);
	stmp.TrimRight();

	if (stmp == "0" || m_strOptionFiller.Find("DEFAULT_NOMID") >= 0)
	{
		m_iShowMIDPRC = 0;
		m_config.bshowMidPrc = 0;
	}
	else
	{
		m_iShowMIDPRC = 2;  //testcode
		m_config.bshowMidPrc = 1;
	}
}

void Cdepth::SetMidShowValue(bool bshow)
{
	if (m_strOptionFiller.Find("DEFAULT_NOMID") < 0)
	{
		CString stmp;
		stmp.Format("%d", m_iShowMIDPRC);
		WritePrivateProfileString("MIDKPRICE", "bSHOW", stmp, (LPCSTR)m_sUserConfFile);
		m_config.bshowMidPrc = bshow ? 1 : 0;
	}
}

CString Cdepth::GetBottomDataType(CString fun, int line)
{
	CString sVal;
	if (m_mkgubn == mkKRX)  //예상체결 <-> 시간외
	{
		sVal = m_bPredict ? "예상체결" : "시간외";
	}
	else if (m_mkgubn == mkNXT)   //현재가(K) <-> 예상체결(N)
	{
		sVal = m_bPredict ? "예상체결" : "시간외";
		//switch (m_iBottomType)   //0:예상체결 , 1:시간외, 2:현재가(K)   --> m_bPredict 대체
		//{
		//case 0:
		//	sVal = "예상체결(N)";
		//	break;
		//case 1:
		//	sVal = "시간외";
		//	break;
		//case 2:
		//	sVal = "현재가(K)";
		//	break;
		//default:
		//	sVal = "예상체결(N)";
		//	break;
		//}
	}
	else if (m_mkgubn == mkTOT)  //
	{
		sVal = m_bPredict ? "예상체결" : "시간외";
	}


	//m_slog.Format("[cx_depth][%s]<%d> called=[%d][%s]", __FUNCTION__, __LINE__, line, fun);
	//OutputDebugString(m_slog);

	//m_slog.Format("[cx_depth][%s]<%d> m_iBottomType =[%d] sval=[%s]", __FUNCTION__, __LINE__, m_iBottomType, sVal);
	//OutputDebugString(m_slog);


	return sVal;
}

void Cdepth::DrawItemData(int index, CString sval)
{
	std::shared_ptr<Citem> item = nullptr;
	item = m_items.GetAt(index);

	item->m_data = sval;

	CDC* pDC = GetDC();

	if (pDC)
	{
		UINT	style = 0;
		style = DT_SINGLELINE | DT_VCENTER;

		if (item->m_center)
			style |= DT_CENTER;
		else
			style |= DT_RIGHT;

		pDC->DrawText(sval, item->m_fRc, style);


		ReleaseDC(pDC);
	}

}

BSTR Cdepth::GetsDepthInfo()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%d", m_iDataType);
	return strResult.AllocSysString();
}


void Cdepth::SetsDepthInfo(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sVal;
	sVal.Format("%s", newVal);

	m_iDataType = atoi(sVal);

	Invalidate();
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}
