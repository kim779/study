#include "stdafx.h"
#include "IBNEWSXX.h"
#include "MapWnd.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CMapStringToString CMapWnd::m_codes;
CMapStringToString CMapWnd::m_names;
CMapStringToString CMapWnd::m_rest;
CMapStringToString CMapWnd::m_rlst;
CSortStringArray CMapWnd::m_snames;
BOOL CMapWnd::m_bDataFileLoad = FALSE;
CRect CMapWnd::m_rcCurrent;
int CMapWnd::m_iOpenCount = 0;

LPCSTR font_size[] = {"8","9","10","11","12","14","16","18","20","22","24",NULL};

CMapWnd::CMapWnd(CWnd *pParent)
: CAxisExt(pParent)
{
	++m_iOpenCount;
	m_rcConfig.SetRect(-1,-1,-1,-1);

	m_hGlobal = NULL;
	m_pStream = NULL;

	m_fontsize = 9;
	m_bShowCodeBtn = FALSE;

	m_pCbFont = NULL;
	m_pBrowser = NULL;

	m_home = Variant(homeCC, "");
	m_user = Variant(userCC, "");
	m_name = Variant(nameCC, "");

	m_bIE6 = (GetIEVersion()=="6") ? TRUE : FALSE;

	Variant(titleCC, "[뉴스조회]");
	LoadImage();
}

CMapWnd::~CMapWnd()
{
	--m_iOpenCount;
}

