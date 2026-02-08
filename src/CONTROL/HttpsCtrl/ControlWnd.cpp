// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "HttpsCtrl.h"
#include "ControlWnd.h"

#include <Windows.h>
#include <atlstr.h> // CString 사용을 위한 헤더
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT constCtrlID		= 20900;
const UINT constHttpsCtrlID	=  1000;

#define WRITELOG	0

#define HTTPS_GET	1
#define HTTPS_POST	2

static char* MAPFIELD[] = { _T("Https_id"), _T("Httpsuserid"), _T("Httpsdeptcd"), _T("Httpssender"), _T("Httpssendnm"), _T("Httpsrecvllst"), _T("Httpsrecvnmlist"), _T("Httpssubj"), _T("Httpscont"), _T("Httpsattach") };

#define HTTPS_DEFAULT	0
#define HTTPS_ID	1
#define HTTPS_USERID	2
#define HTTPS_DEPTCD	3
#define HTTPS_SENDER	4
#define HTTPS_SENDDM	5
#define HTTPS_RELIST	6
#define HTTPS_RENMLIST	7
#define HTTPS_SUBJ	8
#define HTTPS_CONT	9
#define HTTPS_ATTACH	10
#define HTTPS_TEXTDATA	20
#define IDS_ADDRESS	757

#define HTTPS_MULTIHEADER	1
#define HTTPS_FORMHEADER	2
#define HTTPS_JSONHEADER	3

#define boundary		_T("----WebKitFormBoundaryu8FzpUGNDgydoA4zJaeho");

const UINT uEdgeCtrlID = 2000;

// Base64 문자표
const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				 "abcdefghijklmnopqrstuvwxyz"
				 "0123456789+/";

// Lookup table for Base64 characters to their 6-bit values
const int base64_chars_to_value[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
}

CControlWnd::~CControlWnd()
{
	ClearHttps();
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, _T("ConnectServer"), _ConnectServer, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("SendHttps"), _SendHttps, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, _T("SetHttpsInfo"), _SetHttpsInfo, VT_I4, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("SetHttpsSendMode"), _SetHttpsSendMode, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("ConvertUtf8"), _ConvertUtf8, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("ConvertAnsi"), _ConvertAnsi, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("Base64Encode"), _Base64Encode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("Base64Decode"), _Base64Decode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, _T("ConvertUtf8Len"), _ConvertUtf8Len, VT_BSTR, VTS_BSTR VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()
// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {58553D20-F769-4AB6-AF78-3CAA7E115074}
static const IID IID_IControlWnd =
{ 0x58553d20, 0xf769, 0x4ab6, { 0xaf, 0x78, 0x3c, 0xaa, 0x7e, 0x11, 0x50, 0x74 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
BOOL CControlWnd::Create(CWnd *pParent, void *param)
{
	m_pParent = pParent;
	SetParam((_param*)param);

	const BOOL bResult{};
	
	return bResult;
}

void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	m_sRoot = Variant(m_pParent, homeCC, "");

	DebugString(_T("SetParam"), _T("ROOT"), m_sRoot);
}

CString CControlWnd::Variant(const CWnd* pWizard, int comm, CString data)
{
	if (!pWizard) 
		return _T("");
	CString retvalue;

	const char* dta = (char*)pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return _T("");

	return retvalue;
}

long CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			const _extTH	*extTH = (_extTH*)lParam;
			const char *pExData = (char*)(lParam + L_extTH);
		}
		break;
	}

	return 0;
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
}

void CControlWnd::_ConnectServer(LPCTSTR strGetPost, LPCTSTR strUrl, LPCTSTR strExt, LPCTSTR strPort)
{
	m_strBody = _T("");
	m_strFileBody.clear();
	m_bOverFileSize = false;
	m_sGetPost = strGetPost;
	m_sExt = strExt;
	m_sPort = strPort;
	m_sUrl = strUrl;

	DebugString(_T("ConnectServer"), _T("GetPost"), m_sGetPost, _T("URL"), m_sUrl, _T("EXT"), m_sExt, _T("PORT"), m_sPort);
}

std::string CControlWnd::lpctstrToString(LPCTSTR lpctstr) 
{
#ifdef UNICODE
	// 유니코드 환경 (LPCTSTR이 LPCWSTR)
	int len = WideCharToMultiByte(CP_UTF8, 0, lpctstr, -1, NULL, 0, NULL, NULL);
	if (len <= 0) {
		return "";
	}
	char* buffer = new char[len];
	WideCharToMultiByte(CP_UTF8, 0, lpctstr, -1, buffer, len, NULL, NULL);
	std::string str(buffer);
	delete[] buffer;
	return str;
#else
	// 멀티바이트 환경 (LPCTSTR이 LPCSTR)
	return std::string(lpctstr);
#endif
}

