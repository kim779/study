// CodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CodeCtrl.h"
#include "./axisfire.h"
#include "../../SrcDrfn/inc/IMasterDataMng.h"
#include "../../SrcDrfn/inc/DRCommon.h"

#include <atlbase.h>
extern CComModule _Module;
#include <atlwin.h>
#include <atlcom.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		CCodeCtrl_BtnCodeDlg		2
#define		CCodeCtrl_EditCode			3

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl

CCodeCtrl::CCodeCtrl(CWnd* wnd)
:CWinixCodeCtrl(wnd)
{
	m_bUseCodeDlg = FALSE;
	m_nCodeType = GU_CODE;//kospiCODE;	

	m_bUseFutureOption = true;
	m_bFOJipyo = false;
	m_bFirstCalled = true;
	m_bFixUnit = false;
}

CCodeCtrl::~CCodeCtrl()
{
	m_btnImageList.DeleteImageList();
}

BEGIN_MESSAGE_MAP(CCodeCtrl, CWinixCodeCtrl)
	//{{AFX_MSG_MAP(CCodeCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(CCodeCtrl_BtnCodeDlg, OnBtnCodeDlg)
	ON_CBN_SELENDOK(CCodeCtrl_BtnCodeDlg, OnSelendokCodeCtrl)
END_MESSAGE_MAP()

int CCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWinixCodeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

//KHD	if(m_bUseCodeDlg)
//KHD		_DoCreateCodeDlgBtn();

//KHD	m_editCode.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, CRect(0,0,100,20), this, CCodeCtrl_EditCode);
//KHD	m_editCode.ShowWindow(SW_HIDE);

	return 0;
}

// #define kospiType		1			// if kind == codeType, *type return (kospiType ~ indexType)
// #define kosdaqType		2
// #define thirdType		3			
// #define mufundType		4			// 뮤추얼펀드
// #define reitsType		5			// 리츠
// #define etfType			6			// ETF
// #define sinjuType		7			// 신주인수권
// #define hyfundType		8			// 하일리펀드
// #define elwType			9			// elw
// #define foreignType		10			// 외국주
// #define futureType		11			// 선물
// #define callType			12			// 콜옵션
// #define putType			13			// 풋옵션
// #define spreadType		14			// 스프레드
// #define koptionType		15			// 주식옵션
// #define sfutureType		16			// 주식선물
// #define indexType		20			// 업종
// #define smbondType		21			// 소매채권
// #define conbondType		22			// 전환사채
// #define babybondType		23			// 소액채권
// #define reitsallType		24			// 리츠전종목
// #define shipType			25			// 선백
// #define infraType		26			// 인프라
bool CCodeCtrl::SetUnit(int nCodeType)
{
//	if(m_nCodeType==nCodeType) return true; : KHD : 주선과 해외가 CodeType이 같아서 예외처리 지움(아래에 Unit별로 구분하는 기능 있음) 
//KHD
 	CString strUnit;
	switch(nCodeType)
	{
		case kospiType:
		case kosdaqType:
		case etfType	:
		case etnType:
			strUnit = "주식";
			m_nCodeType= GU_CODE;
			break;

		case elwType	:
			m_nCodeType= GU_ELWCODE;
			strUnit = "ELW";
			break;

//KHD		case usType:
		case foreignType:
			strUnit = "해외";
			m_nCodeType= GU_FOREIGN;
			//m_nCodeType= usType;
			break;

		case futureType:
			strUnit = "선물옵션";
	//		if(m_bUseFutureOption)
	//			m_nCodeType= linkFCODE;//foptionCODE;
	//		else
				m_nCodeType= GU_FOCODE;//GU_FUTURE;
			break;

		case callType	:
		case putType	:
			strUnit = "선물옵션";
	//		if(m_bUseFutureOption)
	//			m_nCodeType= linkFCODE;//foptionCODE;
	//		else
			m_nCodeType= GU_OPTION;
			break;
/*		case spreadType:	//스프레드
			strUnit = "선물옵션";
			//m_nCodeType= spreadCODE;
	//		if(m_bUseFutureOption)
	//			m_nCodeType= linkFCODE;//foptionCODE;
	//		else
				m_nCodeType= GU_FUTURE;
			break;
*/
		case koptionType:	// 주식옵션
			strUnit = "주식옵션";
			m_nCodeType= GU_POPTION;
			break;

		case sfutureType:	//주식선물
			strUnit = "주식선물";
			m_nCodeType= GU_FCODE;
			break;

		case indexType:
			strUnit = "업종";
			m_nCodeType= GU_INDEX;
			break;

//		case foindexType:
//			strUnit = "FO지표";
//			m_nCodeType= foindexCODE;
//			break;

//		case USD_OPTION:
//			strUnit = "달러옵션";
//			m_nCodeType= coptionCODE;
//			break;

//		case gfutureType:
//			strUnit = "해외선물";
//			//nCodeType= gfutureCODE;
//			m_nCodeType= linkGFCODE;
//			break;

		//case gfxType:
		//	strUnit = "FX";
		//	m_nCodeType= gfxCODE;
		//	break;
			
		//case thirdType:		m_nCodeType= ;	break;
		//case mufundType:	m_nCodeType= ;	break;
		//case reitsType:		m_nCodeType= ;	break;
		//case sinjuType:		m_nCodeType= ;	break;
		//case hyfundType:	m_nCodeType= ;	break;
		//case smbondType:	m_nCodeType= ;	break;
		//case conbondType:	m_nCodeType= ;	break;
		//case babybondType:	m_nCodeType= ;	break;
		//case reitsallType:	m_nCodeType= ;	break;
		//case shipType	:	m_nCodeType= ;	break;
		//case infraType:		m_nCodeType= ;	break;
		default:
			//if (nCodeType >= THREE_BONDS && nCodeType <= MGOLD_FUTURE)
			if (nCodeType >= THREE_BONDS && nCodeType <= NEW_TEN_BONDS+20)
			{
				strUnit = "상품선물옵션";
				m_nCodeType= GU_BOND;
				break;
			}

			strUnit = "주식";
			m_nCodeType= GU_CODE;
			break;
	}
//END
	if(m_szOldUnit==strUnit) return true;

	m_szOldUnit = strUnit;
	return CWinixCodeCtrl::SetUnit(m_nCodeType);
/*
	bool (APIENTRY *axSetUnit)(CWnd* , CString);
	axSetUnit = (bool (APIENTRY*)(CWnd*, CString))GetProcAddress(m_hiCodeCtrl, "axSetUnit");
	if (!axSetUnit)
		return false;

	return axSetUnit(m_pwndCodeCtrl, strUnit);
*/
}

