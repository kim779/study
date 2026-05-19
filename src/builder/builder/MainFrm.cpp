
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "axisBuilder.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "splashWnd.h"
#include "Group.h"

#include "h/mainvar.h"
#include "../h/axisfire.h"
#include "../dll/lib/axislib.h"
#include "h/iForm.h"

#include "awBuild/libBuild.h"

#include "awDlg/addrDlg.h"
#include "awDlg/batch.h"
#include "awDlg/editOrder.h"
#include "awDlg/lineDlg.h"
#include "awDlg/saveDlg.h"
#include "awDlg/setupSheet.h"
#include "awDlg/upDlg.h"
#include "awDlg/SearchDlg.h"
#include "awDlg/TrLayout.h"
#include "awDlg/RtsInfo.h"
#include "awDlg/LayoutUpload.h"
#include "awDlg/DiffViewerDlg.h"
#include "awDlg/DiffViewerDlg.h"
#include "awDlg/ADBDlg.h"
#include "awDlg/CustomObjDlg.h"
#include "LoadTrlayout.h"

#include "awTool/awObjectLoad.h"
#include "awTool/awFormCtrl.h"
#include "awSock/builderLogin.h"
#include "ResourceHistDlg.h"
#include "CheckOutDlg.h"
#include "h/workio.h"
#include "awSock/download.h"
#include "FormDefault.h"
#include "h\seq.H"

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
	ON_WM_SETTINGCHANGE()
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, &CMainFrame::OnUpdateEditFindReplace)
	ON_COMMAND(ID_EDIT_REPLACE, &CMainFrame::OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, &CMainFrame::OnUpdateEditFindReplace)
	ON_COMMAND_RANGE(ID_NEW_MAP, ID_WINDOWS_CLOSE, &CMainFrame::OnMainCommand)
	ON_COMMAND_RANGE(ID_ETC_FORMAT, ID_ETC_CORGB, &CMainFrame::OnEtcCommand)
	ON_COMMAND_RANGE(ID_AL_LEFT, ID_SZ_VEQUAL, &CMainFrame::OnAlignCommand)
	ON_COMMAND_RANGE(ID_TOOL_POINTER, ID_TOOL_OBJECT, &CMainFrame::OnDrawCommand)
	ON_COMMAND_RANGE(ID_BOOKMARK_BASE, ID_BOOKMARK_BASE+50, &CMainFrame::OnBookmark)
	ON_UPDATE_COMMAND_UI_RANGE(ID_NEW_MAP, ID_WINDOWS_CLOSE, &CMainFrame::OnUpdateMainCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_ETC_FORMAT, ID_ETC_CORGB, &CMainFrame::OnUpdateEtcCOmmand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AL_LEFT, ID_SZ_VEQUAL, &CMainFrame::OnUpdateAlignCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_POINTER, ID_TOOL_OBJECT, &CMainFrame::OnUpdateDrawCommand)
	ON_COMMAND(ID_EDIT_FORMSEARCH, &CMainFrame::OnEditFormsearch)
	ON_MESSAGE(WM_OPENOBJ, &CMainFrame::OnOpenObject)
	ON_MESSAGE(WM_USER, &CMainFrame::OnMessage)
	ON_MESSAGE(WM_LOGIN, &CMainFrame::OnLoginMessage)
	ON_WM_SIZE()
	ON_WM_TIMER()
	
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_EVENTSINK_MAP(CMainFrame)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CMainFrame, -1, 1, OnFireEvent, VTS_I4 VTS_I8 VTS_I8)
END_EVENTSINK_MAP()

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
	m_pToken  = new CMapToken[MAX_TOKEN_N];
	m_pGroup  = NULL;

	m_palette = (CAxisPalette *) NULL;
	m_draw    = (CAxisDraw *) NULL;
	m_children.RemoveAll();

	//ZeroMemory((char *)&m_clipMAP, sizeof(_mapH));
	m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), -GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	m_dropPnt  = CPoint(-1, -1);
	m_status   = 0;

	m_wizard = NULL;

	m_showFORM = true;
	m_initMap = _T("");
	m_curDir   = _T("");

	m_tabBar = NULL;

	m_hook = NULL;
	m_callproc = NULL;
	m_piForm = NULL;
	m_resouceDlg = NULL;
	m_builderLogin = NULL;
	m_SearchDlg = NULL;

	m_trDlg = NULL;
	m_rtsDlg = NULL;
	m_bBKLine = false;
	m_bMasterLayout = false;
	m_strDiffViewer = _T("");
	m_pCADBDlg = NULL;
	m_bLayoutMode = false;
	m_strCtmObj = _T("");
}

CMainFrame::~CMainFrame()
{
	if (m_callproc)
		SetWindowLongPtr(m_hWndMDIClient, GWLP_WNDPROC, (LONG_PTR)m_callproc);

	if (m_hook)
		::UnhookWindowsHookEx(m_hook);

	m_tips.DestroyWindow();

	if (m_wizard)
		delete m_wizard;

	if (m_pGroup)
		delete m_pGroup;

	delete[] m_pToken;
	if (m_palette)
		delete m_palette;
	if (m_draw)
		delete m_draw;

	BuilderLogout();

	deleteAryCloneObj();

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

	m_curDir.TrimLeft();
	m_curDir.TrimRight();

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


	m_controlEvent.RemoveAll();

	m_mainTB.RemoveAllButtons();
	m_drawTB.RemoveAllButtons();
	m_alignTB.RemoveAllButtons();
	m_etcTB.RemoveAllButtons();

	if (m_rtsDlg)
	{
		m_rtsDlg->DestroyWindow();
		delete m_rtsDlg;
	}

	if (m_trDlg)
	{
		m_trDlg->DestroyWindow();
		delete m_trDlg;
	}

	if (m_piForm)
		delete m_piForm;

	CObjectLoad* FormLoad = NULL;
	for (size_t ii = 0; ii < (size_t)m_ArryNewMap.GetCount(); ii++)
	{
		FormLoad = (CObjectLoad*)m_ArryNewMap.GetAt(ii);
		delete FormLoad;
	}
	m_ArryNewMap.RemoveAll();
	m_aryFile.RemoveAll();

	if (m_pCADBDlg) delete m_pCADBDlg;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	TCHAR	wb[1024];

	GetCurrentDirectory(sizeof(wb), wb);
	m_currentPath = wb;

	lpCreateStruct->style |= WS_CLIPCHILDREN;
	lpCreateStruct->style &= ~WS_VISIBLE;

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));

	// login
	m_loginDlg.m_currentPath = m_currentPath;
	m_loginDlg.Create(IDD_LOGIN);
	m_loginDlg.ShowWindow(SW_SHOW);
	m_loginDlg.SetParentW(this);


	DragAcceptFiles();

	m_tips.Create(this);

	pMainFrm = this;
	m_hook = SetWindowsHookEx(WH_KEYBOARD, keyProc, AfxGetInstanceHandle(), GetCurrentThreadId());

	HANDLE	instance = (HANDLE)GetWindowLongPtr(m_hWndMDIClient, GWLP_HINSTANCE);
	FARPROC	callproc = (FARPROC)MakeProcInstance(CallProc, instance);
	m_callproc = (FARPROC)SetWindowLongPtr(m_hWndMDIClient, GWLP_WNDPROC, (LONG_PTR)callproc);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (WS_VSCROLL|WS_HSCROLL);

	if (!CMDIFrameWndEx::PreCreateWindow(cs) )
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

