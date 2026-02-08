
#include "stdafx.h"
#include "IB0000X1.h"
#include "MapWnd.h"

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#include "ioformat.h"

#define TRKEY_GROUPARR 99

#include "../../h/jmcode.h"
#include "../../h/interSt.h"
#include "sys/stat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct FIELD_INFO FIELD_INFO[] = {
	{ "1301", "301", "CODE"      , TRUE,  FALSE,  0 },
	{ "1021", "021", "RTS코드"   , TRUE,  FALSE,  0 },
	{ "2934", "934", "전일종가"  , TRUE,  FALSE,  0 },
	{ "2033", "033", "등락률"    , TRUE,  FALSE,  0 },
	{ "1022", "022", "종목명"    , TRUE,  TRUE,  80 },
	{ "2023", "023", "현재가"    , FALSE, TRUE,  55 },
	{ "2024", "024", "전일대비"  , FALSE, TRUE,  55 },
	{ "2033", "033", "등락률"    , FALSE, TRUE,  55 },
	{ "2027", "027", "거래량"    , FALSE, TRUE,  65 },
	{ "2029", "029", "시가"      , FALSE, TRUE,  55 },
	{ "2030", "030", "고가"      , FALSE, TRUE,  55 },
	{ "2031", "031", "저가"      , FALSE, TRUE,  55 },
	{ "3101", "101", "매도총잔량", FALSE, TRUE,  55 },
	{ "3106", "106", "매수총잔량", FALSE, TRUE,  55 },
	{ NULL, NULL, NULL, NULL, NULL, NULL }
};

#define STR(field)	CString(field, sizeof(field))

CMapWnd::CMapWnd(CWnd *pParent) : CAxisExt(pParent)
{
	m_transparent = 50;
	m_bFlash = TRUE;
	m_bSimple = TRUE;
	m_bTracking = TRUE;
	m_bTopMost = TRUE;
	m_bExit = FALSE;
	m_pOldHoverWnd = NULL;
	m_vtype = VT_HORIZONTAL;
	m_newX = DEF_POPUP_POSX;
	m_newY = DEF_POPUP_POSY;
	m_crPopupBack = COLOR_DEF_POPUP_BACK;
	m_crPopupFlash = COLOR_DEF_POPUP_FLASH;

	m_mainWnd = NULL;

	m_bMiniMode = FALSE;

	m_pHistroy = NULL;
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(220,450));

	LoadCodes();
	LoadInterest();
}

CMapWnd::~CMapWnd()
{
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WX_VIEW_PROPERTY, OnViewProperty)
	ON_MESSAGE(WX_CLOSE_WINDOW, OnChildClose)
	ON_MESSAGE(WX_SET_TRACKING, OnSetTracking)
	ON_MESSAGE(WX_LASTPOS_UPDATE, OnLastPosUpdate)
	ON_MESSAGE(WX_ALIGN_H_UPPER, OnAlignHUpper)
	ON_MESSAGE(WX_ALIGN_H_LOWER, OnAlignHLower)
	ON_MESSAGE(WX_ALIGN_V_UPPER, OnAlignVUpper)
	ON_MESSAGE(WX_ALIGN_V_LOWER, OnAlignVLower)
	ON_MESSAGE(WX_CHANGE_CODE, OnCodeChange)
	ON_MESSAGE(WX_ADD_NEWCODE, OnAddNewCode)
	ON_MESSAGE(WD_ADD_CODE, OnAddNewCode)
	ON_MESSAGE(WD_CLOSE_CODE, OnChildClose)
	ON_MESSAGE(WD_SETWND, OnSetMainWnd)
	ON_MESSAGE(WD_HTSCLOSEFORMINI, OnHTSCloseFMini)
	ON_MESSAGE(WF_FLASH,OnWFFlash)
	ON_MESSAGE(WF_VIEWTYPE,OnWFViewType)
	ON_MESSAGE(WF_BKCOLOR,OnWFBkColorChange)
	ON_MESSAGE(WF_FLASHCOLOR,OnWFFlashColorChange)
	ON_MESSAGE(WF_SIMPLECOLOR,OnWFSimpleColor)
	ON_MESSAGE(WF_TOPMOST,OnWFTopMost)
	ON_MESSAGE(WF_FIELDCHANGE,OnWFFieldChange)
	ON_MESSAGE(WD_LASTVIEW,OnLastView)
	ON_MESSAGE(WD_CODENEWS,OnCodeNews)
	ON_MESSAGE(WX_INTEREST, OnInterest)
	ON_MESSAGE(WX_NEWSVIEW, OnNewsView)
	ON_MESSAGE(WX_HOTTRADE, OnHotTrace)
	ON_MESSAGE(WD_TRANSMAX, OnTransMax)
	ON_MESSAGE(WD_TRANSMIN, OnTransMin)
	ON_MESSAGE(WD_HISTORYVIEW, OnHistoryView)
	ON_MESSAGE(WD_HISTORYCLOSE, OnHistoryClose)
	ON_MESSAGE(WD_WINE, OnWine)
	ON_MESSAGE(WD_WINEWND, OnWineWnd)
	ON_MESSAGE(WD_INGONG, OnInGong)
	ON_BN_CLICKED(IDC_BTN_FLASH, OnFlash)
	ON_BN_CLICKED(IDC_BTN_VIEWTYPE, OnViewType)
	ON_BN_CLICKED(IDC_BTN_CODEADD, OnCodeAdd)
	ON_BN_CLICKED(IDC_BTN_BKCOLOR, OnBkColorChange)
	ON_BN_CLICKED(IDC_BTN_FLASHCOLOR, OnFlashColorChange)
	ON_BN_CLICKED(IDC_BTN_SIMPLECOLOR, OnSimpleColor)
	ON_BN_CLICKED(IDC_BTN_TOPMOST, OnTopMost)
	ON_BN_CLICKED(IDC_BTN_INTERADD, OnInterAdd)
	ON_BN_CLICKED(IDC_BTN_LASTVIEW, OnBtnLastView)
	ON_BN_CLICKED(IDC_BTN_CODENEWS, OnBtnCodeNews)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_FIELD_START, IDC_BTN_FIELD_START+100, OnFieldChange)
