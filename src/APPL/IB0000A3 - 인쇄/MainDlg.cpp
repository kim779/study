// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A3.h"
#include "MainDlg.h"
#include "WxReport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define		ENDWAIT		101
#define		PRINTWAIT	102
#define		SENDWAIT	103
#define		WAIT		104

/////////////////////////////////////////////////////////////////////////////
// CMainDlg dialog


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDlg)
	//}}AFX_DATA_INIT
	m_pAxMain = pParent;
	m_bListAdd = m_bNext = FALSE;
	m_strLedger = "";

	m_bTestMode = FALSE;
	m_iQueryCount = 0;
	m_bExecPrn = m_bFirstData = FALSE;
	m_iPreviewPage = 0;
	m_iDrawX = 990;
	m_iDrawY = 805;
	m_hiSha256 = NULL;
}


void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDlg)
	DDX_Control(pDX, IDC_LIST4, m_clxOutputList);
	DDX_Control(pDX, IDC_LIST3, m_clxQueryList);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_COMBO1, m_cboReportType);
	DDX_Control(pDX, IDC_LIST1, m_OubList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDlg)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, OnTest)
	ON_BN_CLICKED(IDC_BUTTON2, OnPrnTest)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, OnNextPAge)
	ON_BN_CLICKED(IDC_BUTTON3, OnPrevPage)
	ON_BN_CLICKED(IDC_CLOSEBUTTON, OnClosebutton)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
LRESULT CMainDlg::SendAxisMessage(WPARAM wParam, LPARAM lPAram)
{	
	return m_pAxMain->SendMessage(WM_USER, wParam, lPAram);
}

void CMainDlg::sendTR(CString trCode, char* datB, int datL, char key)
{

	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(L_userTH + datL + 1);
//	char* sendB = new char [L_userTH + datL + 1];

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB.get();

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB.get()[L_userTH], datB, datL);
	SendAxisMessage( MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB.get());
//	delete [] sendB;
}

CBitmap* CMainDlg::getBitmap(CString path)
{
	path = m_strImgPath + path;

	CBitmap*	pBitmap = (CBitmap*)SendAxisMessage( getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap;
}

LONG CMainDlg::OnUser(WPARAM wParam, LPARAM lParam)
{
	//LONG	ret = 0;
	int ilen{};
	CString sdata;
	switch(LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUB:
			ilen = HIWORD(wParam);
			sdata.Format("%s", (char*)lParam);
			sdata = sdata.Left(ilen);
			ParsingOubData(sdata);
			break;

		case DLL_ALERT:break;
		case DLL_SETFCB:break;
		case DLL_SETFONT:break;
		case DLL_SETFONTx:break;
		case DLL_TRIGGER:break;
		case DLL_DOMINO:
			SetDominoData( (char*)lParam );
			break;
		case DLL_NOTICE:break;
		case DLL_GUIDE:break;
		case DLL_INVOKE:break;
	}

	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CMainDlg message handlers

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

BOOL CMainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_cboReportType.InsertString(0, "WxReport01"); 
	m_cboReportType.InsertString(1, "WxReport02");
	m_cboReportType.InsertString(2, "WxReport03");
	m_cboReportType.InsertString(3, "WxReport04");
	m_cboReportType.InsertString(4, "WxReport05");
	m_cboReportType.InsertString(5, "WxReport06");
	m_cboReportType.InsertString(6, "WxReport07");
	m_cboReportType.SetCurSel(0);
	

	SetInitValue() ;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_bFirstData && m_bTestMode)
		DrawScreen(&dc);
}


void CMainDlg::SetInitValue() 
{
	m_strUserID = (char*)SendAxisMessage(MAKEWPARAM(variantDLL, userCC), NULL);
	m_strPass = (char*)SendAxisMessage(MAKEWPARAM(variantDLL, passCCx), NULL);
	m_strDeptCode = (char*)SendAxisMessage(MAKEWPARAM(variantDLL, deptCC), NULL);
	m_strRootDir = (char*)SendAxisMessage(MAKEWPARAM(variantDLL, homeCC), NULL);

	m_strImgPath.Format("%s\\%s\\",m_strRootDir, IMAGEDIR) ;

	//TEST
	GetDlgItem(IDC_MARKETEDIT)->SetWindowText("1");
	GetDlgItem(IDC_ODRFEDIT)->SetWindowText("1");
	GetDlgItem(IDC_ACCOUNTEDIT)->SetWindowText("101-10-000002");
	m_clxQueryList.InsertColumn(0, "SEQ", LVCFMT_CENTER, 30) ;
	m_clxQueryList.InsertColumn(1, "INPUT", LVCFMT_LEFT, 270) ;

	m_clxQueryList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE |LVS_EX_INFOTIP | LVS_EX_GRIDLINES); 	
	m_clxOutputList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE |LVS_EX_INFOTIP | LVS_EX_GRIDLINES); 	

	m_clxOutputList.InsertColumn(0, "SEQ", LVCFMT_CENTER, 30) ;
	for(int i=1; i<100;i++)
		m_clxOutputList.InsertColumn(i, "DATA_"+CStr(i), LVCFMT_RIGHT, 50) ;

	m_bOneWay = FALSE;
}

