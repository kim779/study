 #include "stdafx.h"
#include "IB771000.h"
#include "MapDlg.h"
#include "../../control/fx_misc/misctype.h"
#include "../../axis/axMsg.hxx"
//#include "../../axis/childfrm.h"
//#include "../../axis/schild.h"
#include "SearchWordSetDlg.h"
#include "OptionWnd.h"
#include "MarketDlg.h"

#include "../H/interMSG.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define	_FILE_DEBUG
#ifdef	_FILE_DEBUG
#include <fstream.h>
#endif

#ifdef	_FILE_DEBUG
	ofstream dFile("C:\\IB771000.trc",ios::app);
#endif


#pragma warning(disable:4800)		// m_bSelect가 BOOL, compile시 warning안나도록

#define IDC_CODECTRL	1003		// code control id
#define IDC_HTMLVIEW	1000		// browser id

#define SIZE_MINLIST	80
#define SIZE_CONTENT	150
#define MAX_WORDSIZE	100
#define	TMID_TOTAL	1000		// Timer
#define	WAITQUERY	1100		// Timer
#define DO_INNER_JOB	1111

#define TM_HTMLFILE 1112
#define TM_EDGECONTROLRESIZE 1113

#define LENGTH_OF_LINE  130
#define	MAXW			627
static	int g_htmlindex = 0;		// 뉴스 내용 저장 index

#define		BACKINDEXCOLOR			64
#define		TOOLINDEXCOLOR			66

//#define NEWS_SIZEGUBN 13
#define NEWS_SIZEGUBN 20
#define DF_WHILE
/////////////////////////////////////////////////////////////////////////////

//test edge
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
std::string URLEncodeUTF8(const std::string& strUTF8) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : strUTF8) {
		// 안전한 문자는 그대로 유지, 특수문자는 URL 인코딩
	//	if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
		if (c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		}
		else if (c == ' ') {
			escaped << '+';
		}
		else {
			escaped << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(c));
		}
	}

	return escaped.str();
}

std::string ToUpperCase(const std::string& input) {
	std::string result = input;

	for (char& c : result) {
		c = std::toupper(c);
	}

	return result;
}

#define	HANGULMSK	0x80
CString ExtractHangulCharacters(const CString& input) {
	CString result;
	CString stmp;

	int len = input.GetLength();
	int i = 0;
	WORD wHangul;
	while (i < len) {
		wchar_t ch = input[i];

		char cName;
		cName = input.GetAt(i);
		if (cName & HANGULMSK)
		{
			stmp = input.Mid(i, 2);
			result += stmp;
			i++;
		}
		else if (cName == '(' || cName == ')')
		{
			stmp = input.Mid(i, 1);
			result += stmp;
		}

		i++;
	}
	return result;
}

CString ExtractHangulCharactersAndURLIncode(const CString& input) 
{
	CString result;
	CString stmp;

	int len = input.GetLength();
	int i = 0;
	WORD wHangul;

	while (i < len) {
		wchar_t ch = input[i];
		char cName;
		cName = input.GetAt(i);
		if (cName & HANGULMSK)
		{
			stmp = input.Mid(i, 2);

			std::wstring strUni = CA2W(stmp); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::string strUTF8 = converter.to_bytes(strUni);
			std::string stdres = ToUpperCase(URLEncodeUTF8(strUTF8));
			CString strEnCode;
			strEnCode = stdres.c_str();
			result += strEnCode;

			i++;
		}
		else if (cName == '(' || cName == ')')
		{
			stmp = input.Mid(i, 1);

			std::wstring strUni = CA2W(stmp); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
			std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
			std::string strUTF8 = converter.to_bytes(strUni);
			std::string stdres = ToUpperCase(URLEncodeUTF8(strUTF8));
			CString strEnCode;
			strEnCode = stdres.c_str();
			result += strEnCode;
		}
		else
			result += CString(input[i], 1);

		i++;
	}
	return result;
}


CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapDlg::IDD, pParent)
{
	m_iSelectSubGubn = -1;
	m_comboSearch.SetParentWnd(this);
	m_List.SetParentWnd(this);
	m_List.SetNewsTypeColumn(0);

	m_pBrowser = NULL;
	m_pParent =pParent;
	m_szRootDir = "";
	m_szUser = "";
	m_szFileName = "";

	m_bJmyd = TRUE;
	m_bTrigger = false;
	memset( m_cDetailSelect,0x01, MAX_NEWS*MAX_NEWS_SUB);
	for(int i = 0 ; i < MAX_NEWS ; i++)
	{
		m_clrTitleText[i] = RGB(255,0,0);
	}
	
	memset( m_bSelect,0x01,sizeof(m_bSelect));
	
	m_szCode= "";
	m_bTotList = TRUE;
	
	m_bSelectScrap = FALSE;

	m_bAutoQuery = TRUE;
	m_bTotSearch = TRUE;
	m_bx = 44;
	m_by = 18;
	
	m_symbol   = NULL;
	m_inst     = NULL;
	m_code     = NULL;

	m_szDate = "";
	m_szSysGubn = "";
	memset(m_szSave, 0x00, 80 );
	memset(m_szCurKey, 0x00, 23 );
	m_bRetail = FALSE;
	m_Ypos = -1;
	m_YMovepos = -1;
	m_XMovepos = -1;
	m_pTab = nullptr;
	m_pMapWnd1 = nullptr;
	m_pMapWnd2 = nullptr;
	m_bTriggerTR = false;

	m_nTreeWidth	= TREE_WIDTH;

	m_bCodeTrigger = FALSE;
	m_pRadioTot	= nullptr;
	m_pRadioSel	= nullptr;

	m_rcBorderTree.SetRect(0, 0, 0, 0);
	m_nOptHeight	= SIZE_BUTTONS;
	m_bOptCheck	= FALSE;
	m_pOptionWnd	= nullptr;

	m_bDaetoo	= TRUE;
	m_bNextPushed	= false;

	m_tabIdx = -1;
	m_bCapture = false;
	
	m_bTwoToolBar = FALSE;

	m_nTrkey = type_total;
	//트리 선택 ITem 임시 저장용
	select_hItem = NULL;
	select_tab = 0;
	m_bCheckNewCount = FALSE;

	m_pFilterWnd = nullptr;
	
	m_bCategory = FALSE;

	m_bCodeNews = FALSE;

	m_bInitSize = TRUE;

	m_bColShow = FALSE;
	m_bInitReq = FALSE;

	m_selHistory.RemoveAll();

	m_bDomino = false;

	m_hGlobal = nullptr;
	m_pStream = nullptr;

	//test edge
	HINSTANCE hins;
	BrowserWindowEdge::InitInstance(hins);
}

CMapDlg::~CMapDlg()
{
#ifdef	_FILE_DEBUG
	dFile.close();
#endif
	if (m_symbol)
	{
		if (IsWindow(m_symbol->GetSafeHwnd()))
			m_symbol->SendMessage(WM_CLOSE);
		m_symbol->Detach();
		delete m_symbol;
	}
	if (m_code)
	{
		delete m_code;
		m_code = NULL;
	}

	if (m_inst)	AfxFreeLibrary(m_inst);
	
	if (m_pRadioTot)
	{
		m_pRadioTot->DestroyWindow();
		delete m_pRadioTot;
	}
	
	if (m_pRadioSel)
	{
		m_pRadioSel->DestroyWindow();
		delete m_pRadioSel;
	}

	//m_btOption.DestroyWindow();  //vc2019

// 	if (m_pOptionWnd)
// 	{
// 		m_pOptionWnd->DestroyWindow();
// 		delete m_pOptionWnd;
// 	}

//	DeleteCriticalSection(&m_cs);
}

void CMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapDlg)
	DDX_Control(pDX, IDC_BUTTON_VIEWMODE, m_btnViewMode);
	DDX_Control(pDX, IDC_BUTTON_RCVSIZE, m_btnSizeInit);
	DDX_Control(pDX, IDC_CHECK_SISE, m_btnSise);
	DDX_Control(pDX, IDC_FONTSIZE, m_pFontSize);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_pStatic);
	DDX_Control(pDX, IDC_BUTTON_PREVIEW, m_btnPreview);
	DDX_Control(pDX, IDC_BUTTON_SCRAP, m_btnScrap);
	DDX_Control(pDX, IDC_EDIT1, m_clSearchEdit);
	DDX_Control(pDX, IDC_TOOLBARPOSBUTTON, m_btToolPos);
	DDX_Control(pDX, IDC_CHECK2, m_btTrg);
	DDX_Control(pDX, IDC_COMBO1, m_cblTrgType);
	DDX_Control(pDX, IDC_TREE, m_TreeWnd);
	DDX_Control(pDX, IDC_BT_RIGHT, m_btnRight);
	DDX_Control(pDX, IDC_BT_LEFT, m_btnLeft);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_comboSearch);
	DDX_Control(pDX, IDC_BUTTON_JONGMOK, m_btnJM);
	DDX_Control(pDX, IDC_BUTTON_SELL, m_btnSell);

	DDX_Control(pDX, IDC_BUTTON_PRC, m_btnPrc);
	DDX_Control(pDX, IDC_BUTTON_CT, m_btnCt);
	DDX_Control(pDX, IDC_BUTTON_COMPANY, m_btnCp);
	DDX_Control(pDX, IDC_BUTTON_MMDH, m_btnMmdh);
	DDX_Control(pDX, IDC_BUTTON_FAVG, m_btnFavg);

	DDX_Control(pDX, IDC_BUTTON_SEARCH_SET, m_btnSSet);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BUTTON_NEXTPAGE, m_btnNext);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_TimeCtrl);
	DDX_Control(pDX, IDC_LIST_NEWS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapDlg, CDialog)
	//{{AFX_MSG_MAP(CMapDlg)
	ON_WM_ERASEBKGND()
	ON_WM_SIZING()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(WM_COMBO_ENTERED, OnComboEnter)
	ON_CBN_SELENDOK(IDC_CODECTRL, OnSelendokComboSearch)
	ON_MESSAGE(WM_REQUEST_NEXT,OnListRequestNext)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_NEWS, OnRclickListNews)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NEWS, OnClickListNews)
	ON_MESSAGE(WM_SIZE_WEBBROW, OnWebBrowSize)
	ON_MESSAGE(WM_SIZING_WEBBROW, OnWebBrowSizing)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER, OnCloseupDatetimepicker)
	ON_BN_CLICKED(IDC_RADIO_TOT, OnRadioTot)
	ON_BN_CLICKED(IDC_RADIO_SEL, OnRadioSel)
	ON_BN_CLICKED(IDC_BUTTON_JONGMOK, OnButtonJongmok)
	ON_BN_CLICKED(IDC_BUTTON_SELL, OnButtonSell)

	ON_BN_CLICKED(IDC_BUTTON_PRC, OnButtonPrc)
	ON_BN_CLICKED(IDC_BUTTON_CT, OnButtonCt)
	ON_BN_CLICKED(IDC_BUTTON_COMPANY, OnButtonCompany)
	ON_BN_CLICKED(IDC_BUTTON_MMDH, OnButtonMmdh)
	ON_BN_CLICKED(IDC_BUTTON_FAVG, OnButtonFavg)

	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_SET, OnButtonSearchSet)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPAGE, OnButtonNextpage)
	ON_BN_CLICKED(IDC_CONBUTTON, OnSetNewsType)
	ON_BN_CLICKED(IDC_TOOLBARPOSBUTTON, OnToolbarRePosition)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_SCRAP, OnButtonScrap)
	ON_WM_NCDESTROY()
	ON_CBN_SELCHANGE(IDC_FONTSIZE, OnSelchangeFontsize)
	ON_BN_CLICKED(IDC_CHECK_SISE, OnCheckSise)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_RCVSIZE, OnButtonRcvsize)
	ON_BN_CLICKED(IDC_BUTTON_VIEWMODE, OnButtonViewmode)
	ON_BN_CLICKED(IDC_STATIC_TITLE, OnStaticTitle)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeOrdTab)
	ON_MESSAGE(WM_MANAGE, OnManage)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_SEND, OnSend)
	ON_MESSAGE(WM_WXMSG, OnWxEditMessage)
END_MESSAGE_MAP()

void CMapDlg::DebugLog(CString strText) 
{
	strText = "ML " + strText;
	OutputDebugString(strText.operator LPCTSTR());
}

LRESULT CMapDlg::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch ((int)wParam)
	{
	case EVT_SENDCONTENT: 
		RequestContent(lParam);
		break;
	case EVT_SENDNAVIGATE:		
		if (!m_pBrowser)
			break;

		try
		{
#ifndef DF_TEST
			if (m_bUsingEdgeBrowser)  //test edge
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
				{
					CString strEdgeUrl;
					strEdgeUrl.Format("%s", (char*)lParam);

					strEdgeUrl = ExtractHangulCharactersAndURLIncode(m_szFileName);
					strEdgeUrl = "file:///" + strEdgeUrl;
					std::string message = strEdgeUrl;
					std::wstring wnewsurl;
					wnewsurl.assign(message.begin(), message.end());
					browserEdge->Init(wnewsurl);

					//m_slog.Format("[edge][%s] strkor = [%s] ", __FUNCTION__, strkor);
					//OutputDebugString(m_slog);

					//std::wstring strUni = CA2W(strkor); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
					//std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
					//std::string strUTF8 = converter.to_bytes(strUni);
					//std::string stdres = ToUpperCase(URLEncodeUTF8(strUTF8));
					//CString strEnCode;
					//strEnCode = stdres.c_str();
					//strEdgeUrl.Replace(strkor, strEnCode);
					//strEdgeUrl = "file:///" + strEdgeUrl;
					//std::string message_a = strEdgeUrl;
					//std::wstring wurl;
					//wurl.assign(message_a.begin(), message_a.end());
					//browserEdge->Init(wurl);
				}
			}
			else
				m_pBrowser->Navigate2((char *)lParam);		
#endif
		}
		catch ( CException* e)
		{
			e->ReportError();			
		}
		catch ( ... ) {
			AfxMessageBox("CMapDlg::OnSend");
		}
		
		break;
	}
	return FALSE;
}

LONG CMapDlg::OnManage(WPARAM wParam, LPARAM lParam)
{
	const LONG		ret = 0;
	HTREEITEM	hItem{};

	try
	{
		switch (wParam)
		{
		case MSG_TREE_CLICK :		// Tree Click	
			{
// 				OutputDebugString("Pre MSG_TREE_CLICK");
				hItem = (HTREEITEM)lParam;
				select_hItem = (HTREEITEM)lParam; //임시 저장
				m_bSelectScrap = FALSE;
				m_List.SetNewsType(0);
				m_List.SetSelectionMark(MSG_TREE_CLICK);

				m_nTrkey = (sendtr_type)SetMediaFilter(hItem);

				if(m_nTrkey == type_category)
				{
					m_bCategory = TRUE;
				}
				else
				{
					m_bCategory = FALSE;
				}

				if( IsNewsCountTR( m_nTrkey ) )				
					SetCountNewsCtrl( FALSE );								
				else				
					SetCountNewsCtrl( TRUE );
				
				m_List.InvalidateHedarCtrl();

				if( IsNewsCountTR( m_nTrkey ) )	
				{
//  					OutputDebugString("Pre RequestCountList");
					RequestCountList();
//  					OutputDebugString("Next RequestCountList");
				}
				else if( m_nTrkey == type_scrap )
				{
					m_bSelectScrap = TRUE;
					m_List.SetNewsType(1);
					SetScrapNews(); 
				}
				else		
				{
//  					OutputDebugString("Pre RequestTitleList");
					RequestTitleList(TRUE);	
//  					OutputDebugString("Next RequestTitleList");
				}
				
				if(m_bColShow)
				{
					ResizeTransformControl();
				}
// 				OutputDebugString("Next MSG_TREE_CLICK");
			}
			break;
		case MSG_TREECHK_CLICK :
 //			OutputDebugString("7710 Pre MSG_TREECHK_CLICK");
			SaveNodeSelect();
// 			OutputDebugString("Next MSG_TREECHK_CLICK");
			break;
		case MSG_COLOR_CHANGE :		// Option Window color change
			// 뉴스 종류에 따라 색상 다르게 설정 가능(0:머니투데이, 1:인포스탁, 2:한경뉴스, 3:이데일리)
			/*
			m_clrTitleText[0] = m_pOptionWnd->GetNewsColor(idMoneyToday);
			m_clrTitleText[1] = m_pOptionWnd->GetNewsColor(idInfoStock);
			m_clrTitleText[2] = m_pOptionWnd->GetNewsColor(idHanNews);
			m_clrTitleText[3] = m_pOptionWnd->GetNewsColor(idEDaily);

			m_clrTitleText[4] = m_pOptionWnd->GetNewsColor(idYounhap);
			m_clrTitleText[5] = m_pOptionWnd->GetNewsColor(idMaekyoung);
			m_clrTitleText[6] = m_pOptionWnd->GetNewsColor(idEToday);
			m_clrTitleText[7] = m_pOptionWnd->GetNewsColor(idNewsp);
			m_List.Invalidate(FALSE);
			*/
			break;
		case MSG_OPTION_CHECK :		// Option Window check change
#ifdef _DEBUG
			AfxMessageBox( "MSG_OPTION_CHECK" );
#endif
//			m_bAutoQuery = m_pOptionWnd->GetCheckContext();	
			break;
		}
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	catch (...)
	{
		AfxMessageBox("CMapDlg::OnManage");
	}

	return ret;
}

void CMapDlg::LoadScreenView()
{
	m_nView = GetConfigFileInt(m_sFile, _T("TOOL"), _T("VIEW"), 0 );
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(GetConfigFileInt(m_sFile, _T("TOOL"), _T("TRIGGER"), 0 ));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(GetConfigFileInt(m_sFile, _T("TOOL"), _T("TRIGGER_TYPE"), 0 ));

//	keys
	CString sFile, strSetion, strSetion2;
	sFile.Format("%s\\tab\\%s", m_szRootDir, NEWS_CONFIG_FILE);//root\tab\newtab

	m_arSpecialKeyList.RemoveAll();
	m_arStepColor.RemoveAll();
	int ikeycnt = GetConfigFileInt(sFile, _T("S_KEYS"), _T("COUNT"), 0 );
	for(int i=0; i<ikeycnt; i++)
	{
		strSetion.Format("K_%02d", i+1);
		strSetion2.Format("C_%02d", i+1);
		m_arSpecialKeyList.Add(AList(GetConfigFileString(sFile,  _T("S_KEYS"), strSetion, ""), GetConfigFileInt(sFile,  _T("S_KEYS"), strSetion2, 0)));
	}

	ikeycnt = GetConfigFileInt(sFile, _T("S_COLOR"), _T("COUNT"), 0 );
	for(int i=0; i<ikeycnt; i++)
	{
		strSetion.Format("C%02d", i+1);
		m_arStepColor.Add(GetConfigFileInt(sFile,  _T("S_COLOR"), strSetion, 0) );
	}

	ikeycnt = GetConfigFileInt(sFile, _T("LINKMAP"), _T("COUNT"), 0 );
	for(int i=0; i<ikeycnt; i++)
	{
		strSetion.Format("N_%02d", i);
		m_arTabNameList.Add( GetConfigFileString(sFile,  _T("LINKMAP"), strSetion, ""));

		strSetion.Format("M_%02d", i);
		m_arMapList.Add( GetConfigFileString(sFile,  _T("LINKMAP"), strSetion, ""));


		strSetion.Format("W_%02d", i);
		m_arMapWidth.Add( GetConfigFileString(sFile,  _T("LINKMAP"), strSetion, ""));

		strSetion.Format("H_%02d", i);
		m_arMapHeight.Add( GetConfigFileString(sFile,  _T("LINKMAP"), strSetion, ""));
	}

	m_iWriteLog = GetConfigFileInt(sFile,  _T("LOG"), _T("WRITE"), 0);
}

void CMapDlg::SaveKeyConfig()
{
	CString sFile, strSetion, strKey;
	sFile.Format("%s\\tab\\%s", m_szRootDir, NEWS_CONFIG_FILE);//root\tab\newtab

	int ikeycnt = m_arSpecialKeyList.GetSize();
	WriteConfigFileInt(sFile, _T("S_KEYS"), _T("COUNT"), ikeycnt);
	for(int i=0; i<ikeycnt; i++)
	{
		strKey.Format("K_%02d", i+1);
		WritePrivateProfileString(_T("S_KEYS"), strKey, (LPTSTR)(LPCTSTR)m_arSpecialKeyList.GetAt(i).sTxt, sFile);

		strKey.Format("C_%02d", i+1);
		WriteConfigFileInt(sFile, _T("S_KEYS"), strKey, m_arSpecialKeyList.GetAt(i).iVal);
	}

	ikeycnt = m_arStepColor.GetSize();
	WriteConfigFileInt(sFile, _T("S_COLOR"), _T("COUNT"), ikeycnt);
	for(int i=0; i<ikeycnt; i++)
	{
		strKey.Format("C%02d", i+1);
		WriteConfigFileInt(sFile, _T("S_COLOR"), strKey, m_arStepColor.GetAt(i));
	}
}

void CMapDlg::SaveScreenView()
{
	WriteConfigFileInt(m_sFile, _T("TOOL"), _T("VIEW"), m_nView);
	WriteConfigFileInt(m_sFile, _T("TOOL"), _T("TRIGGER"), ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck());
	int isel = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	isel = max(isel, 0);
	WriteConfigFileInt(m_sFile, _T("TOOL"), _T("TRIGGER_TYPE"), isel);
}

void CMapDlg::SaveNodeSelect()
{
	CString selectData;

	for(int i=0;i<m_TreeWnd.m_selItem.GetSize();i++)
	{
		HTREEITEM hTmp = (HTREEITEM)m_TreeWnd.m_selItem.GetAt(i);

		selectData += m_TreeWnd.GetItemText(hTmp) + ":";
	}

	selectData.TrimRight(":");

	WritePrivateProfileString( _T("SELITEM"), _T("ITEMS"), selectData,m_sFile);
	WritePrivateProfileString( _T("SELITEM"), _T("VERSION"), "4",m_sFile);    //modi 뉴스웨이 추가 버전업 3->4
}

void CMapDlg::parsingNewsCount( char* inBuf, int len )
{
	char* pBuf = inBuf;

	CString strResultMsg( pBuf, 50 );
	pBuf += 50;
	CString strCurPage( pBuf, 4 );
	pBuf += 4;
	CString strTotalPage( pBuf, 4 );
	pBuf += 4;
	CString strRecCount( pBuf, 4 );
	pBuf += 4;
		
	const int iCurPage = atoi(strCurPage);
	const int iTotalPage = atoi(strTotalPage);
	const int iRecCount = atoi(strRecCount);

	//if( iCurPage == m_sPageHeader.iCurPage && iTotalPage == m_sPageHeader.iMaxPage )
	//	return; // 같은 페이지 가 여러번 온다 

	m_sPageHeader.iCurPage = iCurPage;
	m_sPageHeader.iMaxPage = iTotalPage; 
	m_sPageHeader.iRecCnt = iRecCount;	

// 	TRACE("Cur(%d)Total(%d)\n",iCurPage,iTotalPage);

	const int nShow =m_List.GetItemCount();
	if (nShow == 0)		
		emptyFile();

	if(iRecCount == 0)	
		m_bCheckNewCount = TRUE;	
	else	
		m_bCheckNewCount = FALSE;

	if (m_sPageHeader.iCurPage==m_sPageHeader.iMaxPage)
		m_btnNext.EnableWindow(FALSE);
	else 
		m_btnNext.EnableWindow(TRUE);

	for( int i = 0; i < iRecCount; i++ )
	{
		news_info news;

		strcpy( news.titl, CString( pBuf, 80 ));
		pBuf += 80;
		news.flag = CString( pBuf, 1);
		pBuf += 1;

		CString strTmp = CString( pBuf, 6);
		news.time = strTmp.Left(2) + ":" + strTmp.Mid(2,2);
		pBuf += 6;

		news.code = CString( pBuf, 12 );
		news.code.TrimRight();
		pBuf += 12;
		
		news.hnam = m_code->GetCodeName( news.code );

		news.rCnt = atoi( CString( pBuf, 6));
		pBuf += 6;

		news.date = CString( pBuf, 8);
		pBuf += 8;
		
		news.gubn = CString( pBuf, 2); 
		pBuf += 2;
		news.subg = CString( pBuf, 4);
		pBuf += 4;
		news.key = CString( pBuf, 10 );
		pBuf += 10;		

		news.keyv = news.date+news.gubn+news.subg+news.key;

		const int iGubn = atoi( news.gubn );
		const int iSubg = atoi( news.subg );
		for( int i = 0; i < m_arNewsList.GetSize(); i++ )
		{
			NewsList tnews = m_arNewsList.GetAt(i);
			if( tnews.iGubn == iGubn )
			{
				news.gubn = tnews.sGubnName;

				if( iGubn == 2 ) // 뉴스사가 공시 이고 99(코스닥),88(프리보드) 가 아니면 거래소 공시로 표시 
				{
					if( iSubg != 99 && iSubg != 88 && iSubg != 66 )	//2013.06.25 KSJ 66(코넥스공시) 추가
						news.subg = "거래소공시";
				}						

				for( int j = 0; j < tnews.ibulruCnt; j++ )
				{
					if( atoi(tnews.sBulruGubnList[j]) == iSubg )
					{
						news.subg = tnews.sBulruNameList[j];
					}
				}
			}
		}		
#ifdef _DEBUG
		CString str;
		str.Format("(%d),(%s),(%s),(%s),(%s),(%s),(%s),(%s)", 
			news.rCnt,news.titl, news.date, news.flag, news.time, news.gubn, news.subg, news.key);		
#endif 
// 		OutputDebugString("[KSJ] parsingNewsCount");
		gridAddRow( news, true, true ); 
	}		

	if (m_bNextPushed)
	{
		PostMessage(WM_SEND, EVT_SENDCONTENT, nShow);
		m_List.SetSelectionMark(nShow);
		m_List.SetItemState(nShow,LVIS_SELECTED , LVIS_SELECTED);
		
		m_List.SetScroll(nShow, TRUE);
		
		m_bNextPushed = false;
	}
	else if (m_bAutoQuery)
	{	
		PostMessage(WM_SEND, EVT_SENDCONTENT, 0);
		m_List.SetSelectionMark(0);
		m_List.SetItemState(0,LVIS_SELECTED , LVIS_SELECTED);
	}
}

CString	CMapDlg::GetGubnName( CString sGubn )
{
	const int iGubn = atoi( sGubn );
	for( int i = 0; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList tnews = m_arNewsList.GetAt(i);
		if( tnews.iGubn == iGubn )
			return tnews.sGubnName;
	} 
	return "";
}

CString GetMessage(int i)
{
	CString str;

	switch(i)
	{
	case DLL_OUB:		str = "DLL_OUB";	break;
	case DLL_OUBx:		str = "DLL_OUBx";	break;	//2015.11.05 KSJ DLL_OUBx 추가
// 	case DLL_ALERT:		str = "DLL_ALERT";	break;
	case DLL_ALERTx:	str = "DLL_ALERTx";	break;	//2012.01.19 KSJ DLL_ALERTx 추가
	case DLL_NOTICE:	str = "DLL_NOTICE";	break;
	case DLL_SETPAL:	str = "DLL_SETPAL";	break;
	case DLL_SELECT:	str = "DLL_SELECT";	break;
	case DLL_DOMINO:	str = "DLL_DOMINO";	break;
	case DLL_SETFONT:	str = "DLL_SETFONT";	break;
	case DLL_TRIGGER:	str = "DLL_TRIGGER";	break;
	default:			str = "UNKNOWN";	break;		
	}

	return str;
}

void CMapDlg::GetNavigate( CString str )
{
	m_section.Lock();

	_jobObj job;
	job.msg = DO_NAVIGATE;
	job.msgBuf = str;

	m_bADDING = TRUE;
m_slog.Format("[7710] !!!!!!!!ADD[%x]!!!!!     !!!!!GetNavigate [%d] size =[%d]", this, job.msg, m_jobList.GetSize());
OutputDebugString(m_slog);

	m_jobList.AddTail(job);

	m_bADDING = FALSE;
	m_section.Unlock();
}

void CMapDlg::GetObject(WPARAM wParam, LPARAM lParam )
{
	m_section.Lock();

	BOOL bAdd = FALSE;
	const BOOL bAlert = FALSE;
	BOOL bAlertx = FALSE; //2012.01.19 KSJ Alertx 추가

	_jobObj job;

	job.key = HIBYTE(LOWORD(wParam));
	job.msg = LOBYTE(LOWORD(wParam));

	if (job.msg == DLL_OUBx)
	{
		struct _extTHx* extTH = (struct _extTHx*)lParam;
		job.len = extTH->size;
	}
	else
		job.len = HIWORD(wParam);
	
	//job.len = HIWORD(wParam);
	//if( lParam != NULL && job.len > 0 && job.msg != DLL_SETFCB )	//2015.10.26 KSJ DLL_SETFCB에서 오류나므로 
	if (lParam != NULL && job.len > 0 && job.msg != DLL_SETFCB && job.msg != DLL_OUBx)	//2015.10.26 KSJ DLL_SETFCB에서 오류나므로 
		job.msgBuf = CString( (char*)lParam, job.len );
	if( job.msg == DLL_TRIGGER || job.msg == DLL_DOMINO )
		job.msgBuf = CString( (char*)lParam );

	if( job.msg == DLL_OUB || job.msg ==DLL_OUBx)
	{
		if( job.key >= type_gongsi && job.key <= type_news_count_month )
			bAdd = TRUE;

		if(job.key >= TRKEY_MEMO_POPUPOOP && job.key <= TRKEY_MEMO_CHECK) 
			bAdd = TRUE;
	}
		//2012.01.19 KSJ AlertX 추가
	if( job.msg == DLL_ALERTx)
	{
		if( IsAddAlertx() )
			bAlertx = TRUE;
	}
		//2012.01.19 KSJ Alertx 추가 끝

	if( job.msg == DLL_DOMINO || job.msg == DLL_TRIGGER || job.msg == DLL_SETPAL )
		bAdd = TRUE;

	if( bAdd )
	{
		if(job.msg ==DLL_OUBx)
		{
			if( lParam != NULL )
			{
	//			char* pBytes = (char*)lParam;
				
				struct _extTHx* extTH = (struct _extTHx*)lParam;
// 				CString strTemp;
// 				strTemp.Format("[KSJ] key[%c] size[%d]", extTH->key, extTH->size);
// 				OutputDebugString(strTemp);
				
				job.key = extTH->key;
				job.len = extTH->size;
				job.msgBuf = CString(extTH->data, job.len );
			}
		}
		else
		{
			job.len = HIWORD(wParam);
			if( lParam != NULL && job.len > 0 )
			{
				job.msgBuf = CString( (char*)lParam, job.len );
			}
			if( job.msg == DLL_TRIGGER || job.msg == DLL_DOMINO )
				job.msgBuf = CString( (char*)lParam );
		}
	}
	//2012.01.19 KSJ AlertX 추가
	else if (bAlertx)
	{
		if( lParam != NULL )
			job.alertR = (struct _alertR*)lParam;
		
		CString sRealCode = job.alertR->code;
		sRealCode = Trim(sRealCode);

		CString strTemp;
		
		if (sRealCode.CompareNoCase("S0000") == 0)
		{
			parsingAlertx(job.alertR);	//2012.03.08 KSJ 리스트에 넣지 않고 함수를 바로 호출한다.
		}
	}
	//2012.01.19 KSJ Alertx 추가 끝

	if (bAdd /*||*//* bAlert ||*//* bAlertx*/) 	//2012.01.19 KSJ AlertX 추가
	{
		m_section.Lock();

		m_bADDING = TRUE;
//m_slog.Format("[7710]  !!!!!!!!ADD[%x]!!!!!    GetObject [%d] size =[%d] len=[%d] buf=[%.20s] m_bTriggerTR=[%d]", 
//	this, job.msg, m_jobList.GetSize(), job.len, job.msgBuf, m_bTriggerTR);
//OutputDebugString(m_slog);
	
#ifdef DF_WHILE
		if (job.msg == DLL_TRIGGER)
		{
			CString tmpS = job.msgBuf;
			CString	sSymbol;
			sSymbol = Parser(tmpS, "\t");
			tmpS.TrimRight();

			if (tmpS.GetLength() != 6)
			{
				m_bADDING = FALSE;
				m_section.Unlock();
				return;
			}
		// if (sSymbol != "IMAXSKIN")
		//	{
		//		m_slog.Format("[7710]  GetObject m_szCode=[%s]  tmpS=[%s] sSymbol=[%s]", m_szCode, tmpS, sSymbol);
		//		OutputDebugString(m_slog);
		//	}

			if (sSymbol == "IMAXSKIN")
			{
				m_bADDING = FALSE;
				m_section.Unlock();
				return;
			}
		
			//tests
			//if (tmpS == "1")
			//{
			//	m_slog.Format("[7710]      GetObjects Symbol=[%s] [%d]  size =[%d] len=[%d] buf=[%.20s]",
			//		sSymbol, job.msg, m_jobList.GetSize(), job.len, job.msgBuf);
			//	OutputDebugString(m_slog);
			//}
			//teste

			if (tmpS != m_szCode)
			{
				if (!m_bTriggerTR)
				{
					if(!m_bTotSearch)
						m_code->SetEditData(tmpS);
					m_slog.Format("1_SetEditData_[%s]", tmpS);
					OutputDebugString(m_slog);
					m_SetTime_job = job;
					SetTimer(1301,100,NULL);

				}
			}
			//else
			//{
			//   m_slog.Format("[7710]  not ADD");
			//	OutputDebugString(m_slog);
			//	m_bADDING = FALSE;
			//	m_section.Unlock();
			//	return;
			//}
		}
		else
#endif
			m_jobList.AddTail(job);
	
		m_bADDING = FALSE;
		m_section.Unlock();
	}

	m_section.Unlock();
}

void CMapDlg::InitSavedSize()
{
	m_bInitSize = FALSE;
	
	CString strINI(_T(""));
	CString strSise;
	char readB[256]{};
	int readL = 0;
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
	
	CString strW,strH;
	
	readL = GetPrivateProfileString("IB771000", "WIDTH", "0", readB, sizeof(readB), strINI);
	strW = CString(readB, readL);
	
	readL = GetPrivateProfileString("IB771000", "HEIGHT", "0", readB, sizeof(readB), strINI);
	strH = CString(readB, readL);

	const int top = GetPrivateProfileInt("IB771000", "TOP", 100, strINI);
	int left = GetPrivateProfileInt("IB771000", "LEFT", 0, strINI);

	if(left > 1280)
	{
		left = left - 1280;
	}
	
	//CRect r;
	//GetWindowRect(&r);
	
	//CString str;
	//str.Format("%d %d\n",r.Width(),r.Height());
	
	CWnd* parent = GetParent();
	if(parent)
	{
		CWnd* pparent = parent->GetParent();
		
		if(pparent)
		{
			CRect rect;
			// 
			pparent->GetWindowRect(&rect);
			
			if(strW != "0" && strH != "0")
			{
				CString slog;
				slog.Format("\r\n [7110] [%-40s]  w=[%d] h=[%d]", __FUNCTION__, __LINE__, atoi(strW) - 16, atoi(strH) - 34 );
				OutputDebugString(slog);
				//m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(atoi(strW)-6, atoi(strH)-24) );  //test size
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(atoi(strW) - 16, atoi(strH) - 34));  //test size
			}

			pparent->ShowWindow(SW_SHOW);
			ShowWindow(SW_SHOW);

			//SetTimer(4000,500,NULL);
		}
	}
	
	//GetWindowRect(&r);
	//str.Format("%d %d\n",r.Width(),r.Height());
}

