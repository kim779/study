// scriptWnd.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "scriptWnd.h"

#include "h/keywords.h"
#include "h/mainvar.h"

#include "awWcc/libWcc.h"
#include "h\mapform.H"
#include "awTool/awObjectLoad.h"

#define	CE_CLICK	1
#define	CE_CHANGE	2
#define	CE_DBLCLK	4

#define ID_CTRLLIST	100
#define ID_EVENTLIST	101
#define ID_RICHEDIT	102
#define ID_PIN		103
#define ID_WHOLE	104
#define ID_LIST_MEMBER	105
#define ID_PYTHON	106

#define LISTVWIDTH	150
// CScriptWnd

IMPLEMENT_DYNAMIC(CScriptWnd, CDockablePane)

CScriptWnd::CScriptWnd()
{
	m_click       = _T("");
	m_dblclk      = _T("");
	m_change      = _T("");
	m_char        = _T("");
	m_keychange   = _T("");
	m_index       = -1;
	m_redraw     = true;
	m_bwhole = false;
	m_hPinBitmap  = NULL;
	m_hPinBitmap2 = NULL;

	m_toDescription = FALSE;
	m_spreObject.Empty();
	m_mapScript.RemoveAll();
}

CScriptWnd::~CScriptWnd()
{	
	::DeleteObject(m_hPinBitmap);
	::DeleteObject(m_hPinBitmap2);
	m_mapScript.RemoveAll();
	m_mapCtmEvent.RemoveAll();
	m_mapCtmFunc.RemoveAll();
}


BEGIN_MESSAGE_MAP(CScriptWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELENDOK(ID_CTRLLIST, OnCLSelEndOk)
	ON_CBN_SELENDOK(ID_EVENTLIST, OnELSelEndOk)
	ON_BN_CLICKED(ID_PIN, OnPinClick)
	ON_BN_CLICKED(ID_WHOLE, OnWholeClick)
	ON_BN_CLICKED(ID_PYTHON, OnPythonClick)
	ON_MESSAGE(WM_USER+100, OnMessage)
	ON_LBN_DBLCLK(ID_LIST_MEMBER, OnMemberDClick)
END_MESSAGE_MAP()


// CScriptWnd �޽��� ó�����Դϴ�.



BOOL CScriptWnd::PreTranslateMessage(MSG* pMsg)
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

	switch (pMsg->message)
	{
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_KEYDOWN:
		{
			CPoint	pos = m_editScript.GetCursorPos();
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSLINEINFO, MAKELPARAM(pos.y, pos.x));
		}
		break;
	}
	
	return CDockablePane::PreTranslateMessage(pMsg);
}


LRESULT CScriptWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_CAPTURECHANGED) // window moving event?
		m_editScript.hidePopup();

	return CDockablePane::WindowProc(message, wParam, lParam);
}


int CScriptWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_ctrlCB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS |CBS_SORT | CBS_OWNERDRAWFIXED ,CRect(0), this, ID_CTRLLIST))
		return -1;

	if (!m_eventCB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED, CRect(0), this, ID_EVENTLIST))
		return -1;

	if (!m_editScript.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0, 0, 0, 0), this, ID_RICHEDIT))
		return -1;

	m_editScript.EnableWindow(FALSE);

	if (!m_pinBtn.Create(NULL, _T("P"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0), this, ID_PIN))
		return -1;

	// Script Wnd UI����	
	if (!m_pwholeBtn.Create(NULL, _T("WHOLE"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_WHOLE))
		return -1;

	if (!m_pythonBtn.Create(NULL, _T("PY"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_PYTHON))
		return -1;

	if (!m_listMember.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_SORT | CBS_OWNERDRAWFIXED, CRect(0, 0, 0, 0), this, ID_LIST_MEMBER))
		return -1;

	m_listMember.SetColor(true);

	if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80, _T("MS Sans Serif")))
			return -1;

	LoadAutoList();

	m_ctrlCB.SetFont(&m_font);
	m_eventCB.SetFont(&m_font);
	m_pinBtn.SetFont(&m_font);
	m_pwholeBtn.SetFont(&m_font);
	m_pythonBtn.SetFont(&m_font);
	m_listMember.SetFont(&m_font);
	
	m_hPinBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PINNOPUSH));
	m_pinBtn.m_hBitmap = m_hPinBitmap;
	m_hPinBitmap2 = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PINPUSH));
	m_pinBtn.m_hBitmap2 = m_hPinBitmap2;

	HBITMAP hWholeBitmap, hWholeBitmap2;
	// Whole Image
	hWholeBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_ALL));
	m_pwholeBtn.m_hBitmap = hWholeBitmap;
	hWholeBitmap2 = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_ALL_DN));
	m_pwholeBtn.m_hBitmap2 = hWholeBitmap2;

	m_editScript.Initialize();

	CImageList* imglist = new CImageList;
	CBitmap	bitmap;

	bitmap.LoadBitmap(IDB_CONTROL);
	imglist->Create(17, 17, ILC_COLOR24|ILC_MASK, 26, 1);
	imglist->Add(&bitmap, RGB(0, 255, 255));
	imglist->Add(AfxGetApp()->LoadIcon(IDI_MAP));
	imglist->Add(AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	imglist->Add(AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	imglist->SetBkColor(CLR_NONE);
	m_ctrlCB.SetImageList(imglist);
	bitmap.Detach();

	imglist = new CImageList;
	imglist->Create(IDB_AUTOCMPLIMG, 12, 1, RGB(192, 192, 192));
	imglist->Add(&bitmap, RGB(192, 192, 192));
	m_listMember.SetImageList(imglist);

	return 0;
}


void CScriptWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	CRect	rc, cRc;

	GetClientRect(cRc);
	rc.SetRect(0, 1, 21, 19);
	m_pinBtn.MoveWindow(rc);

	rc.left = rc.right + 1;
	rc.right += 22;
	m_pwholeBtn.MoveWindow(rc);

	rc.left = rc.right + 1;
	rc.right += 24;
	m_pythonBtn.MoveWindow(rc);

	rc.top = 0;
	rc.left = rc.right;
	rc.right = cRc.Width()/2+5;
	rc.bottom = rc.top + 220;
	m_ctrlCB.MoveWindow(rc);
	m_ctrlCB.SetItemHeight(-1, 15);

	rc.left = rc.right;
	rc.right = cRc.right;
	m_eventCB.MoveWindow(rc);
	m_eventCB.SetItemHeight(-1, 15);

	rc.left = cRc.left + LISTVWIDTH;
	rc.right = cRc.right;
	rc.bottom = cRc.bottom;
	m_eventCB.GetClientRect(&cRc);
	rc.top = cRc.bottom;
	m_editScript.MoveWindow(rc);

	rc.left = 0;
	rc.right = LISTVWIDTH;
	m_listMember.MoveWindow(rc);
}

