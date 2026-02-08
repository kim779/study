// GraphColorCollection.cpp: implementation of the CGraphColorCollection class.
//	
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "GraphColorCollection.h"

#include "DlgSubGraphSetting.h"
#include "GraphSettingButton.h"
#include "Conversion.h"
#include "LineColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphColorCollection::CGraphColorCollection( HWND p_hOcxWnd)
{
	// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
	m_hOcxWnd = p_hOcxWnd;

    m_nActiveSubGraphSettingDlg = -1;
    m_Rect.SetRectEmpty();

	// (2006/11/23 - Seung-Won, Bae) Support Scroll in SubGraph Tab Button
	m_bMoreButtons = FALSE;
	m_nSubGraphButtonStartIndex = 0;
}

CGraphColorCollection::~CGraphColorCollection()
{
	DeleteSubGraphSettingDlgandButton();
}

//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////

bool CGraphColorCollection::SetActive(int ix)
{
    if (ix > m_ArraySubGraphSettingDlg.GetUpperBound()) {
        // does not exist
        return(false);
    }
    m_nActiveSubGraphSettingDlg = ix; // make it the active one
    return(true);
}


bool CGraphColorCollection::CreateSubGraphDlg(CString& strSubGraphName, 
											  CString& strSubGraphTitle, 
											  int nType,
											  int nStyle,
											  int nDrawStyle,
											  UINT id, CWnd* pParentWnd)
{
	ML_SET_LANGUAGE_RES();
    CDlgSubGraphSetting* pDlg = new CDlgSubGraphSetting( m_hOcxWnd, pParentWnd);
    ASSERT(pDlg != NULL);
    if (pDlg == NULL) return(false); // exit and tell the world about it
    AddSubGraphSettingDlg(pDlg); // Add it to the internal table
    pDlg->Create(id, pParentWnd);

	CRect r;
    int x,y;
    pDlg->GetWindowRect(&r); // Get the size and position of the dialog in screen coordinates
    x = m_Rect.left - r.left + ((m_Rect.Width() - r.Width()) / 2);
    y = m_Rect.top - r.top + ((m_Rect.Height() - r.Height()) / 2); //3 = 중앙 정렬을 위해.
	pDlg->SetWindowPos(pParentWnd, x, y, r.Width(), r.Height(), SWP_NOZORDER);

	if(strSubGraphName.IsEmpty())
		strSubGraphName = "Graph1";

	pDlg->SetSubGraphName(strSubGraphName);
	pDlg->m_strTitle = strSubGraphTitle;
	pDlg->SetGraphType(nType);
	pDlg->SetGraphStyle(nStyle);
	pDlg->SetDrawStyle(nDrawStyle);

	pDlg->m_GraphStyleCombo.SetCurSel(nStyle);
	pDlg->UpdateData(FALSE);
    return(true); // everything ok
}

bool CGraphColorCollection::ShowSubGraphSettingDlg(int ix)
{
    ASSERT(ix <= m_ArraySubGraphSettingDlg.GetUpperBound());
    if (ix > m_ArraySubGraphSettingDlg.GetUpperBound()) return(false); // does not exist

    CDlgSubGraphSetting* pDlg;
    if (ix != -1) {
        if (m_nActiveSubGraphSettingDlg != -1) {
            pDlg = m_ArraySubGraphSettingDlg.GetAt(m_nActiveSubGraphSettingDlg);
            ASSERT(pDlg != NULL);
            ASSERT(pDlg->m_hWnd != NULL);
            if ((pDlg == NULL) 
            ||  (pDlg->m_hWnd == NULL)) return(false);
            pDlg->ShowWindow(SW_HIDE);
        }
        m_nActiveSubGraphSettingDlg = ix; // this is the active one now;
    }
    else {
        if (m_nActiveSubGraphSettingDlg == -1) {
            m_nActiveSubGraphSettingDlg = 0;
        }
    }
    pDlg = m_ArraySubGraphSettingDlg.GetAt(m_nActiveSubGraphSettingDlg);
    ASSERT(pDlg != NULL);
    ASSERT(pDlg->m_hWnd != NULL);
    if ((pDlg == NULL) 
    ||  (pDlg->m_hWnd == NULL)) return(false);
    pDlg->ShowWindow(SW_SHOW);
    pDlg->Invalidate();
    pDlg->RedrawWindow();
    return(true);
}

