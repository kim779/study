// MapHelper.cpp
//
#include "stdafx.h"
#include "axis.h"
#include "MainFrm.h"
#include "childFrm.h"
#include "SChild.h"
#include "axisdoc.h"
#include "axisview.h"
#include "axscrollview.h"
#include "axtabview.h"

#include "GPop.h"
#include "MPop.h"

#include "axMisc.h"
#include "axMsg.hxx"
#include "../dll/sm/smheader.h"
#include "../dll/sm/TransparentMgr.h"
#include "BkWnd.h"

#include "MapHelper.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable : 6273)


class GS
{
public:
	static int GetSystemMetrics(int index);
};

int GS::GetSystemMetrics(int index)
{
	/**
	switch (index)
	{
	case SM_CXBORDER:	return 2;
	case SM_CYBORDER:	return 2;
	case SM_CXFRAME:	return 2;
	case SM_CYFRAME:	return 2;
	case SM_CXEDGE:		return 1;
	case SM_CYEDGE:		return 1;
	case SM_CYCAPTION:  return 20;
	case SM_CYSMCAPTION:return 20;
	}
	**/

	return ::GetSystemMetrics(index);
}

CMapHelper::CMapHelper(CMainFrame* main)
{
	m_main = main;
}

int CMapHelper::CreateTabView(CString mapN, int trigger, WINDOWPLACEMENT wndpl, BOOL showChild, int vsN)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateTabView[wndpl] : mapN=[%s] trigger=[%d] vsN=[%d]\n",
													mapN,   trigger,	vsN);
OutputDebugString(s);
*/
	if (vsN == -1)	
		vsN = m_main->m_vsN;

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;
	
	CString mapname = mapN.Left(L_MAPN);

	int vtype = 0, size = 0, key = 0;
	const int vwTYPE = 0;
	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	CView*			view  = m_main->GetNewView(TABLISTVIEW);
	CChildFrame*	child = (CChildFrame *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int		nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);//m_key = key;

	m_main->AddMdi(vsN, key, child);
	m_main->m_hooklist[vsN].InsertAt(0, key);

	child->SetTabView(true);
	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;
	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(vsN, key);
		m_main->m_hooklist[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	m_main->set_Trigger(key, trigger, vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, vwTYPE == vtypeWND ? false : true);

	if (showChild)
		child->SetWindowPlacement(&wndpl);
	else
	{
		child->ShowWindow(SW_HIDE);
		wndpl.showCmd = SW_HIDE;
		child->SetWindowPlacement(&wndpl);
	}
#ifdef DF_USE_CPLUS17
	((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu.get(), mapname, axiscall, true);
#else
	((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu, mapname, axiscall,true);
#endif
	
	m_main->add_history(mapname);
	m_main->m_bar1->set_Maps(mapname.Left(L_MAPN));
	m_main->addTool_history(mapname);
#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu.get(), vsN);
#else
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu, vsN);
#endif

	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string, vsN);
	}
		
	m_main->SetForegroundWindow();
	m_main->maxChild();
	m_main->m_activeMapN = child->m_mapN;

	m_main->writeOpenedList();	
	
	return key;
}

int CMapHelper::CreateTabView(CString mapN, int trigger, int key, int position, CPoint point)
{
	if (m_main->m_bSDI)	
		return CreateSDI_Tabview(mapN, trigger, key, position, point);

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;

	CString mapname = mapN.Left(L_MAPN);
	int vtype = 0,  size = 0;
	const int vwTYPE = 0;
	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	CView*		view  = m_main->GetNewView(TABLISTVIEW);
	CChildFrame*	child = (CChildFrame *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();
	
	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);//m_key = key;
	
	m_main->AddMdi(m_main->m_vsN, key, child);
	m_main->m_hooklist[m_main->m_vsN].InsertAt(0, key);

	child->SetTabView(true);
	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx - 1;
	nCy = HIWORD(size) + frameGap.cy - 1;
	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(m_main->m_vsN, key);
		m_main->m_hooklist[m_main->m_vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	m_main->set_Trigger(key, trigger);
	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, vwTYPE == vtypeWND ? false : true);

	if (m_main->IsWebMenu(child->m_mapN))
	{	
		//OutputDebugString("IB980000 HIDE\n");
		child->SetWindowPos(NULL, 0, -10000/*10000*/, nCx, nCy, SWP_HIDEWINDOW);
		
		child->ShowWindow(SW_HIDE);
	}
	else
	{
		const CPoint	sp = child->GetSP();
		switch (m_main->m_posChild)
		{
		default:
		case 0:
			if (!position)	
				child->SetWindowPos(NULL, sp.x, sp.y, nCx, nCy, SWP_SHOWWINDOW);
			else if (position == -1)
			{
				
				m_main->m_MClient->ScreenToClient(&point);
				child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, SWP_SHOWWINDOW);
			}
			else
			{
				child->SetWindowPos(NULL, 0, 0/*POS_CHILDMOVE*/, nCx, nCy, SWP_HIDEWINDOW);
				
				child->CenterWindow();
				child->ShowWindow(SW_SHOW);
			}
			break;
		case 1:	
			if (position == -1)
			{
				m_main->m_MClient->ScreenToClient(&point);
				child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, SWP_SHOWWINDOW);
			}
			else	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_SHOWWINDOW);	
			break;
		case 2:	
			if (position == -1)
			{
				m_main->m_MClient->ScreenToClient(&point);
				child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, SWP_SHOWWINDOW);
			}
			else
			{
				child->SetWindowPos(NULL, 0, 10000, nCx, nCy, SWP_HIDEWINDOW);
				child->CenterWindow();
				child->ShowWindow(SW_SHOW);
			}
			break;
		}

		if (position == CenterPOS)	
			child->CenterWindow();
	}
#ifdef DF_USE_CPLUS17
	((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu.get(), mapname, axiscall, true);
#else
	((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu, mapname, axiscall, true);
#endif

	m_main->add_history(mapname);
	m_main->m_bar1->set_Maps(mapname.Left(L_MAPN));
	m_main->addTool_history(mapname);

#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu.get());
#else
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu);
#endif
	
	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	
	/*
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN)))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
	}else{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
		
	}
	*/

	m_main->SetForegroundWindow();
	m_main->maxChild();
	m_main->m_activeMapN = child->m_mapN;
	m_main->writeOpenedList();	

	return key;
}

int CMapHelper::ChangeTabView(CString mapN, int trigger, int key, int position, CPoint point)
{
CString s;
/*
s.Format("[AXIS]_ [map] ChangeTabView : mapN=[%s] trigger=[%d] key=[%d] position=[%d] x=[%d] y=[%d]\n", 
											mapN, trigger, key, key,     position  , point.x, point.y);
OutputDebugString(s);
*/
WINDOWPLACEMENT	pl{};

	if (m_main->m_bSDI)	
		return ChangeSDITabView(mapN, trigger, key, position, point);

	CString	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

// 	s.Format("TAB MAPNAME : %s\n",mapN);
// 	OutputDebugString(s);
	
	CChildFrame*	child = NULL;
	if (!m_main->m_arMDI[m_main->m_vsN].Lookup(key, child))
	{
		//OutputDebugString("NOT LOOKUP\n");
		if(!m_main->m_screenNew || key == 3)
		{
			if (!m_main->IsTabChange(mapN.Left(L_MAPN), key))
			{
				//OutputDebugString("TAB CREATE\n");
				return CreateTabView(mapN, trigger, 0, position, point);
			}
			else
			{
				if (!m_main->m_arMDI[m_main->m_vsN].Lookup(key, child))
				{
					//OutputDebugString("CHANGE SDI\n");
					return ChangeSDITabView(mapN, trigger, key, position, point);
				}
			}
		}
		else
		{
			//중복 화면 띄울때 사용
			//return CreateTabView(mapN, trigger, 0, position, point);
			CSChild*	schild = NULL;

			if (m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild))
			{
				//OutputDebugString("CHANGE SDI\n");
				return ChangeSDITabView(mapN, trigger, key, position, point);
			}

			const bool flag = false;
			int tkey = key;
			for (POSITION pos = m_main->m_arMDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arMDI[m_main->m_vsN].GetNextAssoc(pos, tkey, child);
// 				s.Format("RUN MAP NAME : %s		%d\n",child->m_mapN,child->m_bIconic);
// 				OutputDebugString(s);
				if(child->m_mapN == mapN)
				{
					int		value{};
					const long		rc = 0;
					CPoint		sp(-1, -1);
					CString		mapname;
					
					child->GetWindowPlacement(&pl);
					value = GetSystemMetrics(SM_CYCAPTION);
					value += GetSystemMetrics(SM_CXFRAME);
					value += GetSystemMetrics(SM_CXBORDER);
					sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
					sp.Offset(value, value);
					
					// 					value = child->m_xcaption.GetGroup();
					// 					
					// 					// 						s.Format("MAP GROUP : %d\n",value);
					// 					// 						OutputDebugString(s);
					// 					switch (value)
					// 					{
					// 					case 0:/*none group*/
					// 					case 99:/*all group*/	break;
					// 					case 1:case 2: case 3:case 4: case 5:
					// 						value++;
					// 						if (value > 5)	value = 1;
					// 						break;
					// 					default:	
					// 						value = 1;	break;
					// 					}		
					
					CString strValue = "";
					
					if(!m_main->m_groupBymapN.Lookup(child->m_mapN,strValue))
					{
						strValue = _T("1");
					}

// 					s.Format("RUN MAP NAME : %s	GROUP : %s\n",child->m_mapN,strValue);
// 					OutputDebugString(s);
					
					if(strValue == "")
					{
						strValue = _T("1");
					}

					value = atoi(strValue);
					
					if(value == 10)
					{
						value = 1;
					}
					else
					{
						value += 1;
					}
					
					//CString s;
// 					s.Format("RUN MAP NAME : %s	GROUP : %d\n",child->m_mapN,value);
// 					OutputDebugString(s);
					
					tkey = CopyScreen(child->m_mapN, value, child->m_xcaption.GetFontSize(), sp);
					
					return tkey;
				}
			}

			for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, tkey, schild);
// 				s.Format("SRUN MAP NAME : %s %d\n",schild->m_mapN,schild->m_bIconic);
// 				OutputDebugString(s);

				if (schild && schild->GetSafeHwnd())
				{
					if(schild->m_mapN == mapN)
					{
						
						int		value{};
					
						CPoint		sp(-1, -1);
						CString		mapname;
						WINDOWPLACEMENT	pl;
						
						schild->GetWindowPlacement(&pl);
						value = GetSystemMetrics(SM_CYCAPTION);
						value += GetSystemMetrics(SM_CXFRAME);
						value += GetSystemMetrics(SM_CXBORDER);
						sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
						sp.Offset(value, value);
						
						CString strValue = "";
						
						if(!m_main->m_groupBymapN.Lookup(schild->m_mapN,strValue))
						{
							strValue = _T("1");
						}
						
						if(strValue == "")
						{
							strValue = _T("1");
						}
						
// 						s.Format("SRUN MAP NAME : %s	GROUP : %d\n",child->m_mapN,strValue);
// 						OutputDebugString(s);
						
						value = atoi(strValue);
						
						if(value == 10)
						{
							value = 1;
						}
						else
						{
							value += 1;
						}
						
						tkey = CopyScreen(schild->m_mapN, value, schild->m_xcaption.GetFontSize(), sp);
						
						return tkey;
					}
				}
			}

			//OutputDebugString("CreateTabView\n");
			
			return CreateTabView(mapN, trigger, 0, position, point);
		}
	}

// 	s.Format("LOOKUP MAPNAME : %s\n",child->m_mapN);
// 	OutputDebugString(s);

	int vwTYPE = 0, size = 0;
	CView* view = child->GetActiveView();

	if (!GetMapInfo(mapname, vwTYPE, size))
		return 0;

	//WINDOWPLACEMENT	pl;
	if (!child->m_mapN.CompareNoCase(mapN))
	{
		child->GetWindowPlacement(&pl);
		if (pl.showCmd & SW_SHOWMINIMIZED)
		{
			pl.showCmd = SW_RESTORE;
			child->SetWindowPlacement(&pl);
			((CAxTabView *)view)->JustWindow(mapname);
		}
		else
			child->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
		return 0;
	}

	int	waitkey{};
	for (int ii = 0; ii < m_main->m_waitlist.GetSize(); ii++)
	{
		waitkey = m_main->m_waitlist.GetAt(ii);
		if (waitkey == key)	
			return key;
	}
	
	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();    //탭이동시
	frameGap.cy += XCAP_HEIGHT + TABH;

	nCx = LOWORD(size) + frameGap.cx - 1;  
	nCy = HIWORD(size) + frameGap.cy - 1;  

	child->SetSize(nCx, nCy);
	
	child->SetMapName(axiscall, mapname);

	BOOL	rc = FALSE, changeFont = FALSE;
	long	ret = 0;
	const int	fSize = child->m_xcaption.GetFontSize();
 
	m_main->m_fit = false;

	if (m_main->m_fontSize != fSize)	
		changeFont = TRUE;

	if (changeFont)
	{
		m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, -1), MAKELONG(0, fSize));
	}
	
	/* TAB 체인지할 경우 기존 화면타입을 변환하기 위하여 아래와 같이 코딩함 - 2010.07.29 by warship */
	/* 2012.08.04 김덕기 - 기존 주석처리 되어있던 부분을 해제하여 버그 수정 후 사용
	/*---------------------------------------------------*/
	///*
// 	int oldKey = key;
// 	m_main->m_arMDI[m_main->m_vsN].RemoveKey(key);
// 
// 	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
// 	{
// 		for (int value = 0; value < m_main->m_hooklist[vsN].GetSize(); value++)
// 		{
// 			if (key != m_main->m_hooklist[vsN].GetAt(value))
// 				continue;
// 			m_main->m_hooklist[vsN].RemoveAt(value);
// 		}
// 	}
// 	
// 	m_main->m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4), (short)oldKey);
// 	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,	(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vwTYPE, oldKey);
// 	child->m_vwTYPE = vwTYPE;
// 	child->m_key = key;
// 	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, vwTYPE == vtypeWND ? false : true);
// 	((CAxTabView*)view)->SetInfo(key);
// 	m_main->m_arMDI[m_main->m_vsN].SetAt(key, child);
// 	m_main->m_hooklist[m_main->m_vsN].InsertAt(0, key);
// 	m_main->set_Trigger(key, child->m_xcaption.GetGroup(), m_main->m_vsN);
// 	if (m_main->m_bar2)
// 		m_main->m_bar2->change_Key(oldKey, key);
	//*/
	/*---------------------------------------------------*/
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);

	if (changeFont)
	{
		m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, -1), MAKELONG(0, m_main->m_fontSize));
	}
	if (!rc)
	{
		child->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		return 0;
	}

	CountMap(mapN);

	if (view)	
		view->Invalidate();

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;
	
	child->SetMapName(axiscall, mapname.Left(L_MAPN));

	m_main->add_history(mapname);
	child->GetWindowPlacement(&pl);

	if (pl.showCmd & SW_SHOWMINIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		pl.rcNormalPosition.right = pl.rcNormalPosition.left + nCx;
		pl.rcNormalPosition.bottom = pl.rcNormalPosition.top + nCy;
		child->SetWindowPlacement(&pl);
	}	
	else
		child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE);
	
	((CAxTabView *)view)->ChangeTab(mapname);
	if (!m_main->m_fit)
		((CAxTabView *)view)->JustWindow(mapname);
		
	((CAxTabView *)view)->SetFocus();

	m_main->m_bar1->set_Maps(mapN.Left(L_MAPN));
	m_main->addTool_history(mapN);
