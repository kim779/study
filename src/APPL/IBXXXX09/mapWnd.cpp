#include "stdafx.h"
#include "IBXXXX09.h"
#include "mapWnd.h"
#include "../../h/grid.h"
#include "../../h/axisvar.h"
#include "../../h/fxCommonx.h"
#include "../../h/symbol.h"
#include "../../h/logio.h"
#include "math.h"
#include <MMSYSTEM.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define	WRITELOG				// write log여부
#define MAPNAME "IBXXXX09"
constexpr char lkHis = '1';
constexpr char lkCatch = '2';

#define maxJONGMOK 80
#define SC_SISE 100
#define SC_LOG 101

#define CATCHPATH "catch.ini"
// section name
#define SECNAME "CATCHENV"
// key name
#define SAVEOPTION "SAVEOPTION" // 저장여부
// default value
#define VALOPTION 0 // 저장 0:저장, 1:저장안함

struct _check
{
	int sym;
	int chk;
	double val;
};

CMapWnd::CMapWnd()
{
	initConstruct();
}

CMapWnd::~CMapWnd()
{
	destroy();
}

void CMapWnd::initConstruct()
{
	m_sRootDir = _T("");
	m_sTabDir = _T("");
	m_sFilename = _T("");
	m_sUser = _T("");
	m_sLogFile = _T("");
	m_sHisFile = _T("");
	m_sFontName = "굴림체";
	m_nFontPoint = 9;
	m_pFont = nullptr;
	m_pBFont = nullptr;

	m_mapCatch.RemoveAll();
	m_mapCapture.RemoveAll();
}

void CMapWnd::destroy()
{
	deleteAllMapCatch();
	deleteAllMapCapture();
	const BOOL bSave = (BOOL)GetPrivateProfileInt(SECNAME, SAVEOPTION, VALOPTION, m_envPath); // 주문확인
	if (bSave != 0)										  // 저장
	{
		DeleteFile(m_sFilename);
	}
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
//{{AFX_MSG_MAP(CMapWnd)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_WM_TIMER()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_USER, OnMessage)
ON_MESSAGE(WM_CAPTURE, OnCapture)
END_MESSAGE_MAP()

LONG CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	// TRACE("OnMessage Start\n");
	int len = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		len = HIWORD(wParam);
		if (HIWORD(wParam))
		{
			switch (HIBYTE(LOWORD(wParam)))
			{
			case SC_SISE: // 시세
				oubSise((char *)lParam, len);
				break;
			case SC_LOG: // history log
				oubHisLog((char *)lParam, len);
				break;
			}
		}
		break;
	case DLL_ALERT:
//		alert((char *)lParam, len);
		break;
	case DLL_ALERTx:
		{
			struct _alertR* alert = (struct _alertR*)lParam;
			struct _codeinfo *codeinfo = nullptr;
			
			if (!m_mapCatch.Lookup(alert->code, codeinfo))
				return 0;

			alertx(gsl::span<DWORD>((DWORD*)alert->ptr[0], 1000), codeinfo);
		}
		break;

	case DLL_TRIGGER:
		trigger((char *)lParam);
		break;
	}
	// TRACE("OnMessage End\n");
	return FALSE;
}

LONG CMapWnd::OnCapture(WPARAM wParam, LPARAM lParam)
{
	if (wParam == wpCONFIRM)
	{
		CCaptureWnd *capture = nullptr;
		if (m_mapCapture.Lookup((int)lParam, capture))
		{
			capture->DestroyWindow();
			delete capture;
			capture = nullptr;
			m_mapCapture.RemoveKey((int)lParam);
		}
	}
	return 0;
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_sRootDir = VariantA(m_pParent, homeCC, "");
	m_sTabDir.Format("%s\\%s\\", m_sRootDir, TABDIR);
	m_sUser = VariantA(m_pParent, userCC, "");
	CString name = VariantA(m_pParent, nameCC, "");
	m_sFilename.Format("%s/%s/%s/codeinfo.dat", m_sRootDir, USRDIR, name);
	m_sLogFile.Format("%s/%s/%s/catch.log", m_sRootDir, USRDIR, name);
	m_sHisFile.Format("%s/%s/%s/hiscatch.log", m_sRootDir, USRDIR, name);

	LOGFONT lf{};
	m_pFont = getAxFontA(m_pParent, m_sFontName, m_nFontPoint, 0);
	m_pBFont = getAxFontA(m_pParent, m_sFontName, m_nFontPoint, 3);
	if (m_pFont)
		m_pFont->GetLogFont(&lf);
	lf.lfHeight = m_nFontPoint * 10;

	m_envPath.Format("%s\\%s\\%s\\%s", m_sRootDir, USRDIR, name, CATCHPATH);

	deleteLog();
	const BOOL bSave = (BOOL)GetPrivateProfileInt(SECNAME, SAVEOPTION, VALOPTION, m_envPath); // 주문확인
	// writeLog("start catch====>save[%s]", bSave ? "저장안함" : "저장");

	if (bSave == 0)			     // 저장
		SetTimer(1001, 10, nullptr); // 블로킹 모드를 없애기 위해 20070523 by kwon

	return 0;
}

