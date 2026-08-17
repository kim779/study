#pragma once

// SockCtrl.h : CSockCtrl ActiveX 컨트롤 클래스의 선언입니다.


// CSockCtrl : 구현에 대해서는 SockCtrl.cpp을(를) 참조하세요.

class CSockCtrl : public COleControl
{
	DECLARE_DYNCREATE(CSockCtrl)

// 생성자입니다.
public:
	CSockCtrl();

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 구현입니다.
private:
	class CDgram* m_dgram;
	class CStream* m_stream;
	class CWsh* m_rsm;
	class CWsh* m_wsh;

protected:
	~CSockCtrl();

	DECLARE_OLECREATE_EX(CSockCtrl)    // 클래스 팩터리와 GUID입니다.
	DECLARE_OLETYPELIB(CSockCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CSockCtrl)     // 속성 페이지 ID입니다.
	DECLARE_OLECTLTYPE(CSockCtrl)		// 형식 이름과 기타 상태입니다.

// 메시지 맵입니다.
	afx_msg LONG OnFireEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFirePush(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFireAxis(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFireStat(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFireWrite(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFireVersion(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

// 디스패치 맵입니다.
	afx_msg BOOL OpenSock(LPCTSTR home, LPCTSTR ip, long port);
	afx_msg void CloseSock();
	afx_msg BOOL WriteStream(long pBytes, long nBytes);
	afx_msg BOOL OpenDgram(long port);
	afx_msg void CloseDgram();
	afx_msg BOOL WriteDgram(LPCTSTR ip, long pBytes, long nBytes, BOOL route);
	afx_msg BOOL StartAxis(long key, long mode);
	afx_msg long GetEnum(long pBytes, long nBytes, long mode);

	DECLARE_DISPATCH_MAP()

// 이벤트 맵입니다.
	void OnStream(long type, long pBytes, long nBytes)
	{
		FireEvent(eventidOnStream, EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), type, pBytes, nBytes);
	}
	DECLARE_EVENT_MAP()

// 디스패치와 이벤트 ID입니다.
public:
	enum {
	dispidOpenSock = 1L,
	dispidCloseSock = 2L,
	dispidWriteStream = 3L,
	dispidOpenDgram = 4L,
	dispidCloseDgram = 5L,
	dispidWriteDgram = 6L,
	dispidStartAxis = 7L,
	dispidGetEnum = 8L,
	eventidOnStream = 1L,
	};
};