void CMapWnd::LoadImage()
{
	char buff[1024];
	CString path, img_path;
	
	path.Format(_T("%s\\tab\\PALETTE.INI"), (LPCSTR)m_home);
	const int readL = GetPrivateProfileString("General", "Palette", "Blue", buff, sizeof(buff), path);
	if (readL==0) return;
	
	img_path.Format(_T("%s\\image\\axpanel%s1_"), (LPCSTR)m_home, buff);

	m_hRndBmp[0] = GetAxBitmap((LPCSTR)( img_path + _T("lt.bmp")));
	m_hRndBmp[1] = GetAxBitmap((LPCSTR)( img_path + _T("rt.bmp")));
	m_hRndBmp[2] = GetAxBitmap((LPCSTR)( img_path + _T("lb.bmp")));
	m_hRndBmp[3] = GetAxBitmap((LPCSTR)( img_path + _T("rb.bmp")));
	
	m_hBtn2[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn.bmp")));
	m_hBtn2[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_dn.bmp")));
	m_hBtn2[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_en.bmp")));
	
	m_hBtn4[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn.bmp")));
	m_hBtn4[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_dn.bmp")));
	m_hBtn4[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_en.bmp")));

	m_hRefresh[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\TURN.bmp")));
	m_hRefresh[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\TURN_dn.bmp")));
	m_hRefresh[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\TURN_en.bmp")));
}

void CMapWnd::DrawRoundRectangle( CDC *pDC, CRect rc, COLORREF colBack )
{
	const CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, colBack);
	DrawBitmap(pDC, rc.left,    rc.top,      5, 5, m_hRndBmp[0]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.top,      5, 5, m_hRndBmp[1]->operator HBITMAP());
	DrawBitmap(pDC, rc.left,    rc.bottom-5, 5, 5, m_hRndBmp[2]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.bottom-5, 5, 5, m_hRndBmp[3]->operator HBITMAP());
}

void CMapWnd::DrawBitmap( CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol/*=0xFF00FF*/ )
{
	HDC hDC = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP tBmp = (HBITMAP)::SelectObject(hDC, hBitmap);
	::TransparentBlt(pDC->m_hDC, x, y, w, h, hDC, 0, 0, w, h, tCol);
	::SelectObject(hDC, tBmp);
	::DeleteObject(hDC);
}

void CMapWnd::DrawBitmap( CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol/*=0xFF00FF*/ )
{
	DrawBitmap(pDC, pRC->left, pRC->top, pRC->Width(), pRC->Height(), hBitmap, tCol);
}

void CMapWnd::MakeBackground()
{
	CRect tRc;
	GetClientRect(&tRc);
	
	const int width = tRc.Width(), height = tRc.Height();
	
	CBitmap* oldBmp{};
	CDC *pdc = GetDC(), mdc;
	
	m_hBackground.DeleteObject();
	m_hBackground.CreateCompatibleBitmap(pdc, width, height);
	
	mdc.CreateCompatibleDC(pdc);
	oldBmp = mdc.SelectObject(&m_hBackground);
	
	DrawRoundRectangle(&mdc, m_rcCtrl, m_crCtrlBk);		// Control window
	DrawRoundRectangle(&mdc, m_rcBody, m_crBodyBk);		// Body window
	
	mdc.SelectObject(oldBmp);
	mdc.DeleteDC();
	
	ReleaseDC(pdc);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
}

void CMapWnd::Draw( CDC *pdc )
{
	CDC mdc;
	CRect rc;
	
	GetClientRect(rc);
	mdc.CreateCompatibleDC(pdc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_hBackground);
	pdc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
//	mdc.DeleteDC();	
	mdc.DeleteDC();	
}

std::unique_ptr< CfxStatic > CMapWnd::CreateStatic( LPCTSTR lpszCaption, COLORREF colBk, COLORREF colFg, COLORREF colBorder )
{
	std::unique_ptr<CfxStatic> pSt = std::make_unique<CfxStatic>(m_pFont);

	if (!pSt) return NULL;
	
	if (pSt->Create(lpszCaption, WS_VISIBLE|WS_CHILD|SS_CENTER, CRect(0,0,0,0), this))
	{
		pSt->Init(ssCOLOR|(colBorder ? ssBORDER : 0), 0, lpszCaption, "", colBk, colBk, colFg);
		pSt->SetBorderColor(colBorder);
	}

	return pSt;
}
std::unique_ptr<CfxImgButton> CMapWnd::CreateImgButton(LPCSTR caption, int nID, CBitmap** bmp)
{
	std::unique_ptr< CfxImgButton> pBtn = std::make_unique< CfxImgButton> (m_pFont);

	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP());
		return pBtn;
	}
	else
		return NULL;
}

/*
CfxImgButton* CMapWnd::CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp )
{
	std::unique_ptr< CfxImgButton > pBtn = std::make_unique< CfxImgButton>(m_pFont);

	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP());
		return pBtn.release();
	}
	else 
		return NULL;
}
*/

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WX_EVT_SIZE, OnEvtSize)
	ON_MESSAGE(WX_EVT_MAPN, OnEvtMapn)
	ON_MESSAGE(WX_EVT_REFRESH, OnEvtRefresh)
	ON_MESSAGE(WM_USER, OnUser)
	ON_CBN_SELCHANGE(IDC_CBN_FONT, OnFontChange)
	ON_BN_CLICKED(IDC_BTN_CURR, OnBtnCurr)
	ON_BN_CLICKED(IDC_BTN_CHART, OnBtnChart)
	ON_BN_CLICKED(IDC_BTN_MADO, OnBtnMado)
	ON_BN_CLICKED(IDC_BTN_MASU, OnBtnMasu)
	ON_BN_CLICKED(IDC_BTN_PRINT, OnBtnPrint)

END_MESSAGE_MAP()

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pStTitle = CreateStatic("", m_crCtrlBk, m_crBlack, m_crCtrlBk);
	m_pStTitle->SetFont(m_pFontB, false);
	m_pStTitle->SetAlignment(alLEFT, false);

	m_pCbFont = std::make_unique<CComboBox>();

	if (m_pCbFont->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(), this, IDC_CBN_FONT))
	{
		m_pCbFont->SetFont(m_pFont);
		for(int n=0; font_size[n]!=NULL; ++n)
			m_pCbFont->AddString(font_size[n]);
	}

	m_pBtnCurr  = CreateImgButton("현재가", IDC_BTN_CURR,  m_hBtn2);
	m_pBtnChart = CreateImgButton("차트",   IDC_BTN_CHART, m_hBtn2);
	m_pBtnMado  = CreateImgButton("매도",   IDC_BTN_MADO,  m_hBtn2);
	m_pBtnMasu  = CreateImgButton("매수",   IDC_BTN_MASU,  m_hBtn2);
	m_pBtnPrint = CreateImgButton("인쇄",   IDC_BTN_PRINT, m_hBtn2);

	m_pStFont = CreateStatic("폰트", m_crCtrlBk, m_crBlack, m_crCtrlBk);

 	CRuntimeClass *pRClass = RUNTIME_CLASS(CAxBrowser);

	m_pBrowser = (CAxBrowser *)pRClass->CreateObject();
	if (m_pBrowser->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(), this, IDC_BROWSER))
	{
		m_pBrowser->SetFont(m_pFont);
	}

	PostMessage(WX_EVT_SIZE, 0, 0);
	LoadConfig();

	return 0;
}

void CMapWnd::OnDestroy() 
{
	SaveConfig();

	if (m_pStream) { m_pStream->Release(); m_pStream = NULL; }
	if (m_hGlobal) { GlobalFree(m_hGlobal); m_hGlobal = NULL; }


	if (m_pBtnCurr) { m_pBtnCurr->DestroyWindow();  }
	if (m_pBtnChart){ m_pBtnChart->DestroyWindow(); }
	if (m_pBtnMado) { m_pBtnMado->DestroyWindow();  }
	if (m_pBtnMasu) { m_pBtnMasu->DestroyWindow();  }
	if (m_pBtnPrint) { m_pBtnPrint->DestroyWindow(); }

	if (m_pCbFont)  { m_pCbFont->DestroyWindow(); }
	if (m_pStFont)  { m_pStFont->DestroyWindow(); }
	if (m_pStTitle) { m_pStTitle->DestroyWindow();}
	if (m_pBrowser) { delete m_pBrowser; }

	CWnd::OnDestroy();
}

LRESULT CMapWnd::OnEvtSize( WPARAM wParam, LPARAM lParam )
{
	LoadConfig();

	CWnd *pParent = GetParent();
	if (pParent)
	{
		pParent = pParent->GetParent();
		if (pParent)
		{
			pParent->SetWindowPos(NULL, m_rcConfig.left, m_rcConfig.top, m_rcConfig.Width(), m_rcConfig.Height(), SWP_SHOWWINDOW);
			pParent->ShowWindow(SW_SHOWNORMAL);
		}
	}
	return 0;
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	
	CalcSize();
}

LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	const int msg = LOBYTE(LOWORD(wParam));
	const int key = HIBYTE(LOWORD(wParam));
	const int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;
	
	switch(msg)
	{
	case DLL_INB:
		break;
		
	case DLL_OUB:
		switch(key)
		{
		case TK_PIBONEWS: ParsePIBONEWS(dat, len); break;
		}
		break;
		
	case DLL_OUBx:	//2015.11.05 KSJ 	DLL_OUBx 추가
		{
			char* pBytes = (char*)lParam;
			
			const struct _extTHx* extTH = (struct _extTHx*)pBytes;
			//pBytes += L_extTH;
			
			switch(extTH->key)
			{
				case TK_PIBONEWS: ParsePIBONEWS(extTH->data, extTH->size); break;
			}
		}
		break;

	case DLL_ALERT:
		break;
		
	case DLL_TRIGGER:
		break;
		
	case DLL_NOTICE:
		break;
		
	case DLL_SETPAL:
	case DLL_SETFONT:
	case DLL_SETFONTx:
		ChangeTheme();
		break;
		
	case DLL_GUIDE:
		return true;
		
	case DLL_DOMINO:
		ParseDomino(dat, strlen(dat));
		break;
	}
	return 0;
}

void CMapWnd::ParseDomino( LPCSTR dat, int lenes )
{
	vector<StringProxy> lines;
	ParseString(dat, dat+lenes, '\n', lines);

	m_bShowCodeBtn = FALSE;
	m_domino.RemoveAll();
	for(size_t n=0; n<lines.size(); ++n)
	{
		vector<StringProxy> fields;
		ParseString(lines[n], '\t',  fields);

		if (fields.size()<2) continue;

		CString key = fields[0].ToCStr(); key.TrimLeft(); key.TrimRight();
		CString val = fields[1].ToCStr(); val.TrimLeft(); val.TrimRight();

		if (key=="1301" && !val.IsEmpty())
		{
			m_bShowCodeBtn = TRUE;
			val.Replace(" ", "");
			if(val.GetAt(0) == 'A')
				m_code = val.Mid(1, 6);
			else
				m_code = val; 
		}

		if (key=="016" && !val.IsEmpty())
		{
			if (val.GetLength()>=8)	m_date = val.Left(8);
			else					m_date = "        ";
		}

		m_domino.SetAt(key, val);
	}

	UpdateTitle();

	CString strVal;
	if (m_domino.Lookup("016", strVal))
	{
		if (m_pBrowser) m_pBrowser->Navigate2("about:blank");
		ReqPIBONEWS(strVal);
	}

	if (!m_bDataFileLoad)
	{
		LoadCodeData();
		LoadRestrictData();
		m_bDataFileLoad = TRUE;
	}

	CalcSize();	// Button show/hide
}

void CMapWnd::UpdateTitle()
{
	if (!m_pStTitle) return;
	if (!m_pStTitle->GetSafeHwnd()) return;

	CString strVal;
	if (m_domino.Lookup("015", strVal))
		m_pStTitle->SetText(strVal);
}

void CMapWnd::ReqPIBONEWS( LPCSTR szKey )
{
	struct pibonews_mid mid;
	FillMemory(&mid, sizeof(mid), ' ');
	CopyMemory(&mid, szKey, min(sizeof(mid), strlen(szKey)));
	SendTR("pnewsrch", 0, (LPCSTR)&mid, sizeof(mid), TK_PIBONEWS);	//2013.08.05 KSJ pibf --> pibo로 수정	//2015.11.20 KSJ pibonews -> pibonew3 //2017.02.06 KSJ pibonew3 -> pnewsrch
	m_tick = GetTickCount();
}

void CMapWnd::ParsePIBONEWS( LPCSTR dat, int len )
{
	if ( len < sizeof(struct pibonews_mod) ) return;

	RenderContent(dat, len);
}

void CMapWnd::RenderContent(LPCSTR dat, int len)
{
	if (!m_pBrowser) return;

	m_tick = GetTickCount();
	const struct pibonews_mod *mod = (struct pibonews_mod *)dat;

	const int modL = sizeof(pibonews_mod);
	const int dlen = len - offsetof(struct pibonews_mod, data);
	if (dlen<=0) 
		return;

	int lineCnt = atoi(CString(mod->size, sizeof(mod->size)));

	len -= modL - 1; // title(132)+size(5)
	dat += modL - 1;

	CMapStringToString rest;
	CMapStringToString rlst;
	CopyStringMap(rest, m_rest);
	CopyStringMap(rlst, m_rlst);

	// 버퍼할당
	int pos = 0;
	char buff[8192 * 5]{};
	std::string head, body, tail;

	head.reserve( 2048 );
	body.reserve( len*4 );	// 약두배까지는 버퍼를 미리 할당하여, 메모리 realloc을 없앤다.
	tail.reserve( 1024 );

	BOOL bHtml=FALSE, bCodeInsert=FALSE;
	LPCSTR st = &dat[0];
	LPCSTR ed = &dat[0] + dlen;
	
	{
		LPCSTR tag = "text://";
		const int tlen = strlen(tag);
		bHtml = (memcmp(&dat[0], tag, tlen)==0);
		if (bHtml) st += tlen;
	}

	// 타이틀제작
	{		
		CString title(mod->titl, sizeof(mod->titl));
		title.TrimRight();

		pos += sprintf(&buff[pos], "<font face=\"굴림체\"><span style=\"font-size:%dpt;line-height:1.5em\">", m_fontsize);
		pos += sprintf(&buff[pos], "<b>&nbsp;&nbsp;제목 : %s [%.4s-%.2s-%.2s]</b>", (LPCSTR)title, ((LPCSTR)m_date+0), ((LPCSTR)m_date+4), ((LPCSTR)m_date+6));
		pos += sprintf(&buff[pos], "</span></font><br>");
//		pos += sprintf(&buff[pos], "</span></font><br>", (LPCSTR)title);
		for(int n=0; n<SCODE_CNT; ++n)
		{
			CString code(&mod->subcod[n][0], SCODE_SIZE);
			code.TrimRight();
			if (code.IsEmpty()) break;

			CString name = GetName(code);
			if (name.IsEmpty()) continue;
			name.TrimLeft(); name.TrimRight();

			rest.RemoveKey(name);

			bCodeInsert = TRUE;
			if (n==0)
			{
				pos += sprintf(&buff[pos], "<font face=\"굴림체\"><span style=\"font-size:%dpt;line-height:1.3em\">", m_fontsize);
				pos += sprintf(&buff[pos], "<b>&nbsp;&nbsp;종목 : ");
			}
			pos += sprintf(&buff[pos], "<a style=\"text-decoration:none\" href=\"axis://menu/%s\">%s</a>, ", (LPCSTR)code, (LPCSTR)name);
		}
		if (bCodeInsert) 
		{
			pos -= 2;	// 마지막 콤마를 없애준다.
			pos += sprintf(&buff[pos], "</b></span></font><br>");
		}
		pos += sprintf(&buff[pos], "<br>");
		
		head.insert(head.end(), &buff[0], &buff[0]+pos);
	}

// 	CString strTemp;

	if (bHtml)
	{
		pos = sprintf(buff, "<font style='font-size:%dpt'>", m_fontsize);
		body.insert(body.end(), buff, buff+pos);
		
		body.insert(body.end(), st, ed);
	
		pos = sprintf(buff, "</font>");
		body.insert(body.end(), buff, buff+pos);
	}
	else
	{
		const int nLine = len/LINE_SIZE;
		string sHEAD = "<html><body>";
		string sTAIL = "</body></html>";
		string sBR = "<br>";

		head.insert(head.begin(), sHEAD.begin(), sHEAD.end());

		pos = sprintf(buff, "<font face=\"굴림체\" style=\"font-size:%dpt;\"><pre>", m_fontsize);
		body.insert(body.end(), buff, buff+pos);

		for(int n=0; n<nLine; ++n)
		{
			LPCSTR line_st = (&dat[0]) + (n*LINE_SIZE);
			LPCSTR line_ed = line_st + LINE_SIZE;

			//	bind2end 람다식 변경 내용 확인 함더할것 escdream 2022.07.04
			line_st = find_if(line_st, line_ed, [& dat](char c)->bool { return c != ' '; });
			//			line_st = find_if(line_st, line_ed, bind2nd(not_equal_to<char>(), ' '));

			if(n == nLine -1)	//2012.12.12 KSJ 도중에 널값이 있는 것도 있음.
 				line_ed = find(line_st, line_ed, '\x00');	

			// 태그안인지 체크
			BOOL bTAG = FALSE;
			for(LPCSTR pos = line_ed-1; pos>=line_st; --pos)
			{
				if      (*pos=='<') { bTAG = TRUE; break; }
				else if (*pos=='>') { bTAG = FALSE; break; }
			}		
			
			body.insert(body.end(), line_st, line_ed);

			// 태그안이 아니고, 끝 글자가 한글의 시작이 아니면 <br>태그를 삽입한다.
			if (!bTAG) body.insert(body.end(), sBR.begin(), sBR.end());
		}

		pos = sprintf(buff, "</pre></font>");
		body.insert(body.end(), buff, buff+pos);

		tail.insert(tail.end(), sTAIL.begin(), sTAIL.end());
	}
	
	if (body.empty()) return;

	AdjustFont(body);
	ConvertLink(body);

	m_tick = GetTickCount();

	if (m_bIE6)
	{
		CString fpath;
		fpath.Format("%s\\%s\\%08u_news%d.html", (LPCSTR)m_home, (LPCSTR)USRDIR, HashKey<LPCSTR>(m_name), m_iOpenCount);
		FILE *fp = fopen(fpath, "wb");
		if (!fp) 
			return;

		fwrite(head.c_str(), 1, head.size(), fp);
		fwrite(body.c_str(), 1, head.size(), fp);
		fwrite(tail.c_str(), 1, head.size(), fp);
		// escdream 처리 및 데이타 내용 확인 후 업데이트
		//fwrite(head.begin(), 1, head.size(), fp);
		//fwrite(body.begin(), 1, body.size(), fp);
		//fwrite(tail.begin(), 1, tail.size(), fp);
		fflush(fp);
		fclose(fp);
		m_pBrowser->Navigate2("file:///" + fpath);
		m_sHtmlFile = fpath;
		SetTimer(2000, 2000, nullptr);
		//DeleteFile(fpath);
	}
	else
	{
		HRESULT hr{};
		ULONG ulWritten{};
		do {
			if (m_pStream) { m_pStream->Release(); m_pStream = NULL; }
			if (m_hGlobal) { GlobalFree(m_hGlobal); m_hGlobal = NULL; }
			
			m_hGlobal = GlobalAlloc(GHND, head.size()+body.size()+tail.size());
			if (m_hGlobal==NULL) break;
			
			hr = CreateStreamOnHGlobal(m_hGlobal, TRUE, &m_pStream);
			if (FAILED(hr) || m_pStream==NULL) break;
			
			hr = m_pStream->Write(&head[0], head.size(), &ulWritten);
			if (FAILED(hr)) break;
			hr = m_pStream->Write(&body[0], body.size(), &ulWritten);
			if (FAILED(hr)) break;
			hr = m_pStream->Write(&tail[0], tail.size(), &ulWritten);
			if (FAILED(hr)) break;
			
			LARGE_INTEGER pos;
			pos.QuadPart = 0;
			hr = m_pStream->Seek(pos, STREAM_SEEK_SET, NULL);
			if (FAILED(hr)) break;
			
			m_pBrowser->NavigateFromStream(m_pStream);
		} while(FALSE);
	}
}

void CMapWnd::LoadCodeData()
{
	
/*
	vector<std::pair<CString, CString>> codes;

	AxStd::getArray(codes, 0);

	vector<std::pair<CString, CString>>::iterator iter;
	for (iter = codes.begin(); iter != codes.end(); iter++) {
		m_codes.SetAt((LPCSTR)*iter->first, (LPCSTR)*iter->second);
		m_names.SetAt((LPCSTR)*iter->second, (LPCSTR)*iter->first);
		m_snames.Add(*iter->second);
	}
*/	

	CString path;
	path.Format("%s\\%s\\%s", (LPCTSTR)m_home, TABDIR, HJCODE);

	vector<char> data;
	ReadFileData(path, data);



	const int nItem = data.size() / sizeof(struct hjcode);
	struct hjcode *hjcode = (struct hjcode *)&data[0];
	for(int n=0; n<nItem; ++n, ++hjcode)
	{
		if (hjcode->codx[0]!='A') continue;
		CString code(hjcode->codx, sizeof(hjcode->codx)); code.TrimLeft(); code.TrimRight();
		CString name(hjcode->hnam, sizeof(hjcode->hnam)); name.Delete(0);  name.TrimRight();
		m_codes.SetAt(code, name);
		m_names.SetAt(name, code);
		m_snames.Add(name);
	}
	m_snames.Sort();

}

CString CMapWnd::GetName( LPCSTR code )
{
	CString hnam;
	m_codes.Lookup(code, hnam);
	return hnam;
}

CString CMapWnd::GetCode( LPCSTR name )
{
	CString code;
	m_names.Lookup(name, code);
	return code;
}

void CMapWnd::LoadRestrictData()
{
	CString path;
	path.Format("%s\\%s\\NEWS_RESTRICT_LIST", (LPCTSTR)m_home, (LPCTSTR)TABDIR);

	vector<char> data;
	ReadFileData(path, data);

	vector<StringProxy> lines;

	const char * array = &data[0];

//	ParseString(data.begin(), data.end(), '\n', lines);
	ParseString(&array[0], &array[data.size()], '\n', lines);

	for(size_t n=0; n<lines.size(); ++n)
	{
		int pos;
		CString data = lines[n].ToCStr();
		data.TrimLeft();
		data.TrimRight();

		if ( (pos=data.Find("LAST|")) >= 0 )
			m_rlst.SetAt(data.Mid(pos+5), "");
		else
			m_rest.SetAt(data, "");
	}
}

void CMapWnd::ReadFileData( LPCSTR path, vector<char> &data )
{
	data.resize(0);

	FILE *fp = fopen((LPCSTR)path, "rb");
	if (!fp) return;

	struct stat sbuf;
	fstat(fileno(fp), &sbuf);
	
	data.resize(sbuf.st_size);

	long rsiz = 0;
	while( (rsiz<sbuf.st_size) && !feof(fp) )
		rsiz += fread(&data[rsiz], 1, sbuf.st_size-rsiz, fp);

	if (rsiz!=sbuf.st_size)
		data.resize(rsiz);

	fclose(fp);
}

void CMapWnd::CopyStringMap( CMapStringToString &dst, const CMapStringToString &src )
{
	dst.RemoveAll();
	POSITION pos = src.GetStartPosition();
	CString key, val;
	while(pos!=NULL)
	{
		src.GetNextAssoc(pos, key, val);
		dst.SetAt(key, val);
	}
}

void CMapWnd::ConvertLink( string &body )
{
	int blen{};
	char buff[1024]{};
	LPCSTR key{};
	string cbody;
	CharFinder t_finder("<>");
	cbody.reserve(body.size()*2);

	CString code;
	const int nName = m_snames.GetSize();
	for(int n=0; n<nName; ++n)
	{
		string etags[] = {"<body>", "<br>"};
		string name = (LPCSTR)m_snames.ElementAt(n);
		string::iterator st = body.begin();
		string::iterator ed = body.end();
		
		if (m_rest.LookupKey(name.c_str(), key)) continue;
		if (!m_names.Lookup(name.c_str(), code)) continue;
		blen = sprintf(buff, "<a style=\"text-decoration:none\" href=\"axis://menu/%s\">%s</a>", (LPCSTR)code, (LPCSTR)name.c_str());

		cbody.resize(0);
		while(st<ed)
		{
			string::iterator pos = search(st, ed, name.begin(), name.end());
			if (pos==ed)
			{
				cbody.insert(cbody.end(), st, ed);
				// st = pos + 1;				
				st = pos;
			}
			else
			{
				if ( pos>st && (pos+name.size())<=ed )
				{
					string::iterator bpos = pos-1;
					string::iterator npos = pos+name.size();
	
					if (*bpos=='>' || *npos=='<')
					{
						bool bConvert = false;
						for(int n=0; n<(sizeof(etags)/sizeof(string)); ++n)
						{
							if ((unsigned)(pos-st)>=etags[n].size())
							{
								string::iterator pos2 = pos-etags[n].size();
								if ( memcmp(&*pos2, etags[n].c_str(), etags[n].size()) == 0 )
								{
									bConvert = true;
									cbody.insert(cbody.end(), st, pos);
									cbody.insert(cbody.end(), buff, buff+blen);
									break;
								}
							}
						}
						
						if (!bConvert)
						{
							// 앞뒤가 바로 >,<로 끝나면 이미 다른종목명으로 Converting 된놈이다. 
							// 아니면 원문에 링크가 걸려있거나
							cbody.insert(cbody.end(), st, pos+name.size());
						}
					}
					else
					{
						string::reverse_iterator rst(pos), red(st), rpos;
						rpos = find_if(rst, red, t_finder);
						if (rpos!=red)
						{
							if (*rpos=='<')
							{
								// 발견위치 앞쪽으로 <>중 <가 먼저 발견되면 태그내의 Attribute값으로 판단, 무시한다.
								cbody.insert(cbody.end(), st, pos+name.size());
							}
							else
							{
								// 태그가 아니라고 판단하여 변환한다.
								cbody.insert(cbody.end(), st, pos);
								cbody.insert(cbody.end(), buff, buff+blen);
							}
						}
						else
						{
							// 태그관련 기호(<>)가 없으면 링크변환한다.
							cbody.insert(cbody.end(), st, pos);
							cbody.insert(cbody.end(), buff, buff+blen);
						}
					}
				}
				else if (pos==st)	// 본문시작에서 종목명 나올때
				{
					cbody.insert(cbody.end(), st, pos);
					cbody.insert(cbody.end(), buff, buff+blen);
				}
				else
				{
					cbody.insert(cbody.end(), st, pos+name.size());
				}
				st = pos + name.size();
			}
		}

		body.assign(cbody.begin(), cbody.end());
	}
}

void CMapWnd::AdjustFont( string &body )
{


	int blen;
	char buff[64];
	blen = sprintf_s(buff, "font-size:%dpt", m_fontsize);

	string ftag = "font-size:12px";
	string::iterator st = body.begin();
	string::iterator ed = body.end();

	if (body.find(ftag, ftag.length()) != string::npos) {
		body.replace(body.find(ftag, ftag.length()), ftag.length(), buff);
	}
	else {
	}

	//while(st<ed)
	//{
	//	string::iterator pos = search(st, ed, ftag.begin(), ftag.end());
	//	if (pos != ed) {
	//		cbody.insert(cbody.end(), st, pos);
	//	}
	//	
	//	cbody.insert(cbody.end(), buff, buff + blen);
	//	st = pos + ftag.size();
	//}
	//body.assign(cbody.begin(), cbody.end());

}

void CMapWnd::CalcSize()
{
	CRect rc;
	GetClientRect(&rc);
	
	m_rcCtrl.SetRect(rc.left, rc.top, rc.right, rc.top+CTRL_WND_HEIGHT);
	m_rcBody.SetRect(rc.left, rc.top+CTRL_WND_HEIGHT-2, rc.right, rc.bottom);
	
	{
		rc = m_rcCtrl;
		rc.DeflateRect(PADDING_VALUE, PADDING_VALUE/2);
		
		int pos_x = rc.right;
		const int pos_y = rc.top;
		const int line_h = 20;

		pos_x -= 36; if (m_pCbFont) m_pCbFont->MoveWindow(pos_x, pos_y, 36, pos_y+160); pos_x -= PADDING_VALUE/3;	//2014.10.21 KSJ 콤보박스크기조절
		pos_x -= 24; if (m_pStFont) m_pStFont->MoveWindow(pos_x, pos_y, 24, line_h); pos_x -= PADDING_VALUE/3;
		pos_x -= 34; if (m_pBtnPrint) m_pBtnPrint->MoveWindow(pos_x, pos_y, 34, line_h); pos_x -= PADDING_VALUE/3;

		if (m_pBtnMasu)  m_pBtnMasu->ShowWindow(m_bShowCodeBtn ? SW_SHOW : SW_HIDE);
		if (m_pBtnMado)  m_pBtnMado->ShowWindow(m_bShowCodeBtn ? SW_SHOW : SW_HIDE);
		if (m_pBtnChart) m_pBtnChart->ShowWindow(m_bShowCodeBtn ? SW_SHOW : SW_HIDE);
		if (m_pBtnCurr)  m_pBtnCurr->ShowWindow(m_bShowCodeBtn ? SW_SHOW : SW_HIDE);
		if (m_bShowCodeBtn)
		{
			pos_x -= 34; if (m_pBtnMasu)  m_pBtnMasu->MoveWindow(pos_x, pos_y, 34, line_h);  pos_x -= PADDING_VALUE/3;
			pos_x -= 34; if (m_pBtnMado)  m_pBtnMado->MoveWindow(pos_x, pos_y, 34, line_h);  pos_x -= PADDING_VALUE/3;
			pos_x -= 34; if (m_pBtnChart) m_pBtnChart->MoveWindow(pos_x, pos_y, 34, line_h); pos_x -= PADDING_VALUE/3;
			pos_x -= 42; if (m_pBtnCurr)  m_pBtnCurr->MoveWindow(pos_x, pos_y, 42, line_h);  pos_x -= PADDING_VALUE/3;
		}
		if (m_pStTitle) m_pStTitle->MoveWindow(rc.left, rc.top, pos_x-rc.left, line_h, FALSE);
	}
	{
		rc = m_rcBody;
		rc.DeflateRect(5, 5);
		if (m_pBrowser)
			m_pBrowser->MoveWindow(&rc, FALSE);
	}
	
	MakeBackground();
}

void CMapWnd::LoadConfig()
{
	LPCSTR szNews = "NEWSXX";
	CString path;
	path.Format("%s\\User\\%s\\u_config.ini", (LPCTSTR)m_home, (LPCTSTR)m_name);

	//2016.12.13 KSJ 모니터 체크해서 이상한 값이면 0으로 초기화
	const int nScrXMax = GetSystemMetrics(SM_CXVIRTUALSCREEN); 
	const int nScrYMax = GetSystemMetrics(SM_CYVIRTUALSCREEN); 

	LONG lTop = 0, lLeft = 0, lRight = 0, lWidth = 0;

	lTop = GetPrivateProfileInt(szNews, "top",  0, path);
	lLeft = GetPrivateProfileInt(szNews, "left",  0, path);

	if(lTop > 0 && lTop < nScrYMax)		m_rcConfig.top = lTop;
	else								m_rcConfig.top = 0;

	m_rcConfig.bottom = m_rcConfig.top  + GetPrivateProfileInt(szNews, "height",  550, path);

	if(lLeft > 0 && lLeft < nScrXMax)	m_rcConfig.left = lLeft;
	else								m_rcConfig.left = 0;

	lWidth = GetPrivateProfileInt(szNews, "width",  220, path);
	lRight = m_rcConfig.left + lWidth;

	if(lRight > 0 && lRight < nScrXMax)	m_rcConfig.right = lRight;
	else
	{
		m_rcConfig.left -= lRight - nScrXMax;
		m_rcConfig.right = m_rcConfig.left + lWidth;
	}
	//2016.12.13 KSJ ENd

	const int idx = GetPrivateProfileInt(szNews, "fontsize", 1, path);
	if (sizeof(font_size)/sizeof(LPCSTR)>idx && idx>=0)
	{
		m_pCbFont->SetCurSel(idx);
		m_fontsize = atoi(font_size[idx]);
	}
}

void CMapWnd::SaveConfig()
{
	LPCSTR szNews = "NEWSXX";
	CString path, val;
	path.Format("%s\\User\\%s\\u_config.ini", (LPCTSTR)m_home, (LPCTSTR)m_name);

	if (m_pCbFont)
	{
		val.Format("%d", m_pCbFont->GetCurSel());
		WritePrivateProfileString(szNews, "fontsize", val, path);
	}

	CWnd *pWnd = GetParent();
	if (!pWnd) return;
	pWnd = pWnd->GetParent();
	if (!pWnd) return;

	CRect rc;
	pWnd->GetWindowRect(&rc);
	val.Format("%d", rc.top);		WritePrivateProfileString(szNews, "top",    val, path);
	val.Format("%d", rc.left);		WritePrivateProfileString(szNews, "left",   val, path);
	val.Format("%d", rc.Height());	WritePrivateProfileString(szNews, "height", val, path);
	val.Format("%d", rc.Width());	WritePrivateProfileString(szNews, "width",  val, path);
}

void CMapWnd::OnFontChange()
{
	CString strVal;

	if (m_pCbFont)
	{
		m_pCbFont->GetWindowText(strVal);
		m_fontsize = atoi(strVal);
		if (m_fontsize == 0) {
			m_fontsize = 9;
		}
	}


	if (m_domino.Lookup("016", strVal))
	{
		//if (m_pBrowser) m_pBrowser->Navigate2("about:blank");
		if (strVal.GetLength()>=8)	m_date = strVal.Left(8);
		else						m_date = "        ";
		ReqPIBONEWS(strVal);
	}
}

void CMapWnd::OnBtnCurr()
{
	CString mapn;
	mapn.Format("IB200100 /t/S/d1301\t%s\n", (LPCSTR)m_code);
	SetView(mapn);
}

void CMapWnd::OnBtnChart()
{
	CString mapn;
	mapn.Format("IB711200 /t/S/d1301\t%s\n", (LPCSTR)m_code);	//2013.01.10 KSJ 신차트로 변경
	SetView(mapn);
}

void CMapWnd::OnBtnMado()
{
	CString mapn;
	mapn.Format("IB101100 /t/S/d1301\t%s\n", (LPCSTR)m_code);
	SetView(mapn);
}

void CMapWnd::OnBtnMasu()
{
	CString mapn;
	mapn.Format("IB101200 /t/S/d1301\t%s\n", (LPCSTR)m_code);
	SetView(mapn);
}

void CMapWnd::OnBtnPrint()
{
	CString	date;

	SAFEARRAYBOUND psabBounds{};
	SAFEARRAY* psaHeadFoot{};
	HRESULT hr = S_OK;

	HGLOBAL hG = 0;
	IStream *pStream= NULL;
	IUnknown *pUnk = NULL;
	ULONG lWrote = 0;
	LPSTR sMem = NULL;
	VARIANT vHeadStr{}, vFootStr{}, vHeadTxtStream{};
	long rgIndices = 0;

	try {
		if (!m_pBrowser) 
		{
			TRACE(_T("DoPrint: Cannot print - WebBrowser control not ready\n"));
			//goto cleanup;
			throw;
		}
	
		// Initialize header and footer parameters to send to ExecWB().
		psabBounds.lLbound = 0;
		psabBounds.cElements = 3;
		psaHeadFoot = SafeArrayCreate(VT_VARIANT, 1, &psabBounds);
		if (NULL == psaHeadFoot) 
		{
//			goto cleanup;
			throw;
		}

		VariantInit(&vHeadStr);
		VariantInit(&vFootStr);
		VariantInit(&vHeadTxtStream);

		// Argument 1: Header
		vHeadStr.vt = VT_BSTR;
		vHeadStr.bstrVal = SysAllocString(L"");
		if (vHeadStr.bstrVal == NULL)
		{
//			goto cleanup;
			throw;
		}

		// Argument 2: Footer
		vFootStr.vt = VT_BSTR;
		vFootStr.bstrVal = SysAllocString(L"");
		if (vFootStr.bstrVal == NULL)
		{
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
//			goto cleanup;
			throw;
		}

		// Argument 3: IStream containing header text. Outlook and Outlook 
		if ((sMem = (LPSTR)CoTaskMemAlloc(512)) == NULL)
		{
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
//			goto cleanup;
			throw;
		}

		sprintf(sMem, "<html><font face= %s><span style=\"font-size:%dpt;\"><pre></html>\0", "굴림체", 9);

		// Allocate an IStream for the LPSTR that we just created.
		hG = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, strlen(sMem));
		if (hG == NULL) {
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}
		hr = CreateStreamOnHGlobal(hG, TRUE, &pStream);

		if (pStream == NULL) { throw; } // goto cleanup; //

		if (FAILED(hr)) {
			TRACE(_T("OnPrint::Failed to create stream from HGlobal: %lX\n"), hr);
			throw;
			//goto cleanup;
		}
		hr = pStream->Write(sMem, strlen(sMem), &lWrote);
		if (SUCCEEDED(hr)) {
			// Set the stream back to its starting position.
			LARGE_INTEGER pos;
			pos.QuadPart = 0;
			pStream->Seek((LARGE_INTEGER)pos, STREAM_SEEK_SET, NULL);
			hr = pStream->QueryInterface(IID_IUnknown, reinterpret_cast<void**>(&pUnk));
			vHeadTxtStream.vt = VT_UNKNOWN;
			vHeadTxtStream.punkVal = pUnk;
		}

		rgIndices = 0;
		SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vHeadStr));
		rgIndices = 1;
		SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vHeadTxtStream));
		rgIndices = 2;
		SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vFootStr));

		// SAFEARRAY must be passed ByRef or else MSHTML transforms it into NULL.
		VARIANT vArg;
		VariantInit(&vArg);
		vArg.vt = VT_ARRAY | VT_BYREF;
		vArg.parray = psaHeadFoot;
		m_pBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_PROMPTUSER, &vArg, NULL);

		return; //WebBrowser control will clean up the SAFEARRAY after printing.
	}
	catch (...) {
		VariantClear(&vHeadStr);
		VariantClear(&vFootStr);
		VariantClear(&vHeadTxtStream);
		if (psaHeadFoot) {
			SafeArrayDestroy(psaHeadFoot);
		}
		if (sMem) {
			CoTaskMemFree(sMem);
		}
		if (hG != NULL) {
			GlobalFree(hG);
		}
		if (pStream != NULL) {
			pStream->Release();
			pStream = NULL;
		}
	}

