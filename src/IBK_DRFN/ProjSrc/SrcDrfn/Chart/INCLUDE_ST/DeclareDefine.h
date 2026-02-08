#if !defined(AFX_DECLAREDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DECLAREDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_

#define		FT_CPLUSPLUS			1
#define		FT_VISUALBASIC			2
#define		FT_STRATEGY				3
#define		FT_USERFUNCTION			4
#define		FT_ALLFILE				10

#define		ST_TRADESTATION			0
#define		ST_YESTRADER			1

#define		IDM_GROUPCOPY_BEGIN			3100
#define		IDM_GORUPCOPY_END			4100
#define		IDM_GROUPMOVE_BEGIN			5100
#define		IDM_GORUPMOVE_END			6100

#define		CM_PARSE_COMPLETE_STRATEGY		10001
#define		CM_PARSE_COMPLETE_FUNCTION		10101
#define		CM_PARSE_COMPLETE_ALL			10201
#define		CM_COMPILE_COMPLETE_STRATEGY	10002
#define		CM_COMPILE_COMPLETE_FUNCTION	10102
#define		CM_COMPILE_COMPLETE_ALL			10202
#define		CM_MAKE_COMPLETE_STRATEGY		10003
#define		CM_MAKE_COMPLETE_FUNCTION		10103
#define		CM_MAKE_COMPLETE_ALL			10203
#define		CM_MAKE_FAILED_STRATEGY			40001
#define		CM_MAKE_FAILED_FUNCTION			40101
#define		CM_ZIP_FAILED_ALL				40201
#define		CM_UNDECLARE_MEMBER_STRATEGY	40003
#define		CM_UNDECLARE_MEMBER_FUNCTION	40103
#define		CM_NOT_INITIALIZED_STRATEGY		40004
#define		CM_NOT_INITIALIZED_FUNCTION		40104
#define		CM_NO_SEPARATOR_STRATEGY		40005
#define		CM_NO_SEPARATOR_FUNCTION		40105
#define		CM_NO_TERMINATOR_STRATEGY		40006
#define		CM_NO_TERMINATOR_FUNCTION		40106
#define		CM_NOT_INIT_ARRAY_STRATEGY		40007
#define		CM_NOT_INIT_ARRAY_FUNCTION		40107
#define		CM_ERROR_ASSIGN_STRATEGY		40008
#define		CM_ERROR_ASSIGN_FUNCTION		40108
#define		CM_ERROR_PARENTHESIS_STRATEGY	40009
#define		CM_ERROR_PARENTHESIS_FUNCTION	40109
#define		CM_ERROR_DUPLINPUT_STRATEGY		40010
#define		CM_ERROR_DUPLINPUT_FUNCTION		40110
#define		CM_ERROR_DUPLVAR_STRATEGY		40011
#define		CM_ERROR_DUPLVAR_FUNCTION		40111
#define		CM_ERROR_ENDSTMTERR_STRATEGY	40012
#define		CM_ERROR_ENDSTMTERR_FUNCTION	40112
#define		CM_ERROR_ENDSTMTERR2_STRATEGY	40013
#define		CM_ERROR_ENDSTMTERR2_FUNCTION	40113
#define		CM_ERROR_ELSESTMTERR_STRATEGY	40014
#define		CM_ERROR_ELSESTMTERR_FUNCTION	40114
#define		CM_ERROR_FUNCPARM_STRATEGY		40015
#define		CM_ERROR_FUNCPARM_FUNCTION		40115
#define		CM_UNKNOWN_ERROR_STRATEGY		49099
#define		CM_UNKNOWN_ERROR_FUNCTION		49199
#define		CM_STOPCOMPILING				49200
#define		CM_COPY_FAILED_STRATEGY			60001
#define		CM_COPY_FAILED_FUNCTION			60101
#define		CM_EMPTYFILE_STRATEGY			60002
#define		CM_EMPTYFILE_FUNCTION			60102
#define		CM_NO_TERM_COMMENT_STRATEGY		60003
#define		CM_NO_TERM_COMMENT_FUNCTION		60103
#define		CM_QUESTIONTOMANAGER			80000

