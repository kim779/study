
// axisBuilderView.cpp : CChildView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "axisMBuilder.h"
#endif

#include "resource.h"
#include "ChildView.h"
#include "ChildFrm.h"
#include "VBScriptEdit.h"
#include "emulateWnd.h"
#include "symbol.h"

#include "h/mainvar.h"
#include "amLib/amlib.h"
#include "amCtrl/tlBase.h"
#include "amCtrl/tlBox.h"
#include "amCtrl/tlButton.h"
#include "amCtrl/tlGrid.h"
#include "amCtrl/tlInOut.h"
#include "amCtrl/tlPanel.h"
#include "amCtrl/tlRegion.h"
#include "amCtrl/tlTab.h"
#include "amCtrl/tlText.h"
#include "amCtrl/tlPointer.h" 
#include "amCtrl/tlImage.h" 
#include "amCtrl/tracker.h"

#include "amCom/formItem.h"
#include "amCom/cellItem.h"

#include "amDlg/attrDlg.h"
#include "amDlg/gridColumn.h"
#include "amDlg/tabPage.h"
#include "EmulCtrl.h"

#include "awObject/ShowObject.h"
#include "LayoutEditDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	CURSORNWSE	1
#define CURSORNESW	2
#define	CURSORNS	3
#define	CURSORWE	4

#define	TM_DRAG		100

#define LAYOUTMENU	1000
#define LAYOUTCOUNT	20

// CChildView

IMPLEMENT_DYNCREATE(CChildView, CWnd)

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	// 표준 인쇄 명령입니다.
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER, &CChildView::OnMessage)
	ON_COMMAND_RANGE(ID_AUX_TOFIRST, ID_AUX_EXTID, &CChildView::OnViewCommand)
	ON_COMMAND_RANGE(ID_LAYOUT_ORIGINAL, ID_MASTERLAYOUT, &CChildView::OnLayoutCommand)
	ON_COMMAND_RANGE(LAYOUTMENU, LAYOUTMENU + LAYOUTCOUNT, &CChildView::OnLayoutModeCommand)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CChildView 생성/소멸

CChildView::CChildView()
{
	m_toolOBs.RemoveAll();
	m_selF      = -1;
	m_stdF      = -1;
	m_move      = false;
	m_drawing   = false;
	m_selected  = false;
	m_bLBtnDown = false;
	m_bLine	    = false;
	m_pBitmap   = NULL;
	m_nTabOdr   = m_nSeqTabOdr = 0;
	m_nIoOdr    = m_nSeqIoOdr = 0;
	m_bRedraw   = true;
	m_formItem  = NULL;
	m_bEmulRun  = false;
	m_bLayoutMode = false;

	m_mapH      = (struct _mapH   *) NULL;
	m_block     = (struct _block  *) NULL;
	m_pSCEdit   = NULL;
	m_pEmulateWnd  = NULL;

	m_nGap = (int)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGAP, 0);
	m_forms.RemoveAll();

	m_sImgPath = "";
	m_hImgBack = NULL;

	m_sPreLayoutName = _T("Original");
	m_param = _T("");
}

CChildView::~CChildView()
{
	for (int ii = 0; ii < m_toolOBs.GetSize(); ii++)
	{
		CTlBase* tool = (CTlBase *)m_toolOBs.GetAt(ii);
		ASSERT_VALID(tool);
		delete tool;
	}
	m_toolOBs.RemoveAll();

	if (m_pSCEdit)
		delete m_pSCEdit;

	if (m_formItem)
		delete m_formItem;

/*	int	key;
	CShowObject*	form;
	for (POSITION pos = m_forms.GetStartPosition(); pos; )
	{
		m_forms.GetNextAssoc(pos, key, form);
		delete form;
	}
	m_forms.RemoveAll();
*/
	if (m_pBitmap)
		delete m_pBitmap;
	if (m_hImgBack)
		DeleteObject(m_hImgBack);
}

// CChildView 진단

#ifdef _DEBUG
void CChildView::AssertValid() const
{
	CWnd::AssertValid();
}

void CChildView::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}

#endif //_DEBUG


// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
			::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_BTNFACE+1), NULL);

	return CWnd::PreCreateWindow(cs);
}

void CChildView::OnDraw(CDC* /*pDC*/)
{
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
	if (m_bEmulRun)
		return FALSE;

	switch (pMsg->message)
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

void CChildView::OnDestroy()
{
	CWnd*	pWizard = (CWnd*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (!pWizard)
		return;

	CWnd::OnDestroy();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
//	return CWnd::OnEraseBkgnd(pDC);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_mapH->mapK == MK_PROCEDURES)
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
  	if (!m_tool->m_name.CompareNoCase(_T("Pointer")))
  	{
		int	oldF;
		switch (nChar)
		{
		case VK_TAB:
			if (m_formItem->GetCount() > 0)
			{
				oldF = m_selF;
				for (;;)
				{
					m_selF = (++m_selF) % m_formItem->GetCount();
					if (m_formItem->GetForm(m_selF)->group)
						m_selF = m_formItem->GetIdxOfPtr(m_formItem->GetForm(m_selF)->group);
					if (!m_formItem->GetFormStr(m_selF, _T("name")).IsEmpty())
						break;
				}
				redrawSelChange(oldF, m_selF);
				AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
			}
			break;

		case VK_DELETE:
			if ((::GetKeyState(VK_SHIFT) & 0x8000) && m_selF >= 0 && m_formItem->GetForm(m_selF)->kind == FM_TAB
				&& m_formItem->GetForm(m_selF)->keys > 0)
			{
				deleteSelPage(m_selF);
				break;
			}

			if (m_selF >= 0 && (m_formItem->GetForm(m_selF)->kind == FM_TABLE || m_formItem->GetForm(m_selF)->kind == FM_GRIDEX || m_formItem->GetForm(m_selF)->kind == FM_GRID))
			{
				if (m_formItem->GetForm(m_selF)->auxR != NULL
					&& ((struct _repR *)m_formItem->GetForm(m_selF)->auxR)->selected >= 0)
				{
					struct _repR *repR = (struct _repR*)m_formItem->GetForm(m_selF)->auxR;
					CCellItem cellItem(repR);
					cellItem.RemoveCell(repR->selected);
					if (m_formItem->GetForm(m_selF)->kind == FM_GRIDEX || m_formItem->GetForm(m_selF)->kind == FM_GRID)
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
			this->DeleteFORMs();
			break;

		case VK_F8:
			AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETFOCUSPRO, 0);
			break;

		case VK_F9:
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
			break;
		}
	}
	
//	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES || m_bEmulRun)
		return;

	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_SCRIPT, 0);
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES || m_bEmulRun)
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
	if (!(nFlags & MK_CONTROL) && m_tool->m_name.CompareNoCase(_T("Pointer")))
	{
		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}
	m_bLBtnDown = true;
//	CWnd::OnLButtonDown(nFlags, point);

	if (!m_drawing)
	{
		CRect	tRc;
		int	fN = m_formItem->GetCount();
		if (m_selF >= 0
			&& (m_formItem->GetForm(m_selF)->kind == FM_GRID || m_formItem->GetForm(m_selF)->kind == FM_GRIDEX || m_formItem->GetForm(m_selF)->kind == FM_TABLE)
			&& m_formItem->GetForm(m_selF)->auxR)
		{
			((struct _repR *)m_formItem->GetForm(m_selF)->auxR)->selected = -1;
		}
		
		if (m_tool->m_name.CompareNoCase(_T("Pointer")))
		{
			int	pageN = 0;

			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
			m_formItem->AddForm();
			struct _formR *formR = m_formItem->GetForm(fN);
			if (formR == NULL)
				return;

			if (!m_tool->m_name.CompareNoCase(_T("Control")) && !m_param.IsEmpty())
			{
				m_formItem->SetFormStr(fN, _T("str"), m_param);
				m_param = _T("");
			}
			
			if (!m_tool->m_name.CompareNoCase(_T("Table")))
			{
				formR->pRGB = 232;
				formR->bRGB = 232;
			}
			else
				formR->pRGB = basPRGB;
			
			formR->tRGB = 232;
			formR->fPoint  = m_mapH->fPoint;
			m_formItem->SetFormStr(fN, _T("fName"), m_mapH->fName);

			if (!m_tool->m_name.CompareNoCase(_T("Grid")) || !m_tool->m_name.CompareNoCase(_T("GridEX")))
				formR->keys = 20;
						
			formR->size = 0;
			formR->attr = 0;
			formR->properties = 0;
			formR->properties |= PR_VISIBLE;
			formR->properties |= PR_ENABLE;
			if (m_tool->m_name.CompareNoCase(_T("Grid")) && m_tool->m_name.CompareNoCase(_T("GridEX")) && m_tool->m_name.CompareNoCase(_T("Table")))
				formR->properties |= PR_CHECKED;
			formR->checked = 0;

			formR->onTap    = -1;
			formR->onTapL   = 0;
			formR->onLongTap  = -1;
			formR->onLongTapL = 0;
			formR->onChange   = -1;
			formR->onChangeL  = 0;
			formR->onChar     = -1;
			formR->onCharL    = 0;
			formR->onDrag	  = -1;
			formR->onDragL    = 0;
			formR->onScroll	  = -1;
			formR->onScrollL  = 0;
			
			if (!m_tool->m_name.CompareNoCase(_T("Edit")) || !m_tool->m_name.CompareNoCase(_T("ComboBox"))
				|| !m_tool->m_name.CompareNoCase(_T("PushButton")) || !m_tool->m_name.CompareNoCase(_T("Control"))
				|| !m_tool->m_name.CompareNoCase(_T("Grid"))	|| m_tool->m_name.CompareNoCase(_T("GridEX"))
				|| !m_tool->m_name.CompareNoCase(_T("Object")) || !m_tool->m_name.CompareNoCase(_T("Memo")))
			{
				SortTabOrder();
				formR->index = ++m_nSeqTabOdr;
			}
			if (!m_tool->m_name.CompareNoCase(_T("Edit")) || !m_tool->m_name.CompareNoCase(_T("ComboBox"))
				|| !m_tool->m_name.CompareNoCase(_T("PushButton")) || !m_tool->m_name.CompareNoCase(_T("Browser"))
				|| !m_tool->m_name.CompareNoCase(_T("Grid"))	|| !m_tool->m_name.CompareNoCase(_T("GridEX"))
				|| !m_tool->m_name.CompareNoCase(_T("Object")) || !m_tool->m_name.CompareNoCase(_T("Memo"))
				|| !m_tool->m_name.CompareNoCase(_T("Table")) || !m_tool->m_name.CompareNoCase(_T("Output")))
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
					switch (m_formItem->GetForm(m_selF)->kind)
					{
					
					case FM_GRID:
						rVal = ID_TOOL_GRID - ID_TOOL_BASE;	break;
					case FM_GRIDEX:
						rVal = ID_TOOL_GRIDEX - ID_TOOL_BASE;	break;
					case FM_TABLE:
						rVal = ID_TOOL_TABLE - ID_TOOL_BASE;	break;
					case FM_TAB:
						rVal = ID_TOOL_TAB - ID_TOOL_BASE;	break;
					default:
						return;
					}
					if (m_bSelect)
						SetTimer(TM_DRAG, 500, NULL);

					m_tool = (CTlBase *)m_toolOBs.GetAt(rVal);
				}
				else
					m_tool = (CTlBase *)m_toolOBs.GetAt(0);
			}
			else if (m_selF >= 0 
				&& (m_formItem->GetForm(m_selF)->kind == FM_GRID || m_formItem->GetForm(m_selF)->kind == FM_GRIDEX || m_formItem->GetForm(m_selF)->kind == FM_TABLE))
			{
				if (m_formItem->GetForm(m_selF)->auxR == NULL)
				{
					struct _repR *repR;

					m_formItem->GetForm(m_selF)->auxR = new char[sizeof(_repR)];
					repR = (struct _repR *)m_formItem->GetForm(m_formItem->GetCount() - 1)->auxR;
					ZeroMemory(repR, sizeof(struct _repR));
					if (m_formItem->GetForm(m_selF)->kind == FM_GRIDEX || m_formItem->GetForm(m_selF)->kind == FM_GRID)
					{
						repR->rows = 10;
						m_formItem->GetForm(m_selF)->keys = MAKELONG(10, HIWORD(m_formItem->GetForm(m_selF)->keys));
					}
				}
				((struct _repR *)m_formItem->GetForm(m_selF)->auxR)->selected = -1;
			}
		}
		m_drawing = true;
		m_tool->MouseDown(point);
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES || m_bEmulRun)
		return;

	if (m_bLine)
	{
		CArray<CRect, CRect> *pLine = (CArray<CRect, CRect> *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STANDARDLINE);
		CRect rcline;
		for (int ll = 0; ll < pLine->GetCount(); ll++)
		{
			rcline = pLine->GetAt(ll);
			rcline.InflateRect(4, 4);
			if (rcline.PtInRect(point))
			{
				pLine->RemoveAt(ll);
				Invalidate();
				break;
			}
		}
	}

	if (m_bSelect)
		KillTimer(TM_DRAG);

	if (m_nTabOdr || m_nIoOdr)
	{
		m_tool->MouseUp();

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, ID_TOOL_POINTER);
		m_tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_POINTER - ID_TOOL_BASE);
		m_selected = false;
		if (m_selF != -1)
		{
			if (m_nTabOdr)
				SortTabOrder();
			else
				SortIoOrder();
		}
		AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
		return;
	}

	CRect	tRc;
	struct	_repR*	repR;
	int	msgId = ID_USR_REFRESHCTRL;

	if (!(nFlags & MK_CONTROL) && m_tool->m_name.CompareNoCase(_T("Pointer")))
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

		if (!(m_move || m_bSelect) && (!m_tool->m_name.CompareNoCase(_T("Grid")) || !m_tool->m_name.CompareNoCase(_T("GridEX")) || !m_tool->m_name.CompareNoCase(_T("Table"))))
		{
			int	formN = m_formItem->GetCount() - 1;
			m_formItem->GetForm(formN)->auxR = new char[sizeof(_repR)];
			repR = (struct _repR *)m_formItem->GetForm(formN)->auxR;
			CCellItem* cellItem = new CCellItem(repR);

			ZeroMemory(repR, sizeof(struct _repR));
			repR->selected = -1;
			if (m_formItem->GetForm(formN)->kind == FM_TABLE)
			{
				m_formItem->GetForm(formN)->keys = 20;
				m_formItem->GetForm(formN)->attr2 |= GO_MULTI;
			}
			else
			{
				if (m_mapH->options & OP_LEDGER)
					m_formItem->GetForm(formN)->attr2 |= GO_HEADER;
				repR->rows = 10;
			}

			for (int jj = 0; jj < cellItem->GetCount(); jj++) // cell 초기화
			{
				struct _cellR	*cellR;

				cellR = cellItem->GetCell(jj);
				cellR->type   = IO_NUM;
				cellR->width  = 80;
				cellR->hpRGB  = basPRGB;
				cellR->pRGB   = basINPUT;
				cellR->alignC = AL_RIGHT;
				cellR->properties |= PR_VISIBLE;
			}
			delete cellItem;
		}
		
		//	selection 처리
		if (m_selF >= 0)
		{
			tRc = getFormRect(m_selF);
			if (m_formItem->GetForm(m_selF)->group != 0)
				m_selF = m_formItem->GetIdxOfPtr(m_formItem->GetForm(m_selF)->group);
		}
		
		// new form
		if (m_tool->m_name.CompareNoCase(_T("Pointer")) && !(m_move || m_bSelect))
		{
			m_selF = m_formItem->GetCount() - 1;
			m_formItem->SetFormStr(m_selF, _T("name"), GetInitSymbol());

			tRc = getFormRect(m_selF);
			tRc.InflateRect(5, 5);
			// layout setting
			m_tool->DefaultLayout();
			InvalidateRect(tRc);
		}
		else
			msgId = ID_USR_SELCTRLTOPRO;

		if (!m_tool->m_name.CompareNoCase(_T("Object")))
			setObject(m_formItem->GetCount()-1);

		m_move    = false;
		m_drawing = false;
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, ID_TOOL_POINTER);
	
	m_tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_POINTER - ID_TOOL_BASE);
	m_selected = false;
	m_bLBtnDown = false;
	SortIoOrder();
	SortTabOrder();
	AfxGetMainWnd()->SendMessage(WM_USER, msgId, (msgId == ID_USR_REFRESHCTRL ? TRUE : FALSE));

