// TabResult.cpp : implementation file
//

#include "stdafx.h"

#include "TabResult.h"
#include "Resource.h"
#include "DlgAnalysis.h"
#include ".\tabresult.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabResult dialog

CTabResult::CTabResult(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabResult)
	//}}AFX_DATA_INIT
	m_pInput = NULL;
//	m_pWebView = NULL;
	m_iDateIndex =0 ;
	m_pFont = NULL;
}


void CTabResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabResult)
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_EXPLORER, m_Browser);
}


BEGIN_MESSAGE_MAP(CTabResult, CDialog)
	//{{AFX_MSG_MAP(CTabResult)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabResult message handlers

BOOL CTabResult::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brBackground.CreateSolidBrush( CLR_BACKGROUND);
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CTabResult::InitPasingData()
{
//	m_nViewPointIndex = {1,3,};
	
}
void CTabResult::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}
void CTabResult::OpenWebView(CString FileName)
{
	CString strPath = m_strFilePath+FileName;
	COleVariant varEmpty;
	COleVariant varURL(m_strFilePath + FileName);
//	m_Browser.Navigate2(varURL,varEmpty,varEmpty,varEmpty,varEmpty);
	return;
}

//Html 파일의 String을 로드하여 파싱하여 데이타를 채우고 Html파일을 생성 한다. 
void CTabResult::MakeHtmlFile()
{
	//성과 분석 파일 로드!!
	CStdioFile NewFile;
///String 파싱루틴 //////////////////////..
	CString strHtml;
	InitHtml();
	strHtml = m_strHtml;
	strHtml = ParsingstrHtml(strHtml);
//////NewFile에 쓰기 1. 성과 분석,전진분석,매매내역루틴 필요//////////////////////////

	SetFileAttributes (m_strFilePath+"\\성과분석.htm", FILE_ATTRIBUTE_NORMAL);//protect Read Only Error 
	if(!NewFile.Open(m_strFilePath+"\\성과분석.htm",CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}else
		NewFile.WriteString(strHtml);
	
	NewFile.Close();
	OpenWebView("\\성과분석.htm");
}
CString CTabResult::ParsingstrHtml(CString strHtml)
{
	//try
	//{
	//	//속도 향상을 위해 미리 파싱할 변수 배열에 저장 //
	//	double Data;
	//	CString strData;
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
	//	CString strName;
	//	CStrategyResult* Result;
	//	ST_DATAINDEX stData;
	//	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	//
	//	int nSize = Strate->GetCntStrategyResult();
	//	if(nSize > 0)
	//	{
	//		Result = Strate->GetStrategyResult(nSize-1);
	//		switch(Result->m_DealType) {
	//		case CStrategyResult::NONE:strName = "NONE";
	//			break;
	//		case CStrategyResult::BUY:strName  = "매수진입";
	//			break;
	//		case CStrategyResult::EXITSHORT:strName  = "매도청산";
	//			break;
	//		case CStrategyResult::SELL:strName  = "매도진입";
	//			break;
	//		case CStrategyResult::EXITLONG: strName = "매수청산";
	//			break;
	//		}
	//	}
	//	else
	//		strName = "NONE";
	//	
	//	m_strCurrentPos = strName;
	//	strHtml.Replace("CurrentPos",strName);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlstPosition.SetText(m_strCurrentPos);

	//	strName = m_pDlgAnalysis->m_stSTName;
	//	m_strStrategyName = strName;
	//	strHtml.Replace("StrategyName",strName);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlSTFILENAME.SetText(m_strStrategyName);

	//	m_strItemName = m_pDlgAnalysis->m_stCodeName;
	//	strHtml.Replace("ItemName", m_strItemName);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlstName.SetText(m_strItemName);
	//	
	//	if(m_iDateIndex < m_pDlgAnalysis->m_stDataIndex.GetCount())
	//	{
	//		stData = m_pDlgAnalysis->m_stDataIndex.GetAt(m_iDateIndex);

	//		strName.Format(" %s %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
	//		m_strPriod = strName;
	//		strHtml.Replace("Priod", strName);
	//		m_pDlgAnalysis->m_pdlgInfo->m_ctlstPeriod.SetText(m_strPriod);
	//	}

	//	//누적 수익률 저장 
	//	Data = Strate->GetStatisticalInfo(0);
	//	strData.Format("%.2f", Data);
	//	m_strACCumProf = strData;
	//	strHtml.Replace("AccumProf",strData);
	//	m_pDlgAnalysis->m_pdlgInfo->m_ctlstNujuk.SetText("dd");//m_strACCumProf);

	//	///////////////////////
	//////옵션에 따라 출력 : 8자리면 선물 6자리면 주식 옵션 //////////////////////////////////////////////////////////////			
	//	for(int i = 1; i<MAXDATANUM;i++)
	//	{
	//		if(i==14) continue;//미청산은 포함 되지 않습니다.
	//		if(i==24)// 전략초과수익률:누적수익률 - 기간등락률 적용!
	//		{
	//			Data =atof(m_strACCumProf)-Strate->GetStatisticalInfo(21);
	//		}
	//		else
	//			Data = Strate->GetStatisticalInfo(i);
	//		//콤마 루틴 필요 Default!! 
	//		if(i==2||i==6||i==18||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
	//			strData.Format("%.2f", Data);
	//		else
	//		{
	//			strData.Format("%d", (INT)Data);
	//		}
	//		//주식 옵션일 경우 .... 소숫점 자리 안보일때...
	//		//총손익, 총이익, 총손실, 건별최대이익, 건별최대손실,총진입, 미청산포함총청산, 
	//		//최대연속이익, 최대연속손실, 
	//		if(m_bIsJusik)
	//		{
	//			if(i==2||i==6||i==18||i==19||i==20||i==21||i==24||i==30||i==33||i==34||i==35)
	//				strData.Format("%.2f", Data);
	//			else
	//				strData.Format("%d", (INT)Data);
	//		}
	//		strData = GetComma(strData);//Comma
	//		int Index = strHtml.Find(m_pHtmlCodeNameArray[i],0);
	//		//if(strData.Find("-",0)>=0)일단 막자..
	//		//	strHtml.Insert(Index-2, "Color=Red");
	//		strHtml.Replace(m_pHtmlCodeNameArray[i],strData);
	//	}
	//	return strHtml;
	//}
	//catch(...)
	//{
	//	return "";
	//}
return "";
}

void CTabResult::InitGrid()
{

	if(!m_GridResult.GetSafeHwnd())
	{
		CRect rect1;
		GetClientRect(rect1);
		rect1.top =1;
		rect1.left +=1;
		rect1.right-=10;
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle(m_pDlgAnalysis->m_hRsc);
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
		m_GridResult.SetBkColor(CLR_BACKGROUND);
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
		AfxSetResourceHandle(hInst);
	}
}

void CTabResult::SetDataSpreadGrid()
{
	CString strName;
	CStrategyResult* Result;
	ST_DATAINDEX stData;
	double Data;
	CString strData;
	DWORD nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//그리드 포맷

	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	int nSize = Strate->GetCntStrategyResult();
	if(nSize > 0)
	{
		Result = Strate->GetStrategyResult(nSize-1);
		switch(Result->m_DealType) {
			case CStrategyResult::NONE:strName = "NONE";
				break;
			case CStrategyResult::BUY:strName  = "매수진입";
				break;
			case CStrategyResult::EXITSHORT:strName  = "매도청산";
				break;
			case CStrategyResult::SELL:strName  = "매도진입";
				break;
			case CStrategyResult::EXITLONG: strName = "매수청산";
				break;
		}
	}
	else
		strName = "NONE";

	m_strCurrentPos = strName;
	m_pDlgAnalysis->m_pdlgInfo->m_ctlstPosition.SetText(m_strCurrentPos);

	strName = m_pDlgAnalysis->m_stSTName;
	m_strStrategyName = strName;
	m_pDlgAnalysis->m_pdlgInfo->m_ctlSTFILENAME.SetText(m_strStrategyName);

	m_strItemName = m_pDlgAnalysis->m_stCodeName;
	m_pDlgAnalysis->m_pdlgInfo->m_ctlstName.SetText(m_strItemName);

	if(m_iDateIndex < m_pDlgAnalysis->m_stDataIndex.GetSize())
	{
		stData = m_pDlgAnalysis->m_stDataIndex.GetAt(m_iDateIndex);
		strName.Format(" [%s] %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
		m_strPriod = strName;
		m_pDlgAnalysis->m_pdlgInfo->m_ctlstPeriod.SetText(m_strPriod);
	}

	//누적 수익률 저장 
	Data = Strate->GetStatisticalInfo(0);
	strData.Format(" %.2f", Data);
	m_strACCumProf = strData;
	m_GridResult.SetItemText(0,1,m_strACCumProf+"%",nFormat);//Grid
	m_pDlgAnalysis->m_pdlgInfo->m_ctlstNujuk.SetText(m_strACCumProf+"%");
	
	///////////////////////
	////옵션에 따라 출력 : 8자리면 선물 6자리면 주식 옵션 //////////////////////////////////////////////////////////////	
	
	for(int i = 1; i<MAXDATANUM;i++)
	{
		if(i==14) continue;//미청산은 포함 되지 않습니다.
		if(i==24)// 전략초과수익률:누적수익률 - 기간등락률 적용!
		{
			Data =atof(m_strACCumProf)-Strate->GetStatisticalInfo(21);
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


void CTabResult::InitHtml()
{
	if(m_strHtml!="")return;
	CStdioFile DefaultFile;
	CString str;
	
	if(!DefaultFile.Open(m_strFilePath+"\\보고서-성과분석.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create 보고서-성과분석.htm File.");
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

void CTabResult::InsertData2HtmlString(short nIndex, CString& strHtml, CIStrategyItem* pStrategy)
{
	if(nIndex < 0)
	{
		switch(nIndex)
		{
		case -1:	// CurrentPos
			{
				int nSize = pStrategy->GetCntStrategyResult();
				if(nSize > 0)
				{
					CStrategyResult* Result = pStrategy->GetStrategyResult(nSize-1);
					switch(Result->m_DealType) 
					{
					case CStrategyResult::NONE:
						m_strCurrentPos = "NONE";
						break;
					case CStrategyResult::BUY:
						m_strCurrentPos  = "매수진입";
						break;
					case CStrategyResult::EXITSHORT:
						m_strCurrentPos  = "매도청산";
						break;
					case CStrategyResult::SELL:
						m_strCurrentPos  = "매도진입";
						break;
					case CStrategyResult::EXITLONG: 
						m_strCurrentPos = "매수청산";
						break;
					}
				}
				else
					m_strCurrentPos = "NONE";

				strHtml += m_strCurrentPos;
			}
			break;
		case -2:	// StrategyName
			{
				m_strStrategyName = m_pDlgAnalysis->m_stSTName;
				strHtml += m_strStrategyName;
			}
			break;
		case -3:	// ItemName
			{
				m_strItemName = m_pDlgAnalysis->m_stCodeName;
				strHtml += m_strItemName;
			}
			break;
		case -4:	// Priod
			{
				if(m_iDateIndex < m_pDlgAnalysis->m_stDataIndex.GetSize())
				{
					ST_DATAINDEX stData = m_pDlgAnalysis->m_stDataIndex.GetAt(m_iDateIndex);

					m_strPriod.Format(" %s %s  ~ %s", stData.strGubun, stData.strStartDate,stData.strEndDate);
					strHtml += m_strPriod;
				}
			}
			break;
		case -5:	// AccumProf
			{
				m_strACCumProf.Format("%.2f%%", pStrategy->GetStatisticalInfo(0));
				strHtml += m_strACCumProf;
			}
		}
	}
	else if(nIndex > 0)
	{
		//		if(nIndex == 14)	// 미청산은 포함 되지 않습니다.
		//			return;

		CString strData;
		double dData = pStrategy->GetStatisticalInfo(nIndex);

		//주식 옵션일 경우 .... 소숫점 자리 안보일때...
		//총손익, 총이익, 총손실, 건별최대이익, 건별최대손실,총진입, 미청산포함총청산, 
		//최대연속이익, 최대연속손실, 
		if(m_bIsJusik && nIndex==1||nIndex==2||nIndex==6||nIndex==9||nIndex==10||nIndex==11||nIndex==19||nIndex==20||nIndex==21||nIndex==30)
			strData.Format("%.2f", dData);
		else
			strData.Format("%d", (INT)dData);

		strHtml += GetComma(strData);//Comma
	}
}

BOOL CTabResult::ParsingstrHtml2(CString& strHtml)
{
	CFile DefaultFile;
	if(!DefaultFile.Open(m_strFilePath+"\\보고서-성과분석2.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create 보고서-성과분석2.htm File.");
		return FALSE;
	}

	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();

	char chRead[2] = {NULL, };
	char szIndex[3] = {NULL, };
	while(DefaultFile.Read(chRead, 1))
	{
		if(*chRead == _T('K'))
		{
			if(!DefaultFile.Read(chRead, 1))
				break;

			if(*chRead == _T('H'))
			{
				if(!DefaultFile.Read(chRead, 1))
					break;

				if(*chRead == _T('D'))
				{
					if(!DefaultFile.Read(szIndex, 2))
						break;

					// 값 입력위치
					InsertData2HtmlString(atoi(szIndex), strHtml, Strate);						
				}
			}
		}
		else
			strHtml += chRead;

	}

	strHtml.Replace("color=\"white\"", "");//
	DefaultFile.Close();

	return TRUE;
}
HBRUSH CTabResult::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDlgInterface::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	pDC->SetBkColor( RGB(255,255,255));//CLR_BACKGROUND;
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return m_brBackground;
}