//cleanup:
//
//	VariantClear(&vHeadStr);
//	VariantClear(&vFootStr);
//	VariantClear(&vHeadTxtStream);
//	if (psaHeadFoot) {
//		SafeArrayDestroy(psaHeadFoot);
//	}
//	if (sMem) {
//		CoTaskMemFree(sMem);	
//	}
//	if (hG != NULL) {
//		GlobalFree(hG);
//	}
//	if (pStream != NULL) {
//		pStream->Release();
//		pStream = NULL;
//	}
}

LRESULT CMapWnd::OnEvtMapn( WPARAM wParam, LPARAM lParam )
{
	LPCSTR mapn = (LPCSTR)lParam;
	SetView(mapn);

	CString code, data = mapn;
	AfxExtractSubString(code, data, 1, '\t');

	SetTrigger(code);	//2012.21.07 KSJ 트리거 보내도록 수정
	return 0;
}

void CMapWnd::ChangeTheme()
{
	AxInit();

	m_pStTitle->SetBkColor(m_crCtrlBk);
	m_pStTitle->SetBk2Color(m_crCtrlBk);
	m_pStTitle->SetBorderColor(m_crCtrlBk);
	m_pStFont->SetBkColor(m_crCtrlBk);
	m_pStFont->SetBk2Color(m_crCtrlBk);
	m_pStFont->SetBorderColor(m_crCtrlBk);
	
	LoadImage();
	MakeBackground();
	Invalidate(TRUE);
}