//	CWnd::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES || m_bEmulRun)
		return;

	if ((nFlags & MK_LBUTTON) && abs((m_dragPnt.x - point.x) * (m_dragPnt.y - point.y)) > 4)
		m_tool->MouseDrag();

	if (m_nTabOdr || m_nIoOdr)
		return;

	m_nCursor = 0;
	if (m_drawing)
	{
		CRect rc, tRc;
		if (!(nFlags & MK_CONTROL) && m_tool->m_name.CompareNoCase(_T("Pointer")))
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
		if (m_selF >= 0 && !m_tool->m_name.CompareNoCase(_T("Pointer")) && (m_block->count <= 1) && !m_formItem->GetForm(m_selF)->group)
		{
			CRect	tRc = getFormRect(m_selF);
			
			CTracker	tracker(tRc, CTracker::resizeOutside);
			int	hit = tracker.HitTest(point);
			hit = CTracker::TrackerHit(hit);
			switch (hit)
			{
			case CTracker::hitTopLeft:
			case CTracker::hitBottomRight:
				m_nCursor = CURSORNWSE;
				break;
			case CTracker::hitTopRight:
			case CTracker::hitBottomLeft:
				m_nCursor = CURSORNESW;
				break;
			case CTracker::hitTop:
			case CTracker::hitBottom:
				m_nCursor = CURSORNS;
				break;
			case CTracker::hitRight:
			case CTracker::hitLeft:
				m_nCursor = CURSORWE;
				break;
			case CTracker::hitMiddle:
				if (movable(point))
				{
					m_move = true;
					m_nCursor = CURSORWE;
					break;
				}
			case CTracker::hitNothing:
			default:
				break;
			}
		}
  	}


	if (m_bLine)
	{
		CArray<CRect, CRect> *pLine = (CArray<CRect, CRect> *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STANDARDLINE);
		CRect rcline;
		for (int ll = 0; ll < pLine->GetCount(); ll++)
		{
			rcline = pLine->GetAt(ll);
			rcline.InflateRect(4, 4);
			if (rcline.PtInRect(point))
			{
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
				break;
			}
			else 
			//	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				::SetCursor(AfxGetApp()->LoadCursor(IDC_PEN));
		}
	}

		
//	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES || m_bEmulRun)
		return;

	CRect	tRc,  fRc, pRc;
	int	oldF  = m_selF;
	int	tForm = 0;

	m_selF = -1;
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		tRc = getRect(ii);
		if (tRc.PtInRect(point))
			m_selF = ii;
	}

	if (m_selF > m_formItem->GetCount() || m_selF < 0)
		m_selF = -1;
	else
	{
		for (;;)
		{
			if (m_selF == tForm)
				break;

			if (m_formItem->GetForm(m_selF)->kind == FM_GROUP || m_formItem->GetForm(m_selF)->kind == FM_BOX)
			{
				tForm = m_selF;
				pRc = getRect(tForm);
				for (int kk = 0; kk < tForm; kk++)
				{
					if (m_formItem->GetFormStr(kk, _T("name")).IsEmpty())
						continue;

					if (m_formItem->GetForm(kk)->group && m_formItem->GetIdxOfPtr(m_formItem->GetForm(kk)->group) != kk)
						continue;

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
		redrawSelChange(oldF, m_selF);

	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
	m_RBDPnt = point;
	
//	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bEmulRun)
		return;

	showMenu();
	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	CRect	rc;

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

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (m_selected)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		return TRUE;
	}

	switch (m_nCursor)
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

void CChildView::OnSetFocus(CWnd* pOldWnd)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		m_pSCEdit->SetFocus();
	else
		CWnd::OnSetFocus(pOldWnd);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
//	CWnd::OnSize(nType, cx, cy);	
	if (nType == SIZE_MINIMIZED || m_new || m_mapH == (struct _mapH *) NULL)
		return;

	CRect	cRc;

	GetClientRect(&cRc);

	m_mapH->width  = cRc.Width();
	m_mapH->height = cRc.Height();

	if (m_mapH->mapK == MK_PROCEDURES)
	{
		if (m_pSCEdit)
			m_pSCEdit->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER);
		CWnd::OnSize(nType, cx, cy);
		return;
	}

	if (m_pBitmap)
	{
		CDC*	pDC = GetDC();

		delete m_pBitmap;
		m_pBitmap = new CBitmap();
		m_pBitmap->CreateCompatibleBitmap(pDC, cRc.Width(), cRc.Height());

		ReleaseDC(pDC);
		Invalidate();
		m_mapH->modified = true;
	}

	Invalidate(FALSE);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
}

void CChildView::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case TM_DRAG:
		KillTimer(nIDEvent);
		m_tool->MouseDrag();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

LRESULT CChildView::OnMessage(WPARAM wp, LPARAM lp)
{
	CString	tmps;
	switch (LOWORD(wp))
	{
	case getCOLOR:
		return GetRGB((int)lp);
	case getROOT:
		return (long)getRoot();
	}

	return 0;
}

// Layout
void CChildView::OnLayoutModeCommand(UINT cmdID)
{
	// 이미 Layout Mode인지 체크
	//if (m_bLayoutMode)
	//	return;

	CString sData, sIndex;
	sIndex = m_arLayout.GetAt(cmdID - LAYOUTMENU);
	m_formItem->m_mapH->maplayout->Lookup(sIndex, sData);

	// 현재 자신인지 체크
	if (!sData.CompareNoCase(m_sPreLayoutName))
		return;

	// Layout 모드 진입
	m_bLayoutMode = true;
	m_formItem->SetCurrLayoutName(sData);
	// 해당 Layout 모드로 정보 세팅
	m_formItem->SetLayoutInfo(sData, m_sPreLayoutName);
	m_sPreLayoutName = sData;
		
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_LAYOUTMODE, 1);
	Invalidate();
}

// Layout
void CChildView::OnLayoutCommand(UINT cmdID)
{
	int EditSel = 0;
	switch(cmdID)
	{
	case ID_LAYOUT_ORIGINAL:	// 원래 편집창으로 이동
		if (!m_bLayoutMode)
			break;

		// 원래 Mode로 복귀
		m_bLayoutMode = false;
		m_formItem->SetCurrLayoutName("Original");
		m_formItem->SetLayoutInfo("Original", m_sPreLayoutName);
		m_sPreLayoutName = "Original";
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_LAYOUTMODE, 0);
		Invalidate();
		break;
	case ID_LAYOUT_EDITOR:
		{
			CLayoutEditDlg dlg;
			dlg.SetLayoutList(m_formItem->m_mapH->maplayout, &m_arLayout);
			dlg.SetCurLayOutName(m_formItem->GetCurrLayoutName());
			if (dlg.DoModal() == IDOK)
			{
				if (dlg.m_mapData.GetCount() < 1)
					break;
				m_formItem->AddLayout(&dlg.m_mapData);
				SetDefaultLayout();
			}
		}
		break;
	case ID_MASTERLAYOUT:
		{
			if (MessageBox("Save You Master Layout ?", "Mater Layout", MB_YESNO) == IDYES)
				AfxGetMainWnd()->SendMessage(WM_USER, ID_MASTERLAYOUT);

		}
		break;
	}
}

void CChildView::SetDefaultLayout()
{
	struct _formR* formR;
	CString sIndex, sData, ss;
	struct _Layout* playout = nullptr;

	CMapStringToString* map = m_formItem->m_mapH->maplayout;
	CMapStringToString mapdel;

	// Delete list 
	for (POSITION pos = map->GetStartPosition(); pos;)
	{
		map->GetNextAssoc(pos, sIndex, sData);
		mapdel.SetAt(sData, sIndex);
	}

	for (POSITION pos = map->GetStartPosition(); pos;)
	{	
		map->GetNextAssoc(pos, sIndex, sData);		
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);	
		
			// Delete layout...
			for (POSITION pos1 = formR->layout->GetStartPosition(); pos1;)
			{
				formR->layout->GetNextAssoc(pos1, ss, (void*&)playout);
				if (!mapdel.Lookup(ss, sIndex))
				{
					formR->layout->RemoveKey(ss);
					delete playout;
				}
			}
			
			if (formR->layout->Lookup(sData, (void*&)playout))
				continue;

			playout = new _Layout;	
			ZeroMemory(playout, sizeof(_Layout));	

			playout->left = formR->left;
			playout->right = formR->right;
			playout->top = formR->top;
			playout->bottom = formR->bottom;
			playout->bfirst = true;
			playout->properties = 0x00;
			if (formR->properties & PR_VISIBLE)
				playout->properties = formR->properties;

			formR->layout->SetAt(sData, playout);
		}
	}
}