//	((CaxisBuilderApp *)AfxGetApp())->SaveState(this);

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

	if (m_wizard)
	{
		BOOL	retvalue = false;
		m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4), loginSHOP, 0, -1);
	}

	tmps.Format("%d", m_wndMapView.IsTwoDepth());
	WritePrivateProfileString(ENVIRONMENT, _T("SVRDEPTH"), tmps, m_currentPath + _T("\\workshop.ini"));
	tmps.Format("%d", m_wndTmplView.IsTwoDepth());
	WritePrivateProfileString(ENVIRONMENT, _T("TMPDEPTH"), tmps, m_currentPath + _T("\\workshop.ini"));

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

			if (m_bMasterLayout)
			{
				if (m_MTLayoutDlg.DoModal() == IDOK)
				{
					int iSel = m_MTLayoutDlg.GetSel();
					if (iSel < m_aryFile.GetCount())
					{
						CString dir;
						dir = m_aryFile.GetAt(iSel);
						openFile(dir, true);
					}
					else
					{
						createNewChild(MK_NORM);
					}
				}
				else
				{
					createNewChild(MK_NORM);
				}
				
			}
			else if (fFind.FindFile(m_initMap) && !(GetFileAttributes(m_initMap) & FILE_ATTRIBUTE_DIRECTORY))
				openFile(m_initMap, true);
			else if (m_initMap.Mid(m_initMap.ReverseFind('.')) == _T(".xml"))
				m_wndMapView.OpenMap(m_initMap);
			else
				createNewChild(MK_NORM);
		}
		break;

	case ID_NEW_TMPL:
		createNewChild(MK_TEMPLATE);
		break;

	case ID_NEW_PROCEDURE:
		createNewChild(MK_PROCEDURES);
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
		else
			saveTmplToServer();
		break;

	case ID_FILE_SAVETOPC:
		saveLocalFile();
		break;

	case ID_FILE_SAVEAS:
		saveLocalPath();
		break;
	case ID_FILE_SETMASTERLAYOUT:
		SetMasterlayout();
		break;
	case ID_FILE_RESETMASTERLAYOUT:
		ResetMasterlayout();
		break;
	case ID_APP_SETTING:
		{
			CSetupSheet dlg(_T("Setting options"), this);
			if (dlg.DoModal() == IDOK)
			{
				bool	bInvalidate = false;

				dlg.Save();
				m_root = dlg.m_basePage->m_path;

				m_sDefFontName = dlg.m_defaultPage->m_fontName;
				m_sDefFontPnt  = dlg.m_defaultPage->m_fontPoint;
				m_initMap      = dlg.m_basePage->m_maps;
				if (dlg.m_basePage->m_displayMode != m_displayMode)
					bInvalidate = true;
				m_displayMode = dlg.m_basePage->m_displayMode;

				loadScriptColors();
				SendMessage(WM_USER, ID_USR_SETROOTDIR, (LPARAM)m_root.operator LPCTSTR());
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
			for (int ii = 0; ii < 4; ii++)
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
		if (m_AryCloneObject.GetCount() > 0 || (child && child->m_mapH.mapK == MK_PROCEDURES))
			child->Paste();
		break;

	case ID_EDIT_SELECTALL:
		child = (CChildFrame *) MDIGetActive();
		if (child->m_mapH.forms.GetCount() > 0)
		{
			child->m_view.m_selF = 0;
			child->m_view.SelObj(-1);
			for (int ii = 0; ii < child->m_mapH.forms.GetCount(); ii++)
				child->m_view.SelAddObj(ii, (child->m_mapH.forms.GetCount() > 1));
			SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
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
			CString sRedo = child->PopRedo();
			if (!sRedo.IsEmpty())
			{
				child->PushUndo(getSource(false, &child->m_mapH));
				changeMap(sRedo);
			}
		}
		break;

	case ID_EDIT_MAPUNDO:
		child = (CChildFrame *) MDIGetActive();
		if (child)
		{
			CString sUndo = child->PopUndo();
			if (!sUndo.IsEmpty())
			{
				child->PushRedo(getSource(false, &child->m_mapH));
				changeMap(sUndo);
			}
		}
		break;

	case ID_EDIT_TABORDER:
		child = (CChildFrame *) MDIGetActive();
		//for (int ii = 0; ii < child->m_mapH.forms.GetCount(); ii++)
		//	((CformProp *)child->m_mapH.forms.GetAt(ii))->block = false;
		child->DrawOrderNumber(1);
		break;

	case ID_EDIT_IOORDER:
		child = (CChildFrame *) MDIGetActive();
		child->DrawOrderNumber(0);
		break;

	case ID_EDIT_TABORDERx:
		{
			child = (CChildFrame *) MDIGetActive();
			child->DrawOrderNumber(1);

			CEditOrder dlg(NULL, &child->m_mapH, 1);	// taborder
			if (dlg.DoModal() == IDOK)
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
			child->ClearOrderNumber();
		}
		break;

	case ID_EDIT_IOORDERx:
		{
			child = (CChildFrame *) MDIGetActive();
			child->DrawOrderNumber(0);

			CEditOrder dlg(NULL, &child->m_mapH);
			if (dlg.DoModal() == IDOK)
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
			child->ClearOrderNumber();
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

	case ID_VIEW_WORKSPACE:
		visible = (m_tabBar->GetStyle() & WS_VISIBLE) ? TRUE : FALSE;
		ShowPane(m_tabBar, !visible, FALSE, FALSE);
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
	case ID_VIEW_DIFFVIEWER:
		SetDiffViewer();
		break;
	case ID_VIEW_LOGCAT:
		ShowADBViewer();
		break;
	case ID_VIEW_CTMMOBJECT:
		ShowCtmObjDlg();
		break;
	case ID_VIEW_HIDEFORM:
		showFORM();
		break;

	case ID_VIEW_REFRESH:
		child = (CChildFrame *) MDIGetActive();
		child->m_view.m_FmCtrl->InvalidateObject();
		child->m_view.SelObj(child->m_view.m_selF);
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
		break;

	case ID_COMPILE_COMPILEALL:
		compileMap(true);
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
	case ID_VIEW_TRLAYOUT:
		editTrLayout();
		break;
	case ID_VIEW_RTSINFO:
		editRtsInfo();
		break;
	case ID_VIEW_RESOURCEHIST:
	{
		if (m_resouceDlg)
			SAFE_DELETE(m_resouceDlg);
		m_resouceDlg = new CResourceHistDlg(this);
		m_resouceDlg->SetRoot(m_root);
		m_resouceDlg->Create(IDD_RESOURCE_HISTORY);
		m_resouceDlg->CenterWindow();
		m_resouceDlg->ShowWindow(SW_SHOW);
	}
		break;
	case ID_EDIT_FORMSEARCH:
	{
		if (m_SearchDlg)
			SAFE_DELETE(m_SearchDlg);
		m_SearchDlg = new CSearchDlg();
		m_SearchDlg->SetRoot(m_root);
		m_SearchDlg->Create(IDD_SEARCH);
		m_SearchDlg->CenterWindow();
		m_SearchDlg->ShowWindow(SW_SHOW);
	}
		break;
	case ID_VIEW_STANDARDLINE:
		m_bBKLine = m_bBKLine ? false : true;
		for (int ii = 0; ii < m_children.GetSize(); ii++)
		{
			child = m_children.GetAt(ii);
			if (IsWindow(child->m_hWnd))
			{
				child->m_view.m_bBKLine = m_bBKLine;
				child->m_view.UnSelBKLine();
				//child->m_view.Invalidate();
			}
		}
		
		break;
	default:
		break;
	}
}

void CMainFrame::OnEtcCommand(UINT cmdID)
{
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	int	edit = child->EditForm(cmdID);
	CMenu*	mainM = new CMenu();
	
	mainM->Attach(m_wndMenuBar.GetHMenu());
	mainM->CheckMenuItem(ID_ETC_FORMAT, MF_BYCOMMAND|MF_UNCHECKED);
	mainM->CheckMenuItem(ID_ETC_CORGB,  MF_BYCOMMAND|MF_UNCHECKED);

	switch (edit)
	{
	case weIT:
		mainM->CheckMenuItem(ID_ETC_FORMAT, MF_BYCOMMAND|MF_CHECKED);
		break;
	case weCORGB:
	//case IE_CORGB2:
		mainM->CheckMenuItem(ID_ETC_CORGB, MF_BYCOMMAND|MF_CHECKED);
		break;
	case weNOP:
	default:
		break;
	}
	mainM->Detach();
	delete mainM;
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
	else if (cmdID == ID_TOOL_OBJECT)
		ShowCtmObjDlg();
	else
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		if (child)
			child->DrawForm(cmdID);
	}
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
	case ID_NEW_TMPL:
	case ID_NEW_PROCEDURE:
		if (m_children.GetSize() < MAX_MAP)
			enable = TRUE;
		break;

	case ID_FILE_SAVETOSERVER:
		tmps = (char *)SendMessage(WM_USER, ID_USR_GETIPS, 0);
		if (child && child->m_mapH.source && !tmps.IsEmpty() && !m_offline)
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
		if (m_AryCloneObject.GetCount() > 0 || (child && child->m_mapH.mapK == MK_PROCEDURES))
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
		enable = TRUE;
		break;

	case ID_FILE_OPENPC:
	case ID_APP_SETTING:
	case ID_COMPILE_BATCHCOMPILE:
	case ID_VIEW_SETVIEWER:
		enable = TRUE;
		break;
	case ID_VIEW_RTSINFO:
	case ID_VIEW_TRLAYOUT:
	case ID_VIEW_RESOURCEHIST:
		if (child)
			enable = TRUE;
		break;
	case ID_VIEW_STANDARDLINE:
		enable = TRUE;
		pCmdUI->SetCheck(m_bBKLine ? 1 : 0);
		break;
	case ID_EDIT_FORMSEARCH:
	case ID_FILE_SETMASTERLAYOUT:
	case ID_FILE_RESETMASTERLAYOUT:
	case ID_VIEW_DIFFVIEWER:
	case ID_VIEW_LOGCAT:
	case ID_VIEW_CTMMOBJECT:
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
	{
		if(child->m_mapH.media == MEDIA_HTS)
			enable = TRUE;
		else if (child->m_mapH.media == MEDIA_MTS)
		{
			switch (pCmdUI->m_nID)
			{
			case ID_TOOL_MEMO:
			case ID_TOOL_TREE:
				enable = FALSE;
				break;
			default:
				enable = TRUE;
				break;
			}
		}
	}
	else
		enable = FALSE;

	if (m_bBKLine)
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
		m_pass = m_loginDlg.m_pass;
		m_domain = m_loginDlg.m_domain;
		loginWizard();
		login();
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
		if (!m_projectName.IsEmpty())
			m_root = AfxGetApp()->GetProfileString(ENVIRONMENT, ROOTDIR);
		else
		{
			char	wb[512];

			GetPrivateProfileString(ENVIRONMENT, ROOTDIR, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
			m_root = wb;
		}
		return (LONG_PTR)m_root.operator LPCTSTR();
	case ID_USR_GETCTMOBJ:
		return (LONG_PTR)m_strCtmObj.operator LPCTSTR();

	case ID_USR_GETCURRENTPATH:	// get currentpath
		return (LONG_PTR)m_currentPath.operator LPCTSTR();
	case ID_USR_DATASET:
		setDataset(pActiveChild, HIWORD(wParam), (char *)lParam);
		break;
	case ID_USR_TRLAYOUT:
		if (m_trDlg && m_trDlg->IsWindowVisible())
			editTrLayout();
		break;
	case ID_USR_LOADTRLAYOUT:
	{
		if (pActiveChild == NULL)
			return 0;
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		CLoadTrlayout loadTrlayout;
		loadTrlayout.loadTrLayoutFile("");
		CPoint pt = m_dropPt;
		int iGap = 12;
		pt.Offset(-iGap, -iGap);
		CString strData, tmps, strName;
		for (int ii = 0; ii < loadTrlayout.m_in.GetCount(); ii++)
		{
			strData = loadTrlayout.m_in.GetAt(ii);
			while (!strData.IsEmpty())
			{
				tmps = loadTrlayout.Parser(strData, _T(","));
				strName = pActiveChild->m_view.AddObject(_T("I"), _T(""), &pt, iGap, tmps);
				loadTrlayout.AddTrLayout(_T("I"), strName, tmps);
			}
		}

		for (int ii = 0; ii < loadTrlayout.m_out.GetCount(); ii++)
		{
			strData = loadTrlayout.m_out.GetAt(ii);
			while (!strData.IsEmpty())
			{
				tmps = loadTrlayout.Parser(strData, _T(","));
				strName = pActiveChild->m_view.AddObject(_T("O"), _T(""), &pt, iGap, tmps);
				loadTrlayout.AddTrLayout(_T("O"), strName, tmps);
			}
		}

		loadTrlayout.setMapH();
	}
		break;
	case ID_USR_NEWTRLAYOUT:
		editTrLayout((char *)lParam);
		break;
	case ID_USR_RTSINFO:
		if (m_rtsDlg && m_rtsDlg->IsWindowVisible())
			editRtsInfo();
		break;
	case ID_USR_GETIPS:		// get ips
		{
			char	wb[512];

			GetPrivateProfileString(ENVIRONMENT, SHOPIPS, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
			text = wb;
			return (LONG_PTR)text.operator LPCTSTR();
		}

	case ID_USR_GETSRCVIEWER:	// get source viewer
		{
			char	wb[512];

			GetPrivateProfileString(ENVIRONMENT, SRCVIEWER, _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
			text = wb;
			return (LONG_PTR)text.operator LPCTSTR();
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

	case ID_USR_SETIPS:		// set ips
		WritePrivateProfileString(ENVIRONMENT, SHOPIPS, (char *)lParam, m_currentPath + _T("\\workshop.ini"));
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

	case ID_USR_ISDROPAREA:		// drop area?
		{
			CRect	rc, rc2;
			CPoint pnt = m_dropPt = CPoint(LOWORD(lParam), HIWORD(lParam));

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
			return (LONG_PTR)text.operator LPCTSTR();
		}
		break;

	case ID_USR_SERVEROPEN:		// server open
		if (lParam == 1) // template
		{
			CChildFrame* child;
			CStringArray files;
			int	count = m_wndMapView.GetFiles(files);
			for (int ii = 0; ii < count; ii++)
			{
				if (m_children.GetSize() >= MAX_MAP)
					break;
					
				wccSetTempPath(text, (char *)files.GetAt(ii).operator LPCTSTR());
				AfxGetApp()->DoWaitCursor(1);
				child = openMap(text);
				if (child != (CChildFrame *) NULL)
					child->m_mapH.pathsetted = false;
			}
			AfxGetApp()->DoWaitCursor(-1);
		}
		else if (lParam == (int)'R' || lParam == (int)'W' || lParam == (int)'T')
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
				if (child != (CChildFrame *)NULL)
				{
					child->m_mapH.mode = (char)lParam;
					child->m_mapH.pathsetted = false;
				}
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
		if (m_wndSymbolView.m_hWnd)
			return m_wndSymbolView.GetSelectedSymbol((CPtrArray*)lParam);
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

	case ID_USR_SETCTMPROP:
	{
		CString strProp = (char*)lParam;
		SetCtmObjProp(HIWORD(wParam), strProp);
	}
		break;
		
	case ID_USR_SETCTMEVENT:
	{
		CString strEvent = (char*)lParam;
		m_wndScript.SetCtmObjEvent(HIWORD(wParam), strEvent);
	}
	break;
	case ID_USR_SETCTMFUNC:
	{
		CString strFunc = (char*)lParam;
		m_wndScript.SetCtmObjFunc(HIWORD(wParam), strFunc);
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
		return (LONG_PTR)m_wizard;

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
				m_tips.RemoveTips();
		}
		break;

	case ID_USR_ALLVIEWREDRAW:
		{
			CChildFrame* child;

			for (int ii = 0; ii < m_children.GetSize(); ii++)
			{
				child = m_children.GetAt(ii);
				if (IsWindow(child->m_hWnd))
					child->m_view.Invalidate();
			}
		}
		break;

	case ID_USR_REMOVECHILD:
		removeChild((void *)lParam);
		break;
	case ID_USR_MEDIA:
	{
		void* val;
		val = &pActiveChild->m_mapH.media;
		return (LONG_PTR)val;
	}
	case ID_USR_FORMLOAD:
		{
		CObjectLoad* ObjLoad = (CObjectLoad*)lParam;
		CformProp* pProp = ObjLoad->getProp();
		CString strPath = m_wndMapView.LoadForm(pProp->strings);
		CObjectLoad* FormLoad = NULL;
		for (size_t ii = (size_t)pActiveChild->m_view.m_ArryFormObj.GetCount(); ii > 0; ii--)  // 이미 폼이 올려져있는 상태라면 올려져있는 폼 제거
		{
			FormLoad = (CObjectLoad*)pActiveChild->m_view.m_ArryFormObj.GetAt(ii-1);
			if (ObjLoad->getWnd() == FormLoad->m_view)
			{
				pActiveChild->m_view.m_ArryFormObj.RemoveAt(ii - 1);
				delete FormLoad;
			}
		}
		
		if (!CheckFile(strPath))
		{
			pProp->strings = "";
			break;
		}
		openForm(strPath, ObjLoad);
		}
		break;
	case ID_USR_FORMRESIZE:
	{
		CObjectLoad* ObjLoad = (CObjectLoad*)lParam;
		CformProp* pProp = ObjLoad->getProp();
		CString strPath;
		strPath.Format("%s\\%s", m_curDir, pProp->strings);
		CObjectLoad* FormLoad = NULL;
		for (size_t ii = (size_t)pActiveChild->m_view.m_ArryFormObj.GetCount(); ii > 0; ii--)  // 이미 폼이 올려져있는 상태라면 올려져있는 폼 제거
		{
			FormLoad = (CObjectLoad*)pActiveChild->m_view.m_ArryFormObj.GetAt(ii - 1);
			if (ObjLoad->getWnd() == FormLoad->m_view)
			{
				pActiveChild->m_view.m_ArryFormObj.RemoveAt(ii - 1);
				delete FormLoad;
			}
		}

		if (!CheckFile(strPath))
		{
			pProp->strings = "";
			break;
		}
		openForm(strPath, ObjLoad);
	}
	case ID_USR_OBJSCROLL:
		{
		CObjectLoad* ObjLoad = (CObjectLoad*)lParam;
		CformProp* pProp = ObjLoad->getProp();
		pActiveChild->m_view.ShowVScroll(pProp);
		}
		break;
	case ID_USR_SWAPTABPAGE:
		{
		pActiveChild->m_view.swapPage(HIWORD(wParam), LOWORD(lParam), HIWORD(lParam));
		}
		break;
	case ID_USR_SWAPCELL:
		pActiveChild->m_view.swapCell(HIWORD(wParam), LOWORD(lParam), HIWORD(lParam));
		break;
	case ID_USR_USERID:
		return (LONG_PTR)m_userID.operator LPCTSTR();
		break;
	case ID_USR_DIFFVIEWER:
		if (m_strDiffViewer.IsEmpty())
		{
			SetDiffViewer();
		}
		return (LONG_PTR)m_strDiffViewer.operator LPCTSTR();
		break;
	case ID_USR_SHOWDIFF:
	{
		CChildFrame* child = (CChildFrame *)MDIGetActive();
		ShowDiff(child->m_mapH.mapN, -1, -1);
	}
		break;
	case ID_USR_MAPPREVIEW:
		ShowPreview((int)lParam);
		break;
	case ID_USR_DELPREVIEW:
	{
		CString fileName = m_aryFile.GetAt((int)lParam);
		DeleteFile(fileName);
		ShowPreview((int)HIWORD(wParam));
	}
		break;
	case ID_USR_BUILDERLOGIN:
	{
		BuilderLogout();
		KillTimer(200);
		char rcod = (char)HIWORD(wParam);
		if (rcod == '0')
		{
			m_offline = false;
			WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("0"), m_currentPath + _T("\\workshop.ini"));
			startWorkshop();
		}
		else
		{
			CString strErr = (char*)lParam;
			MessageBox(strErr, NULL, MB_TOPMOST);
			m_loginDlg.EnableLogin(true);
		}
	}
		break;
	case ID_USR_CHECKOUT:
	{
		CString str= (char*)lParam;
		CChildFrame* child;
		for (int ii = 0; ii < m_children.GetSize(); ii++)
		{
			child = m_children.GetAt(ii);
			if (IsWindow(child->m_hWnd))
			{
				if (child->m_mapH.mapN == str)
				{
					if (!child->DeleteFiles())
						break;
					child->DestroyWindow();
				}
			}
		}
		break;
	}
	}

	if (!m_children.GetSize() || !pActiveChild)
		return 0;

	switch (LOWORD(wParam))
	{
	case ID_USR_REFRESHCTRL:	// from child to propertiesbar refresh control (lParam = bDelete)
					// LOWORD(lParam) & 2 : all combo list , else only one form(HIWORD(lParam) combo list replace
		if (!(LOWORD(lParam) & 2))
		{
			if (LOWORD(lParam) & 1)
				m_wndScript.WriteScript();
				
			m_wndProperties.Initialize(&pActiveChild->m_mapH);
			m_wndProperties.ResetControl();
			m_wndScript.ResetCombo(1);
			m_wndScript.Initialize(&pActiveChild->m_mapH);	// resetcombo -> initialize

			CObjectLoad* ObjectLoad = NULL;
			for (int ii = 0; ii < pActiveChild->m_mapH.forms.GetCount(); ii++)
			{
				ObjectLoad = ((CObjectLoad*)pActiveChild->m_mapH.forms.GetAt(ii));
				if (!ObjectLoad->getProp()->name.IsEmpty())
				{
					text = ObjectLoad->getProp()->name;
					m_wndProperties.AddControl(text, ii);
					m_wndScript.AddComboString(text, ii);
				}
			}
				
			m_wndProperties.SelectControl(pActiveChild->GetSelectedForm(), pActiveChild->IsLayoutMode());
			m_wndScript.SelectControl(pActiveChild->GetSelectedForm());
		}
		else
		{
			if (!((CObjectLoad*)pActiveChild->m_mapH.forms.GetAt(HIWORD(lParam)))->getProp()->name.IsEmpty())
			{
				text = ((CObjectLoad*)pActiveChild->m_mapH.forms.GetAt(HIWORD(lParam)))->getProp()->name;
				m_wndProperties.ReplaceControl(text, HIWORD(lParam));
				m_wndScript.ReplaceComboString(text, HIWORD(lParam));
			}
		}
		break;
	
	case ID_USR_SELCTRLTOPRO:	// from child to propertiesbar select control
		m_wndProperties.SelectControl(pActiveChild->GetSelectedForm(), pActiveChild->IsLayoutMode());
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
				m_wndProperties.SelectControl(pActiveChild->GetSelectedForm(), pActiveChild->IsLayoutMode());
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
						SetWindowLongPtr(pChild->m_hWnd, GWL_STYLE, pChild->GetStyle() & ~WS_CAPTION);
						dW -= 2;
						dH -= (GetSystemMetrics(SM_CYCAPTION) + 2);
						pChild->PostMessage(WM_SIZE, 100, MAKELPARAM(mapWidth + dW, mapHeight + dH));
					}
				}
				else
				{
					if (!(pChild->GetStyle() & WS_CAPTION))
					{
						SetWindowLongPtr(pChild->m_hWnd, GWL_STYLE, pChild->GetStyle() | WS_CAPTION);
						dW += 2;
						dH += (GetSystemMetrics(SM_CYCAPTION) + 2);
					}
				}
					
				pChild->SetWindowPos(NULL, 0, 0, mapWidth + dW, mapHeight + dH, SWP_NOZORDER|SWP_NOMOVE);
			}
		}
		pActiveChild->m_view.InvalidateRect2((int)lParam,HIWORD(wParam));
		break;

	case ID_USR_SETCAPTION:		// active child caption refresh
		if (pActiveChild->m_mapH.mapK == MK_NORM)
			pActiveChild->SetWindowText(pActiveChild->m_mapH.caption);
		else if (pActiveChild->m_mapH.mapK == MK_PROCEDURES)
			pActiveChild->SetWindowText(_T("Procedures"));
		else
		{
			text.Format(_T("Template - %s"), pActiveChild->m_mapH.mapdesc);
			pActiveChild->SetWindowText(text);
		}
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

	case ID_USR_LOADTEMPLATE:	// loadTemplate
		if (HIWORD(wParam) == 1)	// load on Map
		{
			CPoint	pt = CPoint(LOWORD(lParam), HIWORD(lParam));
			pActiveChild->ScreenToClient(&pt);
			loadTemplate(pt);
		}
		else
		{
		//	m_wndTmplView.LoadTemplate(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case ID_USR_SETFOCUSPRO:	// activate properties bar
		ShowPane(&m_wndProperties, TRUE, FALSE, FALSE);
		m_wndProperties.SetFocus();
		break;

	case ID_USR_DROP:		// drop : symbol or template
		{
			CRect	rect;
			CPoint	pt = CPoint(LOWORD(lParam), HIWORD(lParam));

			pActiveChild->SetFocus();
			pActiveChild->m_view.GetWindowRect(&rect);
			if (rect.PtInRect(pt))
				pActiveChild->OnDrop(HIWORD(wParam), pt);
		}
		break;
	case ID_USR_TRLAY_DROP:
		{
			CRect	rect;
			CPoint	pt = CPoint(LOWORD(lParam), HIWORD(lParam));

			pActiveChild->SetFocus();
			pActiveChild->m_view.GetWindowRect(&rect);
			if (rect.PtInRect(pt))
				pActiveChild->OnTRLayOutDrop(HIWORD(wParam), pt);
		}
		break;
	case ID_USR_REFRESHPRO:		// refresh properties lParam = row;
		m_wndProperties.Refresh((int)lParam);
		break;

	case ID_USR_SETUNDO:		// lParam true:pushUndo, false:popUndo
		if (lParam)
		{
			pActiveChild->PushUndo(getSource(false, &pActiveChild->m_mapH));
			pActiveChild->PopRedo(true);
		}
		else
			pActiveChild->PopUndo();
		break;
	case ID_USR_OBJSEL:
		return pActiveChild->m_view.m_selF;
		break;
	case ID_USR_SETFINDCAPTION:	// findreplace dialog caption change
		text = _T("script window - ");
		text += pActiveChild->m_mapH.caption;
		m_wndScript.SetWindowText(text);
		break;

	case ID_USR_CURRENTWORD:	// current selection word of script
		if (lParam == 3)
			return (LONG_PTR)m_wndOutput.m_edit.SendMessage(WM_USER, 0 , 0);
		else if (pActiveChild->m_mapH.mapK == MK_PROCEDURES)
			text = pActiveChild->m_view.m_pSCEdit->SelCursorWord();
		else if (m_wndScript.IsWindowVisible() && m_wndScript.m_editScript.IsWindowEnabled())
			text = m_wndScript.m_editScript.SelCursorWord();
		return (LONG_PTR)text.operator LPCTSTR();

	case ID_USR_GETCONTROLEVENT:	// control object event
		{
			return 1;
			//void*	rtValue;
			//if (m_controlEvent.Lookup((char *)lParam, rtValue))
			//	return (LONG_PTR)rtValue;
			//else
			//{
			//	loadControlEvent();
			//	if (m_controlEvent.Lookup((char *)lParam, rtValue))
			//		return (LONG_PTR)rtValue;
			//	else
			//		return 0;
			//}
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
	case ID_USR_LAYOUTMODE:
	{
		CString Caption;
		CString mode = pActiveChild->m_mapH.LayoutName;

		SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
		if (!mode.CompareNoCase("Original")) // Layout Mode
		{
			pActiveChild->SetWindowText(pActiveChild->m_mapH.caption);
			m_bLayoutMode = false;
		}
		else
		{
			Caption.Format("%s - Layout Mode(%s)", pActiveChild->m_mapH.caption, mode);
			pActiveChild->SetWindowText(Caption);
			m_bLayoutMode = true;
		}

		break;
	}
	}
	return 0;
}

