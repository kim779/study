// ScriptBar.cpp: implementation of the CScriptBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiswork.h"
#include "mainvar.h"
#include "ScriptBar.h"
#include "keywords.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define	CE_CLICK	1
#define	CE_CHANGE	2
#define	CE_DBLCLK	4
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptBar::CScriptBar()
{
	m_click       = _T("");
	m_dblclk      = _T("");
	m_change      = _T("");
	m_char        = _T("");
	m_keychange   = _T("");
	m_pFormItem   = NULL;
	m_index       = -1;
	m_bRedraw     = true;
	m_hPinBitmap  = NULL;
	m_hPinBitmap2 = NULL;

	m_bToDescription = FALSE;
}

CScriptBar::~CScriptBar()
{
	if (m_pFormItem)
		delete m_pFormItem;
	
	::DeleteObject(m_hPinBitmap);
	::DeleteObject(m_hPinBitmap2);
}

BEGIN_MESSAGE_MAP(CScriptBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CScriptBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELENDOK(ID_SRP_CTRLLIST, OnCLSelEndOk)
	ON_CBN_SELENDOK(ID_SRP_EVENTLIST, OnELSelEndOk)
	ON_BN_CLICKED(ID_SCR_PINBTN, OnPinBtnClick)
	ON_BN_CLICKED(ID_SCR_PYBTN, OnPyBtnClick)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER + 100, OnMessage100)
END_MESSAGE_MAP()

int CScriptBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_ctrllistCBO.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS |CBS_SORT | CBS_OWNERDRAWFIXED
		,CRect(0), this, ID_SRP_CTRLLIST ))
		return -1;

	if (!m_eventlistCBO.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED
		,CRect(0), this, ID_SRP_EVENTLIST ))
		return -1;

	if (!m_editScript.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0, 0, 0, 0), this, ID_SRP_RICHEDIT))
		return -1;

	m_editScript.EnableWindow(FALSE);

	if (!m_pinBtn.Create(NULL, "P", WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0), this, ID_SCR_PINBTN))
		return -1;

	if (!m_pythonBtn.Create(NULL, "PY", WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0), this, ID_SCR_PYBTN))
		return -1;

	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80, "MS Sans Serif"))
			return -1;

	LoadAutoList();

	m_ctrllistCBO.SetFont(&m_font);
	m_eventlistCBO.SetFont(&m_font);
	m_pinBtn.SetFont(&m_font);
	m_pythonBtn.SetFont(&m_font);

	m_hPinBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PINBTNNOPUSH));
	m_pinBtn.m_hBitmap = m_hPinBitmap;

	m_hPinBitmap2 = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PINBTNPUSH));
	m_pinBtn.m_hBitmap2 = m_hPinBitmap2;

	m_editScript.Initialize();

	CImageList	*imglist = new CImageList;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_SELECTIONBAR);
	imglist->Create(17, 17, ILC_COLOR24|ILC_MASK, 26, 1);
	imglist->Add(&bitmap, RGB(0, 255, 255));
	imglist->Add(AfxGetApp()->LoadIcon(IDI_MAP));
	imglist->Add(AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	imglist->Add(AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	imglist->SetBkColor(CLR_NONE);
	m_ctrllistCBO.SetImageList(imglist);
	bitmap.Detach();

	return 0;
}

void CScriptBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);

	rc.DeflateRect(0, 2, rc.Width() - 21, 0);
	rc.bottom = rc.top + 18;
	m_pinBtn.MoveWindow(rc);

	GetClientRect(rc);
	rc.DeflateRect(23, 2, rc.Width() - 51, 0);
	rc.bottom = rc.top + 18;
	m_pythonBtn.MoveWindow(rc);

	GetClientRect(rc);

	rc.DeflateRect(53, 2, rc.Width() / 2 + 5, 0);
	rc.bottom = rc.top + 220;
	m_ctrllistCBO.MoveWindow(rc);
	m_ctrllistCBO.SetItemHeight(-1, 15);

	GetClientRect(rc);
	rc.DeflateRect(rc.Width() / 2 + 5, 2, 0, 0);
	rc.bottom = rc.top + 220;
	m_eventlistCBO.MoveWindow(rc);
	m_eventlistCBO.SetItemHeight(-1, 15);

	GetClientRect(rc);
	rc.DeflateRect(0, 25, 0, 2);
	m_editScript.MoveWindow(rc);
}

void CScriptBar::ResetCombo(int type)
{
	if (type == 2)
	{
		m_eventlistCBO.ResetContent();
		m_eventlistCBO.resetItemInfo();
	}
	else
	{
		m_ctrllistCBO.ResetContent();
		m_ctrllistCBO.resetItemInfo();
		if (type == 1)
		{
			m_ctrllistCBO.SetItemData(m_ctrllistCBO.AddString("_form_"), -1);
			m_ctrllistCBO.SetItemData(m_ctrllistCBO.AddString("_declaration_"), -2);
		}
		else
			m_index = -3;
		m_mapH = NULL;
		m_editScript.Initialize(m_mapH);
	}
	insertToEdit("");
}

void CScriptBar::AddComboString(CString sItem, int data, int type)
{
	if (type)
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString(sItem), data);
	else
	{
		int idx;
		m_ctrllistCBO.SetItemData(idx = m_ctrllistCBO.AddString(sItem), data);
		if (data >= 0 && m_pFormItem->getFormPoint(data)->scpN)
			m_ctrllistCBO.SetItemBold(idx, true);
		if (data == m_index)
			m_ctrllistCBO.SetCurSel(idx);
		
		int nImage = 0;
		if ((int)data >= 0)
		{
			nImage = m_pFormItem->getFormPoint(data)->kind;
			if (nImage == FM_BUTTON)
			{
				if (m_pFormItem->getFormPoint(data)->type == BTN_RADIO)
					nImage = 21;
				else if (m_pFormItem->getFormPoint(data)->type == BTN_CHECK)
					nImage = 22;
			}
			else if (nImage >= FM_GRAPH)
				nImage--;
		}
		else if ((int)data < 0)
			nImage = 23 + m_mapH->mapK;
		
		m_ctrllistCBO.SetItemImage(idx, nImage);
	}
}

void CScriptBar::ReplaceComboString(CString sItem, int data)
{
	for (int ii = 0; ii < m_ctrllistCBO.GetCount(); ii++)
	{
		if ((int)m_ctrllistCBO.GetItemData(ii) == data)
		{
			m_ctrllistCBO.DeleteString(ii);
			AddComboString(sItem, data);
		}
	}
}

void CScriptBar::OnCLSelEndOk()
{
	WriteScript();
	if (m_ctrllistCBO.GetCurSel() != LB_ERR)
	{
		m_index = m_ctrllistCBO.GetItemData(m_ctrllistCBO.GetCurSel());
		m_click     = _T("");
		m_dblclk    = _T("");
		m_change    = _T("");
		m_char      = _T("");
		m_keychange = _T("");
		insertToEdit("");
		AddEventList();
		if (m_bRedraw && m_index > -2)
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOCHILD, MAKELPARAM(m_index, SC_SCRIPTBAR));	//	select control
	}
}

