// AxCommon.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"
#include "AxCommon.h"
#include <stdarg.h>

#include "ImgButton.h"
#include "ImgHeaderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDC_HAND MAKEINTRESOURCE(32649)
#ifdef DF_USESTL_COMMON
using MBITMAP = std::tuple<std::shared_ptr<CBitmap>, BOOL, BOOL>;
#endif



AXIS_EXT_DATA CString	Axis::home;
AXIS_EXT_DATA CString	Axis::user;
AXIS_EXT_DATA CString	Axis::userNM;
AXIS_EXT_DATA CString	Axis::userID;
AXIS_EXT_DATA CString	Axis::userIP;
AXIS_EXT_DATA CString	Axis::skinName;
AXIS_EXT_DATA CBitmap	Axis::bmpBandTerm;
AXIS_EXT_DATA COLORREF	Axis::dialogBgColor;
AXIS_EXT_DATA CBrush	Axis::dialogBgBrush;
AXIS_EXT_DATA int		Axis::interFrame;
AXIS_EXT_DATA CSize		Axis::reviseFrameSize;
AXIS_EXT_DATA HCURSOR   Axis::handCursor;
AXIS_EXT_DATA BOOL		Axis::isCustomer;
AXIS_EXT_DATA CWnd*		Axis::mainWnd;
AXIS_EXT_DATA BOOL		Axis::devMode;
AXIS_EXT_DATA CFont		Axis::fondBold;
AXIS_EXT_DATA CFont		Axis::font;
AXIS_EXT_DATA BOOL		Axis::isVista;
AXIS_EXT_DATA int		Axis::WinTheme;
AXIS_EXT_DATA CString		Axis::sFiller;
#ifdef DF_USESTL_COMMON
AXIS_EXT_DATA std::map<CString, MBITMAP> _mbitmaps;
#else
AXIS_EXT_DATA CMap<CString, LPCTSTR, bitmap_res, bitmap_res> Axis::m_bitmaps;
#endif
AXIS_EXT_DATA CMap<CString, LPCTSTR, COLORREF, COLORREF> Axis::m_colors;
AXIS_EXT_DATA CArray<int, int>			Axis::m_newsKey;
AXIS_EXT_DATA CArray<LPCTSTR, LPCTSTR>	Axis::m_newsName;

AXIS_EXT_DATA COLORREF	Axis::maskColor;

//** profile sections
AXIS_EXT_DATA const char* szLastStat		= "LASTSTAT";
AXIS_EXT_DATA const char* szStartMap		= "STARTVSMAP";
AXIS_EXT_DATA const char* szRoot			= "ROOT";
AXIS_EXT_DATA const char* szETC				= "ETC";
AXIS_EXT_DATA const char* szScreen			= "SCREEN";
AXIS_EXT_DATA const char* szMessage			= "MESSAGE";
AXIS_EXT_DATA const char* szBkNotice		= "BKNOTICE";
AXIS_EXT_DATA const char* szSystem			= "SYSTEM";
AXIS_EXT_DATA const char* szTabView			= "TabView";
AXIS_EXT_DATA const char* szCommon			= "Common";
AXIS_EXT_DATA const char* szUserSN			= "UserBar";
AXIS_EXT_DATA const char* szUserEN			= "Current";
AXIS_EXT_DATA const char* szGeneral			= "General";
AXIS_EXT_DATA const char* szWebLink			= "WebLink";
AXIS_EXT_DATA const char* szSecure			= "Secure";
AXIS_EXT_DATA const char* szUserTool		= "USERTOOL";
AXIS_EXT_DATA const char* szSelect			= "SELECT";
AXIS_EXT_DATA const char* szShowBar			= "SHOWBAR";
AXIS_EXT_DATA const char* szFire			= "FIRE";
AXIS_EXT_DATA const char* szLogCheck		= "LOGCHECK";
AXIS_EXT_DATA const char* szDTAccouncement	= "DT_accouncement";
AXIS_EXT_DATA const char* szDTKeyboard		= "DT_KEYBOARD";
AXIS_EXT_DATA const char* szAgreeDuplicate	= "AgreeDuplicate";
AXIS_EXT_DATA const char* szHKey			= "HKEY";
AXIS_EXT_DATA const char* szToolFunc		= "ToolFunc";
AXIS_EXT_DATA const char* szSName			= "SNAME";
AXIS_EXT_DATA const char* szPalette			= "Palette";
AXIS_EXT_DATA const char* szPalettes		= "Palettes";
AXIS_EXT_DATA const char* szAux				= "Aux";

//** site constants
AXIS_EXT_DATA const char* szSite			= "IBK투자증권";

CString CAxisCommon::Variant(int comm, const char* data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_wnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

CPen* CAxisCommon::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_wnd->SendMessage(WM_USER, getPEN, (long)&penR);
}

CPen* CAxisCommon::GetAxPen(struct _penR* pen)
{
	return GetAxPen(pen->clr, pen->width, pen->style);	
}

