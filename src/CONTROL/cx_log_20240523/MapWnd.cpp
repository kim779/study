// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_test.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(_param* pParam, CWnd* pWizard )
{
	EnableAutomation();
	m_pWizard = pWizard;
	m_sParamKey = pParam->key;
	m_sParamName = CString(pParam->name,pParam->name.GetLength());
}

CMapWnd::~CMapWnd()
{
}

void CMapWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMapWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMapWnd)
	DISP_FUNCTION(CMapWnd, "axlog", axlog, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "IDtoSha", IDtoSha, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "getroot", getroot, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMapWnd, "openPDF", openPDF, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "Writelog", Writelog, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "HTSEncode", HTSEncode, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "ExcuteFile", ExcuteFile, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMapWnd, "OpenChrome", dispidOpenChrome, OpenChrome, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMapWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {867CD1F0-86C2-4642-A65D-9036298C4597}
static const IID IID_IMapWnd =
{ 0x867cd1f0, 0x86c2, 0x4642, { 0xa6, 0x5d, 0x90, 0x36, 0x29, 0x8c, 0x45, 0x97 } };

BEGIN_INTERFACE_MAP(CMapWnd, CWnd)
	INTERFACE_PART(CMapWnd, IID_IMapWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

void CMapWnd::axlog(LPCTSTR stag, LPCTSTR slog) 
{
	CString stmp;

	stmp.Format("[axlog] %s  [%s]\b", stag, slog);
	OutputDebugString(stmp);
}

UINT AFXAPI CMapWnd::HashDataAXIS(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

BSTR CMapWnd::IDtoSha(LPCTSTR sId) 
{
	CString strResult;

	strResult.Format("%08u", HashDataAXIS((LPCSTR)sId));
	return strResult.AllocSysString();
}

BSTR CMapWnd::getroot() 
{
	TCHAR	szTemp[MAX_PATH]={0};
	CString strResult;

	::GetModuleFileName(0, szTemp, sizeof(szTemp));
	strResult = szTemp;
	strResult.Replace("exe\\axis.exe", "");
	return strResult.AllocSysString();
}

void CMapWnd::openPDF(LPCTSTR strPDF) 
{
	CString strpath;
	TCHAR	szTemp[MAX_PATH]={0};

	::GetModuleFileName(0, szTemp, sizeof(szTemp));
	strpath = szTemp;
	strpath.Replace("exe\\axis.exe","");
	strpath += "tab\\";
	strpath += strPDF;

	::ShellExecute(NULL, "open", (LPCTSTR)strpath, NULL, NULL, SW_SHOWNORMAL); 
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_sParamKey, evOnDblClk)), (LPARAM)m_sParamName.GetString());
}

void CMapWnd::Writelog(LPCTSTR spath, LPCTSTR slog) 
{
	TCHAR	szTemp[MAX_PATH]={0};
	CString DefaultFileDataPath;
	CString sroot;

	::GetModuleFileName(0, szTemp, sizeof(szTemp));
	DefaultFileDataPath = szTemp;
	DefaultFileDataPath.Replace("exe\\axis.exe", "");
	sroot.Format("%s", spath);
	sroot.TrimRight();
	if (sroot.IsEmpty())
		DefaultFileDataPath += "tab\\url.ini";

	CFileFind finder;
	if (!finder.FindFile(DefaultFileDataPath))
		WritePrivateProfileString("USER","ENC","1",DefaultFileDataPath);

	TRY 
	{
		FILE *fp = fopen(DefaultFileDataPath, "a+");
		if (!fp) return;
		
		const	CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));
		
		va_list argptr;
		va_start(argptr, slog);
		vfprintf(fp, slog, argptr);
		va_end(argptr);
		fprintf(fp, "\n");
		
		fclose(fp);
	}
	CATCH (CMemoryException, e)
	{
		
	}
	END_CATCH

}

void CMapWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonUp(nFlags, point);
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here

	return 0;
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnTimer(nIDEvent);
}

CString Encode(const char* lpszSource, const char* key)
{
	const	int	lenKey = strlen(key);
	const	int	lenSource = strlen(lpszSource);

	if (!lenSource || !lenKey)
		return "";

	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(lenSource + 1);

	for (int ii = 0, idata = 0; ii < lenSource; ii++)
	{
		idata = key[ii % lenKey];
		idata = 0x0f & idata;
		idata =	lpszSource[ii] ^ idata;
		buffer[ii] = (lpszSource[ii] ^ (0x0f & key[ii % lenKey])) - 1;
	}
	buffer[lenSource] = 0x00;

	CString result;
	result = CString(buffer.get());

	/** Decoding test 

	for (int ii = 0; ii < lenSource; ii++)
	{
		buffer[ii] = (result[ii] + 1) ^ (0x0f & key[ii % lenKey]);
	}

	AfxMessageBox(result);
	AfxMessageBox(buffer);
	**/

	buffer.reset();

	return result;
}

BSTR CMapWnd::HTSEncode(LPCTSTR sID, LPCTSTR sKey) 
{
	CString strResult;
	CString strkey;

	strkey.Format("%s", sKey);
	strkey.TrimRight();
	if (strkey.IsEmpty())
		strkey = "ibkis";

	strResult = Encode(sID, strkey);
	return strResult.AllocSysString();
}

void CMapWnd::ExcuteFile(LPCTSTR fullpath) 
{
	::ShellExecute(NULL, "open", fullpath, NULL, NULL, SW_SHOWNORMAL); 
}


void CMapWnd::OpenChrome(BSTR strUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sUrl;
	sUrl.Format("%s", strUrl);
	ShellExecute(nullptr, "open", "chrome.exe", sUrl, nullptr, SW_SHOWNORMAL);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}
