
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "axisMBuilder.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "splashWnd.h"
#include "Group.h"
#include "FormDefault.h"

#include "h/mainvar.h"
#include "h/axisfire.h"
#include "h/axisrsm.h"

#include "amlib/amlib.h"
#include "amCom/formItem.h"
#include "amDlg/addrDlg.h"
#include "amDlg/batch.h"
#include "amDlg/editOrder.h"
#include "amDlg/lineDlg.h"
#include "amDlg/saveDlg.h"
#include "amDlg/setupSheet.h"
#include "amDlg/upDlg.h"
#include "amCc/LibBuild.h"
#include "wSock.h"
#include "h/workio.h"
#include "download.h"
#include "upload.h"
#include "ResourceHistDlg.h"
#include "FormLoadDlg.h"
#include "CheckOutDlg.h"


#define WM_WIZARD WM_USER + 700
#define WM_TRACE  WM_USER + 770
#define	WM_OPENEDSOCK	(WM_USER + 306)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// for F11
CMainFrame*	pMainFrm;
LRESULT CALLBACK keyProc(int code, WPARAM wp, LPARAM lp);
LRESULT CALLBACK CallProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CMainFrame::OnUpdateEditFindReplace)
	ON_COMMAND(ID_EDIT_REPLACE, &CMainFrame::OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CMainFrame::OnUpdateEditFindReplace)
	ON_COMMAND_RANGE(ID_NEW_MAP, ID_WINDOWS_CLOSE, &CMainFrame::OnMainCommand)
	ON_COMMAND_RANGE(ID_ETC_FORMAT, ID_ETC_CORGB, &CMainFrame::OnEtcCommand)
	ON_COMMAND_RANGE(ID_AL_LEFT, ID_SZ_VEQUAL, &CMainFrame::OnAlignCommand)
	ON_COMMAND_RANGE(ID_TOOL_POINTER, ID_TOOL_CONTROL, &CMainFrame::OnDrawCommand)
	ON_COMMAND_RANGE(ID_BOOKMARK_BASE, ID_BOOKMARK_BASE+50, &CMainFrame::OnBookmark)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEW_MAP, ID_WINDOWS_CLOSE, &CMainFrame::OnUpdateMainCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ETC_FORMAT, ID_ETC_CORGB, &CMainFrame::OnUpdateEtcCOmmand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AL_LEFT, ID_SZ_VEQUAL, &CMainFrame::OnUpdateAlignCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_POINTER, ID_TOOL_CONTROL, &CMainFrame::OnUpdateDrawCommand)

	ON_MESSAGE(WM_OPENOBJ, &CMainFrame::OnOpenObject)
	ON_MESSAGE(WM_USER, &CMainFrame::OnMessage)
	ON_MESSAGE(WM_LOGIN, &CMainFrame::OnLoginMessage)
	ON_MESSAGE(WM_WIZARD, OnFireEvent)
	ON_MESSAGE(WM_OPENEDSOCK, &CMainFrame::onsockmessage)
	ON_MESSAGE(WM_TRACE, OnTrace)
	ON_MESSAGE(WM_READDATA, &CMainFrame::trMaptree)
	ON_MESSAGE(WM_UPLOAD, &CMainFrame::UploadRecvData)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_EXT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	m_pGroup  = NULL;

	m_palette = (CAmPalette *) NULL;
	m_draw    = (CAmDraw *) NULL;
	m_children.RemoveAll();

	m_block.bRc.SetRectEmpty();
	m_block.count = 0;

	ZeroMemory((char *)&m_clipMAP, sizeof(_mapH));
	m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), -GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	m_dropPnt  = CPoint(-1, -1);
	m_status   = 0;

	m_bShowFlag = false;
	m_showFORM = true;
	m_initMap = _T("");
	m_curDir   = _T("");

	m_tabBar = NULL;

	m_hook = NULL;
	m_callproc = NULL;
	m_trRayout = NULL;
	m_rtsInfo = NULL;
	m_bsettrlayout = false;

	m_pSock = NULL;
	m_resouceDlg = NULL;

}

CMainFrame::~CMainFrame()
{
	if (m_callproc)
		SetWindowLong(m_hWndMDIClient, GWL_WNDPROC, (long)m_callproc);

	if (m_hook)
		::UnhookWindowsHookEx(m_hook);

	m_tips.DestroyWindow();

	if (m_pGroup)
		delete m_pGroup;

	if (m_palette)
		delete m_palette;
	if (m_draw)
		delete m_draw;

	CChildFrame* child;
	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (IsWindow(child->m_hWnd))
		{
			m_children.RemoveAt(ii);
			child->DestroyWindow();	// delete child;
		}
	}
	m_children.RemoveAll();

	if (!m_curDir.IsEmpty() && m_curDir.Find(_T("\\tmp")) != -1)
	{
		CString	fileN;
		CFileFind ff;
		BOOL	working;

		fileN.Format(_T("%s\\*.*"), m_curDir);
		working = ff.FindFile(fileN);
		while (working)
		{
			working = ff.FindNextFile();
			if (ff.IsDots() || ff.IsDirectory())
				continue;

			DeleteFile(ff.GetFilePath());
		}
		ff.Close();
	}

	CFormItem formItem(&m_clipMAP);
	formItem.RemoveAll();

	m_controlEvent.RemoveAll();

	m_mainTB.RemoveAllButtons();
	m_drawTB.RemoveAllButtons();
	m_alignTB.RemoveAllButtons();
	m_etcTB.RemoveAllButtons();

	if (m_rtsInfo)
	{
		m_rtsInfo->DestroyWindow();
		delete m_rtsInfo;
	}
	if (m_trRayout)
	{
		m_trRayout->DestroyWindow();
		delete m_trRayout;
	}

	//CMFCVisualManager::DestroyInstance();

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	TCHAR	wb[1024];
	GetCurrentDirectory(sizeof(wb), wb);
	m_currentPath = wb;
	lpCreateStruct->style |= WS_CLIPCHILDREN;

	// login
	m_loginDlg.m_currentPath = m_currentPath;
	m_loginDlg.Create(IDD_LOGIN);

	m_loginDlg.ShowWindow(SW_SHOW);
	m_loginDlg.SetParentW(this);


	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pEmul = new CEmulCtrl();
	m_pEmul->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 100, NULL);

	startWorkshop();

	DragAcceptFiles();
	m_tips.Create(this);

	pMainFrm = this;
	m_hook = SetWindowsHookEx(WH_KEYBOARD, keyProc, AfxGetInstanceHandle(), GetCurrentThreadId());

	HANDLE	instance = (HANDLE)GetWindowLong(m_hWndMDIClient, GWL_HINSTANCE);
	FARPROC	callproc = (FARPROC)MakeProcInstance(CallProc, instance);
	m_callproc = (FARPROC)SetWindowLong(m_hWndMDIClient, GWL_WNDPROC, (long)callproc);

	m_pSock = new CwSock(this);
	m_pSock->Open();
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (WS_VSCROLL|WS_HSCROLL);
	if (!CMDIFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIFrameWndEx::OnUpdateFrameTitle(bAddToTitle);
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update(); // sync the mditabctrl with all views
}

LRESULT CMainFrame::OnTrace(WPARAM wParam, LPARAM lParam)
{
	if ( m_wndOutput.GetSafeHwnd() )
		m_wndOutput.SendMessage(WM_USER, wParam, lParam);

	return 0;
}


LRESULT CMainFrame::OnFireEvent(WPARAM wParam, LPARAM lParam)
{
	int type = LOWORD(wParam);
	int nBytes = HIWORD(wParam);
	char* pBytes = (char*)lParam;

	switch(type)
	{
	case FEV_AXIS:
		break;
	case FEV_OPEN:
		m_pEmul->RunAxis(signUSERx, NULL, 0);
		break;
	case FEV_CLOSE:
		break;
	case FEV_GUIDE:
		break;
	case FEV_ERROR:
		break;
	}

	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			CChildFrame* child;
			OnDrawCommand(ID_TOOL_POINTER);
			child = (CChildFrame *)MDIGetActive();
			if (child)
			{
				if (child->m_view.m_pEmulateWnd)
					child->m_view.Emulate();
			}
			break;

		case VK_F7:
			if (::GetKeyState(VK_SHIFT) & 0x8000)
			{
				compileMap(true);
				applyAXIS(true);
			}
			else
			{
				CChildFrame *child = (CChildFrame *)MDIGetActive();
				if (child)
				{
					compileMap();
					applyAXIS();
				}
			}
			break;

		case VK_SPACE:
			if ((::GetKeyState(VK_SHIFT) & 0x8000) && (::GetKeyState(VK_CONTROL) & 0x8000))
			{
				CChildFrame* child = (CChildFrame *)MDIGetActive();
				if (child)
				{
					child->m_view.SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, 0);
					child->m_view.SendMessage(WM_LBUTTONUP, MK_LBUTTON, 0);
				}
			}
			break;
		}
	}

	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnShowPopupMenu(CMFCPopupMenu* pMenuPopup)
{
	CChildFrame* child =  (CChildFrame *)MDIGetActive();
	if (pMenuPopup == NULL || !child)
	        return TRUE;

	CMFCToolBarMenuButton* pParentButton = pMenuPopup->GetParentButton();
	if (pParentButton == NULL)
		return TRUE;

	switch (pParentButton->m_nID)
	{
	case ID_APP_GOBOOKMARK:
		pMenuPopup->RemoveAllItems();
		if (child->GetBookmarkCount() <= 0)
			pMenuPopup->InsertItem(CMFCToolBarMenuButton(ID_BOOKMARK_BASE, NULL, -1, _T("empty")));
		else
		{
			CString	tmps;
			for (int ii = 0; ii < child->GetBookmarkCount(); ii++)
			{
				tmps.Format(_T("Bookmark #%d"), ii+1);
				pMenuPopup->InsertItem(CMFCToolBarMenuButton(ID_BOOKMARK_BASE+ii, NULL, -1, tmps));
			}
		}
	}

	return CMDIFrameWndEx::OnShowPopupMenu(pMenuPopup);
}

void CMainFrame::OnClose()
{
	CString tmps;
	CChildFrame* child;

//	((CaxisMBuilderApp *)AfxGetApp())->SaveState(this);
	for ( ; m_children.GetSize(); )
	{
		child = m_children.GetAt(0);
		if (IsWindow(child->m_hWnd))
		{
			if (!child->DeleteFiles())
				return;
			child->DestroyWindow();
		}
	}

	//if (m_wizard)
	//{
	//	BOOL	retvalue = false;
	//	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4), loginSHOP, 0, -1);
	//}

	tmps.Format("%d", m_wndMapView.IsTwoDepth());
	WritePrivateProfileString(ENVIRONMENT, _T("SVRDEPTH"), tmps, m_currentPath + _T("\\workshop.ini"));

	CMDIFrameWndEx::OnClose();
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	char	wb[_MAX_PATH + 1];

	if (hDropInfo)
        {
		UINT	nFiles = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
		
		for (UINT nNames = 0; nNames < nFiles; nNames++)
		{
			ZeroMemory(wb, _MAX_PATH + 1);
			DragQueryFile(hDropInfo, nNames, (LPTSTR)wb, _MAX_PATH + 1);

			SendMessage(WM_USER, ID_USR_FILEOPEN, (LPARAM)wb);
		}
        }

	// ??? CMDIFrameWndEx::OnDropFiles(hDropInfo);
}

void CMainFrame::OnEditFind()
{
	CChildFrame*	child;
	child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.mapK == MK_PROCEDURES)
		SendMessage(WM_COMMAND, ID_EDIT_FIND);
}

void CMainFrame::OnEditReplace()
{
	CChildFrame*	child;
	child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.mapK == MK_PROCEDURES)
		SendMessage(WM_COMMAND, ID_EDIT_REPEAT);
}

