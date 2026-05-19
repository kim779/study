
#include "stdafx.h"
#include "awFormCtrl.h"

#include "../ChildView.h"
#include "../h/mapform.h"
#include "../h/mapvar.h"
#include "../h/mainvar.h"

#include "../../dll/lib/axislib.h"
#include "../h/fmObject.h"
#include "../../h/ObjectEvent.h"
#include "PointerWnd.h"
#include "SmartGLineMngr.h"
#include "TrackerWnd.h"
#include "awObjectLoad.h"
#include "fmTracker.h"
#include "markWnd.h"

CFmCtrl::CFmCtrl(CChildView* view, CAxisDraw* draw)
{
	m_view = view;
	m_dc = (CDC *)NULL;
	m_draw = draw;
	m_selTool = _T("Pointer");
	m_bObjSel = -1;
	m_iDownEv = -1;
	m_iMoveEv = -1;
	m_markWnd = NULL;
	m_nGap = (int)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETGAP, 0);
	m_bShiftKey = FALSE;
	m_pSmartGLineMngr = NULL;
}

CFmCtrl::~CFmCtrl()
{
	for (size_t ii = m_fmWndArry.GetCount(); ii > 0; ii--)
	{
		CPointerWnd* PointerWnd = m_fmWndArry.GetAt(ii - 1);
		PointerWnd->DestroyWindow();
		delete PointerWnd;
	}
	m_fmWndArry.RemoveAll();

	for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
	{
		CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
		delete fmTracker;
	}
	m_fmTkArry.RemoveAll();

	if (m_markWnd)
	{
		m_markWnd->DestroyWindow();
		delete m_markWnd;
		m_markWnd = NULL;
	}
	if (m_pSmartGLineMngr) delete m_pSmartGLineMngr;
}

void CFmCtrl::SetName(CString name)
{
	m_selTool = name;
}

void CFmCtrl::MouseDown(CPoint point)
{
	int	PenWidth;
	int	PenStyle;
	m_view->SetCapture();
	m_bShiftKey = FALSE;

	m_dc = new CClientDC(m_view);

	m_dc->SelectStockObject(NULL_BRUSH);
	m_bObjSel = ObjSelecte(point);
	CRect ObjRc;

	if (m_selTool == _T("Pointer") && m_bObjSel > -1)	//오브젝트에서 마우스 다운
	{
		PenWidth = 1;
		PenStyle = PS_USERSTYLE;
		if (!(::GetKeyState(VK_CONTROL) & 0x8000))
		{
			point.x = ((int)(point.x / m_nGap)) * m_nGap;
			point.y = ((int)(point.y / m_nGap)) * m_nGap;
		}
		m_startPt = m_toPt = point;

		
		if (::GetKeyState(VK_SHIFT) < 0x00)
		{
			m_bShiftKey = TRUE;

			CformProp* Prop = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))->getProp();
			Prop->block = true;
		}
		else if (m_fmTkArry.GetCount() == 1) //오브젝트 한개 선택되있을 경우
		{
			if (((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad) != ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))) // 선택되있는 오브젝트가 아닌 다른 오브젝트선택의 경우
			{

				CformProp* Prop = NULL;
				for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
				{
					Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
					Prop->block = false;
				}
				for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
				{
					CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
					delete fmTracker;
				}
				m_fmTkArry.RemoveAll();
				if (m_view->m_nCursor == 0) ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			}
			else
			{
				CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
				CformProp* Prop = fmTracker->m_ObjLoad->getProp();
				CfmObject* Obj = fmTracker->m_ObjLoad->getObject();
				if(Prop->group && m_view->m_nCursor != 0) return ; //그룹 일 경우 싸이즈 조정 불가

				if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE || Prop->kind == fmTAB)  //오브젝트의 헤더를 마우스다운한 것 인지 체크
				{
					m_iDownEv = -1;
					m_iDownEv = Obj->receiveBuilder(Prop->kind, MAKEWPARAM(LBTNDOWN_EV, 0), MAKELPARAM(m_toPt.x - Prop->rect.left, m_toPt.y - Prop->rect.top));
					m_iMoveEv = m_iDownEv;
					if (m_iDownEv != -1)	// -1이 아닐경우 오브젝트의 헤더를 클릭함
					{
						if (Prop->kind == fmTAB)
						{
							Prop->checked = m_iDownEv;
							Obj->Reload();
						}
						else
						{
							SetTimer(m_view->m_hWnd, TM_DRAG, 500, NULL);
							Prop->CellSelected = m_iDownEv;
						}
						return;
					}
					if (m_view->m_nCursor == 0) ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
				}
				else
				{
					if (m_view->m_nCursor == 0) ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
				}
				fmTracker->Hide();
			}

			CformProp* Prop = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))->getProp();
			if(Prop->group)
			{
				ObjRc.CopyRect(Prop->groupRc);
				m_view->ClientToScreen(ObjRc);
			}
			else
			{
				ObjRc.CopyRect(Prop->rect);
				m_view->ClientToScreen(ObjRc);
			}

			CPointerWnd* pPointerWnd = new CPointerWnd(m_view);
			pPointerWnd->CreateEx(WS_EX_LAYERED, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH)),
				NULL, WS_POPUP, ObjRc, m_view->GetParent(), NULL);
			::SetLayeredWindowAttributes(pPointerWnd->m_hWnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);
			pPointerWnd->Init(PenWidth, PenStyle);
			pPointerWnd->SaveRect(ObjRc);
			pPointerWnd->ShowWindow(SW_SHOWNOACTIVATE);
			m_fmWndArry.Add(pPointerWnd);			
		}
		else if (m_fmTkArry.GetCount() > 1) //오브젝트가 여러개 선택되어 있을 경우
		{
			if (m_view->m_nCursor == 0) ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			bool bTrackerSel = false;
			for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
			{
				if (((CObjectLoad*)m_fmTkArry.GetAt(ii)->m_ObjLoad) == ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel)))
				{
					bTrackerSel = true;
					break;
				}
			}
			if (bTrackerSel) // 선택된 오브젝트들 중에서 마우스다운이 일어남
			{
				CformProp* Prop = NULL;
				for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
				{
					Prop = ((CObjectLoad*)m_fmTkArry.GetAt(ii)->m_ObjLoad)->getProp();					
					if(Prop->group)
					{
						ObjRc.CopyRect(Prop->groupRc);
						m_view->ClientToScreen(ObjRc);
					}
					else
					{
						ObjRc.CopyRect(Prop->rect);
						m_view->ClientToScreen(ObjRc);
					}

					CPointerWnd* pPointerWnd = new CPointerWnd(m_view);
					pPointerWnd->CreateEx(WS_EX_LAYERED, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH)),
						NULL, WS_POPUP, ObjRc, m_view->GetParent(), NULL);
					::SetLayeredWindowAttributes(pPointerWnd->m_hWnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);
					pPointerWnd->Init(PenWidth, PenStyle);
					pPointerWnd->SaveRect(ObjRc);
					pPointerWnd->ShowWindow(SW_SHOWNOACTIVATE);
					m_fmWndArry.Add(pPointerWnd);
				}
			}
			else // 선택되 있지 않은 오브젝트에서 마우스 다운이 일어남
			{
				CformProp* Prop = NULL;
				for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
				{
					Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
					Prop->block = false;
				}

				for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
				{
					CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
					delete fmTracker;
				}
				m_fmTkArry.RemoveAll();

				CWnd* Wnd = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))->getWnd();
				GetWindowRect(Wnd->m_hWnd, ObjRc);

				CPointerWnd* pPointerWnd = new CPointerWnd(m_view);
				pPointerWnd->CreateEx(WS_EX_LAYERED, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH)),
					NULL, WS_POPUP, ObjRc, m_view->GetParent(), NULL);
				::SetLayeredWindowAttributes(pPointerWnd->m_hWnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);
				pPointerWnd->Init(PenWidth, PenStyle);
				pPointerWnd->SaveRect(ObjRc);
				pPointerWnd->ShowWindow(SW_SHOWNOACTIVATE);
				m_fmWndArry.Add(pPointerWnd);
			}
		}
		else // 모든 오브젝트가 선택되있지 않은 상태에서 오브젝트에 마우스다운이 일어남
		{
			if (m_view->m_nCursor == 0) ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			CformProp* Prop = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))->getProp();
			if(Prop->group)
			{
				ObjRc.CopyRect(Prop->groupRc);
				m_view->ClientToScreen(ObjRc);
			}
			else
			{
				ObjRc.CopyRect(Prop->rect);
				m_view->ClientToScreen(ObjRc);
			}

			CPointerWnd* pPointerWnd = new CPointerWnd(m_view);
			pPointerWnd->CreateEx(WS_EX_LAYERED, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH)),
				NULL, WS_POPUP, ObjRc, m_view->GetParent(), NULL);
			::SetLayeredWindowAttributes(pPointerWnd->m_hWnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);
			pPointerWnd->Init(PenWidth, PenStyle);
			pPointerWnd->SaveRect(ObjRc);
			pPointerWnd->ShowWindow(SW_SHOWNOACTIVATE);
			m_fmWndArry.Add(pPointerWnd);
		}
	}
	else // 오브젝트가 아닌 곳에서 마우스 다운이 일어남
	{
		CformProp* Prop = NULL;
		for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
			Prop->block = false;
		}

		for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
		{
			CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
			delete fmTracker;
		}
		m_fmTkArry.RemoveAll();
		POINT pt;
		pt = point;
		ClientToScreen(m_view->m_hWnd, &pt);
		ObjRc.left = ObjRc.right = pt.x;
		ObjRc.top = ObjRc.bottom = pt.y;

		m_FormRc.left = m_FormRc.right = point.x;
		m_FormRc.top = m_FormRc.bottom = point.y;
		m_startPt = m_toPt = point;
		PenWidth = 1;
		//if(m_selTool == _T("Pointer")) PenStyle = PS_DOT;
		//else PenStyle = PS_SOLID;
		PenStyle = PS_USERSTYLE;

		m_FstRc.CopyRect(ObjRc);
		CPointerWnd* pPointerWnd = new CPointerWnd(m_view);
		pPointerWnd->CreateEx(WS_EX_LAYERED, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH)),
			NULL, WS_POPUP, ObjRc, m_view->GetParent(), NULL);
		::SetLayeredWindowAttributes(pPointerWnd->m_hWnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);
		pPointerWnd->Init(PenWidth, PenStyle);
		pPointerWnd->SaveRect(ObjRc);
		pPointerWnd->ShowWindow(SW_SHOWNOACTIVATE);
		m_fmWndArry.Add(pPointerWnd);

		if (m_pSmartGLineMngr == NULL)
		{
			m_pSmartGLineMngr = new CSmartGLineMngr(m_view);
		}
		m_pSmartGLineMngr->CheckSmartGLine(m_bObjSel, m_FormRc, ObjRc);
	}
}