void CMapWnd::OnDestroy()
{
	// destroy();
	CWnd::OnDestroy();
}

BOOL CMapWnd::CreateMap(CWnd *pParent)
{
	m_pParent = pParent;
	return Create(nullptr, nullptr, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), pParent, 100);
}

//=================================================================================================
void CMapWnd::deleteAllMapCatch()
{
	_vinfo.clear();
	m_mapCatch.RemoveAll();
}

void CMapWnd::deleteAllMapCapture()
{
	int key = 0;
	CCaptureWnd *capture = nullptr;

	for (POSITION pos = m_mapCapture.GetStartPosition(); pos != nullptr;)
	{
		m_mapCapture.GetNextAssoc(pos, key, capture);
		if (capture == nullptr)
			continue;
		if (capture->GetSafeHwnd())
			capture->DestroyWindow();
		delete capture;
		capture = nullptr;
	}
	m_mapCapture.RemoveAll();
}

bool CMapWnd::loadCatch(bool bLog)
{
	deleteAllMapCatch();
	deleteAllMapCapture();

	CFileFind finder;
	if (!finder.FindFile(m_sFilename))
		return false;

	CString logS = "", logS_ = "";
	UINT readL{};
	CString codeS;
	struct _codeinfo *codeinfo = nullptr;

	int cnt = 0;
	logS = "포착된 종목";
	CFile fileH(m_sFilename, CFile::modeRead);
	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		auto &codeinfo = _vinfo.emplace_back(std::make_unique<_codeinfo>());
		readL = fileH.Read(codeinfo.get(), sz_codeinfo);
		if (readL < sz_codeinfo)
		{
			_vinfo.erase((_vinfo.rbegin() + 1).base());
			continue;
		}
		if (codeinfo->capture[0] == '1') // alaram capture ok
		{
			logS_.Format("[%s:%s]", getString(codeinfo->code, sizeof(codeinfo->code)), getString(codeinfo->name, sizeof(codeinfo->name)));
			logS += logS_;
			cnt++;
			_vinfo.erase((_vinfo.rbegin() + 1).base());
			continue;
		}

		codeS = getString(codeinfo->code, sizeof(codeinfo->code));
		if (codeS.GetLength() == 6)
		{
			// for realtime code
			if (codeS.GetAt(0) == '5' || codeS.GetAt(0) == '6')
				codeS.Insert(0, "J");
			else
				codeS.Insert(0, "A");
		}

		m_mapCatch.SetAt(codeS, codeinfo.get());
	}
	fileH.Close();

	if (bLog && m_mapCatch.GetCount() > 0)
	{
		logS_.Format("(%d개)", cnt);
		logS = logS_ + logS;
		// writeLog(logS);		// 포착된 종목

		// writeLog("***loadCatch [%d]개....", m_mapCatch.GetCount());
		cnt = 0;
		for (POSITION pos = m_mapCatch.GetStartPosition(); pos != nullptr;)
		{
			m_mapCatch.GetNextAssoc(pos, codeS, codeinfo);

			cnt++;
			logS = _T("");
			logS_.Format("-%d-[%s:%s] ", cnt, codeS, getString(codeinfo->name, sizeof(codeinfo->name)));
			logS += logS_;

			logS_.Format(" 주문[%c:%s:%s:%s:%s]", codeinfo->o_run[0],
				     codeinfo->o_kind[0] == '1' ? "매수" : "매도", codeinfo->o_mmgb[0] == '1' ? "시장가" : "지정가",
				     getString(codeinfo->o_num, sizeof(codeinfo->o_num)), getString(codeinfo->o_prc, sizeof(codeinfo->o_prc)));
			logS += logS_;

			for (int ii = 0; ii < 6; ii++)
			{
				if (ii == 0)
					logS_.Format("(%d:%c:%s:%s)", ii, codeinfo->alarm[ii].kind[0],
						     getString(codeinfo->alarm[ii].aval, sizeof(codeinfo->alarm[ii].aval)),
						     codeinfo->alarm[ii].cond[0] == '1' ? "이하" : "이상");
				else
					logS_.Format("(%d:%c:%s:%s:%s)", ii, codeinfo->alarm[ii].kind[0],
						     getString(codeinfo->alarm[ii].aval, sizeof(codeinfo->alarm[ii].aval)),
						     codeinfo->alarm[ii].cond[0] == '1' ? "이하" : "이상", codeinfo->alarm[ii].oper[0] == '1' ? "OR" : "AND");

				logS += logS_;
			}
			// writeLog(logS);
		}
	}

	querySise();
	return true;
}

