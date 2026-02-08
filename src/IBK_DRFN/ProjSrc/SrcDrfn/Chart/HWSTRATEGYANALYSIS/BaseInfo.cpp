// BaseInfo.cpp: implementation of the CBaseInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "interestdll.h"
#include "BaseInfo.h"

// start
#include "../../../DataMgr/DBMgr.h"
#include "../../../include/EnvDir.h"
#include "../../../include/Defines.h"				// HTS 용인지 Eagle 용인지 구분자 필요에 의해 추가함
#include "../../../MapDB/MapDBMgr.h"
#include "../../../CtlStd/SelComboObj.h"
#include "../../../CtlStd/CodeComboObj.h"
#include "../../../CtlStd/FundAccountComboObj.h"
#include "../../../CtlStd/SmartAccountComboObj.h"	// Eagle 전용 계좌 인풋 
// end

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseInfo::CBaseInfo()
{
	m_hParent = NULL;
	m_pMapDB = NULL;

	// -----------------------------------------
	m_pSelCode = NULL;
	m_pAcctInput = NULL;
	// -----------------------------------------
}

CBaseInfo::~CBaseInfo()
{
	CloseInfo();
}

void CBaseInfo::CloseInfo()
{
	
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}

	if(m_pAcctInput)
	{
		delete m_pAcctInput;
		m_pAcctInput = NULL;
	}
	

	// -----------------------------------------

	if(m_pMapDB)
	{
		delete m_pMapDB;
		m_pMapDB = NULL;
	}
		
}

void CBaseInfo::InitInfo(void* pDBMgr)
{
	m_pDBMgr = (CDBMgr*)pDBMgr;
	

	GET_MAIN_DIR(m_sBasePath);
	CLogUserInfo* pUserInfo = m_pDBMgr->GetLogUserData();
	m_sUserID = pUserInfo->strUserID;
}

CString CBaseInfo::GetUserPath()
{
	return (m_sBasePath + "\\User\\" + m_sUserID);
}

CString CBaseInfo::GetMainPath()
{
	return m_sBasePath;
}

HWND CBaseInfo::GetSocketHWND()
{
	return m_pDBMgr->GetCommHWnd();	
}

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/24
// Comments		: Code 관리함수들 
//START------------------------------------------------------------------------
BOOL CBaseInfo::GetCodeInfo(DWORD nCode, CStringArray& arCode, CStringArray& arName)
{
	return m_pDBMgr->GetMasterData(nCode, arCode, arName);
}

DWORD CBaseInfo::GetCodeType(LPCSTR szCode, int nECN)
{
	return m_pDBMgr->GetCodeType(szCode,nECN);
}

BOOL CBaseInfo::GetCodeName(DWORD wType, LPCTSTR szCode, CString &strName, BOOL bFullName , BOOL bAlert )
{
	return m_pDBMgr->GetCodeName(wType,szCode,strName,bFullName,bAlert);
}

void CBaseInfo::GetCodeGroup(CString sGroup, CStringArray& arCode, CStringArray& arName)
{
	m_pDBMgr->GetGroupJongMokData(sGroup, arCode, arName);
}

BOOL CBaseInfo::GetCodeIntType(DWORD nType)
{
	switch(nType)
	{
	case CODE_JONGMOK : 			
	case CODE_KJONGMOK : 
	case CODE_FUTURE : 
	case CODE_OPTION : 
	case CODE_JOPTION : 
	case CODE_OTCBB : 
	case CODE_QFUTURE : 
	case CODE_QOPTION : 
	case CODE_EXCHANGE : 
	case CODE_EXCH_AUP : 
	case CODE_EXCH_AJONG : 
	case CODE_EXCH_DR :
		return TRUE;
	}
	return FALSE;
}

int CBaseInfo::GetCodeTRType(DWORD nType)
{
	switch(nType)
	{
	case CODE_JONGMOK : return CODEN_KOSPI; 			
	case CODE_KJONGMOK : return CODEN_KOSDAQ;
	case CODE_FUTURE : return CODEN_FUTURE;
	case CODE_OPTION : return CODEN_OPTION;
	case CODE_JOPTION : return CODEN_SOPTION;
	case CODE_OTCBB : return CODEN_THIRD;
	case CODE_QFUTURE : return CODEN_QFUTURE;
	case CODE_QOPTION : return CODEN_QOPTION;
	case CODE_EXCHANGE : return CODEN_GINDEX;
	case CODE_EXCH_AUP : return CODEN_AINDEX;
	case CODE_EXCH_AJONG : return CODEN_AITEM;
	case CODE_EXCH_DR : return CODEN_DRITEM;
	}
	return 0;
}
//END------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/24
// Comments		: 서버에서 업종, 테마, 그밖의 분류에 해당하는 종목코드들을 요청
//-----------------------------------------------------------------------------
void* CBaseInfo::GetCurTrHWS09501(int nType, int nCode)
{
	CString szMarket;
	switch(nType)
	{
	case E_TREE_2KOSPI :
	case E_TREE_3KOSDOQ :
		szMarket.Format("%03d",nCode);
		return GetCurTrHWS09501(szMarket,"");
	case E_TREE_5THEME:
		szMarket.Format("%3d",nCode);
		return GetCurTrHWS09501(szMarket,"0712");
	}
	return NULL;
}

