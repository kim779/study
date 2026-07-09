#include "StdAfx.h"
#include "AxisCode.h"
#include "libdll.h"

#include "USCodeDlg.h"
#include "jongmukdlg.h"
#include "futuresdlg.h"
#include "stocknewdlg.h"
#include "bonddlg.h"
#include "UPCodeDlg.h"
#include "FundDlg.h"
#include "ElwDlg.h"
#include "K50Dlg.h"
#include "CmmdtDlg.h"
#include "agencyDlg.h"

#include "codeInfo.h"

extern "C";

#define	cTAB	'\t'

__declspec(dllexport) CWnd* WINAPI axCreateBondDlg(CWnd* pwnd, void* user)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString szUser((char*)user);
	pApp->m_Name = szUser;
	pApp->LoadCJCode();
	
	OutputDebugString("@@@@@@ axCreateBondDlg");
	return m_pdlg;
}

__declspec(dllexport) void WINAPI  ShowCodeDlg(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();

	CString szRoot((char*)root);
	pApp->m_RootPath = szRoot;

	if(m_pdlg != NULL)
		delete m_pdlg;

	m_pdlg = new CUnBond(CPoint(0,0));
	m_pdlg->Create(IDD_DIALOG_ALLBOND);
	m_pdlg->m_bINTERMODE = false;
	m_pdlg->Reposition();
	m_pdlg->CenterWindow();
	m_pdlg->ShowWindow(SW_SHOW);
}

// root 디텍토리 정보 
__declspec(dllexport) void WINAPI axCreateEx(void* root)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString szRoot((char*)root);

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	pApp->m_RootPath = szRoot;
	OutputDebugString("@@@@@@ axCreateEx LoadAllCode");
	pApp->LoadAllCode();
}

// 비교챠트용
__declspec(dllexport) void WINAPI axGetLink(char* user, char* code, char* link)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	pApp->m_Myfolder = user;

	int nLen = CString(code).GetLength();
	if (!nLen)
	{
		CJongmukDlg dlg(all2CODE, 0);

		if (dlg.DoModal() == IDOK)
		{
			CString szCode = dlg.GetCode();
			CString szName = dlg.GetName();
			CString szGroup = dlg.GetGroup(szCode);

			CString rValue = _T(""), G_STR = _T("");
			if (szGroup.GetLength())
			{
				if (szGroup.Left(2) == "$1")
				{
					G_STR = _T("(거)");
					szGroup = szGroup.Mid(2);
				}
				else if (szGroup.Left(2) == "$2")
				{
					G_STR = _T("(코)");
					szGroup = szGroup.Mid(2);
				}

			}

			rValue.Format("%s%c%s%s%c%s", szCode, cTAB, G_STR, szName, cTAB
				, szGroup);

			strcpy(link, rValue);
		}
	}
	else
	{
		CJongmukDlg dlg(codeNAME, 0);
		dlg.GetLinkCode(code, link);
	}
}

// 챠트용 미니지수
__declspec(dllexport) void WINAPI axGetMiniJisu(char* link)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CUPCodeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		strcpy(link, dlg.GetRtnData());
	}
}

