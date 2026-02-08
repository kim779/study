#if !defined(AFX_INTERFACE_OF_MAININFO_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_MAININFO_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IMainInfoMng.h"
#include "IBaseDefine.h"
//  int nID = 21;
// 	IMainInfoManager* pMainInfoMng = (IMainInfoManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(nID);

#define	MAX_IMainInfoManager		30  // 주의 ID시작은 '0'

#define	USERINFOMNG_USERID			0
#define	USERINFOMNG_USERENCID		1
#define	USERINFOMNG_ROOTDIR			10		// Root Directory
#define	USERINFOMNG_EXECDIR			11		// 실행 디렉토리
#define	USERINFOMNG_DATADIR			12		// Data 디렉토리
#define	USERINFOMNG_REALUSERDIR		17		// 실제적인 사용자 디렉토리. 사이트마다 다름.

// ID 정의 및 자세한 사항은 문서[정리_PC데이터매니저.xls] 및 [정리_PC데이터매니저.doc] 참조. 
/* --------------------------------------
ID		Contents
--------------------------------------
0		로그인아이디
1		암호화된 아이디
2		로그인(사용자)명
3		사용자암호
4		인증서비밀번호
5		사용자구분
6		사용자등급
7		마지막로그인날자
8		주민번호
9		HTS사용약관등록여부
10		루트디렉토리
11		메인(bin)디렉토리
12		데이터(Data)디렉토리
13		도움말(Help)디렉토리
14		이미지(Image)디렉토리
15		로그(Log)디렉토리
16		맵(Map)디렉토리
17		사용자디렉토리(User/#ID)
18		공인인증사용여부
19		공인인증 메시지 안내문 표시 여부
20		자동입력여부
21		서버명
22		서버IP
23		접속시간
24		서버날자
25		서버구분
26		사용자IP
27		접속방법
28		ATM
-------------------------------------- */

#define		IMainInfoManagerLast		IMainInfoManager0002

DECLARE_INTERFACE_(IMainInfoManager, IBaseDataString)
{
	// '사용자아이디' 리턴
	// I-PARAM : -
	// O-PARAM : 유저아이디
	STDMETHOD_(LPCSTR, GetUserID)() PURE;

	// '암호화된 사용자아이디' 리턴
	// I-PARAM : -
	// O-PARAM : 암호화된 유저아이디
	STDMETHOD_(LPCSTR, GetEncUserID)() PURE;
	
	// Root디렉토리 (bin디렉토리의 위단계 Dir)
	// I-PARAM : -
	// O-PARAM : Root디렉토리
	STDMETHOD_(LPCSTR, GetRootDir)() PURE;

	// Data디렉토리
	STDMETHOD_(LPCSTR, GetDataDir)() PURE;

	// UserID에 할당된 유저디렉토리
	// I-PARAM : -
	// O-PARAM : UserID에 할당된 유저디렉토리
	STDMETHOD_(LPCSTR, GetEncUserDir)(LPCSTR szKey=NULL) PURE;
	
	// szKey="ISHTS"일 때 지점(인지 HTS인지 알려준다.
	// I-PARAM : LPCSTR szKey
	// O-PARAM : 
	STDMETHOD_(CString, GetLoginInfo)(LPCSTR szKey) PURE;

	//-->@.환경변수 처럼 관리
	STDMETHOD_(BOOL, SetEnvironmentVariable)(
				LPCTSTR lpName,  // environment variable name
				LPCTSTR lpValue  // new value for variable
			) PURE;

	STDMETHOD_(DWORD, GetEnvironmentVariable)(
				LPCTSTR lpName,		// environment variable name
				CString &rValue		// CString for variable value
			) PURE;
	STDMETHOD_(BOOL, FreeEnvironmentStrings)(
				LPTSTR lpName  // environment variable name
			) PURE;

	STDMETHOD_(DWORD, GetEnvironmentStrings)(CStringList& szBufferList) PURE;

	// INI파일에서 읽어서 셋팅한다.
	// INI포맷은 아래형태, key와 data 구분은 콜론(:)으로 한다.
	// Count= 0부터 시작.
	// #=key:data
	STDMETHOD_(int, IniToEnvironmentTable)(LPCSTR szIniPath, LPCSTR szSect) PURE;	
	//<--
};

DECLARE_INTERFACE_(IMainInfoManager0002, IMainInfoManager)
{
	STDMETHOD_(int, GetCountOfAccount)() PURE;
	STDMETHOD_(BOOL, GetUserAccount)(CStringArray& saAccount) PURE;
	STDMETHOD_(CString, GetAccountName)(LPTSTR lpszAccount) PURE;
	STDMETHOD_(CString, GetAccountPass)(LPTSTR lpszAccount) PURE;
	
	STDMETHOD_(int, GetHtsMode)() PURE;
	STDMETHOD_(CString, GetLoginPass)() PURE;
};

#define	USE_IMainInfoManager(xxVal) IMainInfoManager* xxVal=(IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);



#endif //AFX_INTERFACE_OF_MAININFO_1464_45BB_BY_JUNOK_LEE__INCLUDED_
