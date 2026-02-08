// MapLoadCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MapLoadCtrl.h"
#include "limits.h"

#include "../resource.h"
#include "../ConditionMakeWnd.h"
#include "../MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************/
/*																		              */
/*							Map Loader Control							    */
/*							(Mouse Control)								      */
/*																		              */
/*							작성일 : 2002. 10~							    */
/*						작성자 : 우동우 (TEXEN SYSTEM)		    */
/*																		              */
/****************************************************/
#include "../DefineFiles/color_typedef.h"   //# <= Color define 

/////////////////////////////////////////////////////////////////////////////
// CMapLoadCtrl

//## resource ID ##
#define IDC_BUTTON_UPDATA					510
#define IDC_BUTTON_NOTCHECK				520
#define IDC_BUTTON_OK							530
#define IDC_BUTTON_BASE						540
#define IDC_BUTTON_GUBUNCHECK			550
#define IDC_STITIC_CONDITIONTITLE	560

#define IDC_BTN_RADIO_FINANCIAL1	100
#define IDC_BTN_RADIO_FINANCIAL2	101


#define NEWS_CONTROL_TYPE				"KeyWord"
#define NEWS_FILENAME						"News"


//## Condition Title Position
#define POSITION_X_TITLE				10
#define CONDITIONTITLE_WIDTH		270

#define NOTCHECK_WIDTH					95
#define GUBUNCHECK_WIDTH				75

#define RESOLUTION_X					400
#define RESOLUTION_Y					100

const int m_nConstBtnWidth = 60;
const int m_nConstBtnHeight = 20;

const CString m_strConstMapSection = "map";


//## 대상지정 Item File Load ##
#define FILE_UPJONG				"/data/upname.dat"

#define INDUSTRY_TYPE			2;	//# 업종 (상장산업)
#define KOSDAK_TYPE				3;	//# 코스닥 (거래원)
#define GROUP_TYPE				11;	//# 계열사 (그룹사)
#define MEMBERS_TYPE			28;	//# 계열사 (그룹사)

//공백 띄어쓰기 정확해야함 --;
const		LPCTSTR		Exchange_Type[] = {_T("거래원별 거래량"), _T("거래원별 거래량 비율"),
_T("거래원 순간변동량"), _T("거래원중 특정증권사 순위"), _T("특정 거래원이 매도매수 양쪽에 있을때"),
_T("특정 거래원이 매도매수 양쪽에 있을때")};


/*
#define USER_FOLDER                 "\\EzFinder_user"  
#define SYS_FOLDER                  "\\EzFinder_sys"

#define MAP_FOLDER                  "\\map" 
#define EXP_FOLDER                  "\\exp" 
#define DEF_FOLDER                  "\\def" 

#define TREELIST_CONDITION_FILE		"treelist_condition.xml"
#define TREELIST_USER_FILE			"treelist_user.xml"
#define LASTSTATE_FILE				"LastState.ini"
*/

CMapLoadCtrl::CMapLoadCtrl()
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
	//	m_brushDefault.CreateSolidBrush(COLOR_MAPLOAD);
	m_brushDefault.CreateSolidBrush(RGB(232, 226, 210));		// KB투자증권

	m_nOneLineHeight = 25;
	m_strAfterMapTag = "XXX";
	m_strAfterMapFileName = "XXX";
	m_listControlInfo.RemoveAll();


	m_pNotCheckBtn = NULL;
	m_pGubunCheckBtn = NULL;

	m_pRadioFinancial1 = NULL;
	m_pRadioFinancial2 = NULL;

	m_pConditionTitleStatic = NULL;

	m_pUpdateBtn = NULL;
	m_pOKBtn = NULL;
	m_pBaseBtn = NULL;
	m_pwndMsgTarget = NULL;
	m_pwndMain = NULL;

	m_TopExp = 0;
	m_clrSkin = RGB(232, 226, 210);
	m_nLeftAdd = 0;
	m_nTopAdd = 0; 
	m_nLineGapAdd = 0;

	m_bSep = TRUE;
}

CMapLoadCtrl::~CMapLoadCtrl()
{

}

BEGIN_MESSAGE_MAP(CMapLoadCtrl, CStatic)
	//{{AFX_MSG_MAP(CMapLoadCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BUTTON_UPDATA, OnBtnUpdate)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBtnOK)
	ON_BN_CLICKED(IDC_BUTTON_BASE, OnBtnBase)
	ON_BN_CLICKED(IDC_BUTTON_NOTCHECK, OnBtnNotCheck)
	ON_BN_CLICKED(IDC_BUTTON_GUBUNCHECK, OnBtnGubunCheck)
	ON_COMMAND_RANGE(IDC_BTN_RADIO_FINANCIAL1, IDC_BTN_RADIO_FINANCIAL2, OnBtnFinancial)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapLoadCtrl message handlers
CString CMapLoadCtrl::GetAppFileRoot()
{
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	return pMainWnd->Variant(homeCC);
}

BOOL CMapLoadCtrl::Create(CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = FALSE;

	bResult = CStatic::Create(_T(""), WS_CHILD | WS_VISIBLE, //| WS_BORDER, 
		CRect(0, 0, 100, 100), pParentWnd, nID);

	m_pwndMsgTarget = pParentWnd;

	return bResult;
}

int CMapLoadCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	//## Front Load Condition Map File Tag! ##
	m_strAfterMapFileName = "XXXX";
	m_strAfterMapTag = "XXXX";

	//## delete Ctrl List ##
	m_saDelCtrlData.RemoveAll();

	return 0;
}

BOOL CMapLoadCtrl::SetInit(BOOL bIndustry)
{
	m_bIndustry = bIndustry;

	CWnd* pwndParent = GetParent(); //CConditionMakeWnd
	pwndParent = pwndParent->GetParent();		//CConditionView

	if( pwndParent == NULL ) return TRUE;
	pwndParent = pwndParent->GetParent();		

	if( pwndParent == NULL ) return TRUE;
	pwndParent = pwndParent->GetParent();		//CMainFrmWnd	

	m_pwndMain = pwndParent;

	GetLoad_MarketInfo();

	return TRUE;
}

//# 삼성증권
extern BOOL    dll_GetMasterData(CString szType, CStringArray &arrCode, CStringArray &arrName);
extern BOOL    dll_GetMasterDataInUp(CString szType, CStringArray &arrCode, CStringArray &arrName, CString strUp);
extern LPCTSTR dll_GetJongmokName( CString szType, LPCTSTR szCode, BOOL bFullName=TRUE);

extern CString dll_GetChangeMarketCode(BYTE nMarketKind, BOOL m_bOutSideDealing = FALSE);
extern BYTE dll_GetJongMarket(LPCTSTR szCode);

BOOL CMapLoadCtrl::GetLoad_MarketInfo()
{
	BOOL bResult = TRUE;

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;

	// 	CODE_ALLJONGMOK - 장내종목 + 코스닥종목
	//	CODE_JONGMOK - 장내종목
	//	CODE_KJONGMOK - 코스닥종목
	//	CODE_UPJONG - 장내업종
	//	CODE_KUPJONG - 코스닥업종

	CStringArray* parrKospicode = &m_saCode_Kospi;
	CStringArray* parrKospiName = &m_saName_Kospi;
	pMainWnd->GetUpMasterData(parrKospicode, parrKospiName, GETMARKET_KOSPI);
	
	CStringArray* parrKosdaqCode = &m_saCode_Kosdaq;
	CStringArray* parrKosdaqName = &m_saName_Kosdaq;
	pMainWnd->GetUpMasterData(parrKosdaqCode, parrKosdaqName, GETMARKET_KOSDAQ);

	pMainWnd->GetMemberCompany(m_saCode_Member, m_saName_Member);

	return bResult;
}

void CMapLoadCtrl::OnDestroy() 
{
	//	CStatic::OnDestroy();

	TRACE("\n# MapLoader OnDestroy!");

	m_fontDefault.DeleteObject();
	m_fontBold.DeleteObject();

	m_brushDefault.DeleteObject();

	DestoryAllCtrl();


	CStatic::OnDestroy();
}

void CMapLoadCtrl::DestoryAllCtrl()
{
	//## Regist Control Data ##
	DestroyConditionMap();

	DestroyNotCheckBtn();
	DestroyFinancialRadioBtn();
	DestroyUpdateBtn();	
	DestroyOKBtn();
	DestroyBaseBtn();

	DestroyGubunCheckBtn();
	DestroyConditionTitleStatic();

	if( m_pwndMain ){
		//유진 컨트롤 관련
	}	

	TRACE("\n# MapLoader DestoryAllCtrl!\n");
}

BOOL CMapLoadCtrl::LoadConditionMapXMLFile(CString strFileName)
{
	//## Condition Map XML File Load => Dom Document Save ##
	BOOL bResult = FALSE;

	CString strFullRoot, strFolder;
	strFolder = GetFolderMap(); //(CString)FOLDER_MAP;
	if (m_bIndustry == TRUE)
		strFileName.Replace(".", "_Industry.");
	strFullRoot = GetAppFileRoot() + strFolder + "\\" + strFileName;

	HRESULT hr;
	hr = CoInitialize(NULL);

	if(!SUCCEEDED(hr))
		return bResult;

	MSXML::IXMLDOMDocumentPtr pMapDom ("Microsoft.XMLDOM", NULL, CLSCTX_INPROC_SERVER);

	pMapDom->async = false;
	if (!pMapDom->load (strFullRoot.AllocSysString()) ) 
	{
		MSXML::IXMLDOMParseErrorPtr pParsingErr = pMapDom->parseError;			

		char msg[500];
		CString strDebug;
		sprintf(msg, "Error 0x%.8X on line %d, position %d\r\n", 
			pParsingErr->errorCode,pParsingErr->line, pParsingErr->linepos);

		CString strTemp(BSTR(pParsingErr->reason));	
		strDebug =  strTemp + CString(msg);
		TRACE0(strDebug);
	}
	else
	{
		m_domConditionMap = pMapDom; 
		bResult = TRUE;

		TRACE("\n# Condition Map List Loading Success! ( %s )", strFullRoot);
	}

	return bResult;
}


CString CMapLoadCtrl::GetTagToNodeData(CString strTagID, CString strNode)
{
	//## TagID to Node Data Search ##

	CString strTemp;
	strTemp.Format("%s[%d]", strTagID, 0);

	MSXML::IXMLDOMNodeListPtr SelectedNodeListPtr = m_domConditionMap->getElementsByTagName(strTemp.AllocSysString());
	MSXML::IXMLDOMNodePtr pNode = SelectedNodeListPtr->item[0];    

	MSXML::IXMLDOMNodePtr  pChild;
	MSXML::IXMLDOMNamedNodeMapPtr pattrs;

	if ( (pNode != NULL) && (SUCCEEDED(pNode->get_attributes(&pattrs))) ) 
	{
		for(long idx = 0 ; idx < pattrs->length ; idx++) 
		{  
			pChild = pattrs->item[idx];
			_bstr_t attname = pChild->nodeName;

			CString AttrName = (LPTSTR)(LPCTSTR)attname;
			if(AttrName.Compare(strNode) == 0) 
			{
				_variant_t value = pChild->nodeValue;
				_bstr_t bstrValue(value);

				CString strResult = (LPTSTR)(LPCTSTR)bstrValue;

				SelectedNodeListPtr = NULL;
				pNode = NULL;    
				pChild = NULL;
				pattrs = NULL;

				return strResult;
			}
			pChild = NULL;
		}
	}

	SelectedNodeListPtr = NULL;
	pNode = NULL;    
	pChild = NULL;
	pattrs = NULL;

	return "";
}


CString CMapLoadCtrl::GetTagToControlNodeValues(CString strTagID, CString strNode, int nNodeIndex)
{
	//## TagID/Node to Data Search ##

	CString strAttrName;  //추출할 컨트롤의 타입이름
	CString strAttrValue = ""; //추출할 컨트롤의 정보

	CString strTemp;
	strTemp.Format("%s[%d]/CONTROL[%d]", strTagID, 0, nNodeIndex);
	MSXML::IXMLDOMNodeListPtr SelectedNodeListPtr = m_domConditionMap->getElementsByTagName(strTemp.AllocSysString());
	MSXML::IXMLDOMNodePtr pNode = SelectedNodeListPtr->item[0];    

	MSXML::IXMLDOMNodePtr  pChild;
	MSXML::IXMLDOMNamedNodeMapPtr pattrs;

	if ( (pNode != NULL) && (SUCCEEDED(pNode->get_attributes(&pattrs))) )
	{
		for(long idx = 0; idx < pattrs->length; idx++) 
		{
			pChild = pattrs->item[idx];
			_bstr_t attName = pChild->nodeName;
			strAttrName = (LPTSTR)(LPCTSTR)attName;

			if(strAttrName.Compare(strNode) == 0) 
			{
				_variant_t value = pChild->nodeValue;
				_bstr_t bstrValue(value);

				strAttrValue = (LPTSTR)(LPCTSTR)bstrValue;
				break;
			}
		}
	}

	SelectedNodeListPtr = NULL;
	pNode = NULL;    
	pChild = NULL;
	pattrs = NULL;

	return strAttrValue;
}


BOOL CMapLoadCtrl::CheckCondiGubunState(CString strTagID)
{
	//## 단일 방생이 되지 않는 지표 
	BOOL bResult = FALSE;

	if (strTagID.Left(1) == "Y")
	{
		if ((strTagID.Compare("Y1_6") == 0) ||
			(strTagID.Compare("Y1_23") == 0) ||
			(strTagID.Compare("Y1_29") == 0) ||
			(strTagID.Compare("Y2_3") == 0))
			bResult = FALSE;
		else
			bResult = TRUE;
	}

	return bResult;
}