void CMainFrame::OnUpdateEditFindReplace(CCmdUI *pCmdUI)
{
	CChildFrame*	child;
	child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.mapK == MK_PROCEDURES)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMainCommand(UINT cmdID)
{
	CChildFrame*	child;
	BOOL		visible;

	switch (cmdID)
	{
	case ID_NEW_MAP:
		//m_wndOutput.AddText(_T("new map\n"));

		if (m_initMap.IsEmpty())
			createNewChild(MK_NORM);
		else
		{
			CFileFind	fFind;

			if (fFind.FindFile(m_initMap) && !(GetFileAttributes(m_initMap) & FILE_ATTRIBUTE_DIRECTORY))
				openFile(m_initMap, true);
			else if (m_initMap.GetLength() == L_MAPN)
				m_wndMapView.OpenMap(m_initMap);
			else
				createNewChild(MK_NORM);
		}
		break;

	case ID_FILE_OPENPC:
		openLocalFile();
		break;

	case ID_FILE_SAVETOSERVER:
		child = (CChildFrame *) MDIGetActive();
		if (child->m_mapH.mapK == MK_NORM || child->m_mapH.mapK == MK_PROCEDURES)
		{
			generateMap(false, child);
			saveToServer();
		}
		break;

	case ID_FILE_SAVETOPC:
		saveLocalFile();
		break;

	case ID_FILE_SAVEAS:
		saveLocalPath();
		break;

	case ID_APP_SETTING:
		{
			CSetupSheet dlg(_T("Setting options"), this);
			if (dlg.DoModal() == IDOK)
			{
				bool	bInvalidate = false;

				dlg.Save();
				//m_root = dlg.m_basePage->m_path;
				m_EmulPath = dlg.m_basePage->m_path;
				
				m_sDefFontName = dlg.m_defaultPage->m_fontName;
				m_sDefFontPnt  = dlg.m_defaultPage->m_fontPoint;
				m_initMap      = dlg.m_basePage->m_maps;
				m_SourcePath   = dlg.m_basePage->m_maps;
				
				if (dlg.m_basePage->m_displayMode != m_displayMode)
					bInvalidate = true;
				m_displayMode = dlg.m_basePage->m_displayMode;

				loadScriptColors();
				SendMessage(WM_USER, ID_USR_SETEMULDIR, (LPARAM)m_EmulPath.operator LPCTSTR());
				if (bInvalidate)
					PostMessage(WM_USER, ID_USR_ALLVIEWREDRAW);
			}
			break;
		}
		break;

	case ID_APP_SAVEPOS:
		{
			CRect	rc;
			CString tmps;

			GetWindowRect(&rc);
			tmps.Format(_T("%d|%d|%d|%d"), rc.left, rc.top, rc.right, rc.bottom);
			WritePrivateProfileString(_T("POSITION"), _T("MAINFRAME"), tmps, m_currentPath + _T("\\workshop.ini"));
		//	for (int ii = 0; ii < 10; ii++)
			for (int ii = 0; ii < 4; ii++)
			{
				tmps.Format(_T("TOOLBAR%d"), ii);
				WritePrivateProfileString(_T("POSITION"), tmps, getToolBarInfo(ii), m_currentPath + _T("\\workshop.ini"));
			}
		}
		break;

	case ID_APP_DEFPOS:
		{
			CString tmps;

			WritePrivateProfileString(_T("POSITION"), _T("MAINFRAME"), _T(""), m_currentPath + _T("\\workshop.ini"));
			for (int ii = 0; ii < 10; ii++)
			{
				tmps.Format(_T("TOOLBAR%d"), ii);
				WritePrivateProfileString(_T("POSITION"), tmps, _T(""), m_currentPath + _T("\\workshop.ini"));
			}
		}
		break;

	case ID_APP_SETBOOKMARK:
		child = (CChildFrame *) MDIGetActive();
		if (child)
			child->SetBookmark(getSource(false, &child->m_mapH));
		break;

//	case ID_APP_GOBOOKMARK:
//		break;

	case ID_EDIT_COPYx:
		child = (CChildFrame *) MDIGetActive();
		child->Copy();
		break;

	case ID_EDIT_CUTx:
		child = (CChildFrame *) MDIGetActive();
		child->Cut();
		break;

	case ID_EDIT_PASTEx:
		child = (CChildFrame *) MDIGetActive();
		if (m_clipMAP.formR || (child && child->m_mapH.mapK == MK_PROCEDURES))
			child->Paste();
		break;

	case ID_EDIT_SELECTALL:
		child = (CChildFrame *) MDIGetActive();
		if (child->m_formItem->GetCount())
		{
			child->m_view.m_selF = 0;
			if (child->m_formItem->GetCount() > 1)
			{
				int	ii = 0;
				for ( ; ii < child->m_formItem->GetCount(); ii++)
					child->m_formItem->GetForm(ii)->block = true;
				child->m_block->count = ii;
				child->m_view.m_stdF = 0;
			}
			child->m_view.Invalidate();
			PostMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
		}
		break;

	case ID_EDIT_GOTO:
		{
			CLineDlg dlg;
			if (dlg.DoModal() == IDOK)
			{
				child = (CChildFrame *) MDIGetActive();
				if (child && child->m_mapH.mapK == MK_PROCEDURES)
				{
					child->m_view.SetLine(dlg.m_lineNum);
				}
				else if (child)
				{
					if (!m_wndScript.IsWindowVisible())
					{
						visible = m_wndScript.GetStyle() & WS_VISIBLE ? true : false;
						child = (CChildFrame *) MDIGetActive();
						if (child && !visible)
							m_wndScript.SelectControl(child->GetSelectedForm());
						ShowPane(&m_wndScript, !visible, FALSE, FALSE);
					}
					int lineNum = dlg.m_lineNum + 1, idx;
					m_wndScript.WriteScript();
					m_wndScript.GetAllScript(NULL, &lineNum, &idx);
					m_wndScript.SetLinePos(idx / 13, abs(idx % 13) + 1, lineNum - 1);
				}
			}
		}
		break;

	case ID_EDIT_MAPREDO:
		child = (CChildFrame *) MDIGetActive();
		if (child)
		{
			if (child->PopRedo())
			{
				child->m_view.SortIoOrder();
				child->m_view.SortTabOrder();
				child->m_view.m_selF = -1;
				SendMessage(WM_USER, ID_USR_REDRAWCTRL, -1);
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
				child->m_view.Invalidate();
			}
		}
		break;

	case ID_EDIT_MAPUNDO:
		child = (CChildFrame *) MDIGetActive();
		if (child)
		{
			if (child->PopUndo())
			{
				child->m_view.SortIoOrder();
				child->m_view.SortTabOrder();
				child->m_view.m_selF = -1;
				SendMessage(WM_USER, ID_USR_REDRAWCTRL, -1);
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
				child->m_view.Invalidate();
			}
		}
		break;


	case ID_EDIT_TABORDER:
		child = (CChildFrame *) MDIGetActive();
		for (int ii = 0; ii < child->m_formItem->GetCount(); ii++)
			child->m_formItem->GetForm(ii)->block = false;

		child->m_block->count = 0;
		child->m_view.m_nTabOdr = 0;
		child->m_view.SortTabOrder();
		child->m_view.m_nTabOdr = 1;
		child->m_view.m_nIoOdr = 0;
		child->m_view.Invalidate();
		break;

	case ID_EDIT_IOORDER:
		child = (CChildFrame *) MDIGetActive();
		for (int ii = 0; ii < child->m_formItem->GetCount(); ii++)
			child->m_formItem->GetForm(ii)->block = false;

		child->m_block->count = 0;
		child->m_view.m_nIoOdr = 0;
		child->m_view.SortIoOrder();
		child->m_view.m_nIoOdr = 1;
		child->m_view.m_nTabOdr = 0;
		child->m_view.Invalidate();
		break;

	case ID_EDIT_TABORDERx:
		{
			child = (CChildFrame *) MDIGetActive();
			for (int ii = 0; ii < child->m_formItem->GetCount(); ii++)
				child->m_formItem->GetForm(ii)->block = false;
			child->m_block->count = 0;
			child->m_view.m_nTabOdr = 0;
			child->m_view.SortTabOrder();
			child->m_view.m_nTabOdr = 1;
			child->m_view.m_nIoOdr = 0;
			child->m_view.Invalidate();

			CEditOrder dlg(NULL, &child->m_mapH);	// taborder
			if (dlg.DoModal() == IDOK)
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);

			child->m_view.m_nTabOdr = 0;
			child->m_view.SortTabOrder();
			child->m_view.m_nIoOdr = 0;
			child->m_view.Invalidate();
		}
		break;

	case ID_EDIT_IOORDERx:
		{
			child = (CChildFrame *) MDIGetActive();
			for (int ii = 0; ii < child->m_formItem->GetCount(); ii++)
				child->m_formItem->GetForm(ii)->block = false;

			child->m_block->count = 0;
			child->m_view.m_nIoOdr = 0;
			child->m_view.SortIoOrder();
			child->m_view.m_nIoOdr = 1;
			child->m_view.m_nTabOdr = 0;
			child->m_view.Invalidate();

			CEditOrder dlg(NULL, &child->m_mapH, 1);
			if (dlg.DoModal() == IDOK)
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);

			child->m_view.m_nIoOdr = 0;
			child->m_view.SortIoOrder();
			child->m_view.m_nTabOdr = 0;
			child->m_view.Invalidate();
		}
		break;

	case ID_VIEW_TB_MAIN:
		visible = (m_mainTB.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_mainTB, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_TB_DRAW:
		visible = (m_drawTB.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_drawTB, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_TB_ALIGN:
		visible = (m_alignTB.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_alignTB, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_TB_ETC:
		visible = (m_etcTB.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_etcTB, !visible, FALSE, FALSE);
		break;

	case ID_WORKSPACE_SERVER:
		visible = (m_wndMapView.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_wndMapView, !visible, FALSE, FALSE);
		break;
	case ID_WORKSPACE_CLIENT:
		visible = (m_wndFileView.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_wndFileView, !visible, FALSE, FALSE);
		break;
	case ID_WORKSPACE_TRLAYOUT:
		visible = (m_wndSymbolView.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_wndSymbolView, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_WORKSPACE:				
		if (IsWindow(m_tabBar->m_hWnd))
		{
			visible = (m_tabBar->GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
			ShowPane(m_tabBar, !visible, FALSE, FALSE);
		}
		break;

	case ID_VIEW_PROPERTIES:
		visible = (m_wndProperties.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_wndProperties, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_SCRIPT:
		visible = (m_wndScript.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		child = (CChildFrame *) MDIGetActive();
		if (child && !visible)
			m_wndScript.SelectControl(child->GetSelectedForm());
		if (!visible)
			SendMessage(WM_USER, ID_USR_STATUSLINEINFO, -1);
		ShowPane(&m_wndScript, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_TRACE:
		visible = (m_wndOutput.GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(&m_wndOutput, !visible, FALSE, FALSE);
		break;

	case ID_VIEW_WINDOWTAB:
		if (::IsWindow(m_wndMDITabs.m_hWnd))
		{
			m_wndMDITabs.DestroyWindow();
			RecalcLayout();
		}
		else
			reCreateMDITabs();
		break;

	case ID_VIEW_RELOAD:
		reloadMap();
		break;

	case ID_VIEW_HIDEFORM:
		showFORM();
		break;

	case ID_VIEW_REFRESH:
		child = (CChildFrame *) MDIGetActive();
		child->m_view.Invalidate();
		break;

	case ID_VIEW_SOURCE:
		showSource();
		break;

	case ID_VIEW_SETVIEWER:
		setSourceViewer();
		break;

	case ID_COMPILE_SOURCE:
		generateMap();
		break;

	case ID_COMPILE_COMPILE:
		compileMap();
		applyAXIS(true);
		break;

	case ID_COMPILE_COMPILEALL:
		compileMap(true);
		applyAXIS(true);
		break;

	case ID_COMPILE_BATCHCOMPILE:
		batchCompile();
		break;

	case ID_COMPILE_RUN:	// emulate
		child = (CChildFrame *) MDIGetActive();
		if (child && child->m_mapH.mapK == MK_NORM)
		{
			if (!child->m_view.m_pEmulateWnd)
			{
				compileMap();
				applyAXIS();
			}
			child->m_view.Emulate();
		}
		break;

	case ID_COMPILE_MOVE:
		applyAXIS();
		break;

	case ID_COMPILE_MOVEALL:
		applyAXIS(true);
		break;

	case ID_WINDOWS_CLOSE:
		closeChild();
		break;
	
	case ID_VIEW_RTSINFO:
		rtsInfo();
		break;

	case ID_VIEW_TRLAYOUT:
		trlayout();
		break;

	case ID_VIEW_RESOURCEHIST:
		{
			m_resouceDlg = new CResourceHistDlg(this);
			m_resouceDlg->SetRoot(m_root);
			m_resouceDlg->Create(IDD_RESOURCE_HISTORY);
			m_resouceDlg->ShowWindow(SW_SHOW);
		}
		break;
	case ID_MOBILEDEV_MOVE:
		moveMobileDevice();
		break;
	default:
		break;
	}
}

void CMainFrame::OnEtcCommand(UINT cmdID)
{
}

void CMainFrame::OnAlignCommand(UINT cmdID)
{
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	child->AlignForms(cmdID);
}

void CMainFrame::OnDrawCommand(UINT cmdID)
{
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	if (cmdID == ID_TOOL_POINTER)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	if (child)
	{
		if (cmdID == ID_TOOL_CONTROL)
			TrackPopupControl();
		child->DrawForm(cmdID);
	}
}

void CMainFrame::TrackPopupControl()
{
	CPoint	point, movepoint;
	CRect	rc;
	CMenu	menu;
	int	cmd;
	CString path, desc;
	CString tmps, sym, han;
	char	wb[1024];
	int	ii;

	menu.CreatePopupMenu();
	GetCursorPos(&point);
	
	m_drawTB.GetWindowRect(&rc);

	movepoint.x += point.x + (rc.right - point.x);
	movepoint.y += point.y + (rc.bottom - point.y);

	path.Format(_T("%s\\%s\\control.ini"), (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);
	for (ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		GetPrivateProfileString(_T("Custom Objects"), tmps, _T(""), wb, sizeof(wb), path);
		desc = wb;
		tmps = parse(desc, _T(";"));
		han  = parse(desc, _T(";"));

		if (tmps.IsEmpty())
			break;

		menu.AppendMenu(MF_STRING, CONTROL_POP_ID + (ii - 1), han);
	}
	cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, movepoint.x, movepoint.y, this);

	if (cmd >= CONTROL_POP_ID && cmd <= CONTROL_POP_ID + (ii - 1))
	{
		tmps = _T("");
		if (menu.GetMenuString(cmd, tmps,  MF_BYCOMMAND) > 0)
			m_wndProperties.setFormControl(cmd, tmps);
		else
			pressButton(ID_TOOL_POINTER);
	}
	else
		pressButton(ID_TOOL_POINTER);
}

void CMainFrame::OnBookmark(UINT cmdID)
{
	CChildFrame *child = (CChildFrame *) MDIGetActive();
	if (!child)
		return;

	int	index = cmdID - ID_BOOKMARK_BASE;
	if (index < 0 || index >= child->GetBookmarkCount())
		return;

	changeMap(child->GetBookmarkSrc(cmdID - ID_BOOKMARK_BASE));
}

void CMainFrame::OnUpdateMainCommand(CCmdUI *pCmdUI)
{
	CString	tmps;
	BOOL	enable = FALSE;
	CWinApp* app   = AfxGetApp();
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	switch (pCmdUI->m_nID)
	{
	case ID_NEW_MAP:
		if (m_children.GetSize() < MAX_MAP)
			enable = TRUE;
		break;

	case ID_FILE_SAVETOSERVER:
		tmps = (char *)SendMessage(WM_USER, ID_USR_GETIPS, 0);

		if (child && child->m_mapH.source && /*!tmps.IsEmpty() &&*/ !m_offline)
			 enable = TRUE;
		break;

	case ID_APP_SETBOOKMARK:
	case ID_APP_GOBOOKMARK:
	case ID_FILE_SAVETOPC:
	case ID_EDIT_IOORDER:
	case ID_EDIT_IOORDERx:
	case ID_EDIT_TABORDER:
	case ID_EDIT_TABORDERx:
	case ID_EDIT_GOTO:
	case ID_COMPILE_SOURCE:
	case ID_VIEW_REFRESH:
	case ID_WINDOWS_CLOSE:
		if (child && !child->m_view.m_pEmulateWnd)
			enable = TRUE;
		break;

	case ID_FILE_SAVEAS:
	case ID_COMPILE_RUN:
		if (child)
			enable = TRUE;
		break;

	case ID_EDIT_MAPUNDO:
		if (child && child->IsUndoEnable() && !child->m_view.m_pEmulateWnd)
			enable = TRUE;
		break;
	case ID_EDIT_MAPREDO:
		if (child && child->IsRedoEnable() && !child->m_view.m_pEmulateWnd)
			enable = TRUE;
		break;

	case ID_EDIT_COPYx:
	case ID_EDIT_CUTx:
		if ((child && (::GetFocus() == child->m_view.m_hWnd || ::GetFocus() == child->m_hWnd)) || (child && child->m_mapH.mapK == MK_PROCEDURES))
			enable = child->IsCopyEnable();
		else
			enable = FALSE;
		break;

	case ID_EDIT_PASTEx:
		if (m_clipMAP.formR || (child && child->m_mapH.mapK == MK_PROCEDURES))
			enable = TRUE;
		break;

	case ID_EDIT_SELECTALL:
		if ((child && ::GetFocus() == child->m_view.m_hWnd) || (child && child->m_mapH.mapK == MK_PROCEDURES))
			enable = TRUE;
		else
			enable = FALSE;
		break;

	case ID_VIEW_TB_MAIN:
		enable = TRUE;
		pCmdUI->SetCheck(m_mainTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_TB_DRAW:
		enable = TRUE;
		pCmdUI->SetCheck(m_drawTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_TB_ALIGN:
		enable = TRUE;
		pCmdUI->SetCheck(m_alignTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_TB_ETC:
		enable = TRUE;
		pCmdUI->SetCheck(m_etcTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_WORKSPACE_SERVER:
		enable = TRUE;
		//pCmdUI->SetCheck(m_wndMapView.GetStyle() & WS_VISIBLE);		
		break;
	case ID_WORKSPACE_CLIENT:
		enable = TRUE;
		//pCmdUI->SetCheck(m_wndFileView.GetStyle() & WS_VISIBLE);		
		break;
	case ID_WORKSPACE_TRLAYOUT:
		enable = TRUE;
		//pCmdUI->SetCheck(m_wndSymbolView.GetStyle() & WS_VISIBLE);		
		break;
		
	case ID_VIEW_WORKSPACE:
		enable = TRUE;
		if (IsWindow(m_tabBar->m_hWnd))
			pCmdUI->SetCheck(m_tabBar->IsWindowVisible());
		break;

	case ID_VIEW_PROPERTIES:
		enable = TRUE;
		if (IsWindow(m_wndProperties.m_hWnd))
			pCmdUI->SetCheck(m_wndProperties.IsWindowVisible());
		break;

	case ID_VIEW_SCRIPT:
		enable = TRUE;
		if (IsWindow(m_wndScript.m_hWnd))
			pCmdUI->SetCheck(m_wndScript.IsWindowVisible());
		break;

	case ID_VIEW_TRACE:
		enable = TRUE;
		if (IsWindow(m_wndOutput.m_hWnd))
			pCmdUI->SetCheck(m_wndOutput.IsWindowVisible());
		break;

	case ID_VIEW_WINDOWTAB:
		enable = TRUE;
		pCmdUI->SetCheck(::IsWindow(m_wndMDITabs.m_hWnd));
		break;

	case ID_VIEW_HIDEFORM:
		enable = TRUE;
		pCmdUI->SetCheck(m_showFORM ? 1 : 0);
		break;

	case ID_VIEW_SOURCE:
	case ID_VIEW_RELOAD:
	case ID_MOBILEDEV_MOVE:
		if (child && child->m_mapH.source)
			enable = TRUE;
		break;

	case ID_COMPILE_COMPILE:
	case ID_COMPILE_MOVE:
		if (child && (child->m_mapH.mapK == MK_NORM || child->m_mapH.mapK == MK_PROCEDURES) && !child->m_view.m_pEmulateWnd)
			enable = TRUE;
		break;

	case ID_COMPILE_COMPILEALL:
	case ID_COMPILE_MOVEALL:
		if (m_children.GetSize() > 1)
		{
			int	count = 0;
			for (int ii = 0; ii < m_children.GetSize(); ii++)
			{
				if ((m_children.GetAt(ii)->m_mapH.mapK == MK_NORM || m_children.GetAt(ii)->m_mapH.mapK == MK_PROCEDURES) &&
						!m_children.GetAt(ii)->m_view.m_pEmulateWnd)
					count++;
			}
			if (count > 1)
				enable = TRUE;
		}
		break;

	case ID_APP_SAVEPOS:
	case ID_APP_DEFPOS:
		enable = FALSE;
		break;

	case ID_FILE_OPENPC:
	case ID_APP_SETTING:
	case ID_COMPILE_BATCHCOMPILE:
	case ID_VIEW_SETVIEWER:
		enable = TRUE;
		break;

	case ID_VIEW_RTSINFO:
	case ID_VIEW_TRLAYOUT:
		enable = TRUE;
		break;

	case ID_VIEW_RESOURCEHIST:
		enable = TRUE;
		break;
	default:
		break;
	}
	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateEtcCOmmand(CCmdUI *pCmdUI)
{
	BOOL	enable;
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	if (child)
		enable = child->IsEditEnable(pCmdUI->m_nID);
	else
		enable = FALSE;

	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateAlignCommand(CCmdUI *pCmdUI)
{
	BOOL	enable;
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	if (child && child->m_mapH.mapK != MK_PROCEDURES && !child->m_view.m_pEmulateWnd)
		enable = TRUE;
	else
		enable = FALSE;

	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateDrawCommand(CCmdUI *pCmdUI)
{
	BOOL	enable;
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	if (child && child->m_mapH.mapK != MK_PROCEDURES && !child->m_view.m_pEmulateWnd)
		enable = TRUE;
	else
		enable = FALSE;

	pCmdUI->Enable(enable);
}

LRESULT CMainFrame::OnOpenObject(WPARAM wParam, LPARAM lParam)
{
	CChildFrame* child;
	CString	fileN = (char *)lParam;

	if (m_children.GetSize() >= MAX_MAP)
		return 0;

	AfxGetApp()->DoWaitCursor(1);
	child = openMap(fileN);
	if (child != (CChildFrame *) NULL)
		child->m_mapH.pathsetted = false;
	AfxGetApp()->DoWaitCursor(-1);
	return 0;
}

LRESULT CMainFrame::OnLoginMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case IDOK:
		m_loginDlg.EnableLogin(false);
		if (!m_loginDlg.m_offline)
			WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("1"), m_currentPath + _T("\\workshop.ini"));
		else
			WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("0"), m_currentPath + _T("\\workshop.ini"));

		m_userID = m_loginDlg.m_user;
		m_pass   = m_loginDlg.m_pass;

		Login(m_userID, m_pass);
		break;

	case IDCANCEL:
		PostMessage(WM_CLOSE);
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnMessage(WPARAM wParam, LPARAM lParam)
{
	static	CString	text;
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();

	switch (LOWORD(wParam))
	{
	case ID_USR_GETROOTDIR:		// get rootdir
		{
			char	wb[512];
			GetPrivateProfileString(ENVIRONMENT, ROOTDIR, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
			m_root = wb;
			if (m_root.Right(1) == _T("\\"))
				m_root = m_root.Left(m_root.GetLength() - 1);
		}
		return (long)m_root.operator LPCTSTR();

	case ID_USR_GETCURRENTPATH:	// get currentpath
		return (long)m_currentPath.operator LPCTSTR();

	case ID_USR_GETIPS:		// get ips
		{
			char	wb[512];

			GetPrivateProfileString(ENVIRONMENT, SHOPIPS, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
			text = wb;
			return (long)text.operator LPCTSTR();
		}

	case ID_USR_GETSRCVIEWER:	// get source viewer
		{
			char	wb[512];

			GetPrivateProfileString(ENVIRONMENT, SRCVIEWER, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
			text = wb;
			return (long)text.operator LPCTSTR();
		}

	case ID_USR_GETGAP:		// get gap
		{
			int	nGap;

			nGap = GetPrivateProfileInt(ENVIRONMENT, _T("GAP"), 5, m_currentPath + _T("\\workshop.ini"));
			return nGap;
		}

	case ID_USR_SETROOTDIR:		// set rootdir
		WritePrivateProfileString(ENVIRONMENT, ROOTDIR, (char *)lParam, m_currentPath + _T("\\workshop.ini"));
		break;

	case ID_USR_SETEMULDIR:		// set Emulator dir
		WritePrivateProfileString(ENVIRONMENT, "Emul_Path", (char *)lParam, m_currentPath + _T("\\workshop.ini"));
		break;

	case ID_USR_SETFORMSIZE:	// set Form Size	
		WritePrivateProfileString(ENVIRONMENT, "Form_Width", (char *)HIWORD(lParam), m_currentPath + _T("\\workshop.ini"));
		WritePrivateProfileString(ENVIRONMENT, "Form_Height", (char *)LOWORD(lParam), m_currentPath + _T("\\workshop.ini"));
		break;

	case ID_USR_SETIPS:		// set ips
		WritePrivateProfileString(SERVER, SHOPIPS, (char *)lParam, m_currentPath + _T("\\workshop.ini"));
		break;

	case ID_USR_SETSRCVIEWER:	// set source viewer
		WritePrivateProfileString(ENVIRONMENT, SRCVIEWER, (char *)lParam, m_currentPath + _T("\\workshop.ini"));
		break;

	case ID_USR_SETGAP:		// set gap (lParam = nGap)
		{
			CChildFrame* child;

			text.Format(_T("%d"), lParam);
			WritePrivateProfileString(ENVIRONMENT, _T("GAP"), text, m_currentPath + _T("\\workshop.ini"));
			for (int ii = 0; ii < m_children.GetSize(); ii++)
			{
				child = m_children.GetAt(ii);
				if (IsWindow(child->m_hWnd))
					child->m_view.SetGap((int)lParam);
			}
		}
		break;

	case ID_USR_REFRESHCTRL:	// no activate
		if (!pActiveChild)
		{
			m_wndProperties.ResetControl(0);
			m_wndScript.ResetCombo(0);
			return 0;
		}
		break;

	case ID_USR_FILEOPEN:		// file open
		openFile((char *)lParam);
		break;

	case ID_USR_TESTTRLAYOUT:
		trlayout((char *)lParam);
		break;

	case ID_USR_ISDROPAREA:		// drop area?
		{
			CRect	rc, rc2;
			CPoint pnt = CPoint(LOWORD(lParam), HIWORD(lParam));

			NegotiateBorderSpace(1, &rc);
			GetWindowRect(&rc2);
			rc.OffsetRect(rc2.left, rc2.top);
			if (rc.PtInRect(pnt))
				return 1;
		}
		break;

	case ID_USR_SETGROUP:		// setting map group
		m_wndProperties.SetGroup(m_pGroup->getGroup());
		break;

	case ID_USR_GETGROUP:		// get map group (lParam : idx)
		if (m_pGroup)
		{
			text = m_pGroup->getGroup((int)lParam);
			return (long)text.operator LPCTSTR();
		}
		break;

	case ID_USR_SERVEROPEN:		// server open	
		if (lParam == 0)
		{
			CChildFrame* child;
			CStringArray files;
			int	count = m_wndMapView.GetFiles(files);
			for (int ii = 0; ii < count; ii++)
			{
				if (m_children.GetSize() >= MAX_MAP)
					break;
					
				wccSetTempPath(text, (char *)files.GetAt(ii).operator LPCSTR());
				child = openMap(text);
				if (child != (CChildFrame *) NULL)
					child->m_mapH.pathsetted = false;
			}
		}
		else
		{
			if (m_children.GetSize() >= MAX_MAP)
				break;
			m_wndMapView.OpenMap((char *)lParam);
		}
	
		break;

	case ID_USR_GETCOLOR:		// getColor (lParam:colorinfo)
		if (lParam & 0x02000000)
			return (COLORREF) lParam;
		return m_palette->GetPaletteRGB((int)lParam);

	case ID_USR_STATUSERR:		 // status error (lParam: error code)
		m_statusBar.SetPaneText(0, getErrString((int)lParam));
		break;

	case ID_USR_SYMTBL:		// get selected symbol
		//if (m_wndSymbolView.m_hWnd)
		//	return m_wndSymbolView.GetSelectedSymbol((CPtrArray*)lParam);
		break;

	case ID_USR_STATUSLINEINFO:	// status script line info
		if (lParam == -1)
			m_statusBar.SetPaneText(1, _T(""));
		else
		{
			text.Format(_T("Ln %d, Col %d"), LOWORD(lParam) + 1, HIWORD(lParam) + 1);
			m_statusBar.SetPaneText(1, text);
		}
		break;

	case ID_USR_DROPPNT:
		m_dropPnt = CPoint(LOWORD(lParam), HIWORD(lParam));
		break;

	case ID_USR_ISOFFLINE:
		return m_offline ? 1 : 0;

	case ID_USR_MODAL:
		if (lParam)
			m_status |= MS_MODAL;
		else
			m_status &= ~MS_MODAL;
		break;

	case ID_USR_GETWIZARD:
		return	(long)m_pEmul;
		//return 0;
	//	return (long)m_wizard;

	case ID_USR_PROGRESS:
		CSplashWnd::NextScreen(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case ID_USR_SETOFFLINE:
		if (lParam)
		{
			m_offline = true;
			WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("1"), m_currentPath + _T("\\workshop.ini"));
		}
		else
		{
			m_offline = false;
			WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("0"), m_currentPath + _T("\\workshop.ini"));
		}
		return 0;

	case ID_USR_GETCORGB:
		return GetPrivateProfileInt(ENVIRONMENT, _T("CORGB"), 14024913, m_currentPath + _T("\\workshop.ini"));

	case ID_USR_MOUSETIP:
		{
			CPoint	pnt;

			GetCursorPos(&pnt);
			pnt.Offset(10, -10);
			if (lParam)
				m_tips.AddTips(1, (char *)lParam);
			if (HIWORD(wParam) == 1)
				m_tips.ShowTip(1, pnt);
			else
			{	
				m_tips.ShowWindow(SW_HIDE);
				m_tips.RemoveTips();
				//ReleaseCapture();
			}
		}
		break;

	case ID_USR_ALLVIEWREDRAW:
		{
			CChildFrame* child;

			for (int ii = 0; ii < m_children.GetSize(); ii++)
			{
				child = m_children.GetAt(ii);
				if (IsWindow(child->m_hWnd))
					child->m_view.m_display = m_displayMode;
					child->m_view.Invalidate();
			}
		}
		break;

	case ID_USR_REMOVECHILD:
		removeChild((void *)lParam);
		break;

	case ID_USR_TRLIST:
		{
			switch (HIWORD(wParam))
			{
			case TRGB_FORMALL:
				m_wndMapView.LoadMapTree(m_userID, ID_MAPTREE);
				break;
			case TRGB_TRLAYOUT:
				m_wndSymbolView.LoadMapTree(m_userID, ID_TRLATOUT);
				break;
			case TRGB_FORMSORCE:
				break;
			case TRGB_FORMSAVE:
				break;
			case TRGB_FORMCHECK:
				break;
			case TRGB_FORMHISTORY:
				break;
			}
			break;
		}
		break;
	}

	if (!m_children.GetSize() || !pActiveChild)
		return 0;

	switch (LOWORD(wParam))
	{
	case ID_USR_TRLAYOUT:
		if (m_trRayout && m_trRayout->IsWindowVisible())
			trlayout();
		break;
	case ID_USR_RTSINFO:
		if (m_rtsInfo && m_rtsInfo->IsWindowVisible())
			rtsInfo();
		break;
	case ID_USR_REFRESHCTRL:	// from child to propertiesbar refresh control (lParam = bDelete)
					// LOWORD(lParam) & 2 : all combo list , else only one form(HIWORD(lParam) combo list replace
		if (!(LOWORD(lParam) & 2))
		{
			if (LOWORD(lParam) & 1)
				m_wndScript.WriteScript();

			m_wndProperties.Initialize(&pActiveChild->m_mapH, pActiveChild->m_formItem);
			m_wndProperties.ResetControl();
			m_wndScript.ResetCombo(1);
			m_wndScript.Initialize(&pActiveChild->m_mapH);	// resetcombo -> initialize

			for (int ii = 0; ii < pActiveChild->m_formItem->GetCount(); ii++)
			{
				if (!pActiveChild->m_formItem->GetFormStr(ii, _T("name")).IsEmpty())
				{
					text = pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					m_wndProperties.AddControl(text, ii);
					m_wndScript.AddComboString(text, ii);
				}
			}
				
			m_wndProperties.SelectControl(pActiveChild->GetSelectedForm());
			m_wndScript.SelectControl(pActiveChild->GetSelectedForm());

			//if (m_trRayout && m_trRayout->IsWindowVisible())
			//{
				//CString name, path;
				//name = pActiveChild->m_mapH.mapN;
				//path.Format(_T("%s\\%s\\%s\\%s.xml"), m_root, RUNDIR, _T("tmp"), name);
				//load(&pActiveChild->m_mapH, path);
				//trlayout();
			//}			
		}
		else
		{
			if (!pActiveChild->m_formItem->GetFormStr(HIWORD(lParam), _T("name")).IsEmpty())
			{
				text = pActiveChild->m_formItem->GetFormStr(HIWORD(lParam), _T("name"));
				m_wndProperties.ReplaceControl(text, HIWORD(lParam));
				m_wndScript.ReplaceComboString(text, HIWORD(lParam));
			}
		}
		break;
	
	case ID_USR_SELCTRLTOPRO:	// from child to propertiesbar select control
		m_wndProperties.SelectControl(pActiveChild->GetSelectedForm());
		if (m_wndScript.IsWindowVisible())
			m_wndScript.SelectControl(pActiveChild->GetSelectedForm());
		break;

	case ID_USR_SELCTRLTOCHILD:	// from propertiesBar, scriptBar to child select control
		{
			int	idx = (short)LOWORD(lParam);
			
			pActiveChild->SetSelectForm(idx, (HIWORD(wParam)?true:false));
			switch (HIWORD(lParam))
			{
			case SC_PROPERTIESBAR:	// properties bar
				m_wndScript.SelectControl(pActiveChild->GetSelectedForm());
				break;
			case SC_SCRIPTBAR:	// script bar
				m_wndProperties.SelectControl(pActiveChild->GetSelectedForm());
				break;
			}
		}
		break;

	case ID_USR_REDRAWCTRL:		// refresh lParam = control num
		if (lParam == -1)
		{
			CRect	cRc, wRc;
			int	mapWidth = pActiveChild->m_mapH.width, mapHeight = pActiveChild->m_mapH.height;
			pActiveChild->GetClientRect(&cRc);
			pActiveChild->GetWindowRect(&wRc);

			int	dW = wRc.Width()  - cRc.Width();
			int	dH = wRc.Height() - cRc.Height();
			if (cRc.Height() != mapHeight || cRc.Width() != mapWidth)
			{
				CFrameWnd *pChild = (CFrameWnd*)pActiveChild;

				if (mapWidth < 100)
				{
					if (pChild->GetStyle() & WS_CAPTION)
					{
						SetWindowLong(pChild->m_hWnd, GWL_STYLE, pChild->GetStyle() & ~WS_CAPTION);
						dW -= 2;
						dH -= (GetSystemMetrics(SM_CYCAPTION) + 2);
						pChild->PostMessage(WM_SIZE, 100, MAKELPARAM(mapWidth + dW, mapHeight + dH));
					}
				}
				else
				{
					if (!(pChild->GetStyle() & WS_CAPTION))
					{
						SetWindowLong(pChild->m_hWnd, GWL_STYLE, pChild->GetStyle() | WS_CAPTION);
						dW += 2;
						dH += (GetSystemMetrics(SM_CYCAPTION) + 2);
					}
				}
					
				pChild->SetWindowPos(NULL, 0, 0, mapWidth + dW, mapHeight + dH, SWP_NOZORDER|SWP_NOMOVE);
			}
		}
		pActiveChild->m_view.InvalidateRect2((int)lParam);
		break;

	case ID_USR_SETCAPTION:		// active child caption refresh
		if (pActiveChild->m_mapH.mapK == MK_NORM)
			pActiveChild->SetWindowText(pActiveChild->m_mapH.caption);
		else if (pActiveChild->m_mapH.mapK == MK_PROCEDURES)
			pActiveChild->SetWindowText(_T("Procedures"));

		if (::IsWindow(m_wndMDITabs.m_hWnd))
			m_wndMDITabs.Update();
			
		m_wndScript.SetWindowText(CString("script window - ") + pActiveChild->m_mapH.caption);
		break;

	case ID_USR_SETMAPNAME:		// set mapname
		pActiveChild->GetSystemMenu(FALSE)->ModifyMenu(0, MF_BYPOSITION|MF_STRING, 0, pActiveChild->m_mapH.mapN);
		break;

	case ID_USR_SETFOCUSCHILD:	// active child focus
		pActiveChild->SetFocus();
		break;

	case ID_USR_SETFOCUSPRO:	// activate properties bar
		m_wndProperties.SetFocus();
		break;

	case ID_USR_DROP:		// drop : symbol
		{
			CRect	rect;
			CPoint	pt = CPoint(LOWORD(lParam), HIWORD(lParam));

			pActiveChild->SetFocus();
			pActiveChild->m_view.GetWindowRect(&rect);
			if (rect.PtInRect(pt))
				pActiveChild->OnDrop(HIWORD(wParam), pt);
		}
		break;

	case ID_USR_REFRESHPRO:		// refresh properties lParam = row;
		m_wndProperties.Refresh((int)lParam);
		break;
	case ID_USR_SETUNDO:		// lParam true:pushUndo, false:popUndo
		if (lParam)
			pActiveChild->PopRedo(true);
		else
			pActiveChild->PopUndo(false);
		break;

	case ID_USR_SETFINDCAPTION:	// findreplace dialog caption change
		text = _T("script window - ");
		text += pActiveChild->m_mapH.caption;
		m_wndScript.SetWindowText(text);
		break;

	case ID_USR_CURRENTWORD:	// current selection word of script
		if (lParam == 3)
			return (long)m_wndOutput.m_edit.SendMessage(WM_USER, 0 , 0);
		else if (pActiveChild->m_mapH.mapK == MK_PROCEDURES)
			text = pActiveChild->m_view.m_pSCEdit->SelCursorWord();
		else if (m_wndScript.IsWindowVisible() && m_wndScript.m_editScript.IsWindowEnabled())
			text = m_wndScript.m_editScript.SelCursorWord();
		return (long)text.operator LPCTSTR();

	case ID_USR_GETCONTROLEVENT:	// control object event
		{
			void*	rtValue;
			if (m_controlEvent.Lookup((char *)lParam, rtValue))
				return (long)rtValue;
			else
			{
				loadControlEvent();
				if (m_controlEvent.Lookup((char *)lParam, rtValue))
					return (long)rtValue;
				else
					return 0;
			}
		}
		break;

	case ID_USR_ENABLEPROP:		// Enable Properties bar
		if (lParam)
			m_wndProperties.EnableProperty(true);
		else
			m_wndProperties.EnableProperty(false);
		break;

	case ID_USR_TBPRESSBTN:
		pressButton((int)lParam);
		break;
	}
	return 0;
}

BOOL CMainFrame::createToolBars()
{
	// main toolbar
	if (!m_mainTB.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_EX_DRAWDDARROWS, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_mainTB.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0(_T("표준 도구 모음을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_mainTB.SetWindowText(_T("Standard"));

	CMenu	menu, *subM;

	menu.LoadMenu(IDR_MAINFRAME);
	subM = menu.GetSubMenu(0);
	if (subM)
		subM = subM->GetSubMenu(0);
	m_mainTB.ReplaceButton(ID_NEW_MAP, CMFCToolBarMenuButton((UINT)ID_NEW_MAP, subM->GetSafeHmenu(), -1));
	menu.DestroyMenu();

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, ID_BOOKMARK_BASE, _T("empty"));
	m_mainTB.ReplaceButton(ID_APP_GOBOOKMARK, CMFCToolBarMenuButton((UINT)ID_APP_GOBOOKMARK, menu.GetSafeHmenu(), -1));
	menu.DestroyMenu();

	// draw toolbar
	if (!m_drawTB.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER | CBRS_BORDER_3D, ID_VIEW_TB_DRAW)
		|| !m_drawTB.LoadToolBar(IDR_DRAWBAR))
	{
		TRACE0(_T("그리기 도구 모음을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_drawTB.SetWindowText(_T("Draw"));

	// alignment toolbar
	if (!m_alignTB.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER | CBRS_BORDER_3D, ID_VIEW_TB_ALIGN)
		|| !m_alignTB.LoadToolBar(IDR_ALIGNBAR))
	{
		TRACE0(_T("정렬 도구 모음을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_alignTB.SetWindowText(_T("Alignment"));
	
	// etc toolbar
	if (!m_etcTB.Create(this, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC| CBRS_GRIPPER | CBRS_BORDER_3D, ID_VIEW_TB_ETC)
		|| !m_etcTB.LoadToolBar(IDR_ETCBAR))
	{
		TRACE0(_T("보조 도구 모음을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_etcTB.SetWindowText(_T("Additional"));
	
	m_mainTB.EnableDocking(CBRS_ALIGN_ANY);
	m_drawTB.EnableDocking(CBRS_ALIGN_ANY);
	m_alignTB.EnableDocking(CBRS_ALIGN_ANY);
	m_etcTB.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_mainTB);
	DockPane(&m_drawTB);
	DockPane(&m_alignTB);
	DockPane(&m_etcTB);

	return TRUE;
}

BOOL CMainFrame::createDockingWindows()
{
	BOOL	bNameValid;

	// server map view
	CString	strMapView;
	bNameValid = strMapView.LoadString(IDS_MAP_VIEW);
	ASSERT(bNameValid);
	m_wndMapView.SetDirectory(m_curDir);
	if (!m_wndMapView.Create(strMapView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MAPVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("서버맵 뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_wndMapView.SetMapTreeGubn(ID_MAPTREE);

	// client view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("파일 뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}
	
	// symbol view
	CString strSymbolView;
	bNameValid = strSymbolView.LoadString(IDS_SYMBOL_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSymbolView.Create(strSymbolView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("심볼 뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_wndSymbolView.SetMapTreeGubn(ID_TRLATOUT);

	// 출력 창을 만듭니다.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("출력 창을 만들지 못했습니다.\n"));
		return FALSE;
	}

	// 속성 창을 만듭니다.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("속성 창을 만들지 못했습니다.\n"));
		return FALSE;
	}

	// 스크립트 창을 만듭니다.
	CString	strScript;
	bNameValid = strScript.LoadString(IDS_SCRIPT_WND);
	ASSERT(bNameValid);
	if (!m_wndScript.Create(strScript, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_SCRIPT, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("속성 창을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_wndScript.m_toDescription = (GetPrivateProfileInt(ENVIRONMENT, _T("TODESCRIPTION"), 0, m_currentPath + _T("\\workshop.ini")) ? TRUE : FALSE);

	setDockingWindowIcons();
	return TRUE;
}

void CMainFrame::setDockingWindowIcons()
{
	HICON hMapViewIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WORK1), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndMapView.SetIcon(hMapViewIcon, FALSE);

	HICON hFileViewIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WORK2), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hSymbolViewIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WORK2), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSymbolView.SetIcon(hSymbolViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_OUTPUT), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_PROPERTIES), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	HICON hScriptIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_OUTPUT), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndScript.SetIcon(hOutputBarIcon, FALSE);
}

void CMainFrame::setToolbarPosition()
{
	int	kk, value;
	char	wb[512];
	CString tmps;;
	DWORD	rc;

/*	rc = GetPrivateProfileString(_T("POSITION"), _T("MAINFRAME"), _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
	tmps = CString(wb, rc);

	if (!tmps.IsEmpty())
		MoveWindow(parseRect(tmps));
*/
	CString	text;
	CStringArray	sortArr;
//	for (int ii = 0; ii < 10; ii++)
	for (int ii = 0; ii < 4; ii++)
	{
		text.Format(_T("TOOLBAR%d"), ii);
		rc = GetPrivateProfileString(_T("POSITION"), text, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
		tmps = CString(wb, rc);
		if (tmps.IsEmpty())
			continue;

		value = getToolbarPriority(tmps);
		text.Format(_T("%d|%d|%s"), value, ii, tmps);

		for (kk = (int)sortArr.GetSize() - 1; kk >= 0 ; kk--)
		{
			CString sPriority = sortArr.GetAt(kk);
			if (atoi(parse(sPriority, _T("|"))) < value)
				break;
		}

		kk++;
		sortArr.InsertAt(kk, text);
	}

	for (kk = 0; kk < sortArr.GetSize(); kk++)
	{
		tmps = sortArr.GetAt(kk);
		parse(tmps, _T("|"));
		value = atoi(parse(tmps, _T("|")));
		parseToolBar(value, tmps);
	}
	sortArr.RemoveAll();
	RecalcLayout();
}

int CMainFrame::getToolbarPriority(CString info)
{
	if (info.IsEmpty())
		return 0;

	CRect rc;
	DWORD style;
	int	nState;

	rc = parseRect(parse(info, ";"));
	style = atol(parse(info, ";"));
	nState = atoi(parse(info, ";"));

	if ((nState & 2) || !(nState & 1))
		return 0;

	if (style & CBRS_ALIGN_TOP)
		return rc.top;
	else if (style & CBRS_ALIGN_BOTTOM)
		return rc.Height() - rc.bottom;
	else if (style & CBRS_ALIGN_LEFT)
		return rc.left;
	else if (style & CBRS_ALIGN_RIGHT)
		return rc.Width() - rc.right;
	return 0;
}

CRect CMainFrame::parseRect(CString str)
{
	if (str.IsEmpty())
		return CRect(0, 0, 0, 0);
	
	CRect	rc;

	rc.left   = atol(parse(str, "|"));
	rc.top    = atol(parse(str, "|"));
	rc.right  = atol(parse(str, "|"));
	rc.bottom = atol(parse(str, "|"));
	return rc;
}

void CMainFrame::parseToolBar(int idx, CString info)
{
	if (info.IsEmpty() || idx < 0 || idx > 7)
		return;

	CRect	rc;
	DWORD	style, pos = AFX_IDW_DOCKBAR_TOP;
	int	nState, nRows;

	rc = parseRect(parse(info, _T(";")));
	style  = atol(parse(info, _T(";")));
	nState = atoi(parse(info, _T(";")));
	nRows  = atoi(parse(info, _T(";")));

	CPane*	pPane;
	switch (idx)
	{
	case 0:	//	mainBar
		pPane = (CPane*)&m_mainTB;	break;
	case 1:	//	alignBar
		pPane = (CPane*)&m_alignTB;	break;
	case 2:	//	exBar
		pPane = (CPane*)&m_etcTB;	break;
	case 3:	//	drawBar
		pPane = (CPane*)&m_drawTB;	break;
	case 4:	//	workspaceBar
		pPane = (CPane*)m_tabBar;
		break;
	case 5:	//	propertiesBar
		pPane = (CPane*)&m_wndProperties;
		break;
	case 6:	//	scriptBar
		pPane = (CPane*)&m_wndScript;
		break;
	case 7:	//	traceBar
		pPane = (CPane*)&m_wndOutput;
		break;
	}

	int gapY = GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;

	ShowPane(pPane, FALSE, FALSE, FALSE);

	if (style & CBRS_ALIGN_TOP)
		pos = AFX_IDW_DOCKBAR_TOP;
	else if (style & CBRS_ALIGN_BOTTOM)
		pos = AFX_IDW_DOCKBAR_BOTTOM;
	else if (style & CBRS_ALIGN_LEFT)
		pos = AFX_IDW_DOCKBAR_LEFT;
	else if (style & CBRS_ALIGN_RIGHT)
		pos = AFX_IDW_DOCKBAR_RIGHT;

	if (nState & 2)
	{
		rc.InflateRect(0, gapY, 0, 0);
		pPane->FloatPane(rc);
	//	if (idx < 4)
	//	{
	//		CRect	tRc;
	//		((CToolBar*)pControlBar)->GetToolBarCtrl().SetRows(nRows, TRUE, &tRc);
	//		((CToolBar*)pControlBar)->CalcDynamicLayout(rc.Width() + 10, LM_HORZ | LM_COMMIT);
	//	}
	}
	else
	{
		HDWP	hdwp;

		pPane->SetPaneStyle(style);
		DockPane(pPane);
	//	pPane->SetPaneRect(rc);
		pPane->MovePane(rc, FALSE, hdwp);
	}
	
	ShowPane(pPane, (nState & 1), FALSE, FALSE);
}

CString CMainFrame::getToolBarInfo(int idx)
{
	CString	text;
	CRect	rc = CRect(0, 0, 0, 0);
	DWORD	style, nState = 0;
	int	nRows = 1;
	CPane*	pPane = NULL;

	switch (idx)
	{
	case 0:	//	mainBar
		pPane = (CPane*)&m_mainTB;		break;
	case 1:	//	alignBar
		pPane = (CPane*)&m_alignTB;		break;
	case 2:	//	exBar
		pPane = (CPane*)&m_etcTB;		break;
	case 3:	//	drawBar
		pPane = (CPane*)&m_drawTB;		break;
	case 4:	//	workspaceBar
		pPane = (CPane*)m_tabBar;		break;
	case 5:	//	propertiesBar
		pPane = (CPane*)&m_wndProperties;	break;
	case 6:	//	scriptBar
		pPane = (CPane*)&m_wndScript;		break;
	case 7:	//	traceBar
		pPane = (CPane*)&m_wndOutput;		break;
	}

	if (pPane && IsWindow(pPane->m_hWnd))
	{
		pPane->FillWindowRect();
		rc = pPane->GetPaneRect();
		style = pPane->GetPaneStyle();
		nState = (pPane->IsWindowVisible() ? 1: 0);
		nState |= (pPane->IsFloating() ? 2 : 0);
		text.Format(_T("%d|%d|%d|%d|;%ld;%d;%d;"), rc.left, rc.top, rc.right, rc.bottom, style, nState, nRows);
	}
	else	text = _T("");

	return text;
}

void CMainFrame::pressButton(UINT id)
{
	for (UINT ii = 0; ii <= ID_TOOL_CONTROL - ID_TOOL_BASE; ii++)
		m_drawTB.SetButtonStyle(ii, 0);

	if (id != ID_TOOL_POINTER)
	{
		CChildFrame* child = (CChildFrame *)MDIGetActive();
		child->ReleaseFocus();
	}

	m_drawTB.SetButtonStyle(id - ID_TOOL_BASE, TBBS_PRESSED);
}

void CMainFrame::loadScriptColors()
{
	m_wndScript.ReloadColors();
	for (int ii = 0; ii < m_children.GetSize(); ii++)
		m_children.GetAt(ii)->ReloadColors();
}

int CMainFrame::initDir(CMapStringToString& maps)
{
	CString tmps, exes = _T("\\exe");
	char	wb[128];
	DWORD	idir;
	int	l_exe = exes.GetLength();
	CString	fileN;
	CFileFind ff;

	m_root = (char *)SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	idir = GetFileAttributes(m_root);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || m_root.IsEmpty())
	{
		idir = ::GetCurrentDirectory(sizeof(wb), wb);
		tmps = CString(wb, idir);
		if (tmps.GetLength() > l_exe && !tmps.Right(l_exe).CompareNoCase(exes))
			tmps = tmps.Mid(0, tmps.GetLength() - l_exe);
		m_root = tmps;
		WritePrivateProfileString(ENVIRONMENT, ROOTDIR, tmps, m_currentPath + _T("\\workshop.ini"));
	}

	//////////////////////////////////////////////////////////////////////////////
	// Emulator Path & Source Path 추가	
	CString strPath;	
	GetPrivateProfileString(ENVIRONMENT, "Emul_Path", m_currentPath, wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
	m_EmulPath = wb;	
	GetPrivateProfileString(ENVIRONMENT, "INITMAP", m_currentPath, wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
	m_SourcePath = wb;	
	//////////////////////////////////////////////////////////////////////////////

	makeDir(m_root); // directory setting

	m_openDir = m_root + _T('\\') + RUNDIR;
	m_curDir  = m_openDir + _T("\\tmp");
	idir = GetFileAttributes(m_curDir);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!::CreateDirectory(m_curDir, NULL))
		{
			MessageBox(_T("Can't read the directory."), _T("Error"), MB_OK | MB_ICONSTOP);
			return -1;
		}
	}
	wccSetCurrentDirectory(m_curDir);

	CString backFileN;

	fileN.Format(_T("%s\\*.*"), m_curDir);
	backFileN.Format(_T("%s\\backup"), m_curDir);

	idir = GetFileAttributes(backFileN);
	/*
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!::CreateDirectory(backFileN, NULL))
			MessageBox(_T("Can't make the backup directory."), _T("Error"), MB_OK | MB_ICONSTOP);
	}
	*/

	int	nTotal = 0, nCnt = 0;
	BOOL	working = ff.FindFile(fileN);
	while (working)
	{
		working = ff.FindNextFile();
		if (!ff.IsDirectory() && ff.GetFileName().GetLength() == L_MAPN)
			nTotal++;
	}
	ff.Close();

	maps.RemoveAll();
	working = ff.FindFile(fileN);
	while (working)
	{
		working = ff.FindNextFile();
		tmps = ff.GetFileName();
		if (!ff.IsDirectory() && tmps.GetLength() == L_MAPN)
		{
			backFileN.Format(_T("%s\\backup\\%s.xml"), m_curDir, tmps);
			SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(nCnt++ * 100 / nTotal, 4));
			CopyFile(fileN, backFileN, FALSE);
			maps.SetAt(tmps, backFileN);
		}
	}
	ff.Close();
	return 1;
}

// 새로운 창 생성시 호출 다이어로그 정보
void CMainFrame::createNewChild(int mapK)
{
	CRect	rect, rc;
	CString	tmps;
	CChildFrame*	child;
	int	fontpt;

	CFormDefault	dlg;
	CString		path;

	//path.Format(_T("%s\\%s\\amDefault.ini"), m_root, TABDIR);
	//dlg.m_path = path;

	CString	formname = _T("");
	CString	desc = _T("");
	CString	width = _T("");
	CString	height = _T("");
	CString	fontname = _T("");
	CString	fontsize = _T("");

	if (dlg.DoModal() == IDOK)
	{
		formname = dlg.m_formname;
		desc = dlg.m_desc;
		tmps = dlg.m_Title;
		width = dlg.m_width;
		height = dlg.m_height;
		fontname = dlg.m_fontname;
		fontsize = dlg.m_fontsize;
	}
	else
		return;

	if (mapK == MK_NORM)
		rect.SetRect(0, 0, atoi(width), atoi(height));
	else if (mapK == MK_PROCEDURES)
	{
		tmps = _T("Procedures");
		rect.SetRect(0, 0, atoi(width), atoi(height));
	}

	rect.OffsetRect(m_childPnt);
	NegotiateBorderSpace(1, &rc);
	rc.OffsetRect(-rc.left, -rc.top);
	if (rect.right > rc.right || rect.bottom > rc.bottom)
	{
		rect.OffsetRect(-m_childPnt.x, -m_childPnt.y);
		m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), -GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	}
	else
		rect.OffsetRect(-m_childPnt.x, -m_childPnt.y);
	
	m_childPnt.x += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	m_childPnt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	
	rect.OffsetRect(m_childPnt);
	fontpt = 9;

	child = new CChildFrame(this, m_curDir, &m_block);
	if (!child->Create(tmps, rect, m_draw, m_palette, m_displayMode))
		return;

	child->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);

	if (mapK == MK_NORM)
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_MAP), FALSE);
	else 
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_PROCEDURES), FALSE);

	child->SetNew(true);
	strcpy_s(child->m_mapH.mapN, sizeof(child->m_mapH.mapN), formname);
	strcpy_s(child->m_mapH.mapdesc, sizeof(child->m_mapH.mapdesc), desc);
	strcpy_s(child->m_mapH.fName, sizeof(child->m_mapH.fName), fontname);
	child->m_mapH.width  = atoi(width);//rect.Width();
	child->m_mapH.height = atoi(height);//rect.Height();
	child->m_mapH.fPoint = atoi(fontsize);
	child->m_mapH.alpha = 100;	// Alpha Default Setting
	child->m_mapH.mapK = mapK;
	strcpy_s(child->m_mapH.caption, sizeof(child->m_mapH.caption), tmps);
	child->SetVisible(m_showFORM);
	m_children.Add(child);

	CSize	delta;
	CRect	cRc, wRc;
	child->GetClientRect(&cRc);
	child->GetWindowRect(&wRc);
        delta.cx = wRc.Width() - cRc.Width() + GetSystemMetrics(SM_CXBORDER)*4;
        delta.cy = wRc.Height() - cRc.Height() + + GetSystemMetrics(SM_CYBORDER)*4;

	child->SetWindowPos(NULL, 0, 0, rect.Width()+delta.cx, rect.Height()+delta.cy, SWP_NOMOVE|SWP_SHOWWINDOW);

	child->SetWindowText(tmps);
	//child->SetFontSize(atoi(m_sDefFontPnt), FS_NORM, m_sDefFontName);
	child->SetFontSize(atoi(fontsize), FS_NORM, fontname);
	child->SetNew(false);
	child->m_view.SendMessage(WM_SIZE, 0, MAKELPARAM(rect.Width(), rect.Height()));
	child->MDIActivate();

	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	SendMessage(WM_USER, ID_USR_REDRAWCTRL, -1);
	SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	SendMessage(WM_USER, ID_USR_SETCAPTION);
}

void CMainFrame::openFile(CString path, bool bDefault)
{
	int	offs;
	CString	mapN, kind, tmps;

	offs = path.ReverseFind(_T('\\'));
	if (offs == -1)
		return;	

	mapN = path.Mid(offs+1);
	m_openDir = path.Left(offs);

	tmps = _T("");
	CChildFrame* child;

	AfxGetApp()->DoWaitCursor(1);

	wccSetTempPath(tmps, (char *)mapN.operator LPCTSTR());
	CopyFile(path, tmps, FALSE);
	child = openMap(tmps);
	if (child != (CChildFrame *) NULL)
	{
		tmps = m_openDir.Left(offs);
		if (bDefault)
			child->m_mapH.mapN[0] = _T('\0');
		else
			strcpy_s(child->m_mapH.fullN, sizeof(child->m_mapH.fullN), tmps);
		child->m_mapH.pathsetted = !bDefault;
		child->m_mapH.modified   = false;
		PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
		if (bDefault)
			PostMessage(WM_USER, ID_USR_SETMAPNAME);
	}
	m_dropPnt = CPoint(-1, -1);
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CMainFrame::openLocalFile()
{
	CFileDialog fDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"));
	//fDlg.m_ofn.lpstrInitialDir = m_openDir;
	fDlg.m_ofn.lpstrInitialDir = m_SourcePath;
	if (fDlg.DoModal() != IDOK)
		return;

	openFile(fDlg.GetPathName());
}

CChildFrame* CMainFrame::openMap(CString fileN)
{
	int	formN, repN, grN;
	CString tmps;

	fileN.MakeUpper();
//	tmps = fileN;
//	tmps.MakeReverse();
//	if (tmps[0] == '.')
//		fileN.Remove('.');

	if ((fileN.Find(_T(".XML")) == -1))
		fileN += _T(".XML");


	formN = grN = 0; repN = -1;
	CChildFrame* child = new CChildFrame(this, m_curDir, &m_block);
	if (!child->Create(_T(""), CRect(0,0,0,0), m_draw, m_palette, m_displayMode))
		return NULL;

	child->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	child->SetVisible(m_showFORM);
	child->SetFontSize(atoi(m_sDefFontPnt), FS_NORM, m_sDefFontName);

	child->m_mapH.onLoad	= false;
	child->m_mapH.onSend    = false;
	child->m_mapH.onReceive = false;
	child->m_mapH.onAlert   = false;
	child->m_mapH.onTimer   = false;
	child->m_mapH.onClose   = false;
	child->m_mapH.onDevice  = false;
	child->m_mapH.onError	= false;
	child->m_mapH.onFlicking = false;
	child->m_mapH.onDeclaration = false;
	child->m_mapH.onInDeclaration = false;

	load(&child->m_mapH, fileN);
	child->ChangeKind();

	if (checkInEditing(child->m_mapH.mapN, child->m_mapH.mapK))
	{
		child->MDIDestroy();
		if (::IsWindow(m_wndMDITabs.m_hWnd))
			m_wndMDITabs.Update();
		return (CChildFrame *) NULL;
	}
	m_children.Add(child);

	child->m_mapH.formN    = formN;
	child->m_mapH.source   = true;
	child->m_mapH.modified = false;
	child->SetNew(true);

	if (child->m_mapH.width < 100)
	{
		child->ModifyStyle(WS_CAPTION, NULL, SWP_DRAWFRAME);
	}

	CSize	delta;
	CRect	cRc,  wRc, rc;

	child->GetClientRect(&cRc);
	child->GetWindowRect(&wRc);

	delta.cx = wRc.Width()  - cRc.Width() + (child->m_mapH.width >= 100?GetSystemMetrics(SM_CXBORDER)*4 : 0);
	delta.cy = wRc.Height() - cRc.Height() + (child->m_mapH.width >= 100?GetSystemMetrics(SM_CYBORDER)*4 : 0);

	child->SetWindowText(child->m_mapH.caption);

	cRc.OffsetRect(m_childPnt);
	NegotiateBorderSpace(1, &rc);
	rc.OffsetRect(-rc.left, -rc.top);

	if (cRc.right > rc.right || cRc.bottom > rc.bottom)
	{
		cRc.OffsetRect(-m_childPnt.x, -m_childPnt.y);
		m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), -GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	}

	m_childPnt.x += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	m_childPnt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);

	child->SetWindowPos(NULL, m_childPnt.x, m_childPnt.y, child->m_mapH.width + delta.cx, child->m_mapH.height + delta.cy, SWP_SHOWWINDOW);
	child->SetNew(false);

	child->m_view.SendMessage(WM_SIZE, 0, MAKELPARAM(child->m_mapH.width, child->m_mapH.height));
	child->LoadProcedures();
	child->MDIActivate();

	if (child->m_mapH.mapK == MK_NORM)
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_MAP), FALSE);
	else 
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_PROCEDURES), FALSE);

	if (child->m_mapH.formN)
	{
		child->m_view.m_selF = 0;
		child->m_view.SortIoOrder();
		child->m_view.SortTabOrder();
		child->m_view.m_selF = -1;
	}

	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);	// Properties & script reload
	SendMessage(WM_USER, ID_USR_SETMAPNAME);

	return (CChildFrame *) child;
}

void CMainFrame::saveLocalFile()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (!child->m_mapH.pathsetted)
		saveLocalPath();
	else
		saveMap(child);
}

void CMainFrame::saveLocalPath()
{
	DWORD	idir;
	CString	path, old;

	CChildFrame* child = (CChildFrame *)MDIGetActive();
	path = child->m_mapH.fullN;

	if (path.IsEmpty())
		path = m_SourcePath;

	old  = path;

	CSaveDlg* savedlg = new CSaveDlg(this, path);
	if (savedlg->DoModal() != IDOK)
	{
		delete savedlg;
		return;
	}

	path = savedlg->m_path;
	strcpy_s(child->m_mapH.fullN, sizeof(child->m_mapH.fullN), path);
	delete savedlg;

	idir = GetFileAttributes(path);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!CreateDirectory(path, NULL))
		{
			MessageBox(_T("Can't read the directory."), _T("Error"), MB_OK | MB_ICONSTOP);
			strcpy_s(child->m_mapH.fullN, sizeof(child->m_mapH.fullN), old);
			return;
		}
	}

	child->m_mapH.pathsetted = true;
	saveMap(child);
}

void CMainFrame::saveMap(CChildFrame* child)
{
	CString	dir = _T(""), tdir, xmldir;

	generateMap(false, child);
	if (!child->m_mapH.source)
		return;

	dir  = child->m_mapH.fullN;

	if (dir.Right(1) != "\\")
		dir += _T("\\");
	dir += child->m_mapH.mapN;

	tdir = _T("");
	wccSetTempPath(tdir, child->m_mapH.mapN);
	//CopyFile(tdir, dir, false);	// ljh

	tdir += ".xml";	xmldir = dir + ".xml";
	CopyFile(tdir, xmldir, false);
}

void CMainFrame::saveToServer()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.source)
	{
		CUpDlg* upDlg = new CUpDlg(this, &child->m_mapH);
		if (upDlg->DoModal() == IDOK)
		{

			CUpload* pUp = new CUpload();
			pUp->FormUploadSetting(upDlg->m_strTotData);
			pUp->Upload(m_userID, m_root, 0, 0, true, &child->m_mapH);
			delete  pUp;

			if (pUp->GetGubn() != 0)
			{
				pUp = new CUpload();
				pUp->FormUploadSetting(upDlg->m_strTotData);
				pUp->Upload(m_userID, m_root, 1, 0, true, &child->m_mapH);
				delete pUp;
			}
			delete upDlg;
		}
	}
	else
		MessageBox("Must create Source before Upload.", "Message", MB_OK);
}

void CMainFrame::closeChild()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();

  	if (child)
		child->PostMessage(WM_CLOSE);
}

void CMainFrame::removeChild(void* rChild)
{
	CChildFrame*	child;

	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (child == (CChildFrame *) NULL)
			continue;

		if (child == rChild)
		{
			m_children.RemoveAt(ii);
			if (m_children.GetSize() == 0)	//	no child
			{
				m_wndProperties.ResetControl(2);
				m_wndProperties.SelectControl(-1);
				m_wndScript.ResetCombo(2);
			}
			return;
		}
	}
}

int CMainFrame::generateMap(bool alarm, CChildFrame *child)
{
	if (!child)
		child = (CChildFrame *) MDIGetActive();
	
	if (child->m_mapH.mapK == MK_PROCEDURES)
		child->SaveProcedures();
	else
	{
		child->m_view.SortIoOrder();
		child->m_view.SortTabOrder();
	}

	m_wndScript.WriteScript();
	return generateSource(alarm, &child->m_mapH);
}

void CMainFrame::compileMap(bool all)
{
	CString	srcP, objP, nickP;

	CChildFrame* child;
	AfxGetApp()->DoWaitCursor(1);

	for (int ii = 0; ii < (all ? m_children.GetSize() : 1); ii++)
	{
		if (all)
			child = m_children.GetAt(ii);
		else
			child = (CChildFrame *) MDIGetActive();

		switch (generateMap(false, child))
		{
		case 0:
			child->m_mapH.source = true;
			break;
		case 1:
		case 2:
		default:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			continue;
		}

		srcP = objP = nickP = _T("");
		wccSetTempPath(srcP, child->m_mapH.mapN);
		wccSetTempPath(objP, child->m_mapH.mapN, false);
		child->m_mapH.compile = true;
	}
	AfxGetApp()->DoWaitCursor(-1);
}

void CMainFrame::applyAXIS(bool all)
{
	CString	tmps, path;
	CChildFrame* child;

	for (int ii = 0; ii < (all? m_children.GetSize() : 1); ii++)
	{
		if (all)
			child = m_children.GetAt(ii);
		else
			child = (CChildFrame *) MDIGetActive();

		DWORD	idir = GetFileAttributes(m_root);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || !child->m_mapH.compile)
			return;
		
		path.Format(_T("%s\\%s"), m_EmulPath, MAPDIR);
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		/*
		tmps = CString(child->m_mapH.mapN, L_SGID);
		path += _T("\\");
		path += tmps;
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		//tmps = CString(child->m_mapH.mapN, L_TGID);
		
		tmps = EMULDIR;
		path += _T("\\");
		path += tmps;
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		*/
		tmps = _T("");
		wccSetTempPath(tmps, child->m_mapH.mapN, true);
		path += _T("\\");
		path += child->m_mapH.mapN;
		CopyFile(tmps, path, false);		
	}
}

void CMainFrame::reloadMap()
{
	CString	path, tmps, tmp;
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	wccSetTempPath(tmps, child->m_mapH.mapN);
	
	if (tmps.Find(_T("xml")) == -1)
	{
		tmp.Format("%s%s", tmps, _T(".xml"));
		tmps = tmp;
	}

	if (child->m_mapH.pathsetted)
	{
		path = child->m_mapH.fullN;
		tmps.Format(_T("%s\\%s.xml"), path, child->m_mapH.mapN);
	}

	CFile	file;
	if (!file.Open(tmps, CFile::modeRead|CFile::typeBinary))
		return;
	file.Close();

	if (child->m_mapH.onLoad)
		DeleteFile(child->m_mapH.onLoadN);
	if (child->m_mapH.onSend)
		DeleteFile(child->m_mapH.onSendN);
	if (child->m_mapH.onReceive)
		DeleteFile(child->m_mapH.onReceiveN);
	if (child->m_mapH.onAlert)
		DeleteFile(child->m_mapH.onAlertN);
	if (child->m_mapH.onTimer)
		DeleteFile(child->m_mapH.onTimerN);
	if (child->m_mapH.onClose)
		DeleteFile(child->m_mapH.onCloseN);
	if (child->m_mapH.onDevice)
		DeleteFile(child->m_mapH.onDeviceN);
	if (child->m_mapH.onError)
		DeleteFile(child->m_mapH.onErrorN);
	if (child->m_mapH.onFlicking)
		DeleteFile(child->m_mapH.onFlickingN);
	if (child->m_mapH.onDeclaration)
		DeleteFile(child->m_mapH.onDeclarationN);
	if (child->m_mapH.onInDeclaration)
		DeleteFile(child->m_mapH.onInDeclarationN);

    	child->MDIDestroy();
	child = openMap(tmps);
	if (!path.IsEmpty())
	{
		child->m_mapH.pathsetted = true;
		strcpy_s(child->m_mapH.fullN, sizeof(child->m_mapH.fullN), path);
	}
}

void CMainFrame::showFORM()
{
	CMenu*	mainM = new CMenu();
	
	mainM->Attach(m_wndMenuBar.GetHMenu());
	if (mainM->GetMenuState(ID_VIEW_HIDEFORM, MF_CHECKED) & MF_CHECKED)
	{
		m_showFORM = false;
		mainM->CheckMenuItem(ID_VIEW_HIDEFORM, MF_BYCOMMAND|MF_UNCHECKED);
	}
	else
	{
		m_showFORM = true;
		mainM->CheckMenuItem(ID_VIEW_HIDEFORM, MF_BYCOMMAND|MF_CHECKED);
	}
	mainM->Detach();
	delete mainM;

	CChildFrame*	child;
	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (child == (CChildFrame *) NULL)
			continue;

		child->SetVisible(m_showFORM);
	}
}

void CMainFrame::setSourceViewer()
{
	int	offs;
	CString	inits, exes, titles, sCurrentDir;
	CWinApp* app = AfxGetApp();
	char	path[256];

	titles = _T("Source Edit Application");
	inits  = (char *)SendMessage(WM_USER, ID_USR_GETSRCVIEWER, 0);
	GetCurrentDirectory(sizeof(path), path);
	sCurrentDir = path;
	if (inits.IsEmpty())
	{
		GetWindowsDirectory(path, sizeof(path));
		
		inits = path;
		exes  = _T("notepad.exe");
	}
	else
	{
		offs = inits.ReverseFind(_T('\\'));
		if (offs != -1)
		{
			exes  = inits.Mid(offs+1);
			inits = inits.Left(offs);
		}
		else
			exes = _T("");
	}

	CFileDialog fDlg(TRUE, NULL, exes, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
			_T("Application|*.exe|All files|*.*||"));
	fDlg.m_ofn.lpstrInitialDir = inits;
	fDlg.m_ofn.lpstrTitle = titles;
	if (fDlg.DoModal() != IDOK)
		return;

	inits = fDlg.GetPathName();
	SetCurrentDirectory(sCurrentDir);
	if (!inits.IsEmpty())
		SendMessage(WM_USER, ID_USR_SETSRCVIEWER, (LPARAM)inits.operator LPCTSTR());
}

void CMainFrame::showSource()
{
	CString	tmps, cmdN;

	CChildFrame* child = (CChildFrame *) MDIGetActive();
	wccSetTempPath(tmps, child->m_mapH.mapN);
	HANDLE hFile = CreateFile(tmps, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		generateMap();
	else
		CloseHandle(hFile);

	cmdN.Format(_T("\" %s\""), tmps);

	char	pathN[256];
	CWinApp* app = AfxGetApp();
	tmps = (char *)SendMessage(WM_USER, ID_USR_GETSRCVIEWER, 0);
	if (tmps.IsEmpty())
	{
		GetWindowsDirectory(pathN, sizeof(pathN));
		tmps = pathN;
		tmps += _T("\\notepad.exe");
	}

	tmps = pathN;
	tmps += cmdN;
	WinExec(tmps, SW_SHOWNORMAL);
}

void CMainFrame::setServerIP()
{
	CAddrDlg dlg(m_currentPath);
	dlg.DoModal();
}

void CMainFrame::batchCompile()
{
	CBatch*	batch = new CBatch(this, m_curDir);
	batch->DoModal();
	delete batch;
}

bool CMainFrame::dropInChild()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	if (!child || child->m_view.m_pEmulateWnd)
		return false;

	CRect	wRc;

	child->GetWindowRect(&wRc);
	if (wRc.PtInRect(m_dropPnt))
		return true;

	return false;
}

void CMainFrame::loadControlEvent()
{
	CString path, desc;
	CString tmps, sym;
	char	twb[1024];
	DWORD	rc;

	path.Format(_T("%s\\%s\\control.ini"), m_root, TABDIR);
	m_controlEvent.RemoveAll();

	for (int ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		rc = GetPrivateProfileString(_T("Custom Objects"), tmps, _T(""), twb, sizeof(twb), path);
		if (rc <= 0)
			break;

		desc = CString(twb, rc);
		tmps = parse(desc, _T(";"));
		sym  = parse(desc, _T(";"));
		
		if (tmps.IsEmpty())
			break;
		m_controlEvent.SetAt(tmps, (void*)atoi(desc));
	}
}

void CMainFrame::startWorkshop()
{
	CString	tmps;
	CMapStringToString maps;
	long	screenCX = GetSystemMetrics(SM_CXSCREEN), nScreenCY = GetSystemMetrics(SM_CYSCREEN);

//	if (!m_offline)
//		CSplashWnd::ShowSplashScreen(this);

	if (initDir(maps) == -1)
	{
		PostMessage(WM_CLOSE);
		return;
	}

	tmps.Format(_T("%s\\%s\\%s"), m_root, TABDIR, PALETTE);
	m_palette = new CAmPalette((char *)tmps.operator LPCTSTR());
	m_draw    = new CAmDraw();

	m_displayMode = GetPrivateProfileInt(ENVIRONMENT, _T("DISPLAYMODE"), 0, m_currentPath + _T("\\workshop.ini"));

	int	cx, cy;
	cx = GetSystemMetrics(SM_CXMAXIMIZED) - GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	cy = GetSystemMetrics(SM_CYMAXIMIZED) - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	EnableDocking(CBRS_ALIGN_ANY);

	// 메뉴
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0(_T("메뉴 모음을 만들지 못했습니다.\n"));
		return;
	}

	DWORD	mstyle = m_wndMenuBar.GetPaneStyle();
	mstyle |= (CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	mstyle &= ~CBRS_GRIPPER;
	m_wndMenuBar.SetPaneStyle(mstyle);
	CMFCPopupMenu::SetForceMenuFocus(FALSE);	// 메뉴 모음을 활성화해도 포커스가 이동하지 않게 합니다.

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_TOP);
	DockPane(&m_wndMenuBar);

	// 툴바
	if (!createToolBars())
		return;

	// 상태바
	if (!m_statusBar.Create(this))
	{
		TRACE0(_T("상태 표시줄을 만들지 못했습니다.\n"));
		return;
	}
	m_statusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_statusBar.SetPaneInfo(1, ID_INDICATOR_EXT, SBPS_NORMAL, 80);
	m_statusBar.SetPaneText(1, "");

	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
//	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES,IDB_MENU_IMAGES_24);

	// 도킹 창 - workspacebar, propertiesbar, tracebar..etc.
	m_wndMapView.SetTwoDepth(GetPrivateProfileInt(ENVIRONMENT, _T("SVRDEPTH"), 0, m_currentPath + _T("\\workshop.ini")) ? true : false);
	if (!createDockingWindows())
	{
		TRACE0(_T("도킹 창을 만들지 못했습니다.\n"));
		return;
	}

	// downloading area
	// Download mapgroup.ini
	if (!m_offline)
	{
		m_pGroup = new CGroup();
		if (!m_pGroup->Create(NULL ,NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 1))
			m_offline = true;
	//	m_wndWorkBar.m_bOffline = m_bOffline;
	}

	// create MDI Child Tabs
	RECT	rc;

	::GetWindowRect(m_hWndMDIClient, &rc);
	ScreenToClient(&rc);
	DWORD	dwStyle = MT_BOTTOM | MT_IMAGES;
	m_wndMDITabs.Create(this, rc, dwStyle);
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();

	CSplashWnd::HideSplashScreen();
	m_projectName.Empty();

	// TabbedPane Example
	// http://msdn.microsoft.com/ko-kr/library/bb984597.aspx

	/*m_wndMapView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSymbolView.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndMapView);
	m_tabBar = NULL;
		
	m_wndFileView.AttachToTabWnd(&m_wndMapView, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);
	m_wndSymbolView.AttachToTabWnd(m_tabBar, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);*/
		
	//CMFCBaseTabCtrl* pTabCtrl = m_tabBar->GetUnderlyingWindow();	
	//m_pTab = pTabCtrl;

	/*m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	ShowPane(&m_wndOutput, FALSE, FALSE, FALSE);

	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);	
	ShowPane(&m_wndProperties, FALSE, FALSE, FALSE);

	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);
	rc.left = cx-600;
	rc.top = cy-500;
	rc.right = rc.left+500;
	rc.bottom = rc.top+400;
	m_wndScript.EnableDocking(0);
	m_wndScript.SetPaneStyle(m_wndScript.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);*/
	//m_wndScript.FloatPane(rc, DM_UNKNOWN, false);
	//ShowPane(&m_wndScript, FALSE, FALSE, FALSE);
	
	//HKEY hKey;	

	//RegOpenKeyEx( HKEY_CURRENT_USER, "Software\\amBuilder\\AXIS Moblie V01.00\\Workspace", 0, KEY_QUERY_VALUE, &hKey );
	//::RegDeleteKeyA( hKey, "BasePane-32813");	
	//RegCloseKey( hKey );

//	((CaxisMBuilderApp *)AfxGetApp())->LoadState(this);
//	setToolbarPosition();
	/*
	if (maps.GetCount() > 0)
	{
		if (AfxMessageBox(_T("It is found the backup file.\n\nDo you load file?"), MB_YESNO) == IDYES)
		{
			CString sKey, sPath;
			CFileFind finder;
			for (POSITION pos = maps.GetStartPosition(); pos != NULL; )
			{
				maps.GetNextAssoc(pos, sKey, sPath);
				if (finder.FindFile(sPath))
					openFile(sPath, true);
			}
		}
	}
	*/
}

void CMainFrame::MakeDocking()
{
	int cx, cy;
	CRect rc;

	m_wndMapView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSymbolView.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndMapView);
	m_tabBar = NULL;

	m_wndFileView.AttachToTabWnd(&m_wndMapView, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);
	m_wndSymbolView.AttachToTabWnd(m_tabBar, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);
	rc.left = cx-600;
	rc.top = cy-500;
	rc.right = rc.left+500;
	rc.bottom = rc.top+400;
	m_wndScript.EnableDocking(0);
	m_wndScript.SetPaneStyle(m_wndScript.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndScript.FloatPane(rc, DM_UNKNOWN);//, false);
}



void CMainFrame::changeMap(CString src)
{
	if (src.IsEmpty())
		return;

	int	formN = 0;

	CChildFrame *child = (CChildFrame *) MDIGetActive();
	if (!child)
		return;

	child->m_formItem->RemoveAll();
//	child->DeleteUndoMap();
	if (child->m_mapH.onLoad)
		DeleteFile(child->m_mapH.onLoadN);
	if (child->m_mapH.onSend)
		DeleteFile(child->m_mapH.onSendN);
	if (child->m_mapH.onReceive)
		DeleteFile(child->m_mapH.onReceiveN);
	if (child->m_mapH.onAlert)
		DeleteFile(child->m_mapH.onAlertN);
	if (child->m_mapH.onTimer)
		DeleteFile(child->m_mapH.onTimerN);
	if (child->m_mapH.onClose)
		DeleteFile(child->m_mapH.onCloseN);
	if (child->m_mapH.onDevice)
		DeleteFile(child->m_mapH.onDeviceN);
	if (child->m_mapH.onError)
		DeleteFile(child->m_mapH.onErrorN);
	if (child->m_mapH.onFlicking)
		DeleteFile(child->m_mapH.onFlickingN);
	if (child->m_mapH.onDeclaration)
		DeleteFile(child->m_mapH.onDeclarationN);
	if (child->m_mapH.onInDeclaration)
		DeleteFile(child->m_mapH.onInDeclarationN);

	child->m_mapH.onLoad   = false;
	child->m_mapH.onSend    = false;
	child->m_mapH.onReceive = false;
	child->m_mapH.onAlert   = false;
	child->m_mapH.onTimer   = false;
	child->m_mapH.onClose   = false;
	child->m_mapH.onDevice  = false;
	child->m_mapH.onError     = false;
	child->m_mapH.onFlicking = false;
	child->m_mapH.onDeclaration = false;
	child->m_mapH.onInDeclaration = false;

	child->m_view.SortIoOrder();
	child->m_view.SortTabOrder();
	child->m_view.m_selF = -1;
	PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	child->m_view.Invalidate();
}

void CMainFrame::reCreateMDITabs()
{
	RECT	rc;

	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.DestroyWindow();

	::GetWindowRect(m_hWndMDIClient, &rc);
	ScreenToClient(&rc);
	DWORD dwStyle = MT_BOTTOM | MT_IMAGES;
	m_wndMDITabs.Create(this, rc, dwStyle);
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	RecalcLayout();
}

bool CMainFrame::checkInEditing(CString mapN, BYTE mapK)
{
	CString tmps;
	CChildFrame* child;

	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (child == (CChildFrame *)NULL)
			continue;

		if (!mapN.CompareNoCase(child->m_mapH.mapN) && mapK == child->m_mapH.mapK)
		{
			tmps.Format(_T("\n[%s] Editing same map now.\n\n"), mapN);

			m_statusBar.SetPaneText(0, tmps);
			child->MDIActivate();
			return true;
		}
	}
	return false;
}

