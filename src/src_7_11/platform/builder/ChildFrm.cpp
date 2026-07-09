// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "axisWork.h"
#include "resource.h"
#include "childFrm.h"
#include "mainFrm.h"
#include "mapvar.h"
#include "mainvar.h"

#include "awDlg/coRGB.h"
#include "awDlg/format.h"
#include "awWcc/libwcc.h"
#include "../dll/lib/axislib.h"
#include "../h/axisvar.h"
#include "awCommon/CellItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_WM_CLOSE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZING()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_SETTEXT, HandleSetText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame(CMainFrame* frame, CAxisDraw* draw, CString curdir, struct _block* block)
{
	m_block   = block;
	m_mainFRM = frame;
	m_draw    = draw;
	m_dir     = curdir;

	m_delete  = false;
	m_bSDI    = FALSE;

	m_pPopFrame   = NULL;
	m_pVirtualWnd = NULL;
}

CChildFrame::~CChildFrame()
{
	FreePopFrame();
	DeleteUndoMap();
	m_undoArr.RemoveAll();
	m_redoArr.RemoveAll();
	m_bookmarkArr.RemoveAll();
	m_pFormItem->RemoveAllForm();
	delete m_pFormItem;

	m_mainFRM->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
	m_mainFRM->RemoveChild(m_myKey);
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_OVERLAPPEDWINDOW;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWnd *parentWnd = this;
	if (m_bSDI)
	{
		m_pVirtualWnd = new CWnd;
		
		m_pPopFrame = new CPopFrame();
		CString strWndClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
			::LoadCursor(NULL, IDC_ARROW),
			NULL,
			GetIcon(FALSE));
		
		m_pVirtualWnd->CreateEx(WS_EX_TOOLWINDOW, strWndClass, "virtual", NULL, CRect(0, 0, 0, 0), NULL, NULL);
		m_pVirtualWnd->SetOwner(GetMDIFrame());
		m_pPopFrame->SetParent((CChildFrame*)this, m_pVirtualWnd);
		m_pPopFrame->Create(strWndClass, "Child", WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME, CRect(0, 0, 0, 0), NULL,NULL,0,NULL);
		m_pPopFrame->SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW);
		m_pPopFrame->SetOwner(this);
		m_pPopFrame->m_mainFRM = m_mainFRM;

		parentWnd = m_pPopFrame;
		
	}
	m_view.m_pDisplayMode = m_pDisplayMode;
	if (!m_view.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), parentWnd, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_view.m_pChildFrame = this;

	ZeroMemory((char *)&m_mapH, sizeof(_mapH));
	m_mapH.mapGroup = 1;

	m_mapH.type       = 4;
	m_mapH.formN      = 0;
	m_mapH.pathsetted = false;
	m_mapH.modified   = false;
	m_mapH.source     = m_mapH.compile = false;
	if (lpCreateStruct->lpszName && !strcmp("Procedures", lpCreateStruct->lpszName))
		m_mapH.mapK = MK_PROCEDURES;

	m_pFormItem = new CFormItem(&m_mapH);

	m_view.m_draw = m_draw;
	m_view.Init(&m_mapH, m_block, m_dir);

	GetSystemMenu(FALSE)->InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, "MAPNAME");

	return 0;	
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_bSDI)
	{
		return;
	}
	CMDIChildWnd::OnSetFocus(pOldWnd);
	m_view.SetFocus();
}

BOOL CChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (m_bSDI)
		return m_pPopFrame->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);

	if (m_view.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	if (!m_view.m_hWnd)
		return;
	CString	title = _T("AXIS Builder");
	CString	tmps;

	if (bActivate)
	{
		OnActivateMe();
	}
	if (pDeactivateWnd == (CWnd *)this)
	{
		CRect   tRc;
		tRc.right = tRc.bottom = 0;
		tRc.left  = m_mapH.width;
		tRc.top   = m_mapH.height;

		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				m_pFormItem->getFormPoint(ii)->block = false;
				tRc.UnionRect(tRc, getFormRect(ii));
			}
		}

		if (!tRc.IsRectEmpty())
		{
			tRc.InflateRect(5, 5);
     			m_view.InvalidateRect(tRc);
		}
		m_mainFRM->SendMessage(WM_USER, ID_USR_ERASECTRL, 0);
	}
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}