CFont* CAxisCommon::GetAxFont(const char* fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName;
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_wnd->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CFont* CAxisCommon::GetAxFont(struct _fontR* font)
{
	return GetAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

CBrush* CAxisCommon::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_wnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}

COLORREF CAxisCommon::GetAxColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_wnd->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CBitmap* CAxisCommon::GetAxBitmap(LPCTSTR lpszBitmapFileName)
{
	return (CBitmap*)m_wnd->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), 
		(LPARAM)(LPCTSTR)Format("%simage\\%s", m_home, lpszBitmapFileName));
}

CString CAxisCommon::GetUser()
{
	CString user;
	user = Variant(nameCC);

	return user;
}

int CAxisCommon::GetCodeType(const char* code)
{
	const int codeL = strlen(code);
	if (codeL <= 0) return 0;

	const int result = m_wnd->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)code);

	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		if (code[0] == '1' || code[0] == '4')
			return	futureCODE;		// 선물
		else
			return  optionCODE;		// 옵션
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// 업종
		return	indexCODE;
	case thirdType:				// 제3시장
		return	thirdCODE;
	case sinjuType:				// 신주인수권
		return	sinjuCODE;
	case mufundType:			// 뮤츄얼펀드
	case reitsType:				// 리츠
	case hyfundType:			// 하이일드펀드
	default:
		break;
	}

	return 0;
}

LRESULT CAxisCommon::SendMessage(WPARAM wParam, LPARAM lParam)
{
	return m_wnd->SendMessage(WM_USER, wParam, lParam);
}

LRESULT CAxisCommon::SendMessage2(WPARAM wParam, LPARAM lParam)
{
	return m_inter->SendMessage(WM_USER + 24, wParam, lParam);
}

CString CAxisCommon::GetItemName(const char* code)
{
	CString name = (char*)SendMessage(MAKEWPARAM(nameDLL, 0), (LPARAM)code);
	Trim(name);
	return name;
}

void CAxisCommon::GetLogFont(LOGFONT* lf, LOGFONT* lfB)
{
	m_font->GetLogFont(lf);
	if (lfB)
		m_fontBold->GetLogFont(lfB);
}

CWnd* CAxisCommon::GetView()
{
	return m_wnd;
}

// CProfile
CProfile::CProfile(const char* fileName)
{
	SetFileName(fileName);
}

void CProfile::SetFileName(const char* fileName)
{
	m_fileName = fileName;
}

CProfile::CProfile(ProfileKind pk)
{
	SetFileName(pk);
}

void CProfile::SetFileName(ProfileKind pk)
{
	m_fileName = GetProfileFileName(pk);
}

void CProfile::Write(const char* section, const char* item, const char* value)
{
	WritePrivateProfileString(section, item, value, m_fileName);
}

void CProfile::Write(const char* section, const char* item, int value)
{
	Write(section, item, Format("%d", value));
}

void CProfile::Write(const char* section, const char* item, double value)
{
	Write(section, item, Format("%f", value));
}

CString CProfile::GetString(const char* section, const char* item, const char* defValue)
{
	//char buffer[MAX_PATH];

	//GetPrivateProfileString(section, item, defValue, buffer, MAX_PATH, m_fileName);
	char buffer[2048];
	
	GetPrivateProfileString(section, item, defValue, buffer, 2048, m_fileName);

	return CString(buffer);
}

int CProfile::GetInt(const char* section, const char* item, int defValue)
{
	return GetPrivateProfileInt(section, item, defValue, m_fileName);
}

double CProfile::GetDouble(const char* section, const char* item, double defValue)
{
	return atof(GetString(section, item, Format("%f", defValue)));
}

/**
CString CProfile::GetSection(const char* section)
{
	char buffer[1024 * 32];

	int len = GetPrivateProfileSection(section, buffer, sizeof(buffer), m_fileName);
	return CString(buffer, len);
}
이 함수는 호환성 문제로 삭제함. (2008.07.25 macho)
**/

CString CProfile::GetSectionLF(const char* section)
{
	std::unique_ptr<CFile> pFile;
	TRY
	{
		pFile = std::make_unique<CFile>(m_fileName, CFile::modeRead);
	}
		CATCH(CFileException, e)
	{
		return CString("");
	}
	END_CATCH;

	pFile->SeekToBegin();

	int len = (int)pFile->GetLength();
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(len + 1);
	
	pFile->Read(buffer.get(), len);
	buffer.get()[len] = 0x00;

	CString str((char*)buffer.get());
	CString strUpper(str);
	strUpper.MakeUpper();

	CString key(Format("[%s]", section));
	key.MakeUpper();

	int find = strUpper.Find(key);
	if (find < 0)
		return CString("");

	if (find == 0 || strUpper.GetAt(find - 1) == '\n')
	{
		const int firstLine = strUpper.Find('\n', find + 1);
		if (firstLine <= find)
			return CString("");

		find = firstLine;
		int findLast = len - 1;
		int nextFind = find;

		while (TRUE)
		{
			nextFind = strUpper.Find('[', nextFind + 1);
			if (nextFind < 0)
				break;

			if (strUpper.GetAt(nextFind - 1) == '\n')
			{
				findLast = nextFind - 1;
				break;
			}
		}
		
		CString result = str.Mid(find, findLast - find);

		Trim(result);
		result += '\n';
		return result;
	}

	return CString("");
	//**/
}