void CMainFrame::makeDir(CString root)
{
	CFileFind ff;
	CString	path;

	if (ff.FindFile(root))
	{
		ff.FindNextFile();
		if (!ff.IsDirectory())
			CreateDirectory(root, NULL);
	}
	else
		CreateDirectory(root, NULL);
	ff.Close();
	
	path.Format(_T("%s\\exe"), root);	// exe
	if (ff.FindFile(path))
	{
		ff.FindNextFile();
		if (!ff.IsDirectory())
			CreateDirectory(path, NULL);
	}
	else
		CreateDirectory(path, NULL);
	ff.Close();

	path.Format(_T("%s\\tab"), root);	// tab
	if (ff.FindFile(path))
	{
		ff.FindNextFile();
		if (!ff.IsDirectory())
			CreateDirectory(path, NULL);
	}
	else
		CreateDirectory(path, NULL);
	ff.Close();
}

#include "winsock2.h"
CString CMainFrame::getClientIP()
{
	WORD	wVersionRequested;
	WSADATA	wsaData;
	char	name[256];
	CString	ips;
	struct	hostent* hostinfo;

	wVersionRequested = MAKEWORD(2, 0);
	if (WSAStartup(wVersionRequested, &wsaData) == 0)
	{        
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				for (int ii = 0; hostinfo->h_addr_list[ii] != NULL; ii++)
					ips = inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[ii]);        
			}
		}        
		
		WSACleanup();
	}
	return ips;
}

