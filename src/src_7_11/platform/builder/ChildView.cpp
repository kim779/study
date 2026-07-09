// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Imm.h"
#include "resource.h"

#include "axisWork.h"
#include "childView.h"
#include "childFrm.h"
#include "mainFrm.h"

#include "mapvar.h"
#include "mainvar.h"
#include "mapform.h"

#include "awBuild/libBuild.h"
#include "awTool/tool.h"
#include "awTool/tlIO.h"
#include "awTool/tlBox.h"
#include "awTool/tlTab.h"
#include "awTool/tlText.h"
#include "awTool/tlTree.h"
#include "awTool/tlImage.h"
#include "awTool/tlPanel.h"
#include "awTool/tlSheet.h"
#include "awTool/tlSplit.h"
#include "awTool/tlTable.h"
#include "awTool/tlButton.h"
#include "awTool/tlRegion.h"
#include "awTool/tlPointer.h" 
#include "awTool/picture.h"

#include "awObject/show.h"
#include "awDlg/Preview.h"
#include "awDlg/PreviewRun.h"
#include "awDlg/GridColumnDlg.h"
#include "awDlg/attrDlg.h"
#include "awDlg/ControlDlg.h"
#include "awDlg/UTabPage.h"

#include "awDlg/colordlg.h"
#include "awCommon/CellItem.h"

#include "ControlTracker.h"

#include "../dll/lib/axislib.h"

#include "keywords.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CURSORNWSE	1
#define CURSORNESW	2
#define	CURSORNS	3
#define	CURSORWE	4
#define	ID_DRAGTMR	100

#define GAPX	GetSystemMetrics(SM_CXFRAME) * 2 + GetSystemMetrics(SM_CXBORDER) * 2
#define GAPY	GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYBORDER) * 2 + GetSystemMetrics(SM_CYCAPTION)

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_toolOBs.RemoveAll();
	m_selF      = -1;
	m_stdF      = -1;
	m_move      = false;
	m_drawing   = false;
	m_selected  = false;
	m_bLBtnDown = false;
	m_pBitmap   = NULL;
	m_nTabOdr   = m_nSeqTabOdr = 0;
	m_nIoOdr    = m_nSeqIoOdr = 0;
	m_bRedraw   = true;

	m_mapH      = (struct _mapH   *) NULL;
	m_block     = (struct _block  *) NULL;
	m_pSCEdit   = NULL;
	m_dir       = _T("");
	m_pPreView  = NULL;

	m_nGap = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGAP, 0);
	m_forms.RemoveAll();

	m_sImgPath = "";
	m_hImgBack = NULL;
}

CChildView::~CChildView()
{
	for (int ii = 0; ii < m_toolOBs.GetSize(); ii++)
	{
		CTOOL* tool = (CTOOL *)m_toolOBs.GetAt(ii);
		ASSERT_VALID(tool);
		delete tool;
	}
	m_toolOBs.RemoveAll();
	if (m_pSCEdit)
		delete m_pSCEdit;

	delete m_pFormItem;
	int	key;
	CShow*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		delete form;
	}
	m_forms.RemoveAll();

	if (m_pBitmap)	delete m_pBitmap;
	if (m_hImgBack)	DeleteObject(m_hImgBack);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_RSENDTOFIRST, ID_EXTMENUID, OnViewCommand)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers
//
void CChildView::Init(struct _mapH* mapH, struct _block* block, CString dir)
{
	CWinApp* app = AfxGetApp();
	m_root = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);

	m_mapH  = mapH;
	m_pFormItem = new CFormItem(m_mapH);
	m_block = block;
	m_dir   = dir;
	
	changeKind();

	CtlPointer* pointer = new CtlPointer(this);
	pointer->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(pointer);

	CtlText*    sysm    = new CtlText(this, ID_TOOL_SYS);
	sysm->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(sysm);
	
	CtlText*    label   = new CtlText(this, ID_TOOL_LABEL);
	label->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(label);

	CtlBox*     box     = new CtlBox(this);
	box->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(box);
	
	CtlPanel*   group   = new CtlPanel(this, ID_TOOL_GROUP);
	group->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(group);

	CtlPanel*   panel   = new CtlPanel(this, ID_TOOL_PANEL);
	panel->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(panel);

	CtlButton*  push    = new CtlButton(this, ID_TOOL_PUSH);
	push->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(push);

	CtlButton*  radio   = new CtlButton(this, ID_TOOL_RADIO);
	radio->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(radio);
	
	CtlButton*  check   = new CtlButton(this, ID_TOOL_CHECK);
	check->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(check);

	CtlIO*      edit    = new CtlIO(this, ID_TOOL_EDIT);
	edit->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(edit);

	CtlIO*      combo   = new CtlIO(this, ID_TOOL_COMBO);
	combo->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(combo);

	CtlIO*      out     = new CtlIO(this, ID_TOOL_OUT);
	out->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(out);
	
	CtlIO*      memo    = new CtlIO(this, ID_TOOL_MEMO);
	memo->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(memo);
	
	CtlTable*   grid    = new CtlTable(this, ID_TOOL_GRID);
	grid->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(grid);

	CtlTable*   table    = new CtlTable(this, ID_TOOL_TABLE);
	table->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(table);

	CtlTree*    tree    = new CtlTree(this);
	tree->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(tree);

	CtlImage*   avi     = new CtlImage(this, ID_TOOL_AVI);
	avi->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(avi);
	
	CtlRegion*  graph   = new CtlRegion(this, ID_TOOL_GRAPH);
	graph->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(graph);
	
	CtlRegion*  object  = new CtlRegion(this, ID_TOOL_OBJECT);
	object->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(object);

	CtlTab*     tab     = new CtlTab(this, ID_TOOL_TAB);
	tab->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(tab);
	
	CtlTab*     usertab = new CtlTab(this, ID_TOOL_USERTAB);
	usertab->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(usertab);

	CtlSheet*   sheet   = new CtlSheet(this);
	sheet->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(sheet);

	CtlRegion*  browser = new CtlRegion(this, ID_TOOL_BROWSER);
	browser->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(browser);

	CtlIO*  control = new CtlIO(this, ID_TOOL_CONTROL);
	control->m_pDisplayMode = m_pDisplayMode;
	m_toolOBs.Add(control);

	m_tool = pointer;
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
			::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_BTNFACE+1), NULL);

	return TRUE;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
  	if (!m_tool->m_name.CompareNoCase("Pointer"))
  	{
		int	oldF;
		switch (nChar)
		{
		case VK_TAB:
			if (m_pFormItem->getFormCount())
			{
				oldF = m_selF;
				for (;;)
				{
					m_selF = (++m_selF) % m_pFormItem->getFormCount();
					if (m_pFormItem->getFormPoint(m_selF)->group)
						m_selF = m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->group);
					if (m_pFormItem->getFormPoint(m_selF)->tabN && 
						m_pFormItem->getFormPoint(m_selF)->tabN->checked != m_pFormItem->getFormPoint(m_selF)->pageN)
					{
						m_pFormItem->getFormPoint(m_selF)->tabN->checked = m_pFormItem->getFormPoint(m_selF)->pageN;
						InvalidateRect2(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->tabN));
					}
					
					if (!m_pFormItem->getFormStr(m_selF, "name").IsEmpty())
						break;
				}
				reDrawSelChange(oldF, m_selF);
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
			}
			break;

		case VK_DELETE:
			if ((::GetKeyState(VK_SHIFT) & 0x8000) && m_selF >= 0 &&
				(m_pFormItem->getFormPoint(m_selF)->kind == FM_TAB ||
				m_pFormItem->getFormPoint(m_selF)->kind == FM_UTAB) &&
				m_pFormItem->getFormPoint(m_selF)->keys > 0)
			{
				deleteSelPage(m_selF);
				break;
			}
			if (m_selF >= 0 &&
				(m_pFormItem->getFormPoint(m_selF)->kind == FM_TABLE ||
				m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID))
			{
				if (m_pFormItem->getFormPoint(m_selF)->auxR != NULL &&
					((struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR)->selected >= 0)
				{
					struct _repR *repR = (struct _repR*)m_pFormItem->getFormPoint(m_selF)->auxR;
					CCellItem cellItem(repR);
					cellItem.removeCell(repR->selected);
					if (m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID)
					{
						repR->cols--;
						if (repR->cols <= repR->selected)
							repR->selected = repR->cols - 1;
					}
					else
					{
						repR->rows--;
						if (repR->rows <= repR->selected)
							repR->selected = repR->rows - 1;
					}
					Invalidate();
					break;
				}
			}
			deleteFORMs();
			break;

		case VK_F8:
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETFOCUSPRO, 0);
			break;

		case VK_F9:
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETFOCUSWRK, 0);
			break;

		case VK_F10:
			SetFocus();
			break;

		case VK_ESCAPE:
			m_tool->ReleaseBlock();
			break;

		case VK_UP:
			keyArrowUp();
			break;

		case VK_DOWN:
			keyArrowDown();
			break;
			
		case VK_LEFT:
			keyArrowLeft();
			break;
			
		case VK_RIGHT:
			keyArrowRight();
			break;
		default:
			/*if (!(::GetKeyState(VK_CONTROL) & 0x8000))
			{
				DWORD	dwConversion,dwSentence;
				
				HIMC hImc = ImmGetContext(this->m_hWnd);
				if ( ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
				{
					if ((dwConversion & IME_CMODE_HANGEUL) && !(nFlags & 0x0080) && nChar > 128)
					{
						AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_KEYIN, nChar);
						break;
					}
				}
				if (isalnum(nChar) && !(nFlags & 0x0080))
					AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_KEYIN, nChar);
			}*/
			break;
		}
	}
	
//	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
//	CWnd::OnSize(nType, cx, cy);
	if (nType == SIZE_MINIMIZED || m_new || m_mapH == (struct _mapH *) NULL)
		return;

	CRect	cRc;

	GetClientRect(&cRc);

	m_mapH->width    = cRc.Width();
	m_mapH->height   = cRc.Height();

	if (m_mapH->mapK == MK_PROCEDURES)
	{
		if (m_pSCEdit)
			m_pSCEdit->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER);
		CWnd::OnSize(nType, cx, cy);
		return;
	}

	if (m_pBitmap)
	{
		CDC *pDC = GetDC();
		delete m_pBitmap;
	
		m_pBitmap = new CBitmap();
		m_pBitmap->CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());

		ReleaseDC(pDC);
		Invalidate();
		m_mapH->modified = true;
	}

	Invalidate(FALSE);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ERASECTRL);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	SetFocus();
	if (m_nTabOdr || m_nIoOdr)
	{
		m_drawing = false;
		m_tool->MouseDown(point);
		return;
	}
	m_dragPnt = point;
	if (!(nFlags & MK_CONTROL) && m_tool->m_name.CompareNoCase("Pointer"))
	{
		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}
	m_bLBtnDown = true;