//
//	Fire Event
//
void CMainFrame::OnFireEvent(long type, WPARAM wParam, LPARAM lParam)
{
	switch (type)
	{
	case FEV_AXIS:
		switch (LOWORD(wParam))
		{
		case runAXIS:
			startWorkshop();
			break;
		case newVIEW:
			{
				struct	_userWH* userWH = (struct _userWH *)lParam;
				m_wndOutput.AddText(_T("[Create Window ") + CString(userWH->maps, L_MAPN) + _T("]\n"));
			}
			break;
		case renVIEW:
			m_wndOutput.AddText(_T("[Change Map ") + CString((char *)lParam, L_MAPN) + _T("]\n"));
			break;
		case delVIEW:
			{
				CChildFrame *child = (CChildFrame *) MDIGetActive();
				if (child)
				{
					if (child->m_view.m_pEmulateWnd)
					{
						child->m_view.Emulate();
						m_wndOutput.AddText(_T("[Closed]\n"));
					}
				}
			}
			break;
		case titleVIEW:
		case mapNAME:
			{
				CChildFrame *child = (CChildFrame *) MDIGetActive();
				if (child)
				{
					if (child->m_view.m_pEmulateWnd)
						child->SetWindowText((char *)lParam);
				}
			}
			break;
		}
		break;

	case FEV_OPEN:
		if (IsWindowVisible())
			break;
		if (!lParam)
		{
			if (!m_userID.IsEmpty())	// login
			{
				if (!login())
				{
					OnFireEvent(FEV_ERROR, TRUE, (LPARAM)_T("Login Failed"));
					return;
				}
			}
			else				// offline
			{

				BOOL retvalue;
				m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4), signUSERx, NULL, 0);
				startWorkshop();
			}
		}
		else
		{		// socket open error
			CString sIP = (char *)SendMessage(WM_USER, ID_USR_GETIPS);
			if (GetPrivateProfileInt(ENVIRONMENT, _T("CONNECTMODE"), 0, m_currentPath + _T("\\workshop.ini")) != 2)	// builder 전용아니면
			{
				m_offline = true;
				WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("1"), m_currentPath + _T("\\workshop.ini"));
			}
			
			if (m_wizard)
			{
				BOOL	retvalue;
				CString	ips;
				int	nPort = getPort();

				ips = _T("0.0.0.0");
				m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4),
						loginSHOP, (LONG_PTR)ips.operator LPCTSTR(), nPort);
				
				if (!retvalue)
					AfxMessageBox(_T("loginSHOP Method call : fail"));
			}
			else
				startWorkshop();
		}
		break;

	case FEV_CLOSE:
		AfxMessageBox(_T("Communication operation with server was disconnected."));
		break;

	case FEV_GUIDE:
		if (!m_loginDlg.IsWindowVisible() && !IsWindowVisible())
			m_loginDlg.ShowWindow(SW_SHOW);
		break;

	case FEV_ERROR:
		if (wParam && !IsWindowVisible())
		{
			m_loginDlg.SetMessage((char *)lParam);
			m_loginDlg.EnableLogin(true);
			m_loginDlg.ShowWindow(SW_SHOW);
		}
		break;
	}
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
	if (!m_drawTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_RIGHT|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_drawTB.LoadToolBar(IDR_DRAWBAR))
	{
		TRACE0(_T("그리기 도구 모음을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_drawTB.SetWindowText(_T("Draw"));

	// alignment toolbar
	if (!m_alignTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
		|| !m_alignTB.LoadToolBar(IDR_ALIGNBAR))
	{
		TRACE0(_T("정렬 도구 모음을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_alignTB.SetWindowText(_T("Alignment"));

	// etc toolbar
	if (!m_etcTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC)
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
	m_wndMapView.SetMapTreeKind(viewMAP, true);

	// client view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("파일 뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}

	// trlayout view
	CString strTrView;
	bNameValid = strTrView.LoadString(IDS_TR_VIEW);
	ASSERT(bNameValid);
	if (!m_wndTrView.Create(strTrView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_TRVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("TR Layout뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}
	m_wndTrView.SetMapTreeKind(viewTRLAYOUT, true);

	// symbol view
	CString strSymbolView;
	bNameValid = strSymbolView.LoadString(IDS_SYMBOL_VIEW);
	ASSERT(bNameValid);
	if (!m_wndSymbolView.Create(strSymbolView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("심볼 뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}

	// template view
	CString	strTmplView;
	bNameValid = strTmplView.LoadString(IDS_TMPL_VIEW);
	ASSERT(bNameValid);
	m_wndTmplView.SetDirectory(m_curDir);
	if (!m_wndTmplView.Create(strTmplView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_MAPVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0(_T("서버템플릿 뷰 창을 만들지 못했습니다.\n"));
		return FALSE;
	}

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

	HICON hTrViewIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WORK1), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTrView.SetIcon(hTrViewIcon, FALSE);

	HICON hSymbolViewIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WORK2), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndSymbolView.SetIcon(hSymbolViewIcon, FALSE);

	HICON hTmplViewIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_WORK1), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndTmplView.SetIcon(hTmplViewIcon, FALSE);

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

	rc = GetPrivateProfileString(_T("POSITION"), _T("MAINFRAME"), _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
	tmps = CString(wb, rc);

	if (!tmps.IsEmpty())
		MoveWindow(parseRect(tmps));

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

		//for (kk = (int)sortArr.GetSize() - 1; kk >= 0 ; kk--)
		//{
		//	CString sPriority = sortArr.GetAt(kk);
		//	if (atoi(parse(sPriority, _T("|"))) < value)
		//		break;
		//}

		//kk++;
		sortArr.InsertAt(ii, text);
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
		return CRect(0);
	
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
	case 1:	//	drawBar
		pPane = (CPane*)&m_drawTB;	break;
	case 2:	//	alignBar
		pPane = (CPane*)&m_alignTB;	break;
	case 3:	//	exBar
		pPane = (CPane*)&m_etcTB;	break;
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
		//DockPane(pPane);
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
	CPane*	pPane;

	switch (idx)
	{
	case 0:	//	mainBar
		pPane = (CPane*)&m_mainTB;		break;
	case 1:	//	drawBar
		pPane = (CPane*)&m_drawTB;		break;
	case 2:	//	alignBar
		pPane = (CPane*)&m_alignTB;		break;
	case 3:	//	exBar
		pPane = (CPane*)&m_etcTB;		break;
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
	for (UINT ii = 0; ii <= ID_TOOL_OBJECT - ID_TOOL_BASE; ii++)
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

	makeDir(m_root); // directory setting

	m_openDir = m_root + _T("\\") + RUNDIR;
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
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!::CreateDirectory(backFileN, NULL))
			MessageBox(_T("Can't make the backup directory."), _T("Error"), MB_OK | MB_ICONSTOP);
	}

	int	nTotal = 0, nCnt = 0;
	BOOL	working = ff.FindFile(fileN);
	while (working)
	{
		working = ff.FindNextFile();
		if (!ff.IsDirectory() && ff.GetFileName().Mid(ff.GetFileName().ReverseFind('.')) == _T(".xml"))
			nTotal++;
	}
	ff.Close();

	maps.RemoveAll();
	working = ff.FindFile(fileN);
	while (working)
	{
		working = ff.FindNextFile();
		tmps = ff.GetFileName();
		if (!ff.IsDirectory() && tmps.Mid(tmps.ReverseFind('.')) == _T(".xml"))
		{
			backFileN.Format(_T("%s\\backup\\%s"), m_curDir, tmps);
			SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(nCnt++ * 100 / nTotal, 4));
			CopyFile(fileN, backFileN, FALSE);
			maps.SetAt(tmps, backFileN);
		}
	}
	ff.Close();
	return 1;
}

void CMainFrame::createNewChild(int mapK)
{
	CRect	rect, rc;
	CString	tmps;
	CChildFrame*	child;
	int	fontpt;
	CFormDefault	dlg;
	CString	formname = _T("");
	CString	desc = _T("");
	CString	fontname = _T("");
	CString	fontsize = _T("");
	BYTE	media = MEDIA_HTS;

	tmps = _T("Untitled");
	if (mapK == MK_NORM)
	{
		if (dlg.DoModal() == IDOK)
		{
			formname = dlg.m_formname;
			desc = dlg.m_desc;
			tmps = dlg.m_Title;
			rect.SetRect(0, 0, atoi(dlg.m_width), atoi(dlg.m_height));
			fontname = dlg.m_fontname;
			fontsize = dlg.m_fontsize;
			media = dlg.m_media;
		}
		else
			return;
	}
	else if (mapK == MK_TEMPLATE)
	{
		tmps = _T("Template");
		rect.SetRect(0, 0, 500, 350);
	}
	else if (mapK == MK_PROCEDURES)
	{
		tmps = _T("Procedures");
		rect.SetRect(0, 0, 500, 350);
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

	child = new CChildFrame(this, m_curDir);
	if (!child->Create(tmps, rect, m_draw, m_palette, m_piForm, m_displayMode))
		return;

	child->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);

	if (mapK == MK_NORM)
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_MAP), FALSE);
	else if (mapK == MK_TEMPLATE)
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_TEMPLATE), FALSE);
	else 
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_PROCEDURES), FALSE);

	if (!formname.IsEmpty()) strcpy_s(child->m_mapH.mapN, sizeof(child->m_mapH.mapN), formname);
	if (!desc.IsEmpty()) strcpy_s(child->m_mapH.mapdesc, sizeof(child->m_mapH.mapdesc), desc);
	if (!fontname.IsEmpty()) strcpy_s(child->m_mapH.fName, sizeof(child->m_mapH.fName), fontname);
	if (!fontsize.IsEmpty()) child->m_mapH.fPoint = atoi(fontsize);
	if (media != MEDIA_HTS) child->m_mapH.media = media;

	child->SetNew(true);
	child->m_mapH.width  = rect.Width();
	child->m_mapH.height = rect.Height();
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
	if (child->m_mapH.fPoint == 0)
		child->SetFontSize(atoi(m_sDefFontPnt), fsNORMAL, m_sDefFontName);
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
	if (dropInChild())
	{
		wccSetTempPath(tmps, _T("template"));
		CopyFile(path, tmps, FALSE);
		child = (CChildFrame*)MDIGetActive();
		child->m_view.ScreenToClient(&m_dropPnt);
		loadTemplate(m_dropPnt);
		m_dropPnt = CPoint(-1, -1);
		child->m_mapH.modified = true;
	}
	else
	{
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
	}
	m_dropPnt = CPoint(-1, -1);

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CMainFrame::openLocalFile()
{
	CFileDialog fDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"));
	fDlg.m_ofn.lpstrInitialDir = m_openDir;
	if (fDlg.DoModal() != IDOK)
		return;

	openFile(fDlg.GetPathName());
}

bool CMainFrame::SearchElement(int startIdx, CString Element, CString& strData)
{
	ElementNode* pSubNode;
	bool bRet = false;
	for (int jj = startIdx; jj < m_ElementNode.GetCount(); jj++)
	{
		pSubNode = m_ElementNode.GetAt(jj);
		if (pSubNode->strValue.IsEmpty())
		{
			break;
		}
		if (pSubNode->strElement == Element)
		{
			bRet = true;
			strData = pSubNode->strValue;
		}
	}
	return bRet;
}

void CMainFrame::MakeCellProp(CformProp* Prop, CString Element, int cnt)
{
	CString tmps;
	if (Element == GRID || Element == GRIDEX || Element == TABLE)
	{
		//if (SearchElement(startIndex, _T("MATRIX"), tmps))
		//{
		//	tmps = tmps.Mid(tmps.Find(",") + 1);
			SetCellProp(Prop, cnt);
		//}
	}
}

CChildFrame* CMainFrame::openMap(CString fileN)
{
	int	totL;
	int	formN, repN, grN;
	CString tmps;

	totL = wccLoadSource(fileN, (CWnd *) NULL);
	if (totL == -1)
		return (CChildFrame *) NULL;

	formN = grN = 0; repN = -1;
	CChildFrame* child = new CChildFrame(this, m_curDir);
	if (!child->Create(_T(""), CRect(0, 0, 0, 0), m_draw, m_palette, m_piForm, m_displayMode))
		return NULL;

	child->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	child->SetVisible(m_showFORM);
	child->SetFontSize(atoi(m_sDefFontPnt), fsNORMAL, m_sDefFontName);

	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_pToken[ii].Clear();
	load(&child->m_mapH, m_curDir);
	//////////////////////////////////////////////////////////////////////////
	wccGetXmlNode(m_ElementNode, GLOBAL_DIVISION);
	buildXmlHeader(&m_ElementNode);
	int iCellIdx = 0;
	int iFormIdx = 0;
	int cnt = 0;
	ElementNode* pNode;
	while (wccGetFormNode(m_ElementNode, &iFormIdx))
	{
		for (int ii = 0; ii < m_ElementNode.GetCount(); ii++)
		{
			pNode = m_ElementNode.GetAt(ii);
			if (pNode->strValue.IsEmpty())
			{
				if(SearchElement(ii + 1, _T("TYPE"), tmps))
					tmps = tmps.Left(tmps.Find("|"));

				CformProp* Prop = new CformProp();
				Prop->Init(getObjectKind(pNode->strElement, tmps));
				cnt = wccGetCellNode(m_CellPropNode, &iCellIdx);
				MakeCellProp(Prop, pNode->strElement, cnt);
				CObjectLoad* ObjLoad = new CObjectLoad(Prop, m_piForm, &child->m_view);
				child->m_mapH.forms.Add(ObjLoad);

				buildXmlForm(&m_ElementNode, &m_CellPropNode);

				if (child->m_view.isValidTabOrder(Prop))
					Prop->index = ++child->m_view.m_nSeqTabOdr;
				if (child->m_view.isValidIOOrder(Prop))
					Prop->iorder = ++child->m_view.m_nSeqIoOdr;

				ObjLoad->LoadLib(Prop->kind);
				ObjLoad->LoadProc();
			}
		}
	}
	buildTab();

	child->ChangeKind();

	if (checkInEditing(child->m_mapH.mapN, child->m_mapH.mapK))
	{
		child->MDIDestroy();
		if (::IsWindow(m_wndMDITabs.m_hWnd))
			m_wndMDITabs.Update();
		return (CChildFrame *) NULL;
	}
	m_children.Add(child);

	child->m_mapH.onStart   = false;
	child->m_mapH.onSend    = false;
	child->m_mapH.onReceive = false;
	child->m_mapH.onAlert   = false;
	child->m_mapH.onService = false;
	child->m_mapH.onFile    = false;
	child->m_mapH.onSelect	= false;
	child->m_mapH.onTimer   = false;
	child->m_mapH.onFocus   = false;
	child->m_mapH.onClose   = false;
	child->m_mapH.onDevice  = false;
	child->m_mapH.onKey     = false;
	child->m_mapH.onChangeLayout = false;
	child->m_mapH.onFlicking = false;
	child->m_mapH.onTrigger = false;
	child->m_mapH.onDeclaration = false;
	child->m_mapH.onInDeclaration = false;

	wccGetXmlNode(m_ElementNode, RESOURCE_DIVISION);
	buildXmlResource(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, TRLAYOUT_DIVISION);
	BuildXmlTRlayout(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, LAYOUTEDIT_DIVISION);
	BuildXmlLayoutEdit(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, RTS_DIVISION);
	BuildXmlRTS(&m_ElementNode);

	child->m_mapH.formN	= child->m_mapH.forms.GetCount();
	child->m_mapH.source	= true;
	child->m_mapH.modified	= false;
	child->SetNew(true);

	if (child->m_mapH.width < 100)
	{
		child->ModifyStyle(WS_CAPTION, NULL, SWP_DRAWFRAME);
	}

	CSize	delta;
	CRect	cRc,  wRc, rc;

	child->GetClientRect(&cRc);
	child->GetWindowRect(&wRc);
	
	delta.cx = wRc.Width()  - cRc.Width() + (child->m_mapH.width >= 100 ? GetSystemMetrics(SM_CXBORDER)*4 : 0);
	delta.cy = wRc.Height() - cRc.Height() + (child->m_mapH.width >= 100 ? GetSystemMetrics(SM_CYBORDER)*4 : 0);

	if (child->m_mapH.mapK == MK_TEMPLATE)
	{
		CString	tmps = _T("Template - ") + CString(child->m_mapH.mapdesc);
		child->SetWindowText(tmps);
	}
 	else
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
	else if (child->m_mapH.mapK == MK_TEMPLATE)
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_TEMPLATE), FALSE);
	else 
		child->SetIcon(AfxGetApp()->LoadIcon(IDI_PROCEDURES), FALSE);

	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);	// Properties & script reload
	SendMessage(WM_USER, ID_USR_SETMAPNAME);
	
	CObjectLoad* ObjLoad = NULL;
	CformProp* Prop = NULL;
	for (size_t ii = 0; ii < (size_t)child->m_mapH.forms.GetCount(); ii++)
	{
		ObjLoad = (CObjectLoad*)child->m_mapH.forms.GetAt(ii);
		Prop = ObjLoad->getProp();
		if (Prop->kind == fmFORM)
		{
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_FORMLOAD, (LPARAM)ObjLoad);
		}
	}

	child->m_view.m_FmCtrl->InvalidateObject();
	return (CChildFrame *) child;
}