// 종목 이름만 반환..
__declspec(dllexport) bool WINAPI axGetName(int kind,/* char *rootCString root*/ char *code/*CString code*/, char* name, int* type)
{
	// No Dialog...
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString tmpS;
	CString szName;
	CString szCode(code);

	if (kind == whichNAME)
	{
		if(szCode.GetLength() <= 6)
			kind = agname;
		else  //test
		//	kind = 130;
			kind = bondNAME; 
	}

	switch (kind)
	{
	case 131:   //종목코드로부터 종목명을 가져온다  비일반
	case 132:   //회사코드가 오면 회사명을 반환한다 비일반
	case 133:   //회사명이 오면 회사코드 반환한다 비일반
		{
			CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
			if (pApp->FindName(kind, code, name))  //strcpy(code, str);  //종목명 반환
			{
				tmpS.Format(_T("@@@@@axiscode [131] code = [%s]  name = [%s] \n"), code, name);
				OutputDebugString(tmpS);
				*type = pApp->m_nType;
				return TRUE;
			}
		}
		break;
	case bondNAME: // 채권..  심볼을 정해야 함..
		{			
			CUnBond dlg;
			CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();

			if (pApp->FindName(130, code, name))  //strcpy(code, name);  //code에 종목명넣어준다
			{
				OutputDebugString(tmpS);
				*type = pApp->m_nType;
				return TRUE;
			}
		//	return TRUE;
		}	
		break;
	case 130:  //종목명에서 종목코드를 반환한다
		{
			*type = 0;
			CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
			if (pApp->FindName(kind, code, name))  //strcpy(code, name);  //code에 종목명넣어준다
			{
				tmpS.Format(_T("@@@@@axiscode [130] code = [%s]  name = [%s] \n"), code, name);
				OutputDebugString(tmpS);
				*type = pApp->m_nType;
				return TRUE;
			}
			
		}
		break;
	case whichORDER:	// 매매유형
		{
			CFuturesDlg dlg(kind);
			bool result = dlg.GetCodeOrderable(code, type);

			if (result)
				return TRUE;
		}
		break;
	case whichTYPE: // 선택..
		{
			/*
			CJongmukDlg dlg(kind, 0);
				
				dlg.loadJCode();
				dlg.loadELWCode();

				if (dlg.FindCode(kind, code, -1))
				{
					szName = dlg.GetName();
					strcpy(name, szName);

					*type = dlg.GetJongmuk();
					dlg.DeleteAllData();
					
					CString str;
					str.Format("%d", *type);

					return TRUE;
				}

				dlg.DeleteAllData();
			*/
			///*
			*type = 0;
			CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
			if (pApp->FindCode(kind, code, name))
			{
					*type = pApp->m_nType;
				return TRUE;
			}
			//*/
		}
		break;
	case codeNAME:	// 종목명 반환...
		{
/*			CJongmukDlg dlg(kind, 0);

				dlg.loadJCode();
				dlg.loadELWCode();

				if (dlg.FindCode(kind, code, -1))
				{
					szName = dlg.GetName();
					strcpy(name, szName);

					*type = dlg.GetJongmuk();
					dlg.DeleteAllData();
					return TRUE;
				}
				dlg.DeleteAllData();
*/
			*type = 0;		
			CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
			if (pApp->FindCode(kind, code, name))
			{
				*type = pApp->m_nType;
				return TRUE;
			}		
		}    
		break;
	case futureNAME: // 선물명 반환...
		{
			/*
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}
			*/
			return TRUE;
		}
		break;
	case optionNAME: // 옵션명 반환...
		{
			/*
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				szName = szName.Left(20);
				strcpy(name, szName);
				return TRUE;
			}
			*/
			return TRUE;
		}
		break;
	case indexNAME:	// 업종명 반환...
		{
			/*
			CJongmukDlg dlg(kind, *type);
			if (dlg.FindCode(kind, code, *type))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				*type = 3;
				return TRUE;
			}*/
			return TRUE;
		}
		break;
	case kofexNAME:   
		break;
	case koptionNAME: //주식 옵션명 반환...
		{
			/*
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE; 
			}
			*/
			return TRUE;
		}
		break;
	case sinjuNAME: // 신주인수권...
		{
			/*
			CStockNewDlg dlg;
			dlg.loadJCode();
			CString szName = dlg.GetName(code);
			if (!szName.IsEmpty())
			{
				strcpy(name, szName);
				return TRUE;
			}
			*/
			return TRUE;
		}
		break;
	case thirdNAME: // 제3시장...
		break;
	case ecnNAME: // ECN... 
		break;
	case agname:
		{
			agencyDlg dlg;
			if (dlg.FindCode(code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}	
			return TRUE;
		}
		break;
	case fdNAME:	// 펀드네임
		{
			/*
			CFundDlg dlg;
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE;
			}
			*/
			return TRUE;
		}
		break;
	case elwJS:	// elw 기초자산 code
		{
			/*
			CJongmukDlg dlg(kind, 0);
			if (dlg.FindELWJSCode(code))
			{
				szCode = dlg.GetCode();
				if (szCode.IsEmpty())
					return FALSE;
				strcpy(name, szCode);
			
				return TRUE;
			}
			*/
			return TRUE;
		}
		break;
	case kfutureNAME:
		{
			/*
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, code))
			{
				szName = dlg.GetName();
				strcpy(name, szName);
				return TRUE; 
			}
			*/
			return TRUE;
		}
	}

	return FALSE;
}

// 종목 다이얼 로그가 뜸...
__declspec(dllexport) bool WINAPI axGetCode(int kind, /*char *root/*CString root*/ char *user/*CString user*/, char* code, int type, CPoint pt)
{		
	// Dialog ...
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString szCode("") , szName("") , szTemp("");
	CString /*szRoot(root), */szUser(user);

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();

//	pApp->m_RootPath = root; 
	pApp->m_Myfolder = user;
	if (kind != isCODEtype)
	{
		//ZeroMemory(code, 30);
		code[0] = 0x00;
	}
	
	switch (kind)
	{
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
	case isCODEtype: // Sector지수종목(type:1), ETF종목(type:2)
		{
			/*
			CJongmukDlg dlg(kind, type, pt);

			bool result = dlg.IsCodeType(code, type);

			dlg.DeleteAllData();
			if (result)
				return TRUE;
			*/
			CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
			bool result = pApp->IsCodeType(code, type);
			return result ? TRUE : FALSE;
		}
		break;
	case spreadCODE:
		{
			CFuturesDlg dlg(kind, type, pt);
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
	case fdCODE:
		{
			CFundDlg	dlg(pt);
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
/*
	case all2CODE: // 비교챠트용
		{
			CJongmukDlg dlg(kind, type, pt);
			
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();

				CString rValue;
				rValue.Format("%s%c%s%c%s", szCode, cTAB, szName, cTAB
					, dlg.GetGroup(szCode));

				if (rValue.IsEmpty())
					return FALSE;

				strcpy(code, rValue);
				return TRUE;
			}
			return FALSE;
		}
		break;
*/
	case faoptionCODE:	// 옵션 선물 현물 옵션 모두.... symbol 정해야함.
			// 바뀌면 강재만씨에게 알림..
		{	
			CFuturesDlg dlg(kind, type, pt);
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

	case ecnCODE:	// ECN 코드만...
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
	
	case 88:	// 세자리 코드 업종 코드... symbol 미정...
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
					
				strcpy((char*)code, szTemp);
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

	case futureCODE: // Futures...
		{
			CFuturesDlg dlg(kind, type, pt);
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

	case optionCODE: // option...
		{
			CFuturesDlg dlg(kind, type, pt);
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

	case foptionCODE: // Futures && option...	
		{
			CFuturesDlg dlg(kind, type, pt);
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

	case koptionCODE: // 주식 옵션...
		{
			CFuturesDlg dlg(kind, type, pt);
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

	case getCODEfuture: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, szCode))
			{
			//	szName = dlg.GetName();
				szCode = dlg.GetCode();
				
				if (szCode.IsEmpty())
					return FALSE;
			//	szTemp = szCode + "\t" + szName;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case getCODEcall: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, szCode))
			{
			//	szName = dlg.GetName();
				szCode = dlg.GetCode();
			
				if (szCode.IsEmpty())
					return FALSE;
			//	szTemp = szCode + "\t" + szName;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;
		
	case getCODEput: // No Dialog...
		{
			CFuturesDlg dlg(kind);
			if (dlg.FindCode(kind, szCode))
			{
			//	szName = dlg.GetName();
				szCode = dlg.GetCode();
			
				if (szCode.IsEmpty())
					return FALSE;
			//	szTemp = szCode + "\t" + szName;
				strcpy(code, szCode);
				return TRUE;
			}
			return FALSE;
		}
		break;

	case sinjuCODE:	// 신주 인수권
		{
			switch (type)
			{
			case 2:		// ELW선택
				{
					CElwDlg dlg(pt);

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
				}
				break;
			default:
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
				}
				break;
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
			CBondDlg dlg(pt);
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
	case UNbondCODE:
		{		
			CUnBond dlg(pt);
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
			
			/*
			if(!m_pdlg->GetSafeHwnd())
			{
				m_pdlg = new CUnBond(pt);
				m_pdlg->Create(IDD_DIALOG_ALLBOND);
			}
			else
			{
				m_pdlg->SetDlgPos(pt.x, pt.y);
				m_pdlg->m_bINTERMODE = false;
				m_pdlg->Reposition();
			}

			m_pdlg->ShowWindow(SW_SHOW);
			*/
			//m_pdlg->Setdatapointer(
			
		}
		break;
	case AgencyCODE:
		{
			agencyDlg dlg(pt);
			if(type == 1)
				dlg.itype = 1;
			if (dlg.DoModal() == IDOK)
			{
				szCode = dlg.GetCode();
				szName = dlg.GetName();
				szTemp = szCode + "\t" + szName;

				if (szCode.IsEmpty())
					return FALSE;

				strcpy(code, szTemp);
				return true;
			}
			return FALSE;
		}
		break;
	case kfutureCODE:
		{
			CFuturesDlg dlg(kind, type, pt);
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
	case etfCODE:
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
	case kospi50CODE:
		{
			CK50Dlg dlg(pt);
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
	case elwCODE:
		{
			CElwDlg dlg(pt);
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
		}
	case commdtCODE:
		{
			CCmmdtDlg dlg(kind, type, pt);
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
	}
	return FALSE;
}

__declspec(dllexport) bool WINAPI axCodeList(int kind, long list)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CCodeInfo codeInfo;
	CString listS = _T("");
	switch (kind)
	{
	case futureLIST:
		if (!codeInfo.GetFutureList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	case callLIST:
		if (!codeInfo.GetCallList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	case putLIST:
		if (!codeInfo.GetPutList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	case pcodeLIST:				// 주식옵션 list
		if (!codeInfo.GetPCodeList(listS))
			return false;
		strcpy((char*)list, listS);	// 거래대상종목구분 \t 거래대상종목명\t
		break;
	case kfutureLIST:			// 스타지수 list
		if (!codeInfo.GetKFutureList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	case xfutureLIST:			// KRX100 list
		if (!codeInfo.GetXFutureList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	case sfutureLIST:			// 주식선물 LIST	2006.10
		if (!codeInfo.GetSFutureList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	case commdtLIST:
		if (!codeInfo.GetCommdtList(listS))
			return false;
		strcpy((char*)list, listS);
		break;
	}
	return true;
}