void CScriptBar::OnELSelEndOk()
{
	if (!m_mapH || m_mapH->mapK == MK_PROCEDURES)
		return;
	if (m_index == -1)
	{
		WriteScript();
		m_prev = m_eventlistCBO.GetItemData(m_eventlistCBO.GetCurSel());
		switch (m_prev)
		{
		case 1:
			setScript(m_mapH->onStartN);	break;
		case 2:
			setScript(m_mapH->onSendN);	break;
		case 3:
			setScript(m_mapH->onReceiveN);	break;
		case 4:
			setScript(m_mapH->onAlertN);	break;
		case 5:
			setScript(m_mapH->onServiceN);	break;
		case 6:
			setScript(m_mapH->onFileN);	break;
		case 7:
			setScript(m_mapH->onSelectN);	break;
		case 8:
			setScript(m_mapH->onTimerN);	break;
		case 9:
			setScript(m_mapH->onFocusN);	break;
		case 10:
			setScript(m_mapH->onCloseN);	break;
		case 11:
			setScript(m_mapH->onDeviceN);	break;
		case 12:
			setScript(m_mapH->onApproveN);	break;
		case 13:
			setScript(m_mapH->onKeyN);	break;
		case 35:
			setScript(m_mapH->onTimerXN);	break;
		default:
			insertToEdit("");
			m_editScript.EnableWindow(FALSE);
			break;
		}
		return;
	}
	else if (m_index == -2)
	{
		WriteScript();
		m_prev = m_eventlistCBO.GetItemData(m_eventlistCBO.GetCurSel());
		switch(m_prev)
		{
		case 1:
			setScript(m_mapH->onDeclarationN);	break;
		case 2:
			setScript(m_mapH->onInDeclarationN);	break;
		}
		return;
	}

	switch (m_prev)
	{
	case 1:		// click or change
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_EDIT:
		case FM_MEMO:
		case FM_COMBO:
			m_change = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onChangeL = m_change.GetLength();
			break;
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TREE:
		case FM_TABLE:
		case FM_SHEET:
			m_click = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onClickL = m_click.GetLength();
			break;
		case FM_CONTROL:
			{
				CString sym = m_pFormItem->getFormStr(m_index, "str");
				long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
				if (eventMask & CE_CLICK)
				{
					m_click = GetEditScript();
					m_pFormItem->getFormPoint(m_index)->onClickL = m_click.GetLength();
				}
			}
			break;

		default:
			break;
		}
		break;

	case 2:		// dblclk
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_EDIT:
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TABLE:
			m_dblclk = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onDblClkL = m_dblclk.GetLength();
			break;
		case FM_CONTROL:
			{
				CString sym = m_pFormItem->getFormStr(m_index, "str");
				long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
				if (eventMask & CE_DBLCLK)
				{
					m_dblclk = GetEditScript();
					m_pFormItem->getFormPoint(m_index)->onDblClkL = m_dblclk.GetLength();
				}
			}
			break;
		default:
			break;
		}
		break;

	case 3:		// char or change
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_EDIT:
			m_char = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onCharL = m_char.GetLength();
			break;

		case FM_GRID:
			m_change = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onChangeL = m_change.GetLength();
			break;
		case FM_CONTROL:
			{
				CString sym = m_pFormItem->getFormStr(m_index, "str");
				long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
				if (eventMask & CE_CHANGE)
				{
					m_change = GetEditScript();
					m_pFormItem->getFormPoint(m_index)->onChangeL = m_change.GetLength();
				}
			}
			break;
		}
		break;
	case 4:		// char
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_GRID:
			m_char = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onCharL = m_char.GetLength();
			break;
		}
		break;
	case 5:	// keychange
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_GRID:
			m_keychange = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onKeyL = m_keychange.GetLength();
			break;
		case FM_EDIT:
			m_keychange = GetEditScript();
			m_pFormItem->getFormPoint(m_index)->onKeyL = m_keychange.GetLength();
			break;
		}
		break;
	case 0:
	default:
		break;
	}

	m_prev = m_eventlistCBO.GetItemData(m_eventlistCBO.GetCurSel());
	switch (m_prev)
	{
	case 1:		// click or change
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_EDIT:
		case FM_MEMO:
		case FM_COMBO:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_change);
			break;
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID: 
		case FM_TAB:
		case FM_UTAB:
		case FM_TREE:
		case FM_TABLE:
		case FM_SHEET:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_click);
			break;
		case FM_CONTROL:
			{
				CString sym = m_pFormItem->getFormStr(m_index, "str");
				long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
				if (eventMask & CE_CLICK)
				{
					m_editScript.EnableWindow(TRUE);
					m_editScript.EnableScrollBar(SB_BOTH);
					insertToEdit(m_click);
				}
			}
			break;
		default:
			break;
		}
		break;

	case 2: 	// dblclk
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_EDIT:
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID: 
		case FM_TAB:
		case FM_UTAB:
		case FM_TABLE:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_dblclk);
			break;
		case FM_CONTROL:
			{
				CString sym = m_pFormItem->getFormStr(m_index, "str");
				long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
				if (eventMask & CE_DBLCLK)
				{
					m_editScript.EnableWindow(TRUE);
					m_editScript.EnableScrollBar(SB_BOTH);
					insertToEdit(m_dblclk);
				}
			}
			break;
		default:
			break;
		}
		break;

	case 3:		// char or change
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_EDIT:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_char);
			break;
		case FM_GRID:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_change);
			break;
		case FM_CONTROL:
			{
				CString sym = m_pFormItem->getFormStr(m_index, "str");
				long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
				if (eventMask & CE_CHANGE)
				{
					m_editScript.EnableWindow(TRUE);
					m_editScript.EnableScrollBar(SB_BOTH);
					insertToEdit(m_change);
				}
			}
			break;
		}
		break;

	case 4:		// char
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_GRID:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_char);
			break;
		}
		break;

	case 5:		// keychange
		switch (m_pFormItem->getFormPoint(m_index)->kind)
		{
		case FM_GRID:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_keychange);
			break;
		case FM_EDIT:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_keychange);
			break;
		}
		break;
	case 0:
	default:
		insertToEdit("");
		m_editScript.EnableWindow(FALSE);
		break;
	}
}

void CScriptBar::Initialize(_mapH *mapH)
{
	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);
	m_mapH = mapH;
	m_editScript.Initialize(m_mapH);
	m_pythonBtn.SetCheck(m_mapH->pythonMode);
	m_pythonBtn.SetWindowText(m_mapH->pythonMode ? "PY" : "VB");
	m_editScript.SetPythonMode(m_mapH->pythonMode);
	if (m_mapH && m_mapH->mapK == MK_PROCEDURES)
		m_editScript.EnableWindow(FALSE);
	m_index = -1;
	m_prev = -1;
	if (m_mapH->onDeclaration || m_mapH->onInDeclaration)
		m_ctrllistCBO.SetItemBold(0, true);	// 0��°�� declaration
	if (m_mapH->onStart || m_mapH->onSend || m_mapH->onReceive || m_mapH->onAlert
		|| m_mapH->onService || m_mapH->onFile || m_mapH->onSelect || m_mapH->onTimer
		|| m_mapH->onFocus || m_mapH->onClose || m_mapH->onDevice || m_mapH->onApprove 
		|| m_mapH->onKey)
		m_ctrllistCBO.SetItemBold(1, true);	// 1��°�� form
	m_eventlistCBO.ResetContent();
	m_eventlistCBO.resetItemInfo();
}

