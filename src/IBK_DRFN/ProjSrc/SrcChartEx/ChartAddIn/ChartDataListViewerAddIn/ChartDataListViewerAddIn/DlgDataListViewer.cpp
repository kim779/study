// DlgDataListViewer.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "DlgDataListViewer.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Chart/Conversion.h"					// for CDataConversion
#include "../Include_Addin_134221/I134221/_IStringList.h"	// for IStringList
#include "../Include_Addin_134221/I134221/_IDoubleList.h"	// for IDoubleList
#include "Define.h"											//20081025 이문수 for user define message
#include "ChartDataListViewerAddInImp.h"					//20081023 이문수 for m_pAddInImp

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDataListViewer dialog


CDlgDataListViewer::CDlgDataListViewer(CWnd* pParent, const CString& strTitle,
									   const CString& strHorzScale,
									   const CString& strPacketNamesPerBlock,
									   const CString& strPacketNames,
									   const char *p_szSaveFilePath,
									   const CString& strRQs,
									   const bool bViewPacketSequence,
									   CChartDataListViewerAddInImp * pAddIn, //20081023 이문수 >>
									   HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
	: CDialog(CDlgDataListViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDataListViewer)
	m_pOcxWnd = pParent;
	m_bReverse = FALSE;
	//}}AFX_DATA_INIT

	m_strTitles = strTitle;
	m_strHorzScale = strHorzScale;
	m_strPacketNamesPerBlock = strPacketNamesPerBlock;
	m_strPacketNames = strPacketNames;
	m_strFilePath = p_szSaveFilePath;
	m_bViewPacketSequence = bViewPacketSequence;

	m_brBkColor.CreateSolidBrush(RGB(200,200,200));
	m_nFutureDateCount = 0;

	m_pPacketList = NULL;
	m_bPacketHeader = FALSE;
	m_bReverse = FALSE;
	m_nHighlightListItem = -1;
	//2007.05.16 by LYH 복수종목 적용
	m_strRQs = strRQs;

	//20081023 이문수 >>
	m_pAddInImp = pAddIn;
	m_pDataListViewMngWnd = pParent;
	//20081023 이문수 <<

	// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
	m_hOcxWnd = p_hOcxWnd;

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = p_eChartMode;
}
CDlgDataListViewer::~CDlgDataListViewer()
{
	if( m_pPacketList) m_pPacketList->Release();
	POSITION pos = m_DataList.GetHeadPosition();
	while(pos)
	{
		CData* pData = (CData*)m_DataList.GetNext(pos);
		delete pData;
		pData = NULL;
	}
	m_DataList.RemoveAll();
}


void CDlgDataListViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDataListViewer)
	DDX_Control(pDX, IDC_CLIPBOARD, m_btnClipboard);
	DDX_Control(pDX, IDC_UPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_EXCEL_FILE_EXPORT, m_btnRuntoExcel);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
	DDX_Control(pDX, IDC_REVERSE, m_btnReverse);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDataListViewer, CDialog)
	//{{AFX_MSG_MAP(CDlgDataListViewer)
	ON_BN_CLICKED(IDC_EXCEL_FILE_EXPORT, OnExcelFileExport)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	ON_BN_CLICKED(IDC_CLIPBOARD, OnClipboard)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST1, OnGetdispinfo)
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomdrawList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDataListViewer message handlers

