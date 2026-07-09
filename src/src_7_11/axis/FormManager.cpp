#include "stdafx.h"
#include "FormManager.h"
#include "MainFrm.h"
#include "axMisc.h"
#include "SChild.h"
#include "ChildFrm.h"
#include "Linkbar.h"
#include "axScrollView.h"
#include "axisView.h"
#include "../dll/axissm/smheader.h"
#include "../dll/axissm/axCommon.hxx"
#include "axMsg.hxx"
#include "GPop.h"

#include "../h/axis.h"
#include "../h/axisfire.h"
#include "../h/axisvar.h"

CFormManager::CFormManager()
{
	m_pFrame = NULL;
}

CFormManager::~CFormManager()
{
}

void CFormManager::Init(CWnd* pMain, void* pfunc) {

	m_axiscall = (bool(*)(int, WPARAM, LPARAM))pfunc;
	m_pFrame = (CMainFrame*)pMain;
}

template <typename T>
void CFormManager::setChild(T t, CString mapname, int key, int vsN, int vtype, int size, CSize frameGap)
{
	t->m_xcaption.ChangeVS(vsN);
	t->createLink(m_pFrame->m_tMenu, mapname, m_axiscall, m_pFrame->m_home, m_pFrame->GetUserName(), m_pFrame->GetSkinName(), m_pFrame->isShowLinkbar(mapname.Left(L_MAPN)));
	t->m_key = key;
	t->m_vwTYPE = vtype;
	t->m_mapN   = mapname;

	t->m_width  = LOWORD(size) + frameGap.cx;
	t->m_height = HIWORD(size) + frameGap.cy;
}