void CScriptBar::AddEventList()
{
	if (!m_mapH)
		return;
	int kind = -1;
	bool	isScript = false;
	struct _formR* formR = NULL;
	ResetCombo(2);
	m_editScript.EnableWindow(TRUE);
	m_editScript.EnableScrollBar(SB_BOTH);
	if (m_index >= 0)
	{
		formR = m_pFormItem->getFormPoint(m_index);
		kind = formR->kind;
		if (formR->scpN)
		{
			isScript = true;
			m_ctrllistCBO.SetItemBold(m_ctrllistCBO.GetCurSel(), true);
		}
	}
	else
	{
		kind = m_index;
		if (m_index == -2 && (m_mapH->onDeclaration || m_mapH->onInDeclaration))
			m_ctrllistCBO.SetItemBold(m_ctrllistCBO.GetCurSel(), true);
		if (m_index == -1 &&
			(m_mapH->onStart || m_mapH->onSend || m_mapH->onReceive || m_mapH->onAlert
			|| m_mapH->onService || m_mapH->onFile || m_mapH->onSelect || m_mapH->onTimer
			|| m_mapH->onTimerX
			|| m_mapH->onFocus || m_mapH->onClose || m_mapH->onDevice || m_mapH->onApprove
			|| m_mapH->onKey))
			m_ctrllistCBO.SetItemBold(m_ctrllistCBO.GetCurSel(), true);		
	}

	switch (kind)
	{
	case -2:	//	declaration
		if (m_mapH->mapK != MK_PROCEDURES)
		{
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("Declaration"), 1);
			if (m_mapH->onDeclaration) m_eventlistCBO.SetItemBold(0, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("include Declaration"), 2);
			if (m_mapH->onInDeclaration) m_eventlistCBO.SetItemBold(1, true);
		}
		else
			m_editScript.EnableWindow(FALSE);
		break;
	case -1:	//	form
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		if (m_mapH->mapK != MK_PROCEDURES)
		{
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnStart"), 1);
			if (m_mapH->onStart) m_eventlistCBO.SetItemBold(1, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnSend"), 2);
			if (m_mapH->onSend) m_eventlistCBO.SetItemBold(2, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnReceive"), 3);
			if (m_mapH->onReceive) m_eventlistCBO.SetItemBold(3, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnAlert(code, data, state)"), 4);
			if (m_mapH->onAlert) m_eventlistCBO.SetItemBold(4, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnService(data, length)"), 5);
			if (m_mapH->onService) m_eventlistCBO.SetItemBold(5, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnFile(upload, data, length)"), 6);
			if (m_mapH->onFile) m_eventlistCBO.SetItemBold(6, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnSelect(data)"), 7);
			if (m_mapH->onSelect) m_eventlistCBO.SetItemBold(7, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnTimer"), 8);
			if (m_mapH->onTimer) m_eventlistCBO.SetItemBold(8, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnFocus"), 9);
			if (m_mapH->onFocus) m_eventlistCBO.SetItemBold(9, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnClose"), 10);
			if (m_mapH->onClose) m_eventlistCBO.SetItemBold(10, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnDevice"), 11);
			if (m_mapH->onDevice) m_eventlistCBO.SetItemBold(11, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnApprove(key, data, length)"), 12);
			if (m_mapH->onApprove) m_eventlistCBO.SetItemBold(12, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnKey"), 13);
			if (m_mapH->onKey) m_eventlistCBO.SetItemBold(13, true);
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnTimerX(id)"), 35);
			if (m_mapH->onTimerX) m_eventlistCBO.SetItemBold(14, true);
		}
		else
			m_editScript.EnableWindow(FALSE);
		break;
	case FM_EDIT:
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnChange"), 1);
		if (isScript && formR->onChange != -1) m_eventlistCBO.SetItemBold(1, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnCharChange"), 3);
		if (isScript && formR->onChar != -1) m_eventlistCBO.SetItemBold(2, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnKey"), 5);
		if (isScript && formR->onKey != -1) m_eventlistCBO.SetItemBold(3, true);
		break;

	case FM_MEMO:
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnChange"), 1);
		if (isScript && formR->onChange != -1) m_eventlistCBO.SetItemBold(1, true);
		break;

	case FM_COMBO:
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnChange"), 1);
		if (isScript && formR->onChange != -1) m_eventlistCBO.SetItemBold(1, true);
		break;

	case FM_GRID:
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnClick"), 1);
		if (isScript && formR->onClick != -1) m_eventlistCBO.SetItemBold(1, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnDoubleClick"), 2);
		if (isScript && formR->onDblClk != -1) m_eventlistCBO.SetItemBold(2, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnChange"), 3);
		if (isScript && formR->onChange != -1) m_eventlistCBO.SetItemBold(3, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnCharChange"), 4);
		if (isScript && formR->onChar != -1) m_eventlistCBO.SetItemBold(4, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnKey"), 5);
		if (isScript && formR->onKey != -1) m_eventlistCBO.SetItemBold(5, true);
		break;

	case FM_LABEL:
	case FM_OUT:
	case FM_PANEL:
	case FM_BUTTON:
	case FM_TAB:
	case FM_UTAB:
	case FM_TABLE:
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnClick"), 1);
		if (isScript && formR->onClick != -1) m_eventlistCBO.SetItemBold(1, true);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnDoubleClick"), 2);
		if (isScript && formR->onDblClk != -1) m_eventlistCBO.SetItemBold(2, true);
		break;

	case FM_TREE:
	case FM_SHEET:
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
		m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnClick"), 1);
		if (isScript && formR->onClick != -1) m_eventlistCBO.SetItemBold(1, true);
		break;
	case FM_CONTROL:
		{
			CString sym = m_pFormItem->getFormStr(m_index, "str");
			long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
			m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("None"), -1);
			if (eventMask & CE_CLICK)
			{
				m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnClick"), 1);
				if (isScript && formR->onClick != -1) m_eventlistCBO.SetItemBold(1, true);
			}
			if (eventMask & CE_DBLCLK)
			{
				m_eventlistCBO.SetItemData(m_eventlistCBO.AddString("OnDoubleClick"), 2);
				if (isScript && formR->onDblClk != -1) m_eventlistCBO.SetItemBold((eventMask & CE_CLICK)?2:1, true);
			}
			if (eventMask & CE_CHANGE)
			{
				int nIdx = m_eventlistCBO.AddString("OnChange");
				m_eventlistCBO.SetItemData(nIdx, 3);
				if (isScript && formR->onChange != -1) m_eventlistCBO.SetItemBold(nIdx, true);
			}
		}
		break;

	default:
		break;
	}

	if (m_index == -1)
	{
		bool	set = false;
		
		if (!set && m_mapH->onStart)
		{
			m_eventlistCBO.SetCurSel(1);
			setScript(m_mapH->onStartN);
			set = true;
		}
		
		if (!set && m_mapH->onSend)
		{
			m_eventlistCBO.SetCurSel(2);
			setScript(m_mapH->onSendN);
			set = true;
		}
		
		if (!set && m_mapH->onReceive)
		{
			m_eventlistCBO.SetCurSel(3);
			setScript(m_mapH->onReceiveN);
			set = true;
		}

		if (!set && m_mapH->onAlert)
		{
			m_eventlistCBO.SetCurSel(4);
			setScript(m_mapH->onAlertN);
			set = true;
		}

		if (!set && m_mapH->onService)
		{
			m_eventlistCBO.SetCurSel(5);
			setScript(m_mapH->onServiceN);
			set = true;
		}

		if (!set && m_mapH->onFile)
		{
			m_eventlistCBO.SetCurSel(6);
			setScript(m_mapH->onFileN);
			set = true;
		}

		if (!set && m_mapH->onSelect)
		{
			m_eventlistCBO.SetCurSel(7);
			setScript(m_mapH->onSelectN);
			set = true;
		}

		if (!set && m_mapH->onTimer)
		{
			m_eventlistCBO.SetCurSel(8);
			setScript(m_mapH->onTimerN);
			set = true;
		}

		if (!set && m_mapH->onFocus)
		{
			m_eventlistCBO.SetCurSel(9);
			setScript(m_mapH->onFocusN);
			set = true;
		}

		if (!set && m_mapH->onClose)
		{
			m_eventlistCBO.SetCurSel(10);
			setScript(m_mapH->onCloseN);
			set = true;
		}

		if (!set && m_mapH->onDevice)
		{
			m_eventlistCBO.SetCurSel(11);
			setScript(m_mapH->onDeviceN);
			set = true;
		}

		if (!set && m_mapH->onApprove)
		{
			m_eventlistCBO.SetCurSel(12);
			setScript(m_mapH->onApproveN);
			set = true;
		}
		
		if (!set && m_mapH->onKey)
		{
			m_eventlistCBO.SetCurSel(13);
			setScript(m_mapH->onKeyN);
			set = true;
		}
		
		if (!set)
		{
			m_eventlistCBO.SetCurSel(0);
			m_editScript.EnableWindow(FALSE);
			m_mapH->onStart   = false;
			m_mapH->onSend    = false;
			m_mapH->onReceive = false;
			m_mapH->onAlert   = false;
			m_mapH->onService = false;
			m_mapH->onFile    = false;
			m_mapH->onSelect  = false;
			m_mapH->onTimer   = false;
			m_mapH->onFocus   = false;
			m_mapH->onClose   = false;
			m_mapH->onDevice = false;
			m_mapH->onApprove = false;
			m_mapH->onKey     = false;
		}
		else
		{
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
		}
		
		m_prev = m_eventlistCBO.GetItemData(m_eventlistCBO.GetCurSel());
		return;
	}

	if (m_index == -2)
	{
		bool	set = false;

		if (!set && m_mapH->onDeclaration)
		{
			m_eventlistCBO.SetCurSel(0);
			setScript(m_mapH->onDeclarationN);
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			set = true;
		}

		if (!set && m_mapH->onInDeclaration)
		{
			m_eventlistCBO.SetCurSel(1);
			setScript(m_mapH->onInDeclarationN);
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			set = true;
		}

		if (!m_eventlistCBO.GetCount())
		{
			m_editScript.EnableWindow(FALSE);
			m_prev = -1;
			return;
		}

		if (!set)
		{
			m_eventlistCBO.SetCurSel(0);
		}

		m_prev = m_eventlistCBO.GetItemData(m_eventlistCBO.GetCurSel());
		return;
	}
	if (m_pFormItem->getFormStr(m_index, "scpN").IsEmpty())
	{
		m_eventlistCBO.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return;
	}

	HANDLE	file = CreateFile(m_pFormItem->getFormStr(m_index, "scpN"), GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		m_eventlistCBO.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return ;
	}

	DWORD	scpSize = GetFileSize(file, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
		CloseHandle(file);
		m_eventlistCBO.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return ;
	}

	char*	scpAlloc;
	scpAlloc = new char[scpSize+1];

	DWORD	dwReadLen = 0;
	BOOL	bRc = ReadFile(file, scpAlloc, scpSize, &dwReadLen, NULL);
	CloseHandle(file);
	if (!bRc || scpSize != dwReadLen)
	{
		m_eventlistCBO.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		delete[] scpAlloc;
		return ;
	}

	if (formR->onClick != -1)
		m_click = CString(&scpAlloc[formR->onClick], formR->onClickL);

	if (formR->onDblClk != -1)
		m_dblclk = CString(&scpAlloc[formR->onDblClk], formR->onDblClkL);

	if (formR->onChange != -1)
		m_change = CString(&scpAlloc[formR->onChange], formR->onChangeL);

	if (formR->onChar != -1)
		m_char = CString(&scpAlloc[formR->onChar], formR->onCharL);

	if (formR->onKey != -1)
		m_keychange = CString(&scpAlloc[formR->onKey], formR->onKeyL);

	delete[] scpAlloc;

	bool	set = false;
	int	nKind = formR->kind;
	if (!set && !m_click.IsEmpty() && nKind != FM_COMBO && nKind != FM_EDIT)
	{
		m_eventlistCBO.SetCurSel(1);
		insertToEdit(m_click);
		set = true;
	}

	if (!set && !m_change.IsEmpty()
		&& (nKind == FM_COMBO || nKind == FM_EDIT || nKind == FM_GRID || nKind == FM_MEMO))
	{
		m_eventlistCBO.SetCurSel((formR->kind == FM_GRID) ? 3 : 1);
		insertToEdit(m_change);
		set = true;
	}

	if (!set && !m_dblclk.IsEmpty() && nKind != FM_COMBO)
	{
		if (m_eventlistCBO.FindString(0, "OnDoubleClick") != CB_ERR)
			m_eventlistCBO.SetCurSel(m_eventlistCBO.FindString(0, "OnDoubleClick"));
		insertToEdit(m_dblclk);
		set = true;
	}

	if (!set && !m_char.IsEmpty() && (nKind == FM_EDIT || nKind == FM_GRID))
	{
		if (m_eventlistCBO.FindString(0, "OnCharChange") != CB_ERR)
			m_eventlistCBO.SetCurSel(m_eventlistCBO.FindString(0, "OnCharChange"));
		insertToEdit(m_char);
		set = true;
	}

	if (!set && !m_keychange.IsEmpty() && (nKind == FM_GRID || nKind == FM_EDIT))
	{
		if (m_eventlistCBO.FindString(0, "OnKey") != CB_ERR)
			m_eventlistCBO.SetCurSel(m_eventlistCBO.FindString(0, "OnKey"));
		insertToEdit(m_keychange);
		set = true;
	}

	if (!set)
	{
		m_eventlistCBO.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
	}

	m_prev = m_eventlistCBO.GetItemData(m_eventlistCBO.GetCurSel());
	m_editScript.SetFocus();
}

void CScriptBar::insertToEdit(CString str)
{
	m_editScript.SetScriptText(str);
}

CString CScriptBar::GetEditScript()
{
	return m_editScript.GetScriptText();
}

void CScriptBar::WriteScript()
{
	if (!m_mapH)
		return;
	if (m_index < -2)
		return;
	
	if (m_index == -1)
	{
		HANDLE	file;
		DWORD	nData = 0;
		CString	dir = _T(""), sData = "";

		sData = GetEditScript();
		
		switch (m_prev)
		{
		case 1:		// onStart
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onStart)
					DeleteFile(m_mapH->onStartN);
				m_mapH->onStartN[0] = '\0';
				m_mapH->onStart = false;
				break;
			}
			if (m_mapH->onStartN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0';
				wccGetTempName(m_mapH->onStartN, prefix);
			}
			dir = m_mapH->onStartN;
			m_mapH->onStart = true;
			break;
			
		case 2:		// onSend
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onSend)
					DeleteFile(m_mapH->onSendN);
				m_mapH->onSendN[0] = '\0';
				m_mapH->onSend = false;
				break;
			}
			if (m_mapH->onSendN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onSendN, prefix);
			}
			dir = m_mapH->onSendN;
			m_mapH->onSend = true;
			break;
			
		case 3:		// onReceive
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onReceive)
					DeleteFile(m_mapH->onReceiveN);
				m_mapH->onReceiveN[0] = '\0';
				m_mapH->onReceive = false;
				break;
			}
			if (m_mapH->onReceiveN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onReceiveN, prefix);
			}
			dir = m_mapH->onReceiveN;
			m_mapH->onReceive = true;
			break;

		case 4:		// onAlert
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onAlert)
					DeleteFile(m_mapH->onAlertN);
				m_mapH->onAlertN[0] = '\0';
				m_mapH->onAlert = false;
				break;
			}
			if (m_mapH->onAlertN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onAlertN, prefix);
			}
			dir = m_mapH->onAlertN;
			m_mapH->onAlert = true;
			break;

		case 5:		// onService
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onService)
					DeleteFile(m_mapH->onServiceN);
				m_mapH->onServiceN[0] = '\0';
				m_mapH->onService = false;
				break;
			}
			if (m_mapH->onServiceN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onServiceN, prefix);
			}
			dir = m_mapH->onServiceN;
			m_mapH->onService = true;
			break;

		case 6:		// onFile
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onFile)
					DeleteFile(m_mapH->onFileN);
				m_mapH->onFileN[0] = '\0';
				m_mapH->onFile = false;
				break;
			}
			if (m_mapH->onFileN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onFileN, prefix);
			}
			dir = m_mapH->onFileN;
			m_mapH->onFile = true;
			break;

		case 7:		// onSelect
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onSelect)
					DeleteFile(m_mapH->onSelectN);
				m_mapH->onSelectN[0] = '\0';
				m_mapH->onSelect = false;
				break;
			}
			if (m_mapH->onSelectN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onSelectN, prefix);
			}
			dir = m_mapH->onSelectN;
			m_mapH->onSelect = true;
			break;

		case 8:		// onTimer
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onTimer)
					DeleteFile(m_mapH->onTimerN);
				m_mapH->onTimerN[0] = '\0';
				m_mapH->onTimer = false;
				break;
			}
			if (m_mapH->onTimerN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onTimerN, prefix);
			}
			dir = m_mapH->onTimerN;
			m_mapH->onTimer = true;
			break;

		case 9:		// onFocus
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onFocus)
					DeleteFile(m_mapH->onFocusN);
				m_mapH->onFocusN[0] = '\0';
				m_mapH->onFocus = false;
				break;
			}
			if (m_mapH->onFocusN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onFocusN, prefix);
			}
			dir = m_mapH->onFocusN;
			m_mapH->onFocus = true;
			break;

		case 10:		// onClose
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onClose)
					DeleteFile(m_mapH->onCloseN);
				m_mapH->onCloseN[0] = '\0';
				m_mapH->onClose = false;
				break;
			}
			if (m_mapH->onCloseN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onCloseN, prefix);
			}
			dir = m_mapH->onCloseN;
			m_mapH->onClose = true;
			break;

		case 11:		// onDevice
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onDevice)
					DeleteFile(m_mapH->onDeviceN);
				m_mapH->onDeviceN[0] = '\0';
				m_mapH->onDevice = false;
				break;
			}
			if (m_mapH->onDeviceN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onDeviceN, prefix);
			}
			dir = m_mapH->onDeviceN;
			m_mapH->onDevice = true;
			break;

		case 12:		// onApprove
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onApprove)
					DeleteFile(m_mapH->onApproveN);
				m_mapH->onApproveN[0] = '\0';
				m_mapH->onApprove = false;
				break;
			}
			if (m_mapH->onApproveN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0'; 
				wccGetTempName(m_mapH->onApproveN, prefix);
			}
			dir = m_mapH->onApproveN;
			m_mapH->onApprove = true;
			break;
			
		case 13:		// onKey
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onKey)
					DeleteFile(m_mapH->onKeyN);
				m_mapH->onKeyN[0] = '\0';
				m_mapH->onKey = false;
				break;
			}
			if (m_mapH->onKeyN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0';
				wccGetTempName(m_mapH->onKeyN, prefix);
			}
			dir = m_mapH->onKeyN;
			m_mapH->onKey = true;
			break;

		case 35:		// onTimerX
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onTimerX)
					DeleteFile(m_mapH->onTimerXN);
				m_mapH->onTimerXN[0] = '\0';
				m_mapH->onTimerX = false;
				break;
			}
			if (m_mapH->onTimerXN[0] == '\0')
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = '\0';
				wccGetTempName(m_mapH->onTimerXN, prefix);
			}
			dir = m_mapH->onTimerXN;
			m_mapH->onTimerX = true;
			break;

		default:
			break;
		}
	
		if (sData.GetLength() > 0)
		{
			file = CreateFile(dir, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox("XError [d2]");
				return;
			}
			else
			{
				if (!WriteFile(file, (char *)sData.operator LPCTSTR(), sData.GetLength(), &nData, NULL))
					AfxMessageBox("XError [d3]");
				CloseHandle(file);
			}
		}
	}
	else if (m_index == -2)
	{
		HANDLE	file;
		DWORD	nData = 0;
		CString	dir = _T(""), sData = "";
		sData = GetEditScript();

		switch (m_prev)
		{
		case 1: // declaration
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onDeclaration)
					DeleteFile(m_mapH->onDeclarationN);
				m_mapH->onDeclarationN[0] = '\0';
				m_mapH->onDeclaration = false;
			}
			else
			{
				if (m_mapH->onDeclarationN[0] == '\0')
				{
					char	prefix[L_SGID+1];
					CopyMemory(prefix, m_mapH->mapN, L_SGID);
					prefix[L_SGID] = '\0';
					wccGetTempName(m_mapH->onDeclarationN, prefix);
				}
				dir = m_mapH->onDeclarationN;
				m_mapH->onDeclaration = true;
			}
			break;
		case 2: // include declaration
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onInDeclaration)
					DeleteFile(m_mapH->onInDeclarationN);
				m_mapH->onInDeclarationN[0] = '\0';
				m_mapH->onInDeclaration = false;
			}
			else
			{
				if (m_mapH->onInDeclarationN[0] == '\0')
				{
					char	prefix[L_SGID+1];
					CopyMemory(prefix, m_mapH->mapN, L_SGID);
					prefix[L_SGID] = '\0';
					wccGetTempName(m_mapH->onInDeclarationN, prefix);
				}
				dir = m_mapH->onInDeclarationN;
				m_mapH->onInDeclaration = true;
			}
			break;
		}

		if (sData.GetLength() > 0)
		{
			file = CreateFile(dir, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (file == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox("XError [d2]");
				return;
			}
			else
			{
				if (!WriteFile(file, (char *)sData.operator LPCTSTR(), sData.GetLength(), &nData, NULL))
					AfxMessageBox("XError [d3]");
				CloseHandle(file);
			}
		}
	}
	else if (m_index < m_pFormItem->getFormCount())
	{
		switch (m_prev)
		{
		case 1:		// click or change
			switch (m_pFormItem->getFormPoint(m_index)->kind)
			{
			case FM_EDIT:
			case FM_MEMO:
			case FM_COMBO:
				m_change = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onChangeL = m_change.GetLength();
				break;
			case FM_PANEL:
			case FM_OUT:
			case FM_LABEL:
			case FM_BUTTON:
			case FM_GRID:
			case FM_TAB:
			case FM_UTAB:
			case FM_TREE:
			case FM_TABLE:
			case FM_SHEET:
				m_click = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onClickL = m_click.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_pFormItem->getFormStr(m_index, "str");
					long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
					if (eventMask & CE_CLICK)
					{
						m_click = GetEditScript();
						m_pFormItem->getFormPoint(m_index)->onClickL = m_click.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;
			
		case 2:		// dblclk
			switch (m_pFormItem->getFormPoint(m_index)->kind)
			{
			case FM_EDIT:
			case FM_PANEL:
			case FM_OUT:
			case FM_LABEL:
			case FM_BUTTON:
			case FM_GRID:
			case FM_TAB:
			case FM_UTAB:
			case FM_TABLE:
				m_dblclk = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onDblClkL = m_dblclk.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_pFormItem->getFormStr(m_index, "str");
					long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
					if (eventMask & CE_DBLCLK)
					{
						m_dblclk = GetEditScript();
						m_pFormItem->getFormPoint(m_index)->onDblClkL = m_dblclk.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;
			
		case 3:		// char or change
			switch (m_pFormItem->getFormPoint(m_index)->kind)
			{
			case FM_EDIT:
				m_char = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onCharL = m_char.GetLength();
				break;
			case FM_GRID:
				m_change = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onChangeL = m_change.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_pFormItem->getFormStr(m_index, "str");
					long eventMask = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (long)sym.operator LPCTSTR());
					if (eventMask & CE_CHANGE)
					{
						m_change = GetEditScript();
						m_pFormItem->getFormPoint(m_index)->onChangeL = m_change.GetLength();
					}
				}
				break;
			}
			break;

		case 4:		// char
			switch (m_pFormItem->getFormPoint(m_index)->kind)
			{
			case FM_GRID:
				m_char = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onCharL = m_char.GetLength();
				break;
			}
			break;
		
		case 5:		// keychange
			switch (m_pFormItem->getFormPoint(m_index)->kind)
			{
			case FM_GRID:
				m_keychange = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onKeyL = m_keychange.GetLength();
				break;
			case FM_EDIT:
				m_keychange = GetEditScript();
				m_pFormItem->getFormPoint(m_index)->onKeyL = m_keychange.GetLength();
				break;
			}
			break;
		case 0:
		default:
			break;
		}
		
		// write file
		if (m_click.IsEmpty() && (m_pFormItem->getFormPoint(m_index)->onClickL == 0)
			&& m_dblclk.IsEmpty() && (m_pFormItem->getFormPoint(m_index)->onDblClkL == 0)
			&& m_change.IsEmpty() && (m_pFormItem->getFormPoint(m_index)->onChangeL == 0)
			&& m_char.IsEmpty() && (m_pFormItem->getFormPoint(m_index)->onCharL == 0)
			&& m_keychange.IsEmpty() && (m_pFormItem->getFormPoint(m_index)->onKeyL == 0))
		{
			DeleteFile(m_pFormItem->getFormStr(m_index, "scpN"));
			return;
		}
		
		if (m_pFormItem->getFormStr(m_index, "scpN").IsEmpty())
		{
			char	prefix[L_SGID+1], fileName[1024];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			wccGetTempName(fileName, prefix);
			m_pFormItem->setFormStr(m_index, "scpN", fileName);
		}
		
		HANDLE file = CreateFile(m_pFormItem->getFormStr(m_index, "scpN"), GENERIC_WRITE, FILE_SHARE_WRITE,
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox("XError [d1]");
			return;
		}
		
		DWORD	nWritten;
		int	offs = 0;
		if (m_pFormItem->getFormPoint(m_index)->onClickL != 0 && !m_click.IsEmpty())
		{
			nWritten = 0;
			if (WriteFile(file, m_click, m_click.GetLength(), &nWritten, NULL) != 1)
				MessageBox("Write Fail", "", MB_OK);
			else
			{
				m_pFormItem->getFormPoint(m_index)->onClick = offs;
				offs += nWritten;
			}
		}
		
		if (m_pFormItem->getFormPoint(m_index)->onDblClkL != 0 && !m_dblclk.IsEmpty())
		{
			nWritten = 0;
			if (WriteFile(file, m_dblclk, m_dblclk.GetLength(), &nWritten, NULL) != 1)
				MessageBox("Write Fail", "", MB_OK);
			else
			{
				m_pFormItem->getFormPoint(m_index)->onDblClk = offs;
				offs += nWritten;
			}
		}
		
		if (m_pFormItem->getFormPoint(m_index)->onChangeL != 0 && !m_change.IsEmpty())
		{
			nWritten = 0;
			if (WriteFile(file, m_change, m_change.GetLength(), &nWritten, NULL) != 1)
				MessageBox("Write Fail", "", MB_OK);
			else
			{
				m_pFormItem->getFormPoint(m_index)->onChange = offs;
				offs += nWritten;
			}
		}
		
		if (m_pFormItem->getFormPoint(m_index)->onCharL != 0 && !m_char.IsEmpty())
		{
			nWritten = 0;
			if (WriteFile(file, m_char, m_char.GetLength(), &nWritten, NULL) != 1)
				MessageBox("Write Fail", "", MB_OK);
			else
			{
				m_pFormItem->getFormPoint(m_index)->onChar = offs;
				offs += nWritten;
			}
		}

		if (m_pFormItem->getFormPoint(m_index)->onKeyL != 0 && !m_keychange.IsEmpty())
		{
			nWritten = 0;
			if (WriteFile(file, m_keychange, m_keychange.GetLength(), &nWritten, NULL) != 1)
				MessageBox("Write Fail", "", MB_OK);
			else
			{
				m_pFormItem->getFormPoint(m_index)->onKey = offs;
				offs += nWritten;
			}
		}
		
		CloseHandle(file);
	}
}

void CScriptBar::SelectControl(int selctl)
{
	if (m_pinBtn.GetCheck() || m_mapH->mapK == MK_PROCEDURES)
		return;

	if (selctl == -1 && m_bToDescription)
		selctl = -2;

	for (int ii = 0; ii < m_ctrllistCBO.GetCount(); ii++)
	{
		if ((int)m_ctrllistCBO.GetItemData(ii) == selctl)
		{
			if (m_ctrllistCBO.GetCurSel() == ii)
				break;

			m_ctrllistCBO.SetCurSel(ii);
			m_bRedraw = false;
			OnCLSelEndOk();
			m_bRedraw = true;
			break;
		}
	}
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);
}