#ifdef DF_USE_CPLUS17
	m_main->m_bar2->change_Info(key, mapname, m_main->m_tMenu.get());
#else
	m_main->m_bar2->change_Info(key, mapname, m_main->m_tMenu);
#endif
	((CAxTabView *)view)->JustWindow(mapname);
	
	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}

	/*
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN)))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
	}else{
		
		if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
	}
	*/
	m_main->m_activeMapN = child->m_mapN;

	if (child->m_xcaption.IsMax())	child->actionCaption(IDX_MAX, CPoint(-1,-1));

	m_main->HideChildButton(child->m_key);
	return key;
}

int CMapHelper::ChangeSDITabView(CString mapN, int trigger, int key, int position, CPoint point)
{
CString s;
/*
s.Format("[AXIS]_ [map] ChangeSDITabView : mapN=[%s] trigger=[%d] key=[%d] position=[%d] x=[%d] y=[%d]\n", 
												mapN, trigger,      key,       position,  point.x, point.y);
OutputDebugString(s);
*/

	CString	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;
	
	CSChild*	schild = NULL;
	if (!m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild))
	{
		if(!m_main->m_screenNew || key == 3)
		{
			if (!m_main->IsTabChange(mapN.Left(L_MAPN), key))
				return CreateSDI_Tabview(mapN, trigger, 0, position, point);
			else
			{
				if (!m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild))
					return 0;
			}
		}
		else
		{
			const bool flag = false;
			int tkey = key;

			for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, tkey, schild);
				if (schild && schild->GetSafeHwnd() && schild->m_bIconic)
				{
					if(schild->m_mapN == mapN)
					{
						
						int		value{};
						const long		rc = 0;
						
						CPoint		sp(-1, -1);
						CString		mapname;
						WINDOWPLACEMENT	pl;
						
						schild->GetWindowPlacement(&pl);
						value = GetSystemMetrics(SM_CYCAPTION);
						value += GetSystemMetrics(SM_CXFRAME);
						value += GetSystemMetrics(SM_CXBORDER);
						sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
						sp.Offset(value, value);

						CString strValue = "";
						
						if(!m_main->m_groupBymapN.Lookup(schild->m_mapN,strValue))
						{
							strValue = _T("1");
						}
						
						if(strValue == "")
						{
							strValue = _T("1");
						}
						
						// 						CString s;
						// 						s.Format("RUN MAP NAME : %s	GROUP : %d\n",child->m_mapN,strValue);
						// 						OutputDebugString(s);
						
						value = atoi(strValue);
						
						if(value == 10)
						{
							value = 1;
						}
						else
						{
							value += 1;
						}
						
						tkey = CopyScreen(schild->m_mapN, value, schild->m_xcaption.GetFontSize(), sp);
						
						return tkey;
					}
				}
			}
			
			return CreateSDI_Tabview(mapN, trigger, 0, position, point);
		}
	}

	int vwTYPE = 0, size = 0;
	CView* view = schild->GetActiveView();

	if (!GetMapInfo(mapname, vwTYPE, size))
		return 0;

	WINDOWPLACEMENT	pl{};
	if (!schild->m_mapN.CompareNoCase(mapN))
	{
		schild->GetWindowPlacement(&pl);
		if (pl.showCmd & SW_SHOWMINIMIZED)
		{
			pl.showCmd = SW_RESTORE;
			schild->SetWindowPlacement(&pl);
			((CAxTabView *)view)->JustWindow(mapname);
		}
		else
			schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER);
		return 0;
	}

	int	waitkey{};
	for (int ii = 0; ii < m_main->m_waitlist.GetSize(); ii++)
	{
		waitkey = m_main->m_waitlist.GetAt(ii);
		if (waitkey == key)
			return key;
	}
	
	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();
	frameGap.cy += XCAP_HEIGHT + TABH;

	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;
	schild->SetSize(nCx, nCy);

	schild->SetMapName(axiscall, mapname);

	BOOL	rc = FALSE, changeFont = FALSE;
	long	ret = 0;
	const int	fSize = schild->m_xcaption.GetFontSize();

	m_main->m_fit = false;
	if (m_main->m_fontSize != fSize)	
		changeFont = TRUE;

	if (changeFont)
	{
		m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, -1), MAKELONG(0, fSize));
	}

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);

	if (changeFont)
	{
		m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, -1), MAKELONG(0, m_main->m_fontSize));
	}
	
	if (!rc)
	{
		schild->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		return 0;
	}

	CountMap(mapN);

	if (view)	
		view->Invalidate();

	nCx = schild->GetSize().cx;
	nCy = schild->GetSize().cy;

	schild->SetMapName(axiscall, mapname.Left(L_MAPN));

	schild->GetWindowPlacement(&pl);
	if (pl.showCmd & SW_SHOWMINIMIZED)
	{
		pl.showCmd = SW_RESTORE;
		pl.rcNormalPosition.right = pl.rcNormalPosition.left + nCx;
		pl.rcNormalPosition.bottom = pl.rcNormalPosition.top + nCy;
		schild->SetWindowPlacement(&pl);
	}
	else	
		schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE);
	
	((CAxTabView *)view)->ChangeTab(mapname);
	if (!m_main->m_fit)	((CAxTabView *)view)->JustWindow(mapname);
	((CAxTabView *)view)->SetFocus();

	m_main->m_bar1->set_Maps(mapN.Left(L_MAPN));
	m_main->add_history(mapname);
	m_main->addTool_history(mapN);
	
#ifdef DF_USE_CPLUS17
	m_main->m_bar2->change_Info(key, mapname, m_main->m_tMenu.get());
#else
	m_main->m_bar2->change_Info(key, mapname, m_main->m_tMenu);
#endif
	((CAxTabView *)view)->JustWindow(mapname);
	
	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = schild->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	
	m_main->m_activeMapN = schild->m_mapN;
	m_main->HideChildButton(key);
	return key;
}

int CMapHelper::CreateChild(CString mapN, int trigger, WINDOWPLACEMENT wndpl, BOOL showChild, int vsN,CString strTag)
{
CString s;
/*Format("[AXIS]_ [map] CREATE CHILD1[wndpl] : mapN=[%s] trigger=[%d] showChild=[%d] vsN=[%d] strTag=[%s]\n",
													mapN,   trigger,       showChild, vsN,			strTag);
OutputDebugString(s);
*/
	if (vsN == -1)	
		vsN = m_main->m_vsN;

	CString domino_data;

	if(mapN == "IB714200" || mapN == "IB714500")
	{
		if(m_main->m_miniWid == NULL || !m_main->m_miniWid->GetSafeHwnd())
		{
			OutputDebugString("RECREATE MINI WID\n");
			const char* trust = "IB0000X8";
			ChangeChild(trust, 1, 0, CenterPOS);
		}
		
		if(!m_main->m_bSDI)
		{
			const CWnd* base = m_main->m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_WINE,(WPARAM)(LPCSTR)Axis::user,(LPARAM)(LPCSTR)mapN);
		}
		
		// 		CString url;
		// 		url.Format("http://www.futurewiz.co.kr/ibk/winechart/bighand/index.asp?id=%s&handle=%d",Axis::user,m_main->GetSafeHwnd());
		// 		::ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOW);
		
		return TRUE;
	}
	else if(mapN == "IB715100" || mapN == "IB715200")
	{
		mapN.Replace("00","01");

		if(m_main->m_miniWid == NULL || !m_main->m_miniWid->GetSafeHwnd())
		{
			OutputDebugString("RECREATE MINI WID\n");
			const char* trust = "IB0000X8";
			ChangeChild(trust, 1, 0, CenterPOS);
		}
		
		if(!m_main->m_bSDI)
		{
			const CWnd* base = m_main->m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_CHART,(WPARAM)(LPCSTR)Axis::user,(LPARAM)(LPCSTR)mapN);
		}

		m_main->m_bStart = true;
		
		return TRUE;
	}

	if (!IsValidMap(mapN.Left(L_MAPN)) && (!m_main->IsSuperUser()))	
		return 0;
	if (m_main->IsTabView(mapN.Left(L_MAPN)))
		return CreateTabView(mapN+strTag, trigger, wndpl, showChild, vsN);

	if (mapN.Left(L_MAPN) == MAPN_GONGJI)
		return CreateBkNotice(mapN, trigger);

	if (mapN.Left(L_MAPN) == MAPN_STOPLOSS)
	{
		m_main->StopLoss();
		return 0;
	}
	else if (mapN.Left(L_MAPN) == MAPN_FOSTOPLOSS)
	{
		m_main->FOStopLoss();
		return 0;
	}

	CString mapname, saveN = mapN;
	mapname = mapN;

	int	vtype = 0, vwTYPE = 0, vwKIND{}, size = 0, key = 0;

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;
	
	CView	*view = m_main->GetNewView(vwKIND);
	CChildFrame *child = (CChildFrame *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;
	if (vwTYPE == vtypeSCR)
	{	// scroll view
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	
		((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;

	//m_main->m_arMDI[vsN/*m_main->m_vsN*/].SetAt(key, (CChildFrame *) child);
	//RedrawVS();
	// dtitle
// 	CString s;
// 	s.Format("VSN : %d	KEY : %d	child : %s\n",vsN,key,child->m_mapN);
// 	OutputDebugString(s);
	m_main->AddMdi(vsN, key, child);
	m_main->m_hooklist[vsN/*m_main->m_vsN*/].InsertAt(0, key);
	
	child->m_key	 = key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, saveN.Left(L_MAPN));
	nCx = LOWORD(size) + frameGap.cx - 1;
	nCy = HIWORD(size) + frameGap.cy - 1;
	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;

// 	CString tmpTag;
// 
// 	tmpTag = strTag;
// 
// 	while(tmpTag.GetLength() > 0 && tmpTag.Find("ed_focod") > -1)
// 	{
// // 		if(strTag.Find("\n") > -1)
// // 		{
// // 			strTag = strTag.Mid(0,strTag.Find("\n"));
// // 		}
// 		domino_data = Axis::Parse(tmpTag,"\n");
// 
// 		s.Format("DOMINO PARSE : [%s]		MAPN : [%s]\n",domino_data,mapname);
// 		OutputDebugString(s);
// 
// 		if(domino_data.Find("ed_focod") > -1)
// 		{
// 			strTag = domino_data;
// 
// 			break;
// 		}
// 
// // 		domino_data.Replace("40301","ed_focod");
// // 		domino_data.Replace("30301","ed_focod");
// 
// 		//domino_data = strTag;
// 	}

	s.Format("DOMINO : [%s]		MAPN : [%s]\n",strTag,mapname);
	OutputDebugString(s);

	if(IsDRFN(mapN))
	{
// 		strTag.Replace("USRDEFDATA","");
// 		strTag.TrimLeft();
// 		strTag.TrimRight();

//		strTag.Remove(',');

		CString s;
// 		s.Format("DRFN USRDEFDATA : %s\n",strTag);
// 		OutputDebugString(s);

		m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname+strTag, size, false/*true*/);
	}
	else
	{
		//if(strTag.Find("1301") > -1)
		if(strTag.Find("1301") > -1 || strTag.Find("40301") > -1 || strTag.Find("30301") > -1 || strTag.Find("ed_focod") > -1)
		{
			if(mapN.Mid(0,6) == "IB2200")
			{
				m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);
			}
			else
			{
				m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
							(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname+strTag, size, false/*true*/);
			}
		}
		else
		{
			m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);
		}
	}

	if (!rc)
	{
		child->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		m_main->RemoveMdi(vsN, key);
		m_main->m_hooklist[vsN/*m_main->m_vsN*/].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		child->ShowBackGround(true);
		//position = CenterPOS;
		m_main->m_BackGroundKey = child->m_key;
		child->CenterWindow();
	}

	m_main->set_Trigger(key, trigger, vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, vwTYPE == vtypeWND ? false : true);

	/* for z-order  move screen out*/
	child->SetWindowPlacement(&wndpl);

	if (!showChild)	
		child->ShowWindow(SW_HIDE);

	m_main->add_history(saveN.Left(L_MAPN));
	m_main->m_bar1->set_Maps(saveN.Left(L_MAPN));
	m_main->addTool_history(saveN);
#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, saveN.Left(L_MAPN), m_main->m_tMenu.get(), vsN);
#else
	m_main->m_bar2->add_Button(key, saveN.Left(L_MAPN), m_main->m_tMenu, vsN);
#endif
	
	if (!m_main->ExistMenu(saveN.Left(L_MAPN-2)+"00"))
	{
		CString string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	
	m_main->maxChild();

	m_main->m_activeMapN = child->m_mapN;
	//m_main->MRFrame(child); // for Market Radar
	
	m_main->writeOpenedList();	// 20070627 kwon

	if(mapN == "IB202200" || mapN == "IB202700")
	{
		const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
		if (base)
		{
			base->SendMessage(WM_START2022, 0, (LPARAM)strTag.operator LPCTSTR());
		}
	}
// 	else //도미노 데이타를 강제로 트리거
// 	{
// 		m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
// 			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, key), (LPARAM)(const char*)domino_data);
// 	}

	return key;
}

int CMapHelper::CreateSDI(CString mapN, int group, WINDOWPLACEMENT wndpl, BOOL showChild, int vsN)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateSDI[wndpl] : mapN=[%s] group=[%d] vsN=[%d]\n",
												mapN, group,	vsN);
