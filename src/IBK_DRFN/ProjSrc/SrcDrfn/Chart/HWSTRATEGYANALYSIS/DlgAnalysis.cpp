// DlgAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAnalysis.h"
#include "DlgInterface.h"
#include "DlgWait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_TIME_RUNST 10223
#define CRBACKGROUND	RGB(177,199,234)

//#define WMU_GET_AUP_SIZE	WM_USER + 2243			// Aup의 기본 Size를 View에 요청한다.
const UINT RMSG_GETDLLSIZE = ::RegisterWindowMessage("RMSG_GETDLLSIZE");
const UINT RMSG_SETDLLSIZE = ::RegisterWindowMessage("RMSG_SETDLLSIZE");
const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");


/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis dialog


CDlgAnalysis::CDlgAnalysis(CWnd* pParent /*=NULL*/)
	: CEscDialog(CDlgAnalysis::IDD, pParent)
{
	EnableAutomation();
 
	//{{AFX_DATA_INIT(CDlgAnalysis)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStrategyItem = NULL;
	m_pdlgInfo = NULL;
	m_hSocket = NULL;
	m_pChartInterface = NULL;
	m_pCtlList = NULL;

	m_pChartItem = NULL;
	m_nStartType = 0;	// 0:성과분석, 1:적용실행

	m_pDlgWait = NULL;
	m_hTabChartEx = NULL;
	m_hRsc = AfxGetResourceHandle();
	m_bReceiveData = FALSE;
	m_crBk = CRBACKGROUND;
}


void CDlgAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAnalysis)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAnalysis, CDialog)
	//{{AFX_MSG_MAP(CDlgAnalysis)
	ON_WM_DESTROY()	
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_GETDLLSIZE,   OnGetDllSize)
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessagge)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()
//	ON_MESSAGE( WMU_GET_AUP_SIZE, OnGetAupSize )

/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis message handlers
void	CDlgAnalysis::FormInit()
{
}

void CDlgAnalysis::FormLoad()
{
}

//성과분석을 여기서 한다. 기존엔 InputLeft에서 했었는데 말이지..
void CDlgAnalysis::InitSTLoad() 
{

	if(m_stCode=="" || m_stSTName=="") 
	{
		AfxMessageBox("종목명이나 전략명이 선택되지 않았습니다.");
		return;
	}
	//m_stSTName.Delete(m_stSTName.GetLength()-5,m_stSTName.GetLength());
	int nEndPos = -1;
	if((nEndPos = m_stSTName.Find(gSTGubunKey)) != -1)
		m_stSTName = m_stSTName.Left(nEndPos);
	if((nEndPos = m_stSTName.Find('.')) != -1)
		m_stSTName = m_stSTName.Left(nEndPos);

	if(m_pdlgInfo)
	{
		if(m_stCode.GetLength()>6)
			m_pdlgInfo->InitCodeGubun(FALSE); // ¼±¹°¿E¼C 
		else
			m_pdlgInfo->InitCodeGubun(TRUE); // AO½A¿E¼C 
	}
	/////////////////////////////////
	m_stArrayPath.RemoveAll();

	////////////////////////////////
//@유진삭제
//#ifdef _DEBUG
//	CString strSystemPath = m_strRootPath +  "\\" + STSD_D;
//	CString strUserPath = m_strRootPath + "\\" + STUSER_D;
//#else
//	CString strSystemPath = m_strRootPath + "\\" + STSD;
//	CString strUserPath = m_strRootPath + "\\" + STUSER;
//#endif
//@유진삭제
//@유진추가
//@유진추가
//>> [QC No]090806HMC094
//	CString strSystemPath = m_strRootPath + "\\" + PATH_DATA_STDATA_SD;
//	CString strUserPath = m_strRootPath + "\\" + PATH_USER_STDATA_SD;
	CString strSystemPath = m_strDataDir + "\\STData\\SD";
	CString strUserPath = m_strUserDir + "\\STData\\SD";
//<< [QC No]090806HMC094
//@유진추가

	m_stArrayPath.Add(strSystemPath);//Data
	m_stArrayPath.Add(strUserPath);//User

	if(!m_pStrategyItem)
	{
		m_pStrategyItem = m_strategyLoader.GetStrategyItem();// 메모리 생성 
		if(m_pStrategyItem == NULL) return;
		int nRetValue = m_pStrategyItem->Create(this, (long)m_pStrategyItem,m_stArrayPath,m_pChartInterface);
		if(nRetValue==-1) 
		{
			m_pStrategyItem->DestoryWindow();
			delete m_pStrategyItem;
			m_pStrategyItem = NULL;
		}
	}

	//LoadForRun종목명(String), 데이터갯수(int),날짜(String)
	m_bReceiveData = FALSE;//데이터를 받지 않은 것으로 갱신
	BOOL bRetValue = m_pStrategyItem->LoadST(m_stSTPath,m_stCode,m_nCount,m_stEndDate); //
	if(bRetValue)
	{
		m_pStrategyItem->Run();
	}
		//초기화 하기 

	//m_pdlgInfo->InitWebView();
	//if(m_pdlgInfo && m_nStartType==0)
	//	m_pdlgInfo->ApplyST();

}