void CMainDlg::SetDominoData(CString sData)
{
	GetDlgItem(IDC_EDIT1)->SetWindowText(sData);

	CString sTab = " ";
	sTab.SetAt(0, tabCHAR);

//	CString sDominoSymbol = SplitString(sData, tabCHAR);
	CString sDominoSymbol = SplitString(sData, sTab);

	sDomino02 = sDominoSymbol;
	const int index = sData.Find("sumdata");

	if (index>-1) 
	{
		sSumdata = sData.Mid(index+8);
		sData = sData.Mid(0, index);
		
		ParseSum(sSumdata);
	}

	if(sDominoSymbol.Find("WxReport") > -1)
	{
		m_arDominoList.RemoveAll(); 

		CString sTab = " ";
		sTab.SetAt(0, tabCHAR);

		SplitString_MAP(sData, sTab, m_arDominoList);
//		SplitString_MAP(sData, tabCHAR, m_arDominoList);

		if(m_arDominoList.Lookup("b", sData) )
		{
			if(sData.Find("HEAD") >= 0 )
				m_bOneWay = TRUE;
		}

		const int iReport = CInt( sDominoSymbol.Right(2) );
		
		if(m_cboReportType.GetCount()  >= iReport)
		{
			m_cboReportType.SetCurSel(iReport-1);
		}
		else
		{
			CString sReport;
			sReport.Format("WxReport_%02d", iReport);
			m_cboReportType.InsertString(m_cboReportType.GetCount(), sReport) ;
		}

		LoadReportType(iReport);
	}



	if(m_arDominoList.Lookup("DEBUG", sData) )	m_bTestMode = TRUE;
	int iw=390, ih=120;

#if _DEBUG
	m_bTestMode = TRUE;
#endif
	m_bTestMode = TRUE;  //test

	if(m_bTestMode)
		iw=m_iDrawX+3, ih=m_iDrawY+3;


	SendAxisMessage(MAKEWPARAM(sizeDLL, 0), MAKEWPARAM(iw, ih));	
}

