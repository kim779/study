// ChartListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartListDlg.h"
//#include "TitleReceiverDlg.h"
#include "./define/ChartMsgDef.h"
#include "ChartMng.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartListDlg dialog


CChartListDlg::CChartListDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CChartListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChartListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nInfoType = 0;
	m_pPtTopLeftOfDlg = NULL;
}


void CChartListDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChartListDlg)
	DDX_Control(pDX, IDC_LISTCHART, m_listChart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChartListDlg, CRscDlg)
	//{{AFX_MSG_MAP(CChartListDlg)
	ON_LBN_DBLCLK(IDC_LISTCHART, OnDblclkListchart)
	ON_BN_CLICKED(IDC_BTNNEW, OnBtnnew)
	ON_NOTIFY(NM_CLICK, IDC_LISTCHART, OnClickListchart)
	ON_BN_CLICKED(IDC_ITEMUP, OnItemup)
	ON_BN_CLICKED(IDC_ITEMDOWN, OnItemdown)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_BTNDELETE, OnBtndelete)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChartListDlg message handlers

BOOL CChartListDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	switch(m_nInfoType)
	{
	case 0:	// load
		GetDlgItem(IDC_BTNNEW)->ShowWindow(SW_HIDE);
		SetWindowText("차트불러오기");
		break;
	case 1:	// save
		SetWindowText("차트저장하기");
		break;
	}
	m_strChartSelected.Empty();
	//{{리스트컨트롤 초기화
	m_listChart.DeleteAllItems();
	m_listChart.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	CRect rect;
	m_listChart.GetClientRect(&rect);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;

	lvc.fmt = LVCFMT_CENTER;
	lvc.iSubItem = 0;
	lvc.cx = 40;						
	lvc.pszText = "번호";
	m_listChart.InsertColumn(0, &lvc);

	lvc.fmt = LVCFMT_LEFT;
	lvc.iSubItem = 1;
	lvc.cx = rect.Width() - 40;			
	lvc.pszText = "저장이름";
	m_listChart.InsertColumn(1, &lvc);
	//}}

	if(m_strUserPath.GetLength())	
	{
		int nCnt = 0;
		CString strIndexFile = m_strUserPath + "\\ChartIndex.dat";
		CString strIndex, strFileTitle, strKey;
		char szKey[30];

		sprintf(szKey,"%s","CHART LIST");
		nCnt = ::GetPrivateProfileInt(szKey, "COUNT", -1, strIndexFile);

		for(int i=0;i < nCnt; i++)
		{
			strIndex.Format("%02d", i);
			::GetPrivateProfileString(szKey, strIndex, "", strFileTitle.GetBuffer(MAX_PATH), MAX_PATH, strIndexFile);	
			strFileTitle.ReleaseBuffer();
		
			if (strFileTitle.GetLength() == 0)
				continue;

			strIndex.Format("%d", m_listChart.GetItemCount() + 1);

			AddRow(strIndex, strFileTitle);
			m_listChart.SetItemState(m_listChart.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		}

//		CFileFind ff;
//		CString strMultiCharts = m_strUserPath + "\\*.mct";
//
//		BOOL bResult = ff.FindFile(strMultiCharts);
//		if(bResult)
//		{
//			BOOL bFileExist = TRUE;
//			while(bFileExist)
//			{
//				bFileExist = ff.FindNextFile();
//				//CString strTemp = ff.GetFileTitle();
//				//int nPos = strTemp.ReverseFind('_');
//				//CString strFileTitle = strTemp.Mid(nPos + 1);
//				CString strFileTitle = ff.GetFileTitle();
//				//m_listChart.AddString(strFileTitle);
//				CString strIndex;
//				strIndex.Format("%d", m_listChart.GetItemCount() + 1);
//
//				AddRow(strIndex, strFileTitle);
//				m_listChart.SetItemState(m_listChart.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
//			}
//		}
	}
	if(m_pPtTopLeftOfDlg)
	{
		SetWindowPos(&CWnd::wndTop,m_pPtTopLeftOfDlg->x,m_pPtTopLeftOfDlg->y,0,0,SWP_NOSIZE);	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChartListDlg::AddRow(LPCTSTR szNo, LPCTSTR szName)
{
	int nIndex = m_listChart.InsertItem(m_listChart.GetItemCount(), szNo, 0);
	m_listChart.SetItemText(nIndex, 1, szName);
}

void CChartListDlg::OnDblclkListchart() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listChart.GetFirstSelectedItemPosition();
	int nCurSel = -1;
	if (pos != NULL)
		nCurSel = m_listChart.GetNextSelectedItem(pos);

	if(nCurSel!=-1)
	{
		CString strTitleSelected;
		strTitleSelected = m_listChart.GetItemText(nCurSel, 1);
		m_strChartSelected = m_strUserPath + "\\"+ strTitleSelected + ".mct";
//		switch(m_nMarketType)
//		{
//			case STOCK_CHART:
//				m_strChartSelected = m_strUserPath + "\\0301_"+ strTitleSelected + ".mct";
//				break;
//			case FUTOPT_CHART:
//				m_strChartSelected = m_strUserPath + "\\0303_"+ strTitleSelected + ".mct";
//				break;
//			case UPJONG_CHART:
//				m_strChartSelected = m_strUserPath + "\\0302_"+ strTitleSelected + ".mct";
//				break;
//			case FOREIGN_CHART:
//				m_strChartSelected = m_strUserPath + "\\0304_"+ strTitleSelected + ".mct";
//				break;
//		}		
		EndDialog(IDOK);
	}
}

void CChartListDlg::OnOK() 
{
	// TODO: Add extra validation here
	//POSITION pos = m_listChart.GetFirstSelectedItemPosition();
	//int nCurSel = -1;
	//if (pos != NULL)
	//	nCurSel = m_listChart.GetNextSelectedItem(pos);

	//if(nCurSel!=-1)
	//{
		CString strTitleSelected;
	//	strTitleSelected = m_listChart.GetItemText(nCurSel,1);
//		switch(m_nMarketType)
//		{
//			case STOCK_CHART:
//				m_strChartSelected = m_strUserPath + "\\0301_"+ strTitleSelected + ".mct";
//				break;
//			case FUTOPT_CHART:
//				m_strChartSelected = m_strUserPath + "\\0303_"+ strTitleSelected + ".mct";
//				break;
//			case UPJONG_CHART:
//				m_strChartSelected = m_strUserPath + "\\0302_"+ strTitleSelected + ".mct";
//				break;
//			case FOREIGN_CHART:
//				m_strChartSelected = m_strUserPath + "\\0304_"+ strTitleSelected + ".mct";
//				break;
//		}

		GetDlgItemText(IDC_SAVENAME, strTitleSelected);
		m_strChartSelected = m_strUserPath + "\\"+ strTitleSelected + ".mct" ;
	//}
	CRscDlg::OnOK();
}

void CChartListDlg::OnBtnnew() 
{
	// TODO: Add your control notification handler code here
//	CTitleReceiverDlg dlg;
//	if(dlg.DoModal()==IDOK)
//	{
//		dlg.m_strNewTitle.TrimRight();
//		if(!dlg.m_strNewTitle.IsEmpty())
//		{
//			m_listChart.AddString(dlg.m_strNewTitle);
//			m_listChart.SetCurSel(m_listChart.GetCount()-1);
//			GetDlgItem(IDOK)->SetFocus();
//		}
//	}
}

void CChartListDlg::OnClickListchart(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int nIndex = pNMListView->iItem;	
	if( nIndex < 0 ) return;
	SetDlgItemText(IDC_SAVENAME, m_listChart.GetItemText(nIndex, 1));
	GetDlgItem(IDC_SAVENAME)->SetFocus();
	((CEdit*)GetDlgItem(IDC_SAVENAME))->SetSel(0, -1);

	*pResult = 0;
}

void CChartListDlg::OnItemup() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listChart.GetFirstSelectedItemPosition();
	int nCurSel = -1;
	if (pos != NULL)
		nCurSel = m_listChart.GetNextSelectedItem(pos);
	if (nCurSel > 0)
		SwapData(nCurSel, nCurSel - 1);	
}