END_MESSAGE_MAP()

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
		if (key == TRKEY_GROUPARR)
		{
			CString stmp, seq, sgname;
			char* pdata = (char*)lParam;
			int cnt = atoi(CString(pdata, 4));
			if (cnt > 0)
			{
				int parseL = 4;
				struct _grSearch
				{
					char ngrs[2];
					char grseq[2];
					char gname[30];
				};

				struct _grSearch* stgr{};
				for (int ii = 0; ii < cnt; ii++)
				{
					stgr = (struct _grSearch*)&pdata[parseL];
					seq.Format("%.2s", stgr->grseq);
					sgname.Format("%.30s", stgr->gname);
					sgname.TrimRight();
					seq.TrimRight();
					if (seq.GetLength() == 0)
						break;

					m_gpno.Add(seq);
					m_gpnm.Add(sgname);

					sgname.Empty();
					seq.Empty();
					parseL += sizeof(struct _grSearch);
				}
			}
			return 0;
		}

		if (key>=TR_STARTKEY)
		{
			ParseSise(dat, len);
		}
		//와인서비스
		else if(key==TR_WINEKEY)
		{
			//전송할 도미노 데이타
// 			CString domino;
// 			domino.Format("%s?id=[%s]&handle=[%d]",SURL, m_user, m_mainWnd->GetSafeHwnd());
// 			OutputDebugString("[KSJ] " + domino);

			ParseSACAQ601((char*)lParam);
		}
		break;
		
	case DLL_ALERT:
		//ParseAlert(dat, strlen(dat));
		break;
	case DLL_ALERTx :
		ParseAlertx((struct _alertR*)lParam);
		break;
	case DLL_TRIGGER:
		{
//			OutputDebugString("DLL_TRIGGER\n");
			CString value = (LPCSTR)lParam;
//			OutputDebugString(value);
//			OutputDebugString("\n");
			CString s;
			s.Format("$HIWORD %d",len);
//			OutputDebugString(s);
//			OutputDebugString("$==========================\n");
			if(HIWORD(wParam) == false)
			{
//				OutputDebugString("$OK\n");
				CString strSym,strValue;

				strSym = value.Left(value.Find('\t') + 1);
				strValue = value.Right(value.GetLength() - value.Find('\t') - 1);

				strSym.TrimLeft();
				strSym.TrimRight();
				
				if(strSym == "$FLASH")
				{
					SendMessage(WF_FLASH,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$FIELD")
				{
					SendMessage(WF_FIELDCHANGE,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$FLASHCOLOR")
				{
					SendMessage(WF_FLASHCOLOR,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym =="$BKCOLOR")
				{
					SendMessage(WF_BKCOLOR,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$TOPMOST")
				{
					SendMessage(WF_TOPMOST,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$SIMPLECOLOR")
				{
					SendMessage(WF_SIMPLECOLOR,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$VIEWTYPE")
				{
					SendMessage(WF_VIEWTYPE,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$ADDCODE")
				{
					SendMessage(WD_ADD_CODE,(WPARAM)0,(LPARAM)(LPCSTR)strValue);
				}
				else if(strSym == "$LASTVIEW")
				{
					//OutputDebugString("LASTVIEW\n");
					SendMessage(WD_LASTVIEW,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$CODENEWS")
				{
					SendMessage(WD_CODENEWS,(WPARAM)0,(LPARAM)atoi(strValue));
				}
				else if(strSym == "$TRANS")
				{
					m_transparent = atoi(strValue);

					CString key;
					CSisePopup* pwnd{};
					POSITION pos = m_wnds.GetStartPosition();
					while(pos!=NULL)
					{
						m_wnds.GetNextAssoc(pos, key, pwnd);
						pwnd->SetTransparent(m_transparent);
					}

					m_sbTransparent.SetPos(m_transparent);
				}
				else if(strSym == "$HTSCLOSE")
				{
					if(m_mainWnd->GetSafeHwnd())
					{
						m_mainWnd->SendMessage(WD_HTSCLOSEFORMINI,(WPARAM)0,(LPARAM)0);
					}
				}
				else if(strSym == "cfcod")
				{
					CString strHis;
					strHis.Format("%s\t%s","100301",strValue);
					Variant(codeCC,strHis);
					Variant(triggerCC,strHis);
				}
				else if(strSym == "$ISDEV")
				{
					const int isdev = (int)m_mainWnd->SendMessage(WD_ISDEV,(WPARAM)0,(LPARAM)0);

					return isdev;
				}
				else if(strSym == "$ISSTAFF")
				{
					const int isstaff = (int)m_mainWnd->SendMessage(WD_STAFF,(WPARAM)0,(LPARAM)0);
					
					return isstaff;
				}
				else if(strSym == "$WEBHWND")
				{
					CWnd* pwnd = (CWnd*)(atol(strValue));
					m_mainWnd->SendMessage(WD_WEBHWND,(WPARAM)0,(LPARAM)(LONG)pwnd);
				}
				else if(strSym == "$CLOSEWEB")
				{
//					OutputDebugString("$CLOSERSWEB\n");
					m_mainWnd->SendMessage(WD_CLOSEWEB,(WPARAM)0,(LPARAM)0);
				}
// 				else if(strSym == "1301")
// 				{
// 					if(strValue.GetAt(0) == 'A')
// 					{
// 						strValue = strValue.Mid(1);
// 					}
// 
// 					int wParam = atoi(strValue);
// 					::SendMessage(m_wineHwnd, WM_IBK_CODE, wParam, 0);
// 				}
				else if(strSym == "$PHONEPAD")
				{
					m_mainWnd->SendMessage(WD_PHONEPAD,(WPARAM)0,(LPARAM)(LPCSTR)strValue);
				}
				else if(strSym.Find("$SAVEINTER") > -1)
				{
//					OutputDebugString("RCV $SAVEINTER\n");

					//if(m_mainWnd->GetSafeHwnd())
					m_mainWnd->SendMessage(WM_RESINTEREST,(WPARAM)0,(LPARAM)1);
//					OutputDebugString("RCV END\n");
				}
				else if(strSym.Find("$CAPTURE") > -1)
				{
					m_mainWnd->SendMessage(WD_CAPTURE,(WPARAM)0,(LPARAM)1);
				}
			}
		}
		break;
		
	case DLL_NOTICE:
		break;
		
	case DLL_SETPAL:
	case DLL_SETFONT:
	case DLL_SETFONTx:
		break;
		
	case DLL_GUIDE:
		return true;
		
	case DLL_DOMINO:
		break;
	}
	return 0;
}

void CMapWnd::ParseSise( LPCSTR dat, int len )
{
	CString strsym;
	LPCSTR st = dat + sizeof(GRID_O);
	LPCSTR ed = dat + (len-3);	// 마지막에 0A 0D 09 오는것은 버린다

	vector<StringProxy> line, field;
	ParseString(st, ed, 0x0A, line);
	for(size_t n=0; n<line.size(); ++n)
	{
		ParseString(line[n], 0x09, field);
		if (m_fields.GetSize() != field.size()) continue;	// 요청한 필드와 사이즈가 틀리면 Skip한다.

		CString key = field[0].ToCStr(); // 1301 CODE
		CString rts = field[1].ToCStr(); // 1021 RTSKEY

		m_rtsmap.SetAt(rts, key);

		CMapStringToString *val;
		
		if (!m_datas.Lookup(key, val))
		{
	//		val = new CMapStringToString();
			auto& item =_vData.emplace_back(std::make_unique<CMapStringToString>());
			m_datas.SetAt(key, item.get());
			val = item.get();
		}

		for(int i=0; i<m_fields.GetSize(); ++i)
		{
			CString fval = field[i].ToCStr();
			fval.TrimRight();
			fval.TrimLeft();
			//val->SetAt( m_fields[i]->rsym, fval );

			strsym.Format("%s", m_fields[i]->rsym);
			if(strsym.Find("022") >= 0)
			{
				CString sval;
				if(m_mapCodeToName.Lookup(key, sval))
					fval = sval;

				val->SetAt( m_fields[i]->rsym, fval );
			}
			else
				val->SetAt( m_fields[i]->rsym, fval );
		
		}
		
		RefreshWnd(key, val);
	}
}




void CMapWnd::ParseAlert( LPCSTR dat, int len )
{
	int pack_cnt{}, n{};
	CString key, rsym, rval, rval2;
	vector<StringProxy> lines;
	CMapStringToString* val{};
	BOOL bUpdate{};

	//OutputDebugString(dat);
	CString strData(dat,len);
	
	LPCSTR st = dat;
	LPCSTR ed = st + len;
	LPCSTR pos;

	// Key check
	pos = find(st, ed, '\t');
	rsym = CString(st, pos-st);

	rsym.TrimLeft();
	rsym.TrimRight();

	if(rsym == "S0000")
	{
		parsingNews(CString(pos,ed-pos));
		return;
	}

	if (!m_rtsmap.Lookup(rsym, key)) return;
	if (!m_datas.Lookup(key, val)) return;
	
	st = ++pos;
	
	// rts packet count
	while(st<ed)
	{
		pos = find(st, ed, '\n');
		lines.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}

	// rts proc
	pack_cnt = (int)lines.size();
	for(n=0; n<pack_cnt; ++n)
	{
		st = lines[n].first;
		ed = lines[n].second;
		
		bUpdate = FALSE;
		while(st<ed)
		{
			pos = find(st, ed, '\t');
			rsym = StringProxy(st, pos).ToCStr();

			st = pos + 1;
			if (st>ed) break;
			pos = find(st, ed, '\t');
			rval = StringProxy(st, pos).ToCStr();

			if ( val->Lookup(rsym, rval2) )
			{
				if (rval2!=rval)
				{
					val->SetAt(rsym, rval);
					bUpdate = TRUE;
				}
			}

			st = pos + 1;
		}
		if (bUpdate) RefreshWnd(key, val);
	}
}


void CMapWnd::ParseAlertx(struct _alertR* alertR)
{

	DWORD* data{};
	BOOL bUpdate = FALSE;
	CString rsym = alertR->code;

	const int fields[] = { 22,23,24,33,27 };

	if (rsym == "S0000") {
		parsingNewsx(alertR);
		return;
	}

	CMapStringToString* val{};
	CString rval, rval2, key, code;
	if (!m_rtsmap.Lookup(rsym, code)) return;
	if (!m_datas.Lookup(code, val)) return;


	for (int i = alertR->size - 1; i > -1; --i)
	{
		data = (DWORD*)alertR->ptr[i];

		bUpdate = FALSE;
		for (int j = 0; j < 5; j++) {
			const int idx = fields[j];
			if (data[idx]) {
				rsym.Format("%03d", idx);
				rval = (char*)data[idx];
				if (val->Lookup(rsym, rval2))
				{
					if (rval2 != rval)
					{
						val->SetAt(rsym, rval);
						bUpdate = TRUE;
					}
				}
			}
		}
	}

	if (bUpdate) RefreshWnd(code, val);
}

void CMapWnd::parsingNews(CString datB)
{
	datB.TrimLeft();
	//OutputDebugString(datB);
	int	idx = 0;
	CString	code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T(""), szKey = _T("");
	CString s;
	while (!datB.IsEmpty())
	{
		idx = datB.Find('\t');
		if (idx == -1) break;

		symbol = datB.Left(idx++);
		datB = datB.Mid(idx);

		idx = datB.Find('\t');
		if (idx == -1)
		{
			entry = datB;	datB = _T("");
		}
		else
		{
			entry = datB.Left(idx++); datB = datB.Mid(idx);
		}

		// 		s.Format("SYMBOL : %s	ENTRY : %s\n",symbol,entry);
		// 		OutputDebugString(s);

		if (symbol.Compare("301") == 0)
		{
			code = entry;
			code.TrimRight();
		}
		else if (symbol.Compare("015") == 0)
		{
			szTitle = entry;
			szTitle.TrimRight();
		}
		else if (symbol.Compare("016") == 0)
		{
			szKey = entry;
			szKey.TrimRight();
		}
	}

	CSisePopup* pwnd{};
	CString strValue;

	strValue.Format("015\t%s\n016\t%s\n1301\t%s", szTitle, szKey, code);

	if (m_bCodeNews)
	{
		if (!code.IsEmpty())
		{
			if (m_wnds.Lookup(code, pwnd))
			{
				pwnd->UpdateNews(szTitle, strValue);
			}
		}
	}

	if (m_wnds.Lookup("S0000", pwnd))
	{
		pwnd->UpdateNews(szTitle, strValue);
	}
}


void CMapWnd::parsingNewsx(struct _alertR* alertR)
{

	DWORD* data{};
	CString rsym = alertR->code;
	CString	code = _T(""), szTitle = _T(""), symbol = _T(""), entry = _T(""), szKey = _T("");
	CString s;

	const int fields[] = { 301,15,16 };

	data = (DWORD*)alertR->ptr[0];


	if (data[301]) {
		code = (char *)data[301];
		code.TrimRight();
	} 

	if (data[15]) {
		szTitle = (char*)data[15];
		szTitle.TrimRight();
	}

	if (data[16]) {
		szKey = (char*)data[16];
		szKey.TrimRight();
	}

	CSisePopup* pwnd{};
	CString strValue;

	strValue.Format("015\t%s\n016\t%s\n1301\t%s", szTitle, szKey, code);

	if (m_bCodeNews)
	{
		if (!code.IsEmpty())
		{
			if (m_wnds.Lookup(code, pwnd))
			{
				// 			s.Format("NEWS TITLE : %s	KEY : %s\n",szTitle,szKey);
				// 			OutputDebugString(s);
				pwnd->UpdateNews(szTitle, strValue);
			}
		}
	}

	if (m_wnds.Lookup("S0000", pwnd))
	{
		pwnd->UpdateNews(szTitle, strValue);
	}

}

void CMapWnd::AddCode( LPCSTR code, LPRECT pRC )
{
	//if (strlen(code)!=6) return;

	const int size = m_codes.GetSize();
	for(int n=0; n<size; ++n)
	{
		if (m_codes[n]==code) 
			return;
	}

	m_codes.Add(code);
	CreatePopup(code, pRC);
}

void CMapWnd::DelCode( LPCSTR code )
{
	//if (strlen(code)!=6) return;

	const int size = m_codes.GetSize();
	for(int n=0; n<size; ++n)
	{
		if (m_codes[n]==code) 
		{
			m_codes.RemoveAt(n);
			return;
		}
	}
}

void CMapWnd::RequestCode(CString code)
{
	//char buff[2048]{};

	std::unique_ptr<char[]> buff_data = std::make_unique<char[]>(2048);
	char* buff = buff_data.get();

	CArray<CString,LPCSTR> tmp;
	int n{}, key = TR_STARTKEY;
	
	if(code == "")
	{
		tmp.Append(m_codes);
	}
	else
	{
		tmp.Add(m_codeHist);
	}

	while(tmp.GetSize())
	{
		int pos = 0;
		const int nrec = min(tmp.GetSize(), MAX_REQCOUNT);
		
		pos += sprintf(&buff[pos], "%s\x7f%s\x09%s\x7f", "90991", "1", "$90303");
		pos += sprintf(&buff[pos], "%02d%04d%c%c%c%16s%c%04d%80s", nrec, nrec, '1', '1', '0', "", ' ', 1, "");
		for(n=0; n<m_fields.GetSize(); ++n)
			pos += sprintf(&buff[pos], "%s\x0A", m_fields[n]->symb);
		pos += sprintf(&buff[pos], "1301\x7f");
		for(n=0; n<nrec; ++n)
		{
			pos += sprintf(&buff[pos], "%s\x7f", (LPCTSTR)tmp[0]);
			tmp.RemoveAt(0);
		}
		pos += sprintf(&buff[pos], "\x0a\x09");
		SendTR("pooppoop", US_OOP, buff, pos, key++);

		CString s;
		s.Format("IB0000X8 : [%d][%s]\n",key, buff);
//		OutputDebugString(s);
	}
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadConfig();

	SetFont(m_pFont);

	{
		m_sbTransparent.Create(WS_CHILD|WS_VISIBLE, CRect(10, 10, 210, 30), this, IDC_SB_TRANSPARENT);
		m_sbTransparent.SetRange(0, 100, FALSE);
		m_sbTransparent.SetPos(m_transparent);
	}
	{
		m_btnFlash.Create("깜박임", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 35, 105, 55), this, IDC_BTN_FLASH);
		m_btnFlash.SetFont(m_pFont);
		m_btnFlash.SetCheck(m_bFlash);
	}
	{
		m_btnLastView.Create("마지막조회 실행", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 35, 105, 55), this, IDC_BTN_LASTVIEW);
		m_btnLastView.SetFont(m_pFont);
		m_btnLastView.SetCheck(m_bLastView);
	}

	{
		m_btnCodeNews.Create("종목뉴스", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 35, 105, 55), this, IDC_BTN_CODENEWS);
		m_btnCodeNews.SetFont(m_pFont);
		m_btnCodeNews.SetCheck(m_bCodeNews);
	}
	{
		m_btnSimpleColor.Create("색상동기화", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(105, 35, 210, 55), this, IDC_BTN_SIMPLECOLOR);
		m_btnSimpleColor.SetFont(m_pFont);
		m_btnSimpleColor.SetCheck(m_bSimple);
	}
	{
		m_btnViewType.Create("세로보기", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, 60, 105, 80), this, IDC_BTN_VIEWTYPE);
		m_btnViewType.SetFont(m_pFont);
		m_btnViewType.SetCheck((m_vtype==VT_HORIZONTAL) ? FALSE : TRUE);
	}
	{
		m_btnTopMost.Create("항상맨위에", WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(105, 60, 210, 80), this, IDC_BTN_TOPMOST);
		m_btnTopMost.SetFont(m_pFont);
		m_btnTopMost.SetCheck(m_bTopMost);
	}
	{
		m_edCode.Create(WS_CHILD|WS_VISIBLE|WS_BORDER, CRect(10, 85, 160, 105), this, IDC_EDIT_CODE);
		m_edCode.SetFont(m_pFontB);
		m_btnAdd.Create("추가", WS_CHILD|WS_VISIBLE, CRect(165, 85, 210, 105), this, IDC_BTN_CODEADD);
		m_btnAdd.SetFont(m_pFont);
	}
// 	{
// 		m_cbInterest.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SIMPLE, CRect(10, 110, 160, 300), this, IDC_CB_INTEREST);
// 		m_cbInterest.SetFont(m_pFont);
// 		for(int n=0; n<m_gpnm.GetSize(); ++n)
// 			m_cbInterest.AddString(m_gpnm[n]);
// 		m_btnInterAdd.Create("추가", WS_CHILD|WS_VISIBLE, CRect(165, 110, 210, 130), this, IDC_BTN_INTERADD);
// 		m_btnInterAdd.SetFont(m_pFont);
// 	}

	{
		m_btnBkColor.Create("배경색상", WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(10, 135, 110, 155), this, IDC_BTN_BKCOLOR);
		m_btnBkColor.SetFont(m_pFont);
		m_btnBkColor.SetColor(m_crPopupBack);

		m_btnFlashColor.Create("깜박임색상", WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(120, 135, 210, 155), this, IDC_BTN_FLASHCOLOR);
		m_btnFlashColor.SetFont(m_pFont);
		m_btnFlashColor.SetColor(m_crPopupFlash);
		
	}
	{
		int n{}, ypos = 160;
		for(n=0; FIELD_INFO[n].name!=NULL; ++n)
		{
			m_btnFields[n].Create(FIELD_INFO[n].name, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, CRect(10, ypos, 210, ypos+20), this, IDC_BTN_FIELD_START+n);
			m_btnFields[n].SetFont(m_pFont);
			if (FIELD_INFO[n].must) 
			{
				m_btnFields[n].SetCheck(TRUE);
				m_btnFields[n].EnableWindow(FALSE);
			}
			else
			{
				m_btnFields[n].SetCheck(FALSE);
			}
			ypos += 20;
		}
		for(n=0; n<m_fields.GetSize(); ++n)
		{
			const int idx = GetFieldIndex(m_fields[n]->rsym);
			if (idx>=0)
				m_btnFields[n].SetCheck(TRUE);
		}
	}

	SetTimer(100, 50, NULL);
	
//	TestSetting();
	RequestCode();

// 	Variant(titleCC,"MINI_WIZ");
// 
// 	SetWindowText("MINI_WIZ");

// 	CString s,t;
// 
// 	GetParent()->GetParent()->GetWindowText(t);
// 	s.Format("TITLE : %s\n",t);
// 	OutputDebugString(s);

	return 0;
}

void CMapWnd::RefreshWnd( LPCSTR key, CMapStringToString *pd )
{
	CSisePopup *pwnd;
	if (m_wnds.Lookup(key, pwnd))
		pwnd->UpdateData(pd);

	CString strKey;
	strKey.Format("H%s",key);

	if(m_wnds.Lookup(strKey,pwnd))
	{
		pwnd->UpdateData(pd);
	}
}

CSisePopup* CMapWnd::CreatePopup( LPCSTR code, LPRECT pRC )
{
	BOOL bRet{};
	CRect rc;
	CSisePopup *pwnd;


	if (m_wnds.Lookup(code, pwnd)) 
		return pwnd;

	auto& item = _vWnds.emplace_back(std::make_unique<CSisePopup>(m_pParent));
	pwnd = item.get();

	if (pRC==NULL)
	{
		const int nWidth  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		const int nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);

		if(code == "S0000")
		{
			rc.SetRect(nWidth - 400, 100, nWidth, 120); 
			//m_newY += POPUP_HEIGHT;
		}
		else
		{
			switch(m_vtype)
			{
			case VT_HORIZONTAL:
				rc.SetRect(m_newX, m_newY, m_newX+GetPopupWidth(), m_newY+GetPopupHeight()); 
				m_newY += POPUP_HEIGHT;
				if (m_newY>nHeight)
				{
					m_newY = DEF_POPUP_POSY;
					m_newX += GetPopupWidth();
				}
				break;
			case VT_VERTICAL:
			default:
				rc.SetRect(m_newX, m_newY, m_newX+GetPopupWidth(), m_newY + GetPopupHeight());
				m_newX += MAX_FIELD_WIDTH;
				if (m_newX>nWidth)
				{
					m_newX = DEF_POPUP_POSX;
					m_newY += GetPopupHeight();
				}
				break;
			}
		}

		//bRet = pwnd->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_CLIPCHILDREN|WS_VISIBLE, rc, 0, NULL);
		bRet = pwnd->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_CLIPCHILDREN, rc, 0, NULL);
		//bRet = pwnd->CreateEx(WS_EX_TOOLWINDOW, NULL, "", WS_CHILD, rc, GetDesktopWindow(), (int)pwnd);
	}
	else
	{
		pRC->right = pRC->left + GetPopupWidth();
		pRC->bottom = pRC->top + GetPopupHeight();
		//bRet = pwnd->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_CLIPCHILDREN|WS_VISIBLE, *pRC, 0, NULL);

		bRet = pwnd->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_CLIPCHILDREN, *pRC, 0, NULL);
		//bRet = pwnd->CreateEx(WS_EX_TOOLWINDOW, NULL, "", WS_CHILD, *pRC, GetDesktopWindow(), (int)pwnd);
		m_newX = pRC->left;
		m_newY = pRC->bottom;
	}

//	OutputDebugString("POPUP CREATED\n");

	if (!bRet)
	{
		delete pwnd;
		return NULL;
	}

	if (m_bTopMost)	pwnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	else			pwnd->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);

//	OutputDebugString("POPUP STYLE\n");
	
	pwnd->SetTransparent(m_transparent);
	pwnd->SetCode(code);
	pwnd->SetFields(&m_fields, m_vtype);
	pwnd->SetSearchData(&m_search);
	pwnd->SetOwner(this);
	pwnd->SetFlash(FALSE);
	pwnd->SetBkColor(m_crPopupBack);
	pwnd->SetFlashColor(m_crPopupFlash);

	pwnd->ShowWindow(SW_SHOWNOACTIVATE);

//	OutputDebugString("END POPUP STYLE\n");
// 
// 	OutputDebugString("POPUP PROPERTY\n");
// 
// 	pwnd->Invalidate(FALSE);
// 
// 	OutputDebugString("POPUP INVALIDATE\n");

//	CString strCode(code);

// 	if(strCode.GetAt(0) == 'H')
// 	{
// 		pwnd->SetFlash(FALSE);
// 	}
// 	else
// 	{
// 		pwnd->SetFlash(m_bFlash);
// 	}
	//pwnd->SetBkColor(m_crPopupBack);
	//pwnd->SetFlashColor(m_crPopupFlash);

	m_wnds.SetAt(code, pwnd);

	

//	OutputDebugString("END CREATEPOPUP\n");

	return pwnd;
}

CSisePopup* CMapWnd::CreateHistPopup( LPCSTR code, LPRECT pRC )
{
//	OutputDebugString("CreateHistPopup\n");
	BOOL bRet{};
	CRect rc;
//	CSisePopup *pwnd;
	
	if(m_pHistroy != NULL)
	{
		m_pHistroy->DestroyWindow();
		m_pHistroy = nullptr;
	}
		
	m_pHistroy = std::make_unique<CSisePopup>(m_pParent); 

	if (pRC==NULL)
	{
		const int nWidth  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		const int nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		
		if(code == "S0000")
		{
			rc.SetRect(nWidth - 400, 100, nWidth, 120); 
			//m_newY += POPUP_HEIGHT;
		}
		else
		{
			switch(m_vtype)
			{
			case VT_HORIZONTAL:
				rc.SetRect(m_newX, m_newY, m_newX+GetPopupWidth(), m_newY+GetPopupHeight()); 
				m_newY += POPUP_HEIGHT;
				if (m_newY>nHeight)
				{
					m_newY = DEF_POPUP_POSY;
					m_newX += GetPopupWidth();
				}
				break;
			case VT_VERTICAL:
			default:
				rc.SetRect(m_newX, m_newY, m_newX+GetPopupWidth(), m_newY + GetPopupHeight());
				m_newX += MAX_FIELD_WIDTH;
				if (m_newX>nWidth)
				{
					m_newX = DEF_POPUP_POSX;
					m_newY += GetPopupHeight();
				}
				break;
			}
		}
		
		bRet = m_pHistroy->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN, rc, 0, NULL);
	}
	else
	{
		pRC->right = pRC->left + GetPopupWidth();
		pRC->bottom = pRC->top + GetPopupHeight();
		bRet = m_pHistroy->CreateEx(WS_EX_LAYERED|WS_EX_TOOLWINDOW, ::AfxRegisterWndClass(0), "", WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN, *pRC, 0, NULL);
		m_newX = pRC->left;
		m_newY = pRC->bottom;
	}
	
	if (!bRet)
	{
		//delete m_pHistroy;CMapStringToString
		return NULL;
	}
	
	if (m_bTopMost)	m_pHistroy->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	else			m_pHistroy->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	m_pHistroy->SetTransparent(m_transparent);
	m_pHistroy->SetCode(code);
	m_pHistroy->SetFields(&m_fields, m_vtype);
	m_pHistroy->SetSearchData(&m_search);
	m_pHistroy->SetOwner(this);
	m_pHistroy->SetFlash(m_bFlash);
	m_pHistroy->SetBkColor(m_crPopupBack);
	m_pHistroy->SetFlashColor(m_crPopupFlash);
	
	return m_pHistroy.get();
}

void CMapWnd::OnDestroy() 
{
	m_pOldHoverWnd = NULL;
	m_bExit = TRUE;
	KillTimer(0);

	{
		MSG msg;
		while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	SaveConfig();

	POSITION pos{};
	 
	_vWnds.clear();
	_vData.clear();
	_vSearch.clear();

//	OutputDebugString("MINI WID DESTROY\n");

// 	if(m_mainWnd->GetSafeHwnd())
// 		m_mainWnd->SendMessage(WD_MINICLOSE,(WPARAM)0,(LPARAM)0);

	CWnd::OnDestroy();
}

struct FIELD_INFO * CMapWnd::GetFieldInfo( LPCSTR rsym )
{
	for(int n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (strcmp(FIELD_INFO[n].rsym, rsym)==0) return &FIELD_INFO[n];
	}
	return NULL;
}

int CMapWnd::GetFieldIndex( LPCSTR rsym )
{
	for(int n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (strcmp(FIELD_INFO[n].rsym, rsym)==0) return n;
	}
	return -1;
}

DWORD CMapWnd::GetPopupWidth()
{
	if (m_vtype == VT_HORIZONTAL)
	{
		DWORD size = 0;
		const DWORD fcnt = m_fields.GetSize();
		for(DWORD n=0; n<fcnt; ++n)
			size += m_fields[n]->width;
		return size;
	}
	else if (m_vtype == VT_VERTICAL)
	{
		return MAX_FIELD_WIDTH;
	}
	else 
	{
		return 0;
	}
}


DWORD CMapWnd::GetPopupHeight()
{
	if (m_vtype == VT_HORIZONTAL)
	{
		return POPUP_HEIGHT;
	}
	else if (m_vtype == VT_VERTICAL)
	{
		DWORD size = 0;
		const DWORD fcnt = m_fields.GetSize();
		for(DWORD n=0; n<fcnt; ++n)
			size += m_fields[n]->width ? POPUP_HEIGHT : 0;
		return size;
	}
	else 
	{
		return 0;
	}
}

CSisePopup* CMapWnd::FindPopup( LPCSTR code )
{
	CSisePopup *pwnd;
	if (!m_wnds.Lookup(code, pwnd)) pwnd = NULL;
	return pwnd;
}

void CMapWnd::ChangeTransparent( int val )
{
	if (val<0) return;
	if (val>100) return;
	if (m_transparent != val)	
	{
		m_transparent = val;

		CString key;
		CSisePopup* wnd{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos!=NULL)
		{
			m_wnds.GetNextAssoc(pos, key, wnd);
			if (wnd) CLayer::SetTransparent(wnd, val);
		}
	}
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 100)
	{
		if (m_bExit) return;

		CRect rc;
		CString key;
		CSisePopup* pwnd{};
		const DWORD tick = GetTickCount();
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, pwnd);
			if (pwnd != NULL)
				pwnd->UpdateFlash(tick);
		}

		if (m_bTracking && !GetAsyncKeyState(VK_LBUTTON) && !GetAsyncKeyState(VK_RBUTTON))
		{
			CPoint pt;
			GetCursorPos(&pt);
			pos = m_wnds.GetStartPosition();
			while(pos)
			{
				m_wnds.GetNextAssoc(pos, key, pwnd);
				if (pwnd != NULL) {
					pwnd->GetWindowRect(&rc);
					if (rc.PtInRect(pt))
					{
						if (pwnd != m_pOldHoverWnd)
						{
							if (m_pOldHoverWnd) { m_pOldHoverWnd->LeaveWindow(); }
							if (pwnd) { pwnd->HoverWindow(); m_pOldHoverWnd = pwnd; }
						}
						return;
					}
				}
			}

			
			if (m_pOldHoverWnd) 
			{
				m_pOldHoverWnd->LeaveWindow();
				m_pOldHoverWnd = NULL;
			}
		}
	}
	else if(nIDEvent == 200)
	{
		KillTimer(200);

		CSisePopup* pwnd = CreatePopup("S0000", NULL);
		
		pwnd->SetViewType(NEWS_MODE);
	}

	CWnd::OnTimer(nIDEvent);
}

LRESULT CMapWnd::OnChildClose( WPARAM wParam, LPARAM lParam )
{
	CSisePopup *pwnd = (CSisePopup*)lParam;
	if (pwnd)
	{
		if (m_pOldHoverWnd==pwnd) 
			m_pOldHoverWnd = NULL;
		DeletePopup(pwnd);
	}
	else
	{
		CString key;
		CMapStringToString* val2{};
		POSITION pos = m_wnds.GetStartPosition();
		
		while(pos!=NULL)
		{
			m_wnds.GetNextAssoc(pos, key, pwnd);
			pwnd->DestroyWindow();
			delete pwnd;
		}

		m_codes.RemoveAll();
		m_wnds.RemoveAll();
		
		pos = m_datas.GetStartPosition();
		while(pos!=NULL)
		{
			m_datas.GetNextAssoc(pos, key, val2);
			delete val2;
		}
		m_datas.RemoveAll();
		m_pOldHoverWnd = NULL;
	}
	return 0;
}

void CMapWnd::DeletePopup( CSisePopup *pwnd )
{
	CString key;
	CSisePopup* val{};
	CMapStringToString* val2{};
	POSITION pos;

	pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, val);
		if (val==pwnd)
		{
//			OutputDebugString("DELETEPOPUP1\n");
			m_wnds.RemoveKey(key);

			pwnd->DestroyWindow();
			
			// start point 처리 
			// 삭제시 오류 발생함
			// delete pwnd;

//			OutputDebugString("DELETEPOPUP2\n");
			DelCode(key);
//			OutputDebugString("DELETEPOPUP3\n");
			if (m_datas.Lookup(key, val2))
			{
				// start point 처리 
				// 삭제시 오류 발생함
				// delete val2;
//				OutputDebugString("DELETEPOPUP3\n");
				m_datas.RemoveKey(key);
//				OutputDebugString("DELETEPOPUP4\n");
			}
			break;
		}
	}
}