#define		TREEINDEX_DEFUALTITEM			1000
#define		TREEINDEX_TOPGROUP				100
#define		TREEINDEX_GROUP					101

static int GTnumber(const void* n1, const void* n2)
{
	int* val1 = (int*)n1;
	int* val2 = (int*)n2;

	if(*val1 < *val2)
		return 1;
	else if(*val1 == *val2)
		return 0;
	else
		return -1;
}

#define		MAX_PACKET_LEN				8192				// UDP 최대 패킷 크기
#define		MAX_FT_PACKET_LEN			1572864				// TCP 최대 패킷 크기
#define		SOCK_COMMAND_CONNECT		100

// 전송 명령
#define		SOCK_CMD_START						1000			// 스크립트 전송 시작 - 파일명, 크기(리스트)
#define		SOCK_CMD_START_RESPONSE				1010			// 스크립트 파일 수신준비 완료
#define		SOCK_CMD_START_DEBUG				1050
#define		SOCK_CMD_START_RESPONSE_DEBUG		1060			// 스크립트 파일 수신준비 완료
#define		SOCK_CMD_UPLOAD_FILE				1100
#define		SOCK_CMD_UPLOAD_FILE_DEBUG			1150
#define		SOCK_CMD_DNLOAD_START				1200
#define		SOCK_CMD_DNLOAD_START_RESPONSE		1210
#define		SOCK_CMD_DNLOAD_START_DEBUG			1250
#define		SOCK_CMD_DNLOAD_START_RESPONSE_DEBUG	1260
#define		SOCK_CMD_DNLOAD_FILE				1300
#define		SOCK_CMD_TIMEOUT					1400
#define		SOCK_CMD_COMPILE_ERROR				1500
#define		SOCK_CMD_END_COMPILE				1600
#define		SOCK_CMD_START_UF					2000			// 스크립트 전송 시작 - 파일명, 크기(리스트)
#define		SOCK_CMD_START_RESPONSE_UF			2010			// 스크립트 파일 수신준비 완료
#define		SOCK_CMD_START_UF_DEBUG				2050			// 스크립트 전송 시작 - 파일명, 크기(리스트)
#define		SOCK_CMD_START_RESPONSE_UF_DEBUG	2060			// 스크립트 파일 수신준비 완료
#define		SOCK_CMD_UPLOAD_FILE_UF				2100
#define		SOCK_CMD_UPLOAD_FILE_UF_DEBUG		2150
#define		SOCK_CMD_DNLOAD_START_UF			2200
#define		SOCK_CMD_DNLOAD_START_RESPONSE_UF	2210
#define		SOCK_CMD_DNLOAD_START_UF_DEBUG		2250
#define		SOCK_CMD_DNLOAD_START_RESPONSE_UF_DEBUG	2260
#define		SOCK_CMD_DNLOAD_FILE_UF				2300
#define		SOCK_CMD_TIMEOUT_UF					2400
#define		SOCK_CMD_COMPILE_ERROR_UF			2500
#define		SOCK_CMD_START_ALL					3000			// 스크립트 전송 시작 - 파일명, 크기(리스트)
#define		SOCK_CMD_START_RESPONSE_ALL			3010			// 스크립트 파일 수신준비 완료
#define		SOCK_CMD_UPLOAD_FILE_ALL			3100
#define		SOCK_CMD_DNLOAD_START_ALL			3200
#define		SOCK_CMD_DNLOAD_START_RESPONSE_ALL	3210
#define		SOCK_CMD_DNLOAD_FILE_ALL			3300
#define		SOCK_CMD_TIMEOUT_ALL				3400
#define		SOCK_CMD_COMPILE_ERROR_ALL			3500
#define		SOCK_CMD_EXTRACT_ERROR_ALL			3600			// 압축해제 오류
#define		SOCK_CMD_COMPRESS_ERROR_ALL			3601			// 압축 오류
#define		SOCK_CMD_EJECT_OVERUSER				6000
#define		SOCK_CMD_READYTOCOMPILE				6001
#define		SOCK_CMD_READYTOCOMPILE_RESPONSE	6011
#define		SOCK_CMD_STOPCOMPILING				6002			// 컴파일 중지
#define		SOCK_CMD_STOPCOMPILING_RESPONSE		6003			// 컴파일 중지
#define		SOCK_CMD_CLOSE_IDLETIME				6004			// 유휴시간 종료
#define		SOCK_CMD_SERVERMESSAGE				7003			// 서버 메세지
#define		SOCK_CMD_POLLINGMASSAGE				7010			// 서버 폴링 메세지