bool CChildView::IsLayoutMode()
{
	if (m_formItem->GetCurrLayoutName().CompareNoCase("Original"))
		return false;

	return true;
}

void CChildView::OnViewCommand(UINT cmdID)
{
	switch (cmdID)
	{
	case ID_AUX_TOFIRST:
		sendToFirst();
		break;
	case ID_AUX_TOEND:
		sendToEnd();
		break;
	case ID_AUX_SHOWSCRIPT:
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_SCRIPT, 0);
		break;
	case ID_AUX_SHOWATTR:
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSPRO, 0);
		break;
	case ID_AUX_COLUMNEDIT:
		if (m_selF >= 0 && m_block->count <= 1)
		{
			if (m_formItem->GetForm(m_selF)->kind != FM_GRIDEX && m_formItem->GetForm(m_selF)->kind != FM_TABLE && m_formItem->GetForm(m_selF)->kind != FM_GRID)
				break;

			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)TRUE);
			CGridColumn	dlg(this, m_mapH, m_selF);
			dlg.DoModal();
			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)FALSE);
			Invalidate();
		}
		break;
	case ID_AUX_SETGROUP:
		setGroup();
		break;
	case ID_AUX_RELEASEGROUP:
		releaseGroup();
		break;
	case ID_AUX_ADDITEM:
		addItem();
		break;
	case ID_AUX_SELCOLUMN:
		selectColumn();
		break;
	case ID_AUX_FIT:
		columnFit();
		break;
	case ID_AUX_EXTID:
		extMenu();
		break;
	}
}

void CChildView::Init(struct _mapH* mapH, CAmDraw* draw, CAmPalette* palette, struct _block* block, CString dir)
{
	m_mapH  = mapH;
	m_formItem = new CFormItem(m_mapH);

	m_draw  = draw;
	m_palette = palette;
	m_block = block;
	
	ChangeKind();

	CTlPointer* pointer = new CTlPointer(this, draw, m_formItem);
	m_toolOBs.Add(pointer);

	CTlText* label = new CTlText(this, draw, m_formItem, ID_TOOL_LABEL);
	m_toolOBs.Add(label);

	CTlBox* box = new CTlBox(this, draw, m_formItem);
	m_toolOBs.Add(box);
	
	CTlPanel* group = new CTlPanel(this, draw, m_formItem, ID_TOOL_GROUP);
	m_toolOBs.Add(group);

	CTlPanel* panel = new CTlPanel(this, draw, m_formItem, ID_TOOL_IMAGEVIEW);
	m_toolOBs.Add(panel);

	CTlButton* push = new CTlButton(this, draw, m_formItem, ID_TOOL_PUSH);
	m_toolOBs.Add(push);

	CTlButton* radio = new CTlButton(this, draw, m_formItem, ID_TOOL_RADIO);
	m_toolOBs.Add(radio);
	
	CTlButton* check = new CTlButton(this, draw, m_formItem, ID_TOOL_CHECK);
	m_toolOBs.Add(check);

	CTlInOut* edit = new CTlInOut(this, draw, m_formItem, ID_TOOL_EDIT);
	m_toolOBs.Add(edit);

	CTlInOut* combo   = new CTlInOut(this, draw, m_formItem, ID_TOOL_COMBO);
	m_toolOBs.Add(combo);

	CTlInOut* out = new CTlInOut(this, draw, m_formItem, ID_TOOL_OUT);
	m_toolOBs.Add(out);
	
	CTlGrid* grid = new CTlGrid(this, draw, m_formItem, ID_TOOL_GRID);
	m_toolOBs.Add(grid);
	
	CTlGrid* gridex = new CTlGrid(this, draw, m_formItem, ID_TOOL_GRIDEX);
	m_toolOBs.Add(gridex);

	CTlGrid* table    = new CTlGrid(this, draw, m_formItem, ID_TOOL_TABLE);
	m_toolOBs.Add(table);

	CTlRegion* object = new CTlRegion(this, draw, m_formItem, ID_TOOL_OBJECT);
	m_toolOBs.Add(object);

	CTlTab* tab = new CTlTab(this, draw, m_formItem, ID_TOOL_TAB);
	m_toolOBs.Add(tab);
	
	CTlRegion* browser = new CTlRegion(this, draw, m_formItem, ID_TOOL_BROWSER);
	m_toolOBs.Add(browser);

	CTlInOut* control = new CTlInOut(this, draw, m_formItem, ID_TOOL_CONTROL);
	m_toolOBs.Add(control);

	m_tool = pointer;
}

void CChildView::DrawForm(int cmdID, CString param)
{
	if (m_mapH->mapK == MK_PROCEDURES || m_bLine)
		return;

	if (m_block->count)
	{
		for (int jj = m_formItem->GetCount() - 1; jj >= 0; jj--)
		{
			if (m_formItem->GetForm(jj)->block)
				m_formItem->GetForm(jj)->block = false;
		}
		m_block->count = 0;
	}
	m_block->bRc.SetRectEmpty();

	int	tool = cmdID - ID_TOOL_BASE;
	if (tool > 0)
		m_selected = true;
	else
		m_selected = false;

	m_param = param;
	m_tool = (CTlBase *)m_toolOBs.GetAt(tool);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, cmdID);
}

void CChildView::AlignForms(int cmdID)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	if ((cmdID >= ID_AL_LEFT && cmdID <= ID_BLK_VCENTER) && m_stdF < 0)
		return;

	int	stdV, ii, delta;
	long	minV, maxV;
	CRect	wRc, pRc;
	struct	_formR*	formR;

	GetClientRect(pRc);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	switch (cmdID)
	{
	case ID_AL_LEFT:
		setBlockForm(m_formItem->GetForm(m_stdF)->left, 0);
		break;

	case ID_AL_TOP:
		setBlockForm(m_formItem->GetForm(m_stdF)->top, 1);
		break;

	case ID_AL_RIGHT:
		setBlockForm(m_formItem->GetForm(m_stdF)->right, 2);
		break;

	case ID_AL_BOTTOM:
		setBlockForm(m_formItem->GetForm(m_stdF)->bottom, 3);
		break;

	case ID_BLK_HCENTER:
		pRc = getFormRect(m_stdF);
		stdV = pRc.left + pRc.Width() / 2;
		setBlockForm(stdV, 4);
		break;

	case ID_BLK_VCENTER:
		pRc = getFormRect(m_stdF);
		stdV = pRc.top + pRc.Height() / 2;
		setBlockForm(stdV, 5);
		break;

	case ID_WIN_HCENTER:
		if (m_block->count == 0 && m_selF == -1)
			break;

		GetClientRect(&wRc);
		stdV = wRc.Width() / 2;
		minV = wRc.right;
		maxV = 0;
		if (m_block->count != 0)
		{
			pRc.SetRectEmpty();
			for (ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				if (m_formItem->GetForm(ii)->block)
					pRc.UnionRect(pRc, getFormRect(ii));
			}

			delta = wRc.left + stdV - pRc.left - pRc.Width() / 2;

			for (ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				if (m_formItem->GetForm(ii)->block)
					moveForm(ii, delta, 0);
			}
		}
		else
		{
			pRc = getFormRect(m_selF);
			delta = pRc.Width() / 2;
			moveForm(m_selF, stdV - delta + wRc.left - pRc.left, 0);
		}
		break;

	case ID_WIN_VCENTER:
		if (m_block->count == 0 && m_selF == -1)
			break;

		GetClientRect(&wRc);
		stdV = wRc.Height() / 2;
		minV = wRc.bottom;
		maxV = 0;
		if (m_block->count != 0)
		{
			pRc.SetRectEmpty();
			for (ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				if (m_formItem->GetForm(ii)->block)
					pRc.UnionRect(pRc, getFormRect(ii));
			}

			delta = wRc.top + stdV - pRc.top - pRc.Height() / 2;

			for (ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				if (m_formItem->GetForm(ii)->block)
					moveForm(ii, 0, delta);
			}
		}
		else
		{
			pRc = getFormRect(m_selF);
			delta = pRc.Height() / 2;
			moveForm(m_selF, 0, stdV - delta + wRc.top - pRc.top);
		}
		break;

	case ID_SPC_HEQUAL:
		if (m_block->count == 0 && m_selF == -1)
			break;

		GetClientRect(&wRc);
		minV = wRc.right;
		maxV = 0;
		pRc.SetRect(wRc.right, wRc.bottom, 0, 0);

		if (m_block->count == 0 || m_block->count == 1)
			return;

		for (ii = 0 ; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block)
			{
				minV = __min(minV, formR->left);
				maxV = __max(maxV, formR->left);

				pRc.left   = __min(pRc.left,   formR->left);
				pRc.top    = __min(pRc.top,    formR->top);
				pRc.right  = __max(pRc.right,  formR->right);
				pRc.bottom = __max(pRc.bottom, formR->bottom);
			}
			formR->drawn = false;
		} 
		stdV = (maxV - minV)/(m_block->count - 1);
		spaceEqual(stdV, 0, maxV, 0);	
		break;

	case ID_SPC_VEQUAL:
		if (m_block->count == 0 && m_selF == -1)
			break;

		GetClientRect(&wRc);
		minV = wRc.bottom;
		maxV = 0;
		pRc.SetRect(wRc.right, wRc.bottom, 0, 0);

		if (m_block->count == 0 || m_block->count == 1)
			return;

		for (ii = 0 ; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block)
			{
				minV = __min(minV, formR->top);
				maxV = __max(maxV, formR->top);

				pRc.left   = __min(pRc.left,   formR->left);
				pRc.top    = __min(pRc.top,    formR->top);
				pRc.right  = __max(pRc.right,  formR->right);
				pRc.bottom = __max(pRc.bottom, formR->bottom);
			}
			formR->drawn = false;
		} 
		stdV = (maxV - minV)/(m_block->count - 1);
		spaceEqual(stdV, 0, maxV, 0, false);	
		break;

	case ID_SZ_HEQUAL:
		pRc = getFormRect(m_stdF);
		stdV = pRc.Width();
		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block)
			{
				if (formR->group)
				{
					struct _formR *formx, *nGroup = formR->group;
					for (int jj = 0; jj < m_formItem->GetCount(); jj++)
					{
						formx = m_formItem->GetForm(jj);
						if (formx->group == nGroup)
							formx->right = formx->left + stdV;
					}
				}
				else
					formR->right = formR->left + stdV;
			}
		}
		break;

	case ID_SZ_VEQUAL:
		pRc = getFormRect(m_stdF);
		stdV = pRc.Height();
		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block)
			{
				if (formR->group)
				{
					struct _formR *formx, *nGroup = formR->group;
					for (int jj = 0; jj < m_formItem->GetCount(); jj++)
					{
						formx = m_formItem->GetForm(jj);
						if (formx->group == nGroup)
							formx->bottom = formx->top + stdV;
					}
				}
				else
					formR->bottom = formR->top + stdV;
			}
		}
		break;
	}

	Invalidate();
}

void CChildView::RedrawView()
{	
	m_bRedraw = true;
	RedrawWindow();
}

void CChildView::ChangeKind()
{
	if (m_mapH && m_mapH->mapK == MK_PROCEDURES)
	{
		m_pSCEdit = new CVBScriptEdit();
		m_pSCEdit->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 100);

		loadAutoList();
		m_pSCEdit->Initialize();
		m_pSCEdit->Initialize(m_mapH);
	}
}

