// ExOCXDLL.h: interface for the CExOCXDLL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXOCXDLL_H__77308CF7_9EA9_4613_A2F1_40AC279DDBF6__INCLUDED_)
#define AFX_EXOCXDLL_H__77308CF7_9EA9_4613_A2F1_40AC279DDBF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ExDialog.h"
#include "ExControls.h"

#include "ExPlatform.h"	// 플랫폼 관련 부분 모음.
#include "WndObj.h"
#include "Packet.h"

#include "OcxDLLDef.h"

class CExOCXDLL : public CExDialog  
{
	DECLARE_DYNCREATE(CExOCXDLL);

// Construction
public:
	CExOCXDLL();   // standard constructor
	CExOCXDLL(const UINT nID, CWnd* pParent = NULL);   // standard constructor);

#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

	USE_DR_PLATFORM();
	
	void SetPermanentHandle(CWnd* pWnd);

	CString		GetCodeName(LPCTSTR lpszCode);
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/12
	// Return		: void
	// Paramter
	//		UINT nFlag :
	//         1 : nParam1(x방향 증감), nParam2(y방향 증감)
	//		   2 : nParam1(x방향 절대값), nParam2(y방향 절대값)
	//		   3 : nParam1(x방향 절대값)
	//		   4 : nParam1(y방향 절대값)
	//		   5 : nParam1(x방향 절대값<전체영역>), nParam2(y방향 절대값<전체영역>)
	//		   6 : nParam1(x방향 절대값<전체영역>)
	//		   7 : nParam1(y방향 절대값<전체영역>)
	//		   8 : nParam1(x방향 절대값<내부영역>), nParam2(y방향 절대값<내부영역>)
	//		  10 : Flag가 1인 경우와 동일하나 스플릿으로 나뉜 화면에서 호출한 경우
	//		short nSizeX : X증감값
	//		short nSizeY : Y증감값
	// Comments		: ChildFrame의 크기를 증감한다.
	//				  크기값이 -이면 감소값
	//-----------------------------------------------------------------------------
	void	ChangeFrameSize(UINT nFlag, short nSizeX, short nSizeY);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/12
	// Return		: HWND ChildFrame의 핸들값
	// Paramter
	//		void
	// Comments		: ChildFrame의 핸들을 리턴
	//-----------------------------------------------------------------------------
	AFX_INLINE HWND	GetFrameHandle();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: CResourceManager* 리소스 매니저 포인터
	// Paramter
	//		void
	// Comments		: 화면에서 사용되는 리소스 매니저를 넘겨받는다
	//-----------------------------------------------------------------------------
	AFX_INLINE CResourceManager* GetResourceManager();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: void
	// Paramter
	//		CString strDLLParam : DtsDll.ocx에서 넘겨받은 파라메터 전체 문자열
	// Comments		: DtsDll.ocx에서는 맵 디자인때 세팅된 파라메터(3개)를 dll화면 초기에
	//				  넘겨준다. 이때 파라메터를 Key=Value형태로 화면 다이얼로그에서 저장하는데
	//				  이때 상용되는 함수임.
	//				  CStdDialog에 이비 구현되어있어 일반적으로 사용하는 일은 없고
	//				  파라메터 구분에 따라 분기되는 경우에 사용할 수 있음
	//-----------------------------------------------------------------------------
	void	SetParameters1(CString strDLLParam);
	void	SetParameters2(CString strDLLParam);
	void	SetParameters3(CString strDLLParam);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/17
	// Return		: Key에 해당하는 Value
	// Paramter
	//		LPCTSTR lpszKey : 파라메터 Key
	// Comments		: dll OCX는 Dialog를 Create하기전에 3개의 Paramter를 넘겨준다
	//				  Parameter는 맵젠에서 세팅가능하며 dll화면이 넘겨받는다.
	//				  각 Parameter는 문자열 형이고 여러개의 Key=Value형태를 세미콜론(;)으로 
	//				  구분한다.
	//				  예를 들면 맵젠에서 첫번째 Parameter 세팅이 "AA=10;BB=-30;CC=0"이었다면
	//				  GetParameter1(_T("BB");의 리턴값은 "-30"이 되는것이다.
	//-----------------------------------------------------------------------------
	CString		GetParameter1(LPCTSTR lpszKey);
	CString		GetParameter2(LPCTSTR lpszKey);
	CString		GetParameter3(LPCTSTR lpszKey);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/17
	// Return		: void
	// Paramter
	//		void
	// Comments		: 사용한 파라메터 정보는 Create때에만 필요한 경우가 많다.
	//				  따라서 파라메터를 사용하고 더이상 사용하지 않아 메모리에서 삭제할때
	//				  이 함수를 호출한다. 파라메터는 3개있으므로 그에 해당하는 함수를 호출한다.
	//				  dll화면이 종료될때에 이 함수를 호출하지 않아도 메모리에서 삭제된다.
	//-----------------------------------------------------------------------------
	void	RemoveParameter1();
	void	RemoveParameter2();
	void	RemoveParameter3();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/17
	// Return		: void
	// Paramter
	//		void
	// Comments		: 데스크 OCX크기데로 다이얼로그 크기를 변경시킨다.
	//-----------------------------------------------------------------------------
	void	DoFullWindowSize();

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: void
	// Paramter
	//		LPCTSTR lpszMessage : 메세지윈도우에 표시할 문자열
	// Comments		: 맵화면 하단에 있는 메세지 윈도우에 별도로 문자열을 표시할 수 있음
	//				  메세지 문자열의 구조는 서버에서 내려받는 메세지 문자열 형태이어야 함
	//-----------------------------------------------------------------------------
	void	SendMessageString(LPCTSTR lpszMessage);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/2
	// Return		: void
	// Paramter
	//		void
	// Comments		: dll 화면에서 fnLoad이후 호출되는 함수
	//-----------------------------------------------------------------------------
	virtual void FormLoad();
	
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/12/14
	// Return		: void
	// Paramter
	//		void
	// Comments		: 데스크에서 모든 컨트롤, 스크립트 로드, 공유가 완료되었을때 호출됨
	//-----------------------------------------------------------------------------
	virtual void FormLoadCompleted();

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/2/21
	// Return		: 탭오더 리스트 갯수
	// Paramter
	//		HWND hFocus : 탭오더 이외에 포커스상태일때 탭오더로 이동되는 윈도우핸들
	//		HWND hChild : 탭오더에 포함될 윈도우핸들
	//		... : 추가 탭오더에 포함될 윈도우핸들 (항상 마지막 인자에 0을 입력)
	// Comments		: CExOCXDLL에서 탭오더로 포커스를 받는 윈도우핸들을 구성
	//-----------------------------------------------------------------------------	
	int	AddTabStopHandle(HWND hFocus, HWND hChild, ...);

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/2/21
	// Return		: void
	// Paramter
	//		void
	// Comments		: 탭오더 리스트 삭제
	//				  탭오더 리스트를 재구성할때에는 이 함수를 호출해서 리스트를 삭제
	//-----------------------------------------------------------------------------
	void ClearTabStopHandle();

