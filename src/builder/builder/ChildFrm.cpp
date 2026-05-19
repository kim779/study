
// ChildFrm.cpp : CChildFrame 클래스의 구현
//

#include "stdafx.h"
#include "axisBuilder.h"

#include "ChildFrm.h"
#include "MainFrm.h"
#include "VBScriptEdit.h"

#include "h/mainvar.h"

#include "awDlg/coRGB.h"
#include "awDlg/format.h"
#include "awTool/awObjectLoad.h"
#include "awTool/awFormCtrl.h"
#include "awTool/fmOrder.h"

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
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame(CMainFrame* frame, CString curdir)
{
	m_mainFrm = frame;
	m_dir   = curdir;

	m_delete = false;
	m_bookmarks.RemoveAll();
	m_undos.RemoveAll();
	m_redos.RemoveAll();
	m_CloneCnt = 0;
	m_OrderWnd = NULL;
}

CChildFrame::~CChildFrame()
{
	while(m_mapH.forms.GetCount() > 0)
	{
		m_view.deleteForm(0);
	}
	m_mapH.forms.RemoveAll();

	DeleteUndoMap();
	m_undos.RemoveAll();
	m_redos.RemoveAll();
	m_bookmarks.RemoveAll();
	if(m_OrderWnd) delete m_OrderWnd;
	
	m_mainFrm->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
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

	m_mapH.type     = VW_NORM;
	m_mapH.formN    = 0;
	m_mapH.pathsetted = false;
	m_mapH.modified   = false;
	m_mapH.source = m_mapH.compile = false;
	if (lpCreateStruct->lpszName && !strcmp(_T("Procedures"), lpCreateStruct->lpszName))
		m_mapH.mapK = MK_PROCEDURES;

	m_view.Init(&m_mapH, m_draw, m_palette, m_piForm, m_dir);
	GetSystemMenu(FALSE)->InsertMenu(0, MF_BYPOSITION|MF_STRING, 0, _T("MAPNAME"));
	return 0;
}


void CChildFrame::OnClose()
{
	if (!DeleteFiles())
		return;

	CMDIChildWndEx::OnClose();
}


void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 0;
	lpMMI->ptMinTrackSize.y = 0;
	lpMMI->ptMaxTrackSize.x = 3000;
	lpMMI->ptMaxTrackSize.y = 3000;

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
		m_view.SelObj(-1);
	}

	CMDIChildWndEx::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}


void CChildFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	ClearOrderNumber();
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
			m_view.Emulate();
			return;
		}
	}
	CMDIChildWndEx::OnSysCommand(nID, lParam);
}

