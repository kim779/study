
// axisBuilderView.cpp : CChildView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "axisBuilder.h"
#endif

#include "ChildView.h"
#include "ChildFrm.h"
#include "VBScriptEdit.h"
#include "emulateWnd.h"

#include "symbol.h"

#include "h/mapform.h"
#include "h/mainvar.h"
#include "../h/axisfire.h"
#include "../dll/lib/axislib.h"

#include "../h/msg.h"
#include "../h/ObjectEvent.h"
#include "awTool/awFormCtrl.h"
#include "awTool/awObjectLoad.h"
#include "../h/fmObject.h"
#include "awTool/fmTracker.h"
#include "awTool/awAlignBar.h"

#include "awDlg/attrDlg.h"
#include "awDlg/controlDlg.h"
#include "awDlg/gridColumn.h"
#include "awDlg/tabPage.h"

#include "awWcc/libWcc.h"
#include "h\seq.H"
#include "h/color.H"
#include "LayoutEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	CURSORNWSE	1
#define CURSORNESW	2
#define	CURSORNS	3
#define	CURSORWE	4

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
	ON_MESSAGE(WM_TRANSFER, &CChildView::OnTransfer)
	ON_MESSAGE(WM_OBJMSG, &CChildView::OnObjMsg)
	ON_COMMAND_RANGE(ID_AUX_TOFIRST, ID_AUX_EXTID, &CChildView::OnViewCommand)
	ON_COMMAND_RANGE(ID_LAYOUT_ORIGINAL, ID_LAYOUT_EDITOR, &CChildView::OnLayoutCommand)
	ON_COMMAND_RANGE(LAYOUTMENU, LAYOUTMENU + LAYOUTCOUNT, &CChildView::OnLayoutModeCommand)
END_MESSAGE_MAP()

// CChildView 생성/소멸

CChildView::CChildView()
{
	//m_formObArry.RemoveAll();
	m_FmCtrl = NULL;
	m_FmAlignBar = NULL;
	m_selF      = -1;
	m_stdF      = -1;
	m_move      = false;
	m_drawing   = false;
	m_selected  = false;
	m_bLBtnDown = false;
	m_pBitmap   = NULL;
	m_nSeqTabOdr = 0;
	m_nIoOdr    = m_nSeqIoOdr = 0;
	m_bRedraw   = true;
	m_bLayoutMode = false;

	m_mapH      = (class mapForm   *) NULL;
	m_pSCEdit   = NULL;
	m_pEmulateWnd  = NULL;

	m_nGap = (int)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGAP, 0);

	m_sImgPath = "";
	m_hImgBack = NULL;
	m_bBKLine = false;
	m_iBKLineIdx = -1;
}

CChildView::~CChildView()
{
	//m_formObArry.RemoveAll();

	if (m_FmCtrl)
		delete m_FmCtrl;

	if(m_FmAlignBar)
		delete m_FmAlignBar;

	if (m_pSCEdit)
		delete m_pSCEdit;

	CObjectLoad* FormLoad = NULL;
	for (size_t ii = 0; ii < (size_t)m_ArryFormObj.GetCount(); ii++)
	{
		FormLoad = (CObjectLoad*)m_ArryFormObj.GetAt(ii);
		delete FormLoad;
	}
	m_ArryFormObj.RemoveAll();

	if (m_pBitmap)
		delete m_pBitmap;
	if (m_hImgBack)
		DeleteObject(m_hImgBack);
	m_aryBKLine.RemoveAll();
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
	TRACE(_T("OnDraw\n"));
//	OnPaint();
}

BOOL CChildView::PreTranslateMessage(MSG* pMsg)
{
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

	if (m_pEmulateWnd)
	{
		pWizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4), m_emulateKey);
		if (m_pEmulateWnd)
			m_pEmulateWnd->PostMessage(WM_CLOSE);

		m_pEmulateWnd = NULL;
	}

	CWnd::OnDestroy();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
//	return CWnd::OnEraseBkgnd(pDC);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_mapH->mapK == MK_PROCEDURES)
	{
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
	if (m_selTool == _T("Pointer"))
  	{
		CformProp* formProp = (m_selF >= 0) ? ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp() : NULL;
		switch (nChar)
		{
		case VK_TAB:
			if (m_mapH->forms.GetCount() > 0)
			{
				//oldF = m_selF;
				//for (;;)
				//{
				//	m_selF = (++m_selF) % m_formItem->GetCount();
				//	if (m_formItem->GetForm(m_selF)->group)
				//		m_selF = m_formItem->GetIdxOfPtr(m_formItem->GetForm(m_selF)->group);
				//	if (m_formItem->GetForm(m_selF)->tabN && m_formItem->GetForm(m_selF)->tabN->checked != m_formItem->GetForm(m_selF)->pageN)
				//	{
				//		m_formItem->GetForm(m_selF)->tabN->checked = m_formItem->GetForm(m_selF)->pageN;
				//		InvalidateRect2(m_formItem->GetIdxOfPtr(m_formItem->GetForm(m_selF)->tabN));
				//	}
				//	
				//	if (!m_formItem->GetFormStr(m_selF, _T("name")).IsEmpty())
				//		break;
				//}

				CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_selF);
				CformProp*	Prop = (CformProp*)ObjLoad->getProp();
				if (Prop->group)
				{
					int cnt = 0;
					int max = m_mapH->forms.GetCount();
					do
					{
						m_selF = (++m_selF) % m_mapH->forms.GetCount();
						CObjectLoad*	subObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_selF);
						CformProp*	subProp = (CformProp*)subObjLoad->getProp();
						if (Prop->group != subProp->group)
						{
							break;
						}
						cnt++;
					} while (cnt < max);
					
				}
				else
				{
					m_selF = (++m_selF) % m_mapH->forms.GetCount();
				}
				m_FmCtrl->SelObj(m_selF);
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
			}
			break;

		case VK_DELETE:
			if (m_bBKLine && m_iBKLineIdx > -1)
			{
				delBKLine();
			}
			else
			{
				if ((::GetKeyState(VK_SHIFT) & 0x8000) && m_selF >= 0
					&& formProp->kind == fmTAB
					&& formProp->keyValues > 0)
				{
					deleteSelPage(m_selF);
					break;
				}
				this->ObjectRemove();
			}
			break;

		//case VK_F8:
		//	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETFOCUSPRO, 0);
		//	break;

	//	case VK_F9:
	//		break;

		//case VK_F10:
		//	SetFocus();
		//	break;

		case VK_ESCAPE:
			m_FmCtrl->SelObj(-1);
			break;

		case VK_UP:
			if (m_bBKLine && m_iBKLineIdx > -1)
			{
				moveBKLine(VK_UP);
			}
			else
			{
				keyArrowUp();
			}
			break;

		case VK_DOWN:
			if (m_bBKLine && m_iBKLineIdx > -1)
			{
				moveBKLine(VK_DOWN);
			}
			else
			{
				keyArrowDown();
			}
			break;
			
		case VK_LEFT:
			if (m_bBKLine && m_iBKLineIdx > -1)
			{
				moveBKLine(VK_LEFT);
			}
			else
			{
				keyArrowLeft();
			}
			break;
			
		case VK_RIGHT:
			if (m_bBKLine && m_iBKLineIdx > -1)
			{
				moveBKLine(VK_RIGHT);
			}
			else
			{
				keyArrowRight();
			}
			break;
		default:
			break;
		}
	}
	
//	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_VIEW_SCRIPT, 0);
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( ((CChildFrame*)GetParent())->CheckOrder(point) )
		return ;
	if (m_mapH->mapK == MK_PROCEDURES)
	{
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}

	SetFocus();

	m_dragPnt = point;
	if (!(nFlags & MK_CONTROL) && m_selTool != _T("Pointer"))
	{
		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}
	m_bLBtnDown = true;
	if (m_bBKLine)
	{
		CRect rc(point.x,point.y,point.x,point.y);
		m_aryBKLine.Add(rc);
	}
	else if (!m_drawing)
	{
		CformProp* formProp = NULL;
		if (m_selF >= 0
			&& (((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp()->kind == fmGRID
			|| ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp()->kind == fmGRIDEX
			|| ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp()->kind == fmTABLE))
		{
			((CObjectLoad *)m_mapH->forms.GetAt(m_selF))->getProp()->CellSelected = -1;
		}
		if (m_selTool != _T("Pointer"))
		{
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		}
		if (m_selTool == _T("LABEL"))
		{
			formProp = new CformProp();
			formProp->Init(fmLABEL);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("BOX"))
		{
			formProp = new CformProp();
			formProp->Init(fmBOX);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->size = 1;
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("GROUP"))
		{
			formProp = new CformProp();
			formProp->Init(fmGROUP);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("PANEL"))
		{
			formProp = new CformProp();
			formProp->Init(fmIMVIEW);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("BUTTON"))
		{
			formProp = new CformProp();
			formProp->Init(fmBUTTON);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("RADIO"))
		{
			formProp = new CformProp();
			formProp->Init(fmRADIO);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("CHECK"))
		{
			formProp = new CformProp();
			formProp->Init(fmCHECK);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("EDIT"))
		{
			formProp = new CformProp();
			formProp->Init(fmEDIT);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("COMBO"))
		{
			formProp = new CformProp();
			formProp->Init(fmCOMBO);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("OUTPUT"))
		{
			formProp = new CformProp();
			formProp->Init(fmOUTPUT);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("MEMO"))
		{
			formProp = new CformProp();
			formProp->Init(fmMEMO);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("GRID"))
		{
			formProp = new CformProp();
			formProp->Init(fmGRID);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);

			formProp->cellProperties = new CObArray();
		}
		else if (m_selTool == _T("GRIDEX"))
		{
			formProp = new CformProp();
			formProp->Init(fmGRIDEX);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);

			formProp->cellProperties = new CObArray();
		}
		else if (m_selTool == _T("TABLE"))
		{
			formProp = new CformProp();
			formProp->Init(fmTABLE);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
			formProp->auxAttributes |= gaMULTI;

			formProp->cellProperties = new CObArray();
		}
		else if (m_selTool == _T("TREEVIEW"))
		{
			formProp = new CformProp();
			formProp->Init(fmTREE);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("OBJECT"))
		{
			formProp = new CformProp();
			formProp->Init(fmFORM);
			formProp->name = GetInitSymbol("", formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("USRTAB"))
		{
			formProp = new CformProp();
			formProp->Init(fmTAB);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("BROWSER"))
		{
			formProp = new CformProp();
			formProp->Init(fmBROWSER);
			formProp->name = GetInitSymbol("",formProp->kind);
			formProp->rect.OffsetRect(point.x, point.y);
		}
		else if (m_selTool == _T("CONTROL"))
		{
			formProp = new CformProp();
			formProp->Init(fmOBJECT);
			formProp->name = GetInitSymbol("", formProp->kind);
			formProp->size = 1;
			formProp->rect.OffsetRect(point.x, point.y);
			CString strN = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCTMOBJ, 0);
			strN.MakeReverse();
			strN = strN.Mid(strN.Find(".") + 1);
			formProp->strings = strN.MakeReverse();
			formProp->iCtmIdx = 0;
			CformProp* Prop = NULL;
			for (int ii = 0; m_mapH->forms.GetCount() > ii; ii++)
			{
				Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
				if (Prop->iCtmIdx == formProp->iCtmIdx)
				{
					formProp->iCtmIdx++;
					ii = 0;
				}
			}
		}
		if (formProp)
		{
			CObjectLoad* ObjLoad = new CObjectLoad(formProp, m_piForm, this);
			if (!ObjLoad->LoadLib(formProp->kind))
			{
				delete ObjLoad;
				return;
			}
			m_mapH->forms.Add(ObjLoad);

			if (isValidTabOrder(formProp))
				formProp->index = ++m_nSeqTabOdr;
			if (isValidIOOrder(formProp))
				formProp->iorder = ++m_nSeqIoOdr;

		}
		m_drawing = true;
		m_FmCtrl->MouseDown(point);
	}
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	int	msgId = ID_USR_REFRESHCTRL;

	if (!(nFlags & MK_CONTROL) && m_selTool != _T("Pointer"))
	{
		point.x = ((int)(point.x / m_nGap)) * m_nGap;
		point.y = ((int)(point.y / m_nGap)) * m_nGap;
	}
	
	if (m_bBKLine)
	{
		
		CRect* rc = &m_aryBKLine.GetAt(m_aryBKLine.GetCount() - 1);
		CRect aryRC, checkRC, UpdateRC;
		CRect rectInter;

		if (m_iBKLineIdx > -1)
		{
			UpdateRC = m_aryBKLine.GetAt(m_iBKLineIdx);
			m_iBKLineIdx = -1;
			UpdateRC.InflateRect(1, 1);
			InvalidateRect(UpdateRC);
		}

		rc->right = point.x;
		rc->bottom = point.y;
		if (rc->left == point.x && rc->top == point.y)
		{
			checkRC.CopyRect(*rc);
			checkRC.InflateRect(5, 5);
			m_aryBKLine.RemoveAt(m_aryBKLine.GetCount() - 1);
			for (int ii = 0; ii < m_aryBKLine.GetCount(); ii++)
			{
				aryRC = &m_aryBKLine.GetAt(ii);
				aryRC.InflateRect(1, 1);
				if (rectInter.IntersectRect(aryRC, checkRC))
				{
					//m_aryBKLine.RemoveAt(ii);
					m_iBKLineIdx = ii;
					break;
				}
			}
			InvalidateRect(aryRC);
		}
		else
		{
			UnSelBKLine();
			CRect viewRC, UpdateRC;
			m_iBKLineIdx = m_aryBKLine.GetCount() - 1;
			
			GetClientRect(viewRC);
			if (rc->Width() > rc->Height())
			{
				rc->left = 0;
				rc->right = viewRC.right;
				rc->bottom = rc->top;
			}
			else
			{
				rc->top = 0;
				rc->bottom = viewRC.bottom;
				rc->right = rc->left;
			}
			UpdateRC.CopyRect(*rc);
			UpdateRC.InflateRect(1, 1);
			InvalidateRect(UpdateRC);
		}

		UpdateLineText();
	}
	else if (m_drawing)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 0), NULL);
		m_mapH->modified = true;
		if (m_selTool != _T("Pointer"))
		{
			m_selF = m_mapH->forms.GetUpperBound();

			CObjectLoad* ObjectLoad;
			ObjectLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_mapH->forms.GetCount() - 1);
			if ((ObjectLoad->getProp()->rect.left != point.x) && (ObjectLoad->getProp()->rect.top != point.y))
				ObjectLoad->SetLastPoint(point);
			ObjectLoad->LoadProc();
		}
		else
			msgId = ID_USR_SELCTRLTOPRO;

		m_FmCtrl->MouseUp();
		m_move    = false;
		m_drawing = false;

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, ID_TOOL_POINTER);
		m_selTool = _T("Pointer");
		m_FmCtrl->SetName(m_selTool);

		m_selected = false;
		m_bLBtnDown = false;
		AfxGetMainWnd()->SendMessage(WM_USER, msgId, (msgId == ID_USR_REFRESHCTRL ? TRUE : FALSE));
	}

