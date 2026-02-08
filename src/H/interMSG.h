#pragma once

#define WM_INTERMSG								 WM_USER + 2022
#define WM_MSG_MAINTOMAP					 WM_USER + 2023

#define MMSG_SET_INTEREMAP							8    //IBXXXX12 로드후 윈도우 핸들 저장
#define MMSG_SEARCH_INTERGROUP				9   //관심그룹 리스트 조회
#define MMSG_RET_SEARCH_INTERGROUP		10   //관심그룹 리스트 조회 결과 전달
#define MMSG_SEARCH_GROUPCODE				11   //관심그룹 종목 조회
#define MMSG_RET_SEARCH_GROUPCODE		12   //관심그룹 종목 조회 결과 전달
#define MMSG_INTER_DBBACKUP						13   //관심 DB백업 -> 관심설정창을 열면 날린다
#define MMSG_RET_INTER_DBBACKUP				14  //관심 DB백업 조회결과 전달
#define MMSG_INTER_NEWGROUP					15   //관심 새그룹등록
#define MMSG_RET_INTER_NEWGROUP				16   //관심 새그룹등록  조회 결과 전달
#define MMSG_INTER_GROUPCODESAVE			17  //관심 그룹내부종목저장
#define MMSG_RET_INTER_GROUPCODESAVE	18  //관심 그룹내부종목저장
#define MMSG_SEARCH_SISE								19  //관심 그룹코드의 시세 조회
#define MMSG_TIME_INVESTTUJA						20 //티커 투자자 타이머(임시)

//cx_symbol  종목 메모 관련 Trkey
#define TRKEY_MEMO_OOP			 118		//여분
#define TRKEY_MEMO_POPUPOOP 119		//cx_symbol 에서 조회하는 메모장데이터용 oop
#define TRKEY_MEMO_INSERT 120			 //'I' 등록    전체요청, 전체응답
#define TRKEY_MEMO_SEARCH 121		//'S' 조회    부분요청 가능,  전체응답
#define TRKEY_MEMO_UPDATE 122       //'U' 수정   전체요청, 전체응답
#define TRKEY_MEMO_DELETE 123        //'D' 삭제     부분요청 가능,  부분응답
#define TRKEY_MEMO_CHECK 124         //'E' 존재여부확인  부분요청 가능,  부분응답

inline CString GetMemoTrKeyType(int trkey)
{
	CString strtmp;
	switch (trkey)
	{
	case 20:
		strtmp.Format("[%d][%s]", trkey, "서버저장모드가 아닌 dll 화면");
		break;
	case TRKEY_MEMO_INSERT:
		strtmp.Format("[%d][%s]", trkey, "메모추가");
		break;
	case TRKEY_MEMO_SEARCH:
		strtmp.Format("[%d][%s]", trkey, "메모서버조회");
		break;
	case TRKEY_MEMO_UPDATE:
		strtmp.Format("[%d][%s]", trkey, "메모수정 및 업데이트");
		break;
	case TRKEY_MEMO_DELETE:
		strtmp.Format("[%d][%s]", trkey, "메모 삭제");
		break;
	case TRKEY_MEMO_CHECK:
		strtmp.Format("[%d][%s]", trkey, "메모 존재 여부 확인");
		break;
	}

	return strtmp;
}

inline CString GetMemoTrKeyType(char trkey)
{
	CString strtmp;
	switch (trkey)
	{
	case 20:
		strtmp.Format("[%d][%s]", trkey, "서버저장모드가 아닌 dll 화면");
		break;
	case TRKEY_MEMO_INSERT:
		strtmp.Format("[%d][%s]", trkey, "메모추가");
		break;
	case TRKEY_MEMO_SEARCH:
		strtmp.Format("[%d][%s]", trkey, "메모서버조회");
		break;
	case TRKEY_MEMO_UPDATE:
		strtmp.Format("[%d][%s]", trkey, "메모수정 및 업데이트");
		break;
	case TRKEY_MEMO_DELETE:
		strtmp.Format("[%d][%s]", trkey, "메모 삭제");
		break;
	case TRKEY_MEMO_CHECK:
		strtmp.Format("[%d][%s]", trkey, "메모 존재 여부 확인");
		break;
	}

	return strtmp;
}