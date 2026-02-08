// SettingerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SettingerDlg.h"
#include "TabCodeSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingerDlg dialog


CSettingerDlg::CSettingerDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CSettingerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bUpdatingList = FALSE;
	m_plistTimerData = NULL;
	m_pNAlarmFrom = NULL;
	m_pNAlarmTo = NULL;
	m_pBUseAlarmSound = NULL;
	m_pBUseAlarmBkClr = NULL;
	m_pStrSoundFile = NULL;
}


void CSettingerDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingerDlg)
	DDX_Control(pDX, IDC_EDITNUMTO, m_editNumberTo);
	DDX_Control(pDX, IDC_EDITNUMFROM, m_editNumberFrom);
	DDX_Control(pDX, IDC_EDITFILENAME, m_editFileName);
	DDX_Control(pDX, IDC_CHKUSESOUND, m_chkUseSound);
	DDX_Control(pDX, IDC_CHKUSECOLOR, m_chkUseColor);
	DDX_Control(pDX, IDC_CBSTARTTIME, m_cbStartTime);
	DDX_Control(pDX, IDC_EDITVOLUME, m_editVolume);
	DDX_Control(pDX, IDC_BTINSERT, m_btInsert);
	DDX_Control(pDX, IDC_BTDELETE, m_btDelete);
	DDX_Control(pDX, IDC_LIST3, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingerDlg, CRscDlg)
	//{{AFX_MSG_MAP(CSettingerDlg)
	ON_BN_CLICKED(IDC_BTINSERT, OnBtinsert)
	ON_BN_CLICKED(IDC_BTDELETE, OnBtdelete)
	ON_CBN_SELCHANGE(IDC_CBSTARTTIME, OnSelchangeCbstarttime)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, OnItemchangedList3)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BTNSEARCHSND, OnBtnsearchsnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingerDlg message handlers
BOOL CSettingerDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rect;
	GetWindowRect(rect);
	SetWindowPos(&CWnd::wndTop,m_ptButtomLeftOfBtn.x,m_ptButtomLeftOfBtn.y,0,0,SWP_NOSIZE);	
	int colwidths[3] = { 50,80 };	// sixty-fourths

	TCHAR *	lpszHeaders[] = { _T("시작"),
							  _T("거래량"),
							  NULL };
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	int w = 250 - 2*2 - 2;
	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
	//	lvcolumn.cx = (lpszHeaders[i+1] == NULL) ? w - total_cx - 2 : (w * colwidths[i]) / 64;
		lvcolumn.cx = colwidths[i];
		total_cx += lvcolumn.cx;
		m_list.InsertColumn(i, &lvcolumn);
	}


	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		m_list.SetColumn(i, &lvcolumn);
	}	
	m_list.SetHilightColor(RGB(0,0,0),RGB(230,230,230));


	m_stringArrayTime.Add("9시이전");
	m_stringArrayTime.Add("10시");
	m_stringArrayTime.Add("11시");
	m_stringArrayTime.Add("12시");
	m_stringArrayTime.Add("13시");
	m_stringArrayTime.Add("14시");
	m_stringArrayTime.Add("15시이후");


	////////////////
	int nSize = m_stringArrayTime.GetSize();
	int nIndexLastInserted = -1;
	for(int nPos=0;nPos<nSize;nPos++)
	{
		nIndexLastInserted = m_cbStartTime.AddString(m_stringArrayTime.GetAt(nPos));
		m_cbStartTime.SetItemData(nIndexLastInserted,nPos);
	}
	m_cbStartTime.SetCurSel(0);

	m_editVolume.SetWindowText("1");

	int nCntOfList = m_list.GetItemCount();

	LoadInfo();
	////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingerDlg::OnBtinsert() 
{
	m_bUpdatingList = TRUE;
	// TODO: Add your control notification handler code here
	CString strStartTime;
	CString strVolume;
	CString strVolumeModified;
	int nCurSelOfStartTime = m_cbStartTime.GetCurSel();
	m_cbStartTime.GetLBText(nCurSelOfStartTime,strStartTime);
	m_editVolume.GetWindowText(strVolume);
	strVolumeModified.Format("%s천주 이상",strVolume);

	int nPosForInsert = FindPosInListForInsert(strStartTime);
	m_list.SetItemText(nPosForInsert,0,strStartTime);
	m_list.SetItemText(nPosForInsert,1,strVolumeModified);
	m_bUpdatingList = FALSE;
}




long CSettingerDlg::FindPosInArray(LPCTSTR lpTime)
{
	int nSize = m_stringArrayTime.GetSize();
	for(int nPos=0;nPos<nSize;nPos++)
	{
		if(m_stringArrayTime.GetAt(nPos)==lpTime)
		{
			return nPos;
		}
	}
	return -1;
}

