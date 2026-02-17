#pragma once

// CertifyCtrl.h : CCertifyCtrl ActiveX 컨트롤 클래스의 선언입니다.

#include <afxmt.h>
#include "CaLib/interfaceDLL.h"

// CCertifyCtrl : 구현에 대해서는 CertifyCtrl.cpp을(를) 참조하세요.

class CCertifyCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCertifyCtrl)

// 생성자입니다.
public:
	CCertifyCtrl();

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 구현입니다.
private:
	CString		m_name;
	APP_CONTEXT	m_context;
	SD_API_CONTEXT_NEW m_contextNew;
	enum {
		dispidCertifyCloud = 8L,
		caNO, caNOx, caOK, caRUN, caPWD, caPWDa, caOKx } m_ca;

	int		m_nBytes;
	char		m_pBytes[16 * 1024];
	CCriticalSection m_sync;

	enum	resNO { rspNULL = -1, rspMODAL = 0, rspOK = 1, rspISSUE = 2, rspREISSUE = 3, rspPASSWD = 4, rspOTHER = 5 };
	enum	msgNO { msg0 = 0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15, msg16, msg17, msg18, msg19 };

	bool		m_auto;
	bool		m_calogon;
	CString		m_user;
	CString		m_certifys;
	CString		m_string;
	char		m_encpass[32 + 1];	// encrypted passwd

	CString		m_msg;
	//	class	CPassInput*	m_passDlg;

	CMapStringToPtr	m_emaps;

public:     //cloude
	BOOL	m_bCloudeUse{};
	APP_CONTEXT m_cpContext;
	SD_API_CONTEXT_NEW m_cpContextNew;
	BOOL	m_bDev;
	CString m_slog{};
	CString m_root;
	BOOL	m_bCloudeInit{};

	void InitCloude();
	BOOL CheckCloude();
	int Cloude_ConTraction_sign(long pOutB, long pOutL);
	int Cloude_Full_sign(long pOutB, long pOutL);
	int TEST_Cloude_ConTraction_sign();
private:
	bool	sign();
	int	queryDn(CString dn_name, int* nBytes, bool retry = false);
	void	savePasswd();
	BOOL	checkPasswd(CString pass);
	CString	checkPasswd();
	bool	certify(bool reissue = false);
	void	otherCertificate();
	void	removeCertificate();
	bool	guideMsg(msgNO msgno, CString guide = _T(""), CString title = _T(""));
	BOOL	isMustCertify(CString maps);

protected:
	~CCertifyCtrl();

	DECLARE_OLECREATE_EX(CCertifyCtrl)    // 클래스 팩터리와 GUID입니다.
	DECLARE_OLETYPELIB(CCertifyCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCertifyCtrl)     // 속성 페이지 ID입니다.
	DECLARE_OLECTLTYPE(CCertifyCtrl)		// 형식 이름과 기타 상태입니다.

// 메시지 맵입니다.
	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT	OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// 디스패치 맵입니다.
	afx_msg long OnCertify(long pBytes, long nBytes);
	afx_msg BOOL Certify(long pBytes, long nBytes, long infos);
	afx_msg BOOL CertifyErr(long pBytes, long nBytes);
	afx_msg void CertifyId(long pBytes);
	afx_msg BOOL CertifyEx(long pBytes, long nBytes);
	afx_msg long CertifyFull(long pInB, long pInL, long pOutB, long pOutL);
	afx_msg long CertifyName(long pBytes);
	DECLARE_DISPATCH_MAP()

// 이벤트 맵입니다.
	void OnFire(long type, long wParam, long lParam)
	{
		FireEvent(eventidOnFire, EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), type, lParam, wParam);
	}
	DECLARE_EVENT_MAP()

// 디스패치와 이벤트 ID입니다.
public:
	enum {
		dispidOnCertify = 1L,
		dispidCertify = 2L,
		dispidCertifyErr = 3L,
		dispidCertifyId = 4L,
		dispidCertifyEx = 5L,
		dispidCertifyFull = 6L,
		dispidCertifyName = 7L,

		eventidOnFire = 1L,
	};
protected:
	LONG CertifyCloud(LONG func);
};