CView* CFormManager::CreateBaseForm(CString mapname, int& key, bool bSdi, int vsN)
{
	if(!m_pFrame->maxChild())
	{
		::MessageBox(m_pFrame->m_hWnd, "최대 화면 실행 개수를 초과하였습니다.\r\n열려진 화면을 닫으시거나, 설정 >> 환경 설정 >> 기본설정의\r\n화면 설정 옵션을 변경해 주시기 바랍니다.", _T("K-Bond"), MB_ICONWARNING);
		return NULL;
	}

	CView* pView = NULL;
	CString saveN = mapname;
	int	vtype = 0, vwTYPE = 0, vwKIND, size = 0;

	vtype = m_pFrame->m_wizard->getFormInfo(mapname, size);
	if (vtype == vtypeERR)
	{
		key = -1;
		return NULL;
	}

	vwTYPE = vtypeMSK & vtype;	
	vwKIND = (vwTYPE == vtypeSCR) ? SCROLLVIEW : NORMALVIEW;	
	pView = bSdi ? m_pFrame->GetSDIView(vwKIND) : m_pFrame->GetNewView(vwKIND);
	key = m_pFrame->m_wizard->AttachForm((CView*)pView, vtype);

	CSize	frameGap = m_pFrame->GetFrameGap();
	if (m_pFrame->isShowLinkbar(mapname.Left(L_MAPN)))
		frameGap.cy += LBAR_HEIGHT;
	frameGap.cy += XCAP_HEIGHT;
	 
	if (bSdi)
	{
		CSChild* pChild = (CSChild *)pView->GetParent();
		setChild(pChild, mapname.Left(L_MAPN), key, vsN, vtype, size, frameGap);
		m_pFrame->m_arSDI[vsN].SetAt(key, pChild);
		m_pFrame->m_sdiZORDER[vsN].InsertAt(0, key);

		if(mapname == BONDMAP)
		{
			m_pFrame->m_pBondWnd = (CWnd*)pChild;
			m_pFrame->m_Bondkey = key;
		}
	}
	else
	{
		CChildFrame* pChild = (CChildFrame*)pView->GetParent();
		setChild(pChild, mapname.Left(L_MAPN), key, vsN, vtype, size, frameGap);
		m_pFrame->m_arMDI[vsN].SetAt(key, pChild);
		m_pFrame->m_hooklist[vsN].InsertAt(0, key);
	
		if(mapname == BONDMAP)
		{
			m_pFrame->m_pBondWnd = (CWnd*)pChild;
			m_pFrame->m_Bondkey = key;
		}
	}
	
	
	if (vwTYPE == vtypeSCR)
	{	// scroll view
		((CAxScrollView *)pView)->SetInfo(key, m_pFrame->m_axisres);
		((CAxScrollView *)pView)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else
	{		
		((CAxisView *)pView)->SetInfo(key, m_pFrame->m_axisres);
		if (m_pFrame->m_bSDI)
			pView->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	}

	LONG_PTR	rc	= FALSE;
	bool		domino  = false;
	if (m_pFrame->IsDll(mapname))
		domino = true;
	
	if (!m_pFrame->m_wizard->loadMap(mapname, key, size))
	{
		(pView->GetParent())->PostMessage(WM_CLOSE, (WPARAM)0, (LPARAM)0);
		
		if (m_pFrame->m_bSDI)
		{
			m_pFrame->m_arSDI[vsN].RemoveKey(key);
			m_pFrame->m_sdiZORDER[vsN].RemoveAt(0);
		}
		else
		{
			m_pFrame->m_arMDI[vsN].RemoveKey(key);
			m_pFrame->m_hooklist[vsN].RemoveAt(0);
		}
		return NULL;
	}

	saveN = saveN.Left(L_MAPN);
	if(saveN != BONDMAP)
		addScreenList(saveN, key, vsN);
	
	if(pView != NULL)
		m_pFrame->SendUseMap(mapname);
	
	return pView;
}

void CFormManager::addScreenList(CString mapN, int key, int vsN)
{
	m_pFrame->add_history(mapN);
	m_pFrame->m_tMenu->set_Maps(mapN.Left(L_MAPN));
	m_pFrame->addTool_history(mapN);
	m_pFrame->m_bar2->add_Button(key, mapN, m_pFrame->m_tMenu, vsN);
	m_pFrame->m_activeMapN = mapN;
}

int CFormManager::ChangeChild(CString mapN, int trigger, int key, int position)
{
	if (m_pFrame->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_pFrame->m_tMenu->GetURL(mapN.Left(L_MAPN));
		m_pFrame->RunWebBrowser(url);
		return TRUE;
	}

	CView* view = NULL;
	CString	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;

	//if (m_pFrame->m_bSDI || m_pFrame->CheckSDIMap(key))
	if (m_pFrame->m_bSDI || CheckSDIMap(key))  //정식버전
	{
		CSChild*	schild = NULL;
		if (!m_pFrame->m_arSDI[m_pFrame->m_vsN].Lookup(key, schild))
		{
			if (key < winK_NORM)
			{
				if (!m_pFrame->Agree_Duplication(mapN))
					return 0;
			}
			return CreateChild(mapN, trigger, key, position);
		}
	//	schild->GetActiveView();  //정식버전 아래부분 else 전까지
		view = schild->GetActiveView();
		
		
		int vwTYPE = 0, size = 0;
		vwTYPE = m_pFrame->m_wizard->getFormInfo(mapname, size);
		//	return CreateChild(mapN, trigger, 0, position);

		if (vwTYPE <= vtypeERR)		return 0;
		if (vwTYPE != schild->m_vwTYPE)	return CreateChild(mapN, trigger, 0, position);

		schild->m_mapN = mapname.Left(L_MAPN);
		refreshMap(schild, key);
	}
	else
	{
		CChildFrame*	child = NULL;
		if (!m_pFrame->m_arMDI[m_pFrame->m_vsN].Lookup(key, child))
		{
			if (key < winK_NORM)
			{
				if (!m_pFrame->Agree_Duplication(mapN))
					return 0;
			}
			return CreateChild(mapN, trigger, key, position);
		}
		child->GetActiveView();
		child->m_mapN = mapname.Left(L_MAPN);  //정식버전 아래줄까지
		refreshMap(child, key);
	}

	addScreenList(mapname.Left(L_MAPN), key, position);  //정식버전
	return key;
	/*
	int vwTYPE = 0, size = 0;
	vwTYPE = m_pFrame->m_wizard->getFormInfo(mapname, size);

	if (vwTYPE <= vtypeERR)		return 0;
	return 0;
	*/
}


int CFormManager::CreateChild(CString mapN, int trigger, WINDOWPLACEMENT wndpl, bool showChild, int vsN)
{
	if (vsN == -1)	vsN = m_pFrame->m_vsN;
	CString mapname, saveN = mapN;
	mapname = mapN.Left(L_MAPN);

	int key = 0;
	CString sTitle = _T("");
	CView *pView = CreateBaseForm(mapN, key, m_pFrame->m_bSDI,vsN);
	if (pView == NULL)
		return -1;

	m_pFrame->m_wizard->setFontSize(key, m_pFrame->m_fontSize);
	set_Trigger(key, trigger, vsN);

	if (m_pFrame->m_bSDI)
	{
		CSChild* pchild = (CSChild *)pView->GetParent();
		pchild->SetMisc(WM_AXIS, m_pFrame, m_axiscall, m_pFrame->m_fontSize, m_pFrame->m_bSDI, m_pFrame->m_axisres, true);
		pchild->SetWindowPlacement(&wndpl);
		sTitle = pchild->m_xcaption.GetTitle();

		if (m_pFrame->getPopupMap(mapname))
			m_pFrame->actionSCaption(key, IDX_PIN, true);
	}
	else
	{
		CChildFrame *child = (CChildFrame *)pView->GetParent();
		child->SetMisc(WM_AXIS, m_pFrame, m_axiscall, m_pFrame->m_fontSize, m_pFrame->m_axisres, true);
		child->SetWindowPlacement(&wndpl);
		if (!showChild)
			child->ShowWindow(SW_HIDE);
		sTitle = child->m_xcaption.GetTitle();
	}

	if (!m_pFrame->ExistMenu(saveN.Left(L_MAPN)))
		m_pFrame->m_bar2->change_ButtonText(key, sTitle);

	return key;
}

BOOL CFormManager::GetMapInfo(const char* mapName, int& type, int& size)
{
	CString map(mapName);
	if (map.GetLength() < L_MAPN)
		return 0;
	map = map.Left(L_MAPN);

	m_pFrame->m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&type, (BYTE *)(VTS_BSTR VTS_I4), map, &size);

//	size = MAKELONG(m_main->ConvertToDpi(LOWORD(size), -1), m_main->ConvertToDpi(HIWORD(size), -1));

	if (type <= vtypeERR)	
	{
		//return FALSE;
/*		CProfile profile(pkVTCode);

		CString str(profile.GetSectionLF("DLL Controls"));

		int find = str.Find(mapName);
		if (find < 0)
			return FALSE;

		int findLF = str.Find('\n', find);
		if (findLF <= find)
			return FALSE;

		CString line = str.Mid(find, findLF - find);
		Trim(line);

		find = line.Find('=');
		if (find < 0)
			return FALSE;

		line.Delete(0, find + 1);
		Trim(line);

		find = line.Find(';');
		if (find > 0)
		{
			line = line.Left(find);
		}

		Trim(line);

		CString width, height, ts;

		width = Parse(line, " ");
		Trim(line); 
		height = Parse(line, " ");
		Trim(line);
		ts = line;

		short cx = atoi(width);
		short cy = atoi(height);
		//size = MAKELONG(cx, cy);
		size = MAKELONG(m_main->ConvertToDpi(cx, -1), m_main->ConvertToDpi(cy, -1));

		ts.MakeLower();

		if (ts == "fixed")
			//type = vtypeNRM;
			type = vtypeFIX;
		else if (ts == "fixedex")
			//type = vtypeNRM;
			type = vtypeFEX;
		else if (ts == "normal")
			type = vtypeNRM;
		else if (ts == "resizable")
			type = vtypeRSZ;
		else
			return FALSE;
tmp.Format("[DPI] : GetMapInfo _ type [%d]\n", type);
OutputDebugString(tmp);
*/
		return TRUE;
	}
	else
	{
		return TRUE;
	}
	
	return TRUE;
}