int CMainFrame::getPort()
{
	switch (GetPrivateProfileInt(ENVIRONMENT, _T("PROXY"), 0, m_currentPath + _T("\\workshop.ini")))
	{
	case 0:	// Local 15101
		return 15101;
	case 2:	// Proxy 15201
		return 15201;
	default:// Internet 80
		break;
	}
	return 80;
}

CString CMainFrame::getControlName(int kind, int type)
{
	switch(kind)
	{
	case FM_LABEL:
		return _T("LABEL");
	case FM_BOX:
		return _T("BOX");
	case FM_GROUP:
		return _T("GROUP");
	case FM_IMAGEVIEW:
		return _T("IMAGEVIEW");
	case FM_BUTTON:
		return _T("BUTTON");
	case FM_RADIO:
		return _T("RADIO");
	case FM_CHECK:
		return _T("CHECK");
	case FM_EDIT:
		return _T("EDIT");
	case FM_COMBO:
		return _T("COMBO");
	case FM_OUT:
		return _T("OUTPUT");
	case FM_GRID:
		return _T("GRID");
	case FM_GRIDEX:
		return _T("GRIDEX");
	case FM_TABLE:
		return _T("TABLE");
	case FM_OBJECT:
		return _T("OBJECT");
	case FM_TAB:
		return _T("TAB");
	case FM_BROWSER:
		return _T("BROWSER");
	case FM_CONTROL:
		return _T("CONTROL");
	}
	return _T("");
}

