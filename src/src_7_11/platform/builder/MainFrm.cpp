// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "axisWork.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "mapform.h"
#include "mainvar.h"
#include "awDlg/saveDlg.h"
#include "awDlg/addrDlg.h"
#include "awDlg/SettingDlg.h"
#include "awDlg/OptionsSheet.h"
#include "awDlg/EditOrder.h"
#include "awDlg/LineDlg.h"
#include "awDlg/batch.h"
#include "awDlg/updlg.h"
#include "awDlg/uptmpldlg.h"
#include "awBuild/libBuild.h"
#include "awCommon/choiceDlg.h"
#include "Components/Splash.h"
#include "Components/BCMenu.h"


#include "../dll/lib/axislib.h"
#include "../h/axisfire.h"

#if _MSC_VER < 1300
#include <..\src\oleimpl2.h> //for COleFrameHook
#else
#include <..\src\mfc\oleimpl2.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IPORT 15101 //15101 15105

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFindReplace)
	ON_WM_MEASUREITEM()
	ON_WM_MENUCHAR()
	ON_WM_INITMENUPOPUP()
	ON_WM_ACTIVATE()
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditFindReplace)
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATUS, OnStatus)
	ON_MESSAGE(WM_OPENOBJ, OnOpenObject)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnLoginMessage)
	ON_COMMAND_RANGE(ID_APP_NEW, ID_VIEW_SELECTIONBAR, OnMainCommand)
	ON_COMMAND_RANGE(ID_EX_FORMAT, ID_EX_RGB, OnExCommand)
	ON_COMMAND_RANGE(ID_AL_LEFT, ID_SZ_VEQUAL, OnAlignCommand)
	ON_COMMAND_RANGE(ID_TOOL_POINTER, ID_TOOL_CONTROL, OnDrawCommand)
	ON_COMMAND_RANGE(ID_DROPDOWNBASE, ID_DROPDOWNBASE + 50, OnDropdownCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_APP_NEW, ID_VIEW_SELECTIONBAR, OnUpdateMainCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_EX_FORMAT, ID_EX_RGB, OnUpdateExCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AL_LEFT, ID_SZ_VEQUAL, OnUpdateDrawCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_TOOL_POINTER, ID_TOOL_CONTROL, OnUpdateDrawCommand)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_EVENTSINK_MAP(CMainFrame)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CMainFrame, -1, 1, OnFireEvent, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_EXT, 
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_copied   = false;
	m_showFORM = true;
	m_request  = true;
	m_memory   = _T("");

	m_pToken  = new CMapToken[MAX_TOKEN_N];
	m_pGroup  = NULL;

	m_palette = (CAxisPalette *) NULL;
	m_draw    = (CAxisDraw *) NULL;
	m_children.RemoveAll();

	m_block.bRc.SetRectEmpty();
	m_block.count    = 0;

	m_memFORM = (HANDLE) NULL;
	m_memREP  = (HANDLE) NULL;
	ZeroMemory((char *)&m_clipMAP, sizeof(_mapH));

	m_delta    = CSize(0, 0);
	m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), 
			-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	m_dropPnt  = CPoint(-1, -1);
	m_status   = 0;

	m_pWizardCtrl = NULL;

	m_backupSSMap.RemoveAll();
	m_sInitMap.Empty();
	m_curDir.Empty();

	m_bSDI = FALSE;
}

CMainFrame::~CMainFrame()
{
	m_tips.DestroyWindow();

	if (m_pWizardCtrl)
		delete m_pWizardCtrl;

	if (m_pGroup)	delete m_pGroup;
	delete[] m_pToken;

	if (m_palette)	delete m_palette;
	if (m_draw)	delete m_draw;

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

	if (!m_curDir.IsEmpty() && m_curDir.Find("\\tmp") != -1)
	{
		CString		fileN;
		WIN32_FIND_DATA	ffData;
		
		fileN.Format("%s\\*.*", m_curDir);
		HANDLE	hFile = ::FindFirstFile(fileN, &ffData);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			fileN.Format("%s\\%s", m_curDir, ffData.cFileName);
			DeleteFile(fileN);
			for (;;)
			{
				ZeroMemory(&ffData, sizeof(WIN32_FIND_DATA));
				if (!::FindNextFile(hFile, &ffData))
				{
					FindClose(hFile);
					break;
				}
				fileN.Format("%s\\%s", m_curDir, ffData.cFileName);
				DeleteFile(fileN);
			}
		}
	}
	CFormItem formItem(&m_clipMAP);
	formItem.RemoveAllForm();
	m_controlEvent.RemoveAll();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	char cPath[1024];
	GetCurrentDirectory(1024, cPath);
	m_currentPath = cPath;
	lpCreateStruct->style |= WS_CLIPCHILDREN;
	lpCreateStruct->style &= ~WS_VISIBLE;

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (((CWorkApp*)AfxGetApp())->m_wndMDIClient.Attach(m_hWndMDIClient) == 0)
	{
		TRACE("Failed to attach MDIClient.\n");
		return -1;      // fail to create
	}

	// login
	m_LoginDlg.m_currentPath = m_currentPath;
	m_LoginDlg.Create(IDD_LOGIN);
	m_LoginDlg.ShowWindow(SW_SHOW);
	m_LoginDlg.setParent(this);

	DragAcceptFiles();

	m_tips.Create(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (WS_VSCROLL|WS_HSCROLL);
	if (!CMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::RecalcLayout(BOOL bNotify) 
{
	CMDIFrameWnd::RecalcLayout(bNotify);
	//CMDIFrameWnd::RecalcLayout(bNotify);
}

void CMainFrame::OnUpdateMainCommand(CCmdUI* pCmdUI)
{
	CString	tmps   = _T("");
	BOOL	enable = FALSE;
	CWinApp* app   = AfxGetApp();
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	switch (pCmdUI->m_nID)
	{
	case ID_APP_NEW:
	case ID_APP_NEWTMPL:
	case ID_APP_NEWSC:
		if (m_children.GetSize() < MAX_MAP)
			enable = TRUE;
		break;

	case ID_HIDE_FORM:
		enable = TRUE;
		pCmdUI->SetCheck(m_showFORM ? 1 : 0);
		break;

	case ID_APP_COMPILE:
	case ID_APP_MOVE_AXIS:
		if (child && 
			(child->m_mapH.mapK == MK_NORM || child->m_mapH.mapK == MK_PROCEDURES) &&
			!child->m_view.m_pPreView)
			enable = TRUE;
		break;

	case ID_APP_MAPCLOSE:
	case ID_APP_SETBOOKMARK:
	case ID_APP_GOBOOKMARK:
	case ID_APP_ORDER:
	case ID_APP_SOURCE:
	case ID_APP_SAVEPC:
	case ID_APP_REFRESH:
	case ID_EDIT_TABORDER:
	case ID_EDIT_IOORDER:
	case ID_APP_EDITTABORDER:
	case ID_EDIT_GOTO:
		if (child && !child->m_view.m_pPreView)
			enable = TRUE;
		break;
	
	case ID_APP_SAVEPATH:
	case ID_APP_MAPINFO:
	case ID_APP_PREVIEW:
		if (child)
			enable = TRUE;
		break;
	case ID_EDIT_MAPUNDO:
		if (child && child->canUndo() && !child->m_view.m_pPreView)
			enable = TRUE;
		break;
	case ID_EDIT_MAPREDO:
		if (child && child->canRedo() && !child->m_view.m_pPreView)
			enable = TRUE;
		break;
	case ID_APP_ALLCOMPILE:
	case ID_APP_ALLMOVEAXIS:
		if (m_children.GetSize() > 1)
		{
			int cnt = 0;
			for (int ii = 0; ii < m_children.GetSize(); ii++)
				if ((m_children.GetAt(ii)->m_mapH.mapK == MK_NORM || 
					m_children.GetAt(ii)->m_mapH.mapK == MK_PROCEDURES) &&
					!m_children.GetAt(ii)->m_view.m_pPreView)
					cnt++;
			if (cnt > 1)
				enable = TRUE;
		}
		break;

	case ID_APP_USERM:
		if (child && child->m_mapH.mapN[0] != NULL)
			enable = TRUE;
		break;

	case ID_APP_BATCH:
	case ID_VIEW_SOURCE_VIEWER:
		enable = TRUE;
		break;

	case ID_VIEW_SOURCE:
		if (child && child->m_mapH.source)
			enable = TRUE;
		break;

	case ID_APP_CUT:
	case ID_APP_COPY:
		if ((child && (::GetFocus() == child->m_view.m_hWnd ||
			::GetFocus() == child->m_hWnd)) || 
			(child && child->m_mapH.mapK == MK_PROCEDURES))
			enable = child->IsCopyEnable();
		else
			enable = FALSE;
		break;

	case ID_VIEW_ALLSELECT:
		if ((child && ::GetFocus() == child->m_view.m_hWnd) || (child && child->m_mapH.mapK == MK_PROCEDURES))
			enable = TRUE;
		else
			enable = FALSE;
		break;

	case ID_APP_PASTE:
		if (m_clipMAP.formR || (child && child->m_mapH.mapK == MK_PROCEDURES))
			enable = TRUE;
		break;

	case ID_VIEW_MAINBAR:
		enable = TRUE;
		pCmdUI->SetCheck(m_mainTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_DRAWBAR:
		enable = TRUE;
		pCmdUI->SetCheck(m_drawTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_ALIGNBAR:
		enable = TRUE;
		pCmdUI->SetCheck(m_alignTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_EXBAR:
		enable = TRUE;
		pCmdUI->SetCheck(m_exTB.GetStyle() & WS_VISIBLE);
		break;

	case ID_VIEW_WORKSPACE:
		enable = TRUE;
		if (m_wndWorkBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_VIEW_PROPERTIES:
		enable = TRUE;
		if (m_wndPropertiesBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_VIEW_RUN:
		enable = TRUE;
		if (m_wndPreviewBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_VIEW_DEBUG:
		enable = TRUE;
		if (m_wndTraceBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_VIEW_WNDTAB:
		enable = TRUE;
		if (::IsWindow(m_wndMDITabs.m_hWnd))
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_VIEW_SELECTIONBAR:
		enable = TRUE;
		if (m_wndSelectionBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_APP_RELOAD:
		if (m_children.GetSize() > 0)
		{
			if (child && child->m_mapH.source)
				enable = TRUE;
		}
		break;

	case ID_APP_SAVESERVER:
		enable = FALSE;
		tmps = (char*)SendMessage(WM_USER, ID_USR_GETIPS, 0);
		if (child && child->m_mapH.source && !tmps.IsEmpty() && !m_bOffline)
			 enable = TRUE;
		break;

	case ID_FILE_SAVETMPL:
		enable = FALSE;
		tmps = (char*)SendMessage(WM_USER, ID_USR_GETIPS, 0);
		if (child && !tmps.IsEmpty())
			 enable = TRUE;
		break;

	case ID_EDIT_SCRIPT:
		enable = TRUE;
		if (m_wndScriptBar.IsWindowVisible())
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
		break;

	case ID_CONVERT_30:
		enable = TRUE;
		break;

	default:
		return;
	}

	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateExCommand(CCmdUI* pCmdUI)
{
	BOOL	enable = TRUE;
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	if (child)
		enable = child->IsEditEnable(pCmdUI->m_nID);
	else
		enable = FALSE;

	pCmdUI->Enable(enable);
}

void CMainFrame::OnUpdateDrawCommand(CCmdUI* pCmdUI)
{
	BOOL	enable;
	CChildFrame* child = (CChildFrame *)MDIGetActive();

	if (child && child->m_mapH.mapK != MK_PROCEDURES && !child->m_view.m_pPreView)
		enable = TRUE;
	else
		enable = FALSE;

	pCmdUI->Enable(enable);
}

void CMainFrame::OnMainCommand(UINT cmdID)
{
	CChildFrame*	child;
	BOOL		visible;

	SendMessage(WM_USER, ID_USR_ERASECTRL, 0);
	switch (cmdID)
	{
	case ID_APP_NEW:
		if (m_sInitMap.IsEmpty())
			createNewChild(MK_NORM);
		else
		{
			CFileFind fFind;
			if (fFind.FindFile(m_sInitMap) && 
				!(GetFileAttributes(m_sInitMap) & FILE_ATTRIBUTE_DIRECTORY))
				fileOpen(m_sInitMap, true);
			else if (m_sInitMap.GetLength() == 8)
				m_wndWorkBar.m_TreeServer.OpenMap(m_sInitMap);
			else
				createNewChild(MK_NORM);
		}
		break;

	case ID_APP_NEWTMPL:
		createNewChild(MK_TEMPLATE);
		break;

	case ID_APP_NEWSC:
		createNewChild(MK_PROCEDURES);
		break;

	case ID_APP_SETTING:
		{
			COptionsSheet Dlg("Setting options", this);
			if (Dlg.DoModal() == IDOK)
			{
				bool bInvalidate = false;
				Dlg.SaveInfos();
				((CWorkApp*)AfxGetApp())->m_nGap = Dlg.m_pBaseInfoPage->m_nGap;
				m_root = Dlg.m_pBaseInfoPage->m_path;

				m_sDefFontName = Dlg.m_pDefaultPage->m_fontname;
				m_sDefFontPnt  = Dlg.m_pDefaultPage->m_fontpnt;
				m_sInitMap     = Dlg.m_pBaseInfoPage->m_mapPath;
				if (Dlg.m_pBaseInfoPage->m_nDisplayMode != m_nDisplayMode)
					bInvalidate = true;
				m_nDisplayMode = Dlg.m_pBaseInfoPage->m_nDisplayMode;

				LoadScriptColors();

				SendMessage(WM_USER, ID_USR_SETROOTDIR, (LPARAM)m_root.operator LPCTSTR());

				if (bInvalidate)
					PostMessage(WM_USER, ID_USR_ALLVIEWREDRAW);
			}
			break;
			/*CSettingDlg Dlg(this, m_root);
			Dlg.m_currentPath = m_currentPath;
			if (Dlg.DoModal() == IDOK)
			{
				((CWorkApp*)AfxGetApp())->m_nGap = Dlg.m_nGap;
				SendMessage(WM_USER, ID_USR_SETGAP, Dlg.m_nGap);	// set gap
				m_root = Dlg.m_path;

				m_sDefFontName = Dlg.m_fontname;
				m_sDefFontPnt  = Dlg.m_fontpnt;
				m_sInitMap     = Dlg.m_mapPath;

				SendMessage(WM_USER, ID_USR_SETROOTDIR, (LPARAM)m_root.operator LPCTSTR());
			}*/
		}
		break;

	case ID_APP_OPENPC:
		openLocalFILE();	break;

	case ID_APP_SAVEPC:
		saveLocalMAP();		break;

	case ID_APP_SAVEPATH:
		saveLocalPath();	break;

	case ID_APP_MAPCLOSE:
		closeChild();		break;

	case ID_APP_ORDER:
		{
			child = (CChildFrame *) MDIGetActive();
			for (int ii = 0; ii < child->m_pFormItem->getFormCount(); ii++)
				child->m_pFormItem->getFormPoint(ii)->block = false;
			child->m_block->count = 0;
			child->m_view.m_nIoOdr = 0;
			child->m_view.sortIoOrder();
			child->m_view.m_nIoOdr = 1;
			child->m_view.m_nTabOdr = 0;
			child->m_view.Invalidate();

			CEditOrder dlg(NULL, &child->m_mapH);
			if (dlg.DoModal() == IDOK)
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);

			child->m_view.m_nIoOdr = 0;
			child->m_view.sortIoOrder();
			child->m_view.m_nTabOdr = 0;
			child->m_view.Invalidate();
		}
		break;

	case ID_APP_EDITTABORDER:
		{
			child = (CChildFrame *) MDIGetActive();
			for (int ii = 0; ii < child->m_pFormItem->getFormCount(); ii++)
				child->m_pFormItem->getFormPoint(ii)->block = false;
			child->m_block->count = 0;
			child->m_view.m_nTabOdr = 0;
			child->m_view.sortTabOrder();
			child->m_view.m_nTabOdr = 1;
			child->m_view.m_nIoOdr = 0;
			child->m_view.Invalidate();

			CEditOrder dlg(NULL, &child->m_mapH, 1);	// taborder
			if (dlg.DoModal() == IDOK)
				SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);

			child->m_view.m_nTabOdr = 0;
			child->m_view.sortTabOrder();
			child->m_view.m_nIoOdr = 0;
			child->m_view.Invalidate();
		}
		break;

	case ID_APP_COPY:
		child = (CChildFrame *) MDIGetActive();
		child->Copy();
		break;
	
	case ID_APP_CUT:
		child = (CChildFrame *) MDIGetActive();
		child->Cut();
		break;

	case ID_APP_PASTE:
		child = (CChildFrame *) MDIGetActive();
		if (m_clipMAP.formR || (child && child->m_mapH.mapK == MK_PROCEDURES))
			child->Paste();
		break;

	case ID_APP_SOURCE:
		generateMAP();		break;

	case ID_APP_COMPILE:
		compileMAP();		break;

	case ID_APP_MOVE_AXIS:
		applyAXIS();		break;

	case ID_APP_ALLCOMPILE:
		compileMAP(1);		break;

	case ID_APP_ALLMOVEAXIS:
		applyAXIS(1);		break;

	case ID_APP_PREVIEW:
		{
			child = (CChildFrame *) MDIGetActive();
			
			if (child && child->m_mapH.mapK == MK_NORM)
			{
				if (!child->m_view.m_pPreView)
				{
					compileMAP();
					applyAXIS();
				}
				child->m_view.Preview();
			}
		}
		break;

	case ID_APP_SETBOOKMARK:
		child = (CChildFrame *) MDIGetActive();
		if (child)
			child->setBookMark(getSource(false, &child->m_mapH));		
		break;

	case ID_EDIT_MAPUNDO:
		child = (CChildFrame *) MDIGetActive();
		if (child)
		{
			CString sUndo = child->popUndo();
			if (!sUndo.IsEmpty())
			{
				child->pushRedo(getSource(false, &child->m_mapH));
				changeMap(sUndo);
			}
		}
		break;
	case ID_EDIT_MAPREDO:
		child = (CChildFrame *) MDIGetActive();
		if (child)
		{
			CString sRedo = child->popRedo();
			if (!sRedo.IsEmpty())
			{
				child->pushUndo(getSource(false, &child->m_mapH));
				changeMap(sRedo);
			}
		}
		break;

	case ID_VIEW_MAINBAR:
		visible = m_mainTB.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_mainTB, !visible, FALSE);
		break;

	case ID_VIEW_DRAWBAR:
		visible = m_drawTB.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_drawTB, !visible, FALSE);
		break;

	case ID_VIEW_ALIGNBAR:
		visible = m_alignTB.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_alignTB, !visible, FALSE);
		break;

	case ID_VIEW_EXBAR:
		visible = m_exTB.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_exTB, !visible, FALSE);
		break;

	case ID_VIEW_WORKSPACE:
		visible = m_wndWorkBar.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_wndWorkBar, !visible, FALSE);
		break;

	case ID_VIEW_PROPERTIES:
		visible = m_wndPropertiesBar.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_wndPropertiesBar, !visible, FALSE);
		break;

	case ID_VIEW_RUN:
		visible = m_wndPreviewBar.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_wndPreviewBar, !visible, FALSE);
		break;

	case ID_VIEW_DEBUG:
		visible = m_wndTraceBar.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_wndTraceBar, !visible, FALSE);
		break;

	case ID_VIEW_WNDTAB:
		if (::IsWindow(m_wndMDITabs.m_hWnd))
		{
			m_wndMDITabs.DestroyWindow();
			RecalcLayout();
		}
		else
			ReCreateMDITabs();
		break;

	case ID_VIEW_SELECTIONBAR:
		visible = m_wndSelectionBar.GetStyle() & WS_VISIBLE ? true : false;
		ShowControlBar(&m_wndSelectionBar, !visible, FALSE);
		break;

	case ID_CONVERT_30:
		{/*
			CFileDialog fDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, "All Files (*.*)|*.*||");
			fDlg.m_ofn.lpstrInitialDir = m_openDir;
			if (fDlg.DoModal() != IDOK)
				return;
			
			if (convert(fDlg.GetPathName(), m_curDir))
			{
				CString sFileName;
				sFileName.Format("%s\\%s", m_curDir, fDlg.GetFileName());
				fileOpen(sFileName);
			}
		*/
		}
		break;

	case ID_APP_RELOAD:
		reloadMAP();		break;

	case ID_HIDE_FORM:
		showFORM();		break;

	case ID_VIEW_SOURCE_VIEWER:
		setSourceViewer();	break;

	case ID_VIEW_SOURCE:
		showSource();		break;

	case ID_APP_SAVESERVER:
		child = (CChildFrame *) MDIGetActive();
		if (child->m_mapH.mapK == MK_NORM || child->m_mapH.mapK == MK_PROCEDURES)
		{
			generateMAP(false, child);
			saveServerMAP();
		}
		else
			saveServerTmpl();
		break;

	case ID_APP_REFRESH:
		child = (CChildFrame *) MDIGetActive();
		child->m_view.Invalidate();
		break;

	case ID_APP_BATCH:
		batchCompile();		break;

	case ID_EDIT_SCRIPT:
		visible = m_wndScriptBar.GetStyle() & WS_VISIBLE ? true : false;
		child = (CChildFrame *) MDIGetActive();
		if (child && !visible)
			m_wndScriptBar.SelectControl(child->getSelectedForm());
		if (!visible)
			SendMessage(WM_USER, ID_USR_STATUSLINEINFO, -1);
		ShowControlBar(&m_wndScriptBar, !visible, FALSE);
		break;

	case ID_EDIT_TABORDER:
		{
			child = (CChildFrame *) MDIGetActive();
			for (int ii = 0; ii < child->m_pFormItem->getFormCount(); ii++)
				child->m_pFormItem->getFormPoint(ii)->block = false;
			child->m_block->count = 0;
			child->m_view.m_nTabOdr = 0;
			child->m_view.sortTabOrder();
			child->m_view.m_nTabOdr = 1;
			child->m_view.m_nIoOdr = 0;
			child->m_view.Invalidate();
		}
		break;

	case ID_EDIT_IOORDER:
		{
			child = (CChildFrame *) MDIGetActive();
			for (int ii = 0; ii < child->m_pFormItem->getFormCount(); ii++)
				child->m_pFormItem->getFormPoint(ii)->block = false;
			child->m_block->count = 0;
			child->m_view.m_nIoOdr = 0;
			child->m_view.sortIoOrder();
			child->m_view.m_nIoOdr = 1;
			child->m_view.m_nTabOdr = 0;
			child->m_view.Invalidate();
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
					child->m_view.setLine(dlg.m_lineNum);
				}
				else if (child)
				{
					if (!m_wndScriptBar.IsWindowVisible())
					{
						visible = m_wndScriptBar.GetStyle() & WS_VISIBLE ? true : false;
						child = (CChildFrame *) MDIGetActive();
						if (child && !visible && !m_bSDI)
							m_wndScriptBar.SelectControl(child->getSelectedForm());
						ShowControlBar(&m_wndScriptBar, !visible, FALSE);
					}
					int lineNum = dlg.m_lineNum + 1, idx;
					m_wndScriptBar.WriteScript();
					m_wndScriptBar.getAllScript(NULL, &lineNum, &idx);
					m_wndScriptBar.setLinePos(idx / 13, abs(idx % 13) + 1, lineNum - 1);
				}
			}
		}
		break;

	case ID_VIEW_ALLSELECT:
		{
			child = (CChildFrame *) MDIGetActive();
			if (child->m_pFormItem->getFormCount())
			{
				child->m_view.m_selF = 0;
				if (child->m_pFormItem->getFormCount() > 1)
				{
					int ii = 0;
					for (ii = 0; ii < child->m_pFormItem->getFormCount(); ii++)
						child->m_pFormItem->getFormPoint(ii)->block = true;
					child->m_block->count = ii;
					child->m_view.m_stdF = 0;
				}
				child->m_view.Invalidate();
				SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
			}
		}
		break;

	case ID_APP_SAVETOOLBAR:
		{
			CString sToolbar, sSection = "POSITION";
			CRect rc;
			GetWindowRect(&rc);
			if (m_bSDI)
				sSection = "SDI_POSITION";
			sToolbar.Format("%d|%d|%d|%d", rc.left, rc.top, rc.right, rc.bottom);
			WritePrivateProfileString(sSection, "MAINFRAME", sToolbar, m_currentPath + "\\workshop.ini");

			for (int ii = 0; ii < 10; ii++)
			{
				sToolbar.Format("TOOLBAR%d", ii);
				WritePrivateProfileString(sSection, sToolbar, GetToolBarInfo(ii), m_currentPath + "\\workshop.ini");
			}
		}
		break;

	case ID_APP_DEFAULTTOOLBAR:
		{
			CString sToolbar, sSection = "POSITION";
			if (m_bSDI)
				sSection = "SDI_POSITION";
			WritePrivateProfileString(sSection, "MAINFRAME", "", m_currentPath + "\\workshop.ini");

			for (int ii = 0; ii < 10; ii++)
			{
				sToolbar.Format("TOOLBAR%d", ii);
				WritePrivateProfileString(sSection, sToolbar, "", m_currentPath + "\\workshop.ini");
			}
		}
		break;

	default:
		break;
	}
}