bool CGraphColorCollection::CreateButton(int nIndex, CSize FontSize, CString text, CWnd *pParentWnd)
{
	//temp
	if(!text.GetLength())
		text.Format("%s%d", "Graph", nIndex+1);

	int textXsize = (text.GetLength() * FontSize.cx) + 3;
	int textYsize = FontSize.cy;

	CGraphSettingButton* pButton = new CGraphSettingButton(nIndex, pParentWnd);
    ASSERT(pButton != NULL);
    if (pButton == NULL) return(false); // exit and tell the world about it
    AddButton(pButton); // Add it to the internal table

    pButton->Create(text, WS_CHILD | WS_VISIBLE  | BS_FLAT, //BS_OWNERDRAW, | BS_PUSHBUTTON
			CRect(0, 0, textXsize + 5 , textYsize + 5), pParentWnd, nIndex+1); //+5를 하지 않으면 너무 Tight하게 그려짐.

    return(true); // everything ok
}

int CGraphColorCollection::AddButton(CGraphSettingButton *pbutton)
{
	return(m_ArrayButton.Add(pbutton));
}

void CGraphColorCollection::SetButtonPos( CWnd *pParentWnd, int p_nStartIndexChanging)
{
    CRect rect;
	int x = 0, y = 0, i = 0;
	CGraphSettingButton* pButton = m_ArrayButton.GetAt(0);
    pButton->GetWindowRect(&rect); // Get the size and position of the dialog in screen coordinates
    x = m_Rect.left;	// - rect.left; // + 10; //+ ((m_Rect.Width() - r.Width()) / 2);
    y = m_Rect.top;		//  - rect.top;  //+ 11; //+ ((m_Rect.Height() - r.Height()) / 2); //3 = 중앙 정렬을 위해.

	// (2006/11/23 - Seung-Won, Bae) Support Scroll in SubGraph Tab Button
	m_nSubGraphButtonStartIndex += p_nStartIndexChanging;
	if( m_nSubGraphButtonStartIndex < 0) m_nSubGraphButtonStartIndex = 0;
	else if( m_ArrayButton.GetUpperBound() < m_nSubGraphButtonStartIndex) m_nSubGraphButtonStartIndex = m_ArrayButton.GetUpperBound();
	m_bMoreButtons = FALSE;
    for ( i = 0; i <=  m_ArrayButton.GetUpperBound(); i++)
        m_ArrayButton.GetAt(i)->ShowWindow( SW_HIDE);

	CString strbutton;
	int RowSumStringSize = 0, ExceptMeSumStringSize = 0, SumStringSize = 0;
    for( i = m_nSubGraphButtonStartIndex; i <=  m_ArrayButton.GetUpperBound(); i++)
	{
        pButton = m_ArrayButton.GetAt(i);
		pButton->GetWindowRect(&rect);
		ExceptMeSumStringSize = RowSumStringSize;
		SumStringSize += (rect.right - rect.left);
		RowSumStringSize = SumStringSize;
		if( SumStringSize > ( m_Rect.right - m_Rect.left))
		{
			m_bMoreButtons = TRUE;
			break;
		}
        pButton->ShowWindow( SW_SHOW);
 		pButton->SetWindowPos( pParentWnd, x + ExceptMeSumStringSize, y, rect.Width(), rect.Height(), SWP_NOZORDER);// x에 + (i * 1)//(i* r.Width())
   }

	// (2006/11/23 - Seung-Won, Bae) Support Scroll in SubGraph Tab Button
	if( pParentWnd)
	{
		pParentWnd->GetDlgItem( IDC_BTN_PREV_SUBGRAPH)->EnableWindow( 0 < m_nSubGraphButtonStartIndex);
		pParentWnd->GetDlgItem( IDC_BTN_NEXT_SUBGRAPH)->EnableWindow( m_bMoreButtons);
	}
}

