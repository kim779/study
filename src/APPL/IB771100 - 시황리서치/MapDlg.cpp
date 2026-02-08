 #include "stdafx.h"
#include "IB771100.h"
#include "MapDlg.h"
#include "../../control/fx_misc/misctype.h"
#include "../../axis/axMsg.hxx"
//#include "../../axis/childfrm.h"
//#include "../../axis/schild.h"
#include "SearchWordSetDlg.h"
#include "OptionWnd.h"
#include "MarketDlg.h"
#include "ViewChkDlg.h"

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
	ofstream dFile("C:\\IB771100.trc",ios::app);
#endif

const char* szFilterMapName2 = _T("IB771103");

#pragma warning(disable:4800)		// m_bSelect가 BOOL, compile시 warning안나도록

#define IDC_CODECTRL	1003		// code control id
#define IDC_HTMLVIEW	1000		// browser id

#define SIZE_MINLIST	80
#define SIZE_CONTENT	150
#define MAX_WORDSIZE	100
#define	TMID_TOTAL	1000		// Timer
#define	WAITQUERY	1100		// Timer
#define DO_INNER_JOB	1111

#define LENGTH_OF_LINE  130
#define	MAXW			627
static	int g_htmlindex = 0;		// 뉴스 내용 저장 index

#define		BACKINDEXCOLOR			64
#define		TOOLINDEXCOLOR			66

#define NEWS_SIZEGUBN 20

/////////////////////////////////////////////////////////////////////////////