OutputDebugString(s);
*/

	if (vsN == -1)	
		vsN = m_main->m_vsN;
	
	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;

	if (m_main->IsTabView(mapN.Left(L_MAPN)))
		return CreateSDI_Tabview(mapN, group, wndpl, showChild, vsN);

	CString mapname, saveN = mapN;
	mapname = mapN;

	int	vtype = 0, vwTYPE = 0, vwKIND{}, size = 0, key = 0;

	
	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;
	
	CView	*view   = m_main->GetSDIView(vwKIND);
	CSChild *schild = (CSChild *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;
	if (vwTYPE == vtypeSCR)
	{	// scroll view
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else
	{
		((CAxisView *)view)->SetInfo(key);
		view->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	}

	m_main->AddSdi(vsN, key, schild);
	m_main->m_sdiZORDER[vsN].InsertAt(0, key);
	
	schild->m_key	 = key;
	schild->m_vwTYPE = vtype;
	schild->SetMapName(axiscall, saveN.Left(L_MAPN));
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;
	schild->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);
	if (!rc)
	{
		schild->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		m_main->RemoveSdi(vsN, key);
		m_main->m_sdiZORDER[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	m_main->set_STrigger(key, group, vsN);
	schild->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, m_main->m_bSDI, vwTYPE == vtypeWND ? false : true);

	/* for z-order  move screen out*/
	schild->SetWindowPlacement(&wndpl);
	m_main->add_history(saveN.Left(L_MAPN));
	m_main->m_bar1->set_Maps(saveN.Left(L_MAPN));
	m_main->addTool_history(saveN);
#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, saveN.Left(L_MAPN), m_main->m_tMenu.get(), vsN);
#else
	m_main->m_bar2->add_Button(key, saveN.Left(L_MAPN), m_main->m_tMenu, vsN);
#endif
		
	if (!m_main->ExistMenu(saveN.Left(L_MAPN-2)+"00"))
	{
		CString string = schild->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}

	m_main->maxChild();
	m_main->m_activeMapN = schild->m_mapN;

	m_main->writeOpenedList();	
	return key;
}

int CMapHelper::CreateChild(CString mapN, int trigger, int key, int position, CPoint point)
{
CString s;

s.Format("[AXIS]_ [map] CREATE CHILD[position] : mapN=[%s] trigger=[%d] key=[%d] position=[%d] x=[%d] y=[%d]\n", 
														mapN, trigger,    key, position,  point.x, point.y);
OutputDebugString(s);

// 	if (m_main->IsForeignMap(mapN))
// 	{
// 		m_main->RunForeignMap();
// 		return TRUE;
// 	}
	if (m_main->IsNewRealTick(mapN))
	{
		m_main->RunNewRealTick();
		return TRUE;
	}
	else if (m_main->IsPhonePad(mapN))
	{
		m_main->RunPhonePad();
		return TRUE;
	}

	if( mapN == "IB714200" || mapN == "IB714500")
	{
		if(m_main->m_miniWid == NULL || !m_main->m_miniWid->GetSafeHwnd())
		{
			OutputDebugString("RECREATE MINI WID\n");
			const char* trust = "IB0000X8";
			ChangeChild(trust, 1, 0, CenterPOS);
		}
		
		if(!m_main->m_bSDI)
		{
			const CWnd* base = m_main->m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_WINE,(WPARAM)(LPCSTR)Axis::user,(LPARAM)(LPCSTR)mapN);
		}

// 		CString url;
// 		url.Format("http://www.futurewiz.co.kr/ibk/winechart/bighand/index.asp?id=%s&handle=%d",Axis::user,m_main->GetSafeHwnd());
// 		::ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_SHOW);
		
		return TRUE;
	}
	else if(mapN == "IB715101" || mapN == "IB715201")
	{
		//OutputDebugString("7141\n");
		if(m_main->m_miniWid == NULL || !m_main->m_miniWid->GetSafeHwnd())
		{
			OutputDebugString("RECREATE MINI WID\n");
			const char* trust = "IB0000X8";
			ChangeChild(trust, 1, 0, CenterPOS);
		}
		
		if(!m_main->m_bSDI)
		{
			const CWnd* base = m_main->m_miniWid->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WD_CHART,(WPARAM)(LPCSTR)Axis::user,(LPARAM)(LPCSTR)mapN);
		}
		
		return TRUE;
	}
	
	if ((mapN != "IB715100") && (mapN != "IB715200") && (mapN.Mid(6,2) == "00") && (!Axis::devMode) && (!m_main->IsSuperUser()))
	{
		if (!m_main->ExistMenu(mapN.Mid(0,8))) return 0;
	}
	
	if (m_main->m_bSDI || mapN == "IB0000X9")	
		return CreateSDI(mapN, trigger, key, position, point);

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;

	if (m_main->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_main->m_tMenu->GetURL(mapN.Left(L_MAPN));
		m_main->RunWebBrowser(url);
		return 0;
	}
	
	if (m_main->IsTabView(mapN.Left(L_MAPN)))
		return ChangeTabView(mapN, trigger, key, position, point);

	if (mapN.Left(L_MAPN) == MAPN_GONGJI)
		return CreateBkNotice(mapN, trigger);
	
	if (mapN.Left(L_MAPN) == MAPN_STOPLOSS)
	{
		m_main->StopLoss();
		return 0;
	}
	else if (mapN.Left(L_MAPN) == MAPN_FOSTOPLOSS)
	{
		m_main->FOStopLoss();
		return 0;
	}

	

	CString mapname, savemapN = mapN;
 	mapname = mapN;

	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	CView	    *view  = m_main->GetNewView(vwKIND);
	CChildFrame *child = (CChildFrame *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();
	
	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;

	if (vwTYPE == vtypeSCR)
	{	
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	
		((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;

	m_main->AddMdi(m_main->m_vsN, key, child);
	m_main->m_hooklist[m_main->m_vsN].InsertAt(0, key);

	child->m_key	 = key;
	child->m_vwTYPE = vtype;
	
	child->SetMapName(axiscall, savemapN.Left(L_MAPN));//mapname;

	//**frameGap = CSize(2, CAPTION_HEIGHT + 4);
	nCx = LOWORD(size) + frameGap.cx - 1;
	nCy = HIWORD(size) + frameGap.cy - 1;
	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
// 	OutputDebugString("========================================================================\n");
// 	OutputDebugString(mapname+"\n");
// 	OutputDebugString("========================================================================\n");

// 	if(mapN == "IB711200" || mapN == "IB711400")
// 	{
// 		//OutputDebugString("DRFN MAP RUN\n");
// 		CString symbol;
// 
// 		if(mapN == "IB711200")
// 		{
// 			symbol = "1301";
// 		}
// 		else
// 		{
// 			symbol = "ed_focod";
// 		}
// 
// 		m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
// 		(BYTE *)(VTS_I4 VTS_I4), MAKELONG(getHISTORY, 0), (LPARAM)symbol.operator LPCTSTR());
// 
// 		CString strRet = (char*)rc;
// 
// 		if(strRet.IsEmpty())
// 		{
// 			strRet = "";
// 		}
// 
// 		int find = strRet.Find('\t');
// // 		CString s;
// // 		s.Format("SETEDITDATA HISTORY : %s\n",strRet);
// // 		OutputDebugString(s);
// 
// 		CString strDomino;
// 		
// 		if (find < 1) 
// 		{
// 			//OutputDebugString("NOT FOUND\n");
// 			strDomino = "";
// 		}
// 		else
// 		{
// 			CString str = strRet.Left(find);
// 			//strRet.Delete(0, find + 1);
// 
// 			CString code;
// 			CString name;
// 			
// 			find = str.Find(" ");
// 			if (find > 0) {
// 				//_JCode jCodeH;
// 				code = str.Left(find);
// 				name = str.Mid(find + 1);
// 			}
// 
// 			strDomino.Format("%s\t%s",symbol,code);
// 		}
// 
// 		m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
// 				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname+strDomino, size, false/*true*/);
// 	}
// 	else
// 	{-
// 		//OutputDebugString("NOT DRFN MAP RUN\n");
// 		m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
// 				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);
// 	}
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);

	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(m_main->m_vsN, key);
		m_main->m_hooklist[m_main->m_vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	if(mapN == "IB202200" || mapN == "IB202700")
	{
		const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
		if (base)
		{
			base->SendMessage(WM_MENU2022, 0, (LPARAM)0);
		}
	}

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	if(mapN == "IB0000X8")
	{
		m_main->set_Trigger(key, 99);
	}
	else
	{
		m_main->set_Trigger(key, trigger);
	}

	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, vwTYPE == vtypeWND ? false : true);
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		child->ShowBackGround(true);
		position = CenterPOS;
		m_main->m_BackGroundKey = child->m_key;
	}

	const CPoint	sp = child->GetSP();
	const UINT	nFlag = SWP_HIDEWINDOW;
	switch (m_main->m_posChild)
	{
	default:
	case 0:
		if (!position)	
		   child->SetWindowPos(NULL, sp.x, sp.y, nCx, nCy, nFlag);
		else if (position == -1)
		{
			m_main->m_MClient->ScreenToClient(&point);
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		}
		else	
			child->SetWindowPos(NULL, 0, 0/*POS_CHILDMOVE*/, nCx, nCy, nFlag);
		break;
	case 1:	
		if (position == -1)
		{
			m_main->m_MClient->ScreenToClient(&point);
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		}
		else	
			child->SetWindowPos(NULL, 0, 0, nCx, nCy, nFlag);	
		break;
	case 2:	
		if (position == -1)
		{
			m_main->m_MClient->ScreenToClient(&point);
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		}
		else
		{
			child->SetWindowPos(NULL, 0, 0/*10000*/, nCx, nCy, nFlag);
			child->CenterWindow();
		}
		break;
	}

	CRect rect;

	if (position == CenterPOS)	
	{
		child->CenterWindow();
	}
	else if(position == RtopPOS)
	{
		m_main->m_MClient->GetClientRect(&rect);

		child->SetWindowPos(NULL,rect.right - child->GetSize().cx,0,rect.right,0,nFlag);
	}
	else if(position == RbottomPOS)
	{
		m_main->m_MClient->GetClientRect(&rect);
		
		child->SetWindowPos(NULL,rect.right - child->GetSize().cx,rect.bottom - child->GetSize().cy,rect.right,nCy,nFlag);
	}

	s.Format("CREATE MAP NAME [%s]\n",child->m_mapN);
	OutputDebugString(s);

	if (m_main->IsWebMenu(child->m_mapN))
	{	
		OutputDebugString("IB980000 HIDE\n");
		child->SetWindowPos(NULL, 0, -10000/*10000*/, nCx, nCy, nFlag);
		
		child->ShowWindow(SW_HIDE);
	}
	else if(child->m_mapN == "IB0000X8")
	{
		child->ShowWindow(SW_HIDE);
		//child->SendMessage(WD_ADD_CODE,(WPARAM)0,(LPARAM)(LPCSTR)"TR");
		m_main->m_miniWid = child;
		m_main->m_viewHist = child->GetActiveView();
		const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
		base->SendMessage(WD_SETWND,(WPARAM)m_main->m_bMiniMode,(LPARAM)(LONG)m_main);
		//m_main->m_miniWid = (CWnd*)child;
		return key;
	}else if (child->m_mapN != "IB820850")
		child->ShowWindow(SW_SHOW);

	m_main->add_history(savemapN.Left(L_MAPN));
	m_main->m_bar1->set_Maps(savemapN.Left(L_MAPN));
	m_main->addTool_history(savemapN);
#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, savemapN.Left(L_MAPN), m_main->m_tMenu.get());
#else
	m_main->m_bar2->add_Button(key, savemapN.Left(L_MAPN), m_main->m_tMenu);
#endif
	
	/*
	if (!m_main->ExistMenu(savemapN.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	*/
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN)))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
	}else{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
		
	}

	m_main->SetForegroundWindow();
	m_main->maxChild();
	m_main->m_activeMapN = child->m_mapN;
	m_main->MRFrame(child); // for Market Radar

	m_main->writeOpenedList();	// 20070627 kwon

	OutputDebugString("CreateChild");

	return key;
}


int CMapHelper::CreateSDI(CString mapN, int group, int key, int position, CPoint point)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateSDI[point] : mapN=[%s] group=[%d] key=[%d]\n",
											mapN,     group,     key);