void CScriptBar::setScript(char *dir)
{
	HANDLE	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;

	insertToEdit("");
	if (!m_editScript.IsWindowEnabled())
	{
		m_editScript.EnableWindow(TRUE);
		m_editScript.EnableScrollBar(SB_BOTH);
	}

	file = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		insertToEdit("");
		return;
	}

	scpSize = GetFileSize(file, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
		insertToEdit("");
		CloseHandle(file);
		return;
	}
	sbuf = new char[scpSize + 1];
	BOOL  rc = ReadFile(file, sbuf, scpSize, &dwRead, NULL);
	sbuf[scpSize] = '\0';
	CloseHandle(file);
	if (!rc || scpSize != dwRead)
	{
		delete[] sbuf;
		return;
	}

	insertToEdit(sbuf);
	m_editScript.EnableScrollBarCtrl(SB_BOTH);
	delete[] sbuf;
}

BOOL CScriptBar::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_CHAR)
	{
		if (GetFocus()->m_hWnd == m_editScript.m_hWnd)
		{
			m_editScript.SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetFocus()->m_hWnd == m_editScript.m_hWnd)
		{
			switch (pMsg->wParam)
			{
			case VK_RETURN:
			case VK_BACK:
			case VK_DELETE:
				m_editScript.SendMessage(WM_CHAR, pMsg->wParam);
				break;
			}
		}
	}

	switch(pMsg->message)
	{
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_KEYDOWN:
		{
			CPoint pos = m_editScript.GetCursorPos();
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSLINEINFO, MAKELPARAM(pos.y, pos.x));
		}
	}
	
	return CSizingControlBarG::PreTranslateMessage(pMsg);
}