void CMainDlg::LoadReportType(int itype)
{
	CString sFile;
	sFile.Format("%s\\tab\\WxReport_%02d", m_strRootDir, itype);//root\tab\newtab
	sFile.Format("%s\\tab\\WxReport_%02d", m_strRootDir, itype);//root\tab\newtab
	m_bTestMode = GetConfigFileInt(sFile, _T("INPUT"), _T("DEBUG"), 0); //

//Query
	CString sRead;
	int iFind{};

	m_clParser.m_pMain = this;
	m_iNextCount = 0;
	CString sTemp = GetConfigFileString(sFile, _T("INPUT"), _T("SENDPAGE"), "");

	if( (iFind=sTemp.Find("^") ) > -1)
	{
		sRead = sTemp.Right(sTemp.GetLength() - iFind - 1);  
		sRead.Replace(";", ""); 
		if(sRead != "")
		{
			m_arDominoList.Lookup(sRead, sTemp);
			if(sTemp != "")
				m_iNextCount = CInt(sTemp);
		}
	}
	else
	{
		if(sTemp != "") m_iNextCount = CInt(sTemp);
	}

	GetDlgItem(IDC_EDITPAGE)->SetWindowText(CStr(m_iNextCount));
	
	
	m_stQuery.inCount = GetConfigFileInt(sFile, _T("INPUT"), _T("INCOUNT"), 0); //
	m_stQuery.strSVCName = GetConfigFileString(sFile, _T("INPUT"), _T("SVCNAME"), "");
	m_stQuery.iLedgerRCnt = GetConfigFileInt(sFile, _T("INPUT"), _T("SVCCOUNT"), 20);
	
	GetDlgItem(IDC_SVCNAMESTATIC)->SetWindowText(m_stQuery.strSVCName);

	
	sRead = GetConfigFileString(sFile, _T("INPUT"), _T("INLENS"), "");
	SplitInt(sRead, DELIMITER, m_stQuery.inInLens);
	sRead = GetConfigFileString(sFile, _T("INPUT"), _T("INDATA"), "");
	SplitString(sRead, DELIMITER, m_stQuery.strInDatas);

	int idominofind = -1;
	CString stmp, sval;
	int i = 0;

	for(i=0; i<m_stQuery.inCount; i++)
	{
		if( (idominofind=m_stQuery.strInDatas[i].Find("D^") ) > -1)
		{
			stmp = m_stQuery.strInDatas[i].Right(m_stQuery.strInDatas[i].GetLength() - idominofind - 2); 
			if( m_arDominoList.Lookup(stmp, sval) )
				m_stQuery.strInDatas[i] = sval;
		}
	}

	sRead = GetConfigFileString(sFile, _T("INPUT"), _T("INDATATYPE"), "");
	SplitString(sRead, DELIMITER, m_stQuery.strInTypes);
	

	m_stOutput.iType = GetConfigFileInt(sFile, _T("OUTPUT"), _T("TYPE"), 0); //--simple:0, simple+grid:1
	m_stOutput.iOutCount = GetConfigFileInt(sFile, _T("OUTPUT"), _T("OUTCOUNT"), 0); //
	m_stOutput.iGridCntIdx  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("GRIDROWSCOUNTIDX"), 0); //
	m_stOutput.iGridStartIdx  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("GRIDSTARTIDX"), 0); //
	m_stOutput.iRowTypeCnt  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("GRID_ROWTYPECNT"), 1); //
	m_stOutput.iLineDataSize  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("LINEDATASIZE"), 140); //
	m_stOutput.iFloatLen  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("FLOATLEN"), 2); //

	m_stOutput.iSimpleCount  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("SIMPLECOUNT"), 0); //
	m_stOutput.iLastSimpleCount  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("LASTSIMPLECOUNT"), 0); //
	
	m_stOutput.iGridCount  = GetConfigFileInt(sFile, _T("OUTPUT"), _T("GRIDCOUNT"), 0); //
	
	
	sRead = GetConfigFileString(sFile, _T("OUTPUT"), _T("OUTLENS"), "");
	SplitInt(sRead, DELIMITER, m_stOutput.iOutLens);

	sRead = GetConfigFileString(sFile, _T("OUTPUT"), _T("OUTGRIDLENS"), "");
	SplitInt(sRead, DELIMITER, m_stOutput.iOutGridLens);

	sRead = GetConfigFileString(sFile, _T("OUTPUT"), _T("OUTFORMAT"), "");
	int icnt = SplitString(sRead, DELIMITER, m_stOutput.strOutToken);

	for( i=0; i<icnt; i++)
	{
		if(m_stOutput.strOutToken[i].GetLength() > 1)
		{
			m_stOutput.iOutFormat[i] = CInt(m_stOutput.strOutToken[i].Left(1) );
			m_stOutput.strOutToken[i] = m_stOutput.strOutToken[i].Right( m_stOutput.strOutToken[i].GetLength() -1);  
		}
		else
		{
			m_stOutput.iOutFormat[i] = CInt(m_stOutput.strOutToken[i]);
			m_stOutput.strOutToken[i] = "";
		}
	}

	sRead = GetConfigFileString(sFile, _T("OUTPUT"), _T("GRIDFORMAT"), "");
	icnt = SplitString(sRead, DELIMITER, m_stOutput.strOutGridToken);

	for( i=0; i<icnt; i++)
	{
		if(m_stOutput.strOutGridToken[i].GetLength() > 1)
		{
			m_stOutput.iOutGridFormat[i] = CInt(m_stOutput.strOutGridToken[i].Left(1) );
			m_stOutput.strOutGridToken[i] = m_stOutput.strOutGridToken[i].Right( m_stOutput.strOutGridToken[i].GetLength() -1);  
		}
		else
		{
			m_stOutput.iOutGridFormat[i] = CInt(m_stOutput.strOutGridToken[i]);
			m_stOutput.strOutGridToken[i] = "";
		}
	}	

	m_clParser.setFile(sFile); 
	m_clParser.LoadForm(); 

	if(!m_clParser.m_iPaperMode )
	{
		m_iDrawX = 1230;
		m_iDrawY = 725;
	}

	CString sTitle = "출력[" +m_clParser.m_strPrnDocName + "]";
	SendAxisMessage(MAKEWPARAM(variantDLL, titleCC), (LPARAM)(LPCTSTR)sTitle);
	GetDlgItem(IDC_DOCNAMESTATIC)->SetWindowText(m_clParser.m_strPrnDocName);

	OnTest() ;
}

