#pragma once
#include "Guard.h"
#include "Works.h"

// CEmulCtrl
class AFX_EXT_CLASS CEmulCtrl : public CWnd
{
	DECLARE_DYNAMIC(CEmulCtrl)

public:
	CEmulCtrl();
	virtual ~CEmulCtrl();

	CWnd*	m_pSocket;
	class CGuard*	m_pGuard;
	BOOL axTR(char* pBytes, long nBytes);
	long axAttach(CWnd* view, long type, long key);
	void axDetach(long key);
	BOOL axWorkShop(long key, CString maps, long size, BOOL only) ;
	long axWorkView(LPCTSTR maps, long* size);
	long axRunView(CWnd *view, LPCTSTR maps, int nID, long* size);
	void Run();
	BOOL RunAxis(long mode, LONG_PTR pBytes, long nBytes);
	LONG_PTR axWizard(long kind, LONG_PTR variant);


	void	OnAlert(char* pBytes, int nBytes);
	void	OnAlert(int type, char* pBytes, int nBytes);
	void	OnPush(char* pBytes, int nBytes);
	void	OnRead(char* pBytes, int nBytes);
	BOOL	OnAxis(int type, char* pBytes, int variant);
	void	OnSign(int signK, char *pBytes, int nBytes);
	bool	OnAux(struct _axisH* axisH, char* &pBytes, int& nBytes);
	void	OnAxis(struct _axisH* axisH, char *pBytes, int nBytes);


	void OnFire(long type, WPARAM wParam, LPARAM lParam)
	{	
		::AfxGetMainWnd()->SendMessage(WM_USER + 700, MAKEWPARAM(type, wParam), lParam);
		/*FireEvent(eventidOnFire,EVENT_PARAM(VTS_I4  VTS_I8  VTS_I8), type, wParam, lParam);*/	
	}

protected:
	enum	{ mtNO, mtCON, mtSIGN, mtCA, mtXEC, mtMENU, mtRUN }	m_mode;
	enum	{ xtWS, xtRM, xtXEC}	m_xtype;


protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg LRESULT OnStream(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg	LRESULT OnFireEvent(WPARAM wParam, LPARAM lParam);
	LRESULT OnFireAxis(WPARAM wParam, LPARAM lParam);
	LRESULT OnGuide(WPARAM wParam, LPARAM lParam);
	LRESULT OnFireAlert(WPARAM wParam, LPARAM lParam);
	LRESULT OnLuaSend(WPARAM wParam, LPARAM lParam);
	LRESULT OnLuaObj(WPARAM wParam, LPARAM lParam);
	LRESULT	OnLuaGetName(WPARAM wParam, LPARAM lParam);
	LRESULT	OnLuaGetMain(WPARAM wParam, LPARAM lParam);
};