void CScriptBar::OnPinBtnClick()
{
	if (m_pinBtn.GetCheck())
		m_pinBtn.SetCheck(false);
	else
		m_pinBtn.SetCheck(true);
}

void CScriptBar::OnPyBtnClick()
{
	if (!m_mapH) return;

	m_mapH->pythonMode = !m_mapH->pythonMode;
	m_pythonBtn.SetCheck(m_mapH->pythonMode);
	m_pythonBtn.SetWindowText(m_mapH->pythonMode ? "PY" : "VB");
	m_editScript.SetPythonMode(m_mapH->pythonMode);
	m_mapH->modified = true;
}

CString CScriptBar::getAllScript(_mapH *mapH, int *pLineNum, int *pIdx)
{
	if (mapH == NULL)
	{
		if (m_mapH == NULL)
			return "";
		else
			mapH = m_mapH;
	}

	CString sAllScript = "";
	CFormItem	formItem(mapH);
	int lineNum = (pLineNum?*pLineNum:-1), idx = -3, lineCount = -1;

	if (mapH->onDeclaration)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += getScript(mapH->onDeclarationN);
		sAllScript += "\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -80;
		}
	}

	if (mapH->onInDeclaration)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += getScript(mapH->onInDeclarationN);
		sAllScript += "\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -81;
		}
	}

	if (mapH->onStart)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnStart()\n";
		sAllScript += getScript(mapH->onStartN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -40;
		}
	}

	if (mapH->onSend)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnSend()\n";
		sAllScript += getScript(mapH->onSendN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -41;
		}
	}

	if (mapH->onReceive)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnReceive()\n";
		sAllScript += getScript(mapH->onReceiveN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -42;
		}
	}

	if (mapH->onAlert)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnAlert()\n";
		sAllScript += getScript(mapH->onAlertN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -43;
		}
	}

	if (mapH->onService)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnService()\n";
		sAllScript += getScript(mapH->onServiceN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -44;
		}
	}

	if (mapH->onFile)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnFile()\n";
		sAllScript += getScript(mapH->onFileN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -45;
		}
	}

	if (mapH->onSelect)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnSelect()\n";
		sAllScript += getScript(mapH->onSelectN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -46;
		}
	}

	if (mapH->onTimer)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnTimer()\n";
		sAllScript += getScript(mapH->onTimerN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -47;
		}
	}

	if (mapH->onTimerX)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnTimerX(id)\n";
		sAllScript += getScript(mapH->onTimerXN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -74;
		}
	}

	if (mapH->onFocus)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnFocus()\n";
		sAllScript += getScript(mapH->onFocusN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -48;
		}
	}

	if (mapH->onClose)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnClose()\n";
		sAllScript += getScript(mapH->onCloseN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -49;
		}
	}

	if (mapH->onDevice)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnDevice()\n";
		sAllScript += getScript(mapH->onDeviceN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -50;
		}
	}

	if (mapH->onApprove)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnApprove()\n";
		sAllScript += getScript(mapH->onApproveN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -51;
		}
	}

	if (mapH->onKey)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += "Sub OnKey\n";
		sAllScript += getScript(mapH->onKeyN);
		sAllScript += "\nEnd Sub\n";
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -52;
		}
	}

	for (int ii = 0; ii < formItem.getFormCount(); ii++)
	{
		if (!m_pFormItem->getFormStr(ii, "scpN").IsEmpty())
		{
			CString formScript = getScript(m_pFormItem->getFormPoint(ii)->scpN);
			if (formItem.getFormPoint(ii)->onClick != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.getFormPoint(ii)->name;
				sAllScript += "Sub " + symbol + "_Click\n";
				sAllScript += formScript.Left(formItem.getFormPoint(ii)->onClickL);
				sAllScript += "\nEnd Sub\n";
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 40;
				}
			}

			if (m_pFormItem->getFormPoint(ii)->onDblClk != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.getFormPoint(ii)->name;
				sAllScript += "Sub " + symbol + "_DblClick\n";
				sAllScript += formScript.Left(m_pFormItem->getFormPoint(ii)->onDblClkL);
				sAllScript += "\nEnd Sub\n";
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 40 + 1;
				}
			}

			if (m_pFormItem->getFormPoint(ii)->onChange != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.getFormPoint(ii)->name;
				sAllScript += "Sub " + symbol + "_Change\n";
				sAllScript += formScript.Left(m_pFormItem->getFormPoint(ii)->onChangeL);
				sAllScript += "\nEnd Sub\n";
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 40 + 2;
				}
			}

			if (m_pFormItem->getFormPoint(ii)->onChar != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.getFormPoint(ii)->name;
				sAllScript += "Sub " + symbol + "_CharChange\n";
				sAllScript += formScript.Left(m_pFormItem->getFormPoint(ii)->onCharL);
				sAllScript += "\nEnd Sub\n";
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 40 + 3;
				}
			}

			if (m_pFormItem->getFormPoint(ii)->onKey != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.getFormPoint(ii)->name;
				sAllScript += "Sub " + symbol + "_Key\n";
				sAllScript += formScript.Left(m_pFormItem->getFormPoint(ii)->onKeyL);
				sAllScript += "\nEnd Sub\n";
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 40 + 4;
				}
			}
		}
			
	}
	if (pLineNum && pIdx)
	{
		*pLineNum = lineNum;
		*pIdx = idx;
	}

	return sAllScript;
}