	int ExMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	long GetOpenMapData(CString& strData);
	void SetOpenMapData(LPCTSTR lpszData);

protected:
	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/2/18
	// 함수 이름          : Platform_GetBase
	// 리턴 타입          : long
	// 리턴 타입 설명     : 
	// 파라미터
	//		LPCTSTR szKey    : 
	// 함수 설명          : ■ TR IN / OUT에 관련된 클래스 정보를 얻어온다.
	//						   SzKey값에 따라 
	//                         "PACKIN"  : CBaseCtrlPackIn를 구현한 클래스의 포인터
	//                         "PACKOUT" : CBaseCtrlPackOut를 구현한 클래스의 포인터
	//                      ■ DLL화면에서 TR처리를 위해서는 구현을 해줘야 한다.
	///////////////////////////////////////////////////////////////////////////////
	virtual	long Platform_GetBase(LPCTSTR szKey);
	virtual long Platform_GetKey(LPCTSTR szKey);
	virtual void Platform_SetKey(LPCTSTR szKey, long dwKey);

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/2/21
	// 함수 이름          : Platform_Initialize
	// 리턴 타입          : void
	// 파라미터
	//		long dwMainDllObj  : 
	//		long dwBaseDesk  : 
	//		long dwRscMng  : 
	//		CStringList* pAllProperties   : 
	// 함수 설명          : CBaseDesk2의 포인터를 넘겨준다.
	//                      데스크에서 넘오는 SetAllProperties 정보를 그대로 넘겨준다.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void Platform_Initialize(long dwMainDllObj, long dwBaseDesk, long dwRscMng, CStringList* pAllProperties);

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/2/19
	// 함수 이름          : Platform_SetProperty
	// 리턴 타입          : void
	// 파라미터
	//		STDATAITEM *stData  : 
	//		int nDataCnt   : 
	// 함수 설명          : DLLOCX에서 어떤 값을 넘겨주고자 할 때
	///////////////////////////////////////////////////////////////////////////////
	virtual void Platform_SetProperty(STDATAITEM *stData, int nDataCnt);

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/2/19
	// 함수 이름          : Platform_GetState
	// 리턴 타입          : long
	// 리턴 타입 설명     : 
	// 파라미터
	//		int nStatus  : 
	//		long dwData1  : 
	//		long dwData2   : 
	// 함수 설명          : DLLOCX를 통해 상태정보를 넘겨받는다.
	///////////////////////////////////////////////////////////////////////////////
	virtual long Platform_GetState(int nStatus, long dwData1, long dwData2);
	virtual long Platform_SetState(int nStatus, long dwData1, long dwData2);

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/2/19
	// 함수 이름          : Platform_WindowProc
	// 리턴 타입          : LRESULT
	// 리턴 타입 설명     : 
	// 파라미터
	//		UINT message  : 
	//		WPARAM wParam  : 
	//		LPARAM lParam  : 
	//		BOOL &bContinue   : 
	// 함수 설명          : DLL_OCX에서 발생하는 메시지를 그대로 준다.
	//                      nContinue 0 : 메시지 처리함. 리턴된 값으로 DLL_OCX에서 리턴하기 원함.
	//                                1 : 처리할 메시지 없음
	//                                2 : 메시지 처리함. 리턴값 의미 없음. DLL_OCX에서 메시지 계속처리.
	//	message가 RMSG_FORMINITIALCOMPLETED 일경우 폼이 로드가 다 된후에 불리는 메시지.
	//  지수옵션차익기회의 컨트롤들에 값을 세팅하고 TR을 보내도록 데스크에 요청한다.
	//  "DLL로더 OCX.doc" 파일참조.
	///////////////////////////////////////////////////////////////////////////////
	virtual LRESULT Platform_WindowProc(UINT message, WPARAM wParam, LPARAM lParam, int &nContinue);
	virtual LRESULT Platform_PreTranslateMessage(MSG* pMsg);
	
