/************************************************************************/
/* FILENAME:    AxMBarCtrl.cpp                                          */
/* DESCRIPTION: CAxMBarCtrl class Implementation file.                  */
/* WRITTEN:     macho@2006.11.06                                        */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "AxMCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AXMULTIBAR_CLASS			"AxMultiBarCtrlClass"
#define DEFAULT_BUTTON_HEIGHT		17

#define IDC_CONTROL_BASE			1001
/////////////////////////////////////////////////////////////////////////////
// CAxMBarCtrl

CAxMBarCtrl::CAxMBarCtrl()
{
	m_tab = NULL;
	m_idBase = IDC_CONTROL_BASE;
	m_buttonHeight = DEFAULT_BUTTON_HEIGHT;
	XRegisterWindowClass(AXMULTIBAR_CLASS);

	m_font.CreatePointFont(90, "±¼¸²");

	m_buttonShadeID = CAxShadeButton::SHS_SOFTBUMP;
	m_buttonColor = clSkyBlue;
	m_buttonColorPercent = 70;
	
	m_messageHandler = NULL;
	m_currBtn = NULL;	
	m_imageFolder.Empty();
	m_paletteName.Empty();
}

CAxMBarCtrl::~CAxMBarCtrl()
{
}


BEGIN_MESSAGE_MAP(CAxMBarCtrl, CWnd)
	//{{AFX_MSG_MAP(CAxMBarCtrl)
	ON_MESSAGE(AXM_SETMSGHANDLER, OnSetMessageHandler)
	ON_MESSAGE(AXM_GETMSGHANDLER, OnGetMessageHandler)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_CONTROL_BASE, OnSelchangeTab)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxMBarCtrl message handlers

int CAxMBarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	InitializeControls();
	
	return 0;
}

void CAxMBarCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
	
	ReleaseControls();
	
}

void CAxMBarCtrl::SetImageFolder(LPCTSTR folder)
{
	m_imageFolder = folder;
}

void CAxMBarCtrl::InitializeControls()
{
	CAxMTabCtrl::EnableCustomLook(TRUE, ETC_SELECTION | ETC_FLAT | ETC_COLOR | ETC_GRADIENT | ETC_BACKTABS);

	m_tab = new CAxMTabCtrl(this);
	m_tab->Create(WS_VISIBLE | WS_CHILD | TCS_OWNERDRAWFIXED, CRect(0, 0, 10, 10), this, GetAutoID());
	m_tab->SetItemSize(CSize(100, 16));
	TabCtrl_SetPadding(m_tab->m_hWnd, 0, 0);
}

void CAxMBarCtrl::ReleaseControls()
{
	PCONTROLLIST pl;
	while (!m_cons.IsEmpty()) 
	{
		pl = m_cons.RemoveHead();
		DeleteAllControls(&pl->list);

		delete pl;
	}

	SAFE_DELETE_WND(m_tab);
}

CWnd* CAxMBarCtrl::Create(const CRect &rect, UINT id, CWnd *parent, DWORD style)
{
	CWnd::Create(AXMULTIBAR_CLASS, "", style, rect, parent, id);
	return this;
}

void CAxMBarCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	ResizeControls(cx, cy);	
}

void CAxMBarCtrl::ResizeControls()
{
	CRect rc;

	GetClientRect(&rc);
	ResizeControls(rc.Width(), rc.Height());
}

const int sideMargin = 3;
const int bottomMargin = 2;

void CAxMBarCtrl::ResizeControls(int cx, int cy)
{
	if (!m_tab) return;

	m_tab->SetWindowPos(NULL, 0, 0, cx, cy, SWP_SHOWWINDOW);

	PCONTROLLIST pl;
	POSITION pos;
	PAXCONTROL pxb;

	for (int i = 0; i < m_tab->GetItemCount(); i++) 
	{
		if (i == GetTabIndex())
			continue;

		pos = m_cons.FindIndex(i);
		if (pos) 
		{
			pl = m_cons.GetAt(pos);

			for (POSITION posButton = pl->list.GetHeadPosition(); posButton; ) 
			{
				pxb = pl->list.GetNext(posButton);

				if (pxb->btn)
					pxb->btn->ShowWindow(SW_HIDE);
				if (pxb->cc) 
					pxb->cc->ShowWindow(SW_HIDE);
			}
		}
	}

	pl = GetControlList();
	if (!pl) return;

	switch (pl->conType) 
	{
		case MBAR_BAR: 
			ResizeControl_BAR(pl->list, cx, cy);
			break;
		case MBAR_DLG:
			ResizeControl_DLG(pl->list, cx, cy);
			break;
	}
}