void CGraphColorCollection::DeleteSubGraphSettingDlgandButton()
{
	int i = 0;
    // Free all m_SubGraphSettingDlg on destruction
    for ( i = 0; i <=  m_ArraySubGraphSettingDlg.GetUpperBound(); i++) {
        CDlgSubGraphSetting* pDlg = m_ArraySubGraphSettingDlg.GetAt(i);
        pDlg->DestroyWindow();
        delete pDlg;
    }
	m_ArraySubGraphSettingDlg.RemoveAll();

    for (i = 0; i <=  m_ArrayButton.GetUpperBound(); i++) {
        CGraphSettingButton* pButton = m_ArrayButton.GetAt(i);
		pButton->DestroyWindow();
        delete pButton;
    }
	m_ArrayButton.RemoveAll();
}
/*
CDlgSubGraphSetting* CGraphColorCollection::GetCurDlg(int nIndex)
{
	CDlgSubGraphSetting* pDlg = NULL;
    for (int i = 0; i <=  m_ArrayButton.GetUpperBound(); i++) {
         pDlg = m_ArraySubGraphSettingDlg.GetAt(i);
		 return pDlg;
    }
	return pDlg;
}
*/
CString CGraphColorCollection::GetSubGraphDatas( const char *p_szIndicatorName)
{
	CString strColor = "";
    for (int i = 0; i <=  m_ArrayButton.GetUpperBound(); i++) {
         CDlgSubGraphSetting* pDlg = m_ArraySubGraphSettingDlg.GetAt(i);
		 CString temp = pDlg->GetAllSettingData( p_szIndicatorName);
		 if(temp.GetLength())
			strColor += temp;
    }
	return strColor;	
}

void CGraphColorCollection::SetLineColor(const CString &strLineColor)
{
	CString strColor = strLineColor;
    for (int i = 0 ; i <= m_ArraySubGraphSettingDlg.GetUpperBound() ; i++) {
		 CString data = CDataConversion::GetStringData(strColor, ";");
         CDlgSubGraphSetting* pDlg = m_ArraySubGraphSettingDlg.GetAt(i);
		 if(data.GetLength() > 0)
			 pDlg->SetLineColor(data);
	}
}

void CGraphColorCollection::WhenIndicatorTypeisLineTypeSetLineColor(const int nGraphCount, const CString& strIndicatorType, CLineColor* pLineColor)
{
	if(strIndicatorType.Compare( _MTEXT( C5_LINE_TYPE)) == 0) {
		SetLineColor(pLineColor->MakeFromSixLineColorStringToOneString(nGraphCount));
	}
}

void CGraphColorCollection::CreateButtonlikeasTab(CRect& rcButtonFrame, const int nGraphCount, CString &strSubGraphName, CWnd* pWnd)
{
	CSize size = GetTextSize(pWnd);
	for(int i = 0; i< nGraphCount ; i++) {
		CString text = CDataConversion::GetStringData(strSubGraphName, ",");
		text = _MTOL1( text);
		CreateButton(i, size, text, pWnd); // create the subgraphsettingdlgs
	}

	m_nSubGraphButtonStartIndex = 0;
	SetButtonPos( pWnd, 0);
}