//	CWnd::OnLButtonUp(nFlags, point);
}

int CChildView::GetHitCursor()
{
	return m_nCursor;
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;
	
	if (m_drawing)
	{
		if (!(nFlags & MK_CONTROL) && m_selTool != _T("Pointer"))
		{
			point.x = ((int)(point.x / m_nGap)) * m_nGap;
			point.y = ((int)(point.y / m_nGap)) * m_nGap;
		}

		m_FmCtrl->MouseMove(point);
	}
	else
	{
		m_nCursor = 0;
		m_move = false;
		m_bSelect = false;
		CArray <CfmTracker*, CfmTracker*> *fmTkArry = m_FmCtrl->GetTkArray();
		int iCnt = fmTkArry->GetCount();
		if (iCnt == 1)
		{
			CfmTracker* fmTracker = fmTkArry->GetAt(iCnt-1);
			for (size_t jj = 0; jj < 8; jj++)
			{
				if (fmTracker->m_rc[jj].PtInRect(point))
				{
					fmTracker->SetHit(jj);
					switch (jj)
					{
					case 0:
					case 4:
						m_nCursor = CURSORNWSE;
						break;
					case 1:
					case 5:
						m_nCursor = CURSORNS;
						break;
					case 2:
					case 6:
						m_nCursor = CURSORNESW;
						break;
					case 3:
					case 7:
						m_nCursor = CURSORWE;
						break;
					}
				}
			}
		}
  	}

//	CWnd::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	bool bSel = false;
	
	CObjectLoad*	ObjLoad = NULL;
	CformProp*	Prop = NULL;
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii);
		Prop = ObjLoad->getProp();
		if (Prop->group)
		{
			if (Prop->groupRc.PtInRect(point))
			{
				if(Prop->group == Prop)
				{
					m_selF = ii;
					SelObj(m_selF);
				}
				bSel = true;
				break;
			}
		}
		else
		{
			if (Prop->rect.PtInRect(point))
			{
				bSel = true;
				if(Prop->block) 
				{
					m_selF = ii;
					break;
				}
				else 
				{
					m_selF = ii;
					SelObj(m_selF);
					break;
				}
			}
		}
	}

	if(!bSel)
	{
		m_selF = -1;
		SelObj(m_selF);
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
	m_RBDPnt = point;

//	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
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
	ModifyStyle(0, WS_CLIPCHILDREN); // turn on WS_CLIPCHILDREN
	if (nType == SIZE_MINIMIZED || m_new || m_mapH == (class mapForm *) NULL)
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
		//Invalidate();
		m_mapH->modified = true;
	}

	//Invalidate(FALSE);

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
	UpdateWindow();
	ModifyStyle(WS_CLIPCHILDREN, 0); // turn off WS_CLIPCHILDREN        
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TM_DRAG:
		KillTimer(nIDEvent);
		m_FmCtrl->MouseDrag();
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

LRESULT CChildView::OnTransfer(WPARAM wp, LPARAM lp) // 오브젝트에서 보낸 메세지
{
	int idata = 0;

	switch (HIWORD(wp)) //폼 오브젝트 안에있는 오브젝트들 후킹메세지
	{
	case fmFORM:
	{
		CPoint pt;
		switch (LOWORD(wp))
		{
		case WM_LBUTTONDOWN:
			pt.x = LOWORD(lp);
			pt.y = HIWORD(lp);
			ScreenToClient(&pt);
			OnLButtonDown(0, pt);
			break;
		case WM_LBUTTONUP:
			pt.x = LOWORD(lp);
			pt.y = HIWORD(lp);
			this->ScreenToClient(&pt);
			OnLButtonUp(0, pt);
			break;
		case WM_MOUSEMOVE:
			pt.x = LOWORD(lp);
			pt.y = HIWORD(lp);
			this->ScreenToClient(&pt);
			OnMouseMove(0, pt);
			break;
		case WM_RBUTTONDOWN:
			pt.x = LOWORD(lp);
			pt.y = HIWORD(lp);
			this->ScreenToClient(&pt);
			OnRButtonDown(0, pt);
			break;
		case WM_RBUTTONUP:
			pt.x = LOWORD(lp);
			pt.y = HIWORD(lp);
			this->ScreenToClient(&pt);
			OnRButtonUp(0, pt);
			break;
		}
	}
	case fmGRID:
	{
		switch (LOWORD(wp))
		{
		case MHOOK_EV:
		{
			if (m_selF >= 0)
			{
				CObjectLoad* ObjLoad = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF));
				ObjLoad->SetHooking((LOWORD(lp) != false));
			}
			break;
		}
		}
	}
	return 0;
	}

	switch (LOWORD(wp))
	{
	case LCLICK_EV:
		idata = LOWORD(lp);
		if (m_mapH->forms.GetCount() > m_selF)
		{
			CformProp* Prop = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();
			if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
			{
				if (Prop->cellProperties->GetCount() > idata)
				{
					Prop->CellSelected = idata;
					AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, FALSE);
				}
			}
			else // fmTAB
			{
				Prop->checked = idata;
				AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REDRAWCTRL, FALSE), m_selF);
			}
		}
		break;
	case LDCLICK_EV:
		break;
	case RCLICK_EV:
		break;
	}
	return 0;
}
LRESULT CChildView::OnMessage(WPARAM wp, LPARAM lp)
{
	CString	tmps;
	switch (LOWORD(wp))
	{
	case getCOLOR:
		return GetRGB((int)lp);
	case getROOT:
		return (LONG_PTR)getRoot();
	}

	return 0;
}

LRESULT CChildView::OnObjMsg(WPARAM wp, LPARAM lp) // 오브젝트 이벤트 후킹
{
	CPoint pt;
	switch (LOWORD(wp))
	{
	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnLButtonDown(0, pt);
		break;
	case WM_LBUTTONUP:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnLButtonUp(0, pt);
		break;
	case WM_MOUSEMOVE:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnMouseMove(0, pt);
		break;
	case WM_RBUTTONDOWN:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnRButtonDown(0, pt);
		break;
	case WM_RBUTTONUP:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnRButtonUp(0, pt);
		break;
	}

	return 0;
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
		if (m_selF >= 0 && m_FmCtrl->GetSelCount() <= 1)
		{
			if (((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp()->kind != fmGRID 
				&& ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp()->kind != fmGRIDEX
				&& ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp()->kind != fmTABLE)
				break;

			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)TRUE);
			CGridColumn	dlg(this, m_mapH, m_selF);
			dlg.DoModal();
			AfxGetMainWnd()->SendMessage(WM_USER,MAKEWPARAM(ID_USR_MODAL, 0), (LPARAM)FALSE);
			InvalidateRect2(m_selF);
			//Invalidate();
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

void CChildView::Init(class mapForm* mapForm, CAxisDraw* draw, CAxisPalette* palette, CiForm* piForm, CString dir)
{
	m_mapH  = mapForm;

	m_draw  = draw;
	m_palette = palette;
	m_piForm = piForm;
	
	ChangeKind();

	m_FmCtrl = new CFmCtrl(this, m_draw);
	m_FmAlignBar = new CFmAlignBar(this, m_FmCtrl);
	m_selTool = _T("Pointer");
}

void CChildView::DrawForm(int cmdID)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	if (ID_TOOL_BASE != cmdID)
		m_selected = true;
	else
		m_selected = false;

	switch (cmdID)
	{
	case ID_TOOL_POINTER:
		m_selTool = _T("Pointer");
		break;
	case ID_TOOL_LABEL:
		m_selTool = _T("LABEL");
		break;
	case ID_TOOL_BOX:
		m_selTool = _T("BOX");
		break;
	case ID_TOOL_GROUP:
		m_selTool = _T("GROUP");
		break;
	case ID_TOOL_PANEL:
		m_selTool = _T("PANEL");
		break;
	case ID_TOOL_PUSH:
		m_selTool = _T("BUTTON");
		break;
	case ID_TOOL_RADIO:
		m_selTool = _T("RADIO");
		break;
	case ID_TOOL_CHECK:
		m_selTool = _T("CHECK");
		break;
	case ID_TOOL_EDIT:
		m_selTool = _T("EDIT");
		break;
	case ID_TOOL_COMBO:
		m_selTool = _T("COMBO");
		break;
	case ID_TOOL_OUT:
		m_selTool = _T("OUTPUT");
		break;
	case ID_TOOL_MEMO:
		m_selTool = _T("MEMO");
		break;
	case ID_TOOL_GRID:
		m_selTool = _T("GRID");
		break;
	case ID_TOOL_GRIDEX:
		m_selTool = _T("GRIDEX");
		break;
	case ID_TOOL_TABLE:
		m_selTool = _T("TABLE");
		break;
	case ID_TOOL_TREE:
		m_selTool = _T("TREEVIEW");
		break;
	case ID_TOOL_FORM:
		m_selTool = _T("OBJECT");
		break;
	case ID_TOOL_TAB:
		m_selTool = _T("USRTAB");
		break;
	case ID_TOOL_BROWSER:
		m_selTool = _T("BROWSER");
		break;
	case ID_TOOL_OBJECT:
		m_selTool = _T("CONTROL");
		break;
	default:
		m_selTool = _T("Pointer");
		break;
	}

	if (cmdID != ID_TOOL_OBJECT) AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_TBPRESSBTN, cmdID);
	m_FmCtrl->SetName(m_selTool);
}