CString CMainFrame::getErrString(int nCode)
{
	switch (nCode)
	{
	case 0:
		return _T("Duplicated Symbol.");
	case 1:
		return _T("Reserved keyword.");
	}

	if (nCode > 100)
		return (char *)nCode;

	return _T("");
}

CString CMainFrame::parse(CString &src, CString sub)
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

void CMainFrame::OnColumnEdit()
{
	if (m_status & MS_MODAL)
		return;

	CChildFrame* child = (CChildFrame *)MDIGetActive();
	if (child)
		child->OnColumnEdit();
}

LRESULT CALLBACK keyProc(int code, WPARAM wp, LPARAM lp)
{	
	if (/* Resource 중복 */false && code >= 0 && IsWindow(pMainFrm->GetSafeHwnd()) && pMainFrm->IsWindowVisible())
	{
		MSG	msg;

		if (wp == VK_F11)	// F11
		{
			while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE|PM_NOYIELD))
				;
			pMainFrm->OnColumnEdit();
		}
	}
	return CallNextHookEx(pMainFrm->m_hook, code, wp, lp);
}

LRESULT CALLBACK CallProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SIZE:
		{
			RECT	rect;

			GetWindowRect(hwnd, &rect);
			pMainFrm->ScreenToClient(&rect);
			pMainFrm->m_wndMDITabs.SetPosition(rect);
		}
		break;