//	CWnd::OnLButtonDown(nFlags, point);

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ERASECTRL, 0);
	if (!m_drawing) 
	{
		CRect	tRc;
		int	fN = m_pFormItem->getFormCount();
		if (m_selF >= 0 && 
			(m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID || 
			m_pFormItem->getFormPoint(m_selF)->kind == FM_TABLE) &&
			m_pFormItem->getFormPoint(m_selF)->auxR)
			((struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR)->selected = -1;
		
		if (m_tool->m_name.CompareNoCase("Pointer"))
		{
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
			int tabN, pageN = 0;
			if (!isInTab(point, tabN, pageN))
				tabN = -1;

			m_pFormItem->addForm();
			struct _formR *formR = m_pFormItem->getFormPoint(fN);
			
			if (!m_tool->m_name.CompareNoCase("Table"))
			{
				formR->pRGB = 232;
				formR->bRGB = 232;
			}
			else
				formR->pRGB = basPRGB;
			
			formR->tRGB = 232;
			formR->fPoint  = m_mapH->fPoint;
			m_pFormItem->setFormStr(fN, "fName", m_mapH->fName);
			
			if (!m_tool->m_name.CompareNoCase("Grid"))
				formR->keys = 20;
			
			formR->size = 0;
			formR->attr = 0;
			formR->properties = 0;
			formR->properties |= PR_VISIBLE;
			formR->properties |= PR_ENABLE;
			if (m_tool->m_name.CompareNoCase("Grid") && m_tool->m_name.CompareNoCase("Table"))
				formR->properties |= PR_CHECKED;
			formR->checked = 0;
			
			formR->onClick    = -1;
			formR->onClickL   = 0;
			formR->onDblClk   = -1;
			formR->onDblClkL  = 0;
			formR->onChange   = -1;
			formR->onChangeL  = 0;
			formR->onChar     = -1;
			formR->onCharL    = 0;
			formR->onKey      = -1;
			formR->onKeyL     = 0;
			
			formR->tabN = m_pFormItem->getFormPoint(tabN);
			formR->pageN = pageN;
			if (!m_tool->m_name.CompareNoCase("Edit") || !m_tool->m_name.CompareNoCase("ComboBox") ||
				!m_tool->m_name.CompareNoCase("PushButton") || !m_tool->m_name.CompareNoCase("Grid") ||
				!m_tool->m_name.CompareNoCase("Object") || !m_tool->m_name.CompareNoCase("Memo") ||
				!m_tool->m_name.CompareNoCase("Control"))
			{
				sortTabOrder();
				formR->index = ++m_nSeqTabOdr;
			}
			if (!m_tool->m_name.CompareNoCase("Edit") || !m_tool->m_name.CompareNoCase("ComboBox") ||
				!m_tool->m_name.CompareNoCase("PushButton") || !m_tool->m_name.CompareNoCase("Grid") ||
				!m_tool->m_name.CompareNoCase("Object") || !m_tool->m_name.CompareNoCase("Memo") ||
				!m_tool->m_name.CompareNoCase("Table") || !m_tool->m_name.CompareNoCase("TreeView") ||
				!m_tool->m_name.CompareNoCase("Output") || !m_tool->m_name.CompareNoCase("Graph") ||
				!m_tool->m_name.CompareNoCase("Sheet") || !m_tool->m_name.CompareNoCase("Browser"))
			{
				formR->iorder = ++m_nSeqIoOdr;
			}
			RedrawWindow();
		}
		else	// grid, table ... tool pointer
		{
			if (selectable(point))
				m_bSelect = true;
			if (m_move || m_bSelect)
			{
				int	rVal = getTableForm(point);
				if (rVal >= 0)
				{
					m_selF = rVal;
					switch (m_pFormItem->getFormPoint(m_selF)->kind)
					{
					case FM_GRID:
						rVal = ID_TOOL_GRID - ID_TOOL_BASE;	break;
					case FM_TABLE:
						rVal = ID_TOOL_TABLE - ID_TOOL_BASE;	break;
					case FM_TAB:
						rVal = ID_TOOL_TAB - ID_TOOL_BASE;	break;
					case FM_UTAB:
						rVal = ID_TOOL_USERTAB - ID_TOOL_BASE;	break;
					default:
						return;
					}
					if (m_bSelect)
						SetTimer(ID_DRAGTMR, 500, NULL);

					m_tool = (CTOOL *)m_toolOBs.GetAt(rVal);
				}
				else
					m_tool = (CTOOL *)m_toolOBs.GetAt(0);
			}
			else if (m_selF >= 0 && 
				(m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID || 
				m_pFormItem->getFormPoint(m_selF)->kind == FM_TABLE))
			{
				if (m_pFormItem->getFormPoint(m_selF)->auxR == NULL)
				{
					struct _repR *repR;
					m_pFormItem->getFormPoint(m_selF)->auxR = new char[sizeof(_repR)];
					repR = (struct _repR *)m_pFormItem->getFormPoint(m_pFormItem->getFormCount() - 1)->auxR;
					ZeroMemory(repR, sizeof(struct _repR));
					if (m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID)
					{
						repR->rows = 10;
						m_pFormItem->getFormPoint(m_selF)->keys = MAKELONG(10, HIWORD(m_pFormItem->getFormPoint(m_selF)->keys));
					}
				}
				((struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR)->selected = -1;
			}
		}
		m_drawing = true;
		m_tool->MouseDown(point);
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	if (m_bSelect)
		KillTimer(ID_DRAGTMR);
	if (m_nTabOdr || m_nIoOdr)
	{
		m_tool->MouseUp();
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, ID_TOOL_POINTER);
		m_tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_POINTER - ID_TOOL_BASE);
		m_selected = false;
		if (m_selF != -1)
		{
			if (m_nTabOdr)
				sortTabOrder();
			else
				sortIoOrder();
		}

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
		return;
	}
	CRect	tRc;
	struct	_repR*	repR;
	int msgId = ID_USR_REFRESHCTRL;

	if (!(nFlags & MK_CONTROL) && m_tool->m_name.CompareNoCase("Pointer"))
	{
		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}

	if (m_drawing)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 0), NULL);
		//m_tool->MouseMove(point);
		m_tool->MouseUp();
		
		m_mapH->modified = true;

		if (!(m_move || m_bSelect) && (!m_tool->m_name.CompareNoCase("Grid") || !m_tool->m_name.CompareNoCase("Table")))
		{
			int formN = m_pFormItem->getFormCount() - 1;
			m_pFormItem->getFormPoint(formN)->auxR = new char[sizeof(_repR)];
			repR = (struct _repR *)m_pFormItem->getFormPoint(formN)->auxR;
			CCellItem *cellItem = new CCellItem(repR);

			ZeroMemory(repR, sizeof(struct _repR));
			repR->selected = -1;
			if (m_pFormItem->getFormPoint(formN)->kind == FM_TABLE)
			{
				m_pFormItem->getFormPoint(formN)->keys = 20;
				m_pFormItem->getFormPoint(formN)->attr2 |= GO_MULTI;
			}
			else
			{
				if (m_mapH->typeH != TH_LEDGER)
					m_pFormItem->getFormPoint(formN)->attr2 |= GO_HEADER;
				repR->rows = 10;
			}

			for (int jj = 0; jj < cellItem->getCellCount(); jj++) // cell �ʱ�ȭ
			{
				struct _cellR	*cellR;
				cellR = cellItem->getCellPoint(jj);
				cellR->type   = IO_NUM;
				cellR->width  = 80;
				cellR->hpRGB  = basPRGB;
				cellR->pRGB   = basINPUT;
				cellR->alignC = AL_RIGHT;
				cellR->properties |= PR_VISIBLE;
			}
			delete cellItem;
		}
		
		//	selection ó��.
		if (m_selF >= 0)
		{
			tRc = getFormRect(m_selF);
			if (m_pFormItem->getFormPoint(m_selF)->group != 0)
				m_selF = m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->group);
		}
		
		if (m_tool->m_name.CompareNoCase("Pointer") && !(m_move || m_bSelect))
		{
			m_selF = m_pFormItem->getFormCount() - 1;
			m_pFormItem->setFormStr(m_selF, "name", getInitSymbol());
			tRc = getFormRect(m_selF);
			tRc.InflateRect(5, 5);
			InvalidateRect(tRc);
		}
		else
			msgId = ID_USR_SELCTRLTOPRO;

		if (!m_tool->m_name.CompareNoCase("Object"))
			SetObject(m_pFormItem->getFormCount()-1);

		m_move    = false;
		m_drawing = false;
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, ID_TOOL_POINTER);
	
	m_tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_POINTER - ID_TOOL_BASE);
	m_selected = false;
	m_bLBtnDown = false;
	sortIoOrder();
	sortTabOrder();
	AfxGetMainWnd()->SendMessage(WM_USER, msgId, (msgId == ID_USR_REFRESHCTRL?TRUE:FALSE));
	
//	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_EDIT_SCRIPT, 0);	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	if ((nFlags & MK_LBUTTON) && abs((m_dragPnt.x - point.x) * (m_dragPnt.y - point.y)) > 4)
		m_tool->MouseDrag();
	if (m_nTabOdr || m_nIoOdr)
		return;
	m_nCursor = 0;
	if (m_drawing)
	{
		if (!(nFlags & MK_CONTROL) && m_tool->m_name.CompareNoCase("Pointer"))
		{
			point.x = ((int)(point.x / m_nGap)) * m_nGap;
			point.y = ((int)(point.y / m_nGap)) * m_nGap;
		}

		m_tool->MouseMove(point);

		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}
	else
	{
		m_move = false;
		m_bSelect = false;
		if (m_selF >= 0 && !m_tool->m_name.CompareNoCase("Pointer") && (m_block->count <= 1) && !m_pFormItem->getFormPoint(m_selF)->group)
		{
			CRect	tRc = getFormRect(m_selF);
			
			CControlTracker	tracker(tRc, CControlTracker::resizeOutside);
			int	hit = tracker.HitTest(point);
			hit = CControlTracker::TrackerHit(hit);
			switch (hit)
			{
			case CControlTracker::hitTopLeft:
			case CControlTracker::hitBottomRight:
				m_nCursor = CURSORNWSE;
				break;
			case CControlTracker::hitTopRight:
			case CControlTracker::hitBottomLeft:
				m_nCursor = CURSORNESW;
				break;
			case CControlTracker::hitTop:
			case CControlTracker::hitBottom:
				m_nCursor = CURSORNS;
				break;
			case CControlTracker::hitRight:
			case CControlTracker::hitLeft:
				m_nCursor = CURSORWE;
				break;
			case CControlTracker::hitMiddle:
				if (movable(point))
				{
					m_move = true;
					m_nCursor = CURSORWE;
					break;
				}
			case CControlTracker::hitNothing:
			default:
				break;
			}
		}
  	}

//	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	CRect	tRc,  fRc, pRc;
	int	oldF  = m_selF;
	int	tForm = 0;

	m_selF = -1;
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (m_pFormItem->getFormPoint(ii)->tabN && 
			m_pFormItem->getFormPoint(ii)->tabN->checked != m_pFormItem->getFormPoint(ii)->pageN)
			continue;
		tRc = getRect(ii);
		if (tRc.PtInRect(point))
			m_selF = ii;
	}

	if (m_selF > m_pFormItem->getFormCount() || m_selF < 0)
		m_selF = -1;
	else
	{
		for (;;)
		{
			if (m_selF == tForm)
				break;

			if (m_pFormItem->getFormPoint(m_selF)->kind == FM_GROUP || m_pFormItem->getFormPoint(m_selF)->kind == FM_PANEL
					|| m_pFormItem->getFormPoint(m_selF)->kind == FM_BOX)
			{
				tForm = m_selF;
				pRc = getRect(tForm);
				for (int kk = 0; kk < tForm; kk++)
				{
					if (m_pFormItem->getFormStr(kk, "name").IsEmpty())
						continue;

					if (m_pFormItem->getFormPoint(kk)->group && 
						m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(kk)->group) != kk)
						continue;

					if (m_pFormItem->getFormPoint(kk)->tabN)
						fRc = getRect(kk, 1);
					else
						fRc = getRect(kk);

					if (fRc.PtInRect(point))
					{
						tRc.IntersectRect(fRc, pRc);
						if (fRc.EqualRect(tRc))
							m_selF = kk;
					}
				}
			}
			else
				break;
		}
	}

	if (m_selF != oldF)
		reDrawSelChange(oldF, m_selF);

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);

	m_RBDPnt = point;
	
//	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ShowMenu();
	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::DrawForm(int cmdID)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	if (m_block->count)
	{
		for (int jj = m_pFormItem->getFormCount() - 1; jj >= 0; jj--)
			if (m_pFormItem->getFormPoint(jj)->block)
				m_pFormItem->getFormPoint(jj)->block = false;
			m_block->count = 0;
	}
	m_block->bRc.SetRectEmpty();

	int	tool = cmdID - ID_TOOL_BASE;
	if (tool > 0)
		m_selected = true;
	else
		m_selected = false;
	m_tool = (CTOOL *)m_toolOBs.GetAt(tool);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, cmdID);
}

void CChildView::AlignForms(int cmdID)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	int	stdV, ii, delta;
	long	minV, maxV;
	CRect	wRc, pRc;
	GetClientRect(pRc);

	if ((cmdID >= ID_AL_LEFT && cmdID <= ID_BLK_VCENTER) && m_stdF < 0)
		return;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);

	switch (cmdID)
	{
	case ID_AL_LEFT:
		SetBlockForm(m_pFormItem->getFormPoint(m_stdF)->left, 0);
		break;

	case ID_AL_TOP:
		SetBlockForm(m_pFormItem->getFormPoint(m_stdF)->top, 1);
		break;

	case ID_AL_RIGHT:
		SetBlockForm(m_pFormItem->getFormPoint(m_stdF)->right, 2);
		break;

	case ID_AL_BOTTOM:
		SetBlockForm(m_pFormItem->getFormPoint(m_stdF)->bottom, 3);
		break;

	case ID_BLK_HCENTER:
		pRc = getFormRect(m_stdF);
		stdV = pRc.left + pRc.Width() / 2;
		SetBlockForm(stdV, 4);
		GetClientRect(pRc);
		break;

	case ID_BLK_VCENTER:
		pRc = getFormRect(m_stdF);
		stdV = pRc.top + pRc.Height() / 2;
		SetBlockForm(stdV, 5);
		GetClientRect(pRc);
		break;

	case ID_WIN_HCENTER:
		if (m_block->count == 0 && m_selF == -1)
			break;
		GetClientRect(&wRc);
		if (m_block->count != 0)
		{
			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					if (m_pFormItem->getFormPoint(ii)->tabN)
						wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->tabN));
			}
		}
		else
			if (m_pFormItem->getFormPoint(m_selF)->tabN)
				wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->tabN));
		stdV = wRc.Width() / 2;
		minV = wRc.right;
		maxV = 0;
		if (m_block->count != 0)
		{
			pRc.SetRectEmpty();
			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					pRc.UnionRect(pRc, getFormRect(ii));
			}

			delta = wRc.left + stdV - pRc.left - pRc.Width() / 2;

			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					MoveForm(ii, delta, 0);
			}
		}
		else
		{
			pRc = getFormRect(m_selF);
			delta = pRc.Width() / 2;
			MoveForm(m_selF, stdV - delta + wRc.left - pRc.left, 0);
		}
		break;

	case ID_WIN_VCENTER:
		if (m_block->count == 0 && m_selF == -1)
			break;
		GetClientRect(&wRc);
		if (m_block->count != 0)
		{
			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					if (m_pFormItem->getFormPoint(ii)->tabN)
						wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->tabN));
			}
		}
		else
			if (m_pFormItem->getFormPoint(m_selF)->tabN)
				wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->tabN));
		stdV = wRc.Height() / 2;
		minV = wRc.bottom;
		maxV = 0;
		if (m_block->count != 0)
		{
			pRc.SetRectEmpty();
			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					pRc.UnionRect(pRc, getFormRect(ii));
			}

			delta = wRc.top + stdV - pRc.top - pRc.Height() / 2;

			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					MoveForm(ii, 0, delta);
			}
		}
		else
		{
			pRc = getFormRect(m_selF);
			delta = pRc.Height() / 2;
			MoveForm(m_selF, 0, stdV - delta + wRc.top - pRc.top);
			GetClientRect(pRc);
		}
		break;

	case ID_SPC_HEQUAL:
		if (m_block->count == 0 && m_selF == -1)
			break;
		GetClientRect(&wRc);
		if (m_block->count != 0)
		{
			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					if (m_pFormItem->getFormPoint(ii)->tabN)
						wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->tabN));
			}
		}
		else
			if (m_pFormItem->getFormPoint(m_selF)->tabN)
				wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->tabN));
		minV = wRc.right;
		maxV = 0;
		pRc.SetRect(wRc.right, wRc.bottom, 0, 0);

		if (m_block->count == 0 || m_block->count == 1)
			return;

		for (ii = 0 ; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				minV = __min(minV, m_pFormItem->getFormPoint(ii)->left);
				maxV = __max(maxV, m_pFormItem->getFormPoint(ii)->left);

				pRc.left   = __min(pRc.left,   m_pFormItem->getFormPoint(ii)->left);
				pRc.top    = __min(pRc.top,    m_pFormItem->getFormPoint(ii)->top);
				pRc.right  = __max(pRc.right,  m_pFormItem->getFormPoint(ii)->right);
				pRc.bottom = __max(pRc.bottom, m_pFormItem->getFormPoint(ii)->bottom);
			}
		} 
		stdV = (maxV - minV)/(m_block->count - 1);
		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			m_pFormItem->getFormPoint(ii)->drawn = false;
		spaceEqual(stdV, 0, maxV, 0);	
		break;

	case ID_SPC_VEQUAL:
		if (m_block->count == 0 && m_selF == -1)
			break;
		GetClientRect(&wRc);
		if (m_block->count != 0)
		{
			for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				if (m_pFormItem->getFormPoint(ii)->block)
					if (m_pFormItem->getFormPoint(ii)->tabN)
						wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->tabN));
			}
		}
		else
			if (m_pFormItem->getFormPoint(m_selF)->tabN)
				wRc = getFormRect(m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->tabN));
		minV = wRc.bottom;
		maxV = 0;
		pRc.SetRect(wRc.right, wRc.bottom, 0, 0);

		if (m_block->count == 0 || m_block->count == 1)
			return;

		for (ii = 0 ; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				minV = __min(minV, m_pFormItem->getFormPoint(ii)->top);
				maxV = __max(maxV, m_pFormItem->getFormPoint(ii)->top);

				pRc.left   = __min(pRc.left,   m_pFormItem->getFormPoint(ii)->left);
				pRc.top    = __min(pRc.top,    m_pFormItem->getFormPoint(ii)->top);
				pRc.right  = __max(pRc.right,  m_pFormItem->getFormPoint(ii)->right);
				pRc.bottom = __max(pRc.bottom, m_pFormItem->getFormPoint(ii)->bottom);
			}
		} 
		stdV = (maxV - minV)/(m_block->count - 1);
		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			m_pFormItem->getFormPoint(ii)->drawn = false;
		spaceEqual(stdV, 0, maxV, 0, false);	
		break;

	case ID_SZ_HEQUAL:
		pRc = getFormRect(m_stdF);
		stdV = pRc.Width();
		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				if (m_pFormItem->getFormPoint(ii)->group)
				{
					struct _formR *nGroup = m_pFormItem->getFormPoint(ii)->group;
					for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
					{
						if (m_pFormItem->getFormPoint(ii)->group == nGroup)
							m_pFormItem->getFormPoint(jj)->right = m_pFormItem->getFormPoint(jj)->left + stdV;
					}
				}
				else
					m_pFormItem->getFormPoint(ii)->right = m_pFormItem->getFormPoint(ii)->left + stdV;
			}
		}
		break;

	case ID_SZ_VEQUAL:
		pRc = getFormRect(m_stdF);
		stdV = pRc.Height();
		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				if (m_pFormItem->getFormPoint(ii)->group)
				{
					struct _formR *nGroup = m_pFormItem->getFormPoint(ii)->group;
					for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
					{
						if (m_pFormItem->getFormPoint(ii)->group == nGroup)
							m_pFormItem->getFormPoint(jj)->bottom = m_pFormItem->getFormPoint(jj)->top + stdV;
					}
				}
				else
					m_pFormItem->getFormPoint(ii)->bottom = m_pFormItem->getFormPoint(ii)->top + stdV;
			}
		}
		break;
	}

	Invalidate();
}