#define		SOCK_CMD_NEXTFILE			_T("")			// 다음 파일 수신준비 완료
#define		SOCK_CMD_COMPILE			_T("")			// 컴파일중..
#define		SOCK_CMD_START_DLL			_T("")			// DLL 파일 전송시작 - 파일명, 크기(리스트)
#define		SOCK_CMD_DLL_RESPONSE		_T("")			// DLL 파일 수신준비 완료
#define		SOCK_CMD_NEXTFILE_DLL		_T("")			// 다음 파일 수신준비 완료
#define		SOCK_CMD_END				_T("")			// 컴파일 과정 완료

#define		STX                     0x02				// 패킷 시작 명령어
#define		ETX_PREV				_T("#PE#")			// 패킷 종료 문자열
#define		ETX                     0x03				// 패킷 종료 명령어


typedef void	(* FpNotifyList)(CPtrList*, long);
typedef void	(* FpNotifyExtract)(BOOL, LPCSTR);

#define		IDT_STARTCOMPILE				3214
#define		IDT_STARTCOMPILE_ZIP			3224

#define		COMPILE_PROGRESS_CERT			10
#define		COMPILE_PROGRESS_READYDLL		20
#define		COMPILE_PROGRESS_STARTDLL		30
#define		COMPILE_PROGRESS_UPFILE			40
#define		COMPILE_PROGRESS_DNFILE			70
#define		COMPILE_PROGRESS_CREATEFILE		80
#define		COMPILE_PROGRESS_COMPLETE		90

#define		COMPILE_PROGRESS_READYDLL_ZIP	15
#define		COMPILE_PROGRESS_STARTDLL_ZIP	20
#define		COMPILE_PROGRESS_UPFILE_ZIP		30

//	StradeStation keywords
static LPTSTR s_apszStsKeywordList[] =
{
	_T("AND"),
	_T("OR"),
	_T("NOT"),
	_T("ARRAY"),
	_T("INPUT"),
	_T("INPUTS"),
	_T("VAR"),
	_T("VARIABLE"),
	_T("VARIABLES"),
	_T("STRING"),
	_T("STRINGREF"),
	_T("STRINGSERIES"),
	_T("STRINGSIMPLE"),
	_T("STRINGARRAY"),
	_T("STRINGARRAYREF"),
	_T("NUMERIC"),
	_T("NUMERICREF"),
	_T("NUMERICSERIES"),
	_T("NUMERICSIMPLE"),
	_T("NUMERICARRAY"),
	_T("NUMERICARRAYREF"),
	_T("TRUEFALSE"),
	_T("TRUEFALSEREF"),
	_T("TRUEFALSESERIES"),
	_T("TRUEFALSESIMPLE"),
	_T("TRUEFALSEARRAY"),
	_T("TRUEFALSEARRAYREF"),
	_T("TRUE"),
	_T("FALSE"),
	_T("IF"),
	_T("ELSEIF"),
	_T("ELSE"),
	_T("THEN"),
	_T("BEGIN"),
	_T("END"),
	_T("FOR"),
	_T("TO"),
	_T("DOWN"),
	_T("WHILE"),
	_T("CROSS"),
	_T("ABOVE"),
	_T("BELOW"),
	_T("CURRENTBAR"),
	_T("시가"),
	_T("고가"),
	_T("저가"),
	_T("종가"),
	_T("거래량"),
	_T("OPEN"),
	_T("HIGH"),
	_T("LOW"),
	_T("CLOSE"),
	_T("VOLUME"),
	_T("FIND"),
	_T("BLACK"),
	_T("BLUE"),
	_T("GREEN"),
	_T("CYAN"),
	_T("RED"),
	_T("MAGENTA"),
	_T("YELLOW"),
	_T("WHITE"),
	_T("GRAY"),
	_T("LBLUE"),
	_T("LGREEN"),
	_T("LCYAN"),
	_T("PINK"),
	_T("LMAGENTA"),
	_T("LYELLOW"),
	_T("BWHITE"),
	_T("ONCLOSE"),
	_T("ATMARKET"),
	_T("ATLIMIT"),
	_T("ATSTOP"),
	_T("PERCENTSTOP"),
	_T("POINTSTOP"),
	_T("DEF"),
	NULL
};