void CChildView::SortIoOrder()
{
	if (m_selF < 0)
		return;

	CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	struct	_formR*	formR;
	int	tmp, nMax = 0, nEtcSeq = 0;

	mapArr.RemoveAll();
	sortArr.RemoveAll();
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (isValidIOOrder(ii))
		{
			int	index = formR->iorder;
			if (index)
			{
				if (formR->kind == FM_TABLE)
				{
					if (!(formR->attr2 & GO_MULTI) && !(formR->attr2 & GO_XHEAD))
					{
						formR->iorder = 0;
						continue;
					}
				}
				while (mapArr.Lookup(index, tmp))
				{
					index = ++m_nSeqIoOdr;
					formR->iorder = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				if (formR->kind == FM_TABLE)
				{
					if (!(formR->attr2 & GO_MULTI) && !(formR->attr2 & GO_XHEAD))
						continue;
				}
				index = ++nEtcSeq;
				mapArr.SetAt(index + m_formItem->GetCount(), ii);
			}
		}
		else
			formR->iorder = 0;
	}

	for (int ii = 1; ii <= nMax; ii++)
	{
		if (mapArr.Lookup(ii, tmp) && (!m_nIoOdr || tmp != m_selF))
			sortArr.Add(tmp);
	}

	for (int ii = 1; ii <= nEtcSeq; ii++)
	{
		if (mapArr.Lookup(ii + m_formItem->GetCount(), tmp) && (!m_nIoOdr || tmp != m_selF))
			sortArr.Add(tmp);
	}

	if (m_nIoOdr)
	{
		sortArr.InsertAt(m_nIoOdr - 1, m_selF);
		
		if (m_nIoOdr <= m_formItem->GetForm(m_selF)->iorder && m_nIoOdr < sortArr.GetSize())
			m_nIoOdr++;
	}

	for (int ii = 0; ii < sortArr.GetSize(); ii++)
		m_formItem->GetForm(sortArr.GetAt(ii))->iorder = ii + 1;

	m_nSeqIoOdr = (int)sortArr.GetSize();
	sortArr.RemoveAll();
	mapArr.RemoveAll();
}

void CChildView::SortTabOrder()
{
	if (m_selF < 0)
		return;

	CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	struct	_formR*	formR;
	int	tmp, nMax = 0;

	mapArr.RemoveAll();
	sortArr.RemoveAll();

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (isValidTabOrder(ii))
		{
			int	index = formR->index;
			if (index)
			{
				while (mapArr.Lookup(index, tmp))
				{
					index = ++m_nSeqTabOdr;
					formR->index = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				index = ++m_nSeqTabOdr;
				formR->index = index;
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
		}
		else
			formR->index = 0;
	}
	
	for (int ii = 1; ii <= nMax; ii++)
	{
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
		if (m_nTabOdr <= m_formItem->GetForm(m_selF)->index && m_nTabOdr < sortArr.GetSize())
			m_nTabOdr++;
	}

	for (int ii = 0; ii < sortArr.GetSize(); ii++)
		m_formItem->GetForm(sortArr.GetAt(ii))->index = ii + 1;

	m_nSeqTabOdr = (int)sortArr.GetSize();
	sortArr.RemoveAll();
	mapArr.RemoveAll();
}

void CChildView::DeleteFORMs()
{
	if (m_selF < 0)
		return;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);

	CRect	pRc;
	struct	_formR*	formR;

	pRc.left  = m_mapH->width;
	pRc.top   = m_mapH->height;
	pRc.right = pRc.bottom = 0;

	if (m_block->count > 1)
	{
		for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
		{
			formR = m_formItem->GetForm(ii);
			if (formR && formR->block)
			{
				struct _formR* nGroup = formR->group;
				pRc.UnionRect(pRc, getFormRect(ii));
				if (nGroup)
				{
					for (int jj = m_formItem->GetCount() - 1; jj >= 0; jj--)
						if (m_formItem->GetForm(jj)->group == nGroup)
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
		if (m_formItem->GetForm(m_selF)->group)
		{
			struct _formR* nGroup = m_formItem->GetForm(m_selF)->group;
			for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
				if (m_formItem->GetForm(ii)->group == nGroup)
					ii = deleteForm(ii);
		}
		else
			deleteForm(m_selF);
	}

	SortTabOrder();
	SortIoOrder();
	pRc.InflateRect(5, 5);
	InvalidateRect(pRc);
	m_selF = -1;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

void CChildView::OnDrop(int kind, CPoint pt)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	if (kind == 0)	// symbol drop
	{
		Csymbol* symbol;
		CPtrArray arr;

		ScreenToClient(&pt);
		int	grid = getTableForm(pt);
		bool	is_default = AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SYMTBL, 0), (LPARAM)&arr) ? true : false;
		for (int ii = 0; ii < arr.GetSize(); ii++)
		{
			symbol = (Csymbol *)arr.GetAt(ii);
			if (symbol == (Csymbol *) NULL)
				continue;

			if (grid >= 0)
			{
				if (m_formItem->GetForm(grid)->kind == FM_TAB)
					addForm(is_default, symbol, pt);
				else
					addColumn(is_default, grid, symbol);
			}
			else
			{
				addForm(is_default, symbol, pt, NULL, ii);
				pt.y += symbol->m_datasz.cy;
			}
		}
		m_mapH->modified = true;
	}
}

void CChildView::Emulate()
{
	CEmulCtrl* pWizard = (CEmulCtrl*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (!pWizard)
		return;

	CMainFrame * pMain = (CMainFrame*)AfxGetMainWnd();

	if (m_pEmulateWnd)
	{
		pWizard->axDetach(m_emulateKey);

		if (m_pEmulateWnd)
			m_pEmulateWnd->PostMessage(WM_CLOSE);

		m_pEmulateWnd = NULL;
		m_bEmulRun = false;
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
		return;
	}

	CRect	rect;
	CString	fname;
	CFileFind finder;

	GetClientRect(&rect);
	{
		long	size, wndType;
	
		wndType = 0;
		wndType = pWizard->axWorkView(m_mapH->mapN, &size);
		if (wndType == 0)
		{
			AfxMessageBox(_T("Loading MapInfo Error!"));
			pMain->DestroyEmulator();
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
			return;
		}
		
		m_pEmulateWnd = new CEmulateWnd();
		m_pEmulateWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, 0);
		m_bEmulRun = true;

		m_emulateKey = pWizard->axAttach(m_pEmulateWnd, wndType, 0);


		if (m_emulateKey
			&& pWizard->axWorkShop(m_emulateKey, m_mapH->mapN, size, false))
		{
			m_pEmulateWnd->SetWindowPos(&CWnd::wndTop, 0, 0, LOWORD(size),  HIWORD(size),  SWP_NOMOVE);
			m_pEmulateWnd->Invalidate();
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, FALSE);
			m_pEmulateWnd->RedrawWindow();
			m_pEmulateWnd->SetFocus();
		}
		else
		{
			AfxMessageBox(_T("Creating Map Key Error!"));
			pMain->DestroyEmulator();
			delete m_pEmulateWnd;
			m_pEmulateWnd = NULL;
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
		}
	}
	
}

CString CChildView::GetInitSymbol(CString slist)
{
	CString sym;
	int	initN, fN = m_formItem->GetCount() - 1;

	if (slist.IsEmpty())
	{
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			slist += m_formItem->GetFormStr(ii, _T("name"));
			slist += _T('\t');
		}
	}

	initN = 0;
	for (;;)
	{
		sym.Format(_T("%s%d"), GetControlName(m_formItem->GetForm(fN)->kind, m_formItem->GetForm(fN)->type), initN);
		if (slist.Find(sym) == -1)
			break;
		else
			initN++;
	}
	return sym;
}

CString CChildView::GetControlName(int kind, int type)
{
	switch (kind)
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
		return _T("SUBFORM");
	case FM_TAB:
		return _T("TAB");
	case FM_BROWSER:
		return _T("BROWSER");
	case FM_CONTROL:
		return _T("CUSTOM");
	}
	return _T("SYMBOL");
}

void CChildView::SetLine(int nLine)
{
	if (m_mapH->mapK != MK_PROCEDURES)
		return;

	CString	txt;
	for (int ii = 0; ii <= nLine; ii++)
	{
		txt = m_pSCEdit->GetLineString(ii);
		txt.Remove(_T('\r'));
		txt.Remove(_T('\t'));
		txt.Remove(_T(' '));
		if (txt.IsEmpty())
			nLine++;
		if (nLine >= m_pSCEdit->GetLineCnt())
			break;
	}
}

void CChildView::InvalidateRect2(int idx)
{
	if (idx >= 0)
	{
		CRect	tRc;
		struct	_formR*	formR;

		if (m_block->count >= 2)
		{
			for (int ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				formR = m_formItem->GetForm(ii);
				if (formR->block)
				{
					tRc = getFormRect(ii);
					tRc.InflateRect(5, 5);
					InvalidateRect(tRc);
				}
			}
		}
		else
		{
			tRc = getFormRect(idx);
			tRc.InflateRect(5, 5);
			InvalidateRect(tRc);
		}
	}
	else
		Invalidate();
}

void CChildView::SetGap(int nGap)
{
	m_nGap = nGap;
	if (m_toolOBs.GetSize())
		((CTlPointer*)m_toolOBs.GetAt(0))->SetGap(nGap);
}

void CChildView::ShowForm(bool show)
{
	CRect	tRc;

	m_showFORM = show;
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		if (!(m_formItem->GetForm(ii)->properties & PR_VISIBLE))
		{
			tRc = getFormRect(ii);
			InvalidateRect(tRc);
                }
        }
}

COLORREF CChildView::GetRGB(int color)
{
	return m_palette->GetPaletteRGB(color);
}

char* CChildView::getRoot()
{
	return (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);

}

int CChildView::getDefaultValue(CString sSection, CString sKey, int nDefault)
{
	CString	path;

	path.Format(_T("%s\\%s\\awDefault.ini"), getRoot(), TABDIR);
	return GetPrivateProfileInt(sSection, sKey, nDefault, path);

}

bool CChildView::isValidTabOrder(int idx)
{
	struct	_formR*	formR = m_formItem->GetForm(idx);

	switch (formR->kind)
	{
	case FM_EDIT:
	case FM_BUTTON:
	case FM_COMBO:
	case FM_OBJECT:	
	case FM_GRID:
	case FM_GRIDEX:
		return true;
	case FM_CONTROL:
		if (formR->iok == EIO_INPUT || formR->iok == EIO_INOUT)
			return true;
		break;
	}
	return false;
}

bool CChildView::isValidIOOrder(int idx)
{
	struct	_formR*	formR = m_formItem->GetForm(idx);

	switch (formR->kind)
	{
	case FM_EDIT:
	case FM_OUT:
	case FM_COMBO:
	case FM_GRID:
	case FM_GRIDEX:
	case FM_TABLE:
	case FM_OBJECT:
		return true;
	case FM_BROWSER:
		if (formR->iok == EIO_OUTPUT)
			return true;
		break;
	case FM_RADIO:
	case FM_CHECK:
	case FM_CONTROL:
		if (formR->iok != EIO_NOP)
			return true;
		break;
	}
	return false;
}

void CChildView::loadAutoList()
{
	int	nScriptID = 100;
	CMapStringToPtr mapFormID;

	mapFormID.SetAt("LABEL",   (void*)FM_LABEL);
	mapFormID.SetAt("BOX",     (void*)FM_BOX);
	mapFormID.SetAt("GROUP",   (void*)FM_GROUP);
	mapFormID.SetAt("IMAGEVIEW",(void*)FM_IMAGEVIEW);
	mapFormID.SetAt("BUTTON",  (void*)FM_BUTTON);
	mapFormID.SetAt("EDIT",    (void*)FM_EDIT);
	mapFormID.SetAt("COMBO",   (void*)FM_COMBO);
	mapFormID.SetAt("OUTPUT",  (void*)FM_OUT);
	mapFormID.SetAt("GRID",    (void*)FM_GRID);
	mapFormID.SetAt("GRIDEX",  (void*)FM_GRIDEX);
	mapFormID.SetAt("TABLE",   (void*)FM_TABLE);
	mapFormID.SetAt("OBJECT",  (void*)FM_OBJECT);
	mapFormID.SetAt("TAB",     (void*)FM_TAB);
	mapFormID.SetAt("BROWSER", (void*)FM_BROWSER);
	mapFormID.SetAt("CONTROL", (void*)FM_CONTROL);
	mapFormID.SetAt("RADIO",   (void*)FM_RADIO);
	mapFormID.SetAt("CHECK",   (void*)FM_CHECK);

	char	twb[256];
	CString path, info, sLevel = _T("CONTROLS");

	path.Format(_T("%s\\%s\\public.ini"), getRoot(), TABDIR);
	for (int ii = 1; ; ii++)
	{
		void*	rtValue;
		int	FormNum;
		CString	tmps;

		tmps.Format(_T("%02d"), ii);
		GetPrivateProfileString(sLevel, tmps, _T(""), twb, sizeof(twb), path);
		tmps = twb;
		tmps = parse(tmps, _T(";"));

		if (tmps.IsEmpty())
		{
			if (!sLevel.CompareNoCase(_T("controls")))
			{
				sLevel = _T("public");
				ii = 0;
				continue;
			}
			break;
		}

		if (!sLevel.CompareNoCase(_T("controls")))
		{
			if (mapFormID.Lookup(tmps, rtValue))
				FormNum = (int)rtValue;
			else
			{
				if (tmps.IsEmpty())
					continue;
				else
				{
					FormNum = nScriptID;
					nScriptID++;
				}
			}
		}
		else
			FormNum = 200 + ii;

		info = _T("");
		for (int jj = 1; ; jj++)
		{
			CString str;

			str.Format(_T("%02d"), jj);
			GetPrivateProfileString(tmps, str, _T(""), twb, sizeof(twb), path);
			str = twb;
			if (str.IsEmpty())
				break;
			
			info += str;
			info += _T('\t');
		}

		if (!info.IsEmpty())
		{
			m_pSCEdit->m_AutoListMap.SetAt(FormNum, info);
			if (!sLevel.CompareNoCase(_T("controls")) && FormNum >= 100)
				m_pSCEdit->m_ScriptStrMap.SetAt(tmps, (void*)FormNum);
			tmps.MakeUpper();
		}
	}

	mapFormID.RemoveAll();
}