//일단 계산하는 루틴 : 데이터를 가져오는 루틴까지만 KHD 2006.07.27
LRESULT CDlgAnalysis::OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam)
{
	if(m_bReceiveData) return 0;//데이터를 받았으면 리턴..
	CString strDataItemName, szTData;
	CString strDiv, strRep;
	CStringArray strDate; //Date 정보를 저장
	ST_DATAINDEX Index;
	BOOL nIsDate = -1;
	int i = 0;
	m_stDataIndex.RemoveAll();

	if(wParam==(WPARAM)m_pStrategyItem||m_DivideState ==1||m_DivideState==2)
	{
		m_pStrategyItem = (CIStrategyItem*)wParam;	
		m_pStrategyItem->Stop();
		ST_DRAWINFO_EX* pInfo = (ST_DRAWINFO_EX*)lParam;
		CPtrArray* pPtrArrayPlot = pInfo->pPtrArraySDataItem;
		m_pDataItem = NULL;
		int nSize = pPtrArrayPlot->GetSize();

		if(nSize ==0) return 0;

		/////////////List 채우기 ///////////////////////////////////////
		m_pDataItem = (CISDataItem *)pPtrArrayPlot->GetAt(0);

		long nTotalData	= m_pDataItem->GetSize();
		//Data 요청 //////////////////////////
		/////////////////////
		strDate.SetSize( nTotalData );

		//전체기간 멀티차트 리스트에 넣기(KHD:2006.08.08)
		m_pDataItem->GetAt(0,&strDataItemName);//맨 처음 값이 처음
		InsertGubun(strDataItemName);
		Index.nStart = 0;
		Index.strStartDate = strDataItemName;
		Index.strGubun ="전체";
		
		m_pCtlList->DeleteAllItems();
		m_pCtlList->InsertItem(0,"*");		// 글자폭을 줄이기 위해서 수정함.

		nIsDate = strDataItemName.Find('/');
		if(nIsDate<0 ||m_DivideState==1)
		{
			m_pCtlList->SetItemText(0,1,strDataItemName);
		}
		else 
		{
			szTData = strDataItemName.Mid(2);
			m_pCtlList->SetItemText(0,1,szTData);
		}

		m_pDataItem->GetAt(nTotalData-1,&strDataItemName);//맨 마지막 값이끝 
		InsertGubun(strDataItemName);

		if(nIsDate<0||m_DivideState==1)
		{
			m_pCtlList->SetItemText(0,2,strDataItemName);
		}
		else
		{
			szTData = strDataItemName.Mid(2);
			m_pCtlList->SetItemText(0,2,szTData);
		}

		Index.nEnd = nTotalData-1;
		Index.strEndDate = strDataItemName;

		m_stDataIndex.Add(Index);//전체구간..넣기
		//if(m_nDivideCnt==1)
		//{
		//	m_stDataIndex.Add(Index);
		//}
	//배열에 날짜를 가지고 와서 저장한다. 구간을 나눠야 하는 저장장소 마련 
		for( i = 0;i<nTotalData;i++)
		{
			m_pDataItem->GetAt(i,&strDataItemName);
			strDate.SetAt(i,strDataItemName);
		}

	//	//중첩율 구하기 :  사전 작업
	//	//num - ((Div-1)*Repnum/Div)
		int Div = m_nDivideCnt; //분할 갯수 가져 오기.
		int num = strDate.GetSize();//전체 데이터갯수
		int Divnum =0;
		int Repnum;
		long double Repfloat;

		if(m_dfDivideRate > 0)
		{
			Repnum		= ((num/10) * (int)m_dfDivideRate/10);//atoi(strRep) *;//한 막대상의 중첩률
			int tot		= num - ((Div-1)*Repnum/Div);//전체상 길이
			Repfloat	= long double(num/long double(tot));//Repfloat * num-Repnum = 400
		}
		else
		{
			Repnum = 0;
			Repfloat =1;
		}

		//중첩율 넣어야지대~
		for(i =0; i<Div; i++)//구간까지..
		{
			//분할적용 하여 시작점 산정 : 0.5를 빼야 -1이 적용이 되어 시작점이 정확해진다.
			int a = int(((Divnum-(i*(Repnum/Div)))*Repfloat)-0.5);
			if(a>0)
			{
				strDataItemName = strDate.GetAt(a);
				Index.nStart = a;
			}
			else//첫 시작점에서는 0 자체에서 값을 가져와야 하므로
			{
				strDataItemName = strDate.GetAt(Divnum );
				Index.nStart = int(Divnum);
			}
			
			InsertGubun(strDataItemName);
			Index.strStartDate = strDataItemName;
			//Index.strGubun.Format("[%d구간]",i+1);
			Index.strGubun.Format("%d",i+1);		// 글자폭을 줄이기 위해서 수정함.
			//m_pCtlList->InsertItem(i+1,"");
			m_pCtlList->InsertItem(i+1,Index.strGubun);

			if(nIsDate<0||m_DivideState==1)
			{
				m_pCtlList->SetItemText(i+1,1,strDataItemName);
			}
			else
			{
				szTData = strDataItemName.Mid(2);
				m_pCtlList->SetItemText(i+1,1,szTData);
			}

			//배열은 시작점이 0이므로 첫 화면에서는 그대로 마지막엔 1개씩 빼서 해야한다.
			// 0~99 , 100~199, 200~299 : 실질적으로 숫자는 99개지만 배열상의 주소는 100개가 된다.
			if(i==0)
				Divnum+=(num/Div);
			else
				Divnum += ((num/Div)-1);//1구간씩 만든다. 분할한 만큼 더한다.
			
			if(Divnum > nTotalData-1)//만약 400개의 데이터 보다 많을 경우 마지막값을 세팅한다.
				Divnum = nTotalData-1;

	//		//계산 로직 
			int b = int(((Divnum - (i*(Repnum/Div)))*Repfloat)-0.5);//int((Divnum+(Repnum/Div))*Repfloat);

			if(b<num && i!=Div)// 데이터개수에 따른 예외 처리 : 최대갯수를 넘지 않기 위해..
			{
				strDataItemName = strDate.GetAt(b);
				Index.nEnd = b;
			}

			if(m_nDivideCnt==1 || i == Div-1)//마지막 값을 가져온다. 
			{
				strDataItemName = strDate.GetAt(num-1);
				Index.nEnd = num-1;

			}
			InsertGubun(strDataItemName);
			Index.strEndDate = strDataItemName;
			/////////정보를 저장한다. KHD : 2006/9/4/////////////
			m_pStrategyItem->CalculateStatisticalInfo(Index.nStart, Index.nEnd);
			// 승률 = 18 , 누적수익률 =1 , 총매매수 = 15, 총수익률 = 3
			
			CString stData1;
			stData1.Format("%.2f",m_pStrategyItem->GetStatisticalInfo(0));
			stData1 = CDlgInterface::GetComma(stData1);

			CString stData3;
			stData3.Format("%.0f",m_pStrategyItem->GetStatisticalInfo(15));
			stData3 = CDlgInterface::GetComma(stData3);
			CString stData4;
			stData4.Format("%.2f",m_pStrategyItem->GetStatisticalInfo(19));
			stData4 = CDlgInterface::GetComma(stData4);
			Index.strInfo1.Format("누적:%s ",stData1);
			Index.strInfo2.Format("승률:%s 매매수:%s",stData4,stData3);
			Index.STName = m_stSTName;
			/////////////////////////////////////////////////////
			m_stDataIndex.Add(Index);//중복 여기에 저장된 걸로 차트에서 쓴다.
			
			if(nIsDate<0||m_DivideState==1)
			{	
				m_pCtlList->SetItemText(i+1,2,strDataItemName);
			}
			else 
			{
				szTData = strDataItemName.Mid(2);
				m_pCtlList->SetItemText(i+1,2,szTData);
			}	
			Divnum++;
		}	
		if(m_pdlgInfo && m_nStartType!=1)
		{
			Result();
			//m_pdlgInfo->AllUpdateDlg(); // UpdateDlg
			//	////////////////////////////////////////////////////
			//m_pdlgInfo->m_CtlResult.EnableWindow(TRUE);
		}
	}


	m_pCtlList->Invalidate();
	if(m_pdlgInfo)
		m_pdlgInfo->DrawGraph();

	if(m_nStartType==1)
		PostMessage(WM_DESTROY, 0, 0);
	m_bReceiveData = TRUE;//데이터를 받았다..
	return 0;
}