BSTR CControlWnd::_ConvertUtf8(LPCTSTR strValue)
{
	CString sRecv = _T("");

	sRecv = ansi_to_utf8(strValue).c_str();		// utf8 변환

	_Base64Encode((LPSTR)(LPCTSTR)sRecv);		// Base64Encode	
	charStarToHex(sRecv);				// Hex 		

	return sRecv.AllocSysString();
}

BSTR CControlWnd::_ConvertUtf8Len(LPCTSTR strValue, short nLen)
{
	CString sRecv = _T("");
	CString sTmp = _T("");

	std::string spTmp(strValue);

	sRecv = ansi_to_utf8(strValue).c_str();		// utf8 변환
	sTmp = ansi_to_utf8(strValue).c_str();

	_Base64Encode((LPSTR)(LPCTSTR)sTmp);		// Base64Encode	
	charStarToHex(sTmp);				// Hex 		

	return sRecv.AllocSysString();
}

BSTR CControlWnd::_ConvertAnsi(LPCTSTR strValue)
{
	CString sRecv = _T("");
	CString sTmp = _T("");

	sRecv = utf8_to_ansi(strValue).c_str();
	sTmp = sRecv;

	charStarToHex((LPSTR)(LPCTSTR)sTmp);

	return sRecv.AllocSysString();
}

BSTR CControlWnd::_Base64Encode(LPCTSTR strValue)
{
	CString sRecv = _T("");
	std::string sBase64Encode;
	
	sBase64Encode = base64_encode(strValue);	// Base64Encode 변환
	std::cout << "Encoded: " << sBase64Encode << std::endl;

	sRecv = sBase64Encode.c_str();

	_Base64Decode((LPSTR)(LPCTSTR)sRecv);		// Base64Decode

	return sRecv.AllocSysString();
}

BSTR CControlWnd::_Base64Decode(LPCTSTR strValue)
{
	CString sRecv = _T("");
	CString sTmp = _T("");
	std::string sBase64Decode;

	try
	{
		sBase64Decode = base64_decode(strValue);	// Base64Decode 변환
		std::cout << "Decoded: " << sBase64Decode << std::endl;
	}
	catch (const std::runtime_error & e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	sRecv = sBase64Decode.c_str();

	sTmp = sRecv;

	utf8_to_ansi((LPSTR)(LPCTSTR)sTmp);

	return sRecv.AllocSysString();
}

BSTR CControlWnd::_SendHttps()
{
	DWORD dwError = 0;
	CString sRecv = _T("");

	dwError = HttpsSend(sRecv);

	if (dwError == 201 || dwError == 0)
	{

		CString xx;
		xx.Format(_T("%d"), dwError);
		DebugString(_T("SendHttps_pos1"), _T("SuccesMsg"), xx);

	}
	else
	{
		//에러메시지 작성후 리턴.
		LPVOID lpMsgBuf{};
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)& lpMsgBuf, 0, NULL);


		CString Msg = lpMsgBuf == 0 ? ErrorOut(dwError) : (char*)lpMsgBuf;
		char buffer[20];

		_itoa_s(dwError, buffer, _countof(buffer), 10);

		Msg = buffer;

		LocalFree(lpMsgBuf);

		DebugString(_T("SendHttps_pos2"), _T("ErrorMsg"), Msg);

		return sRecv.AllocSysString();;
	}

	return sRecv.AllocSysString();
}

long CControlWnd::_SetHttpsInfo(LPCTSTR strEmKind, LPCTSTR strEmName, LPCTSTR strEmData)
{
	CString nKind = strEmKind;
	std::string strBody;

	switch (atoi(nKind))
	{
	case HTTPS_ID:
	case HTTPS_USERID:
	case HTTPS_DEPTCD:
	case HTTPS_SENDER:
	case HTTPS_SENDDM:
		break;
	case HTTPS_RELIST:
		strBody = MakeListFormatData(strEmName, strEmData);
		//m_strBody += CSTRING(strBody);
		m_strBody += strBody.c_str();
		break;
	case HTTPS_ATTACH:
		strBody = MakeEmFileData(strEmName, strEmData);

		if (strBody.empty())
			return -1;

		m_strFileBody += strBody;
		break;
	case HTTPS_TEXTDATA:
		strBody = MakeEmTextData(strEmName, strEmData);
		//m_strBody += CSTRING(strBody);
		m_strBody += strBody.c_str();
		break;
	case HTTPS_DEFAULT:
	case HTTPS_RENMLIST:
	case HTTPS_SUBJ:
	case HTTPS_CONT:
	default:
		switch (m_nSendModeKind)
		{
		case HTTPS_FORMHEADER:
			strBody = MakeFormData(strEmName, strEmData);
			break;
		case HTTPS_JSONHEADER:
			strBody = MakeJsonData(strEmName, strEmData);
			break;
		case HTTPS_MULTIHEADER:
		default:
			strBody = MakeEmData(strEmName, strEmData);
			break;
		}

		//m_strBody += CSTRING(strBody);
		m_strBody += strBody.c_str();
		break;
	}

	return 0;
}