void* CBaseInfo::GetCurTrHWS09501(LPCSTR szMarket, LPCSTR szCode)
{
	m_Msg.ResetData();
	m_Msg.AddString("HWS09501", QRYCODE_LEN);
	m_Msg.AddString("N0010000", 8);
	if( strlen(szCode) == 0)
	{
		m_Msg.AddLong(4 + 4 * 2);
		m_Msg.AddLong(1);
		m_Msg.AddString("0101", 4);	// 코드
		m_Msg.AddString(szMarket, 4);		// 시장구분
	}
	else
	{
		m_Msg.AddLong(4 + 4 * 2 + strlen(szCode));

		//데이타 채우기
		m_Msg.AddLong(2);
		m_Msg.AddString("0101", 4);	// 코드
		m_Msg.AddString(szMarket, 4);		// 시장구분
		m_Msg.AddString(szCode, strlen(szCode));
	}
	return (void*)&m_Msg;
}

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/24
// Comments		: 리얼 등록 및 리얼에 관련된 함수들 
//START-------------------------------------------------------------------------
void CBaseInfo::AddRealUpdate(CMapStringToPtr *pKeyMap, HWND hWnd)
{
	m_pDBMgr->AddRealUpdate(pKeyMap,hWnd);
}

void CBaseInfo::AddRealUpdate(LPCTSTR szKey, HWND hWnd)
{
	m_pDBMgr->AddRealUpdate(szKey,hWnd);
}

void CBaseInfo::ResetRealUpdate(CMapStringToPtr *pKeyMap, HWND hWnd)
{
	m_pDBMgr->ResetRealUpdate(pKeyMap,hWnd);	
}

void CBaseInfo::ResetRealUpdate(LPCTSTR szKey, HWND hWnd)
{
	m_pDBMgr->ResetRealUpdate(szKey,hWnd);
}

WORD CBaseInfo::GetRealType(long lParam)
{
	CRealData *pData = (CRealData *)lParam;
	return pData->wRecordID;
}
//START-------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Author		: Geun-moon Oh	Date :2005/7/11
// Return void				: 
// Comments					: Delete Control CodeInput 
//-----------------------------------------------------------------------------
void CBaseInfo::DestoryControl_CodeInput()
{
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}
}

//-----------------------------------------------------------------------------
// Author					: Geun-moon Oh	Date :2005/7/11
// Return void				: 
// Comments					: Delete Control FutOptCodeCombo
//-----------------------------------------------------------------------------
void CBaseInfo::DestoryControl_FutOptCodeCombo()
{
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}
}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return void				: 
// Param  HWND hWnd			: Parent Handle
// Param  CRect rc			: CodeInput 이 생성될 위치 ( Bottom 값을 히스토리리스트 영역까지 포함해서 설정한다. )
// Param  WORD nID /*= 0*/	: 코드인풋의 리소스 ID 를 넘긴다.
// Comments					: 
//-----------------------------------------------------------------------------
void CBaseInfo::CreateControl_CodeInput(HWND hWnd, CRect rc, WORD nID /*= 0*/, 
										long lcodetype /*=CODE_JONGMOK|CODE_KJONGMOK*/,
										long lcodeMaxLength /*=6*/)
{

	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);

	//if( (lcodetype&CODE_FUTURE == CODE_FUTURE) || (lcodetype&CODE_OPTION == CODE_OPTION) )
