// TabVariables.cpp : implementation file
//

#include "stdafx.h"
#include "TabVariables.h"
#include "DlgSTOption.h"
#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/IDataItem.h"
#include "../Include_ST/ISTControllerD.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabVariables dialog


CTabVariables::CTabVariables(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabVariables::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabVariables)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlgSTOption = pParent;
}


void CTabVariables::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabVariables)
	DDX_Control(pDX, IDC_CHK_SAVEASDEFAULT, m_chkSaveAsDefault);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_LIST_VARIABLES, m_listVariables);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabVariables, CRscDlg)
	//{{AFX_MSG_MAP(CTabVariables)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabVariables message handlers

BOOL CTabVariables::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();

	InitColumn();
	int nType = ((CDlgSTOption*)m_pDlgSTOption)->GetType();
	if(nType==1||nType==2)
	{
		m_btnAdd.EnableWindow();
		m_btnDelete.EnableWindow();
	}
	else
	{
		m_btnAdd.EnableWindow(FALSE);
		m_btnDelete.EnableWindow(FALSE);
	}

	if(!pSTControllerCfg)
	{
		m_chkSaveAsDefault.SetCheck(1);
		m_chkSaveAsDefault.EnableWindow(FALSE);
	}
	else
	{
		CString strSTNamePath = pSTControllerCfg->GetSTNamePath();
		if(!strSTNamePath.GetLength())
		{
			m_chkSaveAsDefault.SetCheck(1);
			m_chkSaveAsDefault.EnableWindow(FALSE);
		}
	}
	LoadInfo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabVariables::InitColumn()
{
	int colwidthsType0[6] = { 100, 200};	// sixty-fourths

	TCHAR *	lpszHeadersType0[] = {
							  _T("변수명"),
							  _T("변수값"),
							  NULL };
	m_listVariables.SetExtendedStyle(LVS_EX_FULLROWSELECT);

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
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.pszText = lpszHeadersType0[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidthsType0[i];
		total_cx += lvcolumn.cx;
		m_listVariables.InsertColumn(i, &lvcolumn);
	}
	// iterate through header items and attach the image list
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeadersType0[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		m_listVariables.SetColumn(i, &lvcolumn);
	}
	m_listVariables.SetHilightColor(RGB(0,0,0),RGB(208,221,240));
}

LRESULT CTabVariables::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_ST_SAVE_ENV:
		{
			return SaveInfo();
		}		
		break;
	case UMSG_ST_REMOVE_ENV:
		{
			RemoveInfo();
		}
		break;
	}	
	return CRscDlg::WindowProc(message, wParam, lParam);
}

void CTabVariables::LoadInfo()
{
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();
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
		m_listVariables.InsertItem(nPos,"");
		m_listVariables.SetItemText(nPos,0,strItemName);
		m_listVariables.SetItemText(nPos,1,strText);		


		int nType = ((CDlgSTOption*)m_pDlgSTOption)->GetType();
		if(!pSTControllerCfg&&(nType==1||nType==2))
		{
			m_listVariables.SetEditBox(nPos,0);
		}
		m_listVariables.SetEditBox(nPos,1);
	}
}

BOOL CTabVariables::SaveInfo()
{
	int nItemCount = m_listVariables.GetItemCount();	
	int nUseType = m_chkSaveAsDefault.GetCheck()==TRUE? 3 : 2;

	CString strItemText;
	CString strKeyName;
	strItemText.Format("%d",nItemCount);

	m_listVariables.UnpressEditBox();
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,INPUTCNT,strItemText,nUseType);
	for(int nPos=0;nPos<nItemCount;nPos++)
	{		
		strItemText = m_listVariables.GetItemText(nPos,1);
		strKeyName.Format("%s%d",INPUTVALUE,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,strKeyName,strItemText,nUseType);
		strItemText = m_listVariables.GetItemText(nPos,0);
		strKeyName.Format("%s%d",INPUTNAME,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,strKeyName,strItemText,nUseType);
	}
	return TRUE;
}

void CTabVariables::RemoveInfo()
{
	int nItemCount = m_listVariables.GetItemCount();	
	int nUseType = 2; // Remove In STFile

	CString strKeyName;
	m_listVariables.UnpressEditBox();
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,INPUTCNT,"",nUseType);
	for(int nPos=0;nPos<nItemCount;nPos++)
	{		
		strKeyName.Format("%s%d",INPUTVALUE,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,strKeyName,"",nUseType);
		strKeyName.Format("%s%d",INPUTNAME,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(VARIABLES,strKeyName,"",nUseType);
	}
}


void CTabVariables::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
	int nCnt = m_listVariables.GetItemCount();
	int nPos = m_listVariables.GetCurSel() + 1;
	if(nPos==0&&nCnt>0)
	{
		nPos = nCnt;
	}
	CString strName;
	for(int nIndex=0;nIndex<100;nIndex++)
	{
		strName.Format("NewVariable%d",nIndex);
		if(CheckWhetherHasSameName(strName))
		{
			m_listVariables.InsertItem(nPos,"");
			m_listVariables.SetItemText(nPos,0,strName);
			m_listVariables.SetEditBox(nPos,0);
			m_listVariables.SetItemText(nPos,1,"0");
			m_listVariables.SetEditBox(nPos,1);
			m_listVariables.SetCurSel(nPos);
			return;
		}
	}	
}

void CTabVariables::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	int nCnt = m_listVariables.GetItemCount();
	int nPos = m_listVariables.GetCurSel();
	if(nPos==-1) return;
	m_listVariables.DeleteItem(nPos);
	nCnt -= 1;
	if(nCnt>nPos&&nCnt>0)
	{
		m_listVariables.SetCurSel(nPos);
	}
	else if(nCnt>0)
	{
		m_listVariables.SetCurSel(nCnt-1);
	}
	
}

BOOL CTabVariables::CheckWhetherHasSameName(LPCTSTR lpNewNameForInsertion)
{
	CString strItemName;
	int nCnt = m_listVariables.GetItemCount();
	for(int nPos=0;nPos<nCnt;nPos++)
	{
		strItemName = m_listVariables.GetItemText(nPos,0);
		if(strItemName==lpNewNameForInsertion)
		{
			return FALSE;
		}
	}
	return TRUE;
}


HBRUSH CTabVariables::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
