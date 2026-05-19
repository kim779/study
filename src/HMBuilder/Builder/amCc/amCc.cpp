// amCc.cpp : 해당 DLL의 초기화 루틴을 정의합니다.
//

#include "stdafx.h"
#include "amCc.h"
#include "../h/mainvar.h"

//#include "../amCrypt/libCrypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며 
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// CamCcApp

BEGIN_MESSAGE_MAP(CamCcApp, CWinApp)
END_MESSAGE_MAP()


// CamCcApp 생성

CamCcApp::CamCcApp()
{
	m_trlayout.RemoveAll();
	m_trlayoutxml.RemoveAll();
	m_rtssymbol.RemoveAll();
	m_rtssymbolxml.RemoveAll();
	m_control.RemoveAll();
	m_loadControl = false;
}


CamCcApp::~CamCcApp()
{
	m_trlayout.RemoveAll();
	m_trlayoutxml.RemoveAll();
	m_rtssymbol.RemoveAll();
	m_rtssymbolxml.RemoveAll();
	m_control.RemoveAll();
}
// 유일한 CamCcApp 개체입니다.

CamCcApp theApp;


// CamCcApp 초기화

BOOL CamCcApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CamCcApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::ExitInstance();
}

void CamCcApp::SetTempPath(CString& dir, char* fileN, bool source)
{
	dir.Format("%s\\%s", m_curdir, fileN);
	if (!source)
		dir += ".xml";
}

void CamCcApp::SetMapFilePath(CString& dir, char* fileN, bool source)
{
	if (source)
		dir += _T("\\src\\");
	else
		dir += _T("\\obj\\");

	dir += fileN;
}

void CamCcApp::GetTempName(char* fileN, char* prefix)
{
	char	wb[256];
	size_t	size = 0;

	errno_t err = getenv_s(&size, wb, sizeof(wb), "TMP");
	_putenv(_T("TMP=NULL"));

	char*   tmpN  = _tempnam((char *)m_curdir.operator LPCTSTR(), prefix);
	CString tmps = CString(tmpN);
	CopyMemory(fileN, (char*)tmps.operator LPCTSTR(), tmps.GetLength());
	fileN[tmps.GetLength()] = '\0';
	free(tmpN);

	if (size > 0)
	{
		tmps.Format(_T("TMP=%s"), wb);
		_putenv((char *)tmps.operator LPCSTR());
	}
}

void CamCcApp::setTrLayOut(int kind, CString tr)
{
	CString key, redata, tmp;
	
	tmp = tr;
	key = Parser(tmp, _T("\t"));

	switch (kind)
	{
	case 0:
		if (m_trlayout.Lookup(key, redata))
			m_trlayout.RemoveKey(key);
		if (!tmp.IsEmpty())
			m_trlayout.SetAt(key, tr);
//TRACE("-----------m_trlayout--------------%s%d\n", tr, m_trlayout.GetCount());
		break;
	case 1:
		if (m_trlayoutxml.Lookup(key, redata))
			m_trlayoutxml.RemoveKey(key);
		if (!tmp.IsEmpty())
			m_trlayoutxml.SetAt(key, tr);
//TRACE("-----------m_trlayoutxml--------------%s%d\n", tr, m_trlayoutxml.GetCount());
		break;
	}
	//m_build.SetTrlayoutxml(kind, tr);
}

CString CamCcApp::GetTrLayOut()
{
	CString tmp = _T("");
	/*CString sKey, str;

	for (POSITION pos = m_trlayoutxml.GetStartPosition(); pos != NULL; )
	{
		m_trlayoutxml.GetNextAssoc(pos, sKey, str);
		tmp += str;
		tmp += _T("^");
	}*/
	//tmp = m_build.GetTrLayOut();
	return tmp; 
}

void CamCcApp::setRtsSymbol(int kind, CString tr)
{
	CString key, redata, tmp;

	tmp = tr;
	key = Parser(tmp, _T("\t"));

	switch (kind)
	{
	case 0:
		if (m_rtssymbol.Lookup(key, redata))
			m_rtssymbol.RemoveKey(key);
		if (!tmp.IsEmpty())
			m_rtssymbol.SetAt(key, tr);
//TRACE("-----------m_rtssymbol--------------%d\n", m_rtssymbol.GetCount());
		break;
	case 1:
		if (m_rtssymbolxml.Lookup(key, redata))
			m_rtssymbolxml.RemoveKey(key);
		if (!tmp.IsEmpty())
			m_rtssymbolxml.SetAt(key, tr);
//TRACE("-----------m_rtssymbolxml--------------%d\n", m_rtssymbolxml.GetCount());
		break;
	}
}

CString CamCcApp::GetRtsSymbol()
{
	CString tmp = _T("");
	CString sKey, str;

	for (POSITION pos = m_rtssymbolxml.GetStartPosition(); pos != NULL; )
	{
		m_rtssymbolxml.GetNextAssoc(pos, sKey, str);
		tmp += str;
		tmp += _T("^");
	}

	return tmp; 
}

void CamCcApp::removeAllCMap()
{
	m_build.m_trlayoutxml.RemoveAll();
	m_build.m_rtssymbolxml.RemoveAll();
}

CString CamCcApp::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return _T("");
}

void CamCcApp::LoadControl()
{
	if (m_loadControl)
		return;

	CString path, desc;
	CString tmps, text, sym, tmp, idx, retval, str, han;
	char	wb[1024];
	DWORD	value;

	path.Format(_T("%s\\%s\\control.ini"), m_root, TABDIR);
	for (int ii = 1; ii < 100; ii++)
	{
		tmps.Format(_T("%02d"), ii);
		idx = tmps;

		GetPrivateProfileString(_T("Custom Objects"), tmps, _T(""), wb, sizeof(wb), path);
		desc = wb;
		tmps = Parser(desc, _T(";"));
		han  = Parser(desc, _T(";"));

		if (tmps.IsEmpty())
			break;

		m_control.SetAt(han, tmps);

		value = GetPrivateProfileSection(tmps, wb, sizeof(wb), path);
	}
	m_loadControl = true;
	m_build.m_control = &m_control;
	m_load.m_control = &m_control;
}