// DlgTrade.cpp : implementation file
//

#include "stdafx.h"

#include "DlgTrade.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTrade dialog

CDlgTrade::CDlgTrade(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CDlgTrade::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTrade)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStrategyItem = NULL;
	m_pFont = NULL;
}


void CDlgTrade::DoDataExchange(CDataExchange* pDX)
{
	CDlgInterface::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTrade)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTrade, CDlgInterface)
	//{{AFX_MSG_MAP(CDlgTrade)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTrade message handlers

BOOL CDlgTrade::OnInitDialog() 
{
	CDlgInterface::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgTrade::InitGrid()
{
	if(!m_TradeGridCtrl.GetSafeHwnd())
	{
		CRect rect1;
		GetClientRect(rect1);
		rect1.top =1;
		rect1.left +=2;
		rect1.bottom-=1;

		if(!m_TradeGridCtrl.Create(rect1,this,IDC_STATIC_RESULT,WS_CHILD | WS_TABSTOP | WS_VISIBLE ))
		{
			AfxMessageBox("No");
			return;
		}
		m_TradeGridCtrl.SetEditable(FALSE);
		m_TradeGridCtrl.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
		m_TradeGridCtrl.SetGridLineColor(RGB(204,210,224));
		m_TradeGridCtrl.SetColumnCount(10);
		m_TradeGridCtrl.SetRowCount(1);
		m_TradeGridCtrl.SetBkColor(CLR_BACKGROUND );
		m_TradeGridCtrl.ShowScrollBar(SB_VERT );
		if(m_pFont)
			m_TradeGridCtrl.SetFont(m_pFont);

		char* GridIndexName[] = {"NO","포지션","매매시점","가격","수량","진입/청산금액","건별손익","누적손익","건별수익률","누적수익률"};
		m_TradeGridCtrl.SetColumnWidth(0,30);
		m_TradeGridCtrl.SetColumnWidth(1,70);
		m_TradeGridCtrl.SetColumnWidth(2,80);
		m_TradeGridCtrl.SetColumnWidth(3,85);
		m_TradeGridCtrl.SetColumnWidth(4,40);
		m_TradeGridCtrl.SetColumnWidth(5,100);
		m_TradeGridCtrl.SetColumnWidth(6,80);
		m_TradeGridCtrl.SetColumnWidth(7,70);
		m_TradeGridCtrl.SetColumnWidth(8,70);
		m_TradeGridCtrl.SetColumnWidth(9,70);

		for(int col =0 ; col <10; col++)
		{
			m_TradeGridCtrl.SetItemBkColour(0,col,RGB(242,248,254));
			m_TradeGridCtrl.SetItemText(0,col,GridIndexName[col]);
		}
		//m_TradeGridCtrl.ExpandColumnsToFit();
	}
}


void CDlgTrade::SetDataSpreadGrid()
{
	CString strResultData;
	CStringArray ResultData;
	CStrategyResult* Result;
	CStringArray stNujukArray;//누적수익률
	CArray<double,double> dGunSu;
	CArray<double,double> dSonIk;//누적손익률
	BOOL bNujuk = FALSE;

	CIStrategyItem* Strate = m_pDlgOptimizer->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	DWORD nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//그리드 포맷
	m_TradeGridCtrl.SetRowCount(1);

	char* szParam1 = "%.2f";
	if(m_pDlgOptimizer->m_nCodeType == 0)	szParam1 = "%2.f";

	for(int i =0 ;i < nSize; i++)
	{	
		Result = Strate->GetStrategyResult(i);
		if(Result==NULL) break;//예외 처리 :KHD 

		//먼저 파싱 해서 데이터를 초기화 해서 넣는다.
		ResultData.RemoveAll();
		strResultData.Format("%d",i+1);
		ResultData.Add(strResultData);//NO

		switch(Result->m_DealType) {
		case CStrategyResult::NONE:strResultData = "NONE"; bNujuk = FALSE;
			break;
		case CStrategyResult::BUY:strResultData  = "매수진입";bNujuk = FALSE;
			break;
		case CStrategyResult::EXITSHORT:strResultData  = "매도청산";bNujuk = FALSE;
			break;
		case CStrategyResult::SELL:strResultData  = "매도진입";bNujuk = TRUE;
			break;
		case CStrategyResult::EXITLONG: strResultData = "매수청산";bNujuk = TRUE;
			break;
		}

		ResultData.Add(strResultData);//Position
		//Time
		strResultData.Format("%2.f",Result->m_dTime);
		InsertGubun(strResultData);
		ResultData.Add(strResultData);//매매시점


		//strResultData.Format("%2.f",Result->m_dPrice);
		strResultData.Format(szParam1, Result->m_dPrice);
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
		ResultData.Add(strResultData);//가격

		strResultData.Format("%ld",Result->m_lQty);
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
		ResultData.Add(strResultData);//수량

		//strResultData.Format("%2.f",Result->m_dAmount);
		strResultData.Format(szParam1, Result->m_dAmount);
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
		ResultData.Add(strResultData);//진입금액

		//strResultData.Format("%2.f",Result->m_dProfit);
		strResultData.Format(szParam1, Result->m_dProfit);
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
		ResultData.Add(strResultData);//건별손익

		//strResultData.Format("%2.f",Result->m_dProfitAccmulated);
		strResultData.Format(szParam1,Result->m_dProfitAccmulated);
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);		
		ResultData.Add(strResultData);//누적손익


		strResultData.Format("%.2f%%",Result->m_dProfitRate);
		if(bNujuk)//건별수익률
			dGunSu.Add(Result->m_dProfitRate);
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
		ResultData.Add(strResultData);//건별수익률

		if(bNujuk)
			dSonIk.Add(atof(strResultData));

		strResultData.Format("%.2f%%",Result->m_dProfitRateAccmulated);//누적수익률
		strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);

		if(bNujuk)
			stNujukArray.Add(strResultData);

		ResultData.Add(strResultData);

		m_TradeGridCtrl.InsertRow("");
		for(int col = 0 ; col < ResultData.GetSize(); col++)
		{
			strResultData = ResultData.GetAt(col);
			m_TradeGridCtrl.SetItemText(m_TradeGridCtrl.GetRowCount()-1, col, strResultData,nFormat);
			if(strResultData== "매수청산"||strResultData == "매도청산")
			{
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col,RGB(0,0,255));
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col+1,RGB(0,0,255));
			}
			else if(strResultData == "매수진입"||strResultData == "매도진입" )
			{
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col,RGB(255,0,0));
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col+1,RGB(255,0,0));
			}

			if(strResultData.Find('-')>=0)
				m_TradeGridCtrl.SetItemFgColour(m_TradeGridCtrl.GetRowCount()-1,col,RGB(0,0,255));
		}
	}

	if(m_pDlgOptimizer && m_pDlgOptimizer->m_DlgSooik->GetSafeHwnd()!=NULL)
	{
		m_pDlgOptimizer->m_DlgSooik->PrepareShow(stNujukArray.GetSize());
		m_pDlgOptimizer->m_DlgSooik->ShowData2Chart(stNujukArray,stNujukArray.GetSize());
		m_pDlgOptimizer->m_DlgSooik->ShowData1Chart((long)&dGunSu);
		m_pDlgOptimizer->m_DlgSonik->ShowData1Chart((long)&dSonIk);//누적손익률 : 손익챠트 데이터
	}
}
void CDlgTrade::SetStrategyItem(CIStrategyItem* pStrat)
{
	m_pStrategyItem = pStrat;
}
void CDlgTrade::SetOptimizer(CDlgOptimizer* Opt)
{
	m_pDlgOptimizer = Opt;
}

 
void CDlgTrade::SelectRunData(CString pRunData)
{
	if(m_pStrategyItem == NULL) return;
	
	m_pDlgOptimizer->SetSTMessageType(2);
	m_pStrategyItem->Run(pRunData);
}

