// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Symbol.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"
#include "sk_pc_identity.h"
#include "../../H/interMSG.h"
//#include "../../../axis/axMsg.hxx"
#include "../../AXIS/axMsg.hxx"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int indicatorW = 11;
const int bitmapW = 30;

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

#define OOP_REQ_17414		245

CControlWnd::CControlWnd()
	: m_pTBmp(nullptr)
	, m_pBBmp(nullptr)
	, m_pRBmp(nullptr)
	, m_pTipCtrl(nullptr)
{
	EnableAutomation();

	m_capture = false;
	m_bCheck = false;
	m_pPopup = nullptr;
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bMouseOver = false;
	m_sData = "";

	m_sCode = _T("");
	m_ujongsise = false;
	m_memostate = MEMO_NOT_YET;
//	m_bitmap = NULL;

	m_bFromDLL = FALSE;
	m_bSideTime = FALSE;

	m_sMkMsg.Empty();
}

CControlWnd::~CControlWnd()
{
	if (m_pPopup)
	{
		if (IsWindow(m_pPopup->GetSafeHwnd()))
			m_pPopup->SendMessage(WM_CLOSE);
		m_pPopup->Detach();
		m_pPopup.reset();
	}
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
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "visible", m_visible, OnVisibleChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CControlWnd, "sInfo", m_sInfo, OnSInfoChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "sMkMsg", m_sMkMsg, OnSMkMsgChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetSideTime", SetSideTime, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "GetCodeSymbolType", GetCodeSymbolType, VT_BSTR, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {AA69DFD8-D09A-4295-8B0B-0AC78A214F10}
static const IID IID_IControlWnd =
{ 0xaa69dfd8, 0xd09a, 0x4295, { 0x8b, 0xb, 0xa, 0xc7, 0x8a, 0x21, 0x4f, 0x10 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

void CControlWnd::load_jinfo(char* pData)
{
	CString strTemp;
	FILE	*fp = fopen("jinfo.txt", "w");

	fprintf(fp, "%s", pData);
	fclose(fp);

	//2013.06.14 KSJ 매매단위, 지정자문인 심볼추가
	strTemp.Format("%s", pData);
	strTemp = strTemp.Right(strTemp.GetLength() - L_jinfo);
	strTemp.Replace("\t", "");
	m_jinfo.str1377.Format("%d", atoi(strTemp));
	m_jinfo.str1971 = strTemp.Mid(m_jinfo.str1377.GetLength());
	//KSJ

	struct	_jinfo	jinfo{};
	CopyMemory(&jinfo, pData, L_jinfo);	
	m_jinfo.codx = CString(jinfo.codx, sizeof(jinfo.codx));
	m_jinfo.hnam = CString(jinfo.hnam, sizeof(jinfo.hnam));
	m_jinfo.jgub = CString(jinfo.jgub, sizeof(jinfo.jgub));
	m_jinfo.size = CString(jinfo.size, sizeof(jinfo.size));
	m_jinfo.k2gb = CString(jinfo.k2gb, sizeof(jinfo.k2gb));
	m_jinfo.usun = CString(jinfo.usun, sizeof(jinfo.usun));
	m_jinfo.ksmm = CString(jinfo.ksmm, sizeof(jinfo.ksmm));
	m_jinfo.amga = CString(jinfo.amga, sizeof(jinfo.amga));
	m_jinfo.jbkm = CString(jinfo.jbkm, sizeof(jinfo.jbkm));
	m_jinfo.siga = CString(jinfo.siga, sizeof(jinfo.siga));
	m_jinfo.sjjs = CString(jinfo.sjjs, sizeof(jinfo.sjjs));
	m_jinfo.jrat = CString(jinfo.jrat, sizeof(jinfo.jrat));
	m_jinfo.nrat = CString(jinfo.nrat, sizeof(jinfo.nrat));	// 20070718
	

	// extended infomation
	m_jinfo.krgb = CString(jinfo.krgb, sizeof(jinfo.krgb));
	m_sSearchkrgb = m_jinfo.krgb;
	m_sSearchkrgb.TrimRight();
	m_jinfo.jggb = CString(jinfo.jggb, sizeof(jinfo.jggb));
	m_jinfo.lock = CString(jinfo.lock, sizeof(jinfo.lock));
	m_jinfo.dyga = CString(jinfo.dyga, sizeof(jinfo.dyga));
	m_jinfo.ujgb = CString(jinfo.ujgb, sizeof(jinfo.ujgb));
	m_jinfo.ujcd = CString(jinfo.ujcd, sizeof(jinfo.ujcd));
	m_jinfo.jisu = CString(jinfo.jisu, sizeof(jinfo.jisu));
	m_jinfo.diff = CString(jinfo.diff, sizeof(jinfo.diff));
	m_jinfo.rate = CString(jinfo.rate, sizeof(jinfo.rate));
	m_jinfo.gvol = CString(jinfo.gvol, sizeof(jinfo.gvol));
	m_jinfo.unio = CString(jinfo.unio, sizeof(jinfo.unio));
	m_jinfo.frgb = CString(jinfo.frgb, sizeof(jinfo.frgb));
	m_jinfo.rlnk = CString(jinfo.rlnk, sizeof(jinfo.rlnk));
	m_jinfo.rned = CString(jinfo.rned, sizeof(jinfo.rned));
	m_jinfo.dist = CString(jinfo.dist, sizeof(jinfo.dist));
	m_jinfo.vist = CString(jinfo.vist, sizeof(jinfo.vist));
	m_jinfo.vitime = CString(jinfo.vitime, sizeof(jinfo.vitime));
	m_jinfo.vitype = CString(jinfo.vitype, sizeof(jinfo.vitype));
	m_jinfo.sijang = CString(jinfo.sijang, sizeof(jinfo.sijang));

	CString strCode = m_jinfo.codx;
	strCode.TrimRight();
	m_sInfo = m_jinfo.krgb;
	m_sInfo.TrimRight();

	m_sCode = m_jinfo.codx; m_sCode.Trim();
	m_sRtsCode = m_sCode;
	//m_sCode.Replace("N.", "");
	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->1", __FUNCTION__, __LINE__, "load_jinfo");
	m_slog.Trim();
	//OutputDebugString(m_slog);

	m_slog.Format("\r\n[cx_symbol][%-40s][%d][%-35s]-->1", __FUNCTION__, __LINE__, m_sRtsCode);
	m_slog.Trim();
	OutputDebugString(m_slog);

	checkMemo(m_sCode);

	loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);

	m_jinfo.hnam.TrimRight();

	m_sData = m_jinfo.hnam;

	m_jinfo.jisu.Replace("+", ""); m_jinfo.jisu.Replace("-", "");
	Invalidate();
}

void CControlWnd::load_jinfo3(char* pData)
{//종목콤보 연동이 트리거만 오면 메도여부확인 하지만 메모창이 떠있는상태에서는 메모창 세부 내역도 조회하는 방식으로
	CString  stmp;
	struct	_jinfo3 jinfo {};

	CopyMemory(&jinfo, pData, L_jinfo3);	

	m_jinfo.codx = CString(jinfo.codx, sizeof(jinfo.codx));
	m_jinfo.hnam = CString(jinfo.hnam, sizeof(jinfo.hnam));
	m_jinfo.jgub = CString(jinfo.jgub, sizeof(jinfo.jgub));
	m_jinfo.jrat = CString(jinfo.jrat, sizeof(jinfo.jrat));
	m_jinfo.nrat = CString(jinfo.nrat, sizeof(jinfo.nrat));	// 20070718

m_slog.Format("[cx_symbol][%s]<%d>  m_jinfo.codx = [%s]  m_jinfo.hnam =[%s]", __FUNCTION__, __LINE__, m_jinfo.codx, m_jinfo.hnam);
OutputDebugString(m_slog);

	// extended infomation
	m_jinfo.krgb = CString(jinfo.krgb, sizeof(jinfo.krgb));	
	m_sSearchkrgb = m_jinfo.krgb;
	m_sSearchkrgb.TrimRight();
	m_jinfo.rned = CString(jinfo.rned, sizeof(jinfo.rned));
	m_jinfo.dist = CString(jinfo.dist, sizeof(jinfo.dist));
	m_jinfo.vist = CString(jinfo.vist, sizeof(jinfo.vist));
	m_jinfo.vitime = CString(jinfo.vitime, sizeof(jinfo.vitime));
	m_jinfo.vitype = CString(jinfo.vitype, sizeof(jinfo.vitype));
	m_sMkMsg = CString(jinfo.fill, 1);

	m_sCode = m_jinfo.codx; m_sCode.Trim();
	m_sRtsCode = m_sCode;

	m_slog.Format("\r\n[cx_symbol][%-40s][%d][%-35s]-->1", __FUNCTION__, __LINE__, m_sRtsCode);
	m_slog.Trim();
	OutputDebugString(m_slog);

	m_sInfo = m_jinfo.krgb;
	m_sInfo.TrimRight();
	loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
	m_jinfo.hnam.TrimRight();
	m_sData = m_jinfo.hnam;

	if (m_pPopup && m_pPopup.get()->IsWindowVisible())  //here  화면이 떠 있을때 메모팝업창 내부 내용 조회
	{
		CString tmpCode{};
		char	szBuf[64]{};
		int	datL = 0;
		struct	_jinfo3 jinfo {};
		CopyMemory(&jinfo, pData, L_jinfo3);

		tmpCode = m_sCode;
		tmpCode.Replace("N.", "");
		tmpCode.Replace("M.", "");
		m_capture = false;
		ReleaseCapture();
		datL = sprintf(szBuf, "1301%c%s\t1777%c%d\t17414\t1377\t1971\t", 0x7f, (char*)m_sCode.GetString(), 0x7f, GetMKgubn(m_sCode));	//2013.06.14 KSJ 매매단위, 지정자문인 심볼추가
		if (m_bFromDLL)
			SendTR_Dll(szBuf, datL);
		else
			SendTR_Control("POOPPOOP", szBuf, datL, US_OOP | US_PASS);
	}
	else
	{
		CString  stmp;
		m_memostate = MEMO_NOT_YET;
		m_slog.Format("\r\n[memo][%s]<%d>  m_sCode=[%s]", __FUNCTION__, __LINE__, m_sCode);
		m_slog.Trim();
		OutputDebugString(m_slog);
		checkMemo(m_sCode);
	}
	Invalidate();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int key{};
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)m_rtnStr.GetString();

	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;

		m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> key=[%s] len=[%d] ",
			__FUNCTION__, __LINE__, "DLL_OUBx_size", GetMemoTrKeyType(exth->key), exth->size);
		m_slog.Trim();
		OutputDebugString(m_slog);


		if (exth->key == TRKEY_MEMO_INSERT || exth->key == TRKEY_MEMO_UPDATE)
		{
			mod_memo* poub = (mod_memo*)exth->data;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->  ret=[%c]  msg=[%.64s] ",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key), chRet, poub->chemsg);
			m_slog.Trim();
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s]  ", __FUNCTION__, __LINE__, "save or update", poub->chcode);
				m_slog.Trim();
				OutputDebugString(m_slog);
				m_memostate = MEMO_EXIST;
				if (m_pPopup)
				{
				//	m_pPopup->saveMemo(CString(poub->chcode, 16).Trim());   //컨트롤 서버 메모추가,변경
					m_pPopup->removeMemoInMemoFile(CString(poub->chcode, 16).Trim());
				}

			}
			else
			{
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->[%s]  ", __FUNCTION__, __LINE__, "save or update",
					exth->key == TRKEY_MEMO_INSERT ? "세이브 실패" : "업데이트 실패");
				OutputDebugString(m_slog);
			}
			Invalidate();
		}
		else if (exth->key == TRKEY_MEMO_SEARCH)
		{
			mod_memo* poub = (mod_memo*)exth->data;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->ret=[%c] len=[%d] msg=[%.64s] ",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", chRet, exth->size, poub->chemsg);
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				CString sMemo{};
				sMemo.Format("%s", poub->chmemo);

				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] memo=[%s]",
					__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, CString(poub->chmemo, 64).Trim());
				OutputDebugString(m_slog);

				if (m_pPopup)
				{
					sMemo = sMemo.Left(sizeof(poub->chmemo));
					sMemo.Trim();
					m_pPopup->m_mode = m_pPopup->memo_update;
					m_pPopup->m_pEdit->SetWindowText(sMemo);
					m_pPopup->m_pEdit->SetSel(0, -1);
				}
				m_memostate = MEMO_EXIST;
				Invalidate();
			}
			else
			{
				if (m_pPopup) //조회 내역이 없다면 파일은 본다
				{
					m_memostate = MEMO_NOEXSIST;
					Invalidate();
					if (!m_pPopup->m_sKey.IsEmpty())
						m_pPopup->loadMemo(m_pPopup->m_sKey);     //컨트롤 서버메모 조회후 내역없다고 해서 파일조회
				}
			}
		}
		else if (exth->key == TRKEY_MEMO_CHECK)
		{
			st_mod_SDEmemo* poub = (st_mod_SDEmemo*)exth->data;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->ret=[%c]   msg=[%.64s] ", __FUNCTION__, __LINE__,
				GetMemoTrKeyType(exth->key) + " dll_oubx", chRet, (char*)exth->data);
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] 메모 서버 존재",
					__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode);
				OutputDebugString(m_slog);
				m_memostate = MEMO_EXIST;
			}
			else
			{
				m_memostate = checkFileMemo(CString(poub->chcode, 16).Trim());    // 컨트롤 메모 서버없어서  파일 체크
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] 메모 서버없어서  파일 존재 체크 = [%s]",
					__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, m_memostate == 1 ? "파일은있음" : "파일도없음");
				OutputDebugString(m_slog);
			}

			Invalidate();
		}
		else if (exth->key == TRKEY_MEMO_DELETE)
		{
			st_mod_SDEmemo* poub = (st_mod_SDEmemo*)exth->data;
			const char chRet = poub->chretc[0];

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] ret=[%c]",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, chRet);
			OutputDebugString(m_slog);
			m_memostate = MEMO_EXIST;

			//if (chRet == '1')
			{
				m_memostate = MEMO_NOEXSIST;
				if (m_pPopup)
				{
					m_pPopup->saveMemo(CString(poub->chcode, 16).Trim());   //컨트롤 서버 메모 삭제
					m_pPopup->removeMemoInMemoFile(CString(poub->chcode, 16).Trim());
				}

				m_memostate = checkFileMemo(CString(poub->chcode, 16).Trim());   //컨트롤 메모 서버삭제 후  파일 체크
				Invalidate();
			}
		}
		//else if (exth->size >= L_jinfo)   //here   TRKEY_MEMO_POPUPOOP 긴거 300
		else if (exth->key == TRKEY_MEMO_POPUPOOP)
		{
			load_jinfo(exth->data);  //메모 팝업창에 필요한 데이터 세팅

			CRect	rc;

			GetClientRect(&rc);
			if (!m_sData.IsEmpty() && rc.PtInRect(m_ptUp))
			{
				GetWindowRect(&rc);
				if (m_pPopup)
					m_pPopup.reset();

				if (!m_pPopup)
				{
					int	cx = 225, cy = 292;//m_ujongsise ? 190 : 150;	//KSJ 2012.05.04 매매단위 추가되어 250-> 272로 수정함
					if (m_jinfo.jgub.GetAt(0) == 'A')//2013.06.11 KSJ 코넥스 추가
					{
						cx = 225;
						cy = 314;
					}

					CString sClassName = AfxRegisterWndClass(0);

					m_pPopup = std::make_unique<CInfoPopup>();

					m_pPopup.get()->m_pParent = this;

					m_pPopup->m_pWizard = m_pParent;
					if (!m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
						WS_POPUP | WS_BORDER, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
						NULL, NULL, NULL))
					{
						m_pPopup = NULL;
						return 0;
					}
					else
					{
						//IsNXTAble
						m_jinfo.bNXT = IsNXTAble(m_sCode);
						m_pPopup->setJinfo(m_jinfo, m_ujongsise);
						m_pPopup->setPathInfo(m_sRoot, m_sUserID);
						m_pPopup->ShowWindow(SW_SHOWNORMAL);
						m_pPopup->Invalidate();
					}
				}
			}

			m_memostate = MEMO_NOT_YET;
			m_slog.Format("\r\n[memo][%-40s][%d][%-30s] --> 3 sCode=[%s]", __FUNCTION__, __LINE__, "MEMO_NOT_YET", m_sCode);
			m_slog.Trim();
			OutputDebugString(m_slog);
			checkMemo(m_sCode); Invalidate();
		}
		else if (exth->size >= L_jinfo3)
		{ //다른 화면에서 조회한 결과가 여기로 넘어옴 짧은거 105  긴거 342
			int isize = L_jinfo3;
			isize = L_jinfo;

			m_slog.Format("[cx_symbol][%s]<%d>  데이터 크기 size = [%d]  L_jinfo3=[%d] ", __FUNCTION__, __LINE__,
				exth->size, L_jinfo3);
			OutputDebugString(m_slog);

			load_jinfo3(exth->data);
		}
		else
		{
		m_slog.Format("[cx_symbol][%s]<%d>  데이터 크기  크기 = [%d]  L_jinfo3=[%d] ", __FUNCTION__, __LINE__,
			exth->size, L_jinfo3);
		//GetTopLevelParent()->SendMessage(WM_USER, MMSG_SHARED_GUIDEMESSAGE, (LPARAM)(LPSTR)(LPCTSTR)m_slog);
			
		CString strINI;
		strINI.Format("%s\\User\\%s\\Error.ini", m_sRoot, m_sUserID);
		WritePrivateProfileString("cx_symbol", "sizeError", m_slog, strINI);

			m_sData.Empty();
			m_pTBmp = NULL;
			m_pBBmp = NULL;
			m_pRBmp = NULL;
			Invalidate();
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		key = LOWORD(HIBYTE(wParam));
		m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->key=[%s] len=[%d] [%.50s] ", __FUNCTION__, __LINE__, "DLL_OUB",
			GetMemoTrKeyType(key), HIWORD(wParam), (char*)lParam);
		m_slog.Trim();
		OutputDebugString(m_slog);


		if (key == TRKEY_MEMO_INSERT || key == TRKEY_MEMO_UPDATE)
		{
			mod_memo* poub = (mod_memo*)lParam;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->ret=[%c]  msg=[%.64s]",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(key),
				chRet, poub->chemsg);
			m_slog.Trim();
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s]  ",
					__FUNCTION__, __LINE__, "dll_oub save or update 성공",
					poub->chcode);
				m_slog.Trim();
				OutputDebugString(m_slog);
				m_memostate = MEMO_EXIST;
				if (m_pPopup)
				{
				//	m_pPopup->saveMemo(CString(poub->chcode, 16).Trim());   //DLL 화면 서버 메모추가,변경
					m_pPopup->removeMemoInMemoFile(CString(poub->chcode, 16).Trim());
				}

			}
			else
			{
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->  [%s]  ", __FUNCTION__, __LINE__, "dll_oub save or update",
					key == TRKEY_MEMO_INSERT ? "세이브 실패" : "업데이트 실패");
				OutputDebugString(m_slog);
			}
			Invalidate();
		}
		else if (key == TRKEY_MEMO_SEARCH)
		{
			mod_memo* poub = (mod_memo*)lParam;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> ret=[%c]  len=[%d] msg=[%.64s] ",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(key),
				chRet, HIWORD(wParam), poub->chemsg);
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				CString sMemo{};
				sMemo.Format("%.9999s", poub->chmemo);
				sMemo.Trim();

				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] memo=[%s] ",
					__FUNCTION__, __LINE__, "dll_oub load TRKEY_MEMO_SEARCH", poub->chcode, sMemo.Left(16));
				OutputDebugString(m_slog);

				if (m_pPopup)
				{
					sMemo = sMemo.Left(sizeof(poub->chmemo));
					sMemo.Trim();
					m_pPopup->m_mode = m_pPopup->memo_update;
					m_pPopup->m_pEdit->SetWindowText(sMemo);
					m_pPopup->m_pEdit->SetSel(0, -1);
				}
				m_memostate = MEMO_EXIST;
				Invalidate();
			}
			else
			{
				if (m_pPopup) //조회 내역이 없다면 파일은 본다
				{
					//m_pPopup->m_mode = m_pPopup->memo_non;
					if (!m_pPopup->m_sKey.IsEmpty())
						m_pPopup->loadMemo(m_pPopup->m_sKey);  //DLL화면 서버메모 조회후 내역없다고 해서 파일조회
					Invalidate();
				}
			}
		}
		else if (key == TRKEY_MEMO_CHECK)
		{
			mod_memo* poub = (mod_memo*)lParam;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> ret=[%c]  msg=[%.64s] ", __FUNCTION__, __LINE__, GetMemoTrKeyType(key) + " DLL_OUB",
				chRet, poub->chemsg);
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] 메모 서버 존재",
					__FUNCTION__, __LINE__, "dll_oub TRKEY_MEMO_CHECK", poub->chcode);
				OutputDebugString(m_slog);
				m_memostate = MEMO_EXIST;
			}
			else
			{
				m_memostate = checkFileMemo(CString(poub->chcode, 16).Trim());   //dll화면 메모 서버없어서  파일 체크
				m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%.16s] 메모 서버없어서  파일 존재 체크 = [%s]",
					__FUNCTION__, __LINE__, "dll_oub TRKEY_MEMO_CHECK", poub->chcode, m_memostate == 1 ? "파일은있음" : "파일도없음");
				OutputDebugString(m_slog);
			}
			Invalidate();
		}
		else if (key == TRKEY_MEMO_DELETE)
		{
			st_mod_SDEmemo* poub = (st_mod_SDEmemo*)lParam;

			const char chRet = poub->chretc[0];
			m_memostate = MEMO_NOEXSIST;

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->len=[%d]  ret=[%c]",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(key) + " DLL_OUB", HIWORD(wParam), chRet);
			OutputDebugString(m_slog);

			//if (chRet == '1')
			{
				if (m_pPopup)
				{
					m_pPopup->saveMemo(CString(poub->chcode, 16).Trim());   //DLL 화면 서버 삭제
					m_pPopup->removeMemoInMemoFile(CString(poub->chcode, 16).Trim());
				}

				m_memostate = checkFileMemo(CString(poub->chcode, 16).Trim());  //dll화면 메모 서버삭제 후  파일 체크
				Invalidate();
			}
		}
		else if (key == TRKEY_MEMO_POPUPOOP)
		{
			oubMemoPopup((char*)lParam);
		}
		else if (HIWORD(wParam) >= L_jinfo && m_trKey > -1) //dll_oub  //2016 복수종목 //클릭메모팝업
		{
			oubMemoPopup((char*)lParam);
		}
		else if (HIWORD(wParam) >= L_jinfo && !m_bServerMode) //dll_oub  //클릭메모팝업 //메모관련 메시지 분류 안한 기존모듈들
		{
			oubMemoPopup((char*)lParam);
		}
		else if (HIWORD(wParam) >= L_jinfo3)  //종목트리거로 올때
	//	else if (HIWORD(wParam) == L_jinfo3)
		{
			load_jinfo3((char*)lParam);
		}
		else
		{
			m_sData.Empty();
			m_pTBmp = NULL;
			m_pBBmp = NULL;
			m_pRBmp = NULL;
			Invalidate();
		}
		break;