bool CMapWnd::saveCatch(CString codeS /* = _T("")*/)
{
	bool bAllSave = false;
	if (codeS.IsEmpty())
		bAllSave = true;

	if (bAllSave)
		return true;

	// 해당종목 포착완료 저장
	CString cmpCodeS;
	UINT readL{};
	struct _codeinfo codeinfo;
	CFile fileH(m_sFilename, CFile::modeReadWrite);

	for (int ii = 0; fileH.m_hFile != CFile::hFileNull && ii < maxJONGMOK; ii++)
	{
		readL = fileH.Read((void *)&codeinfo, sz_codeinfo);
		if (readL < sz_codeinfo)
			break;

		cmpCodeS = getString(codeinfo.code, sizeof(codeinfo.code));
		if (cmpCodeS.GetLength() == 6)
		{
			if (cmpCodeS.GetAt(0) == '5' || cmpCodeS.GetAt(0) == '6')
				cmpCodeS.Insert(0, "J");
			else
				cmpCodeS.Insert(0, "A");
		}
		if (codeS.CompareNoCase(cmpCodeS))
			continue;

		codeinfo.capture[0] = '1';

		fileH.Seek(sz_codeinfo * ii, CFile::begin);
		fileH.Write((void *)&codeinfo, sz_codeinfo);
		break;
	}
	fileH.Close();
	return true;
}

CString CMapWnd::getString(char *datB, int datL)
{
	CString string = CString(datB, datL);
	string.TrimLeft();
	string.TrimRight();
	return string;
}

void CMapWnd::querySise()
{
	//  =============>   종목코드, 현재가, 대비, 등락률, 매도호가, 매수호가, 거래량
	const char *const symbols[] = {"1021", "2023", "2024", "2033", "2025", "2026", "2027"};
	CString tmpS;
	CString sendS = _T("");

	sendS.Format("%s%c", "$90303", 0x7f);

	struct _gridHi gridHi;
	memset(&gridHi, ' ', sizeof(struct _gridHi));

	CopyMemory(gridHi.visible, "99", sizeof(gridHi.visible));
	tmpS.Format("%04d", m_mapCatch.GetCount());
	CopyMemory(gridHi.rows, tmpS, sizeof(gridHi.rows));

	gridHi.type = '0';
	gridHi.dir = '1';
	gridHi.sort = '0';

	sendS += CString((char *)&gridHi, sizeof(struct _gridHi));

	for (int ii = 0; ii < 7; ii++)
	{
		tmpS.Format("%s%c", CString(symbols[ii]), 0x0A);
		sendS += tmpS;
	}

	tmpS.Format("%s%c", "1301", 0x7f); // 종목코드
	sendS += tmpS;

	CString codeS;
	struct _codeinfo *codeinfo = nullptr;

	for (POSITION pos = m_mapCatch.GetStartPosition(); pos != nullptr;)
	{
		m_mapCatch.GetNextAssoc(pos, codeS, codeinfo);
		if (!codeS.IsEmpty() && (codeS.GetAt(0) == 'A' || codeS.GetAt(0) == 'J'))
			codeS = codeS.Mid(1);

		tmpS.Format("%s%c", codeS, 0x7f);
		sendS += tmpS;
	}

	tmpS.Format("%c%c", 0x0A, 0x09);
	sendS += tmpS;
	TRACE("%s\n", sendS);

	//	Sleep(10);	// 응답없음 해결 20070523 by kwon
	sendTR("POOPPOOP", US_OOP | US_PASS, sendS, SC_SISE);
}

BOOL CMapWnd::sendTR(CString name, BYTE type, CString data, int key)
{
	CString trData = "";

	struct _userTH udat;

	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;

	trData += CString((char *)&udat, L_userTH);
	trData += data;

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)trData.operator LPCTSTR());
}

void CMapWnd::oubSise(char *buf, int bufL)
{
	// TRACE("oub[%.*s]\n", min(bufL, 300), buf);
}

void CMapWnd::oubHisLog(char *buf, int len)
{
	struct _limod *limod = (struct _limod *)buf;
	if (limod->retc[0] != '0')
		TRACE("oubHisLog[%c] ERROR\n", limod->retc[0]);
}