void CAxMBarCtrl::ResizeControl_BAR(CControlList& list, int cx, int cy)
{
	int top = GetTopPixel(), count = 1, bottom;
	PAXCONTROL pxb;

	for (POSITION pos = list.GetHeadPosition(); pos; count++) 
	{
		pxb = list.GetNext(pos);

		ASSERT(pxb->btn);
		pxb->btn->SetWindowPos(NULL, sideMargin, top, cx-sideMargin*2, m_buttonHeight, SWP_SHOWWINDOW);
		if (m_currBtn == pxb->btn) 
		{
			bottom = cy - (list.GetCount() - count) * m_buttonHeight - bottomMargin;
			if (pxb->cc) 
			{
				pxb->cc->SetWindowPos(NULL, sideMargin, top + m_buttonHeight, 
					cx - sideMargin * 2, bottom - (top + m_buttonHeight), SWP_SHOWWINDOW | SWP_DRAWFRAME);
			}
			top = bottom;
		}
		else 
		{
			if (pxb->cc) 
			{
				pxb->cc->ShowWindow(SW_HIDE);
			}
			top += m_buttonHeight;
		}
	}
}

void CAxMBarCtrl::ResizeControl_DLG(CControlList& list, int cx, int cy)
{
	int top = GetTopPixel(), bottom = cy - bottomMargin, left = sideMargin, width = cx - sideMargin * 2;
	int height;
	PAXCONTROL pxb;

	for (POSITION pos = list.GetHeadPosition(); pos; ) 
	{
		pxb = list.GetNext(pos);
		ASSERT(pxb->cc);

		switch (pxb->align) 
		{
			case MBARAL_TOP:
				height = pxb->height;
				if (top + height > bottom) 
					height = bottom - top;
				pxb->cc->SetWindowPos(NULL, left, top, 
					width,  height, SWP_SHOWWINDOW);

				top += height;
				break;
			case MBARAL_BOTTOM:
				pxb->cc->SetWindowPos(NULL, left, bottom - pxb->height, 
					width, pxb->height, SWP_SHOWWINDOW);
				bottom -= pxb->height;
				break;
			case MBARAL_NONE:
				if (pxb->cc->GetParent() == this)
					pxb->cc->SetWindowPos(NULL, left, pxb->top, 
						width, pxb->height, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
				break;
		}
	}

	for (pos = list.GetHeadPosition(); pos; ) 
	{
		pxb = list.GetNext(pos);
		ASSERT(pxb->cc);

		if (pxb->align == MBARAL_CLIENT)
			pxb->cc->SetWindowPos(NULL, left, top, width, bottom - top, SWP_SHOWWINDOW);
	}
}

int CAxMBarCtrl::AddTab(LPCTSTR tabName, LONG value)
{
	m_tab->InsertItem(m_tab->GetItemCount(), tabName);

	int index = m_tab->GetItemCount() - 1;
	SetTabValue(index, value);

	PCONTROLLIST pl = new CONTROLLIST;
	pl->conType = MBAR_NONE;
	m_cons.AddTail(pl);
	return index;
}

BOOL CAxMBarCtrl::IsValidTabIndex(int index)
{
	return (index >= 0) && (index < m_tab->GetItemCount());
}

void CAxMBarCtrl::DeleteTab(int index)
{
	if (IsValidTabIndex(index)) 
	{
		m_tab->DeleteItem(index);
		POSITION pos = m_cons.FindIndex(index);
		if (pos) 
		{
			PCONTROLLIST pl = m_cons.GetAt(pos);
			DeleteAllControls(&pl->list);
			m_cons.RemoveAt(pos);
			delete pl;
		}
	}
}

int CAxMBarCtrl::GetTabIndex()
{
	return m_tab->GetCurSel();
}

void CAxMBarCtrl::SetTabIndex(int index)
{
	if (IsValidTabIndex(index) && GetTabIndex() != index) 
	{
		m_tab->SetCurSel(index);
		ResizeControls();
	}
}

CString	CAxMBarCtrl::GetTabString(int index)
{
	if (IsValidTabIndex(index)) 
	{
		CString str;
		TCITEM item;
		ZeroMemory(&item, sizeof(item));
		item.mask = TCIF_TEXT;
		m_tab->GetItem(index, &item);
		str = item.pszText;
		return str;
	}

	return CString("");
}

void CAxMBarCtrl::SetTabString(int index, LPCTSTR str)
{
	if (IsValidTabIndex(index)) 
	{
		TCITEM item;
		ZeroMemory(&item, sizeof(item));
		item.mask = TCIF_TEXT;
		item.pszText = (LPSTR)str;
		m_tab->SetItem(index, &item);
	}
}

LONG CAxMBarCtrl::GetTabValue(int index)
{
	if (IsValidTabIndex(index)) 
	{
		TCITEM item;
		ZeroMemory(&item, sizeof(item));
		item.mask = TCIF_PARAM;
		m_tab->GetItem(index, &item);
		return item.lParam;
	}
	return 0L;
}

void CAxMBarCtrl::SetTabValue(int index, LONG value, PLONG oldValue) // = NULL)
{
	if (IsValidTabIndex(index)) 
	{
		TCITEM item;
		ZeroMemory(&item, sizeof(item));
		item.mask = TCIF_PARAM;
		if (oldValue) 
		{
			m_tab->GetItem(index, &item);
			*oldValue = item.lParam;
		}
		item.lParam = value;
		m_tab->SetItem(index, &item);
	}
}

// Button management functions..
int CAxMBarCtrl::AddButton(LPCTSTR btnName, UINT containControlType, LONG value)
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return -1;
	if (pl->conType == MBAR_DLG) return -1;

	pl->conType = MBAR_BAR;

	PAXCONTROL pxb = new AXCONTROL;

	ZeroMemory(pxb, sizeof(AXCONTROL));

	CAxMSpeedButton* btn = new CAxMSpeedButton();
	btn->Create(btnName, CRect(0, 0, 10, 10), this, GetAutoID());
	if (!m_currBtn) 
	{
		m_currBtn = btn;
		SetButtonBitmap(btn, "GTB_BTN_ON.BMP");
	}
	else 
	{
		SetButtonBitmap(btn, "GTB_BTN_OFF.BMP");
	}
	btn->StretchMode(TRUE);
	btn->SetFont(&m_font);
	btn->SetTextDraw(TRUE);

	pxb->btn = btn;
	pxb->ccType = containControlType;
	pxb->cc = CreateContainControl(pxb->ccType);

	pl->list.AddTail(pxb);

	ResizeControls();
	return pl->list.GetCount() - 1;
}