LRESULT CMapWnd::OnSetTracking( WPARAM wParam, LPARAM lParam )
{
	m_bTracking = (BOOL)lParam;
	return 0;
}

void CMapWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->GetDlgCtrlID()==IDC_SB_TRANSPARENT)
	{
		m_transparent = m_sbTransparent.GetPos();
		CString key;
		CSisePopup* pwnd{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos!=NULL)
		{
			m_wnds.GetNextAssoc(pos, key, pwnd);
			pwnd->SetTransparent(m_transparent);
		}
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMapWnd::OnFlash()
{	
	m_bFlash = m_btnFlash.GetCheck()!=0;

	CString key;
	CSisePopup* pwnd{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->SetFlash(m_bFlash);
	}
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc, m_crBodyBk);
}

void CMapWnd::OnCodeAdd()
{
	CString code;
	m_edCode.GetWindowText(code);
	if (code.GetLength()>=3 && code.GetLength()<=8)
	{
		AddCode(code);
		RequestCode();
	}
	else
	{
		MessageBox("지원하지 않는 종목입니다. (3~8자리 입력)", "IBK투자증권", MB_OK);
	}
}

void CMapWnd::OnFieldChange(UINT nID)
{
	int nSelFields = 0;
	CArray<struct FIELD_INFO*, struct FIELD_INFO*> t_fields;
	for(int n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (m_btnFields[n].GetSafeHwnd())
		{
			if (m_btnFields[n].GetCheck())
			{
				t_fields.Add(&FIELD_INFO[n]);
				if (!FIELD_INFO[n].must) ++nSelFields;
			}
		}
	}
	if (nSelFields==0)
	{
		m_btnFields[nID-IDC_BTN_FIELD_START].SetCheck(TRUE);
		MessageBox("최소 한항목 이상을 선택해야합니다.", "IBK투자증권");
		return;
	}
	m_fields.Copy(t_fields);

	RequestCode();
	CRect rc;
	CString key;
	CSisePopup* pwnd{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		if(pwnd->m_nType == NEWS_MODE)
		{
			continue;
		}
		pwnd->SetFields( &m_fields, m_vtype );
		pwnd->GetWindowRect(&rc);
		rc.right = rc.left + GetPopupWidth();
		rc.bottom = rc.top + GetPopupHeight();
		pwnd->MoveWindow(&rc, TRUE);
	}
}