void CMapWnd::alertx(gsl::span<DWORD> data, struct _codeinfo* codeinfo)
{
	if (codeinfo->capture[0] == '1') // 시세포착 완료
		return;

	// TRACE("ALERT #2\n");
	struct _check check[] = {
	    {CURR, -1, 0},
	    {DIFF, -1, 0},
	    {MDGA, -1, 0},
	    {MSGA, -1, 0},
	    {UDYL, -1, 0},
	    {GVOL, -1, 0},
	};
	
	if (!data[check[0].sym])
		return;
	
	// 각각의 항목 check
	bool bMinus = false;
	double chkVal = 0.0;
	CString datS;
	for (int ii = 0; ii < sizeof(codeinfo->alarm) / sizeof(codeinfo->alarm[0]); ii++)
	{
		// TRACE("ALERT #4-1\n");
		if (codeinfo->alarm[ii].kind[0] == '0') // 감시항목 없음
			continue;

		if (!data[check[ii].sym])
			continue;

		datS = (char*)data[check[ii].sym];
		datS.Trim();

		if (check[ii].sym == DIFF && datS.GetLength() > 0)
		{
			switch (datS.GetAt(0))
			{
			case '4':
			case '5':
			case '-':
				bMinus = true;
				break;
			}
			datS = datS.Mid(1);
			if (bMinus)
				datS.Insert(0, '-');
		}
		
		check[ii].val = atof(datS);
		chkVal = atof(getString(codeinfo->alarm[ii].aval, sizeof(codeinfo->alarm[ii].aval)));
		switch (check[ii].sym)
		{
		case DIFF:
		case UDYL:
			break;
		default:
			check[ii].val = fabs(check[ii].val);
			chkVal = fabs(chkVal);
			break;
		}
		// TRACE("ALERT #4-3\n");
		if (codeinfo->alarm[ii].cond[0] == '0') // 이상
		{
			check[ii].chk = 0;
			if (check[ii].val >= chkVal)
				check[ii].chk = 1;
			/*
			switch (check[ii].sym)
			{
			case DIFF: case UDYL:
				if (check[ii].val >= chkVal)
					check[ii].chk = 1;
				break;
			default:
				if (fabs(check[ii].val) >= fabs(chkVal))
					check[ii].chk = 1;
				break;
			}
			*/
		}
		else // 이하
		{
			check[ii].chk = 0;
			if (check[ii].val <= chkVal)
				check[ii].chk = 1;
			/*
			switch (check[ii].sym)
			{
			case DIFF: case UDYL:
				if (check[ii].val <= chkVal)
					check[ii].chk = 1;
				break;
			default:
				if (fabs(check[ii].val) <= fabs(chkVal))
					check[ii].chk = 1;
				break;
			}
			*/
		}
	}

	CString codeS = codeinfo->code;
	
	char chk[6]{};
	bool bAlarm = false;
	memset(&chk, 0x00, sizeof(chk));
	for (int ii = 0, jj = 0; ii < sizeof(check) / sizeof(check[0]); ii++)
	{
		if (check[ii].chk == -1)
			continue;

		if (jj == 0)
		{
			bAlarm = check[ii].chk == 1 ? true : false;
			chk[ii] = bAlarm ? '1' : '0';
			jj++;
			continue;
		}

		if (codeinfo->alarm[ii].oper[0] == '0') // AND
			bAlarm &= (check[ii].chk == 1 ? true : false);
		else // OR
			bAlarm |= (check[ii].chk == 1 ? true : false);

		chk[ii] = bAlarm ? '1' : '0';
		jj++;
	}
	if (!bAlarm)
		return;
	// TRACE("ALERT #6\n");
	codeinfo->capture[0] = '1';
	saveCatch(codeS);
	// TRACE("ALERT #7\n");
	//  주문창 실행
	enum Gubun
	{
		None = 0,
		Stock,
		Future,
		Option
	};
	const Gubun gType = None;
	CString dominoD;

	if (codeinfo->o_run[0] & 0x01)
	{
		dominoD = SetOrderDominoData(codeinfo);
		// TRACE("ALERT #8\n");
		openView(typeVIEW, dominoD);
	}

	TRACE("STEP1\n");

	CString string;
	// 알람확인창
	if (codeinfo->a_kind[0] & 0x02) // 관심종목신호적용
	{
		string.Format("Alarm\t%s", codeS);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	}

	TRACE("STEP2\n");

	// 시세포착 설정 화면에서도 표시되도록
	string.Format("AlarmSetup\t%s", codeS);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	if (codeinfo->a_kind[0] & 0x01) // 확인창
	{
		CCaptureWnd* capture = new CCaptureWnd(this, m_pFont, m_pBFont);
		capture->m_pWizard = m_pParent;
		string = _T("");
		for (int ii = 0; ii < sizeof(check) / sizeof(check[0]); ii++)
		{
			switch (ii)
			{
			case 1: // sDIFF
				switch (gType)
				{
				case Stock:
					datS.Format("%.0f", check[ii].val);
					break;
				case Future:
				case Option:
					datS.Format("%.02f", check[ii].val);
					break;
				}
				break;
			case 4: // sRATE
				datS.Format("%.2f", check[ii].val);
				break;
			case 5:
				datS.Format("%.0f", check[ii].val);
				break;
			default:
				switch (gType)
				{
				case Stock:
					datS.Format("%.0f", check[ii].val);
					break;
				case Future:
				case Option:
					datS.Format("%.02f", check[ii].val);
					datS.Remove('-');
					datS.Remove('+');
					break;
				}
				break;
			}
			string += datS;
			string += "\t";
		}

		capture->CreatePopup((int)capture, (void*)codeinfo, (void*)string.operator LPCTSTR(), 0);
		m_mapCapture.SetAt((int)capture, capture);
	}

	TRACE("STEP3\n");

	if (codeinfo->a_kind[0] & 0x04) // 소리적용
	{
		string = getString(codeinfo->a_wave, sizeof(codeinfo->a_wave));
		if (!string.IsEmpty())
			sndPlaySound(string, SND_ASYNC);
	}

	TRACE("STEP4\n");

	CString currS, diffS, mdgaS, msgaS, rateS, gvolS;
	currS.Format("%.f", check[0].val);
	diffS.Format("%.f", check[1].val);
	mdgaS.Format("%.f", check[2].val);
	msgaS.Format("%.f", check[3].val);
	rateS.Format("%.2f", check[4].val);
	gvolS.Format("%.f", check[5].val);

	writeHistory(codeinfo, chk, currS, diffS, mdgaS, msgaS, rateS, gvolS);

	TRACE("STEP5\n");



}

