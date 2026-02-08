// VirtualListCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "../comparechart.h"
#include "VirtualListCtrl.h"
//#include "../define/ChartMsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualListCtrl

int g_nSelCol = 1;  //부득이 전역으로 한다.
CVirtualListCtrl::CVirtualListCtrl()
{
	g_nSelCol = m_nFindCol = 1;
	m_nListDataType = STOCK;
	m_strCodeNName = _T("");
	m_strCode	   = _T("");
}

CVirtualListCtrl::~CVirtualListCtrl()
{
}


BEGIN_MESSAGE_MAP(CVirtualListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CVirtualListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
//	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualListCtrl message handlers

void CVirtualListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;	
	LV_ITEM* pItem= &(pDispInfo)->item;

	if(m_AllArray.GetSize() <= 0) 
	{
		*pResult = 0;
		return;
	}
	//Which column?	
	CString text;
	MASTERCODE* pCode = NULL;
	if(pItem->iSubItem == 0)
	{	
		//text = m_arrKor.GetAt(pItem->iItem);		
		pCode = (MASTERCODE*)m_AllArray.GetAt(pItem->iItem);
		if(pCode != NULL)
			text = pCode->strKor;
	}
	else if (pItem->iSubItem == 1)
	{		
		//text = m_arrCode.GetAt(pItem->iItem);
		pCode = (MASTERCODE*)m_AllArray.GetAt(pItem->iItem);
		if(pCode != NULL)
			text = pCode->strCode;
	}

	if(pCode != NULL)
		pItem->pszText = (LPTSTR)(LPCTSTR)text;
	
	//pItem->pszText = (LPTSTR)(LPCTSTR)m_strCodeList.GetAt(m_strCodeList.FindIndex(pItem->iItem));		

	*pResult = 0;
}

void CVirtualListCtrl::OnDestroy() 
{
	CListCtrl::OnDestroy();

//	m_arrCode.RemoveAll();
//	m_arrKor.RemoveAll();
//	m_arrEng.RemoveAll();

	//======================================================	
	DeleteAllItems();
	fnUsingMemoryClear();
	//======================================================
	
}

void CVirtualListCtrl::fnUsingMemoryClear()
{
	for( int i = 0; i < m_AllArray.GetSize(); i++)
		delete m_AllArray.GetAt(i);
	m_AllArray.RemoveAll();	
}

//#define LVS_EX_LABELTIP 0x00004000
//void CVirtualListCtrl::Init_ListCtrl()
//{
//	
//	CRect lRect;
//	GetWindowRect(lRect);
//
//	CString strText;	
//	strText.Format("종목명", 0);
//	//InsertColumn(0,strText,LVCFMT_LEFT, lRect.Width() - 90);		
//	InsertColumn(0,strText,LVCFMT_LEFT, 70);		
//	strText.ReleaseBuffer();
//
//	strText.Format("코드", 0);
//	InsertColumn(1,strText,LVCFMT_LEFT, 55);		
//	strText.ReleaseBuffer();		
//
//	CRect subRect;	
//	//const int LVS_EX_LABELTIP = 0x00004000;
//	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP);		
//}

int Compare(const void *arg1, const void *arg2)
{
	MASTERCODE Data1, Data2;
	Data1 = *(MASTERCODE *)arg1;
    Data2 = *(MASTERCODE *)arg2;

	if(g_nSelCol == 0)
		return (Data1.strKor).CollateNoCase((LPCTSTR)(Data2.strKor));  // 한글이 먼저 나옴
	else
		return (Data1.strCode).CollateNoCase((LPCTSTR)(Data2.strCode));  
}