CString CMainDlg::GetEncPassword(CString sPswd)
{
	CString slog;
	
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_strRootDir, _T("\\dev\\CX_SHA256.DLL"));
	CString strRetrun;
	
	
	
	if(m_hiSha256 == NULL)
	{
		m_hiSha256 = LoadLibrary(dllPath);
		slog.Format("[print] GetEncPassword = [%s]\n", sPswd);
		OutputDebugString(slog);
		if (!m_hiSha256)
		{
			TRACE("CX_SHA256 컨트롤 생성 실패1");
			return "";
		}
	}
	
	if(m_hiSha256)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*, int);	
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(m_hiSha256, "axEncrypt");
		
		if (func)
		{
			
			strRetrun =  (char*)func((LPSTR)(LPCTSTR)sPswd,sPswd.GetLength());
			slog.Format("[print] GetEncPassword = [%s]\n", strRetrun);
			OutputDebugString(slog);
		}
		
	}	
	
	return strRetrun;
}

CString CMainDlg::GetLedger(CString svcd, CString strAccount, char cMarket, char cOdrf, CString sNext) 
{
	struct	_ledgerH ledger;
	CString sBrno = strAccount.Left(3); 

	FillMemory(&ledger, L_ledgerH, _T(' '));
	SendAxisMessage(ledgerDLL, (LPARAM)&ledger);

	CopyMemory(&ledger.size, "000590", sizeof(ledger.size));
	CopyMemory(&ledger.svcd, svcd, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)m_strUserID, m_strUserID.GetLength());
	CopyMemory(&ledger.brno, sBrno, 3);

	if(m_bOneWay)
	{
		CString sPswd, stmp;
		if(m_arDominoList.Lookup("z", stmp))
		{
			stmp.TrimRight();
			CopyMemory(&ledger.hsiz, "44", sizeof(ledger.hsiz));
			CopyMemory(&ledger.epwd, GetEncPassword(stmp), GetEncPassword(stmp).GetLength());
		}
	}

	CString sCnt;
	sCnt.Format("%04d", m_stQuery.iLedgerRCnt); 
	CopyMemory(&ledger.rcnt, sCnt, sizeof(ledger.rcnt));
	
	ledger.fkey[0] = _T('C');
	ledger.mkty[0] = cMarket;
	ledger.odrf[0] = cOdrf;//화면dll-이게 조회키

	if (!sNext.IsEmpty())
	{
		CopyMemory(&ledger.next, "Y", 1);
		CopyMemory(&ledger.nkey, sNext, sNext.GetLength());
	}
	else
		CopyMemory(&ledger.next, " ", 1);
	

	CString sData = CString( (char*)&ledger, L_ledgerH);
	return sData;
}

void CMainDlg::OnTest() 
{
	if(m_stQuery.strSVCName.IsEmpty() )
	{
		AfxMessageBox("Form Laod 오류");
		return;
	}

	GetDlgItem(IDC_EDIT2)->SetWindowText("");
	CString sAccount;
	GetDlgItem(IDC_ACCOUNTEDIT)->GetWindowText(sAccount);
	sAccount = RemoveToken(sAccount, '-');

	
	sendTransaction(sAccount, m_stQuery.strSVCName, m_stQuery.inCount, m_stQuery.strInTypes, m_stQuery.inInLens, m_stQuery.strInDatas) ;
}


void CMainDlg::sendTransaction(CString sAccount, CString strSVCD, int iInputCnt, CString* sInputType, int* iInputLen, CString* sInputData) 
{
	GetDlgItem(IDC_MSGSTATIC)->SetWindowText("");
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	SetPrinterStatus("데이타 요청중입니다..");

	CString sMarket, sOrdf;
	GetDlgItem(IDC_MARKETEDIT)->GetWindowText(sMarket);
	GetDlgItem(IDC_ODRFEDIT)->GetWindowText(sOrdf);
	if(sMarket.IsEmpty() ) sMarket = "2"; 
	if(sOrdf.IsEmpty() ) sMarket = "1";

	sAccount = RemoveToken(sAccount, '-');
	CString sLedger; 

	if(m_bNext) sLedger = m_strLedger; 
	else sLedger = GetLedger(strSVCD, sAccount, sMarket.GetAt(0) , sOrdf.GetAt(0) ); 

	CString szSend = sLedger, sTmp;
	CString sLog;

	CString sfiltoken=_T(" ");
	int		ifiltype = 0;

	m_clxQueryList.DeleteAllItems(); 
	for(int i=0; i<iInputCnt; i++)
	{
		sfiltoken=_T(" ");
		ifiltype = 0;
		if(sInputType[i] == "S" || sInputType[i] == "N")
		{
			sfiltoken = _T("0");
			ifiltype = 1;
		}

		sTmp = sInputData[i];
		sTmp = filltoken(sTmp, iInputLen[i], sfiltoken, ifiltype);
		if(sInputType[i] == "S") sTmp = _T("+") + sTmp;

		szSend += sTmp;
		InsertQueryList(sTmp);
	}

	const int iDataLen =  szSend.GetLength();

	szSend.Delete(0, 6); 
	CString sLen;
	sLen.Format("%06d",  iDataLen);
	szSend.Insert(0, sLen); 

// 	OutputDebugString("[KSJ] data[" + szSend + "]");

	sendTR("PIBOPBXQ", (char*)(LPCTSTR)szSend, iDataLen , 0x01);

	SetTimer(SENDWAIT, 10000, NULL);
}

