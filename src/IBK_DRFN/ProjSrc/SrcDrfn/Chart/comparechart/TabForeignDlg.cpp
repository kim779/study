// TabStockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "comparechart.h"
#include "TabForeignDlg.h"
#include "StdDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabForeignDlg dialog


CTabForeignDlg::CTabForeignDlg(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabForeignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabForeignDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//m_pDBMgr = NULL;
	m_pTreeListMng = NULL;
	m_dwTreeKey = 0;
	//2007.04.30 by LYH 리사이즈시 코드콤보 원래 위치 유지 위해 추가
	m_rectCmb = CRect(0,0,0,0);

	m_pIGwanMonitorManager = NULL;
	m_dwIGwanMonitorManager = NULL;
}


void CTabForeignDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabForeignDlg)
	DDX_Control(pDX, IDC_COMBO_SORTWAY, m_cmbSort);
	DDX_Control(pDX, IDC_LIST_STOCK, m_listStockCode);
	DDX_Control(pDX, IDC_TREESEARCH, m_treeSearch);	
	DDX_Control(pDX, IDC_BTN_ADD, m_ctrlBtnAdd);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabForeignDlg, CTabDlg)
	//{{AFX_MSG_MAP(CTabForeignDlg)
//	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
//	ON_BN_CLICKED(IDC_RADIO_KOSPI, OnRadioKospi)
//	ON_BN_CLICKED(IDC_RADIO_KOSDAQ, OnRadioKosdaq)
	//ON_EN_CHANGE(IDC_EDIT_STOCK, OnChangeEditStock)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREESEARCH, OnSelchangedTreesearch)
// 	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_CBN_SELCHANGE(IDC_CTRL_CODEINPUT, OnSelchangeCodeInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabForeignDlg message handlers

//void CTabForeignDlg::OnRadioAll() 
//{	
//	m_nOption = ALL;
//	m_listStockCode.LoadCodeList(m_nOption);
//}
//
//void CTabForeignDlg::OnRadioKospi() 
//{
//	// TODO: Add your control notification handler code here
//	m_nOption = KOSPI;
//	m_listStockCode.LoadCodeList(m_nOption);
//}
//
//void CTabForeignDlg::OnRadioKosdaq() 
//{
//	// TODO: Add your control notification handler code here
//	m_nOption = KOSDAQ;	
//	m_listStockCode.LoadCodeList(m_nOption);
//}

BOOL CTabForeignDlg::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	m_pCodeInput = new CCodeCtrl(GetParent()->GetParent()->GetParent()->GetParent());
	CStdDialog* pStdDialog = ((CStdDialog*)m_pCompareChartWnd);
	m_pCodeInput->SetRootPath(pStdDialog->m_strRootPath);
	m_pCodeInput->UseCodeDlg(TRUE);
	
//	HWND hCommHandle = NULL;
//	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
//	int nCnt = pPtSvrMng->GetParentHandle(0, hCommHandle);

	//m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);
//@Solomon	m_ctrlCodeInput.InitCodeInput(this, IDC_CTRL_CODEINPUT, CODEINPUT_PROP_STOCK);

//@Solomon	if(m_ctrlCodeInput.GetSafeHwnd() != NULL)
	{
		CRect rectComp, ctlRect;
		GetDlgItem(IDC_CTRL_CODEINPUT)->GetWindowRect(ctlRect);
		ScreenToClient(ctlRect);

		CWnd* pwndWizard = CWnd::FromHandle(pStdDialog->m_hPlatform);
		m_pCodeInput->CreateCodeCtrl(this, pwndWizard, GetParent()->GetParent()->GetParent(),ctlRect, IDC_CTRL_CODEINPUT);
		m_pCodeInput->SetColor(RGB(0,0,0), RGB(255,255,255));
		m_pCodeInput->m_bIsComareChart = TRUE;
		m_pCodeInput->SetUnit("해외");
		
		//@Solomonm_ctrlCodeInput.GetWindowRect(rectComp);
		m_pCodeInput->GetWindowRect(rectComp);
		ScreenToClient(rectComp);
		//@Solomonm_ctrlCodeInput.SetWindowPos(NULL, ctlRect.left, ctlRect.top, rectComp.Width(), 20, SWP_NOZORDER);
		//2007.04.30 by LYH 리사이즈시 코드콤보 원래 위치 유지 위해 추가
		m_rectCmb = CRect(ctlRect.left, ctlRect.top, ctlRect.left + rectComp.Width(), ctlRect.top + 20);
	}

	m_ctrlBtnAdd.FP_vSetImage( m_pIL_W31H20, 12, 14, TRUE, "추가");

	m_cmbSort.AddString("종목명순");
	m_cmbSort.AddString("종목코드순");

	m_cmbSort.SetCurSel(0);

	m_nOption = ALL;
	