CString CChildView::getInitSymbol(CString sSymbolList)
{
	CString tSym;
	int	initN, fN = m_pFormItem->getFormCount() - 1;

	if (sSymbolList.IsEmpty())
	{
		for (int kk = 0; kk < m_pFormItem->getFormCount(); kk++)
		{
			sSymbolList += m_pFormItem->getFormStr(kk, "name");
			sSymbolList += '\t';
		}
	}
	initN = 0;
	for (;;)
	{
		tSym.Format("%s%d", 
			getControlName(m_pFormItem->getFormPoint(fN)->kind,
			m_pFormItem->getFormPoint(fN)->type), initN);
		if (sSymbolList.Find(tSym) == -1)
			break;
		else
			initN++;
	}
	return tSym;
}

CString CChildView::getItemSymbol(CString sSymbol)
{
	CString tSym, sSymbolList;
	int	initN;
	struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR;
	if (!repR)
		return sSymbol;
	CCellItem cellItem(repR);

	for (int kk = 0; kk < cellItem.getCellCount(); kk++)
	{
		if (cellItem.getCellPoint(kk)->name[0] != NULL)
		{
			sSymbolList += cellItem.getCellPoint(kk)->name;
			sSymbolList += '\t';
		}
	}

	initN = 0;
	for (;;)
	{
		tSym.Format("%s%d", sSymbol, initN);
		if (sSymbolList.Find(tSym + "\t") == -1)
			break;
		else
			initN++;
	}
	
	return tSym;
}

void CChildView::SetObject(int index)
{
	CRect	rect;

	rect.SetRect(m_pFormItem->getFormPoint(index)->left, m_pFormItem->getFormPoint(index)->top, m_pFormItem->getFormPoint(index)->right, m_pFormItem->getFormPoint(index)->bottom);
	CShow*	form = new CShow(this, m_palette, m_draw, rect, m_root, m_pFormItem->getFormPoint(index)->str);
	m_forms.SetAt(index, form);
}

void CChildView::OnDrop(CPoint pt)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	CChildFrame* child = (CChildFrame *)m_pChildFrame;

	if (child->m_mainFRM->m_status & AWS_SDRAG)
	{
		ScreenToClient(&pt);
		Csymbol* symbol;

		int grid = getTableForm(pt);
		for (int ii = 0; ii < child->m_mainFRM->m_wndWorkBar.GetSelectedCount(); ii++)
		{
			symbol = child->m_mainFRM->m_wndWorkBar.m_SymTbl.GetSelectedData(ii);
			if (symbol == (Csymbol *) NULL)
				continue;

			if (grid >= 0)
			{
				if (m_pFormItem->getFormPoint(grid)->kind == FM_TAB)
					addForm(symbol, pt, m_pFormItem->getFormPoint(grid));
				else if (m_pFormItem->getFormPoint(grid)->kind == FM_UTAB)
					addForm(symbol, pt);
				else
					addColumn(grid, symbol);
			}
			else
			{
				addForm(symbol, pt, NULL, ii);
				pt.y += symbol->m_datasz.cy;
			}
		}
		m_mapH->modified = true;
	}
	if (child->m_mainFRM->m_status & AWS_TDRAG)
	{
		child->m_mainFRM->m_wndWorkBar.m_TreeTemplate.loadTemplate(pt);
		m_mapH->modified = true;
	}
}

void CChildView::spaceEqual(int std, long pos, long maxV, int count, bool horizontal)
{
	int 	ii, minF, delta;
	long	minV;

	if (horizontal)
	{
		minV = maxV;
		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block && !m_pFormItem->getFormPoint(ii)->drawn)
				minV = __min(minV, m_pFormItem->getFormPoint(ii)->left);
		}

		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block && (minV == m_pFormItem->getFormPoint(ii)->left))
			{
				minF = ii;
				count++;
				break;
			}
		}

		if (count == 1)
		{
			pos = m_pFormItem->getFormPoint(minF)->left;
			m_pFormItem->getFormPoint(minF)->drawn = true;
		}
		else
		{
			delta = m_pFormItem->getFormPoint(minF)->right - m_pFormItem->getFormPoint(minF)->left;
			m_pFormItem->getFormPoint(minF)->left  = pos + std;
			m_pFormItem->getFormPoint(minF)->right = m_pFormItem->getFormPoint(minF)->left + delta;
			pos = m_pFormItem->getFormPoint(minF)->left;
			m_pFormItem->getFormPoint(minF)->drawn = true;
		}
	}
	else
	{
		minV = maxV;
		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block && !m_pFormItem->getFormPoint(ii)->drawn)
				minV = __min(minV, m_pFormItem->getFormPoint(ii)->top);
		}

		for (ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			if (m_pFormItem->getFormPoint(ii)->block && (minV == m_pFormItem->getFormPoint(ii)->top))
			{
				minF = ii;
				count++;
				break;
			}
		}

		if (count == 1)
		{
			pos = m_pFormItem->getFormPoint(minF)->top;
			m_pFormItem->getFormPoint(minF)->drawn = true;
		}
		else
		{
			delta = m_pFormItem->getFormPoint(minF)->bottom - m_pFormItem->getFormPoint(minF)->top;
			m_pFormItem->getFormPoint(minF)->top    = pos + std;
			m_pFormItem->getFormPoint(minF)->bottom = m_pFormItem->getFormPoint(minF)->top + delta;
			pos = m_pFormItem->getFormPoint(minF)->top;
			m_pFormItem->getFormPoint(minF)->drawn = true;
		}
	}	

	if (count < m_block->count)
		spaceEqual(std, pos, maxV, count, horizontal);
}

CRect CChildView::getRect(int fN, int flag)
{
	CRect	tRc;

	if (fN >= m_pFormItem->getFormCount() || fN < 0)
		tRc.SetRectEmpty();
	else if (m_pFormItem->getFormPoint(fN)->group)
		tRc = GroupRect(m_pFormItem->getFormPoint(fN)->group);
	else
		tRc.SetRect(m_pFormItem->getFormPoint(fN)->left, m_pFormItem->getFormPoint(fN)->top, m_pFormItem->getFormPoint(fN)->right, m_pFormItem->getFormPoint(fN)->bottom);

	if (flag == 1 && m_pFormItem->getFormPoint(fN)->tabN)
	{
		struct _formR *tabN = m_pFormItem->getFormPoint(fN)->tabN;

		if (IsInPage(fN) && !tabN->group)
		{
			if (tabN->tabN)
				tRc.IntersectRect(tRc, getRect(m_pFormItem->getIdxOfPtr(tabN), 1));
			else
				tRc.IntersectRect(tRc, getRect(m_pFormItem->getIdxOfPtr(tabN), 0));
		}
		else
			tRc.SetRectEmpty();
	}
	return tRc;
}

void CChildView::deleteFORMs()
{
	if (m_selF < 0)
		return;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
	CRect	pRc;

	pRc.left  = m_mapH->width;
	pRc.top   = m_mapH->height;
	pRc.right = pRc.bottom = 0;

	if (m_block->count > 1)
	{
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii) && m_pFormItem->getFormPoint(ii)->block)
			{
				struct _formR* nGroup = m_pFormItem->getFormPoint(ii)->group;
				pRc.UnionRect(pRc, getFormRect(ii));
				if (nGroup)
				{
					for (int jj = m_pFormItem->getFormCount() - 1; jj >= 0; jj--)
						if (m_pFormItem->getFormPoint(jj)->group == nGroup)
							jj = deleteForm(jj);
				}
				else
					ii = deleteForm(ii);
			}
		}

		m_tool->ReleaseBlock();
	}
	else
	{
		pRc = getFormRect(m_selF);
		if (m_pFormItem->getFormPoint(m_selF)->group)
		{
			struct _formR* nGroup = m_pFormItem->getFormPoint(m_selF)->group;
			for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
				if (m_pFormItem->getFormPoint(ii)->group == nGroup)
					ii = deleteForm(ii);
		}
		else
			deleteForm(m_selF);
	}

	sortTabOrder();
	sortIoOrder();
	pRc.InflateRect(5, 5);
	InvalidateRect(pRc);
	m_selF = -1;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

void CChildView::keyArrowLeft()
{
	if (m_selF < 0)
		return;

	CRect	pRc;

	pRc.SetRectEmpty();
	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
		pRc = SizeForm(-1, 0);
	else		// Move
		pRc = MoveForm(-1, 0);

	m_mapH->modified = true;
	pRc.InflateRect(5, 5);
	if (!pRc.IsRectEmpty())
		InvalidateRect(pRc);
}

void CChildView::keyArrowRight()
{
	if (m_selF == -1)
		return;

	CRect	pRc;

	pRc.SetRectEmpty();
	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
		pRc = SizeForm(1, 0);
	else		// Move
		pRc = MoveForm(1, 0);

	m_mapH->modified = true;
	pRc.InflateRect(5, 5);
	if (!pRc.IsRectEmpty())
		InvalidateRect(pRc);
}

void CChildView::keyArrowUp()
{
	if (m_selF == -1)
		return;

	CRect	pRc;

	pRc.SetRectEmpty();
	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
		pRc = SizeForm(0, -1);
	else	// Move
		pRc = MoveForm(0, -1);

	m_mapH->modified = true;
	pRc.InflateRect(5, 5);
	if (!pRc.IsRectEmpty())
		InvalidateRect(pRc);
}

void CChildView::keyArrowDown()
{
	if (m_selF == -1)
		return;

	CRect	pRc;

	pRc.SetRectEmpty();
	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
		pRc = SizeForm(0, 1);
	else	// Move
		pRc = MoveForm(0, 1);

	m_mapH->modified = true;
	pRc.InflateRect(5, 5);
	if (!pRc.IsRectEmpty())
		InvalidateRect(pRc);
}

void CChildView::showScripts()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_EDIT_SCRIPT, 0);
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	CRect rc;
	GetClientRect(&rc);
	if (m_bFirstDrawing || m_bRedraw)
	{
		drawBitmap(&dc);
		if (m_bFirstDrawing)
			m_bFirstDrawing = false;
	}
	
	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap;
		
		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		
		dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
}

int CChildView::getTableForm(CPoint point)
{
	CRect	tRc;

	for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
	{
		switch (m_pFormItem->getFormPoint(ii)->kind)
		{
		case FM_TAB:
		case FM_UTAB:
		case FM_GRID:
		case FM_TABLE:	
			if (m_pFormItem->getFormPoint(ii)->tabN)
			{
				if (IsInPage(ii))
					break;
				else
					continue;
			}
			break;
		default:	continue;
		}

		tRc.SetRect(m_pFormItem->getFormPoint(ii)->left, m_pFormItem->getFormPoint(ii)->top, m_pFormItem->getFormPoint(ii)->right, m_pFormItem->getFormPoint(ii)->bottom);
		if (tRc.PtInRect(point))
			return ii;
	}

	return (-1);
}