/*	if(lisFut == CODE_FUTURE  || lisOpt == CODE_OPTION)
	{
		m_pSelCode= new CCodeComboObj;
		m_pSelCode->SetObjMode(1);
		CWnd* pWnd = CWnd::FromHandle(m_hParent);
		m_pSelCode->SetParentFrameWnd(pWnd);
		m_pSelCode->SetMapDB(m_pMapDB);
		
		m_pSelCode->CreateControl(nID,pWnd,rc);

		CPropData propdata;

		//------------------------------------------------------------------------->>
		// Create History Button
		propdata.lData = 1;
		m_pSelCode->SetProperty(PI_USER+6 , &propdata);

		//------------------------------------------------------------------------->>
		// 코드인풋에서 사용할 필드 아이템 셋팅
		propdata.strData	= "종목코드";
		m_pSelCode->SetProperty(PI_FIDITEM , &propdata);

		
		//------------------------------------------------------------------------->>
		// 필요에 의하여 SelComboObj.cpp 에 임의로 추가한 부분 구현
		// 코드인풋에서 사용할 코드 타입 셋팅
		m_pSelCode->SetProperty("DataType" , "KFut|KOpt");

		
		//------------------------------------------------------------------------->>
		// 종목코드의 최대 길이 설정
		propdata.lData = 8;
		m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);
	}
	else
	{
*/
		// If object exists, delete it.  By Gm0604 2005.7.11
		if(m_pSelCode)
		{
			delete m_pSelCode;
			m_pSelCode = NULL;
		}
		// end. By Gm0604 2005.7.11
		m_pSelCode= new CSelComboObj;
		m_pSelCode->SetObjMode(1);
		CWnd* pWnd = CWnd::FromHandle(m_hParent);
		m_pSelCode->SetParentFrameWnd(pWnd);
		m_pSelCode->SetMapDB(m_pMapDB);
		
		m_pSelCode->CreateControl(nID,pWnd,rc);

		CPropData propdata;

		//------------------------------------------------------------------------->>
		// Create History Button
		propdata.lData = 1;
		m_pSelCode->SetProperty(PI_USER+6 , &propdata);			// History Button

		long lisKospi = lcodetype&CODE_JONGMOK;
		long lisKosdaq = lcodetype&CODE_KJONGMOK;
		if(lisKospi == CODE_JONGMOK  || lisKosdaq == CODE_KJONGMOK)
		//if((lcodetype&CODE_JONGMOK == CODE_JONGMOK) || (lcodetype&CODE_KJONGMOK == CODE_KJONGMOK))
			m_pSelCode->SetProperty(PI_USER+13 , &propdata);		// "S" Button	
		

		m_pSelCode->SetProperty(PI_USER+14 , &propdata);		// "?" Button
		//m_pSelCode->SetProperty(PI_USER+15 , &propdata);		// "M" Button	

		//------------------------------------------------------------------------->>
		// 코드인풋에서 사용할 필드 아이템 셋팅
		propdata.strData	= "종목코드";
		m_pSelCode->SetProperty(PI_FIDITEM , &propdata);

		
		//------------------------------------------------------------------------->>
		// 필요에 의하여 SelComboObj.cpp 에 임의로 추가한 부분 구현
		// 코드인풋에서 사용할 코드 타입 셋팅
		propdata.lData = lcodetype;
		m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);


		//------------------------------------------------------------------------->>
		// 종목코드의 최대 길이 설정
		propdata.lData = lcodeMaxLength;
		//m_pSelCode->SetProperty(PI_USER+2 , &propdata);	-> 이렇게 설정하면 텍스트 입력시 문제가 됨
		m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);


		//------------------------------------------------------------------------->>
		// DR Mode Setting -> 코드 변경시 메시지로 알려주도록 하기 위해 
		// CChkSelCombo.OnChange -> 
		//if (m_bDRMode)
		//{
		//	CString	code;		// ssj 20020424
		//	code.Format("%s", m_curJongCode);			// ssj
		//	GetParent()->SendMessage(WMU_SET_KEYCODE , GetDlgCtrlID(), (LPARAM)(LPSTR)(LPCTSTR)code);
		//}
		propdata.lData	= 1;
		m_pSelCode->SetProperty("SetUserDefMode", &propdata);
