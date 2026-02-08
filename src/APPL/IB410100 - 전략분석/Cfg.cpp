// Cfg.cpp : implementation file
//

#include "stdafx.h"
#include "Cfg.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfg

CCfg::CCfg()
{
	m_hInst		= NULL;
	m_WCfg		= NULL;
	m_ICfg		= NULL;
}

CCfg::~CCfg()
{

}


BEGIN_MESSAGE_MAP(CCfg, CWnd)
	//{{AFX_MSG_MAP(CCfg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnSend)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCfg message handlers


BOOL CCfg::CreateCfg(CWnd* parent, CString root)
{
	m_parent = parent;
	m_root   = root;

	return CWnd::Create(NULL, "CfgWnd", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), parent, 1001);
}

int CCfg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	struct	_param	param;
	CString	path; path.Format("%s/%s/%s", m_root, DEVDIR, "CX_CFG.DLL");

	param.name	= "Cfg";
	param.fonts	= "±¼¸²Ã¼";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 63;
	param.pRGB	= 62;
	param.options = "/mMINGAM/dIB4015C0";
	GetClientRect(&param.rect);

	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("CX_CFG control load error....\n");
		return -1;
	}

	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hInst, "axCreate");
	if (axCreate == NULL)
	{
		TRACE("chart axCreate proc error...\n");
		return -1;
	}

	m_WCfg = axCreate(this, (void *) &param);
	m_ICfg = std::make_unique<ICfgWnd>(m_WCfg->GetIDispatch(TRUE));
	
	return 0;
}

void CCfg::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_WCfg != NULL)
	{
		m_WCfg->DestroyWindow();	delete m_WCfg;
		m_ICfg->DetachDispatch();//	delete m_ICfg;
	}
	if (m_hInst)	FreeLibrary(m_hInst);	
}

BOOL CCfg::SetMap(LPCTSTR PrivateCfgName, LPCTSTR PublicCfgName, LPCTSTR CfgMapName)
{
	if (m_ICfg == NULL) return FALSE;
	return m_ICfg->SetMap(PrivateCfgName, PublicCfgName, CfgMapName);
}

BOOL CCfg::ShowDlg()
{
	if (m_ICfg == NULL) return FALSE;
	return m_ICfg->ShowDlg();
}

CString CCfg::ReadCfg(LPCTSTR Ident)
{
	if (m_ICfg == NULL) return "";
	return m_ICfg->ReadCfg(Ident);
}

void CCfg::LoadDefaultCfg()
{
	if (m_ICfg == NULL) return;
	m_ICfg->LoadDefaultCfg();
}

void CCfg::SaveCfg()
{
	if (m_ICfg == NULL) return;
	m_ICfg->SaveCfg();
}

void CCfg::WriteCfg(LPCTSTR Ident, LPCTSTR Value)
{
	if (m_ICfg == NULL) return;
	m_ICfg->WriteCfg(Ident, Value);
}

void CCfg::LoadCfg()
{
	m_ICfg->LoadCfg();
}

long CCfg::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case variantDLL:
		return m_parent->SendMessage(WM_USER, wParam, lParam);
		break;
	case viewDLL:
		return m_parent->SendMessage(WM_USER, wParam, lParam);
		break;
	default:
		TRACE("CCFG::OnSend wParamLo:%d wParamHi:%d", LOWORD(wParam), HIWORD(wParam));
		break;
	}
	return 0;
}

void CCfg::SetCfgMap(LPCTSTR CfgMapName)
{
	m_ICfg->SetCfgMap(CfgMapName);
}