void CChildFrame::SetFontSize(int point, WORD style, CString name)
{
	m_mapH.fPoint  = point;
	m_mapH.fStyle  = style;
	CopyMemory(m_mapH.fName, (char *)name.operator LPCTSTR(), name.GetLength());
}

void CChildFrame::SetPalette(CAxisPalette* palette)
{
	m_view.m_palette = palette;
}

void CChildFrame::SetVisible(bool showFORM)
{
	CRect	tRc;

	m_view.m_showFORM = showFORM;
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (!(m_pFormItem->getFormPoint(ii)->properties & PR_VISIBLE))
		{
			tRc = getFormRect(ii);
			m_view.InvalidateRect(tRc);
                }
        }
}

void CChildFrame::SetNew(bool newWindow)
{
	m_view.m_new = newWindow;
}

void CChildFrame::OnDrop(CPoint pt)
{
	m_view.OnDrop(pt);
}

BOOL CChildFrame::IsCopyEnable()
{
	if (m_view.m_selF >= 0 || m_view.m_block->count != 0 || m_mapH.mapK == MK_PROCEDURES)
		return TRUE;

	return FALSE;
}

void CChildFrame::CopyForm(_formR *targetR, _formR *sourceR)
{
	CopyMemory(targetR, sourceR, sizeof(_formR));
	targetR->name = NULL;
	if (sourceR->name)
	{
		targetR->name = new char[strlen(sourceR->name) + 1];
		_tcscpy_s(targetR->name, strlen(sourceR->name) + 1, sourceR->name);
	}
	targetR->fName = NULL;
	if (sourceR->fName)
	{
		targetR->fName = new char[strlen(sourceR->fName) + 1];
		_tcscpy_s(targetR->fName, strlen(sourceR->fName) + 1, sourceR->fName);
	}
	targetR->help = NULL;
	if (sourceR->help)
	{
		targetR->help = new char[strlen(sourceR->help) + 1];
		_tcscpy_s(targetR->help, strlen(sourceR->help) + 1, sourceR->help);
	}
	targetR->auxS = NULL;
	if (sourceR->auxS)
	{
		targetR->auxS = new char[strlen(sourceR->auxS) + 1];
		_tcscpy_s(targetR->auxS, strlen(sourceR->auxS) + 1, sourceR->auxS);
	}
	targetR->scpN = NULL;
	if (sourceR->scpN && strlen(sourceR->scpN))
	{
		CFileFind finder;
		if (!finder.FindFile(sourceR->scpN))
		{
			strcpy_s(sourceR->scpN, 2, "");
		}
		else
		{
			char	prefix[L_SGID+1], fileName[1024];
			CopyMemory(prefix, m_mapH.mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			wccGetTempName(fileName, prefix);
			if (!CopyFile(sourceR->scpN, fileName, TRUE))
				AfxMessageBox("file access Error [X1]");
			
			targetR->scpN = new char[strlen(fileName) + 1];
			
			//strcpy_s(targetR->scpN, fileName);
		}
	}
	targetR->editS = NULL;
	if (sourceR->editS)
	{
		targetR->editS = new char[strlen(sourceR->editS) + 1];
		_tcscpy_s(targetR->editS, strlen(sourceR->editS) + 1, sourceR->editS);
	}
	targetR->refS = NULL;
	if (sourceR->refS)
	{
		targetR->refS = new char[strlen(sourceR->refS) + 1];
		_tcscpy_s(targetR->refS, strlen(sourceR->refS) + 1, sourceR->refS);
	}
	targetR->image = NULL;
	if (sourceR->image)
	{
		targetR->image = new char[strlen(sourceR->image) + 1];
		_tcscpy_s(targetR->image, strlen(sourceR->image) + 1, sourceR->image);
	}
	targetR->str = NULL;
	if (sourceR->str)
	{
		targetR->str = new char[strlen(sourceR->str) + 1];
		_tcscpy_s(targetR->str, strlen(sourceR->str) + 1, sourceR->str);
	}
	targetR->str2 = NULL;
	if (sourceR->str2)
	{
		targetR->str2 = new char[strlen(sourceR->str2) + 1];
		_tcscpy_s(targetR->str2, strlen(sourceR->str2) + 1, sourceR->str2);
	}
	targetR->dat = NULL;
	if (sourceR->dat)
	{
		targetR->dat = new char[strlen(sourceR->dat) + 1];
		_tcscpy_s(targetR->dat, strlen(sourceR->dat) + 1, sourceR->dat);
	}
}

char* CChildFrame::CopyCell(char* idx)
{
	int cellCnt = 0;
	char* auxR = new char[sizeof(_repR)];
	struct _repR *repR = (struct _repR *)auxR;
	struct _repR *cRepR = (struct _repR *)idx;
	CopyMemory(repR, cRepR, sizeof(struct _repR));
	CCellItem cellItem(cRepR);
	repR->cellR = NULL;
	CCellItem tCellItem(repR);
	cellCnt = cellItem.getCellCount();
	for (int ii = 0; ii < cellCnt; ii++)
	{
		struct _cellR *cellR = (struct _cellR *)malloc(sizeof(_cellR));
		CopyMemory(cellR, cellItem.getCellPoint(ii), sizeof(_cellR));
		tCellItem.addCell(cellR);
	}
	return auxR;
}

bool CChildFrame::Copy()
{
	if (m_mapH.mapK == MK_PROCEDURES)
	{
		/*m_view.m_pSCEdit->Copy();
		m_view.m_pSCEdit->SetFocus();*/
		return true;
	}
	int	selF, nGroup;
	CArray	<int, int>	formArr;
	bool	bTab = false;
	
	selF = m_view.m_selF;
	
	if ((m_view.m_block->count == 1 || m_view.m_block->count == 0) && selF >= 0)
	{
		nGroup = (int)m_pFormItem->getFormPoint(selF)->group;
		if (nGroup)
			getGroupNum(nGroup, formArr);
		else
		{
			formArr.Add(selF);
			if (m_pFormItem->getFormPoint(selF)->kind == FM_TAB)
				getTabNum(m_pFormItem->getFormPoint(selF), formArr);
		}
	}
	else
	{
		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				nGroup = (int)m_pFormItem->getFormPoint(ii)->group;
				if (nGroup)
					getGroupNum(nGroup, formArr);
				else
				{
					formArr.Add(ii);
					if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
						getTabNum(m_pFormItem->getFormPoint(ii), formArr);
				}
			}
		}
	}

	CFormItem formItem(&m_mainFRM->m_clipMAP);
	formItem.RemoveAllForm();
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, MAKEWORD(1, 0));
	for (int ii = 0; ii < formArr.GetSize(); ii++)
	{
		int idx;
		struct _formR *formR = (struct _formR *)malloc(sizeof(_formR));
		CopyForm(formR, m_pFormItem->getFormPoint(formArr.GetAt(ii)));

		if (formR->kind == FM_TAB)
			bTab = true;
		idx = formItem.addForm(formR);

		if (bTab && !formR->tabN)
			bTab = false;
		/*else if (!bTab)
			formR->tabN = 0;*/

		switch (m_pFormItem->getFormPoint(formArr.GetAt(ii))->kind)
		{
		case FM_GRID:
		case FM_TABLE:
			formItem.getFormPoint(idx)->auxR = CopyCell(m_pFormItem->getFormPoint(formArr.GetAt(ii))->auxR);
			break;
		}
	}
	formArr.RemoveAll();
	if (m_view.m_selF >= 0 && m_pFormItem->getFormPoint(m_view.m_selF)->kind == FM_TAB)
		m_view.m_selF = -1;

	return true;
}