long CSettingerDlg::FindPosInListForInsert(LPCTSTR lpTime)
{
	int nPosInArray = FindPosInArray(lpTime);
	int nSize = m_list.GetItemCount();
	CString strTimeInList;
	int nPosInListInArray = -1;
	int nPosForRetValue = 0;
	for(int nPos=0;nPos<nSize;nPos++)
	{		
		strTimeInList = m_list.GetItemText(nPos,0);
		nPosInListInArray = FindPosInArray(strTimeInList);
		if(nPosInListInArray==nPosInArray)
		{
			return nPos;
		}
		else if(nPosInListInArray>nPosInArray)
		{
			break;
		}
		nPosForRetValue++;
	}
	m_list.InsertItem(nPosForRetValue,"");
	return nPosForRetValue;
}


void CSettingerDlg::OnSelchangeCbstarttime() 
{
	// TODO: Add your control notification handler code here
	CString strStartTime;
	CString strEndTime;
	int nCurSelOfStartTime = m_cbStartTime.GetCurSel();
	m_cbStartTime.GetLBText(nCurSelOfStartTime,strStartTime);		
	
	int nStartTimePosInArray = FindPosInArray(strStartTime);
	int nEndTimePosInArray = FindPosInArray(strEndTime);


	int nCurSelOfList = m_list.GetCurSel();	
	if(nCurSelOfList==LB_ERR)
	{	
	}
	else
	{
		CString strEndTimeOfList = m_list.GetItemText(nCurSelOfList,1);
		int nEndTimeOfListInArray = FindPosInArray(strEndTimeOfList);
	}
}

void CSettingerDlg::OnBtdelete() 
{
	// TODO: Add your control notification handler code here
	int nCurSelOfList = m_list.GetCurSel();
	int nPosForInsert = 0;
	if(nCurSelOfList==LB_ERR)
	{
		return;
	}
	m_list.DeleteItem(nCurSelOfList);	
	nCurSelOfList--;
	if(nCurSelOfList>=0)
	{
		m_list.SetCurSel(nCurSelOfList);
	}
	else
	{
		int nCnt = m_list.GetItemCount();
		if(nCnt)
		{
			m_list.SetCurSel(0);
		}
		
	}
}

void CSettingerDlg::OnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	if(phdn->iItem!=-1&&!m_bUpdatingList)
	{
		CString strVolume;
		CString strStartTime = m_list.GetItemText(phdn->iItem,0);
		CString strVolumeModified = m_list.GetItemText(phdn->iItem,1);		
		int nPos = m_cbStartTime.FindString(0,strStartTime);
		if(nPos!=-1)
		{
			int nFind = strVolumeModified.Find("천주 이상");
			strVolume = strVolumeModified.Left(nFind);
			m_cbStartTime.SetCurSel(nPos);
			m_editVolume.SetWindowText(strVolume);
		}
		
	}
	*pResult = 0;
}

void CSettingerDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString strVolume;
	long lDelta = pNMUpDown->iDelta*-1;
	long lVolume = 0;
	m_editVolume.GetWindowText(strVolume);
	if(!strVolume.IsEmpty())
	{
		lVolume = atol(strVolume);
		lVolume+=lDelta;
		if(lVolume<=0) return;
		strVolume.Format("%d",lVolume);
		m_editVolume.SetWindowText(strVolume);
	}	
	*pResult = 0;
}


void CSettingerDlg::SetData(CTabCodeSearch *pTabCodeSearch)
{
	m_plistTimerData = &pTabCodeSearch->m_listTimerData;
	m_pNAlarmFrom = &pTabCodeSearch->m_nAlarmFrom;
	m_pNAlarmTo = &pTabCodeSearch->m_nAlarmTo;
	m_pBUseAlarmSound = &pTabCodeSearch->m_bUseAlarmSound;
	m_pBUseAlarmBkClr = &pTabCodeSearch->m_bUseAlarmBkClr;
	m_pStrSoundFile = &pTabCodeSearch->m_strSoundFile;
}