//Log 찍는 루틴 !! KHD
void CDlgAnalysis::WriteDebug(CString szDebug)
{
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	//	FILE *fp = fopen("M:\\Logs\\GridDebug.txt", "a+t");
	FILE *fp = fopen("c:\\Logs\\성과분석.txt", "a+t");

	if(fp != NULL) 
	{
		fprintf(fp, "[%02d:%02d:%02d]", sTime.wHour+9, sTime.wMinute, sTime.wSecond);
		fprintf(fp, "[%s]\n", szDebug);
		fclose(fp);
	}

}

void CDlgAnalysis::InsertMonthGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
}

void CDlgAnalysis::InsertDateGubun(CString &str)
{
	if(str.GetLength()<4) return;
	str.Insert(4,'/');
	str.Insert(7,'/');
}

//현재 Tick의 경우 데이터 자체가 잘 못 나오고 있습니다. : KHD : 2006.09.29
void CDlgAnalysis::InsertTimeGubun(CString &str)
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

void CDlgAnalysis::InsertHourGubun(CString &str)
{
	int nSize = str.GetLength();
	if(nSize == 7)
	{
		str.Insert(0,'0');
		nSize++;
	}
	if(nSize<7) return;

	str.Insert(2,'/');
	str.Insert(5,'/');
	str.Insert(8,':');
}