LRESULT CMapWnd::OnAlignHUpper( WPARAM wParam, LPARAM lParam )
{
	AlignCustom( m_fields[wParam], (CSisePopup *)lParam, true, VT_HORIZONTAL);
	return 0;
}

LRESULT CMapWnd::OnAlignHLower( WPARAM wParam, LPARAM lParam )
{
	AlignCustom( m_fields[wParam], (CSisePopup *)lParam, false, VT_HORIZONTAL);
	return 0;
}

LRESULT CMapWnd::OnAlignVUpper( WPARAM wParam, LPARAM lParam )
{
	AlignCustom( m_fields[wParam], (CSisePopup *)lParam, true, VT_VERTICAL);
	return 0;
}

LRESULT CMapWnd::OnAlignVLower( WPARAM wParam, LPARAM lParam )
{
	AlignCustom( m_fields[wParam], (CSisePopup *)lParam, false, VT_VERTICAL);
	return 0;
}

void CMapWnd::AlignCustom( struct FIELD_INFO *fi, CSisePopup *pbase, bool bUpper, VIEW_TYPE vtype )
{
	if (!fi || !pbase) return;

	if (!fi->sort) return;

	vector<SORT_OBJECT> vSort;
	
	if ( strcmp(fi->rsym, "022") == 0 )	// 종목명 정렬
	{
		CString key;
		CMapStringToString* val{};
		POSITION pos = m_datas.GetStartPosition();
		while(pos!=NULL)
		{
			m_datas.GetNextAssoc(pos, key, val);
			vSort.emplace_back( SORT_OBJECT(key, (*val)[fi->rsym]) );
		}
		
		sort(vSort.begin(), vSort.end(), struct stringCompare(bUpper));	
	}
	else									// 숫자형
	{
		CString key;
		CMapStringToString* val{};
		POSITION pos = m_datas.GetStartPosition();
		while(pos!=NULL)
		{
			m_datas.GetNextAssoc(pos, key, val);
			vSort.push_back( SORT_OBJECT(key, (*val)[fi->rsym]) );
		}
		
		if ( strcmp(fi->rsym, "033")==0 )		// 등락률은 -가 있어!!
			sort(vSort.begin(), vSort.end(), struct priceCompare(bUpper, false));
		else
			sort(vSort.begin(), vSort.end(), struct priceCompare(bUpper, true));
	}
	
	CRect rc;
	pbase->GetWindowRect(rc);
	if (vSort.size() == m_datas.GetCount())
	{
		CSisePopup* pwnd{};

		const int nWidth  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		const int nHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		const int nBaseX = rc.left;
		const int nBaseY = rc.top;
		const int cx = rc.Width();
		const int cy = rc.Height();

		for(size_t n=0; n<vSort.size(); ++n)
		{
			if (m_wnds.Lookup(vSort[n].first, pwnd))
			{
				pwnd->MoveWindow(&rc, TRUE);
				if (vtype==VT_HORIZONTAL)
				{
					rc.OffsetRect(cx, 0);
					if (rc.right>nWidth)
					{
						rc.OffsetRect(0, cy);
						rc.left = nBaseX;
						rc.right = nBaseX + cx;
					}
						
				}
				else 
				{
					rc.OffsetRect(0, cy);
					if (rc.bottom>nHeight)
					{
						rc.OffsetRect(cx, 0);
						rc.top = nBaseY;
						rc.bottom = nBaseY + cy;
					}
				}
			}
		}
	}
}