bool CChildView::movable(CPoint point)
{
	if (m_pFormItem->getFormPoint(m_selF)->auxR == (char *)0)
		return false;

	CRect	tRc;
	int	ii, width;
	struct	_repR*	repR;

	tRc.SetRect(m_pFormItem->getFormPoint(m_selF)->left, m_pFormItem->getFormPoint(m_selF)->top, m_pFormItem->getFormPoint(m_selF)->right, m_pFormItem->getFormPoint(m_selF)->bottom);
	if (!(m_pFormItem->getFormPoint(m_selF)->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	width = tRc.left;
	repR = (struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	switch (m_pFormItem->getFormPoint(m_selF)->kind)
	{
	case FM_GRID:
		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR	*cellR = cellItem->getCellPoint(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;
			if (ii < m_pFormItem->getFormPoint(m_selF)->scrollPos)
				continue;

			width += cellR->width;
			if (width >= tRc.right)
			{
				delete cellItem;
				return false;
			}

			if (point.x >= width-2 && point.x <= width+2)
			{
				if ((m_pFormItem->getFormPoint(m_selF)->attr2 & GO_HSCR) &&
					point.y >= m_pFormItem->getFormPoint(m_selF)->bottom - szSCROLL)
				{
					delete cellItem;
					return false;
				}
				delete cellItem;
				return true;
			}

			if (cellR->attr & FA_WRAP)
				width = tRc.left;
		}
		break;

	case FM_TABLE:
		if (!(m_pFormItem->getFormPoint(m_selF)->attr2 & GO_MULTI) || !repR->rows)
		{
			delete cellItem;
			return false;
		}

		struct _cellR	*cellR = cellItem->getCellPoint(0);
		tRc.left += cellR->width - 1;
		tRc.right = tRc.left;

		tRc.InflateRect(2, 0);
		if (tRc.PtInRect(point))
		{
			delete cellItem;
			return true;
		}
		break;
	}
	delete cellItem;

	return false;
}

bool CChildView::selectable(CPoint point)
{
	if (m_selF < 0)
		return false;
	struct _formR *formR = m_pFormItem->getFormPoint(m_selF);

	if (formR->kind == FM_TAB || formR->kind == FM_UTAB)
	{
		CRect tRc = getFormRect(m_selF);
		CPoint tPt;
		bool	rtVal = false;
		int nID = (formR->kind == FM_TAB?ID_TOOL_TAB:ID_TOOL_USERTAB);
		
		((CtlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_fN = m_selF;
		((CtlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_dc = GetDC();

		if (nID == ID_TOOL_TAB && ((CtlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->Selectable(point, tRc, tPt) >= 0)
			rtVal = true;
		if (nID == ID_TOOL_USERTAB && ((CtlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->SelectableUTab(point, tRc, tPt) >= 0)
			rtVal = true;
		
		ReleaseDC(((CtlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_dc);
		((CtlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_dc = NULL;
		return rtVal;
	}
	if (formR->auxR == (char *)0)
		return false;

	CRect	tRc, rc;
	int	ii, width, line, multi = 0, drawRows, headHeight;
	struct	_repR*	repR;

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	if (!(formR->attr2 & GO_X3D))
		tRc.DeflateRect(2, 2);

	width = tRc.left;
	repR = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	switch (formR->kind)
	{
	case FM_GRID:
		line = 0;

		for (ii = 0; ii < repR->cols; ii++)
		{
			if (formR->attr2 & GO_MULTI)
				multi = max(multi, (int)cellItem->getCellPoint(ii)->lineN);
		}
		multi++;

		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->getCellPoint(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;
			if (line != cellR->lineN)
			{
				line++;
				width = tRc.left;
			}

			drawRows = repR->rows;
			headHeight = LOWORD(formR->keys);
			if (formR->attr2 & GO_AUTOSIZE)
			{
				drawRows = HIWORD(formR->keys);
				headHeight = 0;
				if (drawRows)
					headHeight = (formR->bottom - formR->top) / (drawRows * multi);
			}

			int height = tRc.top + line * headHeight;
			rc.SetRect(width, height,
				width + cellR->width,
				headHeight + height);

			if (rc.PtInRect(point))
			{
				delete cellItem;
				return true;
			}
						
			width += cellR->width;
			if (width >= tRc.right)
				width  = tRc.right;
		}
		if (formR->attr2 & GO_HSCR)
		{
			CRect tRc1, tRc2;
			if  (formR->attr2 & GO_VSCR)
				tRc.right -= szSCROLL;
			
			if (tRc.Height() > szSCROLL)
				tRc.top = tRc.bottom - szSCROLL;
			
			tRc1 = tRc2 = tRc;
			if (tRc.Width() < szSCROLL*2)
			{
				tRc1.right = tRc.left + tRc.Width()/2;
				tRc2.left  = tRc.left + tRc.Width()/2+1;
			}
			else
			{
				tRc1.right = tRc.left + szSCROLL;
				tRc2.left  = tRc.right -szSCROLL;
			}
			if (tRc1.PtInRect(point) || tRc2.PtInRect(point))
			{
				delete cellItem;
				return true;
			}
		}
		break;
	case FM_TABLE:
		if (!(formR->attr2 & GO_MULTI) || !repR->rows)
		{
			delete cellItem;
			return false;
		}

		struct _cellR	*cellR = cellItem->getCellPoint(0);
		tRc.right = tRc.left + cellR->width;
		/*if (m_pFormItem->getFormPoint(m_selF)->keys < 3)
			tRc.InflateRect(2, 0);*/
		if (tRc.PtInRect(point))
		{
			delete cellItem;
			return true;
		}
		break;
	}

	delete cellItem;
	return false;
}

bool CChildView::optimumSize(CPoint point)
{
	int	toolN;

	switch (m_pFormItem->getFormPoint(m_selF)->kind)
	{
	case FM_GRID:
		toolN = ID_TOOL_GRID - ID_TOOL_BASE;	break;
	case FM_TABLE:
		toolN = ID_TOOL_TABLE - ID_TOOL_BASE;	break;
	default:
		return false;
	}
	CTOOL*	tool = (CTOOL *)m_toolOBs.GetAt(toolN);
	return tool->OptimumSize(point);
}

void CChildView::addColumn(int grid, Csymbol* symbol)
{
	CRect	rRc;
	int	jj;
	struct	_repR*	repR;
	CChildFrame* child = (CChildFrame *)m_pChildFrame;

	m_selF = grid;
	repR   = (struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	int ii = cellItem->addCell();
	struct _cellR	*cellR = cellItem->getCellPoint(ii);

	strcpy_s(cellR->name, symbol->m_name);
	strcpy_s(cellR->head, symbol->m_head);
	cellR->size = symbol->m_size;
	cellR->type = symbol->m_type;
	cellR->iok  = symbol->m_iok;
	if (child->m_mainFRM->m_wndWorkBar.m_SymTbl.m_default)
	{
		switch (symbol->m_kind)
		{
		case FM_COMBO:
			cellR->kind = CK_COMBO;	break;

		case FM_BUTTON:
			if (symbol->m_type == BTN_CHECK)
			{
				cellR->type   = IO_NUM;
				cellR->kind = CK_CHECK;
			}
		default:
			break;
		}
	}

	switch (m_pFormItem->getFormPoint(m_selF)->kind)
	{
	case FM_GRID:
		repR->cols++;
		cellR->width = (WORD)symbol->m_headsz.cx;
		m_pFormItem->getFormPoint(m_selF)->keys  = MAKELONG((WORD)symbol->m_headsz.cy, HIWORD(m_pFormItem->getFormPoint(m_selF)->keys));
		cellR->width = (WORD)max(cellR->width, symbol->m_datasz.cx);
		m_pFormItem->getFormPoint(m_selF)->keys  = MAKELONG((DWORD)max((int)m_pFormItem->getFormPoint(m_selF)->keys, symbol->m_datasz.cy), HIWORD(m_pFormItem->getFormPoint(m_selF)->keys));
		break;

	case FM_TABLE:
		repR->rows++;
		cellItem->getCellPoint(0)->width  = (WORD)max(cellItem->getCellPoint(0)->width, symbol->m_headsz.cx);
		for (jj = 1; jj <= ii; jj++)
			cellItem->getCellPoint(jj)->width = cellItem->getCellPoint(0)->width;
		break;
	}
	
	cellR->alignH = AL_CENTER;
	cellR->alignC = symbol->m_align;
	cellR->hpRGB  = symbol->m_hpRGB;
	cellR->htRGB  = symbol->m_htRGB;
	cellR->pRGB   = symbol->m_pRGB;
	cellR->tRGB   = symbol->m_tRGB;
	cellR->attr   = symbol->m_attr;
	if (cellR->attr & FA_CORGB)
	{
		cellR->attr2 |= SA_COSUP;
		cellR->isEdit =IE_CORGB;
		cellR->onEdit = MAKELONG(CORGB_DOWN, CORGB_UP);
	}
	
	if (cellR->type == IO_NUM && cellR->iok == EIO_OUTPUT)
		cellR->vals = symbol->m_float;
	
	if (!symbol->m_edit.IsEmpty())
	{
		cellR->isEdit = IE_EDIT;
		strcpy_s(cellR->editS, symbol->m_edit);
	}
	//cellR->headN  = 1;
	cellR->properties |= PR_VISIBLE;

	delete cellItem;
	rRc.SetRect(m_pFormItem->getFormPoint(m_selF)->left, m_pFormItem->getFormPoint(m_selF)->top, m_pFormItem->getFormPoint(m_selF)->right, m_pFormItem->getFormPoint(m_selF)->bottom);
	rRc.InflateRect(5, 5);
	InvalidateRect(&rRc);
}

void CChildView::addForm(Csymbol* symbol, CPoint pt, struct _formR *tabN, int idx)	// label + data
{
	if (m_pFormItem->getFormCount() >= MAX_FORM)
		return;

	CRect	cRc;
	CChildFrame* child = (CChildFrame *) m_pChildFrame;

	if (child->m_mainFRM->m_wndWorkBar.m_SymTbl.m_default && symbol->m_kind <= 0)
		return;

	GetClientRect(&cRc);
	
	if (symbol->m_iok == EIO_OUTPUT)
	{
		if (idx == 0)
			m_pFormItem->addForm();
		// table
		m_selF = m_pFormItem->getFormCount() - 1;
		struct _formR *formR = m_pFormItem->getFormPoint(m_selF);
		formR->kind = FM_TABLE;
		if (idx == 0)
		{
			m_pFormItem->setFormStr(m_selF, "name", getInitSymbol());
			formR->top       = pt.y;
			formR->left      = pt.x;
		}
		formR->bottom    = min(cRc.bottom, formR->top + symbol->m_headsz.cy * (idx + 1));
		formR->right     = min(cRc.right,  formR->left + symbol->m_headsz.cx + symbol->m_datasz.cx);
		formR->iok       = EIO_OUTPUT;
		formR->attr2     = GO_MULTI;
		formR->attr2	|= GO_AUTOSIZE;
		formR->properties |= PR_VISIBLE;
		formR->properties |= PR_ENABLE;
		formR->properties |= PR_CHECKED;
		formR->checked = 0;
		formR->fStyle = 0;
		
		formR->onClick    = -1;
		formR->onClickL   = 0;
		formR->onDblClk   = -1;
		formR->onDblClkL  = 0;
		formR->onChange   = -1;
		formR->onChangeL  = 0;
		formR->onChar     = -1;
		formR->onCharL    = 0;
		formR->onKey      = -1;
		formR->onKeyL     = 0;

		sortTabOrder();
		formR->index = ++m_nSeqTabOdr;
		formR->iorder = ++m_nSeqIoOdr;

		if (!m_pFormItem->getFormPoint(m_selF)->auxR)
		{
			struct _repR *repR;
			m_pFormItem->getFormPoint(m_selF)->auxR = new char[sizeof(_repR)];
			repR = (struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR;
			ZeroMemory(repR, sizeof(struct _repR));
			repR->selected = -1;
		}
		addColumn(m_selF, symbol);
		formR->keys	 = 20;
		
		formR->tabN = tabN;
		if (tabN)
			formR->pageN = tabN->checked;
	}
	else
	{
		m_pFormItem->addForm();
		// label
		if (symbol->m_headsz.cx != 0 && symbol->m_headsz.cy != 0)
		{
			m_selF = m_pFormItem->getFormCount() - 1;
			struct _formR *formR = m_pFormItem->getFormPoint(m_selF);
			formR->kind = FM_LABEL;
			
			m_pFormItem->setFormStr(m_selF, "name", getInitSymbol());
			formR->top       = pt.y;
			formR->left      = pt.x;
			formR->bottom    = min(cRc.bottom, formR->top + symbol->m_headsz.cy);
			formR->right     = min(cRc.right,  formR->left + symbol->m_headsz.cx);
			pt.x = formR->right;
			m_pFormItem->setFormStr(m_selF, "str", symbol->m_head);
			formR->alignment = AL_CENTER;
			formR->pRGB      = symbol->m_hpRGB;
			formR->tRGB      = symbol->m_htRGB;
			
			formR->properties |= PR_VISIBLE;
			formR->properties |= PR_ENABLE;
			formR->properties |= PR_CHECKED;
			formR->fPoint  = m_mapH->fPoint;
			m_pFormItem->setFormStr(m_selF, "fName", m_mapH->fName);
			
			formR->fStyle = 0;
			formR->checked   =  0;
			formR->onClick   = -1;
			formR->onClickL  =  0;
			formR->onDblClk  = -1;
			formR->onDblClkL =  0;
			formR->onChange  = -1;
			formR->onChangeL =  0;
			formR->onChar    = -1;
			formR->onCharL   =  0;
			formR->onKey     = -1;
			formR->onKeyL    =  0;

			sortTabOrder();
			formR->index = ++m_nSeqTabOdr;
			
			formR->tabN = tabN;
			if (tabN)
				formR->pageN = tabN->checked;
		}
		
		// data
		m_pFormItem->addForm();
		m_selF = m_pFormItem->getFormCount() - 1;
		struct _formR *formR = m_pFormItem->getFormPoint(m_selF);
		
		m_pFormItem->setFormStr(m_selF, "name", symbol->m_name);
		
		if (child->m_mainFRM->m_wndWorkBar.m_SymTbl.m_default)
			formR->kind = symbol->m_kind;
		else
		{
			switch (symbol->m_iok)
			{
			case EIO_INPUT:
			case EIO_INOUT:
				formR->kind = FM_EDIT;		break;
			case EIO_OUTPUT:
			default:
				formR->kind = FM_OUT;		break;
			}
		}
		formR->size      = symbol->m_size;
		formR->type      = symbol->m_type;
		formR->iok       = symbol->m_iok;
		formR->top       = pt.y;
		formR->left      = pt.x + 1;
		formR->bottom    = min(cRc.bottom, formR->top + symbol->m_datasz.cy);
		formR->right     = min(cRc.right, formR->left + symbol->m_datasz.cx);
		formR->alignment = symbol->m_align;
		formR->pRGB      = symbol->m_pRGB;
		formR->tRGB      = symbol->m_tRGB;
		formR->attr      = symbol->m_attr;
		if (formR->attr & FA_CORGB)
		{
			formR->attr2 |= SA_COSUP;
			formR->isEdit = IE_CORGB;
			formR->onEdit = MAKELONG(CORGB_DOWN, CORGB_UP);
		}
		
		if (formR->type == IO_NUM && formR->kind == FM_OUT)
			formR->offs = symbol->m_float = symbol->m_float;
		
		if (!symbol->m_edit.IsEmpty())
		{
			formR->isEdit = IE_EDIT;
			m_pFormItem->setFormStr(m_selF, "editS", symbol->m_edit);
		}
		
		formR->properties |= PR_VISIBLE;
		formR->properties |= PR_ENABLE;
		formR->properties |= PR_CHECKED;
		formR->fPoint  = m_mapH->fPoint;
		m_pFormItem->setFormStr(m_selF, "fName", m_mapH->fName);
		
		formR->fStyle = 0;
		
		formR->checked   =  0;
		formR->onClick   = -1;
		formR->onClickL  =  0;
		formR->onDblClk  = -1;
		formR->onDblClkL =  0;
		formR->onChange  = -1;
		formR->onChangeL =  0;
		formR->onChar    = -1;
		formR->onCharL   =  0;
		formR->onKey     = -1;
		formR->onKeyL    =  0;
		
		formR->tabN = tabN;
		if (tabN)
			formR->pageN = tabN->checked;
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	Invalidate(FALSE);
}

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_selected)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return TRUE;
	}
	switch(m_nCursor)
	{
	case CURSORNWSE:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
		return TRUE;
	case CURSORNESW:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENESW));
		return TRUE;
	case CURSORWE:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		return TRUE;
	case CURSORNS:
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		return TRUE;
	}

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CChildView::ShowMenu()
{
	CPoint	point;
	CMenu	menu, submenu;
	struct _formR *formR = NULL;
	if (m_selF >= 0)
		formR = m_pFormItem->getFormPoint(m_selF);
	
	menu.CreatePopupMenu();
	submenu.CreatePopupMenu();

	GetCursorPos(&point);
	submenu.AppendMenu(MF_STRING, ID_GROUPMAKE, "Set Group");
	submenu.AppendMenu(MF_STRING, ID_DISGROUPING, "Release Group");
	
	if (m_selF >= 0)
	{
		menu.AppendMenu(MF_STRING, NULL, formR->name);
		menu.AppendMenu(MF_SEPARATOR);
	}
	menu.AppendMenu(MF_STRING, ID_RSENDTOFIRST, "Bring Foreward\t(Ctrl+F)");
	menu.AppendMenu(MF_STRING, ID_RSENDTOEND, "Send Backward\t(Ctrl+B)");
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_SHOWSCRIPT, "Script\t(F6)");
	if (m_block->count <= 1)
		menu.AppendMenu(MF_STRING, ID_SHOWATTRIBUTE, "Properties\t(F8)");
	menu.AppendMenu(MF_POPUP, UINT(submenu.GetSafeHmenu()),"Group");
	
	if (m_selF >= 0 && m_block->count <= 1)
	{
		switch(formR->kind)
		{
		case FM_GRID:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_COLUMNEDIT, "Column Edit\t(F10)");
			menu.AppendMenu(MF_STRING, ID_ADDITEM, "Add Item");
			menu.AppendMenu(MF_STRING, ID_COLUMNFIT, "Column Fit");
			if (formR->attr2 & GO_XHEAD)
				menu.AppendMenu(MF_STRING, ID_SELECTCOLUMN, "Select Column");
			break;
		case FM_TABLE:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_COLUMNEDIT, "Row Edit\t(F10)");
			menu.AppendMenu(MF_STRING, ID_ADDITEM, "Add Item");
			if (!(formR->attr2 & GO_MULTI))
				menu.AppendMenu(MF_STRING, ID_SELECTCOLUMN, "Select Row");
			break;
		case FM_TAB:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_ADDITEM, "Add Item");
			menu.AppendMenu(MF_STRING, ID_EXTMENUID, "Delete Item\t(~Del)");
			break;
		case FM_UTAB:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_ADDITEM, "Add Item");
			menu.AppendMenu(MF_STRING, ID_EXTMENUID, "PageEdit\t(F10)");
			break;
		case FM_EDIT:
		case FM_COMBO:
		case FM_OUT:
			menu.AppendMenu(MF_STRING, ID_EXTMENUID, "Attributes...\t(F10)");
			break;
		case FM_CONTROL:
			menu.AppendMenu(MF_STRING, ID_EXTMENUID, "Control\t(F10)");
			break;
		case FM_BUTTON:
			menu.AppendMenu(MF_STRING, ID_COLUMNFIT, "Image Fit");
			break;
		case FM_OBJECT:
			{
				CString sMap = m_pFormItem->getFormStr(m_selF, "str");
				if (!sMap.IsEmpty() && sMap.GetLength() == 8)
				{
					menu.AppendMenu(MF_STRING, ID_EXTMENUID, "Open Map \"" + sMap + "\"");
					if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISOFFLINE))
						menu.EnableMenuItem(ID_EXTMENUID, MF_GRAYED);
				}
			}
			break;
		}
	}

	if (!(m_selF >= 0 && (formR->kind == FM_GRID ||
		formR->kind == FM_TABLE) && !formR->group))
		menu.EnableMenuItem(ID_COLUMNEDIT, MF_GRAYED);
	
	if (m_selF == -1)
	{
		menu.EnableMenuItem(ID_RSENDTOFIRST, MF_GRAYED);
		menu.EnableMenuItem(ID_RSENDTOEND, MF_GRAYED);
	}
	else if (formR->kind == FM_TAB)
	{
		menu.EnableMenuItem(ID_RSENDTOFIRST, MF_GRAYED);
	}

	if (!(m_selF >= 0 && formR->group))
		submenu.EnableMenuItem(ID_DISGROUPING, MF_GRAYED);
	if (!(m_selF >= 0 && m_block->count > 1 && formR->group == 0))
		submenu.EnableMenuItem(ID_GROUPMAKE, MF_GRAYED);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				point.x, point.y, this);
}

void CChildView::OnRsendtofirst() 
{
	if (m_selF == m_pFormItem->getFormCount() - 1 || m_selF < 0)
		return;
	m_pFormItem->moveForm(m_selF, m_pFormItem->getFormCount());
	m_selF = m_pFormItem->getFormCount() - 1;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	Invalidate(FALSE);
}

void CChildView::OnRsendtoend() 
{
	if (m_selF <= 0)
		return;
	if (m_pFormItem->getFormPoint(m_selF)->tabN)
		m_pFormItem->moveForm(m_selF, m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->tabN) + 1);
	else
		m_pFormItem->moveForm(m_selF, 0);
	m_selF = 0;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	Invalidate(FALSE);
}

void CChildView::OnViewCommand(UINT cmdID)
{
	switch (cmdID)
	{
	case ID_RSENDTOFIRST:
		OnRsendtofirst();
		break;
	case ID_RSENDTOEND:
		OnRsendtoend();
		break;
	case ID_SHOWSCRIPT:
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_EDIT_SCRIPT, 0);
		break;
	case ID_SHOWATTRIBUTE:
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSPRO, 0);
		break;
	case ID_COLUMNEDIT:
		if (m_selF >= 0 && m_block->count <= 1)
		{
			if (m_pFormItem->getFormPoint(m_selF)->kind != FM_GRID &&
				m_pFormItem->getFormPoint(m_selF)->kind != FM_TABLE)
				break;
			CGridColumnDlg	Dlg(this, m_mapH, m_selF);
			Dlg.DoModal();
			Invalidate();
		}
		break;
	case ID_GROUPMAKE:
		OnGroupMake();
		break;
	case ID_DISGROUPING:
		OnDisGrouping();
		break;
	case ID_ADDITEM:
		OnAddItem();
		break;
	case ID_SELECTCOLUMN:
		OnSelectColumn();
		break;
	case ID_COLUMNFIT:
		OnColumnFit();
		break;
	case ID_EXTMENUID:
		OnExtMenu();
		break;
	}
}

void CChildView::OnGroupMake()
{
	int nGroup = -1;
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->block && m_pFormItem->getFormPoint(ii)->group)
		{
			if (nGroup == -1 && MessageBox("It is Lost that Group Information in Group!\n\nDo you make Group?", "Builder", MB_YESNO) == IDNO)
				return;
			nGroup = (int)m_pFormItem->getFormPoint(ii)->group;
			for (int jj = 0; jj < m_pFormItem->getFormCount(); jj++)
				if ((int)m_pFormItem->getFormPoint(jj)->group == nGroup)
				{
					m_pFormItem->getFormPoint(jj)->group = 0;
					m_pFormItem->getFormPoint(jj)->block = true;
				}
		}
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->block)
		{
			m_pFormItem->getFormPoint(ii)->group = m_pFormItem->getFormPoint(m_stdF);
			m_pFormItem->getFormPoint(ii)->block = false;
		}
	m_block->count = 0;
	CRect tRc = GroupRect(m_pFormItem->getFormPoint(m_stdF));
	tRc.InflateRect(5, 5);
	InvalidateRect(tRc);
}

void CChildView::OnDisGrouping()
{
	CRect tRc = GroupRect(m_pFormItem->getFormPoint(m_selF));
	m_stdF = m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->group);
	m_block->count = 0;
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		if (m_pFormItem->getFormPoint(ii)->group == m_pFormItem->getFormPoint(m_stdF))
		{
			m_pFormItem->getFormPoint(ii)->group = 0;
			m_pFormItem->getFormPoint(ii)->block = true;
			m_block->count++;
		}
	tRc.InflateRect(5, 5);
	InvalidateRect(tRc);
}

