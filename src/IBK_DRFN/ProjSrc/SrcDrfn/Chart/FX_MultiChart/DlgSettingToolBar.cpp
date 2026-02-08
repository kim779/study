// DlgSettingToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "DlgSettingToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHART_TOOL	3
/////////////////////////////////////////////////////////////////////////////
// CDlgSettingToolBar dialog


CDlgSettingToolBar::CDlgSettingToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingToolBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSettingToolBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bApply = FALSE;
	m_nDataVer = 20061013;
}


void CDlgSettingToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSettingToolBar)
	DDX_Control(pDX, IDC_LIST_SHOW, m_listShow);
	DDX_Control(pDX, IDC_LIST_AVAILABLE, m_listAvailable);
	DDX_Control(pDX, IDC_CMB_SHOW, m_cmbShow);
	DDX_Control(pDX, IDC_CMB_AVAILABLE, m_cmbAvailable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSettingToolBar, CDialog)
	//{{AFX_MSG_MAP(CDlgSettingToolBar)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_CBN_SELCHANGE(IDC_CMB_AVAILABLE, OnSelchangeCmbAvailable)
	ON_CBN_SELCHANGE(IDC_CMB_SHOW, OnSelchangeCmbShow)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_INIT, OnBtnInit)
	ON_BN_CLICKED(IDC_BTN_MOVEDOWN, OnBtnMovedown)
	ON_BN_CLICKED(IDC_BTN_MOVEUP, OnBtnMoveup)
	ON_BN_CLICKED(IDC_BTN_REMOVE, OnBtnRemove)
	ON_BN_CLICKED(IDC_BTN_REMOVEALL, OnBtnRemoveall)
	ON_BN_CLICKED(IDC_BTN_ADDALL, OnBtnAddall)
	ON_LBN_DBLCLK(IDC_LIST_AVAILABLE, OnDblclkListAdd)
	ON_LBN_DBLCLK(IDC_LIST_SHOW, OnDblclkListRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingToolBar message handlers

BOOL CDlgSettingToolBar::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//<<20100308_JS.Kim 솔로몬
	//AfxGetIGateManagerFX(m_pGMng);
	AfxGetIGateManager(m_pGMng);
	//>>
	ReadIniFile();		// File Read

	m_cmbAvailable.SetCurSel(0);
	m_cmbShow.SetCurSel(0);	

	FillAvailableList(m_arrayAnalIdx, m_mapAnalList);
	FillShowList(m_arrayUserIdx, m_mapUserList);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingToolBar::OnOK() 
{
	OnApply();
	
	CDialog::OnOK();
}

void CDlgSettingToolBar::OnCancel() 
{	
	CDialog::OnCancel();
}

void CDlgSettingToolBar::OnApply() 
{
	m_bApply = FALSE;
	GetDlgItem(IDC_APPLY)->EnableWindow(m_bApply);

	int nID = 21;
	IMainInfoManager* pMng = (IMainInfoManager*)m_pGMng->GetInterface(nID);
	
	CFileFind FileFind;
	CString strPath, strConfigPath;
	strPath = pMng->GetEncUserDir();
	strConfigPath.Format("%s\\%s", strPath, "FX_ChartToolBarSetting.dat");

	CString strVersion;
	strVersion.Format("%d", m_nDataVer);
	::WritePrivateProfileString("TOTAL", "VERSION", strVersion, strConfigPath);
	
	//***********************************************************
	// Anal List Item User file Write
	//***********************************************************
	char szKey[30];
	CString str, strValue, strFieldName;
	int nCnt = m_arrayAnalIdx.GetSize();
	int nFieldKey;
	
	sprintf(szKey,"%s","ANAL LIST");

	strValue.Format("%d",nCnt);
	::WritePrivateProfileString(szKey, "COUNT", strValue, strConfigPath);
	
	for (int i=0; i < nCnt ; i++)
	{ //CListBox
		str.Format("%02d", i);

		nFieldKey = m_arrayAnalIdx.GetAt(i);
		m_mapAnalList.Lookup(nFieldKey, strFieldName);
		strValue.Format("%d,%s", nFieldKey, strFieldName);
		::WritePrivateProfileString(szKey, str, strValue, strConfigPath);
	}

	//***********************************************************
	// Addition List Item User file Write
	//***********************************************************
	nCnt = m_arrayAdditionIdx.GetSize();
	
	sprintf(szKey,"%s","ADDITION LIST");

	strValue.Format("%d",nCnt);
	::WritePrivateProfileString(szKey, "COUNT", strValue, strConfigPath);
	for (i=0; i < nCnt ; i++)
	{ //CListBox
		nFieldKey = m_arrayAdditionIdx.GetAt(i);
		m_mapAdditionList.Lookup(nFieldKey, strFieldName);
		strValue.Format("%d,%s", nFieldKey, strFieldName);

		str.Format("%02d", i);
		::WritePrivateProfileString(szKey, str, strValue, strConfigPath);
	}

	//***********************************************************
	// User List Item User file Write
	//***********************************************************
	nCnt = m_arrayUserIdx.GetSize();
	
	sprintf(szKey,"%s","USER LIST");

	strValue.Format("%d",nCnt);
	::WritePrivateProfileString(szKey, "COUNT", strValue, strConfigPath);
	for (i=0; i < nCnt ; i++)
	{ //CListBox
		nFieldKey = m_arrayUserIdx.GetAt(i);
		m_mapUserList.Lookup(nFieldKey, strFieldName);
		strValue.Format("%d,%s", nFieldKey, strFieldName);

		str.Format("%02d", i);
		::WritePrivateProfileString(szKey, str, strValue, strConfigPath);
	}	
}

//-----------------------------------------------------------------------------
// Author		:	Hui-Eun, Lee	Date :2006.09.11
// Modifier		: 
// Comments		:	UD$$$$$.cfg 파일 Read
//					[CONFIG]
//					VERSION=20060911
//					[ANAL LIST]			//nCulSel: 1, 분석도구
//					COUNT=0
//					Col_num=FieldKey,FieldName		
//					[ADDITION LIST]		//nCulSel: 2, 부가기능
//					COUNT=0
//					Col_num=FieldKey,FieldName
//					[USER LIST]			//nCulSel: 0, 사용자설정
//					COUNT=0
//					Col_num=FieldKey,FieldName
										//nCulSel: 3, 전체
//{{ user폴더에 .cfg파일이 없을 때 설정화면을 안띄우고 바로 닫았을경우 Available List가 저장되지않음 수정 }}
//-----------------------------------------------------------------------------
void CDlgSettingToolBar::ReadIniFile(int nCulSel/*=3*/)
{

	int nID = 21;
	IMainInfoManager* pMng = (IMainInfoManager*)m_pGMng->GetInterface(nID);
	
	CFileFind FileFind;
	CString strPath, strDataPath, strUserPath, strConfigPath, strTemp;
	char szKey[30];

	//# User Folder
	strPath = pMng->GetEncUserDir();
	strUserPath.Format("%s\\%s", strPath, "FX_ChartToolBarSetting.dat");
	//# Data Folder
	strPath = pMng->GetDataDir();
	strDataPath.Format("%s\\%s", strPath, "ChartToolBarSetting.dat");	

	//# Version확인
	int nUserCnt, nDataCnt, nUserVer, nDataVer;
	sprintf(szKey,"%s","TOTAL");
	m_nDataVer = ::GetPrivateProfileInt(szKey, "VERSION", -1, strDataPath);

	if (!FileFind.FindFile(strUserPath) || nCulSel != 3 )		//# Data 폴더
	{
		strTemp = strConfigPath = strDataPath;		
	}
	else										//# User 폴더
	{
		//# Count
		nUserCnt	= ::GetPrivateProfileInt(szKey, "COUNT", -1, strDataPath);
		nDataCnt	= ::GetPrivateProfileInt(szKey, "COUNT", -1, strUserPath);
		//# Version
		nUserVer = ::GetPrivateProfileInt(szKey, "VERSION", -1, strDataPath);	
		nDataVer = ::GetPrivateProfileInt(szKey, "VERSION", -1, strUserPath);

		if(nUserVer != nDataVer)
			strConfigPath = strDataPath;
		else // 버전일치
			strConfigPath = strUserPath;
	}

	CString szIndex, szColNum, szColInfo, szFieldKey, szFieldName;
	int nColCount, nFieldKey;;

	//************************************************
	// 분석도구 리스트
	//************************************************
	if(1 == nCulSel || 3 == nCulSel)
	{
		sprintf(szKey,"%s","ANAL LIST");
		nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);

		for(int i=0;i < nColCount; i++)
		{
			szColNum.Format("%02d", i);
			::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
			szColInfo.ReleaseBuffer();
		
			if (szColInfo.GetLength() == 0)
				continue;

			GetFieldData(szColInfo, nFieldKey, szFieldName);

			m_arrayAnalIdx.Add(nFieldKey);
			m_mapAnalList.SetAt(nFieldKey, szFieldName);
		}
	}
	
		
	//************************************************
	// 부가기능 리스트
	//************************************************
	if(2 == nCulSel || 3 == nCulSel)
	{
		sprintf(szKey,"%s","ADDITION LIST");
		nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);

		for(int i=0;i < nColCount; i++)
		{
			szColNum.Format("%02d", i);
			::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
			szColInfo.ReleaseBuffer();
		
			if (szColInfo.GetLength() == 0)
				continue;

			GetFieldData(szColInfo, nFieldKey, szFieldName);

			m_arrayAdditionIdx.Add(nFieldKey);
			m_mapAdditionList.SetAt(nFieldKey, szFieldName);
		}
	}
	
	//************************************************
	// 사용자설정 리스트
	//************************************************
	if(0 == nCulSel || 3 == nCulSel)
	{
		sprintf(szKey,"%s","USER LIST");
		nColCount = ::GetPrivateProfileInt(szKey, "COUNT", -1, strConfigPath);

		for(int i=0;i < nColCount; i++)
		{
			szColNum.Format("%02d", i);
			::GetPrivateProfileString(szKey, szColNum, "", szColInfo.GetBuffer(MAX_PATH), MAX_PATH, strConfigPath);	
			szColInfo.ReleaseBuffer();
		
			if (szColInfo.GetLength() == 0)
				continue;

			GetFieldData(szColInfo, nFieldKey, szFieldName);

			m_arrayUserIdx.Add(nFieldKey);
			m_mapUserList.SetAt(nFieldKey, szFieldName);
		}
	}
	
	return ;
}