void CChildView::AlignForms(int cmdID)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	if ((cmdID >= ID_AL_LEFT && cmdID <= ID_BLK_VCENTER) && m_selF < 0)
		return;

	CRect	wRc, pRc;
	if(m_selF < 0)
		return ;
	CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();

	GetClientRect(pRc);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	switch (cmdID)
	{
	case ID_AL_LEFT:
		m_FmAlignBar->AlignLeft();
		break;

	case ID_AL_TOP:
		m_FmAlignBar->AlignTop();
		break;

	case ID_AL_RIGHT:
		m_FmAlignBar->AlignRight();
		break;

	case ID_AL_BOTTOM:
		m_FmAlignBar->AlignBottom();
		break;

	case ID_BLK_HCENTER:
		m_FmAlignBar->AlignHCenter();
		break;

	case ID_BLK_VCENTER:
		m_FmAlignBar->AlignVCenter();
		break;

	case ID_WIN_HCENTER:
		m_FmAlignBar->AlignWHCenter();
		break;

	case ID_WIN_VCENTER:
		m_FmAlignBar->AlignWVCenter();
		break;

	case ID_SPC_HEQUAL:
		m_FmAlignBar->QualHSpace();
		break;

	case ID_SPC_VEQUAL:
		m_FmAlignBar->QualVSpace();
		break;

	case ID_SZ_HEQUAL:
		m_FmAlignBar->QualHSize();
		break;

	case ID_SZ_VEQUAL:
		m_FmAlignBar->QualVSize();
		break;
	}
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
}

void CChildView::ObejctRemoveAll()
{
	while (m_mapH->forms.GetCount() > 0)
	{
		deleteForm(0);
	}
	m_selF = -1;
	SelObj(-1);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

void CChildView::ObjectRemove()
{
	if (m_selF < 0)
		return;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);

	//CRect	pRc;
	if (m_FmCtrl->GetSelCount() > 1)
	{
		CformProp* Prop = NULL;
		for (int ii = 0; m_mapH->forms.GetCount() > ii; ii++)
		{
			Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
			if (Prop->block == true)
			{
				deleteForm(ii);
				ii = -1;
			}
		}
	}
	else
	{
		deleteForm(m_selF);
	}

	SortIoOrder();
	m_selF = -1;
	SelObj(-1);
	m_FmCtrl->InvalidateObject();
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
}

void CChildView::OnTRLayoutDrop(int kind, CPoint pt)
{

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
				
				//if (((CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(grid))->getProp())->kind == fmTAB)
				//	addForm(is_default, symbol, pt);
				//else
				{
					addColumn(is_default, grid, symbol);
					((CObjectLoad*)m_mapH->forms.GetAt(grid))->getObject()->Reload();
				}
			}
			else
			{
				addForm(is_default, symbol, pt, NULL, ii);
				pt.y += symbol->m_datasz.cy;
			}
		}
		m_mapH->modified = true;
	}
	else if (kind == 1)	// template drop
	{
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_LOADTEMPLATE, 0), MAKELPARAM(pt.x, pt.y));
		m_mapH->modified = true;
	}
}

void CChildView::Emulate()
{
	CWnd*	pWizard = (CWnd*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETWIZARD);
	if (!pWizard)
		return;

	if (m_pEmulateWnd)
	{
		pWizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4), m_emulateKey);

		if (m_pEmulateWnd)
			m_pEmulateWnd->PostMessage(WM_CLOSE);

		m_pEmulateWnd = NULL;
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETCAPTION);
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
		return;
	}

	CRect	rect;
	CString	fname;
	CFileFind finder;

	GetClientRect(&rect);
	wccSetTempPath(fname, m_mapH->mapN, false);
	
	if (finder.FindFile(fname))
	{
		long	size, wndType;
				
		wndType = vtypeERR;
		pWizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void*)&wndType, (BYTE*)(VTS_BSTR VTS_I4), m_mapH->mapN, &size);

		if (wndType == vtypeERR)
		{
			AfxMessageBox(_T("Loading MapInfo Error!"));
			if (m_pEmulateWnd)
			{
				m_pEmulateWnd->PostMessage(WM_CLOSE);
				m_pEmulateWnd = NULL;
			}
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
			return;
		}
		
		m_pEmulateWnd = new CEmulateWnd();
		m_pEmulateWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, 0);
		pWizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void*)&m_emulateKey, (BYTE*)(VTS_I8 VTS_I4 VTS_I4), (LONG_PTR)m_pEmulateWnd, wndType, 0);
		
		if (m_emulateKey)
		{
			BOOL	bRetval;
			pWizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void*)&bRetval, (BYTE*)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), m_emulateKey, m_mapH->mapN, size, false);
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, FALSE);
			m_pEmulateWnd->RedrawWindow();
			m_pEmulateWnd->SetFocus();
		}
		else
		{
			AfxMessageBox(_T("Creating Map Key Error!"));
			m_pEmulateWnd->PostMessage(WM_CLOSE);
			m_pEmulateWnd = NULL;
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
		}
	}
}

CString CChildView::GetInitSymbol(CString slist, char kind)
{
	CString sym;
	int	initN, fN = m_mapH->forms.GetCount() - 1;
	if (slist.IsEmpty())
	{
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			slist += ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp()->name;
			slist += _T('\t');
		}
	}

	initN = 0;
	char objKind = kind;
	if(kind == NULL && fN > -1)
		objKind = ((CObjectLoad*)m_mapH->forms.GetAt(fN))->getProp()->kind;
	for (;;)
	{
		sym.Format(_T("%s%d"), GetControlName(objKind, 0), initN);
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
		return _T("FORM");
	case fmTAB:
		return _T("TAB");
	case fmBROWSER:
		return _T("BROWSER");
	case fmOBJECT:
		return _T("CONTROL");
	}
	return _T("SYMBOL");
}

bool CChildView::IsInTab(CPoint pnt, int &formN, int &selectedN)
{
	return false;
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

void CChildView::InvalidateRect2(int idx, int iFormLoad)
{
	if (idx >= 0)
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
		CfmObject* obj = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getObject();

		if (m_FmCtrl->GetSelCount() >= 2)
		{
			CformProp* form = NULL;
			CfmObject* obj = NULL;
			CWnd* Wnd = NULL;
			for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
			{
				form = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();				
				if (form->block)
				{
					obj = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getObject();
					Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getWnd();
					obj->Reload();
					InvalidateRect(form->rect, TRUE);
					Wnd->MoveWindow(form->rect);
				}
			}
			m_FmCtrl->ReDrawTracker();
		}
		else
		{
			CObjectLoad* ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
			CformProp* form = ObjLoad->getProp();
			CfmObject* obj = ObjLoad->getObject();
		
			CWnd* Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getWnd();

			if ((form->kind == fmGRID || form->kind == fmGRIDEX) && iFormLoad == GRIDSCROLL)
			{
				ShowVScroll(form);
			}

			obj->Reload();
			InvalidateRect(form->rect, TRUE);
			Wnd->MoveWindow(form->rect);
			m_FmCtrl->SetTracker(idx);

			if (form->kind == fmFORM && iFormLoad == FORMOBJECT)
			{
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_FORMLOAD, (LPARAM)ObjLoad);
			}
		}
		//m_FmCtrl->InvalidateObject();
	}
	else
	{
		if (iFormLoad == FORMVIEW)
			Invalidate();
		else
			m_FmCtrl->InvalidateObject();
	}
}

void CChildView::SetGap(int nGap)
{
	m_nGap = nGap;
	m_FmCtrl->SetGap(m_nGap);
}

void CChildView::ShowForm(bool show)
{
	CRect	tRc;

	m_showFORM = show;
	CformProp* Prop = NULL;
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		if (!(Prop->styles & stVISIBLE))
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

bool CChildView::isValidTabOrder(CformProp * Prop)
{
	switch (Prop->kind)
	{
	case fmEDIT:
	case fmBUTTON:
	case fmRADIO:
	case fmCHECK:
	case fmCOMBO:
	case fmFORM:
	case fmGRID:
	case fmGRIDEX:
	case fmMEMO:
		return true;
	case fmOBJECT:
		if (Prop->ioType != ioFREE)
			return true;
		break;
	}
	return false;
}

bool CChildView::isValidIOOrder(CformProp* Prop)
{
	switch (Prop->kind)
	{
	case fmEDIT:
	case fmOUTPUT:
	case fmCOMBO:
	case fmGRID:
	case fmGRIDEX:
	case fmTABLE:
	case fmFORM:
	case fmMEMO:
	case fmTREE:
		return true;
	case fmBROWSER:
		if (Prop->ioType == ioOUTPUT)
			return true;
		break;
	//case fmBUTTON:
	case fmCHECK:
	case fmRADIO:
		if (Prop->ioType != ioFREE)
			return true;
		break;
	case fmOBJECT:
		if (Prop->ioType != ioFREE)
			return true;
	}
	return false;
}

void CChildView::loadAutoList()
{
	int	nScriptID = 100;
	CMapStringToPtr mapFormID;

	mapFormID.SetAt("LABEL",   (void*)fmLABEL);
	mapFormID.SetAt("BOX",     (void*)fmBOX);
	mapFormID.SetAt("GROUP",   (void*)fmGROUP);
	mapFormID.SetAt("PANEL",   (void*)fmIMVIEW);
	mapFormID.SetAt("BUTTON",  (void*)fmBUTTON);
	mapFormID.SetAt("EDIT",    (void*)fmEDIT);
	mapFormID.SetAt("COMBO",   (void*)fmCOMBO);
	mapFormID.SetAt("OUTPUT",  (void*)fmOUTPUT);
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
			if (sLevel.CompareNoCase(_T("controls")))
				m_pSCEdit->m_publicStrMap.SetAt(tmps, (void*)FormNum);
		}
	}

	mapFormID.RemoveAll();
}