void CAxMBarCtrl::DeleteButton(int index)
{

}

void CAxMBarCtrl::DeleteAllControls(CControlList* bl) 
{
	PAXCONTROL pxb;

	while (!bl->IsEmpty()) 
	{
		pxb = bl->RemoveHead();
		SAFE_DELETE_WND(pxb->btn);
		SAFE_DELETE_WND(pxb->cc);

		delete pxb;
	}
}

int CAxMBarCtrl::GetButtonIndex()
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return -1;
	if (pl->conType != MBAR_BAR) return -1;

	int i = 0;
	CAxMSpeedButton* btn;

	for (POSITION pos = pl->list.GetHeadPosition(); pos; i++) 
	{
		btn = pl->list.GetNext(pos)->btn;
		if (btn->GetDown()) 
		{
			return i;
		}
	}
	return -1;
}

void CAxMBarCtrl::SetButtonIndex(int index)
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return;
	if (pl->conType != MBAR_BAR) return;

	ResetButtons();
	POSITION pos = pl->list.FindIndex(index);
	if (pos) 
	{
		//CAxMButtonCtrl* btn = pl->list.GetAt(pos)->btn;
		CAxMSpeedButton* btn = pl->list.GetAt(pos)->btn;
		btn->SetDown(TRUE);
		SetButtonBitmap(btn, "GTB_BTN_ON.BMP");
		ResizeControls();
	}
}

CString CAxMBarCtrl::GetButtonString(int index)
{
	return CString("");
}

void CAxMBarCtrl::SetButtonString(int index, LPCTSTR str)
{
}

LONG CAxMBarCtrl::GetButtonValue(int index)
{
	return 0L;
}

void CAxMBarCtrl::SetButtonValue(int index, LONG value)
{
}

BOOL CAxMBarCtrl::IsValidButtonIndex(int index)
{
	return TRUE;
}

PCONTROLLIST CAxMBarCtrl::GetControlList()
{
	POSITION pos = m_cons.FindIndex(GetTabIndex());
	if (pos) 
		return m_cons.GetAt(pos);

	return NULL;
}

int CAxMBarCtrl::GetButtonHeight()
{
	return m_buttonHeight;
}

void CAxMBarCtrl::SetButtonHeight(int height)
{
	if (m_buttonHeight != height) 
	{
		m_buttonHeight = height;
		ResizeControls();
	}
}

void CAxMBarCtrl::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	ResizeControls();

	Notify(WM_AXITEMCLICK, MAKEWPARAM(BARN_TABCHANGE, m_tab->GetCurSel()), (LPARAM)this);
}