//-----------------------------------------------------------------------------
// Author		: Hui-Eun, Lee	Date :2006.9.11
// Modifier		: 
// Parameter	:	strFieldData	: 파일데이터
//					nFieldKey		: 리스트인덱스
//					strFieldName	: 리스트항목
// Comments		:  파일데이터를 읽어서 parsing
//-----------------------------------------------------------------------------
void CDlgSettingToolBar::GetFieldData(CString strFieldData, int &nFieldKey, CString &strFieldName)
{
	CString strFieldKey;

	int nPos = strFieldData.Find(',');
	if(nPos==-1)	return;		
	strFieldKey = strFieldData.Left(nPos);			//필드키
	nFieldKey = atoi(strFieldKey);
	strFieldData.Delete(0,nPos+1);		

	strFieldName = strFieldData;			//필드명
}

void CDlgSettingToolBar::OnSelchangeCmbAvailable() 
{
	int nCul = m_cmbAvailable.GetCurSel();
	switch(nCul)
	{
	case 0:		// 분석도구
		FillAvailableList(m_arrayAnalIdx, m_mapAnalList);
		break;
	case 1:		// 부가기능
		FillAvailableList(m_arrayAdditionIdx, m_mapAdditionList);
		break;	
	default:
		break;
	}
}

void CDlgSettingToolBar::OnSelchangeCmbShow() 
{
	int nCul = m_cmbShow.GetCurSel();
	switch(nCul)
	{
	case 0:		// 사용자설정
		GetDlgItem(IDC_CMB_AVAILABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_AVAILABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_REMOVEALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADDALL)->EnableWindow(TRUE);

		FillShowList(m_arrayUserIdx, m_mapUserList);
		break;
	case 1:		// 분석도구
		GetDlgItem(IDC_CMB_AVAILABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_AVAILABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REMOVEALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADDALL)->EnableWindow(FALSE);

		FillShowList(m_arrayAnalIdx, m_mapAnalList);
		break;
	case 2:		// 부가기능
		GetDlgItem(IDC_CMB_AVAILABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_AVAILABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REMOVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_REMOVEALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADDALL)->EnableWindow(FALSE);

		FillShowList(m_arrayAdditionIdx, m_mapAdditionList);
		break;	
	default:
		break;
	}
}

