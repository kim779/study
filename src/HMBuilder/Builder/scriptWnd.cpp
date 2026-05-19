// scriptWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "scriptWnd.h"

#include "h/keywords.h"
#include "h/mainvar.h"

#include "amCom/formItem.h"
#include "amCc/LibBuild.h"

#define	CE_CLICK	1
#define	CE_CHANGE	2
#define	CE_DBLCLK	4

#define ID_CTRLLIST	100
#define ID_EVENTLIST	101
#define ID_RICHEDIT	102
#define ID_PIN		103
#define ID_LINE		104
#define ID_SEARCH	105
#define ID_REPLACE	106
#define ID_OBJECT	107
#define ID_WHOLE	108
#define ID_VERIFY	109
#define ID_LIST_MEMBER	110

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
	m_strDrag     = _T("");
	m_Scroll      = _T("");
	m_formItem   = NULL;
	m_index       = -1;
	m_redraw     = true;
	m_bwhole      = false;
	m_bEmulRun    = false;
	m_hPinBitmap  = NULL;
	m_hPinBitmap2 = NULL;
	m_toDescription = FALSE;

	m_mapScript.RemoveAll();
	m_spreObject.Empty();
}

CScriptWnd::~CScriptWnd()
{
	if (m_formItem)
		delete m_formItem;
	
	::DeleteObject(m_hPinBitmap);
	::DeleteObject(m_hPinBitmap2);
	
	m_mapScript.RemoveAll();
}


BEGIN_MESSAGE_MAP(CScriptWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELENDOK(ID_CTRLLIST, OnCLSelEndOk)
	ON_CBN_SELENDOK(ID_EVENTLIST, OnELSelEndOk)
	ON_BN_CLICKED(ID_PIN, OnPinClick)
	ON_BN_CLICKED(ID_WHOLE, OnWholeClick)
	ON_MESSAGE(WM_USER+100, OnMessage)
	ON_LBN_DBLCLK(ID_LIST_MEMBER, OnMemberDClick)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CScriptWnd 메시지 처리기입니다.
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

	if (!m_ctrlCB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS |CBS_SORT | CBS_OWNERDRAWFIXED ,CRect(0, 0, 0, 0), this, ID_CTRLLIST))
		return -1;

	if (!m_eventCB.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_OWNERDRAWFIXED, CRect(0, 0, 0, 0), this, ID_EVENTLIST))
		return -1;

	if (!m_editScript.Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(0, 0, 0, 0), this, ID_RICHEDIT))
		return -1;

	m_editScript.EnableWindow(FALSE);
	if (!m_pinBtn.Create(NULL, _T("P"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_PIN))
		return -1;

	// Script Wnd UI변경	
	if (!m_pwholeBtn.Create(NULL, _T("WHOLE"), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0, 0, 0, 0), this, ID_WHOLE))
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
	m_listMember.SetFont(&m_font);

	// Pin Image
	m_hPinBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PIN));
	m_pinBtn.m_hBitmap = m_hPinBitmap;
	m_hPinBitmap2 = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PIN_DN));
	m_pinBtn.m_hBitmap2 = m_hPinBitmap2;

	HBITMAP hWholeBitmap, hWholeBitmap2;
	// Whole Image
	hWholeBitmap = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_ALL));
	m_pwholeBtn.m_hBitmap = hWholeBitmap;
	hWholeBitmap2 = ::LoadBitmap(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_ALL_DN));
	m_pwholeBtn.m_hBitmap2 = hWholeBitmap2;

	m_editScript.Initialize();

	CImageList* pImgList = new CImageList;
	CBitmap	bitmap;

	bitmap.LoadBitmap(IDR_DRAWMINI);
	pImgList->Create(16, 16, ILC_COLOR24 | ILC_MASK, 26, 1);
	pImgList->Add(&bitmap, RGB(192, 192, 192));
	pImgList->Add(AfxGetApp()->LoadIcon(IDI_MAP));
	pImgList->Add(AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	pImgList->SetBkColor(CLR_NONE);	
	m_ctrlCB.SetImageList(pImgList);
	bitmap.Detach();

	pImgList = new CImageList;
	pImgList->Create(IDB_AUTOCMPLIMG, 12, 1, RGB(192, 192, 192));
	pImgList->Add(&bitmap, RGB(192, 192, 192));
	m_listMember.SetImageList(pImgList);

	return 0;
}


void CScriptWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	double btnWidth = 0.0;
	CRect	rc, cRc;

	GetClientRect(cRc);
	rc.SetRect(0, 1, 21, 19);
	m_pinBtn.MoveWindow(rc);

	rc.left = rc.right + 1;
	rc.right += 22;
	m_pwholeBtn.MoveWindow(rc);

	rc.top = 0;