int CFormManager::CopyScreen(CString mapN, int group, int fontsize, bool bboldfont, CPoint sp)
{
	
	//if (!IsValidMap(mapN.Left(L_MAPN)))
	//	return 0;

	//if (m_main->bNotOverMap(mapN))
	//	return 0;

//	if (m_pFrame->IsTabView(mapN.Left(L_MAPN)))
//		return CopyTabScreen(mapN, group, fontsize, bboldfont, sp);

	CString SendMapName = mapN;
	CString mapname, savemapN = mapN;

	//mapname = m_pFrame->getSelectMAP(mapN.Mid(2,4));

//	if (!mapname.IsEmpty())
	{
//		SendMapName.Format("%sSelectMAP\t%s", mapname, mapN.Mid(2,4));
//		mapN = SendMapName;
	}

	mapname = mapN.Left(L_MAPN);
	
	if (mapname.IsEmpty())	
		mapname = m_pFrame->m_mapN;
	
	int vtype = 0, vwTYPE = 0, size = 0, vwKIND, key = 0;

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	CMenu*		menu  = m_pFrame->GetMenu();
	CView*		view  = m_pFrame->GetNewView(vwKIND);
	CChildFrame*	child = (CChildFrame *) view->GetParent();

	OutputDebugString("[DPI] DI_ATTACH : pos14");
	m_pFrame->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx, nCy;
	CSize	frameGap = m_pFrame->GetFrameGap();


	if (vwTYPE == vtypeSCR)
	{	// scroll view
		((CAxScrollView *)view)->SetInfo(key, m_pFrame->m_axisres);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	
		((CAxisView *)view)->SetInfo(key, m_pFrame->m_axisres);//((CAxisView *)view)->m_key = key;

//	m_pFrame->AddMdi(m_pFrame->m_vsN, key, child);
	m_pFrame->m_arMDI[m_pFrame->m_vsN].SetAt(key, child);
	m_pFrame->m_hooklist[m_pFrame->m_vsN].InsertAt(0, key);

	child->m_key	 = key;
	child->m_vwTYPE = vtype;
//	child->SetMapName(axiscall, savemapN);

	nCx = LOWORD(size) + frameGap.cx - 1;
	nCy = HIWORD(size) + frameGap.cy - 1;

	//child->SetSize(nCx, nCy);

	
	BOOL	rc = FALSE;
	m_pFrame->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, SendMapName, size, false);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_pFrame->RemoveMdi(m_pFrame->m_vsN, key);
		m_pFrame->m_hooklist[m_pFrame->m_vsN].RemoveAt(0);
		return 0;
	}

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	m_pFrame->m_bar1->set_Maps(savemapN.Left(L_MAPN));
	m_pFrame->addTool_history(savemapN);
	m_pFrame->m_bar2->add_Button(key, savemapN.Left(L_MAPN), m_pFrame->m_tMenu);

	if (!m_pFrame->ExistMenu(mapname))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_pFrame->m_bar2->change_ButtonText(key, string);
	}

	child->SetWindowPos(NULL, sp.x, sp.y, nCx, nCy, SWP_HIDEWINDOW);
	child->ShowWindow(SW_SHOW);
	OutputDebugString("[DPI] : pos69");
 	int nBoldfont =  0;
 	m_pFrame->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
 		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(nBoldfont, fontsize));
	OutputDebugString("[DPI] : Font_23");
	//m_pFrame->set_Trigger(key, group);
 	//child->SetMisc(WM_AXIS, m_main, axiscall, fontsize, nBoldfont, vwTYPE == vtypeWND ? false : true);

	m_pFrame->SetForegroundWindow();
	m_pFrame->maxChild();
	m_pFrame->m_activeMapN = child->m_mapN;

	return key;
	
}
		
int CFormManager::CreateChild(CString mapN, int trigger, int key, int position, CPoint point)
{
	if (m_pFrame->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_pFrame->m_tMenu->GetURL(mapN.Left(L_MAPN));
		m_pFrame->RunWebBrowser(url);
		return TRUE;
	}
	
	CString mapname, savemapN;
	savemapN = mapN;
	mapname  = mapN.Left(L_MAPN);

	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;

	CView *pView = CreateBaseForm(mapN, key, m_pFrame->m_bSDI, m_pFrame->m_vsN);
	if (pView == NULL)
		return -1;
	
	CWnd *child = (CWnd *)pView->GetParent();
	CString sTitle;
	set_Trigger(key, trigger);

	if (m_pFrame->m_bSDI)
	{
		((CSChild*)child)->SetMisc(WM_AXIS, m_pFrame, m_axiscall, m_pFrame->m_fontSize, m_pFrame->m_bSDI, m_pFrame->m_axisres, true);
		m_pFrame->setFormPos(child, mapname, key, position, point, ((CSChild*)child)->m_width, ((CSChild*)child)->m_height);
		sTitle = ((CSChild*)child)->m_xcaption.GetTitle();
	}
	else
	{
		((CChildFrame*)child)->SetMisc(WM_AXIS, m_pFrame, m_axiscall, m_pFrame->m_fontSize, m_pFrame->m_axisres, true);
		m_pFrame->setFormPos(child, mapname, key, position, point, ((CChildFrame*)child)->m_width, ((CChildFrame*)child)->m_height);
		sTitle = ((CChildFrame*)child)->m_xcaption.GetTitle();
	}

	child->ShowWindow(SW_SHOW);
	if (!m_pFrame->ExistMenu(savemapN.Left(L_MAPN)))
	{
		m_pFrame->m_bar2->change_ButtonText(key, sTitle);
	}

	if (m_pFrame->getPopupMap(mapname))
		m_pFrame->actionCaption(key, IDX_SINGLE);

	if (!m_pFrame->IsDll(mapname))
	{
		m_pFrame->m_wizard->setTrigger(key, _T("CODX\tPASS"));
		m_pFrame->m_wizard->setTrigger(key, _T("CODJ\tPASS"));
	}

	if (m_pFrame->m_bSDI)
		child->SetActiveWindow();
	else
		m_pFrame->SetForegroundWindow();

	return key;
}

int CFormManager::CreateSDIbyMDI(CString mapN, int group, int fontN, CRect mdiRc, CRect sdiRc, int vsN)
{
	if (vsN == -1)	vsN = m_pFrame->m_vsN;

	CString savemapN = mapN;
	CString mapname = mapN;
	mapN = mapN.Left(L_MAPN);
	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;

	int key = 0, vwTYPE = 0;
	int nCx, nCy;
	CView*	view = CreateBaseForm(mapname, key, true, vsN);
	if (view == NULL)
		return -1;

	CSChild* child = (CSChild *)view->GetParent();
	vwTYPE = child->m_vwTYPE;

	m_pFrame->m_wizard->setFontSize(key, fontN);
	if (vwTYPE == vtypeFIX)
	{
		nCx = child->m_width;
		nCy = child->m_height;
	}
	else
	{
		nCx = sdiRc.Width();
		nCy = sdiRc.Height();
	}

	set_Trigger(key, group, vsN);
	child->SetMisc(WM_AXIS, m_pFrame, m_axiscall, fontN, m_pFrame->m_bSDI, m_pFrame->m_axisres, true);

	if (vsN != m_pFrame->m_vsN)
		child->SetWindowPos(NULL, sdiRc.left, sdiRc.top, nCx, nCy, SWP_HIDEWINDOW);
	else
	{
		child->SetWindowPos(NULL, sdiRc.left, sdiRc.top, nCx, nCy, SWP_SHOWWINDOW);
		child->SetFocus();
		m_pFrame->m_activeMapN = child->m_mapN;
	}

	child->m_mdiPt = mdiRc.TopLeft();
	child->m_rectMDI.CopyRect(&mdiRc);

	return key;
}