/*	case WM_MDICREATE:
		LPMDICREATESTRUCT pMCS = reinterpret_cast<LPMDICREATESTRUCT>(lp);
		pMCS->y += 30;	// height of mdiTab
		break;*/
	}

	return ::CallWindowProc((LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))pMainFrm->m_callproc, hwnd, msg, wp, lp);
}

void CMainFrame::OnDestroy()
{
	if (m_pEmul)
	{
		m_pEmul->DestroyWindow();
		delete m_pEmul;	
	}

	
	if (m_pSock)
	{
		m_pSock->Close();
		m_pSock->DestroyWindow();
		delete m_pSock;
	}
	
	CMDIFrameWndEx::OnDestroy();
}

void CMainFrame::rtsInfo()
{
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();
	struct _repR	*repR;
	struct _cellR *cellR;
	CCellItem *cellItem;
	CString tmp = _T("");
	CString tmps = _T("");

	if (m_children.GetSize() && pActiveChild)
	{
		if (m_rtsInfo && !m_rtsInfo->IsWindowVisible())
		{
			delete m_rtsInfo;
			m_rtsInfo = NULL;
		}

		for (int ii = 0; ii < pActiveChild->m_formItem->GetCount(); ii++)
		{
			if (!pActiveChild->m_formItem->GetFormStr(ii, _T("name")).IsEmpty())
			{
				switch (pActiveChild->m_formItem->GetForm(ii)->kind)
				{
				case FM_GRID:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("$=%s"), _T("GRID"));
					tmp += tmps;
					repR = (struct _repR*)pActiveChild->m_formItem->GetForm(ii)->auxR;
					cellItem = new CCellItem(repR);
					for (int jj = 0; jj < repR->cols; jj++)
					{
						cellR = cellItem->GetCell(jj);
						tmps.Format(_T(":%s"), cellR->name);
						tmp += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					tmp += tmps;
					delete cellItem;
					break;
				case FM_GRIDEX:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("$=%s"), _T("GRIDEX"));
					tmp += tmps;
					repR = (struct _repR*)pActiveChild->m_formItem->GetForm(ii)->auxR;
					cellItem = new CCellItem(repR);
					for (int jj = 0; jj < repR->cols; jj++)
					{
						cellR = cellItem->GetCell(jj);
						tmps.Format(_T(":%s"), cellR->name);
						tmp += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					tmp += tmps;
					delete cellItem;
					break;
				case FM_TABLE:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("$=%s"), _T("TABLE"));
					tmp += tmps;
					repR = (struct _repR*)pActiveChild->m_formItem->GetForm(ii)->auxR;
					cellItem = new CCellItem(repR);
					for (int jj = 0; jj < repR->rows; jj++)
					{
						cellR = cellItem->GetCell(jj);
						tmps.Format(_T(":%s"), cellR->name);
						tmp += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					tmp += tmps;
					delete cellItem;
					break;
				case FM_COMBO:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("COMBO"), GB_SYMBOL);
					tmp += tmps;
					break;
				case FM_CONTROL:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("CONTROL"), GB_SYMBOL);
					tmp += tmps;
					break;
				case FM_OUT:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("OUT"), GB_SYMBOL);
					tmp += tmps;
					break;
				case FM_EDIT:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("EDIT"), GB_SYMBOL);
					tmp += tmps;
					break;
				}
			}
		}

		if (m_rtsInfo == NULL)
		{
			m_rtsInfo = new CRtsinfo(this);
			m_rtsInfo->Create(IDD_RTSINFO);
			m_rtsInfo->ShowWindow(SW_SHOW);
		}
		if (!tmp.IsEmpty())
		{
			m_rtsInfo->makeformTR(tmp);
			m_rtsInfo->makeFormsymbol(&pActiveChild->m_mapH);
		}
	}
}