void CDlgSettingToolBar::FillShowList(CArray<int, int> &arrayList, CMap<int, int, CString, CString> &mapList)
{
	int nIdx;
	CString strFieldName;

	m_listShow.ResetContent();
	for(int i=0; i< arrayList.GetSize(); i++)
	{
		nIdx = arrayList.GetAt(i);
		mapList.Lookup(nIdx,strFieldName);
		m_listShow.AddString(strFieldName);
	}

	if(!m_bApply)
	{
		m_bApply = TRUE;
		GetDlgItem(IDC_APPLY)->EnableWindow(m_bApply);
	}
}

void CDlgSettingToolBar::FillAvailableList(CArray<int, int> &arrayList, CMap<int, int, CString, CString> &mapList)
{
	ASSERT( arrayList.GetSize() > 0);
	ASSERT( mapList.GetCount() > 0 );

	int nIdx;
	CString strFieldName;

	m_listAvailable.ResetContent();
	for(int i=0; i< arrayList.GetSize(); i++)
	{
		nIdx = arrayList.GetAt(i);
		mapList.Lookup(nIdx,strFieldName);
		m_listAvailable.AddString(strFieldName);
	}
}

void CDlgSettingToolBar::OnBtnAdd() 
{

	int nKey;
	CString strFieldName;
	int nCmbCurSel = m_cmbAvailable.GetCurSel();
	int nCurSel = m_listAvailable.GetCurSel();
	if(nCurSel < 0) return;
	if(0 ==nCmbCurSel)		// Anal
	{
		nKey = m_arrayAnalIdx.GetAt(nCurSel);
		m_mapAnalList.Lookup(nKey, strFieldName);
	}
	else if(1 == nCmbCurSel)	// Addition
	{
		nKey = m_arrayAdditionIdx.GetAt(nCurSel);
		m_mapAdditionList.Lookup(nKey, strFieldName);
	}	

	if(!m_mapUserList.Lookup(nKey, strFieldName))
	{
		m_arrayUserIdx.Add(nKey);
		m_mapUserList.SetAt(nKey, strFieldName);

		FillShowList(m_arrayUserIdx, m_mapUserList);
	}	
	else
		AfxMessageBox("해당 항목은 이미 목록에 존재합니다.");

}