//여기서 뿌려야할 데이터를 받아야 한다.
void CDlgTrade::GetSTMessageData()
{
	CString TableCode;
	CString str2,strHtml;
	CString strResultData;
	CStringArray ResultData;
	CStrategyResult* Result;
	CString str;

	if(m_pDlgOptimizer==NULL)
	{
		AfxMessageBox("[PROJ : IndexOptimizer ] CDlgTrade::GetSTMessageData() 루틴확인");
		return;
	}

	m_pStrategyItem = m_pDlgOptimizer->GetStrategyItem();
	strHtml = m_strHtml;
	CString m_pHtmlCodeNameArray[10] =
	{"Num","Position","Date","Price","Amount","Entry","ProfitAccum","NuAccum","GunRate","NuJukRate"};

	str= _T("<tr Align=\"Right\"><td width=\"39\" height=\"18\" ><font face=\"굴림\" color=\"black\"><span style=\"font-size:10pt;\">Num</span></font></td>\n")
      _T("<td width=\"71\" height=\"18\"><font face=\"굴림\" color=\"FC1\"><span style=\"font-size:9pt;\">Position</span></font></td>\n")
      _T("<td width=\"70\" height=\"18\"><font face=\"굴림\" color=\"FC1\"><span style=\"font-size:9pt;\">Date</span></font></td>\n")
      _T("<td width=\"70\" height=\"18\"><font face=\"굴림\" color=\"black\"><span style=\"font-size:9pt;\">Price</span></font></td>\n")
      _T("<td width=\"35\" height=\"18\"><font face=\"굴림\" color=\"black\"><span style=\"font-size:9pt;\">Amount</span></font></td>\n")
      _T("<td width=\"73\" height=\"18\"><font face=\"굴림\" color=\"black\"><span style=\"font-size:9pt;\">Entry</span></font></td>\n")
      _T("<td width=\"73\" height=\"18\"><font face=\"굴림\" color=\"FC6\"><span style=\"font-size:9pt;\">ProfitAccum</span></font></td>\n")
      _T("<td width=\"78\" height=\"18\"><font face=\"굴림\" color=\"FC7\"><span style=\"font-size:9pt;\">NuAccum</span></font></td>\n")
      _T("<td width=\"73\" height=\"18\"><font face=\"굴림\" color=\"FC8\"><span style=\"font-size:9pt;\">GunRate</span></font></td>\n")
      _T("<td width=\"78\" height=\"18\"><font face=\"굴림\" color=\"FC9\"><span style=\"font-size:9pt;\">NuJukRate</span></font></td>\n")
	  _T("</tr>\n");
	
		int nSize =m_pStrategyItem->GetCntStrategyResult();
		CStringArray stNujukArray;//누적수익률
		CArray<double,double> dGunSu;
		CArray<double,double> dSonIk;//누적손익률 
		BOOL bNujuk = FALSE;
		for(int i =0 ;i < nSize; i++)
		{	
			Result = m_pStrategyItem->GetStrategyResult(i);
			str2 = str;

			//먼저 파싱 해서 데이터를 초기화 해서 넣는다.
			ResultData.RemoveAll();
			strResultData.Format("%d",i+1);
			ResultData.Add(strResultData);
			switch(Result->m_DealType) {
			case CStrategyResult::NONE:strResultData = "NONE";bNujuk = FALSE;
				break;
			case CStrategyResult::BUY:strResultData  = "매수진입";bNujuk = FALSE;
				break;
			case CStrategyResult::EXITSHORT:strResultData  = "매도청산";bNujuk = FALSE;
				break;
			case CStrategyResult::SELL:strResultData  = "매도진입";bNujuk = TRUE;
				break;
			case CStrategyResult::EXITLONG: strResultData = "매수청산";bNujuk = TRUE;
				break;
			}
			
			ResultData.Add(strResultData);
			//Time
			strResultData.Format("%2.f",Result->m_dTime);
 			InsertGubun(strResultData);
			ResultData.Add(strResultData);
			
			//if(m_bIsJusik)//주식이면
			{
				strResultData.Format("%2.f",Result->m_dPrice);
				strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
				ResultData.Add(strResultData);

				strResultData.Format("%ld",Result->m_lQty);
				strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
				ResultData.Add(strResultData);

				strResultData.Format("%2.f",Result->m_dAmount);
				strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
				ResultData.Add(strResultData);

				strResultData.Format("%2.f",Result->m_dProfit);
				strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
				ResultData.Add(strResultData);

				////////////////////////////////////////////
				strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
				ResultData.Add(strResultData);
			}
			//else //선물
			{
			/*	strResultData.Format("%.2f",Result->m_dPrice);
				ResultData.Add(strResultData);
				strResultData.Format("%ld",Result->m_lQty);
				ResultData.Add(strResultData);
				strResultData.Format("%.2f",Result->m_dAmount);
				ResultData.Add(strResultData);
				strResultData.Format("%.2f",Result->m_dProfit);
				ResultData.Add(strResultData);
				strResultData.Format("%.2f",Result->m_dProfitAccmulated);
				ResultData.Add(strResultData);*/
			}
	//	
			strResultData.Format("%.2f%%",Result->m_dProfitRate);
			if(bNujuk)//건별수익률
				dGunSu.Add(Result->m_dProfitRate);

			strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
			ResultData.Add(strResultData);

			//건별손익률을 보내준다. 건별수익률은 int형태로 처리한다.
			// strResultData.Format("%2.f",Result->m_dProfitRate);
			double m_dProfitRate = 0;			// 건별수익률
			if(Result->m_dProfitRate > 0)
			{
				strResultData.Format("%.f",Result->m_dProfitRate +0.5);
			}
				
			else if(Result->m_dProfitRate < 0)
			{
				strResultData.Format("%.f",Result->m_dProfitRate -0.5);
			}
			
			if(bNujuk) {
				dSonIk.Add(atof(strResultData));
			}

			/*
			strResultData.Format("%.f",Result->m_dProfitRate);
			if(bNujuk) {
				dSonIk.Add(atof(strResultData));
				//dSonIk.Add(Result->m_dProfitAccmulated);
			}*/

			strResultData.Format("%.2f%%",Result->m_dProfitRateAccmulated);//누적수익률
			if(bNujuk)
				stNujukArray.Add(strResultData);
			strResultData = m_pDlgOptimizer->m_DlgResult->GetComma(strResultData);
			ResultData.Add(strResultData);
			
			for(int j =0;j < 10;j++)
			{
				if(j==1)
				{
					if(ResultData.GetAt(j) == "매수진입"||ResultData.GetAt(j) == "매도진입" )
					{
						for(int i = 7; i<10;i++)
							ResultData.SetAt(i," ");	
					}

					if(ResultData.GetAt(j) == "매도진입"||ResultData.GetAt(j) == "매수진입" )
						str2.Replace("\"FC1\"","\"blue\"");
					else if(ResultData.GetAt(j) == "매도청산"||ResultData.GetAt(j) == "매수청산" )
						str2.Replace("\"FC1\"","\"red\"");
			
				}
				if(j > 5)
				{
					CString Data;
					CString Index;
					Index.Format("\"FC%d\"",j);
					Data =ResultData.GetAt(j);
					if(Data.Find("-",0)==-1)
						str2.Replace(Index,"\"black\"");//발견되지 않음 
					else
						str2.Replace(Index,"\"blue\"");
				}

				str2.Replace(m_pHtmlCodeNameArray[j], ResultData.GetAt(j));
			}
		
			TableCode +=str2;
		}
	strHtml.Replace("<!--ADD-->",TableCode);

	if(m_pDlgOptimizer && m_pDlgOptimizer->m_DlgSooik->GetSafeHwnd()!=NULL)
	{
		m_pDlgOptimizer->m_DlgSooik->PrepareShow(stNujukArray.GetSize());
		m_pDlgOptimizer->m_DlgSooik->ShowData2Chart(stNujukArray,stNujukArray.GetSize());
		m_pDlgOptimizer->m_DlgSooik->ShowData1Chart((long)&dGunSu);
		m_pDlgOptimizer->m_DlgSonik->ShowData1Chart((long)&dSonIk);//누적손익률 : 손익챠트 데이터
	}
	stNujukArray.RemoveAll();
	dGunSu.RemoveAll();
	dSonIk.RemoveAll();
	////////////////////////////////
	CStdioFile NewFile;
	SetFileAttributes (m_strDir+"\\IOTradeData.htm", FILE_ATTRIBUTE_NORMAL);//protect Read Only Error
	if(!NewFile.Open(m_strDir+"\\IOTradeData.htm",CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}else
		NewFile.WriteString(strHtml);

		
	NewFile.Close();
	OpenWebView("\\IOTradeData.htm");

}
void CDlgTrade::OpenWebView(CString FileName)
{

}


void CDlgTrade::InitHtml()
{
	if(m_strHtml!="")return;
	CStdioFile DefaultFile;
	CString str;
	
	if(!DefaultFile.Open(m_strDir+"\\IOTrade.htm",CFile::modeRead))
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
	m_strHtml.Replace("white", "Black");
	DefaultFile.Close();
}

void CDlgTrade::SetDataDir(CString DataDir)
{
	m_strDir = DataDir;
}

void CDlgTrade::InsertMonthGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
}