void CChildView::extMenu()
{
	if (m_selF < 0)
		return;

	struct	_formR*	formR = m_formItem->GetForm(m_selF);
	switch (formR->kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_OUT:
		{
			CAttrDlg dlg(this, formR->kind, formR->type, formR->iok, formR->attr, formR->attr2, HIWORD(formR->onEdit), LOWORD(formR->onEdit));
			if (dlg.DoModal() == IDOK)
			{
				formR->attr  = dlg.m_attr;
				formR->attr2 = dlg.m_attr2;
				formR->onEdit = MAKELONG(dlg.m_dnRGB, dlg.m_upRGB);
			}
		}
		break;
	case FM_CONTROL:
		break;

	case FM_TAB:
		{
			CTabPage dlg(this, m_mapH, m_selF);
			
			if (dlg.DoModal() == IDOK)
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, -1);
		}
		break;
	case FM_OBJECT:
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, (LPARAM)(m_formItem->GetFormStr(m_selF, _T("str")).operator LPCTSTR()));
		break;
	}
	Invalidate();
}

void CChildView::columnFit()
{
	if (m_selF < 0)
		return;

	struct	_formR*	formR = m_formItem->GetForm(m_selF);
	if (formR->kind == FM_GRID ||formR->kind == FM_GRIDEX)
	{
		int	nWidth, nVisibleCnt, nFormWidth;
		struct _repR *repR  = (struct _repR *)formR->auxR;
		CCellItem *cellItem = new CCellItem(repR);

		nWidth = nVisibleCnt = 0;
		nFormWidth = formR->right - formR->left - 4;
		if (formR->attr2 & GO_VSCR)
			nFormWidth -= szSCROLL;
		
		if (repR->selected >= 0)
		{
			for (int ii = 0; ii < cellItem->GetCount(); ii++)
			{
				if (cellItem->GetCell(ii)->properties & PR_VISIBLE)
				{
					nWidth += cellItem->GetCell(ii)->width;
					nVisibleCnt++;
				}
			}
			nWidth -= cellItem->GetCell(repR->selected)->width;
			cellItem->GetCell(repR->selected)->width = nFormWidth - nWidth;
			if (nFormWidth - nWidth < 0)
			{
				for (int ii = 0; ii < cellItem->GetCount(); ii++)
				{
					nWidth = (nFormWidth * (ii + 1)) / nVisibleCnt - (nFormWidth * ii) / nVisibleCnt;
					cellItem->GetCell(ii)->width = nWidth;
				}
			}
		}
		else
		{
			for (int ii = 0; ii < cellItem->GetCount(); ii++)
			{
				if (cellItem->GetCell(ii)->properties & PR_VISIBLE)
					nVisibleCnt++;
			}
			for (int ii = 0; ii < cellItem->GetCount(); ii++)
			{
				if (cellItem->GetCell(ii)->properties & PR_VISIBLE)
				{
					nWidth = (nFormWidth * (ii + 1)) / nVisibleCnt - (nFormWidth * ii) / nVisibleCnt;
					cellItem->GetCell(ii)->width = nWidth;
				}
			}
		}
		
		delete cellItem;
		InvalidateRect2(m_selF);
	}
	else if (formR->kind == FM_BUTTON)
	{
		if (formR->properties & (PR_IMAGE | PR_IMAGETEXT))
		{
			if (!m_formItem->GetFormStr(m_selF, _T("str2")).IsEmpty())
			{
				CString	path, tmps = m_formItem->GetFormStr(m_selF, _T("str2"));
				if (tmps.Find("\\") == -1)
					path.Format(_T("%s\\%s\\%s"), getRoot(), IMAGEDIR, tmps);
				else
					path = tmps;
				
				CTlImage pic(m_draw, path);
				CSize	sz = pic.GetSize();

				if (sz != CSize(0, 0))
				{
					formR->right  = formR->left + sz.cx + 1;
					formR->bottom = formR->top + sz.cy + 1;
					
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
					Invalidate();
				}
			}
		}
	}
}

CString CChildView::getItemSymbol(CString sSymbol)
{
	CString sym, slist;
	int	initN;
	struct	_repR*	repR   = (struct _repR *)m_formItem->GetForm(m_selF)->auxR;

	if (!repR)
		return sSymbol;

	CCellItem cellItem(repR);

	for (int ii = 0; ii < cellItem.GetCount(); ii++)
	{
		if (cellItem.GetCell(ii)->name[0] != NULL)
		{
			slist += cellItem.GetCell(ii)->name;
			slist += _T('\t');
		}
	}

	initN = 0;
	for (;;)
	{
		sym.Format(_T("%s%d"), sSymbol, initN);
		if (slist.Find(sym + _T("\t")) == -1)
			break;
		else
			initN++;
	}
	
	return sym;
}

void CChildView::selectColumn()
{
	m_bSelect = true;
	SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKEWPARAM(m_RBDPnt.x, m_RBDPnt.y));
	SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKEWPARAM(m_RBDPnt.x, m_RBDPnt.y));
}

void CChildView::addCol()
{
	struct _cellR *cellR;
	struct _repR *repR = (struct _repR *)m_formItem->GetForm(m_selF)->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	if (m_formItem->GetForm(m_selF)->kind == FM_GRIDEX || m_formItem->GetForm(m_selF)->kind == FM_GRID)
	{
		cellR = cellItem->GetCell(cellItem->AddCell());
		cellR->pRGB = getDefaultValue(_T("GRID"), _T("DPRGB"), basINPUT);
		cellR->tRGB = getDefaultValue(_T("GRID"), _T("DTRGB"), 232);
		if (repR->cols)
		{
			cellR->hpRGB = getDefaultValue(_T("GRID"), _T("HPRGB"), cellItem->GetCell(repR->cols - 1)->hpRGB);
			cellR->htRGB = getDefaultValue(_T("GRID"), _T("HTRGB"), cellItem->GetCell(repR->cols - 1)->htRGB);
			cellR->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), cellItem->GetCell(repR->cols - 1)->width);
		}
		else
		{
			cellR->hpRGB = getDefaultValue(_T("GRID"), _T("HPRGB"), basPRGB);
			cellR->htRGB = getDefaultValue(_T("GRID"), _T("HTRGB"), 232);
			cellR->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), 80);
		}
		cellR->size = 0;
		//cellR->headN = 1;
		cellR->type = IO_NUM;
		cellR->iok = EIO_OUTPUT;
		cellR->kind = CK_NORMAL;
		strcpy_s(cellR->name, sizeof(cellR->name), getItemSymbol(_T("Col")));
		if (repR->cols == 0)
			cellR->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), 80);
		else
			cellR->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), cellItem->GetCell(0)->width);
		cellR->alignC = AL_RIGHT;
		cellR->properties |= PR_VISIBLE;
		
		repR->cols++;
	}
	else
	{
		cellR = cellItem->GetCell(cellItem->AddCell());
		cellR->pRGB = getDefaultValue(_T("TABLE"), _T("DPRGB"), basINPUT);
		cellR->tRGB = getDefaultValue(_T("TABLE"), _T("DTRGB"), 232);
		if (repR->rows)
		{
			cellR->hpRGB = getDefaultValue(_T("TABLE"), _T("HPRGB"), cellItem->GetCell(repR->rows - 1)->hpRGB);
			cellR->htRGB = getDefaultValue(_T("TABLE"), _T("HTRGB"), cellItem->GetCell(repR->rows - 1)->htRGB);
			cellR->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), cellItem->GetCell(repR->rows - 1)->width);
		}
		else
		{
			cellR->hpRGB = getDefaultValue(_T("TABLE"), _T("HPRGB"), basPRGB);
			cellR->htRGB = getDefaultValue(_T("TABLE"), _T("HTRGB"), 232);
			cellR->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), 80);
		}
		cellR->size = 0;
		//cellR->headN = 1;
		cellR->type = IO_NUM;
		strcpy_s(cellR->name, sizeof(cellR->name), getItemSymbol(_T("Row")));
		if (repR->rows == 0)
			cellR->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), 80);
		else
			cellR->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), cellItem->GetCell(0)->width);
		cellR->alignC = AL_RIGHT;
		cellR->properties |= PR_VISIBLE;
		
		repR->rows++;
	}
	delete cellItem;

	Invalidate();
}

void CChildView::addPage(int nPage)
{
	struct _formR *formR = m_formItem->GetForm(m_selF);
	if (nPage >= 0)
	{
		CString	data, str, tmps;

		data = m_formItem->GetFormStr(m_selF, _T("dat"));
		str  = m_formItem->GetFormStr(m_selF, _T("str"));
		
		tmps.Format(_T("Tab%d\t"), formR->keys);
		data += tmps;

		tmps.Format(_T("1:%d\t"), formR->keys);
		str += tmps;

		formR->keys++;
		m_formItem->SetFormStr(m_selF, _T("dat"), data);
		if (formR->kind == FM_TAB)
			m_formItem->SetFormStr(m_selF, _T("str"), str);
		
		if (nPage >= (int)formR->keys)
			formR->checked = 0;
		formR->checked = nPage;
	}
	Invalidate();
}

void CChildView::addItem()
{
	if (m_selF < 0)
		return;

	switch (m_formItem->GetForm(m_selF)->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
	case FM_TABLE:
		addCol();
		break;

	case FM_TAB:
		addPage(m_formItem->GetForm(m_selF)->checked);
		break;
	}
}

int CChildView::deleteForm(int idx)
{
	m_formItem->RemoveForm(idx, true);
	return m_formItem->GetCount();
}

void CChildView::moveForm(int idx, int mx, int my, bool bSize)
{
	struct _formR* formR = m_formItem->GetForm(idx);

	if (formR->group)
		setBlockGroup(idx, mx, my, bSize);
	else
	{
		if (!bSize)
		{
			formR->left += mx;
			formR->top  += my;
		}
		formR->right  += mx;
		formR->bottom += my;
		if (formR->left > formR->right)
			formR->right = formR->left;
		if (formR->top > formR->bottom)
			formR->bottom = formR->top;
	}
	if (!m_block->count)
	{// 위치 갱신
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
	}
}

