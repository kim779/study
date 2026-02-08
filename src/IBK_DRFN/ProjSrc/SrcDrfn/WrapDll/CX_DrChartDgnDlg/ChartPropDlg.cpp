// ChartPropDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wDrChartDgnDlg.h"
#include "ChartPropDlg.h"

#include "../common/PropertyMap.h"
#include "../../ForSite/FS_Env.h"

#include "FidSetDlg.h"
#include "TrSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartPropDlg dialog


CChartPropDlg::CChartPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChartPropDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartPropDlg)
	m_nTRType = 0;
	m_nSortType = FALSE;
	m_bDirectSend = FALSE;
	//}}AFX_DATA_INIT
	//m_nFID = 0;
	m_pChart = NULL;
}


void CChartPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartPropDlg)
	DDX_Control(pDX, IDC_CHKDIRECT, m_ChkDirect);
	DDX_Control(pDX, IDC_STATIC_CNTTITLE, m_stcCntTitle);
	DDX_Control(pDX, IDC_BTN_DATASET, m_btnDataSet);
	DDX_Control(pDX, IDC_EDIT_ROWCNT, m_editRowCnt);
	DDX_Radio(pDX, IDC_RADIO_FID, m_nTRType);
	DDX_Check(pDX, IDC_DATASORT, m_nSortType);
	DDX_Check(pDX, IDC_CHKDIRECT, m_bDirectSend);
	DDX_Control(pDX, IDC_STATIC_GRIDSYMBOL, m_stcGridSymbol);
	DDX_Control(pDX, IDC_EDIT_GRIDSYMBOL, m_editGridSymbol);
	DDX_Control(pDX, IDC_COMBO_CHARTTYPE, m_cbChartType);
	//}}AFX_DATA_MAP
	//DDX_Radio(pDX, IDC_RADIO_FID, m_nFID);
	//DDX_Control(pDX, IDC_HMCCHARTCTRL, m_chart);
}


BEGIN_MESSAGE_MAP(CChartPropDlg, CDialog)
	//{{AFX_MSG_MAP(CChartPropDlg)
	ON_BN_CLICKED(IDC_BTN_DATASET, OnBtnDataset)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_FID, OnRadioFid)
	ON_BN_CLICKED(IDC_RADIO_TR, OnRadioTr)
	ON_BN_CLICKED(IDC_DATASORT, OnDatasort)
	ON_BN_CLICKED(IDC_CHKDIRECT, OnChkdirect)
	ON_CBN_SELCHANGE(IDC_COMBO_CHARTTYPE, OnSelchangeCmbChartType)
	ON_BN_CLICKED(IDC_DRSAVE_CHARTSET, OnDrsaveChartset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartPropDlg message handlers

void CChartPropDlg::OnBtnDataset() 
{
	if (m_converter.m_bDirectSend)
	{
		if (m_nTRType)
		{
			CTrSetDlg	dlg(this);
			dlg.SetWindowText(_T("TR Set Dialog"));
			dlg.SetTRQry(m_converter.GetTrQryArray());
			dlg.m_bDirectSend = TRUE;
			if (dlg.DoModal() == IDOK)
				dlg.GetTRQry(m_converter.GetTrQryArray());
		}
		else
		{
			CFidSetDlg	dlg(this);
			dlg.SetWindowText(_T("FID Set Dialog"));
			dlg.SetFIDQry(m_converter.GetFidQryArray());
			dlg.m_bDirectSend = TRUE;
			if (dlg.DoModal() == IDOK)
				dlg.GetFIDQry(m_converter.GetFidQryArray());
		}
	}
	else
	{
		CTrSetDlg	dlg(this);
		dlg.SetWindowText(_T("TR Set Dialog"));
		dlg.SetTRQry(m_converter.GetTrQryArray());
		dlg.m_bDirectSend = FALSE;
		if (dlg.DoModal() == IDOK)
			dlg.GetTRQry(m_converter.GetTrQryArray());
	}
}

void CChartPropDlg::OnOK() 
{
	CPropertyMap	pmProperty;
	IPropertyMap *	pIPropertyMap = &pmProperty;

	//m_chart.GetAllProperties3((long)pIPropertyMap);
	if(m_pChart) m_pChart->GetAllProperties3((long)pIPropertyMap);

	m_converter.SetPropMap(pIPropertyMap);

	SaveFile();

	CDialog::OnOK();
}

void CChartPropDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CChartPropDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

// 	m_hCtrlWnd = m_chart.GetSafeHwnd();
// 	m_chart.SetRunTimeMode(0);
// 	// OCX에 경로 설정
// 	CString szRootDir; GetDir_DrfnRoot(szRootDir);
// 	m_chart.SetHtsPath(szRootDir);
// 	m_chart.SetContainerDllInfo((long)GetSafeHwnd(), IDC_SMSCHARTCTRL);
// 	
// 	// 4. Set Loaded Property
// 	SetAllPropertiesToOCX();
// 	m_chart.ResetChart();
// 	m_chart.ShowWindow(SW_SHOW);

	m_pChart = new CDRChartOcx();
	BOOL bReturn = m_pChart->Create( NULL, WS_VISIBLE | WS_CHILD, CRect(0,0,444,212), this, IDC_SMSCHARTCTRL);
	if( bReturn)
	{
		m_hCtrlWnd = m_pChart->GetSafeHwnd();

		m_pChart->SetRunTimeMode(0);

		// OCX에 경로 설정
		CString szRootDir; GetDir_DrfnRoot(szRootDir);

		m_pChart->SetHtsPath(szRootDir);
		m_pChart->SetContainerDllInfo((long)GetSafeHwnd(), IDC_SMSCHARTCTRL);

		// 4. Set Loaded Property
		SetAllPropertiesToOCX();
		m_pChart->ResetChart();
		m_pChart->ShowWindow(SW_SHOW);
	}
	else
	{
		delete m_pChart;
		m_pChart = NULL;
		AfxMessageBox("DRChart 생성 Failed");
	}

	CString strData;
	strData.Format(_T("%d"), m_converter.m_nCnt);
	m_editRowCnt.SetWindowText(strData);
	m_editGridSymbol.SetWindowText(m_converter.GetGridSymbol());

	m_ChkDirect.SetCheck(m_converter.m_bDirectSend);

	//>>0001087: 맵으로 CME화면 가능하도록 기능추가.
	int nMaxCount = m_cbChartType.GetCount();
	int nSel = m_converter.m_nChartType;
	if(nSel>=nMaxCount) nSel = nMaxCount-1;
	m_cbChartType.SetCurSel(nSel);
	//<<0001087

	ShowFIDCtls();

	CheckDlgButton(IDC_DATASORT, m_nSortType);
	CheckDlgButton(IDC_REALDATA_ABS, m_converter.m_nRealAbs);

	CheckEnableOfChartSet();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CChartPropDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadFile();

	return 0;
}

void CChartPropDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (m_pChart)
	{
		m_pChart->DestroyWindow();
		delete m_pChart;
		m_pChart = NULL;
	}
}

BOOL CChartPropDlg::SetAllPropertiesToOCX()
{
	if (IsWindow(m_hCtrlWnd) && m_pChart)
		//return m_chart.SetAllProperties3((long)m_converter.GetPropMap());
		return m_pChart->SetAllProperties3((long)m_converter.GetPropMap());

	return FALSE;
}

LRESULT CChartPropDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	if (message==RMSG_GET_USERID)
//		return (long)(LPCSTR)strID;

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CChartPropDlg::SetAllPath(CString strPath)
{
	int nFind = strPath.ReverseFind(_T('\\'));

	if (nFind <= 0)
		return FALSE;

	m_strMapPath = strPath.Left(nFind+1);
	m_strFileName = strPath.Right(strPath.GetLength()-nFind-1);

	return TRUE;
}

void CChartPropDlg::SaveFile()
{
	CString strPath;
	strPath = m_strMapPath + m_strFileName + _T(".map");
	CFile file;
	if (!file.Open(strPath, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
	{
		CString strMsg;
		strMsg = _T("failed create file \"") + strPath + _T("\"");
		AfxMessageBox(strMsg);

		return ;
	}

	CArchive ar(&file, CArchive::store);

	//m_converter.m_nFID = m_nFID;
	m_converter.m_nTRType = m_nTRType;
	m_converter.m_nSortType = m_nSortType;
	CString strData;
	m_editRowCnt.GetWindowText(strData);
	m_converter.m_nCnt = atoi(strData);

	m_converter.m_nRealAbs = IsDlgButtonChecked(IDC_REALDATA_ABS);

	m_editGridSymbol.GetWindowText(strData);
	m_converter.SetGridSymbol(strData);

	m_converter.Serialize(ar);

	ar.Close();
	file.Close();
}

#include "ChartTypeDlg.h"
void CChartPropDlg::LoadFile()
{
	CString strPath;
	{
		CString szMap = m_strFileName;
		szMap.MakeUpper();
		int nPos = szMap.Find(".MAP");
		if(nPos>0)
			m_strFileName = szMap.Left(nPos);
	}

	strPath = m_strMapPath + m_strFileName + _T(".map");
	CFile file;
	if (!file.Open(strPath, CFile::typeBinary|CFile::modeRead))
	{
		//기존에 map으로 저장된 경우가 없는 경우임.
		//CString strMsg;
		//strMsg = _T("not find file \"") + strPath + _T("\"");
		//AfxMessageBox(strMsg);

		//기존에 세팅된게 없을 때는 Default Style을 보여주고 선택하도록 한다.
		CChartTypeDlg	dlg;
		if(dlg.DoModal()==IDOK)
		{
			char* szSect = (LPSTR)(LPCSTR)dlg.m_szSelectKey;
			CString szBaseDir, szCFG;
			dlg.GetBasePath(szBaseDir);
			szCFG = szBaseDir + "\\ChartType.ini";

			char aDataFile[MAX_PATH]={0,};
			if(::GetPrivateProfileString(szSect, "BaseDataFile", "",aDataFile,  MAX_PATH, szCFG)>0)
			{
				strPath = szBaseDir + "\\" + aDataFile;

				if (!file.Open(strPath, CFile::typeBinary|CFile::modeRead))
				{
					CString szMsg;
					szMsg.Format("[%s] File이 발견되지 않습니다.", strPath);
						
					AfxMessageBox(szMsg);
					return;
				}
			}
			else
			{
				AfxMessageBox("파일지정이 되어있지 않습니다.");
				return;
			}
		}
		else
		{
			return ;
		}
	}

	CArchive ar(&file, CArchive::load);

	m_converter.Serialize(ar);

	//m_nFID = m_converter.m_nFID;
	m_nTRType = m_converter.m_nTRType;
	m_nSortType = m_converter.m_nSortType;
	m_bDirectSend = m_converter.m_bDirectSend;
	CheckDlgButton(IDC_DATASORT, m_nSortType);
	CheckDlgButton(IDC_REALDATA_ABS, m_converter.m_nRealAbs);
	CString strData;
	strData.Format(_T("%d"), m_converter.m_nCnt);
	m_editRowCnt.SetWindowText(strData);
	m_editGridSymbol.SetWindowText(m_converter.GetGridSymbol());

	ShowFIDCtls();

	ar.Close();
	file.Close();
}

void CChartPropDlg::OnRadioFid()
{
	//m_nFID = 0;
	m_nTRType = 0;
	ShowFIDCtls();
}

void CChartPropDlg::OnRadioTr()
{
	//m_nFID = 1;
	m_nTRType = 1;
	ShowFIDCtls();
}

void CChartPropDlg::ShowFIDCtls()
{
	if ((m_converter.m_bDirectSend) || m_nTRType)
	{
		m_btnDataSet.EnableWindow(TRUE);
		m_editRowCnt.EnableWindow(FALSE);
		m_editGridSymbol.EnableWindow(FALSE);
	}
	else
	{
		m_btnDataSet.EnableWindow(FALSE);
		m_editRowCnt.EnableWindow(TRUE);
		m_editGridSymbol.EnableWindow(TRUE);
	}
}


void CChartPropDlg::OnDatasort() 
{
	m_nSortType = IsDlgButtonChecked(IDC_DATASORT);	
}

void CChartPropDlg::OnChkdirect()
{
	m_converter.m_bDirectSend = m_ChkDirect.GetCheck();
	ShowFIDCtls();
}

void CChartPropDlg::OnSelchangeCmbChartType()
{
	m_converter.m_nChartType = m_cbChartType.GetCurSel();
}

void CChartPropDlg::CheckEnableOfChartSet()
{
	CString strPath;
	strPath.Format("%s\\DrDeveloper.ini", m_strMapPath);

	CFileFind finder;
	if(finder.FindFile(strPath)==TRUE)
	{
		GetDlgItem(IDC_DRSAVE_CHARTSET)->ShowWindow(SW_SHOW);
	}
}

void CChartPropDlg::OnDrsaveChartset() 
{
	//CString strPath(_T("c:\\Logs\\test.ini"));
	CString strPath;
	{
		CString szMap = m_strFileName;
		szMap.MakeUpper();
		int nPos = szMap.Find(".MAP");
		if(nPos>0)
			m_strFileName = szMap.Left(nPos);
	}
	
	strPath = m_strMapPath + m_strFileName + _T(".dcp");		//DrChartProperties -> dcp

	CString szMessage;
	if(m_converter.SaveOnlyChartProperties(strPath)==TRUE)
		szMessage.Format("파일(%s)\n정상적으로 저장되었습니다.", strPath);
	else
		szMessage.Format("파일(%s)\n정상적으로 저장되지 않았습니다.", strPath);

	AfxMessageBox(szMessage);
}
