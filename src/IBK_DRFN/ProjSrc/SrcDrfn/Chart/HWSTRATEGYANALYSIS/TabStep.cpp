// TabStep.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TabStep.h"
//#include "InputLeft.h"
#include "DlgAnalysis.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabStep dialog


CTabStep::CTabStep(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabStep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabStep)
	//}}AFX_DATA_INIT

	m_nDiv = 0;
	m_pFont = NULL;
}


void CTabStep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabStep)
	//}}AFX_DATA_MAP
//	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
}


BEGIN_MESSAGE_MAP(CTabStep, CDialog)
	//{{AFX_MSG_MAP(CTabStep)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabStep message handlers

BOOL CTabStep::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabStep::InitGrid()
{
	if(!m_StepGridCtrl.GetSafeHwnd())
	{
		CRect rect1;
		GetClientRect(rect1);
		rect1.top =1;
		rect1.left +=1;
		rect1.right-=10;
//		rect1.bottom-=5;
		HINSTANCE hInst = AfxGetResourceHandle();
		AfxSetResourceHandle(m_pDlgAnalysis->m_hRsc);
		if(!m_StepGridCtrl.Create(rect1,this,IDC_STATIC_RESULT,WS_CHILD | WS_TABSTOP | WS_VISIBLE ))
		{
			AfxMessageBox("No");
			return;
		}
		m_StepGridCtrl.SetEditable(FALSE);
		m_StepGridCtrl.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(255, 255, 255));
		m_StepGridCtrl.SetGridLineColor(RGB(204,210,224));
		m_StepGridCtrl.SetColumnCount(12);
		m_StepGridCtrl.SetRowCount(16);
		m_StepGridCtrl.SetBkColor(CLR_BACKGROUND);		
		m_StepGridCtrl.SetRowHeight(0,23);
		if(m_pFont)
			m_StepGridCtrl.SetFont(m_pFont);

		char* IndexName[] = {"구분","전체","1","2","3","4","5","6","7","8","9","10"};
		char* IndexName2[] = {"","누적수익률","건별평균수익률","총손익","총이익","총손실","총이익/총손실","최대이익","최대손실","총진입","미청산포함총청산","총매매수","이익수/손실수","기간등락률","전략초과수익률","기간참여율"};
		for(int col = 0; col< 12;col++)
		{		
			m_StepGridCtrl.SetItemBkColour(0,col,RGB(218,222,237));
			m_StepGridCtrl.SetItemText(0,col,IndexName[col]);
			if(col==0)
			{
				m_StepGridCtrl.SetColumnWidth(0, 95);
				for(int row =1; row < 16 ; row++)
				{
					m_StepGridCtrl.SetRowHeight(row,23);
					m_StepGridCtrl.SetItemBkColour(row,col,RGB(242,248,254));
					m_StepGridCtrl.SetItemText(row,col,IndexName2[row]);
				}
			}
			else
				m_StepGridCtrl.SetColumnWidth(col, (rect1.right/11)-9);
		}
	}
}