CMapDlg::CMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapDlg::IDD, pParent)
{
	m_iSelectSubGubn = -1;
	m_comboSearch.SetParentWnd(this);
	m_List.SetParentWnd(this);
	m_List.SetNewsTypeColumn(0);
	m_FilterResultListCtrl.SetParentWnd(this);
	m_FilterResultListCtrl.SetNewsTypeColumn(0);

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
	m_pTab = NULL;
	m_pMapWnd1 = NULL;
	m_pMapWnd2 = NULL;


	m_bTriggerTR = false;

	m_nTreeWidth	= TREE_WIDTH;

	m_bCodeTrigger = FALSE;
	m_pRadioTot	= NULL;
	m_pRadioSel	= NULL;

	m_rcBorderTree.SetRect(0, 0, 0, 0);
	m_nOptHeight	= SIZE_BUTTONS;
	m_bOptCheck	= FALSE;
	m_pOptionWnd	= NULL;

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

	m_pFilterWnd = NULL;
	
	m_bCategory = FALSE;
	m_iTabIndex = 0;
	m_selHistory.RemoveAll();

	m_pThirdMap = NULL;
	m_strHeadertitle = "";
	m_strHeaderKey = "";
	m_strHeadercode = "";
	m_iTabCount=0;

	m_bFirstSend = FALSE;
	m_bFlag2Send = false;
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
	//	delete m_code;
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

	m_btOption.DestroyWindow();

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
	DDX_Control(pDX, IDC_FONTSIZE, m_pFontSize);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_pStatic);
	DDX_Control(pDX, IDC_STATIC_TITLE2, m_pStatic2);
	DDX_Control(pDX, IDC_BUTTON_PREVIEW, m_btnPreview);
	DDX_Control(pDX, IDC_EDIT1, m_clSearchEdit);
	DDX_Control(pDX, IDC_TOOLBARPOSBUTTON, m_btToolPos);
	DDX_Control(pDX, IDC_CHECK2, m_btTrg);
	DDX_Control(pDX, IDC_COMBO1, m_cblTrgType);
	DDX_Control(pDX, IDC_TREE, m_TreeWnd);
	DDX_Control(pDX, IDC_BT_RIGHT, m_btnRight);
	DDX_Control(pDX, IDC_BT_LEFT, m_btnLeft);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_comboSearch);
	DDX_Control(pDX, IDC_BUTTON_JONGMOK, m_btnJM);
	DDX_Control(pDX, IDC_BUTTON_VIEWSET, m_btnViewSet);
	DDX_Control(pDX, IDC_BUTTON_SELL, m_btnSell);
	DDX_Control(pDX, IDC_BUTTON_SEARCH_SET, m_btnSSet);
	DDX_Control(pDX, IDC_BUTTON_QUERY, m_btnQuery);
	DDX_Control(pDX, IDC_BUTTON_PRINT, m_btnPrint);
	DDX_Control(pDX, IDC_BUTTON_NEXTPAGE, m_btnNext);
	DDX_Control(pDX, IDC_DATETIMEPICKER, m_TimeCtrl);
	DDX_Control(pDX, IDC_LIST_NEWS, m_List);
	DDX_Control(pDX, IDC_FILTERRESULT_LIST, m_FilterResultListCtrl);
	DDX_Control(pDX, IDC_RADIO_TOTAL, m_radioTotal);
	DDX_Control(pDX, IDC_RADIO_JONGMOK, m_radioJongmok);
	DDX_Control(pDX, IDC_RADIO_TOTAL2, m_radioTotal2);
	DDX_Control(pDX, IDC_RADIO_GONGSI, m_radioGongsi);
	DDX_Control(pDX, IDC_RADIO_SIHWANG, m_radioSihwang);
	DDX_Control(pDX, IDC_STATIC_RADIO1, m_staticRadio);
	DDX_Control(pDX, IDC_STATIC_RADIO2, m_staticRadio2);
	DDX_Control(pDX, IDC_CHKTRIGGER, m_bChkAutoTrigger);
	DDX_Control(pDX, IDC_CHKONLYJONGMOK, m_bChkOnlyjongmok);
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
	ON_NOTIFY(NM_CLICK, IDC_FILTERRESULT_LIST, OnClickFilterListNews)
	ON_MESSAGE(WM_SIZE_WEBBROW, OnWebBrowSize)
	ON_MESSAGE(WM_SIZING_WEBBROW, OnWebBrowSizing)
	ON_NOTIFY(DTN_CLOSEUP, IDC_DATETIMEPICKER, OnCloseupDatetimepicker)
	ON_BN_CLICKED(IDC_RADIO_TOT, OnRadioTot)
	ON_BN_CLICKED(IDC_RADIO_SEL, OnRadioSel)
	ON_BN_CLICKED(IDC_BUTTON_JONGMOK, OnButtonJongmok)
	ON_BN_CLICKED(IDC_BUTTON_VIEWSET, OnButtonViewset)
	ON_BN_CLICKED(IDC_BUTTON_SELL, OnButtonSell)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_SET, OnButtonSearchSet)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_NEXTPAGE, OnButtonNextpage)
	ON_BN_CLICKED(IDC_CONBUTTON, OnSetNewsType)
	ON_BN_CLICKED(IDC_TOOLBARPOSBUTTON, OnToolbarRePosition)
	ON_WM_MOVE()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_STATIC_TITLE, OnStaticTitle)
	ON_BN_CLICKED(IDC_BUTTON_TITLE, OnButtonTitle)
	ON_BN_CLICKED(IDC_RADIO_TOTAL, OnRadioTotal)
	ON_BN_CLICKED(IDC_RADIO_JONGMOK, OnRadioJongmok)
	ON_BN_CLICKED(IDC_RADIO_TOTAL2, OnRadioTotal2)
	ON_BN_CLICKED(IDC_RADIO_GONGSI, OnRadioGongsi)
	ON_BN_CLICKED(IDC_RADIO_SIHWANG, OnRadioSihwang)
	ON_BN_CLICKED(IDC_CHKONLYJONGMOK, OnOnlyJongmok)
	ON_WM_NCDESTROY()
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_FONTSIZE, OnSelchangeFontsize)
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
			m_pBrowser->Navigate2((char *)lParam);			
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
	const LONG	ret = 0;
	//HTREEITEM	hItem;

	try
	{
		switch (wParam)
		{
		case MSG_TREE_CLICK :		// Tree Click	
			{
			}
			break;
		case MSG_TREECHK_CLICK :
//			SaveNodeSelect();
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
//	m_nView = GetConfigFileInt(m_sFile, _T("TOOL"), _T("VIEW"), 0 );
	m_nView = 0;

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
//	WriteConfigFileInt(m_sFile, _T("TOOL"), _T("VIEW"), m_nView);
	WriteConfigFileInt(m_sFile, _T("TOOL"), _T("TRIGGER"), ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck());
	int isel = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	isel = max(isel, 0);
	WriteConfigFileInt(m_sFile, _T("TOOL"), _T("TRIGGER_TYPE"), isel);

	int nNode = 0;
	CString sztmp, sFile;
	sFile.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	
	if(m_radioTotal.GetCheck() == TRUE)
	{
		nNode = 0;
	}
	else if(m_radioJongmok.GetCheck() == TRUE)
	{
		nNode = 1;
	}
	
	sztmp.Format("%d", nNode);
	WritePrivateProfileString("IB771100", "RADIO1", sztmp, sFile);
	
	if(m_radioTotal2.GetCheck() == TRUE)
	{
		nNode = 0;
	}
	else if(m_radioGongsi.GetCheck() == TRUE)
	{
		nNode = 1;
	}
	else if(m_radioSihwang.GetCheck() == TRUE)
	{
		nNode = 2;
	}
	





	sztmp.Format("%d", nNode);
	WritePrivateProfileString("IB771100", "RADIO2", sztmp, sFile);
	
	nNode = m_chkTrigger.GetCheck();
	sztmp.Format("%d", nNode);
	WritePrivateProfileString("IB771100", "CHKTRIGGER", sztmp, sFile);

	CRect rcw;
	m_pParent->GetParent()->GetWindowRect(&rcw);

	sztmp.Format("%d", rcw.Width());
	WritePrivateProfileString("IB771100", "WIDTH", sztmp, sFile);
	sztmp.Format("%d", rcw.Height());
	WritePrivateProfileString("IB771100", "HEIGHT", sztmp, sFile);	
	sztmp.Format("%d", m_bChkAutoTrigger.GetCheck());
	WritePrivateProfileString("IB771100", "AUTOTRIGGER", sztmp, sFile);
	sztmp.Format("%d", m_bChkOnlyjongmok.GetCheck());
	WritePrivateProfileString("IB771100", "ONLYJONGMOK", sztmp, sFile);

// 	CString str;
// 	str.Format("width %d height %d\n", rcw.Width(), rcw.Height());
// 	OutputDebugString(str);
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
	WritePrivateProfileString( _T("SELITEM"), _T("VERSION"), "2",m_sFile);


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

//	TRACE("Cur(%d)Total(%d)\n",iCurPage,iTotalPage);

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
		
//		news.hnam = m_code->GetCodeName( news.code );

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
	case DLL_ALERT:		str = "DLL_ALERT";	break;
	case DLL_ALERTx:	str = "DLL_ALERTx";	break;
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

	m_jobList.AddTail(job);

	m_section.Unlock();
}

void CMapDlg::GetObject(WPARAM wParam, LPARAM lParam )
{
	m_section.Lock();

	BOOL bAdd = FALSE;
	BOOL bAlert = FALSE;

	_jobObj job;

	job.key = HIBYTE(LOWORD(wParam));
	job.msg = LOBYTE(LOWORD(wParam));

	if (job.msg == DLL_OUB)
	{
		if (job.key >= type_gongsi && job.key <= type_news_count_month)
			bAdd = TRUE;

		if (job.key >= TRKEY_MEMO_INSERT && job.key <= TRKEY_MEMO_CHECK)
			bAdd = TRUE;
	}

	//if( job.msg == DLL_ALERT )
	//	if( IsAddAlert() )
	//		bAlert = TRUE;

	if (job.msg == DLL_ALERTx) {
		if (IsAddAlert())
			bAlert = TRUE;
	}

	if( job.msg == DLL_DOMINO  || job.msg == DLL_SETPAL )
		bAdd = TRUE;

	if(job.msg == DLL_TRIGGER)
	{
		parseTrigger((char *) lParam);
	}
	if( bAdd )
	{
		job.len = HIWORD(wParam);
		if( lParam != NULL && job.len > 0 )
			job.msgBuf = CString( (char*)lParam, job.len );
		if( job.msg == DLL_TRIGGER || job.msg == DLL_DOMINO )
			job.msgBuf = CString( (char*)lParam );
	}
	else if( bAlert )
	{
		job.len = 0;
		if (lParam != NULL) {
			if (job.msg == DLL_ALERTx) {
				struct _alertR * alert = (struct _alertR*)lParam;

				job.alert = alert;

				if (alert->code.CompareNoCase("S0000") == 0) {
					//	bAlert = TRUE;
					//else
					//	bAlert = FALSE;
					if (IsAddAlert())
					{
						for (int i = alert->size - 1; i >= 0; i--) {
							parsingAlertx( (DWORD *)alert->ptr[i]);
						}
					}
				}

			}
			else {
				job.msgBuf = CString((char*)lParam);

				CString sRealCode = SplitString(job.msgBuf, "\t");
				sRealCode = Trim(sRealCode);

				if (sRealCode.CompareNoCase("S0000") == 0)
					bAlert = TRUE;
				else
					bAlert = FALSE;
			}

		}

	
	}
	


	//TRACE("%d,%d,%s\n",job.key,job.msg,job.msgBuf);
	AfxTrace("%d,%d\n",job.key,job.msg);

	if( bAdd || bAlert )
		m_jobList.AddTail(job);

	m_section.Unlock();
}

void CMapDlg::DoObject(_jobObj* p )
{
	switch(p->msg)
	{
	case DLL_OUB:
		{
			switch (p->key)
			{
			case TRKEY_MEMO_INSERT:
			case TRKEY_MEMO_SEARCH:
			case TRKEY_MEMO_UPDATE:
			case TRKEY_MEMO_DELETE:
			case TRKEY_MEMO_CHECK:
			{
				CString stmp;
				stmp.Format("[memo][IB771100]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(p->key), p->len);
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

			if(p->key == type_content)
			{
				parsingContent((char*)(LPCTSTR)p->msgBuf, p->len);
			}
			else if ((p->key >= type_gongsi && p->key <= type_total) || p->key == type_category || p->key == type_subtotal)
			{
				parsingTitleList(p->key ,(char*)(LPCTSTR)p->msgBuf, p->len);
				
// 				if(m_bFlag2Send == true)
// 				{
// 					m_List.SetSelectionMark(0);
//  					m_List.SetItemState(0,LVIS_SELECTED , LVIS_SELECTED);
//  					m_List.SetScroll(0, TRUE);
// 					m_bFlag2Send = false;
// 				}
				m_bFirstSend = false;
			}
			else if(p->key == type_symbol)	
			{
				parsingSymbol((char*)(LPCTSTR)p->msgBuf, p->len);				
			}
		}
		break;
	case DLL_ALERT:
		{
			if( IsAddAlert() )
			{
				CStringArray arList;
				SplitString(p->msgBuf, "\n", arList);

				for(int i=arList.GetSize()-1; i>=0; i--)
					parsingAlert(arList[i]);				
			}
		}
		break;


	case DLL_ALERTx:
	{
		//if (IsAddAlert())
		//{
		//	for (int i = p->alert->size - 1; i >= 0; i--) {
		//		parsingAlertx( (DWORD *)p->alert->ptr[i]);
		//	}
		//}
	}
	break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		changeColors(); 	
		break;
	case DLL_SELECT:
		break;
	case DLL_DOMINO:
		{
			bool bMenuNews = false;
			CString szDomino = p->msgBuf;
			while (!szDomino.IsEmpty())
			{
				CString str = Parser(szDomino, "\n"), sym = Parser(str, "\t");
				if( sym.CompareNoCase("1301") == 0)
				{
					m_szCode = str;
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
					m_code->SetEditData(m_szCode);
				OnRadioJongmok();
			}			
		}
		break;
	case DLL_SETFONT:
		break;
	case DLL_TRIGGER:
		{
			if (m_bTotSearch)	// 종목 선택 라디오가 선택 되어 있을 때만 트리거를 받는다 
				break;

			m_bTrigger = true;

			CString tmpS = p->msgBuf; 
			CString	sSymbol;
			sSymbol = Parser(tmpS, "\t");
			
			if (m_bJmyd && sSymbol =="1301" && tmpS.GetLength() == 6)
			{
				if (m_szCode.CollateNoCase(tmpS))
				{
					m_szCode = tmpS;
					if (m_code)
					{
						m_code->SetEditData(m_szCode);
						m_code->SetHistory(GetAxVariant(historyCC, _T("1301")));
					}
					m_bTriggerTR = true;
					OnRadioJongmok();
				}
			}
			else if (!sSymbol.CompareNoCase("DATE"))
			{
				SYSTEMTIME tm;
				m_TimeCtrl.GetTime(&tm); 
				tm.wYear = atoi(tmpS.Left(4));
				tm.wMonth = atoi(tmpS.Mid(4, 2));
				tm.wDay = atoi(tmpS.Mid(6));
				m_TimeCtrl.SetTime(&tm);
				m_TimeCtrl.GetTime(&tm);
				OnRadioJongmok();
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
	GetAxVariant(titleCC, "[7711]미니시황");
	m_clrLineColor[0] = GetAxColor(131);
	m_clrLineColor[1] = GetAxColor(132);

	m_szRootDir = GetAxVariant(homeCC);
	m_szUser = GetAxVariant(nameCC);
	m_pFont = GetAxFont(8,false,"굴림체");
	m_pBFont = GetAxFont(9,true,"굴림체");

	m_sFile.Format("%s/%s/%s/%s", m_szRootDir, USRDIR, m_szUser, NEWS_CHECK_FILE);
	m_clrText = GetAxColor(69);
	m_clrBorder = GetAxColor(65);
	m_br.CreateSolidBrush(m_clrBorder);
	
	SetBrush(GetAxColor(BACKINDEXCOLOR));
	
	initListCtrl();
	initFilterListCtrl();
	initControl();

	if(m_pFilterWnd == NULL) // 시황 설정을 모달리스 형식으로 연다
	{
		m_pFilterWnd = std::make_unique<CMarketDlg>(m_szRootDir, m_szUser, m_pParent, m_pFont, this); 
		m_pFilterWnd->m_arKeyList.Copy(m_arSpecialKeyList);
		m_pFilterWnd->m_arStepColor.Copy(m_arStepColor);
		m_pFilterWnd->m_arNewsList.Copy(m_arNewsList);
		m_pFilterWnd->Create(IDD_MARKET_DLG, this);
	}	

	VisibleSecondTab(SW_HIDE);
	m_pThirdMap->ShowWindow(SW_HIDE);

	LoadScreenView();

//	InitWnd();
	
	HideControl();

	m_bmpBorder.LoadBitmap( IDB_BITMAP8 );

	UpdateData(false);
	
	RequestTitleList();

	SetTimer( DO_INNER_JOB, 500, NULL );

	return TRUE;
}

void CMapDlg::InitWnd()
{
// 	m_btnLeft.ShowWindow(SW_HIDE);
// 	m_btnRight.ShowWindow(SW_HIDE);
// 
// 	CRect rcMain;
// 	GetClientRect(rcMain);
// 
// 	int iw = NEWS_WIDTH+TREE_WIDTH+GAPW;
// 	if(!m_nView) iw -= TREE_WIDTH-GAPW;
	 
//	MoveWindow(0, 0, iw, rcMain.Height());
//	m_pOptionWnd->ShowWindow(SW_HIDE);
	
	CString strINI;
	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	int width{}, height{};
	width = (int)GetPrivateProfileInt("IB771100", "WIDTH", 0, strINI);
	height = (int)GetPrivateProfileInt("IB771100", "HEIGHT", 0, strINI);
	
	if(width == 0 || height == 0)
	{

	}
	else
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(width - 6, height - 24));
	}
	

	Invalidate();
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

// 	if(m_pStatic && pWnd->GetSafeHwnd() == m_pStatic.GetSafeHwnd())		
// 	{
// 		return (HBRUSH)m_hStaticClr;
// 	}

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
	CRect rc{}, toolrect{}, lineRc{};
	GetClientRect(&rc);
	DrawBkgrd(&dc,rc);

	rc.top = rc.top;
	dc.FillSolidRect(rc, GetAxColor(181)); 
	DrawRoundBitmap(&dc, rc, ROUND_PANEL);
	m_pTab->GetWindowRect(&rc);
	ScreenToClient(&rc);

	toolrect.left = rc.left;
	toolrect.right = rc.right;
	toolrect.top = rc.bottom; 
	toolrect.bottom = rc.top + PANEL_HEIGHT + PANEL_HEIGHT; 
	dc.FillSolidRect(toolrect, GetAxColor(66)); 

	CPen pen; 
	pen.CreatePen(PS_SOLID, 0, GetAxColor(165));
	dc.SelectObject(&pen); 
	dc.SetBkMode(TRANSPARENT);

	lineRc = toolrect;
	
	lineRc.left = lineRc.left + 1;
	lineRc.right = lineRc.left + 260;
	lineRc.top = lineRc.top + GAP3;
	lineRc.bottom = lineRc.top + 21;
	

	dc.Rectangle(lineRc);//BUFFET LINE

	lineRc.left = lineRc.left + 1;
	lineRc.right = lineRc.left + 2;
	lineRc.top = lineRc.top + 1;
	lineRc.bottom = lineRc.bottom + -1;
	dc.FillSolidRect(lineRc, GetAxColor(66));	
	
//	pen.DeleteObject();
/*	
	lineRc.left = lineRc.left;
	lineRc.right = lineRc.left + 131;
	lineRc.top = lineRc.bottom + GAP3;
	lineRc.bottom = lineRc.top + 20;
	
	dc.Rectangle(lineRc);//BUFFET LINE
*/
	
///	dc.DeleteDC();	//escdream 추가 확인 필요
}

void CMapDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize (nType, cx, cy);

 	if (!m_pMapWnd1 || !m_pMapWnd2 || !m_pOptionWnd || !m_pThirdMap)
 		return;
	//IB771100 = 758 490 Fixed	        ; news
	
	//m_nWidth  = cx;
	//m_nHeight = cy;
// 	OutputDebugString("OnSize\t");
	ResizeControl();
}

void CMapDlg::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == DO_INNER_JOB )
	{
		WorkObject();
	}
	/*else if( nIDEvent == DO_SIZE )
	{
		SetFrameSize();
		ResizeControl();

		KillTimer(DO_SIZE);
	}*/
	
	CDialog::OnTimer(nIDEvent);
}

void CMapDlg::WorkObject( void )
{
	KillTimer(DO_INNER_JOB);
	m_section.Lock();	

	POSITION pos = m_jobList.GetHeadPosition();
	while( pos )
	{
		_jobObj obj = m_jobList.GetNext( pos );
		DoObject( &obj );		
	}
	m_jobList.RemoveAll();
	
	m_section.Unlock();
	SetTimer(DO_INNER_JOB,500,NULL);
}

BOOL CMapDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	const int nSel = m_pTab->GetCurSel();
	if ((nSel != 1 && nSel != 2) && m_rcBorder.PtInRect(pt))
	{
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		return TRUE;
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
	
	SaveScreenView();		// 화면 와이드 여부 설정

	//SaveNodeSelect();

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
	
	if (m_pFilterWnd)
	{
		if (m_pFilterWnd->m_hWnd && ::IsWindow(m_pFilterWnd->m_hWnd))
			m_pFilterWnd->DestroyWindow();
		//delete m_pFilterWnd;	
		m_pFilterWnd = NULL;
	}

	if(m_pThirdMap)
	{
		if (m_pThirdMap->m_hWnd && ::IsWindow(m_pThirdMap->m_hWnd))
			m_pThirdMap->DestroyWindow();
		delete m_pThirdMap;	m_pThirdMap = NULL;
	}

	///브라우저 폰트사이즈 저장
	//
	CString strINI(_T(""));

	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	
	CString strVal(_T(""));

	strVal.Format("%d", m_pFontSize.GetCurSel());
//	WritePrivateProfileString("NEWSXX", "fontsize", (LPCTSTR)strVal, (LPCTSTR)strINI);  //2020 0326 시황리서치화면에서 폰트저장 막기
	//////////////////////////

	CoUninitialize();

	CDialog::OnDestroy();
}


void CMapDlg::OnClose() 
{
	//DestroyWindow();
	CDialog::OnClose();
}

void CMapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*	int	nSel = m_pTab->GetCurSel();
	if ((nSel != 1 && nSel != 2) && m_rcBorder.PtInRect(point))
	{
		m_YMovepos = m_Ypos = point.y;
		SetCapture();
		ClipCursor(getClipRect());
		drawMove(m_rcBorder);
		m_bCapture = true;
	}
*/
	CDialog::OnLButtonDown(nFlags, point);
}

void CMapDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	const int	nSel = m_pTab->GetCurSel();
	if ((nSel != 1 && nSel != 2) && m_Ypos >= 0)
	{
		drawMove(m_rcBorder);
		m_rcBorder.OffsetRect(0, (point.y - m_YMovepos));
		drawMove(m_rcBorder);
		m_YMovepos = point.y;
	}
   
	CDialog::OnMouseMove(nFlags, point);
}

void CMapDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
/*	if (m_bCapture)
	{
		ReleaseCapture();
		ClipCursor(NULL);
	}

	int	nSel = m_pTab->GetCurSel();
 	if ((nSel != 1 && nSel != 2) && m_Ypos >= 0)
	{
		drawMove(m_rcBorder);
		m_Ypos = -1;
		m_YMovepos = -1;

		CRect	rect;
		GetClientRect(rect);
		m_nHeight = rect.Height();
		m_List.GetWindowRect(rect);
	}

	m_bCapture = false;
*/
	CDialog::OnLButtonUp(nFlags, point);
}


void CMapDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CString str;
	CRect rc;
	GetWindowRect(&rc);
	point.x  +=rc.left;
	point.y  +=rc.top;
	str.Format("IB771110\n%d\t%d\n",point.x,point.y);
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
	OnRadioJongmok();
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
		str.Format("IB771100\n%d\t%d\n", pt.x, pt.y);
	else
		str.Format("IB771100\n%d\t%d\n1301\t%s\n", pt.x, pt.y, szcode);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)str.operator LPCTSTR());
	*pResult = 0;
}