void CMainDlg::InsertQueryList(CString sData)
{
	int ino = m_clxQueryList.GetItemCount();
	ino = m_clxQueryList.InsertItem(m_clxQueryList.GetItemCount() , CStr(ino+1)); 
	m_clxQueryList.SetItemText(ino, 1, sData); 
}

void CMainDlg::InsertOutputGridList(int icolcnt)
{
	int iadd = 0;
	m_clxOutputList.DeleteAllItems(); 
	for(int i=0; i<m_arGridList.GetSize(); i++)
	{
		iadd = m_clxOutputList.InsertItem(m_clxOutputList.GetItemCount(), CStr(i+1)  );
		for(int ic=0; ic<icolcnt; ic++)
			m_clxOutputList.SetItemText(iadd, ic+1, m_arGridList.GetAt(i).strData[ic]); 
	}
}

void CMainDlg::ParsingOubData(CString sReceive)
{
//	m_bNext = FALSE;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	if(sReceive.GetLength() <  L_ledgerH) return;

	m_strLedger = sReceive.Left(L_ledgerH);
//	AfxMessageBox(m_strLedger);
	struct	_ledgerH ledger;
	CopyMemory(&ledger, m_strLedger, L_ledgerH);
	const int itotlen = CInt( CString(ledger.size, 6) );
	
	CString sErrCode = CString(ledger.ecod, sizeof(ledger.ecod) );
	CString sErrMsg = CString(ledger.emsg, sizeof(ledger.emsg) );

	
	if( CString(ledger.next, 1) == "Y" ) m_bNext = TRUE;
	else 
	{
		m_strLedger = "";
		m_bNext = FALSE;
	}
	if(m_iQueryCount >= m_iNextCount) m_bNext = FALSE;

	if(m_bNext) m_bListAdd = TRUE;

//	오류코드 :(정상코드: 0000 ~ 0999, 비정상코드 : 1000 ~ 9999, 요청시 Space로 채움)
	GetDlgItem(IDC_MSGSTATIC)->SetWindowText(sErrMsg);
	SetPrinterStatus(sErrMsg);
	const int iErrCode = atoi(sErrCode);
	if(iErrCode > 999)
	{
		GetDlgItem(IDC_MSGSTATIC)->SetWindowText(sErrMsg);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		m_iQueryCount = 0;
		return;
	}
	
	
	sReceive = sReceive.Right(sReceive.GetLength() -   L_ledgerH);
	switch(m_stOutput.iType)
	{
		case 2:
			ParsingMultiData(sReceive);
			break;

		case 0:
		case 1:
		case 3:
			ParsingSimpleData(sReceive, m_stOutput.iType);
			break;
	}

	
	if(m_bNext)
	{	
		GetDlgItem(IDC_MSGSTATIC)->SetWindowText("다음조회중...");
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
		CString sAccount;
		GetDlgItem(IDC_ACCOUNTEDIT)->GetWindowText(sAccount);
		sAccount = RemoveToken(sAccount, '-');
		sendTransaction(sAccount, m_stQuery.strSVCName, m_stQuery.inCount, m_stQuery.strInTypes, m_stQuery.inInLens, m_stQuery.strInDatas) ;
		m_iQueryCount++;
	}
	else 
	{
		m_iQueryCount = 0;
		m_strLedger = "";
		m_bExecPrn = true;
		
		m_clParser.setOutputValue();
		SetTimer(PRINTWAIT, 2000, NULL);
	}
}

void CMainDlg::ParsingMultiData(CString sData)
{
	int isimplecnt = 0;
	for(int i=0; i<m_stOutput.iSimpleCount; i++)
		isimplecnt += m_stOutput.iOutLens[i]; 

	CString strSimpleData = sData.Left(isimplecnt);
	CString	strGridData = sData.Right(sData.GetLength() - isimplecnt);

	strSimpleData += ParsingGridData(strGridData);
	ParsingSimpleData(strSimpleData);
}