BOOL CDlgDataListViewer::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	DWORD dwNewStyle = LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_SUBITEMIMAGES; //LVS_EX_GRIDLINES ;//| /*LVS_EX_SUBITEMIMAGES |*/
	m_ctlList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(dwNewStyle));

	//Dialog에 원버튼으로 포커스를 주기 위해 사용
	//mouse_event(MOUSEEVENTF_LEFTDOWN, 1, 1, 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTUP, 1, 1, 0, 0);

	if(m_bViewPacketSequence) {
		MakeHeader_PacketSequence();
		InsertData_PacketSequence();
	}
	else {

		//20081025 이문수 >>
		//ReCalc_WindowSize(m_strPacketNamesPerBlock);
		Make_ListHeader(m_strTitles, m_strPacketNamesPerBlock);
		ReCalc_WindowSize(m_strPacketNamesPerBlock);
		//20081025 이문수 <<

		InsertDatatoList(m_strTitles, m_strPacketNames, m_strPacketNamesPerBlock);	
	}

	CHeaderCtrl* pHeader = NULL;
	pHeader = m_ctlList.GetHeaderCtrl();

	if(pHeader == NULL)
		return TRUE;
	
	VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));	

	// 리스트 item 간 간격을 주기 위해 임의로 생성한다
	CImageList m_ImageList;
	m_ImageList.Create(IDB_TEMP, 1, 1, RGB(172, 172, 172));
	m_ctlList.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_HeaderCtrl.SetImageList(&m_ImageList);
	m_ctlList.SetFocus();
	m_ctlList.SetItemState( 0, LVIS_SELECTED, LVIS_SELECTED);

	EnsureVisible();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDataListViewer::ReCalc_WindowSize(const CString &szPacketNames)
{
	//20081025 이문수 >>
	int nListHeight = 400;
	int nBtnAndListInterval = 12;
	int nW, nH;
	int nWidth;
	int nMinWidth;
	int nMaxWidth = 730;	
	int nXMargin  = 20;
	int nTotalColWidth = 0;
	CRect rectList, rectRunToExcel, rectClipboard, rectReverse;
	
	m_ctlList.GetWindowRect(&rectList);				ScreenToClient(&rectList);
	m_btnRuntoExcel.GetWindowRect(&rectRunToExcel); ScreenToClient(&rectRunToExcel);
	m_btnClipboard.GetWindowRect(&rectClipboard);	ScreenToClient(&rectClipboard);
	m_btnReverse.GetWindowRect(&rectReverse);		ScreenToClient(&rectReverse);

	nMinWidth = rectReverse.right;

	CHeaderCtrl* pHeaderCtrl = m_ctlList.GetHeaderCtrl();
	if (pHeaderCtrl == NULL) return;
	int  i, nColumnCount = pHeaderCtrl->GetItemCount();
	for( i = 0  ; i < nColumnCount ; i++ )
	{
		nTotalColWidth += m_ctlList.GetColumnWidth(i);
	}
	
	nWidth	= nTotalColWidth + 100;
	nWidth	= min(nMaxWidth, nWidth);
	nWidth	= max(nMinWidth, nWidth);
	
	nW		= nWidth + nXMargin;
	nH		= rectList.top + nListHeight + nBtnAndListInterval * 2 + rectReverse.Height() + GetSystemMetrics(SM_CYCAPTION);
	SetWindowPos(NULL, 0, 0, nW, nH, SWP_NOZORDER | SWP_NOMOVE);
	m_ctlList.SetWindowPos(NULL,  0, 0, nWidth, nListHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_btnRuntoExcel.SetWindowPos(NULL,	rectRunToExcel.left, nListHeight + nBtnAndListInterval, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_btnClipboard.SetWindowPos(NULL,	rectClipboard.left,	 nListHeight + nBtnAndListInterval, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_btnReverse.SetWindowPos(NULL,		rectReverse.left,    nListHeight + nBtnAndListInterval, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

//	int nColumnCount = CDataConversion::GetStringDataCount(m_strPacketNamesPerBlock, ",") ;	
//	int nWidth = 70*nColumnCount + 100;
//	if(nWidth > 730)
//		nWidth = 730;	
//	if( nWidth < 205) nWidth = 205;
//	SetWindowPos(NULL, 0, 0, nWidth + 20, 400+70, SWP_NOMOVE);
//	m_btnRuntoExcel.SetWindowPos(NULL, 10, 412, 0, 0, SWP_NOSIZE);
//	m_btnClipboard.SetWindowPos(NULL, 145, 412, 0, 0, SWP_NOSIZE);
//	m_btnReverse.SetWindowPos(NULL, 252, 412, 0, 0, SWP_NOSIZE);
//	m_ctlList.SetWindowPos(NULL, 0, 0, nWidth, 400, SWP_NOMOVE);
	//20081025 이문수 <<
}

void CDlgDataListViewer::SetPacketListAddr( IPacketManager *pPacketList)
{
	if( m_pPacketList) m_pPacketList->Release();
	m_pPacketList = pPacketList;
	if( m_pPacketList) m_pPacketList->AddRef();
}

void CDlgDataListViewer::MakeHeader_PacketSequence()
{
	if(m_pPacketList == NULL)	return;

	CString szPackets = m_strPacketNames;
	CString szTemp;
	int i = 0;
	while(!szPackets.IsEmpty()) {
		szTemp = CDataConversion::GetStringData(szPackets, ",");
		m_ctlList.InsertColumn(i++, szTemp, LVCFMT_RIGHT, 100);
	}
}

void CDlgDataListViewer::InsertData_PacketSequence()
{
	if(m_pPacketList == NULL)	return;

	CString szPacketNames = m_strPacketNames;
	m_strHorzScale = CDataConversion::GetStringData(szPacketNames, ",");
	InsertDatatoList("", szPacketNames, "");
}

void CDlgDataListViewer::Make_ListHeader(const CString& strTitles, const CString& strPacketNamesPerBlock)
{
	if(strPacketNamesPerBlock.IsEmpty())
		return ;
	CString szTitles = strTitles, szT = m_strPacketNamesPerBlock ;

	int ntitleCount = CDataConversion::GetStringDataCount(strTitles, ";");

	LV_COLUMN lc;

	int width = 80;
	int j = 0, nStart = 0 ;

	CDC * pDC = GetDC();//20081025 이문수 >>

	//20081106 이문수 >>
	CString strRQs = m_strRQs;
	CString strRQ  = CDataConversion::GetStringData(strRQs, ";");
	IPacket *pPacketheader = m_pPacketList->GetPacket( strRQ, m_strHorzScale);
	//IPacket *pPacketheader = m_pPacketList->GetPacket( m_strHorzScale);
	//20081106 이문수 <<
	m_ctlList.SetIconSpacing(300, 10000);

	// 1st header
	if(pPacketheader)
	{
		lc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH ;
		lc.fmt = LVCFMT_RIGHT ; 
		//sy 2003.10.30. -> "PF_자료일자"일 경우 처리
		//if(pPacketheader->GetName() == "자료일자")	{
		ILPCSTR szPacketName = pPacketheader->GetPacketNameString();
		CString strPacketName( szPacketName);
		//if(strPacketName.Find("자료일자") >= 0)
		if(strPacketName.Find(_MTEXT(C0_DATE_TIME)) >= 0)
		{
			ILPCSTR szPacketType = pPacketheader->GetType();

			CString strType = szPacketType;
			CString strTime;
			CString strDate;

			// 2011.01.24 by SYS >> 분차트 YYYY(년) : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacketheader->GetDateUnitType();
			if (strType == _T("YYYYMMDDHHMMSS"))
			{
				int nHourType = 0;
				if( CScaleBaseData::HORZ_TIME_ONE	== eDateUnit || 
					CScaleBaseData::HORZ_TIME_TWO	== eDateUnit ||
					CScaleBaseData::HORZ_TICK		== eDateUnit ||
					CScaleBaseData::HORZ_TIME_SECOND== eDateUnit)
				{
					if (strType.Find("HH") >= 0)
						nHourType = 1;
				}
				else
				{
					nHourType = 0;
				}

				if (nHourType == 1)
				{
					if(strPacketName.Find("PF_") >= 0)
					{
						strTime.LoadString(IDS_TIME);
						strTime = "P&F_" + strTime;
						// lc.pszText = "PF_시간";
						lc.pszText = (LPSTR)(LPCTSTR)strTime;
					}
					else if(strPacketName.Find("ThreeLineBreak_") >= 0)
					{
						strTime.LoadString(IDS_TIME);
						CString strThreeLineBreak;
						strThreeLineBreak.LoadString( IDS_THREE_LINE_BREAK);
						strTime = strThreeLineBreak + "_" + strTime;
						// lc.pszText = "ThreeLineBreak_시간";
						lc.pszText = (LPSTR)(LPCTSTR)strTime;
					}
					else
					{
						strTime.LoadString(IDS_TIME);
						lc.pszText = (LPSTR)(LPCTSTR)strTime;
					}
				}
				else
				{
					if(strPacketName.Find("PF_") >= 0)
					{
						strDate.LoadString(IDS_DATE);
						strDate = "P&F_" + strDate;
						// lc.pszText = "PF_일자";
						lc.pszText = (LPSTR)(LPCTSTR)strDate;
					}
					else if(strPacketName.Find("ThreeLineBreak_") >= 0)
					{
						strDate.LoadString(IDS_DATE);
						CString strThreeLineBreak;
						strThreeLineBreak.LoadString( IDS_THREE_LINE_BREAK);
						strDate = strThreeLineBreak + "_" + strDate;
						// lc.pszText = "ThreeLineBreak_일자";
						lc.pszText = (LPSTR)(LPCTSTR)strDate;
					}
					else
					{
						strDate.LoadString(IDS_DATE);
						// lc.pszText = "일자";
						lc.pszText = (LPSTR)(LPCTSTR)strDate;
					}
				}
			}
			else
			{
				if (strType.Find("HH") >= 0)
				{
					if(strPacketName.Find("PF_") >= 0)
					{
						strTime.LoadString(IDS_TIME);
						strTime = "P&F_" + strTime;
						// lc.pszText = "PF_시간";
						lc.pszText = (LPSTR)(LPCTSTR)strTime;
					}
					else if(strPacketName.Find("ThreeLineBreak_") >= 0)
					{
						strTime.LoadString(IDS_TIME);
						CString strThreeLineBreak;
						strThreeLineBreak.LoadString( IDS_THREE_LINE_BREAK);
						strTime = strThreeLineBreak + "_" + strTime;
						// lc.pszText = "ThreeLineBreak_시간";
						lc.pszText = (LPSTR)(LPCTSTR)strTime;
					}
					else
					{
						strTime.LoadString(IDS_TIME);
						lc.pszText = (LPSTR)(LPCTSTR)strTime;
					}
				}
				else
				{
					if(strPacketName.Find("PF_") >= 0)
					{
						strDate.LoadString(IDS_DATE);
						strDate = "P&F_" + strDate;
						// lc.pszText = "PF_일자";
						lc.pszText = (LPSTR)(LPCTSTR)strDate;
					}
					else if(strPacketName.Find("ThreeLineBreak_") >= 0)
					{
						strDate.LoadString(IDS_DATE);
						CString strThreeLineBreak;
						strThreeLineBreak.LoadString( IDS_THREE_LINE_BREAK);
						strDate = strThreeLineBreak + "_" + strDate;
						// lc.pszText = "ThreeLineBreak_일자";
						lc.pszText = (LPSTR)(LPCTSTR)strDate;
					}
					else
					{
						strDate.LoadString(IDS_DATE);
						// lc.pszText = "일자";
						lc.pszText = (LPSTR)(LPCTSTR)strDate;
					}
				}
			}
			// 2011.01.24 by SYS <<

			//end
			lc.iSubItem = j ;
			j++;
			//sy 2005.08.25. -> type 길이에 따라 폭을 정해준다.

			// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			if(strType == "YYYYMMDDHHMMSS")
			{
				CString strFormat = _T("YYYYMMDD");	// 디폴트포맷
				
				// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
				switch(eDateUnit)
				{
				case CScaleBaseData::HORZ_DAILY:		// 일
				case CScaleBaseData::HORZ_WEEKLEY:		// 주 
					strFormat = _T("YYYYMMDD");		
					break;
				case CScaleBaseData::HORZ_MONTHLY:		// 월 
					strFormat = _T("YYYYMM");		
					break;
				case CScaleBaseData::HORZ_TIME_ONE:		// 분
				case CScaleBaseData::HORZ_TIME_TWO:
					strFormat = _T("MMDDHHMM");
					break;
				case CScaleBaseData::HORZ_TICK:			// 틱 
				case CScaleBaseData::HORZ_TIME_SECOND:
					strFormat = _T("DDHHMMSS");
					break;
				}

				lc.cx = (strFormat.GetLength() * 9) + 10;
			}
			else
			{
				lc.cx = (strType.GetLength() * 9) + 10;
			}
			// 2011.01.24 by SYS <<

			//if(strType == "YYYYMMDDHHMMSS")
			//	lc.cx = (8 * 9) + 10;
			//else
			//	// 2011.01.06 by SYS <<
			//	lc.cx = (strType.GetLength() * 9) + 10;

			//20081025 이문수 >>
			CSize szTitle;
			CString strTitle= lc.pszText;
			szTitle			= pDC->GetTextExtent(strTitle);
			lc.cx			= (lc.cx < szTitle.cx) ? szTitle.cx : lc.cx;
			//20081025 이문수 <<

			m_ctlList.InsertColumn(j, &lc) ;
			nStart  ++;
		}
		else
		{
			// (2008/10/28 - Seung-Won, Bae) for MultiLanguage
			strPacketName = _MTOL0( strPacketName);

			lc.iSubItem = j ;
			j++;
			lc.cx = 67;
			lc.pszText = (LPSTR)(LPCTSTR)strPacketName;

			//20081025 이문수 >>
			CSize szTitle;
			CString strTitle= lc.pszText;
			szTitle			= pDC->GetTextExtent(strTitle);
			lc.cx			= (lc.cx < szTitle.cx) ? szTitle.cx : lc.cx;
			//20081025 이문수 <<

			m_ctlList.InsertColumn(j, &lc) ;
			nStart  ++;
		}

		pPacketheader->Release();
	}

	for(int i = nStart ; i < ntitleCount + nStart ; i++)
	{
		lc.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH ;
		lc.fmt = LVCFMT_RIGHT ; 
		CString maintitle = CDataConversion::GetStringData(szTitles, ";");

		maintitle.TrimLeft(); maintitle.TrimRight();		
		CString strP = CDataConversion::GetStringData(szT, ";");

		//nami 11.15
		if(strP.Find("Zig_Index") >= 0)
			strP.Replace("Zig_Index,", "");

		if(strP.Find("CLC_포인트") >= 0)
			strP.Replace("CLC_포인트,", "");

		CString strUIText;
		int nOneRecordCount = CDataConversion::GetStringDataCount(strP, ",") ;
		if(nOneRecordCount == 1 && !maintitle.IsEmpty())
		{
			//lc.pszText = (LPSTR)(LPCTSTR)maintitle;
			strUIText = _MTOL1_N( maintitle);
			if( strUIText.IsEmpty()) strUIText = _MTOL3( maintitle);
			lc.pszText = (LPSTR)(LPCTSTR)strUIText;

			lc.iSubItem = j ;
			j++;
			lc.cx = width ;
			
			//20081025 이문수 >>
			CSize szTitle;
			CString strTitle= lc.pszText;
			szTitle			= pDC->GetTextExtent(strTitle);
			lc.cx			= (lc.cx < szTitle.cx) ? szTitle.cx : lc.cx;
			//20081025 이문수 <<

			m_ctlList.InsertColumn(j, &lc) ;
		}
		else
		{
			for(int p = 0 ; p < nOneRecordCount ; p++)
			{
				CString strName = CDataConversion::GetStringData(strP, ",") ;
				//sy 2003.10.30. "자료일자" 부분이 먼저 들어가기 때문에 생략한다.
				//if(strName.Find("_자료일자") >= 0)
				if( strName.Find(_MTEXT(C0_DATE_TIME)) >= 0 && m_strHorzScale.Find(_MTEXT(C0_DATE_TIME)) >= 0
					|| strName == m_strHorzScale)
					continue;

				// (2008/3/21 - Seung-Won, Bae) Check Dynamic Packet Name
				// 1. Rainbow Chart
				int nPrefixLen = 0;
				if( !strncmp( strName, _MTEXT( C0_RAINBOW_PACKET), nPrefixLen = strlen( _MTEXT( C0_RAINBOW_PACKET))))
					strName = _LTEXT0( C0_RAINBOW_PACKET) + strName.Mid( nPrefixLen);
				else
				// 2. Box Chart
				if( !strncmp( strName, _MTEXT( C2_BOX_CHART), nPrefixLen = strlen( _MTEXT( C2_BOX_CHART))))
						strName = CString( _LTEXT2( C2_BOX_CHART)) + "_" + _MTOL0( strName.Mid( nPrefixLen));
				else
				// 3. Others
					strName = _MTOL0( strName);

				lc.pszText = ( LPSTR)( LPCTSTR)strName;
				lc.iSubItem = j ;
				j++;
				lc.cx = width ;

				//20081025 이문수 >>
				CSize szTitle;
				CString strTitle= lc.pszText;
				szTitle			= pDC->GetTextExtent(strTitle);
				lc.cx			= (lc.cx < szTitle.cx) ? szTitle.cx : lc.cx;
				//20081025 이문수 <<

				m_ctlList.InsertColumn(j, &lc) ;
			}
		}
	}

	ReleaseDC(pDC);

	//20081025 이문수 <<
}

void CDlgDataListViewer::InsertDatatoList(const CString& strTitles, const CString& strPacketNames, const CString& strPacketNamesPerBlock)
{
	if(strPacketNames.IsEmpty()) return ;
	CString szPacketNames = strPacketNames;

	int j = 0, nStart = 0 ;
	
	//20081106 이문수 >>
	CString sRQs = m_strRQs;
	CString sRQ  = CDataConversion::GetStringData(sRQs, ";");
	IPacket * pPacketheader = m_pPacketList->GetPacket( sRQ, m_strHorzScale);
	//IPacket * pPacketheader = m_pPacketList->GetPacket( m_strHorzScale);
	//20081106 이문수 <<

	if( pPacketheader)
	{
		nStart++ ;
		m_bPacketHeader = TRUE;
	}
	// insert data value
	CString strPT = strPacketNames;
	int nPacketCount = CDataConversion::GetStringDataCount(szPacketNames, ",");

	//20081106 이문수 >>
	int nRowCount = pPacketheader->GetDataCount();
	//nRowCount = m_pPacketList->GetMaximumDataCount( szPacketNames);
	//20081106 이문수 <<

	// row
	// 1st column data
	m_nPacketCount = nPacketCount;
	m_nRowCount = nRowCount;

	//일목균형표의 선행지표1 및 선행지표2 표시되도록 처리.
	int nShowRowCount = nRowCount;

	CData* pData = NULL;
	if(pPacketheader)
	{
		pData = new CData;
		ILPCSTR szPacketType = pPacketheader->GetType();
		pData->m_strPacketType = szPacketType;
		pData->m_lDataCount = pPacketheader->GetDataCount();

		if( pData->m_strPacketType == _MTEXT(C6_CHARACTER))
		{
			if( pData->m_pistrDataList) pData->m_pistrDataList->Release();
			pData->m_pistrDataList = pPacketheader->GetStrDataList();
		}
		else
		{
			if( pData->m_pidDataList) pData->m_pidDataList->Release();
			pData->m_pidDataList = pPacketheader->GetDataList();
			for(j = 0; j < m_nRowCount; j++)
			{
				double value = 0.0;
				pPacketheader->GetData(j, value);
				//sy 2004.2.16.
				//-> 미래 데이터를 가져온다. 일목균형표와 같은 경우 필요함.
				if(value == ERRORVALUE || value == -ERRORVALUE)
					m_nFutureDateCount++;
			}
		}

		m_DataList.AddTail(pData);
		pPacketheader->Release();
	}
	
	//{{2007.05.16 by LYH 복수종목 적용	
	CString strRQs = m_strRQs;
	CString strPacketName, strRQ;
	//}}
	for(int i = nStart ; i < nPacketCount + nStart ; i++)
	{
		strPacketName = CDataConversion::GetStringData(strPT, ",");

		//sy 2003.10.30. "자료일자" 부분이 먼저 들어가기 때문에 생략한다.
		//if(strName.Find("_자료일자") >= 0)
		if( strPacketName.Find(_MTEXT(C0_DATE_TIME)) >= 0 && m_strHorzScale.Find(_MTEXT(C0_DATE_TIME)) >= 0
			|| strPacketName == m_strHorzScale)
		{
			i--;
			continue;
		}

		if((strPacketName.Find("Zig_Index") >= 0) || (strPacketName.Find("CLC_포인트") >= 0))
		{
			i--;
			continue;
		}
		//{{2007.05.16 by LYH 복수종목 적용	
		//IPacket * pPacket = m_pPacketList->GetPacket(strPacketName);
		strRQ = CDataConversion::GetStringData(strRQs, ";");
		IPacket * pPacket = m_pPacketList->GetPacket(strRQ, strPacketName);
		//}}
		if(!pPacket) continue;

		pData = new CData;
		ILPCSTR szPacketType = pPacket->GetType();
		pData->m_strPacketType = szPacketType;
		pData->m_strPacketName = strPacketName;
		pData->m_lDataCount = pPacket->GetDataCount();

		nShowRowCount = max(nShowRowCount, pData->m_lDataCount);	//일목균형표의 선행지표1 및 선행지표2 표시되도록 처리.

		if( pData->m_strPacketType == _MTEXT(C6_CHARACTER))
		{
			if( pData->m_pistrDataList) pData->m_pistrDataList->Release();
			pData->m_pistrDataList = pPacket->GetStrDataList();
		}
		else
		{
			if( pData->m_pidDataList) pData->m_pidDataList->Release();
			pData->m_pidDataList = pPacket->GetDataList();
		}

		m_DataList.AddTail(pData);
		pPacket->Release();
	}

	//일목균형표의 선행지표1 및 선행지표2 표시되도록 처리. -->
	//m_ctlList.SetItemCountEx(nRowCount);
	m_nFutureDateCount = nShowRowCount - m_nRowCount;
	m_nRowCount = nShowRowCount;	
	m_ctlList.SetItemCountEx(nShowRowCount);
	//<--

	m_ctlList.Invalidate();

}


void CDlgDataListViewer::LoadListFocusMove(int nIndex)
{
	if(nIndex < 0)
		return ;

	m_ctlList.SetItemState(nIndex, 0xFFFF, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctlList.SetSelectionMark(nIndex);

	int pos, j;
	int nPage, nLine;
	SCROLLINFO sInfo;

	m_ctlList.GetScrollInfo(SB_VERT, &sInfo);
	pos = m_ctlList.GetScrollPos(SB_VERT);
	if(pos < nIndex){
		nPage = (nIndex - pos) / sInfo.nPage;
		nLine = (nIndex - pos) % sInfo.nPage;
		for(j=0; j<nPage; j++)
			m_ctlList.SendMessage(WM_VSCROLL, SB_PAGEDOWN, NULL);
		for(j=0; j<nLine; j++)
			m_ctlList.SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
	}
	else{
		nPage = (pos - nIndex) / sInfo.nPage;
		nLine = (pos - nIndex) % sInfo.nPage;
		for(j=0; j<nPage; j++)
			m_ctlList.SendMessage(WM_VSCROLL, SB_PAGEUP, NULL);
		for(j=0; j<nLine; j++)
			m_ctlList.SendMessage(WM_VSCROLL, SB_LINEUP, NULL);
	}
}

void CDlgDataListViewer::MouseMove(int nIndex)
{
	LoadListFocusMove(m_pPacketList->GetMaximumDataCount() - nIndex - 1) ;
}

void CDlgDataListViewer::SetPacketNames(const CString& strTitle,
									    const CString& strPacketNamesPerBlock,
										const CString& strPacketNames)
{
	m_strTitles = strTitle;
	m_strPacketNamesPerBlock = strPacketNamesPerBlock;
	m_strPacketNames = strPacketNames ;
}

void CDlgDataListViewer::OnExcelFileExport() 
{
	UpdateData();
	LoadExcel();
}

void CDlgDataListViewer::LoadExcel()
{
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	CString  szFilePath ;
	//szFilePath.Format("%sChartData_%04d%02d%02d_%02d%02d%02d.xls", m_strFilePath, 
	szFilePath.Format("%sChartData_%04d%02d%02d_%02d%02d%02d.nc2", m_strFilePath, 
				sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour+9, sTime.wMinute, sTime.wSecond);

	// (2008/8/29 - Seung-Won, Bae) for exception in Non-Excel system.
	CFile file;
	CFileException e;
	if( !file.Open( szFilePath, CFile::modeCreate | CFile::modeWrite, &e ) )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return;
	}

	CString szData, szTmp;
	
	CPoint ptRowAndCol = GetRowandColumnCount();

	// header column
	szData.Empty();
	CHeaderCtrl* pHeader = NULL;
	pHeader = m_ctlList.GetHeaderCtrl();
	ptRowAndCol.y = pHeader->GetItemCount();
	for(int col = 0 ; col < ptRowAndCol.y ; col++) 
	{
		HDITEM hdi;
		TCHAR  lpBuffer[256];

		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;
		pHeader->GetItem(col, &hdi);
		szTmp = hdi.pszText;
		szData += szTmp;
		szData += _T("\t");
	}
	szData += _T("\r\n");
	file.Write(szData, szData.GetLength());

	for(int row = 0  ; row <= ptRowAndCol.x ; row++) 
	{
		szData.Empty();

		for(int col = 0 ; col < ptRowAndCol.y ; col++) 
		{
			szTmp = m_ctlList.GetItemText(row, col);
			szData += szTmp;
			szData += _T("\t");
		}
		szData +=_T("\r\n");
		file.Write(szData, szData.GetLength());
	}
	file.Close();

	CString szExcel = "NO" ;
	CString strKey = "Excel.csv\\shell\\open\\command" ;
	int Ver = 60 ;
	HKEY hkey = HKEY_CLASSES_ROOT ;
	szExcel = GetStrValue(hkey, strKey, "", "NO") ;
	if( strcmp( szExcel, "NO"))
	{
		szExcel.MakeLower() ;
		int start = szExcel.Find("excel.exe") ;
		szExcel = szExcel.Left(start + 9) ;
		if(szExcel.Left(1).Compare("\"") == 0)
			szExcel = szExcel.Right(szExcel.GetLength() - 1) ;

		// 04.17.2002 파일명 앞뒤에 따옴표(" ")가 있어야 한다
		szExcel = szExcel + " \"" + szFilePath + "\"";	

		if(::WinExec((LPCTSTR)szExcel, SW_SHOW) > 31)
			return;
	}

	CString szMsg, strFileSave1, strFileSave2;
	strFileSave1.LoadString(IDS_FILESAVE1);
	strFileSave2.LoadString(IDS_FILESAVE2);
	szMsg.Format("%s \"%s\"%s", strFileSave1, szFilePath, strFileSave2);
	AfxMessageBox( szMsg )	;//, "Chart") ;
	return;
}

void CDlgDataListViewer::LoadExcel2()
{
#if 0
	SYSTEMTIME sTime;
	GetSystemTime(&sTime);

	CString  szFilePath ;
	szFilePath.Format("%sChartData_%04d%02d%02d_%02d%02d%02d.xls", m_strFilePath, 
				sTime.wYear, sTime.wMonth, sTime.wDay, sTime.wHour+9, sTime.wMinute, sTime.wSecond);

	CString szExcel = "NO" ;
	CString strKey = "Excel.csv\\shell\\open\\command" ;
	int Ver = 60 ;
	HKEY hkey = HKEY_CLASSES_ROOT ;
	szExcel = GetStrValue(hkey, strKey, "", "NO") ;
	szExcel.MakeLower() ;
	int start = szExcel.Find("excel.exe") ;
	szExcel = szExcel.Left(start + 9) ;
	if(szExcel.Left(1).Compare("\"") == 0)
		szExcel = szExcel.Right(szExcel.GetLength() - 1) ;

	CreateDirectory("C:\\Temp", NULL) ; 
	CString strFileName = szFilePath;
	BasicExcel e;

	e.New(1);	// sheet 하나 짜리 생성 

	CString szData;
 	CPoint ptRowAndCol = GetRowandColumnCount();

	BasicExcelWorksheet* sheet1 = e.GetWorksheet("Sheet1");
	if (sheet1)
	{
		szData.Empty();
		CHeaderCtrl* pHeader = NULL;
		pHeader = m_ctlList.GetHeaderCtrl();
		ptRowAndCol.y = pHeader->GetItemCount();
		for(int col = 0 ; col < ptRowAndCol.y ; col++) 
		{
			HDITEM hdi;
			TCHAR  lpBuffer[256];

			hdi.mask = HDI_TEXT;
			hdi.pszText = lpBuffer;
			hdi.cchTextMax = 256;
			pHeader->GetItem(col, &hdi);
			szData = hdi.pszText;

			// ANSI to UNICODE
			int nLen = MultiByteToWideChar(CP_ACP, 0,hdi.pszText, -1, NULL, NULL);
			LPWSTR lpszW = new WCHAR[nLen];
			MultiByteToWideChar(CP_ACP, 0, hdi.pszText, -1, lpszW, nLen);

			sheet1->Cell(0,col)->SetWString(lpszW);
		}

		for(int row = 1  ; row <= ptRowAndCol.x ; row++) 
		{
			for(int col = 0 ; col < ptRowAndCol.y ; col++) 
			{
				szData = m_ctlList.GetItemText(row, col);

// 				if(pIPacket->GetType() == "문자")
// 				{
// 					// ANSI to UNICODE
// 					int nLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, NULL, NULL);
// 					LPWSTR lpszW = new WCHAR[nLen];
// 					MultiByteToWideChar(CP_ACP, 0, szData, -1, lpszW, nLen);
// 					sheet1->Cell(row,col)->SetString(lpszW);
// 				}
// 				else
				{
					// ANSI to UNICODE
					int nLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, NULL, NULL);
					LPWSTR lpszW = new WCHAR[nLen];
					if(szData.Find("/") >= 0)
					{
						//	szData.Replace("/","-");
						MultiByteToWideChar(CP_ACP, 0, szData, -1, lpszW, nLen);
						sheet1->Cell(row,col)->SetWString(lpszW);
					}
					else
					{
						sheet1->Cell(row,col)->SetDouble(atof(szData));
						MultiByteToWideChar(CP_ACP, 0, szData, -1, lpszW, nLen);
					}
				}
			}
		}		
	}

	if(!e.SaveAs(szFilePath.GetBuffer(szFilePath.GetLength()))) return;

	// 04.17.2002 파일명 앞뒤에 따옴표(" ")가 있어야 한다
	szExcel = szExcel + " \"" + szFilePath + "\"";	

	if(::WinExec((LPCTSTR)szExcel, SW_SHOW) > 31)
		return;
	else	
	{
		CString szMsg, strFileSave;
		// szMsg.Format("이 파일은 \"%s\"라는 파일명으로 저장되어 있습니다.",szFilePath);
		szMsg.Format("%s \"%s\"%s", strFileSave.LoadString(IDS_FILESAVE1), szFilePath, strFileSave.LoadString(IDS_FILESAVE2));
		MessageBox(szMsg,"nfxChart") ;
		return;
	}
#endif
	return;
}

CString CDlgDataListViewer::GetStrValue(HKEY Hkey,CString strkey, CString strsubkey,CString strRValue)
{
	LONG err;
   HKEY openKey;
   BYTE Data[128];
   DWORD type;
   DWORD size=sizeof(Data);

   err=RegOpenKey(Hkey,strkey,&openKey);

   if(ERROR ==err)
	   err=RegQueryValueEx(openKey,strsubkey,0,&type,Data,&size);
	 
   if(err==ERROR_SUCCESS)
	{
	   RegCloseKey(openKey);
		CString str(Data);
		return str;
	}
	
   RegCloseKey(openKey);
	return strRValue;

}


CPoint CDlgDataListViewer::GetRowandColumnCount()
{
	int nColumnCount = CDataConversion::GetStringDataCount(m_strTitles, ";");
	int nRowCount = 0;

	if(!m_pPacketList)	return CPoint(0, nColumnCount);

	//20081106 이문수 >>
	//IPacket * pPacketheader = m_pPacketList->GetPacket(m_strHorzScale);
	CString strRQs = m_strRQs;
	CString strRQ  = CDataConversion::GetStringData(strRQs, ";");
	IPacket * pPacketheader = m_pPacketList->GetPacket(strRQ, m_strHorzScale);
	//20081106 이문수 <<

	if( pPacketheader)
	{
		nColumnCount ++;
		nRowCount = pPacketheader->GetDataCount();
		pPacketheader->Release();

		nRowCount += m_nFutureDateCount;	//일목균형표의 선행지표1 및 선행지표2 표시되도록 처리.
	}
	else	{
		CString strT = m_strPacketNames;
		CString strPacketName = CDataConversion::GetStringData(strT, ",");

		//20081106 이문수 >>
		IPacket * pPacket = m_pPacketList->GetPacket(strRQ, strPacketName);	
		//IPacket * pPacket = m_pPacketList->GetPacket(strPacketName);	
		//20081106 이문수 <<
		if( pPacket)
		{
			nRowCount = pPacket->GetDataCount();
			pPacket->Release();
		}
	}

	return CPoint(nRowCount, nColumnCount);
}

void CDlgDataListViewer::OnClose() 
{
	if(m_pDataListViewMngWnd && IsWindow(m_pDataListViewMngWnd->GetSafeHwnd()))
		m_pDataListViewMngWnd->PostMessage(UDM_DATALISTVIEWER_CLOSE);
}


void CDlgDataListViewer::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we set this to something else below.
    *pResult = 0;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		*pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
    {
		COLORREF crBkgnd;
		if((pLVCD->nmcd.dwItemSpec % 2) == 0)
			crBkgnd = RGB(255,255,255);
		else
			crBkgnd = RGB(237,241,245);

		pLVCD->clrTextBk = crBkgnd;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
	}
}

HBRUSH CDlgDataListViewer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_BTN)	{
		pDC->SetBkMode(TRANSPARENT) ;		
		return (HBRUSH)m_brBkColor ;
	}

	pDC->SetBkMode(TRANSPARENT) ;		
	return (HBRUSH)m_brBkColor ;

	return hbr;
}