void CMainFrame::OnDrawCommand(UINT cmdID)
{
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	if (cmdID == ID_TOOL_POINTER)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	if (child)
		child->DrawForm(cmdID);
}

void CMainFrame::OnAlignCommand(UINT cmdID)
{
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	child->AlignForms(cmdID);
}

void CMainFrame::OnExCommand(UINT cmdID)
{
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	int	edit = child->EditForm(cmdID);
	setExChecked(edit);
}

void CMainFrame::RemoveChild(int key)
{
	CChildFrame*	child;
	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (child == (CChildFrame *) NULL)
			continue;

		if (child->m_myKey == key)
		{
			m_wndSelectionBar.RemoveMap(&child->m_mapH);
			m_children.RemoveAt(ii);
			
			if (m_children.GetSize() == 0)	//	no child
			{
				m_wndPropertiesBar.ResetCombo(2);
				m_wndPropertiesBar.SelectControl(-1);
				m_wndScriptBar.ResetCombo(2);
			}
			PostMessage(WM_USER, ID_USR_RECALCMAINSIZE);
			return;
		}
	}
}

BOOL CMainFrame::PressButton(UINT id)
{
	CToolBarCtrl&	bar = m_drawTB.GetToolBarCtrl();
	for (UINT ii = ID_TOOL_POINTER; ii <= ID_TOOL_CONTROL; ii++)
		bar.CheckButton(ii, FALSE);
	if (id != ID_TOOL_POINTER)
	{
		m_wndPropertiesBar.killFocus();
		CChildFrame* child = (CChildFrame *)MDIGetActive();
		child->killFocus();
	}

	return bar.CheckButton(id);
}

void CMainFrame::setExChecked(int edit)
{
	CMenu*	mainM = GetMenu();

	mainM->CheckMenuItem(ID_EX_FORMAT, MF_BYCOMMAND|MF_UNCHECKED);
	mainM->CheckMenuItem(ID_EX_RGB,    MF_BYCOMMAND|MF_UNCHECKED);


	switch (edit)
	{
	case IE_EDIT:
		mainM->CheckMenuItem(ID_EX_FORMAT, MF_BYCOMMAND|MF_CHECKED);	break;
	case IE_CORGB:
	case IE_CORGB2:
		mainM->CheckMenuItem(ID_EX_RGB,    MF_BYCOMMAND|MF_CHECKED);	break;
	case IE_NOP:
	default:
		break;
	}
}

void CMainFrame::createNewChild(int mapK)
{
	CRect	rect, rc;
	CString	tmps;
	CChildFrame*	child;
	int	fontpt;
	CFrameWnd*	pRealChild;

	tmps = _T("Untitled");
	if (mapK == MK_NORM)
		rect.SetRect(0, 0, 700, 450);
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
	if (m_bSDI)
		rc = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	else
		NegotiateBorderSpace(1, &rc);
	rc.OffsetRect(-rc.left, -rc.top);
	if (rect.right > rc.right || rect.bottom > rc.bottom)
	{
		rect.OffsetRect(-m_childPnt.x, -m_childPnt.y);
		m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), 
			-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	}
	else
		rect.OffsetRect(-m_childPnt.x, -m_childPnt.y);
	
	m_childPnt.x += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	m_childPnt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	
	if (m_bSDI)
	{
		CRect mainRc, controlRc;
		GetWindowRect(&mainRc);
		if (mainRc.bottom < GetSystemMetrics(SM_CYSCREEN) / 3 && m_childPnt.y < mainRc.bottom)
			m_childPnt.y = mainRc.bottom;
		if (m_wndWorkBar.IsWindowVisible())
		{
			m_wndWorkBar.GetWindowRect(&controlRc);
			controlRc.InflateRect(30, mainRc.Height(), 0 , 0);
			if (controlRc.PtInRect(m_childPnt) && 
				controlRc.right < GetSystemMetrics(SM_CXSCREEN) / 3 && m_childPnt.x < controlRc.right)
				m_childPnt.x = controlRc.right + 2;
		}

		if (m_wndPropertiesBar.IsWindowVisible())
		{
			m_wndPropertiesBar.GetWindowRect(&controlRc);
			controlRc.InflateRect(30, mainRc.Height(), 0 , 0);
			if (controlRc.PtInRect(m_childPnt) && 
				controlRc.right < GetSystemMetrics(SM_CXSCREEN) / 3 && m_childPnt.x < controlRc.right)
				m_childPnt.x = controlRc.right + 2;
		}
	}

	rect.OffsetRect(m_childPnt);

	fontpt = 9;

	child = new CChildFrame(this, m_draw, m_curDir, &m_block);
	child->m_pDisplayMode = &m_nDisplayMode;
	child->m_bSDI = m_bSDI;
	child->Create(NULL, tmps, WS_SYSMENU|WS_CHILD|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, rect, this);
	if (m_bSDI)
		pRealChild = (CFrameWnd*)child->m_pPopFrame;
	else
		pRealChild = (CFrameWnd*)child;
	
	pRealChild->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);

	if (mapK == MK_NORM)
		pRealChild->SetIcon(AfxGetApp()->LoadIcon(IDI_MAP), FALSE);
	else if (mapK == MK_TEMPLATE)
		pRealChild->SetIcon(AfxGetApp()->LoadIcon(IDI_TEMPLATE), FALSE);
	else 
		pRealChild->SetIcon(AfxGetApp()->LoadIcon(IDI_PROCEDURES), FALSE);

	child->SetNew(true);
	child->SetPalette(m_palette);
	child->m_mapH.width  = rect.Width();
	child->m_mapH.height = rect.Height();
	child->m_mapH.mapK = mapK;
	strcpy_s(child->m_mapH.caption, tmps);
	child->SetVisible(m_showFORM);
	m_children.Add(child);
	child->m_myKey = (long)child;

	CRect	cRc, wRc;
	pRealChild->GetClientRect(&cRc);
	pRealChild->GetWindowRect(&wRc);

        m_delta.cx = wRc.Width() - cRc.Width() + GetSystemMetrics(SM_CXBORDER)*4;
        m_delta.cy = wRc.Height() - cRc.Height() + GetSystemMetrics(SM_CYBORDER)*4;

	if (m_bSDI)
		pRealChild->SetWindowPos(NULL, m_childPnt.x, m_childPnt.y, rect.Width() + m_delta.cx, rect.Height() + m_delta.cy, SWP_NOZORDER|SWP_SHOWWINDOW);
	else
		pRealChild->SetWindowPos(NULL, 0, 0, rect.Width()+m_delta.cx, rect.Height()+m_delta.cy, SWP_NOMOVE|SWP_SHOWWINDOW);

	child->SetWindowText(tmps);
	child->SetFontSize(atoi(m_sDefFontPnt), FS_NORM, m_sDefFontName);
	child->SetNew(false);
	child->m_view.SendMessage(WM_SIZE, 0, MAKELPARAM(rect.Width(), rect.Height()));
	child->MDIActivate();

	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	SendMessage(WM_USER, ID_USR_REDRAWCTRL, -1);
	SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	SendMessage(WM_USER, ID_USR_SETCAPTION);
}

void CMainFrame::openLocalFILE()
{
	CFileDialog fDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, "All Files (*.*)|*.*||");
	fDlg.m_ofn.lpstrInitialDir = m_openDir;
	if (fDlg.DoModal() != IDOK)
		return;

	fileOpen(fDlg.GetPathName());
}