void CScriptWnd::OnELSelEndOk()
{
	if (!m_mapH || m_mapH->mapK == MK_PROCEDURES)
		return;

	if (m_index == -1)
	{
		WriteScript();
		m_prev = (int)m_eventCB.GetItemData(m_eventCB.GetCurSel());
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
		case 14:
			setScript(m_mapH->onChangeLayoutN);	break;
		case 15:
			setScript(m_mapH->onFlickingN);	break;
		default:
			insertToEdit(_T(""));
			m_editScript.EnableWindow(FALSE);
			break;
		}
		return;
	}
	else if (m_index == -2)
	{
		WriteScript();
		m_prev = (int)m_eventCB.GetItemData(m_eventCB.GetCurSel());
		switch(m_prev)
		{
		case 1:
			setScript(m_mapH->onDeclarationN);	break;
		case 2:
			setScript(m_mapH->onInDeclarationN);	break;
		}
		return;
	}

	CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
	if (!m_bwhole)
	{
		switch (m_prev)
		{
		case 1:		// click or change
			switch (form->kind)
			{
			case fmEDIT:
			case fmMEMO:
			case fmCOMBO:
				m_change = getEditScript();
				form->onChangeL = m_change.GetLength();
				break;
			case fmLABEL:
			case fmOUTPUT:
			case fmIMVIEW:
			case fmBUTTON:
			case fmRADIO:
			case fmCHECK:
			case fmGRID:
			case fmGRIDEX:
			case fmTAB:
			case fmTREE:
			case fmTABLE:
			case 0:
				//case FM_SHEET:
				m_click = getEditScript();
				form->onClickL = m_click.GetLength();
				break;
			case fmOBJECT:
			{
				CString sym = form->strings;
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
				if (eventMask & CE_CLICK)
				{
					m_click = getEditScript();
					form->onClickL = m_click.GetLength();
				}
			}
			break;

			default:
				break;
			}
			break;

		case 2:		// dblclk
			switch (form->kind)
			{
			case fmEDIT:
			case fmLABEL:
			case fmOUTPUT:
			case fmIMVIEW:
			case fmBUTTON:
			case fmRADIO:
			case fmCHECK:
			case fmGRID:
			case fmGRIDEX:
			case fmTAB:
			case fmTABLE:
				m_dblclk = getEditScript();
				form->onDblClkL = m_dblclk.GetLength();
				break;
			case fmOBJECT:
			{
				CString sym = form->strings;
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
				if (eventMask & CE_DBLCLK)
				{
					m_dblclk = getEditScript();
					form->onDblClkL = m_dblclk.GetLength();
				}
			}
			break;
			default:
				break;
			}
			break;

		case 3:		// char or change
			switch (form->kind)
			{
			case fmEDIT:
				m_char = getEditScript();
				form->onCharL = m_char.GetLength();
				break;

			case fmGRID:
			case fmGRIDEX:
				m_change = getEditScript();
				form->onChangeL = m_change.GetLength();
				break;
			case fmOBJECT:
			{
				CString sym = form->strings;
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
				if (eventMask & CE_CHANGE)
				{
					m_change = getEditScript();
					form->onChangeL = m_change.GetLength();
				}
			}
			break;
			}
			break;
		case 4:		// char
			switch (form->kind)
			{
			case fmGRID:
			case fmGRIDEX:
				m_char = getEditScript();
				form->onCharL = m_char.GetLength();
				break;
			}
			break;
		case 5:	// keychange
			switch (form->kind)
			{
			case fmGRID:
			case fmGRIDEX:
				m_keychange = getEditScript();
				form->onKeyL = m_keychange.GetLength();
				break;
			case fmEDIT:
				m_keychange = getEditScript();
				form->onKeyL = m_keychange.GetLength();
				break;
			}
			break;
		case 0:
		default:
			break;
		}
	}

	m_prev = (int)m_eventCB.GetItemData(m_eventCB.GetCurSel());
	switch (m_prev)
	{
	case 1:		// click or change
		switch (form->kind)
		{
		case fmEDIT:
		case fmMEMO:
		case fmCOMBO:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_change);
			break;
		case fmLABEL:
		case fmOUTPUT:
		case fmIMVIEW:
		case fmBUTTON:
		case fmRADIO:
		case fmCHECK:
		case fmGRID: 
		case fmGRIDEX: 
		case fmTAB:
		case fmTREE:
		case fmTABLE:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_click);
			break;
		case fmOBJECT:
			{
				CString sym = form->strings;
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
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
		switch (form->kind)
		{
		case fmEDIT:
		case fmLABEL:
		case fmOUTPUT:
		case fmIMVIEW:
		case fmBUTTON:
		case fmRADIO:
		case fmCHECK:
		case fmGRID: 
		case fmGRIDEX: 
		case fmTAB:
		case fmTABLE:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_dblclk);
			break;
		case fmOBJECT:
			{
				CString sym = form->strings;
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
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
		switch (form->kind)
		{
		case fmEDIT:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_char);
			break;
		case fmGRID:
		case fmGRIDEX:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_change);
			break;
		case fmOBJECT:
			{
				CString sym = form->strings;
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
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
		switch (form->kind)
		{
		case fmGRID:
		case fmGRIDEX:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_char);
			break;
		}
		break;

	case 5:		// keychange
		switch (form->kind)
		{
		case fmGRID:
		case fmGRIDEX:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_keychange);
			break;
		case fmEDIT:
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			insertToEdit(m_keychange);
			break;
		}
		break;
	case 0:
	default:
		insertToEdit(_T(""));
		m_editScript.EnableWindow(FALSE);
		break;
	}
}

void CScriptWnd::OnCLSelEndOk()
{
	if (!m_bwhole)
		WriteScript();
	if (m_ctrlCB.GetCurSel() != LB_ERR)
	{
		if (!m_bwhole)
		{
			m_index = (int)m_ctrlCB.GetItemData(m_ctrlCB.GetCurSel());
			m_click = _T("");
			m_dblclk = _T("");
			m_change = _T("");
			m_char = _T("");
			m_keychange = _T("");
			insertToEdit("");
			addEventList();
			if (m_redraw && m_index > -2)
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOCHILD, MAKELPARAM(m_index, SC_SCRIPTBAR));	// select control
		}
		CString sSymbol;
		m_ctrlCB.GetLBText(m_ctrlCB.GetCurSel(), sSymbol);
		int nPos = m_editScript.IsSymbol(sSymbol);

		showList(nPos, sSymbol);
	}
}

LRESULT CScriptWnd::OnMessage(WPARAM wp, LPARAM lp)
{
	switch (LOWORD(wp))
	{
	case 1:	// script bold
		if (lp)
		{
			int nCtrlList = m_ctrlCB.GetCurSel(), nEventList = m_eventCB.GetCurSel();
			if (nCtrlList != CB_ERR)
				m_ctrlCB.SetItemBold(nCtrlList, true);
			if (nEventList != CB_ERR)
			{
				m_eventCB.SetItemBold(nEventList, true);
				MainPostMsg(MAKEWPARAM(ID_USR_SCRIPTEVENT, TRUE), MAKELPARAM(m_ctrlCB.GetItemData(nCtrlList), nEventList));
			}
		}
		else
		{
			int nCtrlList = m_ctrlCB.GetCurSel(), nEventList = m_eventCB.GetCurSel();
			if (nEventList != CB_ERR)
			{
				m_eventCB.SetItemBold(nEventList, false);
				MainPostMsg(MAKEWPARAM(ID_USR_SCRIPTEVENT, FALSE), MAKELPARAM(m_ctrlCB.GetItemData(nCtrlList), nEventList));
			}
			if (nCtrlList != CB_ERR)
			{
				bool	bBold = false;
				for (int nCnt = 0; nCnt < m_eventCB.GetCount(); nCnt++)
					if (m_eventCB.IsItemBold(nCnt))
						bBold = true;
				if (!bBold)
					m_ctrlCB.SetItemBold(nCtrlList, false);
			}

		}
		break;
	}
	return 0;
}

void CScriptWnd::Initialize(class mapForm* mapForm)
{
	m_mapH = mapForm;
	m_editScript.Initialize(m_mapH);
	if (m_mapH && m_mapH->mapK == MK_PROCEDURES)
		m_editScript.EnableWindow(FALSE);
	m_editScript.SetPythonMode(m_mapH->pythonMode);
	m_pythonBtn.SetCheck(m_mapH->pythonMode);
	m_index = -1;
	m_prev = -1;
	if (m_mapH->onDeclaration || m_mapH->onInDeclaration)
		m_ctrlCB.SetItemBold(0, true);	// 0��°�� declaration
	if (m_mapH->onStart || m_mapH->onSend || m_mapH->onReceive || m_mapH->onAlert
		|| m_mapH->onService || m_mapH->onFile || m_mapH->onSelect || m_mapH->onTimer
		|| m_mapH->onFocus || m_mapH->onClose || m_mapH->onDevice || m_mapH->onApprove 
		|| m_mapH->onKey || m_mapH->onChangeLayout || m_mapH->onFlicking)
		m_ctrlCB.SetItemBold(1, true);	// 1��°�� form
	m_eventCB.ResetContent();
	m_eventCB.ResetItemInfo();
}

void CScriptWnd::AddComboString(CString sItem, int data, int type)
{
	if (type)
		m_eventCB.SetItemData(m_eventCB.AddString(sItem), data);
	else
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(data))->getProp();
		int idx;
		m_ctrlCB.SetItemData(idx = m_ctrlCB.AddString(sItem), data);
		if (data >= 0 && !form->scpN.IsEmpty())
			m_ctrlCB.SetItemBold(idx, true);
		if (data == m_index)
			m_ctrlCB.SetCurSel(idx);
		
		int nImage = 0;
		if ((int)data >= 0)
		{
			nImage = form->kind;
			//if (nImage == fmBUTTON)
			//{
			//	if (form->charType == BTN_RADIO)
			//		nImage = 21;
			//	else if (form->charType == BTN_CHECK)
			//		nImage = 22;
			//}
			//else if (nImage >= FM_GRAPH)
			//	nImage--;
		}
		else if ((int)data < 0)
			nImage = 23 + m_mapH->mapK;
		
		m_ctrlCB.SetItemImage(idx, nImage);
	}
}