//####################################################################################
//## Drew Tag to Map! ##
//####################################################################################

BOOL CMapLoadCtrl::SetLoadTagIDToXMLFile(CString strTagID)
{
	//## 전달 받은 TAG로 해당 Condition Map XML File Load! ##
	BOOL bResult = FALSE;
	int nPos, nPosNext = -1;

	CString strMapFileName;
	nPos = strTagID.Find("_");
	nPosNext = strTagID.Find("_", nPos + 1);
	CString strTemp = strTagID.Left(1);
	if (nPos > 0) 
	{
		strMapFileName = strTagID.Left(nPos);
		m_nAfterMapTagPos = nPos;
		if ((nPosNext > 0) &&
			((strTemp.Compare("C") == 0) || (strTemp.Compare("Z") == 0)))
		{
			strMapFileName = strTagID.Left(nPosNext);
			m_nAfterMapTagPos = nPosNext;	
		}

		strMapFileName += ".XML";
	}
	else 
	{
		nPos = 1;
		strMapFileName = strTagID.Left(1) + ".XML";

		m_nAfterMapTagPos = 1;
	}

	//## Map XML File Load! ##
	if (LoadConditionMapXMLFile(m_strConstMapSection + strMapFileName) == TRUE)
	{
		m_strAfterMapTag = strTagID;  //# <= Tag ID Save
		bResult = TRUE;
	}
	else
		bResult = FALSE;

	return bResult;
}


BOOL CMapLoadCtrl::SetLoadXMLFile(CString strFileName)
{
	BOOL bResult = FALSE;

	if (strFileName.IsEmpty() == FALSE)
	{
		if (strFileName != m_strAfterMapFileName)
		{
			//## Map XML File Load! ##
			if (LoadConditionMapXMLFile(m_strConstMapSection + strFileName + ".xml") == TRUE)
			{
				m_strAfterMapFileName = strFileName;  //# <= Map File Name Save
				bResult = TRUE;
			}
		}
		else
			bResult = TRUE;
	}

	if (bResult == FALSE)
		TRACE("\n # Map File Load Failure! : (%s)", strFileName);

	return bResult;
}


BOOL CMapLoadCtrl::SetDrewConditionMap(XMLTreeInfo *pstXMLTreeInfo, int nGroup, BOOL bModifyBtnView, BOOL bReDraw, CString strElement)
{
	BOOL bResult = FALSE;

	if ((pstXMLTreeInfo->strTag.IsEmpty() == TRUE) || (pstXMLTreeInfo->strMapFile.IsEmpty() == TRUE))
		return bResult;

	CString strGroupType;
	strGroupType = pstXMLTreeInfo->strTag.GetAt(0);
	
	//m_stNowXMLInfo = *pstXMLTreeInfo;
	COPY_XMLTreeInfo(pstXMLTreeInfo, (&m_stNowXMLInfo));
	m_listControlInfo.RemoveAll();

	ControlInfo		stControlInfo;
	if (SetLoadXMLFile(pstXMLTreeInfo->strMapFile) == FALSE)
		return bResult;

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	
	//## Condition Map Info All Delete ##
	DestroyConditionMap();

	CString strType, strTerm;
	CString strNodeVal = GetTagToNodeData(pstXMLTreeInfo->strTag, "CONTROL_NUMBER");
	int nControlNum = atoi(strNodeVal.operator const char*());

	CString strTop_Line;
	for (int i = 0; i <= nControlNum - 1; i++) 
	{
		m_nOneLineHeight = 0;
		strType = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "TYPE", i);

		int nFlag = 0;
		if (pstXMLTreeInfo->nGubun == 2) //# 순차검색
		{
			strTerm = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i);
			if (strTerm == "TERM")
				nFlag = 1;
		}

		CString strTemp;
		strTemp = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "ID", i);
		int nResourceID = atoi(strTemp.operator const char*());	


		if (strType.Compare(NEWS_CONTROL_TYPE) == 0)
			strTemp.Format("%s,%d", "Static", nResourceID);
		else 
			strTemp.Format("%s,%d", strType, nResourceID);

		m_saDelCtrlData.Add(strTemp);

		int nLeft, nTop, nRight, nBottom;
		int nStartTop = 5;
		CString Left = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "LEFT", i);
		strTop_Line = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "TOP_LINE", i);
		CString strWidth = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "WIDTH", i);
		CString strHeight = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "HEIGHT", i);	
		int nHeight = atoi(strHeight.operator const char*());

		nLeft = atoi(Left.operator const char*());
		nTop = (atoi(strTop_Line.operator const char*()) - 1) * ( m_nOneLineHeight + m_nLineGapAdd ) + nStartTop;
		nRight = atoi(strWidth.operator const char*()) + nLeft;
		nBottom = nTop + nHeight;


		if(strTop_Line.CompareNoCase("1") == 0)
			nTop = 5;
		if(strTop_Line.CompareNoCase("1.5") == 0)
			nTop = nHeight + 5 + 1;
		else if(strTop_Line.CompareNoCase("2") == 0)
			nTop = nHeight * 2 + 5 + 1;
		else if(strTop_Line.CompareNoCase("2.5") == 0)
			nTop = nHeight * 3 + 5 + 1;
		else if(strTop_Line.CompareNoCase("3") == 0)
			nTop = nHeight * 4 + 5 + 1;

		if(strType.Compare("Edit") == 0) 
			nHeight = 20;

		nBottom = nTop + nHeight;/*atoi(strHeight.operator const char*());*/

		CRect reControl(m_nLeftAdd + nLeft, m_nTopAdd + nTop, m_nLeftAdd + nRight, m_nTopAdd + nBottom);
		if (strType.Compare("Static") == 0) 
		{
			reControl.top += 3;
			CString strCaption = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "STATIC_CAPTION", i);

			if(strTop_Line == "1" && nGroup != 0)
				strCaption = "";
			
			m_pStatic = new CStatic;
			int nFindPos = strCaption.Find("&");
			if (nFindPos != -1)
				strCaption.Replace("&", " && ");

			m_pStatic->Create((LPCTSTR)strCaption,
				WS_CHILD |/* WS_VISIBLE |*/ SS_CENTERIMAGE,// | SS_WHITERECT,
				reControl,
				this,
				nResourceID);
			GetDlgItem(nResourceID)->SetFont(&m_fontDefault, FALSE);
		}
		else if (strType.Compare(NEWS_CONTROL_TYPE) == 0) 
		{
			reControl.top += 3;

			m_pStatic = new CStatic;
			/*
			int nFindPos = pstXMLTreeInfo->strName.Find("&");
			if (nFindPos != -1)
			pstXMLTreeInfo->strName.Replace("&", " && ");
			*/

			m_pStatic->Create((LPCTSTR)pstXMLTreeInfo->strName,
				WS_CHILD | /*WS_VISIBLE |*/ SS_CENTERIMAGE,
				reControl,
				this,
				nResourceID);
			GetDlgItem(nResourceID)->SetFont(&m_fontDefault, FALSE);
		}
		else if (strType.Compare("Edit") == 0) 
		{
			CString strEditLength = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "EDIT_LENGTH", i);

			int nFind = strEditLength.Find('.', 0);
			int nIntegerLength, nRealLength;
			if (nFind > -1)
			{
				nIntegerLength = atoi(strEditLength.Left(nFind));
				nRealLength = atoi(strEditLength.Right(strEditLength.GetLength() - (nFind + 1))); 
			}
			else
			{
				nIntegerLength = atoi(strEditLength);
				nRealLength = 0;
			}

			m_pEdit = new CTEditCtrlWnd;

			//# 2005.04.12 => User Patten!
			BOOL bEnable = FALSE;
			if ((pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_USERPATTEN) && 
				(GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i) == "CODE"))
				bEnable = TRUE;

			m_pEdit->Create(reControl, this, nResourceID, 0xFFFFFF, nIntegerLength, nRealLength, bEnable);
			GetDlgItem(nResourceID)->SetFont(&m_fontDefault, FALSE);

			if(nGroup > 0)
			{
				strTerm = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i);
				if(strTerm == "STERM" || strTerm == "ETERM" || strTerm == "SCNT1" || strTerm == "SCNT2")
					m_pEdit->ShowWindow(SW_HIDE);
			}

			if(GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i) == "CODE")
				m_pEdit->IngnoreComma(TRUE);

			if(pstXMLTreeInfo->strTag == "D1_30" || pstXMLTreeInfo->strTag == "D1_31" ||
				pstXMLTreeInfo->strTag == "D7_3" || pstXMLTreeInfo->strTag == "D7_4" || 
				pstXMLTreeInfo->strTag == "A25" || pstXMLTreeInfo->strTag == "C3_1")

			{
				if(GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i) == "C1")
					m_pEdit->IngnoreComma(TRUE);
			}

			//## Tab Move Info
			stControlInfo.nResourceID = nResourceID;
			stControlInfo.strControlType = "EDIT";
			stControlInfo.nLine = atof(strTop_Line);
			m_listControlInfo.AddTail(stControlInfo);
		}
		else if (strType.Compare("ComboBox") == 0) 
		{
			reControl.bottom += 100;

			m_pComboBox = new CComboBox;
			m_pComboBox->Create(WS_CHILD /*| WS_VISIBLE*/ | CBS_DROPDOWNLIST | WS_VSCROLL,
				reControl,
				this,
				nResourceID);

			GetDlgItem(nResourceID)->SetFont(&m_fontDefault, FALSE);

			//## Tab Move Info
			stControlInfo.nResourceID = nResourceID;
			stControlInfo.strControlType = "COMBOBOX";
			stControlInfo.nLine = atoi(strTop_Line);
			m_listControlInfo.AddTail(stControlInfo);

			CString strComboPacketID = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i);
			CString strComboLoadType = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "COMBO_LOAD_TYPE", i);
			if (atoi(strComboLoadType) == 1) 
			{
				CString strComboLoadData = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "COMBO_LOAD_DATA", i);

				int nItemFileType = -1;
				CStringArray saName;
				if (strComboLoadData.Compare("거래소") == 0)
				{
					if ((pstXMLTreeInfo->strTag.Compare("A1") == 0) ||	//# 거래소
						(pstXMLTreeInfo->strTag.Compare("A3") == 0))		//# 거래소 + 코스닥
					{
						nItemFileType = INDUSTRY_TYPE;
						saName.Copy(m_saName_Kospi);
					}
				}
				else if (strComboLoadData.Compare("코스닥") == 0)
				{
					if ((pstXMLTreeInfo->strTag.Compare("A2") == 0) ||  //# 코스닥
						(pstXMLTreeInfo->strTag.Compare("A3") == 0))		//# 거래소 + 코스닥
					{
						nItemFileType = KOSDAK_TYPE;
						saName.Copy(m_saName_Kosdaq);
					}
				}
				else if (strComboLoadData.Compare("계열사") == 0)
				{
					if ((pstXMLTreeInfo->strTag.Compare("A7") == 0) && (strComboPacketID.Compare("C1") == 0))		//# 계열사
					{
						nItemFileType = GROUP_TYPE;

					}
				}
				else if (strComboLoadData.Compare("회원사") == 0)
				{
					if ((pstXMLTreeInfo->strTag.Compare("A28") == 0) )
							//# 거래소 + 코스닥
					{
						nItemFileType = MEMBERS_TYPE;
						saName.Copy(m_saName_Member);
					}
				}

				if (nItemFileType != -1)
				{
					TRACE("\n [ Map Load Combo Info]");
					for (int nIndex = 0; nIndex < saName.GetSize(); nIndex++)
					{
						CString strTemp;
						strTemp = saName.GetAt(nIndex);
						strTemp.TrimRight();

						CString strTrim;
						for (int nPos = 0; nPos < strTemp.GetLength(); nPos++)
						{
							if (strTemp[nPos] != ' ')
								strTrim += (CString)strTemp[nPos];
						}
						m_pComboBox->AddString(strTrim);
					}
				}
			}
			else
			{
				BOOL bExchange = FALSE;
				CString strComboItem = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "COMBO_ITEM", i);
				int nFindPos;
				int nIndex = 0;
				//거래원 @로 구분 
				if(strComboItem.Left(1) == "@")
				{
					bExchange = TRUE;
					strComboItem = strComboItem.Mid(2);
				}

				if(strComboItem != "")
				{				
					do
					{
						nFindPos = strComboItem.Find('/');					
						if (nFindPos < 0) 
						{
							m_pComboBox->AddString((LPCTSTR)strComboItem);
							m_pComboBox->SetItemData(nIndex++, -1);
							break;
						}
						m_pComboBox->AddString((LPCTSTR)strComboItem.Mid(0, nFindPos));
						m_pComboBox->SetItemData(nIndex++, -1);
						strComboItem = strComboItem.Mid(nFindPos + 1, strComboItem.GetLength() - nFindPos);
					}while(1);
				}

				int nItemCount, nData;				
				if(bExchange)
				{
					CString strName;
					for(nItemCount = 0; nItemCount < m_saName_Member.GetSize(); nItemCount++)
					{	
						strName = m_saName_Member.GetAt(nItemCount);
						m_pComboBox->AddString(strName);
						nData = atoi(m_saCode_Member.GetAt(nItemCount));
						m_pComboBox->SetItemData(nIndex++, nData);
					}
				}
				
				
			}
		}
		else if (strType.Compare("Radio") == 0) 
		{

			stControlInfo.nResourceID = nResourceID;
			stControlInfo.strControlType = "RADIO";
			stControlInfo.nLine = atoi(strTop_Line);
			m_listControlInfo.AddTail(stControlInfo);

			CString strComboPacketID = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i);
			CString strRaioText = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "STATIC_CAPTION", i);

			CString strGrouping = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "GROUPING", i);
			
			m_pRadio = new CButton;
			m_pRadio->Create((LPCTSTR)strRaioText, WS_CHILD/*|WS_VISIBLE*/|BS_AUTORADIOBUTTON, 
				reControl,
				this,
				nResourceID);
			GetDlgItem(nResourceID)->SetFont(&m_fontDefault, FALSE);

			if(strGrouping.CompareNoCase("1") == 0)
				m_pRadio->ModifyStyle(0, WS_GROUP);

			// 				reControl.top = reControl.top + nRadioHeight;
			// 				reControl.bottom = reControl.top + atoi(strHeight.operator const char*());	

			// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 >>
			CString strCheckIndex;
			if(!strElement.IsEmpty())
			{
				CString strKey;
				strKey.Format("%d%s%d", nGroup, strElement, nResourceID);
				strCheckIndex = GetRadioCheckState(strKey);
				
				if(strCheckIndex.CompareNoCase("-1") == 0)
					strCheckIndex = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "CHECKED", i);
			}
			else
				strCheckIndex = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "CHECKED", i);

			//strCheckIndex = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "CHECKED", i);
			// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 <<
			if(strCheckIndex.CompareNoCase("1") == 0) 
				m_pRadio->SetCheck(TRUE);
		}
		else if (strType.Compare("DivideLine") == 0) 
		{
			stControlInfo.nResourceID = nResourceID;
			stControlInfo.strControlType = "RADIO";
			stControlInfo.nLine = atoi(strTop_Line);
			m_listControlInfo.AddTail(stControlInfo);

			m_pStatic = new CStatic;
			m_pStatic->Create("",
				WS_CHILD | /*WS_VISIBLE | */SS_SUNKEN,
				reControl,
				this,
				nResourceID);
			GetDlgItem(nResourceID)->SetFont(&m_fontDefault, FALSE);
		}
	}

	m_pStatic = NULL;
	m_pEdit = NULL;
	m_pComboBox = NULL;
	m_pRadio = NULL;

	//## Check Button Create! ##
	CreateNotCheckBtn(pstXMLTreeInfo->nNotUser);
	
	CConditionMakeWnd* pMKWnd = (CConditionMakeWnd*)GetParent();
	if(strGroupType == "B" && pMKWnd->m_nSeparateForm == 0)	// 재무분석
		CreateFinancialRadioBtn();

	//## 연속/단일 Check Button Create! ##
	BOOL bEnable = TRUE;
	if ((pstXMLTreeInfo->strTag == "Z2_55") ||		//# 이평간 비교(연속)
		(pstXMLTreeInfo->strTag == "Z2_56") ||	//# 이평대비 가격등락율(연속) 
		(pstXMLTreeInfo->strTag == "Z2_57"))	//# 가격/이평비교(연속)
		bEnable = FALSE;
	CreateGubunCheckBtn(pstXMLTreeInfo->nGubun, bEnable);


	//20100511 SJ_KIM 조건명 제거 요청
	// 	if (pstXMLTreeInfo->strName.IsEmpty() == 0)
	// 	{
	// 		//## Condition Title Create! ##
	// 		CreateConditionTitleStatic();
	// 		
	// 		CString strOtherTitle;
	// 		CONVERT_TYPETOTITLE(pstXMLTreeInfo->nItemType, strOtherTitle);
	// 		if (strOtherTitle.IsEmpty() == TRUE)
	// 			SetConditionTitleText(pstXMLTreeInfo->strName);
	// 		else
	// 			SetConditionTitleText("[" + strOtherTitle + "] " + pstXMLTreeInfo->strName);
	// 		
	// 		OnSize_ConditionTitleStatic();
	// 	}


	if ((pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_ITEM_COMM) ||
		(pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_ITEM_ORDER) ||
		(pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_PORTPOLIO))
	{
		CreateBaseBtn();

		//# 2004.10.08
		if (bModifyBtnView == TRUE)
			CreateUpdateBtn();
		else
			DestroyUpdateBtn();
	}
	else if (pstXMLTreeInfo->nItemType == (int)TREE_SELECT_TYPE_LANGUAGE)
	{
		//유진 수정
		//	CreateUpdateBtn();	
		//	DestroyOKBtn();
		DestroyBaseBtn();
		//	CreateBaseBtn();	

		if (bModifyBtnView == TRUE)
			CreateUpdateBtn();
		else
			DestroyUpdateBtn();
	}
	else
	{
		DestroyUpdateBtn();
		DestroyOKBtn();
		DestroyBaseBtn();
	}

	Invalidate();

	int nCondiontCnt = pMKWnd->GetConditionCount();

	if(m_TopExp != 1 && pMKWnd->m_ptabSet->GetCurSel() != 1)
	{		
		if(m_pBaseBtn)
		{	
			
		}
		else
		{	
			
		}

		if(m_pUpdateBtn)
		{
			
		}

		m_pwndMain->GetParent()->SendMessage(RMSG_SET_VISIBLE_MAPBUTTON, 1, 0);
	}

	if(bReDraw)
	{
		for (int i = 0; i <= nControlNum - 1; i++)
		{
			CString strTemp;
			strTemp = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "ID", i);
			int nResourceID = atoi(strTemp.operator const char*());	
			
			GetDlgItem(nResourceID)->ShowWindow(SW_SHOW);
		}

		if(nGroup != 0)
		{
			for(int i = 0; i < m_listControlInfo.GetCount(); i++)
			{
				POSITION pos = m_listControlInfo.FindIndex(i);
				stControlInfo = m_listControlInfo.GetAt(pos);

				if(stControlInfo.nLine == 1 && stControlInfo.strControlType == "EDIT")
				{					
					GetDlgItem(stControlInfo.nResourceID)->ShowWindow(SW_HIDE);
				}
			}
		}
	}

	CRect rect;
	GetClientRect(&rect);
	SendMessage(WM_SIZE, (WPARAM)SIZE_RESTORED, MAKELPARAM(rect.Width(), rect.Height())); 	

	//유진 컨트롤 관련 -=>

	bResult = TRUE;
	return bResult;
}


