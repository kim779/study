// TabCodeSearchELW.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "TabCodeSearchELW.h"
#include "./define/ChartMsgDef.h"
#include "DataInListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchELW dialog


CTabCodeSearchELW::CTabCodeSearchELW(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabCodeSearchELW::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabCodeSearchELW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInsertingInListCode = FALSE;
}


void CTabCodeSearchELW::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabCodeSearchELW)
	DDX_Control(pDX, IDC_LISTCODE, m_listCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabCodeSearchELW, CTabDlg)
	//{{AFX_MSG_MAP(CTabCodeSearchELW)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LISTCODE, OnClickListcode)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCODE, OnDblclkListcode)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCodeSearchELW message handlers

BOOL CTabCodeSearchELW::OnInitDialog() 
{
	CTabDlg::OnInitDialog();

	m_listCode.DeleteAllItems();
	InitColumns();
	m_listCode.SetHilightColor(RGB(0,0,0), RGB(180,200,240));
	InsertData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabCodeSearchELW::InitColumns()
{
	m_listCode.SetExtendedStyle(LVS_EX_FULLROWSELECT);// | LVS_EX_GRIDLINES);
	m_listCode.SetRowHeight(17);

	int colwidths[3] = { 141,50 };	// sixty-fourths
	TCHAR *	lpszHeaders[] = { _T("종목명"),	_T("코드"),	NULL };

	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// add columns
	for (int i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = i>1 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.pszText = lpszHeaders[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx = colwidths[i];
		m_listCode.InsertColumn(i, &lvcolumn);
	}
	memset(&lvcolumn, 0, sizeof(lvcolumn));

	// set the format again - must do this twice or first column does not get set
	for (i = 0; ; i++)
	{
		if (lpszHeaders[i] == NULL)
			break;
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM;
		lvcolumn.fmt = i>0 ? LVCFMT_RIGHT:LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		m_listCode.SetColumn(i, &lvcolumn);
	}	
}

void CTabCodeSearchELW::RecalcLayout()
{	
	RecalcLayoutCtrl(m_listCode);
}


// ELW 이름순으로 정렬
int ELWItemNameFunc(const void* p1,const void *p2)
{
	CELWItem *q1 = *(CELWItem**)p1;
	CELWItem *q2 = *(CELWItem**)p2;

	if(q1->m_strName.CompareNoCase(q2->m_strName) < 0)
		return -1;
	else if(q1->m_strName.CompareNoCase(q2->m_strName) > 0)
		return 1;
	return 0;
}

BOOL CTabCodeSearchELW::InsertData()
{
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	IMasterDataManager* pDataManager;
	if(pGateMng)
		pDataManager = (IMasterDataManager*)pGateMng->GetInterface(5);

	CStringArray* pastrCode;	// ELW 종목 코드 
	CStringArray* pastrName;	// ELW 종목 명 
	CStringArray* pastrEngName;	// ELW 종목 명 

	pastrCode = new CStringArray;
	pastrName = new CStringArray;
	pastrEngName = new CStringArray;

//@유진삭제
//	pDataManager->GetELWMst(pastrCode, pastrName, pastrEngName);
//@유진삭제

	int nCount = pastrCode->GetSize();
	CString strCode, strName;
	if(nCount)
	{
		CELWItem **pElwData = new CELWItem * [nCount];
		for(int i=0; i<pastrCode->GetSize(); i++)
		{
			pElwData[i] = new CELWItem;
			strCode = pastrCode->GetAt(i);
			strName = pastrName->GetAt(i);
			pElwData[i]->SetItem(strCode, strName);
		}

		::qsort(pElwData, nCount, sizeof(void*), ELWItemNameFunc);

		int nItemNum = m_listCode.GetItemCount();

		m_bInsertingInListCode = TRUE;
		for(int nIndex=0; nIndex < nCount; nIndex++)
		{
			CString strCode, strName, strTemp;
			strCode = pElwData[nIndex]->m_strCode; 	
			strCode.TrimLeft();		strCode.TrimRight();
			strTemp = pElwData[nIndex]->m_strName;
			strName = strTemp.Left(20);

			m_listCode.InsertItem(nItemNum,(LPTSTR)(LPCTSTR)strName);
			CDataInListCtrl *pData = NULL;
			pData = (CDataInListCtrl *)m_listCode.GetItemData(nItemNum);
			if(!pData)
			{
				pData = new CDataInListCtrl;
				m_listCode.SetItemData(nItemNum,(DWORD)pData);
			}
			pData->m_strJongCode = strCode;
			pData->m_strCodeName = strName;

			nItemNum++;		
			delete pElwData[nIndex];
		}

		delete pElwData;
		m_listCode.SetItemCountEx(nItemNum);
		m_listCode.Invalidate();
		m_bInsertingInListCode = FALSE;
	}

	delete pastrCode;
	delete pastrName;
	delete pastrEngName;

	return TRUE;
}

CRect CTabCodeSearchELW::GetRectOfCtrl(INT nID,BOOL bInit/*=FALSE*/)
{
	CRect rect;
	GetClientRect(rect);
	int nWidth = rect.Width();
	rect.DeflateRect(GAP_TABDLG,GAP_TABDLG,GAP_TABDLG,GAP_TABDLG);
	switch(nID)
	{
	case IDC_LISTCODE:
		rect.top = rect.top + GAP_TABDLG;
		rect.bottom = rect.bottom - GAP_TABDLG;
		break;		
	default:
		break;
	}
	return rect;
}

void CTabCodeSearchELW::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	RecalcLayout();		
}

BOOL CTabCodeSearchELW::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* phdr = (NMHDR*)lParam;
	if(phdr->hwndFrom == m_listCode.m_hWnd)
	{
		switch(phdr->code)		{
		case LVN_GETDISPINFO:
			{
				NMLVDISPINFO* pLvdi;
				pLvdi = (NMLVDISPINFO*)lParam;
				GetDispInfo(&pLvdi->item);
			}
			if(pResult != NULL) *pResult = 0;
			break;
//		case LVN_ODCACHEHINT:
//			if(pResult != NULL) *pResult = 0;
//			break;
//		case LVN_ODFINDITEM:
//			if(pResult != NULL) *pResult = 0;
//			break;
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CTabCodeSearchELW::GetDispInfo(LVITEM* pItem)
{
	CDataInListCtrl* pData = (CDataInListCtrl*)m_listCode.GetItemData(pItem->iItem);

	if(pData == NULL)
		return;

	if(pItem->mask & (LVIF_TEXT | LVIF_PARAM))	
	{
		switch(pItem->iSubItem) 
		{
			case 0:
				lstrcpy(pItem->pszText, pData->m_strCodeName);
				break;
			case 1:
				lstrcpy(pItem->pszText, pData->m_strJongCode);
				break;
		}
	}
}

void CTabCodeSearchELW::OnClickListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
	if(!bInsertMode&&!m_bInsertingInListCode)
	{
		int nCurSel = m_listCode.GetCurSel();

		CDataInListCtrl *pData = NULL;
		pData = (CDataInListCtrl *)m_listCode.GetItemData(nCurSel);

		CString strCodeSelected = pData->m_strJongCode;
		if(!strCodeSelected.IsEmpty())
		{
			STINPUTCODEDATA stInputCodeData;
			stInputCodeData.m_strCode = strCodeSelected;
			m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
		}
	}		

	*pResult = 0;
}

void CTabCodeSearchELW::OnDestroy() 
{
	CTabDlg::OnDestroy();
	
	CDataInListCtrl *pData = NULL;
	int nCnt = m_listCode.GetItemCount();
	for(int i=0;i<nCnt;i++)
	{
		pData = (CDataInListCtrl *)m_listCode.GetItemData(i);
		if(pData)
		{
			delete pData;
			pData = NULL;
		}
	}
	m_listCode.DeleteAllItems();
	
}

void CTabCodeSearchELW::OnDblclkListcode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	BOOL bInsertMode = m_pChartMng->SendMessage(UMSG_GETINC);
	if(bInsertMode)
	{
		int nCurSel = m_listCode.GetCurSel();

		CDataInListCtrl *pData = NULL;
		pData = (CDataInListCtrl *)m_listCode.GetItemData(nCurSel);

		CString strCodeSelected = pData->m_strJongCode;
		if(!strCodeSelected.IsEmpty())
		{
			STINPUTCODEDATA stInputCodeData;
			stInputCodeData.m_strCode = strCodeSelected;
			m_pChartMng->SendMessage(UMSG_INPUTCODETOCHARTMNG,NULL,(LPARAM)&stInputCodeData);
		}
	}
	//	}		

	*pResult = 0;
}