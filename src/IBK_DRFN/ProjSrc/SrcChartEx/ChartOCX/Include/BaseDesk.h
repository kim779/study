// 2000.11.24 이준옥(Junok Lee)(mailto:alzio@dooriic.co.kr)
// CBaseDesk2, CDeskParent2   added by Junok Lee. 2001. 07. 12

#if !defined(AFX_0DBASE_DESK__D1DA4A3_84KKEI_EJKJKJD_FHDD0D2E__INCLUDED_)
#define AFX_0DBASE_DESK__D1DA4A3_84KKEI_EJKJKJD_FHDD0D2E__INCLUDED_

// #include "../include/BaseDesk.h"

#define		GDB_OK					0	// 성공
#define		GDB_ERROR				-1	// UnKnown Error

class CBaseDesk {
public:
	// * 기본 설치경로를 얻는다.
	// * 이 디렉토리를 기준으로 해서 Bin, Data 등의 디렉토리를 구한다.
	virtual LPCSTR GetBasePath() { ASSERT(FALSE); return NULL; }

	// * 데스크의 핸들을 구한다.
	virtual HWND  GetDeskHwnd() { ASSERT(FALSE); return m_hDeskHwnd; }

	// * 데스크의 IDispatch를 구한다.
	virtual LPDISPATCH GetUnknown() { ASSERT(FALSE); return NULL; }

	// * 컨트롤의 IDispatch를 구한다.
	virtual LPUNKNOWN GetCtrlUnknown(LPCTSTR szName) { return NULL; }

	// * 컨트롤의 핸들을 구한다.
	virtual HWND GetCtrlHwnd(LPCTSTR szName) { return NULL; }

	//* Global 공유의 클래스 포인터를 구한다.
	virtual LPVOID GetSharedInterface() { return NULL; }

	// * 핸들을 가지고 이벤트를 발생시킨다.
	// * 데스크에게 이벤트가 일어남을 알린다.
	virtual BOOL InvokeEvent(HWND _hWnd, LPCSTR _szEventName, LPCSTR _szParam) { return TRUE; }

	// * 컨트롤의 이름으로 이벤트를 발생시킨다.
	// * 데스크에게 이벤트가 일어남을 알린다.
	// * 컨트롤의 이름은 SetAllProperties2을 통해서 넘어간다.
	// * 아직 구현 전일지도 모르니 확인하고 작업하기 바람. 2000.12.04
	// * !VBNAME= 형태로 값을 넘겨준다.
	virtual BOOL InvokeEvent2(LPCSTR _szCtrlName, LPCSTR _szEventName, LPCSTR _szParam) { return TRUE; }

	// * 데스크에게 키 상태를 알린다.
	virtual BOOL SendKey(HWND _hWnd, UINT nChar, BOOL _bIsShift, BOOL _bIsCtrl) { return FALSE; }

	// * Local 맵단위 공유 자료를 구한다.
	virtual LPVOID GetLSharedData() { return NULL; }

	// * 데스크에게 코맨드형태의 명령을 콜한다.
	virtual void CallCommand(LPCSTR _szCmd) {}

protected:
	CString	m_szPath;
	HWND	m_hDeskHwnd;
	LPUNKNOWN	m_pDeskUnk;
};

class CBaseDesk2 : public CBaseDesk
{
public:
	// * 색상정보는 GetMainInfo 함수를 호출할 때 키값을 "USERCOLORTABLE" 해서 얻은 결과다.
	// * nFlag 0 : nIndex값을 이용해 컬러값 구하기
	// *       1 : szName을 이용해 컬러값 구하기
	// * 36=일반프레임바탕색,190,221,250
	// * nFlag=0이고 nIndex가 36일 때 RGB(190,221,250)
	// * nFlag=1이고 szName이 "일반프레임바탕색"일 때 RGB(190,221,250)
	virtual OLE_COLOR GetUserColor(short nIndex, LPCTSTR szName, short nFlag) { return RGB(0,0,0); }

	// * 데스크에게 필요한 정보를 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	virtual long GetDeskInfo(LPCSTR szKey) { return NULL; }

	// * 사용자의 정보에 관한 것을 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	// * CBaseDeskParent2에 있는 같은 함수를 콜한다.
	virtual long GetUserInfo(LPCSTR szKey) { return NULL; }

	// * 맵이 올라가는 프레임의 정보에 관한 것을 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	// * CBaseDeskParent2에 있는 같은 함수를 콜한다.
	virtual long GetFrameInfo(LPCSTR szKey) { return NULL; }