//	rc.left = rc.right;		// with whole button
	rc.left = rc.right + 1;		// without whole button
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
	//rc.bottom = cRc.bottom - 30;
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

	/*if (m_eventCB.GetCurSel() == 0)
	{
		//insertToEdit(_T(""));
		m_editScript.EnableWindow(FALSE);
		m_editScript.EnableScrollBar(ESB_DISABLE_BOTH);
	}*/

	if (m_index == -1)
	{
		WriteScript();

		m_prev = (int)m_eventCB.GetItemData(m_eventCB.GetCurSel());
		switch (m_prev)
		{
		case 1:
			setScript(m_mapH->onLoadN);		break;
		case 2:
			setScript(m_mapH->onSendN);		break;
		case 3:
			setScript(m_mapH->onReceiveN);		break;
		case 4:
			setScript(m_mapH->onAlertN);		break;
		case 5:
			setScript(m_mapH->onTimerN);		break;
		case 6:
			setScript(m_mapH->onCloseN);		break;
		case 7:
			setScript(m_mapH->onDeviceN);		break;
		case 8:
			setScript(m_mapH->onErrorN);		break;
		case 9:
			setScript(m_mapH->onFlickingN);		break;
		case 10:
			setScript(m_mapH->onChangeLayoutN);	break;
		case 11:
			setScript(m_mapH->onDeclarationN);	break;
		case 12:
			setScript(m_mapH->onInDeclarationN);	break;
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
			setScript(m_mapH->onDeclarationN);	
			break;
		case 2:
			setScript(m_mapH->onInDeclarationN);
			break;
		default:
			insertToEdit(_T(""));
			m_editScript.EnableWindow(FALSE);
			break;
		}
		return;
	}

	// save script
	if (!m_bwhole)
	{
		switch (m_prev)
		{
		case 1:		// click or change
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_EDIT:
			case FM_COMBO:
				m_change = getEditScript();
				m_formItem->GetForm(m_index)->onChangeL = m_change.GetLength();
				break;
			case FM_LABEL:
			case FM_OUT:
			case FM_IMAGEVIEW:
			case FM_BUTTON:
			case FM_RADIO:
			case FM_CHECK:		
			case FM_GRID:
			case FM_GRIDEX:
			case FM_TAB:
			case FM_TABLE:
				m_click = getEditScript();
				m_formItem->GetForm(m_index)->onTapL = m_click.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_CLICK)
					{
						m_click = getEditScript();
						m_formItem->GetForm(m_index)->onTapL = m_click.GetLength();
					}
				}
				break;

			default:
				break;
			}
			break;

		case 2:		// dblclk
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_LABEL:
			case FM_OUT:
			case FM_IMAGEVIEW:
			case FM_BUTTON:
			case FM_RADIO:
			case FM_CHECK:
			case FM_GRIDEX:
			case FM_GRID:
			case FM_TAB:
			case FM_TABLE:
				m_dblclk = getEditScript();
				m_formItem->GetForm(m_index)->onLongTapL = m_dblclk.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_DBLCLK)
					{
						m_dblclk = getEditScript();
						m_formItem->GetForm(m_index)->onLongTapL = m_dblclk.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;

		case 3:		// char or change
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_EDIT:
				m_char = getEditScript();
				m_formItem->GetForm(m_index)->onCharL = m_char.GetLength();
				break;

			case FM_GRIDEX:
			case FM_GRID:
				m_Scroll = getEditScript();
				m_formItem->GetForm(m_index)->onScrollL = m_Scroll.GetLength();
				break;
			case FM_COMBO:
				m_change = getEditScript();
				m_formItem->GetForm(m_index)->onChangeL = m_change.GetLength();
				break;

			case FM_CONTROL:
				{
					CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_CHANGE)
					{
						m_change = getEditScript();
						m_formItem->GetForm(m_index)->onChangeL = m_change.GetLength();
					}
				}
				break;
			}
			break;

		case 4:		// char change
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_GRIDEX:
			case FM_GRID:
			case FM_EDIT:
				m_char = getEditScript();
				m_formItem->GetForm(m_index)->onCharL = m_char.GetLength();
				break;
			}
			break;

		case 5:		// OnDrag
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_GRIDEX:
			case FM_GRID:
				m_strDrag = getEditScript();
				m_formItem->GetForm(m_index)->onDragL = m_strDrag.GetLength();			
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
		switch (m_formItem->GetForm(m_index)->kind)
		{
		case FM_EDIT:
		case FM_COMBO:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_change);
			break;
		case FM_LABEL:
		case FM_OUT:
		case FM_IMAGEVIEW:
		case FM_BUTTON:
		case FM_RADIO:
		case FM_CHECK:
		case FM_GRIDEX: 
		case FM_GRID:
		case FM_TAB:
		case FM_TABLE:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_click);
			break;
		case FM_CONTROL:
			{
				CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
				if (eventMask & CE_CLICK)
				{
					if (!m_bEmulRun)
					{
						m_editScript.EnableWindow(TRUE);
						m_editScript.EnableScrollBar(SB_BOTH);
					}
					insertToEdit(m_click);
				}
			}
			break;
		default:
			break;
		}
		break;

	case 2: 	// long tap
	/*	if (m_pwholeBtn.GetCheck())
		{
			CString symbol = m_formItem->GetForm(m_index)->name;
			CString strAddScript;
			strAddScript = getEditScript();
			strAddScript.Replace(_T("\r\n"), _T("\n"));
			m_dblclk = strAddScript + _T("Sub ") + symbol + _T("_LongTab\n\nend Sub\n");
		}
		*/
		switch (m_formItem->GetForm(m_index)->kind)
		{
//		case FM_EDIT:
		case FM_LABEL:
		case FM_OUT:
		case FM_IMAGEVIEW:
		case FM_BUTTON:
		case FM_RADIO:
		case FM_CHECK:
		case FM_GRIDEX: 
		case FM_GRID:
		case FM_TAB:
		case FM_TABLE:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_dblclk);
			break;
		case FM_CONTROL:
			{
				CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
				if (eventMask & CE_DBLCLK)
				{
					if (!m_bEmulRun)
					{
						m_editScript.EnableWindow(TRUE);
						m_editScript.EnableScrollBar(SB_BOTH);
					}
					insertToEdit(m_dblclk);
				}
			}
			break;
		default:
			break;
		}
		break;

	case 3:		// char or change or scroll
		switch (m_formItem->GetForm(m_index)->kind)
		{
		case FM_EDIT:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_char);
			break;
		case FM_GRIDEX:
		case FM_GRID:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_Scroll);
			break;
		case FM_COMBO:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_change);
			break;
		case FM_CONTROL:
			{
				CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
				long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
				if (eventMask & CE_CHANGE)
				{
					if (!m_bEmulRun)
					{
						m_editScript.EnableWindow(TRUE);
						m_editScript.EnableScrollBar(SB_BOTH);
					}
					insertToEdit(m_change);
				}
			}
			break;
		}
		break;

	case 4:		// char
		switch (m_formItem->GetForm(m_index)->kind)
		{
		case FM_GRIDEX:
		case FM_GRID:
		case FM_EDIT:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_char);
			break;
		}
		break;

	case 5:		// OnDrag
		switch (m_formItem->GetForm(m_index)->kind)
		{
		case FM_GRIDEX:
		case FM_GRID:
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			insertToEdit(m_strDrag);
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
			m_Scroll = _T("");
			if (!m_bwhole)
			{
				insertToEdit("");
			}

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

void CScriptWnd::Initialize(struct _mapH* mapH)
{
	if (m_formItem)
		delete m_formItem;

	m_mapH = mapH;
	m_formItem = new CFormItem(mapH);
	m_editScript.Initialize(m_mapH);
	if (m_mapH && m_mapH->mapK == MK_PROCEDURES)
		m_editScript.EnableWindow(FALSE);
	m_index = -1;
	m_prev = -1;
	if (m_mapH->onDeclaration || m_mapH->onInDeclaration)
		m_ctrlCB.SetItemBold(0, true);	// 0번째에 declaration
	if (m_mapH->onLoad || m_mapH->onSend || m_mapH->onReceive || m_mapH->onAlert
		|| m_mapH->onTimer || m_mapH->onClose || m_mapH->onDevice || m_mapH->onError 
		|| m_mapH->onFlicking || m_mapH->onChangeLayout)
		m_ctrlCB.SetItemBold(1, true);	// 1번째에 form
	m_eventCB.ResetContent();
	m_eventCB.ResetItemInfo();
}

void CScriptWnd::AddComboString(CString sItem, int data, int type)
{
	if (type)
		m_eventCB.SetItemData(m_eventCB.AddString(sItem), data);
	else
	{
		int idx;
		m_ctrlCB.SetItemData(idx = m_ctrlCB.AddString(sItem), data);
		if (data >= 0 && m_formItem->GetForm(data)->scpN)
			m_ctrlCB.SetItemBold(idx, true);
		if (data == m_index)
			m_ctrlCB.SetCurSel(idx);
		
		int nImage = 0;
		if ((int)data >= 0)
			nImage = m_formItem->GetForm(data)->kind;

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
	m_editScript.EnableWindow(FALSE);
}

void CScriptWnd::LoadAutoList()
{
	int	nScriptID = 100;
	CMapStringToPtr mapFormID;

	mapFormID.SetAt(_T("LABEL"),   (void*)FM_LABEL);
	mapFormID.SetAt(_T("BOX"),     (void*)FM_BOX);
	mapFormID.SetAt(_T("GROUP"),   (void*)FM_GROUP);
	mapFormID.SetAt(_T("IMAGEVIEW"),(void*)FM_IMAGEVIEW);
	mapFormID.SetAt(_T("BUTTON"),  (void*)FM_BUTTON);
	mapFormID.SetAt(_T("EDIT"),    (void*)FM_EDIT);
	mapFormID.SetAt(_T("COMBO"),   (void*)FM_COMBO);
	mapFormID.SetAt(_T("OUTPUT"),  (void*)FM_OUT);
	mapFormID.SetAt(_T("GRID"),    (void*)FM_GRID);
	mapFormID.SetAt(_T("GRIDEX"),  (void*)FM_GRIDEX);
	mapFormID.SetAt(_T("TABLE"),   (void*)FM_TABLE);
	mapFormID.SetAt(_T("OBJECT"),  (void*)FM_OBJECT);
	mapFormID.SetAt(_T("TAB"),     (void*)FM_TAB);
	mapFormID.SetAt(_T("BROWSER"), (void*)FM_BROWSER);
	mapFormID.SetAt(_T("CONTROL"), (void*)FM_CONTROL);
	mapFormID.SetAt(_T("RADIO"),   (void*)FM_RADIO);
	mapFormID.SetAt(_T("CHECK"),   (void*)FM_CHECK);

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

CString CScriptWnd::GetScript(char *dir)
{
	CFile	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;
	CString	retValue = _T("");

	if (!file.Open(dir, CFile::modeRead | CFile::typeBinary ))
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


///////////////////////////////////////////////////////////////////////////////
CString CScriptWnd::GetBookmark(_mapH *mapH, int *pLineNum, int *pIdx)
{
	if (mapH == NULL)
	{
		if (m_mapH == NULL)
			return _T("");
		else
			mapH = m_mapH;
	}

	CString sAllScript = _T("");
	CString strTmp = _T("");

	CFormItem formItem(mapH);
	int	lineNum = (pLineNum ? *pLineNum : -1), idx = -3, lineCount = -1;
		
	if (mapH->onDeclaration)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -26;
		}
	}

	if (mapH->onInDeclaration)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -27;
		}
	}

	if (mapH->onLoad)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -13;
		}
	}

	if (mapH->onSend && mapH->onSendN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -14;
		}
	}

	if (mapH->onReceive)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -15;
		}
	}

	if (mapH->onAlert)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -16;
		}
	}

	if (mapH->onTimer)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -20;
		}
	}

	if (mapH->onClose)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -22;
		}
	}

	if (mapH->onDevice)
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += strTmp;
		sAllScript += _T("\n");
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -23;
		}
	}

	for (int ii = 0; ii < formItem.GetCount(); ii++)
	{
		if (!m_formItem->GetFormStr(ii, _T("scpN")).IsEmpty())
		{
			CString formScript = GetScript(m_formItem->GetForm(ii)->scpN);
			if (formItem.GetForm(ii)->onTap != -1)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.GetForm(ii)->name;
				sAllScript += _T("Sub ") + symbol + _T("_Click\n");
				sAllScript += formScript.Left(formItem.GetForm(ii)->onTapL);
				formScript = formScript.Mid(formItem.GetForm(ii)->onTapL);
				sAllScript += _T("\nend Sub\n");
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13;
				}
			}

			if (m_formItem->GetForm(ii)->onLongTap != -1 && formItem.GetForm(ii)->onLongTapL != 0)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.GetForm(ii)->name;
				sAllScript += _T("Sub ") + symbol + _T("_LongTab\n");
				sAllScript += formScript.Left(m_formItem->GetForm(ii)->onLongTapL);
				formScript = formScript.Mid(formItem.GetForm(ii)->onLongTapL);
				sAllScript += _T("\nend Sub\n");
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 1;
				}
			}

			if (m_formItem->GetForm(ii)->onChange != -1 && formItem.GetForm(ii)->onChangeL != 0)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.GetForm(ii)->name;
				sAllScript += _T("Sub ") + symbol + _T("_Change\n");
				sAllScript += formScript.Left(m_formItem->GetForm(ii)->onChangeL);
				formScript = formScript.Mid(formItem.GetForm(ii)->onChangeL);
				sAllScript += _T("\nend Sub\n");
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 2;
				}
			}

			if (m_formItem->GetForm(ii)->onChar != -1 && formItem.GetForm(ii)->onCharL != 0)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.GetForm(ii)->name;
				sAllScript += _T("Sub ") + symbol + _T("_CharChange\n");
				sAllScript += formScript.Left(m_formItem->GetForm(ii)->onCharL);
				formScript = formScript.Mid(formItem.GetForm(ii)->onCharL);
				sAllScript += _T("\nend Sub\n");
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 3;
				}
			}

			if (m_formItem->GetForm(ii)->onScroll != -1 && formItem.GetForm(ii)->onScrollL != 0)
			{
				lineCount = getLineCount(sAllScript);
				CString symbol = formItem.GetForm(ii)->name;
				sAllScript += _T("Sub ") + symbol + _T("_Scroll\n");
				sAllScript += formScript.Left(m_formItem->GetForm(ii)->onScrollL);
				formScript = formScript.Mid(formItem.GetForm(ii)->onScrollL);
				sAllScript += _T("\nend Sub\n");
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CString CScriptWnd::GetAllScript(struct _mapH* mapH, int* pLineNum, int* pIdx)
{
	if (mapH == nullptr)
	{
		if (m_mapH == nullptr)
			return _T("");
		else
			mapH = m_mapH;
	}

	m_mapScript.RemoveAll();
	CString sAllScript = _T("");
	CString strTmp = _T("");

	CFormItem formItem(mapH);
	int	lineNum = (pLineNum ? *pLineNum : -1), idx = -3, lineCount = -1;

	strTmp = GetScript(mapH->onDeclarationN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onDeclaration && mapH->onDeclarationN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ onDeclaration()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING; 
		m_mapScript.SetAt("@@@ onDeclaration()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -26;
		}
	}

	strTmp = GetScript(mapH->onInDeclarationN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onInDeclaration && mapH->onInDeclarationN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ onInDeclaration()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING; 
		m_mapScript.SetAt("@@@ onInDeclaration()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount;
			idx = -27;
		}
	}

	strTmp = GetScript(mapH->onLoadN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onLoad && mapH->onLoadN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnLoad()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING; 
		m_mapScript.SetAt("@@@ OnLoad()", strTmp);


		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -13;
		}
	}

	strTmp = GetScript(mapH->onSendN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onSend && mapH->onSendN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnSend()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING;
		m_mapScript.SetAt("@@@ OnSend()", strTmp);

		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -14;
		}
	}

	strTmp = GetScript(mapH->onReceiveN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onReceive && mapH->onReceiveN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnReceive()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING; 

		m_mapScript.SetAt("@@@ OnReceive()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -15;
		}
	}

	strTmp = GetScript(mapH->onAlertN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onAlert && mapH->onAlertN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnAlert()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING; 

		m_mapScript.SetAt("@@@ OnAlert()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -16;
		}
	}

	strTmp = GetScript(mapH->onTimerN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onTimer && mapH->onTimerN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnTimer()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING;	

		m_mapScript.SetAt("@@@ OnTimer()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -20;
		}
	}

	strTmp = GetScript(mapH->onCloseN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onClose && mapH->onCloseN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnClose()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING;	
	
		m_mapScript.SetAt("@@@ OnClose()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -22;
		}
	}

	strTmp = GetScript(mapH->onDeviceN);
	strTmp.Replace(_T("\r\n"), _T("\n"));
	if (mapH->onDevice && mapH->onDeviceN[0] != '\0' && !strTmp.IsEmpty())
	{
		lineCount = getLineCount(sAllScript);
		sAllScript += _T("@@@ OnDevice()\n");
		sAllScript += strTmp;
		sAllScript += LINESTRING; 

		m_mapScript.SetAt("@@@ OnDevice()", strTmp);
		if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
		{
			lineNum = *pLineNum - lineCount - (lineCount?0:1);
			idx = -23;
		}
	}

	CString sKey;
	for (int ii = 0; ii < formItem.GetCount(); ii++)
	{
		if (!m_formItem->GetFormStr(ii, _T("scpN")).IsEmpty())
		{
			CString sScript;
			CString formScript = GetScript(m_formItem->GetForm(ii)->scpN);
			CString symbol = formItem.GetForm(ii)->name;
			m_mapScript.SetAt(symbol, formScript);

			//formScript.Replace(_T("\r\n"), _T("\n"));
			if (formItem.GetForm(ii)->onTap != -1 && formItem.GetForm(ii)->onTapL != 0)
			{
				lineCount = getLineCount(sAllScript);
				sAllScript += _T("@@@ ") + symbol + _T("_Click\n");
				sScript = formScript.Left(formItem.GetForm(ii)->onTapL);
				sAllScript += sScript;
				formScript = formScript.Mid(formItem.GetForm(ii)->onTapL);
				sAllScript += LINESTRING;	
				sScript.Replace(_T("\r\n"), _T("\n"));
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13;
				}
			}

			if (m_formItem->GetForm(ii)->onLongTap != -1 && formItem.GetForm(ii)->onLongTapL != 0)
			{
				lineCount = getLineCount(sAllScript);
				sAllScript += _T("@@@ ") + symbol + _T("_LongTab\n");
				sScript = formScript.Left(formItem.GetForm(ii)->onLongTapL);
				sAllScript += sScript;
				formScript = formScript.Mid(formItem.GetForm(ii)->onLongTapL);
				sAllScript += LINESTRING;	
				sScript.Replace(_T("\r\n"), _T("\n"));
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 1;
				}
			}

			if (m_formItem->GetForm(ii)->onChange != -1 && formItem.GetForm(ii)->onChangeL != 0)
			{
				lineCount = getLineCount(sAllScript);
				sAllScript += _T("@@@ ") + symbol + _T("_Change\n");
				sScript = formScript.Left(formItem.GetForm(ii)->onChangeL);
				sAllScript += sScript;
				formScript = formScript.Mid(formItem.GetForm(ii)->onChangeL);
				sAllScript += LINESTRING; 
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 2;
				}
			}

			if (m_formItem->GetForm(ii)->onChar != -1 && formItem.GetForm(ii)->onCharL != 0)
			{
				lineCount = getLineCount(sAllScript);
				sAllScript += _T("@@@ ") + symbol + _T("_CharChange\n");
				sScript = formScript.Left(formItem.GetForm(ii)->onCharL);
				sAllScript += sScript;
				formScript = formScript.Mid(formItem.GetForm(ii)->onCharL);
				sAllScript += LINESTRING; 
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 3;
				}
			}

			// onscroll
			if (m_formItem->GetForm(ii)->onScroll != -1 && formItem.GetForm(ii)->onScrollL != 0)
			{
				lineCount = getLineCount(sAllScript);
				sAllScript += _T("@@@ ") + symbol + _T("_Scroll\n");
				sScript = formScript.Left(formItem.GetForm(ii)->onScrollL);
				sAllScript += sScript;
				formScript = formScript.Mid(formItem.GetForm(ii)->onScrollL);
				sAllScript += LINESTRING; 
				if (pLineNum && *pLineNum > lineCount && *pLineNum <= getLineCount(sAllScript))
				{
					lineNum = *pLineNum - lineCount - (lineCount?0:1);
					idx = ii * 13 + 3;
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


void CScriptWnd::SetAllScript(struct _mapH* mapH)
{
	CFile	file;
	CString sEvent, sData, allScript, sSymbol, sline, sMap;
	CString sKey;
	CString	dir = _T("");
	sline = LINESTRING;
	sline.Remove('\n');

	allScript = getEditScript();
	int iPos = 0;

	CFormItem formItem(mapH);
	CMap<CString, LPCTSTR, CString, LPCTSTR>  map;

	while (allScript.GetLength() > 1)
	{
		int len = allScript.GetLength();

		iPos = allScript.Find(sline);
		sData = allScript.Left(iPos-2);
		allScript = allScript.Mid(iPos+strlen(sline) + 2);
		
		sData.Replace("\r\n", "\n");
		iPos = sData.Find("\n");
		sEvent = sData.Left(iPos);
		sData = sData.Mid(iPos + 1);

		if (sData.IsEmpty())
			continue;

		if (sEvent.Find(_T("_")) < 0)
		{
			if (strcmp(sEvent, "@@@ onDeclaration()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onDeclaration = true;
				dir = m_mapH->onDeclarationN;
			}
			else if (strcmp(sEvent, "@@@ onInDeclaration()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onInDeclaration = true;
				dir = m_mapH->onInDeclarationN;
			}
			else if (strcmp(sEvent, "@@@ OnLoad()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onLoad = true;
				dir = m_mapH->onLoadN;
			}
			else if (strcmp(sEvent, "@@@ OnSend()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onSend = true;
				dir = m_mapH->onSendN;
			}
			else if (strcmp(sEvent, "@@@ OnReceive()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onReceive = true;
				dir = m_mapH->onReceiveN;
			}
			else if (strcmp(sEvent, "@@@ OnAlert()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onAlert = true;
				dir = m_mapH->onAlertN;
			}
			else if (strcmp(sEvent, "@@@ OnTimer()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onTimer = true;
				dir = m_mapH->onTimerN;
			}
			else if (strcmp(sEvent, "@@@ OnClose()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onClose = true;
				dir = m_mapH->onCloseN;
			}
			else if (strcmp(sEvent, "@@@ OnDevice()") == 0)
			{
				m_mapScript.Lookup(sEvent, sMap);
				if (sMap.CompareNoCase(sData) == 0)
					continue;

				mapH->onDevice = true;
				dir = m_mapH->onDeviceN;
			}

			// declration && form
			if (sData.GetLength() > 0 && !dir.IsEmpty())
			{
				if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
				{
					file.Write((char *)sData.operator LPCTSTR(), sData.GetLength());
					file.Close();
				}
				else
				{
					AfxMessageBox("XError [d2]");
					return;
				}
				dir = _T("");
			}
		}
		else
		{
			int offset = 0;
			sMap.Empty();
	
			sKey = sEvent;

			iPos = sEvent.Find(_T("_"));
			sSymbol = sEvent.Left(iPos);
			sSymbol = sSymbol.Mid(strlen(_T("@@@ ")));
			sEvent = sEvent.Mid(iPos);

			CString tmpData;
			if (map.Lookup(sSymbol, tmpData))
			{
				offset = tmpData.GetLength();
				tmpData += sData;
				map.SetAt(sSymbol, tmpData);
			}
			else
				map.SetAt(sSymbol, sData);
			
			for (int ii = 0; ii < formItem.GetCount(); ii++)
			{
				if (strcmp(sSymbol, formItem.GetForm(ii)->name) == 0)
				{
					if (strcmp(sEvent, "_Click") == 0)
					{
						formItem.GetForm(ii)->onTap = offset;
						formItem.GetForm(ii)->onTapL = sData.GetLength();

						if (m_spreObject.CompareNoCase(sSymbol) == 0)
							m_click = sData;
					}
					if (strcmp(sEvent, "_LongTab") == 0)
					{
						formItem.GetForm(ii)->onLongTap = offset;
						formItem.GetForm(ii)->onLongTapL = sData.GetLength();

						if (m_spreObject.CompareNoCase(sSymbol) == 0)
							m_dblclk = sData;
					}
					if (strcmp(sEvent, "_Change") == 0)
					{
						formItem.GetForm(ii)->onChange = offset;
						formItem.GetForm(ii)->onChangeL = sData.GetLength();

						if (m_spreObject.CompareNoCase(sSymbol) == 0)
							m_change = sData;	
					}
					if (strcmp(sEvent, "_CharChange") == 0)
					{
						formItem.GetForm(ii)->onChar = offset;
						formItem.GetForm(ii)->onCharL = sData.GetLength();

						if (m_spreObject.CompareNoCase(sSymbol) == 0)
							m_char = sData;
					}
					break;
				}
			}			
		}		
	}

	for (POSITION pos = map.GetStartPosition(); pos; )
	{
		map.GetNextAssoc(pos, sSymbol, sData);

		m_mapScript.Lookup(sSymbol, sMap);
		if (sMap.CompareNoCase(sData) == 0)
		{
			map.RemoveKey(sSymbol);
			continue;
		}
	
		for (int ii = 0; ii < formItem.GetCount(); ii++)
		{
			if (strcmp(sSymbol, formItem.GetForm(ii)->name) == 0)
			{
				dir = formItem.GetForm(ii)->scpN;
				break;
			}
		}

		if (sData.GetLength() > 0 && !dir.IsEmpty())
		{
			if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
			{
				file.Write((char *)sData.operator LPCTSTR(), sData.GetLength());
				file.Close();
			}
			else
			{
				AfxMessageBox("XError [d2]");
				return;
			}
			dir = _T("");
		}
	}	
}



void CScriptWnd::OnPinClick()
{
	if (m_pinBtn.GetCheck())
		m_pinBtn.SetCheck(false);
	else
		m_pinBtn.SetCheck(true);
}
/////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////

void CScriptWnd::SelectControl(int selctl, bool bnew)
{
	if (m_pinBtn.GetCheck() || m_mapH->mapK == MK_PROCEDURES)
		return;

	BOOL bflag = IsWindowVisible();
	
	if (bnew)
	{
//		m_ctrlCB.EnableWindow(TRUE);
		m_eventCB.EnableWindow(TRUE);
		m_pwholeBtn.SetCheck(false);
		m_bwhole = false;
	}
	else if (m_bwhole && !bflag)
	{
//		m_ctrlCB.EnableWindow(TRUE);
		m_eventCB.EnableWindow(TRUE);
		m_prev = -1;
		m_bwhole = false;
		m_pwholeBtn.SetCheck(false);
		SetAllScript(m_mapH);	
	}
	else if (m_bwhole && bflag)
	{
		SetAllScript(m_mapH);
		return;
	}
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
		case 1:		// onLoad
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onLoad)
					DeleteFile(m_mapH->onLoadN);
				m_mapH->onLoadN[0] = _T('\0');
				m_mapH->onLoad = false;
				break;
			}
			if (m_mapH->onLoadN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0');
				wccGetTempName(m_mapH->onLoadN, prefix);
			}
			dir = m_mapH->onLoadN;
			m_mapH->onLoad = true;
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

		case 5:		// onTimer
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

		case 6:		// onClose
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

		case 7:		// onDevice
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

		case 8:		// onTimer
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onError)
					DeleteFile(m_mapH->onErrorN);
				m_mapH->onErrorN[0] = _T('\0');
				m_mapH->onError = false;
				break;
			}
			if (m_mapH->onErrorN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
				wccGetTempName(m_mapH->onErrorN, prefix);
			}
			dir = m_mapH->onErrorN;
			m_mapH->onError = true;
			break;

		case 9:		// onFlicking
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
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
				wccGetTempName(m_mapH->onFlickingN, prefix);
			}
			dir = m_mapH->onFlickingN;
			m_mapH->onFlicking = true;
			break;

		case 10:		// onChangeLayout
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
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
				wccGetTempName(m_mapH->onChangeLayoutN, prefix);
			}
			dir = m_mapH->onChangeLayoutN;
			m_mapH->onChangeLayout = true;
			break;

		case 11:		// onDeclaration
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onDeclaration)
					DeleteFile(m_mapH->onDeclarationN);
				m_mapH->onDeclarationN[0] = _T('\0');
				m_mapH->onDeclaration = false;
				break;
			}
			if (m_mapH->onDeclarationN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
				wccGetTempName(m_mapH->onDeclarationN, prefix);
			}
			dir = m_mapH->onDeclarationN;
			m_mapH->onDeclaration = true;
			break;	

		case 12:		// onInDeclaration
			if (sData.GetLength() <= 0)
			{
				if (m_mapH->onInDeclaration)
					DeleteFile(m_mapH->onInDeclarationN);
				m_mapH->onInDeclarationN[0] = _T('\0');
				m_mapH->onInDeclaration = false;
				break;
			}
			if (m_mapH->onInDeclarationN[0] == _T('\0'))
			{
				char	prefix[L_SGID+1];
				CopyMemory(prefix, m_mapH->mapN, L_SGID);
				prefix[L_SGID] = _T('\0'); 
				wccGetTempName(m_mapH->onInDeclarationN, prefix);
			}
			dir = m_mapH->onInDeclarationN;
			m_mapH->onInDeclaration = true;
			break;

		default:
			break;
		}
	
		if (sData.GetLength() > 0)
		{
			if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
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
			if (file.Open(dir, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
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
	else if (m_index < m_formItem->GetCount())
	{
		switch (m_prev)
		{
		case 1:		// click or change
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_EDIT:
			case FM_COMBO:
				m_change = getEditScript();
				m_formItem->GetForm(m_index)->onChangeL = m_change.GetLength();
				break;
			case FM_IMAGEVIEW:
			case FM_OUT:
			case FM_LABEL:
			case FM_BUTTON:
			case FM_RADIO:
			case FM_CHECK:
			case FM_GRID:
			case FM_GRIDEX:
			case FM_TAB:
			case FM_TABLE:
				m_click = getEditScript();
				m_formItem->GetForm(m_index)->onTapL = m_click.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_CLICK)
					{
						m_click = getEditScript();
						m_formItem->GetForm(m_index)->onTapL = m_click.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;
			
		case 2:		// dblclk
			switch (m_formItem->GetForm(m_index)->kind)
			{
//			case FM_EDIT:
			case FM_IMAGEVIEW:
			case FM_OUT:
			case FM_LABEL:
			case FM_BUTTON:
			case FM_RADIO:
			case FM_CHECK:
			case FM_GRIDEX:
			case FM_GRID:
			case FM_TAB:
			case FM_TABLE:
				m_dblclk = getEditScript();
				m_formItem->GetForm(m_index)->onLongTapL = m_dblclk.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_DBLCLK)
					{
						m_dblclk = getEditScript();
						m_formItem->GetForm(m_index)->onLongTapL = m_dblclk.GetLength();
					}
				}
				break;
			default:
				break;
			}
			break;
			
		case 3:		// char or change or scroll
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_EDIT:
				m_char = getEditScript();
				m_formItem->GetForm(m_index)->onCharL = m_char.GetLength();
				break;
			case FM_GRIDEX:
			case FM_GRID:
				m_Scroll = getEditScript();
				m_formItem->GetForm(m_index)->onScrollL = m_Scroll.GetLength();
				break;
			case FM_COMBO:
				m_change = getEditScript();
				m_formItem->GetForm(m_index)->onChangeL = m_change.GetLength();
				break;
			case FM_CONTROL:
				{
					CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
					long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
					if (eventMask & CE_CHANGE)
					{
						m_change = getEditScript();
						m_formItem->GetForm(m_index)->onChangeL = m_change.GetLength();
					}
				}
				break;
			}
			break;

		case 4:		// char
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_GRIDEX:
			case FM_GRID:
			case FM_EDIT:
				m_char = getEditScript();
				m_formItem->GetForm(m_index)->onCharL = m_char.GetLength();
				break;
			}
			break;
		case 5:
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_GRIDEX:
			case FM_GRID:
				m_strDrag = getEditScript();
				m_formItem->GetForm(m_index)->onDragL = m_strDrag.GetLength();
				break;
			}
			
		case 6:		// Onscroll
			switch (m_formItem->GetForm(m_index)->kind)
			{
			case FM_GRIDEX:
			case FM_GRID:
				m_Scroll = getEditScript();
				m_formItem->GetForm(m_index)->onScrollL = m_Scroll.GetLength();
				break;
			}
			break;
		case 0:
		default:
			break;
		}
		
		// write file
		if (m_click.IsEmpty() && (m_formItem->GetForm(m_index)->onTapL == 0)
			&& m_dblclk.IsEmpty() && (m_formItem->GetForm(m_index)->onLongTapL == 0)
			&& m_change.IsEmpty() && (m_formItem->GetForm(m_index)->onChangeL == 0)
			&& m_char.IsEmpty() && (m_formItem->GetForm(m_index)->onCharL == 0)
			&& m_strDrag.IsEmpty() && (m_formItem->GetForm(m_index)->onDragL == 0)
			&& m_Scroll.IsEmpty() && (m_formItem->GetForm(m_index)->onScrollL == 0)
			)
		{
			DeleteFile(m_formItem->GetFormStr(m_index, _T("scpN")));
			return;
		}
		
		if (m_formItem->GetFormStr(m_index, _T("scpN")).IsEmpty())
		{
			char	prefix[L_SGID+1], fileName[1024];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = _T('\0'); 
			wccGetTempName(fileName, prefix);
			m_formItem->SetFormStr(m_index, _T("scpN"), fileName);
		}
	
		if (!file.Open(m_formItem->GetFormStr(m_index, _T("scpN")), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary ))
		{
			AfxMessageBox("XError [d1]");
			return;
		}
		
		DWORD	nWritten;
		int	offs = 0;
		if (m_formItem->GetForm(m_index)->onTapL != 0 && !m_click.IsEmpty())
		{
			nWritten = m_click.GetLength();
			file.Write(m_click, nWritten);

			m_formItem->GetForm(m_index)->onTap = offs;
			offs += nWritten;
		}
		
		if (m_formItem->GetForm(m_index)->onLongTapL != 0 && !m_dblclk.IsEmpty())
		{
			nWritten = m_dblclk.GetLength();
			file.Write(m_dblclk, nWritten);

			m_formItem->GetForm(m_index)->onLongTap = offs;
			offs += nWritten;
		}
		
		if (m_formItem->GetForm(m_index)->onChangeL != 0 && !m_change.IsEmpty())
		{
			nWritten = m_change.GetLength();
			file.Write(m_change, nWritten);

			m_formItem->GetForm(m_index)->onChange = offs;
			offs += nWritten;
		}
		
		if (m_formItem->GetForm(m_index)->onCharL != 0 && !m_char.IsEmpty())
		{
			nWritten = m_char.GetLength();
			file.Write(m_char, nWritten);

			m_formItem->GetForm(m_index)->onChar = offs;
			offs += nWritten;
		}

		if (m_formItem->GetForm(m_index)->onDragL != 0 && !m_strDrag.IsEmpty())
		{
			nWritten = m_strDrag.GetLength();
			file.Write(m_strDrag, nWritten);

			m_formItem->GetForm(m_index)->onDrag = offs;
			offs += nWritten;
		}

		// onScroll
		if (m_formItem->GetForm(m_index)->onScrollL != 0 && !m_Scroll.IsEmpty())
		{
			nWritten = m_Scroll.GetLength();
			file.Write(m_Scroll, nWritten);

			m_formItem->GetForm(m_index)->onScroll = offs;
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

void CScriptWnd::addEventList()
{
	if (!m_mapH)
		return;

	int	kind = -1;
	bool	isScript = false;
	struct	_formR* formR = NULL;

	ResetCombo(2);
	
	if (!m_bEmulRun)
	{
		m_editScript.EnableWindow(TRUE);
		m_editScript.EnableScrollBar(SB_BOTH);
	}

	if (m_index >= 0)
	{
		formR = m_formItem->GetForm(m_index);
		kind = formR->kind;
		if (formR->scpN)
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
			(m_mapH->onLoad || m_mapH->onSend || m_mapH->onReceive || m_mapH->onAlert
			|| m_mapH->onTimer || m_mapH->onClose || m_mapH->onDevice || m_mapH->onError 
			|| m_mapH->onFlicking || m_mapH->onChangeLayout))
			m_ctrlCB.SetItemBold(m_ctrlCB.GetCurSel(), true);
	}

	switch (kind)
	{
	case -2:	// declaration
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		if (m_mapH->mapK != MK_PROCEDURES)
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("Declaration")), 1);
			if (m_mapH->onDeclaration) m_eventCB.SetItemBold(1, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("include Declaration")), 2);
			if (m_mapH->onInDeclaration) m_eventCB.SetItemBold(2, true);
		}
		else
			m_editScript.EnableWindow(FALSE);
		break;

	case -1:	// form
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		if (m_mapH->mapK != MK_PROCEDURES)
		{
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnLoad")), 1);
			if (m_mapH->onLoad) 
				m_eventCB.SetItemBold(1, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnSend(tr, session)")), 2);
			if (m_mapH->onSend) 
				m_eventCB.SetItemBold(2, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnReceive(tr, session)")), 3);
			if (m_mapH->onReceive) 
				m_eventCB.SetItemBold(3, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnAlert(code, data, state)")), 4);
			if (m_mapH->onAlert) 
				m_eventCB.SetItemBold(4, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnTimer")), 5);
			if (m_mapH->onTimer) 
				m_eventCB.SetItemBold(5, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnClose")), 6);
			if (m_mapH->onClose) 
				m_eventCB.SetItemBold(6, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("OnDevice")), 7);
			if (m_mapH->onDevice) 
				m_eventCB.SetItemBold(7, true);
			//m_eventCB.SetItemData(m_eventCB.AddString(_T("onError")), 8);
			//if (m_mapH->onError)	// 제거
			//	m_eventCB.SetItemBold(8, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("onFlicking")), 9);
			if (m_mapH->onFlicking) 
				m_eventCB.SetItemBold(8, true);
			m_eventCB.SetItemData(m_eventCB.AddString(_T("onChangeLayout(layout)")), 10);
			if (m_mapH->onChangeLayout) 
				m_eventCB.SetItemBold(9, true);
		}
		else
			m_editScript.EnableWindow(FALSE);
		break;
	case FM_EDIT:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 1);
		if (isScript && formR->onChange != -1 && formR->onChangeL > 0) 
			m_eventCB.SetItemBold(1, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnCharChange")), 4);
		if (isScript && formR->onChar != -1 && formR->onCharL > 0) 
			m_eventCB.SetItemBold(2, true);
		break;

	case FM_COMBO:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 3);
		if (isScript && formR->onChange != -1 && formR->onChangeL > 0) 
			m_eventCB.SetItemBold(3, true);
		break;

	case FM_GRIDEX:
	case FM_GRID:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("onClick")), 1);
		if (isScript && formR->onTap != -1 && formR->onTapL > 0) 
			m_eventCB.SetItemBold(1, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("onLongTap")), 2);	
		if (isScript && formR->onLongTap != -1 && formR->onLongTapL > 0) 
			m_eventCB.SetItemBold(2, true);
		/*	remove 2013. 12. 04.
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnChange")), 3);
		if (isScript && formR->onChange != -1 && formR->onChangeL > 0) 
			m_eventCB.SetItemBold(3, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnCharChange")), 4);
		if (isScript && formR->onChar != -1 && formR->onCharL > 0) 
			m_eventCB.SetItemBold(4, true);
		
		//******************************************************************
		m_eventCB.SetItemData(m_eventCB.AddString(_T("OnDrag")), 5);
		if (isScript && formR->onDrag != -1 && formR->onDragL > 0) 
			m_eventCB.SetItemBold(5, true);
		//***********************************************************************
		*/
		m_eventCB.SetItemData(m_eventCB.AddString(_T("onScroll(x, y)")), 3);	
		if (isScript && formR->onScroll != -1 && formR->onScrollL > 0) 
			m_eventCB.SetItemBold(3, true);

		break;

	case FM_OUT:
	case FM_LABEL:
	case FM_IMAGEVIEW:
	case FM_BUTTON:
	case FM_RADIO:
	case FM_CHECK:
	case FM_TAB:
	case FM_TABLE:
		m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("onClick")), 1);
		if (isScript && formR->onTap != -1 && formR->onTapL > 0)
			m_eventCB.SetItemBold(1, true);
		m_eventCB.SetItemData(m_eventCB.AddString(_T("onLongTap")), 2);
		if (isScript && formR->onLongTap != -1 && formR->onLongTapL > 0) 
			m_eventCB.SetItemBold(2, true);
		break;

	case FM_CONTROL:
		{
			CString sym = m_formItem->GetFormStr(m_index, _T("dat"));
			long eventMask = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCONTROLEVENT, (LPARAM)sym.operator LPCTSTR());
			m_eventCB.SetItemData(m_eventCB.AddString(_T("None")), -1);
			if (eventMask & CE_CLICK)
			{
				m_eventCB.SetItemData(m_eventCB.AddString(_T("onClick")), 1);
				if (isScript && formR->onTap != -1 && formR->onTapL > 0) 
					m_eventCB.SetItemBold(1, true);
			}
			if (eventMask & CE_DBLCLK)
			{
				int nIdx = m_eventCB.AddString(_T("onLongTap"));
				m_eventCB.SetItemData(nIdx, 2);
				if (isScript && formR->onLongTap != -1 && formR->onLongTapL > 0) 
					m_eventCB.SetItemBold(nIdx, true);
			}
			if (eventMask & CE_CHANGE)
			{
				int nIdx = m_eventCB.AddString(_T("OnChange"));
				m_eventCB.SetItemData(nIdx, 3);
				if (isScript && formR->onChange != -1 && formR->onChangeL > 0) 
					m_eventCB.SetItemBold(nIdx, true);
			}
		}
		break;
	default:
		break;
	}
		

	if (m_index == -1)
	{
		bool	set = false;

		if (!set && m_mapH->onLoad)
		{
			m_eventCB.SetCurSel(1);
			setScript(m_mapH->onLoadN);
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

		if (!set && m_mapH->onTimer)
		{
			m_eventCB.SetCurSel(5);
			setScript(m_mapH->onTimerN);
			set = true;
		}

		if (!set && m_mapH->onClose)
		{
			m_eventCB.SetCurSel(6);
			setScript(m_mapH->onCloseN);
			set = true;
		}

		if (!set && m_mapH->onDevice)
		{
			m_eventCB.SetCurSel(7);
			setScript(m_mapH->onDeviceN);
			set = true;
		}

// 		if (!set && m_mapH->onError)
// 		{
// 			m_eventCB.SetCurSel(8);
// 			setScript(m_mapH->onErrorN);
// 			set = true;
// 		}

		if (!set && m_mapH->onFlicking)
		{
			m_eventCB.SetCurSel(8);
			setScript(m_mapH->onFlickingN);
			set = true;
		}

		if (!set && m_mapH->onChangeLayout)
		{
			m_eventCB.SetCurSel(9);
			setScript(m_mapH->onChangeLayoutN);
			set = true;
		}
		/*
		if (!set && m_mapH->onDeclaration)
		{
			m_eventCB.SetCurSel(10);
			setScript(m_mapH->onDeclarationN);
			set = true;
		}

		if (!set && m_mapH->onInDeclaration)
		{
			m_eventCB.SetCurSel(11);
			setScript(m_mapH->onInDeclarationN);
			set = true;
		}
		*/
		if (!set)
		{
			m_eventCB.SetCurSel(0);
			m_editScript.EnableWindow(FALSE);
			m_mapH->onLoad		= false;
			m_mapH->onSend		= false;
			m_mapH->onReceive	= false;
			m_mapH->onAlert		= false;
			m_mapH->onTimer		= false;
			m_mapH->onClose		= false;
			m_mapH->onDevice	= false;
			m_mapH->onError		= false;
			m_mapH->onFlicking	= false;
			m_mapH->onChangeLayout	= false;
			//m_mapH->onDeclaration   = false;
			//m_mapH->onInDeclaration = false;
		}
		else
		{
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
		}

		m_prev = (long)m_eventCB.GetItemData(m_eventCB.GetCurSel());
		return;
	}	

	if (m_index == -2)
	{
		bool	set = false;

		if (!set && m_mapH->onDeclaration)
		{
			m_eventCB.SetCurSel(1);
			setScript(m_mapH->onDeclarationN);
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			set = true;
		}

		if (!set && m_mapH->onInDeclaration)
		{
			m_eventCB.SetCurSel(2);
			setScript(m_mapH->onInDeclarationN);
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
			set = true;
		}

		if (!m_eventCB.GetCount())
		{
			m_editScript.EnableWindow(FALSE);
			m_prev = -1;
			return;
		}

		if (!set)
		{
			m_eventCB.SetCurSel(0);
			m_editScript.EnableWindow(FALSE);
		}

		m_prev = (long)m_eventCB.GetItemData(m_eventCB.GetCurSel());
		return;
	}	

	if (m_formItem->GetFormStr(m_index, _T("scpN")).IsEmpty())
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return;
	}

	CFile	file;

	CString xx = m_formItem->GetFormStr(m_index, _T("scpN"));
	if (!file.Open(m_formItem->GetFormStr(m_index, _T("scpN")), CFile::modeRead | CFile::typeBinary ))
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		return;
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
	ZeroMemory(scpAlloc, scpSize+1);
	FillMemory(scpAlloc, scpSize+1, ' ');

	DWORD	dwReadLen = file.Read(scpAlloc, scpSize);
	file.Close();
	if (scpSize != dwReadLen)
	{
		m_eventCB.SetCurSel(0);
		m_editScript.EnableWindow(FALSE);
		delete[] scpAlloc;
		return ;
	}

	if (formR->onTap != -1)
	{
		m_click = CString(&scpAlloc[formR->onTap], formR->onTapL);
		m_click += _T('\0');
	}

	if (formR->onLongTap != -1)
	{
		m_dblclk = CString(&scpAlloc[formR->onLongTap], formR->onLongTapL);
		m_dblclk += _T('\0');
	}

	if (formR->onChange != -1)
	{
		m_change = CString(&scpAlloc[formR->onChange], formR->onChangeL);
		m_change += _T('\0');
	}

	if (formR->onChar != -1)
	{
		m_char = CString(&scpAlloc[formR->onChar], formR->onCharL);
		m_char += _T('\0');
	}
	
	/*
	if (formR->onDrag != -1)
	{
		m_char = CString(&scpAlloc[formR->onDrag], formR->onDragL);
		m_char += _T('\0');
	}
	*/
	
	if (formR->onScroll != -1)
	{
		m_Scroll = CString(&scpAlloc[formR->onScroll], formR->onScrollL);
		m_Scroll += _T('\0');
	}

	delete[] scpAlloc;

	bool	set = false;
	int	nKind = formR->kind;
	if (!set && m_click != _T("\0") && nKind != FM_COMBO && nKind != FM_EDIT)
	{
		setEventSel("OnClick");
		insertToEdit(m_click);
		set = true;
	}

	if (!set && m_change != _T("\0")
		&& (nKind == FM_COMBO || nKind == FM_EDIT || nKind == FM_GRID || nKind == FM_GRIDEX || nKind == FM_CONTROL))
	{
		setEventSel("OnChange");
		insertToEdit(m_change);
		set = true;
	}

	if (!set && m_dblclk != _T("\0") && (nKind != FM_COMBO || nKind != FM_EDIT))
	{
		setEventSel("OnLongTap");
		insertToEdit(m_dblclk);
		set = true;
	}

	if (!set && m_char != _T("\0") && (nKind == FM_EDIT || nKind == FM_GRID || nKind == FM_GRIDEX))
	{
		setEventSel("OnCharChange");
		insertToEdit(m_char);
		set = true;
	}

	// onScroll
	if (!set && m_Scroll != _T("\0") && (nKind == FM_GRID || nKind == FM_GRIDEX))
	{
		setEventSel("OnScroll");
		insertToEdit(m_Scroll);
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

// 전체 script보기
void CScriptWnd::setWholeScript()
{
	CString str;
	m_bwhole = true;
	str = GetAllScript(m_mapH);
	str.Replace(_T("\r\n"), _T("\n"));
	
	insertToEdit(str);
}

void CScriptWnd::setScript(char* dir)
{
	CFile	file;
	DWORD	dwRead;
	DWORD	scpSize;
	char*	sbuf;
	CString sdir(dir);
	if (!sdir.IsEmpty())
	{
		insertToEdit(_T(""));
		if (!m_editScript.IsWindowEnabled())
		{
			if (!m_bEmulRun)
			{
				m_editScript.EnableWindow(TRUE);
				m_editScript.EnableScrollBar(SB_BOTH);
			}
		}

		if (!file.Open(dir, CFile::modeRead | CFile::typeBinary ))
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
	else
	{
		insertToEdit("");
		m_editScript.EnableWindow(TRUE);
	}

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

void CScriptWnd::setEventSel(CString text)
{
	int	idx = m_eventCB.FindString(0, text);
	if (idx != CB_ERR)
		m_eventCB.SetCurSel(idx);
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
	sName.Remove('_');

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
	info = m_editScript.GetBaseControlInfo(kind);

	if (kind == FM_CONTROL)
		return;

	int index = 0;
	if (!info.IsEmpty())
	{
		if (kind == FM_CONTROL)
			info = m_editScript.GetControlInfo(sName);
		do
		{
			CString temp = m_editScript.Parser(info, _T("\t"));
			CString text = m_editScript.Parser(temp, _T(":"));
			CString color = m_editScript.Parser(temp, _T(":"));
			
			m_listMember.InsertString(index, text);
			m_listMember.SetItemImage(index, atoi(color) == 1 ? 0 : 1);
			m_listMember.SetItemBold(index, true);
			m_listMember.SetItemData(index++, atoi(color) == 1 ? RGB(189, 87, 119) : RGB(78, 201, 176));


		} while (!info.IsEmpty());
	}

}

void CScriptWnd::OnDestroy()
{
	CDockablePane::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
