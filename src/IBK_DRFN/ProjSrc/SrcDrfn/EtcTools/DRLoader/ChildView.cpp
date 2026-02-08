// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DRLoader.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView() :
	m_pLoadLib(NULL),
	m_pChildWnd(NULL)
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}


void CChildView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_pLoadLib) return;

	//TRACE("point[%d, %d]\n", point.x, point.y);

	UINT nPopMenuID = 1000;
	CMenu menu;
	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING,	nPopMenuID ,	"DLL화면열기");

	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, point.x, point.y, this);
	if(!nRetValue) return;

	menu.DestroyMenu();

	if(nRetValue==1000)
		_DoOpenDll();
}

#include "../../inc/EnvDir.h"
void CChildView::_DoOpenDll()
{
	// 파일 대화상자.
	char ext[] = "dll";
	DWORD	dwFlag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER|OFN_LONGNAMES|OFN_NONETWORKBUTTON;
	char szFilter[] = "DrfnContents Files (*.dll)|*.dll||";

	CFileDialog openDlg(TRUE, ext, NULL, dwFlag, szFilter, NULL);

	CString strInitDir = AfxGetApp()->GetProfileString("MAIN", "OpenDir", "");
	if(strInitDir.GetLength()==0)
		GET_MAIN_DIR(strInitDir);

	openDlg.m_ofn.lpstrInitialDir = (LPCTSTR)strInitDir;
	if (openDlg.DoModal() != IDOK)
		return;
	
	CString szDllName = openDlg.GetPathName();	// Open file path

	int _loc_ = szDllName.ReverseFind('\\');
	if(_loc_>0)
	{
		strInitDir = szDllName.Left(_loc_);
		AfxGetApp()->WriteProfileString("MAIN", "OpenDir", strInitDir);
	}
	
	_DoOpenDllFile(szDllName);
}


// void CChildView::_DoOpenDllFile(LPCSTR szDllName)
// {
// 	if(!m_pLoadLib)
// 	{
// 		m_pLoadLib = new CLoadLib;
// 		m_pLoadLib->m_szDllName = szDllName;
// 		HMODULE hLib = m_pLoadLib->LoadLib();
// 		if(!hLib)
// 		{
// 			delete m_pLoadLib;
// 			m_pLoadLib = NULL;
// 		}
// 
// 		typedef  CWnd* (WINAPI* View_axCreate)(CWnd *pParent);
// 		View_axCreate m_fpDrdsInit = (View_axCreate)::GetProcAddress(hLib, "axCreate");
// 		if(m_fpDrdsInit)
// 		{
// 			CWnd* pWnd = m_fpDrdsInit(this);
// 			if(pWnd)
// 			{
// 				m_pChildWnd = pWnd;
// 				CRect rect;
// 				GetClientRect(&rect);
// 				pWnd->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_SHOWWINDOW|SWP_NOZORDER);
// 			}
// 		}
// 	}
// }

void CChildView::_DoOpenDllFile(LPCSTR szDllName)
{
	if(!m_pLoadLib)
	{
		m_pLoadLib = new CLoadLib;
		m_pLoadLib->m_szDllName = szDllName;
		HMODULE hLib = m_pLoadLib->LoadLib();
		if(!hLib)
		{
			delete m_pLoadLib;
			m_pLoadLib = NULL;
		}

		typedef CWnd * (CreateTradingAup)(UINT nDldresId, CWnd* pParent, LPCTSTR strMapFileName);
		CreateTradingAup *pCreateTrading = (CreateTradingAup *)GetProcAddress(hLib,_T("CREATE_TRADING"));
		if(pCreateTrading)
		{
			CWnd* pWnd = pCreateTrading(9999, this, "test");
			if(pWnd)
			{
				m_pChildWnd = pWnd;
				CRect rect;
				GetClientRect(&rect);
				pWnd->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_SHOWWINDOW|SWP_NOZORDER);
			}
		}
	}
}

void CChildView::OnDestroy() 
{
	if(m_pLoadLib)
	{
		if(m_pChildWnd->GetSafeHwnd() && IsWindow(m_pChildWnd->GetSafeHwnd()))
			m_pChildWnd->DestroyWindow();
		m_pChildWnd = NULL;
		m_pLoadLib->UnLoadLib();
		delete m_pLoadLib;
		m_pLoadLib = NULL;
	}
	
	CWnd ::OnDestroy();	

}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_pChildWnd && m_pChildWnd->GetSafeHwnd())
	{
		m_pChildWnd->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE|SWP_NOZORDER);
	}	
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pChildWnd && m_pChildWnd->GetSafeHwnd())
		return FALSE;
	
	else
		return CWnd ::OnEraseBkgnd(pDC);
}