void CProfile::WriteSection(const char* section, const char * data)
{
	WritePrivateProfileSection(section, data, m_fileName);
}

CString Format(const char* formatString, ...)
{
	CString	s;

	va_list	valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);

	return s;
}

void FormatCopy(void* buffer, const char* formatString, ...)
{
	CString s;

	va_list valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);

	memcpy(buffer, (const char*)s, s.GetLength());
}

CString Parse(CString &src, const char* delim)
{
	CString s;
	const int find = src.Find(delim);

	if (find == -1)
	{
		s = src;
		src.Empty();
		return s;
	}
	else 
	{
		s = src.Left(find);
		src = src.Mid(find + lstrlen(delim));
		return s;
	}
}

BOOL IsFileExist(const char* fileName)
{
	CFileFind ff;

	if (ff.FindFile(fileName))
	{
		ff.Close();
		return TRUE;
	}
	else
		return FALSE;
}

CString NormalizeFolderName(CString& folder)
{
	if (folder.IsEmpty())
		return folder;

	const char dirDelimeter = '\\';

	if (folder[folder.GetLength() - 1] != dirDelimeter)
		folder += dirDelimeter;
	return folder;
}


CString GetPasswordChar(const char* szPassword)
{
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(strlen(szPassword) + 1);
	memset(buffer.get(), '*', strlen(szPassword));
	buffer.get()[strlen(szPassword)] = 0x00;
	CString result((char*)buffer.get());
	return result;
}

const char delim = '|';

CString EncodeAcctName(const char* name, const char* alias)
{
	CString s(name);
	if (alias && strlen(alias))
	{
		s += delim;
		s += alias;
	}

	return s;
}

void DecodeAcctName(const char* encodedName, CString& name, CString& alias)
{
	CString s(encodedName);
	int find(s.Find(delim));

	if (find > 0)
	{
		name = s.Left(find);
		alias = s.Right(s.GetLength() - (find + 1));

		find = alias.Find(delim);
		if (find >= 0)
			alias = alias.Left(find);
	}
	else
	{
		name = encodedName;
		alias.Empty();
	}
}

CString EncodeAcctNo(const char* szAcct)
{
	CString acct(szAcct);
	if (acct.GetLength() < LEN_ACCT)
		return CString("");

	CString s;
	s.Format("%s-%s-%s", acct.Left(acctLeft), acct.Mid(acctLeft, acctMid), 
		acct.Mid(acctLeft + acctMid, acctRight));

	return s;
}

CString DecodeAcctNo(const char* szAcct)
{
	CString acct(szAcct);
	acct.Replace("-", "");

	if (acct.GetLength() >= LEN_ACCT)
		return acct.Left(LEN_ACCT);
	else
		return CString("");
}


#define MMI_MAXSIZE			0x01
#define MMI_MAXPOSITION		0x02
#define MMI_MINTRACKSIZE	0x04
#define MMI_MAXTRACKSIZE	0x08


void ConditionCopyMinMaxInfo(MINMAXINFO* src, MINMAXINFO* dest)
{
	if ((src->ptReserved.x & MMI_MAXSIZE) == MMI_MAXSIZE)
		dest->ptMaxSize.x = src->ptMaxSize.x;
	if ((src->ptReserved.x & MMI_MAXPOSITION) == MMI_MAXPOSITION)
		dest->ptMaxPosition.x = src->ptMaxPosition.x;
	if ((src->ptReserved.x & MMI_MINTRACKSIZE) == MMI_MINTRACKSIZE)
		dest->ptMinTrackSize.x = src->ptMinTrackSize.x;
	if ((src->ptReserved.x & MMI_MAXTRACKSIZE) == MMI_MAXTRACKSIZE)
		dest->ptMaxTrackSize.x = src->ptMaxTrackSize.x;

	if ((src->ptReserved.y & MMI_MAXSIZE) == MMI_MAXSIZE)
		dest->ptMaxSize.y = src->ptMaxSize.y;
	if ((src->ptReserved.y & MMI_MAXPOSITION) == MMI_MAXPOSITION)
		dest->ptMaxPosition.y = src->ptMaxPosition.y;
	if ((src->ptReserved.y & MMI_MINTRACKSIZE) == MMI_MINTRACKSIZE)
		dest->ptMinTrackSize.y = src->ptMinTrackSize.y;
	if ((src->ptReserved.y & MMI_MAXTRACKSIZE) == MMI_MAXTRACKSIZE)
		dest->ptMaxTrackSize.y = src->ptMaxTrackSize.y;

	if (dest->ptMinTrackSize.x < MMI_MINWIDTH)
		dest->ptMinTrackSize.x = MMI_MINWIDTH;
	if (dest->ptMinTrackSize.y < MMI_MINHEIGHT)
		dest->ptMinTrackSize.y = MMI_MINHEIGHT;
}