void SortArray(CArray<MASTERCODE*, MASTERCODE*>& astrSort)
{
    //CString *pstrSave;
	MASTERCODE* pstrSave; 
    int     i, nTotal;

    nTotal   = astrSort.GetSize();
    //pstrSave  = new CString[nTotal];
	pstrSave  = new MASTERCODE[nTotal];
    for(i = 0 ; i < nTotal ; i++)
	{	
		MASTERCODE* pData	= astrSort.GetAt(i);
		pstrSave[i].strCode	= pData->strCode;
		pstrSave[i].strKor	= pData->strKor;
		pstrSave[i].strEng	= pData->strEng;
	}

   qsort(pstrSave, nTotal, sizeof(MASTERCODE), Compare);
	
   for( i = 0; i < astrSort.GetSize(); i++)
		delete astrSort.GetAt(i);
	astrSort.RemoveAll();	

    for(i = 0 ; i < nTotal ; i++)
	{
		MASTERCODE* pData = new MASTERCODE;
		pData->strCode	= pstrSave[i].strCode;
		pData->strKor	= pstrSave[i].strKor;
		pData->strEng	= pstrSave[i].strEng;
		
		astrSort.Add(pData);
	}
    delete[] pstrSave;
	
}


//관심을 제외한 종목을 추가시 사용한다. =======================================
//=============================================================================
void CVirtualListCtrl::LoadCodeList(int nOption)
{
//	if( !m_pDBMgr )
//		return;

	//======================================================	
	DeleteAllItems();
	for( int i = 0; i < m_AllArray.GetSize(); i++)
		delete m_AllArray.GetAt(i);
	m_AllArray.RemoveAll();	
	//======================================================

	//상장종목코드를 로드함.
//	CStringArray	arrCode, arrKor, arrEng;
//
//	if(m_nListDataType == STOCK)
//	{		
//		g_nSelCol = 0;
//		m_nFindCol = 0;
//
//		m_nOption = nOption;		
//		if( m_nOption == 0 || m_nOption == 1) //Radio index가 전종목 또는 KOSPI
//		{	
//			// KOSPI ===================================================================
//			m_pDBMgr->GetMasterData(CODE_JONGMOK, arrCode, arrKor);
//			int nCnt = arrCode.GetSize();
//			for( int i = 0; i < nCnt; i++)
//			{
//				MASTERCODE* pData = new MASTERCODE;
//				pData->strCode = arrCode[i];
//				pData->strKor = arrKor[i];
//				pData->strEng = _T("");	
//				m_AllArray.Add(pData);
//			}
//		}
//
//		if( m_nOption == 0 || m_nOption == 2) //Radio index가 전종목 또는 KOSDAQ
//		{
//			// KOSDAQ =================================================================
//			arrCode.RemoveAll();
//			arrKor.RemoveAll();
//			m_pDBMgr->GetMasterData(CODE_KJONGMOK, arrCode, arrKor);	//stock (1)
//			int nCnt = arrCode.GetSize();
//			for(int i = 0; i < nCnt; i++)
//			{
//				MASTERCODE* pData = new MASTERCODE;
//				pData->strCode = arrCode[i];
//				pData->strKor = arrKor[i];
//				pData->strEng = _T("");				
//				m_AllArray.Add(pData);
//			}
//		}
//
//		SortArray(m_AllArray);		
//	}
//	else if(m_nListDataType == UPJONG)
//	{		
//		m_nOption = nOption;
//		
//		if( m_nOption == 0 || m_nOption == 1 ) //Radio index가 전종목 또는 KOSPI
//		{	
//			// KOSPI UPjong ===================================================================
//			arrCode.RemoveAll();
//			arrKor.RemoveAll();
//			m_pDBMgr->GetMasterData(CODE_UPJONG		, arrCode, arrKor);
//			int nCnt = arrCode.GetSize();
//			for( int i = 0; i < nCnt; i++)
//			{
//				MASTERCODE* pData = new MASTERCODE;
//				pData->strCode = arrCode[i];
//				pData->strKor = arrKor[i];
//				pData->strEng = _T("");	
//				m_AllArray.Add(pData);
//			}
//			//KOSPI200 Upjong===================================================================
//			arrCode.RemoveAll();
//			arrKor.RemoveAll();
//			m_pDBMgr->GetMasterData( CODE_K200UPJONG, arrCode, arrKor);			
//			nCnt = arrCode.GetSize();
//			for( i = 0; i < nCnt; i++)
//			{
//				MASTERCODE* pData = new MASTERCODE;
//				pData->strCode = arrCode[i];
//				pData->strKor = arrKor[i];
//				pData->strEng = _T("");	
//				m_AllArray.Add(pData);
//			}
//		}		
//		if( m_nOption == 0 || m_nOption == 2) //Radio index가 전종목 또는 KOSDAQ
//		{
//			// KOSDAQ =================================================================
//			arrCode.RemoveAll();
//			arrKor.RemoveAll();
//			m_pDBMgr->GetMasterData(CODE_KUPJONG, arrCode, arrKor);	//stock (1)
//			int nCnt = arrCode.GetSize();
//			for(int i = 0; i < nCnt; i++)
//			{
//				MASTERCODE* pData = new MASTERCODE;
//				pData->strCode = arrCode[i];
//				pData->strKor = arrKor[i];
//				pData->strEng = _T("");				
//				m_AllArray.Add(pData);
//			}
//		}
//
//		SortArray(m_AllArray);		
//	}
//	else if(m_nListDataType == FUTOPT) //선물 옵션 종목 조회
//	{
//		g_nSelCol = m_nFindCol = 1;
//		// FUTURE ===================================================================
//		m_pDBMgr->GetMasterData(CODE_FUTURE, arrCode, arrKor);
//		int nCnt = arrCode.GetSize();
//		for( int i = 0; i < nCnt; i++)
//		{
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = arrCode[i];
//			pData->strKor = arrKor[i];
//			pData->strEng = _T("");	
//
//			pData->strKor.TrimLeft(); pData->strKor.TrimRight();
//			pData->strCode.TrimLeft();pData->strCode.TrimRight();
//			
//			m_AllArray.Add(pData);
//		}	
//		// OPTION ===================================================================
//		arrCode.RemoveAll();
//		arrKor.RemoveAll();
//		m_pDBMgr->GetMasterData(CODE_OPTION, arrCode, arrKor);
//		nCnt = arrCode.GetSize();
//		for( i = 0; i < nCnt; i++)
//		{
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = arrCode[i];
//			pData->strKor = arrKor[i];
//			pData->strEng = _T("");	
//
//			pData->strKor.TrimLeft(); pData->strKor.TrimRight();
//			pData->strCode.TrimLeft();pData->strCode.TrimRight();
//			pData->strKor.Replace("*", ""); 
//			pData->strKor.Replace("#", ""); 
//
//			m_AllArray.Add(pData);
//		}
//	}	
//	else if(m_nListDataType == FOREIGN)  //해외 지수 항목 조회
//	{
//		//=====================================================
//		// 해외 국가지수(구:해외지수 및 환율)
//		m_pDBMgr->GetMasterData(CODE_EXCHANGE, arrCode, arrKor);
//		int nCnt = arrCode.GetSize();
//		for( int i = 0; i < nCnt; i++)
//		{
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = arrCode[i];
//			pData->strKor = arrKor[i];
//			pData->strEng = _T("");	
//
//			
//			pData->strKor.Replace("0", "");
//			pData->strKor.Replace("1", "");
//			pData->strKor.Replace("2", "");
//			pData->strKor.TrimLeft();
//			pData->strKor.TrimRight();
//			
//			m_AllArray.Add(pData);
//		}
//
//		//=====================================================
//		// 미국 지수/업종						
//		arrCode.RemoveAll();
//		arrKor.RemoveAll();
//		m_pDBMgr->GetMasterData(CODE_EXCH_AUP, arrCode, arrKor);
//		nCnt = arrCode.GetSize();
//		for( i = 0; i < nCnt; i++)
//		{
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = arrCode[i];
//			pData->strKor = arrKor[i];
//			pData->strEng = _T("");	
//			
//			pData->strKor.Replace("0", "");
//			pData->strKor.Replace("1", "");
//			pData->strKor.Replace("2", "");
//			pData->strKor.TrimLeft();
//			pData->strKor.TrimRight();
//
//			m_AllArray.Add(pData);
//		}
//
//
//		//=====================================================
//		// 미국 종목(DOW30, NASDAQ100, S&P500)	
//		arrCode.RemoveAll();
//		arrKor.RemoveAll();
//		m_pDBMgr->GetMasterData(CODE_EXCH_AJONG, arrCode, arrKor);
//		nCnt = arrCode.GetSize();
//		for( i = 0; i < nCnt; i++)
//		{
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = arrCode[i];
//			pData->strKor = arrKor[i];
//			pData->strEng = _T("");	
//			
//			pData->strKor.Replace("0", "");
//			pData->strKor.Replace("1", "");
//			pData->strKor.Replace("2", "");
//			pData->strKor.TrimLeft();
//			pData->strKor.TrimRight();
//
//			m_AllArray.Add(pData);
//		}
//
//
//		//=====================================================
//		// 해외 DR종목							
//		arrCode.RemoveAll();
//		arrKor.RemoveAll();
//		m_pDBMgr->GetMasterData(CODE_EXCH_DR, arrCode, arrKor);
//		nCnt = arrCode.GetSize();
//		for( i = 0; i < nCnt; i++)
//		{
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = arrCode[i];
//			pData->strKor = arrKor[i];
//			pData->strEng = _T("");	
//			
//			pData->strKor.Replace("0", "");
//			pData->strKor.Replace("1", "");
//			pData->strKor.Replace("2", "");
//			pData->strKor.TrimLeft();
//			pData->strKor.TrimRight();
//
//			m_AllArray.Add(pData);
//		}		
//		
//		SortArray(m_AllArray);
//
//		//======================================
//	}
//	else if(m_nListDataType == KWANSIM)  //관심종목 항목 조회
//	{
//		
//	}
		
	
	SetItemCount(m_AllArray.GetSize());		
	//SortItem은 속성에 OWNERDATA속성이 있으면 사용불가다.
	//SortItems(Compare, (LPARAM)this);
}