void CFmCtrl::MouseMove(CPoint point)
{
	if (m_bShiftKey)
	{
		return;
	}
	CRect rc;
	if (m_selTool == _T("Pointer") && m_bObjSel > -1)
	{
		if (!(::GetKeyState(VK_CONTROL) & 0x8000))
		{
			point.x = ((int)(point.x / m_nGap)) * m_nGap;
			point.y = ((int)(point.y / m_nGap)) * m_nGap;
		}
		if (m_fmTkArry.GetCount() > 1)
		{
			for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
			{
				rc.CopyRect(((CPointerWnd*)m_fmWndArry.GetAt(ii))->LoadRect());
				rc.OffsetRect(point.x - m_toPt.x, point.y - m_toPt.y);
				((CPointerWnd*)m_fmWndArry.GetAt(ii))->MoveWindow(rc);
				((CPointerWnd*)m_fmWndArry.GetAt(ii))->Redraw();
				((CPointerWnd*)m_fmWndArry.GetAt(ii))->SaveRect(rc);
			}
			m_toPt = point;
		}
		else
		{
			if(m_fmTkArry.GetCount() > 0)
			{
				CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
				CformProp* Prop = (CformProp*)fmTracker->m_ObjLoad->getProp();
				if(Prop->group && m_view->m_nCursor != 0) return ;//그룹 일 경우 싸이즈 조정 불가

				if (m_iDownEv != -1 && (Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE || Prop->kind == fmTAB))
				{  //오브젝트의 헤더를 마우스다운 한 상태에서 드래그가 일어난경우
					CfmObject* Obj = fmTracker->m_ObjLoad->getObject();
					m_toPt = point;
					int iMoveEv = Obj->receiveBuilder(Prop->kind, MAKEWPARAM(MOUSEMOVE_EV, 0), MAKELPARAM(m_toPt.x - Prop->rect.left, m_toPt.y - Prop->rect.top));
					if (iMoveEv != -1)
					{
						if (m_iMoveEv != iMoveEv)
						{
							if (Prop->kind != fmTAB)
							{
								m_iMoveEv = iMoveEv;
								MouseDrag();
							}
							else
							{
								AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SWAPTABPAGE, m_view->m_selF), MAKELPARAM(m_iDownEv, iMoveEv));
								m_iDownEv = m_iMoveEv = iMoveEv;
							}
						}
						//if (m_markWnd && !m_markWnd->IsWindowVisible()) m_markWnd->ShowWindow(SW_SHOW);
					}
					else
					{
						//if (m_markWnd && m_markWnd->IsWindowVisible()) m_markWnd->ShowWindow(SW_HIDE);
					}
					return;
				}
			}
			rc.CopyRect(((CPointerWnd*)m_fmWndArry.GetAt(0))->LoadRect());
			CPoint pt;
			pt.x = point.x - m_toPt.x;
			pt.y = point.y - m_toPt.y;
			MoveRect(rc, pt);
			MoveRect(m_FormRc, pt);
			
			((CPointerWnd*)m_fmWndArry.GetAt(0))->MoveWindow(rc);
			((CPointerWnd*)m_fmWndArry.GetAt(0))->Redraw();
			((CPointerWnd*)m_fmWndArry.GetAt(0))->SaveRect(rc);
			if (m_view->m_nCursor == 0) m_strTip.Format("(%d, %d, %d, %d)", m_FormRc.left, m_FormRc.top, m_FormRc.right, m_FormRc.bottom);
			else m_strTip.Format("(%d, %d)", m_FormRc.right, m_FormRc.bottom);
			AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_MOUSETIP, 1), (LPARAM)m_strTip.operator LPCTSTR());

			if (m_pSmartGLineMngr == NULL)
			{
				m_pSmartGLineMngr = new CSmartGLineMngr(m_view);
			}
			m_pSmartGLineMngr->DelSGLineWnd(m_FormRc.left, m_FormRc.right, m_FormRc.top, m_FormRc.bottom);
			m_pSmartGLineMngr->CheckSmartGLine(m_bObjSel, m_FormRc, rc);

			m_toPt = point;
		}
	}
	else
	{
		m_pt = point;
		ClientToScreen(m_view->m_hWnd, &m_pt);
		rc.CopyRect(((CPointerWnd*)m_fmWndArry.GetAt(0))->LoadRect());
		if (m_FstRc.left < m_pt.x)
		{
			rc.left = m_FstRc.left;
			rc.right = m_pt.x;
			m_FormRc.left = m_startPt.x;
			m_FormRc.right = point.x;
		}
		else if (m_FstRc.left > m_pt.x)
		{
			rc.right = m_FstRc.left;
			rc.left = m_pt.x;
			m_FormRc.left = point.x;
			m_FormRc.right = m_startPt.x;
		}
		if (m_FstRc.bottom < m_pt.y)
		{
			rc.top = m_FstRc.top;
			rc.bottom = m_pt.y;
			m_FormRc.top = m_startPt.y;
			m_FormRc.bottom = point.y;
		}
		else if (m_FstRc.bottom > m_pt.y)
		{
			rc.bottom = m_FstRc.top;
			rc.top = m_pt.y;
			m_FormRc.top = point.y;
			m_FormRc.bottom = m_startPt.y;
		}
		((CPointerWnd*)m_fmWndArry.GetAt(0))->MoveWindow(rc);
		((CPointerWnd*)m_fmWndArry.GetAt(0))->Redraw();
		((CPointerWnd*)m_fmWndArry.GetAt(0))->SaveRect(rc);

		if (m_pSmartGLineMngr == NULL)
		{
			m_pSmartGLineMngr = new CSmartGLineMngr(m_view);
		}
		m_pSmartGLineMngr->DelSGLineWnd(m_FormRc.left, m_FormRc.right, m_FormRc.top, m_FormRc.bottom);
		m_pSmartGLineMngr->CheckSmartGLine(m_bObjSel, m_FormRc, rc);

		m_toPt = point;
	}
}