typedef struct {
	LPVOID pClassWnd;
	CString szUnit;
} ST_CodeSetUnit;

UINT CCodeCtrl_SetUnit(LPVOID lpVoid)
{
	ST_CodeSetUnit* pSTUnit = (ST_CodeSetUnit*)lpVoid;
	
	CString strUnit(pSTUnit->szUnit);
	
	CCodeCtrl* pCtrl = (CCodeCtrl*)pSTUnit->pClassWnd;
	pCtrl->SetUnitEx(strUnit);
	delete pSTUnit;
	
	return 0L;
}

bool CCodeCtrl::SetUnitEx(CString strUnit)
{
	if (strUnit.Compare("FO지표") == 0)
	{
		m_bFOJipyo = true;
	//KHD	m_editCode.ShowWindow(SW_SHOW);
		if (m_pwndCodeCtrl)
			m_pwndCodeCtrl->ShowWindow(SW_HIDE);
	}
	else
	{
		int nCodeType = kospiType;
		
		if (strUnit.CompareNoCase("주식") == 0)				nCodeType = kospiType;
		else if (strUnit.CompareNoCase("ELW") == 0)			nCodeType = elwType;
		else if (strUnit.CompareNoCase("해외") == 0)		nCodeType = foreignType;
		else if (strUnit.CompareNoCase("선물옵션") == 0)	nCodeType = futureType;
		else if (strUnit.CompareNoCase("주식옵션") == 0)	nCodeType = koptionType;
		else if (strUnit.CompareNoCase("주식선물") == 0)	nCodeType = sfutureType;
		else if (strUnit.CompareNoCase("업종") == 0)		nCodeType = indexType;
		else if (strUnit.CompareNoCase("상품선물") == 0)	nCodeType = THREE_BONDS;
//KHD		else if (strUnit.CompareNoCase("해외선물") == 0)	nCodeType = gfutureType;
//KHD		else if (strUnit.CompareNoCase("FX") == 0)			nCodeType = gfxType;

		if (nCodeType != foreignType)
		{
			m_bFOJipyo = false;
	//KHD		m_editCode.ShowWindow(SW_HIDE);
			if (m_pwndCodeCtrl)
				m_pwndCodeCtrl->ShowWindow(SW_SHOW);
		}
		
		return SetUnit(nCodeType);
	}

	return FALSE;
}