//	case DLL_ALERT:
//		WriteAlert((char*)lParam);
//		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		
		alertR = (struct _alertR*)lParam;
		parseAlert(alertR);
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		Invalidate();
		break;
	
	case DLL_SETFONT:
		{
			CRect	rc;
			CString tmp = CString((char*)lParam);
			if (!tmp.IsEmpty())
				m_Param.fonts = tmp;
			Invalidate();
			
			GetClientRect(&rc);
			m_Param.point = HIWORD(wParam);
			if (rc.Height() == 20)
				m_Param.point = 9;
		}
		break;
	}

	return 0;
}

void CControlWnd::oubMemoPopup(char* pdata)
{
	load_jinfo(pdata);

	CRect	rc;

	GetClientRect(&rc);
	if (!m_sData.IsEmpty() && rc.PtInRect(m_ptUp))
	{
		GetWindowRect(&rc);
		if (m_pPopup)
			m_pPopup.reset();

		if (!m_pPopup)
		{
			int	cx = 225, cy = 292;//m_ujongsise ? 190 : 150;	//KSJ 2012.05.04 매매단위 추가되어 250-> 272로 수정함
			if (m_jinfo.jgub.GetAt(0) == 'A')//2013.06.11 KSJ 코넥스 추가
			{
				cx = 225;
				cy = 314;
			}

			CString sClassName = AfxRegisterWndClass(0);

			m_pPopup = std::make_unique<CInfoPopup>();

			m_pPopup.get()->m_pParent = this;

			m_pPopup->m_pWizard = m_pParent;
			if (!m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, sClassName, NULL,
				WS_POPUP | WS_BORDER, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
				NULL, NULL, NULL))
			{
				m_pPopup = NULL;
				return;
			}
			else
			{
				m_jinfo.bNXT = IsNXTAble(m_sCode);
				m_pPopup->setJinfo(m_jinfo, m_ujongsise);
				m_pPopup->setPathInfo(m_sRoot, m_sUserID);
				m_pPopup->ShowWindow(SW_SHOWNORMAL);
				m_pPopup->Invalidate();
			}
		}
	}

	m_memostate = MEMO_NOT_YET;
	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->4", __FUNCTION__, __LINE__, "onmessage dll_oub");
	m_slog.Trim();
	OutputDebugString(m_slog);
	checkMemo(m_sCode); Invalidate();
}