void CMainDlg::ParseSum(CString sSumdata)
{	
	int i = 0;
	CString temp = "", temp_2 = "";

	while(AfxExtractSubString(sParseData[i], sSumdata, i, 0x09))
	{
		i++;
	}

	i = 0;
	CString tmp;
	while(sParseData[i] != "")
	{
		tmp = sParseData[i];
		temp = tmp.Left(1);

		if(temp == "-")
		{
			tmp = sParseData[i];
			
			tmp = tmp.Right(15);
			tmp.TrimLeft(); 
			tmp.TrimRight();

			sParseData[i] = temp + tmp; 
		}
		else
		{
			tmp = sParseData[i];
			
			tmp = tmp.Right(15);
			tmp.TrimLeft(); 
			tmp.TrimRight();
			
			sParseData[i] = tmp; 

		}
		i++;
	}
}

void CMainDlg::ParsingData(int ilinecnt, int irowcnt, CString sData, int istartidx, int icolcnt, int* iLenlist, int* iFormatList, 
						   CString* sTokenList, COutDataListArray& arDataList)
{
	CString		sList[MAXCOUNT], sLine;// , sTmp;
//	CString srtToken = "";
	int idata = 0;
//	int count = 0;
	for(int ir=0; ir<=irowcnt; ir++)
	{
		idata = 0;
		sLine = sData.Left( ilinecnt);
		sData = sData.Right(sData.GetLength() - ilinecnt);

		for(int i=istartidx; i<icolcnt; i++)
		{
			//그리드 데이터
			if(sLine.GetLength() <  iLenlist[i] ) 
			{
				const int ilen = sLine.GetLength();
				sList[idata++] = SetDataFormat(iFormatList[i], sLine.Left(ilen), sTokenList[i], m_stOutput.iFloatLen);
				break;
			}

			sList[idata++] = SetDataFormat(iFormatList[i], sLine.Left( iLenlist[i]), sTokenList[i], m_stOutput.iFloatLen);

			sLine = sLine.Right(sLine.GetLength() - iLenlist[i]);
		}

		arDataList.Add( OutDataList(1, icolcnt, sList) );
		if(sData.IsEmpty()) break; 
		if(sData.GetLength() <  ilinecnt) break;
	}
}

void CMainDlg::ParsingSimpleData(CString sData, int itype)
{

	if(sData.IsEmpty() ) return;
	GetDlgItem(IDC_EDIT2)->SetWindowText(sData);
	COutDataListArray	arList;
	ParsingData(sData.GetLength(), 1, sData, 0, m_stOutput.iSimpleCount+m_stOutput.iLastSimpleCount, m_stOutput.iOutLens, m_stOutput.iOutFormat, 
						   m_stOutput.strOutToken, arList);
	m_OubList.ResetContent();
	if(arList.GetSize() <= 0 ) return;

	OutDataList pList = arList.GetAt(0);
	CString sMultiline;

	int imultiIdx = 0;
	int i = 0;
	for(i=0; i<m_stOutput.iSimpleCount+m_stOutput.iLastSimpleCount; i++)
	{
		if( m_stOutput.iOutFormat[i] == 9)
		{
			m_stSimpleList.strData[i] += pList.strData[i]; 
			sMultiline = m_stSimpleList.strData[i];
			imultiIdx = i;
		}
		else {
			if(sDomino02 == "WxReport02")
			{
				if ( i>=8 && i<=25 ) 
				{
					m_stSimpleList.strData[i] = sParseData[i-8];
				} 
				else 
				{
					m_stSimpleList.strData[i] = pList.strData[i];
				}
			}
			else
			{
				m_stSimpleList.strData[i] = pList.strData[i];
			}
		}
	}

	for( i=0; i<m_stOutput.iSimpleCount+m_stOutput.iLastSimpleCount; i++)
	{
		m_OubList.InsertString(i, m_stSimpleList.strData[i]); 
	}

	if(!m_bNext)
	{
		if(sMultiline != "")
		{
			GetDlgItem(IDC_EDIT2)->SetWindowText(sMultiline);
			CString sText;
			const int idatacnt = sMultiline.GetLength(); 
			char szmulti[1024*64];
			strcpy(szmulti, sMultiline);
			const int imultilen = strlen(szmulti);

			const int icolcnt = m_stOutput.iLineDataSize;
			const int iline = idatacnt/icolcnt;
			const int idiv = idatacnt % icolcnt;
			CString stmp, slog;

			m_arMultiLines[imultiIdx].RemoveAll(); 
			for(int i=0; i<iline; i++)
			{
				stmp = CString(&szmulti[i*icolcnt], icolcnt);
				m_arMultiLines[imultiIdx].Add( stmp);
				slog += stmp+"\r\n";
			}

			GetDlgItem(IDC_EDIT2)->SetWindowText(slog);
		}	
	}
}

