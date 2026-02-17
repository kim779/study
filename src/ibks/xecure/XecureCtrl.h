#pragma once

#include <afxmt.h>
#include "Xecure/xc_main.h"
constexpr int maxlen = 1024 * 64;

// XecureCtrl.h : CXecureCtrl ActiveX 컨트롤 클래스의 선언입니다.
// CXecureCtrl : 구현에 대해서는 XecureCtrl.cpp을(를) 참조하세요.

class CXecureCtrl : public COleControl
{
	DECLARE_DYNCREATE(CXecureCtrl)

// 생성자입니다.
public:
	CXecureCtrl();

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 구현입니다.
private:
	XC_CTX	m_ctx;
	enum class stepENC { encHELLO, encOK, encERR, encRUN } m_enc;

	int	m_nBytes;
	char	m_pBytes[maxlen + 1]{};
	CCriticalSection m_sync;

	class	CLog* m_log;

protected:
	~CXecureCtrl();

	DECLARE_OLECREATE_EX(CXecureCtrl)    // 클래스 팩터리와 GUID입니다.
	DECLARE_OLETYPELIB(CXecureCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CXecureCtrl)     // 속성 페이지 ID입니다.
	DECLARE_OLECTLTYPE(CXecureCtrl)		// 형식 이름과 기타 상태입니다.

// 메시지 맵입니다.
	DECLARE_MESSAGE_MAP()

// 디스패치 맵입니다.
	afx_msg BOOL Encrypt(long pBytes, long nBytes);
	afx_msg BOOL Decrypt(long pBytes, long nBytes);
	afx_msg long Xecure(long pBytes, long nBytes);

	DECLARE_DISPATCH_MAP()

// 이벤트 맵입니다.
	DECLARE_EVENT_MAP()

// 디스패치와 이벤트 ID입니다.
public:
	enum {
	dispidEncrypt = 1L,
	dispidDecrypt = 2L,
	dispidXecure = 3L,
	};
};