void CChildView::OnAddItem()
{
	if (m_selF < 0)
		return;
	switch(m_pFormItem->getFormPoint(m_selF)->kind)
	{
	case FM_GRID:
	case FM_TABLE:
		addCol();
		break;
	case FM_TAB:
	case FM_UTAB:
		addPage(m_pFormItem->getFormPoint(m_selF)->checked);
		break;
	}
}

CRect CChildView::GroupRect(struct _formR* nGroup)
{
	CRect rc, tRc;
	rc.SetRectEmpty();
	for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
	{
		struct _formR* formR = m_pFormItem->getFormPoint(ii);
		if (formR->group == nGroup)
		{
			tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
			rc.UnionRect(rc, tRc);
		}
	}
	return rc;
}

CRect CChildView::getFormRect(int idx)
{
	if (m_pFormItem->getFormCount() <= idx)
		return CRect(0);
	CRect tRc;
	struct _formR* formR = m_pFormItem->getFormPoint(idx);
	if (!formR)
		return CRect(0);
	if (formR->group)
		return GroupRect(formR->group);
	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	return tRc;
}

int CChildView::deleteForm(int idx)
{
	if (m_pFormItem->getFormPoint(idx)->kind == FM_TAB)
	{
		struct _formR *tabN = m_pFormItem->getFormPoint(idx);
		m_pFormItem->removeForm(idx);
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
			if (m_pFormItem->getFormPoint(ii)->tabN == tabN)
			{
				if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
					ii = deleteForm(ii);
				else
					m_pFormItem->removeForm(ii);
			}
		return m_pFormItem->getFormCount();
	}
	m_pFormItem->removeForm(idx);
	return m_pFormItem->getFormCount();
}

void CChildView::SetGap(int nGap)
{
	m_nGap = nGap;
	if (m_toolOBs.GetSize())
		((CtlPointer*)m_toolOBs.GetAt(0))->m_nGap = nGap;
}

void CChildView::InvalidateRect2(int idx)
{
	if (idx >= 0 && m_block->count >= 2)
	{
		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				CRect tRc = getFormRect(ii);
				tRc.InflateRect(5, 5);
				if (m_pFormItem->getFormPoint(ii)->kind == FM_OBJECT &&
					m_pFormItem->getFormPoint(ii)->bFixedSize)
					Invalidate();
				else
					InvalidateRect(tRc);
			}
	}
	else if (idx >= 0)
	{
		if (m_pFormItem->getFormPoint(idx)->kind == FM_OBJECT &&
			m_pFormItem->getFormPoint(idx)->bFixedSize)
		{
			Invalidate();
			return;
		}
		CRect tRc = getFormRect(idx);
		tRc.InflateRect(5, 5);
		InvalidateRect(tRc);
	}
	else if (idx < 0)
	{
		Invalidate();
	}
}

CString CChildView::getControlName(int kind, int type)
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
	return "SYMBOL";
}

bool CChildView::isInTab(CPoint pnt, int &formN, int &selectedN)
{
	for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
	{
		struct _formR *formR = m_pFormItem->getFormPoint(ii);
		if (formR->group)
			continue;
		if (formR->tabN && !IsInPage(ii))
			continue;
		if (formR->kind != FM_TAB && !formR->tabN)
			continue;
		if (getFormRect(formR->kind != FM_TAB?m_pFormItem->getIdxOfPtr(formR->tabN):ii).PtInRect(pnt))
		{
			formN = formR->kind != FM_TAB?m_pFormItem->getIdxOfPtr(formR->tabN):ii;
			selectedN = m_pFormItem->getFormPoint(formR->kind != FM_TAB?m_pFormItem->getIdxOfPtr(formR->tabN):ii)->checked;
			return true;
		}
	}
	return false;
}