void CChildView::extMenu()
{
	if (m_selF < 0)
		return;

	CformProp* Prop = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();
	switch (Prop->kind)
	{
	case fmEDIT:
	case fmCOMBO:
	case fmOUTPUT:
		{
			CAttrDlg dlg(this, Prop->kind, Prop->charType, Prop->ioType, Prop->attributes, Prop->auxAttributes);
			if (dlg.DoModal() == IDOK)
			{
				Prop->attributes  = dlg.m_attributes;
				Prop->auxAttributes = dlg.m_auxAttributes;
			}
		}
		break;
	case fmOBJECT:
		{
			CControlDlg dlg(m_selF);
			
			dlg.m_ctrlName  = Prop->strings;
			//dlg.m_ctrlParam = m_formItem->GetFormStr(m_selF, _T("str2"));
			if (dlg.DoModal() == IDOK)
			{
				m_mapH->modified = true;
				Prop->strings = dlg.m_ctrlName;
				//m_formItem->SetFormStr(m_selF, _T("str2"), dlg.m_ctrlParam);
				//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, -1);
				AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);	// control event refresh
			}
		}
		break;
	case fmTAB:
		{
			CTabPage dlg(this, m_mapH, m_selF);
			
			if (dlg.DoModal() == IDOK)
			{
				//AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, -1);
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_selF);
			}
		}
		break;
	case fmFORM:
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SERVEROPEN, (LPARAM)(Prop->strings.operator LPCTSTR()));
		break;
	}
	Invalidate();
}

void CChildView::columnFit()
{
	if (m_selF < 0)
		return;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_selF);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();

	if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
	{
		int	nWidth, nVisibleCnt, nFormWidth;

		nWidth = nVisibleCnt = 0;
		nFormWidth = Prop->rect.right - Prop->rect.left - 4;
		if (Prop->auxAttributes & gaVSCR)
			nFormWidth -= szSCROLL;
		
		if (Prop->CellSelected == 0)
		{
			cellOb* CellProp = NULL;
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				if (CellProp->styles & stVISIBLE)
				{
					nWidth += CellProp->width;
					nVisibleCnt++;
				}
			}
			CellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->CellSelected);
			nWidth -= CellProp->width;
			CellProp->width = nFormWidth - nWidth;
			if (nFormWidth - nWidth < 0)
			{
				for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
				{
					CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
					nWidth = (nFormWidth * (ii + 1)) / nVisibleCnt - (nFormWidth * ii) / nVisibleCnt;
					CellProp->width = nWidth;
				}
			}
		}
		else
		{
			cellOb* CellProp = NULL;
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				if (CellProp->styles & stVISIBLE)
					nVisibleCnt++;
			}
			for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
			{
				CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				if (CellProp->styles & stVISIBLE)
				{
					nWidth = (nFormWidth * (ii + 1)) / nVisibleCnt - (nFormWidth * ii) / nVisibleCnt;
					CellProp->width = nWidth;
				}
			}
		}
		ShowVScroll(Prop);
		InvalidateRect2(m_selF);
	}
	else if (Prop->kind == fmBUTTON)
	{
		if (Prop->styles & (stIMAGE | stIMAGETEXT))
		{
			if (Prop->images)
			{
				CString	path, tmps = Prop->images;
				if (tmps.Find("\\") == -1)
					path.Format(_T("%s/%s/%s"), getRoot(), IMAGEDIR, tmps);
				else
					path = tmps;
				
				CBitmap* bmp;
				BITMAP	bm;
				bmp = m_draw->SetBitmap((char *)path.operator LPCTSTR());
				bmp->GetBitmap(&bm);
				CSize	sz(bm.bmWidth,bm.bmHeight);

				if (sz != CSize(0, 0))
				{
					Prop->rect.right  = Prop->rect.left + sz.cx + 1;
					Prop->rect.bottom = Prop->rect.top + sz.cy + 1;

					if (m_FmCtrl) m_FmCtrl->MoveObj(Prop->rect);
					
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
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();
	if (Prop->cellProperties == NULL)
		return sSymbol;


	for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
	{
		if (!((cellOb*)Prop->cellProperties->GetAt(ii))->name.IsEmpty())
		{
			slist += ((cellOb*)Prop->cellProperties->GetAt(ii))->name;
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
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();

	if (Prop->kind == fmGRID)
	{
		cellOb* cellProp = new cellOb();
		cellProp->paintColor = getDefaultValue(_T("GRID"), _T("DPRGB"), basINPUT);
		cellProp->textColor = getDefaultValue(_T("GRID"), _T("DTRGB"), 232);
		if (Prop->cellProperties->GetCount() > 0)
		{
			cellOb* lstCellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->cellProperties->GetCount() - 1);
			cellProp->headPaintColor = getDefaultValue(_T("GRID"), _T("HPRGB"),lstCellProp->headPaintColor);
			cellProp->headTextColor = getDefaultValue(_T("GRID"), _T("HTRGB"), lstCellProp->headTextColor);
			cellProp->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), lstCellProp->width);
		}
		else
		{
			cellProp->headPaintColor = getDefaultValue(_T("GRID"), _T("HPRGB"),basPRGB);
			cellProp->headTextColor = getDefaultValue(_T("GRID"), _T("HTRGB"), 232);
			cellProp->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), 80);
		}
		cellProp->size = 0;
		cellProp->charType = ctNUMERIC;
		cellProp->ioType = ioOUTPUT;
		cellProp->kind = ckNORMAL;
		cellProp->name = getItemSymbol(_T("Col"));
		if (Prop->cellProperties->GetCount() == 0)
			cellProp->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), 80);
		else
			cellProp->width = getDefaultValue(_T("GRID"), _T("COLWIDTH"), ((cellOb*)Prop->cellProperties->GetAt(0))->width);
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		
		Prop->cellProperties->Add(cellProp);
	}
	else if (Prop->kind == fmGRIDEX)
	{
		cellOb* cellProp = new cellOb();
		cellProp->paintColor = getDefaultValue(_T("GRIDEX"), _T("DPRGB"), basINPUT);
		cellProp->textColor = getDefaultValue(_T("GRIDEX"), _T("DTRGB"), 232);
		if (Prop->cellProperties->GetCount() > 0)
		{
			cellOb* lstCellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->cellProperties->GetCount() - 1);
			cellProp->headPaintColor = getDefaultValue(_T("GRIDEX"), _T("HPRGB"),lstCellProp->headPaintColor);
			cellProp->headTextColor = getDefaultValue(_T("GRIDEX"), _T("HTRGB"), lstCellProp->headTextColor);
			cellProp->width = getDefaultValue(_T("GRIDEX"), _T("COLWIDTH"), lstCellProp->width);
		}
		else
		{
			cellProp->headPaintColor = getDefaultValue(_T("GRIDEX"), _T("HPRGB"),basPRGB);
			cellProp->headTextColor = getDefaultValue(_T("GRIDEX"), _T("HTRGB"), 232);
			cellProp->width = getDefaultValue(_T("GRIDEX"), _T("COLWIDTH"), 80);
		}
		cellProp->size = 0;
		cellProp->charType = ctNUMERIC;
		cellProp->ioType = ioOUTPUT;
		cellProp->kind = ckNORMAL;
		cellProp->name = getItemSymbol(_T("Col"));
		if (Prop->cellProperties->GetCount() == 0)
			cellProp->width = getDefaultValue(_T("GRIDEX"), _T("COLWIDTH"), 80);
		else
			cellProp->width = getDefaultValue(_T("GRIDEX"), _T("COLWIDTH"), ((cellOb*)Prop->cellProperties->GetAt(0))->width);
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		
		Prop->cellProperties->Add(cellProp);
	}
	else
	{
		cellOb* cellProp = new cellOb();
		cellProp->paintColor = getDefaultValue(_T("TABLE"), _T("DPRGB"), basINPUT);
		cellProp->textColor = getDefaultValue(_T("TABLE"), _T("DTRGB"), 232);
		if (Prop->cellProperties->GetCount() > 0)
		{
			cellOb* lstCellProp = (cellOb*)Prop->cellProperties->GetAt(Prop->cellProperties->GetCount() - 1);
			cellProp->headPaintColor = getDefaultValue(_T("TABLE"), _T("HPRGB"), lstCellProp->headPaintColor);
			cellProp->headTextColor  = getDefaultValue(_T("TABLE"), _T("HTRGB"), lstCellProp->headTextColor);
			cellProp->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"),lstCellProp->width);
		}
		else
		{
			cellProp->headPaintColor = getDefaultValue(_T("TABLE"), _T("HPRGB"), basPRGB);
			cellProp->headTextColor  = getDefaultValue(_T("TABLE"), _T("HTRGB"), 232);
			cellProp->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), 80);
		}
		cellProp->size = 0;
		cellProp->charType = ctNUMERIC;
		cellProp->name = getItemSymbol(_T("Row"));
		if (Prop->cellProperties->GetCount() == 0)
			cellProp->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), 80);
		else
			cellProp->width = getDefaultValue(_T("TABLE"), _T("ROWWIDTH"), ((cellOb*)Prop->cellProperties->GetAt(0))->width);
		cellProp->alignmentText = atRIGHT;
		cellProp->styles |= stVISIBLE;
		
		Prop->cellProperties->Add(cellProp);
	}
}

void CChildView::addPage(int nPage)
{
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();
	if (nPage >= 0)
	{
		CString	data, str, tmps;

		data = Prop->displays;
		str  = Prop->strings;

		tmps.Format(_T("Tab%d"), LOWORD(Prop->keyValues));
		Prop->displays += tmps;
		Prop->displays += _T('\0');
						
		tmps.Format(_T("1:%d"),LOWORD(Prop->keyValues));
		Prop->strings += tmps;

		Prop->strings += _T('\0');
		Prop->checked = nPage;
		Prop->keyValues = MAKELONG(LOWORD(Prop->keyValues)+1, HIWORD(Prop->keyValues));
	}
}

void CChildView::addItem()
{
	if (m_selF < 0)
		return;

	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();

	switch (Prop->kind)
	{
	case fmGRID:
	case fmGRIDEX:
		addCol();
		ShowVScroll(Prop);
		break;
	case fmTABLE:
		addCol();
		break;

	case fmTAB:
		addPage(Prop->checked);
		break;
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REDRAWCTRL, m_selF);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOPRO, 0);
}