bool CCodeCtrl::SetUnit(CString strUnit)
{
	if(m_szCodeUnit.Compare(strUnit)==0) return true;
	m_szCodeUnit = strUnit;

// 	if(strUnit.CompareNoCase("주식") == 0)
// 	{
// 		if(m_bFirstCalled)
// 		{
// 			m_bFirstCalled = false;
// 			
// 			int nSize = strUnit.GetLength();
// 			
// 			if(nSize>0)
// 			{
// 				ST_CodeSetUnit * pUnit = new ST_CodeSetUnit;
// 				pUnit->pClassWnd = (LPVOID)this;
// 				pUnit->szUnit = strUnit;
// 				
// 				AfxBeginThread(CCodeCtrl_SetUnit, (LPVOID)pUnit);
// 				return 0;
// 			}
// 		}
// 	}

	{
		if (strUnit.Compare("FO지표") == 0)
		{
			m_bFOJipyo = true;
		//KHD	m_editCode.ShowWindow(SW_SHOW);
			if (m_pwndCodeCtrl)
				m_pwndCodeCtrl->ShowWindow(SW_HIDE);
		}
		else
		{
			int nCodeType = kospiType;
			
			if (strUnit.CompareNoCase("주식") == 0)				nCodeType = kospiType;
			else if (strUnit.CompareNoCase("ELW") == 0)			nCodeType = elwType;
			else if (strUnit.CompareNoCase("해외") == 0)		nCodeType = foreignType;
			else if (strUnit.CompareNoCase("선물") == 0)		nCodeType = futureType;
			else if (strUnit.CompareNoCase("선물옵션") == 0)	nCodeType = futureType;
			else if (strUnit.CompareNoCase("주식옵션") == 0)	nCodeType = koptionType;
			else if (strUnit.CompareNoCase("주식선물") == 0)	nCodeType = sfutureType;
			else if (strUnit.CompareNoCase("업종") == 0)		nCodeType = indexType;
			else if (strUnit.CompareNoCase("상품선물") == 0)	nCodeType = THREE_BONDS;
//KHD			else if (strUnit.CompareNoCase("해외선물") == 0)	nCodeType = gfutureType;
//KHD			else if (strUnit.CompareNoCase("FX") == 0)			nCodeType = gfxType;
			
			if (nCodeType != foreignType)
			{
				m_bFOJipyo = false;
		//KHD		m_editCode.ShowWindow(SW_HIDE);
				if (m_pwndCodeCtrl)
					m_pwndCodeCtrl->ShowWindow(SW_SHOW);
			}

			m_bFirstCalled = false;			
			return SetUnit(nCodeType);
		}
	}
	
	return true;
}

bool CCodeCtrl::SetHistory(LPCSTR  szCode, LPCSTR szSymbol)
{
	CString szSetCode;
	szSetCode.Format("%s\t%s", szSymbol, szCode);
	
//KHD	m_pwndWizard->SendMessage(WM_USER, setHISTORY, (LPARAM)(LPCSTR)szSetCode);
	
	return true;
}

void CCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	//CWinixCodeCtrl::OnSize(nType, cx, cy);
	CWnd::OnSize(nType, cx, cy);
	
	int nDlgBtnWidth=0;
	if(m_bUseCodeDlg && m_btnCodeDlg.GetSafeHwnd())
	{
		nDlgBtnWidth = 20;
		m_btnCodeDlg.SetWindowPos(NULL, cx-nDlgBtnWidth, 0, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}

//KHD	m_editCode.MoveWindow(0, 0, cx-nDlgBtnWidth, cy);

	if (m_pwndCodeCtrl && IsWindow(m_pwndCodeCtrl->m_hWnd))
		m_pwndCodeCtrl->MoveWindow(0, 0, cx-nDlgBtnWidth, cy);
}