void CChildView::Preview()
{
	CWnd* pWizardCtrl = (CWnd*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (!pWizardCtrl)
		return;
	if (m_pPreView)
	{
		pWizardCtrl->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
					(BYTE *)(VTS_I4), m_Previewkey);

		if (m_pPreView)
			m_pPreView->PostMessage(WM_CLOSE);

		m_pPreView = NULL;
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
		return;
	}
	CRect rect;
	CString fname;
	CFileFind finder;

	GetClientRect(&rect);
	wccSetTempPath(fname, m_mapH->mapN, false);
	
	if (finder.FindFile(fname))
	{
		long size, wndType;
				
		wndType = vtypeERR;
		pWizardCtrl->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void*)&wndType, (BYTE*)(VTS_BSTR VTS_I4), m_mapH->mapN, &size);

		if (wndType == vtypeERR)
		{
			AfxMessageBox("Loading MapInfo Error!");
			if (m_pPreView)
			{
				m_pPreView->PostMessage(WM_CLOSE);
				m_pPreView = NULL;
			}
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
			return;
		}
		
		m_pPreView = new CPreviewWnd();
		m_pPreView->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, 0);
		pWizardCtrl->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void*)&m_Previewkey, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), (long)m_pPreView, wndType, 0);
		
		if (m_Previewkey)
		{
			BOOL bRetval;
			pWizardCtrl->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void*)&bRetval, (BYTE*)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), m_Previewkey, m_mapH->mapN, size, false);
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, FALSE);
			m_pPreView->SetFocus();
		}
		else
		{
			AfxMessageBox("Creating Map Key Error!");
			m_pPreView->PostMessage(WM_CLOSE);
			m_pPreView = NULL;
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
		}
	}
}

//
//	drawing
//

void CChildView::drawMap(CDC *pDC)
{
	CRect	tRc, clipRect, IntersectRc;
	CTOOL*	tool;

	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;

	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		struct _formR *formR = m_pFormItem->getFormPoint(ii);
		if (!m_showFORM && !(formR->properties & PR_VISIBLE))
			continue;

		tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
		if (!IntersectRc.IntersectRect(tRc, clipRect))
			continue;

		switch (formR->kind)
		{
		case FM_SYS:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_SYS - ID_TOOL_BASE);
			break;
	
		case FM_LABEL:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_LABEL - ID_TOOL_BASE);
			break;

		case FM_BOX:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_BOX - ID_TOOL_BASE);
			break;
	
		case FM_GROUP:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_GROUP - ID_TOOL_BASE);
			break;

		case FM_PANEL:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_PANEL - ID_TOOL_BASE);
			break;

		case FM_BUTTON:
			switch (formR->type)
			{
			case BTN_RADIO:
				tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_RADIO - ID_TOOL_BASE);
				break;
			case BTN_CHECK:
				tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_CHECK - ID_TOOL_BASE);
				break;
			case BTN_PUSH:
			case BTN_USER:
				tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_PUSH - ID_TOOL_BASE);
				break;
			default:
				continue;
			}
			break;

		case FM_EDIT:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_EDIT - ID_TOOL_BASE);
			break;

		case FM_COMBO:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_COMBO - ID_TOOL_BASE);
			break;

		case FM_OUT:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_OUT - ID_TOOL_BASE);
			break;
	
		case FM_MEMO:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_MEMO - ID_TOOL_BASE);
			break;
	
		case FM_GRID:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_GRID - ID_TOOL_BASE);
			break;

		case FM_TABLE:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_TABLE - ID_TOOL_BASE);
			break;

		case FM_TREE:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_TREE - ID_TOOL_BASE);
			break;

		case FM_AVI:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_AVI - ID_TOOL_BASE);
			break;
	
		case FM_GRAPH:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_GRAPH - ID_TOOL_BASE);
			break;

		case FM_OBJECT:
			CShow*	form;
			if (!m_forms.Lookup(ii, form))
				SetObject(ii);
			if (m_forms.Lookup(ii, form))
			{
				struct _formR *tabN = formR->tabN;
				if (tabN && !IsInPage(ii))
					break;
				if (form->Load(formR->str))
				{
					if (tabN)
					{
						if (!setTabRgn(pDC, ii, RGN_COPY))
						{
							pDC->SelectClipRgn(NULL);
							continue;
						}
					}
					else
						pDC->SelectClipRgn(NULL);
					if (formR->bFixedSize)
					{
						formR->right = formR->left + form->GetSize().cx;
						formR->bottom = formR->top + form->GetSize().cy;
						tRc.right = tRc.left + form->GetSize().cx;
						tRc.bottom = tRc.top + form->GetSize().cy;
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
					}
					form->Resize(tRc);
					pDC->IntersectClipRect(tRc);
					form->Draw(pDC);
					tool = NULL;
					break;
				}
			}
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_OBJECT - ID_TOOL_BASE);
			break;

		case FM_TAB:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_TAB - ID_TOOL_BASE);
			break;

		case FM_UTAB:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_USERTAB - ID_TOOL_BASE);
			break;

		case FM_SHEET:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_SHEET - ID_TOOL_BASE);
			break;

		case FM_BROWSER:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_BROWSER - ID_TOOL_BASE);
			break;

		case FM_CONTROL:
			tool = (CTOOL *)m_toolOBs.GetAt(ID_TOOL_CONTROL - ID_TOOL_BASE);
			break;

		default:
			continue;
		}

		if (!m_nTabOdr && !m_nIoOdr && formR->tabN)
		{
			if (!setTabRgn(pDC, ii, RGN_COPY))
			{
				pDC->SelectClipRgn(NULL);
				continue;
			}
		}
		else
			pDC->SelectClipRgn(NULL);
			
		if (tool)
			tool->Draw(pDC, ii);

		if (formR->block && !(formR->group && 
			m_pFormItem->getIdxOfPtr(formR->group) != ii) && m_block->count > 1)
		{
			tRc = getFormRect(ii);
			
			CControlTracker	multiTracker(tRc, (m_stdF >= 0 && m_stdF == ii)?CControlTracker::hatchedBorder | CControlTracker::resizeInside:CControlTracker::hatchedBorder);
			multiTracker.Draw(pDC);
		}
		pDC->SelectClipRgn(NULL);

		if (m_nTabOdr && formR->index)
		{
			CRect tRc;
			CString tStr;
			tRc.SetRect(formR->left, formR->top,
				formR->left + 20, formR->top + 20);
			pDC->FillSolidRect(tRc, RGB(0, 0, 120));
			tStr.Format("%d", formR->index);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(tStr, tRc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}

		if (m_nIoOdr && formR->iorder)
		{
			CRect tRc;
			CString tStr;
			tRc.SetRect(formR->left, formR->top,
				formR->left + 20, formR->top + 20);
			if (formR->attr & FA_SKIP)
				pDC->FillSolidRect(tRc, RGB(120, 120, 0));
			else
				pDC->FillSolidRect(tRc, RGB(0, 120, 0));
			tStr.Format("%d", formR->iorder);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(tStr, tRc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}
	
	if (m_block->count == 0 || m_block->count == 1)
	{
		if (m_selF != -1)
		{
			tRc = getFormRect(m_selF);
			if (m_pFormItem->getFormPoint(m_selF)->group)
				m_selF = m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(m_selF)->group);
			
			if (m_pFormItem->getFormPoint(m_selF)->tabN)
			{
				struct _formR *tabN = m_pFormItem->getFormPoint(m_selF)->tabN;
				CRgn rgn;
				CRect tRc = getFormRect(m_pFormItem->getIdxOfPtr(tabN));
				rgn.CreateRectRgn(tRc.left, tRc.top, tRc.right, tRc.bottom);
				pDC->SelectClipRgn(&rgn, RGN_COPY);
				rgn.DeleteObject();
			}
			CControlTracker	tracker(tRc, CControlTracker::resizeOutside);
			if (!m_bLBtnDown)
				tracker.Draw(pDC);
		}
	}
	else
	{
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
			if (m_pFormItem->getFormPoint(ii)->block && m_pFormItem->getFormPoint(ii)->group && 
				m_pFormItem->getIdxOfPtr(m_pFormItem->getFormPoint(ii)->group) == ii)
			{
				tRc = getFormRect(ii);
				
				CControlTracker	multiTracker(tRc, (m_stdF >= 0 && m_stdF == ii)?CControlTracker::hatchedBorder | CControlTracker::resizeInside:CControlTracker::hatchedBorder);
				multiTracker.Draw(pDC);
			}
	}
}

void CChildView::drawBitmap(CDC *pDC)
{
	CDC	memDC;
	CBitmap*	pOldBitmap;

	if (!m_pBitmap)
	{
		CRect	rc;
		GetClientRect(&rc);
		m_pBitmap = new CBitmap();
		m_pBitmap->CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	}
	
	memDC.CreateCompatibleDC(pDC);
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	drawBackground(&memDC);
	drawMap(&memDC);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

void CChildView::drawBackground(CDC *pDC)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	CBrush	*brush;

	bool	bBMP = false;
	switch (m_mapH->imageK)
	{
	case IK_BRUSH:
		DWORD	value;
		value = m_mapH->bkClr;
		if (value & userCOLOR)
			brush = new CBrush(value);
		else
			brush = new CBrush(m_palette->GetPaletteRGB(value));
		break;
	case IK_BMP:
		bBMP = true;
	case IK_NOP:
	default:
		brush = new CBrush(PALETTERGB(214, 214, 214));
		break;
	}

	CBrush* oldBrush = pDC->SelectObject(brush); 
	CRect rect; 
	pDC->GetClipBox(&rect); 

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(oldBrush); 

	brush->DeleteObject();
	delete brush;

	if (bBMP)	drawImage(pDC);
}

