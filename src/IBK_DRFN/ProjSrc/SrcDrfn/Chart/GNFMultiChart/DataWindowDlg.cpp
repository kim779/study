// DataWindowDlg.cpp : implementation file
//
#include "stdafx.h"
#include "multichart.h"

#include "DataWindowDlg.h"
//#include "../chart_common/DataList/PacketList.h"
//#include "../chart_common/DataList/MainBlock.h"
//#include "../chart_common/DataList/OutsideCommonInterface.h"
//#include "../chart_common/DataList/ViewData.h"
#include "../chart_common/DataList/Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataWindowDlg dialog


CDataWindowDlg::CDataWindowDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDataWindowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataWindowDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pPacketList = NULL;
//	m_pMainBlock = NULL;
//	m_pChartOcxInterface = NULL;
//	m_nDataIndex = -1;
//	m_nPrevDataCount = 0;
//	m_ptPrevMousePoint = CPoint(-1, -1);
//	m_pChartOcxInterface = NULL;
	m_nDataIndex = -1;
	m_nPrevDataCount = 0;
	m_ptPrevMousePoint = CPoint(-1, -1);
}


void CDataWindowDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataWindowDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_listctrlData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataWindowDlg, CRscDlg)
	//{{AFX_MSG_MAP(CDataWindowDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataWindowDlg message handlers