void CMapDlg::OnClickFilterListNews(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_LISTVIEW *pNMListView = (NM_LISTVIEW*)pNMHDR;	
	
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
		
		int cnt = 0;
		
		cnt = m_FilterResultListCtrl.GetItemCount()-1;
		
		if (cnt < 0)	
			return;
		
		NewsInfo pinfo;
		
		if (!m_FilterResultListCtrl.GetNewsInfo(pNMListView->iItem, pinfo) )
			return;
		
		CString strTitle;
		strTitle = m_FilterResultListCtrl.GetItemText(pNMListView->iItem, 4);
		
		CString str;
		strTitle.Replace("/", "-");
		str.Format("IBNEWSXX /t /p5 /d 015\t%s\n016\t%s\n1301\t%s", strTitle, pinfo.sKeyv, pinfo.sCode);
		
		if(m_pParent)
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)str);
		
		m_List.SetFocus();

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
void CMapDlg::OnClickListNews(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;	

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
		
		int cnt = 0;

		cnt = m_List.GetItemCount()-1;

		if (cnt < 0)	
			return;

		NewsInfo pinfo;


		if (!m_List.GetNewsInfo(pNMListView->iItem, pinfo) )
			return;


		CString strTitle;

		strTitle = m_List.GetItemText(pNMListView->iItem, 4);

			
		CString str;
		
		strTitle.Replace("/", "-");
		str.Format("IBNEWSXX /t /p5 /d 016\t%s\n1301\t%s\n015\t%s", pinfo.sKeyv, pinfo.sCode,strTitle);

		if(m_pParent)
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)str);

		if(m_bChkAutoTrigger.GetCheck() == TRUE)
		{
			CString code,msg;

			if(pinfo.sCode.GetLength() > 6)
			{
				code = pinfo.sCode.Mid(1);
			}
			else
			{
				code = pinfo.sCode;
			}

			msg.Format("1301\t%s", code);

			GetAxVariant(triggerCC, msg);
		}

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
		m_radioTotal.SetCheck(TRUE);
		m_radioJongmok.SetCheck(FALSE);
	}
	else
	{
		m_radioTotal.SetCheck(FALSE);
		m_radioJongmok.SetCheck(TRUE);
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
	m_bTotSearch = false;
	if (m_bTotSearch)
	{
		m_radioTotal.SetCheck(TRUE);
		m_radioJongmok.SetCheck(FALSE);
	}
	else
	{	m_radioTotal.SetCheck(FALSE);
		m_radioJongmok.SetCheck(TRUE);
	}

	m_btnJM.EnableWindow(true);

	if (m_symbol)	
		m_symbol->EnableWindow(true);

	if (m_code)
	{
		m_code->EnableWindow(true);
		if ( m_szCode == "")
		{
			CString sHistory = m_code->m_sHistory;
			CString sCode = Parser(sHistory,"\t");
			if (sCode.GetLength() >= 6)
				m_code->SetEditData( sCode.Left(6) );
			m_code->SetHistory( GetAxVariant(historyCC, _T("1301")));
			m_szCode = m_code->GetEditData();
		}
	}

//	m_clSearchEdit.SetWindowText("");// 종목 선택시 검색어 지움 

	RequestTitleList();
}

void CMapDlg::OnButtonJongmok() 
{
	CPoint pt(152, 50); ClientToScreen(&pt);
	CString nameS = GetAxCode(kospiCODE,MAKELPARAM(pt.x, pt.y));
	CString temp = Parser(nameS, "\t");
	if (m_code && nameS != "")
	{
		m_szCode = temp;
		m_code->SetEditData(m_szCode);
		m_code->SetHistory( GetAxVariant(historyCC, _T("1301") ));
		OnRadioJongmok();
	}
}

void CMapDlg::OnButtonViewset()
{
/*	CViewChkDlg* vDlg = new CViewChkDlg(this, m_pParent);
	
	if(vDlg->DoModal() == IDOK)
	{
		m_bChkAutoTrigger = vDlg->m_bChkAutoTrigger;
		m_bChkOnlyjongmok = vDlg->m_bChkOnlyjongmok;
		
		if(m_bChkOnlyjongmok == TRUE)
		{
			setOnlyJongmok(TRUE);
		}
		else
		{
			setOnlyJongmok(FALSE);
		}

		RequestTitleList();
	}
*/
}

void CMapDlg::ChangeInitSize()
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(283, 345));
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
		mapS = "DH010100/s/p5";
	openMap(typeVIEW, mapS);
}

void CMapDlg::SetCountNewsCtrl( BOOL bShow )
{
	CRect rc;
	m_List.GetWindowRect( rc );
/*
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

		int iWidth = rc.Width() - (18+70+36+90+69)-17;
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

		int iWidth = rc.Width() - (30+18+70+36+90+69)-17;
		m_List.SetColumnWidth(4,iWidth);		
	}
	*/
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
	IStream* pStream = nullptr;
	IUnknown *pUnk = NULL;
	ULONG lWrote = 0;
	LPSTR sMem = NULL;

	VARIANT vHeadStr{}, vFootStr{}, vHeadTxtStream{};
	long rgIndices{};

	if (pStream != nullptr) return;

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
			//goto cleanup;
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
			//goto cleanup;
			throw;
			
		}

		// Argument 2: Footer
		vFootStr.vt = VT_BSTR;
		vFootStr.bstrVal = SysAllocString(L"");
		if (vFootStr.bstrVal == NULL)
		{
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}

		// Argument 3: IStream containing header text. Outlook and Outlook 
		if ((sMem = (LPSTR)CoTaskMemAlloc(512)) == NULL)
		{
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}

		date = CString(m_szCurKey, 8);
		date.Insert(4, "-");	date.Insert(7, "-");
		sprintf(sMem, "<html><font face= %s><span style=\"font-size:%dpt;\"><pre></html>\0", "굴림체", 9);

		// Allocate an IStream for the LPSTR that we just created.
		hG = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, strlen(sMem));
		if (hG == NULL) {
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}
		hr = CreateStreamOnHGlobal(hG, TRUE, &pStream);
		if (FAILED(hr)) {
			TRACE(_T("OnPrint::Failed to create stream from HGlobal: %lX\n"), hr);
			//goto cleanup;
			throw;

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
		//	GetAxVariant(guideCC ,"출력완료." );
		return; //WebBrowser control will clean up the SAFEARRAY after printing.
	}
	catch (CException*) {
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

}

void CMapDlg::OnButtonPreview() 
{
	// TODO: Add your control notification handler code here
	CString	date;
//	GetAxVariant(guideCC ,"인쇄중입니다." );

	SAFEARRAYBOUND psabBounds{};
	SAFEARRAY *psaHeadFoot=NULL;
	HRESULT hr = S_OK;

	HGLOBAL hG = 0;
	IStream* pStream{};
	IUnknown *pUnk = NULL;
	ULONG lWrote = 0;
	LPSTR sMem = NULL;

	VARIANT vHeadStr{}, vFootStr{}, vHeadTxtStream{};
	long rgIndices{};

	try {
		if (!m_pBrowser)
		{
			TRACE(_T("DoPrint: Cannot print - WebBrowser control not ready\n"));
			throw;
			//goto cleanup;
		}

		// Initialize header and footer parameters to send to ExecWB().
		psabBounds.lLbound = 0;
		psabBounds.cElements = 3;
		psaHeadFoot = SafeArrayCreate(VT_VARIANT, 1, &psabBounds);
		if (NULL == psaHeadFoot)
		{
			//goto cleanup;
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
			//goto cleanup;
			throw;
		}

		// Argument 2: Footer
		vFootStr.vt = VT_BSTR;
		vFootStr.bstrVal = SysAllocString(L"");
		if (vFootStr.bstrVal == NULL)
		{
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}

		// Argument 3: IStream containing header text. Outlook and Outlook 
		if ((sMem = (LPSTR)CoTaskMemAlloc(512)) == NULL)
		{
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}

		date = CString(m_szCurKey, 8);
		date.Insert(4, "-");	date.Insert(7, "-");
		sprintf(sMem, "<html><font face= %s><span style=\"font-size:%dpt;\"><pre></html>\0", "굴림", 9);

		// Allocate an IStream for the LPSTR that we just created.
		hG = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, strlen(sMem));
		if (hG == NULL) {
			TRACE(_T("DoPrint: Could not allocate memory in %s: Line %d\n"), __FILE__, __LINE__);
			//goto cleanup;
			throw;
		}
		hr = CreateStreamOnHGlobal(hG, TRUE, &pStream);

		if (pStream == NULL) {
			throw;
		}

		if (FAILED(hr)) {
			TRACE(_T("OnPrint::Failed to create stream from HGlobal: %lX\n"), hr);
			//goto cleanup;
			throw;
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
		SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vFootStr));
		rgIndices = 2;
		SafeArrayPutElement(psaHeadFoot, &rgIndices, static_cast<void*>(&vHeadTxtStream));

		// SAFEARRAY must be passed ByRef or else MSHTML transforms it into NULL.
		VARIANT vArg;
		VariantInit(&vArg);
		vArg.vt = VT_ARRAY | VT_BYREF;
		vArg.parray = psaHeadFoot;
		m_pBrowser->ExecWB(OLECMDID_PRINTPREVIEW, OLECMDEXECOPT_DONTPROMPTUSER, &vArg, NULL);
		//	GetAxVariant(guideCC ,"출력완료." );
		return; //WebBrowser control will clean up the SAFEARRAY after printing.
	}
	catch (CException*) {

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

		return;
	}

/*
cleanup:
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
*/
}

void CMapDlg::OnButtonSearchSet() 
{
	if(m_pFilterWnd == NULL) // 시황 설정을 모달리스 형식으로 연다
	{
		m_pFilterWnd = std::make_unique< CMarketDlg>(m_szRootDir, m_szUser, m_pParent, m_pFont, this); 
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
	}

	ResizeControl();
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

void CMapDlg::OnStaticTitle()
{
	CString str;
	m_strHeadertitle.Replace("/", "-");
	str.Format("IBNEWSXX /t /p5 /d 015\t%s\n016\t%s\n1301\t%s", m_strHeadertitle, m_strHeaderKey, m_strHeadercode);
	
	if(m_pParent)
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)str);
}

void CMapDlg::OnButtonTitle()
{
	CString str;
	m_strHeadertitle.Replace("/", "-");
	str.Format("IBNEWSXX /t /p5 /d 015\t%s\n016\t%s\n1301\t%s", m_strHeadertitle, m_strHeaderKey, m_strHeadercode);
	
	if(m_pParent)
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUPX), (LPARAM)(LPCTSTR)str);
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

	if( IsNewsCountTR( m_nTrkey ) )		RequestCountList();
	else
		RequestTitleList();

	*pResult = 0;
}
//===========================================================================================================
CString CMapDlg::GetAxVariant(int cmd, CString data)
{
	if( !m_pParent )
		return "";

	const char* dta = (char*)m_pParent->SendMessage(WM_USER, 
			MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)	return dta;
	else			return "";
}

CString CMapDlg::GetAxCode(int cmd,LPARAM lParam)
{
	if( !m_pParent )
		return "";

	const char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(cmd, NULL)), lParam);
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
		
		select_tab = 0;		//탭 선택값 0으로 초기화

		if(icnt > 1)
		{
			for(int i=icnt-1; i>0; i--)
				m_pTab->DeleteItem(i);
		}

	}
	else
	{
		m_btnSell.ShowWindow(SW_SHOW);
		if(icnt >= m_arTabNameList.GetSize()) 
			return;

		// 	종목연동 기사일 경우
		m_pTab->SetMinTabWidth(1);
	
		for(int i=1; i<m_arTabNameList.GetSize(); i++)
		{
			m_pTab->InsertItem(m_pTab->GetItemCount(), m_arTabNameList[i]);
		}
	}
}

void CMapDlg::RequestContent(int nIdx)
{
	try
	{
		if (nIdx < 0 || nIdx >= m_List.GetItemCount())
			return;
		
		pnewsrc_mid mid;
		memset(&mid,0x00,sizeof(pnewsrc_mid));
		
		CString szTemp = "";
		
		NewsInfo pnew;
		
		if (!m_List.GetNewsInfo(nIdx, pnew))
			return;
		
		//int keyvL = 24;//sizeof(pnew.sKeyv);
		//if (memcmp(pnew.keyv, m_szCurKey, keyvL) == 0)		//현재 같은 본문내용이 이미 출력되어있음
		//	return;
		if( pnew.sKeyv == CString(m_szCurKey) )
			return;
		
		//		CheckTab(nIdx);
		szTemp = m_List.GetItemText(nIdx, 4); // 제목
		szTemp.TrimRight();	szTemp.Remove(0);
		
		const int keyvL = pnew.sKeyv.GetLength();
		memcpy(mid.titl, (char*)(LPCTSTR)szTemp, title_len);
		memcpy(mid.keyv, (char*)(LPCTSTR)pnew.sKeyv, keyvL);
		memcpy(m_szCurKey, (char*)(LPCTSTR)pnew.sKeyv, keyvL);
		
		sendTR("pnewsrch", &mid, sizeof(mid_content), type_content);
		/*
		if (nIdx < 0 || nIdx >= m_List.GetItemCount())
			return;

		mid_content mid;
		memset(&mid,0x00,sizeof(mid_content));
		
		CString szTemp = "";

		NewsInfo pnew;

		if (!m_List.GetNewsInfo(nIdx, pnew))
			return;

		//int keyvL = 24;//sizeof(pnew.sKeyv);
		//if (memcmp(pnew.keyv, m_szCurKey, keyvL) == 0)		//현재 같은 본문내용이 이미 출력되어있음
		//	return;
		if( pnew.sKeyv == CString(m_szCurKey) )
			return;

//		CheckTab(nIdx);
		szTemp = m_List.GetItemText(nIdx, 4); // 제목
		szTemp.TrimRight();	szTemp.Remove(0);

		int keyvL = pnew.sKeyv.GetLength();
		memcpy(mid.titl, (char*)(LPCTSTR)szTemp, title_len);
		memcpy(mid.keyv, (char*)(LPCTSTR)pnew.sKeyv, keyvL);
		memcpy(m_szCurKey, (char*)(LPCTSTR)pnew.sKeyv, keyvL);
		
		sendTR("pibonews", &mid, sizeof(mid_content), type_content);
		*/
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

	m_List.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, 0);
	m_List.InsertColumn(1, _T("순위"), LVCFMT_CENTER, 0, 1);
	m_List.InsertColumn(2, _T("날짜"), LVCFMT_CENTER, 0, 2);
    m_List.InsertColumn(3, _T("시간"), LVCFMT_CENTER, 38, 3);

    m_List.InsertColumn(4, _T("제목"), LVCFMT_LEFT, 185, 4);

	m_List.InsertColumn(5, _T("조회수"), LVCFMT_RIGHT, 0, 5);
	m_List.InsertColumn(6, _T("종목명"), LVCFMT_LEFT, 0, 6);
    m_List.InsertColumn(7, _T("정보구분"), LVCFMT_CENTER, 0, 7);
	
    m_List.InsertColumn(8, _T("분류명"), LVCFMT_LEFT, 0, 8);
	m_List.InsertColumn(9, _T("idx"), LVCFMT_RIGHT, 0, 9);				// next처리 위함
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

}