BOOL CCodeCtrl::_DoCreateCodeDlgBtn()
{
	CBitmap bitmap;
	CString strBitmapFile = m_szRootPath + "\\Image\\";
	
	if(!m_btnImageList.Create(20, 20, ILC_MASK | ILC_COLORDDB, 1, 1) )
	{
		AfxMessageBox("Can not Find W15_H15.bmp file");	
		return FALSE;
	}
	
	bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile + "검색.bmp", IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
	m_btnImageList.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();
	
	bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile + "검색_DN.bmp", IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
	m_btnImageList.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();
	
	bitmap.Attach ((HBITMAP)::LoadImage(NULL, strBitmapFile + "검색_EN.bmp", IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));
	m_btnImageList.Add(&bitmap, RGB(255, 0, 255));
	bitmap.DeleteObject();
	
	m_btnCodeDlg.Create("",WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|WS_CLIPSIBLINGS,CRect(0,0,20,20), this, CCodeCtrl_BtnCodeDlg);
	m_btnCodeDlg.FP_vSetImage(&m_btnImageList, 0, 1, FALSE, "", FALSE, -1, 2);
	
	return TRUE;
}

void CCodeCtrl::SetFixUnit(CString strUnit)
{
	SetUnit(strUnit);
	m_nFixUnit = m_nCodeType;
	m_bFixUnit = true;
}
void CCodeCtrl::SetFixUnit(int nFixUnit)
{
	m_nFixUnit = nFixUnit;
	m_bFixUnit = true;
}

//종목코드창열기 : IBK에선 안쓴다. KHD
void CCodeCtrl::OnBtnCodeDlg()
{
	return;//KHD
// 	if(m_bFixUnit)
// 	{
// 		m_nCodeType = m_nFixUnit;
// 	}
// 	else
// 	{
// 		CString szUnit,strCode;
// 		strCode = GetCode();
// 		int nLen = strCode.GetLength();
// 		if(nLen>0)
// 		{
// 			//		BYTE nCodeType = (BYTE)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);
// 			int nCodeType = (int)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);
// 			//값을 제대로 주지 않는 경우가 있어서 불필요한 아래의 루틴을  추가함.
// 			if (nCodeType<0 ||  nCodeType == 255)
// 			{
// 				IMasterDataManagerLast* pMasterMng = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
// 				if(pMasterMng)
// 				{
// 					BYTE cCodeType = pMasterMng->GetCodeTypeFromCode(strCode);
// 					
// 					switch(cCodeType)
// 					{
// 						//case CMasterTypeInfo::STOCK_CHART:		nCodeType = kospiType; break;
// 						//case CMasterTypeInfo::UPJONG_CHART:		nCodeType = indexType; break;
// 					case CDRCodeTypeInfo::CODE_KOSPI_STOCK:
// 						nCodeType = kospiType;
// 						break;
// 					case CDRCodeTypeInfo::CODE_KOSDAQ_STOCK:
// 						nCodeType = kosdaqType;
// 							break;
// 					case CDRCodeTypeInfo::CODE_ELW:
// 						nCodeType = thirdType;
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_KOSPI_UPJONG:
// 					case CDRCodeTypeInfo::CODE_KOSDAQ_UPJONG:
// 						nCodeType = indexType;
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_KFUTURE:
// 					case CDRCodeTypeInfo::CODE_FUTURE_SPREAD:
// 					case CDRCodeTypeInfo::CODE_KOPTION:
// //KHD						nCodeType = linkFCODE; 
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_STOCKFUT:
// 						nCodeType = sfutureType;
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_STOCKOPT:
// 						nCodeType = koptionType;
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_US_ITEM:
// 						nCodeType = foreignType; 
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_FUOPT_JIPYO:
// //KHD						nCodeType = foindexType;
// 						break;
// 						
// 					case CDRCodeTypeInfo::CODE_FUOPT_COMMODITY:
// 						nCodeType = THREE_BONDS;
// 						break;
// 					}
// 				}
// 			}
// 			if(nCodeType>=0 && nCodeType!=255)	SetUnit(nCodeType);
// 		}
// 		//입력된 코드가 없을 때는 종합차트의 셋팅을 물어봐서 셋팅하도록 한다.
// 		else
// 		{
// 			WPARAM wp = MAKEWPARAM(drbt_GETINPUTBARMARKET, 0);
// 
// 			int nMarketType = m_pwndParent->GetParent()->SendMessage(RMSG_DRUSER, wp, 0);	//kospiType;
// 			//if(nCodeType>=0 && nCodeType!=255)	SetUnit(nCodeType);	
// 
// 			CString strUnit;
// 			switch(nMarketType)
// 			{
// 				case 0:	strUnit = "주식"; break;
// 				case 1:	strUnit = "업종"; break;
// 				case 2:	strUnit = "선물옵션"; break;
// 				case 3:	strUnit = "주식옵션"; break;
// 				case 4:	strUnit = "해외"; break;
// 				case 5:	strUnit = "FO지표"; break;
// 				case 6:	strUnit = "상품선물"; break;
// 				default:break;
// 			}
// 			if(strUnit.IsEmpty()==FALSE) SetUnitEx(strUnit);
// 		}
// 
// 	}
// 	
// 	CPoint pt;
// 	CRect rcBtn;
// 	m_btnCodeDlg.GetWindowRect(&rcBtn);
// 	pt.x = rcBtn.left;
// 	pt.y = rcBtn.bottom;
// 
// 	if(m_nCodeType==usCODE)
// 	{		
// 		CString str = (char*)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, systemCC), (LPARAM)(LPCTSTR)"");
// 
// 		if (str ==  "F") // King Pro 
// 			m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL,  MAKEWORD(usCODE, 100)), MAKELPARAM(pt.x, pt.y));        
// 		else    // Other     
// 			m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL,  MAKEWORD(usCODE, 0)), MAKELPARAM(pt.x, pt.y));
// 	}
// 	else
// 	{
// 		m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeDLL, m_nCodeType), MAKELPARAM(pt.x, pt.y));
// 	}
}