void CDlgTrade::InsertDateGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
	str.Insert(7,'/');
}


void CDlgTrade::InsertTimeGubun(CString &str)
{
	int nSize = str.GetLength();
	if(nSize == 7)
	{
		str.Insert(0,'0');
		nSize++;
	}
	if(nSize<7) return;

	str.Insert(2,'/');
	str.Insert(5,':');
	str.Insert(8,':');
}

//분할 상태,  틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
BOOL CDlgTrade::InsertGubun(CString &stTime,BOOL IsHtml)
{
	switch(m_pDlgOptimizer->m_nTimeState)
	{
	case 0://틱
		InsertTimeGubun(stTime);	
		break;
	case 1://분
		InsertMinuteGubun(stTime,IsHtml);
		break;
	case 2://일
	case 3://주
		InsertDateGubun(stTime);
		break;
	case 4://월
		InsertMonthGubun(stTime);
		break;
	}
	return TRUE;
}

void CDlgTrade::InsertMinuteGubun(CString &str, BOOL IsHtml)
{
	int nSize = str.GetLength();
	if(nSize == 7)
	{
		str.Insert(0,'0');
		nSize++;
	}
	if(nSize<7) return;
	if(IsHtml)//When HTML File
	{
		str.Insert(2,'/');
		str.Insert(5,"&nbsp;");
		str.Insert(nSize+5,":");
	}
	else
	{
		str.Insert(2,'/');
		str.Insert(5,'/');
		str.Insert(8,':');
	}
}



void CDlgTrade::OnDestroy() 
{
	CDlgInterface::OnDestroy();
	
	// TODO: Add your message handler code here
}