void CDlgDataListViewer::RealUpdate()
{
	if(!m_pPacketList)
		return;

	//sy 2003.10.30.
	int nMaxPacketCount = GetMaxCount();
	if(nMaxPacketCount <= 0)
		return;

	int nListCount = m_ctlList.GetItemCount();
	if(nListCount == (nMaxPacketCount+m_nFutureDateCount))
	{
		int nUpdateIndex = m_pPacketList->GetLastestRealIndex();
		if(nUpdateIndex == -1)
			nUpdateIndex = nMaxPacketCount-1;
		UpdateRecord(nUpdateIndex);
	}
	else
	{
		// (2006/12/29 - Seung-Won, Bae) Update Previous Time Closing
		if( 0 < nListCount) UpdateRecord( nListCount - 1);
		InsertRecord( nMaxPacketCount);
	}
}

void CDlgDataListViewer::InsertRecord(int nIndex)
{
	POSITION posSelect = m_ctlList.GetFirstSelectedItemPosition();
	int nSelect = -1;
	if( posSelect) nSelect = m_ctlList.GetNextSelectedItem( posSelect);
	m_ctlList.SetItemCountEx( nIndex);
	if( 0 <= nSelect) m_ctlList.SetItemState( nSelect + nIndex - m_nRowCount, 0xFFFF, LVIS_SELECTED | LVIS_FOCUSED);
	m_ctlList.Invalidate(FALSE);
	m_nRowCount = nIndex;
}