void CVirtualListCtrl::SetMasterArray()
{	
//	for( int i = 0; i < m_AllArray.GetSize(); i++)
//	{
//		MASTERCODE *pData = m_AllArray.GetAt(i);
//		m_arrCode.Add(pData->strCode);
//		m_arrKor.Add(pData->strKor);
//	}
}

void CVirtualListCtrl::ChangeCode(CString strFind)
{	
	char* pChar1 = (LPTSTR)(LPCTSTR)strFind.Left(1);
	if(m_nFindCol == 0)  //한글로 검색 상태인데 
		if(*pChar1 > '0' && *pChar1 < '9') //숫자의 값일경우 
			return;		//리턴한다.		
	else if(m_nFindCol == 1)  //숫자(종목코드)로 검색 상태인데 
		if(*pChar1 < '0' && *pChar1 > '9') //숫자가 아닌 경우 
			return;		//리턴한다.				

	int nFindItem = FindItemByString(m_nFindCol, strFind);
	if( nFindItem <= 0 )
		return;
	
	SetRedraw(FALSE);		
	SetTopIndex(nFindItem);
	SetRedraw(TRUE);
	Invalidate(FALSE);
	UpdateWindow();
	SetItemState(nFindItem, LVIS_FOCUSED|LVIS_SELECTED,LVIS_FOCUSED|LVIS_SELECTED);

	m_strCode = GetItemText(nFindItem, 1);

	CString Code, Name;
	Code = GetItemText(nFindItem, 0);
	Name = GetItemText(nFindItem, 1);
	m_strCodeNName = Code + "|" + Name;

/**/
}