BOOL CChildFrame::Create(CString wName, CRect rect, CAxisDraw* draw, CAxisPalette* palette, CiForm* piForm, int display)
{
	m_draw = draw;
	m_palette = palette;
	m_display = display;
	m_piForm = piForm;
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

void CChildFrame::OnTRLayOutDrop(int kind, CPoint pt)
{
	m_view.OnTRLayoutDrop(kind, pt);
}

bool CChildFrame::Copy()
{
	if (m_mapH.mapK == MK_PROCEDURES)
		return true;

	m_mainFrm->deleteAryCloneObj();

	int cnt = m_view.m_FmCtrl->GetSelCount();
	if (cnt >= 0 && m_view.m_selF >= 0)
	{
		m_CloneCnt = 0;
		CformProp* dstProp = NULL;
		CformProp* srcProp = NULL;
		CObjectLoad* srcObj = NULL;
		CMap<CformProp*, CformProp*, CformProp*, CformProp*> mapGropProp;
		for (size_t ii = 0; ii < (size_t)m_mapH.forms.GetCount(); ii++)
		{
			if (((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp()->block == true)
			{
				CObjectLoad* dstObj = NULL;
				srcObj = (CObjectLoad*)m_mapH.forms.GetAt(ii);
				m_mainFrm->ObjClone(&dstObj, srcObj);
				////////////////////////////////////////////////////////////////////////////////////////
				dstProp = dstObj->getProp();
				srcProp = srcObj->getProp();
				if (srcProp->group != NULL)
				{
					if (srcProp->group == srcProp)
					{
						mapGropProp.SetAt(dstProp, srcProp);
						dstProp->group = dstProp;
					}
				}
				////////////////////////////////////////////////////////////////////////////////////////
				m_mainFrm->m_AryCloneObject.Add(dstObj);
			}
		}


		/////////////////////////////////////////////그룹 복사 일 경우 그룹 맞춰줌////////////////////////////////
		CformProp* Prop = NULL;
		POSITION pos = NULL;
		pos = mapGropProp.GetStartPosition();

		while (pos != NULL)
		{
			mapGropProp.GetNextAssoc(pos, dstProp, srcProp);
			for (size_t ii = 0; ii < (size_t)m_mapH.forms.GetCount(); ii++)
			{
				Prop = ((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp();
				if (Prop->group != NULL && Prop->group == srcProp && Prop != srcProp)
				{
					//Prop->group = dstProp;
					CObjectLoad* dstObj = NULL;
					srcObj = (CObjectLoad*)m_mapH.forms.GetAt(ii);
					m_mainFrm->ObjClone(&dstObj, srcObj);
					dstObj->getProp()->group = dstProp;
					m_mainFrm->m_AryCloneObject.Add(dstObj);
				}
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	return true;
}

bool CChildFrame::MultiCopy()
{
	bool bMulti = false;
	if (m_mainFrm->m_AryCloneObject.GetCount() > 1)
	{
		CformProp* gropProp = NULL;
		CformProp* Prop = ((CObjectLoad*)m_mainFrm->m_AryCloneObject.GetAt(0))->getProp();
		for (size_t ii = 0; ii < (size_t)m_mainFrm->m_AryCloneObject.GetCount(); ii++)
		{
			gropProp = ((CObjectLoad*)m_mainFrm->m_AryCloneObject.GetAt(ii))->getProp();
			if (gropProp->group == NULL || (gropProp->group != NULL && gropProp->group != Prop->group))
			{
				bMulti = true;
				break;
			}
		}
	}
	return bMulti;
}

void CChildFrame::Paste()
{
	if (m_mapH.mapK == MK_PROCEDURES)
		return;

	m_mapH.modified = true;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETUNDO, TRUE);
	m_CloneCnt++;
	
	int gg = m_view.m_FmCtrl->GetSelCount();
	bool bMulti = MultiCopy();
	m_view.SelObj(-1);

	CformProp* dstProp = NULL;
	CformProp* srcProp = NULL;
	CObjectLoad*	dstObj = NULL;
	CObjectLoad*	srcObj = NULL;
	CMap<CformProp*, CformProp*, CformProp*, CformProp*> mapGropProp;
	for (size_t ii = 0; ii < (size_t)m_mainFrm->m_AryCloneObject.GetCount(); ii++)
	{
		dstObj = NULL;
		srcObj = m_mainFrm->m_AryCloneObject.GetAt(ii);
		m_mainFrm->ObjClone(&dstObj, srcObj);
		////////////////////////////////////////////////////////////////////////////////////////
		dstProp = dstObj->getProp();
		srcProp = srcObj->getProp();
		if (srcProp->group != NULL)
		{
			if (srcProp->group == srcProp)
			{
				mapGropProp.SetAt(dstProp, srcProp);
				dstProp->group = dstProp;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////
		dstObj->LoadProc();
		m_mapH.forms.Add(dstObj);

		m_view.SelAddObj(m_mapH.forms.GetCount() - 1, bMulti);

		if (m_view.isValidTabOrder((CformProp*)dstObj->getProp()))
		{
			dstObj->getProp()->index = ++m_view.m_nSeqTabOdr;
		}

		if (m_view.isValidIOOrder((CformProp*)dstObj->getProp()))
		{
			dstObj->getProp()->iorder = ++m_view.m_nSeqIoOdr;
		}
	}
	m_view.m_selF = m_mapH.forms.GetCount() - 1;

	/////////////////////////////////////////////그룹 복사 일 경우 그룹 맞춰줌////////////////////////////
	CformProp* Prop = NULL;
	POSITION pos = NULL;
	pos = mapGropProp.GetStartPosition();
	while (pos != NULL)
	{
		mapGropProp.GetNextAssoc(pos, dstProp, srcProp);
		for (size_t ii = 0; ii < (size_t)m_mapH.forms.GetCount(); ii++)
		{
			Prop = ((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp();
			if (Prop->group != NULL && Prop->group == srcProp && Prop != srcProp)
			{
				srcObj = (CObjectLoad*)m_mapH.forms.GetAt(ii);
				Prop->group = dstProp;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////

	m_view.Invalidate();
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
}

void CChildFrame::Cut()
{
	if (Copy())
	{
		for (int ii = 0; ii < m_mapH.forms.GetCount(); ii++)
		{
			if (((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp()->block == true)
			{
				m_view.deleteForm(ii);
				ii = -1;
			}
		}
		m_view.m_selF = -1;
		m_view.SelObj(-1);
	}
}

BOOL CChildFrame::IsCopyEnable()
{
	if (m_view.m_selF >= 0 || m_view.m_FmCtrl->GetSelCount() != 0 || m_mapH.mapK == MK_PROCEDURES)
		return TRUE;

	return FALSE;
}

BOOL CChildFrame::IsEditEnable(UINT cmdID)
{
	int	selF   = m_view.m_selF;
	BOOL	enable = FALSE;

	if (selF < 0 || m_view.m_FmCtrl->GetSelCount() > 1)
		return FALSE;

	CformProp* form = NULL;
	if (!m_mapH.forms.IsEmpty()) form = ((CObjectLoad*)m_mapH.forms.GetAt(selF))->getProp();
	else return enable;
	switch (cmdID)
	{
	case ID_ETC_FORMAT:
		if ((form->kind == fmEDIT || form->kind == fmOUTPUT || form->kind == fmCOMBO)
			&& (form->charType == ctNUMERIC || form->charType == ctALPHA)
			&& (form->whatEdit == weIT || form->whatEdit == weNOP))
		{
			enable = TRUE;
		}
		break;

	case ID_ETC_CORGB:
		if ((form->kind == fmOUTPUT
			|| (form->kind == fmEDIT && form->ioType == ioBOTH))
			&& (form->whatEdit == weCORGB || form->whatEdit == weNOP))
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
	if (selF < 0 || m_view.m_FmCtrl->GetSelCount() > 1)
		return weNOP;

	CformProp* form = ((CObjectLoad*)m_mapH.forms.GetAt(selF))->getProp();
	switch (cmdID)
	{
	case ID_ETC_FORMAT:
		{
			CFormat* dlg;
			CString	name = form->name;
			CString	editS = form->editFormat;

			dlg = new CFormat(this, form->whatEdit, editS, form->attributes, name, form->size);
			if (dlg->DoModal() == IDOK)
			{
				form->attributes = dlg->m_attributes;
				form->size = dlg->m_size;
				form->whatEdit = dlg->m_whatEdit;
				form->editFormat = dlg->m_edit;
				
				CRect	tRc;
				tRc.SetRect(form->rect.left, form->rect.top, form->rect.right, form->rect.bottom);
				m_view.InvalidateRect(tRc);
			}
			delete dlg;
		}
		break;

	case ID_ETC_CORGB:
		CCoRGB*	dlg;
		if (form->whatEdit == weCORGB)
		{
			dlg = new CCoRGB(this, form->name, form->whatEdit,
					HIWORD(form->auxValues), LOWORD(form->auxValues));
		}
		else
		{
			long lCorgb = (long)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCORGB);
			dlg = new CCoRGB(this, form->name, form->whatEdit, LOWORD(lCorgb), HIWORD(lCorgb));
		}

		if (dlg->DoModal() == IDOK)
		{
			form->whatEdit = dlg->m_whatEdit;
			if (form->whatEdit == weCORGB)
			{
				form->auxValues = MAKELONG(dlg->m_dnRGB, dlg->m_upRGB);
				form->attributes |= faCORGB;
				form->auxAttributes |= ccRAW;
			}
			else
			{
				form->auxValues = 0;
				form->attributes &= ~faCORGB;
				form->auxAttributes &= ~ccRAW;
			}
		}
		delete dlg;
		break;
	default:
		break;
	}

	return weNOP;
}

bool CChildFrame::DeleteFiles()
{
	if (!this->m_hWnd)
		return true;

	if (m_delete)
		return true;

	if (m_mapH.modified)
	{
		if (MessageBox(m_mapH.mapN + CString("\n\nIt may be damaged unless you store this map.\n\nDo you close this map?"), 
				_T("Message"), MB_ICONEXCLAMATION|MB_YESNO) == IDNO)
			return false;
	}

	CString	tmps;

	m_delete = true;
	if (m_mapH.mapN[0] != NULL)
	{
		if (m_mapH.source)
		{
			wccSetTempPath(tmps, m_mapH.mapN);
			DeleteFile(tmps);
		}

		if (m_mapH.compile)
		{
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
		if (m_mapH.onChangeLayout)
			DeleteFile(m_mapH.onChangeLayoutN);
		if (m_mapH.onFlicking)
			DeleteFile(m_mapH.onFlickingN);
		if (m_mapH.onTrigger)
			DeleteFile(m_mapH.onTriggerN);
		if (m_mapH.onDeclaration)
			DeleteFile(m_mapH.onDeclarationN);
		if (m_mapH.onInDeclaration)
			DeleteFile(m_mapH.onInDeclarationN);
	}

	for (int ii = 0; ii < m_mapH.forms.GetCount(); ii++)
	{
		tmps = ((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp()->scpN;
		if (!tmps.IsEmpty())
			DeleteFile(tmps);
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
		for (int ii = 0; ii < m_mapH.forms.GetCount(); ii++)
			((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp()->block = false;
		m_view.m_selF = idx;
		m_view.m_stdF = -1;
	}
	else
	{
		((CObjectLoad*)m_mapH.forms.GetAt(idx))->getProp()->block = true;
		if (m_view.m_selF >= 0 && m_view.m_FmCtrl->GetSelCount() == 0)
		{
			((CObjectLoad*)m_mapH.forms.GetAt(m_view.m_selF))->getProp()->block = true;
		}
		if (m_view.m_stdF < 0)
			m_view.m_stdF = idx;
	}
	m_view.SelObj(idx);
	m_view.RedrawView();
}

int CChildFrame::GetSelectedForm()
{
	if (m_view.m_FmCtrl->GetSelCount() == 0 || m_view.m_FmCtrl->GetSelCount() == 1)
		return m_view.m_selF;
	else
		return -2;
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
	m_view.ObjectRemove();
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

CString CChildFrame::PopRedo(bool isAll)
{
	if (!m_redos.GetSize())
		return _T("");

	CString	sRedo = m_redos.GetAt(0);
	if (isAll)
		m_redos.RemoveAll();
	else
		m_redos.RemoveAt(0);

	return sRedo;
}

CString	CChildFrame::PopUndo()
{
	if (!m_undos.GetSize())
		return _T("");

	CString sUndo = m_undos.GetAt(0);
	m_undos.RemoveAt(0);
	return sUndo;
}

void CChildFrame::PushRedo(CString src)
{
	m_redos.InsertAt(0, src);
	if (m_redos.GetSize() > MAX_UNDO)
		m_redos.RemoveAt(m_redos.GetSize() - 1);
}

void CChildFrame::PushUndo(CString src)
{
	m_undos.InsertAt(0, src);
	TRACE("%s\n",src);
	if (m_undos.GetSize() > MAX_UNDO)
		m_undos.RemoveAt(m_undos.GetSize() - 1);
}

void CChildFrame::ReleaseFocus()
{
	ClearOrderNumber();
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
	m_mainFrm->SendMessage(WM_USER, ID_USR_TRLAYOUT, NULL);
	m_mainFrm->SendMessage(WM_USER, ID_USR_RTSINFO, NULL);
	m_mainFrm->SendMessage(WM_USER, ID_USR_REFRESHCTRL, TRUE);
	m_mainFrm->SendMessage(WM_USER, ID_USR_SETFINDCAPTION, NULL);
	if (m_view.m_pEmulateWnd)
		m_mainFrm->SendMessage(WM_USER, ID_USR_ENABLEPROP, FALSE);
	else
		m_mainFrm->SendMessage(WM_USER, ID_USR_ENABLEPROP, TRUE);
}

void CChildFrame::getGroupNum(CformProp* Group, CArray <int, int> &arr)
{
	CformProp*	form;

	for (int ii = 0; ii < m_mapH.forms.GetCount(); ii++)
	{
		form = ((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp();
		if (form->group == Group)
		{
			arr.Add(ii);
			//if (form->kind == fmTAB)
			//	getTabNum(formR, arr);
		}
	}
}

CRect CChildFrame::getFormRect(int idx)
{
	CformProp*	form = ((CObjectLoad*)m_mapH.forms.GetAt(idx))->getProp();

	if (form->group)
		return groupRect(form->group);

	return CRect(form->rect.left, form->rect.top, form->rect.right, form->rect.bottom);
}

CRect CChildFrame::groupRect(CformProp* group)
{
	CRect	rc, tRc;
	CformProp*	form;

	rc.SetRectEmpty();
	for (int ii = 0; ii < m_mapH.forms.GetCount(); ii++)
	{
		form = ((CObjectLoad*)m_mapH.forms.GetAt(ii))->getProp();
		if (form->group == group)
		{
			tRc.SetRect(form->rect.left, form->rect.top, form->rect.right, form->rect.bottom);
			rc.UnionRect(rc, tRc);
		}
	}
	return rc;
}

void CChildFrame::DrawOrderNumber(DWORD dwOrderKind)
{
	ClearOrderNumber();
	m_OrderWnd = new CfmOrder(&m_view,&m_mapH.forms);
	m_OrderWnd->DrawOrderNumber(dwOrderKind);
}

void CChildFrame::ClearOrderNumber()
{
	if(m_OrderWnd)
	{
		delete m_OrderWnd;
		m_OrderWnd = NULL;
	}
}

bool CChildFrame::CheckOrder(CPoint point)
{
	bool bCheck = false;
	if(m_OrderWnd)
	{
		bCheck = m_OrderWnd->CheckOrder(point);
	}

	if(bCheck == false)
	{
		ClearOrderNumber();
	}
	return bCheck;
}

bool CChildFrame::IsLayoutMode()
{
	// Layout : 현재 상태에 해당하는 Properties 세팅
	if (m_view.m_bLayoutMode)
		return true;

	return false;
}