CFileBitmap::CFileBitmap(const char* home, const char* fileName)
: CBitmap()
{
	CString file(fileName);
	file.MakeUpper();
	file.Replace(".BMP", "");
	Open(Format("%s\\image\\%s.bmp", home, file));
}


CFileBitmap::CFileBitmap(const char* filePath)
{
	Open(filePath);
}

CFileBitmap::~CFileBitmap()
{
	Close();
}

void CFileBitmap::Open(const char* filePath)
{
	Close();
	HBITMAP bmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filePath,
								IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (bmp)
	{
		Attach(bmp);
	}
}

void CFileBitmap::Close()
{
	if (m_hObject)
	{
		::DeleteObject(Detach());
	}
}

CSize CFileBitmap::GetSize()
{
	if (!m_hObject)
		return CSize(0, 0);

	BITMAP bm;

	if (GetBitmap(&bm))
		return CSize(bm.bmWidth, bm.bmHeight);
	else
		return CSize(0, 0);
}

//** getting profile filename each kind
CString GetProfileFileName(ProfileKind pk)
{
	const char* szTabDir = TABDIR;
	const char* szUserDir = USRDIR;
	
	switch (pk)
	{
	case pkAxis: 
		return Format("%s\\%s\\axis.ini", Axis::home, szTabDir);
	case pkAxisMenu:
		return Format("%s\\%s\\%s", Axis::home, szTabDir, AXISMENU);
	case pkAxisUser:
		return Format("%s\\%s\\axisuser.ini", Axis::home, szTabDir);
	case pkAxisTab:
		return Format("%s\\%s\\axistab.ini", Axis::home, szTabDir);
	case pkAxisTicker:
		return Format("%s\\%s\\%s\\axisticker.ini", Axis::home, szUserDir, Axis::user);
	case pkPalette:
		return Format("%s\\%s\\%s", Axis::home, szTabDir, PALETTE);
	case pkVTCode:
		return Format("%s\\%s\\vtcode.ini", Axis::home, szTabDir);
	case pkEnvironment:
		return Format("%s\\%s\\%s\\%s", Axis::home, szUserDir, Axis::user, "axisensetup.ini");
	case pkUserSetup:
		return Format("%s\\%s\\%s\\%s.ini", Axis::home, szUserDir, Axis::user, Axis::user);
	case pkUserConfig:
		return Format("%s\\user\\%s\\userconf.ini", Axis::home, Axis::user);
	case pkLink:
		return Format("%s\\%s\\link.ini", Axis::home, szTabDir);
	case pkLinkBar:
		return Format("%s\\%s\\linkbar.ini", Axis::home, szTabDir);
	case pkLinkName:
		return Format("%s\\%s\\linkname.ini", Axis::home, szTabDir);
	case pkManageSetup:
		return Format("%s\\%s\\%s\\%s", Axis::home, szUserDir, Axis::user, "mngsetup.ini");
	case pkOrderSetup:
		return Format("%s\\%s\\%s\\%s", Axis::home, szUserDir, Axis::user, "ORDCFG.ini");
	case pkUserTool:
		return Format("%s\\%s\\%s\\usertool.ini", Axis::home, szUserDir, Axis::user);
	case pkScreenReg:
		return Format("%s\\%s\\%s\\%s", Axis::home, szUserDir, Axis::user, "screenreg.ini");
	case pkUserProgram:
		return Format("%s\\%s\\%s\\userprogram.ini", Axis::home, szUserDir, Axis::user);
	case pkFirewall:
		return Format("%s\\%s\\%s\\firewall.ini", Axis::home, szUserDir, Axis::user);
	case pkSTInfo:
		return Format("%s\\mtbl\\st_info.ini", Axis::home);
	case pkYellowPage:
		return Format("%s\\%s\\yellowPage.ini", Axis::home, szTabDir);
	case pkHotKey:
		return Format("%s\\%s\\%s\\hkey.ini", Axis::home, szUserDir, Axis::user);
	case pkPortfolio:
		return Format("%s\\user\\%s\\portfolio.ini", Axis::home, Axis::user);
	}

	return "";
}


//** Axis static functions
typedef BOOL(WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);

BOOL GetVersion(OSVERSIONINFOEX* os) {
	HMODULE hMod{};
	RtlGetVersion_FUNC func{};

	OSVERSIONINFOEXW o{};
	OSVERSIONINFOEXW* osw = &o;

	hMod = LoadLibrary(TEXT("ntdll.dll"));
	if (hMod)
	{
		func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
		if (func == 0)
		{
			FreeLibrary(hMod);
			return FALSE;
		}
		ZeroMemory(osw, sizeof(*osw));
		osw->dwOSVersionInfoSize = sizeof(*osw);
		func(osw);

		os->dwBuildNumber = osw->dwBuildNumber;
		os->dwMajorVersion = osw->dwMajorVersion;
		os->dwMinorVersion = osw->dwMinorVersion;
		os->dwPlatformId = osw->dwPlatformId;
		os->dwOSVersionInfoSize = sizeof(*os);
	}
	else
		return FALSE;
	FreeLibrary(hMod);
	return  TRUE;
}