BOOL CAxMBarCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT id = LOWORD(wParam);

	if (id >= IDC_CONTROL_BASE && id < m_idBase) 
		OnCommandControl(LOWORD(wParam), HIWORD(wParam), lParam);

	return CWnd::OnCommand(wParam, lParam);
}

void CAxMBarCtrl::OnCommandControl(UINT id, UINT notify, LONG lParam)
{
	CAxMSpeedButton* btn = GetButton(id);

	if (!btn)
		return;

	switch (notify) 
	{
		case BN_CLICKED:
			ResetButtons();
			m_currBtn = btn;
			SetButtonBitmap(btn, "GTB_BTN_ON.BMP");
			Notify(WM_AXITEMCLICK, MAKEWPARAM(BARN_BTNCHANGE, id - IDC_CONTROL_BASE), (LPARAM)this);
			
			ResizeControls();
			break;
	}
}

CAxMSpeedButton* CAxMBarCtrl::GetButton(UINT id)
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return NULL;
	if (pl->conType != MBAR_BAR) return NULL;

	CAxMSpeedButton* btn;
	for (POSITION pos = pl->list.GetHeadPosition(); pos; ) 
	{
		btn = pl->list.GetNext(pos)->btn;

		if (id == (UINT)::GetWindowLong(btn->m_hWnd, GWL_ID))
			return btn;
	}
	return NULL;
}

void CAxMBarCtrl::ResetButtons()
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return;
	if (pl->conType != MBAR_BAR) return;

	for (POSITION pos = pl->list.GetHeadPosition(); pos; ) 
	{
		pl->list.GetAt(pos)->btn->SetDown(FALSE);
		SetButtonBitmap(pl->list.GetNext(pos)->btn, "GTB_BTN_OFF.BMP");
	}
}

CWnd* CAxMBarCtrl::CreateContainControl(UINT ccType, LPCTSTR text)
{
	CWnd* wnd = NULL;
	CRect rcTemp(0, 0, 10, 10);

	switch (ccType) 
	{
		case MBAR_TREE:
			wnd = new CAxMTreeCtrl();
			((CAxMTreeCtrl*)wnd)->Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | TVS_HASBUTTONS | 
				TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP | BS_NOTIFY, rcTemp, this, GetAutoID());
			break;
		case MBAR_LIST:
			wnd = new CAxMListCtrl();
			((CAxMListCtrl*)wnd)->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT,
				rcTemp, this, GetAutoID());
			break;
		case MBAR_STATIC:
			wnd = new CAxMStaticCtrl();
			((CAxMStaticCtrl*)wnd)->Create(text, WS_CHILD | WS_VISIBLE | SS_CENTER,
				rcTemp, this, GetAutoID());
			break;
		case MBAR_BUTTON:
			wnd = new CAxMButtonCtrl();
			((CAxMButtonCtrl*)wnd)->Create(text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_NOTIFY, rcTemp, this, GetAutoID());
			break;			
		case MBAR_SPDBTN:
			wnd = new CAxMSpeedButton();
			((CAxMSpeedButton*)wnd)->Create(text, rcTemp, this, GetAutoID());
			break;
		case MBAR_RADIO:
			wnd = new CAxMRadioButton();
			((CAxMRadioButton*)wnd)->Create(text, rcTemp, this, GetAutoID());
			break;
		case MBAR_SPIN:
			wnd = new CAxMSpinCtrl();
			((CAxMSpinCtrl*)wnd)->Create(WS_VISIBLE | WS_CHILD | UDS_NOTHOUSANDS | UDS_SETBUDDYINT 
				/*| UDS_AUTOBUDDY*/, rcTemp, this, GetAutoID());
			break;
		case MBAR_EDIT:
			wnd = new CAxMEdit();
			((CAxMEdit*)wnd)->Create(WS_VISIBLE | WS_CHILD | WS_BORDER, rcTemp, this, GetAutoID());
			break;
	}

	if (wnd)
		wnd->SetFont(&m_font);

	return wnd;
}

CWnd* CAxMBarCtrl::GetButtonControl(int index)
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return NULL;
	if (pl->conType != MBAR_BAR) return NULL;

	POSITION pos = pl->list.FindIndex(index);
	if (pos) 
	{
		PAXCONTROL pxb = pl->list.GetAt(pos);
		return pxb->cc;
	}

	return NULL;
}