bool CChildView::drawImage(CDC *pDC)
{
	CString path = m_mapH->imageV;
	if (path.Find("\\") == -1)	path = m_root + "\\" + IMAGEDIR + "\\" + path;

	if (path == "")
	{
		m_sImgPath = "";
		if (m_hImgBack)
		{
			DeleteObject(m_hImgBack);
			m_hImgBack = NULL;
		}

		return false;
	}

	if (path != m_sImgPath)
	{
		if (m_hImgBack)
		{
			DeleteObject(m_hImgBack);
			m_hImgBack = NULL;
		}

		m_sImgPath = path;
		m_hImgBack = (HBITMAP)LoadImage(NULL, m_sImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (!m_hImgBack)	return false;

		BITMAP	bmp;
		CBitmap	bmpInfo;
		ZeroMemory(&bmp, sizeof(bmp));
		bmpInfo.Attach(m_hImgBack);
		bmpInfo.GetBitmap(&bmp);
		m_szWidth = bmp.bmWidth;
		m_szHeight = bmp.bmHeight;
		bmpInfo.Detach();
	}
	else if (!m_hImgBack)
	{
		m_hImgBack = (HBITMAP)LoadImage(NULL, m_sImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (!m_hImgBack)	return false;

		BITMAP	bmp;
		CBitmap	bmpInfo;
		ZeroMemory(&bmp, sizeof(bmp));
		bmpInfo.Attach(m_hImgBack);
		bmpInfo.GetBitmap(&bmp);
		m_szWidth = bmp.bmWidth;
		m_szHeight = bmp.bmHeight;
		bmpInfo.Detach();
	}

	CDC	mdcBACK;
	HBITMAP	sBACK;
	mdcBACK.CreateCompatibleDC(pDC);
	sBACK = (HBITMAP)::SelectObject(mdcBACK.m_hDC, m_hImgBack);

	CRect rect; 
	pDC->GetClipBox(&rect); 
	TransparentBlt(pDC->m_hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				mdcBACK.m_hDC, 0, 0, m_szWidth, m_szHeight, RGB(255, 255, 255));

	::SelectObject(mdcBACK.m_hDC, sBACK);
	mdcBACK.DeleteDC();

	return true;
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

void CChildView::sortTabOrder()
{
	if (m_selF < 0)
		return;
	CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	int nMax = 0;

	mapArr.RemoveAll();
	sortArr.RemoveAll();

	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		int kind = m_pFormItem->getFormPoint(ii)->kind;
		if (isValidTabOrder(ii))
		{
			int index = m_pFormItem->getFormPoint(ii)->index;
			if (index)
			{
				int tmp;
				while(mapArr.Lookup(index, tmp))
				{
					index = ++m_nSeqTabOdr;
					m_pFormItem->getFormPoint(ii)->index = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				index = ++m_nSeqTabOdr;
				m_pFormItem->getFormPoint(ii)->index = index;
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
		}
		else
			m_pFormItem->getFormPoint(ii)->index = 0;
	}
	
	for (int ii = 1; ii <= nMax; ii++)
	{
		int tmp;
		if (mapArr.Lookup(ii, tmp))
		{
			if (m_nTabOdr && m_selF == tmp)
				continue;
			sortArr.Add(tmp);
		}
	}
	if (m_nTabOdr)
	{
		sortArr.InsertAt(m_nTabOdr - 1, m_selF);
		
		if (m_nTabOdr <= m_pFormItem->getFormPoint(m_selF)->index && m_nTabOdr < sortArr.GetSize())
			m_nTabOdr++;
	}

	for (int ii = 0; ii < sortArr.GetSize(); ii++)
		m_pFormItem->getFormPoint(sortArr.GetAt(ii))->index = ii + 1;

	m_nSeqTabOdr = sortArr.GetSize();
	sortArr.RemoveAll();
	mapArr.RemoveAll();
}

void CChildView::sortIoOrder()
{
	if (m_selF < 0)
		return;
	CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	int nMax = 0, nEtcSeq = 0;
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		int kind = m_pFormItem->getFormPoint(ii)->kind;
		if (isValidIOOrder(ii))
		{
			int index = m_pFormItem->getFormPoint(ii)->iorder;
			if (index)
			{
				/*if (m_pFormItem->getFormPoint(ii)->attr & FA_SKIP)
				{
					m_pFormItem->getFormPoint(ii)->iorder = 0;
					continue;
				}*/
				
				if (kind == FM_TABLE)
				{
					if (!(m_pFormItem->getFormPoint(ii)->attr2 & GO_MULTI) && !(m_pFormItem->getFormPoint(ii)->attr2 & GO_XHEAD))
					{
						m_pFormItem->getFormPoint(ii)->iorder = 0;
						continue;
					}
				}
				int tmp;
				while(mapArr.Lookup(index, tmp))
				{
					index = ++m_nSeqIoOdr;
					m_pFormItem->getFormPoint(ii)->iorder = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				/*if (m_pFormItem->getFormPoint(ii)->attr & FA_SKIP)
					continue;*/
				
				if (kind == FM_TABLE)
				{
					if (!(m_pFormItem->getFormPoint(ii)->attr2 & GO_MULTI) && !(m_pFormItem->getFormPoint(ii)->attr2 & GO_XHEAD))
						continue;
				}
				index = ++nEtcSeq;
				mapArr.SetAt(index + m_pFormItem->getFormCount(), ii);
			}
		}
		else
			m_pFormItem->getFormPoint(ii)->iorder = 0;
	}
	for (int ii = 1; ii <= nMax; ii++)
	{
		int tmp;
		if (mapArr.Lookup(ii, tmp) && (!m_nIoOdr || tmp != m_selF))
			sortArr.Add(tmp);
	}
	for (int ii = 1; ii <= nEtcSeq; ii++)
	{
		int tmp;
		if (mapArr.Lookup(ii + m_pFormItem->getFormCount(), tmp) && (!m_nIoOdr || tmp != m_selF))
			sortArr.Add(tmp);
	}
	if (m_nIoOdr)
	{
		sortArr.InsertAt(m_nIoOdr - 1, m_selF);
		
		if (m_nIoOdr <= m_pFormItem->getFormPoint(m_selF)->iorder && m_nIoOdr < sortArr.GetSize())
			m_nIoOdr++;
	}

	for (int ii = 0; ii < sortArr.GetSize(); ii++)
		m_pFormItem->getFormPoint(sortArr.GetAt(ii))->iorder = ii + 1;

	m_nSeqIoOdr = sortArr.GetSize();
	sortArr.RemoveAll();
	mapArr.RemoveAll();
}

void CChildView::reDrawSelChange(int oldF, int selF)
{
	CRect tRc;
	if (m_block->count > 1 && (selF == -1 || !m_pFormItem->getFormPoint(selF)->block))
		m_tool->ReleaseBlock();

	if (oldF >= 0)
	{
		tRc = getFormRect(oldF);
		tRc.InflateRect(5, 5);
		InvalidateRect(tRc);
	}
	if (selF >= 0)
	{
		tRc = getFormRect(selF);
		tRc.InflateRect(5, 5);
		InvalidateRect(tRc);
	}
}

bool CChildView::setTabRgn(CDC *pDC, int idx, int nMode)
{
	struct _formR *tabN = m_pFormItem->getFormPoint(idx)->tabN;
	CRgn rgn;
	CRect tRc = getFormRect(m_pFormItem->getIdxOfPtr(tabN));
	rgn.CreateRectRgn(tRc.left, tRc.top, tRc.right, tRc.bottom);
	pDC->SelectClipRgn(&rgn, nMode);
	rgn.DeleteObject();
	if (tabN->tabN)
		if (!setTabRgn(pDC, m_pFormItem->getIdxOfPtr(tabN), RGN_AND))
			return false;
	if (m_pFormItem->getFormPoint(idx)->pageN != tabN->checked)
		return false;
	return true;
}

bool CChildView::IsInPage(int idx)
{
	struct _formR *formR = m_pFormItem->getFormPoint(idx);
	if (formR->tabN->checked != formR->pageN)
		return false;
	if (formR->tabN->tabN)
		return IsInPage(m_pFormItem->getIdxOfPtr(formR->tabN));
	return true;
}

void CChildView::deleteSelPage(int idx)
{
	struct _formR *formR = m_pFormItem->getFormPoint(idx);
	if (formR->checked >= 0)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		CString temp = m_pFormItem->getFormStr(idx, "dat"), tmp = "";
		CString temp2 = m_pFormItem->getFormStr(idx, "str"), tmp2 = "";
		for (int jj = 0; jj < (int)formR->keys; jj++)
		{
			if (formR->checked == jj)
			{
				Parser(temp, "\t");
				Parser(temp2, "\t");
				continue;
			}
			tmp += Parser(temp, "\t");
			tmp += '\t';
			if (formR->kind == FM_UTAB)
			{
				tmp2 += Parser(temp2, "\t");
				tmp2 += '\t';
			}
		}
		m_pFormItem->setFormStr(idx, "dat", tmp);
		if (formR->kind == FM_UTAB)
			m_pFormItem->setFormStr(idx, "str", tmp2);
		if (formR->keys > 0)
			formR->keys--;
		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii)->tabN == formR)
			{
				if (m_pFormItem->getFormPoint(ii)->pageN > formR->checked)
					m_pFormItem->getFormPoint(ii)->pageN--;
				else if (m_pFormItem->getFormPoint(ii)->pageN == formR->checked)
					ii = deleteForm(ii);
			}
		}
		if (formR->checked >= formR->keys)
			formR->checked = 0;
		InvalidateRect2(idx);
	}
}

CString CChildView::Parser(CString &srcstr, CString substr)
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

void CChildView::setBlockGroup(int nGroup, int mx, int my, bool bSize)
{
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		struct _formR *formR = m_pFormItem->getFormPoint(ii);
		if (formR->group == m_pFormItem->getFormPoint(nGroup)->group)
		{
			if (!bSize && formR->kind == FM_TAB)
				setBlockTab(formR, mx, my);
			if (!bSize)
			{
				formR->left   += mx;
				formR->top    += my;
			}
			formR->right  += mx;
			formR->bottom += my;
			if (formR->left > formR->right)
				formR->right = formR->left;
			if (formR->top > formR->bottom)
				formR->bottom = formR->top;
		}
	}
}

void CChildView::setBlockTab(_formR *tabN, int mx, int my, bool bSize)
{
	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (m_pFormItem->getFormPoint(ii)->tabN == tabN)
		{
			if (m_pFormItem->getFormPoint(ii)->kind == FM_TAB)
				setBlockTab(m_pFormItem->getFormPoint(ii), mx, my);
			m_pFormItem->getFormPoint(ii)->left   += mx;
			m_pFormItem->getFormPoint(ii)->top    += my;
			m_pFormItem->getFormPoint(ii)->right  += mx;
			m_pFormItem->getFormPoint(ii)->bottom += my;
		}
	}
}

CRect CChildView::MoveForm(int mx, int my, bool bSize)
{
	if (!(::GetKeyState(VK_CONTROL) & 0x8000) && m_nGap != 1)
	{
		int xx = m_pFormItem->getFormPoint(m_selF)->left, 
			yy = m_pFormItem->getFormPoint(m_selF)->top;
		if (bSize)
		{
			xx = m_pFormItem->getFormPoint(m_selF)->right;
			yy = m_pFormItem->getFormPoint(m_selF)->bottom;
		}
		if (mx != 0)
		{
			if (mx > 0)
				mx += m_nGap - (xx + mx) % m_nGap;
			else
				mx += -(xx + mx) % m_nGap;
			if (mx == 0)
				mx = -m_nGap;
		}
		if (my != 0)
		{
			if (my > 0)
				my += m_nGap - (yy + my) % m_nGap;
			else
				my += -(yy + my) % m_nGap;
			if (my == 0)
				my = -m_nGap;
		}
	}
	CRect pRc = CRect(0, 0, 0, 0);
	if (m_block->count == 0 || m_block->count == 1)
	{
		struct _formR *formR = m_pFormItem->getFormPoint(m_selF);
		if (formR->group)
		{
			pRc = GroupRect(formR->group);
			setBlockGroup(m_selF, mx, my, bSize);
			pRc.InflateRect(abs(mx), abs(my));
		}
		else
		{
			pRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
			if (!bSize)
			{
				formR->left += mx;
				formR->top += my;
				if (formR->left < 0)
				{
					mx -= formR->left;
					formR->left = 0;
				}
				if (formR->top < 0)
				{
					my -= formR->top;
					formR->top = 0;
				}
				if (formR->kind == FM_TAB)
					setBlockTab(formR, mx, my);
			}
			formR->right += mx;
			formR->bottom += my;
			if (formR->left > formR->right)
				formR->right = formR->left;
			if (formR->top > formR->bottom)
				formR->bottom = formR->top;
			pRc.InflateRect(abs(mx), abs(my));
		}
	}
	else
	{
		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			if (m_pFormItem->getFormPoint(ii)->block)
				pRc.UnionRect(pRc, getFormRect(ii));
		
		if (pRc.left + mx < 0)
			mx -= (pRc.left + mx);
		if (pRc.top  + my < 0)
			my -= (pRc.top + my);

		pRc.SetRectEmpty();
		
		for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
		{
			struct _formR *formR = m_pFormItem->getFormPoint(ii);
			if (formR->block)
			{
				if (formR->group)
				{
					pRc.UnionRect(pRc, GroupRect(formR->group));
					setBlockGroup(ii, mx, my, bSize);
				}
				else
				{
					if (!bSize && formR->kind == FM_TAB)
						setBlockTab(formR, mx, my);
					
					pRc.UnionRect(pRc, CRect(formR->left, formR->top, formR->right, formR->bottom));
					if (!bSize)
					{
						formR->left += mx;
						formR->top += my;
					}
					formR->right += mx;
					formR->bottom += my;
					if (formR->left > formR->right)
						formR->right = formR->left;
					if (formR->top > formR->bottom)
						formR->bottom = formR->top;

				}
			}
		}
		pRc.InflateRect(abs(mx), abs(my));
	}
	if (!m_block->count)
	{// ��ġ ����
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
	}
	return pRc;
}

CRect CChildView::SizeForm(int mx, int my)
{
	return MoveForm(mx, my, true);
}

void CChildView::SetBlockForm(int value, int type)
{
	for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
	{
		if (m_pFormItem->getFormPoint(ii)->block)
		{
			int dValue = 0;
			struct _formR *formR = m_pFormItem->getFormPoint(ii);
			switch(type)
			{
			case 0:	// left
				dValue = value - formR->left;
				if (formR->group)
					dValue = value - GroupRect(formR->group).left;
				MoveForm(ii, dValue, 0);
				break;
			case 1: // top
				dValue = value - formR->top;
				if (formR->group)
					dValue = value - GroupRect(formR->group).top;
				MoveForm(ii, 0, dValue);
				break;
			case 2: // right
				dValue = value - formR->right;
				if (formR->group)
					dValue = value - GroupRect(formR->group).right;
				MoveForm(ii, dValue, 0);
				break;
			case 3: // bottom
				dValue = value - formR->bottom;
				if (formR->group)
					dValue = value - GroupRect(formR->group).bottom;
				MoveForm(ii, 0, dValue);
				break;
			case 4: // VCENTER
				dValue = value - (formR->right - formR->left) / 2 - formR->left;
				if (formR->group)
					dValue = value - (GroupRect(formR->group).right - GroupRect(formR->group).left) / 2 - GroupRect(formR->group).left;
				MoveForm(ii, dValue, 0);
				break;
			case 5: // HCENTER
				dValue = value - (formR->bottom - formR->top) / 2 - formR->top;
				if (formR->group)
					dValue = value - (GroupRect(formR->group).bottom - GroupRect(formR->group).top) / 2 - GroupRect(formR->group).top;
				MoveForm(ii, 0, dValue);
				break;
			}
		}
	}
}

void CChildView::MoveForm(int idx, int mx, int my, bool bSize)
{
	struct _formR *formR = m_pFormItem->getFormPoint(idx);
	if (formR->group)
		setBlockGroup(idx, mx, my, bSize);
	else
	{
		if (!bSize && formR->kind == FM_TAB)
			setBlockTab(formR, mx, my);
		
		if (!bSize)
		{
			formR->left += mx;
			formR->top += my;
		}
		formR->right += mx;
		formR->bottom += my;
		if (formR->left > formR->right)
			formR->right = formR->left;
		if (formR->top > formR->bottom)
			formR->bottom = formR->top;
	}
	if (!m_block->count)
	{// ��ġ ����
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
	}
}

void CChildView::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case ID_DRAGTMR:
		KillTimer(nIDEvent);
		m_tool->MouseDrag();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}


void CChildView::addPage(int nPage)
{
	struct _formR *formR = m_pFormItem->getFormPoint(m_selF);
	if (nPage >= 0)
	{
		CString temp = m_pFormItem->getFormStr(m_selF, "dat");
		CString temp2 = m_pFormItem->getFormStr(m_selF, "str");
		
		CString tmp;
		tmp.Format("Tab%d\t", formR->keys);
		
		temp += tmp;

		tmp.Format("1:%d\t", formR->keys);
		temp2 += tmp;

		formR->keys++;
		
		m_pFormItem->setFormStr(m_selF, "dat", temp);
		if (formR->kind == FM_UTAB)
			m_pFormItem->setFormStr(m_selF, "str", temp2);
		
		if (nPage >= (int)formR->keys)
			formR->checked = 0;
		formR->checked = nPage;
	}
	Invalidate();
}