void CControlWnd::parseAlert(struct _alertR* alert)
{
	CString sym, val;
	bool	bChanged = false;
	CString rned,dist,vist,vitime, vitype;

	if (alert->code == m_sRtsCode)
	{
		DWORD*	data = nullptr;

		m_slog.Format("[SYM_IMG_DIAG] 시작");
		OutputDebugString(m_slog);


		for (int ii = alert->size - 1; ii >= 0; ii--)
		{
			if(ii==1)
			{
				m_slog.Format("[SYM_IMG_DIAG] !!!!!!!!!!   alert->size=[%d]", alert->size);
				OutputDebugString(m_slog);
			}

			data = (DWORD *)alert->ptr[ii];

			//2026-08-26: 패킷마다 독립적으로 판단하도록 이전 반복의 잔여값을 걷어내고
			//현재 유지중인 m_jinfo 값으로 초기화한다(이전엔 for문 밖에서 한번만 선언되어
			//한번 바뀌면 이후 패킷들까지 계속 loadBitmap이 재호출되고, 그때 안 바뀐
			//필드는 빈 문자열이 넘어가던 문제가 있었음).
			bChanged = false;
			rned = m_jinfo.rned;
			dist = m_jinfo.dist;
			vist = m_jinfo.vist;
			vitime = m_jinfo.vitime;
			vitype = m_jinfo.vitype;

			if (data[950])		//배분정보
			{
				dist = (char*)data[950];

				bChanged = true;
				m_slog.Format("[SYM_IMG_DIAG][%s] bChanged=true (data[950] 배분정보) dist=[%s]", m_sCode, dist);
				OutputDebugString(m_slog);
			}

			if (data[951])       //임의종료
			{
				rned = (char*)data[951];

				bChanged = true;
				m_slog.Format("[SYM_IMG_DIAG][%s] bChanged=true (data[951] 임의종료) rned=[%s]", m_sCode, rned);
				OutputDebugString(m_slog);
			}

			if (data[701])      //변동성
			{
				vist   = (char*)data[701];
				vitime = (char*)data[702];
				vitype = (char*)data[703];
				bChanged = true;
				m_slog.Format("[SYM_IMG_DIAG](data[701] 변동성) [%s] bChanged=true  vist=[%s] vitime=[%s] vitype=[%s]", m_sCode, vist, vitime, vitype);
				OutputDebugString(m_slog);
			}

			m_jinfo.rned = rned;
			m_jinfo.dist = dist;
			m_jinfo.vist = vist;
			m_jinfo.vitime = vitime;
			m_jinfo.vitype = vitype;

			if (data[704])      //정지
			{
				CString sval;
				sval.Format("%s", (char*)data[704]);
				sval.TrimRight();

				m_slog.Format("[SYM_IMG_DIAG] (data[704] 정지) [%s]  sval=[%s] krgb=[%s] m_sSearchkrgb=[%s]", m_sCode, sval, m_jinfo.krgb, m_sSearchkrgb);
				OutputDebugString(m_slog);

				if (sval == "0")  //정상
				{
					if(m_jinfo.krgb == "정지")
						m_jinfo.krgb = "정상";
					else 
						m_jinfo.krgb = m_sSearchkrgb;
				}
				else  //정지
				{
					/*if(m_jinfo.krgb != "정지")
						m_sSearchkrgb = m_jinfo.krgb;*/
					m_jinfo.krgb = "정지";
				}

				if(m_sSearchkrgb == "VI" && sval == "0")
					loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, "1", "", "");
				else
					loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
				Invalidate();
				bChanged = true;
				m_slog.Format("[SYM_IMG_DIAG] (data[704] 정지) [%s] bChanged=true sval=[%s] krgb=[%s] ", m_sCode, sval, m_jinfo.krgb);
				OutputDebugString(m_slog);
			}

			if (data[191])   //투자유의
			{ 
				val.Format("%s", (char *)data[191]);  //19인 경우 유의
				val.Trim();
			
				if (val == "19")
				{
					if (m_jinfo.krgb.Find("유의") < 0)
					{
						m_jinfo.krgb = "유의";
						m_sSearchkrgb = m_jinfo.krgb;
						m_sInfo = m_jinfo.krgb;
						loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
						Invalidate();
					}

				}
				else if (val == "0")
				{
					if (m_jinfo.krgb.Find("정상") < 0)
					{
						m_jinfo.krgb = "정상";
						m_sSearchkrgb = m_jinfo.krgb;
						m_sInfo = m_jinfo.krgb;
						loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
						Invalidate();
					}
				}
			}

			if (bChanged)
			{
				m_slog.Format("[SYM_IMG_DIAG][마지막][%s] bChanged loadBitmap  krgb=[%s] 지역변수 rned=[%s] dist=[%s] vist=[%s] vitime=[%s] vitype=[%s] ",
					m_sCode, m_jinfo.krgb, rned, dist, vist, vitime, vitype);
				OutputDebugString(m_slog);

				m_slog.Format("[SYM_IMG_DIAG][마지막][%s] bChanged loadBitmap  krgb=[%s] m_jinfo rned=[%s] dist=[%s] vist=[%s] vitime=[%s] vitype=[%s]",
					m_sCode, m_jinfo.krgb, m_jinfo.rned, m_jinfo.dist, m_jinfo.vist, m_jinfo.vitime, m_jinfo.vitype);
				OutputDebugString(m_slog);

				loadBitmap(m_jinfo.jrat, m_jinfo.nrat, m_jinfo.jgub, m_jinfo.krgb,rned,dist,vist,vitime,vitype);

				Invalidate();
			}
		}
	}
}