void CCodeCtrl::OnSelendokCodeCtrl() 
{
	CString strCode = GetCode();
	//AfxMessageBox(strCode);
	
	// 	if(m_pMSDataMng)
	// 	{
	// 		SetChartActivation(CENTER_CHART);	//Center Chart 활성화
	// 		SendTr_ChartData(strCode);
	// 		m_ctrlCodeName.SetWindowText(m_pMSDataMng->GetCodeName(strCode));
	// 	}
}
// IBK 추가 함수 
CString CCodeCtrl::GetCode()
{
	if (!m_bFOJipyo)
		 return GetCtrlProperty("Data");
		//return CWinixCodeCtrl::GetCode();

	return "";
//	CString strCode;
//	m_editCode.GetWindowText(strCode);
//	return strCode;
}

bool CCodeCtrl::SetCode(CString strCode, bool bOnlySet)
{
	//if(m_nCodeType==koptionCODE||m_nCodeType==sfCODE)
	if(m_nCodeType==GU_POPTION||m_nCodeType==GU_FCODE)
	{
		int nCodeType = (int)m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);
		switch(nCodeType)
		{
		case GU_FCODE:	//sfutureType:
			//m_nCodeType = GU_FCODE;		//sfCODE;
			SetUnit("주식선물");
			break;				
		case GU_POPTION:	//koptionType:
			//m_nCodeType = GU_POPTION;	//koptionCODE;
			SetUnit("주식옵션");
			break;
		}
	}
	SetCtrlProperty("Data", _variant_t(strCode));
//	SetCode(strCode);
//	if (!m_bFOJipyo)
//		return CWinixCodeCtrl::SetCode(strCode, bOnlySet);
		//return CWinixCodeCtrl::SetCode(strCode, bOnlySet, true);

//	m_editCode.SetWindowText(strCode);

	return true;
}




/////////////////////////////////////////////////////////////////////////////
// CWinixCodeCtrl

CWinixCodeCtrl::CWinixCodeCtrl(CWnd* wnd)
:m_pwndWizard(wnd)
{
	m_nID = 0;

	m_hiCodeCtrl = NULL;

	m_pwndCodeCtrl = NULL;
	m_pwndParent = NULL;
	m_pwndWizard = wnd;
	m_pCodeSendWnd = NULL;
	m_bIsComareChart = FALSE;
//	m_pwndWizard = NULL;
}

CWinixCodeCtrl::~CWinixCodeCtrl()
{
	if (m_pwndCodeCtrl)
	{
		m_pwndCodeCtrl->DestroyWindow();
		delete m_pwndCodeCtrl;
		m_pwndCodeCtrl = NULL;
	}

	if (m_hiCodeCtrl)
	{
		BOOL bResult = FreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
	}
}