//	m_listStockCode.SetDBMgr(m_pDBMgr);
	m_listStockCode.SetListDataType(STOCK);
	m_listStockCode.Init_ListCtrl();
	m_listStockCode.SetRowHeight(15);

	InitTreeSearch();
	//m_listStockCode.LoadCodeList(m_nOption);

	AdviseGwansim();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//void CTabForeignDlg::OnChangeEditStock() 
//{
//	CString strCode;
//	m_editCode.GetWindowText(strCode);	
//	m_listStockCode.ChangeCode(strCode);
//	strCode.ReleaseBuffer();
//}
LRESULT CTabForeignDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

//	if(message == RMSG_EVENT_CODEINPUT)
//	{
//		LPST_CTRLEVENT stEvent = (LPST_CTRLEVENT)lParam;
//		if(strcmp("LengthFull", stEvent->lpszEventName) == 0 || strcmp("SelChanged", stEvent->lpszEventName) == 0)
//		{
//			CString strCode = m_ctrlCodeInput.GetDataText();
//			CString strName = ((CStdDialog*)m_pCompareChartWnd)->GetCodeNameAndGubun(0, strCode);
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
//@solomon			if(m_ctrlCodeInput.GetSafeHwnd())
//@solomon			{
//@solomon				m_ctrlCodeInput.SetWindowPos(&CWnd::wndTop, 
//@solomon					m_rectCmb.left, m_rectCmb.top, m_rectCmb.Width(), m_rectCmb.Height(), SWP_NOZORDER);		
//@solomon			}
			if(m_pCodeInput && m_pCodeInput->GetSafeHwnd())
			{
				m_pCodeInput->SetWindowPos(&CWnd::wndTop, 
					m_rectCmb.left, m_rectCmb.top, m_rectCmb.Width(), m_rectCmb.Height(), SWP_NOZORDER);		
			}
		case UMSG_ADDCODE : //KHD : 종목코드 입력시 적용 되도록 수정 
			SetCodeFromOutSide(m_pCodeInput->GetCode());
			break;
			break;			
	}
	return CTabDlg::WindowProc(message, wParam, lParam);
}

BOOL CTabForeignDlg::PreTranslateMessage(MSG* pMsg) 
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

void CTabForeignDlg::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	if(m_treeSearch.GetSafeHwnd())
	{
		CRect rect, rect2;
		GetClientRect(rect);

		m_treeSearch.GetWindowRect(rect2);
		ScreenToClient(rect2);
		rect2.left = rect.left + 1;
		rect2.right = rect.right;
		m_treeSearch.MoveWindow(rect2);
	}

	if(m_listStockCode.GetSafeHwnd())
	{
		CRect rect, rect2;
		GetClientRect(rect);

		m_listStockCode.GetWindowRect(rect2);
		ScreenToClient(rect2);
		rect2.left = rect.left + 1;
		rect2.right = rect.right;
		rect2.bottom = rect.bottom; 
		m_listStockCode.MoveWindow(rect2);
	}	
}

void CTabForeignDlg::InitTreeSearch()
{
	m_pTreeListMng = (ITreeListManager2*)AfxGetPctrInterface(UUID_ITreeListManager2);

	CInputCodeListEx inData;
	inData.m_pTreeCtrl = &m_treeSearch;
	inData.m_pListCtrl = NULL;
	inData.m_szSection = "해외";

	if(m_pTreeListMng)
	{
		m_dwTreeKey = m_pTreeListMng->LoadTreeList(&inData, inData.m_szSection);
		m_pTreeListMng->SetBaseData("SCREENCODE", ((CStdDialog*)m_pCompareChartWnd)->m_strScreenNo);
	}
}

void CTabForeignDlg::OnDestroy() 
{
	UnAdviseGwansim();
	fnRemovelistStockCode();
	if(m_pTreeListMng) m_pTreeListMng->UnLoadTreeList(m_dwTreeKey);
	if(m_pCodeInput) delete m_pCodeInput; m_pCodeInput=NULL;

	CTabDlg::OnDestroy();
}

void CTabForeignDlg::fnRemovelistStockCode()
{
	for( int i = 0; i < m_listStockCode.m_AllArray.GetSize(); i++)
		delete m_listStockCode.m_AllArray.GetAt(i);
	m_listStockCode.m_AllArray.RemoveAll();
}