void CMapDlg::initFilterListCtrl()
{
	m_txtcolor = GetAxColor(69);
	m_listhdrcolor = GetAxColor(74);
	m_listlinecolor = GetAxColor(65);
	
	m_FilterResultListCtrl.SetnnFont( GetAxFont(9,false,"굴림체") );
	
	m_FilterResultListCtrl.SetTextColor(m_txtcolor);
	m_FilterResultListCtrl.SetBkColor(GetAxColor(68),GetAxColor(77));
	m_FilterResultListCtrl.SetEdgeColor(GetAxColor(65));
	m_FilterResultListCtrl.SetSelectColor(GetAxColor(78));
	m_FilterResultListCtrl.m_HdCtrl.SetTextColor(GetAxColor(76));
	m_FilterResultListCtrl.m_HdCtrl.SetBkColor(m_listhdrcolor);
	m_FilterResultListCtrl.m_HdCtrl.SetEdgeColor(m_listlinecolor);
	
	m_FilterResultListCtrl.InsertColumn(0, _T(""), LVCFMT_CENTER, 0, 0);
	m_FilterResultListCtrl.InsertColumn(1, _T("순위"), LVCFMT_CENTER, 0, 1);
	m_FilterResultListCtrl.InsertColumn(2, _T("날짜"), LVCFMT_CENTER, 0, 2);
    m_FilterResultListCtrl.InsertColumn(3, _T("시간"), LVCFMT_CENTER, 38, 3);
	
    m_FilterResultListCtrl.InsertColumn(4, _T("제목"), LVCFMT_LEFT, 205, 4);
	
	m_FilterResultListCtrl.InsertColumn(5, _T("조회수"), LVCFMT_RIGHT, 0, 5);
	m_FilterResultListCtrl.InsertColumn(6, _T("종목명"), LVCFMT_LEFT, 0, 6);
    m_FilterResultListCtrl.InsertColumn(7, _T("정보구분"), LVCFMT_CENTER, 0, 7);
	
    m_FilterResultListCtrl.InsertColumn(8, _T("분류명"), LVCFMT_LEFT, 0, 8);
	m_FilterResultListCtrl.InsertColumn(9, _T("idx"), LVCFMT_RIGHT, 0, 9);				// next처리 위함
	m_FilterResultListCtrl.InsertColumn(10, _T("isam"), LVCFMT_RIGHT, 0, 10);			// next처리 위함
	m_FilterResultListCtrl.InsertColumn(11, _T("code"), LVCFMT_LEFT, 0, 11);		 	// next처리 위함
	
	m_FilterResultListCtrl.ModifyStyle(0,LVS_SINGLESEL|LVS_SHOWSELALWAYS|WS_CLIPCHILDREN);
	m_FilterResultListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	CFont font;
	font.CreatePointFont(130,"굴림체");
	m_FilterResultListCtrl.SetFont(&font);
	font.DeleteObject();
	
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_szRootDir, IMAGEDIR);
	
//	m_bmpSave.LoadBitmap( IDB_BITMAP6 );
	
	HBITMAP	hbitmap1{}, hbitmap1_dn{}, hbitmap2{}, hbitmap2_dn{};
	hbitmap1 = (HBITMAP)m_bmpSave;//getBitmap(fileIMG + "저장_BTN.bmp");
	hbitmap1_dn = (HBITMAP)m_bmpSave;//getBitmap(fileIMG + "저장_BTN_dn.bmp");
	hbitmap2 = getBitmap(fileIMG + "삭제_BTN.bmp");
	hbitmap2_dn = getBitmap(fileIMG + "삭제_BTN_dn.bmp");
	
	m_FilterResultListCtrl.SetImageType(hbitmap1, hbitmap1_dn, hbitmap2, hbitmap2_dn);
	m_FilterResultListCtrl.SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

}


HBITMAP CMapDlg::getBitmap(CString path)
{
	const CBitmap*	pBitmap = (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

#include "GubnConfigDlg.h"
void CMapDlg::InitTree()
{
	HTREEITEM hItem{}, hRootItem{}, hCateRootItem{}, hMediaRootItem = NULL, hGongsi{};

	m_arNewsList.RemoveAll();

	CString version = GetConfigFileString(m_sFile,_T("SELITEM"),_T("VERSION"),_T(""));

	if(version == "2")
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

		m_arNewsList.Add(NewsList(sTitle, sGubn, icnt, sList1, sList2, ikey++, hItem));
	}

	if(m_selHistory.GetCount() == 0)
	{
		SaveNodeSelect();
	}

	m_TreeWnd.Expand(hGongsi,TVE_EXPAND);

	if (hMediaRootItem != 0)
		m_TreeWnd.Expand(hMediaRootItem,TVE_EXPAND);

	hCateRootItem = m_TreeWnd.InsertGroup("카테고리별", 2, 3, hRootItem);//전체
	m_arNewsList.Add( NewsList("카테고리별", "0", 0, NULL, NULL, type_subtotal, hCateRootItem));
	m_TreeWnd.SetItemState(hCateRootItem,INDEXTOSTATEIMAGEMASK(3),TVIS_STATEIMAGEMASK);

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

		m_arNewsList.Add( NewsList(sTitle, sGubn,type_category,sKey,TRUE, typegroup,hItem));
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
//	CString strGUBN = "111111011111";
	CString strGUBN = "11111101111111110000";
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

	sprintf_s(req.vgbn,"%s", (LPCSTR)strVGBN ); 
	sprintf_s(req.vgbn,"%s", (LPCSTR)strVGBN );
	sprintf_s(req.gubn,"%s", (LPCSTR)strGUBN );
	sprintf_s(req.subg,"%s", (LPCSTR)strCODE );
	sprintf_s(req.sdir,"%s", (LPCSTR)strSORT );
	sprintf_s(req.cpag,"%s", (LPCSTR)strPAGE );
	sprintf_s(req.cpag,"%s", (LPCSTR)strPAGE );

	sendTR("piho7711", &req, sizeof(req_news_count), m_nTrkey, false);
}

void CMapDlg::GetGugnAndSubgMask( char* szGubn, char* szSubg )
{
	if( m_TreeWnd.GetParentItem( select_hItem ) == NULL )
	{
//		memcpy( szGubn, "1111110111111", 13 );
		memcpy( szGubn, "11111101111111110000", 20 );  //test
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}
	else if(m_TreeWnd.GetItemText(select_hItem) != "공시" && m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
	  //memcpy( szGubn, "1011110111111", 13 );
		memcpy( szGubn, "10111101111111110000", 20 ); //test
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}

	//memcpy( szGubn, "1000000000000", 13 );
	memcpy( szGubn, "10000000000000000000", 20 );

	CString strText = m_TreeWnd.GetItemText( select_hItem );
	const _TREEITEM *hParent = m_TreeWnd.GetParentItem( select_hItem );

	for( int i = 0; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList news = m_arNewsList.GetAt(i);
		if( news.sGubnName == strText  && news.bCategory != TRUE )					// TREE TEXT 로 구분이름(뉴스사) 검색 
		{
			for( int k = 1; k < NEWS_SIZEGUBN+1; k++ )	// 구분배열 만듬   
			{
				if( k == news.iGubn )				
					szGubn[k-1] = '1';				
			}

			memcpy( szSubg, "999", 3 );
			break;
		}			

		if( news.hTreeItem == hParent)						// 종목의 부모(뉴스사) 검색 
		{			
			for( int j = 0; j < news.ibulruCnt; j++ )
			{
				if( news.sBulruNameList[j] == strText  && news.bCategory != TRUE )		// TREE TEXT 로 종목 검색 
				{
					for( int k = 1; k < NEWS_SIZEGUBN+1; k++ ) 
					{	
						if( k == news.iGubn )				
							szGubn[k-1] = '1';								
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
//		memcpy( szGubn, "1111110111111", 13 );  //test
		memcpy( szGubn, "11111101111111110000", 20 );
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}
	else if(m_TreeWnd.GetParentItem(m_TreeWnd.GetParentItem( select_hItem )) == NULL)
	{
//		memcpy( szGubn, "1011110111111", 13 ); //test
		memcpy( szGubn, "10111101111111110000", 20 );
		memcpy( szSubg, "999", 3 );

		// 전체 뉴스 config 체크 

		return;
	}

  //memcpy( szGubn, "1000000000000", NEWS_SIZEGUBN );
	memcpy( szGubn, "10000000000000000000", NEWS_SIZEGUBN );//test

	CString strText = m_TreeWnd.GetItemText( select_hItem );
	//const HTREEITEM hParent = m_TreeWnd.GetParentItem( select_hItem );
	
	for( int i = NEWS_SIZEGUBN; i < m_arNewsList.GetSize(); i++ )
	{
		NewsList news = m_arNewsList.GetAt(i);
		if( news.sGubnName == strText && news.bCategory)					// TREE TEXT 로 구분이름(뉴스사) 검색 
		{
			for( int k = 1; k < NEWS_SIZEGUBN+1; k++ )	// 구분배열 만듬   
			{				
				szGubn[k-1] = '1';				
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

	if(bNext == TRUE)
	{
		m_bFirstSend = false;
	}
	else
	{
		m_bFirstSend = true;
	}

	CString	szPortfolio;
	CString szSearch;
	CString szDate;
	CString szGubn;
	CString szFlag;
	CTime time;

/*	if (m_code) 
	{
		m_szCode = m_code->GetEditData();
		m_szCode.TrimLeft();	
		m_szCode.TrimRight();	
		m_szCode.Remove(0);		
	}
*/
	m_szCode = "";

	mid_title req;
	memset(&req, ' ', sizeof(mid_title));
	
	if(m_bChkOnlyjongmok.GetCheck())
	{
		req.type[0] = '1';
	}
	else
	{
		req.type[0] = ' ';
	}

	m_TimeCtrl.GetTime(time);
	szDate = time.Format("%Y%m%d");
	memcpy( req.fday, szDate, sizeof(req.fday) );	
	
	memcpy( req.skey, m_strSearchKey, min(m_strSearchKey.GetLength(),20));	

	if( !m_bTotSearch && m_szCode.GetLength() > 0 /*&& !bKeyword*/ )	
		memcpy(req.cod2,(char*)(LPCTSTR)m_szCode , min(m_szCode.GetLength(), 6) );	

	memcpy( req.gubn, "11111101111111110000", 20 );

	memcpy( req.sugb, "999", 3 );

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
		
		int cnt = 0;

		cnt = m_List.GetItemCount()-1;

		if (cnt < 0)	
			return;

		NewsInfo pinfo;

		if (!m_List.GetNewsInfo(cnt, pinfo) )	return;



		struct _page *page = (struct _page *)&m_szSave;			// m_szSave 와 연동 				
		memcpy(page->isrn, (char*)(LPCTSTR)pinfo.sGisn, sizeof(page->isrn));	// isrn 이 셋팅 되어 올라 가야 여기서 부터 찾는 다. 
		
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
		m_pOptionWnd = std::make_unique< COptionWnd >(this);
		m_pOptionWnd->SetGubn(m_bDaetoo);

		if (m_bDaetoo)
			m_pOptionWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, TREE_WIDTH - 5, OPTION_WND_HEIGHT), this, IDC_OPTION_WND);
		else
			m_pOptionWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, TREE_WIDTH - 5, OPTION_WND_HEIGHT-100), this, IDC_OPTION_WND);

		m_pOptionWnd->Init(m_szRootDir, m_szUser);
	}
	// buttons font
	m_btnSell.SetFont(m_pFont);
	m_btnSSet.SetFont(m_pFont);
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

	
	szn = m_szRootDir + "\\image\\" + "4BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "4BTN_en.BMP";
	szd = m_szRootDir + "\\image\\" + "4BTN_dn.BMP";
	m_btnViewSet.SetSkin(szn,szd,szu);
	m_btnViewSet.SetFont(m_pFont);
	m_btnViewSet.SetWindowPos(NULL,0,0,m_bx,m_by,SWP_NOMOVE);
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

	// escdream 임시 주석처리
	m_code = std::make_unique<CfxCodeCtrl>(this, m_szRootDir + "\\" + "tab" + "\\");//  new CfxCodeCtrl(this, m_szRootDir + "\\" + "tab" + "\\");
	if (m_code->Create(this, CRect(ileft, 3, ileft+65, 23), IDC_CODECTRL))
	{
	//	/*
	//	szn = m_szRootDir + "\\image\\" + "DOWN_EX.BMP";
	//	szd = m_szRootDir + "\\image\\" + "DOWN_EX_dn.BMP";
	//	m_code->SetButtonBitmap(getBitmap(szn), getBitmap(szd));
	//	*/
		m_code->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
		m_code->SetUnit(GU_CODE);
		m_code->SetFont(m_pFont);
	}
	else
	{
	//	delete m_code;		
		m_code = NULL;
	}

	m_btnJM.MoveWindow(ileft+65, 3, 19, 20, TRUE);
	initSymbol();

	CString text = GetAxVariant(historyCC, _T("1301"));
	if (m_code)	m_code->SetHistory(text);
	if (text.GetLength()>6)
	{
		m_szCode = text.Left(6);
		if (m_code)	m_code->SetEditData(m_szCode);
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

	szn = m_szRootDir + "\\image\\" + "4BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "4BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "4BTN_DN.BMP";
	m_btnSSet.SetSkin(szn,szd,szu);
	
	szn = m_szRootDir + "\\image\\" + "조회BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "조회BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "조회BTN_DN.BMP";
	szd = m_szRootDir + "\\image\\" + "조회BTN_DN.BMP";
	m_btnQuery.SetSkin(szn,szd,szu);

	szn = m_szRootDir + "\\image\\" + "2BTN.BMP";
	szu = m_szRootDir + "\\image\\" + "2BTN_EN.BMP";
	szd = m_szRootDir + "\\image\\" + "2BTN_DN.BMP";
	m_btnSell.SetSkin(szn,szd,szu);
	m_btnPrint.SetSkin(szn,szd,szu);
	m_btnNext.SetSkin(szn,szd,szu);

	m_btnTitle.Create("", WS_CHILD | WS_VISIBLE  | BS_LEFT | BS_LEFTTEXT, CRect(0,0,0,0), this, IDC_BUTTON_TITLE);
	
	szn = m_szRootDir + "\\image\\" + "newsbg.BMP";
	szu = m_szRootDir + "\\image\\" + "newsbg.BMP";
	szd = m_szRootDir + "\\image\\" + "newsbg.BMP";
	m_btnTitle.SetSkin(szn,szd,szu);
	m_btnTitle.SetFont(m_pFont);

	m_chkTrigger.Create("연동", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, CRect(0,0,0,0), this, IDC_CHECK_TRIGGER);
	m_chkTrigger.SetFont(m_pFont);
//	m_chkTrigger.SetToolTipText("자동 종목연동 기능");

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

	//필터링설정 체크가 되어있는지 확인
	CString strINI(_T(""));
	CString strFilter;
	char	readB[256] = _T("");
	int readL = 0;
	
	strINI.Format("%s\\User\\%s\\userconf.ini", m_szRootDir, m_szUser);
	
	readL = GetPrivateProfileString(szFilterMapName2, "FILTERING", "0", readB, sizeof(readB), strINI);
	strFilter = CString(readB, readL);

	m_pTab = new CfxTab();
	m_pTab->Create(rcTab, this, IDC_TAB);

	m_pTab->LoadTabImage(m_szRootDir, false);
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CLR_FORM_BACK), 
		GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CLR_FORM_BACK), GetIndexColor(CLR_FORM_BACK));
	m_pTab->SetFont(m_pFont);
	m_pTab->InsertItem(0, "실시간시황");


	if (strFilter == "1")
	{
		m_pTab->LoadTabImage(m_szRootDir, FALSE);
		m_pTab->SetColor(RGB(255, 255, 255), NULL, RGB(0, 0, 0), NULL, RGB(255, 255, 255));
		m_pTab->InsertItem(1, "필터링결과");
	}
	else
	{
	}

	m_pTab->LoadTabImage(m_szRootDir, FALSE);
	m_pTab->SetColor(RGB(255, 255, 255), NULL, RGB(0, 0, 0), NULL, RGB(255, 255, 255));
	m_pTab->InsertItem(2, "필터링설정");

	m_pTab->ChangeBKColor(m_clrRoundColor);
	
	m_pTab->SetStyle(tsIMAGE, TRUE);

	m_iTabCount = m_pTab->GetItemCount();


	GetClientRect(&rcc);
	
	// browser
	m_pBrowser = new CAxBrower();
	m_pBrowser->SetParent(m_pParent);
	rc.bottom -= 30;
	m_pBrowser->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, 
		rc, this, IDC_HTMLVIEW);
	m_pBrowser->ModifyStyleEx(0,WS_EX_CLIENTEDGE,0);
	m_pBrowser->SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	m_pBrowser->Navigate2("about:blank");
//	m_pBrowser->SetSilent(true);


	// 현재가
	m_pMapWnd1 = new CMapWnd(m_pParent);
	m_pMapWnd1->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW);

	// 차트
	m_pMapWnd2 = new CMapWnd(m_pParent);
	m_pMapWnd2->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPVIEW2);
	m_nHeightStd = m_nHeight = rcc.Height();

	// 필터링설정
	m_pThirdMap = new CMapWnd(m_pParent);
	m_pThirdMap->Create(NULL, NULL, WS_CHILD, rc, this, IDC_MAPFILTER);

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
//  	m_pStatic = new CStatic();
//  	m_pStatic.Create("폰트", WS_CHILD | WS_VISIBLE | SS_RIGHT,	rc, this, IDC_STATIC_TITLE);
// 	m_pStatic.SetFont(m_pFont);

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

	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	m_pFontSize.SetCurSel(GetPrivateProfileInt("NEWSXX", _T("fontsize"), 1, (LPCTSTR)strINI));
	
	// Initialize Tree
	InitTree();
	LoadCode();
	


	//라디오 / 체크박스 초기 세팅
	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