CRect CChildView::moveForm(int mx, int my, bool bSize)
{
	CRect	pRc;
	struct	_formR*	formR;

	pRc.SetRectEmpty();
	if (!(::GetKeyState(VK_CONTROL) & 0x8000) && m_nGap != 1)
	{
		int	xx, yy;

		formR = m_formItem->GetForm(m_selF);
		if (bSize)
		{
			xx = formR->right;
			yy = formR->bottom;
		}
		else
		{
			xx = formR->left, 
			yy = formR->top;
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
	
	if (m_block->count == 0 || m_block->count == 1)
	{
		formR = m_formItem->GetForm(m_selF);
		if (formR->group)
		{
			pRc = groupRect(formR->group);
			setBlockGroup(m_selF, mx, my, bSize);
			pRc.InflateRect(abs(mx), abs(my));
		}
		else
		{
			pRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
			if (!bSize)
			{
				formR->left += mx;
				formR->top  += my;
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
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
			if (m_formItem->GetForm(ii)->block)
				pRc.UnionRect(pRc, getFormRect(ii));
		
		if (pRc.left + mx < 0)
			mx -= (pRc.left + mx);
		if (pRc.top  + my < 0)
			my -= (pRc.top + my);

		pRc.SetRectEmpty();
		for (int ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block)
			{
				if (formR->group)
				{
					pRc.UnionRect(pRc, groupRect(formR->group));
					setBlockGroup(ii, mx, my, bSize);
				}
				else
				{
					pRc.UnionRect(pRc, CRect(formR->left, formR->top, formR->right, formR->bottom));
					if (!bSize)
					{
						formR->left += mx;
						formR->top  += my;
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
		pRc.InflateRect(abs(mx), abs(my));
	}
	if (!m_block->count)
	{// 위치 갱신
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
	}
	return pRc;
}

CRect CChildView::sizeForm(int mx, int my)
{
	return moveForm(mx, my, true);
}

void CChildView::setBlockForm(int value, int type)
{
	struct _formR*	formR;

	for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->block)
		{
			int dValue = 0;
			switch(type)
			{
			case 0:	// left
				dValue = value - formR->left;
				if (formR->group)
					dValue = value - groupRect(formR->group).left;
				moveForm(ii, dValue, 0);
				break;
			case 1: // top
				dValue = value - formR->top;
				if (formR->group)
					dValue = value - groupRect(formR->group).top;
				moveForm(ii, 0, dValue);
				break;
			case 2: // right
				dValue = value - formR->right;
				if (formR->group)
					dValue = value - groupRect(formR->group).right;
				moveForm(ii, dValue, 0);
				break;
			case 3: // bottom
				dValue = value - formR->bottom;
				if (formR->group)
					dValue = value - groupRect(formR->group).bottom;
				moveForm(ii, 0, dValue);
				break;
			case 4: // VCENTER
				dValue = value - (formR->right - formR->left) / 2 - formR->left;
				if (formR->group)
					dValue = value - (groupRect(formR->group).right - groupRect(formR->group).left) / 2 - groupRect(formR->group).left;
				moveForm(ii, dValue, 0);
				break;
			case 5: // HCENTER
				dValue = value - (formR->bottom - formR->top) / 2 - formR->top;
				if (formR->group)
					dValue = value - (groupRect(formR->group).bottom - groupRect(formR->group).top) / 2 - groupRect(formR->group).top;
				moveForm(ii, 0, dValue);
				break;
			}
		}
	}
}

void CChildView::setBlockGroup(int nGroup, int mx, int my, bool bSize)
{
	struct	_formR *formR, *formx = m_formItem->GetForm(nGroup);

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->group == formx->group)
		{
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

CRect CChildView::groupRect(struct _formR* nGroup)
{
	CRect	rc, tRc;
	struct	_formR*	formR;

	rc.SetRectEmpty();
	for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->group == nGroup)
		{
			tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
			rc.UnionRect(rc, tRc);
		}
	}
	return rc;
}

void CChildView::setObject(int index)
{
	CRect	rect;
	struct	_formR*	formR = m_formItem->GetForm(index);
	rect.SetRect(formR->left, formR->top, formR->right, formR->bottom);

	CShowObject* form = new CShowObject(this, rect, getRoot(), m_formItem->GetForm(index)->str);

	form->m_bfirst = false;

	m_forms.SetAt(index, form);
}

void CChildView::drawBackground(CDC *pDC)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	CBrush	*brush;

	switch (1/*m_mapH->bgK*/)//ljh
	{
	case IK_BRUSH:
		DWORD	value;
		value = m_mapH->bgClr;
		if (value & 0x02000000)
			brush = new CBrush(value);
		else
			brush = new CBrush(m_palette->GetPaletteRGB(value));
		break;
	case IK_BMP:
		{
			drawImage(pDC);
			if (m_hImgBack)
			{
				CBitmap bitmap;
				brush = new CBrush(bitmap.FromHandle(m_hImgBack));
				break;
			}
		}
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


	CArray<CRect, CRect> *pLine = (CArray<CRect, CRect> *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_STANDARDLINE);
	
	CRect rcline, rc;
	CPoint ptMove, ptLine;
	CPen pen, *oldpen;

	GetClientRect(rc);

	// Create a geometric pen.
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(130, 130, 130);
	pen.CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);


	oldpen = pDC->SelectObject(&pen);
	pDC->SetBkMode(TRANSPARENT);

	for (int ii = 0; ii < pLine->GetCount(); ii++)
	{
		rcline = pLine->GetAt(ii);
		if (rcline.Width() > rcline.Height())
		{
			ptMove = CPoint(rc.left, rcline.top);
			ptLine = CPoint(rc.right, rcline.top);
		}
		else
		{
			ptMove = CPoint(rcline.left, rc.top);
			ptLine = CPoint(rcline.left, rc.bottom);
		}
		pDC->MoveTo(ptMove);
		pDC->LineTo(ptLine);
	}

	pDC->SelectObject(oldpen);
	pen.DeleteObject();
	brush->DeleteObject();
	delete brush;
}

void CChildView::drawBitmap(CDC *pDC)
{
	CDC	memDC;
	CBitmap* pOldBitmap;

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

void CChildView::drawMap(CDC *pDC)
{
	CRect	tRc, clipRect, IntersectRc;
	CTlBase* tool;
	struct	_formR*	formR;

	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (!m_showFORM && !(formR->properties & PR_VISIBLE))
			continue;

		tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
		if (!IntersectRc.IntersectRect(tRc, clipRect) && formR->kind != FM_BOX)
			continue;

		switch (formR->kind)
		{
		case FM_LABEL:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_LABEL - ID_TOOL_BASE);
			break;

		case FM_BOX:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_BOX - ID_TOOL_BASE);
			break;
	
		case FM_GROUP:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_GROUP - ID_TOOL_BASE);
			break;

		case FM_IMAGEVIEW:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_IMAGEVIEW - ID_TOOL_BASE);
			break;

		case FM_BUTTON:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_PUSH - ID_TOOL_BASE);
			break;

		case FM_RADIO:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_RADIO - ID_TOOL_BASE);
			break;

		case FM_CHECK:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_CHECK - ID_TOOL_BASE);
			break;

		case FM_EDIT:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_EDIT - ID_TOOL_BASE);
			break;

		case FM_COMBO:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_COMBO - ID_TOOL_BASE);
			break;

		case FM_OUT:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_OUT - ID_TOOL_BASE);
			break;
	
		case FM_GRID:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_GRID - ID_TOOL_BASE);			
			break;
	
		case FM_GRIDEX:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_GRIDEX - ID_TOOL_BASE);
			break;

		case FM_TABLE:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_TABLE - ID_TOOL_BASE);
			break;
	
		case FM_OBJECT:			
			CShowObject*	form;
			if (!m_forms.Lookup(ii, form))
				setObject(ii);
			if (m_forms.Lookup(ii, form))
			{
				//tool = NULL;
				//break;

				//if (form->Load(formR->str))
				//{
					pDC->SelectClipRgn(NULL);
					//if (formR->bFixedSize)
					/*{
						formR->right = formR->left + form->GetSize().cx;
						formR->bottom = formR->top + form->GetSize().cy;
						tRc.right = tRc.left + form->GetSize().cx;
						tRc.bottom = tRc.top + form->GetSize().cy;
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
					}*/

					form->Resize(tRc);
					pDC->IntersectClipRect(tRc);
					form->Draw(pDC);					
					tool = NULL;

					//form->m_bfirst = true;
					break;
				//}
			}			
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_OBJECT - ID_TOOL_BASE);
			break;

		case FM_TAB:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_TAB - ID_TOOL_BASE);
			break;

		case FM_BROWSER:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_BROWSER - ID_TOOL_BASE);
			break;

		case FM_CONTROL:
			tool = (CTlBase *)m_toolOBs.GetAt(ID_TOOL_CONTROL - ID_TOOL_BASE);
			break;

		default:
			continue;
		}

		pDC->SelectClipRgn(NULL);
		if (tool)
			tool->Draw(pDC, ii);

		if (formR->block && !(formR->group && m_formItem->GetIdxOfPtr(formR->group) != ii) && m_block->count > 1)
		{
			tRc = getFormRect(ii);
			
			CTracker	mTracker(tRc, (m_stdF >= 0 && m_stdF == ii) ? (CTracker::hatchedBorder | CTracker::resizeInside) : CTracker::hatchedBorder);
			mTracker.Draw(pDC);
		}
		pDC->SelectClipRgn(NULL);

		if (m_nTabOdr && formR->index)
		{
			CString tStr;

			tRc.SetRect(formR->left, formR->top,formR->left + 20, formR->top + 20);
			pDC->FillSolidRect(tRc, RGB(0, 0, 120));
			tStr.Format(_T("%d"), formR->index);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(tStr, tRc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}

		if (m_nIoOdr && formR->iorder)
		{
			CString tStr;

			tRc.SetRect(formR->left, formR->top, formR->left + 20, formR->top + 20);
			if (formR->attr & FA_SKIP)
				pDC->FillSolidRect(tRc, RGB(120, 120, 0));
			else
				pDC->FillSolidRect(tRc, RGB(0, 120, 0));
			tStr.Format(_T("%d"), formR->iorder);
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(tStr, tRc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	if (m_block->count == 0 || m_block->count == 1)
	{
		if (m_selF != -1)
		{
			tRc = getFormRect(m_selF);
			formR = m_formItem->GetForm(m_selF);
			if (formR->group)
				m_selF = m_formItem->GetIdxOfPtr(formR->group);
			
			CTracker	tracker(tRc, CTracker::resizeOutside);
			if (!m_bLBtnDown)
				tracker.Draw(pDC);
		}
	}
	else
	{
		for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block && formR->group && m_formItem->GetIdxOfPtr(formR->group) == ii)
			{
				tRc = getFormRect(ii);
				
				CTracker	mTracker(tRc, (m_stdF >= 0 && m_stdF == ii) ? (CTracker::hatchedBorder | CTracker::resizeInside) : CTracker::hatchedBorder);
				mTracker.Draw(pDC);
			}
		}
	}
}

bool CChildView::drawImage(CDC *pDC)	// just load bitmap
{
	CString path = m_mapH->bgFileN;

	if (path.IsEmpty())
	{
		m_sImgPath = _T("");
		if (m_hImgBack)
		{
			DeleteObject(m_hImgBack);
			m_hImgBack = NULL;
		}

		return false;
	}

	if (path.Find("\\") == -1)
	{
		CString	tmps;

		tmps.Format(_T("%s\\%s\\%s"), getRoot(), IMAGEDIR, path);
		path = tmps;
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
		if (!m_hImgBack)
			return false;
	}
	else if (!m_hImgBack)
	{
		m_hImgBack = (HBITMAP)LoadImage(NULL, m_sImgPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (!m_hImgBack)
			return false;
	}
	return true;
}

CString CChildView::parse(CString& src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src = src.Mid(pos + sub.GetLength());
	return tmps;
}

void CChildView::deleteSelPage(int idx)
{
	struct	_formR*	formR = m_formItem->GetForm(idx);
	if (formR->checked >= 0)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		CString temp = m_formItem->GetFormStr(idx, _T("dat")), tmp = _T("");
		CString temp2 = m_formItem->GetFormStr(idx, _T("str")), tmp2 = _T("");
		for (int jj = 0; jj < (int)formR->keys; jj++)
		{
			if (formR->checked == jj)
			{
				parse(temp, _T("\t"));
				parse(temp2, _T("\t"));
				continue;
			}
			tmp += parse(temp, _T("\t"));
			tmp += _T('\t');
			if (formR->kind == FM_TAB)
			{
				tmp2 += parse(temp2, _T("\t"));
				tmp2 += _T('\t');
			}
		}
		m_formItem->SetFormStr(idx, _T("dat"), tmp);
		if (formR->kind == FM_TAB)
			m_formItem->SetFormStr(idx, _T("str"), tmp2);
		if (formR->keys > 0)
			formR->keys--;
		if (formR->checked >= formR->keys)
			formR->checked = 0;
		InvalidateRect2(idx);
	}
}