void CTabStep::SetDataSpreadGrid()
{
	double Data;
	CString strData;
	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();
	DWORD nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;//그리드 포맷
	//루틴 바꿔야 한다..!!!!
	int nIndex = 0;
	for(int i = 0; i<= m_nDiv ;i++)//Index만큼 돈다.
	{		
		m_pDlgAnalysis->SetCalculateData(i);
		nIndex = i +1;
		for(int j =0;j<31;j++)
		{
			if(j==2||j==9||j==10||j==11||j==14||j==16||j==17||j==19||j==20||j==22||j==23||( j>24 && j<30 ))
				continue;//미청산은 포함 되지 않습니다.

			Data = Strate->GetStatisticalInfo(j);
			if(j==15||j==31)
				strData.Format("%.0f", Data);
			else
				strData.Format("%.2f", Data);

			if(m_bIsJusik)
			{
				if(j==3||j==4||j== 5||j== 7||j== 8||j== 12||j== 13)
					strData.Format("%.0f", Data);
				else
					strData.Format("%.2f", Data);
			}

			if(Data == 0)
				strData.Format("0",Data);
			strData = GetComma(strData);
			
			switch(j) 
			{
			case PROFITRATEACCMULATED:
				m_StepGridCtrl.SetItemText(1,nIndex,strData+"%",nFormat);//Grid
				break;
			case PROFITRATEAVG:
				m_StepGridCtrl.SetItemText(2,nIndex,strData+"%",nFormat);//Grid
				break;
			case PROFITLOSSALL:
				m_StepGridCtrl.SetItemText(3,nIndex,strData,nFormat);//Grid
				break;
			case PROFITALL:
				m_StepGridCtrl.SetItemText(4,nIndex,strData,nFormat);//Grid
				break;
			case LOSSALL:
				m_StepGridCtrl.SetItemText(5,nIndex,strData,nFormat);//Grid
				break;
			case PROFITLOSSRATEALL:
				m_StepGridCtrl.SetItemText(6,nIndex,strData,nFormat);//Grid
				break;
			case MAXPROFIT:
				m_StepGridCtrl.SetItemText(7,nIndex,strData,nFormat);//Gridi
				break;
			case MAXLOSS:
				m_StepGridCtrl.SetItemText(8,nIndex,strData,nFormat);//Grid
				break;
			case AMOUNTENTEREDALL:
				m_StepGridCtrl.SetItemText(9,nIndex,strData,nFormat);//Grid
				break;
			case AMOUNTEXITALL:
				m_StepGridCtrl.SetItemText(10,nIndex,strData,nFormat);//Grid
				break;
			case CNTALL:
				m_StepGridCtrl.SetItemText(11,nIndex,strData,nFormat);//Grid
				break;
			case RATEPROFITLOSS:
				m_StepGridCtrl.SetItemText(12,nIndex,strData,nFormat);//Grid
				break;
			case CHANGERATE:
				m_StepGridCtrl.SetItemText(13,nIndex,strData+"%",nFormat);//Grid
				break;
			case RATEPROFITEXCEED:///전략 초과 수익률 : 누적수익률(0) - 기간등락률(21)
				m_StepGridCtrl.SetItemText(14,nIndex,strData+"%",nFormat);//Grid
				break;
			case RATEACCESS:
				m_StepGridCtrl.SetItemText(15,nIndex,strData+"%",nFormat);//Grid
				break;
			}
		}
	}
}
void CTabStep::OpenWebView(CString FileName)
{ 
}

//File Pasing!!
void CTabStep::MakeHtmlFile()
{
}