//	}
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return void				: 
// Param  CString strCode	: 종목 코드  ex)"000660"
// Comments					: 한화 코드 인풋에 종목 코드를 셋팅한다.
//-----------------------------------------------------------------------------
void CBaseInfo::SetTextToCodeInput(CString strCode)
{
	if(m_pSelCode)
	{
		m_pSelCode->SetText(strCode);
	}
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/24
// Return CString	: 현재의 종목 코드를 리턴한다.
// Comments			: 한화 코드 인풋에 현재 셋팅 되어 있는 값을 가져온다.
//-----------------------------------------------------------------------------
CString CBaseInfo::GetTextToCodeInput()
{
	if(m_pSelCode)
	{
		return m_pSelCode->GetText();
	}
	else
		return "";
}

//void CBaseInfo::SetFocusToCodeInput()
//{
//	if(m_pSelCode)
//	{
//		CPropData propdata;
//		m_pSelCode->SetProperty("SetFocusForDR", &propdata);
//	}
//}



void CBaseInfo::CreateControl_AcctInput(HWND hWnd, CRect rc, WORD nID /*= 0*/,
										long lAcctTypeCnt /*=2*/,
										CString strAcctType /*="0102"*/)
{
	m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);
	

#ifdef __INFO			// - Eagle 전용
	m_pAcctInput= new CSmartAccountComboObj;
#else					// - HTS 전용 
	m_pAcctInput= new CAccountComboObj;
#endif



	m_pAcctInput->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pAcctInput->SetParentFrameWnd(pWnd);
	m_pAcctInput->SetMapDB(m_pMapDB);
	

	//////////////////////////////////////////////////////////////////////////
	CPropData propdata;

	//------------------------------------------------------------------------>>
	// SetUserDefUseAccTypeList  -> 사용할 계좌 타입을 설정 -> CreateControl 하기 전에 호출 
	// 계좌 구분 
	// 01 위탁
	// 02 저축
	// 03 선물
	// 04 수익
	// 05 CD
	// 06 CP
	// 07 RP
	// 08 관심주식
	// 09 관심선물
	// 10 Wrap
	propdata.lData = lAcctTypeCnt;				// -> 적용계좌 개수
	propdata.strData = strAcctType;				// -> 01 : 위탁 , 02 : 저축 계좌 설정
	m_pAcctInput->SetProperty("SetUserDefUseAccTypeList" , &propdata);
	
	//------------------------------------------------------------------------->>
	// 계좌번호 구분 표시 설정 
	propdata.lData = 1;
	m_pAcctInput->SetProperty("SetUserDefShowDash", &propdata);

	//------------------------------------------------------------------------->>
	// 타인명의 계좌 포함 설정 
	m_pAcctInput->SetProperty("SetUserDefOtherAcc", &propdata);
	
	//------------------------------------------------------------------------->>
	// SelectItem Index 설정 
	propdata.lData = 0;
	m_pAcctInput->SetProperty("SetUserDefPSelPos", &propdata);


	//------------------------------------------------------------------------->>
	// LogFont 설정 
	LOGFONT lfdata;
	strcpy(lfdata.lfFaceName, "굴림");
	lfdata.lfHeight = -12;
	lfdata.lfWidth = 0;
	lfdata.lfEscapement = 0;
	lfdata.lfOrientation = 0; 
	lfdata.lfWeight = FW_DONTCARE;
    lfdata.lfItalic = false ;    
	lfdata.lfUnderline= false;
    lfdata.lfStrikeOut= false; 
	lfdata.lfCharSet = DEFAULT_CHARSET;
    lfdata.lfOutPrecision = OUT_DEFAULT_PRECIS ;    
	lfdata.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lfdata.lfQuality = DEFAULT_QUALITY;    
	lfdata.lfPitchAndFamily = DEFAULT_PITCH;

	propdata.fontData = lfdata;
	m_pAcctInput->SetProperty("SetUserDefFontData", &propdata);


	//----------------------------------------------------------------------------->>
	// DR Mode Setting -> 계좌 변경시 메시지로 알려주도록 하기 위해 
	// CFundAccountComboCtl.OnChange -> 
	//if (m_bDRMode)
	//{
	//	GetParent()->SendMessage(WMU_SET_KEYCODE , GetDlgCtrlID(), (LPARAM)(LPSTR)(LPCTSTR)strAcc);
	//}
	propdata.lData	= 1;
	m_pAcctInput->SetProperty("SetUserDefMode", &propdata);	
	//-----------------------------------------------------------------------------<<
	
	//////////////////////////////////////////////////////////////////////////
	m_pAcctInput->CreateControl(nID,pWnd,rc);


}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/25
// Modifier		: 
// Comments		: 계좌번호 가져 오기
//-----------------------------------------------------------------------------
CString	CBaseInfo::GetTextToAcctInput()
{
	if(m_pAcctInput)
	{
		CPropData propdata;
		m_pAcctInput->GetProperty(PI_TEXT, &propdata);

		return propdata.strData;
	}
	
	return "";
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/5/25
// Return CString		 : 계좌명
// Param  CString strAcc : 계좌번호
// Comments				 : 계좌명 가져 오기 
//-----------------------------------------------------------------------------
CString CBaseInfo::GetAccountToGNameForDR(CString strAcc)
{
	if(m_pAcctInput)
	{
		CPropData propdata;
		propdata.strData = strAcc;
		
		if(m_pAcctInput->GetProperty("GetUserDefAccountToGName" , &propdata))
		{
			return propdata.strData;
		}
	}

	return "";
}

void CBaseInfo::SetEnableToAcctInput(BOOL bEnable/*=TRUE*/)
{
	return;
//	if(m_pAcctInput)
//	{
//		CPropData propdata;
//		propdata.lData = bEnable;
//
//		m_pAcctInput->SetProperty("SetEnableForDR", &propdata);
//	}
}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/6/21
// Return void : 
// Param  HWND hWnd : Parent Handle
// Param  CRect rc	: Position Rect
// Param  WORD nID	: Resource ID
// Param  CString strcodetype : Upjong Code Type
// Comments		: 업종 코드 콤보를 생성한다.
//-----------------------------------------------------------------------------
void CBaseInfo::CreateControl_UpjongCodeCombo(HWND hWnd, CRect rc, WORD nID/* = 0*/,CString strcodetype/*="CODE_ALLUPCODE"*/)
{
	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);

	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}

	m_pSelCode = new CSelComboObj;
	m_pSelCode->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pSelCode->SetParentFrameWnd(pWnd);
	m_pSelCode->SetMapDB(m_pMapDB);
	

	
	
	// 속성 셋팅 
	CPropData propdata;
	//------------------------------------------------------------------------->>
	// 업종 코드인풋에서 사용할 코드 타입 셋팅
	// 전체업종			"CODE_ALLUPCODE"
	// 장내업종			"CODE_UPJONG"
	// KOSPI200 업종	"CODE_K200UPJONG"
	// KOSDAQ 업종		"CODE_KUPJONG"
	//m_pSelCode->SetProperty("DataType" , strcodetype);
	propdata.lData = CODE_UPJONG | CODE_KUPJONG | CODE_K200UPJONG;
	m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);
	

	//------------------------------------------------------------------------->>
	// 업종코드의 최대 길이 설정
	propdata.lData = 3;
	m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);


	//------------------------------------------------------------------------->>
	// 업종코드콤보에서 사용할 필드 아이템 셋팅
	propdata.strData	= "업종구분/코드";
	m_pSelCode->SetProperty(PI_FIDITEM , &propdata);
	//-------------------------------------------------------------------------<<

	
	m_pSelCode->CreateControl(nID,pWnd,rc);


	//------------------------------------------------------------------------->>
	//2005. 07. 11
	// DR Mode Setting -> 코드 변경시 메시지로 알려주도록 하기 위해 
	propdata.lData	= 1;
	m_pSelCode->SetProperty("SetUserDefMode", &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	// 초기 공유에 의해 셋팅된 업종코드를 셋팅한다.
	char szBuf[20];
	int nKind = m_pSelCode->GetDataKind();
	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf))
	{
		if( szBuf[0] )
		{
			m_pSelCode->SetText(szBuf);
		}
	}
	//-------------------------------------------------------------------------<<
}