void CFmCtrl::MouseUp()
{
	::ReleaseCapture();

	for (size_t ii = m_fmWndArry.GetCount(); ii > 0; ii--)
	{
		CPointerWnd* PointerWnd = m_fmWndArry.GetAt(ii - 1);
		PointerWnd->DestroyWindow();
		delete PointerWnd;
	}
	m_fmWndArry.RemoveAll();

	if (m_pSmartGLineMngr)
	{
		delete m_pSmartGLineMngr;
		m_pSmartGLineMngr = NULL;
	}

	if (m_selTool == _T("Pointer") && m_bObjSel > -1) //오브젝트에서 마우스 업
	{
		if ((m_toPt.x - m_startPt.x) + (m_toPt.y - m_startPt.y) != 0)
		{
			AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
		}
		if (m_bShiftKey)
		{
			CformProp* Prop = NULL;
			CfmTracker* fmTracker = NULL;
			CRect rc;
			CObjectLoad* ObjLoad = NULL;

			for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
			{
				CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
				delete fmTracker;
			}
			m_fmTkArry.RemoveAll();

			m_view->m_selF = m_bObjSel;
			for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
			{
				Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
				ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1);
				
				if (Prop->block)
				{
					rc.CopyRect(Prop->rect);
					CfmTracker* fmTracker = new CfmTracker(m_view, ObjLoad);
					fmTracker->SetPostion(rc.left, rc.top, rc.Width(), rc.Height());
					if (m_bObjSel == ii - 1)
						fmTracker->SetFst(true);
					else
						fmTracker->SetFst(false);
					fmTracker->Init();
					fmTracker->MoveTracker();
					fmTracker->Redraw();

					m_fmTkArry.Add(fmTracker);
				}
			}
		}
		else if (m_fmTkArry.GetCount() == 1) // 단일 선택이였을 경우
		{
			CformProp* Prop = ((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getProp();
			CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
			
			CPoint point;
			if(Prop->group) ///그룹일 경우
			{
				CPoint point;
				point.x = (m_toPt.x - m_startPt.x);
				point.y = (m_toPt.y - m_startPt.y);
				Prop->groupRc.OffsetRect(point);

				CObjectLoad* ObjLoad = NULL;
				CformProp* groupProp = NULL;
				CWnd* Wnd = NULL;
				CfmObject* Obj = NULL;
				for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
				{
					ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii-1);
					groupProp = ObjLoad->getProp();
					Wnd = ObjLoad->getWnd();
					if (groupProp->group == Prop)
					{
						Obj = ObjLoad->getObject();
						groupProp->rect.OffsetRect(point);
						//Wnd->ShowWindow(SW_HIDE);
						InvalidateRect(m_view->m_hWnd, Prop->rect, FALSE);
						Wnd->MoveWindow(groupProp->rect);
						Obj->Reload();
					}
				}

				fmTracker->SetPostion(Prop->groupRc.left - 5, Prop->groupRc.top - 5, Prop->groupRc.Width() + 10, Prop->groupRc.Height() + 10);
				fmTracker->MoveTracker();
				fmTracker->Redraw();

				m_view->m_selF = m_bObjSel;
			}
			else ///그룹이 아닐경우
			{
				CfmObject* Obj = ((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getObject();
				point.x = (m_toPt.x - m_startPt.x);
				point.y = (m_toPt.y - m_startPt.y);

				if (m_iDownEv != -1 && (Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE || Prop->kind == fmTAB))
				{// 오브젝트의 헤더를 마우스다운한 상태에서 마우스업 일 경우
					KillTimer(m_view->m_hWnd, TM_DRAG);
					if (m_markWnd)
					{
						m_markWnd->DestroyWindow();
						delete m_markWnd;
						m_markWnd = NULL;
					}

					CformProp* Prop = ((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getProp();
					if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX || Prop->kind == fmTABLE)
					{
						if (m_iMoveEv != m_iDownEv && m_iMoveEv != -1)
						{
							//int iUpEv = Obj->receiveBuilder(Prop->kind, MAKEWPARAM(HEADSEL_EV, 0), MAKELPARAM((m_iMoveEv - 1) == -1 ? 0 : (m_iMoveEv - 1), 0));
							if (m_iMoveEv > m_iDownEv)
							{
								Obj->receiveBuilder(Prop->kind, MAKEWPARAM(HEADSEL_EV, 0), MAKELPARAM((m_iMoveEv - 1) == -1 ? 0 : (m_iMoveEv - 1), 0));
							}
							else
							{
								if (m_iMoveEv == 0) Obj->receiveBuilder(Prop->kind, MAKEWPARAM(HEADSEL_EV, 0), MAKELPARAM(m_iMoveEv, 0));
							}
							Obj->Reload();
							if (m_iDownEv != m_iMoveEv && m_iDownEv != m_iMoveEv - 1)
							{
								AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SWAPCELL, m_view->m_selF), MAKELPARAM(m_iDownEv, m_iMoveEv));
							}
						}
					}
					else // fmTAB
					{
						if (m_iMoveEv != m_iDownEv || m_iMoveEv != -1)
						{
							//AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_REDRAWCTRL, FALSE), m_view->m_selF);
							if (m_iDownEv != m_iMoveEv && m_iDownEv != m_iMoveEv - 1)
							{
								AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SWAPTABPAGE, m_view->m_selF), MAKELPARAM(m_iDownEv, m_iMoveEv));
							}
						}
					}
				}
				else
				{
					CWnd* Wnd = ((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getWnd();
					CRect rc;
					rc.CopyRect(Prop->rect);
					MoveRect(Prop->rect, point);
					if (Prop->kind == fmTABLE && !(Prop->auxAttributes & gaAUTOSIZE))
					{
						Prop->rect.bottom = Prop->rect.top + (LOWORD(Prop->keyValues) * Prop->cellProperties->GetCount()) + Prop->cellProperties->GetCount() + 1;
					}
					else if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
					{
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_OBJSCROLL, (LPARAM)(CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad);
					}
					Prop->rect.NormalizeRect();
					//Wnd->ShowWindow(SW_HIDE);
					InvalidateRect(m_view->m_hWnd, Prop->rect, FALSE);
					Wnd->MoveWindow(Prop->rect);
					Obj->Reload();
					///////////////////////////////////////////////////////////////////////////////////////////////// 폼 오브젝트 싸이즈 변경시 다시 그림
					int rcSize = rc.Width() + rc.Height();
					int PropSize = Prop->rect.Width() + Prop->rect.Height();
					if (Prop->kind == fmFORM && rcSize != PropSize)
						AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_FORMRESIZE, (LPARAM)(CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad);
					/////////////////////////////////////////////////////////////////////////////////////////////////

					fmTracker->SetPostion(Prop->rect.left - 5, Prop->rect.top - 5, Prop->rect.Width() + 10, Prop->rect.Height() + 10);
					fmTracker->MoveTracker();
					fmTracker->Redraw();
					m_view->m_selF = m_bObjSel;
				}
			}
		}
		else if (m_fmTkArry.GetCount() > 0) // 다중선택 이였을 경우
		{
			CfmObject* Obj = NULL;
			CformProp* Prop = NULL;
			CWnd* Wnd = NULL;
			CfmTracker* fmTracker = NULL;
			CRect rc;
			CPoint point;

			CObjectLoad* ObjLoad = NULL;
			CformProp* groupProp = NULL;
			CWnd* groupWnd = NULL;
			CformProp* selProp = NULL;
			CPoint pt;

			for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
			{
				Obj = ((CObjectLoad*)m_fmTkArry.GetAt(ii)->m_ObjLoad)->getObject();
				Prop = ((CObjectLoad*)m_fmTkArry.GetAt(ii)->m_ObjLoad)->getProp();
				Wnd = ((CObjectLoad*)m_fmTkArry.GetAt(ii)->m_ObjLoad)->getWnd();
				fmTracker = m_fmTkArry.GetAt(ii);

				point.x = (m_toPt.x - m_startPt.x);
				point.y = (m_toPt.y - m_startPt.y);
				Prop->groupRc.OffsetRect(point);

				if(Prop->group) ///그룹일 경우
				{
					for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
					{
						ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii-1);
						groupProp = ObjLoad->getProp();
						groupWnd = ObjLoad->getWnd();
						if (Prop == groupProp->group) // 현재 오브젝트와 같은 그룹일 경우
						{
							rc.CopyRect(groupProp->rect);
							rc.OffsetRect(point);
							groupProp->rect.CopyRect(rc);
							//Wnd->ShowWindow(SW_HIDE);
							InvalidateRect(m_view->m_hWnd, groupProp->rect, TRUE);
							groupWnd->MoveWindow(rc);
							Obj->Reload();
						}
					}

					selProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))->getProp();
					if (Prop == selProp) // 그룹일 경우 기준 오브젝트을 클릭된 오브젝트로 변경
					{
						m_view->m_selF = m_bObjSel;
						fmTracker->SetFst(true);
					}
					else
					{
						fmTracker->SetFst(false);
					}

					fmTracker->SetPostion(Prop->groupRc.left, Prop->groupRc.top, Prop->groupRc.Width(), Prop->groupRc.Height());
					fmTracker->MoveTracker();
					fmTracker->Redraw();
				}
				else //그룹이 아닐 경우
				{
					rc.CopyRect(Prop->rect);
					MoveRect(rc, point);

					if(Prop->kind == fmTABLE && !(Prop->auxAttributes & gaAUTOSIZE))
					{
						Prop->rect.bottom = Prop->rect.top + (LOWORD(Prop->keyValues) * Prop->cellProperties->GetCount()) + Prop->cellProperties->GetCount() + 1;
					}
					Prop->rect.CopyRect(rc);
					//Wnd->ShowWindow(SW_HIDE);
					InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
					Wnd->MoveWindow(rc);
					Obj->Reload();
					
					pt.x = m_toPt.x - m_startPt.x;
					pt.y = m_toPt.y - m_startPt.y;

					selProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel))->getProp();
					if(Prop == selProp) // 그룹일 경우 기준 오브젝트을 클릭된 오브젝트로 변경
					{
						m_view->m_selF = m_bObjSel;
						fmTracker->SetFst(true);
					}
					else
					{
						fmTracker->SetFst(false);
					}
					fmTracker->SetPostion(Prop->rect.left, Prop->rect.top, Prop->rect.Width(), Prop->rect.Height());
					fmTracker->MoveTracker();
					fmTracker->Redraw();
				}
			}
			//m_view->m_selF = m_bObjSel;
			//m_bObjSel = -1;
		}
		else // 모든 오브젝트가 셀렉트 되있던 경우가 아니라면
		{
			CObjectLoad* ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(m_bObjSel);
			CformProp* Prop = (CformProp*)ObjLoad->getProp();
			CfmObject* Obj = ObjLoad->getObject();
			CWnd* Wnd = ObjLoad->getWnd();

			CRect rc;
			if(Prop->group) ///그룹일 경우
			{
				rc.CopyRect(Prop->groupRc);
				rc.left += (m_toPt.x - m_startPt.x);
				rc.top += (m_toPt.y - m_startPt.y);
				rc.right += (m_toPt.x - m_startPt.x);
				rc.bottom += (m_toPt.y - m_startPt.y);
				Prop->groupRc.CopyRect(rc);

				CPoint point;
				point.x = (m_toPt.x - m_startPt.x);
				point.y = (m_toPt.y - m_startPt.y);
				CObjectLoad* ObjLoad = NULL;
				CformProp* groupProp = NULL;
				CWnd* groupWnd = NULL;
				CfmObject* Obj = NULL;
				for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
				{
					ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii-1);
					groupProp = ObjLoad->getProp();
					groupWnd = ObjLoad->getWnd();
					if (groupProp->group == Prop)
					{
						Obj = ObjLoad->getObject();
						groupProp->rect.OffsetRect(point);
						//Wnd->ShowWindow(SW_HIDE);
						InvalidateRect(m_view->m_hWnd, groupProp->rect, FALSE);
						groupWnd->MoveWindow(groupProp->rect);
						Obj->Reload();
					}
				}
			}
			else
			{
				rc.CopyRect(Prop->rect);
				rc.left += (m_toPt.x - m_startPt.x);
				rc.top += (m_toPt.y - m_startPt.y);
				rc.right += (m_toPt.x - m_startPt.x);
				rc.bottom += (m_toPt.y - m_startPt.y);

				if(Prop->kind == fmTABLE && !(Prop->auxAttributes & gaAUTOSIZE) && !Prop->group)
				{
					Prop->rect.bottom = Prop->rect.top + (LOWORD(Prop->keyValues) * Prop->cellProperties->GetCount()) + Prop->cellProperties->GetCount() + 1;
				}
				Prop->rect.CopyRect(rc);
				//Wnd->ShowWindow(SW_HIDE);
				InvalidateRect(m_view->m_hWnd, Prop->rect, FALSE);
				Wnd->MoveWindow(rc);
				Obj->Reload();
			}
			m_view->m_selF = m_bObjSel;

			CformProp* pProp = NULL;
			for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
			{
				pProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
				pProp->block = false;
			}

			for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
			{
				CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
				delete fmTracker;
			}
			m_fmTkArry.RemoveAll();
			
			CfmTracker* fmTracker = new CfmTracker(m_view, ObjLoad);
			fmTracker->SetPostion(rc.left - 5, rc.top - 5, rc.Width() + 10, rc.Height() + 10);
			fmTracker->SetFst(true);
			fmTracker->Init();
			m_fmTkArry.Add(fmTracker);
			Prop->block = true;
		}
	}
	else if (m_selTool == _T("Pointer")) // 오브젝트가 아닌 곳에서 마우스 업
	{
		m_view->m_selF = -1;

		CformProp* Prop = NULL;
		for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
			Prop->block = false;
		}

		for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
		{
			CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
			delete fmTracker;
		}
		m_fmTkArry.RemoveAll();

		for (size_t ii = 0; ii < (size_t)m_view->m_mapH->forms.GetCount(); ii++)
		{
			Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getProp();
			if (m_FormRc.PtInRect(Prop->rect.TopLeft()) && m_FormRc.PtInRect(Prop->rect.BottomRight()))
			{
				if(Prop->group)
				{
					if(Prop->group == Prop)
					{
						Prop->block = true;
						CfmTracker* fmTracker = new CfmTracker(m_view, (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii));
						m_fmTkArry.Add(fmTracker);
						if (m_view->m_selF == -1)
						{
							m_view->m_selF = ii;
						}
					}
				}
				else
				{
					CfmTracker* fmTracker = new CfmTracker(m_view, (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii));
					m_fmTkArry.Add(fmTracker);
					Prop->block = true;
					if (m_view->m_selF == -1)
					{
						m_view->m_selF = ii;
					}
				}
			}
		}

		CfmTracker* fmTracker = NULL;
		if (m_fmTkArry.GetCount() == 1)
		{
			fmTracker = m_fmTkArry.GetAt(0);
			Prop = fmTracker->m_ObjLoad->getProp();
			if(Prop->group)
			{
				fmTracker->SetPostion(Prop->groupRc.left - 5, Prop->groupRc.top - 5, Prop->groupRc.Width() + 10, Prop->groupRc.Height() + 10);
			}
			else
			{
				fmTracker->SetPostion(Prop->rect.left - 5, Prop->rect.top - 5, Prop->rect.Width() + 10, Prop->rect.Height() + 10);
			}
			fmTracker->SetFst(true);
			fmTracker->Init();
			fmTracker->MoveTracker();
			fmTracker->Redraw();
		}
		else if (m_fmTkArry.GetCount() > 1)
		{
			bool bFst = true;
			for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
			{
				fmTracker = m_fmTkArry.GetAt(ii);
				Prop = fmTracker->m_ObjLoad->getProp();
				if(Prop->group)
				{
					fmTracker->SetPostion(Prop->groupRc.left, Prop->groupRc.top, Prop->groupRc.Width(), Prop->groupRc.Height());
				}
				else
				{
					fmTracker->SetPostion(Prop->rect.left, Prop->rect.top, Prop->rect.Width(), Prop->rect.Height());
				}
				
				if (bFst)
				{
					fmTracker->SetFst(true);
					bFst = false;
				}
				fmTracker->Init();
				fmTracker->MoveTracker();
				fmTracker->Redraw();
			}
		}
		else
		{
			m_view->m_selF = -1;
		}
	}
	else // 오브젝트 새로 생성될 때
	{
		CformProp* Prop = NULL;
		for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
			Prop->block = false;

		}
		for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
		{
			CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
			delete fmTracker;
		}
		m_fmTkArry.RemoveAll();
		Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_mapH->forms.GetCount()-1))->getProp();
		CfmTracker* fmTracker = new CfmTracker(m_view, (CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_mapH->forms.GetCount()-1));
		m_fmTkArry.Add(fmTracker);
		Prop->block = true;

		CRect rc = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_mapH->forms.GetCount() - 1))->GetRect();
		fmTracker->SetPostion(rc.left - 5, rc.top - 5, rc.Width() + 10, rc.Height() + 10);
		fmTracker->SetFst(true);
		fmTracker->Init();
		fmTracker->MoveTracker();
		fmTracker->Redraw();
	}

	//InvalidateObject();

	if (m_dc)
	{
		delete m_dc;
		m_dc = (CDC *)NULL;
	}
}