void CMapWnd::SaveConfig()
{
	LPCSTR sct = "MINIPOPUP";
	CString path, val;
	path.Format("%s\\user\\%s\\userconf.ini", Variant(homeCC,""), Variant(nameCC,""));

	int n = 0;
	CRect rc;
	CString key;
	CSisePopup* pwnd{};
	POSITION pos{};
	
	val = "";
	for(n=0; n<m_fields.GetSize(); ++n)
	{
		if (m_fields[n]->must) continue;
		val += CString(m_fields[n]->rsym) + ",";
	}
	if (val.GetLength()>0) val.Delete(val.GetLength()-1);	// "마지막 콤마 없애기
	WritePrivateProfileString(sct, "FIELDS", val, path);


	val.Format("%d", m_wnds.GetCount());
	WritePrivateProfileString(sct, "SCOUNT", val, path);
	
	n = 0;
	pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->GetWindowRect(&rc);
		val.Format("%s,%d,%d,%d,%d", key, rc.left, rc.top, rc.right, rc.bottom);
		key.Format("S%03d", ++n);
		WritePrivateProfileString(sct, key, val, path);
	}

	val.Format("%d", m_transparent);
	WritePrivateProfileString(sct, "TRANSPARENT", val, path);
	val.Format("%d", m_bFlash);
	WritePrivateProfileString(sct, "FLASH", val, path);
 	val.Format("%d", m_bSimple);
 	WritePrivateProfileString(sct, "SIMPLE_COLOR", val, path);
	val.Format("%d", m_crPopupBack);
	WritePrivateProfileString(sct, "POP_BK_COLOR", val, path);
	val.Format("%d", m_crPopupFlash);
	WritePrivateProfileString(sct, "POP_FL_COLOR", val, path);
	val.Format("%d", (int)m_vtype);
	WritePrivateProfileString(sct, "VIEW_TYPE", val, path);
	val.Format("%d", m_bTopMost);
 	WritePrivateProfileString(sct, "TOPMOST", val, path);
	val.Format("%d", m_bLastView);
	WritePrivateProfileString(sct, "LASTVIEW", val, path);
	val.Format("%d", m_bCodeNews);
	WritePrivateProfileString(sct, "CODENEWS", val, path);
}

void CMapWnd::LoadConfig()
{
	int n{}, m{};
	char buff[1024]{};
	CRect rc{};
	LPCSTR sct = "MINIPOPUP";
	CString path, key, val;
	path.Format("%s\\user\\%s\\userconf.ini", Variant(homeCC,""), Variant(nameCC,""));

	m_fields.RemoveAll();
	m_codes.RemoveAll();

	// 기본필드 세팅
	for(n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (FIELD_INFO[n].must)	
			m_fields.Add(&FIELD_INFO[n]);
	}

	vector<StringProxy> fields;
	GetPrivateProfileString(sct, "FIELDS", "022,023,024,033,027", buff, sizeof(buff), path);
	ParseString(buff, buff+strlen(buff), ',', fields);
	for(size_t n=0; n<fields.size(); ++n)
	{
		for(m=0; FIELD_INFO[m].name!=NULL; ++m)
		{
			if (fields[n].ToCStr()==FIELD_INFO[m].rsym && !FIELD_INFO[m].must)
			{
				m_fields.Add(&FIELD_INFO[m]);
				break;
			}
		}
	}

	m_transparent = GetPrivateProfileInt(sct, "TRANSPARENT", 100, path);
	m_bFlash = GetPrivateProfileInt(sct, "FLASH", TRUE, path);
	m_bSimple = GetPrivateProfileInt(sct, "SIMPLE_COLOR", TRUE, path);
	m_crPopupBack = GetPrivateProfileInt(sct, "POP_BK_COLOR", COLOR_DEF_POPUP_BACK, path);
	m_crPopupFlash = GetPrivateProfileInt(sct, "POP_FL_COLOR", COLOR_DEF_POPUP_FLASH, path);
	m_vtype = (VIEW_TYPE)GetPrivateProfileInt(sct, "VIEW_TYPE", VT_HORIZONTAL, path);
	m_bTopMost = GetPrivateProfileInt(sct, "TOPMOST", m_bTopMost, path);
	m_bCodeNews = GetPrivateProfileInt(sct, "CODENEWS", 1, path);

	const int scnt = GetPrivateProfileInt(sct, "SCOUNT", 0, path);

	m_bLastView = GetPrivateProfileInt(sct,"LASTVIEW", 0, path);

	if(m_bMiniMode)
	{
		if(m_bLastView)
		{
			for(n=0; n<scnt; ++n)
			{
				key.Format("S%03d", n+1);
				GetPrivateProfileString(sct, key, "", buff, sizeof(buff), path);
				ParseString(buff, buff+strlen(buff), ',', fields);
				if (fields.size()==5)	// 종목코드, left, top, right, bottom
				{
					CString code = fields[0].ToCStr();
					rc.SetRect(atoi(fields[1].ToCStr()),atoi(fields[2].ToCStr()),atoi(fields[3].ToCStr()),atoi(fields[4].ToCStr()));
					AddCode(code, &rc);
				}
			}
		}
	}
}

LRESULT CMapWnd::OnLastPosUpdate( WPARAM wParam, LPARAM lParam )
{
	m_newX = wParam;
	m_newY = lParam;
	switch(m_vtype)
	{
	case VT_VERTICAL:   m_newX += GetPopupWidth(); break;
	case VT_HORIZONTAL: m_newY += GetPopupHeight(); break;
	}
	return 0;
}

void CMapWnd::AddSearchData(const CString &code, const CString &name )
{
	CStringArray* obj1{}, * obj2{};
	CString val, key;
	
	key = name.Left( IsDBCSLeadByte(name[0]) ? 2 : 1 ); 	// 한글이면 2글자, 영문이면 1글자
	if (!m_search.Lookup(key, obj1))
	{
		//obj1 = new CStringArray();
		auto& item = _vSearch.emplace_back(std::make_unique<CStringArray>());
		obj1 = item.get();
		m_search.SetAt(key, obj1);
	}
	val.Format("%s|%s@", name, code);
	obj1->Add(val);
	
	key = code.Left(1);
	if (!m_search.Lookup(key, obj2))
	{
//		obj2 = new CStringArray();
		auto& item = _vSearch.emplace_back(std::make_unique<CStringArray>());
		obj2 = item.get();
		m_search.SetAt(key, obj2);
	}
	val.Format("%s|%s#", code, name);
	obj2->Add(val);
}

