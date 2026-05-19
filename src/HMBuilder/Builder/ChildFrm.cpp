
// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "axisMBuilder.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "VBScriptEdit.h"
// updateXX__2014
#include "undoredo.h"

#include "h/mainvar.h"
//#include "amCom/formItem.h"

#include "amCc/LibBuild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	MAX_UNDO	20
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_WM_MDIACTIVATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_SYSCOMMAND()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame(CMainFrame* frame, CString curdir, struct _block* block)
{
	m_mainFrm = frame;
	m_dir   = curdir;
	m_block = block;

	m_bEmulRun = false;
	m_bSubChild = false;

	m_delete = false;
	m_formItem = NULL;
	m_bookmarks.RemoveAll();
	m_undos.RemoveAll();
	m_redos.RemoveAll();
}

CChildFrame::~CChildFrame()
{	
	DeleteUndoMap();
	for (int ii = 0; ii < m_undos.GetCount(); ii++)
		delete (CUndoRedo*)m_undos.GetAt(ii);
	m_undos.RemoveAll();

	for (int ii = 0; ii < m_redos.GetCount(); ii++)
		delete (CUndoRedo*)m_redos.GetAt(ii);	
	
	m_redos.RemoveAll();
	m_bookmarks.RemoveAll();

	if (m_formItem)
	{
		m_formItem->RemoveAll(m_bEmulRun);
		delete m_formItem;
	}

	if (m_mapH.trinfo != NULL)
	{
		delete[] m_mapH.trinfo;
		m_mapH.trinfo = NULL;
	}
	if (m_mapH.rtsinfo != NULL)
	{
		delete[] m_mapH.rtsinfo;
		m_mapH.rtsinfo = NULL;
	}

	if (m_bEmulRun)
		m_mainFrm->SendMessage(WM_USER, ID_USR_EMULCLOSE);

	// by NMH
	m_mainFrm->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	
	m_mainFrm->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REMOVECHILD, 0), (LPARAM)this);
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if (!CMDIChildWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_OVERLAPPEDWINDOW;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_view.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CMDIChildWndEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기


int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_view.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0(_T("Failed to create view window\n"));
		return -1;
	}
	m_view.m_display = m_display;

	ZeroMemory((char *)&m_mapH, sizeof(_mapH));
	m_mapH.formN    = 0;
	m_mapH.pathsetted = false;
	m_mapH.modified   = false;
	m_mapH.source = m_mapH.compile = false;
	if (lpCreateStruct->lpszName && !strcmp(_T("Procedures"), lpCreateStruct->lpszName))
		m_mapH.mapK = MK_PROCEDURES;

	m_formItem = new CFormItem(&m_mapH);
	m_view.Init(&m_mapH, m_draw, m_palette, m_block, m_dir);
	GetSystemMenu(FALSE)->InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, _T("MAPNAME"));
	return 0;
}


void CChildFrame::OnClose()
{
	if (m_view.m_pEmulateWnd)
	{
		m_mainFrm->EmulatorRun(this);
		return;
	}
	else if (m_bEmulRun)
		return;

	if (!DeleteFiles())
		return;

	CMDIChildWndEx::OnClose();
}


void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 0;
	lpMMI->ptMinTrackSize.y = 0;
	lpMMI->ptMaxTrackSize.x = 10000;
	lpMMI->ptMaxTrackSize.y = 10000;

	CMDIChildWndEx::OnGetMinMaxInfo(lpMMI);
}


void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	if (!IsWindow(m_view.m_hWnd))
		return;

	if (bActivate)
		onActivate();

	if (pDeactivateWnd == (CWnd *)this)
	{
		CRect   tRc;
		struct	_formR*	formR;

		tRc.left  = m_mapH.width;
		tRc.top   = m_mapH.height;
		tRc.right = tRc.bottom = 0;

		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block)
			{
				formR->block = false;
				tRc.UnionRect(tRc, getFormRect(ii));
			}
		}

		if (!tRc.IsRectEmpty())
		{
			tRc.InflateRect(5, 5);
     			m_view.InvalidateRect(tRc);
		}
	}

	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}


void CChildFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (m_view.m_nIoOdr || m_view.m_nTabOdr)
	{
		m_view.m_nIoOdr  = 0;
		m_view.m_nTabOdr = 0;
		m_view.RedrawView();
	}

	if (GetSelectedForm() != -1)
	{
		SetSelectForm(-1);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
	}
	
	CMDIChildWndEx::OnNcLButtonDown(nHitTest, point);
}


void CChildFrame::OnSetFocus(CWnd* pOldWnd)
{
	CMDIChildWndEx::OnSetFocus(pOldWnd);
	m_view.SetFocus();
}


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{	
	if (nType == 100 && cx < 100)
		SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);

	CMDIChildWndEx::OnSize(nType, cx, cy);
}


void CChildFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	if (pRect->right - pRect->left <= 120)
	{
		if (GetStyle() & WS_CAPTION)
			ModifyStyle(WS_CAPTION, NULL, SWP_DRAWFRAME);
	}
	else
	{
		if (!(GetStyle() & WS_CAPTION))
			ModifyStyle(NULL, WS_CAPTION, SWP_DRAWFRAME);
	}

	CMDIChildWndEx::OnSizing(fwSide, pRect);
}


void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		if (m_view.m_pEmulateWnd)
		{
			m_mainFrm->EmulatorRun(this);
			return;
		}
		else if (m_bEmulRun)
			return;
	}
	CMDIChildWndEx::OnSysCommand(nID, lParam);
}

BOOL CChildFrame::Create(CString wName, CRect rect,  CAmDraw* draw, CAmPalette* palette, int display)
{
	m_draw = draw;
	m_palette = palette;
	m_display = display;
	return CMDIChildWndEx::Create(NULL, wName, WS_SYSMENU|WS_CHILD|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN, rect, m_mainFrm);
}

void CChildFrame::SetFontSize(int point, WORD style, CString name)
{
	m_mapH.fPoint = point;
	m_mapH.fStyle = style;
	CopyMemory(m_mapH.fName, (char *)name.operator LPCTSTR(), name.GetLength());
}

void CChildFrame::SetVisible(bool showFORM)
{
	m_view.ShowForm(showFORM);
}

void CChildFrame::SetNew(bool newWindow)
{
	m_view.SetNew(newWindow);
}

void CChildFrame::OnDrop(int kind, CPoint pt)
{
	m_view.OnDrop(kind, pt);
}

bool CChildFrame::Copy()
{
	if (m_mapH.mapK == MK_PROCEDURES)
		return true;

	int	selF, nGroup;
	CArray	<int, int>	formArr;
	
	selF = m_view.m_selF;
	
	if ((m_view.m_block->count == 1 || m_view.m_block->count == 0) && selF >= 0)
	{
		nGroup = (int)m_formItem->GetForm(selF)->group;
		if (nGroup)
			getGroupNum(nGroup, formArr);
		else
			formArr.Add(selF);
	}
	else
	{
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			if (m_formItem->GetForm(ii)->block)
			{
				nGroup = (int)m_formItem->GetForm(ii)->group;
				if (nGroup)
					getGroupNum(nGroup, formArr);
				else
					formArr.Add(ii);
			}
		}
	}

	CFormItem formItem(&m_mainFrm->m_clipMAP);
	formItem.RemoveAll();
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, MAKEWORD(1, 0));

	for (int ii = 0; ii < formArr.GetSize(); ii++)
	{
		int	idx;

		struct _formR *formR = (struct _formR *)malloc(sizeof(_formR));
		copyForm(formR, m_formItem->GetForm(formArr.GetAt(ii)));
		idx = formItem.AddForm(formR);

		switch (m_formItem->GetForm(formArr.GetAt(ii))->kind)
		{
		case FM_GRID:
		case FM_GRIDEX:
		case FM_TABLE:
			formItem.GetForm(idx)->auxR = copyCell(m_formItem->GetForm(formArr.GetAt(ii))->auxR);
			break;
		}
	}
	formArr.RemoveAll();
	if (m_view.m_selF >= 0 && m_formItem->GetForm(m_view.m_selF)->kind == FM_TAB)
		m_view.m_selF = -1;

	return true;
}

