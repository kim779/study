// DlgResult.cpp : implementation file
//

#include "stdafx.h"
#include "DlgResult.h"
#include ".\dlgresult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgResult dialog


CDlgResult::CDlgResult(CWnd* pParent /*=NULL*/)
	: CEscDialog(CDlgResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pFont = NULL;
}


void CDlgResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgResult)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgResult, CDialog)
	//{{AFX_MSG_MAP(CDlgResult)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgResult message handlers

BOOL CDlgResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brBackground.CreateSolidBrush( CLR_BACKGROUND);
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgResult::InitGrid()
{
	if(!m_GridResult.GetSafeHwnd())
	{
		CRect rect1;
		GetClientRect(rect1);
		rect1.top =1;
		rect1.left +=2;
		rect1.bottom +=3;
		if(!m_GridResult.Create(rect1,this,IDC_STATIC_RESULT,WS_CHILD | WS_TABSTOP | WS_VISIBLE ))
		{
			AfxMessageBox("No");
			return;
		}
		m_GridResult.SetEditable(FALSE);
		m_GridResult.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
		m_GridResult.SetGridLineColor(RGB(204,210,224));
		m_GridResult.SetColumnCount(6);
		m_GridResult.SetRowCount(14);
		m_GridResult.SetBkColor(CLR_BACKGROUND );
		if(m_pFont)
			m_GridResult.SetFont(m_pFont);

		CString str;
		GV_ITEM Item;
		int nWidth = (rect1.right/6) -3;
		char* IndexName[47]={"누적수익률","총손익","총이익/총손실","건별평균손익","총진입","","총매매수","이익수/손실수","기간등락률","전략초과수익률","","총봉수","기간참여율","평균매매봉수",
			"건별평균수익률","총이익","건별최대이익","건별평균이익","미청산포함총청산","","이익매매수","이익매매율(승률)","최대연속이익수","최대연속이익","","이익매매봉수","진입상태봉수","이익매매평균봉수",
			"현재포지션수익률","총손실","건별최대손실","건별평균손실"," ","","손실매매수","손실매매율(패율)","최대연속손실수","최대연속손실","","손실매매봉수","청산상태봉수","손실매매평균봉수"};
		int nRowIndex =0;
		for (int col = 0; col < 6; col++)
		{
			Item.mask = GVIF_TEXT;
			m_GridResult.SetColumnWidth(col,nWidth);
			if(!(col%2))
			{
				Item.nFormat = DT_CENTER;
				for(int row = 0; row<14;row++)
				{
					m_GridResult.SetRowHeight(row,23);
					if(IndexName[nRowIndex]=="")
					{
						m_GridResult.SetItemBkColour(row,col,RGB(204,210,224));
						m_GridResult.SetItemBkColour(row,col+1,RGB(204,210,224));
					}
					else
						m_GridResult.SetItemBkColour(row,col,RGB(242,248,254));
					m_GridResult.SetItemText(row,col,IndexName[nRowIndex]);
					++nRowIndex;
				}
			}

		}
		m_GridResult.ExpandColumnsToFit();
	}
}