//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/6/21
// Return CString		  : 해당 strcode 에 대한 업종명
// Param  CString strcode : 업종코드
// Param  long lcodetype  : 업종코드 타입
// Comments				  :	업종코드에 대한 업종명을 얻어 온다.
//				- 업종코드 뿐만 아니라 종목코드 관련해서도 얻어 올수 있다.
//-----------------------------------------------------------------------------
CString CBaseInfo::GetUpjongNameToCodeForDR(CString strcode , long lcodetype /*= CODE_ALLUPCODE*/)
{
	CString strName;
	
	m_pDBMgr->GetCodeName(lcodetype, strcode, strName);

	return strName;
}

//-----------------------------------------------------------------------------
// Author		: Dae-Sung, Byun	Date :2005/6/21
// Return void	: 
// Comments		: SelCode , AcctInput , UpjongCodeCombo 관련 현재 데이터를 
//				메인쪽에 셋팅시킨다.(공유관련)		
//-----------------------------------------------------------------------------
void CBaseInfo::SetKeyCodeToMain_AllControl()
{
	int nKind;
	CString strData;

	if(m_pSelCode)
	{
		nKind = m_pSelCode->GetDataKind();
		if( nKind != CODE_NONE)
		{
			strData = m_pSelCode->GetText();
			if( !strData.IsEmpty())
			{
				AfxGetMainWnd()->SendMessage(WMU_SET_KEYCODE, (WPARAM)nKind, (LPARAM)(LPCTSTR)strData);
			}
		}
	}

	if(m_pAcctInput)
	{
		nKind = m_pAcctInput->GetDataKind();
		if( nKind != CODE_NONE)
		{
			strData = m_pAcctInput->GetText();
			if( !strData.IsEmpty())
			{
				AfxGetMainWnd()->SendMessage(WMU_SET_KEYCODE, (WPARAM)nKind, (LPARAM)(LPCTSTR)strData);
			}
		}
	}
	//-----------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/21
// Return void : 
// Param  HWND hWnd : Parent Handle
// Param  CRect rc	: Position Rect
// Param  WORD nID	: Resource ID
// Param  CString strcodetype : Upjong Code Type
// Comments		: 해외지수 코드 콤보를 생성한다.
//-----------------------------------------------------------------------------
void CBaseInfo::CreateControl_ForeignCodeCombo(HWND hWnd, CRect rc, WORD nID/* = 0*/,CString strcodetype/*="CODE_EXCH_ALL"*/)
{
	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);


	m_pSelCode = new CSelComboObj;
	m_pSelCode->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pSelCode->SetParentFrameWnd(pWnd);
	m_pSelCode->SetMapDB(m_pMapDB);
	
	
	// 속성 셋팅 
	CPropData propdata;
	//------------------------------------------------------------------------->>
	// 해외 코드인풋에서 사용할 코드 타입 셋팅
	//CODE_EXCHANGE	 해외 국가지수(구:해외지수 및 환율)	
	//CODE_EXCH_AUP	 미국 지수/업종						
	//CODE_EXCH_AJONG	 미국 종목(DOW30, NASDAQ100, S&P500)	
	//CODE_EXCH_DR	 해외 DR종목							
	//m_pSelCode->SetProperty("DataType" , strcodetype);
	propdata.lData = CODE_EXCHANGE | CODE_EXCH_AUP | CODE_EXCH_AJONG | CODE_EXCH_DR;
	m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);

	//------------------------------------------------------------------------->>
	// 해외지수코드의 최대 길이 설정
	propdata.lData = 6;
	m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);


	//------------------------------------------------------------------------->>
	// 해외지수코드콤보에서 사용할 필드 아이템 셋팅
	propdata.strData	= "해외지수구분/코드";
	m_pSelCode->SetProperty(PI_FIDITEM , &propdata);
	//-------------------------------------------------------------------------<<
	
	m_pSelCode->CreateControl(nID,pWnd,rc);

	//------------------------------------------------------------------------->>
	//2005. 07. 11
	// DR Mode Setting -> 코드 변경시 메시지로 알려주도록 하기 위해 
	propdata.lData	= 1;
	m_pSelCode->SetProperty("SetUserDefMode", &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	// 초기 공유에 의해 셋팅된 해외지수코드를 셋팅한다.
	char szBuf[20];
	int nKind = m_pSelCode->GetDataKind();
	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf))
	{
		if( szBuf[0] )
		{
			m_pSelCode->SetText(szBuf);
		}
	}
	//-------------------------------------------------------------------------<<
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/21
// Return CString		  : 해당 strcode 에 대한 해외지수명
// Param  CString strcode : 해외지수코드
// Param  long lcodetype  : 해외지수코드 타입
// Comments				  :	해외지수코드에 대한 해외지수명을 얻어 온다.
//				- 해외지수코드 뿐만 아니라 종목코드 관련해서도 얻어 올수 있다.
//-----------------------------------------------------------------------------
CString CBaseInfo::GetForeignNameToCodeForDR(CString strcode , long lcodetype /*= CODE_EXCH_ALL*/)
{
	CString strName;
	
	m_pDBMgr->GetCodeName(lcodetype, strcode, strName);

	return strName;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/23
// Return CString		  : 해당 strcode 에 대한 선물옵션명
// Param  CString strcode : 선물옵션코드
// Param  long lcodetype  : 선물옵션코드 타입
// Comments				  :	선물옵션코드에 대한 선물옵션명을 얻어 온다.			
//-----------------------------------------------------------------------------

void CBaseInfo::CreateControl_FutOptCodeCombo(HWND hWnd, CRect rc, WORD nID /*= 0*/, 
										CString  strcodetype /*=CODE_FUTURE|CODE_OPTION*/,
										long lcodeMaxLength /*=8*/)
{

	if(!m_hParent)
		m_hParent = hWnd;
	
	if(!m_pMapDB)
		m_pMapDB = new CMapDB(m_hParent, m_pDBMgr->GetCommHWnd(),m_pDBMgr);

	// If object exists, delete it.  By Gm0604 2005.7.11
	if(m_pSelCode)
	{
		delete m_pSelCode;
		m_pSelCode = NULL;
	}
	// end. By Gm0604 2005.7.11
		
	m_pSelCode= new CSelComboObj;
	m_pSelCode->SetObjMode(1);
	CWnd* pWnd = CWnd::FromHandle(m_hParent);
	m_pSelCode->SetParentFrameWnd(pWnd);
	m_pSelCode->SetMapDB(m_pMapDB);
	
	m_pSelCode->CreateControl(nID,pWnd,rc);

	CPropData propdata;

	//------------------------------------------------------------------------->>
	// Create History Button
	//propdata.lData = 1;
	//m_pSelCode->SetProperty(PI_USER+6 , &propdata);		
	//-------------------------------------------------------------------------<<
	

	//------------------------------------------------------------------------->>
	// 코드인풋에서 사용할 필드 아이템 셋팅
	propdata.strData	= "종목코드/선물옵션";
	m_pSelCode->SetProperty(PI_FIDITEM , &propdata);

	
	//------------------------------------------------------------------------->>
	// 필요에 의하여 SelComboObj.cpp 에 임의로 추가한 부분 구현
	// 코드인풋에서 사용할 코드 타입 셋팅	
	//m_pSelCode->SetProperty("DataType" , strcodetype);
	if(strcodetype.CompareNoCase("allcode") == 0)
	{
		propdata.lData =	CODE_FUTURE		|
							CODE_OPTION		|
							CODE_QFUTURE	|
							CODE_QFUTURES	|
							CODE_FSPREAD	|
							CODE_QOPTION	|
							CODE_QOPTIONS	|
							CODE_JOPTION	;
	}
	else
	{
		propdata.lData =	CODE_FUTURE		|
							CODE_OPTION		;
	}

	m_pSelCode->SetProperty("SetUserDefCodeType" , &propdata);
	//-------------------------------------------------------------------------<<


	
	//------------------------------------------------------------------------->>
	// 종목코드의 최대 길이 설정
	propdata.lData = 12;
	m_pSelCode->SetProperty("SetUserDefMaxLength" , &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	//2005. 07. 11
	// DR Mode Setting -> 코드 변경시 메시지로 알려주도록 하기 위해 
	propdata.lData	= 1;
	m_pSelCode->SetProperty("SetUserDefMode", &propdata);
	//-------------------------------------------------------------------------<<
	
	//------------------------------------------------------------------------->>
	// 초기 공유에 의해 셋팅된 해외지수코드를 셋팅한다.
	char szBuf[20];
	int nKind = m_pSelCode->GetDataKind();
	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf))
	{
		if( szBuf[0] )
		{
			m_pSelCode->SetText(szBuf);
		}
	}
	//-------------------------------------------------------------------------<<
}