void CDlgDataListViewer::UpdateRecord(int nIndex)
{
//	int nListCount = m_ctlList.GetItemCount();
//	m_ctlList.SetItemState(nListCount - nIndex - 1, 0xFFFF, LVIS_SELECTED | LVIS_FOCUSED);
//	m_ctlList.SetSelectionMark(nListCount - nIndex - 1);
	m_ctlList.Invalidate(FALSE);
}
// void CDlgDataListViewer::InsertRecord(int nIndex)
// {
// 	CString strPT = m_strPacketNames;
// 	int nPacketCount = CDataConversion::GetStringDataCount(strPT, ",");
// 	//sy 2003.10.30.
// 	int nMaxPacketCount = GetMaxCount();
// 	if(nMaxPacketCount <= 0)
// 		return;
// 
// 	IPacket * pPacketheader = m_pPacketList->GetPacket(m_strHorzScale);
// 	if(pPacketheader == NULL)
// 		pPacketheader = m_pPacketList->GetPacket("Text");
// 
// 	int nStart = 0 ;
// 	int j = 0;
// 
// 	if(pPacketheader)
// 	{
// 		for(int row = nIndex; row < nMaxPacketCount; row++)
// 		{
// 			//sy 2003.10.30. -> "PF_자료일자"일 경우 처리
// 			/*if(m_strHorzScale.Compare("자료일자") == 0) {
// 				strv = CDataConversion::DoubleToString(pPacketheader->GetData(row));
// 				strv = CDataConversion::GetStringFormatTrans(strv, pPacketheader->GetType(), "");
// 			}
// 			else
// 				strv = CDataConversion::DoubleToString(pPacketheader->GetData(row));
// 			*/
// 			double dData = pPacketheader->GetData(row);
// 			if(dData == ERRORVALUE || dData == -ERRORVALUE)
// 				continue;
// 
// 			CString strv = CDataConversion::DoubleToString(dData);
// 			if(m_strHorzScale.Find("자료일자") >= 0) {
// 				strv = CDataConversion::GetStringFormatTrans(strv, pPacketheader->GetType(), "");
// 			}
// 			//end
// 			m_ctlList.InsertItem(0, strv);
// 		}
// 		nStart++;
// 
// 		pPacketheader->Release();
// 	}
// 	
// 	for(int row = nIndex; row < nMaxPacketCount; row++)
// 	{
// 		for(int i = nStart ; i < nPacketCount + nStart; i++)
// 		{
// 			IPacket * pPacket = m_pPacketList->GetPacket(CDataConversion::GetStringData(strPT, ","));
// 			if(!pPacket)	continue;
// 
// 			// (2007/2/3 - Seung-Won, Bae) Skip Indent Data
// 			if( i != 0 && row < pPacket->GetIndexOfRealDataStart()) continue;
// 
// 			if(pPacket->GetType() != "문자")
// 			{
// 				//sy 2003.10.30. data가 잘못 된경우
// 				double dData = pPacket->GetData(nIndex);
// 				if(dData != ERRORVALUE && dData != -ERRORVALUE)
// 				{
// 					CString strv = CDataConversion::DoubleToString(dData);
// 					strv = CDataConversion::GetStringFormatTrans(strv, pPacket->GetType());
// 					if(i == 0)
// 						m_ctlList.InsertItem(0, strv);
// 					else
// 						m_ctlList.SetItemText(0, i, strv);
// 				}
// 			}
// 			else 	{
// 				CString strv;
// 				pPacket->GetData(nIndex, strv);
// 				if(i == 0)
// 					m_ctlList.InsertItem(0, strv);
// 				else
// 					m_ctlList.SetItemText(0, i, strv);
// 			}
// 
// 			pPacket->Release();
// 		}
// 	}
// }
// 
// void CDlgDataListViewer::UpdateRecord(int nIndex)
// {
// 	CString strPT = m_strPacketNames;
// 	int nPacketCount = CDataConversion::GetStringDataCount(strPT, ",");
// 	int nListCount = m_ctlList.GetItemCount();
// 
// 	IPacket * pPacketheader = m_pPacketList->GetPacket(m_strHorzScale);
// 	if(pPacketheader == NULL)
// 		pPacketheader = m_pPacketList->GetPacket("Text");
// 
// 	int nStart = 0 ;
// 	POSITION pos2, pos;
// #if 1
// 	if(pPacketheader)
// 	{
// 		pos = m_DataList.FindIndex(nStart);
// 		CData* pData;	
// 		if(pos)
// 		{
// 			pData = (CData*)m_DataList.GetAt(pos);
// 			pos2 = pData->m_pdDataList->FindIndex(0);
// 			pData->m_pdDataList->SetAt(pos2, pPacketheader->GetData(nIndex));
// 		}
// //		m_ctlList.SetItemText( nListCount - nIndex - 1, 0, strv);
// 		nStart = 1;
// 
// 		pPacketheader->Release();
// 	}
// 
// 	for(int i = nStart ; i < nPacketCount + nStart; i++)
// 	{
// 		IPacket * pPacket = m_pPacketList->GetPacket( CDataConversion::GetStringData(strPT, ","));
// 		if(!pPacket) continue;
// 
// 		// (2007/2/3 - Seung-Won, Bae) Skip Indent Data
// 		if( i != 0 && nIndex < pPacket->GetIndexOfRealDataStart()) continue;
// 
// 		if( pPacket->GetType() != "문자")	{
// 			//sy 2004.05.20. -> data가 잘못된 경우
// 			double dData = pPacket->GetData(nIndex);
// 			if( dData != ERRORVALUE && dData != -ERRORVALUE)
// 			{
// 				pos = m_DataList.FindIndex(nStart);
// 				CData* pData;	
// 				if(pos)
// 				{
// 					pData = (CData*)m_DataList.GetAt(pos);
// 					pos2 = pData->m_pdDataList->FindIndex(0);
// 					pData->m_pdDataList->SetAt(pos2, dData);
// 				}
// 			}
// 			TRACE1("\n\n\t\treal data = %f\n\n",dData);
// 		}
// 		else
// 		{
// 			CString strv;
// 			pPacket->GetData(nIndex, strv);
// 			pos = m_DataList.FindIndex(nStart);
// 			CData* pData;	
// 			if(pos)
// 			{
// 				pData = (CData*)m_DataList.GetAt(pos);
// 				pos2 = pData->m_pstrDataList->FindIndex(0);
// 				pData->m_pstrDataList->SetAt(pos2, strv);
// 			}
// 		}
// 
// 		pPacket->Release();
// 	}
// #endif
// 	m_nHighlightListItem = nIndex;
// 	m_ctlList.SetItemState((!m_bReverse)?0:nListCount - 1 , 0xFFFF, LVIS_SELECTED | LVIS_FOCUSED);
// 	m_ctlList.SetSelectionMark((!m_bReverse)?0:nListCount - 1);
// //	m_ctlList.SetItemCountEx(nRowCount);
// 	m_ctlList.Invalidate();
// 
// 
// }

