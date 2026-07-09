// PreViewBar.cpp: implementation of the CPreViewBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiswork.h"
#include "PreViewBar.h"
#include "awObject/show.h"
#include "awUser/DownLoad.h"
#include "../h/axisrsm.h"
#include "../h/axisvar.h"
#include "awBuild/libBuild.h"
#include "awCommon/FormItem.h"
#include "../dll/lib/axislib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPreViewBar::CPreViewBar()
{
	m_bInvalidate = true;
	m_pBitmap = NULL;
	m_pBackBrush = NULL;
	m_pShow = NULL;
	m_pLoadThread = NULL;
}

CPreViewBar::~CPreViewBar()
{
	if (m_pShow)
		delete m_pShow;
	if (m_pBackBrush)
	{
		m_pBackBrush->DeleteObject();
		delete m_pBackBrush;
	}
	if (m_pBitmap)
		delete m_pBitmap;
	EndThread();
}

BEGIN_MESSAGE_MAP(CPreViewBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CPreViewBar)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

void CPreViewBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rt = m_ClientRt;

	if (m_bInvalidate)
		DrawBitmap(&dc);
	
	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap;
		
		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		
		dc.BitBlt(0, 0, rt.Width(), rt.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	
	// Do not call CSizingControlBarG::OnPaint() for painting messages
}

void CPreViewBar::DrawBitmap(CDC *pDC)
{
	CDC	memDC;
	CBitmap*	pOldBitmap;

	if (m_pBitmap)
		delete m_pBitmap;

	m_pBitmap = new CBitmap();

	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_ClientRt.Width(), m_ClientRt.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);

	CFont	font, *pOldFont;
	// set font
	LOGFONT lf = setFont(9, "");
	font.CreatePointFontIndirect(&lf, &memDC);
	pOldFont = (CFont*)memDC.SelectObject(&font);

	memDC.FillSolidRect(m_ClientRt, RGB(255, 255, 255));
	if (m_pShow)
	{
		CBrush* oldBrush = memDC.SelectObject(m_pBackBrush); 
		
		if (m_pShow->Load(m_filename, 1))
		{
			CSize sz = m_pShow->GetSize();
			CRect rc;
			if ((m_ClientRt.Width() * sz.cy)/sz.cx < m_ClientRt.Height())
				rc.SetRect(0, 0, m_ClientRt.Width(), (m_ClientRt.Width() * sz.cy)/sz.cx);
			else
				rc.SetRect(0, 0, (m_ClientRt.Height() * sz.cx)/ sz.cy, m_ClientRt.Height());
			
			memDC.PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
			memDC.SelectObject(oldBrush);
			
			m_pShow->Resize(rc);
			m_pShow->Draw(&memDC);
		}
	}
	
	memDC.SelectObject(pOldFont);
	font.DeleteObject();
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	//m_bInvalidate = false;
}

LOGFONT CPreViewBar::setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy_s(lf.lfFaceName, fname);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic = bItalic;
	return lf;
}

void CPreViewBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	
	GetClientRect(&m_ClientRt);
	
	m_bInvalidate = true;
	Invalidate();
}

int CPreViewBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CPreViewBar::setShow(CAxisPalette *palette, CAxisDraw *draw, CRect rect, CString root, CString fileN, CString caption, CString backBrush)
{
	m_palette = palette;
	m_draw = draw;
	m_root = root;
	m_filename = "preview_";
	if (m_pBackBrush)
	{
		m_pBackBrush->DeleteObject();
		delete m_pBackBrush;
	}
	if (backBrush.IsEmpty())
		m_pBackBrush = new CBrush(PALETTERGB(192, 192, 192));
	else
	{
		if (atoi(backBrush) & 0x02000000/*userCOLOR*/)
			m_pBackBrush = new CBrush(atoi(backBrush));
		else
			m_pBackBrush = new CBrush(m_palette->GetPaletteRGB(atoi(backBrush)));
	}

	/*if (m_mapName.Compare(fileN))
	{
		m_mapName = fileN;
		
		wccSetTempPath(m_path, "preview_");

		CDownLoad *pDownload = new CDownLoad();
		
		if (!m_mapName.IsEmpty() && m_mapName.Find("template") == 0)
			pDownload->downLoad(m_mapName, m_path, 1, false);
		else
			pDownload->downLoad(m_mapName, m_path, 0, false);
		
		delete pDownload;
		compile();
		
		if (m_pShow)
			delete m_pShow;
		m_pShow = new CShow(this, m_palette, m_draw, m_rect, m_root, m_filename);
		Invalidate();
	}*/

	if (m_pLoadThread && m_pLoadThread->m_running)
		return;
	
	EndThread();

	if (!fileN.IsEmpty() && m_mapName.Compare(fileN))
	{
		m_mapName = fileN;
		m_pLoadThread = new CLoadThread();
		m_pLoadThread->m_bAutoDelete = FALSE;
		m_pLoadThread->setRoot(m_root);
		m_pLoadThread->setMapName(fileN);
		m_pLoadThread->setParent(this);
		if (!m_pLoadThread->CreateThread())
			AfxMessageBox("Thread Create failed!");
	}

	m_rect = m_ClientRt;
}

bool CPreViewBar::open(CString fileN)
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

bool CPreViewBar::compile()
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

LRESULT CPreViewBar::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_pShow)
		delete m_pShow;
	m_pShow = new CShow(this, m_palette, m_draw, m_rect, m_root, m_filename);
	Invalidate();

	return 0;
}

void CPreViewBar::EndThread()
{
	if (m_pLoadThread)
	{
		DWORD dwResult = ::WaitForSingleObject(m_pLoadThread->m_hThread, 100);
		
		if ( dwResult == WAIT_OBJECT_0 )
		{
		}
		else if ( dwResult == WAIT_TIMEOUT )
		{
			DWORD dwExitCode;
			GetExitCodeThread(m_pLoadThread->m_hThread, &dwExitCode);
			TerminateThread(m_pLoadThread->m_hThread, dwExitCode);
		}
		delete m_pLoadThread;
		m_pLoadThread = NULL;
	}
}