//-----------------------------------------------------------------------------
// Author		: Sang-Yun, Nam	Date :2005/6/23
// Return CString		  : 해당 strcode 에 대한 해외지수명
// Param  CString strcode : 해외지수코드
// Param  long lcodetype  : 해외지수코드 타입
// Comments				  :	해외지수코드에 대한 해외지수명을 얻어 온다.
//				- 해외지수코드 뿐만 아니라 종목코드 관련해서도 얻어 올수 있다.
//-----------------------------------------------------------------------------
CString	CBaseInfo::GetFutOptNameToCodeForDR(CString strcode , long lcodetype /* CODE_EXCH_ALL */)
{
	CString strName;
	
	m_pDBMgr->GetCodeName(lcodetype, strcode, strName);

	return strName;
}

//-----------------------------------------------------------------------------
// Author		: Ki-Tea, Yoo	Date :2005/6/29
// Modifier		: 
// Comments		: 공유 등록(advise) 및 제거(unadvise)
//-----------------------------------------------------------------------------
//#include "new/include/SharedBase.h"
//1. SharedName : 반드시 ini에 등록시킨다음 사용 한다.
//2. pSharedbase : m_hChild -> 반드시 핸들을 유효한 핸드을 넣어주어야 한다. 없으면 메인 핸들이라도 넣어두어야 한다.
//                 m_hView -> 범위를 설정할때 필요하다. 넣어도 되고 안넣어도 된다.
//                 m_hCtrl -> 여기에는 핸들이 없는 녀석인 경우는 Unique ID를 줘야 한다. this 포인트라도 주자.
//                 Set할때 자신을 다시 한번 안 부를려면 그렇게 해야 하고 제거할때도 필요하므로 반드시 있어야 한다.
//3. nSharedType : OR 조합.
//#define DE_SHARDSET		1
//#define DE_SHARDGET		2
//#define DE_SHARDSTART	4
//#define DE_SHAREDALL DE_SHARDSET|DE_SHARDGET
//4. nSharedArea
//#define DE_AREA_GENERAL 0  // 일반적 원칙에 따름.
//#define DE_AREA_FREE    1  // 현재 액티브된 버츄얼 데스크에만 적용(예외) 
//#define DE_AREA_ALL		2  // 전체적으로 적용(예외)	 
//5. nGroup
//DE_AREA_GENERAL일 경우만 가능하다. 특별한 그룹번호를 주어 서로 그 번호로만 통신할때 사용 가능  
BOOL CBaseInfo::AdviseSharedInfo(CString sShareName, CSharedBase* pSharedBase, int nSharedType, int nSharedArea, int nGroup)
{
	return m_pDBMgr->AdviseSharedInfo(sShareName,pSharedBase,nSharedType,nSharedArea,nGroup);
}