void CMainFrame::openForm(CString fileN, CObjectLoad* ObjLoad)
{
	//CfmObject* Obj = ObjLoad->getObject();
	CformProp* Prop = ObjLoad->getProp();
	CWnd* pWnd = ObjLoad->getWnd();

	int	totL;
	int	formN, repN, grN;
	CString tmps;

	totL = wccLoadSource(fileN, (CWnd *)NULL);
	if (totL == -1)
		return ;

	formN = grN = 0; repN = -1;

	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_pToken[ii].Clear();

	class mapForm	m_mapH;
	load(&m_mapH, m_curDir);
	wccGetXmlNode(m_ElementNode, GLOBAL_DIVISION);
	buildXmlHeader(&m_ElementNode);
	int iCellIdx = 0;
	int iFormIdx = 0;
	int cnt = 0;
	ElementNode* pNode;
	while (wccGetFormNode(m_ElementNode, &iFormIdx))
	{
		for (int ii = 0; ii < m_ElementNode.GetCount(); ii++)
		{
			pNode = m_ElementNode.GetAt(ii);
			if (pNode->strValue.IsEmpty())
			{
				if (SearchElement(ii + 1, _T("TYPE"), tmps))
					tmps = tmps.Left(tmps.Find("|"));

				CformProp* Prop = new CformProp();
				Prop->Init(getObjectKind(pNode->strElement, tmps));
				cnt = wccGetCellNode(m_CellPropNode, &iCellIdx);
				MakeCellProp(Prop, pNode->strElement, cnt);
				CObjectLoad* formObjLoad = new CObjectLoad(Prop, m_piForm, (CChildView*)pWnd);
				m_mapH.forms.Add(formObjLoad);

				buildXmlForm(&m_ElementNode, &m_CellPropNode);

				if (!Prop->bFixedSize) FormResize(Prop, pWnd, &m_mapH); //폼 오브젝트의 크기에 맞춰 리싸이즈 기능
				formObjLoad->LoadLib(Prop->kind);
				formObjLoad->LoadProc();

				CChildFrame* child = (CChildFrame *)MDIGetActive();
				child->m_view.m_ArryFormObj.Add(formObjLoad);
			}
		}
	}

	if (Prop->bFixedSize)
	{
		CRect rc;
		rc.CopyRect(Prop->rect);
		rc.right = rc.left + m_mapH.width;
		rc.bottom = rc.top + m_mapH.height;
		CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();
		pActiveChild->m_view.m_FmCtrl->SetObjRect(ObjLoad, rc);
	}
}

