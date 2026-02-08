// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_MarketPicker.h"
#include "MainWnd.h"
#include "AxStd.hpp"
#include "../../../H/axdll.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

#define DF_TRIGGER_GUBN "[CXMARKETPICKER]"

#include "../../../axis/axMsg.hxx"
//#define MMSG_SHARED_REGWND		0x10
//#define MMSG_SHARED_BROADCAST		0x11
//#define MMSG_SHARED_CTRLDESTROY		0x12
//#define MMSG_SHARED_GETHANDLECNT		0x13
//#define MMSG_SHARED_GUIDEMESSAGE		0x14
//#define MMSG_MKMSG_FROM_MAP				0x17
//#define MMSG_MKMSG_GET_MAP_MARKET 0x22

#define TM_SENDMKMSG_TOMAIN	9898
#define TM_REDRAW   9897

CString MarketToString(int imarket)
{
	if (imarket == 1)
		return "KRX";
	if (imarket == 2)
		return "NXT";
	if (imarket == 3)
		return "통합";
}

auto iMarketTobit = [](int imarket)
{
	if (imarket == 1)
		return 1;
	if (imarket == 2)
		return 2;
	if (imarket == 3)
		return 4;
	return 0;
};

void WriteLog(LPCSTR log, ...)
{
#if 1
	TRY
	{
		CString slog;
		slog.Format("[AXIS][WriteLog] [%s]\n", log);

		char chfile[500]{};
		GetModuleFileName(nullptr, chfile, 260);

		CString spath, stmp;
		spath.Format("%s", chfile);
		spath.TrimRight();
		spath.Replace("axis.exe", "");

		FILE* fp;
		fopen_s(&fp, spath + "\\axis.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}

CMainWnd::CMainWnd(CWnd* pParent, void* pParam)
{
	EnableAutomation();
	m_pWizard = pParent;

	m_Param.key = ((struct _param*)pParam)->key;
	m_Param.name = CString(((struct _param*)pParam)->name, ((struct _param*)pParam)->name.GetLength());
	m_Param.rect = CRect(((struct _param*)pParam)->rect.left, ((struct _param*)pParam)->rect.top, ((struct _param*)pParam)->rect.right, ((struct _param*)pParam)->rect.bottom);
	m_Param.fonts = CString(((struct _param*)pParam)->fonts, ((struct _param*)pParam)->fonts.GetLength());
	m_Param.point = ((struct _param*)pParam)->point;
	m_Param.style = ((struct _param*)pParam)->style;
	m_Param.tRGB = ((struct _param*)pParam)->tRGB;
	m_Param.pRGB = ((struct _param*)pParam)->pRGB;
	m_Param.options = CString(((struct _param*)pParam)->options, ((struct _param*)pParam)->options.GetLength());
	//m_Param.options = m_Param.options.Right(2);

	CString stemp, stmp;
	stemp = m_Param.options;
	parseOptions();

	m_slog.Format("[cxMarketPicker][MARKETPICKER][%s]<%d>[%s] m_Param.key=[%d]   m_Param.options=[%s] ", __FUNCTION__, __LINE__,
		m_sCtrlName, m_Param.key,  m_Param.options);
	OutputDebugString(m_slog);

	m_clrBack = RGB(255, 0, 255);

	codedll.Load("axiscode.dll");
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 2, OnMsgFromDll)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "GetMarketType", GetMarketType, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP  
	DISP_PROPERTY_EX_ID(CMainWnd, "sMarket", dispidsMarket, GetsMarket, SetsMarket, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "sNick", dispidsNick, GetsNick, SetsNick, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "sCode", dispidsCode, GetsCode, SetsCode, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "Major", dispidMajor, GetMajor, SetMajor, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "Minor", dispidMinor, GetMinor, SetMinor, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "Permission", dispidPermission, GetPermission, SetPermission, VT_BSTR)
	DISP_PROPERTY_EX_ID(CMainWnd, "MNum", dispidMNum, GetMNum, SetMNum, VT_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "SetKey", dispidSetKey, SetKey, VT_EMPTY, VTS_I2)
	DISP_FUNCTION_ID(CMainWnd, "IsNXTAble", dispidIsNXTAble, IsNXTAble, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "GetBroadData", dispidGetBroadData, GetBroadData, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "GetMapPermission", dispidGetMapPermission, GetMapPermission, VT_I2, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "displayGuide", dispiddisplayGuide, displayGuide, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "WriteFile", dispidWriteFile, WriteFile, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "ReadFile", dispidReadFile, ReadFile, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "TraceLog", dispidTraceLog, TraceLog, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "visible", dispidvisible, visible, VT_EMPTY, VTS_I2)
	
	DISP_FUNCTION_ID(CMainWnd, "IsMajor", dispidIsMajor, IsMajor, VT_I2, VTS_NONE)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5A6EDD2B-E9DC-498F-BBFD-90D677E4A9EE}
static const IID IID_IMainWnd =
{ 0x5a6edd2b, 0xe9dc, 0x498f, { 0xbb, 0xfd, 0x90, 0xd6, 0x77, 0xe4, 0xa9, 0xee } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

void CMainWnd::parseOptions()
{
	int	idx = 0, pos = 0;
	CString	keys, text, tmps, strtemp;
	CString sOption{};
	sOption = m_Param.options;

	tmps = _T("/ ");
	keys = _T("dpbschlutoargmxwfyvk");

	for (int ii = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = sOption.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = sOption.Find('/', idx);

		text = (pos < 0) ? sOption.Mid(idx) : sOption.Mid(idx, pos - idx);
		text.TrimLeft();

		switch (keys[ii])
		{
			case 'u':
			{
				if (text == "2")
					m_bBroadMsg = TRUE;
				else
					m_bBroadMsg = FALSE;
			}
			break;
			case 't':
			{
				m_sID = text;
			}
			break;
			case 'k':
			{
				if (text == "2")
					m_sizetype = TYPE_SMALL;
				else
					m_sizetype = TYPE_NORMAL;
			}
			break;
			case 'd':
			{
				if (text == "2")
					m_bTriggerMK = FALSE;
			}
			break;
			case 'v':
			{
				m_major = atoi(text);
			}
			break;
			case 'y':
			{
				if (text == "2")
					m_bAutoMarketChange = FALSE;
			}
			break;
			case 'f':
			{
				if(text.GetLength() > 0)
						m_sCtrlName = text;
			}
			break;
		}
	}
}

CString CMainWnd::Parser(CString& srcstr, CString substr)
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
	return "";
}

BSTR CMainWnd::GetMarketType() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here
	strResult.Format("%s", m_strMarket);
	return strResult.AllocSysString();
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

long CMainWnd::OnMsgFromDll(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case 100:
		{
			m_slog.Format("[cxMarketPicker][MARKETPICKER][OnMsgFromDll]       DLL 화면 -->  컨트롤MSG            [%d][%x]", LOWORD(wParam), (CWnd*)lParam);
			//OutputDebugString(m_slog);
		
			m_pContainerWnd = (CWnd*)lParam;
			m_bInDLL = TRUE;

			return 1;
		}
		break;
	}

	return 0;
}

long CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	m_slog.Format("[cxMarketPicker][MARKETPICKER][%s]<%d>[%s] m_sID=[%s] OnMessage [0x%0d]   ", 
		__FUNCTION__, __LINE__, m_sCtrlName, m_sID, (LOBYTE(LOWORD(wParam))));
	OutputDebugString(m_slog);
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)"";
	case DLL_OUBx:
	{
		struct _extTHx* pextTHx{};
		pextTHx = (struct _extTHx*)lParam;
		
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		int length = exth->size;

		if (length < 5)   //modi NXT   시장구분 피커 클릭시 트리거 연동 메시지
		{
			CString str = CString(exth->data);
			str.TrimRight();
			if (str == "통합")
				m_markettype = TYPE_TOTAL;
			else if (str == "KRX")
				m_markettype = TYPE_KRX;
			else
				m_markettype = TYPE_NXT;
		}
			
		Invalidate();

		m_slog.Format("[cxMarketPicker][MARKETPICKER][%s]<%d>[%s] cx_marketpicker DLL_OUBx [%s]   ", __FUNCTION__, __LINE__, m_sCtrlName,
			m_markettype == 3 ? "통합" : m_markettype == 1 ? "KRX" : "NXT");
		OutputDebugString(m_slog);
	}
	break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		break;

	case DLL_ALERTx:
		struct	_alertR* alertR;
		alertR = (struct _alertR*)lParam;
		//WriteAlertx(alertR);
		break;

	case DLL_ALERT:
		//	WriteAlert((char*)lParam);
		break;

	case DLL_TRIGGER:
	{
		m_slog.Format("[cxMarketPicker][MARKETPICKER][NXT_TIME][%s]<%d>[%s] m_bBroadMsg = [%d] DLL_TRIGGER [%s]   ", __FUNCTION__, __LINE__, 
			m_sCtrlName, m_bBroadMsg ,(char*)lParam);
		OutputDebugString(m_slog);
		CString sval, stmp;
		sval.Format("%s", (char*)lParam);  //MKTIME
		stmp = Parser(sval, "\t");

		m_slog.Format("[cxMarketPicker][MARKETPICKER][NXT_TIME][%s]<%d>[%s] sval = [%s]    sval [%s]   ", __FUNCTION__, __LINE__,
			m_sCtrlName, stmp, sval);
		OutputDebugString(m_slog);
		//WriteLog(m_slog);

		m_mapStrToStr.SetAt(stmp, sval);
		if(m_bBroadMsg)
			m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());
	}
	break;
	case DLL_DOMINO:
	case DLL_NOTICE:
		break;

	case DLL_SETFONT:
	{
		m_Param.point = HIWORD(wParam);
		CString tmp = CString((char*)lParam);
		if (!tmp.IsEmpty())
			m_Param.fonts = tmp;
		else
			m_Param.fonts = _T("굴림체");
		Invalidate();
	}
	break;
	default:break;
	}
	return 0;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