void CMainFrame::trlayout(CString path, bool breset)
{
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();
	struct _repR	*repR;
	struct _cellR *cellR;
	CCellItem *cellItem;
	CString tmp = _T("");
	CString tmps = _T("");
	CString	trname;

	if (m_children.GetSize() && pActiveChild)
	{
		if (m_trRayout && !m_trRayout->IsWindowVisible())
		{
			delete m_trRayout;
			m_trRayout = NULL;
		}
			
		for (int ii = 0; ii < pActiveChild->m_formItem->GetCount(); ii++)
		{
			if (!pActiveChild->m_formItem->GetFormStr(ii, _T("name")).IsEmpty())
			{
				switch (pActiveChild->m_formItem->GetForm(ii)->kind)
				{
				case FM_GRID:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("$=%s"), _T("GRID"));
					tmp += tmps;
					repR = (struct _repR*)pActiveChild->m_formItem->GetForm(ii)->auxR;
					cellItem = new CCellItem(repR);
					for (int jj = 0; jj < repR->cols; jj++)
					{
						cellR = cellItem->GetCell(jj);
						tmps.Format(_T(":%s"), cellR->name);
						tmp += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					tmp += tmps;
					delete cellItem;
					break;
				case FM_GRIDEX:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("$=%s"), _T("GRIDEX"));
					tmp += tmps;
					repR = (struct _repR*)pActiveChild->m_formItem->GetForm(ii)->auxR;
					cellItem = new CCellItem(repR);
					for (int jj = 0; jj < repR->cols; jj++)
					{
						cellR = cellItem->GetCell(jj);
						tmps.Format(_T(":%s"), cellR->name);
						tmp += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					tmp += tmps;
					delete cellItem;
					break;
				case FM_TABLE:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("$=%s"), _T("TABLE"));
					tmp += tmps;
					repR = (struct _repR*)pActiveChild->m_formItem->GetForm(ii)->auxR;
					cellItem = new CCellItem(repR);
					for (int jj = 0; jj < repR->rows; jj++)
					{
						cellR = cellItem->GetCell(jj);
						tmps.Format(_T(":%s"), cellR->name);
						tmp += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					tmp += tmps;
					delete cellItem;
					break;
				case FM_COMBO:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("COMBO"), GB_SYMBOL);
					tmp += tmps;
					break;
				case FM_CONTROL:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("CUSTOM"), GB_SYMBOL);
					tmp += tmps;
					break;
				case FM_OUT:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("OUTPUT"), GB_SYMBOL);
					tmp += tmps;
					break;
				case FM_EDIT:
					tmp += pActiveChild->m_formItem->GetFormStr(ii, _T("name"));
					tmps.Format(_T("=%s%c"), _T("EDIT"), GB_SYMBOL);
					tmp += tmps;
					break;
				}
			}
		}

		bool bfile = false;

		if (m_trRayout == NULL)
		{
			m_trRayout = new CTrLayOut(this);
			m_trRayout->Create(IDD_TRLAYOUT);
			m_trRayout->ShowWindow(SW_SHOW);
		}

		if (path.IsEmpty() && pActiveChild->m_mapH.trinfo != NULL)
		{
			CString trinfo, trName;
			int pos;
			trinfo = CString(pActiveChild->m_mapH.trinfo, strlen(pActiveChild->m_mapH.trinfo));
			pos = trinfo.Find("/");
			trName = trinfo.Left(pos);
			path.Format("%s\\trlayout\\%s", m_root, trName);				
		}

		m_trRayout->readTrList(path);
		if (!path.IsEmpty())
			bfile = true;

		if (!tmp.IsEmpty())
			m_trRayout->formObjectLayout(&pActiveChild->m_mapH, tmp, bfile);	
	}
}