CString CMainDlg::ParsingGridData(CString sData)
{
	if(!m_bListAdd)
	{
		m_arGridList.RemoveAll(); 
	}

	GetDlgItem(IDC_EDIT2)->SetWindowText(sData);
	CString sTmp = sData.Left( m_stOutput.iOutGridLens[0]);
	const int irows = CInt(sTmp);
	sData = sData.Right(sData.GetLength() - m_stOutput.iOutGridLens[0]);

	const int igridcolcnt = m_stOutput.iOutCount - m_stOutput.iSimpleCount - 1 - m_stOutput.iLastSimpleCount;

	CString		sList[MAXCOUNT], sLine;
	int ilinecnt = 0, i = 0;
	for(i=m_stOutput.iGridStartIdx; i<(m_stOutput.iGridStartIdx+igridcolcnt); i++)
		ilinecnt += m_stOutput.iOutGridLens[i];

	CString simpleData="";
	const int itotrec = ilinecnt * irows;

	if(m_stOutput.iLastSimpleCount)
	{
		const int iend = m_stOutput.iSimpleCount - m_stOutput.iLastSimpleCount ;
		int isimplecnt = 0;
		for( i=m_stOutput.iSimpleCount-1; i>iend; i--)
			isimplecnt += m_stOutput.iOutLens[i]; 

		simpleData = sData.Mid( itotrec, sData.GetLength()- itotrec);
	}
	ParsingData(ilinecnt, irows, sData, m_stOutput.iGridStartIdx, igridcolcnt, m_stOutput.iOutGridLens, m_stOutput.iOutGridFormat, 
						   m_stOutput.strOutGridToken, m_arGridList);
	if(!m_bNext)
		InsertOutputGridList(igridcolcnt);
	return simpleData;
}





void CMainDlg::OnPrnTest() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	SetPrinterStatus("인쇄 준비중...");
	CWxReport	pReport;
	pReport.m_pMain = this;
	pReport.Print(); 
}

void CMainDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here

	const int isel = m_cboReportType.GetCurSel(); 
	if(isel < 0) return;

	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_MSGSTATIC)->SetWindowText("");
	m_clxQueryList.DeleteAllItems(); 	
	m_clxOutputList.DeleteAllItems(); 	

	m_arGridList.RemoveAll(); 

	m_OubList.ResetContent(); 
	LoadReportType(isel+1);
}

CString CMainDlg::GetInputData(int idx)
{
	return m_stQuery.strInDatas[idx];
}

CString CMainDlg::GetDominoData(CString sDominoSymbol)
{
	CString sval;
	m_arDominoList.Lookup(sDominoSymbol, sval);
	return Trim(sval);
}

CString CMainDlg::GetOutputData(CString sText)
{
	CString sResult = "";
	int idx = 0;
	if( isNumber(sText) )
	{
		idx = CInt(sText);
		sResult = m_stSimpleList.strData[idx];  
	}
	else
	{
		int idxlist[10]{}, iloop = 0;
		CString stmp = "", sToken[10];
		int i = 0;
		for(int i=0; i<10; i++)
		{
			sToken[i] = "";
			idxlist[i] = 0;
		}

		for( i=0; i<sText.GetLength(); i++)
		{
			if( isNum(sText.GetAt(i) ) )
				stmp += sText.GetAt(i);
			else
			{
				sToken[iloop] += sText.GetAt(i);
				if(stmp != "")
				{
					idxlist[iloop++] = CInt(stmp);
					stmp = "";
				}
			}
		}

		if(stmp != "")
		{
			idxlist[iloop++] = CInt(stmp);
			stmp = "";
		}
		
		for(i=0; i<iloop; i++)
		{
			idx = idxlist[i];
			sResult += m_stSimpleList.strData[idx] + sToken[i];
		}
	}

	return sResult;
}

/*
-0:output
-1:sign number + comma
-2:number + comma
-3:account
-4:yyyymmdd
-5:mmdd
-6:hhmmss
-7:mmss
*/



int CMainDlg::GetGridTextMargin()
{
	return m_clParser.m_iGridTextMargin;
}
int CMainDlg::GetTextMargin()
{
	return m_clParser.m_iTextMargin;
}

CString CMainDlg::GetDocName()
{
	return m_clParser.m_strPrnDocName;
}

CString CMainDlg::GetCurrentDate(CString stoken)
{
	const CTime time = CTime::GetCurrentTime();
	CString stime;
	stime.Format("%04d%s%02d%s%02d", time.GetYear(), stoken, time.GetMonth(), stoken, time.GetDay() );    

	return stime;
}