#ifdef DF_USE_GDIPLUS
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
#endif

	Init();
	return 0;
}

CBitmap* CMainWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

COLORREF CMainWnd::GetIndexColor(int index)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}


void CMainWnd::SetImages()
{
	CString sImageDir = "";
	sImageDir.Format("%s\\image\\", m_sRoot);

	//HBITMAP		m_hIBitmap, m_hKBitmap, m_hNBitmap;
	CFileFind	finder;
	m_sImagePath.MakeLower();
	//m_sImagePath.Replace(".bmp", "");

#ifdef DF_USE_GDIPLUS
	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "Market_Total.bmp";
	else
		m_sImagePath = sImageDir + "Market_S_Total.png";

	CStringW strW;
	strW = CA2W(m_sImagePath, CP_ACP);
	m_pTBitmap = Gdiplus::Bitmap::FromFile(strW);

	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "Market_KRX.bmp";
	else
		m_sImagePath = sImageDir + "Market_S_KRX.png";

	strW = CA2W(m_sImagePath, CP_ACP);
	m_pKBitmap = Gdiplus::Bitmap::FromFile(strW);


	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "Market_NXT.bmp";
	else
		m_sImagePath = sImageDir + "Market_S_NXT.png";

	strW = CA2W(m_sImagePath, CP_ACP);
	m_pNBitmap = Gdiplus::Bitmap::FromFile(strW);
#else
	if(m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "Market_Total.bmp";
	else
		m_sImagePath = sImageDir + "Market_S_Total.bmp";

	if (finder.FindFile(m_sImagePath))
		m_hIBitmap = getBitmap(m_sImagePath)->operator HBITMAP();
	else
		m_hIBitmap = NULL;

	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "Market_KRX.bmp";
	else
		m_sImagePath = sImageDir + "Market_S_KRX.bmp";

	if (finder.FindFile(m_sImagePath))
		m_hKBitmap = getBitmap(m_sImagePath)->operator HBITMAP();
	else
		m_hKBitmap = m_hIBitmap;

	if (m_sizetype == TYPE_NORMAL)
		m_sImagePath = sImageDir + "Market_NXT.bmp";
	else
		m_sImagePath = sImageDir + "Market_S_NXT.bmp";

	if (finder.FindFile(m_sImagePath))
		m_hNBitmap = getBitmap(m_sImagePath)->operator HBITMAP();
	else
		m_hNBitmap = m_hIBitmap;
#endif


}

void CMainWnd::Init()
{
	CString strdata, name;
	m_sRoot = Variant(homeCC, "");
	name = Variant(nameCC, "");
	//ReadMarketFile();

//	if (m_strMarket.IsEmpty())
	{
		CString sfile;
		char readB[32]{};
		sfile.Format("%s\\%s\\%s\\%s", m_sRoot, "user", name, "userconf.ini");
		int readL = GetPrivateProfileString("MKgubn", "Market", "KRX", readB, sizeof(readB), sfile);
		strdata.Format("%s", readB);
		strdata.TrimRight();

		m_strMarket = strdata;
		if (m_strMarket == "KRX")
			m_markettype = TYPE_KRX;
		else if (m_strMarket == "NXT")
			m_markettype = TYPE_NXT;
		else
			m_markettype = TYPE_TOTAL;

		m_orimarkettype = m_markettype;
	}
	
	SetImages();

	int readL = 0;
	char readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", m_sRoot, "user", name, name);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return;

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
	m_pMainWnd = CWnd::FromHandle(hWnd);

	if (m_bBroadMsg)
		m_pMainWnd->SendMessage(WM_USER, MMSG_SHARED_REGWND, (LPARAM)(LPCSTR)this);  //shared memory 에 본인주소 저장

	int mapAuth = m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_GET_MAP_MARKET, (LPARAM)(LPCSTR)m_sID);   //INIT

	m_slog.Format("~~~~ [cxMarketPicker][MARKET]<Init>[%s]  m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_minor=[%d]", 
		m_sCtrlName, m_sID, MarketToString(m_markettype), mapAuth, m_major, m_minor);
	OutputDebugString(m_slog);


	if (mapAuth == 0)  //axisMarket.ini 에 정의되어있지 않은 경우 
	{
		if (m_sID.Left(2) == "IB")   
		{ //간혹 정의되있지 않은 대표화면 내부의 오브젝트들이 정의되있는 다른 화면에서 혼용되는 경우 디폴트 KRX 해야함
			m_markettype = TYPE_KRX;
			m_strMarket = "KRX";
		}
		else  //사용자가 지정한 거래소 파일 참조(위에서 함)
		{

		}
	}
	else
	{
		int bitMarket = iMarketTobit(m_markettype);
		if ((mapAuth & bitMarket) == 0) 
		{  //KRX, NXT, 통합 전부 비트연산0 면 사용자 저장 파일 거래소를 따라간다.
			if (mapAuth & 1)
			{
				m_markettype = TYPE_KRX;
				m_strMarket = "KRX";
			}
			else if (mapAuth & 2)
			{
				m_markettype = TYPE_NXT;
				m_strMarket = "NXT";
			}
			else if (mapAuth & 4)
			{
				m_markettype = TYPE_TOTAL;
				m_strMarket = "통합";
			}
		}
	}

}