//nami 8.31
bool CDlgDataListViewer::IsAllZeroData(CList<double, double>* datalist)
{
	POSITION pos = datalist->GetHeadPosition();
	while(pos) {
		double data = datalist->GetNext(pos);
		if(data) 
			return false;
	}
	return true;
}

void CDlgDataListViewer::OnUpdate() 
{
	m_ctlList.DeleteAllItems();

	InsertDatatoList(m_strTitles, m_strPacketNames, m_strPacketNamesPerBlock);	
}

//sy 2003.10.30. 실질적인 maxCount를 구한다.
int CDlgDataListViewer::GetMaxCount() const
{
	if(m_pPacketList == NULL)
		return 0;

	//20081106 이문수 >>
	CString strRQs = m_strRQs, strRQ;
	strRQ = CDataConversion::GetStringData(strRQs, ";");
	IPacket * pPacketheader = m_pPacketList->GetPacket( strRQ, m_strHorzScale);
	if( !pPacketheader)
		return 0;
	
	int nMaxPacketCount = pPacketheader->GetDataCount();
	//int nMaxPacketCount = m_pPacketList->GetMaximumDataCount();
	//20081106 이문수 <<
	
	CString strDateTimePF, strDateThreeLineBreak;
	strDateTimePF.Format("PF_%s", _MTEXT(C0_DATE_TIME));
	strDateThreeLineBreak.Format("ThreeLineBreak_%s", _MTEXT(C0_DATE_TIME));
	// if( m_strHorzScale.Find("PF_자료일자") >= 0 || m_strHorzScale.Find( "ThreeLineBreak_자료일자") >= 0)
	if( m_strHorzScale.Find(strDateTimePF) >= 0 || m_strHorzScale.Find(strDateThreeLineBreak) >= 0)
	{
		//20081106 이문수 >>
		//IPacket * pPacketheader = m_pPacketList->GetPacket(m_strHorzScale);
		IPacket * pPacketheader = m_pPacketList->GetPacket(strRQ, m_strHorzScale);
		//20081106 이문수 <<
		if(pPacketheader != NULL)
		{
			nMaxPacketCount = pPacketheader->GetDataCount();
			pPacketheader->Release();
		}
	}
	return nMaxPacketCount;
}