void CControlWnd::LoadLoanGrade( void )
{
	CStdioFile file;
	CString strPath;

	strPath.Format("%s/tab/loangrade.mst", m_sRoot);
	if (!file.Open(strPath, CFile::modeRead | CFile::shareDenyNone))
		return;	

	CString str;
	CString strCode, strGrade, strName;
	int	iFind = 0;
	int	iOld = 0;
	sGrade	info;

	while (file.ReadString(str))
	{
		iOld = str.Find( ';', iFind+1 );
		if (iOld < 0 )
			continue;

		strCode = str.Mid(iFind,iOld-iFind);
		iFind = iOld;

		iOld = str.Find( ';', iFind+1 );
		if (iOld < 0 )
			continue;

		strName = str.Mid(iFind,iOld-iFind);
		iFind = iOld;

		strGrade = str.Right( 2 );
 
		strCode.TrimRight();
		info.strCode  = strCode.Left(7);
		info.strName  = strName;
		info.strGrade = strGrade;

		m_arrGrade.Add( info );
	}

	file.Close();
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CString txt;
	CPen	Pen;
	CRect	rc, rect;
	COLORREF bxColor = 0, bgColor = 0, fgColor = 0;

	GetClientRect(&rc);
	// set font
	CFont	*pFont = getAxFont(m_Param.fonts, m_Param.point, m_Param.style);
	CFont	*pOldFont = (CFont*)dc.SelectObject(pFont);

	if (!m_bHover)
	{
		bxColor = m_boxColor;
		bgColor = GetIndexColor(m_Param.pRGB);
		fgColor = GetIndexColor(130);//m_Param.tRGB);
	}
	else
	{
		bxColor = m_boxColorHover;
		bgColor = m_pRgbHover;
		fgColor = m_tRgbHover;
	}

	Pen.CreatePen(PS_SOLID, 0, bxColor);
	CPen	*pOldPen = (CPen*)dc.SelectObject(&Pen);
	dc.Rectangle(rc);
	dc.SelectObject(pOldPen); Pen.DeleteObject();

	rc.DeflateRect(1, 1);
	if (GetState() & 0x0004)
		dc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	else
		dc.FillSolidRect(rc, bgColor);

	rc.InflateRect(1, 1);

//	drawTriangle(&dc, rc, fgColor);

	rect = rc; rect.left = rect.right - indicatorW;
	drawIndicator1(&dc, rect, RGB(128, 128, 128));	// 종목정보
	if (m_memostate == MEMO_EXIST)
		drawIndicator2(&dc, rect, RGB( 68, 225, 115));	// 메모여부
	rc.right -= indicatorW;

	// bitmap draw
	rect = rc; rect.right = rect.left + bitmapW;
//	drawBitmap(&dc, rect, m_bitmap);
	DrawBitmap(&dc, rect);
	
	rc.left += bitmapW + 1;

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(fgColor);
	dc.DrawText(m_sData, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		
	dc.SelectObject(pOldFont);
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_slog.Format("[cx_symbol] m_sRtsCode = [%s]", m_sRtsCode);
	OutputDebugString(m_slog);
	if (!m_capture)
	{
		SetCapture();
		m_capture = true;
		m_bMouseOver = true;
		Invalidate();
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CString strINI;

	strINI.Format("%s\\User\\%s\\userconf.ini", m_sRoot, m_sUserID);

	CRect rc;

	GetClientRect(&rc);
	
	rc.left = 11;
	rc.right = 27;

	if (m_chjgub == '8' || m_chjgub == '9' || m_chjgub == '0')
	{
		if (rc.PtInRect(point))
		{
			m_capture = false;
			ReleaseCapture();
			
			CString tmp;
			
			tmp = m_jinfo.codx;
			
			tmp = tmp.Mid(1);
			
			WritePrivateProfileString("IB202600", "POPUP_FLAG", "1", strINI);
			WritePrivateProfileString("IB202600", "POPUP_CODE", tmp, strINI);
			
			CString string;
			
			string.Format("IB202600 /S/t0/d%s\t%s", "1301", m_jinfo.codx);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)string.GetString());
			
			Invalidate();
		}
		else
		{
			if (m_capture)
			{
				CString tmpCode{};
				char	szBuf[64]{};
				int	datL = 0;

				tmpCode = m_sCode;
				tmpCode.Replace("N.", "");
				tmpCode.Replace("M.", "");
				m_capture = false;
				ReleaseCapture();
				datL = sprintf(szBuf, "1301%c%s\t1777%c%d\t17414\t1377\t1971\t",0x7f, (char *)m_sCode.GetString(), 0x7f, GetMKgubn(m_sCode));	//2013.06.14 KSJ 매매단위, 지정자문인 심볼추가
				if (m_bFromDLL)
					SendTR_Dll(szBuf, datL);
				else
					SendTR_Control("POOPPOOP", szBuf, datL, US_OOP|US_PASS);

				m_ptUp = point;
				Invalidate();
			}
		}
	}  //쓸일이 없다.
	else
	{
		if (m_capture)
		{
			CString tmpCode{};
			char	szBuf[64]{};
			int	datL = 0;

			m_capture = false;
			ReleaseCapture();
			
			tmpCode = m_sCode;
			tmpCode.Replace("N.", "");
			tmpCode.Replace("M.", "");
			datL = sprintf(szBuf,"1301%c%s\t1777%c%d\t17414\t1377\t1971\t",0x7f, (char *)m_sCode.GetString(), 0x7f, GetMKgubn(m_sCode));		//2013.06.14 KSJ 매매단위, 지정자문인 심볼추가
			if (m_bFromDLL)
				SendTR_Dll(szBuf, datL);
			else
				SendTR_Control("POOPPOOP", szBuf, datL, US_OOP|US_PASS);

			m_ptUp = point;
			Invalidate();
		}
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	/*if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CRect rc; GetClientRect(&rc);
	if (rc.PtInRect(point))
	{
		if (!m_bMouseOver)
			Invalidate();
		m_bMouseOver = true;
	}
	else
	{
		if (m_bMouseOver)
			Invalidate();
		m_bMouseOver = false;
	}*/

	m_slog.Format("\r\n[symbol] m_sRtsCode = [%s]" , m_sRtsCode);
	m_slog.Trim();
	OutputDebugString(m_slog);

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CControlWnd::GetState()
{
	if (m_capture && m_bMouseOver)
		return 0x0004;
	return 0;
}

void CControlWnd::SetCheck(bool bCheck)
{
	m_bCheck = bCheck; Invalidate();
}

bool CControlWnd::GetCheck()
{
	return m_bCheck;
}

void CControlWnd::OnDestroy() 
{
	if (m_pTipCtrl)
		m_pTipCtrl.reset();

	CWnd::OnDestroy();	
}

COLORREF CControlWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

LOGFONT CControlWnd::setFont(CString fName, int fsize, int style)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, fName);  
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	style = 3;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf.lfItalic = TRUE;
		break;
	case 3: // bold
		lf.lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;
		break;
	}

	return lf;
}