void CMainWnd::ReadMarketFile()
{
	CString sfile;
	sfile.Format("%s\\%s\\AXISMARKET.INI", m_sRoot, "tab");
	CStdioFile file;
	CString line;

	if (!file.Open(sfile, CFile::modeRead | CFile::typeText))
		return;

	while (file.ReadString(line))
	{
		line.TrimRight(); // 앞뒤 공백 제거

		// 빈 줄, 주석은 건너뜀
		if (line.IsEmpty() || line[0] == _T('#') || line[0] == _T(';'))
			continue;

		int pos = line.Find(_T('='));
		if (pos > 0)
		{
			CString key = line.Left(pos);
			CString value = line.Mid(pos + 1);

			key.TrimRight();
			value.TrimRight();

			if (!key.IsEmpty())
				m_mapPermissions.SetAt(key, value);
		}
	}
	file.Close();
}


CSize CMainWnd::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);

	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

#ifdef DF_USE_GDIPLUS
	CRect	rc;
	CString txt;

	GetClientRect(&rc);
	if (m_markettype == TYPE_TOTAL)
	{
		if (m_pTBitmap)
		{
			Gdiplus::Graphics graphics(dc.m_hDC);
			// 이미지 크기 조정하여 그리기
			graphics.DrawImage(m_pTBitmap,
				0, 0,                     // 그릴 위치 (좌상단)
				rc.Width() + 1, rc.Height()); // 그릴 크기 (클라이언트 영역 크기
		}
	}
	else if (m_markettype == TYPE_KRX)
	{
		if (m_pKBitmap)
		{
			Gdiplus::Graphics graphics(dc.m_hDC);
			// 이미지 크기 조정하여 그리기
			graphics.DrawImage(m_pKBitmap,
				0, 0,                     // 그릴 위치 (좌상단)
				rc.Width() + 1, rc.Height()); // 그릴 크기 (클라이언트 영역 크기
		}
	}
	else
	{
		if (m_pNBitmap)
		{
			Gdiplus::Graphics graphics(dc.m_hDC);
			// 이미지 크기 조정하여 그리기
			graphics.DrawImage(m_pNBitmap,
				0, 0,                     // 그릴 위치 (좌상단)
				rc.Width() + 1, rc.Height()); // 그릴 크기 (클라이언트 영역 크기
		}
	}
	