CString CScriptBar::getScript(char *dir)
{
	HANDLE	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;
	CString	retValue = "";

	file = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return "";

	scpSize = GetFileSize(file, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
		retValue.Empty();
		CloseHandle(file);
		return retValue;
	}

	sbuf = new char[scpSize + 1];
	BOOL  rc = ReadFile(file, sbuf, scpSize, &dwRead, NULL);
	sbuf[scpSize] = '\0';
	CloseHandle(file);
	if (!rc || scpSize != dwRead)
	{
		delete[] sbuf;
		return "";
	}

	retValue = sbuf;
	delete[] sbuf;
	return retValue;
}


long CScriptBar::getLineCount(CString str)
{
	if (str.IsEmpty())
		return 0;

	// Must count every physical line (including blank ones), because callers
	// compare this against real editor line numbers (CCrystalTextView line
	// indices / Ctrl+G line numbers), which also count blank lines. The
	// previous version skipped blank lines, so any script containing a blank
	// line desynced the block-boundary math in getAllScript() from the
	// actual editor line positions used by setLinePos().
	int nPos = 0, lineCount = 1;

	while ((nPos = str.Find('\n', nPos)) >= 0)
	{
		lineCount++;
		nPos++;
	}
	return lineCount;
}

void CScriptBar::setLinePos(int idx, int nEvent, int linePos)
{
	for (int ii = 0; ii < m_ctrllistCBO.GetCount(); ii++)
		if ((int)m_ctrllistCBO.GetItemData(ii) == idx)
		{
			m_ctrllistCBO.SetCurSel(ii);
			m_bRedraw = false;
			OnCLSelEndOk();
			for (int jj =0; jj < m_eventlistCBO.GetCount(); jj++)
			{
				if ((int)m_eventlistCBO.GetItemData(jj) == nEvent)
				{
					m_eventlistCBO.SetCurSel(jj);
					OnELSelEndOk();

					for (int vLine = 0; vLine <= linePos; vLine++)
					{
						CString txt = m_editScript.GetLineString(vLine);
						int nLength = txt.GetLength();
						txt.Remove('\r');
						txt.Remove('\t');
						txt.Remove(' ');
						if (txt.IsEmpty())
							linePos++;
						if (linePos >= m_editScript.GetLineCnt())
							break;
						m_editScript.SetSel(CPoint(0, vLine), CPoint(nLength, vLine));
					}
					break;	
				}
			}
			m_bRedraw = true;
			break;
		}
	m_editScript.SetFocus();
	//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);	
}