OutputDebugString(s);	
*/

	CString mapname = mapN.Left(L_MAPN);
	if (!IsValidMap(mapname))	
		return 0;
	if (m_main->m_tMenu->IsWeb(mapname))
	{
		CString url = m_main->m_tMenu->GetURL(mapname);
		m_main->RunWebBrowser(url);
		return 0;
	}

	if (m_main->IsTabView(mapname))
		return CreateSDI_Tabview(mapN, group, key, position, point);

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	CView*		view = m_main->GetSDIView(vwKIND);
	CSChild*	child= (CSChild *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;
	if (vwTYPE == vtypeSCR)
	{	
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else
	{
		((CAxisView *)view)->SetInfo(key);
		view->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	}
	
	m_main->AddSdi(m_main->m_vsN, key, child);
	m_main->m_sdiZORDER[m_main->m_vsN].InsertAt(0, key);

	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx-1;
	nCy = HIWORD(size) + frameGap.cy-1;

	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveSdi(m_main->m_vsN, key);
		m_main->m_sdiZORDER[m_main->m_vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	m_main->set_STrigger(key, group, m_main->m_vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, m_main->m_bSDI, vwTYPE == vtypeWND ? false : true);
	
	const UINT	nFlag = SWP_SHOWWINDOW;
	switch (m_main->m_posChild)
	{
	default:
	case 0:
		if (!position)	
			child->SetWindowPos(NULL, child->m_defaultpos.x, child->m_defaultpos.y, nCx, nCy, nFlag);
		else if (position == -1)
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else	
			child->SetWindowPos(NULL, 0, 0/*POS_CHILDMOVE*/, nCx, nCy, nFlag);
		break;
	case 1:	
		if (position == -1)
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else	
			child->SetWindowPos(NULL, 0, 0, nCx, nCy, nFlag);	
		break;
	case 2:	
		if (position == -1)
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else
		{
			child->SetWindowPos(NULL, 0, 10000, nCx, nCy, SWP_SHOWWINDOW);		// cej 20070117
			child->CenterWindow();
		}
		break;
	}

	if(mapN == "IB876100")
	{
		child->SetWindowPos(NULL, 0, -23, nCx, nCy+23, nFlag);
		child->MoveWindow(0,-23,nCx,nCy+23,FALSE);
	}
	
	if (position == CenterPOS)	
		child->CenterWindow();

	if(child->m_mapN == "IB0000X8")
	{
		child->ShowWindow(SW_HIDE);
		//child->SendMessage(WD_ADD_CODE,(WPARAM)0,(LPARAM)(LPCSTR)"TR");
		//m_main->m_miniWid = child;
		
		const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
		base->SendMessage(WD_SETWND,(WPARAM)m_main->m_bMiniMode,(LPARAM)(LONG)m_main);
		m_main->m_miniSWid = child;
		return key;
	}
	else if (child->m_mapN == "IB820850")
		child->ShowWindow(SW_HIDE);
	//if (child->m_mapN != "IB820850")
	m_main->addScreenList(child->m_mapN, child->m_key, m_main->m_vsN);
	m_main->maxChild();

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::CreateSDI_Tabview(CString mapN, int group, int key, int position, CPoint point)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateSDI_Tabview [point] : mapN=[%s] group=[%d] key=[%d]\n", 
														mapN, group, key);
OutputDebugString(s);
*/
	CString mapname = mapN.Left(L_MAPN);
	if (!IsValidMap(mapname))	
		return 0;

	int	vtype = 0, vwTYPE = 0, size = 0;

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;

	CView*		view = m_main->GetSDIView(TABLISTVIEW);
	CSChild*	child= (CSChild *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	BOOL	rc = FALSE;
	CSize	frameGap = m_main->GetFrameGap();

	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);
	view->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	
	child->SetTabView(true);
	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	child->SetSize(nCx, nCy);

	m_main->AddSdi(m_main->m_vsN, key, child);
	m_main->m_sdiZORDER[m_main->m_vsN].InsertAt(0, key);

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveSdi(m_main->m_vsN, key);
		m_main->m_sdiZORDER[m_main->m_vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	m_main->set_STrigger(key, group, m_main->m_vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, m_main->m_bSDI, vwTYPE == vtypeWND ? false : true);

	const UINT	nFlag = SWP_SHOWWINDOW;
	switch (m_main->m_posChild)
	{
	default:
	case 0:
		if (!position)	
			child->SetWindowPos(NULL, child->m_defaultpos.x, child->m_defaultpos.y, nCx, nCy, nFlag);
		else if (position == -1)
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else	
			child->SetWindowPos(NULL, 0, 0/*POS_CHILDMOVE*/, nCx, nCy, nFlag);
		break;
	case 1:	
		if (position == -1)
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else	
			child->SetWindowPos(NULL, 0, 0, nCx, nCy, nFlag);	
		break;
	case 2:	
		if (position == -1)
			child->SetWindowPos(NULL, point.x, point.y, nCx, nCy, nFlag);
		else
		{
			child->CenterWindow();
			child->SetWindowPos(NULL, 0, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
		}
		break;
	}
	if (position == CenterPOS)	
		child->CenterWindow();

#ifdef DF_USE_CPLUS17
	((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu.get(), mapname, axiscall);
#else
	((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu, mapname, axiscall);
#endif

	m_main->addScreenList(child->m_mapN, child->m_key, m_main->m_vsN);
	m_main->maxChild();

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::CreateSDI_Tabview(CString mapN, int group, WINDOWPLACEMENT wndpl, BOOL showChild, int vsN)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateSDI_Tabview [wndpl] : mapN=[%s] group=[%d] vsN=[%d]\n", 
															mapN, group, vsN);
OutputDebugString(s);
*/

	if (vsN == -1)	
		vsN = m_main->m_vsN;
	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;

	CString mapname = mapN.Left(L_MAPN);

	int vtype = 0, size = 0, key = 0;
	const int vwTYPE = 0;
	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	CView*		view  = m_main->GetSDIView(TABLISTVIEW);
	CSChild*	schild = (CSChild *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);
	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);//m_key = key;
	view->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	
	m_main->AddSdi(vsN, key, schild);
	m_main->m_sdiZORDER[vsN].InsertAt(0, key);

	schild->SetTabView(true);
	schild->m_key	 = key;
	schild->m_vwTYPE = vtype;
	schild->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	schild->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);
	if (!rc)
	{
		schild->PostMessage(WM_CLOSE);
		m_main->RemoveSdi(vsN, key);
		m_main->m_sdiZORDER[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	m_main->set_STrigger(key, group, vsN);
	schild->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, m_main->m_bSDI, vwTYPE == vtypeWND ? false : true);
	if (showChild)
		schild->SetWindowPlacement(&wndpl);
	else
	{
		schild->ShowWindow(SW_HIDE);
		wndpl.showCmd = SW_HIDE;
		schild->SetWindowPlacement(&wndpl);
	}

#ifdef DF_USE_CPLUS17
	((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu.get(), mapname, axiscall, true);
#else
	((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu, mapname, axiscall, true);
#endif

	m_main->add_history(mapname);
	m_main->m_bar1->set_Maps(mapname.Left(L_MAPN));
	m_main->addTool_history(mapname);

#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu.get(), vsN);
#else
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu, vsN);
#endif
	
	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = schild->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string, vsN);
	}
	/*
	if ((CString(schild->m_mapN).Mid(0,6) == "IB7700") && (CString(schild->m_mapN).Mid(0,8) != "IB770000"))
	{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN)))
		{
			CString	string = schild->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string, vsN);
		}
	}else{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
		{
			CString	string = schild->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string, vsN);
		}
		
	}
	*/
	
	m_main->SetForegroundWindow();
	m_main->maxChild();
	m_main->m_activeMapN = schild->m_mapN;

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}


int CMapHelper::CreateSDIbyMDI(CString mapN, int group, int fontN, CRect mdiRc, CRect sdiRc, int vsN)
{
	
	if (vsN == -1)	
		vsN = m_main->m_vsN;
	if (m_main->IsTabView(mapN))	
		return CreateSDIbyMDI_Tabview(mapN, group, fontN, mdiRc, sdiRc, vsN);

	int	key = 0, vtype = 0;
	int	vwTYPE{}, size{}, vwKIND{};

	if (!GetMapInfo(mapN, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? SCROLLVIEW : NORMALVIEW;

	CView*		view = m_main->GetSDIView(vwKIND);
	CSChild*	child= (CSChild *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	BOOL	rc = FALSE;
	CSize	frameGap = m_main->GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;
	if (vwTYPE == vtypeSCR)
	{	
		((CAxScrollView *)view)->SetInfo(key);
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else
	{
		((CAxisView *)view)->SetInfo(key);
		view->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	}

	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapN);


	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	child->SetSize(nCx, nCy);

	m_main->AddSdi(vsN, key, child);
	m_main->m_sdiZORDER[vsN].InsertAt(0, key);

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveSdi(vsN, key);
		m_main->m_sdiZORDER[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, fontN));
	if (vwTYPE == vtypeFIX)
	{
		nCx = child->GetSize().cx;
		nCy = child->GetSize().cy;
	}
	else
	{
		nCx = sdiRc.Width();
		nCy = sdiRc.Height();
	}
	
	m_main->set_STrigger(key, group, vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, fontN, m_main->m_bSDI, vwTYPE == vtypeWND ? false : true);
	
	if (vsN != m_main->m_vsN)
		child->SetWindowPos(NULL, sdiRc.left, sdiRc.top, nCx, nCy, SWP_HIDEWINDOW);
	else
	{
		child->SetWindowPos(NULL, sdiRc.left, sdiRc.top, nCx, nCy, SWP_SHOWWINDOW);
		child->SetFocus();
		m_main->m_activeMapN = child->m_mapN;
	}

	child->m_mdiPt = mdiRc.TopLeft();
	child->m_rectMDI.CopyRect(&mdiRc);

	m_main->addScreenList(child->m_mapN, child->m_key, vsN);

	m_main->writeOpenedList();	// 20070627 kwon
	
	return key;
}

int CMapHelper::CreateSDIbyMDI_Tabview(CString mapN, int group, int fontN, CRect mdiRc, CRect sdiRc, int vsN)
{
	if (vsN == -1)	
		vsN = m_main->m_vsN;

	int	key = 0, vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (!GetMapInfo(mapN, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = TABLISTVIEW;

	CView*		view = m_main->GetSDIView(vwKIND);
	CSChild*	schild= (CSChild *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);
	view->SetWindowPos(NULL, 0, 0, LOWORD(size), HIWORD(size), SWP_SHOWWINDOW);
	
	schild->SetTabView(true);
	schild->m_key	= key;
	schild->m_vwTYPE = vtype;
	schild->SetMapName(axiscall, mapN);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	schild->SetSize(nCx, nCy);

	m_main->AddSdi(vsN, key, schild);
	m_main->m_sdiZORDER[vsN].InsertAt(0, key);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		schild->PostMessage(WM_CLOSE);
		m_main->RemoveSdi(vsN, key);
		m_main->m_sdiZORDER[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);
		
	m_main->set_STrigger(key, group, vsN);
	schild->SetMisc(WM_AXIS, m_main, axiscall, fontN, m_main->m_bSDI, vwTYPE == vtypeWND ? false : true);

	if (vsN != m_main->m_vsN)
	{
		schild->SetWindowPos(NULL, sdiRc.left, sdiRc.top, schild->GetSize().cx, schild->GetSize().cy, SWP_HIDEWINDOW);
#ifdef DF_USE_CPLUS17
		((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu.get(), mapN, axiscall);
#else
		((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu, mapN, axiscall);
#endif
	}
	else
	{
		schild->SetWindowPos(NULL, sdiRc.left, sdiRc.top, schild->GetSize().cx, schild->GetSize().cy, SWP_SHOWWINDOW);
#ifdef DF_USE_CPLUS17
		((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu.get(), mapN, axiscall);
#else
		((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu, mapN, axiscall);
#endif
		schild->SetFocus();
		m_main->m_activeMapN = schild->m_mapN;
	}
	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, fontN));

	schild->m_mdiPt = mdiRc.TopLeft();
	schild->m_rectMDI.CopyRect(&mdiRc);
	m_main->addScreenList(schild->m_mapN, schild->m_key, vsN);

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::CreateMDIbySDI(CString mapN, int group, int fontN, CRect sdiRc, CRect mdiRc, int vsN)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateMDIbySDI : mapN=[%s] \n",mapN);
OutputDebugString(s);	
*/
	if (vsN == -1)	
		vsN = m_main->m_vsN;
	if (m_main->IsTabView(mapN))	
		return CreateMDIbySDI_Tabview(mapN, group, fontN, sdiRc, mdiRc, vsN);

	int	key = 0, vtype = 0;
	int	vwTYPE{}, size{}, vwKIND{};

	if (!GetMapInfo(mapN, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? SCROLLVIEW : NORMALVIEW;

	CView*		view = m_main->GetNewView(vwKIND);
	CChildFrame*	child= (CChildFrame *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	BOOL	rc = FALSE;
	CSize	frameGap = m_main->GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;

	if (vwTYPE == vtypeSCR)
	{	
		((CAxScrollView *)view)->SetInfo(key);
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	
		((CAxisView *)view)->SetInfo(key);

	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapN);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	child->SetSize(nCx, nCy);

	m_main->AddMdi(vsN, key, child);
	m_main->m_hooklist[vsN].InsertAt(0, key);

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(vsN, key);
		m_main->m_hooklist[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, fontN));

	if (false && vwTYPE == vtypeFIX)
	{
		nCx = child->GetSize().cx;
		nCy = child->GetSize().cy;
	}
	else
	{
		nCx = sdiRc.Width();
		nCy = sdiRc.Height();
	}
	
	m_main->set_Trigger(key, group, vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, fontN, vwTYPE == vtypeWND ? false : true);
	
	if (vsN != m_main->m_vsN)
		child->SetWindowPos(NULL, mdiRc.left, mdiRc.top, nCx, nCy, SWP_HIDEWINDOW);
	else
	{
		child->SetWindowPos(NULL, mdiRc.left, mdiRc.top, nCx, nCy, SWP_SHOWWINDOW);
		child->SetFocus();
		m_main->m_activeMapN = child->m_mapN;
	}

	child->m_sdiPt = sdiRc.TopLeft();
	child->m_rectSDI.CopyRect(&sdiRc);

	m_main->addScreenList(child->m_mapN, child->m_key, vsN);

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::CreateMDIbySDI_Tabview(CString mapN, int group, int fontN, CRect sdiRc, CRect mdiRc, int vsN)
{
	if (vsN == -1)	
		vsN = m_main->m_vsN;
	int	key = 0, vtype = 0, vwTYPE = 0, size = 0;

	if (!GetMapInfo(mapN, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;

	CView*		view = m_main->GetNewView(TABLISTVIEW);
	CChildFrame*	child= (CChildFrame *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);

	child->SetTabView(true);
	child->m_key	= key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapN);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	child->SetSize(nCx, nCy);

	m_main->AddMdi(vsN, key, child);
	m_main->m_hooklist[vsN].InsertAt(0, key);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(vsN, key);
		m_main->m_hooklist[vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	m_main->set_Trigger(key, group, vsN);
	child->SetMisc(WM_AXIS, m_main, axiscall, fontN, vwTYPE == vtypeWND ? false : true);

	if (vsN != m_main->m_vsN)
	{
		child->SetWindowPos(NULL, mdiRc.left, mdiRc.top, child->GetSize().cx, child->GetSize().cy, SWP_HIDEWINDOW);

#ifdef DF_USE_CPLUS17
		((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu.get(), mapN, axiscall);
#else
		((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu, mapN, axiscall);
#endif
	}
	else
	{
		child->SetWindowPos(NULL, mdiRc.left, mdiRc.top, child->GetSize().cx, child->GetSize().cy, SWP_SHOWWINDOW);
#ifdef DF_USE_CPLUS17
		((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu.get(), mapN, axiscall);
#else
		((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapN), m_main->m_tMenu, mapN, axiscall);
#endif
		child->SetFocus();
	}

	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, fontN));

	child->m_sdiPt = sdiRc.TopLeft();
	child->m_rectSDI.CopyRect(&sdiRc);
	m_main->addScreenList(child->m_mapN, child->m_key, vsN);

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::CopyScreen(CString mapN, int group, int fontsize, CPoint sp)
{
	//test cdd
#ifdef DF_CDDUSE  
	if (m_main->isCDDScreen(mapN))
	{
		ChangeChild(MAPN_CDDEDD);
		return 0;
	}
#endif

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;
	if (m_main->IsTabView(mapN.Left(L_MAPN)))
		return CopyTabScreen(mapN, group, fontsize, sp);

	CString mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{}, key = 0;

	if (!GetMapInfo(mapname, vtype, size))  //vtcode에서 읽어온값 
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	const CMenu*		menu  = m_main->GetMenu();
	CView*		view  = m_main->GetNewView(vwKIND);
	CChildFrame*	child = (CChildFrame *) view->GetParent();
	
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	if (vwTYPE != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;

	if (vwTYPE == vtypeSCR)
	{	// scroll view
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	
		((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;
	
	m_main->AddMdi(m_main->m_vsN, key, child);
	m_main->m_hooklist[m_main->m_vsN].InsertAt(0, key);

	child->m_key	 = key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);

	nCx = LOWORD(size) + frameGap.cx - 1;
	nCy = HIWORD(size) + frameGap.cy - 1;

	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapname, size, false/*true*/);
	
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(m_main->m_vsN, key);
		m_main->m_hooklist[m_main->m_vsN].RemoveAt(0);
		return 0;
	}
	
	CountMap(mapN);

	if(mapN == "IB202200" || mapN == "IB202700")
	{
		const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
		if (base)
		{
			base->SendMessage(WM_MENU2022, 0, (LPARAM)0);
		}
	}

	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFONTx, key), MAKELONG(0, fontsize));

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;
	m_main->set_Trigger(key, group);

	child->SetMisc(WM_AXIS, m_main, axiscall, fontsize, vwTYPE == vtypeWND ? false : true);
	m_main->m_bar1->set_Maps(mapname);
	m_main->addTool_history(mapname);

#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, mapname, m_main->m_tMenu.get());
#else
	m_main->m_bar2->add_Button(key, mapname, m_main->m_tMenu);
#endif
	
	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	/*
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN)))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
	}else{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
		
	}
	*/

// 	CString s;
// 	s.Format("COPYSCREEN : %d	%d	%d	%d\n",sp.x,sp.y,nCx,nCy);
// 	OutputDebugString(s);

	int nX{}, nY{};

	nX = sp.x;
	nY = sp.y;

	if(nX < 0)
		nX = 0;

	if(nY < 0)
		nY = 0;

	child->SetWindowPos(NULL, nX, nY, nCx, nCy, SWP_HIDEWINDOW);
	child->ShowWindow(SW_SHOW);
	m_main->SetForegroundWindow();
	m_main->maxChild();
	m_main->m_activeMapN = child->m_mapN;

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::CopySScreen(CString mapN, int group, int fontsize, CPoint sp)
{
	return CreateSDI(mapN, group, 0, -1, sp);
}

int CMapHelper::CopyTabScreen(CString mapN, int group, int fontsize, CPoint sp)
{
	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;
	CString mapname = mapN.Left(L_MAPN);

	int vtype = 0, size = 0, key = 0;
	const int  vwTYPE = 0;
	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	CView*		view  = m_main->GetNewView(TABLISTVIEW);
	CChildFrame*	child = (CChildFrame *) view->GetParent();

	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	frameGap.cy += XCAP_HEIGHT + TABH;
	((CAxTabView *)view)->SetInfo(key);

	m_main->AddMdi(m_main->m_vsN, key, child);
	m_main->m_hooklist[m_main->m_vsN].InsertAt(0, key);

	child->SetTabView(true);
	child->m_key	 = key;
	child->m_vwTYPE = vtype;
	child->SetMapName(axiscall, mapname);
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
			(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		m_main->RemoveMdi(m_main->m_vsN, key);
		m_main->m_hooklist[m_main->m_vsN].RemoveAt(0);
		return 0;
	}

	CountMap(mapN);

	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;

	m_main->set_Trigger(key, group);
	child->SetMisc(WM_AXIS, m_main, axiscall, m_main->m_fontSize, vwTYPE == vtypeWND ? false : true);

	int nX{}, nY{};

	nX = sp.x;
	nY = sp.y;

	if(nX < 0)
		nX = 0;
	if(nY < 0)
		nY = 0;

	child->SetWindowPos(NULL, nX, nY, nCx, nCy, SWP_HIDEWINDOW);
	child->ShowWindow(SW_SHOW);

#ifdef DF_USE_CPLUS17
	((CAxTabView*)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu.get(), mapname, axiscall);
#else
	((CAxTabView *)view)->JustWindow(Axis::home, m_main->GetTabKey(mapname), m_main->m_tMenu, mapname, axiscall);
#endif

	m_main->add_history(mapname);
	m_main->m_bar1->set_Maps(mapname.Left(L_MAPN));
	m_main->addTool_history(mapname);

#ifdef DF_USE_CPLUS17
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu.get());
#else
	m_main->m_bar2->add_Button(key, mapname.Left(L_MAPN), m_main->m_tMenu);
#endif

	if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	/*
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN)))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
	}else{
		if (!m_main->ExistMenu(mapname.Left(L_MAPN-2)+"00"))
		{
			CString	string = child->m_xcaption.GetTitle();
			m_main->m_bar2->change_ButtonText(key, string);
		}
		
	}
	*/

	m_main->SetForegroundWindow();
	m_main->maxChild();
	m_main->m_activeMapN = child->m_mapN;

	m_main->writeOpenedList();	// 20070627 kwon
	return key;
}

int CMapHelper::ChangeChild(CString mapN, int trigger, int key, int position)
{
	//test cdd
#ifdef DF_CDDUSE  
	if (m_main->isCDDScreen(mapN))
	{
		ChangeChild(MAPN_CDDEDD);
		return 0;
	}
#endif
CString s;

s.Format("[AXIS]_ [map] ChangeChild[CString] : mapN=[%s] trigger=[%d] key=[%d]\n",
												    mapN, trigger, key);
OutputDebugString(s);

//	WriteLog("ChangeChild %s %d %d", (LPCSTR)mapN, trigger, position);
// 	if (m_main->IsForeignMap(mapN))
// 	{
// 		m_main->RunForeignMap();
// 		return TRUE;
// 	}  

	if(m_main->IsNoTwopop(mapN))
		return TRUE;

	s.Format("[axis]ChangeChild %s %d %d", (LPCSTR)mapN, trigger, position);
	OutputDebugString(s);
	if (m_main->IsNewRealTick(mapN))
	{
		m_main->RunNewRealTick();
		return TRUE;
	}
	else if (m_main->IsPhonePad(mapN))
	{
		m_main->RunPhonePad();
		return TRUE;
	}
	if ((mapN.Mid(6,2) == "00") && (!Axis::devMode) && (!m_main->IsSuperUser()))
	{
		if (!m_main->ExistMenu(mapN.Mid(0,8))) return 0;
	}	

	if (m_main->m_MClient->m_pBkMapWnd && key == ((CBkWnd*)m_main->m_MClient->m_pBkMapWnd)->m_key)
		return ChangeBkNotice(mapN, trigger, key, position);

	if (m_main->m_bSDI || mapN == "IB876100")	
		return ChangeSDIChild(mapN, trigger, key, position);

	if (m_main->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_main->m_tMenu->GetURL(mapN.Left(L_MAPN));

		// new added by kwon 20070521
		
		if (url.Find(INSERTID) >= 0)
			url.Replace(INSERTID, Axis::userID);
		
		m_main->RunWebBrowser(url);
		return 1;		// 0:인경우 focus잃어버림
	}

	if (mapN.Left(L_MAPN) == MAPN_GONGJI)
	{
		if (!m_main->m_MClient->m_pBkMapWnd)
			return CreateBkNotice(mapN, trigger);
		else
			return 0;
	}
	
	const int	ret = m_main->ExceptionProcess(mapN);
	if (ret)	
		return ret;

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;

	//m_main->m_screenNew = 0;

	if (m_main->IsTabView(mapN.Left(L_MAPN)))	
		return ChangeTabView(mapN, trigger, key, position);

	CString	mapname = mapN.Left(L_MAPN);
	
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	/*
	CSChild*	schild = NULL;
	for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
	{
		m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, key, schild);
		if (schild && schild->GetSafeHwnd())
		{
			TRACE("%s\n",schild->m_mapN);
			//schild-
			//return TRUE;
		}
	}
	*/

	CChildFrame*	child = NULL;
	if (!m_main->m_arMDI[m_main->m_vsN].Lookup(key, child))
	{

		CSChild*	schild = NULL;
		/*
		if (!m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild))
		{
			return CreateChild(mapN, trigger, key, position);
		}
		*/
		if (m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild))
		{
			return ChangeSDIChild(mapN, trigger, key, position);
		}
		//최소화된 창중에 해당 맵이 있으면 활성화
// 		CString s;
// 		s.Format("CHANGEMAP : %s\n",mapname);
// 		OutputDebugString(s);

		BOOL bOnlyOne = FALSE;

		if( mapname.Find("IB713300") > -1 || mapname.Find("IB713400") > -1 || mapname.Find("IB714200") > -1 || mapname.Find("IB714500") > -1)
		{
			//OutputDebugString("ONLY ONE SCREEN\n");
			bOnlyOne = TRUE;
		}

		//key : 3 은 돌려보기
		if(!m_main->m_screenNew || key == 3 || bOnlyOne)
		{
			//OutputDebugString("NOT NEW SCREEN\n");
			int tkey = key;
			const CRect rc;
			for (POSITION pos = m_main->m_arMDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arMDI[m_main->m_vsN].GetNextAssoc(pos, tkey, child);
				if (child && child->GetSafeHwnd() && child->m_bIconic)
				{
					if(child->m_mapN == mapN)
					{
						child->actionCaption(IDX_RESTORE, CPoint(0, 0));
						
						return tkey;
					}
				}
			}

			for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, tkey, schild);
				if (schild && schild->GetSafeHwnd() && schild->m_bIconic)
				{
					if(schild->m_mapN == mapN)
					{
						
						schild->ActivateFrame();  //SDI모드일땐 SetRestore만으로 활성화가 되지 않아 해당 함수 호출
						schild->SetRestore();
					
						return tkey;
					}
				}
			}
			
			if (key < winK_NORM)
			{
				if(key == 3)
				{
					if (!m_main->Agree_Duplication(mapN,true))
						return 0;
				}
				else if(bOnlyOne)
				{
					if (!m_main->Agree_Duplication(mapN,true))
						return 0;
				}
				else
				{
					if (!m_main->Agree_Duplication(mapN))
						return 0;
				}
			}
		}
		else
		{
			int tkey = key;
			const CRect rc;
			for (POSITION pos = m_main->m_arMDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arMDI[m_main->m_vsN].GetNextAssoc(pos, tkey, child);
				if (child && child->GetSafeHwnd() && child->m_bIconic)
				{
					if(child->m_mapN == mapN)
					{
						int		value{};
						const long		rc = 0;
						const CRect		sdiRc, mdiRc;
						CPoint		sp(-1, -1);
						CString		mapname;
						WINDOWPLACEMENT	pl;
						
						child->GetWindowPlacement(&pl);

						value = GetSystemMetrics(SM_CYCAPTION);
						value += GetSystemMetrics(SM_CXFRAME);
						value += GetSystemMetrics(SM_CXBORDER);
						sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
						sp.Offset(value, value);
						
// 						value = child->m_xcaption.GetGroup();
// 						switch (value)
// 						{
// 						case 0:/*none group*/
// 						case 99:/*all group*/	break;
// 						case 1:case 2: case 3:case 4: case 5:
// 							value++;
// 							if (value > 5)	value = 1;
// 							break;
// 						default:	
// 							value = 1;	break;
// 						}		
						CString strValue = "";

						if(!m_main->m_groupBymapN.Lookup(child->m_mapN,strValue))
						{
							strValue = _T("1");
						}

						if(strValue == "")
						{
							strValue = _T("1");
						}

// 						CString s;
// 						s.Format("RUN MAP NAME : %s	GROUP : %d\n",child->m_mapN,strValue);
// 						OutputDebugString(s);

						value = atoi(strValue);

						if(value == 10)
						{
							value = 1;
						}
						else
						{
							value += 1;
						}

						tkey = CopyScreen(child->m_mapN, value, child->m_xcaption.GetFontSize(), sp);
						
						return tkey;
					}
				}
			}
			
			for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
			{
				m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, tkey, schild);
				if (schild && schild->GetSafeHwnd() && schild->m_bIconic)
				{
					if(schild->m_mapN == mapN)
					{
						
						int		value{};
						const long		rc = 0;
						const CRect		sdiRc, mdiRc;
						CPoint		sp(-1, -1);
						CString		mapname;
						WINDOWPLACEMENT	pl;
						
						child->GetWindowPlacement(&pl);
						value = GetSystemMetrics(SM_CYCAPTION);
						value += GetSystemMetrics(SM_CXFRAME);
						value += GetSystemMetrics(SM_CXBORDER);
						sp = CPoint(pl.rcNormalPosition.left, pl.rcNormalPosition.top);
						sp.Offset(value, value);
						
// 						value = child->m_xcaption.GetGroup();
// 						switch (value)
// 						{
// 						case 0:/*none group*/
// 						case 99:/*all group*/	break;
// 						case 1:case 2: case 3:case 4: case 5:
// 							value++;
// 							if (value > 5)	value = 1;
// 							break;
// 						default:	
// 							value = 1;	break;
// 						}
						CString strValue = "";
						
						if(!m_main->m_groupBymapN.Lookup(child->m_mapN,strValue))
						{
							strValue = _T("1");
						}
						
						if(strValue == "")
						{
							strValue = _T("1");
						}
						
						// 						CString s;
						// 						s.Format("RUN MAP NAME : %s	GROUP : %d\n",child->m_mapN,strValue);
						// 						OutputDebugString(s);
						
						value = atoi(strValue);
						
						if(value == 10)
						{
							value = 1;
						}
						else
						{
							value += 1;
						}

						tkey = CopyScreen(child->m_mapN, value, child->m_xcaption.GetFontSize(), sp);
						
						return tkey;
					}
				}
			}

			if (key < winK_NORM)
			{
				
				if (!m_main->Agree_Duplication(mapN))
					return 0;
			}
		}

		//OutputDebugString("DUPLICATE RUN\n");

		return CreateChild(mapN, trigger, key, position);
	}

	//OutputDebugString("ACROSS\n");

	int vwTYPE = 0, size = 0;
	
	CView* view = child->GetActiveView();

	if (!GetMapInfo(mapname, vwTYPE, size))
		return 0;

	if (child->IsTabView())		
		return ChangeTabView(mapN, trigger, key, position);
	if (vwTYPE != child->m_vwTYPE)	
		return CreateChild(mapN, trigger, 0, position);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();
	//if (!((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000")))
	{
		if ((vwTYPE & vtypeMSK) != vtypeWND)	
			frameGap.cy += XCAP_HEIGHT;
	}

	switch (vwTYPE)
	{
	case 0:
	default:
		if ((vwTYPE & vtypeMSK) == vtypeSCR)
		{
			((CAxScrollView *)view)->SetInfo(key);//m_key = key;
			((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
		}
		else	((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;
		break;
	}

	child->SetMapName(axiscall, mapname.Left(L_MAPN));
	/*
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		nCx = LOWORD(size);
		nCy = HIWORD(size);
		
	}else
	{
	*/
		nCx = LOWORD(size) + frameGap.cx;
		nCy = HIWORD(size) + frameGap.cy;		
	//}

	child->SetSize(nCx, nCy);

	BOOL	rc = FALSE;

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		return 0;
	}

	CountMap(mapN);

	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
		//view->SetWindowPos(NULL,0,0, nCx, nCy, SWP_SHOWWINDOW);
		child->ShowBackGround(true);
		
		::SetWindowPos(child->m_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
	
	if (view)	
		view->Invalidate();
	/*
	if ((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000"))
	{
	
	}
	else
	{
	*/
		nCx = child->GetSize().cx;
		nCy = child->GetSize().cy;
	//}
	if (!((CString(child->m_mapN).Mid(0,6) == "IB7700") && (CString(child->m_mapN).Mid(0,8) != "IB770000")))
		child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_HIDEWINDOW);
	else
		child->SetWindowPos(&m_main->wndBottom, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_HIDEWINDOW);

	
	
	//AfxMessageBox("asdf");
	//child->m_xcaption.SetMax(false);
	if (child->m_xcaption.IsMax())	child->actionCaption(IDX_MAX, CPoint(-1,-1));
	
	
	if (child->m_mapN != "IB820850" && child->m_mapN != "IB0000X8")
		child->ShowWindow(SW_SHOW);
s.Format("[axis]ChangeChild %s child[%x] --------------------------------------------------", (LPCSTR)mapN, child);
OutputDebugString(s);
	m_main->add_history(child->m_mapN);
	
	m_main->m_bar1->set_Maps(child->m_mapN);
	m_main->addTool_history(child->m_mapN);

#ifdef DF_USE_CPLUS17
	m_main->m_bar2->change_Info(key, child->m_mapN, m_main->m_tMenu.get());
#else
	m_main->m_bar2->change_Info(key, child->m_mapN, m_main->m_tMenu);
#endif

	if (!m_main->ExistMenu(child->m_mapN.Left(L_MAPN-2)+"00"))
	{
		CString	string = child->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	m_main->m_activeMapN = child->m_mapN;
	
	return key;
}

int CMapHelper::ChangeSDIChild(CString mapN, int trigger, int key, int position)
{
CString s;

s.Format("[AXIS]_ [map] ChangeSDIChild[CString] : mapN=[%s] trigger=[%d] key=[%d]\n", 
													mapN, trigger, key);
OutputDebugString(s);


	if (m_main->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_main->m_tMenu->GetURL(mapN.Left(L_MAPN));
		m_main->RunWebBrowser(url);
		return 0;
	}

	if(mapN == "IB876100")
	{
		m_main->actionCaption(-1,MIDX_MIN);
	}

	const int	ret = m_main->ExceptionProcess(mapN);
	if (ret)	
		return ret;

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;
	if (m_main->IsTabView(mapN.Left(L_MAPN)))	
		return ChangeSDITabView(mapN, trigger, key, position);

	CString	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;
	
	CSChild*	schild = NULL;
	if (!m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild))
	{
		if (key < winK_NORM)
		{
			if (!m_main->Agree_Duplication(mapN))
				return 0;
		}
		
		return CreateSDI(mapN, trigger, key, position);
	}

	int vwTYPE = 0, size = 0;
	CView* view = schild->GetActiveView();

	if (!GetMapInfo(mapname, vwTYPE, size))
		return 0;
	
	if (vwTYPE != schild->m_vwTYPE)	
		return CreateSDI(mapN, trigger, 0, position);
	if (schild->IsTabView())		
		return ChangeSDITabView(mapN, trigger, key, position);

	int	nCx{}, nCy{};
	CSize	frameGap = m_main->GetFrameGap();

	if ((vwTYPE & vtypeMSK) != vtypeWND)	
		frameGap.cy += XCAP_HEIGHT;

	switch (vwTYPE)
	{
	case 0:
	default:
		if ((vwTYPE & vtypeMSK) == vtypeSCR)
		{
			((CAxScrollView *)view)->SetInfo(key);//m_key = key;
			((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
		}
		else	
			((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;
		break;
	}

	schild->SetMapName(axiscall, mapname.Left(L_MAPN));
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	schild->SetSize(nCx, nCy);

	BOOL	rc = FALSE;

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		schild->PostMessage(WM_CLOSE);
		return 0;
	}

	CountMap(mapN);

	if (view)	
		view->Invalidate();

	if(mapN == "IB876100")
	{
		nCx = 0;
		nCy = -23;
	}
	else
	{
		nCx = schild->GetSize().cx;
		nCy = schild->GetSize().cy+4;
	}
	schild->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_SHOWWINDOW);
	
	schild->ShowWindow(SW_SHOW);

	m_main->add_history(schild->m_mapN);
	m_main->m_bar1->set_Maps(schild->m_mapN);
	m_main->addTool_history(schild->m_mapN);

#ifdef DF_USE_CPLUS17
	m_main->m_bar2->change_Info(key, schild->m_mapN, m_main->m_tMenu.get());
#else
	m_main->m_bar2->change_Info(key, schild->m_mapN, m_main->m_tMenu);
#endif

	if (!m_main->ExistMenu(schild->m_mapN.Left(L_MAPN-2)+"00"))
	{
		CString	string = schild->m_xcaption.GetTitle();
		m_main->m_bar2->change_ButtonText(key, string);
	}
	m_main->m_activeMapN = schild->m_mapN;
	return key;
}


int CMapHelper::ChangeChild(int index)
{
CString s;

s.Format("[AXIS]_ [map] ChangeChild[int] : mapN=[%d]\n", index);
OutputDebugString(s);

	CString	mapN, args, title;
	if (index < ID_MENU_TB)
	{
		index -= ID_MENU_BASE;
		if (index >= 0 && index < m_main->m_tMenu->GetMCount())
		{
			mapN = m_main->m_tMenu->GetMap(index);
			if (mapN.Find(WEB) == 0)
			{
				char	wb[1024]{};
				CString url = m_main->m_tMenu->GetInfo(mapN);
				if (url.IsEmpty())	
					return 0;

				// new added by kwon 20070521
				if (url.Find(INSERTID) >= 0)
					url.Replace(INSERTID, Axis::userID);

				sprintf(wb, "%s", url);
				m_main->RunWebBrowser(wb);
				return 0;
			}
// 			else if (m_main->IsForeignMap(mapN))
// 			{
// 				m_main->RunForeignMap();
// 				return 0;
// 			}
			else if (m_main->IsNewRealTick(mapN))
			{
				m_main->RunNewRealTick();
				return 0;
			}
			else if (m_main->IsPhonePad(mapN))
			{
				m_main->RunPhonePad();
				return 0;
			}
		}
	}
	else if (index < ID_MENU_USERTOOL)
	{
		index -= ID_MENU_TB;
		title.LoadString(ST_TEXT_OK);
		//**::MessageBox(m_main->m_hWnd, "ID_MENU_TB", title, MB_OK);
		Axis::MessageBox(m_main, "ID_MENU_TB");
		return 0;
	}
	else if (index < ID_MENU_USER)		// ID_MENU_TB ? 
	{
		index -= ID_MENU_USERTOOL;
		m_main->runProgram(index);
		return 0;
	}
	else if (index < ID_MENU_END)
	{
		index -= ID_MENU_USER;
		m_main->createUserScreen(index);
		return 0;
	}
	else
	{
		title.LoadString(ST_TEXT_OK);
		//**::MessageBox(m_main->m_hWnd, "Error select", title, MB_OK);
		Axis::MessageBox(m_main, "Error select");
		return 0;
	}
// 	OutputDebugString("=========================================================\n");
// 	OutputDebugString("[CHANGECHILD INDEX]\n");
// 	CString s;
// 	s.Format("[MAPNAME : %s CALL INDEX : %d]\n",mapN,index);
// 	OutputDebugString(s);
// 	OutputDebugString("=========================================================\n");
	if (mapN.GetLength() < L_MAPN)
		return 0;
	return ChangeChild(mapN.Left(L_MAPN));
}

#pragma warning (disable : 26400)
#pragma warning (disable : 26409)
int CMapHelper::CreatePopup(CString mapN, int trigger, int key, int position, int actkey, CPoint point, BOOL closex, BOOL NoCaption)
{
	CString s;
	/*
	s.Format("[AXIS]_ [map] CreatePopup : mapN=[%s] trigger=[%d] key=[%d] position=[%d] x=[%d] y=[%d] [%d][%d][%d]\n",
												mapN, trigger, key,     position,  point.x, point.y, actkey, closex, NoCaption);
	OutputDebugString(s);
	*/
	if (!IsValidMap(mapN.Left(L_MAPN)))
		return 0;
	CString mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())
		mapname = m_main->m_mapN;

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (mapN.Left(8) == "IBNEWSXX")
	{
		closex = false;
		CGPop* p = m_main->getGPOP(mapN);
		if (p)
		{
			//AfxMessageBox(mapN);
			BOOL	rc = FALSE;
			m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void*)&rc,
				(BYTE*)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), p->m_key, mapN, size, false);

			m_main->SetFocus();

			CRect trc;
			p->GetWindowRect(&trc);
			trc.InflateRect(-1, 0);
			p->MoveWindow(&trc);
			trc.DeflateRect(1, 0);
			p->MoveWindow(&trc);

			//AfxMessageBox("dd");

			return p->m_key;
		}
	}

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	CString strWndClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
		::LoadCursor(NULL, IDC_ARROW),
		(HBRUSH) ::GetStockObject(WHITE_BRUSH),
		m_main->m_resourceHelper->GetIcon());

	CGPop* pop = new CGPop(m_main);
	if (NoCaption)
	{
		//AfxMessageBox("a");
		if (!pop->Create(strWndClass, AXISKEY, /*WS_OVERLAPPEDWINDOW*/ WS_POPUP | WS_VISIBLE | WS_THICKFRAME, CRect(0, 0, 0, 0), NULL, NULL, 0, NULL))
		{
			CString	title;
			title.LoadString(ST_TEXT_OK);
			//**::MessageBox(m_main->m_hWnd, "Fail create popup windows. Error code = [04]", title, MB_ICONWARNING);
			Axis::MessageBox(m_main, "Fail create popup windows. Error code = [04]", MB_ICONWARNING);
			delete pop;
			return 0;
		}
	}
	else
	{
		//AfxMessageBox("b");
		if (!pop->Create(strWndClass, AXISKEY, WS_OVERLAPPEDWINDOW, CRect(0, 0, 0, 0), NULL, NULL, 0, NULL))
		{
			CString	title;
			title.LoadString(ST_TEXT_OK);
			//**::MessageBox(m_main->m_hWnd, "Fail create popup windows. Error code = [04]", title, MB_ICONWARNING);
			Axis::MessageBox(m_main, "Fail create popup windows. Error code = [04]", MB_ICONWARNING);
			delete pop;
			return 0;
		}
	}
	CRuntimeClass* pNewViewClass{};
	if (vwTYPE == vtypeSCR)	
		pNewViewClass = RUNTIME_CLASS(CAxScrollView);
	else			
		pNewViewClass = RUNTIME_CLASS(CAxisView);

	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = m_main->GetActiveDocument();

	CView* view = pop->AttachView(context);
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)view, vtype, key);
	
	if (vwTYPE == vtypeSCR)	
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
	else			
		((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;

	pop->m_key  = key;
	pop->m_vwTYPE = vtype;
	pop->m_mapN   = mapname;

	const CRect	rect;
	int	nCx{}, nCy{};

	if (vwTYPE == vtypeSCR)
	{	// scroll view
		((CAxScrollView *)view)->SetInfo(key);//m_key = key;
		((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
	}
	else	
		((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;

	m_main->m_arGPOP.SetAt(key, pop);

	nCx = LOWORD(size);
	nCy = HIWORD(size);

	pop->SetSize(nCx, nCy);//, FALSE);

	BOOL	rc = FALSE;

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		pop->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		m_main->m_arGPOP.RemoveKey(key);
		return 0;
	}
	
	CountMap(mapN);

	int	ret;
	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, trigger));

	pop->m_key    = key;
	pop->m_vwTYPE = vtype;

	nCx = pop->GetSize().cx;
	nCy = pop->GetSize().cy;
	
	if (mapN.Find("IBNEWSXX")==-1)  //뉴스 화면이라면 초기에 안보이게 하고 화면자체에서 위치 지정후 뜨게끔 처리
		pop->SetWindowPos(&m_main->wndTopMost, 0, 10000, nCx, nCy, SWP_SHOWWINDOW);
	else
		pop->SetWindowPos(&m_main->wndTopMost, 0, 10000, nCx, nCy, SWP_HIDEWINDOW);
	if (position == -1)
		pop->SetWindowPos(&m_main->wndTopMost, point.x, point.y, nCx, nCy, SWP_SHOWWINDOW);
	else	
		m_main->positionWindow(actkey, pop, position, false);

	if (mapN.Find("IB877700") > -1)
	{
		CMenu* menu = pop->GetSystemMenu(0);
		menu->EnableMenuItem( SC_CLOSE, MF_DISABLED );
		pop->ModifyStyle( WS_MAXIMIZEBOX, 0 );
	}

	pop->m_closex = closex != 0;
	//pop->m_closex = true;
	m_main->m_activeMapN = pop->m_mapN;
	return key;
}

int CMapHelper::CreateBkNotice(CString mapN, int trigger, int key, int position, CPoint point)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateBkNotice : mapN=[%s] trigger=[%d] key=[%d] \n",
												mapN, trigger, key);
OutputDebugString(s);
*/


	if (m_main->m_bSDI)	
		return CreateSDI(mapN, trigger, key, position, point);
	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;
	if (m_main->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_main->m_tMenu->GetURL(mapN.Left(L_MAPN));
		m_main->RunWebBrowser(url);
		return 0;
	}

	if (m_main->IsTabView(mapN.Left(L_MAPN)))
		return ChangeTabView(mapN, trigger, key, position, point);

	CString mapname, savemapN = mapN;
 	mapname = mapN;
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (!GetMapInfo(mapname, vtype, size))
		return 0;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	LPCTSTR className = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH) ::GetStockObject(WHITE_BRUSH),	NULL);

#ifdef DF_USE_CPLUS17
	std::unique_ptr<CBkWnd> bkWnd = std::make_unique<CBkWnd>();
#else
	CBkWnd* bkWnd = new CBkWnd();
#endif
	
	if (!bkWnd->Create(className, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, CRect(0, 0, 0, 0), m_main->m_MClient, 0))
	{
#ifndef DF_USE_CPLUS17
		delete bkWnd;
#endif
		return 0;
	}

#ifdef DF_USE_CPLUS17
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void *)&key,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), (long)bkWnd.get(), vtype, key);
#else
	m_main->m_wizard->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void*)&key,
		(BYTE*)(VTS_I4 VTS_I4 VTS_I4), (long)bkWnd, vtype, key);
#endif
	
	bkWnd->m_key  = key;
	bkWnd->m_vwTYPE = vtype;
	bkWnd->m_mapN   = mapname;
	bkWnd->SetSize(LOWORD(size), HIWORD(size));

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false);
	if (!rc)
	{
		bkWnd->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		return 0;
	}

	CountMap(mapN);
	
	int	ret;
	m_main->m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
				(BYTE *)(VTS_I4 VTS_I4), setGID, (long)MAKELONG(key, trigger));

	bkWnd->SetWindowPos(&m_main->wndBottom, 0, 0, bkWnd->GetSize().cx, bkWnd->GetSize().cy, SWP_HIDEWINDOW);
	if (position == -1)
		bkWnd->SetWindowPos(&m_main->wndBottom, point.x, point.y, bkWnd->GetSize().cx, bkWnd->GetSize().cy, SWP_HIDEWINDOW);
	else
#ifdef DF_USE_CPLUS17
		m_main->positionWindow(key, bkWnd.get(), position, false);
#else
		m_main->positionWindow(key, bkWnd, position, false);
#endif

	bkWnd->SetWindowPos(&m_main->wndNoTopMost, 0, 0, bkWnd->GetSize().cx, bkWnd->GetSize().cy, SWP_HIDEWINDOW|SWP_NOACTIVATE);
	bkWnd->ShowWindow(SW_SHOWNA);
	bkWnd->ModifyStyle(WS_TABSTOP, NULL, SWP_NOACTIVATE|SWP_SHOWWINDOW);
#ifdef DF_USE_CPLUS17
	m_main->m_MClient->m_pBkMapWnd = (CWnd*)bkWnd.get();
#else
	m_main->m_MClient->m_pBkMapWnd = (CWnd*)bkWnd;
#endif
	CRect clientRc;
	m_main->m_MClient->GetWindowRect(&clientRc);
	m_main->m_MClient->m_nBkMode = m_main->m_nBkMode;
	m_main->m_MClient->SendMessage(WM_SIZE, 0, MAKELPARAM(clientRc.Width(), clientRc.Height()));

	return key;
}