std::string CControlWnd::MakeEmFileData(CString strEmName, CString strEmData)
{
	CString strEm;
	CString strPath;
	std::string strBody;
	std::string sTmp;
	std::string strData;
	std::string MULTIPART_BOUNDARY = _T("--------------------------411150112343180142269133");

	int nIdx = strEmData.ReverseFind('\\');

	if (nIdx != -1)
		sTmp = strEmData.Right(strEmData.GetLength() - (nIdx + 1));

	strEm.Format(_T("name=\"%s\";"), strEmName);
	strPath.Format(_T("\"%s\""), ansi_to_utf8(sTmp).c_str());

	sTmp = ReadFile(strEmData);

	if (sTmp.empty())
	{
		AfxMessageBox(_T("파일을 읽지 못했습니다."), MB_OK);
		return "";
	}

	strData = sTmp;

	strBody = std::string(_T("--"));
	strBody += MULTIPART_BOUNDARY;
	strBody += std::string(_T("\r\n"));
	strBody += std::string(_T("Content-Disposition: form-data; "));
	strBody += std::string(strEm);
	strBody += std::string(_T("filename="));
	strBody += std::string(strPath);
	strBody += std::string(_T("\r\n"));
	strBody += std::string(_T("Content-Type: application/octet-stream"));
	strBody += std::string(_T("\r\n\r\n"));
	strBody += strData;
	strBody += std::string(_T("\r\n"));

	WriteFile(_T("MakeEmFileData"), strBody.c_str(), strBody.size());
	DebugString(_T("MakeEmFileData"), _T("emName"), strEmName, _T("emData"), strEmData, _T("strBody"), strBody.c_str());

	return strBody;
}

CString CControlWnd::MakeEmTextData(CString strEmName, CString strEmData)
{
	CString strBody = _T("");
	CString strEm = _T("");
	CString MULTIPART_BOUNDARY = _T("--------------------------411150112343180142269133");

	DebugString(_T("MakeEmTextData"), _T("emName"), strEmName, _T("emData"), strEmData);

	strEm.Format(_T("name=\"%s\""), strEmName);

	strBody = _T("--");
	strBody += MULTIPART_BOUNDARY;
	strBody += _T("\r\n");
	strBody += _T("Content-Disposition: form-data; ");
	strBody += strEm;
	strBody += _T("\r\n");
	strBody += "Content-Type: application/octet-stream";
	strBody += _T("\r\n\r\n");
	strBody += strEmData;
	strBody += _T("\r\n");

	return strBody;
}

CString CControlWnd::MakeEmData(CString strEmName, CString strEmData)
{
	CString strBody = _T("");
	CString strEm = _T("");
	CString sTmp = _T("");
	CString MULTIPART_BOUNDARY = _T("--------------------------411150112343180142269133");

	DebugString(_T("MakeEmData"), _T("emName"), strEmName, _T("emData"), strEmData);

	strEm.Format(_T("name=\"%s\""), strEmName);

	strBody = _T("--");
	strBody += MULTIPART_BOUNDARY;

	strBody += _T("\r\n");
	strBody += _T("Content-Disposition: form-data; ");
	strBody += strEm;

	strBody += _T("\r\n\r\n");
	strBody += strEmData;

	strBody += _T("\r\n");

	return strBody;
}

CString CControlWnd::MakeFormData(CString strEmName, CString strEmData)
{
	CString strBody = _T("");
	std::string sTmp(strEmData);

	DebugString(_T("MakeFormData"), _T("emName"), strEmName, _T("emData"), strEmData);

	strBody += _T("\n");
	strBody += ansi_to_utf8(sTmp).c_str();

	strBody += _T("\n");

	return strBody;
}

CString CControlWnd::MakeListFormatData(CString strEmList, CString strEmData)
{
	CString strBody = _T("");
	CString strTmp{};

	DebugString(_T("MakeListFormatData"), _T("emList"), strEmList, _T("emData"), strEmData);

	strTmp.Format(_T("%s\":["), strEmList);
	strBody = _T("\"");
	strBody += strTmp;
	strBody += _T("\n");

	while (!strEmData.IsEmpty())
	{
		strTmp = Parser(strEmData, ",");
		strBody += _T("\"");
		strBody += strTmp;
		strBody += _T("\"");
		strBody += _T("\n");
	}

	strBody += _T("],");

	WriteFile(_T("ListData : \n"), (LPSTR)(LPCTSTR)strBody, strBody.GetLength(), _T("log"));

	return strBody;
}