/*	readL = (int)GetPrivateProfileInt("IB771100", "RADIO1", 0, strINI);
	
	if(readL == 0)
	{
		m_radioTotal.SetCheck(TRUE);
		m_btnJM.ShowWindow(false);
		
		if (m_symbol)	
			m_symbol->ShowWindow(false);
		
		if (m_code)
		{
			m_code->ShowWindow(false);
		}
	}
	else if(readL == 1)
	{
		m_radioJongmok.SetCheck(TRUE);
		m_btnJM.ShowWindow(true);
		
		if (m_symbol)	
			m_symbol->ShowWindow(true);
		
		if (m_code)
		{
			m_code->ShowWindow(true);
		}
	}

	readL = (int)GetPrivateProfileInt("IB771100", "RADIO2", 0, strINI);
	
	if(readL == 0)
	{
		m_radioTotal2.SetCheck(TRUE);
	}
	else if(readL == 1)
	{
		m_radioGongsi.SetCheck(TRUE);
	}
	else if(readL == 2)
	{
		m_radioSihwang.SetCheck(TRUE);
	}

	readL = (int)GetPrivateProfileInt("IB771100", "CHKTRIGGER", 1, strINI);

	if(readL == 1)
	{
		m_chkTrigger.SetCheck(TRUE);
	}
	else
	{
		m_chkTrigger.SetCheck(FALSE);
	}
*/
	readL = (int)GetPrivateProfileInt("IB771100", "AUTOTRIGGER", 0, strINI);
	if(readL > 0)
	{
		m_bChkAutoTrigger.SetCheck(TRUE);
	}
	else
	{
		m_bChkAutoTrigger.SetCheck(FALSE);
	}
	
	readL = (int)GetPrivateProfileInt("IB771100", "ONLYJONGMOK", 0, strINI);
	if(readL > 0)
	{
		m_bChkOnlyjongmok.SetCheck(TRUE);
	}
	else
	{
		m_bChkOnlyjongmok.SetCheck(FALSE);
	}

	
	strINI.Format("%s\\User\\%s\\u_config.ini", m_szRootDir, m_szUser);
	int width{}, height{};
	width = (int)GetPrivateProfileInt("IB771100", "WIDTH", 285, strINI);
	height = (int)GetPrivateProfileInt("IB771100", "HEIGHT", 325, strINI);
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(width - 6, height - 24));

	CRect mainrc(0,0,width- 6,height- 24);
	MoveWindow(mainrc);

	return true;
}

COLORREF CMapDlg::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

void CMapDlg::HideControl()
{
	m_TreeWnd.ShowWindow(SW_HIDE);
	m_pOptionWnd->ShowWindow(SW_HIDE);
	m_btnSell.ShowWindow(SW_HIDE);
	m_btnSSet.ShowWindow(SW_HIDE);
	m_btnQuery.ShowWindow(SW_HIDE);
	m_btnPrint.ShowWindow(SW_HIDE);
	m_btnNext.ShowWindow(SW_HIDE);
	m_btnJM.ShowWindow(SW_HIDE);
	m_btToolPos.ShowWindow(SW_HIDE);
	m_btnLeft.ShowWindow(SW_HIDE);
	m_btnRight.ShowWindow(SW_HIDE);
	m_pRadioTot->ShowWindow(SW_HIDE);
	m_pRadioSel->ShowWindow(SW_HIDE);
	if (m_code) m_code->ShowWindow(SW_HIDE);
	if (m_symbol) m_symbol->ShowWindow(SW_HIDE);
	m_btnPreview.ShowWindow(SW_HIDE);
	m_btnSell.ShowWindow(SW_HIDE);
	m_btnPrint.ShowWindow(SW_HIDE);
	m_comboSearch.ShowWindow(SW_HIDE);
	m_pBrowser->ShowWindow(SW_HIDE);
	m_pFontSize.ShowWindow(SW_HIDE);
	m_clSearchEdit.ShowWindow(SW_HIDE);
	m_btTrg.ShowWindow(SW_HIDE);
	m_cblTrgType.ShowWindow(SW_HIDE);
	m_TimeCtrl.ShowWindow(SW_HIDE);
	m_staticRadio.ShowWindow(SW_HIDE);
	m_staticRadio2.ShowWindow(SW_HIDE);
	m_pStatic2.ShowWindow(SW_HIDE);
	m_chkTrigger.ShowWindow(SW_HIDE);
	m_btnViewSet.ShowWindow(SW_HIDE);
	m_radioTotal.ShowWindow(SW_HIDE);
	m_radioTotal2.ShowWindow(SW_HIDE);
	m_radioJongmok.ShowWindow(SW_HIDE);
	m_radioSihwang.ShowWindow(SW_HIDE);
	m_radioGongsi.ShowWindow(SW_HIDE);
}

void CMapDlg::SetFrameSize()
{
	int iw = NEWS_WIDTH+TREE_WIDTH+GAPW+120;
	if(!m_nView) iw-= (TREE_WIDTH-2);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iw, m_nHeight) );
}

void CMapDlg::initSymbol()
{
	// 종목 symbol
	CString	text;
	CWnd* (APIENTRY * axCreate)(CWnd*, void*) {};

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
			param.options = _T("/a89/b91/c92/d69/i99");

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

	m_List.InvalidateHedarCtrl();
	m_List.Invalidate();

	m_FilterResultListCtrl.SetTextColor( m_txtcolor);
	m_FilterResultListCtrl.SetBkColor( GetAxColor(68),GetAxColor(77));
	m_FilterResultListCtrl.SetEdgeColor( GetAxColor(65));
	m_FilterResultListCtrl.SetSelectColor( GetAxColor(78));
	m_FilterResultListCtrl.m_HdCtrl.SetTextColor(GetAxColor(76));
	m_FilterResultListCtrl.m_HdCtrl.SetBkColor(GetAxColor(74));
	m_FilterResultListCtrl.m_HdCtrl.SetEdgeColor(GetAxColor(65));

	m_FilterResultListCtrl.InvalidateHedarCtrl();
	m_FilterResultListCtrl.Invalidate();

	Invalidate();
	
	GetDlgItem(IDC_RADIO_SEL)->Invalidate();
	GetDlgItem(IDC_RADIO_TOT)->Invalidate();
	GetDlgItem(IDC_CHECK2)->Invalidate();

	m_symbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
	m_symbol->Invalidate();
	m_code->Invalidate();

	if(m_pFilterWnd)
		m_pFilterWnd->ChangeColors(m_clrBk, m_clrRoundColor);
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
	struct	_userTH* userth = nullptr;

//	wb = new char[L_userTH+datl];
	std::unique_ptr<char[]> wb = std::make_unique<char[]>(L_userTH + datl);

	FillMemory(&wb.get()[0], L_userTH+datl, ' ');

	userth = (struct _userTH *)wb.get();

	CopyMemory(userth->trc, "POOPPOOP", sizeof(userth->trc));
	userth->key  = key;
	userth->stat = US_OOP ;
	CopyMemory(&wb.get()[L_userTH], datb, datl);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)wb.get());
	if (m_code)
	{
		CString data;
		data = m_code->GetEditData();
		data.TrimLeft(); data.TrimRight();data.Remove(0);
		if (!data.IsEmpty() && data.GetLength() == 6 && GetAxVariant(whichCC, data))
		{
			data.Insert(0, "1301\t");
			GetAxVariant(codeCC, data);
			
			if (m_bJmyd && !m_bTrigger)
				GetAxVariant(triggerCC, data);
		}
		else	
			m_code->SetEditData(_T(""));
	}

//	delete[] wb;
}

void CMapDlg::sendTRA(CString trCode, char* datB, int datL, char key)
{
//	char* sendB = new char [L_userTH + datL + 1];
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode); 
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());
//	delete [] sendB;
}