int CMapHelper::ChangeBkNotice(CString mapN, int trigger, int key, int position)
{
CString s;
/*
s.Format("[AXIS]_ [map] ChangeBkNotice[CString] : mapN=[%s] trigger=[%d] key=[%d]\n", 
															mapN, trigger, key);
OutputDebugString(s);
*/

	if (m_main->m_nBkMode == BKMODE_NORMAL)
		m_main->m_nBkMode = BKMODE_CHANGE;
	else
		m_main->m_nBkMode = BKMODE_NORMAL;

	if (m_main->m_bSDI)	
		return ChangeSDIChild(mapN, trigger, key, position);

	if (!m_main->m_MClient->m_pBkMapWnd) 
		return 0;

	if (m_main->m_tMenu->IsWeb(mapN.Left(L_MAPN)))
	{
		CString url = m_main->m_tMenu->GetURL(mapN.Left(L_MAPN));

		// new added by kwon 20070521
		if (url.Find(INSERTID) >= 0)
			url.Replace(INSERTID, Axis::userID);

		m_main->RunWebBrowser(url);
		return 1;		// 0:인경우 focus잃어버림
	}

	const int	ret = m_main->ExceptionProcess(mapN);
	if (ret)	
		return ret;

	if (!IsValidMap(mapN.Left(L_MAPN)))	
		return 0;

	if (m_main->IsTabView(mapN.Left(L_MAPN)))	
		return ChangeTabView(mapN, trigger, key, position);

	CString	mapname = mapN.Left(L_MAPN);

	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	CBkWnd*	child = (CBkWnd*)m_main->m_MClient->m_pBkMapWnd;

	int vwTYPE = 0, size = 0;

	if (!GetMapInfo(mapname, vwTYPE, size))
		return 0;

	if (vwTYPE != child->m_vwTYPE)	
		return CreateChild(mapN, trigger, 0, position);

	m_main->m_MClient->m_pBkMapWnd->MoveWindow(CRect(0, 0, 0, 0));

	int	nCx{}, nCy{};
	const CSize	frameGap = CSize(0, 0);//m_main->GetFrameGap();

	child->SetMapName(axiscall, mapname.Left(L_MAPN));
	nCx = LOWORD(size) + frameGap.cx;
	nCy = HIWORD(size) + frameGap.cy;

	child->SetSize(nCx, nCy);
	
	BOOL	rc = FALSE;

	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		child->PostMessage(WM_CLOSE);
		return 0;
	}

	CountMap(mapN);
	
	nCx = child->GetSize().cx;
	nCy = child->GetSize().cy;
	child->SetWindowPos(NULL, 0, 0, nCx, nCy, SWP_NOMOVE|SWP_HIDEWINDOW);

	CRect clientRc;
	m_main->m_MClient->GetWindowRect(&clientRc);
	m_main->m_MClient->m_nBkMode = m_main->m_nBkMode;
	m_main->m_MClient->SendMessage(WM_SIZE, 0, MAKELPARAM(clientRc.Width(), clientRc.Height()));
	
	child->ShowWindow(SW_SHOW);

	return key;
}