void CControlWnd::SetParam(_param *pParam)
{
	CString	string, text, tmps, keys;

	m_sUserID = Variant(nameCC, "");
	m_sRoot   = Variant(homeCC, "");

	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	tmps	= _T("/ ");
	keys = _T("abcdeirsut");
	string	= pParam->options;
	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = string.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = string.Find('/', idx);

		text = (pos < 0) ? string.Mid(idx) : string.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'a':	// box color
			if (text.IsEmpty())
				m_boxColor = GetIndexColor(89);
			else
				m_boxColor = GetIndexColor(atoi(text));
			break;
		case 'b':	// hover box color
			if (text.IsEmpty())
				m_boxColorHover = GetIndexColor(91);
			else
				m_boxColorHover = GetIndexColor(atoi(text));
			break;
		case 'c':	// hover background color
			if (text.IsEmpty())
				m_pRgbHover = GetIndexColor(92);
			else
				m_pRgbHover = GetIndexColor(atoi(text));
			break;
		case 'd':
			if (text.IsEmpty())
				m_tRgbHover = GetIndexColor(130);//69);
			else
				m_tRgbHover = GetIndexColor(130);//atoi(text));
			break;
		case 'e':
			m_ujongsise = true;
			break;
		case 'i':
			m_bFromDLL = TRUE;
			break;
		case 'r':
			m_trKey = atoi(text);
			break;
		case 's':
			m_bServerMode = TRUE;
			break;
		case 'u':
		
			break;
		case 't':
			m_sID = text;
			break;
		}
	}
	m_bServerMode = TRUE;
	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> option=[%s]  m_bFromDLL=[%d]", __FUNCTION__, __LINE__, "setparam", pParam->options, m_bFromDLL);
	m_slog.Trim();
	OutputDebugString(m_slog);
}

void CControlWnd::drawTriangle(CDC *pDC, CRect rc, COLORREF clr)
{
	if (rc.Width() < 3)
		return;

	const	CSize	sz(10, 8);

	pDC->FillSolidRect(rc.right - sz.cx, rc.bottom - sz.cy, 5, 1, clr);
	pDC->FillSolidRect(rc.right - sz.cx + 1, rc.bottom - sz.cy + 1, 3, 1, clr);
	pDC->FillSolidRect(rc.right - sz.cx + 2, rc.bottom - sz.cy + 2, 1, 1, clr);
}