void CMapDlg::sendTR(CString strCode, void *m, int nLen, int key, bool bOOP )
{
	try
	{
		if (strCode.GetLength() != 8)
			return;
		

		std::unique_ptr<char[]> cDataBuffer = std::make_unique<char[]>(2048);

		memset(cDataBuffer.get(), 0x00, 2048);
		//_userTH* userTH = (_userTH*)&cDataBuffer[0];
		_userTH* userTH = (_userTH*)cDataBuffer.get();
		userTH->key = key;
		userTH->stat = NULL;		
		CopyMemory(&userTH->trc[0], &strCode.GetString()[0], 8);
		
		CopyMemory(&cDataBuffer.get()[L_userTH], m, nLen);


		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nLen), (LPARAM)cDataBuffer.get());

		CString sDat, sTmp = _T("");
		if (m_szCode.GetLength() > 0 && bOOP &&!m_bTotSearch) // 코드가 있고, 전체 선택이 아니면 
		{
			sDat.Format("1301%c%s\t1021\t17413\t", 0x7f, m_szCode);
			sendOOP(type_symbol,(char *)sDat.operator LPCTSTR(), sDat.GetLength());
		}
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

	int iselect = -1;

	iselect  = m_List.GetSelectedItem();

	
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
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)(LPCTSTR)sdata );
}

void CMapDlg::parsingContent(char *pBytes, int nBytes)
{
	
	int modL = sizeof(pnewsrc_mod);
	if (nBytes < modL)	return;
	const pnewsrc_mod*	mod = (pnewsrc_mod*)pBytes;
	int lineCnt = atoi(CString(mod->size, sizeof(mod->size)));

	nBytes -= modL - 1; 
	pBytes += modL - 1;
	
	CString titleS;
	titleS.Format("%s", CString(mod->titl, title_len));
	titleS.TrimRight();

/*
	int modL = sizeof(mod_content);
	if (nBytes < modL)	return;
	mod_content*	mod = (mod_content*)pBytes;
	int lineCnt = atoi(CString(mod->size, sizeof(mod->size)));

	nBytes -= modL - 1; 
	pBytes += modL - 1;

	CString titleS;
	titleS.Format("%s", CString(mod->titl, title_len));
	titleS.TrimRight();
*/

	return;
	
	//----------------------------------------------------------------------
/*
	if (g_htmlindex > 100)	g_htmlindex = 0;

	try {	

		CString strDir;
		strDir.Format("%s\\%s\\%s", m_szRootDir, "User", m_szUser); 
		m_szFileName.Format("%s\\%s\\%s%X.html", m_szRootDir, "User", m_szUser, g_htmlindex++);

		CFile	file;
		CFileException filex;
		if (!file.Open(m_szFileName, CFile::modeCreate|CFile::modeReadWrite, &filex))
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

			szWrite = strTitle+strData;
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
			//pBytes += LENGTH_OF_LINE;
			//nBytes -= LENGTH_OF_LINE;  // 2009/08/05 특정 기사 본문의 앞부분이 짤리는 문제 때문에 주석 처리함

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
			
				int iLen = strData.GetLength();

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

//	AfxTrace("parsingContent(OUT)\n");
*/
}

void CMapDlg::parsingTitleList(int nKey, void* buffer, int len)
{
	if(m_nTrkey != nKey) return;

	if (!m_bTriggerTR)	
	{
		m_List.SetFocus();	// 20070518(trigger경우 다른 map의 focus잃어 버리지 않도록 focus처리 안함)
	}	
	m_bTriggerTR = false;

	CString szTemp;
	int value{};
	const mod_title *pmod = (mod_title*)buffer;

	CopyMemory(&m_grido, &pmod->grid_o, sizeof(GRID_O));
	memset(m_szSave, 0x00, 80);
	CopyMemory(m_szSave, pmod->grid_o.save, sizeof(m_szSave));

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
// 		if (isMediaChecked(news))
// 			gridAddRow(news);
		
//		if(isNewsViewChecked(news) && isMediaChecked2(news))
//		if(isNewsViewChecked(news))
		{
			if(ii == 0)
			{	
				if(m_bFirstSend == true)
				{
					m_strHeadercode = news.code;
					m_strHeaderKey = news.keyv;
					m_strHeadertitle = news.titl;

					if(m_strHeadercode.GetLength() > 6)
					{
						m_strHeadercode = m_strHeadercode.Mid(1);
					}
				
 					m_btnTitle.SetFont(m_pBFont);
 					m_btnTitle.SetWindowText(m_strHeadertitle);
				}
			}

			gridAddRow(news);	
		}
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
		m_List.SetSelectionMark(nShow);
		m_List.SetItemState(nShow,LVIS_SELECTED , LVIS_SELECTED);
	}	
	
	if (m_bFlag2Send)
	{
		PostMessage(WM_SEND, EVT_SENDCONTENT, 0);
		
		m_bFlag2Send = FALSE;
		
		m_List.SetSelectionMark(0);
		m_List.SetItemState(0,LVIS_SELECTED , LVIS_SELECTED);
		
		m_List.SetScroll(0, TRUE);
	}

	//스크롤바가 없을 경우 한번더 호출
	const int nMax = m_List.GetScrollLimit(SB_VERT);
	
	if(nMax == 0)
	{
		m_bFlag2Send = TRUE;
		SendMessage(WM_REQUEST_NEXT);
//		RequestTitleList(FALSE, TRUE);
	}
}

void CMapDlg::parsingSymbol(char* datb, int datl)
{
	int	value;
	CString	text;

	text  = CString(datb, datl);
	value = text.Find('\t');
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
				m_code->SetHistory( GetAxVariant(historyCC, _T("1301")));
			}
		
			text   = text.Mid(value);
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (text.GetLength() > 6) ? text.GetLength() : 0);
			m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}
		else
		{
			//text   = text.Mid(value);
			const int len = text.GetLength();

			CString str;
			str.Format( "[%d]:[%s]",len,text );
			//AfxMessageBox(str);

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), len);
			m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}

	}
}


CString CMapDlg::GetNewGubnText(int igubn)
{
	for(int i=0; i<m_arNewsList.GetSize(); i++)
	{
		if(m_arNewsList.GetAt(i).iGubn  ==  igubn )
		{
			return m_arNewsList.GetAt(i).sGubnName;
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
		
// 		bool isFiltering = false;
// 
// 		if(m_iTabCount > 2)
// 		{
// 			if(m_iTabIndex == 1)
// 			{
// 				isFiltering = true;
// 			}	
// 		}

		// 필터링 결과인지 확인
//		if(isFiltering == true)
		{
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

			const bool result = m_pFilterWnd->SetMarketNewsData(&news);
			
			if(result == true)
			{
				FiltergridAddRow(news, false);
			}
		}
//		else
		{
//			if(isNewsViewChecked(news) && isMediaChecked2(news))
			if(isNewsViewChecked(news))
			{
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

				gridAddRow(news, false);

				//헤더에 내용 추가
				CString titleS;
				titleS.Format("%s", CString(news.titl, title_len));
				titleS.TrimRight();	

	// 			m_pStatic.SetFont(m_pBFont);
	// 			SetDlgItemText(IDC_STATIC_TITLE, titleS);

				m_btnTitle.SetFont(m_pBFont);
				m_btnTitle.SetWindowText(titleS);

				m_strHeadertitle = titleS;
				m_strHeaderKey = news.keyv;
				m_strHeadercode = news.code;
				
				if(m_strHeadercode.GetLength() > 6)
				{
					m_strHeadercode = m_strHeadercode.Mid(1);
				}

	// 			if(m_chkTrigger.GetCheck() == TRUE)
	// 			{
	// 				CString msg;
	// 
	// 				if(m_strHeadercode.GetLength() >= 6)
	// 				{
	// 					msg.Format("1301\t%s", m_strHeadercode);
	// 					GetAxVariant(triggerCC, msg);
	// 				}
	// 			}

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

				if(m_iTabCount > 2)
				{
					if(m_iTabIndex == 0)
					{
						gridAddRow(news, false);
					}
					else if(m_iTabIndex == 1)
					{
						const bool result = m_pFilterWnd->SetMarketNewsData(&news);
						
						if(result == true)
						{
							FiltergridAddRow(news, false);
						}
					}
				}
			}	// news Check
		}	//tab Index
		fms.RemoveAll();
	}
	catch (CException* e)
	{
		e->ReportError();
	}

}

void CMapDlg::parsingAlertx(DWORD * data)
{
	if (m_bSelectScrap)
		return;


	try
	{
		CString			value, str, szBuffer, szText;
		news_info		news;


		if (data[301]) {
			news.code = (char*)data[301];
			news.code.TrimRight();
		}


		//종목명 라디오 버튼 누르고 뉴스 볼 경우
		if (m_pRadioSel)
		{
			const int icodechk = m_pRadioSel->GetCheck();
			if (icodechk)
			{
				CString sCode = m_code->GetEditData();
				sCode.TrimRight();
				if (sCode != news.code) return;
			}
		}


		if (data[15]) {
			value = (char*)data[15]; // 제목
			value.TrimRight();
			CopyMemory(news.titl, value.GetString(), min(value.GetLength(), title_len));
		}
		else {
			return;
		}

		if (data[16]) {
			news.keyv = (char*)data[16]; // Keyv
		}

		if (data[22]) {
			value = (char*)data[22]; 		// 종목명
			value.TrimLeft(), value.TrimRight();
			news.hnam = value;
		}
		else {
			news.hnam.Empty();
		}


		if (data[41]) {		// 뉴스구분
			value = (char*)data[41]; 
			const int gubn = atoi(value);
			news.gubn = GetNewGubnText(gubn);
			news.gubn.TrimRight();
		}
		else {
			news.hnam.Empty();
		}

		if (data[44]) {
			value = (char*)data[44];
			if (value.GetLength() >= 4)		// 시간
				news.time.Format("%s:%s", value.Left(2), value.Mid(2, 2));
		}

		if (data[45]) news.subg = (char*)data[45]; // 분류명
		if (data[46]) news.gnam = (char*)data[46]; // 구분명
		if (data[47]) news.flag = (char*)data[47]; // 텍스트(0) or HTML(1)


		if (data[48]) {
			value = (char*)data[48];
			if (value.GetLength() >= 8) {		// date
				news.date = value.Left(4) + "/" + value.Mid(4, 2) + "/" + value.Right(2);
			}
		}


		if (data[14]) news.gisn = (char*)data[14]; // 텍스트(0) or HTML(1)

		if (news.code.GetLength() == 6)
			news.code = "A" + news.code;

		// 		bool isFiltering = false;
		// 
		// 		if(m_iTabCount > 2)
		// 		{
		// 			if(m_iTabIndex == 1)
		// 			{
		// 				isFiltering = true;
		// 			}	
		// 		}

				// 필터링 결과인지 확인
		//		if(isFiltering == true)
		{
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
				while (listCnt > nItem + 3)
				{
					if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
						break;
					m_List.DeleteItem(listCnt - 1);
					listCnt = m_List.GetItemCount();
				}
			}

			const bool result = m_pFilterWnd->SetMarketNewsData(&news);

			if (result == true)
			{
				FiltergridAddRow(news, false);
			}
		}
		//		else
		{
			//			if(isNewsViewChecked(news) && isMediaChecked2(news))
			if (isNewsViewChecked(news))
			{
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
					while (listCnt > nItem + 3)
					{
						if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
							break;
						m_List.DeleteItem(listCnt - 1);
						listCnt = m_List.GetItemCount();
					}
				}

				gridAddRow(news, false);

				//헤더에 내용 추가
				CString titleS;
				titleS.Format("%s", CString(news.titl, title_len));
				titleS.TrimRight();

				// 			m_pStatic.SetFont(m_pBFont);
				// 			SetDlgItemText(IDC_STATIC_TITLE, titleS);

				m_btnTitle.SetFont(m_pBFont);
				m_btnTitle.SetWindowText(titleS);

				m_strHeadertitle = titleS;
				m_strHeaderKey = news.keyv;
				m_strHeadercode = news.code;

				if (m_strHeadercode.GetLength() > 6)
				{
					m_strHeadercode = m_strHeadercode.Mid(1);
				}

				// 			if(m_chkTrigger.GetCheck() == TRUE)
				// 			{
				// 				CString msg;
				// 
				// 				if(m_strHeadercode.GetLength() >= 6)
				// 				{
				// 					msg.Format("1301\t%s", m_strHeadercode);
				// 					GetAxVariant(triggerCC, msg);
				// 				}
				// 			}

			}
			else		//뉴스사를 클릭한 경우
			{
				//OutputDebugString(_T("isMdiaChecked(FALSE) ------>"));
				//선택한 뉴스사와 실시간으로 받은 뉴스사가 동일한지 체키럽~!
				if (m_TreeWnd.GetItemText(select_hItem) != news.gubn)
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
					while (listCnt > nItem + 3)
					{
						if (listCnt - 1 < 0 || listCnt - 1 >= m_List.GetItemCount())
							break;
						m_List.DeleteItem(listCnt - 1);
						listCnt = m_List.GetItemCount();
					}
				}

				if (m_iTabCount > 2)
				{
					if (m_iTabIndex == 0)
					{
						gridAddRow(news, false);
					}
					else if (m_iTabIndex == 1)
					{
						const bool result = m_pFilterWnd->SetMarketNewsData(&news);

						if (result == true)
						{
							FiltergridAddRow(news, false);
						}
					}
				}
			}	// news Check
		}	//tab Index
	}
	catch (CException* e)
	{
		e->ReportError();
	}

}


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
	//CoInitialize(NULL);

	CFileFind finder;
	memset(m_szCurKey, 0x00, 23);
	if (finder.FindFile(m_szFileName))
		DeleteFile(m_szFileName);
	m_pBrowser->Stop();
	m_pBrowser->Navigate2("about:blank");

//	CheckTab( -1 );

	//CoUninitialize(); 
}