void CSettingerDlg::LoadInfo()
{
	if(!m_plistTimerData)	return;
	m_bUpdatingList = TRUE;
	int nCntOfList = m_plistTimerData->GetSize();
	int nCntOfArrayTime = m_stringArrayTime.GetSize();
	m_list.DeleteAllItems();
	CTimerData timerData;
	CString strStartTime;
	CString strEndTime;
	CString strVolumeModified;
	int nPosForInsert = 0;
	for(int nIndex=0;nIndex<nCntOfList;nIndex++)
	{
		timerData = m_plistTimerData->GetAt(nIndex);
		if(timerData.m_lStartTime==100)
		{			
			strStartTime = m_stringArrayTime.GetAt(0);
		}
		else if(timerData.m_lStartTime>=10&&timerData.m_lStartTime<=15)
		{
			strStartTime = m_stringArrayTime.GetAt(timerData.m_lStartTime-9);
		}
		else
		{
			continue;
		}
		strVolumeModified.Format("%d천주 이상",timerData.m_lVolumeLimitation);
		m_list.InsertItem(nPosForInsert,"");
		m_list.SetItemText(nPosForInsert,0,strStartTime);
		m_list.SetItemText(nPosForInsert,1,strVolumeModified);	
		nPosForInsert++;
	}
	m_bUpdatingList = FALSE;
	int nCntOfListItem = m_list.GetItemCount();
	if(nCntOfListItem)
	{
		m_list.SetCurSel(nCntOfListItem-1);
	}
	if(m_pNAlarmFrom)
	{
		CString strTemp;
		strTemp.Format("%d",*m_pNAlarmFrom);
		m_editNumberFrom.SetWindowText(strTemp);
	}
	if(m_pNAlarmTo)
	{
		CString strTemp;
		strTemp.Format("%d",*m_pNAlarmTo);
		m_editNumberTo.SetWindowText(strTemp);
	}
	if(m_pBUseAlarmSound)
	{
		m_chkUseSound.SetCheck(*m_pBUseAlarmSound);
	}
	if(m_pBUseAlarmBkClr)
	{
		m_chkUseColor.SetCheck(*m_pBUseAlarmBkClr);
	}
	if(m_pStrSoundFile)
	{
		m_editFileName.SetWindowText(*m_pStrSoundFile);
	}	
}

void CSettingerDlg::SaveInfo()
{
	if(!m_plistTimerData) return;
	m_plistTimerData->RemoveAll();
	int nItemCntInList = m_list.GetItemCount();
	int nCntOfArrayTime = m_stringArrayTime.GetSize();
	CString strStartTime;
	CString strVolumeModified;
	CString strVolume;
	CTimerData timerData;
	int nFind = 0;
	int nPosOfArrayTime = 0;
	for(int nPos=0;nPos<nItemCntInList;nPos++)
	{
		strStartTime = m_list.GetItemText(nPos,0);
		strVolumeModified = m_list.GetItemText(nPos,1);

		nPosOfArrayTime = FindPosInArray(strStartTime);
		if(nPosOfArrayTime==0)
		{
			timerData.m_lStartTime=100;	// 장시작전..
		}
		else if(nPosOfArrayTime>0&&nPosOfArrayTime<(nCntOfArrayTime-1))
		{
			timerData.m_lStartTime=nPosOfArrayTime+9;
		}
		else
		{
			continue;
		}
		nFind = strVolumeModified.Find("천주 이상");
		strVolume = strVolumeModified.Left(nFind);
		timerData.m_lVolumeLimitation = atol(strVolume);
		if(timerData.IsAvailable())
		{
			m_plistTimerData->Add(timerData);
		}
	}
	if(m_pNAlarmFrom)
	{
		CString strTemp;
		m_editNumberFrom.GetWindowText(strTemp);
		*m_pNAlarmFrom = atol(strTemp);
	}
	if(m_pNAlarmTo)
	{
		CString strTemp;
		m_editNumberTo.GetWindowText(strTemp);
		*m_pNAlarmTo = atol(strTemp);		
	}
	if(m_pBUseAlarmSound)
	{
		*m_pBUseAlarmSound = m_chkUseSound.GetCheck();
	}
	if(m_pBUseAlarmBkClr)
	{		
		*m_pBUseAlarmBkClr  = m_chkUseColor.GetCheck();
	}
	if(m_pStrSoundFile)
	{
		CString strTemp;
		m_editFileName.GetWindowText(strTemp);
		*m_pStrSoundFile = strTemp;
	}
}

void CSettingerDlg::OnOK() 
{
	// TODO: Add extra validation here
	SaveInfo();
	CRscDlg::OnOK();
}

void CSettingerDlg::OnBtnsearchsnd() 
{
	// TODO: Add your control notification handler code here
	char szFilter[] = "Wav Files (*.wav)|*.wav|All Files (*.*)|*.*||";
	char szTargetDllPath[MAX_PATH];
	char szCurrentDirPath[MAX_PATH];
	GetWindowsDirectory(szTargetDllPath, MAX_PATH-1);
	lstrcat(szTargetDllPath,"\\");
	lstrcpyn(szTargetDllPath + lstrlen(szTargetDllPath), "Media\\", MAX_PATH - lstrlen(szTargetDllPath));

	GetCurrentDirectory(MAX_PATH,szCurrentDirPath);
	SetCurrentDirectory(szTargetDllPath);
	CFileDialog dlg(TRUE, "wav", *m_pStrSoundFile, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		*m_pStrSoundFile = dlg.GetPathName();			
	}
	SetCurrentDirectory(szCurrentDirPath);	
}