void CChildFrame::Paste()
{
	if (m_mapH.mapK == MK_PROCEDURES)
		return;

	int	fN, nGroup = 0, kGroup = 0, startFN;
	CRect	tRc, pRc;
	char	prefix[L_SGID+1];
	struct	_formR *tabN = NULL;
	CFormItem	formItem(&m_mainFrm->m_clipMAP);

	m_mapH.modified = true;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	startFN = m_formItem->GetCount();

	m_view.GetClientRect(&tRc);
	pRc.SetRect(tRc.Width(), tRc.Height(), 0, 0);

	for (int ii = 0; ii < startFN; ii++)
		m_formItem->GetForm(ii)->block = false;

	CopyMemory(prefix, m_mapH.mapN, L_SGID);
	prefix[L_SGID] = '\0';
	if (formItem.GetCount() == 1)
	{
		fN = startFN;
		struct _formR *formR = (struct _formR *)malloc(sizeof(_formR));
		copyForm(formR, formItem.GetForm(0));

		tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
		pRc.left = __min(pRc.left, tRc.left);
		pRc.top  = __min(pRc.top,  tRc.top);

		tRc.OffsetRect(10, 10);
		formR->left   = formItem.GetForm(0)->left   = tRc.left;
		formR->top    = formItem.GetForm(0)->top    = tRc.top;
		formR->right  = formItem.GetForm(0)->right  = tRc.right;
		formR->bottom = formItem.GetForm(0)->bottom = tRc.bottom;

		pRc.right  = __max(pRc.right,  tRc.right);
		pRc.bottom = __max(pRc.bottom, tRc.bottom);
		if (formR->index)
		{
			m_view.SortTabOrder();
			formR->index = ++m_view.m_nSeqTabOdr;
		}

		m_formItem->AddForm(formR);
		
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			if (!m_formItem->GetFormStr(ii, _T("name")).IsEmpty() && fN != ii && 
					!m_formItem->GetFormStr(ii, _T("name")).Compare(m_formItem->GetFormStr(fN, _T("name"))))
				m_formItem->SetFormStr(fN, _T("name"), m_view.GetInitSymbol());
		}

		m_formItem->SetFormStr(fN, _T("scpN"), _T(""));
		if (formItem.GetForm(0)->scpN != NULL)
		{
			char	fileName[1024];

			wccGetTempName(fileName, prefix);
			m_formItem->SetFormStr(fN, _T("scpN"), fileName);
			CopyFile(formItem.GetForm(0)->scpN, fileName, false);
		}

		switch (formItem.GetForm(0)->kind)
		{
		case FM_GRID:
		case FM_GRIDEX:
		case FM_TABLE:
			m_formItem->GetForm(fN)->auxR = copyCell(formItem.GetForm(0)->auxR);
			break;
		}
	}
	else
	{
		CString symbolName = _T("");

		for (int kk = 0; kk < m_formItem->GetCount(); kk++)
		{
			symbolName += m_formItem->GetFormStr(kk, _T("name"));
			symbolName += _T('\t');
		}

		for (int ii = 0; ii < formItem.GetCount(); ii++)
		{
			fN = m_formItem->GetCount();
			struct _formR *formR = (struct _formR *)malloc(sizeof(_formR));
			struct _formR *pSrcFormR = formItem.GetForm(ii);
			copyForm(formR, pSrcFormR);
			
			tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);

			pRc.left = __min(pRc.left, formR->left);
			pRc.top  = __min(pRc.top,  formR->top);

			tRc.OffsetRect(10, 10);
			formR->left   = pSrcFormR->left   = tRc.left;
			formR->top    = pSrcFormR->top    = tRc.top;
			formR->right  = pSrcFormR->right  = tRc.right;
			formR->bottom = pSrcFormR->bottom = tRc.bottom;

			pRc.right  = __max(pRc.right,  tRc.right);
			pRc.bottom = __max(pRc.bottom, tRc.bottom);

			if (formR->index)
			{
				m_view.SortTabOrder();
				formR->index = ++m_view.m_nSeqTabOdr;
			}

			m_formItem->AddForm(formR);
			if (m_formItem->GetForm(fN)->group)
			{
				if (kGroup == (int)m_formItem->GetForm(fN)->group)
					m_formItem->GetForm(fN)->group = (struct _formR*)nGroup;
				else
				{
					kGroup = (int)m_formItem->GetForm(fN)->group;
					nGroup = (int)m_formItem->GetForm(fN);
					m_formItem->GetForm(fN)->group = (struct _formR*)nGroup;
				}
			}

			if (symbolName.Find(m_formItem->GetFormStr(fN, _T("name")) + _T("\t")) == 0 ||
					symbolName.Find(_T("\t") + m_formItem->GetFormStr(fN, _T("name")) + _T("\t")) > 0)
				m_formItem->SetFormStr(fN, _T("name"), m_view.GetInitSymbol());

			symbolName += m_formItem->GetFormStr(fN, _T("name"));
			symbolName += _T('\t');
			m_formItem->SetFormStr(fN, _T("scpN"), _T(""));

			if (pSrcFormR->scpN != NULL)
			{
				char	fileName[1024];

				wccGetTempName(fileName, prefix);
				m_formItem->SetFormStr(fN, _T("scpN"), fileName);
				CopyFile(pSrcFormR->scpN, fileName, false);
			}

			switch (pSrcFormR->kind)
			{
			case FM_GRID:
			case FM_GRIDEX:
			case FM_TABLE:
				m_formItem->GetForm(fN)->auxR = copyCell(pSrcFormR->auxR);
				break;
			}
		}
	}

	//pRc.InflateRect(3, 3);
	m_view.m_selF = max(0, startFN);	
	m_view.Invalidate();
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