CString CControlWnd::MakeJsonData(CString strEmName, CString strEmData)
{
	CString strBody = _T("");
	std::string sTmp(strEmData);

	DebugString(_T("MakeJsonData"), _T("emName"), strEmName, _T("emData"), strEmData);

	strBody += _T("\n");
	strBody += ansi_to_utf8(sTmp).c_str();

	strBody += _T("\n");
	return strBody;
}

std::string CControlWnd::ReadFile(CString sPath)
{
	CFile file;
	CFileException e;

	CString xx;
	DebugString(_T("ReadFile_POS1"), _T("path"), sPath);

	if (!file.Open(sPath, CFile::modeRead | CFile::shareDenyNone, &e))
	{
		TRACE("[READ] %s file open error(%d)\n", sPath, e.m_cause);
		return "";
	}

	const UINT nLen = (int)file.GetLength();

	xx.Format("%d", nLen);
	DebugString(_T("ReadFile_POS2"), _T("Len"), xx);

	if (nLen <= 0)
		return _T("");

	if (nLen > 100000000)
	{
		m_bOverFileSize = true;
		return _T("");
	}

	std::unique_ptr<char[]> pBuffer = std::make_unique<char[]>(nLen);
	const UINT nByteRead = file.Read(pBuffer.get(), nLen);

	if (nByteRead != UINT(nLen))
	{
		file.Close();
		return _T("");
	}

	std::string strResult(pBuffer.get(), nLen);
	file.Close();

	return strResult;
}
void CControlWnd::_SetHttpsSendMode(LPCTSTR sKind)
{
	m_nSendModeKind = atoi(sKind);

	DebugString(_T("SetHttpsSendMode"), _T("SendMode"), sKind);
}

CString CControlWnd::MadeEmBody()
{
	CString strBody = _T("");
	CString sTmp = _T("");
	CString MULTIPART_BOUNDARY = _T("--------------------------411150112343180142269133");

	strBody = _T("--");
	strBody += MULTIPART_BOUNDARY;
	strBody += _T("--");
	strBody += _T("\r\n");

	return strBody;
}

void CControlWnd::ClearHttps()
{
	m_strBody = _T("");
	m_strFileBody.clear();
	m_bOverFileSize = false;
}

CString CControlWnd::MakeHeader()
{
	CString aHeader{};

	switch (m_nSendModeKind)
	{
	case HTTPS_FORMHEADER:
		aHeader += _T("Content-type: application/x-www-form-urlencoded;");
		aHeader += _T("charset=UTF-8\r\n");
		aHeader += _T("Accept: application/x-www-form-urlencoded\r\n");
		break;
	case HTTPS_JSONHEADER:
		aHeader += _T("Content-type: application/json;");
		aHeader += _T("charset=UTF-8\r\n");
		aHeader += _T("Accept: application/json\r\n");
		break;
	case HTTPS_MULTIHEADER:
	default:
	{
		CString MULTIPART_BOUNDARY = _T("--------------------------411150112343180142269133;");
	
		aHeader += _T("Content-type: multipart/form-data; boundary=");
		aHeader += MULTIPART_BOUNDARY;
		aHeader += _T("charset=UTF-8\r\n");
		aHeader += _T("Accept: application/json\r\n");
		break;
	}
	}

	aHeader += _T("Cache-Control: no-cache\r\n");
	aHeader += _T("Accept-Language: ko-KR\r\n");
	aHeader += _T("Accept-Encoding: gzip, deflate\r\n");
	aHeader += _T("Connection: keep-alive\n");
	//aHeader += _T("\r\n");

	return aHeader;
}