int CChildView::removeForm(int idx)
{
	if (idx < 0) return m_mapH->forms.GetCount();
	int index;
	int iorder;
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = (CformProp*)ObjLoad->getProp();
	CString strName(""); 
	strName.Format("%s=",Prop->name);
	if (Prop->kind == fmFORM)
	{
		CObjectLoad* FormLoad = NULL;
		for (size_t ii = 0; ii < (size_t)m_ArryFormObj.GetCount(); ii++)
		{
			FormLoad = (CObjectLoad*)m_ArryFormObj.GetAt(ii);
			if (ObjLoad->getWnd() == FormLoad->m_view)
			{
				delete FormLoad;
				m_ArryFormObj.RemoveAt(ii);
				break;
			}
		}
	}

	index = Prop->index;
	iorder = Prop->iorder;
	/////////////////////////////////////////////////////////////////////////////////////////////
	int posStart = m_mapH->trinfo.Find(strName);
	int pos = 0;
	if (posStart != -1)
	{
		char Seq;
		int cnt = m_mapH->trinfo.GetLength();
		for (int ii = posStart; ii < cnt; ii++)
		{
			Seq = m_mapH->trinfo.GetAt(ii);
			switch (Seq)
			{
			case GB_XML_SYMBOL:
				pos++;
				ii = cnt;
				break;
			case GB_XML_INOUT:
				ii = cnt;
				pos++;
				break;
			case GB_XML_SEQ:
				pos++;
				ii = cnt;
				break;
			default:
				pos++;
				break;
			}
		}

		m_mapH->trinfo.Delete(posStart, pos);
	}
	/////////////////////////////////////tab order sort//////////////////////////////////////////
	if (index != 0)
	{
		CformProp* Prop = NULL;
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
			if (isValidTabOrder(Prop))
			{
				if (Prop->index > index)
					Prop->index--;
			}
		}
		--m_nSeqTabOdr;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////io order sort//////////////////////////////////////////
	if (iorder != 0)
	{
		CformProp* Prop = NULL;
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
			if (isValidIOOrder(Prop))
			{
				if (Prop->iorder > iorder)
					Prop->iorder--;
			}
		}
		--m_nSeqIoOdr;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	delete ObjLoad;
	m_mapH->forms.RemoveAt(idx);
	return m_mapH->forms.GetCount();
}

int CChildView::deleteForm(int idx)
{
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
	if (Prop->group)
	{
		CformProp* stdProp = Prop->group;
		CObjectLoad*	GroupObjLoad = NULL;
		CformProp*	GroupProp = NULL;
		for (int ii = m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			GroupObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii - 1);
			GroupProp = GroupObjLoad->getProp();
			if (GroupProp->group == stdProp && GroupProp != stdProp)
			{
				removeForm(ii - 1);
			}
		}

		for (int ii = m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			GroupObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii - 1);
			GroupProp = GroupObjLoad->getProp();
			if (GroupProp == stdProp)
			{
				removeForm(ii - 1);
			}
		}
	}
	else
	{
		removeForm(idx);
	}
	return m_mapH->forms.GetCount();
}

bool CChildView::isInPage(int idx)
{
	return false;
	//struct	_formR*	formR = m_formItem->GetForm(idx);
	//if (formR->tabN->checked != formR->pageN)
	//	return false;
	//if (formR->tabN->tabN)
	//	return isInPage(m_formItem->GetIdxOfPtr(formR->tabN));
	//return true;
}

void CChildView::drawBackground(CDC *pDC)
{
	if (m_mapH->mapK == MK_PROCEDURES)
		return;

	CBrush	*brush;

	switch (m_mapH->imageK)
	{
	case IK_BRUSH:
		DWORD	value;
		value = m_mapH->bkClr;
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
		brush = new CBrush(PALETTERGB(236, 236, 236));
		break;
	}

	CBrush* oldBrush = pDC->SelectObject(brush); 
	CRect rect; 
	pDC->GetClipBox(&rect); 

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(oldBrush); 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CPen pen, *oldpen;

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = LINE_COLOR;
	pen.CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);

	oldpen = pDC->SelectObject(&pen);
	pDC->SetBkMode(TRANSPARENT);

	CRect rcline;
	for (int ii = 0; ii < m_aryBKLine.GetCount(); ii++)
	{
		if (m_iBKLineIdx == ii)
			continue;
		rcline = &m_aryBKLine.GetAt(ii);
		pDC->MoveTo(rcline.left, rcline.top);
		pDC->LineTo(rcline.right, rcline.bottom);
	}
	pDC->SelectObject(oldpen);
	pen.DeleteObject();

	if (m_iBKLineIdx > -1)
	{
		logBrush.lbColor = LINE_SELCLR;
		pen.CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);

		oldpen = pDC->SelectObject(&pen);

		rcline = &m_aryBKLine.GetAt(m_iBKLineIdx);
		pDC->MoveTo(rcline.left, rcline.top);
		pDC->LineTo(rcline.right, rcline.bottom);

		pDC->SelectObject(oldpen);
		pen.DeleteObject();

		CString text;
		if (rcline.left == 0)
			text.Format("Pos Y : %d", rcline.top);
		else
			text.Format("Pos X : %d", rcline.left);
		pDC->DrawText(text, m_LineTextRC, DT_VCENTER | DT_SINGLELINE | DT_RIGHT);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	brush->DeleteObject();
	delete brush;
}

void CChildView::UnSelBKLine()
{
	if (m_iBKLineIdx > -1)
	{
		CRect UpdateRC;
		UpdateRC = m_aryBKLine.GetAt(m_iBKLineIdx);
		m_iBKLineIdx = -1;
		UpdateRC.InflateRect(1, 1);
		InvalidateRect(UpdateRC);
	}
	UpdateLineText();
}

void CChildView::UpdateLineText()
{
	GetClientRect(m_LineTextRC);
	m_LineTextRC.right -= 5;
	m_LineTextRC.left = m_LineTextRC.right;
	m_LineTextRC.top = m_LineTextRC.bottom;
	m_LineTextRC.left -= 150;
	m_LineTextRC.top -= 20;
	InvalidateRect(m_LineTextRC);
}

void CChildView::moveBKLine(DWORD arrow)
{
	CRect* rc;
	rc = &m_aryBKLine.GetAt(m_iBKLineIdx);
	switch (arrow)
	{
	case VK_UP:
		if (rc->top != 0)
		{
			rc->top -= 1;
			rc->bottom -= 1;
		}
		break;
	case VK_DOWN:
		if (rc->top != 0)
		{
			rc->top += 1;
			rc->bottom += 1;
		}
		break;
	case VK_LEFT:
		if (rc->left != 0)
		{
			rc->left -= 1;
			rc->right -= 1;
		}
		break;
	case VK_RIGHT:
		if (rc->left != 0)
		{
			rc->left += 1;
			rc->right += 1;
		}
		break;
	default:
		break;
	}

	CRect UpdateRC;
	UpdateRC.CopyRect(*rc);
	UpdateRC.InflateRect(2, 2);
	InvalidateRect(UpdateRC);

	UpdateLineText();
}

void CChildView::delBKLine()
{
	CRect rc;
	rc = m_aryBKLine.GetAt(m_iBKLineIdx);
	m_aryBKLine.RemoveAt(m_iBKLineIdx);
	rc.InflateRect(1, 1);
	InvalidateRect(rc);
	m_iBKLineIdx--;
	if (m_aryBKLine.GetCount() > 0)
	{
		m_iBKLineIdx = m_aryBKLine.GetCount() - 1;
		rc = m_aryBKLine.GetAt(m_iBKLineIdx);
		rc.InflateRect(1, 1);
		InvalidateRect(rc);
	}
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

	if (pDC->GetClipBox(&clipRect) == ERROR)
		return;

	CformProp* Prop = NULL;
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		if (!m_showFORM && !(Prop->styles & stVISIBLE))
			continue;

		tRc.SetRect(Prop->rect.left, Prop->rect.top, Prop->rect.right, Prop->rect.bottom);
		if (!IntersectRc.IntersectRect(tRc, clipRect))
			continue;

		switch (Prop->kind)
		{
		case fmLABEL:
		case fmBOX:
		case fmGROUP:
		case fmIMVIEW:
		case fmBUTTON:
		case fmRADIO:
		case fmCHECK:
		case fmEDIT:
		case fmCOMBO:
		case fmOUTPUT:
		case fmMEMO:
		case fmGRID:
		case fmGRIDEX:
		case fmTABLE:
		case fmTREE:
			break;
		case fmFORM:
		case fmTAB:
		case fmBROWSER:
		case fmOBJECT:
			break;
		default:
			continue;
		}
	}
}

bool CChildView::drawImage(CDC *pDC)	// just load bitmap
{
	CString path = m_mapH->imageV;

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

		tmps.Format(_T("%s/%s/%s"), getRoot(), IMAGEDIR, path);
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
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CfmObject*	Obj = ObjLoad->getObject();
	CformProp*	Prop = ObjLoad->getProp();
	CWnd*		Wnd = ObjLoad->getWnd();

	if (Prop->checked >= 0)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		CString strTmp(""),strTmp2("");
		char*	sDsp = (char *)Prop->displays.operator LPCTSTR();
		for (int ii = 0; ii < (int)Prop->keyValues; ii++)
		{
			if ((int)Prop->checked != ii)
			{
				strTmp2.Format(_T("%s"), sDsp);
				strTmp += strTmp2;
				strTmp += _T('\0');
			}
			sDsp += strlen(sDsp);
			sDsp++;
		}
		Prop->displays = strTmp;

		strTmp = _T("");
		strTmp2 = _T("");
		char*	sStr = (char *)Prop->strings.operator LPCTSTR();
		for (int ii = 0; ii < (int)Prop->keyValues; ii++)
		{
			if ((int)Prop->checked != ii)
			{
				strTmp2.Format(_T("%s"), sStr);
				strTmp += strTmp2;
				strTmp += _T('\0');
			}
			sStr += strlen(sStr);
			sStr++;
		}
		Prop->keyValues--;
		Prop->strings = strTmp;

		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REDRAWCTRL, false), idx);
	}
}

CRect CChildView::getFormRect(int idx)
{
	CRect	tRc;

	tRc.SetRectEmpty();
	if (m_mapH->forms.GetCount() <= idx)
		return tRc;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = ObjLoad->getProp();

	tRc.SetRect(Prop->rect.left, Prop->rect.top, Prop->rect.right, Prop->rect.bottom);
	return tRc;
}

void CChildView::keyArrowLeft()
{
	if (m_selF == -1)
		return;

	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(0,0,-1,0);
		else
			m_FmCtrl->SetObjRect(0,0,-5,0);
	}
	else	// Move
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(-1,0,0,0);
		else
			m_FmCtrl->SetObjRect(-5,0,0,0);
	}

	m_mapH->modified = true;
}

void CChildView::keyArrowRight()
{
	if (m_selF == -1)
		return;

	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(0,0,1,0);
		else
			m_FmCtrl->SetObjRect(0,0,5,0);
	}
	else	// Move
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(1,0,0,0);
		else
			m_FmCtrl->SetObjRect(5,0,0,0);
	}

	m_mapH->modified = true;
}

void CChildView::keyArrowUp()
{
	if (m_selF == -1)
		return;

	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(0,0,0,-1);
		else
			m_FmCtrl->SetObjRect(0,0,0,-5);
	}
	else	// Move
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(0,-1,0,0);
		else
			m_FmCtrl->SetObjRect(0,-5,0,0);
	}

	m_mapH->modified = true;
}

void CChildView::keyArrowDown()
{
	if (m_selF == -1)
		return;

	if (::GetKeyState(VK_SHIFT) & 0x8000)		// Size 
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(0,0,0,1);
		else
			m_FmCtrl->SetObjRect(0,0,0,5);
	}
	else	// Move
	{
		if(::GetKeyState(VK_CONTROL) & 0x8000)
			m_FmCtrl->SetObjRect(0,1,0,0);
		else
			m_FmCtrl->SetObjRect(0,5,0,0);
	}

	m_mapH->modified = true;
}

void CChildView::setGroup()
{
	m_FmCtrl->SetGroup();
}

void CChildView::releaseGroup()
{
	m_FmCtrl->ReleaseGroup();
	Invalidate();
}