BOOL CBaseInfo::UnAdviseSharedInfo(CString sShareName, CSharedBase* pSharedBase)
{
	return m_pDBMgr->UnAdviseSharedInfo(sShareName, pSharedBase);
}

// 일반적인 경우 이 공유를 사용 한다.
//1.hCtrl :  Set을 걸 경우 자기는 빼고 다른 녀석들한테만 공유를 걸 경우 사용한다. Advise에서 m_hCtrl에 준 그 핸들 또는 유니크포인트
//2.sShareName : 공유 이름
//3.pData : 공유 데이터
//4.nLen : 공유 데이터 크기
//5.nSotck : 어떤 특정시장 데이터라는 표시 (코스닥,코스피, 선물,옵션 ,,, 구분값은 아직 결정 안됨)
//6.bOnlySetData : 오직 쉐어드 버퍼에 데이터만 만들때 사용. 
BOOL CBaseInfo::SetSharedData(HWND hCtrl,CString sShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData)
{
	return m_pDBMgr->SetSharedData(hCtrl,sShareName,pData,nLen,nStock,bOnlySetData);
}

// 주로 스크립트나 Advise되지 않는 곳에서 공유를 보낼때 사용한다. 범위 지정을 잘 해야 한다.
//1. hArea : 공유가 적용될 핸들 (반드시 현재 떠 있는 윈도우 핸들이어야 한다.)
//2. nArea : 아래 범위에서만 공유가 적용될수 있게 범위 한정. 위의 핸들은 이범위의 핸들이어야만 한다. 
//#define AREA_ALL	0
//#define AREA_VIEW	1
//#define AREA_CHILD	2
//#define AREA_GROUP	3
//#define AREA_MAIN	4
//3. 공유 데이터
//4. 공유 데이터 크기
//5. 어떤 특정시장 데이터 표시
//6. bOnlySetData : 오직 버퍼에 데이터만 저장.
BOOL CBaseInfo::SetSharedData2(HWND hArea,int nArea, CString sShareName,BYTE* pData, int nLen, int nStock, BOOL bOnlySetData)
{
	return m_pDBMgr->SetSharedData2(hArea,nArea,sShareName,pData,nLen,nStock,bOnlySetData);
}

// return 값이 NULL이면 버퍼에 해당 공유값이 없는 것임. 
BYTE* CBaseInfo::GetSharedData(CString sShareName, int& nLen, int& nStock)
{
	return m_pDBMgr->GetSharedData(sShareName,nLen,nStock);
}

///////////////////////////////////////////////////////////////////////////
// 컬러 테이블 사용 방법 
COLORREF CBaseInfo::GetColor(int nID)
{
	m_MapColor.m_lClIdx = nID;
	return m_MapColor.GetColor();
}
///////////////////////////////////////////////////////////////////
void CBaseInfo::ShowCodeCombo(int nShow)
{
	m_pSelCode->ShowControl(nShow);
}
void CBaseInfo::SetWindowRectCodeCombo(CRect rect)
{
	m_pSelCode->ReposWindow(rect);
}
void CBaseInfo::EnabelCodeCombo(BOOL bEnable)
{
	HWND hWnd = m_pSelCode->GetCtrlWindow();
	CWnd* cWnd = CWnd::FromHandle(hWnd);
	cWnd->EnableWindow(bEnable);
}