void CDlgSettingToolBar::OnBtnRemove() 
{
	int nCurSel = m_listShow.GetCurSel();
	if(nCurSel < 0) return;
	int nKey = m_arrayUserIdx.GetAt(nCurSel);

	if(3 == nKey)		// 차트툴편집
	{
		AfxMessageBox("해당 항목은 삭제할 수 없습니다.");
		return;
	}
	m_arrayUserIdx.RemoveAt(nCurSel);
	m_mapUserList.RemoveKey(nKey);
	
	FillShowList(m_arrayUserIdx, m_mapUserList);
	if(nCurSel < 1)
		nCurSel = 0;
	else
		nCurSel--;
	m_listShow.SetCurSel(nCurSel);
}

void CDlgSettingToolBar::OnBtnRemoveall() 
{
	m_arrayUserIdx.RemoveAll();
	m_mapUserList.RemoveAll();

	m_arrayUserIdx.Add(CHART_TOOL);	//차트툴편집
	m_mapUserList.SetAt(CHART_TOOL, "차트툴편집");

	FillShowList(m_arrayUserIdx, m_mapUserList);
}

void CDlgSettingToolBar::OnBtnInit() 
{
	int nCurSel = m_cmbShow.GetCurSel();

	switch(nCurSel)
	{
	case 0:		// 사용자설정
		m_arrayUserIdx.RemoveAll();
		m_mapUserList.RemoveAll();
		ReadIniFile(nCurSel);
		FillShowList(m_arrayUserIdx, m_mapUserList);
		break;
	case 1:		// 분석도구
		m_arrayAnalIdx.RemoveAll();
		m_mapAnalList.RemoveAll();
		ReadIniFile(nCurSel);
		FillShowList(m_arrayAnalIdx, m_mapAnalList);
		break;
	case 2:		//부가기능
		m_arrayAdditionIdx.RemoveAll();
		m_mapAdditionList.RemoveAll();
		ReadIniFile(nCurSel);
		FillShowList(m_arrayAdditionIdx, m_mapAdditionList);
		break;
	default:
		break;
	}
}