int CFmCtrl::ObjSelecte(CPoint point)
{
	if (m_view->m_nCursor != 0) return m_view->m_selF;

	int iObjSel = -1;
	CRect rc;
	bool bGroup = false;
	CformProp* groupProp = NULL;
	CformProp* Prop = NULL;
	for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
	{
		Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii-1))->getProp();
		if (Prop->group)
		{
			if(Prop->group == Prop)
			{
				Prop->group->groupRc;
				rc.CopyRect(Prop->group->groupRc);
				if (rc.PtInRect(point))
				{
					m_FormRc.CopyRect(rc);
					groupProp = Prop->group;
					bGroup = true;
					break;
				}
			}
		}
		else
		{
			rc.CopyRect(Prop->rect);	
			if (rc.PtInRect(point))
			{
				m_FormRc.CopyRect(rc);
				iObjSel = ii-1;
				break;
			}
		}
	}

	if(bGroup)
	{
		for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii-1))->getProp();
			if(Prop == groupProp)
			{
				iObjSel = ii-1;
			}
		}
	}
	return iObjSel;
}

CArray <CfmTracker*, CfmTracker*> *CFmCtrl::GetTkArray()
{
	return &m_fmTkArry;
}

void CFmCtrl::MoveRect(CRect& rc, CPoint point)
{
	if (m_view->m_nCursor != 0)
	{
		CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
		switch (fmTracker->m_nHit)
		{
		case 0:
			rc.left += point.x;
			rc.top += point.y;
			break;
		case 1:
			rc.top += point.y;
			break;
		case 2:
			rc.right += point.x;
			rc.top += point.y;
			break;
		case 3:
			rc.right += point.x;
			break;
		case 4:
			rc.right += point.x;
			rc.bottom += point.y;
			break;
		case 5:
			rc.bottom += point.y;
			break;
		case 6:
			rc.left += point.x;
			rc.bottom += point.y;
			break;
		case 7:
			rc.left += point.x;
			break;
		}
	}
	else
	{
		rc.OffsetRect(point.x, point.y);
	}
}