void CMapDlg::DoObject(_jobObj* p )
{
	CString strTemp;

//m_slog.Format("[7710] DoObject   msg=[%d] [%s] [%s]", p->msg , p->msg == DLL_TRIGGER?"트리거":"다른거", 
//	p->msg == DLL_TRIGGER ? p->msgBuf:"");
//OutputDebugString(m_slog);
	
	switch(p->msg)
	{
	case DLL_OUB:
	case DLL_OUBx:

		{
// 			OutputDebugString("[KSJ]DLL_OUB");

			switch (p->key)  
			{
			case TRKEY_MEMO_POPUPOOP:
			case TRKEY_MEMO_INSERT:
			case TRKEY_MEMO_SEARCH:
			case TRKEY_MEMO_UPDATE:
			case TRKEY_MEMO_DELETE:
			case TRKEY_MEMO_CHECK:
			{
				CString stmp;
				stmp.Format("[memo][IB771000]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(p->key), p->len);
				OutputDebugString(stmp);

				CString text;
				WPARAM	wParam{};
				wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, p->key), HIWORD(wParam));
				text = CString((char*)(LPCTSTR)p->msgBuf, p->len);
				m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
				break;
			}
			default:
				break;
			}

			if(m_bInitSize)
			{
				m_bInitSize =FALSE;

				InitSavedSize();
			}

			if(p->key == type_content)
			{
				parsingContent((char*)(LPCTSTR)p->msgBuf, p->len);								
			}
			else if ((p->key >= type_gongsi && p->key <= type_total) || p->key == type_category || p->key == type_subtotal || p->key == type_foreigner || p->key == type_PBOpinion)	//2011.12.30 KSJ 외국인/기관 동향 추가함  
			{
				parsingTitleList(p->key ,(char*)(LPCTSTR)p->msgBuf, p->len);
									
				if(m_pTab->SetCurSel(select_tab) != 0)
				{
// 					strTemp.Format("[KSJ]DLL_OUB buf[%s] len[%d]",p->msgBuf, p->len);
// 					OutputDebugString(strTemp);
					selectTab(select_tab);
				}
			}
			else if(p->key == type_symbol)
			{
// 				strTemp.Format("[KSJ]DLL_OUB buf[%s] len[%d]",p->msgBuf, p->len);
// 				OutputDebugString(strTemp);
				parsingSymbol((char*)(LPCTSTR)p->msgBuf, p->len);				
			}
			else if(p->key >= type_news_count_day && p->key <= type_news_count_month )						
				parsingNewsCount((char*)(LPCTSTR)p->msgBuf, p->len );								
		}
		break;
		//2012.01.19 KSJ Alertx 추가
	case DLL_ALERTx:
		{
			if( IsAddAlertx() )
			{
				parsingAlertx(p->alertR);			
			}
		}
	case DLL_NOTICE:	break;
	case DLL_SETPAL:	changeColors(); 	break;
	case DLL_SELECT:	break;
	case DLL_DOMINO:
		{
			//2012.05.03 KSJ 도미노가 들어왔을때의 시간을 저장한다.
			m_bDomino = true;

			m_timeDomino = CTime::GetCurrentTime();
			// KSJ

			bool bMenuNews = false;

			CString szDomino = p->msgBuf;
			CString strDomino = szDomino;

// 			OutputDebugString("[KSJ]DLL_DOMINO " + szDomino);
// 			GetAxVariant(codeCC, strDomino);

			while (!szDomino.IsEmpty())
			{
				CString str = Parser(szDomino, "\n"), sym = Parser(str, "\t");
				if( sym.CompareNoCase("1301") == 0)
				{
					m_szCode = str;
					m_strDominoCode = str;
				}
				else if( sym.CompareNoCase("NULL") == 0 )	 // 널값이 있는건 메뉴에서 호출한것!
				{ 
					bMenuNews = true;
					m_szCode = "";
				}
				else if (sym.CompareNoCase("DATE") == 0)
				{
					SYSTEMTIME tm;
					m_TimeCtrl.GetTime(&tm);
					tm.wYear = atoi(str.Left(4));
					tm.wMonth = atoi(str.Mid(4, 2));
					tm.wDay = atoi(str.Mid(6));
					m_TimeCtrl.SetTime(&tm);
					m_TimeCtrl.GetTime(&tm);
				}
			}
			if (!bMenuNews /*&& !m_bTotSearch*/)
			{
				if (m_code)	
				{
					m_code->SetEditData(m_szCode);
					m_slog.Format("2_SetEditData_[%s]", m_szCode);
					OutputDebugString(m_slog);
				}
				m_bTotSearch = false;
				if (m_bTotSearch)
				{
					m_pRadioTot->SetCheck(TRUE);
					m_pRadioSel->SetCheck(FALSE);
				}
				else
				{	m_pRadioTot->SetCheck(FALSE);
				m_pRadioSel->SetCheck(TRUE);
				}
				
				m_btnJM.EnableWindow(true);
				if (m_symbol)	
					m_symbol->EnableWindow(true);

				RequestTitleList();
			}
		}
		break;
	case DLL_SETFONT:	break;
	case DLL_TRIGGER:
		{
// 			strTemp = p->msgBuf;
// 			OutputDebugString("[KSJ]DLL_TRIGGER " + strTemp);

			if (m_bTotSearch)	// 종목 선택 라디오가 선택 되어 있을 때만 트리거를 받는다 
				break;

			// 2012.05.03 KSJ 트리거가 들어왔을때의 시간을 저장하고 시간 간격이 1초 이하이면 트리거를 무시한다.
			if(m_bDomino)
			{
				m_bDomino = false;

				m_timeTrigger = CTime::GetCurrentTime();
				
				const CTimeSpan timeDiff = m_timeTrigger - m_timeDomino;
				if (timeDiff.GetTotalSeconds() <= 1)
				
				{
m_slog.Format("[7710] DoObject break!!!!  time= [%I64d]", timeDiff.GetTotalSeconds());
				//	m_slog.Format("[7710] DoObject break!!!! ]");
OutputDebugString(m_slog);
					break;
				}
			}
			// KSJ

			m_bTrigger = true;

			CString tmpS = p->msgBuf; 

			CString	sSymbol;
			sSymbol = Parser(tmpS, "\t");



			if (m_bJmyd && sSymbol =="1301" && tmpS.GetLength() == 6)
			{				
				const int result = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)tmpS);
			

//m_slog.Format("[7710] DoObject  m_bJmydt= [%d],result=[%d]  sSymbol=[%s] tmpS=[%s][%d]", m_bJmyd, result,sSymbol, tmpS, tmpS.GetLength());
//OutputDebugString(m_slog);

			

				if(result > -1)
				{
					if(m_szCode != tmpS)
					{
						if (m_szCode.CollateNoCase(tmpS))
						{
							m_szCode = tmpS;
							if (m_code)
							{
								m_code->SetEditData(m_szCode);
								m_code->SetHistory(GetAxVariant(historyCC, _T("1301")));
								m_slog.Format("3_SetEditData_[%s]", m_szCode);
								OutputDebugString(m_slog);
							}
							m_bTriggerTR = true;
							OnRadioSel();
						}
					}
				}
			}
			else if (!sSymbol.CompareNoCase("DATE"))
			{
				//OutputDebugString(sSymbol+" @@@@@@\n");
				SYSTEMTIME tm;
				m_TimeCtrl.GetTime(&tm); 
				tm.wYear = atoi(tmpS.Left(4));
				tm.wMonth = atoi(tmpS.Mid(4, 2));
				tm.wDay = atoi(tmpS.Mid(6));
				m_TimeCtrl.SetTime(&tm);
				m_TimeCtrl.GetTime(&tm);
				OnRadioSel();
			}

			m_bTrigger = false;			
		}		

		break;
	}
}

LONG CMapDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	GetObject( wParam, lParam );	
	return 0;	
}

BOOL CMapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CoInitialize(NULL);

	CheckUserPath();	

	m_clSearchEdit.m_pParent = this;
	GetAxVariant(titleCC, "[7710]종합시황");
	m_clrLineColor[0] = GetAxColor(131);
	m_clrLineColor[1] = GetAxColor(132);

	m_szRootDir = GetAxVariant(homeCC);
	m_szUser = GetAxVariant(nameCC);
	m_pFont = GetAxFont(8,false,"굴림체");

	//Microsoft Edge WebView2 런타임
	UploadEdgeEnv("Microsoft Edge WebView2 런타임");
	m_bUsingEdgeBrowser = IsProgramInstalled("Microsoft Edge WebView2 런타임");
	
	m_sFile.Format("%s/%s/%s/%s", m_szRootDir, USRDIR, m_szUser, NEWS_CHECK_FILE);
	
	m_clrText = GetAxColor(69);
	m_clrBorder = GetAxColor(65);
	m_br.CreateSolidBrush(m_clrBorder);
	
	SetBrush(GetAxColor(BACKINDEXCOLOR));
	
	initListCtrl();
	initControl();
	
	initSymbol();
	
	LoadScreenView();

	InitWnd();

	if(!m_nView) 
	{
		m_TreeWnd.ShowWindow(SW_HIDE);
		m_btnLeft.ShowWindow(SW_SHOW);
		m_btnRight.ShowWindow(SW_HIDE);
		m_TreeWnd.ShowWindow(SW_HIDE);
		/*GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE)
		;
		GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_SEARCH_SET)->ShowWindow(SW_HIDE);	*/
	}

	loadSearchWordList();		//검색단어리스트 로딩

	if(m_pRadioTot->GetCheck() != TRUE)
	{
		if (m_szCode != "")		
		{
			sendSymbol(m_szCode);
		}
	}

	m_bmpBorder.LoadBitmap( IDB_BITMAP8 );

	UpdateData(false);
	
	OnRadioTot();

	CString strINI(_T(""));
	CString strSise;
	char readB[256]{};
	int readL = 0;
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);

	readL = GetPrivateProfileString("IB771000", "SISE", "1", readB, sizeof(readB), strINI);
	strSise = CString(readB, readL);

	m_btnSise.SetCheck(atoi(strSise));

	OnCheckSise();
	
// 	int nMode = GetPrivateProfileInt("IB771000", "MODE", 0, strINI);
// 	
// 	if(nMode == 1)
// 	{
// 		m_bColShow = FALSE;
// 		
// 		OnButtonViewmode();
// 	}
	SetTimer(4000,500,NULL);

	SetTimer( DO_INNER_JOB, 100, NULL );  //test timer  500->100

	
	return TRUE;
}

void CMapDlg::InitWnd()
{
	m_btnLeft.ShowWindow(SW_HIDE);
	m_btnRight.ShowWindow(SW_SHOW);

	CRect rcMain;
	GetClientRect(rcMain);
	
	int iw = NEWS_WIDTH+TREE_WIDTH+GAPW;
	if(!m_nView) iw -= TREE_WIDTH-GAPW;
	
}


BOOL CMapDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMapDlg::OnSizing(UINT nSide, LPRECT lpRect)
{
	CRect rc;
	GetClientRect(&rc);
	lpRect->left = rc.left;
	lpRect->right = rc.right;
}

HBRUSH CMapDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clrText);

	if(m_pStatic && pWnd->GetSafeHwnd() == m_pStatic.GetSafeHwnd())		
	{
		return (HBRUSH)m_hStaticClr;
	}

	if(m_btnSise && pWnd->GetSafeHwnd() == m_btnSise.GetSafeHwnd())
	{
		return (HBRUSH)m_hStaticClr;
	}

	if (nCtlColor == CTLCOLOR_STATIC ||
		nCtlColor == IDC_CHECK2      ||
		pWnd->GetSafeHwnd() == m_TimeCtrl.GetSafeHwnd() )
		return m_hRoundColor;

	if (m_code)
	{
		if (m_code->m_hWnd == pWnd->GetSafeHwnd())
			return (HBRUSH)m_brBk;
	}

	return hbr;
}

void CMapDlg::OnPaint() 
{
	CPaintDC dc(this);

	const int nGap = 5;
	CRect rcRound;
	CRect rc, toolrect;
	GetClientRect(&rc);
	DrawBkgrd(&dc,rc);

	CRect rectOrg;

	rectOrg = rc;

	CRect rcBorder;
	rcBorder.top = rc.bottom + 2;
	rcBorder.bottom = rcBorder.top+nGap;
	rcBorder.left = rc.left;
	rcBorder.right = rc.right;

	m_List.GetWindowRect(&rc);
	ScreenToClient(&rc);

	toolrect.left = rc.left - 6;
	toolrect.right = rc.right + 7;
	toolrect.top = rc.top - nGap; 
	toolrect.bottom = rc.bottom + nGap; 
	dc.FillSolidRect(toolrect, GetAxColor(181)); 
	DrawRoundBitmap(&dc, toolrect, ROUND_PANEL);

	const int nContentTop = toolrect.top;

	toolrect.left  = 1;
	//toolrect.right = rc.right + 7;
	toolrect.right = rectOrg.right - 1;
	toolrect.top = nGap; 
	toolrect.bottom = toolrect.top + 30; 
	
	dc.FillSolidRect(toolrect, m_clrRoundColor); //toolbar rect
	DrawRoundBitmap(&dc, toolrect, ROUND_PANEL);

	if(m_pMapWnd2)
	{
		m_pTab->GetWindowRect(&rc);
		ScreenToClient(&rc);
		rcRound.top = rc.top - nGap;
		rcRound.left = rc.left-1;

		m_pMapWnd2->GetWindowRect(&rc);
		ScreenToClient(&rc);
		rcRound.bottom = rc.bottom;
		rcRound.right = rc.right;
		rcRound.InflateRect(5,5);
		rcRound.top += 5;

		if(m_bColShow)
		{
			rcRound.top = nContentTop;
		}

		dc.FillSolidRect(rcRound, GetAxColor(181));
		DrawRoundBitmap(&dc, rcRound, ROUND_PANEL);
	}
	
	if (m_TreeWnd.IsWindowVisible())
	{
		CRect rc = m_rcBorderTree;
		if(m_bTwoToolBar) rc.top = TOOLHEIGHT * 2;
		dc.Draw3dRect(rc, RGB(155, 155, 155), RGB(155, 155, 155));
	}

	if(m_nView)
	{
		m_TreeWnd.GetWindowRect(rc);
		ScreenToClient(rc);
		rc.InflateRect(nGap,nGap);
		dc.FillSolidRect(rc, GetAxColor(181)); 
		DrawRoundBitmap(&dc, rc, ROUND_PANEL);
	}

	dc.Draw3dRect( rcBorder, RGB(155, 155, 155), RGB(155, 155, 155));

#ifndef _DEBUG
	dc.DeleteDC();	
#endif

	//m_pMapWnd3->Invalidate();
}

void CMapDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize (nType, cx, cy);

	if (!m_pBrowser || !m_pMapWnd1 || !m_pMapWnd2 || !m_pOptionWnd)
		return;
	//IB771000 = 758 490 Fixed	        ; news
	
	//m_nWidth  = cx;
	//m_nHeight = cy;

	if(m_bColShow)
	{
		ResizeTransformControl();
	}
	else
	{
		ResizeControl();
	}
}

void CMapDlg::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == DO_INNER_JOB )
	{
		WorkObject();
	}
	else if(nIDEvent == 811)
	{
		KillTimer(811);

		m_List.SetFocus();
	}
	else if(nIDEvent == 4000)
	{
		KillTimer(4000);

		CString strINI(_T(""));
		
		strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
		
		const int nMode = GetPrivateProfileInt("IB771000", "MODE", 0, strINI);
		
		if(nMode == 1)
		{
			m_bColShow = FALSE;
			
			OnButtonViewmode();
		}
	}
	else if (nIDEvent == TM_HTMLFILE)
	{
		KillTimer(TM_HTMLFILE);
		CString strEdgeUrl;
		CFileFind find;
		BOOL bfind{};
		strEdgeUrl = m_szFileName;
		if (find.FindFile(m_szFileName)) 
		{
			strEdgeUrl = ExtractHangulCharactersAndURLIncode(m_szFileName);
			strEdgeUrl = "file:///" + strEdgeUrl;
			std::string message = strEdgeUrl;
			std::wstring wnewsurl;
			wnewsurl.assign(message.begin(), message.end());
			browserEdge->Init(wnewsurl);
		}
	}
	else if (nIDEvent == TM_EDGECONTROLRESIZE)
	{
		if (browserEdge && browserEdge->IsWindowVisible())
		{
			KillTimer(TM_EDGECONTROLRESIZE);
			browserEdge->MoveWindow(m_rc);
		}
	}
	else if (nIDEvent == 1301)
	{

		KillTimer(1301);
		m_jobList.AddTail(m_SetTime_job);
	
		m_bADDING = FALSE;
		m_section.Unlock();
	}

	if (nIDEvent == DO_INNER_JOB)
	{
		WorkObject();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CMapDlg::WorkObject( void )
{
#ifndef DF_WHILE
	KillTimer(DO_INNER_JOB);
#endif
	m_section.Lock();	

	POSITION pos = m_jobList.GetHeadPosition();

bool bshow = false;
if (pos)
{
	bshow = true;
	m_slog.Format("[7710] WorkObject      ---------- START<<%d>>-----------       [%x] m_bADDING=[%d]", 
		m_jobList.GetCount(), this, m_bADDING);
	OutputDebugString(m_slog);
}

#ifndef DF_WHILE
	while( pos )
	{
		_jobObj obj = m_jobList.GetNext( pos );

		switch (obj.msg)
		{
			case DLL_OUB:
				m_slog.Format("[7710] WorkObject =[DLL_OUB] size = [%d]", m_jobList.GetSize()); break;
			case DLL_INB:
				m_slog.Format("[7710] WorkObject =[DLL_INB] size = [%d]", m_jobList.GetSize()); break;
			case DLL_ALERT:
				m_slog.Format("[7710] WorkObject =[DLL_ALERT] size = [%d]", m_jobList.GetSize()); break;
			case DLL_TRIGGER:
				m_slog.Format("[7710] WorkObject =[DLL_TRIGGER] size = [%d][%s]", m_jobList.GetSize(), obj.msgBuf); break;
			case DLL_DOMINO:
				m_slog.Format("[7710] WorkObject =[DLL_DOMINO] size = [%d][%s]", m_jobList.GetSize(), obj.msgBuf); break;
			case DLL_NOTICE:
				m_slog.Format("[7710] WorkObject =[DLL_NOTICE] size = [%d]", m_jobList.GetSize()); break;
			default:
				m_slog.Format("[7710] WorkObject =[%d] size =[%d]", obj.msg, m_jobList.GetSize());
				break;
		}
		OutputDebugString(m_slog);


		DoObject( &obj );
		m_slog.Format("[7710]size =[%d]", m_jobList.GetSize());
		OutputDebugString(m_slog);
	}
	m_jobList.RemoveAll();
#else
	if (!m_bADDING)
	{
		int isize = m_jobList.GetCount();
		POSITION tpos = nullptr;
		if (isize > 5)
			isize = 5;

	/*	if(isize >5)
			isize = 5;
		else if (isize > 10)
			isize = 10;
		else if (isize > 20)
			isize = 20;
		else if (isize > 30)
			isize = 30;
		else if (isize > 40)
			isize = 40;*/


		for (int ii = 0; ii < isize; ii++)
		{
			tpos = pos;
			_jobObj obj = m_jobList.GetNext(pos);

			DoObject(&obj);
			m_jobList.RemoveAt(tpos);
			m_slog.Format("[7710] !!!!!!remove[%x] -----------   ",this);
			//OutputDebugString(m_slog);

		}
	}

#endif
	
if (bshow)
{
	m_slog.Format("[7710] WorkObjec         -----------END<<%d>>-----------   [%x][%d] ", 
		m_jobList.GetCount(), this, m_bADDING);
	OutputDebugString(m_slog);
}

	m_section.Unlock();
#ifndef DF_WHILE
	SetTimer(DO_INNER_JOB,100,NULL);
#endif
}

BOOL CMapDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	const int nSel = m_pTab->GetCurSel();
	if(m_bColShow)
	{
		CString s;
		s.Format("%d / %d / %d / %d\n",m_rcBorder.top,m_rcBorder.left,m_rcBorder.bottom,m_rcBorder.right);
		//OutputDebugString(s);

		if (m_rcBorder.PtInRect(pt))
		{
// 			s.Format("%d , %d\n",pt.x,pt.y);
// 			OutputDebugString(s);
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
			return TRUE;
		}
	}
	else
	{
		if ((nSel != 1 && nSel != 2) && m_rcBorder.PtInRect(pt))
		{
			SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
			return TRUE;
		}
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


void CMapDlg::OnDestroy() 
{
	KillTimer( DO_INNER_JOB );
	/*while( m_jobList.GetCount() != 0 )
	{
		Sleep(500);
	}*/
	m_section.Lock();

	SaveScrapNews();

	emptyFile();

	CString strDir;
	strDir.Format("%s\\%s", m_szRootDir, "User"); 

	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	
	CString		allfile, filename;
	allfile.Format("%s\\*.html", strDir);
	hFind = FindFirstFile(allfile, &FindFileData);
	
	while (hFind != INVALID_HANDLE_VALUE)
	{
		filename.Format("%s\\%s", strDir, FindFileData.cFileName);

		if(filename.Find("html") > -1)
			::DeleteFile(filename);
		if (!FindNextFile(hFind, &FindFileData))	break;
	}
	
	SaveScreenView();		// 화면 와이드 여부 설정

	if (m_pStream) { m_pStream->Release(); m_pStream = NULL; }
	if (m_hGlobal) { GlobalFree(m_hGlobal); m_hGlobal = NULL; }

	if (m_pTab)
	{
		if (m_pTab->m_hWnd && ::IsWindow(m_pTab->m_hWnd))
			m_pTab->DestroyWindow();
		delete m_pTab;	m_pTab = NULL;
	}

	if (m_pMapWnd1)
	{
		if (m_pMapWnd1->m_hWnd && ::IsWindow(m_pMapWnd1->m_hWnd))
			m_pMapWnd1->DestroyWindow();
		delete m_pMapWnd1;	m_pMapWnd1 = NULL;
	}

	if (m_pMapWnd2)
	{
		if (m_pMapWnd2->m_hWnd && ::IsWindow(m_pMapWnd2->m_hWnd))
			m_pMapWnd2->DestroyWindow();
		delete m_pMapWnd2;	m_pMapWnd2 = NULL;
	}

	if (m_pMapWnd3)
	{
		m_pMapWnd3->closeMap();

		if (m_pMapWnd3->m_hWnd && ::IsWindow(m_pMapWnd3->m_hWnd))
			m_pMapWnd3->DestroyWindow();
		delete m_pMapWnd3;	m_pMapWnd3 = NULL;
	}
	
	if (m_pFilterWnd)
	{
		if (m_pFilterWnd->m_hWnd && ::IsWindow(m_pFilterWnd->m_hWnd))
			m_pFilterWnd->DestroyWindow();
	//	delete m_pFilterWnd;	
	//	m_pFilterWnd = NULL;
	}
	///브라우저 폰트사이즈 저장
	//
	CString strINI(_T(""));

	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	
	CString strVal(_T(""));

	strVal.Format("%d", m_pFontSize.GetCurSel());
	WritePrivateProfileString("NEWSXX", "fontsize", (LPCTSTR)strVal, (LPCTSTR)strINI);
	//////////////////////////

	CoUninitialize();
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);


	CRect rect;

	CWnd* parent = GetParent();
	if(parent)
	{
		CWnd* pparent = parent->GetParent();
		
		if(pparent)
		{
			// 
			pparent->GetWindowRect(&rect);
			
// 			rect.right = rect.left + 890;
// 			rect.bottom = rect.top + 610;
// 			
// 			pparent->MoveWindow(rect);
		}
	}

	//GetWindowRect(&rect);
	
	//2013.01.28 KSJ 최소화 하고 닫으면 width가 160, height가 25, top가 -25가 된다.
	if(rect.Width() > 200)
		WriteConfigFileInt(strINI, "IB771000", "WIDTH", rect.Width());

	if(rect.Height() > 100)
		WriteConfigFileInt(strINI, "IB771000", "HEIGHT", rect.Height());

	CString slog;
	slog.Format("\r\n [7110] [%-40s]  w=[%d] h=[%d]", __FUNCTION__, __LINE__, rect.Width(), rect.Height());
	OutputDebugString(slog);
	
	if(rect.top > 0)
		WriteConfigFileInt(strINI, "IB771000", "TOP", rect.top);

	//2013.01.28 KSJ End
	WriteConfigFileInt(strINI, "IB771000", "LEFT", rect.left);

	WriteConfigFileInt(strINI, "IB771000", "MODE", m_bColShow);

	CDialog::OnDestroy();
}


void CMapDlg::OnClose() 
{
	//DestroyWindow();
	CDialog::OnClose();
}

void CMapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bColShow)
	{
		if (m_rcBorder.PtInRect(point))
		{
			m_XMovepos = m_Xpos = point.x;
			SetCapture();
			ClipCursor(getClipRect());
			drawMove(m_rcBorder);
			m_bCapture = true;
		}
	}
	else
	{
		const int	nSel = m_pTab->GetCurSel();
		if ((nSel != 1 && nSel != 2) && m_rcBorder.PtInRect(point))
		{
			m_YMovepos = m_Ypos = point.y;
			SetCapture();
			ClipCursor(getClipRect());
			drawMove(m_rcBorder);
			m_bCapture = true;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CMapDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bCapture)
	{
		if(m_bColShow)
		{
			if (m_Xpos >= 0)
			{
				drawMove(m_rcBorder);
				m_rcBorder.OffsetRect((point.x - m_XMovepos), 0);
				drawMove(m_rcBorder);
				m_XMovepos = point.x;
			}
		}
		else
		{
			const int	nSel = m_pTab->GetCurSel();
			if ((nSel != 1 && nSel != 2) && m_Ypos >= 0)
			{
				drawMove(m_rcBorder);
				m_rcBorder.OffsetRect(0, (point.y - m_YMovepos));
				drawMove(m_rcBorder);
				m_YMovepos = point.y;
			}
		}
	}
   
	CDialog::OnMouseMove(nFlags, point);
}

void CMapDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bCapture)
	{
		ReleaseCapture();
		ClipCursor(NULL);
	}
	
	if(m_bColShow)
	{
		if (m_rcBorder.PtInRect(point) && m_Xpos >= 0)
		{
			drawMove(m_rcBorder);
			
			ReSizeSplitTransform();
			m_Xpos = -1;
			m_XMovepos = -1;
			
			CRect	rect;
			GetClientRect(rect);
			m_nHeight = rect.Height();
			m_List.GetWindowRect(rect);
		}
	}
	else
	{
		const int	nSel = m_pTab->GetCurSel();
 		if ((nSel != 1 && nSel != 2) && m_Ypos >= 0)
		{
			drawMove(m_rcBorder);

			ReSizeSplit();
			m_Ypos = -1;
			m_YMovepos = -1;

			CRect	rect;
			GetClientRect(rect);
			m_nHeight = rect.Height();
			m_List.GetWindowRect(rect);
		}
	}

	m_bCapture = false;
	CDialog::OnLButtonUp(nFlags, point);
}

void CMapDlg::ReSizeSplit()
{
	CRect rc, rc1;
	GetClientRect(&rc);

	m_List.GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	rc1.bottom = m_YMovepos - GAP10;
	m_List.MoveWindow(rc1);

	m_rcBorder.top = rc1.bottom + 5;
	m_rcBorder.bottom = m_rcBorder.top + 5;

	int itabh = 0, ih{};
	if (m_pTab)
	{
		m_pTab->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		ih = rc1.Height();

		rc1.top = m_YMovepos + GAPW + 3;
		itabh = rc1.bottom = rc1.top + ih;
		m_pTab->MoveWindow(rc1);
	}

	if (m_pBrowser)
	{
		m_pMapWnd1->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		rc1.top = itabh;
		rc1.bottom = rc.bottom - 8;

		if (m_bCodeNews)
		{
			CRect browserRect;

			browserRect.top = rc1.top;
			browserRect.left = rc1.left;

			if (rc1.Width() > 304)
				browserRect.right = rc1.right - 304;
			else
				browserRect.right = gsl::narrow_cast<long>(rc1.Width() * 0.3);

			browserRect.bottom = rc1.bottom;

			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(browserRect);
			}
			else
				m_pBrowser->MoveWindow(browserRect);


			m_pMapWnd3->MoveWindow(browserRect.right + 1, browserRect.top, rc1.Width() - browserRect.Width(), rc1.Height(), TRUE);
		}
		else
		{
			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(rc1);
			}
			else
				m_pBrowser->MoveWindow(rc1);
		}

		m_pMapWnd1->MoveWindow(rc1);
		m_pMapWnd2->MoveWindow(rc1);
	}

	m_btnPrint.GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	ih = rc1.Height();
	const int ibtnw = rc1.Width();

	rc1.left = rc.right - GAP10 - 54;
	rc1.right = rc1.left + 54;
	rc1.top = m_YMovepos + GAPW + GAPW + 1;
	rc1.bottom = rc1.top + ih;

	m_pFontSize.MoveWindow(rc1);

	rc1.left = rc.right - GAP10 - 80;
	rc1.right = rc1.left + 24;
	rc1.top = m_YMovepos + GAPW + GAPW + 5;
	rc1.bottom = rc1.top + ih - 2;

	m_pStatic.MoveWindow(rc1);

	CRect rcBtn;
	m_btnPreview.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	int iWidth = rcBtn.Width() - 100;

	rc1.top = m_YMovepos + GAPW + GAPW + 1;
	rc1.bottom = rc1.top + ih;
	rc1.left = rc.right - ibtnw - GAP10 - ADDITIONALGAP - 84;
	rc1.right = rc1.left + ibtnw + ADDITIONALGAP;
	m_btnPreview.MoveWindow(rc1);

	m_btnPrint.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - iWidth;
	m_btnPrint.MoveWindow(rc1);

	const BOOL bDraw = m_btnSell.IsWindowVisible();
	m_btnSell.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - iWidth; // m_btnSell의 너비
	m_btnSell.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - (ibtnw + 56); // 너비 100
	m_btnFavg.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - (ibtnw * 2);  // 너비 88
	m_btnMmdh.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - ibtnw;        // 너비 44
	m_btnCp.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - ibtnw;        // 너비 44
	m_btnCt.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - (ibtnw + 22); // 너비 66
	m_btnPrc.MoveWindow(rc1);
	/*
	m_btnCt.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - iWidth;

	m_btnCp.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - iWidth;

	m_btnMmdh.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - iWidth;

	m_btnFavg.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - iWidth;
	*/
	if (bDraw)
	{
		m_btnSell.MoveWindow(rc1);
		m_btnFavg.MoveWindow(rc1);
		m_btnMmdh.MoveWindow(rc1);
		m_btnCp.MoveWindow(rc1);
		m_btnCt.MoveWindow(rc1);
		m_btnPrc.MoveWindow(rc1);
	}
	else
	{
		m_btnSell.MoveWindow(rc1, FALSE);
		m_btnFavg.MoveWindow(rc1, FALSE);
		m_btnMmdh.MoveWindow(rc1, FALSE);
		m_btnCp.MoveWindow(rc1, FALSE);
		m_btnCt.MoveWindow(rc1, FALSE);
		m_btnPrc.MoveWindow(rc1, FALSE);

	}

	m_btnSise.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	iWidth = rcBtn.Width();
	
	rc1.right = rc1.left-GAPW;
	rc1.left = rc1.right-iWidth;
	m_btnSise.MoveWindow(rc1);
	
	Invalidate();
	
	m_btnSise.Invalidate();
	m_btnSell.Invalidate();
	m_btnFavg.Invalidate();
	m_btnMmdh.Invalidate();
	m_btnCp.Invalidate();
	m_btnCt.Invalidate();
	m_btnPrc.Invalidate();
	m_btnPrint.Invalidate();
	m_btnPreview.Invalidate();	
}

void CMapDlg::ReSizeSplitTransform()
{
	CRect rc, rc1;
	GetClientRect(&rc);

	m_List.GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	rc1.right = m_XMovepos - 12;

	const int iWidth = m_List.GetColumnWidth(0) + m_List.GetColumnWidth(3) + m_List.GetColumnWidth(4);

	if(iWidth < rc1.Width())
	{
		const int nDiff = rc1.Width() - iWidth;

		m_List.SetColumnWidth(4,m_List.GetColumnWidth(4)+nDiff);
	}

	m_List.MoveWindow(rc1);

	const int iW = m_List.GetColumnWidth(0) + m_List.GetColumnWidth(3) + m_List.GetColumnWidth(4);
	
	int nW = m_List.GetColumnWidth(4);
	
	nW = nW + (rc1.Width() - iW);
	
	m_List.SetColumnWidth(4,nW);

// 	m_rcBorder.top = rc1.bottom + 5;
// 	m_rcBorder.bottom = m_rcBorder.top+5;
	m_rcBorder.left =rc1.right+6;
	m_rcBorder.right = m_rcBorder.left + 3;


	int itabL = 0; const int ih{};
	if(m_pTab)
	{
		m_pTab->GetWindowRect(&rc1);
		ScreenToClient(&rc1);

		rc1.left = m_XMovepos+GAPW+3;
		itabL = rc1.left;

		m_pTab->MoveWindow(rc1);   	
	}

	if(m_pBrowser)
	{
		m_pMapWnd1->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		//rc1.top = itabh;
		//rc1.bottom = rc.bottom - 8;
		rc1.left = itabL;

		if(m_bCodeNews)
		{
			CRect browserRect;
			
			browserRect.top = rc1.top;
			browserRect.left = rc1.left;
			
			if(rc1.Width() > 304)
				browserRect.right = rc1.right - 304;
			else
				browserRect.right = gsl::narrow_cast<int>(rc1.Width() * 0.3);
			
			browserRect.bottom = rc1.bottom;
			
			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(browserRect);
			}
			else
				m_pBrowser->MoveWindow(browserRect);
			
			m_pMapWnd3->MoveWindow(browserRect.right+1,browserRect.top, rc1.Width() - browserRect.Width(), rc1.Height(),TRUE);
		}
		else
		{
			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(rc1);
			}
			else
				m_pBrowser->MoveWindow(rc1);
		}
  
		m_pMapWnd1->MoveWindow(rc1);  
		m_pMapWnd2->MoveWindow(rc1);  
	}