#else
	CRect	rc;
	CString txt;

	GetClientRect(&rc);
	dc.FillSolidRect(rc, GetIndexColor(m_Param.pRGB));

	dc.SetBkMode(TRANSPARENT);
	if (m_hIBitmap && m_hKBitmap && m_hNBitmap)
	{
		CDC	memDC;
		CBitmap* pOldBitmap = nullptr, * pBitmap = nullptr;
		if (m_markettype == TYPE_TOTAL)
			pBitmap = CBitmap::FromHandle(m_hIBitmap);
		else if (m_markettype == TYPE_KRX)
			pBitmap = CBitmap::FromHandle(m_hKBitmap);
		else
			pBitmap = CBitmap::FromHandle(m_hNBitmap);

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);
		if (memDC.m_hDC != NULL)
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);

		dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, BitmapSize(m_hIBitmap).cx, BitmapSize(m_hIBitmap).cy, SRCCOPY);
		if (pOldBitmap)
			memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
#endif

}


void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
}
//GetMNum
BSTR CMainWnd::GetMNum()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%d", m_sID);
	strResult.TrimRight();

	return strResult.AllocSysString();
}

void CMainWnd::SetMNum(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sVal;
	m_sID.Format("%s", newVal);

	m_slog.Format("@@@@	[cxMarketPicker][MARKET]<SetMNum>[%s]  m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_sCode=[%s]  m_minor=[%d] ",
		m_sCtrlName, m_sID, MarketToString(m_markettype), m_iPermission, m_major, m_sCode, m_minor);
	OutputDebugString(m_slog);
}

BSTR CMainWnd::GetMajor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%d", m_major);
	strResult.TrimRight();

	return strResult.AllocSysString();
}