int CFormManager::CreateMDIbySDI(CString mapN, int group, int fontN, CRect sdiRc, CRect mdiRc, int vsN)
{
	if (vsN == -1)	vsN = m_pFrame->m_vsN;

	CString mapname, savemapN = mapN;
	mapname = mapN;
	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;
	mapN = mapN.Left(L_MAPN);
	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;

	int key = 0, vwTYPE = 0;
	int nCx, nCy;
	CView*	view = CreateBaseForm(mapname, key, false, vsN);
	if (view == NULL)
		return -1;

	CChildFrame* child = (CChildFrame *)view->GetParent();
	vwTYPE = child->m_vwTYPE;

	m_pFrame->m_wizard->setFontSize(key, fontN);

	if (false && vwTYPE == vtypeFIX)
	{
		nCx = child->m_width;
		nCy = child->m_height;
	}
	else
	{
		nCx = sdiRc.Width();
		nCy = sdiRc.Height();
	}

	set_Trigger(key, group, vsN);
	child->SetMisc(WM_AXIS, m_pFrame, m_axiscall, fontN, m_pFrame->m_axisres, true);

	if (vsN != m_pFrame->m_vsN)
		child->SetWindowPos(NULL, mdiRc.left, mdiRc.top, nCx, nCy, SWP_HIDEWINDOW);
	else
	{
		child->SetWindowPos(NULL, mdiRc.left, mdiRc.top, nCx, nCy, SWP_SHOWWINDOW);
		child->SetFocus();
		m_pFrame->m_activeMapN = child->m_mapN;
	}

	child->m_sdiPt = sdiRc.TopLeft();
	child->m_rectSDI.CopyRect(&sdiRc);

	return key;
}


int CFormManager::CreatePopup(CString mapN, int trigger, int key, int position, int actkey, CPoint point, bool closex)
{
	CString mapname = mapN.Left(L_MAPN);

	if (m_pFrame->getPopupMap(mapname))
	{
		key = 0;
		return CreateChild(mapN, trigger, key, position, point);
	}

	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND;
	vtype = m_pFrame->m_wizard->getFormInfo(mapname, size);


	if (vtype == vtypeERR)	return 0;
	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? SCROLLVIEW : NORMALVIEW;

	CString strWndClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		m_pFrame->m_axisres->m_hIcon);

	CGPop*	pop = new CGPop(m_pFrame);
	if (!pop->Create(strWndClass, AXISKEY, WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), NULL, NULL, 0, NULL))
	{
		CString	title;
		title.LoadString(ST_TEXT_OK);
		::MessageBox(m_pFrame->m_hWnd, "Fail create popup windows. Error code = [04]", title, MB_ICONWARNING);
		delete pop;
		return 0;
	}

	CRuntimeClass* pNewViewClass;
	if (vwTYPE == vtypeSCR)	pNewViewClass = RUNTIME_CLASS(CAxScrollView);
	else			pNewViewClass = RUNTIME_CLASS(CAxisView);

	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = m_pFrame->GetActiveDocument();

	CView* view = pop->AttachView(context);
	key = m_pFrame->m_wizard->AttachForm(view, vtype);


	if (vwTYPE == vtypeSCR)	((CAxScrollView *)view)->SetInfo(key, m_pFrame->m_axisres);
	else			((CAxisView *)view)->SetInfo(key, m_pFrame->m_axisres);

	pop->m_key = key;
	pop->m_vwTYPE = vtype;
	pop->m_mapN = mapname;

	CRect	rect;
	int	nCx, nCy, gapX, gapY;

	gapX  = GetSystemMetrics(SM_CXFRAME) * 2;
	gapX += GetSystemMetrics(SM_CXBORDER) * 4;

	gapY = GetSystemMetrics(SM_CYFRAME) * 2;
	gapY += GetSystemMetrics(SM_CYBORDER) * 4;
	gapY += GetSystemMetrics(SM_CYCAPTION);

	if (vwTYPE == vtypeSCR)
	{
		((CAxScrollView *)view)->SetInfo(key, m_pFrame->m_axisres);
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	((CAxisView *)view)->SetInfo(key, m_pFrame->m_axisres);

	m_pFrame->m_arGPOP.SetAt(key, pop);

	pop->m_width = nCx = LOWORD(size) + gapX;
	pop->m_height = nCy = HIWORD(size) + gapY;

	if (!m_pFrame->m_wizard->loadMap(mapN, key, size))
	{
		pop->PostMessage(WM_CLOSE, (WPARAM)0, (LPARAM)0);
		m_pFrame->m_arGPOP.RemoveKey(key);
		return 0;
	}

	m_pFrame->m_wizard->setGroupTrigger(key, trigger);

	pop->m_key = key;
	pop->m_vwTYPE = vtype;

	nCx = pop->m_width;
	nCy = pop->m_height;
	CRect	clientRc;

	pop->SetWindowPos(&CWnd::wndTopMost, 0, 10000, nCx, nCy, SWP_SHOWWINDOW);
	if (position == -1)
		pop->SetWindowPos(&CWnd::wndTopMost, point.x, point.y, nCx, nCy, SWP_SHOWWINDOW);
	else	
		m_pFrame->positionWindow(actkey, pop, position, false);

	pop->m_closex = closex;
	m_pFrame->m_activeMapN = pop->m_mapN;


	return key;
}