int CMapHelper::ChangePopup(CString mapN, int trigger, int key, int position, int actkey)
{
CString s;
/*
s.Format("[AXIS]_ [map] ChangePopup : mapN=[%s] trigger=[%d] key=[%d] position=[%d] actkey=[%d]\n",
										mapN, trigger, key, position,  actkey);
OutputDebugString(s);
*/

	CString	mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	CGPop*	pop = NULL;
	if (!m_main->m_arGPOP.Lookup(key, pop))	
		return CreatePopup(mapN, trigger, key, position, actkey);

	int vwTYPE = 0, size = 0;

	if (!GetMapInfo(mapname, vwTYPE, size))
		return 0;

	if (vwTYPE != pop->m_vwTYPE)	
		return CreatePopup(mapN, trigger, key, position, actkey);

	const CRect	rect;
	int	nCx{}, nCy{}, gapX{}, gapY{};

	gapX  = GS::GetSystemMetrics(SM_CXFRAME) * 2;
	gapX += GS::GetSystemMetrics(SM_CXBORDER) * 4;

	gapY  = GS::GetSystemMetrics(SM_CYFRAME) * 2;
	gapY += GS::GetSystemMetrics(SM_CYBORDER) * 4;
	if ((vwTYPE & vtypeMSK) == vtypeWND)
		pop->ModifyStyle(WS_CAPTION, 0, SWP_DRAWFRAME);
	else	
		gapY += GS::GetSystemMetrics(SM_CYCAPTION);

	CView* view = pop->GetActiveView();

	switch (vwTYPE)
	{
	case 0:
	default:
		if ((vwTYPE & vtypeMSK) == vtypeSCR)
		{
			((CAxScrollView *)view)->SetInfo(key);//m_key = key;
			((CAxScrollView *)view)->SetSizeView(LOWORD(size), HIWORD(size));
		}
		else	
			((CAxisView *)view)->SetInfo(key);//((CAxisView *)view)->m_key = key;
		break;
	}

	nCx = LOWORD(size) + gapX;
	nCy = HIWORD(size) + gapY;
	pop->SetSize(nCx, nCy);

	BOOL	rc = FALSE;
	m_main->m_wizard->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), key, mapN, size, false/*true*/);
	if (!rc)
	{
		pop->PostMessage(WM_CLOSE, (WPARAM) 0, (LPARAM) 0);
		return 0;
	}

	CountMap(mapN);

	nCx = pop->GetSize().cx;
	nCy = pop->GetSize().cy;
	pop->SetWindowPos(&m_main->wndTopMost, 0, 0, nCx, nCy, SWP_NOMOVE);
	m_main->m_activeMapN = pop->m_mapN;
	return key;
}