void CChildFrame::Cut()
{
	int selF = m_view.m_selF;
	if (Copy())
	{
		m_view.m_selF = selF;
		m_view.deleteFORMs();
	}
}

void CChildFrame::Paste()
{
	if (m_mapH.mapK == MK_PROCEDURES)
	{
		/*m_view.m_pSCEdit->Paste();
		m_view.m_pSCEdit->SetFocus();*/
		return;
	}
	m_mapH.modified = true;
	int	fN, nGroup = 0, kGroup = 0, startFN, tabFN = -1;
	CRect	tRc, pRc;
	char	prefix[L_SGID+1];
	struct _formR *tabN = NULL;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	CFormItem	formItem(&m_mainFRM->m_clipMAP);

	if (m_view.m_selF >= 0 && m_pFormItem->getFormPoint(m_view.m_selF)->kind == FM_TAB)
		tabFN = m_view.m_selF;

	startFN = m_pFormItem->getFormCount();

	m_view.GetClientRect(&tRc);
	pRc.SetRect(tRc.Width(), tRc.Height(), 0, 0);

	for (int ii = 0; ii < startFN; ii++)
		m_pFormItem->getFormPoint(ii)->block = false;

	CopyMemory(prefix, m_mapH.mapN, L_SGID);
	prefix[L_SGID] = '\0';
	if (formItem.getFormCount() == 1)
	{
		fN = startFN;
		struct _formR *formR = (struct _formR *)malloc(sizeof(_formR));
		CopyForm(formR, formItem.getFormPoint(0));
		
		tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
		pRc.left = __min(pRc.left, tRc.left);
		pRc.top  = __min(pRc.top,  tRc.top);

		tRc.OffsetRect(10, 10);
		formR->left   = formItem.getFormPoint(0)->left   = tRc.left;
		formR->top    = formItem.getFormPoint(0)->top    = tRc.top;
		formR->right  = formItem.getFormPoint(0)->right  = tRc.right;
		formR->bottom = formItem.getFormPoint(0)->bottom = tRc.bottom;

		pRc.right  = __max(pRc.right,  tRc.right);
		pRc.bottom = __max(pRc.bottom, tRc.bottom);
		if (formR->index)
		{
			m_view.sortTabOrder();
			formR->index = ++m_view.m_nSeqTabOdr;
		}

		m_pFormItem->addForm(formR);
		
		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			if (!m_pFormItem->getFormStr(ii, "name").IsEmpty() && 
				fN != ii && 
				!m_pFormItem->getFormStr(ii, "name").Compare(m_pFormItem->getFormStr(fN, "name")))
				m_pFormItem->setFormStr(fN, "name", m_view.getInitSymbol());

		m_pFormItem->setFormStr(fN, "scpN", "");
		if (formItem.getFormPoint(0)->scpN != NULL)
		{
			char fileName[1024];
			wccGetTempName(fileName, prefix);
			m_pFormItem->setFormStr(fN, "scpN", fileName);
			CopyFile(formItem.getFormPoint(0)->scpN, m_pFormItem->getFormStr(fN, "scpN"), false);
		}

		switch (formItem.getFormPoint(0)->kind)
		{
		case FM_GRID:
		case FM_TABLE:
			m_pFormItem->getFormPoint(fN)->auxR = CopyCell(formItem.getFormPoint(0)->auxR);
			break;
		}
	}
	else
	{
		CString symbolName = "";
		for (int kk = 0; kk < m_pFormItem->getFormCount(); kk++)
		{
			symbolName += m_pFormItem->getFormStr(kk, "name");
			symbolName += '\t';
		}
		for (int ii = 0; ii < formItem.getFormCount(); ii++)
		{
			/*if (m_mapH.mapK == MK_USEROB && formItem.getFormPoint(ii)->kind == FM_OBJECT)
				continue;*/

			fN = m_pFormItem->getFormCount();
			struct _formR *formR = (struct _formR *)malloc(sizeof(_formR));
			struct _formR *pSrcFormR = formItem.getFormPoint(ii);
			CopyForm(formR, pSrcFormR);
			
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
				m_view.sortTabOrder();
				formR->index = ++m_view.m_nSeqTabOdr;
			}

			m_pFormItem->addForm(formR);
			if (m_pFormItem->getFormPoint(fN)->group)
			{
				if (kGroup == (int)m_pFormItem->getFormPoint(fN)->group)
					m_pFormItem->getFormPoint(fN)->group = (struct _formR*)nGroup;
				else
				{
					kGroup = (int)m_pFormItem->getFormPoint(fN)->group;
					nGroup = (int)m_pFormItem->getFormPoint(fN);
					m_pFormItem->getFormPoint(fN)->group = (struct _formR*)nGroup;
				}
			}

			if (symbolName.Find(m_pFormItem->getFormStr(fN, "name") + "\t") == 0 ||
				symbolName.Find("\t" + m_pFormItem->getFormStr(fN, "name") + "\t") > 0)
				m_pFormItem->setFormStr(fN, "name", m_view.getInitSymbol());

			symbolName += m_pFormItem->getFormStr(fN, "name");
			symbolName += '\t';

			m_pFormItem->setFormStr(fN, "scpN", "");
			if (pSrcFormR->scpN != NULL)
			{
				char fileName[1024];
				wccGetTempName(fileName, prefix);
				m_pFormItem->setFormStr(fN, "scpN", fileName);
				CopyFile(pSrcFormR->scpN, m_pFormItem->getFormStr(fN, "scpN"), false);
			}

			switch (pSrcFormR->kind)
			{
			case FM_GRID:
			case FM_TABLE:
				m_pFormItem->getFormPoint(fN)->auxR = CopyCell(pSrcFormR->auxR);
				break;
			}
		}
	}

	int ii = 0;
	for (ii = startFN; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (tabN && m_pFormItem->getFormPoint(ii)->tabN)
		{
			struct _formR *tTabN = m_pFormItem->getFormPoint(ii)->tabN;
			if (m_pFormItem->getIdxOfPtr(tTabN) < startFN)
			{
				for (int jj = m_pFormItem->getFormCount() - 1; jj >= ii; jj--)
					if (m_pFormItem->getFormPoint(jj)->tabN == tTabN)
						m_pFormItem->getFormPoint(jj)->tabN = tabN;
			}
		}
		else
		{
			m_pFormItem->getFormPoint(ii)->tabN = 0;
			m_pFormItem->getFormPoint(ii)->pageN = 0;
			tabN = 0;
		}
		
		if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
			tabN = m_pFormItem->getFormPoint(ii);
	}

	if (tabFN >= 0)	//	when tab selected
	{
		struct _formR *tabR = m_pFormItem->getFormPoint(tabFN);
		for (ii = startFN; ii < m_pFormItem->getFormCount(); ii++)
		{
			struct _formR *formR = m_pFormItem->getFormPoint(ii);
			formR->left   -= pRc.left - tabR->left;
			formR->top    -= pRc.top - tabR->top;
			formR->right  -= pRc.left - tabR->left;
			formR->bottom -= pRc.top - tabR->top;
			formR->tabN = tabR;
			formR->pageN = tabR->checked;
		}
	}

	//pRc.InflateRect(3, 3);
	m_view.m_selF = max(0, startFN);
	m_view.Invalidate();
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