CChildFrame* CMainFrame::openMAP(CString fileN)
{
	int	totL, tokcnt;
	int	formN, repN, grN;
	WORD	type;
	CString tmps;
	CFrameWnd	*pRealChild;

	totL = wccLoadSource((char *)fileN.operator LPCTSTR(), (CWnd *) NULL);
	if (totL == -1)
		return (CChildFrame *) NULL;

	formN = grN = 0; repN = -1;
	CChildFrame* child = new CChildFrame(this, m_draw, m_curDir, &m_block);
	child->m_pDisplayMode = &m_nDisplayMode;
	child->m_bSDI = m_bSDI;
	child->Create(NULL, NULL, WS_SYSMENU|WS_CHILD|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this);
	if (m_bSDI)
		pRealChild = (CFrameWnd*)child->m_pPopFrame;
	else
		pRealChild = (CFrameWnd*)child;
	pRealChild->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	child->SetVisible(m_showFORM);
	child->SetFontSize(atoi(m_sDefFontPnt), FS_NORM, m_sDefFontName);
	child->m_myKey = (long)child;

	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_pToken[ii].Clear();

	load(&child->m_mapH, m_curDir);
	for (int ii = 0; ii < totL; ii++)
	{
		tokcnt = wccParseToken(m_pToken, ii, &type);
		switch (type)
		{
		case TYPE_GLOBAL:
			buildHeader(m_pToken, tokcnt);
			break;
		case TYPE_FORM:
			buildForm(m_pToken, tokcnt, formN);
			break;
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
		default:
			break;
		}

		for (int jj = 0; jj < tokcnt; jj++)
			m_pToken[jj].Clear();
	}
	buildTab();

	child->changeKind();

	if (IsInEditing(child->m_mapH.mapN, child->m_mapH.mapK))
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
	child->m_mapH.onTrigger = false;
	child->m_mapH.onDeclaration = false;
	child->m_mapH.onInDeclaration = false;

	tokcnt = 0;
	CString sBuf = "";
	for (int ii = 0; ii < totL; ii++)
	{
		sBuf = wccParseResource(ii, &type);
		switch (type)
		{
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
			tokcnt++;
			buildResource((LPSTR)sBuf.operator LPCTSTR(), ii, formN); 
		default:	break;
		}
	}
	if (tokcnt > 0)
	{
		sBuf = "}";
		buildResource((LPSTR)sBuf.operator LPCTSTR(), totL, formN);
	}

	child->m_mapH.formN    = formN;
	child->m_mapH.source   = true;
	child->m_mapH.modified = false;

	child->SetNew(true);
	child->SetPalette(m_palette);

	if (child->m_mapH.width < 100)
	{
		pRealChild->ModifyStyle(WS_CAPTION, NULL, SWP_DRAWFRAME);
	}

	CRect	cRc,  wRc, rc;
	pRealChild->GetClientRect(&cRc);
	pRealChild->GetWindowRect(&wRc);

	m_delta.cx = wRc.Width()  - cRc.Width() + (child->m_mapH.width >= 100 ? GetSystemMetrics(SM_CXBORDER)*4 : 0);
	m_delta.cy = wRc.Height() - cRc.Height() + (child->m_mapH.height >= 100 ? GetSystemMetrics(SM_CYBORDER)*4 : 0);

	if (child->m_mapH.mapK == MK_TEMPLATE)
	{
		CString	tmps = _T("Template - ") + CString(child->m_mapH.mapdesc);
		child->SetWindowText(tmps);
	}
 	else
		child->SetWindowText(child->m_mapH.caption);

	cRc.OffsetRect(m_childPnt);
	if (m_bSDI)
		rc = CRect(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	else
		NegotiateBorderSpace(1, &rc);
	rc.OffsetRect(-rc.left, -rc.top);

	if (cRc.right > rc.right || cRc.bottom > rc.bottom)
	{
		cRc.OffsetRect(-m_childPnt.x, -m_childPnt.y);
		m_childPnt = CPoint(-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME), 
			-GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYFRAME));
	}

	m_childPnt.x += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);
	m_childPnt.y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME);

	if (m_bSDI)
	{
		CRect mainRc, controlRc;
		GetWindowRect(&mainRc);
		if (mainRc.bottom < GetSystemMetrics(SM_CYSCREEN) / 3 && m_childPnt.y < mainRc.bottom)
			m_childPnt.y = mainRc.bottom;
		if (m_wndWorkBar.IsWindowVisible())
		{
			m_wndWorkBar.GetWindowRect(&controlRc);
			controlRc.InflateRect(30, mainRc.Height(), 0 , 0);
			if (controlRc.PtInRect(m_childPnt) && 
				controlRc.right < GetSystemMetrics(SM_CXSCREEN) / 3 && m_childPnt.x < controlRc.right)
				m_childPnt.x = controlRc.right + 2;
		}

		if (m_wndPropertiesBar.IsWindowVisible())
		{
			m_wndPropertiesBar.GetWindowRect(&controlRc);
			controlRc.InflateRect(30, mainRc.Height(), 0 , 0);
			if (controlRc.PtInRect(m_childPnt) && 
				controlRc.right < GetSystemMetrics(SM_CXSCREEN) / 3 && m_childPnt.x < controlRc.right)
				m_childPnt.x = controlRc.right + 2;
		}
	}

	pRealChild->SetWindowPos(NULL, m_childPnt.x, m_childPnt.y, 
		child->m_mapH.width + m_delta.cx, child->m_mapH.height + m_delta.cy, SWP_SHOWWINDOW);

	child->SetNew(false);

	child->m_view.SendMessage(WM_SIZE, 0, MAKELPARAM(child->m_mapH.width, child->m_mapH.height));
	child->LoadProcedures();
	child->MDIActivate();
	
	if (child->m_mapH.mapK == MK_NORM)
		pRealChild->SetIcon(AfxGetApp()->LoadIcon(IDI_MAP), FALSE);
	else if (child->m_mapH.mapK == MK_TEMPLATE)
		pRealChild->SetIcon(AfxGetApp()->LoadIcon(IDI_TEMPLATE), FALSE);
	else 
		pRealChild->SetIcon(AfxGetApp()->LoadIcon(IDI_PROCEDURES), FALSE);

	if (child->m_mapH.formN)
	{
		child->m_view.m_selF = 0;
		child->m_view.sortIoOrder();
		child->m_view.sortTabOrder();
		child->m_view.m_selF = -1;
	}

	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);	// Properties & script reload
	SendMessage(WM_USER, ID_USR_SETMAPNAME);
	
	return (CChildFrame *) child;
}

void CMainFrame::saveLocalMAP()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (!child->m_mapH.pathsetted)
		saveLocalPath();
	else
		saveMAP(child);
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
	strcpy_s(child->m_mapH.fullN, path);
	delete savedlg;

	idir = GetFileAttributes(path);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!CreateDirectory(path, NULL))
		{
			MessageBox("Can't read the directory.", "Error", MB_OK | MB_ICONSTOP);
			strcpy_s(child->m_mapH.fullN, old);
			return;
		}
	}

	child->m_mapH.pathsetted = true;
//	child->m_mapH.compile    = false;

	saveMAP(child);
}

void CMainFrame::saveMAP(CChildFrame* child)
{
	CString	dir = "", tdir;

	generateMAP(false, child);
	if (!child->m_mapH.source)
		return;

	dir  = child->m_mapH.fullN;

	dir += "\\";
	dir += child->m_mapH.mapN;
	tdir = _T("");
	wccSetTempPath(tdir, child->m_mapH.mapN);
	CopyFile(tdir, dir, false);
}

void CMainFrame::closeChild()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();

  	if (child)
		child->PostMessage(WM_CLOSE);
}

int CMainFrame::generateMAP(bool alarm, CChildFrame *child)
{
	if (!child)
		child = (CChildFrame *) MDIGetActive();
	
	if (child->m_mapH.mapK == MK_PROCEDURES)
		child->SaveProcedures();
	else
	{
		child->m_view.sortIoOrder();
		child->m_view.sortTabOrder();
	}

	m_wndScriptBar.WriteScript();
	int rc = generateSource(alarm, &child->m_mapH);
	return rc;
}

void CMainFrame::compileMAP(int flag)
{
	int	totL, rc;
	WORD	type;
	CString	srcP, objP, nickP;

	CChildFrame* child;
	AfxGetApp()->DoWaitCursor(1);

	for (int ii = 0; ii < (flag?m_children.GetSize():1); ii++)
	{
		if (flag)
			child = m_children.GetAt(ii);
		else
			child = (CChildFrame *) MDIGetActive();
		
		if (child->m_mapH.mapK == MK_TEMPLATE)
			continue;
		
		/*child->m_view.sortIoOrder();
		child->m_view.sortTabOrder();*/

		switch (generateMAP(false, child))
		{
		case 0:
			child->m_mapH.source = true;	break;
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
		
		rc = wccAllocateMemory(totL);
		if (rc == -1)
		{
			wccFreeMemory();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return;
		}
			
		rc = wccAnalyzeColumn(totL);
		if (rc == -1)
		{
			wccFreeMemory();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return;
		}
		

		for (int jj = 0; jj < MAX_TOKEN_N; jj++)
			m_pToken[jj].Clear();
		
		rc = 0;
		for (int jj = 0; jj < totL; jj++)
		{
			rc += wccBuildToken(m_pToken, jj, &type);
			if (rc < 0)
				rc = 0;
		}
		wccSetDominoForm();
		wccLastCompileWork();
		
		rc = wccBuildObject(objP);
		if (rc == -1)
		{
			wccFreeMemory();
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			continue;
		}
		
		wccFreeMemory();
		child->m_mapH.compile = true;
	}
	AfxGetApp()->DoWaitCursor(-1);
}

void CMainFrame::applyAXIS(int flag)
{
	CString	tmps, path;

	CChildFrame* child;
	for (int ii = 0; ii < (flag?m_children.GetSize():1); ii++)
	{
		if (flag)
			child = m_children.GetAt(ii);
		else
			child = (CChildFrame *) MDIGetActive();
		if (child->m_mapH.mapK == MK_TEMPLATE)
			break;

		DWORD	idir = GetFileAttributes(m_root);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || !child->m_mapH.compile)
			return;
		
		path.Format("%s\\%s", m_root, MAPDIR);
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		tmps = CString(child->m_mapH.mapN, L_SGID);
		path += "\\";
		path += tmps;
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		tmps = CString(child->m_mapH.mapN, L_TGID);
		path += "\\";
		path += tmps;
		idir = GetFileAttributes(path);
		if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
			CreateDirectory(path, NULL);
		
		tmps = _T("");
		wccSetTempPath(tmps, child->m_mapH.mapN, false);
		path += "\\";
		path += child->m_mapH.mapN;
		CopyFile(tmps, path, false);
	}
}

void CMainFrame::reloadMAP()
{
	CString	path, tmps;
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	tmps = _T("");
	wccSetTempPath(tmps, child->m_mapH.mapN);
	if (child->m_mapH.pathsetted)
		path = child->m_mapH.fullN;

	HANDLE hFile = CreateFile(tmps, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	CloseHandle(hFile);

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
	if (child->m_mapH.onTrigger)
		DeleteFile(child->m_mapH.onTriggerN);
	if (child->m_mapH.onDeclaration)
		DeleteFile(child->m_mapH.onDeclarationN);
	if (child->m_mapH.onInDeclaration)
		DeleteFile(child->m_mapH.onInDeclarationN);

    	child->MDIDestroy();
	child = openMAP(tmps);
	if (!path.IsEmpty())
	{
		child->m_mapH.pathsetted = true;
		strcpy_s(child->m_mapH.fullN, path);
	}
}

void CMainFrame::showFORM() 
{
	CMenu*	mainM = GetMenu();

	if (mainM->GetMenuState(ID_HIDE_FORM, MF_CHECKED) & MF_CHECKED)
	{
		m_showFORM = false;
		mainM->CheckMenuItem(ID_HIDE_FORM, MF_BYCOMMAND|MF_UNCHECKED);
	}
	else
	{
		m_showFORM = true;
		mainM->CheckMenuItem(ID_HIDE_FORM, MF_BYCOMMAND|MF_CHECKED);
	}

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
	char	path[128];

	titles = _T("Source Edit Application");
	inits  = (char*)SendMessage(WM_USER, ID_USR_GETSRCVIEWER, 0);
	GetCurrentDirectory(sizeof(path), path);
	sCurrentDir = path;
	if (inits.IsEmpty())
	{
		GetWindowsDirectory(path, sizeof(path));
		
		inits = path;
		exes  = "notepad.exe";
	}
	else
	{
		offs = inits.ReverseFind('\\');
		if (offs != -1)
		{
			exes  = inits.Mid(offs+1);
			inits = inits.Left(offs);
		}
		else
			exes = _T("");
	}

	CFileDialog fDlg(TRUE, NULL, exes, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
			"Application|*.exe|All files|*.*||");
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

	tmps = _T("");
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	wccSetTempPath(tmps, child->m_mapH.mapN);
	HANDLE hFile = CreateFile(tmps, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		generateMAP();
	else
		CloseHandle(hFile);

	cmdN.Format(" %s", tmps);

	char	pathN[256];
	CWinApp* app = AfxGetApp();
	tmps = (char*)SendMessage(WM_USER, ID_USR_GETSRCVIEWER, 0);
	if (tmps.IsEmpty())
	{
		GetWindowsDirectory(pathN, sizeof(pathN));
		strcat_s(pathN, "\\notepad.exe");
	}
	else
		strcpy_s(pathN, tmps);

	tmps = pathN;
	tmps += cmdN;
	WinExec(tmps, SW_SHOWNORMAL);
}

void CMainFrame::setServerIP()
{
	CAddrDlg	addrDlg;
	addrDlg.m_currentPath = m_currentPath;
	addrDlg.DoModal();
}

void CMainFrame::saveServerMAP()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.source)
	{
		CUpDlg* upDlg = new CUpDlg(this, &child->m_mapH);
		upDlg->DoModal();
		delete upDlg;
	}
	else
		MessageBox("Must create Source before Upload.", "Message", MB_OK);
}

void CMainFrame::saveServerTmpl()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();

	if (generateMAP(false) == 0)
		child->m_mapH.source = true;

	if (child && child->m_mapH.source)
	{
		CUpTmplDlg* upTmplDlg = new CUpTmplDlg(this, &child->m_mapH);
		upTmplDlg->DoModal();
		delete upTmplDlg;
	}
	else
		MessageBox("Must create Source before Upload.", "Message", MB_OK);
}

void CMainFrame::batchCompile()
{
	CBatch*	batch = new CBatch(this, m_curDir);
	batch->DoModal();
	delete batch;
}

void CMainFrame::OnClose() 
{
	CChildFrame* child;

	for ( ; m_children.GetSize(); )
	{
		child = m_children.GetAt(0);
		if (IsWindow(child->m_hWnd))
		{
			if (!child->DeleteFiles())
				return;
			else
			{
				child->FreePopFrame();
				child->DestroyWindow();
			}
		}
	}

	if (m_pWizardCtrl)
	{
		BOOL	retvalue = false;
		m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), loginSHOP, 0, -1);
	}

	CString sTmp;
	sTmp.Format("%d", m_wndWorkBar.m_TreeServer.m_nTwoDepth);
	WritePrivateProfileString(ENVIRONMENT, "SVRDEPTH", sTmp, m_currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_wndWorkBar.m_TreeTemplate.m_nTwoDepth);
	WritePrivateProfileString(ENVIRONMENT, "TMPDEPTH", sTmp, m_currentPath + "\\workshop.ini");

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();

	if ((m_status & AWS_SDRAG) || (m_status & AWS_TDRAG) || (m_status & AWS_MDRAG))
		m_wndWorkBar.DeleteDragImg();

	if (m_status & AWS_MDRAG)
	{
		m_wndWorkBar.m_TreeServer.OpenMap();
	}
	else
	{
		short x = (WORD)point.x;
		short y = (WORD)point.y;
		
		if (x < 0)
			point.x = 0;
		else
			x = 0;
		
		if (y < 0)
			point.y = 0;
		else
			y = 0;
		
		ClientToScreen(&point);
		point.x += x;
		point.y += y;
		
		CRect	rect;
		CChildFrame* child = (CChildFrame *)MDIGetActive();
		if (child != (CChildFrame *) NULL)
		{
			child->SetFocus();
			child->m_view.GetWindowRect(&rect);
			if (rect.PtInRect(point))
				child->OnDrop(point);
		}
	}

	m_status &= ~AWS_SDRAG;
	m_status &= ~AWS_TDRAG;
	m_status &= ~AWS_MDRAG;

	CMDIFrameWnd::OnLButtonUp(nFlags, point);
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint	pt(point);
	ClientToScreen(&pt);

	if (((m_status & AWS_SDRAG) || (m_status & AWS_TDRAG) || (m_status & AWS_MDRAG)) && m_wndWorkBar.m_pDragImg)
		m_wndWorkBar.m_pDragImg->DragMove(pt);
	
	CMDIFrameWnd::OnMouseMove(nFlags, point);
}

LRESULT CMainFrame::OnStatus(WPARAM wParam, LPARAM lParam)
{
	m_status = wParam;
	return 0;
}

LRESULT CMainFrame::OnOpenObject(WPARAM wParam, LPARAM lParam)
{
	CChildFrame* child;
	CString	fileN = (char *)lParam;

	if (m_children.GetSize() >= MAX_MAP)
		return 0;

	AfxGetApp()->DoWaitCursor(1);
	child = openMAP(fileN);
	if (child != (CChildFrame *) NULL)
		child->m_mapH.pathsetted = false;
	AfxGetApp()->DoWaitCursor(-1);
	return 0;
}