void CMainFrame::loadTemplate(CPoint pt)
{
	CString tmp, symbolName = _T("\t");
	CMapStringToString	changedSymbolMap;

	wccSetTempPath(tmp, _T("template"));
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	m_dropPnt = pt;

	child->m_view.ClientToScreen(&m_dropPnt);
	if (!dropInChild())
		return;

	int	totL = wccLoadSource(tmp, (CWnd *) NULL), formN, prFormN, groupCount = 0;
	CformProp* nGroup = NULL;
	//int	tabN, pageN = 0;

	//if (!child->m_view.IsInTab(pt, tabN, pageN))
	//	tabN = -1;
	
	load(&child->m_mapH, tmp);
	prFormN = formN = child->m_mapH.forms.GetCount();
	for (int kk = 0; kk < formN; kk++)
	{
		symbolName += ((CObjectLoad *)child->m_mapH.forms.GetAt(kk))->getProp()->name;
		symbolName += _T('\t');
	}
	//////////////////////////////////////////////////////////////////////////////
	wccGetXmlNode(m_ElementNode, GLOBAL_DIVISION);
	buildXmlHeader(&m_ElementNode);
	int iCellIdx = 0;
	int iFormIdx = 0;
	int cnt = 0;
	ElementNode* pNode;
	CString tmps;
	while (wccGetFormNode(m_ElementNode, &iFormIdx))
	{
		for (int ii = 0; ii < m_ElementNode.GetCount(); ii++)
		{
			pNode = m_ElementNode.GetAt(ii);
			if (pNode->strValue.IsEmpty())
			{
				if (SearchElement(ii + 1, _T("TYPE"), tmps))
					tmps = tmps.Left(tmps.Find("|"));

				CformProp* Prop = new CformProp();
				Prop->Init(getObjectKind(pNode->strElement, tmps));
				cnt = wccGetCellNode(m_CellPropNode, &iCellIdx);
				MakeCellProp(Prop, pNode->strElement, cnt);
				CObjectLoad* ObjLoad = new CObjectLoad(Prop, m_piForm, &child->m_view);
				child->m_mapH.forms.Add(ObjLoad);

				buildXmlForm(&m_ElementNode, &m_CellPropNode);

				if (child->m_view.isValidTabOrder(Prop))
					Prop->index = ++child->m_view.m_nSeqTabOdr;
				if (child->m_view.isValidIOOrder(Prop))
					Prop->iorder = ++child->m_view.m_nSeqIoOdr;

				ObjLoad->LoadLib(Prop->kind);
				ObjLoad->LoadProc();
			}
		}
	}
	buildTab(prFormN);

	//for (ii = child->m_formItem->GetIdxOfPtr(nGroup); ii < formN && ii >= 0; ii++)
	//	if (!child->m_formItem->GetForm(ii)->tabN)
	//		groupCount++;
	//for (ii = child->m_formItem->GetIdxOfPtr(nGroup); ii < formN && ii >= 0; ii++)
	//{
	//	struct _formR *formR = child->m_formItem->GetForm(ii);
	//	formR->left   += pt.x;
	//	formR->right  += pt.x;
	//	formR->top    += pt.y;
	//	formR->bottom += pt.y;
	//	
	//	if (!formR->tabN && tabN >= 0)
	//	{
	//		formR->tabN = child->m_formItem->GetForm(tabN);
	//		formR->pageN = pageN;
	//	}
	//	
	//	if (tabN == -1)
	//	{
	//		if (groupCount > 1 && !formR->tabN)
	//			formR->group = nGroup;
	//	}
	//}
	//child->m_view.m_selF = child->m_formItem->GetIdxOfPtr(nGroup);

	wccGetXmlNode(m_ElementNode, RESOURCE_DIVISION);
	buildXmlResource(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, TRLAYOUT_DIVISION);
	BuildXmlTRlayout(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, LAYOUTEDIT_DIVISION);
	BuildXmlLayoutEdit(&m_ElementNode);

	changedSymbolMap.RemoveAll();
	child->m_view.m_bRedraw = true;
	child->m_mapH.modified = true;

	PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	child->m_view.Invalidate();
}

void CMainFrame::saveLocalFile()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (!child->m_mapH.pathsetted)
		saveLocalPath();
	else
		saveMap(child);
}

void CMainFrame::SetMasterlayout()
{
	CString	dir = _T(""), name;
	int iIndex = 0;

	CFileStatus fs;
	do
	{
		dir.Format("%s\\mtlayout\\MSTRLAT%d.xml", m_root, iIndex++);
	} while (CFile::GetStatus(dir, fs));

	name.Format("MSTRLAT%d", iIndex - 1);
	CString tdir = _T(""), mapName;
	m_bMasterLayout = true;

	CChildFrame* child = (CChildFrame *)MDIGetActive();
	mapName.Format("%s", child->m_mapH.mapN);
	strcpy_s(child->m_mapH.mapN, sizeof(child->m_mapH.mapN), name);
	generateMap(false, child);
	if (!child->m_mapH.source)
		return;
	
	CString tmp;
	tmp.Format("%s.xml", child->m_mapH.mapN);
	wccSetTempPath(tdir, (char*)tmp.operator LPCSTR());
	CopyFile(tdir, dir, false);
	strcpy_s(child->m_mapH.mapN, sizeof(child->m_mapH.mapN), mapName);

	CString tmps;
	tmps.Format(_T("%d"), m_bMasterLayout);
	WritePrivateProfileString(_T("MASTERLAYOUT"), _T("NAME"), tmps, m_currentPath + _T("\\workshop.ini"));
}

void CMainFrame::ResetMasterlayout()
{
	m_bMasterLayout = false;

	CString tmps;
	tmps.Format(_T("%d"), m_bMasterLayout);
	WritePrivateProfileString(_T("MASTERLAYOUT"), _T("NAME"), tmps, m_currentPath + _T("\\workshop.ini"));
}

void CMainFrame::saveLocalPath()
{
	DWORD	idir;
	CString	path, old;

	CChildFrame* child = (CChildFrame *)MDIGetActive();
	path = child->m_mapH.fullN;
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
	CString	dir = _T(""), tdir;

	generateMap(false, child);
	if (!child->m_mapH.source)
		return;

	dir  = child->m_mapH.fullN;

	dir += _T("\\");
	dir += child->m_mapH.mapN;
	dir += _T(".xml");
	tdir = _T("");
	CString tmp;
	tmp.Format("%s.xml", child->m_mapH.mapN);
	wccSetTempPath(tdir, (char*)tmp.operator LPCSTR());
	CopyFile(tdir, dir, false);
}

void CMainFrame::saveToServer()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.source)
	{
		CUpDlg* upDlg = new CUpDlg(this, &child->m_mapH, m_userID);
		upDlg->m_sRoot = m_root;
		upDlg->DoModal();
		delete upDlg;
	}
	else
		MessageBox("Must create Source before Upload.", "Message", MB_OK);
}

void CMainFrame::saveTmplToServer()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	if (generateMap(false) == 0)
		child->m_mapH.source = true;

	if (child && child->m_mapH.source)
	{
		CUpDlg* upDlg = new CUpDlg(this, &child->m_mapH, m_userID, false);
		upDlg->m_sRoot = m_root;
		upDlg->DoModal();
		delete upDlg;
	}
	else
		MessageBox(_T("Must create Source before Upload."), _T("Message"), MB_OK);
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
				m_wndProperties.SelectControl(-1, false);
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
	
	//child->m_view.SetOriginalLayout();
	if (child->IsLayoutMode())
	{
		AfxMessageBox("Compile Fail : Layout Mode.");
		return -1;
	}
	if (child->m_mapH.mapK == MK_PROCEDURES)
		child->SaveProcedures();
	else
	{
	}

	m_wndScript.WriteScript();
	return generateSource(alarm, &child->m_mapH);
}

void CMainFrame::compileMap(bool all)
{
	int	totL;
	CString	srcP, objP, nickP;

	CChildFrame* child;
	AfxGetApp()->DoWaitCursor(1);

	for (int ii = 0; ii < (all ? m_children.GetSize() : 1); ii++)
	{
		if (all)
			child = m_children.GetAt(ii);
		else
			child = (CChildFrame *) MDIGetActive();
		
		if (child->m_mapH.mapK == MK_TEMPLATE)
			continue;

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

		totL = wccLoadSource(srcP, this);
		if (totL == -1)
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return;
		}
		wccmakeBinaryFile(srcP, &child->m_mapH);
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

		if (child->m_mapH.mapK == MK_TEMPLATE)
			break;

		DWORD	idir = GetFileAttributes(m_root);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || !child->m_mapH.compile)
			return;
		
		path.Format(_T("%s\\%s"), m_root, MAPDIR);
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		tmps = CString(child->m_mapH.mapN, L_SGID);
		path += _T("\\");
		path += tmps;
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		tmps = CString(child->m_mapH.mapN, L_TGID);
		path += _T("\\");
		path += tmps;
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);

		tmps = _T("");
		wccSetTempPath(tmps, child->m_mapH.mapN, false);
		path += _T("\\");
		path += child->m_mapH.mapN;
		CopyFile(tmps, path, false);
	}
}

void CMainFrame::reloadMap()
{
	CString	path, tmps;
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	wccSetTempPath(tmps, child->m_mapH.mapN);
	if (child->m_mapH.pathsetted)
		path = child->m_mapH.fullN;

	CFile	file;
	if (!file.Open(tmps, CFile::modeRead|CFile::typeBinary))
		return;
	file.Close();

	if (child->m_mapH.onStart)
		DeleteFile(child->m_mapH.onStartN);
	if (child->m_mapH.onSend)
		DeleteFile(child->m_mapH.onSendN);
	if (child->m_mapH.onReceive)
		DeleteFile(child->m_mapH.onReceiveN);
	if (child->m_mapH.onAlert)
		DeleteFile(child->m_mapH.onAlertN);
	if (child->m_mapH.onService)
		DeleteFile(child->m_mapH.onServiceN);
	if (child->m_mapH.onFile)
		DeleteFile(child->m_mapH.onFileN);
	if (child->m_mapH.onSelect)
		DeleteFile(child->m_mapH.onSelectN);
	if (child->m_mapH.onTimer)
		DeleteFile(child->m_mapH.onTimerN);
	if (child->m_mapH.onFocus)
		DeleteFile(child->m_mapH.onFocusN);
	if (child->m_mapH.onClose)
		DeleteFile(child->m_mapH.onCloseN);
	if (child->m_mapH.onDevice)
		DeleteFile(child->m_mapH.onDeviceN);
	if (child->m_mapH.onKey)
		DeleteFile(child->m_mapH.onKeyN);
	if (child->m_mapH.onChangeLayout)
		DeleteFile(child->m_mapH.onChangeLayoutN);
	if (child->m_mapH.onFlickingN)
		DeleteFile(child->m_mapH.onFlickingN);
	if (child->m_mapH.onTrigger)
		DeleteFile(child->m_mapH.onTriggerN);
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
		rc = GetPrivateProfileString(_T("CONTROL"), tmps, _T(""), twb, sizeof(twb), path);
		if (rc <= 0)
			break;

		desc = CString(twb, rc);
		tmps = parse(desc, _T(";"));
		sym  = parse(desc, _T(";"));
		parse(desc, _T(";"));
		
		if (tmps.IsEmpty())
			break;
		m_controlEvent.SetAt(tmps, (void*)atoi(desc));
	}
}

bool CMainFrame::registerCtrl(CString name, bool showMsg)
{
	HINSTANCE handle;
	CString	path;

	path.Format(_T("%s\\exe\\%s"), m_root, name);
	handle = LoadLibrary(path);
	if (handle)
	{
		FARPROC	lpDllEntryPoint;
		(FARPROC&)lpDllEntryPoint = GetProcAddress(handle, _T("DllRegisterServer"));
		if (lpDllEntryPoint != NULL)
			(*lpDllEntryPoint)();
		
		FreeLibrary(handle);
		return true;
	}
	else
	{
		//if (showMsg)
		//	AfxMessageBox(_T("There is not ") + path + _T(" file."));
	}
	return false;
}