/*-----------------------------------------------------------------------------------------
 - Function    :  OnOK, OnCancel
 - Created at  :  2005-07-01   15:36
 - Author      :  최종찬
 - Description :  Enter키와 Esc키에 의해 Dlg가 닫히는것을 막는다.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::OnOK() 
{
//	CDialog::OnOK();
}
void CDataWindowDlg::OnCancel() 
{
//	CDialog::OnCancel();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  CreateDataWindow
 - Created at  :  2005-07-05   13:22
 - Author      :  최종찬
 - Description :  DataWindow Dlg를 생성하고 가장 마지막 데이타를 기준으로 5줄을 
				  ListCtrl에 뿌려준다.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::CreateDataWindow()
{
	// 생성.
	Create(IDD_DIALOG_DATAWINDOW, NULL);
	// TEST: 종찬(05/07/05) 요건 임시.
	CRect rectDlg;
	GetClientRect(&rectDlg);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, rectDlg.Width(), rectDlg.Height(), SWP_SHOWWINDOW);

	m_listctrlData.SetExtendedStyle(m_listctrlData.GetExtendedStyle()|LVS_EX_GRIDLINES);


	// 데이타의 마지막 인덱스를 구해온다.
	int nEndIndex = m_pChartOcxInterface->GetViewEndDataIndex(true);
	// Block의 Column 인덱스는 첫번째껄루 한다.
	int nBlockColumnIndex = 0;

	SetListCtrlHeader(nBlockColumnIndex);

	// ListCtrl Data초기화.
	LV_ITEM ltc;
	ltc.mask = LVIF_TEXT;
	ltc.iSubItem = 0;
	ltc.pszText = _T("");
	for(int i = 0; i < 5; i++)
	{
		ltc.iItem = i;
		m_listctrlData.InsertItem(&ltc);
	}

	SetData(nBlockColumnIndex, nEndIndex);

	// 이전데이타갯수를 기억해둔다.
	m_nPrevDataCount = GetDataCount();

	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  InitDataWindow
 - Created at  :  2005-07-13   11:33
 - Author      :  최종찬
 - Description :  DataWindow 초기화
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::InitDataWindow(CPacketList* pPacketList, CMainBlock* pMainBlock)
{
	m_pPacketList = pPacketList;
	m_pMainBlock = pMainBlock;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetOutsideCommonInterface
 - Created at  :  2005-07-05   13:35
 - Author      :  최종찬
 - Parameters  :  pChartOcxInterface - 차트OCX에 접근할 인터페이스
 - Description :  차트OCX에 접근할 인터페이스를 멤버에 세팅한다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetOutsideCommonInterface(COutsideCommonInterface* pChartOcxInterface)
{
	m_pChartOcxInterface = pChartOcxInterface;
}

/*-----------------------------------------------------------------------------------------
 - Function    :  SetData
 - Created at  :  2005-07-05   11:02
 - Author      :  최종찬
 - Parameters  :  nBlockColumnIndex - Block의 Column Index.
 -                nIndex         - 5줄의 데이타중 가운데(3번째)줄의 데이타인덱스.
 - Return Value:  None
 - Description :  Container에 데이타를 넣는다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetData(const int nBlockColumnIndex, const int nIndex)
{
	CViewData viewData;
	if(!m_pMainBlock->GetViewDataListInAllBlock(viewData))
		return;
	if(viewData.GetColumnViewDataCount() <= 0)
		return;

	// ListCtrl에 표현할 데이타의 끝인덱스를 구해온다.
	int nEndIndex = GetEndIndex(nIndex);

	int nInsertColumnIndex = 0;	// ListCtrl의 Column Index.

	int nBlockRowCount = viewData.GetRowViewDataCount(nBlockColumnIndex);
	for(int i = 0; i < 5; i++)
	{
		nInsertColumnIndex = 0;	// Column Index 초기화

		// 가로스케일데이타 구하여 ListCtrl에 세팅한다.
		CRowViewData rowViewDataForHorzScale = viewData.GetAt(nBlockColumnIndex, 0);
		SetHorzScaleData(rowViewDataForHorzScale.GetHorzScaleName(), nEndIndex, i, nInsertColumnIndex);

		for(int j = 0; j < nBlockRowCount; j++)
		{
			CRowViewData rowViewData = viewData.GetAt(nBlockColumnIndex, j);
		
			for(int k = 0; k < rowViewData.GetSubGraphViewDataCount(); k++)
			{
				POSITION pos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(k);
				if(!pos)
					continue;
				CSubGraphViewData subGraphViewData = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(pos);

				SetViewData(subGraphViewData.GetTitle(), subGraphViewData.GetPacketNames(), nEndIndex, i, nInsertColumnIndex);
			}//end of for(k
		}//end of for(j		

		nEndIndex--;
	}//end of for(i
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHorzScaleData
 - Created at  :  2005-07-05   17:05
 - Author      :  최종찬
 - Parameters  :  strHorzScaleName - 가로스케일명
 -                nDataIndex       - 데이타 인덱스
 -				  nInsertRowIndex  - ListCtrl에 들어갈 인덱스
 -				  nInsertColumnIndex - ListCtrl에 들어갈 칼럼인덱스
 - Description :  스케일데이타를 리스트컨트롤에 Insert한다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetHorzScaleData(const CString& strHorzScaleName, const int nDataIndex, const int nInsertRowIndex, int& nInsertColumnIndex)
{
	CPacket* pPacket = m_pPacketList->GetBaseData(strHorzScaleName);
	if(pPacket == NULL)
		return;

	CString strScaleData = _T("");	// Scale Data(string type)
	double dScaleData = 0.0;		// Scale Data(double type)
	POSITION pos;

	if(pPacket->GetType() == _T("문자"))
	{
		pos = pPacket->GetstrDataList()->FindIndex(nDataIndex);
		if(pos != NULL)
			strScaleData = pPacket->GetstrDataList()->GetNext(pos);
	}
	else
	{
		pos = pPacket->GetnumericDataList()->FindIndex(nDataIndex);
		if(pos != NULL)
			dScaleData = pPacket->GetnumericDataList()->GetNext(pos);
		else
			dScaleData = pPacket->GetDataPastandFuture(nDataIndex);

		if(strHorzScaleName.Compare(_T("자료일자")) != 0)
			strScaleData = CDataConversion::DoubleToString(dScaleData);
		else
			strScaleData = CDataConversion::GetStringFormatTrans(CDataConversion::IntToString((int)dScaleData), pPacket->GetType(), _T(""));
	}

	m_listctrlData.SetItemText(nInsertRowIndex, nInsertColumnIndex, strScaleData);
	
	nInsertColumnIndex++;	// 칼럼값 증가.

	return;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetEndIndex
 - Created at  :  2005-07-05   17:34
 - Author      :  최종찬
 - Parameters  :  nIndex - 마우스가 위치한곳의 데이타 인덱스
 - Return Value:  int - ListCtrl에 표시할 데이타중 첫번째가 될 데이타의 인덱스
 - Description :  ListCtrl에 표현할 데이타의 첫번째 인덱스를 구한다.
 -----------------------------------------------------------------------------------------
int CDataWindowDlg::GetEndIndex(const int nIndex) const
{
	// 앞부분 위치했을 경우 처리
	if(nIndex - 2 < 0)
		return 4;

	// 끝부분 위치했을 경우 처리
	if(nIndex + 2 > m_pChartOcxInterface->GetViewEndDataIndex(true))
		return m_pChartOcxInterface->GetViewEndDataIndex(true);

	return nIndex + 2;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetListCtrlHeader
 - Created at  :  2005-07-06   13:13
 - Author      :  최종찬
 - Description :  ListCtrl의 헤더부분을 세팅한다.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::SetListCtrlHeader(const int nBlockColumnIndex)
{
	CViewData viewData;
	if(!m_pMainBlock->GetViewDataListInAllBlock(viewData))
		return false;
	if(viewData.GetColumnViewDataCount() <= 0)
		return false;

	int nHeaderColumnCount = m_listctrlData.GetHeaderCtrl()->GetItemCount();	// 칼럼갯수.
	int nHeaderColumnIndex = 0;	// 칼럼 초기화.
	
	int nBlockRowCount = viewData.GetRowViewDataCount(nBlockColumnIndex);
	for(int i = 0; i < nBlockRowCount; i++)
	{
		CRowViewData rowViewData = viewData.GetAt(nBlockColumnIndex, i);
		
		if(0 == i)	// 한번만 수행한다.
		{		// 첫번째 헤더: 자료일자
			CString strHorzTitle;
			CPacket* pPacket = NULL;
			pPacket = m_pPacketList->GetBaseData(rowViewData.GetHorzScaleName());
			if(pPacket == NULL) return false;
			if(pPacket->GetType().Find(_T("HH")) >= 0)
				strHorzTitle = _T("시간");
			else
				strHorzTitle = _T("일자");

			SetHeaderTitle(nHeaderColumnIndex, nHeaderColumnCount, strHorzTitle);
		}

		// 그다음 헤더들.
		for(int j = 0; j < rowViewData.GetSubGraphViewDataCount(); j++)
		{
			POSITION pos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(j);
			if(!pos)
				continue;
			CSubGraphViewData subGraphViewData = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(pos);

			SetNextHeader(subGraphViewData.GetTitle(), subGraphViewData.GetPacketNames(), nHeaderColumnIndex, nHeaderColumnCount);
		}
	}

	// 불필요한 Header Column삭제.
	for( ; nHeaderColumnIndex < nHeaderColumnCount; nHeaderColumnIndex++)
	{
		m_listctrlData.DeleteColumn(nHeaderColumnIndex);
	}
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetNextHeader
 - Created at  :  2005-07-07   11:15
 - Author      :  최종찬
 - Parameters  :  strTitle           - 타이틀
 -                strPacketNames     - 패킷명
 -                nHeaderColumnIndex - 헤더의 칼럼인덱스
 -                nHeaderColumnCount - 이전 헤더의 칼럼갯수.
 - Return Value:  bool - true:성공, false:실패.
 - Description :  자료일자칼럼이외의 칼럼을 세팅한다.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::SetNextHeader(const CString& strTitle, const CString& strPacketNames, int& nHeaderColumnIndex, const int nHeaderColumnCount)
{
	CString strRealPacketNames = GetRealPacketNames(strTitle, strPacketNames);
	
	if(1 ==  CDataConversion::GetStringDataCount(strRealPacketNames, _T(",")))
	{
		SetHeaderTitle(nHeaderColumnIndex, nHeaderColumnCount, strTitle);
	}
	else
	{
		while(strRealPacketNames.IsEmpty() == false)
		{
			CString strHeaderTitle = CDataConversion::GetStringData(strRealPacketNames, _T(","));
			SetHeaderTitle(nHeaderColumnIndex, nHeaderColumnCount, strHeaderTitle);
		}
	}
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHeaderTitle
 - Created at  :  2005-07-07   11:10
 - Author      :  최종찬
 - Parameters  :  nColumnIndex   - 칼럼인덱스
 -                nColumnCount   - 이전상태에서의 헤더칼럼갯수
 -                strHeaderTitle - 세팅할 헤더타이틀
 - Return Value:  bool - true:성공, false:실패
 - Description :  헤더타이틀을 세팅한다. 이전 칼럼갯수보다 많으면 Insert하고 그 이전것들은 
				  SetItem을 수행한다. 또 이전 헤더칼럼갯수가 남으면 DeleteColumn을 한다.
 -----------------------------------------------------------------------------------------
bool CDataWindowDlg::SetHeaderTitle(int& nColumnIndex, const int nColumnCount, const CString& strHeaderTitle)
{
	if(nColumnCount < nColumnIndex)
	{	// 새로 추가.
		m_listctrlData.InsertColumn(nColumnIndex, strHeaderTitle, LVCFMT_RIGHT, 80);
	}
	else
	{	// 기존꺼 변경
		HDITEM hdi;
		TCHAR lpBuffer[256];
		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;

		m_listctrlData.GetHeaderCtrl()->GetItem(nColumnIndex, &hdi);
		strcpy(hdi.pszText, (LPCTSTR)strHeaderTitle);
		m_listctrlData.GetHeaderCtrl()->SetItem(nColumnIndex, &hdi);
	}
	nColumnIndex++;
	return true;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetRealPacketNames
 - Created at  :  2005-07-06   15:39
 - Author      :  최종찬
 - Description :  현재 차트상태가 1틱일경우는 가격데이타는 "종가"만 표현한다.
 -----------------------------------------------------------------------------------------
CString CDataWindowDlg::GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const
{
	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";

	if(strTitle == _T("가격") && strPacketNames.Find(_T("종가")) > 0)
	{
		// 숫자 5는 "틱"의 차트에서의 Enum값이다.
		if(m_pPacketList->GetDateUnitType() == 5 && m_pPacketList->GetTimeInterval() == 1)
			strRealPacketNames = _T("종가,");
	}
	return strRealPacketNames;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetViewData
 - Created at  :  2005-07-06   19:34
 - Author      :  최종찬
 - Parameters  :  strTitle		 - 타이틀명
 -				  strPacketNames - 패킷명.
 -                nDataIndex     - 데이타 인덱스
 -				  nInsertRowIndex   - ListCtrl의 Row Index.(즉, 데이타를 Insert할 곳.)
 -				  nInsertColumnIndex- ListCtrl의 Column Index.(즉, 데이타를 Insert할 곳.)
 - Description :  nDataIndex에 해당되는 데이타를 ListCtrl에 Insert시킨다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetViewData(const CString& strTitle, const CString& strPacketNames, const int nDataIndex, const int nInsertRowIndex, int& nInsertColumnIndex)
{
	CString strRealPacketNames = GetRealPacketNames(strTitle, strPacketNames);

	int nPacketCount = CDataConversion::GetStringDataCount(strRealPacketNames, _T(","));

	CString strData;
	if(nPacketCount == 1)
	{
		CString strPacketName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
		CPacket* pPacket = NULL;
		pPacket = m_pPacketList->GetBaseData(strPacketName);		
		//2005. 07. 18 ====================================
		if(pPacket == NULL) return;
		//=================================================


		if(pPacket->GetType() == _T("문자"))
		{
			POSITION pos = pPacket->GetstrDataList()->FindIndex(nDataIndex);
			if(!pos)
				return;
			strData = pPacket->GetstrDataList()->GetNext(pos);
		}
		else
		{
			POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDataIndex);
			if(!pos)
				return;
			double dData = pPacket->GetnumericDataList()->GetNext(pos);
			int nLength = pPacket->GetType().GetLength();
			int nDecimalPt = pPacket->GetType().Find(".") + 1;
			int nDecimal;

			if(nDecimalPt == 0)
				nDecimal = 0;
			else
				nDecimal = nLength - nDecimalPt;

			strData = CDataConversion::DoubleToString(dData, nDecimal);
			strData = CDataConversion::GetStringFormatTrans(strData, pPacket->GetType());
		}
		
		m_listctrlData.SetItemText(nInsertRowIndex, nInsertColumnIndex, strData);
		nInsertColumnIndex++;	// 칼럼값 증가.
	}//end of if(nPacketCount...
	else
	{
		while(strRealPacketNames.IsEmpty() == false)
		{
			CString strPacketName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
			CPacket* pPacket = NULL;
			pPacket = m_pPacketList->GetBaseData(strPacketName);
			if(pPacket == NULL)
				return;
			if(pPacket->GetType() == _T("문자"))
			{
				POSITION pos = pPacket->GetstrDataList()->FindIndex(nDataIndex);
				if(!pos)
					return;
				strData = pPacket->GetstrDataList()->GetNext(pos);
			}
			else
			{
				POSITION pos = pPacket->GetnumericDataList()->FindIndex(nDataIndex);
				if(!pos)
					return;
				double dData = pPacket->GetnumericDataList()->GetNext(pos);
				int nLength = pPacket->GetType().GetLength();
				int nDecimalPt = pPacket->GetType().Find(_T(".")) + 1;
				int nDecimal;

				if(nDecimalPt == 0)
					nDecimal = 0;
				else
					nDecimal = nLength - nDecimalPt;

				strData = CDataConversion::DoubleToString(dData, nDecimal);
				strData = CDataConversion::GetStringFormatTrans(strData, pPacket->GetType());
			}

			m_listctrlData.SetItemText(nInsertRowIndex, nInsertColumnIndex, strData);
			nInsertColumnIndex++;	// 칼럼값 증가.
		}//end of while(
	}//end of else
}


/*-----------------------------------------------------------------------------------------
 - Function    :  MoveMouse
 - Created at  :  2005-07-07   08:06
 - Author      :  최종찬
 - Description :  마우스 움직일때 point값에 해당하는 Data Index를 구하여 listctrl에 해당
				  데이타를 뿌려준다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::MoveMouse(CPoint point)
{
	// 해당 마우스 포인트에 위치하는 데이타 인덱스를 구해온다.
	if(!m_pChartOcxInterface)
		return;
	int nDataIndex = m_pChartOcxInterface->GetViewDataIndex(point);

//	m_listctrlData.DeleteAllItems();
	CBlockIndex blockIndex = m_pMainBlock->FindBlockNumber(point);
	if(blockIndex.GetColumn() == -1)
		return;
	
	SetListCtrlHeader(blockIndex.GetColumn());
	SetData(blockIndex.GetColumn(), nDataIndex);

	// 마우스의 위치를 기억해둔다.
	m_ptPrevMousePoint = point;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeTRData
 - Created at  :  2005-07-12   10:31
 - Author      :  최종찬
 - Description :  TR데이타를 조회할때... 첫번째 Block Column Index에 마지막 데이타 인덱스값을 뿌려준다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::ChangeTRData(void)
{
	// 데이타의 마지막 인덱스를 구해온다.
	int nEndIndex = m_pChartOcxInterface->GetViewEndDataIndex(true);
	// Block의 Column 인덱스는 첫번째껄루 한다.
	int nBlockColumnIndex = 0;

	SetListCtrlHeader(nBlockColumnIndex);
	SetData(nBlockColumnIndex, nEndIndex);
	
	// 이전 데이타를 기억해둔다.
	m_nPrevDataCount = GetDataCount();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ChangeRealData
 - Created at  :  2005-07-12   10:31
 - Author      :  최종찬
 - Description :  리얼데이타를 수신할때... 이전데이타갯수와 비교하여 현재 데이타 갯수가
				  많으면 새로운데이타가 들어온것이고, 이전데이타갯수와 같다면 Update만
				  한 상태이다. 또한 이전데이타갯수가 현재데이타갯수보다 크면
				  오래된 RealPacket데이타 일부를 지운 상태를 의미한다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::ChangeRealData(void)
{
	this->MoveMouse(m_ptPrevMousePoint);
	
	// 데이타갯수를 이전데이타갯수에 갱신해둔다.
	m_nPrevDataCount = GetDataCount();
}



/*-----------------------------------------------------------------------------------------
 - Function    :  GetDataCount
 - Created at  :  2005-07-12   13:55
 - Author      :  최종찬
 - Description :  데이타 갯수를 구한다.
 -----------------------------------------------------------------------------------------
int CDataWindowDlg::GetDataCount(void) const
{
	CViewData viewData;
	if(!m_pMainBlock->GetViewDataListInAllBlock(viewData))
		return -1;
	if(viewData.GetColumnViewDataCount() <= 0)
		return -1;

	// 데이타의 갯수를 구해둔다.
	CRowViewData tmpRowViewData = viewData.GetAt(0, 0);
	return m_pPacketList->GetCount(tmpRowViewData.GetHorzScaleName());
}
*/
void CDataWindowDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRscDlg::OnSize(nType, cx, cy);
	if(m_listctrlData.GetSafeHwnd())
	{
		CRect listRect;
		m_listctrlData.GetWindowRect(&listRect);

		listRect.right  = listRect.left + cx;
		listRect.bottom = listRect.top  + cy;
		ScreenToClient(listRect);
		m_listctrlData.MoveWindow(listRect, TRUE);
	}
}