void CChildView::addCol()
{
	struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	struct _cellR *cellR;
	if (m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID)
	{
		CString temp;
		cellR = cellItem->getCellPoint(cellItem->addCell());
		cellR->pRGB = getDefaultValue("GRID", "DPRGB", basINPUT);
		cellR->tRGB = getDefaultValue("GRID", "DTRGB", 232);
		if (repR->cols)
		{
			cellR->hpRGB = getDefaultValue("GRID", "HPRGB", cellItem->getCellPoint(repR->cols - 1)->hpRGB);
			cellR->htRGB = getDefaultValue("GRID", "HTRGB", cellItem->getCellPoint(repR->cols - 1)->htRGB);
			cellR->width = getDefaultValue("GRID", "COLWIDTH", cellItem->getCellPoint(repR->cols - 1)->width);
		}
		else
		{
			cellR->hpRGB = getDefaultValue("GRID", "HPRGB", basPRGB);
			cellR->htRGB = getDefaultValue("GRID", "HTRGB", 232);
			cellR->width = getDefaultValue("GRID", "COLWIDTH", 80);
		}
		cellR->size = 0;
		//cellR->headN = 1;
		cellR->type = IO_NUM;
		cellR->iok = EIO_OUTPUT;
		cellR->kind = CK_NORMAL;
		strcpy_s(cellR->name, getItemSymbol("Col"));
		if (repR->cols == 0)
			cellR->width = getDefaultValue("GRID", "COLWIDTH", 80);
		else
			cellR->width = getDefaultValue("GRID", "COLWIDTH", cellItem->getCellPoint(0)->width);
		cellR->alignC = AL_RIGHT;
		cellR->properties |= PR_VISIBLE;
		
		repR->cols++;
	}
	else
	{
		CString temp;

		cellR = cellItem->getCellPoint(cellItem->addCell());
		cellR->pRGB = getDefaultValue("TABLE", "DPRGB", basINPUT);
		cellR->tRGB = getDefaultValue("TABLE", "DTRGB", 232);
		if (repR->rows)
		{
			cellR->hpRGB = getDefaultValue("TABLE", "HPRGB", cellItem->getCellPoint(repR->rows - 1)->hpRGB);
			cellR->htRGB = getDefaultValue("TABLE", "HTRGB", cellItem->getCellPoint(repR->rows - 1)->htRGB);
			cellR->width = getDefaultValue("TABLE", "ROWWIDTH", cellItem->getCellPoint(repR->rows - 1)->width);
		}
		else
		{
			cellR->hpRGB = getDefaultValue("TABLE", "HPRGB", basPRGB);
			cellR->htRGB = getDefaultValue("TABLE", "HTRGB", 232);
			cellR->width = getDefaultValue("TABLE", "ROWWIDTH", 80);
		}
		cellR->size = 0;
		//cellR->headN = 1;
		cellR->type = IO_NUM;
		strcpy_s(cellR->name, getItemSymbol("Row"));
		if (repR->rows == 0)
			cellR->width = getDefaultValue("TABLE", "ROWWIDTH", 80);
		else
			cellR->width = getDefaultValue("TABLE", "ROWWIDTH", cellItem->getCellPoint(0)->width);
		cellR->alignC = AL_RIGHT;
		cellR->properties |= PR_VISIBLE;
		
		repR->rows++;
	}
	delete cellItem;

	Invalidate();
}

void CChildView::OnSelectColumn()
{
	m_bSelect = true;
	SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKEWPARAM(m_RBDPnt.x, m_RBDPnt.y));
	SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKEWPARAM(m_RBDPnt.x, m_RBDPnt.y));
}

void CChildView::OnColumnFit()
{
	if (m_selF < 0)
		return;
	if (m_pFormItem->getFormPoint(m_selF)->kind == FM_GRID)
	{
		struct _repR *repR   = (struct _repR *)m_pFormItem->getFormPoint(m_selF)->auxR;
		CCellItem *cellItem = new CCellItem(repR);
		
		int nWidth = 0, nFormWidth = m_pFormItem->getFormPoint(m_selF)->right - m_pFormItem->getFormPoint(m_selF)->left - 4,
			nVisibleCnt = 0;
		if (m_pFormItem->getFormPoint(m_selF)->attr2 & GO_VSCR)
			nFormWidth -= szSCROLL;
		
		if (repR->selected >= 0)
		{
			for (int ii = 0; ii < cellItem->getCellCount(); ii++)
			{
				if (cellItem->getCellPoint(ii)->properties & PR_VISIBLE)
				{
					nWidth += cellItem->getCellPoint(ii)->width;
					nVisibleCnt++;
				}
			}
			nWidth -= cellItem->getCellPoint(repR->selected)->width;
			cellItem->getCellPoint(repR->selected)->width = nFormWidth - nWidth;
			if (nFormWidth - nWidth < 0)
			{
				for (int ii = 0; ii < cellItem->getCellCount(); ii++)
				{
					nWidth = (nFormWidth * (ii + 1)) / nVisibleCnt - (nFormWidth * ii) / nVisibleCnt;
					cellItem->getCellPoint(ii)->width = nWidth;
				}
			}
		}
		else
		{
			for (int ii = 0; ii < cellItem->getCellCount(); ii++)
			{
				if (cellItem->getCellPoint(ii)->properties & PR_VISIBLE)
					nVisibleCnt++;
			}
			for (int ii = 0; ii < cellItem->getCellCount(); ii++)
			{
				if (cellItem->getCellPoint(ii)->properties & PR_VISIBLE)
				{
					nWidth = (nFormWidth * (ii + 1)) / nVisibleCnt - (nFormWidth * ii) / nVisibleCnt;
					cellItem->getCellPoint(ii)->width = nWidth;
				}
			}
		}
		
		delete cellItem;
		InvalidateRect2(m_selF);
	}
	else if (m_pFormItem->getFormPoint(m_selF)->kind == FM_BUTTON)
	{
		if ((m_pFormItem->getFormPoint(m_selF)->properties & PR_IMAGE) ||
			(m_pFormItem->getFormPoint(m_selF)->properties & PR_IMAGETEXT))
			
		{
			if (!m_pFormItem->getFormStr(m_selF, "str2").IsEmpty())
			{
				CString	path = m_pFormItem->getFormStr(m_selF, "str2");
				if (path.Find("\\") == -1)
					path = m_root + "\\" + IMAGEDIR + "\\" + path;
				
				Cpicture pic(path);
				CSize sz = pic.getImageSize();

				if (sz != CSize(0, 0))
				{
					m_pFormItem->getFormPoint(m_selF)->right = m_pFormItem->getFormPoint(m_selF)->left + sz.cx + 1;
					m_pFormItem->getFormPoint(m_selF)->bottom = m_pFormItem->getFormPoint(m_selF)->top + sz.cy + 1;
					
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
					Invalidate();
				}
			}
		}
	}
}

void CChildView::OnExtMenu()
{
	if (m_selF < 0)
		return;

	switch(m_pFormItem->getFormPoint(m_selF)->kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_OUT:
		{
			CAttrDlg attrDlg(this, m_pFormItem->getFormPoint(m_selF)->kind, m_pFormItem->getFormPoint(m_selF)->type, m_pFormItem->getFormPoint(m_selF)->iok, m_pFormItem->getFormPoint(m_selF)->attr, m_pFormItem->getFormPoint(m_selF)->attr2);
			if (attrDlg.DoModal() == IDOK)
			{
				m_pFormItem->getFormPoint(m_selF)->attr  = attrDlg.m_attr;
				m_pFormItem->getFormPoint(m_selF)->attr2 = attrDlg.m_attr2;
			}
		}
		break;
	case FM_CONTROL:
		{
			CControlDlg Dlg;
			
			Dlg.m_ctrlName = m_pFormItem->getFormStr(m_selF, "str");
			Dlg.m_ctrlParam = m_pFormItem->getFormStr(m_selF, "str2");
			if (Dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				m_pFormItem->setFormStr(m_selF, "str", Dlg.m_ctrlName);
				m_pFormItem->setFormStr(m_selF, "str2", Dlg.m_ctrlParam);
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, -1);
				AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);	// control event refresh
			}
		}
		break;

	case FM_TAB:
		{
			if (m_selF >= 0 &&
				(m_pFormItem->getFormPoint(m_selF)->kind == FM_TAB ||
				m_pFormItem->getFormPoint(m_selF)->kind == FM_UTAB) &&
				m_pFormItem->getFormPoint(m_selF)->keys > 0)
			{
				deleteSelPage(m_selF);
			}
		}
		break;
	case FM_UTAB:
		{
			CUTabPage	Dlg(this, m_mapH, m_selF);
			
			if (Dlg.DoModal() == IDOK)
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, -1);
		}
		break;
	case FM_OBJECT:
		{
			m_rtnStr = m_pFormItem->getFormStr(m_selF, "str");
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, (long)m_rtnStr.operator LPCTSTR());
		}
		break;
	}
	Invalidate();
}

void CChildView::LoadAutoList()
{
	int nScriptID = 100;
	CMapStringToPtr mapFormID;
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
	char	twb[128];
	path.Format("%s\\%s\\public.ini", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0), TABDIR);

	for (int ii = 1; ii < 100; ii++)
	{
		void *rtValue;
		int FormNum;
		CString tmps;

		tmps.Format("%02d", ii);
		GetPrivateProfileString(sLevel, tmps, "", twb, sizeof(twb), path);
		tmps = twb;
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
			GetPrivateProfileString(tmps, tmpstr, "", twb, sizeof(twb), path);
			tmpstr = twb;
			if (tmpstr.IsEmpty())
				break;
			
			info += tmpstr;
			info += '\t';
		}

		if (!info.IsEmpty())
		{
			m_pSCEdit->m_AutoListMap.SetAt(FormNum, info);
			if (!sLevel.CompareNoCase("controls") && FormNum >= 100)
				m_pSCEdit->m_ScriptStrMap.SetAt(tmps, (void*)FormNum);
			tmps.MakeUpper();
			if (sLevel.CompareNoCase("controls"))
				m_pSCEdit->m_publicStrMap.SetAt(tmps, (void*)FormNum);
		}
	}

	mapFormID.RemoveAll();
}

void CChildView::changeKind()
{
	if (m_mapH && m_mapH->mapK == MK_PROCEDURES)
	{
		m_pSCEdit = new CVBScriptEdit();
		m_pSCEdit->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 100);

		LoadAutoList();
		m_pSCEdit->Initialize();
		m_pSCEdit->Initialize(m_mapH);
		m_pSCEdit->SetPythonMode(m_mapH->pythonMode);

		/*CMapPtrToPtr *pUndoMap = new CMapPtrToPtr;
		m_mapH->pUndoMap = pUndoMap;

		pUndoMap->SetAt((void*)0, m_pSCEdit->SetUndoArray(NULL));		
		pUndoMap->SetAt((void*)1, m_pSCEdit->SetRedoArray(NULL));*/

		//((CMDIChildWnd*)GetParent())->MDIActivate();	? ���� ���� �ּ�ó��(SDI�϶� ����)
	}
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg) 
{
	switch(pMsg->message)
	{
	case WM_KEYUP:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_KEYDOWN:
		if (m_pSCEdit)
		{
			CPoint pos = m_pSCEdit->GetCursorPos();
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STATUSLINEINFO, MAKELPARAM(pos.y, pos.x));
		}
		break;
	}	
	return CWnd::PreTranslateMessage(pMsg);
}

long CChildView::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case 0x01:
		if (lParam & 0x02000000)
			return (COLORREF) lParam;
		
		return m_palette->GetPaletteRGB(lParam);
		break;
	}
	return 0;
}

void CChildView::RedrawView()
{
	m_bRedraw = true;
	RedrawWindow();
}

void CChildView::OnDestroy() 
{
	CWnd* pWizardCtrl = (CWnd*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (!pWizardCtrl)
		return;
	if (m_pPreView)
	{
		pWizardCtrl->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
					(BYTE *)(VTS_I4), m_Previewkey);
		if (m_pPreView)
			m_pPreView->PostMessage(WM_CLOSE);

		m_pPreView = NULL;
	}
	CWnd::OnDestroy();	
}

void CChildView::setLine(int nLine)
{
	if (m_mapH->mapK != MK_PROCEDURES)
		return;
	
	for (int vLine = 0; vLine <= nLine; vLine++)
	{
		CString txt = m_pSCEdit->GetLineString(vLine);
		txt.Remove('\r');
		txt.Remove('\t');
		txt.Remove(' ');
		if (txt.IsEmpty())
			nLine++;
		if (nLine >= m_pSCEdit->GetLineCnt())
			break;
	}
}


bool CChildView::isValidIOOrder(int idx)
{
	int kind = m_pFormItem->getFormPoint(idx)->kind;

	switch(kind)
	{
	case FM_EDIT:
	case FM_OUT:
	case FM_COMBO:
	case FM_GRID:
	case FM_TABLE:
	case FM_OBJECT:
	case FM_MEMO:
	case FM_TREE:
	case FM_SHEET:
		return true;
	case FM_BROWSER:
		if (m_pFormItem->getFormPoint(idx)->iok == EIO_OUTPUT)
			return true;
		break;
	case FM_BUTTON:
		if ((m_pFormItem->getFormPoint(idx)->type == BTN_RADIO || m_pFormItem->getFormPoint(idx)->type == BTN_CHECK)
			&& m_pFormItem->getFormPoint(idx)->iok != EIO_NOP)
			return true;
		break;
	case FM_CONTROL:
		if (m_pFormItem->getFormPoint(idx)->iok != EIO_NOP)
			return true;
	}
	return false;
}

bool CChildView::isValidTabOrder(int idx)
{
	int kind = m_pFormItem->getFormPoint(idx)->kind;
	switch(kind)
	{
	case FM_EDIT:
	case FM_BUTTON:
	case FM_COMBO:
	case FM_OBJECT:
	case FM_GRID:
	case FM_MEMO:
		return true;
	case FM_CONTROL:
		if (m_pFormItem->getFormPoint(idx)->iok == EIO_INPUT || 
			m_pFormItem->getFormPoint(idx)->iok == EIO_INOUT)
			return true;
		break;
	}
	return false;
}

int CChildView::getDefaultValue(CString sSection, CString sKey, int nDefault)
{
	CString sTabDir;
	sTabDir.Format("%s\\tab\\", (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR));
	return GetPrivateProfileInt(sSection, sKey, nDefault, sTabDir + "awDefault.ini");
}

void CChildView::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_mapH->mapK == MK_PROCEDURES)
		m_pSCEdit->SetFocus();
	else
		CWnd::OnSetFocus(pOldWnd);	
}