void CFmCtrl::ReDrawTracker()
{
	for (int ii = 0; ii < m_fmTkArry.GetCount(); ii++)
	{
		CfmTracker* fmTracker = m_fmTkArry.GetAt(ii);
		CformProp* Prop = (CformProp*)(fmTracker->m_ObjLoad)->getProp();
		fmTracker->SetPostion(Prop->rect.left, Prop->rect.top, Prop->rect.Width(), Prop->rect.Height());
		fmTracker->MoveTracker();
		fmTracker->Redraw();
	}
}

void CFmCtrl::SetTracker(int idx)
{
	CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getProp();

	fmTracker->SetPostion(Prop->rect.left - 5, Prop->rect.top - 5, Prop->rect.Width() + 10, Prop->rect.Height() + 10);
	fmTracker->MoveTracker();
	fmTracker->Redraw();
}

void CFmCtrl::SelObj(int idx, bool bClear)
{
	if (bClear)
	{
		CformProp* Prop = NULL;
		for (size_t ii = m_view->m_mapH->forms.GetCount(); ii > 0; ii--)
		{
			Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii - 1))->getProp();
			Prop->block = false;
		}

		if (m_view->m_mapH->forms.GetCount() <= idx) return;

		for (size_t ii = m_fmTkArry.GetCount(); ii > 0; ii--)
		{
			CfmTracker* fmTracker = m_fmTkArry.GetAt(ii - 1);
			delete fmTracker;
		}
		m_fmTkArry.RemoveAll();
	}

	if (idx < 0) return;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(idx);
	CfmObject*	Obj = ObjLoad->getObject();
	CformProp*	Prop = ObjLoad->getProp();
	CWnd*		Wnd = ObjLoad->getWnd();

	CRect rc(Prop->rect);
	if(Prop->group)
	{
		if (Prop->group != Prop) return;
		Prop = Prop->group;
		rc.CopyRect(Prop->groupRc);
	}
	
	CfmTracker* fmTracker = new CfmTracker(m_view, ObjLoad);

	fmTracker->SetPostion(rc.left - 5, rc.top - 5, rc.Width() + 10, rc.Height() + 10);

	if(m_fmTkArry.GetCount() == 0)
		fmTracker->SetFst(true);
	fmTracker->Init();
	m_fmTkArry.Add(fmTracker);
	Prop->block = true;
}