int CChildView::getTableForm(CPoint point)
{
	CRect	tRc;

	CformProp* Prop = NULL;
	for (int ii = m_mapH->forms.GetCount() - 1; ii >= 0; ii--)
	{
		Prop = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
		switch (Prop->kind)
		{
		case fmTAB:
		case fmGRID:
		case fmGRIDEX:
		case fmTABLE:	
			//if (formR->tabN)
			//{
			//	if (isInPage(ii))
			//		break;
			//	else
			//		continue;
			//}
			break;
		default:
			continue;
		}

		tRc.SetRect(Prop->rect.left, Prop->rect.top, Prop->rect.right, Prop->rect.bottom);
		if (tRc.PtInRect(point))
			return ii;
	}

	return -1;
}

void CChildView::addColumn(bool is_default, int grid, class Csymbol* symbol)
{
	CRect	rRc;
	int	jj;
	
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_selF);
	CformProp*	Prop = ObjLoad->getProp();

	int ii = Prop->cellProperties->GetCount();

	m_selF = grid;

	cellOb* cellProp = new cellOb();
	cellProp->name = symbol->m_name;
	cellProp->headText = symbol->m_head;
	cellProp->size = symbol->m_size;
	cellProp->charType = symbol->m_type;
	cellProp->ioType  = symbol->m_iok;

	if (is_default)
	{
		switch (symbol->m_kind)
		{
		case fmCOMBO:
			cellProp->kind = ckCOMBO;
			break;

		case fmCHECK:
			cellProp->charType = ctNUMERIC;
			cellProp->kind = ckCHECK;
			break;
		default:
			break;
		}
	}

	switch (Prop->kind)
	{
	case fmGRID:
		//repR->cols++;
		cellProp->width = (WORD)symbol->m_headsz.cx;
		Prop->keyValues = MAKELONG((WORD)symbol->m_headsz.cy, HIWORD(Prop->keyValues));
		cellProp->width = (WORD)max(cellProp->width, symbol->m_datasz.cx);
		Prop->keyValues = MAKELONG((DWORD)max((int)Prop->keyValues, symbol->m_datasz.cy), HIWORD(Prop->keyValues));
		break;

	case fmTABLE:
		//repR->rows++;
		cellProp->width = (WORD)max(cellProp->width, symbol->m_headsz.cx);
		for (jj = 1; jj <= ii; jj++)
			cellProp->width = ((cellOb*)Prop->cellProperties->GetAt(0))->width;
		break;
	}
	
	cellProp->alignmentHead = atCENTER;
	cellProp->alignmentText = symbol->m_align;
	cellProp->headPaintColor  = symbol->m_hpRGB;
	cellProp->headTextColor  = symbol->m_htRGB;
	cellProp->paintColor   = symbol->m_pRGB;
	cellProp->textColor   = symbol->m_tRGB;
	cellProp->attributes   = symbol->m_attr;
	if (cellProp->attributes & faCORGB)
	{
		cellProp->auxAttributes |= ccSUP;
		cellProp->whatEdit = weCORGB;
		cellProp->auxValues = MAKELONG(CORGB_DOWN, CORGB_UP);
	}
	
	if (cellProp->charType == ctNUMERIC && cellProp->ioType == ioOUTPUT)
		cellProp->offsets = symbol->m_float;
	
	if (!symbol->m_edit.IsEmpty())
	{
		cellProp->whatEdit = weIT;
		cellProp->editFormat = symbol->m_edit;
	}
	//cellR->headN  = 1;
	cellProp->styles |= stVISIBLE;
	Prop->cellProperties->Add(cellProp);
}

void CChildView::addForm(bool is_default, Csymbol* symbol, CPoint pt, CformProp* Prop, int idx)
{
	if (m_mapH->forms.GetCount() >= MAX_FORM)
		return;

	CRect	cRc;
	//struct	_formR*	formR;

	if (is_default && symbol->m_kind <= 0)
		return;

	GetClientRect(&cRc);
	
	if (symbol->m_iok == ioOUTPUT)
	{
		//// table
		CformProp* formProp = new CformProp();
		formProp->Init(fmTABLE);
		formProp->name = GetInitSymbol("", formProp->kind);
		formProp->rect.OffsetRect(pt.x, pt.y);
		formProp->auxAttributes |= gaMULTI;

		formProp->cellProperties = new CObArray();
		CObjectLoad* ObjLoad = new CObjectLoad(formProp, m_piForm, this);
		if (!ObjLoad->LoadLib(formProp->kind))
		{
			delete ObjLoad;
			return;
		}
		m_mapH->forms.Add(ObjLoad);

		if (isValidTabOrder(formProp))
			formProp->index = ++m_nSeqTabOdr;
		if (isValidIOOrder(formProp))
			formProp->iorder = ++m_nSeqIoOdr;

		if (idx == 0)
		{
			formProp->rect.top = pt.y;
			formProp->rect.left = pt.x;
		}
		formProp->rect.right = min(cRc.right, formProp->rect.left + symbol->m_headsz.cx + symbol->m_datasz.cx);
		formProp->rect.bottom = min(cRc.bottom, formProp->rect.top + symbol->m_headsz.cy * (idx + 1));
		formProp->ioType    = ioOUTPUT;

		
		//formR->keys = 20;
		//formR->tabN = tabN;
		//if (tabN)
		//	formR->pageN = tabN->checked;		
		m_selF = m_mapH->forms.GetCount() - 1;
		addColumn(is_default, m_selF, symbol);
		ObjLoad->LoadProc();
		SelObj(m_selF);
	}
	else
	{
		// label
		if (symbol->m_headsz.cx != 0 && symbol->m_headsz.cy != 0)
		{
			CformProp* formProp = new CformProp();
			formProp->Init(fmLABEL);
			formProp->name = GetInitSymbol("", formProp->kind);
			formProp->rect.OffsetRect(pt.x, pt.y);

			CObjectLoad* ObjLoad = new CObjectLoad(formProp, m_piForm, this);
			if (!ObjLoad->LoadLib(formProp->kind))
			{
				delete ObjLoad;
				return;
			}
			m_mapH->forms.Add(ObjLoad);

			if (isValidTabOrder(formProp))
				formProp->index = ++m_nSeqTabOdr;
			if (isValidIOOrder(formProp))
				formProp->iorder = ++m_nSeqIoOdr;

			formProp->rect.top = pt.y;
			formProp->rect.left   = pt.x;
			formProp->rect.right  = min(cRc.right,  formProp->rect.left + symbol->m_headsz.cx);
			formProp->rect.bottom = min(cRc.bottom, formProp->rect.top + symbol->m_headsz.cy);
			pt.x = formProp->rect.right;
			formProp->strings = symbol->m_head;
			formProp->alignment = atCENTER;
			formProp->paintColor = symbol->m_hpRGB;
			formProp->textColor = symbol->m_htRGB;
			
			formProp->fontPoint  = m_mapH->fPoint;
			formProp->fontName = m_mapH->fName;

			//if (tabN)
			//	formR->pageN = tabN->checked;
			m_selF = m_mapH->forms.GetCount() - 1;
			ObjLoad->LoadProc();
			//SelObj(m_selF);
		}
		
		// data
		CformProp* formProp = new CformProp();
		formProp->Init(fmLABEL);
		formProp->name = GetInitSymbol("", formProp->kind);
		formProp->rect.OffsetRect(pt.x, pt.y);

		CObjectLoad* ObjLoad = new CObjectLoad(formProp, m_piForm, this);
		if (!ObjLoad->LoadLib(formProp->kind))
		{
			delete ObjLoad;
			return;
		}
		m_mapH->forms.Add(ObjLoad);

		if (isValidTabOrder(formProp))
			formProp->index = ++m_nSeqTabOdr;
		if (isValidIOOrder(formProp))
			formProp->iorder = ++m_nSeqIoOdr;
		
		formProp->name = symbol->m_name;
		if (is_default)
			formProp->kind = symbol->m_kind;
		else
		{
			switch (symbol->m_iok)
			{
			case ioINPUT:
			case ioBOTH:
				formProp->kind = fmEDIT;		break;
			case ioOUTPUT:
			default:
				formProp->kind = fmOUTPUT;		break;
			}
		}
		formProp->size = symbol->m_size;
		formProp->charType = symbol->m_type;
		formProp->ioType  = symbol->m_iok;

		formProp->rect.top = pt.y;
		formProp->rect.left   = pt.x + 1;
		formProp->rect.right = min(cRc.right, formProp->rect.left + symbol->m_datasz.cx);
		formProp->rect.bottom = min(cRc.bottom, formProp->rect.top + symbol->m_datasz.cy);

		formProp->alignment = symbol->m_align;
		formProp->paintColor = symbol->m_pRGB;
		formProp->textColor = symbol->m_tRGB;
		formProp->attributes = symbol->m_attr;
		if (formProp->attributes & faCORGB)
		{
			formProp->auxAttributes |= ccSUP;
			formProp->whatEdit = weCORGB;
			formProp->auxValues = MAKELONG(CORGB_DOWN, CORGB_UP);
		}
		
		if (formProp->charType == ctNUMERIC && formProp->kind == fmOUTPUT)
			formProp->offsets = symbol->m_float;
		
		if (!symbol->m_edit.IsEmpty())
		{
			formProp->whatEdit = weIT;
			formProp->editFormat = symbol->m_edit;
		}
		
		formProp->fontPoint  = m_mapH->fPoint;
		formProp->fontName = m_mapH->fName;
		
		//formR->tabN = tabN;
		//if (tabN)
		//	formR->pageN = tabN->checked;
		m_selF = m_mapH->forms.GetCount() - 1;
		ObjLoad->LoadProc();
		SelObj(m_selF);
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL, FALSE);
	//Invalidate(FALSE);
}