BOOL CDataWindowDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message) 
	{
		case WM_KEYDOWN	:
			{
				UINT nChar = (UINT)pMsg->wParam;
				switch(nChar)
				{
					case VK_RETURN :
					case VK_ESCAPE :
					{
						return FALSE;
						break;					
					}
				}
				break;
			}
	}
//	return CRscDlg::PreTranslateMessage(pMsg);
	
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);
}

/*-----------------------------------------------------------------------------------------
 - Function    :  CreateDataWindow
 - Created at  :  2005-07-05   13:22
 - Author      :  최종찬
 - Description :  DataWindow Dlg를 생성하고 가장 마지막 데이타를 기준으로 5줄을 
				  ListCtrl에 뿌려준다.
 -----------------------------------------------------------------------------------------*/
bool CDataWindowDlg::CreateDataWindow()
{
	// 생성.
	Create(IDD_DIALOG_DATAWINDOW, NULL);
	// TEST: 종찬(05/07/05) 요건 임시.
	CRect rectDlg;
	GetClientRect(&rectDlg);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, rectDlg.Width(), rectDlg.Height(), SWP_SHOWWINDOW);

	m_listctrlData.SetExtendedStyle(m_listctrlData.GetExtendedStyle()| LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);


	// ListCtrl Data초기화.
	LV_ITEM ltc;
	ltc.mask = LVIF_TEXT;
	ltc.iSubItem = 0;
	ltc.pszText = _T("");
	for(int i = 0; i < 5; i++)
	{
		ltc.iItem = i;
		m_listctrlData.InsertItem(&ltc);
	}