// updateX_20060110
void CControlWnd::drawIndicator1(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap = 0, height = 0;

	CPen*   oldPen   = dc->SelectObject(getAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(getAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 6;
	height = (rect.Height() - gap) / 2;

	rect.top  = rect.bottom - height;
	rect.left = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.bottom;
	pts[1].x = rect.right;
	pts[1].y = rect.top;
	pts[2].x = rect.right;
	pts[2].y = rect.bottom;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

// updateX_20060110, 메모표시여부
void CControlWnd::drawIndicator2(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap = 0, height = 0;

	CPen*   oldPen   = dc->SelectObject(getAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(getAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 5;
	height = (rect.Height() - gap) / 2;

	rect.bottom = rect.top + height;
	rect.left   = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.top;
	pts[1].x = rect.right;
	pts[1].y = rect.bottom;
	pts[2].x = rect.right;
	pts[2].y = rect.top;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CControlWnd::DrawBitmap(CDC* pDC, CRect rect)
{
	rect.DeflateRect(2, 2, 1, 2);

	if (m_pRBmp == NULL)
		return;

	CDC memDC;
	CBitmap* pOldBmp = nullptr;
	BITMAP bmpInfo;
	memDC.CreateCompatibleDC(pDC);
	m_pRBmp->GetBitmap(&bmpInfo);

	if (memDC.m_hDC != NULL)
		pOldBmp = (CBitmap*)memDC.SelectObject(m_pRBmp);
    //앞쪽의 스플릿 이미지 크기는 12 ,뒷쪽의 스플릿 이미지 크기는 16
	//스플릿 이미지가 아닐경우는 이 두가지의 크기를 합쳐서 그리면 됨
	//m_bBmpSlice는 이미 하나의 이미지로 그리게 되어있음
	if (m_bBmpSlice)
	{
		pDC->StretchBlt(rect.left + 12, rect.top, rect.Width() - 12, rect.Height(),
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

		if (m_pTBmp != NULL)
		{
			m_pTBmp->GetBitmap(&bmpInfo);
			memDC.SelectObject(m_pTBmp);
			pDC->StretchBlt(rect.left, rect.top, rect.Width() - 16, rect.Height() / 2,
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		}

		if (m_pBBmp != NULL)
		{
			m_pBBmp->GetBitmap(&bmpInfo);
			memDC.SelectObject(m_pBBmp);
			pDC->StretchBlt(rect.left, rect.top + rect.Height() / 2, rect.Width() - 16, rect.Height() / 2,
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
		}
	}
	else
	{
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
				&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	}

	if (pOldBmp)
		memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
}

void CControlWnd::drawBitmap(CDC *pDC, CRect rect, CBitmap *pBitmap)
{
	rect.DeflateRect(2, 2, 1, 2);

	if (pBitmap)
	{
		CDC	memDC;
		CBitmap	*pSBitmap = nullptr;
		BITMAP stBitmapInfo{};

		pBitmap->GetBitmap(&stBitmapInfo);
		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
				&memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
		if (pSBitmap)
			memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
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

void CControlWnd::WriteAlert(CString str)
{
	CString sym, dat;
	bool bChanged = false;
	while (!str.IsEmpty())
	{
		sym = Parser(str, "\t");
		dat = Parser(str, "\t");

		switch (atoi(sym))
		{
		case 23:	// 현재가
			m_jinfo.jisu = dat;
			m_jinfo.jisu.Replace("+", "");
			m_jinfo.jisu.Replace("-", "");
			bChanged = true;
			break;
		case 24:	// 전일대비
			m_jinfo.diff = dat;
			bChanged = true;
			break;
		case 33:	// 등락율
			m_jinfo.rate = dat;
			bChanged = true;
			break;
		case 27:	// 거래량
			m_jinfo.gvol = dat;
			bChanged = true;
			break;
		}
	}
	if (bChanged && m_pPopup)
	{
		m_pPopup->setJinfo(m_jinfo, m_ujongsise);
		m_pPopup->Invalidate();
	}
}

LRESULT CControlWnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover = TRUE;
	Invalidate();
	return 0;
}


LRESULT CControlWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;
	Invalidate();
	return 0;
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	CString	string, text, tmps, keys;
	
	tmps	= _T("/ ");
	keys	= _T("abcd");
	m_Param.options = sParam;
	string	= m_Param.options;

	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = string.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = string.Find('/', idx);

		text = (pos < 0) ? string.Mid(idx) : string.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'a':	// box color
			if (text.IsEmpty())
				m_boxColor = GetIndexColor(89);
			else
				m_boxColor = GetIndexColor(atoi(text));
			break;
		case 'b':	// hover box color
			if (text.IsEmpty())
				m_boxColorHover = GetIndexColor(91);
			else
				m_boxColorHover = GetIndexColor(atoi(text));
			break;
		case 'c':	// hover background color
			if (text.IsEmpty())
				m_pRgbHover = GetIndexColor(92);
			else
				m_pRgbHover = GetIndexColor(atoi(text));
			break;
		case 'd':	// hover foreground color
			if (text.IsEmpty())
				m_tRgbHover = GetIndexColor(69);
			else
				m_tRgbHover = GetIndexColor(atoi(text));
			break;
		}
	}
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;

	return strResult.AllocSysString();
}

CPen* CControlWnd::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CBrush* CControlWnd::getAxBrush(COLORREF clr)
{
	return (CBrush*) m_pParent->SendMessage(WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* CControlWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	style = 3;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

CBitmap* CControlWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

// updateX_20060110

int  CControlWnd::checkFileMemo(CString sKey)
{
	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->  code=[%s] ",
		__FUNCTION__, __LINE__, "checkFileMemo start", sKey);
	OutputDebugString(m_slog);

	if (sKey.IsEmpty())
		return false;

	CString dir, dat, stmp;
	char	key[12]{}, lBytes[4]{};

	dir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, "user", m_sUserID);
	CFileFind	finder;
	if (!finder.FindFile(dir))
		return memo_state::MEMO_NOEXSIST;

	TRY
	{
		CFile file(dir, CFile::modeRead | CFile::shareDenyNone);
		UINT	nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}
					stmp.Format("%-12s", key);
					stmp.Trim();
					char* pbuf = new char[12 + 1];
					memset(pbuf, 0x00, 13);
					memcpy(pbuf, stmp.GetBuffer(0), 12);
					stmp.Format("%-12s", pbuf);
					stmp.Trim();
					delete[] pbuf;

					m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> sKey=[%s]  stmp= [%s] key=[%s]",
						__FUNCTION__, __LINE__, "checkFileMemo", sKey, stmp, key);
					m_slog.Trim();
					OutputDebugString(m_slog);

					if (sKey == stmp && lSize > 0)
					{
						dat.ReleaseBuffer();
						return memo_state::MEMO_EXIST;
					}
					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
		} while ((int)nBytesRead);
		file.Close();
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

		m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->  code=[%s] ",
			__FUNCTION__, __LINE__, "checkFileMemo end", sKey);
	OutputDebugString(m_slog);

	return memo_state::MEMO_NOEXSIST;
}

void CControlWnd::checkMemo(CString sKey)
{
	m_slog.Format("\r\n[memo][%-40s][%-35s]-->m_memostate=[%d]", __FUNCTION__, sKey, m_memostate);
	m_slog.Trim();

	if (m_memostate == MEMO_EXIST || m_memostate == MEMO_NOEXSIST)
		return;

	st_mid_SDEmemo* pmid = new st_mid_SDEmemo;
	pmid->chgubn[0] = 'E';

	char* pData = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		memcpy(pmid->chusid, pData, strlen(pData));


	memcpy(pmid->chcode, (char*)sKey.GetBuffer(0), sKey.GetLength());

	if (m_bFromDLL)
		SendMemoTR_Dll((char*)pmid, sizeof(st_mid_SDEmemo), TRKEY_MEMO_CHECK, sKey);
	else
		SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(st_mid_SDEmemo), US_KEY, TRKEY_MEMO_CHECK, sKey);

}

void CControlWnd::loadBitmap(CString jrab, CString nrat, CString jgub, CString kind, CString rned, CString dist, CString vist, CString vitime, CString vitype)
{
	CString strImgPath;
	CString strTemp;

	kind.Trim();
	m_strCaseTip.Empty();

	const	int	nRat = atoi(m_jinfo.nrat);
	if (vist == _T("1"))
	{

		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/btn_vi.bmp", m_sRoot);
		m_pRBmp = getBitmap(strImgPath);
		
		vitype.Trim();

		strTemp = vitime;
		if (vitype == _T("1")){
			m_strCaseTip = "[정적 변동성 완화장치 발동 "+ strTemp.Mid(0,2) + ":" + strTemp.Mid(2,2) + ":" + strTemp.Mid(4,2) + "]";
		}
		else if (vitype == _T("2")){
			m_strCaseTip = "[동적 변동성 완화장치 발동 "+ strTemp.Mid(0,2) + ":" + strTemp.Mid(2,2) + ":" + strTemp.Mid(4,2) + "]";
		}
		else{
			m_strCaseTip = "[동적/정적 변동성 완화장치 발동 "+ strTemp.Mid(0,2) + ":" + strTemp.Mid(2,2) + ":" + strTemp.Mid(4,2) + "]";
		} 

		
		m_chjgub = -1;			// 값 초기화
		
		return;
	}

	else 
	{
		if (m_bSideTime == FALSE)
		{
			if (rned == _T("12") || rned == _T("14"))
			{
				m_bBmpSlice = FALSE;
				strImgPath.Format("%s/image/임의.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);

				m_strCaseTip = "단일가매매 임의연장";
				
				m_chjgub = -1;			// 값 초기화

				return;
			}
			else if (dist != "")
			{
				if (dist.GetLength() > 1)
				{
					if (dist.GetAt(0) == '1')
					{
						m_bBmpSlice = FALSE;
						strImgPath.Format("%s/image/배분.bmp", m_sRoot);
						m_pRBmp = getBitmap(strImgPath);

						m_strCaseTip = "동시호가 수량배분";
						
						m_chjgub = -1;			// 값 초기화

						return;
					}
				}
			}
		}
		else
		{
			if (rned == _T("16"))
			{
				m_bBmpSlice = FALSE;
				strImgPath.Format("%s/image/임의.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				
				m_strCaseTip = "단일가매매 임의연장";
				
				m_chjgub = -1;			// 값 초기화
				
				return;
			}
			else if (dist != "")
			{
				if (dist.GetLength() > 1)
				{
					if (dist.GetAt(0) == '1')
					{
						m_bBmpSlice = FALSE;
						strImgPath.Format("%s/image/배분.bmp", m_sRoot);
						m_pRBmp = getBitmap(strImgPath);
						
						m_strCaseTip = "동시호가 수량배분";
						
						m_chjgub = -1;			// 값 초기화
						
						return;
					}
				}
			}
		}
	}
	if (kind == _T("주의") || kind == _T("경고") || kind == _T("위예") || kind == _T("위험") || kind == _T("정지") || 
		kind == _T("정리") || kind == _T("관리") || kind == _T("감리") || kind == _T("권리") || kind == _T("배당") ||
		kind == _T("액면") || kind == _T("불성") || kind == _T("종료") || kind == _T("환기") || kind == _T("감자") ||
		kind == _T("병합") || kind == _T("합병") || kind == _T("급등") || kind == _T("유의"))
	{
		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/%s.bmp", m_sRoot, kind);
		m_pRBmp = getBitmap(strImgPath);
	
		const	int	nJrat = atoi(m_jinfo.jrat);
		m_jinfo.hnam.Trim();
		//m_strCaseTip.Format("%s, 현금증거금율 : %d%%, 신용불가", m_jinfo.hnam, nJrat);
		if (nRat == 0)
			m_strCaseTip.Format("%s, 현금증거금률 : %d%%, 신용불가", m_jinfo.hnam, nJrat);
		else
			m_strCaseTip.Format("%s, 현금증거금률 : %d%%, 신용증거금률 : %d%%", m_jinfo.hnam, nJrat, nRat);
     
		m_chjgub = -1;			// 값 초기화
	}
	else if (kind == _T("단기"))	//2012.10.31 KSJ 단기과열종목 추가
	{
		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/과열.bmp", m_sRoot);
		m_pRBmp = getBitmap(strImgPath);
		
		m_strCaseTip = "단기과열종목";

		m_chjgub = -1;			// 값 초기화
	}
	else if (kind == _T("초저유동성"))
	{
		m_bBmpSlice = FALSE;
		strImgPath.Format("%s/image/유동.bmp", m_sRoot);
		m_pRBmp = getBitmap(strImgPath);
		
		m_strCaseTip = "초저유동성 종목 10분단위 체결";
		
		m_chjgub = -1;			// 값 초기화
	}
	else
	{
		m_bBmpSlice = TRUE;
		
		//조기종료일 경우에는 이미지가 하나이기 때문에 다음의 처리는 필요없음
		if (jgub.GetAt(0) != '7')
		{
			strImgPath.Format("%s/image/%d.bmp", m_sRoot, atoi((LPCTSTR)jrab));
			m_pTBmp = getBitmap(strImgPath);

			strImgPath.Format("%s/image/%d.bmp", m_sRoot, atoi((LPCTSTR)nrat));
			m_pBBmp = getBitmap(strImgPath);
		}

		switch (jgub.GetAt(0))
		{
			//case '7'://분리시 이미지 교체
			case 'E':  //test  리츠
			case 'G':  //test 신주인수권
			case 'H':  //test 신주인수권증서
			case 'I':  //test 수익증권
			case 'J':  //test 신탁수익증권
			case 'K':  //test 투자계약증권
			case '6':  //ELW
			case '1':	// 거래소
				strImgPath.Format("%s/image/거래소.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'F':  //스팩
			case '2':	// 코스닥
			case 'C':	// BDC 수익증권
			case 'D':	// BDC 투자회사
				strImgPath.Format("%s/image/코스닥.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '3':	// K-OTC
				strImgPath.Format("%s/image/btn_jang.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);	
				break;
			case '4':
				strImgPath.Format("%s/image/거래소.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '7':
				m_bBmpSlice = FALSE;

				strImgPath.Format("%s/image/종료.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '8' :
				strImgPath.Format("%s/image/추천종목.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '9' :
				strImgPath.Format("%s/image/추천종목.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case '0' :
				strImgPath.Format("%s/image/추천종목.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'R' :
				strImgPath.Format("%s/image/REPORT.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'A' :	//2013.06.11 KSJ 코넥스 추가
				strImgPath.Format("%s/image/코넥스.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
			case 'B':
				strImgPath.Format("%s/image/거래소.bmp", m_sRoot);
				m_pRBmp = getBitmap(strImgPath);
				break;
		}

		m_chjgub = jgub.GetAt(0);
	}



/*	kind.Trim();

	CString	imagePath; imagePath.Format("%s/%s/", m_sRoot, "image");

	if (kind.CompareNoCase("정상"))
	{
		m_bitmap = getBitmap(imagePath + kind + _T(".bmp"));
	}
	else if (jgub.GetLength() >= 1)
	{
		switch (jgub.GetAt(0))
		{
		case '1':	// 거래소
			m_bitmap = getBitmap(imagePath + _T("거래소.bmp"));
			break;
		case '2':	// 코스닥
			m_bitmap = getBitmap(imagePath + _T("코스닥.bmp"));
			break;
		case '3':	// 프리보드
			m_bitmap = getBitmap(imagePath + _T("프리보드.bmp"));
			break;
		}
	}
	else
	{
		m_bitmap = NULL;
	}
*/
}

void CControlWnd::OnVisibleChanged() 
{
	ShowWindow(m_visible);
}

LRESULT CControlWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;

	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		CRect rc;
		GetClientRect(&rc);

		if (m_pTipCtrl == nullptr)
		{
			m_pTipCtrl = std::make_unique<CToolTipCtrl>();
			m_pTipCtrl->Create(this);
		}

		if (m_bBmpSlice && !m_sData.IsEmpty())
		{
			CString strTip;
			const	int	nJrat = atoi(m_jinfo.jrat);
			const	int	nRat = atoi(m_jinfo.nrat);
		
			CPoint	point;

			if (m_chjgub == '8' || m_chjgub == '9' || m_chjgub == '0')
			{
				CRect	rect;

				::GetCursorPos(&point);
				ScreenToClient(&point);

				rect = rc;
				rect.left = 11;
				rect.right = 27;

				if (rect.PtInRect(point))
				{
					strTip = "IBKS 추천종목";
				}
				else
				{
					if (nRat == 0)
						strTip.Format("%s %s, 현금증거금률 : %d%%, 신용불가", IsNXTAble(m_jinfo.codx) == TRUE ? "[NXT거래가능]" : "", m_jinfo.hnam, nJrat);
					else
						strTip.Format("%s %s, 현금증거금률 : %d%%, 신용증거금률 : %d%%",  IsNXTAble(m_jinfo.codx) == TRUE ? "[NXT거래가능]" : "",  m_jinfo.hnam, nJrat, nRat);
				}

				CString tmp;

				m_pTipCtrl->GetText(tmp,this,GetDlgCtrlID());

				if (tmp != "")
				{
					if (tmp != strTip)
					{
						m_pTipCtrl->DelTool(this);
						m_pTipCtrl->Activate(FALSE);
						m_pTipCtrl.reset();
						 
						m_pTipCtrl = std::make_unique<CToolTipCtrl>();
						m_pTipCtrl->Create(this);
					}
				}
			}
			else
			{
				if (nRat == 0)
					strTip.Format("%s %s, 현금증거금률 : %d%%, 신용불가", IsNXTAble(m_jinfo.codx) == TRUE ? "[NXT거래가능]" : "", m_jinfo.hnam, nJrat);
				else
					strTip.Format("%s %s, 현금증거금률 : %d%%, 신용증거금률 : %d%%", IsNXTAble(m_jinfo.codx) == TRUE ? "[NXT거래가능]" : "", m_jinfo.hnam, nJrat, nRat);
			}
			
			//strTip.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s",codx, jgub, jrat, ujgb, frgb, sjjs, amga, ksmm, jbkm, dyga, lock, nrate);
			m_pTipCtrl->AddTool(this, (LPCTSTR)strTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->SendMessage(TTM_SETMAXTIPWIDTH, 0, 1000);
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
		else if (m_strCaseTip != "")
		{
			m_pTipCtrl->AddTool(this, (LPCTSTR)m_strCaseTip, &rc, GetDlgCtrlID());
			m_pTipCtrl->SendMessage(TTM_SETMAXTIPWIDTH, 0, 1000);
			m_pTipCtrl->Activate(TRUE);
			m_pTipCtrl->RelayEvent(&msg);
		}
		else
		{
			m_pTipCtrl->DelTool(this);
			m_pTipCtrl->Activate(FALSE);
		}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}
 
void CControlWnd::SendTR_Control(CString name, char* datb, int datl, char stat)
{
	char	szBuf[1024]{};
	struct _userTH udat {};
	int	idx = m_Param.name.GetLength();

	CopyMemory(udat.trc, (char*)name.GetString(), name.GetLength());
	udat.key = m_Param.key;
	udat.stat = stat | US_KEY;

	szBuf[0] = (int)TRKEY_MEMO_POPUPOOP;
	idx++;
	CopyMemory(&szBuf[1], (char*)m_Param.name.GetString(), m_Param.name.GetLength());

	szBuf[idx++] = '\t';

	CopyMemory(&szBuf[idx], &udat, L_userTH);
	idx += L_userTH;

	CopyMemory(&szBuf[idx], datb, datl);

	int iret{};
	if (m_pParent->GetSafeHwnd())
		iret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);
}

BOOL CControlWnd::SendTR_Dll(char* datb, int datl)
{
	char	szBuf[1024]{};
	struct	_userTH* info = (struct _userTH*)szBuf;;

	CopyMemory(info->trc, "pooppoop", sizeof(info->trc));
	info->stat = US_KEY | US_PASS | US_OOP;

	if (m_trKey > -1)
		info->key = m_Param.key;  //2016 복수종목 화면에서는 
	else if (!m_bServerMode)  //차트와 같이 dll 에서 cx_symbol를 사용하는데 기존파일방식
		info->key = m_Param.key;
	else
		info->key = TRKEY_MEMO_POPUPOOP;

	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->key=[%c] datl=[%d]  [%.50s]", __FUNCTION__, __LINE__, "SendTR_Dll pooppoop", info->key, datl, datb);
	m_slog.Trim();
	OutputDebugString(m_slog);

	memcpy(&szBuf[L_userTH], datb, datl);

	if (m_pParent->GetSafeHwnd())
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);

	return TRUE;
}

void CControlWnd::SendMemoTR_Control(CString name, char* datb, int datl, char stat, int trkey, CString sKey)
{
	struct _userTH udat {};
	char* szBuf{};
	int	idx = m_Param.name.GetLength();

	CString strtmp;
	switch (trkey)
	{
	case TRKEY_MEMO_INSERT:
		strtmp = "메모추가";
		break;
	case TRKEY_MEMO_SEARCH:
		strtmp = "메모서버조회";
		break;
	case TRKEY_MEMO_UPDATE:
		strtmp = "메모수정 및 업데이트";
		break;
	case TRKEY_MEMO_DELETE:
		strtmp = "메모 삭제";
		break;
	case TRKEY_MEMO_CHECK:
		strtmp = "메모 존재 여부 확인";
		break;
	}

	if (trkey == TRKEY_MEMO_CHECK || trkey == TRKEY_MEMO_DELETE)
	{
		szBuf = new char[sizeof(st_mid_SDEmemo) + sizeof(struct _userTH) + 20];
		//udat = (struct _userTH*)&szBuf[1 + name.GetLength()];
		CopyMemory(udat.trc, (char*)name.GetString(), name.GetLength());
		udat.key = m_Param.key;
		udat.stat = stat;

		//// set buffer
		szBuf[0] = trkey;
		idx++;

		CopyMemory(&szBuf[1], (char*)m_Param.name.GetString(), m_Param.name.GetLength());
		szBuf[idx++] = '\t';

		CopyMemory(&szBuf[idx], &udat, L_userTH);
		idx += L_userTH;

		CopyMemory(&szBuf[idx], datb, datl);
	}
	else
	{
		szBuf = new char[sizeof(st_mid_memo) + sizeof(struct _userTH) + 20];
		//udat = (struct _userTH*)&szBuf[1];
		CopyMemory(udat.trc, (char*)name.GetString(), name.GetLength());
		udat.key = m_Param.key;
		udat.stat = stat;

		//// set buffer
		szBuf[0] = trkey;
		idx++;

		CopyMemory(&szBuf[1], (char*)m_Param.name.GetString(), m_Param.name.GetLength());
		szBuf[idx++] = '\t';

		CopyMemory(&szBuf[idx], &udat, L_userTH);
		idx += L_userTH;

		CopyMemory(&szBuf[idx], datb, datl);
	}

	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> datl=[%d] trkey=[%s] [%.50s]",
		__FUNCTION__, __LINE__, "SendMemoTR_Control",
		datl, GetMemoTrKeyType(trkey), datb);
	m_slog.Trim();
	OutputDebugString(m_slog);

	if (m_pParent->GetSafeHwnd())
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);

	delete szBuf;
}

BOOL CControlWnd::SendMemoTR_Dll(char* datb, int datl, int trkey, CString sKey)
{
	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->m_bServerMode =[%s] datl=[%d] trkey=[%s] [%.50s]",
		__FUNCTION__, __LINE__, "SendMemoTR_Dll", m_bServerMode == TRUE ? "서버저장조회모드" : "기존파일모드", datl,
		GetMemoTrKeyType(trkey), datb);
	m_slog.Trim();
	OutputDebugString(m_slog);

	if (!m_bServerMode)  //차트와 같이 파일저장 모드인 경우
	{
		if (trkey == TRKEY_MEMO_INSERT || trkey == TRKEY_MEMO_DELETE || trkey == TRKEY_MEMO_UPDATE)
		{
			if (m_pPopup)
				m_pPopup->saveMemo(sKey);										    //차트등과 같은 서버저장 모드가 아닌경우 추가,변경,삭제
		}

		if (trkey == TRKEY_MEMO_CHECK)
		{
			m_memostate = checkFileMemo(sKey);   //서버저장모드가 아닌경우 메모파일체크
			Invalidate();
		}

		if (trkey == TRKEY_MEMO_SEARCH)
		{
			m_pPopup->loadMemo(m_pPopup->m_sKey); //서버저장모드가 아닌경우 파일메모조회
			Invalidate();
		}
		return true;
	}

	struct	_userTH* info;
	char* szBuf{};

	if (trkey == TRKEY_MEMO_CHECK || trkey == TRKEY_MEMO_DELETE)
	{
		szBuf = new char[sizeof(st_mid_SDEmemo) + sizeof(struct _userTH) + 20];
		info = (struct _userTH*)szBuf;;
		CopyMemory(info->trc, "pidomemo", sizeof(info->trc));
		info->stat = US_KEY;
		if (m_trKey > -1)
			info->key = m_Param.key;
		else
			info->key = (BYTE)trkey;

		memcpy(&szBuf[L_userTH], datb, datl);
	}
	else
	{
		szBuf = new char[sizeof(mid_memo) + sizeof(struct _userTH) + 20];
		info = (struct _userTH*)szBuf;;
		CopyMemory(info->trc, "pidomemo", sizeof(info->trc));
		info->stat = US_KEY;
		if (m_trKey > -1)
			info->key = m_Param.key;
		else
			info->key = (BYTE)trkey;

		memcpy(&szBuf[L_userTH], datb, datl);
	}

	if (m_pParent->GetSafeHwnd())
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);

	delete szBuf;

	return TRUE;
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	codedll.Load("axiscode.dll");
	LoadLoanGrade();
	
	return 0;
}

BOOL CControlWnd::IsNXTAble(CString sCode)
{
	sCode.TrimRight();
	sCode.Replace("M.", "");
	sCode.Replace("N.", "");
	if (sCode.GetLength() == 7 &&( sCode.Left(1) == "A" || sCode.Left(1) == "Q" || sCode.Left(1) == "J"))
		sCode = sCode.Mid(1);
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

		m_slog.Format("       [cx_symbol][ISNXT]   MAP!!!!  code =[%s]  sYN = [%s] ", strCode, sYN);
		//OutputDebugString(m_slog);
		return bret;
	}

	//bret = AxStd::IsGetNxt(strCode);

	if (codedll.IsLoaded())
	{
		bret = codedll.Function<const bool WINAPI(const char*)>("IsNxt")(strCode);

		m_slog.Format("       [cx_symbol][ISNXT]   LOAD!!!!  code =[%s]  ret = [%d] ", strCode, bret);
		//OutputDebugString(m_slog);

		m_mapCodeToNXT.SetAt(strCode, bret == true ? "Y" : "N");
	}

	return bret;
}

CString CControlWnd::addComma(CString data)
{
	CString	tmps;
	int	pos = 0, length = 0;

	tmps = data;
	length = tmps.GetLength();
	pos = tmps.Find('.') + 1;
	if (pos > 0)
		return data;

	length -= pos; 
	if (length < 4)
		return data;

	data.Empty();
	for (int ii = 0; ii < length; ii++)
	{
		if ((length-ii) % 3 == 0 && ii != 0)
			data += ',';
		data += tmps.GetAt(ii);
	}
	return data;

}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	GetWindowRect(&rc);

	if (m_pPopup && m_pPopup->IsWindowVisible())
	{		
		//const	int	cx = 225, cy = 250;
		const	int	cx = 225, cy = 265;

		rc.SetRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy);
		m_pPopup->MoveWindow(&rc);		
	}

}

void CControlWnd::SetSideTime(BOOL flag) 
{
	m_bSideTime = flag;
}

void CControlWnd::OnSInfoChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnSMkMsgChanged() 
{
	// TODO: Add notification handler code

}

int CControlWnd::GetMKgubn(CString sCode)
{
	if (sCode.Find("N.") >= 0)
		return 2;
	if (sCode.Find("M") >= 0)
		return 3;
		
	return 1;
}

BSTR CControlWnd::GetCodeSymbolType()
{
	CString Gbun, strResult,slog;
	Gbun = m_jinfo.jgub;
	strResult = Gbun.GetAt(0);
	slog.Format("getcodetype = %s", strResult);
	OutputDebugString(slog);
	return strResult.AllocSysString();
}