void CFmCtrl::SelAddObj(int idx, bool bMulti)
{
	if (idx < 0) return;

	CObjectLoad*	ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(idx);
	CfmObject*	Obj = ObjLoad->getObject();
	CformProp*	Prop = ObjLoad->getProp();
	CWnd*		Wnd = ObjLoad->getWnd();

	CRect rc(Prop->rect);
	if (Prop->group)
	{
		if (Prop->group != Prop) return;
		Prop = Prop->group;
		rc.CopyRect(Prop->groupRc);
	}

	CfmTracker* fmTracker = new CfmTracker(m_view, ObjLoad);

	if (bMulti)
	{
		fmTracker->SetPostion(rc.left, rc.top, rc.Width(), rc.Height());
	}
	else
	{
		fmTracker->SetPostion(rc.left - 5, rc.top - 5, rc.Width() + 10, rc.Height() + 10);
	}

	if (m_fmTkArry.GetCount() == 0)
		fmTracker->SetFst(true);
	fmTracker->Init();
	m_fmTkArry.Add(fmTracker);
	Prop->block = true;
}

void CFmCtrl::LayoutMoveObj()
{
	CObjectLoad*	ObjLoad = NULL;
	CformProp*	Prop = NULL;
	CWnd*		Wnd = NULL;

	for (int ii = 0; ii < m_view->m_mapH->forms.GetCount(); ii++)
	{
		ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii);
		Prop = ObjLoad->getProp();
		Wnd = ObjLoad->getWnd();
		InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
		Wnd->MoveWindow(Prop->rect);
		//Obj->Reload();
	}
}