void CMapLoadCtrl::SetDefultToMapValues(XMLTreeInfo *pstXMLTreeInfo, BOOL bOtherBtnSet)
{
	//## Draw Map Set Defult ##
	if(pstXMLTreeInfo->strTag.IsEmpty())
		return;

	if (SetLoadXMLFile(pstXMLTreeInfo->strMapFile) == FALSE)
		return;

	BOOL bChartValSet = FALSE;
	CStringArray saDefultData;
	if (pstXMLTreeInfo->nChartValSet == 1)
	{
		if (GetChartDefultData(pstXMLTreeInfo->strName, saDefultData) == TRUE)
		{
			SetChartDefultToMapValues(pstXMLTreeInfo->strTag, &saDefultData);
			bChartValSet = TRUE;
		}
		saDefultData.RemoveAll();
	}
	else if (pstXMLTreeInfo->nChartValSet == 2)		//# User Patten!
	{
		if (GetUserPattenData(pstXMLTreeInfo->strName, saDefultData) == TRUE)
		{
			SetUserPattenDefultToMapValues(pstXMLTreeInfo->strTag, &saDefultData);
			bChartValSet = TRUE;
		}
		saDefultData.RemoveAll();
	}

	if (bChartValSet == FALSE)
	{
		CString strControlCnt = GetTagToNodeData(pstXMLTreeInfo->strTag, "CONTROL_NUMBER");
		int nControlCnt = atoi(strControlCnt.operator const char*());

		CString strType;
		for (int i = 0; i <= nControlCnt - 1; i++) 
		{
			strType = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "TYPE", i);

			CString strResourceID = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "ID", i);
			int nResourceID = atoi(strResourceID.operator const char*()); 

			CString strTemp;
			if (strType.Compare("Edit") == 0) 
			{
				CTEditCtrlWnd *pEditWnd = (CTEditCtrlWnd*)GetDlgItem(nResourceID);
				if (pstXMLTreeInfo->strTag == "P1")
				{
					strTemp = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "PACKET_ID", i);

					CTime ctrlTime(CTime::GetCurrentTime());
					if (strTemp == "C1")
					{
						ctrlTime -= CTimeSpan(50, 0, 0, 0);
						strTemp = ctrlTime.Format("%Y%m%d");
					}
					else if (strTemp == "C3")
						strTemp = ctrlTime.Format("%Y%m%d");
					else
						strTemp = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "DEFAULT_VALUE", i);
				}
				else
					strTemp = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "DEFAULT_VALUE", i);

				if(pstXMLTreeInfo->strTag == "D3_38")
				{
				
					strTemp.Insert(2, _T(':'));
					strTemp.Insert(5, _T(':'));
					pEditWnd->SetTimeType(TRUE);
				}
				else
					pEditWnd->InsertComma(strTemp);
				
				if(atoi(strTemp) == INT_MAX)
				{
					CConditionMakeWnd* pMKWnd = (CConditionMakeWnd*)GetParent();
					if(pMKWnd->m_nSeparateForm == 0)
						::MessageBox(m_hWnd, "입력할 수 있는 최대값은 999,999,999 입니다.", PROGRAM_TITLE, MB_OK);
					else
						::MessageBox(m_hWnd, "입력할 수 있는 최대값은 999,999,999 입니다.", PROGRAM_TITLE_BRIGHT, MB_OK);
					strTemp = "999999999";	
				}
				
				pEditWnd->SetWindowText(strTemp);
				pEditWnd->Invalidate();
			}
			else if (strType.Compare("ComboBox") == 0) 
			{
				CComboBox* pComboBoxWnd = (CComboBox*)GetDlgItem(nResourceID);

				int nComboSel;
				CString strDefaultValues = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "DEFAULT_VALUE", i);
				if (strDefaultValues.IsEmpty() == TRUE)
					nComboSel = 0;
				else
				{
					//# Defult Val => Combo Index Val
					CString	strComboVal = GetTagToControlNodeValues(pstXMLTreeInfo->strTag, "COMBO_INDEX", i);
					strComboVal += "/";

					BOOL bDefultFalg = FALSE;
					int nFindPos, nSelecetedIndex;
					nFindPos = 0;
					nSelecetedIndex = 0;
					while (nFindPos > -1)
					{
						nFindPos = strComboVal.Find('/');
						if (strDefaultValues == strComboVal.Mid(0, nFindPos))
						{
							bDefultFalg = TRUE;
							break;
						}

						strComboVal = strComboVal.Mid(nFindPos + 1, strComboVal.GetLength() - nFindPos);
						nSelecetedIndex++;
					}

					if (bDefultFalg == TRUE)
						nComboSel = nSelecetedIndex;
					else
						nComboSel = 0;
				}

				pComboBoxWnd->SetCurSel(nComboSel);
				pComboBoxWnd->ShowWindow(SW_SHOW);
				/*pComboBoxWnd->Invalidate();*/
			}
			else if(strType.Compare("Static") == 0)
			{
				CStatic *pStaticWnd = (CStatic*)GetDlgItem(nResourceID);
				pStaticWnd->ShowWindow(SW_SHOW);
			}
			else if(strType.Compare("Radio") == 0)
			{
				CButton* pRadio = (CButton*)GetDlgItem(nResourceID);
				pRadio->ShowWindow(SW_SHOW);
			}
			else if(strType.Compare("DivideLine") == 0)
			{
				CStatic *pStaticWnd = (CStatic*)GetDlgItem(nResourceID);
				pStaticWnd->ShowWindow(SW_SHOW);
			}
		}
	}

	if (bOtherBtnSet == TRUE)
	{
		//--> 0016390: [7127] "Not결과제외", "연속발생" 체크하여 수정버튼 누른후 조건한번 클릭한다음 [기본값] 클릭시 체크옵션은 초기화되지 않는 현상
		SetNotCheckValues(0);
		SetGubunCheckValues(0);
		//<--
	}

	Invalidate();
}


void CMapLoadCtrl::SetChartDefultToMapValues(CString strTagID, 
											 CStringArray* psaDefultData)
{
	int nChartDefultValIndex = 0;
	int nChartDefultValCnt = psaDefultData->GetSize();

	//## Draw Map Set Defult ##
	if(strTagID.IsEmpty())
		return;

	CString strControlCnt = GetTagToNodeData(strTagID, "CONTROL_NUMBER");
	int nControlCnt = atoi(strControlCnt.operator const char*());

	CString strType;
	for (int i = 0; i <= nControlCnt - 1; i++) 
	{
		strType = GetTagToControlNodeValues(strTagID, "TYPE", i);

		CString strResourceID = GetTagToControlNodeValues(strTagID, "ID", i);
		int nResourceID = atoi(strResourceID.operator const char*()); 

		CString strTemp;
		if (strType.Compare("Edit") == 0) 
		{
			CTEditCtrlWnd *pEditWnd = (CTEditCtrlWnd*)GetDlgItem(nResourceID);

			if (nChartDefultValCnt == nChartDefultValIndex)
				strTemp = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);
			else
			{
				strTemp = psaDefultData->GetAt(nChartDefultValIndex);
				nChartDefultValIndex++;
			}

			// strTemp = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);

			pEditWnd->SetWindowText((LPCTSTR)strTemp);
			pEditWnd->Invalidate();
		}
		else if (strType.Compare("ComboBox") == 0) 
		{
			CComboBox* pComboBoxWnd = (CComboBox*)GetDlgItem(nResourceID);

			if (pComboBoxWnd != NULL)
				pComboBoxWnd->ResetContent();

			CString strComboItem = GetTagToControlNodeValues(strTagID, "ITEM_STRING", i);
			CString strDefaultIndex = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);
			int nFindPos;
			do
			{
				nFindPos = strComboItem.Find('/');
				if (nFindPos < 0) 
				{
					pComboBoxWnd->AddString((LPCTSTR)strComboItem);
					break;
				}
				CString strTemp;
				strTemp = strComboItem.Mid(0, nFindPos);

				pComboBoxWnd->AddString((LPCTSTR)strComboItem.Mid(0, nFindPos));
				strComboItem = strComboItem.Mid(nFindPos + 1, strComboItem.GetLength() - nFindPos);
			} while(1);

			int nComboSel;
			if (strDefaultIndex.IsEmpty())
				nComboSel = 0;
			else
				nComboSel = atoi(strDefaultIndex.operator const char*());
			pComboBoxWnd->SetCurSel(nComboSel);
			pComboBoxWnd->Invalidate();
		}
	}
}