void CMapWnd::LoadCodes()
{
	vector<char> buff;
	CString path;
	
	// 업종
	{
		path.Format("%s\\tab\\upcode.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		struct upcode *st = (struct upcode *)&buff[0];
		struct upcode *ed = st + buff.size()/sizeof(struct upcode);
		for(; st<ed; ++st)
		{
			CString code, name;
			code.Format("%d%02d", st->jgub, st->ucod);
			name.Format("%.*s", min(strlen(st->hnam),UNameLen), st->hnam); name.Replace(" ", "");
			AdjustHangul(name);
			TRACE("[%s][%s]\n", code, name);
			AddSearchData(code, name);
		}
	}

	// 현물
	{
		path.Format("%s\\tab\\hjcode3.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		struct hjcodex *st = (struct hjcodex *)&buff[0];
		struct hjcodex *ed = st + buff.size()/sizeof(struct hjcodex);
		for(; st<ed; ++st)
		{
			CString code(&st->code[0], sizeof(st->code));
			CString name(&st->hnam[1], sizeof(st->hnam)-1);
			code.TrimRight();
			name.TrimRight();
			AdjustHangul(name);
			if (strlen(code)==7)
			{
				if (code[0]=='A' || code[0]=='J')
					code = code.Mid(1, 6);
			}
			AddSearchData(code, name);
		}
	}

	// 선물
	{
		path.Format("%s\\tab\\fjcode.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		struct fjcode *st = (struct fjcode *)&buff[0];
		struct fjcode *ed = st + buff.size()/sizeof(struct fjcode);
		for(; st<ed; ++st)
		{
			CString code(st->cod2, sizeof(st->cod2));
			CString name = st->hnam;
			AdjustHangul(name);
			AddSearchData(code, name);
		}
	}

	// 옵션
	{
		path.Format("%s\\tab\\opcode.dat", Variant(homeCC, ""));
		ReadFile( path, buff );
		buff.erase(buff.begin(), buff.begin()+sizeof(struct ojcodh));
		struct ojcode *st = (struct ojcode *)&buff[0];
		struct ojcode *ed = st + buff.size()/sizeof(struct ojcode);
		for(; st<ed; ++st)
		{
			for(int n=0; n<11; ++n)
			{
				if (st->call[n].yorn=='1')
				{
					CString code(st->call[n].cod2, OCodeLen);
					CString name(st->call[n].hnam);
					AdjustHangul(name);
					AddSearchData(code, name);
				}
				if (st->put[n].yorn=='1')
				{
					CString code(st->put[n].cod2, OCodeLen);
					CString name(st->put[n].hnam, ONameLen); name.TrimRight();
					AdjustHangul(name);
					AddSearchData(code, name);
				}
			}
		}
	}
}

void CMapWnd::ReadFile( LPCSTR path, vector<char> &dst )
{
	dst.clear();

	FILE *fp = fopen(path, "rb");
	if (!fp) return;

	struct stat st;
	fstat(fileno(fp), &st);
	
	dst.resize(st.st_size);
	long tlen = 0;
	while(tlen<st.st_size)
		tlen += fread(&dst[tlen], 1, st.st_size-tlen, fp);
	fclose(fp);
}

LRESULT CMapWnd::OnCodeChange( WPARAM wParam, LPARAM lParam )
{
	LPCSTR code = (LPCSTR)wParam;
	CSisePopup *dstWnd = (CSisePopup*)lParam;

	CString key;
	CSisePopup *val;

	if (m_wnds.Lookup(code, val))
	{
		if (val==dstWnd)	// 같은종목으로 변경할 필요는 없지!
			return 0;
		else				// 이미 다른창이 해당종목을 사용하고 있어!
			return -1;
	}
	else
	{
		LPCSTR oldCode = dstWnd->GetCode();
		if (m_wnds.Lookup(oldCode, val))
		{
			m_wnds.RemoveKey(oldCode);
			DelCode(oldCode);

			m_codes.Add(code);
			m_wnds.SetAt(code, dstWnd);
			dstWnd->SetCode(code);

			RequestCode();
			return 0;
		}
		else
		{
			// 넌 어디서온놈이냐~
			return -1;
		}
	}
}

LRESULT CMapWnd::OnAddNewCode( WPARAM wParam, LPARAM lParam )
{
	LPCSTR xCode = (LPCSTR)lParam;
	CString s;
	s.Format("ADD CODE : %s\n",xCode);
	//OutputDebugString(s);
// 	AddCode(xCode, NULL);
// 	CSisePopup *pWnd = FindPopup(xCode);
// 	if (pWnd) 
// 	{
// 		pWnd->HoverWindow();
// 		pWnd->EditCode();
// 	}
	CString code = xCode;

	if(code == "TR")
	{
		CString history = Variant(historyCC,"1301");

		int index = history.Find("\t");

		if(index > -1)
		{
			CString str = history.Left(index);

			index = str.Find(" ");

			CString strCode,strName;

			if(index > 0)
			{
				strCode = str.Left(index);
				strName = str.Mid(index + 1);
			}

			code = strCode;
		}
		else
		{
			code = "";
		}
	}

	//if (code.GetLength()>=3 && code.GetLength()<=8)
	if (code.GetLength()>=3 && code.GetLength()<=6)
	{
		POSITION pos;

		pos = m_wnds.GetStartPosition();

		CString key;
		CSisePopup* wnd{};

		int nCount = 0;

		while(pos!=NULL)
		{
			m_wnds.GetNextAssoc(pos, key, wnd);

			if(wnd->m_code.GetAt(0) == 'H')
			{
				continue;
			}

			nCount++;
		}

		if(nCount == 0)
		{
			CRect rc;

			m_mainWnd->GetClientRect(&rc);

			rc.left = 0;
			rc.top = 125;

			AddCode(code,rc);
			RequestCode();
		}
		else
		{
			AddCode(code);
			RequestCode();
		}
	}
	else
	{
		MessageBox("지원하지 않는 종목입니다. (3~8자리 입력)", "IBK투자증권", MB_OK);
	}

	return 0;
}

void CMapWnd::OnViewType()
{
	const BOOL bCheck = m_btnViewType.GetCheck();
	m_vtype = (bCheck) ? VT_VERTICAL : VT_HORIZONTAL;
	
	const int cx = GetPopupWidth();
	const int cy = GetPopupHeight();

	CString key;
	CSisePopup* val{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos)
	{
		m_wnds.GetNextAssoc(pos, key, val);
		if (val)
		{
			val->SetFields(&m_fields, m_vtype);
			val->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER|SWP_NOZORDER);
		}
	}
}

void CMapWnd::OnBkColorChange()
{
	CColorDialog dlg(m_crPopupBack, 0, this);
	if (dlg.DoModal()==IDOK)
	{
		m_crPopupBack = dlg.GetColor();
		m_btnBkColor.SetColor(m_crPopupBack);
		
		CString key;
		CSisePopup* val{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			if (val)
				val->SetBkColor(m_crPopupBack);
		}	
	}
}

void CMapWnd::OnFlashColorChange()
{
	CColorDialog dlg(m_crPopupBack, 0, this);
	if (dlg.DoModal()==IDOK)
	{
		m_crPopupFlash = dlg.GetColor();
		m_btnFlashColor.SetColor(m_crPopupFlash);
		
		CString key;
		CSisePopup* val{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			if (val)
				val->SetFlashColor(m_crPopupFlash);
		}	
	}
}

void CMapWnd::AdjustHangul( CString &text )
{
	BOOL bLead = FALSE;
	const int nSize = text.GetLength();
	for(int n=0; n<nSize; ++n)
	{
		if (IsDBCSLeadByte(text[n]) && !bLead)
		{
			bLead = TRUE;
		}
		else
		{
			bLead = FALSE;
		}
	}
	if (bLead) text.Delete(nSize-1);
}

void CMapWnd::OnSimpleColor()
{
	const BOOL bSimple = m_btnSimpleColor.GetCheck();

	if (m_bSimple!=bSimple)
	{
		CString key;
		CSisePopup* val{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			if (val) 
				val->SetSimpleColor(bSimple);
		}
		m_bSimple = bSimple;
	}
}

void CMapWnd::OnTopMost()
{
	const BOOL bTopMost = m_btnTopMost.GetCheck();
	
	if (m_bTopMost!=bTopMost)
	{
		CString key;
		CSisePopup* val{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			if (bTopMost)
				val->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			else
				val->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}
		m_bTopMost = bTopMost;
	}
}

// void CMapWnd::TestSetting()
// {
// 	AddCode("101F9000");
// 	AddCode("101FC000");
// 	AddCode("101G3000");
// 	AddCode("101G6000");
// 	AddCode("401F9FCS");
// 	AddCode("401F9G3S");
// 	AddCode("401F9G6S");
// 	AddCode("201F7250");
// 	AddCode("201F7252");
// 	AddCode("201F7255");
// 	AddCode("201F7257");
// 	AddCode("201F7260");
// 	AddCode("201F7262");
// 	AddCode("201F7265");
// 	AddCode("201F7267");
// 	AddCode("201F7270");
// 	AddCode("201F7272");
// 	AddCode("201F7275");
// 	AddCode("201F7277");
// 	AddCode("201F7280");
// 	AddCode("201F7282");
// 	AddCode("201F7285");
// 	AddCode("201F7287");
// 	AddCode("201F7290");
// 	AddCode("201F7292");
// 	AddCode("201F7295");
// 	AddCode("201F7297");
// 	AddCode("201F7300");
// 	AddCode("201F7302");
// 	AddCode("201F7305");
// 	AddCode("201F7307");
// 	AddCode("201F7310");
// 	AddCode("301F7250");
// 	AddCode("301F7252");
// 	AddCode("301F7255");
// 	AddCode("301F7257");
// 	AddCode("301F7260");
// 	AddCode("301F7262");
// 	AddCode("301F7265");
// 	AddCode("301F7267");
// 	AddCode("301F7270");
// 	AddCode("301F7272");
// 	AddCode("301F7275");
// 	AddCode("301F7277");
// 	AddCode("301F7280");
// 	AddCode("301F7282");
// 	AddCode("301F7285");
// 	AddCode("301F7287");
// 	AddCode("301F7290");
// 	AddCode("301F7292");
// 	AddCode("301F7295");
// 	AddCode("301F7297");
// 	AddCode("301F7300");
// 	AddCode("301F7302");
// 	AddCode("301F7305");
// 	AddCode("301F7307");
// 	AddCode("301F7310");
// }


void CMapWnd::OnClose() 
{
	m_pOldHoverWnd = NULL;
	m_bExit = TRUE;
	KillTimer(0);
	CWnd::OnClose();
}

void CMapWnd::sendTR(CString trCode, char* datB, int datL, int key)
{
	auto sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH*)sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	int iret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM)sendB.get());
}

void CMapWnd::LoadInterest()
{
	m_gpno.RemoveAll();
	m_gpnm.RemoveAll();

	int	sendL = 0;
	char	sendB[16 * 1024] = { 0, };
	int isz_updn = sz_uinfo;
	struct _updn* updn = (struct _updn*)&sendB[sendL];
	sendL += isz_updn;

	FillMemory((char*)updn, isz_updn, ' ');

	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	updn->uinfo.cont[0] = 'g';
	CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'U';

	sendTR("PIDOMYST", sendB, sendL, TRKEY_GROUPARR);
	/*char buff[1024];
	DWORD dwSize;
	CString path;
	path.Format("%s\\user\\%s\\portfolio.ini", Variant(homeCC,""), Variant(nameCC,""));

	CString keys, gName;
	CStringArray arGroup;
	
	dwSize = GetPrivateProfileString("GROUPORDER", "00", "", buff, sizeof(buff), path);
	if (dwSize==0) return;
	
	gName = CString(buff,dwSize);

	arGroup.RemoveAll();
	while (!gName.IsEmpty())
	{
		int pos = gName.Find(';');
		if (pos == -1)	break;
		keys = gName.Left(pos++);
		gName = gName.Mid(pos);
		arGroup.Add(keys);
	}
	
	CString group;
	for (int ii = 0; ii < arGroup.GetSize(); ii++)
	{
		keys = arGroup.GetAt(ii);

		m_gpno.Add(keys);

		dwSize = GetPrivateProfileString("GROUPNAME", keys, "", buff, sizeof(buff), path);
		if (dwSize==0) continue;

		m_gpnm.Add(buff);
	}*/
}