void CMainFrame::fileOpen(CString path, bool bDefault)
{
	int	offs;
	CString	mapN, kind, tmps;

	offs = path.ReverseFind('\\');
	if (offs == -1)
		return;	

	mapN = path.Mid(offs+1);
	m_openDir = path.Left(offs);

	tmps = _T("");
	CChildFrame* child;

	AfxGetApp()->DoWaitCursor(1);
	if (dropInChild())
	{
		wccSetTempPath(tmps, "template");
		CopyFile(path, tmps, FALSE);
		child = (CChildFrame*)MDIGetActive();
		child->m_view.ScreenToClient(&m_dropPnt);
		loadTemplate(m_dropPnt);
		m_dropPnt = CPoint(-1, -1);
		child->m_mapH.modified   = true;
	}
	else
	{
		wccSetTempPath(tmps, (char *)mapN.operator LPCTSTR());
		CopyFile(path, tmps, FALSE);
		child = openMAP(tmps);
		if (child != (CChildFrame *) NULL)
		{
			tmps = m_openDir.Left(offs);
			if (bDefault)
				child->m_mapH.mapN[0] = '\0';
			else
				strcpy_s(child->m_mapH.fullN, tmps);
			child->m_mapH.pathsetted = bDefault?false:true;
			child->m_mapH.modified   = false;
			PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
			if (bDefault)
				PostMessage(WM_USER, ID_USR_SETMAPNAME);
		}
	}
	m_dropPnt = CPoint(-1, -1);

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

CString CMainFrame::getControlName(int kind, int type)
{
	switch(kind)
	{
	case FM_SYS:	return "SYSTEM";
	case FM_LABEL:	return "LABEL";
	case FM_BOX:	return "BOX";
	case FM_GROUP:	return "GROUP";
	case FM_PANEL:	return "PANEL";
	case FM_BUTTON:
		if (type == BTN_RADIO)
			return "RADIO";
		else if (type == BTN_CHECK)
			return "CHECK";
		else
			return "BUTTON";
	case FM_EDIT:	return "EDIT";
	case FM_COMBO:	return "COMBO";
	case FM_OUT:	return "OUTPUT";
	case FM_MEMO:	return "MEMO";
	case FM_GRID:	return "GRID";
	case FM_TABLE:	return "TABLE";
	case FM_TREE:	return "TREE";
	case FM_AVI:	return "AVI";
	case FM_GRAPH:	return "GRAPH";
	case FM_OBJECT:	return "OBJECT";
	case FM_TAB:	return "TAB";
	case FM_UTAB:	return "UTAB";
	case FM_SHEET:	return "SHEET";
	case FM_BROWSER:	return "BROWSER";
	case FM_CONTROL:	return "CONTROL";
	}
	return "";
}

void CMainFrame::loadTemplate(CPoint pt)
{
	CString tmp, symbolName = "\t";
	CMapStringToString	changedSymbolMap;

	wccSetTempPath(tmp, "template");
	CChildFrame* child = (CChildFrame *)MDIGetActive();
	m_dropPnt = pt;

	child->m_view.ClientToScreen(&m_dropPnt);
	if (!dropInChild())
		return;

	int totL = wccLoadSource(tmp, (CWnd *) NULL), tokcnt, formN, prFormN, groupCount = 0;
	struct _formR* nGroup = NULL;
	WORD type;

	int tabN, pageN = 0;
	if (!child->m_view.isInTab(pt, tabN, pageN))
		tabN = -1;
	
	load(&child->m_mapH, tmp);
	prFormN = formN = child->m_view.m_pFormItem->getFormCount();
	for (int kk = 0; kk < child->m_view.m_pFormItem->getFormCount(); kk++)
	{
		symbolName += child->m_view.m_pFormItem->getFormStr(kk, "name");
		symbolName += '\t';
	}

	for (int ii = 0; ii < totL; ii++)
	{
		tokcnt = wccParseToken(m_pToken, ii, &type);
		switch(type)
		{
		case TYPE_FORM:
			{
				int beforeFormN = formN;
				buildForm(m_pToken, tokcnt, formN);
				if (prFormN == formN - 1 && nGroup == NULL)
					nGroup = child->m_view.m_pFormItem->getFormPoint(prFormN);
				if (beforeFormN < formN)
				{
					CString sSymbol = child->m_view.m_pFormItem->getFormStr(beforeFormN, "name");
					sSymbol.MakeUpper();
					symbolName.MakeUpper();
					if (symbolName.Find("\t" + sSymbol + "\t") != -1)
					{
						CString sSymbolName = child->m_view.getInitSymbol(symbolName);
						changedSymbolMap.SetAt(sSymbol, sSymbolName);
						child->m_view.m_pFormItem->setFormStr(beforeFormN, "name", sSymbolName);
					}
					
					symbolName += child->m_view.m_pFormItem->getFormStr(beforeFormN, "name");
					symbolName += '\t';
				}
			}
			break;
		}
		for (int jj = 0; jj < tokcnt; jj++)
			m_pToken[jj].Clear();
	}
	buildTab(prFormN);

	for (int ii = child->m_view.m_pFormItem->getIdxOfPtr(nGroup); ii < formN && ii >= 0; ii++)
		if (!child->m_view.m_pFormItem->getFormPoint(ii)->tabN)
			groupCount++;
	for (int ii = child->m_view.m_pFormItem->getIdxOfPtr(nGroup); ii < formN && ii >= 0; ii++)
	{
		struct _formR *formR = child->m_view.m_pFormItem->getFormPoint(ii);
		formR->left += pt.x;
		formR->right += pt.x;
		formR->top += pt.y;
		formR->bottom += pt.y;
		
		if (!formR->tabN && tabN >= 0)
		{
			formR->tabN = child->m_view.m_pFormItem->getFormPoint(tabN);
			formR->pageN = pageN;
		}
		
		if (tabN == -1)
		{
			if (groupCount > 1 && !formR->tabN)
				formR->group = nGroup;
		}
	}
	child->m_view.m_selF = child->m_view.m_pFormItem->getIdxOfPtr(nGroup);

	tokcnt = 0;
	CString sBuf = "";
	for (int ii = 0; ii < totL; ii++)
	{
		sBuf = wccParseResource(ii, &type);
		switch (type)
		{
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
			{
				POSITION pos = changedSymbolMap.GetStartPosition();
				CString sUpperBuf;
				sBuf = " " + sBuf;
				sUpperBuf = sBuf;
				sUpperBuf.MakeUpper();
				while(pos)
				{
					CString rKey, rValue;
					changedSymbolMap.GetNextAssoc(pos, rKey, rValue);
					int nPos = -1;

					// script key change
					CString sScriptKey = "_AW_SCRIPT_AW_(" + rKey + ")", sToScriptKey = " _AW_SCRIPT_AW_(" + rValue + ")";
					sScriptKey.MakeUpper();
					sToScriptKey.MakeUpper();
					if (sUpperBuf.Find(sScriptKey) >= 0)
						sBuf = sToScriptKey;
					rKey.MakeUpper();
					
					do
					{
						nPos = sUpperBuf.Find(rKey, nPos + 1);
						if (nPos <= 0)
							break;
						if (!isalnum(sUpperBuf.GetAt(nPos - 1)))
						{
							sBuf.Replace(sBuf.GetAt(nPos - 1) + sBuf.Mid(nPos, rKey.GetLength()) + ".", 
								     sBuf.GetAt(nPos - 1) + rValue + ".");
							sUpperBuf = sBuf;
							sUpperBuf.MakeUpper();

						}
					}while(nPos >= 0);
				}
				sBuf = sBuf.Mid(1);
				
				tokcnt++;
				buildResource((LPSTR)sBuf.operator LPCTSTR(), ii, formN); 
			}
		default:	break;
		}
	}
	if (tokcnt > 0)
	{
		sBuf = "}";
		buildResource((LPSTR)sBuf.operator LPCTSTR(), totL, formN);
	}

	changedSymbolMap.RemoveAll();
	child->m_view.sortIoOrder();
	child->m_view.sortTabOrder();
	child->m_view.m_bRedraw = true;

	PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	child->m_view.Invalidate();
}

void CMainFrame::OnToolbarDropDown(NMHDR* pdr, LRESULT *plr)
{
	NMTOOLBAR *pnmtb = (NMTOOLBAR *)pdr;
	CWnd *pWnd;
	BCMenu	popup;
	CRect rc;
	CChildFrame *child;
	if (!m_defaultMenu.GetSafeHmenu())
		m_defaultMenu.Attach(m_hMenuDefault);

	// Switch on button command id's.
	switch (pnmtb->iItem)
	{
	case ID_APP_GOBOOKMARK:
		{
			pWnd = (CWnd*)&m_mainTB;
			child = (CChildFrame *) MDIGetActive();
			if (!child)
				return;
		}
		break;
	case ID_APP_NEW:
		pWnd = (CWnd*)&m_mainTB;
		break;
	default:
		return;
	}
	
	popup.CreatePopupMenu();
	switch(pnmtb->iItem)
	{
	case ID_APP_GOBOOKMARK:
		{
			for (int ii = 1; ii <= child->getBMCount() && ii < 50; ii++)
			{
				CString temp;
				temp.Format("%d", ii);
				popup.AppendMenu(MF_STRING, ID_DROPDOWNBASE + ii - 1, temp);
			}
		}
		break;
	case ID_APP_NEW:
		popup.AppendMenu(MF_STRING, ID_APP_NEW, "Map");
		popup.AppendMenu(MF_STRING, ID_APP_NEWTMPL, "Template");
		popup.AppendMenu(MF_STRING, ID_APP_NEWSC, "Procedures");
		break;
	}
	CImageList temp;
	temp.Create(17,17,ILC_COLORDDB|ILC_MASK,1,1);
	temp.Add((HICON)AfxGetApp()->LoadIcon(IDI_MAP));
	temp.Add((HICON)AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	temp.Add((HICON)AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	m_defaultMenu.ModifyODMenu(NULL, ID_APP_NEW, &temp, 0);
	m_defaultMenu.ModifyODMenu(NULL, ID_APP_NEWTMPL, &temp, 1);
	m_defaultMenu.ModifyODMenu(NULL, ID_APP_NEWSC, &temp, 2);
	temp.DeleteImageList();

	pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
	pWnd->ClientToScreen(&rc);
	
	popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		rc.left, rc.bottom, this, &rc);
	
}

void CMainFrame::OnDropdownCommand(UINT cmdID)
{
	CChildFrame *child = (CChildFrame *) MDIGetActive();
	if (!child)
		return;
	changeMap(child->getBookmarkSrc(cmdID - ID_DROPDOWNBASE));
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				OnDrawCommand(ID_TOOL_POINTER);
				CChildFrame *child = (CChildFrame *) MDIGetActive();
				if (child)
				{
					if (child->m_view.m_pPreView)
						child->m_view.Preview();
				}
			}
			break;
		case VK_F7:
			if (::GetKeyState(VK_SHIFT) & 0x8000)
			{
				compileMAP(1);
				applyAXIS(1);
			}
			else
			{
				CChildFrame *child = (CChildFrame *) MDIGetActive();
				if (child)
				{
					compileMAP();
					applyAXIS();
				}
			}
			break;
		/*case VK_F3:
			{
				CChildFrame*	child;
				child = (CChildFrame *) MDIGetActive();
				CRichEditCtrl *pRich = NULL;
				if (m_wndTraceBar.IsWindowVisible() && 
					GetFocus() && 
					GetFocus()->m_hWnd == m_wndTraceBar.m_resultEdit.m_hWnd)
					pRich = &m_wndTraceBar.m_resultEdit;
				if (!pRich && child)
				{
					if (child->m_mapH.mapK == MK_PROCEDURES)
						pRich = child->m_view.m_pSCEdit;
					else if (m_wndScriptBar.IsWindowVisible() && 
						m_wndScriptBar.m_editScript.IsWindowEnabled())
						pRich = &m_wndScriptBar.m_editScript;
				}
				if (pRich)
				{
					FINDTEXTEX ft;
					ft.chrg.cpMin = 0;
					ft.chrg.cpMax = -1;
					ft.lpstrText = (char*)SendMessage(WM_USER, ID_USR_CURRENTWORD, 0);
					
					if (::GetKeyState(VK_SHIFT) & 0x8000)
					{
						CHARRANGE beforeRange;
						pRich->GetSel(ft.chrg.cpMax, beforeRange.cpMax);
						beforeRange.cpMin = 0;	beforeRange.cpMax = 0;
						while(pRich->FindText(FR_WHOLEWORD, &ft) != -1)
						{
							ft.chrg.cpMin = ft.chrgText.cpMax;
							beforeRange = ft.chrgText;
						}
						if (beforeRange.cpMin != beforeRange.cpMax)
							pRich->SetSel(beforeRange);
						else
						{
							ft.chrg.cpMin = 0;
							ft.chrg.cpMax = -1;
							while(pRich->FindText(FR_WHOLEWORD, &ft) != -1)
							{
								ft.chrg.cpMin = ft.chrgText.cpMax;
								beforeRange = ft.chrgText;
							}
							if (beforeRange.cpMin != beforeRange.cpMax)
								pRich->SetSel(beforeRange);
						}
					}
					else
					{
						pRich->GetSel(ft.chrg.cpMin, ft.chrg.cpMin);
						if (pRich->FindText(FR_WHOLEWORD, &ft) != -1)
							pRich->SetSel(ft.chrgText);
						else
						{
							ft.chrg.cpMax = -1;
							ft.chrg.cpMin = 0;
							if (pRich->FindText(FR_WHOLEWORD, &ft) != -1)
								pRich->SetSel(ft.chrgText);
						}
					}
				}
			}
			break;*/
		}
		
		if (pMsg->wParam == VK_SPACE && (::GetKeyState(VK_SHIFT) & 0x8000) && (::GetKeyState(VK_CONTROL) & 0x8000))
		{
			CChildFrame *child = (CChildFrame *) MDIGetActive();
			if (child)
			{
				child->m_view.SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, 0);
				child->m_view.SendMessage(WM_LBUTTONUP, MK_LBUTTON, 0);
			}
		}
	}
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

CString CMainFrame::getErrString(int nCode)
{
	switch(nCode)
	{
	case 0:
		return "Duplicated Symbol.";
	case 1:
		return "Reserved keyword.";
	}
	if (nCode > 100)
		return (char*)nCode;
	return "";
}

bool CMainFrame::dropInChild()
{
	CChildFrame* child = (CChildFrame *) MDIGetActive();
	if (!child || child->m_view.m_pPreView)
		return false;

	CRect wRc;
	child->GetWindowRect(&wRc);
	if (wRc.PtInRect(m_dropPnt))
		return true;

	return false;
}

void CMainFrame::MakeDir(CString root)
{
	CFileFind finder;
	if (!finder.FindFile(root))
		CreateDirectory(root, NULL);
	
	CString exePath, tabPath;
	exePath.Format("%s\\exe", root);
	tabPath.Format("%s\\tab", root);
	
	if (finder.FindFile(exePath))
	{
		finder.FindNextFile();
		if (!finder.IsDirectory())
			CreateDirectory(exePath, NULL);
	}
	else
		CreateDirectory(exePath, NULL);
	
	if (finder.FindFile(tabPath))
	{
		finder.FindNextFile();
		if (!finder.IsDirectory())
			CreateDirectory(tabPath, NULL);
	}
	else
		CreateDirectory(tabPath, NULL);
	finder.Close();
}

int CMainFrame::initDir()
{
	CString tmps;
	char	tmpB[128];
	DWORD	idir;
	CString		fileN;
	WIN32_FIND_DATA	ffData;

	m_root = (char*)SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	idir = GetFileAttributes(m_root);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || m_root.IsEmpty())
	{
		idir = ::GetCurrentDirectory(sizeof(tmpB), tmpB);
		tmps = CString(tmpB, idir);
		if (tmps.GetLength() > 4 && !tmps.Right(4).CompareNoCase("\\exe"))
			tmps = tmps.Mid(0, tmps.GetLength() - 4);
		m_root = tmps;
		WritePrivateProfileString(ENVIRONMENT, ROOTDIR, tmps, m_currentPath + "\\workshop.ini");
	}
	
	MakeDir(m_root); // Directory Setting

	tmps = m_root + "\\" + RUNDIR;

	m_openDir = tmps;
	m_curDir  = tmps + "\\tmp";
	idir = GetFileAttributes(m_curDir);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!::CreateDirectory(m_curDir, NULL))
		{
			MessageBox("Can't read the directory.", "Error",
					MB_OK | MB_ICONSTOP);
			return -1;
		}
	}
	wccSetCurrentDirectory(m_curDir);

	fileN.Format("%s\\*.*", m_curDir);
	CString backFileN;
	backFileN.Format("%s\\backup", m_curDir);

	idir = GetFileAttributes(backFileN);
	if (!(idir & FILE_ATTRIBUTE_DIRECTORY) || idir == 0xffffffff)
	{
		if (!::CreateDirectory(backFileN, NULL))
		{
			MessageBox("Can't make the backup directory.", "Error",
					MB_OK | MB_ICONSTOP);
		}
	}

	int nTotal = 0, nCnt = 0;
	HANDLE	hFile = ::FindFirstFile(fileN, &ffData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (!(ffData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			strlen(ffData.cFileName) == 8)
			nTotal++;
		for (;;)
		{
			ZeroMemory(&ffData, sizeof(WIN32_FIND_DATA));
			if (!::FindNextFile(hFile, &ffData))
			{
				FindClose(hFile);
				break;
			}
			if (!(ffData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				strlen(ffData.cFileName) == 8)
				nTotal++;
		}
	}
	
	hFile = ::FindFirstFile(fileN, &ffData);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileN.Format("%s\\%s", m_curDir, ffData.cFileName);
		if (!(ffData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			strlen(ffData.cFileName) == 8)
		{
			backFileN.Format("%s\\backup\\%s", m_curDir, ffData.cFileName);
			SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(nCnt++ * 100 / nTotal, 4));
			CopyFile(fileN, backFileN, TRUE);
		}
		DeleteFile(fileN);

		for (;;)
		{
			ZeroMemory(&ffData, sizeof(WIN32_FIND_DATA));
			if (!::FindNextFile(hFile, &ffData))
			{
				FindClose(hFile);
				break;
			}

			fileN.Format("%s\\%s", m_curDir, ffData.cFileName);
			if (!(ffData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				strlen(ffData.cFileName) == 8)
			{
				backFileN.Format("%s\\backup\\%s", m_curDir, ffData.cFileName);
				SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(nCnt++ * 100 / nTotal, 4));
				CopyFile(fileN, backFileN, FALSE);
				m_backupSSMap.SetAt(ffData.cFileName, backFileN);
			}
			DeleteFile(fileN);
		}
	}
	return 1;
}