int CControlWnd::HttpsSend(CString& sRecv)
{
	try
	{
		HINTERNET hSession{}, hConnection{}, hRequest{};
		CString strObject;
		INTERNET_PORT nPort{};
		DWORD dwServiceType = 0;
		DWORD dwFlags = 0;
		DWORD bytesRead;
		int nFlag = 0;
		int nU8StrLen = 0;
		CString sServer{};
		std::string strData(m_strBody);
		CString aHeader;

		aHeader = MakeHeader();

		DebugString(_T("HttpsSend_POS1"), _T("HEADER"), aHeader);

		if (m_nSendModeKind == HTTPS_MULTIHEADER)
		{
			strData += m_strFileBody;
			strData += MadeEmBody();
		}

		nU8StrLen = m_strBody.GetLength();
		if (m_nSendModeKind == HTTPS_MULTIHEADER)
		{
			nU8StrLen += m_strFileBody.length();
			nU8StrLen += MadeEmBody().GetLength();
		}

		CString xx;
		xx.Format("%d", nU8StrLen);
		DebugString(_T("HttpsSend_POS2"), _T("Len"), xx, _T("data"), CString(strData.c_str()));

		/*CString strServerName, sSubPath, sData;
		DWORD dwError;
		strServerName = _T("edev.imfnsec.com");
		sSubPath = _T("/family/simple_login.jsp?userId=070122&userPswd=5Ad|s6pq");

		CString strUrl;
		strUrl = _T("https://edev.imfnsec.com/family/simple_login.jsp?userId=070122&userPswd=5Ad|s6pq");
		sData = _T("HTTPS_TEST");
		dwError = HttpsSend(_T("GET"), strUrl, sData, sRecv);*/

		// sURL에서 서버 주소와 나머지 정보들을 얻는다. http와 https가 아닌 경우에는 리턴
		if (!AfxParseURL(m_sUrl, dwServiceType, sServer, strObject, nPort) || (dwServiceType != AFX_INET_SERVICE_HTTP && dwServiceType != AFX_INET_SERVICE_HTTPS))
		{
			DebugString(_T("HttpsSend_POS3"), _T("------error_AfxParseURL--------"));

			sRecv = _T("error_AfxParseURL");
			return GetLastError();
		}

		if (!m_sPort.IsEmpty())
		{
			if (dwServiceType == AFX_INET_SERVICE_HTTPS)
				nPort = atoi(m_sPort);
			else
				nPort = 80;
		}

		//세션 오픈
		hSession = InternetOpen(_T("Request"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (hSession == NULL)
		{
			DebugString(_T("HttpsSend_POS4"), _T("------error_InternetOpen--------"));

			sRecv = _T("error_InternetOpen");
			return GetLastError();
		}

		//xx.Format(_T("[HTTPS] : HttpsSend_POS5 -- server[%s] type[%lu] port[%d]HEX[%x] Ext[%s]\n"), sServer, dwServiceType, nPort, nPort, m_sExt);
		//OutputDebugString(xx);

		//서버 및 포트 방식 설정
		hConnection = InternetConnect(hSession, sServer, nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
		if (hConnection == NULL)
		{
			DebugString(_T("HttpsSend_POS6"), _T("------error_InternetConnect--------"));

			InternetCloseHandle(hSession);
			sRecv = _T("error_InternetConnect");
			return GetLastError();
		}

		//만약 https ssl 통신이 필요하다면 INTERNET_FLAG_SECURE 플래그 추가
		if (dwServiceType == AFX_INET_SERVICE_HTTPS)
		{
			dwFlags = INTERNET_FLAG_SECURE |
				INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_DONT_CACHE |
				INTERNET_FLAG_NO_COOKIES;

			DebugString(_T("HttpsSend_POS7"), _T("------HTTPS_DWORDSET--------"));
		}
		else
		{

			dwFlags = INTERNET_FLAG_RELOAD |
				INTERNET_FLAG_DONT_CACHE |
				INTERNET_FLAG_NO_COOKIES;

			DebugString(_T("HttpsSend_POS8"), _T("------NOTHTTPS_DWORDSET--------"));
		}

		/*
		Method 방식 설정 및 플래그 설정
		GET일 경우 sSubPath에 원하는 인자를 넣어주면 된다.
		POST일 경우에 하위 경로만 넣어주면 된다. 실제 데이터는 HttpSendRequest에서 전송
		*/
		//dwFlags = INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
		hRequest = HttpOpenRequest(hConnection, m_sGetPost, m_sExt, _T("HTTP/1.1"), NULL, NULL, dwFlags, 0);
		if (hRequest == NULL)
		{
			DebugString(_T("HttpsSend_POS9"), _T("------error_HttpOpenRequest--------"));

			InternetCloseHandle(hConnection);
			InternetCloseHandle(hSession);
			sRecv = _T("error_HttpOpenRequest");
			return GetLastError();
		}

		/*
		https를 사용할 경우 SSL 통신을 할 때 인증서 관련 옵션
		테스트 환경에서 개인적으로 만든 인증서 일경우 12045 에러가 발생한다.
		12045 알수 없는 발급기관 에러가 뜰경우에 이 옵션을 설정해 주면 무시하고 통신한다.
		*/
		if (dwServiceType == AFX_INET_SERVICE_HTTPS)
		{
			dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
				SECURITY_FLAG_IGNORE_REVOCATION |
				SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
				SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
				SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
				SECURITY_FLAG_IGNORE_CERT_CN_INVALID;

			DebugString(_T("HttpsSend_POS10"), _T("------HTTPS_DEVICETYPE--------"));

			if (!InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags)))
			{
				DebugString(_T("HttpsSend_POS11"), _T("------error_InternetSetOption--------"));

				InternetCloseHandle(hRequest);
				InternetCloseHandle(hConnection);
				InternetCloseHandle(hSession);
				sRecv = _T("error_InternetSetOption");
				return GetLastError();
			}
		}

		//통신할 헤더 설정
		xx.Format("%d", aHeader.GetLength());
		DebugString(_T("HttpsSend_POS2-1"), _T("HeaderLen"), xx, _T("HeaderData"), aHeader);

		dwFlags = HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD;
		if (!HttpAddRequestHeaders(hRequest, (LPCSTR)(LPCTSTR)aHeader, aHeader.GetLength(), dwFlags))
		{
			DebugString(_T("HttpsSend_POS12"), _T("------error_HttpAddRequestHeaders--------"));

			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hSession);
			sRecv = _T("error_HttpAddRequestHeaders");
			return GetLastError();
		}

		//HttpSendRequest 전까진 실제로 서버에 접속하지 않고 접속 준비만 하는 상태이다.
		//서버 설정이 잘 못 되었거나 ip, port 가 맞지 않을 경우 대부분 여기서 에러가 발생한다.

		//xx.Format(_T("[HTTPS] : --HttpsSend_POS2-2--Datasize[%d] : Data[%s]\n"), strData.length(), strData);
		//OutputDebugString(xx);

		LPVOID pDPtr = (LPVOID)strData.c_str();
		if (!HttpSendRequest(hRequest, (LPCSTR)(LPCTSTR)aHeader, aHeader.GetLength(), pDPtr, nU8StrLen))
		{
			DebugString(_T("HttpsSend_POS13"), _T("------error_HttpSendRequest--------"));

			InternetCloseHandle(hRequest);
			InternetCloseHandle(hConnection);
			InternetCloseHandle(hSession);
			sRecv = _T("error_HttpSendRequest");
			return GetLastError();
		}

		WriteFile(_T("Header : "), (LPSTR)(LPCTSTR)aHeader, aHeader.GetLength(), _T("log"));
		WriteFile(_T("Body : "), strData.c_str(), strData.size(), _T("log"));

		char buffer[4096];
		char buffer2[4096];
		unsigned int jj = 0;

		while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0)
		{
			buffer[bytesRead] = 0;
		}

		std::cout << buffer << endl;

		memset(buffer2, 0x00, sizeof(buffer2));

		for (unsigned int ii = 0; ii < strlen(buffer); ii++)
		{
			buffer2[jj] = buffer[ii];
			jj++;
		}
		std::cout << buffer2;

		CString sTmp(buffer2);
		std::cout << sTmp;
		
		WriteFile(_T("Https Return : "), (LPSTR)(LPCTSTR)sTmp, sTmp.GetLength(), _T("log"));
		DebugString(_T("Https Return"), _T("Recive"), sTmp);
		
		sRecv = utf8_to_ansi((LPSTR)(LPCTSTR)sTmp).c_str();

		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnection);
		InternetCloseHandle(hSession);

		return HttpsDebugRecMsg(sRecv);
	}

	catch (CException & e)
	{
		char   szCause[255];
		CString strFormatted;

		e.GetErrorMessage(szCause, 255);

		sRecv = szCause;

		return 1;
	}

	return NULL;
}