void CChildView::redrawSelChange(int oldF, int selF)
{
	CRect	tRc;

	if (m_block->count > 1 && (selF == -1 || !m_formItem->GetForm(selF)->block))
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

CRect CChildView::getFormRect(int idx)
{
	CRect	tRc;

	tRc.SetRectEmpty();
	if (m_formItem->GetCount() <= idx)
		return tRc;

	struct	_formR*	formR = m_formItem->GetForm(idx);
	if (!formR)
		return tRc;
	if (formR->group)
		return groupRect(formR->group);
	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	return tRc;
}

void CChildView::spaceEqual(int std, long pos, long maxV, int count, bool horizontal)
{
	int 	ii, minF, delta;
	long	minV;
	struct	_formR*	formR;
	minF = 0;

	if (horizontal)
	{
		minV = maxV;
		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block && !formR->drawn)
				minV = __min(minV, formR->left);
		}

		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block && (minV == formR->left))
			{
				minF = ii;
				count++;
				break;
			}
		}

		if (count == 1)
		{
			pos = m_formItem->GetForm(minF)->left;
			m_formItem->GetForm(minF)->drawn = true;
		}
		else
		{
			formR = m_formItem->GetForm(minF);
			delta = formR->right - formR->left;
			formR->left  = pos + std;
			formR->right = formR->left + delta;
			pos = formR->left;
			formR->drawn = true;
		}
	}
	else
	{
		minV = maxV;
		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			formR = m_formItem->GetForm(ii);
			if (formR->block && !formR->drawn)
				minV = __min(minV, formR->top);
		}

		for (ii = 0; ii < m_formItem->GetCount(); ii++)
		{
			if (m_formItem->GetForm(ii)->block && (minV == m_formItem->GetForm(ii)->top))
			{
				minF = ii;
				count++;
				break;
			}
		}

		if (count == 1)
		{
			pos = m_formItem->GetForm(minF)->top;
			m_formItem->GetForm(minF)->drawn = true;
		}
		else
		{
			formR = m_formItem->GetForm(minF);
			delta = formR->bottom - formR->top;
			formR->top    = pos + std;
			formR->bottom = formR->top + delta;
			pos = formR->top;
			formR->drawn = true;
		}
	}

	if (count < m_block->count)
		spaceEqual(std, pos, maxV, count, horizontal);
}

CRect CChildView::getRect(int fN, int flag)
{
	CRect	tRc;
	struct	_formR*	formR = m_formItem->GetForm(fN);

	if (fN >= m_formItem->GetCount() || fN < 0)
		tRc.SetRectEmpty();
	else if (formR->group)
		tRc = groupRect(formR->group);
	else
		tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	return tRc;
}

void CChildView::keyArrowLeft()
{
	if (m_selF < 0)
		return;

	CRect	pRc;

	pRc.SetRectEmpty();
	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
		pRc = sizeForm(-1, 0);
	else		// Move
		pRc = moveForm(-1, 0);

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
		pRc = sizeForm(1, 0);
	else		// Move
		pRc = moveForm(1, 0);

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
		pRc = sizeForm(0, -1);
	else	// Move
		pRc = moveForm(0, -1);

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
		pRc = sizeForm(0, 1);
	else	// Move
		pRc = moveForm(0, 1);

	m_mapH->modified = true;
	pRc.InflateRect(5, 5);
	if (!pRc.IsRectEmpty())
		InvalidateRect(pRc);
}

void CChildView::setGroup()
{
	int	nGroup = -1;
	struct	_formR*	formR;

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		formR = m_formItem->GetForm(ii);
		if (formR->block && formR->group)
		{
			if (nGroup == -1 && MessageBox(_T("It is Lost that Group Information in Group!\n\nDo you make Group?"), _T("Builder"), MB_YESNO) == IDNO)
				return;
			nGroup = (int)formR->group;
			for (int jj = 0; jj < m_formItem->GetCount(); jj++)
			{
				if ((int)m_formItem->GetForm(jj)->group == nGroup)
				{
					m_formItem->GetForm(jj)->group = 0;
					m_formItem->GetForm(jj)->block = true;
				}
			}
		}
	}

	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		if (m_formItem->GetForm(ii)->block)
		{
			m_formItem->GetForm(ii)->group = m_formItem->GetForm(m_stdF);
			m_formItem->GetForm(ii)->block = false;
		}
	}
	m_block->count = 0;
	CRect tRc = groupRect(m_formItem->GetForm(m_stdF));
	tRc.InflateRect(5, 5);
	InvalidateRect(tRc);
}

void CChildView::releaseGroup()
{
	CRect	tRc;
	struct	_formR*	formR = m_formItem->GetForm(m_selF);

	tRc = groupRect(formR);
	m_stdF = m_formItem->GetIdxOfPtr(m_formItem->GetForm(m_selF)->group);
	m_block->count = 0;
	for (int ii = 0; ii < m_formItem->GetCount(); ii++)
	{
		if (m_formItem->GetForm(ii)->group == m_formItem->GetForm(m_stdF))
		{
			m_formItem->GetForm(ii)->group = 0;
			m_formItem->GetForm(ii)->block = true;
			m_block->count++;
		}
	}
	tRc.InflateRect(5, 5);
	InvalidateRect(tRc);
}

int CChildView::getTableForm(CPoint point)
{
	CRect	tRc;
	struct	_formR*	formR;

	for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
	{
		formR = m_formItem->GetForm(ii);
		switch (formR->kind)
		{
		case FM_GRID:
		case FM_TAB:
		case FM_GRIDEX:
		case FM_TABLE:	
			break;
		default:
			continue;
		}

		tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
		if (tRc.PtInRect(point))
			return ii;
	}

	return -1;
}