bool CMainFrame::IsInEditing(CString mapN, BYTE mapK)
{
	CString tmps = _T("");
	CChildFrame* child;
	for (int ii = 0; ii < m_children.GetSize(); ii++)
	{
		child = m_children.GetAt(ii);
		if (child == (CChildFrame *)NULL)
			continue;

		if (!mapN.CompareNoCase(child->m_mapH.mapN) && mapK == child->m_mapH.mapK)
		{
			tmps.Format("\n[%s] Editing same map now.\n\n", mapN);

			m_statusBar.SetPaneText(0, tmps);
			child->MDIActivate();
			return true;
		}
	}
	return false;
}

void CMainFrame::ReCreateMDITabs()
{
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.DestroyWindow();
	DWORD dwStyle = MT_TOP | MT_IMAGES;
	m_wndMDITabs.Create(this, dwStyle);
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	RecalcLayout();
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
	
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update(); // sync the mditabctrl with all views
}

void CMainFrame::changeMap(CString src)
{
	if (src.IsEmpty())
		return;

	int	totL, tokcnt, formN = 0;
	WORD	type;

	CChildFrame *child = (CChildFrame *) MDIGetActive();
	if (!child)
		return;

	totL = wccLoadSource("", (CWnd *) NULL, src);
	if (totL == -1)
		return ;

	child->m_pFormItem->RemoveAllForm();
	child->DeleteUndoMap();
	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_pToken[ii].Clear();

	load(&child->m_mapH, m_curDir);
	for (int ii = 0; ii < totL; ii++)
	{
		tokcnt = wccParseToken(m_pToken, ii, &type);
		switch (type)
		{
		case TYPE_GLOBAL:
			buildHeader(m_pToken, tokcnt);
			break;
		case TYPE_FORM:
			buildForm(m_pToken, tokcnt, formN);
			break;
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
		default:
			break;
		}

		for (int jj = 0; jj < tokcnt; jj++)
			m_pToken[jj].Clear();
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
	child->m_mapH.onTrigger = false;
	child->m_mapH.onDeclaration = false;
	child->m_mapH.onInDeclaration = false;

	tokcnt = 0;
	CString sBuf = "";
	for (int ii = 0; ii < totL; ii++)
	{
		sBuf = wccParseResource(ii, &type);
		switch (type)
		{
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
			tokcnt++;
			buildResource((LPSTR)sBuf.operator LPCTSTR(), ii, formN); 
		default:	break;
		}
	}
	if (tokcnt > 0)
	{
		sBuf = "}";
		buildResource((LPSTR)sBuf.operator LPCTSTR(), totL, formN);
	}

	child->m_view.sortIoOrder();
	child->m_view.sortTabOrder();
	child->m_view.m_selF = -1;
	PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	child->m_view.Invalidate();
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

void CMainFrame::OnUpdateEditFindReplace(CCmdUI* pCmdUI) 
{
	CChildFrame*	child;
	child = (CChildFrame *) MDIGetActive();
	if (child && child->m_mapH.mapK == MK_PROCEDURES)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::loadControlEvent()
{
	CString path, info = "", desc;
	char	twb[1024];
	int	helpIdx = 0, nItem = 0;
	path.Format("%s\\%s\\control.ini", m_root, TABDIR);
	m_controlEvent.RemoveAll();

	for (int ii = 1; ii < 100; ii++)
	{
		CString tmps, tmpstr = "", sym = "";

		tmps.Format("%02d", ii);
		GetPrivateProfileString("CONTROL", tmps, "", twb, sizeof(twb), path);
		desc = twb;
		tmps = Parser(desc, ";");
		sym = Parser(desc, ";");
		Parser(desc, ";");
		
		if (tmps.IsEmpty())
			break;
		m_controlEvent.SetAt(tmps, (void*)atoi(desc));
	}
}

CString CMainFrame::Parser(CString &srcstr, CString substr)
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

bool CMainFrame::RegisterCtrl(CString name, bool bMsg)
{
	return true;
	HINSTANCE	handle;
	CString		path;
	path.Format("%s\\exe\\%s", m_root, name);

	handle = LoadLibrary(path);
	if (handle)
	{
		FARPROC	lpDllEntryPoint;
		(FARPROC&)lpDllEntryPoint = GetProcAddress(handle,
			_T("DllRegisterServer"));
		if (lpDllEntryPoint != NULL)
			(*lpDllEntryPoint)();
		
		FreeLibrary(handle);
		return true;
	}
	else
	{
		if (bMsg)
			AfxMessageBox("There is not " + path + " file.");
	}
	return false;
}

bool CMainFrame::UnRegisterCtrl(CString name)
{
	HINSTANCE	handle = NULL;
	CString		path;
	path.Format("%s\\exe\\%s", m_root, name);
	if (!name.CompareNoCase("axwizard.ocx"))
		handle = LoadLibrary(path);

	if (handle)
	{
		FARPROC	lpDllEntryPoint;
		(FARPROC&)lpDllEntryPoint = GetProcAddress(handle,
			_T("DllUnregisterServer"));
		if (lpDllEntryPoint != NULL)
			(*lpDllEntryPoint)();
		
		FreeLibrary(handle);	
		return true;
	}
	else
		AfxMessageBox("Failed to unregister " + name + "!!");
	return false;
}


void CMainFrame::StartWorkshop()
{
	CString sTmp;
	long nScreenCX = GetSystemMetrics(SM_CXSCREEN), nScreenCY = GetSystemMetrics(SM_CYSCREEN);
	m_LoginDlg.ShowWindow(SW_HIDE);
	if (!m_bOffline)
		CSplashWnd::ShowSplashScreen(this);

	m_nDisplayMode = GetPrivateProfileInt(ENVIRONMENT, "DISPLAYMODE", 0, m_currentPath + "\\workshop.ini");
	m_bSDI = GetPrivateProfileInt(ENVIRONMENT, "FLOAT", 0, m_currentPath + "\\workshop.ini");
	if (m_bSDI)
	{
		ModifyStyle(/*WS_THICKFRAME |*/ WS_MAXIMIZEBOX, NULL);
		ModifyStyleEx(NULL, WS_EX_TOPMOST);
	}

	if (!m_statusBar.Create(this)
		|| !m_statusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}

	m_statusBar.SetPaneInfo(1, ID_INDICATOR_EXT, SBPS_NORMAL, 80);
	m_statusBar.SetPaneText(1, "");

	if (initDir() == -1)
		PostMessage(WM_CLOSE);

	sTmp.Format("%s\\%s\\%s", m_root, TABDIR, PALETTE);
	m_palette = new CAxisPalette((char *)sTmp.operator LPCTSTR());
	m_draw    = new CAxisDraw();

	int	cx, cy;
	cx = GetSystemMetrics(SM_CXMAXIMIZED) - GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
	cy = GetSystemMetrics(SM_CYMAXIMIZED) - GetSystemMetrics(SM_CYFIXEDFRAME) * 2;
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);

	EnableDocking(CBRS_ALIGN_ANY);

	MakeMainToolBar();
	MakeAlignBar();
	MakeExBar();
	MakeDrawBar();

	CRect	rect;

	m_wndWorkBar.m_nSvrDepth = GetPrivateProfileInt(ENVIRONMENT, "SVRDEPTH", 3, m_currentPath + "\\workshop.ini");
	m_wndWorkBar.m_nTmpDepth = GetPrivateProfileInt(ENVIRONMENT, "TMPDEPTH", 3, m_currentPath + "\\workshop.ini");
	m_wndWorkBar.m_bOffline = m_bOffline;
	if (!m_wndWorkBar.Create(_T("WorkSpace Bar"), this, ID_WORKSPACEBAR))
	{
		TRACE0("Failed to create WorkSpaceBar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}

	m_wndWorkBar.SetResize(TRUE);
	m_wndWorkBar.SetBarStyle(m_wndWorkBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	if (!m_wndPropertiesBar.Create(_T("Properties Bar"), this, ID_PROPERTIESBAR))
	{
		TRACE0("Failed to create PropertiesBar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}
	m_wndPropertiesBar.SetResize(TRUE);
	m_wndPropertiesBar.SetBarStyle(m_wndPropertiesBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	//m_wndPropertiesBar.ShowWindow(SW_HIDE); //test

	if (!m_wndPreviewBar.Create(_T("Preview Bar"), this, ID_PREVIEWBAR))
	{
		TRACE0("Failed to create PreviewBar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}
	ShowControlBar(&m_wndPreviewBar, FALSE, FALSE);
	m_wndPreviewBar.SetResize(TRUE);
	m_wndPreviewBar.SetBarStyle(m_wndPreviewBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	if (!m_wndScriptBar.Create(_T("Script window"), this, ID_SCRIPTBAR))
	{
		TRACE0("Failed to create ScriptBar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}

	m_wndScriptBar.m_bToDescription = (GetPrivateProfileInt(ENVIRONMENT, "TODESCRIPTION", 0, m_currentPath + "\\workshop.ini")?TRUE:FALSE);
	ShowControlBar(&m_wndScriptBar, FALSE, FALSE);
	m_wndScriptBar.SetResize(TRUE);
	m_wndScriptBar.SetBarStyle(m_wndScriptBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	if (!m_wndTraceBar.Create(_T("Trace window"), this, ID_TRACEBAR))
	{
		TRACE0("Failed to create TraceBar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}

	m_wndTraceBar.SetResize(TRUE);
	m_wndTraceBar.SetBarStyle(m_wndTraceBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	if (!m_wndSelectionBar.Create(_T("Selection window"), this, ID_SELECTIONBAR))
	{
		TRACE0("Failed to create SelectionBar\n");
		PostMessage(WM_CLOSE);      // fail to create
	}

	m_wndSelectionBar.SetResize(TRUE);
	m_wndSelectionBar.SetBarStyle(m_wndSelectionBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	//////////////////////////////////////////////////////////////////////////////////
	// downloading area
	// Download mapgroup.ini
	if (!m_bOffline)
	{
		m_pGroup = new CGroup();
		if (!m_pGroup->Create(NULL ,NULL, WS_CHILD, CRect(0), this, 1))
			m_bOffline = true;
		m_wndWorkBar.m_bOffline = m_bOffline;
	}

	// Download 
	m_wndWorkBar.SetMainInfo(m_curDir, m_memory, MAX_MAP-m_children.GetSize(), m_request);

	// create MDI Child Tabs
	DWORD dwStyle = MT_TOP | MT_IMAGES;
	m_wndMDITabs.Create(this, dwStyle);
	if (::IsWindow(m_wndMDITabs.m_hWnd))
		m_wndMDITabs.Update();
	
	CSplashWnd::HideSplashScreen();
	m_sProjectName.Empty();
	///////////////////////////////////////////////////////////////////////////////////
	m_wndScriptBar.EnableDocking(0);
	if (m_bSDI)
	{
		m_wndTraceBar.EnableDocking(CBRS_ALIGN_BOTTOM);
		m_wndWorkBar.EnableDocking(0);
		m_wndPropertiesBar.EnableDocking(0);
		m_wndPreviewBar.EnableDocking(0);
		m_wndSelectionBar.EnableDocking(0);
	}
	else
	{
		m_wndWorkBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndPropertiesBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndPreviewBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndTraceBar.EnableDocking(CBRS_ALIGN_ANY);
		m_wndSelectionBar.EnableDocking(CBRS_ALIGN_ANY);
	}
	m_wndSelectionBar.m_szVert = CSize(180, 100);
	m_wndSelectionBar.m_szHorz = CSize(180, 100);
	m_wndSelectionBar.m_szFloat = CSize(250, 200);

	m_wndTraceBar.m_szVert = CSize(180, 100);
	m_wndTraceBar.m_szHorz = CSize(180, 100);
	m_wndTraceBar.m_szFloat = CSize(nScreenCX - 250, 150);

	m_wndWorkBar.m_szFloat       = CSize(250, nScreenCY / 3);
	m_wndPropertiesBar.m_szFloat = CSize(250, nScreenCY / 3);

	rect.SetRect(0, nScreenCY - 100, nScreenCX, nScreenCY);
	RecalcLayout();
	if (m_bSDI)
		FloatControlBar(&m_wndTraceBar, CPoint(250, nScreenCY * 2 / 3 - 42));
	else
		DockControlBar(&m_wndTraceBar, AFX_IDW_DOCKBAR_BOTTOM, &rect);
	ShowControlBar(&m_wndTraceBar, FALSE, FALSE);

	rect.SetRect(0, 0, 500, 400);
	RecalcLayout();
	DockControlBar(&m_exTB, AFX_IDW_DOCKBAR_TOP, &rect);
	DockControlBar(&m_alignTB, AFX_IDW_DOCKBAR_TOP, &rect);
	DockControlBar(&m_mainTB, AFX_IDW_DOCKBAR_TOP, &rect);
	if (m_bSDI)
	{
		CRect tRc;
		FloatControlBar(&m_drawTB, CPoint(nScreenCX - 32, 88), AFX_IDW_DOCKBAR_RIGHT|CBRS_SIZE_DYNAMIC);
		m_drawTB.GetToolBarCtrl().SetRows(23, TRUE, &tRc);
		m_drawTB.CalcDynamicLayout(40, LM_HORZ | LM_COMMIT);
		ShowControlBar(&m_drawTB, TRUE, FALSE);
	}
	else
		DockControlBar(&m_drawTB, AFX_IDW_DOCKBAR_RIGHT, &rect);

	if (m_bSDI)
		FloatControlBar(&m_wndWorkBar, CPoint(0, 88));
	else
		DockControlBar(&m_wndWorkBar, AFX_IDW_DOCKBAR_LEFT);
	NegotiateBorderSpace(1, &rect);
	m_wndPropertiesBar.m_szVert = CSize(180, rect.Height() / 2);

	rect.SetRect(0, nScreenCY / 2, 180, nScreenCY);
	RecalcLayout();
	if (m_bSDI)
		FloatControlBar(&m_wndPropertiesBar, CPoint(0, 108 + nScreenCY / 3));
	else
		DockControlBar(&m_wndPropertiesBar, AFX_IDW_DOCKBAR_LEFT, &rect);
	ShowControlBar(&m_wndPropertiesBar, TRUE, FALSE);

	m_wndPreviewBar.m_szVert = CSize(180, 150);
	m_wndPreviewBar.m_szHorz = CSize(180, 150);
	m_wndPreviewBar.m_szFloat = CSize(180, 150);
	FloatControlBar(&m_wndPreviewBar, CPoint(20, 100));
	ShowControlBar(&m_wndPreviewBar, FALSE, FALSE);

	m_wndScriptBar.m_szFloat = CSize(500, 400);
	FloatControlBar(&m_wndScriptBar, CPoint(20, 100));

	if (m_bSDI)
		FloatControlBar(&m_wndSelectionBar, CPoint(rect.left, rect.top));
	else
		DockControlBar(&m_wndSelectionBar, AFX_IDW_DOCKBAR_LEFT);
	ShowControlBar(&m_wndSelectionBar, FALSE, FALSE);

	SetToolbarPosition();
	ShowWindow(SW_SHOW);

	if (m_backupSSMap.GetCount())
	{
		if (AfxMessageBox("It is found the backup file.\n\nDo you load file?", MB_YESNO) == IDYES)
		{
			CString sKey, sPath;
			POSITION pos;
			CFileFind finder;
			for(pos = m_backupSSMap.GetStartPosition(); pos != NULL; )
			{
				m_backupSSMap.GetNextAssoc(pos, sKey, sPath);
				if (finder.FindFile(sPath))
					fileOpen(sPath, true);
			}
		}
	}
}

void CMainFrame::MakeMainToolBar()
{
	if (!m_mainTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE)
		|| !m_mainTB.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("failed to create main toolbar\n");
		PostMessage(WM_CLOSE);
	}
	else
	{
		m_mainTB.ModifyStyle(0, TBSTYLE_FLAT);
		m_mainTB.LoadTrueColorToolBar(17, IDR_MAINFRAME, NULL, IDB_MAINBAR_DISABLE);
		
		m_mainTB.SetWindowText("Main Toolbar");
		m_mainTB.EnableDocking(CBRS_ALIGN_ANY);

		if (m_bSDI)
			m_mainTB.EnableDocking(CBRS_ALIGN_TOP);
		DockControlBar(&m_mainTB);
		
		m_mainTB.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
		DWORD dwStyle = m_mainTB.GetButtonStyle(m_mainTB.CommandToIndex(ID_APP_GOBOOKMARK));
		dwStyle |= TBSTYLE_DROPDOWN;
		m_mainTB.SetButtonStyle(m_mainTB.CommandToIndex(ID_APP_GOBOOKMARK), dwStyle);
		
		dwStyle = m_mainTB.GetButtonStyle(m_mainTB.CommandToIndex(ID_APP_NEW));
		dwStyle |= TBSTYLE_DROPDOWN;
		m_mainTB.SetButtonStyle(m_mainTB.CommandToIndex(ID_APP_NEW), dwStyle);
	}
}

void CMainFrame::MakeAlignBar()
{
	if (!m_alignTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE)
		|| !m_alignTB.LoadToolBar(IDR_ALIGNBAR))
	{
		TRACE0("failed to create align toolbar\n");
		PostMessage(WM_CLOSE);
	}
	else
	{
		m_alignTB.ModifyStyle(0, TBSTYLE_FLAT);
		m_alignTB.LoadTrueColorToolBar(17, IDR_ALIGNBAR, NULL, IDB_ALIGNBAR_DISABLE);
		
		m_alignTB.SetWindowText("Align Toolbar");
		m_alignTB.EnableDocking(CBRS_ALIGN_ANY);
		if (m_bSDI)
			m_alignTB.EnableDocking(CBRS_ALIGN_TOP);
		DockControlBar(&m_alignTB);
	}
}

void CMainFrame::MakeExBar()
{
	if (!m_exTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE)
		|| !m_exTB.LoadToolBar(IDR_EXBAR))
	{
		TRACE0("failed to create ex toolbar\n");
		PostMessage(WM_CLOSE);
	}
	else
	{
		m_exTB.ModifyStyle(0, TBSTYLE_FLAT);
		m_exTB.LoadTrueColorToolBar(17, IDR_EXBAR, NULL, IDB_EXBAR_DISABLE);
		
		m_exTB.SetWindowText("Addition Toolbar");
		m_exTB.EnableDocking(CBRS_ALIGN_ANY);
		if (m_bSDI)
			m_exTB.EnableDocking(CBRS_ALIGN_TOP);
		DockControlBar(&m_exTB);
	}
}

void CMainFrame::MakeDrawBar()
{
	if (!m_drawTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | TBSTYLE_WRAPABLE)
		|| !m_drawTB.LoadToolBar(IDR_DRAWBAR))
	{
		TRACE0("failed to create draw toolbar\n");
		PostMessage(WM_CLOSE);
	}
	else
	{
		m_drawTB.ModifyStyle(0, TBSTYLE_FLAT);
		m_drawTB.LoadTrueColorToolBar(17, IDR_DRAWBAR, NULL, IDB_DRAWBAR_DISABLE);
		
		m_drawTB.SetWindowText("Draw Toolbar");
		
		if (m_bSDI)
			m_drawTB.EnableDocking(CBRS_ALIGN_TOP);
		else
			m_drawTB.EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_drawTB);
	}
}

//
//	Login ����
//

void CMainFrame::LoginWizard()
{
	char cPath[1024];
	CWinApp* app = AfxGetApp();
	CString sTmp = (char*)SendMessage(WM_USER, ID_USR_GETIPS), tmps;

	m_bOffline = (GetPrivateProfileInt(ENVIRONMENT, "OFFLINE", 1, m_currentPath + "\\workshop.ini")?true:false);
	GetPrivateProfileString(ENVIRONMENT, "FONTNAME", "MS Sans Serif", cPath, sizeof(cPath), m_currentPath + "\\workshop.ini");
	m_sDefFontName = cPath;
	GetPrivateProfileString(ENVIRONMENT, "FONTPNT", "9", cPath, sizeof(cPath), m_currentPath + "\\workshop.ini");
	m_sDefFontPnt = cPath;
	GetPrivateProfileString(ENVIRONMENT, "INITMAP", "", cPath, sizeof(cPath), m_currentPath + "\\workshop.ini");
	m_sInitMap = cPath;
	GetPrivateProfileString(ENVIRONMENT, "PROJECTNAME", "AXIS", cPath, sizeof(cPath), m_currentPath + "\\workshop.ini");
	m_sProjectName = cPath;

	((CWorkApp*)AfxGetApp())->setRegistryKey(m_sProjectName);
	m_root = (char*)SendMessage(WM_USER, ID_USR_GETROOTDIR);
	if (m_root.IsEmpty())
		m_root = m_currentPath;
	SendMessage(WM_USER, ID_USR_SETROOTDIR, (LPARAM)m_root.operator LPCTSTR());

	if (sTmp.IsEmpty())
	{
		setServerIP();
		m_bOffline = true;
		WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "1", m_currentPath + "\\workshop.ini");
	}

	if (GetPrivateProfileInt(ENVIRONMENT, "CONNECTMODE", 0, m_currentPath + "\\workshop.ini") == 1)
		m_bOffline = true;	// wizard connect mode�϶� ������ offline����.


	RegisterCtrl("axSock.ocx", true);

	// Wizard.ocx 
	if (m_pWizardCtrl)
	{
		m_LoginDlg.ShowWindow(SW_HIDE);
		if (!m_sUserID.IsEmpty())
		{
			BOOL retvalue;
			int nPort = getPort();
			if (m_bOffline)
			{
				if (GetPrivateProfileInt(ENVIRONMENT, "CONNECTMODE", 0, m_currentPath + "\\workshop.ini") == 1)
					m_rtnStr = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
				else
				{
					m_rtnStr = "0.0.0.0";
					m_LoginDlg.ShowWindow(SW_HIDE);
				}
			}
			else
				m_rtnStr = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
			
			m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), loginSHOP, (long)m_rtnStr.operator LPCTSTR(), nPort);
			
			if (!retvalue)
				AfxMessageBox("loginSHOP Method call : fail");
			Login();
		}
		else
			StartWorkshop();
		return;
	}

	RegisterCtrl("axWizard.ocx", true);
//	RegisterCtrl("axCertify.ocx", false);
//	RegisterCtrl("AxComCtl.ocx", false);

	if (!m_pWizardCtrl)
	{
		CString sCaption;
		GetWindowText(sCaption);
		SetWindowText(m_sProjectName);
		m_pWizardCtrl = new CPreviewWnd();
	//	if (!m_pWizardCtrl->CreateControl("AxisWizard.WizardCtrl.WOW10", NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))
		if (!m_pWizardCtrl->CreateControl("AxisWizard.WizardCtrl.AX2010", NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 1))
		{
			AfxMessageBox("Create AxWizard.ocx : failed");
			delete m_pWizardCtrl;
			m_pWizardCtrl = NULL;
		}
		SetWindowText(sCaption);
	}
	
	if (!m_pWizardCtrl)
	{
		AfxMessageBox("Can't find a AxWizard.ocx");
		m_LoginDlg.ShowWindow(SW_HIDE);
		m_bOffline = true;
		WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "1", m_currentPath + "\\workshop.ini");
		StartWorkshop();
		return;
	}
	
	BOOL retvalue;
	int nPort;
	// ���������϶��� wizard�� ���Ӹ��� ���� �Ѵ�.
	if (m_bOffline)
	{
		if (GetPrivateProfileInt(ENVIRONMENT, "CONNECTMODE", 0, m_currentPath + "\\workshop.ini") == 1)
			m_rtnStr = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
		else
		{
			m_rtnStr = "0.0.0.0";
			m_LoginDlg.ShowWindow(SW_HIDE);
		}	
	}
	else
		m_rtnStr = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETIPS);
	
	nPort = getPort();
	m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), loginSHOP, (long)m_rtnStr.operator LPCTSTR(), nPort);
	
	if (!retvalue)
		AfxMessageBox("loginSHOP Method call : fail");
}

LRESULT CMainFrame::OnLoginMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDOK:
		m_LoginDlg.EnableLogin(false);
		if (!m_LoginDlg.m_offlineChk)
			WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "1", m_currentPath + "\\workshop.ini");
		else
			WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "0", m_currentPath + "\\workshop.ini");

		m_sUserID = m_LoginDlg.m_sUser;
		m_sPass = m_LoginDlg.m_sPass;
		LoginWizard();
		break;
	case IDCANCEL:
		PostMessage(WM_CLOSE);
		break;
	}
	return 0;
}

BOOL CMainFrame::Login()
{
	BOOL	retvalue;
	if (GetPrivateProfileInt(ENVIRONMENT, "SIGNONTYPE", 0, m_currentPath + "\\workshop.ini"))
	{
		struct	_signM {
			char	user[12];
			char	pass[8];
			char	pcip[20];
			char	dats[10];

		}signon;
		
		FillMemory(&signon, sizeof(signon), ' ');
		CopyMemory(signon.user, m_sUserID, min(m_sUserID.GetLength(), 12));
		CopyMemory(signon.pcip, getClientIP(), min(getClientIP().GetLength(), 20));
		
		if (1)//getPort() == 15101)	// CJ�� ��ȣȭ �� �ϰ�, account �����.
		{
			long ret = 0;
			CString sBuf;
			sBuf.Format("%s", m_sPass);
			m_pWizardCtrl->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(encPASS, 0), (LPARAM) sBuf.operator LPCTSTR());
			CopyMemory(signon.pass, (char*)ret, strlen((char*)ret));
		}
		else
			CopyMemory(signon.pass , m_sPass, m_sPass.GetLength());
		signon.dats[3] = '1';
		
		m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), signUSER, (long)&signon, sizeof(signon));
	}
	else
	{
		struct	_signM {
			char	user[12];
			char	pass[8];
			char	dats[10];
		}signon;
		
		FillMemory(&signon, sizeof(signon), ' ');
		CopyMemory(signon.user, m_sUserID, min(m_sUserID.GetLength(), 12));
				
		if (getPort() == IPORT)
		{
			long ret = 0;
			CString sBuf;
			sBuf.Format("%s\t%s", m_sPass, CString(signon.user, 12));
			m_pWizardCtrl->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), MAKEWPARAM(encPASS, 1), (LPARAM) sBuf.operator LPCTSTR());
			
			CopyMemory(signon.pass, (char*)ret, strlen((char*)ret));
		}
		else
			CopyMemory(signon.pass , m_sPass, m_sPass.GetLength());
		signon.dats[3] = '1';
		
		m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), signUSER, (long)&signon, sizeof(signon));
	}
	return retvalue;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	char	szFileName[_MAX_PATH + 1];
	if (NULL != hDropInfo)
        {
		UINT nFiles = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
		
		for(UINT nNames = 0; nNames < nFiles; nNames++)
		{
			ZeroMemory(szFileName, _MAX_PATH + 1);
			DragQueryFile(hDropInfo, nNames, (LPTSTR)szFileName, _MAX_PATH + 1);
			
			m_rtnStr = szFileName;
			SendMessage(WM_USER, ID_USR_FILEOPEN, (LPARAM)m_rtnStr.operator LPCTSTR());
		}
        }
}


