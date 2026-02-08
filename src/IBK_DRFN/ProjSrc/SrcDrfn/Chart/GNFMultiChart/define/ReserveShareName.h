#ifndef __DOORIIC__NEOPLATFORM__KDKJKE9383__DKKDEW_3323DKDKLS_HEADER__
#define __DOORIIC__NEOPLATFORM__KDKJKE9383__DKKDEW_3323DKDKLS_HEADER__

// #include "../include/ReserveShareName.h"

// * 계좌 비밀번호 저장 기능을 사용하지 않으면 pass(8byte) 항목을 공백으로 처리한다. 
#define		NPRSN_ACCTALL			"!NP_AcctAll"		// Acct(11) + Name(20) + Pass(8) : 전체 계좌 리스트 
// #define		NPRSN_ACCTBASE			"NP_Acct"
#define		NPRSN_ACCT01			"!NP_Acct01"		// Acct(11) + Name(20) + Pass(8) : 위탁(현물) 계좌 리스트
#define		NPRSN_ACCT11			"!NP_Acct11"		// Acct(11) + Name(20) + Pass(8) : 위탁(현물) 계좌 리스트
#define		NPRSN_ACCT20			"!NP_Acct20"		// Acct(11) + Name(20) + Pass(8) : 저축 계좌 리스트 
#define		NPRSN_ACCT33			"!NP_Acct33"		// Acct(11) + Name(20) + Pass(8) : 선물 계좌 리스트 
#define		NPRSN_ACCT60			"!NP_Acct60"		// Acct(11) + Name(20) + Pass(8) : 저축 계좌 리스트 
#define		NPRSN_ACCT91			"!NP_Acct91"		// Acct(11) + Name(20) + Pass(8) : 저축 계좌 리스트 
#define		NPRSN_ACCT99			"!NP_Acct99"		// Acct(11) + Name(20) + Pass(8) : 저축 계좌 리스트 
#define		NPRSN_ACCT62			"!NP_Acct62"		// Acct(11) + Name(20) + Pass(8) : 저축 계좌 리스트 
#define		NPRSN_TACCT01			"!NP_TAcct01"		// Acct(11) + Name(20) + Pass(8) : 위탁위임 계좌 리스트 
#define		NPRSN_TACCT11			"!NP_TAcct11"		// Acct(11) + Name(20) + Pass(8) : 위탁위임 계좌 리스트 
#define		NPRSN_TACCT33			"!NP_TAcct33"		// Acct(11) + Name(20) + Pass(8) : 위탁선물 계좌 리스트 
#define		NPRSN_TACCT60			"!NP_TAcct60"		// Acct(11) + Name(20) + Pass(8) : 위탁저축 계좌 리스트 
#define		NPRSN_TACCT91			"!NP_TAcct91"		// Acct(11) + Name(20) + Pass(8) : 위탁저축 계좌 리스트 
#define		NPRSN_TACCT99			"!NP_TAcct99"		// Acct(11) + Name(20) + Pass(8) : 위탁저축 계좌 리스트 
#define		NPRSN_TACCT62			"!NP_TAcct62"		// Acct(11) + Name(20) + Pass(8) : 위탁저축 계좌 리스트 
#define		NPRSN_ACCTEDIT			"!NP_AcctEdit"		// 1 or 0 (계좌번호 직접입력 가능여부)
#define		NPRSN_USERID			"!NP_UserID"		// 사용자 ID
#define		NPRSN_SERVERIP			"!NP_ServerIp"		// 서버 IP
#define		NPRSN_SERVERNAME		"!NP_ServerName"	// 서버명 
// #define		NPRSN_USEACCTBASE		"!NP_UseAcct"
#define		NPRSN_USETACCT01			"!NP_UseTAcct01"		// 가장 최근에 사용한 위탁 계좌
#define		NPRSN_USETACCT11			"!NP_UseTAcct11"		// 가장 최근에 사용한 선물 계좌 
#define		NPRSN_USETACCT20			"!NP_UseTAcct20"		// 가장 최근에 사용한 위탁 계좌
#define		NPRSN_USETACCT33			"!NP_UseTAcct33"		// 가장 최근에 사용한 저축 계좌 
#define		NPRSN_USETACCT60			"!NP_UseTAcct60"		// 가장 최근에 사용한 위탁 계좌
#define		NPRSN_USETACCT91			"!NP_UseTAcct91"		// 가장 최근에 사용한 선물 계좌 
#define		NPRSN_USETACCT99			"!NP_UseTAcct99"		// 가장 최근에 사용한 저축 계좌 
#define		NPRSN_USETACCT62			"!NP_UseTAcct62"		// 가장 최근에 사용한 코스닥 선물 계좌 

#define		NPRSN_USEACCT01			"!NP_UseAcct01"		// 가장 최근에 사용한 위탁 계좌
#define		NPRSN_USEACCT11			"!NP_UseAcct11"		// 가장 최근에 사용한 선물 계좌 
#define		NPRSN_USEACCT20			"!NP_UseAcct20"		// 가장 최근에 사용한 저축 계좌 
#define		NPRSN_USEACCT33			"!NP_UseAcct33"		// 가장 최근에 사용한 저축 계좌 
#define		NPRSN_USEACCT60			"!NP_UseAcct60"		// 가장 최근에 사용한 위탁 계좌
#define		NPRSN_USEACCT91			"!NP_UseAcct91"		// 가장 최근에 사용한 선물 계좌 
#define		NPRSN_USEACCT99			"!NP_UseAcct99"		// 가장 최근에 사용한 저축 계좌 
#define		NPRSN_USEACCT62			"!NP_UseAcct62"		// 가장 최근에 사용한 코스닥 선물 계좌 
#define		NPRSN_SIWHANG			"!NP_SIWHANG"		// 시황 MAP SHARED KEY 
#define		NPRSN_RECENTDATE		"최근영업일"	    // 최근 영업일 
#define		NPRSN_JCODE				"코드-코코2"				// 종목코드(주식)
#define		NPRSN_UCODE				"코드-업종"			// 종목코드(업종)
#define		NPRSN_FCODE				"코드-선물"			// 종목코드(선물)
#define		NPRSN_FPCODE			"코드-공통"			// 종목코드(선물옵션)
#define		NPRSN_PCODE				"코드-옵션"			// 종목코드(옵션)

#define		NPRSN_BRANCH			"지점코드"			// 지점코드
#define 	NPRSN_EDITACCTBRANCH 	"지점계좌변경"		//지점코드 변경여부
#define		NPRSN_CLIENTIP			"!NP_CLIENTIP"      // CLIENT IP 제공
#define		NPRSN_NOTICHART			"노티-차트"			// MAP이 CHART에게 NOTIFY
#endif // __DOORIIC__NEOPLATFORM__KDKJKE9383__DKKDEW_3323DKDKLS_HEADER__