void CMapWnd::alert(char *buf, int len)
{
	// TRACE("ALERT #1\n");
	CString tmpS = CString(buf);
	CString codeS = parseX(tmpS, "\t"), symS, datS;

	struct _codeinfo *codeinfo = nullptr;

	if (!m_mapCatch.Lookup(codeS, codeinfo))
		return;

	if (codeinfo->capture[0] == '1') // 시세포착 완료
		return;
	// TRACE("ALERT #2\n");
	struct _check check[] = {
	    {CURR, -1, 0},
	    {DIFF, -1, 0},
	    {MDGA, -1, 0},
	    {MSGA, -1, 0},
	    {UDYL, -1, 0},
	    {GVOL, -1, 0},
	};
	CMapStringToString mapRTS;
	int ii = 0, jj = 0;

	// RTS값 저장해 놓음
	mapRTS.RemoveAll();
	while (!tmpS.IsEmpty())
	{
		symS = parseX(tmpS, "\t");
		datS = parseX(tmpS, "\t");
		if (symS.IsEmpty())
			break;

		mapRTS.SetAt(symS, datS);
	}
	// TRACE("ALERT #3\n");
	if (!mapRTS.Lookup("023", datS)) // 반드시 체결이 들어와야.
		return;
	// 각각의 항목 check
	bool bMinus = false;
	double chkVal = 0.0;
	for (ii = 0; ii < sizeof(codeinfo->alarm) / sizeof(codeinfo->alarm[0]); ii++)
	{
		// TRACE("ALERT #4-1\n");
		if (codeinfo->alarm[ii].kind[0] == '0') // 감시항목 없음
			continue;

		symS.Format("%03d", check[ii].sym);
		if (!mapRTS.Lookup(symS, datS))
			continue;

		if (atoi(symS) == DIFF && datS.GetLength() > 0)
		{
			switch (datS.GetAt(0))
			{
			case '4':
			case '5':
			case '-':
				bMinus = true;
				break;
			}
			datS = datS.Mid(1);
			if (bMinus)
				datS.Insert(0, '-');
		}
		// TRACE("ALERT #4-2\n");
		check[ii].val = atof(datS);
		chkVal = atof(getString(codeinfo->alarm[ii].aval, sizeof(codeinfo->alarm[ii].aval)));
		switch (check[ii].sym)
		{
		case DIFF:
		case UDYL:
			break;
		default:
			check[ii].val = fabs(check[ii].val);
			chkVal = fabs(chkVal);
			break;
		}
		// TRACE("ALERT #4-3\n");
		if (codeinfo->alarm[ii].cond[0] == '0') // 이상
		{
			check[ii].chk = 0;
			if (check[ii].val >= chkVal)
				check[ii].chk = 1;
			/*
			switch (check[ii].sym)
			{
			case DIFF: case UDYL:
				if (check[ii].val >= chkVal)
					check[ii].chk = 1;
				break;
			default:
				if (fabs(check[ii].val) >= fabs(chkVal))
					check[ii].chk = 1;
				break;
			}
			*/
		}
		else // 이하
		{
			check[ii].chk = 0;
			if (check[ii].val <= chkVal)
				check[ii].chk = 1;
			/*
			switch (check[ii].sym)
			{
			case DIFF: case UDYL:
				if (check[ii].val <= chkVal)
					check[ii].chk = 1;
				break;
			default:
				if (fabs(check[ii].val) <= fabs(chkVal))
					check[ii].chk = 1;
				break;
			}
			*/
		}
	}
	mapRTS.RemoveAll();
	// TRACE("ALERT #5\n");
	//  Alaram 실행여부 결정
	char chk[6]{};
	bool bAlarm = false;
	memset(&chk, 0x00, sizeof(chk));
	for (ii = 0, jj = 0; ii < sizeof(check) / sizeof(check[0]); ii++)
	{
		if (check[ii].chk == -1)
			continue;

		if (jj == 0)
		{
			bAlarm = check[ii].chk == 1 ? true : false;
			chk[ii] = bAlarm ? '1' : '0';
			jj++;
			continue;
		}

		if (codeinfo->alarm[ii].oper[0] == '0') // AND
			bAlarm &= (check[ii].chk == 1 ? true : false);
		else // OR
			bAlarm |= (check[ii].chk == 1 ? true : false);

		chk[ii] = bAlarm ? '1' : '0';
		jj++;
	}
	if (!bAlarm)
		return;
	// TRACE("ALERT #6\n");
	codeinfo->capture[0] = '1';
	saveCatch(codeS);
	// TRACE("ALERT #7\n");
	//  주문창 실행
	enum Gubun
	{
		None = 0,
		Stock,
		Future,
		Option
	};
	const Gubun gType = None;
	CString dominoD;

	if (codeinfo->o_run[0] & 0x01)
	{
		dominoD = SetOrderDominoData(codeinfo);
		// TRACE("ALERT #8\n");
		openView(typeVIEW, dominoD);
	}

	TRACE("STEP1\n");

	CString string;
	// 알람확인창
	if (codeinfo->a_kind[0] & 0x02) // 관심종목신호적용
	{
		string.Format("Alarm\t%s", codeS);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	}

	TRACE("STEP2\n");

	// 시세포착 설정 화면에서도 표시되도록
	string.Format("AlarmSetup\t%s", codeS);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	if (codeinfo->a_kind[0] & 0x01) // 확인창
	{
		CCaptureWnd *capture = new CCaptureWnd(this, m_pFont, m_pBFont);
		;

		string = _T("");
		for (ii = 0; ii < sizeof(check) / sizeof(check[0]); ii++)
		{
			switch (ii)
			{
			case 1: // sDIFF
				switch (gType)
				{
				case Stock:
					datS.Format("%.0f", check[ii].val);
					break;
				case Future:
				case Option:
					datS.Format("%.02f", check[ii].val);
					break;
				}
				break;
			case 4: // sRATE
				datS.Format("%.2f", check[ii].val);
				break;
			case 5:
				datS.Format("%.0f", check[ii].val);
				break;
			default:
				switch (gType)
				{
				case Stock:
					datS.Format("%.0f", check[ii].val);
					break;
				case Future:
				case Option:
					datS.Format("%.02f", check[ii].val);
					datS.Remove('-');
					datS.Remove('+');
					break;
				}
				break;
			}
			string += datS;
			string += "\t";
		}

		capture->CreatePopup((int)capture, (void *)codeinfo, (void *)string.operator LPCTSTR(), 0);
		m_mapCapture.SetAt((int)capture, capture);
	}

	TRACE("STEP3\n");

	if (codeinfo->a_kind[0] & 0x04) // 소리적용
	{
		string = getString(codeinfo->a_wave, sizeof(codeinfo->a_wave));
		if (!string.IsEmpty())
			sndPlaySound(string, SND_ASYNC);
	}

	TRACE("STEP4\n");

	CString currS, diffS, mdgaS, msgaS, rateS, gvolS;
	currS.Format("%.f", check[0].val);
	diffS.Format("%.f", check[1].val);
	mdgaS.Format("%.f", check[2].val);
	msgaS.Format("%.f", check[3].val);
	rateS.Format("%.2f", check[4].val);
	gvolS.Format("%.f", check[5].val);

	writeHistory(codeinfo, chk, currS, diffS, mdgaS, msgaS, rateS, gvolS);

	TRACE("STEP5\n");
}