void CScriptWnd::ReplaceComboString(CString sItem, int data)
{
	for (int ii = 0; ii < m_ctrlCB.GetCount(); ii++)
	{
		if ((int)m_ctrlCB.GetItemData(ii) == data)
		{
			m_ctrlCB.DeleteString(ii);
			AddComboString(sItem, data);
		}
	}
}

void CScriptWnd::ResetCombo(int type)
{
	if (type == 2)
	{
		m_eventCB.ResetContent();
		m_eventCB.ResetItemInfo();
	}
	else
	{
		m_ctrlCB.ResetContent();
		m_ctrlCB.ResetItemInfo();
		if (type == 1)
		{
			m_ctrlCB.SetItemData(m_ctrlCB.AddString(_T("_form_")), -1);
			m_ctrlCB.SetItemData(m_ctrlCB.AddString(_T("_declaration_")), -2);
		}
		else
			m_index = -3;

		m_mapH = NULL;
		m_editScript.Initialize(m_mapH);
	}
	insertToEdit(_T(""));
}

void CScriptWnd::LoadAutoList()
{
	int	nScriptID = 100;
	CMapStringToPtr mapFormID;

	//mapFormID.SetAt(_T("SYS"),     (void*)FM_SYS);
	mapFormID.SetAt(_T("LABEL"),   (void*)fmLABEL);
	mapFormID.SetAt(_T("BOX"),     (void*)fmBOX);
	mapFormID.SetAt(_T("GROUP"),   (void*)fmGROUP);
	mapFormID.SetAt(_T("PANEL"),   (void*)fmIMVIEW);
	mapFormID.SetAt(_T("BUTTON"),  (void*)fmBUTTON);
	mapFormID.SetAt(_T("EDIT"),    (void*)fmEDIT);
	mapFormID.SetAt(_T("COMBO"),   (void*)fmCOMBO);
	mapFormID.SetAt(_T("OUTPUT"),  (void*)fmOUTPUT);
	mapFormID.SetAt(_T("MEMO"),    (void*)fmMEMO);
	mapFormID.SetAt(_T("GRID"),    (void*)fmGRID);
	mapFormID.SetAt(_T("GRIDEX"),    (void*)fmGRIDEX);
	mapFormID.SetAt(_T("TABLE"),   (void*)fmTABLE);
	mapFormID.SetAt(_T("TREEVIEW"),    (void*)fmTREE);
	mapFormID.SetAt(_T("OBJECT"),  (void*)fmFORM);
	mapFormID.SetAt(_T("USRTAB"),     (void*)fmTAB);
	mapFormID.SetAt(_T("BROWSER"), (void*)fmBROWSER);
	mapFormID.SetAt(_T("CONTROL"),  (void*)fmOBJECT);
	mapFormID.SetAt(_T("RADIO"),   (void*)fmRADIO);
	mapFormID.SetAt(_T("CHECK"),   (void*)fmCHECK);

	void*	rtValue;
	int	FormNum;
	CString	tmps, tmpstr;
	CString path, info = _T(""), sLevel = _T("CONTROLS");
	path.Format(_T("%s\\%s\\public.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);

	for (int ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		tmps = getInfoFile(sLevel, tmps, _T(""), path);
		tmps = parse(tmps, _T(";"));
		
		if (tmps.IsEmpty() && !sLevel.CompareNoCase(_T("controls")))
		{
			sLevel = _T("public");
			ii = 0;
			continue;
		}
		else if (tmps.IsEmpty())
			break;

		if (!sLevel.CompareNoCase(_T("controls")) && !mapFormID.Lookup(tmps, rtValue))
		{
			if (tmps.IsEmpty())
				continue;
			else
			{
				FormNum = nScriptID;
				nScriptID++;
			}
		}
		else if (sLevel.CompareNoCase(_T("controls")))
			FormNum = 200 + ii;
		else
			FormNum = (int)rtValue;

		info.Empty();

		for (int jj = 1; jj < 100; jj++)
		{
			tmpstr.Format(_T("%02d"), jj);
			tmpstr = getInfoFile(tmps, tmpstr, _T(""), path);
			if (tmpstr.IsEmpty())
				break;
			
			info += tmpstr;
			info += _T('\t');
		}

		if (!info.IsEmpty())
		{
			m_editScript.m_AutoListMap.SetAt(FormNum, info);
			if (!sLevel.CompareNoCase(_T("controls")) && FormNum >= 100)
				m_editScript.m_ScriptStrMap.SetAt(tmps, (void *)FormNum);
			tmps.MakeUpper();
			if (sLevel.CompareNoCase(_T("controls")))
				m_editScript.m_publicStrMap.SetAt(tmps, (void *)FormNum);
		}
	}

	mapFormID.RemoveAll();
}

void CScriptWnd::SetLinePos(int idx, int nEvent, int linePos)
{
	for (int ii = 0; ii < m_ctrlCB.GetCount(); ii++)
	{
		if ((int)m_ctrlCB.GetItemData(ii) == idx)
		{
			m_ctrlCB.SetCurSel(ii);
			m_redraw = false;
			OnCLSelEndOk();
			for (int jj = 0; jj < m_eventCB.GetCount(); jj++)
			{
				if ((int)m_eventCB.GetItemData(jj) == nEvent)
				{
					m_eventCB.SetCurSel(jj);
					OnELSelEndOk();

					for (int vLine = 0; vLine <= linePos; vLine++)
					{
						CString txt = m_editScript.GetLineString(vLine);
						int nLength = txt.GetLength();
						txt.Remove(_T('\r'));
						txt.Remove(_T('\t'));
						txt.Remove(_T(' '));
						if (txt.IsEmpty())
							linePos++;
						if (linePos >= m_editScript.GetLineCnt())
							break;
						m_editScript.SetSel(CPoint(0, vLine), CPoint(nLength, vLine));
					}
					break;	
				}
			}
			m_redraw = true;
			break;
		}
	}
	m_editScript.SetFocus();
}

long CScriptWnd::getLineCount(CString str)
{
	if (str.IsEmpty())
		return 0;

	int	nBPos = 0, nPos = 0, lineCount = 1;
	CString	txt = _T("");

	while (nPos >= 0)
	{
		nPos = str.Find("\n", nPos + 1);
		if (nPos < 0)
			break;

		txt = str.Mid(nBPos, nPos - nBPos);
		txt.Remove(_T('\r'));
		txt.Remove(_T('\t'));
		txt.Remove(_T(' '));
		txt.Remove(_T('\n'));
		nBPos = nPos;
		if (nPos > 0 && txt.IsEmpty())
			continue;
		lineCount++;
	}
	return lineCount;
}

CString CScriptWnd::GetScript(CString dir)
{
	CFile	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;
	CString	retValue = _T("");

	if (!file.Open(dir, CFile::modeRead|CFile::typeBinary))
		return _T("");

	scpSize = (DWORD)file.GetLength();
	if (scpSize == 0xFFFFFFFF)
	{
		file.Close();
		return _T("");
	}

	sbuf = new char[scpSize + 1];
	dwRead = file.Read(sbuf, scpSize);
	sbuf[scpSize] = _T('\0');
	file.Close();
	if (scpSize != dwRead)
	{
		delete[] sbuf;
		return _T("");
	}

	retValue = sbuf;
	delete[] sbuf;
	return retValue;
}

CString CScriptWnd::GetAllScript(class mapForm* mapForm, int* pLineNum, int* pIdx)
{
	if (mapForm == NULL)
	{
		if (m_mapH == NULL)
			return _T("");
		else
			mapForm = m_mapH;
	}

	m_mapScript.RemoveAll();
	CString sAllScript = _T("");
	CString strTmp = _T("");
	int	lineNum = (pLineNum ? *pLineNum : -1), idx = -3, lineCount = -1;

	if (mapForm->onDeclaration)
	{
		lineCount = getLineCount(sAllScript);
		strTmp = GetScript(mapForm->onDeclarationN);
		sAllScript += _T("@@@ onDeclaration()\n");
		sAllScript += strTmp;
		sAllScript += _T("\n");
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ onDeclaration()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -26;
		}
	}

	if (mapForm->onInDeclaration)
	{
		lineCount = getLineCount(sAllScript);
		strTmp = GetScript(mapForm->onInDeclarationN);
		sAllScript += _T("@@@ onInDeclaration()\n");
		sAllScript += strTmp;
		sAllScript += _T("\n");
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ onInDeclaration()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -27;
		}
	}

	if (mapForm->onStart)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnStart()\n");
		strTmp = GetScript(mapForm->onStartN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnStart()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -13;
		}
	}

	if (mapForm->onSend)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnSend()\n");
		strTmp = GetScript(mapForm->onSendN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnSend()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -14;
		}
	}

	if (mapForm->onReceive)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnReceive()\n");
		strTmp = GetScript(mapForm->onReceiveN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnReceive()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -15;
		}
	}

	if (mapForm->onAlert)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnAlert()\n");
		strTmp = GetScript(mapForm->onAlertN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnAlert()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -16;
		}
	}

	if (mapForm->onService)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnService()\n");
		strTmp = GetScript(mapForm->onServiceN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnService()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -17;
		}
	}

	if (mapForm->onFile)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnFile()\n");
		strTmp = GetScript(mapForm->onFileN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnFile()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -18;
		}
	}

	if (mapForm->onSelect)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnSelect()\n");
		strTmp = GetScript(mapForm->onSelectN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnSelect()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -19;
		}
	}

	if (mapForm->onTimer)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnTimer()\n");
		strTmp = GetScript(mapForm->onTimerN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnTimer()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -20;
		}
	}

	if (mapForm->onFocus)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnFocus()\n");
		strTmp = GetScript(mapForm->onFocusN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnFocus()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -21;
		}
	}

	if (mapForm->onClose)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnClose()\n");
		strTmp = GetScript(mapForm->onCloseN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnClose()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -22;
		}
	}

	if (mapForm->onDevice)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnDevice()\n");
		strTmp = GetScript(mapForm->onDeviceN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnDevice()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -23;
		}
	}

	if (mapForm->onApprove)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnApprove()\n");
		strTmp = GetScript(mapForm->onApproveN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnApprove()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -24;
		}
	}

	if (mapForm->onKey)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnKey()\n");
		strTmp = GetScript(mapForm->onKeyN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnKey()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount ? 0 : 1);
			idx = -25;
		}
	}

	if (mapForm->onChangeLayout)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ onChangeLayout()\n");
		strTmp = GetScript(mapForm->onChangeLayoutN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ onChangeLayout()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount ? 0 : 1);
			idx = -25;
		}
	}

	if (mapForm->onFlicking)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ onFlicking()\n");
		strTmp = GetScript(mapForm->onFlickingN);
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ onFlicking()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount ? 0 : 1);
			idx = -25;
		}
	}

	//CString sKey;
	//CString sText;
	CString sScript;
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		if (!form->scpN.IsEmpty())
		{
			CString formScript = GetScript(form->scpN);
			if (!formScript.IsEmpty())
			{
				m_mapScript.SetAt(form->name, formScript);
			}
			if (form->onClick != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = form->name;
				sAllScript += _T("@@@ ") + symbol + _T("_Click\n");
				sScript = formScript.Left(form->onClickL);
				formScript = formScript.Mid(form->onClickL);
				sAllScript += sScript;
				sAllScript += LINESTRING;
				//sKey = _T("@@@ ") + symbol + _T("_Click");
				//m_mapScript.SetAt(sKey, sScript);
				//sText += sScript;
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13;
				}
			}

			if (form->onDblClk != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = form->name;
				sAllScript += _T("@@@ ") + symbol + _T("_DblClick\n");
				sScript = formScript.Left(form->onDblClkL);
				formScript = formScript.Mid(form->onDblClkL);
				sAllScript += sScript;
				sAllScript += LINESTRING;
				//sKey = _T("@@@ ") + symbol + _T("_DblClick");
				//m_mapScript.SetAt(sKey, sScript);
				//sText += sScript;
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 1;
				}
			}

			if (form->onChange != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = form->name;
				sAllScript += _T("@@@ ") + symbol + _T("_Change\n");
				sScript = formScript.Left(form->onChangeL);
				formScript = formScript.Mid(form->onChangeL);
				sAllScript += sScript;
				sAllScript += LINESTRING;
				//sKey = _T("@@@ ") + symbol + _T("_Change");
				//m_mapScript.SetAt(sKey, sScript);
				//sText += sScript;
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 2;
				}
			}

			if (form->onChar != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = form->name;
				sAllScript += _T("@@@ ") + symbol + _T("_CharChange\n");
				sScript = formScript.Left(form->onCharL);
				formScript = formScript.Mid(form->onCharL);
				sAllScript += sScript;
				sAllScript += LINESTRING;
				//sKey = _T("@@@ ") + symbol + _T("_CharChange");
				//m_mapScript.SetAt(sKey, sScript);
				//sText += sScript;
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 3;
				}
			}

			if (form->onKey != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = form->name;
				sAllScript += _T("@@@ ") + symbol + _T("_Key\n");
				sScript = formScript.Left(form->onKeyL);
				formScript = formScript.Mid(form->onKeyL);
				sAllScript += sScript;
				sAllScript += LINESTRING;
				//sKey = _T("@@@ ") + symbol + _T("_Key");
				//m_mapScript.SetAt(sKey, sScript);
				//sText += sScript;
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 4;
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

void CScriptWnd::OnPinClick()
{
	if (m_pinBtn.GetCheck())
		m_pinBtn.SetCheck(false);
	else
		m_pinBtn.SetCheck(true);
}

void CScriptWnd::SelectControl(int selctl)
{
	if (m_pinBtn.GetCheck() || m_mapH->mapK == MK_PROCEDURES)
		return;

	if (selctl == -1 && m_toDescription)
		selctl = -2;

	for (int ii = 0; ii < m_ctrlCB.GetCount(); ii++)
	{
		if ((int)m_ctrlCB.GetItemData(ii) == selctl)
		{
			if (m_ctrlCB.GetCurSel() == ii)
				break;

			m_ctrlCB.SetCurSel(ii);
			m_redraw = false;
			OnCLSelEndOk();
			m_redraw = true;
			break;
		}
	}
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);
}

