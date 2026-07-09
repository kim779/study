// axMisc.cpp: implementation of the CAxMisc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axis.h"
#include "axisDoc.h"
#include "axMisc.h"
#include "mainfrm.h"
#include "childfrm.h"
#include "NPalette.h"
#include "../h/axisvar.h"

#include <comdef.h>
#define INITGUID
#include <initguid.h>
#include <exdisp.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////
// _axisRes
_axisRes::_axisRes(CString home)
{
	m_bmp[IDX_COPY].LoadBitmap(IDB_COPY);
	m_bmp[IDX_SINGLE].LoadBitmap(IDB_SINGLE);
	m_bmp[IDX_PIN].LoadBitmap(IDB_PIN);
	m_bmp[IDX_DESIZE].LoadBitmap(IDB_RESTORE);
	m_bmp[IDX_CAPTURE].LoadBitmap(IDB_CAPTURE);
	m_bmp[IDX_MIN].LoadBitmap(IDB_MIN);
	m_bmp[IDX_MAX].LoadBitmap(IDB_MAX);
	m_bmp[IDX_CLOSE].LoadBitmap(IDB_CLOSE);
	m_bmp[IDX_RESTORE].LoadBitmap(IDB_RESTORE);
	m_bmp[IDX_GROUP].LoadBitmap(IDB_GROUP);
//	m_bmp[IDX_HELP].LoadBitmap(IDB_HELP);
	m_bmp[IDX_V1].LoadBitmap(IDB_V1);
	m_bmp[IDX_V2].LoadBitmap(IDB_V2);
	m_bmp[IDX_V3].LoadBitmap(IDB_V3);
	m_bmp[IDX_V4].LoadBitmap(IDB_V4);
	m_bmp[IDX_CHILD].LoadBitmap(IDB_CHILD);  
	m_bmp[IDX_TOOLBARBG].LoadBitmap(IDB_TOOLBARBG);
	m_bmp[IDX_DUAL].LoadBitmap(IDB_DUAL);  

	m_bmp[IDX_CHILD_MIN].LoadBitmap(IDB_CHILD_MIN);  
	m_bmp[IDX_CHILD_MAX].LoadBitmap(IDB_CHILD_MAX);  
	m_bmp[IDX_CHILD_CLOSE].LoadBitmap(IDB_CHILD_CLOSE); 
	m_bmp[IDX_CHILD_RESTORE].LoadBitmap(IDB_CHILD_RESTORE); 

	m_icon.LoadBitmap(IDB_MAIN);

	m_home = home;
	m_font.CreatePointFont(90, "±¼¸²");
	m_fontB.CreateFont(	   12,                        // nHeight
				   0,                         // nWidth
				   0,                         // nEscapement
				   0,                         // nOrientation
				   FW_SEMIBOLD,	              // nWeight
				   FALSE,                     // bItalic
				   FALSE,                     // bUnderline
				   0,                         // cStrikeOut
				   DEFAULT_CHARSET,              // nCharSet
				   OUT_DEFAULT_PRECIS,        // nOutPrecision
				   CLIP_DEFAULT_PRECIS,       // nClipPrecision
				   DEFAULT_QUALITY,           // nQuality
				   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				   "±¼¸²");                 // lpszFacename

	m_palette = new CNPalette(m_home);
}

_axisRes::~_axisRes()
{
	int ii = 0;
	for (ii = 0; ii < BMP_CNT; ii++)
		m_bmp[ii].DeleteObject();
	for (ii = 0; ii < TAB_CNT; ii++)
	{
		if (m_tab[ii].GetSafeHandle())
			m_tab[ii].DeleteObject();
	}
	
	m_font.DeleteObject();
	m_fontB.DeleteObject();
	if (m_palette)	delete m_palette;
}

CFont* _axisRes::GetFont()
{
	return &m_font;
}

CFont* _axisRes::GetBoldFont()
{
	return &m_fontB;
}

CBitmap* _axisRes::GetBmp(int index/*, bool down, int groupN*/)
{
	if (index == IDX_ICON)
		return &m_icon;
	else if (index < 0 || index >= BMP_CNT)
		return NULL;

	return &m_bmp[index];
}

CBitmap* _axisRes::GetTabImg(int index)
{
	if (m_tab[index].GetSafeHandle())
		return &m_tab[index];
	return NULL;
}

COLORREF _axisRes::GetColor(int index)
{
	if (!m_palette)	return RGB(255, 255, 255);
	return m_palette->GetColor(index);
}