void CFmCtrl::MoveObj(CRect rc)
{
	CfmObject* Obj = ((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getObject();
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getProp();
	CWnd* Wnd = ((CObjectLoad*)m_fmTkArry.GetAt(0)->m_ObjLoad)->getWnd();
	CfmTracker* fmTracker = m_fmTkArry.GetAt(0);

	Prop->rect.CopyRect(rc);
	//Wnd->ShowWindow(SW_HIDE);
	InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
	Wnd->MoveWindow(rc);
	Obj->Reload();
	//Wnd->ShowWindow(SW_SHOW);
	//InvalidateObject();

	fmTracker->SetPostion(Prop->rect.left - 5, Prop->rect.top - 5, Prop->rect.Width() + 10, Prop->rect.Height() + 10);
	fmTracker->MoveTracker();
	fmTracker->Redraw();
}

void CFmCtrl::SetObjRect(CObjectLoad* ObjLoad, CRect rc)
{
	CfmObject* Obj = ObjLoad->getObject();
	CformProp* Prop = ObjLoad->getProp();
	CWnd* Wnd = ObjLoad->getWnd();

	Prop->rect.CopyRect(rc);
	//Wnd->ShowWindow(SW_HIDE);
	InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
	Wnd->MoveWindow(rc);
	Obj->Reload();
	//Wnd->ShowWindow(SW_SHOW);
	//InvalidateObject();

	if (m_fmTkArry.GetCount() > 0)
	{
		CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
		fmTracker->SetPostion(Prop->rect.left - 5, Prop->rect.top - 5, Prop->rect.Width() + 10, Prop->rect.Height() + 10);
		fmTracker->MoveTracker();
		fmTracker->Redraw();
	}
}


void CFmCtrl::SetGroup()
{
	bool bGroup = false;
	CformProp* groupProp = NULL;
	for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)		// 선택된 오브젝트중에 그룹이 있는지 체크
	{
		groupProp = m_fmTkArry.GetAt(ii)->m_ObjLoad->getProp();
		if(groupProp->group)
		{
			bGroup = true;
			break;
		}
	}
	if(bGroup) // 선택된 오브젝트 중에 그룹이 있으면 그룹 해제됨 경고메세지
	{
		if(MessageBox(NULL,_T("It is Lost that Group Information in Group!\n\nDo you make Group?"),_T("Builder"), MB_YESNO) == IDNO)
		{
			return ;
		}
	}

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);

	CformProp* Prop = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_selF))->getProp();
	CRect rc(0,0,0,0);
	CfmTracker* fmTracker = NULL;
	CformProp* subProp = NULL;
	for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
	{
		fmTracker = m_fmTkArry.GetAt(ii);
		groupProp = fmTracker->m_ObjLoad->getProp();
		if(groupProp->group)
		{
			groupProp->groupRc.SetRectEmpty();
			for (size_t ii = 0; ii < (size_t)m_view->m_mapH->forms.GetCount(); ii++)
			{
				subProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getProp();
				if(groupProp == subProp->group)
				{
					subProp->group = Prop;
					rc.UnionRect(rc,subProp->rect);
				}
			}
		}
		groupProp->group = Prop;
		rc.UnionRect(rc,groupProp->rect);

	}
	Prop->groupRc.CopyRect(rc);
	SelObj(m_view->m_selF);
}