//sy 2004.2.16. 오늘날짜의 위치로 이동시킨다.
void CDlgDataListViewer::EnsureVisible()
{
	if(m_nFutureDateCount <= 0)
		return;

	m_ctlList.EnsureVisible(m_nFutureDateCount, false);
	int nLastIndex = m_nFutureDateCount + m_ctlList.GetCountPerPage() -1;
	m_ctlList.EnsureVisible(nLastIndex, false);
}
//sy end

void CDlgDataListViewer::OnClipboard() 
{
	CString source = _T("");

	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;

	int nPacketCount = CDataConversion::GetStringDataCount(m_strPacketNames, ",");
	int nRowCount = m_pPacketList->GetMaximumDataCount( m_strPacketNames);

		for(int i=0; i<nRowCount; i++ )
		{
			for(int j=0; j<nPacketCount+1; j++)
			{
				source += m_ctlList.GetItemText(i, j) + _T("     ");
			}
			source += _T("\r\n");
		}

		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(source));
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}		
}

void CDlgDataListViewer::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	POSITION pos, pos2;
	double value;
	CString strPT = m_strPacketNames;

	pos = m_DataList.FindIndex(pItem->iSubItem);
	if(!pos)
	{
		*pResult = 0;
		return;
	}
	
	CData* pData = (CData*)m_DataList.GetAt(pos);
	if(!pData)
	{
		*pResult = 0;
		return;
	}
	int nRowIndex = m_bReverse?pItem->iItem:m_nRowCount - pItem->iItem - 1;

	//20081106 이문수 >>
	CString strRQs = m_strRQs, strRQ;
	strRQ = CDataConversion::GetStringData(strRQs, ";");
	IPacket * pPacket = m_pPacketList->GetPacket(strRQ, pData->m_strPacketName);
	//IPacket * pPacket = m_pPacketList->GetPacket(pData->m_strPacketName);
	//20081106 이문수 <<
	if(pPacket)
	{
		// (2007/2/3 - Seung-Won, Bae) Skip Indent Data
		int nStartIndex = pPacket->GetIndexOfRealDataStart();
		pPacket->Release();
		if(nRowIndex < nStartIndex)
		{
			*pResult = 0;
			return;
		}
	}

	if(pItem->iItem == 0)
		pItem->state = LVIS_SELECTED;
	if (pItem->mask & LVIF_TEXT)
	{
		if(m_bPacketHeader && (pItem->iSubItem == 0))	// header가 존재할 경우
		{
			//20081106 이문수 >>
			IPacket * pPacketheader = m_pPacketList->GetPacket( strRQ, m_strHorzScale);
			//IPacket * pPacketheader = m_pPacketList->GetPacket( m_strHorzScale);
			//20081106 이문수 <<

			// if(pData->m_strPacketType == "문자")
			if(pData->m_strPacketType == _MTEXT(C6_CHARACTER))
			{
				pos2 = pData->m_pistrDataList->FindIndex(nRowIndex);
				if(!pos2)
				{
					*pResult = 0;
					return;
				}
				
				ILPCSTR szStr;
				pData->m_pistrDataList->GetAt( pos2, szStr);
				lstrcpy(pItem->pszText, szStr);
			}
			else
			{
				pos2 = pData->m_pidDataList->FindIndex(nRowIndex);
				if(pos2) pData->m_pidDataList->GetAt( pos2, value);

				//-> 미래 데이터를 가져온다. 일목균형표와 같은 경우 필요함.
				double value1 = 0.0;
				pPacketheader->GetData( nRowIndex, value1);
				//sy 2004.2.16.
				//-> 미래 데이터를 가져온다. 일목균형표와 같은 경우 필요함.
				if(value1 == ERRORVALUE || value1 == -ERRORVALUE)
					pPacketheader->GetDataPastandFuture( nRowIndex, value);

				if(value != ERRORVALUE && value != -ERRORVALUE)
				{
					//sy 2003.10.30. -> "PF_자료일자"일 경우 처리
					CString strv = CDataConversion::DoubleToString(value);
					//if((m_strHorzScale.Find("자료일자") >= 0) && m_bPacketHeader && (pItem->iSubItem == 0))

					// 2011.01.24 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
					if (pData->m_strPacketType == _T("YYYYMMDDHHMMSS"))
					{
						// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
						CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacketheader->GetDateUnitType();

						CString strFormat = _T("YYYYMMDD");	// 디폴트포맷
						switch(eDateUnit)
						{
						case CScaleBaseData::HORZ_DAILY:		// 일
						case CScaleBaseData::HORZ_WEEKLEY:		// 주 
							strFormat = _T("YYYYMMDD");		
							break;
						case CScaleBaseData::HORZ_MONTHLY:		// 월 
							strFormat = _T("YYYYMM");		
							break;
						case CScaleBaseData::HORZ_TIME_ONE:		// 분
						case CScaleBaseData::HORZ_TIME_TWO:
							strFormat = _T("MMDDHHMM");
							break;
						case CScaleBaseData::HORZ_TICK:			// 틱 
						case CScaleBaseData::HORZ_TIME_SECOND:
							strFormat = _T("DDHHMMSS");
							break;
						}

						if((m_strHorzScale.Find(_MTEXT(C0_DATE_TIME)) >= 0) && m_bPacketHeader && (pItem->iSubItem == 0))
							strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, strFormat, "");
						else
							strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, strFormat);
						//strv = CommaStr(strv);
					}
					else
					{
						if((m_strHorzScale.Find(_MTEXT(C0_DATE_TIME)) >= 0) && m_bPacketHeader && (pItem->iSubItem == 0))
							strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, pData->m_strPacketType, "");
						else
							strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, pData->m_strPacketType);
							//strv = CommaStr(strv);
					}
					// 2011.01.24 by SYS <<
					lstrcpy(pItem->pszText, strv);

				}
				else
				{
					lstrcpy(pItem->pszText, "N/A");
				}
			}
			pPacketheader->Release();
		}
		else
		{
			CString strPacketName = CDataConversion::GetStringData(strPT, ",");

			if((strPacketName.Find("Zig_Index") >= 0) || (strPacketName.Find("CLC_포인트") >= 0))
			{
				*pResult = 0;
				return;
			}

			// if(pData->m_strPacketType == "문자")
			if(pData->m_strPacketType == _MTEXT(C6_CHARACTER))
			{
				pos2 = pData->m_pistrDataList->FindIndex(nRowIndex);
				if(!pos2)
				{
					*pResult = 0;
					return;
				}
				ILPCSTR szStr;
				pData->m_pistrDataList->GetAt( pos2, szStr);
				lstrcpy(pItem->pszText, szStr);
			}
			else
			{
				// if( (strPacketName == "대기매물") && (pItem->iItem >= pData->m_lDataCount - 2) )
				if( (strPacketName == _MTEXT( C2_OVERHANGING_SUPPLY)) && (pItem->iItem >= pData->m_lDataCount - 2) )
				{
					*pResult = 0;
					return;
				}
				pos2 = pData->m_pidDataList->FindIndex(nRowIndex);
				if(!pos2)
				{
					*pResult = 0;
					return;
				}
				pData->m_pidDataList->GetAt(pos2, value);
				CString strv = CDataConversion::DoubleToString(value);

				strv = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strv, pData->m_strPacketType);
				//strv = CommaStr(strv);
				lstrcpy(pItem->pszText, strv);
			}
		}
	}

	if (pItem->mask & LVIF_STATE)
	{
		TRACE("test");
	}

	*pResult = 0;
}