void CMainWnd::SetMajor(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sVal;
	sVal.Format("%s", newVal);

	m_major = atoi(sVal);
}
//
BSTR CMainWnd::GetMinor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%d", m_minor);
	strResult.TrimRight();

	return strResult.AllocSysString();
}

void CMainWnd::SetMinor(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sVal;
	sVal.Format("%s", newVal);

	m_minor = atoi(sVal);
}

SHORT CMainWnd::GetPermission()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return m_iPermission;
}

void CMainWnd::SetPermission(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//if (!m_bAutoMarketChange)
	//	return;
	
	CString sVal;
	sVal.Format("%s", newVal);
	m_iPermission = atoi(sVal);
}

BSTR CMainWnd::GetsNick()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%s", m_sCtrlName);
	strResult.TrimRight();

	return strResult.AllocSysString();
}

void CMainWnd::SetsNick(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sMarket;
	m_sCtrlName.Format("%s", newVal);
	m_sCtrlName.Trim();
}

BSTR CMainWnd::GetsCode()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%s", m_sCode);
	strResult.TrimRight();

	return strResult.AllocSysString();
}

void CMainWnd::SetsCode(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sMarket;
	m_sCode.Format("%s", newVal);
	m_sCode.Trim();

	if (!m_bAutoMarketChange)
		return;

	if (IsNXTCode(m_sCode) || m_sCode.IsEmpty())
	{
		if (m_sID == "IB303000" || m_sID == "IB100800" || m_sID == "IB101900")
			m_iPermission = 3;
		else if (m_sID == "IB105000" || m_sID == "IB500000")
			m_iPermission = 1;
		else
			m_iPermission = 7;
	}
	else
	{//트리거등으로 입력된 종목코드가 NXT 불가 종목인경우 

		if (m_markettype == TYPE_NXT)
		{
			//if (m_orimarkettype != TYPE_NXT)
			//	m_markettype = m_orimarkettype;
			//else
			//	m_markettype = TYPE_KRX;
			m_markettype = TYPE_TOTAL;
		}

		switch (m_markettype)
		{
			case TYPE_KRX:
				m_strMarket = "KRX"; break;
			case TYPE_NXT:
				m_strMarket = "NXT"; break;
			case TYPE_TOTAL:
				m_strMarket = "통합";  break;
		}

		if(m_sID == "IB303000" || m_sID == "IB100800" || m_sID == "IB101900")
			m_iPermission = 1;
		else if (m_sID == "IB105000" || m_sID == "IB500000")
			m_iPermission = 1;
		else
			m_iPermission = 5;
	}




	m_slog.Format("%s\t%d\t%d\t%d", m_sID, m_markettype, m_iPermission, m_major);   //통합과 NXT만 되는 권한5

	CString slog;
	slog.Format("!!!!!![cxMarketPicker][MARKET]<SetsCode>[%s]  m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_sCode=[%s] m_minor = [%d]", 
		m_sCtrlName,m_sID, MarketToString(m_markettype), m_iPermission, m_major, m_sCode, m_minor);
	OutputDebugString(slog);

	if (m_pMainWnd && m_bTriggerMK && m_sID.GetLength() > 0 && m_sID.Left(2) == "IB")  
		m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_FROM_MAPNMAP, (LPARAM)(LPSTR)(LPCTSTR)m_slog);  //SetsCode 제목타이틀 거래소구분 바꾸기 위한..

	Invalidate();
	SetTimer(TM_REDRAW, 1, nullptr);
}

BSTR CMainWnd::GetsMarket()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	strResult.Format("%s", m_strMarket);

	return strResult.AllocSysString();
}