BOOL CChildFrame::IsEditEnable(UINT cmdID)
{
	int	selF   = m_view.m_selF;
	BOOL	enable = FALSE;

	if (selF < 0 || m_view.m_block->count > 1)
		return FALSE;

	switch (cmdID)
	{
	case ID_EX_FORMAT:
		if ((m_pFormItem->getFormPoint(selF)->kind == FM_EDIT || m_pFormItem->getFormPoint(selF)->kind == FM_OUT || m_pFormItem->getFormPoint(selF)->kind == FM_COMBO)
			&& (m_pFormItem->getFormPoint(selF)->type == IO_NUM || m_pFormItem->getFormPoint(selF)->type == IO_ALPHA)
			&& (m_pFormItem->getFormPoint(selF)->isEdit == IE_EDIT || m_pFormItem->getFormPoint(selF)->isEdit == IE_NOP))
		{
			enable = TRUE;
		}
		break;

	case ID_EX_RGB:
		if ((m_pFormItem->getFormPoint(selF)->kind == FM_OUT 
			|| (m_pFormItem->getFormPoint(selF)->kind == FM_EDIT && m_pFormItem->getFormPoint(selF)->iok == EIO_INOUT))
			&& (m_pFormItem->getFormPoint(selF)->isEdit == IE_CORGB || m_pFormItem->getFormPoint(selF)->isEdit == IE_NOP))
		{
			enable = TRUE;
		}
		break;

	default:
		break;
	}

	return enable;
}