void CMapWnd::OnInterAdd()
{
	int gpno;
	CString path;
	
	gpno = m_cbInterest.GetCurSel();
	if (gpno<0) return;

	path.Format("%s\\user\\%s\\portfolio.i%s", Variant(homeCC,""), Variant(nameCC,""), m_gpno[gpno]);

	FILE *fp = fopen(path, "rb");
	if (!fp) return;

	struct _inters inters;
	while( fread(&inters, 1, sizeof(inters), fp) == sizeof(inters) )
	{
		CString code(inters.code, sizeof(inters.code));
		code.Remove(0x00); code.Remove(' ');
		if (code.GetLength()>=3 && code.GetLength()<=8 && inters.gubn[0]!='m')
		{
			AddCode(code);
		}
	}
	RequestCode();
	fclose(fp);
}

LRESULT CMapWnd::OnViewProperty(WPARAM wParam, LPARAM lParam)
{
	CString option;
	option.Format("IB0000X9 /t/K/p/d");
	SetView(option);
	return 0;
}

LRESULT CMapWnd::OnWFFlash( WPARAM wParam, LPARAM lParam )
{
// 	CString s;
// 	s.Format("WFFLASH : %d\n",(BOOL)lParam);
// 	OutputDebugString(s);
	m_bFlash = (BOOL)lParam;
	
	CString key;
	CSisePopup* pwnd{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->SetFlash(m_bFlash);
	}

	m_btnFlash.SetCheck(m_bFlash);

	return 0;
}

LRESULT CMapWnd::OnWFViewType( WPARAM wParam, LPARAM lParam )
{
	const BOOL bCheck = (BOOL)lParam;
	m_btnViewType.SetCheck(bCheck);
	m_vtype = ((bCheck) ? VT_VERTICAL : VT_HORIZONTAL);
	
	const int cx = GetPopupWidth();
	const int cy = GetPopupHeight();
	
	CString key;
	CSisePopup* val{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos)
	{
		m_wnds.GetNextAssoc(pos, key, val);
		if (val)
		{
			val->SetFields(&m_fields, m_vtype);
			val->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER|SWP_NOZORDER);
		}
	}

	return 0;
}

LRESULT CMapWnd::OnWFSimpleColor( WPARAM wParam, LPARAM lParam )
{
	const BOOL bSimple = (BOOL)lParam;
	m_btnSimpleColor.SetCheck(bSimple);
	
	if (m_bSimple!=bSimple)
	{
		CString key;
		CSisePopup* val{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			if (val) 
				val->SetSimpleColor(bSimple);
		}
		m_bSimple = bSimple;
	}

	return 0;
}

LRESULT CMapWnd::OnWFTopMost( WPARAM wParam, LPARAM lParam )
{
	const BOOL bTopMost = (BOOL)lParam;
	m_btnTopMost.SetCheck(bTopMost);
	
	if (m_bTopMost!=bTopMost)
	{
		CString key;
		CSisePopup* val{};
		POSITION pos = m_wnds.GetStartPosition();
		while(pos)
		{
			m_wnds.GetNextAssoc(pos, key, val);
			if (bTopMost)
				val->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			else
				val->SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		}
		m_bTopMost = bTopMost;
	}

	return 0;
}

LRESULT CMapWnd::OnWFBkColorChange( WPARAM wParam, LPARAM lParam )
{
	m_crPopupBack = (COLORREF)lParam;
	m_btnBkColor.SetColor(m_crPopupBack);

	CString key;
	CSisePopup* val{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos)
	{
		m_wnds.GetNextAssoc(pos, key, val);
		if (val)
			val->SetBkColor(m_crPopupBack);
	}

	return 0;
}

LRESULT CMapWnd::OnWFFlashColorChange( WPARAM wParam, LPARAM lParam )
{
	m_crPopupFlash = (COLORREF)lParam;

	m_btnFlashColor.SetColor(m_crPopupFlash);
	
	CString key;
	CSisePopup* val{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos)
	{
		m_wnds.GetNextAssoc(pos, key, val);
		if (val)
			val->SetFlashColor(m_crPopupFlash);
	}

	return 0;
}

LRESULT CMapWnd::OnWFFieldChange( WPARAM wParam, LPARAM lParam )
{
	int nID = (int)lParam;

// 	CString s;
// 	s.Format("FIELD CHANGE : %d\n",nID);
// 	OutputDebugString(s);

	nID = nID - 2000;

	if(nID < 0)
	{
		return 0;
	}

	int nSelFields = 0;
	CArray<struct FIELD_INFO*, struct FIELD_INFO*> t_fields;
	for(int n=0; FIELD_INFO[n].name!=NULL; ++n)
	{
		if (m_btnFields[n].GetSafeHwnd())
		{
			if(n == nID)
			{
				m_btnFields[n].SetCheck(!m_btnFields[n].GetCheck());
			}

			if (m_btnFields[n].GetCheck())
			{
				t_fields.Add(&FIELD_INFO[n]);
				if (!FIELD_INFO[n].must) ++nSelFields;
			}
		}
	}
	if (nSelFields==0)
	{
		m_btnFields[nID-IDC_BTN_FIELD_START].SetCheck(TRUE);
		MessageBox("최소 한항목 이상을 선택해야합니다.", "IBK투자증권");
		return 0;
	}
	m_fields.Copy(t_fields);
	
	RequestCode();
	CRect rc;
	CString key;
	CSisePopup* pwnd{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->SetFields( &m_fields, m_vtype );
		pwnd->GetWindowRect(&rc);
		rc.right = rc.left + GetPopupWidth();
		rc.bottom = rc.top + GetPopupHeight();
		pwnd->MoveWindow(&rc, TRUE);
	}

	return 0;
}

void CMapWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	

}

LRESULT CMapWnd::OnSetMainWnd( WPARAM wParam, LPARAM lParam )
{
	m_bMiniMode = (int)wParam;
	m_mainWnd = (CWnd*)lParam;

//	OutputDebugString("SET MAIN WND\n");

	if(m_bMiniMode)
		SetTimer(200,1000,NULL);

	return 0;
}

LRESULT CMapWnd::OnHTSCloseFMini( WPARAM wParam, LPARAM lParam )
{
	if(m_mainWnd->GetSafeHwnd())
	{
		m_mainWnd->SendMessage(WD_HTSCLOSEFORMINI,(WPARAM)0,(LPARAM)0);
	}
	return 0;
}

LRESULT CMapWnd::OnLastView( WPARAM wParam, LPARAM lParam )
{
	const BOOL bChk = (BOOL)lParam;

	m_btnLastView.SetCheck(bChk);

	m_bLastView = bChk;
	return 0;
}

LRESULT CMapWnd::OnCodeNews( WPARAM wParam, LPARAM lParam )
{
	const BOOL bChk = (BOOL)lParam;

	m_btnCodeNews.SetCheck(bChk);

	m_bCodeNews = bChk;
	return 0;	
}

void CMapWnd::OnBtnLastView()
{
	if(m_btnLastView.GetCheck())
	{
		m_bLastView = TRUE;
	}
	else
	{
		m_bLastView = FALSE;
	}
}

void CMapWnd::OnBtnCodeNews()
{
	if(m_btnCodeNews.GetCheck())
	{
		m_bCodeNews = TRUE;
	}
	else
	{
		m_bCodeNews = FALSE;
	}
}

LRESULT CMapWnd::OnInterest( WPARAM wParam, LPARAM lParam )
{
	int gpno;
	CString path;
	
	gpno = (int)wParam;

// 	CString s;
// 	s.Format("OnInterest : %d\n",gpno);
// 	OutputDebugString(s);

	if (gpno<0) return 0;
	
	path.Format("%s\\user\\%s\\portfolio.i%s", Variant(homeCC,""), Variant(nameCC,""), m_gpno[gpno]);
	
	FILE *fp = fopen(path, "rb");
	if (!fp) return 0;
	
	struct _inters inters;
	while( fread(&inters, 1, sizeof(inters), fp) == sizeof(inters) )
	{
		CString code(inters.code, sizeof(inters.code));
		code.Remove(0x00); code.Remove(' ');
		if (code.GetLength()>=3 && code.GetLength()<=8 && inters.gubn[0]!='m')
		{
			AddCode(code);
		}
	}
	RequestCode();
	fclose(fp);

	return 0;
}

LRESULT CMapWnd::OnNewsView( WPARAM wParam, LPARAM lParam )
{
	CSisePopup* pwnd;

	if(!m_wnds.Lookup("S0000",pwnd))
	{
		CSisePopup* pwnd = CreatePopup("S0000", NULL);
		
		pwnd->SetViewType(NEWS_MODE);
	}

	return 0;
}

LRESULT CMapWnd::OnHotTrace( WPARAM wParam, LPARAM lParam )
{
	if(m_mainWnd->GetSafeHwnd())
		m_mainWnd->SendMessage(WX_HOTTRADE,(WPARAM)0,(LPARAM)0);

	return 0;
}

LRESULT CMapWnd::OnTransMax( WPARAM wParam, LPARAM lParam )
{
//	OutputDebugString("TRANSMAX");
	m_transparent = 100;
	m_sbTransparent.SetPos(100);
	CString key;
	CSisePopup* pwnd{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->SetTransparent(m_transparent);
	}
	return 0;
}

LRESULT CMapWnd::OnTransMin( WPARAM wParam, LPARAM lParam )
{
//	OutputDebugString("TRANSMIN");
	m_transparent = 0;
	m_sbTransparent.SetPos(0);
	CString key;
	CSisePopup* pwnd{};
	POSITION pos = m_wnds.GetStartPosition();
	while(pos!=NULL)
	{
		m_wnds.GetNextAssoc(pos, key, pwnd);
		pwnd->SetTransparent(m_transparent);
	}
	return 0;
}

