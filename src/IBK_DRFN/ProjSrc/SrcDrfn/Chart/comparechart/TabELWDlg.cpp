// TabELWDlg.cpp : implementation file
//

#include "stdafx.h"
#include "comparechart.h"
#include "TabELWDlg.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabELWDlg dialog


CTabELWDlg::CTabELWDlg(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabELWDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabELWDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_pDBMgr = NULL;
	m_pTreeListMng = NULL;
	m_dwTreeKey = 0;
	//2007.04.30 by LYH 리사이즈시 코드콤보 원래 위치 유지 위해 추가
	m_rectCmb = CRect(0,0,0,0);
}


void CTabELWDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabELWDlg)
	DDX_Control(pDX, IDC_COMBO_SORTWAY, m_cmbSort);
	DDX_Control(pDX, IDC_LIST_STOCK, m_listStockCode);
	DDX_Control(pDX, IDC_TREESEARCH, m_treeSearch);	
	DDX_Control(pDX, IDC_BTN_ADD, m_ctrlBtnAdd);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabELWDlg, CTabDlg)
	//{{AFX_MSG_MAP(CTabELWDlg)
//	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
//	ON_BN_CLICKED(IDC_RADIO_KOSPI, OnRadioKospi)
//	ON_BN_CLICKED(IDC_RADIO_KOSDAQ, OnRadioKosdaq)
	//ON_EN_CHANGE(IDC_EDIT_STOCK, OnChangeEditStock)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREESEARCH, OnSelchangedTreesearch)
// 	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabELWDlg message handlers

//void CTabELWDlg::OnRadioAll() 
//{	
//	m_nOption = ALL;
//	m_listStockCode.LoadCodeList(m_nOption);
//}
//
//void CTabELWDlg::OnRadioKospi() 
//{
//	// TODO: Add your control notification handler code here
//	m_nOption = KOSPI;
//	m_listStockCode.LoadCodeList(m_nOption);
//}
//
//void CTabELWDlg::OnRadioKosdaq() 
//{
//	// TODO: Add your control notification handler code here
//	m_nOption = KOSDAQ;	
//	m_listStockCode.LoadCodeList(m_nOption);
//}

BOOL CTabELWDlg::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	//m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);
	m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);

	if(m_ctrlCodeInput.GetSafeHwnd() != NULL)
	{
		CRect rectComp, ctlRect;
		GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);

		m_ctrlCodeInput.GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		m_ctrlCodeInput.SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
		//2007.04.30 by LYH 리사이즈시 코드콤보 원래 위치 유지 위해 추가
		m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
	}

	m_ctrlBtnAdd.FP_vSetImage( m_pIL_W31H20, 12, 14, TRUE, "추가");

	m_cmbSort.AddString("종목명순");
	m_cmbSort.AddString("종목코드순");

	m_cmbSort.SetCurSel(0);

//	m_listStockCode.SetDBMgr(m_pDBMgr);
	m_listStockCode.SetListDataType(STOCK);
	m_listStockCode.Init_ListCtrl();
	m_listStockCode.SetRowHeight(15);

	InitTreeSearch();
	//m_listStockCode.LoadCodeList(m_nOption);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void CTabELWDlg::OnChangeEditStock() 
//{
//	CString strCode;
//	m_editCode.GetWindowText(strCode);	
//	m_listStockCode.ChangeCode(strCode);
//	strCode.ReleaseBuffer();
//}
LRESULT CTabELWDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

//	if(message == RMSG_EVENT_CODEINPUT)
//	{
//		LPST_CTRLEVENT stEvent = (LPST_CTRLEVENT)lParam;
//		if(strcmp("LengthFull", stEvent->lpszEventName) == 0 || strcmp("SelChanged", stEvent->lpszEventName) == 0)
//		{
//			CString strCode = m_ctrlCodeInput.GetDataText();
//			CString strName = ((CStdDialog*)GetParent()->GetParent()->GetParent())->GetCodeNameAndGubun(0, strCode);
//			CString strData;
//			strData.Format("%s&%s", strName, strCode);
//			if(strData != m_strCodeNName)
//			{
//				m_strCodeNName = strData;
//				GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_strCodeNName));
//			}
//		}
//	}
	switch(message)
	{
		case UMSG_ADDITEM_BTNCLICK:
			if(!m_listStockCode.m_strCodeNName.GetLength()) return 0l;
			GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_listStockCode.m_strCodeNName));			
			break;
		//2007.04.30 by LYH 리사이즈시 코드콤보 원래 위치 유지 위해 추가
		case USMG_PLEASE_RESIZE:
			if(m_ctrlCodeInput.GetSafeHwnd())
			{
				m_ctrlCodeInput.SetWindowPos(&CWnd::wndTop, 
					m_rectCmb.left, m_rectCmb.top, m_rectCmb.Width(), m_rectCmb.Height(), SWP_NOZORDER);		
			}
			break;			
	}
	return CTabDlg::WindowProc(message, wParam, lParam);
}