int CControlWnd::HttpsDebugRecMsg(CString sData)
{
	int nPos = -1;
	CString sStatus = _T("0");

	nPos = sData.Find(_T("status"));
	if (nPos != -1)
		sStatus = sData.Mid(nPos + 8, 3);

	DebugString(_T("HttpsDebugRecMsg"), _T("status"), sStatus, _T("Data"), sData);

	return atoi(sStatus);
}

CString CControlWnd::ErrorOut(DWORD dError)
{
	switch (dError)
	{
	case 12001:
		return _T("(Out of handles)");
	case 12002:
		return _T("(Timeout)");
	case 12004:
		return _T("(Internal Error)");
	case 12005:
		return _T("(Invalid URL)");
	case 12006:
		return _T("(Unrecognized Scheme : Please check the URL name");
	case 12007:
		return _T("(Service Name Not Resolved)");
	case 12008:
		return _T("(Protocol Not Found)");
	case 12013:
		return _T("(Incorrect User Name)");
	case 12014:
		return _T("(Incorrect Password)");
	case 12015:
		return _T("(Login Failure)");
	case 12016:
		return _T("(Invalid Operation)");
	case 12017:
		return _T("(Operation Canceled)");
	case 12020:
		return _T("(Not Proxy Request)");
	case 12023:
		return _T("(No Direct Access)");
	case 12026:
		return _T("(Request Pending)");
	case 12027:
		return _T("(Incorrect Format)");
	case 12028:
		return _T("(Item not found)");
	case 12029:
		return _T("(Cannot connect)");
	case 12030:
		return _T("(Connection Aborted)");
	case 12031:
		return _T("(Connection Reset)");
	case 12033:
		return _T("(Invalid Proxy Request)");
	case 12034:
		return _T("(Need UI)");
	case 12035:
		return _T("(Sec Cert Date Invalid)");
	case 12038:
		return _T("(Sec Cert CN Invalid)");
	case 12044:
		return _T("(Client Auth Cert Needed)");
	case 12045:
		return _T("(Invalid CA Cert)");
	case 12046:
		return _T("(Client Auth Not Setup)");
	case 12150:
		return _T("(HTTP Header Not Found)");
	case 12152:
		return _T("(Invalid HTTP Server Response)");
	case 12153:
		return _T("(Invalid HTTP Header)");
	case 120154:
		return _T("(Invalid Query Request)");
	case 120156:
		return _T("(Redirect Failed)");
	case 120159:
		return _T("(TCP/IP not installed)");
	default:
		return _T("UnKnown");
	}
}