LRESULT CMapWnd::OnHistoryView( WPARAM wParam, LPARAM lParam )
{
//	OutputDebugString("HISTORYVIEW START\n");
	LPCSTR xCode = (LPCSTR)wParam;

	CString scode, sval;
	scode.Format("%s", xCode);
	sval = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)scode.operator LPCTSTR());
	sval.TrimRight();
	//sval = sval.Mid(1);
	scode.Format("%s", xCode);
	if(!sval.IsEmpty())
		m_mapCodeToName.SetAt(scode, sval);

	CRect rc = (LPRECT)lParam;
// 	CString s;
// 	s.Format("ADD CODE : %s\n",xCode);
// 	OutputDebugString(s);
//	OutputDebugString("HISTORYVIEW 1\n");

	CString code = xCode;
	
// 	if(code == "TR")
// 	{
// 		CString history = Variant(historyCC,"1301");
// 		
// 		int index = history.Find("\t");
// 		
// 		if(index > -1)
// 		{
// 			CString str = history.Left(index);
// 			
// 			index = str.Find(" ");
// 			
// 			CString strCode,strName;
// 			
// 			if(index > 0)
// 			{
// 				strCode = str.Left(index);
// 				strName = str.Mid(index + 1);
// 			}
// 			
// 			code = strCode;
// 		}
// 		else
// 		{
// 			code = "";
// 		}
// 	}
//	OutputDebugString("HISTORYVIEW 2\n");
	//if (code.GetLength()>=3 && code.GetLength()<=8)
	if (code.GetLength()>=3 && code.GetLength()<=6)
	{
		if(m_codeHist == code)
		{
			return 0;
		}

		CSisePopup* pwnd;
		
		if(m_wnds.Lookup("H"+m_codeHist,pwnd))
		{
//			OutputDebugString("LOOKUP HIST WND\n");
			//DeletePopup(pwnd);
			//m_codeHist = "";
			pwnd->SetCode("H"+code);

			m_wnds.RemoveKey("H"+m_codeHist);

			m_codeHist = code;

			m_wnds.SetAt("H"+m_codeHist,pwnd);

			RequestCode(m_codeHist);

			rc.right = rc.left + GetPopupWidth();
			rc.bottom = rc.top + GetPopupHeight();

			pwnd->MoveWindow(rc,FALSE);
//			OutputDebugString("END LOOKUP HIST WND\n");

			return 0;
		}
		
		m_codeHist = code;

		//ScreenToClient(rc);
//		OutputDebugString("CREATE HIST WND\n");
		CSisePopup* wnd = CreatePopup("H" + code, rc);
//		OutputDebugString("END CREATE HIST WND\n");
		if(wnd != NULL)
		{
//			OutputDebugString("REQUEST HIST WND\n");
			RequestCode(m_codeHist);
			wnd->SetTransparent(100);
//			OutputDebugString("END REQUEST HIST WND\n");
		}
	}
	else
	{
		MessageBox("지원하지 않는 종목입니다. (3~8자리 입력)", "IBK투자증권", MB_OK);
	}

	return 0;
}

LRESULT CMapWnd::OnHistoryClose( WPARAM wParam, LPARAM lParam )
{
// 	if(m_pHistroy->GetSafeHwnd())
// 	{
// 		m_pHistroy->DestroyWindow();
// 		delete m_pHistroy;
// 		m_pHistroy = NULL;
// 	}
	CSisePopup* pwnd;

	if(m_wnds.Lookup("H"+m_codeHist,pwnd))
	{
//		OutputDebugString("HISTORYVIEW CLOSE\n");
		DeletePopup(pwnd);
//		OutputDebugString("END HISTORYVIEW CLOSE\n");
		m_codeHist = "";
	}

	return 0;
}

LRESULT CMapWnd::OnWine( WPARAM wParam, LPARAM lParam )
{
	m_user = (LPCSTR)wParam;
	m_sMap = (LPCSTR)lParam;

	m_arrMap.Add(m_sMap);

	CString s;

	CString strDept;
	strDept = Variant(deptCC, "");

// 	s.Format("[KSJ] [OnWine] USER [%s] MAP [%s] dept[%s]\n",m_user,m_sMap, strDept);
// 	OutputDebugString(s);

	if(IsDirect())
	{
		//2013.04.17 KSJ 고객만족팀, E-biz 영업, E-biz 지원, 관리자 일때는 가입절차 없이 보여주도록
		CString strView;
		strView.Format("%s/S/t1/d URLDATA\t?id=%s&handle=%d", m_sMap, m_user, (long)m_mainWnd->GetSafeHwnd());
		SetView(strView);
	}
	else
	{
		//와인서비스 요청
		QuerySACAQ601();
	}	
	return 0;
}

LRESULT CMapWnd::OnInGong( WPARAM wParam, LPARAM lParam )
{
	m_user = (LPCSTR)wParam;
	m_sMap = (LPCSTR)lParam;
	
	m_arrMap.Add(m_sMap);

	CString s;
	
	CString strDept;
	strDept = Variant(deptCC, "");
	
// 	s.Format("[KSJ] [OnInGong]USER [%s] MAP [%s] dept[%s]\n",m_user,m_sMap, strDept);
// 	OutputDebugString(s);
	
	if(IsDirect())		//2013.04.17 KSJ 고객만족팀, E-biz 영업, E-biz 지원, 관리자 일때는 가입절차 없이 보여주도록
	{
		CString strView;
		strView.Format("%s00/S/t1", m_sMap.Mid(0,6));
		SetView(strView);
	}
	else
	{
		//와인서비스 요청
		QuerySACAQ601();
	}	
	return 0;
}


void CMapWnd::QuerySACAQ601()
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, _T("SACAQ601"), sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)m_usid, m_usid.GetLength());
	CopyMemory(&ledger.brno, "000", 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
	
	ISACAQ601 isacaq601;
	FillMemory(&isacaq601, L_ISACAQ601, ' ');
	CopyMemory(&isacaq601.zIn, _T("00001"), sizeof(isacaq601.zIn));
	CopyMemory(&isacaq601.zHtsID, (LPCTSTR)m_usid, m_usid.GetLength());
	
	struct _userTH udat;
	
	memset((void*)&udat, 0, sizeof(udat));
	strncpy(udat.trc, _T("pibopbxq"), 8);
	udat.stat = US_ENC | US_KEY;
	udat.key = (BYTE)TR_WINEKEY;
	
	CString strData(_T(""));
	
	strData += CString((char *)&udat, L_userTH);
	strData += CString((char*)&ledger, L_ledgerH);
	strData += CString((char*)&isacaq601, L_ISACAQ601);
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_ISACAQ601 + L_ledgerH), (LPARAM)(LPCTSTR)strData);
}

void CMapWnd::ParseSACAQ601(char* pBuf)
{
	CString strData, strView;

	pBuf = pBuf + L_ledgerH + L_ISACAQ601;

	strData = pBuf;

	OSACAQ601* osacaq601 = (OSACAQ601*)pBuf;
	
	//세자리로 가입여부가 온다., 0 : 미가입,  1 : 가입
	strData = STR(osacaq601->zscvDataID);

// 	OutputDebugString("[KSJ] [" + strData + "]");

	//2013.07.23 KSJ LASTSTAT에 있는 경우 이벤트가 오는것과 조회되는 것이 싱크가 맞지 않아서 한 화면만 열린다.
	//이벤트가 올때마다 저장해 뒀다가 화면을 띄워준다.
	if(m_arrMap.GetSize() > 0)
	{
		m_sMap = m_arrMap.GetAt(0);
		m_arrMap.RemoveAt(0);
	}

	if(!m_sMap.Compare(WINE_S))			//3101 WINE-S 차트
	{
		// escdream check !!! 화면 호출 내용 확인할것 m_mainWnd-<GetSafeHwnd() 를 long로 형변환하여 전송처리함... 
		strView.Format("%s/s/t1/d URLDATA\t?id=%s&handle=%d\nGubn\t%c", m_sMap, m_user, (long)m_mainWnd->GetSafeHwnd(), strData.GetAt(0));
		SetView(strView);
	}
	else if(!m_sMap.Compare(WINE_F))	//3102 WINE-F 차트
	{
		strView.Format("%s/s/t1/d URLDATA\t?id=%s&handle=%d\nGubn\t%c", m_sMap, m_user, (long)m_mainWnd->GetSafeHwnd(), strData.GetAt(1));
		SetView(strView);
	}
	//2013.05.23 KSJ IB715100으로 호출하면 이벤트가 무한으로 들어온다. 그래서 메뉴에서 IB715101로 고치고 메인도 같이 수정하여 해결했음.
	else if((!m_sMap.Compare("IB715101") || !m_sMap.Compare("IB715201")) && strData.GetAt(2) == '1')	//3103 김종철소장의 인공지능차트
	{
		strView.Format("%s00/S/t1", m_sMap.Mid(0,6));

// 		OutputDebugString("[KSJ] [" + strView + "]");
		SetView(strView);
	}
	else if(m_sMap.Find("IB714") != EOF)		//WINE 가입안내 페이지
	{
		strView.Format("%s/s/t1", WINE_JOIN);
		SetView(strView);
	}
	else if(m_sMap.Find("IB715") != EOF)		//인공지능 가입안내 페이지
	{
		strView.Format("%s/s/t1", "IB715300");
		SetView(strView);
	}

	//2013.07.24 KSJ 화면이 다 열리면 이벤트 보낸다.
	if(m_arrMap.GetSize() == 0)
	{
		if(m_mainWnd->GetSafeHwnd())
		{
			m_mainWnd->SendMessage(WD_FOCUSNOTICE,(WPARAM)0,(LPARAM)0);
		}
	}
}

LRESULT CMapWnd::OnWineWnd( WPARAM wParam, LPARAM lParam )
{
	HWND hwnd = (HWND)lParam;

	m_wineHwnd = hwnd;

	return 0;
}

//2013.07.03 KSJ 
//직원용 고객만족팀, E-biz 영업, E-biz 지원, 관리자
//고객용 정보전략팀 아이디
bool CMapWnd::IsDirect()
{
	CString strDept;
	strDept = Variant(deptCC, "");

	CStringArray strUserId;
	strUserId.Add("devilswo");
	strUserId.Add("hwanmun");
	strUserId.Add("dundas");
	strUserId.Add("haejoy");
	strUserId.Add("mira99");
	strUserId.Add("minkyu42");
	strUserId.Add("yookb");
	strUserId.Add("onetym00");
	strUserId.Add("emailp");
	strUserId.Add("june6365");

	if(!strDept.Compare("916") || !strDept.Compare("937") || !strDept.Compare("912") || !strDept.Compare("903") || !strDept.Compare("999"))
		return true;
	
	for(int ii = 0; ii < strUserId.GetSize(); ii++)
	{
		if(!m_usid.Compare(strUserId.GetAt(ii)))
			return true;
	}

	return false;
}