CWnd* CAxMBarCtrl::AddControl(int ctrlKind, int align, int top, LPCTSTR lpszHeight, LPCTSTR lpszText)
{
	PCONTROLLIST pl = GetControlList();
	if (!pl) return NULL;
	if (pl->conType == MBAR_BAR) return NULL;
	pl->conType = MBAR_DLG;

	PAXCONTROL pxb = new AXCONTROL;
	ZeroMemory(pxb, sizeof(AXCONTROL));

	pxb->cc = CreateContainControl(ctrlKind, lpszText);
	pl->list.AddTail(pxb);

	pxb->ccType = ctrlKind;
	pxb->align = align;
	pxb->top = top;
	pxb->height = atoi(lpszHeight);

	ResizeControls();

	return pxb->cc;
}

void CAxMBarCtrl::Refresh()
{
	ResizeControls();
}

void CAxMBarCtrl::SetButtonShade(UINT shadeID)
{
	if (shadeID > 8)
		shadeID = 8;

	m_buttonShadeID = shadeID;
	Refresh();
}

UINT CAxMBarCtrl::GetButtonShade()
{
	return m_buttonShadeID;
}

void CAxMBarCtrl::SetButtonColor(COLORREF color)
{
	m_buttonColor = color;
	Refresh();
}

COLORREF CAxMBarCtrl::GetButtonColor()
{
	return m_buttonColor;
}

void CAxMBarCtrl::SetButtonColorPercent(UINT percent)
{
	if (percent > 100) 
		percent = 100;
	m_buttonColorPercent = percent;
	Refresh();
}

UINT CAxMBarCtrl::GetButtonCOlorPercent()
{
	return m_buttonColorPercent;
}

LRESULT CAxMBarCtrl::OnSetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	SetMessageHandler((CWnd*)wParam, (BOOL)lParam);
	return 0L;
}

LRESULT CAxMBarCtrl::OnGetMessageHandler(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)GetMessageHandler((CWnd*)wParam);
}

void CAxMBarCtrl::SetMessageHandler(CWnd* wnd, BOOL setChild)
{
	m_messageHandler = wnd;
	
	if (setChild) 
	{
		PCONTROLLIST pl;
		PAXCONTROL pxb;
		for (POSITION pos = m_cons.GetHeadPosition(); pos; ) 
		{
			pl = m_cons.GetNext(pos);
			for (POSITION pos1 = pl->list.GetHeadPosition(); pos1; ) 
			{
				pxb = pl->list.GetNext(pos1);

				if (pxb->btn && ::IsWindow(pxb->btn->m_hWnd))
					pxb->btn->SendMessage(AXM_SETMSGHANDLER, (WPARAM)wnd);
				if (pxb->cc && ::IsWindow(pxb->cc->m_hWnd))
					pxb->cc->SendMessage(AXM_SETMSGHANDLER, (WPARAM)wnd);
			}
		}
	}
}

CWnd* CAxMBarCtrl::GetMessageHandler(CWnd* child)
{
	if (!child || !::IsWindow(child->m_hWnd))
		return m_messageHandler;

	return (CWnd*)child->SendMessage(AXM_GETMSGHANDLER);
}

LRESULT CAxMBarCtrl::Notify(UINT msg, WPARAM wParam, LPARAM lParam)
{
	NOTIFY(msg, wParam, lParam);
}

BOOL CAxMBarCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

int CAxMBarCtrl::GetTopPixel()
{
	CRect rc;
	m_tab->GetItemRect(m_tab->GetCurSel(), rc);
	return rc.bottom + 1;
}

void CAxMBarCtrl::SetButtonBitmap(CAxMSpeedButton* btn, LPCTSTR bmpName)
{
	btn->LoadBitmap(m_imageFolder + bmpName);
}

void CAxMBarCtrl::ChangePalette(LPCTSTR paletteName, COLORREF clrHeader)
{
	if (m_paletteName != paletteName)
		m_paletteName = paletteName;
	else 
		return;

	CString	sSel, sBack;
	sSel.Format("%sGTB_STAB.bmp", m_imageFolder);
	sBack.Format("%sGTB_BTAB.bmp", m_imageFolder);
	m_tab->SetTabImage(sSel, sBack);

	m_tab->SetDrawMode(CAxMTabCtrl::DM_IMAGE);


	PAXCONTROL pxb;
	POSITION pos = m_cons.FindIndex(1);
	if (!pos) return;


	PCONTROLLIST pl = m_cons.GetAt(pos);
	
	for (pos = pl->list.GetHeadPosition(); pos; ) 
	{
		pxb = pl->list.GetNext(pos);
		
		ASSERT(pxb->cc);

		if (pxb->ccType == MBAR_STATIC)
			//((CAxMStaticCtrl*)pxb->cc)->SetBgColor(clrHeader);
			((CAxMStaticCtrl*)pxb->cc)->SetBgColor(clBACK);
	}
}