void Axis::Initialize()
{
	Axis::dialogBgColor = 0x00D2C5BC;//0x00FDEEEE;
	Axis::dialogBgBrush.CreateSolidBrush(Axis::dialogBgColor);

	Axis::interFrame = 0;
	Axis::reviseFrameSize = CSize(3, 5);

	// for developer (내부IP로 접속할 경우 TRUE로 셋팅됨.
	Axis::devMode = FALSE;

	Axis::handCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);
	Axis::maskColor = RGB(255, 0, 255); // transparent mask color

	Axis::font.CreatePointFont(90, "굴림체");

	LOGFONT lf;
	Axis::font.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;

	Axis::fondBold.CreateFontIndirect(&lf);

	// initialize news source
	 const char* const newsSource[] =
	{   //moid 20240704 뉴스웨이 추가
		"시황", "공시", "이투데이", "인포스탁", "전자신문", "이데일리", "", "연합뉴스", "머니투데이", "한경뉴스", "매경뉴스", "아시아경제", "헤럴드경제", "IR GO", "컨슈머타임즈", "뉴스웨이", NULL
	};

	for (int i = 0; newsSource[i]; i++)
	{
		if (strlen(newsSource[i]) > 0)
		{
			Axis::m_newsName.Add(newsSource[i]);
			Axis::m_newsKey.Add(i);
		}
	}

	// vc2019
	/*
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (GetVersionEx(&osvi))
	{
		switch (osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			switch (osvi.dwMajorVersion)
			{
				//case 4: WINNT
				//case 5: WINXP
			case 6:
				Axis::isVista = TRUE;
				break;
			}
		}
	}
	*/

	OSVERSIONINFOEX os;
	if (GetVersion(&os) == TRUE && (int)os.dwMajorVersion == 6)
		Axis::isVista = TRUE;

	Axis::WinTheme = GetTheme();

	if (Axis::isVista)
	{
		Axis::interFrame = GetSystemMetrics(SM_CXFRAME) - 4;
		Axis::reviseFrameSize = CSize(GetSystemMetrics(SM_CXFRAME) + 3, GetSystemMetrics(SM_CYFRAME) + 5);
	}

	SkinReady();
}