// 	m_btnPrint.GetWindowRect(&rc1);
// 	ScreenToClient(&rc1);
// 	ih = rc1.Height(); 
// 	int ibtnw = rc1.Width() ;
// 
// 	rc1.left = rc.right - GAP10 - 54;
//  	rc1.right = rc1.left + 54;
// 	rc1.top = m_YMovepos+GAPW+GAPW + 1;
// 	rc1.bottom = rc1.top + ih;
// 
// 	m_pFontSize.MoveWindow(rc1);
// 
//  	rc1.left = rc.right - GAP10 - 80;
//  	rc1.right = rc1.left + 24;
// 	rc1.top = m_YMovepos+GAPW+GAPW + 5;
// 	rc1.bottom = rc1.top + ih - 2;
// 
//  	m_pStatic.MoveWindow(rc1);
// 	
// 	CRect rcBtn;
// 	m_btnPreview.GetWindowRect( &rcBtn );
// 	ScreenToClient(&rcBtn);
// 	int iWidth = rcBtn.Width()-100;
// 	
// 	rc1.top = m_YMovepos+GAPW+GAPW + 1;
// 	rc1.bottom = rc1.top + ih;
// 	rc1.left = rc.right - ibtnw - GAP10 - ADDITIONALGAP - 84;
// 	rc1.right = rc1.left + ibtnw + ADDITIONALGAP;
// 	m_btnPreview.MoveWindow( rc1 );
// 	
// 	m_btnPrint.GetWindowRect( rcBtn );
// 	ScreenToClient(&rcBtn);
// 	iWidth = rcBtn.Width();
// 	
// 	rc1.right = rc1.left-GAPW;
// 	rc1.left = rc1.right-iWidth;
// 	m_btnPrint.MoveWindow( rc1 );
// 	
// 	BOOL bDraw = m_btnSell.IsWindowVisible();
// 	m_btnSell.GetWindowRect( rcBtn );
// 	ScreenToClient(&rcBtn);
// 	iWidth = rcBtn.Width();
// 	
// 	rc1.right = rc1.left-GAPW;
// 	rc1.left = rc1.right-iWidth;
// 	
// 	if( bDraw )
// 		m_btnSell.MoveWindow( rc1 );
// 	else
// 		m_btnSell.MoveWindow( rc1, FALSE );	
// 
// 	m_btnSise.GetWindowRect(rcBtn);
// 	ScreenToClient(&rcBtn);
// 	iWidth = rcBtn.Width();
// 	
// 	rc1.right = rc1.left-GAPW;
// 	rc1.left = rc1.right-iWidth;
// 	m_btnSise.MoveWindow(rc1);
	
	Invalidate();
	
	m_btnSise.Invalidate();
	m_btnSell.Invalidate();
	m_btnFavg.Invalidate();
	m_btnMmdh.Invalidate();
	m_btnCp.Invalidate();
	m_btnCt.Invalidate();
	m_btnPrc.Invalidate();
	m_btnPrint.Invalidate();
	m_btnPreview.Invalidate();	
}

void CMapDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CString str;
	CRect rc;
	GetWindowRect(&rc);
	point.x  +=rc.left;
	point.y  +=rc.top;
	str.Format("IB771000\n%d\t%d\n",point.x,point.y);
	m_pParent->SendMessage( WM_USER ,MAKEWPARAM(mouseDLL,false) ,(LPARAM)(LPCTSTR)str);
	CDialog::OnRButtonDown(nFlags, point);
}

LONG CMapDlg::OnComboEnter(WPARAM wParam,LPARAM lParam)
{
	RequestTitleList();
	return 0L;
}

void CMapDlg::OnSelendokComboSearch() 
{
	OnRadioSel();
}

LONG CMapDlg::OnListRequestNext(WPARAM wParam, LPARAM lParam)
{
	if(m_bSelectScrap) 
		return 0;
	// 체크 
	CString str;
	m_clSearchEdit.GetWindowText(str);
	if( str != "" ) 
	{
		if( IsNewsCountTR( m_nTrkey ) )
			RequestCountList(TRUE);
		else
			RequestTitleList( TRUE, TRUE ); // 키워드 란에 문자가 있으면 키워드 검색 
	}
	else
	{
		if( IsNewsCountTR( m_nTrkey ) )
			RequestCountList(TRUE);
		else
			RequestTitleList( FALSE, TRUE ); // 키워드 란에 문자가 없으면 
	}

	return 0L;
}

void CMapDlg::OnRclickListNews(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString str,szcode="",temp ="";
	CPoint pt,pt1;
	GetCursorPos( &pt);

	int index=-1;
	CRect rc;
	m_List.GetWindowRect(&rc);

	pt1 = pt;
	pt1.x -= rc.left;
	pt1.y -= rc.top ;
	index = m_List.GetSelectedItem();
	NewsInfo pinfo;
	if (m_List.GetNewsInfo(index, pinfo) )
	{
		temp = CString(pinfo.sCode,12 );
		temp.TrimRight();
		temp.Remove(0);
	}
	
	if (temp.GetLength() == 7)
		szcode = temp.Mid(1);
	else
		szcode = temp;
	
	if (szcode.GetLength() != 6)
		str.Format("IB771000\n%d\t%d\n", pt.x, pt.y);
	else
		str.Format("IB771000\n%d\t%d\n1301\t%s\n", pt.x, pt.y, szcode);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)str.operator LPCTSTR());
	*pResult = 0;
}

void CMapDlg::OnClickListNews(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_List.SetFocus();

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	

	try
	{
		if(pNMListView->iSubItem == 0)
		{
			//빈공간 스크랩 클릭시 방어
			if(pNMListView->iItem != -1)
			{
				ScrapNews(pNMListView->iItem);
				return;			
			}
		}
		
		PostMessage(WM_SEND, EVT_SENDCONTENT, pNMListView->iItem);

		if(m_btTrg.GetCheck() == BST_CHECKED)
		{
			const int itrgtype  = m_cblTrgType.GetCurSel();
			int isubsel = 0;
			if(itrgtype == 0) isubsel = 4; //제목
			if(itrgtype == 1) isubsel = 6;//종목명
			
			if(isubsel == pNMListView->iSubItem)					
				CheckCodeTrigger();			
		}
		
		if (!m_bSelectScrap && pNMListView->iItem == m_List.GetItemCount() -1)
		{
			CString str;
			m_clSearchEdit.GetWindowText( str );

			if( IsNewsCountTR(m_nTrkey) )
			{
				RequestCountList( TRUE );
			}
			else
			{
				if( str != "" )
					RequestTitleList( TRUE, TRUE );
				else
					RequestTitleList( FALSE, TRUE );			
			}			
		}
		/*else
		{
			int itrgtype  = m_cblTrgType.GetCurSel(); 
			int isubsel = 0;
			if(itrgtype == 0) isubsel = 4; //제목
			if(itrgtype == 1) isubsel = 6;//종목명
			
			if(isubsel == pNMListView->iSubItem)					
				CheckCodeTrigger();			
		}*/
		
		*pResult = 0;
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	catch (...)
	{
		AfxMessageBox("CMapDlg::OnClickListNews");
	}
}

LONG CMapDlg::OnWebBrowSize(WPARAM w,LPARAM l)
{
	return 0L;
}

LONG CMapDlg::OnWebBrowSizing(WPARAM w,LPARAM l)
{
	LPRECT rc = (LPRECT)w;
	
	CRect rcw;
	GetWindowRect(&rcw);
	const CRect rcb(rc->left,rc->top,rc->right,rc->bottom);
	
	rc->left = rcw.left-2;
	rc->right = rcw.right +2;
	rc->bottom = rcw.bottom +10;

	if( rc->top <= rcw.top + 170 )
		rc->top = rcw.top + 170;

	if( rc->top >= rcw.bottom -100 )
		rc->top = rcw.bottom -100;

	return 0L;
}

void CMapDlg::OnRadioTot() 
{
	m_bTotSearch = true;
	if (m_bTotSearch)
	{
		m_pRadioTot->SetCheck(TRUE);
		m_pRadioSel->SetCheck(FALSE);
	}
	else
	{
		m_pRadioTot->SetCheck(FALSE);
		m_pRadioSel->SetCheck(TRUE);
	}
	if (m_code)
	{
		m_code->SetEditData("");
		m_code->EnableWindow(false);
	}
	m_btnJM.EnableWindow(false); 
	if (m_symbol)
	{
		CString text = "";
		const WPARAM wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (text.GetLength() > 6) ? text.GetLength() : 0);
		m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		m_symbol->EnableWindow(false);
	}

	m_clSearchEdit.SetWindowText("");// 전체 선택시 검색어 지움

	m_szCode = "";
	RequestTitleList();
}

void CMapDlg::OnRadioSel() 
{
	m_slog.Format("[7710] ---test----OnRadioSel m_bTrigger=[%s] m_szCode =[%s]", m_code->GetEditData() , m_szCode);
	OutputDebugString(m_slog);

	m_bTotSearch = false;
	if (m_bTotSearch)
	{
		m_pRadioTot->SetCheck(TRUE);
		m_pRadioSel->SetCheck(FALSE);
	}
	else
	{	m_pRadioTot->SetCheck(FALSE);
		m_pRadioSel->SetCheck(TRUE);
	}

	m_btnJM.EnableWindow(true);
	if (m_symbol)	
		m_symbol->EnableWindow(true);

// 	CString strTemp;
// 	strTemp.Format("[KSJ]OnRadioSel1[%s]", m_szCode);
// 	OutputDebugString(strTemp);
	if (m_code)
	{
		m_code->EnableWindow(true);
		//if ( m_szCode == "")
		//{
			
			CString sHistory = m_code->GetEditData();

			if (sHistory == "")
				sHistory = m_code->m_sHistory;


			CString sCode = Parser(sHistory,"\t");
			m_slog.Format("4_SetEditData_1[%s]", sCode);
			OutputDebugString(m_slog);
			if (sCode.GetLength() >= 6)
			{
				m_code->SetEditData(sCode.Left(6));
				m_slog.Format("4_SetEditData_[%s]", sCode.Left(6));
				OutputDebugString(m_slog);
			}
			m_code->SetHistory( GetAxVariant(historyCC, _T("1301")));
			m_szCode = m_code->GetEditData();
		//}
	}

// 	strTemp.Format("[KSJ]OnRadioSel2[%s]", m_szCode);
// 	OutputDebugString(strTemp);

	m_clSearchEdit.SetWindowText("");// 종목 선택시 검색어 지움 

	RequestTitleList();
}

void CMapDlg::OnButtonJongmok() 
{
	CPoint pt(152, 50); 
	ClientToScreen(&pt);
/*
	CRect rec;
	GetDesktopWindow()->GetWindowRect(&rec);
	if(pt.x < 0)
		pt.x = rec.Width() + pt.x;
*/
	CString nameS = GetAxCode(kospiCODE,MAKELPARAM(pt.x, pt.y));
	CString temp = Parser(nameS, "\t");
	if (m_code && nameS != "")
	{
		m_szCode = temp;
		m_code->SetEditData(m_szCode);
		m_slog.Format("5_SetEditData_[%s]", m_szCode);
		OutputDebugString(m_slog);
		m_code->SetHistory( GetAxVariant(historyCC, _T("1301") ));
		OnRadioSel();
	}
	
}

void CMapDlg::OnButtonSell() 
{
	CString mapS;
	const int nItem = m_List.GetSelectedItem();
	if (nItem >=0)
	{
		NewsInfo pinfo;
		if( !m_List.GetNewsInfo(nItem, pinfo) ) return;

		CString code = "",temp="";
		temp = pinfo.sCode;	temp.TrimRight();
		temp.Remove(0);

		if (temp.GetLength() == 7)
			code = temp.Mid(1);
		else
			code = temp;

		
		if (code != "")
			mapS = "IB100100/t/s/p5/d1301\t" + code;		// updateX_20050325
		else
			mapS = "IB100100/t/s/p5";
	}
	else
		mapS = "IB100100/t/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::OnButtonPrc()
{
	CString mapS;
	const int nItem = m_List.GetSelectedItem();
	if (nItem >= 0)
	{
		NewsInfo pinfo;
		if (!m_List.GetNewsInfo(nItem, pinfo)) return;

		CString code = "", temp = "";
		temp = pinfo.sCode;	temp.TrimRight();
		temp.Remove(0);

		if (temp.GetLength() == 7)
			code = temp.Mid(1);
		else
			code = temp;


		if (code != "")
			mapS = "IB200100/t/s/p5/d1301\t" + code;		// updateX_20050325
		else
			mapS = "IB200100/t/s/p5";
	}
	else
		mapS = "IB200100/t/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::OnButtonCt()
{
	CString mapS;
	const int nItem = m_List.GetSelectedItem();
	if (nItem >= 0)
	{
		NewsInfo pinfo;
		if (!m_List.GetNewsInfo(nItem, pinfo)) return;

		CString code = "", temp = "";
		temp = pinfo.sCode;	temp.TrimRight();
		temp.Remove(0);

		if (temp.GetLength() == 7)
			code = temp.Mid(1);
		else
			code = temp;


		if (code != "")
			mapS = "IB711200/t/s/p5/d1301\t" + code;		// updateX_20050325
		else
			mapS = "IB711200/t/s/p5";
	}
	else
		mapS = "IB711200/t/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::OnButtonCompany()
{
	CString mapS;
	const int nItem = m_List.GetSelectedItem();
	if (nItem >= 0)
	{
		NewsInfo pinfo;
		if (!m_List.GetNewsInfo(nItem, pinfo)) return;

		CString code = "", temp = "";
		temp = pinfo.sCode;	temp.TrimRight();
		temp.Remove(0);

		if (temp.GetLength() == 7)
			code = temp.Mid(1);
		else
			code = temp;


		if (code != "")
			mapS = "IB202300/t/s/p5/d1301\t" + code;		// updateX_20050325
		else
			mapS = "IB202300/t/s/p5";
	}
	else
		mapS = "IB202300/t/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::OnButtonMmdh()
{
	CString mapS;
	const int nItem = m_List.GetSelectedItem();
	if (nItem >= 0)
	{
		NewsInfo pinfo;
		if (!m_List.GetNewsInfo(nItem, pinfo)) return;

		CString code = "", temp = "";
		temp = pinfo.sCode;	temp.TrimRight();
		temp.Remove(0);

		if (temp.GetLength() == 7)
			code = temp.Mid(1);
		else
			code = temp;


		if (code != "")
			mapS = "IB202400/t/s/p5/d1301\t" + code;		// updateX_20050325
		else
			mapS = "IB202400/t/s/p5";
	}
	else
		mapS = "IB202400/t/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::OnButtonFavg()
{
	CString mapS;
	const int nItem = m_List.GetSelectedItem();
	if (nItem >= 0)
	{
		NewsInfo pinfo;
		if (!m_List.GetNewsInfo(nItem, pinfo)) return;

		CString code = "", temp = "";
		temp = pinfo.sCode;	temp.TrimRight();
		temp.Remove(0);

		if (temp.GetLength() == 7)
			code = temp.Mid(1);
		else
			code = temp;


		if (code != "")
			mapS = "IB240100/t/s/p5/d1301\t" + code;		// updateX_20050325
		else
			mapS = "IB240100/t/s/p5";
	}
	else
		mapS = "IB240100/t/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::SetCountNewsCtrl( BOOL bShow )
{
	CRect rc;
	m_List.GetWindowRect( rc );

	if( bShow )
	{
		if( m_pRadioSel->GetCheck() == BST_CHECKED )
			m_code->EnableWindow( TRUE );
		else
			m_code->EnableWindow( FALSE );

		m_clSearchEdit.EnableWindow( TRUE );
		m_btnJM.EnableWindow( TRUE );
		m_pRadioSel->EnableWindow( TRUE );
		m_pRadioTot->EnableWindow( TRUE );		
		
		m_List.SetColumnWidth( 1, 0 );

		const int iWidth = rc.Width() - (18+70+36+90+69)-17;
		m_List.SetColumnWidth(4,iWidth);		
	}
	else
	{		
		m_code->EnableWindow( FALSE );

		m_clSearchEdit.EnableWindow( FALSE );
		m_btnJM.EnableWindow( FALSE );
		m_pRadioSel->EnableWindow( FALSE );
		m_pRadioTot->EnableWindow( FALSE );	

		m_List.SetColumnWidth( 1, 30 );

		const int iWidth = rc.Width() - (30+18+70+36+90+69)-17;
		m_List.SetColumnWidth(4,iWidth);		
	}
}

void CMapDlg::OnButtonPrint() 
{
	//프린터 셋업 다이얼로그 띄워서 프린팅    
// 	CPrintDialog pd(TRUE);
// 	LPDEVMODE k;
// 	pd.GetDefaults();
// 	AfxGetApp()->DoPrintDialog(&pd);
// 
// 	k = pd.GetDevMode();
// 	if(k)
// 	{
// 		k->dmOrientation = DMORIENT_LANDSCAPE;//DMORIENT_PORTRAIT;
// 		k->dmPaperSize = DMPAPER_ENV_DL;
// 	}

	CString	date;
//	GetAxVariant(guideCC ,"인쇄중입니다." );

	SAFEARRAYBOUND psabBounds{};
	SAFEARRAY* psaHeadFoot{};
	HRESULT hr = S_OK;

	HGLOBAL hG = 0;
	IStream *pStream= nullptr;
	IUnknown *pUnk = NULL;
	ULONG lWrote = 0;
	LPSTR sMem = NULL;
	
	if (!m_pBrowser) 
	{
		TRACE(_T("DoPrint: Cannot print - WebBrowser control not ready\n"));
		goto cleanup;
	}
	
	// Initialize header and footer parameters to send to ExecWB().
	psabBounds.lLbound = 0;
	psabBounds.cElements = 3;
	psaHeadFoot = SafeArrayCreate(VT_VARIANT, 1, &psabBounds);
	if (NULL == psaHeadFoot) 
	{
		goto cleanup;
	}
	VARIANT vHeadStr{}, vFootStr{}, vHeadTxtStream{};
	long rgIndices{};
	VariantInit(&vHeadStr);
	VariantInit(&vFootStr);
	VariantInit(&vHeadTxtStream);

	// Argument 1: Header
	vHeadStr.vt = VT_BSTR;
	vHeadStr.bstrVal = SysAllocString(L"");
	if (vHeadStr.bstrVal == NULL) 
	{
		goto cleanup;
	}
	
	// Argument 2: Footer
	vFootStr.vt = VT_BSTR;
	vFootStr.bstrVal = SysAllocString(L"");
	if (vFootStr.bstrVal == NULL) 
	{
		TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
		goto cleanup;
	}

	// Argument 3: IStream containing header text. Outlook and Outlook 
	if ((sMem = (LPSTR)CoTaskMemAlloc(512)) == NULL) 
	{
		TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
		goto cleanup;
	}

	date = CString(m_szCurKey,8);
	date.Insert(4,"-");	date.Insert(7,"-");
	sprintf(sMem, "<html><font face= %s><span style=\"font-size:%dpt;\"><pre></html>\0", "굴림체", 9);

	// Allocate an IStream for the LPSTR that we just created.
	hG = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, strlen(sMem));
	if (hG == NULL) {
		TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
		goto cleanup;
	}
	hr = CreateStreamOnHGlobal(hG, TRUE, &pStream);
	if (FAILED(hr)) {
		TRACE(_T("OnPrint::Failed to create stream from HGlobal: %lX\n"), hr);
		goto cleanup;
	}
	hr = pStream->Write(sMem, strlen(sMem), &lWrote);
	if (SUCCEEDED(hr)) {
	    // Set the stream back to its starting position.
		LARGE_INTEGER pos;
		pos.QuadPart = 0;
		pStream->Seek((LARGE_INTEGER)pos, STREAM_SEEK_SET, NULL);
		hr = pStream->QueryInterface(IID_IUnknown, reinterpret_cast<void **>(&pUnk));
		vHeadTxtStream.vt = VT_UNKNOWN;
		vHeadTxtStream.punkVal = pUnk;
	}

	rgIndices = 0;
	SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void *>(&vHeadStr));
	rgIndices = 1;
	SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void *>(&vHeadTxtStream));
	rgIndices = 2;
	SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void *>(&vFootStr));
		
	// SAFEARRAY must be passed ByRef or else MSHTML transforms it into NULL.
	VARIANT vArg;
	VariantInit(&vArg);
	vArg.vt = VT_ARRAY | VT_BYREF;
	vArg.parray = psaHeadFoot;

#ifndef DF_TEST
	if (!m_bUsingEdgeBrowser)
		m_pBrowser->ExecWB(OLECMDID_PRINT, OLECMDEXECOPT_PROMPTUSER, &vArg, NULL);
#endif
	
	return; //WebBrowser control will clean up the SAFEARRAY after printing.
#pragma warning(disable:6001)
#pragma warning(disable:33001)
#pragma warning(disable:4533)
cleanup:
	VariantClear(&vHeadStr);
	VariantClear(&vFootStr);
	VariantClear(&vHeadTxtStream);
#pragma warning(default:6001)
#pragma warning(default:33001)
#pragma warning(default:4533)
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

void CMapDlg::OnButtonPreview() 
{
	// TODO: Add your control notification handler code here
	CString	date;
//	GetAxVariant(guideCC ,"인쇄중입니다." );

	SAFEARRAYBOUND psabBounds{};
	SAFEARRAY* psaHeadFoot{};
	HRESULT hr = S_OK;

	HGLOBAL hG = 0;
	IStream *pStream= nullptr;
	IUnknown *pUnk = NULL;
	ULONG lWrote = 0;
	LPSTR sMem = NULL;
	
	if (!m_pBrowser) 
	{
		TRACE(_T("DoPrint: Cannot print - WebBrowser control not ready\n"));
		goto cleanup;
	}
	
	// Initialize header and footer parameters to send to ExecWB().
	psabBounds.lLbound = 0;
	psabBounds.cElements = 3;
	psaHeadFoot = SafeArrayCreate(VT_VARIANT, 1, &psabBounds);
	if (NULL == psaHeadFoot) 
	{
		goto cleanup;
	}
	VARIANT vHeadStr{}, vFootStr{}, vHeadTxtStream{};
	long rgIndices{};
	VariantInit(&vHeadStr);
	VariantInit(&vFootStr);
	VariantInit(&vHeadTxtStream);

	// Argument 1: Header
	vHeadStr.vt = VT_BSTR;
	vHeadStr.bstrVal = SysAllocString(L"");
	if (vHeadStr.bstrVal == NULL) 
	{
		goto cleanup;
	}
	
	// Argument 2: Footer
	vFootStr.vt = VT_BSTR;
	vFootStr.bstrVal = SysAllocString(L"");
	if (vFootStr.bstrVal == NULL) 
	{
		TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
		goto cleanup;
	}

	// Argument 3: IStream containing header text. Outlook and Outlook 
	if ((sMem = (LPSTR)CoTaskMemAlloc(512)) == NULL) 
	{
		TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
		goto cleanup;
	}

	date = CString(m_szCurKey,8);
	date.Insert(4,"-");	date.Insert(7,"-");
	sprintf(sMem, "<html><font face= %s><span style=\"font-size:%dpt;\"><pre></html>\0", "굴림", 9);

	// Allocate an IStream for the LPSTR that we just created.
	hG = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, strlen(sMem));
	if (hG == NULL) {
		TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
		goto cleanup;
	}
	hr = CreateStreamOnHGlobal(hG, TRUE, &pStream);
	if (FAILED(hr)) {
		TRACE(_T("OnPrint::Failed to create stream from HGlobal: %lX\n"), hr);
		goto cleanup;
	}
	hr = pStream->Write(sMem, strlen(sMem), &lWrote);
	if (SUCCEEDED(hr)) {
	    // Set the stream back to its starting position.
		LARGE_INTEGER pos;
		pos.QuadPart = 0;
		pStream->Seek((LARGE_INTEGER)pos, STREAM_SEEK_SET, NULL);
		hr = pStream->QueryInterface(IID_IUnknown, reinterpret_cast<void **>(&pUnk));
		vHeadTxtStream.vt = VT_UNKNOWN;
		vHeadTxtStream.punkVal = pUnk;
	}

	rgIndices = 0;
	SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void *>(&vHeadStr));
	rgIndices = 1;
	SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void *>(&vFootStr));
	rgIndices = 2;
	SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void *>(&vHeadTxtStream));
		
	// SAFEARRAY must be passed ByRef or else MSHTML transforms it into NULL.
	VARIANT vArg;
	VariantInit(&vArg);
	vArg.vt = VT_ARRAY | VT_BYREF;
	vArg.parray = psaHeadFoot;

#ifndef DF_TEST
	if (!m_bUsingEdgeBrowser)
		m_pBrowser->ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DONTPROMPTUSER, &vArg, NULL);
#endif
	
//	GetAxVariant(guideCC ,"출력완료." );
	return; //WebBrowser control will clean up the SAFEARRAY after printing.
#pragma warning(disable:6001)
#pragma warning(disable:33001)
#pragma warning(disable:4533)
cleanup:
	VariantClear(&vHeadStr);
	VariantClear(&vFootStr);
	VariantClear(&vHeadTxtStream);
#pragma warning(default:6001)
#pragma warning(default:33001)
#pragma warning(default:4533)
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

void CMapDlg::OnButtonSearchSet() 
{
	if(m_pFilterWnd == NULL) // 시황 설정을 모달리스 형식으로 연다
	{
	//	m_pFilterWnd = new CMarketDlg(m_szRootDir, m_szUser, m_pParent, m_pFont, this); 
		m_pFilterWnd = std::make_unique<CMarketDlg>(m_szRootDir, m_szUser, m_pParent, m_pFont, this);
		m_pFilterWnd->m_arKeyList.Copy(m_arSpecialKeyList);
		m_pFilterWnd->m_arStepColor.Copy(m_arStepColor);
		m_pFilterWnd->m_arNewsList.Copy(m_arNewsList);
		m_pFilterWnd->Create(IDD_MARKET_DLG, this);
	}
	
	if(m_pFilterWnd->GetSafeHwnd())
	{
		m_pFilterWnd->ShowWindow(SW_SHOW);
	}
}

void CMapDlg::OnButtonQuery() 
{
	if( IsNewsCountTR(m_nTrkey) )
		RequestCountList();
	else
		RequestTitleList(TRUE);
}

void CMapDlg::OnButtonNextpage() 
{
	if( IsNewsCountTR(m_nTrkey) )
	{
		RequestCountList( TRUE );
	}
	else
	{
		CString str;
		m_clSearchEdit.GetWindowText( str );
		if( str != "" )
			RequestTitleList( TRUE, TRUE );
		else
			RequestTitleList( FALSE, TRUE ); 
	}	
}

void CMapDlg::ShowTreeMode()
{
	SetFrameSize();
	if(m_nView)
	{
		m_btnLeft.ShowWindow(SW_HIDE);
		m_btnRight.ShowWindow(SW_SHOW);
		m_TreeWnd.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_SEARCH_SET)->ShowWindow(SW_SHOW);	
	}
	else
	{
		m_btnLeft.ShowWindow(SW_SHOW);
		m_btnRight.ShowWindow(SW_HIDE);
		m_TreeWnd.ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BUTTON_SEARCH_SET)->ShowWindow(SW_HIDE);	
	}

	if(m_bColShow)
	{
		ResizeTransformControl(TRUE);
	}
	else
	{
		ResizeControl();
	}
}

BOOL CMapDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (LOWORD(wParam))
	{
	case IDC_BT_LEFT:
	case IDC_BT_RIGHT:
		if(m_nView == 0)
			m_nView = 1;
		else
			m_nView = 0;
		ShowTreeMode();

		if(m_bColShow)
		{
			ResizeTransformControl();
		}
		break;
	case IDC_BT_CONFIG:
		if (m_TreeWnd.IsWindowVisible())
		{
			m_bOptCheck = !m_bOptCheck;
			if (m_bOptCheck)
			{
				if (m_bDaetoo)
					m_nOptHeight = SIZE_BUTTONS + OPTION_WND_HEIGHT;
				else
					m_nOptHeight = SIZE_BUTTONS + OPTION_WND_HEIGHT - 100;
			}
			else
				m_nOptHeight = SIZE_BUTTONS;
			
			SendMessage(WM_SIZE, 0, 0);
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CMapDlg::OnSelchangeOrdTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;	
	const int	nSel = m_pTab->GetCurSel();
	select_tab = nSel;
	selectTab(nSel);	
}

void CMapDlg::OnCloseupDatetimepicker(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_TimeCtrl.SetFocus();
	m_comboSearch.Invalidate();

	if( IsNewsCountTR( m_nTrkey ) )
		RequestCountList();
	else
		RequestTitleList();

	*pResult = 0;
}
//===========================================================================================================
CString CMapDlg::GetAxVariant(int cmd, CString data)
{
	if( !m_pParent )
		return "";

	char* dta = (char*)m_pParent->SendMessage(WM_USER, 
			MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);

// 	CString strTemp;
// 	strTemp.Format("cmd[%d], data[%s]", cmd, data);
// 	OutputDebugString(strTemp);

	if ((long)dta > 1)	return dta;
	else			return "";
}

CString CMapDlg::GetAxCode(int cmd,LPARAM lParam)
{
	if( !m_pParent )
		return "";

	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(cmd, NULL)), lParam);
	if ((long)dta > 1)	return dta;
	else			return "";
}

CFont* CMapDlg::GetAxFont(int size ,bool bBold,CString str)
{
	_fontR fontR;
	memset(&fontR,0x00,sizeof(_fontR));
	fontR.name = (char*)(LPCTSTR)str;
	fontR.point =  size;
	fontR.italic = false;
	fontR.bold = bBold?FW_BOLD:FW_NORMAL;
	return (CFont*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getFONT,0), (long)&fontR);
}

COLORREF CMapDlg::GetAxColor(int iColor)
{
	if (iColor & 0x02000000)
		return iColor;
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)iColor);
}

void CMapDlg::CheckTab(int nIdx)
{
	CString sCode;

	sCode = Trim( m_List.GetItemText(nIdx, 6) ); // 종목명 

//	2-4. 그림 1)-4 의 "오른쪽 중간 프레임
	const int icnt = m_pTab->GetItemCount();
	if(sCode.IsEmpty() || nIdx < 0 )//일반 종목 비연동 기사일때 
	{
		m_btnSell.ShowWindow(SW_HIDE); 
		m_btnFavg.ShowWindow(SW_HIDE);
		m_btnMmdh.ShowWindow(SW_HIDE);
		m_btnCp.ShowWindow(SW_HIDE);
		m_btnCt.ShowWindow(SW_HIDE);
		m_btnPrc.ShowWindow(SW_HIDE);
		
		select_tab = 0;		//탭 선택값 0으로 초기화

		if(icnt > 1)
		{
			for(int i=icnt-1; i>0; i--)
				m_pTab->DeleteItem(i);
		}
	}
	else
	{
		CString option;

		m_btnSell.ShowWindow(SW_SHOW);
		m_btnFavg.ShowWindow(SW_SHOW);
		m_btnMmdh.ShowWindow(SW_SHOW);
		m_btnCp.ShowWindow(SW_SHOW);
		m_btnCt.ShowWindow(SW_SHOW);
		m_btnPrc.ShowWindow(SW_SHOW);


		if(icnt >= m_arTabNameList.GetSize()) 
		{			
			if (!sCode.IsEmpty())
			{
				if (sCode.GetAt(0) == 'A' || sCode.GetAt(0) == 'J')	sCode = sCode.Mid(1);			
			}
			option = "1301\t" + sCode;

m_slog.Format("[7710]checkTab Trigger_CC, size =[%d] option=[%s]", m_jobList.GetSize(), option);
OutputDebugString(m_slog);
			GetAxVariant(triggerCC, option);
			return;
		}

		// 	종목연동 기사일 경우
		m_pTab->SetMinTabWidth(1);

/*		26.01.14 - lwj 종목 연동 일 경우 tab setting 사항 버튼으로 변경. 주문버튼 부분에서 처리할 예정.
		for(int i=1; i<m_arTabNameList.GetSize(); i++)
		{
			m_pTab->InsertItem(m_pTab->GetItemCount(), m_arTabNameList[i]);
		}
*/
// 		rc1.InflateRect(0,1);
// 		m_pMapWnd1->SetWindowPos( NULL, browserRect.right+1,browserRect.top, brwRect.Width() - browserRect.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
// 		rc1.DeflateRect(0,1);
// 		m_pMapWnd1->SetWindowPos( NULL, browserRect.right+1,browserRect.top, brwRect.Width() - browserRect.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
	}
}