void _axisRes::ChangeRES(CString skinName)
{
	HBITMAP	hBitmap;
	CString	filename;
	int ii = 0;

	for (ii = 0; ii < TAB_CNT; ii++)
		if (m_tab[ii].GetSafeHandle())	m_tab[ii].DeleteObject();

	ii = 0;
	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "l_tab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);

	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "tab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);

	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "r_tab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);

	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "l_stab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);

	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "stab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);

	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "r_stab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);

	filename.Format("%s\\image\\%s_%s.bmp", m_home, skinName, "b_tab");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_tab[ii++].Attach(hBitmap);
}

CBitmap* _axisRes::GetIcon()
{
	return &m_icon;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAxMisc::CAxMisc(CString root, CString regkey)
{
	m_root   = root;
	m_regkey = regkey;
	LoadGuide();
}

CAxMisc::~CAxMisc()
{
	m_guide.RemoveAll();
}

int CAxMisc::MsgBox(CString msg, CString title, UINT type)
{
	if (title.IsEmpty())	title = _T("NOTIFY");
	type |= (MB_ICONEXCLAMATION|MB_TOPMOST|MB_SETFOREGROUND);

	return MessageBox(NULL, msg, title.operator LPCTSTR(), type);
}

void CAxMisc::LoadGuide()
{
	CString		tmps;
	CStdioFile	sfile;

	m_guide.RemoveAll();
	tmps = _T("");
	m_guide.SetAt(0, tmps);
	tmps.Format("%s\\%s\\%s", m_root, MTBLDIR, AXISMSG);
	if (!sfile.Open(tmps, CFile::modeRead))
		return;

	int	idx, key;
	while (sfile.ReadString(tmps))
	{
		if (!tmps.Find("//"))
			continue;
		idx = tmps.Find('=');
		if (idx < 0)
			continue;
		key = atoi(tmps.Left(idx++));
		tmps = tmps.Mid(idx);
		idx = tmps.Find("//");
		if (idx != -1)
			tmps = tmps.Left(idx);
		tmps.TrimRight();
		m_guide.SetAt(key, tmps);
	}
	sfile.Close();
}

void CAxMisc::GetGuide(int key, CString& string)
{
	if (!m_guide.Lookup(key, string))
		string.Format("UNKNOWN ERROR ... [%d]", key);
}

BOOL CAxMisc::RunVers(int type, CString user, CString pass, CString cpass)
{
	CString	aps, cmds, exes;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	CAxisApp* app = (class CAxisApp *) AfxGetApp();
	if (app->m_exeName.IsEmpty())
		exes.Format("%s.exe", app->m_pszExeName);
	else	exes.Format("%s.exe", app->m_exeName);
	
	if (app->m_exeName.IsEmpty() && app->m_progK == 'B')
	{
		if (app->m_mode == MD_DEV)
			cmds.Format(" /c %d /d \"%s\" /a \"%s\" /p B /s %c", AfxGetMainWnd()->m_hWnd, m_root, exes, 0x7f);
		else	
			cmds.Format(" /c %d /d \"%s\" /a \"%s\" /p B", AfxGetMainWnd()->m_hWnd, m_root, exes);
	}
	else
	{
		if (app->m_mode == MD_DEV)
			cmds.Format(" /c %d /d \"%s\" /a %s /s %c", AfxGetMainWnd()->m_hWnd, m_root, exes, 0x7f);
		else	
			cmds.Format(" /c %d /d \"%s\" /a %s", AfxGetMainWnd()->m_hWnd, m_root, exes);
	}
	
	cmds += _T(" /u");
	if (!user.IsEmpty())
	{
		cmds += " ";
		cmds += user;
		cmds += " ";
		cmds += pass;
		cmds += "#";
		cmds += cpass;
	}

	if (type == verUSERID)
		cmds += _T(" /i");
	else if (type == verRETRY)
		cmds += _T(" /r");

	aps.Format("%s\\%s\\axisver.exe", m_root, RUNDIR);
	BOOL bRc = CreateProcess(
			aps,				// application name
			(char *)cmds.operator LPCTSTR(),// command line
			NULL,				// process attribute
			NULL,				// thread attribute
			FALSE,				// is inherit handle
			0,				// creation flags
			NULL,				// environment
			NULL,				// current directory
			&si,				// STARTUPINFO
			&pi);				// PROCESS_INFORMATION

	return bRc;
}