BOOL CTabELWDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(pMsg->message == WM_KEYDOWN)
//	{
//		if(pMsg->wParam == VK_RETURN	|| 
//		   pMsg->wParam == VK_ESCAPE	) 
//		{
//			return TRUE;
//		}
//	}
//	return CTabDlg::PreTranslateMessage(pMsg);

	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CTabDlg::PreTranslateMessage(pMsg);
}

void CTabELWDlg::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	if(m_treeSearch.GetSafeHwnd())
	{
		CRect rect, rect2;
		GetClientRect(rect);

		m_treeSearch.GetWindowRect(rect2);
		ScreenToClient(rect2);
		rect2.right = rect.right - 2;
		m_treeSearch.MoveWindow(rect2);
	}

	if(m_listStockCode.GetSafeHwnd())
	{
		CRect rect, rect2;
		GetClientRect(rect);

		m_listStockCode.GetWindowRect(rect2);
		ScreenToClient(rect2);
		rect2.right = rect.right - 2;
		rect2.bottom = rect.bottom; 
		m_listStockCode.MoveWindow(rect2);
	}	
}

void CTabELWDlg::InitTreeSearch()
{
	m_pTreeListMng = (ITreeListManager2*)AfxGetPctrInterface(UUID_ITreeListManager2);

	CInputCodeListEx inData;
	inData.m_pTreeCtrl = &m_treeSearch;
	inData.m_pListCtrl = NULL;
	inData.m_szSection = "ELW";

	if(m_pTreeListMng)
	{
		m_dwTreeKey = m_pTreeListMng->LoadTreeList(&inData, "ELW");
		m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)GetParent()->GetParent()->GetParent())->m_strScreenNo);
	}
}

void CTabELWDlg::OnDestroy() 
{
	fnRemovelistStockCode();
	if(m_pTreeListMng) m_pTreeListMng->UnLoadTreeList(m_dwTreeKey);

	CTabDlg::OnDestroy();
}

void CTabELWDlg::fnRemovelistStockCode()
{
	for( int i = 0; i < m_listStockCode.m_AllArray.GetSize(); i++)
		delete m_listStockCode.m_AllArray.GetAt(i);
	m_listStockCode.m_AllArray.RemoveAll();
}

//@유진 080421월 기존엔 LIST_CCodeTypeB 타입이었으나 LIST_CCodeTypeA 로 변경함.
void CTabELWDlg::CTLMCallback::ReceiveData(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData)
{
	TRACE("CTabELWDlg::CTLMCallback::ReceiveData\n");

	METHOD_PROLOGUE(CTabELWDlg, TLMCallback);
	// pThis->
	LIST_CCodeTypeA *pListTypeA = (LIST_CCodeTypeA*)pData;
	if(pListTypeA)
	{
		pThis->m_listStockCode.DeleteAllItems();
		pThis->fnRemovelistStockCode();

		long	lUpLimit = 0;
		long	lUp = 0;
		long	lSteady = 0;
		long	lDown = 0;
		long	lDownLimit = 0;
		long	lIndexCnt = 0;
		double dDaebiRateTotal = 0;

		//CString strVolumeLimitation;
		//double dVolumeLimitation = atof(strVolumeLimitation)*1000;
		//pThis->m_bInsertingInListCode = TRUE;
		for(POSITION pos=pListTypeA->GetHeadPosition(); pos; )
		{
			CCodeTypeA TypeA = pListTypeA->GetNext(pos);

			
			MASTERCODE* pData = new MASTERCODE;
			pData->strCode = TypeA.m_szcode;
			pData->strKor = TypeA.m_szhname;
			pData->strEng = _T("");	
			pThis->m_listStockCode.m_AllArray.Add(pData);
		}
		pThis->m_listStockCode.SetItemCount(pListTypeA->GetCount());		


//		pThis->m_bInsertingInListCode = FALSE;
		//}
	}
}

void CTabELWDlg::OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_treeSearch.GetSelectedItem();// .m_hSelTreeItem;
	if(hItem == NULL)
		return;

	CItemLongData* pLData = (CItemLongData*)m_treeSearch.GetItemData(hItem);
	if(pLData)
	{
		TRACE("Title[%s], Sub[%s], Method[%s]\n", pLData->m_szTitle, pLData->m_szSub, pLData->m_szMethod);
		long dwReserved=0;
		if(m_pTreeListMng)
		{
			CStdDialog* pMultiChartWnd = (CStdDialog*)GetParent()->GetParent()->GetParent();
			m_pTreeListMng->SetSendKey(((CStdDialog*)pMultiChartWnd)->m_hPlatform, ((CStdDialog*)pMultiChartWnd)->GetParent()->GetParent()->GetSafeHwnd());
			m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);
		}
	}

	*pResult = 0;
}

void CTabELWDlg::OnBtnAdd()
{
	CString strCode = m_ctrlCodeInput.GetDataText();
	CString strName = ((CStdDialog*)GetParent()->GetParent()->GetParent())->GetItemNameFromItemCode(strCode);
	strCode.TrimLeft();
	strCode.TrimRight();
	if(strCode.IsEmpty() || strName.IsEmpty()) return;

	CString strData;
	strData.Format("%s|%s", strName, strCode);
	if(strData != m_strCodeNName)
	{
		m_strCodeNName = strData;
		GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_strCodeNName));
	}
}