bool CChildView::movable(CPoint point)
{
	struct	_formR*	formR = m_formItem->GetForm(m_selF);
	
	if (formR->auxR == (char *)0)
		return false;

	CRect	tRc;
	int	ii, width;
	struct	_repR*	repR;

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	width = tRc.left;
	repR = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);
	switch (formR->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR	*cellR = cellItem->GetCell(ii);
			if (!(cellR->properties & PR_VISIBLE))
				continue;
			if (ii < formR->scrollPos)
				continue;

			width += cellR->width;
			if (width >= tRc.right)
			{
				delete cellItem;
				return false;
			}

			if (point.x >= width-2 && point.x <= width+2)
			{
				if ((formR->attr2 & GO_HSCR) && point.y >= formR->bottom - szSCROLL)
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
		if (!(formR->attr2 & GO_MULTI) || !repR->rows)
		{
			delete cellItem;
			return false;
		}

		struct _cellR	*cellR = cellItem->GetCell(0);
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

void CChildView::addColumn(bool is_default, int grid, class Csymbol* symbol)
{
	CRect	rRc;
	int	jj;
	struct	_repR*	repR;
	struct	_formR*	formR = m_formItem->GetForm(m_selF);

	m_selF = grid;
	repR   = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	int ii = cellItem->AddCell();
	struct _cellR	*cellR = cellItem->GetCell(ii);

	strcpy_s(cellR->name, sizeof(cellR->name), symbol->m_name);
	strcpy_s(cellR->head, sizeof(cellR->head), symbol->m_head);
	cellR->size = symbol->m_size;
	cellR->type = symbol->m_type;
	cellR->iok  = symbol->m_iok;

	if (is_default)
	{
		switch (symbol->m_kind)
		{
		case FM_COMBO:
			cellR->kind = CK_COMBO;
			break;

		case FM_BUTTON:
			if (symbol->m_type == 3)	// BTN_CHECK
			{
				cellR->type = IO_NUM;
				cellR->kind = CK_CHECK;
			}
		default:
			break;
		}
	}

	switch (formR->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		repR->cols++;
		cellR->width = (WORD)symbol->m_headsz.cx;
		formR->keys  = MAKELONG((WORD)symbol->m_headsz.cy, HIWORD(formR->keys));
		cellR->width = (WORD)max(cellR->width, symbol->m_datasz.cx);
		formR->keys  = MAKELONG((DWORD)max((int)formR->keys, symbol->m_datasz.cy), HIWORD(formR->keys));
		break;

	case FM_TABLE:
		repR->rows++;
		cellItem->GetCell(0)->width  = (WORD)max(cellItem->GetCell(0)->width, symbol->m_headsz.cx);
		for (jj = 1; jj <= ii; jj++)
			cellItem->GetCell(jj)->width = cellItem->GetCell(0)->width;
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
		//cellR->isEdit =IE_CORGB;
		cellR->onEdit = MAKELONG(CORGB_DOWN, CORGB_UP);
	}
	
	if (cellR->type == IO_NUM && cellR->iok == EIO_OUTPUT)
		cellR->vals = symbol->m_float;
	
	if (!symbol->m_edit.IsEmpty())
	{
		//cellR->isEdit = IE_EDIT;
		strcpy_s(cellR->editS, sizeof(cellR->editS), symbol->m_edit);
	}
	//cellR->headN  = 1;
	cellR->properties |= PR_VISIBLE;

	delete cellItem;
	rRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	rRc.InflateRect(5, 5);
	InvalidateRect(&rRc);
}

void CChildView::addForm(bool is_default, Csymbol* symbol, CPoint pt, struct _formR *tabN, int idx)
{
	if (m_formItem->GetCount() >= MAX_FORM)
		return;

	CRect	cRc;
	struct	_formR*	formR;

	if (is_default && symbol->m_kind <= 0)
		return;

	GetClientRect(&cRc);
	
	if (symbol->m_iok == EIO_OUTPUT)
	{
		if (idx == 0)
			m_formItem->AddForm();
		// table
		m_selF = m_formItem->GetCount() - 1;
		formR  = m_formItem->GetForm(m_selF);

		formR->kind = FM_TABLE;
		if (idx == 0)
		{
			m_formItem->SetFormStr(m_selF, _T("name"), GetInitSymbol());
			formR->top  = pt.y;
			formR->left = pt.x;
		}


		formR->right  = min(cRc.right,  formR->left + symbol->m_headsz.cx + symbol->m_datasz.cx);
		formR->bottom = min(cRc.bottom, formR->top + symbol->m_headsz.cy * (idx + 1));
		formR->iok    = EIO_OUTPUT;
		formR->attr2  = (GO_MULTI | GO_AUTOSIZE);
		formR->properties |= (PR_VISIBLE | PR_ENABLE | PR_CHECKED);
		formR->checked = 0;
		formR->fStyle  = 0;
		
		formR->onTap    = -1;
		formR->onTapL   = 0;
		formR->onLongTap  = -1;
		formR->onLongTapL = 0;
		formR->onChange   = -1;
		formR->onChangeL  = 0;
		formR->onChar     = -1;
		formR->onCharL    = 0;
		formR->onDrag	  = -1;
		formR->onDragL    = 0;
		formR->onScroll   = -1;
		formR->onScrollL  = 0;
		
		SortTabOrder();
		formR->index = ++m_nSeqTabOdr;
		formR->iorder = ++m_nSeqIoOdr;

		if (!formR->auxR)
		{
			struct _repR *repR;

			formR->auxR = new char[sizeof(_repR)];
			repR = (struct _repR *)formR->auxR;
			ZeroMemory(repR, sizeof(struct _repR));
			repR->selected = -1;
		}
		addColumn(is_default, m_selF, symbol);
		formR->keys = 20;
	}
	else
	{
		m_formItem->AddForm();
		// label
		if (symbol->m_headsz.cx != 0 && symbol->m_headsz.cy != 0)
		{
			m_selF = m_formItem->GetCount() - 1;
			formR  = m_formItem->GetForm(m_selF);

			formR->kind = FM_LABEL;
			m_formItem->SetFormStr(m_selF, _T("name"), GetInitSymbol());
			formR->top    = pt.y;
			formR->left   = pt.x;
			formR->right  = min(cRc.right,  formR->left + symbol->m_headsz.cx);
			formR->bottom = min(cRc.bottom, formR->top + symbol->m_headsz.cy);
			pt.x = formR->right;
			m_formItem->SetFormStr(m_selF, _T("str"), symbol->m_head);
			formR->alignment = AL_CENTER;
			formR->pRGB = symbol->m_hpRGB;
			formR->tRGB = symbol->m_htRGB;
			
			formR->properties |= (PR_VISIBLE | PR_ENABLE | PR_CHECKED);
			formR->fPoint  = m_mapH->fPoint;
			m_formItem->SetFormStr(m_selF, _T("fName"), m_mapH->fName);
			formR->fStyle = 0;

			formR->checked   =  0;
			formR->onTap    = -1;
			formR->onTapL   = 0;
			formR->onLongTap  = -1;
			formR->onLongTapL = 0;
			formR->onChange   = -1;
			formR->onChangeL  = 0;
			formR->onChar     = -1;
			formR->onCharL    = 0;
			formR->onDrag	  = -1;
			formR->onDragL    = 0;
			formR->onScroll   = -1;
			formR->onScrollL  = 0;
			
			SortTabOrder();
			formR->index = ++m_nSeqTabOdr;
		}
		
		// data
		m_formItem->AddForm();
		m_selF = m_formItem->GetCount() - 1;
		formR = m_formItem->GetForm(m_selF);
		
		m_formItem->SetFormStr(m_selF, _T("name"), symbol->m_name);
		if (is_default)
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
		formR->size = symbol->m_size;
		formR->type = symbol->m_type;
		formR->iok  = symbol->m_iok;

		formR->top    = pt.y;
		formR->left   = pt.x + 1;
		formR->right  = min(cRc.right, formR->left + symbol->m_datasz.cx);
		formR->bottom = min(cRc.bottom, formR->top + symbol->m_datasz.cy);

		formR->alignment = symbol->m_align;
		formR->pRGB      = symbol->m_pRGB;
		formR->tRGB      = symbol->m_tRGB;
		formR->attr      = symbol->m_attr;
		if (formR->attr & FA_CORGB)
		{
			formR->attr2 |= SA_COSUP;
			//formR->isEdit = IE_CORGB;
			formR->onEdit = MAKELONG(CORGB_DOWN, CORGB_UP);
		}
		
		if (formR->type == IO_NUM && formR->kind == FM_OUT)
			formR->offs = symbol->m_float = symbol->m_float;
		
		if (!symbol->m_edit.IsEmpty())
		{
			//formR->isEdit = IE_EDIT;
			m_formItem->SetFormStr(m_selF, _T("editS"), symbol->m_edit);
		}
		
		formR->properties |= (PR_VISIBLE | PR_ENABLE | PR_CHECKED);
		formR->fPoint  = m_mapH->fPoint;
		m_formItem->SetFormStr(m_selF, _T("fName"), m_mapH->fName);
		formR->fStyle = 0;
		
		formR->checked   =  0;
		formR->onTap    = -1;
		formR->onTapL   = 0;
		formR->onLongTap  = -1;
		formR->onLongTapL = 0;
		formR->onChange   = -1;
		formR->onChangeL  = 0;
		formR->onChar     = -1;
		formR->onCharL    = 0;
		formR->onDrag	  = -1;
		formR->onDragL    = 0;
		formR->onScroll   = -1;
		formR->onScrollL  = 0;
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	Invalidate(FALSE);
}

void CChildView::showMenu()
{
	CPoint	point;
	CMenu	menu, submenu, Layoutmenu, LayoutmenuSub;
	struct _formR *formR = NULL;
	if (m_selF >= 0)
		formR = m_formItem->GetForm(m_selF);
	
	menu.CreatePopupMenu();
	submenu.CreatePopupMenu();
	Layoutmenu.CreatePopupMenu();		// Layout menu 생성
	LayoutmenuSub.CreatePopupMenu();	// Layout sub menu 생성 (new, delete, rename)
	
	GetCursorPos(&point);
	submenu.AppendMenu(MF_STRING, ID_AUX_SETGROUP, _T("Set Group"));
	submenu.AppendMenu(MF_STRING, ID_AUX_RELEASEGROUP, _T("Release Group"));
		
	if (m_selF >= 0)
	{
		menu.AppendMenu(MF_STRING, NULL, formR->name);
		menu.AppendMenu(MF_SEPARATOR);
	}
	menu.AppendMenu(MF_STRING, ID_AUX_TOFIRST, _T("Bring Foreward"));
	menu.AppendMenu(MF_STRING, ID_AUX_TOEND, _T("Send Backward"));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_AUX_SHOWSCRIPT, _T("Script\t(F6)"));
	if (m_block->count <= 1)
		menu.AppendMenu(MF_STRING, ID_AUX_SHOWATTR, _T("Properties\t(F5)"));
	menu.AppendMenu(MF_POPUP, UINT(submenu.GetSafeHmenu()),_T("Group"));
	menu.AppendMenu(MF_POPUP, UINT(Layoutmenu.GetSafeHmenu()),_T("Layout Edit"));
	menu.AppendMenu(MF_POPUP, ID_MASTERLAYOUT, _T("Add MasterLayout"));

	if (m_selF >= 0 && m_block->count <= 1)
	{
		switch (formR->kind)
		{
		case FM_GRID:
		case FM_GRIDEX:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_AUX_COLUMNEDIT, _T("Edit Column\t(F11)"));
			menu.AppendMenu(MF_STRING, ID_AUX_ADDITEM, _T("Add Item"));
			menu.AppendMenu(MF_STRING, ID_AUX_FIT, _T("Fit Column"));
			if (formR->attr2 & GO_XHEAD)
				menu.AppendMenu(MF_STRING, ID_AUX_SELCOLUMN, _T("Select Column"));
			break;
		case FM_TABLE:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_AUX_COLUMNEDIT, _T("Edit Row\t(F11)"));
			menu.AppendMenu(MF_STRING, ID_AUX_ADDITEM, _T("Add Item"));
			if (!(formR->attr2 & GO_MULTI))
				menu.AppendMenu(MF_STRING, ID_AUX_SELCOLUMN, _T("Select Row"));
			break;
		case FM_TAB:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_AUX_ADDITEM, _T("Add Item"));
			menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("PageEdit"));
			break;
		case FM_EDIT:
		case FM_COMBO:
		case FM_OUT:
			menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("Attributes..."));
			break;
		case FM_CONTROL:
			menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("Control"));
			break;
		case FM_BUTTON:
			menu.AppendMenu(MF_STRING, ID_AUX_FIT, _T("Image Fit"));
			break;
		case FM_OBJECT:
			{
				CString sMap = m_formItem->GetFormStr(m_selF, _T("str"));
				if (!sMap.IsEmpty() && sMap.GetLength() == 8)
				{
					menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("Open Map \"") + sMap + _T("\""));
					if (AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ISOFFLINE))
						menu.EnableMenuItem(ID_AUX_EXTID, MF_GRAYED);
				}
			}
			break;
		}
	}

	//////////////////////////////////////////////
	// Layout menu
	Layoutmenu.AppendMenu(MF_STRING, ID_LAYOUT_ORIGINAL, _T("Original Form"));
	Layoutmenu.AppendMenu(MF_SEPARATOR);
	Layoutmenu.AppendMenu(MF_STRING, ID_LAYOUT_EDITOR, _T("Layout Editor"));
	Layoutmenu.AppendMenu(MF_SEPARATOR);
	// Layout 존재 여부 체크
	int idx = 0;
	m_arLayout.RemoveAll();
	if (m_mapH->maplayout && m_mapH->maplayout->GetCount() > 0)
	{
		CString sIndex, sData;
		CStringArray sArr;

		for (POSITION pos = m_mapH->maplayout->GetStartPosition(); pos;)
		{
			m_mapH->maplayout->GetNextAssoc(pos, sIndex, sData);
			if (!sIndex.CompareNoCase("-1"))
				continue;

			sArr.Add(sData);
			m_arLayout.Add(sIndex);
		}

		for (int ii = sArr.GetCount() - 1; ii >= 0; ii--)
		{
			sData = sArr.GetAt(ii);

			if (m_sPreLayoutName.CompareNoCase(sData) == 0)
				Layoutmenu.AppendMenu(MF_STRING | MF_CHECKED, LAYOUTMENU + ii, sArr.GetAt(ii));
			else 
				Layoutmenu.AppendMenu(MF_STRING, LAYOUTMENU + ii, sArr.GetAt(ii));
		}
	}
	
	if (!(m_selF >= 0 && (formR->kind == FM_GRIDEX || formR->kind == FM_TABLE || formR->kind == FM_GRID) && !formR->group))
		menu.EnableMenuItem(ID_AUX_COLUMNEDIT, MF_GRAYED);
	
	if (m_selF == -1)
	{
		menu.EnableMenuItem(ID_AUX_TOFIRST, MF_GRAYED);
		menu.EnableMenuItem(ID_AUX_TOEND, MF_GRAYED);
	}
	else if (formR->kind == FM_TAB)
	{
		menu.EnableMenuItem(ID_AUX_TOFIRST, MF_GRAYED);
	}

	if (!(m_selF >= 0 && formR->group))
		submenu.EnableMenuItem(ID_AUX_RELEASEGROUP, MF_GRAYED);
	if (!(m_selF >= 0 && m_block->count > 1 && formR->group == 0))
		submenu.EnableMenuItem(ID_AUX_SETGROUP, MF_GRAYED);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

bool CChildView::selectable(CPoint point)
{
	if (m_selF < 0)
		return false;

	struct _formR *formR = m_formItem->GetForm(m_selF);

	if (formR->kind == FM_TAB)
	{
		CPoint	tPt;
		bool	rtVal = false;
		CRect	tRc = getFormRect(m_selF);
		int	nID = ID_TOOL_TAB;
		
		((CTlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_fN = m_selF;
		((CTlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_dc = GetDC();

		if (nID == ID_TOOL_TAB && ((CTlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->Selectable(point, tRc, tPt) >= 0)
			rtVal = true;
		
		ReleaseDC(((CTlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_dc);
		((CTlTab*)m_toolOBs.GetAt(nID - ID_TOOL_BASE))->m_dc = NULL;
		return rtVal;
	}

	if (formR->auxR == (char *)0)
		return false;

	CRect	tRc, rc;
	int	ii, width, height, line, multi = 0, drawRows, headHeight;
	struct	_repR*	repR;

	tRc.SetRect(formR->left, formR->top, formR->right, formR->bottom);
	width = tRc.left;
	repR = (struct _repR *)formR->auxR;
	CCellItem *cellItem = new CCellItem(repR);

	switch (formR->kind)
	{
	case FM_GRID:
	case FM_GRIDEX:
		line = 0;

		for (ii = 0; ii < repR->cols; ii++)
		{
			if (formR->attr2 & GO_MULTI)
				multi = max(multi, (int)cellItem->GetCell(ii)->lineN);
		}
		multi++;

		for (ii = 0; ii < repR->cols; ii++)
		{
			struct _cellR *cellR = cellItem->GetCell(ii);
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

			height = tRc.top + line * headHeight;
			rc.SetRect(width, height, width + cellR->width, headHeight + height);

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
				tRc2.left  = tRc.right - szSCROLL;
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

		struct _cellR	*cellR = cellItem->GetCell(0);
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

void CChildView::sendToFirst()
{
	int	count = m_formItem->GetCount();

	if (m_selF == count - 1 || m_selF < 0)
		return;

	m_formItem->MoveForm(m_selF, count);
	m_selF = count - 1;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	Invalidate(FALSE);
}

void CChildView::sendToEnd()
{
	if (m_selF <= 0)
		return;

	m_formItem->MoveForm(m_selF, 0);
	m_selF = 0;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	Invalidate(FALSE);
}

LRESULT CChildView::OnNcHitTest(CPoint point)
{
	if (m_pEmulateWnd || m_bEmulRun)
		return 0;

	return CWnd::OnNcHitTest(point);
}

void CChildView::DefaultLayout()
{
	CMapStringToString* map = m_mapH->maplayout;

	if (!map || map->GetCount() < 1)
		return;

	if (m_selF == -1)
		return;

	struct	_formR*	formR = m_formItem->GetForm(m_selF);
	CString sIndex, sData;	
	for (POSITION pos = map->GetStartPosition(); pos;)
	{
		struct _Layout* layout = new _Layout;
		ZeroMemory(layout, sizeof(_Layout));	

		map->GetNextAssoc(pos, sIndex, sData);

		if (formR->layout->GetCount() > 0)
		{
			BOOL valid = formR->layout->Lookup(sData, (void*&)layout);
			if (valid && !layout->bfirst)
				continue;
		}
		layout->left = formR->left;
		layout->right = formR->right;
		layout->top = formR->top;
		layout->bottom = formR->bottom;
		layout->bfirst = true;
		layout->properties = 0x00;
		if (formR->properties & PR_VISIBLE)
			layout->properties = formR->properties;
		
		formR->layout->SetAt(sData, layout);
	}
}