BOOL CMapLoadCtrl::GetChartDefultData(CString strConditionName,
									  CStringArray &saDefultData)
{
	BOOL bResult = FALSE;

	/*
	CString strFilePath = CEnvMgr::GetChartDataDir() + "\\searchb.xdb";
	char dbNamePath[100];
	memset(dbNamePath, 0x00, sizeof(dbNamePath));
	memcpy(dbNamePath, strFilePath, strFilePath.GetLength());

	char searchName[50];
	memset(searchName, 0x00, sizeof(searchName));
	memcpy(searchName, strConditionName, strConditionName.GetLength());

	int nDataCount = 0;
	double dDataValue[10] = {0};

	DDB db(TRUE);
	if (db.GetParam(dbNamePath, searchName, &nDataCount, dDataValue))
	{
	CString strTemp;
	for (int i = 0; i < nDataCount; i++)
	{
	strTemp.Format("%.2f", dDataValue[i]);
	saDefultData.Add(strTemp);
	}

	bResult = TRUE;
	}
	*/

	return bResult;
}


void CMapLoadCtrl::SetUserPattenDefultToMapValues(CString strTagID, 
												  CStringArray* psaDefultData)
{
	int nChartDefultValIndex = 1;
	int nChartDefultValCnt = psaDefultData->GetSize();

	//## Draw Map Set Defult ##
	if(strTagID.IsEmpty())
		return;

	CString strControlCnt = GetTagToNodeData(strTagID, "CONTROL_NUMBER");
	int nControlCnt = atoi(strControlCnt.operator const char*());

	CString strType;
	for (int i = 0; i <= nControlCnt - 1; i++) 
	{
		strType = GetTagToControlNodeValues(strTagID, "TYPE", i);

		CString strResourceID = GetTagToControlNodeValues(strTagID, "ID", i);
		int nResourceID = atoi(strResourceID.operator const char*()); 

		CString strTemp;
		if (strType.Compare("Edit") == 0) 
		{
			CString strEditPacketID;
			CTEditCtrlWnd *pEditWnd = (CTEditCtrlWnd*)GetDlgItem(nResourceID);

			if (nChartDefultValCnt < nChartDefultValIndex)
				strTemp = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);
			else
			{
				strEditPacketID = GetTagToControlNodeValues(strTagID, "PACKET_ID", i);
				if (strEditPacketID.Left(1) == "C")
				{
					strTemp = psaDefultData->GetAt(nChartDefultValIndex);

					if (strTemp.IsEmpty() == TRUE)
						strTemp = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);

					nChartDefultValIndex++;
				}
				else
					strTemp = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);
			}

			pEditWnd->SetWindowText((LPCTSTR)strTemp);
			pEditWnd->Invalidate();
		}
		else if (strType.Compare("ComboBox") == 0) 
		{
			CComboBox* pComboBoxWnd = (CComboBox*)GetDlgItem(nResourceID);

			if (pComboBoxWnd != NULL)
				pComboBoxWnd->ResetContent();

			CString strComboItem = GetTagToControlNodeValues(strTagID, "ITEM_STRING", i);
			CString strDefaultIndex = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);
			int nFindPos;
			do
			{
				nFindPos = strComboItem.Find('/');
				if (nFindPos < 0) 
				{
					pComboBoxWnd->AddString((LPCTSTR)strComboItem);
					break;
				}
				CString strTemp;
				strTemp = strComboItem.Mid(0, nFindPos);

				pComboBoxWnd->AddString((LPCTSTR)strComboItem.Mid(0, nFindPos));
				strComboItem = strComboItem.Mid(nFindPos + 1, strComboItem.GetLength() - nFindPos);
			} while(1);

			int nComboSel;
			if (strDefaultIndex.IsEmpty())
				nComboSel = 0;
			else
				nComboSel = atoi(strDefaultIndex.operator const char*());
			pComboBoxWnd->SetCurSel(nComboSel);
			pComboBoxWnd->Invalidate();
		}
	}
}


BOOL CMapLoadCtrl::GetUserPattenData(CString strName, CStringArray &saUserPattenInfo)
{
	typedef struct tagUserPattenInfo
	{
		CString strName;
		CString strCode;
		int		nPattenType;
		int		nTerm;
		int		nCycle;
		CString strStartDate;
		CString strStartTime;
		CString strEndDate;
		CString strEndTime;
	} UserPattenInfo;


	BOOL bResult = TRUE;


	/*
	UserPattenInfo	stUserPattenInfo;

	CFileFind fFind;
	CString strPath_UserPatten = CEnvMgr::GetChartDataDir() + "\\UserPattern.ini";

	char szData[BUFSIZ];
	CString strTemp;
	int nFindPos;
	if (fFind.FindFile(strPath_UserPatten))
	{
	stUserPattenInfo.strName = strName;

	GetPrivateProfileString(strName, "종목코드", "", szData, BUFSIZ, strPath_UserPatten);
	stUserPattenInfo.strCode = szData;

	stUserPattenInfo.nPattenType = GetPrivateProfileInt(strName, "패턴타입", 0, strPath_UserPatten);
	stUserPattenInfo.nTerm = GetPrivateProfileInt(strName, "기간", 0, strPath_UserPatten);
	stUserPattenInfo.nCycle = GetPrivateProfileInt(strName, "주기", 0, strPath_UserPatten);

	GetPrivateProfileString(strName, "시작일자", "", szData, BUFSIZ, strPath_UserPatten);
	strTemp = szData;
	nFindPos = strTemp.Find("-");
	if (nFindPos == -1)
	{
	stUserPattenInfo.strStartDate = strTemp;
	stUserPattenInfo.strStartTime = "";
	}
	else
	{
	stUserPattenInfo.strStartDate = strTemp.Left(nFindPos);
	stUserPattenInfo.strStartTime = strTemp.Right(strTemp.GetLength() - (nFindPos + 1));
	}

	GetPrivateProfileString(strName, "종료일자", "", szData, BUFSIZ, strPath_UserPatten);
	strTemp = szData;
	nFindPos = strTemp.Find("-");
	if (nFindPos == -1)
	{
	stUserPattenInfo.strEndDate = strTemp;
	stUserPattenInfo.strEndTime = "";
	}
	else
	{
	stUserPattenInfo.strEndDate = strTemp.Left(nFindPos);
	stUserPattenInfo.strEndTime = strTemp.Right(strTemp.GetLength() - (nFindPos + 1));
	}
	}
	else
	bResult = FALSE;


	if (bResult == TRUE)
	{
	saUserPattenInfo.Add(stUserPattenInfo.strName);
	saUserPattenInfo.Add(stUserPattenInfo.strCode);
	//stUserPattenInfo.nPattenType;
	//stUserPattenInfo.nTerm;
	//stUserPattenInfo.nCycle;
	saUserPattenInfo.Add(stUserPattenInfo.strStartDate);
	saUserPattenInfo.Add(stUserPattenInfo.strStartTime);
	saUserPattenInfo.Add(stUserPattenInfo.strEndDate);
	saUserPattenInfo.Add(stUserPattenInfo.strEndTime);
	}
	*/

	return bResult;
}


void CMapLoadCtrl::DestroyConditionMap()
{
	int nDivisionPos;
	CString strDelData;

	CString strCtrlType;
	int nResourceID;

	int nDelCnt = m_saDelCtrlData.GetSize();
	for (int i = 0; i < nDelCnt; i++)
	{
		strDelData = m_saDelCtrlData.GetAt(i);
		nDivisionPos = strDelData.Find(",");

		nResourceID = atoi(strDelData.Mid(nDivisionPos + 1, strDelData.GetLength() - nDivisionPos-1).operator const char*());
		strCtrlType = strDelData.Left(nDivisionPos);

		if (strCtrlType.Compare("Static") == 0 || strCtrlType.Compare("DivideLine") == 0) 
		{
			CStatic* pStaticWnd = (CStatic*)GetDlgItem(nResourceID);
			if (pStaticWnd)
			{
				pStaticWnd->DestroyWindow();
				delete pStaticWnd;
			}
		}
		else if (strCtrlType.Compare("Edit") == 0) 
		{
			CTEditCtrlWnd* pEditWnd = (CTEditCtrlWnd*)GetDlgItem(nResourceID);
			if (pEditWnd)
			{
				pEditWnd->DestroyWindow();
				delete pEditWnd;
			}
		}
		else if (strCtrlType.Compare("ComboBox") == 0) 
		{
			CComboBox* pComboBoxWnd = (CComboBox*)GetDlgItem(nResourceID);
			if (pComboBoxWnd)
			{
				pComboBoxWnd->DestroyWindow();
				delete pComboBoxWnd;
			}
		}
		else if (strCtrlType.Compare("Radio") == 0)
		{				
			CButton* pButton = (CButton *)GetDlgItem(nResourceID);
			if(pButton)
			{
				pButton->DestroyWindow();
				delete pButton;
			}		
		}		
	}

	m_saDelCtrlData.RemoveAll();
}

BOOL CMapLoadCtrl::CheckConditionMapDrewState()
{
	BOOL bResult = FALSE;

	int nCount = m_saDelCtrlData.GetSize();

	if (nCount > 0)
		bResult = TRUE;

	return bResult;
}

void CMapLoadCtrl::CreateConditionTitleStatic()
{
	if (CheckConditionTitleStaticDrawState() == TRUE)
	{
		m_pConditionTitleStatic->BringWindowToTop();
		return;
	}

	CRect reClient, reStatic;
	int nHeight = 15;
	GetClientRect(&reClient);

	reStatic.left = reClient.left + POSITION_X_TITLE;
	reStatic.right = reStatic.left + (int)CONDITIONTITLE_WIDTH;
	reStatic.top = reClient.top + 5;
	reStatic.bottom = reStatic.top + nHeight;

	m_pConditionTitleStatic = new CStatic;
	m_pConditionTitleStatic->Create(_T("조건명"), 
		WS_CHILD | WS_VISIBLE,// | WS_BORDER, 
		reStatic,
		this, 
		IDC_STITIC_CONDITIONTITLE);
	m_pConditionTitleStatic->SetFont(&m_fontBold);
}

void CMapLoadCtrl::DestroyConditionTitleStatic()
{
	CStatic *pConditionTitleStatic = (CStatic*)GetDlgItem(IDC_STITIC_CONDITIONTITLE);

	if (pConditionTitleStatic)
	{
		pConditionTitleStatic->DestroyWindow();
		delete pConditionTitleStatic;
	}
}

void CMapLoadCtrl::SetConditionTitleText(CString strConditionTitle)
{
	CStatic *pConditionTitleStatic = (CStatic*)GetDlgItem(IDC_STITIC_CONDITIONTITLE);

	if (pConditionTitleStatic)
	{
		int nFindPos = strConditionTitle.Find("&");
		if (nFindPos != -1)
			strConditionTitle.Replace("&", " && ");

		pConditionTitleStatic->SetWindowText(strConditionTitle);
		pConditionTitleStatic->Invalidate();
	}
}

BOOL CMapLoadCtrl::CheckConditionTitleStaticDrawState()
{
	BOOL bResult = FALSE;

	CStatic *pConditionTitleStatic = (CStatic*)GetDlgItem(IDC_STITIC_CONDITIONTITLE);
	if (pConditionTitleStatic)
		bResult = TRUE;

	return bResult;
}


void CMapLoadCtrl::CreateGubunCheckBtn(int nGubun, BOOL bEnable /*= TRUE*/)
{
	//## Birth Check Button Create ##
	if (nGubun == 1)
	{
		if (CheckGubunCheckBtnDrawState() == TRUE)
		{
			m_pGubunCheckBtn->BringWindowToTop();
			m_pGubunCheckBtn->EnableWindow(bEnable);
		}
		else
		{
			CRect reClient, reChkBtn;

			int nHeight = 15;
			GetClientRect(&reClient);

			if (CheckNotCheckBtnDrawState() == TRUE)
				reClient.right -= NOTCHECK_WIDTH + 5;

			reChkBtn.right = reClient.right - 5;
			reChkBtn.left = reChkBtn.right - GUBUNCHECK_WIDTH;

			reChkBtn.top = reClient.top + 5;
			reChkBtn.bottom = reChkBtn.top + nHeight;

			m_pGubunCheckBtn = new CButton;
			m_pGubunCheckBtn->Create((LPCTSTR)"연속발생",
				WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
				reChkBtn,
				this,
				IDC_BUTTON_GUBUNCHECK);

			GetDlgItem(IDC_BUTTON_GUBUNCHECK)->SetFont(&m_fontDefault);
			GetDlgItem(IDC_BUTTON_GUBUNCHECK)->EnableWindow(bEnable);
		}
	}
	else if (nGubun == 0)
	{
		if (CheckGubunCheckBtnDrawState() == TRUE)
			DestroyGubunCheckBtn();
	}
}