void CChildFrame::Cut()
{
	int	selF = m_view.m_selF;
	if (Copy())
	{
		m_view.m_selF = selF;
		m_view.DeleteFORMs();
	}
}

BOOL CChildFrame::IsCopyEnable()
{
	if (m_view.m_selF >= 0 || m_view.m_block->count != 0 || m_mapH.mapK == MK_PROCEDURES)
		return TRUE;

	return FALSE;
}

BOOL CChildFrame::IsEditEnable(UINT cmdID)
{
	// modify 2013. 12. 04.
	int  selF = m_view.m_selF;
	BOOL	enable = FALSE;


	return enable;
}

void CChildFrame::DrawForm(UINT cmdID, CString param)
{
	m_view.DrawForm(cmdID, param);
}

void CChildFrame::AlignForms(UINT cmdID)
{
	m_view.AlignForms(cmdID);
}

int CChildFrame::EditForm(UINT cmdID)
{
	return 0;
}

bool CChildFrame::DeleteFiles()
{
	if (m_bEmulRun || m_view.m_pEmulateWnd)
		return true;

	if (!this->m_hWnd)
		return true;

	if (m_delete)
		return true;

	if (m_mapH.modified)
	{
		if (MessageBox(m_mapH.mapN + CString("\n\nIt may be damaged unless you store this Form.\n\nDo you want to close this Form?"), 
				_T("Message"), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return false;
	}

	CString	tmps;

	m_delete = true;
	if (m_mapH.mapN[0] != NULL)
	{
		if (m_mapH.source)
		{
			wccSetTempPath(tmps, m_mapH.mapN);// ljh
			DeleteFile(tmps + ".xml");
		}

		if (m_mapH.compile)
		{
			wccSetTempPath(tmps, m_mapH.mapN, false);
			DeleteFile(tmps);
		}

		if (m_mapH.onLoad)
			DeleteFile(m_mapH.onLoadN);
		if (m_mapH.onSend)
			DeleteFile(m_mapH.onSendN);
		if (m_mapH.onReceive)
			DeleteFile(m_mapH.onReceiveN);
		if (m_mapH.onAlert)
			DeleteFile(m_mapH.onAlertN);
		if (m_mapH.onTimer)
			DeleteFile(m_mapH.onTimerN);
		if (m_mapH.onClose)
			DeleteFile(m_mapH.onCloseN);
		if (m_mapH.onDevice)
			DeleteFile(m_mapH.onDeviceN);
		if (m_mapH.onError)
			DeleteFile(m_mapH.onErrorN);
		if (m_mapH.onFlicking)
			DeleteFile(m_mapH.onFlickingN);
		if (m_mapH.onChangeLayout)
			DeleteFile(m_mapH.onChangeLayoutN);
		if (m_mapH.onDeclaration)
			DeleteFile(m_mapH.onDeclarationN);
		if (m_mapH.onInDeclaration)
			DeleteFile(m_mapH.onInDeclarationN);
	}

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		if (!m_formItem->GetFormStr(ii, _T("scpN")).IsEmpty())
			DeleteFile(m_formItem->GetFormStr(ii, _T("scpN")));
	}

	m_delete = false;
	return true;
}

CString CChildFrame::GetBookmarkSrc(int idx)
{
	return m_bookmarks.GetAt(idx);
}

int CChildFrame::GetBookmarkCount()
{
	return (int)m_bookmarks.GetCount();
}

void CChildFrame::SetBookmark(CString src)
{
	m_bookmarks.Add(src);
}

void CChildFrame::SetSelectForm(int idx, bool bMulti)
{
	if (!bMulti)
	{
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
			m_formItem->GetForm(ii)->block = false;
		m_view.m_block->count = 0;
		m_view.m_selF = idx;
		m_view.m_stdF = -1;
	}
	else
	{
		m_formItem->GetForm(idx)->block = true;
		if (m_view.m_selF >= 0 && m_view.m_block->count == 0)
		{
			m_view.m_block->count++;
			m_formItem->GetForm(m_view.m_selF)->block = true;
		}
		m_view.m_block->count++;
		if (m_view.m_stdF < 0)
			m_view.m_stdF = idx;
	}
	m_view.m_nIoOdr = 0;
	m_view.m_nTabOdr = 0;
	m_view.RedrawView();
}

int CChildFrame::GetSelectedForm()
{
	if (m_view.m_block->count == 0 || m_view.m_block->count == 1)
		return m_view.m_selF;
	else
		return -2;
}

bool CChildFrame::IsLayoutMode()
{
	// Layout : 현재 상태에 해당하는 Properties 세팅
	if (m_view.m_bLayoutMode)
		return true;

	return false;
}

void CChildFrame::ReloadColors()
{
	if (m_mapH.mapK == MK_PROCEDURES)
	{
		m_view.m_pSCEdit->LoadColors();
		m_view.m_pSCEdit->Invalidate();
	}
}

void CChildFrame::DeleteUndoMap()
{
	CMapPtrToPtr* pUndoMap = (CMapPtrToPtr*)m_mapH.pUndoMap;
	if (pUndoMap)
	{
		for (POSITION pos = pUndoMap->GetStartPosition(); pos;)
		{
			void*	key;
			CObject* pArr;

			pUndoMap->GetNextAssoc(pos, key, (void*&)pArr);
			delete pArr;
		}
		pUndoMap->RemoveAll();
		delete pUndoMap;
		m_mapH.pUndoMap = NULL;
	}
}

void CChildFrame::DeleteSelForms()
{
	m_view.DeleteFORMs();
}

void CChildFrame::LoadProcedures()
{
	if (!m_view.m_pSCEdit)
		return;

	CFile	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;
	CString dir = m_mapH.onDeclarationN;

	m_view.m_pSCEdit->SetScriptText(_T(""));

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
	sbuf[scpSize] = '\0';
	file.Close();
	if (scpSize != dwRead)
	{
		delete[] sbuf;
		return;
	}

	m_view.m_pSCEdit->SetScriptText(sbuf);
	delete[] sbuf;
}

void CChildFrame::ChangeKind()
{
	m_view.ChangeKind();
}

void CChildFrame::SaveProcedures()
{
	CFile	file;
	CString	sData;

	sData = m_view.m_pSCEdit->GetScriptText();
	if (sData.GetLength() <= 0)
	{
		if (m_mapH.onDeclaration)
			DeleteFile(m_mapH.onDeclarationN);
		m_mapH.onDeclarationN[0] = _T('\0');
		m_mapH.onDeclaration = false;
		return;
	}

	if (m_mapH.onDeclarationN[0] == _T('\0'))
	{
		char	prefix[L_SGID+1];

		CopyMemory(prefix, m_mapH.mapN, L_SGID);
		prefix[L_SGID] = _T('\0');
		wccGetTempName(m_mapH.onDeclarationN, prefix);
	}
	m_mapH.onDeclaration = true;
	
	if (sData.GetLength() > 0)
	{
		if (!file.Open(m_mapH.onDeclarationN, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			AfxMessageBox(_T("XError [d2]"));
			return;
		}
		file.Write((char *)sData.operator LPCSTR(), sData.GetLength());
		file.Close();
	}
}

BOOL CChildFrame::IsRedoEnable()
{
	return (m_redos.GetSize() ? TRUE : FALSE);
}

BOOL CChildFrame::IsUndoEnable()
{
	return (m_undos.GetSize() ? TRUE : FALSE);
}

BOOL CChildFrame::PopRedo(bool isAll)
{
	PushUndo();

	if (!m_redos.GetSize())
		return FALSE;

	if (isAll)
	{
		for (int ii = 0; ii < m_redos.GetCount(); ii++)
			delete (CUndoRedo*)m_redos.GetAt(ii);
		
		m_redos.RemoveAll();
		return TRUE;
	}

	class CUndoRedo* urdo = (class CUndoRedo *) m_redos.GetAt(0);

	CopyMemory(&m_mapH, &urdo->mx_mapH, sizeof(_mapH));
	m_formItem->RemoveAll(true);
	delete m_formItem;
	m_formItem = new CFormItem(&m_mapH, urdo);
	m_redos.RemoveAt(0); 	

	return TRUE;
}

BOOL CChildFrame::PopUndo(bool bflag)
{
	if (m_undos.GetSize() == 0)
		return FALSE;

	class CUndoRedo* urdo = (class CUndoRedo *) m_undos.GetAt(0);
	
	if (bflag)
	{
		PushRedo();
		CopyMemory(&m_mapH, &urdo->mx_mapH, sizeof(_mapH));
		//m_formItem = new CFormItem(&m_mapH, urdo);
		m_formItem->RemoveAll(true);
		delete m_formItem;

		m_formItem = new CFormItem(&m_mapH, urdo);
	}
	else 
	{
		delete urdo;
	}

	m_undos.RemoveAt(0); 	
	
	return TRUE;
}

void CChildFrame::PushRedo(CUndoRedo* urdo)
{
	if (urdo == NULL)
		urdo = new CUndoRedo(&m_mapH, m_formItem);

	m_redos.InsertAt(0, urdo);
	if (m_redos.GetSize() > MAX_UNDO)
	{
		delete (CUndoRedo*) m_redos.GetAt(m_redos.GetSize() - 1);
		m_redos.RemoveAt(m_redos.GetSize() - 1);
	}
}

void CChildFrame::PushUndo(CUndoRedo* urdo)
{
	if (urdo == NULL)
		urdo = new CUndoRedo(&m_mapH, m_formItem);

	m_undos.InsertAt(0, urdo);	
	if (m_undos.GetSize() > MAX_UNDO)
	{
		delete (CUndoRedo*)m_undos.GetAt(m_undos.GetSize() - 1);
		m_undos.RemoveAt(m_undos.GetSize() - 1);
	}
}

void CChildFrame::ReleaseFocus()
{
	if (m_view.m_nTabOdr)
	{
		m_view.m_nTabOdr = 0;
		m_view.Invalidate();
	}
}

void CChildFrame::OnColumnEdit()
{
	m_view.OnViewCommand(ID_AUX_COLUMNEDIT);
}

void CChildFrame::onActivate()
{
	if (!m_view)
		return;

	CString	title = _T("AXIS Builder");
	CString	tmps;

	if (m_mapH.mapN[0] != NULL)
	{
		tmps.Format(_T("[%s]"), m_mapH.mapN);
		title += tmps;
	}
	m_mainFrm->SetWindowText(title);
	m_view.m_block->count = 0;
	m_mainFrm->SendMessage(WM_USER, ID_USR_TRLAYOUT, NULL);
	m_mainFrm->SendMessage(WM_USER, ID_USR_RTSINFO, NULL);
	m_mainFrm->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
	m_mainFrm->SendMessage(WM_USER, ID_USR_SETFINDCAPTION, NULL);
	if (m_view.m_pEmulateWnd)
		m_mainFrm->SendMessage(WM_USER, ID_USR_ENABLEPROP, FALSE);
	else
		m_mainFrm->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
}

void CChildFrame::getGroupNum(int nGroup, CArray <int, int> &arr)
{
	struct	_formR*	formR;

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->group == (struct _formR*)nGroup)
			arr.Add(ii);
	}
}