void CChildView::showMenu()
{
	CPoint	point;
	CMenu	menu, submenu, Layoutmenu, LayoutmenuSub;
	CObjectLoad*	ObjLoad = NULL;
	CfmObject*	Obj = NULL;
	CformProp*	Prop = NULL;
	if (m_selF >= 0)
	{
		ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(m_selF);
		Prop = (CformProp*)ObjLoad->getProp();
	}
	
	menu.CreatePopupMenu();
	submenu.CreatePopupMenu();
	Layoutmenu.CreatePopupMenu();		// Layout menu 생성
	LayoutmenuSub.CreatePopupMenu();	// Layout sub menu 생성 (new, delete, rename)

	GetCursorPos(&point);
	submenu.AppendMenu(MF_STRING, ID_AUX_SETGROUP, _T("Set Group"));
	submenu.AppendMenu(MF_STRING, ID_AUX_RELEASEGROUP, _T("Release Group"));
	
	if (m_selF >= 0)
	{
		menu.AppendMenu(MF_STRING, NULL, Prop->name);
		menu.AppendMenu(MF_SEPARATOR);
	}
	menu.AppendMenu(MF_STRING, ID_AUX_TOFIRST, _T("Bring Foreward"));
	menu.AppendMenu(MF_STRING, ID_AUX_TOEND, _T("Send Backward"));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_AUX_SHOWSCRIPT, _T("Script\t(F6)"));
	if (m_FmCtrl->GetSelCount() <= 1)
		menu.AppendMenu(MF_STRING, ID_AUX_SHOWATTR, _T("Properties\t(F5)"));
	menu.AppendMenu(MF_POPUP, UINT(submenu.GetSafeHmenu()),_T("Group"));
	menu.AppendMenu(MF_POPUP, UINT(Layoutmenu.GetSafeHmenu()), _T("Layout Edit"));
	
	if (m_selF >= 0 && m_FmCtrl->GetSelCount() <= 1)
	{
		switch (Prop->kind)
		{
		case fmGRID:
		case fmGRIDEX:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_AUX_COLUMNEDIT, _T("Column Edit\t(F11)"));
			menu.AppendMenu(MF_STRING, ID_AUX_ADDITEM, _T("Add Item"));
			menu.AppendMenu(MF_STRING, ID_AUX_FIT, _T("Column Fit"));
			if (Prop->auxAttributes & gaXHEAD)
				menu.AppendMenu(MF_STRING, ID_AUX_SELCOLUMN, _T("Select Column"));
			break;
		case fmTABLE:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_AUX_COLUMNEDIT, _T("Row Edit\t(F11)"));
			menu.AppendMenu(MF_STRING, ID_AUX_ADDITEM, _T("Add Item"));
			if (!(Prop->auxAttributes & gaMULTI))
				menu.AppendMenu(MF_STRING, ID_AUX_SELCOLUMN, _T("Select Row"));
			break;
		case fmTAB:
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, ID_AUX_ADDITEM, _T("Add Item"));
			menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("PageEdit"));
			break;
		case fmEDIT:
		case fmCOMBO:
		case fmOUTPUT:
			menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("Attributes..."));
			break;
		case fmOBJECT:
			menu.AppendMenu(MF_STRING, ID_AUX_EXTID, _T("Object"));
			break;
		case fmBUTTON:
			menu.AppendMenu(MF_STRING, ID_AUX_FIT, _T("Image Fit"));
			break;
		case fmFORM:
			{
				CformProp* Prop = (CformProp*)((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getProp();
				CString sMap = Prop->strings;
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
	///////////////////////////////////////

	if (!(m_selF >= 0 && (Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE) /*&& !Prop->group*/))
		menu.EnableMenuItem(ID_AUX_COLUMNEDIT, MF_GRAYED);
	
	if (m_selF == -1)
	{
		menu.EnableMenuItem(ID_AUX_TOFIRST, MF_GRAYED);
		menu.EnableMenuItem(ID_AUX_TOEND, MF_GRAYED);
	}

	if (Prop == NULL || (Prop && Prop->group == NULL))
		submenu.EnableMenuItem(ID_AUX_RELEASEGROUP, MF_GRAYED);
	if (m_FmCtrl->GetSelCount() <= 1 || (Prop && Prop->group != NULL))
		submenu.EnableMenuItem(ID_AUX_SETGROUP, MF_GRAYED);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CChildView::sendToFirst()
{
	if (m_selF < 0)
		return;

	int count = m_mapH->forms.GetCount();
	CObjectLoad* ObjLoad = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF));

	CWnd* TargetWnd = ObjLoad->getWnd();
	m_mapH->forms.InsertAt(count, ObjLoad);
	m_mapH->forms.RemoveAt(m_selF);

	//TargetWnd->ShowWindow(SW_HIDE);
	//CWnd* Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(count - 1))->getWnd();
	//TargetWnd->SetWindowPos(Wnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	//TargetWnd->ShowWindow(SW_SHOW);
	CWnd* Wnd = NULL;
	CWnd* Wnd2 = NULL;
	for (size_t ii = 0; ii < (size_t)m_mapH->forms.GetCount() - 1; ii++)
	{
		Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getWnd();
		Wnd2 = ((CObjectLoad*)m_mapH->forms.GetAt(ii + 1))->getWnd();
		Wnd2->SetWindowPos(Wnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		Wnd->ShowWindow(SW_HIDE);
		Wnd2->ShowWindow(SW_HIDE);
	}

	for (size_t ii = 0; ii < (size_t)m_mapH->forms.GetCount(); ii++)
	{
		Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getWnd();
		Wnd->ShowWindow(SW_SHOW);
	}
}

void CChildView::sendToEnd()
{
	if (m_selF < 0)
		return;

	CObjectLoad* ObjLoad = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF));

	CWnd* TargetWnd = ((CObjectLoad*)m_mapH->forms.GetAt(m_selF))->getWnd();
	m_mapH->forms.RemoveAt(m_selF);
	m_mapH->forms.InsertAt(0, ObjLoad);

	CWnd* Wnd = NULL;
	CWnd* Wnd2 = NULL;
	for (size_t ii = 0; ii < (size_t)m_mapH->forms.GetCount() - 1; ii++)
	{
		Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getWnd();
		Wnd2 = ((CObjectLoad*)m_mapH->forms.GetAt(ii+1))->getWnd();
		Wnd2->SetWindowPos(Wnd, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		Wnd->ShowWindow(SW_HIDE);
		Wnd2->ShowWindow(SW_HIDE);
	}

	for (size_t ii = 0; ii < (size_t)m_mapH->forms.GetCount(); ii++)
	{
		Wnd = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getWnd();
		Wnd->ShowWindow(SW_SHOW);
	}
}

void CChildView::SelObj(int idx, bool bClear)
{
	if (m_FmCtrl) m_FmCtrl->SelObj(idx, bClear);
}

void CChildView::SelAddObj(int idx, bool bMulti)
{
	if (m_FmCtrl) m_FmCtrl->SelAddObj(idx, bMulti);
}

void CChildView::swapPage(int idx, int first, int second)
{
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = ObjLoad->getProp();

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	Prop->checked = second;
	CString strTmp(""), strTmp2("");
	CString strFirst(""), strSecend("");
	char*	sDsp = (char *)Prop->displays.operator LPCTSTR();
	for (int ii = 0; ii < (int)Prop->keyValues; ii++)
	{
		if (first == ii) strFirst.Format(_T("%s"), sDsp);
		else if (second == ii) strSecend.Format(_T("%s"), sDsp);

		sDsp += strlen(sDsp);
		sDsp++;
	}

	sDsp = (char *)Prop->displays.operator LPCTSTR();
	for (int ii = 0; ii < (int)Prop->keyValues; ii++)
	{
		if (first == ii) strTmp2.Format(_T("%s"), strSecend);
		else if (second == ii) strTmp2.Format(_T("%s"), strFirst);
		else strTmp2.Format(_T("%s"), sDsp);

		strTmp += strTmp2;
		strTmp += _T('\0');
		sDsp += strlen(sDsp);
		sDsp++;
	}
	Prop->displays = strTmp;

	strTmp = _T("");
	strTmp2 = _T("");
	char*	sStr = (char *)Prop->strings.operator LPCTSTR();
	for (int ii = 0; ii < (int)Prop->keyValues; ii++)
	{
		if (first == ii) strFirst.Format(_T("%s"), sStr);
		else if (second == ii) strSecend.Format(_T("%s"), sStr);

		sStr += strlen(sStr);
		sStr++;
	}

	sStr = (char *)Prop->strings.operator LPCTSTR();
	for (int ii = 0; ii < (int)Prop->keyValues; ii++)
	{
		if (first == ii) strTmp2.Format(_T("%s"), strSecend);
		else if (second == ii) strTmp2.Format(_T("%s"), strFirst);
		else strTmp2.Format(_T("%s"), sStr);

		strTmp += strTmp2;
		strTmp += _T('\0');
		sStr += strlen(sStr);
		sStr++;
	}
	Prop->strings = strTmp;

	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REDRAWCTRL, false), idx);
}

void CChildView::swapCell(int idx, int first, int second)
{
	CObjectLoad*	ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(idx);
	CformProp*	Prop = ObjLoad->getProp();

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	Prop->CellSelected = second;
	cellOb* fstProp = (cellOb*)Prop->cellProperties->GetAt(first);
	cellOb tmpProp;
	cellPropCopy(&tmpProp, fstProp);

	cellOb* srcProp = NULL;
	cellOb* dstProp = NULL;
	if (first < second)
	{
		for (size_t ii = first; ii < (size_t)Prop->cellProperties->GetCount(); ii++)
		{
			if (ii + 1 == second || ii == Prop->cellProperties->GetCount())
			{
				Prop->CellSelected = ii;
				dstProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				cellPropCopy(dstProp, &tmpProp);
				break;
			}
			else
			{
				dstProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				srcProp = (cellOb*)Prop->cellProperties->GetAt(ii + 1);
				cellPropCopy(dstProp, srcProp);
			}
		}
	}
	else
	{
		for (size_t ii = first; ii >= (size_t)second; ii--)
		{
			if (ii == second)
			{
				Prop->CellSelected = second;
				dstProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				cellPropCopy(dstProp, &tmpProp);
				break;
			}
			else
			{
				dstProp = (cellOb*)Prop->cellProperties->GetAt(ii);
				srcProp = (cellOb*)Prop->cellProperties->GetAt(ii - 1);
				cellPropCopy(dstProp, srcProp);
			}
		}
	}

	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REDRAWCTRL, false), idx);
}

void CChildView::cellPropCopy(cellOb* dstProp, cellOb* srcProp)
{
	dstProp->kind = srcProp->kind;
	dstProp->headKind = srcProp->headKind;
	dstProp->name = srcProp->name;
	dstProp->size = srcProp->size;
	dstProp->charType = srcProp->charType;
	dstProp->ioType = srcProp->ioType;
	dstProp->width = srcProp->width;
	dstProp->entryCombo = srcProp->entryCombo;
	dstProp->alignmentHead = srcProp->alignmentHead;
	dstProp->alignmentText = srcProp->alignmentText;
	dstProp->textColor = srcProp->textColor;
	dstProp->paintColor = srcProp->paintColor;
	dstProp->headText = srcProp->headText;
	dstProp->headTextColor = srcProp->headTextColor;
	dstProp->headPaintColor = srcProp->headPaintColor;
	dstProp->fontStyle = srcProp->fontStyle;
	dstProp->fontPoint = srcProp->fontPoint;
	dstProp->fontName = srcProp->fontName;
	dstProp->attributes = srcProp->attributes;
	dstProp->auxAttributes = srcProp->auxAttributes;
	dstProp->flashKeys = srcProp->flashKeys;
	dstProp->whatEdit = srcProp->whatEdit;
	dstProp->auxValues = srcProp->auxValues;
	dstProp->editFormat = srcProp->editFormat;
	dstProp->styles = srcProp->styles;
	dstProp->offsets = srcProp->offsets;
	dstProp->keyParam = srcProp->keyParam;
	dstProp->dominoName = srcProp->dominoName;
	dstProp->displays = srcProp->displays;
	dstProp->strings = srcProp->strings;
	dstProp->images = srcProp->images;
	dstProp->auxImages = srcProp->auxImages;
	dstProp->merges = srcProp->merges;
	dstProp->typeCursor = srcProp->typeCursor;
	dstProp->typeCursorH = srcProp->typeCursorH;
	dstProp->typeHint = srcProp->typeHint;
	dstProp->hintText = srcProp->hintText;
	dstProp->referenceName = srcProp->referenceName;
}