void CMapLoadCtrl::DestroyGubunCheckBtn()
{
	CButton *pCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_GUBUNCHECK);

	if (pCheckBtn)
	{
		pCheckBtn->DestroyWindow();
		delete pCheckBtn;
		pCheckBtn = NULL;
	}
}


BOOL CMapLoadCtrl::CheckGubunCheckBtnDrawState()
{
	BOOL bResult = FALSE;

	CButton *pGubunCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_GUBUNCHECK);
	if (pGubunCheckBtn)
		bResult = TRUE;

	return bResult;
}


void CMapLoadCtrl::OnBtnGubunCheck()
{
	TRACE("\n# Map Loader 단일/연속 Btn Event");
}


void CMapLoadCtrl::SetGubunCheckValues(int nState)
{
	if (CheckGubunCheckBtnDrawState() == TRUE)
	{
		CButton *pGubunCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_GUBUNCHECK);
		if (pGubunCheckBtn)
		{
			pGubunCheckBtn->SetCheck(nState);
			pGubunCheckBtn->Invalidate();
		}
	}
}


BOOL CMapLoadCtrl::GetGubunCheckValues()
{
	BOOL bResult = FALSE;

	if (CheckGubunCheckBtnDrawState() == TRUE)
	{
		CButton *pGubunCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_GUBUNCHECK);
		if (pGubunCheckBtn)
			bResult = pGubunCheckBtn->GetCheck();
	}

	return bResult;
}


void CMapLoadCtrl::CreateNotCheckBtn(int nNotUser)
{
	//## Not Check Button Create ##

	if (nNotUser == 1)
	{
		if (CheckNotCheckBtnDrawState() == TRUE)
			m_pNotCheckBtn->BringWindowToTop();
		else
		{
			CRect reClient, reChkBtn;

			int nHeight = 15;
			GetClientRect(&reClient);

			reChkBtn.right = reClient.right - 5;
			reChkBtn.left = reChkBtn.right - NOTCHECK_WIDTH;
			reChkBtn.top = reClient.top + 5;
			reChkBtn.bottom = reChkBtn.top + nHeight;
			m_pNotCheckBtn = new CButton;
			m_pNotCheckBtn->Create((LPCTSTR)"NOT 결과제외",
				WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,// | WS_BORDER,
				reChkBtn,
				this,
				IDC_BUTTON_NOTCHECK);

			GetDlgItem(IDC_BUTTON_NOTCHECK)->SetFont(&m_fontDefault);
		}
	}
	else if (nNotUser == 0)
	{
		if (CheckNotCheckBtnDrawState() == TRUE)
			DestroyNotCheckBtn();
	}
}

void CMapLoadCtrl::CreateFinancialRadioBtn()
{
	if(CheckFinacialRadioBtnDrawState())
	{
		m_pRadioFinancial1->BringWindowToTop();
		m_pRadioFinancial2->BringWindowToTop();
	}
	else
	{
		CRect rtClient;
		CRect rtTemp;

		int nHeight = 15;
		GetClientRect(rtClient);

		rtTemp.right = rtClient.right - 10 - NOTCHECK_WIDTH;
		rtTemp.left = rtTemp.right - 43;
		rtTemp.top = rtClient.top + 5;
		rtTemp.bottom = rtTemp.top + nHeight;
		m_pRadioFinancial1 = new CButton;
		m_pRadioFinancial1->Create(_T("개별"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rtTemp, this, IDC_BTN_RADIO_FINANCIAL1);
		m_pRadioFinancial1->SetFont(&m_fontDefault);
		m_pRadioFinancial1->SetCheck(TRUE);
		m_pRadioFinancial1->ShowWindow(SW_HIDE);

		rtTemp.top = rtTemp.bottom;
		rtTemp.bottom = rtTemp.top + 25;
		m_pRadioFinancial2 = new CButton;
		m_pRadioFinancial2->Create(_T("연계"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, rtTemp, this, IDC_BTN_RADIO_FINANCIAL2);
		m_pRadioFinancial2->SetFont(&m_fontDefault);
		m_pRadioFinancial2->ShowWindow(SW_HIDE);

		m_pRadioFinancial1->ModifyStyle(0, WS_GROUP);
		m_pRadioFinancial2->ModifyStyle(0, WS_GROUP);
	}
}

void CMapLoadCtrl::DestroyNotCheckBtn()
{
	CButton *pCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_NOTCHECK);

	if (pCheckBtn)
	{
		pCheckBtn->DestroyWindow();
		delete pCheckBtn;
		pCheckBtn = NULL;
	}
}

void CMapLoadCtrl::DestroyFinancialRadioBtn()
{
	CButton *pRadio = (CButton*)GetDlgItem(IDC_BTN_RADIO_FINANCIAL1);
	if(pRadio)
	{
		pRadio->DestroyWindow();
		delete pRadio;
		pRadio = NULL;
	}

	pRadio = (CButton*)GetDlgItem(IDC_BTN_RADIO_FINANCIAL2);
	if(pRadio)
	{
		pRadio->DestroyWindow();
		delete pRadio;
		pRadio = NULL;
	}
}

BOOL CMapLoadCtrl::CheckNotCheckBtnDrawState()
{
	BOOL bResult = FALSE;

	CButton *pNotCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_NOTCHECK);
	if (pNotCheckBtn)
		bResult = TRUE;

	return bResult;
}

void CMapLoadCtrl::OnBtnNotCheck()
{

}

BOOL CMapLoadCtrl::CheckFinacialRadioBtnDrawState()
{
	BOOL bResult = FALSE;

	CButton* pRadioFinanacial1 = (CButton*)GetDlgItem(IDC_BTN_RADIO_FINANCIAL1);
	if(pRadioFinanacial1)
		bResult = TRUE;

	if(bResult)
	{
		CButton* pRadioFinanacial2 = (CButton*)GetDlgItem(IDC_BTN_RADIO_FINANCIAL2);
		if(pRadioFinanacial2)
			bResult = TRUE;
		else
			bResult = FALSE;
	}

	return bResult;
}

void CMapLoadCtrl::SetNotCheckValues(int nState)
{
	if (CheckNotCheckBtnDrawState() == TRUE)
	{
		CButton *pCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_NOTCHECK);
		pCheckBtn->SetCheck(nState);
		pCheckBtn->Invalidate();
	}
}

BOOL CMapLoadCtrl::GetNotCheckValues()
{
	BOOL bResult = FALSE;

	if (CheckNotCheckBtnDrawState() == TRUE)
	{
		CButton *pCheckBtn = (CButton*)GetDlgItem(IDC_BUTTON_NOTCHECK);
		bResult = pCheckBtn->GetCheck();
	}

	return bResult;
}

void CMapLoadCtrl::OnBtnUpdate()
{
	m_pwndMsgTarget->SendMessage(WM_NOTIFY_MAPLOAD, NM_MAPLOAD_UPDATE, (LPARAM)0);
}

void CMapLoadCtrl::CreateUpdateBtn()
{
	if (CheckUpdataBtnDrawState() == TRUE)
	{
		m_pUpdateBtn->BringWindowToTop();
		return;
	}

	CRect reClient, reUpdataBtn;
	GetClientRect(&reClient);

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	if(pMainWnd->m_nSeparateForm)
		return;
	
	reUpdataBtn.right = reClient.right - 5;
	reUpdataBtn.left = reUpdataBtn.right - m_nConstBtnWidth;
	reUpdataBtn.bottom = reClient.bottom - 5;
	reUpdataBtn.top = reUpdataBtn.bottom - m_nConstBtnHeight;
	m_pUpdateBtn = new CfxImgButton;
	m_pUpdateBtn->Create((LPCTSTR)"수정", reUpdataBtn, this, IDC_BUTTON_UPDATA);
	pMainWnd->SetBtnImg(m_pUpdateBtn, 4);
	//m_pUpdateBtn->ShowWindow(SW_HIDE);
}


void CMapLoadCtrl::DestroyUpdateBtn()
{
	CfxImgButton *pUpdateBtn = (CfxImgButton*)GetDlgItem(IDC_BUTTON_UPDATA);

	if (pUpdateBtn)
	{
		pUpdateBtn->DestroyWindow();
		delete pUpdateBtn;
		m_pUpdateBtn = NULL;
		pUpdateBtn = NULL;
	}

}

BOOL CMapLoadCtrl::CheckUpdataBtnDrawState()
{
	BOOL bResult = FALSE;

	CfxImgButton *pUpdateBtn = (CfxImgButton*)GetDlgItem(IDC_BUTTON_UPDATA);
	if (pUpdateBtn)
		bResult = TRUE;

	return bResult;
}

void CMapLoadCtrl::OnBtnOK()
{
	m_pwndMsgTarget->SendMessage(WM_NOTIFY_MAPLOAD, NM_MAPLOAD_OK, (LPARAM)0);
}


void CMapLoadCtrl::CreateOKBtn()
{
	if (CheckOKBtnDrawState() == TRUE)
	{
		m_pOKBtn->BringWindowToTop();
		return;
	}

	CRect reClient, reOKBtn;
	GetClientRect(&reClient);

	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
	
	reOKBtn.right = reClient.right - 5;
	reOKBtn.left = reOKBtn.right - m_nConstBtnWidth;

	reOKBtn.bottom = reClient.bottom - m_nConstBtnHeight * 2 - 5;
	reOKBtn.bottom -= 2 * 2;  //# 버튼 간격
	reOKBtn.top = reOKBtn.bottom - m_nConstBtnHeight;
	m_pOKBtn = new CfxImgButton;
	m_pOKBtn->Create((LPCTSTR)"취소", reOKBtn, this, IDC_BUTTON_OK);
	pMainWnd->SetBtnImg(m_pOKBtn, 4);
}

void CMapLoadCtrl::DestroyOKBtn()
{
	CfxImgButton *pOKBtn = (CfxImgButton*)GetDlgItem(IDC_BUTTON_OK);

	if (pOKBtn)
	{
		pOKBtn->DestroyWindow();
		delete pOKBtn;
		pOKBtn = NULL;
	}
}

BOOL CMapLoadCtrl::CheckOKBtnDrawState()
{
	BOOL bResult = FALSE;

	CfxImgButton *pOKBtn = (CfxImgButton*)GetDlgItem(IDC_BUTTON_OK);
	if (pOKBtn)
		bResult = TRUE;

	return bResult;
}

void CMapLoadCtrl::OnBtnBase()
{
	//SetDefultToMapValues(m_strNowMapTag, m_strNowMapFileName);
	SetDefultToMapValues(&m_stNowXMLInfo);
}


void CMapLoadCtrl::CreateBaseBtn()
{
	if (CheckBaseBtnDrawState() == TRUE)
	{
		m_pBaseBtn->BringWindowToTop();
		return;
	}

	CRect reClient, reBaseBtn;
	GetClientRect(&reClient);

 	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pwndMain;
// 	if(pMainWnd->m_nSeparateForm)
// 		return;
	
	reBaseBtn.right = reClient.right - 5;
	reBaseBtn.left = reBaseBtn.right - m_nConstBtnWidth;

	reBaseBtn.bottom = reClient.bottom - m_nConstBtnHeight - 5;
	reBaseBtn.bottom -= 2;  //# 버튼 간격
	reBaseBtn.top = reBaseBtn.bottom - m_nConstBtnHeight;
	m_pBaseBtn = new CfxImgButton;
	m_pBaseBtn->Create((LPCTSTR)"기본값", reBaseBtn, this, IDC_BUTTON_BASE);
	pMainWnd->SetBtnImg(m_pBaseBtn, 4);
}

void CMapLoadCtrl::DestroyBaseBtn()
{
	CfxImgButton *pBaseBtn = (CfxImgButton*)GetDlgItem(IDC_BUTTON_BASE);

	if (pBaseBtn)
	{
		pBaseBtn->DestroyWindow();
		delete pBaseBtn;
		m_pBaseBtn = NULL;
		pBaseBtn = NULL;
	}
}

BOOL CMapLoadCtrl::CheckBaseBtnDrawState()
{
	BOOL bResult = FALSE;

	CfxImgButton *pBaseBtn = (CfxImgButton*)GetDlgItem(IDC_BUTTON_BASE);
	if (pBaseBtn)
		bResult = TRUE;

	return bResult;
}


