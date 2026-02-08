// TabIndex.cpp : implementation file
//

#include "stdafx.h"
#include "TabIndex.h"
#include "DlgSTOption.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"
#include "../Common_ST/IDataItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabIndex dialog
extern CString Parser(LPCTSTR lpInputData, int *pNShifter );


CTabIndex::CTabIndex(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabIndex)
	//}}AFX_DATA_INIT
	m_pDlgSTOption = pParent;
}


void CTabIndex::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabIndex)
	DDX_Control(pDX, IDC_ST_THICK, m_stThick);
	DDX_Control(pDX, IDC_ST_SHAPE, m_stShape);
	DDX_Control(pDX, IDC_CHK_SAVEASDEFAULT, m_chkSaveAsDefault);
	DDX_Control(pDX, IDC_BTN_GRAPH_CLR, m_btnGraphClr);
	DDX_Control(pDX, IDC_LIST_GRAPH, m_listGraph);
	DDX_Control(pDX, IDC_CB_GRAPH_TYPE, m_cbGraphType);
	DDX_Control(pDX, IDC_CB_GRAPH_THICK, m_cbGraphThick);
	DDX_Control(pDX, IDC_CB_GRAPH_SHAPE, m_cbGraphShape);
	DDX_Control(pDX, IDC_BTN_CLT_BEAR, m_btnClrBear);
	DDX_Control(pDX, IDC_BTN_CLR_BULL, m_btnClrBull);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabIndex, CRscDlg)
	//{{AFX_MSG_MAP(CTabIndex)
	ON_BN_CLICKED(IDC_BTN_CLR_BULL, OnBtnClrBull)
	ON_BN_CLICKED(IDC_BTN_CLT_BEAR, OnBtnCltBear)
	ON_BN_CLICKED(IDC_BTN_GRAPH_CLR, OnBtnGraphClr)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_GRAPH, OnItemchangedListGraph)
	ON_CBN_SELCHANGE(IDC_CB_GRAPH_SHAPE, OnSelchangeCbGraphShape)
	ON_CBN_SELCHANGE(IDC_CB_GRAPH_THICK, OnSelchangeCbGraphThick)
	ON_CBN_SELCHANGE(IDC_CB_GRAPH_TYPE, OnSelchangeCbGraphType)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabIndex message handlers
BOOL CTabIndex::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();
	InitColumn();
	InitComboBox();
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

void CTabIndex::InitComboBox()
{
	int i = 0;
	m_cbGraphThick.m_bitmap.LoadBitmap(IDB_LINE_THICKNESS);
	m_cbGraphThick.AddString("");
	m_cbGraphThick.AddString("");
	m_cbGraphThick.AddString("");
	m_cbGraphThick.AddString("");
	m_cbGraphThick.AddString("");
	for ( i= -1 ;i < m_cbGraphThick.GetCount();i++) 
		m_cbGraphThick.SetItemHeight(i, 14);

	m_cbGraphShape.m_bitmap.LoadBitmap(IDB_LINE_STYLE);
	m_cbGraphShape.AddString("");
	m_cbGraphShape.AddString("");
	m_cbGraphShape.AddString("");
	m_cbGraphShape.AddString("");
	m_cbGraphShape.AddString("");
	for (i= -1 ;i < m_cbGraphShape.GetCount();i++) 
		m_cbGraphShape.SetItemHeight(i, 14);
}

void CTabIndex::OnBtnClr(CColorButton &rColorButton)
{
	COLORREF crColor = rColorButton.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		rColorButton.SetColor(color.GetColor());				
		rColorButton.Invalidate();
	}
}

void CTabIndex::OnBtnClrBull() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrBull);
}

void CTabIndex::OnBtnCltBear() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrBear);
}


LRESULT CTabIndex::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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