	virtual void Platform_OnSize(UINT nType, int cx, int cy);

	virtual int Packet_GetData(char* buf, int bufLen);

	virtual int Packet_GetPackDataSize(char* pIn);
	virtual int Packet_SetData(char* buf, int bufLen);
	virtual int Packet_SetData2(char* buf, int bufLen);
	virtual int Packet_SetData3(char* buf, int bufLen);
	virtual int Packet_SetData999(int nType, char* buf, int bufLen);

	virtual void OnGSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpGSharedData, BOOL bWithStart);
	virtual void OnLSharedDataChange(LPCTSTR lpSharedName, LPCTSTR lpLSharedData, BOOL bWithStart);
	
	void	ListupControlHandle(CWnd* pParent);
	BOOL	GotoNextWindowFocus(UINT nKey, BOOL bShift);

private:
	CMapStringToString*		m_pMapParam1;
	CMapStringToString*		m_pMapParam2;
	CMapStringToString*		m_pMapParam3;

	HWND					m_hChildFrame;
	HWND					m_hFocusChild;
	CArray<HWND, HWND>		m_arChildHandle;
	UINT					m_nChildHandlePos;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExOCXDLL)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExOCXDLL)
	virtual void OnCancel(){}
	virtual void OnOK(){}
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	LRESULT OnLShareChanged(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

AFX_INLINE CResourceManager* CExOCXDLL::GetResourceManager() {return m_pResMng;}
AFX_INLINE HWND CExOCXDLL::GetFrameHandle()
{
	if(!m_hChildFrame)
		m_hChildFrame = (HWND)m_pBaseDesk->GetFrameInfo(_T("FRAMEHADLE"));

	return m_hChildFrame;
}

//static CBaseDesk2* m_pBaseDesk;
//static IGateManager* m_pIGateManager;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXOCXDLL_H__77308CF7_9EA9_4613_A2F1_40AC279DDBF6__INCLUDED_)