//	YesTrader keywords
static LPTSTR s_apszStsKeywordList_YES[] =
{
	_T("AND"),
	_T("OR"),
	_T("NOT"),
	_T("ARRAY"),
	_T("INPUT"),
	_T("VAR"),
	_T("NUMERIC"),
	_T("LOGICAL"),
	_T("TRUE"),
	_T("FALSE"),
	_T("IF"),
	_T("ELSE"),
	_T("THEN"),
	_T("BEGIN"),
	_T("END"),
	_T("FOR"),
	_T("TO"),
	_T("WHILE"),
	_T("C"),
	_T("CLOSE"),
	_T("O"),
	_T("OPEN"),
	_T("H"),
	_T("HIGH"),
	_T("L"),
	_T("LOW"),
	_T("V"),
	_T("VOLUME"),
	_T("I"),
	_T("INDEX"),
	_T("M"),
	_T("MONEY"),
	_T("BIDS"),
	_T("ASKS"),
	_T("TICKS"),
	_T("UPTICKS"),
	_T("DOWNTICKS"),
	_T("UPVOL"),
	_T("DOWNVOL"),
	_T("DATE"),
	_T("SDATE"),
	_T("TIME"),
	_T("STIME"),
	_T("OI"),
	_T("OPENINTEREST"),
	_T("BP"),
	_T("BASEPRICE"),
	_T("FIND"),
	_T("BLACK"),
	_T("BLUE"),
	_T("GREEN"),
	_T("CYAN"),
	_T("RED"),
	_T("MAGENTA"),
	_T("YELLOW"),
	_T("WHITE"),
	_T("GRAY"),
	_T("LBLUE"),
	_T("LGREEN"),
	_T("LCYAN"),
	_T("PINK"),
	_T("LMAGENTA"),
	_T("LYELLOW"),
	_T("BWHITE"),
	_T("ONCLOSE"),
	_T("ATMARKET"),
	_T("ATLIMIT"),
	_T("ATSTOP"),
	_T("PERCENTSTOP"),
	_T("POINTSTOP"),
	_T("DEF"),
	_T("VAR1"),
	_T("VAR2"),
	_T("VAR3"),
	_T("VAR4"),
	_T("VAR5"),
	_T("VAR6"),
	_T("VAR7"),
	_T("VAR8"),
	_T("VAR9"),
	_T("VAR10"),
	NULL
};

#define		SVRMSG_NORMAL		0
#define		SVRMSG_ALERT		1
#define		SVRMSG_WARNING		2
#define		SVRMSG_SPECIAL		3

#define		STRATEGY_INDEX		0x0001
#define		STRATEGY_ORDER		0x0002
#define		STRATEGY_BULL		0x0004
#define		STRATEGY_FIND	0x0008
#define		STRATEGY_RESERVE2	0x0010

//#define		PATH_STDATA			_T("\\Data\\STData\\")
#define		PATH_STDATA			_T("\\Icss\\STData\\")	//@Solomon