void CScriptWnd::WriteScript()
{
	if (m_bwhole)
		return;

	if (!m_mapH)
		return;

	if (m_index < -2)
		return;
	
	CFile	file;
	CString	dir = _T(""), sData = _T("");
	if (m_index == -1)
	{

		sData = getEditScript();
		switch (m_prev)
		{
		case 1:		// onStart
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onStart)
					DeleteFile(m_mapH->onStartN);
				m_mapH->onStartN[0] = _T('\0');
				m_mapH->onStart = false;
				break;
			}
			if (m_mapH->onStartN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0');
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
				m_mapH->onSendN[0] = _T('\0');
				m_mapH->onSend = false;
				break;
			}
			if (m_mapH->onSendN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onReceiveN[0] = _T('\0');
				m_mapH->onReceive = false;
				break;
			}
			if (m_mapH->onReceiveN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onAlertN[0] = _T('\0');
				m_mapH->onAlert = false;
				break;
			}
			if (m_mapH->onAlertN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onServiceN[0] = _T('\0');
				m_mapH->onService = false;
				break;
			}
			if (m_mapH->onServiceN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onFileN[0] = _T('\0');
				m_mapH->onFile = false;
				break;
			}
			if (m_mapH->onFileN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onSelectN[0] = _T('\0');
				m_mapH->onSelect = false;
				break;
			}
			if (m_mapH->onSelectN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onTimerN[0] = _T('\0');
				m_mapH->onTimer = false;
				break;
			}
			if (m_mapH->onTimerN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onFocusN[0] = _T('\0');
				m_mapH->onFocus = false;
				break;
			}
			if (m_mapH->onFocusN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onCloseN[0] = _T('\0');
				m_mapH->onClose = false;
				break;
			}
			if (m_mapH->onCloseN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onDeviceN[0] = _T('\0');
				m_mapH->onDevice = false;
				break;
			}
			if (m_mapH->onDeviceN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onApproveN[0] = _T('\0');
				m_mapH->onApprove = false;
				break;
			}
			if (m_mapH->onApproveN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
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
				m_mapH->onKeyN[0] = _T('\0');
				m_mapH->onKey = false;
				break;
			}
			if (m_mapH->onKeyN[0] == _T('\0'))
			{
				char	prefix[L_SGID + 1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0');
				wccGetTempName(m_mapH->onKeyN, prefix);
			}
			dir = m_mapH->onKeyN;
			m_mapH->onKey = true;
			break;
		case 14:		// onChangeLayout
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onChangeLayout)
					DeleteFile(m_mapH->onChangeLayoutN);
				m_mapH->onChangeLayoutN[0] = _T('\0');
				m_mapH->onChangeLayout = false;
				break;
			}
			if (m_mapH->onChangeLayoutN[0] == _T('\0'))
			{
				char	prefix[L_SGID + 1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0');
				wccGetTempName(m_mapH->onChangeLayoutN, prefix);
			}
			dir = m_mapH->onChangeLayoutN;
			m_mapH->onChangeLayout = true;
			break;
		case 15:		// onFlicking
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onFlicking)
					DeleteFile(m_mapH->onFlickingN);
				m_mapH->onFlickingN[0] = _T('\0');
				m_mapH->onFlicking = false;
				break;
			}
			if (m_mapH->onFlickingN[0] == _T('\0'))
			{
				char	prefix[L_SGID + 1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0');
				wccGetTempName(m_mapH->onFlickingN, prefix);
			}
			dir = m_mapH->onFlickingN;
			m_mapH->onFlicking = true;
			break;
			
		default:
			break;
		}
	
		if (sData.GetLength() > 0)
		{
			if (file.Open(dir, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
			{
				file.Write((char *)sData.operator LPCTSTR(), sData.GetLength());
				file.Close();
			}
			else
			{
				AfxMessageBox("XError [d2]");
				return;
			}
		}
	}
	else if (m_index == -2)
	{
		sData = getEditScript();

		switch (m_prev)
		{
		case 1: // declaration
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onDeclaration)
					DeleteFile(m_mapH->onDeclarationN);
				m_mapH->onDeclarationN[0] = _T('\0');
				m_mapH->onDeclaration = false;
			}
			else
			{
				if (m_mapH->onDeclarationN[0] == _T('\0'))
				{
					char	prefix[L_SGID+1];
					CopyMemory(prefix, m_mapH->mapN, L_SGID);
					prefix[L_SGID] = _T('\0');
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
				m_mapH->onInDeclarationN[0] = _T('\0');
				m_mapH->onInDeclaration = false;
			}
			else
			{
				if (m_mapH->onInDeclarationN[0] == _T('\0'))
				{
					char	prefix[L_SGID+1];
					CopyMemory(prefix, m_mapH->mapN, L_SGID);
					prefix[L_SGID] = _T('\0');
					wccGetTempName(m_mapH->onInDeclarationN, prefix);
				}
				dir = m_mapH->onInDeclarationN;
				m_mapH->onInDeclaration = true;
			}
			break;
		}

		if (sData.GetLength() > 0)
		{
			if (file.Open(dir, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
			{
				file.Write((char *)sData.operator LPCTSTR(), sData.GetLength());
				file.Close();
			}
			else
			{
				AfxMessageBox("XError [d2]");
				return;
			}
		}
	}

	else if (m_index < m_mapH->forms.GetCount())
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
		switch (m_prev)
		{
		case 1:		// click or change
			switch (form->kind)
			{
			case fmEDIT:
			case fmMEMO:
			case fmCOMBO:
				m_change = getEditScript();
				form->onChangeL = m_change.GetLength();
				break;
			case fmIMVIEW:
			case fmOUTPUT:
			case fmLABEL:
			case fmBUTTON:
			case fmRADIO:
			case fmCHECK:
			case fmGRID:
			case fmGRIDEX:
			case fmTAB:
			case fmTREE:
			case fmTABLE:
			//case FM_SHEET:
				m_click = getEditScript();
				form->onClickL = m_click.GetLength();
				break;
			case fmOBJECT:
				{
					CString sym = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->strings;
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_CLICK)
					{
						m_click = getEditScript();
						form->onClickL = m_click.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;
			
		case 2:		// dblclk
			switch (form->kind)
			{
			case fmEDIT:
			case fmIMVIEW:
			case fmOUTPUT:
			case fmLABEL:
			case fmBUTTON:
			case fmRADIO:
			case fmCHECK:
			case fmGRID:
			case fmGRIDEX:
			case fmTAB:
			case fmTABLE:
				m_dblclk = getEditScript();
				form->onDblClkL = m_dblclk.GetLength();
				break;
			case fmOBJECT:
				{
				CString sym = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->strings;
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_DBLCLK)
					{
						m_dblclk = getEditScript();
						form->onDblClkL = m_dblclk.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;
			
		case 3:		// char or change
			switch (form->kind)
			{
			case fmEDIT:
				m_char = getEditScript();
				form->onCharL = m_char.GetLength();
				break;
			case fmGRID:
			case fmGRIDEX:
				m_change = getEditScript();
				form->onChangeL = m_change.GetLength();
				break;
			case fmOBJECT:
				{
					CString sym = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->strings;
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_CHANGE)
					{
						m_change = getEditScript();
						form->onChangeL = m_change.GetLength();
					}
				}
				break;
			}
			break;

		case 4:		// char
			switch (form->kind)
			{
			case fmGRID:
			case fmGRIDEX:
				m_char = getEditScript();
				form->onCharL = m_char.GetLength();
				break;
			}
			break;
		
		case 5:		// keychange
			switch (form->kind)
			{
			case fmGRID:
			case fmGRIDEX:
				m_keychange = getEditScript();
				form->onKeyL = m_keychange.GetLength();
				break;
			case fmEDIT:
				m_keychange = getEditScript();
				form->onKeyL = m_keychange.GetLength();
				break;
			}
			break;
		case 0:
		default:
			break;
		}
		
		// write file
		if (m_click.IsEmpty() && (form->onClickL == 0)
			&& m_dblclk.IsEmpty() && (form->onDblClkL == 0)
			&& m_change.IsEmpty() && (form->onChangeL == 0)
			&& m_char.IsEmpty() && (form->onCharL == 0)
			&& m_keychange.IsEmpty() && (form->onKeyL == 0))
		{
			DeleteFile(((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->scpN);
			return;
		}
		
		if (((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->scpN.IsEmpty())
		{
			char	prefix[L_SGID+1], fileName[1024];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = _T('\0'); 
			wccGetTempName(fileName, prefix);
			((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->scpN = fileName;
		}
	
		if (!file.Open(((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->scpN, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			AfxMessageBox("XError [d1]");
			return;
		}
		
		DWORD	nWritten;
		int	offs = 0;
		if (form->onClickL != 0 && !m_click.IsEmpty())
		{
			nWritten = m_click.GetLength();
			file.Write(m_click, nWritten);

			form->onClick = offs;
			offs += nWritten;
		}
		
		if (form->onDblClkL != 0 && !m_dblclk.IsEmpty())
		{
			nWritten = m_dblclk.GetLength();
			file.Write(m_dblclk, nWritten);

			form->onDblClk = offs;
			offs += nWritten;
		}
		
		if (form->onChangeL != 0 && !m_change.IsEmpty())
		{
			nWritten = m_change.GetLength();
			file.Write(m_change, nWritten);

			form->onChange = offs;
			offs += nWritten;
		}
		
		if (form->onCharL != 0 && !m_char.IsEmpty())
		{
			nWritten = m_char.GetLength();
			file.Write(m_char, nWritten);

			form->onChar = offs;
			offs += nWritten;
		}

		if (form->onKeyL != 0 && !m_keychange.IsEmpty())
		{
			nWritten = m_keychange.GetLength();
			file.Write(m_keychange, nWritten);

			form->onKey = offs;
			offs += nWritten;
		}
		file.Close();		
	}
}

void CScriptWnd::ReloadColors()
{
	m_editScript.LoadColors();
	m_editScript.Invalidate();
}

void CScriptWnd::SelectEvent(int nEvent)
{
	if (m_eventCB.GetCount() <= nEvent)
		return;

//	if (!(GetStyle() & WS_VISIBLE))
//		((CFrameWnd*)AfxGetMainWnd())->ShowControlBar(this, true, FALSE);

	int nCtrl = m_ctrlCB.GetCurSel();
	if (nCtrl == 1)
	{
		if (nEvent <= 2)
		{
			nEvent--;
			m_ctrlCB.SetCurSel(0);
			OnCLSelEndOk();
		}
		else
			nEvent -= 2; 
	}
	m_eventCB.SetCurSel(nEvent);
	OnELSelEndOk();
}

void CScriptWnd::SetCtmObjFunc(DWORD idx, CString ctmFunc)
{
	m_mapCtmFunc.SetAt(idx, ctmFunc);
	m_editScript.m_AutoListCntObj.SetAt(idx, ctmFunc);
}

void CScriptWnd::SetCtmObjEvent(DWORD idx, CString ctmEvent)
{
	m_mapCtmEvent.SetAt(idx, ctmEvent);
}

void CScriptWnd::AddCtmObjEvent()
{
	CString strList(""), strTmp("");
	if (!m_mapCtmEvent.Lookup(((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->iCtmIdx, strList))
		return;
	int cnt = 1;
	while (!strList.IsEmpty())
	{
		strTmp = parse(strList, "|");
		if (strTmp == "None")
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		}
		else if (strTmp == "OnClick")
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnClick")), cnt++);
			if (/*isScript && */((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onClick != -1) m_eventCB.SetItemBold(1, true);
		}
		else if (strTmp == "OnDoubleClick")
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnDoubleClick")), cnt++);
			if (/*isScript && */((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onDblClk != -1) m_eventCB.SetItemBold(2, true);
		}
		else if (strTmp == "OnChange")
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), cnt++);
			if (/*isScript && */((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChange != -1) m_eventCB.SetItemBold(3, true);
		}
		else if (strTmp == "OnCharChange")
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnCharChange")), cnt++);
			if (/*isScript && */((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChar != -1) m_eventCB.SetItemBold(4, true);
		}
		else if (strTmp == "OnKey")
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnKey")), cnt++);
			if (/*isScript && */((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onKey != -1) m_eventCB.SetItemBold(5, true);
		}
	}
}

void CScriptWnd::addEventList()
{
	if (!m_mapH)
		return;

	int	kind = -1;
	bool	isScript = false;

	ResetCombo(2);
	m_editScript.EnableWindow(TRUE);
	m_editScript.EnableScrollBar(SB_BOTH);

	if (m_index >= 0)
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
		kind = form->kind;
		if (!form->scpN.IsEmpty())
		{
			isScript = true;
			m_ctrlCB.SetItemBold(m_ctrlCB.GetCurSel(), true);
		}
	}
	else
	{
		kind = m_index;
		if (m_index == -2 && (m_mapH->onDeclaration || m_mapH->onInDeclaration))
			m_ctrlCB.SetItemBold(m_ctrlCB.GetCurSel(), true);
		if (m_index == -1 && 
			(m_mapH->onStart || m_mapH->onSend || m_mapH->onReceive || m_mapH->onAlert
			|| m_mapH->onService || m_mapH->onFile || m_mapH->onSelect || m_mapH->onTimer
			|| m_mapH->onFocus || m_mapH->onClose || m_mapH->onDevice || m_mapH->onApprove 
			|| m_mapH->onKey) || m_mapH->onChangeLayout || m_mapH->onFlicking)
			m_ctrlCB.SetItemBold(m_ctrlCB.GetCurSel(), true);		
	}

	switch (kind)
	{
	case -2:	// declaration
		if (m_mapH->mapK != MK_PROCEDURES)
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("Declaration")), 1);
			if (m_mapH->onDeclaration) m_eventCB.SetItemBold(0, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("include Declaration")), 2);
			if (m_mapH->onInDeclaration) m_eventCB.SetItemBold(1, true);
		}
		else
			m_editScript.EnableWindow(FALSE);
		break;

	case -1:	// form
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		if (m_mapH->mapK != MK_PROCEDURES)
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnStart")), 1);
			if (m_mapH->onStart) m_eventCB.SetItemBold(1, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnSend")), 2);
			if (m_mapH->onSend) m_eventCB.SetItemBold(2, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnReceive")), 3);
			if (m_mapH->onReceive) m_eventCB.SetItemBold(3, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnAlert(code, data, state)")), 4);
			if (m_mapH->onAlert) m_eventCB.SetItemBold(4, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnService(data, length)")), 5);
			if (m_mapH->onService) m_eventCB.SetItemBold(5, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnFile(upload, data, length)")), 6);
			if (m_mapH->onFile) m_eventCB.SetItemBold(6, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnSelect(data)")), 7);
			if (m_mapH->onSelect) m_eventCB.SetItemBold(7, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnTimer")), 8);
			if (m_mapH->onTimer) m_eventCB.SetItemBold(8, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnFocus")), 9);
			if (m_mapH->onFocus) m_eventCB.SetItemBold(9, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnClose")), 10);
			if (m_mapH->onClose) m_eventCB.SetItemBold(10, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnDevice")), 11);
			if (m_mapH->onDevice) m_eventCB.SetItemBold(11, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnApprove(key, data, length)")), 12);
			if (m_mapH->onApprove) m_eventCB.SetItemBold(12, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnKey")), 13);
			if (m_mapH->onKey) m_eventCB.SetItemBold(13, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChangeLayout()")), 14);
			if (m_mapH->onChangeLayout) m_eventCB.SetItemBold(14, true);
			if (m_mapH->media == MEDIA_MTS)
			{
				m_eventCB.SetItemData(m_eventCB.AddString(_T("onFlickingN()")), 15);
				if (m_mapH->onFlicking) m_eventCB.SetItemBold(15, true);
			}
		}
		else
			m_editScript.EnableWindow(FALSE);
		break;
	case fmEDIT:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 1);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChange != -1) m_eventCB.SetItemBold(1, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnCharChange")), 3);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChar != -1) m_eventCB.SetItemBold(2, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnKey")), 5);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onKey != -1) m_eventCB.SetItemBold(3, true);
		break;

	case fmMEMO:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 1);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChange != -1) m_eventCB.SetItemBold(1, true);
		break;

	case fmCOMBO:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 1);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChange != -1) m_eventCB.SetItemBold(1, true);
		break;

	case fmGRID:
	case fmGRIDEX:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnClick")), 1);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onClick != -1) m_eventCB.SetItemBold(1, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnDoubleClick")), 2);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onDblClk != -1) m_eventCB.SetItemBold(2, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 3);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChange != -1) m_eventCB.SetItemBold(3, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnCharChange")), 4);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onChar != -1) m_eventCB.SetItemBold(4, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnKey")), 5);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onKey != -1) m_eventCB.SetItemBold(5, true);
		break;

	case fmLABEL:
	case fmOUTPUT:
	case fmIMVIEW:
	case fmBUTTON:
	case fmRADIO:
	case fmCHECK:
	case fmTAB:
	case fmTABLE:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnClick")), 1);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onClick != -1) m_eventCB.SetItemBold(1, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnDoubleClick")), 2);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onDblClk != -1) m_eventCB.SetItemBold(2, true);
		break;

	case fmTREE:
	//case FM_SHEET:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnClick")), 1);
		if (isScript && ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->onClick != -1) m_eventCB.SetItemBold(1, true);
		break;
	case fmOBJECT:
		{
			AddCtmObjEvent();
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
			m_eventCB.SetCurSel(1);
			setScript(m_mapH->onStartN);
			set = true;
		}
		
		if (!set && m_mapH->onSend)
		{
			m_eventCB.SetCurSel(2);
			setScript(m_mapH->onSendN);
			set = true;
		}
		
		if (!set && m_mapH->onReceive)
		{
			m_eventCB.SetCurSel(3);
			setScript(m_mapH->onReceiveN);
			set = true;
		}

		if (!set && m_mapH->onAlert)
		{
			m_eventCB.SetCurSel(4);
			setScript(m_mapH->onAlertN);
			set = true;
		}

		if (!set && m_mapH->onService)
		{
			m_eventCB.SetCurSel(5);
			setScript(m_mapH->onServiceN);
			set = true;
		}

		if (!set && m_mapH->onFile)
		{
			m_eventCB.SetCurSel(6);
			setScript(m_mapH->onFileN);
			set = true;
		}

		if (!set && m_mapH->onSelect)
		{
			m_eventCB.SetCurSel(7);
			setScript(m_mapH->onSelectN);
			set = true;
		}

		if (!set && m_mapH->onTimer)
		{
			m_eventCB.SetCurSel(8);
			setScript(m_mapH->onTimerN);
			set = true;
		}

		if (!set && m_mapH->onFocus)
		{
			m_eventCB.SetCurSel(9);
			setScript(m_mapH->onFocusN);
			set = true;
		}

		if (!set && m_mapH->onClose)
		{
			m_eventCB.SetCurSel(10);
			setScript(m_mapH->onCloseN);
			set = true;
		}

		if (!set && m_mapH->onDevice)
		{
			m_eventCB.SetCurSel(11);
			setScript(m_mapH->onDeviceN);
			set = true;
		}

		if (!set && m_mapH->onApprove)
		{
			m_eventCB.SetCurSel(12);
			setScript(m_mapH->onApproveN);
			set = true;
		}
		
		if (!set && m_mapH->onKey)
		{
			m_eventCB.SetCurSel(13);
			setScript(m_mapH->onKeyN);
			set = true;
		}

		if (!set && m_mapH->onChangeLayout)
		{
			m_eventCB.SetCurSel(14);
			setScript(m_mapH->onChangeLayoutN);
			set = true;
		}

		if (!set && m_mapH->onFlicking)
		{
			m_eventCB.SetCurSel(15);
			setScript(m_mapH->onFlickingN);
			set = true;
		}
		
		if (!set)
		{
			m_eventCB.SetCurSel(0);
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
			m_mapH->onDevice  = false;
			m_mapH->onApprove = false;
			m_mapH->onKey     = false;
			m_mapH->onChangeLayout = false;
			m_mapH->onFlicking = false;
		}
		else
		{
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
		}
		
		m_prev = (long)m_eventCB.GetItemData(m_eventCB.GetCurSel());
		return;
	}

	if (m_index == -2)
	{
		bool	set = false;

		if (!set && m_mapH->onDeclaration)
		{
			m_eventCB.SetCurSel(0);
			setScript(m_mapH->onDeclarationN);
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			set = true;
		}

		if (!set && m_mapH->onInDeclaration)
		{
			m_eventCB.SetCurSel(1);
			setScript(m_mapH->onInDeclarationN);
			m_editScript.EnableWindow(TRUE);
			m_editScript.EnableScrollBar(SB_BOTH);
			set = true;
		}

		if (!m_eventCB.GetCount())
		{
			m_editScript.EnableWindow(FALSE);
			m_prev = -1;
			return;
		}

		if (!set)
			m_eventCB.SetCurSel(0);

		m_prev = (long)m_eventCB.GetItemData(m_eventCB.GetCurSel());
		return;
	}
	//((CformProp *)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp())->scpN()
	//if (((CformProp *)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp())->scpN.IsEmpty())
	//((CformProp *)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp())->scpN;
	if (((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->scpN.IsEmpty())
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return;
	}

	CFile	file;

	if (!file.Open(((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp()->scpN, CFile::modeRead | CFile::typeBinary))
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return ;
	}

	DWORD	scpSize = (DWORD)file.GetLength();
	if (scpSize <= 0)
	{
		file.Close();
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return ;
	}

	char*	scpAlloc;
	scpAlloc = new char[scpSize+1];

	DWORD	dwReadLen = file.Read(scpAlloc, scpSize);
	file.Close();
	if (scpSize != dwReadLen)
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		delete[] scpAlloc;
		return ;
	}
	CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp();
	if (form->onClick != -1)
	{
		m_click = CString(&scpAlloc[form->onClick], form->onClickL);
		m_click += _T('\0');
	}

	if (form->onDblClk != -1)
	{
		m_dblclk = CString(&scpAlloc[form->onDblClk], form->onDblClkL);
		m_dblclk += _T('\0');
	}

	if (form->onChange != -1)
	{
		m_change = CString(&scpAlloc[form->onChange], form->onChangeL);
		m_change += _T('\0');
	}

	if (form->onChar != -1)
	{
		m_char = CString(&scpAlloc[form->onChar], form->onCharL);
		m_char += _T('\0');
	}

	if (form->onKey != -1)
	{
		m_keychange = CString(&scpAlloc[form->onKey], form->onKeyL);
		m_keychange += _T('\0');
	}

	delete[] scpAlloc;

	bool	set = false;
	int	nKind = form->kind;
	if (!set && !m_click.IsEmpty() && nKind != fmCOMBO && nKind != fmEDIT)
	{
		m_eventCB.SetCurSel(1);
		insertToEdit(m_click);
		set = true;
	}

	if (!set && !m_change.IsEmpty()
		&& (nKind == fmCOMBO || nKind == fmEDIT || nKind == fmGRID || nKind == fmGRIDEX || nKind == fmMEMO))
	{
		m_eventCB.SetCurSel((form->kind == fmGRID || form->kind == fmGRIDEX) ? 3 : 1);
		insertToEdit(m_change);
		set = true;
	}

	if (!set && !m_dblclk.IsEmpty() && nKind != fmCOMBO)
	{
		if (m_eventCB.FindString(0, _T("OnDoubleClick")) != CB_ERR)
			m_eventCB.SetCurSel(m_eventCB.FindString(0, _T("OnDoubleClick")));
		insertToEdit(m_dblclk);
		set = true;
	}

	if (!set && !m_char.IsEmpty() && (nKind == fmEDIT || nKind == fmGRID || nKind == fmGRIDEX))
	{
		if (m_eventCB.FindString(0, _T("OnCharChange")) != CB_ERR)
			m_eventCB.SetCurSel(m_eventCB.FindString(0, _T("OnCharChange")));
		insertToEdit(m_char);
		set = true;
	}

	if (!set && !m_keychange.IsEmpty() && (nKind == fmGRID || nKind == fmGRIDEX || nKind == fmEDIT))
	{
		if (m_eventCB.FindString(0, _T("OnKey")) != CB_ERR)
			m_eventCB.SetCurSel(m_eventCB.FindString(0, _T("OnKey")));
		insertToEdit(m_keychange);
		set = true;
	}

	if (!set)
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
	}

	m_prev = (long)m_eventCB.GetItemData(m_eventCB.GetCurSel());
	m_editScript.SetFocus();
}

void CScriptWnd::setScript(char* dir)
{
	CFile	file;
	DWORD	dwRead;
	DWORD	scpSize;
	char*	sbuf;

	insertToEdit(_T(""));
	if (!m_editScript.IsWindowEnabled())
	{
		m_editScript.EnableWindow(TRUE);
		m_editScript.EnableScrollBar(SB_BOTH);
	}

	if (!file.Open(dir, CFile::modeRead|CFile::typeBinary))
		return;

	scpSize = (DWORD)file.GetLength();
	if (scpSize <= 0)
	{
		file.Close();
		return;
	}
	sbuf = new char[scpSize + 1];
	dwRead = file.Read(sbuf, scpSize);
	sbuf[scpSize] = _T('\0');
	file.Close();
	if (scpSize != dwRead)
	{
		delete[] sbuf;
		return;
	}

	insertToEdit(sbuf);
	m_editScript.EnableScrollBarCtrl(SB_BOTH);
	delete[] sbuf;
}

CString CScriptWnd::getEditScript()
{
	return m_editScript.GetScriptText();
}

void CScriptWnd::insertToEdit(CString str)
{
	m_editScript.SetScriptText(str);
}

CString CScriptWnd::getInfoFile(CString sSection, CString sKey, CString sDefault, CString sPath)
{
	char	wb[1024];

	GetPrivateProfileString(sSection, sKey, _T(""), wb, sizeof(wb), sPath);
	return wb;
}

CString CScriptWnd::parse(CString& src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);
	
	if (pos == -1)
	{
		tmps = src;
		src  = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src  = src.Mid(pos + sub.GetLength());
	return tmps;
}

void CScriptWnd::OnWholeClick()
{
	if (m_ctrlCB.GetCount() <= 0)
		return;

	//	m_ctrlCB.EnableWindow(m_bwhole ? TRUE : FALSE);
	m_eventCB.EnableWindow(m_bwhole ? TRUE : FALSE);

	if (m_bwhole)
	{
		SetAllScript(m_mapH);
		OnELSelEndOk();
	}
	else
	{
		m_ctrlCB.GetWindowText(m_spreObject);
		WriteScript();
	}

	m_bwhole = !m_bwhole;

	if (m_bwhole)
	{
		m_pwholeBtn.SetCheck(true);
		setWholeScript();
	}
	else
	{
		m_pwholeBtn.SetCheck(false);
		m_ctrlCB.SelectString(0, m_spreObject);
	}
	m_editScript.EnableWindow(TRUE);
}

// ��ü script����
void CScriptWnd::setWholeScript()
{
	CString str;
	m_bwhole = true;
	str = GetAllScript(m_mapH);
	str.Replace(_T("\r\n"), _T("\n"));

	insertToEdit(str);
}

void CScriptWnd::SetAllScript(class mapForm* mapH)
{
	CFile	file;
	CString sEvent, sData, allScript, sSymbol, ObjName, sline, sMap, tmpData;
	CString sKey;
	CString	dir = _T("");
	sline = LINESTRING;
	sline.Remove('\n');

	allScript = getEditScript();
	int iPos = 0;

	CMap<CString, LPCTSTR, CString, LPCTSTR>  map;

	CMapStringToString mapNewScript;
	while (allScript.GetLength() > 1)
	{
		int len = allScript.GetLength();

		iPos = allScript.Find(sline);
		sData = allScript.Left(iPos - 2);
		allScript = allScript.Mid(iPos + strlen(sline) + 2);

		sData.Replace("\r\n", "\n");
		iPos = sData.Find("\n");
		sEvent = sData.Left(iPos);
		sEvent.Replace("@@@ ", "");

		int iFind = sEvent.Find(_T("_"));
		if (iFind > 0)
		{
			sEvent = sEvent.Left(iFind);
		}
		sData = sData.Mid(iPos + 1);
		
		if (mapNewScript.Lookup(sEvent, tmpData))
		{
			tmpData += _T("\n");
			tmpData += sData;
			mapNewScript.SetAt(sEvent, tmpData);
		}
		else
			mapNewScript.SetAt(sEvent, sData);

		if (sData.IsEmpty())
			continue;
	}

	CformProp* prop = NULL;
	BOOL bRet = FALSE;
	CString strScript(""), strData("");
	char*	sbuf = NULL;
	int off = 0;


	if (mapNewScript.Lookup("onDeclaration()", tmpData))
	{
		m_mapH->onDeclaration = true;
		dir = m_mapH->onDeclarationN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onInDeclaration()", tmpData))
	{
		mapH->onInDeclaration = true;
		dir = m_mapH->onInDeclarationN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onStart()", tmpData))
	{
		mapH->onStart = true;
		dir = m_mapH->onStartN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnSend()", tmpData))
	{
		mapH->onSend = true;
		dir = m_mapH->onSendN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnReceive()", tmpData))
	{
		mapH->onReceive = true;
		dir = m_mapH->onReceiveN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnAlert()", tmpData))
	{
		mapH->onAlert = true;
		dir = m_mapH->onAlertN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onService()", tmpData))
	{
		mapH->onService = true;
		dir = m_mapH->onServiceN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onFile()", tmpData))
	{
		mapH->onFile = true;
		dir = m_mapH->onFileN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onSelect()", tmpData))
	{
		mapH->onSelect = true;
		dir = m_mapH->onSelectN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnTimer()", tmpData))
	{
		mapH->onTimer = true;
		dir = m_mapH->onTimerN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onFocus()", tmpData))
	{
		mapH->onFocus = true;
		dir = m_mapH->onFocusN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnClose()", tmpData))
	{
		mapH->onClose = true;
		dir = m_mapH->onCloseN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnDevice()", tmpData))
	{
		mapH->onDevice = true;
		dir = m_mapH->onDeviceN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onApprove()", tmpData))
	{
		mapH->onApprove = true;
		dir = m_mapH->onApproveN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onKey()", tmpData))
	{
		mapH->onKey = true;
		dir = m_mapH->onKeyN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("OnChangeLayout()", tmpData))
	{
		mapH->onChangeLayout = true;
		dir = m_mapH->onChangeLayoutN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onFlicking()", tmpData))
	{
		mapH->onFlicking = true;
		dir = m_mapH->onFlickingN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}
	if (mapNewScript.Lookup("onTrigger()", tmpData))
	{
		mapH->onTrigger = true;
		dir = m_mapH->onTriggerN;
		if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)tmpData.operator LPCTSTR(), tmpData.GetLength());
			file.Close();
		}
	}

	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		bRet = mapNewScript.Lookup(prop->name, tmpData);
		if (bRet != TRUE) continue;

		strData = _T("");
		off = 0;
		strScript = tmpData;
		if (prop->onClick != -1)
		{
			prop->onClick = off;
			int iFind = strScript.Find("\n");
			if (iFind != -1)
			{
				prop->onClickL = iFind;
				strData += strScript.Left(iFind);
				strScript = strScript.Mid(iFind + 1);
				off += iFind;
			}
			else
			{
				prop->onClickL = strScript.GetLength();
				strData += strScript;
			}
			
		}
		if (prop->onDblClk != -1)
		{
			prop->onDblClk = off;
			int iFind = strScript.Find("\n");
			if (iFind != -1)
			{
				prop->onDblClkL = iFind;
				strData += strScript.Left(iFind);
				strScript = strScript.Mid(iFind + 1);
				off += iFind;
			}
			else
			{
				prop->onDblClkL = strScript.GetLength();
				strData += strScript;
			}
		}
		if (prop->onChange != -1)
		{
			prop->onChange = off;
			int iFind = strScript.Find("\n");
			if (iFind != -1)
			{
				prop->onChangeL = iFind;
				strData += strScript.Left(iFind);
				strScript = strScript.Mid(iFind + 1);
				off += iFind;
			}
			else
			{
				prop->onChangeL = strScript.GetLength();
				strData += strScript;
			}
		}
		if (prop->onChar != -1)
		{
			prop->onChar = off;
			int iFind = strScript.Find("\n");
			if (iFind != -1)
			{
				prop->onCharL = iFind;
				strData += strScript.Left(iFind);
				strScript = strScript.Mid(iFind + 1);
				off += iFind;
			}
			else
			{
				prop->onCharL = strScript.GetLength();
				strData += strScript;
			}
		}
		if (prop->onKey != -1)
		{
			prop->onKey = off;
			int iFind = strScript.Find("\n");
			if (iFind != -1)
			{
				prop->onKeyL = iFind;
				strData += strScript.Left(iFind);
				strScript = strScript.Mid(iFind + 1);
				off += iFind;
			}
			else
			{
				prop->onKeyL = strScript.GetLength();
				strData += strScript;
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		if (file.Open(prop->scpN, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone))
		{
			file.Write((char *)strData.operator LPCTSTR(), strData.GetLength());
			file.Close();
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	addEventList();
}
void CScriptWnd::OnMemberDClick()
{
	CString sTxt, sName;
	int nEndLine, nEndChar;

	int iSel = m_listMember.GetCurSel();
	if (iSel < 0)
		return;

	if (m_eventCB.GetCurSel() <= 0)
		return;

	m_ctrlCB.GetLBText(m_ctrlCB.GetCurSel(), sName);
	m_listMember.GetLBText(iSel, sTxt);

	int ntype = m_listMember.GetItemImage(iSel);
	sTxt.Insert(0, ntype == 1 ? ':' : '.');

	CPoint pos = m_editScript.GetCursorPos();
	sName += sTxt;
	m_editScript.m_buf.InsertText(&m_editScript, pos.y, pos.x, sName, nEndLine, nEndChar);
	m_editScript.SetCursorPos(CPoint(nEndChar, nEndLine));
}

void CScriptWnd::showList(int kind, CString sName)
{
	CString info;
	CRect	rc;
	int	type = HIWORD(kind);

	m_listMember.ResetContent();
	kind = LOWORD(kind);
	if (kind == fmOBJECT)
		info = m_editScript.GetBaseControlInfo(kind, m_index);
	else
		info = m_editScript.GetBaseControlInfo(kind);

	int index = 0;
	if (!info.IsEmpty())
	{
		//if (kind == fmOBJECT)
		//	info = m_editScript.GetControlInfo(sName);
		do
		{
			CString temp = m_editScript.Parser(info, _T("\t"));
			CString text = m_editScript.Parser(temp, _T(":"));
			CString color = m_editScript.Parser(temp, _T(":"));

			m_listMember.InsertString(index, text);
			m_listMember.SetItemImage2(index, atoi(color) == 1 ? 0 : 1);
			m_listMember.SetItemBold(index, true);
			m_listMember.SetItemData(index++, atoi(color) == 1 ? RGB(189, 87, 119) : RGB(78, 201, 176));


		} while (!info.IsEmpty());
	}

}

void CScriptWnd::OnPythonClick()
{
	if (!m_mapH) return;

	m_mapH->pythonMode = !m_mapH->pythonMode;
	m_pythonBtn.SetCheck(m_mapH->pythonMode);
	m_editScript.SetPythonMode(m_mapH->pythonMode);
	m_mapH->modified = true;
}

void CScriptWnd::SetPythonMode(bool bPython)
{
	if (!m_mapH) return;
	m_mapH->pythonMode = bPython;
	m_pythonBtn.SetCheck(bPython);
	m_editScript.SetPythonMode(bPython);
}