int CFormManager::ChangePopup(CString mapN, int trigger, int key, int position, int actkey)
{
	CString	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	mapname = m_pFrame->m_mapN;

	CGPop*	pop = NULL;
	if (!m_pFrame->m_arGPOP.Lookup(key, pop))	return CreatePopup(mapN, trigger, key, position, actkey);

	int vwTYPE = 0, size = 0;
	vwTYPE = m_pFrame->m_wizard->getFormInfo(mapname, size);

	if (vwTYPE <= vtypeERR)		return 0;
	if (vwTYPE != pop->m_vwTYPE)	return CreatePopup(mapN, trigger, key, position, actkey);

	CRect	rect;
	int	nCx, nCy, gapX, gapY;

	gapX = GetSystemMetrics(SM_CXFRAME) * 2;
	gapX += GetSystemMetrics(SM_CXBORDER) * 4;

	gapY = GetSystemMetrics(SM_CYFRAME) * 2;
	gapY += GetSystemMetrics(SM_CYBORDER) * 4;
	gapY += GetSystemMetrics(SM_CYCAPTION);

	CView* view = pop->GetActiveView();
	switch (vwTYPE)
	{
	case 0:
	default:
		if ((vwTYPE & vtypeMSK) == vtypeSCR)
		{
			((CAxScrollView *)view)->SetInfo(key, m_pFrame->m_axisres);
			((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
		}
		else	((CAxisView *)view)->SetInfo(key, m_pFrame->m_axisres);
		break;
	}

	pop->m_width = nCx = LOWORD(size) + gapX;
	pop->m_height = nCy = HIWORD(size) + gapY;

	if (!m_pFrame->m_wizard->loadMap(mapN, key, size))
	{
		pop->PostMessage(WM_CLOSE, (WPARAM)0, (LPARAM)0);
		return 0;
	}

	nCx = pop->m_width;
	nCy = pop->m_height;
	pop->SetWindowPos(&CWnd::wndTopMost, 0, 0, nCx, nCy, SWP_NOMOVE);
	m_pFrame->m_activeMapN = pop->m_mapN;

	return key;
}

bool  CFormManager::OnCloseForm(int key)
{
	CChildFrame*	child;
	m_pFrame->removeWait(key);
	if (key & winK_POPUP)
	{
		for (int ii = 0; ii < m_pFrame->m_arHide.GetSize(); ii++)
		{
			child = m_pFrame->m_arHide.GetAt(ii);
			if (child && child->m_key != key)
				continue;
			m_pFrame->m_arHide.RemoveAt(ii);
			m_pFrame->m_wizard->detachForm(key);

			return true;
		}

		m_pFrame->m_arGPOP.RemoveKey(key);
		m_pFrame->m_wizard->detachForm(key);


		if (m_pFrame->m_arGPOP.IsEmpty())
		{
			if (!m_pFrame->m_bSDI)	m_pFrame->SetFocus();
		}
		else
		{
			CGPop*	pop;
			POSITION pos = m_pFrame->m_arGPOP.GetStartPosition();
			m_pFrame->m_arGPOP.GetNextAssoc(pos, key, pop);
			pop->SetFocus();
			m_pFrame->m_activeMapN = pop->m_mapN;
		}
	}
	else
	{
		bool		match = false;
		CSChild*	schild = NULL;
		int value = 0;
		int vsN = 0;
		for (vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
		{
			for (value = 0; value < m_pFrame->m_hooklist[vsN].GetSize(); value++)
			{
				if (key != m_pFrame->m_hooklist[vsN].GetAt(value))
					continue;
				m_pFrame->m_hooklist[vsN].RemoveAt(value);
			}

			for (value = 0; value <m_pFrame->m_sdiZORDER[vsN].GetSize(); value++)
			{
				if (key != m_pFrame->m_sdiZORDER[vsN].GetAt(value))
					continue;
				m_pFrame->m_sdiZORDER[vsN].RemoveAt(value);
			}
		}

		for (vsN = V_SCREEN1; vsN <= V_SCREEN4 && !match; vsN++)
		{
			if (m_pFrame->m_arMDI[vsN].Lookup(key, child))
			{
				match = true;
				m_pFrame->m_arMDI[vsN].RemoveKey(key);
				m_pFrame->m_wizard->detachForm(key);

				if (m_pFrame->m_bar2)	m_pFrame->m_bar2->del_Button(key, vsN);
			}
			else if (m_pFrame->m_arSDI[vsN].Lookup(key, schild))
			{
				match = true;

				m_pFrame->m_arSDI[vsN].RemoveKey(key);
				m_pFrame->m_wizard->detachForm(key);

				if (m_pFrame->m_bar2)	m_pFrame->m_bar2->del_Button(key, vsN);

				if (m_pFrame->m_bSDI)
					m_pFrame->axSFocus();
			}

			if (match && vsN == m_pFrame->m_vsN)
			{
				if (m_pFrame->m_bSDI && !m_pFrame->m_sdiZORDER[m_pFrame->m_vsN].GetSize())
				{
					m_pFrame->m_sdiZORDER[m_pFrame->m_vsN].RemoveAll();
					m_pFrame->m_activeMapN.Empty();
				}
				else if (m_pFrame->MDIGetActive() == NULL)
				{
					m_pFrame->m_hooklist[m_pFrame->m_vsN].RemoveAll();
					m_pFrame->m_activeMapN.Empty();
				}
			}
		}
	}
	return true;
}

bool CFormManager::closeChild(int key)
{
	class CChildFrame* pChild;
	if (!m_pFrame->m_arMDI[m_pFrame->m_vsN].Lookup(key, pChild))
	{
		CSChild*	schild = NULL;
		m_pFrame->m_arSDI[m_pFrame->m_vsN].Lookup(key, schild);
		if (schild)	schild->PostMessage(WM_CLOSE);
		return false;
	}
	pChild->PostMessage(WM_CLOSE, 0, 0);	return true;
}

bool CFormManager::closeChild(CString strmap)
{
	CChildFrame*	child = NULL;
	CSChild*	schild = NULL;
	CChildFrame*	pChild = NULL;
	int		key;
	POSITION pos;

	for (pos = m_pFrame->m_arSDI[m_pFrame->m_vsN].GetStartPosition(); pos; )
	{
		m_pFrame->m_arSDI[m_pFrame->m_vsN].GetNextAssoc(pos, key, schild);
		if (schild && schild->GetSafeHwnd() && schild->m_bActive && schild->m_mapN == strmap)
		{
			schild->PostMessage(WM_CLOSE);
			return true;
		}
	}

	for (pos = m_pFrame->m_arMDI[m_pFrame->m_vsN].GetStartPosition(); pos; )
	{
		m_pFrame->m_arMDI[m_pFrame->m_vsN].GetNextAssoc(pos, key, pChild);
		if (pChild && pChild->GetSafeHwnd() && pChild->m_bActive && pChild->m_mapN == strmap)
		{
			pChild->PostMessage(WM_CLOSE);
			return true;
		}
	}
	
	return false;
}

bool CFormManager::closeChild()
{
	if (m_pFrame->closePopup())
	{
		m_pFrame->SetFocus();
		return true;
	}

	CChildFrame*	child = NULL;
	CSChild*	schild = NULL;
	int		key;
	POSITION pos;
	for (pos = m_pFrame->m_arSDI[m_pFrame->m_vsN].GetStartPosition(); pos; )
	{
		m_pFrame->m_arSDI[m_pFrame->m_vsN].GetNextAssoc(pos, key, schild);
		if (schild && schild->GetSafeHwnd() && schild->m_bActive)
		{
			schild->PostMessage(WM_CLOSE);
			return true;
		}
	}

	if (m_pFrame->m_arMDI[m_pFrame->m_vsN].GetCount() <= 0)
	{
		for (pos = m_pFrame->m_arSDI[m_pFrame->m_vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arSDI[m_pFrame->m_vsN].GetNextAssoc(pos, key, schild);
			if (schild->m_bActive)
			{
				schild->PostMessage(WM_CLOSE);
				return true;
			}
		}
		return false;
	}

	child = (CChildFrame*)m_pFrame->MDIGetActive();
	if (!child)	return false;
	child->SendMessage(WM_CLOSE, 0, 0);
	return true;
}

void CFormManager::EnableChild(BOOL bflag)
{
	int			key;
	CArray	<int, int>	ary;
	CChildFrame*		child = NULL;
	CSChild*		schild = NULL;

	m_pFrame->keyArray(ary, m_pFrame->m_vsN, true);
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		key = ary.GetAt(ii);
		if (!m_pFrame->m_arSDI[m_pFrame->m_vsN].Lookup(key, schild))
			continue;
		schild->EnableWindow(TRUE);
	}
	ary.RemoveAll();

	m_pFrame->keyArray(ary, m_pFrame->m_vsN);
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		key = ary.GetAt(ii);
		if (!m_pFrame->m_arMDI[m_pFrame->m_vsN].Lookup(key, child))
			continue;
		schild->EnableWindow(TRUE);
	}
	ary.RemoveAll();
}

void CFormManager::Setfocus_Child(int key, bool bRestore)
{
	CChildFrame*	child;
	if (!m_pFrame->m_arMDI[m_pFrame->m_vsN].Lookup(key, child))
	{
		CSChild*	schild;
		if (m_pFrame->m_arSDI[m_pFrame->m_vsN].Lookup(key, schild))
		{
			if (schild->m_bIconic && bRestore)
				schild->PostMessage(WM_SYSCOMMAND, SC_RESTORE);//actionSCaption(schild->m_key, IDX_RESTORE);
			else	schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
	else
	{

		if (child->m_bIconic && bRestore)
			child->actionCaption(IDX_RESTORE, CPoint(0, 0));
		else	child->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}


void CFormManager::displayChild(int vsN, bool show/*=true*/, bool bSDI/*=false*/)
{
	int			key;
	CArray	<int, int>	ary;
	CChildFrame*		child = NULL;
	CSChild*		schild = NULL;
	UINT			nFlags;

	if (show)
		nFlags = SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER;
	else	nFlags = SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER;

	if (bSDI)
	{
		m_pFrame->keyArray(ary, vsN, true);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);
			if (!m_pFrame->m_arSDI[vsN].Lookup(key, schild))
				continue;
			schild->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, nFlags);
			schild->EnableWindow(TRUE);
		}
	}
	else
	{
		m_pFrame->keyArray(ary, vsN);
		for (int ii = 0; ii < ary.GetSize(); ii++)
		{
			key = ary.GetAt(ii);

			if (!m_pFrame->m_arMDI[vsN].Lookup(key, child))
				continue;
			child->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, nFlags);
			child->EnableWindow(TRUE);
		}
	}
	ary.RemoveAll();
}


void CFormManager::change_VirtualScreen(int index)
{
	if (index == m_pFrame->m_vsN)	return;

	displayChild(m_pFrame->m_vsN, false);
	displayChild(m_pFrame->m_vsN, false, true);

	m_pFrame->m_vsN = index;

	m_pFrame->m_bar2->change_VirtualScreen(index);
	//m_pFrame->m_smain->change_VirtualScreen(index); //정식버전

	displayChild(m_pFrame->m_vsN, true);
	displayChild(m_pFrame->m_vsN, true, true);

	m_pFrame->m_wizard->setvirtualScreen(index);

	m_pFrame->SetActiveWindow();
}

void CFormManager::set_Trigger(int key, int triggerN, int vsN)
{
	if (vsN == -1)	vsN = m_pFrame->m_vsN;
	int		group = triggerN;

	if (m_pFrame->m_bSDI)
	{
		CSChild*	child;
		if (m_pFrame->m_arSDI[vsN].Lookup(key, child))
		{
			child->m_xcaption.SetGroup(triggerN);
			if (triggerN == ALL_GROUP)
				group = -1;

			m_pFrame->m_wizard->setGroupTrigger(key, group);
			child->m_xcaption.SetGroup(triggerN);
		}
	}
	else
	{
		CChildFrame*	child = NULL;
		if (!m_pFrame->m_arMDI[vsN].Lookup(key, child))	return;

		child->m_xcaption.SetGroup(triggerN);
		if (triggerN == ALL_GROUP)
			group = -1;

		m_pFrame->m_wizard->setGroupTrigger(key, group);
		child->m_xcaption.SetGroup(triggerN);
		child->PostMessage(WM_NCPAINT, 1, 0);
	}
}

void CFormManager::TransformMDIChildToSingleWindow(int key, int vs)
{
	CChildFrame*	child;
	bool		bMin;

	if (!m_pFrame->m_arMDI[vs].Lookup(key, child))
		return;

	CString mapName;
	int group, font;

	mapName = child->m_mapN;
	group = child->m_xcaption.GetGroup();
	font = child->m_xcaption.GetFontSize();	
	bMin = child->m_bIconic;

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND;
	/*
	CString saveN = _T("");
	bool	bMatrix = false;
	CString	sMatrixMap = _T("");
	
	saveN = m_main->getMatrixMAP(mapName.Mid(2,4));
	
	if (!saveN.IsEmpty())
	{
		bMatrix = true;
		sMatrixMap = "CKXXXX99";
	}

	if (saveN.IsEmpty())
		saveN = m_main->getMatrixMAPEx(mapName.Mid(2,4));

	if (!saveN.IsEmpty() && !bMatrix)
	{
		bMatrix = true;
		sMatrixMap = "CKXXXX98";
	}

	if (saveN.IsEmpty())
		saveN = m_main->getMatrixMAPEx2(mapName.Mid(2,4));
	
	if (!saveN.IsEmpty() && !bMatrix)
	{
		bMatrix = true;
		sMatrixMap = "CKXXXX97";
	}
	
	if (!bMatrix)
	{
		saveN = m_main->getSelectMAP(mapName.Mid(2,4));
		if (!saveN.IsEmpty())
			mapName = saveN;
	}
	
	*/

	vtype = m_pFrame->m_wizard->getFormInfo(mapName, size);
	if (vtype == vtypeERR)
	{
		key = -1;
		return ;
	}

	vwTYPE = vtypeMSK & vtype;	
	vwKIND = (vwTYPE == vtypeSCR) ? SCROLLVIEW : NORMALVIEW;	

	CView*		oldView = child->GetActiveView();
	CView*		newView = m_pFrame->GetSDIView(vwKIND);
	CSChild*	schild= (CSChild *) newView->GetParent();

	if (child->m_bIconic)
		child->actionCaption(IDX_RESTORE, CPoint(0, 0));

	oldView->SetParent(schild);

	CString title = child->m_xcaption.GetTitle();
	newView->SetParent(child);
	child->SetActiveView(newView);

	m_pFrame->RemoveMdi(vs, key);
	m_pFrame->m_arSDI[vs].SetAt(key, schild);
	m_pFrame->m_arSDI[vs].SetAt(key, schild);

	m_pFrame->m_sdiZORDER[vs].InsertAt(0, key);
	schild->m_mapN = child->m_mapN;
	schild->m_xcaption.SetGroup(child->m_xcaption.GetGroup());
	schild->m_xcaption.SetFontSize(child->m_xcaption.GetFontSize());

	schild->m_key = key;
	schild->m_vwTYPE = vtype;

	schild->m_xcaption.SetTitle(title);
	schild->SetMisc(WM_AXIS, m_pFrame, m_axiscall, font, m_pFrame->m_bSDI, m_pFrame->m_axisres, true);
	schild->SetActiveView(oldView);

	CRect rc;
	
	child->GetWindowRect(&rc);
	m_pFrame->m_MClient->ScreenToClient(&rc);

	//schild->SetSize(rc.Width(), rc.Height(), FALSE);  //확인
	schild->MoveWindow(rc.left + 10, rc.top + 10, rc.Width(), rc.Height());
	schild->m_mdiPt = rc.TopLeft();
	schild->m_rectMDI.CopyRect(&rc);
	child->SendMessage(WM_CLOSE);

	if (vs == m_pFrame->m_vsN)
	{
		//schild->SetWindowPos(m_main, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		if (!bMin)
		{
			schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOZORDER);
		}
		else
		{
			schild->m_bIconic = true;
			schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
			schild->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		}
	}
	else
	{
		schild->m_bIconic = bMin;
		schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);			
	}
	//schild->SetTrans(m_main->m_nTrans);  //확인
}