void CMapDlg::selectTab(int nIndex)
{
	const int Index = m_pTab->GetCurSel();
	
	m_iTabCount = m_pTab->GetItemCount();

	if(m_iTabCount > 2)
	{
		switch (Index)
		{
		case 0 :
			m_iTabIndex = 0;
			VisibleFirstTab(SW_SHOW);
			VisibleSecondTab(SW_HIDE);
			m_pThirdMap->ShowWindow(SW_HIDE);

			m_List.SetFocus();
			break;
		case 1 :
			m_iTabIndex = 1;
			VisibleFirstTab(SW_HIDE);
			VisibleSecondTab(SW_SHOW);
			m_pThirdMap->ShowWindow(SW_HIDE);
			m_pFilterWnd->SetFilterSetData();
			m_pFilterWnd->SetFilterResultSetData();
	
			m_FilterResultListCtrl.SetFocus();
			break;
		case 2 :
			m_iTabIndex = 2;
			VisibleFirstTab(SW_HIDE);
			VisibleSecondTab(SW_HIDE);
			m_pThirdMap->ChangeMap(szFilterMapName2, "");
			m_pThirdMap->ShowWindow(SW_SHOW);
			const int m_pKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

			
			CRect rc1;
			m_pThirdMap->GetClientRect(rc1); 	
			rc1.InflateRect(0,1);
			m_pThirdMap->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
			break;
		}
	}
	else
	{
		switch (Index)
		{
		case 0 :
			m_iTabIndex = 0;
			VisibleFirstTab(SW_SHOW);
			VisibleSecondTab(SW_HIDE);
			m_pThirdMap->ShowWindow(SW_HIDE);
			break;
		case 1 :
			m_iTabIndex = 1;
			VisibleFirstTab(SW_HIDE);
			VisibleSecondTab(SW_HIDE);
			m_pThirdMap->ChangeMap(szFilterMapName2, "");
			m_pThirdMap->ShowWindow(SW_SHOW);
			const int m_pKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0);

			CRect rc1;
			m_pThirdMap->GetClientRect(rc1); 	
			rc1.InflateRect(0,1);
			m_pThirdMap->SetWindowPos( NULL, 0,0, rc1.Width(), rc1.Height(), SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOZORDER );
			break;
		}
	}


	ResizeControl();

}


bool CMapDlg::isNewsViewChecked(const news_info& news, bool bAlert)
{
	CString code = news.code;
	bool result = false;
	CString sCode = "";
	
	if(code.GetLength() == 7)
	{
		code = code.Mid(1);
	}

	//종목만 보기에 체크가 되어있는지확인 
	if(m_bChkOnlyjongmok.GetCheck() == TRUE)
	{
		if(code.GetLength() > 5)
		{
			result = true;
		}
	}
	else
	{
		result = true;
/*		if(m_radioTotal.GetCheck() == TRUE)
		{
			result = true;				//전체는 무조건 true
		}
		else
		{
			if (code.GetLength() < 6)	//종목 선택시엔 코드 있을때만 true
			{
				result = false;
			}
			else
			{
				m_code->GetWindowText(sCode);
				
				sCode.TrimRight();
				sCode.TrimLeft();
				code.TrimRight();
				code.TrimLeft();
				
				if(sCode.GetLength() == 0)
				{
					result = true;
				}
				else
				{
					if(sCode.Compare(code) == 0)
					{
						result = true;
					}
				}
			}
		}
*/
	}


	return result;
}


bool CMapDlg::isMediaChecked2(const news_info& news)
{
	CString sTmp = news.keyv; // date(8)+gubn(2)+subg(4)+sqpn(10) : keyv
	CString tggg = news.keyv;

	if(sTmp.GetLength() < 13) return true;
	
	sTmp = sTmp.Mid(8, 6);  // gubn(2)+subg(4)
	CString sGubn = sTmp.Left(2);
	sGubn.TrimLeft();

	const int igubn = CInt( sTmp.Left(2));
	const int isubgubn = CInt( sTmp.Right(4) );
	
	bool result = false;

	//라디오 버튼 체크
	if(m_radioTotal2.GetCheck() == TRUE)		//전체 체크시엔 무조건 true
	{
		result = true;	
	}
	else if(m_radioGongsi.GetCheck() == TRUE)	//공시 체크시엔 2일경우에 true
	{
		if(igubn == 2)
		{
			result = true;
		}
	}	
	else
	{
		if(igubn != 2)
		{
			result = true;
		}
	}

	return result;
	
}