//@유진 080421월 기존엔 LIST_CCodeTypeB 타입이었으나 LIST_CCodeTypeA 로 변경함.
void CTabForeignDlg::CTLMCallback::ReceiveData(int nRtnState, char aDataType, long dwReserved, int nLength, void* pData)
{
	TRACE("CTabForeignDlg::CTLMCallback::ReceiveData\n");

	METHOD_PROLOGUE(CTabForeignDlg, TLMCallback);
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

void CTabForeignDlg::OnSelchangedTreesearch(NMHDR* pNMHDR, LRESULT* pResult) 
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
			m_pTreeListMng->SetSendKey(pMultiChartWnd->m_hPlatform, pMultiChartWnd->GetParent()->GetParent()->GetSafeHwnd());
			m_pTreeListMng->RequestData(pLData, &m_xTLMCallback, 'A', '1', dwReserved);
		}
	}

	*pResult = 0;
}


//HBRUSH CTabForeignDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{
//	HBRUSH hbr = CTabDlg::OnCtlColor(pDC, pWnd, nCtlColor);
//	
//	// TODO: Change any attributes of the DC here
//	switch(nCtlColor) 
//	{
//	case CTLCOLOR_EDIT:
//	case CTLCOLOR_LISTBOX:
//		return hbr;
//		break;
//	default:
//		{
//			pDC->SetBkColor(m_crBk);
//			return m_brBkColor;
//		}		
//		break;
//	}
//	
//	// TODO: Return a different brush if the default is not desired
//	return hbr;
//}

void CTabForeignDlg::SetCodeFromOutSide(LPCSTR szCode)
{
	if(strlen(szCode)>0)
	{
		m_pCodeInput->SetCode(szCode);
		m_pCodeInput->SetUnit("해외");
		OnBtnAdd();
	}
}


void CTabForeignDlg::OnBtnAdd()
{
//@Solomon	CString strCode = m_ctrlCodeInput.GetDataText();
//@Solomon	CString strName = ((CStdDialog*)m_pCompareChartWnd)->GetItemNameFromItemCode(strCode);
	CString strCode = m_pCodeInput->GetCode();
	CString strName = ((CStdDialog*)m_pCompareChartWnd)->GetItemNameFromItemCode(strCode);

	strCode.TrimLeft();
	strCode.TrimRight();
	if(strCode.IsEmpty() || strName.IsEmpty()) return;

	strName.TrimRight();
	if(strName.GetLength())
	{
		
		CString strData;
		strData.Format("%s|%s", strName, strCode);
		if(strData != m_strCodeNName)
		{
			m_strCodeNName = strData;
			GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_strCodeNName));
		}
	}
}

void CTabForeignDlg::DataMonitorCallbackProc(LPCSTR szKey, long pThisPoint, long dwKey, long dwSubKey, long dwNotifyData)
{
	CTabForeignDlg* pThis = (CTabForeignDlg*)pThisPoint;
	CString strKey = szKey;
	
	if(strKey == DataMon_SZGWANSIM) 
	{
		pThis->ReloadGwansim();
	}
}

// 관심그룹 변경정보 등록하고 받기.
//extern int Dll_SetRegKwCombo( CWnd* pComboWnd, int nType);	////[EDIT] 관심종목 콤보를 관리
//	nType:1이면 등록, 0이면 해제
//	WM_USER + 525 LParam이 그룹코드
void CTabForeignDlg::AdviseGwansim()
{
// 	return;//유진에서 관심 싱크작업을 하지 않는다고 해서 막음.
// 	m_pIGwanMonitorManager = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager);
// 	CString szKey = DataMon_SZGWANSIM; 
// 	m_dwIGwanMonitorManager = m_pIGwanMonitorManager->Advise(szKey, (long)this, (long)NULL, DataMonitorCallbackProc);
}

void CTabForeignDlg::UnAdviseGwansim()
{
	return;//유진에서 관심 싱크작업을 하지 않는다고 해서 막음.
	if(m_dwIGwanMonitorManager)	m_pIGwanMonitorManager->UnAdvise(m_dwIGwanMonitorManager);	
}

void CTabForeignDlg::ReloadGwansim()
{
	//유진에서 관심 싱크작업을 하지 않는다고 해서 작업안함.
	TRACE("\nCTabForeignDlg::ReloadGwansim()\n");
}

void CTabForeignDlg::OnSelchangeCodeInput()
{
	OnBtnAdd();
}