int CVirtualListCtrl::FindItemByString(int nCol, CString strFind)
{
	int nFindIdx = -1;		
	int nLen = _mbslen((const unsigned char*)(LPCTSTR)strFind);


	int nTot = m_AllArray.GetSize();
	CString sTemp;
	for(int i = 0 ; i < nTot; i++)
	{
		MASTERCODE *pMaster = m_AllArray.GetAt(i);
		if ( m_nFindCol == 0)
		{
			if( pMaster->strKor >= strFind )	
			{
				nFindIdx = i;
				break;
			}
		}
		else if ( m_nFindCol == 1)
		{
			if( pMaster->strCode.Left(nLen) >= strFind )				
			{
				nFindIdx = i;
				break;
			}
		}
	}
	




//===============================================================
/*	for (int i = 0; i < GetItemCount(); i++)	
	{
		CString str = GetItemText(i, nCol);		
		if( nCol == 0 || nCol == 2)
		{		
			if (str.Left(nLen) >= strFind )	
			{		
				nFindIdx = i;
				return nFindIdx;
			}
		}
		else if( nCol == 1 )
		{
			if (str.Mid(nLen, 1) >= strFind )
			{			//ljj 20030904 종목코드순일때
				nFindIdx = i;
				return nFindIdx;
			}
		}
	} 
*/
	return nFindIdx;
}