int CMapHelper::CreateModal(CString mapN, int trigger, int key, int position, int actkey, CPoint point)
{
CString s;
/*
s.Format("[AXIS]_ [map] CreateModal : mapN=[%s] trigger=[%d] key=[%d] \n",
											mapN, trigger, key);
OutputDebugString(s);
*/

	if (m_main->m_MMapN.IsEmpty() && !IsValidMap(mapN.Left(L_MAPN)))	
	{
		AfxMessageBox(Format("createmodal...%s", mapN));
		return 0;
	}

	CString mapname = mapN.Left(L_MAPN);
	if (mapname.IsEmpty())	
		mapname = m_main->m_mapN;

	int vtype = 0 , size = 0;
	const int vwTYPE = 0;
	if (!GetMapInfo(mapname, vtype, size))
	{
		AfxMessageBox("GetMapInfo Falied");
		return 0;
	}
	
	CWnd*	parent = m_main;
	if (actkey & winK_POPUP)
	{
		m_main->m_arMPOP.Lookup(actkey, (CMPop *&) parent);
		m_main->m_arGPOP.Lookup(actkey, (CGPop *&) parent);
	}
	else
	{
		if (!m_main->m_arMDI[m_main->m_vsN].Lookup(actkey, (CChildFrame *&) parent))
			m_main->m_arSDI[m_main->m_vsN].Lookup(actkey, (CSChild *&) parent);
	}

	CMPop*	pop = new CMPop(parent);

	const int	cyBorder  = GS::GetSystemMetrics(SM_CYBORDER);
	const int	cxBorder  = GS::GetSystemMetrics(SM_CXBORDER);
	const int	cxEdge    = GS::GetSystemMetrics(SM_CXEDGE);
	const int	cyEdge    = GS::GetSystemMetrics(SM_CYEDGE);
	const int	cyCaption = GS::GetSystemMetrics(SM_CYSMCAPTION);

	CRect windowRC(0, 0, 0, 0);
	windowRC.top   -= (HIWORD(size) + cyCaption + (cyBorder*2) + (cyEdge*4));
	windowRC.right += (LOWORD(size) + (cxBorder*2) + (cxEdge*4)); 

	CRect	clientRC ;
	const CRect rcWA;
	int	xPosition = 0, yPosition = 0;
	::GetWindowRect(m_main->m_hWndMDIClient, &clientRC);
	windowRC.SetRect(0, 0, LOWORD(size), HIWORD(size));

	switch (position)
	{
	case LtopPOS:		// left_top
		xPosition = clientRC.left;
		yPosition = clientRC.top;
		break;

	case RtopPOS:		// right_top
		xPosition = clientRC.right - windowRC.Width();
		yPosition = clientRC.top;
		break;

	case LbottomPOS:	// left_bottom
		xPosition = clientRC.left;
		yPosition = clientRC.bottom - windowRC.Height();
		break;

	case RbottomPOS:	// right_bottom
		xPosition = clientRC.right - windowRC.Width();
		yPosition = clientRC.bottom - windowRC.Height();
		break;
	default:		// center
		xPosition = clientRC.CenterPoint().x - windowRC.Width()/2;
		yPosition = clientRC.CenterPoint().y - windowRC.Height()/2;
		break;		
	}

	pop->m_mapN   = mapN;
	pop->m_vwKEY  = key;
	pop->m_vwTYPE = vtype;
	pop->m_wpos   = position;

	if (position == -1)
		pop->SetPos(point.x, point.y);
	else	
		pop->SetPos(xPosition, yPosition);
	const int w = LOWORD(size);
	const int h = HIWORD(size);
	pop->SetSize(LOWORD(size), HIWORD(size));
	m_main->m_modal = pop;
	m_main->m_activeMapN = pop->m_mapN;
	if (IsDLL(mapN)) pop->m_bDll = true;

	if (pop->DoModal() == IDOK)
	{
		if (!m_main->m_MMapN.IsEmpty())
		{
			m_main->m_bExit = false;
			m_main->PostMessage(WM_CLOSE);
		}
	}
	m_main->m_wizard->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY,
				(void *)NULL, (BYTE *)(VTS_I4), (short) pop->m_vwKEY);

	m_main->m_arMPOP.RemoveKey(pop->m_vwKEY);
	m_main->removeWait(pop->m_vwKEY);
	delete pop;
	m_main->m_modal = NULL;

	return key;
}

#pragma warning (default : 26400)
#pragma warning (default : 26409)

void CMapHelper::DirectClose()
{
	BOOL	rc;
	m_main->m_wizard->InvokeHelper(DI_RUN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
				(BYTE *)(VTS_I4 VTS_I4 VTS_I4), loginAXISx, 0, -1);
	((CAxisApp*)m_main->m_axis)->protectKey(Axis::home, false);
}

BOOL CMapHelper::closeChild()
{
	if (closePopup())
	{
		m_main->SetFocus();
		return TRUE;
	}

	CChildFrame*	child  = NULL;
	CSChild* schild = nullptr;
	int		key{};

	for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
	{
		m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, key, schild);
		if (schild && schild->GetSafeHwnd() && schild->m_bActive)
		{
			if(schild->m_mapN.Find("IB202200") > -1)
			{
				const CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
				base->SendMessage(WM_CLOSE2022,0,0);
			}

			schild->PostMessage(WM_CLOSE);
			return TRUE;
		}
	}

	if (m_main->m_arMDI[m_main->m_vsN].GetCount() <= 0)
	{
		for (POSITION pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
		{ 
			m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, key, schild);
			if (schild->m_bActive)
			{
				if(schild->m_mapN.Find("IB202200") > -1)
				{
					const CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
					base->SendMessage(WM_CLOSE2022,0,0);
				}

				schild->PostMessage(WM_CLOSE);
				return TRUE;
			}
		}
		return FALSE;
	}

	child = (CChildFrame*) m_main->MDIGetActive();
	if (!child)	
		return FALSE;
	if (!child->m_bBackGround)
	{
		if(child->m_mapN.Find("IB202200") > -1)
		{
			const CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
			base->SendMessage(WM_CLOSE2022,0,0);
		}
		child->SendMessage(WM_CLOSE, 0, 0);
	}
	return TRUE;
}

BOOL CMapHelper::closeChild(int key)
{
	if (m_main->m_MClient->m_pBkMapWnd && key == ((CBkWnd*)m_main->m_MClient->m_pBkMapWnd)->m_key)
	{
		m_main->m_MClient->m_pBkMapWnd->PostMessage(WM_CLOSE, 0, 0);
		m_main->m_MClient->m_pBkMapWnd = NULL;
		return TRUE;
	}

	CChildFrame* pChild;

	if (!m_main->m_arMDI[m_main->m_vsN].Lookup(key, pChild))
	{
		CSChild*	schild = NULL;
		m_main->m_arSDI[m_main->m_vsN].Lookup(key, schild);
		if (schild)	
		{
			if(schild->m_mapN.Find("IB202200") > -1)
			{
				const CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
				base->SendMessage(WM_CLOSE2022,0,0);
			}
			schild->PostMessage(WM_CLOSE);
		}
		return FALSE;
	}
	
//	if(pChild->m_mapN == "IB0000X8")
//	{
//		pChild->ShowWindow(SW_HIDE);
//		return TRUE;
//	}

	if(pChild->m_mapN.Find("IB202200") > -1)
	{
		const CWnd* base = pChild->GetActiveView()->GetWindow(GW_CHILD);
		base->SendMessage(WM_CLOSE2022,0,0);
	}

	pChild->PostMessage(WM_CLOSE, 0, 0);	

	return TRUE;
}

BOOL CMapHelper::closePopup()
{
	if (m_main->m_arGPOP.IsEmpty())
		return FALSE;

	CGPop* pop{};
	int	key{};

	for (POSITION pos = m_main->m_arGPOP.GetStartPosition(); pos; )
	{
		m_main->m_arGPOP.GetNextAssoc(pos, key, pop);
		if (!pop->m_active)	
			continue;
		//8777 ESC 키 종료 막음 - 2016.01.19
		//닫기 버튼으로만 닫을수 있어야 한다.
		if(pop->m_mapN == "IB877700")
			return TRUE;

		pop->SendMessage(WM_CLOSE, 0, 0);
		return TRUE;
	}
	
	return FALSE;
}

BOOL CMapHelper::closePopup(int key)
{
	CGPop*	pop;
	if (!m_main->m_arGPOP.Lookup(key, pop))		
		return FALSE;
	pop->SendMessage(WM_CLOSE, 0, 0);	
	return TRUE;
}

BOOL CMapHelper::closeX()
{
	if (m_main->m_arMPOP.GetCount() > 0)
	{
		CString	title;
		title.LoadString(ST_TEXT_OK);
		//**::MessageBox(m_main->m_hWnd, "Program exit after close modal windows.",
		//**		title, MB_ICONINFORMATION);
		Axis::MessageBox(m_main, "Program exit after close modal windows.", MB_ICONINFORMATION);
		return FALSE;
	}

	int	key{};
	CGPop* pop{};
	for (POSITION pos = m_main->m_arGPOP.GetStartPosition(); pos; )
	{
		m_main->m_arGPOP.GetNextAssoc(pos, key, pop);
		if (pop->GetSafeHwnd())
			pop->SendMessage(WM_CLOSE, 0, 0);
	}

	return TRUE;
}

BOOL CMapHelper::closeAll(BOOL all)
{
	int		key{};
	POSITION	pos{};
	CChildFrame* child{};
	CSChild* schild{};

	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		if (!all)
		{
		if (/*!m_main->m_saveALLVS &&*/ vsN != m_main->m_vsN)
			continue;
		}
		//2015.02.02 dkkim
		//2000번 화면과 7710이 같이 있을 경우 2000번보다 7710번이 늦게 닫힐 경우 
		//7710에서 오류발생
		//원인 파악이 어려워 7710부터 닫게 변경
		for (pos = m_main->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			
			if(child->m_mapN == "IB771000")
			{
				child->SendMessage(WM_CLOSE, 0, 0);
			}
		}

		for (pos = m_main->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arMDI[vsN].GetNextAssoc(pos, key, child);

			if(child->m_mapN != "IB0000X8")
			{
				child->SendMessage(WM_CLOSE, 0, 0);
			}
		}

		for (pos = m_main->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			schild->SendMessage(WM_CLOSE, 0, 0);
		}
	}

	if (!all)	
		return TRUE;
	if (m_main->m_arGPOP.IsEmpty())	
		return TRUE;

	CGPop* pop{};
	for (pos = m_main->m_arGPOP.GetStartPosition(); pos; )
	{
		m_main->m_arGPOP.GetNextAssoc(pos, key, pop);
		pop->SendMessage(WM_CLOSE, 0, 0);
	}

	return TRUE;
}

BOOL CMapHelper::CloseInterest()
{
	int		key{};
	POSITION	pos{};
	CChildFrame* child{};
	CSChild* schild{};
	BOOL result = FALSE;
	
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		//2015.02.02 dkkim
		//2000번 화면과 7710이 같이 있을 경우 2000번보다 7710번이 늦게 닫힐 경우 
		//7710에서 오류발생
		//원인 파악이 어려워 7710부터 닫게 변경
		for (pos = m_main->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			
			if(child->m_mapN == "IB202200")
			{
 				CWnd* base = child->GetActiveView()->GetWindow(GW_CHILD);
// 				base->SendMessage(WM_CLOSE2022,0,0);
				const int nResult = base->SendMessage(WM_INTISSAVE,0,0);
				
				if(nResult)
				{
					m_arInterWnd.Add(base);

					m_main->m_nInterest++;
					result = TRUE;
				}
			}
		}

		for (pos = m_main->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arSDI[vsN].GetNextAssoc(pos, key, schild);

			if(schild->m_mapN == "IB202200")
			{
 				CWnd* base = schild->GetActiveView()->GetWindow(GW_CHILD);
// 				base->SendMessage(WM_CLOSE2022,0,0);
				const int nResult = base->SendMessage(WM_INTISSAVE,0,0);
				
				if(nResult)
				{
					m_arInterWnd.Add(base);
					
					m_main->m_nInterest++;
					result = TRUE;
				}
			}
		}
	}

	CString s;
	s.Format("CLOSEINTEREST [%d]\n",m_main->m_nInterest);
	OutputDebugString(s);

	return result;
}

void CMapHelper::SendInterSignal()
{
	for(int i=0;i<m_arInterWnd.GetSize();i++)
	{
		const CWnd* base = m_arInterWnd.GetAt(i);
		
		base->SendMessage(WM_CLOSE2022,0,0);
		OutputDebugString("SEND INTERSIGNAL\n");
	}
}

BOOL CMapHelper::closeAllWithoutThis()
{
	int key{};
	POSITION pos{};
	CChildFrame* child{};
	CSChild* schild{};

	for (pos = m_main->m_arMDI[m_main->m_vsN].GetStartPosition(); pos; )
	{
		m_main->m_arMDI[m_main->m_vsN].GetNextAssoc(pos, key, child);
		if (!child->m_bActive)
			child->SendMessage(WM_CLOSE);
	}

	for (pos = m_main->m_arSDI[m_main->m_vsN].GetStartPosition(); pos; )
	{
		m_main->m_arSDI[m_main->m_vsN].GetNextAssoc(pos, key, schild);
		if (!schild->m_bActive)
			schild->SendMessage(WM_CLOSE);
	}

	return TRUE;
}

