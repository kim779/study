// Common.cpp
#include "stdafx.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void XZero(LPVOID buffer, int len)
{
	ZeroMemory(buffer, len);
}

void XCopy(LPVOID dest, LPVOID src, int len)
{
	memcpy(dest, src, len);
}

void XFill(LPVOID buffer, char ch, int len)
{
	memset(buffer, ch, len);
}

void XSpace(LPVOID buffer, int len)
{
	XFill(buffer, ' ', len);
}

int XStrLen(LPCTSTR str)
{
	return lstrlen(str);
}

LPSTR XStrCat(LPSTR str, LPCTSTR catStr)
{
	return lstrcat(str, catStr);
}

LPSTR XStrCpy(LPSTR dest, LPCTSTR src)
{
	return lstrcpy(dest, src);
}

LPSTR XStrNCpy(LPSTR dest, LPCTSTR src, int len, BOOL nullTerminate)
{
	strncpy(dest, src, (size_t)len);
	if (nullTerminate)
		dest[len] = 0x00;

	return dest;
}

LPCTSTR XTruncateZero(CString& str)
{
	if (str.GetLength() < 2)
		return (LPCTSTR) str;

	for (int i = 0; i < str.GetLength(); i++) {
		if (str[i] != '0')
			break;
	}

	str.Delete(0, i);
	if ((str.IsEmpty()) || (str[0] == '.') || (str[0] == '\''))
		str.Insert(0, '0');
	return (LPCTSTR)str;
}

LPCTSTR XCommaString(CString& str)
{
	TRIM(str);

	int len = str.GetLength();
	str.Replace(".", EMPTYSTR);

	int dotPos = str.Find('.') + 1;
	int digit = len - dotPos;

	if (digit < 4)
		return (LPCTSTR)str;

	CString value;
	int count = (digit - 1) / 3;
	value.Empty();

	for (int i = 0; i < digit; i++) {
		if ((digit - i) % 3 == 0 && i)
			value += ',';
		value += str[i];
	}
	str = value;
	return (LPCTSTR)str;
}

///////////////////////////////////////////////////////////////////////////////////

BOOL IsFileExist(LPCTSTR lpszFileName)
{
	CFileFind ff;
	if (ff.FindFile(lpszFileName)) {
		ff.Close();
		return TRUE;
	}
	return FALSE;

	
	CFile* pFile;

	TRY {
		pFile = new CFile(lpszFileName, CFile::modeRead | CFile::shareDenyNone);
	}
	CATCH (CFileException, e) {
		char buffer[MAX_PATH];
		e->GetErrorMessage(buffer, MAX_PATH);
		AfxGetMainWnd()->MessageBox(buffer, COMPANYNAME);
		//**THROW(e);
		return FALSE;
	}
	END_CATCH;
	delete pFile;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Window Handling Routine
void XCenterWindow(CWnd* pWnd, CWnd* pParent)
{
	//Debug("XCenterWindow");
	if (pWnd == NULL) {
		//Debug("pWnd == NULL");
		return;
	}

	if (!pParent)
		pParent = pWnd->GetParent();

	if (!pParent) {
		//Debug("pParent == NULL");
		return;
	}

	CRect rcParent, rcClient;
	pParent->GetClientRect(rcParent);
	pWnd->GetClientRect(rcClient);

	pWnd->MoveWindow(rcParent.Width() / 2 - rcClient.Width() / 2, rcParent.Height() / 2 - rcClient.Height() / 2, 
		rcClient.Width(), rcClient.Height(), TRUE);
}

CRect XCenterWindowRect(CWnd* pParent, int cx, int cy)
{
	CRect rcParent;
	pParent->GetClientRect(rcParent);

	return CRect(rcParent.Width() / 2 - cx / 2, rcParent.Height() / 2 - cy / 2, 
		cx + rcParent.Width() / 2 - cx / 2, cy + rcParent.Height() / 2 - cy / 2);
}

BOOL XRegisterWindowClass(LPCTSTR lpszClassName, HBRUSH hbrBackground)
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
	
    if (!(::GetClassInfo(hInst, lpszClassName, &wndcls)))
    {
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = hbrBackground;
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = lpszClassName;
		
        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }
	
    return TRUE;
}


   

///////////////////////////////////////////////////////////////////////////////////////////////
// string routine
int	XParseString(LPCTSTR data, char delim, CStringList& list)
{
	CString s(data);
	list.RemoveAll();
	int nPos;

	while ((nPos = s.Find(delim)) >= 0) {
		if (nPos > 0)
			list.AddTail(s.Left(nPos));
		s.Delete(0, nPos + 1);
	}

	if (!s.IsEmpty())
		list.AddTail(s);

	return list.GetCount();
}

CString XParse(CString &srcstr, char delim)
{
	if (srcstr.Find(delim) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(delim));
		srcstr = srcstr.Mid(srcstr.Find(delim) + sizeof(char));
		return temp;
	}
	return "";
}

CString XParse(CString &srcstr, LPCTSTR delim)
{
	if (srcstr.Find(delim) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(delim));
		srcstr = srcstr.Mid(srcstr.Find(delim) + lstrlen(delim));
		return temp;
	}
	return "";
}

///////////////////////////////////////////////////////////////////////////////////////////////
// misc.

/***********************************
	IsControlKey
************************************/
BOOL IsControlKey(BYTE key)
{
	BYTE keys[] = {
		VK_RETURN, /**VK_BACK, **/VK_HOME, VK_END, VK_LEFT, VK_UP, VK_DOWN, VK_RIGHT, 
		VK_PRIOR, VK_NEXT, NULL
	};
	for (int i = 0; keys[i]; i++) {
		if (key == keys[i]) {
			return TRUE;
		}
	}
	return FALSE;		
}