//
//	SetListCtrlHeader(nBlockColumnIndex);
//	SetData(nBlockColumnIndex, nEndIndex);
//
//	// 이전데이타갯수를 기억해둔다.
//	m_nPrevDataCount = GetDataCount();

	return true;
}



/*-----------------------------------------------------------------------------------------
 - Function    :  SetOutsideCommonInterface
 - Created at  :  2005-07-05   13:35
 - Author      :  최종찬
 - Parameters  :  pChartOcxInterface - 차트OCX에 접근할 인터페이스
 - Description :  차트OCX에 접근할 인터페이스를 멤버에 세팅한다.
 -----------------------------------------------------------------------------------------
void CDataWindowDlg::SetOutsideCommonInterface(COutsideCommonInterface* pChartOcxInterface)
{
	m_pChartOcxInterface = pChartOcxInterface;
}
 -----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
 - Function    :  SetDataWindowData
 - Created at  :  2005-07-22   11:10
 - Author      :  최종찬
 - Description :  차트OCX에서 List에 담겨 있는 데이타를 가져와서 ListCtrl에 뿌린다.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::SetDataWindowData(CList<CString, CString&>* pListData)
{
	POSITION pos = pListData->GetHeadPosition();
	
	if(!pos)
		return;

	// Header 세팅.
	CString strHeader;
	strHeader.Format("%s", pListData->GetNext(pos));
	//CString strHeader = pListData->GetNext(pos);
	SetHeaderCtrl(strHeader);
	
	// 데이타 세팅.
	CString strListCtrlData;
	int nTot = pListData->GetCount();	nTot -= 2;
	for(int nRowIndex = 0; nRowIndex < nTot ; nRowIndex++)
	{
		strListCtrlData.Format("%s", pListData->GetNext(pos));		
		SetListCtrlData(strListCtrlData, nRowIndex);		
	}

	/*
	int nRowIndex = 0;
	while(pos)
	{
		CString strListCtrlData = pListData->GetNext(pos);		
		SetListCtrlData(strListCtrlData, nRowIndex);
		nRowIndex++;
	}*/

	CString strSelIndex;
	strSelIndex.Format("%s", pListData->GetTail());
	int nSelIndex = atoi(strSelIndex) - 1;
	m_listctrlData.SetItemState(nSelIndex, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);	
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetListCtrlData
 - Created at  :  2005-07-22   13:37
 - Author      :  최종찬
 - Description :  조회데이타를 List컨트롤에 Insert한다.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::SetListCtrlData(CString& strListCtrlData, const int nRowIndex)
{
	int nColumnIndex = 0;
	while(strListCtrlData.IsEmpty() == false)
	{
		CString strData = CDataConversion::GetStringData(strListCtrlData, _T("|"));
		m_listctrlData.SetItemText(nRowIndex, nColumnIndex, strData);
		nColumnIndex++;
	}
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetHeaderCtrl
 - Created at  :  2005-07-22   13:30
 - Author      :  최종찬
 - Description :  헤더컨트롤을 세팅한다.
 -----------------------------------------------------------------------------------------*/
void CDataWindowDlg::SetHeaderCtrl(CString& strListHeader)
{
	int nHeaderColumnCount = m_listctrlData.GetHeaderCtrl()->GetItemCount();

	int nHeaderColumnIndex = 0;
	while(strListHeader.IsEmpty() == false)
	{
		CString strHeaderTitle = CDataConversion::GetStringData(strListHeader, _T("|"));
		
		if(nHeaderColumnCount <= nHeaderColumnIndex)	// 새로 추가.
		{
			m_listctrlData.InsertColumn(nHeaderColumnIndex, strHeaderTitle, LVCFMT_RIGHT, 80);
		}
		else	// 기존꺼 변경.
		{
			HDITEM hdi;
			TCHAR lpBuffer[256];
			hdi.mask = HDI_TEXT;
			hdi.pszText = lpBuffer;
			hdi.cchTextMax = 256;

			m_listctrlData.GetHeaderCtrl()->GetItem(nHeaderColumnIndex, &hdi);
			strcpy(hdi.pszText, (LPCTSTR)strHeaderTitle);
			m_listctrlData.GetHeaderCtrl()->SetItem(nHeaderColumnIndex, &hdi);
		}

		nHeaderColumnIndex++;
	}

	// 불필요한 Header Column삭제.
	for( ; nHeaderColumnIndex < nHeaderColumnCount; nHeaderColumnIndex++)
		m_listctrlData.DeleteColumn(nHeaderColumnIndex);
}