CString CTabStep::ParsingstrHtml(CString strHtml)
{

	double Data;
	CString strData;
	CString m_pHtmlCodeNameArray[STEPMAXDATANUM] =
	{"APR0","AAP0","TLP0","TPF0","TLR0","TTT0","MPF0","MLS0","TER0","TCI0","TTN0","PLN0","PSR0","SOR0","PPR0",
	 "APR1","AAP1","TLP1","TPF1","TLR1","TTT1","MPF1","MLS1","TER1","TCI1","TTN1","PLN1","PSR1","SOR1","PPR1",
	 "APR2","AAP2","TLP2","TPF2","TLR2","TTT2","MPF2","MLS2","TER2","TCI2","TTN2","PLN2","PSR2","SOR2","PPR2",
	 "APR3","AAP3","TLP3","TPF3","TLR3","TTT3","MPF3","MLS3","TER3","TCI3","TTN3","PLN3","PSR3","SOR3","PPR3",
	 "APR4","AAP4","TLP4","TPF4","TLR4","TTT4","MPF4","MLS4","TER4","TCI4","TTN4","PLN4","PSR4","SOR4","PPR4",
	 "APR5","AAP5","TLP5","TPF5","TLR5","TTT5","MPF5","MLS5","TER5","TCI5","TTN5","PLN5","PSR5","SOR5","PPR5",
	 "APR6","AAP6","TLP6","TPF6","TLR6","TTT6","MPF6","MLS6","TER6","TCI6","TTN6","PLN6","PSR6","SOR6","PPR6",
	 "APR7","AAP7","TLP7","TPF7","TLR7","TTT7","MPF7","MLS7","TER7","TCI7","TTN7","PLN7","PSR7","SOR7","PPR7",
	 "APR8","AAP8","TLP8","TPF8","TLR8","TTT8","MPF8","MLS8","TER8","TCI8","TTN8","PLN8","PSR8","SOR8","PPR8",
	 "APR9","AAP9","TLP9","TPF9","TLR9","TTT9","MPF9","MLS9","TER9","TCI9","TTN9","PLN9","PSR9","SOR9","PPR9",
	 "APRTen","AAPTen","TLPTen","TPFTen","TLRTen","TTTTen","MPFTen","MLSTen","TERTen","TCITen","TTNTen","PLNTen","PSRTen","SORTen","PPRTen"
	};

//	CInputLeft* Input = GetCInputLeft();
	CIStrategyItem* Strate = m_pDlgAnalysis->GetStrategyItem();

	//루틴 바꿔야 한다..!!!!
	CStringArray strDataArray;
	int i = 0;
	for( i = 0; i<= m_nDiv ;i++)//Index만큼 돈다.
	{		
		m_pDlgAnalysis->SetCalculateData(i);
		for(int j =0;j<31;j++)
		{
			if(j==2||j==9||j==10||j==11||j==14||j==16||j==17) 
				continue;//미청산은 포함 되지 않습니다.
			if(j==19||j==20||j==22||j==23||( j>24 && j<30 ))
				continue; 
			Data = Strate->GetStatisticalInfo(j);

			if(j==15||j==31)
				strData.Format("%.0f", Data);
			else
				strData.Format("%.2f", Data);

			if(m_bIsJusik)
			{
			if(j==3||j==4||j== 5||j== 7||j== 8||j== 12||j== 13)
				strData.Format("%.0f", Data);
			else
				strData.Format("%.2f", Data);
			}
			
			if(Data == 0)
				strData.Format("0",Data);
			strData = GetComma(strData);
			strDataArray.Add(strData);		
		}
	}
	
	for(i =0;i< strDataArray.GetSize();i++)
	{
		CString str,Code;
		int s = strDataArray.GetSize();
		str = strDataArray.GetAt(i);
		Code = m_pHtmlCodeNameArray[i];
		int Index = strHtml.Find(Code,0);
		if(str.Find("-",0)>=0)
		{
			//strHtml.Delete(Index-15, 13);
			strHtml.Insert(Index-2, " Color=BLUE");
		}
		strHtml.Replace(m_pHtmlCodeNameArray[i],strDataArray.GetAt(i));
	}
	for(i = strDataArray.GetSize(); i<STEPMAXDATANUM;i++)
		strHtml.Replace(m_pHtmlCodeNameArray[i], "  ");
	
	return strHtml;
}

void CTabStep::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_StepGridCtrl.GetSafeHwnd())
		m_StepGridCtrl.DestroyWindow();
}
void CTabStep::InitHtml()
{
	if(m_strHtml!="")return;
	CStdioFile DefaultFile;
	CString str,HtmlData;
	
	if(!DefaultFile.Open(m_strFilePath+"\\보고서-전진분석.htm",CFile::modeRead))
	{
		AfxMessageBox("Do Not Create Html File.");
		return;
	}

	for(;;){
		if(DefaultFile.ReadString(str))
			m_strHtml+=str;
		else
			break;
	}	
	m_strHtml.Replace("color=\"white\"", "");
	DefaultFile.Close();
}