void CMapLoadCtrl::SetStructDataClear(User_Define &stUserDefine)
{
	//User_Index 초기화
	stUserDefine.IndexNo = 0; 			/* 지표 index */
	stUserDefine.GroupNo = 0; 			/* 소속 그룹번호*/
	stUserDefine.GroupIdxCnt = 0;		/* 그룹내 지표 개수 */
	stUserDefine.TargetBase = 0;		/* 0:일간, 1:주간, 2:월간 3:1분,  4:5분    */
	/* 5:10분, 6:20분, 7:30분, 8:60분   */
	stUserDefine.Type = 0; 					/* 지표의 종류*/
	stUserDefine.Type1 = 0; 				/* 조건 형태*/
	stUserDefine.Type2 = 0; 				/* 상세 형태*/

	stUserDefine.STerm = 0;         /* n봉전 기준               	*/
	stUserDefine.ETerm = 1;         /* n봉이내                 	*/

	stUserDefine.SignalCnt1 = 1;				/* 발생회수	(new)	X번에서 X번 만족*/
	stUserDefine.SignalCnt2 = 1;				/* 발생회수	(new)									*/
	stUserDefine.ResultViewNum = 0;		/* 결과값보기 필드 갯수(0:사용안함)	*/    

	/* 검색형태 0:일반 1:사용자정의	2:뉴스 3:패턴   (new) */
	memcpy(&stUserDefine.SearchType, "0", sizeof(stUserDefine.SearchType));   

	stUserDefine.Rank = 100;        /* 순위검색 종목 수             	*/
	stUserDefine.Trend = 2;         /* 추세횟수                     	*/
	stUserDefine.ResultViewNum = 0;	/* 결과값보기 필드 갯수(0:사용안함)*/    

	/* 검색구분 0:일반 1:순위 2:복합*/
	memcpy(&stUserDefine.IndexType, "0", sizeof(stUserDefine.IndexType));   

	/* 연산자'0':OR '1':AND '2':NEXT*/
	memcpy(&stUserDefine.Operator, "1", sizeof(stUserDefine.Operator)); 

	/* 결과제외 '0':A, '1':Not A    */
	memcpy(&stUserDefine.Reverse, "0", sizeof(stUserDefine.Reverse));   

	/* 지표의 연속발생 구분 '0':단일발생, 1:연속발생지표 */
	memcpy(&stUserDefine.IdxContiGubun, "0", sizeof(stUserDefine.IdxContiGubun));  

	memcpy(&stUserDefine.Code, "000000", sizeof(stUserDefine.Code));  

	/* 지표관련 변수값 (ex.12,26,9) 	*/
	memset(stUserDefine.Param, 0, sizeof(stUserDefine.Param));    

	/* 기타 설정 변수값             */
	memset(stUserDefine.Condition, 0, sizeof(stUserDefine.Condition));    
}