void CChildFrame::DrawForm(UINT cmdID)
{
	m_view.DrawForm(cmdID);
}

void CChildFrame::AlignForms(UINT cmdID)
{
	m_view.AlignForms(cmdID);
}

int CChildFrame::EditForm(UINT cmdID)
{
	int	selF;

	selF = m_view.m_selF;
	if (selF < 0 || m_view.m_block->count > 1)
		return IE_NOP;

	struct _formR *formR = m_pFormItem->getFormPoint(selF);
	switch (cmdID)
	{
	case ID_EX_FORMAT:
		{
			CString name = m_pFormItem->getFormStr(selF, "name"),
				editS = m_pFormItem->getFormStr(selF, "editS");
			CFormat*	format;
			format = new CFormat(this, formR->isEdit, (LPSTR)(LPCTSTR)editS,
				formR->attr, (LPSTR)(LPCTSTR)name, formR->size);
			if (format->DoModal() == IDOK)
			{
				formR->attr   = format->m_attr;
				formR->size   = format->m_size;
				formR->isEdit = format->m_isEdit;
				m_pFormItem->setFormStr(selF, "editS", format->m_edit);
				
				CRect	tRc;
				tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
				m_view.InvalidateRect(tRc);
			}
			delete format;
		}
		break;

	case ID_EX_RGB:
		CCoRGB*		coRGB;
		if (formR->isEdit == IE_CORGB)
		{
			coRGB = new CCoRGB(this, m_pFormItem->getFormStr(selF, "name"), formR->isEdit,
					HIWORD(formR->onEdit), LOWORD(formR->onEdit));
		}
		else
		{
			long lCorgb = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
			coRGB = new CCoRGB(this, m_pFormItem->getFormStr(selF, "name"), formR->isEdit, LOWORD(lCorgb), HIWORD(lCorgb));
		}

		if (coRGB->DoModal() == IDOK)
		{
			formR->isEdit = coRGB->m_isEdit;
			if (formR->isEdit == IE_CORGB)
			{
				formR->onEdit = MAKELONG(coRGB->m_dnRGB, coRGB->m_upRGB);
				formR->attr   |= FA_CORGB;
			}
			else
			{
				formR->onEdit = 0;
				formR->attr  &= ~FA_CORGB;
			}
		}
		delete coRGB;
		break;
	default:
		break;
	}

	return IE_NOP;
}