void CDlgResult::SetDataSpreadGrid()
{
	CString strName;
	ST_DATAINDEX stData;
	double Data;
	CString strData;
	DWORD nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//그리드 포맷

	CIStrategyItem* Strate =  m_pOptimizer->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();

	//누적 수익률 저장 
	Data = Strate->GetStatisticalInfo(0);
	strData.Format("%.2f%%", Data);
	CString strACCumProf;
	strACCumProf = strData;
	m_GridResult.SetItemText(0,1,strData,nFormat);//Grid

	///////////////////////
	////옵션에 따라 출력 : 8자리면 선물 6자리면 주식 옵션 //////////////////////////////////////////////////////////////	

	for(int i = 1; i<MAXDATANUM;i++)
	{
		if(i==14) continue;//미청산은 포함 되지 않습니다.
		if(i==24)// 전략초과수익률:누적수익률 - 기간등락률 적용!
		{
			Data =atof(strACCumProf)-Strate->GetStatisticalInfo(21);
		}
		else
			Data = Strate->GetStatisticalInfo(i);
		//콤마 루틴 필요 Default!! 
		if(i==1||i==2||i==6||i==18||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
			strData.Format("%.2f", Data);
		else
		{
			strData.Format("%d", (INT)Data);
		}
		strData = GetComma(strData);//Comma

		switch(i) 
		{
		case PROFITRATEAVG:
			m_GridResult.SetItemText(0,3,strData+"%",nFormat);//Grid
			break;
		case PROFITRATECURRENT:
			m_GridResult.SetItemText(0,5,strData+"%",nFormat);//Grid		
			break;
		case PROFITLOSSALL:
			m_GridResult.SetItemText(1,1,strData,nFormat);//Grid
			break;
		case PROFITALL:
			m_GridResult.SetItemText(1,3,strData,nFormat);//Grid
			break;
		case LOSSALL:
			m_GridResult.SetItemText(1,5,strData,nFormat);//Grid
			break;
		case PROFITLOSSRATEALL:
			m_GridResult.SetItemText(2,1,strData,nFormat);//Grid
			break;
		case MAXPROFIT:
			m_GridResult.SetItemText(2,3,strData,nFormat);//Gridi
			break;
		case MAXLOSS:
			m_GridResult.SetItemText(2,5,strData,nFormat);//Grid
			break;
		case PROFITLOSSAVG:
			m_GridResult.SetItemText(3,1,strData,nFormat);//Grid
			break;
		case PROFITAVG:
			m_GridResult.SetItemText(3,3,strData,nFormat);//Grid
			break;
		case LOSSAVG:
			m_GridResult.SetItemText(3,5,strData,nFormat);//Grid
			break;
		case AMOUNTENTEREDALL:
			m_GridResult.SetItemText(4,1,strData,nFormat);//Grid
			break;
		case AMOUNTEXITALL:
			m_GridResult.SetItemText(4,3,strData,nFormat);//Grid
			break;
			//그리드 빈칸 
		case CNTALL:
			m_GridResult.SetItemText(6,1,strData,nFormat);//Grid
			break;
		case CNTPROFIT:
			m_GridResult.SetItemText(6,3,strData,nFormat);//Grid
			break;
		case CNTLOSS:
			m_GridResult.SetItemText(6,5,strData,nFormat);//Grid
			break;
		case RATEPROFITLOSS:
			m_GridResult.SetItemText(7,1,strData,nFormat);//Grid
			break;
		case RATEPROFIT:
			m_GridResult.SetItemText(7,3,strData+"%",nFormat);//Grid
			break;
		case RATELOSS:
			m_GridResult.SetItemText(7,5,strData+"%",nFormat);//Grid
			break;
		case CHANGERATE:
			m_GridResult.SetItemText(8,1,strData+"%",nFormat);//Grid
			break;
		case MAXPROFITSTRAIGHT:
			m_GridResult.SetItemText(8,3,strData,nFormat);//Grid
			break;
		case MAXLOSSSTRAIGHT:
			m_GridResult.SetItemText(8,5,strData,nFormat);//Grid
			break;
		case RATEPROFITEXCEED:///전략 초과 수익률 : 누적수익률(0) - 기간등락률(21)
			m_GridResult.SetItemText(9,1,strData+"%",nFormat);//Grid
			break;
		case PROFITSTRAIGHT:
			m_GridResult.SetItemText(9,3,strData,nFormat);//Grid
			break;
		case LOSSSTRAIGHT:
			m_GridResult.SetItemText(9,5,strData,nFormat);//Grid
			break;
			//빈칸
		case CNTCANDLE:
			m_GridResult.SetItemText(11,1,strData,nFormat);//Grid
			break;
		case CNTCANDLEPROFIT:
			m_GridResult.SetItemText(11,3,strData,nFormat);//Grid
			break;
		case CNTCANDLELOSS:
			m_GridResult.SetItemText(11,5,strData,nFormat);//Grid
			break;
		case RATEACCESS:
			m_GridResult.SetItemText(12,1,strData+"%",nFormat);//Grid
			break;
		case CNTCANDLEACCESS:
			m_GridResult.SetItemText(12,3,strData,nFormat);//Grid
			break;
		case CNTCANDLEEXIT:
			m_GridResult.SetItemText(12,5,strData,nFormat);//Grid
			break;
		case CNTCANDLEAVGDEAL:
			m_GridResult.SetItemText(13,1,strData,nFormat);//Grid
			break;
		case CNTCANDLEAVGPROFIT:
			m_GridResult.SetItemText(13,3,strData,nFormat);//Grid
			break;
		case CNTCANDLEAVGLOSS:
			m_GridResult.SetItemText(13,5,strData,nFormat);//Grid
			break;
		}
	}
	m_GridResult.Refresh();
}


void CDlgResult::InitHtml()
{
	if(m_strHtml!="")return;
	CStdioFile DefaultFile;
	CString str;
	
	if(!DefaultFile.Open(m_strDir+"\\IndexOptimizer.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create IndexOptimizer.htm File.");
		return;
	}

	for(;;){
		if(DefaultFile.ReadString(str))
			m_strHtml+=str;
		else
			break;
	}
	m_strHtml.Replace("color=\"white\"", "");//

	DefaultFile.Close();
}
//NEW KHD
//Html 파일의 String을 로드하여 파싱하여 데이타를 채우고 Html파일을 생성 한다. 
void CDlgResult::MakeHtmlFile()
{
	//성과 분석 파일 로드!!
	//	m_pStrategyItem->

	CStdioFile NewFile;
	///String 파싱루틴 //////////////////////..
	CString strHtml;
	InitHtml();
	strHtml = m_strHtml;
	strHtml = ParsingstrHtml(strHtml);
	//	strHtml.Replace("white", "Black");//
	//////NewFile에 쓰기 1. 성과 분석,전진분석,매매내역루틴 필요//////////////////////////
	SetFileAttributes (m_strDir+"\\IndexOptimizerdata.htm", FILE_ATTRIBUTE_NORMAL);//protect Read Only Error
	if(!NewFile.Open(m_strDir+"\\IndexOptimizerdata.htm",CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}else
		NewFile.WriteString(strHtml);

	NewFile.Close();
}
CString CDlgResult::ParsingstrHtml(CString strHtml)
{
	//속도 향상을 위해 미리 파싱할 변수 배열에 저장 //
	double Data;
	CString strData;
	CString m_pHtmlCodeNameArray[MAXDATANUM] = 
	{"AccumProf","AAverProf","CurPosRate","TotalSon","TotalProf",
	"Totalloss","TotalPL","MaxProf","MaxLoss","AverSon","AverProf",
	"AverLoss",	"TotalEnt","NoCleanTotal","NoClean","TotalTrade",
	"ProfTrade","LossTrade","PnLn","PTRate","LTRate",
	"PrdSign","MaxConProfN","MaxConLossN","SOverRate","MaxConProf",
	"MaxConLoss","TotalStick","PTStick","LTStick","PrdParty",
	"EntryStick","CleanStick","ATStick","PTAStick","PLTAStick"};
	int m_nViewPointIndex[11] = {15,16,17,18,22,23,27,28,29,31,32}; 
	/////////////////////////////////

	ST_DATAINDEX stData;
	CIStrategyItem* Strate = m_pOptimizer->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	if(nSize<0) return "";

	//누적 수익률 저장 
	Data = Strate->GetStatisticalInfo(0);
	strData.Format("%.2f%%", Data);
	_tprintf("%s", (LPCTSTR) strData);
	CString strACCumProf;
	strACCumProf = strData;
	strHtml.Replace("AccumProf",strData);

	///////////////////////
	////옵션에 따라 출력 : 8자리면 선물 6자리면 주식 옵션 ////////////////////////////////////			
	for(int i = 1; i<MAXDATANUM;i++){
		if(i==14) continue;//미청산은 포함 되지 않습니다.
		if(i==24)// 전략초과수익률:누적수익률 - 기간등락률 적용!
		{
			Data =atof(strACCumProf)-Strate->GetStatisticalInfo(21);
		}
		else
			Data = Strate->GetStatisticalInfo(i);
		//콤마 루틴 필요 Default!! 
		if(i==1||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
			strData.Format("%.2f", Data);
		else
			strData.Format("%d", (INT)Data);

		int Index = strHtml.Find(m_pHtmlCodeNameArray[i],0);
		strData = GetComma(strData);//KHD :콤마
		strHtml.Replace(m_pHtmlCodeNameArray[i],strData);
	}
	return strHtml;
}

CString  CDlgResult::GetNumber( LPCSTR pData )
{
	CString szRet=_T("");
	int nLen = strlen(pData);
	char	ch;

	for(int i=0;  i<nLen ; i++) {
		ch = pData[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	return szRet;
}

CString CDlgResult::GetComma(LPCSTR pData)
{
	//KHD : 오류수정 
	double ddata = atof(pData);
	if(ddata ==0) return "0";

	CString szRet=_T("");
	CString szTmp = pData;
	CString szTmp2;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	szTmp = GetNumber(szTmp);
	if(szTmp.IsEmpty())	return "" ;
	TCHAR ch=' ';
	if(szTmp[0]=='+' ||szTmp[0]=='-') 
	{
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}
	BOOL IsFloat;
	int Index =szTmp.Find(".",0); 
	if(Index>0)
		IsFloat = TRUE;
	else
		IsFloat = FALSE;

	int	nLen;
	if(IsFloat)//소숫점 자리이면... 이렇게한다.
	{
		szTmp2 = szTmp.Mid(Index,szTmp.GetLength()-Index);
		szTmp.Delete(Index,szTmp.GetLength()- Index);
		
	}
	szTmp.MakeReverse();
	nLen= szTmp.GetLength();
	szRet.Empty();
	for(int i=0;  i<nLen;   i++) 
	{
		if(i!=0 && !(i%3))	szRet += ',';
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	szRet.MakeReverse();
	if(IsFloat)
		szRet+=szTmp2;
	return szRet;

}
////////////////KHD
//
//void CDlgResult::MakeHtmlFile()
//{
//	//성과 분석 파일 로드!!
////	m_pStrategyItem->
//
//
//}
//
//CString CDlgResult::ParsingstrHtml(LPARAM lParam)//그리드 데이터가 이리루 온다.
//{
//		//속도 향상을 위해 미리 파싱할 변수 배열에 저장 //
//	CStdioFile NewFile;
//	CString strHtml;
//	strHtml = m_strHtml;
//	ST_GRIDVALUEDATA *GridData = (ST_GRIDVALUEDATA*)lParam;
//
//	if(!NewFile.Open(m_strDir+"\\IndexOptimizerData.htm",CFile::modeCreate|CFile::modeWrite))
//	{
//		AfxMessageBox("Do Not Create Html File.");
//	}
////	double Data;
////	CString strData;
//	CString m_pHtmlCodeNameArray[MAXDATANUM] = 
//	{"AccumProf","AAverProf","CurPosRate","TotalSon","TotalProf",
//	"Totalloss","TotalPL","MaxProf","MaxLoss","AverSon","AverProf",
//	"AverLoss",	"TotalEnt","NoCleanTotal","NoClean","TotalTrade",
//	"ProfTrade","LossTrade","PnLn","PTRate","LTRate",
//	"PrdSign","MaxConProfN","MaxConLossN","SOverRate","MaxConProf",
//	"MaxConLoss","TotalStick","PTStick","LTStick","PrdParty",
//	"EntryStick","CleanStick","ATStick","PTAStick","PLTAStick"};
//	int m_nViewPointIndex[11] = {15,16,17,18,22,23,27,28,29,31,32}; 
//	/////////////////////////////////
//	
//		strHtml.Replace(m_pHtmlCodeNameArray[0],GridData->RateAccm);
//		strHtml.Replace(m_pHtmlCodeNameArray[1],GridData->AAerProf);
//		strHtml.Replace(m_pHtmlCodeNameArray[2],GridData->CurPosRate);
//		strHtml.Replace(m_pHtmlCodeNameArray[3],GridData->RateCurrent);
//		strHtml.Replace(m_pHtmlCodeNameArray[4],GridData->ProfitAll);
//		strHtml.Replace(m_pHtmlCodeNameArray[5],GridData->LossAll);
//		strHtml.Replace(m_pHtmlCodeNameArray[6],GridData->TotalPL);
//
//		strHtml.Replace(m_pHtmlCodeNameArray[7],GridData->MaxProfit);
//		strHtml.Replace(m_pHtmlCodeNameArray[8],GridData->MaxLoss);
//		strHtml.Replace(m_pHtmlCodeNameArray[9],GridData->LossSavg);
//		strHtml.Replace(m_pHtmlCodeNameArray[10],GridData->AverProf);
//		strHtml.Replace(m_pHtmlCodeNameArray[11],GridData->AverLoss);
//
//		strHtml.Replace(m_pHtmlCodeNameArray[12],GridData->TotalEnt);
//		strHtml.Replace(m_pHtmlCodeNameArray[13],GridData->NoCleanTotal);
//		strHtml.Replace(m_pHtmlCodeNameArray[15],GridData->ProfitFactor);//??
//		strHtml.Replace(m_pHtmlCodeNameArray[15],GridData->CntAll);
//		strHtml.Replace(m_pHtmlCodeNameArray[16],GridData->ProfTrace);
//		strHtml.Replace(m_pHtmlCodeNameArray[17],GridData->LossTrade);
//
//		strHtml.Replace(m_pHtmlCodeNameArray[18],GridData->RateProfitLoss);
//		strHtml.Replace(m_pHtmlCodeNameArray[19],GridData->PTRate);
//		strHtml.Replace(m_pHtmlCodeNameArray[20],GridData->LTRate);
//		strHtml.Replace(m_pHtmlCodeNameArray[21],GridData->PriodSign);
//		strHtml.Replace(m_pHtmlCodeNameArray[22],GridData->MaxConProfN);
//		strHtml.Replace(m_pHtmlCodeNameArray[23],GridData->MaxConLossN);
//		strHtml.Replace(m_pHtmlCodeNameArray[24],GridData->SOverRate);
//		strHtml.Replace(m_pHtmlCodeNameArray[25],GridData->MaxConProf);
//		strHtml.Replace(m_pHtmlCodeNameArray[26],GridData->MaxConLoss);
//
//		strHtml.Replace(m_pHtmlCodeNameArray[27],GridData->TotalStick);
//		strHtml.Replace(m_pHtmlCodeNameArray[28],GridData->PTStick);
//		strHtml.Replace(m_pHtmlCodeNameArray[29],GridData->LTStick);
//		strHtml.Replace(m_pHtmlCodeNameArray[30],GridData->PriodParty);
//		strHtml.Replace(m_pHtmlCodeNameArray[31],GridData->EntryStick);
//		strHtml.Replace(m_pHtmlCodeNameArray[32],GridData->CleanStick);
//		strHtml.Replace(m_pHtmlCodeNameArray[33],GridData->AtStick);
//		strHtml.Replace(m_pHtmlCodeNameArray[34],GridData->PTAStick);
//
//		strHtml.Replace(m_pHtmlCodeNameArray[35],GridData->PLTAStick);
//		
//
//	
//
//		strHtml.Replace("StrategyName",m_pOptimizer->m_StragyName);
//		strHtml.Replace("ItemName", m_pOptimizer->m_Name);
/////String 파싱루틴 //////////////////////..
//	
//	NewFile.WriteString(strHtml);
//		NewFile.Close();
//	OpenWebView("\\IndexOptimizerData.htm");
//
////	CString strName;
////	CStrategyResult* Result;
////	ST_DATAINDEX stData;
////	CIStrategyItem* Strate = m_pOptimizer->GetStrategyItem();//m_pOptimizer->GetStrategyItem();
////	
////	int nSize = Strate->GetCntStrategyResult();
////	if(nSize > 0)
////	{
////		Result = Strate->GetStrategyResult(nSize-1);
////		switch(Result->m_DealType) {
////		case CStrategyResult::NONE:strName = "NONE";
////			break;
////		case CStrategyResult::BUY:strName  = "매수진입";
////			break;
////		case CStrategyResult::EXITSHORT:strName  = "매도청산";
////			break;
////		case CStrategyResult::SELL:strName  = "매도진입";
////			break;
////		case CStrategyResult::EXITLONG: strName = "매수청산";
////			break;
////		}
////	}
////	else
////		strName = "NONE";
//	
//
////	strHtml.Replace("CurrentPos",strName);
////	
//////	strName = m_pOptimizer->GetStName();
//////	m_strStrategyName = strName;
//////	strHtml.Replace("StrategyName",strName);
//////	
//////	m_strItemName = m_pOptimizer->m_JMName;
//////	
//////
//////	stData = m_pInput->m_stDataIndex.GetAt(m_iDateIndex);
//////	
//////	strName.Format(" %s %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
//////	m_strPriod = strName;
//////	strHtml.Replace("Priod", strName);
////
////	//누적 수익률 저장 
//////	Data = Strate->GetStatisticalInfo(0);
////	strData.Format("%.2f%%", Data);
////	_tprintf("%s", (LPCTSTR) strData);
////	strHtml.Replace("AccumProf",strData);
////	///////////////////////
////////옵션에 따라 출력 : 8자리면 선물 6자리면 주식 옵션 //////////////////////////////////////////////////////////////			
////	for(int i = 1; i<MAXDATANUM;i++){
////	if(i==14) continue;//미청산은 포함 되지 않습니다.
//////	Data = Strate->GetStatisticalInfo(i);
////		//콤마 루틴 필요 Default!! 
////	if(i ==15||i==16||i==17||i==18||i==22||i==23||i==27||i==28||i==29||i==31||i==32)
////		strData.Format("%.0f", Data);
////	else
////			strData.Format("%.2f", Data);
////		
////		//주식 옵션일 경우 .... 소숫점 자리 안보일때...
////		//총손익, 총이익, 총손실, 건별최대이익, 건별최대손실,총진입, 미청산포함총청산, 
////		//최대연속이익, 최대연속손실, 
//////		if(m_bIsJusik)
//////	{
////			if(i==3||i==4||i== 5||i== 7||i== 8||i== 12||i== 13||i== 22||i== 26)
////				strData.Format("%.0f", Data);
////			else
////				strData.Format("%.2f", Data);
//////		}
////		int Index = strHtml.Find(m_pHtmlCodeNameArray[i],0);
////		if(strData.Find("-",0)>=0)
////			strHtml.Insert(Index-2, "Color=Red");
////		strHtml.Replace(m_pHtmlCodeNameArray[i],strData);
////	}
//	CString ddd;
//	return ddd;
//}

void CDlgResult::SetOptimizer(CDlgOptimizer* Opt)
{
	m_pOptimizer = Opt;
}

void CDlgResult::SetDataDir(CString DataDir)
{
	m_strDir = DataDir;
}

void CDlgResult::OpenWebView(CString FileName)
{
	//InitHtml();
	//CString strPath = m_strDir+FileName;
	//m_Browser.Navigate(strPath,NULL,NULL,NULL,NULL);
	return;
	
}

void CDlgResult::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}


