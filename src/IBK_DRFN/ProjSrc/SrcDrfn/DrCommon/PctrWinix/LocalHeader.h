#if !defined(AFX_LOCAL_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_)
#define AFX_LOCAL_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_

// #include "./LocalHeader.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IAUGwanMng.h"
#include "../../inc/IWinixPlatform.h"
#include "../../Inc/IAUTrCommMng.h"

#include "../../Chart/COMMONTR/TRDefine.h"

//#include "../../../SrcSite/include/FxInterestManager.h"
//#include "../../../SrcSite/include/InterestInfo.h"
//관심조회관련 tr
struct _uinfo {
	char gubn[2]; // 업무구분
	   // "MY" : 관심종목
	   // "UL" : 사용자 데이터 목록 
	   // "UD" : 사용자 데이터
	char dirt[1]; // 전송방향 'U' : pc==>host, 'D' : host==>pc, 'X' : host==>pc (conversion), 'R' : 재조회
	char cont[1]; // 연속구분 'F':First, 'M':Middle, 'L':First&Last, 'C':Cancel, 
		 // 'S':그룹편집, 'E':그룹조합처리, 'b':DB백업, 'R':DB원복, 'V':최종상태저장, 'r':최종상태복원
		 // 'D':그룹삭제, 'j':해당그룹종목전송, 'g':그룹리스트조회, 'G':그룹종목전송, 'C':전송취소
	char name[80]; // file name
	char nblc[5]; // 블럭개수, 파일크기(max:64k)
	char retc[1]; // return value 'O':정상 'E':에러 'U'신규 정상
	char emsg[40]; // error message
};

struct _ginfo {
	char gnox[2]; // group number
	char gnam[20]; // group name
	char jrec[4]; // _jinfo count
};

struct _jinfo {
	char gubn[1]; // 종목구분 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char code[12]; // 종목코드
	char xprc[10]; // 보유단가
	char xnum[10]; // 보유수량
};

struct _updn {
	_uinfo uinfo;
	_ginfo ginfo;
};

const int sz_uinfo = sizeof(struct _uinfo);
const int sz_ginfo = sizeof(struct _ginfo);
const int sz_jinfo = sizeof(struct _jinfo);

const UINT RMSG_SENDMESSAGE = ::RegisterWindowMessage(_T("RMSG_SENDMESSAGE"));

class CWinixGwanItem
{
public:
	CWinixGwanItem(HWND hWinix, HWND hDrfnWrap, HWND hGwanReciever);
	~CWinixGwanItem();
	
	void DoInitialize(BOOL bCreate);
	void RequestGwansimGroup();
public:
	// 2012.2.7 박성경: 관심 솔로몬
	//CFxInterestManager* m_pIM;
	HWND  m_hWinix, m_hDrfnWrap, m_hGwanReciever;
	
	class CTrCommSite : public ITrCommSite
	{
	public:
		STDMETHOD(ReceiveData) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen);
		STDMETHOD(ReceiveRealData) (WORD wFID, LPCSTR szMainKey, void* pRealData);
		STDMETHOD(StatusChange) (int nState);
		STDMETHOD(ReceiveMessage) (long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage);
	} m_xTrCommSite;
	friend class CWinixGwanItem;
	
	// 	enum{
	// 		wgi_ready=0,
	// 		wgi_querygroup=1,
	// 		wgi_queryitem,
	// 	};
	// 	int m_nQueryState;
	ITrCommSite* m_pCallBackSite;
};
typedef CList<CWinixGwanItem*, CWinixGwanItem*> List_CWinixGwanItem;

class CWinixGwanGroup
{
public:
	CString m_szKey;
	CString m_szData;
};
typedef CList<CWinixGwanGroup*, CWinixGwanGroup*> List_CWinixGwanGroup;

class CWinixGwanGroupList
{
public:
	CWinixGwanGroupList();
	~CWinixGwanGroupList();
	void _DoClearList();

public:
	int m_nVersion;
	List_CWinixGwanGroup m_List;
};

#endif //AFX_LOCAL_H__44F0C9D2_6436_434C_8614_0C5E7BEB72CB__INCLUDED_