CSize CGraphColorCollection::GetTextSize(CWnd* pWnd)
{
	CSize size;
	LOGFONT lf;
	CClientDC pDC(pWnd); 
	CFont* font = pDC.GetCurrentFont();
	font->GetLogFont(&lf);
	size.cx = lf.lfWidth;
	size.cy = lf.lfHeight;
	return size;
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CGraphColorCollection::SetSubGraphDatas( IGraphInfoManager *p_pIGraphInfoManager, CString strSubGraphDatas)
{
	if(strSubGraphDatas.Right(2) != "\r\n")
		strSubGraphDatas += "\r\n";

    for (int i = 0 ; i <= m_ArraySubGraphSettingDlg.GetUpperBound() ; i++) {
		 CString data = CDataConversion::GetStringData(strSubGraphDatas, "\r\n");
         CDlgSubGraphSetting* pDlg = m_ArraySubGraphSettingDlg.GetAt(i);
		 pDlg->SetIGraphInfoManager( p_pIGraphInfoManager);
	     pDlg->SetSubGraphDatas(data);
	}
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CGraphColorCollection::CreateSubGraphDlgs(CRect& rcSubFrame, 
											   const int nGraphCount, 
											   const CString& strIndicatorType,
											   const CString& strSubGraphName,
											   const CString& strSubGraphTitle, 
											   CString &strType, 
											   CString &strStyle,
											   CString& strDrawStyle,
											   const CString& strSubGraphDatas, CLineColor* pLineColor, IGraphInfoManager *p_pIGraphInfoManager, int idSubGraphDlg, CWnd* pWnd)
{
	CString strGraphName = strSubGraphName;
	CString strGraphTitle = strSubGraphTitle;
  
	for(int i = 0; i< nGraphCount ; i++) {
		CString Name = CDataConversion::GetStringData(strGraphName, ",");
		CString Title = CDataConversion::GetStringData(strGraphTitle, ",");
		if(Title.IsEmpty())
			Title = Name;
		int Type = atoi(CDataConversion::GetStringData(strType, ","));
		int style = atoi(CDataConversion::GetStringData(strStyle, ","));
		int drawstyle = atoi(CDataConversion::GetStringData(strDrawStyle, ","));
		CreateSubGraphDlg(Name, Title, Type, style, drawstyle, idSubGraphDlg, pWnd); // create the subgraphsettingdlgs
	}
	WhenIndicatorTypeisLineTypeSetLineColor(nGraphCount, strIndicatorType, pLineColor);
	SetSubGraphDatas( p_pIGraphInfoManager, strSubGraphDatas);
	ShowSubGraphSettingDlg(0); // show the first one 
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CGraphColorCollection::CreateSubGraphSettingDialogandButton(CRect& rcSubFrame, CRect& rcButtonFrame, const CString& strIndicatorType, const CString& strIndicatorName, const CString& strSubGraphDatas, CLineColor* pLineColor, IGraphInfoManager *p_pIGraphInfoManager, int idSubGraphDlg, CWnd* pWnd)
{
	DeleteSubGraphSettingDlgandButton();
	SetActive(-1);

	int nGraphCount = 1;
	CString strSubGraphName, strSubGraphTitle, strType, strStyle, strDrawStyle;

	if(strIndicatorName.IsEmpty())
		GetGraphType(strIndicatorType, strType, p_pIGraphInfoManager);
	else{
		GetSubGraphNameandTypeAndStyle( p_pIGraphInfoManager, strIndicatorName, strSubGraphName, strSubGraphTitle, strType, strStyle, strDrawStyle);
	}

	GetSubGraphCount( p_pIGraphInfoManager, strIndicatorName, nGraphCount);

	//strSubGraphName 이 ,로 넘어온다고 생각. 예)ARAtio, BRatio
    SetFramePos( rcSubFrame);     // if the subdialog needs to be centered
	CreateSubGraphDlgs(rcSubFrame, nGraphCount, strIndicatorType, 
		strSubGraphName, strSubGraphTitle, strType, strStyle, strDrawStyle, 
		strSubGraphDatas, pLineColor, p_pIGraphInfoManager, idSubGraphDlg, pWnd);

    SetFramePos( rcButtonFrame);     // if the subdialog needs to be centered
	CreateButtonlikeasTab(rcButtonFrame, nGraphCount, strSubGraphName, pWnd);
}

//sy test
//test -ok
void CGraphColorCollection::GetSubGraphNameandTypeAndStyle( IGraphInfoManager *p_pIGraphInfoManager, const CString& strIndicatorName, CString& strSubGraphName, CString& strTitle, CString& strType, CString& strStyle, CString& strDrawStyle)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	//SubGraph이름얻기.
	if( p_pIGraphInfoManager == NULL) return;

	// (2006/11/23 - Seung-Won, Bae) Use User Indicator Info in Design Time.
	IGraphInfo *pIGraphInfo = p_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, strIndicatorName, IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET_AND_SET);
	if( pIGraphInfo == NULL) return;

	// Graph;\r\n
	// index;가격차트;시가,고가,저자,종가;signal여부;
	//		최저가최고가등락률;최저가현재가등락률;최고가현재가등락률;전일가현재가등락률;전일비;\r\n
	// subGraphtype;subGraphStyle;subGraphDrawStyle;subGraphName;title;숨김;색1;색2;색3;색4;색5;색6;
	//		상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n
	// .... subGraph 갯수만큼..
	// GraphEnd;\r\n
	CString strAllData;
	pIGraphInfo->GetGraphDataForMaking( strAllData);
	pIGraphInfo->Release();
	pIGraphInfo = NULL;

	if(strAllData.Find("Graph;\r\n") >= 0)
	{
		strAllData = CDataConversion::GetMidToTrimDataIndex(strAllData, "Graph;\r\n", "GraphEnd;\r\n");
		CDataConversion::GetStringData(strAllData, "\r\n"); // GraphData 부분
	}
	if( strAllData.IsEmpty()) return;
	if(strAllData.Right(2) != "\r\n") strAllData += "\r\n";

	while(!strAllData.IsEmpty())
	{
		CString strSubGraphData = CDataConversion::GetStringData(strAllData, "\r\n");
		if( strSubGraphData.IsEmpty()) break;

		strType			+= CDataConversion::GetStringData(strSubGraphData, ";") + ",";
		strStyle		+= CDataConversion::GetStringData(strSubGraphData, ";") + ",";
		strDrawStyle	+= CDataConversion::GetStringData(strSubGraphData, ";") + ",";
		strSubGraphName	+= CDataConversion::GetStringData(strSubGraphData, ";") + ",";
		strTitle		+= CDataConversion::GetStringData(strSubGraphData, ";") + ",";
	}
}

//test -ok
void CGraphColorCollection::GetSubGraphCount( IGraphInfoManager *p_pIGraphInfoManager, const CString& strIndicatorName, int &nGraphCount)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	//속성 Dll에서 갯수를 얻어오는데, 함수명이 없으면 갯수를 못얻겠지..그럴땐 nGraphCount를 1로.
	if( p_pIGraphInfoManager == NULL || strIndicatorName.GetLength() <= 0) return;

	// (2006/11/23 - Seung-Won, Bae) Use User Indicator Info in Design Time.
	IGraphInfo *pIGraphInfo = p_pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, strIndicatorName, IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET_AND_SET);
	if( pIGraphInfo == NULL) return;

	nGraphCount =  pIGraphInfo->GetSubGraphCount();
	pIGraphInfo->Release();
	pIGraphInfo = NULL;

	if(!nGraphCount) nGraphCount = 1;
}

//test -ok
void CGraphColorCollection::GetGraphType(const CString& strIndicatorType, CString& strType, IGraphInfoManager *p_pIGraphInfoManager)
{
	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	if(p_pIGraphInfoManager == NULL) return;

	int nType = -1, nStyle = -1;
	p_pIGraphInfoManager->GetGraphTypeAndStyle( strIndicatorType, "", nType, nStyle);
	strType = CDataConversion::IntToString(nType) + ",";
}