bool CMapDlg::isMediaChecked(const news_info& news, bool bAlert /* = false*/)
{
	if( !m_bTotList )
		return true;

	CString sTmp = news.keyv; // date(8)+gubn(2)+subg(4)+sqpn(10) : keyv
	CString tggg = news.keyv;

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

		int iRetGubn{}, iRetSubg{}; // 현재 선택된 트리 아이템의 구분과 서브구분을 얻어온다.
		
		if( (str == "언론사별" || str == "카테고리별") && igubn == 2 )
		{
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
			for(int i=0; i<m_TreeWnd.m_selItem.GetSize();i++)
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
		else
		{
			//OutputDebugString("=================================\n");
			GetGugnAndSubgFromTREEITEM( select_hItem, iRetGubn, iRetSubg );

			if( iRetSubg == -1 && iRetGubn != -1 ) //  구분이 찍혔으면 
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

// 						CString tt = "PARENT :" + str + " : " +  news.gubn + "\n";
// 
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
// 
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
			strName = "K-OTC 공시";
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

void CMapDlg::FiltergridAddRow(news_info &news, bool bPushBack, bool bCountNews)
{
	int nCount = 0;
	
	if (bPushBack)	nCount = m_FilterResultListCtrl.GetItemCount();

	std::shared_ptr<LVITEM> lv = std::make_shared<LVITEM>();
	//LVITEM * lv = new LVITEM;
	if (!lv) return;

	lv->iItem = nCount;
	lv->iSubItem = 0;
	lv->pszText = _T("");
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.InsertItem(lv.get());
	m_FilterResultListCtrl.SetNewsInfo(nCount, news.gisn, news.sisn, news.keyv, news.code, news.flag);
	//OutputDebugString(_T("gridAddRow ------>\n"));
 
	if( bCountNews ) // 순위 
	{
		CString str;
		str.Format("%d", nCount+1 );
		
		m_FilterResultListCtrl.SetItemText( nCount, 1, str );
	}
	
	// 날짜
	if( bCountNews )
		m_FilterResultListCtrl.SetItemText( nCount, 2, news.date.Left(4)+"/"+news.date.Mid(4,2)+"/"+news.date.Right(2) );
	else
		m_FilterResultListCtrl.SetItemText( nCount, 2, news.date );

	// 시간
	lv->iItem = nCount;
	lv->iSubItem = 3;
	lv->pszText = (char*)(LPCSTR)news.time;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	// 제목
	lv->iItem = nCount;
	lv->iSubItem = 4;
	lv->pszText = (char*)(LPCSTR)news.titl;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	if( bCountNews )
	{
		CString str;
		str.Format("%d ", news.rCnt );

		m_FilterResultListCtrl.SetItemText( nCount, 5, str );
	}
	else
	{
		m_FilterResultListCtrl.SetItemText( nCount, 5, "" );
	}

	// 종목명
	lv->iItem = nCount;
	lv->iSubItem = 6;
	lv->pszText = (char*)(LPCSTR)news.hnam;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	// 정보구분
	lv->iItem = nCount;
	lv->iSubItem = 7;
	lv->pszText = (char*)(LPCTSTR)news.gubn;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	// 분류코드
	lv->iItem = nCount;
	lv->iSubItem = 8;
	lv->pszText = (char*)(LPCSTR)news.subg;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	// count
	CString tmpS;
	tmpS.Format("%d", nCount);
	lv->iItem = nCount;
	lv->iSubItem = 9;
	lv->pszText = (char*)(LPCSTR)tmpS;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	// isam
	lv->iItem = nCount;
	lv->iSubItem = 10;
	lv->pszText = (char*)(LPCSTR)news.gisn;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

	lv->iItem = nCount;
	lv->iSubItem = 11;
	lv->pszText = (char*)(LPCSTR)news.code;
	lv->mask = LVIF_TEXT;
	m_FilterResultListCtrl.SetItem(lv.get());

//	delete lv;	
//	lv = NULL;
}


void CMapDlg::gridAddRow(news_info &news ,bool bPushBack, bool bCountNews)
{
	int nCount = 0;
	
	if (bPushBack)	nCount = m_List.GetItemCount();

//	LVITEM* lv = new LVITEM;
	std::shared_ptr< LVITEM> lv = std::make_shared<LVITEM>();
	if (!lv) return;

	lv->iItem = nCount;
	lv->iSubItem = 0;
	lv->pszText = _T("");
	lv->mask = LVIF_TEXT;
	m_List.InsertItem(lv.get());
	m_List.SetNewsInfo(nCount, news.gisn, news.sisn, news.keyv, news.code, news.flag);
	//OutputDebugString(_T("gridAddRow ------>\n"));
// 	if (m_pFilterWnd && m_pFilterWnd->GetSafeHwnd() && !bPushBack)
// 	{
// 		m_pFilterWnd->SetMarketNewsData(&news); // 실시간 뉴스에  대해서만 시황화면에 나타난다.
// 	}

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

//	delete lv;	
//	lv = NULL;

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
	const char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
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

	if(m_iTabCount > 2)
	{
		if(m_iTabIndex == 0)
			m_List.GetWindowRect(rcGrid);
		else if(m_iTabIndex == 1)
			m_FilterResultListCtrl.GetWindowRect(rcGrid);
	}
	else
	{
		m_List.GetWindowRect(rcGrid);
	}

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
		m_pFilterWnd = std::make_unique< CMarketDlg >(m_szRootDir, m_szUser, m_pParent, m_pFont,this); 
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
		const ULONGLONG	pos = pFile.SeekToEnd();
		pFile.Seek( pos, CFile::begin);
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
	const DWORD dFileLength = (DWORD)pFile.GetLength();
	if(dFileLength <= 0	) return;
	strRead.ReleaseBuffer(pFile.Read(strRead.GetBufferSetLength(dFileLength), dFileLength));
	pFile.Close();
	
	
	CStringArray arList;
	CString		sTmplist[MAXCNT];
/*
	SplitStringNew(strRead, NEW_LINE, arList);
	
	int icnt = 0;
	for(int i=0; i<arList.GetSize(); i++)
	{
		if( Trim(arList[i]).IsEmpty() ) continue; 
		icnt = SplitString(arList[i], TABCHAR, sTmplist);
		if(icnt > 0)
		{
			if( Trim(sTmplist[0]).IsEmpty() ) continue; 
			m_arScrapList.Add(AList(icnt, sTmplist));
		}
	}
*/
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
		news.keyv=	m_arScrapList.GetAt(i).sList[10];
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
// 	if(!m_pBrowser) 
// 		return;

	const int nSideGap = 6;
	const int nCtrlHeight = 10;
	const int nCtrlGap = 5;
	CRect rc{}, rc1{}, rc2{};
	const int itreeW = 8;

	GetClientRect(&rc);
	int ibtnh = 0;
	
 	m_pTab->GetWindowRect(&rc1);
 	ScreenToClient(&rc1);

	ibtnh = rc1.Height();
	
	rc1.top = rc.top + GAP3;
	rc1.left = GAP3;
	rc1.right = rc.Width() - GAP3;
	rc1.bottom = rc1.top + ibtnh;		

	m_pTab->MoveWindow(rc1);
 
	if(m_iTabCount > 2)
	{
		if(m_iTabIndex == 0)
		{
			Resizetab1(rc, rc1);
			ResizeNewsTitleField(rc1.Width());
		}
		else if(m_iTabIndex == 1)
		{
			m_FilterResultListCtrl.GetWindowRect(&rc2);
			ScreenToClient(&rc2);
			 
			rc2.left = GAP3;
			rc2.top = rc1.bottom;
			rc2.right = rc.Width() - GAP3;
			rc2.bottom = rc.Height() - GAP3;
			
			m_FilterResultListCtrl.MoveWindow(rc2);  

// 			CString str;
// 			str.Format("tabIndex %d Main width %d map width %d\n",m_iTabIndex, rc.Width(), rc2.Width());
// 			OutputDebugString(str);
			ResizeNewsTitleField(rc2.Width());
		}
		else if(m_iTabIndex == 2)
		{
			m_pThirdMap->GetWindowRect(&rc2);
			ScreenToClient(&rc2);
			
			rc2.left = GAP3;
			rc2.top = rc1.bottom;
			rc2.right = rc.Width() - GAP3;
			rc2.bottom = rc.Height() - GAP3;
			
			m_pThirdMap->MoveWindow(&rc2);
		}
	}
	else
	{
		if(m_iTabIndex == 0)
		{
			Resizetab1(rc, rc1);
			ResizeNewsTitleField(rc1.Width());
		}
		else if(m_iTabIndex == 1)
		{
			m_pThirdMap->GetWindowRect(&rc2);
			ScreenToClient(&rc2);
			
			rc2.left = GAP3;
			rc2.top = rc1.bottom;
			rc2.right = rc.Width() - GAP3;
			rc2.bottom = rc.Height() - GAP3;

			m_pThirdMap->MoveWindow(&rc2);
			ResizeNewsTitleField(rc2.Width());
		}
	}

	Invalidate();
}


void CMapDlg::Resizetab1(CRect rc, CRect rc1)
{
	CRect rcRadio, rcGroup, rc2;

	rc1.left = GAP3;
	rc1.right = rc.Width() - GAP3;
	rc1.top = rc1.bottom;
	rc1.bottom = rc1.top + PANEL_HEIGHT;

	rcRadio = rc1;
	rcGroup = rc1;
	
	rcRadio.top = rcRadio.top + 4;
	rcRadio.bottom = rcRadio.top + 19;
	rcRadio.left = 7;
	rcRadio.right = rcRadio.left + 120;
	m_bChkOnlyjongmok.MoveWindow(rcRadio);	// radio
	
	rcRadio.left = rcRadio.right;
	rcRadio.right = rcRadio.left + 136;
	m_bChkAutoTrigger.MoveWindow(rcRadio);
/*	
	rcRadio.left = rcRadio.right +2;
	rcRadio.right = rcRadio.left + 60;
	rcRadio.top = rcRadio.top;
	rcRadio.bottom = rcRadio.bottom;
	
	if(m_code->GetSafeHwnd())
	{
		m_code->MoveWindow(rcRadio);
	}

	rcRadio.left = rcRadio.right;
	rcRadio.right = rcRadio.left + 20;
	m_btnJM.MoveWindow(rcRadio);

	rcRadio.left = rcRadio.right;
	rcRadio.right = rcRadio.left + 100;
	m_symbol->MoveWindow(rcRadio);

	rcRadio.top = rcRadio.bottom +5;
	rcRadio.bottom = rcRadio.top + 18;
	rcRadio.left = 7;
	rcRadio.right = rcRadio.left + 43;
	m_radioTotal2.MoveWindow(rcRadio);
	
	rcRadio.left = rcRadio.right;
	rcRadio.right = rcRadio.left + 43;
	m_radioGongsi.MoveWindow(rcRadio);
	
	rcRadio.left = rcRadio.right;
	rcRadio.right = rcRadio.left + 43;
	m_radioSihwang.MoveWindow(rcRadio);

		
	rcRadio.right = rc.right - 5;
	rcRadio.left = rcRadio.right - 52;
	m_btnViewSet.MoveWindow(rcRadio);
*/	
	rc1.left = GAP3;
	rc1.right = rc.Width() - GAP3;
	rc1.top = rc1.bottom - 1;
	rc1.bottom = rc1.top + BUTTON_HEIGHT;
	
	m_pStatic.MoveWindow(rc1);	
	m_btnTitle.MoveWindow(rc1);
	
	m_List.GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	
	rc2.left = GAP3;
	rc2.top = rc1.bottom;
	rc2.right = rc.Width() - GAP3;
	rc2.bottom = rc.Height() - GAP3;
	
	m_List.MoveWindow(rc2);  

}

void CMapDlg::Resizetab2(CRect rc, CRect rc1)
{

}

void CMapDlg::Resizetab3(CRect rc, CRect rc1)
{

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


void CMapDlg::SetTitleListHeader(int iw)
{}

void CMapDlg::ResizeNewsTitleField(int iw) 
{
	int iWidth = 0;

	if(m_iTabCount > 2)
	{
		if(m_iTabIndex == 0)
		{
			iWidth += m_List.GetColumnWidth(0); // 스크랩
			iWidth += m_List.GetColumnWidth(1); // 순위 
			iWidth += m_List.GetColumnWidth(2); // 날짜 
			iWidth += m_List.GetColumnWidth(3); // 시간 
			//	iWidth += m_List.GetColumnWidth(4); 
			iWidth += m_List.GetColumnWidth(5); // 조회수
			iWidth += m_List.GetColumnWidth(6);	// 종목명
			iWidth += m_List.GetColumnWidth(7);	// 정부구분 
			
			m_List.SetColumnWidth( 4, iw-iWidth-17 );
		}
		else if(m_iTabIndex == 1)
		{
			iWidth += m_FilterResultListCtrl.GetColumnWidth(0); // 스크랩
			iWidth += m_FilterResultListCtrl.GetColumnWidth(1); // 순위 
			iWidth += m_FilterResultListCtrl.GetColumnWidth(2); // 날짜 
			iWidth += m_FilterResultListCtrl.GetColumnWidth(3); // 시간 
			//	iWidth += m_List.GetColumnWidth(4); 
			iWidth += m_FilterResultListCtrl.GetColumnWidth(5); // 조회수
			iWidth += m_FilterResultListCtrl.GetColumnWidth(6);	// 종목명
			iWidth += m_FilterResultListCtrl.GetColumnWidth(7);	// 정부구분 
			
			m_FilterResultListCtrl.SetColumnWidth( 4, iw-iWidth-17 );
		}
	}
	else
	{
		if(m_iTabIndex == 0)
		{
			iWidth += m_List.GetColumnWidth(0); // 스크랩
			iWidth += m_List.GetColumnWidth(1); // 순위 
			iWidth += m_List.GetColumnWidth(2); // 날짜 
			iWidth += m_List.GetColumnWidth(3); // 시간 
			//	iWidth += m_List.GetColumnWidth(4); 
			iWidth += m_List.GetColumnWidth(5); // 조회수
			iWidth += m_List.GetColumnWidth(6);	// 종목명
			iWidth += m_List.GetColumnWidth(7);	// 정부구분 
			
			m_List.SetColumnWidth( 4, iw-iWidth-17 );
		}
	}


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
		const DWORD	pos = (DWORD)pFile.SeekToEnd();
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

BOOL CMapDlg::findSubCodName(const CString& strCode, CString& strName )
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
			if( strFind.Find(sc.name) >= 0 )	// 종목 이름이 제한 리스트에 포함 되어 있다면 검색 안한다
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
	strPath.Format("%s/tab/hjcode.dat", m_szRootDir );

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
		DWORD length = (DWORD)file.GetLength();
//		char* pBuf = new char[length];
		std::unique_ptr<char[]>pBuf = std::make_unique<char[]>(length);

		file.Read( pBuf.get(), length );
		file.Close();

		length /= sizeof(struct hjcodex);

		CString code,hnam;

		CMap<int, int, CStringArray*, CStringArray*> lengthMap;

		lengthMap.InitHashTable( 30 );

		for( int i = 20; i >=2 ; i-- )
		{
			//CStringArray* pArr = new CStringArray;
			std::unique_ptr<CStringArray> pArr = std::make_unique< CStringArray>();
			lengthMap.SetAt( i, pArr.release() );
		}
		
		struct hjcodex* hjc = (struct hjcodex*)pBuf.get();
		for(int i = 0; i < (int)length; i++, hjc++ )
		{
			code = CString( hjc->code, HCodeLen );
			if( code.GetAt(0) == 'A' )
			{
				code = code.Mid(1);
				code.TrimRight();

				hnam = CString(hjc->hnam,HNameLen);
				hnam = hnam.Mid(1);
				hnam.TrimRight();

				/*if( hnam == CString("EMLSI") )
				{
					break;
				}*/

				const int nameLen = hnam.GetLength();
				
				CStringArray* pArr;
				if( lengthMap.Lookup( nameLen, pArr ) )
				{
					pArr->Add( hnam );
					pArr->Add( code );

					lengthMap.SetAt( nameLen, pArr );
				}				
			}
		}		

//m_pOptionWnd		delete [] pBuf;

		for(int i = 20; i >=2 ; i-- )
		{
			CStringArray* pArr;
			lengthMap.Lookup( i, pArr );
			if( pArr->GetSize() > 1 )
			{
				for( int k = 0; k < pArr->GetSize()-1; k+=2 )
				{
					sorting_cod sc;
					sc.name = pArr->GetAt(k);
					sc.cod = pArr->GetAt(k+1);

					m_sortCode.Add( sc );
				}
			}

			delete pArr;
		}	

		// write sort
		strCur += "\n";
		tFile.SeekToBegin();
		tFile.WriteString( strCur );
		for(int i = 0; i < m_sortCode.GetSize(); i++ )
		{
			sorting_cod tsc = m_sortCode.GetAt(i);
			CString strTemp;
			strTemp.Format( "%s|%s\n", tsc.cod, tsc.name );
			tFile.WriteString( strTemp );			
		}
		tFile.Close();		
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


	}	
}

void CMapDlg::VisibleFirstTab(int bShow)
{
	m_pStatic.ShowWindow(bShow);
	m_List.ShowWindow(bShow);
	m_radioTotal.ShowWindow(SW_HIDE);
	m_radioJongmok.ShowWindow(SW_HIDE);
	m_radioTotal2.ShowWindow(SW_HIDE);
	m_radioGongsi.ShowWindow(SW_HIDE);
	m_radioSihwang.ShowWindow(SW_HIDE);
	m_staticRadio.ShowWindow(SW_HIDE);
	m_staticRadio2.ShowWindow(SW_HIDE);
	m_chkTrigger.ShowWindow(bShow);
	m_btnTitle.ShowWindow(bShow);
	m_btnViewSet.ShowWindow(SW_HIDE);
	if (m_code)m_code->ShowWindow(SW_HIDE);
	m_btnJM.ShowWindow(SW_HIDE);
 	if (m_symbol) m_symbol->ShowWindow(SW_HIDE);
	m_bChkAutoTrigger.ShowWindow(bShow);
	m_bChkOnlyjongmok.ShowWindow(bShow);

// 	if(m_radioTotal.GetCheck() == true)
// 	{
//  		m_code->ShowWindow(false);
//  		m_btnJM.ShowWindow(false);
//  		m_symbol->ShowWindow(false);
// 	}
// 	else
// 	{
//  		m_code->ShowWindow(bShow);
//  		m_btnJM.ShowWindow(bShow);
//  		m_symbol->ShowWindow(bShow);
// 	}

}

void CMapDlg::VisibleSecondTab(int bShow)
{
	m_FilterResultListCtrl.ShowWindow(bShow);
}

#define		P_TAB				0x09

void Wait(DWORD dwMillisecond) {
	MSG msg{};
	DWORD t0=0, t1=0, diff=0;

	t0 = GetTickCount();
	while (TRUE) {
		t1 = GetTickCount();

		if (t0 <= t1) diff = t1 - t0;
		else diff = 0xffffffff - t0 + t1;


		if (diff > dwMillisecond) break;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(1);
	}
}



void CMapDlg::parseTrigger(CString sData)
{
	int iFind = sData.Find(P_TAB);
	if(iFind < 0) return;

	CString sKey = sData.Left(iFind);
	CString sParse = sData.Mid(iFind+1);
	iFind = sParse.Find(P_TAB);
	sParse = sParse.Left(iFind);
	if( sKey == "TABCHANGE")
	{
		if(sParse == "False")
		{
			if (m_pTab->GetItemCount() > 2) {
				BeginWaitCursor();
				m_pTab->DeleteItem(1);
				Sleep(300);
				EndWaitCursor();
			}
		}
		else
		{
			if (m_pTab->GetItemCount() == 2) {
				BeginWaitCursor();
				m_pTab->InsertItem(1, "필터링결과");
				Sleep(300);
				EndWaitCursor();
			}
		}
	}
	else if(sKey == "1301")
	{
		CString code = sData.Mid(5);
		m_code->SetEditData(code);
		m_code->SetHistory(code);

//		if(m_radioJongmok.GetCheck())
//			RequestTitleList();
	}
}


void CMapDlg::OnRadioTotal()
{
/*	m_bTotSearch = true;
	if (m_bTotSearch)
	{
		m_radioTotal.SetCheck(TRUE);
		m_radioJongmok.SetCheck(FALSE);
	}
	else
	{
		m_radioTotal.SetCheck(FALSE);
		m_radioJongmok.SetCheck(TRUE);
	}

	if (m_code)
	{
		m_code->SetEditData("");
		m_code->ShowWindow(false);
	}

	m_btnJM.ShowWindow(false); 

	if (m_symbol)
	{
		CString text = "";
		WPARAM wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 20), (text.GetLength() > 6) ? text.GetLength() : 0);
		m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		m_symbol->ShowWindow(false);
	}

*/
	m_szCode = "";
	RequestTitleList();
}

void CMapDlg::OnRadioJongmok()
{
/*	m_bTotSearch = false;
	if (m_bTotSearch)
	{
		m_radioTotal.SetCheck(TRUE);
		m_radioJongmok.SetCheck(FALSE);
	}
	else
	{
		m_radioTotal.SetCheck(FALSE);
		m_radioJongmok.SetCheck(TRUE);
	}
	
	m_btnJM.ShowWindow(true);
	
	if (m_symbol)	
		m_symbol->ShowWindow(true);
	
	if (m_code)
	{
		m_code->ShowWindow(true);
		if ( m_szCode == "")
		{
			CString sHistory = m_code->m_sHistory;
			CString sCode = Parser(sHistory,"\t");
			if (sCode.GetLength() >= 6)
				m_code->SetEditData( sCode.Left(6) );
			m_code->SetHistory( GetAxVariant(historyCC, _T("1301")));
			m_szCode = m_code->GetEditData();
		}
	}
	
	if(m_radioJongmok.GetCheck())
		RequestTitleList();*/
}

void CMapDlg::OnRadioTotal2()
{
	RequestTitleList();
}

void CMapDlg::OnRadioGongsi()
{
	RequestTitleList();
}

void CMapDlg::OnRadioSihwang()
{
	RequestTitleList();
}

void CMapDlg::OnOnlyJongmok()
{
	RequestTitleList();
}

void CMapDlg::setOnlyJongmok(bool isCheck)
{
	if(isCheck == TRUE)
	{
		m_radioTotal.SetCheck(1);
		m_radioJongmok.SetCheck(0);
		m_radioTotal.EnableWindow(FALSE);
		m_radioJongmok.EnableWindow(FALSE);
		OnRadioTotal();
	}
	else
	{
		m_radioTotal.EnableWindow(TRUE);
		m_radioJongmok.EnableWindow(TRUE);		
	}
}

CPen* CMapDlg::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}