int Axis::GetTheme()
{
	HKEY openKey = NULL;
	char szData[2] = "";
	char szTheme[256] = "";
	const DWORD Type = 0;
	DWORD dwType = REG_SZ;
	DWORD dwCount = sizeof(char) * 2;
	DWORD dwTheme = sizeof(char) * 256;

	::RegOpenKey(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\ThemeManager\\"),&openKey);
	::RegQueryValueEx(openKey,_T("ThemeActive"),0,&dwType,(LPBYTE)szData,&dwCount);
	::RegCloseKey(openKey);

	CString s;
	s.Format("AXISCM THEME [%s]\n",szData);
//	OutputDebugString(s);

	if(strcmp(szData,"1") == 0)
	{
		::RegOpenKey(HKEY_CURRENT_USER,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\"),&openKey);
		::RegQueryValueEx(openKey,_T("CurrentTheme"),0,&dwType,(LPBYTE)szTheme,&dwTheme);
		::RegCloseKey(openKey);

		CString str(szTheme,256);

		s.Format("THEME [%s]\n",str);
	//	OutputDebugString(s);

		if(str.Find("basic.theme") > -1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}

	return 0;
}

void Axis::Release()
{
	if (bmpBandTerm.m_hObject)
		::DeleteObject(bmpBandTerm.Detach());

#ifdef DF_USESTL_COMMON
	_mbitmaps.clear();
#else 
	CString name;

	bitmap_res res;

	for (POSITION pos = m_bitmaps.GetStartPosition(); pos; )
	{
		m_bitmaps.GetNextAssoc(pos, name, res);

		delete res.bitmap;
	}
#endif
}

void Axis::ReloadSkin( void )
{
	//CString name;
	//bitmap_res res{};
	//for (POSITION pos = m_bitmaps.GetStartPosition(); pos; )
	//{
	//	m_bitmaps.GetNextAssoc(pos, name, res);
	//	
	//	if( res.bskin )
	//	{
	//		CString str;
	//		str.Format("%s_%s",Axis::skinName,name);		
	//		
	//		if( res.bStaff && !Axis::isCustomer )
	//			str = "STAFF_" + str;

	//		ReloadFileBitmap( res.bitmap, str );
	//	}
	//}

#ifdef DF_USESTL_COMMON  //ReloadSkin
	//OutputDebugString("[axiscm] ReloadSkin");
	for_each(_mbitmaps.begin(), _mbitmaps.end(), [&](const auto& item) {
		if (std::get<1>(item.second))
		{
			CString str;
			str.Format("%s_%s", Axis::skinName, item.first);

			if (std::get<2>(item.second) && !Axis::isCustomer)
				str = "STAFF_" + str;

			ReloadFileBitmap(std::get<0>(item.second).get(), str);
		}
	});
#else 
	CString name;
	bitmap_res res;

	for (POSITION pos = m_bitmaps.GetStartPosition(); pos; )
	{
		m_bitmaps.GetNextAssoc(pos, name, res);

		if (res.bskin)
		{
			CString str;
			str.Format("%s_%s", Axis::skinName, name);

			if (res.bStaff && !Axis::isCustomer)
				str = "STAFF_" + str;

			ReloadFileBitmap(res.bitmap, str);
		}
	}
#endif

	
}

void Axis::SetSkin(const char* skin)
{
	if (Axis::skinName != skin)
	{
		Axis::skinName = skin;
		SkinReady();
	}
}

void Axis::SkinReady()
{
	Axis::dialogBgColor = Axis::GetColor(66);

	if (Axis::dialogBgBrush.GetSafeHandle())
	{
		Axis::dialogBgBrush.DeleteObject();
	}
	Axis::dialogBgBrush.CreateSolidBrush(Axis::dialogBgColor);
}

void Axis::DrawBandTerm(CDC* pDC, int x)
{
	if (!bmpBandTerm.m_hObject)
	{
		HBITMAP bmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), Format("%s\\image\\BandTerm.bmp", Axis::home),
									IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (bmp)
		{
			bmpBandTerm.Attach(bmp);
		}
		else
			return;
	}
	
	CDC mdc;
	mdc.CreateCompatibleDC(pDC);
	CBitmap* oldBmp = mdc.SelectObject(&bmpBandTerm);

	BITMAP bm;
	bmpBandTerm.GetBitmap(&bm);

	pDC->BitBlt(x, 1, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(oldBmp);
}

HBRUSH Axis::GetCtlBrush(CWnd* pWnd)
{
	if (Axis::dialogBgColor == 0)
		return NULL;

	char buffer[MAX_PATH];

	::GetClassName(pWnd->GetSafeHwnd(), buffer, MAX_PATH);

	 const char* const exceptClass[] =
	{
		"SysTreeView32", "Edit", NULL
	};

	for (int i = 0; exceptClass[i]; i++)
	{
		if (!strcmp(buffer, exceptClass[i]))
			return NULL;
	}

	return (HBRUSH)Axis::dialogBgBrush.m_hObject;
}

CBitmap* Axis::GetBitmap(const char* fileName, const char* key, bool bStaff)
{	
	//OutputDebugString("[axiscm] GetBitmap");
#ifdef DF_USESTL_COMMON   //GetBitmap(file)
	CString strKey;
	CString strFilename;

	strFilename = fileName;

	if (key == NULL)
		strKey = fileName;
	else
		strKey = key;

	strKey.MakeUpper();
	strFilename.MakeUpper();

	auto it = _mbitmaps.emplace(std::move(std::make_pair(strKey, std::make_tuple(
		std::make_shared<CBitmap>(), key ? TRUE : FALSE, bStaff))));
	if (it.second)
		std::get<0>(it.first->second) = LoadFileBitmap(strFilename);

	if (std::get<0>(it.first->second) == nullptr)
		return nullptr;

	return std::get<0>(it.first->second).get();
#else 
	CBitmap* bmp;

	CString strKey;
	CString strFilename;

	strFilename = fileName;

	bitmap_res res;

	res.bStaff = bStaff;

	if (key == NULL)
	{
		res.bskin = FALSE;
		strKey = fileName;
	}
	else
	{
		res.bskin = TRUE;
		strKey = key;
	}

	strKey.MakeUpper();
	strFilename.MakeUpper();

	if (m_bitmaps.Lookup(strKey, res))
	{
		return res.bitmap;
	}

	bmp = LoadFileBitmap(strFilename);
	if (bmp)
	{
		res.bitmap = bmp;
		m_bitmaps[strKey] = res;
	}

	return bmp;
#endif
	
}

CBitmap* Axis::GetBitmap(UINT resourceID)
{
	//OutputDebugString("[axiscm] GetBitmap");
#ifdef DF_USESTL_COMMON //GetBitmap (resource)
	CString name(Format("%d", resourceID));
	auto it = _mbitmaps.emplace(std::move(std::make_pair(name, std::make_tuple(
		std::make_shared<CBitmap>(), FALSE, FALSE))));
	if (it.second)
		std::get<0>(it.first->second)->LoadBitmap(resourceID);

	if (std::get<0>(it.first->second)->m_hObject)
		return std::get<0>(it.first->second).get();
	return nullptr;
#else 
	CBitmap* bmp;

	bitmap_res res;

	CString name(Format("%d", resourceID));

	if (m_bitmaps.Lookup(name, res))
	{
		return res.bitmap;
	}

	bmp = new CBitmap;
	bmp->LoadBitmap(resourceID);
	if (bmp->m_hObject)
	{
		res.bitmap = bmp;
		res.bskin = FALSE;
		TRACE("Loaded Resource Bitmap ID = %d\n", resourceID);
		m_bitmaps[name] = res;
		return bmp;
	}

	delete bmp;

	return NULL;
#endif
	
}

CBitmap* Axis::GetSkinBitmap(const char* fileName, bool bStaff)
{
	CString img(Axis::skinName);
	
	img += '_';
	img += fileName;

	if( bStaff && !Axis::isCustomer )
		img = "STAFF_" + img;	

	return GetBitmap(img, fileName, bStaff);
}

BOOL Axis::ReloadFileBitmap( CBitmap* p, const char* bmpName)
{
	p->DeleteObject();

	HBITMAP hBitmap{};
	CString fileName;
	CString strSkin = Axis::skinName;

	CString path(bmpName);
	path.MakeUpper();
//	if (path.Find(".BMP") < 0)
		fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
//	else
//		fileName = path;

	HINSTANCE  ins = AfxGetInstanceHandle();
	hBitmap = (HBITMAP)::LoadImage(ins, fileName, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		//TRACE("Loaded File Bitmap NAME = %s\n", bmpName);
		TRACE("RELOAD SUCCESS !!!!! = %s\n", bmpName);
		/*CBitmap* bmp = new CBitmap;
		bmp->Attach(hBitmap);*/		
	}
	else
	{
		//TRACE("Load Failed Bitmap NAME = %s\n", bmpName);
		TRACE("RELOAD FAIL!!!!! = %s\n", bmpName);
		return FALSE;
	}

	p->Attach( hBitmap );

	return TRUE;

}

#ifdef DF_USESTL_COMMON  //LoadFileBitmap
std::shared_ptr<CBitmap> Axis::LoadFileBitmap(const char* bmpName)
{
	//OutputDebugString("[axiscm] LoadFileBitmap");
	HBITMAP hBitmap{};
	CString fileName;

	CString strSkin = Axis::skinName;

	CString path(bmpName);
	path.MakeUpper();
	if (path.Find(".BMP") < 0)
		fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	else
		fileName = path;

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		//TRACE("Loaded File Bitmap NAME = %s\n", bmpName);
		TRACE("SUCCESS !!!!! = %s\n", bmpName);
		//CBitmap* bmp = new CBitmap;
		std::shared_ptr<CBitmap> bmp = std::make_shared<CBitmap>();
		bmp->Attach(hBitmap);

		return bmp;
	}
	else
	{
		//TRACE("Load Failed Bitmap NAME = %s\n", bmpName);
		TRACE("FAIL!!!!! = %s\n", bmpName);
	}
	return NULL;
}
#else 
CBitmap* Axis::LoadFileBitmap(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;

	CString strSkin = Axis::skinName;

	CString path(bmpName);
	path.MakeUpper();
	if (path.Find(".BMP") < 0)
		fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	else
		fileName = path;

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		//TRACE("Loaded File Bitmap NAME = %s\n", bmpName);
		TRACE("SUCCESS !!!!! = %s\n", bmpName);
		CBitmap* bmp = new CBitmap;
		bmp->Attach(hBitmap);

		return bmp;
	}
	else
	{
		//TRACE("Load Failed Bitmap NAME = %s\n", bmpName);
		TRACE("FAIL!!!!! = %s\n", bmpName);
	}
	return NULL;
}
#endif

COLORREF Axis::GetColor(int index)
{
	CString colorItem;
	
	colorItem.Format("RGB%03d%s", index, Axis::skinName);
	COLORREF color;

	if (m_colors.Lookup(colorItem, color))
		return color;

	WORD rgb[3]{};

	CProfile profile(pkPalette);
	CString	 key(Format("RGB%03d", index));
	CString	 value(profile.GetString(Axis::skinName, key));

	if (value.IsEmpty())
	{
		value = profile.GetString(szPalette, key);
		if (value.IsEmpty())
			return RGB(215, 215, 215);
	}

	int j = 0, pos{};
	while (!value.IsEmpty() && j < 3)
	{
		pos = value.Find(',');
		if (pos < 0)
		{
			rgb[j] = atoi(value);
			break;
		}
		rgb[j++] = atoi(value.Left(pos));
		value = value.Mid(pos + 1);
	}

	if (j != 2)
		return RGB(215, 215, 215);

	TRACE("GETTING RGB(%d) = RGB(%d, %d, %d)\n", index, rgb[0], rgb[1], rgb[2]);

	m_colors.SetAt(colorItem, RGB(rgb[0], rgb[1], rgb[2]));

	return RGB(rgb[0], rgb[1], rgb[2]);
}

int Axis::GetNewsCount()
{
	return m_newsName.GetSize();
}

int Axis::GetNewsKey(int index)
{
	if (index >= 0 && index < Axis::GetNewsCount())
		return m_newsKey[index];
	else
		return 0;
}

LPCTSTR Axis::GetNewsName(int index)
{
	if (index >= 0 && index < Axis::GetNewsCount())
		return m_newsName[index];
	else
		return 0;
}

int Axis::MessageBox(CWnd* parent, const char* message, UINT button)
{
	return ::MessageBox(parent ? parent->m_hWnd: NULL, message, szSite, button);
}

int Axis::MessageBox(const char* message, UINT button)
{
	return Axis::MessageBox(NULL, message, button);
}

int Axis::MessageBox(UINT stringResourceID, UINT button)
{
	CString str;
	str.LoadString(stringResourceID);

	return Axis::MessageBox(str, button);
}

CString Axis::Parse(CString& source, CString delimiter)
{
	CString result("");
	int find;
	if ((find = source.Find(delimiter)) < 0)
	{
		result = source;
		source.Empty();
		return result;
	}	
	else
	{
		result = source.Left(find);
		source = source.Mid(find + delimiter.GetLength());
		return result;
	}
}

CString Axis::Parse(CString& source, char delimiter)
{
	CString result("");
	int find;
	if ((find = source.Find(delimiter)) < 0)
	{
		result = source;
		source.Empty();
		return result;
	}
	else
	{
		result = source.Left(find);
		source = source.Mid(find + sizeof(char));
		return result;
	}
}

/************************************************************************/
/* CControlChanger Class                                                */
/************************************************************************/

CControlChanger::CControlChanger(CWnd* parent)
{
	ASSERT(parent);

	m_parent = parent;

	m_font.CreatePointFont(90, "굴림체");
	Initialize();
}

CControlChanger::~CControlChanger()
{
	Release();
}

void CControlChanger::Initialize()
{
	CString path;
	path.Format("%s\\%s\\2btn", Axis::home, IMAGEDIR);

	m_btnBitmap[0] = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, path + ".bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_btnBitmap[1] = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, path + "_dn.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	m_btnBitmap[2] = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance, path + "_en.bmp",
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hdrBitmap[0].LoadBitmap(IDB_COLUMNHEADER_START);
	m_hdrBitmap[1].LoadBitmap(IDB_COLUMNHEADER_SPAN);
	m_hdrBitmap[2].LoadBitmap(IDB_COLUMNHEADER_END);

	CWnd* child = m_parent->GetWindow(GW_CHILD);
	char buffer[MAX_PATH]{};

	CArray<CWnd*, CWnd*> wnds;	

	while (child)
	{
		GetClassName(child->m_hWnd, buffer, MAX_PATH);
		wnds.Add(child);
		child = child->GetWindow(GW_HWNDNEXT);
	}

	for (int i = 0; i < wnds.GetSize(); i++)
	{
		ChangeControl(wnds[i]);
	}
}

void CControlChanger::Release()
{
	for (int i = 0; i < m_controls.GetSize(); i++)
	{
		if (::IsWindow(m_controls[i]->m_hWnd))
			::DestroyWindow(m_controls[i]->m_hWnd);
		delete m_controls[i];
	}

	for (int i = 0; i < 3; i++)
		if (m_btnBitmap[i])
			DeleteObject(m_btnBitmap[i]);

	m_controls.RemoveAll();
}

void CControlChanger::ChangeControl(CWnd* wnd)
{
	char buffer[MAX_PATH];
	::GetClassName(wnd->m_hWnd, buffer, MAX_PATH);
	CString name(buffer);

	 const char* const controls[] =
	{
		"Button", "SysListView32", NULL
	};

	BOOL change = FALSE;
	LOGFONT lf{};

	for (int i = 0; controls[i]; i++)
	{
		if (name == controls[i])
		{
			change = TRUE;
			break;
		}
	}

	if (!change)
		return;

	const UINT id = wnd->GetDlgCtrlID();
	const DWORD style = wnd->GetStyle();

	const BOOL visible = style & WS_VISIBLE;
	const BOOL enabled = wnd->IsWindowEnabled();

	CRect rc;
	wnd->GetWindowRect(&rc);
	m_parent->ScreenToClient(&rc);

	CString str;
	wnd->GetWindowText(str);

	CFont* font = wnd->GetFont();
	if (!font)
		font = &m_font;
	font->GetLogFont(&lf);

	if (name == "Button")
	{
		if (id != IDOK && (style & BS_DEFPUSHBUTTON))
		{
			if (style & BS_CHECKBOX)
				return;
			if (style & BS_RADIOBUTTON)
				return;
			if ((style & BS_AUTORADIOBUTTON) && !(style & BS_PUSHLIKE))
					return;
		}

		wnd->DestroyWindow();

		CImgButton* btn = new CImgButton;
		btn->Create(str, style, rc, m_parent, id);
		btn->SetFont(font);

		btn->SetImgBitmap(m_btnBitmap[0], m_btnBitmap[1], m_btnBitmap[2]);

		if (!enabled)
			btn->EnableWindow(FALSE);
		if (!visible)
			btn->ModifyStyle(WS_VISIBLE, 0);

		m_controls.Add(btn);
	}
	else if (name == "SysListView32")
	{
		const CListCtrl* list = (CListCtrl*)wnd;
		CHeaderCtrl* pHeader = list->GetHeaderCtrl();
		if (pHeader == NULL) return;

		CImgHeaderCtrl* header = new CImgHeaderCtrl;
		header->SetImgBitmap(&m_hdrBitmap[0], &m_hdrBitmap[1], &m_hdrBitmap[2]);

		header->SubclassWindow(pHeader->m_hWnd);

		HDITEM hdItem;
		hdItem.mask = HDI_FORMAT;
		
		for (int i = 0; i < header->GetItemCount(); i++)
		{
			header->GetItem(i, &hdItem);
			hdItem.fmt |= HDF_OWNERDRAW;
			header->SetItem(i, &hdItem);
		}
		m_controls.Add(header);
	}
}