void CChartListDlg::OnItemdown() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listChart.GetFirstSelectedItemPosition();
	int nCurSel = -1;
	if (pos != NULL)
		nCurSel = m_listChart.GetNextSelectedItem(pos);
	if (nCurSel < (m_listChart.GetItemCount() - 1))
		SwapData(nCurSel, nCurSel + 1);	
}

void CChartListDlg::SwapData(int srcIdx, int dstIdx)
{
	CString srcName, dstName;

	srcName = m_listChart.GetItemText(srcIdx, 1);
	dstName = m_listChart.GetItemText(dstIdx, 1);
	m_listChart.SetItemText(srcIdx, 1, dstName);
	m_listChart.SetItemText(dstIdx, 1, srcName);
	m_listChart.SetItemState(dstIdx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	WriteIndexFile();
}

void CChartListDlg::WriteIndexFile()
{
	CString strNo, strName;
	int nCnt = m_listChart.GetItemCount();
	char szKey[30];
	CString strValue;
	CString strIndexFile = m_strUserPath + "\\ChartIndex.dat";
	sprintf(szKey,"%s","CHART LIST");

	strValue.Format("%d",nCnt);
	::WritePrivateProfileString(szKey, "COUNT", strValue, strIndexFile);
	for (int i=0; i < nCnt ; i++)
	{ //CListBox
		strNo = m_listChart.GetItemText(i, 0);
		strName = m_listChart.GetItemText(i, 1);
		strNo.Format("%02d", i);
		::WritePrivateProfileString(szKey, strNo, strName, strIndexFile);
	}	
	SetDlgItemText(IDC_SAVENAME, "");
}

void CChartListDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CString strFileName;
	GetDlgItemText(IDC_SAVENAME, strFileName);
	strFileName.TrimRight();

	if(strFileName.GetLength()<=0)
	{
		MessageBox("등록할 파일명이 없습니다.",DLLTITLENAME);
		GetDlgItem(IDC_SAVENAME)->SetFocus();
		return;
	}
	m_strChartSelected = m_strUserPath + "\\"+ strFileName + ".mct" ;
	m_pChartMng->SaveChart(m_strChartSelected);

	BOOL bExist = FALSE;
	for(int i = 0; i < m_listChart.GetItemCount(); i++)
	{
		CString str = m_listChart.GetItemText(i, 1);
		if(!strFileName.CompareNoCase(str))
		{
			bExist = TRUE;
		}
	}
	if(bExist)
	{
	}
	else
	{		
		CFile pfile;
		CFileFind ff;
		if(ff.FindFile(m_strChartSelected,0))
		{
			CString strIndex;
			strIndex.Format("%d", m_listChart.GetItemCount() + 1);
			AddRow(strIndex, strFileName);
			m_listChart.SetItemState(m_listChart.GetItemCount() - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			WriteIndexFile();
		}
	}
}

void CChartListDlg::OnBtndelete() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listChart.GetFirstSelectedItemPosition();
	int nCurSel = -1;
	if (pos != NULL)
		nCurSel = m_listChart.GetNextSelectedItem(pos);
	if(nCurSel!=-1)
	{
		BOOL bRetValue = MessageBox("삭제하시겠습니까?",DLLTITLENAME,MB_YESNO);
		if(bRetValue==IDYES)
		{
			DeleteFile(nCurSel);
			m_listChart.DeleteItem(nCurSel);
			CString strIndex;
			for(int i=nCurSel; i<m_listChart.GetItemCount();i++)
			{
				strIndex.Format("%d", (i+1));
				m_listChart.SetItemText(i, 0, strIndex);
			}
			m_listChart.SetItemState(nCurSel - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			WriteIndexFile();
			//m_listChart.DeleteString(nCurSel);
		}
	}
	else
	{
		MessageBox("삭제할 파일을 선택하십시오.",DLLTITLENAME);
	}	
}

void CChartListDlg::OnRename() 
{
	// TODO: Add your control notification handler code here
	POSITION pos = m_listChart.GetFirstSelectedItemPosition();
	
	if (pos != NULL)
	{
		int nIndex;
		CString strSaveName;
		CString strOldName;

		nIndex = m_listChart.GetNextSelectedItem(pos);
		strOldName = m_listChart.GetItemText(nIndex, 1);
		GetDlgItemText(IDC_SAVENAME, strSaveName);

		if (!strSaveName.IsEmpty() && strSaveName.CompareNoCase(strOldName) != 0) 
		{
			m_strChartSelected = m_strUserPath + "\\"+ strSaveName + ".mct" ;
			m_pChartMng->SaveChart(m_strChartSelected);
			DeleteFile(nIndex);
			m_listChart.SetItemText(nIndex, 1, strSaveName);
			WriteIndexFile();
		}
	}
}

void CChartListDlg::DeleteFile(int nIndex)
{
	CString strTitleSelected;
	CString strTitleSelectedExtra1;
	CString strTitleSelectedExtra2;
	CString strTitleSelectedFile;
	CString strChartSelectedFile;
	long lLengthAll = 0;
	long lLengthOne = 0;
	strTitleSelected = m_listChart.GetItemText(nIndex,1);

	strChartSelectedFile = m_strUserPath + "\\"+ strTitleSelected + ".mct" ;
	strTitleSelectedExtra1 = m_strUserPath + "\\"+ strTitleSelected + "_*" ;
	strTitleSelectedExtra2 = m_strUserPath + "\\IndiCond_"+ strTitleSelected + "_*" ;
	BOOL bRetValue = ::DeleteFile(strChartSelectedFile);
	CFileFind ff;
	BOOL bFindNextFile = FALSE;
	if(ff.FindFile(strTitleSelectedExtra1,0))
	{
		lLengthAll = strTitleSelectedExtra1.GetLength();
		while(1)
		{
			bFindNextFile = ff.FindNextFile();
			strTitleSelectedFile = ff.GetFilePath();
			lLengthOne = strTitleSelectedFile.GetLength();
			//if((lLengthAll + 3)==lLengthOne&&strTitleSelectedFile.Find('.')==-1)
			//{
				::DeleteFile(strTitleSelectedFile);
			//}
			if(!bFindNextFile)	break;
		}
	}
	if(ff.FindFile(strTitleSelectedExtra2,0))
	{
		lLengthAll = strTitleSelectedExtra2.GetLength();
		while(1)
		{
			bFindNextFile = ff.FindNextFile();
			strTitleSelectedFile = ff.GetFilePath();
			lLengthOne = strTitleSelectedFile.GetLength();
			//if((lLengthAll + 3)==lLengthOne&&strTitleSelectedFile.Find('.')==-1)
			//{
				::DeleteFile(strTitleSelectedFile);
			//}
			if(!bFindNextFile)	break;
		}
	}	
}