//
//	main user messages
//

long CMainFrame::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CChildFrame* pActiveChild = (CChildFrame*)MDIGetActive();

	switch(LOWORD(wParam))
	{
	case ID_USR_GETROOTDIR:		// get rootdir
		{
			if (!m_sProjectName.IsEmpty())
			{
				m_root = AfxGetApp()->GetProfileString(ENVIRONMENT, ROOTDIR);
				CString dbg;
				dbg.Format("[AXISWORK][ROOTDIR][DEBUG][%s] registry branch: projectName=[%s] -> root=[%s]\n",
					__FUNCTION__, m_sProjectName.GetString(), m_root.GetString());
				OutputDebugString(dbg);
			}
			else
			{
				char twb[512];
				CString iniPath = m_currentPath + "\\workshop.ini";
				DWORD n = GetPrivateProfileString(ENVIRONMENT, ROOTDIR, "", twb, 512, iniPath);
				DWORD err = GetLastError();
				m_root = twb;
				CString dbg;
				dbg.Format("[AXISWORK][ROOTDIR][DEBUG][%s] ini branch: iniPath=[%s] iniExists=%d n=%d err=%d value=[%s]\n",
					__FUNCTION__, iniPath.GetString(), (int)(GetFileAttributes(iniPath) != INVALID_FILE_ATTRIBUTES), n, err, twb);
				OutputDebugString(dbg);
			}
			return (long)m_root.operator LPCTSTR();
		}
		break;
	case ID_USR_GETCURRENTPATH:	// get currentpath
		return (long)m_currentPath.operator LPCTSTR();
		break;
	case ID_USR_GETIPS:		// get ips
		{
			char twb[512];
			GetPrivateProfileString(ENVIRONMENT, SHOPIPS, "", twb, 512, m_currentPath + "\\workshop.ini");
			m_ips = twb;
			return (long)m_ips.operator LPCTSTR();
		}
	case ID_USR_GETSRCVIEWER:	// get source viewer
		{
			char twb[512];
			GetPrivateProfileString(ENVIRONMENT, SRCVIEWER, "", twb, 512, m_currentPath + "\\workshop.ini");
			m_srcviewer = twb;
			return (long)m_srcviewer.operator LPCTSTR();
		}

	case ID_USR_GETGAP:		// get gap
		{
			int nGap;
			nGap = GetPrivateProfileInt(ENVIRONMENT, "GAP", 5, m_currentPath + "\\workshop.ini");
			return nGap;
		}
		break;
	case ID_USR_SETROOTDIR:		// set rootdir
		WritePrivateProfileString(ENVIRONMENT, ROOTDIR, (char*)lParam, m_currentPath + "\\workshop.ini");
		//m_root = (char*)lParam;
		break;

	case ID_USR_SETIPS:		// set ips
		WritePrivateProfileString(ENVIRONMENT, SHOPIPS, (char*)lParam, m_currentPath + "\\workshop.ini");
		break;

	case ID_USR_SETSRCVIEWER:	// set source viewer
		WritePrivateProfileString(ENVIRONMENT, SRCVIEWER, (char*)lParam, m_currentPath + "\\workshop.ini");
		break;

	case ID_USR_SETGAP:	//	set gap (lParam = nGap)
		{
			CString tmps;
			tmps.Format("%d", lParam);
			WritePrivateProfileString(ENVIRONMENT, "GAP", tmps, m_currentPath + "\\workshop.ini");
			CChildFrame* child;
			for (int ii = 0; ii < m_children.GetSize(); ii++)
			{
				child = m_children.GetAt(ii);
				if (IsWindow(child->m_hWnd))
					child->m_view.SetGap(lParam);
			}
		}
		break;
	case ID_USR_REFRESHCTRL:	// no activate
		if (!pActiveChild)
		{
			m_wndPropertiesBar.ResetCombo(0);
			m_wndScriptBar.ResetCombo(0);
			return 0;
		}
		break;
	case ID_USR_FILEOPEN:	//	file open
		fileOpen((char*)lParam);
		break;
	case ID_USR_ISDROPAREA:	//	Drop area?
		{
			CPoint pnt = CPoint(LOWORD(lParam), HIWORD(lParam));
			CRect rc, rc2;
			NegotiateBorderSpace(1, &rc);
			GetWindowRect(&rc2);
			rc.OffsetRect(rc2.left, rc2.top);
			if (rc.PtInRect(pnt))
				return 1;
		}
		break;
	case ID_USR_SETGROUP:	// setting map group
		m_wndPropertiesBar.setGroup(m_pGroup->getGroup());
		break;
	case ID_USR_GETGROUP:	// get map group (lParam : idx)
		if (m_pGroup)
		{
			m_sGroup = m_pGroup->getGroup((int)lParam);
			return (long)m_sGroup.operator LPCTSTR();
		}
		break;
	case ID_USR_SERVEROPEN:	//	server open
		{
			CString tmps;
			CChildFrame* child;
			m_request = false;
			if (lParam == 1) // template
			{
				m_memory  = m_wndWorkBar.m_TreeTemplate.m_path;
				for (int ii = 0; ii < m_wndWorkBar.m_TreeTemplate.m_files.GetSize(); ii++)
				{
					if (m_children.GetSize() >= MAX_MAP)
						break;
					
					tmps.Empty();
					wccSetTempPath(tmps, (char *)m_wndWorkBar.m_TreeTemplate.m_files.GetAt(ii).operator LPCTSTR());
					AfxGetApp()->DoWaitCursor(1);
					child = openMAP(tmps);
					if (child != (CChildFrame *) NULL)
						child->m_mapH.pathsetted = false;
				}
				AfxGetApp()->DoWaitCursor(-1);
			}
			else if (lParam == 0)
			{
				m_memory  = m_wndWorkBar.m_TreeServer.m_path;
				for (int ii = 0; ii < m_wndWorkBar.m_TreeServer.m_files.GetSize(); ii++)
				{
					if (m_children.GetSize() >= MAX_MAP)
						break;
					
					tmps.Empty();
					wccSetTempPath(tmps, (char *)m_wndWorkBar.m_TreeServer.m_files.GetAt(ii).operator LPCTSTR());
					//AfxGetApp()->DoWaitCursor(1);
					child = openMAP(tmps);
					if (child != (CChildFrame *) NULL)
						child->m_mapH.pathsetted = false;
				}
			}
			else
			{
				if (m_children.GetSize() >= MAX_MAP)
					break;
				tmps = (char*)lParam;
				m_wndWorkBar.m_TreeServer.OpenMap(tmps);
			}
		}
		break;
	case ID_USR_GETCOLOR: //	getColor (lParam:colorinfo)
		if (lParam & 0x02000000)
			return (COLORREF) lParam;
		return m_palette->GetPaletteRGB(lParam);
		break;
	case ID_USR_STATUSERR: //	status error (lParam: error code)
		m_statusBar.SetPaneText(0, getErrString(lParam));
		break;
	case ID_USR_STATUSLINEINFO: //		status script line info
		{
			if (lParam == -1)
				m_statusBar.SetPaneText(1, "");
			else
			{
				CString tmp;
				tmp.Format("Ln %d, Col %d", LOWORD(lParam) + 1, HIWORD(lParam) + 1);
				m_statusBar.SetPaneText(1, tmp);
			}
		}
		break;
	case ID_USR_PREVIEW:
		if (m_wndPreviewBar.IsWindowVisible() && strlen((char*)lParam) >= 0 && !m_bOffline)
			m_wndPreviewBar.setShow(m_palette, m_draw, CRect(0), m_root, (char*)lParam, "preview", "");
		break;
	case ID_USR_DROPPNT:
		m_dropPnt = CPoint(LOWORD(lParam), HIWORD(lParam));
		break;
	case ID_USR_ISOFFLINE:
		if (m_bOffline)
			return 1;
		else
			return 0;
		break;
	case ID_USR_SETFOCUSWRK:
		m_wndWorkBar.m_fnames.SetFocus();
		break;
	case ID_USR_GETWIZARD:
		return (long)m_pWizardCtrl;
	case ID_USR_PROGRESS:
		CSplashWnd::NextScreen(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case ID_USR_SETOFFLINE:
		if (lParam)
		{
			m_bOffline = true;
			WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "1", m_currentPath + "\\workshop.ini");
		}
		else
		{
			m_bOffline = false;
			WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "0", m_currentPath + "\\workshop.ini");
		}
		return 0;
	case ID_USR_GETCORGB:
		{
			return GetPrivateProfileInt(ENVIRONMENT, "CORGB", 14024913, m_currentPath + "\\workshop.ini");
		}
		break;
	case ID_USR_MOUSETIP:
		{
			CPoint pnt;
			GetCursorPos(&pnt);
			pnt.Offset(10, -10);

			if (lParam)
				m_tips.AddTips(1, (char*)lParam);
			if (HIWORD(wParam) == 1)
				m_tips.ShowTip(1, pnt);
			else
				m_tips.RemoveTips();
		}
		break;
	case ID_USR_RECALCMAINSIZE:
		if (m_bSDI)
		{
			CRect rc;
			GetWindowRect(&rc);
			SetWindowPos(NULL, 0, 0, rc.Width(), GetCYSDIFrame(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
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
	}

	if (!m_children.GetSize() || !pActiveChild)
		return 0;

	switch(LOWORD(wParam))
	{
	case ID_USR_REFRESHCTRL:	// from child to propertiesbar refresh control (lParam = bDelete)
					// LOWORD(lParam) & 2 : all combo list , else only one form(HIWORD(lParam) combo list replace
		{
			if (!(LOWORD(lParam) & 2))
			{
				if (LOWORD(lParam) & 1)
					m_wndScriptBar.WriteScript();
				
				m_wndSelectionBar.Initialize(&pActiveChild->m_mapH);
				m_wndPropertiesBar.Initialize(&pActiveChild->m_mapH);
				m_wndSelectionBar.ResetTree();
				m_wndPropertiesBar.ResetCombo();
				m_wndScriptBar.ResetCombo(1);
				m_wndScriptBar.Initialize(&pActiveChild->m_mapH);	// resetcombo -> initialize
				if (pActiveChild->m_view.m_pSCEdit)
					pActiveChild->m_view.m_pSCEdit->SetPythonMode(pActiveChild->m_mapH.pythonMode);

				for (int ii = 0; ii < pActiveChild->m_pFormItem->getFormCount(); ii++)
				{
					if (!pActiveChild->m_pFormItem->getFormStr(ii, "name").IsEmpty())
					{
						CString sCbo = pActiveChild->m_pFormItem->getFormStr(ii, "name");
						m_wndSelectionBar.AddTreeItem(sCbo, ii, 1);
						m_wndPropertiesBar.AddComboString(sCbo, ii);
						m_wndScriptBar.AddComboString(sCbo, ii);
					}
				}
				
				m_wndScriptBar.SelectControl(pActiveChild->getSelectedForm());
				m_wndPropertiesBar.SelectControl(pActiveChild->getSelectedForm());
				m_wndSelectionBar.SelectControl(pActiveChild->getSelectedForm());
			}
			else
			{
				if (!pActiveChild->m_pFormItem->getFormStr(HIWORD(lParam), "name").IsEmpty())
				{
					CString sCbo = pActiveChild->m_pFormItem->getFormStr(HIWORD(lParam), "name");
					m_wndSelectionBar.ReplaceTreeItem(sCbo, HIWORD(lParam));
					m_wndPropertiesBar.ReplaceComboString(sCbo, HIWORD(lParam));
					m_wndScriptBar.ReplaceComboString(sCbo, HIWORD(lParam));
				}
			}
		}
		break;
	case ID_USR_SELCTRLTOPRO:	// from child to propertiesbar select control
		{
			m_wndPropertiesBar.SelectControl(pActiveChild->getSelectedForm());
			if (m_wndScriptBar.IsWindowVisible())
				m_wndScriptBar.SelectControl(pActiveChild->getSelectedForm());
			m_wndSelectionBar.SelectControl(pActiveChild->getSelectedForm());
		}
		break;
	case ID_USR_SELCTRLTOCHILD:	// from propertiesBar, scriptBar, selectionBar to child select control
		{
			int idx = (short)LOWORD(lParam);
			
			pActiveChild->setSelectForm(idx, (HIWORD(wParam)?true:false));
			switch (HIWORD(lParam))
			{
			case SC_PROPERTIESBAR:	// properties bar
				m_wndScriptBar.SelectControl(pActiveChild->getSelectedForm());
				m_wndSelectionBar.SelectControl(pActiveChild->getSelectedForm());
				break;
			case SC_SCRIPTBAR:	// script bar
				m_wndPropertiesBar.SelectControl(pActiveChild->getSelectedForm());
				m_wndSelectionBar.SelectControl(pActiveChild->getSelectedForm());
				break;
			case SC_SELECTIONBAR: // selection bar
				m_wndPropertiesBar.SelectControl(pActiveChild->getSelectedForm());
				m_wndScriptBar.SelectControl(pActiveChild->getSelectedForm());
				break;
			}
		}
		break;
	case ID_USR_FOCUSCHILD:		// focus child from mapH pointer
		{
			if (IsWindow(pActiveChild->m_hWnd))
				if (&pActiveChild->m_mapH == (_mapH*)lParam)
					return FALSE;
			for (int ii = 0; ii < m_children.GetSize(); ii++)
			{
				pActiveChild = m_children.GetAt(ii);
				if (IsWindow(pActiveChild->m_hWnd))
					if (&pActiveChild->m_mapH == (_mapH*)lParam)
					{
						pActiveChild->ActivateFrame();
						return TRUE;
					}
			}
			return FALSE;
		}
		break;
	case ID_USR_REDRAWCTRL:	//	refresh lParam = control num
		{
			if (lParam == -1)
			{
				CRect	cRc, wRc;
				int	mapWidth = pActiveChild->m_mapH.width, mapHeight = pActiveChild->m_mapH.height;
				pActiveChild->GetClientRect(&cRc);
				pActiveChild->GetWindowRect(&wRc);
				if (m_bSDI)
				{
					pActiveChild->m_pPopFrame->GetClientRect(&cRc);
					pActiveChild->m_pPopFrame->GetWindowRect(&wRc);
				}
				int dW = wRc.Width()  - cRc.Width();
				int dH = wRc.Height() - cRc.Height();
				
				if (cRc.Height() != mapHeight || cRc.Width() != mapWidth)
				{
					CFrameWnd *pChild = (CFrameWnd*)pActiveChild;
					if (m_bSDI)
						pChild = (CFrameWnd*)pActiveChild->m_pPopFrame;

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
			pActiveChild->m_view.InvalidateRect2(lParam);
		}
		break;
	case ID_USR_SETCAPTION:	//	active child caption refresh
			if (pActiveChild->m_mapH.mapK == MK_NORM)
				pActiveChild->SetWindowText(pActiveChild->m_mapH.caption);
			else if (pActiveChild->m_mapH.mapK == MK_PROCEDURES)
			{
				pActiveChild->SetWindowText("Procedures");
			}
			else
			{
				CString tmp;
				tmp.Format("Template - %s", pActiveChild->m_mapH.mapdesc);
				pActiveChild->SetWindowText(tmp);
			}
			if (::IsWindow(m_wndMDITabs.m_hWnd))
				m_wndMDITabs.Update();
			
			m_wndScriptBar.SetWindowText(CString("script window - ") + pActiveChild->m_mapH.caption);
		break;
	case ID_USR_SETMAPNAME:		//	set mapname
		if (m_bSDI)
			pActiveChild->m_pPopFrame->GetSystemMenu(FALSE)->ModifyMenu(0, MF_BYPOSITION|MF_STRING, 0, pActiveChild->m_mapH.mapN);
		else
			pActiveChild->GetSystemMenu(FALSE)->ModifyMenu(0, MF_BYPOSITION|MF_STRING, 0, pActiveChild->m_mapH.mapN);
		m_wndSelectionBar.ReplaceMapName(&pActiveChild->m_mapH);
		break;
	case ID_USR_SETFOCUSCHILD:	//	active child focus
		if (m_bSDI)
			pActiveChild->m_pPopFrame->SetFocus();
		else
			pActiveChild->SetFocus();
		break;
	case ID_USR_LOADTEMPLATE:	//	loadTemplate
		{
			CPoint pt = CPoint(LOWORD(lParam), HIWORD(lParam));
			pActiveChild->ScreenToClient(&pt);
			loadTemplate(pt);
		}
		break;
	case ID_USR_SETFOCUSPRO:	//	activate properties bar
		m_wndPropertiesBar.SetFocus();
		break;
	case ID_USR_KEYIN:		//	key pressed at activate view
		m_wndPropertiesBar.SetFocus();
		m_wndPropertiesBar.SetKeyIn((char)lParam);
		break;
	case ID_USR_REFRESHPRO:		//	refresh properties lParam = row;
		m_wndPropertiesBar.Refresh(lParam);
		break;
	case ID_USR_ERASECTRL:		//	listgrid erasectrl
		m_wndPropertiesBar.m_listGrid.EraseCtrls(FALSE);	// SDI������ ���� FALSE
		break;
	case ID_USR_SETUNDO:		//	lParam true:pushUndo, false:popUndo
		if (lParam)
		{
			pActiveChild->pushUndo(getSource(false, &pActiveChild->m_mapH));
			pActiveChild->popRedo(true);
		}
		else
			pActiveChild->popUndo();
		break;
	case ID_USR_SETFINDCAPTION:	//	findreplace dialog caption change
		{
			CString sCaption;			
			sCaption = "script window - ";
			sCaption += pActiveChild->m_mapH.caption;
			m_wndScriptBar.SetWindowText(sCaption);
		}
		break;
	case ID_USR_CURRENTWORD:	//	current selection word of script
		{
			m_rtnStr.Empty();
			if (lParam == 3)
				return m_wndTraceBar.m_resultEdit.SendMessage(WM_USER, 0 , 0);
			else if (pActiveChild->m_mapH.mapK == MK_PROCEDURES)
				m_rtnStr = pActiveChild->m_view.m_pSCEdit->SelCursorWord();
			else if (m_wndScriptBar.IsWindowVisible() && 
				m_wndScriptBar.m_editScript.IsWindowEnabled())
				m_rtnStr = m_wndScriptBar.m_editScript.SelCursorWord();
			return (long)m_rtnStr.operator LPCTSTR();
		}
		break;
	case ID_USR_GETCONTROLEVENT:	//	control object event
		{
			void* rtValue;
			if (m_controlEvent.Lookup((char*)lParam, rtValue))
				return (long)rtValue;
			else
			{
				loadControlEvent();
				if (m_controlEvent.Lookup((char*)lParam, rtValue))
					return (long)rtValue;
				else
					return 0;
			}
		}
		break;
	case ID_USR_ENABLEPROP:		// Enable Properties bar
		if (lParam)
			m_wndPropertiesBar.EnableProperty(true);
		else
			m_wndPropertiesBar.EnableProperty(false);
		break;
	case ID_USR_SORTORDER:		// sort order
		pActiveChild->m_view.sortIoOrder();
		pActiveChild->m_view.sortTabOrder();
		break;
	case ID_USR_DELETESELFORMS:
		pActiveChild->DeleteSelForms();
		break;
	case ID_USR_SCRIPTEVENT:
		m_wndSelectionBar.UpdateEventItem(HIWORD(lParam), (short)LOWORD(lParam), HIWORD(wParam)?true:false);
		break;
	case ID_USR_SELECTEVENT:
		m_wndScriptBar.SelectEvent(lParam);
		break;
	case ID_USR_TBPRESSBTN:
		PressButton(lParam);
		break;
	}
	return 0;
}

//
//	Fire Event
//
void CMainFrame::OnFireEvent(long type, long pBytes, long nBytes)
{
	switch(type)
	{
	case FEV_AXIS:
		switch(LOWORD(pBytes))
		{
		case runAXIS:
			StartWorkshop();
			break;
		case newVIEW:
			{
				struct	_userWH* userWH = (struct _userWH *)nBytes;
				m_wndTraceBar.AddText("[Create Window " + CString(userWH->maps) + "]\n");
			}
			break;
		case renVIEW:
			{
				m_wndTraceBar.AddText("[Change Map " + CString((char*)nBytes, 8) + "]\n");
			}
			break;
		case delVIEW:
			{
				CChildFrame *child = (CChildFrame *) MDIGetActive();
				if (child)
				{
					if (child->m_view.m_pPreView)
					{
						child->m_view.Preview();
						m_wndTraceBar.AddText("[Closed]\n");
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
					if (child->m_view.m_pPreView)
					{
						child->SetWindowText((char*)nBytes);
					}
				}
			}
			break;
		}
		break;
	case FEV_OPEN:
		if (IsWindowVisible())
			break;
		if (!nBytes)
		{
			if (!m_sUserID.IsEmpty())	// login
			{
				if (!Login())
				{
					OnFireEvent(FEV_ERROR, TRUE, (long)"Login Failed");
					return;
				}
			}
			else				// offline
			{

				BOOL retvalue;
				m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), signUSERx, NULL, 0);
				StartWorkshop();
			}
		}
		else
		{		// socket open error
			CString sIP = (char*)SendMessage(WM_USER, ID_USR_GETIPS);
			if (GetPrivateProfileInt(ENVIRONMENT, "CONNECTMODE", 0, m_currentPath + "\\workshop.ini") != 2)	// builder ����ƴϸ�
			{
				m_bOffline = true;
				WritePrivateProfileString(ENVIRONMENT, "OFFLINE", "1", m_currentPath + "\\workshop.ini");
			}
			
			if (m_pWizardCtrl)
			{
				BOOL retvalue;
				int nPort = getPort();
				m_rtnStr = "0.0.0.0";
				m_pWizardCtrl->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void*)&retvalue, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), loginSHOP, (long)m_rtnStr.operator LPCTSTR(), nPort);
				
				if (!retvalue)
					AfxMessageBox("loginSHOP Method call : fail");
			}
			else
				StartWorkshop();
		}
		break;
	case FEV_CLOSE:
		AfxMessageBox("Communication operation with server was disconnected.");
		break;
	case FEV_GUIDE:
		if (!m_LoginDlg.IsWindowVisible() && !IsWindowVisible())
			m_LoginDlg.ShowWindow(SW_SHOW);
		break;
	case FEV_ERROR:
		if (pBytes && !IsWindowVisible())
		{
			m_LoginDlg.setMessage((char*)nBytes);
			m_LoginDlg.EnableLogin(true);
			m_LoginDlg.ShowWindow(SW_SHOW);
		}
		break;
	}
}