void CMainWnd::SetsMarket(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sMarket;
	sMarket.Format("%s", newVal);
	sMarket.Trim();
	
	if (sMarket == "KRX")
	{
		m_markettype = TYPE_KRX;
		m_strMarket = "KRX";
	}
	else if (sMarket == "NXT")
	{
		m_markettype = TYPE_NXT;
		m_strMarket = "NXT";
	}
	else if (sMarket == "통합")
	{
		m_markettype = TYPE_TOTAL;
		m_strMarket = "통합";
	}

	if (m_bAutoMarketChange)
	{
		if (IsNXTCode(m_sCode) || m_sCode.IsEmpty())
		{
			if (m_sID == "IB303000" || m_sID == "IB100800" || m_sID == "IB101900")
				m_iPermission = 3;
			else if (m_sID == "IB105000" || m_sID == "IB500000")
				m_iPermission = 1;
			else
				m_iPermission = 7;
		}
		else
		{//트리거등으로 입력된 종목코드가 NXT 불가 종목인경우 
			if (m_markettype == TYPE_NXT)
				m_markettype = TYPE_TOTAL;

			switch (m_markettype)
			{
			case TYPE_KRX:
				m_strMarket = "KRX"; break;
			case TYPE_NXT:
				m_strMarket = "NXT"; break;
			case TYPE_TOTAL:
				m_strMarket = "통합";  break;
			}

			if (m_sID == "IB303000" || m_sID == "IB100800" || m_sID == "IB101900")
				m_iPermission = 1;
			else if (m_sID == "IB105000" || m_sID == "IB500000")
				m_iPermission = 1;
			else
				m_iPermission = 5;
		}

		m_slog.Format("%s\t%d\t%d\t%d", m_sID, m_markettype, m_iPermission, m_major);

		CString slog;
		slog.Format("				[cxMarketPicker][MARKET]<SetsMarket>[%s] m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_sCode=[%s] m_minor = [%d]", 
			m_sCtrlName, m_sID, MarketToString(m_markettype), m_iPermission, m_major, m_sCode, m_minor);
		OutputDebugString(slog);

		if (m_pMainWnd && m_bTriggerMK && m_sID.GetLength() > 0 && m_sID.Left(2) == "IB")  
			m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_FROM_MAPNMAP, (LPARAM)(LPSTR)(LPCTSTR)m_slog); //SetsMarket 제목타이틀 거래소구분 바꾸기 위한..
	}
	//else
	//	SetTimer(TM_SENDMKMSG_TOMAIN, 1, nullptr);

	Invalidate();
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


void CMainWnd::SetKey(SHORT key)  //컨트롤을 품고있는 맵화면의 키
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_mapKey = (int)key;
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

BOOL CMainWnd::IsInMajor()
{
	if (m_major == 0)
		return FALSE;

	int minor = (int)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, minorCC), NULL);
	int major = (int)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);
	if (m_major == major)
		return TRUE;

	return FALSE;
}

BOOL CMainWnd::IsNXTCode(CString strCode)
{
	strCode.TrimRight();

	BOOL bret{};
	CString sYN{};
	if (m_mapCodeToNXT.Lookup(strCode, sYN))
	{
		if (sYN == "Y")
			bret = TRUE;
		else
			bret = FALSE;

		m_slog.Format("[cxMarketPicker][MARKETPICKER][ISNXT]   MAP!!!!  code =[%s]  ret = [%s] ", strCode, sYN);
		//OutputDebugString(m_slog);
		return bret;
	}

	//bret = AxStd::IsGetNxt(strCode);

	if (codedll.IsLoaded())
	{
		bret = codedll.Function<const bool WINAPI(const char*)>("IsNxt")(strCode);

		m_slog.Format("[cxMarketPicker][MARKETPICKER][ISNXT]   LOAD!!!!  code =[%s]  ret = [%d] ", strCode, bret);
		//OutputDebugString(m_slog);
		m_mapCodeToNXT.SetAt(strCode, bret == true ? "Y" : "N");
	}

	return bret;
}

//
//SHORT CMainWnd::IsMajor()
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	bool bret{};
//
//	bret = IsInMajor();
//	return bret;
//}
SHORT CMainWnd::IsNXTAble(BSTR sCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	CString strCode;
	strCode.Format("%s", sCode);
	strCode.TrimRight();

	bool bret{};
	CString sYN{};
	if (m_mapCodeToNXT.Lookup(strCode, sYN))
	{
		if (sYN == "Y")
			bret = true;
		else
			bret = false;

		m_slog.Format("[cxMarketPicker][MARKETPICKER][ISNXT]   MAP!!!!  code =[%s]  ret = [%s] ", strCode, sYN);
		//OutputDebugString(m_slog);
		return bret;
	}

	//bret = AxStd::IsGetNxt(strCode);

	if (codedll.IsLoaded())
	{
		bret =  codedll.Function<const bool WINAPI(const char*)>("IsNxt")(strCode);

		m_slog.Format("[cxMarketPicker][MARKETPICKER][ISNXT]   LOAD!!!!  code =[%s]  ret = [%s] ", strCode, sYN);
		//OutputDebugString(m_slog);

		m_mapCodeToNXT.SetAt(strCode, bret == true ? "Y" : "N");
	}

	return bret;
}


SHORT CMainWnd::GetMapPermission(BSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int iRet;
	CString sKey;
	sKey.Format("%s", sVal);
	sKey.TrimRight();

	iRet = m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_GET_MAP_MARKET, (LPARAM)(LPCSTR)sKey);  //GetMapPermission
	return iRet;
}

BSTR CMainWnd::GetBroadData(BSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sKey;
	sKey.Format("%s", sVal);
	sKey.TrimRight();
	CString strResult;

	if(m_mapStrToStr.Lookup(sKey, strResult))
		return strResult.AllocSysString();

	return strResult.AllocSysString();
}