//분할 상태,  틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
BOOL CDlgAnalysis::InsertGubun(CString &stTime,BOOL IsHtml)
{
	switch(m_DivideState)
	{
	case 0://틱
		InsertTimeGubun(stTime);	
		break;
// 	case 4://시
// 		InsertHourGubun(stTime);
// 		break;
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

void CDlgAnalysis::InsertMinuteGubun(CString &str, BOOL IsHtml)
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
//KHD : 분할 버튼 누를시 발생하는 이벤트!!
void CDlgAnalysis::SetCalculateData(int nIndexnum)
{
	ST_DATAINDEX Index;
	int nSize = m_stDataIndex.GetSize();
	if(nIndexnum>=nSize)
		return;

	Index = m_stDataIndex.GetAt(nIndexnum);
	//m_pdlgInfo->m_Trade->m_nDiv = nIndexnum;
	m_pStrategyItem->Stop();
	m_pStrategyItem->CalculateStatisticalInfo(Index.nStart, Index.nEnd);

}

void CDlgAnalysis::Result()
{ 
	if(m_pdlgInfo)
	{
		int CurSel = 0;//m_pdlgInfo->m_Trade->m_nDiv;
		SetCalculateData(CurSel);
		m_pdlgInfo->SetRefresh(CurSel);
	}
}
void CDlgAnalysis::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	m_brBkColor.DeleteObject();
	
	if(m_pdlgInfo) 
	{
		m_pdlgInfo->DestroyWindow();
		delete m_pdlgInfo;
		m_pdlgInfo = NULL;
	}

	if(m_pStrategyItem)
	{
		m_pStrategyItem->DestoryWindow();
		delete m_pStrategyItem;
		m_pStrategyItem = NULL;
	}

	if(m_pDlgWait)
	{
		delete m_pDlgWait;
		m_pDlgWait = NULL;
	}
	
//	if(m_pCtlList)
//		m_pCtlList->DeleteAllItems();
}

BOOL CDlgAnalysis::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AfxEnableControlContainer();

	// TODO: Add extra initialization here
	CRect rt(0,0,0,0);
	m_pdlgInfo = new CInfoRight;
	if(m_pdlgInfo)
	{
		m_pdlgInfo->Create(CInfoRight::IDD,this);
		m_pdlgInfo->ShowWindow(SW_SHOW);
		m_pdlgInfo->m_pDlgAnalysis = this;
		m_pdlgInfo->SetParentsRect(rt);
		m_pdlgInfo->m_strFilePath = m_strDataDir + "\\STDATA";
		if(m_nStartType==0)
			m_pdlgInfo->InitTab();
		m_pdlgInfo->m_pChartItem = m_pChartItem;
	}

	//성과분석 실행 :KHD 2006.07.27
	if(m_nStartType==1)
	{
		InitSTLoad();
		return TRUE;
	}

	if(!m_pDlgWait)
	{
		m_pDlgWait = new CDlgWait;
		if(!m_pDlgWait->Create(CDlgWait::IDD, this))
		{
			delete m_pDlgWait;
			m_pDlgWait = NULL;
		}
		else
		{
			m_pDlgWait->CenterWindow(&m_pdlgInfo->m_CtlTab);
			m_pDlgWait->ShowWindow(SW_SHOW);
		}
	}

	m_crBk = DEFAULT_BKGRND_COLOR;
	m_brBkColor.CreateSolidBrush(m_crBk);

	SetTimer(ID_TIME_RUNST, 1, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



CRect CDlgAnalysis::GetRect(int nID)
{

	CRect rect;	
	switch(nID)
	{
	case IDD_DLG_ANALYSIS:
		{
			GetClientRect(rect);
		}
		break;
	}
	return rect;
}

// 060721_001 JLO 성과분석을 다이얼로그 형태로 바로 띄우는 테스트하기위해 추가.
void CDlgAnalysis::OnCancel()
{
	CDialog::OnCancel();
}
// ~060721_001 JLO


//화면로딩시 화면크기를 알기위해 호출되므로 화면크기를 넘겨줌.
//LONG CDlgAnalysis::OnGetAupSize(WPARAM wParam, LPARAM lParam)
// 런타임시에 DLL의 사이즈를 변경하고 싶을 경우에는 RMSG_SETDLLSIZE 메시지를 보낸다. 부모(다이얼로그프레임)에게.
LONG CDlgAnalysis::OnGetDllSize(WPARAM wParam, LPARAM lParam)
{

	CPoint* pSize = (CPoint*)wParam;

	pSize->x = 764;
	pSize->y = 482;

	return 0L;
}

void CDlgAnalysis::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CDlgAnalysis::OnTimer(UINT nIDEvent)
{
	if(nIDEvent == ID_TIME_RUNST)
	{
		KillTimer(ID_TIME_RUNST);

		InitSTLoad();
		if(m_pDlgWait) 
		{
			delete m_pDlgWait;
			m_pDlgWait = NULL;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CDlgAnalysis::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
	}
	
	return hbr;
}