//////////////////////////////////////////////////////////////////////////
// 요건 번호   : 0014
// 요건 제목   : 시세포착
// 요건 요청자 : 김진아
// 요건 내용   : 주문실행에서 설정된 데이터를 만드는 기능
// 수정 일자   : 20090513
// 수정자      : kbkim
//////////////////////////////////////////////////////////////////////////
//+
CString CMapWnd::SetOrderDominoData(_codeinfo *codeinfo)
{
	CString sDominoData = ""; // 화면명 + {심블명 + \t + 데이터 + /n}
	CString sMapName = "";
	CString sSymbolName = "";
	CString sSymbolData = "";
	CString sCtrlData = "";
	CString sOrderType = "";
	CString sOrderCount = "";
	CString sOrderCost = "";
	CString p_code, p_qty, p_prc, p_no, p_mmgb;

#if 0
	if(codeinfo->o_kind[0] == '1') // 매수
		sMapName    = "IB101100";
	else        // 매도
		sMapName    = "IB101200";
	p_code = CString(codeinfo->code, 6); p_code.TrimRight();
#else
	p_code = CString(codeinfo->code, sizeof(codeinfo->code));
	p_code.TrimRight();
	CString tmp, sMap, symbol;
	if (p_code.GetLength() == 6) // 주식
	{
		sMapName = (codeinfo->o_kind[0] == '1') ? "IB101200" : "IB101100";
		sMap     = (codeinfo->o_kind[0] == '1')     ? "1012" : "1011";
		tmp.Format("selectdataA\t%s", sMap);
		symbol   = "1301";
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, pushCC), (LPARAM)(LPCSTR)tmp);
	}
	else if (p_code.GetLength() == 8) // 선물/옵션
	{
		sMapName = (codeinfo->o_kind[0] == '1') ? "IB301200" : "IB301100";
		sMap     = (codeinfo->o_kind[0] == '1') ? "3012" : "3011";
		tmp.Format("selectdataA\t%s", sMap);
		symbol   = "ed_focod";
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, pushCC), (LPARAM)(LPCSTR)tmp);
	}