BOOL CMapHelper::GetMapInfo(const char* mapName, int& type, int& size)
{
	CString map(mapName);
	if (map.GetLength() < L_MAPN)
		return 0;
	map = map.Left(L_MAPN);

	m_main->m_wizard->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void *)&type,
				(BYTE *)(VTS_BSTR VTS_I4), map, &size);

	if (type <= vtypeERR)	
	{
		//return FALSE;
		CProfile profile(pkVTCode);

		CString str(profile.GetSectionLF("DLL Controls"));

		int find = str.Find(mapName);
		if (find < 0)
			return FALSE;

		const int findLF = str.Find('\n', find);
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
		size = MAKELONG(cx, cy);

		ts.MakeLower();

		if (ts == "fixed")
			type = vtypeFIX;
		else if (ts == "fixedex")
			type = vtypeFEX;
		else if (ts == "normal")
			type = vtypeNRM;
		else if (ts == "resizable")
			type = vtypeRSZ;
		else if (ts == "resizableex")
			type = vtypeREX;
		else
			return FALSE;

		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CMapHelper::IsValidMap(const char* mapName)
{
	//if (Axis::devMode) 
		return TRUE;

	CString map(mapName);

// 	for (int i = 0; exceptMaps[i]; i++)
// 	{
// 		if (!map.CompareNoCase(exceptMaps[i]))
// 			return TRUE;
// 	}
	return TRUE;

	if (map.GetLength() >= L_MAPN && map.Mid(6, 2) == "00")
		return m_main->ExistMenu(map.Left(L_MAPN));
	else
		return FALSE;
}

BOOL CMapHelper::IsDLL(const char* mapName)
{
	CProfile p1(pkVTCode), 
		p2(pkAxis);
	CString map(mapName);
	
	if (map.GetLength() >= L_MAPN)
	{
		CString m(map.Mid(2, 4));
		CString buffer = p2.GetString(szSelect, m);
		if (!buffer.IsEmpty())
		{
			Trim(buffer);
			map = map.Left(2) + buffer;
		}
	}
	
	return !p1.GetString("DLL Controls", map).IsEmpty();
}

BOOL CMapHelper::IsDRFN(const char* mapName)
{
	CProfile p1(pkVTCode);

	CString map(mapName);
	
	return !p1.GetString("DRFN CHART", map).IsEmpty();
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void CMapHelper::TransformMDIChildToSingleWindow(int key, int vs)
{
	CChildFrame* child;

	if (!m_main->m_arMDI[vs].Lookup(key, child))
		return;

	CString mapName;
	int group{}, font{};

	mapName = child->m_mapN;
	group = child->m_xcaption.GetGroup();
	font = child->m_xcaption.GetFontSize();
	
	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (!GetMapInfo(mapName, vtype, size))
		return ;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;

	if (m_main->IsTabView(mapName))
		vwKIND = TABLISTVIEW;

	CView*		oldView = child->GetActiveView();
	CView*		newView = m_main->GetSDIView(vwKIND);
	CSChild*	schild= (CSChild *) newView->GetParent();
	
	CRect rc;
	
	child->GetWindowRect(&rc);
	
	if (child->m_bBackGround)
	{
		rc.right += GetSystemMetrics(SM_CXFRAME)*2;
		rc.bottom += (CAPTION_HEIGHT+GetSystemMetrics(SM_CYFRAME));	
		
	}
	//CString tmp; tmp.Format("%s] Left: %d, Top: %d    ====  %d",mapName, rc.left, rc.top,0);
	//TRACE(tmp+"\n");
	//아이콘화 되어있으면 이전 사이즈로 되돌림
	if (child->m_bIconic)
	{
		CRect mrc, src;
		m_main->GetWindowRect(mrc);
		schild->GetWindowRect(src);
		rc = child->m_restoreRc;
		m_main->m_MClient->ClientToScreen(&rc);

	}
#ifdef DF_MK_CAPTION
	schild->m_xcaption.m_marketN = child->m_xcaption.m_marketN;
#endif
	oldView->SetParent(schild);

	CString title = child->m_xcaption.GetTitle();
	newView->SetParent(child);
	child->SetActiveView(newView);


	m_main->RemoveMdi(vs, key);
	m_main->AddSdi(vs, key, schild);

	m_main->m_arSDI[vs].SetAt(key, schild);
	m_main->m_sdiZORDER[vs].InsertAt(0, key);
	schild->m_mapN = child->m_mapN;
	schild->m_xcaption.SetGroup(child->m_xcaption.GetGroup());
	schild->m_xcaption.SetFontSize(child->m_xcaption.GetFontSize());

	schild->m_key = key;
	schild->m_vwTYPE = vtype;

	schild->SetSize(rc.Width(), rc.Height(), FALSE);
	schild->m_xcaption.SetTitle(title);

	schild->SetMisc(WM_AXIS, m_main, axiscall, font, m_main->m_bSDI, vwTYPE == vtypeWND ? false: true);
	schild->SetActiveView(oldView);
	CRect restoreRC;
	restoreRC = schild->m_rectMDI;
	

	schild->MoveWindow(rc.left + 10, rc.top + 10, rc.Width(), rc.Height());
	
	m_main->m_MClient->ScreenToClient(&rc);
	schild->m_mdiPt = rc.TopLeft();
	schild->m_rectMDI.CopyRect(&rc);
	schild->m_bBackGround = child->m_bBackGround;
	schild->m_fixH = child->m_fixH;
	schild->m_fixW = child->m_fixW;

	
	
	child->SendMessage(WM_CLOSE);

	if (vwKIND == TABLISTVIEW)
	{
#ifdef DF_USE_CPLUS17
		((CAxTabView*)oldView)->JustWindow(Axis::home, m_main->GetTabKey(mapName), m_main->m_tMenu.get(), mapName, axiscall, true);
#else
		((CAxTabView *)oldView)->JustWindow(Axis::home, m_main->GetTabKey(mapName), m_main->m_tMenu, mapName, axiscall, true);
#endif
		schild->SetTabView(true);
	}

	
	if (vs == m_main->m_vsN)
	{
		//schild->SetWindowPos(m_main, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		schild->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOZORDER);
	}
	else
	{
		schild->ShowWindow(SW_HIDE);
	}
	if(schild->m_mapN == "IB0000X8")
	{
		schild->ShowWindow(SW_HIDE);

		if(m_main->m_miniSWid == NULL)
		{
			m_main->m_miniSWid = schild;
		}
	}
	//m_main->HideChildButton(key);
	m_main->writeOpenedList();	
	if (schild->m_mapN == "IB820850") schild->ShowWindow(SW_HIDE);
	if (schild->m_bBackGround)	schild->ShowBGCaption(true);
}

void CMapHelper::TransformSingleWindowToMDIChild(int key, int vs)
{
	CSChild* schild;

	if (!m_main->m_arSDI[vs].Lookup(key, schild))
		return;

	CString mapName;
	int group{}, font{};

	mapName = schild->m_mapN;
	group = schild->m_xcaption.GetGroup();
	font = schild->m_xcaption.GetFontSize();
	
	int vtype = 0, vwTYPE = 0, size = 0, vwKIND{};

	if (!GetMapInfo(mapName, vtype, size))
		return ;

	vwTYPE = vtypeMSK & vtype;
	vwKIND = (vwTYPE == vtypeSCR) ? 2 : 1;
	if (m_main->IsTabView(mapName))
		vwKIND = TABLISTVIEW;

	CView*		oldView = schild->GetActiveView();
	CView*		newView = m_main->GetNewView(vwKIND);
	CChildFrame*	child= (CChildFrame *) newView->GetParent();

	CRect rc;
	schild->GetWindowRect(&rc);
	oldView->SetParent(child);

	CString title = schild->m_xcaption.GetTitle();
	CRect restoreRC;
	CPoint restoreOffset;
	restoreRC = schild->m_rectMDI;
	restoreOffset = schild->m_mdiPt;

	m_main->RemoveSdi(vs, key);
	m_main->AddMdi(vs, key, child);

	m_main->m_arMDI[vs].SetAt(key, child);

	child->m_mapN = schild->m_mapN;
	child->m_xcaption.SetGroup(schild->m_xcaption.GetGroup());
	child->m_xcaption.SetFontSize(schild->m_xcaption.GetFontSize());

	child->m_key = key;
	child->m_vwTYPE = vtype;

	child->SetSize(rc.Width(), rc.Height(), FALSE);
	child->m_xcaption.SetTitle(title);

	child->SetMisc(WM_AXIS, m_main, axiscall, font, vwTYPE == vtypeWND ? false: true);
	child->SetActiveView(oldView);
#ifdef DF_MK_CAPTION
	child->m_xcaption.m_marketN = schild->m_xcaption.m_marketN;
#endif
	newView->SetParent(schild);
	schild->SetActiveView(newView);
	child->m_bBackGround = schild->m_bBackGround;
	child->m_fixH = schild->m_fixH;
	child->m_fixW = schild->m_fixW;

	schild->SendMessage(WM_CLOSE);
	if (child->m_bBackGround) child->ShowBackGround(true);
	if (vwKIND == TABLISTVIEW)
	{
#ifdef DF_USE_CPLUS17
		((CAxTabView*)oldView)->JustWindow(Axis::home, m_main->GetTabKey(mapName), m_main->m_tMenu.get(), mapName, axiscall);
#else
		((CAxTabView *)oldView)->JustWindow(Axis::home, m_main->GetTabKey(mapName), m_main->m_tMenu, mapName, axiscall);
#endif
		child->SetTabView(true);
	}
	/*
	if (restoreRC.Width() > 0 && restoreRC.Height())
	{
		//AfxMessageBox("1");
		child->SetSize(restoreRC.Width(), restoreRC.Height(), FALSE);
		//restoreRC.bottom += 10;
		child->MoveWindow(restoreRC);
	}
	else
	{
		//AfxMessageBox("2");
		child->MoveWindow(rc);
	}
	*/
	
	int lgap = (rc.left - restoreRC.left);
	const int tgap = (rc.top - restoreRC.top);
	rc.top -= tgap;
	rc.bottom -= tgap;
	rc.left -= lgap;
	rc.right -= lgap;
	if (rc.right<100)
	{
		lgap = rc.left*-1;
		rc.left += lgap;
		rc.right += lgap;
	}
	//CString tmp; tmp.Format("%s] Left: %d, Top: %d",mapName, rc.left, rc.top);
	//TRACE(tmp+"\n");
	child->MoveWindow(rc);
	
	m_main->m_MClient->ScreenToClient(&rc);
	
	child->m_sdiPt = rc.TopLeft();
	child->m_rectSDI.CopyRect(&rc);
	if (child->m_bBackGround) child->ShowBackGround(true);
	if (vs == m_main->m_vsN)
		child->ShowWindow(SW_SHOW);
	else
		child->ShowWindow(SW_HIDE);

	CString s;
	s.Format("%s",mapName);
	
	mapName.TrimLeft();
	mapName.TrimRight();

	if(s.Find("IB0000X8") > -1)
	{
		child->ShowWindow(SW_HIDE);
		OutputDebugString("HIDE");
		
		if(m_main->m_miniWid == NULL)
		{
			OutputDebugString("MINIWID");
			m_main->m_miniWid = child;
		}
	}

	m_main->writeOpenedList();	
	if (child->m_mapN == "IB820850") child->ShowWindow(SW_HIDE);
}

BOOL CMapHelper::minimizeAll(BOOL all)
{
	int		key{};
	POSITION	pos{};
	CChildFrame* child{};
	CSChild* schild{};
	CPoint sp;
	CRect wRc;
	for (int vsN = V_SCREEN1; vsN <= V_SCREEN6; vsN++)
	{
		if (!all)
		{
		if (/*!m_main->m_saveALLVS &&*/ vsN != m_main->m_vsN)
			continue;
		}
		
		for (pos = m_main->m_arMDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arMDI[vsN].GetNextAssoc(pos, key, child);
			if (!child->m_bBackGround && child->m_bIconic != TRUE)
			{
				sp = m_main->getMinimizePos(child->m_key);
				child->actionCaption(IDX_MIN, sp);
			}
		}

		for (pos = m_main->m_arSDI[vsN].GetStartPosition(); pos; )
		{
			m_main->m_arSDI[vsN].GetNextAssoc(pos, key, schild);
			if (!schild->m_xcaption.IsMax())
			{
				schild->GetWindowRect(wRc);
				schild->m_restoreRc.CopyRect(&wRc);
			}

			if(schild->m_bIconic != TRUE)
				schild->PostMessage(WM_SYSCOMMAND, SC_MINIMIZE);
		}
	}

	if (!all)	
		return TRUE;
	if (m_main->m_arGPOP.IsEmpty())	
		return TRUE;

	CGPop* pop{};
	for (pos = m_main->m_arGPOP.GetStartPosition(); pos; )
	{
		m_main->m_arGPOP.GetNextAssoc(pos, key, pop);
		pop->SendMessage(WM_CLOSE, 0, 0);
	}

	return TRUE;
}

void CMapHelper::CountMap( LPCSTR mapN )
{
	if (mapN==NULL) return;
	if (strlen(mapN)!=8) return;

	CString scrNo(mapN+2, 4);
#ifdef DF_USE_CPLUS17
	auto ft = m_counter.find(scrNo);
	if (ft == m_counter.end())
		m_counter.emplace(scrNo, 1);
	else
		ft->second += 1;
#else
	void* pCnt{};
	if (m_counter.Lookup((LPCSTR)scrNo, pCnt))
	{
		++*((int*)pCnt);
	}
	else
	{
		pCnt = new int;
		*((int*)pCnt) = 1;
		m_counter.SetAt((LPCSTR)scrNo, pCnt);
	}
#endif
}

void CMapHelper::FreeCountMap()
{
#ifdef DF_USE_CPLUS17
	m_counter.clear();
#else
	void* pCnt{};
	CString key;
	POSITION pos = m_counter.GetStartPosition();
	while(pos!=NULL)
	{
		m_counter.GetNextAssoc(pos, key, pCnt);
		delete pCnt;
	}
#endif
}

CString CMapHelper::GetCountData()
{
#ifdef DF_USE_CPLUS17
	CString retval{}, tmp{};

	for (std::map<CString, int>::iterator ii = m_counter.begin(); ii != m_counter.end(); ii++)
	{
		tmp.Format("%s=%d;", ii->first, ii->second);
		retval += tmp;
	}
	return retval;
#else
	void* pCnt{};
	CString key, retval = "", tmp;
	POSITION pos = m_counter.GetStartPosition();
	while(pos!=NULL)
	{
		m_counter.GetNextAssoc(pos, key, pCnt);
		tmp.Format("%s=%d;", key, *(int*)pCnt);
		retval += tmp;
	}
	return retval;
#endif
}

CMapHelper::~CMapHelper()
{
	FreeCountMap();
}

void CMapHelper::SetCountData( LPCSTR data, int dlen )
{
	if (dlen<1) return;
	
	LPCSTR st{}, ed{}, pos{};
	st = pos = data;
	ed = data + dlen;

	while( (pos=std::find(pos, ed, ';')) < ed )
	{
		CString expr(st, pos-st);
		const int equal_pos = expr.Find('=');
		if (equal_pos == 4)
		{
			CString scrNo = expr.Mid(0, equal_pos);
			CString scrCnt = expr.Mid(equal_pos + 1);
#ifdef DF_USE_CPLUS17
			auto ft = m_counter.find(scrNo);
			if (ft == m_counter.end())
				m_counter.emplace(scrNo, atoi(scrCnt));
			else
				ft->second += atoi(scrCnt);
#else
			void* pCnt;
			if (m_counter.Lookup((LPCSTR)scrNo, pCnt))
			{
				*((int*)pCnt) += atoi(scrCnt);
			}
			else
			{
				pCnt = new int;
				*((int*)pCnt) = atoi(scrCnt);
				m_counter.SetAt((LPCSTR)scrNo, pCnt);
			}
#endif
		}
		st = pos = pos + 1;
	}
}

#pragma warning (default : 6273)