void CFormManager::convertSDI(int key, int vsN)
{
	CChildFrame*	child = NULL;
	if (!m_pFrame->m_arMDI[vsN].Lookup(key, child))
		return;

//	if(child->m_mapN == BONDMAP)
//		return;

	int		groupN, fontN;
	CRect		mdiRc, sdiRc;
	CString		mapname;

	child->GetWindowRect(mdiRc);
	if (!child->m_rectSDI.IsRectEmpty())
		sdiRc.CopyRect(child->m_rectSDI);
	else	sdiRc.CopyRect(&mdiRc);

	m_pFrame->m_MClient->ScreenToClient(&mdiRc);

	mapname = child->m_mapN;
	groupN = child->m_xcaption.GetGroup();
	fontN = child->m_xcaption.GetFontSize();

	if(child->m_mapN == INTERMAP)
		closeChild(m_pFrame->m_Bondkey);

	child->SendMessage(WM_CLOSE);
	CreateSDIbyMDI(mapname, groupN, fontN, mdiRc, sdiRc, vsN);
}

void CFormManager::convertMDI(int key, int vsN)
{
	CSChild*	schild = NULL;
	if (!m_pFrame->m_arSDI[vsN].Lookup(key, schild))
		return;

	if(schild->m_mapN == BONDMAP)
		return;

	int		groupN, fontN;
	CRect		sdiRc;
	CRect		mdiRc;
	CString		mapname;

	schild->GetWindowRect(sdiRc);
	if (!schild->m_rectMDI.IsRectEmpty())
		mdiRc.CopyRect(schild->m_rectMDI);
	else
	{
		mdiRc.CopyRect(&sdiRc);
		m_pFrame->ScreenToClient(&mdiRc);
		if (mdiRc.left < 0)	mdiRc.OffsetRect(abs(mdiRc.left), 0);
		if (mdiRc.top < 0)	mdiRc.OffsetRect(0, abs(mdiRc.top));
	}

	mapname = schild->m_mapN;
	groupN = schild->m_xcaption.GetGroup();
	fontN = schild->m_xcaption.GetFontSize();

	if (m_pFrame->getPopupMap(mapname) && !m_pFrame->m_bSDI)
		return;

	if(schild->m_mapN == INTERMAP)
		closeChild(m_pFrame->m_Bondkey);

	schild->SendMessage(WM_CLOSE);
	CreateMDIbySDI(mapname, groupN, fontN, sdiRc, mdiRc, vsN);
}