	// * 메인(CMainFrame, CWinApp)등에서 관리하는 정보에 관한 것을 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	// * CBaseDeskParent2에 있는 같은 함수를 콜한다.
	virtual long GetMainInfo(LPCSTR szKey) { return NULL; }
};

// 메인 또는 NeoDesk의 부모에서 셋팅해줘야할 기본 클래스
class CBaseDeskParent 
{
public:
	// * 부모에게 기본경로(설치경로)를 얻어온다. 
	virtual LPCSTR GetBasePath() { ASSERT(FALSE); return NULL; }

	// * 부모의 핸들을 구한다.
	virtual HWND  GetParentHwnd() { ASSERT(FALSE); return NULL; }

	// * 부모의 IDispatch 포인터를 구한다.
	virtual LPDISPATCH GetParentDisp() { ASSERT(FALSE); return NULL; }

	// * { 종목코드 처리
	// * CBaseCodeTable의 포인터를 넘겨준다.
	// * 현재 사용하지 않는다.
	virtual long GetBaseCodeTable() {ASSERT(FALSE);return NULL;}
	// }

	// { 화면열기
	// _nFlag : (0) 현재메인의 화면여는 룰을 따름
	//          (1) 맵을 새화면에 연다.
	//          (2) 맵을 현재화면에 연다.
	//          (3) 맵을 팝업으로 연다.
	//          (4) 맵을 다이얼로그 형태로 연다.
	// _szParam : 화면을 열면서 그 화면에게 넘겨주는 데이터
	// * 아래 OpenMap은 사용하지 마세요. 메시지형식으로 화면을 열도록하는 루틴으로 바뀌었슴다.
	virtual BOOL OpenMap(LPCSTR _szMapID, int _nFlag, LPCSTR _szParam) {ASSERT(FALSE); return FALSE;}
	// }

	// { 기타 예상하지 못한 기능을 위해 추가(1)
	// 원하는 _szFunc기능이 있으면 TRUE,
	// _szFunc, _szParam을 이용해 원하는 처리를 한다.
	virtual BOOL InvokeEx1(LPCSTR _szFunc, LPCSTR _szParam, CString& szResult) {ASSERT(FALSE); return FALSE;}

	// { 기타 예상하지 못한 기능을 위해 추가(2)
	// 원하는 _szFunc기능이 있으면 TRUE,
	// _szFunc, _dwParam 이용해 원하는 처리를 한다.
	virtual BOOL InvokeEx2(LPCSTR _szFunc, long _dwParam, long& dwResult) {ASSERT(FALSE); return FALSE;}
	// }

	// * 데스크의 부모에게 코맨드형태의 명령을 콜한다.
	virtual void CallCommand(LPCSTR _szCmd) {}

	// * 데스크의 부모에게 코맨드형태의 명령을 콜한다.
	virtual void CallCommand(int _nType, long _dwData, long& dwResult) {}

	// * 데스크의 부모에게 코맨드형태의 명령을 콜한다.
	// * 핸들을 같이 넘겨누는 기능만 더 추가된 형태
	virtual void CallCommand(HWND _hDesk, LPCSTR _szCmd) {}

	// * 키값을 주고 거기에 해당하는 정보를 구하기 위해 사용한다.
	// * 이 부분은 사이트마다 다양하게 사용하기 위한 기능으로 활용할 수 있다.
	// * 메인과 어떤 키값일때는 어떤 정보를 받을것인지 얘기가 되어야 한다.
	virtual long GetKeyData(LPCSTR _szKey1, LPCSTR _szKey2){return NULL;}
	virtual long GetKeyData(LPCSTR _szKey, int _nKey){return NULL;}
};

class CBaseDeskParent2 : public CBaseDeskParent
{
public:
	// * 사용자의 정보에 관한 것을 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	// * CBaseDeskParent2에 있는 같은 함수를 콜한다.
	virtual long GetUserInfo(LPCSTR szKey) { return NULL; }

	// * 맵이 올라가는 프레임의 정보에 관한 것을 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	// * CBaseDeskParent2에 있는 같은 함수를 콜한다.
	virtual long GetFrameInfo(LPCSTR szKey) { return NULL; }

	// * 메인(CMainFrame, CWinApp)등에서 관리하는 정보에 관한 것을 요청한다.
	// * 필요할 때마다 요청해서 추가한다. 일단 컨트롤에 추가하면 고정.
	// * CBaseDeskParent2에 있는 같은 함수를 콜한다.
	virtual long GetMainInfo(LPCSTR szKey) { return NULL; }
};

#endif // AFX_0DBASE_DESK__D1DA4A3_84KKEI_EJKJKJD_FHDD0D2E__INCLUDED_