char* CChildFrame::copyCell(char* idx)
{
	int	count, nLen;
	char*	auxR = new char[sizeof(_repR)];
	struct _repR*	repR = (struct _repR *)auxR;
	struct _repR*	cRepR = (struct _repR *)idx;

	CopyMemory(repR, cRepR, sizeof(struct _repR));
	CCellItem cellItem(cRepR);
	repR->cellR = NULL;
	CCellItem tCellItem(repR);

	count = cellItem.GetCount();
	for (int ii = 0; ii < count; ii++)
	{
		struct _cellR* cellR = (struct _cellR *)malloc(sizeof(_cellR));
		struct _cellR* cellS = cellItem.GetCell(ii);
		CopyMemory(cellR, cellS, sizeof(_cellR));
		if (cellS->fName)
		{
			nLen = strlen(cellS->fName)+1;
			cellR->fName = new char[nLen];
			strcpy_s(cellR->fName, nLen, cellS->fName);
		}
		if (cellS->margin && cellS->margin->GetCount() > 0)
		{
			cellR->margin = new CStringArray;
			cellR->margin->Copy(*cellS->margin);
		}
		tCellItem.AddCell(cellR);
	}
	return auxR;
}

void CChildFrame::copyForm(_formR *targetR, _formR *sourceR)
{
	int	len;

	CopyMemory(targetR, sourceR, sizeof(_formR));
	targetR->name = NULL;
	if (sourceR->name)
	{
		len = (int)strlen(sourceR->name);
		targetR->name = new char[len + 1];
		CopyMemory(targetR->name, sourceR->name, len);
		targetR->name[len] = _T('\0');
	}

	targetR->fName = NULL;
	if (sourceR->fName)
	{
		len = (int)strlen(sourceR->fName);
		targetR->fName = new char[len + 1];
		CopyMemory(targetR->fName, sourceR->fName, len);
		targetR->fName[len] = _T('\0');
	}

	targetR->desc = NULL;
	if (sourceR->desc)
	{
		len = (int)strlen(sourceR->desc);
		targetR->desc = new char[len + 1];
		CopyMemory(targetR->desc, sourceR->desc, len);
		targetR->desc[len] = _T('\0');
	}

	targetR->auxS = NULL;
	if (sourceR->auxS)
	{
		len = (int)strlen(sourceR->auxS);
		targetR->auxS = new char[len + 1];
		CopyMemory(targetR->auxS, sourceR->auxS, len);
		targetR->auxS[len] = _T('\0');
	}

	targetR->scpN = NULL;
	if (sourceR->scpN && strlen(sourceR->scpN))
	{
		CFileFind finder;
		if (!finder.FindFile(sourceR->scpN))
		{
			delete sourceR->scpN;	
			sourceR->scpN = NULL;
		}
		else
		{
			char	prefix[L_SGID+1], fileName[1024];

			CopyMemory(prefix, m_mapH.mapN, L_SGID);
			prefix[L_SGID] = _T('\0');
			wccGetTempName(fileName, prefix);
			if (!CopyFile(sourceR->scpN, fileName, TRUE))
				AfxMessageBox(_T("file access Error [X1]"));
			
			len = (int)strlen(fileName);
			targetR->scpN = new char[len + 1];
			CopyMemory(targetR->scpN, fileName, len);
			targetR->scpN[len] = _T('\0');
		}
	}

	targetR->editS = NULL;
	if (sourceR->editS)
	{
		len = (int)strlen(sourceR->editS);
		targetR->editS = new char[len + 1];
		CopyMemory(targetR->editS, sourceR->editS, len);
		targetR->editS[len] = _T('\0');
	}

	targetR->str = NULL;
	if (sourceR->str)
	{
		len = (int)strlen(sourceR->str);
		targetR->str = new char[len + 1];
		CopyMemory(targetR->str, sourceR->str, len);
		targetR->str[len] = _T('\0');
	}

	targetR->str2 = NULL;
	if (sourceR->str2)
	{
		len = (int)strlen(sourceR->str2);
		targetR->str2 = new char[len + 1];
		CopyMemory(targetR->str2, sourceR->str2, len);
		targetR->str2[len] = _T('\0');
	}

	targetR->dat = NULL;
	if (sourceR->dat)
	{
		len = (int)strlen(sourceR->dat);
		targetR->dat = new char[len + 1];
		CopyMemory(targetR->dat, sourceR->dat, len);
		targetR->dat[len] = _T('\0');
	}

	targetR->rtssy = NULL;
	if (sourceR->rtssy)
	{
		len = (int)strlen(sourceR->rtssy);
		targetR->rtssy = new char[len + 1];
		CopyMemory(targetR->rtssy, sourceR->rtssy, len);
		targetR->rtssy[len] = _T('\0');
	}

	targetR->optionimg = NULL;
	if (sourceR->optionimg)
	{
		len = (int)strlen(sourceR->optionimg);
		targetR->optionimg = new char[len + 1];
		CopyMemory(targetR->optionimg, sourceR->optionimg, len);
		targetR->optionimg[len] = _T('\0');
	}

	targetR->refS = NULL;
	if (sourceR->refS)
	{
		len = (int)strlen(sourceR->refS);
		targetR->refS = new char[len + 1];
		CopyMemory(targetR->refS, sourceR->refS, len);
		targetR->refS[len] = _T('\0');
	}

	targetR->help = NULL;
	if (sourceR->help)
	{
		len = (int)strlen(sourceR->help);
		targetR->help = new char[len + 1];
		CopyMemory(targetR->help, sourceR->help, len);
		targetR->help[len] = _T('\0');
	}

	targetR->optionimg2 = NULL;
	if (sourceR->optionimg2)
	{
		len = (int)strlen(sourceR->optionimg2);
		targetR->optionimg2 = new char[len + 1];
		CopyMemory(targetR->optionimg2, sourceR->optionimg2, len);
		targetR->optionimg2[len] = _T('\0');
	}

	targetR->optionimg3 = NULL;
	if (sourceR->optionimg3)
	{
		len = (int)strlen(sourceR->optionimg3);
		targetR->optionimg3 = new char[len + 1];
		CopyMemory(targetR->optionimg3, sourceR->optionimg3, len);
		targetR->optionimg3[len] = _T('\0');
	}

	targetR->margin = NULL;
	if (sourceR->margin && sourceR->margin->GetCount() > 0)
	{
		targetR->margin = new CStringArray;
		targetR->margin->Copy(*sourceR->margin);
	}

	// layout
	CString sKey;
	struct _Layout *layout, *layoutTmp;
	targetR->layout = new CMapStringToPtr;
	targetR->layout->RemoveAll();
	for (POSITION pos = sourceR->layout->GetStartPosition(); pos;)
	{
		layout = new _Layout;
		sourceR->layout->GetNextAssoc(pos, sKey, (void*&)layoutTmp);

		layout->bfirst	= false;
		layout->properties = layoutTmp->properties;
		layout->left	= layoutTmp->left;
		layout->right	= layoutTmp->right;
		layout->top	= layoutTmp->top;
		layout->bottom	= layoutTmp->bottom;
												
		targetR->layout->SetAt(sKey, (void*&)layout);
	}
}

CRect CChildFrame::getFormRect(int idx)
{
	struct	_formR*	formR = m_formItem->GetForm(idx);

	if (formR->group)
		return groupRect(formR->group);

	return CRect(formR->left, formR->top, formR->right, formR->bottom);
}

CRect CChildFrame::groupRect(struct _formR* nGroup)
{
	CRect	rc, tRc;
	struct	_formR*	formR;

	rc.SetRectEmpty();
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->group == (struct _formR*)nGroup)
		{
			tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
			rc.UnionRect(rc, tRc);
		}
	}
	return rc;
}

LRESULT CChildFrame::OnNcHitTest(CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);		
	if (m_bEmulRun && !(rc.top < (point.y + 24) && rc.top + 24 > (point.y + 24)))
		return 0;

	return CMDIChildWndEx::OnNcHitTest(point);
}