bool CChildFrame::DeleteFiles()
{
	if (!this->m_hWnd)
		return true;

	if (m_delete)
		return true;

	if (m_mapH.modified)
	{
		if (MessageBox(m_mapH.mapN + 
				CString("\n\nIt may be damaged unless you store this map.\n\nDo you close this map?"), 
				"Message", MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return false;
	}

	CString	tmps;

	m_delete = true;
	if (m_mapH.mapN[0] != NULL)
	{
		if (m_mapH.source)
		{
			tmps = _T("");
			wccSetTempPath(tmps, m_mapH.mapN);
			DeleteFile(tmps);
		}

		if (m_mapH.compile)
		{
			tmps = _T("");
			wccSetTempPath(tmps, m_mapH.mapN, false);
			DeleteFile(tmps);
		}

		if (m_mapH.onStart)
			DeleteFile(m_mapH.onStartN);
		if (m_mapH.onSend)
			DeleteFile(m_mapH.onSendN);
		if (m_mapH.onReceive)
			DeleteFile(m_mapH.onReceiveN);
		if (m_mapH.onAlert)
			DeleteFile(m_mapH.onAlertN);
		if (m_mapH.onService)
			DeleteFile(m_mapH.onServiceN);
		if (m_mapH.onFile)
			DeleteFile(m_mapH.onFileN);
		if (m_mapH.onSelect)
			DeleteFile(m_mapH.onSelectN);
		if (m_mapH.onTimer)
			DeleteFile(m_mapH.onTimerN);
		if (m_mapH.onFocus)
			DeleteFile(m_mapH.onFocusN);
		if (m_mapH.onClose)
			DeleteFile(m_mapH.onCloseN);
		if (m_mapH.onApprove)
			DeleteFile(m_mapH.onApproveN);
		if (m_mapH.onDevice)
			DeleteFile(m_mapH.onDeviceN);
		if (m_mapH.onKey)
			DeleteFile(m_mapH.onKeyN);
		if (m_mapH.onTrigger)
			DeleteFile(m_mapH.onTriggerN);
		if (m_mapH.onDeclaration)
			DeleteFile(m_mapH.onDeclarationN);
		if (m_mapH.onInDeclaration)
			DeleteFile(m_mapH.onInDeclarationN);
	}

	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (!m_pFormItem->getFormStr(ii, "scpN").IsEmpty())
			DeleteFile(m_pFormItem->getFormStr(ii, "scpN"));
	}

	m_delete = false;
	return true;
}

void CChildFrame::OnClose() 
{
	FreePopFrame();
	if (!m_bSDI && !DeleteFiles())
		return;

	CMDIChildWnd::OnClose();
}

int CChildFrame::getSelectedForm()
{
	if (m_view.m_block->count == 0 || m_view.m_block->count == 1)
		return m_view.m_selF;
	else
		return -2;
}

void CChildFrame::setSelectForm(int idx, bool bMulti /*= false*/)
{
	if (!bMulti)
	{
		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			m_pFormItem->getFormPoint(ii)->block = false;
		m_view.m_block->count = 0;
		m_view.m_selF = idx;
		m_view.m_stdF = -1;
	}
	else
	{
		m_pFormItem->getFormPoint(idx)->block = true;
		if (m_view.m_selF >= 0 && m_view.m_block->count == 0)
		{
			m_view.m_block->count++;
			m_pFormItem->getFormPoint(m_view.m_selF)->block = true;
		}
		m_view.m_block->count++;
		if (m_view.m_stdF < 0)
			m_view.m_stdF = idx;
	}
	m_view.m_nIoOdr = 0;
	m_view.m_nTabOdr = 0;
	m_view.RedrawView();
}

void CChildFrame::getGroupNum(int nGroup, CArray <int, int> &arr)
{
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->group == (struct _formR*)nGroup)
		{
			arr.Add(ii);
			if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
				getTabNum(m_pFormItem->getFormPoint(ii), arr);
		}
}
void CChildFrame::getTabNum(struct _formR *tabN, CArray <int, int> &arr)
{
	for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
		if (m_pFormItem->getFormPoint(jj)->tabN == tabN)
		{
			arr.Add(jj);
			if (m_pFormItem->getFormPoint(jj)->kind == FM_TAB)
				getTabNum(m_pFormItem->getFormPoint(jj), arr);
		}
}