//#define		PATH_USER			_T("\\User\\STData\\")
#define		PATH_SCRIPTFILES	_T("\\Usr\\STData\\ScriptFiles\\")
//#define		PATH_DLLFILES		_T("\\User\\STData\\SD\\")
//#define		PATH_DLLFILES_D		_T("\\User\\STData\\SDD\\")
#define		PATH_STFILES		_T("\\Usr\\STData\\ST\\사용자단위전략\\")

//@유진추가
#define		PATH_USER2			_T("\\STData\\")
#define		PATH_SCRIPTFILES2	_T("\\STData\\ScriptFiles\\")
#define		PATH_DLLFILES2		_T("\\STData\\SD\\")
#define		PATH_DLLFILES_D2	_T("\\STData\\SDD\\")
#define		PATH_STFILEFOLDER	_T("\\STData\\ST\\")
#define		PATH_STFILES2		_T("\\STData\\ST\\사용자단위전략\\")
#define		KEY_USERDIRECTORY	_T("USERDIRECTORY")

#define		KEY_GETENVVARIABLE()	char xUserEncDir[MAX_PATH]={0,};	\
		DWORD xRead = GetEnvironmentVariable(KEY_USERDIRECTORY, xUserEncDir, MAX_PATH);
//@유진추가

#include "../Common_ST/STSiteDef.h"	//'@', "@" 정의

static BOOL GetSTFile(LPSTR lpszConfigPath, LPCTSTR lpszScriptFile, LPCTSTR lpszRootPath)
{
	//[ 사용자 경로관련 수정. 07.01.12
	KEY_GETENVVARIABLE();

	LPCSTR szAt = ::strchr(lpszScriptFile, '@');
	if(szAt)
	{
//@유진삭제::wsprintf(lpszConfigPath, _T("%s%s%s.ST"), lpszRootPath, PATH_STFILES, lpszScriptFile);
		if(xRead==0)
			::wsprintf(lpszConfigPath, _T("%s%s%s.ST"), lpszRootPath, PATH_STFILES, lpszScriptFile);
		else
			::wsprintf(lpszConfigPath, _T("%s%s%s.ST"), xUserEncDir, PATH_STFILES2, lpszScriptFile);
		return TRUE;
	}

//@유진삭제::wsprintf(lpszConfigPath, _T("%s%s%s@*.ST"), lpszRootPath, PATH_STFILES, lpszScriptFile);
	if(xRead==0)
		::wsprintf(lpszConfigPath, _T("%s%s%s%c*.ST"), lpszRootPath, PATH_STFILES, lpszScriptFile, gSTGubunKey);
	else
		::wsprintf(lpszConfigPath, _T("%s%s%s%c*.ST"), xUserEncDir, PATH_STFILES2, lpszScriptFile, gSTGubunKey);

	CFileFind ff;
	BOOL bFind = ff.FindFile(lpszConfigPath);

	if(bFind)
	{
		ff.FindNextFile();		// 일치하는 결과는 하나이므로...
		::lstrcpy(lpszConfigPath, ff.GetFilePath());
	}
	else
	{
//@유진삭제::wsprintf(lpszConfigPath, _T("%s%s%s@0.ST"), lpszRootPath, PATH_STFILES, lpszScriptFile);
		if(xRead==0)
			::wsprintf(lpszConfigPath, _T("%s%s%s%c0.ST"), lpszRootPath, PATH_STFILES, lpszScriptFile, gSTGubunKey);
		else
			::wsprintf(lpszConfigPath, _T("%s%s%s%c0.ST"), xUserEncDir, PATH_STFILES2, lpszScriptFile, gSTGubunKey);

		char szNewPath[MAX_PATH] = {NULL, };
		::wsprintf(szNewPath, _T("%s%sNewScript.ST"), lpszRootPath, PATH_STDATA);

		::CopyFile(szNewPath, lpszConfigPath, FALSE);
	}

	// 읽기 전용 해제
	DWORD dwAttributes = GetFileAttributes(lpszConfigPath);
	if(dwAttributes & FILE_ATTRIBUTE_READONLY)
	{
		dwAttributes ^= FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(lpszConfigPath, dwAttributes);
	}

	return bFind;
}

#endif // !defined(AFX_DECLAREDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_)