void CMapDlg::RequestContent(int nIdx)
{
	try
	{
		if (nIdx < 0 || nIdx >= m_List.GetItemCount())
			return;

		mid_content mid;
		memset(&mid,0x00,sizeof(mid_content));
		
		CString szTemp = "";

		NewsInfo pnew;
		if (!m_List.GetNewsInfo(nIdx, pnew))	return;

		/*int keyvL = 24;//sizeof(pnew.sKeyv);
		if (memcmp(pnew.keyv, m_szCurKey, keyvL) == 0)		//현재 같은 본문내용이 이미 출력되어있음
			return;*/
		if( pnew.sKeyv == CString(m_szCurKey))
			return;

		CheckTab(nIdx);
		szTemp = m_List.GetItemText(nIdx, 4); // 제목
		szTemp.TrimRight();	szTemp.Remove(0);

		const int keyvL = pnew.sKeyv.GetLength();
		memcpy(mid.titl, (char*)(LPCTSTR)szTemp, title_len);
		memcpy(mid.keyv, (char*)(LPCTSTR)pnew.sKeyv, keyvL);
		memcpy(m_szCurKey, (char*)(LPCTSTR)pnew.sKeyv, keyvL);
		
		sendTR("pnewsrch", &mid, sizeof(mid_content), type_content);		//2015.10.15 KSJ pibonews -> pibonew3 변경 //2017.02.06 pibonew3 -> pnewsrch
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}

void CMapDlg::initListCtrl()
{
	m_txtcolor = GetAxColor(69);
	m_listhdrcolor = GetAxColor(74);
	m_listlinecolor = GetAxColor(65);

	m_List.SetnnFont( GetAxFont(9,false,"굴림체") );

 	m_List.SetTextColor(m_txtcolor);
	m_List.SetBkColor(GetAxColor(68),GetAxColor(77));
	m_List.SetEdgeColor(GetAxColor(65));
	m_List.SetSelectColor(GetAxColor(78));
	m_List.m_HdCtrl.SetTextColor(GetAxColor(76));
	m_List.m_HdCtrl.SetBkColor(m_listhdrcolor);
	m_List.m_HdCtrl.SetEdgeColor(m_listlinecolor);

	m_List.InsertColumn(0, _T(""), LVCFMT_CENTER, 18, 0);
	m_List.InsertColumn(1, _T("순위"), LVCFMT_CENTER, 0, 1);
	m_List.InsertColumn(2, _T("날짜"), LVCFMT_CENTER, 70, 2);
    m_List.InsertColumn(3, _T("시간"), LVCFMT_CENTER, 36, 3);

    m_List.InsertColumn(4, _T("제목"), LVCFMT_LEFT, 389, 4);

	m_List.InsertColumn(5, _T("조회수"), LVCFMT_RIGHT, 0, 5);
	m_List.InsertColumn(6, _T("종목명"), LVCFMT_LEFT, 90, 6);
    m_List.InsertColumn(7, _T("정보구분"), LVCFMT_CENTER, 69, 7);
	
    m_List.InsertColumn(8, _T("분류명"), LVCFMT_LEFT, 0, 8);
	m_List.InsertColumn(9, _T("idx"), LVCFMT_RIGHT, 0, 9);			// next처리 위함
	m_List.InsertColumn(10, _T("isam"), LVCFMT_RIGHT, 0, 10);			// next처리 위함
	m_List.InsertColumn(11, _T("code"), LVCFMT_LEFT, 0, 11);		 	// next처리 위함

	m_List.ModifyStyle(0,LVS_SINGLESEL|LVS_SHOWSELALWAYS|WS_CLIPCHILDREN);
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	CFont font;
	font.CreatePointFont(130,"굴림체");
	m_List.SetFont(&font);
	font.DeleteObject();

	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_szRootDir, IMAGEDIR);

	m_bmpSave.LoadBitmap( IDB_BITMAP6 );

	HBITMAP	hbitmap1{}, hbitmap1_dn{}, hbitmap2{}, hbitmap2_dn{};
	hbitmap1 = (HBITMAP)m_bmpSave;//getBitmap(fileIMG + "저장_BTN.bmp");
	hbitmap1_dn = (HBITMAP)m_bmpSave;//getBitmap(fileIMG + "저장_BTN_dn.bmp");
	hbitmap2 = getBitmap(fileIMG + "삭제_BTN.bmp");
	hbitmap2_dn = getBitmap(fileIMG + "삭제_BTN_dn.bmp");

	m_List.SetImageType(hbitmap1, hbitmap1_dn, hbitmap2, hbitmap2_dn);
	m_List.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
//	m_iOldListWidth = 673;

//	CString streeimg = fileIMG +  "시황_TREE.bmp";
//	CBitmap*	pBitmap = (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)streeimg.operator LPCTSTR());
//	m_TreeWnd.setImage(pBitmap);
}

HBITMAP CMapDlg::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

#include "GubnConfigDlg.h"
void CMapDlg::InitTree()
{
	HTREEITEM hItem{}, hRootItem{}, hCateRootItem{}, hMediaRootItem = NULL, hGongsi{}, hForeignerRootItem = NULL, hIBKRootItem = NULL;	//2011.12.30 KSJ 외국인/기관 동향 추가

	m_arNewsList.RemoveAll();

	CString version = GetConfigFileString(m_sFile,_T("SELITEM"),_T("VERSION"),_T(""));

	if(version == "2" || version == "3")  //test
	{
		GetSelectHistory();
	}
	
	m_TreeWnd.SetParent(this);
	m_TreeWnd.SetBkColor(GetAxColor(181));

	hRootItem = m_TreeWnd.InsertGroup("전체", 2, 3, NULL);//전체
	m_arNewsList.Add( NewsList("전체", "0", 0, NULL, NULL, type_total, hRootItem));

	m_TreeWnd.SetItemState(hRootItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);

	CString sFile;
	sFile.Format("%s\\tab\\%s", m_szRootDir, NEWS_CONFIG_FILE);//root\tab\newtab
	CString strApp, strSetion, strSetion2;

	m_CATESIZEGUBN = GetConfigFileInt(sFile, _T("CATEGORY"), _T("COUNT"), 0 );

	const int inewcnt = GetConfigFileInt(sFile, _T("NEWS"), _T("COUNT"), 0 );
	if(inewcnt <= 0) 
	{
		hItem = m_TreeWnd.InsertGroup("스크랩보기", 2, 3, NULL);//전체
		m_arNewsList.Add( NewsList("스크랩보기", "0", 0, NULL, NULL, 100, hItem));
		return;
	}
	
	CString sGubn, sTitle, sRead, sKey,sType;
	CString sList[2], sList1[MAXCNT], sList2[MAXCNT];
	int icnt =0, idx = 0, ikey=1;
	HTREEITEM tmpItem{};
	CString tmp;

	for( int i=0; i<inewcnt; i++)
	{
		if(i==1) continue;		//리서치 메뉴 제거

		if(i==0)
		{
			tmpItem = hRootItem;
		}
		else
		{
			if(hForeignerRootItem == NULL)
			{
				//20011.12.30 KSJ
				hForeignerRootItem = m_TreeWnd.InsertGroup("외국인/기관 동향", 2, 3, hRootItem);//전체
				m_arNewsList.Add( NewsList("외국인/기관 동향", "0", 0, NULL, NULL, type_foreigner, hForeignerRootItem));
				//KSJ
			}

// 			if(hIBKRootItem == NULL)
// 			{
// 				hIBKRootItem = m_TreeWnd.InsertGroup("IBK리서치", 2, 3, hRootItem);
// 				m_arNewsList.Add( NewsList("IBK리서치", "0", 0, NULL, NULL, type_ibk, hIBKRootItem));
// 			}

			if(hMediaRootItem == NULL)
			{
				hMediaRootItem = m_TreeWnd.InsertGroup("언론사별", 2, 3, hRootItem);//전체
				m_arNewsList.Add( NewsList("언론사별", "0", 0, NULL, NULL, type_subtotal, hMediaRootItem));
			}

			tmpItem = hMediaRootItem;
		}
 
		icnt = 0;
		strApp.Format("NEWS_%02d", i+1);
		sGubn = GetConfigFileString(sFile, strApp, _T("GUBN"), "");
		if(sGubn.IsEmpty()) continue; 

		sTitle = GetConfigFileString(sFile, strApp, _T("NAME"), "");
		sRead = GetConfigFileString(sFile, strApp, _T("BULRU_LIST"), "");

		hItem = m_TreeWnd.InsertGroup(sTitle, 2, 3, tmpItem);

		if(i==0)
		{
			hGongsi = hItem;
		}
		else
		{
			if(m_selHistory.GetCount() == 0)
			{
				m_TreeWnd.SetCheck(hItem,TRUE);
				m_TreeWnd.m_selItem.Add(hItem);
			}
			else
			{
				if(m_selHistory.Lookup(sTitle,tmp))
				{
					m_TreeWnd.SetCheck(hItem,TRUE);
					m_TreeWnd.m_selItem.Add(hItem);
				}

CString slog; 
slog.Format("[7710] [%s] [%d] [%s]", sTitle, m_selHistory.GetCount(), version);
OutputDebugString(slog);
			//	if(m_selHistory.GetCount() != 16)  
				{
					if((sTitle == "IR GO" || sTitle == "컨슈머타임즈") && (version == "2"))   
					{
slog.Format("[7710] [%s] [%s] ",m_sFile,GetConfigFileString(m_sFile,_T("SELITEM"),_T("VERSION"),_T("")));
OutputDebugString(slog);
						m_TreeWnd.SetCheck(hItem,TRUE);
						m_TreeWnd.m_selItem.Add(hItem);
						SaveNodeSelect();
					}

					if (sTitle == "뉴스웨이" && atoi(version) <= 3)  //modi 뉴스웨이
					{
						m_TreeWnd.SetCheck(hItem, TRUE);
						m_TreeWnd.m_selItem.Add(hItem);
						SaveNodeSelect();
					}
				}
			}
		}

		if(!sRead.IsEmpty()) 
		{
			idx = 0;
			strApp = sRead;
			icnt = GetConfigFileInt(sFile, strApp, _T("COUNT"), 0);
			for(int j=0; j<icnt;j++)
			{
				strSetion.Format("%02d", j+1); 
				sRead = GetConfigFileString(sFile, strApp, strSetion, "");
				SplitString(sRead, ";", sList, 2);
			
				sList1[idx] = sList[0];//gubn
				sList2[idx++] = sList[1];//name

				HTREEITEM ttItem = m_TreeWnd.InsertChild(sList[1], 0, 1, hItem);
				
				if(i==0)
				{
					if(m_selHistory.GetCount() == 0)
					{
						m_TreeWnd.SetCheck(ttItem,TRUE);
						m_TreeWnd.m_selItem.Add(ttItem);
					}
					else
					{
						if(m_selHistory.Lookup(sList[1],tmp))
						{
							m_TreeWnd.SetCheck(ttItem,TRUE);
							m_TreeWnd.m_selItem.Add(ttItem);
						}
					}
				}

				if(i != 0)
				{
					m_TreeWnd.SetItemState(ttItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);
				}
			}
		}
//CString slog;
//slog.Format("[7710] add nuws [%s] [%s] [%d] \n", sTitle, sGubn, icnt);
//OutputDebugString(slog);
		m_arNewsList.Add(NewsList(sTitle, sGubn, icnt, sList1, sList2, ikey++, hItem));
	}

	if(m_selHistory.GetCount() == 0)
	{
		SaveNodeSelect();
	}

	m_TreeWnd.Expand(hGongsi,TVE_EXPAND);

	m_TreeWnd.Expand(hMediaRootItem,TVE_EXPAND);

	hCateRootItem = m_TreeWnd.InsertGroup("카테고리별", 2, 3, hRootItem);//전체
	m_arNewsList.Add( NewsList("카테고리별", "0", 0, NULL, NULL, type_category, hCateRootItem));	//2011.12.30 KSJ type_subtotal로 되어 있는거 카테고리로 변경함
	//m_arNewsList.Add( NewsList("카테고리별", "0", 0, NULL, NULL, type_subtotal, hCateRootItem));	//2011.12.30 KSJ 원본
	m_TreeWnd.SetItemState(hCateRootItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);	//체크박스 안나오게 하는것

	for( int a=0; a<m_CATESIZEGUBN; a++)
	{
		icnt = 0;
		strApp.Format("CATE_%02d", a+1);
		sGubn = GetConfigFileString(sFile, strApp, _T("GUBN"), "");
		if(sGubn.IsEmpty()) continue; 

		sTitle = GetConfigFileString(sFile, strApp, _T("NAME"), "");
		sKey = GetConfigFileString(sFile, strApp, _T("KEY"), "");
		sType = GetConfigFileString(sFile,strApp, _T("TYPE"),"");

		CStringArray arList;
		SplitString(sType, "^", arList);

		subg_group typegroup;

		memset(&typegroup,0,sizeof(subg_group));
		
		int n=0;
		
		typegroup.subg = atoi(sKey);

		for(int b=0;b<arList.GetSize();b++)
		{
			//OutputDebugString(arList[b]+"\n");
			CStringArray arGroup;
			SplitString(arList[b],":",arGroup);
			
			const int ngubn = atoi(arGroup[0]);

			CStringArray arSubg;
			SplitString(arGroup[1],",",arSubg);
			
			for(int c=0;c<arSubg.GetSize();c++)
			{
				//OutputDebugString(arSubg[c]+"\n");
				group ssgroup;

				ssgroup.gubn = ngubn;
				ssgroup.subg = atoi(arSubg[c]);

				typegroup.sgroup[n] = ssgroup;

				n++;
			}
		}

		hItem = m_TreeWnd.InsertChild(sTitle, 0, 1, hCateRootItem);
//CString slog;
//slog.Format("[7710] add category nuws [%s] [%s] \n", sTitle, sGubn);
//OutputDebugString(slog);
		m_arNewsList.Add( NewsList(sTitle, sGubn,type_category,sKey,TRUE, typegroup,hItem));  //카테고리 
		m_TreeWnd.SetItemState(hItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);
	}
	m_TreeWnd.Expand(hCateRootItem,TVE_EXPAND);

	m_TreeWnd.Expand(hRootItem,TVE_EXPAND);

	m_iSelectGubn = 0;
	m_iSelectSubGubn = -1;
	//사용자설정 뉴스
	ikey = 100;
	hItem = m_TreeWnd.InsertGroup("스크랩보기", 5, 5, NULL);//스크랩보기
	m_arNewsList.Add( NewsList("스크랩보기", "0", 0, NULL, NULL, type_scrap/*ikey++*/, hItem));
	m_TreeWnd.SetItemState(hItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);

	hItem = m_TreeWnd.InsertGroup("조회수상위(일별)", 4, 4, NULL);
	m_arNewsList.Add( NewsList("조회수상위(일별)","111111011110",0,NULL,NULL,type_news_count_day,hItem) );
	m_TreeWnd.SetItemState(hItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);

	hItem = m_TreeWnd.InsertGroup("조회수상위(주별)", 6, 6, NULL);
	m_arNewsList.Add( NewsList("조회수상위(주별)","111111011110",0,NULL,NULL,type_news_count_week,hItem) );
	m_TreeWnd.SetItemState(hItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);

	hItem = m_TreeWnd.InsertGroup("조회수상위(월별)", 7, 7, NULL);
	m_arNewsList.Add( NewsList("조회수상위(월별)","111111011110",0,NULL,NULL,type_news_count_month,hItem) );
	m_TreeWnd.SetItemState(hItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);

	select_hItem = hRootItem;
	
	LoadScrapNews();
	
}

void CMapDlg::GetSelectHistory()
{
	CString list = GetConfigFileString(m_sFile,_T("SELITEM"),_T("ITEMS"),_T(""));

	SplitStringMAP(list,":",m_selHistory);
}

int CMapDlg::SplitStringMAP(CString strData, CString strToken, CMapStringToString &arMapList) //
{
	if(strToken == "") return 0;
	arMapList.RemoveAll(); 
	CString strSplit, strKey = _T(""), strVal = _T("");
	int iFind = 0;

	while(1)
	{
		iFind = strData.Find(strToken); 
		if(iFind < 0 ) 
		{
			if(strData.GetLength() > 0)
			{
				arMapList.SetAt(strData, "0");
			}
			
			break;
		}

		strKey = strData.Left(iFind);
		if(!strKey.IsEmpty() ) arMapList.SetAt(strKey, "0"); 

		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}

	return arMapList.GetCount() ;
}

void CMapDlg::RequestCountList(BOOL bNext)
{
	CTime time;// = CTime::GetCurrentTime();
	m_TimeCtrl.GetTime(time);
	
	req_news_count req;

	memset( &req, ' ', sizeof(req_news_count) ); // 0 -> ' ' ?

	CString strVGBN = "00";
	CString strCODE = "999";
	CString strPAGE = "0000";
  //CString strGUBN = "111111011111";
	CString strGUBN = "11111101111111110000"; //c
	CString strSORT = "2";

	switch( m_nTrkey )
	{
	case type_news_count_day:	strVGBN = "00"; break;
	case type_news_count_week:	strVGBN = "01"; break;
	case type_news_count_month: strVGBN = "02"; break;
	}

	if( bNext )
	{
		if( m_bNextPushed )
			return;

		if( m_sPageHeader.iCurPage >= m_sPageHeader.iMaxPage )
			return;

		strPAGE.Format("%04d",m_sPageHeader.iCurPage+1);
		
		m_bNextPushed = true;
	}
	else
	{
		m_List.DeleteAllItems();
		m_List.DeleteAllNewsInfo();
	}
	
	sprintf(req.fday,"%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay() );

	sprintf(req.vgbn,"%s", strVGBN ); 
	sprintf(req.gubn,"%s", strGUBN );
	sprintf(req.subg,"%s", strCODE );
	sprintf(req.sdir,"%s", strSORT );
	sprintf(req.cpag,"%s", strPAGE );

// 	OutputDebugString("[KSJ]RequestCountList");
	sendTR("piho7711", &req, sizeof(req_news_count), m_nTrkey, false);
}

void CMapDlg::GetGugnAndSubgMask( char* szGubn, char* szSubg )
{
	if( m_TreeWnd.GetParentItem( select_hItem ) == NULL )
	{
//		memcpy( szGubn, "1111110111111", 13 ); 
		memcpy( szGubn, "11111101111111110000", 20 );  //c
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}
	//2011.12.30 KSJ
	else if(m_TreeWnd.GetItemText(select_hItem) == "공시" && m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
		// 		OutputDebugString("외국인");
		memcpy( szGubn, "01000000000000000000", 20 );  //c
		memcpy( szSubg, "999", 3 );
		
		// 전체 뉴스 config 체크 
		
		return;
	}
	else if(m_TreeWnd.GetItemText(select_hItem) == "외국인/기관 동향" && m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
		// 		OutputDebugString("외국인");
		memcpy( szGubn, "10000000000000000000", 20 );  //c
		memcpy( szSubg, "999", 3 );
		
		// 전체 뉴스 config 체크 
		
		return;
	}
	else if(m_TreeWnd.GetItemText(select_hItem) == "IBK리서치" && m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
		// 		OutputDebugString("외국인");
		memcpy( szGubn, "00000010000000000000", 20 );  //c
		memcpy( szSubg, "999", 3 );
		
		// 전체 뉴스 config 체크 
		
		return;
	}
	else if(m_TreeWnd.GetItemText(select_hItem) != "공시" && m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
		memcpy( szGubn, "10111101111111110000", 20 );  //c
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}     
	


//	memcpy( szGubn, "1000000000000", 13 );
	memcpy( szGubn, "00000000000000000000", 20 );

	CString strText = m_TreeWnd.GetItemText( select_hItem );
	HTREEITEM hParent = m_TreeWnd.GetParentItem( select_hItem ); 

	bool bNewsGubn = false;
	if(m_TreeWnd.GetRootItem() == m_TreeWnd.GetParentItem(hParent))
		bNewsGubn = true;

	for( int i = 0; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList news = m_arNewsList.GetAt(i);
		if( news.sGubnName == strText  && news.bCategory != TRUE && bNewsGubn)					// TREE TEXT 로 구분이름(뉴스사) 검색  //c
		{
			for( int k = 1; k < NEWS_SIZEGUBN+1; k++ )	// 구분배열 만듬    //뉴스 대분류 선택시 세부뉴스구분은 선택 안함
			{
				if( k == news.iGubn )
				{
			//		if( k >= 14)                //IR GO 부터
			//			szGubn[k] = '1';	    //c
			//		else
						szGubn[k-1] = '1';		//c 		
				}
			}

			memcpy( szSubg, "999", 3 );
			break;
		}			

		if( news.hTreeItem == hParent )						// 종목의 부모(뉴스사) 검색 
		{			
			for( int j = 0; j < news.ibulruCnt; j++ )
			{
				if( news.sBulruNameList[j] == strText  && news.bCategory != TRUE )		// TREE TEXT 로 종목 검색 
				{
					for( int k = 1; k < NEWS_SIZEGUBN+1; k++ ) 
					{	
						if( k == news.iGubn )	
						{								
				//			if( k >= 14)			//IR GO 부터
				//				szGubn[k] = '1';	//c
				//			else
								szGubn[k-1] = '1';	//c	
						}
					}

					memcpy( szSubg, news.sBulruGubnList[j], news.sBulruGubnList[j].GetLength() );
						break;
				}
			}		
		}
	}
}

void CMapDlg::GetCateGugnAndSubgMask( char* szGubn, char* szSubg )
{
	if( m_TreeWnd.GetParentItem( select_hItem ) == NULL )
	{
//		memcpy( szGubn, "1111110111111", 13 );
		memcpy( szGubn, "11111101111111110000", 20 );  
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}
	else if(m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
//		memcpy( szGubn, "1011110111111", 13 );
		memcpy( szGubn, "11111101111111110000", 20 );   //c
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}

	//memcpy( szGubn, "1000000000000", NEWS_SIZEGUBN );
	memcpy( szGubn, "1000000000000000    ", NEWS_SIZEGUBN );

	CString strText = m_TreeWnd.GetItemText( select_hItem );
	HTREEITEM hParent = m_TreeWnd.GetParentItem( select_hItem );
	
//	for( int i = NEWS_SIZEGUBN; i < m_arNewsList.GetSize(); i++ )   //카테고리 관련 구분자는 언론사별 리스트 구분자 16개 이후 18개부터이다
	for( int i = 18; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList news = m_arNewsList.GetAt(i);
		if( news.sGubnName == strText && news.bCategory)					// TREE TEXT 로 구분이름(뉴스사) 검색 
		{
			for( int k = 1; k < NEWS_SIZEGUBN+1; k++ )	// 구분배열 만듬   
			{				
		//		if( k >= 14)
		//			szGubn[k] = '1';	              //c
		//		else
					szGubn[k-1] = '1';		          //c
			}

			memcpy( szSubg, news.sKey, 3 );
			break;
		}
	}
}

void CMapDlg::RequestTitleList( BOOL bKeyword, BOOL bNext )
{
	if( bKeyword == FALSE )
		m_strSearchKey = "";
	else
		m_clSearchEdit.GetWindowText(m_strSearchKey);

	if( bNext )
	{
		if( m_bNextPushed )
			return;
	}

	CString	szPortfolio;
	CString szSearch;
	CString szDate;
	CString szGubn;
	CString szFlag;
	CTime time;

	CString strTemp;

	if (m_code) 
	{
		m_szCode = m_code->GetEditData();
		m_szCode.TrimLeft();	m_szCode.TrimRight();	m_szCode.Remove(0);	

// 		strTemp.Format("[KSJ]RequestTitleList in m_code[%s]", m_szCode);
// 		OutputDebugString(strTemp);
	}

// 	CString strTemp;

	mid_title req;
	memset(&req, ' ', sizeof(mid_title));

	m_TimeCtrl.GetTime(time);
	szDate = time.Format("%Y%m%d");
	memcpy( req.fday, szDate, sizeof(req.fday) );	
	
	memcpy( req.skey, m_strSearchKey, min(m_strSearchKey.GetLength(),20));	

	if( !m_bTotSearch && m_szCode.GetLength() > 0 /*&& !bKeyword*/ )	
		memcpy(req.cod2,(char*)(LPCTSTR)m_szCode , min(m_szCode.GetLength(), 6) );	

	if(m_bCategory)
	{
		GetCateGugnAndSubgMask(req.gubn,req.sugb);
	}
	else
	{
		GetGugnAndSubgMask(req.gubn,req.sugb);	
	}	

	// ----------------- grid 셋팅 --------------------- //
	memcpy(req.grid_i.vrow ,"20", 2);		// visible rows
	memcpy(req.grid_i.nrow, "1000", 4);		// # of rows

	req.grid_i.sdir[0] = '2'; // desc sort

	if( bNext ) // 다음 페이지 일 때 
	{
		req.grid_i.ikey[0] = '2'; // PgDn
		req.grid_i.gdir[0] = '2'; // add to bottom

		if( !(m_grido.xpos[0] & 0x02) ) // PgDn, ScrDn 이 가능 할 때만 			
			return;
		
		struct _page
		{
			char    cod2[6];					// 입력한 종목코드 없으면 Space
			char    fday[8];					// GRID가 가지고 있는 일자
			char    gubn[NEWS_SIZEGUBN];		// 입력한 구분자 7 바이트
			char    skey[20];					// 검색에 입력한 문자스트링
			char    isrn[10];					// 한글명 다음의 10바이트    #14
		};
		const int cnt = m_List.GetItemCount()-1;
		if (cnt < 0)	
			return;

		NewsInfo pinfo;

		if (!m_List.GetNewsInfo(cnt, pinfo) )	return;
		struct _page *page = (struct _page *)&m_szSave;			// m_szSave 와 연동 				


		//2016.08.10 KSJ isrn이 없을때 기존거 그대로 넣어준다.
		if(pinfo.sGisn.Compare("0"))
		{
			memcpy(page->isrn, (char*)(LPCTSTR)pinfo.sGisn, sizeof(page->isrn));	// isrn 이 셋팅 되어 올라 가야 여기서 부터 찾는 다. 
		}
		
 //		strTemp.Format("7710 [KSJ]RequestTitleList in pinfo.sGisn[%s] m_szSave[%s]", pinfo.sGisn, m_szSave);
	

		memcpy(req.grid_i.save, m_szSave, sizeof(req.grid_i.save) );
		memcpy(req.grid_i.page, m_grido.page, 4 );

		m_bNextPushed = true;
	}
	else
	{
		req.grid_i.ikey[0] = '1'; // PgUp ?
		req.grid_i.gdir[0] = '1'; // top

		memcpy( req.grid_i.page, "0000", 4 );
	}

	m_szDate = szDate;

	//AfxMessageBox(req.gubn[6]);
	if(req.gubn[6] == '1')
	{
		req.typ[0] = 'R';
	}

	//req.sNewGubn[0] = '1'; 
//	if(CString(req.gubn, 13) == "0000000000000" || CString(req.gubn, 13) == "1111110111111")
	if(CString(req.gubn, 20) == "00000000000000000000" || CString(req.gubn, 20) == "11111101111111110000")  //c
		memcpy(req.sNewGubn, "10000000000000000000", 20);

// 	OutputDebugString("[KSJ]szcode[" + m_szCode +"] req.cod2[" + req.cod2 + "]");
// 
// 	if(m_szCode.Compare(req.cod2))

	//AfxMessageBox(req);
m_slog.Format("[7710] -------[%s] m_bTrigger=[%d m_szCode =[%s]", __FUNCTION__, m_bTrigger, m_szCode);
OutputDebugString(m_slog);
	sendTR("pibo7710", &req, sizeof(mid_title), m_nTrkey, true);
}

// hItem 의 속한 key( sendtr_type ) 를 리턴 
int CMapDlg::SetMediaFilter(HTREEITEM hItem)
{	
	CString strText = m_TreeWnd.GetItemText( hItem );
    HTREEITEM parentItem = m_TreeWnd.GetParentItem(hItem);

	if(m_TreeWnd.GetItemText(parentItem) == "카테고리별")
	{
		for( int i = 0; i < m_arNewsList.GetSize(); i++ )
		{
			NewsList news = m_arNewsList.GetAt(i);
			
			if(news.bCategory)
			{
				if( news.hTreeItem == hItem )
				{
					return news.ikey;
				}
			}
		}
	}
	else
	{
		for( int i = 0; i < m_arNewsList.GetSize(); i++ )
		{
			NewsList news = m_arNewsList.GetAt(i);
			if( news.hTreeItem == hItem )
			{
				return news.ikey;
			}

			for( int j = 0; j < news.ibulruCnt; j++)
			{
				if( news.sBulruNameList[j] == strText )
				{
					return news.ikey;
				}
			}
		}	
	}

	return -1;	
}

bool CMapDlg::initControl()
{
	//CoInitialize(NULL);
	m_szSysGubn = GetAxVariant(systemCC);
	m_clrRoundColor = GetAxColor(TOOLINDEXCOLOR);
	m_hRoundColor = CreateSolidBrush(m_clrRoundColor);
	m_hStaticClr = CreateSolidBrush(GetAxColor(181));
	m_strPal   = GetPalette();

	// 20070212 cej
	CString file;
	file.Format("%s\\tab\\axis.ini", m_szRootDir);
	m_bRetail = (BOOL)GetPrivateProfileInt("HN_RETAIL", "RETAIL", 0, file);
	
	// 20070212 cej IB는 머니투데이 안나옴
	if(m_szSysGubn == "H" && !m_bRetail)
	{
		m_bDaetoo = FALSE;
	} 

	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_szRootDir, IMAGEDIR);

	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};
	hBITMAP = getBitmap(fileIMG + "TROPT.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "TROPT_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "TROPT_en.bmp");
	m_pFont = GetAxFont(9,false,"굴림체");

// 	m_btOption.Create("색상 및 조회 설정", CRect(0, 0, 0, SIZE_BUTTONS), this, IDC_BT_CONFIG, TRUE, TRUE);
// 	m_btOption.SetFont(m_pFont, false);
// 	m_btOption.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
// 
	if (m_pOptionWnd == NULL)		// Option 설정 Window
	{
		//m_pOptionWnd = new COptionWnd(this);
		m_pOptionWnd = std::make_unique<COptionWnd>(this);
		m_pOptionWnd->SetGubn(m_bDaetoo);

		if (m_bDaetoo)
			m_pOptionWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, TREE_WIDTH - 5, OPTION_WND_HEIGHT), this, IDC_OPTION_WND);
		else
			m_pOptionWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, TREE_WIDTH - 5, OPTION_WND_HEIGHT-100), this, IDC_OPTION_WND);

		m_pOptionWnd->Init(m_szRootDir, m_szUser);
	}
	// buttons font
	m_btnSell.SetFont(m_pFont);
	m_btnFavg.SetFont(m_pFont);
	m_btnMmdh.SetFont(m_pFont);
	m_btnCp.SetFont(m_pFont);
	m_btnCt.SetFont(m_pFont);
	m_btnPrc.SetFont(m_pFont);

	m_btnSSet.SetFont(m_pFont);

	m_btnSizeInit.SetFont(m_pFont);
	m_btnViewMode.SetFont(m_pFont);
	m_pFont = GetAxFont(9,true,"굴림체");
	m_btnQuery.SetFont(m_pFont);
	m_pFont = GetAxFont(9,false,"굴림체");
	m_btnPrint.SetFont(m_pFont);
	m_btnNext.SetFont(m_pFont);
	m_btnJM.SetFont(m_pFont);
	m_btToolPos.SetFont(m_pFont);

	// 종목 검색 button
 	CString szn, szu, szd;
	szn = m_szRootDir + "\\image\\" + "S검색.BMP";
	szu = m_szRootDir + "\\image\\" + "S검색_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "S검색_DN.BMP";
	m_btnJM.SetSkin(szn,szd,szu);

	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), szn,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CBitmap Bitmap;
	if (hBitmap)
	{
		Bitmap.Attach(hBitmap);
		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		m_bx = bm.bmWidth;
		m_by = bm.bmHeight;
		Bitmap.Detach();
	}
	m_btnJM.SetWindowPos(NULL,0,0,m_bx,m_by,SWP_NOMOVE);
	
	// 보이기 버튼
	szn = m_szRootDir + "\\image\\" + "LEFT_EX.BMP";
	szu = m_szRootDir + "\\image\\" + "LEFT_EX_en.BMP";
	szd = m_szRootDir + "\\image\\" + "LEFT_EX_dn.BMP";

	m_btnLeft.SetSkin(szn,szd,szu);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), szn,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)
	{
		Bitmap.Attach(hBitmap);
		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		m_bx = bm.bmWidth;
		m_by = bm.bmHeight;
		Bitmap.Detach();
	}
	m_btnLeft.SetWindowPos(NULL,0,0,m_bx,m_by,SWP_NOMOVE | SWP_SHOWWINDOW);
	
	// 숨기기 버튼
	szn = m_szRootDir + "\\image\\" + "RIGHT_EX.BMP";
	szu = m_szRootDir + "\\image\\" + "RIGHT_EX_en.BMP";
	szd = m_szRootDir + "\\image\\" + "RIGHT_EX_dn.BMP";
	m_btnRight.SetSkin(szn,szd,szu);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), szn,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		Bitmap.Attach(hBitmap);
		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		m_bx = bm.bmWidth;
		m_by = bm.bmHeight;
		Bitmap.Detach();
	}
	m_btnRight.SetWindowPos(NULL,0,0,m_bx,m_by,SWP_NOMOVE |SWP_HIDEWINDOW);

	szn = m_szRootDir + "\\image\\" + "DOWN_EX.BMP";
	szu = m_szRootDir + "\\image\\" + "DOWN_EX_en.BMP";
	szd = m_szRootDir + "\\image\\" + "DOWN_EX_dn.BMP";
	m_btToolPos.SetSkin(szn,szd,szu);
	m_btToolPos.SetWindowPos(NULL,0,0,m_bx,m_by,SWP_NOMOVE |SWP_SHOWWINDOW);
	//버튼 숨김
	m_btToolPos.ShowWindow(SW_HIDE);



	// Radio Button
	int ileft = 30;
	m_pRadioTot = new CButton();
	m_pRadioTot->Create("전체", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTORADIOBUTTON, CRect(ileft, 8, ileft+45, 22), this, IDC_RADIO_TOT);
	m_pRadioTot->SetFont(m_pFont);

	ileft += 45;
	m_pRadioSel = new CButton();
	m_pRadioSel->Create("", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTORADIOBUTTON, CRect(ileft, 8, ileft+13, 22), this, IDC_RADIO_SEL);
	m_pRadioSel->SetFont(m_pFont);
	ileft+= 12;
	ileft+= 5;//gap



	// 종목검색combo
	m_code = new CfxCodeCtrl(this, m_szRootDir + "\\" + "tab" + "\\");
	if (m_code->Create(this, CRect(ileft, 3, ileft+65, 23), IDC_CODECTRL))
	{
		/*
		szn = m_szRootDir + "\\image\\" + "DOWN_EX.BMP";
		szd = m_szRootDir + "\\image\\" + "DOWN_EX_dn.BMP";
		m_code->SetButtonBitmap(getBitmap(szn), getBitmap(szd));
		*/
		m_code->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		m_code->SetUnit(GU_CODE);
		m_code->SetFont(m_pFont);
	}
	else
	{
		delete m_code;		
		m_code = NULL;
	}

	m_btnJM.MoveWindow(ileft+65, 3, 19, 20, TRUE);
	

	CString text = GetAxVariant(historyCC, _T("1301"));
	if (m_code)	m_code->SetHistory(text);
	if (text.GetLength()>6)
	{
		m_szCode = text.Left(6);
		
		if (m_code)
		{
			m_code->SetEditData(m_szCode);
			m_slog.Format("6_SetEditData_[%s]", m_szCode);
			OutputDebugString(m_slog);
		}
	}

	// radio 전체:종목선택
	if (m_bTotSearch)
		m_pRadioTot->SetCheck(TRUE);
	else
		m_pRadioSel->SetCheck(TRUE);


	// 일반버튼 image
	szn = m_szRootDir + "\\image\\" + "6BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "6BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "6BTN_DN.BMP";
	m_btnPreview.SetSkin(szn,szd,szu);

	szn = m_szRootDir + "\\image\\" + "3BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "3BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "3BTN_DN.BMP";
	m_btnScrap.SetSkin(szn,szd,szu);
	m_btnScrap.ShowWindow(SW_HIDE);

	szn = m_szRootDir + "\\image\\" + "4BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "4BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "4BTN_DN.BMP";
	m_btnSSet.SetSkin(szn,szd,szu);
	m_btnSSet.SetToolTipText("시황설정");
	m_btnSSet.SetWindowText("시황설정");

	szn = m_szRootDir + "\\image\\" + "창조절좌우.BMP";
	szu = m_szRootDir + "\\image\\" + "창조절좌우_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "창조절좌우_DN.BMP";
	m_btnSizeInit.SetSkin(szn,szd,szu);
	m_btnSizeInit.SetToolTipText("기본크기로 변경");

	szn = m_szRootDir + "\\image\\" + "TURN.BMP";
	szu = m_szRootDir + "\\image\\" + "TURN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "TURN_DN.BMP";
	m_btnViewMode.SetSkin(szn,szd,szu);
	m_btnViewMode.SetToolTipText("가로/세로 보기");

	szn = m_szRootDir + "\\image\\" + "조회BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "조회BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "조회BTN_DN.BMP";
	m_btnQuery.SetSkin(szn,szd,szu);