void CMainWnd::OnDestroy()
{
	CWnd::OnDestroy();
	if (m_bBroadMsg && m_pMainWnd)
		m_pMainWnd->SendMessage(WM_USER, MMSG_SHARED_CTRLDESTROY, (LPARAM)(LPCSTR)this); //shared memory 에 본인주소 삭제

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainWnd::displayGuide(BSTR smsg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strmsg;
	strmsg.Format("%s", smsg);
	
	if(m_pMainWnd)
		m_pMainWnd->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)strmsg);  //가이드메시지 팝업
}


void CMainWnd::WriteFile(BSTR sFile, BSTR sKey, BSTR sSection, BSTR sData)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strFile, strKey, strSection, strData;
	strFile.Format("%s\\tab\\%s", m_sRoot, sFile);
	strKey.Format("%s", sKey);
	strSection.Format("%s", sSection);
	strData.Format("%s", sData);

	WritePrivateProfileString(strKey, strSection, strData, strFile);

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


BSTR CMainWnd::ReadFile(BSTR sFile, BSTR sKey, BSTR sSection)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult{};
	CString strFile, strKey, strSection;
	strFile.Format("%s\\tab\\%s", m_sRoot, sFile);
	strKey.Format("%s", sKey);
	strSection.Format("%s", sSection);
	
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	char		wb[512];
	memset(wb, ' ', 512);
	const int readL = GetPrivateProfileString(strKey, strSection, "", wb, sizeof(wb), strFile);

	if (readL > 0)
		strResult.Format("%s", wb);

	return strResult.AllocSysString();
}


void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (!m_pMainWnd)
		return;

	m_slog.Format("%s", m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_GET_MAP_INFO, 0)); //onlbuttondown  포커스맵정보
	CString mapN;
	mapN = Parser(m_slog, "\t");

	//if(m_slog == "4")  //
	//{
	//	MessageBox("KRX 거래만 지원하는 화면입니다", "IBK투자증권", MB_OK );
	//	return;
	//}

	if (m_iPermission < 0)
		m_iPermission = m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_GET_MAP_MARKET, (LPARAM)(LPCSTR)mapN);  //Onlbutton  포커스맵정보 권한

	m_slog.Format("[cxMarketPicker][MARKET]<onlbuttondown 시작>[%s] m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_minor = [%d]", m_sCtrlName,
		m_sID, MarketToString(m_markettype), m_iPermission, m_major, m_minor);
	OutputDebugString(m_slog);

	int iret = m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_GET_MAP_BLOCK, 0); //onlbuttondown 거래소 잠금 확인
	if (iret == TRUE && (m_iPermission != 0))
	{
		MessageBox("거래소 잠금 해제 후 선택 가능합니다", "IBK투자증권", MB_OK);
		return;
	}

	int	ret{};

	struct MenuItem {
		int menuCode;
		int bit;
		const char* name;
	};

	MenuItem items[] = {
		{3, 4, "통합"},
		{1, 1, "KRX"},
		{2, 2, "NXT"}
	};

	MenuItem items_[] = {
	{3, 4, "통합"},
	{1, 1, "KRX"},
	};


	CMenu cMenu;
	cMenu.CreatePopupMenu();
	
	if ((!IsNXTCode(m_sCode) && !m_sCode.IsEmpty()) && m_bAutoMarketChange)
	{
		for (auto& item : items_)
		{
			if (m_iPermission & item.bit)
			{
				cMenu.AppendMenu(MF_STRING, item.menuCode, item.name);

				if (m_markettype == item.menuCode)
					cMenu.CheckMenuItem(item.menuCode, MF_CHECKED);
			}
		}

	}
	else //m_sCode 가 존재하는데 이게 NXT 지원안하는경우
	{
		for (auto& item : items)
		{
			if (m_iPermission & item.bit)
			{
				cMenu.AppendMenu(MF_STRING, item.menuCode, item.name);

				if (m_markettype == item.menuCode)
					cMenu.CheckMenuItem(item.menuCode, MF_CHECKED);
			}
		}
	}

	GetCursorPos(&point);
	ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, this, NULL);
	
	if (ret == 0)
		return;

	if (ret > 0)
	{
		switch (ret)
		{
		case 3:
			m_strMarket = "통합";
			m_markettype = TYPE_TOTAL;
			break;
		case 1:
			m_strMarket = "KRX";
			m_markettype = TYPE_KRX;
			break;
		case 2:
			m_strMarket = "NXT";
			m_markettype = TYPE_NXT;
			break;
		}
	}

	CString strTrigger{};
	strTrigger.Format("%s\t%s\t%s\t%s", m_Param.name, DF_TRIGGER_GUBN, m_sID, m_strMarket);

	m_slog.Format("[cxMarketPicker]-----[MARKETPICKER][TRIGGER][%s] m_sID=[%s]  m_markettype=[%s]    ", m_sCtrlName, m_sID, MarketToString(m_markettype));
	OutputDebugString(m_slog);

	if (m_pContainerWnd && m_bInDLL)
		m_pContainerWnd->SendMessage(WM_USER + 1, MAKEWPARAM(100, 0), (LPARAM)strTrigger.GetString());

	//맵으로 onClick클릭 이벤트
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)m_Param.name.GetString());


	m_slog.Format("%d", m_markettype);
	if (m_pMainWnd && m_bTriggerMK)     //포커스 있는 화면타이틀 변경
		m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_FROM_MAP, (LPARAM)(LPSTR)(LPCTSTR)m_slog);  //onlbutton 제목타이틀 거래소구분 바꾸기 위한..

	CString slog;
	slog.Format("				[cxMarketPicker][MARKET]<Onlbuttondown>[%s] m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_sCode=[%s] m_minor = [%d]",
		m_sCtrlName, m_sID, MarketToString(m_markettype), m_iPermission, m_major, m_sCode, m_minor);
	OutputDebugString(slog);

	m_slog.Format("%s\t%d\t%d\t%d", m_sID, m_markettype, m_iPermission, m_major);
	if (m_pMainWnd && m_bTriggerMK)   //메이저가 일치하는 화면 타이틀 변경
		m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_FROM_MAPNMAP, (LPARAM)(LPSTR)(LPCTSTR)m_slog);   //onlbutton 제목타이틀 거래소구분 바꾸기 위한..
		
	Invalidate();

	CWnd::OnLButtonDown(nFlags, point);
}


