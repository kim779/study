// CSTool01.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "CSTool01.h"

#define CSTOOL_HEIGHT	50
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSTool01

CCSTool01::CCSTool01(CWnd* parent, CString home)
{
	m_parent	= parent;
	m_home		= home;
	m_height	= CSTOOL_HEIGHT;
	m_instance	= NULL;
	m_tool		= NULL;
}

CCSTool01::~CCSTool01()
{
}


BEGIN_MESSAGE_MAP(CCSTool01, CDialogBar)
	//{{AFX_MSG_MAP(CCSTool01)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSTool01 message handlers
CSize CCSTool01::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;
	size.cx = 32767;
	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CCSTool01::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CCSTool01::loadCStool()
{
	if (m_instance)	return;

	CString	file;
	file.Format("%s\\debug\\cstool01.dll", m_home);

	m_instance = LoadLibrary(file);
	if (!m_instance)
	{
		//**AfxMessageBox("DLL load fail!");
		Axis::MessageBox("DLL laod fail!");
		return;
	}

	axNTool = (_axNTool) GetProcAddress(m_instance, "_axNTool");
	if (!axNTool)
	{
		//AfxMessageBox("Not found function pointer!");
		Axis::MessageBox("Not found function pointer!");
		return;
	}

	m_tool = (CWnd*)(*axNTool)(m_parent, WM_TOOLEVENT, m_home, "");
	
}