void CFormManager::childAll(CString dat)
{
	CString	mapN = dat.Left(L_MAPN);
	dat = dat.Mid(L_MAPN);

	int		key;
	POSITION	pos;
	CChildFrame*	pChild = NULL;
	for (pos = m_pFrame->m_arMDI[m_pFrame->m_vsN].GetStartPosition(); pos; )
	{
		m_pFrame->m_arMDI[m_pFrame->m_vsN].GetNextAssoc(pos, key, pChild);
		if (pChild->m_mapN.Left(L_MAPN) != mapN)
			continue;

		key |= 0xff00;
		m_pFrame->m_wizard->setTrigger(key, dat);
		break;
	}

	CSChild* psChild = NULL;
	for (pos = m_pFrame->m_arSDI[m_pFrame->m_vsN].GetStartPosition(); pos; )
	{
		m_pFrame->m_arSDI[m_pFrame->m_vsN].GetNextAssoc(pos, key, psChild);
		if (pChild->m_mapN.Left(L_MAPN) != mapN)
			continue;

		key |= 0xff00;
		m_pFrame->m_wizard->setTrigger(key, dat);
		break;
	}
}
bool CFormManager::minimizeAll(bool all)
{
	int		key;
	POSITION	pos;
	CChildFrame*	child;
	CSChild*	schild;
	CPoint sp;
	
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		if (!m_pFrame->m_saveALLVS && vsN != m_pFrame->m_vsN)
			continue;
		for (pos = m_pFrame->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			sp = m_pFrame->getMinimizePos(child->m_key);
			child->actionCaption(IDX_MIN, sp);
		}

		for (pos = m_pFrame->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			sp = m_pFrame->getMinimizePos(child->m_key);
			child->actionCaption(IDX_MIN, sp);
		}
	}

	if (!all)	
		return true;

	if (m_pFrame->m_arGPOP.IsEmpty())	
		return true;

	CGPop*	pop;
	for (pos = m_pFrame->m_arGPOP.GetStartPosition(); pos; )
	{
		m_pFrame->m_arGPOP.GetNextAssoc(pos, key, pop);
		pop->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);

	}
	return true;
}

