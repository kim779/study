#include "stdafx.h"
#include "AxisCode.h"
#include "libdll.h"

#include "USCodeDlg.h"
#include "jongmukdlg.h"
#include "futuresdlg.h"
#include "stocknewdlg.h"
#include "bonddlg.h"
#include "UpCodeDlg.h"
#include "ELWDlg.h"
#include "EtfDlg.h"
#include "CustomFuturesDlg.h"
#include "RetailBondDlg.h"
#include "CommodityDlg.h"
#include "SFCodeDlg.h"
#include "CDlgSinJong.h"  //20240422
#include "../../h/axisfire.h"

using namespace AxStd;


__declspec(dllexport) void WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString szRoot((char*)root);

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	pApp->m_root = szRoot;
	pApp->LoadCode();
	pApp->LoadETFCode();
	pApp->loadCJCode();
	pApp->LoadFCode();
}

__declspec(dllexport) bool WINAPI axGetCode(int kind, char *user, char* code, int type, CPoint pt)
{		
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

 	CString szCode , szName , szTemp;
	CString	szUser(user); 

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	pApp->m_folder = user;
#ifdef _DEBUG       
//	kind = comspCODE;
#endif	
	switch (kind) 
	{
	case sinjongCODE:
	{
		CDlgSinJong dlg(kind, type, pt);
	
		if (dlg.DoModal() == IDOK)
		{
			szCode = dlg.GetCode();

			if (szCode.IsEmpty())
				return FALSE;

			szName = dlg.GetName();
			szTemp = szCode + "\t" + szName;
			strcpy(code, szTemp);
			return TRUE;
		}
		return FALSE;
	}	
	break;
	case customCODE:
		{
			szCode.Format("%s", code);
			CCustomFuturesDlg dlg(kind, pt, type, szCode);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;

			}
			return FALSE;	
		}
	case comspCODE:
		{		
			szCode.Format("%s", code);			
			CCommodityDlg dlg( nullptr, pt );
			dlg.SetSpreadMode( TRUE );
			if( dlg.DoModal() == IDOK )
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();

				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case goodFOCODE:
		{
			szCode.Format("%s", code);			
			CCommodityDlg dlg( nullptr, pt );
			if( dlg.DoModal() == IDOK )
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();

				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case kkCODE:
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case ETFCODE:
		{
			CEtfDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
				
				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case reitsCODE:
		{
			CJongmukDlg dlg(kind, type, pt);

			if (type != reitsType && type != shipType && type != infraType && type != reitsallType)
				return FALSE;

			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}

			return FALSE;
		}
		break;

	case isCODEtype: // ECN, ETF
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.IsCodeType(code, type))
				return TRUE;
		}
		break;

	case spreadCODE:
		break;

	case all2CODE:
		{
			CFuturesDlg dlg(kind, pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;

			}
			return FALSE;
		}
		break;

	case usCODE:
		{
			CUSCodeDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				CString rtnData = dlg.GetRtnData();
				if (!rtnData.IsEmpty())
				{
					strcpy(code, dlg.GetRtnData());
					return TRUE;
				}
			}
			return FALSE;
		}
		break;

	case faoptionCODE:
		{	
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;

			}
			return FALSE;
		}
		break;

	case ecnCODE:
		{	
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	
	case 88:	// 세자리 코드 업종 코드
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case allCODE:
	case kkelwCODE:
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	
	case kospiCODE:	//Kospi && Kosdaq...
		{       
			CJongmukDlg dlg(kind, type, pt);
			
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	
	case thirdCODE:
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
			
				if (szCode.IsEmpty())
					return FALSE;
					
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}

		break;
	
	case indexCODE:	//업 종... type에 따라 값이 변함..
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
			
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case 222:	
	case futureCODE: // Futures
		{
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;				

				if (szCode.IsEmpty())
					return FALSE;				

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case optionCODE: // option
		{
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case sfCODE: // option
	case sfCODESMALL:
		{
			CSFCodeDlg dlg(kind,pt);
			if (kind == sfCODESMALL) dlg.m_iTYPE = 1;
			if(dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case foptionCODE: // Futures && option	
		{
			CFuturesDlg dlg(kind, pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;	

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case koptionCODE: // 주식 옵션
		{
			CFuturesDlg dlg(kind, pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;
				
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case getCODEfuture: // No Dialog
		{
			CFuturesDlg dlg(kind);
			dlg.loadFjCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case getCODEcall: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadOjCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEput: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadOjCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case sinjuCODE:	// 신주 인수권
		{
			CStockNewDlg dlg(pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;
		
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case kospi2CODE: //kospi 200
		{
			CJongmukDlg dlg(kind, type, pt);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;
			
				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case bondCODE:	//채 권 
		{
			CBondDlg dlg(pt, type);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;
	case elwCODE:
		{
			CElwDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return TRUE;
			}
		}
		break;
	case smbondCODE:	//소매채권 
		{
			CRetailBondDlg dlg(pt);
			if (dlg.DoModal() == IDOK)
			{
				szName = dlg.GetName();
				szCode = dlg.GetCode();
				szTemp = szCode + "\t" + szName;
				
				if (szCode.IsEmpty())
					return FALSE;
				
				strcpy(code, szTemp);
				return TRUE;
			}
			return FALSE;
		}
		break;

		//2015.07.10 KSJ mini 추가
	case getCODEminifuture: // No Dialog
		{
			CFuturesDlg dlg(kind);
			dlg.loadMfCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEminicall: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadMoCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEminiput: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			dlg.loadMoCode();
			if (dlg.FindCode(kind, szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	//2016.03.11 KSJ 주식선물 코드 가져오는 부분 추가
	case getCODEsfuture: // No Dialog
		{
// 			CFuturesDlg dlg(kind);
// 			dlg.loadSfCode();
// 			if (dlg.FindCode(kind, szCode))
// 			{
// 				szCode = dlg.GetCode();
// 				if (szCode.IsEmpty())
// 					return FALSE;
// 				strcpy(code, szCode);
// 				return TRUE;
// 			}
			CSFCodeDlg dlg(kind);
			dlg.LoadMaster();
			if (dlg.FindCode(kind,szCode))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
	}

	return FALSE;
}

// 챠트용 미니지수
__declspec(dllexport) void WINAPI axGetMiniJisu(char* link)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CUpCodeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		strcpy(link, dlg.GetRtnData());
	}
}

#include <sys/timeb.h>


__declspec(dllexport) bool WINAPI axGetName(int kind, char* code, char* name, int* type)
{
	// No Dialog...
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString szName;
	CString szCode(code); 
	szCode.Trim();
	const CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	
	if (kind == typeforTAX)  //거래세 계산을 위해 추가
	{
		CJongmukDlg dlg(116, 0);
		if (dlg.FindCode(typeforTAX, code, -1))
		{	// 주식
			szName = dlg.GetName();
			CString stype;
			stype.Format("%d", dlg.GetJongmuk());
			strcpy(name, stype);
			
			*type = dlg.GetJongmuk();
			dlg.DeleteAllData();				
			return TRUE;
		}
		else if (szCode.GetLength() == 6 && (szCode[0] == '5' || szCode[0] == '6' || szCode[0] == '7'))
		{
			CString sElwCode;
			sElwCode.Format("J%s", code);
			if (dlg.FindCode(kind, sElwCode, -1))
			{
				CString stype;
				stype.Format("%d", dlg.GetJongmuk());
				strcpy(name, stype);
				
				*type = dlg.GetJongmuk();
				dlg.DeleteAllData();
				return TRUE;
			}
		} 
		*type = -1;
		return FALSE; 
	}
	

	if (kind == whichNAME)
	{
		if (szCode.GetLength() <= 0)
			return FALSE;


		if( (szCode[0] == '1' || szCode[0] == '4' || szCode[0] == 'A' || szCode[0] == 'D') && (szCode[1] == '6' ||  szCode[1] == '7' || szCode[1] == '8') )  //파생상품 코드개편 상품선물
		{
			kind = commodityNAME;
		}
		else if( (szCode[0] == '2' || szCode[0] == '3' || szCode[0] == 'B' || szCode[0] == 'C') && szCode[1] == '7' ) // 달러옵션   //파생상품 코드개편
		{
			kind = comoptionNAME;
		}
		else if (szCode.GetLength() == 6)
		{
			*type = 1;
			kind = codeNAME;
		}
		else if (szCode[0] == '#')	//2016.03.18 KSJ 주식선물 구분 추가
		{
			szCode.Delete(0);
			kind = getMarketsfuture;
		}
		else if (szCode.GetLength() == 9 || szCode.GetLength() == 12)
		{
			kind = bondNAME;
		}
		/*
		else if (szCode.GetLength() == 12)
		{
			kind = smbondNAME;
		}
		*/
		else if (szCode.GetLength() <= 3)
		{
			kind = indexNAME;
		}
		//2024.10.31  주식선물 관련 추가
		else if((szCode.Mid(1,2) >= "10" && szCode.Mid(1,2) <= "59") || (szCode.Mid(1,2) >= "B0" && szCode.Mid(1,2) <= "ZZ") || 
			(szCode.Mid(1, 2) >= "0A" && szCode.Mid(1, 2) <= "0Z") || (szCode.Mid(1, 2) >= "1A" && szCode.Mid(1, 2) <= "1Z") || (szCode.Mid(1, 2) >= "2A" && szCode.Mid(1, 2) <= "2Z")
			|| (szCode.Mid(1, 2) >= "3A" && szCode.Mid(1, 2) <= "3Z") || (szCode.Mid(1, 2) >= "4A" && szCode.Mid(1, 2) <= "4Z") || (szCode.Mid(1, 2) >= "5A" && szCode.Mid(1, 2) <= "5Z")
			|| (szCode.Mid(1, 2) >= "6A" && szCode.Mid(1, 2) <= "6Z") || (szCode.Mid(1, 2) >= "7A" && szCode.Mid(1, 2) <= "7Z") || (szCode.Mid(1, 2) >= "8A" && szCode.Mid(1, 2) <= "8Z")
			|| (szCode.Mid(1, 2) >= "9A" && szCode.Mid(1, 2) <= "9Z"))
		{
			*type = 6;
			kind = sfName;
		}
		else if (szCode[0] == '1' || szCode[0] == '4' || szCode[0] == 'A' || szCode[0] == 'D')  //파생상품 코드개편
		{
			if (szCode[1] == '0')
			{
				*type = 2;
				kind = futureNAME;
			}
			else
			{
				*type = 6;
				kind = sfutureNAME;
			}
		}		
		else if (szCode[0] == '2' || szCode[0] == '3' || szCode[0] == 'B' || szCode[0] == 'C')   //파생상품 코드개편
		{
			if (szCode[1] == '0' || (szCode[1] == 'A' && szCode[2] == 'F'))
			{
				*type = 3;
				kind = optionNAME;
			}
			else
			{
				*type = 5;	
				kind = koptionNAME;
			}
		}
		else if (!szCode.Compare("99999999"))
		{
			strcpy(name, "연결선물");
			return TRUE;
		}		
	}

	switch (kind)
	{
	case ETFNAME:
		{
			//CEtfDlg dlg;

		}
	case whichORDER:	// 매매유형
		{
			CFuturesDlg dlg(kind);
			if (dlg.GetCodeOrderable(code, type))
				return TRUE;
		}
		break;

	case whichTYPE: // 선택..
		{
		CString slog;
		slog.Format("[AXISCODE] whichTYPE szCode=[%s]", szCode);
		OutputDebugString(slog);
			auto& map = pApp->_mapCODEx;
			if (const auto ft = map.find(szCode); ft != map.end())
			{
				const struct hjcodex* hjc = ft->second;
				szName = CString(hjc->hnam, HNameLen).Trim();
				
				switch (hjc->kosd)
				{
				case jmKOSPI:
					*type = kospiType;		break;
				case jmKOSDAQ:
					*type = kosdaqType;		break;
				case jm3RD:
					*type = thirdType;		break;
				case jmMUFND:
					//	case jmMUFND2:
					*type = mufundType;		break;
				case jmREITS:
					*type = reitsType;		break;
				}

				if (hjc->ssgb == jmETF)
					*type = etfType;
				if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS)
					*type = sinjuType;
				if (hjc->ssgb == jmHYFND)
					*type = hyfundType;
				if (hjc->ssgb == jmELW)
					*type = elwType;
				if (hjc->ssgb == jmFOREIGN)
					*type = foreignType;
				if (hjc->ssgb == jmETN)//KSJ 2014.10.28 ETN 추가
					*type = etnType;
				if (hjc->ssgb == jmTJCONJS)//2024/04/22 신종코드타입
					*type = singjongType;
				if (hjc->ssgb == jmSINSUJS)//2024/04/22 신종코드타입
					*type = singjongType;
				if (hjc->ssgb == jmGOODSTOCK)
					*type = singjongGoodType;

				//20191114 맵화면에서 getcodetype를 사용하는 곳이 너무 많아서 kind를 typeforTAX로 주는 경우에만 타입을 세분화 해줌
				//SCREEN.GetName(199, 종목코드, 0)
				if (hjc->ssgb == jmKONEX && kind == typeforTAX)
					*type = KONEXType;
				strcpy(name, szName);

				slog.Format("[AXISCODE] whichTYPE szName=[%s]", szName);
				OutputDebugString(slog);

				return TRUE;			
			}
			else
			{
				szCode = szCode.Mid(1);
				if (const auto ft = map.find(szCode); ft != map.end())
				{
					const struct hjcodex* hjc = ft->second;
					szName = CString(hjc->hnam, HNameLen).Trim();

					switch (hjc->kosd)
					{
					case jmKOSPI:
						*type = kospiType;		break;
					case jmKOSDAQ:
						*type = kosdaqType;		break;
					case jm3RD:
						*type = thirdType;		break;
					case jmMUFND:
						//	case jmMUFND2:
						*type = mufundType;		break;
					case jmREITS:
						*type = reitsType;		break;
					}

					if (hjc->ssgb == jmETF)
						*type = etfType;
					if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS)
						*type = sinjuType;
					if (hjc->ssgb == jmHYFND)
						*type = hyfundType;
					if (hjc->ssgb == jmELW)
						*type = elwType;
					if (hjc->ssgb == jmFOREIGN)
						*type = foreignType;
					if (hjc->ssgb == jmETN)//KSJ 2014.10.28 ETN 추가
						*type = etnType;
					if (hjc->ssgb == jmTJCONJS)//2024/04/22 신종코드타입
						*type = singjongType;
					if (hjc->ssgb == jmSINSUJS)//2024/04/22 신종코드타입
						*type = singjongType;
					if (hjc->ssgb == jmGOODSTOCK)
						*type = singjongGoodType;

					//20191114 맵화면에서 getcodetype를 사용하는 곳이 너무 많아서 kind를 typeforTAX로 주는 경우에만 타입을 세분화 해줌
					//SCREEN.GetName(199, 종목코드, 0)
					if (hjc->ssgb == jmKONEX && kind == typeforTAX)
						*type = KONEXType;
					strcpy(name, szName);

					slog.Format("[AXISCODE] whichTYPE szName=[%s]", szName);
					OutputDebugString(slog);

					return TRUE;
				}


				if (szCode.GetLength() > 5)
				{
					const TCHAR	ch1 = szCode.GetAt(0);
					const TCHAR	ch2 = szCode.GetAt(1);
				
					if ((ch1 == '1' || ch1 == '4' || ch1 == 'A' || ch1 == 'D') && ch2 != '0')  //파생상품 코드개편
					{
						CCustomFuturesDlg cfdlg(0);
						cfdlg.loadSfCode();
						
						if (cfdlg.FindCode(kind, code))
						{
							szName = cfdlg.GetName();
							strcpy(name, szName);

							*type = cfdlg.GetJongmuk();
							return TRUE;
						}
					}
					else
					{
						CFuturesDlg fdlg(kind);
						if (fdlg.FindCode(kind, code))
						{
							// 선물
							szName = fdlg.GetName();
							strcpy(name, szName);

							CString slog;
							slog.Format("[AXISCODE][whichTYPE][%s]<%d> type=[%d] code=[%s] ", __FUNCTION__, __LINE__, fdlg.m_jongmuk, szCode);
							OutputDebugString(slog);


							*type = fdlg.GetJongmuk();
							return TRUE;
						}
					}
				} 
				else if (szCode.GetLength() <= 3)
				{
					CJongmukDlg dlg(kind, 0);

					// 업종
					dlg.LoadUPCode();
					if (dlg.FindCode(indexNAME, code, *type))
					{
						szName = dlg.GetName();
						strcpy(name, szName);

						*type = indexType;
						dlg.DeleteAllData();
						return TRUE;
					} 
				} 

				*type = -1;
//AxStd::_Msg("GetCodeTypexxxxx  [%s][%s][%d]", code, name, type);
			}
		}
		break; 

	case codeNAME:	// 종목명 반환...
		{
			auto& map = pApp->_mapCODEx;
			if (const auto ft = map.find(szCode); ft != map.end())
			{
				const struct hjcodex* hjc = ft->second;
				szName = CString(hjc->hnam, HNameLen).Trim();
				
				switch (hjc->kosd)
				{
				case jmKOSPI:
					*type = kospiType;		break;
				case jmKOSDAQ:
					*type = kosdaqType;		break;
				case jm3RD:
					*type = thirdType;		break;
				case jmMUFND:
					//	case jmMUFND2:
					*type = mufundType;		break;
				case jmREITS:
					*type = reitsType;		break;
				}

				if (hjc->ssgb == jmETF)
					*type = etfType;
				if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS)
					*type = sinjuType;
				if (hjc->ssgb == jmHYFND)
					*type = hyfundType;
				if (hjc->ssgb == jmELW)
					*type = elwType;
				if (hjc->ssgb == jmFOREIGN)
					*type = foreignType;
				if (hjc->ssgb == jmETN)//KSJ 2014.10.28 ETN 추가
					*type = etnType;
				if (hjc->ssgb == jmTJCONJS)//2024/04/22 신종코드타입
					*type = singjongType;				
				if (hjc->ssgb == jmGOODSTOCK)
					*type = singjongGoodType;

				//20191114 맵화면에서 getcodetype를 사용하는 곳이 너무 많아서 kind를 typeforTAX로 주는 경우에만 타입을 세분화 해줌
				//SCREEN.GetName(199, 종목코드, 0)
				if (hjc->ssgb == jmKONEX && kind == typeforTAX)
					*type = KONEXType;

				strcpy(name, szName);
				return TRUE;			
			}
		}
		break;

	case futureNAME: // 선물명 반환...
		{
			CFuturesDlg dlg(kind);
			dlg.loadFjCode();

			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}
		} 
		break;
	
	case sfName:
		{
// 			CFuturesDlg dlg(kind);
// 			dlg.loadSfCode();
// 
// 			if(dlg.FindCode(kind, code))
// 			{
// 				szName = dlg.GetName();
// 				strcpy(name,szName);
// 				return TRUE;
// 			}
			CSFCodeDlg dlg(kind);
			dlg.LoadMaster();
			
			if(dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}
		}
		break;

	case comspName:
		{
			CCommodityDlg dlg;
			dlg.LoadCCodeFromFile();

			if( dlg.FindOCode( code ) )
			{

				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);

				return TRUE;
			}
		}
		break;
	case comoptionNAME:
		{
			CCommodityDlg dlg;
			dlg.LoadCOCodeFromFile();

			if( dlg.FindOCode( code ) )
			{

				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);

			
				return TRUE;
			}
		}
		break;

	case commodityNAME:
		{
			CCommodityDlg dlg;
			dlg.LoadCCodeFromFile();

			if( dlg.FindCode(code) )
			{
				szName = dlg.GetName();
				szName = szName.Left(40);
				strcpy(name, szName);
	
				return TRUE;
			}
			else
			{	
			}
		}
		break;

	case optionNAME: // 옵션명 반환...
		{
			CFuturesDlg dlg(kind);
			dlg.loadOjCode();
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);
				return TRUE;
			}
		}
		break;

	case sfutureNAME: // 주식선물
		{
			CCustomFuturesDlg dlg(kind);
			dlg.loadSfCode();
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);
				return TRUE;
			}
		
		}
		break;

	case basicCODE:
		{
			CCustomFuturesDlg	dlg(kind);
			
			if (szCode.GetLength() == 8)				
			{
				if (dlg.FindCode(kind, szCode))
				{
					szName = dlg.GetBCode();
					strcpy(name, szName);
					return TRUE;
				}
			}			
		}

	case indexNAME:	// 업종명 반환...
		{
			CJongmukDlg dlg(kind, *type);
			dlg.LoadUPCode();
			if (dlg.FindCode(kind, code, *type))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				*type = 3;
				dlg.DeleteAllData();
				return TRUE;
			}
			dlg.DeleteAllData();
		}
		break;
	
	case koptionNAME: //주식 옵션명 반환...
		{
			CFuturesDlg dlg(kind);
			dlg.loadPjCode();
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE; 
			}
		}
		break;

	case sinjuNAME: // 신주인수권...
		{
			CStockNewDlg dlg;
			dlg.loadJCode();
			CString szName = dlg.GetName(code);
			if (!szName.IsEmpty())
			{
				strcpy(name, szName);
				return TRUE;
			}
		}
		break;

	case bondNAME:	// 채권..
		{
		   CBondDlg bondDlg(kind);
//		   bondDlg.LoadCJCode();

		   BOOL bFind = bondDlg.FindCode(code);

		   if (bFind)
		   {
			szName = bondDlg.GetName();
			strcpy(name, szName);

			*type = bondDlg.GetCodeType();
		   }
		   else
		   {
			CRetailBondDlg rtBondDlg(kind);
			rtBondDlg.LoadSJCode();

			bFind = rtBondDlg.FindCode(code);

			if (bFind)
			{
			 szName = rtBondDlg.GetName();
			 strcpy(name, szName);
			}
			else
			{
			 CJongmukDlg jmDlg(kind, 0);

			 //bFind = jmDlg.FindCode(kind, code, -1);
			 bFind = jmDlg.FindCode(whichTYPE, code, -1);

			 if (bFind)
			 {
			  szName = jmDlg.GetName();
			  strcpy(name, szName);

			  /**type = jmDlg.GetJongmuk();*/

			  *type = 1;

			  /*CString str;
			  str.Format("[%d]", *type );
			   AfxMessageBox(str);*/
			 }
 
			 jmDlg.DeleteAllData();
			}
			rtBondDlg.DeleteAllData();
		   }

		   return bFind;
		  }
		  break; 	
	case smbondNAME: // 소매채권..
		{
			CRetailBondDlg dlg(kind);
			dlg.LoadSJCode();
			if (dlg.FindCode(code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				dlg.DeleteAllData();
				return TRUE;
			}
			dlg.DeleteAllData();
		}	
		break;

		//2016.03.18 KSJ 주식선물 마켓구분 추가
	case getMarketsfuture: // No Dialog
		{					
// 			CString strMgubn;
// 			CFuturesDlg dlg(kind);
// 			dlg.loadSfCode();
// 			if (dlg.FindCode(kind, szCode))
// 			{
// 				strMgubn = dlg.GetMgubn();
// 
// 				if (strMgubn.IsEmpty())
// 					return FALSE;
// 				strcpy(name, strMgubn);
// 
// 				return TRUE;
// 			}
			CString strMgubn;
			CSFCodeDlg dlg(kind);
			dlg.LoadMaster();
			if (dlg.FindCode(kind, szCode))
			{
				strMgubn = dlg.GetMgubn();
				
				if (strMgubn.IsEmpty())
					return FALSE;
				strcpy(name, strMgubn);
				
				return TRUE;
			}
			return FALSE;
		}
		break;

	case kofexNAME:   
	case thirdNAME: // 제3시장...
	case ecnNAME:	// ECN... 
	default:
		break;
	}
	
	return FALSE;
}


__declspec(dllexport) char* WINAPI getArray(int type = 0)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	switch (type)
	{
	case 1:
		return (char*)&pApp->_mapCODEx;
		break;
	case 2:
		return (char*)&pApp->_mapNAME;
		break;
	case 0:
	default:
		return (char*)&pApp->_mapACODE;
		break;
	}
	return nullptr;
}


__declspec(dllexport) const bool WINAPI IsNxt(const char* sCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	auto& map = pApp->_mapCODEx;
	const auto ft = map.find(CString(sCode));
	if (ft == map.end())
		return false;

	return (ft->second->nxtf == 'Y') ? true : false;
	
}

__declspec(dllexport) const char* WINAPI getName(const char* sCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	auto& map = pApp->_mapCODEx;
	const auto ft = map.find(CString(sCode));
	if (ft == map.end())
		return "";

	return CString(ft->second->hnam, HNameLen).Mid(1).Trim();
}

__declspec(dllexport) const char* WINAPI getCode(const char* sName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	auto& map = pApp->_mapNAME;
	const auto ft = map.find(CString(sName));
	if (ft == map.end())
		return "";

	return ft->second.GetString();
}