BOOL CMapLoadCtrl::GetMapValuesToStruct(CString strTagID,
										User_Define &stResultUserDefine, 
										CString &strResultComposition, BOOL &bHaveTerm, int nGroup, CString strElement)
{
	//## 현재 조건 화면에서 구성된 값을 Packet와 완성 구문의 만든다. ##

	//#### Need Make Conposition! ####
	CString strParam[MAX_PARAM + 1];      //#define		MAX_PARAM		3
	CString strCondition[MAX_CONDI + 1];	//#define		MAX_CONDI		6
	CString strTrend, strSTerm, strETerm, strType2, strRank, strCode;

	//# 2003. 11. 25 추가 
	CString strSignalCnt1, strSignalCnt2, strGubun;

	CString strRadioGroupID;

	//## Struct Clear ##
	SetStructDataClear(stResultUserDefine);

	CString strTemp;
	strTemp = GetTagToNodeData(strTagID, "CONTROL_NUMBER");
	int nControlCount = atoi(strTemp.operator const char*());

	strTemp = GetTagToNodeData(strTagID, "MAP_TYPE");
	stResultUserDefine.Type = atoi(strTemp.operator const char*());

	strTemp = GetTagToNodeData(strTagID, "T1");
	if (!strTemp.IsEmpty())
		stResultUserDefine.Type1 = atoi(strTemp.operator const char*());

	strTemp = GetTagToNodeData(strTagID, "T2");
	if (!strTemp.IsEmpty())
		stResultUserDefine.Type2 = atoi(strTemp.operator const char*());

	//# m_nCondiType	: 0 (기본검색)
	//#					: 1 (순위검색)
	//#					: 2 (순차검색)
	CString strSearchType;
	strSearchType.Format("%d", m_stNowXMLInfo.nCondiType);
	memcpy(&stResultUserDefine.IndexType, strSearchType.operator const char*(), sizeof(stResultUserDefine.IndexType));


	if (m_stNowXMLInfo.nGubun == 1)
		stResultUserDefine.IdxContiGubun = (GetGubunCheckValues() == TRUE) ? '1':'0';
	else
		stResultUserDefine.IdxContiGubun = '0';

	if (m_stNowXMLInfo.nNotUser == 1)
		stResultUserDefine.Reverse = (GetNotCheckValues() == TRUE) ? '1':'0';
	else
		stResultUserDefine.Reverse = '0';

	CString strControlType;
	int nResourceID;

	for (int i = 0; i <= nControlCount - 1; i++) 
	{
		//#=> 컨트롤의 타입을 얻는다.(EDIT, STATIC, COMBOBOX, BUTTON)
		strControlType = GetTagToControlNodeValues(strTagID, "TYPE", i);

		strTemp = GetTagToControlNodeValues(strTagID, "ID", i);
		nResourceID = atoi(strTemp.operator const char*()); 

		//20100517 SJ_KIM 선택 라디오에 속한 컨트롤인지 체크
		CString strGroupID = GetTagToControlNodeValues(strTagID, "RADIOGROUP_ID", i);
		if(strControlType.CompareNoCase("Radio") != 0 && strGroupID.Find('_') > 0)
		{
			//라디오 그룹에 속한 컨트롤이 아닐경우 패스
			if(strGroupID.CompareNoCase(strRadioGroupID) != 0)
				continue;					 
		}

		if ((strControlType.Compare("Edit") == 0) ||
			(strControlType.Compare("Static") == 0))      //# Add : Danny(2006.10) 
		{
			CString strEditText;
			if (strControlType.Compare("Edit") == 0)
			{
				CTEditCtrlWnd *pEdit = (CTEditCtrlWnd*)GetDlgItem(nResourceID);

				if (pEdit == NULL)
					return FALSE;

				pEdit->GetWindowText(strEditText);

				if(strEditText.IsEmpty())
					strEditText.Format("0");
			}
			else if (strControlType.Compare("Static") == 0)
				strEditText = GetTagToControlNodeValues(strTagID, "DEFAULT_VALUE", i);

			CString strEditPacketID = GetTagToControlNodeValues(strTagID, "PACKET_ID", i);
			if ((!strEditText.IsEmpty()) && (!strEditPacketID.IsEmpty())) 
			{
				int nTempEdit;
				//## Modify : Danny 2002.12
				if (strEditPacketID.Compare("STERM") == 0) 
				{
					bHaveTerm = TRUE;

					nTempEdit = atoi(strEditText.operator const char*());
					stResultUserDefine.STerm = nTempEdit;

					//## Need Make Composition ##
					strSTerm = strEditText;
				}
				else if (strEditPacketID.Compare("ETERM") == 0) 
				{
					bHaveTerm = TRUE;

					nTempEdit = atoi(strEditText.operator const char*());
					stResultUserDefine.ETerm = nTempEdit;

					//## Need Make Composition ##
					strETerm = strEditText;
				}
				else if (strEditPacketID.Compare("RANK") == 0) 
				{
					nTempEdit = atoi(strEditText.operator const char*());
					stResultUserDefine.Rank = nTempEdit;

					//## Need Make Composition ##
					strRank = strEditText;
				}
				else if (strEditPacketID.Compare("TR") == 0) 
				{
					nTempEdit = atoi(strEditText.operator const char*());
					stResultUserDefine.Trend = nTempEdit;

					//## Need Make Composition ##
					strTrend = strEditText;
				}
				else if (strEditPacketID.Compare("SCNT1") == 0) 
				{
					nTempEdit = atoi(strEditText.operator const char*());
					stResultUserDefine.SignalCnt1 = nTempEdit;

					//## Need Make Composition ##
					strSignalCnt1 = strEditText;
				}
				else if (strEditPacketID.Compare("SCNT2") == 0) 
				{
					nTempEdit = atoi(strEditText.operator const char*());
					stResultUserDefine.SignalCnt2 = nTempEdit;

					//## Need Make Composition ##
					strSignalCnt2 = strEditText;
				}
				else if (strEditPacketID.Compare("CODE") == 0) 
				{
					int nJongCodeLength = 6;
					if (strEditText.GetLength() > nJongCodeLength)
						strEditText.Left(nJongCodeLength);
					else
					{
						int nAddTestCnt = nJongCodeLength - strEditText.GetLength();
						for (int nIndex = 0; nIndex < nAddTestCnt; nIndex++)
							strEditText = "0" + strEditText;
					}
					memcpy(&stResultUserDefine.Code, strEditText.operator const char*(), sizeof(stResultUserDefine.Code));

					//## Need Make Composition ##
					strCode = strEditText;
				}
				else 
				{
					//콤마 관련 추가
					CString strWithComma = strEditText;
					strEditText.Remove(',');
					//////////////////////////////
					double dTempEdit;
					if(strTagID == "D3_38")
						strEditText.Remove(':');

					
					dTempEdit = atof(strEditText.operator const char*());

					CString strEditPacketName = strEditPacketID.Left(1);
					CString strEditPacketNum = strEditPacketID.Mid(1);
					int nEditPacketNum = atoi(strEditPacketNum.operator const char*());

					if (strEditPacketName.Compare("C") == 0) 
					{
						stResultUserDefine.Condition[nEditPacketNum - 1] = dTempEdit;
						CString strTemp = strWithComma;
						if(strTagID == "D3_38")
							strWithComma.Remove(':');
						
						strCondition[nEditPacketNum - 1] = strTemp;
					}
					else if (strEditPacketName.Compare("P") == 0) 
					{
						stResultUserDefine.Param[nEditPacketNum - 1] = dTempEdit;
						strParam[nEditPacketNum - 1] = strWithComma;

					}
				}
			}
		}
		else if (strControlType.Compare(NEWS_CONTROL_TYPE) == 0) 
		{
			stResultUserDefine.Condition[0] = m_stNowXMLInfo.nResultViewDefVal;		//# KeyWord Code!
			//## Need Make Composition ##
			strCondition[0] = m_stNowXMLInfo.strName;
		}
		else if (strControlType.Compare("ComboBox") == 0) 
		{
			CComboBox* pComboBox = (CComboBox*)GetDlgItem(nResourceID);

			if (pComboBox == NULL)
				return FALSE;

			//# Index start = 0
			CString strComboText;
			pComboBox->GetWindowText(strComboText);

			int nSelecetedIndex = pComboBox->GetCurSel();
			CString strComboPacketID = GetTagToControlNodeValues(strTagID, "PACKET_ID", i);
			if ((nSelecetedIndex >= 0) && (!strComboPacketID.IsEmpty())) 
			{
				CString strSearchData;
				CString strComboPacketName = strComboPacketID.Left(1);
				int nComboPacketNum = atoi(strComboPacketID.Mid(1));

				CString strComboLoadType = GetTagToControlNodeValues(strTagID, "COMBO_LOAD_TYPE", i);
				if (atoi(strComboLoadType) == 1) 
				{
					CString strComboLoadData = GetTagToControlNodeValues(strTagID, "COMBO_LOAD_DATA", i);

					int nItemFileType = -1;
					CStringArray saCode;
					if (strComboLoadData.Compare("거래소") == 0)
					{
						/*
						if (((m_stNowXMLInfo.strTag.Compare("A1") == 0) && (strComboPacketID.Compare("C1") == 0)) ||	//# 거래소
						((m_stNowXMLInfo.strTag.Compare("A3") == 0) && (strComboPacketID.Compare("C1") == 0)))		//# 거래소 + 코스닥
						*/
						if ((m_stNowXMLInfo.strTag.Compare("A1") == 0) || (m_stNowXMLInfo.strTag.Compare("A3") == 0))
						{
							nItemFileType = INDUSTRY_TYPE;
							saCode.Copy(m_saCode_Kospi);
						}
					}
					else if (strComboLoadData.Compare("코스닥") == 0)
					{
						/*
						if (((m_stNowXMLInfo.strTag.Compare("A2") == 0) && (strComboPacketID.Compare("C1") == 0)) || 
						((m_stNowXMLInfo.strTag.Compare("A3") == 0) && (strComboPacketID.Compare("C2") == 0)))		//# 거래소 + 코스닥
						*/
						if ((m_stNowXMLInfo.strTag.Compare("A2") == 0) || (m_stNowXMLInfo.strTag.Compare("A3") == 0))
						{
							nItemFileType = KOSDAK_TYPE;
							saCode.Copy(m_saCode_Kosdaq);
						}
					}
					else if (strComboLoadData.Compare("계열사") == 0)
					{
						if ((m_stNowXMLInfo.strTag.Compare("A7") == 0) && (strComboPacketID.Compare("C1") == 0))		//# 계열사
						{
							nItemFileType = GROUP_TYPE;

						}
					}
					else if (strComboLoadData.Compare("회원사") == 0)
					{
						if ((m_stNowXMLInfo.strTag.Compare("A28") == 0))		//# 계열사
						{
							nItemFileType = MEMBERS_TYPE;
							saCode.Copy(m_saCode_Member);
						}
					}

					if (nItemFileType != -1)
						strSearchData = saCode.GetAt(nSelecetedIndex);
				}
				else
				{
					BOOL bExchange = FALSE;
					int	 nIdx = 0, nValue;
					CString strCopy;
					CString	strItemData = GetTagToControlNodeValues(strTagID, "COMBO_INDEX", i);
					if(strItemData.Left(1) == "@")
					{
						bExchange = TRUE;
						strItemData = strItemData.Mid(2);

						for(nIdx = 0; nIdx < m_saCode_Member.GetSize(); nIdx++)
						{
							nValue = atoi(m_saCode_Member.GetAt(nIdx));
							strCopy.Format("/%d", nValue);
							strItemData += strCopy;
						}
					}
					//거래원 @로 구분
					strItemData += "/";

					int nFindPos;
					int nCount = 0;
					do
					{
						nFindPos = strItemData.Find('/');
						if (nCount > nSelecetedIndex)
							break;

						if (nFindPos == -1) 
						{
							nFindPos = 1;
							strSearchData = strItemData.Mid(0, nFindPos);
							break;
						}
						strSearchData = strItemData.Mid(0, nFindPos);
						strItemData = strItemData.Mid(nFindPos + 1, strItemData.GetLength() - nFindPos);
						
						if (nFindPos == 0) 
							continue;

						nCount++;
					} while(1);			
				}

				if (strComboPacketName.Compare("C") == 0) 
				{
					double dComboSearchData = atof(strSearchData.operator const char*());
					stResultUserDefine.Condition[nComboPacketNum - 1] = dComboSearchData;

					//## Need Make Composition ##
					strCondition[nComboPacketNum - 1] = strComboText;
				}
				else if (strComboPacketName.Compare("T") == 0) 
				{
					int nComboSearchData = atoi(strSearchData.operator const char*());
					stResultUserDefine.Type2 = nComboSearchData;

					//## Need Make Composition ##
					strType2 = strComboText;
				}
				else if (strComboPacketName.Compare("P") == 0) 
				{
					double dComboSearchData = atof(strSearchData.operator const char*());
					stResultUserDefine.Param[nComboPacketNum - 1] = dComboSearchData;

					//## Need Make Composition ##
					//	strParam[nEditPacketNum - 1] = strEditText;
					strParam[nComboPacketNum - 1] = strComboText;

				}
			}
		}
		else if (strControlType.Compare("Radio") == 0) 
		{
			CString strChecked;// = GetTagToControlNodeValues(strTagID, "CHECKED", i);
			CString strRadioCaption = GetTagToControlNodeValues(strTagID, "STATIC_CAPTION", i);
			
		
			CButton* pRadio = (CButton*)GetDlgItem(nResourceID);
			if(!pRadio)
				return FALSE;
			BOOL bChecked = pRadio->GetCheck();

			// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 >>
			if(!strElement.IsEmpty())
			{
				CString strRadioInfoKey, strTemp;
				strRadioInfoKey.Format("%d%s%d", nGroup, strElement, nResourceID);
				strTemp.Format("%d", bChecked);
				SetRadioCheckState(strRadioInfoKey, strTemp, bChecked);
			}
			// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 <<

			if(bChecked/*strChecked.CompareNoCase("1") == 0*/)
			{
				//선택된 라디오버튼이 다른 컨트롤을 가지고 있는지 여부 확인.
				if(strGroupID.Find('_') > 0 )
					strRadioGroupID = strGroupID;
			
				pRadio->GetWindowText(strChecked);

				double dTempRadio = 0.0;
				CString strCaption = GetTagToControlNodeValues(strTagID, "RADIO_VALUE", i);	
				strCaption.Remove(',');
					dTempRadio = atof(strCaption);	
				
				CString strRadioPacketID = GetTagToControlNodeValues(strTagID, "PACKET_ID", i);
				CString strRadioPacketName = strRadioPacketID.Left(1);		
				CString strRadioPacketNum = strRadioPacketID.Mid(1);
				int nRadioPacketNum = atoi(strRadioPacketNum.operator const char*());

				if (strRadioPacketName.Compare("C") == 0) 
				{
					stResultUserDefine.Condition[nRadioPacketNum - 1] = dTempRadio;

					//## Need Make Composition ##
					//	strCondition[nEditPacketNum - 1] = strEditText;
					strCondition[nRadioPacketNum - 1] = strRadioCaption;
				}
				else if (strRadioPacketName.Compare("P") == 0) 
				{
					stResultUserDefine.Param[nRadioPacketNum - 1] = dTempRadio;

					//## Need Make Composition ##
					//	strParam[nEditPacketNum - 1] = strEditText;
					strParam[nRadioPacketNum - 1] = strRadioCaption;

				}
				else if (strRadioPacketName.Compare("T") == 0) 
				{
					int nRadioData = atoi(strCaption);
					stResultUserDefine.Type2 = nRadioData;

					//## Need Make Composition ##
					strType2 = strRadioCaption;
				}
			}
		}
	}


	CString strXML_INDEX_NAME;
	CString strNameIndex= "";
	int nRadioGroupIndex = 0;
	
	nRadioGroupIndex = strRadioGroupID.Find('_');
	if(nRadioGroupIndex > 0)
		strNameIndex = strRadioGroupID.Mid(nRadioGroupIndex + 1, 1);

	//## 완성된 조건 해당 문구	
	if( atoi(strNameIndex) > 1)
		strXML_INDEX_NAME.Format("COMPLETE_INDEX_NAME%s", strNameIndex);
	else 
		strXML_INDEX_NAME = "COMPLETE_INDEX_NAME";

	CString strComposition = GetTagToNodeData(strTagID, strXML_INDEX_NAME);	
	
	if(nGroup != 0)
	{
		CString strEx = "봉전기준 (ETERM)봉이내, (SCNT1)~(SCNT2)회 발생, ";
		if(strComposition.Find(strEx) != -1)
		{
			int nStrLength = strComposition.GetLength();
			strComposition = strComposition.Mid(55, nStrLength - 55);
		}
	}
	
	//## RETURN 할 완성 구문 ##
	//## strResultComposition : Return Composition
	strResultComposition = strComposition; 

	CString strExtractData;
	CString strReplaceTarget;  //교환될 데이터
	CString strReplaceData;    //교환할 데이터 

	int nStartPos; //<= '(' 를 찾은위치
	int nEndPos;   //<= ')' 를 찾은위치
	
	do
	{
		nStartPos = strComposition.Find('(');
		nEndPos = strComposition.Find(')');
		if (nStartPos < 0 || nEndPos < 0)
			break;

		//( ) 사이에서 뽑아낸 데이터가 단일인지 복수인지 판별
		strExtractData = strComposition.Mid(nStartPos + 1, nEndPos - (nStartPos + 1));
		strReplaceTarget = "(" + strExtractData + ")";

		//해당자리의 실제값을 치완한다.
		CString strItem = strExtractData.Left(1);
		CString strItemIndex = strExtractData.Right(strExtractData.GetLength() - 1);
		int nIndex = atoi(strItemIndex.operator const char*());

		if (strItem.Compare("C") == 0)
		{
			if (strExtractData.Compare("CODE") == 0)
				strReplaceData = strCode;
			else
				strReplaceData = strCondition[nIndex - 1];
		}
		else if (strItem.Compare("P") == 0) 
			strReplaceData = strParam[nIndex - 1];
		else if(strItem.Compare("T") == 0) 
		{
			if (strExtractData.Compare("TR") == 0)
				strReplaceData = strTrend;
			else if (strExtractData.Compare("T2") == 0)
				strReplaceData = strType2; 
		}
		else if (strItem.Compare("R") == 0)
		{
			if (strExtractData.Compare("RANK") == 0)
				strReplaceData = strRank;
		}
		else if (strItem.Compare("S") == 0)
		{
			if (strExtractData.Compare("STERM") == 0)
				strReplaceData = strSTerm;
			else if (strExtractData.Compare("SCNT1") == 0)
				strReplaceData = strSignalCnt1;
			else if (strExtractData.Compare("SCNT2") == 0)
				strReplaceData = strSignalCnt2;
		}
		else if (strItem.Compare("E") == 0)
		{
			if (strExtractData.Compare("ETERM") == 0)
				strReplaceData = strETerm;
		}
		else if(strItem.Compare("G") == 0)
		{
			// 		if(strExtractData.Compare("G1")
			// 		{
			// 
			// 			if (strExtractData.Compare("T2") == 0)
			// 				strReplaceData = strType2; 
			// 
			// 		}
		}

		//## Composition Modify! ##
		strResultComposition.Replace((LPCTSTR)strReplaceTarget, (LPCTSTR)strReplaceData);
		strComposition = strComposition.Mid(nEndPos + 1, strComposition.GetLength() - nEndPos);
	}while(1);

	//재무지표일 경우에 한해서 MAP_TYPE,TYPE1,TYPE2 가변적으로 변하므로 그에따른 처리.
	CString strMapType = GetTagToNodeData(strTagID, "MAP_TYPE");
	if(strMapType.CompareNoCase("4001") == 0 || strMapType.CompareNoCase("4002") == 0)
	{
		if(m_pRadioFinancial1->GetCheck())
			stResultUserDefine.Condition[11] = 0;
		else if(m_pRadioFinancial2->GetCheck())
			stResultUserDefine.Condition[11] = 1;
		
		if(strResultComposition.Find("상위") >= 0 )
		{
			//MAP_TYPE
			stResultUserDefine.Type = 4002;
			//TYPE2
			stResultUserDefine.Type2 = 0;

			stResultUserDefine.IndexType = '1';

			if(stResultUserDefine.Type1 > 100)
			{
				stResultUserDefine.Type1 = stResultUserDefine.Type1 - 100;
				stResultUserDefine.Type = 4004;
			}

		}
		else if(strResultComposition.Find("하위") >= 0)
		{
			//MAP_TYPE
			stResultUserDefine.Type = 4002;
			//TYPE2
			stResultUserDefine.Type2 = 1;

			stResultUserDefine.IndexType = '1';

			if(stResultUserDefine.Type1 > 100)
			{
				stResultUserDefine.Type1 = stResultUserDefine.Type1 - 100;
				stResultUserDefine.Type = 4004;
			}
		}
		else if(strResultComposition.Find("이상") >= 0 && strResultComposition.Find("이하") >= 0)
		{
			//MAP_TYPE
			stResultUserDefine.Type = 4001;
			//TYPE2
			stResultUserDefine.Type2 = 99;

			stResultUserDefine.IndexType = '0';

			if(stResultUserDefine.Type1 > 100)
			{
				stResultUserDefine.Type1 = stResultUserDefine.Type1 - 100;
				stResultUserDefine.Type = 4003;
			}
		}
		else if(strResultComposition.Find("이상") >= 0 || strResultComposition.Find("이하") >= 0)
		{
			//MAP_TYPE
			stResultUserDefine.Type = 4001;
			//TYPE2
			stResultUserDefine.Type2 = 92;

			stResultUserDefine.IndexType = '0';

			if(stResultUserDefine.Type1 > 100)
			{
				stResultUserDefine.Type1 = stResultUserDefine.Type1 - 100;
				stResultUserDefine.Type = 4003;
			}
		}
		else
		{
			//MAP_TYPE
			stResultUserDefine.Type = 4001;
			//TYPE2
			stResultUserDefine.Type2 = 92;

			stResultUserDefine.IndexType = '0';

			if(stResultUserDefine.Type1 > 100)
//>>0000105: 조건검색 화면에서 흑자/적자 전화 조건으로 검색시 검색결과가 나오지 않는 현상
// 수정할 필요가 없어보이므로 작업내역을 막아놓음.
//			if(stResultUserDefine.Type1 > 200)
//			{
//				stResultUserDefine.Type1 = stResultUserDefine.Type1 - 200;
//				stResultUserDefine.Type = 4004;
//			}
//			else if(stResultUserDefine.Type1 > 100)
//<<
			{
				stResultUserDefine.Type1 = stResultUserDefine.Type1 - 100;
				stResultUserDefine.Type = 4003;
			}
		}		
	}

	CString strFrontResultComposition = "";
	if (m_stNowXMLInfo.nGubun == 1)
	{
		if (stResultUserDefine.IdxContiGubun == '0')
			strFrontResultComposition = "[단일] ";
		else if (stResultUserDefine.IdxContiGubun == '1')
			strFrontResultComposition = "[연속] ";
	}

	strFrontResultComposition += strResultComposition;
	strResultComposition = strFrontResultComposition;

	if (stResultUserDefine.Reverse == '1')
		strResultComposition += " (결과제외)";

	TRACE("\n\n# 맵 완성 구문 : %s", strResultComposition);

	return TRUE;
}


