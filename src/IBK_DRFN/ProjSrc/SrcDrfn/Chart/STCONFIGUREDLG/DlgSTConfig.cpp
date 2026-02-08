// DlgSTConfig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgSTOption.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/IDataItem.h"
#include "DlgSTConfig.h"

// CDlgSTConfig 대화 상자입니다.

CDlgSTConfig::CDlgSTConfig(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgSTConfig::IDD, pParent)
{
	m_pDlgSTOption = pParent;
}

CDlgSTConfig::~CDlgSTConfig()
{
}

void CDlgSTConfig::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT_STTITLE, m_ctlEditSTTitle);
	DDX_Control(pDX, IDC_BUTTON_SAVECONFIG, m_ctlSaveConfig);
	DDX_Control(pDX, IDC_EDIT_STNAME, m_ctlEditSTName);
	DDX_Control(pDX, IDC_LIST_STVARIABLES, m_ctlListSTVariables);
}


BEGIN_MESSAGE_MAP(CDlgSTConfig, CRscDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SAVECONFIG, OnBnClickedButtonSaveconfig)
END_MESSAGE_MAP()


// CDlgSTConfig 메시지 처리기입니다.

BOOL CDlgSTConfig::OnInitDialog()
{
	CRscDlg::OnInitDialog();

	InitColumn();
	LoadInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH CDlgSTConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return *m_pBrBkColor;		
		break;
	}
	return hbr;
}

void CDlgSTConfig::LoadInfo()
{
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();

	//////////////////////////////////////////////////////////////////////////
	// Basic
	//if(pSTControllerCfg)
	//	m_ctlEditSTTitle.SetReadOnly();
	//m_ctlEditSTTitle.SetWindowText("이름");

	m_ctlEditSTName.SetWindowText(((CDlgSTOption*)m_pDlgSTOption)->GetSdPath());	


	//////////////////////////////////////////////////////////////////////////
	// Variables
	char szBuffer[256];
	CString strText;
	CIDataItem *pDataItem = NULL;
	DWORD dwSizeRead = 0;
	CString strKeyName;
	CString strInitValue;
	CString strItemName;
	int nCnt = 0;

	if(pSTControllerCfg)
	{
		nCnt = MAXCNTOFINPUTHOLDER;
	}
	else
	{		
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(VARIABLES,INPUTCNT,"0",szBuffer,sizeof(szBuffer),2);
		nCnt = atol(szBuffer);
	}
	for(int nPos=0;nPos<nCnt;nPos++)
	{
		if(pSTControllerCfg)
		{
			pDataItem = pSTControllerCfg->GetInputHolderDataItem(nPos);
			if(!pDataItem)	break;
			strInitValue = pSTControllerCfg->GetInputs(nPos);
			strItemName = pDataItem->GetDataItemName();
		}
		else
		{
			strKeyName.Format("%s%d",INPUTNAME,nPos);
			dwSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(VARIABLES,strKeyName,"",szBuffer,sizeof(szBuffer),2);
			if(dwSizeRead)
			{
				strItemName = szBuffer;
			}
			else
			{
				break;
			}
		}

		strKeyName.Format("%s%d",INPUTVALUE,nPos);
		dwSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(VARIABLES,strKeyName,"",szBuffer,sizeof(szBuffer),2);
		if(!dwSizeRead)
		{
			if(!pSTControllerCfg) break;
			strText = strInitValue;
		}
		else
		{
			strText = szBuffer;
		}		
		if(strText.Find('.')!=-1)
		{
			strText.TrimRight('0');
			if(strText[strText.GetLength()-1]=='.') strText.TrimRight('.');
		}
		m_ctlListSTVariables.InsertItem(nPos,"");
		m_ctlListSTVariables.SetItemText(nPos,0,strItemName);
		m_ctlListSTVariables.SetItemText(nPos,1,strText);		


		int nType = ((CDlgSTOption*)m_pDlgSTOption)->GetType();
		if(!pSTControllerCfg&&(nType==1||nType==2))
		{
			m_ctlListSTVariables.SetEditBox(nPos,0);
		}
		m_ctlListSTVariables.SetEditBox(nPos,1);
	}
}

void CDlgSTConfig::ReloadInfo()
{
	// Basic
	//m_ctlEditSTTitle.SetWindowText("");

	// Variables
	m_ctlListSTVariables.DeleteAllItems();
	LoadInfo();
}

BOOL CDlgSTConfig::SaveInfo()
{
	// Basic
	if(!((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD())
	{
		CString strName;
		m_ctlEditSTName.GetWindowText(strName);
		((CDlgSTOption*)m_pDlgSTOption)->ChangeSfgPath(strName);	
	}


	// Variables
	int nItemCount = m_ctlListSTVariables.GetItemCount();	
	int nUseType = 2;//m_chkSaveAsDefault.GetCheck()==TRUE? 3 : 2;

	CString strItemText;
	CString strKeyName;
	strItemText.Format("%d",nItemCount);

	m_ctlListSTVariables.UnpressEditBox();
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,INPUTCNT,strItemText,nUseType);
	for(int nPos=0;nPos<nItemCount;nPos++)
	{		
		strItemText = m_ctlListSTVariables.GetItemText(nPos,1);
		strKeyName.Format("%s%d",INPUTVALUE,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,strKeyName,strItemText,nUseType);
		strItemText = m_ctlListSTVariables.GetItemText(nPos,0);
		strKeyName.Format("%s%d",INPUTNAME,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,strKeyName,strItemText,nUseType);
	}

	return TRUE;
}

void CDlgSTConfig::InitColumn()
{
	int colwidthsType0[6] = { 100, 124};	// sixty-fourths

	TCHAR *	lpszHeadersType0[] = {
		_T("변수명"),
		_T("변수값"),
		NULL };
	m_ctlListSTVariables.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	int i;
	int total_cx = 0;
	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	// add columns
	for (i = 0; ; i++)
	{
		if (lpszHeadersType0[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.pszText = lpszHeadersType0[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidthsType0[i];
		total_cx += lvcolumn.cx;
		m_ctlListSTVariables.InsertColumn(i, &lvcolumn);
	}
	// iterate through header items and attach the image list
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeadersType0[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = LVCFMT_CENTER;
		lvcolumn.iSubItem = i;
		m_ctlListSTVariables.SetColumn(i, &lvcolumn);
	}
	m_ctlListSTVariables.SetHilightColor(RGB(0,0,0),RGB(208,221,240));
}

LRESULT CDlgSTConfig::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case UMSG_ST_SAVE_ENV:
		{
			return SaveInfo();
		}		
		break;
	}	
	return CRscDlg::WindowProc(message, wParam, lParam);
}

void CDlgSTConfig::OnBnClickedButtonSaveconfig()
{
	((CDlgSTOption*)m_pDlgSTOption)->ApplyConfig();
}