void CDlgDataListViewer::OnReverse() 
{
	m_bReverse = !m_bReverse;
	if(m_bReverse)
	{
		CString strRecent;
		strRecent.LoadString(IDS_RECENTLY);
		// m_btnReverse.SetWindowText("최근 일자부터 보임");
		m_btnReverse.SetWindowText(strRecent);
	}
	else
	{
		CString strPrev;
		strPrev.LoadString(IDS_PREVIOUSLY);
		// m_btnReverse.SetWindowText("이전 일자부터 보임");
		m_btnReverse.SetWindowText(strPrev);
	}
	m_ctlList.Invalidate();
}

//////////////////////////////////////////////////////////////////////////
// 숫자현 텍스트에 콤마 넣기 123456 --> 123,456
CString CDlgDataListViewer::CommaStr(CString in)
{
	int NumOfComma, nPoint, nUnderPoint = 0, nLength;
	CString out = in;

	nPoint = out.Find(".",0);	// 소수점 위치
	if(nPoint >= 0)	nUnderPoint = out.GetLength() - nPoint;
	nLength = out.GetLength() - nUnderPoint;

	NumOfComma = (nLength - ((in.Left(1) == "-")?1:0))/ 3;	// 삽입해야할 컴마 수 계산.음수 고려
	if((in.GetLength() - nUnderPoint - ((in.Left(1) == "-")?1:0))% 3 == 0)NumOfComma--;	// 소수점 이하 고려, 있을 경우 1 감소
	for(int i=0;i<NumOfComma;i++)
	{
		out.Insert(nLength - (3*i+3), ",");
	}
	return out;
}