void CScriptBar::LoadAutoList()
{
	int nScriptID = 100;
	CMapStringToPtr mapFormID;
	mapFormID.SetAt("SYS",     (void*)FM_SYS);
	mapFormID.SetAt("LABEL",   (void*)FM_LABEL);
	mapFormID.SetAt("BOX",     (void*)FM_BOX);
	mapFormID.SetAt("GROUP",   (void*)FM_GROUP);
	mapFormID.SetAt("PANEL",   (void*)FM_PANEL);
	mapFormID.SetAt("BUTTON",  (void*)FM_BUTTON);
	mapFormID.SetAt("EDIT",    (void*)FM_EDIT);
	mapFormID.SetAt("COMBO",   (void*)FM_COMBO);
	mapFormID.SetAt("OUTPUT",  (void*)FM_OUT);
	mapFormID.SetAt("MEMO",    (void*)FM_MEMO);
	mapFormID.SetAt("GRID",    (void*)FM_GRID);
	mapFormID.SetAt("TABLE",   (void*)FM_TABLE);
	mapFormID.SetAt("TREE",    (void*)FM_TREE);
	mapFormID.SetAt("AVI",     (void*)FM_AVI);
	mapFormID.SetAt("GRAPH",   (void*)FM_GRAPH);
	mapFormID.SetAt("OBJECT",  (void*)FM_OBJECT);
	mapFormID.SetAt("TAB",     (void*)FM_TAB);
	mapFormID.SetAt("UTAB",    (void*)FM_UTAB);
	mapFormID.SetAt("SHEET",   (void*)FM_SHEET);
	mapFormID.SetAt("BROWSER", (void*)FM_BROWSER);
	mapFormID.SetAt("CONTROL", (void*)FM_CONTROL);
	mapFormID.SetAt("RADIO",   (void*)(FM_CONTROL + 10));
	mapFormID.SetAt("CHECK",   (void*)(FM_CONTROL + 11));

	CString path, info = "", sLevel = "CONTROLS";
	path.Format("%s\\%s\\public.ini", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	
	{
		CString dbg;
		dbg.Format("[AXISWORK][AUTOLIST][DEBUG] public.ini path=[%s] exists=%d\n",
			path.GetString(), (int)(GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES));
		OutputDebugString(dbg);
	}

	for (int ii = 1; ii < 100; ii++)
	{
		void *rtValue;
		int FormNum;
		CString tmps;

		tmps.Format("%02d", ii);
		tmps = getInfoFile(sLevel, tmps, "", path);
		tmps = Parser(tmps, ";");
		
		if (tmps.IsEmpty() && !sLevel.CompareNoCase("controls"))
		{
			sLevel = "public";
			ii = 0;
			continue;
		}
		else if (tmps.IsEmpty())
			break;

		if (!sLevel.CompareNoCase("controls") && !mapFormID.Lookup(tmps, rtValue))
		{
			if (tmps.IsEmpty())
				continue;
			else
			{
				FormNum = nScriptID;
				nScriptID++;
			}
		}
		else if (sLevel.CompareNoCase("controls"))
			FormNum = 200 + ii;
		else
			FormNum = (int)rtValue;

		info.Empty();

		for (int jj = 1; jj < 100; jj++)
		{
			CString tmpstr;

			tmpstr.Format("%02d", jj);
			tmpstr = getInfoFile(tmps, tmpstr, "", path);
			if (tmpstr.IsEmpty())
				break;
			
			info += tmpstr;
			info += '\t';
		}

		if (!info.IsEmpty())
		{
			m_editScript.m_AutoListMap.SetAt(FormNum, info);
			if (!sLevel.CompareNoCase("controls") && FormNum >= 100)
				m_editScript.m_ScriptStrMap.SetAt(tmps, (void*)FormNum);
			tmps.MakeUpper();
			if (sLevel.CompareNoCase("controls"))
				m_editScript.m_publicStrMap.SetAt(tmps, (void*)FormNum);
		}
	}

	mapFormID.RemoveAll();

	{
		void* rtValue;
		CString dbg;
		dbg.Format("[AXISWORK][AUTOLIST][DEBUG] loaded: ScriptStrMap has SCREEN=%d, AutoListMap count=%d\n",
			(int)m_editScript.m_ScriptStrMap.Lookup("SCREEN", rtValue), (int)m_editScript.m_AutoListMap.GetCount());
		OutputDebugString(dbg);
	}
}

CString CScriptBar::Parser(CString &srcstr, CString substr)
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

long CScriptBar::OnMessage100(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 1:	// script bold
		if (lParam)
		{
			int nCtrlList = m_ctrllistCBO.GetCurSel(),
				nEventList = m_eventlistCBO.GetCurSel();
			if (nCtrlList != CB_ERR)
				m_ctrllistCBO.SetItemBold(nCtrlList, true);
			if (nEventList != CB_ERR)
			{
				m_eventlistCBO.SetItemBold(nEventList, true);
				MainPostMsg(MAKEWPARAM(ID_USR_SCRIPTEVENT, TRUE), 
					MAKELPARAM(m_ctrllistCBO.GetItemData(nCtrlList), nEventList));
			}
		}
		else
		{
			int nCtrlList = m_ctrllistCBO.GetCurSel(),
				nEventList = m_eventlistCBO.GetCurSel();
			if (nEventList != CB_ERR)
			{
				m_eventlistCBO.SetItemBold(nEventList, false);
				MainPostMsg(MAKEWPARAM(ID_USR_SCRIPTEVENT, FALSE), 
					MAKELPARAM(m_ctrllistCBO.GetItemData(nCtrlList), nEventList));
			}
			if (nCtrlList != CB_ERR)
			{
				bool bBold = false;
				for (int nCnt = 0; nCnt < m_eventlistCBO.GetCount(); nCnt++)
					if (m_eventlistCBO.isItemBold(nCnt))
						bBold = true;
				if (!bBold)
					m_ctrllistCBO.SetItemBold(nCtrlList, false);
			}

		}
		break;
	}
	return 0;
}

CString CScriptBar::getInfoFile(CString sSection, CString sKey, CString sDefault, CString sPath)
{
	char twb[1024];
	GetPrivateProfileString(sSection, sKey, "", twb, sizeof(twb), sPath);
	return twb;
}

LRESULT CScriptBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDOWN || message == WM_CAPTURECHANGED) // window moving event?
		m_editScript.hidePopup();

	return CSizingControlBarG::WindowProc(message, wParam, lParam);
}

void CScriptBar::SelectEvent(int nEvent)
{
	if (m_eventlistCBO.GetCount() <= nEvent)
		return;
	if (!(GetStyle() & WS_VISIBLE))
		((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(this, true, FALSE);
	int nCtrl = m_ctrllistCBO.GetCurSel();
	if (nCtrl == 1)
	{
		if (nEvent <= 2)
		{
			nEvent--;
			m_ctrllistCBO.SetCurSel(0);
			OnCLSelEndOk();
		}
		else
			nEvent -= 2; 
	}
	m_eventlistCBO.SetCurSel(nEvent);
	OnELSelEndOk();
}

void CScriptBar::ReloadColors()
{
	m_editScript.LoadColors();
	m_editScript.Invalidate();
}