void CMainFrame::startWorkshop()
{
	CString	tmps;
	CMapStringToString maps;
	long	screenCX = GetSystemMetrics(SM_CXSCREEN), nScreenCY = GetSystemMetrics(SM_CYSCREEN);

	m_loginDlg.ShowWindow(SW_HIDE);
	if (!m_offline)
		CSplashWnd::ShowSplashScreen(this);

	if (initDir(maps) == -1)
	{
		PostMessage(WM_CLOSE);
		return;
	}

	tmps.Format(_T("%s\\%s\\%s"), m_root, TABDIR, PALETTE);
	m_palette = new CAxisPalette((char *)tmps.operator LPCTSTR());
	m_draw    = new CAxisDraw();

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
	m_wndTmplView.SetTwoDepth(GetPrivateProfileInt(ENVIRONMENT, _T("TMPDEPTH"), 3, m_currentPath + _T("\\workshop.ini")) ? true : false);
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
		if (!m_pGroup->Create(NULL ,NULL, WS_CHILD, CRect(0), this, 1))
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

	m_wndMapView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTrView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSymbolView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndTmplView.EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndMapView);
	m_tabBar = NULL;
	m_wndFileView.AttachToTabWnd(&m_wndMapView, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);
	m_wndTrView.AttachToTabWnd(m_tabBar, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);
	m_wndSymbolView.AttachToTabWnd(&m_wndMapView, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);
	m_wndTmplView.AttachToTabWnd(&m_wndMapView, DM_SHOW, TRUE, (CDockablePane**)&m_tabBar);

	CMFCBaseTabCtrl* pTabCtrl = m_tabBar->GetUnderlyingWindow();
	if (pTabCtrl)
	{
		if (m_offline)
			pTabCtrl->SetActiveTab(1);	// file view
		else
			pTabCtrl->SetActiveTab(0);	// map view
	}

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
//	m_wndScript.FloatPane(CRect(20, 100, 500, 400));
	m_wndScript.FloatPane(rc);

	ShowWindow(SW_SHOW);

	//((CaxisBuilderApp *)AfxGetApp())->LoadState(this);
	setToolbarPosition();

	char	wb[512];
	DWORD	dw;
	dw = GetPrivateProfileString(_T("MASTERLAYOUT"), _T("NAME"), _T(""), wb, sizeof(wb), m_currentPath + _T("\\workshop.ini"));
	tmps = CString(wb, dw);
	m_bMasterLayout = atoi(tmps) == 1 ? true : false;

	m_piForm = new CiForm(m_root);
	m_piForm->m_editMode = true;

	if (maps.GetCount() > 0)
	{
		if (AfxMessageBox(_T("It is found the backup file.\n\nDo you load file?"), MB_YESNO) == IDYES)
		{
			CString sKey, sPath;
		//	CFileFind finder;
			for (POSITION pos = maps.GetStartPosition(); pos != NULL; )
			{
				maps.GetNextAssoc(pos, sKey, sPath);
		//		if (finder.FindFile(sPath))
					openFile(sPath, true);
			}
		}
	}
}


BOOL CMainFrame::login()
{
	BuilderLogin();
	//BOOL	retvalue = false;
	//if (GetPrivateProfileInt(ENVIRONMENT, _T("SIGNONTYPE"), 0, m_currentPath + _T("\\workshop.ini")))
	//{
	//	struct	_signM {
	//		char	user[12];
	//		char	pass[8];
	//		char	pcip[20];
	//		char	dats[10];

	//	}signon;
	//	CString sBuf;
	//	LONG_PTR ret = 0;
	//	
	//	FillMemory(&signon, sizeof(signon), _T(' '));
	//	CopyMemory(signon.user, m_userID, min(m_userID.GetLength(), sizeof(signon.user)));
	//	CopyMemory(signon.pcip, getClientIP(), min(getClientIP().GetLength(), sizeof(signon.pcip)));
	//	
	//	sBuf.Format(_T("%s"), m_pass);
	//	m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I8, (void *)&ret,
	//			(BYTE *)(VTS_I4 VTS_I8), MAKEWPARAM(encPASS, 0), (LONG_PTR)sBuf.operator LPCTSTR());
	//	CopyMemory(signon.pass, (char *)ret, strlen((char *)ret));
	//	signon.dats[3] = '1';

	//	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4), signUSER, (LONG_PTR)&signon, sizeof(signon));
	//}
	//else
	//{
	//	BuilderLogin();
	//}

	return true;
}

void CMainFrame::loginWizard()
{
	char	cPath[1024];
	CWinApp* app = AfxGetApp();
	CString sTmp = (char *)SendMessage(WM_USER, ID_USR_GETIPS), tmps;

	m_offline = GetPrivateProfileInt(ENVIRONMENT, _T("OFFLINE"), 1, m_currentPath + _T("\\workshop.ini")) ? true : false;
	GetPrivateProfileString(ENVIRONMENT, _T("FONTNAME"), _T("MS Sans Serif"), cPath, sizeof(cPath), m_currentPath + _T("\\workshop.ini"));
	m_sDefFontName = cPath;
	GetPrivateProfileString(ENVIRONMENT, _T("FONTPNT"), _T("9"), cPath, sizeof(cPath), m_currentPath + _T("\\workshop.ini"));
	m_sDefFontPnt = cPath;
	GetPrivateProfileString(ENVIRONMENT, _T("INITMAP"), _T(""), cPath, sizeof(cPath), m_currentPath + _T("\\workshop.ini"));
	m_initMap = cPath;
	GetPrivateProfileString(ENVIRONMENT, _T("PROJECTNAME"), _T("AXIS"), cPath, sizeof(cPath), m_currentPath + _T("\\workshop.ini"));
	m_projectName = cPath;
	GetPrivateProfileString(ENVIRONMENT, _T("DIFFVIEWER"), _T(""), cPath, sizeof(cPath), m_currentPath + _T("\\workshop.ini"));
	m_strDiffViewer = cPath;

	((CaxisBuilderApp*)AfxGetApp())->SetKey(m_projectName);
	m_root = (char *)SendMessage(WM_USER, ID_USR_GETROOTDIR);
	if (m_root.IsEmpty())
		m_root = m_currentPath;
	SendMessage(WM_USER, ID_USR_SETROOTDIR, (LPARAM)m_root.operator LPCTSTR());

	if (sTmp.IsEmpty())
	{
		setServerIP();
		m_offline = true;
		WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("1"), m_currentPath + _T("\\workshop.ini"));
	}

	if (GetPrivateProfileInt(ENVIRONMENT, _T("CONNECTMODE"), 0, m_currentPath + _T("\\workshop.ini")) == 1)
		m_offline = true;	// wizard connect mode일때 무조건 offline으로.
	return;
	registerCtrl(_T("axSock.ocx"), true);

	// Wizard.ocx 
	if (m_wizard)
	{
		m_loginDlg.ShowWindow(SW_HIDE);
		if (!m_userID.IsEmpty())
		{
			BOOL	retvalue;
			int	nPort = getPort();

			if (m_offline)
			{
				if (GetPrivateProfileInt(ENVIRONMENT, _T("CONNECTMODE"), 0, m_currentPath + _T("\\workshop.ini")) == 1)
					tmps = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
				else
				{
					tmps = _T("0.0.0.0");
					m_loginDlg.ShowWindow(SW_HIDE);
				}
			}
			else
				tmps = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
			
			m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4), loginSHOP, (LONG_PTR)tmps.operator LPCTSTR(), nPort);
			
			if (!retvalue)
				AfxMessageBox(_T("loginSHOP Method call : fail"));
			login();
		}
		else
			startWorkshop();
		return;
	}

	registerCtrl(_T("axWizard.ocx"), true);
	registerCtrl(_T("axCertify.ocx"), false);
	registerCtrl(_T("AxComCtl.ocx"), false);

	if (!m_wizard)
	{
		GetWindowText(tmps);
		SetWindowText(m_projectName);
		m_wizard = new CWnd();
		if (!m_wizard->CreateControl(_T("AxisWizard.WizardCtrl.KTB64"), NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))
		{
			//AfxMessageBox(_T("Create AxWizard.ocx : failed"));
			delete m_wizard;
			m_wizard = NULL;
		}
		SetWindowText(tmps);
	}
	
	if (!m_wizard)
	{
		//AfxMessageBox(_T("Can't find a AxWizard.ocx"));
		m_loginDlg.ShowWindow(SW_HIDE);
		m_offline = true;
		WritePrivateProfileString(ENVIRONMENT, _T("OFFLINE"), _T("1"), m_currentPath + _T("\\workshop.ini"));
		startWorkshop();
		return;
	}
	
	BOOL	retvalue;
	int	nPort;

	// 오프라인일때도 wizard를 접속모드로 가게 한다.
	if (m_offline)
	{
		if (GetPrivateProfileInt(ENVIRONMENT, _T("CONNECTMODE"), 0, m_currentPath + _T("\\workshop.ini")) == 1)
			tmps = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
		else
		{
			tmps = _T("0.0.0.0");
			m_loginDlg.ShowWindow(SW_HIDE);
		}	
	}
	else
		tmps = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
	
	nPort = getPort();
	m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I8 VTS_I4), loginSHOP, (LONG_PTR)tmps.operator LPCTSTR(), nPort);
	
	if (!retvalue)
		AfxMessageBox(_T("loginSHOP Method call : fail"));
}

void CMainFrame::SetCellProp(CformProp* Prop, int iCnt)
{
	Prop->cellProperties = new CObArray();
	for (int ii= 0; ii < iCnt; ii++)
	{
		cellOb* cellProp = new cellOb();
		cellProp->paintColor = basINPUT;
		cellProp->textColor = 232;
		cellProp->headPaintColor = basPRGB;
		cellProp->headTextColor = 232;
		cellProp->width = 80;

		cellProp->size = 10;
		cellProp->charType = ctNUMERIC;
		cellProp->ioType = ioOUTPUT;
		cellProp->kind = ckNORMAL;
		//cellProp->name = getInitSymbol(fN, _T("Col"));
		cellProp->name = _T("");
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		Prop->cellProperties->Add(cellProp);
	}
}