#endif

	p_qty = CString(codeinfo->o_num, 10);
  	p_qty.TrimRight(); 
	p_prc = CString(codeinfo->o_prc, 10);
	p_prc.TrimRight();
	p_mmgb = CString(codeinfo->o_mmgb, 1);
	p_mmgb.TrimRight();
	p_no = "0";

	int gubn = atoi(CString(codeinfo->o_kind[0])) + 1;	
	sDominoData.Format("%s/S/d%s\t%s\ned_jprc\t%s\ned_jqty\t%s\n%s\t%d\n", sMapName, symbol, p_code, p_prc, p_qty, "order_tab", gubn);
	return sDominoData;
}


int CMapWnd::openView(int type, CString data)
{
	// return 0;
	int ret = 0;
	TRACE("openView %d |%s|", type, (LPCSTR)data);
	try
	{
		ret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCSTR)data);
	}
	catch (...)
	{
	}
	return 0;
}

void CMapWnd::trigger(CString tmpS)
{
	if (tmpS.IsEmpty())
		return;
	CString symS = parseX(tmpS, "\t");
	tmpS.TrimLeft();
	if (symS.Find("OnPortfolio") != -1 && tmpS == "ok")
	{
		// writeLog("관심종목 설정 close");
		loadCatch(false);
	}
	else if (symS.Find("OnCatchChange") != -1 && tmpS == "ok")
	{
		writeLog("시세포착 설정 close");
		loadCatch(true);
	}
}

void CMapWnd::writeLog(char *format, ...)
{
#ifndef WRITELOG
	return;
#endif
	CString tmpS;
	va_list valist;
	va_start(valist, format);
	tmpS.FormatV(format, valist);
	va_end(valist);
	writeLog(tmpS);
}

void CMapWnd::writeLog(CString tmpS)
{
#ifndef WRITELOG
	return;
#endif
	/*
		CFile	wfile;

		BOOL bExist = wfile.Open(m_sLogFile, CFile::modeReadWrite);
		if (!bExist)
		{
			wfile.Open(m_sLogFile, CFile::modeCreate|CFile::modeReadWrite);
			wfile.Write((char *)(LPCTSTR)tmpS_, tmpS_.GetLength());
		}
		else
		{
			wfile.SeekToEnd();
			wfile.Write((char *)(LPCTSTR)tmpS_, tmpS_.GetLength());
		}
		wfile.Close();
	*/
	LPBYTE buffer = new BYTE[sz_limid + tmpS.GetLength()];
	_limid *mid = (_limid *)buffer;

	mid->func[0] = '1';
	memcpy(mid->mapx, MAPNAME, sizeof(mid->mapx));
	mid->kind[0] = lkCatch;
	memcpy(mid->data, (LPCTSTR)tmpS, tmpS.GetLength());

	sendTR(TR_LOGIO, (LPSTR)buffer, sz_limid + tmpS.GetLength() - 1, SC_LOG, US_PASS);

	delete[] buffer;
}