void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_REDRAW:
		{
			KillTimer(TM_REDRAW);
			Invalidate();
		}
		break;
		case TM_SENDMKMSG_TOMAIN:
		{
			KillTimer(TM_SENDMKMSG_TOMAIN);
		
			if ((IsNXTCode(m_sCode) || m_sCode.IsEmpty()) && m_bAutoMarketChange)
			{
				if (m_sID == "IB303000" || m_sID == "IB100800" || m_sID == "IB101900")
					m_iPermission = 3;
				else if (m_sID == "IB105000" || m_sID == "IB500000")
					m_iPermission = 1;
				else
					m_iPermission = 7;
			}
			else
			{//트리거등으로 입력된 종목코드가 NXT 불가 종목인경우 
				if (m_markettype == TYPE_NXT)
					m_markettype = TYPE_TOTAL;

				switch (m_markettype)
				{
				case TYPE_KRX:
					m_strMarket = "KRX"; break;
				case TYPE_NXT:
					m_strMarket = "NXT"; break;
				case TYPE_TOTAL:
					m_strMarket = "통합";  break;
				}
				
				if (m_sID == "IB303000" || m_sID == "IB100800" || m_sID == "IB101900")
					m_iPermission = 1;
				else if (m_sID == "IB105000" || m_sID == "IB500000")
					m_iPermission = 1;
				else
					m_iPermission = 5;
			}

			m_slog.Format("%s\t%d\t%d\t%d", m_sID, m_markettype, m_iPermission, m_major);

			CString slog;
			slog.Format("				[cxMarketPicker][MARKET]<Timer>[%s] m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] m_minor = [%d]", 
				m_sCtrlName,  m_sID, MarketToString(m_markettype), m_iPermission, m_major, m_minor);
			OutputDebugString(slog);

			if (m_pMainWnd && m_bTriggerMK && m_sID.GetLength() > 0 && m_sID.Left(2) == "IB")  
				m_pMainWnd->SendMessage(WM_USER, MMSG_MKMSG_FROM_MAPNMAP, (LPARAM)(LPSTR)(LPCTSTR)m_slog); //ontimer 제목타이틀 거래소구분 바꾸기 위한..
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CMainWnd::TraceLog(BSTR slog, BSTR sKey)
{
	m_slog.Format("[cxMarketPicker][TraceLog][%s] [%s]", slog, sKey); //TRACE
	OutputDebugString(m_slog);
}

void CMainWnd::visible(SHORT bShow)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ShowWindow(bShow == 1?TRUE:FALSE);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMainWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int minor = (int)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, minorCC), NULL);
	int major = (int)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);
	m_slog.Format("[cxMarketPicker][MARKET]<onRbuttondown>m_sCtrlName =[%s] m_sID=[%s] m_markettype=[%s] m_iPermission=[%d] m_major=[%d] major=[%d] m_minor=[%d] minor=[%d]",
		m_sCtrlName, m_sID, MarketToString(m_markettype), m_iPermission, m_major, major, m_minor, minor);
	OutputDebugString(m_slog);

	m_slog.Format("[cxMarketPicker][MARKET]<onRbuttondown> IsMajor = [%d]", IsInMajor());
	OutputDebugString(m_slog);
	
	CWnd::OnRButtonDown(nFlags, point);
}


void CMainWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnMButtonDown(nFlags, point);
}


SHORT CMainWnd::IsMajor()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	short ret = IsInMajor();
	return ret;
}