void CMainFrame::changeMap(CString src)
{
	if (src.IsEmpty())
		return;

	int	totL, formN = 0;

	CChildFrame *child = (CChildFrame *) MDIGetActive();
	if (!child)
		return;

	totL = wccLoadSource(_T(""), (CWnd *) NULL, src);
	if (totL == -1)
		return ;

	child->m_view.ObejctRemoveAll();
	child->DeleteUndoMap();
	//for (int ii = 0; ii < MAX_TOKEN_N; ii++)
	//	m_pToken[ii].Clear();

	load(&child->m_mapH, m_curDir);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	wccGetXmlNode(m_ElementNode, GLOBAL_DIVISION);
	buildXmlHeader(&m_ElementNode);
	int iCellIdx = 0;
	int iFormIdx = 0;
	int cnt = 0;
	ElementNode* pNode;
	CString tmps;
	while (wccGetFormNode(m_ElementNode, &iFormIdx))
	{
		for (int ii = 0; ii < m_ElementNode.GetCount(); ii++)
		{
			pNode = m_ElementNode.GetAt(ii);
			if (pNode->strValue.IsEmpty())
			{
				if (SearchElement(ii + 1, _T("TYPE"), tmps))
					tmps = tmps.Left(tmps.Find("|"));

				CformProp* Prop = new CformProp();
				Prop->Init(getObjectKind(pNode->strElement, tmps));
				cnt = wccGetCellNode(m_CellPropNode, &iCellIdx);
				MakeCellProp(Prop, pNode->strElement, cnt);
				CObjectLoad* ObjLoad = new CObjectLoad(Prop, m_piForm, &child->m_view);
				child->m_mapH.forms.Add(ObjLoad);

				buildXmlForm(&m_ElementNode, &m_CellPropNode);

				if (child->m_view.isValidTabOrder(Prop))
					Prop->index = ++child->m_view.m_nSeqTabOdr;
				if (child->m_view.isValidIOOrder(Prop))
					Prop->iorder = ++child->m_view.m_nSeqIoOdr;

				ObjLoad->LoadLib(Prop->kind);
				ObjLoad->LoadProc();
			}
		}
	}

	buildTab();

	if (child->m_mapH.onStart)
		DeleteFile(child->m_mapH.onStartN);
	if (child->m_mapH.onSend)
		DeleteFile(child->m_mapH.onSendN);
	if (child->m_mapH.onReceive)
		DeleteFile(child->m_mapH.onReceiveN);
	if (child->m_mapH.onAlert)
		DeleteFile(child->m_mapH.onAlertN);
	if (child->m_mapH.onService)
		DeleteFile(child->m_mapH.onServiceN);
	if (child->m_mapH.onFile)
		DeleteFile(child->m_mapH.onFileN);
	if (child->m_mapH.onSelect)
		DeleteFile(child->m_mapH.onSelectN);
	if (child->m_mapH.onTimer)
		DeleteFile(child->m_mapH.onTimerN);
	if (child->m_mapH.onFocus)
		DeleteFile(child->m_mapH.onFocusN);
	if (child->m_mapH.onClose)
		DeleteFile(child->m_mapH.onCloseN);
	if (child->m_mapH.onDevice)
		DeleteFile(child->m_mapH.onDeviceN);
	if (child->m_mapH.onKey)
		DeleteFile(child->m_mapH.onKeyN);
	if (child->m_mapH.onChangeLayout)
		DeleteFile(child->m_mapH.onChangeLayoutN);
	if (child->m_mapH.onFlickingN)
		DeleteFile(child->m_mapH.onFlickingN);
	if (child->m_mapH.onTrigger)
		DeleteFile(child->m_mapH.onTriggerN);
	if (child->m_mapH.onDeclaration)
		DeleteFile(child->m_mapH.onDeclarationN);
	if (child->m_mapH.onInDeclaration)
		DeleteFile(child->m_mapH.onInDeclarationN);

	child->m_mapH.onStart   = false;
	child->m_mapH.onSend    = false;
	child->m_mapH.onReceive = false;
	child->m_mapH.onAlert   = false;
	child->m_mapH.onService = false;
	child->m_mapH.onFile    = false;
	child->m_mapH.onSelect	= false;
	child->m_mapH.onTimer   = false;
	child->m_mapH.onFocus   = false;
	child->m_mapH.onClose   = false;
	child->m_mapH.onDevice  = false;
	child->m_mapH.onKey     = false;
	child->m_mapH.onChangeLayout = false;
	child->m_mapH.onFlicking = false;
	child->m_mapH.onTrigger = false;
	child->m_mapH.onDeclaration = false;
	child->m_mapH.onInDeclaration = false;

	wccGetXmlNode(m_ElementNode, RESOURCE_DIVISION);
	buildXmlResource(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, TRLAYOUT_DIVISION);
	BuildXmlTRlayout(&m_ElementNode);

	wccGetXmlNode(m_ElementNode, LAYOUTEDIT_DIVISION);
	BuildXmlLayoutEdit(&m_ElementNode);

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

void CMainFrame::setDataset(CChildFrame* child, int type, CString info)
{
	if (type == 0)	// trinfo
		child->m_mapH.trinfo = CString(info);
	else
		child->m_mapH.rtsinfo = CString(info);
}

void CMainFrame::editRtsInfo()
{
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();
	CString text, tmps;

	text = _T("");
	if (m_children.GetSize() && pActiveChild)
	{
		if (m_rtsDlg && !m_rtsDlg->IsWindowVisible())
		{
			delete m_rtsDlg;
			m_rtsDlg = NULL;
		}

		CformProp* Prop = NULL;
		cellOb* CellProp = NULL;

		for (int ii = 0; ii < pActiveChild->m_mapH.forms.GetCount(); ii++)
		{
			Prop = ((CObjectLoad*)pActiveChild->m_mapH.forms.GetAt(ii))->getProp();
			if (!Prop->name.IsEmpty())
			{
				switch (Prop->kind)
				{
				case fmGRID:
					text += Prop->name;
					text += _T("$=GRID");

					for (int jj = 0; jj < Prop->cellProperties->GetCount(); jj++)
					{
						CellProp = (cellOb*)Prop->cellProperties->GetAt(jj);
						if (CellProp->attributes & faSKIP)
							continue;
						tmps.Format(_T(":%s=%s"), CellProp->name, CellProp->flashKeys);
						text += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmGRIDEX:
					text += Prop->name;
					text += _T("$=GRIDEX");

					for (int jj = 0; jj < Prop->cellProperties->GetCount(); jj++)
					{
						CellProp = (cellOb*)Prop->cellProperties->GetAt(jj);
						if (CellProp->attributes & faSKIP)
							continue;
						tmps.Format(_T(":%s=%s"), CellProp->name, CellProp->flashKeys);
						text += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmTABLE:
					text += Prop->name;
					text += _T("$=TABLE");

					for (int jj = 0; jj < Prop->cellProperties->GetCount(); jj++)
					{
						CellProp = (cellOb*)Prop->cellProperties->GetAt(jj);
						if (CellProp->attributes & faSKIP)
							continue;
						tmps.Format(_T(":%s=%s"), CellProp->name, CellProp->flashKeys);
						text += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmCOMBO:
					text += Prop->name;
					tmps.Format(_T("=COMBO=%s%c"), Prop->flashKeys, GB_SYMBOL);
					text += tmps;
					break;

				case fmOBJECT:
					text += Prop->name;
					tmps.Format(_T("=CUSTOM=%s%c"), Prop->flashKeys, GB_SYMBOL);
					text += tmps;
					break;

				case fmOUTPUT:
					text += Prop->name;
					tmps.Format(_T("=OUTPUT=%s%c"), Prop->flashKeys, GB_SYMBOL);
					text += tmps;
					break;

				case fmEDIT:
					text += Prop->name;
					tmps.Format(_T("=EDIT=%s%c"), Prop->flashKeys, GB_SYMBOL);
					text += tmps;
					break;
				}
			}
		}

		if (m_rtsDlg == NULL)
		{
			m_rtsDlg = new CRtsInfo(this);
			m_rtsDlg->Create(IDD_RTS);
			m_rtsDlg->ShowWindow(SW_SHOW);
		}
		if (!text.IsEmpty())
		{
			m_rtsDlg->SetFormInfo(text);
			m_rtsDlg->SetRtsInfo(pActiveChild->m_mapH.rtsinfo);
		}
	}
}

void CMainFrame::editTrLayout(CString path, bool breset)
{
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();
	CString	text, tmps;

	text = _T("");
	//if (!isTrLayout(path))
	//	return;
	if (m_children.GetSize() && pActiveChild)
	{
		if (m_trDlg && !m_trDlg->IsWindowVisible())
		{
			delete m_trDlg;
			m_trDlg = NULL;
		}

		CformProp* Prop = NULL;
		cellOb* CellProp = NULL;
		for (int ii = 0; ii < pActiveChild->m_mapH.forms.GetCount(); ii++)
		{
			Prop = ((CObjectLoad*)pActiveChild->m_mapH.forms.GetAt(ii))->getProp();
			if (!Prop->name.IsEmpty())
			{
				switch (Prop->kind)
				{
				case fmGRID:
					text += Prop->name;
					text += _T("$=GRID");

					for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
					{
						CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
						if (CellProp->attributes & faSKIP)
							continue;
						tmps.Format(_T(":%s"), CellProp->name);
						text += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmGRIDEX:
					text += Prop->name;
					text += _T("$=GRIDEX");

					for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
					{
						CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
						if (CellProp->attributes & faSKIP)
							continue;
						tmps.Format(_T(":%s"), CellProp->name);
						text += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmTABLE:
					text += Prop->name;
					text += _T("$=TABLE");

					for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
					{
						CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
						if (CellProp->attributes & faSKIP)
							continue;
						tmps.Format(_T(":%s"), CellProp->name);
						text += tmps;
					}
					tmps.Format(_T("%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmCOMBO:
					text += Prop->name;
					tmps.Format(_T("=COMBO%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmOBJECT:
					text += Prop->name;
					tmps.Format(_T("=CUSTOM%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmOUTPUT:
					text += Prop->name;
					tmps.Format(_T("=OUTPUT%c"), GB_SYMBOL);
					text += tmps;
					break;

				case fmEDIT:
					text += Prop->name;
					tmps.Format(_T("=EDIT%c"), GB_SYMBOL);
					text += tmps;
					break;
				}
			}
		}

		bool	isfile = false;
		if (!path.IsEmpty() && !isTrLayout(path))
			return;

		if (m_trDlg == NULL)
		{
			m_trDlg = new CTrLayout(this, m_root);
			m_trDlg->Create(IDD_TRLAYOUT);
			m_trDlg->ShowWindow(SW_SHOW);
		}

		if (path.IsEmpty())
		{
			int	pos;

			tmps = _T("");
			if (!pActiveChild->m_mapH.trinfo.IsEmpty())
				tmps = CString(pActiveChild->m_mapH.trinfo);
			pos = tmps.Find("\t");
			if (pos != -1)
			{
				tmps = tmps.Left(pos);
				path.Format("%s\\%s\\%s", m_root, TRDIR, tmps);
			}
		}

		m_trDlg->ReadTrList(path);
		if (!path.IsEmpty())
			isfile = true;

		if (!text.IsEmpty())
			m_trDlg->FormObjectLayout(pActiveChild->m_mapH.trinfo, text, isfile);
	}
	else
	{
		AfxMessageBox("Please open map file.");
	}
}

BOOL CMainFrame::isTrLayout(CString path)
{
	CFile	file;
	CString data, tmps;

	if (path.IsEmpty())
		return FALSE;

	path.Trim();
	if (file.Open(path, CFile::modeRead))
	{
		char*	ptr;
		UINT	size, nRead;

		size = (UINT)file.GetLength();
		ptr = new char[size + 1];

		nRead = file.Read(ptr, size);
		if (size != nRead)
		{
			file.Close();
			delete[] ptr;
			return FALSE;
		}
		ptr[size] = '\0';
		data = UTF8ToAnsi(ptr);
		if (nRead)
			data = data.Left(nRead);	// ??? 무슨의미의 코드인지?
		file.Close();
		delete[] ptr;
	}
	else
		return FALSE;

	if (data.Find("TR") < 0)
	{
		AfxMessageBox("This file isn't TR_Layout File!");
		return FALSE;
	}
	return TRUE;
}

CString CMainFrame::UTF8ToAnsi(char* pszUTF8)
{
	int	nU8StrLen, nBStrLen, nAStrLen;
	BSTR	bstr;
	CString	str;
	char*	pszANSI;

	nU8StrLen = lstrlen(pszUTF8);
	nBStrLen = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, NULL, NULL);
	bstr = SysAllocStringLen(NULL, nBStrLen);
	MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, bstr, nBStrLen);

	nAStrLen = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);
	pszANSI = new char[nAStrLen];
	WideCharToMultiByte(CP_ACP, 0, bstr, -1, pszANSI, nAStrLen, NULL, NULL);
	str = (CString)bstr;
	SysFreeString(bstr);
	delete[] pszANSI;

	return str;
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
		return 15005;
	case 2:	// Proxy 15201
		return 15005;
	default:// Internet 80
		break;
	}
	return 80;
}
char CMainFrame::getObjectKind(CString strName, CString strSubName)
{
	CMapStringToPtr mapFormID;

	mapFormID.SetAt("LABEL",   (void*)fmLABEL);
	mapFormID.SetAt("BOX",     (void*)fmBOX);
	mapFormID.SetAt("GROUP",   (void*)fmGROUP);
	mapFormID.SetAt("PANEL",   (void*)fmIMVIEW);
	mapFormID.SetAt("BUTTON",  (void*)fmBUTTON);
	mapFormID.SetAt("EDIT",    (void*)fmEDIT);
	mapFormID.SetAt("COMBO",   (void*)fmCOMBO);
	mapFormID.SetAt("OUT",  (void*)fmOUTPUT);
	mapFormID.SetAt("MEMO",    (void*)fmMEMO);
	mapFormID.SetAt("GRID",    (void*)fmGRID);
	mapFormID.SetAt("GRIDEX",    (void*)fmGRIDEX);
	mapFormID.SetAt("TABLE",   (void*)fmTABLE);
	mapFormID.SetAt("TREEVIEW",    (void*)fmTREE);
	mapFormID.SetAt("OBJECT",  (void*)fmFORM);
	mapFormID.SetAt("USRTAB",     (void*)fmTAB);
	mapFormID.SetAt("BROWSER", (void*)fmBROWSER);
	mapFormID.SetAt("CONTROL", (void*)fmOBJECT);
	mapFormID.SetAt("RADIO",   (void*)fmRADIO);
	mapFormID.SetAt("CHECK",   (void*)fmCHECK);

	if (strName == "BUTTON")
	{
		if (strSubName == "TP_RADIO")
			strName = "RADIO";
		else if (strSubName == "TP_CHECK")
			strName = "CHECK";
	}

	void*	rtValue = 0;
	mapFormID.Lookup(strName, rtValue);
	return (char)rtValue;
}

CString CMainFrame::getObjectName(int kind, int type)
{
	switch(kind)
	{
	//case FM_SYS:
	//	return _T("SYSTEM");
	case fmLABEL:
		return _T("LABEL");
	case fmBOX:
		return _T("BOX");
	case fmGROUP:
		return _T("GROUP");
	case fmIMVIEW:
		return _T("PANEL");
	case fmBUTTON:
		return _T("BUTTON");
	case fmCHECK:
		return _T("CHECK");
	case fmRADIO:
		return _T("RADIO");
	case fmEDIT:
		return _T("EDIT");
	case fmCOMBO:
		return _T("COMBO");
	case fmOUTPUT:
		return _T("OUTPUT");
	case fmMEMO:
		return _T("MEMO");
	case fmGRID:
		return _T("GRID");
	case fmGRIDEX:
		return _T("GRIDEX");
	case fmTABLE:
		return _T("TABLE");
	case fmTREE:
		return _T("TREEVIEW");
	case fmFORM:
		return _T("OBJECT");
	case fmTAB:
		return _T("USRTAB");
	case fmBROWSER:
		return _T("BROWSER");
	case fmOBJECT:
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
	if (code >= 0 && IsWindow(pMainFrm->GetSafeHwnd()) && pMainFrm->IsWindowVisible())
	{
		MSG	msg;

		if (wp == VK_F11)	// F11
		{
			while (PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE|PM_NOYIELD))
				;
			pMainFrm->OnColumnEdit();
			return true;
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

bool CMainFrame::deleteAryCloneObj()
{
	CObjectLoad*	ObjLoad = NULL;
	CformProp*	Prop = NULL;
	for (size_t ii = (size_t)m_AryCloneObject.GetCount(); ii > 0; ii--)
	{
		ObjLoad = m_AryCloneObject.GetAt(ii - 1);
		Prop = (CformProp*)ObjLoad->getProp();
		if (Prop->kind == fmFORM)
		{
			//for (size_t ii = 0; ii < (size_t)m_ArryFormObj.GetCount(); ii++)
			//{
			//	CObjectLoad* FormLoad = (CObjectLoad*)m_ArryFormObj.GetAt(ii);
			//	if (ObjLoad->getWnd() == FormLoad->m_view)
			//	{
			//		delete FormLoad;
			//		m_ArryFormObj.RemoveAt(ii);
			//		break;
			//	}
			//}
		}
		delete ObjLoad;
	}
	m_AryCloneObject.RemoveAll();
	return true;
}

void CMainFrame::ObjClone(CObjectLoad** dstObj, CObjectLoad* srcObj)
{
	CformProp*	Prop = (CformProp*)srcObj->getProp();
	CformProp* formProp = new CformProp();
	formProp->SetCopy(Prop);

	CChildFrame* child;
	child = (CChildFrame *)MDIGetActive();

	formProp->rect.OffsetRect(10 * child->m_CloneCnt, 10 * child->m_CloneCnt);
	if (!formProp->groupRc.IsRectEmpty())
	{
		formProp->groupRc.OffsetRect(10 * child->m_CloneCnt, 10 * child->m_CloneCnt);
	}
	*dstObj = new CObjectLoad(formProp, m_piForm, &child->m_view);

	(*dstObj)->LoadLib(formProp->kind);
	(*dstObj)->getProp()->name = child->m_view.GetInitSymbol("", Prop->kind);
}

void CMainFrame::FormResize(CformProp* dstForm, CWnd* srcWnd, mapForm* mapH)
{
	CRect rc;
	srcWnd->GetClientRect(rc);
	float w = (float)rc.Width() / mapH->width;
	float h = (float)rc.Height() / mapH->height;

	dstForm->rect.left = (LONG)(dstForm->rect.left * w);
	dstForm->rect.top = (LONG)(dstForm->rect.top * h);
	dstForm->rect.right = (LONG)(dstForm->rect.right * w);
	dstForm->rect.bottom = (LONG)(dstForm->rect.bottom * h);
	dstForm->fontPoint = (LONG)(dstForm->fontPoint * max(w, h));

	//if (iRet == 2 || iRet == 3) // grid == 2, table == 3
	if (dstForm->cellProperties != NULL && dstForm->cellProperties->IsEmpty())
	{
		int cellH = (int)(LOWORD(dstForm->keyValues) * h);
		dstForm->keyValues = MAKELONG(cellH, HIWORD(dstForm->keyValues));

		for (int ii = 0; ii < dstForm->cellProperties->GetCount(); ii++)
			((cellOb*)dstForm->cellProperties->GetAt(ii))->width = (LONG)(((cellOb*)dstForm->cellProperties->GetAt(ii))->width * w);
	}
}

bool CMainFrame::CheckFile(CString fileN)
{
	CFile fp;
	if (!fp.Open(fileN, CFile::modeRead))
		return false;

	char pbuf[255] = { 0, };
	fp.Read(pbuf, 15);
	
	if (strcmp(pbuf, "GLOBAL DIVISION") == 0)
		return true;
	else
		MessageBox("This file is not AXIS source!!");
	return false;
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
		strPath.Format("%s\\%s.XML", m_currentPath, sMap); //m_SourcePath
	else
		strPath.Format("%s\\%s", m_currentPath, sMap); //m_SourcePath
	m_resouceDlg->setPath(strPath);
	m_resouceDlg->Create(IDD_RESOURCE_HISTORY);
	m_resouceDlg->CenterWindow();
	m_resouceDlg->ShowWindow(SW_SHOW);

}

void CMainFrame::CheckOutDlg(CString sName)
{
	bool wMode = false;
	CChildFrame* child;
	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (IsWindow(child->m_hWnd))
		{
			if (child->m_mapH.mapN == sName && child->m_mapH.mode == 'W')
			{
				wMode = true;
				break;
			}
		}
	}

	if (wMode)
	{
		CCheckOutDlg dlg;
		dlg.SetFormName(sName);
		if (dlg.DoModal() != IDOK)
		{
			return;
		}
	}

	CDownload* pDown = new CDownload();
	if (!pDown->Create())
	{
		delete pDown;
		return;
	}

	if (!pDown->Download(m_userID, sName, _T(""), _T(""), TRGB_FORMCHECK))
	{
		delete pDown;
		return;
	}
	delete pDown;
}

CString CMainFrame::setMapPath(CString mapN)
{
	CString tmps;

	if (mapN.GetLength() != L_MAPN)
		return _T("");

	tmps.Format(_T("map/%s/%s/%s"), mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	return tmps;
}


bool CMainFrame::DownMapDlg(CString sName, int mode, int nSel)
{
	CChildFrame* child;
	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (IsWindow(child->m_hWnd))
		{
			if (child->m_mapH.mapN == sName)
			{
				if (!child->DeleteFiles())
					return false;
				child->DestroyWindow();
			}
		}
	}
	/////////////////////////////////////////
	CString sRoot;
	sRoot.Format("%s\\exe\\tmp\\%s", m_root, sName);

	CDownload* pDown = new CDownload();
	if (!pDown->Create())
	{
		delete pDown;
		return false;
	}
	CString path;
	path = setMapPath(sName);
	if (!pDown->Download(m_userID, "", path, sRoot, 0, mode, nSel))
	{
		delete pDown;
		return false;
	}
	delete pDown;

	return true;
}

bool CMainFrame::DownDiff(CString sName, int nNewSel, int nOldSel)
{
	CString sDiffL, sDiffR;
	sDiffL.Format("%s\\exe\\tmp\\%s_rev%d", m_root, sName, nNewSel);
	sDiffR.Format("%s\\exe\\tmp\\%s_rev%d", m_root, sName, nOldSel);
	DeleteFile(sDiffL);
	DeleteFile(sDiffR);

	CDownload* pDown = new CDownload();
	if (!pDown->Create())
	{
		delete pDown;
		return false;
	}
	CString path;
	path = setMapPath(sName);
	if (!pDown->Download(m_userID, "", path, sDiffL, 0, 'R', nNewSel))
	{
		delete pDown;
		return false;
	}

	if (nNewSel == 1 || nNewSel == -1)
	{
		delete pDown;
		return false;
	}
	
	if (!pDown->Download(m_userID, "", path, sDiffR, 0, 'R', nOldSel))
	{
		delete pDown;
		return false;
	}
	delete pDown;
	return true;
}

void CMainFrame::ShowDiff(CString sName, int nNewSel, int nOldSel)
{
	CString tmp;
	DownDiff(sName, nNewSel, nOldSel);
	if (nNewSel == -1)
		tmp.Format("\"%s\\exe\\tmp\\%s_tmp\" \"%s\\exe\\tmp\\%s.xml\"", m_root, sName, m_root, sName);
	else
		tmp.Format("\"%s\\exe\\tmp\\%s_rev%d\" \"%s\\exe\\tmp\\%s_rev%d\"", m_root, sName, nNewSel, m_root, sName, nOldSel);
	ShellExecute(NULL, "open", m_strDiffViewer, tmp, NULL, SW_SHOW);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIFrameWndEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		CChildFrame* child;
		child = (CChildFrame *)MDIGetActive();
		if (child)
		{
			child->m_view.m_FmCtrl->InvalidateObject();
		}
	}
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 200)
	{
		KillTimer(200);
		BuilderLogout();
		MessageBox("login fail", NULL, MB_TOPMOST);
		m_loginDlg.EnableLogin(true);
	}
	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::BuilderLogin()
{
#ifdef _DEBUG
	if (m_userID == "")
	{
		m_userID = _T("winix0");
		m_pass = _T("winix");
	}
#endif
	if (m_builderLogin)
	{
		delete m_builderLogin;
		m_builderLogin = NULL;
	}
	m_builderLogin = new CbuilderLogin(m_userID, m_pass, m_domain);
	if (!m_builderLogin->Create())
	{
		BuilderLogout();
		return;
	}
	SetTimer(200, 3000, NULL);
}

void CMainFrame::BuilderLogout()
{
	if (m_builderLogin)
	{
		delete m_builderLogin;
		m_builderLogin = NULL;
	}
}

void CMainFrame::OnEditFormsearch()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CChildFrame*	child;
	child = (CChildFrame *)MDIGetActive();
	if (child && child->m_mapH.mapK == MK_PROCEDURES)
		SendMessage(WM_COMMAND, ID_EDIT_FORMSEARCH);
}

void CMainFrame::SetCtmObjProp(DWORD idx, CString strProp)
{
	m_wndProperties.SetCmtObjProp(idx, strProp);
}

void CMainFrame::ShowCtmObjDlg()
{
	CCustomObjDlg CustomObjDlg;
	if (CustomObjDlg.DoModal() == IDOK)
	{
		m_strCtmObj = CustomObjDlg.m_strName;
		CChildFrame* child = (CChildFrame *)MDIGetActive();
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		if (child)
			child->DrawForm(ID_TOOL_OBJECT);
	}
}

void CMainFrame::ShowADBViewer()
{
	if (m_pCADBDlg == NULL)
	{
		m_pCADBDlg = new CADBDlg();
		m_pCADBDlg->Create(IDD_DIALOG_ADB);
	}
	m_pCADBDlg->CenterWindow();
	m_pCADBDlg->ShowWindow(SW_SHOW);
}

void CMainFrame::SetDiffViewer()
{
	CDiffViewerDlg DiffDlg(m_strDiffViewer);
	if (DiffDlg.DoModal() == IDOK)
	{
		m_strDiffViewer = DiffDlg.m_strDiffViewer;
		WritePrivateProfileString(ENVIRONMENT, _T("DIFFVIEWER"), m_strDiffViewer, m_currentPath + _T("\\workshop.ini"));
	}
}

void CMainFrame::ShowPreview(int iStart)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString dir;
	int iIndex = 0;
	CWnd *pWnd = NULL;
	CFileStatus fs;

	int	totL;
	CString tmps;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CString tpath = _T("");
	tpath.Format("%s\\mtlayout\\*.*", m_root);
	

	//검색 클래스
	CFileFind finder;

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(tpath); //

	CString fileName;

	m_aryFile.RemoveAll();
	while (bWorking)
	{
		//다음 파일 / 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();
		//파일 일때
		if (finder.IsArchived())
		{
			//파일의 이름
			fileName = finder.GetFileName();
			// 현재폴더 상위폴더 썸네일파일은 제외
			if (fileName == _T(".") ||
				fileName == _T("..") ||
				fileName == _T("Thumbs.db")) continue;

			fileName.Format("%s\\mtlayout\\%s", m_root, finder.GetFileName());
			m_aryFile.Add(fileName);
			//읽어온 파일 이름을 리스트박스에 넣음
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CObjectLoad* FormLoad = NULL;
	for (size_t ii = 0; ii < (size_t)m_ArryNewMap.GetCount(); ii++)
	{
		FormLoad = (CObjectLoad*)m_ArryNewMap.GetAt(ii);
		delete FormLoad;
	}
	m_ArryNewMap.RemoveAll();

	m_MTLayoutDlg.InitPreview();
	m_MTLayoutDlg.SetMaxCnt(m_aryFile.GetCount());
	for (int ii = 0; ii < m_MTLayoutDlg.m_aryWnd.GetCount(); ii++)
	{
		if (ii + iStart > m_aryFile.GetCount() - 1)
			break;
		fileName = m_aryFile.GetAt(ii + iStart);

		pWnd = m_MTLayoutDlg.m_aryWnd.GetAt(ii);
		totL = wccLoadSource(fileName, (CWnd *)NULL);
		if (totL == -1)
			return;

		for (int jj = 0; jj < MAX_TOKEN_N; jj++)
			m_pToken[jj].Clear();

		class mapForm	m_mapH;
		load(&m_mapH, m_curDir);

		wccGetXmlNode(m_ElementNode, GLOBAL_DIVISION);
		buildXmlHeader(&m_ElementNode);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (m_mapH.imageK == IK_BRUSH)
		{
			m_MTLayoutDlg.SetBkClr(ii, m_palette->GetPaletteRGB(m_mapH.bkClr));
		}
		else if(m_mapH.imageK == IK_BMP)
		{
			CString path("");
			path.Format(_T("%s\\%s\\%s"), m_root, IMAGEDIR, m_mapH.imageV);
			m_MTLayoutDlg.SetBkImg(ii, path);
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int iCellIdx = 0;
		int iFormIdx = 0;
		int cnt = 0;
		ElementNode* pNode;
		while (wccGetFormNode(m_ElementNode, &iFormIdx))
		{
			for (int ii = 0; ii < m_ElementNode.GetCount(); ii++)
			{
				pNode = m_ElementNode.GetAt(ii);
				if (pNode->strValue.IsEmpty())
				{
					if (SearchElement(ii + 1, _T("TYPE"), tmps))
						tmps = tmps.Left(tmps.Find("|"));

					CformProp* Prop = new CformProp();
					Prop->Init(getObjectKind(pNode->strElement, tmps));
					cnt = wccGetCellNode(m_CellPropNode, &iCellIdx);
					MakeCellProp(Prop, pNode->strElement, cnt);
					CObjectLoad* formObjLoad = new CObjectLoad(Prop, m_piForm, (CChildView*)pWnd);
					m_mapH.forms.Add(formObjLoad);

					buildXmlForm(&m_ElementNode, &m_CellPropNode);

					FormResize(Prop, pWnd, &m_mapH);
					formObjLoad->LoadLib(Prop->kind);
					formObjLoad->LoadProc();

					m_ArryNewMap.Add(formObjLoad);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}