BEGIN_MESSAGE_MAP(CWinixCodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CWinixCodeCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_COMMAND, OnCommand)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWinixCodeCtrl message handlers

LRESULT CWinixCodeCtrl::OnUser( WPARAM wParam, LPARAM lParam )
{
	BYTE bt = LOBYTE(HIWORD(wParam));
	//CString szData((char*)lParam);
	switch(LOWORD(wParam))
	{
		case variantDLL:
			if (HIWORD(wParam) == guideCC)
				return m_pwndWizard->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_GUIDE,0), strlen((LPCSTR)lParam)), lParam );
			else if( bt == 2  || bt == 6)// 6은 주식선옵
			{
				if(m_bIsComareChart == TRUE)//비교차트일때.. 
				{
					(m_pwndParent)->SendMessage(WM_USER+94, 0, 0);
					return TRUE	;
				}
				if(m_pwndWizard!=NULL)
					return m_pCodeSendWnd->SendMessage(WM_USER, 21, lParam);			
			}
		break;
	}
	return m_pwndWizard->SendMessage(WM_USER, wParam, lParam);
}
BOOL CWinixCodeCtrl::CreateCodeCtrl(CWnd* pwndParent, CWnd* pwndWizard, CWnd* pwndSendWnd, CRect rect, UINT nID)
{
	m_pwndParent = pwndParent;
	m_pwndWizard = pwndWizard;
	m_pCodeSendWnd = pwndSendWnd;
	m_nID = nID;
	
	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, m_pwndParent, m_nID);
}

int CWinixCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_hiCodeCtrl = LoadLibrary("CX_CODECTRLEX.DLL");
	if (!m_hiCodeCtrl)
		return -1;

	CWnd* (APIENTRY *axCreateDLL)(CWnd*, void *);
	axCreateDLL = (CWnd* (APIENTRY*)(CWnd*,void*))GetProcAddress(m_hiCodeCtrl, "axCreate");
	if (!axCreateDLL)
	{
		FreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
		return -1;
	}

	struct _param param;
	GetClientRect(&param.rect);
	param.rect.right = param.rect.left + 0;//CODECTRL_WIDTH;
	param.rect.bottom = param.rect.top + 18;
	
	param.name  = "";
	param.fonts = "굴림체";
	param.point = 9;
	param.style = 0;
	param.tRGB  = 63;
	param.pRGB  = RGB(255,255,255);//GetIndexColor(7);
	param.options = "/u 0 /k 검색.BMP /l AXCOMBO.BMP /p 3 /s 1 /o True /q False";
	param.key   = m_nID;//특정 ID 
	
	m_pwndCodeCtrl = axCreateDLL(this, (LPVOID)&param);
	if (!m_pwndCodeCtrl)
	{
		FreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
		return -1;
	}

	return 0;
}

bool CWinixCodeCtrl::SetUnit(int nUnit)
{ 
	if ( !m_hiCodeCtrl || !m_pwndCodeCtrl)
		return false;

	SetCtrlProperty("UnitData", _variant_t((short)nUnit));

	return TRUE;
//	bool (APIENTRY *axSetUnit)(CWnd* , CString);
//	axSetUnit = (bool (APIENTRY*)(CWnd*, CString))GetProcAddress(m_hiCodeCtrl, "axSetUnit");
//	if (!axSetUnit)
//	{
//		return false;
//	}

//	return axSetUnit(m_pwndCodeCtrl, strUnit);
}

BOOL CWinixCodeCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if ((HWND)lParam == m_pwndCodeCtrl->m_hWnd)
		return m_pwndParent->SendMessage(WM_COMMAND, MAKEWPARAM(m_nID, HIWORD(wParam)), (LPARAM)m_hWnd);

	return CWnd::OnCommand(wParam, lParam);

}

CString CWinixCodeCtrl::GetCode()
{
	if (!m_pwndCodeCtrl)
		return _T("");
//	char* (APIENTRY *axGetCode)(CWnd*);
//	axGetCode = (char* (APIENTRY*)(CWnd*))GetProcAddress(m_hiCodeCtrl, "axGetCode");
//	if (!axGetCode)
//	{
//		return _T("");
//	}
//
//	CString strCode = CString(axGetCode(m_pwndCodeCtrl));
	return 	GetCtrlProperty("Data");
}