void CMainFrame::SettTrRtsDataset(int kind, CString data)
{
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();
	int len;

	if (pActiveChild)
	{
		switch (kind)
		{
		case 0:
			len = strlen(data);
			pActiveChild->m_mapH.trinfo = new char[len+1];
			CopyMemory(pActiveChild->m_mapH.trinfo, data, len);
			pActiveChild->m_mapH.trinfo[len] = 0x00;
//TRACE("-------[SettTrRtsDataset_TRINFO]-----------%s\n", data);
			break;
		case 1:
			len = strlen(data);
			pActiveChild->m_mapH.rtsinfo = new char[len+1];
			CopyMemory(pActiveChild->m_mapH.rtsinfo, data, len);
			pActiveChild->m_mapH.rtsinfo[len] = 0x00;
//TRACE("-------[SettTrRtsDataset_RTS]-----------%s\n", data);
			break;
		}
	}
}

LRESULT CMainFrame::trMaptree(WPARAM wParam, LPARAM lParam)
{
	// Login Timeout
	KillTimer(100);

	struct _rsmH *rsmH = (struct _rsmH *)lParam;
	CString serrNO = CString(rsmH->erno, sizeof(rsmH->erno));

	MakeDocking();
	CMFCBaseTabCtrl* pTabCtrl = m_tabBar->GetUnderlyingWindow();
	
	if (atoi(serrNO) > 0)
	{
		m_bShowFlag = true;
		//ShowWindow(SW_SHOW);
		if (IsWindow(pTabCtrl->m_hWnd))
			pTabCtrl->SetActiveTab(1);

		/// not login
		m_offline = true;
		m_loginDlg.ShowWindow(SW_HIDE);
		m_loginDlg.DestroyWindow();

		if (m_pSock)
			m_pSock->Close();

		MessageBox("서버에 로그인 되지 않았습니다. 오프라인 모드입니다.", "Message", MB_OK);
		return 0;
	}

	///login
	m_bShowFlag = true;
	//ShowWindow(SW_SHOW);
	if (IsWindow(pTabCtrl->m_hWnd))
		pTabCtrl->SetActiveTab(0);

	m_offline = false;
	m_loginDlg.ShowWindow(SW_HIDE);
	
	if (m_pSock)
		m_pSock->Close();

	LoginEmule();	
	
	SetTimer(500, 500, NULL);
	return 0;
}


BOOL CMainFrame::LoginEmule()
{
	// sise login
	CString ips, sport, port;

	GetPrivateProfileString(SERVER, SHOPIPS, _T(""), ips.GetBuffer(50), 50, m_currentPath + _T("\\workshop.ini"));
	ips.ReleaseBuffer();

	GetPrivateProfileString(SERVER, _T("SPORT"), _T(""), port.GetBuffer(50), 50, m_currentPath + _T("\\workshop.ini"));
	port.ReleaseBuffer();
	
	GetPrivateProfileString(SERVER,  _T("PROXY"), _T(""), sport.GetBuffer(50), 50, m_currentPath + _T("\\workshop.ini"));
	sport.ReleaseBuffer();

	long nPort = 15101;
	if (port.IsEmpty())
	{	
		switch(atoi(sport))
		{
		case 1:
			nPort = 15201;
			break;
		case 2:
			nPort = 80;
			break;
		default:
			nPort = 15101;
			break;
		}
	}
	else
		nPort = atoi(port);
	
	return m_pEmul->RunAxis(loginSHOP, (LONG)ips.GetBuffer(0), nPort);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 100)
	{
		KillTimer(100);
		OfflineMode();
	}
	else if (nIDEvent == 500)
	{
		KillTimer(500);
		
		CString tmps;
		CDownload* download = new CDownload();
		download->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 2000);
		wccSetTempPath(tmps, DN_FILE);
		download->Download(m_userID, _T(""), _T(""), tmps, TRGB_FORMALL, false);
		delete download;

		download = new CDownload();
		download->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 2000);
		wccSetTempPath(tmps, TR_FILE);
		download->Download(m_userID, _T(""), _T(""), tmps, TRGB_TRLAYOUT, false);
		delete download;
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}


CString CMainFrame::setMapPath(CString mapN)
{
	CString tmps;

	if (mapN.GetLength() != L_MAPN)
		return _T("");

	tmps.Format(_T("map/%s/%s/%s"), mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	return tmps;
}

LRESULT CMainFrame::onsockmessage(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

void CMainFrame::Login(CString sUserID, CString sPass)
{
#ifdef _DEBUG
	sUserID = _T("winix0");
	sPass   = _T("winix");
#endif

	CString ips;

	GetPrivateProfileString(SERVER, "IP", _T(""), ips.GetBuffer(50), 50, m_currentPath + _T("\\workshop.ini"));
	ips.ReleaseBuffer();

	/// offline mode
	if (ips.CompareNoCase("0.0.0.0") == 0)
	{
		OfflineMode();
		return;
	}


	m_userID = sUserID;

	struct	_rsmH* prsmH;
	struct _login* plog;
	int len;

	len = L_rsmH + sizeof(struct _login);
	char *pBuffer = new char[len + 1];
	FillMemory(pBuffer, len, ' ');
	prsmH = (struct _rsmH *)pBuffer;
	plog  = (struct _login*)(pBuffer + L_rsmH);

	CString sDat;
	sDat.Format("%08d",  sizeof(struct _login));
	prsmH->dirF = dirF_OUTB;
	prsmH->resC = resC_ASCII;
	prsmH->resK = resK_LOGIN;
	prsmH->resF = resF_ONLY;

	CopyMemory(prsmH->datL, sDat,	 min(sizeof(prsmH->datL), sDat.GetLength())    );
	CopyMemory(plog->usid,  sUserID, min(sizeof(plog->usid),  sUserID.GetLength()) );
	CopyMemory(plog->pswd,  sPass,   min(sizeof(plog->pswd),  sUserID.GetLength()) );

	// false일 경우	
	if (!m_pSock->Write(pBuffer, len))
		OfflineMode();
	else
		SetTimer(100, 3000, NULL);

	delete[] pBuffer;
}


void CMainFrame::DownMapDlg(CString sName, int nSel)
{
	CString sRoot;
	sRoot.Format("%s\\exe\\tmp\\%s", m_root, sName);
	CDownload* pDown = new CDownload();
	pDown->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 10);
	pDown->Download(m_userID, sName, "", sRoot, TRGB_FORMSORCE, nSel);

	delete pDown;
}

void CMainFrame::DownTRlayOut(CString sName, int nSel)
{
	CString sRoot;
	sRoot.Format("%s\\trlayout\\%s", m_root, sName);
	CDownload* pDown = new CDownload();
	pDown->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 10);
	pDown->Download(m_userID, sName, "", sRoot, TRGB_FORMSORCE, nSel);

	delete pDown;
}

void CMainFrame::CheckOutDlg(CString idno, CString sName)
{
	CCheckOutDlg dlg;
	dlg.SetFormName(sName);
	if (dlg.DoModal() == IDOK)
	{
		CDownload* pDown = new CDownload();
		pDown->Create(NULL, NULL, WS_CHILD, CRect(0), this, 10);
		pDown->Download(m_userID, sName, "", "", TRGB_FORMCHECK, atoi(idno));

		delete pDown;
	}
}

LRESULT CMainFrame::UploadRecvData(WPARAM wParam, LPARAM lParam)
{
	struct	_mapH*	m_mapH;
	CString strMsg;
	int iFind;

	m_mapH = (struct _mapH*)lParam;
	strMsg = (char*)wParam;

	CStringArray arstrMsg;
	for (int ii = 0; ; ii++)
	{
		if (strMsg.IsEmpty())
			break;
		iFind = strMsg.Find("/");
		arstrMsg.Add(strMsg.Left(iFind));
		strMsg = strMsg.Mid(iFind+1);
	}

	//CUpload *pUpload = new CUpload();
	//pUpload->Upload();

	return 0;
}



void CMainFrame::GetHistoryDlg(CString sMap)
{
	if (m_resouceDlg) 
		SAFE_DELETE(m_resouceDlg);

	CString strPath;

	m_resouceDlg = new CResourceHistDlg(this);
	m_resouceDlg->SetName(sMap);
	m_resouceDlg->SetRoot(m_root);
	m_resouceDlg->SetID(m_userID);
	sMap = sMap.MakeUpper();
	if (sMap.Find(_T(".XML")) == 0)
		strPath.Format("%s\\%s.XML",m_SourcePath, sMap);		
	else
		strPath.Format("%s\\%s",m_SourcePath, sMap);
	m_resouceDlg->setPath(strPath);
	m_resouceDlg->Create(IDD_RESOURCE_HISTORY);
	m_resouceDlg->ShowWindow(SW_SHOW);

}

// workshop.ini에등록된 해당 경로 읽어오기
void CMainFrame::moveMobileDevice()
{
	CString	adbPath; adbPath.Format("%s/%s/%s", m_root, "adb", "adb.exe");
	CFileFind finder;
	if (!finder.FindFile(adbPath))
	{
		AfxMessageBox("file not found [adb.exe]");
		return;
	}

	CString runAPP = _T("com.winix.axMobileApp/com.winix.app.WinixActivity");

	// copy
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (!child) return;

	CString	mapPath = _T(""); wccSetTempPath(mapPath, child->m_mapH.mapN);

	char	wb[512];
	GetPrivateProfileString(_T("MOBILE"), _T("FORMPATH"), _T("\\sdcard\\WinixMobile\\form"), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
	CString strFormPath = wb;

	AfxMessageBox(strFormPath);

	CString	string;
	string.Format("\"%s\" push %s %s", adbPath, mapPath, _T("\\sdcard\\WinixMobile\\form"));

	AfxMessageBox(string);

	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;
	BOOL			bRc;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	bRc = CreateProcess(NULL, (LPSTR)string.operator LPCSTR(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bRc)
	{
		TRACE("errno = [%s]\n", GetLastError());
		AfxMessageBox("화면복사 오류입니다.");
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);


	string.Format("\"%s\" shell am start -n %s", adbPath, runAPP);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	bRc = CreateProcess(NULL, (LPSTR)string.operator LPCSTR(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bRc)
	{
		AfxMessageBox("앱 실행 오류입니다.");
		return;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
}


void CMainFrame::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CMDIFrameWndEx::OnWindowPosChanging(lpwndpos);

	if(m_bShowFlag)  
		lpwndpos->flags |= SWP_SHOWWINDOW;
	else
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
}

bool CMainFrame::OpenServerMap()
{
	CString text;
	CChildFrame* child;
	CStringArray files;
	int	count = m_wndMapView.GetFiles(files);
	for (int ii = 0; ii < count; ii++)
	{
		if (m_children.GetSize() >= MAX_MAP)
			break;
					
		wccSetTempPath(text, (char *)files.GetAt(ii).operator LPCSTR());
		child = openMap(text);
		if (child != (CChildFrame *) NULL)
			child->m_mapH.pathsetted = false;
	}

	return true;
}

void CMainFrame::OfflineMode()
{
	m_bShowFlag = true;

	/// not login
	m_offline = true;
	m_loginDlg.ShowWindow(SW_HIDE);

	if (m_pSock)
	{
		m_pSock->Close();
		m_pSock->DestroyWindow();
		delete m_pSock;
		m_pSock = NULL;
	}

	MessageBox("서버에 로그인 되지 않았습니다. 오프라인 모드입니다.", "Message", MB_OK);
}