void CFmCtrl::ReleaseGroup()
{
	SelObj(-1);
	CformProp* Prop = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_selF))->getProp();
	for (size_t ii = 0; ii < (size_t)m_view->m_mapH->forms.GetCount(); ii++)
	{
		CformProp* groupProp = (CformProp*)((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getProp();
		if(Prop == groupProp->group)
		{
			groupProp->group = NULL;

			CfmTracker* fmTracker = new CfmTracker(m_view, (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii));
			m_fmTkArry.Add(fmTracker);
			if (m_view->m_selF == -1)
			{
				m_view->m_selF = ii;
			}
		}
	}
	Prop->groupRc.SetRectEmpty();

	bool bFst = true;
	CfmTracker* fmTracker = NULL;
	for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
	{
		fmTracker = m_fmTkArry.GetAt(ii);
		Prop = fmTracker->m_ObjLoad->getProp();
		fmTracker->SetPostion(Prop->rect.left, Prop->rect.top, Prop->rect.Width(), Prop->rect.Height());
				
		if (bFst)
		{
			fmTracker->SetFst(true);
			bFst = false;
		}
		fmTracker->Init();
		fmTracker->MoveTracker();
		fmTracker->Redraw();
	}
}

void CFmCtrl::SetObjRect(int left, int top, int right, int bottom)
{
	CfmTracker* fmTracker = NULL;
	CfmObject* Obj = NULL;
	CformProp* Prop = NULL;
	CWnd* Wnd = NULL;

	CformProp* subProp = NULL;
	CWnd* subWnd = NULL;
	CfmObject* sbuObj = NULL;
	int wCnt = 0;
	int hCnt = 0;
	for (size_t ii = 0; ii < (size_t)m_fmTkArry.GetCount(); ii++)
	{
		fmTracker = m_fmTkArry.GetAt(ii);
		Obj = fmTracker->m_ObjLoad->getObject();
		Prop = fmTracker->m_ObjLoad->getProp();
		Wnd = fmTracker->m_ObjLoad->getWnd();
		if(Prop->group)
		{
			wCnt = 0;
			hCnt = 0;
			for (size_t ii = 0; ii < (size_t)m_view->m_mapH->forms.GetCount(); ii++)
			{
				subProp = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getProp();
				subWnd = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getWnd();
				sbuObj = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getObject();
				if(Prop == subProp->group)
				{
					subProp->rect.OffsetRect(left,top);
					subProp->rect.right += right;
					subProp->rect.bottom += bottom;
					if (subProp->rect.left > subProp->rect.right)
					{
						subProp->rect.right = subProp->rect.left;
					}
					else
					{
						wCnt++;
					}
					if (subProp->rect.top > subProp->rect.bottom)
					{
						subProp->rect.bottom = subProp->rect.top;
					}
					else
					{
						hCnt++;
					}
					//Wnd->ShowWindow(SW_HIDE);
					InvalidateRect(m_view->m_hWnd, Prop->rect, FALSE);
					subWnd->MoveWindow(subProp->rect);
					sbuObj->Reload();
				}
			}
			if (wCnt == 0)
			{
				left = right = 0;
			}
			if (hCnt == 0)
			{
				top = bottom = 0;
			}

			Prop->groupRc.OffsetRect(left, top);
			Prop->groupRc.right += right;
			Prop->groupRc.bottom += bottom;
			if (Prop->groupRc.left > Prop->groupRc.right)
			{
				Prop->groupRc.right = Prop->groupRc.left;
				right = left;
			}
			if (Prop->groupRc.top > Prop->groupRc.bottom)
			{
				Prop->groupRc.bottom = Prop->groupRc.top;
				bottom = top;
			}
			fmTracker->ResizePostion(left, top, right, bottom);
			fmTracker->MoveTracker();
			fmTracker->Redraw();
		}
		else
		{
			Prop->rect.OffsetRect(left,top);
			Prop->rect.right += right;
			Prop->rect.bottom += bottom;
			if (Prop->rect.left > Prop->rect.right)
			{
				Prop->rect.right = Prop->rect.left;
				right = left;
			}
			if (Prop->rect.top > Prop->rect.bottom)
			{
				Prop->rect.bottom = Prop->rect.top;
				bottom = top;
			}
			
			if ((Prop->kind == fmGRID || Prop->kind == fmGRIDEX))
			{
				ShowVScroll(Prop);
			}
			//Wnd->ShowWindow(SW_HIDE);
			InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
			Wnd->MoveWindow(Prop->rect);
			Obj->Reload();

			fmTracker->ResizePostion(left, top, right, bottom);
			fmTracker->MoveTracker();
			fmTracker->Redraw();
		}
	}

	//InvalidateObject();
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 1);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 2);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 3);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_REFRESHPRO, 4);
}

void CFmCtrl::InvalidateObject()
{
	CObjectLoad* ObjLoad = NULL;
	CWnd* Wnd = NULL;
	CformProp* Prop = NULL;
	//m_view->ModifyStyle(0, WS_CLIPCHILDREN); // turn on WS_CLIPCHILDREN
	//m_view->LockWindowUpdate();

	CRect rc, InterRc;
	//if (m_view->m_selF > -1) rc = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(m_view->m_selF))->getProp()->rect;
	for (size_t ii = 0; ii < (size_t)m_view->m_mapH->forms.GetCount(); ii++)
	{
		ObjLoad = (CObjectLoad*)m_view->m_mapH->forms.GetAt(ii);
		Wnd = ObjLoad->getWnd();
		Prop = ObjLoad->getProp();
		//if (InterRc.IntersectRect(rc, Prop->rect))
		//if (Prop->block == true)
		if (m_view->m_selF > -1)
		{
			//if ((Prop->styles & stTRANSPARENT) || Prop->kind == fmBOX)
			if (Prop->block == true)
			{//		Wnd->ShowWindow(SW_HIDE);
				InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
				//Wnd->MoveWindow(Prop->rect);
				//Wnd->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			InvalidateRect(m_view->m_hWnd, Prop->rect, TRUE);
		}
	}
	//m_view->ModifyStyle(WS_CLIPCHILDREN, 0); // turn off WS_CLIPCHILDREN
	//m_view->Invalidate();
	//UpdateWindow(m_view->m_hWnd);

	//for (size_t ii = 0; ii < (size_t)m_view->m_mapH->forms.GetCount(); ii++)
	//{
	//	((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getWnd()->ShowWindow(SW_SHOW);
	//}
	//UpdateWindow(m_view->m_hWnd);
	//m_view->ShowWindow(SW_SHOW);s
	//UpdateWindow(m_view->m_hWnd);
	//m_view->UnlockWindowUpdate();
	//m_view->ModifyStyle(WS_CLIPCHILDREN, 0); // turn off WS_CLIPCHILDREN        
}

void CFmCtrl::MouseDrag()
{
	CfmTracker* fmTracker = m_fmTkArry.GetAt(0);
	CformProp* Prop = fmTracker->m_ObjLoad->getProp();
	if (/*m_view->m_bSelect &&*/ !m_markWnd)
	{
		m_markWnd = new CMarkWnd();
		if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
		{
			m_markWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 8, 5), m_view->GetParent(), 1);
			m_markWnd->SetTP(4);
		}
		else if (Prop->kind == fmTABLE)
		{
			m_markWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 5, 8), m_view->GetParent(), 1);
			m_markWnd->SetTP(3);
		}
	}

	if (Prop->kind == fmGRID || Prop->kind == fmGRIDEX)
	{
		cellOb* CellProp = NULL;
		int iPos = 0;
		for (int ii = 0; ii < Prop->cellProperties->GetCount(); ii++)
		{
			if (ii >= m_iMoveEv) break;
			CellProp = (cellOb*)Prop->cellProperties->GetAt(ii);
			iPos += CellProp->width;
		}
		m_markWnd->SetWindowPos(NULL, Prop->rect.left + iPos - 5 + 3, Prop->rect.top - 6, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
	else if (Prop->kind == fmTABLE)
	{
		cellOb* CellProp = NULL;
		int iPos = 0;
		iPos = Prop->rect.Height() / Prop->cellProperties->GetCount() * m_iMoveEv + 1;
		m_markWnd->SetWindowPos(NULL, Prop->rect.left - 6, Prop->rect.top + iPos - 5, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
	else // fmTAB
	{
		int iPos = 0;
		m_markWnd->SetWindowPos(NULL, Prop->rect.left + iPos - 5, Prop->rect.top - 6, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
}

void CFmCtrl::ShowVScroll(CformProp* Prop)
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