void CChildView::ShowVScroll(CformProp* Prop)
{
	if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
	{
		if (Prop->auxAttributes & gaAUTOSIZE)
		{
			if (HIWORD(Prop->keyValues) < Prop->size && Prop->cellProperties->GetCount() > 0)
			{
				Prop->auxAttributes |= gaVSCR;
			}
			else
			{
				Prop->auxAttributes &= ~gaVSCR;
			}
		}
		else
		{
			int Height = (Prop->iAutoSize * (Prop->size + 1)) + Prop->size + 4;

			int formHeight = Prop->rect.Height() - 4;
			if (Height > formHeight && Prop->cellProperties->GetCount() > 0)
			{
				Prop->auxAttributes |= gaVSCR;
			}
			else
			{
				Prop->auxAttributes &= ~gaVSCR;
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////
		int width = 0;
		cellOb* cellProp = NULL;
		for (size_t ii = 0; ii < (size_t)Prop->cellProperties->GetCount(); ii++)
		{
			cellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
			width += cellProp->width;
		}

		int formWidth = Prop->rect.Width() - 4;
		if (!(Prop->auxAttributes & gaMULTI))
		{
			if (width > formWidth)
			{
				Prop->auxAttributes |= gaHSCR;
			}
			else
			{
				Prop->auxAttributes &= ~gaHSCR;
			}
		}
	}
}

void CChildView::OnLayoutCommand(UINT cmdID)
{
	int EditSel = 0;
	switch (cmdID)
	{
	case ID_LAYOUT_ORIGINAL:	// 원래 편집창으로 이동
	{
		SetOriginalLayout();
	}
		break;
	case ID_LAYOUT_EDITOR:
		CLayoutEditDlg dlg;
		dlg.SetLayoutList(m_mapH->maplayout, &m_arLayout);
		dlg.SetCurLayOutName(m_mapH->LayoutName);
		if (dlg.DoModal() == IDOK)
		{
			if (dlg.m_mapData.GetCount() < 1)
				break;
			//m_formItem->AddLayout(&dlg.m_mapData);
			CString sIndex, sData;
			if (m_mapH->maplayout == nullptr)
				m_mapH->maplayout = new CMapStringToString;
			else
				m_mapH->maplayout->RemoveAll();

			for (POSITION pos = dlg.m_mapData.GetStartPosition(); pos;)
			{
				dlg.m_mapData.GetNextAssoc(pos, sIndex, sData);
				m_mapH->maplayout->SetAt(sIndex, sData);
			}
			////////////////////////////////////////////
			SetDefaultLayout();
		}
		break;
	}
}

void CChildView::SetDefaultLayout()
{
	//struct _formR* formR;
	CString sIndex, sData, ss;
	struct _Layout* playout = nullptr;

	CMapStringToString* map = m_mapH->maplayout;
	CMapStringToString mapdel;

	// Delete list 
	for (POSITION pos = map->GetStartPosition(); pos;)
	{
		map->GetNextAssoc(pos, sIndex, sData);
		mapdel.SetAt(sData, sIndex);
	}

	CObjectLoad*	ObjLoad = NULL;
	CformProp*	Prop = NULL;
	for (POSITION pos = map->GetStartPosition(); pos;)
	{
		map->GetNextAssoc(pos, sIndex, sData);
		for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
		{
			ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii);
			Prop = (CformProp*)ObjLoad->getProp();

			// Delete layout...
			for (POSITION pos1 = Prop->layout->GetStartPosition(); pos1;)
			{
				Prop->layout->GetNextAssoc(pos1, ss, (void*&)playout);
				if (!mapdel.Lookup(ss, sIndex))
				{
					Prop->layout->RemoveKey(ss);
					delete playout;
				}
			}

			if (Prop->layout->Lookup(sData, (void*&)playout))
				continue;

			playout = new _Layout;
			ZeroMemory(playout, sizeof(_Layout));

			playout->left = Prop->rect.left;
			playout->right = Prop->rect.right;
			playout->top = Prop->rect.top;
			playout->bottom = Prop->rect.bottom;
			playout->bfirst = true;
			playout->properties = 0x00;
			if (Prop->styles & stVISIBLE)
				playout->properties = Prop->styles;

			Prop->layout->SetAt(sData, playout);
		}
	}
}

bool CChildView::IsLayoutMode()
{
	CString strLayOutName;
	strLayOutName.Format("%s", m_mapH->LayoutName);
	if (strLayOutName.CompareNoCase("Original"))
		return false;

	return true;
}

// Layout
void CChildView::OnLayoutModeCommand(UINT cmdID)
{
	// 이미 Layout Mode인지 체크
	//if (m_bLayoutMode)
	//	return;

	CString sData, sIndex;
	sIndex = m_arLayout.GetAt(cmdID - LAYOUTMENU);
	m_mapH->maplayout->Lookup(sIndex, sData);

	// 현재 자신인지 체크
	if (!sData.CompareNoCase(m_sPreLayoutName))
		return;

	// Layout 모드 진입
	m_bLayoutMode = true;
	//m_formItem->SetCurrLayoutName(sData);
	ZeroMemory(m_mapH->LayoutName, sizeof(m_mapH->LayoutName));
	CopyMemory(m_mapH->LayoutName, sData, sData.GetLength());
	// 해당 Layout 모드로 정보 세팅
	SetLayoutInfo(sData, m_sPreLayoutName);
	m_sPreLayoutName = sData;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_LAYOUTMODE, 1);
	Invalidate();
}

void CChildView::SetLayoutInfo(CString sKey, CString sPreKey)
{
	struct _Layout *playout, *pPrelayout;
	CObjectLoad*	ObjLoad = NULL;
	CformProp*	Prop = NULL;
	for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
	{
		ObjLoad = (CObjectLoad*)m_mapH->forms.GetAt(ii);
		Prop = ObjLoad->getProp();
		// Save Infomation
		if (Prop->layout->Lookup(sPreKey, (void*&)pPrelayout))
		{
			pPrelayout->left = Prop->rect.left;
			pPrelayout->right = Prop->rect.right;
			pPrelayout->top = Prop->rect.top;
			pPrelayout->bottom = Prop->rect.bottom;
			pPrelayout->bfirst = false;

			if (Prop->styles & stVISIBLE)
				pPrelayout->properties |= stVISIBLE;
			else
				pPrelayout->properties &= ~stVISIBLE;
		}

		// Change Setting
		Prop->layout->Lookup(sKey, (void*&)playout);

		Prop->rect.left = playout->left;
		Prop->rect.right = playout->right;
		Prop->rect.top = playout->top;
		Prop->rect.bottom = playout->bottom;

		if (playout->properties & stVISIBLE)
			Prop->styles |= stVISIBLE;
		else
			Prop->styles &= ~stVISIBLE;
	}

	m_FmCtrl->LayoutMoveObj();
}

CString CChildView::AddObject(CString strInOut, CString strName, CPoint *pt, int iGap, CString strData)
{
	CString strResult(""), strText("");
	short x = (WORD)pt->x;
	short y = (WORD)pt->y;
	bool bGridTxt = false;
	CPoint point;
	point.SetPoint(pt->x, pt->y);
	int size = 0;

	if (x < 0)
		point.x = 0;
	else
		x = 0;

	if (y < 0)
		point.y = 0;
	else
		y = 0;
	ScreenToClient(&point);
	point.x += x;
	point.y += y;

	char Kind = NULL;
	if (strInOut == _T("I"))
		Kind = fmEDIT;
	else
		Kind = fmOUTPUT;
	
	while (!strData.IsEmpty())
	{
		if (strData.GetAt(0) == '@')
		{
			Kind = fmGRID;
			bGridTxt = true;
		}
		else if (strData.GetAt(0) == '$')
		{
			Kind = fmGRID;
			bGridTxt = false;
		}
		CformProp* formProp = new CformProp();
		formProp->Init(Kind);
		formProp->name = GetInitSymbol("", formProp->kind);
		formProp->rect.OffsetRect(point.x, point.y);
		if (Kind == fmGRID)
		{
			formProp->cellProperties = new CObArray();
			CString tmps("");

			cellOb* lstCellProp = NULL;
			//tmps = Parser(strData, _T(":"));
			Parser(strData, _T(":"));
			Parser(strData, _T(":"));
			if (bGridTxt) Parser(strData, _T(":"));

			strResult += formProp->name;
			while (!strData.IsEmpty())
			{
				Parser(strData, _T(":"));
				Parser(strData, _T(":"));
				size = atoi(Parser(strData, _T(":")));
				if (bGridTxt) strText = Parser(strData, _T(":"));
				else strText = _T("");

				cellOb* cellProp = new cellOb();
				cellProp->paintColor = basINPUT;
				cellProp->textColor = 232;
				if (formProp->cellProperties->GetCount() > 0)
				{
					lstCellProp = (cellOb*)formProp->cellProperties->GetAt(formProp->cellProperties->GetCount() - 1);
					cellProp->headPaintColor = lstCellProp->headPaintColor;
					cellProp->headTextColor = lstCellProp->headTextColor;
					cellProp->width = lstCellProp->width;
				}
				else
				{
					cellProp->headPaintColor = basPRGB;
					cellProp->headTextColor = 232;
					cellProp->width = 80;
				}

				cellProp->size = size;
				cellProp->headText = strText;
				cellProp->charType = ctNUMERIC;
				cellProp->ioType = ioOUTPUT;
				cellProp->kind = ckNORMAL;
				cellProp->name = getInitSymbol(formProp, _T("Col"));
				strResult += '\t';
				strResult += cellProp->name;
				cellProp->alignmentText = atRIGHT;
				cellProp->styles |= stVISIBLE;
				formProp->cellProperties->Add(cellProp);
			}
		}
		else if (Kind == fmEDIT || Kind == fmOUTPUT)
		{
			Parser(strData, _T(":"));
			Parser(strData, _T(":"));
			size = atoi(Parser(strData, _T(":")));
			Parser(strData, _T(":"));

			formProp->size = size;
			strResult += formProp->name;
		}

		CObjectLoad* ObjLoad = new CObjectLoad(formProp, m_piForm, this);
		if (!ObjLoad->LoadLib(formProp->kind))
		{
			delete ObjLoad;
			return "";
		}
		m_mapH->forms.Add(ObjLoad);

		if (isValidTabOrder(formProp))
			formProp->index = ++m_nSeqTabOdr;
		if (isValidIOOrder(formProp))
			formProp->iorder = ++m_nSeqIoOdr;

		ObjLoad->LoadProc();

		pt->Offset(iGap, iGap);
		point.Offset(iGap, iGap);
		if (!strData.IsEmpty())
		{ 
			strResult += ","; 
		}
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHCTRL);

	return strResult;
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
	return _T("");
}

CString CChildView::getInitSymbol(CformProp* form, CString syms)
{
	CString tmps, slist;
	int	initN;

	if (form->cellProperties == NULL)
		return syms;

	cellOb* cellProp = NULL;
	for (int kk = 0; kk < form->cellProperties->GetCount(); kk++)
	{
		cellProp = (cellOb*)form->cellProperties->GetAt(kk);
		if (!cellProp->name.IsEmpty())
		{
			slist += cellProp->name;
			slist += _T('\t');
		}
	}

	initN = 0;
	slist.MakeUpper();
	for (;;)
	{
		tmps.Format(_T("%s%d"), syms, initN);
		tmps.MakeUpper();
		if (slist.Find(tmps + _T("\t")) == -1)
			break;
		else
			initN++;
	}

	tmps.Format(_T("%s%d"), syms, initN);
	return tmps;
}

void CChildView::SetOriginalLayout()
{
	if (!m_bLayoutMode)
		return ;

	// 원래 Mode로 복귀
	m_bLayoutMode = false;
	CString sName("Original");
	ZeroMemory(m_mapH->LayoutName, sizeof(m_mapH->LayoutName));
	CopyMemory(m_mapH->LayoutName, sName, sName.GetLength());
	SetLayoutInfo("Original", m_sPreLayoutName);
	m_sPreLayoutName = "Original";

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_LAYOUTMODE, 0);
	Invalidate();
}