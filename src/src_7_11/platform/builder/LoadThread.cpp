// LoadThread.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "LoadThread.h"
#include "mapvar.h"
#include "../h/axisrsm.h"
#include "../h/axisvar.h"
#include "awUser/DownLoad.h"
#include "awBuild/libBuild.h"
#include "awCommon/FormItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadThread

IMPLEMENT_DYNCREATE(CLoadThread, CWinThread)

CLoadThread::CLoadThread()
{
	m_running = false;
}

CLoadThread::~CLoadThread()
{
}

BOOL CLoadThread::InitInstance()
{
	m_running = false;
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CLoadThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CLoadThread, CWinThread)
	//{{AFX_MSG_MAP(CLoadThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadThread message handlers

int CLoadThread::Run() 
{
	m_running = true;
	wccSetTempPath(m_path, "preview_");

	m_section.Lock();
	CDownLoad *pDownload = new CDownLoad();
	
	if (!m_mapName.IsEmpty() && m_mapName.Find("template") == 0)
		pDownload->downLoad(m_mapName, m_path, 1, true);
	else
		pDownload->downLoad(m_mapName, m_path, 0, true);

	delete pDownload;
	m_section.Unlock();

	compile();
	m_pWnd->PostMessage(WM_USER);
	m_running = false;
	
	return CWinThread::Run();
}

void CLoadThread::setRoot(CString sRoot)
{
	m_root = sRoot;
}

void CLoadThread::setMapName(CString mapName)
{
	m_mapName = mapName;
}

bool CLoadThread::open(CString fileN)
{
	struct	_mapH     mapH;

	int	totL, tokcnt;
	int	formN, repN;
	WORD	type;

	totL = wccLoadSource(fileN.operator LPCTSTR(), (CWnd *) NULL);
	if (totL == -1)
		return false;

	ZeroMemory((char *)&mapH, sizeof(_mapH));
	mapH.mapGroup = 1;

	mapH.formN = 0;
	mapH.pathsetted = false;
	mapH.modified   = false;
	mapH.source = mapH.compile = false;

	formN = 0; repN = -1;
	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_token[ii].Clear();

	load(&mapH, m_root);
	for (int ii = 0; ii < totL; ii++)
	{
		tokcnt = wccParseToken(m_token, ii, &type);
		switch (type)
		{
		case TYPE_GLOBAL:
			buildHeader(m_token, tokcnt);
			break;
		case TYPE_FORM:
			buildForm(m_token, tokcnt, formN);
			formN++;
			break;
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
		default:
			break;
		}

		for (int jj = 0; jj < tokcnt; jj++)
			m_token[jj].Clear();
	}

	buildTab();

	mapH.onStart   = false;
	mapH.onSend    = false;
	mapH.onReceive = false;
	mapH.onAlert   = false;
	mapH.onService = false;
	mapH.onFile    = false;
	mapH.onSelect  = false;
	mapH.onTimer   = false;
	mapH.onFocus   = false;
	mapH.onClose   = false;
	mapH.onDevice  = false;
	mapH.onKey     = false;
	mapH.onTrigger = false;

	tokcnt = 0;
	CString sBuf = "";
	for (int ii = 0; ii < totL; ii++)
	{
		sBuf = wccParseResource(ii, &type);
		switch (type)
		{
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
			tokcnt++;
			buildResource((LPSTR)sBuf.operator LPCTSTR(), ii, formN); 
		default:	break;
		}
	}
	if (tokcnt > 0)
	{
		sBuf = "}";
		buildResource((LPSTR)sBuf.operator LPCTSTR(), totL, formN);
	}

	mapH.formN    = formN;
	mapH.source   = true;
	mapH.modified = false;

	switch (generateSource(false, &mapH))
	{
	case 0:
		mapH.source = true;	break;
	case 1:
	case 2:
	default:
		CFormItem FormItem(&mapH);
		FormItem.RemoveAllForm();

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return false;
	}

	CFormItem FormItem(&mapH);
	FormItem.RemoveAllForm();

	return true;
}

bool CLoadThread::compile()
{
	int	totL, rc;
	WORD	type;

	wccSetTempPath(m_srcN, "preview_");
	m_objN = m_srcN;
	m_objN += ".ax";

	if (!open(m_srcN))
		return false;

	totL = wccLoadSource(m_srcN, (CWnd *) NULL);
	if (totL == -1)
		return false;

	if (wccAllocateMemory(totL) < 0)
	{
		wccFreeMemory();
		return false;
	}

	if (wccAnalyzeColumn(totL) < 0)
	{
		wccFreeMemory();
		return false;
	}

	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_token[ii].Clear();

	rc = 0;
	for (int ii = 0; ii < totL; ii++)
	{
		rc += wccBuildToken(m_token, ii, &type);
		if (rc < 0)
			rc = 0;
	}
	wccSetDominoForm();
	wccLastCompileWork();

	if (wccBuildObject(m_objN) < 0)
	{
		wccFreeMemory();
		return false;
	}

	wccFreeMemory();
	return true;
}

void CLoadThread::setParent(CWnd *pWnd)
{
	m_pWnd = pWnd;
}