void CTabIndex::LoadInfo()
{
	CISTControllerD* pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();
	char szBuffer[256];
	long lType = 0;
	long lShape = 0;
	long lThick = 0;
	COLORREF clrIndex = RGB(0,0,0);
	COLORREF clrBull = RGB(0,0,0);
	COLORREF clrBear = RGB(0,0,0);
	CIDataItem *pDataItem = NULL;
	DWORD dSizeRead = 0;
	CString strKeyName;
	CIndexData indexData;
	CString strGraphName;
	BOOL bUseBull = FALSE;
	BOOL bUseBear = FALSE;
	BOOL bIsFirst = TRUE;
	BOOL bHasData = FALSE;
	long lPosOfList = 0;

	if(pSTControllerCfg)
	{
		CString strComment = pSTControllerCfg->GetDeclaredInnerFuncInfo();
		CString strItem;
		int nShifter = 0;
		while(1)
		{
			strItem = Parser(strComment,&nShifter);
			if(strItem.GetLength())
			{
				if(strItem==USEBULL)			bUseBull = TRUE;
				if(strItem==USEBEAR)			bUseBear = TRUE;
			}
			else
			{
				break;
			}			
		}
	}
	else
	{
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,USEBULL,"",szBuffer,sizeof(szBuffer),2);
		bUseBull = atol(szBuffer);
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,USEBEAR,"",szBuffer,sizeof(szBuffer),2);
		bUseBear = atol(szBuffer);
	}

	for(int nPos=0;nPos<MAXCNTOFPLOTHOLDER;nPos++)
	{
		if(pSTControllerCfg)
		{
			pDataItem = pSTControllerCfg->GetPlotHolderDataItem(nPos);
			if(!pDataItem)	continue;
			strGraphName = pDataItem->GetDataItemName();
		}
		else
		{
			// Name
			strKeyName.Format("%s%d",INDEXNAME,nPos);
			dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer),2);
			strGraphName = szBuffer;
			if(strGraphName=="")
			{
				continue;
			}
		}

		// type
		strKeyName.Format("%s%d",INDEXTYPE,nPos);
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lType = INDEXTYPEDEFAULT;	
		}
		else
		{
			lType = atol(szBuffer);
		}	
		
		// color
		strKeyName.Format("%s%d",INDEXCOLOR,nPos);
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrIndex = INDEXCOLORDEFAULT;	
		}
		else
		{
			clrIndex = atol(szBuffer);
		}		

		// Shape
		strKeyName.Format("%s%d",INDEXSHAPE,nPos);
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lShape = INDEXSHAPEDEFAULT;	
		}
		else
		{
			lShape = atol(szBuffer);
		}		

		// Thick
		strKeyName.Format("%s%d",INDEXTHICK,nPos);
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lThick = INDEXTHICKDEFAULT;	
		}
		else
		{
			lThick = atol(szBuffer);
		}		

		m_listGraph.InsertItem(lPosOfList,"");
		m_listGraph.SetItemText(lPosOfList,0,strGraphName);
		indexData.m_lType = lType;
		indexData.m_clrIndex = clrIndex;
		indexData.m_lShape = lShape;
		indexData.m_lThick = lThick;	
		indexData.m_nPos = nPos;
		m_arrayIndexData.SetAtGrow(lPosOfList,indexData);

		if(bIsFirst)
		{
			m_listGraph.SetCurSel(lPosOfList);
			bIsFirst = FALSE;
			bHasData = TRUE;
		}

		lPosOfList++;		
	}
	if(!bHasData)
	{
		m_btnGraphClr.EnableWindow(FALSE);
		m_cbGraphShape.EnableWindow(FALSE);
		m_cbGraphThick.EnableWindow(FALSE);
		m_cbGraphType.EnableWindow(FALSE);
	}


	if(bUseBull)
	{
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,COLORBULL,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrBull = INDEXBULLDEFAULT;	
		}
		else
		{
			clrBull = atol(szBuffer);
		}
		m_btnClrBull.SetColor(clrBull);
	}
	else
	{
		m_btnClrBull.SetColor(DISABLEDEFAULT);
		m_btnClrBull.EnableWindow(FALSE);
	}

	if(bUseBear)
	{
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(INDEX,COLORBEAR,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrBear = INDEXBEARDEFAULT;	
		}
		else
		{
			clrBear = atol(szBuffer);
		}
		m_btnClrBear.SetColor(clrBear);
	}
	else
	{
		m_btnClrBear.SetColor(DISABLEDEFAULT);
		m_btnClrBear.EnableWindow(FALSE);
	}
}

BOOL CTabIndex::SaveInfo()
{
	int nUseType = m_chkSaveAsDefault.GetCheck()==TRUE? 3 : 2;
	COLORREF clrIndex = RGB(0,0,0);
	DWORD dSizeRead = 0;
	CString strKeyName;
	CString strKeyData;
	CIndexData indexData;
	CString strItemText;
	int nIndex = -1;
	int nItemCount = m_listGraph.GetItemCount();
	for(int nPos=0;nPos<nItemCount;nPos++)
	{
		indexData = m_arrayIndexData.GetAt(nPos);
		// type
		strKeyName.Format("%s%d",INDEXTYPE,indexData.m_nPos);
		strKeyData.Format("%d",indexData.m_lType);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,strKeyData,nUseType);
		
		// color
		strKeyName.Format("%s%d",INDEXCOLOR,indexData.m_nPos);
		strKeyData.Format("%d",indexData.m_clrIndex);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,strKeyData,nUseType);

		// Shape
		strKeyName.Format("%s%d",INDEXSHAPE,indexData.m_nPos);
		strKeyData.Format("%d",indexData.m_lShape);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,strKeyData,nUseType);

		// Thick
		strKeyName.Format("%s%d",INDEXTHICK,indexData.m_nPos);
		strKeyData.Format("%d",indexData.m_lThick);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,strKeyData,nUseType);
	}	

	strKeyData.Format("%d",m_btnClrBull.GetColor());
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,COLORBULL,strKeyData,nUseType);
	strKeyData.Format("%d",m_btnClrBear.GetColor());
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,COLORBEAR,strKeyData,nUseType);
	return TRUE;
}