void CMapWnd::OnClose() 
{
	CWnd::OnClose();
}

void CMapWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);
}

LRESULT CMapWnd::OnEvtRefresh( WPARAM wParam, LPARAM lParam )
{
	if (m_hGlobal && m_pStream)
		m_pBrowser->NavigateFromStream(m_pStream);
	return 0;
}

CString CMapWnd::GetIEVersion()
{
	CString vs = "8";
	
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer"), NULL, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		DWORD dwType{}, dwSize{};
		TCHAR szVers[128] = {0,};
		if (RegQueryValueEx(hKey, _T("Version"), NULL, &dwType, (PBYTE)szVers, &dwSize) == ERROR_SUCCESS)
		{
			szVers[dwSize] = 0;
			vs.Format("%c", szVers[0]);
		}
		RegCloseKey(hKey);
	} 
	return vs;
}

void CMapWnd::testSaveFile3(CString code, CString datB)
{
	// 	if(m_user.Compare("q123356"))	return;	//2014.10.10 KSJ 관심종목 저장하는 부분
	
	CString strPath(_T("")), titletime;
	strPath.Format("%s\\NewsSave.dat", (LPCSTR)m_home);
	
	SYSTEMTIME stime;
	GetLocalTime(&stime);
	
	CString strCurTime;
	strCurTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stime.wYear, stime.wMonth, stime.wDay, stime.wHour, stime.wMinute, stime.wSecond);
	
	CStdioFile file;
	
	file.Open(strPath, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	file.SeekToEnd(); 	
	
	titletime.Format("[%s][%s][%s]\n", (LPCSTR)strCurTime, (LPCSTR)code, (LPCSTR)datB);
	
	file.WriteString(titletime);
	file.Close();	
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	//KillTimer(1000);
	

	if(nIDEvent == 1000)
	{
		CString strVal;
		m_domino.Lookup("016", strVal);
		ReqPIBONEWS(strVal);	
	}
	else if (nIDEvent == 2000)
	{
		KillTimer(nIDEvent);
		DeleteFile(m_sHtmlFile);
	}

	CWnd::OnTimer(nIDEvent);
}