/*
	szn = m_szRootDir + "\\image\\" + "TURN.BMP";
	szu = m_szRootDir + "\\image\\" + "TURN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "TURN_DN.BMP";
	m_btnRefresh.SetSkin(szn,szd,szu);
*/

	szn = m_szRootDir + "\\image\\" + "2BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "2BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "2BTN_DN.BMP";
	m_btnSell.SetSkin(szn,szd,szu);

	m_btnCp.SetSkin(szn, szd, szu);
	m_btnCt.SetSkin(szn, szd, szu);

	m_btnPrint.SetSkin(szn, szd, szu);
	m_btnNext.SetSkin(szn, szd, szu);

	szn = m_szRootDir + "\\image\\" + "3BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "3BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "3BTN_DN.BMP";

	m_btnPrc.SetSkin(szn, szd, szu);

	szn = m_szRootDir + "\\image\\" + "4BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "4BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "4BTN_DN.BMP";
	m_btnFavg.SetSkin(szn, szd, szu);

	szn = m_szRootDir + "\\image\\" + "5BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "5BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "5BTN_DN.BMP";
	m_btnMmdh.SetSkin(szn, szd, szu);

	
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), szn,	IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		Bitmap.Attach(hBitmap);
		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		m_bx = bm.bmWidth;
		m_by = bm.bmHeight;
		Bitmap.Detach();
	}

	m_btnQuery.SetWindowPos(&wndTop,0,0,m_bx+1,m_by,SWP_NOMOVE);

	// 검색버튼
	CRect rc;
	GetDlgItem(IDC_RADIO_SEL)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	rc.left = rc.right += 5;
	rc.bottom += 3;
	rc.right = rc.left + 30;
	m_btnQuery.GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_btnNext.SetWindowPos(&wndTop,rc.right+1,rc.top,m_bx,m_by,SWP_SHOWWINDOW);	// 다음버튼
	
	m_comboSearch.InitEdit();							// 검색
	m_comboSearch.SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	// date
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	m_TimeCtrl.SetTime(&tm);
	m_TimeCtrl.SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	// tab
	CRect	rcTab, rcc;

	m_List.GetWindowRect(rc);
	ScreenToClient(rc);
	
	rc.bottom = rc.bottom - 20;
	m_List.MoveWindow(rc);

	m_List.GetWindowRect(rc);
	ScreenToClient(rc);
	
	m_rcBorder = rc;
	m_rcBorder.top = rc.bottom + GAPW;
	rcTab.left = rc.left+5;
	m_rcBorder.bottom = rcTab.top = m_rcBorder.top + 5;

	rcTab.right = rc.right;
	GetDlgItem(IDC_ST_FRAME)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	const int ibtntop = rcTab.top;
	rcTab.bottom = rcTab.top + 26; 
//	rcTab.right = rcTab.left + 465;
	rcTab.right = rcTab.left + 500;

	m_pTab = new CfxTab();
	m_pTab->Create(rcTab, this, IDC_TAB);
	m_pTab->LoadTabImage(m_szRootDir, false);
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CLR_FORM_BACK), 
		GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CLR_FORM_BACK), GetIndexColor(CLR_FORM_BACK));
	m_pTab->SetFont(m_pFont);
	m_pTab->InsertItem(0, "본문");
	m_pTab->ChangeBKColor(m_clrRoundColor);
	
	m_pTab->SetStyle(tsIMAGE, TRUE);

	GetClientRect(&rcc);
	
#ifndef DF_TEST
	m_pBrowser = new CAxBrower();
	m_pBrowser->SetParent(m_pParent);
	rc.bottom -= 30;
	m_pBrowser->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		rc, this, IDC_HTMLVIEW);
	m_pBrowser->ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
	m_pBrowser->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pBrowser->Navigate2("about:blank");

#ifdef DF_USEEDGE
	if (m_bUsingEdgeBrowser)
	{
		m_pBrowser->ShowWindow(SW_HIDE);
		browserEdge = std::make_unique<BrowserWindowEdge>();
		CRect rect;
		GetClientRect(rect);

		if (!browserEdge->Create(nullptr, "BrowserWindowEdge", WS_CHILD | WS_VISIBLE, rect, this, 0)) {
			m_slog.Format("[edge] [%s] create failure", __FUNCTION__);
			OutputDebugString(m_slog);
			AfxMessageBox("edge create fail");
		}

		CString strEmptyPage = _T("about:blank");
		std::string message_a = strEmptyPage;
		std::wstring wurl;
		wurl.assign(message_a.begin(), message_a.end());

		browserEdge->Init(wurl);
		m_slog.Format("[edge] [%s] create success", __FUNCTION__);
		OutputDebugString(m_slog);
	}
#endif
#else
	m_pBrowser = new CContentWnd();
	m_pBrowser->SetParent(m_pParent);
	rc.bottom -= 30;
	m_pBrowser->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		rc, this, IDC_HTMLVIEW);
	m_pBrowser->ModifyStyleEx(0, WS_EX_CLIENTEDGE, 0);
	m_pBrowser->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif
	
//	m_pBrowser->SetSilent(true);

	// 현재가
	m_pMapWnd1 = new CMapWnd(m_pParent);
	m_pMapWnd1->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW);

	// 차트
	m_pMapWnd2 = new CMapWnd(m_pParent);
	m_pMapWnd2->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW2);
	m_nHeightStd = m_nHeight = rcc.Height();

	m_pMapWnd3 = new CMapWnd(m_pParent);
	m_pMapWnd3->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW3);

	m_pMapWnd3->closeMap();
	m_pMapWnd3->ChangeMap("IB771098", "");

	// 뉴스 글꼴 색상 초기화
	m_clrTitleText[0] = m_pOptionWnd->GetNewsColor(idMoneyToday);
	m_clrTitleText[1] = m_pOptionWnd->GetNewsColor(idInfoStock);
	m_clrTitleText[2] = m_pOptionWnd->GetNewsColor(idHanNews);
	m_clrTitleText[3] = m_pOptionWnd->GetNewsColor(idEDaily);
	m_clrTitleText[4] = m_pOptionWnd->GetNewsColor(idYounhap);
	m_clrTitleText[5] = m_pOptionWnd->GetNewsColor(idMaekyoung);
	m_clrTitleText[6] = m_pOptionWnd->GetNewsColor(idEToday);
	m_clrTitleText[7] = m_pOptionWnd->GetNewsColor(idNewsp);
	m_List.Invalidate(FALSE);
	
	// 본문자동 조회 초기화
	m_bAutoQuery = TRUE;//m_pOptionWnd->GetCheckContext();

	//종목연동
	m_cblTrgType.InsertString(0, "제목클릭"); 
	m_cblTrgType.InsertString(1, "종목명클릭"); 
	m_cblTrgType.SetCurSel(0); 

	//폰트 콤보 박스 설정
// 	m_pStatic = new CStatic();
// 	m_pStatic->Create("폰트", WS_CHILD | WS_VISIBLE | SS_RIGHT,
// 						rc, this, IDC_STATIC_TITLE);
	m_pStatic.SetFont(m_pFont);

// 	m_pFontSize = new CComboBox;
// 	m_pFontSize->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBS_DROPDOWNLIST, 
// 						rc, this, IDC_FONTSIZE);
	m_pFontSize.SetFont(m_pFont);

	m_pFontSize.InsertString(0,_T("8"));
	m_pFontSize.InsertString(1,_T("9"));
	m_pFontSize.InsertString(2,_T("10"));
	m_pFontSize.InsertString(3,_T("11"));
	m_pFontSize.InsertString(4,_T("12"));
	m_pFontSize.InsertString(5,_T("14"));
	m_pFontSize.InsertString(6,_T("16"));
	m_pFontSize.InsertString(7,_T("18"));
	m_pFontSize.InsertString(8,_T("20"));
	m_pFontSize.InsertString(9,_T("22"));
	m_pFontSize.InsertString(10,_T("24"));

	m_pFontSize.SetCurSel(1);

	CString strINI(_T(""));
	
	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);

	m_pFontSize.SetCurSel(GetPrivateProfileInt("NEWSXX", _T("fontsize"), 1, (LPCTSTR)strINI));
	
	// Initialize Tree
	InitTree();

	LoadCode();
	
	//CoUninitialize(); 
	return true;
}

COLORREF CMapDlg::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}


void CMapDlg::SetFrameSize()
{
	//int iw = NEWS_WIDTH+TREE_WIDTH+GAPW+120;
	//if(!m_nView) iw-= (TREE_WIDTH-2);
	CRect rect;
	GetWindowRect(&rect);

	int iw = rect.Width();

	if(!m_nView)
		iw -= (TREE_WIDTH - 2);
	else
		iw += (TREE_WIDTH + 2);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iw, m_nHeight) );
}

void CMapDlg::initSymbol()
{
	// 종목 symbol
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*);

	text.Format("%s\\dev\\cx_symbol.dll", m_szRootDir);
	m_inst = AfxLoadLibrary(text);
	if (m_inst != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_inst, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;

			param.key   = type_symbol;
			param.name  = _T("17413");
			CRect rc;

			m_btnJM.GetWindowRect(&rc);
			ScreenToClient(&rc);
			
			param.rect  = CRect(rc.right+3 , rc.top, rc.right+116,rc.bottom);//CRect(160 + 9, rc.top, 293 + 9,rc.bottom);
//			param.rect  = CRect(rc.left-20 , rc.top, rc.left+93, rc.bottom);//CRect(160 + 9, rc.top, 293 + 9,rc.bottom);
			param.fonts = "굴림체";
			param.point = 9;
			param.style = 1;
			param.tRGB  = 63;
			param.pRGB  = 181;
			param.options = _T("/a89/b91/c92/d69/i99/s7710"); 

			m_symbol = (*axCreate)(m_pParent, &param);
			m_symbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);

			//내렸다 올라오면 심볼 사라지는 현상 해결
			m_symbol->SetParent(this);

			if (m_symbol == NULL)
			{
				AfxFreeLibrary(m_inst);
				m_inst = NULL;
			}
		}
	}
}

void CMapDlg::changeColors()
{
	m_listhdrcolor = GetAxColor(74);
	m_listlinecolor = GetAxColor(65);
	m_txtcolor = GetAxColor(69);
	m_strPal   = GetPalette();

	m_clrLineColor[0] = GetAxColor(131);
	m_clrLineColor[1] = GetAxColor(132);
	m_clrRoundColor = GetAxColor(TOOLINDEXCOLOR);
	m_hRoundColor = CreateSolidBrush(m_clrRoundColor);

	SetBrush(GetAxColor(BACKINDEXCOLOR));
	m_pTab->LoadTabImage(m_szRootDir, false);
 	m_pTab->Invalidate();

 	m_List.SetTextColor( m_txtcolor);
	m_List.SetBkColor( GetAxColor(68),GetAxColor(77));
	m_List.SetEdgeColor( GetAxColor(65));
	m_List.SetSelectColor( GetAxColor(78));
	m_List.m_HdCtrl.SetTextColor(GetAxColor(76));
	m_List.m_HdCtrl.SetBkColor(GetAxColor(74));
	m_List.m_HdCtrl.SetEdgeColor(GetAxColor(65));
	Invalidate();
	
	GetDlgItem(IDC_RADIO_SEL)->Invalidate();
	GetDlgItem(IDC_RADIO_TOT)->Invalidate();
	GetDlgItem(IDC_STATIC_SEARCH)->Invalidate();
	GetDlgItem(IDC_CHECK2)->Invalidate();
	m_List.InvalidateHedarCtrl();
	m_List.Invalidate();
	m_symbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
	m_symbol->Invalidate();
	m_code->Invalidate();

	if(m_pFilterWnd)
		m_pFilterWnd->ChangeColors(m_clrBk, m_clrRoundColor);
}

bool CMapDlg::loadSearchWordList()
{
	CString dir, sRtn = "";
	dir.Format("%s\\user\\%s\\searchword.shr", m_szRootDir, m_szUser);

	TRY
	{
		CFile file;
		if (!file.Open(dir, CFile::modeRead))
			return false;

		UINT nBytesRead = 0;
		int nDataCount = 0;
		nBytesRead = file.Read(&nDataCount, 4);
		if (nDataCount <= 0 || nBytesRead != 4)
		{
			file.Close();
			return false;
		}

		char buffer[108]={0};
		CString szTemp ="";
		m_SearchWordArray.RemoveAll();

		for (int i = 0 ; i < nDataCount ; i++)
		{
			memset(buffer,0x00,108);
			if ((nBytesRead = file.Read(buffer,MAX_WORDSIZE)) <= 0)
				break;
			szTemp = CString(buffer,MAX_WORDSIZE);szTemp.TrimRight();szTemp.Remove(0);
			m_SearchWordArray.InsertAt(i,buffer);
		}
		file.Close();

	} CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return false;
	}
	END_CATCH

	m_comboSearch.ResetContent();
	for (int i = 0 ; i < m_SearchWordArray.GetSize() ; i++)
		m_comboSearch.InsertString(i,m_SearchWordArray.GetAt(i));
	return true;
}

bool CMapDlg::saveSearchWordList()
{
	CString dir, sRtn = "";
	dir.Format("%s\\user\\%s\\searchword.shr", m_szRootDir, m_szUser );

	TRY
	{
		const int nDataCount = m_SearchWordArray.GetSize();
		if (nDataCount <= 0)
			return false;

		CFile file;
		if (!file.Open(dir, CFile::modeCreate|CFile::modeWrite))
			return false;

		const UINT nBytesWrite = 0;
		file.Write(&nDataCount, 4);
		char buffer[108]={0};
		CString szTemp ="";
		for( int i = 0 ; i < nDataCount ; i++)
		{
			memset(buffer,0x00,108);
			szTemp = (LPCTSTR)m_SearchWordArray.GetAt(i);szTemp.TrimRight();szTemp.Remove(0);
			memcpy(buffer,(char*)(LPCTSTR)szTemp, min( szTemp.GetLength() , MAX_WORDSIZE) );
			file.Write( buffer, MAX_WORDSIZE);
		}
		file.Close();

	} CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return false;
	}
	END_CATCH
	
	CString str = m_comboSearch.GetEditText();
	m_comboSearch.ResetContent();
	
	for (int i = 0 ; i < m_SearchWordArray.GetSize() ; i++)
		m_comboSearch.InsertString(i,m_SearchWordArray.GetAt(i));
	m_comboSearch.SetWindowText(str);
	m_comboSearch.m_pedSearch->SetWindowText(str);
	return true;
}

void CMapDlg::sendOOP(int key, char* datb, int datl)
{
	std::unique_ptr<char[]> wb;
	struct	_userTH* userth{};

	wb = std::make_unique<char[]>(L_userTH + datl);
	FillMemory(wb.get(), L_userTH + datl, ' ');
	userth = (struct _userTH*)wb.get();
	CopyMemory(userth->trc, "POOPPOOP", sizeof(userth->trc));
	userth->key = key;
	userth->stat = US_OOP;
	CopyMemory(&wb[L_userTH], datb, datl);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)wb.get());
	if (m_code)
	{
		CString data;
		data = m_code->GetEditData();
		data.TrimLeft(); data.TrimRight(); data.Remove(0);
		if (!data.IsEmpty() && data.GetLength() == 6 && GetAxVariant(whichCC, data))
		{
			data.Insert(0, "1301\t");
			GetAxVariant(codeCC, data);

			if (m_bJmyd && !m_bTrigger)
			{
				if (data.Right(6) == m_szCode)
				{
					m_slog.Format("[7710]sendOOP Trigger_CC, m_szCode =[%s]size[%d] data=[%s]size[%d]", m_szCode, m_szCode.GetLength(), data.Right(6), data.GetLength());
					OutputDebugString(m_slog);
					GetAxVariant(triggerCC, data);
				}
			}
		}
		else	m_code->SetEditData(_T(""));
	}
}

void CMapDlg::sendTRA(CString trCode, char* datB, int datL, char key)
{
	std::unique_ptr<char[]> sendB;
	sendB = std::make_unique<char[]>(L_userTH + datL + 1);
		
	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());
}

void CMapDlg::sendTR(CString strCode, void *m, int nLen, int key, bool bOOP )
{
	try
	{
		if (strCode.GetLength() != 8)
			return;

		std::unique_ptr<char[]> cDataBuffer = std::make_unique<char[]>(4096);
		memset(cDataBuffer.get(), 0x00, 4096);

		_userTH* userTH = (_userTH*)&cDataBuffer[0];

		strncpy(userTH->trc, strCode, 8);		// tr 코드를 넣어줌...
		userTH->key = key;
		userTH->stat = NULL;

		CopyMemory(&cDataBuffer[L_userTH], m, nLen);

		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)cDataBuffer.get());

		CString sDat, sTmp = _T("");
		if (m_szCode.GetLength() > 0 && bOOP && !m_bTotSearch) // 코드가 있고, 전체 선택이 아니면 
		{
			sDat.Format("1301%c%s\t1021\t17413\t", 0x7f, m_szCode);
			sendOOP(type_symbol, (char*)sDat.operator LPCTSTR(), sDat.GetLength());
		}
		//if (strCode.GetLength() != 8)
		//	return;
		//
		//char*	cDataBuffer = new char [2048];
		//memset(cDataBuffer,0x00,2048);
		//
		//_userTH* userTH = (_userTH *) &cDataBuffer[0];
		//
		//strncpy(userTH->trc, strCode, 8);		// tr 코드를 넣어줌...
		//userTH->key = key;
		//userTH->stat = NULL;		
		//
		//CopyMemory(&cDataBuffer[L_userTH], m, nLen);
		//
		//m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM) cDataBuffer);

		//delete[] cDataBuffer;
		//CString sDat, sTmp = _T("");
		//if (m_szCode.GetLength() > 0 && bOOP &&!m_bTotSearch) // 코드가 있고, 전체 선택이 아니면 
		//{
		//	sDat.Format("1301%c%s\t1021\t17413\t", 0x7f, m_szCode);
		//	sendOOP(type_symbol,(char *)sDat.operator LPCTSTR(), sDat.GetLength());
		//}
	}
	catch (CException* e)
	{
		e->ReportError();
	}
} 

void CMapDlg::CheckCodeTrigger()
{
	//if(!m_bCodeTrigger) return;
	if(!m_btTrg.GetCheck()) return;
	const int iselect  = m_List.GetSelectedItem();
	if(iselect < 0) 
		return;


	NewsInfo pnews;
	if( !m_List.GetNewsInfo(iselect, pnews)) return;

	CString sCode = pnews.sCode;
	if(sCode.IsEmpty() ) 
		return;
	if(sCode == "" )
		return;
	if(sCode.GetAt(0) =='A') 
		sCode = sCode.Right(sCode.GetLength() -1);  	

	CString sdata;
	sdata.Format("1301\t%s", sCode);

// 	OutputDebugString("CheckCodeTrigger " + sdata);

m_slog.Format("[7710]CheckCodeTrigger Trigger_CC, size =[%d] option=[%s]", m_jobList.GetSize(), sdata);
OutputDebugString(m_slog);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)(LPCTSTR)sdata );
}

void CMapDlg::parsingContent(char *pBytes, int nBytes)
{
	//아래	보여주는것
// 	AfxTrace("parsingContent(IN)\n");

	int modL = sizeof(mod_content);
	if (nBytes < modL)	return;
	mod_content*	mod = (mod_content*)pBytes;
	int lineCnt = atoi(CString(mod->size, sizeof(mod->size)));

	nBytes -= modL - 1; // title(132)+size(5)
	pBytes += modL - 1;

	CString titleS;
	titleS.Format("제목 : %s", CString(mod->titl, title_len));
	titleS.TrimRight();
	
	if (g_htmlindex > 100)	g_htmlindex = 0;

	try {	
		CString strKey;
		strKey.Format("%08u", HashKey((LPCSTR)m_szUser));
		//AfxMessageBox(strKey);
		CString strDir;
		strDir.Format("%s\\%s\\%s", m_szRootDir, "User", strKey); 
		m_szFileName.Format("%s\\%s\\%s%X.html", m_szRootDir, "User", strKey, g_htmlindex++);

		CFile	file;
		CFileException filex;
		if (!file.Open(m_szFileName, CFile::modeCreate | CFile::modeReadWrite, &filex))
		{
			filex.ReportError();
			return;
		}

		if (strstr(pBytes, "text://") != NULL)
		{
			int	tagL = 7;
			CString	strTitle, strData, strCod, szWrite;
			CStringArray subcod;

			strTitle = makeTitle(titleS);
			if( makeSubCod( mod, strCod, subcod ) )
				strData += strCod;
			else
				strTitle += "<br>";
	
			strCod.Format("%.*s", nBytes-tagL, &pBytes[tagL]);		
			if( m_szCurKey[8] == '0' && m_szCurKey[9] == '2' )
			{
				convertLink( strData, subcod );
				strData += strCod;				
			}
			else
			{
				strData += strCod;
				convertLink( strData, subcod );
			}

			szWrite = strTitle + strData;
			CString stmp;
			stmp = szWrite.Right(10);
			szWrite.Replace(stmp, stmp + "  ");
			file.Write(szWrite, szWrite.GetLength());
			file.Flush();
		}
		else
		{
			if (lineCnt*LENGTH_OF_LINE != nBytes)
			{
				GetAxVariant(guideCC, "수신 데이터 오류입니다." );
				file.Close();
				return;
			}

			CString	strTitle, strData, strCod, szWrite;
			CStringArray subcod;

			strTitle = makeTitle(titleS);
			/*pBytes += LENGTH_OF_LINE;
			nBytes -= LENGTH_OF_LINE;*/  // 2009/08/05 특정 기사 본문의 앞부분이 짤리는 문제 때문에 주석 처리함

			if( makeSubCod( mod, strCod, subcod ) )
				szWrite += strCod;
			else
				strTitle += "<br>";

			szWrite += _T("<font face=\"굴림체\" style=\"font-size:12px;\"><pre>");	
		
			int ifind = 0;
			for (int ii = 0; ii < lineCnt; ii++)
			{
				if (!AfxIsValidAddress(pBytes, LENGTH_OF_LINE, FALSE))
					break;

				strData = CString(pBytes, LENGTH_OF_LINE);
				strData.Replace( '\0', ' ');
				strData.TrimRight(' ');
				strData.TrimLeft(' ');
			
				const int iLen = strData.GetLength();

				bool bnewline = true;
				if( (ifind=strData.ReverseFind('<')) > -1 )				
					if( strData.Find(">", ifind+1) < 0) 
						bnewline = false; 	

				if(bnewline) 
					strData += "<br>";
				
				szWrite += strData;

				pBytes += LENGTH_OF_LINE;
				nBytes -= LENGTH_OF_LINE;
				if (nBytes <= 0)	break;
			}			

			//2017.01.31 KSJ 에레메세지 넣어준다.
// 			szWrite += bError ? strError:"";
			szWrite += "</pre></font></body></html>";
			
			convertLink( szWrite, subcod );
			strData = szWrite;
		
			szWrite = _T("<html><body>");
			szWrite += strTitle;
			szWrite += strData;

			file.Write(szWrite, szWrite.GetLength());			
		}		

		file.Close();

		m_pTab->SetCurSel(select_tab);		
		//selectTab(select_tab);
	
		OnSelchangeFontsize();
	} 
	catch( CFileException* e ) {
		e->ReportError();
	}
	catch( CException* e ) {
		e->ReportError();
	}

	AfxTrace("parsingContent(OUT)\n");
}

void CMapDlg::parsingTitleList(int nKey, void* buffer, int len)
{
	CString s;
	s.Format("NEWS [%d] [%d]\n",m_nTrkey,nKey);
//	OutputDebugString(s);

	//제목리스트
	if(m_nTrkey != nKey) return;

	if (!m_bTriggerTR)	
		m_List.SetFocus();	// 20070518(trigger경우 다른 map의 focus잃어 버리지 않도록 focus처리 안함)
	m_bTriggerTR = false;

	CString szTemp;
	int value{};
	mod_title *pmod = (mod_title*)buffer;

	CopyMemory(&m_grido, &pmod->grid_o, sizeof(GRID_O));
	memset(m_szSave, 0x00, 80);
	CopyMemory(m_szSave, pmod->grid_o.save, sizeof(m_szSave));

 //	szTemp.Format("7710 [KSJ] parsingTitleList m_szSave[%s]", m_szSave);
 //	OutputDebugString(szTemp);

	news_info news;
	szTemp = CString(pmod->nrec , 4) ;
	const int nCount = atoi(szTemp);
	
	if(nCount == 0)	
		m_bCheckNewCount = TRUE;	
	else	
		m_bCheckNewCount = FALSE;

	if (pmod->grid_o.xpos[0] == 0x40)
		m_btnNext.EnableWindow(FALSE);
	else 
		m_btnNext.EnableWindow(TRUE);

	if (pmod->grid_o.aflg[0] == '0' )
	{
		m_List.DeleteAllItems();
		m_List.DeleteAllNewsInfo();
	}

	const int nShow =m_List.GetItemCount();
	if (nShow == 0)		
		emptyFile();
	
	for (int ii = 0 ; ii < nCount; ii++)
	{
		CString str;

		// 날짜..........................	
		szTemp	= CString(pmod->grid[ii].date, 8); szTemp.TrimLeft();
		value = atoi(szTemp);
		news.date.Format("%04d/%02d/%02d", (value/10000), ((value % 10000)/100), (value % 100));		

		// 시간...........................
		szTemp = CString(pmod->grid[ii].time, 4); szTemp.TrimLeft();
		value = atoi(szTemp);
		news.time.Format("%02d:%02d", (value/100), (value%100));
		
		// 제목...........................
		szTemp = CString(pmod->grid[ii].titl, title_len);
		szTemp.TrimLeft();	szTemp.TrimRight();	szTemp.Remove(0);
		memset(news.titl, 0x00, title_len);
		CopyMemory(news.titl, szTemp, min(title_len,szTemp.GetLength()));		

		CString s;
// 		s.Format("NEWS TITLE [%s] [%s]\n",szTemp,pmod->grid[ii].titl);
// 		OutputDebugString(s);
		
		// 정보구분.......................
		szTemp = CString(pmod->grid[ii].gubn, 12/*12*/); // 10
		szTemp.TrimLeft();
		szTemp.TrimRight();	szTemp.Remove(0);
		news.gubn = szTemp;		
		
		// 분류코드.......................
		szTemp = CString(pmod->grid[ii].subg, 20);
		szTemp.TrimLeft();
		szTemp.TrimRight();	szTemp.Remove(0);
		news.subg = szTemp;		
		
		//종목코드...
		szTemp = CString(pmod->grid[ii].code, 12);
		szTemp.TrimLeft();	szTemp.TrimRight();	szTemp.Remove(0);
		news.code = szTemp;		

		// 종목명 ........................
		szTemp	= CString(pmod->grid[ii].hnam, 20);
		szTemp.TrimLeft();	szTemp.TrimRight();	szTemp.Remove(0);
		if (szTemp.GetLength() > 16)
			szTemp = szTemp.Left(16);
		news.hnam = szTemp;		

		// CISAM.......................
		szTemp = CString(pmod->grid[ii].gisn, 10);
		szTemp.TrimLeft();	szTemp.Remove(0);
		news.gisn = szTemp;		

		// CISAM.......................
		szTemp = CString(pmod->grid[ii].sisn, 10);
		szTemp.TrimLeft();	szTemp.Remove(0);
		news.sisn = szTemp;		
		
		// param...
		szTemp = CString(pmod->grid[ii].keyv, 24);
			szTemp.TrimRight();	szTemp.Remove(0);
		news.keyv = szTemp;		

		// flag
		szTemp = CString(pmod->grid[ii].flag, 1);
		news.flag = szTemp;	
		
		//체크된것만 추가시킨다.	
		if (isMediaChecked(news))
		{
// 			OutputDebugString("[KSJ] parsingTitleList");
			gridAddRow(news);			
		}

// 		CString strTemp;
// 		strTemp.Format("isMediaChecked(news) = %s", isMediaChecked(news) ? "TRUE" : "FALSE");
// 		OutputDebugString(strTemp);
	}

	if (m_bNextPushed)
	{
		if (!m_bInitReq)
			PostMessage(WM_SEND, EVT_SENDCONTENT, nShow);
		m_List.SetSelectionMark(nShow);
		m_List.SetItemState(nShow,LVIS_SELECTED , LVIS_SELECTED);
		
		m_List.SetScroll(nShow, TRUE);
		
		m_bNextPushed = false;
	}
	else if (m_bAutoQuery)
	{
		if (!m_bInitReq)
			PostMessage(WM_SEND, EVT_SENDCONTENT, 0);
		m_List.SetSelectionMark(nShow);
		m_List.SetItemState(nShow,LVIS_SELECTED , LVIS_SELECTED);
	}	

	if (m_bInitReq)
	{
		PostMessage(WM_SEND, EVT_SENDCONTENT, 0);

		m_bInitReq = FALSE;
		
		m_List.SetSelectionMark(0);
		m_List.SetItemState(0,LVIS_SELECTED , LVIS_SELECTED);

		m_List.SetScroll(0, TRUE);
	}

	if(m_bColShow && m_List.GetItemCount() < 25)
	{
		m_bInitReq = TRUE;

		SendMessage( WM_REQUEST_NEXT);
	}
}

void CMapDlg::parsingSymbol(char* datb, int datl)
{
	int	value;
	CString	text;

	text  = CString(datb, datl);

	value = text.Find('\t');
	CString strTemp;
// 	strTemp.Format("[KSJ]parsingSymbol text [%s] vlaue[%d] ", text, value);
// 	OutputDebugString(strTemp);
// 
// 	OutputDebugString("[KSJ]parsingSymbol " + m_szCode);

	if (value != -1)
	{
		WPARAM	wParam{};
		CString tempCode;

		tempCode  = text.Left(value++);

		if( tempCode.GetLength() ==7 )
		{
			m_szCode = tempCode.Mid(1);
			if(m_code)
			{
				m_code->SetEditData( m_szCode);	
				m_slog.Format("7_SetEditData_[%s]", m_szCode);
				OutputDebugString(m_slog);
				m_code->SetHistory( GetAxVariant(historyCC, _T("1301")));
			}
		
// 			OutputDebugString("[KSJ]parsingSymbol " + m_szCode);

			text   = text.Mid(value);

// 			OutputDebugString("[KSJ]parsingSymbol text : " + text);

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (text.GetLength() > 6) ? text.GetLength() : 0);
			m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}
		else if (!tempCode.Compare("1301"))
		{
			text   = text.Mid(value);

			const int len = text.GetLength();

// 			CString str;
// 			str.Format( "[%d]:[%s]",len,text );
// 			OutputDebugString("[KSJ]parsingSymbol " + str);

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), len);
			m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}
		else
		{
			//text   = text.Mid(value);
			const int len = text.GetLength();
						
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), len);
			m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}

	}
}


CString CMapDlg::GetNewGubnText(int igubn)
{
	if(igubn == 14)
	{
		return "촉n토크";
	}
	else
	{
		for(int i=0; i<m_arNewsList.GetSize(); i++)
		{
			if(m_arNewsList.GetAt(i).iGubn  ==  igubn )
			{
				return m_arNewsList.GetAt(i).sGubnName;
			}
		}	
	}

	return _T("");
}