bool CFormManager::closeAll(bool all)
{
	int		key;
	POSITION	pos;
	CChildFrame*	child;
	CSChild*	schild;

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		if (!m_pFrame->m_saveALLVS && vsN != m_pFrame->m_vsN)
			continue;
		for (pos = m_pFrame->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			child->SendMessage(WM_CLOSE, 0, 0);
		}

		for (pos = m_pFrame->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->SendMessage(WM_CLOSE, 0, 0);
		}
	}

	if (!all)	return true;
	if (m_pFrame->m_arGPOP.IsEmpty())	return true;

	CGPop*	pop;
	for (pos = m_pFrame->m_arGPOP.GetStartPosition(); pos; )
	{
		m_pFrame->m_arGPOP.GetNextAssoc(pos, key, pop);
		pop->SendMessage(WM_CLOSE, 0, 0);
	}
	return true;
}

template<typename T>
void CFormManager::refreshMap(T child, int key)
{
	int		size, vwTYPE, nCx, nCy;
	CSize		frameGap;
	CString		mapname;

	mapname = child->m_mapN.Left(L_MAPN);	
	vwTYPE = m_pFrame->m_wizard->getFormInfo(mapname, size);
	frameGap = m_pFrame->GetFrameGap();
	if (m_pFrame->isShowLinkbar(mapname))		frameGap.cy += LBAR_HEIGHT;
	frameGap.cy += XCAP_HEIGHT;

	child->m_width  = nCx = LOWORD(size) + frameGap.cx;
	child->m_height = nCy = HIWORD(size) + frameGap.cy;

	if (!m_pFrame->m_wizard->loadMap(mapname, key, size))
		return;

	nCx = child->m_width;
	nCy = child->m_height;
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE | SWP_NOZORDER);
}


void CFormManager::refreshMap(int key)
{
	CChildFrame*	child = nullptr;
	CSChild*	schild = nullptr;

	if (m_pFrame->m_arMDI[m_pFrame->m_vsN].Lookup(key, child))
		refreshMap(child, key);
	else if (m_pFrame->m_arSDI[m_pFrame->m_vsN].Lookup(key, schild))
		refreshMap(schild, key);
}

void CFormManager::hidemap(int mapkey)
{
	int		key;
	POSITION	pos;
	CChildFrame*	child;
	CSChild*	schild;

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		for (pos = m_pFrame->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			if(mapkey == key)
				child->ShowWindow(SW_HIDE);
		}

		for (pos = m_pFrame->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			if(mapkey == key)
				schild->ShowWindow(SW_HIDE);
		}
	}
}

bool CFormManager::hideAll(bool bAll)
{
	int		key;
	POSITION	pos;
	CChildFrame*	child;
	CSChild*	schild;

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN4; vsN++)
	{
		for (pos = m_pFrame->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			child->ShowWindow(SW_HIDE);
		}

		for (pos = m_pFrame->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_pFrame->m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->ShowWindow(SW_HIDE);
		}
	}
	
	CGPop*	pop = nullptr;
	for (pos = m_pFrame->m_arGPOP.GetStartPosition(); pos; )
	{
		m_pFrame->m_arGPOP.GetNextAssoc(pos, key, pop);
		pop->ShowWindow(SW_HIDE);
	}
	return true;
}

template<typename T>
long CFormManager::runHooked(T t, WPARAM wParam, LPARAM lParam)
{
	return t->runHookedKey(wParam, lParam);
}

long CFormManager::doFormKey(WPARAM wParam, LPARAM lParam)
{
	//CAxChild *pChild = nullptr;
	//pChild = m_pFrame->m_bSDI ? dynamic_cast<CAxChild*>(m_pFrame->GetActiveWindow()) : dynamic_cast<CChildFrame *>(m_pFrame->MDIGetActive());
	//if (pChild == nullptr)
	//	return 0;

	//return runHooked(pChild, wParam, lParam);

	return 0;
}

long CFormManager::DoPopupKey(WPARAM wParam, LPARAM lParam)
{
	if (m_pFrame->m_modal != NULL)
		return 0;

	CWnd* pWnd = m_pFrame->GetActiveWindow();
	int key; CGPop* popup = NULL;

	for (POSITION pos = m_pFrame->m_arGPOP.GetStartPosition(); pos; )
	{
		m_pFrame->m_arGPOP.GetNextAssoc(pos, key, popup);
		if (popup == pWnd)
		{
			if (runHooked(popup, wParam, lParam))
				return 1;
			else
				return 2;
		}
	}
	return 0;
}

bool CFormManager::CheckSDIMap(int key)
{
	CSChild*	child = NULL;
	if (m_pFrame->m_arSDI[m_pFrame->m_vsN].Lookup(key, child))
		return true;
	else
		return false;
}

void CFormManager::LinkOPEN(int dat, CString mapN)
{
	int	key = HIWORD(dat);
	int	group = LOWORD(dat);
	CString	data, mapname;

	m_pFrame->getScreenData(key, data, FALSE);
	data.Replace(";", "\n");
	mapname.Format("%s%s", mapN, data);
	ChangeChild(mapname, group);
}