int CMainFrame::getPort()
{
	switch(GetPrivateProfileInt(ENVIRONMENT, "PROXY", 0, m_currentPath + "\\workshop.ini"))
	{
	case 0:	// Local 15101
		return IPORT;
	case 2:	// Proxy 15201
		return 15201;
	default:// Internet 80
		return 80;
	}
	return 80;
}

HMENU CMainFrame::NewDefaultMenu()
{
	static UINT toolbars[] = {
		IDR_MAINFRAME,
		IDR_EXBAR
	};
	m_defaultMenu.SetBitmapBackground(RGB(0, 255, 255));
	m_defaultMenu.SetMenuDrawMode(BCMENU_DRAWMODE_XP);
	m_defaultMenu.LoadMenu(IDR_MAINFRAME);

	CImageList temp;
	temp.Create(17, 17, ILC_COLORDDB|ILC_MASK, 1, 1);
	temp.Add((HICON)AfxGetApp()->LoadIcon(IDI_MAP));
	temp.Add((HICON)AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	temp.Add((HICON)AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	m_defaultMenu.ModifyODMenu(NULL, ID_APP_NEW, &temp, 0);
	m_defaultMenu.ModifyODMenu(NULL, ID_APP_NEWTMPL, &temp, 1);
	m_defaultMenu.ModifyODMenu(NULL, ID_APP_NEWSC, &temp, 2);
	temp.DeleteImageList();

	m_defaultMenu.LoadToolbars(toolbars, 2);
	return (m_defaultMenu.Detach());
}

void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	BOOL setflag = FALSE;
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{
		if (IsMenu((HMENU)lpMeasureItemStruct->itemID))
		{
			CMenu* cmenu = CMenu::FromHandle((HMENU)lpMeasureItemStruct->itemID);
			
			if (m_defaultMenu.IsMenu(cmenu))
			{
				m_defaultMenu.MeasureItem(lpMeasureItemStruct);
				setflag = TRUE;
			}
		}
	}
	
	if (!setflag)
		CMDIFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

LRESULT CMainFrame::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	LRESULT lresult;
	if(m_defaultMenu.IsMenu(pMenu))
		lresult = BCMenu::FindKeyboardShortcut(nChar, nFlags, pMenu);
	else
		lresult = CMDIFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
	return lresult;
}

void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	if(!bSysMenu)
	{
		if(m_defaultMenu.IsMenu(pPopupMenu))
			BCMenu::UpdateMenu(pPopupMenu);
	}
}