void CMapDlg::parsingAlert(CString dat/*char *pcBuffer*/)
{
	if(m_bSelectScrap) 
		return;
	
	try
	{
		CMapStringToString	fms;
		CString			value, str, szBuffer, szText;
		news_info		news;

		fms.RemoveAll();
		SplitString_MAP(dat, "\t", fms); //
		
		fms.Lookup("301", news.code);		// 종목코드
		news.code.TrimRight();

		//종목명 라디오 버튼 누르고 뉴스 볼 경우
		if(m_pRadioSel)
		{
			const int icodechk = m_pRadioSel->GetCheck();
			if(icodechk)
			{
				CString sCode = m_code->GetEditData();
				sCode.TrimRight(); 
				if( sCode != news.code) return;
			}
		}

		fms.Lookup("015", value);		// 제목
		value.TrimRight();
		CopyMemory(news.titl, value, min(value.GetLength(), title_len));

		fms.Lookup("016", news.keyv);		// Keyv
		
		if (fms.Lookup("022", value))		// 종목명
		{
			value.TrimLeft(), value.TrimRight();
			news.hnam = value;
		}
		else	news.hnam.Empty();

		if (fms.Lookup("041", value))		// 뉴스구분
		{
			const int gubn = atoi(value);
			news.gubn = GetNewGubnText(gubn);
			news.gubn.TrimRight();
		}

		if (fms.Lookup("044", value) && value.GetLength() >= 4)		// 시간
			news.time.Format("%s:%s", value.Left(2), value.Mid(2, 2));

		fms.Lookup("045", news.subg);		// 분류명
		fms.Lookup("046", news.gnam);		// 구분명
		fms.Lookup("047", news.flag);		// 텍스트(0) or HTML(1)
		if (fms.Lookup("048", value) && value.GetLength() >= 8)		// date
			news.date = value.Left(4) + "/" + value.Mid(4,2) +"/" + value.Right(2);
		
		fms.Lookup("014", news.gisn);		// CISAM

		if (news.code.GetLength() == 6)
			news.code = "A" + news.code;
			
//  		OutputDebugString(_T("This is ------>"));
//  		OutputDebugString(news.gubn+"\n");
		// 실시간은 앞에다 넣기
		if(isMediaChecked(news, true))
		{
			//OutputDebugString(_T("isMdiaChecked(TRUE) ------>\n"));
			
			int	listCnt = m_List.GetItemCount();
			const	int maxCnt = 200;

			const int nItem = m_List.GetSelectedItem();
			if (nItem < maxCnt && listCnt >= maxCnt)
			{
				while (listCnt >= maxCnt)	// 20070515 500->200개로 줄임(메모리 부하)
				{
					if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
						break;
					m_List.DeleteItem(listCnt - 1);
					listCnt = m_List.GetItemCount();
				}
 
			}
			else if (nItem > maxCnt && listCnt >= nItem)
			{
				while (listCnt > nItem+3)
				{
					if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
						break;
					m_List.DeleteItem(listCnt - 1);
					listCnt = m_List.GetItemCount();
				}
			}

// 			OutputDebugString("[KSJ] parsingAlert");

			gridAddRow(news, false);
			/*if (m_bAutoQuery)
			{
				m_List.SetSelectionMark(0);
				m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);


				PostMessage(WM_SEND, EVT_SENDCONTENT, 0);
				
			}*/
		}
		else		//뉴스사를 클릭한 경우
		{
			//OutputDebugString(_T("isMdiaChecked(FALSE) ------>"));
			//선택한 뉴스사와 실시간으로 받은 뉴스사가 동일한지 체키럽~!
			if(m_TreeWnd.GetItemText(select_hItem) != news.gubn)
			{
				//OutputDebugString(_T("SAME CHECK ------>"));
				//OutputDebugString(m_TreeWnd.GetItemText(select_hItem)+"\n");
				//AfxMessageBox("RETURN" + news.gubn);
				return;
			}
			
			int	listCnt = m_List.GetItemCount();
			const	int maxCnt = 200;
			
			const int nItem = m_List.GetSelectedItem();

			if (nItem < maxCnt && listCnt >= maxCnt)
			{
				while (listCnt >= maxCnt)	// 20070515 500->200개로 줄임(메모리 부하)
				{
					if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
						break;
					m_List.DeleteItem(listCnt - 1);
					listCnt = m_List.GetItemCount();
				}
				
			}
			else if (nItem > maxCnt && listCnt >= nItem)
			{
				while (listCnt > nItem+3)
				{
					if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
						break;
					m_List.DeleteItem(listCnt - 1);
					listCnt = m_List.GetItemCount();
				}
			}
			
// 			OutputDebugString("[KSJ] parsingAlert else");
			gridAddRow(news, false);
			/*if (m_bAutoQuery)
			{
				m_List.SetSelectionMark(0);
				m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
				
				
				PostMessage(WM_SEND, EVT_SENDCONTENT, 0);
				
			}	*/	
		}

		fms.RemoveAll();
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}

//2012.01.19 KSJ Alertx 추가

void CMapDlg::parsingAlertx(struct _alertR* alertR)
{
	CString strTemp;

	if(m_bSelectScrap) 
		return;
	
	try
	{
		CString			value, str, szBuffer, szText;
		news_info		news;

		DWORD* data{};

		//2012.02.29 KSJ GetObject에서 체크했지만 DLL_OUB, DLL_TRIGGER 등에서 ADD 하기때문에 다시 체크해준다.
		if(alertR->code.Compare("S0000") != 0)
			return;
		//KSJ

// 		strTemp.Format("alertR->code[%s]", alertR->code);
// 		OutputDebugString(strTemp);

		for(int ii = 0; ii < alertR->size; ii++)
		{
			data = (DWORD*)alertR->ptr[ii];

			if(data[301])
			{
				news.code = (char*)data[301];		//종목코드
				news.code.TrimRight();

// 				OutputDebugString("[KSJ] parsingAlertx news.code[" + news.code + "]");
			}

			//종목명 라디오 버튼 누르고 뉴스 볼 경우
			if(m_pRadioSel)
			{
				const int icodechk = m_pRadioSel->GetCheck();
				if(icodechk)
				{
					CString sCode = m_code->GetEditData();
					sCode.TrimRight(); 
					if( sCode != news.code) return;
				}
			}

			if(data[15])
			{
				value =  (char*)data[15];		// 제목
				value.TrimRight();
				ZeroMemory(news.titl, title_len);
				CopyMemory(news.titl, value, min(value.GetLength(), title_len));

// 				CString s;
// 				s.Format("NEWS ALERT [%s] [%s]\n",value,(char*)data[15]);
// 				OutputDebugString(s);

// 				strTemp.Format("[KSJ] parsingAlertx news.titl[%s]", news.titl);
// 				OutputDebugString(strTemp);
			}


			if(data[16])
			{
				news.keyv = (char*)data[16];		// Keyv
				
// 				OutputDebugString("[KSJ] parsingAlertx news.keyv[" + news.keyv + "]");

// 				strTemp.Format("Key[%s]", (char*)data[16]);
// 				OutputDebugString(strTemp);
			}

			if (data[22])	
			{
				value = (char*)data[22];	//종목명

// 				strTemp.Format("Name[%s]", (char*)data[22]);
// 				OutputDebugString(strTemp);

				value.TrimLeft(), value.TrimRight();
				news.hnam = value;

// 				OutputDebugString("[KSJ] parsingAlertx news.hnam[" + news.hnam + "]");
			}
			else	news.hnam.Empty();

			if (data[41])	
			{
				value = (char*)data[41];	//뉴스구분

 				strTemp.Format("7710 Gubn[%s]", (char*)data[41]);
 	//			OutputDebugString(strTemp);

				const int gubn = atoi(value);
				news.gubn = GetNewGubnText(gubn);
				news.gubn.TrimRight();


if(gubn == 15 || gubn == 16)
{
	CString slog;
	slog.Format("[7710] [%d] [%s] \n", gubn, news.titl);
	OutputDebugString(slog);
}
// 				OutputDebugString("[KSJ] parsingAlertx news.gubn[" + news.gubn + "]");
			}

			if(data[44])
			{
				value = (char*)data[44];		//시간
				
// 				strTemp.Format("Time[%s]", (char*)data[44]);
// 				OutputDebugString(strTemp);

				if (value.GetLength() >= 4)		// 시간
					news.time.Format("%s:%s", value.Left(2), value.Mid(2, 2));

// 				OutputDebugString("[KSJ] parsingAlertx news.time[" + news.time + "]");
			}


			if(data[45])
			{
				news.subg = (char*)data[45];		//분류명
// 				OutputDebugString("[KSJ] parsingAlertx news.subg[" + news.subg + "]");
			}
			
			if(data[46])
			{
				news.gnam = (char*)data[46];		//구분명

// 				OutputDebugString("[KSJ] parsingAlertx news.gnam[" + news.gnam + "]");
			}

			if(data[47])
			{
				news.flag = (char*)data[47];		//텍스트(0) or HTML(1)

// 				OutputDebugString("[KSJ] parsingAlertx news.flag[" + news.flag + "]");
			}

			if(data[48])
			{
				value = (char*)data[48];		//date

				if (value.GetLength() >= 8)		// date
					news.date = value.Left(4) + "/" + value.Mid(4,2) +"/" + value.Right(2);

// 				OutputDebugString("[KSJ] parsingAlertx news.date[" + news.date + "]");
			}
			
			if(data[14])
			{
				news.gisn = (char*)data[14];		//CISAM

// 				OutputDebugString("[KSJ] parsingAlertx news.gisn[" + news.gisn + "]");
			}

			if (news.code.GetLength() == 6)
				news.code = "A" + news.code;		
			
			// 실시간은 앞에다 넣기
			if(isMediaChecked(news, true))
			{		
				const int	listCnt = m_List.GetItemCount();
				const	int maxCnt = 200;

				//2016.08.10 KSJ 메모리부하? 주석처리
/*
				int nItem = m_List.GetSelectedItem();
				if (nItem < maxCnt && listCnt >= maxCnt)
				{
					while (listCnt >= maxCnt)	// 20070515 500->200개로 줄임(메모리 부하)
					{
						if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
							break;
						m_List.DeleteItem(listCnt - 1);
						listCnt = m_List.GetItemCount();
					}

				}
				else if (nItem > maxCnt && listCnt >= nItem)
				{
					while (listCnt > nItem+3)
					{
						if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
							break;
						m_List.DeleteItem(listCnt - 1);
						listCnt = m_List.GetItemCount();
					}
				}
*/

// 				OutputDebugString("[KSJ] parsingAlertX");
				gridAddRow(news, false);
			}
			else		//뉴스사를 클릭한 경우
			{
				//선택한 뉴스사와 실시간으로 받은 뉴스사가 동일한지 체키럽~!
				if(m_TreeWnd.GetItemText(select_hItem) != news.gubn)
					return;
				
				const int	listCnt = m_List.GetItemCount();
				const	int maxCnt = 200;

				//2016.08.10 KSJ 메모리부하? 주석처리

/*
				int nItem = m_List.GetSelectedItem();

				if (nItem < maxCnt && listCnt >= maxCnt)
				{
					while (listCnt >= maxCnt)	// 20070515 500->200개로 줄임(메모리 부하)
					{
						if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
							break;
						m_List.DeleteItem(listCnt - 1);
						listCnt = m_List.GetItemCount();
					}
					
				}
				else if (nItem > maxCnt && listCnt >= nItem)
				{
					while (listCnt > nItem+3)
					{
						if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
							break;
						m_List.DeleteItem(listCnt - 1);
						listCnt = m_List.GetItemCount();
					}
				}
*/
				
// 				OutputDebugString("[KSJ] parsingAlertX else");
				gridAddRow(news, false);
			}
		}
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}
//2012.01.19 KSJ Alertx 추가 끝

CString CMapDlg::makeTitle(CString title)
{
	CString	date = CString(m_szCurKey,8);
	date.Insert(4,"-");	date.Insert(7,"-");
	CString	ret = _T("");
	ret += "<font  color=\"#000000\" face=\"굴림체\" style=\"font-size:12px;line-height:1.5em;text-decoration:none\"><b>";
	ret += "&nbsp;&nbsp;" + title + " [" + date + "]";
	ret += _T("</b></font><br>");
	return ret;
}


void CMapDlg::emptyFile()
{
	CFileFind finder;
	memset(m_szCurKey, 0x00, 23);
	if (finder.FindFile(m_szFileName))
		DeleteFile(m_szFileName);

#ifndef DF_TEST
	if (m_bUsingEdgeBrowser)  //test edge
	{
		/*CString strEmptyPage = _T("about:blank");
		std::string message_a = strEmptyPage;
		std::wstring wurl;
		wurl.assign(message_a.begin(), message_a.end());
		browserEdge->Init(wurl);*/
	}
	else
	{
		m_pBrowser->Stop();
		m_pBrowser->Navigate2("about:blank");
	}
#endif

	CheckTab( -1 );

	//CoUninitialize(); 
}

#define		P_TAB				0x09
void CMapDlg::selectTab(int nIndex)
{
	bool bChange = false;
	if (nIndex != m_tabIdx)
	{
		m_tabIdx = nIndex;
		bChange = true;
	}

	CString	mapname = _T("");
	CString	option = _T("");
	CString	szcode = _T("");

	try
	{
		const int	nSelList = m_List.GetSelectionMark();

		if (nSelList >= 0 && nSelList < m_List.GetItemCount())
		{
			szcode = m_List.GetItemText(nSelList, 11); // 코드 
		}
		else
		{
			CString str;
			m_code->GetWindowText(str);

			if (str.GetLength() == 6)
			{
				szcode = str;
			}
			else if (str.GetLength() == 0)
			{
				CString sHistory = m_code->m_sHistory;
				CString sCode = Parser(sHistory, "\t");
				if (sCode.GetLength() >= 6)
				{
					m_code->SetEditData(sCode.Left(6));
					m_slog.Format("8_SetEditData_[%s]", sCode.Left(6));
					OutputDebugString(m_slog);
				}
				m_code->SetHistory(GetAxVariant(historyCC, _T("1301")));
				szcode = m_code->GetEditData();
			}
		}

		CWnd* pMainWnd = NULL;
		int key = 0;
		const CRect	rcc;

		//{"현재가", "차트", "기업개요", "관심종목", "매매동향", "외국인매매창구집계", "주문"};	
		if (m_arMapList.GetSize() <= nIndex) return;

		mapname = m_arMapList[nIndex];//g_MapTabList[nIndex];

#ifndef DF_TEST
		if (m_bUsingEdgeBrowser)  //test edge
		{
			
				
		}
		else
		{
			if (m_pBrowser)
				m_pBrowser->Stop();
		}
	
#endif
		
		m_pMapWnd1->ShowWindow(SW_HIDE);
		m_pMapWnd2->ShowWindow(SW_HIDE);

		if (m_bUsingEdgeBrowser)
		{
				if (browserEdge && nIndex > 0)
					browserEdge->ShowWindow(SW_HIDE);
				else
				{
					if(browserEdge)
						browserEdge->ShowWindow(SW_SHOW);
				}
		}
		else
		{
			if (nIndex > 0 && m_pBrowser)
				m_pBrowser->ShowWindow(SW_HIDE);
			else
				if (m_pBrowser)
					m_pBrowser->ShowWindow(SW_SHOW);
		}
		
		const CRect rc,rc2;
		CString tmp; 
		
		nIndex = 0; //뉴스 종목이 있을 경우 기존에 tab change 형식에서 버튼형식으로 변경하므로 위 코드는 본문만 조회 하도록 수정. lwj 26.01.14

		switch (nIndex)
		{
			case 1: // 현재가
				{				
			//	ResetLinkMapSize(CInt(m_arMapWidth[nIndex]), CInt(m_arMapHeight[nIndex]));
				if (!szcode.IsEmpty())
				{
					if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	szcode = szcode.Mid(1);			
				}
				option = "1301\t" + szcode;

				m_pMapWnd2->closeMap();
				//m_pMapWnd3->closeMap();
				
				m_pMapWnd3->MoveWindow(CRect(0,0,0,0));

				m_pMapWnd1->ChangeMap(mapname, option);
				m_pMapWnd1->ShowWindow(SW_SHOW);		
				key = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

				CRect rc1;
				m_pMapWnd1->GetClientRect( rc1 ); 	
				rc1.InflateRect(0,1);
				m_pMapWnd1->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				rc1.DeflateRect(0,1);
				m_pMapWnd1->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				//m_pMapWnd1->GetClientRect(&rc1);
				//m_pMapWnd1->GetWindowRect(&rc2);
				//m_pMapWnd1->ScreenToClient(&rc1);
				m_bCodeNews = FALSE;
				}

				break;

			case 2: // 차트
				{				
			//	ResetLinkMapSize(CInt(m_arMapWidth[nIndex]), CInt(m_arMapHeight[nIndex]));
				if (!szcode.IsEmpty())
				{
					if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	
						szcode = szcode.Mid(1);			
				}

				option = "1301\t" + szcode;

				m_pMapWnd1->closeMap();
				//m_pMapWnd3->closeMap();
				
				m_pMapWnd3->MoveWindow(CRect(0,0,0,0));

				m_pMapWnd2->ChangeMap(mapname, option);
				m_pMapWnd2->ShowWindow(SW_SHOW);	
				key = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

				CRect rc1;
				m_pMapWnd2->GetClientRect( rc1 ); 	
				rc1.InflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				rc1.DeflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				m_bCodeNews = FALSE;
				}
				break;

			case 3: // 기업개요
				//CoInitialize(NULL);
				{				
				if (!szcode.IsEmpty())
				{
					if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	
						szcode = szcode.Mid(1);			
				}

				option = "1301\t" + szcode;

				m_pMapWnd1->closeMap();
				//m_pMapWnd3->closeMap();
				
				m_pMapWnd3->MoveWindow(CRect(0,0,0,0));

				m_pMapWnd2->ChangeMap(mapname, option);
				m_pMapWnd2->ShowWindow(SW_SHOW);	
				key = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

				CRect rc1;
				m_pMapWnd2->GetClientRect( rc1 ); 	
				rc1.InflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				rc1.DeflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				m_bCodeNews = FALSE;
				}

				break;
			case 4://매매동향
				{

			//	ResetLinkMapSize(CInt(m_arMapWidth[nIndex]), CInt(m_arMapHeight[nIndex]));
				if (!szcode.IsEmpty())
				{
					if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	
						szcode = szcode.Mid(1);			
				}

				option = "1301\t" + szcode;

				m_pMapWnd1->closeMap();
				//m_pMapWnd3->closeMap();
				
				m_pMapWnd3->MoveWindow(CRect(0,0,0,0));

				m_pMapWnd2->ChangeMap(mapname, option);
				m_pMapWnd2->ShowWindow(SW_SHOW);	
				key = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

				CRect rc1;
				m_pMapWnd2->GetClientRect( rc1 ); 	
				rc1.InflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				rc1.DeflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				m_bCodeNews = FALSE;
				}

				break;
			case 5://외국인매매창구집계
				{				
			//	ResetLinkMapSize(CInt(m_arMapWidth[nIndex]), CInt(m_arMapHeight[nIndex]) );
				if (!szcode.IsEmpty())
				{
					if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	
						szcode = szcode.Mid(1);			
				}

				option = "1301\t" + szcode;

				m_pMapWnd1->closeMap();
				//m_pMapWnd3->closeMap();
				
				m_pMapWnd3->MoveWindow(CRect(0,0,0,0));
				
				m_pMapWnd2->ChangeMap(mapname, option);
				m_pMapWnd2->ShowWindow(SW_SHOW);	
				key = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

				CRect rc1;
				m_pMapWnd2->GetClientRect( rc1 ); 	
				rc1.InflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				rc1.DeflateRect(0,1);
				m_pMapWnd2->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
				m_bCodeNews = FALSE;
				}
				break;

			case 0:	// 본문조회
	//		default:
				if (bChange)
				{

				}
				//asdfasdf
				//PostMessage(WM_SEND, EVT_SENDNAVIGATE, (LPARAM)(LPCTSTR)m_szFileName);
				//GetNavigate( m_szFileName ); 
				m_pMapWnd1->closeMap();
				m_pMapWnd2->closeMap();

				int nShow = 0;

				if(szcode != "")
				{
					if (!szcode.IsEmpty())
					{
						if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	
							szcode = szcode.Mid(1);			
					}
					option = "1301\t" + szcode;

					m_szCode = szcode;
					if (!m_bTotSearch)
					{
						m_code->SetEditData(m_szCode);
						m_slog.Format("9_SetEditData_[%s]", m_szCode);
						OutputDebugString(m_slog);
						m_code->SetHistory(GetAxVariant(historyCC, _T("1301")));
						SetTimer(811, 500, NULL);
					}
					

// 					if(m_bCodeNews)
// 					{
// 						if (!szcode.IsEmpty())
// 						{
// 							if (szcode.GetAt(0) == 'A' || szcode.GetAt(0) == 'J')	
// 								szcode = szcode.Mid(1);			
// 						}
// 						option = "1301\t" + szcode;
// 						
// 						m_pMapWnd3->SendTrigger(szcode);
// 
// 						SetTimer(811,500,NULL);
// 					}
// 					else
					if(!m_bCodeNews)
					{
						OnCheckSise();
					}

					nShow = 1;
				}

				CString send;
				send.Format("%s%c%d", "ShowSise", P_TAB, nShow); 
				
				m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)send);

// 				OutputDebugString("[KSJ]" + szcode + "[" + m_szCode + "]");

				m_pMapWnd3->SendTrigger(szcode);

#ifndef DF_TEST
				//test edge
				if (m_bUsingEdgeBrowser)
				{
					if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					{
						CString strEdgeUrl;
						strEdgeUrl = m_szFileName;
					
						//BOOL bover{};
					/*	auto future = std::async([&]() {
							while (1)
							{
								bfind = find.FindFile(m_szFileName);
								Sleep(10);

								if (bfind)
								{
									m_slog.Format("\r\n [edge] !!!!!!!!!!! future thread  find");
									OutputDebugString(m_slog);
									break;
								}
								else
								{
									m_slog.Format("\r\n [edge] future thread not find");
									OutputDebugString(m_slog);
								}
								if (bover)
								{
									m_slog.Format("\r\n [edge] --------- future thread time over");
									OutputDebugString(m_slog);
									break;
								}
							}
							});

					
						const std::chrono::milliseconds wtime(2000);
						if (future.wait_for(wtime) != std::future_status::ready)
							bover = true;
						*/

						m_slog.Format("[edge][%s] m_szFileName = [%s] ", __FUNCTION__, m_szFileName);
						OutputDebugString(m_slog);

						CFileFind find;
						BOOL bfind{};
						if (find.FindFile(m_szFileName))
						{
							if (m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
							{
								strEdgeUrl = ExtractHangulCharactersAndURLIncode(m_szFileName);
								strEdgeUrl = "file:///" + strEdgeUrl;
								std::string message = strEdgeUrl;
								std::wstring wnewsurl;
								wnewsurl.assign(message.begin(), message.end());
								browserEdge->Init(wnewsurl);
							}
							else
							{
								m_slog.Format("[edge] 직원용  [%s]", __FUNCTION__);
								OutputDebugString(m_slog);

								CString strkor = "투자증권";
								std::wstring strUni = CA2W(strkor); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
								std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
								std::string strUTF8 = converter.to_bytes(strUni);
								std::string stdres = ToUpperCase(URLEncodeUTF8(strUTF8));
								CString strEnCode;
								strEnCode = stdres.c_str();

								CString strkor1 = "(직원용)";
								std::wstring strUni1 = CA2W(strkor1); // std::wstring에 어떤 값이 들어가 있는지에 따라 선언
								std::wstring_convert<std::codecvt_utf8<wchar_t>> converter1;
								std::string strUTF81 = converter1.to_bytes(strUni1);
								std::string stdres1 = ToUpperCase(URLEncodeUTF8(strUTF81));
								CString strEnCode1;
								strEnCode1 = stdres1.c_str();

								strEdgeUrl.Replace(strkor, strEnCode);
								strEdgeUrl.Replace(strkor1, strEnCode1);
								strEdgeUrl = "file:///" + strEdgeUrl;
								std::string message_a = strEdgeUrl;
								std::wstring wurl;
								wurl.assign(message_a.begin(), message_a.end());
								browserEdge->Init(wurl);
							}
						
						}
						else
						{
							OutputDebugString("[edge] no file timer");
							SetTimer(TM_HTMLFILE, 2000, nullptr);
						}
					}
				}
				else
				{
					if (m_pBrowser) //vc2019
						m_pBrowser->Navigate2(m_szFileName);
				}
#endif
			
				break;	
		}
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	
}

void CMapDlg::resizeTab(BOOL bShow)
{
	CStatic	* pStatic;
	CRect	rcMain, rcSise,rcTree, rcOption, rcList, rcTot, rcSel, rcCode, rcSymbol, rcViewMode, rcSizeInit,rcStaticSearch, rcBtnRight, rcBtnLeft, rcComboSearch, rcBtnJm, rcBtnSell, rcBtnSSet, rcBtnQuery, rcBtnPrint, rcBtnNext, rcTimeCtrl, rcBrowser, rcMap1, rcMap2, rcTab, rcBtnFavg, rcBtnMmdh, rcBtnCp, rcBtnCt, rcBtnPrc;
	const CRect rcBtnDSet;

	GetClientRect(rcMain);
	m_List.GetWindowRect(rcList);					ScreenToClient(rcList);
	m_pRadioTot->GetWindowRect(rcTot);				ScreenToClient(rcTot);
	m_pRadioSel->GetWindowRect(rcSel);				ScreenToClient(rcSel);
	m_code->GetWindowRect(rcCode);					ScreenToClient(rcCode);
	m_symbol->GetWindowRect(rcSymbol);				ScreenToClient(rcSymbol);

	pStatic = (CStatic *)GetDlgItem(IDC_STATIC_SEARCH);		pStatic->GetWindowRect(rcStaticSearch);	ScreenToClient(rcStaticSearch);
	m_btnRight.GetWindowRect(rcBtnRight);				ScreenToClient(rcBtnRight);
	m_btnLeft.GetWindowRect(rcBtnLeft);				ScreenToClient(rcBtnLeft);
	m_comboSearch.GetWindowRect(rcComboSearch);			ScreenToClient(rcComboSearch);
	m_btnJM.GetWindowRect(rcBtnJm);					ScreenToClient(rcBtnJm);
	m_btnSell.GetWindowRect(rcBtnSell);				ScreenToClient(rcBtnSell);

	m_btnFavg.GetWindowRect(rcBtnFavg);				ScreenToClient(rcBtnFavg);
	m_btnMmdh.GetWindowRect(rcBtnFavg);				ScreenToClient(rcBtnMmdh);
	m_btnCp.GetWindowRect(rcBtnFavg);				ScreenToClient(rcBtnCp);
	m_btnCt.GetWindowRect(rcBtnFavg);				ScreenToClient(rcBtnCt);
	m_btnPrc.GetWindowRect(rcBtnFavg);				ScreenToClient(rcBtnPrc);
	
	m_btnSise.GetWindowRect(rcSise);				ScreenToClient(rcSise);
	m_btnSSet.GetWindowRect(rcBtnSSet);				ScreenToClient(rcBtnSSet);
	m_btnSizeInit.GetWindowRect(rcSizeInit);		ScreenToClient(rcSizeInit);
	m_btnSizeInit.GetWindowRect(rcViewMode);		ScreenToClient(rcViewMode);
	m_btnQuery.GetWindowRect(rcBtnQuery);				ScreenToClient(rcBtnQuery);
	m_btnPrint.GetWindowRect(rcBtnPrint);				ScreenToClient(rcBtnPrint);
	m_btnNext.GetWindowRect(rcBtnNext);				ScreenToClient(rcBtnNext);
	m_TimeCtrl.GetWindowRect(rcTimeCtrl);				ScreenToClient(rcTimeCtrl);
	if (m_bUsingEdgeBrowser)
	{
		browserEdge->GetWindowRect(rcBrowser);				
		ScreenToClient(rcBrowser);
	}
	else
	{
		m_pBrowser->GetWindowRect(rcBrowser);				
		ScreenToClient(rcBrowser);
	}
	
	m_pMapWnd1->GetWindowRect(rcMap1);				ScreenToClient(rcMap1);
	m_pMapWnd2->GetWindowRect(rcMap2);				ScreenToClient(rcMap2);
	m_pTab->GetWindowRect(rcTab);					ScreenToClient(rcTab);
	

	rcTree = rcMain;
	rcTree.right = m_nTreeWidth - 5;
	rcTree.bottom = rcMain.bottom - m_nOptHeight;
	m_rcBorderTree.SetRect(rcTree.right, rcTree.top, rcTree.right + 5, rcMain.bottom);
	rcOption.SetRect(rcTree.left, rcTree.bottom, rcTree.right, rcTree.bottom + SIZE_BUTTONS);

	m_TreeWnd.SetWindowPos(&wndTop, rcTree.left, rcList.top/*rcTree.top*/, rcTree.Width(), rcTree.Height()-rcList.top, SWP_SHOWWINDOW);

	if (bShow)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(MAXW + m_nTreeWidth, rcMain.Height()));	
				
		rcList.OffsetRect(m_nTreeWidth, 0);
		rcBrowser.OffsetRect(m_nTreeWidth, 0);
		rcMap1.OffsetRect(m_nTreeWidth, 0);
		rcMap2.OffsetRect(m_nTreeWidth, 0);
		rcTab.OffsetRect(m_nTreeWidth, 0);
		

		m_rcBorder.OffsetRect(m_nTreeWidth, 0);
//		m_btOption.ShowWindow(SW_SHOW);
	}
	else
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(MAXW, rcMain.Height()));	
		m_TreeWnd.ShowWindow(SW_HIDE);
		
		rcList.OffsetRect(-m_nTreeWidth, 0);


		rcBrowser.OffsetRect(-m_nTreeWidth, 0);
		rcMap1.OffsetRect(-m_nTreeWidth, 0);
		rcMap2.OffsetRect(-m_nTreeWidth, 0);
		rcTab.OffsetRect(-m_nTreeWidth, 0);

		m_rcBorder.OffsetRect(-m_nTreeWidth, 0);
		
//		m_btOption.ShowWindow(SW_HIDE);
		m_pOptionWnd->ShowWindow(SW_HIDE);
	}

	m_List.MoveWindow(rcList);		
	m_pRadioTot->MoveWindow(rcTot);
	m_pRadioSel->MoveWindow(rcSel);
	m_symbol->MoveWindow(rcSymbol);	

	pStatic = (CStatic *)GetDlgItem(IDC_STATIC_SEARCH);	pStatic->MoveWindow(rcStaticSearch);
	m_code->MoveWindow(rcCode);				
	m_btnRight.MoveWindow(rcBtnRight);				

	m_btnLeft.MoveWindow(rcBtnLeft);				
	m_comboSearch.MoveWindow(rcComboSearch);			
	m_btnJM.MoveWindow(rcBtnJm);	
	m_btnSise.MoveWindow(rcSise);
	m_btnSell.MoveWindow(rcBtnSell);	

	m_btnFavg.MoveWindow(rcBtnPrc);
	m_btnMmdh.MoveWindow(rcBtnCt);
	m_btnCp.MoveWindow(rcBtnCp);
	m_btnCt.MoveWindow(rcBtnMmdh);
	m_btnPrc.MoveWindow(rcBtnFavg);

	m_btnSSet.MoveWindow(rcBtnSSet);
	m_btnSizeInit.MoveWindow(rcSizeInit);
	m_btnViewMode.MoveWindow(rcViewMode);
	m_btnQuery.MoveWindow(rcBtnQuery);				
	m_btnPrint.MoveWindow(rcBtnPrint);				
	m_btnNext.MoveWindow(rcBtnNext);				
	m_TimeCtrl.MoveWindow(rcTimeCtrl);
	m_pBrowser->MoveWindow(rcBrowser);
	if (browserEdge && browserEdge->IsWindowVisible())  //test edge
		browserEdge->MoveWindow(rcBrowser);
	m_pMapWnd1->MoveWindow(rcMap1);
	m_pMapWnd2->MoveWindow(rcMap2);
	m_pTab->MoveWindow(rcTab);
//	m_btOption.MoveWindow(rcOption);
}

void CMapDlg::resizeTab(CRect rcBorder)
{
	CRect	rcGrid{}, rcTab{}, rcContent{};
	const CRect rcMain{};
	const CRect rcSise{};

	int	nHeight{};
	
	m_List.GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	m_pTab->GetWindowRect(rcTab);	
	ScreenToClient(rcTab);

	if(m_bUsingEdgeBrowser)
		browserEdge->GetWindowRect(rcContent);
	else
		m_pBrowser->GetWindowRect(rcContent);

	ScreenToClient(rcContent);
	
	rcGrid.bottom = rcBorder.top;
	nHeight = rcTab.Height();
	rcTab.top = rcBorder.bottom;
	rcContent.top = rcTab.bottom = rcTab.top + nHeight;

	m_List.MoveWindow(rcGrid, TRUE);
	m_pTab->MoveWindow(rcTab, TRUE);
//	rcContent.top -= 15;	// 20071128
	if (m_bUsingEdgeBrowser)
	{
		if (browserEdge && browserEdge->IsWindowVisible())  //test edge
			browserEdge->MoveWindow(rcContent);
	}
	else
		m_pBrowser->MoveWindow(rcContent, TRUE);

	m_pMapWnd1->MoveWindow(rcContent, TRUE);
	m_pMapWnd2->MoveWindow(rcContent, TRUE);
	CRect	rcbtn;
	m_btnSell.GetWindowRect(&rcbtn);
	ScreenToClient(&rcbtn);
	int gap = 1; // 버튼 사이 간격

	int prc_w = m_bx + 22; // 66
	int prc_x = rcbtn.left - prc_w - gap;
	m_btnPrc.SetWindowPos(&wndTop, prc_x, rcTab.top + 3, prc_w, m_by, SWP_SHOWWINDOW);

	int ct_w = m_bx; // 44
	int ct_x = prc_x - ct_w - gap;
	m_btnCt.SetWindowPos(&wndTop, ct_x, rcTab.top + 3, ct_w, m_by, SWP_SHOWWINDOW);

	int cp_w = m_bx; // 44
	int cp_x = ct_x - cp_w - gap;
	m_btnCp.SetWindowPos(&wndTop, cp_x, rcTab.top + 3, cp_w, m_by, SWP_SHOWWINDOW);

	int mmdh_w = m_bx + m_bx; // 88
	int mmdh_x = cp_x - mmdh_w - gap;
	m_btnMmdh.SetWindowPos(&wndTop, mmdh_x, rcTab.top + 3, mmdh_w, m_by, SWP_SHOWWINDOW);

	int favg_w = m_bx + 56; // 100
	int favg_x = mmdh_x - favg_w - gap;
	m_btnFavg.SetWindowPos(&wndTop, favg_x, rcTab.top + 3, favg_w, m_by, SWP_SHOWWINDOW);

	m_btnSell.SetWindowPos(&wndTop,rcbtn.left,rcTab.top+3,m_bx,m_by,SWP_SHOWWINDOW);
	m_btnPrint.SetWindowPos(&wndTop,rcbtn.left+m_bx+1,rcTab.top+3,m_bx,m_by,SWP_SHOWWINDOW);	
}