CRect CChildFrame::GroupRect(struct _formR* nGroup)
{
	CRect rc, tRc;
	rc.SetRectEmpty();
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->group == (struct _formR*)nGroup)
		{
			tRc.SetRect(m_pFormItem->getFormPoint(ii)->left, 
				m_pFormItem->getFormPoint(ii)->top, 
				m_pFormItem->getFormPoint(ii)->right,
				m_pFormItem->getFormPoint(ii)->bottom);
			rc.UnionRect(rc, tRc);
		}
	return rc;
}

CRect CChildFrame::getFormRect(int idx)
{
	CRect tRc;
	if (m_pFormItem->getFormPoint(idx)->group)
		return GroupRect(m_pFormItem->getFormPoint(idx)->group);
	tRc.SetRect(m_pFormItem->getFormPoint(idx)->left, 
		m_pFormItem->getFormPoint(idx)->top, 
		m_pFormItem->getFormPoint(idx)->right,
		m_pFormItem->getFormPoint(idx)->bottom);
	return tRc;
}

void CChildFrame::setBookMark(CString src)
{
	m_bookmarkArr.Add(src);
}

int CChildFrame::getBMCount()
{
	return m_bookmarkArr.GetSize();
}

CString CChildFrame::getBookmarkSrc(int idx)
{
	return m_bookmarkArr.GetAt(idx);
}

void CChildFrame::killFocus()
{
	if (m_view.m_nTabOdr)
	{
		m_view.m_nTabOdr = 0;
		m_view.Invalidate();
	}
}

void CChildFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	if (m_view.m_nIoOdr || m_view.m_nTabOdr)
	{
		m_view.m_nIoOdr = 0;
		m_view.m_nTabOdr = 0;
		m_view.RedrawView();
	}
	if (getSelectedForm() != -1)
	{
		setSelectForm(-1);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
	}
	
	CMDIChildWnd::OnNcLButtonDown(nHitTest, point);
}


void CChildFrame::pushUndo(CString src)
{
	m_undoArr.InsertAt(0, src);
	if (m_undoArr.GetSize() > 20)
		m_undoArr.RemoveAt(m_undoArr.GetSize() - 1);
}

void CChildFrame::pushRedo(CString src)
{
	m_redoArr.InsertAt(0, src);
	if (m_redoArr.GetSize() > 20)
		m_redoArr.RemoveAt(m_redoArr.GetSize() - 1);
}

CString CChildFrame::popUndo()
{
	if (!m_undoArr.GetSize())
		return "";
	CString sUndo = m_undoArr.GetAt(0);
	m_undoArr.RemoveAt(0);
	return sUndo;
}

CString CChildFrame::popRedo(bool isAll)
{
	if (!m_redoArr.GetSize())
		return "";
	CString sRedo = m_redoArr.GetAt(0);
	if (isAll)
		m_redoArr.RemoveAll();
	else
		m_redoArr.RemoveAt(0);

	return sRedo;
}

bool CChildFrame::canUndo()
{
	return (m_undoArr.GetSize()?true:false);
}

bool CChildFrame::canRedo()
{
	return (m_redoArr.GetSize()?true:false);
}