void CMapLoadCtrl::SetStructToMapValues(CString strTagID, User_Define* stUserDefine)
{
	//## 사용자가 설정한지표값을 구조체에서 읽어 화면에 셋팅한다.

	CString strTemp = GetTagToNodeData(strTagID, "CONTROL_NUMBER");
	int nControlCount = atoi(strTemp.operator const char*());

	CString strControlType;
	for (int i = 0 ; i <= nControlCount - 1 ; i++) 
	{
		strControlType = GetTagToControlNodeValues(strTagID, "TYPE", i);

		strTemp = GetTagToControlNodeValues(strTagID, "EDIT_LENGTH", i);
		int nFind = strTemp.Find('.', 0);
		int nIntegerLength, nRealLength;
		if (nFind > -1)
		{
			nIntegerLength = atoi(strTemp.Left(nFind));
			nRealLength = atoi(strTemp.Right(strTemp.GetLength() - (nFind + 1))); 
		}
		else
		{
			if (strTemp.IsEmpty() == TRUE)
				nIntegerLength = 0;
			else
				nIntegerLength = atoi(strTemp);
			nRealLength = 0;
		}

		//# 소숫점 예외 처리 적용 => 2005.03.22
		CString strFormat;
		strFormat.Format("%d", nRealLength);
		strFormat = "%." + strFormat + "f";

		strTemp = GetTagToControlNodeValues(strTagID, "ID", i);
		int nResourceID = atoi(strTemp.operator const char*()); 

		CString strPacketName = GetTagToControlNodeValues(strTagID, "PACKET_ID", i);
		CString strItem = strPacketName.Left(1);
		CString strItemIndex = strPacketName.Right(1);
		int nIndex = atoi(strItemIndex.operator const char*());

		CString strSetControlValues;
		if (strItem.Compare("C") == 0)
		{
			if (strPacketName.Compare("CODE") == 0)		//# 종목
			{
				strSetControlValues.Format("%.6s", stUserDefine->Code);

				while (strSetControlValues.GetLength() < 6)
					strSetControlValues = "0" + strSetControlValues;
			}
			else
				strSetControlValues.Format(strFormat, stUserDefine->Condition[nIndex - 1]);
		}
		else if (strItem.Compare("E") == 0)
		{
			if (strPacketName.Compare("ETERM") == 0)
				strSetControlValues.Format("%ld", stUserDefine->ETerm);
		}
		else if (strItem.Compare("P") == 0) 
			strSetControlValues.Format(strFormat, stUserDefine->Param[nIndex - 1]);
		else if(strItem.Compare("T") == 0) 
		{
			if (strPacketName.Compare("TR") == 0)
				strSetControlValues.Format("%ld", stUserDefine->Trend);
			else if (strPacketName.Compare("T2") == 0)
				strSetControlValues.Format("%ld", stUserDefine->Type2);
		}
		else if (strItem.Compare("R") == 0)
		{
			if (strPacketName.Compare("RANK") == 0)
				strSetControlValues.Format("%ld", stUserDefine->Rank);
		}
		else if (strItem.Compare("S") == 0)
		{
			if (strPacketName.Compare("STERM") == 0)
				strSetControlValues.Format("%ld", stUserDefine->STerm);
			else if (strPacketName.Compare("SCNT1") == 0)
				strSetControlValues.Format("%ld", stUserDefine->SignalCnt1);
			else if (strPacketName.Compare("SCNT2") == 0)
				strSetControlValues.Format("%ld", stUserDefine->SignalCnt2);
		}

		//## Find Values Set Control ##
		if (strControlType.Compare("Edit") == 0) 
		{
			CTEditCtrlWnd* pEditWnd = (CTEditCtrlWnd*)GetDlgItem(nResourceID);
			pEditWnd->SetWindowText(strSetControlValues);
			pEditWnd->Invalidate();
		}
		else if (strControlType.Compare("ComboBox") == 0) 
		{
			CComboBox* pComboBoxWnd = (CComboBox*)GetDlgItem(nResourceID);

			int nFindIndex = -1;
			CString strComboLoadType = GetTagToControlNodeValues(strTagID, "COMBO_LOAD_TYPE", i);
			if (atoi(strComboLoadType) == 1) 
			{
				CString strComboLoadData = GetTagToControlNodeValues(strTagID, "COMBO_LOAD_DATA", i);

				int nItemFileType = -1;
				CStringArray saCode;
				if (strComboLoadData.Compare("거래소") == 0)
				{
					/*
					if (((strTagID.Compare("A1") == 0) && (strPacketName.Compare("C1") == 0)) ||	//# 거래소
					((strTagID.Compare("A3") == 0) && (strPacketName.Compare("C1") == 0)))		//# 거래소 + 코스닥
					*/
					if ((strTagID.Compare("A1") == 0) || (strTagID.Compare("A3") == 0))
					{
						nItemFileType = INDUSTRY_TYPE;
						saCode.Copy(m_saCode_Kospi);
					}
				}
				else if (strComboLoadData.Compare("코스닥") == 0)
				{
					/*
					if (((strTagID.Compare("A2") == 0) && (strPacketName.Compare("C2") == 0)) || 
					((strTagID.Compare("A3") == 0) && (strPacketName.Compare("C2") == 0)))		//# 거래소 + 코스닥
					*/
					if ((strTagID.Compare("A2") == 0) || (strTagID.Compare("A3") == 0))
					{
						nItemFileType = KOSDAK_TYPE;
						saCode.Copy(m_saCode_Kosdaq);
					}
				}
				else if (strComboLoadData.Compare("계열사") == 0)
				{
					if ((strTagID.Compare("A7") == 0) && (strPacketName.Compare("C1") == 0))		//# 계열사
					{
						nItemFileType = GROUP_TYPE;

					}
				}
				else if (strComboLoadData.Compare("회원사") == 0)
				{
					if (strTagID.Compare("A28") == 0) 		//# 회원사
					{
						nItemFileType = MEMBERS_TYPE;
						saCode.Copy(m_saCode_Member);
					}
				}

				if (nItemFileType != -1)
				{
					int nChk = FALSE;
					for (nFindIndex = 0; nFindIndex < saCode.GetSize(); nFindIndex++)
					{
						strTemp = saCode.GetAt(nFindIndex);
						if (strSetControlValues == saCode.GetAt(nFindIndex))
						{
							nChk = TRUE;
							break;
						}
							
					}
					if(!nChk)
						nFindIndex = 0;
				}
			}
			else
			{
				CString	strItemData = GetTagToControlNodeValues(strTagID, "COMBO_INDEX", i);
				strItemData += "/";

				int nFindPos;
				CString strSearchData; //추출된 패킷에 들어갈 데이터

				do
				{
					nFindPos = strItemData.Find('/');
					if (nFindPos == -1) 
					{
						nFindPos = 1;
						strSearchData = strItemData.Mid(0, nFindPos);
						break;
					}
					else
					{
						strSearchData = strItemData.Mid(0, nFindPos);
						nFindIndex++;
					}

					if (atoi(strSearchData) == atoi(strSetControlValues))
						break;

					strItemData = strItemData.Mid(nFindPos + 1, strItemData.GetLength() - nFindPos);
				}while(1);
			}

			
			pComboBoxWnd->SetCurSel(nFindIndex);
			pComboBoxWnd->Invalidate();
		}
	}

	//## Set Not Check Btn ##
	if (CheckNotCheckBtnDrawState() == TRUE)
	{
		int nNotCheck = (stUserDefine->Reverse == '1') ? 1 : 0;

		CButton* pNotCheck = (CButton*)GetDlgItem(IDC_BUTTON_NOTCHECK);
		pNotCheck->SetCheck(nNotCheck);
		pNotCheck->Invalidate();
	}

	//## Set Not Check Btn ##
	if (CheckGubunCheckBtnDrawState() == TRUE)
	{
		int nGubunCheck = (stUserDefine->IdxContiGubun == '1') ? 1 : 0;
		CButton* pGubunCheck = (CButton*)GetDlgItem(IDC_BUTTON_GUBUNCHECK);
		pGubunCheck->SetCheck(nGubunCheck);

		/*
		//# Case Order => Enable
		BOOL bEnable = (stUserDefine->GroupNo > 0) ? FALSE : TRUE;
		pGubunCheck->EnableWindow(bEnable);
		*/

		pGubunCheck->Invalidate();
	}
}

void CMapLoadCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);

	CRect reClient, reChkBtn, reBtn;
	GetClientRect(&reClient);
	reBtn = reClient;

	/*{
		CBrush brush;
		brush.CreateSolidBrush(m_clrSkin);

		CClientDC dc(this);
		CBrush* pOldBrush = (CBrush*)dc.SelectObject(brush);
		dc.Rectangle(reClient);
		dc.SelectObject(pOldBrush);
		brush.DeleteObject();
	}*/
	
	reChkBtn.top = reClient.top + 5;
	reChkBtn.bottom = reChkBtn.top + 15;
	reChkBtn.right = reClient.right - 5;
	if ((CheckNotCheckBtnDrawState() == TRUE) && (CheckGubunCheckBtnDrawState() == FALSE))
	{
		reChkBtn.left = reChkBtn.right - NOTCHECK_WIDTH;
		m_pNotCheckBtn->MoveWindow(reChkBtn);	
	}
	else if ((CheckNotCheckBtnDrawState() == FALSE) && (CheckGubunCheckBtnDrawState() == TRUE))
	{
		reChkBtn.left = reChkBtn.right - GUBUNCHECK_WIDTH;
		m_pGubunCheckBtn->MoveWindow(reChkBtn);	
	}
	else if ((CheckNotCheckBtnDrawState() == TRUE) && (CheckGubunCheckBtnDrawState() == TRUE))
	{
		reChkBtn.left = reChkBtn.right - NOTCHECK_WIDTH;
		m_pNotCheckBtn->MoveWindow(reChkBtn);	

		// 		reChkBtn.right = reChkBtn.left - 5;
		// 		reChkBtn.left = reChkBtn.right - GUBUNCHECK_WIDTH;
		reChkBtn.top += 18;
		reChkBtn.bottom = reChkBtn.top + 20;
		m_pGubunCheckBtn->MoveWindow(reChkBtn);	
	}
	//20100514 SJ_KIM 조건 타이틀 제거	
	//	OnSize_ConditionTitleStatic();

	reBtn.right = reClient.right - 5;
	reBtn.left = reBtn.right - m_nConstBtnWidth;

	reBtn.bottom = reBtn.bottom - 5;
	reBtn.top = reBtn.bottom - m_nConstBtnHeight;
	if (CheckBaseBtnDrawState() == TRUE)
		m_pBaseBtn->MoveWindow(reBtn);
	//# 기본값

	//유진 컨트롤 관련 <	

	if(m_pUpdateBtn && m_pwndMain) 
	{

	}

	reBtn.bottom = reBtn.top - 2;
	reBtn.top = reBtn.bottom - m_nConstBtnHeight;
	if (CheckUpdataBtnDrawState() == TRUE)
		m_pUpdateBtn->MoveWindow(reBtn);

	//# 수정

	if(m_pBaseBtn && m_pwndMain)
	{
			
	}
	
	//유진 컨트롤 관련	>
	reBtn.bottom = reBtn.top - 2;
	reBtn.top = reBtn.bottom - m_nConstBtnHeight;
	if (CheckOKBtnDrawState() == TRUE)
		m_pOKBtn->MoveWindow(reBtn); //# 확인
}

void CMapLoadCtrl::OnSize_ConditionTitleStatic()
{
	CRect reClient, reStatic;
	int nOffsetPos = 5;
	GetClientRect(reClient);
	if ((CheckNotCheckBtnDrawState() == TRUE) && (CheckGubunCheckBtnDrawState() == FALSE))
		nOffsetPos = (int)NOTCHECK_WIDTH + 5 * 2;
	else if (CheckGubunCheckBtnDrawState() == TRUE)
		nOffsetPos = (int)NOTCHECK_WIDTH + (int)GUBUNCHECK_WIDTH + 5 * 3;

	if (CheckConditionTitleStaticDrawState() == TRUE)
	{
		reStatic.left = reClient.left + POSITION_X_TITLE;
		reStatic.right = reClient.right - nOffsetPos;

		reStatic.top = reClient.top + 5;
		reStatic.bottom = reStatic.top + 15;
		m_pConditionTitleStatic->MoveWindow(reStatic);
	}
}

BOOL CMapLoadCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			OnBtnUpdate();
		else if(pMsg->wParam == VK_ESCAPE)
			m_pwndMsgTarget->SendMessage(WM_NOTIFY_MAPLOAD, NM_MAPLOAD_CLOSE, (LPARAM)0);
	}

	return CStatic::PreTranslateMessage(pMsg);
}


HBRUSH CMapLoadCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CStatic::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		//		pDC->SetBkColor(COLOR_MAPLOAD);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrSkin);	// KB투자증권
		hbr = m_brushDefault;
	}
	else if(pWnd->GetDlgCtrlID() == IDC_BUTTON_GUBUNCHECK)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(m_clrSkin);	// KB투자증권
		hbr = m_brushDefault;
	}

	return hbr;
}


void CMapLoadCtrl::EuBtnUpdate()
{
	OnBtnUpdate();
}

void CMapLoadCtrl::EuBtnBase()
{
	OnBtnBase();
}

LRESULT CMapLoadCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{	//CConditionView -> CConditionresultwnd -> OnExpand()호출시
	//유진 (수정, 기본값)버튼 보이기/감추기 위한 플래그  
	if(message == CHANGE_EXP)
		m_TopExp = (int)lParam;

	return CStatic::WindowProc(message, wParam, lParam);
}

void CMapLoadCtrl::ChangeSkinColor(COLORREF clrBk)
{
	if(!clrBk)
		return;

	m_clrSkin = clrBk;
	if(m_brushDefault.DeleteObject())
		m_brushDefault.CreateSolidBrush(clrBk);
}

void CMapLoadCtrl::SetBase()
{
	OnBtnBase();
}

void CMapLoadCtrl::OnBtnFinancial(UINT nID)
{
	if(nID == IDC_BTN_RADIO_FINANCIAL1)
	{
		m_pRadioFinancial2->SetCheck(FALSE);
	}
	else if(nID == IDC_BTN_RADIO_FINANCIAL2)
	{
		m_pRadioFinancial1->SetCheck(FALSE);
	}
}

// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 >>
void CMapLoadCtrl::SetRadioCheckState(CString strKey, CString strData, BOOL bChecked)
{
	CString strCheckState;
	if(m_mapRadioCheck.Lookup(strKey, strCheckState))
	{
		if(atoi(strCheckState) != bChecked)
			m_mapRadioCheck.SetAt(strKey, strData);
	}
	else
		m_mapRadioCheck.SetAt(strKey, strData);
}

CString CMapLoadCtrl::GetRadioCheckState(CString strKey)
{
	CString strCheckState("-1");
	
	if(m_mapRadioCheck.Lookup(strKey, strCheckState))
		return strCheckState;
	else
		return strCheckState;
}
// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 <<