bool CMapDlg::isMediaChecked(const news_info& news, bool bAlert /* = false*/)
{
	if( !m_bTotList )
		return true;

	CString slog;
	CString sTmp = news.keyv; // date(8)+gubn(2)+subg(4)+sqpn(10) : keyv
	CString tggg = news.keyv;

//slog.Format("[7710] gubn=[%s] titl=[%s] subg=[%s] gnam=[%s] \n", news.gubn, news.titl, news.subg, news.gnam);
//OutputDebugString(slog);

	if(sTmp.GetLength() < 13) return true;
	
	
	sTmp = sTmp.Mid(8, 6);  // gubn(2)+subg(4)
	CString sGubn = sTmp.Left(2);
	sGubn.TrimLeft();

	const int igubn = CInt( sTmp.Left(2));		
	const int isubgubn = CInt( sTmp.Right(4) );

	if(bAlert)
	{
		CString str = m_TreeWnd.GetItemText( select_hItem );
// 		OutputDebugString(str + "\n");
	
	//	CString strTemp;
 	//	strTemp.Format("[7710] igubn = [%d], isubgubn = [%d],구분 : %s, Time : %s, Title: %s, KEY : %s, Code : %s, HNAM : %s\n", igubn, isubgubn, news.gubn, news.time, news.titl, news.keyv, news.code, news.hnam);
	//	if(news.gubn.Find("IR") >= 0  || news.gubn.Find("컨슈머") >= 0 || igubn == 15 || igubn == 16)  
 	//		OutputDebugString(strTemp);

		int iRetGubn{}, iRetSubg{}; // 현재 선택된 트리 아이템의 구분과 서브구분을 얻어온다.
		
		if( (str == "언론사별" || str == "카테고리별" || str == "외국인/기관 동향") && igubn == 2 )// 2011.12.30 KSJ
		{
			//2011.11.14 KSJ 주석해제하면
			//트리에 언론사별, 카테고리별이 선택되어 있어도 실시간데이터 공시를 가지고 온다.
			if(igubn == 2)
			{
// 				int iSel = -1;
// 				if( isubgubn == 99 ) // 코스닥 공시 		
// 					iSel = 1;		
// 				else if( isubgubn == 88 ) // 프리보드 공시 
// 					iSel = 2;
// 				else
// 					iSel = 0;	
// 				
// 				if( iSel != -1 ) // config 에 인한 체크, 0 에 공시?
// 				{
// 					return GetCheckGongsi(iSel);
// 				}
			}
			else
			{
				for(int i=0;i<m_TreeWnd.m_selItem.GetSize();i++)
				{
					HTREEITEM selectTmp = (HTREEITEM)m_TreeWnd.m_selItem.GetAt(i);

					GetGugnAndSubgFromTREEITEM( selectTmp, iRetGubn, iRetSubg );

					if( iRetSubg == -1 && iRetGubn != -1 ) //  구분이 찍혔으면 
					{
						if( iRetGubn == igubn )
							return true;
					}

					if( iRetSubg != -1 && iRetGubn != -1 ) // 서브구분이 찍혔으면 
					{
						if( iRetSubg == isubgubn && iRetGubn == igubn)
							return true;
					}			
				}
			}

			return false;
		}
		else if( str == "전체" || str == "언론사별" || str == "카테고리별")
		{
			if(igubn == 2)
			{
				int iSel = -1;
				if( isubgubn == 99 ) // 코스닥 공시 		
					iSel = 1;		
				else if( isubgubn == 88 ) // 프리보드 공시 //2013.06.25 KSJ 66(코넥스공시) 추가
					iSel = 2;
				else if( isubgubn == 66 ) // 2013.06.25 KSJ 66(코넥스공시) 추가
					iSel = 3;
				else
					iSel = 0;	
				
				if( iSel != -1 ) // config 에 인한 체크, 0 에 공시?
				{
					return GetCheckGongsi(iSel);
				}
			}
			else if(igubn == 14)
			{
				if(str == "전체")
					return true;
			}
			else
			{
				for(int i=0;i<m_TreeWnd.m_selItem.GetSize();i++)
				{
					HTREEITEM selectTmp = (HTREEITEM)m_TreeWnd.m_selItem.GetAt(i);

					GetGugnAndSubgFromTREEITEM( selectTmp, iRetGubn, iRetSubg );

					if( iRetSubg == -1 && iRetGubn != -1 ) //  구분이 찍혔으면 
					{
						if( iRetGubn == igubn )
							return true;
					}

					if( iRetSubg != -1 && iRetGubn != -1 ) // 서브구분이 찍혔으면 
					{
						if( iRetSubg == isubgubn && iRetGubn == igubn)
							return true;
					}			
				}

			}

			return false;
		}

		if(m_TreeWnd.GetItemText(m_TreeWnd.GetParentItem(select_hItem)) == "카테고리별")
		{
// 			OutputDebugString(_T("CATEGORY TYPE===============>"));
// 			OutputDebugString(news.gnam + "\n");
// 			OutputDebugString(news.gubn + "\n");
// 			OutputDebugString(news.subg + "\n");
			int i = 0;
			for(i=0;i<m_TreeWnd.m_selItem.GetSize();i++)
			{
				HTREEITEM selectTmp = (HTREEITEM)m_TreeWnd.m_selItem.GetAt(i);

				if(GetCateGugnAndSubgFromTREEITEM(selectTmp,igubn,isubgubn))
				{
					//OutputDebugString(_T("CATEGORY TYPE TRUE"));
					return true;
				}
				else
				{
					//OutputDebugString(_T("CATEGORY TYPE FALSE"));
					return false;
				}			
			}
// 			if(GetCateGugnAndSubgFromTREEITEM(select_hItem,igubn,isubgubn))
// 			{
// 				//OutputDebugString(_T("CATEGORY TYPE TRUE"));
// 				return true;
// 			}
// 			else
// 			{
// 				//OutputDebugString(_T("CATEGORY TYPE FALSE"));
// 				return false;
// 			}
		}
		else if (m_TreeWnd.GetItemText(select_hItem) == "외국인/기관 동향" && m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
		{
			if(igubn == 1)
				return true;
		}
		else
		{
			GetGugnAndSubgFromTREEITEM( select_hItem, iRetGubn, iRetSubg );

			if( iRetSubg == -1 && iRetGubn != -1 ) //  구분이 찍혔으면  //뉴스 대분류 선택시 세부뉴스구분은 선택 안함
			{
				if( iRetGubn == igubn )
				{
					if(GetMediaCheck(str))
					{
						return true;
					}
					else 
					{
						return false;
					}
				}
			}

			if( iRetSubg != -1 && iRetGubn != -1 ) // 서브구분이 찍혔으면 
			{
				if( iRetSubg == isubgubn && iRetGubn == igubn)
				{
					if(igubn == 2)
					{
						int iSel = -1;
						if( isubgubn == 99 ) // 코스닥 공시 		
							iSel = 1;		
						else if( isubgubn == 88 ) // 프리보드 공시 
							iSel = 2;
						else if( isubgubn == 66 ) // 2013.06.25 KSJ 66(코넥스공시) 추가
							iSel = 3;
						else
							iSel = 0;	
						
						if( iSel != -1 ) // config 에 인한 체크, 0 에 공시?
						{
							return GetCheckGongsi(iSel);
						}
					}
					else
					{
						HTREEITEM hParent = m_TreeWnd.GetParentItem(select_hItem);

						str = m_TreeWnd.GetItemText(hParent);

						CString tt = "PARENT :" + str + " : " +  news.gubn + "\n";

// 						OutputDebugString(tt);

						if(GetMediaCheck(str))

						{
							return true;
						}
						else 
						{
							return false;
						}
					}
				}
			}
		}
		return false;
	}	

	if( igubn == 2 ) // 들어온 내용이 공시 이면 
	{
		int iSel = -1;
		if( isubgubn == 99 ) // 코스닥 공시 		
			iSel = 1;		
		else if( isubgubn == 88 ) // 프리보드 공시 
			iSel = 2;
		else if( isubgubn == 66 ) // 2013.06.25 KSJ 66(코넥스공시) 추가
			iSel = 3;
		else
			iSel = 0;	
		
		if( iSel != -1 ) // config 에 인한 체크, 0 에 공시?
				if( m_arNewsList.GetAt(1).iAllCheck )
					return true;
				else
					return m_arNewsList.GetAt(1).iCheck[iSel];	
	}

	for(int i=1; i<m_arNewsList.GetSize(); i++) // 1 에서 부터; 0 은 전체 
	{
		NewsList news = m_arNewsList.GetAt(i);

		if(news.iGubn != igubn )
			continue;

		if( news.iAllCheck )
			return true;

		for(int j=0; j<news.ibulruCnt; j++)
		{
			if( atoi( news.sBulruGubnList[j])  !=  isubgubn )
				continue;

			return  m_arNewsList.GetAt(i).iCheck[j]; 				
		}			
	}	

	return true;
}

BOOL CMapDlg::GetMediaCheck(CString strName)
{
	for(int i=0;i<m_TreeWnd.m_selItem.GetSize();i++)
	{
		HTREEITEM selectTmp = (HTREEITEM)m_TreeWnd.m_selItem.GetAt(i);
		
// 		CString tt = "CHECK : " + m_TreeWnd.GetItemText(selectTmp) + "\n";

// 		OutputDebugString(tt);
		if(m_TreeWnd.GetItemText(selectTmp) == strName)
		{
			return true;
		}
	}

	return false;
}

BOOL CMapDlg::GetCheckGongsi(int iSel)
{
	CString strName;

	switch(iSel)
	{
		case 0:
			strName = "거래소공시";
			break;
		case 1:
			strName = "코스닥공시";
			break;
		case 2:
			strName = "프리보드공시";
			break;
		case 3:
			strName = "코넥스공시"; // 2013.06.25 KSJ 66(코넥스공시) 추가
			break;
	}

	for(int i=0;i<m_TreeWnd.m_selItem.GetSize();i++)
	{
		HTREEITEM selectTmp = (HTREEITEM)m_TreeWnd.m_selItem.GetAt(i);

		if(m_TreeWnd.GetItemText(selectTmp) == strName)
		{
			return true;
		}
	}

	return false;
}

void CMapDlg::GetGugnAndSubgFromTREEITEM( HTREEITEM hItem, int& iGubn, int& iSubg )
{
	CString str = m_TreeWnd.GetItemText( hItem );
	HTREEITEM pParent = m_TreeWnd.GetParentItem(hItem);

	CString strParent = m_TreeWnd.GetItemText(pParent);
	//OutputDebugString(str+"\n");
	iGubn = -1;
	iSubg = -1;

	for( int i = 0 ; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList news = m_arNewsList.GetAt(i);
		if( news.sGubnName == str )
		{
			iGubn = news.iGubn;
			return;
		}
		
		if(news.sGubnName == strParent)
		{
			for( int j = 0; j < news.ibulruCnt; j++ )
			{
				if( news.sBulruNameList[j] == str )
				{
					iGubn = news.iGubn;
					iSubg = atoi(news.sBulruGubnList[j]);
				}
			}
		}
	}
}

//2011.12.30 KSJ 외국인/기관 동향
void CMapDlg::GetForeignerGugnAndSubgFromTREEITEM(int iSel)
{

}
// KSJ

BOOL CMapDlg::GetCateGugnAndSubgFromTREEITEM( HTREEITEM hItem, int iGubn, int iSubg )
{
	CString str = m_TreeWnd.GetItemText( hItem );

	for( int i = 0 ; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList news = m_arNewsList.GetAt(i);
		if( news.sGubnName == str && news.ikey == type_category)
		{
			//OutputDebugString(_T("GetCateGugnAndSubgFromTREEITEM===============>"));

			for(int j=0;j<100;j++)
			{
				if(news.stype.sgroup[j].gubn == 0)
				{
					//OutputDebugString(_T("GetCateGugnAndSubgFromTREEITEM Break===============>"));
					break;
				}

				if(news.stype.sgroup[j].gubn == iGubn && news.stype.sgroup[j].subg == iSubg)
				{
					return TRUE;
				}
			}
			return FALSE;
		}
	}

	return FALSE;
}

void CMapDlg::gridAddRow(news_info &news ,bool bPushBack, bool bCountNews)
{
	int nCount = 0;
 	if (bPushBack)	nCount = m_List.GetItemCount();

	std::unique_ptr<LVITEM>lv = std::make_unique<LVITEM>();
	if (!lv) return;

	lv->iItem = nCount;
	lv->iSubItem = 0;
	lv->pszText = _T("");
	lv->mask = LVIF_TEXT;
	m_List.InsertItem(lv.get());
	m_List.SetNewsInfo(nCount, news.gisn, news.sisn, news.keyv, news.code, news.flag);

	if (m_pFilterWnd && m_pFilterWnd->GetSafeHwnd() && !bPushBack)
	{
		m_pFilterWnd->SetMarketNewsData(&news); // 실시간 뉴스에  대해서만 시황화면에 나타난다.
	}

	if( bCountNews ) // 순위 
	{
		CString str;
		str.Format("%d", nCount+1 );
		
		m_List.SetItemText( nCount, 1, str );
	}
	
	// 날짜
	if( bCountNews )
		m_List.SetItemText( nCount, 2, news.date.Left(4)+"/"+news.date.Mid(4,2)+"/"+news.date.Right(2) );
	else
		m_List.SetItemText( nCount, 2, news.date );

	// 시간
	lv->iItem = nCount;
	lv->iSubItem = 3;
	lv->pszText = (char*)(LPCSTR)news.time;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	// 제목
	lv->iItem = nCount;
	lv->iSubItem = 4;
	lv->pszText = (char*)(LPCSTR)news.titl;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	if( bCountNews )
	{
		CString str;
		str.Format("%d ", news.rCnt );

		m_List.SetItemText( nCount, 5, str );
	}
	else
	{
		m_List.SetItemText( nCount, 5, "" );
	}

	// 종목명
	lv->iItem = nCount;
	lv->iSubItem = 6;
	lv->pszText = (char*)(LPCSTR)news.hnam;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	// 정보구분
	lv->iItem = nCount;
	lv->iSubItem = 7;
	lv->pszText = (char*)(LPCTSTR)news.gubn;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	// 분류코드
	lv->iItem = nCount;
	lv->iSubItem = 8;
	lv->pszText = (char*)(LPCSTR)news.subg;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	// count
	CString tmpS;
	tmpS.Format("%d", nCount);
	lv->iItem = nCount;
	lv->iSubItem = 9;
	lv->pszText = (char*)(LPCSTR)tmpS;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	// isam
	lv->iItem = nCount;
	lv->iSubItem = 10;
	lv->pszText = (char*)(LPCSTR)news.gisn;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	lv->iItem = nCount;
	lv->iSubItem = 11;
	lv->pszText = (char*)(LPCSTR)news.code;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv.get());

	/*
	int nCount = 0;
 	if (bPushBack)	nCount = m_List.GetItemCount();

    LVITEM * lv = new LVITEM;
	if (!lv) return;

	lv->iItem = nCount;
	lv->iSubItem = 0;
	lv->pszText = _T("");
	lv->mask = LVIF_TEXT;
	m_List.InsertItem(lv);
	m_List.SetNewsInfo(nCount, news.gisn, news.sisn, news.keyv, news.code, news.flag);

	if (m_pFilterWnd && m_pFilterWnd->GetSafeHwnd() && !bPushBack)
	{
		m_pFilterWnd->SetMarketNewsData(&news); // 실시간 뉴스에  대해서만 시황화면에 나타난다.
	}

	if( bCountNews ) // 순위 
	{
		CString str;
		str.Format("%d", nCount+1 );
		
		m_List.SetItemText( nCount, 1, str );
	}
	
	// 날짜
	if( bCountNews )
		m_List.SetItemText( nCount, 2, news.date.Left(4)+"/"+news.date.Mid(4,2)+"/"+news.date.Right(2) );
	else
		m_List.SetItemText( nCount, 2, news.date );

	// 시간
	lv->iItem = nCount;
	lv->iSubItem = 3;
	lv->pszText = (char*)(LPCSTR)news.time;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	// 제목
	lv->iItem = nCount;
	lv->iSubItem = 4;
	lv->pszText = (char*)(LPCSTR)news.titl;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	if( bCountNews )
	{
		CString str;
		str.Format("%d ", news.rCnt );

		m_List.SetItemText( nCount, 5, str );
	}
	else
	{
		m_List.SetItemText( nCount, 5, "" );
	}

	// 종목명
	lv->iItem = nCount;
	lv->iSubItem = 6;
	lv->pszText = (char*)(LPCSTR)news.hnam;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	// 정보구분
	lv->iItem = nCount;
	lv->iSubItem = 7;
	lv->pszText = (char*)(LPCTSTR)news.gubn;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	// 분류코드
	lv->iItem = nCount;
	lv->iSubItem = 8;
	lv->pszText = (char*)(LPCSTR)news.subg;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	// count
	CString tmpS;
	tmpS.Format("%d", nCount);
	lv->iItem = nCount;
	lv->iSubItem = 9;
	lv->pszText = (char*)(LPCSTR)tmpS;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	// isam
	lv->iItem = nCount;
	lv->iSubItem = 10;
	lv->pszText = (char*)(LPCSTR)news.gisn;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	lv->iItem = nCount;
	lv->iSubItem = 11;
	lv->pszText = (char*)(LPCSTR)news.code;
	lv->mask = LVIF_TEXT;
	m_List.SetItem(lv);

	delete lv;	lv = NULL;
	*/

}

void CMapDlg::openMap(int type, CString data)
{
	if (m_pParent)
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CMapDlg::sendSymbol(CString code)
{
	CString sDat;
	sDat.Format("1301%c%s\t1021\t17413\t", 0x7f, m_szCode);
// 	OutputDebugString("[KSJ]sendSymbol " + sDat);

	sendOOP(type_symbol, (char *)sDat.operator LPCTSTR(), sDat.GetLength());
}

void CMapDlg::parseNews(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos{};
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}
		ary.SetAt(sym, value);
	}
}

void CMapDlg::drawMove(CRect rect)
{
	CWindowDC	dc(this);
	const int	nMode = dc.SetROP2(R2_XORPEN);
	CBrush		brush(RGB(128, 128, 128));
	CBrush	*oldbrush = dc.SelectObject(&brush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	dc.SetROP2(nMode);

	brush.DeleteObject();
	dc.DeleteDC();
}

CString CMapDlg::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_szRootDir, "tab", "palette.ini");
	
	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;
	
	return strPal;
}

void CMapDlg::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");
		
		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");
		
		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMapDlg::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;
	
	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;
	
	memDC.CreateCompatibleDC(pDC);
	
	if (memDC.m_hDC != NULL) 
		memDC.SelectObject(BitMap);

	if (memDC.m_hDC != NULL)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	
	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMapDlg::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";
	
	return retvalue;
}

CRect CMapDlg::getClipRect()
{
	CRect	ret;
	CRect	rect;
	CRect	rcGrid;
	GetClientRect(rect);
	ret = rect;
	m_List.GetWindowRect(rcGrid);
	ScreenToClient(rcGrid);
	ret.top = rcGrid.top + SIZE_MINLIST ;
	ret.bottom = rect.bottom - SIZE_CONTENT;	
	ClientToScreen(ret);
	return ret;
}

void CMapDlg::OnSetNewsType() 
{
	// TODO: Add your control notification handler code here
	if(m_pFilterWnd == NULL)
	{
	//	m_pFilterWnd = new CMarketDlg(m_szRootDir, m_szUser, m_pParent, m_pFont,this); 
		m_pFilterWnd = std::make_unique<CMarketDlg>(m_szRootDir, m_szUser, m_pParent, m_pFont, this);
		m_pFilterWnd->m_arKeyList.Copy(m_arSpecialKeyList);
		m_pFilterWnd->m_arStepColor.Copy(m_arStepColor);
		m_pFilterWnd->m_arNewsList.Copy(m_arNewsList);
		m_pFilterWnd->Create(IDD_MARKET_DLG, this);
	}
	
	if(m_pFilterWnd->GetSafeHwnd())
	{
		m_pFilterWnd->ShowWindow(SW_SHOW);
	}
}


void CMapDlg::CheckUserPath()
{
	CString strUserDir;
	strUserDir.Format("%s/%s/%s/", m_szRootDir, USRDIR, m_szUser);
	if(GetFileAttributes(strUserDir) != 0xFFFFFFFF) return;

	strUserDir.Format("%s/%s/", m_szRootDir, USRDIR);
	if(GetFileAttributes(strUserDir) == 0xFFFFFFFF) 	CreateDirectory(strUserDir,NULL); 
	strUserDir += m_szUser;
	if(GetFileAttributes(strUserDir) == 0xFFFFFFFF) 	CreateDirectory(strUserDir,NULL); 
}

void CMapDlg::ScrapNews(int irow)
{
	if(m_List.GetItemCount() <= 0) 
		return;
	
	CString		sTmplist[MAXCNT];	
	
	NewsInfo pinfo;
	if( !m_List.GetNewsInfo(irow, pinfo)) 
		return;
	sTmplist[8] = pinfo.sGisn;
	sTmplist[9] = pinfo.sSisn;
	sTmplist[10] = pinfo.sKeyv; //key
	sTmplist[11] = pinfo.sCode;
	sTmplist[12] = pinfo.sFlag;
	
	if(m_bSelectScrap)
	{
		//DELETE SCRAP
		if(irow < 0 ) 	
			return;
		m_arScrapList.RemoveAt(irow);
		m_List.DeleteItem(irow); 
		return;
	}
	
	sTmplist[2] = m_List.GetItemText(irow, 4);	// 제목 
	sTmplist[0] = m_List.GetItemText(irow, 2);	// 날짜 
	sTmplist[1] = m_List.GetItemText(irow, 3);	// 시간 

	sTmplist[3] = m_List.GetItemText(irow, 6);	// 종목명  

	sTmplist[4] = m_List.GetItemText(irow, 7);	// 정보구분  
	
	sTmplist[5] = m_List.GetItemText(irow, 8);	// 분류코드 
	sTmplist[6] = m_List.GetItemText(irow, 9);	// count
	sTmplist[7] = m_List.GetItemText(irow, 10);	// isam 	
	
	m_arScrapList.InsertAt(0, AList(13, sTmplist));
}

void CMapDlg::SaveScrapNews()
{
	CheckUserPath();
	CString sFile, strWrite=_T(""), stmp;
	CFile pFile;
	sFile.Format("%s/%s/%s/%s", m_szRootDir, USRDIR, m_szUser, NEWS_SCRAP_FILE);
	
	
	if( !pFile.Open(sFile, CFile::modeCreate | CFile::modeReadWrite) ) return;
	for(int i=0; i<m_arScrapList.GetSize(); i++)
	{
		strWrite = _T("");
		if( m_arScrapList.GetAt(i).iDataCnt < 10) break;
		
		for(int j=0; j<m_arScrapList.GetAt(i).iDataCnt; j++)
		{
			stmp.Format("%s%c", m_arScrapList.GetAt(i).sList[j], TABCHAR);
			strWrite += stmp;
		}
		stmp.Format("%c", NEW_LINE);
		strWrite += stmp;
		pFile.Write( strWrite, strWrite.GetLength());
	}
	
	pFile.Close(); 
}

void CMapDlg::SaveScrapNews(AList scraplist)
{
	CheckUserPath();
	CString sFile, strWrite=_T("");
	CFile pFile;
	sFile.Format("%s/%s/%s/%s", m_szRootDir, USRDIR, m_szUser, NEWS_SCRAP_FILE);
	if (pFile.Open(sFile, CFile::modeReadWrite))
	{
		const	ULONGLONG	pos = pFile.SeekToEnd();
		pFile.Seek((long) pos, CFile::begin);
	}
	else
		pFile.Open(sFile, CFile::modeCreate|CFile::modeReadWrite);
	
	
	for(int j=0; j<scraplist.iDataCnt; j++)
	{
		strWrite += scraplist.sList[j] + "\t";
	}
	strWrite += "\n";
	pFile.Write( strWrite, strWrite.GetLength());
	pFile.Close();
}

void CMapDlg::LoadScrapNews()
{
	CheckUserPath();
	
	CString sFile, strRead=_T("");
	CFile pFile;
	sFile.Format("%s/%s/%s/%s", m_szRootDir, USRDIR, m_szUser, NEWS_SCRAP_FILE);
	
	if( !pFile.Open(sFile, CFile::modeRead ) ) return;
	const	int dFileLength = (int)pFile.GetLength();
	if(dFileLength <= 0	) return;
	strRead.ReleaseBuffer(pFile.Read(strRead.GetBufferSetLength(dFileLength), dFileLength));
	pFile.Close();
	
	
	CStringArray arList;
	CString		sTmplist[MAXCNT];
	SplitString(strRead, (char)NEW_LINE, arList);
	
	int icnt = 0;
	for(int i=0; i<arList.GetSize(); i++)
	{
		if( Trim(arList[i]).IsEmpty() ) continue; 
		icnt = SplitString(arList[i], (char)TABCHAR, sTmplist);
		if(icnt > 0)
		{
			if( Trim(sTmplist[0]).IsEmpty() ) continue; 
			m_arScrapList.Add(AList(icnt, sTmplist));
		}
	}

}

void CMapDlg::SetScrapNews()
{
	m_List.DeleteAllItems();
	m_List.DeleteAllNewsInfo();
	news_info news;
	
	CString stmp, sgisn, skeyv, sflag, scode;
	for(int i=0; i<m_arScrapList.GetSize(); i++)
	{
		if( Trim(m_arScrapList.GetAt(i).sList[0]).IsEmpty() ) continue; 
		news.date = m_arScrapList.GetAt(i).sList[0];// 날짜...
		news.time = m_arScrapList.GetAt(i).sList[1];// 시간
		
		memset(&news.titl, 0x00, title_len);
		CopyMemory(news.titl, m_arScrapList.GetAt(i).sList[2], min(title_len, m_arScrapList.GetAt(i).sList[2].GetLength()));// 제목..
		news.gubn = m_arScrapList.GetAt(i).sList[4];// 정보구분..
		news.subg = m_arScrapList.GetAt(i).sList[5];// 분류코드.
		
		news.hnam = m_arScrapList.GetAt(i).sList[3];// 종목명 
		
		/*
													news.gisn = pnew->gisn;// CISAM....
													news.keyv = pnew->keyv ;// param...
													news.flag = pnew->flag ;// flag...
													news.code = pnew->code; 
		*/
		news.gisn =  m_arScrapList.GetAt(i).sList[8];
		news.keyv= m_arScrapList.GetAt(i).sList[10];
		news.flag=  m_arScrapList.GetAt(i).sList[12];
		news.code=  m_arScrapList.GetAt(i).sList[11];
		
		gridAddRow(news);
	}
	
	if(m_arScrapList.GetSize() > 0)
	{
		m_List.SetSelectionMark(0);
		m_List.SetItemState(0,LVIS_SELECTED , LVIS_SELECTED);
//		m_section.Lock();
		RequestContent(0);
//		m_section.Unlock();
	}
	SendMode(TRUE);
}
void CMapDlg::ResizeControl()
{
	if(!m_pBrowser) 
		return;

	const int nSideGap = 6;
	int nCtrlLeftPos{};
	const int nCtrlHeight = 10;
	const int nCtrlGap = 5;
	CRect rc, rc1;

	GetClientRect(&rc);

	//rc.right = rc.right - 96;
	
	const int itoolbarH = TOOLHEIGHT;

	int itreeW = TREE_WIDTH + GAP10;
	m_nHeight = rc.Height(); 

	if(m_nView)
	{
		m_TreeWnd.MoveWindow(nSideGap, itoolbarH, itreeW, rc.bottom - nSideGap - TOOLHEIGHT); 
		itreeW = itreeW+2*GAP10+2;
	}
	else
		itreeW = 8;

	nCtrlLeftPos = MoveCtrl(IDC_BT_RIGHT, GAP10-1 , nCtrlHeight+1, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_RADIO_TOT, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_RADIO_SEL, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_CODECTRL, nCtrlLeftPos, nCtrlHeight, 0);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_JONGMOK, nCtrlLeftPos, nCtrlHeight, GAP3);
	
	nCtrlLeftPos = MoveCtrl(m_symbol, nCtrlLeftPos, nCtrlHeight, 2*GAP3);
	
	nCtrlLeftPos = MoveCtrl(IDC_STATIC_SEARCH, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_EDIT1, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_DATETIMEPICKER, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_QUERY, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_NEXTPAGE, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_CHECK2, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_COMBO1, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_SEARCH_SET, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_RCVSIZE, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_VIEWMODE, nCtrlLeftPos, nCtrlHeight, GAP3);
	
	m_List.GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	
	const int iBtnTop = rc1.bottom + 6 + 4;
	
	rc1.top = itoolbarH;
	rc1.left = itreeW;
	rc1.bottom = rc1.bottom;

	GetClientRect(&rc);
	rc1.right = rc.right-8; 

	m_List.MoveWindow(rc1);  
	ResizeNewsTitleField(rc1.Width());

	m_rcBorder = rc1;
	m_rcBorder.top =rc1.bottom+1+5;
	m_rcBorder.bottom = m_rcBorder.top + 5;

	m_btnPrint.GetWindowRect(&rc1);
	ScreenToClient(&rc1);

	const int ibtnw = rc1.Width();
	int ibtnh = rc1.Height();

	rc1.left = rc.right - GAP10 - 54;
 	rc1.right = rc1.left + 54;
	rc1.top = iBtnTop+nCtrlGap + 1;
	rc1.bottom = rc1.top+ibtnh;

	m_pFontSize.MoveWindow(rc1);

 	rc1.left = rc.right - GAP10 - 80;
 	rc1.right = rc1.left + 24;
	rc1.top = iBtnTop+nCtrlGap +5;
	rc1.bottom = rc1.top+ibtnh -2;
// 
 	m_pStatic.MoveWindow(rc1);
	
	rc1.left = rc.right - ibtnw - GAP10 - ADDITIONALGAP - 84;
	rc1.right = rc1.left + ibtnw + ADDITIONALGAP;
	rc1.top = iBtnTop+nCtrlGap+1;
	rc1.bottom = rc1.top+ibtnh;
	
	m_btnPreview.MoveWindow(rc1); 
	
	rc1.right = rc1.left  - GAPW ;
	rc1.left = rc1.right - ibtnw ;
	m_btnPrint.MoveWindow(rc1); 

// 	rc1.right = rc1.left  - GAPW ;
// 	rc1.left = rc1.right - ibtnw - 20 ;
// 	m_btnScrap.MoveWindow(rc1); 
	
	rc1.right = rc1.left  - GAPW;
	rc1.left = rc1.right - ibtnw ;
	//rc1.left = rc1.right - 30;		//미리보기 버튼 추가로 인한 사이즈 조정
	m_btnSell.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - (ibtnw + 56);
	m_btnFavg.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - (ibtnw * 2);
	m_btnMmdh.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - ibtnw;
	m_btnCp.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - ibtnw;
	m_btnCt.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - (ibtnw + 22);
	m_btnPrc.MoveWindow(rc1);

	CRect rcBtn;
	m_btnSise.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	const int iWidth = rcBtn.Width();
	
	rc1.right = rc1.left-GAPW;
	rc1.left = rc1.right-iWidth;
	m_btnSise.MoveWindow(rc1);
	
	if(m_pTab)
	{
		m_pTab->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		
		rc1.left = itreeW;
		ibtnh = rc1.Height();
		
		rc1.right = rc.right - GAP10;
		rc1.top = iBtnTop + nCtrlGap;
		rc1.bottom = rc1.top + ibtnh;		
		
		m_rcBorder.bottom = m_rcBorder.top + 5;
		
		m_pTab->MoveWindow(rc1);   
	}	
	
	if(m_pBrowser)
	{
		rc1.left = itreeW-2;
		rc1.bottom = rc.bottom-nSideGap;
		rc1.right = rc.right-nSideGap; 
		
		if(m_pTab)
			m_pTab->GetWindowRect( rc );

		ScreenToClient( rc );
		rc1.top = rc.bottom+1;

		if(m_bCodeNews)
		{
			CRect browserRect;

			browserRect.top = rc1.top;
			browserRect.left = rc1.left;

			if(rc1.Width() > 304)
				browserRect.right = rc1.right - 304;
			else
				browserRect.right = rc1.Width() * 0.3;

			browserRect.bottom = rc1.bottom;

			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(browserRect);
			}
			else
				m_pBrowser->MoveWindow(browserRect);
			
			m_pMapWnd3->MoveWindow(browserRect.right+1,browserRect.top, rc1.Width() - browserRect.Width(), rc1.Height(),FALSE);
		}
		else
		{
			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible() && m_nView)
					browserEdge->MoveWindow(rc1);
				else
				{
					if (m_nView == 0)
					{
						m_rc = rc1;
						KillTimer(TM_EDGECONTROLRESIZE);
						SetTimer(TM_EDGECONTROLRESIZE, 500, nullptr);
					}
				}
			}
			else
				m_pBrowser->MoveWindow(rc1);
		}

		m_pMapWnd1->MoveWindow(rc1);  
		m_pMapWnd2->MoveWindow(rc1); 

	}

	m_clSearchEdit.Invalidate(FALSE);
}

void CMapDlg::ResizeTransformControl(BOOL bFirst)
{
	if(!m_pBrowser) 
		return;

	const int nSideGap = 6;
	int nCtrlLeftPos{};
	const int nCtrlHeight = 10;
	const int nCtrlGap = 5;
	CRect rc, rc1;

	GetClientRect(&rc);

	//rc.right = rc.right - 96;
	
	const int itoolbarH = TOOLHEIGHT;

	int itreeW = TREE_WIDTH + GAP10;
	m_nHeight = rc.Height(); 

	if(m_nView)
	{
		m_TreeWnd.MoveWindow(nSideGap, itoolbarH, itreeW, rc.bottom - nSideGap - TOOLHEIGHT); 
		itreeW = itreeW+2*GAP10+2;
	}
	else
		itreeW = 8;

	nCtrlLeftPos = MoveCtrl(IDC_BT_RIGHT, GAP10-1 , nCtrlHeight+1, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_RADIO_TOT, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_RADIO_SEL, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_CODECTRL, nCtrlLeftPos, nCtrlHeight, 0);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_JONGMOK, nCtrlLeftPos, nCtrlHeight, GAP3);
	
	nCtrlLeftPos = MoveCtrl(m_symbol, nCtrlLeftPos, nCtrlHeight, 2*GAP3);
	
	nCtrlLeftPos = MoveCtrl(IDC_STATIC_SEARCH, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_EDIT1, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_DATETIMEPICKER, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_QUERY, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_NEXTPAGE, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_CHECK2, nCtrlLeftPos, nCtrlHeight+4, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_COMBO1, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_SEARCH_SET, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_RCVSIZE, nCtrlLeftPos, nCtrlHeight, GAP3);
	nCtrlLeftPos = MoveCtrl(IDC_BUTTON_VIEWMODE, nCtrlLeftPos, nCtrlHeight, GAP3);
	
	m_List.GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	
	const int iBtnTop = rc1.top - 5;
	
	rc1.top = itoolbarH;
	rc1.left = itreeW;
	rc1.bottom = rc.bottom - nSideGap;

	GetClientRect(&rc);
	
	if(bFirst)
	{
		//rc1.right = rc.right - (rc1.Width() - itreeW) * 0.6; 
// 		CString s;
// 		s.Format("%d = %d\n",itreeW,rc.Width());
// 		OutputDebugString(s);
		rc1.right = itreeW + (rc.Width() - itreeW) * 0.4;
	}

	m_List.MoveWindow(rc1);  
	ResizeNewsTitleField(rc1.Width());

	const int iW = m_List.GetColumnWidth(0) + m_List.GetColumnWidth(3) + m_List.GetColumnWidth(4);
	
	int nW = m_List.GetColumnWidth(4);
	
	nW = nW + (rc1.Width() - iW);
	
	m_List.SetColumnWidth(4,nW);

	const int nListWidth = rc1.Width() + 3 + GAP10 + 3;

	m_List.GetWindowRect(&rc1);
	ScreenToClient(&rc1);

	m_rcBorder.top = rc1.top;
	m_rcBorder.bottom = rc.bottom - 8;
	m_rcBorder.left =rc1.right+6;
	m_rcBorder.right = m_rcBorder.left + 3;

	m_btnPrint.GetWindowRect(&rc1);
	ScreenToClient(&rc1);

	const int ibtnw = rc1.Width();
	int ibtnh = rc1.Height();

	rc1.left = rc.right - GAP10 - 54;
 	rc1.right = rc1.left + 54;
	rc1.top = iBtnTop+nCtrlGap + 1;
	rc1.bottom = rc1.top+ibtnh;

	m_pFontSize.MoveWindow(rc1);

 	rc1.left = rc.right - GAP10 - 80;
 	rc1.right = rc1.left + 24;
	rc1.top = iBtnTop+nCtrlGap +5;
	rc1.bottom = rc1.top+ibtnh -2;