void CTabIndex::RemoveInfo()
{
	int nUseType = 2; // Remove In STFile
	CString strKeyName;
	for(int nPos=0;nPos<MAXCNTOFPLOTHOLDER;nPos++)
	{
		// type
		strKeyName.Format("%s%d",INDEXTYPE,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,"",nUseType);
		
		// color
		strKeyName.Format("%s%d",INDEXCOLOR,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,"",nUseType);

		// Shape
		strKeyName.Format("%s%d",INDEXSHAPE,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,"",nUseType);

		// Thick
		strKeyName.Format("%s%d",INDEXTHICK,nPos);
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,strKeyName,"",nUseType);
	}	

	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,COLORBULL,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(INDEX,COLORBEAR,"",nUseType);
}

void CTabIndex::InitColumn()
{
	int colwidthsType0[6] = { 140};	// sixty-fourths

	TCHAR *	lpszHeadersType0[] = {
							  _T("ÁöÇ¥¸í"),
							  NULL };
	m_listGraph.SetExtendedStyle(LVS_EX_FULLROWSELECT);

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
		m_listGraph.InsertColumn(i, &lvcolumn);
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
		m_listGraph.SetColumn(i, &lvcolumn);
	}
	m_listGraph.SetHilightColor(RGB(0,0,0),RGB(208,221,240));
}

void CTabIndex::OnItemchangedListGraph(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMListView->uNewState==3)
	{
		CIndexData indexData = m_arrayIndexData.GetAt(pNMListView->iItem);
		m_listGraph.SetCurSel(pNMListView->iItem);
		m_cbGraphType.SetCurSel(indexData.m_lType);
		m_cbGraphThick.SetCurSel(indexData.m_lThick);
		m_cbGraphShape.SetCurSel(indexData.m_lShape);
		m_btnGraphClr.SetColor(indexData.m_clrIndex);
		m_btnGraphClr.Invalidate();
		if(indexData.m_lType!=0)
		{
			m_stThick.ShowWindow(SW_HIDE);
			m_stShape.ShowWindow(SW_HIDE);
			m_cbGraphThick.ShowWindow(SW_HIDE);
			m_cbGraphShape.ShowWindow(SW_HIDE);
		}
		else
		{
			m_stShape.ShowWindow(SW_SHOW);
			m_cbGraphShape.ShowWindow(SW_SHOW);
			
			if(indexData.m_lShape!=0)
			{
				m_stThick.ShowWindow(SW_HIDE);
				m_cbGraphThick.ShowWindow(SW_HIDE);
			}
			else
			{
				m_stThick.ShowWindow(SW_SHOW);
				m_cbGraphThick.ShowWindow(SW_SHOW);				
			}			
		}
	}
	
	*pResult = 0;
}

void CTabIndex::OnSelchangeCbGraphShape() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_listGraph.GetCurSel();
	CIndexData indexData = m_arrayIndexData.GetAt(nCurSel);	
	indexData.m_lShape = m_cbGraphShape.GetCurSel();
	m_arrayIndexData.SetAtGrow(nCurSel,indexData);
	if(indexData.m_lShape!=0)
	{
		m_stThick.ShowWindow(SW_HIDE);
		m_cbGraphThick.ShowWindow(SW_HIDE);
	}
	else
	{
		m_stThick.ShowWindow(SW_SHOW);
		m_cbGraphThick.ShowWindow(SW_SHOW);
	}
}

void CTabIndex::OnSelchangeCbGraphThick() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_listGraph.GetCurSel();
	CIndexData indexData = m_arrayIndexData.GetAt(nCurSel);	
	indexData.m_lThick = m_cbGraphThick.GetCurSel();
	m_arrayIndexData.SetAtGrow(nCurSel,indexData);	
}

void CTabIndex::OnSelchangeCbGraphType() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_listGraph.GetCurSel();
	CIndexData indexData = m_arrayIndexData.GetAt(nCurSel);	
	indexData.m_lType = m_cbGraphType.GetCurSel();	
	m_arrayIndexData.SetAtGrow(nCurSel,indexData);
	if(indexData.m_lType!=0)
	{
		m_stThick.ShowWindow(SW_HIDE);
		m_stShape.ShowWindow(SW_HIDE);
		m_cbGraphThick.ShowWindow(SW_HIDE);
		m_cbGraphShape.ShowWindow(SW_HIDE);
	}
	else
	{
		m_stThick.ShowWindow(SW_SHOW);
		m_stShape.ShowWindow(SW_SHOW);
		m_cbGraphThick.ShowWindow(SW_SHOW);
		m_cbGraphShape.ShowWindow(SW_SHOW);
	}

}

void CTabIndex::OnBtnGraphClr() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnGraphClr);
	int nCurSel = m_listGraph.GetCurSel();
	CIndexData indexData = m_arrayIndexData.GetAt(nCurSel);	
	indexData.m_clrIndex = m_btnGraphClr.GetColor();
	m_arrayIndexData.SetAtGrow(nCurSel,indexData);
}

HBRUSH CTabIndex::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