void CDlgSettingToolBar::OnBtnMovedown() 
{
	int nNextKey;

	int nCurSel = m_cmbShow.GetCurSel();
	int nSelected = m_listShow.GetCurSel();
	if( nSelected < 0 || 
	    nSelected >= m_listShow.GetCount()-1)	
	   return;

	if(0 == nCurSel && 0 == nSelected)
	{
		AfxMessageBox("더이상 이동할 수 없습니다.");
		m_listShow.SetCurSel(nSelected+1);
		return;
	}

	switch(nCurSel)
	{
	case 0:		// 사용자설정
		nNextKey = m_arrayUserIdx.GetAt(nSelected+1);
		m_arrayUserIdx.RemoveAt(nSelected+1);
		m_arrayUserIdx.InsertAt(nSelected, nNextKey);
		FillShowList(m_arrayUserIdx, m_mapUserList);
		m_listShow.SetCurSel(nSelected+1);
		break;
	case 1:		// 분석도구
		nNextKey = m_arrayAnalIdx.GetAt(nSelected+1);
		m_arrayAnalIdx.RemoveAt(nSelected+1);
		m_arrayAnalIdx.InsertAt(nSelected, nNextKey);
		FillShowList(m_arrayAnalIdx, m_mapAnalList);
		m_listShow.SetCurSel(nSelected+1);
		break;
	case 2:		//부가기능
		nNextKey = m_arrayAdditionIdx.GetAt(nSelected+1);
		m_arrayAdditionIdx.RemoveAt(nSelected+1);
		m_arrayAdditionIdx.InsertAt(nSelected, nNextKey);
		FillShowList(m_arrayAdditionIdx, m_mapAdditionList);
		m_listShow.SetCurSel(nSelected+1);
		break;
	default:
		break;
	}
	
}

void CDlgSettingToolBar::OnBtnMoveup() 
{
	int nSelectedKey;

	int nCurSel = m_cmbShow.GetCurSel();
	int nSelected = m_listShow.GetCurSel();
	if(nSelected < 1) return;
	if(0 == nCurSel && 1 == nSelected) 
	{
		AfxMessageBox("더이상 이동할 수 없습니다.");
		m_listShow.SetCurSel(nSelected);
		return;
	}

	switch(nCurSel)
	{
	case 0:		// 사용자설정
		nSelectedKey = m_arrayUserIdx.GetAt(nSelected);
		m_arrayUserIdx.RemoveAt(nSelected);
		m_arrayUserIdx.InsertAt(nSelected-1, nSelectedKey);
		FillShowList(m_arrayUserIdx, m_mapUserList);
		m_listShow.SetCurSel(nSelected-1);
		break;
	case 1:		// 분석도구
		nSelectedKey = m_arrayAnalIdx.GetAt(nSelected);
		m_arrayAnalIdx.RemoveAt(nSelected);
		m_arrayAnalIdx.InsertAt(nSelected-1, nSelectedKey);
		FillShowList(m_arrayAnalIdx, m_mapAnalList);
		m_listShow.SetCurSel(nSelected-1);
		break;
	case 2:		//부가기능
		nSelectedKey = m_arrayAdditionIdx.GetAt(nSelected);
		m_arrayAdditionIdx.RemoveAt(nSelected);
		m_arrayAdditionIdx.InsertAt(nSelected-1, nSelectedKey);
		FillShowList(m_arrayAdditionIdx, m_mapAdditionList);
		m_listShow.SetCurSel(nSelected-1);
		break;
	default:
		break;
	}	
}

void CDlgSettingToolBar::OnBtnAddall() 
{
	int nKey;
	CString strFieldName;

	//User Data 삭제
	m_arrayUserIdx.RemoveAll();
	m_mapUserList.RemoveAll();

	//User Data 기본항목 추가
	m_arrayUserIdx.Add(CHART_TOOL);	//차트툴편집
	m_mapUserList.SetAt(CHART_TOOL, "차트툴편집");

	int nCmbCurSel = m_cmbAvailable.GetCurSel();
	if(0 ==nCmbCurSel)		// Anal
	{
		for (int nIdx=0; nIdx<m_arrayAnalIdx.GetSize(); nIdx++)
		{
			nKey = m_arrayAnalIdx.GetAt(nIdx);
			m_mapAnalList.Lookup(nKey, strFieldName);

			m_arrayUserIdx.Add(nKey);
			m_mapUserList.SetAt(nKey, strFieldName);
		}
	}
	else if(1 == nCmbCurSel)	// Addition
	{
		for (int nIdx=0; nIdx<m_arrayAdditionIdx.GetSize(); nIdx++)
		{
			nKey = m_arrayAdditionIdx.GetAt(nIdx);
			m_mapAdditionList.Lookup(nKey, strFieldName);

			m_arrayUserIdx.Add(nKey);
			m_mapUserList.SetAt(nKey, strFieldName);
		}
	}	

	FillShowList(m_arrayUserIdx, m_mapUserList);
}

void CDlgSettingToolBar::OnDblclkListAdd() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cmbShow.GetCurSel();
	if(nCurSel == 0)
		OnBtnAdd();
}

void CDlgSettingToolBar::OnDblclkListRemove() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cmbShow.GetCurSel();
	if(nCurSel == 0)
		OnBtnRemove();
}