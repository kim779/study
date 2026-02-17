#pragma once

// WizardCtrl.h : CWizardCtrl ActiveX 컨트롤 클래스의 선언입니다.


// CWizardCtrl : 구현에 대해서는 WizardCtrl.cpp을(를) 참조하세요.

class CWizardCtrl : public COleControl
{
	DECLARE_DYNCREATE(CWizardCtrl)

// 생성자입니다.
public:
	CWizardCtrl();
	CString m_slog;
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 구현입니다.
protected:
	class CGuard* m_guard;
	enum class mtFlag { mtNO, mtCON, mtSIGN, mtCA, mtXEC, mtMENU, mtRUN }	m_mode;
	enum class xtFlag { xtWS, xtRM, xtXEC }	m_xtype;

private:
	int	m_axisL;
	char* m_axisB;

private:
	void	OnAlert(char* pBytes, int nBytes);
	void	OnAlert(int type, char* pBytes, int nBytes);
	void	OnPush(char* pBytes, int nBytes);
	void	OnRead(char* pBytes, int nBytes);
	BOOL	OnAxis(int type, char* pBytes, int variant);

	void	OnAxis(struct _axisH* axisH, char* pBytes, int nBytes);
	void	OnStream(struct _axisH* axisH, char* pBytes, int nBytes);
	bool	OnAux(struct _axisH* axisH, char*& pBytes, int& nBytes);
	void	OnXecure(int encK, char* pBytes, int nBytes);
	void	OnSign(int signK, char* pBytes, int nBytes);
	void	OnCertify(char* pBytes, int nBytes);

	void	Xecure();
	int	Xecure(char* pBytes, int nBytes);
	void	Run();
protected:
	~CWizardCtrl();

	DECLARE_OLECREATE_EX(CWizardCtrl)    // 클래스 팩터리와 GUID입니다.
	DECLARE_OLETYPELIB(CWizardCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWizardCtrl)     // 속성 페이지 ID입니다.
	DECLARE_OLECTLTYPE(CWizardCtrl)		// 형식 이름과 기타 상태입니다.

// 메시지 맵입니다.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LONG OnFireAxis(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGuide(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnVersion(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnFireAlert(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	afx_msg	void OnFireEvent(int type, char* pBytes, int nBytes);
	DECLARE_EVENTSINK_MAP()

// 디스패치 맵입니다.
	afx_msg BOOL RunAxis(long mode, long pBytes, long nBytes);
	afx_msg BOOL axTR(long pBytes, long nBytes);
	afx_msg long axAttach(long view, long type, long key);
	afx_msg void axDetach(long key);
	afx_msg BOOL axWorkShop(long key, LPCTSTR maps, long size, BOOL only);
	afx_msg long axWorkName(long key);
	afx_msg long axWorkView(LPCTSTR maps, long size);
	afx_msg long axWizard(long kind, long variant);
	afx_msg long axInvoke(long wParam, long lParam);
	DECLARE_DISPATCH_MAP()

// 이벤트 맵입니다.
	void OnFire(long type, long wParam, long lParam)
	{
		FireEvent(eventidOnFire, EVENT_PARAM(VTS_I4  VTS_I4  VTS_I4), type, wParam, lParam);
	}
	DECLARE_EVENT_MAP()

// 디스패치와 이벤트 ID입니다.
public:
	enum {
	dispidRunAxis = 1L,
	dispidAxTR = 2L,
	dispidAxAttach = 3L,
	dispidAxDetach = 4L,
	dispidAxWorkShop = 5L,
	dispidAxWorkName = 6L,
	dispidAxWorkView = 7L,
	dispidAxWizard = 8L,
	dispidAxInvoke = 9L,
	eventidOnFire = 1L,
	};
};