void CMapWnd::deleteLog()
{
#ifndef WRITELOG
	return;
#endif
	CFile wfile;
	const BOOL bExist = wfile.Open(m_sLogFile, CFile::modeRead);
	if (bExist)
	{
		if (wfile.GetLength() >= 1024 * 200)
		{
			wfile.Close();
			DeleteFile(m_sLogFile);
		}
	}
}
void CMapWnd::writeHistory(_codeinfo *codeinfo, char chk[6], CString currS, CString diffS, CString mdgaS, CString msgaS, CString rateS, CString gvolS)
{
#ifndef WRITELOG
	return;
#endif
	struct _hisinfo hisinfo;
	memset(&hisinfo, ' ', sz_hisinfo);

	CString tmpS;
	const CTime time = CTime::GetCurrentTime();
	tmpS.Format("%04d%02d%02d%02d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	memcpy(&hisinfo.date, tmpS, tmpS.GetLength());
	memcpy(&hisinfo.codeinfo, codeinfo, sz_codeinfo);
	memcpy(&hisinfo.gval[0], currS, currS.GetLength());
	memcpy(&hisinfo.gval[1], diffS, diffS.GetLength());
	memcpy(&hisinfo.gval[2], mdgaS, mdgaS.GetLength());
	memcpy(&hisinfo.gval[3], msgaS, msgaS.GetLength());
	memcpy(&hisinfo.gval[4], rateS, rateS.GetLength());
	memcpy(&hisinfo.gval[5], gvolS, gvolS.GetLength());
	memcpy(&hisinfo.chk, chk, sizeof(hisinfo.chk));

	if (hisinfo.codeinfo.a_kind[0] == 0x00) // log시 문제가 생김
		hisinfo.codeinfo.a_kind[0] = ' ';
	/*
	CFile	wfile;
	BOOL bExist = wfile.Open(m_sHisFile, CFile::modeReadWrite);
	if (!bExist)
	{
		wfile.Open(m_sHisFile, CFile::modeCreate|CFile::modeReadWrite);
		wfile.Write((char *)&hisinfo, sz_hisinfo);
	}
	else
	{
		wfile.SeekToEnd();
		wfile.Write((char *)&hisinfo, sz_hisinfo);
	}
	wfile.Close();
	*/
	const int len = sizeof(sz_limid) + sz_hisinfo;
	LPBYTE buffer = new BYTE[len];

	memset(buffer, ' ', len);

	_limid *mid = (_limid *)buffer;

	mid->func[0] = '1'; // 입력
	memcpy(mid->mapx, MAPNAME, sizeof(mid->mapx));

	mid->kind[0] = lkHis;
	memcpy(mid->data, &hisinfo, sz_hisinfo);

	sendTR(TR_LOGIO, (char *)buffer, len - 1, SC_LOG, 0);

	delete[] buffer;
	/**
	_limid limid;
	memset(&limid, ' ', sz_limid);

	limid.func[0] = '1';	// 입력
	memcpy(&limid.mapx, MAPNAME, sizeof(limid.mapx));	// main map name

	limid.kind[0] = lkHis;
	memcpy(&limid.data, &hisinfo, sz_hisinfo);		// log

	sendTR(TR_LOGIO, (char *)&limid, sz_limid, SC_LOG, 0);
	**/
}

void CMapWnd::sendTR(CString trC, char *pBytes, int nBytes, int key, BYTE stat)
{
	if (!m_pParent)
		return;

	const int len = L_userTH + nBytes;
	auto sndB = std::make_unique<char[]>(len + 1);
	struct _userTH *udat = (struct _userTH *)sndB.get();

	//	ZeroMemory(sndB, sizeof(sndB));

	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = key;
	udat->stat = stat;

	CopyMemory(&sndB[L_userTH], pBytes, nBytes);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)sndB.get());
	// delete []sndB;
	// sndB = nullptr;
}

void CMapWnd::OnTimer(UINT nIDEvent)
{
	KillTimer(nIDEvent);

	loadCatch(true);

	CWnd::OnTimer(nIDEvent);
}