CString CControlWnd::AnsiToUTF8RetCString(CString sData)
{
	char*	pszANSI = (LPSTR)(LPCTSTR)sData;
	int	nAStrLen{}, nBStrLen{}, nU8StrLen{};
	BSTR	bstr{};
	char*	pszUTF8 = nullptr;
	CString	stmp, stmps;
	
	nAStrLen = lstrlen(pszANSI);
	nBStrLen = MultiByteToWideChar(CP_ACP, 0, pszANSI, nAStrLen, nullptr, NULL);
	bstr = SysAllocStringLen(nullptr, nBStrLen);
	MultiByteToWideChar(CP_ACP, 0, pszANSI, nAStrLen, bstr, nBStrLen);
	
	nU8StrLen = WideCharToMultiByte(CP_UTF8, 0, bstr, -1, pszUTF8, 0, nullptr, nullptr);
	
	pszUTF8 = new char[nU8StrLen + 1];
	WideCharToMultiByte(CP_UTF8, 0, bstr, -1, pszUTF8, nU8StrLen, nullptr, nullptr);
	
	SysFreeString(bstr);

	stmp = pszUTF8;

	delete [] pszUTF8;

	return stmp;
}

CString CControlWnd::Utf8ToCString(const char* utf8String) 
{
	if (utf8String == nullptr) 
		return CString();

	int utf8Length = strlen(utf8String);
	int requiredSize = MultiByteToWideChar(CP_UTF8, 0, utf8String, utf8Length, nullptr, 0);

	if (requiredSize <= 0) 
		return CString();

	wchar_t* wideCharBuffer = new wchar_t[requiredSize + 1];
	if (MultiByteToWideChar(CP_UTF8, 0, utf8String, utf8Length, wideCharBuffer, requiredSize) <= 0) 
	{
		delete[] wideCharBuffer;
		return CString();
	}

	wideCharBuffer[requiredSize] = L'\0'; // Null terminator 추가
	CString result(wideCharBuffer);

	delete[] wideCharBuffer;

	return result;
}

void CControlWnd::DebugString(CString sFunction, CString sKey1, CString sval1, CString sKey2, CString sval2, CString sKey3, CString sval3, CString sKey4, CString sval4)
{
	if (WRITELOG == 0)
		return;

	CString xx;
	xx.Format(_T("[HTTPS] : [%s] %s:[%s] %s:[%s] %s:[%s] %s:[%s]\n"), 
		sFunction, sKey1, sval1, sKey2, sval2, sKey3, sval3, sKey4, sval4);

	OutputDebugString(xx);
}

void CControlWnd::WriteFile(CString sKey, const char* pBytes, int nBytes, CString sComp)
{
	if (WRITELOG == 0)
		return;

	if (sComp.IsEmpty())
		sComp = _T("Log");

	CFile		Dfile;
	DWORD		pos;
	CString		filename, time;

	filename.Format(_T("%s\\%s.txt"), m_sRoot, sComp);

	BOOL bExist = Dfile.Open(filename, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		Dfile.Open(filename, CFile::modeCreate | CFile::modeReadWrite);
		Dfile.Write(sKey, sKey.GetLength() + 2);
		Dfile.Write(pBytes, nBytes);
		Dfile.Write("\r\n", 2);
	}
	else
	{
		pos = Dfile.SeekToEnd();
		Dfile.Seek((long)pos, CFile::begin);
		Dfile.Write(sKey, sKey.GetLength());
		Dfile.Write(pBytes, nBytes);
		Dfile.Write("\r\n", 2);
	}

	Dfile.Close();
}