CString CMainDlg::GetCurrentTime1(CString stoken)
{
	const CTime time = CTime::GetCurrentTime();
	CString stime;
	stime.Format("%02d%s%02d%s%02d", time.GetHour (), stoken, time.GetMinute(), stoken, time.GetSecond() );    

	return stime;
}

CString CMainDlg::GetHeaderText(CString sText)
{
	CString sResult = sText;
	if(sText.Find("DATE") > -1)
	{
		CString sToken = "-";
		if(sText.GetLength() > 4)  sToken = sText.Right(1); 
		return GetCurrentDate(sToken);
	}

	if(sText.Find("TIME") > -1)
	{
		CString sToken = ":";
		if(sText.GetLength() > 4)  sToken = sText.Right(1); 
		return GetCurrentTime1(sToken);
	}

	return sResult;
}


void CMainDlg::SetProgressRange(int iMax, int iMin) 
{
	m_Progress.SetRange(iMin, iMax); 
	m_Progress.SetPos(iMin); 
}

void CMainDlg::SetProgressPos(int iPos) 
{
	if(iPos < 0)
	{
		int ilower, iupper;
		m_Progress.GetRange(ilower, iupper);
		iPos = iupper;
	}
	else
		iPos += m_Progress.GetPos();

	m_Progress.SetPos(iPos); 
}

void CMainDlg::SetPrinterStatus(CString sMessage) 
{
	GetDlgItem(IDC_PRNMSGSTATIC)->SetWindowText(sMessage);
}
void CMainDlg::SetPrinterPage(CString sPage) 
{
	GetDlgItem(IDC_PAGESTATIC)->SetWindowText(sPage);
}

void CMainDlg::ending()
{
	SetTimer(ENDWAIT, 1000, NULL);
}

void CMainDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	if(ENDWAIT == nIDEvent)
		SendAxisMessage(MAKEWPARAM(closeDLL, 0), 0);
	if(PRINTWAIT == nIDEvent)
	{
		
		m_bFirstData = TRUE;
		if(!m_bTestMode)
			OnPrnTest() ;
		else 
		{
			if(m_bExecPrn)GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
			RedrawWindow();
		}
	}

	if(SENDWAIT == nIDEvent)
	{
		SetPrinterStatus("데이타 요청오류입니다. 잠시후 재시도 하십시요.");
	}

	CDialog::OnTimer(nIDEvent);
}


int CMainDlg::GetMultiLineSze(int icontentIDX)
{
	return m_arMultiLines[icontentIDX].GetSize(); 
}

int CMainDlg::GetMultiLineSze(int icontentIDX, CStringArray& arMultiLines)
{
	arMultiLines.Copy( m_arMultiLines[icontentIDX]);
	return m_arMultiLines[icontentIDX].GetSize(); 
}


void CMainDlg::DrawScreen(CDC* pDC)
{
	CWxReport	pReport;
	pReport.m_pMain = this;
	const CRect drect = CRect(400, 5, m_iDrawX, m_iDrawY);
	pReport.SetDrawRect(pDC,  drect);
	m_iScreenTotPage = pReport.DrawScreen(pDC, drect, m_iPreviewPage);

	CString sProg;
	sProg.Format("%d / %d",  m_iPreviewPage+1, m_iScreenTotPage);
	GetDlgItem(IDC_PROGSTATIC)->SetWindowText(sProg);

}
void CMainDlg::OnNextPAge() 
{
	// TODO: Add your control notification handler code here
	m_iPreviewPage++;
	if(m_iScreenTotPage < m_iPreviewPage ) m_iPreviewPage = m_iScreenTotPage;

	CString sProg;
	sProg.Format("%d / %d",  m_iPreviewPage+1, m_iScreenTotPage);
	GetDlgItem(IDC_PROGSTATIC)->SetWindowText(sProg);
	InvalidateRect(CRect(400, 5, m_iDrawX, m_iDrawY));
}

void CMainDlg::OnPrevPage() 
{
	// TODO: Add your control notification handler code here
	m_iPreviewPage--;
	if(m_iPreviewPage < 0) m_iPreviewPage = 0;
	CString sProg;
	sProg.Format("%d / %d",  m_iPreviewPage+1, m_iScreenTotPage);
	GetDlgItem(IDC_PROGSTATIC)->SetWindowText(sProg);

	InvalidateRect(CRect(400, 5, m_iDrawX, m_iDrawY));
}

void CMainDlg::OnClosebutton() 
{
	// TODO: Add your control notification handler code here
	SendAxisMessage(MAKEWPARAM(closeDLL, 0), 0);	
}
