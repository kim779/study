// ChartTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wdrchartdgndlg.h"
#include "ChartTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartTypeDlg dialog


CChartTypeDlg::CChartTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChartTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartTypeDlg)
	DDX_Control(pDX, IDC_CT_TEXT, m_ctrlTxt);
	DDX_Control(pDX, IDC_CT_LIST, m_CTList);
	DDX_Control(pDX, IDC_CT_RECT, m_ImageViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CChartTypeDlg)
	ON_LBN_SELCHANGE(IDC_CT_LIST, OnSelchangeCtList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartTypeDlg message handlers

BOOL CChartTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetBasePath(m_szBaseDir);
	CString szCFG;

	// 기본적으로  제공되는 정보에 의한 설정.
	szCFG = m_szBaseDir + "\\ChartType.ini";
	_DoLoadList(szCFG);

	// 사용자가 추가한 정보에 의한 설정.
	szCFG = m_szBaseDir + "\\ChartTypeUser.ini";
	_DoLoadList(szCFG);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#include "../../ForSite/FS_Env.h"
void CChartTypeDlg::GetBasePath(CString& rString)
{
	GetDir_DrfnRoot(rString);
	rString+="\\gex\\ChartType";
}

int CChartTypeDlg::_DoLoadList(LPCSTR szCFG)
{
	int nAddCnt=0;	
	DWORD nSize = 9600;
	char *pReturnedStrng=(char*)malloc(nSize);	//Section MaxSize 9600.

	DWORD lReads = ::GetPrivateProfileSection("BaseType", pReturnedStrng, nSize, szCFG);
	if(lReads<0) return 0; // Read Error

	int nStart=0, nEnd=0;
	int nStrSize=0;
	char aLine[MAX_PATH];
	for(int i=0; i<(int)lReads; i++)
	{
		if(pReturnedStrng[i] == 0x00)
		{
			memcpy(aLine, &pReturnedStrng[nStart], i-nStart+1);	//Null포함.
			nStart = i+1;

			m_CTList.AddString(aLine);
			//theApp.m_DataUtilMng.GetWord(aKey, aLine, '=');

			nAddCnt++;
		}
	}
	
	free(pReturnedStrng);

	if(nAddCnt>0)
	{
		int nSel = 0;
		m_CTList.SetCurSel(nSel);
		DoShowImage(nSel);
	}

	return nAddCnt;
}

/*
void CChartTypeDlg::_DoLoadList(LPCSTR szCFG)
{
	int nCnt = ::GetPrivateProfileInt("BaseType", "Count", 0, szCFG);
	char aKey[5+1]={0,}
		, aSection[30+1]={0,}
		, aTitle[100+1]={0,};
		//aImage[100+1]={0,};
	for(int i=0; i<nCnt; i++)
	{
		sprintf(aKey, "%d", i);

		if(::GetPrivateProfileString("BaseType", aKey, "",aTitle,  30, szCFG)>0)
		{
			m_CTList.AddString(aTitle);
		}
		//::GetPrivateProfileString(aKey, "Image", aImage, "", 100, szCFG);
	}
	int nSel = 0;
	m_CTList.SetCurSel(nSel);
	DoShowImage(nSel);
}
*/


void CChartTypeDlg::OnSelchangeCtList() 
{
	int nSel = m_CTList.GetCurSel();
	DoShowImage(nSel);
}

void CChartTypeDlg::DoShowImage(int nSel)
{
	if(nSel<0) return;

	CString szCFG;
	szCFG = m_szBaseDir + "\\ChartType.ini";

	CString rString;
	m_CTList.GetText(nSel, rString);
	rString.TrimLeft(); rString.TrimRight();
	m_szSelectKey = rString;

	char aKey[5+1]={0,}
	, aSection[30+1]={0,}
		, aImage[100+1]={0,};

	//이미지 보이기
	if(::GetPrivateProfileString(rString, "Image",  "", aImage, 100, szCFG)>0)
	{
		CString szImage;
		szImage = m_szBaseDir + "\\" + aImage;
		m_ImageViewer.LoadImage(szImage);
		m_ImageViewer.Invalidate();
	}

	//텍스트보이기
	rString += "_TXT";
	{
		m_ctrlTxt.SetWindowText("");

		int nAddCnt=0;	
		DWORD nSize = 4096+1;
		char *pReturnedStrng=(char*)malloc(nSize);	//Section MaxSize 9600.

		DWORD lReads = ::GetPrivateProfileSection(rString, pReturnedStrng, nSize, szCFG);
		if(lReads<0) return ; // Read Error

		CString szTxt;
		int nStart=0, nEnd=0;
		int nStrSize=0;
		char aLine[MAX_PATH];
		for(int i=0; i<(int)lReads; i++)
		{
			if(pReturnedStrng[i] == 0x00)
			{
				memcpy(aLine, &pReturnedStrng[nStart], i-nStart+1);	//Null포함.
				nStart = i+1;
				
				//m_CTList.AddString(aLine);
				szTxt += aLine; 
				szTxt += "\r\n";
			}
		}
		m_ctrlTxt.SetWindowText(szTxt);
		
		free(pReturnedStrng);
	}
}