std::string CControlWnd::ansi_to_utf8(const std::string& ansi_string)
{
	int buffer_size = MultiByteToWideChar(CP_ACP, 0, ansi_string.c_str(), -1, nullptr, 0);
	std::vector<wchar_t> wide_string(buffer_size);

	MultiByteToWideChar(CP_ACP, 0, ansi_string.c_str(), -1, wide_string.data(), buffer_size);

	buffer_size = WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), -1, nullptr, 0, nullptr, nullptr);
	
	std::string utf8_string(buffer_size, 0);
	
	WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), -1, utf8_string.data(), buffer_size, nullptr, nullptr);
	utf8_string.resize(strlen(utf8_string.c_str())); // Null terminator 제거
	
	WriteFile(_T("ConvertUtf8 : "), utf8_string.c_str(), utf8_string.length(), _T("HEX"));
	DebugString(_T("ConvertUtf8"), _T("Recive"), utf8_string.c_str());

	return utf8_string;
}

std::string CControlWnd::utf8_to_ansi(const std::string& utf8_string)
{
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, nullptr, 0);

	if (wlen == 0)
		return "";

	std::wstring wstr(wlen, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &wstr[0], wlen);

	// 2. UTF-16 → ANSI (e.g., CP949)
	int alen = WideCharToMultiByte(949, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (alen == 0) return "";

	std::string ansiStr(alen, 0);
	WideCharToMultiByte(949, 0, wstr.c_str(), -1, &ansiStr[0], alen, nullptr, nullptr);

	WriteFile(_T("utf8_to_ansi"), ansiStr.c_str(), ansiStr.size());
	DebugString(_T("utf8_to_ansi"), _T("Recive"), ansiStr.c_str());

	return ansiStr;
}

std::string CControlWnd::charStarToHex(const char* str) 
{
	std::string hexString = "";

	while (*str) 
	{
		hexString += toHex(*str);
		str++;
	}

	WriteFile(_T("ConvertHex : "), hexString.c_str(), hexString.length(), _T("HEX"));

	return hexString;
}

std::string CControlWnd::toHex(char c) 
{
	unsigned char uc = (unsigned char)c;
	std::stringstream s;

	s << std::hex << std::setw(2) << std::setfill('0') << (int)uc;

	return s.str();
}

std::string CControlWnd::base64_encode(const std::string& in) 
{
	std::string Base64Encode;
	int val = 0;
	int valb = -6;

	for (unsigned char c : in) 
	{
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) 
		{
			Base64Encode.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) 
	{
		Base64Encode.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	}
	while (Base64Encode.size() % 4)
	{
		Base64Encode.push_back('=');
	}

	WriteFile(_T("Base64encode : "), Base64Encode.c_str(), Base64Encode.length(), _T("HEX"));

	return Base64Encode;
}

/*int main() {
	std::string data = "안녕하세요.";
	std::string encoded_data = base64_encode(data);
	std::cout << "Encoded: " << encoded_data << std::endl;

	return 0;
}*/

std::string CControlWnd::base64_decode(const std::string& encoded_string) 
{
	int padding = 0;

	if (encoded_string.length() > 0 && encoded_string[encoded_string.length() - 1] == '=')
	{
		padding++;
	}
	if (encoded_string.length() > 1 && encoded_string[encoded_string.length() - 2] == '=') 
	{
		padding++;
	}

	// Calculate the expected decoded size
	size_t decoded_size = (encoded_string.length() * 3) / 4 - padding;
	std::string Base64decoded;
	Base64decoded.reserve(decoded_size);

	unsigned int val = 0;
	int val_bits = 0;

	for (char c : encoded_string) 
	{
		int char_value = base64_chars_to_value[static_cast<unsigned char>(c)];
		if (char_value == -1) 
		{ // Invalid Base64 character or padding
			if (c == '=') continue; // Skip padding characters for now
			throw std::runtime_error("Invalid Base64 character encountered.");
		}

		val = (val << 6) | char_value;
		val_bits += 6;

		if (val_bits >= 8) 
		{
			val_bits -= 8;
			Base64decoded += static_cast<char>((val >> val_bits) & 0xFF);
		}
	}

	WriteFile(_T("Base64decoded : "), Base64decoded.c_str(), Base64decoded.length(), _T("HEX"));

	return Base64decoded;
}

CString CControlWnd::Parser(CString& srValue, CString sGubn)
{
	CString tmps;
	const int pos = srValue.Find(sGubn);

	if (pos == -1)
	{
		tmps = srValue;
		srValue.Empty();
		return tmps;
	}

	tmps = srValue.Left(pos);
	srValue = srValue.Mid(pos + _tcslen(sGubn));

	return tmps;
}