// 
 	m_pStatic.MoveWindow(rc1);
	
	rc1.left = rc.right - ibtnw - GAP10 - ADDITIONALGAP - 84;
	rc1.right = rc1.left + ibtnw + ADDITIONALGAP;
	rc1.top = iBtnTop+nCtrlGap+1;
	rc1.bottom = rc1.top+ibtnh;
	
	m_btnPreview.MoveWindow(rc1); 
	
	rc1.right = rc1.left  - GAPW ;
	rc1.left = rc1.right - ibtnw ;
	m_btnPrint.MoveWindow(rc1); 
	
	rc1.right = rc1.left  - GAPW;
	rc1.left = rc1.right - 30;		//미리보기 버튼 추가로 인한 사이즈 조정
	m_btnSell.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - 30; // 미리보기 버튼용 사이즈 30
	m_btnSell.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - 100; // ibtnw + 56 (44+56)
	m_btnFavg.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - 88;  // ibtnw * 2 (44*2)
	m_btnMmdh.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - 44;  // ibtnw (44)
	m_btnCp.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - 44;  // ibtnw (44)
	m_btnCt.MoveWindow(rc1);

	rc1.right = rc1.left - GAPW;
	rc1.left = rc1.right - 66;  // ibtnw + 22 (44+22)
	m_btnPrc.MoveWindow(rc1);

	CRect rcBtn;
	m_btnSise.GetWindowRect(rcBtn);
	ScreenToClient(&rcBtn);
	const int iWidth = rcBtn.Width();
	
	rc1.right = rc1.left-GAPW;
	rc1.left = rc1.right-iWidth;
	m_btnSise.MoveWindow(rc1);
	
	if(m_pTab)
	{
		m_pTab->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		
		rc1.left = itreeW + nListWidth + 2;
		ibtnh = rc1.Height();
		
		rc1.right = rc.right - GAP10;
		rc1.top = iBtnTop + nCtrlGap;
		rc1.bottom = rc1.top + ibtnh;		
		
		m_rcBorder.bottom = m_rcBorder.left + 3;
		
		m_pTab->MoveWindow(rc1);   
	}	
	
	if(m_pBrowser)
	{
		rc1.left = itreeW+nListWidth;
		rc1.bottom = rc.bottom-nSideGap;
		rc1.right = rc.right-nSideGap; 
		
		if (m_pTab)
			m_pTab->GetWindowRect( rc );
		ScreenToClient( rc );
		rc1.top = rc.bottom+1;

		if(m_bCodeNews)
		{
			CRect browserRect;

			browserRect.top = rc1.top;
			browserRect.left = rc1.left;

			if(rc1.Width() > 304)
				browserRect.right = rc1.right - 304;
			else
				browserRect.right = rc1.Width() * 0.3;

			browserRect.bottom = rc1.bottom;

			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(browserRect);
			}
			else
				m_pBrowser->MoveWindow(browserRect);
		
			m_pMapWnd3->MoveWindow(browserRect.right+1,browserRect.top, rc1.Width() - browserRect.Width(), rc1.Height(),FALSE);
		}
		else
		{
			if (m_bUsingEdgeBrowser)
			{
				if (browserEdge && browserEdge->IsWindowVisible())  //test edge
					browserEdge->MoveWindow(rc1);
			}
			else
				m_pBrowser->MoveWindow(rc1);
		}
	
		m_pMapWnd1->MoveWindow(rc1);  
		m_pMapWnd2->MoveWindow(rc1);  
	}

	m_clSearchEdit.Invalidate(FALSE);
}

void CMapDlg::OnToolbarRePosition() 
{
	// TODO: Add your control notification handler code here
	m_bTwoToolBar = !m_bTwoToolBar;

	CString simg;
	if(m_bTwoToolBar) simg = "UP_EX";
	else simg = "DOWN_EX";

	CString szn = m_szRootDir + "\\image\\" + simg + ".BMP";
	CString szu = m_szRootDir + "\\image\\" + simg + "_en.BMP";
	CString szd = m_szRootDir + "\\image\\" + simg + "_dn.BMP";
	m_btToolPos.SetSkin(szn,szd,szu);

	ResizeControl();
	InvalidateRect(CRect(0, TOOLHEIGHT, 200, TOOLHEIGHT+TOOLHEIGHT));
}

void CMapDlg::SendMode(BOOL bEnable)
{
	return;
	
	m_TreeWnd.EnableWindow(bEnable);
	m_List.EnableWindow(bEnable);

	GetDlgItem(IDC_BUTTON_QUERY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CONBUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_TOOLBARPOSBUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SEARCH_SET)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO1)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK2)->EnableWindow(bEnable);
}

void CMapDlg::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
	
}

void CMapDlg::ResetLinkMapSize(int iw, int ih) 
{
	CRect rect;
	if(iw <= 0) iw = NEWS_WIDTH;
	//799 548
	int itreeW = TREE_WIDTH;
	if(!m_nView) itreeW = 0;
	
	iw = iw+itreeW+GAPW;
 	int iwindowH = m_nHeight;
	int idifh = 0;

	if(ih > 0 && m_pMapWnd1)
	{
		CRect rc; const CRect wrc;
		m_pMapWnd1->GetWindowRect(&rc);
		idifh = ih - rc.Height();
		
		if(ih != rc.Height() )
		{
			ScreenToClient(rc);
			rc.bottom = rc.top + ih;

			m_pMapWnd1->MoveWindow(rc);
			m_pMapWnd2->MoveWindow(rc);

			m_pMapWnd1->GetWindowRect(rect);
			ScreenToClient(rect);
		}
	}

	if (itreeW<=0) iw+=12;
	else iw+=38;
	iwindowH += idifh;
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iw, iwindowH) );
}


void CMapDlg::SetTitleListHeader(int iw)
{}

void CMapDlg::ResizeNewsTitleField(int iw) 
{
	int iWidth = 0;
	iWidth += m_List.GetColumnWidth(0); // 스크랩
	iWidth += m_List.GetColumnWidth(1); // 순위 
	iWidth += m_List.GetColumnWidth(2); // 날짜 
	iWidth += m_List.GetColumnWidth(3); // 시간 
	//idth += m_List.GetColumnWidth(4); 
	iWidth += m_List.GetColumnWidth(5); // 조회수
	iWidth += m_List.GetColumnWidth(6);	// 종목명
	iWidth += m_List.GetColumnWidth(7);	// 정부구분 

	m_List.SetColumnWidth( 4, iw-iWidth-17 );
}

LONG CMapDlg::OnWxEditMessage(WPARAM wParam, LPARAM lParam) 
{
	if(WXEDIT_ENTER == wParam)
	{
		RequestTitleList(TRUE);
		m_TreeWnd.UnselectCurrentItem();		
	}

	return 0;
}


void CMapDlg::WriteLog(CString slog)
{
	CFile pFile;
	CString sfile = "c:\\newsreal.log";

	if (pFile.Open(sfile, CFile::modeReadWrite))
	{
		const	ULONGLONG	pos = pFile.SeekToEnd();
		pFile.Seek((long) pos, CFile::begin);
	}
	else
		pFile.Open(sfile, CFile::modeCreate|CFile::modeReadWrite);

	pFile.Write(slog, slog.GetLength() );
	pFile.Close();

}

BOOL CMapDlg::IsAddAlert()
{
	if( IsNewsCountTR( m_nTrkey ) ) // 조회 수 카운 트 할 때 
		return FALSE;

	CString str;
	m_clSearchEdit.GetWindowText( str ); // 서치 에디트에 문자가 있을 때 
	if( str != "" )
		return FALSE;	

	return TRUE;
}

//2012.01.19 KSJ Alertx 추가
BOOL CMapDlg::IsAddAlertx()
{
	if( IsNewsCountTR( m_nTrkey ) ) // 조회 수 카운 트 할 때 
		return FALSE;
	
	CString str;
	m_clSearchEdit.GetWindowText( str ); // 서치 에디트에 문자가 있을 때 
	if( str != "" )
		return FALSE;	
	
	return TRUE;
}
//2012.01.19 KSJ Alertx 추가 끝

void CMapDlg::OnNcDestroy() 
{
	CDialog::OnNcDestroy();
	
	// TODO: Add your message handler code here	
}

int CCommandThread::Run()
{
	return 1;
}

BOOL CMapDlg::makeSubCod( mod_content* mod, CString& strRet, CStringArray& arrSubCod )
{	
	int iCount = 0;
	CString strTemp = "종목 : ";

	for( int j = 0; j < 10; j++ )
	{
		CString strSubcod = CString(mod->subcod[j],12);
		strSubcod.TrimRight();
		if( strSubcod != "" )
		{
			CString strName;
			if( findSubCodName( strSubcod.Mid(1), strName ) )
			{
				iCount++;
				strTemp += strName + ", ";
				arrSubCod.Add( strName );
			}
		}		
	}

	strRet = "";
	strRet += "<font face=\"굴림체\" style=\"font-size:9pt;line-height:1.3em\"><b>";
	strRet += "&nbsp;&nbsp;" + strTemp.Left(strTemp.GetLength()-2);
	strRet += _T(" </b></font><br><br> ");

	if( iCount == 0 )
		return FALSE;
	else
		return TRUE;
}

BOOL CMapDlg::findSubCodName( const CString& strCode, CString& strName )
{
	strName = "";
	for( int i = 0 ; i < m_sortCode.GetSize(); i++ )
	{
		if( m_sortCode.GetAt(i).cod.Find(strCode) >= 0 )
		{
			strName = m_sortCode.GetAt(i).name;
			return TRUE;
		}
	}
	return FALSE;
}

void CMapDlg::convertLink(CString& str, const CStringArray& arrSubCod )
{
	CStringArray restictList;
//	CArray<sorting_cod,sorting_cod> addList;
	sorting_cod searchCod;

	restictList.Copy(m_restrictArray);

	/* 서브 종목으로 들어온 종목 검색 제한 종목에서 제거 */
	for( int j = 0; j < arrSubCod.GetSize(); j++ )
	{
		CString strFindKeyword = arrSubCod.GetAt(j);
		for( int k = 0; k < restictList.GetSize(); k++ )
		{
			if( restictList.GetAt(k) == strFindKeyword )
			{
				restictList.RemoveAt( k );
			}
		}		
	}
	/* 서브 종목으로 들어온 종목 검색 제한 종목에서 제거 */

	/* 종목 코드 전체 검색 */
	int iFind = 0;
	int len = str.GetLength(); 

	for( int k = 0; k < m_sortCode.GetSize(); k++ )
	{
		sorting_cod sc = m_sortCode.GetAt(k);

		BOOL bSearchIt = TRUE; // 검색 여부 		
		for( int j = 0; j < restictList.GetSize(); j++ )
		{
			CString strFind = restictList.GetAt(j);

			if( strFind == sc.name)	// 종목 이름이 제한 리스트에 포함 되어 있다면 검색 안한다
			{
				bSearchIt = FALSE;				
				searchCod = sc;
			}
		}

	/*	if( bSearchIt == FALSE )
			continue;
		searchCod = sc;*/


		if( bSearchIt ) // 검색
		{	
			BOOL bFind = FALSE;
			iFind = 0;
			do {
				iFind = str.Find(sc.name, iFind);
				const int addlen = sc.name.GetLength();
				
				if( iFind >= 0 )
				{
					BOOL bReplace = TRUE;
					const TCHAR cFirst = str.GetAt(iFind-1);
					const TCHAR cLast = str.GetAt(iFind+sc.name.GetLength());

					if( cFirst == '>' )					
						if( str.Mid(iFind-3,3) != "br>" )
							bReplace = FALSE;					
					if( cFirst == '=' ) bReplace = FALSE;
					if( cLast == '\"' ) bReplace = FALSE;
					if( cLast != '(' ) bReplace = FALSE;	

					if( bReplace ) 
					{		
						CString strOldLeft, strOldRight;
						strOldLeft = str.Left( iFind );
						strOldRight = str.Right( len-iFind-addlen );

						CString strLink;
						strLink.Format("<A style=\"text-decoration:none\" href=\"axis://menu/%s\">%s</A>"
							,sc.cod, sc.name );

						str = strOldLeft + strLink + strOldRight;
						iFind += 56;
						len = str.GetLength();

						bFind = TRUE;
					}
					iFind++;
				}					
			} while(iFind>=0);	
			
			if( bFind ) // 검색이 되었다면 제한 리스트에 넣는다 다시 검색하지 않는다.
				restictList.Add( sc.name );
		}
		else // 종목 이름이 제한 리스트에 있다면 형태소 검색 한다.
		{
			int find = 0;	 
			do {
				find = str.Find( searchCod.name, find );
				if( find >= 0 )
				{
					BOOL bReplace = TRUE;
					const TCHAR cFirst = str.GetAt(find-1);
					// 전치사 룰 
					// >,= 이면 안된다
					// 공백,쉼표,",- 이면 된다 

					if( cFirst == '>' )
						if( str.Mid(find-3,3) != "br>" )
							bReplace = FALSE;
					if( cFirst == '=' ) bReplace = FALSE;
					if( cFirst == ' ' || cFirst == ',' || cFirst == '\"' || cFirst == '-' || cFirst == '/' )
						bReplace = FALSE;

					if( bReplace )
					{
						const int addlen = searchCod.name.GetLength();
						CString strTempFind = str.GetAt( find+addlen );
						for( int l = 0; l < m_wordArray.GetSize(); l++ ) // 접미사 리스트 검색 
						{
							if( strTempFind == m_wordArray.GetAt(l) )
							{
								CString strOldLeft, strOldRight;
								strOldLeft = str.Left( find );
								strOldRight = str.Right( len-find-addlen );

								CString strLink;
								strLink.Format("<A style=\"text-decoration:none\" href=\"axis://menu/%s\">%s</A>"
									,searchCod.cod, searchCod.name );

								str = strOldLeft + strLink + strOldRight;
								find += 56;
								len = str.GetLength();
							}
						}
					}
					find++;
				}
			} while(find>=0 && find < len-10 );		

			find = 0;
		} // bSearchIt == FALSE
	} // For End
}

////////////////////////// 
// 함수명	    : CMarketDlg::MoveCtrl
// 설 명	    : 다이얼로그에 컨트롤을 움직이는 기능
// 작성일시	    : 2009/06/02
// 작성자       : kbkim
// 파라미터	    : UINT nCtrlID : 움직이고자 하는  컨트롤 ID
//				  int nCx : 움직이고자 하는 X 크기(음수:왼쪽으로)
//				  int nCy : 움직이고자 하는 Y 크기(음수:위쪽으로)
// 리턴 값 		: 여백을 포함한 오른쪽 X좌표
// 관련요건	    : 
////////////////////////// 
int CMapDlg::MoveCtrl(UINT nCtrlID, int nCx, int nCy, int nCtrlWidthGap)
{
	CRect rCtrl;
	
	GetDlgItem(nCtrlID)->SetWindowPos(NULL, nCx, nCy, 0, 0, SWP_NOSIZE);
	GetDlgItem(nCtrlID)->GetWindowRect(rCtrl);
	ScreenToClient(&rCtrl);
	
	return int(rCtrl.right) + nCtrlWidthGap;
}

int CMapDlg::MoveCtrl(CWnd* pWnd, int nCx, int nCy, int nCtrlWidthGap)
{
	CRect rCtrl;
	
	pWnd->SetWindowPos(NULL, nCx, nCy, 0, 0, SWP_NOSIZE);
	pWnd->GetWindowRect(rCtrl);
	ScreenToClient(&rCtrl);
	
	return int(rCtrl.right) + nCtrlWidthGap;
}

void CMapDlg::LoadCode()
{
	BOOL bSort = FALSE;

	CString strPath;
	strPath.Format("%s/tab/hjcode3.dat", m_szRootDir );

	CFile file;
	if( !file.Open(strPath,CFile::modeRead|CFile::typeBinary) )
	{
		return;
	}

	const CTime time = CTime::GetCurrentTime();
	CString strCur;
	strCur.Format("%04d%02d%02d",time.GetYear(),time.GetMonth(),time.GetDay());

	CStdioFile tFile;
	CString strtPath;
	strtPath.Format("%s/tab/newskey", m_szRootDir);
	if( !tFile.Open( strtPath, CFile::modeReadWrite|CFile::typeText) )
	{
		bSort = TRUE;		
		if( !tFile.Open( strtPath, CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			file.Close();
			return;
		}
	}
	else
	{
		CString strRead;
		tFile.ReadString( strRead );

		if( strCur == strRead )		
			bSort = FALSE;
		else
		{
			tFile.Close();
			if( !tFile.Open( strtPath, CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
			{
				file.Close();
				return;				
			}
			bSort = TRUE;
		}
	}

	if( bSort )
	{
		ULONGLONG length = file.GetLength();
		std::unique_ptr<char[]> pBuf = std::make_unique<char[]>(length);

		file.Read(pBuf.get(), length);
		file.Close();

		//length /= sizeof(struct hjcode);
		length /= sizeof(struct hjcodex);

		CString code, hnam;

		CMap<int, int, CStringArray*, CStringArray*> lengthMap;
		lengthMap.InitHashTable(50);

		for (int i = 40; i >= 2; i--)
		{
			std::unique_ptr<CStringArray> pArr = std::make_unique<CStringArray>();
			lengthMap.SetAt(i, pArr.release());
		}

		struct hjcodex* hjc = (struct hjcodex*)pBuf.get();
		for (int i = 0; i < (int)length; i++, hjc++)
		{
			code = CString(hjc->code, HCodeLen);
			if (code.GetAt(0) == 'A' || code.GetAt(0) == 'Q')
			{
				code = code.Mid(1);
				code.TrimRight();

				hnam = CString(hjc->hnam, HNameLen);
				hnam = hnam.Mid(1);
				hnam.TrimRight();

				const int nameLen = hnam.GetLength();

				CStringArray* pArr;
				if (lengthMap.Lookup(nameLen, pArr))
				{
					pArr->Add(hnam);
					pArr->Add(code);

					lengthMap.SetAt(nameLen, pArr);
				}
			}
		}

		for (int i = 40; i >= 2; i--)
		{
			CStringArray* pArr;
			lengthMap.Lookup(i, pArr);
			if (pArr->GetSize() > 1)
			{
				for (int k = 0; k < pArr->GetSize() - 1; k += 2)
				{
					sorting_cod sc;
					sc.name = pArr->GetAt(k);
					sc.cod = pArr->GetAt(k + 1);

					m_sortCode.Add(sc);
				}
			}

			delete pArr;
		}

		// write sort
		strCur += "\n";
		tFile.SeekToBegin();
		tFile.WriteString(strCur);
		for (int i = 0; i < m_sortCode.GetSize(); i++)
		{
			sorting_cod tsc = m_sortCode.GetAt(i);
			CString strTemp;
			strTemp.Format("%s|%s\n", tsc.cod, tsc.name);
			tFile.WriteString(strTemp);
		}
		tFile.Close();
		//ULONGLONG length = file.GetLength();
		//char* pBuf = new char[length];

		//file.Read( pBuf, length );
		//file.Close();

		//length /= sizeof(struct hjcode);

		//CString code,hnam;

		//CMap<int,int,CStringArray*,CStringArray*> lengthMap;
		//lengthMap.InitHashTable( 50 );

		//for( int i = 40; i >=2 ; i-- )
		//{
		//	CStringArray* pArr = new CStringArray;
		//	lengthMap.SetAt( i, pArr );
		//}
		//
		//struct hjcode* hjc = (struct hjcode*)pBuf;
		//for( int i = 0; i < (int)length; i++, hjc++ )
		//{
		//	code = CString( hjc->code, HCodeLen );
		//	if( code.GetAt(0) == 'A' || code.GetAt(0) == 'Q')
		//	{
		//		code = code.Mid(1);
		//		code.TrimRight();

		//		hnam = CString(hjc->hnam,HNameLen);
		//		hnam = hnam.Mid(1);
		//		hnam.TrimRight();

		//		const int nameLen = hnam.GetLength();
		//		
		//		CStringArray* pArr;
		//		if( lengthMap.Lookup( nameLen, pArr ) )
		//		{
		//			pArr->Add( hnam );
		//			pArr->Add( code );

		//			lengthMap.SetAt( nameLen, pArr );
		//		}				
		//	}
		//}		

		//delete [] pBuf;

		//for( int i = 40; i >=2 ; i-- )
		//{
		//	CStringArray* pArr;
		//	lengthMap.Lookup( i, pArr );
		//	if( pArr->GetSize() > 1 )
		//	{
		//		for( int k = 0; k < pArr->GetSize()-1; k+=2 )
		//		{
		//			sorting_cod sc;
		//			sc.name = pArr->GetAt(k);
		//			sc.cod = pArr->GetAt(k+1);

		//			m_sortCode.Add( sc );
		//		}
		//	}

		//	delete pArr;
		//}	

		//// write sort
		//strCur += "\n";
		//tFile.SeekToBegin();
		//tFile.WriteString( strCur );
		//for( int i = 0; i < m_sortCode.GetSize(); i++ )
		//{
		//	sorting_cod tsc = m_sortCode.GetAt(i);
		//	CString strTemp;
		//	strTemp.Format( "%s|%s\n", tsc.cod, tsc.name );
		//	tFile.WriteString( strTemp );			
		//}
		//tFile.Close();		
	}
	else
	{
		file.Close();
		// load sorting file
		CString strTemp;
		while( tFile.ReadString( strTemp ) )
		{
			const int iFind = strTemp.Find( "|" );
			sorting_cod sc;
			sc.cod = strTemp.Left(iFind);
			sc.name = strTemp.Right( strTemp.GetLength()-iFind-1 );

			m_sortCode.Add( sc );
		}	
		tFile.Close();
	}

	CStdioFile rFile;
	CString strRPath;
	strRPath.Format("%s/tab/news_restrict_list", m_szRootDir );
	if( rFile.Open(strRPath,CFile::modeRead|CFile::typeText) )
	{
		CString strTemp2;
		while( rFile.ReadString(strTemp2) )
		{
			if( strTemp2.Left(4) == "LAST" )			
			{
				m_wordArray.Add(strTemp2.Right(strTemp2.GetLength()-5));			
			}
			else			
				m_restrictArray.Add( strTemp2 );
		}
		rFile.Close();
	}
}


void CMapDlg::OnSelchangeFontsize() 
{
	CFile	file;
	CString strSize;
	
	m_pFontSize.GetWindowText(strSize);
	const int nNewSize = atoi((LPCTSTR)strSize);
	
	CString strOldTag(_T(""));
	CString strOldTag_server(_T("font-size:12px"));
	CString strNewTag(_T(""));

	if (!file.Open(m_szFileName, CFile::modeReadWrite | CFile::shareDenyWrite))
	{
		return;
	}
	else
	{
		CString strData(_T(""));
		BYTE buffer[4096] = { 0, };
		DWORD dwRead{};
		int nLength = 0;
		
		do
		{
			dwRead = file.Read(buffer, 4096);
			strData += CString((char*)buffer, sizeof(buffer));
			memset(&buffer, 0, 4096);
			nLength += (int)dwRead;
		}
		while (dwRead > 0);
		
		strNewTag.Format("font-size:%dpt", nNewSize);
	
		const int startStr = strData.Find("font-size:");
		if(startStr != -1)
		{
			strOldTag = strData.Mid(startStr, 14);
		}
		else
		{
			strOldTag.Format("font-size:9pt"); 
		}
		
		if(strOldTag.Right(1) != "t")
		{
			if(strOldTag.Right(1) != "x")
			{
				strOldTag = strOldTag.Left(13);
			}
		}
		
		strData.Replace((LPCTSTR)strOldTag, (LPCTSTR)strNewTag);
		strData.Replace((LPCTSTR)strOldTag_server, (LPCTSTR)strNewTag);
		nLength = nLength + (strNewTag.GetLength() - strOldTag.GetLength());
		strData = strData.Left(nLength+1);

		file.SeekToBegin();
		file.Write((LPCTSTR)strData, strData.GetLength());
		file.Close();

		selectTab(select_tab);

		//SetTimer(100,20,NULL);

		//m_nSize = nNewSize;
	}	
}

void CMapDlg::OnCheckSise() 
{
	TransformWnd();
}

void CMapDlg::TransformWnd(BOOL bFlag)
{
	if(m_btnSise.GetCheck() == 0)
	{
		CRect brwRect;
		
		m_pMapWnd2->GetWindowRect(brwRect);
		ScreenToClient(brwRect);
		
		if (m_bUsingEdgeBrowser)
		{
			if (browserEdge && browserEdge->IsWindowVisible())  //test edge
				browserEdge->MoveWindow(brwRect);
		}
		else
			m_pBrowser->MoveWindow(brwRect);
	
		//m_pMapWnd3->closeMap();
		
		m_pMapWnd3->MoveWindow(CRect(0,0,0,0));

		m_pMapWnd3->ShowWindow(SW_HIDE);

		m_bCodeNews = FALSE;
	}
	
	if(bFlag == FALSE)
	{
		CString strINI(_T(""));
		
		strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
		
		WriteConfigFileInt(strINI, "IB771000", "SISE", m_btnSise.GetCheck());
	}
}

void CMapDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CMapDlg::OnButtonRcvsize() 
{
	CString strINI(_T(""));
	//char readB[256]{};
	// int readL = 0;
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);

	CWnd* parent = GetParent();

	CString strW,strH;
	
// 	readL = GetPrivateProfileString("IB771000", "WIDTH", "0", readB, sizeof(readB), strINI);
// 	strW = CString(readB, readL);
// 	
// 	readL = GetPrivateProfileString("IB771000", "HEIGHT", "0", readB, sizeof(readB), strINI);
// 	strH = CString(readB, readL);
	
	const int top = GetPrivateProfileInt("IB771000", "TOP", 100, strINI);
	int left = GetPrivateProfileInt("IB771000", "LEFT", 0, strINI);

	if(left > 1280)
	{
		left = left - 1280;
	}

	if(parent)
	{
		CWnd* pparent = parent->GetParent();
		
		if(pparent)
		{
			CRect rect;
			// 
			pparent->GetWindowRect(&rect);


			if(rect.Width() == 827 && rect.Height() == 620)
				return;
			
// 			rect.left = left;
// 			rect.top = top;
// 			rect.right = rect.left + 890;
// 			rect.bottom = rect.top + 620;
			
			//parent->MoveWindow(rect);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(821, 596) );
		}
	}
}

void CMapDlg::OnButtonViewmode() 
{
	if(m_bColShow == FALSE)
	{
		m_bColShow =TRUE;

		m_nColWidth1 = m_List.GetColumnWidth(2);
		m_nColWidth2 = m_List.GetColumnWidth(5);
		m_nColWidth3 = m_List.GetColumnWidth(6);
		m_nColWidth4 = m_List.GetColumnWidth(7);
		
		m_List.SetColumnWidth(2,0);
		m_List.SetColumnWidth(5,0);
		m_List.SetColumnWidth(6,0);
		m_List.SetColumnWidth(7,0);
		
		const int iWidth = m_List.GetColumnWidth(0) + m_List.GetColumnWidth(3) + m_List.GetColumnWidth(4);
		
		m_List.GetWindowRect(&m_rectListOrg);
		ScreenToClient(&m_rectListOrg);

		m_pTab->GetWindowRect(&m_rectTabOrg);
		ScreenToClient(&m_rectTabOrg);
		
// 		CString s;
// 		s.Format("%d / %d\n",iWidth,m_rectListOrg.Width());
// 		OutputDebugString(s);

		if(iWidth < m_rectListOrg.Width())
		{
			int nW = m_List.GetColumnWidth(4);
			
			nW = nW + (m_rectListOrg.Width() - iWidth);
			
			m_List.SetColumnWidth(4,nW);
		}
		
		m_btnSise.SetCheck(FALSE);
		m_btnSise.ShowWindow(SW_HIDE);

		m_pTab->ShowWindow(SW_HIDE);
		
		//OnCheckSise();
		TransformWnd(TRUE);
		
		ResizeTransformControl(TRUE);

		if(m_bColShow && m_List.GetItemCount() < 25)
		{
			m_bInitReq = TRUE;
			
			SendMessage( WM_REQUEST_NEXT);
		}
	}
	else
	{
		m_bColShow =FALSE;

		m_List.SetColumnWidth(2,m_nColWidth1);
		m_List.SetColumnWidth(5,m_nColWidth2);
		m_List.SetColumnWidth(6,m_nColWidth3);
		m_List.SetColumnWidth(7,m_nColWidth4);

		m_btnSise.ShowWindow(SW_SHOW);

		m_pTab->ShowWindow(SW_SHOW);

		CRect rect;

		GetClientRect(&rect);

		m_rectListOrg.right = rect.right - 8;

		m_rectTabOrg.right = rect.right - 8;

		m_List.MoveWindow(m_rectListOrg);
		m_pTab->MoveWindow(m_rectTabOrg);

		CString strINI(_T(""));
		CString strSise;
		char readB[256]{};
		int readL = 0;
		
		strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
		
		readL = GetPrivateProfileString("IB771000", "SISE", "1", readB, sizeof(readB), strINI);
		strSise = CString(readB, readL);
		
		m_btnSise.SetCheck(atoi(strSise));

		/*OnCheckSise();*/
		TransformWnd(TRUE);

		ResizeControl();
	}
	
	Invalidate();	
}

void CMapDlg::OnStaticTitle() 
{
	// TODO: Add your control notification handler code here
	
}

void CMapDlg::OnButtonScrap()
{
	const int nIndex = m_List.GetSelectedItem();
	ScrapNews(nIndex);
}

void CMapDlg::UploadEdgeEnv(CString sProgramName)
{
	struct _pidouini_item {
		char	usid[8];				// 사용자 ID
		char	innm[100];				// INI 파일명
		char	senm[100];				// 섹션명
		char	skey[100];				// KEY
		char	valu[2000];				// VALUE
		char	date[8];				// 업데이트 일자
	};

	struct _pidouini_mid {
		char	gubn[1];				//구분 'Q': 조회, 'I': 입력
		struct	_pidouini_item item;
	};

	CString strINI(_T("")), strW, stime;
	CString strEdgeEnv, stmp;
	char readB[256]{};
	int readL = 0;

	strINI.Format("%s\\tab\\axis.ini", m_szRootDir, m_szUser);
	readL = GetPrivateProfileString("EDGE", "UpLoadDate", "0", readB, sizeof(readB), strINI);
	strW = CString(readB, readL);

	CTime	time;
	time = time.GetCurrentTime();
	stime.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay()); 

	readL = GetPrivateProfileString("MODE", "dev", "0", readB, sizeof(readB), strINI);
	stmp.Format("%s", readB);
	stmp.Trim();

	wil::unique_cotaskmem_string version_info;
	HRESULT hr = GetAvailableCoreWebView2BrowserVersionString(nullptr, &version_info);
	stmp.Format("\r\n [edge][szSubKey]  version_info=[%s]", CString(version_info.get()));
	OutputDebugString(stmp);

	BOOL binstalled{};
	if (CString(version_info.get()).IsEmpty())
		binstalled = FALSE;
	else
		binstalled = TRUE;

	if (atoi(strW) >= atoi(stime) && stmp != "1")   //개발기는 계속 저장
		return;

	//HKEY hKey;
	////컴퓨터\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Microsoft EdgeWebView
	//if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	//{
	//	char szSubKey[MAX_PATH];
	//	DWORD dwIndex = 0;
	//	DWORD dwSize = MAX_PATH;

	//	//해당레지스트리의 하위키를 열거 한다. 
	//	while (RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
	//	{
	//		HKEY hSubKey;
	//		if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
	//		{
	//			OutputDebugString("\r\n-------------------------------------------------------------");
	//			stmp.Format("\r\n szSubKey=[%s], dwindex=[%d]", szSubKey, dwIndex);
	//			//OutputDebugString(stmp);
	//			char szDisplayName[MAX_PATH];
	//			DWORD dwSize = sizeof(szDisplayName);

	//			//패키지명 버전 확인
	//			if (RegQueryValueEx(hSubKey, _T("Displayname"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayName), &dwSize) == ERROR_SUCCESS)
	//			{
	//				CString strDisplayName(szDisplayName);
	//				if (strDisplayName.Find(sProgramName) >= 0)
	//				{
	//					binstalled = TRUE;
	//					char szDisplayVersion[MAX_PATH];
	//					dwSize = sizeof(szDisplayVersion);
	//					if (RegQueryValueEx(hSubKey, _T("DisplayVersion"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szDisplayVersion), &dwSize) == ERROR_SUCCESS)
	//					{
	//						CString strDisplayVersion(szDisplayVersion);
	//						strEdgeEnv += "DisplayVersion = ";
	//						strEdgeEnv += strDisplayVersion;
	//						strEdgeEnv += "|";
	//					}

	//					char szInstallDate[MAX_PATH];
	//					dwSize = sizeof(szInstallDate);
	//					if (RegQueryValueEx(hSubKey, _T("InstallDate"), nullptr, nullptr, reinterpret_cast<LPBYTE>(szInstallDate), &dwSize) == ERROR_SUCCESS)
	//					{
	//						CString szInstallDate(szInstallDate);
	//						strEdgeEnv += "InstallDate = ";
	//						strEdgeEnv += szInstallDate;
	//						strEdgeEnv += "|";
	//					}

	//					stmp.Format("\r\n [edge][szSubKey]  strEdgeEnv=[%s]", strEdgeEnv);
	//					OutputDebugString(stmp);
	//					break;
	//				}
	//			}

	//		}
	//		dwSize = MAX_PATH;
	//		dwIndex++;
	//	}//while
	//}//if

	if(binstalled)
		WritePrivateProfileString(_T("EDGE"), sProgramName, "1", strINI);
	else
		WritePrivateProfileString(_T("EDGE"), sProgramName, "0", strINI);

	CString sdat, strLog, userID;
	userID = Variant(userCC, "");
	userID.TrimRight();

	
	stime.Format("%04d%02d%02d %02d:%02d:%02d", time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	sdat.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());

	WritePrivateProfileString(_T("EDGE"), _T("UpLoadDate"), stime.Left(8), strINI);


	std::unique_ptr<char[]> pdata = std::make_unique<char[]>(sizeof(struct _pidouini_mid) + 1);
	memset(pdata.get(), 0, sizeof(struct	_pidouini_mid) + 1);
	struct _pidouini_mid* pmid = (struct	_pidouini_mid*)pdata.get();
	pmid->gubn[0] = 'I';
	memcpy(pmid->item.usid, (LPCSTR)userID, userID.GetLength());
	memcpy(pmid->item.innm, "EDGE", 8);
	memcpy(pmid->item.senm, "EDGE", 8);
	memcpy(pmid->item.skey, "IB771000", 8);
	memcpy(pmid->item.valu, CString(version_info.get()), CString(version_info.get()).GetLength());
	memcpy(pmid->item.date, sdat, 8);
	sendTR("pidouini", pmid, sizeof(struct	_pidouini_mid), type_Edge_Env);	
}

BOOL CMapDlg::IsProgramInstalled(CString sProgramName)
{ 
	BOOL bInstalled{};
	CString strINI(_T(""));
	CString strEdgeEnv, stmp;
	char readB[256]{};
	int readL = 0;

	strINI.Format("%s\\tab\\axis.ini", m_szRootDir, m_szUser);

	CString strW, stime;
	readL = GetPrivateProfileString("EDGE", sProgramName, "0", readB, sizeof(readB), strINI);
	strW = CString(readB, readL);

	if (strW == "1")
		return TRUE;
	else
		return FALSE;
}