CString CWinixCodeCtrl::GetName()
{
	if (!m_pwndCodeCtrl)
		return _T("");

	if (!m_pwndCodeCtrl) return "";
	if (!m_pwndCodeCtrl->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pwndCodeCtrl->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t rval;
	CComDispatchDriver driver(pDisp);
	
	HRESULT hr = driver.Invoke0(L"GetName", &rval);
	
	return (LPCSTR)(_bstr_t)rval;

// 	char* (APIENTRY *axGetName)(CWnd*);
// 	axGetName = (char* (APIENTRY*)(CWnd*))GetProcAddress(m_hiCodeCtrl, "axGetName");
// 	if (!axGetName)
// 	{
// 		return _T("");
// 	}
// 
// 	CString strName = CString(axGetName(m_pwndCodeCtrl));
//	return strName;
}

bool CWinixCodeCtrl::SetCode(CString strCode, bool bOnlySet, bool bSetHistory)
{
	if (!m_pwndCodeCtrl)
		return false;
	
	bool (APIENTRY *axSetCode)(CWnd*, char*, bool, bool);
	axSetCode = (bool (APIENTRY*)(CWnd*, char*, bool, bool))GetProcAddress(m_hiCodeCtrl, "axSetCode");
	if (!axSetCode)
	{
		return false;
	}
	
	//return axSetCode(m_pwndCodeCtrl, (char*)(LPCTSTR)strCode, bOnlySet, bSetHistory);
	bool bret = axSetCode(m_pwndCodeCtrl, (char*)(LPCTSTR)strCode, bOnlySet, bSetHistory);
	return bret;
}


bool CWinixCodeCtrl::SetColor(COLORREF rgbFG, COLORREF rgbBG)
{
	if (!m_pwndCodeCtrl)
		return false;

	bool (APIENTRY *axSetColor)(CWnd*, COLORREF, COLORREF);
	axSetColor = (bool (APIENTRY*)(CWnd*, COLORREF, COLORREF))GetProcAddress(m_hiCodeCtrl, "axSetColor");
	if (!axSetColor)
	{
		return false;
	}

	return axSetColor(m_pwndCodeCtrl, rgbFG, rgbBG);
}

void CWinixCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pwndCodeCtrl && IsWindow(m_pwndCodeCtrl->m_hWnd))
	{
	//	m_pwndCodeCtrl->MoveWindow(0, 0, cx, cy);
		CRect rc;
		GetClientRect(&rc);
		m_pwndCodeCtrl->MoveWindow(&rc, TRUE);

	}
}

bool CWinixCodeCtrl::SetFont(CFont* pfont)
{
	if (!m_pwndCodeCtrl)
		return false;

	bool (APIENTRY *axSetFont)(CWnd*, CFont*);
	axSetFont = (bool (APIENTRY*)(CWnd*, CFont*))GetProcAddress(m_hiCodeCtrl, "axSetFont");
	if (!axSetFont)
	{
		return false;
	}

	return axSetFont(m_pwndCodeCtrl, pfont);
}

// 2010.01.05 scbang : edit에 focus 주기
bool CWinixCodeCtrl::SetFocus()
{
	if (!m_pwndCodeCtrl)
		return false;
	
	m_pwndCodeCtrl->SetFocus();
	
	return true;
}

///////////////////////////////////////////////////////////////

void CWinixCodeCtrl::SetCtrlProperty( _bstr_t prop_name, _variant_t &var )
{
	if (!m_pwndCodeCtrl) return;
	if (!m_pwndCodeCtrl->GetSafeHwnd()) return;
	
	IDispatch *pDisp = m_pwndCodeCtrl->GetIDispatch(FALSE);
	if (!pDisp) return;
	
	CComDispatchDriver driver(pDisp);
	driver.PutPropertyByName(prop_name, &var);
}

CString CWinixCodeCtrl::GetCtrlProperty( LPCSTR prop_name )
{
	USES_CONVERSION;
	if (!m_pwndCodeCtrl) return "";
	if (!m_pwndCodeCtrl->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pwndCodeCtrl->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(_bstr_t(prop_name), &var);
	
	return (LPCSTR)(_bstr_t)var;
}