void CVirtualListCtrl::SetTopIndex(int nIndex)	
{
	int nPage, nLine,i = 0;
	
	SCROLLINFO	sInfo;
	GetScrollInfo(SB_VERT, &sInfo);
	int pos = GetScrollPos(SB_VERT);
	
	if (pos < nIndex)	{
		nPage = (nIndex-pos)/sInfo.nPage;
		nLine = (nIndex-pos)%sInfo.nPage;
		for( i=0; i < nPage; i++)
			SendMessage(WM_VSCROLL, SB_PAGEDOWN, NULL);
		for(i=0; i < nLine; i++)
			SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
	}
	else	{
		nPage = (pos - nIndex) / sInfo.nPage;
		nLine = (pos - nIndex) % sInfo.nPage;
		for(int i=0; i < nPage; i++)
			SendMessage(WM_VSCROLL, SB_PAGEUP, NULL);
		for(i=0; i < nLine; i++)
			SendMessage(WM_VSCROLL, SB_LINEUP, NULL);
	}	

}


void CVirtualListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if( m_nFindCol == pNMListView->iSubItem )
	{
		*pResult = 0;
		return;
	}
	//DeleteAllItems();
	if(m_nListDataType == STOCK ||
	   m_nListDataType == FOREIGN )
	{
		g_nSelCol = m_nFindCol = pNMListView->iSubItem;
		SortArray(m_AllArray);
		Invalidate();
	}

	*pResult = 0;
}

































//DEL BOOL CVirtualListCtrl::CompareAndSwap(int pos) 
//DEL { 
//DEL     CString vc_kor; 
//DEL 	CString vc_code; 
//DEL 	CString vc_eng;
//DEL 
//DEL     int posFirst = pos; 
//DEL     int posNext = pos + 1; 
//DEL 
//DEL 	if(m_nFindCol == 0)
//DEL 	{    
//DEL 		if (m_arrKor.GetAt(posFirst).CompareNoCase(m_arrKor.GetAt(posNext)) > 0) 
//DEL 		{ 
//DEL 			//======================================================================
//DEL 			vc_kor = m_arrKor.GetAt(posFirst); 
//DEL 			m_arrKor.SetAt(posFirst, m_arrKor.GetAt(posNext)); 
//DEL 			m_arrKor.SetAt(posNext, vc_kor); 
//DEL 			//======================================================================
//DEL 
//DEL 			//======================================================================
//DEL //			vc_code = m_arrCode.GetAt(posFirst); 
//DEL //			m_arrCode.SetAt(posFirst, m_arrCode.GetAt(posNext)); 
//DEL //			m_arrCode.SetAt(posNext, vc_code); 
//DEL 			//======================================================================
//DEL 
//DEL 			//======================================================================
//DEL 			//vc_eng = m_arrEng.GetAt(posFirst); 
//DEL 			//m_arrEng.SetAt(posFirst, m_arrEng.GetAt(posNext)); 
//DEL 			//m_arrEng.SetAt(posNext, vc_eng); 
//DEL 			//======================================================================
//DEL 			return TRUE; 
//DEL 		} 
//DEL 	}
//DEL 	else if(m_nFindCol == 1)
//DEL 	{
//DEL 		if (m_arrCode.GetAt(posFirst).CompareNoCase(m_arrCode.GetAt(posNext)) > 0) 
//DEL 		{ 
//DEL 			//======================================================================
//DEL 			vc_code = m_arrCode.GetAt(posFirst); 
//DEL 			m_arrCode.SetAt(posFirst, m_arrCode.GetAt(posNext)); 
//DEL 			m_arrCode.SetAt(posNext, vc_kor); 
//DEL 			//======================================================================
//DEL 
//DEL 			//======================================================================
//DEL //			vc_kor = m_arrKor.GetAt(posFirst); 
//DEL //			m_arrKor.SetAt(posFirst, m_arrKor.GetAt(posNext)); 
//DEL //			m_arrKor.SetAt(posNext, vc_kor); 
//DEL 			//======================================================================
//DEL 
//DEL 			//======================================================================
//DEL 			//vc_eng = m_arrEng.GetAt(posFirst); 
//DEL 			//m_arrEng.SetAt(posFirst, m_arrEng.GetAt(posNext)); 
//DEL 			//m_arrEng.SetAt(posNext, vc_eng); 
//DEL 			//======================================================================
//DEL 			return TRUE; 
//DEL 		}
//DEL 	}
//DEL     return FALSE;     
//DEL } 

void CVirtualListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CString Code, Name;
	Code = GetItemText(pNMListView->iItem, 0);
	Name = GetItemText(pNMListView->iItem, 1);
	m_strCodeNName = Code + "|" + Name;
	*pResult = 0;
}


//void CVirtualListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	// TODO: Add your control notification handler code here
//	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
//	CString Code, Name;
//	Code = GetItemText(pNMListView->iItem, 0);
//	Name = GetItemText(pNMListView->iItem, 1);
//	m_strCodeNName = Code + "|" + Name;
//
//	::SendMessage(GetParent()->GetSafeHwnd(), UMSG_ADDITEM_BTNCLICK, 0, 0);
//	*pResult = 0;
//}

BOOL CVirtualListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CVirtualListCtrl::Load_GwanSim_CodeList(CStringArray* pArrCodeNName)
{
//	if( !m_pDBMgr )
//		return;
//
//	//======================================================	
//	DeleteAllItems();
//	for( int i = 0; i < m_AllArray.GetSize(); i++)
//		delete m_AllArray.GetAt(i);
//	m_AllArray.RemoveAll();	
//	//======================================================
//
//
//	CString str, strCode, strName, strGrpNo, strGubun;
//	for( i = 1; i < pArrCodeNName->GetSize(); i++)
//	{
//		str = pArrCodeNName->GetAt(i);
//		if(m_pDBMgr)
//		{
//			strGrpNo	=  str.Left(GWANSIM_GRPNO_LEN); //그룹번호
//			strCode		=  str.Mid(GWANSIM_GRPNO_LEN, 15);  strCode.TrimRight();
//
//			strGubun	= str.Mid(15);	  strGubun.TrimLeft();
//			m_pDBMgr->GetCodeName((DWORD)atoi(strGubun),	strCode, strName);
//			strName.TrimRight();
//		
//			strCode.TrimLeft(); strCode.TrimRight();
//			strName.TrimLeft(); strName.TrimRight();
//			if(!strCode.GetLength() || !strName.GetLength()) continue;
//
//			MASTERCODE* pData = new MASTERCODE;
//			pData->strCode = strCode;
//			pData->strKor = strName;
//			pData->strEng = _T("");	
//			m_AllArray.Add(pData);
//		}
//	}
//
//	//SortArray(m_AllArray); //11.04 == 소트 기능 빼달라 그래서 뺀다.
//	SetItemCount(m_AllArray.GetSize());
}

void CVirtualListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);	
	// TODO: Add your message handler code here
	if(cx > 83)
	{
		LONG WindowStyle = GetWindowLong(m_hWnd, GWL_STYLE);	
		if( WindowStyle & WS_VSCROLL) 
			SetColumnWidth(0,  cx - 55 - 1);
		else
			SetColumnWidth(0, cx - 55 - 18);
		SetColumnWidth(1, 55);
	}

	UpdateWindow();
}

//	리스트의 행의 높이를 적용한다.
void CVirtualListCtrl::SetRowHeight(short nHeight)
{
	m_ImageList.Create(1,nHeight,ILC_COLOR,1,1);
	SetImageList(&m_ImageList,LVSIL_SMALL);
}