#include "winsock2.h"
CString CMainFrame::getClientIP()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	char name[255];
	CString sIp;
	
	struct hostent* hostinfo;
	wVersionRequested = MAKEWORD(2, 0);
	
	if (WSAStartup( wVersionRequested, &wsaData ) == 0)
	{        
		if (gethostname ( name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				for (int ii = 0; hostinfo->h_addr_list[ii] != NULL; ii++)
				{
					sIp = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[ii]);        
				}
			}
		}        
		
		WSACleanup();
	}
	return sIp;
}

CString CMainFrame::GetToolBarInfo(int idx)
{
	CString retVal = "";
	CRect rc = CRect(0, 0, 0, 0);
	DWORD style, nState = 0;
	int	nRows = 1;
	CControlBar	*pControlBar = NULL;

	switch (idx)
	{
	case 0:	//	mainBar
		pControlBar = (CControlBar*)&m_mainTB;			break;
	case 1:	//	alignBar
		pControlBar = (CControlBar*)&m_alignTB;			break;
	case 2:	//	exBar
		pControlBar = (CControlBar*)&m_exTB;			break;
	case 3:	//	drawBar
		pControlBar = (CControlBar*)&m_drawTB;			break;
	case 4:	//	workspaceBar
		pControlBar = (CControlBar*)&m_wndWorkBar;		break;
	case 5:	//	propertiesBar
		pControlBar = (CControlBar*)&m_wndPropertiesBar;	break;
	case 6:	//	previewBar
		pControlBar = (CControlBar*)&m_wndPreviewBar;		break;
	case 7:	//	scriptBar
		pControlBar = (CControlBar*)&m_wndScriptBar;		break;
	case 8:	//	traceBar
		pControlBar = (CControlBar*)&m_wndTraceBar;		break;
	case 9:	//	selectionBar
		pControlBar = (CControlBar*)&m_wndSelectionBar;		break;
	}

	if (pControlBar && IsWindow(pControlBar->m_hWnd))
	{
		pControlBar->GetWindowRect(&rc);
		style = pControlBar->GetBarStyle();
		nState = (pControlBar->IsWindowVisible() ? 1: 0);
		nState |= (pControlBar->IsFloating() ? 2 : 0);
		if (pControlBar->IsFloating() && idx >= 4)
		{
			rc.right = rc.left + ((CSizingControlBarG*)pControlBar)->m_szFloat.cx;
			rc.bottom = rc.top + ((CSizingControlBarG*)pControlBar)->m_szFloat.cy;
		}
		if (idx < 4)
			nRows = ((CToolBar*)pControlBar)->GetToolBarCtrl().GetRows();
		retVal.Format("%d|%d|%d|%d|;%ld;%d;%d;", rc.left, rc.top, rc.right, rc.bottom, style, nState, nRows);
	}

	return retVal;
}

void CMainFrame::SetToolBarInfo(int idx, CRect rc, DWORD style, int nState, int nRows)
{
	if (idx < 0 || idx > 9)
		return;
	CControlBar	*pControlBar = NULL;
	switch (idx)
	{
	case 0:	//	mainBar
		pControlBar = (CControlBar*)&m_mainTB;			break;
	case 1:	//	alignBar
		pControlBar = (CControlBar*)&m_alignTB;			break;
	case 2:	//	exBar
		pControlBar = (CControlBar*)&m_exTB;			break;
	case 3:	//	drawBar
		pControlBar = (CControlBar*)&m_drawTB;			break;
	case 4:	//	workspaceBar
		pControlBar = (CControlBar*)&m_wndWorkBar;
		m_wndWorkBar.m_szVert = CSize(rc.Width(), rc.Height());
		m_wndWorkBar.m_szHorz = CSize(rc.Width(), rc.Height());
		m_wndWorkBar.m_szFloat = CSize(rc.Width(), rc.Height());
		break;
	case 5:	//	propertiesBar
		pControlBar = (CControlBar*)&m_wndPropertiesBar;
		m_wndPropertiesBar.m_szVert = CSize(rc.Width(), rc.Height());
		m_wndPropertiesBar.m_szHorz = CSize(rc.Width(), rc.Height());
		m_wndPropertiesBar.m_szFloat = CSize(rc.Width(), rc.Height());
		break;
	case 6:	//	previewBar
		pControlBar = (CControlBar*)&m_wndPreviewBar;
		m_wndPreviewBar.m_szVert = CSize(rc.Width(), rc.Height());
		m_wndPreviewBar.m_szHorz = CSize(rc.Width(), rc.Height());
		m_wndPreviewBar.m_szFloat = CSize(rc.Width(), rc.Height());
		break;
	case 7:	//	scriptBar
		pControlBar = (CControlBar*)&m_wndScriptBar;
		m_wndScriptBar.m_szVert = CSize(rc.Width(), rc.Height());
		m_wndScriptBar.m_szHorz = CSize(rc.Width(), rc.Height());
		m_wndScriptBar.m_szFloat = CSize(rc.Width(), rc.Height());
		break;
	case 8:	//	traceBar
		pControlBar = (CControlBar*)&m_wndTraceBar;
		m_wndTraceBar.m_szVert = CSize(rc.Width(), rc.Height());
		m_wndTraceBar.m_szHorz = CSize(rc.Width(), rc.Height());
		m_wndTraceBar.m_szFloat = CSize(rc.Width(), rc.Height());
		break;
	case 9:	//	selectionBar
		pControlBar = (CControlBar*)&m_wndSelectionBar;
		m_wndSelectionBar.m_szVert = CSize(rc.Width(), rc.Height());
		m_wndSelectionBar.m_szHorz = CSize(rc.Width(), rc.Height());
		m_wndSelectionBar.m_szFloat = CSize(rc.Width(), rc.Height());
		break;
	}

	int gapY = GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;

	ShowControlBar(pControlBar, FALSE, FALSE);
	DWORD	dwPos;

	if (style & CBRS_ALIGN_TOP)
		dwPos = AFX_IDW_DOCKBAR_TOP;
	else if (style & CBRS_ALIGN_BOTTOM)
		dwPos = AFX_IDW_DOCKBAR_BOTTOM;
	else if (style & CBRS_ALIGN_LEFT)
		dwPos = AFX_IDW_DOCKBAR_LEFT;
	else if (style & CBRS_ALIGN_RIGHT)
		dwPos = AFX_IDW_DOCKBAR_RIGHT;

	if (m_bSDI && !(nState & 2) && !(style & CBRS_ALIGN_TOP))
		nState |= 2;

	if ((m_bSDI && (idx == 4 || idx == 5 || idx == 6 || idx == 9)) || (nState & 2))
	{
		rc.InflateRect(0, gapY, 0, 0);
		FloatControlBar(pControlBar, CPoint(rc.left, rc.top), dwPos|CBRS_SIZE_DYNAMIC);
		if (idx < 4)
		{
			CRect tRc;
			((CToolBar*)pControlBar)->GetToolBarCtrl().SetRows(nRows, TRUE, &tRc);
			((CToolBar*)pControlBar)->CalcDynamicLayout(rc.Width() + 10, LM_HORZ | LM_COMMIT);
		}
	}
	else
		DockControlBar(pControlBar, dwPos, &rc);
	
	ShowControlBar(pControlBar, (nState & 1), FALSE);
}

void CMainFrame::ParseToolBar(int idx, CString info)
{
	if (info.IsEmpty())
		return;
	CRect rc;
	DWORD style;
	int   nState, nRows;

	rc = ParseRect(Parser(info, ";"));
	style = atol(Parser(info, ";"));
	nState = atoi(Parser(info, ";"));
	nRows = atoi(Parser(info, ";"));

	SetToolBarInfo(idx, rc, style, nState, nRows);
}

CRect CMainFrame::ParseRect(CString str)
{
	if (str.IsEmpty())
		return CRect(0);
	
	CRect rt;
	rt.left = atol(Parser(str, "|"));
	rt.top = atol(Parser(str, "|"));
	rt.right = atol(Parser(str, "|"));
	rt.bottom = atol(Parser(str, "|"));
	return rt;
}

int CMainFrame::GetToolbarPriority(CString info)
{
	if (info.IsEmpty())
		return 0;
	CRect rc;
	DWORD style;
	int	nState;

	rc = ParseRect(Parser(info, ";"));
	style = atol(Parser(info, ";"));
	nState = atoi(Parser(info, ";"));

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

void CMainFrame::SetToolbarPosition()
{
	CString sTmp = "", sSection = "POSITION";
	if (m_bSDI)
		sSection = "SDI_POSITION";
	GetPrivateProfileString(sSection, "MAINFRAME", "", sTmp.GetBuffer(512), 512, m_currentPath + "\\workshop.ini");
	sTmp.ReleaseBuffer();

	if (!sTmp.IsEmpty())
		MoveWindow(ParseRect(sTmp));

	if (m_bSDI)
		SendMessage(WM_USER, ID_USR_RECALCMAINSIZE);

	CStringArray	sortArr;
	for (int ii = 0; ii < 10; ii++)
	{
		CString sToolbar;
		sToolbar.Format("TOOLBAR%d", ii);
		sTmp.GetBufferSetLength(512);
		GetPrivateProfileString(sSection, sToolbar, "", sTmp.GetBuffer(512), 512, m_currentPath + "\\workshop.ini");
		sTmp.ReleaseBuffer();
		if (sTmp.IsEmpty())
			continue;
		int nPriority = GetToolbarPriority(sTmp);
		sToolbar.Format("%d|%d|%s", nPriority, ii, sTmp);

		int kk = 0;
		for (int kk = sortArr.GetSize() - 1; kk >= 0 ; kk--)
		{
			CString sPriority = sortArr.GetAt(kk);
			if (atoi(Parser(sPriority, "|")) < nPriority)
				break;
		}

		kk++;
		sortArr.InsertAt(kk, sToolbar);
	}
	for (int kk = 0; kk < sortArr.GetSize(); kk++)
	{
		sTmp = sortArr.GetAt(kk);
		Parser(sTmp, "|");
		int nIdx = atoi(Parser(sTmp, "|"));
		ParseToolBar(nIdx, sTmp);
	}
	sortArr.RemoveAll();
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_ACTIVE || nState == WA_CLICKACTIVE)
		if (m_bSDI && !pWndOther && !bMinimized)
			SDIShowChild();
}

void CMainFrame::GetChildStack(CArray <CChildFrame*, CChildFrame*>&Stack)
{
	CChildFrame* pChild;
	((CWorkApp*)AfxGetApp())->m_pWndCurrentChild = NULL;
	do
	{
		pChild = (CChildFrame*)((CWorkApp*)AfxGetApp())->GetNextMDIChildWnd();
		if (pChild)
			Stack.Add(pChild);
	}while (pChild);
}

void CMainFrame::SDIShowChild(BOOL bShow /*= TRUE*/)
{
	CArray <CChildFrame*, CChildFrame*> Stack;
	GetChildStack(Stack);
	CChildFrame* pChild;
	
	if (bShow)
	{
		//for (int ii = 0; ii < Stack.GetSize(); ii++)
		for (int ii = Stack.GetSize() - 1; ii >= 0; ii--)
		{
			pChild = Stack.GetAt(ii);
			pChild->m_pPopFrame->ShowWindow(SW_SHOWNA);
		}
		ShowWindow(SW_SHOWNA);
	}
	else
	{
		for (int ii = Stack.GetSize() - 1; ii >= 0; ii--)
		{
			pChild = Stack.GetAt(ii);
			pChild->m_pPopFrame->ShowWindow(SW_HIDE);
		}
	}
	Stack.RemoveAll();
	PostMessage(WM_USER, ID_USR_RECALCMAINSIZE);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (m_bSDI && nID == SC_RESTORE)
	{
		SDIShowChild();
	}
	if (m_bSDI && nID == SC_MINIMIZE)
	{
		SDIShowChild(FALSE);
	}
	CMDIFrameWnd::OnSysCommand(nID, lParam);
}

long CMainFrame::GetCYSDIFrame()
{
	CRect rc;
	long cy_min = GetSystemMetrics(SM_CYSCREEN), cy_max = 0, 
		retvalue = GetSystemMetrics(SM_CYBORDER) * 4 + 
				GetSystemMetrics(SM_CYCAPTION) + 
				GetSystemMetrics(SM_CYMENU) * 2;
	CArray <CControlBar*, CControlBar*> pTemp;

	pTemp.Add((CControlBar*)&m_mainTB);
	pTemp.Add((CControlBar*)&m_alignTB);
	pTemp.Add((CControlBar*)&m_exTB);
	pTemp.Add((CControlBar*)&m_drawTB);
	pTemp.Add((CControlBar*)&m_wndTraceBar);		
	
	for (int ii = 0; ii < pTemp.GetSize(); ii++)
	{
		if (!pTemp.GetAt(ii)->IsFloating() && pTemp.GetAt(ii)->IsWindowVisible())
		{
			pTemp.GetAt(ii)->GetWindowRect(&rc);
			if (rc.top >= 0 && cy_min > rc.top)
				cy_min = rc.top;
			if (rc.bottom >= 0 && cy_max < rc.bottom)
				cy_max = rc.bottom;
		}
	}

	if (::IsWindow(m_wndMDITabs.m_hWnd) && m_children.GetSize())
	{
		m_wndMDITabs.GetWindowRect(&rc);
		if (rc.top >= 0 && cy_min > rc.top)
			cy_min = rc.top;
		if (rc.bottom >= 0 && cy_max < rc.bottom)
			cy_max = rc.bottom;
	}
	
	if (cy_max - cy_min > 0)
		retvalue += cy_max - cy_min;
	pTemp.RemoveAll();
	
	return retvalue + 3;
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	if (m_bSDI && IsWindowVisible())
	{
		long cy = GetCYSDIFrame();
		lpMMI->ptMaxSize.y      = cy;
		lpMMI->ptMinTrackSize.y = cy;
		lpMMI->ptMaxTrackSize.y = cy;
	}
	CMDIFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::LoadScriptColors()
{
	m_wndScriptBar.ReloadColors();
	for (int ii = 0; ii < m_children.GetSize(); ii++)
		m_children.GetAt(ii)->ReloadColors();
}