void CChildFrame::SaveProcedures()
{
	HANDLE	file;
	DWORD	nData = 0;
	CString	dir = _T(""), sData = "";

	sData = m_view.m_pSCEdit->GetScriptText();
	
	if (sData.GetLength() <= 0)
	{
		if (m_mapH.onDeclaration)
			DeleteFile(m_mapH.onDeclarationN);
		m_mapH.onDeclarationN[0] = '\0';
		m_mapH.onDeclaration = false;
	}
	else
	{
		if (m_mapH.onDeclarationN[0] == '\0')
		{
			char	prefix[L_SGID+1];
			CopyMemory(prefix, m_mapH.mapN, L_SGID);
			prefix[L_SGID] = '\0';
			wccGetTempName(m_mapH.onDeclarationN, prefix);
		}
		dir = m_mapH.onDeclarationN;
		m_mapH.onDeclaration = true;
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

void CChildFrame::changeKind()
{
	m_view.changeKind();
}


void CChildFrame::LoadProcedures()
{
	if (!m_view.m_pSCEdit)
		return;

	HANDLE	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;
	CString dir = m_mapH.onDeclarationN;

	m_view.m_pSCEdit->SetScriptText("");

	file = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return;

	scpSize = GetFileSize(file, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
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

	m_view.m_pSCEdit->SetScriptText(sbuf);
	delete[] sbuf;
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == 100 && cx < 100)
		SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
	if (m_bSDI)
	{
		m_pPopFrame->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE);
		return;
	}
	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CChildFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	lpMMI->ptMinTrackSize.x = 0;
	lpMMI->ptMinTrackSize.y = 0;
// kwon 2007.05.20 
	lpMMI->ptMaxTrackSize.x = 3000;
	lpMMI->ptMaxTrackSize.y = 3000;
//	lpMMI->ptMaxTrackSize.x = 2048;
//	lpMMI->ptMaxTrackSize.y = 2048;

	CMDIChildWnd::OnGetMinMaxInfo(lpMMI);
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
	CMDIChildWnd::OnSizing(fwSide, pRect);	
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if (nID == SC_CLOSE)
	{
		if (m_view.m_pPreView)
		{
			m_view.Preview();
			return;
		}
	}
	CMDIChildWnd::OnSysCommand(nID, lParam);
}

void CChildFrame::DeleteSelForms()
{
	m_view.deleteFORMs();
}

void CChildFrame::DeleteUndoMap()
{
	CMapPtrToPtr *pUndoMap = (CMapPtrToPtr*)m_mapH.pUndoMap;
	if (pUndoMap)
	{
		for (POSITION pos = pUndoMap->GetStartPosition(); pos;)
		{
			void *key;
			CObject *pArr;
			pUndoMap->GetNextAssoc(pos, key, (void*&)pArr);
			delete pArr;
		}
		pUndoMap->RemoveAll();
		delete pUndoMap;
		m_mapH.pUndoMap = NULL;
	}
}

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bSDI)
		return m_pPopFrame->PreTranslateMessage(pMsg);
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CChildFrame::FreePopFrame()
{
	if (m_pPopFrame)
	{
		delete m_pVirtualWnd;

		m_pPopFrame   = NULL;
		m_pVirtualWnd = NULL;

	}
}

LRESULT CChildFrame::HandleSetText(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		if (m_bSDI && m_pPopFrame)
			m_pPopFrame->SetWindowText((char*)lParam);
		DefWindowProc(WM_SETTEXT, wParam, lParam);
	}
	return 0;
}

void CChildFrame::ReloadColors()
{
	if (m_mapH.mapK == MK_PROCEDURES)
	{
		m_view.m_pSCEdit->LoadColors();
		m_view.m_pSCEdit->Invalidate();
	}
}

void CChildFrame::OnActivateMe()
{
	if (!m_view)
		return;
	CString	title = _T("AXIS Builder");
	CString	tmps;

	if (m_mapH.mapN[0] != NULL)
	{
		tmps.Format("[%s]", m_mapH.mapN);
		title += tmps;
	}
	m_mainFRM->SetWindowText(title);
	m_view.m_block->count = 0;
	m_mainFRM->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
	m_mainFRM->SendMessage(WM_USER, ID_USR_SETFINDCAPTION, NULL);
	if (m_view.m_pPreView)
		m_mainFRM->SendMessage(WM_USER, ID_USR_ENABLEPROP, FALSE);
	else
		m_mainFRM->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
}
